/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 任务层模块
*
*  文件名称    : MainTask.c
*
*  功能简介    : 主任务
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-30
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
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../inc/MainTask.h"
#include "Sys/inc/Sys.h"
#include "ExterComm/inc/ExterComm.h"
#include "CS1131/inc/CS1131.h"
#include "OPC/inc/OPC.h"
#include "SOE/inc/SOE.h"
#include "Client/inc/Client.h"
#include "Modbus/inc/Modbus.h"
#include "GPS/inc/GPS.h"
#include "NTP/inc/NTP.h"
#include "P2P/inc/P2P.h"
#include "AMS/inc/AMS.h"
#include "InterComm/inc/InterComm.h"
#include "LED/inc/LED.h"
#include "Version/inc/Version.h"
#include "Monitor/inc/Monitor.h"
#include "Config/inc/Config.h"
#include "Log/inc/Log.h"

/*
 ***********************************************************************************************************************
 *                                全局变量声明
 ***********************************************************************************************************************
 */
extern pid_t MainID;
extern pid_t TimingID;

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

static void TskMainTaskInit(void);

static void TskMainTaskCycle(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: MainTask
*
* 功能描述: 主任务
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void  TskMainTask(void )
{
    //struct timeval stTmpTimeVal;
    uint32_t uiStartTime = 0;
    uint32_t uiFinishTime = 0;
    uint32_t uiIntervalTime = 0;

    int prio_process;
    //int prio;//-5
    prio_process = getpriority(PRIO_PROCESS, MainID);
    printf("Process (%d) Priority is %d.\n", MainID, prio_process);

#if 0
    unsigned long mask = 0;
    
    mask = 2; /* process switches to processor 1 now */
    if (sched_setaffinity(0, sizeof(mask), &mask) <0)
    {
        perror("sched_setaffinity");
    }
#endif
    /* 主任务初始化函数  */
    TskMainTaskInit();
    
    /* 主任务周期运行函数  */
    while(1)
    {
        SetLEDState(LED_NAME_PASS, LED_ON);

        //memset(&stTmpTimeVal, 0, sizeof(struct timeval));
        //gettimeofday(&stTmpTimeVal, NULL);
        //uiStartTime = stTmpTimeVal.tv_sec * 1000 *1000 + stTmpTimeVal.tv_usec;
        uiStartTime = SysGetFreeCounterValue32(); //us
        //printf("st=%d\n", uiStartTime);
        printf("--------------Main task----------------\n");
 
        TskMainTaskCycle();
        
        //gettimeofday(&stTmpTimeVal, NULL);
        //uiFinishTime = stTmpTimeVal.tv_sec * 1000 *1000 + stTmpTimeVal.tv_usec;
        uiFinishTime = SysGetFreeCounterValue32(); //us
        //printf("end=%d\n", uiFinishTime);

        if(MAIN_TASK_PERIOD > (uiFinishTime - uiStartTime))
        {
            printf("sleep=%d\n",uiIntervalTime);
            uiIntervalTime = MAIN_TASK_PERIOD - (uiFinishTime - uiStartTime);
            usleep(uiIntervalTime);
        }
        else
        {
            printf("@@@@@@@@@ CM CYCLE TIMEOUT! @@@@@@@@@@ cycle_time=%d\n",uiFinishTime - uiStartTime);
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
* 函数名称: TskMainTaskInit
*
* 功能描述: 主任务初始函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/

static void TskMainTaskInit(void)
{
    /* 系统资源管理模块初始化 */
    SysInit();

    /* 日志模块初始化 */
    LogInit();

    /* 外部通讯模块初始化 */
    ExternalCommInit();
    
    /* 内部通讯模块初始化 */
    InternalCommInit();

    /* 配置模块初始化 */
    CMConfigInit();
    
    /* 诊断模块初始化 */


    /* LED状态灯模块初始化 */
    LEDManagerInit();

    /* SOE模块初始化 */
    SOEInit();

    /* CS1131组态软件模块初始化*/
    CS1131Init();
    
    /* P2P模块初始化*/
    P2PInit();

    /* Modbus模块初始化 */
    ModbusInit();

    /* OPC模块初始化*/
    OPCInit();

    /* 客户端模块初始化 */
    ClientInit();

    /* AMS模块初始化 */
    AMSInit();

    /* 监控模块初始化 */
    MonitorInit();

    /* 显示CM版本号 */
    ShowCMVersion();

    /* GPS模块初始化 */
    GPSInit();

    /* NTP模块初始化 */
    NTPInit();
}

/*
***********************************************************************************************************************
* 函数名称: TskMainTaskCycle
*
* 功能描述: 主任务周期运行函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/

static void TskMainTaskCycle(void)
{
    /* 内部通讯模块周期运行函数 */
    InternalCommCycle();

    /* 外部通讯模块周期运行函数 */
    ExternalCommCycle();
    
    /* 配置模块周期运行函数 */
    CMConfigCycle();

    /* 诊断模块周期运行函数 */

    /* LED状态灯模块周期运行函数 */
    LEDManagerCycle();

    /* SOE模块周期运行函数 */
    SOECycle();

    /* CS1131组态软件模块周期运行函数*/
    CS1131Cycle();

    /* P2P模块周期运行函数*/
    P2PCycle();

    /* Modbus模块周期运行函数 */
    ModbusCycle();

    /* OPC模块周期运行函数*/
    OPCCycle();

    /* 客户端模块周期运行函数 */
    ClientCycle();

    /* AMS模块周期运行函数 */
    AMSCycle();

    /* 监控模块周期运行函数 */
    MonitorCycle();

    /* Modbus模块周期运行函数 */
    GPSCycle();

    /* NTP模块周期运行函数 */
    NTPCycle();
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
