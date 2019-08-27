/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal: FPGA configure
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
 * 03jun15,ezio created
 *
 * DESCRIPTION
 * This file contains the board FPGA component
 */

#include "fpga.h"
#include <libc/cagboot.h>
#include <config.h>


uint16_t fpga_read(uint16_t reg_num)
{
    return *(uint16_t *)(_CAG_CONFIG_FPGA_ADDR+reg_num);
}


void fpga_write(uint16_t reg_num,uint16_t value)
{
    *(uint16_t *)(_CAG_CONFIG_FPGA_ADDR+reg_num) = value;
}

