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

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <config.h>

#define  CLOCK_CFG_TASK_START_PRIO 0
/*#define  OS_CFG_STAT_TASK_PRIO    31*/
//#define  APP_CFG_TASK_START_PRIO  6/*30u*/
#define  OS_CFG_TASK_STK_LIMIT_PCT_EMPTY  10u

#define  OS_CFG_MSG_POOL_SIZE            100u               /* Maximum number of messages */
#define TASK_STACK_MASK (TASK_STACK_SIZE-1)
#define TASK_STACK_0  (TASK_STACK_HIGH-(0*TASK_STACK_SIZE))
#define TASK_STACK_1  (TASK_STACK_HIGH-(1*TASK_STACK_SIZE))
#define TASK_STACK_2  (TASK_STACK_HIGH-(2*TASK_STACK_SIZE) )
#define TASK_STACK_3  (TASK_STACK_HIGH-(3*TASK_STACK_SIZE) )
#define TASK_STACK_4  (TASK_STACK_HIGH-(4*TASK_STACK_SIZE) )
#define TASK_STACK_5  (TASK_STACK_HIGH-(5*TASK_STACK_SIZE) )

/* FOR SPE REGS OPS STACK */
#define TASK_SPESTACK_0  (TASK_SPESTACK_HIGH-(0*TASK_SPESTACK_SIZE))
#define TASK_SPESTACK_1  (TASK_SPESTACK_HIGH-(1*TASK_SPESTACK_SIZE))
#define TASK_SPESTACK_2  (TASK_SPESTACK_HIGH-(2*TASK_SPESTACK_SIZE))
#define TASK_SPESTACK_3  (TASK_SPESTACK_HIGH-(3*TASK_SPESTACK_SIZE))
#define TASK_SPESTACK_4  (TASK_SPESTACK_HIGH-(4*TASK_SPESTACK_SIZE))
#define TASK_SPESTACK_5  (TASK_SPESTACK_HIGH-(5*TASK_SPESTACK_SIZE))
#endif
