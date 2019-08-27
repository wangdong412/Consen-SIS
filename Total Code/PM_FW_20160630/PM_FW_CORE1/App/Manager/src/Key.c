﻿/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 
*
*  文件名称    : Key.c
*
*  功能简介    : 
*
*  作者          : 李琦
*
*  创建日期    : 2016-06-14
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
#include "../inc/Key.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/SharedMem/inc/SharedMem.h"

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
* 函数名称: KeyManagerInit
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
void KeyManagerInit(void)
{


}

/*
***********************************************************************************************************************
* 函数名称: KeyManagerCycle
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
void KeyManagerCycle(void)
{
    uint32_t uiKeyPos = 0;

    
//    printf("  KeyManagerCycle  \n");
    
    /* 获取钥匙开关位置 */
    uiKeyPos = SharedGetSwitchPos();

    /* 更新状态灯的状态 */
    switch(uiKeyPos)
    {
        case KS_INIT:
        {
            SetLEDState(LED_NAME_INIT, LED_ON);
            SetLEDState(LED_NAME_PROG, LED_OFF);
            SetLEDState(LED_NAME_RUN, LED_OFF);
            SetLEDState(LED_NAME_STOP, LED_OFF);
        }
        break;

        case KS_PROGRAM:
        {
            SetLEDState(LED_NAME_INIT, LED_OFF);
            SetLEDState(LED_NAME_PROG, LED_ON);
            SetLEDState(LED_NAME_RUN, LED_OFF);
            SetLEDState(LED_NAME_STOP, LED_OFF);
        }
        break;

        case KS_RUN:
        {
            SetLEDState(LED_NAME_INIT, LED_OFF);
            SetLEDState(LED_NAME_PROG, LED_OFF);
            SetLEDState(LED_NAME_RUN, LED_ON);
            SetLEDState(LED_NAME_STOP, LED_OFF);
        }
        break;

        case KS_STOP:
        {
            SetLEDState(LED_NAME_INIT, LED_OFF);
            SetLEDState(LED_NAME_PROG, LED_OFF);
            SetLEDState(LED_NAME_RUN, LED_OFF);
            SetLEDState(LED_NAME_STOP, LED_ON);
        }
        break;

        default:
        break;
    }

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