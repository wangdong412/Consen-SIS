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
#include "Srv/Log/inc/Log.h"
#include "Srv/Log/inc/LogID.h"

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
static uint32_t s_uiSeqNum[TASK_SEQ_GRP_MAX] = {0U};       /* 顺序号            */
static const uint32_t s_uiCheckTab[TASK_SEQ_MAX] =         /* 顺序和预设值      */
    {
           0U,    2U, 5U,    10U,
          17U,   28U, 41U,   58U,
          77U,  100U, 129U,  160U,
         197U,  238U, 281U,  328U,
         381U,  440U, 501U,  568U,
         639U,  712U, 791U,  874U,
         963U, 1060U, 1161U, 1264U,
        1371U, 1480U, 1593U, 1720U,
        1851U, 1988U, 2127U, 2276U,
        2427U, 2584U, 2747U, 2914U,
        3087U, 3266U, 3447U, 3638U
    };
static const uint32_t s_uiAddTab[TASK_SEQ_MAX] =          /* 顺序累加值         */
    {
        2U, 3U, 5U, 7U,
        11U, 13U, 17U, 19U,
        23U, 29U, 31U, 37U,
        41U, 43U, 47U, 53U,
        59U, 61U, 67U, 71U,
        73U, 79U, 83U, 89U,
        97U, 101U, 103U, 107U,
        109U, 113U, 127U, 131U,
        137U, 139U, 149U, 151U,
        157U, 163U, 167U, 173U,
        179U, 181U, 191U, 193U
    };

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
static uint32_t TaskCheckRunSeq(uint32_t uiGrpSn, uint32_t uiCheckId, const uint32_t uiCheckMax);

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
    PMController_t emLocalPMId = INVALID_PM_ID; /* 本地PMID号 */
    PMController_t emNextPMId = INVALID_PM_ID; /* 下一个PMID号 */
    PMController_t emPrePMId = INVALID_PM_ID; /* 前一个PMID号 */
    SyncTime64_t unSyncTime;
    uint32_t uiSwitchPos1 = 0;
    uint32_t uiSwitchPos2 = 0;
    uint32_t uiRetainDataLen = 0U;
    DisableTablHead_t stDisableTablHead;
    bool_t bRet = false;
    uint32_t uiSignal = 0;
    bool_t bRet1 = false;
    bool_t bRet2 = false;

    /* 显示版本号 */
    ShowVersion();

    /* System resource module initial 初始化系统资源，共享数据区初始化在core0中 */
    SysInit();

    /* 判断插稳，如果未插稳，循环至看门狗复位 */
    do
    {
        bRet = GetLocalPMStabState();

    }while(bRet == false);

    /* 设置本系槽位，如果两套地址不一致，循环至看门狗复位 */
    do
    {
        bRet = SetLocalPMAddr();

    }while(bRet == false);

    /* 第一次握手，如果core1判断未插稳或者两系槽位不一致，则core0，core1都不运行，等待看门狗复位 */
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
        udelay(100);
        bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE0_STEP0);
        udelay(100);
        printf("Handshake: s%d %d %d\n", uiSignal,bRet1, bRet2);
    }while((SIGNAL_CORE1_STEP0 != uiSignal) || (true != bRet1) || (true != bRet2));

    /* 判断本系槽位号，进行延时处理 */
    emLocalPMId = SysGetLocalPMID();
    emNextPMId = SysGetNextPMID(emLocalPMId);
    emPrePMId = SysGetPrePMID(emLocalPMId);
    printf("  LocalPMId :%d  PrePMId :%d NextPMId :%d \n",emLocalPMId,emPrePMId,emNextPMId);

    if(PMA == emLocalPMId)
    {
       ; /* A系，不延时 */
    }
    else if(PMB == emLocalPMId)
    {
        udelay(5000000u); /* B系，延时5s */
    }
    else if(PMC == emLocalPMId)
    {
        udelay(10000000u); /* C系，延时10s */
    }
    else
    {
        ;
    }

    /* 校时处理 */
    do
    {
        /* 设置PM软件运行OK */
        SysSetSWRuninOK(SW_RUNING_OK_VAR);
        /* 设置PM升主使能 */
        SysSetTimerMasterEnable(MASTER_ENABLE_VAR);
        
        if((TIMER_MASTER == SysGetTimerStatus(emLocalPMId)) || (TIMER_MASTER == SysGetTimerStatus(emPrePMId))
                || (TIMER_MASTER == SysGetTimerStatus(emNextPMId)))
        {
            unSyncTime.stTimeStruct.uiLow32b = 10000U;
            unSyncTime.stTimeStruct.uiHigh32b = 0U;
            SysSetSyncTime(&unSyncTime);
        }
    }while(ADJUST_TIME_OK != SysGetSyncTimeState());

    /* 读钥匙开关，间隔100ms读两次 */
    do
    {

        uiSwitchPos1 = GetSwitchKeyPos();
        udelay(100000);
        uiSwitchPos2 = GetSwitchKeyPos();
    }while(uiSwitchPos1 != uiSwitchPos2);

    /* 当钥匙开关为INIT时，清工程文件等 */
    if(uiSwitchPos1 == 0)
    {
        /* 清空工程文件 */
        printf("flash format start...\n");
        fs_format(0);
        printf("flash format finished\n");
        /* 清空掉电保持变量 */
        SharedSetTask0RetainData((puint8_t)&uiRetainDataLen,sizeof(uiRetainDataLen));
        SharedSetTask1RetainData((puint8_t)&uiRetainDataLen,sizeof(uiRetainDataLen));
        /* 清空强制列表 */
        memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
        SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
    }

    printf("\n   #-------------Create initial task cagRTS CORE1-------------#\n");
    /* 各个应用模块功能初始化 */
    TskInitTask();
    printf("\n   #-----------Create initial task cagRTS CORE1 end------------#\n");

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
void TskInitTask(void)
{
    int32_t iTaskId = -1;
    uint32_t uiErr = 0;
    uint32_t uiSignal = 0;
    bool_t bRet1 = false;
    bool_t bRet2 = false;

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
    /* Power on diagnosis */
    DiagInit();

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
    LogWrite(LOG1_ID_USER_L0_INIT_END);

    /* Create RTS task */
    iTaskId = os_create_task("RTS_task", TskRTSTask, NULL, TASK_PRIO_RTS, TASK_TIME_SLIDE_RTS, &uiErr);
    printf("Create RTS task.\n");
    if(iTaskId < 0)
    {
        printf("Create RTS task failed! %d %d\n", iTaskId, uiErr);
    }

    /* Create diagnosis task */
    iTaskId = os_create_task("Diag_task", TskDiagTask, NULL, TASK_PRIO_DIAG, TASK_TIME_SLIDE_DIAG, &uiErr);
    printf("Create diag task.\n");
    if(iTaskId < 0)
    {
        printf("Create diag task failed! %d %d\n", iTaskId, uiErr);
    }

    /* Create file management task */
    iTaskId = os_create_task("File_task", TskFileTask, NULL, TASK_PRIO_FILE, TASK_TIME_SLIDE_FILE, &uiErr);
    printf("Create file task.\n");
    if(iTaskId < 0)
    {
        printf("Create file task failed! %d %d\n", iTaskId, uiErr);
    }

    /* Create communication task */
    iTaskId = os_create_task("Comm_task", TskCommTask, NULL, TASK_PRIO_COMM, TASK_TIME_SLIDE_COMM, &uiErr);
    printf("Create comm task.\n");
    if(iTaskId < 0)
    {
        printf("Create comm task failed! %d %d\n", iTaskId, uiErr);
    }

    //WatchdogManagerEnable();

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
    WatchdogManagerEnable();
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
    printf("RS=%d\n",SysGetSyncTimeLow32 ());
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
    uint32_t uiRet = TASK_CHECK_SEQ_ERR;

    TskRTSTaskInit();

    while(1)
    {
        uiRet = TaskCheckRunSeq(0, RTS_TASK_SEQ, MAX_TASK_SEQ_NUM);

        if(TASK_CHECK_SEQ_OK == uiRet)
        {
            TskRTSTaskExeEntry();
        }
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
    //DiagCycle();

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
    uint32_t uiRet = TASK_CHECK_SEQ_ERR;

    while(1)
    {
        uiRet = TaskCheckRunSeq(0, COMM_TASK_SEQ, MAX_TASK_SEQ_NUM);

        if(TASK_CHECK_SEQ_OK == uiRet)
        {
            TskCommTaskExeEntry();
            TaskCheckRunSeq(0, MAX_TASK_SEQ_NUM, MAX_TASK_SEQ_NUM);
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: TaskCheckRunSeq
*
* 功能描述: 校验程序运行顺序。
*
* 输入参数:
*      uiGrpSn              顺序自检组号
*      uiCheckId            校验ID号
*      uiCheckMax           本项顺序自检最大个数
* 输出参数: 无
*
* 返 回 值  :
*       uiRetCheck           若校验成功: TASK_CHECK_SEQ_OK
*                           否则: TASK_CHECK_SEQ_ERR
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t TaskCheckRunSeq(uint32_t uiGrpSn, uint32_t uiCheckId, const uint32_t uiCheckMax)
{
    uint32_t uiRetCheck = TASK_CHECK_SEQ_ERR;                    /* 顺序校验结果      */

    /* 检查输入参数 */
    if((TASK_SEQ_GRP_MAX > uiGrpSn) && (TASK_SEQ_MAX > uiCheckMax))
    {
        /* 检查顺序和 */
        if(s_uiCheckTab[uiCheckId] != s_uiSeqNum[uiGrpSn])
        {
            /* 顺序自检失败 */
            uiRetCheck = TASK_CHECK_SEQ_ERR;
        }
        else
        {
            /* 顺序自检通过 */
            uiRetCheck = TASK_CHECK_SEQ_OK;
            /* 更新顺序和 */
            s_uiSeqNum[uiGrpSn] += s_uiAddTab[uiCheckId];
        }
        /* 判断本轮顺序自检是否结束 */
        if((TASK_CHECK_SEQ_OK == uiRetCheck) && (uiCheckMax == uiCheckId))
        {
            s_uiSeqNum[uiGrpSn] = 0U;
        }
    }
    return uiRetCheck;
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


