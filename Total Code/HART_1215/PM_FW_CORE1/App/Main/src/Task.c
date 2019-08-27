/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : Main模块
*
*  文件名称    : task.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-02-01
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
#include <Srv/Scheduler/cagos.h>
#include "../inc/Task.h"
#include "Srv/Ver/inc/Ver.h"
#include "App/Manager/inc/Manager.h"
#include "App/Manager/inc/Watchdog.h"
#include "App/Monitor/inc/Monitor.h"
#include "App/Diag/inc/Diag.h"
#include "App/Comm/inc/Comm.h"
#include "App/FileManager/inc/FileManager.h"
#include "App/Sync/inc/Sync.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/CommProtocol/inc/CMBusProtocol.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
//static int32_t s_iInitTaskId = -1;

extern unsigned long rtsRunning;
extern UINT32 fpga_tick;

/*-----------------The following variables are used by RTS task------------*/
static uint32_t uiStartTime = 0;

/*-------------------------------------------------------------------------*/

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 初始任务 */
static void TskInitTask (void);
static void TskRTSTask (void);
static void TskDiagTask (void);
static void TskCommTask (void);
static void TskFileTask (void);
//static void TimeAlignment(void);
/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SysStart
*
* 功能描述: 系统启动函数
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
void SysStart(void)
{
    //uint32_t uiErr = 0;
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);             /* 前一个PMID号     */
    SyncTime64_t unSyncTime;
    uint32_t uiSwitchPos = 0;
    uint32_t uiSwitchPos2 = 0;
    uint32_t uiRetainDataLen = 0U;

    /* 显示版本号 */
    ShowVersion();
    
    
    SysReadLocalPMID();
    
    emLocalPMId = SysGetLocalPMID();
    emNextPMId = SysGetNextPMID(emLocalPMId);
    emPrePMId = SysGetPrePMID(emLocalPMId);
    
       printf("  LocalPMId :%d  PrePMId :%d NextPMId :%d \n",emLocalPMId,emPrePMId,emNextPMId);
    
    if(PMA == emLocalPMId)
    {
        
    }
    
    if(PMB == emLocalPMId)
    {
        udelay(5000000u);
    }
    
    if(PMC == emLocalPMId)
    {
        udelay(10000000u);
    }

    
    
    do
    {
        /*PM软件运行OK*/
        SysSetSWRuninOK(SW_RUNING_OK_VAR);
        /*PM升主使能*/
        SysSetTimerMasterEnable(MASTER_ENABLE_VAR);

        
        SysReadLocalPMID();
        
        emLocalPMId = SysGetLocalPMID();
        emNextPMId = SysGetNextPMID(emLocalPMId);
        emPrePMId = SysGetPrePMID(emLocalPMId);
        
 //       printf("  LocalPMId :%d  PrePMId :%d NextPMId :%d \n",emLocalPMId,emPrePMId,emNextPMId);
        
            if((TIMER_MASTER == SysGetTimerStatus(emLocalPMId)) || (TIMER_MASTER == SysGetTimerStatus(emPrePMId)) ||\
               (TIMER_MASTER == SysGetTimerStatus(emNextPMId)))
            {
                unSyncTime.stTimeStruct.uiLow32b = 10000U;
                unSyncTime.stTimeStruct.uiHigh32b = 0U;
                
                SysSetSyncTime(&unSyncTime);
                
//                printf("\n  PowerOn Adjust Time  \n");
            }
    }while(ADJUST_TIME_OK != SysGetSyncTimeState());

    do
    {
        /*从硬件寄存器读取数值*/
        uiSwitchPos = GetSwitchKeyPos();
        udelay(100000);//100ms
        uiSwitchPos2 = GetSwitchKeyPos();
    }while(uiSwitchPos != uiSwitchPos2);

    if(uiSwitchPos == 0)
    {
        printf("flash format start...\n");
        fs_format(0);
        printf("flash format finished\n");
        
        SharedSetTask0RetainData((puint8_t)&uiRetainDataLen,sizeof(uiRetainDataLen));
        SharedSetTask1RetainData((puint8_t)&uiRetainDataLen,sizeof(uiRetainDataLen));
    }

    printf("\n   #------------Create initial task cagRTS CORE1------------------#\n");

#if 0
    /* Create initial task */
    s_iInitTaskId = os_create_task("Init_task", TskInitTask, NULL, TASK_PRIO_INIT, TASK_TIME_SLIDE_INIT, &uiErr);
    
    if(s_iInitTaskId < 0)
    {
        printf("Create initial task failed %d\n", s_iInitTaskId);
    }

    if(uiErr != 0)
    {
        printf("Create initial task failed %d\n", uiErr);
    }
#endif

    TskInitTask();

    printf("\n   #------------Create initial task cagRTS CORE1 end------------------#\n");
    printf("RTS$:\n");
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskInitTask
*
* 功能描述: 初始化任务入口函数
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
//extern void LogTest(void);
void TskInitTask(void)
{
    int32_t iTaskId = -1;
    uint32_t uiErr = 0;
    uint32_t uiSignal = 0;
    bool_t bRet1 = false;
    bool_t bRet2 = false;
    //int32_t iRet = 0;

//    rtsRunning = 0x1;

    printf("\n   #------------Initial task cagRTS CORE1------------------#\n");

    /* Power on diagnosis */
    DiagInit();
    printf("DiagInit\n");

    /* System resource module initial */
    SysInit();
    udelay(100000);

    /* Monitor module initial */
    MonitorInit();

    /* Communication module initial */
    CommInit();
    
    /* Manager module initial */
    ManagerInit();

    /* File manager module initial */
    FileManagerInit();

    /* Communication Protocol module initial */
    CMBusProtocolInit();
    

#if 1
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE0_STEP1);
        udelay(500u);

    }while((SIGNAL_CORE1_STEP1 != uiSignal) || (true != bRet1) || (true != bRet2));
#endif
    
    
    ConfigCycle();
    
    SyncStart();
    
#if 1
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
        udelay(100);
        bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE0_START);
        udelay(100);
        printf("Handshake: s%d %d %d\n", uiSignal,bRet1, bRet2);
    }while((SIGNAL_CORE1_START != uiSignal) || (true != bRet1) || (true != bRet2));
#endif
    

    
    printf("CORE1 INIT END  \n");
    
    

    /* Create RTS task */
    iTaskId = os_create_task("RTS_task", TskRTSTask, NULL, TASK_PRIO_RTS, TASK_TIME_SLIDE_RTS, &uiErr);
    printf("Create RTS task.\n");
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create RTS task failed %d\n", uiErr);
    }

    /* Create diagnosis task */
    iTaskId = os_create_task("Diag_task", TskDiagTask, NULL, TASK_PRIO_DIAG, TASK_TIME_SLIDE_DIAG, &uiErr);
    printf("Create diag task.\n");
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create diagnosis task failed %d\n", uiErr);
    }

    /* Create file management task */
    iTaskId = os_create_task("File_task", TskFileTask, NULL, TASK_PRIO_FILE, TASK_TIME_SLIDE_FILE, &uiErr);
    printf("Create file task.\n");
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create file task failed %d\n", uiErr);
    }

    /* Create communication task */
    iTaskId = os_create_task("Comm_task", TskCommTask, NULL, TASK_PRIO_COMM, TASK_TIME_SLIDE_COMM, &uiErr);
    printf("Create comm task.\n");
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create communication task failed %d\n", uiErr);
    }
    
    
    //WatchdogManagerEnable();

    printf("init end\n");

#if 0
    printf("Delete initial task\n");
    iRet = os_del_task(s_iInitTaskId, &uiErr);
    printf("Delete initial task ...\n");
    if(iRet != 0)
    {
        printf("Delete initial task failed!\n");
    }
#endif

    /* Show log info test*/
    //LogTest();
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskRTSTaskInit
*
* 功能描述: RTS周期任务初始化函数
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
static void TskRTSTaskInit(void)
{
    rtsRunning = 0x1;
    uiStartTime = SysGetSyncTimeLow32 ();
    printf("os%d rts%d\n",fpga_tick, uiStartTime);
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskRTSTaskExe
*
* 功能描述: RTS周期任务执行函数
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
void TskRTSTaskExe(void)
{
    /* 看门狗 */
    WatchdogManagerCycle();

    /* Monitor module cycle function */
    MonitorCycle();

    /* Manager module cycle function */
    ManagerCycle();

    /* 更新看门狗计数 */
    UpdateWatchdogCounter();

    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskRTSTask
*
* 功能描述: RTS周期任务
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void TskRTSTask (void)
{
    TskRTSTaskInit();

    while(1)
    {
        TskRTSTaskExeEntry();
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: TskDiagTaskExe
*
* 功能描述: 自检周期任务执行函数
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
void TskDiagTaskExe(void)
{
    DiagCycle();
    SetLEDState(LED_NAME_PASS, LED_ON); //diag pass

    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskDiagTask
*
* 功能描述: 诊断周期任务
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
static void TskDiagTask (void)
{
    while(1)
    {
        TskDiagTaskExeEntry();
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskFileTaskExe
*
* 功能描述: 文件管理周期任务执行函数
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
void TskFileTaskExe(void)
{
    FileManagerCycle();

    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskFileTask
*
* 功能描述: 文件管理周期任务
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
static void TskFileTask (void)
{
    while(1)
    {
        TskFileTaskExeEntry();
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskCommTaskExe
*
* 功能描述: 通讯周期任务执行函数
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
void TskCommTaskExe(void)
{
    CommCycle();
    CMBusProtocolCycle();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: TskCommTask
*
* 功能描述: 通讯周期任务
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
static void TskCommTask (void)
{
    while(1)
    {
        TskCommTaskExeEntry();
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


