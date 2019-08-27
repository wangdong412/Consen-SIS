/* spePpcLib.h - PowerPC E500 sipgnal processing engine support library header */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:
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
 * This file contains SPE management.
 */

#ifndef __INCspePpcLibh
#define __INCspePpcLibh

#include <libc/cagboot.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPE_OFFSET(x) (8*x)
#define ACC_SIZE  2     /* UINT32 */
#define SPE_NUM_REGS 32
#define SPE_FRAME_SIZE 0x140

#ifndef	_ASMLANGUAGE

/* This structure should be aligned to the size of a cache block */
typedef struct {	/* SPE_REG_SET */
    UINT32 gpr[SPE_NUM_REGS];	/* general pourpose registers */
    UINT32 acc[ACC_SIZE];
} SPE_CONTEXT;

#define SPEREG_SET SPE_CONTEXT

extern void speSave (SPE_CONTEXT *spePtr);
extern void speRestore (SPE_CONTEXT *spePtr);

#define SPE_CONTEXT_GET(pTcb)       (SPE_CONTEXT *)(pTcb->ExtPtr)
#define SPE_CONTEXT_SET(pTcb,X)     (pTcb->ExtPtr=(int)(X))

#endif	/* _ASMLANGUAGE */

/* define offsets in SPE_CONTEXT structure */

#ifdef __cplusplus
}
#endif

#endif  /* INCspePpcLibh */
