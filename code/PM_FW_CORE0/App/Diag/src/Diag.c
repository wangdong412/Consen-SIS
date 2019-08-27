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
*  文件名称    : Diag.c
*
*  功能简介    : 
*
*  作者          : 李琦
*
*  创建日期    : 2016-08-01
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
#include <Srv/Scheduler/include/string.h>
#include "../inc/Diag.h"
#include "Srv/Diag/inc/CPUTest.h"
#include "Srv/Diag/inc/CodeTest.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Diag/inc/BoardDiag.h"
#include "Srv/Diag/inc/DataCacheTest.h"
#include "Srv/Sys/inc/CommSys.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint32_t s_uiVectorTest=0xFFFFFFFFU;
static char temp[140];
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void os_switch(void);
static void CreateSwitchTask(void);
static void PassOrFailLEDUpdate(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: DiagInit
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
void DiagInit(void)
{
    s_uiVectorTest=0xFFFFFFFFU;
    DataCacheTestInit();
    CPUTestInit();
    CpuTestPowerOn(&s_uiVectorTest);
    printf("Power on test %d\n",s_uiVectorTest);
    PassOrFailLEDUpdate();
}

/*
***********************************************************************************************************************
* 函数名称: DiagCycle
*
* 功能描述: 周期运行函数
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
void DiagCycle(void)
{
    static bool_t bSignal0 = false;
    static bool_t bSignal1 = false;
    static bool_t bSignal2 = false;

    bool_t bRet1 = false;
    bool_t bRet2 = false;
    uint32_t uiSignal = 0;

    //printf("cpu test start\n");
    CPUTestCycle(&s_uiVectorTest);

    //DataCacheTestCycle(&s_uiVectorTest);
#if 0
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(5u);
    }while(true != bRet1);

    if(SIGNAL_CORE1_DIAG_DONE0 == uiSignal)
    {
        bSignal0 = true;
        do
        {
            bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE0_DIAG_DONE0);
            udelay(5u);
        }while(true != bRet2);
    }
    else if(SIGNAL_CORE1_DIAG_DONE1 == uiSignal)
    {
        bSignal1 = true;
        do
        {
            bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE0_DIAG_DONE1);
            udelay(5u);
        }while(true != bRet2);
    }
    else if(SIGNAL_CORE1_DIAG_DONE2 == uiSignal)
    {
        bSignal2 = true;

        if((true == bSignal0)&&(true == bSignal1)&&(true == bSignal2))
        {
            do
            {
                bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE0_DIAG_DONE2);
                udelay(5u);
            }while(true != bRet2);

            CreateSwitchTask();
            bSignal0 = false;
            bSignal1 = false;
            bSignal2 = false;
        }
    }
    else
    {
        bSignal0 = false;
        bSignal1 = false;
        bSignal2 = false;
    }
#endif
    PassOrFailLEDUpdate();
}


/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: os_switch
*
* 功能描述: 内存拷贝及切换
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
static void os_switch(void)
{
    powerpcIntDisable();
    //printf("start0\n");
    os_copy_data(0x5000000,0x1000000);
    os_copy_stack();
    os_change_mmu();
    os_update_mno();
    //printf("end0\n");
    powerpcIntEnable();
}
/*
***********************************************************************************************************************
* 函数名称: CreateSwitchTask
*
* 功能描述: 创建内存拷贝及切换任务
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
static void CreateSwitchTask(void)
{
    int err = 0;
    //char *temp=0x4300000;
    OSTaskCreate(temp,
            "switchos",
            os_switch,
            NULL,
            1,
            0x4100000,
            0x100,
            0x200,
            0,
            0,
            0x4100000+0x1000,
            1|2,
            &err);
}

/*
***********************************************************************************************************************
* 函数名称: PassOrFailLEDUpdate
*
* 功能描述: 刷新pass和fault状态灯
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
static void PassOrFailLEDUpdate(void)
{
    if(MASK_SETALL == s_uiVectorTest)
    {
        if((MCU_OK == SysGetMCUMonitorState()) \
          && (CLK_DIAG_OK == SysGetClkDiagVal()))
        {
            /* diag pass */
            SetLEDState(LED_NAME_PASS, LED_ON);
            /* fault LED指示状态灯操作 */
            SetLEDState(LED_NAME_FAULT, LED_OFF);

            SysSetAlarmLED(ALARM_OFF);
        }
        else
        {
            /* diag fail */
            SetLEDState(LED_NAME_PASS, LED_OFF);
            /* fault LED指示状态灯操作 */
            SetLEDState(LED_NAME_FAULT, LED_ON);

            SysSetAlarmLED(ALARM_ON);
        }
    }
    else
    {
        /* diag fail */
        SetLEDState(LED_NAME_PASS, LED_OFF);
        /* fault LED指示状态灯操作 */
        SetLEDState(LED_NAME_FAULT, LED_ON);

        SysSetAlarmLED(ALARM_ON);

        DiagCommSetCore0FailFlag();

        while(1)//死机
        {
            printf("Diag failed.\n");
        }
    }
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
