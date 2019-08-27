/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 状态灯模块
*
*  文件名称    : LED.c
*
*  功能简介    :
*
*  作者        : 李琦
*
*  创建日期    : 2016-01-13
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
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <error.h>
#include <dirent.h>
#include <string.h>
#include <byteswap.h>
#include <linux/fs.h>
#include "../inc/LED.h"
#include "Sys/inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static LEDState_t s_emLEDStatus[NUM_OF_LED];

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void LEDInit(void);

static void UpdateLEDRunningState(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: LEDManagerInit
*
* 功能描述: 状态灯管理初始化
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
void LEDManagerInit(void)
{
    LEDInit();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: LEDManagerCycle
*
* 功能描述: 状态灯管理周期运行函数
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
void LEDManagerCycle(void)
{
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;         /*控制站配置信息可写片段指针*/
    ProjVerInfo_t stProjVer;

    /* 临时增加CMBUS灯 */
    if(((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMA)) ||
            ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMB)) ||
            ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMC)))
    {
        SetLEDState(LED_NAME_CMBUS, LED_BLINK);
    }
    else
    {
        SetLEDState(LED_NAME_CMBUS, LED_OFF);
    }

    /*获取控制站配置信息可写片段指针*/
    pstCtrlStationInfo = CMCfgGetCtrlStationInfo();

    memset(&stProjVer, 0,sizeof(ProjVerInfo_t));
    SysGetLocalProjVersion(&stProjVer);

    if(NULL != pstCtrlStationInfo)
    {
        if((HAS_BE_CONFIGURED == pstCtrlStationInfo->ucIsConfigured) && (0 != stProjVer.uiProjCRC))
        {
            SetLEDState(LED_NAME_ACTIVE, LED_ON);
        }
    }

    if(0 == stProjVer.uiProjCRC)
    {
        SetLEDState(LED_NAME_ACTIVE, LED_OFF);
    }

    UpdateLEDRunningState();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetLEDState
*
* 功能描述: 应用程序通过该函数获取LED的状态
*
* 输入参数: emLED:相应的LED名称
*
* 输出参数: 无
*
* 返 回 值  : LED的状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
LEDState_t GetLEDState(LEDName_t emLED)
{
    LEDState_t emState = LED_ILLEGAL_STATE;

    if(emLED < NUM_OF_LED)
    {
        emState = s_emLEDStatus[emLED];
    }

    return emState;
}

/*
***********************************************************************************************************************
* 函数名称: SetLEDState
*
* 功能描述: 应用程序设置指定的LED的状态，LED的实际亮灭情况不发生改变，当应用程序调用UpdateLEDRunningState()时，其实际运行情况才发生改变
*
* 输入参数: emLED:相应的LED名称
*        emLEDState: 相应的LED的状态（亮，灭，闪...）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SetLEDState(LEDName_t emLED, LEDState_t emLEDState)
{
    if((emLED < NUM_OF_LED) && (emLEDState < NUM_OF_LED_STATE))
    {
        s_emLEDStatus[emLED] = emLEDState;
    }

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: LEDInit
*
* 功能描述: 状态灯初始化，全部熄灭
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
static void LEDInit(void)
{
    memset((puint8_t)&s_emLEDStatus[0], LED_OFF , sizeof(LEDState_t)*NUM_OF_LED);
    return;
}


/*
***********************************************************************************************************************
* 函数名称: UpdateLEDRunningState
*
* 功能描述: 更新状态灯的实际运行情况（亮、灭、闪）
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
static void UpdateLEDRunningState(void)
{
    uint16_t usState = 0;
    uint16_t usStateLedRegVal = 0;

    uint16_t usComLedRegVal = 0;


    usState = (uint16_t)GetLEDState(LED_NAME_ACTIVE);
    usStateLedRegVal |= ((usState & 0x3)<<4);
    usState = (uint16_t)GetLEDState(LED_NAME_FAULT);
    usStateLedRegVal |= ((usState & 0x3)<<2);
    usState = (uint16_t)GetLEDState(LED_NAME_PASS);
    usStateLedRegVal |= (usState & 0x3);
    UpdateStateLED(usStateLedRegVal);

    SysSetCM2PMStateLED(usStateLedRegVal);

    usState = (uint16_t)GetLEDState(LED_NAME_CMBUS);
    usComLedRegVal |= (usState & 0x3);
    UpdateComLED(usComLedRegVal);

    SysSetCM2PMComLED(usComLedRegVal);

}







/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
