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

#include <config.h>
#include <kernel/sysOs.h>
#include <kernel/sysMap.h>
#include <ppc/ppc85xx.h>
#include <libc/stdio.h>
#include <kernel/sysPic.h>
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
extern  void  sysEpicInit (void);
extern  void  main (void);

/*record boot stage*/
int booting_stage = 0;
void sysBootstage(int stage)
{
    booting_stage = stage;
    printf("bs %d\n",booting_stage);
}

#if 0
void __attribute__((weak)) main(void)
{
    printf("main() : call rts \n");
}
#endif

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
    printf("tstA-6\n");
    printf("stack from %x to %x\n",KERNEL_STACK_LOW,IRQ_STACK_HIGH);
    memset(KERNEL_STACK_HIGH,0xbb,IRQ_STACK_HIGH-KERNEL_STACK_LOW);
    os_gap_fill();
    os_gap_check();
    OS_ERR  os_err;
    OSInit(&os_err);                                            /* Initialize Micrium's uC/OS-III Kernel.               */
    os_gap_check();

    printf("tst0.1\n");
    UINT32 msr=0;
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    msr &= ~(_PPC_MSR_EE);
    msr &=~(_PPC_MSR_ME);
    msr |= _PPC_MSR_ME;
    __asm__ __volatile__ ("mtmsr %0"::"r" (msr));
    __asm__ __volatile__ ("mfmsr %0":"=r" (msr):);
    printf("tst1\n");

    OSTaskCreate(&clockTaskStartTCB,   /* Create the clock interrupt Task*/
                 "clock interrupt Task Start",
                 clockTaskStart,
                 0u,
                 CLOCK_CFG_TASK_START_PRIO,
                 (CPU_STK*)TASK_CLK_STACK_HIGH,
                 (STASK_SIZE /10u),
                 STASK_SIZE,
                 0u,
                 0u,
                 (UINT8 *)(TASK_SPESTACK_HIGH-4*(TASK_SPESTACK_SIZE+GAP_SIZE)),
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);

    printf("main() start\n");
    main();
    os_gap_check();
    printf("main() done\n");

#if _CAG_CONFIG_EPIC == 1
    powerpcIntDisable();
    sysEpicInit();
    powerpcIntEnable();
#endif
    OSStart(&os_err);                                           /* Start Multitasking (I.E. give control to uC/OS-III.  */
    while (DEF_ON) {
    };                                                          /* Should Never get here.                               */
}

/*
DESCRIPTION
This library provides routines to perform a number of atomic operations
on a memory location: add, subtract, increment, decrement, bitwise OR,
bitwise NOR, bitwise AND, bitwise NAND, set, clear and compare-and-swap.
*/

UINT32 atomic32Set (UINT32 * target, UINT32 value)
{
    UINT32 oldValue;
    oldValue = *(volatile UINT32 *)target;
    *(volatile UINT32 *)target = value;
    return (oldValue);
}

UINT32 atomic32Cas (UINT32 * target, UINT32 oldValue, UINT32 newValue)
{
    int swapped = FALSE;
    if (*(volatile UINT32 *)target == oldValue) {
        *(volatile UINT32 *)target = newValue;
        swapped = TRUE;
    }
    return (swapped);
}

UINT32 atomic32Get (UINT32 * target)
{
    /* This assumes UINT32 type can be read atomically on the running CPU */
    return *(volatile UINT32 *)target;
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

        OSTimeDly((OS_TICK )6000,                                  /* Synchronize with clock tick                            */
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&os_err);
    }
}
