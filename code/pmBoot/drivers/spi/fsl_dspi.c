/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004-2009, 2013 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 * Chao Fu (B44548@freescale.com)
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <fsl_dspi.h>
#include "fsl_spi_interface.h"

#if defined(CONFIG_VF610) || defined(CONFIG_LS102XA)
#include <asm/arch/clock.h>
#endif

#if !defined(CONFIG_VF610) && !defined(CONFIG_LS102XA)
#include <asm/immap.h>
#endif

#ifdef CONFIG_SYS_FSL_DSPI_LE
#define dspi_read32		in_le32
#define dspi_write32		out_le32
#elif defined(CONFIG_SYS_FSL_DSPI_BE)
#define dspi_read32		in_be32
#define dspi_write32		out_be32
#else
#error Neither CONFIG_SYS_FSL_DSPI_LE nor CONFIG_SYS_FSL_DSPI_BE is defined
#endif

struct dspi_slave {
	struct spi_slave slave;
	struct dspi *regs;
	uint baudrate;
	int charbit;
};

static inline struct dspi_slave *to_dspi_spi(struct spi_slave *slave)
{
	return container_of(slave, struct dspi_slave, slave);
}

#ifndef CONFIG_SPI_IDLE_VAL
#if defined(CONFIG_SPI_MMC)
#define CONFIG_SPI_IDLE_VAL	0xFFFF
#else
#define CONFIG_SPI_IDLE_VAL	0x0
#endif
#endif

#if defined(CONFIG_FSL_DSPI)
/* DSPI specific mode */
#define SPI_MODE_MOD	0x00200000
#define SPI_DBLRATE	0x00100000

static void dspi_tx(struct dspi_slave *dspislave, u32 ctrl, u16 data)
{
	while ((dspi_read32(&dspislave->regs->sr) & 0x0000F000) >= 4)
		;

	dspi_write32(&dspislave->regs->tfr, (ctrl | data));
}

static u16 dspi_rx(struct dspi_slave *dspislave)
{
	while ((dspi_read32(&dspislave->regs->sr) & 0x000000F0) == 0)
		;

	return (u16)(dspi_read32(&dspislave->regs->rfr) & 0xFFFF);
}

int dspi_xfer(struct spi_slave *slave, uint bitlen, const void *dout,
	       void *din, ulong flags)
{
	struct dspi_slave *dspislave = to_dspi_spi(slave);
	u16 *spi_rd16 = NULL, *spi_wr16 = NULL;
	u8 *spi_rd = NULL, *spi_wr = NULL;
	static u32 ctrl = 0;
	uint len = bitlen >> 3;

	if (dspislave->charbit == 16) {
		bitlen >>= 1;
		spi_wr16 = (u16 *) dout;
		spi_rd16 = (u16 *) din;
	} else {
		spi_wr = (u8 *) dout;
		spi_rd = (u8 *) din;
	}

	if ((flags & SPI_XFER_BEGIN) == SPI_XFER_BEGIN)
		ctrl |= DSPI_TFR_CONT;

	ctrl = (ctrl & 0xFF000000) | ((1 << slave->cs) << 16);

	if (len > 1) {
		int tmp_len = len - 1;
		while (tmp_len--) {
			if (dout != NULL) {
				if (dspislave->charbit == 16)
					dspi_tx(dspislave, ctrl, *spi_wr16++);
				else
					dspi_tx(dspislave, ctrl, *spi_wr++);
				dspi_rx(dspislave);
			}

			if (din != NULL) {
				dspi_tx(dspislave, ctrl, CONFIG_SPI_IDLE_VAL);
				if (dspislave->charbit == 16)
					*spi_rd16++ = dspi_rx(dspislave);
				else
					*spi_rd++ = dspi_rx(dspislave);
			}
		}

		len = 1;	/* remaining byte */
	}

	if ((flags & SPI_XFER_END) == SPI_XFER_END)
		ctrl &= ~DSPI_TFR_CONT;

	if (len) {
		if (dout != NULL) {
			if (dspislave->charbit == 16)
				dspi_tx(dspislave, ctrl, *spi_wr16);
			else
				dspi_tx(dspislave, ctrl, *spi_wr);
			dspi_rx(dspislave);
		}

		if (din != NULL) {
			dspi_tx(dspislave, ctrl, CONFIG_SPI_IDLE_VAL);
			if (dspislave->charbit == 16)
				*spi_rd16 = dspi_rx(dspislave);
			else
				*spi_rd = dspi_rx(dspislave);
		}
	} else {
		/* dummy read */
		dspi_tx(dspislave, ctrl, CONFIG_SPI_IDLE_VAL);
		dspi_rx(dspislave);
	}

	return 0;
}

struct spi_slave *dspi_setup_slave(unsigned int bus, unsigned int cs,
				unsigned int max_hz, unsigned int mode)
{
	/*
	 * bit definition for mode:
	 * bit 31 - 28: Transfer size 3 to 16 bits
	 *     27 - 26: PCS to SCK delay prescaler
	 *     25 - 24: After SCK delay prescaler
	 *     23 - 22: Delay after transfer prescaler
	 *     21     : Allow overwrite for bit 31-22 and bit 20-8
	 *     20     : Double baud rate
	 *     19 - 16: PCS to SCK delay scaler
	 *     15 - 12: After SCK delay scaler
	 *     11 -  8: Delay after transfer scaler
	 *      7 -  0: SPI_CPHA, SPI_CPOL, SPI_LSB_FIRST
	 */
	struct dspi_slave *dspislave;
	int prescaler[] = { 2, 3, 5, 7 };
	int scaler[] = {
		2, 4, 6, 8,
		16, 32, 64, 128,
		256, 512, 1024, 2048,
		4096, 8192, 16384, 32768
	};
	int i, j, pbrcnt, brcnt, diff, tmp, dbr = 0;
	int best_i, best_j, bestmatch = 0x7FFFFFFF, baud_speed, bus_clk;
	u32 bus_setup = 0;

	dspislave = spi_alloc_slave(struct dspi_slave, bus, cs);
	if (!dspislave)
		return NULL;

	dspislave->baudrate = max_hz;
	dspislave->regs = (struct dspi *)get_spi_bus_base(bus);

#if !defined(CONFIG_VF610) && !defined(CONFIG_LS102XA)
	cfspi_port_conf();	/* port configuration */
#endif

	dspi_write32(&dspislave->regs->mcr,
		     DSPI_MCR_MSTR | DSPI_MCR_CSIS7 | DSPI_MCR_CSIS6 |
		     DSPI_MCR_CSIS5 | DSPI_MCR_CSIS4 | DSPI_MCR_CSIS3 |
		     DSPI_MCR_CSIS2 | DSPI_MCR_CSIS1 | DSPI_MCR_CSIS0 |
		     DSPI_MCR_CRXF | DSPI_MCR_CTXF);
	/* Default setting in platform configuration */
#ifdef CONFIG_SYS_DSPI_CTAR0
	dspi_write32(&dspislave->regs->ctar[0], CONFIG_SYS_DSPI_CTAR0);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR1
	dspi_write32(&dspislave->regs->ctar[1], CONFIG_SYS_DSPI_CTAR1);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR2
	dspi_write32(&dspislave->regs->ctar[2], CONFIG_SYS_DSPI_CTAR2);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR3
	dspi_write32(&dspislave->regs->ctar[3], CONFIG_SYS_DSPI_CTAR3);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR4
	dspi_write32(&dspislave->regs->ctar[4], CONFIG_SYS_DSPI_CTAR4);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR5
	dspi_write32(&dspislave->regs->ctar[5], CONFIG_SYS_DSPI_CTAR5);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR6
	dspi_write32(&dspislave->regs->ctar[6], CONFIG_SYS_DSPI_CTAR6);
#endif
#ifdef CONFIG_SYS_DSPI_CTAR7
	dspi_write32(&dspislave->regs->ctar[7], CONFIG_SYS_DSPI_CTAR7);
#endif

	tmp = (prescaler[3] * scaler[15]);
#if defined(CONFIG_VF610) || defined(CONFIG_LS102XA)
	bus_clk = mxc_get_clock(MXC_DSPI_CLK);
#else
	bus_clk = get_dspi_clk();
#endif
	/* Maximum and minimum baudrate it can handle */
	if ((dspislave->baudrate > (bus_clk >> 1)) ||
	    (dspislave->baudrate < (bus_clk / tmp))) {
		printf("Exceed baudrate limitation: Max %d - Min %d\n",
		       (int)(bus_clk >> 1), (int)(bus_clk / tmp));
		return NULL;
	}

	/* Activate Double Baud when it exceed 1/4 the bus clk */
	if ((CONFIG_SYS_DSPI_CTAR0 & DSPI_CTAR_DBR) ||
	    (dspislave->baudrate > (bus_clk / (prescaler[0] * scaler[0])))) {
		bus_setup |= DSPI_CTAR_DBR;
		dbr = 1;
	}

	if (mode & SPI_CPOL)
		bus_setup |= DSPI_CTAR_CPOL;
	if (mode & SPI_CPHA)
		bus_setup |= DSPI_CTAR_CPHA;
	if (mode & SPI_LSB_FIRST)
		bus_setup |= DSPI_CTAR_LSBFE;

	/* Overwrite default value set in platform configuration file */
	if (mode & SPI_MODE_MOD) {

		if ((mode & 0xF0000000) == 0)
			bus_setup |=
			    dspi_read32(&dspislave->regs->
				ctar[dspislave->slave.cs])
			    & 0x78000000;
		else
			bus_setup |= ((mode & 0xF0000000) >> 1);

		/*
		 * Check to see if it is enabled by default in platform
		 * config, or manual setting passed by mode parameter
		 */
		if (mode & SPI_DBLRATE) {
			bus_setup |= DSPI_CTAR_DBR;
			dbr = 1;
		}
		bus_setup |= (mode & 0x0FC00000) >> 4;	/* PSCSCK, PASC, PDT */
		bus_setup |= (mode & 0x000FFF00) >> 4;	/* CSSCK, ASC, DT */
	} else
		bus_setup |=
			dspi_read32(&dspislave->regs->ctar[dspislave->slave.cs])
			& 0x78FCFFF0;

	dspislave->charbit =
	    ((dspi_read32(&dspislave->regs->ctar[dspislave->slave.cs])
	    & 0x78000000)
	    == 0x78000000) ? 16 : 8;

	pbrcnt = sizeof(prescaler) / sizeof(int);
	brcnt = sizeof(scaler) / sizeof(int);

	/* baudrate calculation - to closer value, may not be exact match */
	for (best_i = 0, best_j = 0, i = 0; i < pbrcnt; i++) {
		baud_speed = bus_clk / prescaler[i];
		for (j = 0; j < brcnt; j++) {
			tmp = (baud_speed / scaler[j]) * (1 + dbr);

			if (tmp > dspislave->baudrate)
				diff = tmp - dspislave->baudrate;
			else
				diff = dspislave->baudrate - tmp;

			if (diff < bestmatch) {
				bestmatch = diff;
				best_i = i;
				best_j = j;
			}
		}
	}

	bus_setup |= (DSPI_CTAR_PBR(best_i) | DSPI_CTAR_BR(best_j));
	dspi_write32(&dspislave->regs->ctar[dspislave->slave.cs], bus_setup);

	return &dspislave->slave;
}
#endif				/* CONFIG_FSL_DSPI */

void spi_init_f(void)
{
}

void spi_init_r(void)
{
}

void dspi_init(void)
{
}


void dspi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

int dspi_claim_bus(struct spi_slave *slave)
{
#if defined(CONFIG_VF610) || defined(CONFIG_LS102XA)
	return 0;
#else
	return cfspi_claim_bus(slave->bus, slave->cs);
#endif
}

void dspi_release_bus(struct spi_slave *slave)
{
#if !defined(CONFIG_VF610) && !defined(CONFIG_LS102XA)
	cfspi_release_bus(slave->bus, slave->cs);
#endif
}

