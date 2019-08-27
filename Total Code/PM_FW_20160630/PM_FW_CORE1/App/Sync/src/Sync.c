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

void SyncHandleSrvData(SyncData_t* pstSyncData, puint32_t puiSrvAckHandleFlag);

uint32_t SyncHandleCtrlData(const SysPMStateInfo_t* pstOneStatData, PMController_t emSrcPMId,int16_t sDataLen);

void SyncCtrlInfoHandle(void);

void SyncCtrlPWHandle(void);

uint32_t SyncHandleCS1131SrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleOPCSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleP2PSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleModbusSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

uint32_t SyncHandleCtrlSrvData(const SrvData_t* pstSrvData, PMController_t emSrcPMId, uint32_t uiDataLen);

void SyncBroadCastCS1131Srv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastOPCSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastModbusSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastP2PSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag);

void SyncBroadCastCtrlData(ack_flag_t emAckFlag);

void SyncBroadCastActive(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                         uint32_t uiSrvAckHandleFlag, ack_flag_t emAckFlag);


void SyncBroadCastCancel(uint32_t uiDstRecord);

void SyncRecvRealData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncRecvData(SyncRealData_t* pstSyncPrjData, uint32_t uiDataLen, PMController_t emSrcPMId);

void SyncRecvFileOk(PMController_t emSrcPMId);

void SyncRecvRealDataOk(PMController_t emSrcPMId);

void SyncSendRealData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendData(PMController_t emDstPMId);

void SyncSendDataOk(PMController_t emDstPMId);

uint32_t SyncGetNextArea(SysRtDataType_t* pemCurAreaId);

void SyncWaitAck(PMController_t emSrcPMId, sync_task_stat_t emLastStatus);

void SyncRecvFileData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendFileData(SysSyncPrjQueue_t* pstSyncPrjQueue);

void SyncSendFile(SysSyncPrjQueue_t* pstSyncPrjQueue);

uint32_t SyncGetNextFile(SyncFileStatus_t* pstSyncFileStatus);


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
    
    memset((puint8_t)&s_stSyncFileStatus, 0U, sizeof(SyncFileStatus_t));
    
    s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
    
    memset((puint8_t)&s_stSyncDataStatus, 0U, sizeof(SyncDataStatus_t));
    
    memset((puint8_t)&s_stSyncTimer, 0U, sizeof(SyncTimerStatus_t));
    
    memset((puint8_t)&s_stSyncData, 0U, sizeof(SyncData_t));
    
    memset((puint8_t)&emFileHandleFlg, 0U, sizeof(File_Handle_Flag_t));

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
    //SyncData_t stSyncData;                                        /* 同步数据             */
    uint32_t uiSrvAckHandleFlag = SYNC_SRVACK_NOHANDLE;             /* 接收到应答数据标识   */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();      /* 同步任务队列         */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();      /* 同步工程队列         */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号       */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PM ID号     */
    static uint32_t s_uiWaitCount = 0U;
    
//    printf("  SyncMngt  \n");
    

    if(FILE_HANDLE_FREE == emFileHandleFlg)
    {/*当一包工程文件同步数据未处理完时，不再收发同步数据*/
        /* 接收来自其它PM的状态数据 */
        //memset((puint8_t)&s_stSyncData, 0U, sizeof(SyncData_t));
        SyncRecvSyncData(&s_stSyncData);
        /* 处理接收到错误状态数据 */
        if((s_stSyncData.sLinkASrvErr != RET_OK) || (s_stSyncData.sLinkAPrjErr != RET_OK) || \
           (s_stSyncData.sLinkBSrvErr != RET_OK) || (s_stSyncData.sLinkBPrjErr != RET_OK))
        {
            if((s_stSyncData.sLinkASrvErr != RET_OK) || (s_stSyncData.sLinkAPrjErr != RET_OK))
            {
                /*log LinkA接收错误*/
            }

            if((s_stSyncData.sLinkBSrvErr != RET_OK) || (s_stSyncData.sLinkBPrjErr != RET_OK))
            {
                /*log LinkB接收错误*/
            }

        }
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
            
//            DEBUG_SYNC("  SyncSrvQueue is not empty; TaskId is : %x  TaskWait is : %x\n",\
//                    stSyncTask.emTaskId,stSyncTask.emTaskWait);
            

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
                    case SYS_BROADCAST_P2PSRV:
                        /* 广播P2P服务 */
                        SyncBroadCastP2PSrv(pstSyncSrvQueue, &s_stSyncData, \
                                            uiSrvAckHandleFlag, stSyncTask.emCMId, NEED_ACK);
                        break;
                    case SYS_BROADCAST_CTRLDATA:
                        /* 广播PM信息 */
                        SyncBroadCastCtrlData(NO_ACK);
                        break;
                    case SYS_REQUEST_ACTIVE:
                        /* 请求激活服务 */
//                        if(SYNC_TIME_AJUST_OK == SysGetAjustTimerStatus())
//                        {
//                            SyncBroadCastActive(pstSyncSrvQueue, &s_stSyncData, \
//                                            uiSrvAckHandleFlag, NEED_ACK);
//                        }
                        SyncBroadCastActive(pstSyncSrvQueue, &s_stSyncData, \
                                        uiSrvAckHandleFlag, NEED_ACK);
                        break;
                    case SYS_PAUSE:

                    case SYS_CONTINUE:
                        /* 同步运行 */

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
            //SyncInitStatus();
            SyncInitTimerStatus();
        }
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
                    SyncRecvRealData(pstSyncPrjQueue);
                    break;
                case SYS_SEND_REALDATA:
                    /* 发送实时数据 */
//                    if(AJUST_HAS== SysGetSyncAjust())
//                    {
//                        SyncSendRealData(pstSyncPrjQueue);
//                    }
                    
                    
                    
                    SyncSendRealData(pstSyncPrjQueue);
                    
                    break;
                case SYS_RECV_FILE:
                    /* 接收文件数据 */
                    
                    DEBUG_SYNC("  RECV_FILE \n");
                    
                    SyncRecvFileData(pstSyncPrjQueue);
                    break;
                case SYS_SEND_FILE:
                    /* 发送文件数据 */
//                    if(AJUST_HAS== SysGetSyncAjust())
//                    {
//                        SyncSendFileData(pstSyncPrjQueue);
//                    }
                    
                    //DEBUG_SYNC("  SYS_SEND_FILE \n");
                    
                    SyncSendFileData(pstSyncPrjQueue);
                    
                    
                    break;
                default:
                    /* 错误任务ID号，出队 */
                    SysDeSyncPrjQueue(pstSyncPrjQueue);
                    
                    DEBUG_SYNC(" SYNC TaskId is ERR \n");
                    
                    break;
            }
        }
        else
        {/* 同步工程需要等待 */
            s_uiWaitCount++;
            DEBUG_SYNC("  TASK_WAIT \n");
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
        
//        static uint8_t a = 0;
//        if( 100 == a)
//        {
//            DEBUG_SYNC("  PRJ SYNC IS EMPTY \n");
//            a = 0U;
//        }
//        a++;
        
    }

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
    s_stSyncFileStatus.uiFileSize = 0U;
    //s_stSyncFileStatus.uiFnode = FS_MAX_FNODE_NUM;
    s_stSyncFileStatus.uiSendSize = 0U;
    s_stSyncFileStatus.emStartFlag = NEW_FILE;
    /* 清空文件名记录 */
    memset(s_stSyncFileStatus.ucFileName, 0U, MAX_FILE_NAME_LEN);

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
    PMController_t emLocalPMId = SysGetLocalPMID();                      /* 本地 PM ID号       */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);             /* 下一个 PM ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);               /* 前一个 PM ID号     */
    uint8_t ucRetErrNo = RET_OK;                                         /* 接收协议错误码       */
    int16_t sRetLen = SYNC_RECV_SIZE_0;                                /* 接收数据长度         */

    if(pstSyncData != NULL)
    {/* 校验接收缓存指针不为空 */
        
        /* 接收链路A即下一个 PM 发送的服务数据 */
        sRetLen = PMBUS_Receive(PM_BUS_A, PM_RECV_BUFF_SRV, (puint8_t)&pstSyncData->stSyncMSGA.stSyncSrvData);
        
        if(sRetLen <= SYNC_RECV_SIZE_0)
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkASrvLen = 0;
            pstSyncData->sLinkASrvErr = RET_OK;
            
//            DEBUG_SYNC("  RECV NEXT PM SRV DATA 0 ! \n");
            
        }
        else
        {/* 接收到来自后一个 PM 的状态数据 */
            pstSyncData->sLinkASrvLen = sRetLen;
            pstSyncData->sLinkASrvErr = RET_OK;
            
//            DEBUG_SYNC("  RECV NEXT PM SRV DATA !  Len is : %d \n",pstSyncData->sLinkASrvLen);
        }
        
        /* 接收链路A即下一个 PM 发送的工程数据 */
        sRetLen = PMBUS_Receive(PM_BUS_A, PM_RECV_BUFF_PRJ, (puint8_t)&pstSyncData->stSyncMSGA.stSyncPrjData);
        
        if(sRetLen <= SYNC_RECV_SIZE_0)
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkAPrjLen = 0;
            pstSyncData->sLinkAPrjErr = RET_OK;
            
//            DEBUG_SYNC("  RECV NEXT PM PRJ DATA 0 ! \n");
            
        }
        else
        {/* 接收到来自后一个 PM 的状态数据 */
            pstSyncData->sLinkAPrjLen = sRetLen;
            pstSyncData->sLinkAPrjErr = RET_OK;
            
//            DEBUG_SYNC("  RECV NEXT PM PRJ DATA ! Len is : %d \n",pstSyncData->sLinkAPrjLen);
            
        }
        
        
        
        /* 接收链路B即下一个 PM 发送的服务数据 */
        sRetLen = PMBUS_Receive(PM_BUS_B, PM_RECV_BUFF_SRV, (puint8_t)&pstSyncData->stSyncMSGB.stSyncSrvData);
        
        if(sRetLen <= SYNC_RECV_SIZE_0)
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkBSrvLen = 0;
            pstSyncData->sLinkBSrvErr = RET_OK;
            
//            DEBUG_SYNC("  RECV PRE PM SRV DATA 0 ! \n");
            
        }
        else
        {/* 接收到来自后一个 PM 的状态数据 */
            pstSyncData->sLinkBSrvLen = sRetLen;
            pstSyncData->sLinkBSrvErr = RET_OK;
            
//            DEBUG_SYNC("  RECV PRE PM SRV DATA ! Len is: %d \n",pstSyncData->sLinkBSrvLen);
            
        }
        
        /* 接收链路B即下一个 PM 发送的工程数据 */
        sRetLen = PMBUS_Receive(PM_BUS_B, PM_RECV_BUFF_PRJ, (puint8_t)&pstSyncData->stSyncMSGB.stSyncPrjData);
        
        if(sRetLen <= SYNC_RECV_SIZE_0)
        {/* 接收长度小于零，接收数据错误 */
            pstSyncData->sLinkBPrjLen = 0;
            pstSyncData->sLinkBPrjErr = RET_OK;
            
//            DEBUG_SYNC("  RECV PRE PM PRJ DATA 0 ! \n");
            
        }
        else
        {/* 接收到来自后一个 PM 的状态数据 */
            pstSyncData->sLinkBPrjLen = sRetLen;
            pstSyncData->sLinkBPrjErr = RET_OK;
            
//            DEBUG_SYNC("  RECV PRE PM PRJ DATA ! Len is: %d \n",pstSyncData->sLinkBPrjLen);
            
        }
    }

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

    /* 设置未收到应答数据 */
    *puiSrvAckHandleFlag = SYNC_SRVACK_NOHANDLE;

    if((pstSyncData->sLinkASrvLen > SYNC_RECV_SIZE_0))
    {/* 处理链接A接收到数据，即后一个PM 发送的数据 */
        
//        DEBUG_SYNC(" A SyncSrvDataLen is : %d \n",pstSyncData->stSyncMSGA.stSyncSrvData.uiSyncSrvDataLen);
        
        if(pstSyncData->stSyncMSGA.stSyncSrvData.uiSyncSrvDataLen == sizeof(SysPMStateInfo_t))
        {/* 若数据长度等于PM状态数据结构，说明只有有状态数据 */
         /*具体的PM同步状态信息包含哪些内容，需要经讨论决定，而后确定具体处理函数*/
            uiRet = SyncHandleCtrlData(&pstSyncData->stSyncMSGA.stSyncSrvData.stPMState, emNextPMId, \
                    pstSyncData->sLinkASrvLen);
//            /* PM 信息同步处理 */
//            SyncCtrlInfoHandle();
//            /* PM 密码同步处理 */
//            SyncCtrlPwHandle();
            /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
//            SysSyncSrvQueueWait(TASK_WAIT);
        }
        else if(pstSyncData->stSyncMSGA.stSyncSrvData.uiSyncSrvDataLen > sizeof(SysPMStateInfo_t))
        {
            /*具体的PM同步状态信息包含哪些内容，需要经讨论决定，而后确定具体处理函数*/
               uiRet = SyncHandleCtrlData(&pstSyncData->stSyncMSGA.stSyncSrvData.stPMState, emNextPMId, \
                       pstSyncData->sLinkASrvLen);
//               /* PM 信息同步处理 */
//               SyncCtrlInfoHandle();
//               /* PM 密码同步处理 */
//               SyncCtrlPwHandle();
               /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
//               SysSyncSrvQueueWait(TASK_WAIT);
            
            
            if((SRV_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType) &&
                    (CS1131_SRV == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为CS1131服务 */
                uiRet = SyncHandleCS1131SrvData(&pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData, emNextPMId, \
                                                    pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.uiSrvDataLen);
                
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType) &&
                    (OPC_SRV == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为OPC服务 */
                uiRet = SyncHandleOPCSrvData(&pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData, emNextPMId, \
                                                pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType) &&
                    (P2P_SRV == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PSrvData(&pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData, emNextPMId, \
                                                pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType) &&
                    (MODBUS_SRV == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleModbusSrvData(&pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData, emNextPMId, \
                                                pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType) &&
                    (CTRL_SRV == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为PM 间服务 */
                uiRet = SyncHandleCtrlSrvData(&pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData, emNextPMId, \
                                                pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if(SRVACK_DATA == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.emType)
            {/* 若数据类型为应答数据，置接收到应答数据标识 */
                *puiSrvAckHandleFlag |= BITMASK(SYNC_SRVACK_HANDLE_BITA);
            }
            else
            {/* 接收到状态数据类型错误 */
                uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
            }
        }
        else
        {
            
        }

        if(uiRet != SYNC_HANDLE_STATDATA_OK)
        {/* 数据处理错误 */

        }
    }

    /* handle the link b stat data */
    if(pstSyncData->sLinkBSrvLen > SYNC_RECV_SIZE_0)
    {/* 处理链接B接收到数据，即前一个PM 发送的数据 */
        
//        DEBUG_SYNC(" B SyncSrvDataLen is : %d \n",pstSyncData->stSyncMSGB.stSyncSrvData.uiSyncSrvDataLen);
        
        if(pstSyncData->stSyncMSGB.stSyncSrvData.uiSyncSrvDataLen == sizeof(SysPMStateInfo_t))
        {/* 若数据长度等于PM状态数据结构，说明只有状态数据 */
            uiRet = SyncHandleCtrlData(&pstSyncData->stSyncMSGB.stSyncSrvData.stPMState, emPrePMId, \
                                        pstSyncData->sLinkBSrvLen);
//            /* PM 信息同步处理 */
//            SyncCtrlInfoHandle();
//            /* PM 密码同步处理 */
//            SyncCtrlPwHandle();
            /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
//            SysSyncSrvQueueWait(TASK_WAIT);
        }
        else if(pstSyncData->stSyncMSGB.stSyncSrvData.uiSyncSrvDataLen > sizeof(SysPMStateInfo_t))
        {
            
            uiRet = SyncHandleCtrlData(&pstSyncData->stSyncMSGB.stSyncSrvData.stPMState, emPrePMId, \
                                        pstSyncData->sLinkBSrvLen);
//            /* PM 信息同步处理 */
//            SyncCtrlInfoHandle();
//            /* PM 密码同步处理 */
//            SyncCtrlPwHandle();
            /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
//            SysSyncSrvQueueWait(TASK_WAIT);
            
            if((SRV_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType) &&
                    (CS1131_SRV == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为CS1131服务 */
                uiRet = SyncHandleCS1131SrvData(&pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData, emPrePMId, \
                        pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType) &&
                    (OPC_SRV == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为OPC服务 */
                uiRet = SyncHandleOPCSrvData(&pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData, emPrePMId, \
                        pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType) &&
                    (P2P_SRV == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleP2PSrvData(&pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData, emPrePMId, \
                        pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType) &&
                    (MODBUS_SRV == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为P2P服务 */
                uiRet = SyncHandleModbusSrvData(&pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData, emPrePMId, \
                        pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if((SRV_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType) &&
                    (CTRL_SRV == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emSubType))
            {/* 若数据类型为服务数据，服务类型为PM 间服务 */
                uiRet = SyncHandleCtrlSrvData(&pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData, emPrePMId, \
                        pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.uiSrvDataLen);
                /* 若此时同步任务队列不为空，将同步任务做等待一拍处理，防止连续发送导致丢包 */
                SysSyncSrvQueueWait(TASK_WAIT);
            }
            else if(SRVACK_DATA == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.emType)
            {/* 若数据类型为应答数据，置接收到应答数据标识 */
                *puiSrvAckHandleFlag |= BITMASK(SYNC_SRVACK_HANDLE_BITB);
            }
            else
            {/* 接收到状态数据类型错误 */
                 uiRet = SYNC_HANDLE_STATDATA_TYPE_ERR;
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
* 函数名称: SyncHandleCtrlData
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
uint32_t SyncHandleCtrlData(const SysPMStateInfo_t* pstOneStatData, PMController_t emSrcPMId, int16_t sDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;
    SysPMStateInfo_t* pstPMState = NULL;
    
    if(sDataLen >= sizeof(SysPMStateInfo_t))
    {
        pstPMState = SysGetPMStateInfo(emSrcPMId);
        /*将接收到的源PM的状态信息记录在本地*/
        memcpy(pstPMState, pstOneStatData, sizeof(SysPMStateInfo_t));
        
        SysSetWorkStatus(emSrcPMId, pstPMState->emWorkStatus);
        
//        DEBUG_SYNC("  SrcPMId %d: emWorkStatus  is : %x   %x\n",emSrcPMId,pstPMState->emWorkStatus,SysGetWorkStatus(emSrcPMId));
//        DEBUG_SYNC("  SrcPMId %d: uiPrjID  is : %x \n",emSrcPMId,pstPMState->stPrjInfo.uiPrjID);
//        DEBUG_SYNC("  SrcPMId : uiMinorVern  is : %x \n",pstPMState->stPrjInfo.uiMinorVern);
    }
    else
    {
        uiRet = SYNC_HANDLE_CTRLDATA_ACKERR;
    }


    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncCtrlInfoHandle
*
* 功能描述: 工程信息同步处理函数。
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
void SyncCtrlInfoHandle(void)
{


    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncCtrlPWHandle
*
* 功能描述: 密码同步处理函数。
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
void SyncCtrlPWHandle(void)
{

    return;
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
//    uint32_t uiSendLen = 0U;                                                            /* 应答数据长度             */
//    uint32_t uiRetSend = RET_OK;                                                        /* 应答发送返回值           */
//    SrvData_t stSyncSrvACKData;                                                         /* 应答数据结构             */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, CS1131_SRV);        /* CS1131服务数据指针 ,存放在共享内存上的1131服务数据结构*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本地PM ID号           */
//
//    /* 将服务数据拷贝到共享内存上服务数据结构中 */
//    memcpy((puint8_t)pstSysSrvInfo, pstSrvData->ucData, uiDataLen);
//    /* 设置本机服务处理后不需给CS1131应答 */
//    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;
//    /* 默认设置服务同步正确 */
//    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//    /* 默认设置有服务需要处理标识 */
//    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
//
//    /* 若该状态数据需要应答 */
//    if(NEED_ACK == pstSrvData->emAckFlag)
//    {
//        /* 清空应答数据结构 */
//        memset((puint8_t)&stSyncSrvACKData, 0U, sizeof(SrvData_t));
//        /* 填充应答数据头 */
//        stSyncSrvACKData.emType = SRVACK_DATA;
//        stSyncSrvACKData.emSubType = NOT_USED;
//        stSyncSrvACKData.emAckFlag = NO_ACK;
//        /* 应答接收服务OK */
//        stSyncSrvACKData.ucData[0] = ACK_DATA_OK;
//        /* 计算应答数据长度 */
//        uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvACKData.emType) + \
//                    sizeof(stSyncSrvACKData.emSubType) + sizeof(stSyncSrvACKData.emAckFlag);
//        /* 调用状态数据协议发送函数 */
//        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvACKData, emLocalPMId, emSrcPMId, uiSendLen);
//
//        /* 数据发送失败 */
//        if(RET_OK != uiRetSend)
//        {
//            uiRet = SYNC_HANDLE_SRVDATA_ACKERR;
//        }
//    }

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
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                       /* 数据处理结果             */
//    uint32_t uiSendLen = 0U;                                        /* 应答数据长度             */
//    uint32_t uiRetSend = RET_OK;                                    /* 应答发送返回值           */
//    SrvData_t stSyncSrvAckData;                                     /* 应答数据结构             */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, OPC_SRV);           /* OPC服务数据指针 ,存放在共享内存上的OPC服务数据结构*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
//
//    /* 将服务数据拷贝到共享内存上服务数据结构中 */
//    memcpy((puint8_t)pstSysSrvInfo, pstSrvData->ucData, uiDataLen);
//    /* 设置本机服务处理后不需给OPC应答 */
//    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
//    /* 默认设置服务同步正确 */
//    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//    /* 默认设置有服务需要处理标识 */
//    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
//
//    /* 若该状态数据需要应答 */
//    if(NEED_ACK == pstSrvData->emAckFlag)
//    {
//        /* 清空应答数据结构 */
//        memset((puint8_t)&stSyncSrvAckData, 0U, sizeof(SrvData_t));
//        /* 填充应答数据头 */
//        stSyncSrvAckData.emType = SRVACK_DATA;
//        stSyncSrvAckData.emSubType = NOT_USED;
//        stSyncSrvAckData.emAckFlag = NO_ACK;
//        /* 应答接收服务OK */
//        stSyncSrvAckData.ucData[0] = ACK_DATA_OK;
//        /* 计算应答数据长度 */
//        uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvAckData.emType) + \
//                    sizeof(stSyncSrvAckData.emSubType) + sizeof(stSyncSrvAckData.emAckFlag);
//        /* 调用状态数据协议发送函数 */
//        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvAckData, emLocalPMId, emSrcPMId, uiSendLen);
//
//        /* 数据发送失败 */
//        if(RET_OK != uiRetSend)
//        {
//            uiRet = SYNC_HANDLE_SRVDATA_ACKERR;
//        }
//    }

    return uiRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncHandleP2PSrvData
*
* 功能描述: P2P服务同步包处理函数。
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
uint32_t SyncHandleP2PSrvData(SrvData_t* pstSrvData, PMController_t emSrcPMId,
                             uint32_t uiDataLen)
{
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                       /* 数据处理结果             */
//    uint32_t uiSendLen = 0U;                                        /* 应答数据长度             */
//    uint32_t uiRetSend = RET_OK;                                    /* 应答发送返回值           */
//    SrvData_t stSyncSrvAckData;                                     /* 应答数据结构             */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, P2P_SRV);           /* P2P服务数据指针 ,存放在共享内存上的P2P服务数据结构*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
//    P2PRecvHead_t* pstP2PRecvHead;
//
//    pstP2PRecvHead = (P2PRecvHead_t*)&pstSrvData->ucData;
//    if( P2P_RES_FRAME == pstP2PRecvHead->ucFrameType )
//    {
//        CommClearP2PWaitAckFlg(pstP2PRecvHead->ucSrcStaID);
//        /*调用安全层解析应答数据*/
//        StaDecodeRes();
//
//    }
//    /* 将服务数据拷贝到共享内存上服务数据结构中 */
//    memcpy((puint8_t)pstSysSrvInfo,  pstSrvData->ucData, uiDataLen);
//
//    /* 设置本机服务处理后不需给P2P应答 */
//    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
//    /* 默认设置服务同步正确 */
//    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//    /* 默认设置有服务需要处理标识 */
//    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
//
//    /* 若该状态数据需要应答 */
//    if(NEED_ACK == pstSrvData->emAckFlag)
//    {
//        /* 清空应答数据结构 */
//        memset((puint8_t)&stSyncSrvAckData, 0U, sizeof(SrvData_t));
//        /* 填充应答数据头 */
//        stSyncSrvAckData.emType = SRVACK_DATA;
//        stSyncSrvAckData.emSubType = NOT_USED;
//        stSyncSrvAckData.emAckFlag = NO_ACK;
//        /* 应答接收服务OK */
//        stSyncSrvAckData.ucData[0] = ACK_DATA_OK;
//        /* 计算应答数据长度 */
//        uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvAckData.emType) + \
//                    sizeof(stSyncSrvAckData.emSubType) + sizeof(stSyncSrvAckData.emAckFlag);
//        /* 调用状态数据协议发送函数 */
//        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvAckData, emLocalPMId, emSrcPMId, uiSendLen);
//
//        /* 数据发送失败 */
//        if(RET_OK != uiRetSend)
//        {
//            uiRet = SYNC_HANDLE_SRVDATA_ACKERR;
//
//        }
//    }

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
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                       /* 数据处理结果             */
//    uint32_t uiSendLen = 0U;                                        /* 应答数据长度             */
//    uint32_t uiRetSend = RET_OK;                                    /* 应答发送返回值           */
//    SrvData_t stSyncSrvAckData;                                     /* 应答数据结构             */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, MODBUS_SRV);        /* Modbus数据指针 ,存放在共享内存上的Modbus服务数据结构*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
//
//    /* 将服务数据拷贝到共享内存上服务数据结构中 */
//    memcpy((puint8_t)pstSysSrvInfo,  pstSrvData->ucData, uiDataLen);
//    /* 设置本机服务处理后不需给P2P应答 */
//    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
//    /* 默认设置服务同步正确 */
//    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//    /* 默认设置有服务需要处理标识 */
//    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
//
//    /* 若该状态数据需要应答 */
//    if(NEED_ACK == pstSrvData->emAckFlag)
//    {
//        /* 清空应答数据结构 */
//        memset((puint8_t)&stSyncSrvAckData, 0U, sizeof(SrvData_t));
//        /* 填充应答数据头 */
//        stSyncSrvAckData.emType = SRVACK_DATA;
//        stSyncSrvAckData.emSubType = NOT_USED;
//        stSyncSrvAckData.emAckFlag = NO_ACK;
//        /* 应答接收服务OK */
//        stSyncSrvAckData.ucData[0] = ACK_DATA_OK;
//        /* 计算应答数据长度 */
//        uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvAckData.emType) + \
//                    sizeof(stSyncSrvAckData.emSubType) + sizeof(stSyncSrvAckData.emAckFlag);
//        /* 调用状态数据协议发送函数 */
//        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvAckData, emLocalPMId, emSrcPMId, uiSendLen);
//
//        /* 数据发送失败 */
//        if(RET_OK != uiRetSend)
//        {
//            uiRet = SYNC_HANDLE_SRVDATA_ACKERR;
//
//        }
//    }

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
    uint32_t uiRet = SYNC_HANDLE_STATDATA_OK;                           /* 数据处理结果             */
//    uint32_t uiSendLen = 0U;                                            /* 应答数据长度             */
//    uint32_t uiRetSend = RET_OK;                                        /* 应答发送返回值           */
//    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
//    SrvData_t stSyncSrvAckData;                                         /* 应答数据结构             */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(pstSrvData->emCMid, CTRL_SRV);              /* 服务数据指针           */
//    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();          /* 同步任务队列             */
//    SysJoinReqStatus_t* pstSysJoinReqStatus = SysGetJoinReqInfo();      /* PM 请求加入数据指针   */
//    SyncTime64_t unCurTime;                                             /* 当前同步时间             */
//    SyncCtrlSrv_t* pstSyncCtrlSrv = NULL;                               /* PM 服务数据指针       */
//
//    /* 设置PM 服务指针初始值 */
//    pstSyncCtrlSrv = (SyncCtrlSrv_t*)&pstSrvData->ucData;
//
//    /* PM 服务号进行相应处理，共有两种PM 服务:
//     * 1、系统由2系运行到3系运行升级重构时，请求加入服务
//     * 2、PM 同步失败时，取消同步数据服务 */
//    switch(pstSyncCtrlSrv->uiSrvId)
//    {
//        case SYS_REQUEST_ACTIVE:
//            /* 获取当前同步时间 */
//            SysGetSyncTime(&unCurTime);
//
//            if(SysDiffSyncTime(&pstSyncCtrlSrv->unTimeOutTime, &unCurTime) < 0L)
//            {/* 若请求加入服务时间未过期 */
//                pstSysJoinReqStatus->unActiveTime.ullTime = pstSyncCtrlSrv->unActiveTime.ullTime;
//                pstSysJoinReqStatus->unTimeOutTime.ullTime = pstSyncCtrlSrv->unTimeOutTime.ullTime;
//                pstSysJoinReqStatus->emJoinReqFlag = JOIN_REQ_ACTIVE;
//                stSyncSrvAckData.ucData[0] = ACK_DATA_OK;
//            }
//            else
//            {/* 若请求加入服务时间过期 */
//                pstSysJoinReqStatus->emJoinReqFlag = NO_JOIN_REQ_ACTIVE;
//                stSyncSrvAckData.ucData[0] = ACK_DATA_ERR;
//            }
//
//
//            /* 填充应答数据头 */
//            stSyncSrvAckData.emType = SRVACK_DATA;
//            stSyncSrvAckData.emSubType = NOT_USED;
//            stSyncSrvAckData.emAckFlag = NO_ACK;
//            /* 计算应答数据长度 */
//            uiSendLen = sizeof(uint8_t) + sizeof(stSyncSrvAckData.emType) + \
//                        sizeof(stSyncSrvAckData.emSubType) + sizeof(stSyncSrvAckData.emAckFlag);
//            /* 发送应答数据 */
//            uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvAckData, \
//                                    emLocalPMId, emSrcPMId, uiSendLen);
//
//            /* 发送失败 */
//            if(uiRetSend != RET_OK)
//            {/* send error */
//                uiRet = SYNC_HANDLE_SRVDATA_ACKERR;
//                /*log*/
//            }
//            break;
//        case SYS_PAUSE:
//            break;
//
//        case SYS_CONTINUE:
//            break;
//
//        case SYS_LAST_SRV_CANCEL:
//            /* 接收到取消服务 */
//            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
//            pstSysJoinReqStatus->emJoinReqFlag = NO_JOIN_REQ_ACTIVE;
//
//            break;
//        default:
//            break;
//    }

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
//    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};  /* 等待应答记录             */
//    static uint32_t s_uiRecordCon = 0U;                                 /* 广播取消处理记录         */
//    uint32_t uiCount = 0U;                                              /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                            /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                        /* 发送返回值               */
//    long64_t llInternalTime = 0L;                                       /* 同步时间间隔             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PM ID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PM ID号     */
//    PMController_t emDstPMId = PMA;                                     /* 目的PM ID号           */
//    SrvData_t stSyncSrvData;                                            /* 发送CS1131服务数据缓存       */
//    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */
//    SyncTime64_t unCurTime;                                             /* 当前时间                 */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, CS1131_SRV);            /* 全局CS1131服务数据指针       */
//
//    /* 获取任务队列任务 */
//    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
//    /* 根据同步CS1131服务任务状态做相应处理，共有两种任务状态
//     * 1、数据发送状态
//     * 2、等待应答状态 */
//    switch(stSyncTask.emTaskStatus)
//    {
//        case TASK_SEND:
//            /* 任务处于发送状态时，填充发送缓存 */
//            stSyncSrvData.emType = SRV_DATA;
//            uiSendLen += sizeof(stSyncSrvData.emType);
//            stSyncSrvData.emSubType = CS1131_SRV;
//            uiSendLen += sizeof(stSyncSrvData.emSubType);
//            stSyncSrvData.emCMid = emCMid;
//            uiSendLen += sizeof(stSyncSrvData.emCMid);
//            stSyncSrvData.emAckFlag = emAckFlag;
//            uiSendLen += sizeof(stSyncSrvData.emAckFlag);
//
//            /* 填充CS1131服务，此处仅发送服务实际长度，而非结构体全部长度 */
//            memcpy(stSyncSrvData.ucData, (puint8_t)pstSysSrvInfo, \
//                          sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
//            /* 计算发送数据长度 */
//            uiSendLen +=  sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);
//
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送CS1131服务 */
//                for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
//                {
//                    emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
//                    if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
//                    {
//                        /* 记录需接收应答标识 */
//                        s_uiWaitAck[emDstPMId] = NEED_ACK;
//                        s_uiRecordCon |= BITMASK(emDstPMId);
//                        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                            emDstPMId, uiSendLen);
//                        if(uiRetSend != RET_OK)
//                        {/* 发送数据错误 */
//                             /*log: send error*/
//                        }
//                    }
//                }
//            }
//            else
//            {/* 给指定PM 发送CS1131服务 */
//                s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
//                s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
//                uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                    stSyncTask.emPMId, uiSendLen);
//                if(uiRetSend != RET_OK)
//                {/* 发送数据错误 */
//                    /*log: send error*/
//                }
//            }
//
//            /* 更新同步任务队列 */
//            if(NO_ACK == emAckFlag)
//            {
//                /* 若不需要接收应答，出队 */
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//            }
//            else
//            {
//                /* 若需要接收应答，更新任务状态为等待应答 */
//                SysSyncSrvQueueUpdate(TASK_WAITACK);
//            }
//            break;
//        case TASK_WAITACK:
//            if(uiSrvAckHandleFlag)
//            {/* 处理应答数据 */
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
//                   NEED_ACK == s_uiWaitAck[emNextPMId])
//                {/* 接收到下一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emNextPMId] = NO_ACK;
//                    }
//                }
//
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
//                   NEED_ACK == s_uiWaitAck[emPrePMId])
//                {/* 接收到前一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emPrePMId] = NO_ACK;
//                        /**/
//                    }
//                }
//            }
//
//            if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
//               && (NO_ACK == s_uiWaitAck[PMC]))
//            {/* 超时时间内接收到应答，CS1131服务同步成功 */
//                s_uiRecordCon = 0U;
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//                pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//
//                /*log*/
//            }
//            else
//            {/* 判断是否超时 */
//                SysGetSyncTime(&unCurTime);
//                /* 计算当前时间与超时时间差值 */
//                llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
//                                                &unCurTime);
//                if(llInternalTime >= 0)
//                {/* 等待应答超时 */
//                    SysDeSyncSrvQueue(pstSyncSrvQueue);
//                    /* 发送取消服务 */
//                    SyncBroadCastCancel(s_uiRecordCon);
//                    s_uiRecordCon = 0U;
//                    s_uiWaitAck[PMA] = NO_ACK;
//                    s_uiWaitAck[PMB] = NO_ACK;
//                    s_uiWaitAck[PMC] = NO_ACK;
//                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//                    pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
//                    /*log*/
//                }
//            }
//            break;
//        default:
//            break;
//    }

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
//    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};  /* 等待应答记录             */
//    static uint32_t s_uiRecordCon = 0U;                                 /* 广播取消处理记录         */
//    uint32_t uiCount = 0U;                                              /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                            /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                        /* 发送返回值               */
//    long64_t llInternalTime = 0L;                                       /* 同步时间间隔             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PM ID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PM ID号     */
//    PMController_t emDstPMId = PMA;                                     /* 目的PM ID号           */
//    SrvData_t stSyncSrvData;                                            /* 发送OPC服务数据缓存       */
//    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */
//    SyncTime64_t unCurTime;                                             /* 当前时间                 */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, OPC_SRV);               /* 全局OPC服务数据指针       */
//
//    /* 获取任务队列任务 */
//    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
//    /* 根据同步OPC服务任务状态做相应处理，共有两种任务状态
//     * 1、数据发送状态
//     * 2、等待应答状态 */
//    switch(stSyncTask.emTaskStatus)
//    {
//        case TASK_SEND:
//            /* 任务处于发送状态时，填充发送缓存 */
//            stSyncSrvData.emType = SRV_DATA;
//            uiSendLen += sizeof(stSyncSrvData.emType);
//            stSyncSrvData.emSubType = OPC_SRV;
//            uiSendLen += sizeof(stSyncSrvData.emSubType);
//            stSyncSrvData.emCMid = emCMid;
//            uiSendLen += sizeof(stSyncSrvData.emCMid);
//            stSyncSrvData.emAckFlag = emAckFlag;
//            uiSendLen += sizeof(stSyncSrvData.emAckFlag);
//
//            /* 填充OPC服务，此处仅发送服务实际长度，而非结构体全部长度 */
//            memcpy(stSyncSrvData.ucData, (puint8_t)pstSysSrvInfo, \
//                          sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
//            /* 计算发送数据长度 */
//            uiSendLen +=  sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);
//
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送OPC服务 */
//                for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
//                {
//                    emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
//                    if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
//                    {
//                        /* 记录需接收应答标识 */
//                        s_uiWaitAck[emDstPMId] = NEED_ACK;
//                        s_uiRecordCon |= BITMASK(emDstPMId);
//                        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                            emDstPMId, uiSendLen);
//
//                        if(uiRetSend != RET_OK)
//                        {/* 发送数据错误 */
//                            /*log: send error*/
//
//                        }
//                    }
//                }
//            }
//            else
//            {/* 给指定PM 发送OPC服务 */
//                s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
//                s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
//                uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                    stSyncTask.emPMId, uiSendLen);
//
//                if(uiRetSend != RET_OK)
//                {/* 发送数据错误 */
//                    /*log: send error*/
//                }
//            }
//
//            /* 更新同步任务队列 */
//            if(NO_ACK == emAckFlag)
//            {
//                /* 若不需要接收应答，出队 */
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//            }
//            else
//            {
//                /* 若需要接收应答，更新任务状态为等待应答 */
//                SysSyncSrvQueueUpdate(TASK_WAITACK);
//            }
//            break;
//        case TASK_WAITACK:
//            if(uiSrvAckHandleFlag)
//            {/* 处理应答数据 */
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
//                   NEED_ACK == s_uiWaitAck[emNextPMId])
//                {/* 接收到下一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emNextPMId] = NO_ACK;
//                    }
//                }
//
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
//                   NEED_ACK == s_uiWaitAck[emPrePMId])
//                {/* 接收到前一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emPrePMId] = NO_ACK;
//                    }
//                }
//            }
//
//            if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
//               && (NO_ACK == s_uiWaitAck[PMC]))
//            {/* 超时时间内接收到应答，OPC服务同步成功 */
//                s_uiRecordCon = 0U;
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//                pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//                /*log*/
//            }
//            else
//            {/* 判断是否超时 */
//                SysGetSyncTime(&unCurTime);
//                /* 计算当前时间与超时时间差值 */
//                llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
//                                                &unCurTime);
//                if(llInternalTime >= 0)
//                {/* 等待应答超时 */
//                    SysDeSyncSrvQueue(pstSyncSrvQueue);
//                    /* 发送取消服务 */
//                    SyncBroadCastCancel(s_uiRecordCon);
//                    s_uiRecordCon = 0U;
//                    s_uiWaitAck[PMA] = NO_ACK;
//                    s_uiWaitAck[PMB] = NO_ACK;
//                    s_uiWaitAck[PMC] = NO_ACK;
//                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//
//                    /*log*/
//                }
//            }
//            break;
//        default:
//            break;
//    }

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
//    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};  /* 等待应答记录             */
//    static uint32_t s_uiRecordCon = 0U;                                 /* 广播取消处理记录         */
//    uint32_t uiCount = 0U;                                              /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                            /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                        /* 发送返回值               */
//    long64_t llInternalTime = 0L;                                       /* 同步时间间隔             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PM ID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PM ID号     */
//    PMController_t emDstPMId = PMA;                                     /* 目的PM ID号           */
//    SrvData_t stSyncSrvData;                                            /* 发送Modbus服务数据缓存       */
//    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */
//    SyncTime64_t unCurTime;                                             /* 当前时间                 */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, MODBUS_SRV);            /* 全局Modbus服务数据指针       */
//
//    /* 获取任务队列任务 */
//    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
//    /* 根据同步Modbus服务任务状态做相应处理，共有两种任务状态
//     * 1、数据发送状态
//     * 2、等待应答状态 */
//    switch(stSyncTask.emTaskStatus)
//    {
//        case TASK_SEND:
//            /* 任务处于发送状态时，填充发送缓存 */
//            stSyncSrvData.emType = SRV_DATA;
//            uiSendLen += sizeof(stSyncSrvData.emType);
//            stSyncSrvData.emSubType = MODBUS_SRV;
//            uiSendLen += sizeof(stSyncSrvData.emSubType);
//            stSyncSrvData.emCMid = emCMid;
//            uiSendLen += sizeof(stSyncSrvData.emCMid);
//            stSyncSrvData.emAckFlag = emAckFlag;
//            uiSendLen += sizeof(stSyncSrvData.emAckFlag);
//
//            /* 填充Modbus服务，此处仅发送服务实际长度，而非结构体全部长度 */
//            memcpy(stSyncSrvData.ucData, (puint8_t)pstSysSrvInfo,
//                          sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
//            /* 计算发送数据长度 */
//            uiSendLen +=  sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);
//
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送Modbus服务 */
//                for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
//                {
//                    emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
//                    if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
//                    {
//                        /* 记录需接收应答标识 */
//                        s_uiWaitAck[emDstPMId] = NEED_ACK;
//                        s_uiRecordCon |= BITMASK(emDstPMId);
//                        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                            emDstPMId, uiSendLen);
//                        if(uiRetSend != RET_OK)
//                        {/* 发送数据错误 */
//                             /*log: send error*/
//                        }
//                    }
//                }
//            }
//            else
//            {/* 给指定PM 发送Modbus服务 */
//                s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
//                s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
//                uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                    stSyncTask.emPMId, uiSendLen);
//                if(uiRetSend != RET_OK)
//                {/* 发送数据错误 */
//                    /*log: send error*/
//                }
//            }
//
//            /* 更新同步任务队列 */
//            if(NO_ACK == emAckFlag)
//            {
//                /* 若不需要接收应答，出队 */
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//            }
//            else
//            {
//                /* 若需要接收应答，更新任务状态为等待应答 */
//                SysSyncSrvQueueUpdate(TASK_WAITACK);
//            }
//            break;
//        case TASK_WAITACK:
//            if(uiSrvAckHandleFlag)
//            {/* 处理应答数据 */
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
//                   NEED_ACK == s_uiWaitAck[emNextPMId])
//                {/* 接收到下一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emNextPMId] = NO_ACK;
//                    }
//                }
//
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
//                   NEED_ACK == s_uiWaitAck[emPrePMId])
//                {/* 接收到前一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emPrePMId] = NO_ACK;
//                    }
//                }
//            }
//
//            if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
//               && (NO_ACK == s_uiWaitAck[PMC]))
//            {/* 超时时间内接收到应答，Modbus服务同步成功 */
//                s_uiRecordCon = 0U;
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//                pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//                /*log*/
//            }
//            else
//            {/* 判断是否超时 */
//                SysGetSyncTime(&unCurTime);
//                /* 计算当前时间与超时时间差值 */
//                llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
//                                                &unCurTime);
//                if(llInternalTime >= 0)
//                {/* 等待应答超时 */
//                    SysDeSyncSrvQueue(pstSyncSrvQueue);
//                    /* 发送取消服务 */
//                    SyncBroadCastCancel(s_uiRecordCon);
//                    s_uiRecordCon = 0U;
//                    s_uiWaitAck[PMA] = NO_ACK;
//                    s_uiWaitAck[PMB] = NO_ACK;
//                    s_uiWaitAck[PMC] = NO_ACK;
//                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//                    /*log*/
//                }
//            }
//            break;
//        default:
//            break;
//    }
//
//    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastP2PSrv
*
* 功能描述: 广播P2P服务
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
void SyncBroadCastP2PSrv(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                      uint32_t uiSrvAckHandleFlag, CMController_t emCMid, ack_flag_t emAckFlag)
{
//    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};  /* 等待应答记录             */
//    static uint32_t s_uiRecordCon = 0U;                                 /* 广播取消处理记录         */
//    uint32_t uiCount = 0U;                                              /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                            /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                        /* 发送返回值               */
//    long64_t llInternalTime = 0L;                                       /* 同步时间间隔             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PM ID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PM ID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PM ID号     */
//    PMController_t emDstPMId = PMA;                                     /* 目的PM ID号           */
//    SrvData_t stSyncSrvData;                                            /* 发送P2P服务数据缓存       */
//    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */
//    SyncTime64_t unCurTime;                                             /* 当前时间                 */
//    SysSrvInfo_t* pstSysSrvInfo = SysGetSrvInfo(emCMid, P2P_SRV);               /* 全局P2P服务数据指针       */
//
//    /* 获取任务队列任务 */
//    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
//    /* 根据同步P2P服务任务状态做相应处理，共有两种任务状态
//     * 1、数据发送状态
//     * 2、等待应答状态 */
//    switch(stSyncTask.emTaskStatus)
//    {
//        case TASK_SEND:
//            /* 任务处于发送状态时，填充发送缓存 */
//            stSyncSrvData.emType = SRV_DATA;
//            uiSendLen += sizeof(stSyncSrvData.emType);
//            stSyncSrvData.emSubType = P2P_SRV;
//            uiSendLen += sizeof(stSyncSrvData.emSubType);
//            stSyncSrvData.emCMid = emCMid;
//            uiSendLen += sizeof(stSyncSrvData.emCMid);
//            stSyncSrvData.emAckFlag = emAckFlag;
//            uiSendLen += sizeof(stSyncSrvData.emAckFlag);
//
//            /* 填充P2P服务，此处仅发送服务实际长度，而非结构体全部长度 */
//            memcpy(stSyncSrvData.ucData, (puint8_t)pstSysSrvInfo,
//                          sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen));
//            /* 计算发送数据长度 */
//            uiSendLen +=  sizeof(SysSrvInfoHead_t) + sizeof(pstSysSrvInfo->stSysSrvData.ucSrvNo)
//                          + pstSysSrvInfo->stSysSrvData.uiSrvLen +
//                          sizeof(pstSysSrvInfo->stSysSrvData.uiSrvLen);
//
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送P2P服务 */
//                for(uiCount = 0; uiCount < (NUM_OF_PM - HARDCODE_ONE); uiCount++)
//                {
//                    emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
//                    if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
//                    {
//                        /* 记录需接收应答标识 */
//                        s_uiWaitAck[emDstPMId] = NEED_ACK;
//                        s_uiRecordCon |= BITMASK(emDstPMId);
//                        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                            emDstPMId, uiSendLen);
//                        if(uiRetSend != RET_OK)
//                        {/* 发送数据错误 */
//                             /*log: send error*/
//                        }
//                    }
//                }
//            }
//            else
//            {/* 给指定PM 发送P2P服务 */
//                s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
//                s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
//                uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                    stSyncTask.emPMId, uiSendLen);
//                if(uiRetSend != RET_OK)
//                {/* 发送数据错误 */
//                    /*log: send error*/
//                }
//            }
//
//            /* 更新同步任务队列 */
//            if(NO_ACK == emAckFlag)
//            {
//                /* 若不需要接收应答，出队 */
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//            }
//            else
//            {
//                /* 若需要接收应答，更新任务状态为等待应答 */
//                SysSyncSrvQueueUpdate(TASK_WAITACK);
//            }
//            break;
//        case TASK_WAITACK:
//            if(uiSrvAckHandleFlag)
//            {/* 处理应答数据 */
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
//                   NEED_ACK == s_uiWaitAck[emNextPMId])
//                {/* 接收到下一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emNextPMId] = NO_ACK;
//                    }
//                }
//
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
//                   NEED_ACK == s_uiWaitAck[emPrePMId])
//                {/* 接收到前一个PM 应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emPrePMId] = NO_ACK;
//                    }
//                }
//            }
//
//            if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
//               && (NO_ACK == s_uiWaitAck[PMC]))
//            {/* 超时时间内接收到应答，P2P服务同步成功 */
//                s_uiRecordCon = 0U;
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//                pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_OK;
//                /*log*/
//            }
//            else
//            {/* 判断是否超时 */
//                SysGetSyncTime(&unCurTime);
//                /* 计算当前时间与超时时间差值 */
//                llInternalTime = SysDiffSyncTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
//                                                &unCurTime);
//                if(llInternalTime >= 0)
//                {/* 等待应答超时 */
//                    SysDeSyncSrvQueue(pstSyncSrvQueue);
//                    /* 发送取消服务 */
//                    SyncBroadCastCancel(s_uiRecordCon);
//                    s_uiRecordCon = 0U;
//                    s_uiWaitAck[PMA] = NO_ACK;
//                    s_uiWaitAck[PMB] = NO_ACK;
//                    s_uiWaitAck[PMC] = NO_ACK;
//                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//                    /*log*/
//                }
//            }
//            break;
//        default:
//            break;
//    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastCtrlData
*
* 功能描述: 广播PM信息数据
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
void SyncBroadCastCtrlData(ack_flag_t emAckFlag)
{
    static uint32_t s_uiFirstFlag = SYS_FIRST_IN;                       /* 系统启动时第一次调用标识 */
    int32_t iSendLen = 0;                                               /* 发送数据长度             */
    int16_t sRetSend = RET_OK;                                          /* 发送返回值               */
    ulong64_t ullTimeValue = 0UL;                                       /* 时间差值                 */
    SyncSrvData_t stSyncSrvData;                                        /* PM同步服务数据           */
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);             /* 前一个PMID号     */
    timer_status_t uiTimerStatus = SysGetTimerStatus(emLocalPMId);      /* 本PM同步时钟主从状态 */
    SysPMStateInfo_t *pstPMStateInfo = SysGetPMStateInfo(emLocalPMId);                 /* 本机工程信息数据指针     */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();          /* 同步任务队列             */
    SysSyncTask_t stSyncTask;                                           /* 同步任务                 */

    if(SYS_QUEUE_NOEMPTY == SysSyncSrvQueueEmpty(pstSyncSrvQueue))
    {/* 若非启动时调用广播PM信息 */
        SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
    }
    else
    {/* 若第一次启动时调用广播PM信息 */
        stSyncTask.emPMId = INVALID_PM_ID;
    }

    do
    {
//        if((AJUST_EN == SysGetAjustTimerEn(stSyncTask.emPMId)) &&
//           (SYS_AJUST_SYNCTIME_NOREADY == SysGetAjustTimerFlag()))
//        {/* 若本机同步时钟状态为主机，且当前时间不在前半秒钟，若非启动时调用，退出
//          * 本周期调用，若是启动时第一次调用，则循环等待时间在前半秒钟，保证第一次
//          * 调用时同步时钟状态为主机的PM给其它PM实现正确校时 */
//            if(s_uiFirstFlag != SYS_FIRST_IN)
//            {
//                break;
//            }
//        }
//        else
//        {
//            /* 记录日志 */
//            /* 填充状态数据头 */
//            stSyncCtrlData.emType = CTRL_DATA;
//            iSendLen += sizeof(stSyncCtrlData.emType);
//
//            stSyncCtrlData.emSubType = NOT_USED;
//            iSendLen += sizeof(stSyncCtrlData.emSubType);
//
//            stSyncCtrlData.emAckFlag = emAckFlag;
//            iSendLen += sizeof(stSyncCtrlData.emAckFlag);
//
//            /* 设置本机同步时钟主从状态 */
//            /*pstSysCtrl->stSysAjtSyncTime.uiMaster = uiTimerStatus;*/
//
//            if(TIMER_MASTER == uiTimerStatus)
//            {
//                /* 设置本机同步时间已校标识 */
//                SysSetSyncAjust(AJUST_HAS);
//            }
//
//            if(AJUST_EN == SysGetAjustTimerEn(stSyncTask.emPMId))
//            {
//                SysSetAjustTimerEn(emLocalPMId, AJUST_EN);
//            }
//            else
//            {
//                SysSetAjustTimerEn(emLocalPMId, AJUST_DISEN);
//            }
//
//            /* 计算校时时间 */
//            SysGetSyncTime(&pstSysCtrl->stSysAjtSyncTime.unSyncAjustTime);
//            ullTimeValue = pstSysCtrl->stSysAjtSyncTime.unSyncAjustTime.ullTime;
//            pstSysCtrl->stSysAjtSyncTime.unSyncAjustTime.ullTime += \
//                SYS_ONE_SEC_VALUE - SysDiv64By32_ASM(&ullTimeValue, SYS_ONE_SEC_VALUE);
//
//            /* 填充本PM信息数据 */
//            memcpy(stSyncCtrlData.ucData, (puint8_t)pstSysCtrl, \
//                          sizeof(SysPMStateInfo_t));
//            uiSendLen += sizeof(SysPMStateInfo_t);
//
//            /* 发送PM信息数据 */
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送，发送数据给所有非离线PM */
//                if(SysGetWorkStatus(emPrePMId) != WS_OFFLINE)
//                {
//                    /* 调用协议处理函数发送 */
//                    uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCtrlData, emLocalPMId, \
//                                    emPrePMId, uiSendLen);
//
//                    if(uiRetSend != RET_OK)
//                    {/* 发送失败 */
//                        /*error log*/
//                    }
//                }
//
//                if(SysGetWorkStatus(emNextPMId) != WS_OFFLINE)
//                {
//                    /* 调用协议处理函数发送 */
//                    uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCtrlData, emLocalPMId, \
//                                    emNextPMId, uiSendLen);
//                    if(uiRetSend != RET_OK)
//                    {/* 发送失败 */
//                        /*error log*/
//                    }
//                }
//                SysSetAjustTimerEn(emNextPMId, AJUST_DISEN);
//                SysSetAjustTimerEn(emPrePMId, AJUST_DISEN);
//            }
//            else
//            {/* 点对点发送，发送数据给指定非离线PM */
//                if(SysGetWorkStatus(stSyncTask.emPMId) != WS_OFFLINE)
//                {
//                    /* 调用协议处理函数发送 */
//                    uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCtrlData, emLocalPMId, \
//                        stSyncTask.emPMId, uiSendLen);
//
//                    if(uiRetSend != RET_OK)
//                    {/* 发送失败 */
//                        /*error log*/
//                    }
//                }
//                SysSetAjustTimerEn(stSyncTask.emPMId, AJUST_DISEN);
//            }
//            /* 将广播PM信息同步任务从同步任务队列中删除 */
//            SysDeSyncSrvQueue(pstSyncSrvQueue);
//            /* 若此时同步任务队列不为空，将下一个同步任务做等待一拍处理，防止连续发送导致丢包 */
//            SysSyncSrvQueueWait(TASK_WAIT);
//            s_uiFirstFlag = SYS_NOFIRST_IN;
//        }
        
        
/*test*/
        
        pstPMStateInfo->emWorkStatus = SysGetWorkStatus(emLocalPMId);
        
        /* 填充本PM信息数据 */
        memcpy(&stSyncSrvData.stPMState, pstPMStateInfo, sizeof(SysPMStateInfo_t));
        
//        DEBUG_SYNC(  " emWorkStatus is : %x \n", pstPMStateInfo->emWorkStatus);
//        DEBUG_SYNC(  " uiPrjID is : %x \n", pstPMStateInfo->stPrjInfo.uiPrjID);
//        DEBUG_SYNC(  " uiMainVern is : %x \n", pstPMStateInfo->stPrjInfo.uiMainVern);
//        DEBUG_SYNC(  " uiMinorVern is : %x \n", pstPMStateInfo->stPrjInfo.uiMinorVern);
//        DEBUG_SYNC(  " uiPrjCRC is : %x \n", pstPMStateInfo->stPrjInfo.uiPrjCRC);
        
        iSendLen += sizeof(SysPMStateInfo_t);
        
        stSyncSrvData.uiSyncSrvDataLen = sizeof(SysPMStateInfo_t);
        
        iSendLen += sizeof(stSyncSrvData.uiSyncSrvDataLen);
        
        /* 发送PM信息数据 */
        if(INVALID_PM_ID == stSyncTask.emPMId)
        {/* 广播方式发送，发送数据给所有非离线PM */
            if((SysGetWorkStatus(emNextPMId) != WS_OFFLINE) || (SYS_NOFIRST_IN == s_uiFirstFlag))
            {
                /* 调用协议处理函数发送 */
                sRetSend = PMBUS_Send(PM_BUS_A,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);
                
//                DEBUG_SYNC("   Send Next PMState , sRetSend is  %x\n",sRetSend);
                

                if(sRetSend != RET_OK)
                {/* 发送失败 */
                    /*error log*/
                }
            }
            
            if((SysGetWorkStatus(emPrePMId) != WS_OFFLINE) || (SYS_NOFIRST_IN == s_uiFirstFlag))
            {
                /* 调用协议处理函数发送 */
                sRetSend = PMBUS_Send(PM_BUS_B,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);
                
//                DEBUG_SYNC("   Send Pre PMState , sRetSend is  %x\n",sRetSend);
                

                if(sRetSend != RET_OK)
                {/* 发送失败 */
                    /*error log*/
                }
            }
            
            
///*test*/
//            /* 调用协议处理函数发送 */
//            sRetSend = PMBUS_Send(PM_BUS_A,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);
//            
////            DEBUG_SYNC("  INVALID_PM_ID Send Next PMState , sRetSend is %d  %x\n",sRetSend,sRetSend);
//            
//
//            if(sRetSend != RET_OK)
//            {/* 发送失败 */
//                /*error log*/
//                
//                DEBUG_SYNC("  sRetSend != RET_OK \n");
//                
//            }
//        
//
//            /* 调用协议处理函数发送 */
//            sRetSend = PMBUS_Send(PM_BUS_B,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);
//
//            if(sRetSend != RET_OK)
//            {/* 发送失败 */
//                /*error log*/
//            }
///*test*/
            
            SysSetAjustTimerEn(emNextPMId, AJUST_DISEN);
            SysSetAjustTimerEn(emPrePMId, AJUST_DISEN);
        }
        else
        {/* 点对点发送，发送数据给指定非离线PM */
            if(stSyncTask.emPMId == emNextPMId)
            {
                /* 调用协议处理函数发送 */
                sRetSend = PMBUS_Send(PM_BUS_A,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);

                
//                DEBUG_SYNC("  Send Next PMState , sRetSend is %d  %x\n",sRetSend,sRetSend);
                
                if(sRetSend != RET_OK)
                {/* 发送失败 */
                    /*error log*/
                }
            }
            if(stSyncTask.emPMId == emPrePMId)
            {
                /* 调用协议处理函数发送 */
                sRetSend = PMBUS_Send(PM_BUS_B,PM_RECV_BUFF_SRV, (puint8_t)&stSyncSrvData, iSendLen,PMBUS_SEND_TIMEOUT);

                if(sRetSend != RET_OK)
                {/* 发送失败 */
                    /*error log*/
                }
            }
            SysSetAjustTimerEn(stSyncTask.emPMId, AJUST_DISEN);
        }
        /* 将广播PM信息同步任务从同步任务队列中删除 */
        SysDeSyncSrvQueue(pstSyncSrvQueue);
        /* 若此时同步任务队列不为空，将下一个同步任务做等待一拍处理，防止连续发送导致丢包 */
        SysSyncSrvQueueWait(TASK_WAIT);
        s_uiFirstFlag = SYS_NOFIRST_IN;
        
/*test*/
        
    }while(SYS_FIRST_IN == s_uiFirstFlag);

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: SyncBroadCastActive
*
* 功能描述: 广播PM请求加入服务
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
void SyncBroadCastActive(SysSyncSrvQueue_t* pstSyncSrvQueue, const SyncData_t* pstSyncData,
                         uint32_t uiSrvAckHandleFlag, ack_flag_t emAckFlag)
{
//    static uint32_t s_uiWaitAck[NUM_OF_PM] = {NO_ACK, NO_ACK, NO_ACK};         /* 等待应答记录             */
//    static uint32_t s_uiRecordCon = 0U;                                        /* 广播取消处理记录         */
//    uint32_t uiCount = 0U;                                                     /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                                   /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                               /* 发送返回值               */
//    long64_t llInternalTime = 0L;                                              /* 同步时间间隔             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                            /* 本地PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                   /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                     /* 前一个PMID号     */
//    PMController_t emDstPMId = PMA;                                            /* 目的PMID号           */
//    SysSyncTask_t stSyncTask;                                                  /* 同步任务                 */
//    SyncTime64_t unCurTime;                                                    /* 当前时间                 */
//    SrvData_t stSyncSrvData;                                                   /* 发送数据缓存             */
//    SyncCtrlSrv_t* pstSyncCtrlSrv = (SyncCtrlSrv_t*)stSyncSrvData.ucData;      /* 发送PM服务指针       */
//    SysJoinReqStatus_t* pstSysActive = SysGetActiveInfo();                      /* 本机全局激活数据指针     */
//
//    /* 获取任务队列任务 */
//    SysSyncSrvQueueFront(pstSyncSrvQueue, &stSyncTask);
//    /* 根据同步PM激活服务任务状态做相应处理，共有两种任务状态
//     * 1、数据发送状态
//     * 2、等待应答状态 */
//    switch(stSyncTask.emTaskStatus)
//    {
//        case TASK_SEND:
//            /* 任务处于发送状态时，填充发送缓存 */
//            stSyncSrvData.emType = SRV_DATA;
//            uiSendLen += sizeof(stSyncSrvData.emType);
//            stSyncSrvData.emSubType = CTRL_SRV;
//            uiSendLen += sizeof(stSyncSrvData.emSubType);
//            stSyncSrvData.emAckFlag = emAckFlag;
//            uiSendLen += sizeof(stSyncSrvData.emAckFlag);
//            /* 填充PM服务号 */
//            pstSyncCtrlSrv->uiSrvId = SYS_REQUEST_ACTIVE;
//            /* 计算服务生效及超时时间 */
//            SysGetSyncTime(&unCurTime);
//            SysCalRelativeTime(&pstSyncCtrlSrv->unActiveTime, &unCurTime, SYNC_RELATIVE_ACTIVE_TIME);
//            SysCalRelativeTime(&pstSyncCtrlSrv->unTimeOutTime, &unCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
//            uiSendLen += sizeof(SyncCtrlSrv_t);
//
//            if(INVALID_PM_ID == stSyncTask.emPMId)
//            {/* 广播方式发送请求激活服务 */
//                for(uiCount = 0; uiCount < NUM_OF_PM - HARDCODE_ONE; uiCount++)
//                {
//                    emDstPMId = (PMController_t)((emLocalPMId + uiCount + HARDCODE_ONE) % NUM_OF_PM);
//                    if(SysGetWorkStatus(emDstPMId) != WS_OFFLINE)
//                    {
//                        /* 记录需接收应答标识 */
//                        s_uiWaitAck[emDstPMId] = NEED_ACK;
//                        s_uiRecordCon |= BITMASK(emDstPMId);
//                        uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                        emDstPMId, uiSendLen);
//                        if(RET_OK != uiRetSend)
//                        {/* 发送数据错误 */
//
//                        }
//                    }
//                }
//            }
//            else
//            {/* 给指定PM请求激活服务 */
//                s_uiWaitAck[stSyncTask.emPMId] = NEED_ACK;
//                s_uiRecordCon |= BITMASK(stSyncTask.emPMId);
//                uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncSrvData, emLocalPMId,
//                                    stSyncTask.emPMId, uiSendLen);
//                if(RET_OK != uiRetSend)
//                {/* 发送数据错误 */
//
//
//                }
//            }
//
//            /* 更新同步任务队列 */
//            if(NO_ACK == emAckFlag)
//            {
//                /* 若不需要接收应答，出队 */
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//
//            }
//            else
//            {
//                /* 若需要接收应答，更新任务状态为等待应答 */
//                pstSysActive->unTimeOutTime.ullTime = pstSyncCtrlSrv->unTimeOutTime.ullTime;
//                pstSysActive->unActiveTime.ullTime = pstSyncCtrlSrv->unActiveTime.ullTime;
//                pstSysActive->emJoinReqFlag = JOIN_REQ_ACTIVE;
//                SysSyncSrvQueueUpdate(TASK_WAITACK);
//
//            }
//            break;
//        case TASK_WAITACK:
//            if(uiSrvAckHandleFlag)
//            {/* 处理应答数据 */
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITA)) && \
//                   NEED_ACK == s_uiWaitAck[emNextPMId])
//                {/* 接收到下一个PM应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGA.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emNextPMId] = NO_ACK;
//                    }
//                }
//
//                if((uiSrvAckHandleFlag & BITMASK(SYNC_SRVACK_HANDLE_BITB)) && \
//                   NEED_ACK == s_uiWaitAck[emPrePMId])
//                {/* 接收到前一个PM应答数据 */
//                    if(ACK_DATA_OK == pstSyncData->stSyncMSGB.stSyncSrvData.stSrvData.ucData[0])
//                    {/* 应答OK */
//                        s_uiWaitAck[emPrePMId] = NO_ACK;
//                    }
//                }
//            }
//
//            if((NO_ACK == s_uiWaitAck[PMA]) && (NO_ACK == s_uiWaitAck[PMB])
//               && (NO_ACK == s_uiWaitAck[PMC]))
//            {/* 超时时间内接收到应答，请求加入服务同步成功 */
//                s_uiRecordCon = 0U;
//                SysDeSyncSrvQueue(pstSyncSrvQueue);
//
//            }
//            else
//            {/* 判断是否超时 */
//                SysGetSyncTime(&unCurTime);
//                /* 计算当前时间与超时时间差值 */
//                llInternalTime = SysDiffSyncTime(&pstSysActive->unTimeOutTime, &unCurTime);
//                if(llInternalTime >= 0)
//                {/* 等待应答超时 */
//                    SysDeSyncSrvQueue(pstSyncSrvQueue);
//                    /* 发送取消服务 */
//                    SyncBroadCastCancel(s_uiRecordCon);
//                    pstSysActive->emJoinReqFlag = NO_JOIN_REQ_ACTIVE;
//                    s_uiRecordCon = 0U;
//                    s_uiWaitAck[PMA] = NO_ACK;
//                    s_uiWaitAck[PMB] = NO_ACK;
//                    s_uiWaitAck[PMC] = NO_ACK;
//                }
//            }
//            break;
//        default:
//            break;
//    }
//
//    return;
}

void SyncBroadCastCancel(uint32_t uiDstRecord)
{
//    uint32_t uiCount = 0U;                                          /* 计数值                   */
//    uint32_t uiSendLen = 0U;                                        /* 发送数据长度             */
//    uint32_t uiRetSend = RET_OK;                                    /* 发送返回值               */
//    PMController_t emLocalConId = SysGetLocalPMID();                /* 本地PM的ID号           */
//    SrvData_t stSyncCancelData;                                     /* 取消服务数据             */
//    SyncCtrlSrv_t* pstSyncCtrlSrv = NULL;                           /* PM服务指针           */
//
//    /* PM服务指针初始化 */
//    pstSyncCtrlSrv = (SyncCtrlSrv_t*)stSyncCancelData.ucData;
//    /* 清空PM服务数据 */
//    memset((puint8_t)stSyncCancelData.ucData, 0U, sizeof(SyncCtrlSrv_t));
//
//    /* 填充状态数据头结构 */
//    stSyncCancelData.emType = SRV_DATA;
//    uiSendLen += sizeof(stSyncCancelData.emType);
//    stSyncCancelData.emSubType = CTRL_SRV;
//    uiSendLen += sizeof(stSyncCancelData.emSubType);
//    stSyncCancelData.emAckFlag = NO_ACK;
//    uiSendLen += sizeof(stSyncCancelData.emAckFlag);
//    /* 填充PM服务号 */
//    pstSyncCtrlSrv->uiSrvId = SYS_LAST_SRV_CANCEL;
//    pstSyncCtrlSrv->unActiveTime.ullTime = 0U;
//    pstSyncCtrlSrv->unTimeOutTime.ullTime = 0U;
//    uiSendLen += sizeof(SyncCtrlSrv_t);
//
//    if(uiDstRecord & BITMASK(PMA))
//    {/* 需给PM A发送取消服务 */
//        if(SysGetWorkStatus(PMA) != WS_OFFLINE)
//        {/* PM A不处于离线状态 */
//            uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCancelData, emLocalConId,
//                                            PMA, uiSendLen);
//            if(RET_OK != uiRetSend)
//            {/* 发送数据失败 */
//                /*log*/
//            }
//        }
//    }
//
//    if(uiDstRecord & BITMASK(PMB))
//    {/* 需给PM B发送取消服务 */
//        if(SysGetWorkStatus(PMB) != WS_OFFLINE)
//        {/* PM B不处于离线状态 */
//            uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCancelData, emLocalConId,
//                                            PMB, uiSendLen);
//            if(RET_OK != uiRetSend)
//            {/* 发送数据失败 */
//                /*log*/
//            }
//        }
//    }
//
//    if(uiDstRecord & BITMASK(PMC))
//    {/* 需给PM C发送取消服务 */
//        if(SysGetWorkStatus(PMC) != WS_OFFLINE)
//        {/* PM C不处于离线状态 */
//            uiRetSend = PMProtocolSendSrv((puint8_t)&stSyncCancelData, emLocalConId,
//                                            PMC, uiSendLen);
//            if(RET_OK != uiRetSend)
//            {/* 发送数据失败 */
//                /*log*/
//            }
//        }
//    }

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
    uint32_t uiRealDataLen = 0U;                                /* 除去头结构具体数据长度   */
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本机PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);               /* 本机PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                 /* 本机PMID号           */
    SysSyncTask_t stSyncTask;                                   /* 同步任务                 */
    SyncPrjData_t* pstSyncPrjData = NULL;
    SyncRealData_t* pstSyncRealData = NULL;
    int16_t sRecvErr = 0;

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

    if(stSyncTask.emPMId == emNextPMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncMSGA.stSyncPrjData;
        sRecvErr = s_stSyncData.sLinkAPrjErr;
    }

    if(stSyncTask.emPMId == emPrePMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncMSGB.stSyncPrjData;
        sRecvErr = s_stSyncData.sLinkBPrjErr;
    }

    if((pstSyncPrjData->uiSyncPrjDataLen > 0U) && (sRecvErr == RET_OK))
    {/* 接收到数据长度大于0,且接收数据正确 */
        /* 根据接收数据类型进行处理 */
        pstSyncRealData = (SyncRealData_t*)pstSyncPrjData->ucPrjData;
        switch(pstSyncRealData->stSyncHead.emSrvType)
        {
            case SYNC_REAL_DATA:
                /* 接收到实时数据 */
                uiRealDataLen = pstSyncPrjData->uiSyncPrjDataLen - sizeof(SyncHead_t) - sizeof(SyncDataHead_t);
                SyncRecvData(pstSyncRealData, uiRealDataLen, stSyncTask.emPMId);
                break;
            case SYNC_COMPLETE:
                /* 接收到实时数据同步完成 */
                SyncRecvRealDataOk(stSyncTask.emPMId);
                break;
            default:
                break;
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
void SyncRecvData(SyncRealData_t* pstSyncPrjData, uint32_t uiDataLen, PMController_t emSrcPMId)
{
    uint32_t uiAreaBaseAddr = 0U;                                   /* 数据区域基址         */
    uint32_t uiAreaMaxLen = 0U;                                     /* 数据区域最大长度     */
    int16_t sRetSend = RET_OK;                                      /* 发送数据结果         */
    int32_t iSendLen = 0;                                           /* 发送数据长度         */
    uint32_t uiRecvAck = ACK_DATA_ERR;                              /* 应答数据结果         */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    SyncStaticAck_t stSyncDataAck;                                  /* 应答数据缓存         */

    do
    {
        if((RT_DATA_TYPE_G != pstSyncPrjData->stDataHead.emAreaId) &&
                (RT_DATA_TYPE_I != pstSyncPrjData->stDataHead.emAreaId) &&
                (RT_DATA_TYPE_Q != pstSyncPrjData->stDataHead.emAreaId))
        {
            break;
        }
        /* 获取区域基地址 */
        uiAreaBaseAddr = SysGetSharedMemRtDataStartAddr(pstSyncPrjData->stDataHead.emAreaId);
        memcpy((puint8_t)uiAreaBaseAddr + pstSyncPrjData->stDataHead.uiOffset, \
                pstSyncPrjData->ucData, uiDataLen);
        if(LAST_PACK == pstSyncPrjData->stDataHead.emLastPackFlag)
        {
        }
        uiRecvAck = ACK_DATA_OK;
    }while(NULL);

    /* 打包应答数据 */
    stSyncDataAck.stSyncHead.emSrvType = SYNC_REAL_DATA;
    stSyncDataAck.ucData[0] = uiRecvAck;
    /* 计算发送数据长度 */
    iSendLen = sizeof(stSyncDataAck.stSyncHead) + sizeof(stSyncDataAck.ucData[0]);

    if(emSrcPMId == emNextPMId)
    {
        /* 发送数据 */
        sRetSend = PMBUS_Send(PM_BUS_A, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncDataAck, iSendLen, PMBUS_SEND_TIMEOUT);
        
        if(sRetSend != RET_OK)
        {/* 发送数据失败 */

        }
    }
    else if(emSrcPMId == emPrePMId)
    {
        /* 发送数据 */
        sRetSend = PMBUS_Send(PM_BUS_B, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncDataAck, iSendLen, PMBUS_SEND_TIMEOUT);
        
        if(sRetSend != RET_OK)
        {/* 发送数据失败 */
        }
    }
    else
    {

    }

    return;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncRecvFileOk
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
void SyncRecvFileOk(PMController_t emSrcPMId)
{
//    uint32_t uiRetSend = RET_OK;                                    /* 发送数据结果         */
//    uint32_t uiSendLen = 0U;                                        /* 发送数据长度         */
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
//    SyncStaticAck_t stSyncAck;                                      /* 应答数据缓存         */
////    uint32_t uiCheckRet = SysCheckAllFile();                        /* 校验文件             */
//
//    stSyncAck.stSyncHead.emSrvType = SYNC_COMPLETE;
//    if(SYS_FILE_CHECK_OK == uiCheckRet)
//    {
//        stSyncAck.ucData[0] = ACK_DATA_OK;
//    }
//    else
//    {
//        stSyncAck.ucData[0] = ACK_DATA_ERR;
//    }
//    uiSendLen = sizeof(stSyncAck.stSyncHead) + sizeof(stSyncAck.ucData[0]);
//
//    if(emNextPMId == emSrcPMId)
//    {
//        uiRetSend = PMProtocolSendPrj((puint8_t)&stSyncAck, emLocalPMId, CPU_CON_NEXT,
//                        uiSendLen, SYNC_DATA);
//        if(uiRetSend != RET_OK)
//        {/* 发送数据错误 */
//
//        }
//        else
//        {
//            SyncInitStatus();
//            if(SYS_FILE_CHECK_OK == uiCheckRet)
//            {/* 校验文件 */
//                /* 设置工程有效,待补充 */
//
//                /* 设置工程有效,待补充 */
//
//                SysDeSyncPrjQueue(pstSyncPrjQueue);
//            }
//            else
//            {
//                SysSyncPrjQueueUpdate(TASK_FILE);
//            }
//        }
//    }
//    else if(emPrePMId == emSrcPMId)
//    {
//        uiRetSend = PMProtocolSendPrj((puint8_t)&stSyncAck, emLocalPMId, CPU_CON_PRE,
//                        uiSendLen, SYNC_DATA);
//        if(uiRetSend != RET_OK)
//        {/* 发送数据错误 */
//        }
//        else
//        {
//            SyncInitStatus();
//            if(SYS_FILE_CHECK_OK == uiCheckRet)
//            {/* 校验文件 */
//                /* 设置工程有效,待补充 */
//
//                /* 设置工程有效,待补充 */
//                SysDeSyncPrjQueue(pstSyncPrjQueue);
//            }
//            else
//            {
//                SysSyncPrjQueueUpdate(TASK_FILE);
//            }
//        }
//    }
//    else
//    {
//
//    }

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
void SyncRecvRealDataOk(PMController_t emSrcPMId)
{
//    uint32_t uiSendLen = 0U;                                        /* 发送数据长度         */
//    uint32_t uiRetSend = 0U;                                        /* 发送数据结果         */
//    uint32_t uiRecvLen = 0U;                                        /* 接收数据长度         */
//    uint32_t uiRealDataLen = 0U;                                    /* 实际数据长度         */
//    uint32_t uiRecvAck = ACK_DATA_ERR;                              /* 应答数据             */
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    SyncStaticAck_t stSyncDataAck;                                  /* 应答数据缓存         */
//    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
//    uint8_t ucRecvBuf[MAX_IO_USER_DATA_SIZE];                       /* 接收数据缓存         */
//    SyncData_t* pstSyncData = (SyncData_t*)ucRecvBuf;               /* 接收数据指针         */
//    uint32_t uiRecordCon = 0U;                                      /* 应答记录             */
//
//    /* 清空接收缓存区 */
//    memset(ucRecvBuf, 0U, MAX_IO_USER_DATA_SIZE);
//
//
//    if(ACK_DATA_OK == uiRecvAck)
//    {
//        /* 发送应答 */
//        stSyncDataAck.stSyncHead.emSrvType = SYNC_REAL_DATA;
//        stSyncDataAck.ucData[0] = uiRecvAck;
//        uiSendLen = sizeof(stSyncDataAck.stSyncHead) + sizeof(stSyncDataAck.ucData[0]);
//
//        if((SysGetWorkStatus(emNextPMId) != WS_OFFLINE) && (SysGetWorkStatus(emPrePMId) !=WS_OFFLINE))
//        {
//            uiRetSend = PMProtocolSendPrj((puint8_t)&stSyncDataAck, emLocalPMId, CPU_CON_PRE_AND_NEXT, \
//                            uiSendLen, SYNC_DATA);
//            if(RET_OK != uiRetSend)//
//            {/* 发送错误 */
//
//            }
//            else
//            {
//                SysSetWorkStatus(emLocalPMId, WS_RUN);
//            }
//        }
//        else if(WS_OFFLINE != SysGetWorkStatus(emPrePMId))
//        {
//            uiRetSend = PMProtocolSendPrj((puint8_t)&stSyncDataAck, emLocalPMId, CPU_CON_PRE, \
//                            uiSendLen, SYNC_DATA);
//            if(RET_OK != uiRetSend)
//            {/* 发送错误 */
//
//            }
//            else
//            {
//                SysSetWorkStatus(emLocalPMId, WS_RUN);
//            }
//        }
//        else if(WS_OFFLINE != SysGetWorkStatus(emNextPMId))
//        {
//            uiRetSend = PMProtocolSendPrj((puint8_t)&stSyncDataAck, emLocalPMId, CPU_CON_NEXT, \
//                            uiSendLen, SYNC_DATA);
//            if(RET_OK != uiRetSend)
//            {/* 发送错误 */
//
//            }
//            else
//            {
//                SysSetWorkStatus(emLocalPMId, WS_RUN);
//            }
//        }
//        else
//        {
//        }
//    }


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

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    /* 根据任务状态发送相关数据 */
    switch(stSyncTask.emTaskStatus)
    {
        case TASK_G_DATA:
            /* 发送G区数据 */
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_I_DATA:
            /* 发送I区数据 */
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_Q_DATA:
            /* 发送Q区数据 */
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendData(stSyncTask.emPMId);
            break;
        case TASK_COMPLETE:
            /* 发送静态数据同步完成 */
            s_emLastStatus = stSyncTask.emTaskStatus;
            SyncSendDataOk(stSyncTask.emPMId);
            break;
        case TASK_WAITACK:
            /* 等待应答 */
            SyncWaitAck(stSyncTask.emPMId, s_emLastStatus);
            break;
        default:
            break;
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
void SyncSendData(PMController_t emDstPMId)
{
    int32_t iSendLen = 0;                                               /* 发送数据长度         */
    uint32_t uiRealDataLen = 0U;                                        /* 实际数据长度         */
    int16_t sRetSend = RET_OK;                                          /* 发送数据结果         */
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
    
    
    pstSyncSendData = (SyncRealData_t*)&stSyncPrjData.ucPrjData[0];

    pstTaskConfigInfo = SharedGetTaskConfigInfo(s_stSyncDataStatus.emTaskID);

    /* 初始化数据区基地址 */
    pucAreaBaseAddr = SysGetSharedMemRtDataStartAddr(s_stSyncDataStatus.emAreaId);

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
    }
    if(RT_DATA_TYPE_Q == s_stSyncDataStatus.emAreaId )
    {
        uiAreaUsedLen = pstTaskConfigInfo->uiOUsedSize;
        uiAreaUsedOffset = pstTaskConfigInfo->uiOOffset;
    }
    
    /* 清空数据发送缓存 */
    //memset((puint8_t)&pstSyncSendData, 0U, sizeof(SyncData_t));
    pstSyncSendData->stSyncHead.emSrvType = SYNC_REAL_DATA;

    if(s_stSyncDataStatus.uiCurDataLen >= uiAreaUsedLen)
    {
        if(HAS_NONEXT_DATA == SyncGetNextArea(&s_stSyncDataStatus.emAreaId))
        {
            SysSyncPrjQueueUpdate(TASK_COMPLETE);
        }
        s_stSyncDataStatus.uiCurOffset = 0;
        s_stSyncDataStatus.uiCurDataLen = 0;
    }
    else
    {
        /* 填充数据头 */
        pstSyncSendData->stDataHead.emTaskID = s_stSyncDataStatus.emTaskID;
        pstSyncSendData->stDataHead.emAreaId = s_stSyncDataStatus.emAreaId;
        pstSyncSendData->stDataHead.uiOffset = s_stSyncDataStatus.uiCurOffset;

        /* 计算实际数据长度并填充 */
        uiRealDataLen = MIN(CFG_MAX_SYNC_DATA_SIZE, uiAreaUsedLen - s_stSyncDataStatus.uiCurDataLen);

        s_stSyncDataStatus.uiCurOffset = uiAreaUsedOffset + s_stSyncDataStatus.uiCurDataLen;
        
        memcpy(&pstSyncSendData->ucData[0], (puint8_t)(pucAreaBaseAddr + s_stSyncDataStatus.uiCurOffset), \
                        uiRealDataLen);
        
        s_stSyncDataStatus.uiCurDataLen += uiRealDataLen;
        if(s_stSyncDataStatus.uiCurDataLen >= uiAreaUsedLen)
        {
            pstSyncSendData->stDataHead.emLastPackFlag = LAST_PACK;
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
            /* 发送数据 */
            sRetSend = PMBUS_Send(PM_BUS_A, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjData, iSendLen, PMBUS_SEND_TIMEOUT);
            if(sRetSend != RET_OK)
            {/* 发送错误重新启动数据传输 */
                /*log*/
                SyncInitDataStatus();
            }
            else
            {/* 发送成功 */
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
            }
        }
        else if(emPrePMId == emDstPMId)
        {
            /* 发送数据 */
            sRetSend = PMBUS_Send(PM_BUS_B, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjData, iSendLen, PMBUS_SEND_TIMEOUT);
            if(sRetSend != RET_OK)
            {/* 发送错误重新启动数据传输 */
                /*log*/
                SyncInitDataStatus();
            }
            else
            {/* 发送成功 */
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
            }
        }
        else
        {
            /*log*/
            SysDeSyncPrjQueue(pstSyncPrjQueue);
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
//    int16_t sRetSend = RET_OK;                                      /* 发送数据结果         */
//    int32_t iSendLen = 0U;                                          /* 发送数据长度         */
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);         /* 前一个PMID号     */
//    SyncStaticAck_t stSyncSend;                                     /* 静态数据同步服务缓存 */
//
//    stSyncSend.stSyncHead.emSrvType = SYNC_COMPLETE;
//    uiSendLen = sizeof(stSyncSend.stSyncHead);
//
//    if(emNextPMId == emDstPMId)
//    {
//        uiRetSend = PMProtocolSendData((puint8_t)&stSyncSend, emLocalPMId, CPU_CON_NEXT, \
//                            uiSendLen, SYNC_DATA);
//        if(uiRetSend != RET_OK)
//        {/* 发送数据错误 */
//
//        }
//        else
//        {/* 发送数据成功 */
//
//            SysSyncPrjQueueUpdate(TASK_WAITACK);
//            SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
//        }
//    }
//    else if(emPrePMId == emDstPMId)
//    {
//        uiRetSend = PMProtocolSendData((puint8_t)&stSyncSend, emLocalPMId, CPU_CON_PRE,
//                            uiSendLen, SYNC_DATA);
//        if(uiRetSend != RET_OK)
//        {/* 发送数据错误 */
//
//        }
//        else
//        {/* 发送数据成功 */
//
//            SysSyncPrjQueueUpdate(TASK_WAITACK);
//            SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
//        }
//    }
//    else
//    {
//        /*log: send id error!*/
//    }

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
    uint32_t uiRet = HAS_NEXT_DATA;                                 /* 函数返回值           */

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
    uint8_t ucRetErrNo = RET_OK;                                    /* 接收数据应答错误码   */
    SyncStaticAck_t* pstSyncAck = NULL;                             /* 接收应答数据缓存     */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();      /* 同步任务队列指针     */
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
        case TASK_COMPLETE:
            emRecvSrvType = SYNC_COMPLETE;
            break;
        default:
            break;
    }

    /* 判断接收应答数据 */
    if(emSrcPMId == emNextPMId)
    {
        sRetLen = s_stSyncData.sLinkASrvLen;
        pstSyncAck = (SyncStaticAck_t*)&s_stSyncData.stSyncMSGA.stSyncPrjData.ucPrjData[0];
    }
    if(emSrcPMId == emPrePMId)
    {
        sRetLen = s_stSyncData.sLinkBSrvLen;
        pstSyncAck = (SyncStaticAck_t*)&s_stSyncData.stSyncMSGB.stSyncPrjData.ucPrjData[0];
    }

    if(SYNC_RECV_SIZE_0 == sRetLen)
    {/* 接收到数据长度为0或接收到错误数据 */
        SysGetSyncTime(&unCurTime);

        if(SysDiffSyncTime(&s_stSyncTimer.unTaskStartTime, &unCurTime) >= SYNC_TIMER_TIMEOUT)
        {/* 等待应答超时或接收应答错误 */
            
            DEBUG_SYNC("  RECV ACK TIMEOUT \n");
            
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
            else
            {
                SysSyncPrjQueueUpdate(emLastStatus);
            }
        }
    }
    else
    {/* 接收到应答数据 */
        if((emRecvSrvType != pstSyncAck->stSyncHead.emSrvType) || (pstSyncAck->ucData[0] != ACK_DATA_OK))
        {/* 接收到错误应答数据 */
            
            DEBUG_SYNC("  RECV ACK ERR \n");
            DEBUG_SYNC("  SrvType: %x  RecvSrvType: %x  AckData: %x \n",pstSyncAck->stSyncHead.emSrvType,pstSyncAck->ucData[0]);
            
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
            else
            {
                SyncInitStatus();
                SysSyncPrjQueueUpdate(TASK_FILE);
            }
        }
        else
        {/* 接收到正确应答数据 */
            
            DEBUG_SYNC("  RECV ACK OK \n");
            
            if(emLastStatus != TASK_COMPLETE)
            {
/*test*/
                if(NEW_FILE == s_stSyncFileStatus.emStartFlag)
                {/* 发送新文件 */
                    if(HAS_NONEXT_FILE == SyncGetNextFile(&s_stSyncFileStatus))
                    {/* 文件发送完成，更新同步任务 */
//                        SysSyncPrjQueueUpdate(TASK_COMPLETE);
                        SysDeSyncPrjQueue(pstSyncPrjQueue);
                        
                        memset(&s_stSyncFileStatus, 0x00,sizeof(SyncFileStatus_t));
                        
                        s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
                        
                        SetLEDState(LED_NAME_SYNC, LED_ON);
                        
                        DEBUG_SYNC("  Sync File Compelete !\n");
                    }
                    else
                    {
//                        DEBUG_SYNC("  Next File is: %x !\n",s_stSyncFileStatus.emCfgFileType);
                        
                        SysSyncPrjQueueUpdate(emLastStatus);
                        
                    }
                    /* 新文件已发送数据长度为0 */
                    s_stSyncFileStatus.uiSendSize = 0U;
                }
                else
                {
                    SysSyncPrjQueueUpdate(emLastStatus);
                }
/*test*/
                
//                SysSyncPrjQueueUpdate(emLastStatus);
            }
            else
            {/* 同步完成并且收到应答 */
                /* 初始化同步相关状态 */
                SyncInitStatus();
                /* 设置对方工程有效 ......待补充*/

                /* 清除同步任务 */
                SysDeSyncPrjQueue(pstSyncPrjQueue);
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
    int16_t sRetSend = RET_OK;                                      /* 发送返回值 */
    int32_t iSendLen = 0;                                          /* 发送数据长度 */
    uint32_t uiDataLen = 0U;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PMID号 */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号 */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号 */
    PMController_t emSrcPMId = INVALID_PM_ID;
    SyncFile_t* pstSyncRecvFile = NULL;
    uint8_t ucRecvBuf[MAX_IO_USER_DATA_SIZE];                       /* 接收数据缓存区           */
    SysSyncTask_t stSyncTask;                                       /* 同步任务                 */
    SysCfgFileState_t stCfgFileState;
    SyncPrjData_t* pstSyncPrjData = NULL;
    SyncPrjData_t stSyncPrjDataSend;
    SyncStaticAck_t* pstSyncFileAck;                                  /* 接收文件应答 */
    puint32_t puiTest = NULL;
    puint32_t puiFileConfigFlg = NULL;
    uint16_t usWritDataLen = 0U;
    
    memset(&stSyncPrjDataSend, 0x00, sizeof(SyncPrjData_t));
    
    pstSyncFileAck = (SyncStaticAck_t*)&stSyncPrjDataSend.ucPrjData[0];

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    

    emSrcPMId = stSyncTask.emPMId;
    if(emSrcPMId == emNextPMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncMSGA.stSyncPrjData;
    }
    else if(emSrcPMId == emPrePMId)
    {
        pstSyncPrjData = (SyncPrjData_t*)&s_stSyncData.stSyncMSGB.stSyncPrjData;
    }
    else
    {
        DEBUG_SYNC(" RECV SrcPMId is ERR: %d\n",emSrcPMId);
    }
    if((pstSyncPrjData!=NULL) && (pstSyncPrjData->uiSyncPrjDataLen > 0U))
    {
        do
        {
            
//            puiTest = (puint32_t)pstSyncPrjData;
//            for(uint8_t a =0; a< 10; a++)
//            {
//                printf("  %x ",*puiTest);
//                puiTest++;
//            }
//            printf("\n");
            
            pstSyncRecvFile = (SyncFile_t*)&pstSyncPrjData->ucPrjData[0];
            uiDataLen = pstSyncPrjData->uiSyncPrjDataLen - sizeof(SyncHead_t) - sizeof(SyncFileHead_t);
            if((pstSyncRecvFile->stFileHead.emCfgFileType == s_stSyncFileStatus.emCfgFileType) && 
                    (pstSyncRecvFile->stFileHead.uiFileOffset != 0U))
            {/* 不是接收到新文件 */
                /*将数据写入内存*/
                SysGetCfgFileState(pstSyncRecvFile->stFileHead.emCfgFileType,&stCfgFileState);
                if(stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE)
                {
                    DEBUG_SYNC("  stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE ... \n");
//                    break;
                }
                else
                {
                    usWritDataLen = SysSetCfgFileInfo(pstSyncRecvFile->stFileHead.emCfgFileType,&pstSyncRecvFile->ucData[0], \
                            uiDataLen);
    //                SyncWriteFileContinue(s_stSyncFileStatus.ucFileName,pstSyncRecvFile->ucData, uiDataLen);
                    if(usWritDataLen != uiDataLen)
                    {
                        DEBUG_SYNC("  Write FileData ERR ...\n");
                    }
                    else
                    {
                        s_uiCacheSize += uiDataLen;

                        if(LAST_PACK == pstSyncRecvFile->stFileHead.emLastPackFlag)
                        {/* 接收到文件最后一包数据 */
                            /*设置向内存写文件完成*/
                            SysSetCfgFileFinished(pstSyncRecvFile->stFileHead.emCfgFileType);
                            s_uiCacheSize = 0U;
                           
//                          SyncWriteFileEnd(s_stSyncFileStatus.ucFileName);
                            
                            if(CFG_CTRL_STATION == pstSyncRecvFile->stFileHead.emCfgFileType)
                            {
                                SysSetAllCfgFileFinished();
                                
                                SysDeSyncPrjQueue(pstSyncPrjQueue);
                                
                                SetLEDState(LED_NAME_SYNC, LED_ON);
                                
                                memset(&s_stSyncFileStatus, 0x00,sizeof(SyncFileStatus_t));
                                
                                s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
                                
                                DEBUG_SYNC("  SysDeSyncPrjQueue ...\n");
                            }
                                
                            memset(&s_stSyncFileStatus, 0U, sizeof(SyncFileStatus_t));
                        }
                    }
                }
            }
            else
            {/* 接收到新文件 */
                /* 更新文件名记录 */
                memcpy(s_stSyncFileStatus.ucFileName, pstSyncRecvFile->stFileHead.ucFileName, MAX_FILE_NAME_LEN);
                s_stSyncFileStatus.ucFileName[MAX_FILE_NAME_LEN] = BLANK_CHAR;
                
                s_stSyncFileStatus.emCfgFileType = pstSyncRecvFile->stFileHead.emCfgFileType;
                
                DEBUG_SYNC("  CfgFileType is : %d  \n",s_stSyncFileStatus.emCfgFileType);
                
                if(CFG_PRG == pstSyncRecvFile->stFileHead.emCfgFileType)
                {
                    SetLEDState(LED_NAME_SYNC, LED_BLINK);
                }
                

                SysGetCfgFileState(pstSyncRecvFile->stFileHead.emCfgFileType,&stCfgFileState);
                if(stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE)
                {
                    
                    DEBUG_SYNC("  stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE  \n");
                    
//                    break;
                }
                else
                {
                    if((stCfgFileState.uiLen != 0U) || (stCfgFileState.uiWrOffset != 0U))
                    {/*若文件信息显示文件内容非空，将文件数据删除*/
                        SysDeleteCfgFile(pstSyncRecvFile->stFileHead.emCfgFileType);
                    }
                    
                    puiFileConfigFlg = &pstSyncRecvFile->ucData[0];
                    *puiFileConfigFlg = FILE_NOT_CONFIG;
                    

                    DEBUG_SYNC("  DataLen is : %d \n",uiDataLen);
                    
                    /*将文件数据存入内存*/
                    usWritDataLen = SysSetCfgFileInfo(pstSyncRecvFile->stFileHead.emCfgFileType,&pstSyncRecvFile->ucData[0], \
                            uiDataLen);
//                    SyncWriteFileStart(s_stSyncFileStatus.ucFileName,pstSyncRecvFile->ucData, uiDataLen);
                    s_uiCacheSize += uiDataLen;
                    if(usWritDataLen!= uiDataLen)
                    {
                        DEBUG_SYNC("  Write FileData ERR .\n");
                    }
                    else
                    {
                        if(LAST_PACK == pstSyncRecvFile->stFileHead.emLastPackFlag)
                        {/* 接收到文件最后一包数据 */
                            /*设置向内存写文件完成*/
                            SysSetCfgFileFinished(pstSyncRecvFile->stFileHead.emCfgFileType);
                            s_uiCacheSize = 0U;
                            
//                            DEBUG_SYNC("  SysSetCfgFileFinished : %x \n",pstSyncRecvFile->stFileHead.emCfgFileType);
                            
                            if(CFG_CTRL_STATION == pstSyncRecvFile->stFileHead.emCfgFileType)
                            {
                                SysSetAllCfgFileFinished();
                                
                                SysDeSyncPrjQueue(pstSyncPrjQueue);
                                
                                SetLEDState(LED_NAME_SYNC, LED_ON);
                                
                                memset(&s_stSyncFileStatus, 0x00,sizeof(SyncFileStatus_t));
                                
                                s_stSyncFileStatus.emCfgFileType = INVALID_CFG_FILE_TYPE;
                                
                                DEBUG_SYNC("  SysDeSyncPrjQueue .\n");
                            }
                            
//                                memset(&s_stSyncFileStatus, 0U, sizeof(SyncFileStatus_t));
        //                    SyncWriteFileEnd(s_stSyncFileStatus.ucFileName);
                        }
                    }
                    
                }
            }
            /* 应答OK */
            uiRecvAck = ACK_DATA_OK;
            
        }while(NULL);

        /* 打包应答数据 */
        pstSyncFileAck->stSyncHead.emSrvType = SYNC_FILE;
        pstSyncFileAck->ucData[0] = uiRecvAck;
        /* 计算发送数据长度 */
        iSendLen = sizeof(pstSyncFileAck->stSyncHead) + sizeof(pstSyncFileAck->ucData[0]);
        stSyncPrjDataSend.uiSyncPrjDataLen = iSendLen;
        
        iSendLen += sizeof(stSyncPrjDataSend.uiSyncPrjDataLen);

        if(emNextPMId == emSrcPMId)
        {
            sRetSend = PMBUS_Send(PM_BUS_A, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjDataSend, iSendLen, PMBUS_SEND_TIMEOUT);
            
            if(sRetSend != RET_OK)
            {/* 发送数据失败 */

                DEBUG_SYNC(" SEND ACK TO NEXT ERR : %d   %d \n",sRetSend,iSendLen);
            }
            else
            {
//                DEBUG_SYNC(" SEND ACK TO NEXT ： %x Len is : %d\n",pstSyncFileAck->ucData[0],iSendLen);
                DEBUG_SYNC("  RECV FILE OK, SEND ACK TO NEXT !\n");
            }
        }
        else if(emPrePMId == emSrcPMId)
        {
            sRetSend = PMBUS_Send(PM_BUS_B, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjDataSend, iSendLen, PMBUS_SEND_TIMEOUT);
            
            if(sRetSend != RET_OK)
            {/* 发送数据失败 */
                
                DEBUG_SYNC(" SEND ACK TO PRE ERR : %d   %d \n",sRetSend,iSendLen);

            }
            else
            {
//                DEBUG_SYNC(" SEND ACK TO PRE ： %x Len is : %d\n",pstSyncFileAck->ucData[0],iSendLen);
                DEBUG_SYNC("  RECV FILE OK, SEND ACK TO PRE !\n");
            }
        }
        else
        {
            /* 目标端错误 */
            /*log*/
            
            DEBUG_SYNC(" ACK SEND SrcPMId is ERR: %d\n",emSrcPMId);
            
        }
        
        pstSyncPrjData->uiSyncPrjDataLen = 0U;
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
    uint32_t uiRealDataLen = 0U;                                /* 除去头结构具体数据长度   */
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本机PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);               /* 本机PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                 /* 本机PMID号           */
    SysSyncTask_t stSyncTask;                                   /* 同步任务                 */
    SyncPrjData_t* pstSyncPrjData = NULL;
    SyncRealData_t* pstSyncRealData = NULL;
    int16_t sRecvErr = 0;
    static sync_task_stat_t s_emLastStatus = TASK_WAITACK;            /* 等待应答前状态   */
    
    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

        /* 根据任务状态发送相关数据 */
        switch(stSyncTask.emTaskStatus)
        {
            case TASK_FILE:
                /* 发送工程文件数据 */
                
                DEBUG_SYNC("  SEND_FILE \n");
                s_emLastStatus = stSyncTask.emTaskStatus;
                SyncSendFile(pstSyncPrjQueue);
                break;
            case TASK_WAITACK:
                /* 等待应答 */
                
                DEBUG_SYNC("  TASK_WAITACK \n");
                
                SyncWaitAck(stSyncTask.emPMId, s_emLastStatus);
                break;
            default:
                break;
        }

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
    int16_t sRetSend = RET_OK;                                      /* 发送结果返回值       */
    SysSyncTask_t stSyncTask;                                       /* 同步任务         */
    SysCfgFileState_t stCfgFileState;
    puint32_t puiTest = NULL;
    
    pstSyncSendFile = (SyncFile_t*)&stSyncPrjData.ucPrjData[0];

    /* 获取任务队列任务 */
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);

    emDstPMId = stSyncTask.emPMId;

    /* 清空发送缓存区 */
    //memset((puint8_t)pstSyncSendFile, 0U, sizeof(SyncFile_t));

    do
    {
//        if(NEW_FILE == s_stSyncFileStatus.emStartFlag)
//        {/* 发送新文件 */
//            if(HAS_NONEXT_FILE == SyncGetNextFile(&s_stSyncFileStatus))
//            {/* 文件发送完成，更新同步任务 */
//                SysSyncPrjQueueUpdate(TASK_COMPLETE);
////                SysDeSyncPrjQueue(pstSyncPrjQueue);
//                DEBUG_SYNC("  Sync File Compelete !\n");
//                
//                break;
//            }
//            /* 新文件已发送数据长度为0 */
//            s_stSyncFileStatus.uiSendSize = 0U;
//        }
        
        if(NEW_FILE == s_stSyncFileStatus.emStartFlag)
        {/* 发送新文件 */
            if(INVALID_CFG_FILE_TYPE == s_stSyncFileStatus.emCfgFileType)
            {
                s_stSyncFileStatus.emCfgFileType = CFG_PRG;
                
                /* 新文件已发送数据长度为0 */
                s_stSyncFileStatus.uiSendSize = 0U;
                
                SetLEDState(LED_NAME_SYNC, LED_BLINK);
            }
        }
        
        DEBUG_SYNC("  SEND FILE: CfgFileType is : %d \n",s_stSyncFileStatus.emCfgFileType);
        
        /*获取文件操作状态*/
        SysGetCfgFileState(s_stSyncFileStatus.emCfgFileType, &stCfgFileState);


        if(stCfgFileState.bReadable != CFG_FILE_RD_ENABLE)
        {/* 文件不可读 */
            /*log*/
            s_stSyncFileStatus.emStartFlag = NOT_NEW_FILE;
            
            DEBUG_SYNC("  stCfgFileState.bReadable != CFG_FILE_RD_ENABLE\n");
            
            break;
        }
        /* 填充发送同步数据类型为文件 */
        pstSyncSendFile->stSyncHead.emSrvType = SYNC_FILE;
        iSendLen += sizeof(SyncHead_t);

        /* 填充文件数据头 */
        memcpy(pstSyncSendFile->stFileHead.ucFileName, s_stSyncFileStatus.ucFileName, \
                MAX_FILE_NAME_LEN);
        pstSyncSendFile->stFileHead.uiFileOffset = s_stSyncFileStatus.uiSendSize;
        
        DEBUG_SYNC("  SEND FILE: FileOffset is : %d \n",pstSyncSendFile->stFileHead.uiFileOffset);
        
        pstSyncSendFile->stFileHead.emCfgFileType = s_stSyncFileStatus.emCfgFileType;
        iSendLen += sizeof(SyncFileHead_t);
        
        
        usReadFileLen = MIN(CFG_MAX_SYNC_FILE_SIZE, stCfgFileState.uiLen - s_stSyncFileStatus.uiSendSize);
        
        DEBUG_SYNC("  SEND FILE: ReadFileLen is : %d\n",usReadFileLen);

        usRealFileLen = SysGetCfgFileInfo(pstSyncSendFile->stFileHead.emCfgFileType, \
                pstSyncSendFile->stFileHead.uiFileOffset, &pstSyncSendFile->ucData[0], usReadFileLen);
        
        if(usRealFileLen != usReadFileLen)
        {
            DEBUG_SYNC("  READ FILE ERR !\n");
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
            
            DEBUG_SYNC("  LAST_PACK \n");
            
        }
        else
        {/* 当前文件发送没有完成 */
            s_stSyncFileStatus.emStartFlag = NOT_NEW_FILE;
            pstSyncSendFile->stFileHead.emLastPackFlag = NOT_LAST_PACK;
            
            DEBUG_SYNC("  NOT_LAST_PACK \n");
        }
        
        stSyncPrjData.uiSyncPrjDataLen = iSendLen;
        
        iSendLen += sizeof(stSyncPrjData.uiSyncPrjDataLen);
        
        
//        puiTest = (puint32_t)&stSyncPrjData;
//        for(uint8_t a =0; a< 10; a++)
//        {
//            printf("  %x ",*puiTest);
//            puiTest++;
//        }
//        printf("\n");
        

        if(emNextPMId == emDstPMId)
        {
            sRetSend = PMBUS_Send(PM_BUS_A, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjData, iSendLen, PMBUS_SEND_TIMEOUT);
            if(sRetSend != RET_OK)
            {/* 发送错误重新启动文件传输 */
                /*log*/
                SyncInitFileStatus();
                DEBUG_SYNC("  NEXT SEND FILE ERR \n");
            }
            else
            {/* 发送成功，等待应答 */
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
                
//                DEBUG_SYNC("  NEXT CfgFileType is :%x  Len is %d \n",s_stSyncFileStatus.emCfgFileType,iSendLen);
                DEBUG_SYNC("  SEND FILE TO NEXT OK ! \n");
                
            }
        }
        else if(emPrePMId == emDstPMId)
        {
            sRetSend = PMBUS_Send(PM_BUS_B, PM_RECV_BUFF_PRJ, (puint8_t)&stSyncPrjData, iSendLen, PMBUS_SEND_TIMEOUT);
            if(sRetSend != RET_OK)
            {/* 发送错误重新启动文件传输 */
                /*log*/
                SyncInitFileStatus();
                DEBUG_SYNC("  PRE SEND FILE ERR \n");
            }
            else
            {/* 发送成功，等待应答 */
                SysSyncPrjQueueUpdate(TASK_WAITACK);
                SysGetSyncTime(&s_stSyncTimer.unTaskStartTime);
                
//                DEBUG_SYNC("  PRE CfgFileType is :%x  Len is %d \n",s_stSyncFileStatus.emCfgFileType,iSendLen);
                DEBUG_SYNC("  SEND FILE TO PRE OK ! \n");
                
            }
        }
        else
        {/* 任务ID错误，出队 */
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
    uint32_t uiRet = HAS_NEXT_FILE;                                 /* 函数返回值           */

    switch(pstSyncFileStatus->emCfgFileType)
    {
        case INVALID_CFG_FILE_TYPE:
            uiRet = HAS_NEXT_DATA;
            pstSyncFileStatus->emCfgFileType = CFG_PRG;
            break;
        case CFG_PRG:
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
            pstSyncFileStatus->emCfgFileType = CFG_CM;
            break;
        case CFG_CM:
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


//
//static uint16_t SyncWriteFileStart(uint8_t ucFileName[], uint8_t ucFileData[],uint32_t uiFileDataLen)
//{
//    static FileWriteStag_t emProcessStag = FWRITE_START;
//    uint8_t ucSubSeason = ERR_SUCCESS;
//    static FileWriteInfo_t stFileWriteInfo;
//    uint16_t usACKDataLen = 0;
//    uint32_t uiCalcCRC = 0;
//    FileInfo_t* pstFileInfo = NULL;
//    FileOperateResult_t ucFileOptRst = 0;
//
//    if(NULL == ucFileData)
//    {
//        ucSubSeason = ERR_SRC_ADDR;
//        emProcessStag = FWRITE_END;
//    }
//    if(NULL == ucFileName)
//    {
//        ucSubSeason = ERR_SRC_ADDR;
//        emProcessStag = FWRITE_END;
//    }
//    switch(emProcessStag)
//    {
//        case FWRITE_START:
//        {
//            //save the data in memory
//        }
//        break;
//
//        case FWRITE_DETER_TMPFILE_EXIST:
//        {
//            ucFileOptRst = CommDetermineFileExist(pstFileInfo->ucTmpFileName);
//            if(ucFileOptRst == FILE_EXIST)
//            {
//                emProcessStag = FWRITE_ERASE_TMPFILE;
//            }
//            if(ucFileOptRst == FILE_NOT_EXIST)
//            {
//                emProcessStag = FWRITE_OPEN_TMPFILE;
//            }
//            if(ucFileOptRst == TOME_OUT)
//            {
//                emProcessStag = FWRITE_CLOSE_TMPFILE;
//            }
//        }
//        break;
//
//        case  FWRITE_ERASE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_DELETE,NULL);
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_OPEN_TMPFILE;
//            }
//            if(ucFileOptRst == FAIL)
//            {
//                emProcessStag = FWRITE_CLOSE_TMPFILE;
//            }
//        }
//        break;
//
//        case  FWRITE_OPEN_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_OPEN,NULL);
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_OPEN_TMPFILE;
//            }
//            if(ucFileOptRst == FAIL)
//            {
//                emProcessStag = FWRITE_CLOSE_TMPFILE;//
//            }
//        }
//        break;
//
//        case  FWRITE_WRITE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_WRITE,ucFileData);
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_END;
//
//                ucSubSeason = ERR_SUCCESS;
//
//            }
//            if(ucFileOptRst == FAIL)
//            {
//                emProcessStag = FWRITE_CLOSE_TMPFILE;
//            }
//        }
//        break;
//
//        case FWRITE_CLOSE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_CLOSE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            emProcessStag = FWRITE_DELETE_TMPFILE;
//        }
//        break;
//
//        case FWRITE_DELETE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_DELETE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            ucSubSeason = ERR_WRITE_FILE_START;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//
//        case FWRITE_END:
//        {
//            /*produce ACK data*/
//            emProcessStag = FWRITE_START;
//        }
//        break;
//
//        default:
//        {
//            //record error log
//            ucSubSeason = ERR_WRITE_FILE_START;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//    }
//
//    return usACKDataLen;
//}
//
//static uint16_t SyncWriteFileContinue(puint8_t pucCS1131Data, uint32_t ulCS1131DataLen,puint8_t pucACKData)
//{
//    static FileWriteStag_t emProcessStag = FWRITE_START;
//    static puint8_t pucServiceData = NULL;
//    uint8_t ucSubSeason = ERR_SUCCESS;
//    ACKHead_t* pstACKHead;
//    MSGHead_t* pstMSGHead;
//    static FileWriteInfo_t stFileWriteInfo;
//    uint16_t usACKDataLen = 0;
//    uint32_t uiCalcCRC = 0;
//    static uint8_t ucFileName[MAX_FILE_NAME_LEN];
//    FileInfo_t* pstFileInfo = NULL;
//    uint8_t ucFileOptRst = 0;
//    puint8_t pucACK = pucACKData;
//
//    if(NULL == pucServiceData)
//    {
//        ucSubSeason = ERR_SRC_ADDR;
//        emProcessStag = FWRITE_END;
//    }
//    if(NULL == pucACK)
//    {
//        ucSubSeason = ERR_ACK_ADDR;
//        emProcessStag = FWRITE_END;
//
//    }
//
//    switch(emProcessStag)
//    {/**/
//        case FWRITE_START:
//        {/**/
//            pucServiceData = pucCS1131Data;
//            memset(&ucFileName[0], 0, MAX_FILE_NAME_LEN);
//            memset((puint8_t)&stFileWriteInfo,0x00,sizeof(stFileWriteInfo));
//            //pstMSGHead = (MSGHead_t*)pucServiceData;
//            pucServiceData += sizeof(MSGHead_t);
//            memcpy(&stFileWriteInfo.ucFileType, pucServiceData, sizeof(stFileWriteInfo.ucFileType));
//            pucServiceData += sizeof(stFileWriteInfo.ucFileType);
//            memcpy(&stFileWriteInfo.usFileDataLen, pucServiceData, sizeof(stFileWriteInfo.usFileDataLen));
//            pucServiceData += sizeof(stFileWriteInfo.usFileDataLen);
//            memcpy(&stFileWriteInfo.usFileNameLen, pucServiceData, sizeof(stFileWriteInfo.usFileNameLen));
//            pucServiceData += sizeof(stFileWriteInfo.usFileNameLen);
//            memcpy(&ucFileName[0], pucServiceData, stFileWriteInfo.usFileNameLen);
//            pucServiceData += stFileWriteInfo.usFileNameLen;
//            pstFileInfo = (FileInfo_t*)CommGetFileInfo(&ucFileName[0]);
//            if(pstFileInfo == NULL)
//            {
//                //file name is error ,file info is not exist
//                ucSubSeason = ERR_WRITE_FILE_CONTINUE;
//                emProcessStag = FWRITE_END;
//            }
//            else
//            {
//                //save the data in memory
//                memcpy((puint8_t)(pstFileInfo->pucFileDataAddr+pstFileInfo->uiFileWriteIndex),pucServiceData, \
//                        stFileWriteInfo.usFileDataLen);
//                pstFileInfo->uiFileWriteIndex += stFileWriteInfo.usFileDataLen;
//                emProcessStag = FWRITE_WRITE_TMPFILE;
//            }
//        }
//        break;
//
//        case  FWRITE_WRITE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_WRITE,pucServiceData);
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_END;
//                ucSubSeason = ERR_SUCCESS;
//            }
//            if(ucFileOptRst == FAIL)
//            {
//                emProcessStag = FWRITE_CLOSE_TMPFILE;
//            }
//        }
//        break;
//
//        case FWRITE_CLOSE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_CLOSE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            emProcessStag = FWRITE_DELETE_TMPFILE;
//        }
//        break;
//
//        case FWRITE_DELETE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_DELETE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//
//        case FWRITE_END:
//        {
//            /*produce ACK data*/
//            pstACKHead = (ACKHead_t*)pucACKData;
//            pstMSGHead = (MSGHead_t*)pucCS1131Data;
//            pstACKHead->ucType = pstMSGHead->ucProtect;
//            pstACKHead->ucProtect = pstMSGHead->ucType;
//            pstACKHead->usActualType = pstMSGHead->usActualType;
//            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//            pstACKHead->ucStationID = pstMSGHead->ucStationID;
//            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//            pucACK += sizeof(ACKHead_t);
//            if(ucSubSeason != ERR_SUCCESS)
//            {
//                pstACKHead->ucResCode = RESCODE_FAILED;
//            }
//            else
//            {
//                pstACKHead->ucResCode = RESCODE_SUCCESS;
//            }
//            pstACKHead->usLength = (uint16_t)(pucACK - pucACKData);
//            pstACKHead->ucSubReason = ucSubSeason;
//            uiCalcCRC = SysCrc32Cal( 0, pucACKData, pstACKHead->usLength );
//            memcpy(pucACK,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//            pucACK += sizeof(uiCalcCRC);
//            usACKDataLen = (uint16_t)(pucACK - pucACKData);
//            emProcessStag = FWRITE_START;
//        }
//        break;
//
//        default:
//        {
//            //record error log
//            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//    }
//
//    return usACKDataLen;
//}
//
//static uint16_t SyncWriteFileEnd(puint8_t pucCS1131Data)
//{
//    static FileWriteStag_t emProcessStag = FWRITE_START;
//    static puint8_t pucServiceData = NULL;
//    uint8_t ucSubSeason = ERR_SUCCESS;
//    ACKHead_t* pstACKHead;
//    MSGHead_t* pstMSGHead;
//    static FileWriteInfo_t stFileWriteInfo;
//    uint16_t usACKDataLen = 0;
//    uint32_t uiCalcCRC = 0;
//    static uint8_t ucFileName[MAX_FILE_NAME_LEN];
//    FileInfo_t* pstFileInfo = NULL;
//    uint8_t ucFileOptRst = 0;
//    puint8_t pucACK = pucACKData;
//
//    if(NULL == pucServiceData)
//    {
//        ucSubSeason = ERR_SRC_ADDR;
//        emProcessStag = FWRITE_END;
//    }
//    if(NULL == pucACK)
//    {
//        ucSubSeason = ERR_ACK_ADDR;
//        emProcessStag = FWRITE_END;
//    }
//
//    switch(emProcessStag)
//    {
//        case FWRITE_START:
//        {
//            pucServiceData = pucCS1131Data;
//            memset(&ucFileName[0], 0, MAX_FILE_NAME_LEN);
//            memset((puint8_t)&stFileWriteInfo,0x00,sizeof(stFileWriteInfo));
//            //pstMSGHead = (MSGHead_t*)pucServiceData;
//            pucServiceData += sizeof(MSGHead_t);
//            memcpy(&stFileWriteInfo.ucFileType, pucServiceData, sizeof(stFileWriteInfo.ucFileType));
//            pucServiceData += sizeof(stFileWriteInfo.ucFileType);
//            memcpy(&stFileWriteInfo.usFileNameLen, pucServiceData, sizeof(stFileWriteInfo.usFileNameLen));
//            pucServiceData += sizeof(stFileWriteInfo.usFileNameLen);
//            memcpy(&ucFileName[0], pucServiceData, stFileWriteInfo.usFileNameLen);
//            pucServiceData += stFileWriteInfo.usFileNameLen;
//            pstFileInfo = (FileInfo_t*)CommGetFileInfo(&ucFileName[0]);
//            if(pstFileInfo == NULL)
//            {
//                //file name is error ,file info is not exist
//                ucSubSeason = ERR_WRITE_FILE_END;
//                emProcessStag = FWRITE_END;
//            }
//            else
//            {
//                emProcessStag = FWRITE_WRITE_TMPFILEDONE;
//            }
//        }
//        break;
//
//        case  FWRITE_WRITE_TMPFILEDONE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_CLOSE,pucServiceData);
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_DETER_FORMFILE_EXIST;
//            }
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//                emProcessStag = FWRITE_DELETE_TMPFILE;
//            }
//        }
//        break;
//
//        case FWRITE_DETER_FORMFILE_EXIST:
//        {
//            ucFileOptRst = CommDetermineFileExist(pstFileInfo->ucFileName);
//            if(ucFileOptRst == FILE_EXIST)
//            {
//                emProcessStag = FWRITE_RENAME_FORMFILE;
//            }
//            if(ucFileOptRst == FILE_NOT_EXIST)
//            {
//                emProcessStag = FWRITE_RENAME_TMPFILE;
//            }
//            if(ucFileOptRst == TOME_OUT)
//            {
//                //record error log
//                emProcessStag = FWRITE_CLOSE_FORMFILE;
//            }
//        }
//        break;
//
//        case FWRITE_RENAME_FORMFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucFileName,FWRITE_DETER_FORMFILE_EXIST,pstFileInfo->ucOldFileName);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//                emProcessStag = FWRITE_CLOSE_FORMFILE;
//            }
//            if(ucFileOptRst == SUCCESS)
//            {
//                emProcessStag = FWRITE_RENAME_TMPFILE;
//            }
//        }
//        break;
//
//        case FWRITE_RENAME_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FWRITE_DETER_FORMFILE_EXIST,pstFileInfo->ucFileName);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//                emProcessStag = FWRITE_CLOSE_TMPFILE;
//            }
//            if(ucFileOptRst == SUCCESS)
//            {
//                ucSubSeason = ERR_SUCCESS;
//                pstFileInfo->emActive = ACTIVE;
//                pstFileInfo->uiFileDataLen = pstFileInfo->uiFileWriteIndex;
//                emProcessStag = FWRITE_RENAME_TMPFILE;
//            }
//        }
//        break;
//
//        case FWRITE_CLOSE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_CLOSE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            emProcessStag = FWRITE_DELETE_TMPFILE;
//        }
//        break;
//
//        case FWRITE_DELETE_TMPFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucTmpFileName,FS_DELETE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            ucSubSeason = ERR_WRITE_FILE_END;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//
//        case FWRITE_CLOSE_FORMFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucFileName,FS_CLOSE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            emProcessStag = FWRITE_DELETE_TMPFILE;
//        }
//        break;
//
//        case FWRITE_DELETE_FORMFILE:
//        {
//            ucFileOptRst = CommFileOperate(pstFileInfo->ucFileName,FS_DELETE,NULL);
//            if(ucFileOptRst == FAIL)
//            {
//                //record error log
//            }
//            ucSubSeason = ERR_WRITE_FILE_END;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//
//        case FWRITE_END:
//        {
//            /*produce ACK data*/
//            pstACKHead = (ACKHead_t*)pucACKData;
//            pstMSGHead = (MSGHead_t*)pucCS1131Data;
//            pstACKHead->ucType = pstMSGHead->ucProtect;
//            pstACKHead->ucProtect = pstMSGHead->ucType;
//            pstACKHead->usActualType = pstMSGHead->usActualType;
//            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//            pstACKHead->ucStationID = pstMSGHead->ucStationID;
//            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//            pucACK += sizeof(ACKHead_t);
//            if(ucSubSeason != ERR_SUCCESS)
//            {
//                pstACKHead->ucResCode = RESCODE_FAILED;
//            }
//            else
//            {
//                pstACKHead->ucResCode = RESCODE_SUCCESS;
//            }
//            pstACKHead->usLength = (uint16_t)(pucACK - pucACKData);
//            pstACKHead->ucSubReason = ucSubSeason;
//            uiCalcCRC = SysCrc32Cal( 0, pucACKData, pstACKHead->usLength );
//            memcpy(pucACK,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//            pucACK += sizeof(uiCalcCRC);
//            usACKDataLen = (uint16_t)(pucACK - pucACKData);
//            emProcessStag = FWRITE_START;
//        }
//        break;
//
//        default:
//        {
//            //record error log
//            ucSubSeason = ERR_WRITE_FILE_END;
//            emProcessStag = FWRITE_END;
//        }
//        break;
//
//    }
//
//    return usACKDataLen;
//}
//



/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
