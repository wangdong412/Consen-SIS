/* sysCacheE500Lib.c - PowerPC 85xx (E500) cache management library */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch L1cache C code for PPC85XX.
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
 * This file contains L1cache management.
 */

/*
DESCRIPTION

This library contains architecture-specific cache library functions for
the PowerPC E500 instruction and data caches.

INCLUDE FILES: mmuE500Lib.h
*/

/* includes */

#include <libc/stddef.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

/* Cache mode soft bit masks */

#define CACHE_DISABLED          0x00    /* No cache or disabled */
#define CACHE_WRITETHROUGH      0x01    /* Write-through Mode */
#define CACHE_COPYBACK          0x02    /* Copyback Mode */
#define CACHE_WRITEALLOCATE     0x04    /* Write Allocate Mode */
#define CACHE_NO_WRITEALLOCATE  0x08
#define CACHE_SNOOP_ENABLE      0x10    /* Bus Snooping */
#define CACHE_SNOOP_DISABLE     0x20
#define CACHE_BURST_ENABLE      0x40    /* Cache Burst Cycles */
#define CACHE_BURST_DISABLE     0x80

/* explicit names of the cache types */

typedef enum                            /* CACHE_TYPE */
{
    INSTRUCTION_CACHE = _INSTRUCTION_CACHE,
    DATA_CACHE        = _DATA_CACHE
} CACHE_TYPE;

/* imports */

IMPORT STATUS cacheE500EnableI_Impl  (void);
IMPORT STATUS cacheE500EnableD_Impl  (void);
IMPORT STATUS cacheE500DisableI_Impl (void);
IMPORT STATUS cacheE500DisableD_Impl (void);

IMPORT void   sysL1CSR0Set (UINT32);
IMPORT UINT32 sysL1CSR0Get (void);
IMPORT void   sysL1CSR1Set (UINT32);
IMPORT UINT32 sysL1CSR1Get (void);
IMPORT int    intCpuLock (void);
IMPORT void   intCpuUnlock (int);

/* forward declarations */

void cacheE500EnableI   (void);
void cacheE500EnableD   (void);
void cacheE500DisableI  (void);
void cacheE500DisableD  (void);
BOOL   cacheIsOnI            (void);
BOOL   cacheIsOnD            (void);

void cacheArchEnable (CACHE_TYPE cache);

void cacheE500DisableIFromMmu (void);
void cacheE500DisableDFromMmu (void);
void cacheE500InvalidateAllD  (void);

/* variables */

/* L2 cache BSP callbacks */
VOIDFUNCPTR _pSysL2CacheInvFunc = NULL;
VOIDFUNCPTR _pSysL2CacheEnable  = NULL;
VOIDFUNCPTR _pSysL2CacheFlush   = NULL;
VOIDFUNCPTR _pSysL2CacheDisable = NULL;

/*
 * low-level cache routines use buffer as the start of a memory region
 * from which we can read enough data to flush any dirty cache lines
 * out to RAM.  The region must be cacheable in order to displace
 * contents in the cache.  It should also be page-aligned.
 */
IMPORT char *cachePpcReadOrigin;

/******************************************************************************
*
* cacheArchEnable - enable cache
*
* RETURNS: OK, or ERROR if the cache type is invalid.
*
* Called directly from MMU, bypassing AIM.  Fixable?
*
* \NOMANUAL
*/

void cacheArchEnable
(
    CACHE_TYPE cache                    /* cache to enable */
)
{
    if (cache == INSTRUCTION_CACHE)
        return cacheE500EnableI ();

    if (cache == DATA_CACHE)
        return cacheE500EnableD ();
}

/******************************************************************************
*
* cacheE500EnableI - aim icache enable
*
* RETURNS: OK
*
* \NOMANUAL
*/

void cacheE500EnableI (void)
{
    int key;

    key = intCpuLock();
    (void) cacheE500EnableI_Impl (); /* return OK always */
    intCpuUnlock(key);
}


/******************************************************************************
*
* cacheE500EnableD - aim dcache enable
*
* RETURNS: OK
*
* \NOMANUAL
*/

void cacheE500EnableD (void)
{
    int key;

    key = intCpuLock();

    (void) cacheE500EnableD_Impl (); /* return OK always */

    if (_pSysL2CacheInvFunc != NULL)
        (*_pSysL2CacheInvFunc) ();

    if (_pSysL2CacheEnable != NULL)
        (*_pSysL2CacheEnable) ();

    intCpuUnlock(key);
}

/******************************************************************************
*
* cacheE500DisableI - aim icache disable
*
* RETURNS: OK
*
* \NOMANUAL
*/

void cacheE500DisableI (void)
{
    cacheE500DisableIFromMmu ();
}

/******************************************************************************
*
* cacheE500DisableIFromMmu - aim icache disable for mmu use
*
* RETURNS: N/A
*
* \NOMANUAL
*/

void cacheE500DisableIFromMmu (void)
{
    int key;

    key = intCpuLock();
    (void) cacheE500DisableI_Impl (); /* return OK always */
    intCpuUnlock(key);
}

/******************************************************************************
*
* cacheE500DisableD - aim dcache disable
*
* RETURNS: OK
*
* \NOMANUAL
*/

void cacheE500DisableD (void)
{
    cacheE500DisableDFromMmu ();
}

/******************************************************************************
*
* cacheE500DisableDFromMmu - aim dcache disable for mmu use
*
* RETURNS: N/A
*
* \NOMANUAL
*/

void cacheE500DisableDFromMmu (void)
{
    int key;
    key = intCpuLock();

    if (_pSysL2CacheFlush != NULL)
        (*_pSysL2CacheFlush) ();

    if (_pSysL2CacheDisable != NULL)
        (*_pSysL2CacheDisable) ();

    (void) cacheE500DisableD_Impl (); /* return OK always */

    intCpuUnlock(key);
}

/******************************************************************************
*
* cacheE500InvalidateAllD - aim dcache invalidate all
*
* RETURNS: OK
*
* \NOMANUAL
*/

void cacheE500InvalidateAllD (void)
{
    UINT32  licsr0;
    int     key = intCpuLock();

    if (_pSysL2CacheInvFunc != NULL)
        (*_pSysL2CacheInvFunc) ();

    while (    ((licsr0=sysL1CSR0Get()) & _PPC_L1CSR_FI) != 0);
    sysL1CSR0Set( licsr0                | _PPC_L1CSR_FI);
    while (    (        sysL1CSR0Get()  & _PPC_L1CSR_FI) != 0);

    intCpuUnlock(key);
}

/******************************************************************************
*
* cacheIsOnI - boolean function to return state of instruction cache
*
* RETURNS: state of Icache
*
* \NOMANUAL
*/

BOOL cacheIsOnI (void)
{
    return ((sysL1CSR1Get () & _PPC_L1CSR_E) != 0); /* CSR1 - Instruction */
}

/******************************************************************************
*
* cacheIsOnD - boolean function to return state of data cache
*
* RETURNS: state of Dcache
*
* \NOMANUAL
*/

BOOL cacheIsOnD (void)
{
    return ((sysL1CSR0Get () & _PPC_L1CSR_E) != 0); /* CSR0 - Data */
}
