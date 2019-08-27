/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal: PM BUS configure
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
 * 03jun15,jeff created
 *
 * DESCRIPTION
 * This file contains the board PMBUS component
 */

#pragma once

#include "../device/pcie/pcie.h"
#include <libc/cagboot.h>
#include <config.h>
#include <logh.h>

#define NULL 0

#define IPBUS_INPUT	 	    0x40000
#define IPBUS_INPUT_SIZE	0x8000 
#define IPBUS_OUTPUT	 	0x48000
#define IPBUS_OUTPUT_SIZE 	0x10000
#define IPBUS_ARG1		    0x50000
#define IPBUS_ARG1_SIZE		0x10000
#define IPBUS_ARG2		    0x60000
#define IPBUS_ARG2_SIZE		0x10000
#define IPBUS_SENDBOX		0x70000
#define IPBUS_SENDBOX_SIZE	0x200
#define IPBUS_RECVBOX		0x70200
#define IPBUS_RECVBOX_SIZE	0x200

enum IPBUS_TYPE {
    READ_IPBUS_INPUT,
    WRITE_IPBUS_OUTPUT,
    WRITE_IPBUS_ARG1,
    WRITE_IPBUS_ARG2,
    WRITE_SENDBOX,
    READ_RECVBOX,
};

INT16 ipbus_read(UINT8 opt, UINT8 *data, UINT32 offset, UINT32 len);
INT16 ipbus_write(UINT8 opt, UINT8 *data, UINT32 offset, UINT32 len);
