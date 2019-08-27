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
*  文件名称    : Watchdog.c
*
*  功能简介    : 
*
*  作者          : 李琦
*
*  创建日期    : 2016-08-05
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
#include "App/Main/inc/Task.h"
#include <Srv/Scheduler/include/stdio.h>
#include "../inc/Watchdog.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/Scheduler/cagos.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Diag/inc/DiagCommon.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint32_t s_uiCount = 0;
static uint32_t s_uiRTSCount = 0;
static uint32_t s_uiCore0Wtd = 0;
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
* 函数名称: WatchdogManagerInit
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
void WatchdogManagerInit(void)
{
    printf("WTD init\n");
    SysWatchdogConfig(WTD_CONFIG_ENABLE);
    SysSetWatchdogLowLimit(WTD_LOW_LIMIT);
    SysSetWatchdogHighLimit(WTD_HIGH_LIMIT);

    /* GPIO5管脚初始化 */
    sysGpioLineConfig (GPIO_5, GPIO_OUT);
    sysGpioLineSet (GPIO_5, GPIO_HIGH);


//    sysGpioLineConfig (GPIO_2, GPIO_OUT);
//    sysGpioLineConfig (GPIO_3, GPIO_OUT);

    s_uiCount = 0;
    s_uiRTSCount = 0;
    s_uiCore0Wtd = 0;
}

/*
***********************************************************************************************************************
* 函数名称: WatchdogManagerEnable
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
void WatchdogManagerEnable(void)
{
    SysWatchdogEnable();
    printf("WTD Enable\n");
}

/*
***********************************************************************************************************************
* 函数名称: WatchdogManagerCycle
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
#if 0
void WatchdogManagerCycle(void)
{
    static uint32_t uiCore0CounterOld = 0;
    uint32_t uiCore0CounterNew = 0;
    bool_t bRet = false;

    //do
    //{
        bRet = SysGetSMWatchdogInfo(&uiCore0CounterNew);
    //}while(bRet != true);

    //printf("cnt0=%d cnt1=%d\n", uiCore0CounterNew, s_uiCount);

    if((0 == uiCore0CounterNew) || (uiCore0CounterNew != uiCore0CounterOld))
    {
        /* 操作GPIO5每次翻转，喂狗 */
        if(0 == s_uiCount % 2)
        {
            sysGpioLineSet (GPIO_5, GPIO_HIGH);
            //printf("g1\n");
        }
        else
        {
            sysGpioLineSet (GPIO_5, GPIO_LOW);
            //printf("g0\n");
        }
    }
    else
    {
        #ifdef CONTROL_CYCLE_20MS
		//printf("c0 wtd err %d\n",uiCore0CounterNew);
		#endif
		
		#ifdef CONTROL_CYCLE_5MS
        printf("@");
		#endif
    }
    uiCore0CounterOld = uiCore0CounterNew;

    return;
}
#endif

void WatchdogManagerCycle(void)
{
    static uint32_t uiCore0CounterOld = 0;
    uint32_t uiCore0CounterNew = 0;
    bool_t bRet = false;

    bRet = SysGetSMWatchdogInfo(&uiCore0CounterNew);

    if(bRet == true)
    {
        if((uiCore0CounterNew == uiCore0CounterOld) && (uiCore0CounterNew != 0))
        {
            s_uiCore0Wtd++;
        }
        else
        {
            s_uiCore0Wtd = 0;
        }
    }

    if(s_uiCore0Wtd < 1000)
    {
        /* 操作GPIO5每次翻转，喂狗 */
        if(0 == s_uiCount % 2)
        {
            sysGpioLineSet (GPIO_5, GPIO_HIGH);
            //printf("g1\n");
        }
        else
        {
            sysGpioLineSet (GPIO_5, GPIO_LOW);
            //printf("g0\n");
        }
    }

    uiCore0CounterOld = uiCore0CounterNew;
    return;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateWatchdogCounter
*
* 功能描述: 更新看门狗计数器
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
void UpdateWatchdogCounter(void)
{
    uint32_t uiFlag = 0;

    uiFlag = DiagCommGetCore1FailFlag();

    if(MASK_SETALL == uiFlag)
    {
        s_uiCount++;
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
                                 文件结束
***********************************************************************************************************************
*/
