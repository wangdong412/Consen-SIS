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
 * 2015.10.23, Ezio created
 *
 * DECRIPTION
 * spi driver source file. contain a simple spi flash driver.
 *
 */

#include <libc/cagboot.h>
#include "../cagos.h"
#include "./demo.h"
//#include "../driver/device/dfs/dfs.h"
//#include "../driver/device/spi/device_defs.h"


extern int demoSfDid ;
extern int demoDfd ;

int spi_test_demo(void)
{
    return 0;
    int ret;
    unsigned char buf[1024];
    memset(buf,0,sizeof(buf));

    //read spi flash
    ret = dfs_read(demoSfDid,0x100000,buf,128);
    printf("read 0 ret = %d \n",ret);
    for(int i =0; i<128; i++) {
        printf("0x%x ",buf[i]);
    }
    printf("\n");

    //ret = sf_erase_chip();
    //printf("ea 0x%x\n",ret);

    //erase one sector
    ret = dfs_erase(demoSfDid,0,1);
    printf("sf erase %d ret %d\n",0,ret);

    //write spi flash
    memset(buf,0,sizeof(buf));
    ret = dfs_write(demoSfDid,0,buf,128);
    printf("sf write %d bytes %d ret %d\n",0,128,ret);

    //read spi flash
    ret = dfs_read(demoSfDid,0,buf,128);
    printf("read 1 ret = %d \n",ret);
    for(int i =0; i<128; i++) {
        printf("0x%x ",buf[i]);
    }
    printf("\n");

    return 0;
}
