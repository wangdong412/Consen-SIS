/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  shell cmd code for PPC85XX.
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
 * 2015.10.10, Ezio created
 *
 * DESCRIPTION
 * nor test source file.
 */


#include "./demo.h"
#include <drv_config.h>
#include <libc/rand.h>
#include "../driver/device/dfs/dfs.h"


extern struct flash_struct nor_demo_st;

extern int demo_dfd1,demo_dfd2;
int do_demo_nor(void)
{
    printf("test nor\n");
    int i=0;
    INT32 t,step,size;
    UINT8 buf[32*1024];

    t=0;
    step = nor_demo_st.ssSize;
    size = nor_demo_st.seSize;
    for(int i =0; i<step; i++)
        buf[i]=(rand()*2)%256;

    //对flash 的第一部分进行操作
    //擦除第i 个sector
    dfs_erase(demo_dfd1,i,1);

    //对第一部分第i 个sector 进行write-read 操作，每次操作step 字节
    printf("ori %x %x %x %x\n",buf[0],buf[1024],buf[2345],buf[step-1]);

    //写第一部分的flash
    if ( dfs_write(demo_dfd1,t*step,buf,step) != step) {
        printf("write fail addr 0x%x len %d pos 0x%x\n",i*size,step,t*step);
    }
    memset(&buf[step],0,step);
    //读第一部分的flash
    if ( dfs_read(demo_dfd1,t*step,&buf[step],step) != step) {
        printf("read fail addr 0x%x len %d pos 0x%x\n",i*size,step,t*step);
    }
    printf("red %x %x %x %x\n",buf[0+step],buf[1024+step],buf[2345+step],buf[step-1+step]);

    /*****************************************************************************/

    //对flash的第二部分进行操作
    i=0;
    t=0;
    //擦除第二部分的第i 个sector
    dfs_erase(demo_dfd2,i,1);

    //对第i 个sector 进行write-read 操作，每次操作step 字节
    printf("ori %x %x %x %x\n",buf[0],buf[1024],buf[2345],buf[step-1]);

    //写第二部分的flash
    if ( dfs_write(demo_dfd2,t*step,buf,step) != step) {
        printf("write fail addr 0x%x len %d pos 0x%x\n",i*size,step,t*step);
    }
    memset(&buf[step],0,step);
    //读第二部分的flash
    if ( dfs_read(demo_dfd2,t*step,&buf[step],step) != step) {
        printf("read fail addr 0x%x len %d pos 0x%x\n",i*size,step,t*step);
    }
    printf("red %x %x %x %x\n",buf[0+step],buf[1024+step],buf[2345+step],buf[step-1+step]);
    return 0;
}
