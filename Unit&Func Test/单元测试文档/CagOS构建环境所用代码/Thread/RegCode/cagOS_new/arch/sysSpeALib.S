/* sysSpeALib.s  - SPE assembly routines */

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

/* includes */

#define _ASMLANGUAGE
#include <config.h>
#include <libc/cagboot.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/asmPpc.h>
#include <ppc/ppc85xx.h>
#include <ppc/spePpcLib.h>

    /* globals */

    FUNC_EXPORT(speSave)
    FUNC_EXPORT(speRestore)

    _CAG_TEXT_SEG_START

/**********************************************************************
*
* speRestore - restores from memory values back into upper 32 bits 
*              of the general purpose registers, and accumulator
*
* This routine restores the SPE coprocessor context.
* It requires specific SP APU instructions to do this so enables
* the SPE bit in MSR.
*
* The context restored is:
*       - upper 32 bits of r0 - r31 registers
*       - acc register
*
* RETURNS: N/A
*
* SEE ALSO: speSave()
*
* void speRestore
*    (
*    SPE_CONTEXT * pSpeContext  /@ from where to restore context @/
*    )
*/

FUNC_BEGIN(speRestore)
	mfmsr	r11				/* r11: original MSR */
	oris	r5, r11, HI(_PPC_MSR_SPE)	/* Set SPE Bit so SPE
						 * instructions don't cause
						 * exception.
						 */
        stwu    r3, -SPE_FRAME_SIZE(r3)

	mtmsr	r5
	isync

	/*
	 * First, restore upper 32 bits of non-volatile (R14-R31) and R13
	 * registers, and 64-bit ACC register.
	 * (Do not corrupt lower 32-bit of the registers that are used by the
	 * caller of this routine (r14-r31), and in interrupt-context (r13).)
	 *
	 * Then, restore upper 32 bits of volatile registers (R0, R3-R12),
	 * R1 and R2 registers.
	 * (Do not corrupt lower 32-bit of the r1, r2, r3 and r11 registers
	 * that are later used within this function (r3, r11), or in
	 * interrupt-context (r1, r2).)
	 *
	 * The r7, r9, r10, r0, r12, r4, r6, r8, r5 can be used as 64-bit
	 * scratch registers till the upper 32 bits are restored. They are
	 * listed in the order to use as temporary 64-bit value storage with
	 * the least possibility of the process stall to maximize the run-time
	 * performance.
	 */

	/*
	 * Load upper 32 bits of R2,R3,R14-R27 and ACC from SPE_CONTEXT
	 * structure to volatile registers.
	 *
	 * The offsets, (2,14-26,32)*4-byte, of the SPE_CONTEXT
	 * structure are first accessed to force to push the data in
	 * the structure onto to D-Cache as much as possible.
	 * The data at the offsets, (4-10)*4-byte, will be cached later
	 * if the SPE_CONTEXT structure is not aligned on a 32-byte boundary.
	 * (Assuming the D-Cache line size is 32-byte, the SPE_CONTEXT
	 * structure is aligned on a 16-byte boundary, that is the alignment
	 * currently specified to create a SPE context structure in coprocLib.)
	 */

						/* Location : value      */
	evldw		r7 ,SPE_OFFSET(14)(r3)	/* r7 ( 0:31): R14( 0:31) */
						/* r7 (32:63): R15( 0:31) */
	evldw		r9 ,SPE_OFFSET(16)(r3)	/* r9 ( 0:31): R16( 0:31) */
						/* r9 (32:63): R17( 0:31) */
	evldw		r10,SPE_OFFSET(18)(r3)	/* r10( 0:31): R18( 0:31) */
						/* r10(32:63): R19( 0:31) */
	evldw		r0 ,SPE_OFFSET(20)(r3)	/* r0 ( 0:31): R20( 0:31) */
						/* r0 (32:63): R21( 0:31) */
	evldw		r12,SPE_OFFSET(22)(r3)	/* r12( 0:31): R22( 0:31) */
						/* r12(32:63): R23( 0:31) */
	evldw		r4 ,SPE_OFFSET(24)(r3)	/* r4 ( 0:31): R24( 0:31) */
						/* r4 (32:63): R25( 0:31) */
	evldw		r6 ,SPE_OFFSET(26)(r3)	/* r6 ( 0:31): R26( 0:31) */
						/* r6 (32:63): R27( 0:31) */
	evldw		r8 ,SPE_OFFSET(2)(r3)	/* r8 ( 0:31): R2 ( 0:31) */
						/* r8 (32:63): R3 ( 0:31) */
	evldd		r5 ,SPE_OFFSET(32)(r3)	/* r5 ( 0:63): ACC( 0:63) */

	/*
	 * Restore upper 32 bits of non-volatile GPRs.
	 * (Lower 32 bits are unchanged.)
	 */

	evmergehilo	r14,r7 ,r14		/* r14( 0:31): R14( 0:31) */
	evmergelo	r15,r7 ,r15		/* r15( 0:31): R15( 0:31) */
	evmergehilo	r16,r9 ,r16		/* r16( 0:31): R16( 0:31) */
	evmergelo	r17,r9 ,r17		/* r17( 0:31): R17( 0:31) */
	evmergehilo	r18,r10,r18		/* r18( 0:31): R18( 0:31) */
	evmergelo	r19,r10,r19		/* r19( 0:31): R19( 0:31) */
	evmergehilo	r20,r0 ,r20		/* r20( 0:31): R20( 0:31) */
	evmergelo	r21,r0 ,r21		/* r21( 0:31): R21( 0:31) */
	evmergehilo	r22,r12,r22		/* r22( 0:31): R22( 0:31) */
	evmergelo	r23,r12,r23		/* r23( 0:31): R23( 0:31) */

	/*
	 * Load upper 32 bits of R28-R31 from SPE_CONTEXT structure
	 * to volatile registers.
	 */

	evldw		r7 ,SPE_OFFSET(28)(r3)	/* r7 ( 0:31): R28( 0:31) */
						/* r7 (32:63): R29( 0:31) */
	evldw		r9 ,SPE_OFFSET(30)(r3)	/* r9 ( 0:31): R30( 0:31) */
						/* r9 (32:63): R31( 0:31) */

	/*
	 * Continue restoring upper 32 bits of non-volatile GPRs.
	 * (Lower 32 bits are unchanged.)
	 */

	evmergehilo	r24,r4 ,r24		/* r24( 0:31): R24( 0:31) */
	evmergelo	r25,r4 ,r25		/* r25( 0:31): R25( 0:31) */
	evmergehilo	r26,r6 ,r26		/* r26( 0:31): R26( 0:31) */
	evmergelo	r27,r6 ,r27		/* r27( 0:31): R27( 0:31) */

	/*
	 * Restore upper 32 bits of R2 and R3.
	 * (Lower 32 bits are unchanged.)
	 */

	evmergehilo	r2 ,r8 ,r2		/* r2 ( 0:31): R2 ( 0:31) */
	evmergelo	r3 ,r8 ,r3		/* r3 ( 0:31): R3 ( 0:31) */

	/* 
	 * Load upper 32 bits of R10 from SPE_CONTEXT structure.
	 *
	 * The data at the offsets, (4-10)*4-byte, will be cached here
	 * if the SPE_CONTEXT structure is not aligned on a 32-byte boundary.
	 * (Assuming the D-Cache line size is 32-byte, the SPE_CONTEXT
	 * structure is aligned on a 16-byte boundary.)
	 */
	 
	/* Restore upper 32 bits of volatile register. */

	evldw		r10,SPE_OFFSET(10)(r3)	/* r10( 0:31): R10( 0:31) */
						/* r10(32:63): R11( 0:31) */

	/* restore 64-bit accumulator */

	evmra		r5 ,r5			/* acc( 0:63): ACC( 0:63) */

	/*
	 * Continue restoring upper 32 bits of non-volatile GPRs.
	 * (Lower 32 bits are unchanged.)
	 */

	evmergehilo	r28,r7 ,r28		/* r28( 0:31): R28( 0:31) */
	evmergelo	r29,r7 ,r29		/* r29( 0:31): R29( 0:31) */
	evmergehilo	r30,r9 ,r30		/* r30( 0:31): R30( 0:31) */
	evmergelo	r31,r9 ,r31		/* r31( 0:31): R31( 0:31) */

	/* 
	 * Load upper 32 bits of R0, R1 and R4-R11,R12,R13 from
	 * SPE_CONTEXT structure to volatile registers.
	 *
	 * Continue restoring upper 32 bits of volatile registers.
	 */

	evldw		r0 ,SPE_OFFSET(0)(r3)	/* r0 ( 0:31): R0 ( 0:31) */
						/* r0 (32:63): R1 ( 0:31) */
	evldw		r12,SPE_OFFSET(12)(r3)	/* r12( 0:31): R12( 0:31) */
						/* r12(32:63): R13( 0:31) */
	evldw		r4 ,SPE_OFFSET(4)(r3)	/* r4 ( 0:31): R4 ( 0:31) */
						/* r4 (32:63): R5 ( 0:31) */
	evldw		r6 ,SPE_OFFSET(6)(r3)	/* r6 ( 0:31): R6 ( 0:31) */
						/* r6 (32:63): R7 ( 0:31) */
	evldw		r8 ,SPE_OFFSET(8)(r3)	/* r8 ( 0:31): R8 ( 0:31) */
						/* r8 (32:63): R9 ( 0:31) */

	/* Continue restoring upper 32 bits of volatile registers. */

	evmergelo	r11,r10,r11		/* r11( 0:31): R11( 0:31) */

	/*
	 * Restore upper 32 bits of SP (R1).
	 * (Lower 32 bits are unchanged.)
	 */

	evmergelo	r1 ,r0 ,r1		/* r1 ( 0:31): R1 ( 0:31) */

	/*
	 * Restore upper 32 bits of R13.
	 * (Lower 32 bits are unchanged.)
	 */

	evmergelo	r13,r12,r13		/* r13( 0:31): R13( 0:31) */


	/* Continue restoring upper 32 bits of volatile registers. */

	evmergelo	r5 ,r4 ,r5		/* r5 ( 0:31): R5 ( 0:31) */
	evmergelo	r7 ,r6 ,r7		/* r7 ( 0:31): R7 ( 0:31) */
	evmergelo	r9 ,r8 ,r9		/* r9 ( 0:31): R9 ( 0:31) */

	mtmsr	r11				/* restore MSR */
	isync					/* CSI */
	blr
FUNC_END(speRestore)

/**********************************************************************
*
* speSave - saves the upper 32 bits of the general purpose registers, and
*           accumulator
*
* This routine saves the SPE coprocessor context.
* It requires specific SP APU instructions to do this so enables
* the SPE bit in MSR.
*
* The context saved is:
*       - upper 32 bits of r0 - r31 registers
*       - acc register
*
* RETURNS: N/A
*
* SEE ALSO: speRestore()
*
* void speSave
*    (
*    SPE_CONTEXT * pSpeContext  /@ where to save context @/
*    )
*/
	
FUNC_BEGIN(speSave)
	mfmsr  r4                       /* r4: original MSR */
        oris   r5,r4,HI(_PPC_MSR_SPE)	/* Set SPE Bit so SPE instructions don't cause exception */  
        mtmsr  r5               
        isync			        /* CSI */

        stwu    r3, -SPE_FRAME_SIZE(r3)

	evstwwe r0,SPE_OFFSET(0)(r3)	/* Store Upper word into context offset for r0 */
	evstwwe r1,SPE_OFFSET(1)(r3)
	evstwwe r2,SPE_OFFSET(2)(r3)
	evstwwe r3,SPE_OFFSET(3)(r3)
	evstwwe r4,SPE_OFFSET(4)(r3)
	evstwwe r5,SPE_OFFSET(5)(r3)
	evstwwe r6,SPE_OFFSET(6)(r3)
	evstwwe r7,SPE_OFFSET(7)(r3)
	evstwwe r8,SPE_OFFSET(8)(r3)
	evstwwe r9,SPE_OFFSET(9)(r3)
	evstwwe r10,SPE_OFFSET(10)(r3)
	evstwwe r11,SPE_OFFSET(11)(r3)
	evstwwe r12,SPE_OFFSET(12)(r3)
	evstwwe r13,SPE_OFFSET(13)(r3)
	evstwwe r14,SPE_OFFSET(14)(r3)
	evstwwe r15,SPE_OFFSET(15)(r3)
	evstwwe r16,SPE_OFFSET(16)(r3)
	evstwwe r17,SPE_OFFSET(17)(r3)
	evstwwe r18,SPE_OFFSET(18)(r3)
	evstwwe r19,SPE_OFFSET(19)(r3)
	evstwwe r20,SPE_OFFSET(20)(r3)
	evstwwe r21,SPE_OFFSET(21)(r3)
	evstwwe r22,SPE_OFFSET(22)(r3)
	evstwwe r23,SPE_OFFSET(23)(r3)
	evstwwe r24,SPE_OFFSET(24)(r3)
	evstwwe r25,SPE_OFFSET(25)(r3)
	evstwwe r26,SPE_OFFSET(26)(r3)
	evstwwe r27,SPE_OFFSET(27)(r3)
	evstwwe r28,SPE_OFFSET(28)(r3)
	evstwwe r29,SPE_OFFSET(29)(r3)
	evstwwe r30,SPE_OFFSET(30)(r3)
	evstwwe r31,SPE_OFFSET(31)(r3)

	/* To avoid corruption of r5 this is done at the end */

	evxor r5, r5, r5 
	evmwumiaa r5, r5, r5
	evstdd  r5,SPE_OFFSET(32)(r3)	/* Offset 32 words into structure for acc */
    mtmsr  r4                       /* restore MSR */
    isync							/* CSI */
	blr
FUNC_END(speSave)
