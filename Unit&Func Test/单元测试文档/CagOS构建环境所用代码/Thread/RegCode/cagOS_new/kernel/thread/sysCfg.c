/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  OS configure code.
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
 * This file contains OS CONFIGURATION INITIALIZATION
 */

#define  MICRIUM_SOURCE
#include <kernel/sysOs.h>
#include <kernel/sysMap.h>

#define  OS_CFG_IDLE_TASK_STK_LIMIT      ((TASK_STACK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#define  OS_CFG_STAT_TASK_STK_LIMIT      ((TASK_STACK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#define  OS_CFG_TICK_TASK_STK_LIMIT      ((TASK_STACK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)
#define  OS_CFG_STAT_TASK_RATE_HZ       10u               /* Rate of execution (1 to 10 Hz)                         */
#define  OS_CFG_TICK_RATE_HZ            1000u               /* Tick rate in Hertz (10 to 1000 Hz)                     */

/*
************************************************************************************************************************
*                                                    DATA STORAGE
************************************************************************************************************************
*/

CPU_STK        OSCfg_IdleTaskStk   [TASK_STACK_SIZE];

#if (OS_MSG_EN > 0u)
OS_MSG         OSCfg_MsgPool       [OS_CFG_MSG_POOL_SIZE];
#endif

/*
************************************************************************************************************************
*                                                      CONSTANTS
************************************************************************************************************************
*/

CPU_STK      * const  OSCfg_IdleTaskStkBasePtr   = (CPU_STK    *)TASK_STACK_31;
CPU_STK_SIZE   const  OSCfg_IdleTaskStkLimit     = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_STK_LIMIT;
CPU_STK_SIZE   const  OSCfg_IdleTaskStkSize      = (CPU_STK_SIZE)TASK_STACK_SIZE;
CPU_INT32U     const  OSCfg_IdleTaskStkSizeRAM   = (CPU_INT32U  )sizeof(OSCfg_IdleTaskStk);

OS_OBJ_QTY     const  OSCfg_IntQSize             = (OS_OBJ_QTY  )0;
CPU_INT32U     const  OSCfg_IntQSizeRAM          = (CPU_INT32U  )0;
CPU_STK      * const  OSCfg_IntQTaskStkBasePtr   = (CPU_STK    *)0;
CPU_STK_SIZE   const  OSCfg_IntQTaskStkLimit     = (CPU_STK_SIZE)0;
CPU_STK_SIZE   const  OSCfg_IntQTaskStkSize      = (CPU_STK_SIZE)0;
CPU_INT32U     const  OSCfg_IntQTaskStkSizeRAM   = (CPU_INT32U  )0;

#if (OS_MSG_EN > 0u)
OS_MSG_SIZE    const  OSCfg_MsgPoolSize          = (OS_MSG_SIZE)OS_CFG_MSG_POOL_SIZE;
CPU_INT32U     const  OSCfg_MsgPoolSizeRAM       = (CPU_INT32U )sizeof(OSCfg_MsgPool);
OS_MSG       * const  OSCfg_MsgPoolBasePtr       = (OS_MSG    *)&OSCfg_MsgPool[0];
#else
OS_MSG_SIZE    const  OSCfg_MsgPoolSize          = (OS_MSG_SIZE)0;
CPU_INT32U     const  OSCfg_MsgPoolSizeRAM       = (CPU_INT32U )0;
OS_MSG       * const  OSCfg_MsgPoolBasePtr       = (OS_MSG    *)0;
#endif

CPU_STK_SIZE   const  OSCfg_StkSizeMin           = (CPU_STK_SIZE)OS_CFG_STK_SIZE_MIN;

OS_RATE_HZ     const  OSCfg_TickRate_Hz          = (OS_RATE_HZ  )OS_CFG_TICK_RATE_HZ;

/*
************************************************************************************************************************
*                                         TOTAL SIZE OF APPLICATION CONFIGURATION
************************************************************************************************************************
*/

CPU_INT32U     const  OSCfg_DataSizeRAM          = 0u

#if (OS_MSG_EN > 0u)
        + sizeof(OSCfg_MsgPool)
#endif
        + sizeof(OSCfg_IdleTaskStk);

/*
************************************************************************************************************************
*                                             OS CONFIGURATION INITIALIZATION
*
* Description: This function is used to make sure that debug variables that are unused in the application are not
*              optimized away.  This function might not be necessary for all compilers.  In this case, you should simply
*              DELETE the code in this function while still leaving the declaration of the function itself.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : (1) This code doesn't do anything, it simply prevents the compiler from optimizing out the 'const'
*                  variables which are declared in this file.
*              (2) You may decide to 'compile out' the code (by using #if 0/#endif) INSIDE the function if your compiler
*                  DOES NOT optimize out the 'const' variables above.
************************************************************************************************************************
*/

void  OSCfg_Init (void)
{
    (void)&OSCfg_DataSizeRAM;

    (void)&OSCfg_IdleTaskStkBasePtr;
    (void)&OSCfg_IdleTaskStkLimit;
    (void)&OSCfg_IdleTaskStkSize;
    (void)&OSCfg_IdleTaskStkSizeRAM;

#if (OS_MSG_EN > 0u)
    (void)&OSCfg_MsgPoolSize;
    (void)&OSCfg_MsgPoolSizeRAM;
    (void)&OSCfg_MsgPoolBasePtr;
#endif

    (void)&OSCfg_StkSizeMin;

    (void)&OSCfg_TickRate_Hz;
}
