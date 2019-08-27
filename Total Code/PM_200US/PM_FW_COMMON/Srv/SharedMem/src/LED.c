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
*  作者       : 李琦
*
*  创建日期    : 2016-04-22
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
#include <Srv/Scheduler/cagos.h>
#include "../inc/LED.h"
#include "Srv/Sys/inc/SysHardware.h"
#include "Srv/SharedMem/inc/SharedRtData.h"

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





/*
***********************************************************************************************************************
*                                全局函数实现
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
* 注意事项: core0执行该函数，故未使用原子操作
***********************************************************************************************************************
*/
void LEDInit(void)
{
    memset((puint8_t)&s_emLEDStatus[0], LED_OFF , sizeof(LEDState_t)*NUM_OF_LED);
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
        //emState = s_emLEDStatus[emLED];
        emState = atomic32Get(&(s_emLEDStatus[emLED]));
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
* 注意事项: core0和core1均会调用该函数
***********************************************************************************************************************
*/
void SetLEDState(LEDName_t emLED, LEDState_t emLEDState)
{
    if((emLED < NUM_OF_LED) && (emLEDState < NUM_OF_LED_STATE))
    {
        //s_emLEDStatus[emLED] = emLEDState;
        atomic32Set(&(s_emLEDStatus[emLED]), emLEDState);
    }

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
* 注意事项: 该函数只被core1调用，，故未使用原子操作
***********************************************************************************************************************
*/
void UpdateLEDRunningState(void)
{
    uint16_t usState = 0;
    uint16_t usStateLedRegVal = 0;
    uint16_t usKeyLedRegVal = 0;
    uint16_t usComLedRegVal = 0;
    SharedAppend_t *pstAppend = NULL;

    usState = (uint16_t)GetLEDState(LED_NAME_FORCE);
    usStateLedRegVal |= ((usState & 0x3)<<8);
    usState = (uint16_t)GetLEDState(LED_NAME_SYNC);
    usStateLedRegVal |= ((usState & 0x3)<<6);
    usState = (uint16_t)GetLEDState(LED_NAME_ACTIVE);
    usStateLedRegVal |= ((usState & 0x3)<<4);
    usState = (uint16_t)GetLEDState(LED_NAME_FAULT);
    usStateLedRegVal |= ((usState & 0x3)<<2);
    usState = (uint16_t)GetLEDState(LED_NAME_PASS);
    usStateLedRegVal |= (usState & 0x3);
    UpdateStateLED(usStateLedRegVal);

    usState = (uint16_t)GetLEDState(LED_NAME_INIT);
    usKeyLedRegVal |= ((usState & 0x3)<<6);
    usState = (uint16_t)GetLEDState(LED_NAME_PROG);
    usKeyLedRegVal |= ((usState & 0x3)<<4);
    usState = (uint16_t)GetLEDState(LED_NAME_RUN);
    usKeyLedRegVal |= ((usState & 0x3)<<2);
    usState = (uint16_t)GetLEDState(LED_NAME_STOP);
    usKeyLedRegVal |= (usState & 0x3);
    UpdateKeyLED(usKeyLedRegVal);

    usState = (uint16_t)GetLEDState(LED_NAME_CMBUS);
    usComLedRegVal |= ((usState & 0x3)<<4);
    usState = (uint16_t)GetLEDState(LED_NAME_PMBUS);
    usComLedRegVal |= ((usState & 0x3)<<2);
    usState = (uint16_t)GetLEDState(LED_NAME_IPBUS);
    usComLedRegVal |= (usState & 0x3);
    UpdateComLED(usComLedRegVal);

    /* 填写Append区中灯的状态 */
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    pstAppend->stPMState.stPMFirmwareState.usStateLED = usStateLedRegVal;
    pstAppend->stPMState.stPMFirmwareState.usKeyLED = usKeyLedRegVal;
    pstAppend->stPMState.stPMFirmwareState.usComLED = usComLedRegVal;

}


/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/





/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
