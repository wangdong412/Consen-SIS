/* sysSpeShow.c - spe show routines */

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
 * This file contains SPE management.
 */

/*
DESCRIPTION
This library provides the routines necessary to show a task's optional
spe context.  To use this facility, it must first be
installed using speShowInit(), which is called automatically
when the spe show facility is configured into VxWorks
using either of the following methods:
.iP
If you use the configuration header files, define
INCLUDE_SHOW_ROUTINES in config.h.
.iP
.LP

This library enhances task information routines, such as ti(), to display
the spe context.

INCLUDE FILES: speLib.h

SEE ALSO: speLib
*/

#include <stdio.h>

/* global variables */

char *speTaskRegsFmt = "%-6.6s = %8x";
REG_INDEX speGprRegName [] = {
    {"r0h",  SPE_OFFSET( 0)},
    {"r1h",  SPE_OFFSET( 1)},
    {"r2h",  SPE_OFFSET( 2)},
    {"r3h",  SPE_OFFSET( 3)},
    {"r4h",  SPE_OFFSET( 4)},
    {"r5h",  SPE_OFFSET( 5)},
    {"r6h",  SPE_OFFSET( 6)},
    {"r7h",  SPE_OFFSET( 7)},
    {"r8h",  SPE_OFFSET( 8)},
    {"r9h",  SPE_OFFSET( 9)},
    {"r10h", SPE_OFFSET(10)},
    {"r11h", SPE_OFFSET(11)},
    {"r12h", SPE_OFFSET(12)},
    {"r13h", SPE_OFFSET(13)},
    {"r14h", SPE_OFFSET(14)},
    {"r15h", SPE_OFFSET(15)},
    {"r16h", SPE_OFFSET(16)},
    {"r17h", SPE_OFFSET(17)},
    {"r18h", SPE_OFFSET(18)},
    {"r19h", SPE_OFFSET(19)},
    {"r20h", SPE_OFFSET(20)},
    {"r21h", SPE_OFFSET(21)},
    {"r22h", SPE_OFFSET(22)},
    {"r23h", SPE_OFFSET(23)},
    {"r24h", SPE_OFFSET(24)},
    {"r25h", SPE_OFFSET(25)},
    {"r26h", SPE_OFFSET(26)},
    {"r27h", SPE_OFFSET(27)},
    {"r28h", SPE_OFFSET(28)},
    {"r29h", SPE_OFFSET(29)},
    {"r30h", SPE_OFFSET(30)},
    {"r31h", SPE_OFFSET(31)},
    {NULL, 0},
};

/*******************************************************************************
*
* speTaskRegsShow - print the contents of a task's spe registers
*
* This routine prints to standard output the contents of a task's
* spe registers.
*
* ERRNO: N/A
*
* RETURNS: N/A
*/

void speTaskRegsShow
(
    int task		/* task to display spe registers for */
)
{
    SPE_CONTEXT *	pRegs;
    WIND_TCB *		pTcb;
    int			reg;
    UINT32 *		pInt;

    pTcb = taskTcb (task);
    pRegs = SPE_CONTEXT_GET (pTcb);

    if (pRegs) {
        (void) printf ("\nUpper 32 bits of GPR \n");
        for (reg=0; reg<32; reg+=4) {
            (void) printf ("r%d 0x%x r%d 0x%x r%d 0x%x r%d 0x%x\n",
                           reg,pRegs->gpr[reg],
                           reg+1,pRegs->gpr[reg+1],
                           reg+2,pRegs->gpr[reg+2],
                           reg+3,pRegs->gpr[reg+3]);

        }
        (void) printf ("\naccumulator = %x %x \n", pRegs->acc[0], pRegs->acc[1]);

    } else {
        (void) printf ("No Spe Context\n");
    }
}

/******************************************************************************
*
* speCtxShow - print the contents of a task's SPE registers
*
* This routine prints to standard output the contents of a task's
* SPE registers.
*
* ERRNO: N/A
*
* RETURNS: N/A
*/

void speCtxShow
(
    SPE_CONTEXT * pRegs
)
{
    int	reg;

    if (pRegs == (SPE_CONTEXT *)NULL) {
        (void) printf ("Invalid or no SPE context\n");
    } else {
        (void) printf ("\nSPE context - Upper 32 bits of GPR");
        for (reg = 0; reg < SPE_NUM_REGS; reg++) {
            if ((reg & 0x3) == 0x0)
                (void) printf ("\n");
            else
                (void) printf ("%3s", "");

            (void) printf (speTaskRegsFmt, speGprRegName[reg].regName,
                           pRegs->gpr[reg]);

        }

        (void) printf ("\n");
        (void) printf ("acch   = %8x   ", pRegs->acc[1]);
        (void) printf ("accl   = %8x   \n", pRegs->acc[0]);
    }

    (void) printf ("\n");
}
