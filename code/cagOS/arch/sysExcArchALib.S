/* excALib.s - miscellaneous assembly language routines */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch registers access code for PPC85XX.
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
 * This file contains ppc85XX interface register library.
 */

/*
DESCRIPTION
This module contains miscellaneous cagOS support routines.

SEE ALSO: sysLib
*/

#define PPC750CX_ERRATUM_16_STWCX    /* IBM PPC750CX/CXe DD2.0-2.3 erratum #16
				      * stwcx. after snoop hit may store wrong
				      * data. As cost to eval PVR might be
				      * higher using static workaround for now
				      */

#define _ASMLANGUAGE
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>
#include <ppc/ppc85xx.h>

#define SVR	1023
	FUNC_EXPORT(sysSdaInit)
	FUNC_EXPORT(sysTimeBaseSet)
	FUNC_EXPORT(sysTimeBaseGet)
        FUNC_EXPORT(sysTimeBaseInit)

	FUNC_EXPORT(sysMsrGet)
	FUNC_EXPORT(sysMsrSet)
	FUNC_EXPORT(sysDearGet)
	FUNC_EXPORT(sysDearSet)
	FUNC_EXPORT(sysCsrr0Get)
	FUNC_EXPORT(sysCsrr0Set)
	FUNC_EXPORT(sysCsrr1Get)
	FUNC_EXPORT(sysCsrr1Set)
	FUNC_EXPORT(sysMcsrr0Get)
	FUNC_EXPORT(sysMcsrr0Set)
	FUNC_EXPORT(sysMcsrr1Get)
	FUNC_EXPORT(sysMcsrr1Set)
#ifdef _CAG_SPE_SUPPORT
	FUNC_EXPORT(sysSpefscrGet)
	FUNC_EXPORT(sysSpefscrSet)
#endif /* CAG_SPE_SUPPORT */
	FUNC_EXPORT(sysDecarSet)
	FUNC_EXPORT(sysL1CSR0Get)
	FUNC_EXPORT(sysL1CSR0Set)
	FUNC_EXPORT(sysL1CSR1Get)
	FUNC_EXPORT(sysL1CSR1Set)
	FUNC_EXPORT(sysL1CFG0Get)
	FUNC_EXPORT(sysL1CFG1Get)
	FUNC_EXPORT(sysIvor1Set)
	FUNC_EXPORT(sysEnableBP)
	FUNC_EXPORT(sysDisableBP)
	FUNC_EXPORT(sysBucsrGet)
	FUNC_EXPORT(sysSrr0Get)
	FUNC_EXPORT(sysSrr0Set)
	FUNC_EXPORT(sysSrr1Get)
	FUNC_EXPORT(sysSrr1Set)

	FUNC_EXPORT(sysPvrGet)
	FUNC_EXPORT(sysSvrGet)
	FUNC_EXPORT(sysFirstBit)
	FUNC_EXPORT(sysFirstRBit)
	FUNC_EXPORT(sysEieio)

	FUNC_EXPORT(sysHid0Get)
	FUNC_EXPORT(sysHid0Set)
	FUNC_EXPORT(sysHid1Get)
	FUNC_EXPORT(sysHid1Set)
    FUNC_EXPORT(sysTas)

	FUNC_EXPORT(sysDecSet)
	FUNC_EXPORT(sysDecGet)
	FUNC_EXPORT(sysDbcr2Get)
	FUNC_EXPORT(sysDbcr2Set)
	FUNC_EXPORT(sysDecIntEnable)
	FUNC_EXPORT(sysDecIntAck)
	FUNC_EXPORT(sysFitIntEnable)
	FUNC_EXPORT(sysFitIntDisable)
	FUNC_EXPORT(sysFitIntAck)
	FUNC_EXPORT(sysIvprGet)
	FUNC_EXPORT(sysIvprSet)
	FUNC_EXPORT(sysTcrGet)
	FUNC_EXPORT(sysTcrSet)
	FUNC_EXPORT(sysTsrGet)
	FUNC_EXPORT(sysTsrSet)
	FUNC_EXPORT(sysDbcr2Get)
	FUNC_EXPORT(sysDbcr2Set)
	FUNC_EXPORT(sysDbsrGet)
	FUNC_EXPORT(sysDbsrClear)
	FUNC_EXPORT(sysDac1Get)
	FUNC_EXPORT(sysDac1Set)
	FUNC_EXPORT(sysDac2Get)
	FUNC_EXPORT(sysDac2Set)
	FUNC_EXPORT(sysIac1Get)
	FUNC_EXPORT(sysIac1Set)
	FUNC_EXPORT(sysIac2Get)
	FUNC_EXPORT(sysIac2Set)
	FUNC_EXPORT(sysPirGet)		/* Get the PIR register */
	FUNC_EXPORT(sysPirSet)		/* Set the PIR register */
        FUNC_EXPORT(sysLpidSet)
	FUNC_EXPORT(sysCpuLibInit)
	FUNC_EXPORT(excIvorInit)
	FUNC_EXPORT(sysCpuIndexGet)
        FUNC_EXPORT(get_ticks) 

	_CAG_TEXT_SEG_START

/*******************************************************************************
*
* sysTas - this routine performs the atomic test and set for the PowerPC arch.
*
* SYNOPSIS
* \ss
* BOOL sysTas (void * address)
* \se
*
* DESCRIPTION: This routine provides a C-callable interface to a test-and-set
* instruction. The test-and-set instruction is executed on the specified
* address.  The architecture test-and-set instruction is:
*
* RETURNS: TRUE if the value had not been set (but is now), or FALSE if the
* value was set already.
*/

FUNC_BEGIN(sysTas)
        lis     r4, 0x8000      /* set the upper bit of r4 */
#ifdef PPC750CX_ERRATUM_16_STWCX
        sync                    /* SPR 65319:  empty L1 store queues
                                 * to ensure they are not full when stwcx.
                                 * executes.  The next lwarx might cause
                                 * one L1 store queue to fill (dirty blk
                                 * replacement) but another should be
                                 * empty.
                                 * ****************************************
                                 * **** Future code change might need  ****
                                 * **** to replace sync with eieio and ****
                                 * **** add sync closer to stwcx.      ****
                                 * ****************************************
                                 */
#else
        eieio
#endif  /* PPC750CX_ERRATUM_16_STWCX */
        lwarx   r5, 0, r3
        cmpwi   r5, 0
        bne     sysTasEnd

        stwcx.  r4, 0, r3
        eieio
        bne-    sysTas
        li      r3, 0x01
        blr
sysTasEnd:
        li      r3, 0
        blr
FUNC_END(sysTas)

/*******************************************************************************
*
* sysSdaInit - this routine sets the r2 and r13 regs to SDA2 and SDA  base addrs.
*
* void sysSdaInit (void)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSdaInit)
    /* initialize Small Data Area (SDA) start address */
	lis     r2, HI(_SDA2_BASE_)
	ori 	r2, r2, LO(_SDA2_BASE_)

	lis     r13, HI(_SDA_BASE_)
	ori	r13, r13, LO(_SDA_BASE_)
	blr
FUNC_END(sysSdaInit)

/*******************************************************************************
*
* sysMsrGet - this routine returns the content of msr
*
* UINT32 sysMsrGet (void)
*
* RETURN: content of msr.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMsrGet)
	mfmsr	p0
	blr
FUNC_END(sysMsrGet)


/*******************************************************************************
*
* sysMsrSet - this routine sets the content of msr
*
* void sysMsrSet (UINT32)
*
* RETURN: N/A.
*
* INTERNAL:
*   Guest limitation: sysMsrSet only affects _PPC_MSR_EE if EHV if off.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMsrSet)
	mtmsr	p0
	isync
	blr

FUNC_END(sysMsrSet)

/***************************************************************************
*
* sysDearGet - this routine returns the content of DEAR
*
* UINT32 sysDearGet (void)
*
* RETURNS: content of DEAR
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDearGet)
	mfspr   p0, DEAR
	blr
FUNC_END(sysDearGet)


/***************************************************************************
*
* sysDearSet - this routine sets the content of DEAR
*
* void sysDearSet (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDearSet)
	mtspr   DEAR, p0
	blr
FUNC_END(sysDearSet)

/***************************************************************************
*
* sysCsrr0Get - this routine returns the content of CSRR0
*
* UINT32 sysCsrr0Get (void)
*
* RETURNS: content of CSRR0
*
* \NOMANUAL
*/

FUNC_BEGIN(sysCsrr0Get)
	mfspr   p0, CSRR0
	blr
FUNC_END(sysCsrr0Get)


/***************************************************************************
*
* sysCsrr0Set - this routine sets the content of CSRR0
*
* void sysCsrr0Set (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysCsrr0Set)
	mtspr   CSRR0, p0
	blr
FUNC_END(sysCsrr0Set)


/***************************************************************************
*
* sysCsrr1Get - this routine returns the content of CSRR1
*
* UINT32 sysCsrr1Get (void)
*
* RETURNS: content of CSRR1
*
* \NOMANUAL
*/

FUNC_BEGIN(sysCsrr1Get)
	mfspr   p0, CSRR1
	blr
FUNC_END(sysCsrr1Get)


/***************************************************************************
*
* sysCsrr1Set - this routine sets the content of CSRR1
*
* void sysCsrr1Set (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysCsrr1Set)
	mtspr   CSRR1, p0
	blr
FUNC_END(sysCsrr1Set)

/***************************************************************************
*
* sysMcsrr0Get - this routine returns the content of MCSRR0
*
* UINT32 sysMcsrr0Get (void)
*
* RETURNS: content of MCSRR0
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMcsrr0Get)
	mfspr   p0, MCSRR0
	blr
FUNC_END(sysMcsrr0Get)


/***************************************************************************
*
* sysMcsrr0Set - this routine sets the content of MCSRR0
*
* void sysMcsrr0Set (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMcsrr0Set)
	mtspr   MCSRR0, p0
	blr
FUNC_END(sysMcsrr0Set)


/***************************************************************************
*
* sysMcsrr1Get - this routine returns the content of MCSRR1
*
* UINT32 sysMcsrr1Get (void)
*
* RETURNS: content of MCSRR1
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMcsrr1Get)
	mfspr   p0, MCSRR1
	blr
FUNC_END(sysMcsrr1Get)


/***************************************************************************
*
* sysMcsrr1Set - this routine sets the content of MCSRR1
*
* void sysMcsrr1Set (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysMcsrr1Set)
	mtspr   MCSRR1, p0
	blr
FUNC_END(sysMcsrr1Set)

#ifdef _CAG_SPE_SUPPORT
/***************************************************************************
*
* sysSpefscrGet - this routine returns the content of SPEFSCR
*
* UINT32 sysSpefscrGet (void)
*
* RETURNS: content of SPEFSCR
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSpefscrGet)
	mfspr   p0, SPEFSCR
	blr
FUNC_END(sysSpefscrGet)


/***************************************************************************
*
* sysSpefscrSet - this routine sets the content of SPEFSCR
*
* void sysSpefscrSet (UINT32)
*
* RETURNS: none
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSpefscrSet)
	mtspr   SPEFSCR, p0
	isync
	blr
FUNC_END(sysSpefscrSet)
#endif /* _CAG_SPE_SUPPORT */

/***************************************************************************
*
* sysDecarSet - this routine sets the content of the DECAR register
*
* void sysDecarSet (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDecarSet)
	mtspr	DECAR, p0			/* set DECAR */
	blr
FUNC_END(sysDecarSet)


/***************************************************************************
*
* sysL1CSR0Get - this routine sets the content of the L1CSR0 register
*
* UINT32 sysL1CSR0Get (void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CSR0Get)
	sync
	mfspr	p0, L1CSR0
	blr
FUNC_END(sysL1CSR0Get)


/***************************************************************************
*
* sysL1CSR0Set - this routine sets the content of the L1CSR0 register
*
* void sysL1CSR0Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CSR0Set)
	msync
	isync
	mtspr	L1CSR0, p0
	isync
	blr
FUNC_END(sysL1CSR0Set)


/***************************************************************************
*
* sysL1CSR1Get - this routine gets the content of the L1CSR1 register
*
* UINT32 sysL1CSR1Get (void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CSR1Get)
	sync
	mfspr	p0, L1CSR1
	blr
FUNC_END(sysL1CSR1Get)


/***************************************************************************
*
* sysL1CSR1Set - this routine sets the content of the L1CSR1 register
*
* void sysL1CSR1Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CSR1Set)
	msync
	isync
	mtspr	L1CSR1, p0
	isync
	blr
FUNC_END(sysL1CSR1Set)

/***************************************************************************
*
* sysL1CFG0Get - this routine gets the content of the L1CFG0 register
*
* UINT32 sysL1CFG0Get (void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CFG0Get)
	mfspr	p0, L1CFG0
	blr
FUNC_END(sysL1CFG0Get)


/***************************************************************************
*
* sysL1CFG1Get - this routine sets the content of the L1CFG1 register
*
* UINT32 sysL1CFG1Get (void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysL1CFG1Get)
	mfspr	p0, L1CFG1
	blr
FUNC_END(sysL1CFG1Get)


/***********************************************************************
*
* sysIvor1Set -  Write to Machine check vector offset register
*
* void sysIvor1Set (UINT32)
*
* \NOMANUAL
*
*/

FUNC_BEGIN(sysIvor1Set)
	mtspr	IVOR1, p0
	blr
FUNC_END(sysIvor1Set)

/*******************************************************************************
*
* sysSrr0Get - this routine returns the content of SRR0
*
* UINT32 sysSrr0Get (void)
*
* RETURN: content of srr0.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSrr0Get)
	mfsrr0  p0
	blr
FUNC_END(sysSrr0Get)


/*******************************************************************************
*
* sysSrr0Set - this routine modifies the content of SRR0
*
* void sysSrr0Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSrr0Set)
	mtsrr0  p0
	blr
FUNC_END(sysSrr0Set)


/*******************************************************************************
*
* sysSrr1Get - this routine returns the content of SRR1
*
* UINT32 sysSrr1Get (void)
*
* RETURN: content of srr1.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSrr1Get)
	mfsrr1  p0
	blr
FUNC_END(sysSrr1Get)


/*******************************************************************************
*
* sysSrr1Set - this routine modifies the content of SRR1
*
* void sysSrr1Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSrr1Set)
	mtsrr1  p0
	blr
FUNC_END(sysSrr1Set)

/*******************************************************************************
*
* sysFirstBit - this routine returns the first bit set in the register
*              i.e. the number of consecutive zeros (0..32) from the MSB
*
* UINT32 sysFirstBit (UINT32)
*
* RETURN: the bit position of the first set bit (0 for MSB)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysFirstBit)
	cntlzw	p0, p0
	blr
FUNC_END(sysFirstBit)


/*******************************************************************************
*
* sysFirstRBit - this routine returns the number of consecutive zeros (0..32)
*               from the LSB in the register (e.g. to detect alignment)
*
* UINT32 sysFirstRBit (UINT32)
*
* RETURN: the number of first bit set.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysFirstRBit)
	mr	p1, p0
	cntlzw	p0, p1
	cmpwi	p0, 32
	bne	findBit
	blr
findBit:
	li	p0, -1
findBit2:
	addic	p0, p0, 1
	rotrwi	p1, p1, 1
	cntlzw.	p2, p1
	bne	findBit2
	blr
FUNC_END(sysFirstRBit)


/******************************************************************************
*
* sysPvrGet - Get the processor type from the Processor Version Register
*
* UINT32 sysPvrGet (void)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysPvrGet)
	mfspr	p0, PVR
	blr
FUNC_END(sysPvrGet)

/******************************************************************************
*
* sysEieio - Execute the eieio instruction to enforce in-order execution
*
* void sysEieio(void)
*
* This routine is useful for drivers that try to write I/O registers and
* the registers have an inherent order dependency.
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysEieio)
	eieio
	blr
FUNC_END(sysEieio)

/*******************************************************************************
*
* sysHid0Set - this routine Set the content of HID0
*
* void sysHid0Set (UINT32)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysHid0Set)
	isync
	mtspr	HID0, p0
	isync
	blr
FUNC_END(sysHid0Set)


/*******************************************************************************
*
* sysHid0Get - this routine returns the content of HID0
*
* UINT32 sysHid0Get (void)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysHid0Get)
	mfspr	p0, HID0
	blr
FUNC_END(sysHid0Get)

/*******************************************************************************
*
* sysHid1Set - this routine Set the content of HID1
*
* void sysHid1Set (UINT32)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysHid1Set)
	msync
	mtspr	HID1, p0
	isync
	blr
FUNC_END(sysHid1Set)

/*******************************************************************************
*
* sysHid1Get - this routine returns the content of HID1
*
* UINT32 sysHid1Get (void)
*
* \NOMANUAL
*/

FUNC_BEGIN(sysHid1Get)
    mfspr   p0, HID1
    blr
FUNC_END(sysHid1Get)

/*******************************************************************************
*
* sysTimeBaseInit - initiiazlie Time Base for the initialization phase.
*
* This routine is used to initiiazlie Time Base for the initialization phase.
*
* void sysTimeBaseInit
* (
*   UINT32 tbHI
*   UINT32 tbLO
* )
*
* RETURNS: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTimeBaseInit)
	mttbl  p0
	mttbu  p1
	blr
FUNC_END(sysTimeBaseInit)

/*******************************************************************************
*
* sysTimeBaseSet - set time base
*
* void sysTimeBaseSet
*	(
*	UINT32	tbHI,
*	UINT32	tbLO
*	)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTimeBaseSet)
	li	p2, 0
	mtspr	TBL_W, p2		/* set TBLO to zero, preventing carry */
	mtspr	TBU_W, p0		/* set TBHI */
	mtspr	TBL_W, p1		/* set TBLO */
	blr
FUNC_END(sysTimeBaseSet)

/*******************************************************************************
*
* sysTimeBaseGet - get time base
*
* UINT32 sysTimeBaseGet
*	(
*	UINT32	* tbHI,
*	UINT32	* tbLO
*	)
*
* RETURN: upper 32 bit value of time in *(uint *)p0,
*         lower 32 bit value of time in *(uint *)p1.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTimeBaseGet)

	mfspr	p2, TBU_R
	mfspr	p3, TBL_R
	mfspr	p4, TBU_R
	cmpw	p2, p4
	bne	sysTimeBaseGet
	stw	p2, 0x0(p0)
	stw	p3, 0x0(p1)
	blr
FUNC_END(sysTimeBaseGet)

/*******************************************************************************
*
* sysDecSet - set decrementer
*
* void sysDecSet (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDecSet)
	mtspr	DEC, p0
	mtspr	DECAR, p0
	blr
FUNC_END(sysDecSet)


/*******************************************************************************
*
* sysDecGet - get decrementer
*
* UINT32 sysDecGet (void)
*
* RETURN: the value of decrementer.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDecGet)
	mfspr	p0, DEC
	blr
FUNC_END(sysDecGet)


/*******************************************************************************
*
* sysDecIntEnable - enable decrementer timer interrupt
*
* void sysDecIntEnable(void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDecIntEnable)
	mfspr   p1, TCR
	oris    p1, p1, _PPC_TCR_DIE_U
	mtspr   TCR, p1
	mfmsr   p0
	ori	p0, p0, _PPC_MSR_EE
	mtmsr   p0
	blr
FUNC_END(sysDecIntEnable)


/*******************************************************************************
*
* sysDecIntAck - acknowledge decrementer timer interrupt
*
* void sysDecIntAck(void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDecIntAck)
	addis    p0, r0, _PPC_TSR_DIS_U
	mtspr   TSR, p0
	blr
FUNC_END(sysDecIntAck)

/******************************************************************************
* sysEnableBP - enable branch prediction for 85xx
*
* SYNOPSIS
* \ss
* void sysEnableBP
*     (
*     void
*     )
* \se
*
* RETURNS: NONE
*
* \NOMANUAL
*/
FUNC_BEGIN(sysEnableBP)
	mfspr	p0,    BUCSR
	andi.	p1,    p0, _PPC_BUCSR_E
	bnelr
	andi.	p1,    p0, LO(~_PPC_BUCSR_FI)
	mtspr	BUCSR, p1
	isync
	ori	p1,    p0, LO(_PPC_BUCSR_FI)
	mtspr	BUCSR, p1
	isync
	mfspr	p0,    BUCSR
	ori	p1,    p0, LO(_PPC_BUCSR_E)
	mtspr	BUCSR, p1
	isync
	blr
FUNC_END(sysEnableBP)

/******************************************************************************
* sysDisableBP - disable branch prediction for 85xx
*
* SYNOPSIS
* \ss
* void sysDisableBP
*     (
*     void
*     )
* \se
*
* RETURNS: NONE
*
* \NOMANUAL
*/
FUNC_BEGIN(sysDisableBP)
	mfspr	p0,    BUCSR
	andi.	p1,    p0, LO(~_PPC_BUCSR_E)
	mtspr	BUCSR, p1
	isync
	mfspr	p0,    BUCSR
	andi.	p1,    p0, LO(~_PPC_BUCSR_FI)
	mtspr	BUCSR, p1
	isync
	ori	p1,    p0, LO(_PPC_BUCSR_FI)
	mtspr	BUCSR, p1
	isync
	blr
FUNC_END(sysDisableBP)

/*******************************************************************************
*
* sysBucsrGet - this routine returns the content of BUCSR
*
* UINT32 sysBucsrGet (void)
*
* RETURN: content of BUCSR.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysBucsrGet)
	mfspr	p0,    BUCSR
	blr
FUNC_END(sysBucsrGet)

/*******************************************************************************
*
* sysDbcr0Get - this routine gets DBCR0 register value
*
* UINT32 sysDbcr0Get (void)
*
* RETURN: debug control register value.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr0Get)
	mfspr	p0, DBCR0
	blr
FUNC_END(sysDbcr0Get)


/*******************************************************************************
*
* sysDbcr0Set - this routine sets DBCR0 register to a specific value
*
* void sysDbcr0Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr0Set)
	mtspr	DBCR0, p0
	isync
	blr
FUNC_END(sysDbcr0Set)


/*******************************************************************************
*
* sysDbcr1Get - this routine gets DBCR1 register value
*
* UINT32 sysDbcr1Get (void)
*
* RETURN: debug control register value.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr1Get)
	mfspr	p0, DBCR1
	blr
FUNC_END(sysDbcr1Get)


/*******************************************************************************
*
* sysDbcr1Set - this routine sets DBCR1 register to a specific value
*
* void sysDbcr1Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr1Set)
	mtspr	DBCR1, p0
	isync
	blr
FUNC_END(sysDbcr1Set)


/*******************************************************************************
*
* sysDbcr2Get - this routine gets DBCR2 register value
*
* UINT32 sysDbcr2Get (void)
*
* RETURN: debug control register value.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr2Get)
	mfspr	p0, DBCR2
	blr
FUNC_END(sysDbcr2Get)


/*******************************************************************************
*
* sysDbcr2Set - this routine sets DBCR2 register to a specific value
*
* void sysDbcr2Set (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbcr2Set)
	mtspr	DBCR2, p0
	isync
	blr
FUNC_END(sysDbcr2Set)


/*******************************************************************************
*
* sysFitIntAck - acknowledge fixed interval timer interrupt
*
* void sysFitIntAck(void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysFitIntAck)
	addis	p0, r0, _PPC_TSR_FIS_U
	mtspr	TSR, p0
	blr
FUNC_END(sysFitIntAck)


/*******************************************************************************
*
* sysFitIntDisable - disable fixed interval timer interrupt
*
* void sysFitIntDisable(void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysFitIntDisable)
	mfspr	p0, TCR
	addis	p1, r0, _PPC_TCR_FIE_U
	andc	p1, p0, p1
	mtspr	TCR, p1
	blr
FUNC_END(sysFitIntDisable)


/*******************************************************************************
*
* sysFitIntEnable - enable fixed interval timer interrupt
*
* void sysFitIntEnable(void)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysFitIntEnable)
	mfspr	p1, TCR
	oris	p1, p1, _PPC_TCR_FIE_U
	mtspr	TCR, p1
	blr
FUNC_END(sysFitIntEnable)

/*******************************************************************************
*
* sysTcrGet - this routine returns the content of TCR
*
* UINT32 sysTcrGet (void)
*
* RETURN: value of tcr.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTcrGet)
	mfspr	p0, TCR
	blr
FUNC_END(sysTcrGet)


/*******************************************************************************
*
* sysTcrSet - this routine sets the content of TCR
*
* void sysTcrSet (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTcrSet)
	mtspr	TCR, p0
	blr
FUNC_END(sysTcrSet)


/*******************************************************************************
*
* sysTsrGet - this routine returns the content of TSR
*
* UINT32 sysTsrGet (void)
*
* RETURN: value of tsr.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTsrGet)
	mfspr	p0, TSR
	blr
FUNC_END(sysTsrGet)


/*******************************************************************************
*
* sysTsrSet - this routine sets the content of TSR
*
* void sysTsrSet (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysTsrSet)
	mtspr	TSR, p0
	blr
FUNC_END(sysTsrSet)

# ifdef	IVPR
/*******************************************************************************
*
* sysIvprGet - this routine returns the content of IVPR
*
* UINT32 sysIvprGet (void)
*
* RETURN: value of IVPR.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIvprGet)
	mfspr	p0, IVPR
	blr
FUNC_END(sysIvprGet)


/*******************************************************************************
*
* sysIvprSet - this routine sets the IVPR to the specified value
*
* void sysIvprSet (UINT32)
*
* RETURN: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIvprSet)
	mtspr	IVPR, p0
	blr
FUNC_END(sysIvprSet)

# endif /* IVPR */


/*******************************************************************************
*
* sysDbsrGet - return value of debug status register
*
* UINT32 sysDbsrGet (void)
*
* RETURNS: value of debug status register.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbsrGet)
	mfspr	p0, DBSR
	blr
FUNC_END(sysDbsrGet)


/*******************************************************************************
*
* sysDbsrClear - clear debug status register bits
*
* void sysDbsrClear(UINT32)
*
* RETURNS: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDbsrClear)
	mtspr	DBSR, p0
	isync
	blr
FUNC_END(sysDbsrClear)


/*******************************************************************************
*
* sysDac1Get - return value of data adress compare register 1
*
* UINT32 sysDac1Get (void)
*
* RETURNS: value of data adress compare register 1.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDac1Get)
	mfspr	p0, DAC1
	blr
FUNC_END(sysDac1Get)


/*******************************************************************************
*
* sysDac1Set - set data adress compare register 1 to a specific value
*
* void sysDac1Set (UINT32)
*
* RETURNS: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDac1Set)
	mtspr	DAC1, p0
	blr
FUNC_END(sysDac1Set)


/*******************************************************************************
*
* sysDac2Get - return value of data adress compare register 2
*
* UINT32 sysDac2Get (void)
*
* RETURNS: value of data adress compare register 2.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDac2Get)
	mfspr	p0, DAC2
	blr
FUNC_END(sysDac2Get)


/*******************************************************************************
*
* sysDac2Set - set data adress compare register 2 to a specific value
*
* void sysDac2Set (UINT32)
*
* RETURNS: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysDac2Set)
	mtspr	DAC2, p0
	blr
FUNC_END(sysDac2Set)


/*******************************************************************************
*
* sysIac1Get - return value of instruction adress compare register 1
*
* UINT32 sysIac1Get (void)
*
* RETURNS: value of instruction adress compare register 1.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIac1Get)
	mfspr	p0, IAC1
	blr
FUNC_END(sysIac1Get)


/*******************************************************************************
*
* sysIac1Set - set instruction adress compare register 1 to a specific value
*
* void sysIac1Set (UINT32)
*
* RETURNS: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIac1Set)
	mtspr	IAC1, p0
	isync
	blr
FUNC_END(sysIac1Set)


/*******************************************************************************
*
* sysIac2Get - return value of instruction adress compare register 2
*
* UINT32 sysIac2Get (void)
*
* RETURNS: value of instruction adress compare register 2.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIac2Get)
	mfspr	p0, IAC2
	blr
FUNC_END(sysIac2Get)


/*******************************************************************************
*
* sysIac2Set - set instruction adress compare register 2 to a specific value
*
* void sysIac2Set (UINT32)
*
* RETURNS: N/A.
*
* \NOMANUAL
*/

FUNC_BEGIN(sysIac2Set)
	mtspr	IAC2, p0
	isync
	blr
FUNC_END(sysIac2Set)

/******************************************************************************
*
* sysSvrGet - get the system version from the System Version Register
*
* UINT32 sysSvrGet (void)
*
* This routine returns the SVR register value.
*
* RETURNS: SVR register value
*
* \NOMANUAL
*/

FUNC_BEGIN(sysSvrGet)
	mfspr	p0, SVR
	blr
FUNC_END(sysSvrGet)

#ifdef PIR

/******************************************************************************
*
* sysPirGet - get the processor ID from the Processor Identification Register
*
* UINT32 sysPirGet (void)
*
* This routine returns the PIR register value.
*
* RETURNS: PIR register value
*
* \NOMANUAL
*/

FUNC_BEGIN(sysPirGet)
	mfspr	p0, PIR
	blr
FUNC_END(sysPirGet)

/******************************************************************************
*
* sysPirSet - set the processor ID in the Processor Identification Register
*
* void sysPirSet (UINT32)
*
* This routine sets the PIR register value.
*
* RETURNS: N/A
*
* \NOMANUAL
*/

FUNC_BEGIN(sysPirSet)
	mtspr	PIR, p0
	blr
FUNC_END(sysPirSet)

#endif /* PIR */

/*******************************************************************************
*
* sysCpuLibInit - init to support CPU identification and index functions
*
* void sysCpuLibInit (void)
*
* RETURNS: CPU ID
*
*/

FUNC_BEGIN(sysCpuLibInit)
	li	p0, 0
	mtspr	PIR, p0
	blr
FUNC_END(sysCpuLibInit)


/*******************************************************************************
*
* sysCpuIndexGet - return the SMP CPU number
*
* UINT32 sysCpuIndexGet (void)
*
* On an SMP system, this routine returns the PIR (Processor Identification
* Register) value.  On a UP or AMP system, it returns zero.
*
* RETURNS: PIR value, or zero
*
*/


FUNC_BEGIN(sysCpuIndexGet)
	li	p0, 0
	blr
FUNC_END(sysCpuIndexGet)

/*******************************************************************************
*
* excIvorInit - set IVOR's as defined in excPpcLib.h
*
* NOMANUAL
*
* void excIvorInit(void)
*
*/
FUNC_BEGIN(excIvorInit)
	li	p0, IVOR0_VAL
	mtspr	IVOR0, p0
	li	p0, IVOR1_VAL
	mtspr	IVOR1, p0
	li	p0, IVOR2_VAL
	mtspr	IVOR2, p0
	li	p0, IVOR3_VAL
	mtspr	IVOR3, p0
	li	p0, IVOR4_VAL
	mtspr	IVOR4, p0
	li	p0, IVOR5_VAL
	mtspr	IVOR5, p0
	li	p0, IVOR6_VAL
	mtspr	IVOR6, p0
	li	p0, IVOR7_VAL
	mtspr	IVOR7, p0
	li	p0, IVOR8_VAL
	mtspr	IVOR8, p0
	li	p0, IVOR9_VAL
	mtspr	IVOR9, p0
	li	p0, IVOR10_VAL
	mtspr	IVOR10, p0
	li	p0, IVOR11_VAL
	mtspr	IVOR11, p0
	li	p0, IVOR12_VAL
	mtspr	IVOR12, p0
	li	p0, IVOR13_VAL
	mtspr	IVOR13, p0
	li	p0, IVOR14_VAL
	mtspr	IVOR14, p0
	li	p0, IVOR15_VAL
	mtspr	IVOR15, p0
# ifdef	IVOR32
	li	p0, IVOR32_VAL
	mtspr	IVOR32, p0
# endif /* IVOR32 */
# ifdef	IVOR33
	li	p0, IVOR33_VAL
	mtspr	IVOR33, p0
# endif /* IVOR33 */
# ifdef	IVOR34
	li	p0, IVOR34_VAL
	mtspr	IVOR34, p0
# endif /* IVOR34 */
# ifdef  IVOR35
	li	p0, IVOR35_VAL
	mtspr	IVOR35, p0
# endif	/* IVOR35 */
	blr
FUNC_END(excIvorInit)

/*
 * unsigned long long get_ticks(void);
 *
 * read timebase as "long long"
 */
FUNC_BEGIN(get_ticks)
get_ticks:
1:  mftbu   r3
    mftb    r4
    mftbu   r5
    cmp 0,r3,r5
    bne 1b
    blr
FUNC_END(get_ticks)

