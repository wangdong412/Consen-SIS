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
*  文件名称    : SharedRetain.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-08-15
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
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/SharedRetain.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/



/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/




/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedRetainInit
*
* 功能描述: 初始化
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
void SharedRetainInit(void)
{
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetTask0RetainData
*
* 功能描述: 将Task0的retain数据写入SRAM
*
* 输入参数: pucBuf：指向待写入数据起始地址
*        usLen：待写入数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedSetTask0RetainData(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint16_t pusTask0RetainStartAddr = NULL;
    uint16_t i = 0;
    uint16_t usNum = 0;
    puint16_t pusBuf = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pusTask0RetainStartAddr = (puint16_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_TASK0_OFFSET);
        //memcpy(pucTask0RetainStartAddr, pucBuf, usLen);

        usNum = usLen / 2;
        pusBuf = (puint16_t)pucBuf;
        for(i = 0; i < usNum; i++)
        {
            pusTask0RetainStartAddr[i] = pusBuf[i];
        }

        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetTask1RetainData
*
* 功能描述: 将Task1的retain数据写入SRAM
*
* 输入参数: pucBuf：指向待写入数据起始地址
*        usLen：待写入数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedSetTask1RetainData(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint16_t pusTask1RetainStartAddr = NULL;
    uint16_t i = 0;
    uint16_t usNum = 0;
    puint16_t pusBuf = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pusTask1RetainStartAddr = (puint16_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_TASK1_OFFSET);
        //memcpy(pucTask1RetainStartAddr, pucBuf, usLen);

        usNum = usLen / 2;
        pusBuf = (puint16_t)pucBuf;
        for(i = 0; i < usNum; i++)
        {
            pusTask1RetainStartAddr[i] = pusBuf[i];
        }

        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTask0RetainData
*
* 功能描述: 将Task0的retain数据从SRAM读出
*
* 输入参数: pucBuf：指向待读出数据起始地址
*        usLen：待读出数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-读出成功
*        false-读出失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedGetTask0RetainData(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint8_t pucTask0RetainStartAddr = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pucTask0RetainStartAddr = (puint8_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_TASK0_OFFSET);
        memcpy(pucBuf, pucTask0RetainStartAddr, usLen);
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTask1RetainData
*
* 功能描述: 将Task1的retain数据从SRAM读出
*
* 输入参数: pucBuf：指向待读出数据起始地址
*        usLen：待读出数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-读出成功
*        false-读出失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedGetTask1RetainData(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint8_t pucTask1RetainStartAddr = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pucTask1RetainStartAddr = (puint8_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_TASK1_OFFSET);
        memcpy(pucBuf, pucTask1RetainStartAddr, usLen);
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SRAMtest
*
* 功能描述: 测试程序
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
void SRAMtest(void)
{
    uint8_t ucBuf[8]={11,12,13,14,15,16,17,18};
    uint8_t ucBuf2[10];
    uint16_t i = 0;
    uint16_t len = 8;

    SharedGetTask0RetainData(&ucBuf2[0], len);

    for(i=0;i<len;i++)
    {
        printf("%x ", ucBuf2[i]);
    }
    printf("\n");

    SharedSetTask0RetainData(&ucBuf[0], len);

    SharedGetTask0RetainData(&ucBuf2[0], len);

    for(i=0;i<len;i++)
    {
        printf("%x ", ucBuf2[i]);
    }
    printf("\n");

    SharedGetTask1RetainData(&ucBuf2[0], len);

    for(i=0;i<len;i++)
    {
        printf("%x ", ucBuf2[i]);
    }
    printf("\n");

    SharedSetTask1RetainData(&ucBuf[0], len);

    SharedGetTask1RetainData(&ucBuf2[0], len);

    for(i=0;i<len;i++)
    {
        printf("%x ", ucBuf2[i]);
    }
    printf("\n");

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDisableTabl
*
* 功能描述: 将Disable表写入SRAM
*
* 输入参数: pucBuf：指向待写入数据起始地址
*        usLen：待写入数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedSetDisableTabl(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint16_t pusDisableTablStartAddr = NULL;
    uint16_t i = 0;
    uint16_t usNum = 0;
    puint16_t pusBuf = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pusDisableTablStartAddr = (puint16_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_DISABLE_OFFSET);

        usNum = usLen / 2;
        pusBuf = (puint16_t)pucBuf;
        for(i = 0; i < usNum; i++)
        {
            pusDisableTablStartAddr[i] = pusBuf[i];
        }

        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDisableTabl
*
* 功能描述: 将Disable表从SRAM读出
*
* 输入参数: pucBuf：指向待读出数据起始地址
*        usLen：待读出数据长度
*
* 输出参数: 无
*
* 返 回 值  : true-读出成功
*        false-读出失败
*
* 注意事项: usLen必须为偶数
***********************************************************************************************************************
*/
bool_t SharedGetDisableTabl(puint8_t pucBuf, uint16_t usLen)
{
    bool_t bRet = false;
    puint8_t pucDisableTablStartAddr = NULL;

    if((pucBuf != NULL) && (usLen > 0) && (0 == (usLen % 2)))
    {
        pucDisableTablStartAddr = (puint8_t)(RETAIN_SRAM_SEC4_START_ADDR + RETAIN_SRAM_DISABLE_OFFSET);
        memcpy(pucBuf, pucDisableTablStartAddr, usLen);
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
