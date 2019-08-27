/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  platform dec code for p1020rdb.
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
 * This file contains the board DEC component
 */

#include <libc/stdio.h>
#include <libc/cagboot.h>

extern UINT32 sysClkFreqGet(void);

#define CONFIG_SYS_FSL_TBCLK_DIV 8

#define PPC_READ_TSC(t)                                         \
       ({                                                       \
               UINT32 __tbu;                                    \
               __asm__ __volatile__ ("1: mftbu %0\n"            \
                                     "mftb %1\n"                \
                                     "mftbu %2\n"               \
                                     "cmpw %2,%0\n"             \
                                     "bne- 1b\n"                \
                                     :"=r" (((UINT32 *)&t)[0]), \
                                      "=r" (((UINT32 *)&t)[1]), \
                                      "=r" (__tbu));            \
               t;                                               \
       })

static UINT32 tbldiv (UINT64 *a, UINT32 d)
{
    UINT32 r = *a % d;
    *a /= d;
    return r;
}

#define TBL_DIV(a, d) tbldiv(&(a), (d))

/*
 * Get timebase clock frequency
 */
UINT32 get_tbclk (void)
{
    UINT32 tbclk_div = CONFIG_SYS_FSL_TBCLK_DIV;
    UINT32 busClk  = sysClkFreqGet ();
    return (busClk  + (tbclk_div >> 1)) / tbclk_div;
}

UINT32 getTblus (void)
{
    UINT32 us=0;
    UINT64 delta=0;
    UINT32 tbFreq = get_tbclk();
    PPC_READ_TSC(delta);
    TBL_DIV(delta, tbFreq/1000000+1);
    us = (UINT32)delta;
    return us;
}

#if 0
UINT32 getTblms (void)
{
    UINT32 ms=0;
    UINT64 delta=0;
    UINT32 tbFreq = get_tbclk();
    PPC_READ_TSC(delta);
    ms = ((((UINT32)(delta)) * 1000) / (tbFreq));
    return ms;
}
#endif

UINT32 getTblms (void)
{
    UINT32 tbl;
    UINT32 tbu1, tbu2;
    UINT32 ms;
    unsigned long long tmp;

    UINT32 tbclk = get_tbclk();
    if (tbclk < 1000) {
        return 0;
    }

    /* get the timebase ticks */
    do {
        __asm__ volatile ("mftbu %0":"=r" (tbu1):);
        __asm__ volatile ("mftb %0":"=r" (tbl):);
        __asm__ volatile ("mftbu %0":"=r" (tbu2):);
    } while (tbu1 != tbu2);

    /* convert ticks to ms */
    tmp = (unsigned long long)(tbu1);
    tmp = (tmp << 32);
    tmp += (unsigned long long)(tbl);
    ms = tmp/(tbclk/1000);
    return ms;
}

UINT32 get_fpga(void)
{
    UINT16 time1   = 0;
    UINT16 time1s  = 0;
    UINT16 time1ss = 0;
    UINT16 time2   = 0;
    UINT16 time2s  = 0;
    UINT16 time2ss = 0;
    UINT16 time3   = 0;
    UINT32 time    = 0;
    time1s  = *(VUINT16 *)0xff7000a0;
    time2s = *(VUINT16 *)0xff7000a2;
    time1ss  = *(VUINT16 *)0xff7000a0;
    time2ss = *(VUINT16 *)0xff7000a2;
    if(time2s == time2ss) {
        if( time1s < time1ss) {
            time1 = time1s;
        }
        else {
            time1 = time1ss;
        }
        time2 = time2s;
    }
    else {
        time1  = *(VUINT16 *)0xff7000a0;
        time2 = time2ss;
    }
    time3 = *(VUINT16 *)0xff7000a4;
    time = time1 | (time2 << 16);
    return time;
}

UINT32 get_free(void)
{
    UINT16 time1  = 0;
    UINT16 time1s = 0;
    UINT16 time1ss = 0;
    UINT16 time2s = 0;
    UINT16 time2ss = 0;
    UINT16 time2  = 0;
    UINT16 time3  = 0;
    UINT32 time   = 0;
    time1s  = *(VUINT16 *)0xff700120;
    time2s  = *(VUINT16 *)0xff700122;
    time1ss = *(VUINT16 *)0xff700120;
    time2ss = *(VUINT16 *)0xff700122;
    if(time2s == time2ss) {
        if( time1s < time1ss) {
            time1 = time1s;
        }
        else {
            time1 = time1ss;
        }
        time2 = time2s;
    }
    else {
        time1  = *(VUINT16 *)0xff700120;
        time2 = time2ss;
    }
    time3 = *(VUINT16 *)0xff700124;
    time = time1 | (time2 << 16);
    return time;
}


