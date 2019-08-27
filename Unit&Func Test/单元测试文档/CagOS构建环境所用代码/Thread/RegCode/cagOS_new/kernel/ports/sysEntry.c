/* sysEntry.c - sysOs kernel entry */

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

#include <kernel/sysOs.h>
#include <kernel/sysMap.h>
#include <ppc/ppc85xx.h>

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

OS_TCB        AppTaskStartTCB;
OS_TCB        MainTaskStartTCB;
OS_TCB        clockTaskStartTCB;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg);
extern  void  clockTaskStart (void *p_arg);
extern  void  sysSysEpicInit (void);
extern  void  main (void);

/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
* Arguments   : none.
*
* Returns     : none.
*********************************************************************************************************
*/
int sysOs(void)
{
    OS_ERR  os_err;

    OSInit(&os_err);                                            /* Initialize Micrium's uC/OS-III Kernel.               */

    unsigned long msr=0;
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    msr &= ~(_PPC_MSR_EE);
    msr &=~(_PPC_MSR_ME);
    msr |= _PPC_MSR_ME;
    __asm__ __volatile__ ("mtmsr %0"::"r" (msr));
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);

#if 0
    sysSysEpicInit();
    sysInitException();
    timers_init();
#endif

    OSTaskCreate(&clockTaskStartTCB,                              /* Create the clock interrupt Task                                */
                 "clock interrupt Task Start",
                 clockTaskStart,
                 0u,
                 CLOCK_CFG_TASK_START_PRIO,
                 (CPU_STK*)TASK_STACK_0,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 0u,
                 (void *) TASK_SPESTACK_0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);

    OSTaskCreate(&AppTaskStartTCB,                              /* Create the Start Task                                */
                 "App Task Start",
                 AppTaskStart,
                 0u,
                 APP_CFG_TASK_START_PRIO,
                 (CPU_STK*)TASK_STACK_1,
                 (TASK_STACK_SIZE / 10u),
                 TASK_STACK_SIZE,
                 0u,
                 0u,
                 (void *) TASK_SPESTACK_1,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);


    main ();
    OSStart(&os_err);                                           /* Start Multitasking (I.E. give control to uC/OS-III.  */

    while (DEF_ON) {
    };                                                          /* Should Never get here.                               */
}


/*
*********************************************************************************************************
*                                           App_TaskStart()
*
* Description : Startup task example code.
*
* Arguments   : p_arg       Argument passed by 'OSTaskCreate()'.
*
* Returns     : none.
*
* Created by  : main().
*
* Notes       : (1) The ticker MUST be initialised AFTER multitasking has started.
*********************************************************************************************************
*/

static void AppTaskStart (void *p_arg)
{
    OS_ERR             os_err;

    while (DEF_ON) {

        OSTimeDly((OS_TICK )30,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
    }
}
