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
 * 03jun15,ezio created
 *
 * DESCRIPTION
 * This file contains the board PMBUS component
 */

#pragma once

#include <libc/cagboot.h>
#include <config.h>
#define NULL 0

#define PMBUS1_SEND_BUF         0  
#define PMBUS1_SEND_BUF_SIZE    0x2000      
#define PMBUS1_DATA_BUF      0x2000
#define PMBUS1_DATA_BUF_SIZE      0x800
#define PMBUS1_STATE_BUF     0x4000
#define PMBUS1_STATE_BUF_SIZE     0x2000
#define PMBUS1_SPACE        0x6000

#define PMBUS2_SEND_BUF      0x6000  
#define PMBUS2_SEND_BUF_SIZE    0x2000      
#define PMBUS2_DATA_BUF      0x8000
#define PMBUS2_DATA_BUF_SIZE      0x800
#define PMBUS2_STATE_BUF     0xa000
#define PMBUS2_STATE_BUF_SIZE     0x2000
#define PMBUS2_SPACE        0x6000

#define PMBUS_BUF_SPACE     0x2000

#define PARA_ERR    -3
#define FUNC_ERR    -1
#define FUNC_OK     0
#define DATA_ERR    -2

int8_t set_pm_slot(void);
int8_t get_pm_slot(void);
int16_t copy_to_pmbuf(int8_t bus_no, uint8_t *data, int32_t len);
int16_t copy_from_pmbuf(int8_t bus_no, int8_t buf_no, uint8_t *buf);
int16_t get_pmrecv_time(int8_t bus_no, int8_t no);
int16_t enable_pm_send(int bus_no,int buf_no);
int16_t check_pm_send(int32_t bus_no);
int16_t check_pm_recv(int32_t bus_no,int32_t buf_no);
#if 0
int16_t pmbus_send(int32_t bus_no, int32_t buf_no, int8_t *buf, int32_t len,int32_t timeout);
int16_t pmbus_recv(int32_t bus_no, int32_t buf_no, int8_t *buf, int32_t timeout);
#endif
