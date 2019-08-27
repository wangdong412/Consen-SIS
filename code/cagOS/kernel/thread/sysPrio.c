/* sysPrio.c - kernel/sysOs task priorities handling facilities */

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

CPU_DATA   OSPrioTbl[OS_PRIO_TBL_SIZE];                     /* Declare the array local to this file to allow for  ... */
/* ... optimization.  In other words, this allows the ... */
/* ... table to be located in fast memory                 */

/*
************************************************************************************************************************
*                                               INITIALIZE THE PRIORITY LIST
*
* Description: This function is called by uC/OS-III to initialize the list of ready priorities.
*
* Arguments  : none
*
* Returns    : none
*
* Note       : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_PrioInit (void)
{
    CPU_DATA  i;


    /* Clear the bitmap table ... no task is ready            */
    for (i = 0u; i < OS_PRIO_TBL_SIZE; i++) {
        OSPrioTbl[i] = (CPU_DATA)0;
    }
}

/*
************************************************************************************************************************
*                                           GET HIGHEST PRIORITY TASK WAITING
*
* Description: This function is called by other uC/OS-III services to determine the highest priority task
*              waiting on the event.
*
* Arguments  : none
*
* Returns    : The priority of the Highest Priority Task (HPT) waiting for the event
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

OS_PRIO  OS_PrioGetHighest (void)
{
    CPU_DATA  *p_tbl;
    OS_PRIO    prio;


    prio  = (OS_PRIO)0;
    p_tbl = &OSPrioTbl[0];
    while (*p_tbl == (CPU_DATA)0) {                         /* Search the bitmap table for the highest priority       */
        prio += DEF_INT_CPU_NBR_BITS;                       /* Compute the step of each CPU_DATA entry                */
        p_tbl++;
    }
    prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl);              /* Find the position of the first bit set at the entry    */
    return (prio);
}

/*
************************************************************************************************************************
*                                                  INSERT PRIORITY
*
* Description: This function is called to insert a priority in the priority table.
*
* Arguments  : prio     is the priority to insert
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_PrioInsert (OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;


    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
    OSPrioTbl[ix] |= bit;
}

/*
************************************************************************************************************************
*                                                   REMOVE PRIORITY
*
* Description: This function is called to remove a priority in the priority table.
*
* Arguments  : prio     is the priority to remove
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_PrioRemove (OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;


    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
    OSPrioTbl[ix] &= ~bit;
}
