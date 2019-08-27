/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 通讯模块
*
*  文件名称    : Comm.c
*
*  功能简介    : 完成与CM模块的通讯和上位机数据的处理功能
*
*  作者        : 刘阳
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
#include <Srv/Scheduler/cagos.h>
#include"../inc/Sync.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
/* 文件同步记录 */
static SyncFileStatus_t s_stSyncFileStatus;
/* 数据同步记录 */
static SyncDataStatus_t s_stSyncDataStatus;
static uint8_t s_ucBuff[1024];
/* 同步定时器记录 */
static SyncTimerStatus_t s_stSyncTimer;
/* 同步数据             */
static SyncData_t s_stSyncData;

static File_Handle_Flag_t emFileHandleFlg;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
void SyncInitFileStatus(void);

void SyncInitDataStatus(void);

void SyncInitParamStatus(void);

void SyncInitTimerStatus(void);

void SyncInitStatus(void);

void SyncRecvSyncData(SyncData_t* pstSyncData);

void SyncSendSyncData(SyncData_t* pstSyncData,bool_t bForceSendFlg);

void SyncHandleSrvData(SyncData_t* pstSyncData, puint32_t puiSrvAckHandleFlag);

uint32_t SyncHandleStateData(const SysPMStateInfo_t* pstOneStatData, PMController_t emSrcPMId,int16_t sDataLen);

uint32_t SyncHandleCS1131SrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleOPCSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleP2PReqSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleP2PRspSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleModbusSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleCtrlSrvData(const SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleDiagSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

void SyncBroadCastCS1131Srv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastOPCSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastModbusSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastP2PReqSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastP2PRspSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastCancel(uint32_t uiDstRecord, srv_srvdata_type_t emSrvType);

void SyncBroadCastCtrlSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, ack_flag_t emAckFlag);

void SyncBroadCastDiagSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncRecvRealData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncRecvData(SyncRealData_t* pstSyncPrjData, PMController_t emSrcPMId);

void SyncRecvRealDataOk(SyncRealData_t* pstSyncPrjData,PMController_t emSrcPMId);

void SyncSendRealData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendData(PMController_t emDstPMId);

void SyncSendDataOk(PMController_t emDstPMId);

uint32_t SyncGetNextArea(SysRtDataType_t* pemCurAreaId);

void SyncWaitAck(PMController_t emSrcPMId, sync_task_stat_t emLastStatus);

void SyncRecvFileData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendFileData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendFile(SysSyncPrjQueue_t* pstSyncPrjQueue);

uint32_t SyncGetNextFile(SyncFileStatus_t* pstSyncFileStatus);

uint32_t SyncGetAjustTimerFlag(void);

static uint8_t SyncWriteFileEnd(SysCfgFileType_t emCfgFileType);

static uint8_t SyncWriteFileContinue(SysCfgFileType_t emCfgFileType, uint8_t ucFileData[],uint32_t uiFileDataLen);

static uint8_t SyncWriteFileStart(SysCfgFileType_t emCfgFileType, uint8_t ucFileData[],uint32_t uiFileDataLen);


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInit
*
* 功能描述: 初始化同步记录信息
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
void SyncInit(void)
{
    /* 初始化同步记录信息 */
    SyncInitStatus();
    
    CommSysSyncInit();
    
    SysSyncInit();
    
    memset((puint8_t)&s_stSyncFileStatus, 0U, sizeof(SyncFileStatus_t));
    
    s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
    
    memset((puint8_t)&s_stSyncDataStatus, 0U, sizeof(SyncDataStatus_t));
    
    memset((puint8_t)&s_stSyncTimer, 0U, sizeof(SyncTimerStatus_t));
    
    memset((puint8_t)&s_stSyncData, 0U, sizeof(SyncData_t));
    
    memset((puint8_t)&emFileHandleFlg, 0U, sizeof(File_Handle_Flag_t));
    
    memset((puint8_t)&s_ucBuff[0], 0U, 1024);

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncStart
*
* 功能描述: 完成上电同步状态
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
void SyncStart(void)
{
    uint16_t usState = 0U; 
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    uint32_t uiRecordCon = 0U;                                                          /* 记录等待应答标识         */
    SysPMStateInfo_t* pstLocalPMState = NULL;
    SysPMStateInfo_t* pstNextPMState = NULL;
    SysPMStateInfo_t* pstPrePMState = NULL;
    DisableTablHead_t stDisableTablHead;

    
    if((HAS_BE_CONFIRMED == SysGetIsConfigured()) && 
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP1)) && 
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP2)))
    {
        SysSetWorkStatus(emLocalPMId,WS_WORK);
    }
    else
    {
        SysSetWorkStatus(emLocalPMId,WS_STANDBY);
    }

    /* 获取本机工程信息 */
    pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
    
    if(pstLocalPMState != NULL)
    {
        pstLocalPMState->emSyncStatus = SS_NOSYNC;
        pstLocalPMState->emWorkStatus = SysGetWorkStatus(emLocalPMId);
        pstLocalPMState->emDownloadFlg = NOT_DOWNLOAD;
        pstLocalPMState->emSwitchKeys = SharedGetSwitchPos();
        pstLocalPMState->emTaskStatus[TASK_UP1] = SysGetTaskStatus(emLocalPMId,TASK_UP1);
        pstLocalPMState->emTaskStatus[TASK_UP2] = SysGetTaskStatus(emLocalPMId,TASK_UP2);
        pstLocalPMState->emPoweronFlg = PM_POWERON;
    }
    

    do
    {
        /* 获取硬件提供状态值 */
        usState = SysGetModularOnLine();
        if(!(usState & BITMASK(0)))
        {/*若后一系不在线*/
            
            /* 更新下一个PM状态 */
            SysSetWorkStatus(emNextPMId, WS_OFFLINE);
            uiRecordCon |= BITMASK(emNextPMId);
            printf("NextPM OFFLINE\n");
        }
        else
        {
            /* 置需接收后一个控制器应答标识 */
            uiRecordCon &= ~(BITMASK(emNextPMId));
        }
        
        if(!(usState & BITMASK(1)))
        {/*若前一系不在线*/
            
            /* 更新下一个PM状态 */
            SysSetWorkStatus(emPrePMId, WS_OFFLINE);
            uiRecordCon |= BITMASK(emPrePMId);
            printf("PrePM OFFLINE\n");
        }
        else
        {
            uiRecordCon &= ~(BITMASK(emPrePMId));
        }
        
        uiRecordCon |= BITMASK(emLocalPMId);
        
        SyncSendSyncData(&s_stSyncData,true);

        
        
        /* 接收来自其它PM的同步数据 */
        SyncRecvSyncData(&s_stSyncData);
        
        if(s_stSyncData.sLinkASrvLen > SYNC_RECV_SIZE_0)
        {
//            printf("Next SyncStat %x DownloadFlg %x\n", 
//                    s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emSyncStatus,
//                    s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emDownloadFlg);
            if((SS_NOSYNC == s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emSyncStatus) &&
               (NOT_DOWNLOAD == s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emDownloadFlg))
            {
                uiRecordCon |= BITMASK(emNextPMId);
                printf("RECV NextPM\n");
                SyncHandleStateData(&s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState, emNextPMId, \
                        s_stSyncData.sLinkASrvLen);
            }
            else
            {
                printf("NextPMState Disallowed %d %d\n",s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emSyncStatus,
                        s_stSyncData.stSyncRecvMSGA.stSyncSrvData.stPMState.emDownloadFlg);
            }
        }
        
        if(s_stSyncData.sLinkBSrvLen > SYNC_RECV_SIZE_0)
        {
//            printf("Pre SyncStat %x DownloadFlg %x\n", 
//                    s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emSyncStatus,
//                    s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emDownloadFlg);
            if((SS_NOSYNC == s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emSyncStatus) &&
               (NOT_DOWNLOAD == s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emDownloadFlg))
            {
                uiRecordCon |= BITMASK(emPrePMId);
                printf("RECV PrePM\n");
                SyncHandleStateData(&s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState, emPrePMId, \
                        s_stSyncData.sLinkBSrvLen);
            }
            else
            {
                printf("PrePMState Disallowed %d %d\n",s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emSyncStatus,
                        s_stSyncData.stSyncRecvMSGB.stSyncSrvData.stPMState.emDownloadFlg);
            }
        }
        
        udelay(1000000u);
    }while(SysGetBin1Num(uiRecordCon) < NUM_OF_PM);
    
    pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
    pstNextPMState = SysGetPMStateInfo(emNextPMId);
    pstPrePMState = SysGetPMStateInfo(emPrePMId);
    
    if((pstLocalPMState != NULL) && (pstNextPMState != NULL) && (pstPrePMState != NULL))
    {
        if(HAS_BE_CONFIRMED == pstLocalPMState->stPrjInfo.ucIsConfigured)
        {
            if(SysGetWorkStatus(emNextPMId) != WS_OFFLINE)
            {/*后一系PM已在线*/
                if((HAS_BE_CONFIRMED != pstNextPMState->stPrjInfo.ucIsConfigured) || 
    
                   ((HAS_BE_CONFIRMED == pstNextPMState->stPrjInfo.ucIsConfigured) && 
                    ((pstLocalPMState->stPrjInfo.uiPrjID != pstNextPMState->stPrjInfo.uiPrjID) ||
                     (pstLocalPMState->stPrjInfo.uiMainVern != pstNextPMState->stPrjInfo.uiMainVern) ||
                     (pstLocalPMState->stPrjInfo.uiMinorVern != pstNextPMState->stPrjInfo.uiMinorVern) ||
                     (pstLocalPMState->stPrjInfo.uiPrjCRC != pstNextPMState->stPrjInfo.uiPrjCRC))))
                {/* 已在线PM无工程或者与已在线PM的工程不一致，删除本地工程及配置信息*/
                    
                    printf("N SYNC INIT\n");
                    
                    SysCfgFileInit();
                    
                    memset(&pstLocalPMState->stPrjInfo, 0x00, sizeof(PrjInfo_t));
                    memset(&pstLocalPMState->stDLPrjInfo, 0x00, sizeof(PrjInfo_t));
                    
                    SysSetIsConfigured(NOT_HAS_BE_CONFIGURED);
                    
                    SharedSetPowerOnConfigSwitchFlg(NOT_NEED_POWERON_CONIFG_SWITCH);
    
                    SysSetTaskStatus(emLocalPMId,TASK_UP1, TS_INIT);
                    SysSetTaskStatus(emLocalPMId,TASK_UP2, TS_INIT);
                    
                    SharedConfigAreaInit();
                    
                    SetLEDState(LED_NAME_SYNC, LED_OFF);
    
                }
                
                SharedSetSyncDataFlag(TASK_UP1,DATA_NOT_SYNC);
                SharedSetSyncDataFlag(TASK_UP2,DATA_NOT_SYNC);
                memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
                SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
            }
            else if(SysGetWorkStatus(emPrePMId) != WS_OFFLINE)
            {/*前一系PM已在线*/
                if((HAS_BE_CONFIRMED != pstPrePMState->stPrjInfo.ucIsConfigured) || 
    
                   ((HAS_BE_CONFIRMED == pstPrePMState->stPrjInfo.ucIsConfigured) && 
                    ((pstLocalPMState->stPrjInfo.uiPrjID != pstPrePMState->stPrjInfo.uiPrjID) ||
                     (pstLocalPMState->stPrjInfo.uiMainVern != pstPrePMState->stPrjInfo.uiMainVern) ||
                     (pstLocalPMState->stPrjInfo.uiMinorVern != pstPrePMState->stPrjInfo.uiMinorVern) ||
                     (pstLocalPMState->stPrjInfo.uiPrjCRC != pstPrePMState->stPrjInfo.uiPrjCRC))))
                {/* 已在线PM无工程或者与已在线PM的工程不一致，删除本地工程及配置信息*/
                    
                    printf("P SYNC INIT\n");
                    
                    SysCfgFileInit();
                    
                    memset(&pstLocalPMState->stPrjInfo, 0x00, sizeof(PrjInfo_t));
                    memset(&pstLocalPMState->stDLPrjInfo, 0x00, sizeof(PrjInfo_t));
                    
                    SysSetIsConfigured(NOT_HAS_BE_CONFIGURED);
                    
                    SharedSetPowerOnConfigSwitchFlg(NOT_NEED_POWERON_CONIFG_SWITCH);
    
                    SysSetTaskStatus(emLocalPMId,TASK_UP1, TS_INIT);
                    SysSetTaskStatus(emLocalPMId,TASK_UP2, TS_INIT);
    
                    SharedConfigAreaInit();
                    
                    SetLEDState(LED_NAME_SYNC, LED_OFF);
                }
                
                SharedSetSyncDataFlag(TASK_UP1,DATA_NOT_SYNC);
                SharedSetSyncDataFlag(TASK_UP2,DATA_NOT_SYNC);
                memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
                SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
            }
            else
            {/*前后两系PM均不在线*/
                printf("N P OFFLINE\n");
                
                SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);
                SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
                
                SetLEDState(LED_NAME_SYNC, LED_ON);
                
                SharedSetDisableRecoverFlg(NEED_DISABLE_RECOVER);
            }
        }
        
        if((HAS_BE_CONFIRMED == pstLocalPMState->stPrjInfo.ucIsConfigured) && 
           (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP1)) && 
           (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP2)))
        {
            SysSetWorkStatus(emLocalPMId,WS_WORK);
        }
        else
        {
            SysSetWorkStatus(emLocalPMId,WS_STANDBY);
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncMngt
*
* 功能描述: 对同步任务进行周期管理，完成三冗余功能
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
void SyncMngt(void)
{
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
    uint32_t uiSrvAckHandleFlag = SYNC_SRVACK_NOHANDLE;             /* 接收到应答数据标识   */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();      /* 同步任务队列         */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();      /* 同步工程队列         */
    static uint32_t s_uiWaitCount = 0U;


    /* 接收来自其它PM的同步数据 */
    SyncRecvSyncData(&s_stSyncData);
    
    /* 处理接收到正确状态数据 */
    if((s_stSyncData.sLinkASrvLen > SYNC_RECV_SIZE_0) || (s_stSyncData.sLinkBSrvLen > SYNC_RECV_SIZE_0))
    {
        SyncHandleSrvData(&s_stSyncData, &uiSrvAckHandleFlag);
    }
    
    /* 若正与本机同步的PM离线，初始化同步状态记录 */
    if(SYNC_DATA_INIT == SysGetSyncDataInit())
    {
        SyncInitStatus();
        SysSetSyncDataInit(SYNC_DATA_NOINIT);
    }
    
    /* 处理同步任务队列中同步任务 */
    if(SYS_QUEUE_NOEMPTY == SysSyncSrvQueueEmpty(pstSyncSrvQueue))
    {/* 若同步任务队列不为空 */
        /* 获取当前任务队列中队首元素 */
        SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);

        /* 当前同步任务是否需要等待一拍后再发送，若需要则本循环不进行该同步任务处理，该
         * 处理方式防止连续发送两拍数据，导致出现丢包 */
        if(TASK_NOWAIT == stSyncTask.emTaskWait)
        {/* 同步任务不需要等待 */
            switch(stSyncTask.emTaskId)
            {
                case SYS_BROADCAST_CS1131SRV:
                    /* 广播1131服务 */
                    SyncBroadCastCS1131Srv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                case SYS_BROADCAST_OPCSRV:
                    /* 广播OPC服务 */
                    SyncBroadCastOPCSrv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                case SYS_BROADCAST_MODBUSSRV:
                    /* 广播Modbus服务 */
                    SyncBroadCastModbusSrv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                case SYS_BROADCAST_P2PREQSRV:
                    /* 广播P2P服务 */
                    SyncBroadCastP2PReqSrv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                case SYS_BROADCAST_P2PRSPSRV:
                    /* 广播P2P服务 */
                    SyncBroadCastP2PRspSrv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                case SYS_BROADCAST_CTRLSRV:
                    
                    SyncBroadCastCtrlSrv(pstSyncSrvQueue, &s_stSyncData, \
                            uiSrvAckHandleFlag, NEED_ACK);
                    break;
                case SYS_BROADCAST_DIAGSRV:
                    /* 广播Diag服务 */
                    SyncBroadCastDiagSrv(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                    break;
                default:
                    /* 错误任务ID号，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    break;
            }
        }
        else
        {/* 同步任务需要等待 */
            s_uiWaitCount++;
            if(s_uiWaitCount >= SYNC_TASK_DELAYCYCLES)
            {
                SysSyncSrvQueueWait(TASK_NOWAIT);
                s_uiWaitCount = 0U;
            }
        }
    }
    else
    {/* 若任务同步任务队列为空 */
//        SyncInitStatus();
//        SyncInitTimerStatus();
    }

    /* 处理同步任务队列中同步任务 */
    if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
    {/* 若同步任务队列不为空 */

        /* 获取当前任务队列中队首元素 */
        SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

        /* 当前同步任务是否需要等待一拍后再发送，若需要则本循环不进行该同步任务处理，该
         * 处理方式防止连续发送两拍数据，导致出现丢包 */
        if(TASK_NOWAIT == stSyncTask.emTaskWait)
        {/* 同步任务不需要等待 */
            switch(stSyncTask.emTaskId)
            {
                case SYS_RECV_REALDATA:
                    /* 接收实时数据 */
//                    if(ucCount >= 5)
//                    {
//                        ucCount = 0;
//                        DEBUG_SYNC("RECV_REAL %d\n",stSyncTask.emUPTaskId);
//                    }
//                    ucCount++;
                    SyncRecvRealData(pstSyncPrjQueue);
                    break;
                case SYS_SEND_REALDATA:
                    /* 发送实时数据 */
//                        if(AJUST_HAS== SysGetSyncAjust())
//                        {
//                            SyncSendRealData(pstSyncPrjQueue);
//                        }
                    SyncSendRealData(pstSyncPrjQueue);
                    break;
                case SYS_RECV_FILE:
                    /* 接收文件数据 */
//                    if(ucCount >= 20)
//                    {
//                        ucCount = 0;
//                        DEBUG_SYNC("RECV_FILE\n");
//                    }
//                    ucCount++;
                    SyncRecvFileData(pstSyncPrjQueue);
                    break;
                case SYS_SEND_FILE:
                    /* 发送文件数据 */
//                        if(AJUST_HAS== SysGetSyncAjust())
//                        {
//                            SyncSendFileData(pstSyncPrjQueue);
//                        }
                    SyncSendFileData(pstSyncPrjQueue);
                    break;
                default:
                    /* 错误任务ID号，出队 */
                    SysDeSyncPrjQueue(pstSyncPrjQueue);
                    
                    DEBUG_SYNC("SYNC Id ERR %d\n",stSyncTask.emTaskId);
                    
                    break;
            }
        }
        else
        {/* 同步工程需要等待 */
            s_uiWaitCount++;
            DEBUG_SYNC("TASK_WAIT \n");
            if(s_uiWaitCount >= SYNC_TASK_DELAYCYCLES)
            {
                SysSyncPrjQueueWait(TASK_NOWAIT);
                s_uiWaitCount = 0U;
            }
        }
        
    }
    else
    {/* 若工程同步任务队列为空 */
        /* 文件同步记录初始化 */
        SyncInitFileStatus();
        /* 数据同步记录初始化 */
        SyncInitDataStatus();
        /* IO参数同步记录初始化 */
        SyncInitParamStatus();
        
    }
    /* 发送给其它PM的同步数据 */
    SyncSendSyncData(&s_stSyncData, false);
    
    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInitFileStatus
*
* 功能描述: 文件同步记录状态初始化。
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
void SyncInitFileStatus(void)
{
    /* 初始化文件同步记录状态 */
    s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
    s_stSyncFileStatus.uiFileSize = 0U;
    s_stSyncFileStatus.uiSendSize = 0U;
    s_stSyncFileStatus.emStartFlag = NEW_FILE;

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInitDataStatus
*
* 功能描述: 数据同步记录状态初始化。
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
void SyncInitDataStatus(void)
{
    /* 初始化数据同步记录 */
    s_stSyncDataStatus.emAreaId = RT_DATA_TYPE_G;
    s_stSyncDataStatus.uiCurOffset = 0U;
    s_stSyncDataStatus.uiCurDataLen = 0U;

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInitDataStatus
*
* 功能描述: IO参数同步记录状态初始化。
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
void SyncInitParamStatus(void)
{
    /* 初始化IO参数同步记录 */

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInitTimerStatus
*
* 功能描述: 同步服务定时器初始化，该定时器用于实现同步模块重发机制。
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
void SyncInitTimerStatus(void)
{
    /* 初始化同步服务定时器 */
    s_stSyncTimer.unTaskStartTime.ullTime = 0UL;

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInitStatus
*
* 功能描述: 同步记录状态初始化。
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
void SyncInitStatus(void)
{
    /* 文件同步记录初始化 */
    SyncInitFileStatus();
    /* 数据同步记录初始化 */
    SyncInitDataStatus();
    /* IO参数同步记录初始化 */
    SyncInitParamStatus();
    /* 服务时间初始化 */
    SyncInitTimerStatus();

    return;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvSyncData
*
* 功能描述: 接收到来自其它PM的同步数据
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
void SyncRecvSyncData(SyncData_t* pstSyncData)
{
    int16_t sRetLen = SYNC_RECV_SIZE_0;                                  /* 接收数据长度         */
    uint8_t ucSyncRecvData[MAX_SYNC_SIZE];
    uint32_t uiBuffIndex = 0U;
    uint32_t uiSyncSrvDataLen = 0U;
    uint32_t uiSyncPrjDataLen = 0U;
    SysPMStateInfo_t* pstPMStateInfo = NULL;
    static uint32_t uiRecvNextPackNum = 0U;
    static uint32_t uiRecvPrePackNum = 0U;

    if(pstSyncData != NULL)
    {/* 校验接收缓存指针不为空 */
        
        memset(pstSyncData, 0x00, sizeof(SyncData_t));
        memset(&ucSyncRecvData[0], 0x00, MAX_SYNC_SIZE);
        uiBuffIndex = 0U;
        
        /* 接收链路A即下一个 PM 发送的服务数据 */
        sRetLen = PMBUS_Receive(PM_BUS_A, PM_BUFF_SYNC, (puint8_t)&ucSyncRecvData[0]);

        if((sRetLen >= SYNC_RECV_SIZE_0) && (sRetLen < MAX_SYNC_SIZE))
        {/* 接收长度大于等于零，接收到数据 */
            
            if(sRetLen < sizeof(SysPMStateInfo_t))
            {/*接收数据长度小于状态数据大小，接收长度错误*/
                pstSyncData->sLinkASrvLen = 0;
                pstSyncData->sLinkASrvErr = RET_FAIL;
                pstSyncData->sLinkAPrjLen = 0;
                pstSyncData->sLinkAPrjErr = RET_FAIL;
                DEBUG_SYNC("RECV N: Len < PMState !\n");
            }
            else
            {
                pstPMStateInfo = (SysPMStateInfo_t*)&ucSyncRecvData[0];
                
                if(uiRecvNextPackNum != (pstPMStateInfo->ucPackNum - 1)) 
                {
                    DEBUG_SYNC("N: Last %d Num %d Len %d\n",\
                            uiRecvNextPackNum,pstPMStateInfo->ucPackNum,sRetLen);
                }
                
                uiRecvNextPackNum = pstPMStateInfo->ucPackNum;
                
                if(ACTIVE == pstPMStateInfo->emActiveFlag)
                {
                    /*将状态数据放入结构中*/
                    memcpy((puint8_t)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.stPMState,\
                            (puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(SysPMStateInfo_t));
                    
                }
                else
                {
                    DEBUG_SYNC("RECV N: ACTIVE != PMState\n");
                }

                uiBuffIndex += sizeof(SysPMStateInfo_t);
                
                /*获取服务数据长度*/
                memcpy((puint8_t)&uiSyncSrvDataLen,(puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                if((uiSyncSrvDataLen >= 0U) && (uiSyncSrvDataLen < CFG_MAX_SYNC_SRV_SIZE))
                {
                    if(uiSyncSrvDataLen > 0U)
                    {
                        DEBUG_SYNC("RECV N SRV: %d\n",uiSyncSrvDataLen);   
                    }
                    
                    pstSyncData->stSyncRecvMSGA.stSyncSrvData.uiSyncSrvDataLen = uiSyncSrvDataLen;

                    /*将服务数据放入结构中*/

                    memcpy((puint8_t)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0],\
                            (puint8_t)&ucSyncRecvData[uiBuffIndex],uiSyncSrvDataLen);
                    uiBuffIndex += uiSyncSrvDataLen;

                    pstSyncData->sLinkASrvLen = uiSyncSrvDataLen + sizeof(SysPMStateInfo_t);
                    pstSyncData->sLinkASrvErr = RET_OK;
                    
                    /*将工程数据放入结构中*/
                    memcpy((puint8_t)&uiSyncPrjDataLen,(puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);

                    if((uiSyncPrjDataLen >= 0U) && (uiSyncPrjDataLen < MAX_PRJ_SYNC_SIZE))
                    {
                        if(uiSyncPrjDataLen > 0U)
                        {
                            DEBUG_SYNC("RECV N PRJ: %d\n",uiSyncPrjDataLen);
                        }
                        
                        pstSyncData->stSyncRecvMSGA.stSyncPrjData.uiSyncPrjDataLen = uiSyncPrjDataLen;
                        
                        memcpy((puint8_t)&pstSyncData->stSyncRecvMSGA.stSyncPrjData.ucPrjData[0],\
                                (puint8_t)&ucSyncRecvData[uiBuffIndex],uiSyncPrjDataLen);
                        uiBuffIndex += uiSyncPrjDataLen;
                        
                        pstSyncData->sLinkAPrjLen = uiSyncPrjDataLen;
                        pstSyncData->sLinkAPrjErr = RET_OK;
                    }
                }
            }
        }
        else
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkASrvLen = 0;
            pstSyncData->sLinkASrvErr = RET_FAIL;
            pstSyncData->sLinkAPrjLen = 0;
            pstSyncData->sLinkAPrjErr = RET_FAIL;
        }

        memset(&ucSyncRecvData[0], 0x00, MAX_SYNC_SIZE);
        uiBuffIndex = 0U;
        
        /* 接收链路B即下一个 PM 发送的服务数据 */
        sRetLen = PMBUS_Receive(PM_BUS_B, PM_BUFF_SYNC, (puint8_t)&ucSyncRecvData[0]);

        if((sRetLen >= SYNC_RECV_SIZE_0) && (sRetLen < MAX_SYNC_SIZE))
        {/* 接收长度大于等于零，接收到数据 */
            
            if(sRetLen < sizeof(SysPMStateInfo_t))
            {/*接收数据长度小于状态数据大小，接收长度错误*/
                pstSyncData->sLinkBSrvLen = 0;
                pstSyncData->sLinkBSrvErr = RET_FAIL;
                pstSyncData->sLinkBPrjLen = 0;
                pstSyncData->sLinkBPrjErr = RET_FAIL;
                DEBUG_SYNC("RECV P:Len < PMState !\n");
            }
            else
            {
                pstPMStateInfo = (SysPMStateInfo_t*)&ucSyncRecvData[0];
                if(uiRecvPrePackNum != (pstPMStateInfo->ucPackNum - 1)) 
                {
                    DEBUG_SYNC("P: last %d Num %d Len %d\n",\
                            uiRecvPrePackNum,pstPMStateInfo->ucPackNum,sRetLen);
//                    printe("3");
                }
                
                uiRecvPrePackNum = pstPMStateInfo->ucPackNum;

                if(ACTIVE == pstPMStateInfo->emActiveFlag)
                {
                    /*将状态数据放入结构中*/
                    memcpy((puint8_t)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.stPMState,\
                            (puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(SysPMStateInfo_t));
                    
                }
                else
                {
                    DEBUG_SYNC("P: ACTIVE != PMState \n");
                }
                uiBuffIndex += sizeof(SysPMStateInfo_t);
                
                /*获取服务数据长度*/
                memcpy((puint8_t)&uiSyncSrvDataLen,(puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                if((uiSyncSrvDataLen >= 0U) && (uiSyncSrvDataLen < CFG_MAX_SYNC_SRV_SIZE))
                {
                    
                    if(uiSyncSrvDataLen > 0U)
                    {
                        DEBUG_SYNC("RECV P SEV: %d \n",uiSyncSrvDataLen);
                    }
                    
                    pstSyncData->stSyncRecvMSGB.stSyncSrvData.uiSyncSrvDataLen = uiSyncSrvDataLen;
                    
                    /*将服务数据放入结构中*/

                    memcpy((puint8_t)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0],\
                            (puint8_t)&ucSyncRecvData[uiBuffIndex],uiSyncSrvDataLen);
                    uiBuffIndex += uiSyncSrvDataLen;
                    
                    pstSyncData->sLinkBSrvLen = uiSyncSrvDataLen + sizeof(SysPMStateInfo_t);
                    pstSyncData->sLinkBSrvErr = RET_OK;
                    
                    /*将工程数据放入结构中*/
                    memcpy((puint8_t)&uiSyncPrjDataLen,(puint8_t)&ucSyncRecvData[uiBuffIndex],sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    
                    if((uiSyncPrjDataLen >= 0U) && (uiSyncPrjDataLen < MAX_PRJ_SYNC_SIZE))
                    {
                        if(uiSyncPrjDataLen > 0U)
                        {
                            DEBUG_SYNC("RECV P PRJ: %d\n",uiSyncPrjDataLen);
//                            printe("2");
                        }
                        
                        pstSyncData->stSyncRecvMSGB.stSyncPrjData.uiSyncPrjDataLen = uiSyncPrjDataLen;
                        
                        memcpy((puint8_t)&pstSyncData->stSyncRecvMSGB.stSyncPrjData.ucPrjData[0],\
                                (puint8_t)&ucSyncRecvData[uiBuffIndex],uiSyncPrjDataLen);
                        uiBuffIndex += uiSyncPrjDataLen;
                        
                        pstSyncData->sLinkBPrjLen = uiSyncPrjDataLen;
                        pstSyncData->sLinkBPrjErr = RET_OK;
                    }
                }
            }
        }
        else
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkBSrvLen = 0;
            pstSyncData->sLinkBSrvErr = RET_FAIL;
            pstSyncData->sLinkBPrjLen = 0;
            pstSyncData->sLinkBPrjErr = RET_FAIL;
        }
    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendSyncData
*
* 功能描述: 发送同步数据
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
void SyncSendSyncData(SyncData_t* pstSyncData,bool_t bForceSendFlg)
{
    static uint32_t s_uiStateSendFlag1 = 0U;                            /* 系统启动时第一次调用标识 */
    static uint32_t s_uiStateSendFlag2 = 0U;                            /* 系统启动时第一次调用标识 */
    int32_t iSendLen = 0;                                               /* 发送数据长度             */
    int16_t sRetSend = RET_OK;                                          /* 发送返回值               */
    uint32_t uiTimeValue = 0U;                                          /* 时间差值                 */
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PMID号     */
    timer_status_t emTimerStatus = SysGetTimerStatus(emLocalPMId);      /* 本PM同步时钟主从状态 */
    SysPMStateInfo_t *pstPMStateInfo = SysGetPMStateInfo(emLocalPMId);  /* 本机工程信息数据指针     */
    uint8_t ucSyncSendBuff[MAX_SYNC_SIZE];
    uint32_t uiBuffIndex = 0U;
    static uint32_t uiPackNum1 = 0;
    static uint32_t uiPackNum2 = 0;
    uint8_t ucNextSendFlag = 0U;
    uint8_t ucPreSendFlag = 0U;

    if(pstSyncData != NULL)
    {
        if((SysGetWorkStatus(emNextPMId) != WS_OFFLINE) || (true == bForceSendFlg) || (s_uiStateSendFlag1 > SYS_SEND_STAT))
        {/*后一系在线*/
            memset((puint8_t)ucSyncSendBuff, 0x00, MAX_SYNC_SIZE);
            uiBuffIndex = 0U;
            
//            /* 填充本PM信息数据 */
//            pstPMStateInfo->emSwitchKeys = SharedGetSwitchPos();
//            pstPMStateInfo->emWorkStatus = SysGetWorkStatus(emLocalPMId);
//            pstPMStateInfo->emTaskStatus[TASK_UP1] = SysGetTaskStatus(emLocalPMId,TASK_UP1);
//            pstPMStateInfo->emTaskStatus[TASK_UP2] = SysGetTaskStatus(emLocalPMId,TASK_UP2);
            
            memcpy(&pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState, pstPMStateInfo, sizeof(SysPMStateInfo_t));

            
            if((AJUST_EN == SysGetAjustTimerEn(emNextPMId)) && 
               (SYS_AJUST_SYNCTIME_NOREADY == SyncGetAjustTimerFlag()))
            {/* 若本机同步时钟状态为主机，且当前时间不在前半秒钟，若非启动时调用，退出 */
                pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.emAjustEn = AJUST_DISEN;
            }
            else
            {
                pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.emAjustEn = AJUST_EN;
                pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.uiMaster = emTimerStatus;

                if(TIMER_MASTER == emTimerStatus)
                {
                    /* 设置本机同步时间已校标识 */
                    SysSetSyncAjust(AJUST_HAS);
                }
                
                if(AJUST_EN == SysGetAjustTimerEn(emNextPMId))
                {
                    SysSetAjustTimerEn(emLocalPMId, AJUST_EN);
                }
                else
                {
                    SysSetAjustTimerEn(emLocalPMId, AJUST_DISEN);
                }
                
                /* 计算校时时间 */
                SysGetSyncTime(&pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime);
                uiTimeValue = pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime.stTimeStruct.uiLow32b;
                pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime.ullTime += \
                    SYS_ONE_SEC_VALUE - (uiTimeValue % SYS_ONE_SEC_VALUE);
            }
            
            pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.emActiveFlag = ACTIVE;
            pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState.ucPackNum = uiPackNum1;
            
            memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncSrvData.stPMState, \
                    sizeof(SysPMStateInfo_t));
            uiBuffIndex += sizeof(SysPMStateInfo_t);

            if((pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen > 0) && \
                    (pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen < CFG_MAX_SYNC_SRV_SIZE))
            {
                DEBUG_SYNC("SEND N: SRV %d\n", pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen);
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                /*拷贝同步服务数据*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncSrvData.ucSrvData[0],\
                        pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen);
                uiBuffIndex += pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                
                ucNextSendFlag = 1U;
            }
            else
            {
                pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen = 0U;
                
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
            }
            
            if((pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen > 0) && \
                    (pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen < MAX_PRJ_SYNC_SIZE))
            {
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                /*拷贝同步服务数据*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGA.stSyncPrjData.ucPrjData[0],\
                        pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen);
                uiBuffIndex += pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen;
                
                ucNextSendFlag = 1U;
                
                DEBUG_SYNC("SEND N: PRJ %d\n", pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen);
//                printe("1");
            }
            iSendLen = (int32_t)uiBuffIndex;
            
            if((s_uiStateSendFlag1 > SYS_SEND_STAT) || (true == bForceSendFlg))
            {
                ucNextSendFlag = 1U;
            }

            if(1U == ucNextSendFlag)
            {
                uiPackNum1++;
                s_uiStateSendFlag1 = 0U;
                sRetSend = PMBUS_Send(PM_BUS_A, PM_BUFF_SYNC, (puint8_t)&ucSyncSendBuff[0], iSendLen, PMBUS_SEND_TIMEOUT);

                if(sRetSend != RET_OK)
                {/* 发送错误重新启动文件传输 */
                    /*log*/
                    SyncInitFileStatus();
                    DEBUG_SYNC("N SEND ERR \n");
//                    printe("4");
                }
                else
                {/* 发送成功，等待应答 */
                    
                    if(true == bForceSendFlg)
                    {
                        DEBUG_SYNC("SEND NEXT OK !\n");
                    }
                    if(pstSyncData->stSyncSendMSGA.stSyncPrjData.uiSyncPrjDataLen > 0)
                    {
                        SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
                    }
                }
            }
        }
        
        if((SysGetWorkStatus(emPrePMId) != WS_OFFLINE) || (true == bForceSendFlg) || (s_uiStateSendFlag2 > SYS_SEND_STAT))
        {/*前一系在线*/
            memset((puint8_t)ucSyncSendBuff, 0x00, MAX_SYNC_SIZE);
            uiBuffIndex = 0U;
            
//            /* 填充本PM信息数据 */
//            pstPMStateInfo->emSwitchKeys = SharedGetSwitchPos();
//            pstPMStateInfo->emWorkStatus = SysGetWorkStatus(emLocalPMId);
//            pstPMStateInfo->emTaskStatus[TASK_UP1] = SysGetTaskStatus(emLocalPMId,TASK_UP1);
//            pstPMStateInfo->emTaskStatus[TASK_UP2] = SysGetTaskStatus(emLocalPMId,TASK_UP2);
            
            /* 填充本PM信息数据 */
            memcpy(&pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState, pstPMStateInfo, sizeof(SysPMStateInfo_t));
            pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.emWorkStatus = SysGetWorkStatus(emLocalPMId);
            
            if((AJUST_EN == SysGetAjustTimerEn(emPrePMId)) && 
               (SYS_AJUST_SYNCTIME_NOREADY == SyncGetAjustTimerFlag()))
            {/* 若本机同步时钟状态为主机，且当前时间不在前半秒钟，若非启动时调用，退出 */
                pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.emAjustEn = AJUST_DISEN;
            }
            else
            {
                pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.emAjustEn = AJUST_EN;
                pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.uiMaster = emTimerStatus;
                if(TIMER_MASTER == emTimerStatus)
                {
                    /* 设置本机同步时间已校标识 */
                    SysSetSyncAjust(AJUST_HAS);
                }
                
                if(AJUST_EN == SysGetAjustTimerEn(emPrePMId))
                {
                    SysSetAjustTimerEn(emLocalPMId, AJUST_EN);
                }
                else
                {
                    SysSetAjustTimerEn(emLocalPMId, AJUST_DISEN);
                }
                
                /* 计算校时时间 */
                SysGetSyncTime(&pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime);
                uiTimeValue = pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime.stTimeStruct.uiLow32b;
                pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.stAjustSyncTime.unSyncAjustTime.ullTime += \
                    SYS_ONE_SEC_VALUE - (uiTimeValue % SYS_ONE_SEC_VALUE);
            }
            
            pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.emActiveFlag = ACTIVE;
            pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState.ucPackNum = uiPackNum2;
            
            memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncSrvData.stPMState, \
                    sizeof(SysPMStateInfo_t));
            uiBuffIndex += sizeof(SysPMStateInfo_t);
            
            if((pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen > 0) && \
                    (pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen < CFG_MAX_SYNC_SRV_SIZE))
            {
                DEBUG_SYNC("SEND P:SRV %d\n", pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen);
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                /*拷贝同步服务数据*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncSrvData.ucSrvData[0],\
                        pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen);
                uiBuffIndex += pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;

                ucPreSendFlag = 1U;
            }
            else
            {
                pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen = 0U;
                
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
            }
            
            if((pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen > 0) && \
                    (pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen < MAX_PRJ_SYNC_SIZE))
            {
                /*拷贝同步服务数据长度*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], \
                        (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen, sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                /*拷贝同步服务数据*/
                memcpy((puint8_t)&ucSyncSendBuff[uiBuffIndex], (puint8_t)&pstSyncData->stSyncSendMSGB.stSyncPrjData.ucPrjData[0],\
                        pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen);
                uiBuffIndex += pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen;
                
                ucPreSendFlag = 1U;
                
                DEBUG_SYNC("SEND P:PRJ %d\n", pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen);
                
            }
            
            iSendLen = (int32_t)uiBuffIndex;
            
            if((s_uiStateSendFlag2 > SYS_SEND_STAT) || (true == bForceSendFlg))
            {
                ucPreSendFlag = 1U;
            }
            
            if(1U == ucPreSendFlag)
            {
                uiPackNum2++;
                s_uiStateSendFlag2 = 0U;
                sRetSend = PMBUS_Send(PM_BUS_B, PM_BUFF_SYNC, (puint8_t)&ucSyncSendBuff[0], iSendLen, PMBUS_SEND_TIMEOUT);
                
                if(sRetSend != RET_OK)
                {/* 发送错误重新启动文件传输 */
                    /*log*/
                    SyncInitFileStatus();
                    DEBUG_SYNC("P SEND ERR \n");
                }
                else
                {/* 发送成功，等待应答 */
                    
                    if(true == bForceSendFlg)
                    {
                        DEBUG_SYNC("SEND PRE OK !\n");
                    }
                    if(pstSyncData->stSyncSendMSGB.stSyncPrjData.uiSyncPrjDataLen > 0)
                    {
                        
                        SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
                    }
                }
            }
        }
    }

    s_uiStateSendFlag1++;
    s_uiStateSendFlag2++;
    
    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleSrvData
*
* 功能描述: 处理接收到来自其它PM状态数据，根据状态数据类型调用相应处理函数，若数据类型为应答数据， 置接收到应答数据标识，不对数据进行处理。
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
void SyncHandleSrvData(SyncData_t* pstSyncData, puint32_t puiSrvAckHandleFlag)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                      /* 数据处理结果         */
    PMController_t emLocalPMId = SysGetLocalPMID();                /* 本地PM ID号       */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);       /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);         /* 前一个PM ID号     */
    SrvData_t* pstSrvData = NULL;

    /* 设置未收到应答数据 */
    *puiSrvAckHandleFlag = SYNC_SRVACK_NOHANDLE;
    
    pstSrvData = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];

    if((pstSyncData->sLinkASrvLen > SYNC_RECV_SIZE_0) && (pstSrvData != NULL))
    {/* 处理链接A接收到数据，即后一个PM 发送的数据 */

        if(ACTIVE == pstSyncData->stSyncRecvMSGA.stSyncSrvData.stPMState.emActiveFlag)
        {/* 若数据长度等于PM状态数据结构，说明只有有状态数据 */
         /*具体的PM同步状态信息包含哪些内容，需要经讨论决定，而后确定具体处理函数*/
            uiRet = SyncHandleStateData(&pstSyncData->stSyncRecvMSGA.stSyncSrvData.stPMState, emNextPMId, \
                    pstSyncData->sLinkASrvLen);
        }
        if(pstSyncData->stSyncRecvMSGA.stSyncSrvData.uiSyncSrvDataLen > 0)
        {
            if((SRV_DATA == pstSrvData->emType) && (CS1131_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为CS1131服务 */
                uiRet = SyncHandleCS1131SrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                        pstSrvData->uiSrvDataLen);
                
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (OPC_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为OPC服务 */
                uiRet = SyncHandleOPCSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                                                pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (P2P_REQ_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PReqSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                                                pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (P2P_RSP_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PRspSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                                                pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (MODBUS_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleModbusSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                                                pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (CTRL_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为PM 间服务 */
                uiRet = SyncHandleCtrlSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                                                pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (CLIENT_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为Client服务 */
                uiRet = SyncHandleDiagSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0], emNextPMId, \
                        pstSrvData->uiSrvDataLen);
                
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if(SRVACK_DATA == pstSrvData->emType)
            {/* 若数据类型为应答数据，置接收到应答数据标识 */
                *puiSrvAckHandleFlag |= BITMASK(SYNC_SRVACK_HANDLE_BITA);
                DEBUG_SYNC("RECV N SRVACK\n");
            }
            else
            {/* 接收到状态数据类型错误 */
                uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
                
                DEBUG_SYNC("N STAT_TYPE_ERR : %d  %d\n", \
                        pstSrvData->emType,pstSrvData->emSubType);
            }
        }
        else
        {
        }

        if(uiRet != SYNC_HANDLE_STATDATA_OK)
        {/* 数据处理错误 */
        }
    }

    
    uiRet = SYNC_HANDLE_STATDATA_OK;
    /* handle the link b stat data */
    
    pstSrvData = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
    
    if((pstSyncData->sLinkBSrvLen > SYNC_RECV_SIZE_0) && (pstSrvData != NULL))
    {/* 处理链接B接收到数据，即前一个PM 发送的数据 */
        if(ACTIVE == pstSyncData->stSyncRecvMSGB.stSyncSrvData.stPMState.emActiveFlag)
        {/* 若数据长度等于PM状态数据结构，说明只有状态数据 */
            uiRet = SyncHandleStateData(&pstSyncData->stSyncRecvMSGB.stSyncSrvData.stPMState, emPrePMId, \
                                        pstSyncData->sLinkBSrvLen);
        }
        if(pstSyncData->stSyncRecvMSGB.stSyncSrvData.uiSyncSrvDataLen > 0)
        {
            if((SRV_DATA == pstSrvData->emType) && (CS1131_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为CS1131服务 */
                uiRet = SyncHandleCS1131SrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (OPC_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为OPC服务 */
                uiRet = SyncHandleOPCSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (P2P_REQ_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PReqSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (P2P_RSP_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PRspSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (MODBUS_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleModbusSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (CTRL_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为PM 间服务 */
                uiRet = SyncHandleCtrlSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSrvData->emType) && (CLIENT_SRV == pstSrvData->emSubType))
            {/* 若数据类型为服务数据，服务类型为Client服务 */
                uiRet = SyncHandleDiagSrvData((SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0], emPrePMId, \
                        pstSrvData->uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if(SRVACK_DATA == pstSrvData->emType)
            {/* 若数据类型为应答数据，置接收到应答数据标识 */
                *puiSrvAckHandleFlag |= BITMASK(SYNC_SRVACK_HANDLE_BITB);
                
                DEBUG_SYNC("RECV P SRVACK\n");
            }
            else
            {/* 接收到状态数据类型错误 */
                 uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
                 
                 DEBUG_SYNC("P STAT_TYPE_ERR :%d  %d\n", \
                         pstSrvData->emType,pstSrvData->emSubType);
            }
        }
        else
        {
        }

        if(uiRet != SYNC_HANDLE_STATDATA_OK)
        {/* 数据处理错误 */
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleStateData
*
* 功能描述: 处理接收到PM 信息数据，若发送端需要应答，则发送本机PM 信息应答。
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
uint32_t SyncHandleStateData(const SysPMStateInfo_t* pstOneStatData, PMController_t emSrcPMId, int16_t sDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    SysPMStateInfo_t* pstPMState = NULL;
    SysPMStateInfo_t* pstLocalPMState = NULL;
    SyncTime64_t unCurTime;                                         /* 当前同步时间             */
    
    pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
    if((pstLocalPMState != NULL) && (pstOneStatData != NULL) && (ACTIVE == pstOneStatData->emActiveFlag) && \
       (sDataLen >= sizeof(SysPMStateInfo_t)))
    {
        if((pstOneStatData->emPoweronFlg != PM_POWERON) || \
           ((pstLocalPMState->emSyncStatus == SS_NOSYNC) && (pstLocalPMState->emDownloadFlg == NOT_DOWNLOAD)))
        {
            pstPMState = SysGetPMStateInfo(emSrcPMId);
            /*将接收到的源PM的状态信息记录在本地*/
            memcpy(pstPMState, pstOneStatData, sizeof(SysPMStateInfo_t));

            if(INIT_DOWNLOAD == SharedGetDownloadFlg())
            {
                if(pstPMState->emWorkStatus != WS_STANDBY)
                {
                    pstPMState->emWorkStatus = WS_STANDBY;
                }
            }
            SysSetWorkStatus(emSrcPMId, pstPMState->emWorkStatus);
            

            
            if((TIMER_MASTER == pstPMState->stAjustSyncTime.uiMaster) && 
               (AJUST_EN == pstPMState->stAjustSyncTime.emAjustEn))
            {/* 发送端PM同步时钟状态为主 */
                if(AJUST_NO == SysGetSyncAjust())
                {/* 若本机未接收过校时，直接校时处理 */
                    SysSetSyncTime(&pstPMState->stAjustSyncTime.unSyncAjustTime);
                    SysSetSyncAjust(AJUST_HAS);
                    
                    DEBUG_SYNC("AJUST_HAS\n");
                }
                else
                {/* 若本机已接收过校时，若接收到校时时间小于当前时间，记录日志 */
                    SysGetSyncTime(&unCurTime);
                    if(pstPMState->stAjustSyncTime.unSyncAjustTime.ullTime < unCurTime.ullTime)
                    {
                        /*log*/
    //                    DEBUG_SYNC("LOG_SYNC_AJUSTTIME_BACK \n");
                    }
                }
            }
    
            SysSetAjustTimerEn(emSrcPMId, AJUST_DISEN);
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_CTRLDATA_ACKERR;
    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleCS1131SrvData
*
* 功能描述: CS1131服务同步包处理函数。
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
uint32_t SyncHandleCS1131SrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0;                                                               /* 应答数据长度             */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, CS1131_SRV);        /* CS1131服务数据指针 ,存放在共享内存上的1131服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */
//    uint32_t uiBuffIndex = 0U;

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleOPCSrvData
*
* 功能描述: OPC服务同步包处理函数。
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
uint32_t SyncHandleOPCSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0;                                                             /* 应答数据长度             */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, OPC_SRV);           /* OPC服务数据指针 ,存放在共享内存上的OPC服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */
//    uint32_t uiBuffIndex = 0U;

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleP2PReqSrvData
*
* 功能描述: P2P请求服务同步包处理函数。
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
uint32_t SyncHandleP2PReqSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0;                                                             /* 应答数据长度             */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, P2P_REQ_SRV);       /* P2P请求服务数据指针 ,存放在共享内存上的P2P服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                        (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                        (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleP2PRspSrvData
*
* 功能描述: P2P应答服务同步包处理函数。
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
uint32_t SyncHandleP2PRspSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0;                                                             /* 应答数据长度             */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, P2P_RSP_SRV);       /* P2P应答服务数据指针 ,存放在共享内存上的P2P服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */;

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                        (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                        (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleModbusSrvData
*
* 功能描述: Modbus数据同步包处理函数。
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
uint32_t SyncHandleModbusSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0U;                                                            /* 应答数据长度             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, MODBUS_SRV);        /* Modbus数据指针 ,存放在共享内存上的Modbus服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}



/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleCtrlSrvData
*
* 功能描述: PM内部命令同步包处理函数。
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
uint32_t SyncHandleCtrlSrvData(const SrvData_t* pstSrvData, PMController_t emSrcPMId,
                               uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                   /* 数据处理结果             */
    uint32_t uiSendLen = 0U;                                                    /* 应答数据长度             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 后一系PM ID号           */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                                         /* 服务数据指针           */
    SyncCtrlSrv_t* pstSyncCtrlSrv = NULL;                                       /* PM 服务数据指针       */
    SrvData_t stSyncSrvACKData;                                                 /* 应答数据结构             */
    uint32_t uiBuffIndex = 0U;

    /* 设置PM 服务指针初始值 */
    pstSyncCtrlSrv = (SyncCtrlSrv_t*)&pstSrvData->ucData;

    uiBuffIndex += sizeof(SyncCtrlSrv_t);

    /* PM 服务号进行相应处理，共有两种PM 服务:
     * 1、系统由2系运行到3系运行升级重构时，请求加入服务
     * 2、PM 同步失败时，取消同步数据服务 */
    switch(pstSyncCtrlSrv->uiSrvId)
    {
        case SYS_BROADCAST_CTRLSRV:
        {
            pstSysSrvInfo = SysGetCtrlSrvInfo(pstSrvData->emCMid);
            
            if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
            {
                /* 将服务数据拷贝到共享内存上服务数据结构中 */
                memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[uiBuffIndex], uiDataLen - uiBuffIndex);
                /* 默认设置服务同步正确 */
                pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
                /* 默认设置有服务需要处理标识 */
                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
            
                /* 若该状态数据需要应答 */
                if(NEED_ACK == pstSrvData->emAckFlag)
                {/**/
                    /* 清空应答数据结构 */
                    memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
                    /* 填充应答数据头 */
                    stSyncSrvACKData.emType = SRVACK_DATA;
                    stSyncSrvACKData.emSubType = NOT_USED;
                    stSyncSrvACKData.emAckFlag = NO_ACK;
                    stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
                    /* 应答接收服务OK */
                    stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
                    /* 计算应答数据长度 */
                    uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                                sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                                sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
                    
                    /*将服务应答数据发送发送缓存*/
                    if(emNextPMId == emSrcPMId)
                    {
//                        uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvACKData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
                    }
                    if(emPrePMId == emSrcPMId)
                    {
//                        uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvACKData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
                    }
                }
            }
            else
            {
                uiRet = SYNC_HANDLE_CTRLDATA_ACKERR;
                
                DEBUG_SYNC("Ctrl: SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
            }
        }
            
        break;

        case SYS_LAST_SRV_CANCEL:
        {
            if(CTRL_SRV == pstSyncCtrlSrv->emSrvType)
            {
                pstSysSrvInfo = SysGetCtrlSrvInfo(pstSrvData->emCMid);
            }
            else
            {
                pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, pstSyncCtrlSrv->emSrvType);
            }
            
            if(pstSysSrvInfo != NULL)
            {
                /* 接收到取消服务 */
                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            }
        }

        break;
        default:
            
        break;
    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleDiagSrvData
*
* 功能描述: CLIENT服务同步包处理函数。
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
uint32_t SyncHandleDiagSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                                           /* 数据处理结果             */
    uint32_t uiSendLen = 0;                                                             /* 应答数据长度             */
    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, CLIENT_SRV);        /* Client服务数据指针 ,存放在共享内存上的Diag服务数据结构*/
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 后一系PM ID号           */
//    uint32_t uiBuffIndex = 0U;

    if((pstSysSrvInfo != NULL) && (uiDataLen <= CFG_SINGLE_SRV_SIZE))
    {
        /* 将服务数据拷贝到共享内存上服务数据结构中 */
        memcpy((puint8_t)pstSysSrvInfo, &pstSrvData->ucData[0], uiDataLen);
        /* 默认设置服务同步正确 */
        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
        /* 默认设置有服务需要处理标识 */
        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;

        /* 若该状态数据需要应答 */
        if(NEED_ACK == pstSrvData->emAckFlag)
        {/**/
            /* 清空应答数据结构 */
            memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
            /* 填充应答数据头 */
            stSyncSrvACKData.emType = SRVACK_DATA;
            stSyncSrvACKData.emSubType = NOT_USED;
            stSyncSrvACKData.emAckFlag = NO_ACK;
            stSyncSrvACKData.uiSrvDataLen = sizeof(uint8_t);
            /* 应答接收服务OK */
            stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
            /* 计算应答数据长度 */
            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
                        sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag) + \
                        sizeof(stSyncSrvACKData.emCMid) + sizeof(stSyncSrvACKData.uiSrvDataLen);
            
            /*将服务应答数据发送发送缓存*/
            if(emNextPMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == emSrcPMId)
            {
//                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], (puint8_t)&stSyncSrvACKData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
            }
        }
    }
    else
    {
        uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
        
        DEBUG_SYNC("SrvInfo NULL CMid: %d\n", pstSrvData->emCMid);
    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastCS1131Srv
*
* 功能描述: 广播CS1131服务
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
void SyncBroadCastCS1131Srv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送CS1131服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, CS1131_SRV);            /* 全局CS1131服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步CS1131服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */

    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = CS1131_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    break;
                }

                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > SINGLE_SRV\n");
                    break;
                }
    
                /* 填充CS1131服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));

                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送CS1131服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
                                DEBUG_SYNC("N: SendLen %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
                                DEBUG_SYNC("P: SendLen %d\n",uiSendLen);

                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送CS1131服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);

                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
                        DEBUG_SYNC("N: SendLen %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
                        DEBUG_SYNC("P: SendLen %d\n",uiSendLen);
                        
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:

                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，CS1131服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;

                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("WAIT ACK TO  Cur: %d Time: %d\n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon,CS1131_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }
    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastOPCSrv
*
* 功能描述: 广播OPC服务
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
void SyncBroadCastOPCSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送OPC服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, OPC_SRV);               /* 全局OPC服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步OPC服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */

    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = OPC_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    break;
                }
                
                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > SINGLE_SRV\n");
                    break;
                }
    
                /* 填充OPC服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
                
                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送CS1131服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND N: SendLen: %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND P: SendLen: %d\n",uiSendLen);
                                
                                
                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送OPC服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);

                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND N: SendLen: %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND P: SendLen: %d\n",uiSendLen);
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:
                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
    
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，OPC服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;

                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("OPC WAIT ACK TO  Cur: %d Time: %d \n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon, OPC_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastModbusSrv
*
* 功能描述: 广播Modbus服务
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
void SyncBroadCastModbusSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};  /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                 /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                              /* 计数值                   */
    uint32_t uiSendLen = 0U;                                            /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                       /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                     /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                            /* 发送Modbus服务数据缓存       */
    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */
    SyncTime64_t unCurTime;                                             /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, MODBUS_SRV);            /* 全局Modbus服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步OPC服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */

    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = MODBUS_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    break;
                }
                
                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > SINGLE_SRV %d\n",CFG_SINGLE_SRV_SIZE);
                    break;
                }
    
                /* 填充OPC服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
                
                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送MODBUS服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND N: SendLen: %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND P: SendLen: %d\n",uiSendLen);
                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送Modbus服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);

                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND N: SendLen: %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND P: SendLen: %d\n",uiSendLen);
                        
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:

                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
    
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，OPC服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;

                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("MODBUS WAIT ACK TO  Cur: %d Time: %d\n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon, MODBUS_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastP2PReqSrv
*
* 功能描述: 广播P2P请求服务
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
void SyncBroadCastP2PReqSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送P2P服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, P2P_REQ_SRV);           /* 全局P2P请求服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步P2P服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */
    
    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = P2P_REQ_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    break;
                }
                
                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > SINGLE_SRV\n");
                    break;
                }
    
                /* 填充P2P服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
                
                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送CS1131服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND N: SendLen: %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND P: SendLen: %d\n",uiSendLen);
                                
                                
                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送OPC服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
                    
                    
                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND N:Len: %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND P:Len: %d\n",uiSendLen);
                        
                    }
                }
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:
                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
    
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
    
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，OPC服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
                    
    
                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("P2P WAIT ACK TO  Cur: %d Time: %d \n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon, P2P_REQ_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastP2PRspSrv
*
* 功能描述: 广播P2P应答服务
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
void SyncBroadCastP2PRspSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送P2P服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, P2P_RSP_SRV);           /* 全局P2P请求服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步P2P服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */
    
    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = P2P_RSP_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    break;
                }
                
                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > SINGLE_SRV\n");
                    break;
                }
    
                /* 填充P2P服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
                
                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送CS1131服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND N:Len: %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND P:Len: %d\n",uiSendLen);
                                
                                
                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送OPC服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);

                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND N:Len: %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND P:Len: %d\n",uiSendLen);
                        
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:
                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
    
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
    
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，OPC服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
                    
    
                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("P2P WAIT ACK TO  Cur: %d Time: %d \n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon, P2P_RSP_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }
    return;
}

void SyncBroadCastCancel(uint32_t uiDstRecord, srv_srvdata_type_t emSrvType)
{
    uint32_t uiSendLen = 0U;                                        /* 发送数据长度             */
    PMController_t emLocalPMId = SysGetLocalPMID();                /* 本地PM的ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PM ID号     */
    SrvData_t stSyncCancelData;                                     /* 取消服务数据             */
    SyncCtrlSrv_t* pstSyncCtrlSrv = NULL;                           /* PM服务指针           */
    uint32_t uiBuffIndex = 0U;


    /* PM服务指针初始化 */
    pstSyncCtrlSrv = (SyncCtrlSrv_t*)stSyncCancelData.ucData;
    /* 清空PM服务数据 */
    memset((puint8_t)stSyncCancelData.ucData, 0U, sizeof(SyncCtrlSrv_t));

    /* 填充状态数据头结构 */
    stSyncCancelData.emType = SRV_DATA;
    uiSendLen += sizeof(stSyncCancelData.emType);
    stSyncCancelData.emSubType = CTRL_SRV;
    uiSendLen += sizeof(stSyncCancelData.emSubType);
    stSyncCancelData.emAckFlag = NO_ACK;
    uiSendLen += sizeof(stSyncCancelData.emAckFlag);
    stSyncCancelData.emCMid = INVALID_CM_ID;
    uiSendLen += sizeof(stSyncCancelData.emCMid);
    /* 填充PM服务号 */
    pstSyncCtrlSrv->uiSrvId = SYS_LAST_SRV_CANCEL;
    pstSyncCtrlSrv->emSrvType = emSrvType;
    pstSyncCtrlSrv->unActiveTime.ullTime = 0U;
    pstSyncCtrlSrv->unTimeOutTime.ullTime = 0U;
    
    stSyncCancelData.uiSrvDataLen = sizeof(SyncCtrlSrv_t);
    
    uiSendLen += sizeof(stSyncCancelData.uiSrvDataLen);

    uiSendLen += sizeof(SyncCtrlSrv_t);

    if(uiDstRecord & BITMASK(PMA))
    {/* 需给PM A发送取消服务 */
        if(SysGetWorkStatus(PMA) != WS_OFFLINE)
        {/* PM A不处于离线状态 */
            /*将取消服务数据发送发送缓存*/
            if(emNextPMId == PMA)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == PMA)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            
            DEBUG_SYNC("Cancel A");
            
        }
    }

    if(uiDstRecord & BITMASK(PMB))
    {/* 需给PM B发送取消服务 */
        if(SysGetWorkStatus(PMB) != WS_OFFLINE)
        {/* PM B不处于离线状态 */
            /*将取消服务数据发送发送缓存*/
            if(emNextPMId == PMB)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == PMB)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            
            DEBUG_SYNC("Cancel B");
            
        }
    }

    if(uiDstRecord & BITMASK(PMC))
    {/* 需给PM C发送取消服务 */
        if(SysGetWorkStatus(PMC) != WS_OFFLINE)
        {/* PM C不处于离线状态 */
            /*将取消服务数据发送发送缓存*/
            if(emNextPMId == PMC)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            if(emPrePMId == PMC)
            {
                uiBuffIndex = s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen;
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[uiBuffIndex], \
                        (puint8_t)&stSyncCancelData,uiSendLen);
                
                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                
            }
            
            DEBUG_SYNC("Cancel C");
            
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastCtrlSrv
*
* 功能描述: 广播CTRL服务
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
void SyncBroadCastCtrlSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送OPC服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                                         /* 全局CTRL服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */
    SyncCtrlSrv_t stSyncCtrlSrv;                                                /* PM服务指针           */

    DEBUG_SYNC("Broad Ctrl\n");

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    
    pstSysSrvInfo = SysGetCtrlSrvInfo(stSyncTask.emUPTaskId);
    /* 根据同步PM内部命令服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */

    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = CTRL_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = stSyncTask.emUPTaskId;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                /* 填充PM服务号 */
                stSyncCtrlSrv.uiSrvId = SYS_BROADCAST_CTRLSRV;
                stSyncCtrlSrv.emSrvType = NOT_USED;
                stSyncCtrlSrv.unActiveTime.ullTime = 0U;
                stSyncCtrlSrv.unTimeOutTime.ullTime = 0U;
                
                stSyncSrvData.uiSrvDataLen = sizeof(SyncCtrlSrv_t);
                
                /* 填充Ctrl服务 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)&stSyncCtrlSrv, sizeof(SyncCtrlSrv_t));

                stSyncSrvData.uiSrvDataLen += sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    DEBUG_SYNC("SrvLen > %d \n",CFG_SINGLE_SRV_SIZE);
                    break;
                }
    
                /* 填充Ctrl服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[sizeof(SyncCtrlSrv_t)], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
                
                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送CTRL服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND N:Len: %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("SEND P:Len: %d\n",uiSendLen);
                                
                                
                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送CTRL服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
                    
                    
                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND N:Len: %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("SEND P:Len: %d\n",uiSendLen);
                        
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:
                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
    
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
    
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，CTRL服务同步成功 */
                    
                    DEBUG_SYNC("Ctrl OK \n");
                    
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
                    
    
                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("CTRL WAIT ACK TO  Cur: %d Time: %d \n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon,CTRL_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }

    return;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastDiagSrv
*
* 功能描述: 广播诊断服务
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
void SyncBroadCastDiagSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};          /* 等待应答记录             */
    static uint32_t s_uiRecordCon = 0U;                                         /* 广播取消处理记录         */
    uint32_t uiCount = 0U;                                                      /* 计数值                   */
    uint32_t uiSendLen = 0U;                                                    /* 发送数据长度             */
    long64_t llInternalTime = 0L;                                               /* 同步时间间隔             */
    PMController_t emLocalPMId = SysGetLocalPMID();                             /* 本地PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                    /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                      /* 前一个PM ID号     */
    PMController_t emDstPMId = PMA;                                             /* 目的PM ID号           */
    SrvData_t stSyncSrvData;                                                    /* 发送Diag服务数据缓存       */
    SysSyncTask_t stSyncTask;                                                   /* 同步任务                 */
    SyncTime64_t unCurTime;                                                     /* 当前时间                 */
    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, CLIENT_SRV);            /* 全局Diag服务数据指针       */
    SrvData_t* pstSyncSrvACK = NULL;                                            /* 同步服务应答数据指针       */

    /* 获取任务队列任务 */
    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    /* 根据同步Diag服务任务状态做相应处理，共有两种任务状态
     * 1、数据发送状态
     * 2、等待应答状态 */

    if(pstSysSrvInfo != NULL)
    {
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_SEND:
                /* 任务处于发送状态时，填充发送缓存 */
                stSyncSrvData.emType = SRV_DATA;
                uiSendLen += sizeof(stSyncSrvData.emType);
                stSyncSrvData.emSubType = CLIENT_SRV;
                uiSendLen += sizeof(stSyncSrvData.emSubType);
                stSyncSrvData.emCMid = emCMid;
                uiSendLen += sizeof(stSyncSrvData.emCMid);
                stSyncSrvData.emAckFlag = emAckFlag;
                uiSendLen += sizeof(stSyncSrvData.emAckFlag);
                
                if(emCMid > CM4)
                {
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("CMid: %d\n",stSyncSrvData.emCMid);
                    break;
                }

                stSyncSrvData.uiSrvDataLen = sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                                      + pstSysSrvInfo->stSysSrvData.uiSrvLen + sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);

                if(stSyncSrvData.uiSrvDataLen > CFG_SINGLE_SRV_SIZE)
                {
                    stSyncSrvData.uiSrvDataLen = 0;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    DEBUG_SYNC("SrvLen > %d\n",CFG_SINGLE_SRV_SIZE);
                    break;
                }
    
                /* 填充CS1131服务，此处仅发送服务实际长度，而非结构体全部长度 */
                memcpy(&stSyncSrvData.ucData[0], (puint8_t)pstSysSrvInfo, \
                              sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.usSrvNo) \
                              + pstSysSrvInfo->stSysSrvData.uiSrvLen + \
                              sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));

                uiSendLen += sizeof(stSyncSrvData.uiSrvDataLen);
                
                /* 计算发送数据长度 */
                uiSendLen += stSyncSrvData.uiSrvDataLen;
    
                if(INVALID_PM_ID == stSyncTask.emPMId)
                {/* 广播方式发送Diag服务 */
                    for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
                    {
                        emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
                        if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
                        {
                            /* 记录需接收应答标识 */
                            s_uiWaitAck[emDstPMId] = NEED_ACK;
                            s_uiRecordCon |= BITMASK(emDstPMId);

                            /*将服务数据发送发送缓存*/
                            if(emNextPMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("N: SendLen %d\n",uiSendLen);
                                
                            }
                            if(emPrePMId == emDstPMId)
                            {
                                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                        (puint8_t)&stSyncSrvData,uiSendLen);
                                
                                s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                                
//                                DEBUG_SYNC("P: SendLen %d\n",uiSendLen);

                            }
                        }
                    }
                }
                else
                {/* 给指定PM 发送Diag服务 */
                    s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
                    s_uiRecordCon |= BITMASK(stSyncTask.emPMId);

                    /*将服务数据发送发送缓存*/
                    if(emNextPMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGA.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("N: SendLen %d\n",uiSendLen);
                        
                    }
                    if(emPrePMId == stSyncTask.emPMId)
                    {
                        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncSrvData.ucSrvData[0], \
                                (puint8_t)&stSyncSrvData,uiSendLen);
                        
                        s_stSyncData.stSyncSendMSGB.stSyncSrvData.uiSyncSrvDataLen += uiSendLen;
                        
//                        DEBUG_SYNC("P: SendLen %d\n",uiSendLen);
                        
                    }
                }
    
                /* 更新同步任务队列 */
                if(NO_ACK == emAckFlag)
                {
                    /* 若不需要接收应答，出队 */
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                }
                else
                {
                    /* 若需要接收应答，更新任务状态为等待应答 */
                    SysSyncSrvQueueUpdate(TASK_WAITACK);
                }
                break;
            case TASK_WAITACK:

                if(uiSrvAckHandleFlag)
                {/* 处理应答数据 */
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
                       NEED_ACK == s_uiWaitAck[emNextPMId])
                    {/* 接收到下一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGA.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emNextPMId] = NO_ACK;
                        }
                    }
                    if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
                       NEED_ACK == s_uiWaitAck[emPrePMId])
                    {/* 接收到前一个PM 应答数据 */
                        pstSyncSrvACK = (SrvData_t*)&pstSyncData->stSyncRecvMSGB.stSyncSrvData.ucSrvData[0];
                        if((pstSyncSrvACK != NULL) && (ACK_DATA_OK == pstSyncSrvACK->ucData[0]))
                        {/* 应答OK */
                            s_uiWaitAck[emPrePMId] = NO_ACK;
                        }
                    }
                }
                if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
                   && (NO_ACK == s_uiWaitAck[PMC]))
                {/* 超时时间内接收到应答，Diag服务同步成功 */
                    s_uiRecordCon = 0U;
                    SysDeSyncSrvQueue(pstSyncSrvQueue);
                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
                    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;

                    /*log*/
                }
                else
                {/* 判断是否超时 */
                    SysGetSyncTime(&unCurTime);
                    /* 计算当前时间与超时时间差值 */
                    llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                                    &unCurTime);
                    if(llInternalTime >= 0)
                    {/* 等待应答超时 */
                        
                        DEBUG_SYNC("WAIT ACK TO  Cur: %d Time: %d\n",unCurTime.stTimeStruct.uiLow32b, \
                                pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime.stTimeStruct.uiLow32b);
                        
                        SysDeSyncSrvQueue(pstSyncSrvQueue);
                        /* 发送取消服务 */
                        SyncBroadCastCancel(s_uiRecordCon,CLIENT_SRV);
                        s_uiRecordCon = 0U;
                        s_uiWaitAck[PMA] = NO_ACK;
                        s_uiWaitAck[PMB] = NO_ACK;
                        s_uiWaitAck[PMC] = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        /*log*/
                    }
                }
                break;
            default:
                break;
        }
    }
    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvRealData
*
* 功能描述: 接收实时数据服务
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
void SyncRecvRealData(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本机PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);    /* 本机PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);      /* 本机PMID号           */
    SysSyncTask_t stSyncTask;                                   /* 同步任务                 */
    SyncPrjData_t* pstSyncPrjData = NULL;
    SyncRealData_t* pstSyncRealData = NULL;
    int16_t sRecvErr = 0;
    static sync_srv_type_t s_emDEBUGSrvType = SYNC_FILE;

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

    if(stSyncTask.emPMId == emNextPMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncRecvMSGA.stSyncPrjData;
        sRecvErr = s_stSyncData.sLinkAPrjErr;
    }

    if(stSyncTask.emPMId == emPrePMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncRecvMSGB.stSyncPrjData;
        sRecvErr = s_stSyncData.sLinkBPrjErr;
    }

    if((pstSyncPrjData->uiSyncPrjDataLen > 0U) && (sRecvErr == RET_OK))
    {/* 接收到数据长度大于0,且接收数据正确 */
        /* 根据接收数据类型进行处理 */
        pstSyncRealData = (SyncRealData_t*)&pstSyncPrjData->ucPrjData[0];
        if((TASK_G_DATA == stSyncTask.emTaskStatus) || (TASK_I_DATA == stSyncTask.emTaskStatus) || \
           (TASK_Q_DATA == stSyncTask.emTaskStatus) || (TASK_SYNC_OK == stSyncTask.emTaskStatus))
        {
            switch(pstSyncRealData->stSyncHead.emSrvType)
            {
                case SYNC_REAL_DATA:
                    /* 接收到实时数据 */
                    if(s_emDEBUGSrvType != pstSyncRealData->stSyncHead.emSrvType)
                    {
                        printf("REAL_DATA\n");
                    }
                    
                    SyncRecvData(pstSyncRealData, stSyncTask.emPMId);
                    break;
                case SYNC_COMPLETE:
                    /* 接收到实时数据同步完成 */
                    if(s_emDEBUGSrvType != pstSyncRealData->stSyncHead.emSrvType)
                    {
                        printf("COMPLETE\n");
                    }

                    SyncRecvRealDataOk(pstSyncRealData,stSyncTask.emPMId);
                    break;
                default:
                    printf("ERR SrvType: %d\n",pstSyncRealData->stSyncHead.emSrvType);
                    break;
            }
            
            s_emDEBUGSrvType = pstSyncRealData->stSyncHead.emSrvType;
        }
    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvData
*
* 功能描述: 接收实时数据
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
void SyncRecvData(SyncRealData_t* pstSyncPrjData, PMController_t emSrcPMId)
{
    puint8_t pucAreaBaseAddr = 0U;                                   /* 数据区域基址         */
    int32_t iSendLen = 0;                                           /* 发送数据长度         */
    uint32_t uiRecvAck = ACK_DATA_ERR;                              /* 应答数据结果         */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    SyncStaticAck_t* pstSyncDataAck;                                /* 应答数据缓存         */
    SyncPrjData_t stSyncPrjDataSend;
    
    memset(&stSyncPrjDataSend, 0x00, sizeof(SyncPrjData_t));
    pstSyncDataAck = (SyncStaticAck_t*)&stSyncPrjDataSend.ucPrjData[0];

    do
    {
        if((pstSyncPrjData->stDataHead.emAreaId != RT_DATA_TYPE_G) &&
           (pstSyncPrjData->stDataHead.emAreaId != RT_DATA_TYPE_I) &&
           (pstSyncPrjData->stDataHead.emAreaId != RT_DATA_TYPE_Q))
        {
            DEBUG_SYNC("ERR AreaId: %d\n",pstSyncPrjData->stDataHead.emAreaId);
            break;
        }
        else
        {
            if(RT_DATA_TYPE_G == pstSyncPrjData->stDataHead.emAreaId)
            {
                if((pstSyncPrjData->stDataHead.uiOffset + pstSyncPrjData->stDataHead.uiDataLen) > MAX_RTDATA_G_AREA_LEN)
                {
                    DEBUG_SYNC("L > G_LEN\n");
                    break;
                }
            }
            
            if(RT_DATA_TYPE_I == pstSyncPrjData->stDataHead.emAreaId)
            {
                if((pstSyncPrjData->stDataHead.uiOffset + pstSyncPrjData->stDataHead.uiDataLen) > MAX_RTDATA_I_AREA_LEN)
                {
                    DEBUG_SYNC("L > I_LEN\n");
                    break;
                }
            }
            
            if(RT_DATA_TYPE_Q == pstSyncPrjData->stDataHead.emAreaId)
            {
                if((pstSyncPrjData->stDataHead.uiOffset + pstSyncPrjData->stDataHead.uiDataLen) > MAX_RTDATA_Q_AREA_LEN)
                {
                    DEBUG_SYNC("L > Q_LEN\n");
                    break;
                }
            }
        }
        if(pstSyncPrjData->stDataHead.emTaskID >= MAX_TASK_NUM)
        {
            DEBUG_SYNC("ERR TaskID %d\n",pstSyncPrjData->stDataHead.emTaskID);
            break;
        }

        /* 获取区域基地址 */
        pucAreaBaseAddr = SysGetSharedMemRtDataStartAddr(pstSyncPrjData->stDataHead.emAreaId);
        
        if(NULL == pucAreaBaseAddr)
        {
            DEBUG_SYNC("AreaBaseAddr NULL\n");
            break;
        }
        
        printf("AreaId: %d Off: %d Len: %d Addr %d\n",pstSyncPrjData->stDataHead.emAreaId,\
                pstSyncPrjData->stDataHead.uiOffset, pstSyncPrjData->stDataHead.uiDataLen,pucAreaBaseAddr);
       
        
        memcpy((pucAreaBaseAddr + pstSyncPrjData->stDataHead.uiOffset), \
                &pstSyncPrjData->ucData[0], pstSyncPrjData->stDataHead.uiDataLen);
        
        if(RT_DATA_TYPE_I == pstSyncPrjData->stDataHead.emAreaId)
        {
            SharedSetInputModuleVar(&pstSyncPrjData->ucData[0], 
                                    pstSyncPrjData->stDataHead.uiOffset,
                                    pstSyncPrjData->stDataHead.uiDataLen);
        }
        
//        if(LAST_PACK == pstSyncPrjData->stDataHead.emLastPackFlag)
//        {
//            if(RT_DATA_TYPE_Q == pstSyncPrjData->stDataHead.emAreaId)
//            {
//                SysSyncPrjQueueUpdate(TASK_COMPLETE);
//            }
//        }
        uiRecvAck = ACK_DATA_OK;
    }while(NULL);

    /* 打包应答数据 */
    pstSyncDataAck->stSyncHead.emSrvType = SYNC_REAL_DATA;
    pstSyncDataAck->ucData[0] = uiRecvAck;
    /* 计算发送数据长度 */
    iSendLen = sizeof(pstSyncDataAck->stSyncHead) + sizeof(pstSyncDataAck->ucData[0]);
    stSyncPrjDataSend.uiSyncPrjDataLen = iSendLen;
    
    iSendLen += sizeof(stSyncPrjDataSend.uiSyncPrjDataLen);

    if(emSrcPMId == emNextPMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
    }
    if(emSrcPMId == emPrePMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvRealDataOk
*
* 功能描述: 接收实时数据
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
void SyncRecvRealDataOk(SyncRealData_t* pstSyncPrjData,PMController_t emSrcPMId)
{
    int32_t iSendLen = 0;                                           /* 发送数据长度         */
    uint32_t uiRecvAck = ACK_DATA_ERR;                              /* 应答数据结果         */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    SyncStaticAck_t* pstSyncDataAck;                                /* 应答数据缓存         */
    SyncPrjData_t stSyncPrjDataSend;
    SharedAppend_t *pstAppend = NULL;
    uint32_t uiAppendDataLen = 0U;
    uint32_t uiDataLen = 0U;
    uint16_t usDisableTablLen = 0U;
    DisableTablHead_t* pstDisableTablHead = NULL;
    uint32_t uiCheckSumCal = 0U;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    
    uiAppendDataLen = MAX_MODBUS_CTRL_INFO*sizeof(uint8_t) + \
               MAX_P2P_CTRL_INFO*sizeof(uint8_t) + MAX_SOE_CTRL_INFO*sizeof(uint8_t);
    
    uiDataLen = pstSyncPrjData->stDataHead.uiDataLen;
    
    if(uiDataLen < uiAppendDataLen)
    {
        /*log*/
        printf("AppendData Len Err\n");
    }
    else if(NULL == pstAppend)
    {
        printf("pAppendData NULL\n");
    }
    else
    {
        memcpy((puint8_t)(&pstAppend->ucModBusCtrl[0]), &pstSyncPrjData->ucData[0], \
                uiAppendDataLen);
        uiDataLen -= uiAppendDataLen;
        if((uiDataLen > sizeof(DisableTablHead_t)) && (uiDataLen <= RETAIN_SRAM_DISABLE_SIZE))
        {
            usDisableTablLen = uiDataLen;
            printf("DisTablLen %d\n", usDisableTablLen);

            pstDisableTablHead = (DisableTablHead_t*)&pstSyncPrjData->ucData[uiAppendDataLen];
            if(usDisableTablLen == pstDisableTablHead->uiSize)
            {
                /*计算文件总CRC*/
                uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&pstSyncPrjData->ucData[uiAppendDataLen + sizeof(uint32_t)]), \
                        pstDisableTablHead->uiSize - sizeof(uint32_t));
                if(uiCheckSumCal != pstDisableTablHead->uiCheckSum)
                {
                    /*log*/
                    printf("DIS TAB CRC ERR\n");
                }
                else
                {
                    if((usDisableTablLen % 2) != 0)
                    {
                        usDisableTablLen = usDisableTablLen + 1;
                    }
                    SharedSetDisableTabl(&pstSyncPrjData->ucData[uiAppendDataLen], usDisableTablLen);
                    
                    SharedSetDisableRecoverFlg(NEED_DISABLE_RECOVER);
                    
//                    uint16_t ustest = 0U;
//                    for(ustest = sizeof(DisableTablHead_t); ustest < usDisableTablLen; ustest++)
//                    {
//                        printf("%x ",pstSyncPrjData->ucData[uiAppendDataLen + ustest]);
//                    }
//                    printf("\n");
                }
            }
        }
    }
    
    memset(&stSyncPrjDataSend, 0x00, sizeof(SyncPrjData_t));
    pstSyncDataAck = (SyncStaticAck_t*)&stSyncPrjDataSend.ucPrjData[0];

    SysSyncPrjQueueUpdate(TASK_COMPLETE);
    uiRecvAck = ACK_DATA_OK;


    /* 发送应答 */
    /* 打包应答数据 */
    pstSyncDataAck->stSyncHead.emSrvType = SYNC_COMPLETE;
    pstSyncDataAck->ucData[0] = uiRecvAck;
    /* 计算发送数据长度 */
    iSendLen = sizeof(pstSyncDataAck->stSyncHead) + sizeof(pstSyncDataAck->ucData[0]);
    stSyncPrjDataSend.uiSyncPrjDataLen = iSendLen;
    
    iSendLen += sizeof(stSyncPrjDataSend.uiSyncPrjDataLen);

    if(emSrcPMId == emNextPMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
    }
    if(emSrcPMId == emPrePMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
    }
  


    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendRealData
*
* 功能描述: 发送实时数据
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
void SyncSendRealData(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    static sync_task_stat_t s_emLastStatus = TASK_WAITACK;            /* 等待应答前状态   */
    SysSyncTask_t stSyncTask;                                       /* 同步任务         */
    static sync_task_stat_t s_emDEBUGLastStatus = TASK_WAITACK;            /* 等待应答前状态   */

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
    {
        DEBUG_SYNC("SEND_REAL ");
    }
    
    /* 根据任务状态发送相关数据 */
    switch(stSyncTask.emTaskStatus)
    {
        case TASK_G_DATA:
            /* 发送G区数据 */
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d G\n",stSyncTask.emUPTaskId);
            }
            
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_I_DATA:
            /* 发送I区数据 */
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d I\n",stSyncTask.emUPTaskId);
            }
            
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_Q_DATA:
            /* 发送Q区数据 */
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d Q\n",stSyncTask.emUPTaskId);
            }
            
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_SYNC_OK:
            /* 发送静态数据同步完成 */
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d SYNC_OK \n",stSyncTask.emUPTaskId);
            }

            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendDataOk(stSyncTask.emPMId);
            break;
        case TASK_COMPLETE:
            /* 发送静态数据同步完成 */
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d COMPLETE \n",stSyncTask.emUPTaskId);
            }

            s_emLastStatus = stSyncTask.emTaskStatus;
//            SyncSendDataOk(stSyncTask.emPMId);
            break;
        case TASK_WAITACK:
            if(s_emDEBUGLastStatus != stSyncTask.emTaskStatus)
            {
                printf(" %d WAITACK \n",stSyncTask.emUPTaskId);
            }

            /* 等待应答 */
            SyncWaitAck(stSyncTask.emPMId, s_emLastStatus);
            break;
        default:
            break;
    }
    
    s_emDEBUGLastStatus = stSyncTask.emTaskStatus;

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendRealData
*
* 功能描述: 发送实时数据
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
void SyncSendData(PMController_t emDstPMId)
{
    int32_t iSendLen = 0;                                               /* 发送数据长度         */
    uint32_t uiRealDataLen = 0U;                                        /* 实际数据长度         */
    puint8_t pucAreaBaseAddr = 0U;                                      /* 数据区域基址         */
    uint32_t uiAreaUsedLen = 0U;                                        /* 数据区域使用长度     */
    uint32_t uiAreaUsedOffset = 0U;                                     /* 数据区域使用的开始偏移     */
    SyncRealData_t* pstSyncSendData = NULL;                             /* 发送数据缓存         */
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);             /* 前一个PMID号     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();          /* 同步任务队列指针     */
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;
    SyncPrjData_t stSyncPrjData;
    SysSyncTask_t stSyncTask;                                           /* 同步任务         */

    pstSyncSendData = (SyncRealData_t*)&stSyncPrjData.ucPrjData[0];
    
    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    
    s_stSyncDataStatus.emTaskID = stSyncTask.emUPTaskId;

    pstTaskConfigInfo = SharedGetTaskConfigInfo(s_stSyncDataStatus.emTaskID);

    /* 初始化数据区基地址 */
    pucAreaBaseAddr = SysGetSharedMemRtDataStartAddr(s_stSyncDataStatus.emAreaId);

    DEBUG_SYNC("TaskID %d AreaId %d\n",s_stSyncDataStatus.emTaskID,s_stSyncDataStatus.emAreaId);

    if((pstTaskConfigInfo != NULL) && (pucAreaBaseAddr != NULL))
    {
        /* 初始化数据区使用长度 */
        if(RT_DATA_TYPE_G == s_stSyncDataStatus.emAreaId )
        {
            uiAreaUsedLen = pstTaskConfigInfo->uiGUsedSize;
            uiAreaUsedOffset = pstTaskConfigInfo->uiGOffset;
        }
        if(RT_DATA_TYPE_I == s_stSyncDataStatus.emAreaId )
        {
            uiAreaUsedLen = pstTaskConfigInfo->uiIUsedSize;
            uiAreaUsedOffset = pstTaskConfigInfo->uiIOffset;
            uiAreaUsedOffset = uiAreaUsedOffset - (MAX_RTDATA_G_AREA_LEN);
        }
        if(RT_DATA_TYPE_Q == s_stSyncDataStatus.emAreaId )
        {
            uiAreaUsedLen = pstTaskConfigInfo->uiOUsedSize;
            uiAreaUsedOffset = pstTaskConfigInfo->uiOOffset;
            uiAreaUsedOffset = uiAreaUsedOffset - (MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN);
        }
        
        

        pstSyncSendData->stSyncHead.emSrvType = SYNC_REAL_DATA;
    
        if(s_stSyncDataStatus.uiCurDataLen >= uiAreaUsedLen)
        {
            if(HAS_NONEXT_DATA == SyncGetNextArea(&s_stSyncDataStatus.emAreaId))
            {
                SysSyncPrjQueueUpdate(TASK_SYNC_OK);
                s_stSyncDataStatus.emAreaId = RT_DATA_TYPE_G;
                
                DEBUG_SYNC("SYNC_OK\n");
                
            }
            s_stSyncDataStatus.uiCurOffset = 0;
            s_stSyncDataStatus.uiCurDataLen = 0;
            
            
        }
        else
        {
            DEBUG_SYNC("UsedLen %d Off %d \n",uiAreaUsedLen,uiAreaUsedOffset);
            
            s_stSyncDataStatus.uiCurOffset = uiAreaUsedOffset + s_stSyncDataStatus.uiCurDataLen;
            
            /* 填充数据头 */
            pstSyncSendData->stDataHead.emTaskID = s_stSyncDataStatus.emTaskID;
            pstSyncSendData->stDataHead.emAreaId = s_stSyncDataStatus.emAreaId;
            pstSyncSendData->stDataHead.uiOffset = s_stSyncDataStatus.uiCurOffset;

            /* 计算实际数据长度并填充 */
            uiRealDataLen = MIN(CFG_MAX_SYNC_DATA_SIZE, uiAreaUsedLen - s_stSyncDataStatus.uiCurDataLen);
            
            pstSyncSendData->stDataHead.uiDataLen = uiRealDataLen;
            
            DEBUG_SYNC("CurOff : %d Len: %d\n",s_stSyncDataStatus.uiCurOffset,uiRealDataLen);
            
            memcpy(&pstSyncSendData->ucData[0], (puint8_t)(pucAreaBaseAddr + s_stSyncDataStatus.uiCurOffset), \
                            uiRealDataLen);
            
            s_stSyncDataStatus.uiCurDataLen += uiRealDataLen;
            if(s_stSyncDataStatus.uiCurDataLen >= uiAreaUsedLen)
            {
                pstSyncSendData->stDataHead.emLastPackFlag = LAST_PACK;
                
                DEBUG_SYNC("LAST_PACK \n");
                
            }
            else
            {
                pstSyncSendData->stDataHead.emLastPackFlag = NOT_LAST_PACK;
            }
            /* 计算发送数据长度 */
            iSendLen = sizeof(SyncHead_t) + sizeof(SyncDataHead_t) + uiRealDataLen;
            
            stSyncPrjData.uiSyncPrjDataLen = iSendLen;
            
            iSendLen += sizeof(stSyncPrjData.uiSyncPrjDataLen);
    
            if(emNextPMId == emDstPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData, (puint8_t)&stSyncPrjData, (uint32_t)iSendLen);
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                
            }
            if(emPrePMId == emDstPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData, (puint8_t)&stSyncPrjData, (uint32_t)iSendLen);
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                
            }
        }
    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendDataOk
*
* 功能描述: 发送实时数据完成
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
void SyncSendDataOk(PMController_t emDstPMId)
{
    int32_t iSendLen = 0U;                                          /* 发送数据长度         */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);         /* 前一个PMID号     */
    SyncPrjData_t stSyncPrjData;
    SyncRealData_t* pstSyncSendData = NULL;                             /* 发送数据缓存         */
    SharedAppend_t *pstAppend = NULL;
    uint32_t uiAppendDataLen = 0U;
    uint32_t uiDataLen = 0U;
    DisableTablHead_t stDisableTablHead;
    uint16_t usDisableTablLen = 0U;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    pstSyncSendData = (SyncRealData_t*)&stSyncPrjData.ucPrjData[0];
    pstSyncSendData->stSyncHead.emSrvType = SYNC_COMPLETE;

    uiAppendDataLen = MAX_MODBUS_CTRL_INFO*sizeof(uint8_t) + MAX_P2P_CTRL_INFO*sizeof(uint8_t) + \
                      MAX_SOE_CTRL_INFO*sizeof(uint8_t);
    
    memcpy(&pstSyncSendData->ucData[0], (puint8_t)(&pstAppend->ucModBusCtrl[0]), uiAppendDataLen);
    
    uiDataLen = uiAppendDataLen;

    if(TASK_UP1 == s_stSyncDataStatus.emTaskID)
    {
        memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
        SharedGetDisableTabl(&stDisableTablHead, (uint16_t)sizeof(DisableTablHead_t));
        usDisableTablLen = stDisableTablHead.uiSize;
        if((stDisableTablHead.uiSize > sizeof(DisableTablHead_t)) && 
           (stDisableTablHead.uiSize <= RETAIN_SRAM_DISABLE_SIZE))
        {
            if((usDisableTablLen % 2) != 0)
            {
                usDisableTablLen = usDisableTablLen + 1;
            }
        }
        else
        {
            usDisableTablLen = 0;
        }
        
        SharedGetDisableTabl(&pstSyncSendData->ucData[uiAppendDataLen], usDisableTablLen);
        
        uiDataLen += usDisableTablLen;
    }
    
    pstSyncSendData->stDataHead.uiDataLen = uiDataLen;
    
    DEBUG_SYNC("Len: %d\n",uiDataLen);
    
    /* 计算发送数据长度 */
    iSendLen = sizeof(SyncHead_t) + sizeof(SyncDataHead_t) + uiDataLen;
    
    stSyncPrjData.uiSyncPrjDataLen = iSendLen;
    
    iSendLen += sizeof(stSyncPrjData.uiSyncPrjDataLen);

    if(emNextPMId == emDstPMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData, (puint8_t)&stSyncPrjData, (uint32_t)iSendLen);
        SysSyncPrjQueueUpdate(TASK_WAITACK);
        
    }
    if(emPrePMId == emDstPMId)
    {
        memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData, (puint8_t)&stSyncPrjData, (uint32_t)iSendLen);
        SysSyncPrjQueueUpdate(TASK_WAITACK);
        
    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncGetNextArea
*
* 功能描述: 获取哪一个数据区ID
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
uint32_t SyncGetNextArea(SysRtDataType_t* pemCurAreaId)
{
    uint32_t uiRet = HAS_NONEXT_DATA;                                 /* 函数返回值           */

    switch(*pemCurAreaId)
    {
        case RT_DATA_TYPE_G:
            uiRet = HAS_NEXT_DATA;
            *pemCurAreaId = RT_DATA_TYPE_I;
            break;
        case RT_DATA_TYPE_I:
            uiRet = HAS_NEXT_DATA;
            *pemCurAreaId = RT_DATA_TYPE_Q;
            break;
        case RT_DATA_TYPE_Q:
            uiRet = HAS_NONEXT_DATA;
            *pemCurAreaId = RT_DATA_TYPE_G;
            break;
        default:
            printf("ERR AreaId: %d\n",*pemCurAreaId);
            break;
    }

    return uiRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncWaitAck
*
* 功能描述: 发送实时数据后，等待应答。
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
void SyncWaitAck(PMController_t emSrcPMId, sync_task_stat_t emLastStatus)
{
    int16_t sRetLen = SYNC_RECV_SIZE_0;                             /* 接收数据长度         */
    SyncStaticAck_t* pstSyncAck = NULL;                             /* 接收应答数据缓存     */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    sync_srv_type_t emRecvSrvType = SYNC_FILE;                      /* 接收到应答服务类型   */
    SyncTime64_t unCurTime;                                         /* 当前同步时间         */

    switch(emLastStatus)
    {/* 根据服务设置同步接收 */
        case TASK_FILE:
            emRecvSrvType = SYNC_FILE;
            break;
        case TASK_G_DATA:
            emRecvSrvType = SYNC_REAL_DATA;
            break;
        case TASK_I_DATA:
            emRecvSrvType = SYNC_REAL_DATA;
            break;
        case TASK_Q_DATA:
            emRecvSrvType = SYNC_REAL_DATA;
            break;
        case TASK_SYNC_OK:
            emRecvSrvType = SYNC_COMPLETE;
            break;
        case TASK_COMPLETE:
//            emRecvSrvType = SYNC_COMPLETE;
            break;
        default:
            break;
    }
    /* 判断接收应答数据 */
    if(emSrcPMId == emNextPMId)
    {
        sRetLen = s_stSyncData.sLinkAPrjLen;
        pstSyncAck = (SyncStaticAck_t*)&s_stSyncData.stSyncRecvMSGA.stSyncPrjData.ucPrjData[0];
    }
    if(emSrcPMId == emPrePMId)
    {
        sRetLen = s_stSyncData.sLinkBPrjLen;
        pstSyncAck = (SyncStaticAck_t*)&s_stSyncData.stSyncRecvMSGB.stSyncPrjData.ucPrjData[0];
    }

    if(SYNC_RECV_SIZE_0 == sRetLen)
    {/* 接收到数据长度为0或接收到错误数据 */
        SysGetSyncTime(&unCurTime);

        if(SysDiffSyncTime(&s_stSyncTimer.unTaskStartTime, &unCurTime) >= SYNC_TIMER_TIMEOUT)
        {/* 等待应答超时或接收应答错误 */

            DEBUG_SYNC("R ACK TOUT Start %d  Cur %d\n", \
                    s_stSyncTimer.unTaskStartTime.stTimeStruct.uiLow32b,unCurTime.stTimeStruct.uiLow32b);
            
            if(TASK_FILE == emLastStatus)
            {
                SyncInitFileStatus();
                SysSyncPrjQueueUpdate(emLastStatus);
            }
            else if((TASK_G_DATA == emLastStatus) || (TASK_I_DATA == emLastStatus)|| (TASK_Q_DATA == emLastStatus))
            {
                SyncInitDataStatus();
                SysSyncPrjQueueUpdate(emLastStatus);
            }
            else if(TASK_SYNC_OK == emLastStatus)
            {
                SysSyncPrjQueueUpdate(emLastStatus);
            }
            else
            {
                SysSyncPrjQueueUpdate(emLastStatus);
            }
        }
    }
    else
    {/* 接收到应答数据 */
        if(emLastStatus != TASK_COMPLETE)
        {
            if((emRecvSrvType != pstSyncAck->stSyncHead.emSrvType) || (pstSyncAck->ucData[0] != ACK_DATA_OK))
            {/* 接收到错误应答数据 */
                
                DEBUG_SYNC("RECV ACK ERR");
                printf("Type: %x RecvType: %x  Ack: %x\n", \
                        emRecvSrvType,pstSyncAck->stSyncHead.emSrvType,pstSyncAck->ucData[0]);
                
                if(TASK_FILE == emLastStatus)
                {/* 文件重传 */
                    SyncInitFileStatus();
                    SysSyncPrjQueueUpdate(emLastStatus);
                }
                else if((TASK_G_DATA == emLastStatus) || (TASK_I_DATA == emLastStatus)|| (TASK_Q_DATA == emLastStatus))
                {/* 数据重传 */
                    SyncInitDataStatus();
                    SysSyncPrjQueueUpdate(emLastStatus);
                }
                else if(TASK_SYNC_OK == emLastStatus)
                {
                    SysSyncPrjQueueUpdate(emLastStatus);
                }
                else
                {
                    SyncInitStatus();
                    SysSyncPrjQueueUpdate(TASK_FILE);
                }
            }
            else
            {/* 接收到正确应答数据 */
                DEBUG_SYNC("R ACK OK %d\n",pstSyncAck->stSyncHead.emSrvType);
                if((TASK_FILE == emLastStatus) && (NEW_FILE == s_stSyncFileStatus.emStartFlag))
                {/* 发送新文件 */
                    if(HAS_NONEXT_FILE == SyncGetNextFile(&s_stSyncFileStatus))
                    {/* 文件发送完成，更新同步任务 */
                        SysSyncPrjQueueUpdate(TASK_COMPLETE);
                        memset(&s_stSyncFileStatus, 0x00,sizeof(SyncFileStatus_t));

                        s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
                        DEBUG_SYNC("File OK!\n");
                    }
                    else
                    {
                        SysSyncPrjQueueUpdate(emLastStatus);
                    }
                    /* 新文件已发送数据长度为0 */
                    s_stSyncFileStatus.uiSendSize = 0U;
                }
                else if(TASK_SYNC_OK == emLastStatus)
                {
                    SyncInitDataStatus();
                    
                    SysSyncPrjQueueUpdate(TASK_COMPLETE);
                }
                else
                {
                    SysSyncPrjQueueUpdate(emLastStatus);
                }
            }
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvFileData
*
* 功能描述: 接收文件数据，并存储到内存
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
void SyncRecvFileData(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    static uint32_t s_uiCacheSize = 0U;                             /* 当前缓存大小*/
    uint32_t uiRecvAck = ACK_DATA_ERR;                              /* 发送应答结果 */
    int32_t iSendLen = 0;                                           /* 发送数据长度 */
    static uint32_t uiDataLen = 0U;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号 */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号 */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号 */
    PMController_t emSrcPMId = INVALID_PM_ID;
    SyncFile_t* pstSyncRecvFile = NULL;
    SysSyncTask_t stSyncTask;                                       /* 同步任务                 */
    SysCfgFileState_t stCfgFileState;
    static SyncPrjData_t* pstSyncPrjData = NULL;
    SyncPrjData_t stSyncPrjDataSend;
    SyncStaticAck_t* pstSyncFileAck;                                /* 接收文件应答 */
    uint16_t usWritDataLen = 0U;
    static pack_type_t emLastPackFlag = NOT_LAST_PACK;               /* 最后一包标识          */
    static uint8_t ucData[CFG_MAX_SYNC_FILE_SIZE];                   /* 文件数据              */
    uint8_t ucRet = ERR_HANDLE_NOT_DONE;
    
    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    
    if(FILE_HANDLE_FREE == emFileHandleFlg)
    {
        emSrcPMId = stSyncTask.emPMId;
        if(emSrcPMId == emNextPMId)
        {
            pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncRecvMSGA.stSyncPrjData;
        }
        else if(emSrcPMId == emPrePMId)
        {
            pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncRecvMSGB.stSyncPrjData;
        }
        else
        {
            DEBUG_SYNC("R PM ERR %d\n",emSrcPMId);
        }
        if((pstSyncPrjData != NULL) && (pstSyncPrjData->uiSyncPrjDataLen > 0U))
        {
            pstSyncRecvFile = (SyncFile_t*)&pstSyncPrjData->ucPrjData[0];
            uiDataLen = pstSyncPrjData->uiSyncPrjDataLen - sizeof(SyncHead_t) - sizeof(SyncFileHead_t);
            if((0 == uiDataLen) || (uiDataLen > 4*1024))
            {
                printf("File Len Err %d\n",uiDataLen);
            }
            else
            {
                printf("FileOffset %d\n",pstSyncRecvFile->stFileHead.uiFileOffset);
                
                emLastPackFlag = pstSyncRecvFile->stFileHead.emLastPackFlag;
                memcpy(&ucData[0], &pstSyncRecvFile->ucData[0], uiDataLen);
                if((pstSyncRecvFile->stFileHead.emCfgFileType == s_stSyncFileStatus.emCfgFileType) && 
                        (pstSyncRecvFile->stFileHead.uiFileOffset != 0U))
                {/* 不是接收到新文件 */
                    /*将数据写入内存*/
                    SysGetCfgFileState(pstSyncRecvFile->stFileHead.emCfgFileType,&stCfgFileState);
                    usWritDataLen = SysSetCfgFileInfo(pstSyncRecvFile->stFileHead.emCfgFileType,&pstSyncRecvFile->ucData[0], \
                            uiDataLen);

                    if(usWritDataLen != uiDataLen)
                    {
                        DEBUG_SYNC("WR File ERR %d %d\n",usWritDataLen,uiDataLen);
                    }
                    else
                    {
                        s_uiCacheSize += uiDataLen;
                        
                        emFileHandleFlg = FILE_WRITE_CONTINUE;
    //                    printe("%d",pstSyncRecvFile->stFileHead.emCfgFileType);
                    }
                }
                else
                {/* 接收到新文件 */
                    /* 更新文件名记录 */
                    
                    s_stSyncFileStatus.emCfgFileType = pstSyncRecvFile->stFileHead.emCfgFileType;

                    DEBUG_SYNC("FileType %d\n",s_stSyncFileStatus.emCfgFileType);
        
                    SysGetCfgFileState(pstSyncRecvFile->stFileHead.emCfgFileType,&stCfgFileState);
                    
                    if((stCfgFileState.uiLen != 0U) || (stCfgFileState.uiWrOffset != 0U))
                    {/*若文件信息显示文件内容非空，将文件数据删除*/
                        SysDeleteCfgFile(pstSyncRecvFile->stFileHead.emCfgFileType);
                    }
                    /*将文件数据存入内存*/
                    usWritDataLen = SysSetCfgFileInfo(pstSyncRecvFile->stFileHead.emCfgFileType,&pstSyncRecvFile->ucData[0], \
                            uiDataLen);

                    s_uiCacheSize += uiDataLen;
                    if(usWritDataLen!= uiDataLen)
                    {
                        DEBUG_SYNC("WR File Err %d %d\n",usWritDataLen,uiDataLen);
                    }
                    else
                    {
                        emFileHandleFlg = FILE_WRITE_START;
                        
    //                    printe("%d",pstSyncRecvFile->stFileHead.emCfgFileType);
                    }
                }
            }
        }
    }
    else
    {
        switch(emFileHandleFlg)
        {
            case FILE_WRITE_START:
            {
                ucRet = SyncWriteFileStart(s_stSyncFileStatus.emCfgFileType, &ucData[0], uiDataLen);
                
                if(ERR_HANDLE_NOT_DONE == ucRet)
                {
                    emFileHandleFlg = FILE_WRITE_START;
                }
                else
                {
                    if(ERR_SUCCESS == ucRet)
                    {
                        if(LAST_PACK == emLastPackFlag)
                        {
                            emFileHandleFlg = FILE_WRITE_DONE;
                            
                            ucRet = ERR_HANDLE_NOT_DONE;
                        }
                        else
                        {
                            emFileHandleFlg = FILE_HANDLE_FREE;
                        }
                    }
                    else
                    {
                        emFileHandleFlg = FILE_HANDLE_FREE;
                    }
                }
            }
            break;
            case FILE_WRITE_CONTINUE:
            {
                ucRet = SyncWriteFileContinue(s_stSyncFileStatus.emCfgFileType,&ucData[0], uiDataLen);
                
                if(ERR_HANDLE_NOT_DONE == ucRet)
                {
                    emFileHandleFlg = FILE_WRITE_CONTINUE;
                }
                else
                {
                    if(ERR_SUCCESS == ucRet)
                    {
                        if(LAST_PACK == emLastPackFlag)
                        {
                            emFileHandleFlg = FILE_WRITE_DONE;
                            ucRet = ERR_HANDLE_NOT_DONE;
                        }
                        else
                        {
                            emFileHandleFlg = FILE_HANDLE_FREE;
                        }
                    }
                    else
                    {
                        emFileHandleFlg = FILE_HANDLE_FREE;
                    }
                }
            }
            
            break;
            case FILE_WRITE_DONE:
            {
                ucRet = SyncWriteFileEnd(s_stSyncFileStatus.emCfgFileType);
                
                if(ERR_HANDLE_NOT_DONE == ucRet)
                {
                    emFileHandleFlg = FILE_WRITE_DONE;
                }
                else
                {
                    emFileHandleFlg = FILE_HANDLE_FREE;
                    
                    if(ERR_SUCCESS == ucRet)
                    {/* 接收到文件最后一包数据 */
                        /*设置向内存写文件完成*/
                        SysSetCfgFileFinished(s_stSyncFileStatus.emCfgFileType);
                        printf("WR File OK %d\n",s_stSyncFileStatus.emCfgFileType);
                        s_uiCacheSize = 0U;
                        if(CFG_CTRL_STATION == s_stSyncFileStatus.emCfgFileType)
                        {
                            SysSetAllCfgFileFinished();
                            
                            SysSyncPrjQueueUpdate(TASK_COMPLETE);
                            
                            memset(&s_stSyncFileStatus, 0x00,sizeof(SyncFileStatus_t));
                            
                            s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
                        }
                    }
                }
            }
            
            break;
            
            default:
            {
                emFileHandleFlg = FILE_HANDLE_FREE;
            }
            
            break;
        }
        
        if(ucRet != ERR_HANDLE_NOT_DONE)
        {
            if(ERR_SUCCESS == ucRet)
            {
                /* 应答OK */
                uiRecvAck = ACK_DATA_OK;
                DEBUG_SYNC("ACK_OK\n");
            }
            else
            {
                /* 应答OK */
                uiRecvAck = ACK_DATA_ERR;
                DEBUG_SYNC("ACK_ERR %d\n",ucRet);
            }
            
            memset(&stSyncPrjDataSend, 0x00, sizeof(SyncPrjData_t));
            pstSyncFileAck = (SyncStaticAck_t*)&stSyncPrjDataSend.ucPrjData[0];
            
            emSrcPMId = stSyncTask.emPMId;
    
            /* 打包应答数据 */
            pstSyncFileAck->stSyncHead.emSrvType = SYNC_FILE;
            pstSyncFileAck->ucData[0] = uiRecvAck;
            /* 计算发送数据长度 */
            iSendLen = sizeof(pstSyncFileAck->stSyncHead) + sizeof(pstSyncFileAck->ucData[0]);
            stSyncPrjDataSend.uiSyncPrjDataLen = iSendLen;
            
            iSendLen += sizeof(stSyncPrjDataSend.uiSyncPrjDataLen);
    
            if(emNextPMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
            }
            if(emPrePMId == emSrcPMId)
            {
                memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData,(puint8_t)&stSyncPrjDataSend,(uint32_t)iSendLen);
            }
            if(pstSyncPrjData != NULL )
            {
                pstSyncPrjData->uiSyncPrjDataLen = 0U;
            }
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendFileData
*
* 功能描述: 发送文件数据
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
void SyncSendFileData(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    SysSyncTask_t stSyncTask;                                   /* 同步任务                 */
    static sync_task_stat_t s_emLastStatus = TASK_WAITACK;            /* 等待应答前状态   */
    static sync_task_stat_t ucDebugLastStatus = 0U;

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

        /* 根据任务状态发送相关数据 */
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_FILE:
                /* 发送工程文件数据 */
                if(ucDebugLastStatus != stSyncTask.emTaskStatus)
                {
                    DEBUG_SYNC("SEND_FILE\n");
                }
                s_emLastStatus = stSyncTask.emTaskStatus;
                SyncSendFile(pstSyncPrjQueue);
                break;
            case TASK_WAITACK:
                /* 等待应答 */
                if(ucDebugLastStatus != stSyncTask.emTaskStatus)
                {
                    DEBUG_SYNC("WAITACK\n");
                }
                
                SyncWaitAck(stSyncTask.emPMId, s_emLastStatus);
                break;
            case TASK_COMPLETE:
                /* 等待应答 */
                if(ucDebugLastStatus != stSyncTask.emTaskStatus)
                {
                    DEBUG_SYNC("SEND_FILE OK\n");
                }
                
                break;
            default:
                break;
        }
        
        ucDebugLastStatus = stSyncTask.emTaskStatus;

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncSendFile
*
* 功能描述: 发送文件数据
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
void SyncSendFile(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    SyncPrjData_t stSyncPrjData;
    SyncFile_t* pstSyncSendFile = NULL;                             /* 发送文件数据缓存     */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号 */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号 */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号 */
    PMController_t emDstPMId = INVALID_PM_ID;
    uint16_t usRealFileLen = 0U;                                    /* 文件读取长度         */
    uint16_t usReadFileLen = 0U;
    int32_t iSendLen = 0U;                                          /* 发送数据长度         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务         */
    SysCfgFileState_t stCfgFileState;
    
    pstSyncSendFile = (SyncFile_t*)&stSyncPrjData.ucPrjData[0];
    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

    emDstPMId = stSyncTask.emPMId;
    do
    {
        
        if(NEW_FILE == s_stSyncFileStatus.emStartFlag)
        {/* 发送新文件 */
            if(s_stSyncFileStatus.emCfgFileType >= INVALID_CFG_FILE_TYPE)
            {
                s_stSyncFileStatus.emCfgFileType = CFG_PRG;
                /* 新文件已发送数据长度为0 */
                s_stSyncFileStatus.uiSendSize = 0U;
            }
        }
        
        DEBUG_SYNC("FileType %d\n",s_stSyncFileStatus.emCfgFileType);
        
        /*获取文件操作状态*/
        SysGetCfgFileState(s_stSyncFileStatus.emCfgFileType, &stCfgFileState);

        if(stCfgFileState.bReadable != CFG_FILE_RD_ENABLE)
        {/* 文件不可读 */
            /*log*/
            s_stSyncFileStatus.emStartFlag = NOT_NEW_FILE;
            
            DEBUG_SYNC("RD_DISABLE %d\n",stCfgFileState.bReadable);
            
            break;
        }
        /* 填充发送同步数据类型为文件 */
        pstSyncSendFile->stSyncHead.emSrvType = SYNC_FILE;
        iSendLen += sizeof(SyncHead_t);

        /* 填充文件数据头 */
        pstSyncSendFile->stFileHead.uiFileOffset = s_stSyncFileStatus.uiSendSize;
        printf("FileOff %d\n",pstSyncSendFile->stFileHead.uiFileOffset);
        pstSyncSendFile->stFileHead.emCfgFileType = s_stSyncFileStatus.emCfgFileType;
        iSendLen += sizeof(SyncFileHead_t);

        usReadFileLen = MIN(CFG_MAX_SYNC_FILE_SIZE, stCfgFileState.uiLen - s_stSyncFileStatus.uiSendSize);

        usRealFileLen = SysGetCfgFileInfo(pstSyncSendFile->stFileHead.emCfgFileType, \
                pstSyncSendFile->stFileHead.uiFileOffset, &pstSyncSendFile->ucData[0], usReadFileLen);
        
        if(usRealFileLen != usReadFileLen)
        {
            DEBUG_SYNC("RD FILE ERR !\n");
        }
        else
        {
            printf("SD F Len %d!\n",usRealFileLen);
        }

        iSendLen += usRealFileLen;
        /* 更新已发送文件位置 */
        s_stSyncFileStatus.uiSendSize += usRealFileLen;
        /* 获取文件大小 */
        s_stSyncFileStatus.uiFileSize = stCfgFileState.uiLen;
        /* 判断是否已发送完成 */
        if(((usRealFileLen > 0U) && (usRealFileLen < CFG_MAX_SYNC_FILE_SIZE)) ||
           (s_stSyncFileStatus.uiSendSize == s_stSyncFileStatus.uiFileSize))
        {/* 当前文件发送完成 */
            /*log*/
            pstSyncSendFile->stFileHead.emLastPackFlag = LAST_PACK;
            s_stSyncFileStatus.emStartFlag = NEW_FILE;
        }
        else
        {/* 当前文件发送没有完成 */
            s_stSyncFileStatus.emStartFlag = NOT_NEW_FILE;
            pstSyncSendFile->stFileHead.emLastPackFlag = NOT_LAST_PACK;
        }
        
        stSyncPrjData.uiSyncPrjDataLen = iSendLen;
        
        iSendLen += sizeof(stSyncPrjData.uiSyncPrjDataLen);
        
//        printe("%d",s_stSyncFileStatus.emCfgFileType);

        if(emNextPMId == emDstPMId)
        {
            memcpy((puint8_t)&s_stSyncData.stSyncSendMSGA.stSyncPrjData,(puint8_t)&stSyncPrjData,(uint32_t)iSendLen);
            
            SysSyncPrjQueueUpdate(TASK_WAITACK);
        }
        else if(emPrePMId == emDstPMId)
        {
            memcpy((puint8_t)&s_stSyncData.stSyncSendMSGB.stSyncPrjData,(puint8_t)&stSyncPrjData,(uint32_t)iSendLen);
            
            SysSyncPrjQueueUpdate(TASK_WAITACK);
        }
        else
        {/* PMID错误，出队 */
            /*log*/
            SysDeSyncPrjQueue(pstSyncPrjQueue);
        }
    }while(NULL);

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncGetNextFile
*
* 功能描述: 获取下一个发送的文件信息
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
uint32_t SyncGetNextFile(SyncFileStatus_t* pstSyncFileStatus)
{
    uint32_t uiRet = HAS_NONEXT_DATA;                                 /* 函数返回值           */

    switch(pstSyncFileStatus->emCfgFileType)
    {
        case INVALID_CFG_FILE_TYPE:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_PRG;
            break;
        case CFG_PRG:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_CM;
            break;
        case CFG_CM:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_IO_MODULAR;
            break;
        case CFG_IO_MODULAR:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_SOE;
            break;
        case CFG_SOE:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_P2P;
            break;
        case CFG_P2P:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_MODBUS_MASTER;
            break;
        case CFG_MODBUS_MASTER:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_MODBUS_SLAVE;
            break;
        case CFG_MODBUS_SLAVE:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_VOTE;
            break;
        case CFG_VOTE:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_RETAIN;
            break;
        case CFG_RETAIN:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_OPC;
            break;
        case CFG_OPC:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_CTRL_STATION;
            break;
        case CFG_CTRL_STATION:
            uiRet = HAS_NONEXT_DATA;
            pstSyncFileStatus->emCfgFileType = INVALID_CFG_FILE_TYPE;
            break;
        default:
            break;
    }

    return uiRet;
}

static uint8_t SyncWriteFileStart(SysCfgFileType_t emCfgFileType, uint8_t ucFileData[],uint32_t uiFileDataLen)
{
    static FileWriteStag_t emProcessStag = FWRITE_START;
    static uint8_t ucSubSeason = ERR_SUCCESS;
    uint8_t ucRet = ERR_HANDLE_NOT_DONE;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;

    if(NULL == ucFileData)
    {
        ucSubSeason = ERR_WRITE_FILE_START;
        emProcessStag = FWRITE_END;
    }
    switch(emProcessStag)
    {
        case FWRITE_START:
        {
            //save the data in memory
            DEBUG_SYNC("WRFileStart\n");
            /*根据文件类型，获取文件的控制结构*/
            pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
            if(pstFileInfo == NULL)
            {
                DEBUG_SYNC("FileInfo NULL\n");
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
            else
            {
                emProcessStag = FWRITE_DETER_TMPFILE_EXIST;
            }
        }
        break;

        case FWRITE_DETER_TMPFILE_EXIST:
        {/*完成判断文件是否存在操作*/
            ucFileOptRst = CommDetermineFileExist(&pstFileInfo->ucTmpFileName[0]);
            if(FILE_EXIST == ucFileOptRst)
            {/*临时文件存在，将其删除*/
                DEBUG_SYNC("EXIST %s\n",&pstFileInfo->ucTmpFileName[0]);
                emProcessStag = FWRITE_ERASE_TMPFILE;
            }
            if(FILE_NOT_EXIST == ucFileOptRst)
            {/*临时文件不存在，将其以创建方式打开*/
                DEBUG_SYNC("NOT_EXIST %s\n",&pstFileInfo->ucTmpFileName[0]);
                emProcessStag = FWRITE_OPEN_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败*/
                DEBUG_SYNC("DETER_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_ERASE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_OPEN_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，并应答错误信息*/
                
                DEBUG_SYNC("ERASE_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_OPEN_TMPFILE:
        {
            ucFileOptRst = CommOpenFile(&pstFileInfo->ucTmpFileName[0], &pstFileInfo->iFid);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_WRITE_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                
                DEBUG_SYNC("OPEN_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_WRITE_TMPFILE:
        {
            if(pstFileInfo->iFid != -1)
            {
                ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucFileData[0], uiFileDataLen);
                
//                DEBUG_SYNC("Fid: %x %s Len: %d\n",pstFileInfo->iFid,&pstFileInfo->ucTmpFileName[0],uiFileDataLen);
                
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/

                    ucSubSeason = ERR_SUCCESS;
                    emProcessStag = FWRITE_END;
                }
                if(FAIL == ucFileOptRst)
                {/*操作失败，关闭文件，等待删除，并应答错误信息*/
                    
                    DEBUG_SYNC("WR_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                    
                    ucSubSeason = ERR_WRITE_FILE_START;
                    emProcessStag = FWRITE_CLOSE_TMPFILE;
                }
            }
            else
            {
                DEBUG_SYNC("Fid ERR %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
                
                emProcessStag = FWRITE_CLOSE_TMPFILE;
            }
        }
        break;

        case FWRITE_CLOSE_TMPFILE:
        {
            ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
            if(FAIL == ucFileOptRst)
            {/*操作失败，删除文件*/
                /*record error log*/
                
                DEBUG_SYNC("CLOSE_TMP FAIL %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
                
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功*/
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
            
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                /*record error log*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("DELET_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }

        }
        break;

        case FWRITE_END:
        {
            /*produce ACK data*/
            emProcessStag = FWRITE_START;
            
            ucRet = ucSubSeason;
        }
        break;

        default:
        {
            //record error log
            ucSubSeason = ERR_WRITE_FILE_START;
            emProcessStag = FWRITE_END;
        }
        break;
    }

    return ucRet;
}

static uint8_t SyncWriteFileContinue(SysCfgFileType_t emCfgFileType, uint8_t ucFileData[],uint32_t uiFileDataLen)
{
    static FileWriteStag_t emProcessStag = FWRITE_START;
    static uint8_t ucSubSeason = ERR_SUCCESS;
    uint8_t ucRet = ERR_HANDLE_NOT_DONE;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;

    if(NULL == ucFileData)
    {
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_END;
    }
    switch(emProcessStag)
    {/**/
        case FWRITE_START:
        {
            //save the data in memory
            /*根据文件类型，获取文件的控制结构*/
            DEBUG_SYNC("WRFileCont\n");
            pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
            if(pstFileInfo == NULL)
            {
                DEBUG_SYNC("FileInfo NULL\n");
                ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                emProcessStag = FWRITE_END;
            }
            else
            {
                emProcessStag = FWRITE_WRITE_TMPFILE;
            }
        }
        break;

        case  FWRITE_WRITE_TMPFILE:
        {
            if(pstFileInfo->iFid != -1)
            {
//                DEBUG_SYNC("Fid %x %s Len %d\n",pstFileInfo->iFid,&pstFileInfo->ucTmpFileName[0],uiFileDataLen);
                
                ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucFileData[0], uiFileDataLen);
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/
                    emProcessStag = FWRITE_END;
                    ucSubSeason = ERR_SUCCESS;
                }
                if(FAIL == ucFileOptRst)
                {/*操作失败，关闭文件，等待删除，并应答错误信息*/
                    emProcessStag = FWRITE_CLOSE_TMPFILE;
                    
                    DEBUG_SYNC("WR_TMP FAIL %s \n",&pstFileInfo->ucTmpFileName[0]);
                }
            }
            else
            {
                ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("Fid ERR  %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
            }
        }
        break;
        
        case FWRITE_CLOSE_TMPFILE:
        {
            ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
            if(FAIL == ucFileOptRst)
            {/*操作失败，删除文件*/
                /*record error log*/
                
                DEBUG_SYNC("CLOSE_TMP FAIL  %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
                
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功*/
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
            
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                /*record error log*/
                ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("DELET_TMP FAIL  %s\n",&pstFileInfo->ucTmpFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功*/
                ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                emProcessStag = FWRITE_END;
            }

        }
        break;

        case FWRITE_END:
        {
            emProcessStag = FWRITE_START;
            
            ucRet = ucSubSeason;
        }
        break;

        default:
        {/*其他异常情况，错误应答*/
            /*record error log*/
            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
            emProcessStag = FWRITE_END;
        }
        break;
    }
    
    return ucRet;
}

static uint8_t SyncWriteFileEnd(SysCfgFileType_t emCfgFileType)
{
    static FileWriteStag_t emProcessStag = FWRITE_START;
    static uint8_t ucSubSeason = ERR_SUCCESS;
    uint8_t ucRet = ERR_HANDLE_NOT_DONE;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;

    switch(emProcessStag)
    {
        case FWRITE_START:
        {
            DEBUG_SYNC("WRFileEnd\n");
            /*根据文件类型，获取文件的控制结构*/
            pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
            if(pstFileInfo == NULL)
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("FileInfo NULL\n");
                
            }
            else
            {
                emProcessStag = FWRITE_WRITE_TMPFILEDONE;
            }
        }
        break;

        case  FWRITE_WRITE_TMPFILEDONE:
        {
            if(pstFileInfo->iFid != -1)
            {
                ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
                if(SUCCESS == ucFileOptRst)
                {/*操作成功，进行下一步操作*/
                    emProcessStag = FWRITE_DETER_FORMFILE_EXIST;
                }
                if(FAIL == ucFileOptRst)
                {/*操作失败，删除文件*/
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                    
                    DEBUG_SYNC("WR_TMP_DONE FAIL %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
                    
                }
            }
            else
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("Fid ERR %s Fid %d\n",&pstFileInfo->ucTmpFileName[0],pstFileInfo->iFid);
            }
        }
        break;

        case FWRITE_DETER_FORMFILE_EXIST:
        {
            ucFileOptRst = CommDetermineFileExist(&pstFileInfo->ucFileName[0]);
            if(FILE_EXIST == ucFileOptRst)
            {/*文件存在，重命名文件*/
                emProcessStag = FWRITE_RENAME_FORMFILE;
                
                DEBUG_SYNC("EXIST %s\n",&pstFileInfo->ucFileName[0]);
                
            }
            if(FILE_NOT_EXIST == ucFileOptRst)
            {/*文件不存在，重命名临时文件*/
                emProcessStag = FWRITE_RENAME_TMPFILE;
                
                DEBUG_SYNC("NOT_EXIST %s\n",&pstFileInfo->ucFileName[0]);
                
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败,应答错误信息*/
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("DETER_FORM FAIL %s\n",&pstFileInfo->ucFileName[0]);
                
            }
        }
        break;
        
        case FWRITE_RENAME_FORMFILE://FWRITE_DELETE_FORMFILE
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucFileName[0]);
            if(FAIL == ucFileOptRst)
            {/*重命名正式文件为旧文件失败，删除文件*/
                /*record error log*/
                emProcessStag = FWRITE_DELETE_FORMFILE;
                
                DEBUG_SYNC("DELET_FORM FAIL  %s\n",&pstFileInfo->ucFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {/*重命名正式文件为旧文件成功，将临时文件重命名为正式文件*/
                emProcessStag = FWRITE_RENAME_TMPFILE;
            }
        }
        break;

        case FWRITE_RENAME_TMPFILE:
        {
            ucFileOptRst = CommRenameFile(&pstFileInfo->ucTmpFileName[0], &pstFileInfo->ucFileName[0]);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
                emProcessStag = FWRITE_DELETE_TMPFILE;
                
                DEBUG_SYNC("RENAME FAIL  %s  %s\n",&pstFileInfo->ucTmpFileName[0],&pstFileInfo->ucFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {
                ucSubSeason = ERR_SUCCESS;
                pstFileInfo->uiFileDataLen = pstFileInfo->uiFileWriteIndex;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("RENAME OK %s %s\n",&pstFileInfo->ucTmpFileName[0],&pstFileInfo->ucFileName[0]);
                
            }
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("DELET_TMP FAIL %s\n",&pstFileInfo->ucTmpFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
            }

        }
        break;

        case FWRITE_DELETE_FORMFILE:
        {
            ucFileOptRst = CommDeleteFile(&pstFileInfo->ucFileName[0]);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_SYNC("DELET_FORM FAIL  %s\n",&pstFileInfo->ucFileName[0]);
                
            }
            if(SUCCESS == ucFileOptRst)
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
            }

        }
        break;

        case FWRITE_END:
        {
            emProcessStag = FWRITE_START;
            
            ucRet = ucSubSeason;
        }
        break;

        default:
        {/*其他异常情况，错误应答*/
            /*record error log*/
            ucSubSeason = ERR_WRITE_FILE_END;
            emProcessStag = FWRITE_END;
        }
        break;

    }

    return ucRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncGetAjustTimerFlag
*
* 功能描述: 获取是否可进行校时标识。
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
uint32_t SyncGetAjustTimerFlag(void)
{
    uint32_t uiRet = SYS_AJUST_SYNCTIME_NOREADY;
    SyncTime64_t unSyncTime;
    static uint32_t s_uiAjustStart = SYS_AJUST_DELAY_START;
    static SyncTime64_t s_unStartAjustTime;

    SysGetSyncTime(&unSyncTime);
    if(SYS_AJUST_DELAY_START == s_uiAjustStart)
    {
        s_unStartAjustTime.ullTime = unSyncTime.ullTime;
        s_uiAjustStart = SYS_AJUST_DELAYING;
    }
    else if(SYS_AJUST_DELAYING == s_uiAjustStart)
    {
        if(SysDiffSyncTime(&s_unStartAjustTime, &unSyncTime) >= CFG_ALLOW_AJUST_DELAY)
        {
            s_uiAjustStart = SYS_AJUST_DELAY_END;
        }
    }
    else
    {
        /* the timer is ready to ajust or not */
        if((unSyncTime.stTimeStruct.uiLow32b / CFG_ALLOW_AJUST_UNIT) % 10U < CFG_ALLOW_AJUST_COUNT)
        {
            uiRet = SYS_AJUST_SYNCTIME_READY;
            s_uiAjustStart = SYS_AJUST_DELAY_START;
        }
    }

    return uiRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
