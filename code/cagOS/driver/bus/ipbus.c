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

#include "ipbus.h"


#define ipbus_log(log)
/*
 * write data to send buf depend on bus_no
 * bus_no : 1 pmbus1, 2 pmbus2
 * data : input data buffer
 * len : data len ,write to controller
 * return : >=0 - copyied data length , <0 - error
 *
*/

INT16 ipbus_write(UINT8 opt, UINT8 *data, UINT32 offset, UINT32 len)
{
    if (data == NULL) {
        ipbus_log(IPBUS_PARA_ERR1);
        return -1;
    }

    int res = 0;
    UINT32 adrs = 0;
    UINT32 size = 0;
    switch (opt) {
    case WRITE_IPBUS_OUTPUT:
        adrs = IPBUS_OUTPUT;
        size = IPBUS_OUTPUT_SIZE;
        break;
    case WRITE_IPBUS_ARG1:
        adrs = IPBUS_ARG1;
        size = IPBUS_ARG1_SIZE;
        break;
    case WRITE_IPBUS_ARG2:
        adrs = IPBUS_ARG2;
        size = IPBUS_ARG2_SIZE;
        break;
    case WRITE_SENDBOX:
        adrs = IPBUS_SENDBOX;
        size = IPBUS_SENDBOX_SIZE;
        break;
    default:
        res = -2;
        ipbus_log(IPBUS_PARA_ERR2);
        return res;
    }

    if(len > (size - offset) || offset > size) {
        res = -1;
        ipbus_log(IPBUS_PARA_ERR3);
        return res;
    }

    pcie_write((UINT8 *)(adrs + offset), data, (UINT32)len);
    return len;
}

/*
 * get data from pmbus receive buffer
 * bus_no : 1 pmbus1, 2 pmbus2
 * buf : receive data buffer
 * no : data recv buf = 1 , state recv buf = 2
 *
*/

INT16 ipbus_read(UINT8 opt, UINT8 *data, UINT32 offset, UINT32 len)
{
    if (data == NULL) {
        ipbus_log(IPBUS_PARA_ERR4);
        return -1;
    }

    int res = 0;
    UINT32 adrs = 0;
    UINT32 size = 0;
    switch (opt) {
    case READ_IPBUS_INPUT:
        adrs = IPBUS_INPUT;
        size = IPBUS_INPUT_SIZE;
        break;
    case READ_RECVBOX:
        adrs = IPBUS_RECVBOX;
        size = IPBUS_RECVBOX_SIZE;
        break;
    default:
        res = -2;
        ipbus_log(IPBUS_PARA_ERR5);
        return res;
    }

    if(len > (size - offset) || offset > size) {
        res = -1;
        ipbus_log(IPBUS_PARA_ERR6);
        return res;
    }

    pcie_read(data, (UINT8*)(adrs + offset), (UINT32)len);
    return len;
}
