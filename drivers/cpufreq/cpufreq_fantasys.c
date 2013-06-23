/*
 * drivers/cpufreq/cpufreq_fantasys.c
 *
 * copyright (c) 2012-2013 allwinnertech
 *
 * based on ondemand policy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
#include <linux/cpu.h>
#include <linux/jiffies.h>
#include <linux/kernel_stat.h>
#include <linux/mutex.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/threads.h>
#include <linux/suspend.h>
#include <linux/delay.h>


/*
 * dbs is used in this file as a shortform for demandbased switching
 * It helps to keep variable names smaller, simpler
 */
#define FANTASY_DEBUG_LEVEL     (3)

#if (FANTASY_DEBUG_LEVEL == 0 )
    #define FANTASY_DBG(format,args...)
    #define FANTASY_WRN(format,args...)
    #define FANTASY_ERR(format,args...)
#elif(FANTASY_DEBUG_LEVEL == 1)
    #define FANTASY_DBG(format,args...)
    #define FANTASY_WRN(format,args...)
    #define FANTASY_ERR(format,args...)     printk(KERN_ERR "[fantasy] err "format,##args)
#elif(FANTASY_DEBUG_LEVEL == 2)
    #define FANTASY_DBG(format,args...)
    #define FANTASY_WRN(format,args...)     printk(KERN_WARNING "[fantasy] wrn "format,##args)
    #define FANTASY_ERR(format,args...)     printk(KERN_ERR "[fantasy] err "format,##args)
#elif(FANTASY_DEBUG_LEVEL == 3)
    #define FANTASY_DBG(format,args...)     printk(KERN_DEBUG "[fantasy] dbg "format,##args)
    #define FANTASY_WRN(format,args...)     printk(KERN_WARNING "[fantasy] wrn "format,##args)
    #define FANTASY_ERR(format,args...)     printk(KERN_ERR "[fantasy] err "format,##args)
#endif


#define DEF_FREQUENCY_DOWN_DIFFERENTIAL (5)
#define DEF_FREQUENCY_UP_THRESHOLD      (85)

#define DEF_CPU_UP_RATE                 (10)    /* check cpu up period is 10*sample_rate=0.5 second */
#define DEF_CPU_DOWN_RATE               (40)    /* check cpu down period is 40*sample_rate=2 second */
#define DEF_FREQ_UP_RATE                (20)    /* check cpu frequency up period is 20*sample_rate=1 second     */
#define DEF_FREQ_DOWN_RATE              (40)    /* check cpu frequency down period is 40*sample_rate=2 second   */
#define MAX_HOTPLUG_RATE                (80)    /* array size for save history sample data          */

#define DEF_MAX_CPU_LOCK                (0)
#define DEF_START_DELAY                 (0)

#define MIN_FREQUENCY_UP_THRESHOLD      (11)
#define MAX_FREQUENCY_UP_THRESHOLD      (100)
#define DEF_SAMPLING_RATE               (50000) /* check cpu frequency sample rate is 50ms */

#define SINGLE_CPU_DOWN_LOADING         (30)
#define SINGLE_CPU_DOWN_RUNNING         (200)

/*
 * static data of cpu usage
 */
struct cpu_usage {
    unsigned int freq;              /* cpu frequency value              */
    unsigned int loading[NR_CPUS];  /* cpu frequency loading            */
    unsigned int running[NR_CPUS];  /* cpu running list loading         */
    unsigned int iowait[NR_CPUS];   /* cpu waiting                      */
    unsigned int loading_avg;       /* system average freq loading      */
    unsigned int running_avg;       /* system average thread loading    */
    unsigned int iowait_avg;        /* system average waiting           */

};

/* record cpu history loading information */
struct cpu_usage_history {
    struct cpu_usage usage[MAX_HOTPLUG_RATE];
    unsigned int num_hist;          /* current number of history data   */
};
static struct cpu_usage_history *hotplug_history;


/*
 * get average loading of all cpu's run list
 */
static unsigned int get_rq_avg_sys(void)
{
    return nr_running_avg();
}

/*
 * get average loading of spec cpu's run list
 */
static unsigned int get_rq_avg_cpu(int cpu)
{
    return nr_running_avg_cpu(cpu);
}



/*
 ----------------------------------------------------------
 define policy table for cpu-hotplug
 ----------------------------------------------------------
 */
/*
 * define thread loading policy table for cpu hotplug
 * if(rq > hotplug_rq[x][1]) cpu hotplug in;
 * if(rq < hotplug_rq[x][0]) cpu hotplug out;
 */
static int hotplug_rq_def[4][2] = {
    {0  , 200},
    {150, 300},
    {250, 400},
    {350, 0  },
};

/*
 * define frequncy loading policy table for cpu hotplug
 */
static int hotplug_freq_def[4][2] = {
    {0 , 80},    /* if(freq > policy->max * 80%) cpu hotplug in */
    {40, 80},    /* if(freq > policy->max * 80%) cpu hotplug in, if(freq < policy->max * 40%) cpu hotplug out */
    {50, 80},    /* if(freq > policy->max * 80%) cpu hotplug in, if(freq < policy->max * 50%) cpu hotplug out */
    {60, 0 },    /* if(freq < policy->max * 60%) cpu hotplug out */
};

#ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
/*
 * define frequency policy table for user event triger
 */
static int usrevent_freq_def[4] = {
    100,        /* switch cpu frequency to policy->max * 100% if single core currently */
    90 ,        /* switch cpu frequency to policy->max * 90% if dule core currently    */
    80 ,        /* switch cpu frequency to policy->max * 80% if triple core currently  */
    80 ,        /* switch cpu frequency to policy->max * 80% if quad core currently    */
};
#endif


static int hotplug_rq[NR_CPUS][2];
static int hotplug_freq[NR_CPUS][2];

#ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
static int usrevent_freq[NR_CPUS];
#endif

/*
 * define data structure for dbs
 */
struct cpu_dbs_info_s {
    u64 prev_cpu_idle;          /* previous idle time statistic */
    u64 prev_cpu_iowait;        /* previous iowait time stat    */
    u64 prev_cpu_wall;          /* previous total time stat     */
    u64 prev_cpu_nice;          /* previous nice time           */
    struct cpufreq_policy *cur_policy;
    struct delayed_work work;
    struct work_struct up_work;     /* cpu plug-in processor    */
    struct work_struct down_work;   /* cpu plug-out processer   */
    struct cpufreq_frequency_table *freq_table;
    unsigned int rate_mult;
    int cpu;                    /* current cpu number           */
    /*
     * percpu mutex that serializes governor limit change with
     * do_dbs_timer invocation. We do not want do_dbs_timer to run
     * when user is changing the governor or limits.
     */
    struct mutex timer_mutex;   /* semaphore for protection     */
};

/*
 * define percpu variable for dbs information
 */
static DEFINE_PER_CPU(struct cpu_dbs_info_s, od_cpu_dbs_info);
struct workqueue_struct *dvfs_workqueue;
static int cpufreq_governor_dbs(struct cpufreq_policy *policy, unsigned int event);

/* number of CPUs using this policy */
static unsigned int dbs_enable;

/*
 * dbs_mutex protects dbs_enable in governor start/stop.
 */
static DEFINE_MUTEX(dbs_mutex);


static struct dbs_tuners {
    unsigned int sampling_rate;     /* dvfs sample rate                             */
    unsigned int up_threshold;      /* cpu freq up threshold, up freq if higher     */
    unsigned int down_threshold;    /* cpu freq down threshold, down freq if lower  */
    unsigned int ignore_nice;       /* flag to mark if need calculate the nice time */
    unsigned int io_is_busy;        /* flag to mark if iowait calculate as cpu busy */

    /* pegasusq tuners */
    unsigned int cpu_up_rate;       /* history sample rate for cpu up               */
    unsigned int cpu_up_rate_adj;   /* sample rate adjusted value for cpu up        */
    unsigned int cpu_down_rate;     /* history sample rate for cpu down             */
    unsigned int freq_up_rate;      /* history sample rate for cpu frequency up     */
    unsigned int freq_down_rate;    /* history sample rate for cpu frequency down   */
    unsigned int freq_down_delay;   /* delay sample rate for cpu frequency down     */
    unsigned int max_cpu_lock;      /* max count of online cpu, user limit          */
    atomic_t hotplug_lock;          /* lock cpu online number, disable plug-in/out  */
    unsigned int dvfs_debug;        /* dvfs debug flag, print dbs information       */
} dbs_tuners_ins = {
    .up_threshold = DEF_FREQUENCY_UP_THRESHOLD,
    .down_threshold = DEF_FREQUENCY_DOWN_DIFFERENTIAL,
    .ignore_nice = 0,
    .cpu_up_rate = DEF_CPU_UP_RATE,
    .cpu_up_rate_adj = DEF_CPU_UP_RATE,
    .cpu_down_rate = DEF_CPU_DOWN_RATE,
    .freq_up_rate = DEF_FREQ_UP_RATE,
    .freq_down_rate = DEF_FREQ_DOWN_RATE,
    .freq_down_delay = 0,
    .max_cpu_lock = DEF_MAX_CPU_LOCK,
    .hotplug_lock = ATOMIC_INIT(0),
    .dvfs_debug = 0,
};


struct cpufreq_governor cpufreq_gov_fantasys = {
    .name                   = "fantasys",
    .governor               = cpufreq_governor_dbs,
    .owner                  = THIS_MODULE,
};


/*
 * CPU hotplug lock interface
 */
atomic_t g_hotplug_lock = ATOMIC_INIT(0);

/*
 * CPU max power flag
 */
atomic_t g_max_power_flag = ATOMIC_INIT(0);

/*
 * apply cpu hotplug lock, up or down cpu
 */
static void apply_hotplug_lock(void)
{
    int online, possible, lock, flag;
    struct work_struct *work;
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    online = num_online_cpus();
    possible = num_possible_cpus();
    lock = atomic_read(&g_hotplug_lock);
    flag = lock - online;

    if (flag == 0)
        return;

    work = flag > 0 ? &dbs_info->up_work : &dbs_info->down_work;

    FANTASY_DBG("%s online:%d possible:%d lock:%d flag:%d %d\n",
         __func__, online, possible, lock, flag, (int)abs(flag));

    queue_work_on(dbs_info->cpu, dvfs_workqueue, work);
}

/*
 * lock cpu number, the number of onlie cpu should less then num_core
 */
int cpufreq_fantasys_cpu_lock(int num_core)
{
    int prev_lock;
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    mutex_lock(&dbs_info->timer_mutex);

    if (num_core < 1 || num_core > num_possible_cpus()) {
        mutex_unlock(&dbs_info->timer_mutex);
        return -EINVAL;
    }

    prev_lock = atomic_read(&g_hotplug_lock);
    if (prev_lock != 0 && prev_lock < num_core) {
        mutex_unlock(&dbs_info->timer_mutex);
        return -EINVAL;
    }

    atomic_set(&g_hotplug_lock, num_core);
    apply_hotplug_lock();
    mutex_unlock(&dbs_info->timer_mutex);

    return 0;
}

/*
 * unlock cpu hotplug number
 */
int cpufreq_fantasys_cpu_unlock(int num_core)
{
    int prev_lock = atomic_read(&g_hotplug_lock);
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    mutex_lock(&dbs_info->timer_mutex);

    if (prev_lock != num_core) {
        mutex_unlock(&dbs_info->timer_mutex);
        return -EINVAL;
    }

    atomic_set(&g_hotplug_lock, 0);
    mutex_unlock(&dbs_info->timer_mutex);

    return 0;
}


/* cpufreq_pegasusq Governor Tunables */
#define show_one(file_name, object)                    \
static ssize_t show_##file_name                        \
(struct kobject *kobj, struct attribute *attr, char *buf)        \
{                                    \
    return sprintf(buf, "%u\n", dbs_tuners_ins.object);        \
}
show_one(sampling_rate, sampling_rate);
show_one(io_is_busy, io_is_busy);
show_one(up_threshold, up_threshold);
show_one(down_threshold, down_threshold);
show_one(cpu_up_rate, cpu_up_rate);
show_one(cpu_down_rate, cpu_down_rate);
show_one(max_cpu_lock, max_cpu_lock);
show_one(dvfs_debug, dvfs_debug);
static ssize_t show_hotplug_lock(struct kobject *kobj, struct attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", atomic_read(&g_hotplug_lock));
}

static ssize_t show_max_power(struct kobject *kobj, struct attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", atomic_read(&g_max_power_flag));
}

static ssize_t store_sampling_rate(struct kobject *a, struct attribute *b, const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.sampling_rate = input;
    return count;
}

static ssize_t store_io_is_busy(struct kobject *a, struct attribute *b, const char *buf, size_t count)
{
    unsigned int input;
    int ret;

    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;

    dbs_tuners_ins.io_is_busy = !!input;
    return count;
}

static ssize_t store_up_threshold(struct kobject *a, struct attribute *b,
                  const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);

    if (ret != 1 || input > MAX_FREQUENCY_UP_THRESHOLD ||
        input < MIN_FREQUENCY_UP_THRESHOLD) {
        return -EINVAL;
    }
    dbs_tuners_ins.up_threshold = input;
    return count;
}

static ssize_t store_down_threshold(struct kobject *a, struct attribute *b,
                       const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.down_threshold = min(input, 100u);
    return count;
}

static ssize_t store_cpu_up_rate(struct kobject *a, struct attribute *b,
                 const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.cpu_up_rate = (unsigned int)min((int)input, (int)MAX_HOTPLUG_RATE);
    return count;
}

static ssize_t store_cpu_down_rate(struct kobject *a, struct attribute *b,
                   const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.cpu_down_rate = (unsigned int)min((int)input, (int)MAX_HOTPLUG_RATE);
    return count;
}

static ssize_t store_max_cpu_lock(struct kobject *a, struct attribute *b,
                  const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.max_cpu_lock = min(input, num_possible_cpus());
    return count;
}

static ssize_t store_hotplug_lock(struct kobject *a, struct attribute *b,
                  const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    int prev_lock;

    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    input = min(input, num_possible_cpus());
    prev_lock = atomic_read(&dbs_tuners_ins.hotplug_lock);

    if (prev_lock)
        cpufreq_fantasys_cpu_unlock(prev_lock);

    if (input == 0) {
        atomic_set(&dbs_tuners_ins.hotplug_lock, 0);
        return count;
    }

    ret = cpufreq_fantasys_cpu_lock(input);
    if (ret) {
        printk(KERN_ERR "[HOTPLUG] already locked with smaller value %d < %d\n",
            atomic_read(&g_hotplug_lock), input);
        return ret;
    }

    atomic_set(&dbs_tuners_ins.hotplug_lock, input);

    return count;
}

static ssize_t store_dvfs_debug(struct kobject *a, struct attribute *b,
                const char *buf, size_t count)
{
    unsigned int input;
    int ret;
    ret = sscanf(buf, "%u", &input);
    if (ret != 1)
        return -EINVAL;
    dbs_tuners_ins.dvfs_debug = input > 0;
    return count;
}

static ssize_t store_max_power(struct kobject *a, struct attribute *b,
                const char *buf, size_t count)
{
    unsigned int input;
    int ret, nr_up, cpu, max_power_flag;
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    mutex_lock(&dbs_info->timer_mutex);
    ret = sscanf(buf, "%u", &input);
    if (ret != 1) {
        mutex_unlock(&dbs_info->timer_mutex);
        return -EINVAL;
    }

    atomic_set(&g_max_power_flag, !!input);
    max_power_flag = atomic_read(&g_max_power_flag);
    if (max_power_flag == 1) {
        __cpufreq_driver_target(dbs_info->cur_policy, dbs_info->cur_policy->max, CPUFREQ_RELATION_H);
        nr_up = nr_cpu_ids - num_online_cpus();
        for_each_cpu_not(cpu, cpu_online_mask) {
            if (cpu == 0)
                continue;

            if (nr_up-- == 0)
                break;

            cpu_up(cpu);
        }
    } else if (max_power_flag == 0) {
        hotplug_history->num_hist = 0;
        queue_delayed_work_on(dbs_info->cpu, dvfs_workqueue, &dbs_info->work, 0);
    } else {
        mutex_unlock(&dbs_info->timer_mutex);
        return -EINVAL;
    }
    mutex_unlock(&dbs_info->timer_mutex);

    return count;
}

define_one_global_rw(sampling_rate);
define_one_global_rw(io_is_busy);
define_one_global_rw(up_threshold);
define_one_global_rw(down_threshold);
define_one_global_rw(cpu_up_rate);
define_one_global_rw(cpu_down_rate);
define_one_global_rw(max_cpu_lock);
define_one_global_rw(hotplug_lock);
define_one_global_rw(dvfs_debug);
define_one_global_rw(max_power);

static struct attribute *dbs_attributes[] = {
    &sampling_rate.attr,
    &up_threshold.attr,
    &io_is_busy.attr,
    &down_threshold.attr,
    &cpu_up_rate.attr,
    &cpu_down_rate.attr,
    &max_cpu_lock.attr,
    &hotplug_lock.attr,
    &dvfs_debug.attr,
    &max_power.attr,
    NULL
};

static struct attribute_group dbs_attr_group = {
    .attrs = dbs_attributes,
    .name = "fantasys",
};


/*
 * cpu hotplug, just plug in one cpu
 */
static void cpu_up_work(struct work_struct *work)
{
    int cpu, nr_up, online, hotplug_lock;
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    mutex_lock(&dbs_info->timer_mutex);
    online = num_online_cpus();
    hotplug_lock = atomic_read(&g_hotplug_lock);

    if (hotplug_lock) {
        nr_up = (hotplug_lock - online) > 0? (hotplug_lock-online) : 0;
    } else {
        nr_up = 1;
    }

    for_each_cpu_not(cpu, cpu_online_mask) {
        if (cpu == 0)
            continue;

        if (nr_up-- == 0)
            break;

        FANTASY_WRN("cpu up:%d\n", cpu);
        cpu_up(cpu);
    }
    mutex_unlock(&dbs_info->timer_mutex);
}

/*
 * cpu hotplug, cpu plugout
 */
static void cpu_down_work(struct work_struct *work)
{
    int cpu, nr_down, online, hotplug_lock;
    struct cpu_dbs_info_s *dbs_info;

    dbs_info = &per_cpu(od_cpu_dbs_info, 0);
    mutex_lock(&dbs_info->timer_mutex);
    online = num_online_cpus();
    hotplug_lock = atomic_read(&g_hotplug_lock);

    if (hotplug_lock) {
        nr_down = (online - hotplug_lock) > 0? (online-hotplug_lock) : 0;
    } else {
        nr_down = 1;
    }

    for_each_online_cpu(cpu) {
        if (cpu == 0)
            continue;

        if (nr_down-- == 0)
            break;

        FANTASY_DBG("cpu down:%d\n", cpu);
        cpu_down(cpu);
    }
    mutex_unlock(&dbs_info->timer_mutex);
}


/*
 * check if need plug in one cpu core
 */
static int check_up(struct cpufreq_policy *policy)
{
    struct cpu_usage *usage;
    int i, online, freq, rq_avg, up_freq, up_rq, up_rate;
    int min_freq = INT_MAX, min_rq_avg = INT_MAX;
    int num_hist = hotplug_history->num_hist;
    int hotplug_lock = atomic_read(&g_hotplug_lock);

    if(dbs_tuners_ins.cpu_up_rate_adj != dbs_tuners_ins.cpu_up_rate)
        up_rate = dbs_tuners_ins.cpu_up_rate_adj;
    else
        up_rate = dbs_tuners_ins.cpu_up_rate;

    /* hotplug has been locked, do nothing */
    if (hotplug_lock > 0)
        return 0;

    online = num_online_cpus();
    up_freq = policy->max * hotplug_freq[online-1][1];
    up_rq = hotplug_rq[online-1][1];

    /* check if count of the cpu reached the max value */
    if (online == num_possible_cpus()) {
        return 0;
    }
    if (dbs_tuners_ins.max_cpu_lock != 0 && online >= dbs_tuners_ins.max_cpu_lock) {
        return 0;
    }

    /* check if reached the switch point */
    if (num_hist == 0 || num_hist % up_rate) {
        return 0;
    }

    /* clear adjust value */
    dbs_tuners_ins.cpu_up_rate_adj = dbs_tuners_ins.cpu_up_rate;

    /* check system average loading */
    for (i = num_hist - 1; i >= num_hist - up_rate; --i) {
        usage = &hotplug_history->usage[i];

        freq = usage->freq * usage->loading_avg;
        rq_avg = usage->running_avg;

        min_freq = min(min_freq, freq);
        min_rq_avg = min(min_rq_avg, rq_avg);
    }

    if (dbs_tuners_ins.dvfs_debug) {
        printk("%s: min_freq:%u, up_freq:%u, min_rq_avg:%u, up_rq:%u\n",    \
                __func__, min_freq, up_freq, min_rq_avg, up_rq);
    }

    if (min_freq >= up_freq && min_rq_avg > up_rq) {
        FANTASY_WRN("cpu need plugin, freq: %d>=%d && rq: %d>%d\n", min_freq, up_freq, min_rq_avg, up_rq);

        for(i=online; i<num_possible_cpus(); i++) {
            if(min_rq_avg > hotplug_rq[i-1][1]) {
                /* check next quickly */
                dbs_tuners_ins.cpu_up_rate_adj >>= 1;
            }
        }
        if(!dbs_tuners_ins.cpu_up_rate_adj) {
            dbs_tuners_ins.cpu_up_rate_adj = 1;
        }

        /* need plug in cpu, reset the stat cycle */
        hotplug_history->num_hist = 0;

        return 1;
    }

    return 0;
}

/*
 * check if need plug out one cpu core
 */
static int check_down(struct cpufreq_policy *policy)
{
    struct cpu_usage *usage;
    int i, cpu, online, freq, rq_avg, down_freq, down_rq;
    int max_freq = 0, max_rq_avg = 0;
    unsigned int cpus_load_max[NR_CPUS], cpus_rq_max[NR_CPUS];
    int down_rate = dbs_tuners_ins.cpu_down_rate;
    int num_hist = hotplug_history->num_hist;
    int hotplug_lock = atomic_read(&g_hotplug_lock);

    /* hotplug has been locked, do nothing */
    if (hotplug_lock > 0)
        return 0;

    online = num_online_cpus();
    down_freq = policy->max * hotplug_freq[online-1][0];
    down_rq = hotplug_rq[online-1][0];

    /* just one cpu, can't be plug out */
    if (online == 1)
        return 0;

    /* if count of online cpu is larger than the max value, plug out cpu */
    if (dbs_tuners_ins.max_cpu_lock != 0 && online > dbs_tuners_ins.max_cpu_lock)
        return 1;

    /* check if reached the switch point */
    if (num_hist == 0 || num_hist % down_rate) {
        return 0;
    }

    for_each_online_cpu(cpu) {
        cpus_load_max[cpu] = 0;
        cpus_rq_max[cpu] = 0;
    }

    /* check system average loading */
    for (i = num_hist - 1; i >= num_hist - down_rate; --i) {
        usage = &hotplug_history->usage[i];

        freq = usage->freq * usage->loading_avg;
        rq_avg = usage->running_avg;

        max_freq = max(max_freq, freq);
        max_rq_avg = max(max_rq_avg, rq_avg);

        for_each_online_cpu(cpu) {
            cpus_load_max[cpu] = max(cpus_load_max[cpu], usage->loading[cpu]);
            cpus_rq_max[cpu] = max(cpus_rq_max[cpu], usage->running[cpu]);
        }
    }

    if (dbs_tuners_ins.dvfs_debug) {
        for_each_online_cpu(cpu) {
            printk("%s:%d, cpu:%d, load_max:%d, rq_max:%d\n", __func__, __LINE__,  \
                cpu, cpus_load_max[cpu], cpus_rq_max[cpu]);
        }
    }

    if (dbs_tuners_ins.dvfs_debug) {
        printk("%s: max_freq:%u, down_freq:%u, max_rq_avg:%u, down_rq:%u\n", __func__,     \
                    max_freq, down_freq, max_rq_avg, down_rq);
    }

    if (max_freq <= down_freq && max_rq_avg <= down_rq) {
        FANTASY_WRN("cpu need plugout, freq: %d<=%d && rq: %d<%d\n", max_freq, down_freq, max_rq_avg, down_rq);
        hotplug_history->num_hist = 0;
        /* need plug in cpu, reset the stat cycle */
        return 1;
    }

    /* check loading and running for spec cpu */
    for_each_online_cpu(cpu) {
        if(((cpus_load_max[cpu] < SINGLE_CPU_DOWN_LOADING) || (max_freq <= down_freq))  \
            && (cpus_rq_max[cpu] < SINGLE_CPU_DOWN_RUNNING)) {
            FANTASY_WRN("cpu need plugout, cpus_load_max:%d<%d && cpus_rq_max: %d<%d\n",    \
                    cpus_load_max[cpu], SINGLE_CPU_DOWN_LOADING, cpus_rq_max[cpu], SINGLE_CPU_DOWN_RUNNING);
            hotplug_history->num_hist = 0;
            return 1;
        }
    }

    return 0;
}


/* idle rate coarse adjust for cpu frequency down */
#define FANTASY_CPUFREQ_LOAD_MIN_RATE(freq)         \
    (freq<100000? 30 : (freq<200000? 40 : (freq<600000? 50 : (freq<900000? 60 : 70))))

/*
 * minimum rate for idle task, if idle rate is less than this
 * value, cpu frequency should be adjusted to the mauximum value
*/
#define FANTASY_CPUFREQ_LOAD_MAX_RATE(freq)         \
    (freq<100000? 60 : (freq<200000? 70 : (freq<600000? 80 : (freq<900000? 90 : 100))))

/*
 * define frequency limit for io-wait rate, cpu frequency should be limit to higher if io-wait higher
 */
#define FANTASY_CPUFREQ_IOW_LIMIT_RATE(iow)         \
    (iow<10? (iow>0? 20 : 0) : (iow<20? 40 : (iow<40? 60 : (iow<60? 80 : 100))))

#define DECRASE_FREQ_STEP_LIMIT1    (300000)   /* decrase frequency limited to 300Mhz when frequency is [900Mhz, 1008Mhz] */
#define DECRASE_FREQ_STEP_LIMIT2    (200000)   /* decrase frequency limited to 200Mhz when frequency is [600Mhz,  900Mhz) */
#define DECRASE_FREQ_STEP_LIMIT3    (100000)   /* decrase frequency limited to 100Mhz when frequency is [200Mhz,  600Mhz) */
#define DECRASE_FREQ_STEP_LIMIT4    (30000)    /* decrase frequency limited to  30Mhz when frequency is [60Mhz,   200Mhz) */

static int check_freq_up(struct cpufreq_policy *policy, unsigned int *target)
{
    int i, cpu;
    struct cpu_usage *usage;
    unsigned int avg_load[NR_CPUS], max_load;
    int up_rate = dbs_tuners_ins.freq_up_rate;
    int num_hist = hotplug_history->num_hist;

    /* check if reached the switch point */
    if (num_hist == 0 || num_hist % up_rate) {
        return 0;
    }

    for_each_online_cpu(cpu) {
        avg_load[cpu] = 0;
    }

    /* check cpu loading */
    for (i = num_hist - 1; i >= num_hist - up_rate; --i) {
        usage = &hotplug_history->usage[i];
        for_each_online_cpu(cpu) {
            avg_load[cpu] += usage->loading[cpu];
        }
    }

    max_load = 0;
    for_each_online_cpu(cpu) {
        avg_load[cpu] /= up_rate;
        max_load = max(max_load, avg_load[cpu]);
    }

    if(max_load > FANTASY_CPUFREQ_LOAD_MAX_RATE(policy->cur)) {
        if (dbs_tuners_ins.dvfs_debug) {
            printk("%s(%d): min_load=%d, cur_freq=%d, load_rate=%d\n",  \
                __func__, __LINE__, max_load, policy->cur, FANTASY_CPUFREQ_LOAD_MAX_RATE(policy->cur));
        }

        *target = policy->max;
        return 1;
    }

    return 0;
}


static int check_freq_down(struct cpufreq_policy *policy, unsigned int *target)
{
    int i, cpu;
    struct cpu_usage *usage;
    unsigned int avg_load[NR_CPUS], avg_iow[NR_CPUS], max_load, max_iow, freq_load, freq_iow;
    unsigned int freq_cur, freq_target;
    int down_rate = dbs_tuners_ins.freq_down_rate;
    int num_hist = hotplug_history->num_hist;

    if (dbs_tuners_ins.freq_down_delay) {
        if (!(num_hist % down_rate)) {
            dbs_tuners_ins.freq_down_delay = 0;
        }
        return 0;
    }

    /* check if reached the switch point */
    if (num_hist == 0 || num_hist % down_rate) {
        return 0;
    }

    for_each_online_cpu(cpu) {
        avg_load[cpu] = 0;
        avg_iow[cpu] = 0;
    }

    /* check cpu loading */
    for (i = num_hist - 1; i >= num_hist - down_rate; --i) {
        usage = &hotplug_history->usage[i];
        for_each_online_cpu(cpu) {
            avg_load[cpu] += usage->loading[cpu];
            avg_iow[cpu] += usage->iowait[cpu];
        }
    }

    max_load = 0;
    max_iow  = 0;
    for_each_online_cpu(cpu) {
        avg_load[cpu] /= down_rate;
        max_load = max(max_load, avg_load[cpu]);

        avg_iow[cpu] /= down_rate;
        max_iow = max(max_iow, avg_iow[cpu]);
    }

    /* check cpu loading */
    if (max_load >= FANTASY_CPUFREQ_LOAD_MIN_RATE(policy->cur))
        return 0;

    /*
     * calculate freq load
     * freq_load = (cur_freq * max_load) / ((max_rate + min_rate)/2)
     */
    freq_load = (policy->cur * max_load)/100;
    freq_load = (FANTASY_CPUFREQ_LOAD_MAX_RATE(freq_load) + FANTASY_CPUFREQ_LOAD_MIN_RATE(freq_load))>>1;
    freq_load = (policy->cur * max_load) / freq_load;

    /* check cpu io-waiting */
    freq_iow = (policy->max*FANTASY_CPUFREQ_IOW_LIMIT_RATE(max_iow))/100;

    if (dbs_tuners_ins.dvfs_debug) {
        printk("%s(%d): max_load=%d, cur_freq=%d, load_rate=%d, freq_load=%d\n", \
            __func__, __LINE__, max_load, policy->cur, FANTASY_CPUFREQ_LOAD_MIN_RATE(policy->cur), freq_load);
        printk("%s(%d): max_iow=%d, limit_rate=%d, freq_iow=%d\n", \
            __func__, __LINE__, max_iow, FANTASY_CPUFREQ_IOW_LIMIT_RATE(max_iow), freq_iow);
    }

    /* select target frequency */
    freq_target = max(freq_load, freq_iow);
    freq_cur = policy->cur;
    if (freq_cur >= 900000) {
        if (freq_cur - freq_target > DECRASE_FREQ_STEP_LIMIT1) {
            freq_target = freq_cur - DECRASE_FREQ_STEP_LIMIT1;
        }
    }
    else if (freq_cur >= 600000) {
        if(freq_cur - freq_target > DECRASE_FREQ_STEP_LIMIT2){
            freq_target = freq_cur - DECRASE_FREQ_STEP_LIMIT2;
        }
    }
    else if (freq_cur >= 200000) {
        if(freq_cur - freq_target > DECRASE_FREQ_STEP_LIMIT3){
            freq_target = freq_cur - DECRASE_FREQ_STEP_LIMIT3;
        }
    }
    else if (freq_cur >= 60000) {
        if(freq_cur - freq_target > DECRASE_FREQ_STEP_LIMIT4){
            freq_target = freq_cur - DECRASE_FREQ_STEP_LIMIT4;
        }
    }
    *target = freq_target;

    return 1;
}


#if ((LINUX_VERSION_CODE) < (KERNEL_VERSION(3, 3, 0)))
static inline cputime64_t get_cpu_idle_time_jiffy(unsigned int cpu,
                            cputime64_t *wall)
{
    cputime64_t idle_time;
    cputime64_t cur_wall_time;
    cputime64_t busy_time;

    cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());
    busy_time = cputime64_add(kstat_cpu(cpu).cpustat.user,
            kstat_cpu(cpu).cpustat.system);

    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.irq);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.softirq);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.steal);
    busy_time = cputime64_add(busy_time, kstat_cpu(cpu).cpustat.nice);

    idle_time = cputime64_sub(cur_wall_time, busy_time);
    if (wall)
        *wall = (cputime64_t)jiffies_to_usecs(cur_wall_time);

    return (cputime64_t)jiffies_to_usecs(idle_time);
}

static inline cputime64_t get_cpu_idle_time(unsigned int cpu, cputime64_t *wall)
{
    u64 idle_time = get_cpu_idle_time_us(cpu, wall);

    if (idle_time == -1ULL)
        return get_cpu_idle_time_jiffy(cpu, wall);

    return idle_time;
}

static inline cputime64_t get_cpu_iowait_time(unsigned int cpu, cputime64_t *wall)
{
    u64 iowait_time = get_cpu_iowait_time_us(cpu, wall);

    if (iowait_time == -1ULL)
        return 0;

    return iowait_time;
}
#else
static inline u64 get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
    u64 idle_time;
    u64 cur_wall_time;
    u64 busy_time;

    cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

    busy_time  = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
    busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
    busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
    busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
    busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
    busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

    idle_time = cur_wall_time - busy_time;
    if (wall)
        *wall = jiffies_to_usecs(cur_wall_time);

    return jiffies_to_usecs(idle_time);
}

static inline cputime64_t get_cpu_idle_time(unsigned int cpu, cputime64_t *wall)
{
    u64 idle_time = get_cpu_idle_time_us(cpu, NULL);

    if (idle_time == -1ULL)
        return get_cpu_idle_time_jiffy(cpu, wall);
    else
        idle_time += get_cpu_iowait_time_us(cpu, wall);

    return idle_time;
}

static inline cputime64_t get_cpu_iowait_time(unsigned int cpu, cputime64_t *wall)
{
    u64 iowait_time = get_cpu_iowait_time_us(cpu, wall);

    if (iowait_time == -1ULL)
        return 0;

    return iowait_time;
}
#endif


/*
 * check if need plug in/out cpu, if need increase/decrease cpu frequency
 */
static void dbs_check_cpu(struct cpu_dbs_info_s *this_dbs_info)
{
    unsigned int index, cpu, freq_target;
    struct cpufreq_policy *policy;
    int num_hist = hotplug_history->num_hist;
    int max_hotplug_rate = max((int)dbs_tuners_ins.cpu_up_rate, (int)dbs_tuners_ins.cpu_down_rate);

    max_hotplug_rate = max(max_hotplug_rate, (int)dbs_tuners_ins.freq_up_rate);
    max_hotplug_rate = max(max_hotplug_rate, (int)dbs_tuners_ins.freq_down_rate);

    policy = this_dbs_info->cur_policy;

    /* static cpu loading */
    hotplug_history->usage[num_hist].freq = policy->cur;
    hotplug_history->usage[num_hist].running_avg = get_rq_avg_sys();
    hotplug_history->usage[num_hist].loading_avg = 0;
    hotplug_history->usage[num_hist].iowait_avg = 0;
    ++hotplug_history->num_hist;

    for_each_online_cpu(cpu) {
        struct cpu_dbs_info_s *j_dbs_info;
        u64 cur_wall_time, cur_idle_time, cur_iowait_time;
        u64 prev_wall_time, prev_idle_time, prev_iowait_time;
        unsigned int idle_time, wall_time, iowait_time;
        unsigned int load = 0, iowait = 0;

        j_dbs_info = &per_cpu(od_cpu_dbs_info, cpu);
        prev_wall_time = j_dbs_info->prev_cpu_wall;
        prev_idle_time = j_dbs_info->prev_cpu_idle;
        prev_iowait_time = j_dbs_info->prev_cpu_iowait;

        cur_idle_time = get_cpu_idle_time(cpu, &cur_wall_time);
        cur_iowait_time = get_cpu_iowait_time(cpu, &cur_wall_time);

        wall_time = cur_wall_time - prev_wall_time;
        j_dbs_info->prev_cpu_wall = cur_wall_time;

        idle_time = cur_idle_time - prev_idle_time;
        j_dbs_info->prev_cpu_idle = cur_idle_time;

        iowait_time = cur_iowait_time - prev_iowait_time;
        j_dbs_info->prev_cpu_iowait = cur_iowait_time;


        if (dbs_tuners_ins.io_is_busy && idle_time >= iowait_time)
            idle_time -= iowait_time;

        if(wall_time && (wall_time > idle_time)) {
            load = 100 * (wall_time - idle_time) / wall_time;
        } else {
            load = 0;
        }
        hotplug_history->usage[num_hist].loading[cpu] = load;

        if(wall_time && (iowait_time < wall_time)) {
            iowait = 100 * iowait_time / wall_time;
        } else {
            iowait = 0;
        }
        hotplug_history->usage[num_hist].iowait[cpu] = iowait;

        /* calculate system average loading */
        hotplug_history->usage[num_hist].running[cpu] = get_rq_avg_cpu(cpu);
        hotplug_history->usage[num_hist].loading_avg += load;
        hotplug_history->usage[num_hist].iowait_avg  += iowait;
    }
    hotplug_history->usage[num_hist].loading_avg /= num_online_cpus();
    hotplug_history->usage[num_hist].iowait_avg  /= num_online_cpus();

    /* Check for CPU hotplug */
    if (check_up(policy)) {
        queue_work_on(this_dbs_info->cpu, dvfs_workqueue, &this_dbs_info->up_work);
    } else if (check_down(policy)) {
        queue_work_on(this_dbs_info->cpu, dvfs_workqueue, &this_dbs_info->down_work);
    }

    /*
     * The optimal frequency is the frequency that is the lowest that
     * can support the current CPU usage without triggering the up
     * policy. To be safe, we focus 10 points under the threshold.
     */
    if(check_freq_up(policy, &freq_target)) {
        /* should switch cpu frequency to the max value */
    } else if(check_freq_down(policy, &freq_target)) {
        /* down cpu frequency, set target frequency */
    } else {
        /* need do nothing */
        freq_target = policy->cur;
    }

    /* check if history array is out of range */
    if (hotplug_history->num_hist == max_hotplug_rate)
        hotplug_history->num_hist = 0;

    if (cpufreq_frequency_table_target(policy, this_dbs_info->freq_table, freq_target, CPUFREQ_RELATION_L, &index)) {
        FANTASY_ERR("%s: failed to get next lowest frequency\n", __func__);
        return;
    }
    freq_target = this_dbs_info->freq_table[index].frequency;

    if (policy->cur != freq_target) {
        /* set target frequency */
        FANTASY_WRN("%s, %d : try to switch cpu freq to %d \n", __func__, __LINE__, freq_target);
        __cpufreq_driver_target(policy, freq_target, CPUFREQ_RELATION_L);
    }

    return;
}


static void do_dbs_timer(struct work_struct *work)
{
    struct cpu_dbs_info_s *dbs_info = container_of(work, struct cpu_dbs_info_s, work.work);
    unsigned int cpu = dbs_info->cpu;
    int delay, max_power_flag;

    mutex_lock(&dbs_info->timer_mutex);

    max_power_flag = atomic_read(&g_max_power_flag);
    if (!max_power_flag) {
        dbs_check_cpu(dbs_info);

        /* We want all CPUs to do sampling nearly on
         * same jiffy
         */
        delay = usecs_to_jiffies(dbs_tuners_ins.sampling_rate);

        queue_delayed_work_on(cpu, dvfs_workqueue, &dbs_info->work, delay);
    }

    mutex_unlock(&dbs_info->timer_mutex);
}


static inline void dbs_timer_init(struct cpu_dbs_info_s *dbs_info)
{
    /* We want all CPUs to do sampling nearly on same jiffy */
    int delay = usecs_to_jiffies(DEF_START_DELAY * 1000 * 1000 + dbs_tuners_ins.sampling_rate);
    if (num_online_cpus() > 1)
        delay -= jiffies % delay;

    INIT_DELAYED_WORK_DEFERRABLE(&dbs_info->work, do_dbs_timer);
    INIT_WORK(&dbs_info->up_work, cpu_up_work);
    INIT_WORK(&dbs_info->down_work, cpu_down_work);
    queue_delayed_work_on(dbs_info->cpu, dvfs_workqueue, &dbs_info->work, delay + 2 * HZ);
}

static inline void dbs_timer_exit(struct cpu_dbs_info_s *dbs_info)
{
    cancel_delayed_work_sync(&dbs_info->work);
    cancel_work_sync(&dbs_info->up_work);
    cancel_work_sync(&dbs_info->down_work);
}


/*
 * cpufreq dbs governor
 */
static int cpufreq_governor_dbs(struct cpufreq_policy *policy, unsigned int event)
{
    unsigned int cpu = policy->cpu;
    struct cpu_dbs_info_s *this_dbs_info;
    unsigned int j;

    this_dbs_info = &per_cpu(od_cpu_dbs_info, cpu);

    switch (event) {
        case CPUFREQ_GOV_START:
        {
            if ((!cpu_online(cpu)) || (!policy->cur))
                return -EINVAL;

            hotplug_history->num_hist = 0;

            mutex_lock(&dbs_mutex);

            dbs_enable++;
            for_each_possible_cpu(j) {
                struct cpu_dbs_info_s *j_dbs_info;
                j_dbs_info = &per_cpu(od_cpu_dbs_info, j);
                j_dbs_info->cur_policy = policy;

                j_dbs_info->prev_cpu_idle = get_cpu_idle_time(j, &j_dbs_info->prev_cpu_wall);
            }
            this_dbs_info->cpu = cpu;
            this_dbs_info->freq_table = cpufreq_frequency_get_table(cpu);

            /*
             * Start the timerschedule work, when this governor
             * is used for first time
             */
            if (dbs_enable == 1) {
                dbs_tuners_ins.sampling_rate = DEF_SAMPLING_RATE;
                dbs_tuners_ins.io_is_busy = 1;
            }
            mutex_unlock(&dbs_mutex);

            mutex_init(&this_dbs_info->timer_mutex);
            dbs_timer_init(this_dbs_info);

            break;
        }

        case CPUFREQ_GOV_STOP:
        {
            dbs_timer_exit(this_dbs_info);

            mutex_lock(&dbs_mutex);
            mutex_destroy(&this_dbs_info->timer_mutex);

            dbs_enable--;
            mutex_unlock(&dbs_mutex);

            break;
        }

        case CPUFREQ_GOV_LIMITS:
        {
            mutex_lock(&this_dbs_info->timer_mutex);

            if (policy->max < this_dbs_info->cur_policy->cur)
                __cpufreq_driver_target(this_dbs_info->cur_policy, policy->max, CPUFREQ_RELATION_H);
            else if (policy->min > this_dbs_info->cur_policy->cur)
                __cpufreq_driver_target(this_dbs_info->cur_policy, policy->min, CPUFREQ_RELATION_L);

            mutex_unlock(&this_dbs_info->timer_mutex);
            break;
        }

        #ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
        case CPUFREQ_GOV_USRENET:
        {
            unsigned int freq_trig, index;
            /* cpu frequency limitation has changed, adjust current frequency */
            if (!mutex_trylock(&this_dbs_info->timer_mutex)) {
                FANTASY_WRN("CPUFREQ_GOV_USRENET try to lock mutex failed!\n");
                return 0;
            }

            freq_trig = (this_dbs_info->cur_policy->max*usrevent_freq[num_online_cpus() - 1])/100;
            if(!cpufreq_frequency_table_target(policy, this_dbs_info->freq_table, freq_trig, CPUFREQ_RELATION_H, &index)) {
                freq_trig = this_dbs_info->freq_table[index].frequency;
                if(this_dbs_info->cur_policy->cur < freq_trig) {
                    /* set cpu frequenc to the max value, and reset state machine */
                    FANTASY_DBG("CPUFREQ_GOV_USREVENT\n");
                    __cpufreq_driver_target(this_dbs_info->cur_policy, freq_trig, CPUFREQ_RELATION_H);
                }
            }
            dbs_tuners_ins.freq_down_delay = 1;
            mutex_unlock(&this_dbs_info->timer_mutex);
            break;
        }
        #endif

    }
    return 0;
}


/*
 * cpufreq governor dbs initiate
 */
static int __init cpufreq_gov_dbs_init(void)
{
    int i, ret;

    /* init policy table */
    for(i=0; i<NR_CPUS; i++) {
        hotplug_rq[i][0] = hotplug_rq_def[i][0];
        hotplug_rq[i][1] = hotplug_rq_def[i][1];

        hotplug_freq[i][0] = hotplug_freq_def[i][0];
        hotplug_freq[i][1] = hotplug_freq_def[i][1];

        #ifdef CONFIG_CPU_FREQ_USR_EVNT_NOTIFY
        usrevent_freq[i] = usrevent_freq_def[i];
        #endif
    }
    hotplug_rq[NR_CPUS-1][1] = INT_MAX;
    hotplug_freq[NR_CPUS-1][1] = INT_MAX;

    hotplug_history = kzalloc(sizeof(struct cpu_usage_history), GFP_KERNEL);
    if (!hotplug_history) {
        FANTASY_ERR("%s cannot create hotplug history array\n", __func__);
        ret = -ENOMEM;
        goto err_hist;
    }

    /* create dvfs daemon */
    dvfs_workqueue = create_singlethread_workqueue("fantasys");
    if (!dvfs_workqueue) {
        pr_err("%s cannot create workqueue\n", __func__);
        ret = -ENOMEM;
        goto err_queue;
    }

    /* register cpu freq governor */
    ret = cpufreq_register_governor(&cpufreq_gov_fantasys);
    if (ret)
        goto err_reg;

    ret = sysfs_create_group(cpufreq_global_kobject, &dbs_attr_group);
    if (ret) {
        goto err_governor;
    }

    return ret;

err_governor:
    cpufreq_unregister_governor(&cpufreq_gov_fantasys);
err_reg:
    destroy_workqueue(dvfs_workqueue);
err_queue:
    kfree(hotplug_history);
err_hist:
    return ret;
}

/*
 * cpufreq governor dbs exit
 */
static void __exit cpufreq_gov_dbs_exit(void)
{
    cpufreq_unregister_governor(&cpufreq_gov_fantasys);
    destroy_workqueue(dvfs_workqueue);
    kfree(hotplug_history);
}

MODULE_AUTHOR("kevin.z.m <kevin@allwinnertech.com>");
MODULE_DESCRIPTION("'cpufreq_fantasys' - A dynamic cpufreq/cpuhotplug governor");
MODULE_LICENSE("GPL");

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_FANTASYS
fs_initcall(cpufreq_gov_dbs_init);
#else
module_init(cpufreq_gov_dbs_init);
#endif
module_exit(cpufreq_gov_dbs_exit);
