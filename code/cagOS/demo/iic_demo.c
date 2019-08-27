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
 * 2015.10.29, Ezio created
 *
 * DECRIPTION
 * I2C driver source file. with a simple driver to r/w rtc.
 *
 */
#include <libc/cagboot.h>
#include "../cagos.h"

#include <libc/rand.h>
#include <libc/string.h>
/*
 * rtc format
 * 0 sec(00-59)
 * 1 min(00-59)
 * 2 hour(00-23 / 01-12)  bit 6: 12, /24
 * 3 Days of the week(01-07)
 * 4 Dates (01-31)
 * 5 Months (01-12)
 * 6 Years (00-99)
 * 7 ctrl
 */

extern int demoRtcId;


int test_iic_demo(void)
{
    printf("test iic:rtc\n");

    unsigned char buf[100];

    buf[0]=rand()%60;
    buf[1]=(rand()+1)%60;
    buf[2]=(rand()+3)%60;

    printf("rtc write 0x%x 0x%x 0x%x\n",buf[0],buf[1],buf[2]);

    //写rtc
    dfs_write(demoRtcId,0,buf,3);

    //读rtc
    dfs_read(demoRtcId,0,buf,1);
    printf("rtc read 0x%x 0x%x 0x%x\n",buf[0],buf[1],buf[2]);
    return 0;

}
