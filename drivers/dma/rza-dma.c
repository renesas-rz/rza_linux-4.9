/*
 * Renesas RZA1 DMA Engine support
 *
 * base is drivers/dma/imx-dma.c
 *
 * Copyright (C) 2009-2017 Renesas Electronics
 * Copyright 2012 Javier Martin, Vista Silicon <javier.martin@vista-silicon.com>
 * Copyright (C) 2011-2012 Guennadi Liakhovetski <g.liakhovetski@gmx.de>
 * Copyright 2010 Sascha Hauer, Pengutronix <s.hauer@pengutronix.de>
 * Copyright (C) 2009 Nobuhiro Iwamatsu <iwamatsu.nobuhiro@renesas.com>
 * Copyright (C) 2007 Freescale Semiconductor, Inc. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 */
#define	END_DMA_WITH_LE		0	/* 1: LE=1, 0: LV=0 */
#define	IRQ_EACH_TX_WITH_DEM	1	/* 1: DEM=0, 0: DIM=0 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/dmaengine.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_dma.h>

#include <asm/irq.h>

#include "dmaengine.h"
#define RZA1DMA_MAX_CHAN_DESCRIPTORS	16

#include <linux/scatterlist.h>
#include <linux/device.h>
#include <linux/dmaengine.h>


/* Call the callback function from within a thread instead of a standard tasklet.
 * The reason is because of upstream commit
 *	52ad9a8e854c ("mmc: host: tmio: ensure end of DMA and SD access are in sync")
 * This commit now uses a wait_for_completion in the DMA callback which when called in
 * the contex of a tasklet causes a "BUG: scheduling while atomic".
 * Therefore we have no choice but to use a threaded IRQ for callbacks like the
 * R-Car DMA driver.
 */
#define THREADED_CALLBACK

static void rza1dma_tasklet(unsigned long data);	//asdf

/* DMA slave IDs */
enum {
	RZA1DMA_SLAVE_PCM_MEM_SSI0 = 1,	/* DMA0		MEM->(DMA0)->SSI0 */
	RZA1DMA_SLAVE_PCM_MEM_SRC1,		/* DMA1		MEM->(DMA1)->FFD0_1->SRC1->SSI0 */
	RZA1DMA_SLAVE_PCM_SSI0_MEM,		/* DMA2		SSI0->(DMA2)->MEM */
	RZA1DMA_SLAVE_PCM_SRC0_MEM,		/* DMA3		SSI0->SRC0->FFU0_0->(DMA3)->MEM */
	RZA1DMA_SLAVE_PCM_MAX,
	RZA1DMA_SLAVE_SDHI0_TX,
	RZA1DMA_SLAVE_SDHI0_RX,
	RZA1DMA_SLAVE_SDHI1_TX,
	RZA1DMA_SLAVE_SDHI1_RX,
	RZA1DMA_SLAVE_MMCIF_TX,
	RZA1DMA_SLAVE_MMCIF_RX,
};

union chcfg_reg {
	u32	v;
	struct {
		u32 sel:  3;	/* LSB */
		u32 reqd: 1;
		u32 loen: 1;
		u32 hien: 1;
		u32 lvl:  1;
		u32 _mbz0:1;
		u32 am:   3;
		u32 _mbz1:1;
		u32 sds:  4;
		u32 dds:  4;
		u32 _mbz2:2;
		u32 tm:   1;
		u32 _mbz3:9;
	};
};

union dmars_reg {
	u32 v;
	struct {
		u32 rid:   2;	/* LSB */
		u32 mid:   7;
		u32 _mbz0:23;
	};
};

/*
 * Drivers, using this library are expected to embed struct shdma_dev,
 * struct shdma_chan, struct shdma_desc, and struct shdma_slave
 * in their respective device, channel, descriptor and slave objects.
 */

struct rza1dma_slave {
	int slave_id;
};

/* Used by slave DMA clients to request DMA to/from a specific peripheral */
struct rza1_dma_slave {
	struct rza1dma_slave	rza1dma_slaveid;	/* Set by the platform */
};

struct rza1_dma_slave_config {
	int			slave_id;
	dma_addr_t		addr;
	union chcfg_reg		chcfg;
	union dmars_reg		dmars;
};

/* Static array to hold our slaves */
// TODO: move into driver data structure
static struct rza1_dma_slave_config rza1_dma_slaves[20];

/* set the offset of regs */
#define	CHSTAT	0x0024
#define CHCTRL	0x0028
#define	CHCFG	0x002c
#define	CHITVL	0x0030
#define	CHEXT	0x0034
#define NXLA	0x0038
#define	CRLA	0x003c

#define	DCTRL		0x0000
#define	DSTAT_EN	0x0010
#define	DSTAT_ER	0x0014
#define	DSTAT_END	0x0018
#define	DSTAT_TC	0x001c
#define	DSTAT_SUS	0x0020

#define	EACH_CHANNEL_OFFSET		0x0040
#define	CHANNEL_0_7_OFFSET		0x0000
#define	CHANNEL_0_7_COMMON_BASE		0x0300
#define CHANNEL_8_15_OFFSET		0x0400
#define	CHANNEL_8_15_COMMON_BASE	0x0700

/* set bit filds */
/* CHSTAT */
#define	CHSTAT_TC	(0x1 << 6)
#define CHSTAT_END	(0x1 << 5)
#define CHSTAT_ER	(0x1 << 4)
#define CHSTAT_EN	(0x1 << 0)

/* CHCTRL */
#define CHCTRL_CLRINTMSK	(0x1 << 17)
#define	CHCTRL_SETINTMSK	(0x1 << 16)
#define	CHCTRL_CLRSUS		(0x1 << 9)
#define	CHCTRL_SETSUS		(0x1 << 8)
#define CHCTRL_CLRTC		(0x1 << 6)
#define	CHCTRL_CLREND		(0x1 << 5)
#define	CHCTRL_CLRRQ		(0x1 << 4)
#define	CHCTRL_SWRST		(0x1 << 3)
#define	CHCTRL_STG		(0x1 << 2)
#define	CHCTRL_CLREN		(0x1 << 1)
#define	CHCTRL_SETEN		(0x1 << 0)
#define	CHCTRL_DEFAULT	(CHCTRL_CLRINTMSK | \
			CHCTRL_CLRSUS | \
			CHCTRL_CLRTC | \
			CHCTRL_CLREND | \
			CHCTRL_CLRRQ | \
			CHCTRL_SWRST | \
			CHCTRL_CLREN)

/* CHCFG */
#define	CHCFG_DMS		(0x1 << 31)
#define	CHCFG_DEM		(0x1 << 24)
#define	CHCFG_DAD		(0x1 << 21)
#define	CHCFG_SAD		(0x1 << 20)
#define	CHCFG_8BIT		(0x00)
#define	CHCFG_16BIT		(0x01)
#define	CHCFG_32BIT		(0x02)
#define	CHCFG_64BIT		(0x03)
#define CHCFG_128BIT		(0x04)
#define	CHCFG_256BIT		(0x05)
#define	CHCFG_512BIT		(0x06)
#define	CHCFG_1024BIT		(0x07)
#define	CHCFG_SEL(bits)		((bits & 0x07) << 0)

/* DCTRL */
#define	DCTRL_LVINT		(0x1 << 1)
#define	DCTRL_PR		(0x1 << 0)
#define DCTRL_DEFAULT		(DCTRL_LVINT | DCTRL_PR)

/* DMARS */
#define	DMARS_RID(bit)		(bit << 0)
#define	DMARS_MID(bit)		(bit << 2)

/* LINK MODE DESCRIPTOR */
#define	HEADER_DIM	(0x1 << 3)
#define	HEADER_WBD	(0x1 << 2)
#define	HEADER_LE	(0x1 << 1)
#define	HEADER_LV	(0x1 << 0)

#define to_rza1dma_chan(c) container_of(c, struct dmac_channel, chan)

enum  rza1dma_prep_type {
	RZA1DMA_DESC_MEMCPY,
	RZA1DMA_DESC_SLAVE_SG,
};

struct lmdesc {
	u32 header;
	u32 sa;
	u32 da;
	u32 tb;
	u32 chcfg;
	u32 chitvl;
	u32 chext;
	u32 nxla;
};
#define	DMAC_NR_LMDESC		64

struct dmac_desc {
	struct list_head		node;
	struct dma_async_tx_descriptor	desc;
	enum dma_status			status;
	dma_addr_t			src;
	dma_addr_t			dest;
	size_t				len;
	enum dma_transfer_direction	direction;
	enum rza1dma_prep_type		type;
	/* For memcpy */
	unsigned int			config_port;
	unsigned int			config_mem;
	/* For slave sg */
	struct scatterlist		*sg;
	unsigned int			sgcount;
};

struct dmac_channel {
	struct rza1dma_engine		*rza1dma;
	unsigned int			nr;
	spinlock_t			lock;
	struct tasklet_struct		dma_tasklet;
	struct list_head		ld_free;
	struct list_head		ld_queue;
	struct list_head		ld_active;
	int				descs_allocated;
	enum dma_slave_buswidth		word_size;
	dma_addr_t			per_address;
	struct dma_chan			chan;
	struct dma_async_tx_descriptor	desc;
	enum dma_status			status;

	const struct rza1_dma_slave_config	*slave;
	void __iomem			*ch_base;
	void __iomem			*ch_cmn_base;
	struct {
		struct lmdesc *base;
		struct lmdesc *head;
		struct lmdesc *tail;
		int valid;
		dma_addr_t base_dma;
	} lmdesc;

	u32	chcfg;
	u32	chctrl;

	struct {
		int issue;
		int prep_slave_sg;
	} stat;
};

struct rza1dma_engine {
	struct device			*dev;
	struct device_dma_parameters	dma_parms;
	struct dma_device		dma_device;
	void __iomem			*base;
	void __iomem			*ext_base;
	int				irq;
	spinlock_t			lock;		/* unused now */
	struct dmac_channel		*channel;
	unsigned int			n_channels;
	const struct rza1_dma_slave_config *slave;
	int				slave_num;
};

static void rza1dma_writel(struct rza1dma_engine *rza1dma, unsigned val,
				unsigned offset)
{
	__raw_writel(val, rza1dma->base + offset);
}

static void rza1dma_ext_writel(struct rza1dma_engine *rza1dma, unsigned val,
				unsigned offset)
{
	__raw_writel(val, rza1dma->ext_base + offset);
}

static u32 rza1dma_ext_readl(struct rza1dma_engine *rza1dma, unsigned offset)
{
	return __raw_readl(rza1dma->ext_base + offset);
}

static void rza1dma_ch_writel(struct dmac_channel *channel, unsigned val,
				unsigned offset, int which)
{
	if (which)
		__raw_writel(val, channel->ch_base + offset);
	else
		__raw_writel(val, channel->ch_cmn_base + offset);
}

static u32 rza1dma_ch_readl(struct dmac_channel *channel, unsigned offset,
				int which)
{
	if (which)
		return __raw_readl(channel->ch_base + offset);
	else
		return __raw_readl(channel->ch_cmn_base + offset);
}

static void lmdesc_setup(struct dmac_channel *channel,
	struct lmdesc *lmdesc)
{
	u32 nxla;

	channel->lmdesc.base = lmdesc;
	channel->lmdesc.head = lmdesc;
	channel->lmdesc.tail = lmdesc;
	channel->lmdesc.valid = 0;
	nxla = channel->lmdesc.base_dma;
	while (lmdesc < (channel->lmdesc.base + (DMAC_NR_LMDESC - 1))) {
		lmdesc->header = 0;
		nxla += sizeof(*lmdesc);
		lmdesc->nxla = nxla;
		lmdesc++;
	}
	lmdesc->header = 0;
	lmdesc->nxla = channel->lmdesc.base_dma;
}

static void lmdesc_recycle(struct dmac_channel *channel)
{
	struct lmdesc *lmdesc = channel->lmdesc.head;

	while (!(lmdesc->header & HEADER_LV)) {
		lmdesc->header = 0;
		channel->lmdesc.valid--;
		lmdesc++;
		if (lmdesc >= (channel->lmdesc.base + DMAC_NR_LMDESC))
			lmdesc = channel->lmdesc.base;
	}
	channel->lmdesc.head = lmdesc;
}

static void rza1dma_enable_hw(struct dmac_desc *d)
{
	struct dma_chan *chan = d->desc.chan;
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	unsigned long flags;
	u32 nxla;
	u32 chctrl;
	u32 chstat;

	dev_dbg(rza1dma->dev, "%s channel %d\n", __func__, channel->nr);

	local_irq_save(flags);

	lmdesc_recycle(channel);

	nxla = channel->lmdesc.base_dma + (sizeof(struct lmdesc)
		* (channel->lmdesc.head - channel->lmdesc.base));

//	if (chctrl & CHCTRL_SETEN) {			/* When [SETEN]is "0".already before process add Descriptor */
//								/* Only add Descriptor case.skip write register */
	chstat = rza1dma_ch_readl(channel, CHSTAT, 1);
	if (!(chstat & CHSTAT_EN)) {

		chctrl = (channel->chctrl | CHCTRL_SETEN);
		rza1dma_ch_writel(channel, nxla, NXLA, 1);		/* NXLA reg */
		rza1dma_ch_writel(channel, channel->chcfg, CHCFG, 1);	/* CHCFG reg */
		rza1dma_ch_writel(channel, CHCTRL_SWRST, CHCTRL, 1);	/* CHCTRL reg */
		rza1dma_ch_writel(channel, chctrl, CHCTRL, 1);		/* CHCTRL reg */
	}

	local_irq_restore(flags);
}

static void rza1dma_disable_hw(struct dmac_channel *channel)
{
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	unsigned long flags;

	dev_dbg(rza1dma->dev, "%s channel %d\n", __func__, channel->nr);

	local_irq_save(flags);
	rza1dma_ch_writel(channel, CHCTRL_DEFAULT, CHCTRL, 1); /* CHCTRL reg */
	local_irq_restore(flags);
}

static bool dma_irq_handle_channel(struct dmac_channel *channel)
{
	u32 chstat, chctrl;
	struct rza1dma_engine *rza1dma = channel->rza1dma;

	chstat = rza1dma_ch_readl(channel, CHSTAT, 1);
	if (chstat & CHSTAT_ER) {
		dev_err(rza1dma->dev, "RZA1 DMAC error ocurred\n");
		dev_err(rza1dma->dev, "CHSTAT_%d = %08X\n", channel->nr, chstat);
		rza1dma_ch_writel(channel,
				CHCTRL_DEFAULT,
				CHCTRL, 1);
		goto schedule;
	}
//	if (!(chstat & CHSTAT_END)) {
//		dev_err(rza1dma->dev, "RZA1 DMAC premature IRQ (%x)\n", chstat);
//		return;
//	}

	chctrl = rza1dma_ch_readl(channel, CHCTRL, 1);
	rza1dma_ch_writel(channel,
			chctrl | CHCTRL_CLREND | CHCTRL_CLRRQ,
			CHCTRL, 1);
schedule:
#ifndef THREADED_CALLBACK
	/* Tasklet irq */
	tasklet_schedule(&channel->dma_tasklet);
#endif
	return true;
}

static irqreturn_t rza1dma_irq_handler(int irq, void *dev_id)
{
	struct rza1dma_engine *rza1dma = dev_id;
	int i, channel_num = rza1dma->n_channels;

	dev_dbg(rza1dma->dev, "%s called\n", __func__);

	i = irq - rza1dma->irq;
	if (i < channel_num) {	/* handle DMAINT irq */
#ifdef THREADED_CALLBACK
		if (dma_irq_handle_channel(&rza1dma->channel[i]))
			return IRQ_WAKE_THREAD;	/* start the tasklets from the thread */
#else
		dma_irq_handle_channel(&rza1dma->channel[i]);
		return IRQ_HANDLED;
#endif
	}
	/* handle DMAERR irq */
	return IRQ_HANDLED;
}

static void set_dmars_register(struct rza1dma_engine *rza1dma,
				int nr, u32 dmars)
{
	u32 dmars_offset = (nr / 2) * 4;
	u32 dmars32;

	dmars32 = rza1dma_ext_readl(rza1dma, dmars_offset);
	if (nr % 2) {
		dmars32 &= 0x0000ffff;
		dmars32 |= dmars << 16;
	} else {
		dmars32 &= 0xffff0000;
		dmars32 |= dmars;
	}
	rza1dma_ext_writel(rza1dma, dmars32, dmars_offset);

}

static void prepare_desc_for_memcpy(struct dmac_desc *d)
{
	struct dma_chan *chan = d->desc.chan;
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	struct lmdesc *lmdesc = channel->lmdesc.base;
	u32 chcfg = 0x80400008;
	u32 dmars = 0;

	dev_dbg(rza1dma->dev, "%s called\n", __func__);
	BUG_ON(1);

	lmdesc = channel->lmdesc.tail;

	/* prepare descriptor */
	lmdesc->sa = d->src;
	lmdesc->da = d->dest;
	lmdesc->tb = d->len;
	lmdesc->chcfg = chcfg;
	lmdesc->chitvl = 0;
	lmdesc->chext = 0;
#if END_DMA_WITH_LE
	lmdesc->header = HEADER_LV | HEADER_LE;
#else
	lmdesc->header = HEADER_LV;
#endif

	/* and set DMARS register */
	set_dmars_register(rza1dma, channel->nr, dmars);

	channel->chcfg = chcfg;
	channel->chctrl = CHCTRL_STG | CHCTRL_SETEN;
}

static void prepare_descs_for_slave_sg(struct dmac_desc *d)
{
	struct dma_chan *chan = d->desc.chan;
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	struct lmdesc *lmdesc;
	const struct rza1_dma_slave_config *slave = channel->slave;
	struct scatterlist *sg, *sgl = d->sg;
	unsigned int i, sg_len = d->sgcount;
	unsigned long flags;
	u32 chcfg;
	u32 dmars;
	
	dev_dbg(rza1dma->dev, "%s called\n", __func__);

	chcfg = channel->slave->chcfg.v | (CHCFG_SEL(channel->nr) | CHCFG_DEM | CHCFG_DMS);

	if (d->direction == DMA_DEV_TO_MEM)
		chcfg |= CHCFG_SAD;	/* source address is fixed */
	else
		chcfg |= CHCFG_DAD;	/* distation address is fixed */

	channel->chcfg = chcfg;			/* save */
	channel->per_address = slave->addr;	/* slave device address */

	spin_lock_irqsave(&channel->lock, flags);

	/* Prepare descriptors */
	lmdesc = channel->lmdesc.tail;

	for (i = 0, sg = sgl; i < sg_len; i++, sg = sg_next(sg)) {
		if (d->direction == DMA_DEV_TO_MEM) {
			lmdesc->sa = channel->per_address;
			lmdesc->da = sg_dma_address(sg);
		} else {
			lmdesc->sa = sg_dma_address(sg);
			lmdesc->da = channel->per_address;
		}

		lmdesc->tb = sg_dma_len(sg);
		lmdesc->chitvl = 0;
		lmdesc->chext = 0;
		if (i == (sg_len - 1)) {
#if IRQ_EACH_TX_WITH_DEM
			lmdesc->chcfg = (chcfg & ~CHCFG_DEM);
#else
			lmdesc->chcfg = chcfg;
#endif
#if END_DMA_WITH_LE
			lmdesc->header = (HEADER_LV | HEADER_LE);
#else
			lmdesc->header = HEADER_LV;
#endif
		} else {
			lmdesc->chcfg = chcfg;
			lmdesc->header = HEADER_LV;
		}
		if (++lmdesc >= (channel->lmdesc.base + DMAC_NR_LMDESC))
			lmdesc = channel->lmdesc.base;
	}
	channel->lmdesc.tail = lmdesc;

	/* and set DMARS register */
	dmars = channel->slave->dmars.v;
	set_dmars_register(rza1dma, channel->nr, dmars);

	channel->chctrl = CHCTRL_SETEN;		/* always */

	spin_unlock_irqrestore(&channel->lock, flags);

}

static int rza1dma_xfer_desc(struct dmac_desc *d)
{
	/* Configure and enable */
	switch (d->type) {
	case RZA1DMA_DESC_MEMCPY:
		prepare_desc_for_memcpy(d);
		break;

	case RZA1DMA_DESC_SLAVE_SG:
		prepare_descs_for_slave_sg(d);
		break;

	default:
		return -EINVAL;
	}

	rza1dma_enable_hw(d);
	return 0;
}

#ifdef THREADED_CALLBACK
static irqreturn_t rza1dma_irq_handler_thread(int irq, void *dev_id)
{
	struct rza1dma_engine *rza1dma = dev_id;
	int i, channel_num = rza1dma->n_channels;

	i = irq - rza1dma->irq;
	if (i < channel_num) {
		/* call tasklet directly in thread context */
		rza1dma_tasklet((unsigned long) &rza1dma->channel[i]);
	}

	return IRQ_HANDLED;
}
#endif

static void rza1dma_tasklet(unsigned long data)
{
	struct dmac_channel *channel = (void *)data;
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	struct dmac_desc *desc;
	unsigned long flags;

	dev_dbg(rza1dma->dev, "%s called\n", __func__);

	/* Protection of critical section */
	spin_lock_irqsave(&channel->lock, flags);

	if (list_empty(&channel->ld_active)) {
		/* Someone might have called terminate all */
		goto out;
	}

	desc = list_first_entry(&channel->ld_active, struct dmac_desc, node);

#ifndef THREADED_CALLBACK
	if (desc->desc.callback)
		desc->desc.callback(desc->desc.callback_param);
#endif

	dma_cookie_complete(&desc->desc);


	if (list_empty(&channel->ld_active)) {
		goto out;
	} else {
		list_move_tail(channel->ld_active.next, &channel->ld_free);
	}

	if (!list_empty(&channel->ld_queue)) {
		desc = list_first_entry(&channel->ld_queue, struct dmac_desc,
					node);
		if (rza1dma_xfer_desc(desc) < 0) {
			dev_warn(rza1dma->dev, "%s: channel: %d couldn't xfer desc\n",
				 __func__, channel->nr);
		} else {
			list_move_tail(channel->ld_queue.next, &channel->ld_active);
		}
	}
out:
	spin_unlock_irqrestore(&channel->lock, flags);
#ifdef THREADED_CALLBACK
	dmaengine_desc_get_callback_invoke(&desc->desc, NULL);
#endif
}

static int rza1dma_config(struct dma_chan *chan, struct dma_slave_config *config)
{
	struct dmac_channel *rza1dmac = to_rza1dma_chan(chan);
	if (config->direction == DMA_DEV_TO_MEM) {
		rza1dmac->per_address = config->src_addr;
		rza1dmac->word_size = config->src_addr_width;
	} else {
		rza1dmac->per_address = config->dst_addr;
		rza1dmac->word_size = config->dst_addr_width;
	}
	return 0;
}

static int rza1dma_terminate_all(struct dma_chan *chan)
{
	struct dmac_channel *rza1dmac = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = rza1dmac->rza1dma;
	unsigned long flags;
	rza1dma_disable_hw(rza1dmac);
	spin_lock_irqsave(&rza1dma->lock, flags);
	list_splice_tail_init(&rza1dmac->ld_active, &rza1dmac->ld_free);
	list_splice_tail_init(&rza1dmac->ld_queue, &rza1dmac->ld_free);
	spin_unlock_irqrestore(&rza1dma->lock, flags);
	return 0;
}

static const struct rza1_dma_slave_config *dma_find_slave(
		const struct rza1_dma_slave_config *slave,
		int slave_num,
		int slave_id)
{
	int i;

	for (i = 0; i < slave_num; i++) {
		const struct rza1_dma_slave_config *t = &slave[i];

		if (slave_id == t->slave_id)
			return t;
	}

	return NULL;
}

bool rza1dma_chan_filter(struct dma_chan *chan, void *arg)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	const struct rza1_dma_slave_config *slave = rza1dma->slave;
	const struct rza1_dma_slave_config *hit;
	int slave_num = rza1dma->slave_num;
	int slave_id = (int)arg;

	struct of_phandle_args *dma_spec = arg;
	slave_id = dma_spec->args[0];

	hit = dma_find_slave(slave, slave_num, slave_id);
	if (hit) {
		channel->slave = hit;
		return true;
	}
	return false;
}
EXPORT_SYMBOL(rza1dma_chan_filter);

/* called through rza1dma->dma_device.device_tx_status */
static enum dma_status rza1dma_tx_status(struct dma_chan *chan,
					dma_cookie_t cookie,
					struct dma_tx_state *txstate)
{
	return dma_cookie_status(chan, cookie, txstate);
}

static dma_cookie_t rza1dma_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct dma_chan *chan = tx->chan;
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	dma_cookie_t cookie;
	unsigned long flags;

	spin_lock_irqsave(&channel->lock, flags);
	list_move_tail(channel->ld_free.next, &channel->ld_queue);
	cookie = dma_cookie_assign(tx);
	spin_unlock_irqrestore(&channel->lock, flags);

	return cookie;
}

/* called through rza1dma->dma_device.device_alloc_chan_resources */
static int rza1dma_alloc_chan_resources(struct dma_chan *chan)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	const struct rza1_dma_slave_config *slave = rza1dma->slave;
	const struct rza1_dma_slave_config *hit;
	int slave_num = rza1dma->slave_num;
	int *slave_id = chan->private;

	if (slave_id) {
		hit = dma_find_slave(slave, slave_num, *slave_id);
		if (!hit)
			return -ENODEV;
		channel->slave = hit;
	}

	while (channel->descs_allocated < RZA1DMA_MAX_CHAN_DESCRIPTORS) {
		struct dmac_desc *desc;

		desc = kzalloc(sizeof(*desc), GFP_KERNEL);
		if (!desc)
			break;
		__memzero(&desc->desc, sizeof(struct dma_async_tx_descriptor));
		dma_async_tx_descriptor_init(&desc->desc, chan);
		desc->desc.tx_submit = rza1dma_tx_submit;
		/* txd.flags will be overwritten in prep funcs */
		desc->desc.flags = DMA_CTRL_ACK;
		desc->status = DMA_COMPLETE;

		list_add_tail(&desc->node, &channel->ld_free);
		channel->descs_allocated++;
	}
	if (!channel->descs_allocated)
		return -ENOMEM;

	return channel->descs_allocated;
}

/* called through rza1dma->dma_device.device_free_chan_resources */
static void rza1dma_free_chan_resources(struct dma_chan *chan)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct lmdesc *lmdesc = channel->lmdesc.base;
	struct dmac_desc *desc, *_desc;
	unsigned long flags;
	unsigned int i;

	spin_lock_irqsave(&channel->lock, flags);

	for (i = 0; i < DMAC_NR_LMDESC; i++) {
		lmdesc[i].header = 0;
	}

	rza1dma_disable_hw(channel);
	list_splice_tail_init(&channel->ld_active, &channel->ld_free);
	list_splice_tail_init(&channel->ld_queue, &channel->ld_free);

	spin_unlock_irqrestore(&channel->lock, flags);

	list_for_each_entry_safe(desc, _desc, &channel->ld_free, node) {
		kfree(desc);
		channel->descs_allocated--;
	}
	INIT_LIST_HEAD(&channel->ld_free);
}

/* called through rza1dma->dma_device.device_prep_slave_sg */
static struct dma_async_tx_descriptor *rza1dma_prep_slave_sg(
	struct dma_chan *chan, struct scatterlist *sgl,
	unsigned int sg_len, enum dma_transfer_direction direction,
	unsigned long flags, void *context)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct scatterlist *sg;
	int i, dma_length = 0;
	struct dmac_desc *desc;

	if (list_empty(&channel->ld_free))
		return NULL;

	desc = list_first_entry(&channel->ld_free, struct dmac_desc, node);

	for_each_sg(sgl, sg, sg_len, i) {
		dma_length += sg_dma_len(sg);
	}

	desc->type = RZA1DMA_DESC_SLAVE_SG;
	desc->sg = sgl;
	desc->sgcount = sg_len;
	desc->len = dma_length;
	desc->direction = direction;

	if (direction == DMA_DEV_TO_MEM)
		desc->src = channel->per_address;
	else
		desc->dest = channel->per_address;

	desc->desc.callback = NULL;
	desc->desc.callback_param = NULL;

	return &desc->desc;
}

/* called through rza1dma->dma_device.device_prep_dma_memcpy */
static struct dma_async_tx_descriptor *rza1dma_prep_dma_memcpy(
	struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
	size_t len, unsigned long flags)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	struct dmac_desc *desc;

	dev_dbg(rza1dma->dev, "%s channel: %d src=0x%x dst=0x%x len=%d\n",
			__func__, channel->nr, src, dest, len);

	if (list_empty(&channel->ld_free))
		return NULL;

	desc = list_first_entry(&channel->ld_free, struct dmac_desc, node);

	desc->type = RZA1DMA_DESC_MEMCPY;
	desc->src = src;
	desc->dest = dest;
	desc->len = len;
	desc->direction = DMA_MEM_TO_MEM;
	desc->desc.callback = NULL;
	desc->desc.callback_param = NULL;

	return &desc->desc;
}

static struct dma_chan *rza1dma_of_xlate(struct of_phandle_args *dma_spec,
					   struct of_dma *ofdma)
{
	struct dma_chan *chan;
	dma_cap_mask_t mask;

	if (dma_spec->args_count != 1)
		return NULL;

	/* Only slave DMA channels can be allocated via DT */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	chan = dma_request_channel(mask, rza1dma_chan_filter, dma_spec);
	if (!chan)
		return NULL;

	return chan;
}

/* called through rza1dma->dma_device.device_issue_pending */
static void rza1dma_issue_pending(struct dma_chan *chan)
{
	struct dmac_channel *channel = to_rza1dma_chan(chan);
	struct rza1dma_engine *rza1dma = channel->rza1dma;
	struct dmac_desc *desc;
	unsigned long flags;

	spin_lock_irqsave(&channel->lock, flags);

	/* queue is piled up on the next active even during execution DMA forwarding */
	if (!list_empty(&channel->ld_queue)) {
		desc = list_first_entry(&channel->ld_queue,
					struct dmac_desc, node);

		if (rza1dma_xfer_desc(desc) < 0) {
			dev_warn(rza1dma->dev,
				 "%s: channel: %d couldn't issue DMA xfer\n",
				 __func__, channel->nr);
		} else {
			list_move_tail(channel->ld_queue.next,
				       &channel->ld_active);
		}
	}
	spin_unlock_irqrestore(&channel->lock, flags);
}

static int rza1dma_parse_of(struct device *dev, struct rza1dma_engine *rza1dma)
{

	int slave_id[10];
	u32 addr_slave[10];
	int chcfg[80];
	int dmars[20];
	int i = 0;
	int n_slaves;
	int ret;

	struct device_node *np = dev->of_node;

	ret = of_property_read_u32(np, "dma-channels", &rza1dma->n_channels);
	if (ret < 0) {
		dev_err(dev, "unable to read dma-channels property\n");
		return ret;
	}

	if (rza1dma->n_channels <= 0 || rza1dma->n_channels >= 100) {
		dev_err(dev, "invalid number of channels %u\n",
			rza1dma->n_channels);
		return -EINVAL;
	}

	n_slaves = of_property_count_elems_of_size(np, "slave_id", sizeof(u32));

	if (of_property_read_u32_array(np, "slave_id", slave_id, n_slaves)) {
		dev_err(dev, "get get id fail \n");
		return -ENOMEM;
	}

	if (of_property_read_u32_array(np, "addr", addr_slave, n_slaves)) {
		dev_err(dev, "get addr_slave fail \n");
		return -ENOMEM;
	}

	if (of_property_read_u32_array(np, "chcfg", chcfg, n_slaves*8)) {
		dev_err(dev, "get chcfg fail \n");
		return -ENOMEM;
	}

	if (of_property_read_u32_array(np, "dmars", dmars, n_slaves*2)) {
		dev_err(dev, "get dmars fail \n");
		return -ENOMEM;
	}

	// TODO: change to kzalloc or just hard code in the struct
	for(i = 0; i < n_slaves; i++)
	{
		rza1_dma_slaves[i].slave_id = slave_id[i];
		rza1_dma_slaves[i].addr = addr_slave[i];

		rza1_dma_slaves[i].chcfg.reqd	= chcfg[i*8];
		rza1_dma_slaves[i].chcfg.loen	= chcfg[i*8+1];
		rza1_dma_slaves[i].chcfg.hien	= chcfg[i*8+2];
		rza1_dma_slaves[i].chcfg.lvl	= chcfg[i*8+3];
		rza1_dma_slaves[i].chcfg.am	= chcfg[i*8+4];
		rza1_dma_slaves[i].chcfg.sds	= chcfg[i*8+5];
		rza1_dma_slaves[i].chcfg.dds	= chcfg[i*8+6];
		rza1_dma_slaves[i].chcfg.tm	= chcfg[i*8+7];

		rza1_dma_slaves[i].dmars.rid	= dmars[i*2];					\
		rza1_dma_slaves[i].dmars.mid	= dmars[i*2+1];
	}

	rza1dma->slave = rza1_dma_slaves;
	rza1dma->slave_num = n_slaves;

	return 0;
}

const char irqnames[16][16] = {
"rza-dma: ch0", "rza-dma: ch1", "rza-dma: ch2", "rza-dma: ch3", "rza-dma: ch4",
"rza-dma: ch5", "rza-dma: ch6", "rza-dma: ch7", "rza-dma: ch8", "rza-dma: ch9",
"rza-dma: ch10", "rza-dma: ch11", "rza-dma: ch12", "rza-dma: ch13", "rza-dma: ch14",
"rza-dma: ch15" };
static int rza1dma_probe(struct platform_device *pdev)
{
	struct rza1dma_engine *rza1dma;
	int channel_num;
	struct resource *mem;
	char *irqname;
	int ret, i;
	int irq;
	char pdev_irqname[50];

	rza1dma = devm_kzalloc(&pdev->dev, sizeof(*rza1dma), GFP_KERNEL);
	if (!rza1dma)
		return -ENOMEM;

	platform_set_drvdata(pdev, rza1dma);

	ret = rza1dma_parse_of(&pdev->dev, rza1dma);
	if (ret < 0)
		return ret;

	channel_num = rza1dma->n_channels;

	/* Request resources */
	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	rza1dma->base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(rza1dma->base))
		return PTR_ERR(rza1dma->base);

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	rza1dma->ext_base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(rza1dma->ext_base))
		return PTR_ERR(rza1dma->ext_base);

	/* Register interrupt handler for error */
	irq = platform_get_irq_byname(pdev, "error");
	if (irq < 0) {
		dev_err(&pdev->dev, "no error IRQ specified\n");
		return -ENODEV;
	}

//	irqname = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s:error",
//				 dev_name(rza1dma->dev));
//devm_kasprintf causes crash
	irqname = "rza-dma: error";

	if (!irqname)
		return -ENOMEM;

	ret = devm_request_irq(&pdev->dev, irq, rza1dma_irq_handler, 0,
			       irqname, rza1dma);
	if (ret) {
		dev_err(&pdev->dev, "failed to request IRQ %u (%d)\n",
			irq, ret);
		return ret;
	}

	INIT_LIST_HEAD(&rza1dma->dma_device.channels);
	dma_cap_set(DMA_SLAVE, rza1dma->dma_device.cap_mask);
	dma_cap_set(DMA_MEMCPY, rza1dma->dma_device.cap_mask);
	spin_lock_init(&rza1dma->lock);

	rza1dma->channel = devm_kzalloc(&pdev->dev,
				sizeof(struct dmac_channel) * channel_num,
				GFP_KERNEL);

	/* Initialize channel parameters */
	for (i = 0; i < channel_num; i++) {
		struct dmac_channel *channel = &rza1dma->channel[i];
		struct lmdesc *lmdesc;

		channel->rza1dma = rza1dma;

		/* Request the channel interrupt. */
		sprintf(pdev_irqname, "ch%u", i);
		irq = platform_get_irq_byname(pdev, pdev_irqname);
		if (irq < 0) {
			dev_err(rza1dma->dev, "no IRQ specified for channel %u\n", i);
			return -ENODEV;
		}

		/* save the IRQ ID of channel 0 */
		if (i == 0)
			rza1dma->irq = irq;

//		irqname = devm_kasprintf(&pdev->dev, GFP_KERNEL, "%s:%u",
//					 dev_name(&pdev->dev), i);
//devm_kasprintf causes crash
		//irqname = "rza-dma: ch";

		if (!irqname)
			return -ENOMEM;

#ifdef THREADED_CALLBACK
		ret = devm_request_threaded_irq(&pdev->dev, irq, rza1dma_irq_handler,
				rza1dma_irq_handler_thread, 0, irqnames[i], rza1dma);
				//rza1dma_irq_handler_thread, 0, dev_name(&pdev->dev), rza1dma);
#else
		ret = devm_request_irq(&pdev->dev, irq, rza1dma_irq_handler, 0,
				dev_name(&pdev->dev), rza1dma);
#endif
		if (ret) {
			dev_err(rza1dma->dev, "failed to request IRQ %u (%d)\n", irq, ret);
			return ret;
		}

		INIT_LIST_HEAD(&channel->ld_queue);
		INIT_LIST_HEAD(&channel->ld_free);
		INIT_LIST_HEAD(&channel->ld_active);

		spin_lock_init(&channel->lock);
		tasklet_init(&channel->dma_tasklet, rza1dma_tasklet,
			     (unsigned long)channel);
		channel->chan.device = &rza1dma->dma_device;
		dma_cookie_init(&channel->chan);
		channel->nr = i;

		/* Set io base address for each channel */
		if (i < 8) {
			channel->ch_base = rza1dma->base + CHANNEL_0_7_OFFSET +
						EACH_CHANNEL_OFFSET * i;
			channel->ch_cmn_base = rza1dma->base +
						CHANNEL_0_7_COMMON_BASE;
		} else {
			channel->ch_base = rza1dma->base + CHANNEL_8_15_OFFSET	+
						EACH_CHANNEL_OFFSET * (i - 8);
			channel->ch_cmn_base = rza1dma->base +
						CHANNEL_8_15_COMMON_BASE;
		}
		/* Allocate descriptors */
		lmdesc = dma_alloc_coherent(NULL,
			sizeof(struct lmdesc) * DMAC_NR_LMDESC,
			&channel->lmdesc.base_dma, GFP_KERNEL);
		if (!lmdesc) {
			dev_err(&pdev->dev, "Can't alocate memory (lmdesc)\n");
			goto err;
		}
		lmdesc_setup(channel, lmdesc);

		/* Add the channel to the DMAC list */
		list_add_tail(&channel->chan.device_node,
			      &rza1dma->dma_device.channels);

		/* Initialize register for each channel */
		rza1dma_ch_writel(channel, CHCTRL_DEFAULT, CHCTRL, 1);
	}

	/* Register the DMAC as a DMA provider for DT. */
	if (of_dma_controller_register(pdev->dev.of_node, rza1dma_of_xlate,
				       NULL) < 0 )
		dev_err(&pdev->dev, "unable to register as provider provider for DT\n");

	/* Initialize register for all channels */
	rza1dma_writel(rza1dma, DCTRL_DEFAULT, CHANNEL_0_7_COMMON_BASE	+ DCTRL);
	rza1dma_writel(rza1dma, DCTRL_DEFAULT, CHANNEL_8_15_COMMON_BASE + DCTRL);

	rza1dma->dev = &pdev->dev;
	rza1dma->dma_device.dev = &pdev->dev;

	rza1dma->dma_device.device_alloc_chan_resources = rza1dma_alloc_chan_resources;
	rza1dma->dma_device.device_free_chan_resources = rza1dma_free_chan_resources;
	rza1dma->dma_device.device_tx_status = rza1dma_tx_status;
	rza1dma->dma_device.device_prep_slave_sg = rza1dma_prep_slave_sg;
	rza1dma->dma_device.device_prep_dma_memcpy = rza1dma_prep_dma_memcpy;
	rza1dma->dma_device.device_config = rza1dma_config;
	rza1dma->dma_device.device_terminate_all = rza1dma_terminate_all;
	rza1dma->dma_device.device_issue_pending = rza1dma_issue_pending;

	platform_set_drvdata(pdev, rza1dma);

	rza1dma->dma_device.copy_align = 0; /* Set copy_align to zero for net_dma_find_channel
					     * func to run well. But it might cause problems.
					     */
	rza1dma->dma_device.copy_align = 0;
	rza1dma->dma_device.dev->dma_parms = &rza1dma->dma_parms;
	dma_set_max_seg_size(rza1dma->dma_device.dev, 0xffffff);

	ret = dma_async_device_register(&rza1dma->dma_device);
	if (ret) {
		dev_err(&pdev->dev, "unable to register\n");
		goto err;
	}
	return 0;
err:
	return ret;
}

static int __exit rza1dma_remove(struct platform_device *pdev)
{
	struct rza1dma_engine *rza1dma = platform_get_drvdata(pdev);
	int i, channel_num = rza1dma->n_channels;

	of_dma_controller_free(pdev->dev.of_node);
	dma_async_device_unregister(&rza1dma->dma_device);

	/* free allocated resources */
	for (i = 0; i < channel_num; i++) {
		struct dmac_channel *channel = &rza1dma->channel[i];

		dma_free_coherent(NULL,
				sizeof(struct lmdesc) * DMAC_NR_LMDESC,
				channel->lmdesc.base,
				channel->lmdesc.base_dma);
	}

	return 0;
}

static struct of_device_id of_rza1dma_match[] = {
	{.compatible = "renesas,rza-dma"},
	{},
};
MODULE_DEVICE_TABLE(of, of_rza1dma_match);

static struct platform_driver rza1dma_driver = {
	.driver		= {
		.name	= "rza-dma",
		.of_match_table = of_rza1dma_match,
	},
	.probe 		= rza1dma_probe,
	.remove		= __exit_p(rza1dma_remove),
};

static int rza1dma_init(void)
{
	return platform_driver_register(&rza1dma_driver);
}
subsys_initcall(rza1dma_init);

MODULE_AUTHOR("Renesas Electronics");
MODULE_DESCRIPTION("Renesas RZA DMA Engine driver");
MODULE_LICENSE("GPL");
