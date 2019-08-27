/* sysL2Cache.c -  L2 cache routines */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  L2Cache code for PPC85XX.
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
 * This file contains the L2Cache library
 */

/*
DESCRIPTION
L2 cache support routines including populating hooks for cache
library access.

INCLUDE FILES: sysL2Cache.h
*/

#include <libc/cagboot.h>
#include <config.h>
#include <libc/stdio.h>
#include <libc/access.h>
#include <libc/system.h>
#include "sysL2Cache.h"

/* L2 Cache Routines */

IMPORT VOIDFUNCPTR _pSysL2CacheInvFunc;
IMPORT VOIDFUNCPTR _pSysL2CacheEnable;
IMPORT VOIDFUNCPTR _pSysL2CacheDisable;
IMPORT VOIDFUNCPTR _pSysL2CacheFlush;
IMPORT UINT32 sysL2CTLGet(VUINT32 *);
IMPORT void sysL2CTLSet(UINT32,VUINT32*);
IMPORT char * cachePpcReadOrigin;
IMPORT UINT32 ppcE500DCACHE_LINE_NUM;
IMPORT UINT32 ppcE500CACHE_ALIGN_SIZE;

LOCAL void sysL2CacheInvFunc(void);
LOCAL void sysL2CacheEnable(void);
LOCAL void sysL2CacheDisable(void);
LOCAL void sysL2CacheFlush(void);
IMPORT void sysCacheFlush(UINT32,char *,UINT32);

/* 512/64  == (Min Cache Size / L2 Cache Line Size) */
int l2CacheLineNum = ((4096 + 8192) * L2_CACHE_SIZE);

/******************************************************************************
*
* sysL2CacheInit - L2 cache initialization
*
* L2 cache initialization routine of blk size etc if only L2 cache
* else expect L2 setup by sysL2SramEnable. Attaches hooks into kernel
* cache code.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void sysL2CacheInit(void)
{
    volatile UINT32 l2CtlVal;
    int n;

    /* Get L2 cache parameters */
    l2CtlVal = sysL2CTLGet(M85XX_L2CTL(_CAG_CONFIG_CCSBAR));

    sysL2CTLSet(l2CtlVal&(~M85XX_L2CTL_L2E_MSK),M85XX_L2CTL(_CAG_CONFIG_CCSBAR));

    _pSysL2CacheInvFunc = (VOIDFUNCPTR)sysL2CacheInvFunc;
    _pSysL2CacheEnable = sysL2CacheEnable;
    _pSysL2CacheDisable = sysL2CacheDisable;
    _pSysL2CacheFlush = sysL2CacheFlush;

    /* Clean external write region registers */
    for ( n=0 ; n < NO_L2_EXT_WRITE_REGIONS ; n++) {
        *M85XX_L2CEWARn(_CAG_CONFIG_CCSBAR,n) = 0;
        *M85XX_L2CEWCRn(_CAG_CONFIG_CCSBAR,n) = 0;
    }

}


/******************************************************************************
*
* sysL2CacheInvFunc - Invalidates L2 cache
*
* This routine Invalidates the L2 Cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void sysL2CacheInvFunc ()
{
    volatile UINT32 l2CtlVal;

    /* Get present value */
    l2CtlVal = sysL2CTLGet(M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
    /* Invalidate entire L2 */
    l2CtlVal |=  M85XX_L2CTL_L2I_MSK;
    /* Set L2CTL */
    sysL2CTLSet(l2CtlVal,M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
    while (sysL2CTLGet(M85XX_L2CTL(_CAG_CONFIG_CCSBAR)) & M85XX_L2CTL_L2I_MSK);

}

/******************************************************************************
*
* sysL2CacheEnable - Enables L2 cache
*
* This routine Enables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
LOCAL void sysL2CacheEnable ()
{
    volatile UINT32 l2CtlVal;

    l2CtlVal = sysL2CTLGet(M85XX_L2CTL(_CAG_CONFIG_CCSBAR));

    l2CtlVal |= M85XX_L2CTL_L2E_MSK ;
    /* Set L2CTL */
    sysL2CTLSet(l2CtlVal,M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
}

/******************************************************************************
*
* sysL2CacheDisable - Disables L2 cache
*
* This routine Disables the L2 cache.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
LOCAL void sysL2CacheDisable ()
{
    volatile UINT32 l2CtlVal;

    /* Set L2E to disabled */
    l2CtlVal = sysL2CTLGet(M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
    l2CtlVal &= ~((UINT32)M85XX_L2CTL_L2E_MSK);
    /* Set L2CTL */
    sysL2CTLSet(l2CtlVal,M85XX_L2CTL(_CAG_CONFIG_CCSBAR));
}

/******************************************************************************
*
* sysL2CacheFlush - Flushes L1 and L2 cache lines
*
* This routine Flushes L1 and L2 cache lines.  Required to make sure L2
* is actually flushed. Uses cachePpcReadOrigin as a safe region which
* equates to start of text section.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
LOCAL void sysL2CacheFlush ()
{
    int noOfCacheLines;

    noOfCacheLines = (ppcE500DCACHE_LINE_NUM + l2CacheLineNum );

    sysCacheFlush(noOfCacheLines,cachePpcReadOrigin, ppcE500CACHE_ALIGN_SIZE);

}
