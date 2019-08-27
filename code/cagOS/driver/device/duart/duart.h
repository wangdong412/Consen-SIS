#pragma once

#include <libc/cagboot.h>
#include <config.h>
#include <libc/access.h>


#define SIO_READ(a) inb(a)
#define SIO_WRITE(a, v) outb(a, v)



#define NS16550(x) \
    unsigned char x; \
    unsigned char __pad__##x [_CAG_CONFIG_SIO_REG_INTERVAL - 1];

struct duart{
    UINT8 rbr;
    UINT8 ier;
    UINT8 iir;
    UINT8 lcr;
    UINT8 mcr;
    UINT8 lsr;
    UINT8 msr;
    UINT8 scr;
    UINT8 pad1[2];
    UINT8 dsr;
};

#define thr rbr
#define dlb rbr
#define dmb ier
#define fcr iir
#define afr iir

#define DUART1_BASE 0xFFE04500
#define DUART2_BASE 0xFFE04600


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
#define FCR_RXCLR   0x02    /* clears rcvr fifo */
#define FCR_TXCLR   0x04    /* clears xmit fifo */
#define FCR_DMS     0x08    /* dma */
#define FCR_RXTRIG_L    0x40    /* rcvr fifo trigger lvl low */
#define FCR_RXTRIG_H    0x80    /* rcvr fifo trigger lvl high */
#define RTL_14  0xc0
#define RTL_4  0x40
#define RTL_1  0x00


