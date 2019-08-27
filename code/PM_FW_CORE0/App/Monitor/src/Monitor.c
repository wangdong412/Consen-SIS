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
*  文件名称    : Monitor.c
*
*  功能简介    : 
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-28
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
#include "../inc/Monitor.h"
#include "App/DataProcess/inc/DataProcess.h"
#include "Srv/SharedSys/inc/Sys.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SharedMem/inc/SharedRetain.h"
#include "Srv/Sys/inc/CommSysSync.h"
#include "Srv/ExtLib/inc/ExtLib.h"
#include "Srv/SafetyProtocol/inc/P2PSafetyComm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t s_ucDataAreaInitFlg[MAX_TASK_NUM];
static ulong64_t s_ullTaskNextShedTime[MAX_TASK_NUM];
static uint8_t s_ucSingleStepCount[MAX_TASK_NUM];
static WriteVarArray_t s_stWriteVarArray[MAX_TASK_NUM];
static ForceItemArray_t s_stForceItemArray[MAX_TASK_NUM];
static P2PRecvReqCtrl_t s_stP2PRecvReqCtrl[MAX_P2P_BLOCK_NUM+1];
//static uint8_t s_ucIECCtrlFlg[MAX_TASK_NUM];
/* SCANSTATUS 外部库临时增加 */
uint32_t uiUP1ScanSurplus[100] = {0};
uint8_t ucUP1ScanOverrun[100] = {0};
uint8_t ucUP1ScanSurplusIndex = 0;
uint8_t ucUP1ScanSurplusFullFlag = 0;
uint8_t ucUP1ScanOverrunIndex = 0;

static uint32_t uiUP1AverageScanSurplus;
static uint32_t uiUP1AverageScanOverrun;

uint32_t uiUP2ScanSurplus[100] = {0};
uint8_t ucUP2ScanOverrun[100] = {0};
uint8_t ucUP2ScanSurplusIndex = 0;
uint8_t ucUP2ScanSurplusFullFlag = 0;
uint8_t ucUP2ScanOverrunIndex = 0;

static uint32_t uiUP2AverageScanSurplus;
static uint32_t uiUP2AverageScanOverrun;

extern uint32_t uiUP1IECTime;
extern uint32_t uiUP2IECTime;
                           
//debug
extern unsigned long pcstack[512];
extern unsigned long pcstack1[512];
extern unsigned long pn;
extern unsigned long pn1;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void MonitorIECCycleCtrl1(void);

static void MonitorIECCycleCtrl2(void);

static void MonitorSetTaskNextShedTime(Task_ID_t emTaskID, ulong64_t ullTime);

static ulong64_t MonitorGetTaskNextShedTime(Task_ID_t emTaskID);

static void MonitorSetTaskDataInitFlg(Task_ID_t emTaskID, uint8_t ucDataAreaInitFlg);

static uint8_t MonitorGetTaskDataInitFlg(Task_ID_t emTaskID);

static void MonitorServicPreprocess(void);

static void MonitorModbusPreprocess(void);

static void MonitorModbusWriteVarPreHandle(uint8_t ucModbusData[], uint32_t usModbusDataLen);

static void MonitorCS1131Preprocess(void);

static uint16_t MonitorCS1131StartRun(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131StopRun(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131PauseRun(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131SingleStep(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131WriteVarPreHandle(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131DisableVarPreHandle(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131OSCPStart(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static uint16_t MonitorCS1131OSCPDone(uint8_t ucCS1131Data[],uint8_t ucACKData[]);

static void MonitorOSCPHandle(Task_ID_t emTaskID);

static void MonitorWriteVarHandle (Task_ID_t emTaskID);

static void MonitorForceVarHandle (Task_ID_t emTaskID);

static void MonitorOPCPreprocess(void);

static uint16_t MonitorOPCWriteVarPreHandle (uint8_t ucOPCData[], uint8_t ucACKData[]);

static void MonitorRetainVarHanlde (Task_ID_t emTaskID);

static void MonitorRecoveryRetainVar(void);

static void MonitorCtrlSrvProcess(Task_ID_t emTaskID);

static void MonitorP2PPreprocess(void);

static uint16_t MonitorP2PREQPreHandle(uint8_t ucP2PData[], uint32_t usP2PDataLen,uint8_t ucACKData[],
                                       CMController_t emCMID);

static bool_t MakePMP2PRespMsg( P2PRecvReqHead_t const *pstP2PReqHead, uint8_t ucACKData[],
                                  puint16_t pusLen, CMController_t emCMID );

static void MonitorOnlineDownLoadSwitchState(Task_ID_t emTaskID);

static void MonitorDisableTablProcess(void);




/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: MonitorInit
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
void MonitorInit(void)
{
    uint8_t ucTaskIndex= 0U;
    PMController_t emPMId;
    
    for(emPMId = PMA; emPMId < NUM_OF_PM; emPMId++)
    {
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            SysSetTaskStatus(emPMId, ucTaskIndex, TS_INIT);
        }
    }
    
    for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
    {
        SysSetIECShedCtrlFlg(ucTaskIndex, IEC_END);
        SysSetIOPollFlg(ucTaskIndex, IO_POLL_END);
        SysSetOutputPollFlg(ucTaskIndex, OUTPUT_POLL_DISENABLE);
    }
    
    memset(&s_ucDataAreaInitFlg[0], 0x00, sizeof(uint8_t)*MAX_TASK_NUM);
    memset(&s_ullTaskNextShedTime[0], 0x00, sizeof(ulong64_t)*MAX_TASK_NUM);
    memset(&s_ucSingleStepCount[0], 0x00, sizeof(uint8_t)*MAX_TASK_NUM);
    memset(&s_stWriteVarArray[0], 0x00, sizeof(WriteVarArray_t)*MAX_TASK_NUM);
    memset(&s_stForceItemArray[0], 0x00, sizeof(ForceItemArray_t)*MAX_TASK_NUM);
    memset(&s_stP2PRecvReqCtrl[0], 0, sizeof(P2PRecvReqCtrl_t)*(MAX_P2P_BLOCK_NUM+1));
//    memset(&s_ucIECCtrlFlg[0], 0x00, sizeof(uint8_t)*MAX_TASK_NUM);

    P2PCommInitRecv();
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCycle
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
void MonitorCycle(void)
{
    static uint8_t ucFirstFlag = SYS_FIRST_IN;
    PMController_t emLocalPMId = SysGetLocalPMID(); /* 本地PM ID号 */
    uint8_t ucWriteVarFlag = 0U;
    uint8_t ucP2PRecvID = 0U;
    uint32_t uiRecvTimeout = 0U;  /* ms */
    P2PBlock_t* pstP2PBlock = NULL;
    P2PRecvReqCtrl_t *pstRecvCtrl = NULL;
    
    /* P2P Receive Request Control */
    for( ucP2PRecvID = 1U; ucP2PRecvID <= (uint8_t)(MAX_P2P_BLOCK_NUM); ucP2PRecvID++ )
    {
        pstRecvCtrl = &s_stP2PRecvReqCtrl[ucP2PRecvID];

        if( HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            pstP2PBlock = SharedGetP2PRecvBlockInfo( ucP2PRecvID );
            if((pstP2PBlock != NULL) && (ACTIVE == pstP2PBlock->emActive))
            {
                if( pstRecvCtrl->bHasRecv )
                {
                    /* Check if recv timeout */
                    pstRecvCtrl->uiIntrCycCnt++;
                    uiRecvTimeout = pstP2PBlock->stP2PBlockHead.uiPeriod; /* ms */
                    if(((uiRecvTimeout*15)/10) < (uint32_t)(P2P_TIMEOUT_TIME))
                    {
                        uiRecvTimeout = (uint32_t)(P2P_TIMEOUT_TIME);
                    }
                    else
                    {
                        uiRecvTimeout = (uiRecvTimeout*15)/10;
                    }

                    if( pstRecvCtrl->uiIntrCycCnt > (10*uiRecvTimeout/(uint32_t)(TASK_PERIOD)))
                    {
                        /* Recv Timeout */
                        P2PHandleRecvReqTO( ucP2PRecvID );
                        pstRecvCtrl->uiIntrCycCnt = 0U;
                    }
                }
            }
        }
        else
        {
            memset( pstRecvCtrl, 0, sizeof(P2PRecvReqCtrl_t));
        }
    }
    
    MonitorIECCycleCtrl1();

    if(IEC_END == SysGetIECShedCtrlFlg(TASK_UP1))
    {
        if(s_stWriteVarArray[TASK_UP1].uiWriteIndex > 0U)
        {
            MonitorWriteVarHandle(TASK_UP1);
            ucWriteVarFlag = 1U;
        }
        if(s_stForceItemArray[TASK_UP1].uiWriteIndex > 0U)
        {
            MonitorForceVarHandle(TASK_UP1);
            ucWriteVarFlag = 1U;
        }
    }
    
    if(IEC_END == SysGetIECShedCtrlFlg(TASK_UP2))
    {
        if(s_stWriteVarArray[TASK_UP2].uiWriteIndex > 0U)
        {
            MonitorWriteVarHandle(TASK_UP2);
            ucWriteVarFlag = 1U;
        }
        if(s_stForceItemArray[TASK_UP2].uiWriteIndex > 0U)
        {
            MonitorForceVarHandle(TASK_UP2);
            ucWriteVarFlag = 1U;
        }
    }
    if(ucWriteVarFlag != 1U)
    {
        MonitorServicPreprocess();
    }
    
    DataProcessInputRetainHandle();
    
    MonitorDisableTablProcess();
    
    if((IEC_END == SysGetIECShedCtrlFlg(TASK_UP1)) &&
       (SYS_FIRST_IN != ucFirstFlag) &&
       ((TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP1)) ||
        (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP1)) ||
        (TS_STOP == SysGetTaskStatus(emLocalPMId,TASK_UP1))))
    {
        MonitorRetainVarHanlde(TASK_UP1);
    }
    
    if((IEC_END == SysGetIECShedCtrlFlg(TASK_UP2)) &&
       (SYS_FIRST_IN != ucFirstFlag) &&
       ((TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP2)) ||
        (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP2)) ||
        (TS_STOP == SysGetTaskStatus(emLocalPMId,TASK_UP2))))
    {
        MonitorRetainVarHanlde(TASK_UP2);
    }
    

    MonitorIECCycleCtrl2();
    
    if((SYS_FIRST_IN == ucFirstFlag) && (HAS_BE_CONFIGURED == SysGetIsConfigured()))
    {
        MonitorRecoveryRetainVar();
        /* 上电时恢复retain数据后进行软SOE初始化 */
        DataProcessSoftSOEInit(TASK_UP1, true);
        DataProcessSoftSOEInit(TASK_UP2, true);
    }
    else
    {
        if(SYS_FIRST_IN == ucFirstFlag)
        {
            DEBUG_MONITOR("NOT_HAS_BE_CONFIGURED \n");
        }
    }
    
    ucFirstFlag = SYS_NOFIRST_IN;

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: MonitorIECCycleCtrl1
*
* 功能描述: IEC任务的周期控制函数1
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
static void MonitorIECCycleCtrl1(void)
{
    uint8_t ucTaskIndex= 0U;
    uint8_t ucIECShedCtrlFlg = IEC_END;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    task_status_t emTaskStat;
    SysIoModInfo_t* pstIoModInfo = NULL;
    uint32_t uiSysTime = 0;

    for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
    {
        emTaskStat = SysGetTaskStatus(emLocalPMId,ucTaskIndex);
        ucIECShedCtrlFlg = SysGetIECShedCtrlFlg(ucTaskIndex);

        if(ucIECShedCtrlFlg != IEC_END)
        {
            if((TS_RUN == emTaskStat) || (TASK_SINGLE_STEP == SysGetTaskSingleStepFlg(ucTaskIndex)))
            {
                SysSetIECShedCtrlFlg(ucTaskIndex, IEC_CONTINUE);
            }
            if((TS_STOP == emTaskStat) || (TS_PAUSE == emTaskStat))
            {
                SysSetIECShedCtrlFlg(ucTaskIndex, IEC_END);
            }
        }
        else
        {/*本周期IEC运算完成*/
            
//            DEBUG_MONITOR("  IEC_END  \n");

            if(NEED_INIT == MonitorGetTaskDataInitFlg(ucTaskIndex))
            {/*若任务数据区需要初始化，进行数据初始化*/
//                SysInitTaskDataArea(ucTaskIndex);/*指定任务的数据区初始化函数，！！！*/
                /*初始化数据区后，更新数据初始化标志*/
                MonitorSetTaskDataInitFlg(ucTaskIndex, NOT_NEED_INIT);
            }

            if((TASK_SINGLE_STEP == SysGetTaskSingleStepFlg(ucTaskIndex)) && (s_ucSingleStepCount[ucTaskIndex] != 0U))
            {/*任务单步且执行次数不为0,更新任务状态为暂停*/
                SysSetTaskStatus(emLocalPMId, ucTaskIndex, TS_PAUSE);
                SysSetTaskSingleStepFlg(ucTaskIndex, TASK_NOT_SINGLE_STEP);
                
                s_ucSingleStepCount[ucTaskIndex] = 0U;
            }
        }
    }
    
    pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
    if((pstIoModInfo != NULL))
    {
        if(0 == pstIoModInfo->stSysPMInfo.usPPMSource)
        {
            uiSysTime = SysGetTimingPulse();
//            printf("SysTime0:%d\n",uiSysTime);
            if(((uiSysTime % (60*1000000)) - (30*1000000)) < CFG_MINIMUM_BEAT_TIME*100)
            {
                printf("SysTime1:%d\n",uiSysTime);
                SysSetTimingPulse();
                uiSysTime = SysGetTimingPulse();
                printf("SysTime2:%d\n",uiSysTime);
            }
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorIECCycleCtrl2
*
* 功能描述: IEC任务的周期控制函数2
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
static void MonitorIECCycleCtrl2(void)
{
    uint8_t ucTaskIndex= 0U;
    uint32_t uiShedTime = 0U;
    SyncTime64_t unCurTime;                                         /* 当前同步时间             */
    SyncTime64_t unNextTime;                                        /* 下个工程调度时间         */
    SyncTime64_t unTempTime;                                        /* 临时时间，用于计算       */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    task_status_t emTaskStat;
    static task_status_t s_emLastTaskStat[MAX_TASK_NUM];
    uint32_t uiTaskSingleStepFlg = 0U;
    static uint8_t ucFirstFlg = 0U;
    static Key_Switch_Type_t emOldSwitchKey = KS_INIT;
    Key_Switch_Type_t emSwitchKey = KS_INIT;
    GlobaInit GlobalInitPtr = NULL;  
    puint8_t pucDataAreaAddr = NULL;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;
    uint32_t uiOffset = 0U;
    uint32_t uiUsedDataLen = 0U;
    uint32_t uiCycle = 0;
    SysIoModInfo_t* pstIoModInfo = NULL;
    uint8_t i= 0U;
    uint32_t uiUP1AverageScanSurplusSum = 0;
    uint32_t uiUP2AverageScanSurplusSum = 0;
    SharedAppend_t *pstAppend = NULL;
    
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    emSwitchKey = SharedGetSwitchPos();
    
    if(emOldSwitchKey != emSwitchKey)
    {
        emOldSwitchKey = emSwitchKey;
    }

    if(KS_RUN == emSwitchKey)
    {
        if((HAS_BE_CONFIGURED == SysGetIsConfigured()))
        {
            pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP1);
            if(ACTIVE == pstTaskConfigInfo->emActiveFlag)
            {
                if(DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP1))
                {
                    SysSetTaskStatus(emLocalPMId, TASK_UP1, TS_RUN);
                }
            }
            pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP2);
            if(ACTIVE == pstTaskConfigInfo->emActiveFlag)
            {
                if(DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP2))
                {
                    SysSetTaskStatus(emLocalPMId, TASK_UP2, TS_RUN);
                }
            }
        }
    }
    if(KS_STOP == emSwitchKey)
    {
        if((HAS_BE_CONFIGURED == SysGetIsConfigured()))
        {
            pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP1);
            if(ACTIVE == pstTaskConfigInfo->emActiveFlag)
            {
                SysSetTaskStatus(emLocalPMId, TASK_UP1, TS_STOP);
            }
            pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP2);
            if(ACTIVE == pstTaskConfigInfo->emActiveFlag)
            {
                SysSetTaskStatus(emLocalPMId, TASK_UP2, TS_STOP);
            }
        }
    }
    
    if(NEED_POWERON_CONIFG_SWITCH == SharedGetPowerOnConfigSwitchFlg())
    {
        SharedSwitchAllConfigInfo();

        pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
        if((pstIoModInfo != NULL))
        {
            SysSelectAdjustTimePulseSrc(pstIoModInfo->stSysPMInfo.usPPMSource);
            SysSelectAdjustTimePulsePolarity(pstIoModInfo->stSysPMInfo.usPPMPolarity);
        }

        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(SharedGetTaskActive(ucTaskIndex) == ACTIVE)
            {
                SysSetTaskStatus(SysGetLocalPMID(),ucTaskIndex, TS_STOP);
            }
        }

        DataProcessSlaveConfirmInit();
        
        SharedSetPowerOnConfigSwitchFlg(NOT_NEED_POWERON_CONIFG_SWITCH);
        
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            SharedBackupOldCMSlotIdTabl();
        }
    }

    for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
    {
        emTaskStat = SysGetTaskStatus(emLocalPMId,ucTaskIndex);
        pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].ucUPState = (uint8_t)emTaskStat;

//        if(s_emLastTaskStat[ucTaskIndex] != emTaskStat)
//        {
//            if((TS_STOP == emTaskStat) && (s_emLastTaskStat[ucTaskIndex] != TS_INIT))
//            {/*当任务状态由其他状态变化为STOP时，对任务进行数据区初始化*/
//                pstTaskConfigInfo = SharedGetTaskConfigInfo(ucTaskIndex);
//                GlobalInitPtr = (GlobaInit)SharedGetTaskGlobalInitEntry(ucTaskIndex);
//                
//                if((pstTaskConfigInfo != NULL) && (ACTIVE == pstTaskConfigInfo->emActiveFlag))
//                {
//                    uiOffset = pstTaskConfigInfo->uiGOffset;
//                    uiUsedDataLen = pstTaskConfigInfo->uiGUsedSize;
//                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
//                    
//                    if(((uiOffset + uiUsedDataLen) < MAX_RTDATA_G_AREA_LEN) && (pucDataAreaAddr != NULL))
//                    {
//                        pucDataAreaAddr += uiOffset;
//                        memset(pucDataAreaAddr,0,uiUsedDataLen);
//                    }
//                    
//                    uiOffset = pstTaskConfigInfo->uiIOffset;
//                    uiUsedDataLen = pstTaskConfigInfo->uiIUsedSize;
//                    
//                    uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN;
//                    
//                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_I);
//                    
//                    if(((uiOffset + uiUsedDataLen) < MAX_RTDATA_I_AREA_LEN) && (pucDataAreaAddr != NULL))
//                    {
//                        pucDataAreaAddr += uiOffset;
//                        memset(pucDataAreaAddr,0,uiUsedDataLen);
//                    }
//                    
//                    uiOffset = pstTaskConfigInfo->uiOOffset;
//                    uiUsedDataLen = pstTaskConfigInfo->uiOUsedSize;
//                    
//                    uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN - MAX_RTDATA_I_AREA_LEN;
//                    
//                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_Q);
//                    
//                    if(((uiOffset + uiUsedDataLen) < MAX_RTDATA_Q_AREA_LEN) && (pucDataAreaAddr != NULL))
//                    {
//                        pucDataAreaAddr += uiOffset;
//                        memset(pucDataAreaAddr,0,uiUsedDataLen);
//                    }
//                    
//                    pucDataAreaAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);
//                    
//                    if(((uiOffset + uiUsedDataLen) < MAX_RTDATA_Q_AREA_LEN) && (pucDataAreaAddr != NULL))
//                    {
//                        pucDataAreaAddr += uiOffset;
//                        memset(pucDataAreaAddr,0,uiUsedDataLen);
//                    }
//                }
//
//                if((HAS_BE_CONFIGURED == SysGetIsConfigured()))
//                {
//                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
//                    
//                    if((pucDataAreaAddr != NULL) && (GlobalInitPtr != NULL))
//                    {
//                        #ifdef CONTROL_CYCLE_20MS
//                        printf("Stop Init UP%d 0x%x 0x%x\n", ucTaskIndex, GlobalInitPtr);
//                        #endif
//                        GlobalInitPtr((uint32_t)pucDataAreaAddr);
//                        #ifdef CONTROL_CYCLE_20MS
//                        printf("ST:%d \n", SysGetPrjShedTime(ucTaskIndex));
//                        #endif
//                    }
//                    
////                    DataProcessOutputDataCopy((uint32_t)ucTaskIndex); /* 输出数据从运算数据区Q区拷贝到临时数据区Q区 */
//                }
//            }
//        }

        uiTaskSingleStepFlg = SysGetTaskSingleStepFlg(ucTaskIndex);

        /* 如果本周期状态为停止或者暂停时，将初次运算的标志清0 */
        if((TS_STOP == emTaskStat) || (TS_PAUSE == emTaskStat) || (PF_PAUSE == SharedGetPauseFlag(ucTaskIndex)))
        {
            SharedSetDelataFlag(ucTaskIndex, false);
        }

        if((TS_STOP == emTaskStat) || (TS_RUN == emTaskStat) ||
           (TS_PAUSE == emTaskStat) || (TASK_SINGLE_STEP == uiTaskSingleStepFlg))
        {
            /* 获取当前PM同步时间 */
            SysGetSyncTime((SyncTime64_t*)&unCurTime);
            
            /* 获取任务的工程调度周期 */
            uiShedTime = SysGetPrjShedTime(ucTaskIndex);
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanTime = uiShedTime*1000;

            /* 填写append的相关信息 */
            if(ucTaskIndex == TASK_UP1)
            {
                /* 计算实际使用的周期数 */
                uiCycle = uiUP1IECTime / 5000;
                if((uiUP1IECTime % 5000) > 1000)
                {/* 剩余部分超过一个周期 */
                    uiCycle = uiCycle + 1;
                }
                /* 除去非任务1部分 */
                pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime = uiUP1IECTime - uiCycle * 4000;
                if(TS_STOP == emTaskStat)
                {
                    uiUP1IECTime = 0; /* 如果状态是停止，本次使用后清0 */
                }

                /* 剩余时间 */
                if((uiShedTime / 5 * 1000) > pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime)
                {
                    pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus = uiShedTime / 5 * 1000 -
                            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime;
                }
                else
                {
                    pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus = 0;
                }

                uiUP1ScanSurplus[ucUP1ScanSurplusIndex] = pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus / 1000;
                ucUP1ScanSurplusIndex = ucUP1ScanSurplusIndex + 1;

                if(ucUP1ScanSurplusIndex == 100)
                {
                    ucUP1ScanSurplusFullFlag = 1;
                    ucUP1ScanSurplusIndex = 0;
                }

                if(ucUP1ScanSurplusFullFlag == 1)
                {
                    for(i = 0; i < 100; i++)
                    {
                        uiUP1AverageScanSurplusSum = uiUP1AverageScanSurplusSum + uiUP1ScanSurplus[i];
                    }
                    uiUP1AverageScanSurplus = uiUP1AverageScanSurplusSum / 100;
                    SharedSetAvgScanSurplus(TASK_UP1, uiUP1AverageScanSurplus);
                }
                else
                {
                    for(i = 0; i < ucUP1ScanSurplusIndex; i++)
                    {
                        uiUP1AverageScanSurplusSum = uiUP1AverageScanSurplusSum + uiUP1ScanSurplus[i];
                    }
                    uiUP1AverageScanSurplus = uiUP1AverageScanSurplusSum / ucUP1ScanSurplusIndex;
                    SharedSetAvgScanSurplus(TASK_UP1, uiUP1AverageScanSurplus);
                }
            }
            if(ucTaskIndex == TASK_UP2)
            {
                uiCycle = uiUP2IECTime / 5000;
                if((uiUP2IECTime % 5000) > 500)
                {
                    uiCycle = uiCycle + 1;
                }
                pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime = uiUP2IECTime - uiCycle * 4500;
                if(TS_STOP == emTaskStat)
                {
                    uiUP2IECTime = 0; /* 如果状态是停止，本次使用后清0 */
                }

                if((uiShedTime / 5 * 500) > pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime)
                {
                    pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus = uiShedTime / 5 * 500 -
                            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPIECTime;
                }
                else
                {
                    pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus = 0;
                }

                uiUP2ScanSurplus[ucUP1ScanSurplusIndex] = pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].uiUPScanSurplus / 1000;
                ucUP2ScanSurplusIndex = ucUP2ScanSurplusIndex + 1;

                if(ucUP2ScanSurplusIndex == 100)
                {
                    ucUP2ScanSurplusFullFlag = 1;
                    ucUP2ScanSurplusIndex = 0;
                }

                if(ucUP2ScanSurplusFullFlag == 1)
                {
                    for(i = 0; i < 100; i++)
                    {
                        uiUP2AverageScanSurplusSum = uiUP2AverageScanSurplusSum + uiUP2ScanSurplus[i];
                    }
                    uiUP2AverageScanSurplus = uiUP2AverageScanSurplusSum / 100;
                    SharedSetAvgScanSurplus(TASK_UP2, uiUP2AverageScanSurplus);
                }
                else
                {
                    for(i = 0; i < ucUP2ScanSurplusIndex; i++)
                    {
                        uiUP2AverageScanSurplusSum = uiUP2AverageScanSurplusSum + uiUP2ScanSurplus[i];
                    }
                    uiUP2AverageScanSurplus = uiUP2AverageScanSurplusSum / ucUP2ScanSurplusIndex;
                    SharedSetAvgScanSurplus(TASK_UP2, uiUP2AverageScanSurplus);
                }
            }

            uiShedTime = uiShedTime*10;/*转化为百微秒*/
            
            if((uiShedTime != 0U ) && (0U == (uiShedTime % CFG_MINIMUM_BEAT_TIME)) && (HAS_BE_CONFIGURED == SysGetIsConfigured()))
            {
                /* 计算下个工程调度周期起始时间为 */
                unTempTime.ullTime = unCurTime.ullTime;
                
                if(0U == ucFirstFlg)
                {
                    /*计算并设置下一个IEC周期开始时间*/
                    unNextTime.ullTime = unCurTime.ullTime + uiShedTime - (unTempTime.stTimeStruct.uiLow32b % uiShedTime);
                    
                    /*将下一个IEC周期开始时间，记录下来*/
                    MonitorSetTaskNextShedTime(ucTaskIndex,unNextTime.ullTime);
                    
                    ucFirstFlg = 1U;
                }
                
                unNextTime.ullTime = MonitorGetTaskNextShedTime(ucTaskIndex);

                if((unCurTime.stTimeStruct.uiLow32b - unNextTime.stTimeStruct.uiLow32b) <= CFG_MINIMUM_BEAT_TIME)
                {/*由操作系统调度保证5ms小周期，当当前时间大于上一次记录的调度开始时间且不大于5ms时，新的一个IEC周期开始*/
                    
                    MonitorCtrlSrvProcess(ucTaskIndex);
                    
                    MonitorOSCPHandle(ucTaskIndex);
                    
                    if(SysGetIOPollFlg(ucTaskIndex) != IO_POLL_END)
                    {
                        /*error log*/
                        #ifdef CONTROL_CYCLE_20MS
                        printf("IO POLL NOT END %d\n",SysGetIOPollFlg(ucTaskIndex));
                        #endif
                    }
                    else
                    {/*输入轮询完成，若需要开始新一轮轮询*/
                        
                        /*设置新一轮轮询开始*/
                        SysSetIOPollFlg(ucTaskIndex,IO_POLL_START);

                        MonitorOnlineDownLoadSwitchState(ucTaskIndex);

                        if(((TS_RUN == emTaskStat) || (TASK_SINGLE_STEP == uiTaskSingleStepFlg)) && \
                           (PF_PAUSE != SharedGetPauseFlag(ucTaskIndex)))
                        {/*任务状态为运行和单步时，进行IEC运算*/
                            if(IEC_END != SysGetIECShedCtrlFlg(ucTaskIndex))
                            {
                                SharedAddIECErrTime(ucTaskIndex);
                                pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].ucUPTimeoutCnt = SharedGetIECErrTime(ucTaskIndex);
                                pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].ucUPTimeout = 0x01;
//                                s_ucIECCtrlFlg[ucTaskIndex] = NOT_ALLOW_IEC_RUN;
                                if(ucTaskIndex == TASK_UP1)
                                {
                                    ucUP1ScanOverrun[ucUP1ScanOverrunIndex] = 1;
                                    ucUP1ScanOverrunIndex = ucUP1ScanOverrunIndex + 1;
                                    if(ucUP1ScanOverrunIndex == 100)
                                    {
                                        ucUP1ScanOverrunIndex = 0;
                                    }
                                }

                                if(ucTaskIndex == TASK_UP2)
                                {
                                    ucUP2ScanOverrun[ucUP2ScanOverrunIndex] = 1;
                                    ucUP2ScanOverrunIndex = ucUP2ScanOverrunIndex + 1;
                                    if(ucUP2ScanOverrunIndex == 100)
                                    {
                                        ucUP2ScanOverrunIndex = 0;
                                    }
                                }

                                #ifdef CONTROL_CYCLE_20MS
                                printf("IEC NOT END%d\n", ucTaskIndex);
                                #if 0 //debug
                                unsigned int i = 0;
                                printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
                                for(i=0; i<511; i++)
                                {   
                                    printf("%d:0x%x ",i,pcstack[i]);
                                    if( i % 4 == 0x0)
                                    printf("\n");
                                }   
                                printf("\npn:%d \n",pn);
                                printf("----------------------------------------------\n");
                                for(i=0; i<511; i++)
                                {   
                                    printf("%d:0x%x ",i,pcstack1[i]);
                                    if( i % 4 == 0x0)
                                    printf("\n");
                                }   
                                printf("\npn1:%d \n",pn1);
                                #endif
                                #endif
                            }
                            else
                            {
                                if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg((Task_ID_t)ucTaskIndex))
                                {/* 已经经过一次IEC运算，可以进行输出数据处理 */
                                    /* 每运算一次，只处理一次SOE，拷贝一次数据 */
                                    DataProcessSoftSOEHandle((uint32_t)ucTaskIndex); /* 处理软SOE数据 */
                                    DataProcessOutputDataCopy((uint32_t)ucTaskIndex); /* 输出数据从运算数据区Q区拷贝到临时数据区Q区 */
                                }
                                
                                SysSetIECShedCtrlFlg(ucTaskIndex, IEC_START);


                                if(ucTaskIndex == TASK_UP1)
                                {
                                    ucUP1ScanOverrun[ucUP1ScanOverrunIndex] = 0;
                                    ucUP1ScanOverrunIndex = ucUP1ScanOverrunIndex + 1;
                                    if(ucUP1ScanOverrunIndex == 100)
                                    {
                                        ucUP1ScanOverrunIndex = 0;
                                    }
                                }

                                if(ucTaskIndex == TASK_UP2)
                                {
                                    ucUP2ScanOverrun[ucUP2ScanOverrunIndex] = 0;
                                    ucUP2ScanOverrunIndex = ucUP2ScanOverrunIndex + 1;
                                    if(ucUP2ScanOverrunIndex == 100)
                                    {
                                        ucUP2ScanOverrunIndex = 0;
                                    }
                                }

                                pstAppend->stPMState.stPMFirmwareState.stSharedUPState[ucTaskIndex].ucUPTimeout = 0x00;
                                #ifdef CONTROL_CYCLE_20MS
                                printf("IEC%d\n", ucTaskIndex);
                                #endif
                            }
                            uiUP1AverageScanOverrun = 0;
                            uiUP2AverageScanOverrun = 0;
                            for(i = 0; i < 100; i++)
                            {
                                uiUP1AverageScanOverrun = uiUP1AverageScanOverrun + ucUP1ScanOverrun[i];
                                uiUP2AverageScanOverrun = uiUP2AverageScanOverrun + ucUP2ScanOverrun[i];
                            }
                            SharedSetAvgScanOverrun(TASK_UP1, uiUP1AverageScanOverrun);
                            SharedSetAvgScanOverrun(TASK_UP2, uiUP2AverageScanOverrun);
                        }
                        else
                        {
                            if(TS_PAUSE == emTaskStat)
                            {
                                if(IEC_END == SysGetIECShedCtrlFlg(ucTaskIndex))
                                {
                                    if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg((Task_ID_t)ucTaskIndex))
                                    {/* 已经经过一次IEC运算，可以进行输出数据处理 */
                                        /* 每运算一次，只处理一次SOE，拷贝一次数据 */
                                        DataProcessSoftSOEHandle((uint32_t)ucTaskIndex); /* 处理软SOE数据 */
                                        DataProcessOutputDataCopy((uint32_t)ucTaskIndex); /* 输出数据从运算数据区Q区拷贝到临时数据区Q区 */
                                    }
                                }
                            }
                        }

                        if(TASK_SINGLE_STEP == uiTaskSingleStepFlg)
                        {
                            s_ucSingleStepCount[ucTaskIndex]++;
                        }
                    }
                }
                
                /*计算并设置下一个IEC周期开始时间*/
                unNextTime.ullTime = unCurTime.ullTime + uiShedTime - (unTempTime.stTimeStruct.uiLow32b % uiShedTime);
                
                /*将下一个IEC周期开始时间，记录下来*/
                MonitorSetTaskNextShedTime(ucTaskIndex,unNextTime.ullTime);
                
                if((TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP1)) || (TS_RUN == SysGetTaskStatus(emLocalPMId,TASK_UP2)) ||
                   (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP1)) || (TS_PAUSE == SysGetTaskStatus(emLocalPMId,TASK_UP2)))
                {/*有配置信息，当有任务处于RUN或PAUSE状态时，点亮Active灯*/
                    SetLEDState(LED_NAME_ACTIVE, LED_ON);
                }
                else
                {/*有配置信息，当有任务都不处于RUN或PAUSE状态时，点闪Active灯*/
                    SetLEDState(LED_NAME_ACTIVE, LED_BLINK);
                }
            }
        }
        else if(TS_INIT == emTaskStat)
        {
            MonitorCtrlSrvProcess(ucTaskIndex);
        }
        else
        {
            DEBUG_MONITOR(" TaskStat is Err \n");
        }
        
        s_emLastTaskStat[ucTaskIndex] = emTaskStat;
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: MonitorSetTaskNextShedTime
*
* 功能描述: IEC任务的周期控制函数2
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
static void MonitorSetTaskNextShedTime(Task_ID_t emTaskID, ulong64_t ullTime)
{
    s_ullTaskNextShedTime[emTaskID] = ullTime;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorGetTaskNextShedTime
*
* 功能描述: IEC任务的周期控制函数2
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
static ulong64_t MonitorGetTaskNextShedTime(Task_ID_t emTaskID)
{
    return s_ullTaskNextShedTime[emTaskID];
}

/*
***********************************************************************************************************************
* 函数名称: MonitorSetTaskDataInitFlg
*
* 功能描述: 设置指定任务的数据区初始化标志
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
static void MonitorSetTaskDataInitFlg(Task_ID_t emTaskID, uint8_t ucDataAreaInitFlg)
{
    s_ucDataAreaInitFlg[emTaskID] = ucDataAreaInitFlg;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorGetTaskDataInitFlg
*
* 功能描述: 获取指定任务的数据区初始化标志
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
static uint8_t MonitorGetTaskDataInitFlg(Task_ID_t emTaskID)
{
    return s_ucDataAreaInitFlg[emTaskID];
}

/*
***********************************************************************************************************************
* 函数名称: MonitorServicPreprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorServicPreprocess(void)
{
    MonitorCS1131Preprocess();

    MonitorOPCPreprocess();

    MonitorModbusPreprocess();
    
    MonitorP2PPreprocess();
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorModbusPreprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorModbusPreprocess(void)
{
    bool_t bRet = false;
    uint16_t usModbusDataLen = 0U;
    uint8_t ucModbusData[MAX_SM_MSG_LEN];
    CMController_t emCMID = INVALID_CM_ID;

    /* 从共享内存上获取Modbus服务数据 */
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        bRet = SysGetSMMsg(SM_MODBUS_REQ, emCMID, &ucModbusData[0], &usModbusDataLen);
        if((true == bRet) && (usModbusDataLen != 0U) && (usModbusDataLen < MAX_SM_MSG_LEN))
        {
//            printf("***\n");
            if( HAS_BE_CONFIGURED == SysGetIsConfigured())
            {
                MonitorModbusWriteVarPreHandle(&ucModbusData[0], usModbusDataLen);
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorModbusWriteVarPreHandle
*
* 功能描述: 完成对Modbus输入变量服务的预处理.
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
static void MonitorModbusWriteVarPreHandle(uint8_t ucModbusData[], uint32_t usModbusDataLen)
{
    CommModbusCommonHead_t stModbusHead;
    CommModbusMasterHead_t stModbusMasterHead;
    CommModbusSlaveHead_t stModbusSlaveHead;
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;    /* modbus主站配置信息地址 */
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;      /* modbus从站配置信息地址 */
    ModbusSlaveBlock_t* pstModbusSlaveBlock = NULL;    /* modbus从站的块配置信息地址 */
    ModbusMasterBlock_t* pstModbusMasterBlock = NULL;    /* modbus主站的块配置信息地址 */
    uint16_t usWriteVarNum = 0U;
    uint16_t usBuffIndex = 0U;
    uint16_t usCount = 0U;
    uint8_t ucTaskID = 0U;
    uint16_t usSize = 0U;
    uint32_t uiModbusIndex = 0U;
    uint32_t uiQuotient = 0U;
    uint32_t uiResidue  = 0U;
    uint8_t ucValue = 0U;
    uint16_t usModbusSlaveAddr = 0U;

    if(ucModbusData != NULL)
    {
        /* 读取modbus公共头结构 */
        memcpy(&stModbusHead, &ucModbusData[0], sizeof(CommModbusCommonHead_t));
        if(stModbusHead.ucPortType == MONITOR_MODBUS_MASTER)
        {/* modbus公共头结构中，类型为modbus主 */
            if(usModbusDataLen == stModbusHead.usDataLen + sizeof(CommModbusMasterHead_t))
            {/* 检查读取的数据长度和公共头结构中数据长度是否符合 */
                /* 读取modbus主头结构 */
                memcpy(&stModbusMasterHead, &ucModbusData[0], sizeof(CommModbusMasterHead_t));
                usBuffIndex += sizeof(CommModbusMasterHead_t);

                pstModbusMasterInfo = SharedGetModbusMasterInfo();
                if(pstModbusMasterInfo != NULL)
                {/* modbus主站配置信息地址正确 */
                    if(stModbusMasterHead.uiBlockID < MAX_MODBUS_BLOCK_NUM)
                    {
                        pstModbusMasterBlock = &pstModbusMasterInfo->stModbusMasterBlock[stModbusMasterHead.uiBlockID];
                        usWriteVarNum = pstModbusMasterBlock->stModbusMasterBlockHead.uiModbusVarNum;
                        if(usWriteVarNum <= MAX_MODBUS_VAR_NUM)
                        {
                            for(usCount = 0U; usCount < usWriteVarNum; usCount++)
                            {
                                ucTaskID = pstModbusMasterBlock->stModbusVar[usCount].ucTaskID;
                                usSize = pstModbusMasterBlock->stModbusVar[usCount].usVarSize;
                                uiModbusIndex = pstModbusMasterBlock->stModbusVar[usCount].uiModbusIndex - pstModbusMasterBlock->stModbusMasterBlockHead.uiTargetBeginAddr;

                                if(ucTaskID < MAX_TASK_NUM)
                                {
                                    /* 根据uiWriteIndex判断ucWriteVar是否有剩余空间 */
                                    if(s_stWriteVarArray[ucTaskID].uiWriteIndex > MONITOR_WRITE_BUF_LEN - 8)
                                    {
                                        break;
                                    }
                                    /* 将变量信息拷贝到写变量队列，不拷贝uiModbusIndex元素 */
                                    memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                            &pstModbusMasterBlock->stModbusVar[usCount], sizeof(WriteVar_t));
                                    s_stWriteVarArray[ucTaskID].uiWriteIndex += sizeof(WriteVar_t);

                                    if((pstModbusMasterBlock->stModbusMasterBlockHead.ucModbusRgn == MONITOR_HOLD_REGS)
                                            || (pstModbusMasterBlock->stModbusMasterBlockHead.ucModbusRgn == MONITOR_INPUT_REGS))
                                    {/* 寄存器 */

                                        memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                                &ucModbusData[usBuffIndex + sizeof(uint16_t) * uiModbusIndex], usSize);
                                        s_stWriteVarArray[ucTaskID].uiWriteIndex += usSize;
                                    }
                                    else if((pstModbusMasterBlock->stModbusMasterBlockHead.ucModbusRgn == MONITOR_COIL_STATUS)
                                            || (pstModbusMasterBlock->stModbusMasterBlockHead.ucModbusRgn == MONITOR_INPUT_STATUS))
                                    {/* 线圈 */
                                        uiQuotient = uiModbusIndex / UINT8_BITS; /* 商 */
                                        uiResidue =  uiModbusIndex % UINT8_BITS; /* 余数 */
                                        ucValue = (ucModbusData[usBuffIndex + sizeof(uint8_t) * uiQuotient]>>uiResidue) & (uint8_t)1U;

                                        memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                                &ucValue, sizeof(uint8_t));
                                        s_stWriteVarArray[ucTaskID].uiWriteIndex += sizeof(uint8_t);
                                    }
                                    else
                                    {
                                        ;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(stModbusHead.ucPortType == MONITOR_MODBUS_SLAVE)
        {/* modbus公共头结构中，类型为modbus从 */

//            printf("#3;%d;%d\n",usModbusDataLen,stModbusHead.usDataLen);
            if(usModbusDataLen == stModbusHead.usDataLen + sizeof(CommModbusSlaveHead_t))
            {/* 检查读取的数据长度和公共头结构中数据长度是否符合 */
                /* 读取modbus从头结构 */

                memcpy(&stModbusSlaveHead, &ucModbusData[0], sizeof(CommModbusSlaveHead_t));
                usBuffIndex = usBuffIndex + sizeof(CommModbusSlaveHead_t);

                pstModbusSlaveInfo = SharedGetModbusSlaveInfo();
                if((pstModbusSlaveInfo != NULL) && (stModbusSlaveHead.uiBlockID < MAX_MODBUS_AREA_NUM) )
                {/* modbus从站配置信息地址正确 */

                    /* 获取modbus从站的块配置信息地址 */
                    pstModbusSlaveBlock = &pstModbusSlaveInfo->stModbusSlaveBlockInfo[stModbusSlaveHead.uiBlockID];

                    for(usCount = 0U; usCount < stModbusSlaveHead.usElementNum; usCount++)
                    {/* 根据modbus从站头结构中的元素个数，轮询 */
                        /* 元素在块内的序号 */
                        usModbusSlaveAddr = stModbusSlaveHead.usBeginAddr + usCount;
//                        printf("S#6:%d\n",pstModbusSlaveBlock->stModbusSlaveVar[usModbusSlaveAddr].ucActive);
                        if(pstModbusSlaveBlock->stModbusSlaveVar[usModbusSlaveAddr].ucActive == 1)
                        {/* 检查该元素是否有对应具体变量，如果没有，说明工程中未配置，可省去 */
                            ucTaskID = pstModbusSlaveBlock->stModbusSlaveVar[usModbusSlaveAddr].stModbusVar.ucTaskID;
                            usSize = pstModbusSlaveBlock->stModbusSlaveVar[usModbusSlaveAddr].stModbusVar.usVarSize;
                            if(ucTaskID < MAX_TASK_NUM)
                            {
                                /* 根据uiWriteIndex判断ucWriteVar是否有剩余空间 */
                                if(s_stWriteVarArray[ucTaskID].uiWriteIndex > MONITOR_WRITE_BUF_LEN - 8)
                                {
                                    break;
                                }
                                /* 将变量信息拷贝到写变量队列，不拷贝uiModbusIndex元素 */
                                memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                        &pstModbusSlaveBlock->stModbusSlaveVar[usModbusSlaveAddr].stModbusVar, sizeof(WriteVar_t));
                                s_stWriteVarArray[ucTaskID].uiWriteIndex += sizeof(WriteVar_t);

                                if(stModbusSlaveHead.uiBlockID == MONITOR_COIL_STATUS)
                                {/* 线圈 */
                                    uiQuotient = usCount / UINT8_BITS; /* 商 */
                                    uiResidue =  usCount % UINT8_BITS; /* 余数 */
                                    ucValue = (ucModbusData[usBuffIndex + sizeof(uint8_t) * uiQuotient]>>uiResidue) & (uint8_t)1U;
//                                    printf("S#7:%d\n",ucValue);
                                    memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                            &ucValue, sizeof(uint8_t));
                                    s_stWriteVarArray[ucTaskID].uiWriteIndex += sizeof(uint8_t);
                                }
                                else if(stModbusSlaveHead.uiBlockID == MONITOR_HOLD_REGS)
                                {/* 寄存器 */
//                                    printf("S#7:%d;%d\n",usSize,ucModbusData[usBuffIndex + sizeof(uint16_t) * usCount]);
                                    memcpy(&s_stWriteVarArray[ucTaskID].ucWriteVar[s_stWriteVarArray[ucTaskID].uiWriteIndex],
                                            &ucModbusData[usBuffIndex + sizeof(uint16_t) * usCount], usSize);
                                    s_stWriteVarArray[ucTaskID].uiWriteIndex += usSize;
                                }
                                else
                                {
                                    ;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            ;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131Preprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorCS1131Preprocess(void)
{
    bool_t bRet = false;
    uint16_t usCS1131DataLen = 0U;
    uint8_t ucCS1131Data[MAX_SM_MSG_LEN];
    MSGHead_t* pstMSGHead = NULL;
    CMController_t emCMID = CM1;
    uint32_t uiCalCrc = 0U;
    uint32_t uiReadCrc = 0U;
    uint32_t usBuffIndex = 0U;
    uint16_t usACKDataLen = 0U;
    bool_t bSetRet = false;
    uint8_t ucACKData[1024];


    /*从共享内存上获取1131服务数据*/
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        bRet = SysGetSMMsg(SM_CS1131_REQ, emCMID, &ucCS1131Data[0], &usCS1131DataLen);

        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            if((true == bRet) && (usCS1131DataLen != 0U) && (usCS1131DataLen < MAX_SM_MSG_LEN))
            {/*函数返回值正确且CS1131数据长度大于0，预处理数据*/
                /*解析同步过来的CS1131请求数据，包括，同步服务头，1131消息头，消息内容等。*/
                /*暂只需要对写变量和强制服务进行预处理*/
                
                pstMSGHead = (MSGHead_t*)&ucCS1131Data[0];
                
                uiCalCrc = SysCrc32Cal(0x00, (puint8_t)&ucCS1131Data[0], pstMSGHead->usLength);
                usBuffIndex = pstMSGHead->usLength;
                memcpy((puint8_t)&uiReadCrc,(puint8_t)&ucCS1131Data[usBuffIndex],CHKSUM_CRC_SIZE);
                     
                if(uiCalCrc != uiReadCrc)
                {/*CRC 不一致*/
                    /*log*/
                    DEBUG_MONITOR("CalCrc != ReadCrc\n");
                }
                else if(usCS1131DataLen != pstMSGHead->usLength + CHKSUM_CRC_SIZE)
                {/*数据长度不一致*/
                    /*log*/
                    DEBUG_MONITOR("DataLen err\n");
                }
                else
                {
                    switch(pstMSGHead->usActualType)
                    {/*根据服务号，处理1131服务*/
                        /*these service need to synchronize*/
                       case CS1131_START_RUN:
                       {/*处理开始运行服务*/
                           usACKDataLen = MonitorCS1131StartRun(&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_STOP_RUN:
                       {/*处理停止运行服务*/
                           usACKDataLen = MonitorCS1131StopRun(&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_PAUSE_RUN:
                       {/*处理暂停运行服务*/
                           usACKDataLen = MonitorCS1131PauseRun(&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_WRITE_VAR:
                       {/*处理写变量服务*/
                           usACKDataLen = MonitorCS1131WriteVarPreHandle(&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_SINGLE_STEP:
                       {/*处理单步运行服务*/
                           usACKDataLen = MonitorCS1131SingleStep(&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_DISABLE_VAR:
                       {/*处理Disable变量服务*/
                           usACKDataLen = MonitorCS1131DisableVarPreHandle (&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_START_OSC:
                       {/*处理变量示波器开始服务*/
                           usACKDataLen = MonitorCS1131OSCPStart (&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       case CS1131_STOP_OSC:
                       {/*处理变量示波器开始服务*/
                           usACKDataLen = MonitorCS1131OSCPDone (&ucCS1131Data[0], &ucACKData[0]);
                       }
    
                       break;
                       
                       default:
                       {
                           DEBUG_MONITOR("RECV ERROR SERVISE:  %d \n",pstMSGHead->usActualType);
                       }
                       break;
                    }
    
                    if(usACKDataLen > 0)
                    {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
    //                    DEBUG_MONITOR(" Has RESP Data \n");
                        bSetRet = SysSetSMMsg(SM_CS1131_RESP,emCMID,&ucACKData[0],usACKDataLen);
                        if(false == bSetRet)
                        {
                            DEBUG_MONITOR("SET CS1131 ACK ERR\n");
                        }
                        
                        usACKDataLen = 0U;
                    }
                }
            }
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131StartRun
*
* 功能描述: 完成对开始运行服务的处理.
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
static uint16_t MonitorCS1131StartRun(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID = 0U;
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PMID号       */
    
    printf("Run\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的下装标志*/
        uiBuffIndex += sizeof(MSGHead_t);
        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        emTaskID = ucTaskID;
        /*若当前是调试模式，设置任务的状态*/
//        if((KS_PROGRAM == SharedGetSwitchPos()) && ((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID)))
//        {
//            SysSetTaskStatus(emLocalPMId,emTaskID,TS_RUN);
//        }
        
        if((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID))
        {
            SysSetTaskStatus(emLocalPMId,emTaskID,TS_RUN);
        }
        else
        {
            ucSubSeason = ERR_TASK_ID;
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131StopRun
*
* 功能描述: 完成对停止运行服务的处理.
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
static uint16_t MonitorCS1131StopRun(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID = 0U;
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PMID号       */
    
    printf("Stop\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的下装标志*/
        uiBuffIndex += sizeof(MSGHead_t);
        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        emTaskID = ucTaskID;
        /*若当前是调试模式，设置任务的状态*/
//        if((KS_PROGRAM == SharedGetSwitchPos()) && ((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID)))
//        {
//            SysSetTaskStatus(emLocalPMId,emTaskID,TS_STOP);
//        }
        
        if((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID))
        {
            SysSetTaskStatus(emLocalPMId,emTaskID,TS_STOP);
        }
        else
        {
            ucSubSeason = ERR_TASK_ID;
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(uiACKDataIndex);
        
    }
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131PauseRun
*
* 功能描述: 完成对暂停运行服务的处理.
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
static uint16_t MonitorCS1131PauseRun(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID = 0U;
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PMID号       */
    
    printf("Pause\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的下装标志*/
        uiBuffIndex += sizeof(MSGHead_t);
        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        emTaskID = ucTaskID;
        /*若当前是调试模式，设置任务的状态*/
//        if((KS_PROGRAM == SharedGetSwitchPos()) && ((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID)))
//        {
//            SysSetTaskStatus(emLocalPMId,emTaskID,TS_PAUSE);
//        }
        
        if((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID))
        {
            SysSetTaskStatus(emLocalPMId,emTaskID,TS_PAUSE);
        }
        else
        {
            ucSubSeason = ERR_TASK_ID;
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
        
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131SingleStep
*
* 功能描述: 完成对单步运行服务的处理.
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
static uint16_t MonitorCS1131SingleStep(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID = 0U;
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    
    printf("SingleStep\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的下装标志*/
        uiBuffIndex += sizeof(MSGHead_t);
        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        emTaskID = ucTaskID;
        /*若当前是调试模式，设置任务的状态*/
//        if((KS_PROGRAM == SharedGetSwitchPos()) && ((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID)))
//        {
//            SysSetTaskSingleStepFlg(emTaskID,TASK_SINGLE_STEP);
//        }
        
        if((TASK_UP1 == emTaskID) || (TASK_UP2 == emTaskID))
        {
            SysSetTaskSingleStepFlg(emTaskID,TASK_SINGLE_STEP);
        }
        else
        {
            ucSubSeason = ERR_TASK_ID;
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131WriteVarPreHandle
*
* 功能描述: 完成对CS1131写变量服务的预处理.
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
static uint16_t MonitorCS1131WriteVarPreHandle(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint16_t usWriteVarNum = 0U;
    uint16_t usCount = 0U;
    WriteVar_t* pstWriteVar = NULL;
    uint32_t uiTestIndex = 0U;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    
//    DEBUG_MONITOR("1131WRVarPreHandle\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的写变量标志*/
        uiBuffIndex += sizeof(MSGHead_t);

        memcpy((puint8_t)&usWriteVarNum,&ucCS1131Data[uiBuffIndex],sizeof(uint16_t));
        uiBuffIndex += sizeof(uint16_t);
        
//            DEBUG_MONITOR("WriteVarNum : %d \n",usWriteVarNum);
        
        if(usWriteVarNum <= MAX_WRITE_VAR_NUM)
        {
            for(usCount = 0U; usCount < usWriteVarNum; usCount++)
            {
                pstWriteVar = (WriteVar_t*)&ucCS1131Data[uiBuffIndex];
                
//                    DEBUG_MONITOR("TaskID: %d RefID: %d Size: %d OffSet: %d \n",\
//                            pstWriteVar->ucTaskID,pstWriteVar->ucRefID,pstWriteVar->usSize,pstWriteVar->uiOffSet);
                
                if((pstWriteVar->ucTaskID < MAX_TASK_NUM) && (pstWriteVar->usSize <= 8))
                {
                    if(TS_STOP != SysGetTaskStatus(emLocalPMId,pstWriteVar->ucTaskID))
                    {
                        emTaskID = pstWriteVar->ucTaskID;
                        memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                &ucCS1131Data[uiBuffIndex], sizeof(WriteVar_t));
                        uiBuffIndex += sizeof(WriteVar_t);
                        
                        s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(WriteVar_t);
                        if(pstWriteVar->usSize != 0)
                        {
                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                    &ucCS1131Data[uiBuffIndex], pstWriteVar->usSize);
                            
                            uiBuffIndex += pstWriteVar->usSize;
                            s_stWriteVarArray[emTaskID].uiWriteIndex += pstWriteVar->usSize;
                        }
                        else
                        {
                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                    &ucCS1131Data[uiBuffIndex], sizeof(uint8_t));
    
    //                            DEBUG_MONITOR("Value: %x \n", \
    //                                    s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex]);
                            
                            uiBuffIndex += sizeof(uint8_t);
                            s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(uint8_t);
                        }
                        
                        if(s_stWriteVarArray[emTaskID].uiWriteIndex >= 2*1024)
                        {
                            break;
                        }
                    }
                    else
                    {
                        ucSubSeason = ERR_WRTIE_VAR_TASK_STOP;
                    }
                }
            }
        }
        else
        {
            ucSubSeason = ERR_WRITE_VAR;
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131DisableVarPreHandle
*
* 功能描述: 完成对CS1131Disable变量服务的预处理.
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
static uint16_t MonitorCS1131DisableVarPreHandle(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint16_t usForceItemNum = 0U;
    uint16_t usForceTablLen = 0U;
    uint16_t usCount = 0U;
    ForceItem_t* pstForceItem = NULL;
    uint32_t uiTestIndex = 0U;
    DisableTablHead_t* pstDisableTablHead = NULL;
    uint32_t uiCheckSumCal = 0U;
    
//    DEBUG_MONITOR("1131DisVarPreHandle\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucCS1131Data\n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData\n");
    }
    else
    {/*拷贝1131下发的写变量标志*/
        uiBuffIndex += sizeof(MSGHead_t);

        memcpy((puint8_t)&usForceItemNum,&ucCS1131Data[uiBuffIndex],sizeof(uint16_t));
        uiBuffIndex += sizeof(uint16_t);
        
        if(usForceItemNum <= MAX_WRITE_VAR_NUM)
        {
            for(usCount = 0U; usCount < usForceItemNum; usCount++)
            {
                pstForceItem = (ForceItem_t*)&ucCS1131Data[uiBuffIndex];
                
//                printf("Task %d Ref %d Size %d Off %d\n",\
//                        pstForceItem->ucTaskID,pstForceItem->ucRefID,pstForceItem->usSize,pstForceItem->uiOffSet);
                
                if((pstForceItem->ucTaskID < MAX_TASK_NUM) && (pstForceItem->usSize <= 8))
                {
                    emTaskID = pstForceItem->ucTaskID;
                    memcpy(&s_stForceItemArray[emTaskID].ucForceItem[s_stForceItemArray[emTaskID].uiWriteIndex], \
                            &ucCS1131Data[uiBuffIndex], sizeof(ForceItem_t));
                    uiBuffIndex += sizeof(ForceItem_t);
                    
                    s_stForceItemArray[emTaskID].uiWriteIndex += sizeof(ForceItem_t);
                    if(pstForceItem->usSize != 0)
                    {
                        memcpy(&s_stForceItemArray[emTaskID].ucForceItem[s_stForceItemArray[emTaskID].uiWriteIndex], \
                                &ucCS1131Data[uiBuffIndex], sizeof(uint8_t));
                        
                        uiTestIndex = s_stForceItemArray[emTaskID].uiWriteIndex;
//                        printf("V:");
//                        uint8_t x = 0U;
//                        for(x = 0U; x< pstForceItem->usSize; x++)
//                        {
//                            printf("%x ",s_stForceItemArray[emTaskID].ucForceItem[uiTestIndex]);
//                            uiTestIndex++;
//                        }
//                        printf("\n");
                        
                        uiBuffIndex += sizeof(uint8_t);
                        s_stForceItemArray[emTaskID].uiWriteIndex += sizeof(uint8_t);
                    }
                    else
                    {
                        memcpy(&s_stForceItemArray[emTaskID].ucForceItem[s_stForceItemArray[emTaskID].uiWriteIndex], \
                                &ucCS1131Data[uiBuffIndex], sizeof(uint8_t));

//                        printf("V: %x \n", \
//                                s_stForceItemArray[emTaskID].ucForceItem[s_stForceItemArray[emTaskID].uiWriteIndex]);
                        
                        uiBuffIndex += sizeof(uint8_t);
                        s_stForceItemArray[emTaskID].uiWriteIndex += sizeof(uint8_t);
                    }
                    
                    if(s_stForceItemArray[emTaskID].uiWriteIndex >= 2*1024)
                    {
                        break;
                    }
                }
            }
//            printf("FIndex %d\n", s_stForceItemArray[emTaskID].uiWriteIndex);
            
            /*获取强制表中变量个数，暂时为0，无用*/
            memcpy(&usForceTablLen, &ucCS1131Data[uiBuffIndex], sizeof(uint16_t));
            
            printf("DisTablLen %d\n", usForceTablLen);
            
            uiBuffIndex += sizeof(uint16_t);
            pstDisableTablHead = (DisableTablHead_t*)&ucCS1131Data[uiBuffIndex];
            if(usForceTablLen == pstDisableTablHead->uiSize)
            {
                /*计算文件总CRC*/
                uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucCS1131Data[uiBuffIndex + sizeof(uint32_t)]), \
                        pstDisableTablHead->uiSize - sizeof(uint32_t));
                if(uiCheckSumCal != pstDisableTablHead->uiCheckSum)
                {
                    /*log*/
                    printf("DIS TAB CRC ERR\n");
                }
                else
                {
                    if((usForceTablLen % 2) != 0)
                    {
                        usForceTablLen = usForceTablLen + 1;
                    }
                    SharedSetDisableTabl(&ucCS1131Data[uiBuffIndex], usForceTablLen);
//                    uint16_t ustest = 0U;
//                    for(ustest = sizeof(DisableTablHead_t); ustest < usForceTablLen; ustest++)
//                    {
//                        printf("%x ",ucCS1131Data[uiBuffIndex + ustest]);
//                    }
//                    printf("\n");
                }
            }
        }
        else
        {
            ucSubSeason = ERR_FORCE_LIST;
        }

        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
//        DEBUG_MONITOR("SubSeason : %d\n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131OSCPStart
*
* 功能描述: 完成对变量示波器开始服务的处理.
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
static uint16_t MonitorCS1131OSCPStart(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint16_t usCount = 0U;
    uint32_t uiCheckSumCal = 0U;
    OSCPCfgInfo_t* pstOSCPCfg = NULL;
    OSCPCtrlInfo_t* pstOSCPCtrl = NULL;
    uint32_t uiTotalLen = 0;
//    DEBUG_MONITOR("1131StartOSCP\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == CS1131Data\n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData\n");
    }
    else
    {/*拷贝1131下发的写变量标志*/
        uiBuffIndex += sizeof(MSGHead_t);

        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
//        uiBuffIndex += sizeof(uint8_t);
        if(ucTaskID < MAX_TASK_NUM)
        {
            pstOSCPCfg = SharedGetOSCPCfg(ucTaskID);
            pstOSCPCtrl = SharedGetOSCPCtrl(ucTaskID);
            
            if((pstOSCPCfg != NULL) && (pstOSCPCtrl != NULL))
            {
                memcpy((puint8_t)&pstOSCPCfg->stOSCPCfgHead,&ucCS1131Data[uiBuffIndex],sizeof(OSCPCfgHead_t));
                uiBuffIndex += sizeof(OSCPCfgHead_t);
                
                if(pstOSCPCfg->stOSCPCfgHead.usChannelNum <= MAX_OSCP_VAR)
                {
                    for(usCount = 0U; usCount < pstOSCPCfg->stOSCPCfgHead.usChannelNum; usCount++)
                    {
                        memcpy((puint8_t)&pstOSCPCfg->stVar[usCount], &ucCS1131Data[uiBuffIndex], sizeof(OSCPVarInfo_t));
                        
                        uiBuffIndex += sizeof(OSCPVarInfo_t);
                        
                        if(0 == pstOSCPCfg->stVar[usCount].usSize)
                        {
                            uiTotalLen = uiTotalLen + 1;
                        }
                        else
                        {
                            uiTotalLen = uiTotalLen + pstOSCPCfg->stVar[usCount].usSize;
                        }
                    }
                    
                    memset(pstOSCPCtrl, 0x00, sizeof(OSCPCtrlInfo_t));
                    
                    uiTotalLen = uiTotalLen * pstOSCPCfg->stOSCPCfgHead.uiTotalPoints;
                    if(uiTotalLen > OSCP_BUFF_LEN)
                    {
                        ucSubSeason = ERR_OSCP_START;
                        printf("TotalLen err %d\n",uiTotalLen);
                    }
                }
                else
                {
                    ucSubSeason = ERR_OSCP_START;
                }
            }
            else
            {
                ucSubSeason = ERR_OSCP_START;
            }
        }
        else
        {
            ucSubSeason = ERR_OSCP_START;
        }

        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);

        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);

    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131OSCPDone
*
* 功能描述: 完成对变量示波器完成服务的处理.
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
static uint16_t MonitorCS1131OSCPDone(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint32_t uiCheckSumCal = 0U;
    OSCPCfgInfo_t* pstOSCPCfg = NULL;
    OSCPCtrlInfo_t* pstOSCPCtrl = NULL;
    uint8_t ucIsRepeat = 0U;
//    DEBUG_MONITOR("1131StartOSCP\n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == CS1131Data\n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData\n");
    }
    else
    {/*拷贝1131下发的写变量标志*/
        uiBuffIndex += sizeof(MSGHead_t);

        memcpy((puint8_t)&ucTaskID,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        memcpy((puint8_t)&ucIsRepeat,&ucCS1131Data[uiBuffIndex],sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        if(ucTaskID < MAX_TASK_NUM)
        {
            pstOSCPCfg = SharedGetOSCPCfg(ucTaskID);
            pstOSCPCtrl = SharedGetOSCPCtrl(ucTaskID);
            
            if((pstOSCPCfg != NULL) && (pstOSCPCtrl != NULL))
            {
                if(1U == ucIsRepeat)
                {
                    memset(pstOSCPCtrl, 0x00, sizeof(OSCPCtrlInfo_t));
                }
                else
                {
                    memset(pstOSCPCfg, 0x00, sizeof(OSCPCfgInfo_t));
                    memset(pstOSCPCtrl, 0x00, sizeof(OSCPCtrlInfo_t));
                }
            }
            else
            {
                ucSubSeason = ERR_OSCP_DONE;
            }
        }
        else
        {
            ucSubSeason = ERR_OSCP_DONE;
        }


        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);

        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_MONITOR("ACK %d\n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorOSCPHandle
*
* 功能描述: 完成对变量示波器功能的处理.
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
static void MonitorOSCPHandle(Task_ID_t emTaskID)
{
    OSCPCfgInfo_t* pstOSCPCfg = NULL;
    OSCPCtrlInfo_t* pstOSCPCtrl = NULL;
    puint8_t pucOSCPBuff = NULL;
    puint8_t pucVarAddr = NULL;
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint16_t usCount = 0;
    uint8_t ucBool = 0;
    
    pstOSCPCfg = SharedGetOSCPCfg(emTaskID);
    pstOSCPCtrl = SharedGetOSCPCtrl(emTaskID);
    
    if((pstOSCPCfg != NULL) && (pstOSCPCtrl != NULL))
    {
        if((pstOSCPCfg->stOSCPCfgHead.usChannelNum > 0) && (pstOSCPCfg->stOSCPCfgHead.uiTotalPoints > pstOSCPCtrl->uiWRVarNum))
        {
            pstOSCPCtrl->uiPeriodCount++;
            
            if(pstOSCPCtrl->uiPeriodCount >= pstOSCPCfg->stOSCPCfgHead.usTimeIntervals)
            {
                uiBuffIndex = pstOSCPCtrl->uiWRIndex;
                
                
                for(usCount = 0; usCount < pstOSCPCfg->stOSCPCfgHead.usChannelNum; usCount++)
                {
                    if((pstOSCPCfg->stVar[usCount].ucRefID < NUM_OF_RT_DATA_TYPE) && (pstOSCPCfg->stVar[usCount].usSize <= 8))
                    {
                        pucVarAddr = SysGetSharedMemRtDataStartAddr(pstOSCPCfg->stVar[usCount].ucRefID);
                        pucOSCPBuff = SharedGetOSCPBuff(emTaskID, uiBuffIndex);
                        if((pucVarAddr != NULL) && (pucOSCPBuff != NULL))
                        {
                            if(pstOSCPCfg->stVar[usCount].usSize != 0)
                            {
                                memcpy(pucOSCPBuff, pucVarAddr + pstOSCPCfg->stVar[usCount].uiOffset, 
                                       pstOSCPCfg->stVar[usCount].usSize);
                                
                                uiBuffIndex += pstOSCPCfg->stVar[usCount].usSize;
                            }
                            else
                            {
                                memcpy(&ucBool, pucVarAddr + (pstOSCPCfg->stVar[usCount].uiOffset / 8),
                                       sizeof(uint8_t));
                                
                                ucBool = (ucBool >> (pstOSCPCfg->stVar[usCount].uiOffset % 8)) & 0x01;
                                
                                memcpy(pucOSCPBuff, &ucBool, sizeof(uint8_t));
                                
                                uiBuffIndex += sizeof(uint8_t);
                            }
                        }
                    }
                }
                
                pstOSCPCtrl->uiWRVarNum++;
                pstOSCPCtrl->uiWRIndex = uiBuffIndex;
                pstOSCPCtrl->uiPeriodCount = 0;
                
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorWriteVarHandle
*
* 功能描述: 完成对写变量服务的处理.
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
static void MonitorWriteVarHandle (Task_ID_t emTaskID)
{
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    WriteVar_t* pstWriteVar = NULL;
    puint8_t pucRefAreaAddr = NULL;
    uint8_t ucIOBool = 0U;
    uint32_t uiByteOffset = 0U;
    uint32_t uiBitOffset = 0U;
    puint8_t pucBaseAddr = NULL;
//    uint32_t uiTestIndex = 0U;
    
//    DEBUG_MONITOR("begin Monitor WriteVar  \n");
    
    if(s_stWriteVarArray[emTaskID].uiWriteIndex > 0U)
    {
        do
        {
            pstWriteVar = (WriteVar_t*)&s_stWriteVarArray[emTaskID].ucWriteVar[uiBuffIndex];
            uiBuffIndex += sizeof(WriteVar_t);
            
//            DEBUG_MONITOR("W TaskID: %d RefID: %d Size: %d OffSet: %d \n",pstWriteVar->ucTaskID,pstWriteVar->ucRefID,pstWriteVar->usSize,pstWriteVar->uiOffSet);
//            printf("W:%d;%d;%d;%d\n",pstWriteVar->ucTaskID,pstWriteVar->ucRefID,pstWriteVar->usSize,pstWriteVar->uiOffSet);
            
            if( (pstWriteVar->ucTaskID < MAX_TASK_NUM) && (pstWriteVar->ucRefID < NUM_OF_RT_DATA_TYPE))
            {
                if(RT_DATA_TYPE_I == pstWriteVar->ucRefID)
                {
                    pucRefAreaAddr = SysGetSharedMemRtDataStartAddr(pstWriteVar->ucRefID);
                }
                else
                {
                    pucRefAreaAddr = SysGetPRGRtDataStartAddr(pstWriteVar->ucRefID);
                }
                if((pucRefAreaAddr != NULL) && (pstWriteVar->usSize <= 8))
                {
                    if(pstWriteVar->usSize != 0)
                    {
//                        uiTestIndex = uiBuffIndex;
//                        DEBUG_MONITOR("W Value: ");
//                        printf("V:");
//                        uint8_t x = 0U;
//                        for(x = 0U; x< pstWriteVar->usSize; x++)
//                        {
//                            printf("%x ",s_stWriteVarArray[TASK_UP1].ucWriteVar[uiTestIndex]);
//                            uiTestIndex++;
//                        }
//                        printf("\n");
                        
                        memcpy((puint8_t)(pucRefAreaAddr + pstWriteVar->uiOffSet), \
                                (puint8_t)&s_stWriteVarArray[emTaskID].ucWriteVar[uiBuffIndex], pstWriteVar->usSize);
                        uiBuffIndex += pstWriteVar->usSize;
                    }
                    else
                    {
                        memcpy(&ucIOBool, &s_stWriteVarArray[emTaskID].ucWriteVar[uiBuffIndex],\
                                sizeof(ucIOBool));
                        
                        uiByteOffset = pstWriteVar->uiOffSet >> 3;
                        uiBitOffset = pstWriteVar->uiOffSet & 7;
                        
                        pucBaseAddr = (puint8_t)(pucRefAreaAddr + uiByteOffset);
                        
                        if(ucIOBool == 0)
                        {
                            ucIOBool = 0x01;
                            ucIOBool = ucIOBool << uiBitOffset;
                            *pucBaseAddr = ((uint8_t)*pucBaseAddr) & (~ucIOBool);
                        }
                        else
                        {
                            ucIOBool = 0x01;
                            ucIOBool = ucIOBool << uiBitOffset;
                            *pucBaseAddr = ((uint8_t)*pucBaseAddr) | ucIOBool;
                        }
                        
                        uiBuffIndex += sizeof(ucIOBool);
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }while(uiBuffIndex < s_stWriteVarArray[emTaskID].uiWriteIndex);
        
        s_stWriteVarArray[emTaskID].uiWriteIndex = 0U;
    }
    
//    DEBUG_MONITOR("END Monitor WriteVar  \n");
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorForceVarHandle
*
* 功能描述: 完成对强制变量服务的处理.
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
static void MonitorForceVarHandle (Task_ID_t emTaskID)
{
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    ForceItem_t* pstForceItem = NULL;
    puint8_t pucRefAreaAddr = NULL;
    uint8_t ucIOBool = 0U;
    uint32_t uiByteOffset = 0U;
    uint32_t uiBitOffset = 0U;
    puint8_t pucBaseAddr = NULL;
//    uint32_t uiTestIndex = 0U;
    
//    DEBUG_MONITOR("ForceVar\n");
    
    if(s_stForceItemArray[emTaskID].uiWriteIndex > 0U)
    {
        do
        {
            pstForceItem = (ForceItem_t*)&s_stForceItemArray[emTaskID].ucForceItem[uiBuffIndex];
            uiBuffIndex += sizeof(ForceItem_t);
            
//            DEBUG_MONITOR("W TaskID: %d RefID: %d Size: %d OffSet: %d \n",pstForceItem->ucTaskID,pstForceItem->ucRefID,pstForceItem->usSize,pstForceItem->uiOffSet);
//            printf("F:%d;%d;%d;%d\n",pstForceItem->ucTaskID,pstForceItem->ucRefID,pstForceItem->usSize,pstForceItem->uiOffSet);
            
            if( (pstForceItem->ucTaskID < MAX_TASK_NUM) && (pstForceItem->ucRefID < NUM_OF_RT_DATA_TYPE))
            {
                pucRefAreaAddr = SysGetRtDataDisableFlagStartAddr(pstForceItem->ucRefID);
                if((pucRefAreaAddr != NULL) && (pstForceItem->usSize <= 8))
                {
                    if(pstForceItem->usSize != 0)
                    {
                        
                        memcpy((puint8_t)(pucRefAreaAddr + pstForceItem->uiOffSet), \
                                (puint8_t)&s_stForceItemArray[emTaskID].ucForceItem[uiBuffIndex], sizeof(uint8_t));
                        uiBuffIndex += sizeof(uint8_t);
                    }
                    else
                    {
                        memcpy(&ucIOBool, &s_stForceItemArray[emTaskID].ucForceItem[uiBuffIndex],\
                                sizeof(ucIOBool));
                        
                        uiByteOffset = pstForceItem->uiOffSet >> 3;
                        uiBitOffset = pstForceItem->uiOffSet & 7;
                        
                        pucBaseAddr = (puint8_t)(pucRefAreaAddr + uiByteOffset);
                        
                        if(ucIOBool == 0)
                        {
                            ucIOBool = 0x01;
                            ucIOBool = ucIOBool << uiBitOffset;
                            *pucBaseAddr = ((uint8_t)*pucBaseAddr) & (~ucIOBool);
                        }
                        else
                        {
                            ucIOBool = 0x01;
                            ucIOBool = ucIOBool << uiBitOffset;
                            *pucBaseAddr = ((uint8_t)*pucBaseAddr) | ucIOBool;
                        }
                        
                        uiBuffIndex += sizeof(ucIOBool);
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }while(uiBuffIndex < s_stForceItemArray[emTaskID].uiWriteIndex);
        
        s_stForceItemArray[emTaskID].uiWriteIndex = 0U;
    }
    else
    {
        printf("WriteIndex 0\n");
    }
    
//    DEBUG_MONITOR("END ForceVar\n");
    
    return;
}


/*
***********************************************************************************************************************
* 函数名称: MonitorOPCPreprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorOPCPreprocess(void)
{
    bool_t bRet = false;
    uint16_t usOPCDataLen = 0U;
    uint8_t ucOPCData[MAX_SM_MSG_LEN];
    MSGHead_t* pstMSGHead = NULL;
    CMController_t emCMID = CM1;
    uint32_t uiCalCrc = 0U;
    uint32_t uiReadCrc = 0U;
    uint32_t usBuffIndex = 0U;
    uint16_t usACKDataLen = 0U;
    bool_t bSetRet = false;
    uint8_t ucACKData[1024];

    /*从共享内存上获取1131服务数据*/
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        bRet = SysGetSMMsg(SM_OPC_REQ, emCMID, &ucOPCData[0], &usOPCDataLen);
        
//        DEBUG_MONITOR("bRet : %d  OPCDataLen : %d \n",bRet,usOPCDataLen);
        if( HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            if((true == bRet) && (usOPCDataLen != 0U) && (usOPCDataLen < MAX_SM_MSG_LEN))
            {/*函数返回值正确且OPC数据长度大于0，预处理数据*/
                /*解析同步过来的OPC请求数据，包括，同步服务头，OPC消息头，消息内容等。*/
    
                pstMSGHead = (MSGHead_t*)&ucOPCData[0];
                
                uiCalCrc = SysCrc32Cal(0x00, (puint8_t)&ucOPCData[0], \
                        pstMSGHead->usLength);
                usBuffIndex = pstMSGHead->usLength;
                memcpy((puint8_t)&uiReadCrc,(puint8_t)&ucOPCData[usBuffIndex],CHKSUM_CRC_SIZE);
                     
                if(uiCalCrc != uiReadCrc)
                {/*CRC 不一致*/
                    /*log*/
                    DEBUG_MONITOR("CalCrc != ReadCrc\n");
                }
                else if(usOPCDataLen != pstMSGHead->usLength + CHKSUM_CRC_SIZE)
                {/*数据长度不一致*/
                    /*log*/
                    DEBUG_MONITOR("DataLen is error.\n");
                }
                else
                {
                    switch(pstMSGHead->usActualType)
                    {/*根据服务号，处理OPC服务*/
                        /*these service need to synchronize*/
                       case OPC_WRITE:
                       {/*处理写变量服务*/
                           usACKDataLen = MonitorOPCWriteVarPreHandle (&ucOPCData[0], &ucACKData[0]);
                       }
                       break;
                       
                       default:
                       {
                           DEBUG_MONITOR("RECV ERROR SERVISE: %d \n",pstMSGHead->usActualType);
                       }
                       break;
                    }
    
                    if(usACKDataLen > 0)
                    {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
    //                    DEBUG_MONITOR("OPC RESP Data \n");
                        bSetRet = SysSetSMMsg(SM_OPC_RESP,emCMID,&ucACKData[0],usACKDataLen);
                        if(false == bSetRet)
                        {
                            DEBUG_MONITOR("SET OPC ACK ERR \n");
                        }
                        
                        usACKDataLen = 0U;
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorOPCWriteVarPreHandle
*
* 功能描述: 完成对OPC写变量服务的预处理.
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
static uint16_t MonitorOPCWriteVarPreHandle (uint8_t ucOPCData[], uint8_t ucACKData[])
{
    Task_ID_t emTaskID;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    uint16_t usWriteVarNum = 0U;
    uint16_t usCount = 0U;
    WriteVar_t* pstWriteVar = NULL;
    OPCWritVarHead_t* pstOPCWritVarHead = NULL;
    
    uint32_t uiTestIndex = 0U;
    
//    DEBUG_MONITOR("Begin OPCWriteVarPreHandle\n");
    
    if(NULL == ucOPCData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        DEBUG_MONITOR("NULL == ucOPCData  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else
    {
        uiBuffIndex += sizeof(MSGHead_t);

        /*若当前是调试模式，设置任务的状态*/
//        if(KS_PROGRAM == SharedGetSwitchPos())
        if((KS_PROGRAM == SharedGetSwitchPos()) || (KS_RUN == SharedGetSwitchPos()) ||\
                (KS_STOP == SharedGetSwitchPos()) || (KS_INIT == SharedGetSwitchPos()))
        {
            pstOPCWritVarHead = (OPCWritVarHead_t*)&ucOPCData[uiBuffIndex];
            uiBuffIndex += sizeof(OPCWritVarHead_t);
            
            usWriteVarNum = pstOPCWritVarHead->usOPCWritVarNum;
            
//            DEBUG_MONITOR("WriteVarNum : %d \n",usWriteVarNum);
            
            if(usWriteVarNum < MAX_WRITE_VAR_NUM)
            {
                for(usCount = 0U; usCount < usWriteVarNum; usCount++)
                {
                    pstWriteVar = (WriteVar_t*)&ucOPCData[uiBuffIndex];
                    
//                    DEBUG_MONITOR("TaskID: %d RefID: %d Size: %d OffSet: %d \n",\
//                            pstWriteVar->ucTaskID,pstWriteVar->ucRefID,pstWriteVar->usSize,pstWriteVar->uiOffSet);
                    
                    if((pstWriteVar->ucTaskID < MAX_TASK_NUM) && (pstWriteVar->usSize <= 8))
                    {
                        emTaskID = pstWriteVar->ucTaskID;
                        memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                &ucOPCData[uiBuffIndex], sizeof(WriteVar_t));
                        uiBuffIndex += sizeof(WriteVar_t);
                        
                        s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(WriteVar_t);
                        
                        if(pstWriteVar->usSize != 0)
                        {
                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                    &ucOPCData[uiBuffIndex], pstWriteVar->usSize);
                            
                            uiTestIndex = s_stWriteVarArray[emTaskID].uiWriteIndex;
//                            DEBUG_MONITOR("Value: ");
//                            uint8_t x = 0U;
//                            for(x = 0U; x< pstWriteVar->usSize; x++)
//                            {
//                                printf("%x ",s_stWriteVarArray[emTaskID].ucWriteVar[uiTestIndex]);
//                                uiTestIndex++;
//                            }
//                            printf(" \n");
                            
                            uiBuffIndex += pstWriteVar->usSize;
                            s_stWriteVarArray[emTaskID].uiWriteIndex += pstWriteVar->usSize;
                        }
                        else
                        {
                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                    &ucOPCData[uiBuffIndex], sizeof(uint8_t));
                            
                            uiTestIndex = s_stWriteVarArray[emTaskID].uiWriteIndex;
                            DEBUG_MONITOR("Value: %x \n", \
                                    s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex]);
                            
                            uiBuffIndex += sizeof(uint8_t);
                            s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(uint8_t);
                        }
                        
                        if(s_stWriteVarArray[emTaskID].uiWriteIndex >= 2*1024)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                ucSubSeason = ERR_OPC_WRITE_VAR;
            }
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucOPCData;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
    //    DEBUG_MONITOR("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
    //    DEBUG_MONITOR("ucSubSeason is: %d  \n",ucSubSeason);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(uiACKDataIndex);
    }

//    DEBUG_MONITOR("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorRetainVarHanlde
*
* 功能描述: 完成对掉电保持变量的周期备份.
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
static void MonitorRetainVarHanlde (Task_ID_t emTaskID)
{
    RetainVarInfo_t* pstRetainVarInfo = NULL;                   /*Retain变量配置信息结构可写片区指针*/
    RetainVar_t* pstRetainVar = NULL;
    puint8_t pucDataAreaAddr = NULL;
    uint32_t uiRetainVarCount = 0U;
    uint8_t ucRetainBuff[1028];
    uint32_t uiBuffIndex = 0U;
    uint8_t ucIOBool = 0U;
    uint32_t uiByteOffset = 0U;
    uint32_t uiBitOffset = 0U;
    puint8_t pucBaseAddr = NULL;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    uint16_t usVarSizeTmp = 0U;

    uiBuffIndex += sizeof(uiBuffIndex);
    
    /*获取Retain变量配置信息结构可读片区指针*/
    pstRetainVarInfo = SharedGetRetainVarInfo();
    
    if((HAS_BE_CONFIGURED == SysGetIsConfigured()) && (pstRetainVarInfo != NULL) )
    {
        if((emTaskID < MAX_TASK_NUM) && (pstRetainVarInfo->uiUPVarNum[emTaskID] <= MAX_RETAIN_NUM))
        {
            memset(&ucRetainBuff[0], 0x00, 4);

//            printf("UPVarNum: %x \n",pstRetainVarInfo->uiUPVarNum[emTaskID]);

            for(uiRetainVarCount = 0U; uiRetainVarCount < pstRetainVarInfo->uiUPVarNum[emTaskID]; uiRetainVarCount++)
            {
                pstRetainVar = (RetainVar_t*)&pstRetainVarInfo->stRetainVar[emTaskID][uiRetainVarCount];
                if(TS_RUN == SysGetTaskStatus(emLocalPMId,emTaskID))
                {
//                    printf("%d TaskID: %x ",uiRetainVarCount,pstRetainVar->ucTaskID);
//                    printf("RefID: %x ",pstRetainVar->ucRefID);
//                    printf("VarSize: %x ",pstRetainVar->usVarSize);
//                    printf("Offset: %x \n",pstRetainVar->uiOffset);
                }
                
                if(pstRetainVar->ucRefID < NUM_OF_RT_DATA_TYPE)
                {
                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(pstRetainVar->ucRefID);
                }
                if(pucDataAreaAddr != NULL)
                {
                    if(pstRetainVar->usVarSize != 0)
                    {
                        usVarSizeTmp = pstRetainVar->usVarSize;
                    }
                    else
                    {
                        usVarSizeTmp = sizeof(ucIOBool);
                    }
                    if((uiBuffIndex + usVarSizeTmp) <= 1028)
                    {
                        if(pstRetainVar->usVarSize != 0)
                        {
                            memcpy((puint8_t)&ucRetainBuff[uiBuffIndex], (puint8_t)(pucDataAreaAddr + pstRetainVar->uiOffset),\
                                    pstRetainVar->usVarSize);

                            uiBuffIndex += pstRetainVar->usVarSize;
                        }
                        else
                        {
                            uiByteOffset = pstRetainVar->uiOffset >> 3;
                            uiBitOffset = pstRetainVar->uiOffset & 7;
                            
                            pucBaseAddr = (puint8_t)(pucDataAreaAddr + uiByteOffset);

                            ucIOBool = *pucBaseAddr;

                            if(TS_RUN == SysGetTaskStatus(emLocalPMId,emTaskID))
                            {
//                                printf("IOBool：%x \n",ucIOBool);
                            }
                            
                            ucIOBool = (ucIOBool >> uiBitOffset) & 0x01;
                            memcpy((puint8_t)&ucRetainBuff[uiBuffIndex], &ucIOBool,\
                                    sizeof(ucIOBool));
                            
                            uiBuffIndex += sizeof(ucIOBool);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        if(0 != (uiBuffIndex % 2))
        {
            uiBuffIndex = uiBuffIndex + 1;
        }
        
        memcpy(&ucRetainBuff[0],&uiBuffIndex,sizeof(uiBuffIndex));
        
        /*将组织在一起的数据拷贝进SRAM*/
        if(TASK_UP1 == emTaskID)
        {
    //        SharedSetTask0RetainData((puint8_t)&uiBuffIndex,sizeof(uiBuffIndex));
            SharedSetTask0RetainData((puint8_t)&ucRetainBuff[0],uiBuffIndex);
        }
        
        if(TASK_UP2 == emTaskID)
        {
    //        SharedSetTask1RetainData((puint8_t)&uiBuffIndex,sizeof(uiBuffIndex));
            SharedSetTask1RetainData((puint8_t)&ucRetainBuff[0],uiBuffIndex);
        }
        /*将组织在一起的数据拷贝进SRAM*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorRetainVarHanlde
*
* 功能描述: 完成对掉电保持变量的周期备份.
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
static void MonitorRecoveryRetainVar(void)
{
    RetainVarInfo_t* pstRetainVarInfo = NULL;                   /*Retain变量配置信息结构可写片区指针*/
    RetainVar_t* pstRetainVar = NULL;
    puint8_t pucDataAreaAddr = NULL;
    uint32_t uiRetainVarCount = 0U;
    uint8_t ucRetainBuff[1024 + 4];
    uint32_t uiBuffIndex = 0U;
    uint32_t uiDataLen = 0U;
    Task_ID_t emTaskID = TASK_UP1;
    uint8_t ucIOBool = 0U;
    uint32_t uiByteOffset = 0U;
    uint32_t uiBitOffset = 0U;
    puint8_t pucBaseAddr = NULL;
    puint8_t pucMemStartAddr = NULL;
    puint8_t pucPrgStartAddr = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    pucMemStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_G);
    pucPrgStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    
    DEBUG_MONITOR("RecoveryRtnVar\n");
    
    /*获取Retain变量配置信息结构可读片区指针*/
    pstRetainVarInfo = SharedGetRetainVarInfo();
    
    if((HAS_BE_CONFIGURED == SysGetIsConfigured()) && (pstRetainVarInfo != NULL))
    {
        for(emTaskID = TASK_UP1; emTaskID < MAX_TASK_NUM; emTaskID++)
        {
            uiBuffIndex = 0;
            if((pstRetainVarInfo->uiUPVarNum[emTaskID] <= MAX_RETAIN_NUM))
            {
                memset(&ucRetainBuff[0], 0x00, 4);
                
                /*将数据从SRAM拷贝出来*/
                if(TASK_UP1 == emTaskID)
                {
                    SharedGetTask0RetainData((puint8_t)&uiDataLen,sizeof(uiDataLen));
                    if(uiDataLen > 1028)
                    {
                        break;
                    }
                    else
                    {
                        SharedGetTask0RetainData((puint8_t)&ucRetainBuff[0],uiDataLen);
                        uiBuffIndex += sizeof(uiDataLen);
                    }
                }
                
                if(TASK_UP2 == emTaskID)
                {
                    SharedGetTask1RetainData((puint8_t)&uiDataLen,sizeof(uiDataLen));
                    if(uiDataLen > 1028)
                    {
                        break;
                    }
                    else
                    {
                        SharedGetTask1RetainData((puint8_t)&ucRetainBuff[0],uiDataLen);
                        uiBuffIndex += sizeof(uiDataLen);
                    }
                    
                }
                /*将数据从SRAM拷贝出来*/
                
                for(uiRetainVarCount = 0U; uiRetainVarCount < pstRetainVarInfo->uiUPVarNum[emTaskID]; uiRetainVarCount++)
                {
                    pstRetainVar = (RetainVar_t*)&pstRetainVarInfo->stRetainVar[emTaskID][uiRetainVarCount];
                    
                    if(pstRetainVar->ucRefID < NUM_OF_RT_DATA_TYPE)
                    {
                        pucDataAreaAddr = SysGetPRGRtDataStartAddr(pstRetainVar->ucRefID);
                    }
                    if(pucDataAreaAddr != NULL)
                    {/*将变量拷贝放入数据区*/
                        if((uiBuffIndex + pstRetainVar->usVarSize) <= 1028)
                        {
                            if(pstRetainVar->usVarSize != 0)
                            {
                                memcpy((puint8_t)(pucDataAreaAddr + pstRetainVar->uiOffset), &ucRetainBuff[uiBuffIndex],\
                                        pstRetainVar->usVarSize);
                                
                                uiBuffIndex += pstRetainVar->usVarSize;
                            }
                            else
                            {
                                memcpy(&ucIOBool, &ucRetainBuff[uiBuffIndex],\
                                        sizeof(ucIOBool));
                                
                                uiByteOffset = pstRetainVar->uiOffset >> 3;
                                uiBitOffset = pstRetainVar->uiOffset & 7;
                                
                                pucBaseAddr = (puint8_t)(pucDataAreaAddr + uiByteOffset);

                                if(ucIOBool == 0)
                                {
                                    ucIOBool = 0x01;
                                    ucIOBool = ucIOBool << uiBitOffset;
                                    *pucBaseAddr = ((uint8_t)*pucBaseAddr) & (~ucIOBool);
                                }
                                else
                                {
                                    ucIOBool = 0x01;
                                    ucIOBool = ucIOBool << uiBitOffset;
                                    *pucBaseAddr = ((uint8_t)*pucBaseAddr) | ucIOBool;
                                }
                                
                                uiBuffIndex += sizeof(ucIOBool);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            
            DataProcessOutputDataCopy(emTaskID); /* 输出数据从运算数据区Q区拷贝到临时数据区Q区 */
            
            if(pstCtrlStationConfigInfo != NULL)
            {
                memcpy(pucMemStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiOOffset,
                        pucPrgStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiOOffset,
                        pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiOUsedSize);
            }
        }
    }

    return;
}



/*
***********************************************************************************************************************
* 函数名称: MonitorCtrlSrvProcess
*
* 功能描述: 完成对服务队列的读取，服务的处理及应答.
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
static void MonitorCtrlSrvProcess(Task_ID_t emTaskID)
{
    bool_t bRet = false;
    uint16_t usCtrlDataLen = 0U;
    uint8_t ucCtrlSrvData[MAX_SM_MSG_LEN];
    MSGHead_t* pstMSGHead = NULL;
    ACKHead_t* pstACKHead = NULL;                               /*应答头部*/
    uint32_t usBuffIndex = 0U;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint16_t usACKDataLen = 0U;
    bool_t bSetRet = false;
    uint8_t ucACKData[1024];
    Task_ID_t emReadTaskID = TASK_UP1;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    uint32_t uiCalcCRC = 0U;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;
    puint8_t pucSrcAddr = NULL;
    puint8_t pucDstAddr = NULL;
    uint32_t uiAreaUsedSize = 0U;
    uint32_t uiAreaOffset = 0U;
    GlobaInit GlobalInitPtr = NULL;  
    GlobaInit ChangeGlobalInitPtr = NULL;
    puint8_t pucDataAreaAddr = NULL;
    puint8_t pucDisableAreaAddr = NULL;
    Task_ID_t emTaskIndex = TASK_UP1;
    SysIoModInfo_t* pstIoModInfo = NULL;
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PM ID号 */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);      /* 前一系PM ID号 */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);    /* 后一系PM ID号 */
    uint16_t usPMState = 0U;
    uint8_t i = 0;

    /*从共享内存上获取Ctrl服务数据*/
    bRet = SysGetSMMsg(SM_CTRLSRV_REQ, (CMController_t)emTaskID, &ucCtrlSrvData[0], &usCtrlDataLen);
    
    if((true == bRet) && (usCtrlDataLen != 0U) && (usCtrlDataLen < MAX_SM_MSG_LEN))
    {
        pstMSGHead = (MSGHead_t*)&ucCtrlSrvData[0];
        usBuffIndex = sizeof(MSGHead_t);
        
        memcpy(&emReadTaskID, &ucCtrlSrvData[usBuffIndex], sizeof(Task_ID_t));
        
        if(emReadTaskID < MAX_TASK_NUM)
        {
            switch(pstMSGHead->usActualType)
            {/*根据服务号，处理Ctrl服务*/
               case SYS_UP_PAUSE:
               {/*处理暂停服务*/
                   if(emTaskID == emReadTaskID)
                   {
                       SharedSetPauseFlag(PF_PAUSE, emTaskID);
                   }
                   else
                   {
                       ucSubSeason = ERR_CTRL_PAUSE;
                       printf("Task: %d, %d PF_PAUSE ERR\n",emTaskID,emReadTaskID);
                   }
               }
               break;
               
               case SYS_UP_CONTINUE:
               {/*处理恢复服务*/
                   if(emTaskID == emReadTaskID)
                   {
//                       pstTaskConfigInfo = SharedGetTaskConfigInfo(emTaskID);
//
//                       if(pstTaskConfigInfo != NULL)
//                       {
//                           pucSrcAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_G);
//                           pucDstAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
//                           
//                           if((pucSrcAddr != NULL) && (pucDstAddr != NULL))
//                           {
//                               uiAreaOffset = pstTaskConfigInfo->uiGOffset;
//                               uiAreaUsedSize = pstTaskConfigInfo->uiGUsedSize;
//                               
//                               if((uiAreaOffset + uiAreaUsedSize) <= MAX_RTDATA_G_AREA_LEN)
//                               {
//                                   memcpy((puint8_t)(pucDstAddr + uiAreaOffset), (puint8_t)(pucSrcAddr + uiAreaOffset), \
//                                           uiAreaUsedSize);
//                               }
//                               else
//                               {
//                                   printf("ID %d Off %d, S %d\n",RT_DATA_TYPE_G, uiAreaOffset,uiAreaUsedSize);
//                               }
//                           }
//                           
//                           pucSrcAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);
//                           pucDstAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_Q);
//                           
//                           if((pucSrcAddr != NULL) && (pucDstAddr != NULL))
//                           {
//                               if(pstTaskConfigInfo->uiOOffset >= (MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN))
//                               {
//                                   uiAreaOffset = pstTaskConfigInfo->uiOOffset - MAX_RTDATA_G_AREA_LEN - MAX_RTDATA_I_AREA_LEN;
//                               }
//                               uiAreaUsedSize = pstTaskConfigInfo->uiOUsedSize;
//                               
//                               if((uiAreaOffset + uiAreaUsedSize) <= MAX_RTDATA_Q_AREA_LEN)
//                               {
//                                   memcpy((puint8_t)(pucDstAddr + uiAreaOffset), (puint8_t)(pucSrcAddr + uiAreaOffset), \
//                                           uiAreaUsedSize);
//                                   printe("Q %x\n",*(pucDstAddr + uiAreaOffset));
//                               }
//                           }
//                       }
                       DataProcessSoftSOEInit(emTaskID, true);
                       SharedSetPauseFlag(PF_CONTINUE, emTaskID);
                   }
                   else
                   {
                       ucSubSeason = ERR_CTRL_CONTINUE;
                       DEBUG_MONITOR("Task: %d, %d PF_CONTINUE ERR\n",emTaskID,emReadTaskID);
                   }
               }
               break;
               
               case SYS_PRJ_SWITCH:
               {
                   if(INIT_DOWNLOAD == SharedGetDownloadFlg())
                   {
                       SysSetSMWatchdogInfo(0);
                       SharedSwitchAllConfigInfo();
                       pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
                       if((pstIoModInfo != NULL))
                       {
                           SysSelectAdjustTimePulseSrc(pstIoModInfo->stSysPMInfo.usPPMSource);
                           SysSelectAdjustTimePulsePolarity(pstIoModInfo->stSysPMInfo.usPPMPolarity);
                       }

                       SharedSetDownloadFlg(NOT_DOWNLOAD);
                       
                       /*初始化下装，进行全数据区初始化，初始化外部库*/

                       /* 下装时清DeltaFlag标志 */
                       SharedSetDelataFlag(TASK_UP1, false);
                       SharedSetDelataFlag(TASK_UP2, false);
                       SharedSetDelataTime(TASK_UP1, 0);
                       SharedSetDelataTime(TASK_UP2, 0);
                       SharedSetAvgScanSurplus(TASK_UP1, 0);
                       SharedSetAvgScanSurplus(TASK_UP2, 0);

                       /* 临时清ExlibTRScanStatus相关标志 */
                       for(i = 0; i < 100; i++)
                       {
                           uiUP1ScanSurplus[i] = 0;
                           uiUP2ScanSurplus[i] = 0;
                           ucUP1ScanOverrun[i] = 0;
                           ucUP2ScanOverrun[i] = 0;
                       }
                       ucUP1ScanOverrunIndex = 0;
                       ucUP2ScanOverrunIndex = 0;
                       ucUP1ScanSurplusIndex = 0;
                       ucUP2ScanSurplusIndex = 0;
                       ucUP1ScanSurplusFullFlag = 0;
                       ucUP2ScanSurplusFullFlag = 0;

                       for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
                       {
                           if(SharedGetTaskActive(emTaskIndex) == ACTIVE)
                           {
                               SysSetTaskStatus(SysGetLocalPMID(),emTaskIndex, TS_STOP);
                           }
                           
                           SharedSetSyncDataFlag(emTaskIndex,DATA_SYNC_COMPLETE);
                           SysSetIOPollFlg(emTaskIndex,IO_POLL_END);
                       }

                       SharedSetDownloadPM(INVALID_PM_ID);

                       /*设置PM状态中，关于下装完成标志*/
                       usPMState = SharedGetPMState();
                       usPMState = usPMState & 0xfffc;
                       usPMState = usPMState | 0x0001;
                       SharedSetPMState(usPMState);
                       
                       DataProcessSlaveConfirmInit();
                       SysClearDownloadAllStartFlag();
                       if(HAS_BE_CONFIGURED == SysGetIsConfigured())
                       {
                           SharedBackupOldCMSlotIdTabl();
                       }
                   }
                   
                   if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
                   {
                       /*在线下装记录老的配置信息使用片区，开始分步骤的切换工程*/
                       for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
                       {
                           SharedRecordTmpAvailable(emTaskIndex);
                       }
                       
                       SharedSwitchAllConfigInfo();
                       
                       pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
                       if((pstIoModInfo != NULL))
                       {
                           SysSelectAdjustTimePulseSrc(pstIoModInfo->stSysPMInfo.usPPMSource);
                           SysSelectAdjustTimePulsePolarity(pstIoModInfo->stSysPMInfo.usPPMPolarity);
                       }

                       SharedSetDownloadFlg(NOT_DOWNLOAD);
                       
                       for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
                       {
                           SharedSetSyncDataFlag(emTaskIndex,DATA_SYNC_COMPLETE);
                       }
                       
                       
                       SharedSetDownloadPM(INVALID_PM_ID);
                       SharedSwitchUsedParaBufID();

                       
                       /*按照任务进行软SOE初始化*/
                       for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
                       {
                           DataProcessSoftSOEInit(emTaskIndex, true);
                       }
                       
                       /*在线下装设置配置信息切换标志，开始分步骤的切换工程*/
                       for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
                       {
                           if(ACTIVE == SharedGetTaskActive(emTaskIndex))
                           {
                               SharedSetOnlineDownLoadSwitchFlg(emTaskIndex, OS_SWITCH_START);
                           }
                       }
                       SysClearDownloadAllStartFlag();
                       if(HAS_BE_CONFIGURED == SysGetIsConfigured())
                       {
                           SharedBackupOldCMSlotIdTabl();
                       }
                   }
               }
               break;
               
               default:
               {
                   ucSubSeason = ERR_CTRL_CONTINUE;
               }
               break;
            }
            
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            pstMSGHead = (MSGHead_t*)ucCtrlSrvData;
            pstACKHead->ucType = pstMSGHead->ucProtect;
            pstACKHead->ucProtect = pstMSGHead->ucType;
            pstACKHead->usActualType = pstMSGHead->usActualType;
            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
            pstACKHead->ucDataFlg = 0U;
            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
            uiACKDataIndex += sizeof(ACKHead_t);
            
            if(ucSubSeason != ERR_SUCCESS)
            {/*服务处理失败*/
                pstACKHead->ucResCode = RESCODE_FAILED;
            }
            else
            {/*服务处理成功*/
                pstACKHead->ucResCode = RESCODE_SUCCESS;
            }
            pstACKHead->ucSubReason = ucSubSeason;

            pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
            
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
            uiACKDataIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(uiACKDataIndex);
        }
        else
        {
            DEBUG_MONITOR("Task ERR: %d\n",emReadTaskID);
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            bSetRet = SysSetSMMsg(SM_CTRLSRV_RESP, emTaskID, &ucACKData[0], usACKDataLen);
            if(false == bSetRet)
            {
                DEBUG_MONITOR("SET CTRL ACK ERR \n");
            }
            usACKDataLen = 0U;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorP2PPreprocess
*
* 功能描述: 完成对P2P服务队列的读取，服务的预处理，为服务的最终处理做准备.
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
static void MonitorP2PPreprocess( void )
{
    bool_t bRet = false;
    uint16_t usP2PDataLen = 0U;
    uint8_t ucP2PData[MAX_SM_MSG_LEN];
    P2PRecvReqHead_t* pstP2PRecvHead = NULL;
    CMController_t emCMID = CM1;
    uint32_t uiCalCrc = 0U;
    uint32_t uiReadCrc = 0U;
    uint32_t usBuffIndex = 0U;
    uint16_t usACKDataLen = 0U;
    bool_t bSetRet = false;
    uint8_t ucACKData[1024];

    /*从共享内存上获取P2P服务数据*/
    for( emCMID = CM1; emCMID < NUM_OF_CM; emCMID++ )
    {
        bRet = SysGetSMMsg( SM_P2P_REQ, emCMID, &ucP2PData[0], &usP2PDataLen );
        if(( bRet ) && ( usP2PDataLen != 0U ) && ( usP2PDataLen <= (uint16_t)(MAX_SM_MSG_LEN)))
        {/* 函数返回值正确且P2P数据长度大于0，预处理数据 */
            if( HAS_BE_CONFIGURED == SysGetIsConfigured())
            {
                /* 解析同步过来的P2P请求数据，包括，同步服务头，P2P接收消息头，解包消息内容等 */
                //DEBUG_MONITOR("SRV DATA  %d \n",usP2PDataLen);

                pstP2PRecvHead = (P2PRecvReqHead_t*)&ucP2PData[0];
                usBuffIndex = pstP2PRecvHead->usTotalLen - CHKSUM_CRC_SIZE;
                uiCalCrc = SysCrc32Cal(0x00, (puint8_t)&ucP2PData[0], usBuffIndex);
                memcpy((puint8_t)&uiReadCrc,(puint8_t)&ucP2PData[usBuffIndex],CHKSUM_CRC_SIZE);

                if(uiCalCrc != uiReadCrc)
                {
                    //printf("PECRC\n");
                }
                else if(usP2PDataLen != pstP2PRecvHead->usTotalLen)
                {
                    //printf("PEL\n");
                }
                else
                {
                    switch(pstP2PRecvHead->ucFrameType)
                    {/*根据服务号，处理P2P服务*/
                       case P2P_REQ_FRAME:
                       {/*处理写变量服务*/
                           usACKDataLen = MonitorP2PREQPreHandle(&ucP2PData[0], usP2PDataLen, &ucACKData[0], emCMID);
                       }
                       break;

                       default:
                       {
                           //printf( "PE%d\n", pstP2PRecvHead->ucFrameType );
                       }
                       break;
                    }

                    if( usACKDataLen > 0 )
                    {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
                        //printf( "PRsp\n" );

                        bSetRet = SysSetSMMsg(SM_P2P_RESP,emCMID,&ucACKData[0],usACKDataLen);
                        if( !bSetRet )
                        {
                            //printf( "SSPRF\n" );
                        }
                        else
                        {
                            /*printf( "SysSetP2PResp OK\n" );*/
                        }

                        usACKDataLen = 0U;
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorP2PREQPreHandle
*
* 功能描述: 完成对P2P请求帧进行预处理.
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
static uint16_t MonitorP2PREQPreHandle(uint8_t ucP2PData[], uint32_t usP2PDataLen,uint8_t ucACKData[],
                                       CMController_t emCMID)
{
    Task_ID_t emTaskID = TASK_UP1;
    uint16_t usACKDataLen = 0U;
    uint32_t uiBuffIndex = 0U;
    WriteVar_t stWriteVar;
    P2PRecvReqHead_t *pstP2PRecvHead = NULL;
    ReqDecodeResultInfo_t stReqDRInfo;
    P2PBlock_t* pstP2PBlock = NULL;
    bool_t bRedundant = false;
    uint16_t usDataLen = 0U;
    uint32_t uiP2PVarConut = 0U;
    uint32_t uiMaxVarCnt = 0U;
    uint32_t uiVarDataLen = 0U;
    P2PRecvReqCtrl_t *pstRecvCtrl = NULL;
    uint8_t const *pucDataBuff = NULL;
    
    if(NULL == ucP2PData)
    {/*服务数据指针错误*/
        //DEBUG_MONITOR("NULL == ucP2PData  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        //DEBUG_MONITOR("NULL == ucACKData  \n");
    }
    else if((emCMID < CM1) || (emCMID >= NUM_OF_CM))
    {
        //
    }
    else
    {
        /*若当前是调试模式，设置任务的状态*/
//        if(KS_PROGRAM == SharedGetSwitchPos())
        if((KS_PROGRAM == SharedGetSwitchPos()) || (KS_RUN == SharedGetSwitchPos()) ||\
           (KS_STOP == SharedGetSwitchPos()) || (KS_INIT == SharedGetSwitchPos()))
        {
            memset(&stReqDRInfo, 0x00, sizeof(ReqDecodeResultInfo_t));
            pstP2PRecvHead = (P2PRecvReqHead_t*)&ucP2PData[0];
            uiBuffIndex = sizeof(P2PRecvReqHead_t);
            
            if(ACTIVE == SharedGetP2PBlockRecvActive(pstP2PRecvHead->ucSrcStaID))
            {
                pstP2PBlock = SharedGetP2PRecvBlockInfo(pstP2PRecvHead->ucSrcStaID);

                /* CommProperty 0：发送数据；1：接收数据 */
                if((pstP2PBlock != NULL) && (1 == pstP2PBlock->stP2PBlockHead.usCommProperty))
                {
                    /* P2P Receive Req Ctrl */
                    pstRecvCtrl = &s_stP2PRecvReqCtrl[pstP2PRecvHead->ucSrcStaID];

                    if((pstP2PBlock->stP2PBlockHead.uiTargetStationIP1 != 0U) && \
                       (pstP2PBlock->stP2PBlockHead.uiTargetStationIP2 != 0U))
                    {
                        bRedundant = true;
                    }
                    
                    if( !P2PDecodeReq(pstP2PRecvHead->ucSrcStaID, bRedundant, &ucP2PData[0], usP2PDataLen, \
                                      &stReqDRInfo))
                    {
                        //printf( "PDF!\n" );
                    }
                    else
                    {
                        /* Repeat: Discard-Not need to response and write variable */
                        if( stReqDRInfo.bRepeat )
                        {
                            //printf( "PRpt\n" );
                        }
                        else
                        {
                            /* P2PRecvReq Ctrl */
                            pstRecvCtrl->bHasRecv = true;
                            pstRecvCtrl->uiIntrCycCnt = 0U;

                            /* Handle Valid Data */
                            if((stReqDRInfo.emSelPMID != INVALID_PM_ID) &&
                               (stReqDRInfo.usDataLen > 0U))
                            {
                                /* Copy Valid Safety Data */
                                usDataLen = stReqDRInfo.usDataLen;
                                pucDataBuff = &stReqDRInfo.ucBuff[0];

                                /* Make write variable config info. and its value */
                                uiBuffIndex = 0U;
                                uiMaxVarCnt = pstP2PBlock->stP2PBlockHead.uiP2PVarNum;
                                if( uiMaxVarCnt > (uint32_t)(MAX_P2P_VAR_NUM))
                                {
                                    uiMaxVarCnt = (uint32_t)(MAX_P2P_VAR_NUM);
                                }

                                for(uiP2PVarConut = 0U; uiP2PVarConut < uiMaxVarCnt; uiP2PVarConut++)
                                {
                                    uiVarDataLen = (uint32_t)pstP2PBlock->stP2PVar[uiP2PVarConut].usVarSize;
                                    if( 0U == uiVarDataLen )
                                    {
                                        uiVarDataLen = sizeof(uint8_t);
                                    }
                                    
                                    if( uiBuffIndex + uiVarDataLen > (uint32_t)usDataLen )
                                    {
                                        break;
                                    }

                                    stWriteVar.ucTaskID = pstP2PBlock->stP2PVar[uiP2PVarConut].ucTaskID;
                                    stWriteVar.ucRefID = pstP2PBlock->stP2PVar[uiP2PVarConut].ucRefID;
                                    stWriteVar.usSize = pstP2PBlock->stP2PVar[uiP2PVarConut].usVarSize;
                                    stWriteVar.uiOffSet = pstP2PBlock->stP2PVar[uiP2PVarConut].uiOffset;

                                    if((stWriteVar.ucTaskID < MAX_TASK_NUM) && (stWriteVar.usSize <= 8))
                                    {/*组织写变量包*/
                                        emTaskID = stWriteVar.ucTaskID;
                                        memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                                &stWriteVar, sizeof(WriteVar_t));

                                        s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(WriteVar_t);

                                        if(stWriteVar.usSize != 0)
                                        {
                                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                                    &pucDataBuff[uiBuffIndex], stWriteVar.usSize);

                                            uiBuffIndex += stWriteVar.usSize;
                                            s_stWriteVarArray[emTaskID].uiWriteIndex += stWriteVar.usSize;
                                        }
                                        else
                                        {
                                            memcpy(&s_stWriteVarArray[emTaskID].ucWriteVar[s_stWriteVarArray[emTaskID].uiWriteIndex], \
                                                    &pucDataBuff[uiBuffIndex], sizeof(uint8_t));

                                            uiBuffIndex += sizeof(uint8_t);
                                            s_stWriteVarArray[emTaskID].uiWriteIndex += sizeof(uint8_t);
                                        }

                                        if(s_stWriteVarArray[emTaskID].uiWriteIndex >= 2*1024)
                                        {
                                            break;
                                        }
                                    }
                                }/*end of for*/
                            }
                        }

                        /* Make PM P2P Response Message */
                        if( !MakePMP2PRespMsg( pstP2PRecvHead, &ucACKData[0], &usACKDataLen, emCMID ))
                        {
                            usACKDataLen = 0U;
                        }
                    }
                }
            }
        }
    }

    return usACKDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: MakePMP2PRespMsg
*
* 功能描述: 构造应答帧: 根据配置信息，添加附加信息和FCS
*
* 输入参数: pstP2PReqHead：请求帧头信息；
*          ucACKData：应答帧首地址；
*          emCMID：请求帧来自的CMID。
*
* 输出参数: pusLen：返回应答帧长度
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t MakePMP2PRespMsg( P2PRecvReqHead_t const *pstP2PReqHead, uint8_t ucACKData[],
                                puint16_t pusLen, CMController_t emCMID )
{
    bool_t bResult = false;
    bool_t bRedundant = false;  /* Link Redundant Flag */
    uint8_t ucCMSlotID = 0U;    /* Local Source CM Slot ID */
    uint8_t ucSrcStaID = 0U;    /* Request Source: Send StationID */
    uint16_t usSDataLen = 0U;   /* Safety Data Length */
    uint32_t uiBuffIndex = 0U;
    uint32_t uiCalcCRC = 0U;
    uint32_t uiCRCIndex = 0U;
    P2PBlock_t* pstP2PRecvBlk = NULL;
    P2PSendRspHead_t* pstPMP2PRspHead = NULL;
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;

    if((pstP2PReqHead != NULL) &&
       (ucACKData != NULL) &&
       (pusLen != NULL) &&
       ((emCMID >= CM1) && (emCMID < NUM_OF_CM)))
    {
        ucSrcStaID = pstP2PReqHead->ucSrcStaID;
        pstP2PRecvBlk = SharedGetP2PRecvBlockInfo( ucSrcStaID );
        if( pstP2PRecvBlk != NULL )
        {
            /* Ckeck Communication Redundant */
            if((pstP2PRecvBlk->stP2PBlockHead.ucCMID1 != 0U) &&
               (pstP2PRecvBlk->stP2PBlockHead.ucCMID2 != 0U))
            {
                bRedundant = true;
            }

            /* Safety Message */

            uiBuffIndex = sizeof(P2PSendRspHead_t);
            usSDataLen = 1024 - sizeof(P2PSendRspHead_t) - sizeof(uint32_t);
            if( !P2PMakeResp( ucSrcStaID, &ucACKData[uiBuffIndex], &usSDataLen ))
            {
                //printf( "PRspF\n" );
                usSDataLen = 0;
            }
            else
            {
                /* Additional Info. */
                pstPMP2PRspHead = (P2PSendRspHead_t*)&ucACKData[0];
                pstPMP2PRspHead->ucSrcStaID = (uint8_t)SharedGetStationID();
                pstPMP2PRspHead->ucDstStaID = ucSrcStaID;
                pstPMP2PRspHead->ucFrameType = P2P_RES_FRAME;
                pstPMP2PRspHead->uiTotalLen = sizeof(P2PSendRspHead_t) + usSDataLen + sizeof(uint32_t);
                uiCRCIndex = sizeof(P2PSendRspHead_t) + usSDataLen;

                /* CM Slot ID */
                /* CM Table */
                pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                if(pstCMSlotIdTabl != NULL)
                {
                    ucCMSlotID = (uint8_t)( pstCMSlotIdTabl->uiCMSlotId[emCMID]);
                }
                if( bRedundant && (pstP2PRecvBlk->stP2PBlockHead.ucCMID1 == pstP2PRecvBlk->stP2PBlockHead.ucCMID2))
                {
                    if( pstP2PRecvBlk->stP2PBlockHead.ucCMID1 == ucCMSlotID )
                    {
                        if( 1U == pstP2PReqHead->ucLinkID )
                        {
                            /* ExtReq From Link1 */
                            pstPMP2PRspHead->ucSrcCMID = pstP2PRecvBlk->stP2PBlockHead.ucCMID1;
                            pstPMP2PRspHead->ucSrcNETID = pstP2PRecvBlk->stP2PBlockHead.ucNetID1 - 1;
                            pstPMP2PRspHead->uiDstIP1Addr = pstP2PRecvBlk->stP2PBlockHead.uiTargetStationIP1;
                            pstPMP2PRspHead->uiDstIP2Addr = 0;

                            /* FCS */
                            uiCalcCRC = SysCrc32Cal( 0, &ucACKData[0], uiCRCIndex );
                            memcpy( &ucACKData[uiCRCIndex], &uiCalcCRC, sizeof(uint32_t));

                            /* Result */
                            *pusLen = (uint16_t)pstPMP2PRspHead->uiTotalLen;
                            bResult = true;
                        }
                        else if( 2U == pstP2PReqHead->ucLinkID )
                        {
                            /* ExtReq From Link2 */
                            pstPMP2PRspHead->ucSrcCMID = pstP2PRecvBlk->stP2PBlockHead.ucCMID2;
                            pstPMP2PRspHead->ucSrcNETID = pstP2PRecvBlk->stP2PBlockHead.ucNetID2 - 1;
                            pstPMP2PRspHead->uiDstIP1Addr = pstP2PRecvBlk->stP2PBlockHead.uiTargetStationIP2;
                            pstPMP2PRspHead->uiDstIP2Addr = 0;

                            /* FCS */
                            uiCalcCRC = SysCrc32Cal( 0, &ucACKData[0], uiCRCIndex );
                            memcpy( &ucACKData[uiCRCIndex], &uiCalcCRC, sizeof(uint32_t));

                            /* Result */
                            *pusLen = (uint16_t)pstPMP2PRspHead->uiTotalLen;
                            bResult = true;
                        }
                        else
                        {
                            //printf( "PEL\n" );
                        }
                    }
                    else
                    {
                        //printf( "PCE1\n" );
                    }
                }
                else
                {
                    if( pstP2PRecvBlk->stP2PBlockHead.ucCMID1 == ucCMSlotID )
                    {
                        /* ExtReq From CM1 */
                        pstPMP2PRspHead->ucSrcCMID = pstP2PRecvBlk->stP2PBlockHead.ucCMID1;
                        pstPMP2PRspHead->ucSrcNETID = pstP2PRecvBlk->stP2PBlockHead.ucNetID1 - 1;
                        pstPMP2PRspHead->uiDstIP1Addr = pstP2PRecvBlk->stP2PBlockHead.uiTargetStationIP1;
                        pstPMP2PRspHead->uiDstIP2Addr = 0;

                        /* FCS */
                        uiCalcCRC = SysCrc32Cal( 0, &ucACKData[0], uiCRCIndex );
                        memcpy( &ucACKData[uiCRCIndex], &uiCalcCRC, sizeof(uint32_t));

                        /* Result */
                        *pusLen = (uint16_t)pstPMP2PRspHead->uiTotalLen;
                        bResult = true;
                    }
                    else if( pstP2PRecvBlk->stP2PBlockHead.ucCMID2 == ucCMSlotID )
                    {
                        /* ExtReq From CM2 */
                        pstPMP2PRspHead->ucSrcCMID = pstP2PRecvBlk->stP2PBlockHead.ucCMID2;
                        pstPMP2PRspHead->ucSrcNETID = pstP2PRecvBlk->stP2PBlockHead.ucNetID2 - 1;
                        pstPMP2PRspHead->uiDstIP1Addr = pstP2PRecvBlk->stP2PBlockHead.uiTargetStationIP2;
                        pstPMP2PRspHead->uiDstIP2Addr = 0;

                        /* FCS */
                        uiCalcCRC = SysCrc32Cal( 0, &ucACKData[0], uiCRCIndex );
                        memcpy( &ucACKData[uiCRCIndex], &uiCalcCRC, sizeof(uint32_t));

                        /* Result */
                        *pusLen = (uint16_t)pstPMP2PRspHead->uiTotalLen;
                        bResult = true;
                    }
                    else
                    {
                        //printf( "PCE2\n", ucSrcStaID );
                    }
                }
            }
        }
    }

    return bResult;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorOnlineDownLoadSwitchState
*
* 功能描述: 监控在线下装时任务的切换标志
*
* 输入参数: emTaskID ：任务的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void MonitorOnlineDownLoadSwitchState(Task_ID_t emTaskID)
{
    OnlineDownLoad_Switch_Flg_t emOnlineDownLoadSwitchFlg;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        emOnlineDownLoadSwitchFlg = SharedGetOnlineDownLoadSwitchFlg(emTaskID);
        
        switch(emOnlineDownLoadSwitchFlg)
        {
            case OS_SWITCH_START:
            {
                /*进入从站使能阶段*/
                SharedSetOnlineDownLoadSwitchFlg(emTaskID, OS_SLAVE_ENABLE);
                printf("OS_SLAVE_ENABLE\n");
                
            }
            break;
            
            case OS_SLAVE_ENABLE:
            {
                /*进入IO配置信息切换阶段，此时清除IO配置临时记录标志*/
                SharedSetOnlineDownLoadSwitchFlg(emTaskID, OS_CONFIG_SWITCH);
                SharedClearIOTmpAvailable(emTaskID);
                SharedClearVoteTmpAvailable(emTaskID);
                SharedOnlineDownLoadFillDualDataTable(emTaskID);
                printf("OS_CONFIG_SWITCH\n");
            }
            break;
            
            case OS_CONFIG_SWITCH:
            {
                /*进入Code区切换阶段，此时清除CS配置和Code区临时记录标志*/
                SharedSetOnlineDownLoadSwitchFlg(emTaskID, OS_CODE_SWITCH);
                printf("OS_CODE_SWITCH end\n");
                SharedClearCSTmpAvailable(emTaskID);
                SharedClearCodeTmpAvailable(emTaskID);
                SharedClearSOETmpAvailable(emTaskID);

                SharedSetOnlineDownLoadSwitchFlg(emTaskID, OS_NOT_NEED_SWITCH);
                
                uint16_t usPMState = 0U;
                usPMState = SharedGetPMState();
                usPMState = usPMState & 0xfffc;
                usPMState = usPMState | 0x0001;
                SharedSetPMState(usPMState);
            }
            break;
            
            case OS_NOT_NEED_SWITCH:
            {

            }
            break;
            
            default:
                printf("OS_ERR %d\n",emOnlineDownLoadSwitchFlg);
            break;
        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorDisableTablProcess
*
* 功能描述: 完成disable表相关处理.
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
static void MonitorDisableTablProcess(void)
{   
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    ForceItem_t* pstForceItem = NULL;
    puint8_t pucRefAreaAddr = NULL;
    uint8_t ucIOBool = 0U;
    uint32_t uiByteOffset = 0U;
    uint32_t uiBitOffset = 0U;
    puint8_t pucBaseAddr = NULL;
    uint32_t uiTestIndex = 0U;
    uint8_t ucDisableTab[4*1024];
    DisableTablHead_t stDisableTablHead;
    uint16_t usTmp = 0U;
    
    
    if(NEED_DISABLE_RECOVER == SharedGetDisableRecoverFlg())
    {/*当需要回复disable表时，将disable回复到disable区*/
        
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
            SharedGetDisableTabl(&stDisableTablHead, (uint16_t)sizeof(DisableTablHead_t));
            if((stDisableTablHead.uiSize > sizeof(DisableTablHead_t)) && (stDisableTablHead.uiSize <= RETAIN_SRAM_DISABLE_SIZE) &&
               (stDisableTablHead.uiSize == (sizeof(DisableTablHead_t) + (stDisableTablHead.uiItemNum * sizeof(ForceItem_t)))))
            {
                /*当PM有工程且有非空有效disable表时，点亮disable灯*/
                SetLEDState(LED_NAME_FORCE, LED_ON);
                
                printf("Re Dis\n");
                
    //            memset(&ucDisableTab[0], 0x00, 4*1024);
                SharedGetDisableTabl(&ucDisableTab[0], stDisableTablHead.uiSize);
                uiBuffIndex = sizeof(DisableTablHead_t);
                do
                {
                    pstForceItem = (ForceItem_t*)&ucDisableTab[uiBuffIndex];
                    
                    pucRefAreaAddr = SysGetRtDataDisableFlagStartAddr(pstForceItem->ucRefID);
                            
                    if((pucRefAreaAddr != NULL) && (pstForceItem->usSize <= 8))
                    {
                        if(pstForceItem->usSize != 0)
                        {
                            memset((puint8_t)(pucRefAreaAddr + pstForceItem->uiOffSet), 0x01, sizeof(uint8_t));
                        }
                        else
                        {
                            uiByteOffset = pstForceItem->uiOffSet >> 3;
                            uiBitOffset = pstForceItem->uiOffSet & 7;
                            
                            pucBaseAddr = (puint8_t)(pucRefAreaAddr + uiByteOffset);
                            
                            ucIOBool = 0x01;
                            ucIOBool = ucIOBool << uiBitOffset;
                            *pucBaseAddr = ((uint8_t)*pucBaseAddr) | ucIOBool;
                        }
                        
                        if(RT_DATA_TYPE_I == pstForceItem->ucRefID )
                        {
                            pucRefAreaAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
                            if(pstForceItem->usSize != 0)
                            {
                                SharedGetInputModuleVar(pucRefAreaAddr + pstForceItem->uiOffSet, 
                                                        pstForceItem->uiOffSet, pstForceItem->usSize);
                            }
                            else
                            {
                                uiByteOffset = pstForceItem->uiOffSet >> 3;
                                uiBitOffset = pstForceItem->uiOffSet & 7;
                                
                                if((uiByteOffset % 2) != 0)
                                {
                                    uiByteOffset = uiByteOffset - 1;
                                }
                                SharedGetInputModuleVar((puint8_t)&usTmp, uiByteOffset, sizeof(uint16_t));
                                
                                
                                uiByteOffset = pstForceItem->uiOffSet >> 3;
                                pucBaseAddr = (puint8_t)(pucRefAreaAddr + uiByteOffset);
                                if((uiByteOffset % 2) != 0)
                                {
                                    ucIOBool = usTmp & 0xff;
                                }
                                else
                                {
                                    ucIOBool = (usTmp >> 8) & 0xff;
                                }
        
                                ucIOBool = ucIOBool & (0x01 << uiBitOffset);
                                
                                if(ucIOBool != 0)
                                {
                                    *pucBaseAddr = ((uint8_t)*pucBaseAddr) | ucIOBool;
                                }
                                else
                                {
                                    *pucBaseAddr = ((uint8_t)*pucBaseAddr) & (~ucIOBool);
                                }
                            }
                        }
                    }
                    
                    uiBuffIndex += sizeof(ForceItem_t);
                    
                }while(uiBuffIndex < stDisableTablHead.uiSize);
                
                SharedSetDisableRecoverFlg(NOT_NEED_DISABLE_RECOVER);
            }
            else
            {
                memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
                SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
                SetLEDState(LED_NAME_FORCE, LED_OFF);
                
                SharedSetDisableRecoverFlg(NOT_NEED_DISABLE_RECOVER);
            }
        }
    }
    else
    {
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
            SharedGetDisableTabl(&stDisableTablHead, (uint16_t)sizeof(DisableTablHead_t));
            if((stDisableTablHead.uiSize > sizeof(DisableTablHead_t)) && (stDisableTablHead.uiSize <= RETAIN_SRAM_DISABLE_SIZE) &&
               (stDisableTablHead.uiSize == (sizeof(DisableTablHead_t) + (stDisableTablHead.uiItemNum * sizeof(ForceItem_t)))))
            {
                /*当PM有工程且有非空有效disable表时，点亮disable灯*/
                SetLEDState(LED_NAME_FORCE, LED_ON);

            }
            else
            {
                memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
                SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
                SetLEDState(LED_NAME_FORCE, LED_OFF);
            }
        }
        else
        {
            memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
            SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
            SetLEDState(LED_NAME_FORCE, LED_OFF);
            
            printf("INIT DIS\n");
        }
    }

    return;
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
