#include <linux/earlysuspend.h>
#include <linux/cpufreq.h>
#include <linux/wakelock.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <asm/cpu.h>


static struct wake_lock ealysuspend_hotplug_work;
static struct work_struct earlysuspend_up_work;
static struct work_struct earlysuspend_down_work;
static struct workqueue_struct *earlysuspend_workqueue;

static struct cpumask online_backup_cpumask;
static int online_backup = 0;

#ifdef CONFIG_CPU_FREQ_GOV_FANTASYS
extern int cpufreq_fantasys_cpu_lock(int num_core);
extern int cpufreq_fantasys_cpu_unlock(int num_core);
extern atomic_t g_hotplug_lock;
extern atomic_t g_max_power_flag;
static int online_backup_fantasys = 0;
static int backup_max_power_flag = 0;
#endif

/*
 * cpu hotplug, cpu plugout
 */
static void cpu_down_work(struct work_struct *work)
{
    struct cpufreq_policy policy;
    int cpu, nr_down;

    printk("%s:%s\n", __FILE__, __func__);
    if (cpufreq_get_policy(&policy, 0))
        goto out;

    #ifdef CONFIG_CPU_FREQ_GOV_FANTASYS
    if (!strcmp(policy.governor->name, "fantasys")) {
        online_backup_fantasys = atomic_read(&g_hotplug_lock);
        if ((online_backup_fantasys > 0) && (online_backup_fantasys <= nr_cpu_ids)) {
            printk("online_backup_fantasys is %d\n", online_backup_fantasys);
            cpufreq_fantasys_cpu_lock(1);
            while (num_online_cpus() != 1) {
                msleep(50);
            }
        } else if (online_backup_fantasys == 0){
            backup_max_power_flag = atomic_read(&g_max_power_flag);
            printk("max_power_flag=%d\n", backup_max_power_flag);
            cpufreq_fantasys_cpu_lock(1);
            while (num_online_cpus() != 1) {
                msleep(50);
            }
        } else {
            printk("ERROR online_backup_fantasys is %d\n", online_backup_fantasys);
        }
        goto out;
    }
    #endif

    online_backup = num_online_cpus();
    cpumask_clear(&online_backup_cpumask);
    cpumask_copy(&online_backup_cpumask, cpu_online_mask);
    if ((online_backup > 1) && (online_backup <= nr_cpu_ids)) {
        nr_down = online_backup - 1;
        for_each_cpu(cpu, &online_backup_cpumask) {
            if (cpu == 0)
                continue;

            if (nr_down-- == 0)
                break;

            printk("cpu down:%d\n", cpu);
            cpu_down(cpu);
        }
    } else if (online_backup == 1) {
        printk("only cpu0 online, need not down\n");
    } else {
        printk("ERROR online cpu sum is %d\n", online_backup);
    }

out:
    wake_unlock(&ealysuspend_hotplug_work);
    printk("%s:%s done\n", __FILE__, __func__);
}

/*
 * cpu hotplug, just plug in one cpu
 */
static void cpu_up_work(struct work_struct *work)
{
    struct cpufreq_policy policy;
    int cpu, nr_up;

    printk("%s:%s\n", __FILE__, __func__);
    if (cpufreq_get_policy(&policy, 0))
        goto out;

    #ifdef CONFIG_CPU_FREQ_GOV_FANTASYS
    if (!strcmp(policy.governor->name, "fantasys")) {
        if (online_backup_fantasys > 0 && online_backup_fantasys <= nr_cpu_ids) {
            printk("online_backup_fantasys is %d\n", online_backup_fantasys);
            cpufreq_fantasys_cpu_unlock(1);
            cpufreq_fantasys_cpu_lock(online_backup_fantasys);
            while (num_online_cpus() != online_backup_fantasys) {
                msleep(50);
            }
        } else if (online_backup_fantasys == 0){
            printk("max_power_flag=%d\n", backup_max_power_flag);
            cpufreq_fantasys_cpu_unlock(1);
            if (backup_max_power_flag == 1) {
                nr_up = nr_cpu_ids - num_online_cpus();
                for_each_cpu_not(cpu, cpu_online_mask) {
                    if (cpu == 0)
                        continue;

                    if (nr_up-- == 0)
                        break;

                    cpu_up(cpu);
                }
            }
        } else {
            printk("ERROR online_backup_fantasys is %d\n", online_backup_fantasys);
        }
        goto out;
    }
    #endif

    if (online_backup > 1 && online_backup <= nr_cpu_ids) {
        nr_up = online_backup - 1;
        for_each_cpu(cpu, &online_backup_cpumask) {
            if (cpu == 0)
                continue;

            if (nr_up-- == 0)
                break;

            printk("cpu up:%d\n", cpu);
            cpu_up(cpu);
        }
    } else if (online_backup == 1) {
        printk("only cpu0 online, need not up\n");
    } else {
        printk("ERROR online backup cpu sum is %d\n", online_backup);
    }

out:
    wake_lock(&ealysuspend_hotplug_work);
    printk("%s:%s done\n", __FILE__, __func__);
}


static void hotplug_early_suspend(struct early_suspend *h)
{
    queue_work_on(0, earlysuspend_workqueue, &earlysuspend_down_work);
}

static void hotplug_late_resume(struct early_suspend *h)
{
    queue_work_on(0, earlysuspend_workqueue, &earlysuspend_up_work);
}

static struct early_suspend hotplug_earlysuspend =
{
    .level   = EARLY_SUSPEND_LEVEL_DISABLE_FB + 100,
	.suspend = hotplug_early_suspend,
	.resume = hotplug_late_resume,
};


int hotplug_early_suspend_init(void)
{
    cpumask_clear(&online_backup_cpumask);

    earlysuspend_workqueue = create_singlethread_workqueue("earlysuspend_hotplug");
    if (!earlysuspend_workqueue) {
        pr_err("%s cannot create workqueue\n", __func__);
        return -ENOMEM;
    }

    INIT_WORK(&earlysuspend_up_work, cpu_up_work);
    INIT_WORK(&earlysuspend_down_work, cpu_down_work);

    wake_lock_init(&ealysuspend_hotplug_work, WAKE_LOCK_SUSPEND, "suspend_hotplug");
    wake_lock(&ealysuspend_hotplug_work);

    register_early_suspend(&hotplug_earlysuspend);

    return 0;
}

int hotplug_early_suspend_exit(void)
{
    if(earlysuspend_workqueue) {
        destroy_workqueue(earlysuspend_workqueue);
        earlysuspend_workqueue = NULL;
    }

    unregister_early_suspend(&hotplug_earlysuspend);
    wake_unlock(&ealysuspend_hotplug_work);
    wake_lock_destroy(&ealysuspend_hotplug_work);
    return 0;
}

