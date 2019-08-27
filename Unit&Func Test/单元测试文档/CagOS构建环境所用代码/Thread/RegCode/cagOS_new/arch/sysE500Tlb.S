/* sysE500Tlb.S - functions common to all E500-derived MMUs */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch MMU code for PPC85XX.
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
 * This file contains the MMU assembly functions.
 */

/*
 * This file contains assembly-language functions common to most e500
 * MMUs. This includes e500, e500v2 and e500mc.
 */

#define _ASMLANGUAGE
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

/* defines */

#define INV_TLB1 0x8
#define INV_TLB0 0x0
#define TLBSYNC()

/* externals */

/* globals */

	FUNC_EXPORT(mmuPpcPidSet)		/* set the PID register */
	FUNC_EXPORT(mmuPpcSearchPidGet)		/* get the search PID value */
	FUNC_EXPORT(mmuPpcSearchPidSet)		/* set the search PID value */
	FUNC_EXPORT(mmuPpcPidGet)		/* get the PID register */
	FUNC_EXPORT(mmuE500TlbDynamicInvalidate) /* Invalidate L1 dynamic TLB1 entries */
	FUNC_EXPORT(mmuE500TlbStaticInvalidate)  /* Invalidate L2 static (CAM) TLB1 entries */
	FUNC_EXPORT(mmuPpcE500Tlbie)		/* invalidate TLB0 entry */
	FUNC_EXPORT(mmuPpcE500TlbieStatic)	/* invalidate TLB1 entry */
	FUNC_EXPORT(mmuPpcTlbReadEntryWord0)	/* Read TLB entry word 0 */
	FUNC_EXPORT(mmuPpcTlbReadEntryWord1)	/* Read TLB entry word 1 */
	FUNC_EXPORT(mmuPpcTlbReadEntryWord2)	/* Read TLB entry word 2 */
	FUNC_EXPORT(mmuPpcTlbWriteExecute)	/* execute MAS registers and sync tlb */
	FUNC_EXPORT(mmuPpcTlb1NEntriesGet)      /* Get the number of TLB1 entries */

    	_CAG_TEXT_SEG_START

/*******************************************************************************
*
* mmuPpcTlbReadEntryWord0 - TLB Read Entry Word 0
*
*/
FUNC_BEGIN(mmuPpcTlbReadEntryWord0)
	mfspr	p0, MAS1
	blr
FUNC_END(mmuPpcTlbReadEntryWord0)

/*******************************************************************************
*
* mmuPpcTlbReadEntryWord1 - TLB Read Entry Word 1
*
*/

FUNC_BEGIN(mmuPpcTlbReadEntryWord1)
	mfspr	p0, MAS2
	blr
FUNC_END(mmuPpcTlbReadEntryWord1)

/*******************************************************************************
*
* mmuPpcTlbReadeEntryWord2 - TLB Read Entry Word 2
*
*/
FUNC_BEGIN(mmuPpcTlbReadEntryWord2)
	mfspr	p0, MAS3
	blr
FUNC_END(mmuPpcTlbReadEntryWord2)

/*******************************************************************************
*
* mmuPpcTlbReadEntryWord3 - TLB Read Entry Word 3
*
*/
FUNC_BEGIN(mmuPpcTlbReadEntryWord3)
	mfspr	p0, MAS7
	blr
FUNC_END(mmuPpcTlbReadEntryWord3)

/*******************************************************************************
*
* mmuPpcTlbWriteExecute - Execute TLB Update
*
* Write mas0..mas7 values then execute the corresponding TLB update.
*
* It is critical to pass MAS values as registers otherwise the load/execute
* sequence may be preempted by a TLB exception destroying the MAS values.
*
* Interrupts and dynamic MMU must be disabled upon entry to avoid TLB miss
* exception.
*
* USAGE: void mmuPpcTlbWriteExecute (mas0, mas1, mas2, mas3, mas7);
*/

FUNC_BEGIN(mmuPpcTlbWriteExecute)

    .balign 64

	mtspr	MAS0, r3
	mtspr	MAS1, r4
	mtspr	MAS2, r5
	mtspr	MAS3, r6
	mtspr	MAS7, r7
	isync

	tlbwe
	isync
	sync           /* Per e500   CoreRM Rev 1 tbl 3-3 note 6 */
	isync          /* Per e500mc CoreRM Rev J tbl 3-2 note 5 */

	blr
FUNC_END(mmuPpcTlbWriteExecute)

/*******************************************************************************
*
* mmuE500TlbDynamicInvalidate - Invalidate TLB0 dynamic entries
*
* The caller of this function should ensure that core migration does not
* happen while this function is being executed.
*
*/
FUNC_BEGIN(mmuE500TlbDynamicInvalidate)
	lis	p0, HIADJ(_PPC_MMUCSR0_L2TLB0_FI)
	addi	p0, p0, LO (_PPC_MMUCSR0_L2TLB0_FI)
	isync
	mtspr	MMUCSR0, p0
	isync
	blr
FUNC_END(mmuE500TlbDynamicInvalidate)

/*******************************************************************************
*
* mmuE500TlbStaticInvalidate - Invalidate TLB1 static entries
*
* The caller of this function should ensure that core migration does not
* happen while this function is being executed.
*
*/
FUNC_BEGIN(mmuE500TlbStaticInvalidate)

	lis	p0, HIADJ(_PPC_MMUCSR0_L2TLB1_FI)
	addi	p0, p0, LO (_PPC_MMUCSR0_L2TLB1_FI)
	isync
	mtspr	MMUCSR0, p0
	isync
	blr
FUNC_END(mmuE500TlbStaticInvalidate)

/*******************************************************************************
*
* mmuPpcPidSet - set the PID register
*
*/
FUNC_BEGIN(mmuPpcPidSet)
	mtspr	PID, r3
	isync
	blr
FUNC_END(mmuPpcPidSet)

/*******************************************************************************
*
* mmuPpcSearchPidGet - get the PID register
*
*/
FUNC_BEGIN(mmuPpcSearchPidGet)
	mfspr	r3, MAS6
	isync
	blr
FUNC_END(mmuPpcSearchPidGet)

/*******************************************************************************
*
* mmuPpcSearchPidSet - set the PID register
*
*/
FUNC_BEGIN(mmuPpcSearchPidSet)
	mtspr	MAS6, r3
	isync
	blr
FUNC_END(mmuPpcSearchPidSet)

/*******************************************************************************
*
* mmuPpcPidGet - get the PID register
*
*/
FUNC_BEGIN(mmuPpcPidGet)
	mfspr	r3, PID
	blr
FUNC_END(mmuPpcPidGet)

/*******************************************************************************
*
* mmuPpcE500TlbieStatic - Invalidate tlb entry for the specified effective addr
*
* INPUTS: effectiveAddr (EA) in r3
*
* Interrupts must be disabled upon entry.
*
* USAGE: mmuPpcE500TlbieStatic (void * effAddr);
*
* RETURNS: None.
*
*/

FUNC_BEGIN(mmuPpcE500TlbieStatic)
	ori	p0, p0, INV_TLB1
	b	mmuPpcE500TlbieCommon
FUNC_END(mmuPpcE500TlbieStatic)

/*******************************************************************************
*
* mmuPpcE500Tlbie - Invalidate tlb entry for the specified effective addr
*
* INPUTS: effectiveAddr (EA) in r3
*
* Interrupts must be disabled upon entry.
*
* USAGE: mmuPpcE500Tlbie (void * effAddr);
*
* RETURNS: None.
*
*/

FUNC_BEGIN(mmuPpcE500Tlbie)
#if	(INV_TLB0 != 0)
	ori	p0, p0, INV_TLB0
#endif	/* INV_TLB0 != 0 */

mmuPpcE500TlbieCommon:
	isync
	tlbivax	r0, p0
	msync
	isync
	TLBSYNC()
	blr
FUNC_END(mmuPpcE500Tlbie)

/*******************************************************************************
*
* mmuPpcTlb1NEntriesGet - Get the number of TLB1 entries.
*
*/
FUNC_BEGIN(mmuPpcTlb1NEntriesGet)
    mfspr  r3, TLB1CFG
    clrlwi r3,r3,20
    blr
FUNC_END(mmuPpcTlb1NEntriesGet)
