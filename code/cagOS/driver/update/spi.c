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
#include "spi.h"

#define TIMEOUT_SPI 300
extern void sysUsDelay (int delay);

void spi_delay(void)
{
    sysUsDelay(20);
}
/*******************************************************************************
*
* spi_operation - read and write data via spi
* unsigned int *in:  send buffer
* unsigned int insize: send length
* unsigned int *out: receive buffer
* unsigned int outsize: receive length
*
*
* RETURNS: N/A
*
*/
void spi_operation(unsigned int* in,unsigned int insize, unsigned int* out, unsigned int outsize)
{
    unsigned int event, tmpout, tmpin;
    unsigned int len = insize + outsize;
    unsigned int numBlks = len / 4 + (len % 4 ? 1 : 0);
    unsigned int charsize = 4;
    unsigned int command = 0;

    Spi_t* pSpi = (Spi_t*)(SPI_REG_BASE);

    command |= COM_CS(CS);
    command |= insize + outsize -1 ;

    if ( outsize == 0 )
        command |= COM_TO;
    else {
        command &= ~COM_TO;
        command |= COM_RXSKIP(insize);
    }

    pSpi->m_com = command;

    /*clear any previous events.*/
    pSpi->m_event = 0xFFFFFFFF;

    while (numBlks--) {
        charsize = (len >=4 ? 4: len);

        event = pSpi->m_event;
        if (event & EVENT_TNF) {
            /*send data in buffer in to SPI through m_tx*/
            tmpin = *(unsigned int*)in;
            /*setup next iteration if sending > 4 bytes*/
            if (len > 4) {
                len -= 4;
                in += 1;
            }
            pSpi->m_tx = tmpin;
            pSpi->m_event |= EVENT_TNF;
        }

        /*Wait for data to transmit....*/
        spi_delay();

        event = pSpi->m_event;
        if (event & EVENT_RNE) {
            /*get data from SPI through m_rx and store it to out*/
            tmpout = pSpi->m_rx;
            *(unsigned int*) out = tmpout;
            if (charsize == 4 ) {
                out += 1;
            }
            pSpi->m_event |=EVENT_RNE;
        }
    }

    pSpi->m_mode &= ~MODE_EN;
    pSpi->m_mode |= MODE_EN;
}

/*******************************************************************************
*
* sf_read - read data from spi flash
*
* unsigned int adrInFlash: address in spi flash
*
* RETURNS: actual read data length.or return -1 if the operation is failed.
*
*/
int sf_read(unsigned int adrInFlash, unsigned char* pBuf, unsigned int size)
{
    unsigned char cmd[4];
    spi_operation((unsigned int*)cmd,0,(unsigned int*)pBuf,size);
    return size;
}
/*******************************************************************************
*
* sf_write - write data to spi flash
*
* unsigned int adrInFlash: address in spi flash
*
* RETURNS: actual write data length.or return -1 if the operation is failed.
*
*/
int sf_write(unsigned int addressInFlash, unsigned char* pBuf,unsigned int size)
{
    /*program*/
    spi_operation((unsigned int*)pBuf,size,0,0);
    return size;
}
