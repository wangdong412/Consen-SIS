/* stdio.h - standard buffered input/output */

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

#ifndef __INCgpioh__
#define __INCgpioh__

#include <libc/stddef.h>

#define GPIO_BASE                       (_CAG_CONFIG_CCSBAR + 0xF000)
#define GPDIR                           (GPIO_BASE + 0xC00)
#define GPODR                           (GPIO_BASE + 0xC04)
#define GPDAT                           (GPIO_BASE + 0xC08)
#define GPIER                           (GPIO_BASE + 0xC0C)
#define GPIMR                           (GPIO_BASE + 0xC10)
#define GPICR                           (GPIO_BASE + 0xC14)

#define GPIO_0                          0
#define GPIO_1                          1
#define GPIO_2                          2
#define GPIO_3                          3
#define GPIO_4                          4
#define GPIO_5                          5
#define GPIO_6                          6
#define GPIO_7                          7
#define GPIO_8                          8
#define GPIO_9                          9
#define GPIO_10                         10
#define GPIO_11                         11
#define GPIO_12                         12
#define GPIO_13                         13
#define GPIO_14                         14
#define GPIO_15                         15

#define GPIO_PIN_MAX                    15  /* 16 pins (incl. 0) */

#define GPIO_LOW                        0x00
#define GPIO_HIGH                       0x01

#define GPIO_REG_WRITE(regPtr,val)      (*((VUINT32 *)(regPtr)) = (val))
#define GPIO_REG_READ(regPtr,res)       ((res) = *(VUINT32 *)(regPtr))

#define GPIO_CODE_REENTRANT

#ifdef GPIO_CODE_REENTRANT
#   define GPIO_INTLOCK(key)
#   define GPIO_INTUNLOCK(key)
#   define GPIO_KEYDECLARE
#else  /* GPIO_CODE_REENTRANT */
#   define GPIO_INTLOCK(key)            (key = intLock())
#   define GPIO_INTUNLOCK(key)          (intUnlock(key))
#   define GPIO_KEYDECLARE              int intKey
#endif  /* GPIO_CODE_REENTRANT */

/* GPIO pin types */

#define GPIO_OUT                        0x00008000
#define GIPO_OD                         0x00000001
#define GPIO_IN                         0x00000000
#define GPIO_INT                        0x80000000

/* Interrupt styles, these refer to actual values used in reg */

#define GPIO_INT_FAIL_TRIG              0x00010000
#define GPIO_INT_LEVEL_TRIG             0x00020000

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
