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
//#include "../cagos.h"

#include "../driver/device/norflash/lld.h"
#include "../driver/device/dfs/dfs.h"
#include "./demo.h"
#include <drv_config.h>
#include <libc/rand.h>
//#include "../driver/device/iic/iic.h"
#include <libc/string.h>
//#include "../driver/device/spi/spi.h"

struct dfs_ops nor_demo_ops; //s29gl512pOps;     //logical info
struct dfs_ops nor_demo_ops2;
struct flash_struct nor_demo_st;//s29gl512pSt; //physical info

extern int nor_flash_errno;
int nor_demo_did= 0;

int demo_dfd1= 0,demo_dfd2 = 0;

#ifdef CONFIG_NOR_FLASH
void nor_init_demo(void)
{
    printf("hello nor demo\n");
    //spasion nor flash init
    lld_InitCmd((UINT16 *)0xec000000);

    //将nor flash 分为两部分，分别配置、操作
    memset(&nor_demo_ops,0,sizeof(nor_demo_ops));
    memset(&nor_demo_ops2,0,sizeof(nor_demo_ops2));

    //定义flash 物理信息
    nor_demo_st.seSize  =   SE_SIZE;//4*1024;
    nor_demo_st.seNum   =   SE_NUM;

    nor_demo_st.ssSize      =   SS_SIZE; //128*1024;
    nor_demo_st.ssNum       =   SS_NUM;

    nor_demo_st.totalSize   =   DEVICE_SIZE;

    /***********************************/
    //初始化nor flash 两部分对应的dfs_ops 结构体
    nor_demo_ops.dname     =  (unsigned char *)"s29gl512p_1\0";
    //管理实际的驱动
    nor_demo_ops.write     =   lld_write;
    nor_demo_ops.read      =   lld_read;
    nor_demo_ops.erase     =   lld_erase;
    //配置flash 参数
    nor_demo_ops.base_addr =   FLASH_BASE_ADDR;
    nor_demo_ops.size      =   20*1024*1024;
    nor_demo_ops.start     =   1024*1024;
    nor_demo_ops.end       =   nor_demo_ops.start  + nor_demo_ops.size;
    nor_demo_ops.errno     =   &nor_flash_errno;
    nor_demo_ops.data      =   (struct flash_struct *)&nor_demo_st;

    /**********************************/
    //限制第二部分对flash 只有只读权限
    nor_demo_ops2.dname    =    (unsigned char *)"s29gl512p_2\0";
//    nor_demo_ops2.write     =   lld_write;
    nor_demo_ops2.read      =   lld_read;
//    nor_demo_ops2.erase     =   lld_erase;
    nor_demo_ops2.base_addr =   FLASH_BASE_ADDR;
    nor_demo_ops2.size      =   10*1024*1024;
    nor_demo_ops2.start     =   30*1024*1024;
    nor_demo_ops2.end       =   nor_demo_ops.start  + nor_demo_ops.size ;
    nor_demo_ops2.errno     =   &nor_flash_errno;
    nor_demo_ops2.data      =   (struct flash_struct *)&nor_demo_st;

    //注册dfs_ops 结构体，获取dfs id
    demo_dfd1 = dfs_register(&nor_demo_ops);
    demo_dfd2 = dfs_register(&nor_demo_ops2);

    printf("dfd 1 %d 2 %d\n",demo_dfd1,demo_dfd2);
    int ret;
    ret = dfs_cap(demo_dfd1,DFS_WRITE);
    printf("nor 1 can ? write %d\n",ret);

    ret = dfs_cap(demo_dfd2,DFS_WRITE);
    printf("nor 2 can ? write %d\n",ret);
}
#endif

#ifdef CONFIG_IIC
extern void fsl_i2c_init(int id, int speed, int slaveadd);

struct dfs_ops demoRtcOps;
int demoRtcId;
void iic_init_demo(void)
{
    //init i2c controller
    fsl_i2c_init(0,CONFIG_SYS_FSL_I2C_SPEED,CONFIG_SYS_FSL_I2C_SLAVE);

    memset(&demoRtcOps,0,sizeof(demoRtcOps));

    //初始化i2c 的dfs_ops 结构体
    //关联实际的驱动
    demoRtcOps.write = rtc_write;
    demoRtcOps.read = rtc_read;
    //配置外设参数
    demoRtcOps.size = 32;
    demoRtcOps.start = 0;
    demoRtcOps.end = 31;
    demoRtcOps.base_addr = 0;

    //注册i2c 设备
    demoRtcId = dfs_register(&demoRtcOps);

    int ret;
    ret = dfs_cap(demoRtcId,DFS_ERASE);
    printf("iic can ? erase %d\n",ret);
    return ;
}
#endif

#ifdef CONFIG_SPI_FLASH
struct dfs_ops spi_demo;//s25fl128sOps;
struct flash_struct spiflash_demo;//s25fl128sSt;
int demoSfDid = 0;

int demoDfd =0;

void spi_init_demo(void)
{
    memset(&spi_demo,0,sizeof(spi_demo));
    memset(&spiflash_demo,0,sizeof(spiflash_demo));


    //定义spasion spi flash 的结构体
    spiflash_demo.seSize      =   BYTES_PER_SECTOR;
    spiflash_demo.seNum       =   256;
    spiflash_demo.ssSize     =   BYTES_PER_PAGE;
    spiflash_demo.ssNum      =   256;
    spiflash_demo.totalSize      =   16*1024*1024;

    //初始化spi flash 的dfs_ops 结构体
    spi_demo.dname          =   (UINT8 *)"s25fl128s\n";
    spi_demo.write          =   sf_write;
    spi_demo.read           =   sf_read;
    spi_demo.erase          =   sf_erase;
    spi_demo.base_addr      =   0;
    spi_demo.start          =   0;
    spi_demo.size           =   16*1024*1024;
    spi_demo.end            =   spi_demo.start +spi_demo.size;
    spi_demo.data           =   (struct flash_struct *)&spiflash_demo;


    //注册spi 设备
    demoDfd = dfs_register(&spi_demo);
    demoSfDid = demoDfd;
    if (demoDfd <0) {
        printf("sf ops register fail %d\n",demoDfd);
        return ;
    }

    int ret;
    ret = dfs_cap(demoSfDid,DFS_ERASE);
    printf("spi can ? erase %d\n",ret);
    return ;
}
#endif

