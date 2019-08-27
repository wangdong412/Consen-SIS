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
#include "Srv/SharedSys/inc/Sys.h"
#include "Srv/ExtLib/inc/ExtLib.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/Sys/inc/CommSysCommon.h"
#include "Srv/Diag/inc/DiagCommon.h"
#include "Srv/Log/inc/Log.h"
#include "Srv/Log/inc/LogID.h"
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

extern UINT32 fpga_tick;
extern unsigned long rtsRunning;
extern unsigned long rtsdebug;
/*-----------------The following variables are used by RTS task------------*/
static uint32_t uiErr = 0;
static int32_t iPend = -1;

static uint32_t uiStartTime = 0;
static long64_t ulStartTime = 0;
static uint32_t uiEndTime = 0;
static long64_t ulEndTime = 0;

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

static void exception_handler_SPE(char_t * cpPram);
static uint32_t GetCPUExpType(void);
/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
//void MainManagerCycle(void)
//{
//    static uint32_t y = 0;
//
//    /* 操作GPIO5每次翻转，喂狗 */
//    if(0 == y % 2)
//    {
//        sysGpioLineSet (GPIO_3, GPIO_HIGH);
//    }
//    else
//    {
//        sysGpioLineSet (GPIO_3, GPIO_LOW);
//    }
//    y++;
//    return;
//}

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
    bool_t bRet = false;
    bool_t bRet1 = false;
    bool_t bRet2 = false;

    /* 显示版本号 */
    ShowVersion();
    
    /* System resource module initial 初始化系统资源，共享数据区初始化在本core0中 */
    SharedSysInit();

    printf("SharedSysInit OK!\n");

    /* 判断插稳，如果未插稳，循环至看门狗复位 */
    do
    {
        bRet = GetLocalPMStabState();
        printf("PMStabState: %d\n", bRet);
    }while(bRet == false);

    /* 设置本系槽位，如果两套地址不一致，循环至看门狗复位 */
    do
    {
        bRet = SetLocalPMAddr();
        printf("LocalPMAddr: %d\n", bRet);
    }while(bRet == false);

    /* 第一次握手，如果core1判断未插稳或者两系槽位不一致，则core0，core1都不运行，等待看门狗复位 */
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE1_STEP0);
        udelay(500u);
    }while((SIGNAL_CORE0_STEP0 != uiSignal) || (true != bRet1) || (true != bRet2));

    printf("Handshake OK!\n");

    do
    {
        printf("Get SyncTime State!\n");
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
    exception_handler fpExceptFunc = exception_handler_SPE;

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

    registerException(EXCEPTION_SPE, fpExceptFunc, NULL, "SPE");

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
    DiagInit();//自检失败时触发死机或重启
    printf("DiagInit\n");

#if 1
    printf("Handshake\n");
    do
    {
        bRet1 = SysGetHandshakeSignal(CORE1_TO_CORE0, &uiSignal);
        udelay(500u);
        bRet2 = SysSetHandshakeSignal(CORE0_TO_CORE1, SIGNAL_CORE1_START);
        udelay(500u);
        //printf("Handshake: s%d %d %d\n", uiSignal,bRet1, bRet2);
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

    #ifdef CONTROL_CYCLE_5MS
    printCfg(0);
    #endif

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
    long64_t ulTestTime1 = 0;
    long64_t ulTestTime2 = 0;
    long64_t ulTestTime3 = 0;
    uint32_t ulTestinterval = 0;

    uiStartTime = SysGetSyncTimeLow32 ();
    ulStartTime = SysGetFreeCounterValue();

    s_uiRTSExeTime++;
	
	#ifdef CONTROL_CYCLE_20MS
    printf("RS=%d\n",uiStartTime);
	#endif

    /* 清除PMBUS数据标志 */
    clear_precvdata_status(1);
    clear_precvdata_status(2);

    /*输出*/
    DataProcessOutputHandle();

    ulTestTime1 = SysGetFreeCounterValue();
    if(ulTestTime1 - ulStartTime > 500)
    {
        ulTestinterval = (uint32_t)(ulTestTime1 - ulStartTime);
//        printCfg(1);
//        printf("OO=%d\n",ulTestinterval);
//        printCfg(0);
        LogWrite(LOG0_ID_APP_MAIN_LEVEL0_OUTPUT_TO);
    }

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
            (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP1)) ||
            (TASK_SINGLE_STEP == SysGetTaskSingleStepFlg(TASK_UP1)))
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
            (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP2)) ||
            (TASK_SINGLE_STEP == SysGetTaskSingleStepFlg(TASK_UP2)))
         {
             SysSetOutputPollFlg(TASK_UP2, OUTPUT_POLL_ENABLE);
         }
     }

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
    DiagCycle();

    /* Monitor module cycle function */
    MonitorCycle();

    /* 三系同步开始进行输入处理 */
    uiEndTime = SysGetSyncTimeLow32 ();
    ulEndTime = SysGetFreeCounterValue();
	
	#ifdef CONTROL_CYCLE_20MS
    if(ulEndTime - ulStartTime < 8000)
    {
        //printf("end=%d %d\n",uiEndTime,uiEndTime - uiStartTime);
        udelay(8000-(ulEndTime - ulStartTime));
    }
	#endif

	#ifdef CONTROL_CYCLE_5MS
    if(ulEndTime - ulStartTime < 2500)
    {
        udelay(2500-(ulEndTime - ulStartTime));
    }
    else
    {
//        printCfg(1);
//        printf("T0\n");
//        printCfg(0);
        LogWrite(LOG0_ID_APP_MAIN_LEVEL0_MAIN_TO);
    }
	#endif	
	
    ulTestTime2 = SysGetFreeCounterValue();

    /* 清除PMBUS数据标志 */
    clear_precvdata_status(1);
    clear_precvdata_status(2);

    /*输入*/
    DataProcessInputHandle();

    /* ------test------ */
    ulTestTime3 = SysGetFreeCounterValue();
    if(ulTestTime3 - ulTestTime2 > 1000)
    {
        ulTestinterval = (uint32_t)(ulTestTime3 - ulTestTime2);
//        printCfg(1);
//        printf("IO=%d\n",ulTestinterval);
//        printCfg(0);
        LogWrite(LOG0_ID_APP_MAIN_LEVEL0_INPUT_TO);
    }
    /* ------test------ */

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
    bool_t bDeltaFlag = false;
    uint32_t uiDeltaTime = 0;

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

            bDeltaFlag = SharedGetDelataFlag(TASK_UP1);
            if(bDeltaFlag == true)
            {
                uiStartTime = SysGetSyncTimeLow32 ();
                uiDeltaTime = (uiStartTime - uiUP1LastStartTime) / 10;
                SharedSetDelataTime(TASK_UP1, uiDeltaTime);
                uiUP1LastStartTime = uiStartTime;
            }
            else
            {/* 未经过运算 */
                uiShedTime = SysGetPrjShedTime(0);
                uiDeltaTime = uiShedTime;
                SharedSetDelataTime(TASK_UP1, uiDeltaTime);
                uiUP1LastStartTime = SysGetSyncTimeLow32 ();
            }

            ExecuteUP1();
            s_U1T2 = getTblus();
            uiUP1IECTime = s_U1T2-s_U1T1;
            bDeltaFlag = true;
            SharedSetDelataFlag(TASK_UP1, bDeltaFlag);
            SharedSetFirstScanFlag(TASK_UP1, true);
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
    bool_t bDeltaFlag = false;
    uint32_t uiDeltaTime = 0;

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

            bDeltaFlag = SharedGetDelataFlag(TASK_UP2);
            if(bDeltaFlag == true)
            {
                uiStartTime = SysGetSyncTimeLow32 ();
                uiDeltaTime = (uiStartTime - uiUP2LastStartTime) / 10;
                SharedSetDelataTime(TASK_UP2, uiDeltaTime);
                uiUP2LastStartTime = uiStartTime;
            }
            else
            {/* 未经过运算 */
                uiShedTime = SysGetPrjShedTime(1);
                uiDeltaTime = uiShedTime * 1000;
                SharedSetDelataTime(TASK_UP2, uiDeltaTime);
                uiUP2LastStartTime = SysGetSyncTimeLow32 ();
            }

            s_U2T1 = getTblus();
            ExecuteUP2();
            s_U2T2 = getTblus();
            uiUP2IECTime = s_U2T2-s_U2T1;
            bDeltaFlag = true;
            SharedSetDelataFlag(TASK_UP2, bDeltaFlag);
            SharedSetFirstScanFlag(TASK_UP2, true);
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
* 函数名称: GetCPUExpType
*
* 功能描述: CPU异常类型
*
* 输入参数:
* 输出参数: 无
*
* 返 回 值  : 异常类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t GetCPUExpType(void)
{
    uint32_t uiType = 0;
    uiType = os_get_spescr();
    return uiType;
}

/*
***********************************************************************************************************************
* 函数名称: exception_handler_SPE
*
* 功能描述: 浮点异常处理函数
*
* 输入参数:
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void exception_handler_SPE(char_t * cpPram)
{
    uint32_t uiErrType = 0;
    //printe("oo\n");
    /* get exception type */
    uiErrType = GetCPUExpType();
    if(uiErrType  == CPU_EXP_DIV_ZERO)
    {
        SharedSetCheckErr(CPU_EXP_DIV_ZERO);
        LogWrite(LOG0_ID_EXP_LEVEL0_SPE);
    }
    else if(uiErrType  == CPU_EXP_OVERFLOW)
    {
        SharedSetCheckErr(CPU_EXP_OVERFLOW);
        LogWrite(LOG0_ID_EXP_LEVEL0_SPE1);
    }
    else if(uiErrType  == CPU_EXP_UNDERFLOW)
    {
        SharedSetCheckErr(CPU_EXP_UNDERFLOW);
        LogWrite(LOG0_ID_EXP_LEVEL0_SPE2);
    }
    if(uiErrType  == CPU_EXP_INVALID_INPUT)
    {
        SharedSetCheckErr(CPU_EXP_INVALID_INPUT);
        LogWrite(LOG0_ID_EXP_LEVEL0_SPE3);
    }
    else
    {

    }

    return;
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


