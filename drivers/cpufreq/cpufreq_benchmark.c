/*
 *  linux/drivers/cpufreq/cpufreq_benchmark.c
 *
 *  Copyright (C) 2013 James Deng <csjamesdeng@allwinnertech.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cpufreq.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/clk.h>
#include <asm/uaccess.h>

#include <mach/clock.h>


#define MBUS_TARGET_VALID

#define CPU_EXTREMITY_FREQ              1008000     /* cpu extremity frequency: 1008M */

#define GPU_EXTREMITY_FREQ              "408000000" /* gpu extremity frequency:  408M */
#define SYS_VDD_EXTREMITY               "1400000"   /* sys vdd: 1.4V                  */

#define GPU_CLK_FILE                    "/sys/devices/platform/mali-utgard.0/aw_mali_freq/mali_clk"
#define GPU_VOL_FILE                    "/sys/devices/platform/mali-utgard.0/aw_mali_freq/mali_vol"

static struct file *clkp = NULL;
static struct file *volp = NULL;
static char sys_vdd_normal_bak[16] = {0}; 
static char gpu_normal_freq_bak[16] = {0}; 
static unsigned int max_cpu_freq_bak = 912000;


#ifdef MBUS_TARGET_VALID
static struct clk *mbus_clk = NULL;
#endif

static struct work_struct cpu_up_queue;

extern int cpu_up(unsigned int cpu);
extern int cpu_down(unsigned int cpu);

static int __gpu_target_init(void)
{
    mm_segment_t old_fs;
    if (!clkp) {
        clkp = filp_open(GPU_CLK_FILE, O_RDWR, 0);
        if (IS_ERR(clkp)) {
            clkp = NULL;
            return -1;
        }
    }

    if (!volp) {
        volp = filp_open(GPU_VOL_FILE, O_RDWR, 0);
        if (IS_ERR(volp)) {
            volp = NULL;
            filp_close(clkp, NULL);
            clkp = NULL;
            return -1;
        }
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    volp->f_op->read(volp, sys_vdd_normal_bak, sizeof(sys_vdd_normal_bak), &volp->f_pos);
    clkp->f_op->read(clkp, gpu_normal_freq_bak, sizeof(gpu_normal_freq_bak), &clkp->f_pos);
    set_fs(old_fs);

    return 0;
}

static void __gpu_target_extremity(void)
{
    mm_segment_t old_fs;
    if (clkp && volp) {
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        volp->f_op->write(volp, SYS_VDD_EXTREMITY, strlen(SYS_VDD_EXTREMITY), &volp->f_pos);
        clkp->f_op->write(clkp, GPU_EXTREMITY_FREQ, strlen(GPU_EXTREMITY_FREQ), &clkp->f_pos);
        set_fs(old_fs);
    }
}

static void __gpu_target_normal(void)
{
    mm_segment_t old_fs;
    if (clkp && volp) {
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        clkp->f_op->write(clkp, gpu_normal_freq_bak, strlen(gpu_normal_freq_bak), &clkp->f_pos);
        volp->f_op->write(volp, sys_vdd_normal_bak, strlen(sys_vdd_normal_bak), &volp->f_pos);
        set_fs(old_fs);
    }
}

static void __gpu_target_exit(void)
{
    if (clkp) {
        filp_close(clkp, NULL);
        clkp = NULL;
    }

    if (volp) {
        filp_close(volp, NULL);
        volp = NULL;
    }
}

#ifdef MBUS_TARGET_VALID
static int __mbus_target_400M(void)
{
    if (mbus_clk) {
        if (!clk_set_rate(mbus_clk, 200000000) &&
            !clk_set_rate(mbus_clk, 400000000)) {
            return 0;
        }
    }

    return -1;
}

static int __mbus_target_300M(void)
{
    if (mbus_clk) {
        if (!clk_set_rate(mbus_clk, 200000000) &&
            !clk_set_rate(mbus_clk, 300000000)) {
            return 0;
        }
    }

    return -1;
}
#endif

static void cpu_up_work(struct work_struct *work)
{
    cpu_up(1);
}

#if 0
static void cpu_down_work(struct work_struct *work)
{
    cpu_down(1);
}
#endif

static int cpufreq_governor_benchmark(struct cpufreq_policy *policy,
					unsigned int event)
{
    static int cpu1_state = 1;

	switch (event) {
	case CPUFREQ_GOV_START:
//	case CPUFREQ_GOV_LIMITS:
        /* set cpu frequency to extremity */
        max_cpu_freq_bak = policy->max;
        policy->max = CPU_EXTREMITY_FREQ;
		__cpufreq_driver_target(policy, CPU_EXTREMITY_FREQ,
                CPUFREQ_RELATION_H);

        /* init once */
        if (__gpu_target_init() < 0) {
            break;
        }

        /* set gpu frequency to extremity */
        __gpu_target_extremity();

#ifdef MBUS_TARGET_VALID
        /* set mbus clock to 400M */
        __mbus_target_400M();
#endif

        /* up cpu1 */
        cpu1_state = cpu_online(1);
        if (cpu1_state) {
            /* do nothing */
        } else {
            schedule_work(&cpu_up_queue);
        }

		break;
    case CPUFREQ_GOV_STOP:
#ifdef MBUS_TARGET_VALID
        /* set mbus clock to 300M */
        __mbus_target_300M();
#endif

        /* set gpu frequency to normal */
        __gpu_target_normal();

        /* set cpu frequency to highest */
        policy->max = max_cpu_freq_bak;
		__cpufreq_driver_target(policy, policy->max,
                CPUFREQ_RELATION_H);

        break;
	default:
		break;
	}
	return 0;
}

#ifdef CONFIG_CPU_FREQ_GOV_BENCHMARK_MODULE
static
#endif
struct cpufreq_governor cpufreq_gov_benchmark = {
	.name		= "benchmark",
	.governor	= cpufreq_governor_benchmark,
	.owner		= THIS_MODULE,
};


static int __init cpufreq_gov_benchmark_init(void)
{
#ifdef MBUS_TARGET_VALID
    /* mbus clock source is pll6x2 */
    mbus_clk = clk_get(NULL, CLK_MOD_MBUS);
    if (IS_ERR(mbus_clk)) {
        mbus_clk = NULL;
    }
#endif

    INIT_WORK(&cpu_up_queue, cpu_up_work);

	return cpufreq_register_governor(&cpufreq_gov_benchmark);
}


static void __exit cpufreq_gov_benchmark_exit(void)
{
#ifdef MBUS_TARGET_VALID
    if (mbus_clk) {
        clk_put(mbus_clk);
    }
#endif
    __gpu_target_exit();
	cpufreq_unregister_governor(&cpufreq_gov_benchmark);
}


MODULE_AUTHOR("James Deng <csjamesdeng@allwinnertech.com>");
MODULE_DESCRIPTION("CPUfreq policy governor 'benchmark'");
MODULE_LICENSE("GPL");

fs_initcall(cpufreq_gov_benchmark_init);
module_exit(cpufreq_gov_benchmark_exit);
