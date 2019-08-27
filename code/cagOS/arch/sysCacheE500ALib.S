/* cacheE500ALib.s - Assembly support file and init routines */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch cache code for PPC85XX.
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
 * This file contains the L1cache operation.
 */

/*
DESCRIPTION
This library contains routines to manipulate the PowerPC E500 family caches.

INCLUDE FILES: cacheLib.h

SEE ALSO: cacheLib
*/

#define _ASMLANGUAGE
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

	FUNC_EXPORT(cacheE500EnableI_Impl)
	FUNC_EXPORT(cacheE500EnableD_Impl)
	FUNC_EXPORT(cacheE500DisableI_Impl)
	FUNC_EXPORT(cacheE500DisableD_Impl)

	FUNC_EXPORT(cacheE500InvalidateI)
	FUNC_EXPORT(cacheE500InvalidateD)
	FUNC_EXPORT(cacheE500FlushD)
	FUNC_EXPORT(cacheE500ClearD)

	FUNC_EXPORT(cacheE500FlushAllD)
	FUNC_EXPORT(cacheE500PipeFlush)
	FUNC_EXPORT(cacheE500TextUpdate)

	DATA_IMPORT(cachePpcReadOrigin)
	DATA_IMPORT(ppcE500DCACHE_FLUSH_NUM)    /* from ARCH/PSP */
	DATA_IMPORT(ppcE500CACHE_ALIGN_SIZE)    /* from BSP */

    _CAG_TEXT_SEG_START

/******************************************************************************
*
* cacheE500EnableI_Impl -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500EnableI_Impl)
	mfspr   p1,L1CSR1
	andi.   p2,p1,_PPC_L1CSR_E
	bne	cacheArchOK
	ori     p2,p1,_PPC_L1CSR_FI
	mtspr   L1CSR1,p2
	isync
    oris    p2, p1, HI(_PPC_L1CSR_CPE | _PPC_L1CSR_E)
    ori     p2, p2, LO(_PPC_L1CSR_CPE | _PPC_L1CSR_E)
	mtspr   L1CSR1,p2
	isync
	b	cacheArchOK
FUNC_END(cacheE500EnableI_Impl)

/******************************************************************************
*
* cacheE500EnableD_Impl -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500EnableD_Impl)
	mfspr	p1, L1CSR0
	andi.   p2,p1,_PPC_L1CSR_E
	bne	cacheArchOK
	ori     p2, p1,_PPC_L1CSR_FI
	msync
	isync
	mtspr   L1CSR0,p2
    oris    p2, p1, HI(_PPC_L1CSR_CPE | _PPC_L1CSR_E)
    ori     p2, p2, LO(_PPC_L1CSR_CPE | _PPC_L1CSR_E)
	msync
	isync
	mtspr   L1CSR0,p2
	isync
	b	cacheArchOK
FUNC_END(cacheE500EnableD_Impl)

/******************************************************************************
*
* cacheE500InvalidateI -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500InvalidateI)
	icbi	r0, p0
	add	p0, p0, p2
	cmplw   p0, p1
	blt	cacheE500InvalidateI
	isync
	li      p0,OK
	blr
FUNC_END(cacheE500InvalidateI)


/******************************************************************************
*
* cacheE500InvalidateD -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500InvalidateD)
	dcbi	r0, p0
	add	p0, p0, p2
	cmplw   p0, p1
	blt	cacheE500InvalidateD
	isync
	b	cacheArchOK
FUNC_END(cacheE500InvalidateD)


/******************************************************************************
*
* cacheE500ClearD -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500ClearD)
	dcbf	r0, p0
	add	p0, p0, p2
	cmplw   p0, p1
	blt	cacheE500ClearD
	sync
	b	cacheArchOK
FUNC_END(cacheE500ClearD)


/******************************************************************************
*
* cacheE500FlushD -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500FlushD)
	dcbst	r0,p0
	add	p0,p0,p2
	cmplw	p0,p1
	blt	cacheE500FlushD
	b	cacheArchOK
FUNC_END(cacheE500FlushD)

/******************************************************************************
*
* cacheE500PipeFlush -
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500PipeFlush)
	eieio
	sync
	li      p0,OK
	blr
FUNC_END(cacheE500PipeFlush)


/******************************************************************************
*
* cacheE500TextUpdate -
*
* note: this assumes IcacheLineSize == DcacheLineSize
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500TextUpdate)
	add	p1,p1,p0
	clrrwi	p0,p0,_CPU_CACHE_ALIGN_SHIFT
ctuLoop:
	dcbst	r0,p0
	sync
	icbi	r0,p0
	add	p0,p0,p3
	cmplw	p0,p1
	blt	ctuLoop
	isync
	sync
	li      p0,OK
	blr

FUNC_END(cacheE500TextUpdate)

/******************************************************************************
*
* cacheE500DisableI_Impl - Disable the E500 PowerPC Instruction cache.
*
* The instruction cache is expected to be consistent with memory so disabling
* it is sufficient.
*
* This routine assumes it doesn't have to check if cache is enabled or not.
* RETURNS: OK or ERROR if the cache can not be disabled.
*
* SEE ALSO:
*.I "E500 Core Users Manual"
*
* NOMANUAL
*
*/

FUNC_BEGIN(cacheE500DisableI_Impl)
	mfspr   p1,L1CSR1
	andi.   p2,p1,_PPC_L1CSR_E
	beq	cacheArchOK
	lis     p2,HIADJ(~_PPC_L1CSR_E)
	addi	p2,p2,LO(~_PPC_L1CSR_E)
	and     p2,p2,p1
	mtspr   L1CSR1, p2
	isync
	b	cacheArchOK
FUNC_END(cacheE500DisableI_Impl)

/******************************************************************************
*
* cacheE500DisableD_Impl - Disable the E500 PowerPC Data cache.
*
* For data cache the routines updates ITLB and DTLBs so no
* TLB misses occur during cache flush which might cause unwanted cache usage.
* It then loads cacheable data from known addresses and then flushes them
* to memory. The cache should now be empty and can be disabled with out any
* loss of data.
*
*
* SEE ALSO:
*.I "E500 Core Users Manual"
*
* NOMANUAL
*/

FUNC_BEGIN(cacheE500DisableD_Impl)

	mfspr   p1,L1CSR0
	andi.   p2,p1,_PPC_L1CSR_E
	beq	cacheArchOK
	lis     p3, HIADJ(ppcE500DCACHE_FLUSH_NUM)
	lwz     p3, LO(ppcE500DCACHE_FLUSH_NUM)(p3)
	srwi	p2, p3, MMU_RPN_SHIFT - _CPU_CACHE_ALIGN_SHIFT
	mtspr	CTR, p2
	lis	p2, HIADJ(cachePpcReadOrigin)
	lwz	p2, LO(cachePpcReadOrigin)(p2)
	li      p6,MMU_PAGE_SIZE
	mfmsr	p5
	INT_MASK(p5, p0)
	_PPC_KERNEL_MTMSR(p0, p0, cacheE500DisableD_Impl)
	b	cachePpcDisableLoadItlb

cachePpcDisableLoadDtlb:
	lbzu	p4,0(p2)
	add     p2,p2,p6
	bdnz	cachePpcDisableLoadDtlb
	mtspr   CTR, p3
	lis     p4, HIADJ(ppcE500CACHE_ALIGN_SIZE)
	lwz     p4, LO(ppcE500CACHE_ALIGN_SIZE)(p4)
	lis	p7, HIADJ(cachePpcReadOrigin)
	lwz	p7, LO(cachePpcReadOrigin)(p7)
	subf	p2, p4, p7

cacheE500DisableLoad:
	add	p2, p4, p2
	lbzu	p7, 0(p2)
	bdnz    cacheE500DisableLoad
	isync
	mtspr   CTR, p3
	lis	p7, HIADJ(cachePpcReadOrigin)
	lwz	p7, LO(cachePpcReadOrigin)(p7)
	subf	p2, p4, p7

cacheE500DisableFlush:
	add	p2, p4, p2
	dcbf	0,p2
	bdnz    cacheE500DisableFlush
	sync
	isync
	mfspr    p2,L1CSR0
	lis      p1,HIADJ(~_PPC_L1CSR_E)
	addi	 p1,p1,LO(~_PPC_L1CSR_E)
	and      p2,p2,p1
	msync
	isync
	mtspr   L1CSR0,p2
	mtmsr	p5
	b	cacheArchOK

cachePpcDisableLoadItlb:
	b	cachePpcDisableLoadDtlb
FUNC_END(cacheE500DisableD_Impl)

/******************************************************************************
*
* cacheE500FlushAllD - Flush the E500 PowerPC Data cache.
*
* For data cache the routines updates ITLB and DTLBs so no
* TLB misses occur during cache flush which might cause unwanted cache usage.
* It then loads cacheable data from known addresses and then flushes them
* to memory. The cache should now be empty assuming only L1 enabled
*
* NOTE:
* This routine should not be called by the user application.
*
* SEE ALSO:
*.I "E500 Core Users Manual"
*
* NOMANUAL
*/
FUNC_BEGIN(cacheE500FlushAllD)
	lis     p3, HIADJ(ppcE500DCACHE_FLUSH_NUM)
	lwz     p3, LO(ppcE500DCACHE_FLUSH_NUM)(p3)
	srwi	p2, p3, MMU_RPN_SHIFT - _CPU_CACHE_ALIGN_SHIFT
	mtspr	CTR, p2
	lis	p2, HIADJ(cachePpcReadOrigin)
	lwz	p2, LO(cachePpcReadOrigin)(p2)
	li      p6,MMU_PAGE_SIZE
	mfmsr	p5
	INT_MASK(p5, p0)
	_PPC_KERNEL_MTMSR(p0, p0, cacheE500FlushAllD)
	b	cachePpcDisableLoadItlb2

cachePpcDisableLoadDtlb2:
	lbzu	p4,0(p2)
	add     p2,p2,p6
	bdnz	cachePpcDisableLoadDtlb2
	mtspr   CTR, p3
	lis     p4, HIADJ(ppcE500CACHE_ALIGN_SIZE)
	lwz     p4, LO(ppcE500CACHE_ALIGN_SIZE)(p4)
	lis	p7, HIADJ(cachePpcReadOrigin)
	lwz	p7, LO(cachePpcReadOrigin)(p7)
	subf	p2, p4, p7

cacheE500DisableLoad2:
	add	p2, p4, p2
	lbzu	p7, 0(p2)
	bdnz    cacheE500DisableLoad2
	isync
	mtspr   CTR, p3
	lis	p7, HIADJ(cachePpcReadOrigin)
	lwz	p7, LO(cachePpcReadOrigin)(p7)
	subf	p2, p4, p7

cacheE500DisableFlush2:
	add	p2, p4, p2
	dcbf	0,p2
	bdnz    cacheE500DisableFlush2
	sync
	isync
	mtmsr	p5
	blr

cachePpcDisableLoadItlb2:
	b	cachePpcDisableLoadDtlb2
FUNC_END(cacheE500FlushAllD)

/*************************************************************************
*
* cacheArchOK - common OK return routine
*
* NOMANUAL
*
*/

FUNC_BEGIN(cacheArchOK)
	sync
cacheArchOkNoSync:
	li      p0,OK		/* return OK */
	blr
FUNC_END(cacheArchOK)
