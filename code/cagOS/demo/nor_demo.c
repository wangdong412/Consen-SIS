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
#include "../driver/device/norflash/lld.h"


extern struct flash_struct nor_demo_st;

extern int demo_dfd1,demo_dfd2;
unsigned int buf[128*1024]={0};
unsigned int buf2[128*1024]={0};
#define getTblus get_free
int do_demo_nor1()
{
    int ret =0,i;
    long t1 ,t2;

    ret = lld_erase(240,1);
    if(ret <0){
        printf("erase fail\n");
        return 0;
    }

    ret = lld_write(0xede00000,buf,128*1024);
    if(ret != 128*1024){
        printf("ret %d\n",ret);
    }
    ret = lld_read(0xede00000,buf2,128*1024);
    if(ret != 128*1024){
        printf("ret %d\n",ret);
    }
    for(int i=1000;i<1020;i++){
        printf("%d ",buf2[i]);
    }
    printf(" Aok ret %d\n",ret);

        return 0;
    ret = lld_erase(2,40);
    if(ret <0){
        printf("erase fail\n");
        return 0;
    }
    return 0;
    for(i=0;i<0x20000/300*40;i++){

        ret = lld_write(0xec040000+i*300,buf,300);
        if(ret != 300){
            printf("ret %d\n",ret);
        }
    }
    printf("done1\n");
    return 0;

    for(i=0;i<0x20000/0x600;i++){
        t1 = getTblus();
        lld_write(0xEDE00000+i*0x600,buf,224);
        t2 = getTblus();
        printf("224 t %d %d %d\n",t2,t1,t2-t1);

        t1 = getTblus();
        lld_write(0xEDE00100+i*0x600,buf,196);
        t2 = getTblus();
        printf("196 t %d %d %d\n",t2,t1,t2-t1);

        t1 = getTblus();
        lld_write(0xEDE00200+i*0x600,buf,192);
        t2 = getTblus();
        printf("192 t %d %d %d\n",t2,t1,t2-t1);

        t1 = getTblus();
        lld_write(0xEDE00300+i*0x600,buf,64);
        t2 = getTblus();
        printf("64 t %d %d %d\n",t2,t1,t2-t1);

        t1 = getTblus();
        lld_write(0xEDE00400+i*0x600,buf,32);
        t2 = getTblus();
        printf("32 t %d %d %d\n",t2,t1,t2-t1);
    }
    while(1);


    t1 = getTblus();
    lld_write(0xEDE00200,buf,320);
    t2 = getTblus();
    printf("320 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00400,buf,300);
    t2 = getTblus();
    printf("300 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00600,buf,240);
    t2 = getTblus();
    printf("240 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00800,buf,224);
    t2 = getTblus();
    printf("224 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00a00,buf,200);
    t2 = getTblus();
    printf("200 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00c00,buf,196);
    t2 = getTblus();
    printf("196 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE00e00,buf,192);
    t2 = getTblus();
    printf("192 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE01000,buf,180);
    t2 = getTblus();
    printf("180 t %d %d %d\n",t2,t1,t2-t1);

    t1 = getTblus();
    lld_write(0xEDE01200,buf,160);
    t2 = getTblus();
    printf("160 t %d %d %d\n",t2,t1,t2-t1);
    while(1);
}
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
