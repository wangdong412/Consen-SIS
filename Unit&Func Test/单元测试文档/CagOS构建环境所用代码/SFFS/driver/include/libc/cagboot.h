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

#include <libc/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __iomem

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
//to be exported
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned long long UINT64;
typedef signed long long INT64;

typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef long ptrdiff_t;

typedef unsigned long phys_addr_t;
typedef unsigned long phys_size_t;

//end exported

typedef long long sector_t;

/* specific definitions, to make migration easy */

typedef int STATUS;

typedef int (*FUNCPTR) (void);

typedef int BOOL;

typedef void (*VOIDFUNCPTR) (void); /* ptr to function returning void */


#define _CAG_SHARED_BSS __attribute__((__section__(".shared.bss"))); 

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

#define OK 0
#define ROUND_UP(x, align)  (((size_t) (x) + (align - 1)) & ~(align - 1))

/* helper routines */

#define Stringify(s) __Stringify(s)
#define __Stringify(s) #s

#define __address_add_offset(addr, size) \
        ((void *)((char *)(addr) + (size)))

#define __address_del_offset(addr, size) \
        ((void *)((char *)(addr) - (size)))

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

/* Handy sizes */

#define SZ_4K                           0x00001000
#define SZ_8K                           0x00002000
#define SZ_16K                          0x00004000
#define SZ_32K                          0x00008000
#define SZ_64K                          0x00010000
#define SZ_128K                         0x00020000
#define SZ_256K                         0x00040000
#define SZ_512K                         0x00080000

#define SZ_1M                           0x00100000
#define SZ_2M                           0x00200000
#define SZ_4M                           0x00400000
#define SZ_8M                           0x00800000
#define SZ_16M                          0x01000000
#define SZ_32M                          0x02000000
#define SZ_64M                          0x04000000
#define SZ_128M                         0x08000000
#define SZ_256M                         0x10000000
#define SZ_512M                         0x20000000

#define SZ_1G                           0x40000000
#define SZ_2G                           0x80000000

#ifdef __cplusplus
}
#endif

#endif
