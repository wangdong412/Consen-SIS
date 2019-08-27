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
 * I2C driver header file.
 *
 */

#ifndef __IIC_H__
#define __IIC_H__

#include <p1020rdb.h>
#include <libc/cagboot.h>
#include <config.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
/* I2C */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_FSL
#define CONFIG_SYS_FSL_I2C_SPEED    400000
#define CONFIG_SYS_FSL_I2C_SLAVE    0x7F
#define CONFIG_SYS_FSL_I2C_OFFSET   0x3000
#define CONFIG_SYS_FSL_I2C2_SPEED   400000
#define CONFIG_SYS_FSL_I2C2_SLAVE   0x7F
#define CONFIG_SYS_FSL_I2C2_OFFSET  0x3100
#define CONFIG_SYS_I2C_NOPROBES     { {0, 0x29} }
#define CONFIG_SYS_I2C_EEPROM_ADDR  0x52
#define CONFIG_SYS_SPD_BUS_NUM      1 /* For rom_loc and flash bank */
#define CONFIG_SYS_I2C_RTC_ADDR     0x68
#define CONFIG_SYS_I2C_PCA9557_ADDR 0x18

#define ON 1
#define OFF 0


typedef struct fsl_i2c {
    UINT8 adr;     /* I2C slave address */
    UINT8 res0[3];
#define I2C_ADR     0xFE
#define I2C_ADR_SHIFT   1
#define I2C_ADR_RES ~(I2C_ADR)
    UINT8 fdr;     /* I2C frequency divider register */
    UINT8 res1[3];
#define IC2_FDR     0x3F
#define IC2_FDR_SHIFT   0
#define IC2_FDR_RES ~(IC2_FDR)
    UINT8 cr;      /* I2C control redister */
    UINT8 res2[3];
#define I2C_CR_MEN  0x80
#define I2C_CR_MIEN 0x40
#define I2C_CR_MSTA 0x20
#define I2C_CR_MTX  0x10
#define I2C_CR_TXAK 0x08
#define I2C_CR_RSTA 0x04
#define I2C_CR_BIT6 0x02    /* required for workaround A004447 */
#define I2C_CR_BCST 0x01
    UINT8 sr;      /* I2C status register */
    UINT8 res3[3];
#define I2C_SR_MCF  0x80
#define I2C_SR_MAAS 0x40
#define I2C_SR_MBB  0x20
#define I2C_SR_MAL  0x10
#define I2C_SR_BCSTM    0x08
#define I2C_SR_SRW  0x04
#define I2C_SR_MIF  0x02
#define I2C_SR_RXAK 0x01
    UINT8 dr;      /* I2C data register */
    UINT8 res4[3];
#define I2C_DR      0xFF
#define I2C_DR_SHIFT    0
#define I2C_DR_RES  ~(I2C_DR)
    UINT8 dfsrr;   /* I2C digital filter sampling rate register */
    UINT8 res5[3];
#define I2C_DFSRR   0x3F
#define I2C_DFSRR_SHIFT 0
#define I2C_DFSRR_RES   ~(I2C_DR)
    /* Fill out the reserved block */
    UINT8 res6[0xE8];
} fsl_i2c_t;


int rtc_write(UINT32 addrT, unsigned char *buf, UINT32 len);
int rtc_read(UINT32 addrF, unsigned char *buf, UINT32 len);
#endif
