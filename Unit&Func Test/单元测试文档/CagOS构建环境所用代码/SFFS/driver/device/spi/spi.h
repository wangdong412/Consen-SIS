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
 * spi driver header file.
 *
 */

#include "device_defs.h"
#include <libc/stdio.h>

#define MAX_TRANSFER_SIZE 0xFFFF

#define EVENT_RNE   1<<9
#define EVENT_TNF   1<<8
#define MODE_EN     1<<31
#define COM_CS(x)   ((x) << 30)
#define COM_TO      (1 << 27)
#define COM_RXSKIP(x)   ((x) << 16)
#define CS          0

typedef struct {
    unsigned int m_mode;
    unsigned int m_event;
    unsigned int m_mask;
    unsigned int m_com;
    unsigned int m_tx;
    unsigned int m_rx;
    unsigned int m_unused[2];
    unsigned int m_modex[4];
} Spi_t;


void spi_delay(void);
void spi_operation(unsigned int* in,unsigned int insize, unsigned int* out, unsigned int outsize);

int sf_erase(int no,int num);

int sf_erase_chip(void);
int sf_erase_sector(unsigned int iSector);
int sf_write(unsigned int addressInFlash, unsigned char* pBuf,unsigned int size);
int sf_read(unsigned int adrInFlash, unsigned char* pBuf, unsigned int size);
