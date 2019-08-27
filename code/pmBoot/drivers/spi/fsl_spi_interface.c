/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * Freescale Muti Serial Peripheral Interface (QSPI and DSPI) driver support
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <asm/errno.h>
#include "fsl_spi_interface.h"

struct fsl_spi_driver {
	void (*init)(void);
	int (*claim_bus)(struct spi_slave *slave);
	int (*release_bus)(struct spi_slave *slave);
	int (*cs_is_valid)(unsigned int bus, unsigned int cs);
	struct spi_slave * (*setup_slave)(unsigned int bus, unsigned int cs,
					unsigned int max_hz, unsigned int mode);
	void (*free_slave)(struct spi_slave *slave);
	void (*cs_activate)(struct spi_slave *slave);
	void (*cs_deactivate)(struct spi_slave *slave);
	int (*xfer)(struct spi_slave *slave, unsigned int bitlen,
		    const void *data_out, void *data_in, unsigned long flags);
};

static struct fsl_spi_driver fsl_spi_drivers[] = {
#ifdef CONFIG_FSL_DSPI
	{
		.init		= dspi_init,
		.claim_bus	= dspi_claim_bus,
		.cs_is_valid	= NULL,
		.setup_slave	= dspi_setup_slave,
		.free_slave	= dspi_free_slave,
		.cs_activate	= NULL,
		.cs_deactivate	= NULL,
		.xfer		= dspi_xfer,
	},
#endif
#ifdef CONFIG_FSL_QSPI
	{
		.init		= NULL,
		.claim_bus	= qspi_claim_bus,
		.cs_is_valid	= NULL,
		.setup_slave	= qspi_setup_slave,
		.free_slave	= qspi_free_slave,
		.cs_activate	= NULL,
		.cs_deactivate	= NULL,
		.xfer		= qspi_xfer,
	},
#endif
};

enum fsl_spi_driver_sel {
#ifdef CONFIG_FSL_DSPI
	DSPI_DRIVER,
#endif
#ifdef CONFIG_FSL_QSPI
	QSPI_DRIVER,
#endif
};

struct fsl_spi_driver *find_fsl_spi_driver(unsigned int bus, unsigned int cs)
{
	unsigned int driver_sel;

	if (board_spi_find_bus(bus, cs)) {
		board_print_spi_device();
		printf("can't find device on bus %d cs %d in board.\n",
		       bus, cs);
		return NULL;
	}

	switch (bus) {
#if defined(CONFIG_FSL_DSPI) && defined(SPI_BUS_FSL_DSPI1)
	case SPI_BUS_FSL_DSPI1:
		driver_sel = DSPI_DRIVER;
		break;
#endif
#if defined(CONFIG_FSL_DSPI) && defined(SPI_BUS_FSL_DSPI2)
	case SPI_BUS_FSL_DSPI2:
		driver_sel = DSPI_DRIVER;
		break;
#endif
#if defined(CONFIG_FSL_DSPI) && defined(SPI_BUS_FSL_DSPI3)
	case SPI_BUS_FSL_DSPI3:
		driver_sel = DSPI_DRIVER;
		break;
#endif
#if defined(CONFIG_FSL_DSPI) && defined(SPI_BUS_FSL_DSPI4)
	case SPI_BUS_FSL_DSPI4:
		driver_sel = DSPI_DRIVER;
		break;
#endif
#if defined(CONFIG_FSL_QSPI) && defined(SPI_BUS_FSL_QSPI)
	case SPI_BUS_FSL_QSPI:
		driver_sel = QSPI_DRIVER;
		break;
#endif
#if defined(CONFIG_FSL_QSPI) && defined(SPI_BUS_FSL_QSPI2)
	case SPI_BUS_FSL_QSPI2:
		driver_sel = QSPI_DRIVER;
		break;
#endif
	default:
		return NULL;
	}

	return &fsl_spi_drivers[driver_sel];
}

unsigned long get_spi_bus_base(unsigned int bus)
{
	switch (bus) {
#ifdef SPI_BUS_FSL_DSPI1
	case SPI_BUS_FSL_DSPI1:
		return DSPI1_BASE_ADDR;
#endif
#ifdef SPI_BUS_FSL_DSPI2
	case SPI_BUS_FSL_DSPI2:
		return DSPI2_BASE_ADDR;
#endif
#ifdef SPI_BUS_FSL_DSPI3
	case SPI_BUS_FSL_DSPI3:
		return DSPI3_BASE_ADDR;
#endif
#ifdef SPI_BUS_FSL_DSPI4
	case SPI_BUS_FSL_DSPI4:
		return DSPI4_BASE_ADDR;
#endif
#ifdef SPI_BUS_FSL_QSPI
	case SPI_BUS_FSL_QSPI:
		return QSPI_BASE_ADDR;
#endif
#ifdef SPI_BUS_FSL_QSPI2
	case SPI_BUS_FSL_QSPI2:
		return QSPI2_BASE_ADDR;
#endif
	default:
		printf("FSL SPL:get bus base error %d", bus);
		return 0;
	}
}

unsigned long get_qspi_amba_base(unsigned int bus)
{
	switch (bus) {
#ifdef SPI_BUS_FSL_QSPI
	case SPI_BUS_FSL_QSPI:
		return QSPI_AMBA_BASE;
#endif
#ifdef SPI_BUS_FSL_QSPI2
	case SPI_BUS_FSL_QSPI2:
		return QSPI2_AMBA_BASE;
#endif
	default:
		printf("FSL SPL:get qspi amba bus base error %d", bus);
		return 0;
	}

	return 0;
}

void spi_init(void)
{
	int i;
	struct fsl_spi_driver *driver;

	for (i = 0; i < ARRAY_SIZE(fsl_spi_drivers); i++) {
		driver = &fsl_spi_drivers[i];
		if (driver && driver->init)
			driver->init();
	}
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(bus, cs);

	if (!driver || !driver->setup_slave)
		return NULL;

	return driver->setup_slave(bus, cs, max_hz, mode);
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(slave->bus,
								slave->cs);

	if (!driver)
		return 1;
	if (!driver->claim_bus)
		return 0;

	return driver->claim_bus(slave);
}

void spi_free_slave(struct spi_slave *slave)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(slave->bus,
								slave->cs);

	if (driver && driver->free_slave)
		return driver->free_slave(slave);
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(slave->bus,
								slave->cs);

	if (!driver || !driver->xfer)
		return -1;

	return driver->xfer(slave, bitlen, dout, din, flags);
}

void spi_release_bus(struct spi_slave *slave)
{
}

void spi_cs_activate(struct spi_slave *slave)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(slave->bus,
								slave->cs);

	if (driver && driver->cs_activate)
		driver->cs_activate(slave);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	struct fsl_spi_driver *driver = find_fsl_spi_driver(slave->bus,
								slave->cs);

	if (driver && driver->cs_deactivate)
		driver->cs_deactivate(slave);
}
