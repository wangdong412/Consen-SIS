#pragma once

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
 * 2015.10.21, Ezio created
 *
 * DECRIPTION
 * fpga spi flash driver source file. 
 *
 */

#include <logh.h>
#include <config.h>
#include <libc/cagboot.h>
#include <drv_config.h>
#include "../fpga/fpga.h"
#include "../pcie/pcie.h"

#include "../../fs/sffs/sffs.h"

#define SF_CMD_CS0      (0<<15)
#define SF_CMD_CS1      (1<<15)
#define SF_CMD_INV      0
#define SF_CMD_ID       1
#define SF_CMD_ER       2/*erase command*/
#define SF_CMD_WR       3/*write command*/
#define SF_CMD_RD       4/*read command*/

#define SF_STAT_FIN     (0<<15)
#define SF_STAT_IN      (1<<15)
#define SF_STAT_RESET   0  
#define SF_STAT_OK      1
#define SF_STAT_FAIL    /*except 0 && 1*/


#define SF_REG_CMD      (0x100)         /*command register*/
#define SF_REG_ADDRH    (0x104)         /*address high register*/
#define SF_REG_ADDRL    (0x102)         /*address low register*/
#define SF_REG_ADDR     //(SF_REG_ADDRH<<16 | SF_REG_ADDRL)
#define SF_REG_LEN      (0x106)         /*length register*/
#define SF_REG_STAT     (0x108)         /*status register*/    
#define SF_REG_TOH      (0x10c)         /*timeout high register*/
#define SF_REG_TOL      (0x10a)         /*timeout low register*/
#define SF_REG_TO       //(SF_REG_TOH<<16 | SF_REG_TOL)


#define SF_IS_ERR(addr)
#define SF_IS_OK(addr)

#define SF_BASE     0x0
#define SF_SIZE     0x00800000

INT32 spiflash_write(UINT32 addr , UINT8 *buf , UINT32 cnt);
INT32 spiflash_read(UINT32 addr , UINT8 *buf , UINT32 cnt);
INT32 spiflash_erase(INT32 no , INT32 num);
