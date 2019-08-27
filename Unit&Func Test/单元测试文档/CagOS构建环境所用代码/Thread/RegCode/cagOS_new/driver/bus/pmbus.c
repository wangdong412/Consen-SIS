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


#include "pmbus.h"

//pmbus1
#define  STARTSEND1(base)     ((VUINT16 *)(base + 0x0))
#define  SENDSTATUS1(base)    ((VUINT16 *)(base + 0x2))
#define  SENDLEN1(base)       ((VUINT16 *)(base + 0x4))
#define  DATASTATUS1(base)    ((VUINT16 *)(base + 0x6))
#define  DATALEN1(base)       ((VUINT16 *)(base + 0x8))
#define  DATATIME1(base)      ((VUINT16 *)(base + 0xa))
#define  STATESTATUS1(base)   ((VUINT16 *)(base + 0xc))
#define  STATELEN1(base)      ((VUINT16 *)(base + 0xe))
#define  STATETIME1(base)     ((VUINT16 *)(base + 0x10))

//pmbus2
#define  STARTSEND2(base)     ((VUINT16 *)(base + 0x20))
#define  SENDSTATUS2(base)    ((VUINT16 *)(base + 0x22))
#define  SENDLEN2(base)       ((VUINT16 *)(base + 0x24))
#define  DATASTATUS2(base)    ((VUINT16 *)(base + 0x26))
#define  DATALEN2(base)       ((VUINT16 *)(base + 0x28))
#define  DATATIME2(base)      ((VUINT16 *)(base + 0x2a))
#define  STATESTATUS2(base)   ((VUINT16 *)(base + 0x2c))
#define  STATELEN2(base)      ((VUINT16 *)(base + 0x2e))
#define  STATETIME2(base)     ((VUINT16 *)(base + 0x30))

//station
#define  SLOT(base)    	      ((VUINT16 *)(base + 0x60))
#define  STATION(base)        ((VUINT16 *)(base + 0x6e))

/*
 * check slot ok
 *
 * return : -1,     A slot not equals to B slot
 *          0,1,2,  actual slot value
 *
 */
int8_t set_pm_slot(void)
{
    int8_t s1 ,s2;
    int8_t ret = 0;
    s1 = *SLOT(_CAG_CONFIG_FPGA_ADDR) & 0xff;
    s2 = (*SLOT(_CAG_CONFIG_FPGA_ADDR) & 0xff00)>>8;
    if (s1 != s2) {
        ret = -1;
    }
    else {
        s2 = s1 & 0x3;
        *STATION(_CAG_CONFIG_FPGA_ADDR) = s2;
    }
    return ret;
}

/*
 * read slot value
 * return : >=0 - slot number , <0 - slot error
 *
 */
int8_t get_pm_slot(void)
{
    uint8_t s1,s2;
    int8_t res = 0;

    s1 = *SLOT(_CAG_CONFIG_FPGA_ADDR) & 0x3;
    s2 = ((*SLOT(_CAG_CONFIG_FPGA_ADDR) & 0xff00)>>8) &0x2;
    if (s1 !=s2){
        res = -1;
    }
    else {
        res = s1;
    }
    
    return res;
}

/*
 * write data to send buf depend on bus_no
 * bus_no : 1 pmbus1, 2 pmbus2
 * data : input data buffer
 * len : data len ,write to controller 
 * return : >=0 - copyied data length , <0 - error
 *
 */
int16_t copy_to_pmbuf(int8_t bus_no, uint8_t *data, int32_t len)
{
    int16_t res = 0;
    if(len <0 || len > PMBUS_BUF_SPACE)
        return -3;
    if(bus_no == 1 && !(*SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR) & 1)){
        pcie_write(PMBUS1_SEND_BUF,data,(uint32_t)len);
        *SENDLEN1(_CAG_CONFIG_FPGA_ADDR) = len;
    }
    else if(bus_no == 2 && !(*SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR) & 1)){
        pcie_write((uint8_t *)PMBUS2_SEND_BUF,data,(uint32_t)len);
        *SENDLEN2(_CAG_CONFIG_FPGA_ADDR) = len;
    }
    else {
        res = -1;
    }
    return res;
}


/*
 * get data from pmbus receive buffer
 * bus_no : 1 pmbus1, 2 pmbus2
 * buf : receive data buffer
 * no : data recv buf = 1 , state recv buf = 2
 *
 */
int16_t copy_from_pmbuf(int8_t bus_no, int8_t no, uint8_t *buf)
{
    int16_t len = -1;

    if(buf == NULL){
        return -3;
    }

    if(bus_no == 1){
        /*get received data/state len*/
        if (no == 1 ){
                len = *DATALEN1(_CAG_CONFIG_FPGA_ADDR);
                pcie_read(buf,(uint8_t *)(no*PMBUS_BUF_SPACE) ,(uint32_t)len);
                /*clear recv status*/
                *DATASTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff; 
        }
        else if (no == 2 ){
                len = *STATELEN1(_CAG_CONFIG_FPGA_ADDR);
                pcie_read(buf,(uint8_t*)(no*PMBUS_BUF_SPACE) ,(uint32_t)len);
                /*clear recv status*/
                *STATESTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
        }
        else{
            len = -3;
        }

    }
    else if(bus_no == 2) {
        if (no == 1 ){
                len = *DATALEN2(_CAG_CONFIG_FPGA_ADDR);
                pcie_read(buf,(uint8_t*)(no*PMBUS_BUF_SPACE + PMBUS1_SPACE),(uint32_t)len);
                /*clear recv status*/
                *DATASTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
        }
        else if (no == 2){
                len = *STATELEN2(_CAG_CONFIG_FPGA_ADDR);
                pcie_read(buf,(uint8_t*)(no*PMBUS_BUF_SPACE + PMBUS1_SPACE),(uint32_t)len);
                /*clear recv status*/
                *STATESTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
        }
        else{
            len = -3;
        }

    }
    else{
        len = -3;
    } 

    return len;
}


/*
 * get data received timestamp
 * buf_no : 1 - data buffer , 2 - state buffer
 * bus_no : 1 - pmbus1, 2 -  pmbus2
 * return : timestamp
 *
 */
int16_t get_pmrecv_time(int8_t bus_no, int8_t buf_no)
{
    int16_t res = -1;
    if (( bus_no != 1 && buf_no != 2) || (bus_no !=1 && bus_no !=2)){
        res = -3;
    }

    if(bus_no == 1){
        if (buf_no == 1) {
            res = *DATATIME1(_CAG_CONFIG_FPGA_ADDR);
        }
        else if (buf_no ==2 ) {
            res = *STATETIME1(_CAG_CONFIG_FPGA_ADDR);
        }
    }
    else if(bus_no == 2){
        if (buf_no == 1) {
            res = *DATATIME2(_CAG_CONFIG_FPGA_ADDR);
        }
        else if (buf_no ==2 ) {
            res = *STATETIME2(_CAG_CONFIG_FPGA_ADDR);
        }
    }
    else{
        res = -3;
    }

    return res;
}

/*
 * enable pmbus send
 * buf_no : 1 - data buffer , 2 - state buffer
 * bus_no : 1 - pmbus1, 2 -  pmbus2
 * return : 0 - ok , <0 - error
 *
 */
int16_t enable_pm_send(int bus_no,int buf_no)
{
    int16_t st = 0;
    int16_t res = 0;
    if (( buf_no != 1 && buf_no != 2) || (bus_no !=1 && bus_no !=2)){
        res = -3;
    }

    if(bus_no == 1){
        if (buf_no == 1) {
            st |= 0x1;
        }
        else if (buf_no == 1 ) {
            st |= 0x2;
        }
        *STARTSEND1(_CAG_CONFIG_FPGA_ADDR) = st;
    }
    else if(bus_no == 2){
        if (buf_no == 1) {
            st |= 0x1;
        }
        else if (buf_no == 1 ) {
            st |= 0x2;
        }
        *STARTSEND2(_CAG_CONFIG_FPGA_ADDR) = st;
    }
    else {
        res = -3;
    }
    return res;
}

/*
 * check whether receive data or ack
 * buf_no : 1 - data buffer , 2 - state buffer
 * bus_no : 1 - pmbus1, 2 -  pmbus2
 * return : 0 - recv data , <0 - no recv or errors
 *
 */
int16_t check_pm_recv(int32_t bus_no,int32_t buf_no)
{
    int16_t res = -1;
    if (( bus_no != 1 && buf_no != 2) || (bus_no !=1 && bus_no !=2)){
        res = -3;
    }

    if ( bus_no == 1){
        if (buf_no == 1) {
            if(*DATASTATUS1(_CAG_CONFIG_FPGA_ADDR) & 0x1) {
                res = 0;
            }
            else {
                res = -2;
            }
        }
        else if (buf_no ==2 ) {
            if(*STATESTATUS1(_CAG_CONFIG_FPGA_ADDR) & 0x1) {
                res = 0;
            }
            else {
                res = -2;
            }
        }
        else {
            res = -1;
        }
    }
    else if ( bus_no == 2 ) {
        if (buf_no == 1) {
            if(*DATASTATUS2(_CAG_CONFIG_FPGA_ADDR) & 0x1) {
                res = 0;
            }
            else {
                res = -2;
            }
        }
        else if (buf_no ==2 ) {
            if(*STATESTATUS2(_CAG_CONFIG_FPGA_ADDR) & 0x1) {
                res = 0;
            }
            else {
                res = -2;
            }
        }
        else {
            res = -1;
        }
    }
    return res;
}

/*
 * check whether cmbus send is over
 * bus_no : 1 - pmbus1, 2 -  pmbus2
 * return : 0 - ok, <0 - sending
 *
 */
int16_t check_pm_send(int32_t bus_no)
{
    int16_t res = 0;
    //neg
    if(bus_no == 1){
        if (*SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR) & 0x1){
            return -1;
        }
        *SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    }
    else if(bus_no == 2){
           if (*SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR) & 0x1){
            return -1;
        }
        *SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    }
    else {
        res = -3;
    }
    return res;
}


#if 0
/*test*/
int16_t pmbus_send(int32_t bus_no, int32_t buf_no, int8_t *buf, int32_t len,int32_t timeout)
{
    int16_t res = 0;
    if (( bus_no != 1 && buf_no != 2) || (bus_no !=1 && bus_no !=2)){
        res = -3;
    }
    res = copy_to_pmbuf(bus_no,buf,len);
    if (res <0)
        goto gone;

    res = enable_pm_send(bus_no,buf_no);

    if (res <0)
        goto gone;

     while( check_pm_send(bus_no)<0)
         ;

gone:
    return res;
}

int16_t pmbus_recv(int32_t bus_no, int32_t buf_no, int8_t *buf, int32_t timeout)
{
    int16_t res;
    if (( bus_no != 1 && buf_no != 2) || (bus_no !=1 && bus_no !=2)){
        res = -3;
    }

    do {
        if (check_pm_recv(bus_no,buf_no)==0){
            res = copy_from_pmbuf(bus_no,buf_no,buf);
            break;
        }
        if(timeout--)
            continue;
    }while(1);
    return res;
}
#endif
