/* sysSpeExcLib.c - PowerPC e500v2 FP IEEE-754 compliance library  */

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
Purpose of this library is to make double-precision and single-precision
scalar floating-point operations compliant with the IEEE-754 floating-point
standard.

Operations on denormalized numbers, Infinities and NaNs are not IEEE-754
compliant on the e500 platforms. If any of these values are used as input
or overflow/underflow occurs on output of a floating-point instruction
then an Embedded Floating-Point Data Interrupt is generated. This library
provides exception handler for the mentioned Embedded Floating-Point Data
Interrupt which purpose is to provide IEEE-754 compliant result for
instruction which generated the interrupt.

INTERNALS
Handler provided in this library extracts the operands from and stores
the result into the register set in the task context block of the interrupted
task (in case of upper 32-bits of GPRs result is written directly to the
register). This way an IEEE compliant result is simulated on return from
the interrupt.

It is assumed that the OS enables the SPE APU as well as SPE exceptions
(invalid operation/input, divide by zero, underflow and overflow) for the tasks
that may use the SPE floating-point scalar instructions. The OS is also
responsible for maintaning the state of the SPE specific registers between
task context switches (i.e. SPEFSCR, upper 32-bits of GPRs).

This library must be initialized during system initialization by means
of the speExcInit() service.  It uses the exception base user hook called by
excExcHandle() to recognize and handle the exception.

NOTE
Vector single-precision floating-point operations are not supported
by this handler.

*/

/* includes */


#include <config.h>
#include <libc/cagboot.h>
#include <ppc/archPpc.h>
#include <ppc/toolPpc.h>
#include <ppc/ppc85xx.h>
#include <ppc/excPpcLib.h>
#include <ppc/spePpcLib.h>

/* locals */

UINT32 e500_spe_exc_enable;
UINT32 e500_spe_exc_mask;

/*******************************************************************************
*
* speExcInit - initialize SPE exceptions support
*
* This routine must be called before the first task using SPE instructions
* is run within any partition.
*
* RETURNS: N/A
*
*/

void speExcInit (void)
{
    UINT32 spefscr = sysSpefscrGet ();
    UINT32 msr = sysMsrGet ();
    sysMsrSet (msr | _PPC_MSR_SPE);
    e500_spe_exc_mask = _PPC_SPEFSCR_FINVE | _PPC_SPEFSCR_FDBZE |
                        _PPC_SPEFSCR_FUNFE | _PPC_SPEFSCR_FOVFE;
    spefscr |= e500_spe_exc_mask;
    sysSpefscrSet (spefscr);
    e500_spe_exc_enable = TRUE;
}
