/* sysAmpDemo.c - PowrPC E500 */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  arch mmu code for PPC85XX.
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
 * This file contains the mmu management.
 */

/*
DESCRIPTION
This file is used to configure and initialize the PowrPC E500 cores based
on the required components or configuration. This file includes SMP & AMP
multicore flag word configuration requried among cores, and inlcudes SMP time
base synchronization.

NOMANUAL
*/

/* includes */

#include <libc/stdio.h>
#include <libc/cagboot.h>
#include <config.h>
#include <ppc/toolPpc.h>
#include <ppc/archPpc.h>

UINT32 ampCore0[1024] _CAG_SHARED_BSS;
UINT32 ampCore1[1024] _CAG_SHARED_BSS;

static UINT32 ampCore01[1024];
UINT32 ampCore11[1024];

void sysAmpShare1 ()
{
    ampCore01[1023] = 0;
    ampCore11[1023] = 0;
}

void sysAmpShare ()
{
    sysAmpShare1();
    ampCore01[1023] = 0;
    ampCore11[1023] = 0;
    ampCore0[1023] = 0;
    ampCore1[1023] = 0;
}
