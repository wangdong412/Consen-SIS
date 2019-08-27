/* ns16550.c - serial handling facilities */

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
 * This file contains
 */

#include <libc/cagboot.h>
#include <libc/stdio.h>
#include <libc/access.h>
#include <config.h>

#if !defined(_CAG_CONFIG_SIO_BASE)
#error "sio device base address not configured!"
#endif

#if !defined(_CAG_CONFIG_SIO_REG_INTERVAL)
#error "sio device register size not configured!"
#endif

#define SIO_READ(a) inb(a)
#define SIO_WRITE(a, v) outb(a, v)

#define NS16550(x) \
    unsigned char x; \
    unsigned char __pad__##x [_CAG_CONFIG_SIO_REG_INTERVAL - 1];

struct ns16550_dev {
    NS16550(rhr)
    NS16550(ier)
    NS16550(iir)
    NS16550(lcr)
    NS16550(mcr)
    NS16550(lsr)
    NS16550(msr)
    NS16550(spr)
    NS16550(mdr1)
};
#define thr rhr
#define rbr rhr
#define dll rhr
#define dlm ier
#define fcr iir
#define tcr msr
#define tlr spr

#define BAUD_LO(clk, baud)  ((clk / (16 * baud)) & 0xff)
#define BAUD_HI(clk, baud)  (((clk / (16 * baud)) & 0xff00) >> 8)

/* Line Control Register */

#define CHAR_LEN_5  0x00    /* 5bits data size */
#define CHAR_LEN_6  0x01    /* 6bits data size */
#define CHAR_LEN_7  0x02    /* 7bits data size */
#define CHAR_LEN_8  0x03    /* 8bits data size */
#define LCR_STB     0x04    /* 2 stop bits */
#define ONE_STOP    0x00    /* one stop bit */
#define LCR_PEN     0x08    /* parity enable */
#define PARITY_NONE 0x00    /* parity disable */
#define LCR_EPS     0x10    /* even parity select */
#define LCR_SP      0x20    /* stick parity select */
#define LCR_SBRK    0x40    /* break control bit */
#define LCR_DLAB    0x80    /* divisor latch access enable */
#define DLAB        LCR_DLAB

/* Line Status Register */

#define LSR_DR      0x01    /* data ready */
#define RxCHAR_AVAIL    LSR_DR  /* data ready */
#define LSR_OE      0x02    /* overrun error */
#define LSR_PE      0x04    /* parity error */
#define LSR_FE      0x08    /* framing error */
#define LSR_BI      0x10    /* break interrupt */
#define LSR_THRE    0x20    /* transmit holding register empty */
#define LSR_TEMT    0x40    /* transmitter empty */
#define LSR_FERR    0x80    /* in fifo mode, set when PE,FE or BI error */

/* Interrupt Identification Register */

#define IIR_IP      0x01
#define IIR_ID      0x0e
#define IIR_RLS     0x06    /* received line status */
#define Rx_INT      IIR_RLS /* received line status */
#define IIR_RDA     0x04    /* received data available */
#define RxFIFO_INT  IIR_RDA /* received data available */
#define IIR_THRE    0x02    /* transmit holding register empty */
#define TxFIFO_INT  IIR_THRE
#define IIR_MSTAT   0x00    /* modem status */
#define IIR_TIMEOUT 0x0c    /* char receive timeout */

/* Interrupt Enable Register */

#define IER_ERDAI   0x01    /* received data avail. & timeout int */
#define RxFIFO_BIT  IER_ERDAI
#define IER_ETHREI  0x02    /* transmitter holding register empty int */
#define TxFIFO_BIT  IER_ETHREI
#define IER_ELSI    0x04    /* receiver line status int enable */
#define Rx_BIT      IER_ELSI
#define IER_EMSI    0x08    /* modem status int enable */

/* Modem Control Register */

#define MCR_DTR     0x01    /* dtr output */
#define DTR     MCR_DTR
#define MCR_RTS     0x02    /* rts output */
#define MCR_OUT1    0x04    /* output #1 */
#define MCR_OUT2    0x08    /* output #2 */
#define MCR_LOOP    0x10    /* loopback enable */

/* Modem Status Register */

#define MSR_DCTS    0x01    /* cts change */
#define MSR_DDSR    0x02    /* dsr change */
#define MSR_TERI    0x04    /* ring indicator change */
#define MSR_DDCD    0x08    /* data carrier indicator change */
#define MSR_CTS     0x10    /* complement of cts */
#define MSR_DSR     0x20    /* complement of dsr */
#define MSR_RI      0x40    /* complement of ring signal */
#define MSR_DCD     0x80    /* complement of dcd */

/* FIFO Control Register */

#define FCR_EN      0x01    /* enable xmit and rcvr */
#define FIFO_ENABLE FCR_EN
#define FCR_RXCLR   0x02    /* clears rcvr fifo */
#define RxCLEAR     FCR_RXCLR
#define FCR_TXCLR   0x04    /* clears xmit fifo */
#define TxCLEAR     FCR_TXCLR
#define FCR_DMA     0x08    /* dma */
#define FCR_RXTRIG_L    0x40    /* rcvr fifo trigger lvl low */
#define FCR_RXTRIG_H    0x80    /* rcvr fifo trigger lvl high */

/* static functions */

void ns16550OutputChar (char c);
char ns16550InputChar (void);
static struct ns16550_dev *siodev;

/* external functions */

UINT32 sysUartClkRateGet(void);

/******************************************************************************
*
* ns16550OutputChar - output a character
*
* This routine outputs a character to serial port.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void ns16550OutputChar
(
    char c
)
{
    while ((SIO_READ(&siodev->lsr) &  LSR_THRE) == 0)
        ;
    SIO_WRITE(&siodev->thr, c);
}

/******************************************************************************
*
* ns16550InputChar - read a character
*
* This routine reads a character from serial port.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

char ns16550InputChar (void)
{
    char c;

    while ((SIO_READ(&siodev->lsr) &  LSR_DR) == 0)
        ;
    c = SIO_READ(&siodev->rbr);

    return c;
}

/******************************************************************************
*
* ns16550DevInit - initialize ns16550 device
*
* This routine initializes the ns16550 hardware
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void ns16550DevInit(void)
{
    UINT32 uartClk = sysUartClkRateGet();

    siodev = (struct ns16550_dev *)_CAG_CONFIG_SIO_BASE;

    SIO_WRITE (&siodev->ier, 0x0);

    SIO_WRITE (&siodev->lcr, CHAR_LEN_8 | ONE_STOP | LCR_DLAB);
    SIO_WRITE (&siodev->dll, 0x0);
    SIO_WRITE (&siodev->dlm, 0x0);
    SIO_WRITE (&siodev->lcr, CHAR_LEN_8 | ONE_STOP);
    SIO_WRITE (&siodev->fcr, RxCLEAR | TxCLEAR | FIFO_ENABLE | \
               FCR_RXTRIG_L | FCR_RXTRIG_H);
    SIO_WRITE (&siodev->lcr, CHAR_LEN_8 | ONE_STOP | LCR_DLAB);
    SIO_WRITE (&siodev->dll, BAUD_LO(uartClk, _CAG_CONFIG_SIO_BAUDRATE));
    SIO_WRITE (&siodev->dlm, BAUD_HI(uartClk, _CAG_CONFIG_SIO_BAUDRATE));
    SIO_WRITE (&siodev->lcr, CHAR_LEN_8 | ONE_STOP);

}

/******************************************************************************
*
* ns16550SioInit - register console
*
* This routine registers console
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void ns16550SioInit(void)
{
    ns16550DevInit();
}

