/* sysOsSwitchALib.S - PowerPC Task Switch handling facilities */

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
 * This file contains
 */

#define _ASMLANGUAGE
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>
#include <ppc/ppc85xx.h>

/*
;********************************************************************************************************
;                                          PUBLIC FUNCTIONS
;********************************************************************************************************
*/

    DATA_IMPORT(OSRunning)
    DATA_IMPORT(OSPrioCur)
    DATA_IMPORT(OSPrioHighRdy)
    DATA_IMPORT(OSTCBCurPtr)
    DATA_IMPORT(OSTCBHighRdyPtr)

    FUNC_IMPORT(OSTaskSwHook)
    FUNC_EXPORT(OSStartHighRdy)
    FUNC_EXPORT(OSCtxSw)

/*
#-------------------------------------------------------------------------------
#  STACK FRAME DEFINITION 栈帧描述
#-------------------------------------------------------------------------------*/

    /*
    ;XR1 	.equ	0
	;XBLK	.equ	XR1+4
	;XR0	.equ	XBLK+4
	;XSRR0	.equ	XR0+4
	;XSRR1	.equ	XSRR0+4
	;XCTR	.equ	XSRR1+4
	;XXER	.equ	XCTR+4
	;XCR	.equ	XXER+4
	;XLR	.equ	XCR+4
	;XR2	.equ	XLR+4
	;XR3	.equ	XR2+4
	;XR4	.equ	XR3+4
	;XR5	.equ	XR4+4
	;XR6	.equ	XR5+4
	;XR7	.equ	XR6+4
	;XR8	.equ	XR7+4
	;XR9	.equ	XR8+4
	;XR10	.equ	XR9+4
	;XR11	.equ	XR10+4
	;XR12	.equ	XR11+4
	;XR13	.equ	XR12+4
	;XR14	.equ	XR13+4
	;XR15	.equ	XR14+4
	;XR16	.equ	XR15+4
	;XR17	.equ	XR16+4
	;XR18	.equ	XR17+4
	;XR19	.equ	XR18+4
	;XR20	.equ	XR19+4
	;XR21	.equ	XR20+4
	;XR22	.equ	XR21+4
	;XR23	.equ	XR22+4
        ;XR24	.equ	XR23+4
	;XR25	.equ	XR24+4
	;XR26	.equ	XR25+4
	;XR27	.equ	XR26+4
	;XR28	.equ	XR27+4
	;XR29	.equ	XR28+4
	;XR30	.equ	XR29+4
	;XR31	.equ	XR30+4
	;XMSR	.equ	XR31+4
	;STACK_FRAME_SIZE	.equ	XMSR+4
    */

/*
#********************************************************************************************************
#                                         START MULTITASKING
#                                      void OSStartHighRdy(void)
#
# Note(s) : 1) OSStartHighRdy() MUST:
#              a) Call OSTaskSwHook() then,
#              b) Set OSRunning to OS_STATE_OS_RUNNING,
#              c) Switch to the highest priority task.
#********************************************************************************************************
*/

FUNC_BEGIN(OSStartHighRdy)

    //bl	OSTaskSwHook

    lis r10,OSTCBHighRdyPtr@ha
    lwz r10,OSTCBHighRdyPtr@l(r10)
    lwz r1,0(r10)
    lwz	r2,XR2(r1)
    lwz	r3,XR3(r1)
    lwz	r4,XR4(r1)
    lwz	r5,XR5(r1)
    lwz	r6,XR6(r1)
    lwz	r7,XR7(r1)
    lwz	r8,XR8(r1)
    lwz	r9,XR9(r1)
    lwz	r10,XR10(r1)
    lwz	r11,XR11(r1)
    lwz	r12,XR12(r1)
    lwz	r13,XR13(r1)
    lwz	r14,XR14(r1)
    lwz	r15,XR15(r1)
    lwz	r16,XR16(r1)
    lwz	r17,XR17(r1)
    lwz	r18,XR18(r1)
    lwz	r19,XR19(r1)
    lwz	r20,XR20(r1)
    lwz	r21,XR21(r1)
    lwz	r22,XR22(r1)
    lwz	r23,XR23(r1)
    lwz	r24,XR24(r1)
    lwz	r25,XR25(r1)
    lwz	r26,XR26(r1)
    lwz	r27,XR27(r1)
    lwz	r28,XR28(r1)
    lwz	r29,XR29(r1)
    lwz r30,XR30(r1)
    lwz	r31,XR31(r1)
    lwz	r0,XLR(r1)
    mtlr    r0
    lwz	    r0,XCR(r1)
    mtcrf   255,r0
    lwz	    r0,XXER(r1)
    mtxer   r0
    lwz	    r0,XCTR(r1)
    mtctr   r0
    lwz	    r0,XSRR1(r1)
    mtspr   SRR1,r0
    lwz     r0,XSRR0(r1)
    mtspr   SRR0,r0
    lwz     r0,XR0(r1)
    addi    r1,r1, EXC_FRAME_SIZE
    SYNC
    bl os_gap_check
    rfi
FUNC_END(OSStartHighRdy)

/*
;********************************************************************************************************
;                       PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
;
; Note(s) : 1) OSCtxSw() is called in SVC mode with BOTH FIQ and IRQ interrupts DISABLED.
;
;           2) The pseudo-code for OSCtxSw() is:
;              a) Save the current task's context onto the current task's stack,
;              b) OSTCBCurPtr->StkPtr = SP;
;              c) OSTaskSwHook();
;              d) OSPrioCur           = OSPrioHighRdy;
;              e) OSTCBCurPtr         = OSTCBHighRdyPtr;
;              f) SP                  = OSTCBHighRdyPtr->StkPtr;
;              g) Restore the new task's context from the new task's stack,
;              h) Return to new task's code.
;
;           3) Upon entry:
;              OSTCBCurPtr      points to the OS_TCB of the task to suspend,
;              OSTCBHighRdyPtr  points to the OS_TCB of the task to resume.
;********************************************************************************************************
*/
FUNC_BEGIN(OSCtxSw)

    stwu	r1,-EXC_FRAME_SIZE(r1)
    stw	    r0,XR0(r1)
    mflr    r0

    stw	    r0,XSRR0(r1)
    mfmsr	r0
    stw	    r0,XSRR1(r1)
    mfctr   r0
    stw	    r0,XCTR(r1)
    mfxer   r0
    stw	    r0,XXER(r1)
    mfcr  	r0
    stw	    r0,XCR(r1)
    stw	    r2,XR2(r1)
    stw	    r3,XR3(r1)
    stw	    r4,XR4(r1)
    stw	    r5,XR5(r1)
    stw	    r6,XR6(r1)
    stw	    r7,XR7(r1)
    stw	    r8,XR8(r1)
    stw	    r9,XR9(r1)
    stw	    r10,XR10(r1)
    stw	    r11,XR11(r1)
    stw	    r12,XR12(r1)
    stw     r14,XR14(r1)
    stw     r15,XR15(r1)
    stw     r16,XR16(r1)
    stw     r17,XR17(r1)
    stw     r18,XR18(r1)
    stw     r19,XR19(r1)
    stw     r20,XR20(r1)
    stw     r21,XR21(r1)
    stw     r22,XR22(r1)
    stw     r23,XR23(r1)
    stw     r24,XR24(r1)
    stw     r25,XR25(r1)
    stw     r26,XR26(r1)
    stw     r27,XR27(r1)
    stw     r28,XR28(r1)
    stw     r29,XR29(r1)
    stw     r30,XR30(r1)
    stw     r31,XR31(r1)

    xor     r0,r0,r0
    addis   r10,r0,OSTCBCurPtr@ha
    lwz r7,OSTCBCurPtr@l(r10)
    stw r1,0(r7)
    
    lis     r10,OSTCBHighRdyPtr@ha
    lwz     r7,OSTCBHighRdyPtr@l(r10)
    lwz r3,4(r7)
    bl speRestore

    //bl  OSTaskSwHook 
    xor     r0,r0,r0
    addis   r10,r0,OSTCBHighRdyPtr@ha
    lwz r7,OSTCBHighRdyPtr@l(r10)

    xor r0,r0,r0
    addis   r10,r0,OSTCBCurPtr@ha
    stw r7,OSTCBCurPtr@l(r10)

    lis	r9,OSPrioHighRdy@ha
    lwz	r9,OSPrioHighRdy@l(r9)
 
    lis	r10,OSPrioCur@ha
    stw r9,OSPrioCur@l(r10)

    lwz r1,0(r7)
    lwz r2,XR2(r1)
    lwz r3,XR3(r1)
    lwz r4,XR4(r1)
    lwz r5,XR5(r1)
    lwz r6,XR6(r1)
    lwz r7,XR7(r1)
    lwz r8,XR8(r1)
    lwz r9,XR9(r1)
    lwz r10,XR10(r1)
    lwz r11,XR11(r1)
    lwz r12,XR12(r1)
    lwz r13,XR13(r1)
    lwz r14,XR14(r1)
    lwz r15,XR15(r1)
    lwz r16,XR16(r1)
    lwz r17,XR17(r1)
    lwz r18,XR18(r1)
    lwz r19,XR19(r1)
    lwz r20,XR20(r1)
    lwz r21,XR21(r1)
    lwz r22,XR22(r1)
    lwz r23,XR23(r1)
    lwz r24,XR24(r1)
    lwz r25,XR25(r1)
    lwz r26,XR26(r1)
    lwz r27,XR27(r1)
    lwz r28,XR28(r1)
    lwz r29,XR29(r1)
    lwz r30,XR30(r1)
    lwz r31,XR31(r1)
    lwz	  r0,XLR(r1)
    mtlr  r0
    lwz	  r0,XCR(r1)
    mtcrf 255,r0
    lwz	  r0,XXER(r1)
    mtxer r0
    lwz	  r0,XCTR(r1)
    mtctr r0
    lwz   r0,XSRR1(r1)
    mtspr SRR1,r0
    lwz   r0,XSRR0(r1)
    mtspr SRR0,r0
    lwz	  r0,XR0(r1)
    addi  r1,r1,EXC_FRAME_SIZE
    SYNC
    rfi
FUNC_END(OSCtxSw)
