/*
 * sound\soc\sun7i\i2s\sun7i-i2s.c
 * (C) Copyright 2007-2011
 * Reuuimlla Technology Co., Ltd. <www.reuuimllatech.com>
 * chenpailin <chenpailin@Reuuimllatech.com>
 *
 * some simple description for this code
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/jiffies.h>
#include <linux/io.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include <mach/clock.h>
#include <mach/sys_config.h>

#include <mach/hardware.h>
#include <asm/dma.h>
#include <mach/dma.h>

#include <linux/gpio.h>


#include "sun7i-i2sdma.h"
#include "sun7i-i2s.h"

static int regsave[8];
static int i2s_used = 1;


static struct sun7i_dma_params sun7i_i2s_pcm_stereo_out = {
	
	.name        = "I2S PCM Stereo out",
	.dma_addr    =	SUN7I_IISBASE + SUN7I_IISTXFIFO,
	
};

static struct sun7i_dma_params sun7i_i2s_pcm_stereo_in = {
	
	.name       = "I2S PCM Stereo in",
	.dma_addr 	=	SUN7I_IISBASE + SUN7I_IISRXFIFO,

};


 struct sun7i_i2s_info sun7i_iis;
//static u32 i2s_handle = 0;
 static struct clk *i2s_apbclk, *i2s_pll2clk, *i2s_pllx8, *i2s_moduleclk;

void sun7i_snd_txctrl_i2s(struct snd_pcm_substream *substream, int on)
{
	u32 reg_val;
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	reg_val = readl(sun7i_iis.regs + SUN7I_TXCHSEL);
	reg_val &= ~0x7;
	reg_val |= SUN7I_TXCHSEL_CHNUM(substream->runtime->channels);
	writel(reg_val, sun7i_iis.regs + SUN7I_TXCHSEL);

	reg_val = readl(sun7i_iis.regs + SUN7I_TXCHMAP);
	reg_val = 0;
	if(substream->runtime->channels == 1) {
		reg_val = 0x00000000;
	} else {
		reg_val = 0x00000010;
	}
	writel(reg_val, sun7i_iis.regs + SUN7I_TXCHMAP);

	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);	
	reg_val &= ~SUN7I_IISCTL_SDO0EN;
	switch(substream->runtime->channels) {
		case 1:
		case 2:
			reg_val |= SUN7I_IISCTL_SDO0EN; break;	
		default:
			reg_val |= SUN7I_IISCTL_SDO0EN; break;
	}
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	
	//flush TX FIFO
	reg_val = readl(sun7i_iis.regs + SUN7I_IISFCTL);
	reg_val |= SUN7I_IISFCTL_FTX;	
	writel(reg_val, sun7i_iis.regs + SUN7I_IISFCTL);
	
	//clear TX counter
	writel(0, sun7i_iis.regs + SUN7I_IISTXCNT);

	if (on) {
		/* IIS TX ENABLE */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val |= SUN7I_IISCTL_TXEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
		
		/* enable DMA DRQ mode for play */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISINT);
		reg_val |= SUN7I_IISINT_TXDRQEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISINT);
	} else {
		/* IIS TX DISABLE */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val &= ~SUN7I_IISCTL_TXEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
			
		/* DISBALE dma DRQ mode */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISINT);
		reg_val &= ~SUN7I_IISINT_TXDRQEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISINT);
	}		
}

void sun7i_snd_rxctrl_i2s(struct snd_pcm_substream *substream, int on)
{	
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	u32 reg_val;
	reg_val = readl(sun7i_iis.regs + SUN7I_RXCHSEL);
	reg_val &= ~0x1;
	reg_val |= SUN7I_RXCHSEL_CHNUM(substream->runtime->channels);
	writel(reg_val, sun7i_iis.regs + SUN7I_RXCHSEL);

	reg_val = readl(sun7i_iis.regs + SUN7I_RXCHMAP);
	reg_val = 0;
	if(substream->runtime->channels == 1) {
		reg_val = 0x00000000;
	} else {
		reg_val = 0x00000010;
	}
	writel(reg_val, sun7i_iis.regs + SUN7I_RXCHMAP);
	
	//flush RX FIFO
	reg_val = readl(sun7i_iis.regs + SUN7I_IISFCTL);
	reg_val |= SUN7I_IISFCTL_FRX;	
	writel(reg_val, sun7i_iis.regs + SUN7I_IISFCTL);

	//clear RX counter
	writel(0, sun7i_iis.regs + SUN7I_IISRXCNT);
	
	if (on) {
		/* IIS RX ENABLE */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val |= SUN7I_IISCTL_RXEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
			
		/* enable DMA DRQ mode for record */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISINT);
		reg_val |= SUN7I_IISINT_RXDRQEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISINT);
			
		//Global Enable Digital Audio Interface
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val |= SUN7I_IISCTL_GEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
			
	} else {
		/* IIS RX DISABLE */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val &= ~SUN7I_IISCTL_RXEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
			
		/* DISBALE dma DRQ mode */
		reg_val = readl(sun7i_iis.regs + SUN7I_IISINT);
		reg_val &= ~SUN7I_IISINT_RXDRQEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISINT);
				
		//Global disable Digital Audio Interface
		reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
		reg_val &= ~SUN7I_IISCTL_GEN;
		writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	}		
}

static inline int sun7i_snd_is_clkmaster(void)
{
	return ((readl(sun7i_iis.regs + SUN7I_IISCTL) & SUN7I_IISCTL_MS) ? 0 : 1);
}

static int sun7i_i2s_set_fmt(struct snd_soc_dai *cpu_dai, unsigned int fmt)
{
	u32 reg_val;
	u32 reg_val1;
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	//SDO ON
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	reg_val |= (SUN7I_IISCTL_SDO0EN); 
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);

	/* master or slave selection */
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	switch(fmt & SND_SOC_DAIFMT_MASTER_MASK){
		case SND_SOC_DAIFMT_CBM_CFM:   /* codec clk & frm master */
			reg_val |= SUN7I_IISCTL_MS;
			break;
		case SND_SOC_DAIFMT_CBS_CFS:   /* codec clk & frm slave */
			reg_val &= ~SUN7I_IISCTL_MS;
			break;
		default:
			return -EINVAL;
	}
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	
	/* pcm or i2s mode selection */
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	reg_val1 = readl(sun7i_iis.regs + SUN7I_IISFAT0);
	reg_val1 &= ~SUN7I_IISFAT0_FMT_RVD;
	switch(fmt & SND_SOC_DAIFMT_FORMAT_MASK){
		case SND_SOC_DAIFMT_I2S:        /* I2S mode */
			reg_val &= ~SUN7I_IISCTL_PCM;
			reg_val1 |= SUN7I_IISFAT0_FMT_I2S;
			break;
		case SND_SOC_DAIFMT_RIGHT_J:    /* Right Justified mode */
			reg_val &= ~SUN7I_IISCTL_PCM;
			reg_val1 |= SUN7I_IISFAT0_FMT_RGT;
			break;
		case SND_SOC_DAIFMT_LEFT_J:     /* Left Justified mode */
			reg_val &= ~SUN7I_IISCTL_PCM;
			reg_val1 |= SUN7I_IISFAT0_FMT_LFT;
			break;
		case SND_SOC_DAIFMT_DSP_A:      /* L data msb after FRM LRC */
			reg_val |= SUN7I_IISCTL_PCM;
			reg_val1 &= ~SUN7I_IISFAT0_LRCP;
			break;
		case SND_SOC_DAIFMT_DSP_B:      /* L data msb during FRM LRC */
			reg_val |= SUN7I_IISCTL_PCM;
			reg_val1 |= SUN7I_IISFAT0_LRCP;
			break;
		default:
			return -EINVAL;
	}
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	writel(reg_val1, sun7i_iis.regs + SUN7I_IISFAT0);
	
	/* DAI signal inversions */
	reg_val1 = readl(sun7i_iis.regs + SUN7I_IISFAT0);
	switch(fmt & SND_SOC_DAIFMT_INV_MASK){
		case SND_SOC_DAIFMT_NB_NF:     /* normal bit clock + frame */
			reg_val1 &= ~SUN7I_IISFAT0_LRCP;
			reg_val1 &= ~SUN7I_IISFAT0_BCP;
			break;
		case SND_SOC_DAIFMT_NB_IF:     /* normal bclk + inv frm */
			reg_val1 |= SUN7I_IISFAT0_LRCP;
			reg_val1 &= ~SUN7I_IISFAT0_BCP;
			break;
		case SND_SOC_DAIFMT_IB_NF:     /* invert bclk + nor frm */
			reg_val1 &= ~SUN7I_IISFAT0_LRCP;
			reg_val1 |= SUN7I_IISFAT0_BCP;
			break;
		case SND_SOC_DAIFMT_IB_IF:     /* invert bclk + frm */
			reg_val1 |= SUN7I_IISFAT0_LRCP;
			reg_val1 |= SUN7I_IISFAT0_BCP;
			break;
	}
	writel(reg_val1, sun7i_iis.regs + SUN7I_IISFAT0);
	
	/* word select size */
	reg_val = readl(sun7i_iis.regs + SUN7I_IISFAT0);
	reg_val &= ~SUN7I_IISFAT0_WSS_32BCLK;
	if(sun7i_iis.ws_size == 16)
		reg_val |= SUN7I_IISFAT0_WSS_16BCLK;
	else if(sun7i_iis.ws_size == 20) 
		reg_val |= SUN7I_IISFAT0_WSS_20BCLK;
	else if(sun7i_iis.ws_size == 24)
		reg_val |= SUN7I_IISFAT0_WSS_24BCLK;
	else
		reg_val |= SUN7I_IISFAT0_WSS_32BCLK;
	writel(reg_val, sun7i_iis.regs + SUN7I_IISFAT0);

	/* PCM REGISTER setup */
	reg_val = sun7i_iis.pcm_txtype&0x3;
	reg_val |= sun7i_iis.pcm_rxtype<<2;
	
	if(!sun7i_iis.pcm_sync_type)
		reg_val |= SUN7I_IISFAT1_SSYNC;							//short sync		
	if(sun7i_iis.pcm_sw == 16)
		reg_val |= SUN7I_IISFAT1_SW;
			
	reg_val |=((sun7i_iis.pcm_start_slot - 1)&0x3)<<6;		//start slot index
		
	reg_val |= sun7i_iis.pcm_lsb_first<<9;			//MSB or LSB first
		
	if(sun7i_iis.pcm_sync_period == 256)
		reg_val |= 0x4<<12;
	else if (sun7i_iis.pcm_sync_period == 128)
		reg_val |= 0x3<<12;
	else if (sun7i_iis.pcm_sync_period == 64)
		reg_val |= 0x2<<12;
	else if (sun7i_iis.pcm_sync_period == 32)
		reg_val |= 0x1<<12;
	writel(reg_val, sun7i_iis.regs + SUN7I_IISFAT1);
	
	/* set FIFO control register */
	reg_val = 1 & 0x3;
	reg_val |= (1 & 0x1)<<2;
	reg_val |= SUN7I_IISFCTL_RXTL(0xf);				//RX FIFO trigger level
	reg_val |= SUN7I_IISFCTL_TXTL(0x40);				//TX FIFO empty trigger level
	writel(reg_val, sun7i_iis.regs + SUN7I_IISFCTL);
	return 0;
}

static int sun7i_i2s_hw_params(struct snd_pcm_substream *substream,
																struct snd_pcm_hw_params *params,
																struct snd_soc_dai *dai)
{
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun7i_dma_params *dma_data;
	
	/* play or record */
	if(substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dma_data = &sun7i_i2s_pcm_stereo_out;
	else
		dma_data = &sun7i_i2s_pcm_stereo_in;
	
	snd_soc_dai_set_dma_data(rtd->cpu_dai, substream, dma_data);
	return 0;
}

static int sun7i_i2s_trigger(struct snd_pcm_substream *substream,
                              int cmd, struct snd_soc_dai *dai)
{
	int ret = 0;
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_RESUME:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			
			if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
				sun7i_snd_rxctrl_i2s(substream, 1);
			} else {
				sun7i_snd_txctrl_i2s(substream, 1);
			}
			
			break;
		case SNDRV_PCM_TRIGGER_STOP:
		case SNDRV_PCM_TRIGGER_SUSPEND:
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			
			if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
				sun7i_snd_rxctrl_i2s(substream, 0);
				/*printk("[IIS]SPECIAL CLK 0x01c20068 = %#x, line= %d\n", *(volatile int*)0xF1C20068, __LINE__);
				printk("[IIS]SPECIAL CLK 0x01c200B8 = %#x, line = %d\n", *(volatile int*)0xF1C200B8, __LINE__);*/
			} else {
			  sun7i_snd_txctrl_i2s(substream, 0);
			}
			break;
		default:
			ret = -EINVAL;
			break;
	}

	return ret;
}

//freq:   1: 22.5792MHz   0: 24.576MHz  
static int sun7i_i2s_set_sysclk(struct snd_soc_dai *cpu_dai, int clk_id, 
                                 unsigned int freq, int dir)
{	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	if (!freq) {
		clk_set_rate(i2s_pll2clk, 24576000);
	} else {
		clk_set_rate(i2s_pll2clk, 22579200);
	}

	return 0;
}

static int sun7i_i2s_set_clkdiv(struct snd_soc_dai *cpu_dai, int div_id, int div)
{
	u32 reg;
	/*pr_info("sun7i-i2s::func:%s(line:%d)\n",__func__,__LINE__);*/
	switch (div_id) {
		case SUN7I_DIV_MCLK:
			if(div <= 8)
				div  = (div >>1);
			else if(div  == 12)
				div  = 0x5;
			else if(div  == 16)
				div  = 0x6;
			else if(div == 24)
				div = 0x7;
			else if(div == 32)
				div = 0x8;
			else if(div == 48)
				div = 0x9;
			else if(div == 64)
				div = 0xa;
			reg = (readl(sun7i_iis.regs + SUN7I_IISCLKD) & ~SUN7I_IISCLKD_MCLK_MASK) | (div << SUN7I_IISCLKD_MCLK_OFFS);
			writel(reg, sun7i_iis.regs + SUN7I_IISCLKD);
			break;
		case SUN7I_DIV_BCLK:
			if(div <= 8)
				div = (div>>1) - 1;
			else if(div == 12)
				div = 0x4;
			else if(div == 16)
				div = 0x5;
			else if(div == 32)
				div = 0x6;
			else if(div == 64)
				div = 0x7;
			reg = (readl(sun7i_iis.regs + SUN7I_IISCLKD) & ~SUN7I_IISCLKD_BCLK_MASK) | (div <<SUN7I_IISCLKD_BCLK_OFFS);
			writel(reg, sun7i_iis.regs + SUN7I_IISCLKD);
			break;
		default:
			return -EINVAL;
	}
	
	//diable MCLK output when high samplerate
	reg = readl(sun7i_iis.regs + SUN7I_IISCLKD);
	if (!(reg & 0xF)) {
		reg &= ~SUN7I_IISCLKD_MCLKOEN;
		writel(reg, sun7i_iis.regs + SUN7I_IISCLKD);
	} else {
		reg |= SUN7I_IISCLKD_MCLKOEN;
		writel(reg, sun7i_iis.regs + SUN7I_IISCLKD);
	}
	
	return 0;
}

static int sun7i_i2s_dai_probe(struct snd_soc_dai *dai)
{			
	return 0;
}
static int sun7i_i2s_dai_remove(struct snd_soc_dai *dai)
{
	return 0;
}

static void iisregsave(void)
{
	regsave[0] = readl(sun7i_iis.regs + SUN7I_IISCTL);
	regsave[1] = readl(sun7i_iis.regs + SUN7I_IISFAT0);
	regsave[2] = readl(sun7i_iis.regs + SUN7I_IISFAT1);
	regsave[3] = readl(sun7i_iis.regs + SUN7I_IISFCTL) | (0x3<<24);
	regsave[4] = readl(sun7i_iis.regs + SUN7I_IISINT);
	regsave[5] = readl(sun7i_iis.regs + SUN7I_IISCLKD);
	regsave[6] = readl(sun7i_iis.regs + SUN7I_TXCHSEL);
	regsave[7] = readl(sun7i_iis.regs + SUN7I_TXCHMAP);
}

static void iisregrestore(void)
{
	writel(regsave[0], sun7i_iis.regs + SUN7I_IISCTL);
	writel(regsave[1], sun7i_iis.regs + SUN7I_IISFAT0);
	writel(regsave[2], sun7i_iis.regs + SUN7I_IISFAT1);
	writel(regsave[3], sun7i_iis.regs + SUN7I_IISFCTL);
	writel(regsave[4], sun7i_iis.regs + SUN7I_IISINT);
	writel(regsave[5], sun7i_iis.regs + SUN7I_IISCLKD);
	writel(regsave[6], sun7i_iis.regs + SUN7I_TXCHSEL);
	writel(regsave[7], sun7i_iis.regs + SUN7I_TXCHMAP);
}

static int sun7i_i2s_suspend(struct snd_soc_dai *cpu_dai)
{
	u32 reg_val;
	printk("[IIS]Entered %s\n", __func__);

	//Global Enable Digital Audio Interface
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	reg_val &= ~SUN7I_IISCTL_GEN;
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);

	iisregsave();
	
	//release the module clock
	clk_disable(i2s_moduleclk);

	clk_disable(i2s_apbclk);
	
	//printk("[IIS]PLL2 0x01c20008 = %#x\n", *(volatile int*)0xF1C20008);
	/*printk("[IIS]SPECIAL CLK 0x01c20068 = %#x, line= %d\n", *(volatile int*)0xF1C20068, __LINE__);
	printk("[IIS]SPECIAL CLK 0x01c200B8 = %#x, line = %d\n", *(volatile int*)0xF1C200B8, __LINE__);*/
	
	return 0;
}
static int sun7i_i2s_resume(struct snd_soc_dai *cpu_dai)
{
	u32 reg_val;
	printk("[IIS]Entered %s\n", __func__);

	//release the module clock
	clk_enable(i2s_apbclk);

	//release the module clock
	clk_enable(i2s_moduleclk);
	
	iisregrestore();
	
	//Global Enable Digital Audio Interface
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	reg_val |= SUN7I_IISCTL_GEN;
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	
	//printk("[IIS]PLL2 0x01c20008 = %#x\n", *(volatile int*)0xF1C20008);
	printk("[IIS]SPECIAL CLK 0x01c20068 = %#x, line= %d\n", *(volatile int*)0xF1C20068, __LINE__);
	printk("[IIS]SPECIAL CLK 0x01c200B8 = %#x, line = %d\n", *(volatile int*)0xF1C200B8, __LINE__);
	
	return 0;
}

#define SUN7I_I2S_RATES (SNDRV_PCM_RATE_8000_192000 | SNDRV_PCM_RATE_KNOT)
static struct snd_soc_dai_ops sun7i_iis_dai_ops = {
	.trigger 	= sun7i_i2s_trigger,
	.hw_params 	= sun7i_i2s_hw_params,
	.set_fmt 	= sun7i_i2s_set_fmt,
	.set_clkdiv = sun7i_i2s_set_clkdiv,
	.set_sysclk = sun7i_i2s_set_sysclk, 
};

static struct snd_soc_dai_driver sun7i_iis_dai = {	
	.probe 		= sun7i_i2s_dai_probe,
	.suspend 	= sun7i_i2s_suspend,
	.resume 	= sun7i_i2s_resume,
	.remove 	= sun7i_i2s_dai_remove,
	.playback 	= {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SUN7I_I2S_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.capture 	= {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SUN7I_I2S_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	
	.ops 		= &sun7i_iis_dai_ops,	
};		

static int __devinit sun7i_i2s_dev_probe(struct platform_device *pdev)
{
	int reg_val = 0;
	int ret;
	
	sun7i_iis.regs = ioremap(SUN7I_IISBASE, 0x100);
	if (sun7i_iis.regs == NULL)
		return -ENXIO;

	//i2s apbclk
	i2s_apbclk = clk_get(NULL, CLK_APB_I2S0);
	
	if(!i2s_apbclk || IS_ERR(i2s_apbclk)){
	
	/* »ñÈ¡Ê±ÖÓ¾ä±úÊ§°Ü */
	
	printk("try to get CLK_APB_I2S0 failed!\n");
	
	}
	
	if(-1 == clk_enable(i2s_apbclk)){
		printk("i2s_apbclk failed! line = %d\n", __LINE__);
		goto out;
	}

	i2s_pllx8 = clk_get(NULL, CLK_SYS_PLL2X8);
	if(!i2s_pllx8 || IS_ERR(i2s_pllx8)){
	
	/* »ñÈ¡Ê±ÖÓ¾ä±úÊ§°Ü */
	
	printk("try to get CLK_SYS_PLL2X8 failed!\n");
	
	}
	
	//i2s pll2clk
	i2s_pll2clk = clk_get(NULL, CLK_SYS_PLL2);
	if(!i2s_pll2clk || IS_ERR(i2s_pll2clk)){
	
	/* »ñÈ¡Ê±ÖÓ¾ä±úÊ§°Ü */
	
	printk("try to get CLK_SYS_PLL2 failed!\n");
	
	}
	
	//i2s module clk
	i2s_moduleclk = clk_get(NULL, CLK_MOD_I2S0);
	if(!i2s_moduleclk || IS_ERR(i2s_moduleclk)){
	
	/* »ñÈ¡Ê±ÖÓ¾ä±úÊ§°Ü */
	printk("try to get CLK_MOD_I2S0 failed!\n");
	
	}
	
	if(clk_set_parent(i2s_moduleclk, i2s_pll2clk)){
		printk("try to set parent of i2s_moduleclk to i2s_pll2ck failed! line = %d\n",__LINE__);
		goto out1;
	}
	
	if(clk_set_rate(i2s_moduleclk, 24576000/8)){
		printk("set i2s_moduleclk clock freq to 24576000 failed! line = %d\n", __LINE__);
		goto out1;
	}
	
	if(-1 == clk_enable(i2s_moduleclk)){
		printk("open i2s_moduleclk failed! line = %d\n", __LINE__);
		goto out1;
	}
	
	reg_val = readl(sun7i_iis.regs + SUN7I_IISCTL);
	reg_val |= SUN7I_IISCTL_GEN;
	writel(reg_val, sun7i_iis.regs + SUN7I_IISCTL);
	
	/*iounmap(sun7i_iis.ioregs);*/
	ret = snd_soc_register_dai(&pdev->dev, &sun7i_iis_dai);	
	if (ret) {
		dev_err(&pdev->dev, "Failed to register DAI\n");
		goto out2;
	}

	goto out;
	out2:
		clk_disable(i2s_moduleclk);
	out1:
		clk_disable(i2s_apbclk);
	out:
	return 0;
}

static int __devexit sun7i_i2s_dev_remove(struct platform_device *pdev)
{
	if(i2s_used) {
		i2s_used = 0;
		/*release the module clock*/
		clk_disable(i2s_moduleclk);
		
		/*release pllx8clk*/
		clk_put(i2s_pllx8);
		
		/*release pll2clk*/
		clk_put(i2s_pll2clk);
		
		/*release apbclk*/
		clk_put(i2s_apbclk);
		
		/*gpio_release(i2s_handle, 2);*/		
		snd_soc_unregister_dai(&pdev->dev);
		platform_set_drvdata(pdev, NULL);
	}
	return 0;
}

/*data relating*/
static struct platform_device sun7i_i2s_device = {
	.name = "sun7i-i2s",
};

/*method relating*/
static struct platform_driver sun7i_i2s_driver = {
	.probe = sun7i_i2s_dev_probe,
	.remove = __devexit_p(sun7i_i2s_dev_remove),
	.driver = {
		.name = "sun7i-i2s",
		.owner = THIS_MODULE,
	},
};

static int __init sun7i_i2s_init(void)
{	
	int err = 0;
	int cnt = 0;
	int i 	= 0;
	script_item_u val;
	script_item_u *list = NULL;
	script_item_value_type_e  type;

	type = script_get_item("i2s_para", "i2s_used", &val);
	if (SCIRPT_ITEM_VALUE_TYPE_INT != type) {
        printk("[I2S] type err!\n");
    }

	i2s_used = val.val;
 	if (i2s_used) {
		/* get gpio list */
		cnt = script_get_pio_list("i2s_para", &list);
		if (0 == cnt) {
			printk("get i2s_para gpio list failed\n");
			return -EFAULT;
		}
	/* req gpio */
	for (i = 0; i < cnt; i++) {
		if (0 != gpio_request(list[i].gpio.gpio, NULL)) {
			printk("[i2s] request some gpio fail\n");
			goto end;
		}
	}
	/* config gpio list */
	if (0 != sw_gpio_setall_range(&list[0].gpio, cnt)) {
		printk("sw_gpio_setall_range failed\n");
	}

	if((err = platform_device_register(&sun7i_i2s_device)) < 0)
		return err;

	if ((err = platform_driver_register(&sun7i_i2s_driver)) < 0)
			return err;	
	} else {
        printk("[I2S]sun7i-i2s cannot find any using configuration for controllers, return directly!\n");
        return 0;
    }

end:
	/* release gpio */
	while(i--)
		gpio_free(list[i].gpio.gpio);

	return 0;
}

module_init(sun7i_i2s_init);

static void __exit sun7i_i2s_exit(void)
{	
	platform_driver_unregister(&sun7i_i2s_driver);
}
module_exit(sun7i_i2s_exit);

/* Module information */
MODULE_AUTHOR("REUUIMLLA");
MODULE_DESCRIPTION("sun7i I2S SoC Interface");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sun7i-i2s");

