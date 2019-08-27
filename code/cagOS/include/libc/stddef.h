/* stddef.h - standard type definitions */

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

#ifndef __INCStddef_h__
#define __INCStddef_h__

#include <libc/cagboot.h>

#ifdef __cpluplus
extern "C" {
#endif

#if __STDC_VERSION__ >= 199901L
    /* "restrict" is a keyword */
#else
# define restrict
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/* Note: ptrdiff_t and wchar_t are not defined for VXBL */

#define CHAR_BIT        8               /* number of bits in a char */
#define MB_LEN_MAX      32              /* no multibyte characters */

/*
 * According to ANSI (section 2.2.4.2), the values below must be usable by
 * #if preprocessing directives.  Additionally, the expression must have the
 * same type as would an expression that is an object of the corresponding
 * type converted according to the integral promotions.  The subtraction for
 * INT_MIN and LONG_MIN is so the value is not unsigned; 2147483648 is an
 * unsigned int for 32-bit two's complement ANSI compilers (section 3.1.3.2).
 * These numbers work for pcc as well.  The UINT_MAX and ULONG_MAX values
 * are written as hex so that GCC will be quiet about large integer constants.
 */
#define SCHAR_MAX       127             /* min value for a signed char */
#define SCHAR_MIN       (-128)          /* max value for a signed char */

#define UCHAR_MAX       255             /* max value for an unsigned char */
#define CHAR_MAX        127             /* max value for a char */
#define CHAR_MIN        (-128)          /* min value for a char */

#define USHRT_MAX       65535           /* max value for an unsigned short */
#define SHRT_MAX        32767           /* max value for a short */
#define SHRT_MIN        (-32768)        /* min value for a short */

#define UINT_MAX        0xffffffffU     /* max value for an unsigned int */
#define INT_MAX         2147483647      /* max value for an int */
#define INT_MIN         (-2147483647-1) /* min value for an int */

#define SSIZE_MAX       INT_MAX         /* max value for a ssize_t */
#define SIZE_T_MAX      UINT_MAX        /* max value for a size_t */

#define INT8_MIN    (-0x7F - 1)
#define INT8_MAX    0x7F
#define UINT8_MAX    0xFF

#define INT16_MIN    (-0x7FFF - 1)
#define INT16_MAX    0x7FFF
#define UINT16_MAX    0xFFFF

#define INT32_MIN    (-0x7FFFFFFF - 1)
#define INT32_MAX    0x7FFFFFFF
#define UINT32_MAX    0xFFFFFFFFU

#define INT64_MIN    (-0x7FFFFFFFFFFFFFFFLL - 1)
#define INT64_MAX    0x7FFFFFFFFFFFFFFFLL
#define UINT64_MAX    0xFFFFFFFFFFFFFFFFULL

#define INTPTR_MIN    INT32_MIN
#define INTPTR_MAX    INT32_MAX
#define UINTPTR_MAX    UINT32_MAX

#define PTRDIFF_MIN    INT32_MIN
#define PTRDIFF_MAX    INT32_MAX

#define SIG_ATOMIC_MIN    INT32_MIN
#define SIG_ATOMIC_MAX    INT32_MAX

#define SIZE_MAX    UINT32_MAX

#define ULONG_MAX   UINT32_MAX
#define LONG_MAX    INT32_MAX
#define LONG_MIN    INT32_MIN

#ifdef __cpluplus
}
#endif

#endif
