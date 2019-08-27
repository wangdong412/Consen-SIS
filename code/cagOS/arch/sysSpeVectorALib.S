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
    mfmsr  r5      
    oris   r5, r5, HI(_PPC_MSR_SPE)   
    b 1f
    .balign 64
1:
    mtsrr1  r5    
    lis     r5, HI(2f)     
    ori     r5, r5, LO(2f)
    mtsrr0  r5 
    rfi
    2:
    
    subi   r3,r3,SPE_FRAME_SIZE 
    //stwu    r3, -SPE_FRAME_SIZE(r3)
	evldd r0,SPE_OFFSET(0)(r3)	
	evldd r1,SPE_OFFSET(1)(r3)
	evldd r2,SPE_OFFSET(2)(r3)
    evldd r4,SPE_OFFSET(4)(r3)
	evldd r5,SPE_OFFSET(5)(r3)
	evldd r6,SPE_OFFSET(6)(r3)
	evldd r7,SPE_OFFSET(7)(r3)
	evldd r8,SPE_OFFSET(8)(r3)
	evldd r9,SPE_OFFSET(9)(r3)
	evldd r10,SPE_OFFSET(10)(r3)
	evldd r11,SPE_OFFSET(11)(r3)
	evldd r12,SPE_OFFSET(12)(r3)
	evldd r13,SPE_OFFSET(13)(r3)
	evldd r14,SPE_OFFSET(14)(r3)
	evldd r15,SPE_OFFSET(15)(r3)
	evldd r16,SPE_OFFSET(16)(r3)
	evldd r17,SPE_OFFSET(17)(r3)
	evldd r18,SPE_OFFSET(18)(r3)
	evldd r19,SPE_OFFSET(19)(r3)
	evldd r20,SPE_OFFSET(20)(r3)
	evldd r21,SPE_OFFSET(21)(r3)
	evldd r22,SPE_OFFSET(22)(r3)
	evldd r23,SPE_OFFSET(23)(r3)
	evldd r24,SPE_OFFSET(24)(r3)
	evldd r25,SPE_OFFSET(25)(r3)
	evldd r26,SPE_OFFSET(26)(r3)
	evldd r27,SPE_OFFSET(27)(r3)
	evldd r28,SPE_OFFSET(28)(r3)
	evldd r29,SPE_OFFSET(29)(r3)
	evldd r30,SPE_OFFSET(30)(r3)
	evldd r31,SPE_OFFSET(31)(r3)
	evldd r3,SPE_OFFSET(3)(r3)
	
    /* restore 64-bit accumulator */
    evmra		r5 ,r5			/* acc( 0:63): ACC( 0:63) */
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
    mfmsr  r5      
    oris   r5, r5, HI(_PPC_MSR_SPE)   
    b 1f
    .balign 64
1:
    mtsrr1  r5    
    lis     r5, HI(2f)     
    ori     r5, r5, LO(2f)
    mtsrr0  r5 
    rfi
    2:
    
    //stwu    r3, -SPE_FRAME_SIZE(r3)
	subi   r3,r3,SPE_FRAME_SIZE 
    evstdd r0,SPE_OFFSET(0)(r3)	/* Store Upper word into context offset for r0 */
	evstdd r1,SPE_OFFSET(1)(r3)
	evstdd r2,SPE_OFFSET(2)(r3)
	evstdd r3,SPE_OFFSET(3)(r3)
	evstdd r4,SPE_OFFSET(4)(r3)
	evstdd r5,SPE_OFFSET(5)(r3)
	evstdd r6,SPE_OFFSET(6)(r3)
	evstdd r7,SPE_OFFSET(7)(r3)
	evstdd r8,SPE_OFFSET(8)(r3)
	evstdd r9,SPE_OFFSET(9)(r3)
	evstdd r10,SPE_OFFSET(10)(r3)
	evstdd r11,SPE_OFFSET(11)(r3)
	evstdd r12,SPE_OFFSET(12)(r3)
	evstdd r13,SPE_OFFSET(13)(r3)
	evstdd r14,SPE_OFFSET(14)(r3)
	evstdd r15,SPE_OFFSET(15)(r3)
	evstdd r16,SPE_OFFSET(16)(r3)
	evstdd r17,SPE_OFFSET(17)(r3)
	evstdd r18,SPE_OFFSET(18)(r3)
	evstdd r19,SPE_OFFSET(19)(r3)
	evstdd r20,SPE_OFFSET(20)(r3)
	evstdd r21,SPE_OFFSET(21)(r3)
	evstdd r22,SPE_OFFSET(22)(r3)
	evstdd r23,SPE_OFFSET(23)(r3)
	evstdd r24,SPE_OFFSET(24)(r3)
	evstdd r25,SPE_OFFSET(25)(r3)
	evstdd r26,SPE_OFFSET(26)(r3)
	evstdd r27,SPE_OFFSET(27)(r3)
	evstdd r28,SPE_OFFSET(28)(r3)
	evstdd r29,SPE_OFFSET(29)(r3)
	evstdd r30,SPE_OFFSET(30)(r3)
	evstdd r31,SPE_OFFSET(31)(r3)

	/* To avoid corruption of r5 this is done at the end */

    addi   r3, r3, SPE_OFFSET(32)
	evxor r5, r5, r5 
	evmwumiaa r5, r5, r5
	evstdd  r5,0(r3)	/* Offset 32 words into structure for acc */
    isync							/* CSI */
    blr
FUNC_END(speSave)


