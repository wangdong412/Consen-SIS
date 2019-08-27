/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * Freescale Muti Serial Peripheral Interface (QSPI and DSPI) driver support
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

void dspi_init(void);
int dspi_claim_bus(struct spi_slave *slave);
void dspi_release_bus(struct spi_slave *slave);
int dspi_cs_is_valid(unsigned int bus, unsigned int cs);
struct spi_slave *dspi_setup_slave(unsigned int bus, unsigned int cs,
				unsigned int max_hz, unsigned int mode);
void dspi_free_slave(struct spi_slave *slave);
void dspi_cs_activate(struct spi_slave *slave);
void dspi_cs_deactivate(struct spi_slave *slave);
int dspi_xfer(struct spi_slave *slave, unsigned int bitlen,
	    const void *data_out, void *data_in, unsigned long flags);
void qspi_init(void);
int qspi_claim_bus(struct spi_slave *slave);
void qspi_release_bus(struct spi_slave *slave);
int qspi_cs_is_valid(unsigned int bus, unsigned int cs);
struct spi_slave *qspi_setup_slave(unsigned int bus, unsigned int cs,
				unsigned int max_hz, unsigned int mode);
void qspi_free_slave(struct spi_slave *slave);
void qspi_cs_activate(struct spi_slave *slave);
void qspi_cs_deactivate(struct spi_slave *slave);
int qspi_xfer(struct spi_slave *slave, unsigned int bitlen,
	    const void *data_out, void *data_in, unsigned long flags);

void board_print_spi_device(void);
int board_spi_find_bus(unsigned int bus, unsigned int cs);

unsigned long get_spi_bus_base(unsigned int bus);
unsigned long get_qspi_amba_base(unsigned int bus);
