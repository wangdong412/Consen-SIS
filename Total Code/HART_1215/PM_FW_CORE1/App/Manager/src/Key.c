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
#include "../inc/LogIDManager.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Log/inc/Log.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/SharedMem/inc/SharedRtData.h"

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
    /*初始化钥匙开关位置*/
    SysSwitchPosInit();

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
    static uint32_t uiCntInit = 0;
    static uint32_t uiCntPrgm = 0;
    static uint32_t uiCntRun = 0;
    static uint32_t uiCntStop = 0;
    
    static bool_t bInitWrLogFlag = false;
    static bool_t bStartWrLogFlag = false;
    static bool_t bStopWrLogFlag = false;
    static bool_t bProgWrLogFlag = false;

    SharedAppend_t *pstAppend = NULL;

//    printf("  KeyManagerCycle  \n");
    
    /* 获取钥匙开关位置 */
    uiKeyPos = SysReadSwitchPos();

    /* 填写Append区中钥匙开关的状态 */
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    pstAppend->stPMState.stPMFirmwareState.ucKeyPosition = (uint8_t)uiKeyPos;

    /* 更新状态灯的状态 */
    switch(uiKeyPos)
    {
        case KS_INIT:
        {
            uiCntInit++;
            uiCntPrgm = 0;
            uiCntRun = 0;
            uiCntStop = 0;
            if(uiCntInit > KEY_FILTER_TIME)
            {
                SetLEDState(LED_NAME_INIT, LED_ON);
                SetLEDState(LED_NAME_PROG, LED_OFF);
                SetLEDState(LED_NAME_RUN, LED_OFF);
                SetLEDState(LED_NAME_STOP, LED_OFF);
                SharedSetSwitchPos(uiKeyPos);
                if(false == bInitWrLogFlag)
                {
                    LogWrite(LOG1_ID_APP_MANAGER_L0_KEY_INIT);
                    bInitWrLogFlag = true;
                    bStartWrLogFlag = false;
                    bStopWrLogFlag = false;
                    bProgWrLogFlag = false;
                }
            }
        }
        break;

        case KS_PROGRAM:
        {
            uiCntInit = 0;
            uiCntPrgm ++;
            uiCntRun = 0;
            uiCntStop = 0;
            if(uiCntPrgm > KEY_FILTER_TIME)
            {
                SetLEDState(LED_NAME_INIT, LED_OFF);
                SetLEDState(LED_NAME_PROG, LED_ON);
                SetLEDState(LED_NAME_RUN, LED_OFF);
                SetLEDState(LED_NAME_STOP, LED_OFF);
                SharedSetSwitchPos(uiKeyPos);

                if(false == bProgWrLogFlag)
                {
                    LogWrite(LOG1_ID_APP_MANAGER_L0_KEY_PROG);
                    bInitWrLogFlag = false;
                    bStartWrLogFlag = false;
                    bStopWrLogFlag = false;
                    bProgWrLogFlag = true;
                }
            }
        }
        break;

        case KS_RUN:
        {
            uiCntInit = 0;
            uiCntPrgm = 0;
            uiCntRun ++;
            uiCntStop = 0;
            if(uiCntRun > KEY_FILTER_TIME)
            {
                SetLEDState(LED_NAME_INIT, LED_OFF);
                SetLEDState(LED_NAME_PROG, LED_OFF);
                SetLEDState(LED_NAME_RUN, LED_ON);
                SetLEDState(LED_NAME_STOP, LED_OFF);
                SharedSetSwitchPos(uiKeyPos);

                if(false == bStartWrLogFlag)
                {
                    LogWrite(LOG1_ID_APP_MANAGER_L0_KEY_STRT);
                    bInitWrLogFlag = false;
                    bStartWrLogFlag = true;
                    bStopWrLogFlag = false;
                    bProgWrLogFlag = false;
                }
            }
        }
        break;

        case KS_STOP:
        {
            uiCntInit = 0;
            uiCntPrgm = 0;
            uiCntRun = 0;
            uiCntStop ++;
            if(uiCntStop > KEY_FILTER_TIME)
            {
                SetLEDState(LED_NAME_INIT, LED_OFF);
                SetLEDState(LED_NAME_PROG, LED_OFF);
                SetLEDState(LED_NAME_RUN, LED_OFF);
                SetLEDState(LED_NAME_STOP, LED_ON);
                SharedSetSwitchPos(uiKeyPos);

                if(false == bStopWrLogFlag)
                {
                    LogWrite(LOG1_ID_APP_MANAGER_L0_KEY_STOP);
                    bInitWrLogFlag = false;
                    bStartWrLogFlag = false;
                    bStopWrLogFlag = true;
                    bProgWrLogFlag = false;
                }
            }
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
