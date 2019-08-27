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
*  创建日期    : 2016-11-22
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
#include "../inc/Diag.h"
#include "Srv/Diag/inc/BoardDiag.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Scheduler/cagos.h"
#include "Srv/Sys/inc/CommSys.h"

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

    Core1DiagInit();
    Core1DiagPowerOn();
    FirstErrRestart();
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
    Core1DiagCycle();
    PassOrFailLEDUpdate();
}

/*
***********************************************************************************************************************
* 函数名称: FirstErrRestart
*
* 功能描述: 自检失败是否复位
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
void FirstErrRestart(void)
{
    SysIoModInfo_t* pCfg = NULL;
    uint32_t* puiRestartFlag = (uint32_t*)FIRST_ERR_RETRY_FLAG_ADDR;

    if( HAS_BE_CONFIGURED == SysGetIsConfigured())
    {
        pCfg = SharedGetIoModInfo(MAX_TASK_NUM);

        if(FIRST_ERROR_RESTART_SUPPORT == pCfg->stSysPMInfo.usFirstErrRetry)
        {
            /* SRAM 掉电起来后值为0 */
            if(*puiRestartFlag < 1)
            {
                //本次失败可以重启
                sysGpioLineSet (GPIO_4, GPIO_HIGH);

                if(TRUE == DiagCommGetCore0FailFlag())
                {
                    //core0引起复位，下次失败不可重启
                    *puiRestartFlag = 1;
                }
            }
            else
            {
                //不重启
                sysGpioLineSet (GPIO_4, GPIO_LOW);
            }
        }
        else
        {
            //不重启
            sysGpioLineSet (GPIO_4, GPIO_LOW);
        }
    }
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
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
    /* 获取诊断控制信息地址指针 */
    DiagCtrlInfo_t * pstThis = NULL;
    uint32_t uiVecFlag = FALSE;
    uint32_t* puiRestartFlag = (uint32_t*)FIRST_ERR_RETRY_FLAG_ADDR;

    pstThis= DiagCommGetDiagCtrlInfo();

    if(MASK_SETALL != pstThis->uiCodeDiagVec)
    {/* 代码自检失败 */
        BoardDiagErrHandle(TEST_FAILED,ERR_SYSCODE_A_TESTFAILED, 0, "code test failed");
        uiVecFlag = FALSE;
    }
    else if(MASK_SETALL != pstThis->uiCPUDiagVec)
    {/* CPU类自检失败 */
        uiVecFlag = FALSE;
        BoardDiagErrHandle(TEST_FAILED,ERR_TEST_CPU_UNKNOWN, 0, "CPU test failed");
    }
    else if(MASK_SETALL != pstThis->uiRamDiagVec)
    {/* RAM自检失败   */
        uiVecFlag = FALSE;
        BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED, 0 ,"RAM test failed");
    }
    else if(MASK_SETALL != pstThis->uiExceptionDiagVec)
    {/* 异常自检失败  */
        uiVecFlag = FALSE;
        BoardDiagErrHandle(TEST_FAILED,ERR_EXCPTTEST_FAILED, 0, "Exception test failed");
    }
    else
    {
        uiVecFlag = TRUE;
    }

    if(TRUE == uiVecFlag)
    {
        if(MCU_OK == SysGetMCUMonitorState())
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

        DiagCommSetCore1FailFlag();

        //下次失败不可重启
        *puiRestartFlag = 1;

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
