/* sysTick.c - kernel/sysOs Tick handling facilities */

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

/*
************************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
************************************************************************************************************************
*/

static  CPU_TS  OS_TickListUpdateDly     (void);
static  CPU_TS  OS_TickListUpdateTimeout (void);
extern CPU_INT32U getTblms (void);

/*
************************************************************************************************************************
*                                                      TICK TASK
*
* Description: This task is internal to uC/OS-III and is triggered by the tick interrupt.
*
* Arguments  : p_arg     is an argument passed to the task when the task is created (unused).
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TickUpdate (void)
{
    CPU_TS  ts_delta;
    CPU_TS  ts_delta_dly;
    CPU_TS  ts_delta_timeout;

    OSTickCtr++;                                        /* Keep track of the number of ticks                    */
    ts_delta_dly     = OS_TickListUpdateDly();
    ts_delta_timeout = OS_TickListUpdateTimeout();
    ts_delta         = ts_delta_dly + ts_delta_timeout; /* Compute total execution time of list updates         */

    if (OSTickTaskTimeMax < ts_delta) {
        OSTickTaskTimeMax = ts_delta;
    }
}

/*
************************************************************************************************************************
*                                                      INSERT
*
* Description: This task is internal to uC/OS-III and allows the insertion of a task in a tick list.
*
* Arguments  : p_list      is a pointer to the desired list
*
*              p_tcb       is a pointer to the TCB to insert in the list
*
*              time        is the amount of time remaining (in ticks) for the task to become ready
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TickListInsert (OS_TICK_LIST  *p_list,
                         OS_TCB        *p_tcb,
                         OS_TICK        time)
{
    OS_TCB  *p_tcb1;
    OS_TCB  *p_tcb2;
    OS_TICK  remain;


    if (p_list->TCB_Ptr == (OS_TCB *)0) {                               /* Is the list empty?                                */
        p_tcb->TickRemain  = time;                                      /* Yes, Store time in TCB                            */
        p_tcb->TickNextPtr = (OS_TCB *)0;
        p_tcb->TickPrevPtr = (OS_TCB *)0;
        p_tcb->TickListPtr = (OS_TICK_LIST *)p_list;                    /*      Link to this list                            */
        p_list->TCB_Ptr    = p_tcb;                                     /*      Point to TCB of task to place in the list    */
    } else {
        p_tcb1 = p_list->TCB_Ptr;
        p_tcb2 = p_list->TCB_Ptr;                                       /* No,  Insert somewhere in the list in delta order  */
        remain = time;
        while (p_tcb2 != (OS_TCB *)0) {
            if (remain <= p_tcb2->TickRemain) {
                if (p_tcb2->TickPrevPtr == (OS_TCB *)0) {               /*      Insert before the first entry in the list?   */
                    p_tcb->TickRemain   = remain;                       /*      Yes, Store remaining time                    */
                    p_tcb->TickPrevPtr  = (OS_TCB *)0;
                    p_tcb->TickNextPtr  = p_tcb2;
                    p_tcb->TickListPtr  = (OS_TICK_LIST *)p_list;       /*           Link TCB to this list                   */
                    p_tcb2->TickRemain -= remain;                       /*           Reduce time of next entry in the list   */
                    p_tcb2->TickPrevPtr = p_tcb;
                    p_list->TCB_Ptr     = p_tcb;                        /*           Add TCB to the list                     */
                } else {                                                /*      No,  Insert somewhere further in the list    */
                    p_tcb1              = p_tcb2->TickPrevPtr;
                    p_tcb->TickRemain   = remain;                       /*           Store remaining time                    */
                    p_tcb->TickPrevPtr  = p_tcb1;
                    p_tcb->TickNextPtr  = p_tcb2;
                    p_tcb->TickListPtr  = (OS_TICK_LIST *)p_list;       /*           TCB points to this list                 */
                    p_tcb2->TickRemain -= remain;                       /*           Reduce time of next entry in the list   */
                    p_tcb2->TickPrevPtr = p_tcb;
                    p_tcb1->TickNextPtr = p_tcb;
                }
                return;
            } else {
                remain -= p_tcb2->TickRemain;                           /*           Point to the next TCB in the list       */
                p_tcb1  = p_tcb2;
                p_tcb2  = p_tcb2->TickNextPtr;
            }
        }
        p_tcb->TickRemain   = remain;
        p_tcb->TickPrevPtr  = p_tcb1;
        p_tcb->TickNextPtr  = (OS_TCB *)0;
        p_tcb->TickListPtr  = (OS_TICK_LIST *)p_list;                   /*           Link the list to the TCB                */
        p_tcb1->TickNextPtr = p_tcb;
    }
}

/*
************************************************************************************************************************
*                                            ADD TASK TO DELAYED TICK LIST
*
* Description: This function is called to place a task in a list of task waiting for either time to expire
*
* Arguments  : p_tcb          is a pointer to the OS_TCB of the task to add to the tick list
*              -----
*
*              time           represents either the 'match' value of OSTickCtr or a relative time from the current
*                             value of OSTickCtr as specified by the 'opt' argument..
*
*                             relative when 'opt' is set to OS_OPT_TIME_DLY
*                             relative when 'opt' is set to OS_OPT_TIME_TIMEOUT
*                             match    when 'opt' is set to OS_OPT_TIME_MATCH
*                             periodic when 'opt' is set to OS_OPT_TIME_PERIODIC
*
*              opt            is an option specifying how to calculate time.  The valid values are:
*              ---
*                                 OS_OPT_TIME_DLY
*                                 OS_OPT_TIME_TIMEOUT
*                                 OS_OPT_TIME_PERIODIC
*                                 OS_OPT_TIME_MATCH
*
*              p_err          is a pointer to a variable that will contain an error code returned by this function.
*              -----
*                                 OS_ERR_NONE           the call was successful and the time delay was scheduled.
*                                 OS_ERR_TIME_ZERO_DLY  if delay is zero or already occurred.
*
* Returns    : None
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/

void  OS_TickListInsertDly (OS_TCB   *p_tcb,
                            OS_TICK   time,
                            OS_OPT    opt,
                            OS_ERR   *p_err)
{
    OS_TICK   remain;

    if (opt == OS_OPT_TIME_MATCH) {                                     /* MATCH to absolute OSTickCtr value mode            */
        remain = time - OSTickCtr;
        if ((remain > OS_TICK_TH_RDY) ||                                /* If delay already occurred, ...                    */
            (remain == (OS_TICK)0u)) {
            p_tcb->TickRemain = (OS_TICK)0u;
            *p_err             =  OS_ERR_TIME_ZERO_DLY;                  /* ... do NOT delay.                                 */
            return;
        }

    } else if (opt == OS_OPT_TIME_PERIODIC) {                           /* PERIODIC mode.                                    */
        if ((OSTickCtr - p_tcb->TickCtrPrev) > time) {
            remain             = time;                                  /* ... first time we load .TickCtrPrev               */
            p_tcb->TickCtrPrev = OSTickCtr + time;
        } else {
            remain = time - (OSTickCtr - p_tcb->TickCtrPrev);
            if ((remain > OS_TICK_TH_RDY) ||                            /* If delay time has already passed, ...             */
                (remain == (OS_TICK)0u)) {
                p_tcb->TickCtrPrev += time + time * ((OSTickCtr - p_tcb->TickCtrPrev) / time); /* Try to recover the period  */
                p_tcb->TickRemain   = (OS_TICK)0u;
                *p_err               =  OS_ERR_TIME_ZERO_DLY;            /* ... do NOT delay.                                 */
                return;
            }
            p_tcb->TickCtrPrev += time;
        }

    } else if (time > (OS_TICK)0u) {                                    /* RELATIVE time delay mode                          */
        remain = time;

    } else {                                                            /* Zero time delay; ...                              */
        p_tcb->TickRemain = (OS_TICK)0u;
        *p_err             =  OS_ERR_TIME_ZERO_DLY;                      /* ... do NOT delay.                                 */
        return;
    }

    p_tcb->TaskState = OS_TASK_STATE_DLY;
    OS_TickListInsert(&OSTickListDly, p_tcb, remain);

    *p_err = OS_ERR_NONE;
}

/*
************************************************************************************************************************
*                                         REMOVE A TASK FROM THE TICK LIST
*
* Description: This function is called to remove a task from the tick list
*
* Arguments  : p_tcb          Is a pointer to the OS_TCB to remove.
*              -----
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/

void  OS_TickListRemove (OS_TCB  *p_tcb)
{
    OS_TICK_LIST  *p_list;
    OS_TCB        *p_tcb1;
    OS_TCB        *p_tcb2;


    p_list = (OS_TICK_LIST *)p_tcb->TickListPtr;
    p_tcb1  = p_tcb->TickPrevPtr;
    p_tcb2  = p_tcb->TickNextPtr;
    if (p_tcb1 == (OS_TCB *)0) {
        if (p_tcb2 == (OS_TCB *)0) {                                    /* Remove ONLY entry in the list?                    */
            p_list->TCB_Ptr    = (OS_TCB        *)0;
            p_tcb->TickRemain   = (OS_TICK       )0u;
            p_tcb->TickListPtr  = (OS_TICK_LIST *)0;
        } else {
            p_tcb2->TickPrevPtr = (OS_TCB       *)0;
            p_tcb2->TickRemain += p_tcb->TickRemain;                    /* Add back the ticks to the delta                   */
            p_list->TCB_Ptr    = p_tcb2;
            p_tcb->TickNextPtr  = (OS_TCB       *)0;
            p_tcb->TickRemain   = (OS_TICK       )0u;
            p_tcb->TickListPtr  = (OS_TICK_LIST *)0;
        }
    } else {
        p_tcb1->TickNextPtr = p_tcb2;
        if (p_tcb2 != (OS_TCB *)0) {
            p_tcb2->TickPrevPtr = p_tcb1;
            p_tcb2->TickRemain += p_tcb->TickRemain;                    /* Add back the ticks to the delta list              */
        }
        p_tcb->TickPrevPtr  = (OS_TCB       *)0;
        p_tcb->TickNextPtr  = (OS_TCB       *)0;
        p_tcb->TickRemain   = (OS_TICK       )0u;
        p_tcb->TickListPtr  = (OS_TICK_LIST *)0;
    }
}

/*
************************************************************************************************************************
*                                              RESET TICK LIST PEAK DETECTOR
*
* Description: This function is used to reset the peak detector for the number of entries in each spoke.
*
* Arguments  : void
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TickListResetPeak (void)
{
}

/*
************************************************************************************************************************
*                                           UPDATE THE LIST OF TASKS DELAYED
*
* Description: This function updates the delta list which contains tasks that have been delayed.
*
* Arguments  : non
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

static  CPU_TS  OS_TickListUpdateDly (void)
{
    OS_TCB       *p_tcb;
    OS_TICK_LIST *p_list;
    CPU_TS        ts_start;
    CPU_TS        ts_delta_dly;

    ts_start    = OS_TS_GET();
    p_list      = &OSTickListDly;
    p_tcb       = p_list->TCB_Ptr;
    if (p_tcb != (OS_TCB *)0) {
        p_tcb->TickRemain--;
        while (p_tcb->TickRemain == 0u) {
            if (p_tcb->TaskState == OS_TASK_STATE_DLY) {
                p_tcb->TaskState = OS_TASK_STATE_RDY;
                OS_RdyListInsert(p_tcb);                                /* Insert the task in the ready list                 */
            } else if (p_tcb->TaskState == OS_TASK_STATE_DLY_SUSPENDED) {
                p_tcb->TaskState = OS_TASK_STATE_SUSPENDED;
            }

            p_list->TCB_Ptr = p_tcb->TickNextPtr;
            p_tcb           = p_list->TCB_Ptr;                          /* Get 'p_tcb' again for loop                        */
            if (p_tcb == (OS_TCB *)0) {
                break;
            } else {
                p_tcb->TickPrevPtr = (OS_TCB *)0;
            }
        }
    }
    ts_delta_dly       = OS_TS_GET() - ts_start;                        /* Measure execution time of the update              */

    return (ts_delta_dly);
}


/*
************************************************************************************************************************
*                                       UPDATE THE LIST OF TASKS PENDING WITH TIMEOUT
*
* Description: This function updales the delta list which contains tasks that are pending with a timeout.
*
* Arguments  : non
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

static  CPU_TS  OS_TickListUpdateTimeout (void)
{
    OS_TCB       *p_tcb;
    OS_TICK_LIST *p_list;
    CPU_TS        ts_start;
    CPU_TS        ts_delta_timeout;
#if OS_CFG_MUTEX_EN > 0u
    OS_TCB       *p_tcb_owner;
    OS_PRIO       prio_new;
#endif

    ts_start    = OS_TS_GET();
    p_list      = &OSTickListTimeout;
    p_tcb       = p_list->TCB_Ptr;
    if (p_tcb != (OS_TCB *)0) {
        p_tcb->TickRemain--;
        while (p_tcb->TickRemain == 0u) {

#if OS_CFG_MUTEX_EN > 0u
            p_tcb_owner = (OS_TCB *)0;
            if (p_tcb->PendOn == OS_TASK_PEND_ON_MUTEX) {
                p_tcb_owner = ((OS_MUTEX *)p_tcb->PendDataTblPtr->PendObjPtr)->OwnerTCBPtr;
            }
#endif

#if (OS_MSG_EN > 0u)
            p_tcb->MsgPtr  = (void      *)0;
            p_tcb->MsgSize = (OS_MSG_SIZE)0u;
#endif
            p_tcb->TS      = OS_TS_GET();
            OS_PendListRemove(p_tcb);                                   /* Remove from wait list                             */
            if (p_tcb->TaskState == OS_TASK_STATE_PEND_TIMEOUT) {
                OS_RdyListInsert(p_tcb);                                /* Insert the task in the ready list                 */
                p_tcb->TaskState  = OS_TASK_STATE_RDY;
            } else if (p_tcb->TaskState == OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED) {

                p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;
            }
            p_tcb->PendStatus = OS_STATUS_PEND_TIMEOUT;                 /* Indicate pend timed out                           */
            p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;                /* Indicate no longer pending                        */

#if OS_CFG_MUTEX_EN > 0u
            if(p_tcb_owner != (OS_TCB *)0) {
                if ((p_tcb_owner->Prio != p_tcb_owner->BasePrio) &&
                    (p_tcb_owner->Prio == p_tcb->Prio)) {               /* Has the owner inherited a priority?               */
                    prio_new = OS_MutexGrpPrioFindHighest(p_tcb_owner);
                    prio_new = prio_new > p_tcb_owner->BasePrio ? p_tcb_owner->BasePrio : prio_new;
                    if(prio_new != p_tcb_owner->Prio) {
                        OS_TaskChangePrio(p_tcb_owner, prio_new);
                    }
                }
            }
#endif

            p_list->TCB_Ptr = p_tcb->TickNextPtr;
            p_tcb           = p_list->TCB_Ptr;                          /* Get 'p_tcb' again for loop                        */
            if (p_tcb == (OS_TCB *)0) {
                break;
            } else {
                p_tcb->TickPrevPtr = (OS_TCB *)0;
            }
        }
    }
    ts_delta_timeout   = OS_TS_GET() - ts_start;                        /* Measure execution time of the update              */

    return (ts_delta_timeout);
}
