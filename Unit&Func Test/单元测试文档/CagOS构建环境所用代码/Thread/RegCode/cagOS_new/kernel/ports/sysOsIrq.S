/* sysOsIrq.S - PowerPC interrupt handling facilities */

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


/*
;********************************************************************************************************
;                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
;
; Note(s) : 1) OSIntCtxSw() is called in SVC mode with BOTH FIQ and IRQ interrupts DISABLED.
;
;           2) The pseudo-code for OSCtxSw() is:
;              a) OSTaskSwHook();
;              b) OSPrioCur   = OSPrioHighRdy;
;              c) OSTCBCurPtr = OSTCBHighRdyPtr;
;              d) SP          = OSTCBHighRdyPtr->OSTCBStkPtr;
;              e) Restore the new task's context from the new task's stack,
;              f) Return to new task's code.
;
;           3) Upon entry:
;              OSTCBCurPtr      points to the OS_TCB of the task to suspend,
;              OSTCBHighRdyPtr  points to the OS_TCB of the task to resume.
;********************************************************************************************************
*/


#define _ASMLANGUAGE
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>
#include <ppc/asmPpc.h>
#include <ppc/excPpcLib.h>

    FUNC_EXPORT(OSIntCtxSw)
    FUNC_IMPORT(OSCtxSw)
    DATA_IMPORT(OSTCBCurPtr)

/*
;---------------------------------------
;   r31     高地址
;   r4
;   r3
;   r2
;   LR 保存连接返回地址，函数调用的返回的下一条执行的指令，如task1线程里，有A函数，A函数调用B函数，
;   此时B函数里发生了中断，此时LR就为A调用B，B函数的返回地址
;   XCR
;   XER
;   CTR
;   SRR1     保存被中断打断的机器状态字
;   SRR0     保存被中断打断的下一条指令的地址
;   R0
;   R1<---sp栈指针r1指向位置
;---------------------------------------
;*/

FUNC_BEGIN(OSIntCtxSw)
    lis     r10,OSTCBCurPtr@ha
    lwz     r7,OSTCBCurPtr@l(r10)
    lwz     r1,0(r7)
    bl      OSCtxSw
    lwz     r2,XR2(r1)
    lwz     r3,XR3(r1)
    lwz     r4,XR4(r1)
    lwz     r5,XR5(r1)
    lwz     r6,XR6(r1)
    lwz     r7,XR7(r1)
    lwz     r8,XR8(r1)
    lwz     r9,XR9(r1)
    lwz     r10,XR10(r1)
    lwz     r11,XR11(r1)
    lwz     r12,XR12(r1)
    lwz     r13,XR13(r1)
    lwz     r14,XR14(r1)
    lwz     r15,XR15(r1)
    lwz     r16,XR16(r1)
    lwz     r17,XR17(r1)
    lwz     r18,XR18(r1)
    lwz     r19,XR19(r1)
    lwz     r20,XR20(r1)
    lwz     r21,XR21(r1)
    lwz     r22,XR22(r1)
    lwz     r23,XR23(r1)
    lwz     r24,XR24(r1)
    lwz     r25,XR25(r1)
    lwz     r26,XR26(r1)
    lwz     r27,XR27(r1)
    lwz     r28,XR28(r1)
    lwz     r29,XR29(r1)
    lwz     r30,XR30(r1)
    lwz     r31,XR31(r1)
    lwz	    r0,XLR(r1)
    mtlr    r0
    lwz	    r0,XCR(r1)
    mtcrf   255,r0
    lwz	    r0,XXER(r1)
    mtxer   r0
    lwz	    r0,XCTR(r1)
    mtctr   r0
    lwz     r0,XSRR1(r1)
    mtspr   SRR1,r0
    lwz     r0,XSRR0(r1)
    mtspr   SRR0,r0
    lwz     r0,XMSR(r1)
    mtmsr   r0
    lwz	    r0,XR0(r1)
    addi    r1,r1,0xa0
    SYNC
    rfi
FUNC_END(OSIntCtxSw)
