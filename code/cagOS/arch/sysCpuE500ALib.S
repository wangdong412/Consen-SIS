/* sysCpuE500ALlb.S - functions common to all E500-derived MMUs */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch Int code for PPC85XX.
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
 * This file contains arch Int lock functions.
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

    FUNC_EXPORT(intCpuLock)		/* Local Interrupt Lock routine */
    FUNC_EXPORT(intCpuUnlock)           /* Local Interrupt Unlock routine */
    FUNC_EXPORT(intCpuMicroLock)        /* Lightweight Int Lock routine */
    FUNC_EXPORT(intCpuMicroUnlock)	/* Lightweight Int Unlock routine */
    FUNC_EXPORT(intLock)                /* Interrupt Lock routine */
    FUNC_EXPORT(intUnlock)              /* Interrupt Unlock routine */

    _CAG_TEXT_SEG_START

/*******************************************************************************
*
* intCpuLock - lock out interrupts on the local CPU
*
* This routine disables interrupts on the local CPU
*
* IMPORTANT CAVEAT
* The routine intCpuLock() can be called from either interrupt or task level.
* When called from a task context, the interrupt lock level is part of the
* task context.  Locking out interrupts does not prevent rescheduling.
* Thus, if a task locks out interrupts and invokes kernel services that
* cause the task to block (e.g., taskSuspend() or taskDelay()) or causes a
* higher priority task to be ready (e.g., semGive() or taskResume()), then
* rescheduling will occur and interrupts will be unlocked while other tasks
* run.  Rescheduling may be explicitly disabled with taskLock().
*
* EXAMPLE
* .CS
*     lockKey = intCpuLock ();
*
*      ...
*
*     intCpuUnlock (lockKey);
* .CE
*
* RETURNS
* An architecture-dependent lock-out key for the interrupt state
* prior to the call.
*
* SEE ALSO: intCpuUnlock(), taskLock()

* int intCpuLock ()

*/

#if	(_CPU_CACHE_ALIGN_SIZE == 32) || (_CPU_CACHE_ALIGN_SIZE == 16)
	.balign _CPU_CACHE_ALIGN_SIZE
#endif

FUNC_BEGIN(intCpuLock)
FUNC_BEGIN(intCpuMicroLock)

/*******************************************************************************
*
* intLock - lock out interrupts

* EXAMPLE
* .CS
*     lockKey = intLock ();
*
*      ...
*
*     intUnlock (lockKey);
* .CE
*
* RETURNS
* An architecture-dependent lock-out key for the interrupt state
* prior to the call.
*
* SEE ALSO: intLockALib, intUnlock(), taskLock(), intLockLevelSet()

* int intLock ()

*/

FUNC_LABEL(intLock)
	mfmsr	 p0		/* load msr to parm0 */
	INT_MASK(p0, p1)	/* mask EE bit */
	_PPC_KERNEL_MTMSR_WITH_BLR(p1, p1)
				/* LOCK INTERRUPT */
FUNC_END(intCpuMicroLock)
FUNC_END(intCpuLock)


/*******************************************************************************
*
* intCpuUnlock - cancel interrupt locks on the local CPU
*
* This routine re-enables interrupts that have been disabled by the routine
* intCpuLock().  Use the architecture-dependent lock-out key obtained from the
* preceding intCpuLock() call.
*
* RETURNS: N/A
*
* SEE ALSO: intCpuLock()

* void intCpuUnlock
*	(
*	int lockKey
*	)

*/

FUNC_BEGIN(intCpuUnlock)

/*******************************************************************************
*
* intUnlock - cancel interrupt locks
*
* This routine re-enables interrupts that have been disabled by the routine
* intLock().  Use the architecture-dependent lock-out key obtained from the
* preceding intLock() call.
*
* RETURNS: N/A
*
* SEE ALSO: intLock()

* void intCpuUnlock
*   (
*   int lockKey
*   )

*/

FUNC_LABEL(intUnlock)

	mfmsr	p1			/* move MSR to parm1 */

#ifdef	_PPC_MSR_CE_U
	rlwinm	p0, p0, 0, _PPC_MSR_BIT_CE, _PPC_MSR_BIT_EE
	rlwinm	p0, p0, 0, _PPC_MSR_BIT_EE, _PPC_MSR_BIT_CE
#else	/* _PPC_MSR_CE_U */
	rlwinm	p0, p0, 0, _PPC_MSR_BIT_EE, _PPC_MSR_BIT_EE
#endif	/* _PPC_MSR_CE_U */

	or	p0, p1, p0		/* restore EE bit (and CE for 4xx) */

FUNC_BEGIN(intCpuMicroUnlock)
	mtmsr	p0			/* UNLOCK INTERRUPTS */
	blr

FUNC_END(intCpuMicroUnlock)
FUNC_END(intCpuUnlock)
