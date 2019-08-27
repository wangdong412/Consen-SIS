﻿/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : Main模块
*
*  文件名称    : Task.c
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
#include "App/Monitor/inc/Monitor.h"
#include "App/DataProcess/inc/DataProcess.h"
#include "App/Diag/inc/Diag.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/ExtLib/inc/ExtLib.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/Sys/inc/CommSysCommon.h"
#include "Srv/Diag/inc/DiagCommon.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
#if 0
static int32_t s_iInitTaskId = -1;
#endif
int32_t s_iRTSTaskId = -1;
int32_t s_iUP1TaskId = -1;
int32_t s_iUP2TaskId = -1;

static int32_t s_iRTStoUP1Sem;
static int32_t s_iRTStoUP2Sem;
static int32_t s_iUP1toRTSSem;
static int32_t s_iUP2toRTSSem;

static bool_t s_bUP1FinishFlag;
static bool_t s_bUP2FinishFlag;
static bool_t s_bUP1StartFlag;
static bool_t s_bUP2StartFlag;

static uint32_t s_uiCounter;

static UP1Entry pUP1Func = NULL;
static UP2Entry pUP2Func = NULL;
static puint8_t pucDataAreaAddr = NULL;

/* 临时Append */
uint32_t uiUP1IECTime = 0;
uint32_t uiUP2IECTime = 0;

static uint32_t uiUP1LastStartTime;
static uint32_t uiUP2LastStartTime;
uint32_t uiUP1DeltaTime = 0;
uint32_t uiUP2DeltaTime = 0;
bool_t bUP1DeltaFlag = false;
bool_t bUP2DeltaFlag = 0;

extern UINT32 fpga_tick;
extern unsigned long rtsRunning;
extern unsigned long rtsdebug;
/*-----------------The following variables are used by RTS task------------*/
static uint32_t uiErr = 0;
static int32_t iPend = -1;
static uint32_t uiStartTime = 0;
static uint32_t uiEndTime = 0;
static uint32_t uiTimeWindow1 = 0;
static uint32_t uiTimeWindow2 = 0;
static uint32_t s_uiRTSExeTime = 0U;
static PMController_t emLocalPMId = INVALID_PM_ID;
//static fpRTSTaskEntry g_fpRtsTaskEntry = NULL;
//static uint8_t s_ucExeIndex = 0;
/*-------------------------------------------------------------------------*/


/*-----------------used by UP1 task----------------------------------------*/
static uint32_t s_uiU1Err = 0;
static int32_t s_iU1Pend = -1;
static uint32_t s_U1T1 = 0;
static uint32_t s_U1T2 = 0;
static uint32_t s_uiUP1ExeTime = 0U;

/*-------------------------------------------------------------------------*/


/*-----------------used by UP2 task----------------------------------------*/
static uint32_t s_uiU2Err = 0;
static int32_t s_iU2Pend = -1;
static uint32_t s_U2T1 = 0;
static uint32_t s_U2T2 = 0;
static uint32_t s_uiUP2ExeTime = 0U;

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
/* RTS周期任务 */
static void TskRTSTask (void);
/* UP1周期任务 */
static void TskUP1Task (void);
/* UP2周期任务 */
static void TskUP2Task (void);

static void UpdateWatchdogCounter(void);

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
    uint32_t uiSignal = 0;
    bool_t bRet1 = false;
    bool_t bRet2 = false;

    /* 显示版本号 */
    ShowVersion();
    
    /* System resource module initial 初始化系统资源，共享数据区初始化在本core0中 */
    SysInit();

    /* 第一次握手，如果core1判断未插稳或者两系槽位不一致，则core0，core1都不运行，等待看门狗复位 */
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE1_STEP0);
        udelay(500u);
    }while((SIGNAL_CORE0_STEP0 != uiSignal) || (true != bRet1) || (true != bRet2));

    do
    {

    }while(ADJUST_TIME_OK != SysGetSyncTimeState());

    printf("\n   #------------Create initial task cagRTS CORE0------------------#\n");
    /* 各个应用模块功能初始化 */
    TskInitTask();
    printf("\n   #------------Create initial task cagRTS CORE0 end------------------#\n");

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
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void TskInitTask (void)
{
    uint32_t uiErr = 0;
    uint32_t uiSignal = 0;
    bool_t bRet1 = false;
    bool_t bRet2 = false;

    s_uiCounter = 0;

    s_iRTStoUP1Sem = os_sem_create("RTStoUP1", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iRTStoUP1Sem create failed.\n");
    }

    s_iRTStoUP2Sem = os_sem_create("RTStoUP2", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iRTStoUP2Sem create failed.\n");
    }

    s_iUP1toRTSSem = os_sem_create("UP1toRTS", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iUP1toRTSSem create failed.\n");
    }

    s_iUP2toRTSSem = os_sem_create("UP2toRTS", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iUP2toRTSSem create failed.\n");
    }

    s_bUP1FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
    s_bUP2FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;

    s_bUP1StartFlag = UP_TASK_RUNNING_NOT_START;
    s_bUP2StartFlag = UP_TASK_RUNNING_NOT_START;

    /* External library initial */
    ExtLibInit();
    ExtAppendDataInit();
    printf("ExtLibInit\n");

    /* Monitor module initial */
    MonitorInit();
    printf("MonitorInit\n");

    /* Manager module initial */
    ManagerInit();
    printf("ManagerInit\n");

    /* 初始化数据处理 */
    DataProcessInit();
    printf("DataProcessInit\n");

    
#if 1
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE1_STEP1);
        udelay(500u);
//        printf("Handshake: s%d %d %d\n", uiSignal,bRet1, bRet2);
    }while((SIGNAL_CORE0_STEP1 != uiSignal) || (true != bRet1) || (true != bRet2));
#endif
    /* Power on diagnosis */
    DiagInit();
    printf("DiagInit\n");

#if 1
    printf("Handshake\n");
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE1_START);
        udelay(500u);
        printf("Handshake: s%d %d %d\n", uiSignal,bRet1, bRet2);
    }while((SIGNAL_CORE0_START != uiSignal) || (true != bRet1) || (true != bRet2));
#endif
    printf("CORE0  INIT END  \n");

    /* Create RTS task */
    s_iRTSTaskId = os_create_task("RTS_task", TskRTSTask, NULL, TASK_PRIO_RTS, TASK_TIME_SLIDE_RTS, &uiErr);
    printf("Create rts task.\n");
    if(uiErr != 0)
    {
        printf("Create RTS task failed %d\n", uiErr);
    }

    /* Create UP1 task */
    s_iUP1TaskId = os_create_task("UP1_task", TskUP1Task, NULL, TASK_PRIO_UP1, TASK_TIME_SLIDE_UP1, &uiErr);
    printf("Create up1 task.\n");
    if(uiErr != 0)
    {
        printf("Create UP1 task failed %d\n", uiErr);
    }

    /* Create UP2 task */
    s_iUP2TaskId = os_create_task("UP2_task", TskUP2Task, NULL, TASK_PRIO_UP2, TASK_TIME_SLIDE_UP2, &uiErr);
    printf("Create up2 task.\n");
    if(uiErr != 0)
    {
        printf("Create UP2 task failed %d\n", uiErr);
    }

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
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void TskRTSTaskInit(void)
{
    emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */

    //printf("\n   #------------RTS TASK------------------#\n");
    rtsRunning = 0x1;
    uiTimeWindow1 = RTS_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;

    uiStartTime = SysGetSyncTimeLow32 ();
    //printf("os%d rts%d\n",fpga_tick, uiStartTime);
}
/*
***********************************************************************************************************************
* 函数名称: TskRTSTaskExe
*
* 功能描述: RTS周期任务入口函数
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
void TskRTSTaskExe(void)
{
    uiStartTime = SysGetSyncTimeLow32 ();

    s_uiRTSExeTime++;
	
	#ifdef CONTROL_CYCLE_20MS
    printf("RS=%d %d\n",uiStartTime,s_uiRTSExeTime);
	#endif
	
    /* RTS task get semaphore from UP1 task*/
     iPend = os_sem_pend(s_iUP1toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);

     if(0 == iPend)
     {
         s_bUP1FinishFlag = UP_TASK_RUNNING_FINISHED;
     }

     /* Calculate UP1 running period */
     if(UP_TASK_RUNNING_FINISHED == s_bUP1FinishFlag)
     {
         s_bUP1FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
         SysSetIECShedCtrlFlg(TASK_UP1, IEC_END);
         if((TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP1)) ||
            (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP1)))
         {
             SysSetOutputPollFlg(TASK_UP1, OUTPUT_POLL_ENABLE);
         }
     }

     /* RTS task get semaphore from UP2 task*/
     iPend = os_sem_pend(s_iUP2toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);

     if(0 == iPend)
     {
         s_bUP2FinishFlag = UP_TASK_RUNNING_FINISHED;
     }


     /* Calculate UP2 running period */
     if(UP_TASK_RUNNING_FINISHED == s_bUP2FinishFlag)
     {
         s_bUP2FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
         SysSetIECShedCtrlFlg(TASK_UP2, IEC_END);
         if((TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP2)) ||
            (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP2)))
         {
             SysSetOutputPollFlg(TASK_UP2, OUTPUT_POLL_ENABLE);
         }
     }


    /*输出*/
    DataProcessOutputHandle();

    /* core0 --> shared memory */
    if(IEC_END == SysGetIECShedCtrlFlg(TASK_UP1))
    {
        if(PF_PAUSE != SharedGetPauseFlag(TASK_UP1))
        {
            SysCopyCore0RtDataToSharedRtDataUP1();

            /* Backup UP1 to UP2 data */
            SysBackupRtDataUP1();
        }
    }
    if(IEC_END == SysGetIECShedCtrlFlg(TASK_UP2))
    {
        if(PF_PAUSE != SharedGetPauseFlag(TASK_UP2))
        {
            SysCopyCore0RtDataToSharedRtDataUP2();

            /*Backup UP2 to UP1 data */
            SysBackupRtDataUP2();
        }
    }

    /* Diag module cycle function */
    //DiagCycle();

    /* Monitor module cycle function */
    MonitorCycle();

    /* 等待输出扫描完成 */
    uiEndTime = SysGetSyncTimeLow32 ();
	
	#ifdef CONTROL_CYCLE_20MS
    if(uiEndTime - uiStartTime < 80)
    {
        //printf("end=%d %d\n",uiEndTime,uiEndTime - uiStartTime);
        udelay(8000-(uiEndTime - uiStartTime)*100);
    }
	#endif

	#ifdef CONTROL_CYCLE_5MS
    if(uiEndTime - uiStartTime < 30)
    {
        //printf("end=%d %d\n",uiEndTime,uiEndTime - uiStartTime);	
        udelay(3000-(uiEndTime - uiStartTime)*100);		
    }
	#endif	
	

    /*输入*/
    DataProcessInputHandle();

//            uiStartTime = SysGetSyncTimeLow32 ();
//            printf("RS1=%d\n",uiStartTime);

    if(IEC_START == SysGetIECShedCtrlFlg(TASK_UP1))
    {
        /* RTS task send semaphore to UP1 task*/
        os_sem_post(s_iRTStoUP1Sem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);

        if(uiErr != OS_ERR_NONE)
        {
            printf("RTS task post s_iRTStoUP1Sem failed.\n");
        }
    }

    if(IEC_START == SysGetIECShedCtrlFlg(TASK_UP2))
    {
        /* RTS task send semaphore to UP1 task*/
        os_sem_post(s_iRTStoUP2Sem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);

        if(uiErr != OS_ERR_NONE)
        {
            //printf("RTS task post s_iRTStoUP2Sem failed.\n");
        }
    }

    /* Manager module cycle function */
    ManagerCycle();

    UpdateWatchdogCounter();

}
/*
***********************************************************************************************************************
* 函数名称: TskRTSTask
*
* 功能描述: RTS周期任务入口函数
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
    rtsdebug = 1;
    //uint32_t uiRet = TASK_CHECK_SEQ_ERR;
    TskRTSTaskInit();

    while(1)
    {
        //uiRet = TaskCheckRunSeq(0, RTS_TASK_SEQ, MAX_TASK_SEQ_NUM);

        //if(TASK_CHECK_SEQ_OK == uiRet)
        {
            TskRTSTaskExeEntry();
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExecuteUP1
*
* 功能描述: 执行UP1用户程序
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
static void ExecuteUP1(void)
{
    pUP1Func = (UP1Entry)SharedGetTaskCodeEntry(TASK_UP1);
    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    static uint32_t s_uiExeTime = 0U;

    if((pUP1Func != NULL) && (pucDataAreaAddr != NULL))
    {
        pUP1Func((uint32_t)pucDataAreaAddr);
        s_uiExeTime++;
		#ifdef CONTROL_CYCLE_20MS
        printf("UP1 %d\n",s_uiExeTime);
		#endif
    }
    else
    {
		#ifdef CONTROL_CYCLE_20MS
        printf("U1E %d",pUP1Func);
		#endif
    }
	return;
}

/*
***********************************************************************************************************************
* 函数名称: TskUP1TaskExe
*
* 功能描述: UP1周期执行函数
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
void TskUP1TaskExe (void)
{
    //printf("\n   #------------UP1 TASK------------------#\n");
    uint32_t uiShedTime = 0;

    s_uiUP1ExeTime++;
	#ifdef CONTROL_CYCLE_20MS
    printf("U1=%d\n",s_uiUP1ExeTime);
	#endif
    s_iU1Pend = os_sem_pend(s_iRTStoUP1Sem, 0, OS_OPT_PEND_NON_BLOCKING, &s_uiU1Err);
    if(s_uiU1Err != OS_ERR_NONE)
    {
        //printf("UP1 task get s_iRTStoUP1Sem failed.\n");
    }
    else
    {
        if(0 == s_iU1Pend)
        {
//                    printf("S\n");
            s_bUP1StartFlag = UP_TASK_RUNNING_START;
        }
    }

    if(UP_TASK_RUNNING_START == s_bUP1StartFlag)
    {
        /* Execute user program 1 */
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
//                    printf("UP1\n");
            //SysExchangeUP2toUP1();
            s_U1T1 = getTblus();

            if(bUP1DeltaFlag == true)
            {
                uiStartTime = SysGetSyncTimeLow32 ();
                uiUP1DeltaTime = (uiStartTime - uiUP1LastStartTime) / 10;
                uiUP1LastStartTime = uiStartTime;
            }
            else
            {/* 未经过运算 */
                uiShedTime = SysGetPrjShedTime(0);
                uiUP1DeltaTime = uiShedTime;
                uiUP1LastStartTime = SysGetSyncTimeLow32 ();
            }

            ExecuteUP1();
            s_U1T2 = getTblus();
            uiUP1IECTime = s_U1T2-s_U1T1;
            bUP1DeltaFlag = true;
        }

        os_sem_post(s_iUP1toRTSSem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &s_uiU1Err);

        if(s_uiU1Err != OS_ERR_NONE)
        {
            printf("UP1 task post s_uiUP1toRTSSem failed.\n");
        }
        s_bUP1StartFlag = UP_TASK_RUNNING_NOT_START;
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskUP1Task
*
* 功能描述: UP1周期任务入口函数
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
static void TskUP1Task (void)
{
    //uint32_t uiRet = TASK_CHECK_SEQ_ERR;

    while(1)
    {
        //uiRet = TaskCheckRunSeq(0, UP1_TASK_SEQ, MAX_TASK_SEQ_NUM);

        //if(TASK_CHECK_SEQ_OK == uiRet)
        {
            TskUP1TaskExeEntry();
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExecuteUP2
*
* 功能描述: 执行UP2用户程序
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
static void ExecuteUP2(void)
{
    pUP2Func = (UP2Entry)SharedGetTaskCodeEntry(TASK_UP2);
    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    static uint32_t s_uiExeTime = 0U;

    if((pUP2Func != NULL) && (pucDataAreaAddr != NULL))
    {
        pUP2Func((uint32_t)pucDataAreaAddr);
        s_uiExeTime++;
		#ifdef CONTROL_CYCLE_20MS
        printf("UP2\n");
		#endif
    }
    else
    {
		#ifdef CONTROL_CYCLE_20MS
        printf("U2E %d",pUP2Func);
		#endif
    }
	return;
}
/*
***********************************************************************************************************************
* 函数名称: TskUP2TaskExe
*
* 功能描述: UP2周期执行函数
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
void TskUP2TaskExe (void)
{
    //printf("\n   #------------UP2 TASK------------------#\n");
    uint32_t uiShedTime = 0;

    s_uiUP2ExeTime++;
	#ifdef CONTROL_CYCLE_20MS
    printf("U2=%d\n",s_uiUP2ExeTime);
    #endif
    s_iU2Pend = os_sem_pend(s_iRTStoUP2Sem, 0, OS_OPT_PEND_NON_BLOCKING, &s_uiU2Err);

    if(s_uiU2Err != OS_ERR_NONE)
    {
        //printf("UP2 task get s_iRTStoUP2Sem failed.\n");
    }
    else
    {
        if(0 == s_iU2Pend)
        {
            s_bUP2StartFlag = UP_TASK_RUNNING_START;
        }
    }

    if(UP_TASK_RUNNING_START == s_bUP2StartFlag)
    {
        /* Execute user program 2 */
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
//                    printf("UP2\n");
            //SysExchangeUP1toUP2();

            if(bUP2DeltaFlag == 1)
            {
                uiStartTime = SysGetSyncTimeLow32 ();
                uiUP2DeltaTime = (uiStartTime - uiUP2LastStartTime) / 10;
                uiUP2LastStartTime = uiStartTime;
            }
            else
            {/* 未经过运算 */
                uiShedTime = SysGetPrjShedTime(1);
                uiUP2DeltaTime = uiShedTime * 1000;
                uiUP2LastStartTime = SysGetSyncTimeLow32 ();
            }

            s_U2T1 = getTblus();
            ExecuteUP2();
            s_U2T2 = getTblus();
            uiUP2IECTime = s_U2T2-s_U2T1;
            bUP2DeltaFlag = true;
        }

        os_sem_post(s_iUP2toRTSSem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &s_uiU2Err);

        if(s_uiU2Err != OS_ERR_NONE)
        {
            printf("UP2 task post s_uiUP2toRTSSem failed.\n");
        }
        s_bUP2StartFlag = UP_TASK_RUNNING_NOT_START;
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskUP2Task
*
* 功能描述: UP2周期任务入口函数
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
static void TskUP2Task (void)
{
    //uint32_t uiRet = TASK_CHECK_SEQ_ERR;

    while(1)
    {
        //uiRet = TaskCheckRunSeq(0, UP2_TASK_SEQ, MAX_TASK_SEQ_NUM);

        //if(TASK_CHECK_SEQ_OK == uiRet)
        {
            TskUP2TaskExeEntry();
            TaskCheckRunSeq(0, MAX_TASK_SEQ_NUM, MAX_TASK_SEQ_NUM);
        }
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
* 函数名称: UpdateWatchdogCounter
*
* 功能描述: 更新看门狗计数值
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
static void UpdateWatchdogCounter(void)
{
    uint32_t uiFlag = 0;

    uiFlag = DiagCommGetCore0FailFlag();

    if(MASK_SETALL == uiFlag)
    {
        s_uiCounter++;

        SysSetSMWatchdogInfo(s_uiCounter);
        //printf("cnt=%d\n", s_uiCounter);
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
                                 文件结束
***********************************************************************************************************************
*/

