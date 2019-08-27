/* shell.h - shell interface */

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
 * This file contains.
 */

#ifndef __INCShellh__
#define __INCShellh__

#ifdef __cplusplus
extern "C" {
#endif

struct shell_cmd {
    const char * name;
    const char * help;
    const char * usage;
    int (*handler)(int argc, char * argv[]);
};

void shellCmdRegister(const struct shell_cmd * cmd);

extern void usrShell(void);

#ifdef __cplusplus
}
#endif

#endif
