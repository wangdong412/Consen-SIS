/* sysOsSwitchLib.c - PowerPC task Switch handling facilities */

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

#define   OS_CPU_GLOBALS

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <config.h>
#include <libc/stdio.h>
#include <ppc/ppc85xx.h>
#include <ppc/toolPpc.h>
#include <ppc/excPpcLib.h>
#include <ppc/spePpcLib.h>
#include <kernel/sysOs.h>

#ifdef __cplusplus
extern  "C" {
#endif

extern char _SDA_BASE_[];
extern char _SDA2_BASE_[];
extern CPU_INT32U getTblms (void);

#define STACK_BUFFER_SIZE   0xA0


void powerpcIntDisable(void)
{
    CPU_INT32U msr=0;
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    msr &= ~(_PPC_MSR_EE);
    __asm__ __volatile__ ("mtmsr %0"::"r" (msr));
    __asm__ __volatile__ ("isync");
}

void powerpcIntEnable(void)
{
    CPU_INT32U msr=0;
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    msr &= ~(_PPC_MSR_EE);
    msr |= _PPC_MSR_EE;
    __asm__ __volatile__ ("mtmsr %0"::"r" (msr));
    __asm__ __volatile__ ("isync");
}

/*
*********************************************************************************************************
*                                           IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSIdleTaskHook (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppIdleTaskHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppIdleTaskHookPtr)();
    }
#endif
}

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSInitHook (void)
{
}

/*
*********************************************************************************************************
*                                         STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-III's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSStatTaskHook (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppStatTaskHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppStatTaskHookPtr)();
    }
#endif
}


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : p_tcb        Pointer to the task control block of the task being created.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskCreateHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskCreateHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskCreateHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                            /* Prevent compiler warning                               */
#endif
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : p_tcb        Pointer to the task control block of the task being deleted.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskDelHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskDelHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskDelHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                            /* Prevent compiler warning                               */
#endif
}


/*
*********************************************************************************************************
*                                            TASK RETURN HOOK
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : p_tcb        Pointer to the task control block of the task that is returning.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  OSTaskReturnHook (OS_TCB  *p_tcb)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskReturnHookPtr != (OS_APP_HOOK_TCB)0) {
        (*OS_AppTaskReturnHookPtr)(p_tcb);
    }
#else
    (void)p_tcb;                                            /* Prevent compiler warning                               */
#endif
}


/*
**********************************************************************************************************
*                                       INITIALIZE A TASK'S STACK
*
* Description: This function is called by OS_Task_Create() or OSTaskCreateExt() to initialize the stack
*              frame of the task being created. This function is highly processor specific.
*
* Arguments  : p_task       Pointer to the task entry point address.
*
*              p_arg        Pointer to a user supplied data area that will be passed to the task
*                               when the task first executes.
*
*              p_stk_base   Pointer to the base address of the stack.
*
*              stk_size     Size of the stack, in number of CPU_STK elements.
*
*              opt          Options used to alter the behavior of OS_Task_StkInit().
*                            (see OS.H for OS_TASK_OPT_xxx).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : 1) Interrupts are enabled when task starts executing.
*
*              2) All tasks run in SVC mode.
**********************************************************************************************************
*/

CPU_STK  *OSTaskStkInit (OS_TASK_PTR    p_task,
                         void          *p_arg,
                         CPU_STK       *p_stk_base,
                         CPU_STK       *p_stk_limit,
                         CPU_STK_SIZE   stk_size,
                         OS_OPT         opt)
{
    CPU_STK    *p_stk;
    (void)opt; /* Prevent compiler warning*/

    /* 8-byte align task's stack pointer (EABI) */
    p_stk = (CPU_STK *)((CPU_STK)p_stk_base & ~(CPU_CFG_STK_ALIGN_BYTES - 1u)); /* Align stack pointer.                  */

    /* Leave buffer area for locals "above the stack" */
    /* in case the compiler prolog puts variables above the stack */
    //p_stk -= STACK_BUFFER_SIZE;

    CPU_INT32U msr=0;

    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    msr &= ~(_PPC_MSR_EE);
    msr |= (_PPC_MSR_EE);
    msr &=~(_PPC_MSR_PR);

    msr &= ~(_PPC_MSR_ME);
    msr |= (_PPC_MSR_ME);
    __asm__ __volatile__ ("mtmsr %0"::"r" (msr));

    /*
     * Ensure any changes to the order of these initializations
     * are congruent with the Stack Frame definitions in bseppca.s
     */

    *--p_stk = (CPU_INT32U)msr;   		 /* MSR */
    *--p_stk = 0L;                   /* r31 */
    *--p_stk = 0L;                   /* r30 */
    *--p_stk = 0L;                   /* r29 */
    *--p_stk = 0L;                   /* r28 */
    *--p_stk = 0L;                   /* r27 */
    *--p_stk = 0L;                   /* r26 */
    *--p_stk = 0L;                   /* r25 */
    *--p_stk = 0L;                   /* r24 */
    *--p_stk = 0L;                   /* r23 */
    *--p_stk = 0L;                   /* r22 */
    *--p_stk = 0L;                   /* r21 */
    *--p_stk = 0L;                   /* r20 */
    *--p_stk = 0L;                   /* r19 */
    *--p_stk = 0L;                   /* r18 */
    *--p_stk = 0L;                   /* r17 */
    *--p_stk = 0L;                   /* r16 */
    *--p_stk = 0L;                   /* r15 */
    *--p_stk = 0L;                   /* r14 */
    *--p_stk = (CPU_INT32U)&_SDA_BASE_;  /* r13 */
    *--p_stk = 0L;                   /* r12 */
    *--p_stk = 0L;                   /* r11 */
    *--p_stk = 0L;                   /* r10 */
    *--p_stk = 0L;                   /* r09 */
    *--p_stk = 0L;                   /* r08 */
    *--p_stk = 0L;                   /* r07 */
    *--p_stk = 0L;                   /* r06 */
    *--p_stk = 0L;                   /* r05 */
    *--p_stk = 0L;                   /* r04 */
    *--p_stk = (CPU_STK)p_arg;       /* r03 */
    *--p_stk = (CPU_INT32U)&_SDA2_BASE_; /* r02 */
    *--p_stk = (CPU_INT32U)OS_TaskReturn; /* LR */
    --p_stk;                         /* CR */
    --p_stk;                         /* XER */
    --p_stk;                         /* CTR */
    *--p_stk = (CPU_INT32U)msr;      /* SRR1 */
    *--p_stk = (CPU_INT32U)p_task;   /* SRR0 */
    --p_stk;                         /* Reg R0 : argument   */
    --p_stk;                         /* BLANK for 0xA0 size */
    *--p_stk = (CPU_INT32U)p_stk_base;   /* Stack Ptr */

    return (p_stk);
}


/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : None.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdyPtr' points to the TCB of the task
*                 that will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCurPtr' points
*                 to the task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

void  OSTaskSwHook (void)
{
#if OS_CFG_TASK_PROFILE_EN > 0u
    CPU_TS  ts;
#endif

#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTaskSwHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppTaskSwHookPtr)();
    }
#endif

#if defined(_CAG_SPE_SUPPORT)
    SPE_CONTEXT * pSpe;
    if ((OSTCBCurPtr->Opt & OS_OPT_TASK_SAVE_FP) != (OS_OPT)0) {
        pSpe = SPE_CONTEXT_GET(OSTCBCurPtr);
        speSave (pSpe);
    }

    if ((OSTCBHighRdyPtr->Opt & OS_OPT_TASK_SAVE_FP) != (OS_OPT)0) {
        pSpe = SPE_CONTEXT_GET(OSTCBHighRdyPtr);
        speRestore (pSpe);
    }
#endif

#if OS_CFG_TASK_PROFILE_EN > 0u
    ts = OS_TS_GET();
    if (OSTCBCurPtr != OSTCBHighRdyPtr) {
        OSTCBCurPtr->CyclesDelta  = ts - OSTCBCurPtr->CyclesStart;
        OSTCBCurPtr->CyclesTotal += (OS_CYCLES)OSTCBCurPtr->CyclesDelta;
    }

    OSTCBHighRdyPtr->CyclesStart = ts;
#endif
}

/*
*********************************************************************************************************
*                                              TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : None.
*
* Note(s)    : 1) This function is assumed to be called from the Tick ISR.
*********************************************************************************************************
*/

void  OSTimeTickHook (void)
{
#if OS_CFG_APP_HOOKS_EN > 0u
    if (OS_AppTimeTickHookPtr != (OS_APP_HOOK_VOID)0) {
        (*OS_AppTimeTickHookPtr)();
    }
#endif
}

#ifdef __cplusplus
}
#endif
