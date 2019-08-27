/* sysCpuE500Lib.c - PowrPC E500 configuration */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch mmu code for PPC85XX.
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
 * This file contains the mmu management.
 */

/*
DESCRIPTION
This file is used to configure and initialize the PowrPC E500 cores based
on the required components or configuration. This file includes SMP & AMP
multicore flag word configuration requried among cores, and inlcudes SMP time
base synchronization.

NOMANUAL
*/

/* includes */

#include <libc/stdio.h>
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

/* defines */

#define xbit0(x, n)    ((x & (1 << (31 - n))) >> (31 - n))  /* 0..31 */
#define xbit32(x, n)   ((x & (1 << (63 - n))) >> (63 - n))  /* 32..63 */
#define onoff0(x, n)   xbit0(x, n) ? "ON", "OFF"
#define onoff32(x, n)  xbit32(x, n) ? "ON", "OFF"
#define MAX_CPUS 2
#define TLB1INDEX 15

/* globals */

/* externals */
IMPORT int     intCpuLock (void);
IMPORT void    intCpuUnlock (int);
IMPORT void     mmuPpcTlbWriteExecute(UINT32 mas0, UINT32 mas1,
                                      UINT32 mas2, UINT32 mas3, UINT32 mas7);
IMPORT UINT32 sysCpuIndexGet (void);

/* locals */
void cpuE500Show (void);
int    firstStaticDesc[MAX_CPUS];
int    numberStaticAvail[MAX_CPUS];
int    totalStaticDesc  = MMUE500_NUM_STATIC_TLB_ENTIRES;

/*******************************************************************************
*
* cpuE500Show - Show CPU registers
*
* This routine shows the CPU registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void cpuE500Show (void)
{
    volatile UINT32 tmp, tmp2;

    tmp = sysMsrGet();
    (void) printf("MSR - 0x%x\n", tmp);
    (void) printf("      UCLE-%x SPE-%x WE-%x CE-%x EE-%x PR-%x ME-%x\n",
                  xbit32(tmp,37), xbit32(tmp,38), xbit32(tmp,45), xbit32(tmp,46),
                  xbit32(tmp,48), xbit32(tmp,49), xbit32(tmp,51));
    (void) printf("      UBLE-%x DE-%x IS-%x DS-%x PMM-%x\n",
                  xbit32(tmp,53), xbit32(tmp,54), xbit32(tmp,58), xbit32(tmp,59),
                  xbit32(tmp,61));
    tmp = sysHid0Get();

    tmp2 = sysHid1Get();
    (void) printf("HID0 = 0x%x, HID1 = 0x%x\n", tmp, tmp2);

    tmp = sysL1CSR0Get();
    (void) printf("L1CSR0: cache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = sysL1CSR1Get();
    (void) printf("L1CSR1: Icache is %s - 0x%x\n", tmp&1?"ON":"OFF", tmp);
    tmp = sysL1CFG0Get();
    tmp2 = sysL1CFG1Get();
    (void) printf("L1CFG0 = 0x%x, L1CFG1 = 0x%x\n", tmp, tmp2);
}

/*******************************************************************************
*
* mmuE500TlbStaticEntrySet - write a static TLB entry
*
* This function writes one MMU TLB Entry based on the given TLB Entry
* Description.
*
* CAVEAT: If previously-enabled caching is being disabled, the caller must
* pre-flush or pre-invalidate the appropriate cache lines prior to calling
* this function.
*
* NOTE: For MMU library internal use only.
*
* RETURNS: N/A
*/

LOCAL void mmuE500TlbStaticEntrySet
(
    int	             index,     /* index of TLB Entry to set */
    TLB_ENTRY_DESC * pTlbDesc,  /* description of TLB Entry to set */
    BOOL             cacheAllow,/* always TRUE allow caching to switched on */
    BOOL             setTid     /* Set TIDs for allocation using lock */
)
{
    MMUE500_PTE e500Pte;        /* TLB entry */

    /* zero all first, then set non-zero fields */
    e500Pte.words.word0 = 0;
    e500Pte.words.word1 = 0;
    e500Pte.words.word2 = 0;
    e500Pte.words.word3 = 0;

    /* word 0 */
    e500Pte.field.v = 1;                /* valid */
    e500Pte.field.iprot = ((pTlbDesc->attr & _MMU_TLB_IPROT) ? 1 : 0);
    e500Pte.field.tid = (setTid ? 1 : 0);  /* kernel 1 or global 0 only */
    e500Pte.field.ts = ((pTlbDesc->attr & _MMU_TLB_TS_1) ? 1 : 0);
    e500Pte.field.tsize =
        ((pTlbDesc->attr & _MMU_TLB_SZ_MASK) >> _MMU_TLB_SZ_SHIFT);

    /* word 1 */
    e500Pte.field.epn = pTlbDesc->effAddr >> MMU_RPN_SHIFT;
    if (cacheAllow == TRUE) {
        /* cache as desired */
        e500Pte.field.w = (pTlbDesc->attr & _MMU_TLB_ATTR_W ? 1 : 0);
        e500Pte.field.i = (pTlbDesc->attr & _MMU_TLB_ATTR_I ? 1 : 0);
    } else {
        /* cache inhibited -- warning, caller must preflush if necessary */
        e500Pte.field.w = 0;
        e500Pte.field.i = 1;
    }
    e500Pte.field.m = (pTlbDesc->attr & _MMU_TLB_ATTR_M ? 1 : 0);
    e500Pte.field.g = (pTlbDesc->attr & _MMU_TLB_ATTR_G ? 1 : 0);

    /* word 2 */
    e500Pte.field.rpn = pTlbDesc->realAddr >> MMU_RPN_SHIFT;
    e500Pte.field.sx = (pTlbDesc->attr & _MMU_TLB_PERM_X ? 1 : 0);
    e500Pte.field.sw = (pTlbDesc->attr & _MMU_TLB_PERM_W ? 1 : 0);
    e500Pte.field.sr = 1;               /* supervisor read on */
    e500Pte.field.ux = (pTlbDesc->attr & _MMU_TLB_PERM_UX ? 1 : 0);
    e500Pte.field.uw = (pTlbDesc->attr & _MMU_TLB_PERM_UW ? 1 : 0);
    e500Pte.field.ur = (pTlbDesc->attr & _MMU_TLB_PERM_UR ? 1 : 0);

    /* word 3 */
    e500Pte.field.erpn = pTlbDesc->realAddrExt;

    int         lockKey = intCpuLock ();

    mmuPpcTlbWriteExecute(((index << _PPC_MAS0_ESEL_BIT) &
                           _PPC_MAS0_ESEL_MASK) |  _PPC_MAS0_TLBSEL1,
                          e500Pte.words.word0,
                          e500Pte.words.word1,
                          e500Pte.words.word2,
                          e500Pte.words.word3
                         );

    intCpuUnlock (lockKey);
}

/*******************************************************************************
*
* mmuE500TlbStaticInit - initialize all static TLB entries
*
* This function initializes MMU TLB Entries from the supplied array of
* TLB Entry Descriptions.
*
* CAVEAT: If previously-enabled caching is being disabled, the caller must
* pre-flush or pre-invalidate the appropriate cache lines prior to calling
* this function.
*
* NOTE: For MMU library internal use only.
*
* RETURNS: N/A
*/

void mmuE500TlbStaticInit
(
    int		     numDescs,	/* number of TLB Entry Descriptors */
    TLB_ENTRY_DESC * pTlbDesc,	/* pointer to array of TLB Entries */
    BOOL	     cacheAllow	/* if TRUE, caching will be enabled */
)
{
    UINT32		index;		/* current index being init'ed */
    UINT32 uCpuIndex = sysCpuIndexGet ();

    firstStaticDesc[uCpuIndex] = numDescs;
    numberStaticAvail[uCpuIndex] = totalStaticDesc - numDescs;

    /* Set static TLBs only from thread0. Thread1 shares the TLBs. */

    for (index = TLB1INDEX; index > TLB1INDEX - numDescs; index--, pTlbDesc++)
        mmuE500TlbStaticEntrySet(index, pTlbDesc, cacheAllow,0);
}
