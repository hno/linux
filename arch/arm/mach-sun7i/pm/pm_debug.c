#include "pm_types.h"
#include "pm.h"

//for io-measure time
#define PORT_E_CONFIG (SW_VA_PORTC_IO_BASE + 0x90)
#define PORT_E_DATA (SW_VA_PORTC_IO_BASE + 0xa0)
#define PORT_CONFIG PORT_E_CONFIG
#define PORT_DATA PORT_E_DATA

volatile int  print_flag = 0;

void busy_waiting(void)
{
#if 1
	volatile __u32 loop_flag = 1;
	while(1 == loop_flag);
	
#endif
	return;
}

void fake_busy_waiting(void)
{
#if 1
	volatile __u32 loop_flag = 2;
	while(1 == loop_flag);
	
#endif
	return;
}


#define RTC_DATA_REG1				(SW_VA_TIMERC_IO_BASE + 0x124)
#define RTC_DATA_REG2				(SW_VA_TIMERC_IO_BASE + 0x128)
#define RTC_DATA_REG3				(SW_VA_TIMERC_IO_BASE + 0x12c)
#define lread(n)                    (*((volatile unsigned int *)(n)))
#define lwrite(addr,value)   (*((volatile unsigned int *)(addr)) = (value))

void standby_enable_crc(int enabled)
{
    if (enabled)
    {
        lwrite(RTC_DATA_REG1, 0x16510001);
    }
    else
    {
        lwrite(RTC_DATA_REG1, 0);
    }
    return;
}

void standby_set_crc(void *addr, int size)
{
    if (size > 0)
    {
        lwrite(RTC_DATA_REG2, (unsigned int)addr);
        lwrite(RTC_DATA_REG3, (size >> 2));
    }
}

unsigned int standby_get_crc(void)
{
    int i;
    unsigned int *tmp;
    unsigned int crc = -1;

    if (lread(RTC_DATA_REG1) != (unsigned int)(0x16510001))
    {
        return -1;
    }
    lwrite(RTC_DATA_REG1, 0);
    tmp = (unsigned int *)lread(RTC_DATA_REG2);
    i = lread(RTC_DATA_REG3);
    printk("RTC_DATA_REG3(%x):%x\n",RTC_DATA_REG1,lread(RTC_DATA_REG1));    
    printk("RTC_DATA_REG3(%x):%x\n",RTC_DATA_REG2,lread(RTC_DATA_REG2));    
    printk("RTC_DATA_REG3(%x):%x\n",RTC_DATA_REG3,lread(RTC_DATA_REG3));    
    while (i > 0)
    {
        crc += *tmp++;
        i--;
    }
    return crc;
}






/*
 * notice: dependant with perf counter to delay.
 */
void io_init(void)
{
	//config port output
	*(volatile unsigned int *)(PORT_CONFIG)  = 0x111111;
	
	return;
}

void io_init_high(void)
{
	__u32 data;
	
	//set port to high
	data = *(volatile unsigned int *)(PORT_DATA);
	data |= 0x3f;
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}

void io_init_low(void)
{
	__u32 data;

	data = *(volatile unsigned int *)(PORT_DATA);
	//set port to low
	data &= 0xffffffc0;
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}

/*
 * set pa port to high, num range is 0-7;	
 */
void io_high(int num)
{
	__u32 data;
	data = *(volatile unsigned int *)(PORT_DATA);
	//pull low 10ms
	data &= (~(1<<num));
	*(volatile unsigned int *)(PORT_DATA) = data;
	delay_us(10000);
	//pull high
	data |= (1<<num);
	*(volatile unsigned int *)(PORT_DATA) = data;

	return;
}
