/* exception.c - PowerPC exception handling facilities */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch exception manage code for PPC85XX.
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
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains the exception handler attach.
 */

#include <libc/stddef.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include "sysExcLib.h"

exceptionDesc exceptionDescTable[EXCEPTION_MAX_NUM];


void exceInit(void)
{
    memset(exceptionDescTable,0,sizeof(exceptionDescTable));
}

/*************************************************************************
*
* unregister_exception -
*
* \NOMANUAL
*/

int unregisterException(enum EXCEPTION_TYPE exception_type)
{
    signed int ret=0;

    if(exception_type>=EXCEPTION_MAX_NUM) {
        ret=-1;
        printf("exception_num is beyond the max num :%s\n",__FUNCTION__);

    } else {
        if(exceptionDescTable[exception_type].handler!=NULL) {
            exceptionDescTable[exception_type].handler=(exception_handler)NULL;
            exceptionDescTable[exception_type].p_data=NULL;
            memset(exceptionDescTable[exception_type].name,0,sizeof(exceptionDescTable[exception_type].name));
            exceptionDescTable[exception_type].flags=0;
            exceptionDescTable[exception_type].exception_num=0xffffffff;
        }
    }
    return ret;
}

/*************************************************************************
*
* register_exception -
*
* \NOMANUAL
*/

int registerException(enum EXCEPTION_TYPE exception_type,exception_handler handler,void * private_data,const char * name)
{
    signed int ret=0;

    if(name == NULL) {
        printf("please set the exc name, %s\n",__FUNCTION__);
        return -1;
    }

    if(exception_type>=EXCEPTION_MAX_NUM) {
        ret=-1;
        printf("exception_type is beyond the max num :%s\n",__FUNCTION__);
    } else {
        if(NULL==exceptionDescTable[exception_type].handler) {
            exceptionDescTable[exception_type].handler=handler;
            exceptionDescTable[exception_type].p_data=private_data;
            strncpy(exceptionDescTable[exception_type].name,name,32);
            exceptionDescTable[exception_type].flags=0;
            exceptionDescTable[exception_type].exception_num=exception_type;
        } else {
            ret=-1;
        }
    }
    return ret;
}

/*************************************************************************
*
* executeException -
*
* \NOMANUAL
*/

void executeException(UINT8 vector)
{
    if(NULL==exceptionDescTable[vector].handler) {
        printf("i give you a interface,you should do something before i shut down the cagOS\n");
        return;
    } else {
        (exceptionDescTable[vector].handler)(vector,exceptionDescTable[vector].p_data);
    }
}
