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
 * 2015.10.12, Ezio created
 *
 * DESCRIPTION
 * test source file.
 */


#include "./demo.h"
#include <config.h>
#include <libc/stdio.h>
#include <p1020rdb.h>
#include <kernel/sysPic.h>
#include <ppc/toolPpc.h>

#include <kernel/sysOs.h>
#include "../driver/device/pcie/pcie.h"

void main(void)
{

    printf("3demo main\n");

    unsigned int t1,t2;

    dma_init();
    memset(0x100000,0x0c,0x19000);
    t1 = get_free();
    pcie_recv(1,0,0,0x400);
    t2=get_free();
    printf("%d %d %d-\n",t1,t2,t2-t1);

    t1 = get_free();
    pcie_recv(1,0,0,8096);
    t2=get_free();
    printf("%d %d %d+\n",t1,t2,t2-t1);

    memset(0xC100000,0x3c,0x19000);
    t1=getTblus();
    dma_memcpy(0xc100000,(uint8_t *)(C200000) ,2000);
    t2=getTblus();
    printf("%d %d %dX\n",t1,t2,t2-t1);

    memset(0xC100000,0x3c,0x19000);
    t1=getTblus();
    dma_memcpy(0xc100000,(uint8_t *)(FPGA_PCIE1_BAR0_BASE) ,2000);
    t2=getTblus();
    printf("%d %d %d$\n",t1,t2,t2-t1);

    memset(0x100000,0x2c,0x19000);
    t1 = get_free();
    dma_memcpy(0xc100000,(uint8_t *)(FPGA_PCIE1_BAR0_BASE) ,2000);
    t2=get_free();
    printf("%d %d %d^\n",t1,t2,t2-t1);
    
    memset(0x100000,0x1c,0x19000);
    t1 = get_free();
    dma_memcpy(0xc100000,(uint8_t *)(FPGA_PCIE1_BAR0_BASE) ,0x19000);
    t2=get_free();
    printf("%d %d %d-\n",t1,t2,t2-t1);
    printf("%x %x\n",*(unsigned int *)0x100000,*(unsigned int *)0x100010);
    dual_test();
}


#if 0
/*duart demo*/
    INT8 data[16]={0xa};
    memset(data,0xac,16);
    data[0]='0';
    data[1]='1';
    data[2]='2';
    data[3]='3';
    data[4]='4';
    data[5]='5';
    data[6]='6';
    data[7]='7';
    data[8]='8';
    data[9]='9';
    data[10]='a';
    data[11]='b';
    data[12]='c';
    data[13]='d';
    data[14]='e';
    data[15]='f';



    init_uart(1);
#if 0
    uart_send(2,16,data);
    printf("abcd\n");
    uart_send(2,16,data);
#endif

    while(1){

#if 1
recv:   if (uart_recv(1,16,data)==0){
            goto recv ;
        }
        printf("recv:");
        for(int i=0;i<16;i++){
            printf("%c:%x\n",data[i],data[i]);
        }
        printf("\n");
#endif
#if 1
send:   if (uart_send(1,16,data)==0)
            goto send;
        //printf("send\n");
        udelay(1400);
#endif

    }
   


    return 0;

#endif
