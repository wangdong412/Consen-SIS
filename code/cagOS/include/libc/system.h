/* system.h - dummy header file */

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

#ifndef __INCsystemh__
#define __INCsystemh__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE
typedef unsigned char uchar_t;
typedef unsigned long ulong_t;
#define FAST  register
#define IMPORT  extern

#define OFFSET(structure, member)       /* byte offset of member in structure*/\
                ((long) &(((structure *) 0) -> member))

#define MEMBER_SIZE(structure, member)  /* size of a member of a structure */\
                (sizeof (((structure *) 0) -> member))

#define NELEMENTS(array)                /* number of elements in an array */ \
                (sizeof (array) / sizeof ((array) [0]))

#define FOREVER for (;;)

#define CHAR_FROM_CONST(x) (char *)(x)
#define VOID_FROM_CONST(x) (void *)(x)

#if !defined(__cplusplus)

#ifndef max
#define max(x, y)	(((x) < (y)) ? (y) : (x))
#endif

#ifndef min
#define min(x, y)	(((x) < (y)) ? (x) : (y))
#endif

#endif /* !__cplusplus */

#endif

#ifdef __cplusplus
}
#endif

#endif
