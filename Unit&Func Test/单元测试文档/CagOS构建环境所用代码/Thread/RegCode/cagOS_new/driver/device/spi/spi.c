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

void sf_id(void)
{
    unsigned char cmd = RDID;
    unsigned int id[5];
    /*write one byte, read 3*/
    spi_operation((unsigned int *)&cmd, 1, id, 5);
    printf("id 0x%x 0x%x 0x%x 0x%x 0x%x\n",id[0],id[1],id[2],id[3],id[4]);
    return;
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
    /*Watch it! If size > FFFF it overflows the 16 bits it has alocated */
    if (size > MAX_TRANSFER_SIZE)
        return -1;

    cmd[0]  =   READ;
    cmd[1]  =   ((unsigned char*)(&adrInFlash))[1];
    cmd[2]  =   ((unsigned char*)(&adrInFlash))[2];
    cmd[3]  =   ((unsigned char*)(&adrInFlash))[3];

    spi_operation((unsigned int*)cmd,4,(unsigned int*)pBuf,size);
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
    volatile unsigned int i;
    int protect = 0;
    unsigned char cmd[4 + BYTES_PER_PAGE];
    unsigned char reply[1];

    /*Watch it! If size > BYTES_PER_PAGE operation will fail */
    if (size > BYTES_PER_PAGE)
        return -1;

    cmd[0]  =   RDSR;
    spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);
    protect = (reply[0] & BLOCKLOCKMASK) >> 2;
    printf("wsr 0x%x\n",reply[0]);

    if (protect & ALL_SECTORS) {
        printf("protected addr 0x%x va 0x%x\n",addressInFlash,reply[0]);
        return -2;
        while(1);
    }
    /*setting "Write Enable"*/
    cmd[0]  =   WREN;
    spi_operation((unsigned int*)cmd,1,0,0);

    cmd[0]  =   PP;
    cmd[1]  =   ((unsigned char*)(&addressInFlash))[1];
    cmd[2]  =   ((unsigned char*)(&addressInFlash))[2];
    cmd[3]  =   ((unsigned char*)(&addressInFlash))[3];
    for ( i=0; i < size; i++ ) {
        cmd[4+i]    =   pBuf[i];
    }

    /*program*/
    spi_operation((unsigned int*)cmd,4+size,0,0);

    for ( i=0; i <= MAX_WAIT_COMMAND; i++) {
        cmd[0]  =   RDSR;
        spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);

        if((reply[0] & WIP)==0)
            break;
    }
    /*operation*/
    if (i >= MAX_WAIT_COMMAND) {
        printf("write not finish\n");
        while(1);
        return -5;
    }
    return size;
}
/*******************************************************************************
*
* sf_erase_sector - erase spi flash
*
* unsigned int iSector: sector number of spi flash
*
* RETURNS: return 0 if successfully, return -1 if failed
*
*/
int sf_erase_sector(unsigned int iSector)
{
    unsigned int addressInFlash;
    unsigned int i;
    int rc = 0;
    unsigned char cmd[4];
    unsigned char reply[1];
    int protect = 0;

    addressInFlash  =   BYTES_PER_SECTOR*iSector;

    cmd[0]  =   RDSR;
    spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);
    printf("esr 0x%x\n",reply[0]);

    protect = (reply[0] & BLOCKLOCKMASK) >> 2;

    if (protect & ALL_SECTORS) {
        printf("1 protected ino 0x%x va 0x%x\n",iSector,reply[0]);
        while(1);
        return -1;
    }

    cmd[0]  =   WREN;
    spi_operation((unsigned int*)cmd,1,0,0);


    cmd[0]  =   RDSR;
    spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);

    cmd[0]  =   SE;
    cmd[1]  =   ((unsigned char*)(&addressInFlash))[1];
    cmd[2]  =   ((unsigned char*)(&addressInFlash))[2];
    cmd[3]  =   ((unsigned char*)(&addressInFlash))[3];

    spi_operation((unsigned int*)cmd,4,0,0);

    for(i=0; i<=MAX_ERASE_TIME; i++) {
        cmd[0]  =   RDSR;
        spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);

        /*check "write in progress" status bit*/
        if((reply[0] & WIP)==0)break;
    }

    if(i>=MAX_ERASE_TIME) {
        return -2;
    }

    return rc;
}
int sf_erase(int no,int num)
{
    if (no <0 )
        return -1;
#if 0
    if(num < SECTOR_NUM) {
        for( i=0; i<num; i++ ) {
            ret = lld_SectorEraseOp((FLASHDATA *)FLASH_BASE_ADDR, no*LLD_SECTOR_SIZE);
            if (ret != DEV_NOT_BUSY)
                return -2;
        }
    } else if(num == SECTOR_NUM) {
        ret = lld_ChipEraseOp((FLASHDATA *)FLASH_BASE_ADDR);
        if (ret != DEV_NOT_BUSY)
            return -3;
    }
#endif
    return sf_erase_sector(no);
}

int sf_erase_chip(void)
{
    unsigned int i;
    int rc = 0;
    unsigned char cmd[4];
    unsigned char reply[1];
    int protect = 0;

    cmd[0]  =   RDSR;
    spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);
    protect = (reply[0] & BLOCKLOCKMASK) >> 2;
    if (protect != 0) { /*we have protected sectors*/
        printf("erase procted\n");
        return -1;
    }

    /*write Enable*/
    cmd[0]  =   WREN;
    spi_operation((unsigned int*)cmd,1,0,0);

    /*bulk erase*/
    cmd[0]  =   BE;
    spi_operation((unsigned int*)cmd,1,0,0);

    for (i=0; i<=MAX_ERASE_TIME; i++) {
        cmd[0]  =   RDSR;
        spi_operation((unsigned int*)cmd,1,(unsigned int*)reply,1);

        /*check "write in progress" status bit*/
        if((reply[0] & WIP)==0)
            break;
    }

    if (i>=MAX_ERASE_TIME) {
        printf("erase fail\n");
        return -2;
    }

    return rc;
}
