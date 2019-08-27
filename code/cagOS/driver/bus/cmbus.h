/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal: CM BUS configure
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
 * This file contains the board CMBUS component
 */

#pragma once


#include <libc/cagboot.h>
#include <config.h>
#include "../device/fpga/fpga.h"
#include "../device/pcie/pcie.h"
#include <logh.h>


#define NULL 0

#define CMBUS_SEND_BUF      0xc000
#define CMBUS_SEND_BUF_SIZE  0x2000
#define CMBUS_RECV_BUF      0xe000
#define CMBUS_RECV_BUF_SIZE  0x2000

#define PARA_ERR    -3
#define FUNC_ERR    -1
#define FUNC_OK     -1
#define DATA_ERR    -2


int16_t copy_to_cmbuf(uint8_t *data, int32_t len);
int16_t enable_cm_send(int station);
int16_t check_cm_send(void);

int16_t check_cm_recv(void);
int16_t copy_from_cmbuf(uint8_t *buf);

int16_t get_cmrecv_time(void);
