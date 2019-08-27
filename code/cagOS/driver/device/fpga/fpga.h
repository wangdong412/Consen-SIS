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

#pragma once

typedef enum _freg {
    /*pmbus1*/
    SEND_ENABLE1 = 0x0,
    SEND_STATUS1 = 0x2,
    SEND_LEN1    = 0x4,
    DATA_RECV_STATUS1 = 0x6,
    DATA_RECV_LEN1    = 0x8,
    DATA_RECV_TIME1   = 0xa,
    STATE_RECV_STATUS1 = 0xc,
    STATE_RECV_LEN1    = 0xe,
    STATE_RECV_TIME1   = 0x10,
    
    /*pmbus2*/
    SEND_ENABLE2 = 0x20,
    SEND_STATUS2 = 0x22,
    SEND_LEN2    = 0x24,
    DATA_RECV_STATUS2 = 0x26,
    DATA_RECV_LEN2    = 0x28,
    DATA_RECV_TIME2   = 0x2a,
    STATE_RECV_STATUS2 = 0x2c,
    STATE_RECV_LEN2    = 0x2e,
    STATE_RECV_TIME2   = 0x30,

    /*cmbus*/
    SEND_ENABLE3    = 0x40,
    SEND_STATUS3    = 0x42,
    SEND_LEN3   =0x44,
    RECV_STATUS3    = 0x46,
    RECV_LEN3   = 0x48,
    RECV_TIME3   = 0x4a,
    TAR_STATION = 0x4c,

    /* other */
    SLOT_ADDR   = 0x60,
    KEY_SWITCH  = 0x62,
    STATE_MON   = 0x64,
    TEMP_MON    = 0x66,
    STATE_LED   = 0x68,
    KEY_LED     = 0x6a,
    COMM_LED    = 0x6c,
    STATION_ADDR = 0x6e,
    HB_STATUS   = 0x80,
    MODULE_OL   = 0x82,
    SW_OK       = 0x84,
    UP_EN       = 0x86,
    SYNC_TIME1  = 0xa0,
    SYNC_TIME2  = 0xa2,
    SYNC_TIME3  = 0xa4,
    SYNC_STATUS = 0xa6,
    SYNC_CALI1  = 0xa8,
    SYNC_CALI2  = 0xaa,
    SYNC_CALI3  = 0xac,
    SYNCCALI_EN = 0x1ae,
    SOE_TIME1   = 0xc0,
    SOE_TIME2   = 0xc2,
    SOE_TIME3   = 0xc4,
    SOE_TIME4   = 0xc6,
    SOE_CALI1   = 0xc8,
    SOE_CALI2   = 0xca,
    SOE_CALI3   = 0xcc,
    SOE_CALI4   = 0xce,
    SOECALI_EN  = 0xd0,
} freg_t;

unsigned short  fpga_read(unsigned int reg_num);
void fpga_write(unsigned int reg_num, unsigned short value);


