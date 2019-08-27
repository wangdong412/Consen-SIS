/* cagboot.h - cagBl header file */

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

#ifndef __CAGBOOT_H__
#define __CAGBOOT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  _ASMLANGUAGE
#   define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile unsigned char VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile int VINT32;     /* volatile unsigned word */
typedef volatile short VINT16;     /* volatile unsigned halfword */
typedef volatile char VINT8;      /* volatile unsigned byte */
typedef volatile unsigned int VUINT32;    /* volatile unsigned word */
typedef volatile unsigned short VUINT16;    /* volatile unsigned halfword */
typedef volatile unsigned char VUINT8;     /* volatile unsigned byte */
#   define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

#ifndef _ASMLANGUAGE
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned long long UINT64;
typedef signed long long INT64;

typedef long long sector_t;

/* specific definitions, to make migration easy */

typedef int STATUS;

typedef int (*FUNCPTR) (void);

typedef int BOOL;

typedef void (*VOIDFUNCPTR) (void); /* ptr to function returning void */

#define _CAG_ASM(x) __asm__ volatile (x)

#define _CAG_PACK_ALIGN(x) __attribute__((packed, aligned(x)))

#define _CAG_UNQUALIFIED_ASM(x) __asm__ (x)

#define _CAG_BARRIER(x) __asm volatile (x ::: "memory")

#define _CAG_DATA_ALIGN_BYTES(x) __attribute__((aligned(x)))

#ifndef LOCAL
#define LOCAL static
#endif

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR -1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif
