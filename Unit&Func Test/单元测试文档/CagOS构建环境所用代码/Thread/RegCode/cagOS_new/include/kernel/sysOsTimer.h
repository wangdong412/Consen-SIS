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
 * This file contains
 */

#ifndef __INCsysbM85xxTimerh
#define __INCsysbM85xxTimerh

#ifdef __cplusplus
extern "C" {
#endif

/* includes */
#include <libc/cagboot.h>
#include <ppc/toolPpc.h>
#include <ppc/excPpcLib.h>
#include <ppc/ppc85xx.h>

/* defines */

#define DEFAULT_DEC_TICKS_PER_SECOND           1000
#define	M85XX_MAX_COUNT_VALUE	               0xffffffff

#define TCR_DIE         (_PPC_TCR_DIE)
#define TCR_ARE         (_PPC_TCR_ARE_U   << 16)
#define TSR_DIS         (_PPC_TSR_DIS)
#define TCR_FP          (_PPC_TCR_FP_U    << 16)
#define TCR_FP_EXT      (0x0001e000)
#define TCR_FIE         (_PPC_TCR_FIE_U   << 16)
#define TSR_FIS         (_PPC_TSR_FIS_U   << 16)
#define TCR_WIE         (_PPC_TCR_WIE_U   << 16)
#define TSR_WIS         (_PPC_TSR_WIS_U   << 16)
#define TCR_WP          (_PPC_TCR_WP_U    << 16)
#define TCR_WP_EXT      (0x001e  << 16)

/* externs */

extern UINT32   sysTcrGet (void);            /* timer ctrl reg */
extern void     sysTcrSet (UINT32);
extern UINT32   sysTsrGet (void);            /* timer status reg */
extern void     sysTsrSet (UINT32);
extern void     sysDecIntAck (void);         /* Decrementer Interrupt acknowledge */
extern void     sysFitIntAck (void);         /* FIT Interrupt acknowledge */
extern void	sysDecSet (UINT32);
extern void	sysDecarSet (UINT32);        /* dec auto reload reg */
extern void     sysMsrSet (UINT32);

#ifdef __cplusplus
}
#endif

#endif /* __INCsysbM85xxTimerh */
