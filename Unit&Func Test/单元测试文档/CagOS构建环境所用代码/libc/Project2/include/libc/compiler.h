/* compiler.h - compiler support routine header file */

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

#ifndef __INCcompilerh__
#define __INCcompilerh__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
/*
 * gcc version lower than 4.6 has a bug,
 * that __BYTE_ORDER__ not properly defined.
 * here we define our own
 */

#if __GNUC__ < 4 || \
    (__GNUC__ == 4 && __GNUC_MINOR__ <= 6)
#define __ORDER_LITTLE_ENDIAN 0x1234
#define __ORDER_BIG_ENDIAN 0x4321
#else
#define __BYTE_ORDER __BYTE_ORDER__
#define __ORDER_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define __ORDER_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#endif
#else
#define __ORDER_LITTLE_ENDIAN 0x1234
#define __ORDER_BIG_ENDIAN 0x4321
#if defined(_CAG_CONFIG_ARCH_PENTIUM) || \
       defined(_CAG_CONFIG_ARCH_ARM)
#define __BYTE_ORDER __ORDER_LITTLE_ENDIAN
#else
#define __BYTE_ORDER __ORDER_BIG_ENDIAN
#endif
#endif

#define swap16(x)\
        ((((x) & 0xff00) >> 8) | \
        (((x) & 0x00ff) << 8))

#define swap32(x)\
        ((((x) & 0xff000000) >> 24) | \
         (((x) & 0x00ff0000) >>  8) | \
         (((x) & 0x0000ff00) <<  8) | \
         (((x) & 0x000000ff) << 24))

#define swap64(x)\
        ((((x) & 0xff00000000000000ULL) >> 56) | \
          (((x) & 0x00ff000000000000ULL) >> 40) | \
         (((x) & 0x0000ff0000000000ULL) >> 24) | \
         (((x) & 0x000000ff00000000ULL) >>  8) | \
         (((x) & 0x00000000ff000000ULL) <<  8) | \
         (((x) & 0x0000000000ff0000ULL) << 24) | \
         (((x) & 0x000000000000ff00ULL) << 40) | \
         (((x) & 0x00000000000000ffULL) << 56))

#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN
# define cpu_to_le16(x)        (x)
# define cpu_to_le32(x)        (x)
# define cpu_to_le64(x)        (x)
# define le16_to_cpu(x)        (x)
# define le32_to_cpu(x)        (x)
# define le64_to_cpu(x)        (x)
# define cpu_to_be16(x)        swap16(x)
# define cpu_to_be32(x)        swap32(x)
# define cpu_to_be64(x)        swap64(x)
# define be16_to_cpu(x)        swap16(x)
# define be32_to_cpu(x)        swap32(x)
# define be64_to_cpu(x)        swap64(x)

#define htons(x)	swap16((x))
#define ntohs(x)	swap16((x))
#define htonl(x)	swap32((x))
#define ntohl(x)	swap32((x))
#define htonll(x)	swap64(x)
#define ntohll(x)	swap64(x)
#else
# define cpu_to_le16(x)        swap16(x)
# define cpu_to_le32(x)        swap32(x)
# define cpu_to_le64(x)        swap64(x)
# define le16_to_cpu(x)        swap16(x)
# define le32_to_cpu(x)        swap32(x)
# define le64_to_cpu(x)        swap64(x)
# define cpu_to_be16(x)        (x)
# define cpu_to_be32(x)        (x)
# define cpu_to_be64(x)        (x)
# define be16_to_cpu(x)        (x)
# define be32_to_cpu(x)        (x)
# define be64_to_cpu(x)        (x)

#define htons(x)	((x))
#define ntohs(x)	((x))
#define htonl(x)	((x))
#define ntohl(x)	((x))
#define htonll(x)	(x)
#define ntohll(x)	(x)
#endif

#define _CONCATENATE(a, b)  __CONCATENATE(a, b)
#define __CONCATENATE(a, b) a ## b
#define ASM_PFX(name) _CONCATENATE (__USER_LABEL_PREFIX__, name)
#define GCC_ASM_EXPORT(func__)  \
            .globl  _CONCATENATE (__USER_LABEL_PREFIX__, func__)    \

#define GCC_ASM_IMPORT(name)

#ifdef __cplusplus
}
#endif

#endif
