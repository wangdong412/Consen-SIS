/* assert.h - verify program assertion */

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

#ifndef __INCasserth__
#define __INCasserth__

#include <libc/stdio.h>
#include <libc/errno.h>
#include <libc/string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NDEBUG
#ifdef NDEBUG
#define assert(x) ((void)0)
#else
#define assert(x) \
    do \
        { \
        if (!(x)) \
            { \
            (void)printf("assertion failed at file: %s, line: %u\n",\
             __FILE__, __LINE__); \
            abort(); \
            } \
        } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif
