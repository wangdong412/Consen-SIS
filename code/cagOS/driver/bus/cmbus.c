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


#include <libc/cagboot.h>
#include "cmbus.h"

/*cmbus*/

#define STARTSEND3(base)   ((VUINT16 *)(base + SEND_ENABLE3))
#define SENDSTATUS3(base)  ((VUINT16 *)(base + SEND_STATUS3))
#define SENDLEN3(base)     ((VUINT16 *)(base + SEND_LEN3))

#define RECVSTATUS3(base)   ((VUINT16 *)(base + RECV_STATUS3))
#define RECVLEN3(base)      ((VUINT16 *)(base + RECV_LEN3))
#define RECVTIME3(base)     ((VUINT16 *)(base + RECV_TIME3))
#define TATSTATION(base)    ((VUINT16 *)(base + TAR_STATION))

#if 0
static volatile VUINT16 *start_send3   = (VUINT16 *)(_CAG_CONFIG_FPGA_ADDR + SEND_ENABLE3);
#endif

#define cmbus_log(log)
/*
 * copy data to cm buffer
 * data : input data buffer
 * len : input data length
 * return : 0 - ok , <0 - error
 */
int16_t copy_to_cmbuf(uint8_t *data, int32_t len)
{
    int16_t res = 0;
    if(len <0 || len > CMBUS_SEND_BUF_SIZE) {
        cmbus_log(CMBUS_PARA_ERR1);
        return -3;
    }

    if(!(*SENDSTATUS3(_CAG_CONFIG_FPGA_ADDR) & 1)) {
        pcie_write((uint8_t*)CMBUS_SEND_BUF,data,(uint32_t)len);
        *SENDLEN3(_CAG_CONFIG_FPGA_ADDR) = len;
    } else {
        res = -1;
        cmbus_log(CMBUS_SENDNREADY_ERR);
    }
    return res;
}

/*
 * enable cmbus send
 * station : target cmboard station number
 * return : 0 - ok , <0 - error
 *
 */
int16_t enable_cm_send(int station)
{
    int16_t res = 0;
    if (station <3 || station >15) {
        res = -3;
        cmbus_log(CMBUS_PARA_ERR2);
        return res;
    }

    *TATSTATION(_CAG_CONFIG_FPGA_ADDR) = station;
    *STARTSEND3(_CAG_CONFIG_FPGA_ADDR) = 0x1;

    return res;
}

/*
 * check whether cmbus send is over
 * return : 0 - ok, <0 - sending
 *
 */

int16_t check_cm_send(void)
{
    int16_t res = -1;
    if( (*SENDSTATUS3(_CAG_CONFIG_FPGA_ADDR) & 0x1) == 0) {
        res = 0;
    }
    return res;
}

/*
 * check whether receive data or ack
 * return : 1 - recv data , 2 - recv ack , <0 - no recv
 *
 */
int16_t check_cm_recv(void)
{
    int16_t res = -1;
    VUINT16 stat = *RECVSTATUS3(_CAG_CONFIG_FPGA_ADDR);

    if ((stat & 0x3) == 0) {
        /* no recv */
        res = -1;
    } else if ((stat & 0x1) == 1 && (stat &0x4) == 0) {
        /* recv data */
        res = 1;
    } else if ((stat & 0x2) == 0x2 && (stat & 0x4) == 0) {
        /* recv ack */
        res = 2;
    } else {
        cmbus_log(CMBUS_RECVFAIL_ERR);
        res =-2;
        /*clear err recv status*/
        *RECVSTATUS3(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    }

    return res;
}

/*
 * get data from cmbus receive buffer if we received data
 * buf : receive data buffer
 * return : >0 - data length , <0 - error
 *
 */
int16_t copy_from_cmbuf(uint8_t *buf)
{
    int16_t len = -1;
    if(buf == NULL) {
        cmbus_log(CMBUS_PARA_ERR3);
        return -3;
    }

    len = *RECVLEN3(_CAG_CONFIG_FPGA_ADDR);
    pcie_read(buf,(uint8_t*)CMBUS_RECV_BUF,(uint32_t)len);
    /*clear recv status*/
    *RECVSTATUS3(_CAG_CONFIG_FPGA_ADDR) = 0xffff;

    return len;
}

void clear_crecv_status(void)
{
    /*clear recv status*/
    *RECVSTATUS3(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
}

/*
 * get data received timestamp
 * return : timestamp
 *
 */
int16_t get_cmrecv_time(void)
{
    return *RECVTIME3(_CAG_CONFIG_FPGA_ADDR);
}
