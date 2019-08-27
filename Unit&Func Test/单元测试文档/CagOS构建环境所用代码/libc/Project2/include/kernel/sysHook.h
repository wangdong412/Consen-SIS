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

#ifndef  OS_APP_HOOKS_H
#define  OS_APP_HOOKS_H


#ifdef   OS_APP_HOOKS_H_GLOBALS
#define  OS_APP_HOOKS_H_EXT
#else
#define  OS_APP_HOOKS_H_EXT  extern
#endif

/*
************************************************************************************************************************
*                                                 INCLUDE HEADER FILES
************************************************************************************************************************
*/

#include "sysOs.h"

/*
************************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
************************************************************************************************************************
*/

void  App_OS_SetAllHooks   (void);
void  App_OS_ClrAllHooks   (void);


                                                                /* ---------------------- HOOKS --------------------- */
void  App_OS_TaskCreateHook(OS_TCB  *p_tcb);
void  App_OS_TaskDelHook   (OS_TCB  *p_tcb);
void  App_OS_TaskReturnHook(OS_TCB  *p_tcb);

void  App_OS_IdleTaskHook  (void);
void  App_OS_InitHook      (void);
void  App_OS_StatTaskHook  (void);
void  App_OS_TaskSwHook    (void);
void  App_OS_TimeTickHook  (void);

#endif
