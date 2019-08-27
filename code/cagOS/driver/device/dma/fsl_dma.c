/*
 * Copyright 2004,2007,2008 Freescale Semiconductor, Inc.
 * (C) Copyright 2002, 2003 Motorola Inc.
 * Xianghua Xiao (X.Xiao@motorola.com)
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#include <ppc/io.h>
#include "fsl_dma.h"

/* Controller can only transfer 2^26 - 1 bytes at a time */
#define FSL_DMA_MAX_SIZE	(0x3ffffff)

#define FSL_DMA_MR_DEFAULT (FSL_DMA_MR_BWC_DIS | FSL_DMA_MR_CTM_DIRECT)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))


ccsr_dma_t *dma_base = (void *)(DMA1_REGBASE);

static void dma_sync(void)
{
	__asm__("sync; isync; msync");
}

static void out_dma32(volatile unsigned *addr, int val)
{
	out_be32(addr, val);
}

static uint32_t in_dma32(volatile unsigned *addr)
{
	return in_be32(addr);
}

static uint32_t dma_check(void) 
{
	volatile fsl_dma_t *dma = &dma_base->dma[0];
	uint32_t status;

	/* While the channel is busy, spin */
	do {
		status = in_dma32(&dma->sr);
	} while (status & FSL_DMA_SR_CB);

	/* clear MR[CS] channel start bit */
	out_dma32(&dma->mr, in_dma32(&dma->mr) & ~FSL_DMA_MR_CS);
	dma_sync();

	if (status != 0)
		printf ("DMA Error: status = %x\n", status);

	return status;
}

void dma_init(void) 
{
	volatile fsl_dma_t *dma = &dma_base->dma[0];

	out_dma32(&dma->satr, FSL_DMA_SATR_SREAD_SNOOP);
	out_dma32(&dma->datr, FSL_DMA_DATR_DWRITE_SNOOP);
	out_dma32(&dma->sr, 0xffffffff); /* clear any errors */
	dma_sync();
}

int dma_memcpy(phys_addr_t dest, phys_addr_t src, phys_size_t count) 
{
	volatile fsl_dma_t *dma = &dma_base->dma[0];
	uint32_t xfer_size;

	while (count) {
		xfer_size = MIN(FSL_DMA_MAX_SIZE, count);

		out_dma32(&dma->dar, (uint32_t) (dest & 0xFFFFFFFF));
		out_dma32(&dma->sar, (uint32_t) (src & 0xFFFFFFFF));
		out_dma32(&dma->bcr, xfer_size);
		dma_sync();

		/* Prepare mode register */
		out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT);
		dma_sync();

		/* Start the transfer */
		out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT | FSL_DMA_MR_CS);

		count -= xfer_size;
		src += xfer_size;
		dest += xfer_size;

		dma_sync();

		if (dma_check())
			return -1;
	}

	return 0;
}
