/*
 *------------------------------------------------------------
 * Project: cagOS
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
 * 2015.10.21, Ezio created
 *
 * DECRIPTION
 * spi driver source file. contain a simple spi flash driver.
 *
 */


#include <p1020rdb.h>
#include <config.h>
#include "update.h"
#include <libc/gpio.h>

#define NULL     0
#define OK       0
#define ERROR   -1

/* defines */

#define SPI_REG_READ_32(addr)         *((VUINT32 *)(SPI_REG_BASE + addr))
#define SPI_REG_WRITE_32(addr, data)  *((VUINT32 *)(SPI_REG_BASE + addr)) = data

#define EIEIO_SYNC                    __asm__ __volatile__ (" eieio; sync")

#define TIMEOUT_SPI 300

#define ESPI_MAX_CS_NUM		4
#define ESPI_FIFO_WIDTH_BIT	32

#define ESPI_EV_RNE		(1 << 9)
#define ESPI_EV_TNF		(1 << 8)
#define ESPI_EV_DON		(1 << 14)
#define ESPI_EV_TXE		(1 << 15)
#define ESPI_EV_RFCNT_SHIFT	24
#define ESPI_EV_RFCNT_MASK	(0x3f << ESPI_EV_RFCNT_SHIFT)

#define ESPI_MODE_EN		(1 << 31)	/* Enable interface */
#define ESPI_MODE_TXTHR(x)	((x) << 8)	/* Tx FIFO threshold */
#define ESPI_MODE_RXTHR(x)	((x) << 0)	/* Rx FIFO threshold */

#define ESPI_COM_CS(x)		((x) << 30)
#define ESPI_COM_TRANLEN(x)	((x) << 0)

#define ESPI_CSMODE_CI_INACTIVEHIGH	(1 << 31)
#define ESPI_CSMODE_CP_BEGIN_EDGCLK	(1 << 30)
#define ESPI_CSMODE_REV_MSB_FIRST	(1 << 29)
#define ESPI_CSMODE_REV_LSB_FIRST	(0 << 29)
#define ESPI_CSMODE_DIV16		(1 << 28)
#define ESPI_CSMODE_PM(x)		((x) << 24)
#define ESPI_CSMODE_POL_ASSERTED_LOW	(1 << 20)
#define ESPI_CSMODE_LEN(x)		((x) << 16)
#define ESPI_CSMODE_CSBEF(x)		((x) << 12)
#define ESPI_CSMODE_CSAFT(x)		((x) << 8)
#define ESPI_CSMODE_CSCG(x)		((x) << 3)

#define ESPI_CSMODE_INIT_VAL (ESPI_CSMODE_POL_ASSERTED_LOW | \
		ESPI_CSMODE_CSBEF(0) | ESPI_CSMODE_CSAFT(0) | \
		ESPI_CSMODE_CSCG(1))

#define ESPI_MAX_DATA_TRANSFER_LEN 0xFFF0
#define READ_MAX_LEN            0xfff8
#define min(a, b) (((a) < (b)) ? (a) : (b))

extern void sysUsDelay (int delay);

void spi_delay(void)
{
    sysUsDelay(20);
}

/*
 * sysSpiCyclone
 * config CS0
*/

void sysCycloneInit(void)
{
    UINT32 clk = 0;
    UINT32 temp = 0;
    UINT32 clkFreq;
    UINT16 divideRatio = 0;
    UINT8 pm = 0;
    BOOL  div16 = FALSE;
    UINT32 freq = 0;
    //UINT32 spmode0 = 0x28170008;
    UINT32 spmode0 = 0x2F170008;
    printf("SPMODE0:%x,pram:%x\n", spmode0, ~(ESPI_CSMODE_PM(0xF) | ESPI_CSMODE_DIV16 | ESPI_CSMODE_POL_ASSERTED_LOW \
                        | ESPI_CSMODE_CI_INACTIVEHIGH | ESPI_CSMODE_CP_BEGIN_EDGCLK \
                        | ESPI_CSMODE_REV_MSB_FIRST | ESPI_CSMODE_LEN(0xF)));
    SPI_REG_WRITE_32 (SPMODE0, spmode0 & ~ESPI_CSMODE_REV_MSB_FIRST);
    printf("SPMODE0:%x\n",SPI_REG_READ_32(SPMODE0)); 
    sysGpioLineConfig (GPIO_11, GPIO_IN);
    sysGpioLineConfig (GPIO_12, GPIO_IN);
    sysGpioLineConfig (GPIO_13, GPIO_OUT);
    sysGpioLineSet (GPIO_13, GPIO_HIGH);
    sysUsDelay (100000);
}

/*******************************************************************************
*
* sysSpiReadData - read data from SPI receive FIFO on poll mode
*
* The purpose of this function is to read data from eSPI receive FIFO to dest
* address.
*
* RETURNS:  OK or ERROR if there is an error.
*/

INT32 sysSpiReadData
(
    UINT32 datalen,
    UINT8 * data
)
{
    VUINT32 readData;
    UINT8 * pointer = NULL;

    UINT32 numCount = (datalen / 4 + ((datalen % 4 != 0) ? 1 : 0));
    UINT32 numChars = datalen % 4;
    UINT32 i;
    UINT32 command;
    UINT32 mode = 0;

    if (datalen > 0xffff) {
        return ERROR;
    }

    /* read the SPIRF numCount times  */

    for (i = numCount; i > 0; i--) {

        /* read the SPIRF always in 32 bit */

        readData = SPI_REG_READ_32 (SPIRF);

        /* handle the 8bit data */

        if (numCount == 0 && numChars != 0) {
            /* get the pointer of the byte*/

            pointer = (UINT8 *) &readData;
            do {
                *(UINT8 *) data++ = *pointer++;
            } while (numChars--);
        } else {

            /* read data with 32 bit directly stored into buffer */

            *(UINT32 *) data = readData;
            data += 4;
        }

        /*
         * delay for one IO READ cycle.
         * This delay is the experienced value but needed, to get high
         * performance, the delay can be optimized.
         */

        sysUsDelay (2);
        EIEIO_SYNC;    /* eieio, sync */
    }

    /* clear the RXCNT via enable eSPI */

    SPI_REG_WRITE_32 (SPMODE, SPI_REG_READ_32 (SPMODE) & (~SPI_EN));
    SPI_REG_WRITE_32 (SPMODE, SPI_REG_READ_32 (SPMODE) | (SPI_EN));
    return OK;
}

/*******************************************************************************
*
* sysSpiWriteData - write data to SPI device on poll mode
*
* The purpose of this function is to write data to SPI device on poll mode. The
* routine put the command as the data.
*
* RETURNS:  OK or ERROR if there is an error.
*/

UINT32 k = 0;

INT32 sysSpiWriteData
(
    UINT32 cs,
    UINT32 cmdlen,
    UINT32 datalen,
    UINT8 * cmd,
    UINT8 * data,
    UINT32 mode
)
{
    UINT32 command = 0;
    UINT32  gpioStatus;
    UINT32 dataCount = 0;
    UINT32 t = 0;

    UINT32 cmdCount = (cmdlen / 4 + ((cmdlen % 4 != 0) ? 1 : 0));

    if (data != NULL) {
        dataCount = (datalen / 4 + ((datalen % 4 != 0) ? 1 : 0));
    }

    command = SPCOM_CS (cs) | (cmdlen + datalen - 1) ;

    /*
     * if TRANSONLY=0, RxSKIP=0, the eSPI changes to full duplex mode
     * if TRANSONLY=1, RxSKIP is ignored, no reception, useful for write
     * transcations.
     * RxSKIP: Num of ch skipped for reception from frame start, Non-zero
     * values of RxSKIP force the eSPI to half-duplex mode.
     */

    command |= SPCOM_TO (mode);

    /* only write command, so skip the cmdlen */

    if (data == NULL) {
        command |= SPCOM_RxSKIP (cmdlen);
    }

    SPI_REG_WRITE_32 (SPCOM, command);
    k++;
    if( k > 1061000)
    {
        printf(" %08x ",*(volatile unsigned int *) data);
        if(k % 8 == 0x0)
            printf("\n");
    }

    while (dataCount--) {
        SPI_REG_WRITE_32 (SPITF, *(VUINT32 *) data);
        data += 4;
        /*
         * delay for one IO WRTIE data cycle.
         * This delay is the experienced value. To get high performance,
         * it could be optimized by reducing the delay.
         */
        sysUsDelay (10);
        EIEIO_SYNC;    /* eieio, sync */
    }
    return OK;
}

/*******************************************************************************
*
* cycloneFpgaRx - read Cyclone FPGA RAM data
*
* This is the spansion SPI flash data read out routine.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS cycloneFpgaRx
(
    UINT8  chipSelect,
    UINT32 startAddr,
    UINT32 dataLen,
    UINT8 * dest
)
{
    UINT32 cmd32;
    UINT32 len = 0;
    UINT32 count = dataLen / READ_MAX_LEN + (dataLen % READ_MAX_LEN ? 1 : 0);

    printf("count:%x\n",count);
    while (count --) {

        /* Compute the number of data ready to read for one read cycle */

        len = min (dataLen, READ_MAX_LEN);

        cmd32 = (startAddr & 0xffffff);
        
        /* Send the read command */
        
        if (sysSpiWriteData (chipSelect, 0, len, (UINT8*)&cmd32, 
                             NULL, NORMAL) != OK)
            return ERROR;

        if (sysSpiReadData (len, dest) != OK)
            return ERROR;

        startAddr += len;
        dest += len;
        dataLen -= len;
    }

    return OK;
}

/*******************************************************************************
*
* CycloneSpiXfer - SPI program fpga RAM
*
* RETURNS: OK or ERROR if there is an error.
*/

INT32 cycloneFpgaTx
(
    UINT8  chipSelect,
    UINT32 startAddr,
    UINT32  len,
    UINT8 * buf         /* program data buf */
)
{
    UINT32 cmd32;
    UINT32 dataLen = 0;
    UINT32 offset = 0;

    /* Probe the Spansion device */
    while (offset < len) {
        /* Compute the number of data ready to write for one write cycle */
        dataLen = min (len - offset, 4);
        cmd32 = (offset + startAddr) & 0xffffff;
        if (sysSpiWriteData (chipSelect, 0, dataLen, (UINT8*)&cmd32,
                             buf + offset, TRANSONLY) != OK)
            return ERROR;
        sysUsDelay(10);
        offset += dataLen;
    }

    return OK;
}

UINT32 rbf[1024];

void sysCycloneFlush()
{
    UINT32 chipSelect = 0;
    UINT32  i, j;
    UINT32  BinSize = 0x40c540;
    UINT8 * pSrc =CYCLONE_BIN_RAM_HIGH;
    UINT8 * pDest =CYCLONE_BIN_RAM_HIGH + 0x400000;
    UINT32  gpioStatus;
    UINT32 * aSrc = pSrc;
    UINT32 * aDst = pDest;
    UINT32 * srbf = rbf;

    for(i=0;i<1024;i++)
    {
    *(UINT32 *) srbf = 0x021bee01;
    srbf++;
    }

TryAgain:
    sysCycloneInit();
   
    sysGpioLineGet (&gpioStatus, GPIO_12);
    printf("gpioStatus:%x,%d\n",gpioStatus,__LINE__);
    sysGpioLineSet (GPIO_13, GPIO_LOW);
    sysUsDelay(10000);
    sysGpioLineGet (&gpioStatus, GPIO_12);
    printf("gpioStatus:%x,%d\n",gpioStatus,__LINE__);
    if(gpioStatus == 0x1)
        goto TryAgain;
    sysGpioLineSet (GPIO_13, GPIO_HIGH);
    sysUsDelay(10000);
    sysGpioLineGet (&gpioStatus, GPIO_12);
    printf("gpioStatus:%x,%d\n",gpioStatus,__LINE__);
    if(gpioStatus == 0x0)
        goto TryAgain;
   
    sysGpioLineGet (&gpioStatus, GPIO_11);
    printf("gpioConfigDone:%x,%d\n",gpioStatus,__LINE__);
    
    /* Program pData to SPI flash */
    UINT32 *preg = 0xffe07000;
    for(i=0;i<8;i++)
    {
    printf(" %x :-> %x \n", preg,*(UINT32 *)preg);
    preg++;
    }
TryTx:
    {
    if (cycloneFpgaTx(chipSelect, 0, BinSize, pSrc) != OK) {
        printf ("ERROR: spiFlash program failure. \r\n");
    }
    sysUsDelay(10000000);
    sysGpioLineGet (&gpioStatus, GPIO_12);
    printf("gpioStatus:%x,%d\n",gpioStatus,__LINE__);
    sysGpioLineGet (&gpioStatus, GPIO_11);
    printf("gpioConfigDone:%x,%d\n",gpioStatus,__LINE__);
    }
    if(gpioStatus == 0x0)
        goto TryAgain;
    return;
}
