/*
 *------------------------------------------------------------
 * Project: cagOS
 * Goal:
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 2015.10.26, Ezio created
 *
 * DECRIPTION
 * I2C driver source file. with a simple driver to r/w rtc.
 *
 */


#include <stdarg.h>
#include <ppc/io.h>
#include "iic.h"		/* Functional interface */

/* The maximum number of microseconds we will wait until another master has
 * released the bus.  If not defined in the board header file, then use a
 * generic value.
 */
#ifndef CONFIG_I2C_MBB_TIMEOUT
#define CONFIG_I2C_MBB_TIMEOUT	100000
#endif

#define readbi(addr)   in_8(addr)
#define writebi(b,addr)  out_8(addr,b)

#define __BIG_ENDIAN//ezio

extern UINT32 get_tbclk (void);
extern unsigned long long get_ticks(void);
extern void sysUsDelay(int delay);
int i2c_debug = OFF;

void debug(char *fmt,...)
{
    if(i2c_debug == ON) {
        va_list args;
        va_start(args,fmt);
        printf(fmt);
    }
}

/* The maximum number of microseconds we will wait for a read or write
 * operation to complete.  If not defined in the board header file, then use a
 * generic value.
 */
#ifndef CONFIG_I2C_TIMEOUT
#define CONFIG_I2C_TIMEOUT	100000
#endif

#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0

static const struct fsl_i2c *i2c_dev[2] = {
    (struct fsl_i2c *)(I2C_BUS1_BASE),
    (struct fsl_i2c *)(I2C_BUS2_BASE)
};
static int i2c_clks[2] = {FREQ_400_MHZ/2-2,FREQ_400_MHZ/2-2};

/*just compare unsigned int*/
#define min(X, Y)               \
        ({ UINT32 __x = (X);         \
            UINT32  __y = (Y);        \
        (__x < __y) ? __x : __y; })

#define max(X, Y)               \
        ({ UINT32  __x = (X);         \
         UINT32  __y = (Y);        \
         (__x > __y) ? __x : __y; })
/*
 * This function is intended for SHORT delays only.
 * It will overflow at around 10 seconds @ 400MHz,
 * or 20 seconds @ 200MHz.
 * and 5 seconds @ 800MHZ
 */
unsigned long usec2ticks(unsigned long usec)
{
    UINT32 ticks;

    if (usec < 1000) {
        ticks = ((usec * (get_tbclk()/1000)) + 500) / 1000;
    } else {
        ticks = ((usec / 10) * (get_tbclk() / 100000));
    }

    return (ticks);
}
/**
 * Set the I2C bus speed for a given I2C device
 *
 * @param dev: the I2C device
 * @i2c_clk: I2C bus clock frequency
 * @speed: the desired speed of the bus
 *
 * The I2C device must be stopped before calling this function.
 *
 * The return value is the actual bus speed that is set.
 */
static unsigned int set_i2c_bus_speed(struct fsl_i2c *dev, unsigned int i2c_clk, unsigned int speed)
{
    unsigned short divider = min(i2c_clk / speed, (unsigned short) -1);

    /*
     * We want to choose an FDR/DFSR that generates an I2C bus speed that
     * is equal to or lower than the requested speed.  That means that we
     * want the first divider that is equal to or greater than the
     * calculated divider.
     */
    volatile UINT8 dfsr, fdr = 0x31; /* Default if no FDR found */
    /* a, b and dfsr matches identifiers A,B and C respectively in AN2919 */
    unsigned short a, b, ga, gb;
    unsigned long c_div, est_div;

    /* Condition 1: dfsr <= 50/T */
    dfsr = (5 * (i2c_clk / 1000)) / 100000;
    debug("Requested speed:%d, i2c_clk:%d\n", speed, i2c_clk);
    if (!dfsr)
        dfsr = 1;

    est_div = ~0;
    for (ga = 0x4, a = 10; a <= 30; ga++, a += 2) {
        for (gb = 0; gb < 8; gb++) {
            b = 16 << gb;
            c_div = b * (a + ((3*dfsr)/b)*2);
            if ((c_div > divider) && (c_div < est_div)) {
                unsigned short bin_gb, bin_ga;

                est_div = c_div;
                bin_gb = gb << 2;
                bin_ga = (ga & 0x3) | ((ga & 0x4) << 3);
                fdr = bin_gb | bin_ga;
                speed = i2c_clk / est_div;
                debug("FDR:0x%.2x, div:%ld, ga:0x%x, gb:0x%x,a:%d, b:%d, speed:%d\n",
                      fdr, est_div, ga, gb, a, b, speed);
                /* Condition 2 not accounted for */
                debug("Tr <= %d ns\n",
                      (b - 3 * dfsr) * 1000000 /
                      (i2c_clk / 1000));
            }
        }
        if (a == 20)
            a += 2;
        if (a == 24)
            a += 4;
    }
    debug("divider:%d, est_div:%ld, DFSR:%d\n", divider, est_div, dfsr);
    debug("FDR:0x%.2x, speed:%d\n", fdr, speed);
    writebi(dfsr, &dev->dfsrr);	/* set default filter */
    writebi(fdr, &dev->fdr);		/* set bus speed */
    return speed;
}


static int fsl_i2c_fixup(struct fsl_i2c *dev)
{
    const unsigned long long timeout = usec2ticks(CONFIG_I2C_MBB_TIMEOUT);
    unsigned long long timeval = 0;
    int ret = -1;
    unsigned int flags = 0;


    writebi(I2C_CR_MEN | I2C_CR_MSTA, &dev->cr);

    timeval = get_ticks();
    while (!(readbi(&dev->sr) & I2C_SR_MBB)) {
        if ((get_ticks() - timeval) > timeout)
            goto err;
    }

    if (readbi(&dev->sr) & I2C_SR_MAL) {
        /* SDA is stuck low */
        writebi(0, &dev->cr);
        sysUsDelay(100);
        writebi(I2C_CR_MSTA | flags, &dev->cr);
        writebi(I2C_CR_MEN | I2C_CR_MSTA | flags, &dev->cr);
    }

    readbi(&dev->dr);

    timeval = get_ticks();
    while (!(readbi(&dev->sr) & I2C_SR_MIF)) {
        if ((get_ticks() - timeval) > timeout)
            goto err;
    }
    ret = 0;

err:
    writebi(I2C_CR_MEN | flags, &dev->cr);
    writebi(0, &dev->sr);
    sysUsDelay(100);

    return ret;
}

/*******************************************************************************
*
* fsl_i2c_init - init i2c controller
*
* id, i2c controller id
* speed, i2c running speed
* slaveadd, i2c slave address
*
* RETURNS: actual write data length.or return -1 if the operation is failed.
*
*/
void fsl_i2c_init(int id, int speed, int slaveadd)
{
    struct fsl_i2c *dev;
    const unsigned long long timeout = usec2ticks(CONFIG_I2C_MBB_TIMEOUT);
    unsigned long long timeval;

    dev = (struct fsl_i2c *)i2c_dev[id];

    writebi(0, &dev->cr);		/* stop I2C controller */
    sysUsDelay(5);			/* let it shutdown in peace */
    set_i2c_bus_speed(dev, i2c_clks[id] , speed);
    writebi(slaveadd << 1, &dev->adr);/* write slave address */
    writebi(0x0, &dev->sr);		/* clear status register */
    writebi(I2C_CR_MEN, &dev->cr);	/* start I2C controller */

    timeval = get_ticks();
    while (readbi(&dev->sr) & I2C_SR_MBB) {
        if ((get_ticks() - timeval) < timeout) {
            //printf("%x\n",get_ticks());
            continue;
        }

        if (fsl_i2c_fixup(dev))
            debug("i2c_init: BUS#%d failed to init\n",id);

        break;
    }
}

static int i2c_wait4bus(int id)
{
    struct fsl_i2c *dev = (struct fsl_i2c *)i2c_dev[id];
    unsigned long long timeval =get_ticks();
    const unsigned long long timeout = usec2ticks(CONFIG_I2C_MBB_TIMEOUT);
    //printf("mbb %d\n",CONFIG_I2C_MBB_TIMEOUT);

    while (readbi(&dev->sr) & I2C_SR_MBB) {
        if ((get_ticks() - timeval) > timeout) {
            //printf("i4e\n");
            return -1;
        }
    }

    return 0;
}

static __inline__ int i2c_wait(int id, int write)
{
    UINT32 csr;
    unsigned long long timeval = get_ticks();
    const unsigned long long timeout = usec2ticks(CONFIG_I2C_TIMEOUT);
    struct fsl_i2c *dev = (struct fsl_i2c *)i2c_dev[id];

    do {
        csr = readbi(&dev->sr);
        if (!(csr & I2C_SR_MIF))
            continue;
        /* Read again to allow register to stabilise */
        csr = readbi(&dev->sr);

        writebi(0x0, &dev->sr);

        if (csr & I2C_SR_MAL) {
            debug("i2c_wait: MAL\n");
            return -1;
        }

        if (!(csr & I2C_SR_MCF))	{
            debug("i2c_wait: unfinished\n");
            return -1;
        }

        if (write == I2C_WRITE_BIT && (csr & I2C_SR_RXAK)) {
            debug("i2c_wait: No RXACK\n");
            return -1;
        }

        return 0;
    } while ((get_ticks() - timeval) < timeout);

    debug("i2c_wait: timed out\n");
    return -1;
}

static __inline__ int i2c_write_addr(int id, UINT8 dev, UINT8 dir, int rsta)
{
    struct fsl_i2c *device = (struct fsl_i2c *)i2c_dev[id];

    writebi(I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_MTX
           | (rsta ? I2C_CR_RSTA : 0),
           &device->cr);

    writebi((dev << 1) | dir, &device->dr);

    if (i2c_wait(id, I2C_WRITE_BIT) < 0) {
        return 0;
    }

    return 1;
}

static __inline__ int __i2c_write(int id, UINT8 *data, int length)
{
    struct fsl_i2c *dev = (struct fsl_i2c *)i2c_dev[id];
    int i;

    for (i = 0; i < length; i++) {
        writebi(data[i], &dev->dr);

        if (i2c_wait(id, I2C_WRITE_BIT) < 0)
            break;
    }

    return i;
}

static __inline__ int __i2c_read(int id, UINT8 *data, int length)
{
    struct fsl_i2c *dev = (struct fsl_i2c *)i2c_dev[id];
    int i;

    writebi(I2C_CR_MEN | I2C_CR_MSTA | ((length == 1) ? I2C_CR_TXAK : 0),&dev->cr);

    /* dummy read */
    readbi(&dev->dr);

    for (i = 0; i < length; i++) {
        if (i2c_wait(id, I2C_READ_BIT) < 0)
            break;

        /* Generate ack on last next to last byte */
        if (i == length - 2)
            writebi(I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_TXAK,
                   &dev->cr);

        /* Do not generate stop on last byte */
        if (i == length - 1)
            writebi(I2C_CR_MEN | I2C_CR_MSTA | I2C_CR_MTX,
                   &dev->cr);

        data[i] = readbi(&dev->dr);
    }

    return i;
}

/*******************************************************************************
*
* fsl_i2c_read - read data via i2c
*
* id, i2c controller id
* dev, i2c device id
* addr, address in device
* alen = 1
*
* RETURNS: actual read data length.or return -1 if the operation is failed.
*
*/
int fsl_i2c_read(int id, UINT8 dev, UINT32 addr, int alen, UINT8 *data, int length)
{
    struct fsl_i2c *device = (struct fsl_i2c *)i2c_dev[id];
    int i = -1; /* signal error */
    UINT8 *a = (UINT8*)&addr;
    int len = alen * -1;

    if (i2c_wait4bus(id) < 0)
        return -1;

    /* To handle the need of I2C devices that require to write few bytes
     * (more than 4 bytes of address as in the case of else part)
     * of data before reading, Negative equivalent of length(bytes to write)
     * is passed, but used the +ve part of len for writing data
     */
    if (alen < 0) {
        /* Generate a START and send the Address and
         * the Tx Bytes to the slave.
         * "START: Address: Write bytes data[len]"
         * IF part supports writing any number of bytes in contrast
         * to the else part, which supports writing address offset
         * of upto 4 bytes only.
         * bytes that need to be written are passed in
         * "data", which will eventually keep the data READ,
         * after writing the len bytes out of it
         */
        if (i2c_write_addr(id, dev, I2C_WRITE_BIT, 0) != 0)
            i = __i2c_write(id, data, len);

        if (i != len)
            return -1;

        if (length && i2c_write_addr(id, dev, I2C_READ_BIT, 1) != 0)
            i = __i2c_read(id, data, length);
    } else {
        if ((!length || alen > 0) &&
            i2c_write_addr(id, dev, I2C_WRITE_BIT, 0) != 0  &&
            __i2c_write(id, &a[4 - alen], alen) == alen) {
            i = 0; /* No error so far */
        }

        if (length &&
            i2c_write_addr(id, dev, I2C_READ_BIT, alen ? 1 : 0) != 0) {
            i = __i2c_read(id, data, length);
        }
    }

    writebi(I2C_CR_MEN, &device->cr);

    if (i2c_wait4bus(id)) /* Wait until STOP */
        debug("i2c_read: wait4bus timed out\n");

    if (i == length)
        return 0;
    return -1;
}

/*******************************************************************************
*
* fsl_i2c_write - write data via i2c
*
* id, i2c controller id
* dev, i2c device id
* addr, address in device
* alen = 1
*
* RETURNS: actual write data length.or return -1 if the operation is failed.
*
*/
int fsl_i2c_write(int id, UINT8 dev, UINT32 addr, int alen, UINT8 *data, int length)
{
    struct fsl_i2c *device = (struct fsl_i2c *)i2c_dev[id];
    int i = -1; /* signal error */
    UINT8 *a = (UINT8*)&addr;

    if (i2c_wait4bus(id) < 0)
        return -1;

    if (i2c_write_addr(id, dev, I2C_WRITE_BIT, 0) != 0 &&
        __i2c_write(id, &a[4 - alen], alen) == alen) {
        i = __i2c_write(id, data, length);
    }

    writebi(I2C_CR_MEN, &device->cr);
    if (i2c_wait4bus(id)) /* Wait until STOP */
        debug("i2c_write: wait4bus timed out\n");

    if (i == length)
        return 0;

    return -1;
}


/*rtc*/
int rtc_read(UINT32 addrF, unsigned char *buf, UINT32 len)
{
    return fsl_i2c_read(0,CONFIG_SYS_I2C_RTC_ADDR, addrF,1,buf,len);
}

int rtc_write(UINT32 addrT, unsigned char *buf, UINT32 len)
{
    return fsl_i2c_write(0,CONFIG_SYS_I2C_RTC_ADDR,addrT,1,buf,len);
}
