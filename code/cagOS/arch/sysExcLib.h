/* exception.h - PowerPC exception handling facilities */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch exception manage header for PPC85XX.
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
 * This file contains the exception handler.
 */


#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <libc/cagboot.h>

#define EXCEPTION_MAX_NUM 6

typedef struct exceptionDescriptor exceptionDesc;
typedef void (*exception_handler)(UINT32,void *);

struct exceptionDescriptor {
    exception_handler handler;
    unsigned int exception_num;
    void * p_data;
    UINT32 flags;
    UINT32 count;
    char name[32];
    exceptionDesc * next;
};

enum EXCEPTION_TYPE {
    EXCEPTION_MACHINE_CHECK,
    EXCEPTION_DSI,
    EXCEPTION_ISI,
    EXCEPTION_ALIGMENT,
    EXCEPTION_PROGRAM,
    EXCEPTION_SPE,
    EXCEPTION_IMISS,
    EXCEPTION_DMISS,
};

int registerException(enum EXCEPTION_TYPE exception_type,exception_handler handler,void * private_data,const char * name);
int unregisterException(enum EXCEPTION_TYPE exception_type);
#endif



