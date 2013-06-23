/*
*********************************************************************************************************
* File    : dram_i.h
* By      : Berg.Xing
* Date    : 2011-12-07
* Descript: dram for AW1625 chipset
* Update  : date                auther      ver     notes
*			2011-12-07			Berg        1.0     create file from aw1623
*			2011-12-31			Berg        1.1     create file from aw1623
*     2012-06-12      Daniel      1.2     Add DQS Phase Control
*     2012-06-15      Daniel      1.3     Adjust Initial Delay(including relation among RST/CKE/CLK)
*	  2013-03-06	  CPL		  1.4	  modify for A20
*********************************************************************************************************
*/
#include "dram_i.h"

static __u32 hpcr_value[32];
static 	__u32 gating = 0;

#ifdef STANDBY_CHECKCRC
__u32 standbyval1, standbyval2;

__u32 standby_dram_crc(__u32 dram_base,__u32 test_size)
{
   __u32 i,sum=0;
   for(i=dram_base;i<dram_base+test_size;i+=4)
   {
        sum += *((__u32*)(i));
   }
   return sum;
}
#endif

/*
*********************************************************************************************************
*                 DRAM ENTER SELF REFRESH
*
* Description: dram enter/exit self-refresh;
*
* Arguments  : none
*
* Returns    : none
*
* Note       :
*********************************************************************************************************
*/
void mctl_precharge_all(void)
{
	__u32 i;
	__u32 reg_val;

	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x15U<<27;
	mctl_write_w(SDR_DCR, reg_val);

	//check whether command has been executed
	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
    standby_delay(0x100);
}

void DRAMC_enter_selfrefresh(void)
{
	__u32 i;
	__u32 reg_val;

//	//disable all port
//	for(i=0; i<31; i++)
//	{
//		DRAMC_hostport_on_off(i, 0x0);
//	}
	for(i=0; i<8; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), 0);
	}
	
	for(i=16; i<28; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), 0);
	}	
	
	mctl_write_w(SDR_HPCR + (29<<2), 0);
	mctl_write_w(SDR_HPCR + (31<<2), 0);
/*
	//disable auto-fresh
	reg_val = mctl_read_w(SDR_DRR);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DRR, reg_val);
*/
	//issue prechage all command
	mctl_precharge_all();

	//enter into self-refresh
	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x12U<<27;
	mctl_write_w(SDR_DCR, reg_val);
	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
	standby_delay(0x100);
	
	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x3<<28);
	reg_val |= 0x2<<28;
	mctl_write_w(SDR_CR, reg_val);

	//dram pad odt hold
	mctl_write_w(SDR_DPCR, 0x16510001);
	
	while(!(mctl_read_w(SDR_DPCR) & 0x1));
}
void mctl_mode_exit(void)
{
	__u32 i;
	__u32 reg_val;

	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x17U<<27;
	mctl_write_w(SDR_DCR, reg_val);

	//check whether command has been executed
	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
	standby_delay(0x100);
}

void DRAMC_exit_selfrefresh(void)
{
	__u32 i;
	__u32 reg_val;

	//exit self-refresh state
	mctl_mode_exit();

	//issue a refresh command
	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x13U<<27;
	mctl_write_w(SDR_DCR, reg_val);
	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
    standby_delay(0x100);

	//enable auto-fresh
	reg_val = mctl_read_w(SDR_DRR);
	reg_val &= ~(0x1U<<31);
	mctl_write_w(SDR_DRR, reg_val);

	//enable all port
	for(i=0; i<31; i++)
	{
		DRAMC_hostport_on_off(i, 0x1);
	}
}

/*
*********************************************************************************************************
*                 DRAM POWER DOWN
*
* Description: enter/exit dram power down state
*
* Arguments  :
*
* Returns    : none;
*
* Note       :
*********************************************************************************************************
*/
void DRAMC_enter_power_down(void)
{
	__u32 i;
	__u32 reg_val;

	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x1eU<<27;
	mctl_write_w(SDR_DCR, reg_val);

	//check whether command has been executed
	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
	standby_delay(0x100);
}

void DRAMC_exit_power_down(void)
{
    mctl_mode_exit();
}

/*
**********************************************************************************************************************
*                 DRAM HOSTPORT CONTROL
*
* Description: dram host port enable/ disable
*
* Arguments  : __u32 port_idx		host port index   (0,1,...31)
*				__u32 on		enable or disable (0: diable, 1: enable)
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void DRAMC_hostport_on_off(__u32 port_idx, __u32 on)
{
    __u32   reg_val;

    if(port_idx<=31)
    {
	    reg_val = mctl_read_w(SDR_HPCR + (port_idx<<2));
	    if(on)
	    	reg_val |= 0x1;
	    else
	    	reg_val &= ~(0x1);
	    mctl_write_w(SDR_HPCR + (port_idx<<2), reg_val);
	}
}
/*
**********************************************************************************************************************
*                 DRAM GET HOSTPORT STATUS
*
* Description: dram get AHB FIFO status
*
* Arguments  : __u32 port_idx		host port index   	(0,1,...31)
*
* Returns    : __u32 ret_val		AHB FIFO status 	(0: FIFO not empty ,1: FIFO empty)
*
* Notes      :
*
**********************************************************************************************************************
*/
__u32 DRAMC_hostport_check_ahb_fifo_status(__u32 port_idx)
{
    __u32   reg_val;

    if(port_idx<=31)
    {
	    reg_val = mctl_read_w(SDR_CFSR);
	    return ( (reg_val>>port_idx)&0x1 );
	}
	else
	{
		return 0;
	}
}
/*
**********************************************************************************************************************
*                 DRAM GET HOSTPORT STATUS
*
* Description: dram get AHB FIFO status
*
* Arguments  : 	__u32 port_idx				host port index   	(0,1,...31)
*				__u32 port_pri_level		priority level		(0,1,2,3)
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/
void DRAMC_hostport_setup(__u32 port_idx, __u32 port_pri_level, __u32 port_wait_cycle, __u32 cmd_num)
{
    __u32   reg_val;

    if(port_idx<=31)
    {
	    reg_val = mctl_read_w(SDR_HPCR + (port_idx<<2));
	    reg_val &= ~(0x3<<2);
	    reg_val |= (port_pri_level&0x3)<<2;
	    reg_val &= ~(0xf<<4);
	    reg_val |= (port_wait_cycle&0xf)<<4;
	    reg_val &= ~(0xff<<8);
	    reg_val |= (cmd_num&0x3)<<8;
	    mctl_write_w(SDR_HPCR + (port_idx<<2), reg_val);
	}
}
/*
*********************************************************************************************************
*                 DRAM power save process
*
* Description: We can save power by disable DRAM PLL.
*
* Arguments  : none
*
* Returns    : none
*
* Note       :
*********************************************************************************************************
*/
__u32 mctl_ahb_reset(void)
{
	__u32 i;
	__u32 reg_val;

	reg_val = mctl_read_w(DRAM_CCM_AHB_GATE_REG);
	reg_val &=~(0x3<<14);
	mctl_write_w(DRAM_CCM_AHB_GATE_REG,reg_val);
    standby_delay(0x10);

	reg_val = mctl_read_w(DRAM_CCM_AHB_GATE_REG);
	reg_val |=(0x3<<14);
	mctl_write_w(DRAM_CCM_AHB_GATE_REG,reg_val);
}

__s32 DRAMC_retraining(void)
{
	__u32 i;
	__u32 reg_val;
	__u32 ret_val;
	__u32 reg_dcr, reg_drr, reg_tpr0, reg_tpr1, reg_tpr2, reg_tpr3, reg_mr, reg_emr, reg_emr2, reg_emr3;
	__u32 reg_zqcr0, reg_iocr, reg_ccr, reg_zqsr;

	//remember register value
	reg_dcr = mctl_read_w(SDR_DCR);
	reg_drr = mctl_read_w(SDR_DRR);
	reg_tpr0 = mctl_read_w(SDR_TPR0);
	reg_tpr1 = mctl_read_w(SDR_TPR1);
	reg_tpr2 = mctl_read_w(SDR_TPR2);
	reg_mr = mctl_read_w(SDR_MR);
	reg_emr = mctl_read_w(SDR_EMR);
	reg_emr2 = mctl_read_w(SDR_EMR2);
	reg_emr3 = mctl_read_w(SDR_EMR3);
	reg_zqcr0 = mctl_read_w(SDR_ZQCR0);
	reg_iocr = mctl_read_w(SDR_IOCR);
	reg_tpr3 = (mctl_read_w(SDR_DLLCR1)>>14) & 0xf;
	reg_tpr3 |= ((mctl_read_w(SDR_DLLCR2)>>14) & 0xf)<<4;
	reg_tpr3 |= ((mctl_read_w(SDR_DLLCR3)>>14) & 0xf)<<8;
	reg_tpr3 |= ((mctl_read_w(SDR_DLLCR4)>>14) & 0xf)<<12;
	reg_ccr = mctl_read_w(SDR_CCR);
    reg_zqsr = mctl_read_w(SDR_ZQSR);
	while(1){
		mctl_ahb_reset();

		mctl_set_drive();

		mctl_itm_disable();

		mctl_enable_dll0();

		//set CCR value
		mctl_write_w(SDR_CCR, reg_ccr);

		//configure external DRAM
		mctl_write_w(SDR_DCR, reg_dcr);

		//set ZQ value
		reg_val = reg_zqsr&0xfffff;
		reg_val |= 0x1<<30;
		reg_val |= 0x1<<28;
		reg_val |= reg_zqcr0&(0xff<<20);
		reg_val |= reg_zqcr0&(0x1<<29);
		mctl_write_w(SDR_ZQCR0, reg_val);

    //Set CKE Delay to about 1ms
	  reg_val = mctl_read_w(SDR_IDCR);
	  reg_val |= 0x1ffff;
	  mctl_write_w(SDR_IDCR, reg_val);
		
		//dram clock on
		DRAMC_clock_output_en(1);
		mctl_ddr3_reset();
		
		standby_delay(0x10);
		while(mctl_read_w(SDR_CCR) & (0x1U<<31)) {};

		mctl_enable_dllx(reg_tpr3);

		//set I/O configure register
		mctl_write_w(SDR_IOCR, reg_iocr);

		//set refresh period
		mctl_write_w(SDR_DRR, reg_drr);

		//set timing parameters
		mctl_write_w(SDR_TPR0, reg_tpr0);
		mctl_write_w(SDR_TPR1, reg_tpr1);
		mctl_write_w(SDR_TPR2, reg_tpr2);

		//set mode register
		mctl_write_w(SDR_MR, reg_mr);
		mctl_write_w(SDR_EMR, reg_emr);
		mctl_write_w(SDR_EMR2, reg_emr2);
		mctl_write_w(SDR_EMR3, reg_emr3);

		//initial external DRAM
		reg_val = mctl_read_w(SDR_CCR);
		reg_val |= 0x1U<<31;
		mctl_write_w(SDR_CCR, reg_val);
		while(mctl_read_w(SDR_CCR) & (0x1U<<31)) {};

		//dram pad hold release
		mctl_write_w(SDR_DPCR, 0x16510000);
		standby_delay(0x10000);
		
		while(mctl_read_w(SDR_DPCR) & 0x1);

		//scan read pipe value
		mctl_itm_enable();
		ret_val = DRAMC_scan_readpipe();

		//configure all host port
		mctl_configure_hostport();

		if(ret_val == 0)
			return 0;
    }
}

__s32 dram_power_save_process(void)
{
#if 0
	__u32 reg_val;

	//put external SDRAM into self-fresh state
	DRAMC_enter_selfrefresh();

	//disable ITM
	mctl_itm_disable();

	//disable and reset all DLL
	mctl_disable_dll();
#else
	__u32 i;	
	__u32 reg_val;

	
#ifdef STANDBY_CHECKCRC
	standbyval1 = standby_dram_crc(0xc0000000, 0x2000000);
#endif
	
	gating = mctl_read_w(DRAM_CCM_SDRAM_CLK_REG);
	mctl_write_w(DRAM_CCM_SDRAM_CLK_REG, 0);
	
	for(i=0; i<8; i++)
	{
		hpcr_value[i] = mctl_read_w(SDR_HPCR + (i<<2));
	}

	for(i=16; i<28; i++)
	{
		hpcr_value[i] = mctl_read_w(SDR_HPCR + (i<<2));
	}

	hpcr_value[29] = mctl_read_w(SDR_HPCR + (29<<2));
	hpcr_value[31] = mctl_read_w(SDR_HPCR + (31<<2));

	for(i=0; i<8; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), 0);
	}

	for(i=16; i<28; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), 0);
	}

	mctl_write_w(SDR_HPCR + (29<<2), 0);
	mctl_write_w(SDR_HPCR + (31<<2), 0);
	
//	printk("[DRAM] 1 ======================\n");	

	//enter into self-refresh
	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x12U<<27;
	mctl_write_w(SDR_DCR, reg_val);

	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );
	
//	printk("[DRAM] 2 ======================\n");

	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x3<<28);
	reg_val |= 0x2<<28;
	mctl_write_w(SDR_CR, reg_val);
	
//	printk("[DRAM] 3 ======================\n");

	//dram pad odt hold
	mctl_write_w(SDR_DPCR, 0x16510001);

	while(!(mctl_read_w(SDR_DPCR) & 0x1));
	
//	printk("[DRAM] 4 ======================\n");

	//itm disable
	reg_val = mctl_read_w(SDR_CCR);
	reg_val |= 0x1<<28;
	reg_val &= ~(0x1U<<31);          //danielwang, 2012-05-18
	mctl_write_w(SDR_CCR, reg_val);
	
//	printk("[DRAM] 5 ======================\n");

	//turn off sclk
	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x1<<16);
	mctl_write_w(SDR_CR, reg_val);
	
//	printk("[DRAM] 6 ======================\n");

	//disable dll
	reg_val = mctl_read_w(SDR_DLLCR0);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR0, reg_val);
	
//	printk("[DRAM] 7 ======================\n");

	reg_val = mctl_read_w(SDR_DLLCR1);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR1, reg_val);
	
//	printk("[DRAM] 8 ======================\n");

	reg_val = mctl_read_w(SDR_DLLCR2);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR2, reg_val);
	
//	printk("[DRAM] 9 ======================\n");

	reg_val = mctl_read_w(SDR_DLLCR3);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR3, reg_val);
	
//	printk("[DRAM] 10 ======================\n");

	reg_val = mctl_read_w(SDR_DLLCR4);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1U<<31;
	mctl_write_w(SDR_DLLCR4, reg_val);
	
//	printk("[DRAM] 11 ======================\n");
	
	//	//turn off pll5
	
//	reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
//	reg_val &= ~(0x1<<29);
//	mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);
//	
//	
//	reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
//	printk("[DRAM]before pll5 reg_val = %x ======================\n", reg_val);
//	reg_val &= ~(0x1u<<31);
//	mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);
//	printk("[DRAM] 12 ======================\n");
	
	
#endif
	return 0;
}
__u32 dram_power_up_process(void)
{
#if 0
	return DRAMC_retraining();
#else
	__u32 reg_val;
	__u32 i;
	 //turn on pll5
	//setup DRAM PLL
//	reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
//	reg_val &= ~0x3;
//	reg_val |= 0x1;                                             //m factor
//	reg_val &= ~(0x3<<4);
//	reg_val |= 0x1<<4;                                          //k factor
//	reg_val &= ~(0x1f<<8);
//	reg_val |= ((432/24)&0x1f)<<8;                              //n factor
//	reg_val &= ~(0x3<<16);
//	reg_val |= 0x1<<16;                                         //p factor
//	reg_val &= ~(0x1<<29);                                      //clock output disable
//	reg_val |= (__u32)0x1<<31;                                  //PLL En
//	mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);
//	
//	standby_delay(0x100000);
//	
//	reg_val = mctl_read_w(DRAM_CCM_SDRAM_PLL_REG);
//	reg_val |= 0x1<<29;
//	mctl_write_w(DRAM_CCM_SDRAM_PLL_REG, reg_val);

	//turn on dll
	mctl_enable_dll0();
	mctl_enable_dllx(0);
	
	//turn on sclk
	reg_val = mctl_read_w(SDR_CR);
	reg_val |= (0x1<<16);
	mctl_write_w(SDR_CR, reg_val);

    //itm enable
	reg_val = mctl_read_w(SDR_CCR);
	reg_val &= ~(0x1<<28);
	mctl_write_w(SDR_CCR, reg_val);

	reg_val = mctl_read_w(SDR_CR);
	reg_val &= ~(0x3<<28);
	mctl_write_w(SDR_CR, reg_val);

	//dram pad odt hold
	mctl_write_w(SDR_DPCR, 0x16510000);

	while((mctl_read_w(SDR_DPCR) & 0x1));


	//exit from self-refresh
	reg_val = mctl_read_w(SDR_DCR);
	reg_val &= ~(0x1fU<<27);
	reg_val |= 0x17U<<27;
	mctl_write_w(SDR_DCR, reg_val);

	while( mctl_read_w(SDR_DCR)& (0x1U<<31) );

	for(i=0; i<8; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), hpcr_value[i]);
	}

	for(i=16; i<28; i++)
	{
		mctl_write_w(SDR_HPCR + (i<<2), hpcr_value[i]);
	}

	mctl_write_w(SDR_HPCR + (29<<2), hpcr_value[29]);
	mctl_write_w(SDR_HPCR + (31<<2), hpcr_value[31]);

	mctl_write_w(DRAM_CCM_SDRAM_CLK_REG, gating);
	
#ifdef STANDBY_CHECKCRC
	standbyval2 = standby_dram_crc(0xc0000000, 0x2000000);
	if(standbyval1 == standbyval2)
		printk("\n[DRAM]CRC SUCCESS==================\n");
	else
		printk("\n[DRAM]CRC FAILED==================\n");
#endif
	return 0;
#endif
}


void dram_enter_selfrefresh(void)
{
    DRAMC_enter_selfrefresh();
}


void dram_exit_selfrefresh(void)
{
    DRAMC_exit_selfrefresh();
}


void dram_enter_power_down(void)
{
    DRAMC_enter_power_down();
}


void dram_exit_power_down(void)
{
    DRAMC_exit_power_down();
}


void dram_hostport_on_off(__u32 port_idx, __u32 on)
{
    DRAMC_hostport_on_off(port_idx, on);
}


__u32 dram_hostport_check_ahb_fifo_status(__u32 port_idx)
{
    return DRAMC_hostport_check_ahb_fifo_status(port_idx);
}


void dram_hostport_setup(__u32 port, __u32 prio, __u32 wait_cycle, __u32 cmd_num)
{
    DRAMC_hostport_setup(port, prio, wait_cycle, cmd_num);
}

