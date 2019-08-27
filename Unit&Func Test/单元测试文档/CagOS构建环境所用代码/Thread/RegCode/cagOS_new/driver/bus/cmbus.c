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

extern VOIDFUNCPTR _pSysL2CacheFlush;

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

/*
 * copy data to cm buffer
 * data : input data buffer
 * len : input data length
 * return : 0 - ok , <0 - error
 */
int16_t copy_to_cmbuf(uint8_t *data, int32_t len)
{
    int16_t res = 0;
    if(len <0 || len > CMBUS_SEND_BUF_SIZE)
        return -3;

    if(!(*SENDSTATUS3(_CAG_CONFIG_FPGA_ADDR) & 1)){
        pcie_write((uint8_t*)CMBUS_SEND_BUF,data,(uint32_t)len);
        *SENDLEN3(_CAG_CONFIG_FPGA_ADDR) = len;
    }
    else {
        res = -1;
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
    if (station <3 || station >15){ 
        res = -3;
    }

    *TATSTATION(_CAG_CONFIG_FPGA_ADDR) = station;
    *STARTSEND3(_CAG_CONFIG_FPGA_ADDR) = 0x1;

    return res;
}


void cmbus_hook(void)
{
 (*_pSysL2CacheFlush) ();
 cacheE500FlushD();
}

/*
 * check whether cmbus send is over
 * return : 0 - ok, <0 - sending
 *
 */

int16_t check_cm_send(void)
{
    int16_t res = -1;
    if( (*SENDSTATUS3(_CAG_CONFIG_FPGA_ADDR) & 0x1) == 0){
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

    if ((stat & 0x3) == 0){
        /* no recv */
        res = -1;
    }
    else if ((stat & 0x1) == 1 && (stat &0x4) == 0) {
        /* recv data */
        res = 1;
    }
    else if ((stat & 0x2) == 0x2 && (stat & 0x4) == 0){
        /* recv ack */
        res = 2;
    }
    else {
        res =-2;
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
    if(buf == NULL){
        return -3;
    }

    len = *RECVLEN3(_CAG_CONFIG_FPGA_ADDR);
    pcie_read(buf,(uint8_t*)CMBUS_RECV_BUF,(uint32_t)len);
    /*clear recv status*/
    *RECVSTATUS3(_CAG_CONFIG_FPGA_ADDR) = 0xffff; 

    return len;
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

/*************************************/

#if 0
/* test */
int16_t cmbus_send(int32_t station, int8_t *buf, int32_t len,int32_t timeout)
{
    int16_t res = -1;
    if(copy_to_cmbuf(buf,len)<0){
        return -1;
    }

    if ( enable_cm_send(station) <0){
        return -1;
    }
    while(1){
        res = check_cm_send();
        if (res >=0 || timeout-- == 0)
            break;
    }

    return res;
}
int16_t cmbus_recv(int8_t *buf, int32_t timeout)
{
    int16_t res = -1;
    while(1){
        res = check_cm_recv();
        if(res >=0 || timeout-- == 0)
            break;
    }
    if (res >0 ){
        res = copy_from_cmbuf(buf);
    }
    return res;
}
#endif
