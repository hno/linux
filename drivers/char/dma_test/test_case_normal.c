/*
 * drivers/char/dma_test/test_case_normal.c
 * (C) Copyright 2010-2015
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * liugang <liugang@reuuimllatech.com>
 *
 * sun7i dma test driver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include "sun7i_dma_test.h"

/* src/dst start address */
static u32 g_src_addr = 0, g_dst_addr = 0;
/* cur buf index */
static atomic_t g_acur_cnt = ATOMIC_INIT(0);

/**
 * __cb_fd_normal - full done callback for case DTC_NORMAL
 * @dma_hdl:	dma handle
 * @parg:	args registerd with cb function
 *
 * Returns 0 if sucess, the err line number if failed.
 */
void __cb_fd_normal(dma_hdl_t dma_hdl, void *parg)
{
	u32 	uret = 0;
	u32	ucur_saddr = 0, ucur_daddr = 0;
	u32	uloop_cnt = TOTAL_LEN_NORMAL / ONE_LEN_NORMAL;
	u32 	ucur_cnt = 0;

	pr_info("%s: called!\n", __func__);

	/* enqueue if not done */
	ucur_cnt = atomic_add_return(1, &g_acur_cnt);
	if(ucur_cnt < uloop_cnt) {
		printk("%s, line %d\n", __func__, __LINE__);
		/* NOTE: fatal err, when read here, g_acur_cnt has changed by other place, 2012-12-2 */
		//ucur_saddr = g_src_addr + atomic_read(&g_acur_cnt) * ONE_LEN_NORMAL;
		ucur_saddr = g_src_addr + ucur_cnt * ONE_LEN_NORMAL;
		ucur_daddr = g_dst_addr + ucur_cnt * ONE_LEN_NORMAL;
		if(0 != sw_dma_enqueue(dma_hdl, ucur_saddr, ucur_daddr, ONE_LEN_NORMAL))
			printk("%s err, line %d\n", __func__, __LINE__);
	} else { /* buf enqueue complete */
		printk("%s, line %d\n", __func__, __LINE__);
		//sw_dma_dump_chan(dma_hdl); /* for debug */

		/* maybe it's the last irq */
		atomic_set(&g_adma_done, 1);
		wake_up_interruptible(&g_dtc_queue[DTC_NORMAL]);
	}

	if(0 != uret)
		pr_err("%s err, line %d!\n", __func__, uret);
}

/**
 * __cb_hd_normal - half done callback for case DTC_NORMAL
 * @dma_hdl:	dma handle
 * @parg:	args registerd with cb function
 *
 * Returns 0 if sucess, the err line number if failed.
 */
void __cb_hd_normal(dma_hdl_t dma_hdl, void *parg)
{
	u32 	uret = 0;
	u32	ucur_saddr = 0, ucur_daddr = 0;
	u32	uloop_cnt = TOTAL_LEN_NORMAL / ONE_LEN_NORMAL;
	u32 	ucur_cnt = 0;

	pr_info("%s: called!\n", __func__);

	/* enqueue if not done */
	ucur_cnt = atomic_add_return(1, &g_acur_cnt);
	if(ucur_cnt < uloop_cnt) {
		printk("%s, line %d\n", __func__, __LINE__);
		/* NOTE: fatal err, when read here, g_acur_cnt has changed by other place, 2012-12-2 */
		//ucur_saddr = g_src_addr + atomic_read(&g_acur_cnt) * ONE_LEN_NORMAL;
		ucur_saddr = g_src_addr + ucur_cnt * ONE_LEN_NORMAL;
		ucur_daddr = g_dst_addr + ucur_cnt * ONE_LEN_NORMAL;
		if(0 != sw_dma_enqueue(dma_hdl, ucur_saddr, ucur_daddr, ONE_LEN_NORMAL))
			printk("%s err, line %d\n", __func__, __LINE__);
	}

	if(0 != uret)
		pr_err("%s err, line %d!\n", __func__, uret);
}

/**
 * __waitdone_normal - wait dma transfer function for case DTC_NORMAL
 *
 * Returns 0 if sucess, the err line number if failed.
 */
u32 __waitdone_normal(void)
{
	long 	ret = 0;
	long 	timeout = 2 * HZ;

	/* wait dma done */
	ret = wait_event_interruptible_timeout(g_dtc_queue[DTC_NORMAL], \
		atomic_read(&g_adma_done)== 1, timeout);
	/* reset dma done flag to 0 */
	atomic_set(&g_adma_done, 0);

	if(-ERESTARTSYS == ret) {
		pr_info("%s success!\n", __func__);
		return 0;
	} else if(0 == ret) {
		pr_info("%s err, time out!\n", __func__);
		return __LINE__;
	} else {
		pr_info("%s success with condition match, ret %d!\n", __func__, (int)ret);
		return 0;
	}
}

u32 __dtc_normal(void)
{
	u32 	uret = 0/* , tmp = 0 */;
	void 	*src_vaddr = NULL, *dst_vaddr = NULL;
	u32 	src_paddr = 0, dst_paddr = 0;
	dma_hdl_t dma_hdl = (dma_hdl_t)NULL;
	dma_cb_t done_cb;
	dma_config_t dma_config;

	pr_info("%s enter\n", __func__);

	/* prepare the buffer and data */
	src_vaddr = dma_alloc_coherent(NULL, TOTAL_LEN_NORMAL, (dma_addr_t *)&src_paddr, GFP_KERNEL);
	if(NULL == src_vaddr) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: src_vaddr 0x%08x, src_paddr 0x%08x\n", __func__, (u32)src_vaddr, src_paddr);
	dst_vaddr = dma_alloc_coherent(NULL, TOTAL_LEN_NORMAL, (dma_addr_t *)&dst_paddr, GFP_KERNEL);
	if(NULL == dst_vaddr) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: dst_vaddr 0x%08x, dst_paddr 0x%08x\n", __func__, (u32)dst_vaddr, dst_paddr);

	/* init src buffer */
	get_random_bytes(src_vaddr, TOTAL_LEN_NORMAL);
	memset(dst_vaddr, 0x54, TOTAL_LEN_NORMAL);

	/* init loop para */
	atomic_set(&g_acur_cnt, 0);
	g_src_addr = src_paddr;
	g_dst_addr = dst_paddr;

	/* request dma channel */
	dma_hdl = sw_dma_request("m2m_dma", CHAN_NORAML);
	if(NULL == dma_hdl) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_request success, dma_hdl 0x%08x\n", __func__, (u32)dma_hdl);

	/* set full done callback */
	done_cb.func = __cb_fd_normal;
	done_cb.parg = NULL;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_FD_CB, (void *)&done_cb)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: set fulldone_cb success\n", __func__);
	/* set half done callback */
	done_cb.func = __cb_hd_normal;
	done_cb.parg = NULL;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_HD_CB, (void *)&done_cb)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: set halfdone_cb success\n", __func__);

	/* config para */
	memset(&dma_config, 0, sizeof(dma_config));
	dma_config.xfer_type.src_data_width 	= DATA_WIDTH_32BIT;
	dma_config.xfer_type.src_bst_len 	= DATA_BRST_4;
	dma_config.xfer_type.dst_data_width 	= DATA_WIDTH_32BIT;
	dma_config.xfer_type.dst_bst_len 	= DATA_BRST_4;
	dma_config.address_type.src_addr_mode 	= NDMA_ADDR_INCREMENT;
	dma_config.address_type.dst_addr_mode 	= NDMA_ADDR_INCREMENT;
	dma_config.src_drq_type 	= N_SRC_SDRAM;
	dma_config.dst_drq_type 	= N_DST_SDRAM;
	dma_config.bconti_mode 		= false;
	dma_config.irq_spt 		= CHAN_IRQ_HD | CHAN_IRQ_FD;
	if(0 != sw_dma_config(dma_hdl, &dma_config)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_config success\n", __func__);

#if 0 /* add or not add, both ok, 2013-2-28 21:08 */
	/* set src/dst secu */
	tmp = SRC_SECU_DST_SECU;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_SECURITY, &tmp)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: DMA_OP_SET_SECURITY success\n", __func__);

	/* set wait state, ndma only */
	{
		u32 state = 0; /* 0~7, from spec */
		if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_WAIT_STATE, &state)) {
			uret = __LINE__;
			goto end;
		}
		pr_info("%s: DMA_OP_SET_WAIT_STATE success\n", __func__);
	}
#endif

#if 0
	/* set para reg, ddma only */
	{
		dma_para_t para;
		para.src_blk_sz 	= 0;
		para.src_wait_cyc 	= 0;
		para.dst_blk_sz 	= 0;
		para.dst_wait_cyc 	= 0;
		if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_PARA_REG, &tmp)) {
			uret = __LINE__;
			goto end;
		}
		pr_info("%s: DMA_OP_SET_PARA_REG success\n", __func__);
	}
#endif

	/* enqueue first buf */
	if(0 != sw_dma_enqueue(dma_hdl, g_src_addr, g_dst_addr, ONE_LEN_NORMAL)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_enqueue first buf success\n", __func__);

	/* dump chain */
	sw_dma_dump_chan(dma_hdl);

	/* start dma */
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_START, NULL)) {
		uret = __LINE__;
		goto end;
	}

	/* enqueue other buffer, with callback enqueue simutanously */
	{
		u32 	ucur_cnt = 0, ucur_saddr = 0, ucur_daddr = 0;
		u32	uloop_cnt = TOTAL_LEN_NORMAL / ONE_LEN_NORMAL;
		while((ucur_cnt = atomic_add_return(1, &g_acur_cnt)) < uloop_cnt) {
			ucur_saddr = g_src_addr + ucur_cnt * ONE_LEN_NORMAL;
			ucur_daddr = g_dst_addr + ucur_cnt * ONE_LEN_NORMAL;
			if(0 != sw_dma_enqueue(dma_hdl, ucur_saddr, ucur_daddr, ONE_LEN_NORMAL))
				printk("%s err, line %d\n", __func__, __LINE__);
		}
	}
	pr_info("%s, line %d\n", __func__, __LINE__);

	/* wait dma done */
	if(0 != __waitdone_normal()) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: __waitdone_normal sucess\n", __func__);

	/*
	 * NOTE: must sleep here, becase when __waitdone_normal return, buffer enqueue complete, but
	 * data might not transfer complete, 2012-11-14
	 */
	msleep(1200);

	/* check if data ok */
	if(0 == memcmp(src_vaddr, dst_vaddr, TOTAL_LEN_NORMAL))
		pr_info("%s: data check ok!\n", __func__);
	else {
		pr_err("%s: data check err!\n", __func__);
		uret = __LINE__; /* return err */
		goto end;
	}

	/* stop and release dma channel */
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_STOP, NULL)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_stop success\n", __func__);
	if(0 != sw_dma_release(dma_hdl)) {
		uret = __LINE__;
		goto end;
	}
	dma_hdl = (dma_hdl_t)NULL;
	pr_info("%s: sw_dma_release success\n", __func__);

end:
	if(0 != uret)
		pr_err("%s err, line %d!\n", __func__, uret); /* print err line */
	else
		pr_info("%s, success!\n", __func__);

	/* stop and free dma channel, if need */
	if((dma_hdl_t)NULL != dma_hdl) {
		pr_err("%s, stop and release dma handle now!\n", __func__);
		if(0 != sw_dma_ctl(dma_hdl, DMA_OP_STOP, NULL))
			pr_err("%s err, line %d!\n", __func__, __LINE__);
		if(0 != sw_dma_release(dma_hdl))
			pr_err("%s err, line %d!\n", __func__, __LINE__);
	}
	pr_info("%s, line %d!\n", __func__, __LINE__);

	/* free dma memory */
	if(NULL != src_vaddr)
		dma_free_coherent(NULL, TOTAL_LEN_NORMAL, src_vaddr, src_paddr);
	if(NULL != dst_vaddr)
		dma_free_coherent(NULL, TOTAL_LEN_NORMAL, dst_vaddr, dst_paddr);

	pr_info("%s, end!\n", __func__);
	return uret;
}

void __cb_fd_normal_conti(dma_hdl_t dma_hdl, void *parg)
{
	pr_info("%s: called!\n", __func__);
}

void __cb_hd_normal_conti(dma_hdl_t dma_hdl, void *parg)
{
	pr_info("%s: called!\n", __func__);
}

u32 __dtc_normal_conti(void)
{
	u32 	uret = 0, tmp = 0;
	void 	*src_vaddr = NULL, *dst_vaddr = NULL;
	u32 	src_paddr = 0, dst_paddr = 0;
	dma_hdl_t dma_hdl = (dma_hdl_t)NULL;
	dma_cb_t done_cb;
	dma_config_t dma_config;

	pr_info("%s enter\n", __func__);

	/* prepare the buffer and data */
	src_vaddr = dma_alloc_coherent(NULL, TOTAL_LEN_NORMAL, (dma_addr_t *)&src_paddr, GFP_KERNEL);
	if(NULL == src_vaddr) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: src_vaddr 0x%08x, src_paddr 0x%08x\n", __func__, (u32)src_vaddr, src_paddr);
	dst_vaddr = dma_alloc_coherent(NULL, TOTAL_LEN_NORMAL, (dma_addr_t *)&dst_paddr, GFP_KERNEL);
	if(NULL == dst_vaddr) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: dst_vaddr 0x%08x, dst_paddr 0x%08x\n", __func__, (u32)dst_vaddr, dst_paddr);

	/* init src buffer */
	get_random_bytes(src_vaddr, TOTAL_LEN_NORMAL);
	memset(dst_vaddr, 0x54, TOTAL_LEN_NORMAL);

	/* init loop para */
	atomic_set(&g_acur_cnt, 0);
	g_src_addr = src_paddr;
	g_dst_addr = dst_paddr;

	/* request dma channel */
	dma_hdl = sw_dma_request("m2m_dma", CHAN_NORAML);
	if(NULL == dma_hdl) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_request success, dma_hdl 0x%08x\n", __func__, (u32)dma_hdl);

	/* set full done callback */
	done_cb.func = __cb_fd_normal_conti;
	done_cb.parg = NULL;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_FD_CB, (void *)&done_cb)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: set fulldone_cb success\n", __func__);
	/* set half done callback */
	done_cb.func = __cb_hd_normal_conti;
	done_cb.parg = NULL;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_HD_CB, (void *)&done_cb)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: set halfdone_cb success\n", __func__);

	/* config para */
	memset(&dma_config, 0, sizeof(dma_config));
	dma_config.xfer_type.src_data_width 	= DATA_WIDTH_32BIT;
	dma_config.xfer_type.src_bst_len 	= DATA_BRST_4;
	dma_config.xfer_type.dst_data_width 	= DATA_WIDTH_32BIT;
	dma_config.xfer_type.dst_bst_len 	= DATA_BRST_4;
	dma_config.address_type.src_addr_mode 	= NDMA_ADDR_INCREMENT;
	dma_config.address_type.dst_addr_mode 	= NDMA_ADDR_INCREMENT;
	dma_config.src_drq_type 	= N_SRC_SDRAM;
	dma_config.dst_drq_type 	= N_DST_SDRAM;
	dma_config.bconti_mode 		= true;
	dma_config.irq_spt 		= CHAN_IRQ_HD | CHAN_IRQ_FD;
	if(0 != sw_dma_config(dma_hdl, &dma_config)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_config success\n", __func__);

	/* set src/dst secu */
	tmp = SRC_SECU_DST_SECU;
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_SECURITY, &tmp)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: DMA_OP_SET_SECURITY success\n", __func__);

	/* set wait state, ndma only */
	{
		u32 state = 0; /* 0~7, from spec */
		if(0 != sw_dma_ctl(dma_hdl, DMA_OP_SET_WAIT_STATE, &state)) {
			uret = __LINE__;
			goto end;
		}
		pr_info("%s: DMA_OP_SET_WAIT_STATE success\n", __func__);
	}

	/* enqueue first buf */
	if(0 != sw_dma_enqueue(dma_hdl, g_src_addr, g_dst_addr, TOTAL_LEN_NORMAL)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_enqueue success\n", __func__);

	/* dump chain */
	sw_dma_dump_chan(dma_hdl);

	/* start dma */
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_START, NULL)) {
		uret = __LINE__;
		goto end;
	}

	/*
	 * sleep to see the irq occur again and again
	 */
	msleep(1000);

	/* check if data ok */
	if(0 == memcmp(src_vaddr, dst_vaddr, TOTAL_LEN_NORMAL))
		pr_info("%s: data check ok!\n", __func__);
	else {
		pr_err("%s: data check err!\n", __func__);
		uret = __LINE__; /* return err */
		goto end;
	}

	/* stop and release dma channel */
	if(0 != sw_dma_ctl(dma_hdl, DMA_OP_STOP, NULL)) {
		uret = __LINE__;
		goto end;
	}
	pr_info("%s: sw_dma_stop success\n", __func__);
	if(0 != sw_dma_release(dma_hdl)) {
		uret = __LINE__;
		goto end;
	}
	dma_hdl = (dma_hdl_t)NULL;
	pr_info("%s: sw_dma_release success\n", __func__);

end:
	if(0 != uret)
		pr_err("%s err, line %d!\n", __func__, uret); /* print err line */
	else
		pr_info("%s, success!\n", __func__);

	/* stop and free dma channel, if need */
	if((dma_hdl_t)NULL != dma_hdl) {
		pr_err("%s, stop and release dma handle now!\n", __func__);
		if(0 != sw_dma_ctl(dma_hdl, DMA_OP_STOP, NULL))
			pr_err("%s err, line %d!\n", __func__, __LINE__);
		if(0 != sw_dma_release(dma_hdl))
			pr_err("%s err, line %d!\n", __func__, __LINE__);
	}
	pr_info("%s, line %d!\n", __func__, __LINE__);

	/* free dma memory */
	if(NULL != src_vaddr)
		dma_free_coherent(NULL, TOTAL_LEN_NORMAL, src_vaddr, src_paddr);
	if(NULL != dst_vaddr)
		dma_free_coherent(NULL, TOTAL_LEN_NORMAL, dst_vaddr, dst_paddr);

	pr_info("%s, end!\n", __func__);
	return uret;
}

