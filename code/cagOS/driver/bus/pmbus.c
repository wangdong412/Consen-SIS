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

/*pmbus1*/
#define  STARTSEND1(base)     ((VUINT16 *)(base + 0x0))
#define  SENDSTATUS1(base)    ((VUINT16 *)(base + 0x2))
#define  SENDLEN1(base)       ((VUINT16 *)(base + 0x4))
#define  DATASTATUS1(base)    ((VUINT16 *)(base + 0x6))
#define  DATALEN1(base)       ((VUINT16 *)(base + 0x8))
#define  DATATIME1(base)      ((VUINT16 *)(base + 0xa))
#define  STATESTATUS1(base)   ((VUINT16 *)(base + 0xc))
#define  STATELEN1(base)      ((VUINT16 *)(base + 0xe))
#define  STATETIME1(base)     ((VUINT16 *)(base + 0x10))

/*pmbus2*/
#define  STARTSEND2(base)     ((VUINT16 *)(base + 0x20))
#define  SENDSTATUS2(base)    ((VUINT16 *)(base + 0x22))
#define  SENDLEN2(base)       ((VUINT16 *)(base + 0x24))
#define  DATASTATUS2(base)    ((VUINT16 *)(base + 0x26))
#define  DATALEN2(base)       ((VUINT16 *)(base + 0x28))
#define  DATATIME2(base)      ((VUINT16 *)(base + 0x2a))
#define  STATESTATUS2(base)   ((VUINT16 *)(base + 0x2c))
#define  STATELEN2(base)      ((VUINT16 *)(base + 0x2e))
#define  STATETIME2(base)     ((VUINT16 *)(base + 0x30))

/*station*/
#define  SLOT(base)    	      ((VUINT16 *)(base + 0x60))
#define  STATION(base)        ((VUINT16 *)(base + 0x6e))

#define pmbus_log(log)
/*
 * check slot ok
 *
 * return : -1,     A slot not equals to B slot
 *          0,  set ok
 *
 */
int8_t set_pm_slot(void)
{
    uint16_t s1;
    int8_t s2, s3;
    int8_t ret = 0;

    s1 = *SLOT(_CAG_CONFIG_FPGA_ADDR) ;
    if ( (s1 & 0x8000) && (s1 & 0x80)) {
        s2 = s1 & 0xff;
        s3 = (s1 & 0xff00)>>8;
        if (s2 == s3) {
            s2 &= 0x3;
            *STATION(_CAG_CONFIG_FPGA_ADDR) = s2;
            ret = 0;
        } else {
            ret = -2;
            pmbus_log(PMBUS_NREADY_ERR1);
        }
    } else {
        ret = -1;
        pmbus_log(PMBUS_NREADY_ERR2);
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
    uint16_t s1;
    uint8_t s2,s3;
    int8_t res = 0;

    s1 = *SLOT(_CAG_CONFIG_FPGA_ADDR) ;
    if ( (s1 & 0x8000) && (s1 & 0x80)) {
        s2 = s1 & 0xff;
        s3 = (s1 & 0xff00)>>8;
        if (s2 == s3) {
            s2 &= 0x3;
            res = s2;
        } else {
            res = -2;
            pmbus_log(PMBUS_NREADY_ERR3);
        }
    } else {
        res = -1;
        pmbus_log(PMBUS_NREADY_ERR4);
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
    int8_t temp;
    if(len <0 || len > PMBUS_BUF_SPACE || data == NULL) {
        pmbus_log(PMBUS_PARA_ERR1);
        return -3;
    }

    if(bus_no == 1 && !(*SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR) & 1)) {
        pcie_write((uint8_t *)PMBUS1_SEND_BUF,data,(uint32_t)len);
        *SENDLEN1(_CAG_CONFIG_FPGA_ADDR) = len;
        pcie_read((uint8_t *)&temp,(uint8_t *)PMBUS1_SEND_BUF,1);
    } else if(bus_no == 2 && !(*SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR) & 1)) {
        pcie_write((uint8_t *)PMBUS2_SEND_BUF,data,(uint32_t)len);
        *SENDLEN2(_CAG_CONFIG_FPGA_ADDR) = len;
        pcie_read((uint8_t *)&temp,(uint8_t *)PMBUS2_SEND_BUF,1);
    } else {
        res = -1;
        pmbus_log(PMBUS_PARA_ERR2);
    }
    return res;
}


/*
 * get data from pmbus receive buffer
 * bus_no : 1 pmbus1, 2 pmbus2
 * buf : receive data buffer
 * buf_no : data recv buf = 1 , state recv buf = 2
 *
 */
int16_t copy_from_pmbuf(int8_t bus_no, int8_t buf_no, uint8_t *buf)
{
    int16_t len = -1;

    if(buf == NULL) {
        pmbus_log(PMBUS_CNULLP_ERR);
        return -3;
    }

    if (bus_no == 1 && buf_no == 1) {
        len = *DATALEN1(_CAG_CONFIG_FPGA_ADDR);
        pcie_read(buf,(uint8_t *)(buf_no*PMBUS_BUF_SPACE) ,(uint32_t)len);
        /*clear recv status*/
        *DATASTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else if (bus_no == 1 && buf_no == 2) {
        len = *STATELEN1(_CAG_CONFIG_FPGA_ADDR);
        pcie_read(buf,(uint8_t*)(buf_no*PMBUS_BUF_SPACE) ,(uint32_t)len);
        /*clear recv status*/
        *STATESTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else if (bus_no == 2 && buf_no == 1) {
        len = *DATALEN2(_CAG_CONFIG_FPGA_ADDR);
        pcie_read(buf,(uint8_t*)(buf_no*PMBUS_BUF_SPACE + PMBUS1_SPACE),(uint32_t)len);
        /*clear recv status*/
        *DATASTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else if (bus_no == 2 && buf_no == 2) {
        len = *STATELEN2(_CAG_CONFIG_FPGA_ADDR);
        pcie_read(buf,(uint8_t*)(buf_no*PMBUS_BUF_SPACE + PMBUS1_SPACE),(uint32_t)len);
        /*clear recv status*/
        *STATESTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else {
        len = -1;
        pmbus_log(PMBUS_PARA_ERR3);
    }

    return len;
}

void clear_precv_status(int8_t bus_no)
{
    if (bus_no == 1 ) {
        /*clear recv status*/
        *DATASTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else if (bus_no == 2 ) {
        /*clear recv status*/
        *DATASTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
    } else {
        pmbus_log(PMBUS_PARA_ERR4);
    }
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

    if (bus_no == 1 && buf_no == 1) {
        res = *DATATIME1(_CAG_CONFIG_FPGA_ADDR);
    } else if (bus_no == 1 && buf_no == 2) {
        res = *STATETIME1(_CAG_CONFIG_FPGA_ADDR);
    } else if (bus_no == 2 && buf_no == 1) {
        res = *DATATIME2(_CAG_CONFIG_FPGA_ADDR);
    } else if (bus_no == 2 && buf_no == 2) {
        res = *STATETIME2(_CAG_CONFIG_FPGA_ADDR);
    } else {
        res = -1;
        pmbus_log(PMBUS_PARA_ERR5);
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
int16_t enable_pm_send(int8_t bus_no,int8_t buf_no)
{
    int16_t st = 0;
    int16_t res = 0;
    if (bus_no == 1 && buf_no == 1) {
        st |= 0x1;
        *STARTSEND1(_CAG_CONFIG_FPGA_ADDR) = st;
    } else if (bus_no == 1 && buf_no == 2) {
        st |= 0x2;
        *STARTSEND1(_CAG_CONFIG_FPGA_ADDR) = st;
    } else if (bus_no == 2 && buf_no == 1) {
        st |= 0x1;
        *STARTSEND2(_CAG_CONFIG_FPGA_ADDR) = st;
    } else if (bus_no == 2 && buf_no == 2) {
        st |= 0x2;
        *STARTSEND2(_CAG_CONFIG_FPGA_ADDR) = st;
    } else {
        res = -1;
        pmbus_log(PMBUS_PARA_ERR6);
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
int16_t check_pm_recv(int8_t bus_no,int8_t buf_no)
{
    int16_t res = -1;
    int16_t reg = 0;

    if (bus_no == 1 && buf_no == 1) {
        reg = *DATASTATUS1(_CAG_CONFIG_FPGA_ADDR);
        if(/**DATASTATUS1(_CAG_CONFIG_FPGA_ADDR)*/ reg & 0x1) {
            if ( reg & 0x2 ){
                res = -3;
            }
            else {
                res = 0;
            }
        } else {
            res = -2;
            pmbus_log(PMBUS_DATARECVFAIL_ERR1);
        }
    } else if (bus_no == 1 && buf_no == 2) {
        reg = *STATESTATUS1(_CAG_CONFIG_FPGA_ADDR);
        if(/**STATESTATUS1(_CAG_CONFIG_FPGA_ADDR)*/reg & 0x1) {
            if ( reg & 0x2 ){
                res = -3;
            }
            else {
                res = 0;
            }
        } else {
            res = -2;
            pmbus_log(PMBUS_STATRECVFAIL_ERR1);
        }
    } else if (bus_no == 2 && buf_no == 1) {
        reg = *DATASTATUS2(_CAG_CONFIG_FPGA_ADDR);
        if(/**DATASTATUS2(_CAG_CONFIG_FPGA_ADDR)*/ reg & 0x1) {
            if ( reg & 0x2 ){
                res = -3;
            }
            else {
                res = 0;
            }
        } else {
            res = -2;
            pmbus_log(PMBUS_DATARECVFAIL_ERR2);
        }
    } else if (bus_no == 2 && buf_no == 2) {
        reg = *STATESTATUS2(_CAG_CONFIG_FPGA_ADDR);
        if(/**STATESTATUS2(_CAG_CONFIG_FPGA_ADDR)*/ reg & 0x1) {
            if ( reg & 0x2 ){
                res = -3;
            }
            else {
                res = 0;
            }
        } else {
            res = -2;
            pmbus_log(PMBUS_STATRECVFAIL_ERR2);
        }
    } else {
        res = -1;
        pmbus_log(PMBUS_PARA_ERR7);
    }

    return res;
}

/*
 * check whether cmbus send is over
 * bus_no : 1 - pmbus1, 2 -  pmbus2
 * return : 0 - ok, <0 - sending
 *
 */
int16_t check_pm_send(int8_t bus_no)
{
    int16_t res = 0;
    int16_t reg = 0;
    if(bus_no == 1) {
        reg = *SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR);
        if (/* *SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR)*/reg & 0x1) {
            pmbus_log(PMBUS_SENDFAIL_ERR1);
            return -1;
        }
        *SENDSTATUS1(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
        if (reg & 0x6) {
            return -2;
        }
    } else if(bus_no == 2) {
        reg = *SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR);
        if (/* *SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR)*/reg & 0x1) {
            pmbus_log(PMBUS_SENDFAIL_ERR2);
            return -1;
        }
        *SENDSTATUS2(_CAG_CONFIG_FPGA_ADDR) = 0xffff;
        if (reg & 0x6) {
            return -2;
        }
    } else {
        res = -3;
        pmbus_log(PMBUS_PARA_ERR8);
    }
    return res;
}
