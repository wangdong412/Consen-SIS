/* sysTime.c - kernel/sysOs Time handling facilities */

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

#define  MICRIUM_SOURCE
#include <kernel/sysOs.h>

extern void  OS_TickUpdate (void);

/*
************************************************************************************************************************
*                                                  DELAY TASK 'n' TICKS
*
* Description: This function is called to delay execution of the currently running task until the specified number of
*              system ticks expires.  This, of course, directly equates to delaying the current task for some time to
*              expire.  No delay will result if the specified delay is 0.  If the specified delay is greater than 0
*              then, a context switch will result.
*
* Arguments  : dly       is a value in 'clock ticks' that the task will either delay for or, the target match value
*                        of the tick counter (OSTickCtr).  Note that specifying 0 means the task is not to delay.
*
*                        depending on the option argument, the task will wake up when OSTickCtr reaches:
*
*                            OS_OPT_TIME_DLY      : OSTickCtr + dly
*                            OS_OPT_TIME_TIMEOUT  : OSTickCtr + dly
*                            OS_OPT_TIME_MATCH    : dly
*                            OS_OPT_TIME_PERIODIC : OSTCBCurPtr.TickCtrPrev + dly
*
*              opt       specifies whether 'dly' represents absolute or relative time; default option marked with *** :
*
*                        *** OS_OPT_TIME_DLY        specifies a relative time from the current value of OSTickCtr.
*                            OS_OPT_TIME_TIMEOUT    same as OS_OPT_TIME_DLY.
*                            OS_OPT_TIME_MATCH      indicates that 'dly' specifies the absolute value that OSTickCtr
*                                                   must reach before the task will be resumed.
*                            OS_OPT_TIME_PERIODIC   indicates that 'dly' specifies the periodic value that OSTickCtr
*                                                   must reach before the task will be resumed.
*
*              p_err     is a pointer to a variable that will contain an error code from this call.
*
*                            OS_ERR_NONE            the call was successful and the delay occurred.
*                            OS_ERR_OPT_INVALID     if you specified an invalid option for this function.
*                            OS_ERR_SCHED_LOCKED    can't delay when the scheduler is locked.
*                            OS_ERR_TIME_DLY_ISR    if you called this function from an ISR.
*                            OS_ERR_TIME_ZERO_DLY   if you specified a delay of zero.
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeDly (OS_TICK   dly,
                 OS_OPT    opt,
                 OS_ERR   *p_err)
{
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_TIME_DLY_ISR;
        return;
    }
#endif

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0u) {       /* Can't delay when the scheduler is locked               */
        *p_err = OS_ERR_SCHED_LOCKED;
        return;
    }

    switch (opt) {
    case OS_OPT_TIME_DLY:
    case OS_OPT_TIME_TIMEOUT:
    case OS_OPT_TIME_PERIODIC:
        if (dly == (OS_TICK)0u) {                      /* 0 means no delay!                                      */
            *p_err = OS_ERR_TIME_ZERO_DLY;
            return;
        }
        break;

    case OS_OPT_TIME_MATCH:
        break;

    default:
        *p_err = OS_ERR_OPT_INVALID;
        return;
    }

    OS_CRITICAL_ENTER();
    OS_TickListInsertDly(OSTCBCurPtr,
                         dly,
                         opt,
                         p_err);
    if (*p_err != OS_ERR_NONE) {
        OS_CRITICAL_EXIT_NO_SCHED();
        return;
    }
    OS_RdyListRemove(OSTCBCurPtr);                          /* Remove current task from ready list                    */
    OS_CRITICAL_EXIT_NO_SCHED();
    OSSched();                                              /* Find next task to run!                                 */
    *p_err = OS_ERR_NONE;
}

/*
************************************************************************************************************************
*                                                RESUME A DELAYED TASK
*
* Description: This function is used resume a task that has been delayed through a call to either OSTimeDly() or
*              OSTimeDlyHMSM().  Note that you cannot call this function to resume a task that is waiting for an event
*              with timeout.
*
* Arguments  : p_tcb    is a pointer to the TCB of the task to resume.
*
*              p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE                  Task has been resumed
*                           OS_ERR_STATE_INVALID         Task is in an invalid state
*                           OS_ERR_TIME_DLY_RESUME_ISR   If called from an ISR
*                           OS_ERR_TIME_NOT_DLY          Task is not waiting for time to expire
*                           OS_ERR_TASK_SUSPENDED        Task cannot be resumed, it was suspended by OSTaskSuspend()
*
* Note(s)    : none
************************************************************************************************************************
*/

#if OS_CFG_TIME_DLY_RESUME_EN > 0u
void  OSTimeDlyResume (OS_TCB  *p_tcb,
                       OS_ERR  *p_err)
{

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_TIME_DLY_RESUME_ISR;
        return;
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u
    if (p_tcb == (OS_TCB *)0) {                             /* Not possible for the running task to be delayed!       */
        *p_err = OS_ERR_TASK_NOT_DLY;
        return;
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == OSTCBCurPtr) {                             /* Not possible for the running task to be delayed!       */
        *p_err = OS_ERR_TASK_NOT_DLY;
        CPU_CRITICAL_EXIT();
        return;
    }

    switch (p_tcb->TaskState) {
    case OS_TASK_STATE_RDY:                             /* Cannot Abort delay if task is ready                    */
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    case OS_TASK_STATE_DLY:
        OS_CRITICAL_ENTER_CPU_EXIT();
        p_tcb->TaskState = OS_TASK_STATE_RDY;
        OS_TickListRemove(p_tcb);                      /* Remove task from tick list                             */
        OS_RdyListInsert(p_tcb);                       /* Add to ready list                                      */
        OS_CRITICAL_EXIT_NO_SCHED();
        *p_err = OS_ERR_NONE;
        break;

    case OS_TASK_STATE_PEND:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    case OS_TASK_STATE_PEND_TIMEOUT:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    case OS_TASK_STATE_SUSPENDED:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    case OS_TASK_STATE_DLY_SUSPENDED:
        OS_CRITICAL_ENTER_CPU_EXIT();
        p_tcb->TaskState = OS_TASK_STATE_SUSPENDED;
        OS_TickListRemove(p_tcb);                      /* Remove task from tick list                             */
        OS_CRITICAL_EXIT_NO_SCHED();
        *p_err            = OS_ERR_TASK_SUSPENDED;
        break;

    case OS_TASK_STATE_PEND_SUSPENDED:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_TASK_NOT_DLY;
        break;

    default:
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_STATE_INVALID;
        break;
    }

    OSSched();
}
#endif

/*
************************************************************************************************************************
*                                               GET CURRENT SYSTEM TIME
*
* Description: This function is used by your application to obtain the current value of the counter which keeps track of
*              the number of clock ticks.
*
* Arguments  : p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE           If the call was successful
*
* Returns    : The current value of OSTickCtr
************************************************************************************************************************
*/

OS_TICK  OSTimeGet (OS_ERR  *p_err)
{
    OS_TICK  ticks;

    CPU_CRITICAL_ENTER();
    ticks = OSTickCtr;
    CPU_CRITICAL_EXIT();
    *p_err = OS_ERR_NONE;
    return (ticks);
}

/*
************************************************************************************************************************
*                                                   SET SYSTEM CLOCK
*
* Description: This function sets the counter which keeps track of the number of clock ticks.
*
* Arguments  : ticks    is the desired tick value
*
*              p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE           If the call was successful
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeSet (OS_TICK   ticks,
                 OS_ERR   *p_err)
{
    CPU_CRITICAL_ENTER();
    OSTickCtr = ticks;
    CPU_CRITICAL_EXIT();
    *p_err     = OS_ERR_NONE;
}


/*
************************************************************************************************************************
*                                                 PROCESS SYSTEM TICK
*
* Description: This function is used to signal to uC/OS-III the occurrence of a 'system tick' (also known as a
*              'clock tick').  This function should be called by the tick ISR.
*
* Arguments  : none
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeTick (void)
{
    OSTimeTickHook();                                       /* Call user definable hook                               */
    OS_TickUpdate();

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
    OS_SchedRoundRobin(&OSRdyList[OSPrioCur]);
#endif
}
