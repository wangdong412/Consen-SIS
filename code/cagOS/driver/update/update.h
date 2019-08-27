/* sysFslSpi.h - FreeScale PowerPC eSPI driver header file */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,19sep12,d_l  add some SPI MODE definitions.(WIND00376535)
01a,10jun11,y_y  written.
*/

#ifndef __INCsysfslSpih
#define __INCsysfslSpih

#ifdef __cplusplus
extern "C"
{
#endif

#define NORMAL                  0
#define TRANSONLY               1

/* SPI register offsets */

#define SPMODE                  0x00   /* eSPI mode register */
#define SPIE                    0x04   /* eSPI event register */
#define SPIM                    0x08   /* eSPI mask register */
#define SPCOM                   0x0C   /* eSPI command register */
#define SPITF                   0x10   /* eSPI transmit FIFO access register*/
#define SPIRF                   0x14   /* eSPI receive FIFO access register */
#define SPMODE0                 0x20   /* eSPI CS0 mode register */
#define SPMODE1                 0x24   /* eSPI CS1 mode register */
#define SPMODE2                 0x28   /* eSPI CS2 mode register */
#define SPMODE3                 0x2C   /* eSPI CS3 mode register */

/* eSPI Mode Register bits */

#define SPI_EN_BIT              0
#define SPI_EN                  ((UINT32)0x01 << (31 - SPI_EN_BIT))
#define SPI_TXTHR_BIT           23
#define SPI_TXTHR_LEN           (0x3 << (31 - SPI_TXTHR_BIT))
#define SPI_RXTHR_BIT           31
#define SPI_RXTHR_LEN           (0x4 << (31 - SPI_RXTHR_BIT))

#define SPI_CSMODE_CSnCG        (0x01 << 3)
#define SPI_CSMODE_LENn         (0x07 << 16)
#define SPI_CSMODE_POLn_LOW     (0x01 << 20)
#define SPI_CSMODE_DIV16n       (0x01 << 28)
#define SPI_CSMODE_PMn(x)       ( (x) << 24)
#define SPI_CSMODE_REVn_MSB     (0x01 << 29)      

/* mask */

#define SPIMODE_PM_MASK         0xE07FFFFF
#define SPIMODE_CS_MASK         0x3FFFFFFF
#define SPIMODE_CICP_MASK       0x3FFFFFFF

#define SPICSMODE_DIV16         0x10000000
#define SPICSMODE_ODD           0x00800000

/* eSPI Event Register bits */

#define SPI_EVENT_CLR           0xFFFFFFFF

/* eSPI Command Register bits */

#define SPCOM_CS(x)             ((x) << 30)
#define SPCOM_RxSKIP(x)         ((x) << 16)
#define SPCOM_TO(x)             ((x) << 27)

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCsysfslSpih */
