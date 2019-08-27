/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 
*
*  文件名称    : Ver.c
*
*  功能简介    : 
*
*  作者            : 李琦
*
*  创建日期    : 2016-07-14
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/Ver.h"
#include "Srv/Sys/inc/CommSys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
const static uint8_t s_ucPMVersion[]={"PM version: 2017/08/17"};
const static uint8_t s_ucCore0Version[]={"CORE0 version: 2017/08/17"};
const static uint8_t s_ucCore1Version[]={"CORE1 version: 2017/08/17"};

const uint8_t s_ucPMVersion1[4]={1,0,0,37};
const uint8_t s_ucOSVersion1[4]={1,0,0,25};
uint8_t s_ucSN[14]={0};

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void ShowfpgaVer(void);
static void SetSN(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ShowVersion
*
* 功能描述: 显示版本号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void ShowVersion(void)
{
    printf("%s\n", &s_ucPMVersion[0]);
    printf("%s\n", &s_ucCore0Version[0]);
    printf("%s\n", &s_ucCore1Version[0]);
    printf("OS version: %s\n", os_version());
    ShowfpgaVer();
    SetSN();

    return;
}
/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
static void ShowfpgaVer(void)
{
    uint16_t usVal = 0;
    
    usVal = SysGetFPGAModID();
    printf("FPGA MOD ID: 0x%x\n", usVal);
    usVal = SysGetFPGAHardwareVer();
    printf("FPGA HARDWARE VERSION: 0x%x\n", usVal);
    usVal = SysGetFPGAChipID();
    printf("FPGA MICROCHIP ID: 0x%x\n", usVal);
    usVal = SysGetFPGAFirmwareVer();
    printf("FPGA FIRMWARE VERSION: 0x%x\n", usVal);
    usVal = SysGetFPGAFirmwareSubVer();
    printf("FPGA FIRMWARE SUB VERSION: 0x%x\n", usVal);
    usVal = SysGetMCUModID();
    printf("MCU MOD ID: 0x%x\n", usVal);
    usVal = SysGetMCUHardwareVer();
    printf("MCU HARDWARE VERSION: 0x%x\n", usVal);
    usVal = SysGetMCUChipID();
    printf("MCU MICROCHIP ID: 0x%x\n", usVal);
    usVal = SysGetMCUFirmwareVer();
    printf("MCU FIRMWARE VERSION: 0x%x\n", usVal);

    return;
}

static void SetSN(void)
{
    uint16_t usVal = 0;
    uint8_t data[32];
    uint32_t datalen = 32;

    lld_read (VER_SN_ADDR, &data[0], datalen);
    memcpy(&s_ucSN[0], &data[18], 14);

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
