/* sysBanner.c - cagOS banner */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  the cagOs entry code.
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
 * This library provides board-specific routines for P1020RDB
 */

#include <libc/cagboot.h>
#include <libc/system.h>
#include <libc/stdio.h>

/*******************************************************************************
*
* printLogo - print the bootLoader logo
*
* This command displays the bootLoader banner seen at boot time.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

void printLogo (void)
{

    static char *logo[] = {
        "\n",
        "     @@@@     @@@      @@@@@ (R)   ####      ###### ",
        "   @@       @@  @@    @@         ##    ##   ##      ",
        "  @@       @@    @@  @@   @@@@  ##      ## ######   ",
        "  @@      @@@@@@@@@@ @@    @@    ##    ##      ## # ",
        "   @@@@@ @@@      @@@  @@@@@@      ####  ####### ## ",
        "                   Copyright CAG Systems, Inc. 2015 ",
        "                             #Powered by Jeff & Lei ",
    };
    FAST int ix;

    for (ix = 0; ix < (int)NELEMENTS(logo); ix++)
        printf (" %s\n", logo [ix]);

    printf ("\n");
}

/*******************************************************************************
*
* usrBanner - print the BootStrap banner
*
* This routine prints the BootStrap banner. This routine is called
* from usrRoot.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void sysBanner(void)
{
    printLogo ();
}
