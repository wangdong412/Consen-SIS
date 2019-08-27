/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统状态管理模块
*
*  文件名称    : StateMngt.c
*
*  功能简介    : 完成对系统状态的管理
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-01
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
#include "../inc/StateMngt.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/* 稳态及非稳态码值表是根据三系PM各自工作状态，排序后合并而生成。 */

/* 非稳态状态码表  */
//const static uint32_t s_uiUnstableCode[] = { P_O_O, P_O_R, 
//        P_O_T, P_O_S, P_R_O, P_R_R, P_T_O, P_T_T, P_S_O, P_S_S, T_O_P, T_P_O, 
//        T_P_T, T_T_P, S_O_P, S_P_O, S_P_S, S_S_P, R_O_P, R_P_O, R_P_R, R_R_P, 
//        D_O_O, D_D_O, D_O_D, D_D_D,R_P_P,P_R_P,P_P_R };

/* 非稳态状态码表  */
const static uint32_t s_uiUnstableCode[MAX_UNSTABLE_STATE] = { S_O_W, S_W_O, S_W_W, W_O_S, W_S_O, W_S_W, W_W_S, \
        S_S_W,S_W_S,W_S_S };

/* 稳态状态码表  */
//const static uint32_t s_uiStableCode[MAX_UNSTABLE_STATE] = { S_O_O, S_O_S, S_S_O, S_S_S, W_O_O, W_O_W, W_W_O, W_W_W };


static work_status_t emlastPreWorkStat = WS_OFFLINE;
static work_status_t emlastNextWorkStat = WS_OFFLINE;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
void SysUpdateState(void);

uint32_t SysStateChange(puint32_t puiOldState, puint32_t puiCurState);

uint32_t SysStateReady(void);

uint32_t SysGetStateCode(uint32_t uiLocalStat, uint32_t uiPreStat, uint32_t uiNextStat);

sys_state_t SysInUnStableArray(uint32_t uiStateCode);

void SysUnstableStateHandle(uint32_t uiStateCode);

void SysStableStateHandle(uint32_t uiStateCode);

void SysStateHandle1(void);

void SysStateHandle2(void);

void SysStateHandle3(void);

void SysStateHandle4(void);

void SysStateHandle5(void);

void SysStateHandle6(void);

void SysStateHandle7(void);

void SysStateHandle8(void);

void SysStateHandle9(void);

void SysStateHandle10(void);

void SysStateHandle11(void);

void SysStateHandle12(void);

void SysStateHandle13(void);

void SysStateHandle14(void);


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: StateMngtInit
*
* 功能描述: 系统状态管理初始化。
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
void StateMngtInit(void)
{
    SysStateMngtInit();
    
    emlastPreWorkStat = WS_OFFLINE;
    emlastNextWorkStat = WS_OFFLINE;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateMngt
*
* 功能描述: 系统状态管理函数，周期性检测系统状态，当系统处于非稳态时进行相应处理。
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
void SysStateMngt(void)
{
    /* 静态变量记录上一拍运行状态信息 */
    static uint32_t s_uiOldState[NUM_OF_PM] = {WS_OFFLINE, WS_OFFLINE, WS_OFFLINE};     /* 三冗余PM上拍状态   */

    uint32_t uiState[NUM_OF_PM] = {WS_OFFLINE, WS_OFFLINE, WS_OFFLINE};                 /* 当前三冗余PM状态   */
    uint32_t uiSortState[NUM_OF_PM] = {WS_OFFLINE, WS_OFFLINE, WS_OFFLINE};             /* 排序后三冗余PM状态 */
    uint32_t uiStateCode = 0U;                                                          /* 状态码值               */
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();                          /* 工程任务队列           */

//    printf("  SysStateMngt  \n");
    

    /* 获取三系PM当前工作状态 */
    uiState[PMA] = SysGetWorkStatus(PMA);
    uiState[PMB] = SysGetWorkStatus(PMB);
    uiState[PMC] = SysGetWorkStatus(PMC);
    
    
    
    uiSortState[CMP_LOCAL] = SysGetWorkStatus(emLocalPMId);
    uiSortState[CMP_PRE] = SysGetWorkStatus(emPrePMId);
    uiSortState[CMP_NEXT] = SysGetWorkStatus(emNextPMId);

    /* 根据排序后三冗余PM状态生成系统状态码值 */
    uiStateCode = SysGetStateCode(uiSortState[CMP_LOCAL], uiSortState[CMP_PRE], uiSortState[CMP_NEXT]);
    

    if(SYS_STATE_CHANGED == SysStateChange(&s_uiOldState[PMA], &uiState[PMA]))
    {
    /*log*/
        if(pstSyncPrjQueue!= NULL)
        {
            if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
            {
                SysDeSyncPrjQueue(pstSyncPrjQueue);
            }
        }
    }
    

    if(SYS_UNSTABLE_STATE == SysInUnStableArray(uiStateCode))
    {/* 若系统状态为非稳态 */

        if(SYS_SYNC_STATE_READY == SysStateReady())
        {
            if(SYS_STATE_CHANGED == SysStateChange(&s_uiOldState[PMA], &uiState[PMA]))
            {/* 判断系统状态是否发生改变，即三个PM的状态是否发生改变 */
            /*log*/
                
                DEBUG_STATMNGT("SYS_UNSTABLE_STATE %x\n",uiStateCode);
//                DEBUG_STATMNGT("State %x\n",uiStateCode);
                
                /* 设置当前系统处于非稳态 */
                SysSetState(SYS_UNSTABLE_STATE);
            }
            
            /* 调用非稳态处理函数 */
            SysUnstableStateHandle(uiStateCode);
        }
    }

    if(SYS_UNSTABLE_STATE != SysInUnStableArray(uiStateCode))
    {/* 若系统状态处于稳定状态 */
        /* 调用稳态处理函数 */
        SysStableStateHandle(uiStateCode);
        /* 设置当前系统处于稳定状态 */
        SysSetState(SYS_STABLE_STATE);
        
        if(SYS_STATE_CHANGED == SysStateChange(&s_uiOldState[PMA], &uiState[PMA]))
        {
            DEBUG_STATMNGT("SYS_STABLE_STATE %x\n",uiStateCode);
//            DEBUG_STATMNGT("State %x\n",uiStateCode);
        }
        
        /* 调用稳态处理函数 */
        SysStableStateHandle(uiStateCode);
    }

    emlastPreWorkStat = SysGetWorkStatus(emPrePMId);
    emlastNextWorkStat = SysGetWorkStatus(emNextPMId);
    s_uiOldState[PMA] = uiState[PMA];
    s_uiOldState[PMB] = uiState[PMB];
    s_uiOldState[PMC] = uiState[PMC];
    
    /* 硬件诊断更新三系PM状态，判断PM是否离线 */
    SysUpdateState();
    
//    printf("  SysStateMngt END  \n");
    

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SysUpdateState
*
* 功能描述: 系统状态更新。
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
void SysUpdateState(void)
{
    uint16_t usState = 0U;                                                              /* 硬件状态值           */
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();                          /* 同步任务队列         */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();                          /* 同步任务队列         */
    SysPMStateInfo_t* pstLocalPMState = NULL;
    SysPMStateInfo_t* pstNextPMState = NULL;
    SysPMStateInfo_t* pstPrePMState = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;                                 /*控制站配置信息可读片段指针*/
    
    
    static uint8_t sucTest = 0U;

//    /*PM升主使能*/
//    SysSetTimerMasterEnable(MASTER_ENABLE_VAR);
//    /*PM软件运行OK*/
//    SysSetSWRuninOK(SW_RUNING_OK_VAR);
    
    /* 获取本机工程信息 */
    pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
    pstNextPMState = SysGetPMStateInfo(emNextPMId);
    pstPrePMState = SysGetPMStateInfo(emPrePMId);
    /*获取控制站配置信息可写片段指针*/
    pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    

    if((HAS_BE_CONFIGURED == SysGetIsConfigured()) && 
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP1)) && 
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP2)))
    {
        SysSetWorkStatus(emLocalPMId,WS_WORK);
    }
    else
    {
        SysSetWorkStatus(emLocalPMId,WS_STANDBY);
    }

    if(pstLocalPMState != NULL)
    {
        if(pstCtrlStationInfo != NULL)
        {
            if(HAS_BE_CONFIGURED == SysGetIsConfigured())
            {
                pstLocalPMState->stPrjInfo.uiPrjID = pstCtrlStationInfo->stPrjConfigInfo.uiProjectID;
                pstLocalPMState->stPrjInfo.uiMainVern = pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion;
                pstLocalPMState->stPrjInfo.uiMinorVern = pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion;
                pstLocalPMState->stPrjInfo.uiPrjCRC = pstCtrlStationInfo->uiCRC;
                pstLocalPMState->stPrjInfo.ucIsConfigured = HAS_BE_CONFIGURED;
                pstLocalPMState->stPrjInfo.ucIsIOConfigured = IPBUSGetDeviceConfigSlaveFlag();

//                sucTest++;
//                if(sucTest > 100U)
//                {
//                    printf("DEV C:%x\n",IPBUSGetDeviceConfigSlaveFlag());
//                    
//                    sucTest = 0U;
//                }
                if((pstLocalPMState->stDLPrjInfo.uiPrjID == pstLocalPMState->stPrjInfo.uiPrjID) && \
                   (pstLocalPMState->stDLPrjInfo.uiMainVern == pstLocalPMState->stPrjInfo.uiMainVern) && \
                   (pstLocalPMState->stDLPrjInfo.uiMinorVern == pstLocalPMState->stPrjInfo.uiMinorVern) && \
                   (pstLocalPMState->stDLPrjInfo.uiPrjCRC == pstLocalPMState->stPrjInfo.uiPrjCRC))
                {/*当下装配置生效，将下装配置信息清空*/
                    pstLocalPMState->stDLPrjInfo.uiPrjID = 0U;
                    pstLocalPMState->stDLPrjInfo.uiMainVern = 0U;
                    pstLocalPMState->stDLPrjInfo.uiMinorVern = 0U;
                    pstLocalPMState->stDLPrjInfo.uiPrjCRC = 0U;
                    pstLocalPMState->stDLPrjInfo.ucIsConfigured = NOT_HAS_BE_CONFIGURED;
                    pstLocalPMState->stDLPrjInfo.ucIsIOConfigured = IPBUS_DEVICE_NOCONFIG_SLAVE;
                    pstNextPMState->stDLPrjInfo.uiPrjID = 0U;
                    pstNextPMState->stDLPrjInfo.uiMainVern = 0U;
                    pstNextPMState->stDLPrjInfo.uiMinorVern = 0U;
                    pstNextPMState->stDLPrjInfo.uiPrjCRC = 0U;
                    pstNextPMState->stDLPrjInfo.ucIsConfigured = NOT_HAS_BE_CONFIGURED;
                    pstNextPMState->stDLPrjInfo.ucIsIOConfigured = IPBUS_DEVICE_NOCONFIG_SLAVE;
                    pstPrePMState->stDLPrjInfo.uiPrjID = 0U;
                    pstPrePMState->stDLPrjInfo.uiMainVern = 0U;
                    pstPrePMState->stDLPrjInfo.uiMinorVern = 0U;
                    pstPrePMState->stDLPrjInfo.uiPrjCRC = 0U;
                    pstPrePMState->stDLPrjInfo.ucIsConfigured = NOT_HAS_BE_CONFIGURED;
                    pstPrePMState->stDLPrjInfo.ucIsIOConfigured = IPBUS_DEVICE_NOCONFIG_SLAVE;
                }
            }
        }
        
        if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
        {
            pstLocalPMState->emSyncStatus = SS_SYNC;
        }
        else
        {
            pstLocalPMState->emSyncStatus = SS_NOSYNC;
        }
        pstLocalPMState->emSwitchKeys = SharedGetSwitchPos();
        pstLocalPMState->emDownloadFlg = SharedGetDownloadFlg();
        pstLocalPMState->emWorkStatus = SysGetWorkStatus(emLocalPMId);
        pstLocalPMState->emTaskStatus[TASK_UP1] = SysGetTaskStatus(emLocalPMId,TASK_UP1);
        pstLocalPMState->emTaskStatus[TASK_UP2] = SysGetTaskStatus(emLocalPMId,TASK_UP2);
        pstLocalPMState->emPoweronFlg = PM_CYCLE;
        
        if(HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured)
        {/*在下装过程中当解析完成后，记录IO向下配置状态*/
            pstLocalPMState->stDLPrjInfo.ucIsIOConfigured = IPBUSGetDeviceConfigSlaveFlag();
        }
    }
    


    
    /* 获取硬件提供状态值 */
    usState = SysGetModularOnLine();
    
    if(!(usState & BITMASK(0)))
    {

        if(SysGetWorkStatus(emNextPMId) != WS_OFFLINE)
        {
            /*log,PM离线*/
            
            DEBUG_STATMNGT("NEXT OFFLINE\n");

        }
        /* 更新下一个PM状态 */
        SysSetWorkStatus(emNextPMId, WS_OFFLINE);
        
        if(pstNextPMState != NULL)
        {
            pstNextPMState->emWorkStatus = SysGetWorkStatus(emNextPMId);
        }

        /* 清空工程同步队列 */
        SysClearSyncPrjQueue(pstSyncPrjQueue, emNextPMId);
        /* 清空任务同步队列 */
        SysClearSyncSrvQueue(pstSyncSrvQueue, emNextPMId);
    }
    
    if(!(usState & BITMASK(1)))
    {
        
        if(SysGetWorkStatus(emPrePMId) != WS_OFFLINE)
        {
            /*log,前一系PM离线*/
            
            DEBUG_STATMNGT("PRE OFFLINE\n");
            
        }
        /* 更新下一个PM状态 */
        SysSetWorkStatus(emPrePMId, WS_OFFLINE);
        
        if(pstPrePMState != NULL)
        {
            pstPrePMState->emWorkStatus = SysGetWorkStatus(emPrePMId);
        }

        /* 清空工程同步队列 */
        SysClearSyncPrjQueue(pstSyncPrjQueue, emPrePMId);
        /* 清空任务同步队列 */
        SysClearSyncSrvQueue(pstSyncSrvQueue, emPrePMId);
    }
    
    
    if((WS_OFFLINE == SysGetWorkStatus(emNextPMId)) && (WS_OFFLINE == SysGetWorkStatus(emPrePMId)))
    {
        SetLEDState(LED_NAME_PMBUS, LED_OFF);
        SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);
        SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
    }
    else
    {
        SetLEDState(LED_NAME_PMBUS, LED_BLINK);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateChange
*
* 功能描述: 系统状态变化判断函数，用于判断系统状态是否发生变化。
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
uint32_t SysStateChange(puint32_t puiOldState, puint32_t puiCurState)
{
    uint32_t uiRet = SYS_STATE_NOCHANGED;
    uint32_t uiCount = 0;

    /* verify state is changed or not */
    for(uiCount = 0; uiCount < NUM_OF_PM; uiCount++)
    {
        if(puiOldState[uiCount] != puiCurState[uiCount])
        {
            uiRet = SYS_STATE_CHANGED;
            break;
        }
    }

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateReady
*
* 功能描述: 判断系统是否有新的PM加入，是否处于可同步状态。
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
uint32_t SysStateReady(void)
{
//    uint16_t usState = 0U;                                          /* 硬件状态值           */
    uint32_t uiRetRecvOk = SYS_SYNC_STATE_READY;                    /* 返回值               */
//    PMController_t emWaitPMId = SysGetWaitPMId();

//    if((emWaitPMId != INVALID_PM_ID) && (WS_OFFLINE == SysGetWorkStatus(emWaitPMId)) )
//    {
//        uiRetRecvOk = SYS_SYNC_STATE_NOTREADY;
//    }
//    else
//    {
//        SysSetWaitPMId(INVALID_PM_ID);
//        uiRetRecvOk = SYS_SYNC_STATE_READY;
//    }
    
    uiRetRecvOk = SYS_SYNC_STATE_READY;

    return uiRetRecvOk;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetStateCode
*
* 功能描述: 将三系PM状态整合为码值。
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
uint32_t SysGetStateCode(uint32_t uiLocalStat, uint32_t uiPreStat, uint32_t uiNextStat)
{
    uint32_t uiRetStateCode = 0U;

    uiRetStateCode = (uiLocalStat * 0x10000) + (uiPreStat * 0x100) + uiNextStat;

    return uiRetStateCode;
}
/*
***********************************************************************************************************************
* 函数名称: SysInUnStableArray
*
* 功能描述: 判断系统状态为稳态或非稳态。
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
sys_state_t SysInUnStableArray(uint32_t uiStateCode)
{
    sys_state_t emRetState = SYS_STABLE_STATE;
    uint32_t uiCount = 0U;

    for(uiCount = 0U; uiCount < MAX_UNSTABLE_STATE; uiCount++)
    {
        if(uiStateCode == s_uiUnstableCode[uiCount])
        {
            emRetState = SYS_UNSTABLE_STATE;
            break;
        }
    }

    return emRetState;
}

/*
***********************************************************************************************************************
* 函数名称: SysUnstableStateHandle
*
* 功能描述: 非稳态处理函数。
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
void SysUnstableStateHandle(uint32_t uiStateCode)
{
    switch(uiStateCode)
    {
        /*接收后一系工程，接收实时数据前，需要暂停系统*/
        case S_O_W:
        {
            SysStateHandle3();
        }
        break;
        /*接收前一系工程，接收实时数据前，需要暂停系统*/
        case S_W_O:
        case S_W_W:
        case S_W_S:
        {
            SysStateHandle5();
        }
        break;
        /*向后一系发送工程，同步实时数据前，需要暂停系统*/
        case W_O_S:
        case W_W_S:
        case W_S_S:
        {
            SysStateHandle9();
        }
        break;
        /*向前一系发送工程，同步实时数据前，需要暂停系统*/
        case W_S_O:
        {
            SysStateHandle10();
        }
        break;
        /*不处理*/
        case W_S_W:
        case S_S_W:
        break;
    
        default:
        /*记录日志*/
        break;
    }


    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStableStateHandle
*
* 功能描述: 稳态处理函数。
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
void SysStableStateHandle(uint32_t uiStateCode)
{
    PMController_t emLocalPMId = SysGetLocalPMID();                         /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                  /* 前一个PMID号     */
    SysPMStateInfo_t *pstLocalPMState = SysGetPMStateInfo(emLocalPMId);     /* 本机状态信息数据指针     */
    SysPMStateInfo_t *pstNextPMState = SysGetPMStateInfo(emNextPMId);       /* 后一系状态信息数据指针     */
    SysPMStateInfo_t *pstPrePMState = SysGetPMStateInfo(emPrePMId);         /* 前一系状态信息数据指针     */
    SysSrvInfo_t* pstUP1CtrlSrvInfo = NULL;                                 /* 系统资源上服务信息         */
    SysSyncTask_t stSyncTask;                                               /* 同步任务             */
    MSGHead_t stMSGHead;
    uint32_t uiBuffIndex = 0U;
    SyncTime64_t stSyncCurTime;                                             /* 时间结构体               */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();              /* 同步任务队列         */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();              /* 同步工程队列         */
    uint8_t ucSyncSwitchSrvFlg = 0U;
    
    pstUP1CtrlSrvInfo = SysGetCtrlSrvInfo(TASK_UP1);
    SysGetSyncTime(&stSyncCurTime);                                         /* 获取当前时间             */
    
    /* 获取任务队列任务 */
    memset(&stSyncTask, 0x00, sizeof(SysSyncTask_t));
    SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
    
    switch(uiStateCode)
    {/* 根据状态码值调用相应处理函数 */
        case S_O_O:
        case W_O_O:
        {
            if((INIT_DOWNLOAD == SharedGetDownloadFlg()) || (ONLINE_DOWNLOAD == SharedGetDownloadFlg()))
            {
                if((emLocalPMId == SharedGetDownloadPM()) && \
                   (HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (IPBUS_DEVICE_CONFIG_SLAVE == pstLocalPMState->stDLPrjInfo.ucIsIOConfigured))
                {/*发送三系同步切换命令*/
                    ucSyncSwitchSrvFlg = 0x01;
                }
            }
        }
        break;
        case S_O_S:
        case W_O_W:
        {
            if((INIT_DOWNLOAD == SharedGetDownloadFlg()) || (ONLINE_DOWNLOAD == SharedGetDownloadFlg()))
            {/*初始化下装时，产生同步*/
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured))
                {/*向后一系发送工程*/
                    SysStateHandle11();
                }
                
                if((HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured))
                {/*接收后一系工程*/
                    SysStateHandle12();
                }
                
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured) && \
                   (pstNextPMState->stDLPrjInfo.uiPrjID == pstLocalPMState->stDLPrjInfo.uiPrjID) && \
                   (pstNextPMState->stDLPrjInfo.uiMainVern == pstLocalPMState->stDLPrjInfo.uiMainVern) && \
                   (pstNextPMState->stDLPrjInfo.uiMinorVern == pstLocalPMState->stDLPrjInfo.uiMinorVern) && \
                   (pstNextPMState->stDLPrjInfo.uiPrjCRC == pstLocalPMState->stDLPrjInfo.uiPrjCRC))
                {/*发送三系同步切换命令*/
//                    DEBUG_STATMNGT("DeSyncPrj\n");
                    SysDeSyncPrjQueue(pstSyncPrjQueue);
                    if((emLocalPMId == SharedGetDownloadPM()) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstLocalPMState->stDLPrjInfo.ucIsIOConfigured) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstNextPMState->stDLPrjInfo.ucIsIOConfigured))
                    {
                        ucSyncSwitchSrvFlg = 0x01;
                    }
                }
            }
        }
        break;
        
        case S_S_O:
        case W_W_O:
        {
            if((INIT_DOWNLOAD == SharedGetDownloadFlg()) || (ONLINE_DOWNLOAD == SharedGetDownloadFlg()))
            {/*初始化下装时，产生同步*/
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured))
                {/*向前一系发送工程*/
                    SysStateHandle13();
                }
                
                if((HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured))
                {/*接收前一系工程*/
                    SysStateHandle14();
                }
                
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured) && \
                   (pstPrePMState->stDLPrjInfo.uiPrjID == pstLocalPMState->stDLPrjInfo.uiPrjID) && \
                   (pstPrePMState->stDLPrjInfo.uiMainVern == pstLocalPMState->stDLPrjInfo.uiMainVern) && \
                   (pstPrePMState->stDLPrjInfo.uiMinorVern == pstLocalPMState->stDLPrjInfo.uiMinorVern) && \
                   (pstPrePMState->stDLPrjInfo.uiPrjCRC == pstLocalPMState->stDLPrjInfo.uiPrjCRC))
                {/*发送三系同步切换命令*/
                    
                    SysDeSyncPrjQueue(pstSyncPrjQueue);
//                    DEBUG_STATMNGT("DeSyncPrj\n");
                    if((emLocalPMId == SharedGetDownloadPM()) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstLocalPMState->stDLPrjInfo.ucIsIOConfigured) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstPrePMState->stDLPrjInfo.ucIsIOConfigured))
                    {
                        ucSyncSwitchSrvFlg = 0x01;
                    }
                }
            }
        }
        break;

        case S_S_S:
        case W_W_W:
        {
            if((INIT_DOWNLOAD == SharedGetDownloadFlg()) || (ONLINE_DOWNLOAD == SharedGetDownloadFlg()))
            {/*初始化下装时，产生同步*/
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured))
                {/*向后一系发送工程*/
                    SysStateHandle11();
                }
                
                if((HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured))
                {
                    if((SYS_SEND_FILE == stSyncTask.emTaskId) && (emNextPMId == stSyncTask.emPMId))
                    {
                        SysDeSyncPrjQueue(pstSyncPrjQueue);
//                        DEBUG_STATMNGT("DeSyncPrj\n");
                    }
                }
                
                if((HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured) && \
                   (NOT_HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured))
                {/*接收前一系工程*/
                    SysStateHandle14();
                }
                
                if((HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured))
                {
                    if((SYS_RECV_FILE == stSyncTask.emTaskId) && (emPrePMId == stSyncTask.emPMId))
                    {
                        SysDeSyncPrjQueue(pstSyncPrjQueue);
//                        DEBUG_STATMNGT("DeSyncPrj\n");
                    }
                }
                
                if((emLocalPMId == SharedGetDownloadPM()) && \
                   (HAS_BE_CONFIGURED == pstLocalPMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstPrePMState->stDLPrjInfo.ucIsConfigured) && \
                   (HAS_BE_CONFIGURED == pstNextPMState->stDLPrjInfo.ucIsConfigured) && \
                   (pstNextPMState->stDLPrjInfo.uiPrjID == pstLocalPMState->stDLPrjInfo.uiPrjID) && \
                   (pstNextPMState->stDLPrjInfo.uiMainVern == pstLocalPMState->stDLPrjInfo.uiMainVern) && \
                   (pstNextPMState->stDLPrjInfo.uiMinorVern == pstLocalPMState->stDLPrjInfo.uiMinorVern) && \
                   (pstNextPMState->stDLPrjInfo.uiPrjCRC == pstLocalPMState->stDLPrjInfo.uiPrjCRC) && \
                   (pstPrePMState->stDLPrjInfo.uiPrjID == pstLocalPMState->stDLPrjInfo.uiPrjID) && \
                   (pstPrePMState->stDLPrjInfo.uiMainVern == pstLocalPMState->stDLPrjInfo.uiMainVern) && \
                   (pstPrePMState->stDLPrjInfo.uiMinorVern == pstLocalPMState->stDLPrjInfo.uiMinorVern) && \
                   (pstPrePMState->stDLPrjInfo.uiPrjCRC == pstLocalPMState->stDLPrjInfo.uiPrjCRC))
                {/*发送三系同步切换命令*/
                    if((IPBUS_DEVICE_CONFIG_SLAVE == pstLocalPMState->stDLPrjInfo.ucIsIOConfigured) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstPrePMState->stDLPrjInfo.ucIsIOConfigured) && \
                       (IPBUS_DEVICE_CONFIG_SLAVE == pstNextPMState->stDLPrjInfo.ucIsIOConfigured))
                    {
                        ucSyncSwitchSrvFlg = 0x01;
                    }
                    else
                    {
                        printf("DEV %x %x %x\n",pstLocalPMState->stDLPrjInfo.ucIsIOConfigured,\
                                pstPrePMState->stDLPrjInfo.ucIsIOConfigured,pstPrePMState->stDLPrjInfo.ucIsIOConfigured);
                    }
                }
            }
        }
        break;

        default:
        /*记录日志*/
        break;
    }

    if(0x01 == ucSyncSwitchSrvFlg)
    {
        if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
           (SRV_NEED_PROCESS != pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
        {
            printf("SWITCH SRV %x\n",uiStateCode);
            
            stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
            stSyncTask.emPMId = INVALID_PM_ID;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_SEND;
            stSyncTask.emUPTaskId = TASK_UP1;

            uiBuffIndex = 0;
            memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
            
            pstUP1CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_PRJ_SWITCH;
            stMSGHead.ucType =0xAA;
            stMSGHead.ucProtect = 0xBB;
            stMSGHead.usActualType = SYS_PRJ_SWITCH;
            stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
            
            memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                    sizeof(MSGHead_t));
            uiBuffIndex += sizeof(MSGHead_t);
            memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                    sizeof(stSyncTask.emUPTaskId));
            uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
            pstUP1CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;
            SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                              &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
            SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                              &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
            pstUP1CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

            SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);

            pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle1
*
* 功能描述: 非稳态处理函数1，处理状态为：P_O_O  。
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
void SysStateHandle1(void)
{/*系统状态是：P_O_O*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地控制器ID号       */
//    Key_Switch_Type_t emKeySwitchValue = KS_INVALID_TYPE;
//
//
//    emKeySwitchValue = SharedGetSwitchPos();
//    switch(emKeySwitchValue)
//    {
//        case KS_INIT:
//        {
//
//        }
//        break;
//
//        case KS_STOP:
//        case KS_PROGRAM:
//        case KS_RUN:
//        {
//            if(((PRG_FILE_EXIST) == SysGetPrjFileExist()) && (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
//            {/*工程文件存在,并且配置信息存在*/
//                SysSetWorkStatus(emLocalPMId,WS_WORK);
//            }
//            else
//            {
//                SysSetWorkStatus(emLocalPMId,WS_STANDBY);
//            }
//        }
//        break;
//
//        default:
//        break;
//    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle2
*
* 功能描述: 非稳态处理函数2，处理状态为：P_S_O、P_S_S、P_O_S。
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
void SysStateHandle2(void)
{
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地控制器ID号       */
//
//
//    if((PRG_FILE_EXIST) == SysGetPrjFileExist() || (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
//    {/*工程文件存在，或配置信息存在，说明有工程，清除工程*/
//        /*清除所有工程信息，文件，配置等*/
//    	
////        SysClearProject();//待补充。。。。。。
//
//    }
//    SysSetWorkStatus(emLocalPMId,WS_STANDBY);
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle3
*
* 功能描述: 非稳态处理函数3，处理状态为：S_O_W。
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
void SysStateHandle3(void)
{/*接收后一系数据,同步工程区数据前系统需暂停*/
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);            /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                                  /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                                   /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();          /* 同步工程队列         */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();          /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                           /* 同步任务             */
    SysSrvInfo_t* pstUP1CtrlSrvInfo = NULL;                             /* 系统资源上服务信息         */
    SysSrvInfo_t* pstUP2CtrlSrvInfo = NULL;                             /* 系统资源上服务信息         */
    SyncTime64_t stSyncCurTime;                                         /* 时间结构体               */
    MSGHead_t stMSGHead;
    uint32_t uiBuffIndex = 0U;
    SysPMStateInfo_t *pstPMStateInfo = SysGetPMStateInfo(emNextPMId);   /* 本机工程信息数据指针     */
    SysPMStateInfo_t* pstLocalPMState = NULL;
    
    SysGetSyncTime(&stSyncCurTime);                                     /* 获取当前时间             */
    
    pstUP1CtrlSrvInfo = SysGetCtrlSrvInfo(TASK_UP1);
    pstUP2CtrlSrvInfo = SysGetCtrlSrvInfo(TASK_UP2);

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    if((pstLocalPrjInfo != NULL) && (pstNextPrjInfo != NULL) && (pstUP1CtrlSrvInfo != NULL) && \
       (pstUP2CtrlSrvInfo != NULL) && (pstPMStateInfo != NULL))
    {
        if(((pstNextPrjInfo->uiPrjID != pstLocalPrjInfo->uiPrjID) || (pstNextPrjInfo->uiMainVern != pstLocalPrjInfo->uiMainVern) || \
           (pstNextPrjInfo->uiMinorVern != pstLocalPrjInfo->uiMinorVern) || (pstNextPrjInfo->uiPrjCRC != pstLocalPrjInfo->uiPrjCRC)) ||
           (SYS_PRJ_CONFIGURED != pstLocalPrjInfo->ucIsConfigured))
        {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/

            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                
                SysCfgFileInit();
                pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
                
                memset(&pstLocalPMState->stPrjInfo, 0x00, sizeof(PrjInfo_t));
                memset(&pstLocalPMState->stDLPrjInfo, 0x00, sizeof(PrjInfo_t));
                
                SysSetIsConfigured(NOT_HAS_BE_CONFIGURED);

                SysSetTaskStatus(emLocalPMId,TASK_UP1, TS_INIT);
                SysSetTaskStatus(emLocalPMId,TASK_UP2, TS_INIT);
                
                SharedConfigAreaInit();
                
                SharedSetSyncDataFlag(TASK_UP1,DATA_NOT_SYNC);
                SharedSetSyncDataFlag(TASK_UP2,DATA_NOT_SYNC);
                
                stSyncTask.emTaskId = SYS_RECV_FILE;
                stSyncTask.emPMId = emNextPMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);

                DEBUG_STATMNGT("RECV_FILE\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
            
            if((SYS_RECV_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                SysDeSyncPrjQueue(pstSyncPrjQueue);
            }
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
            {
                if(SharedGetPauseFlag(TASK_UP1) != PF_PAUSE)
                {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
                    if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                       (SRV_NEED_PROCESS != pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                    {
                        stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                        stSyncTask.emPMId = INVALID_PM_ID;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_SEND;
                        stSyncTask.emUPTaskId = TASK_UP1;
                        
                        uiBuffIndex = 0;
                        memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                        
                        pstUP1CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_PAUSE;
                        stMSGHead.ucType =0xAA;
                        stMSGHead.ucProtect = 0xBB;
                        stMSGHead.usActualType = SYS_UP_PAUSE;
                        stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                        
                        memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                sizeof(MSGHead_t));
                        uiBuffIndex += sizeof(MSGHead_t);
                        memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                sizeof(stSyncTask.emUPTaskId));
                        uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                        pstUP1CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;
                        SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                          &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                        SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                          &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                        pstUP1CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        
                        SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                        
                        pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                        
                    }
                }
                else
                {/* 同步工程数据区，从G区*/
                    
                    
                    
                    if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
                    {
                        stSyncTask.emTaskId = SYS_RECV_REALDATA;
                        stSyncTask.emPMId = emNextPMId;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_G_DATA;
                        stSyncTask.emUPTaskId = TASK_UP1;
                        SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);
                    }
                }
            }
            else
            {/*队列中存在同步实时数据服务*/
                /* 获取当前任务队列中队首元素 */
                SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
                
                if((SYS_RECV_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                   (TASK_UP2 == stSyncTask.emUPTaskId))
                {/*同步到UP2的实时数据完成*/

                    if(PF_CONTINUE != SharedGetPauseFlag(TASK_UP2))
                    {/*若任务状态未恢复，添加同步任务恢复状态*/
                        if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                           (SRV_NEED_PROCESS != pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                        {
                            stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                            stSyncTask.emPMId = INVALID_PM_ID;
                            stSyncTask.emTaskWait = TASK_NOWAIT;
                            stSyncTask.emTaskStatus = TASK_SEND;
                            stSyncTask.emUPTaskId = TASK_UP2;
                            
                            uiBuffIndex = 0;
                            memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                            
                            pstUP2CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_CONTINUE;
                            stMSGHead.ucType =0xAA;
                            stMSGHead.ucProtect = 0xBB;
                            stMSGHead.usActualType = SYS_UP_CONTINUE;
                            stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                            
                            memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                    sizeof(MSGHead_t));
                            uiBuffIndex += sizeof(MSGHead_t);
                            memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                    sizeof(stSyncTask.emUPTaskId));
                            uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                            pstUP2CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;

                            SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                            SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                            pstUP2CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                            
                            
                            SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                            
                            SysSetTaskStatus(emLocalPMId, TASK_UP2, pstPMStateInfo->emTaskStatus[TASK_UP2]);
                            
                            DEBUG_STATMNGT("Sync UP2 Stat\n");
                            
                            pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                        }
                    }
                    else
                    {
                        SysDeSyncPrjQueue(pstSyncPrjQueue);
                        SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
                        DEBUG_STATMNGT("SYNC OK\n");
                    }
                }
                else
                {
                    if((SYS_RECV_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                       (TASK_UP1 == stSyncTask.emUPTaskId))
                    {/*同步到UP1的实时数据完成*/
                        
                        if(PF_CONTINUE != SharedGetPauseFlag(TASK_UP1))
                        {/*若任务状态未恢复，添加同步任务恢复状态*/
                            if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                               (SRV_NEED_PROCESS != pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                            {
                                stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                                stSyncTask.emPMId = INVALID_PM_ID;
                                stSyncTask.emTaskWait = TASK_NOWAIT;
                                stSyncTask.emTaskStatus = TASK_SEND;
                                stSyncTask.emUPTaskId = TASK_UP1;
                                
                                
                                uiBuffIndex = 0;
                                memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                                
                                pstUP1CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_CONTINUE;
                                stMSGHead.ucType =0xAA;
                                stMSGHead.ucProtect = 0xBB;
                                stMSGHead.usActualType = SYS_UP_CONTINUE;
                                stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                                
                                memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                        sizeof(MSGHead_t));
                                uiBuffIndex += sizeof(MSGHead_t);
                                memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                        sizeof(stSyncTask.emUPTaskId));
                                uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                                pstUP1CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;

                                SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                                  &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                                SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                                  &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                                pstUP1CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                                
                                SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                                
                                SysSetTaskStatus(emLocalPMId, TASK_UP1, pstPMStateInfo->emTaskStatus[TASK_UP1]);
								DEBUG_STATMNGT("Sync UP1 Stat\n");
                                
                                pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                
                            }
                        }
                        else
                        {/*UP1数据同步完成并恢复，开始同步UP2数据*/
                            if(SharedGetPauseFlag(TASK_UP2) != PF_PAUSE)
                            {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
                                if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                                   (SRV_NEED_PROCESS != pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                                {
                                    stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                                    stSyncTask.emPMId = INVALID_PM_ID;
                                    stSyncTask.emTaskWait = TASK_NOWAIT;
                                    stSyncTask.emTaskStatus = TASK_SEND;
                                    stSyncTask.emUPTaskId = TASK_UP2;
                                    
                                    
                                    uiBuffIndex = 0;
                                    memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                                    
                                    pstUP2CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_PAUSE;
                                    stMSGHead.ucType =0xAA;
                                    stMSGHead.ucProtect = 0xBB;
                                    stMSGHead.usActualType = SYS_UP_PAUSE;
                                    stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                                    
                                    memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                            sizeof(MSGHead_t));
                                    uiBuffIndex += sizeof(MSGHead_t);
                                    memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                            sizeof(stSyncTask.emUPTaskId));
                                    uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                                    pstUP2CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;

                                    SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                                    SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                                    pstUP2CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                                    
                                    SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);

                                    pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                }
                            }
                            else
                            {/*UP2暂停之后， 同步UP2的工程数据区，从G区*/
                                
                                stSyncTask.emTaskId = SYS_RECV_REALDATA;
                                stSyncTask.emPMId = emNextPMId;
                                stSyncTask.emTaskWait = TASK_NOWAIT;
                                stSyncTask.emTaskStatus = TASK_G_DATA;
                                stSyncTask.emUPTaskId = TASK_UP2;

                                SysDeSyncPrjQueue(pstSyncPrjQueue);
                                SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);

                                SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);
                                /**/
                            }
                        }
                    }
                }
            }
        }
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle4
*
* 功能描述: 非稳态处理函数4，处理状态为：P_O_T。
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
void SysStateHandle4(void)
{/*接收后一系数据,同步工程区数据前系统不需暂停*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
//    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
//    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
//    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
//
//    /* 获取本机工程信息 */
//    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
//    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);
//    if((pstLocalPrjInfo != NULL)&&(pstNextPrjInfo != NULL))
//    {
//        if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
//           (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
//           (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
//           (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
//        {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
//            {/* 若同步任务队列无同步工程文件任务 */
//                stSyncTask.emTaskId = SYS_RECV_FILE;
//                stSyncTask.emPMId = emNextPMId;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_FILE;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//    //    else
//    //    {/* 同步工程数据区，从G区*/
//    //        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//    //        {
//    //            stSyncTask.emTaskId = SYS_RECV_REALDATA;
//    //            stSyncTask.emPMId = emNextPMId;
//    //            stSyncTask.emTaskWait = TASK_NOWAIT;
//    //            stSyncTask.emTaskStatus = TASK_G_DATA;
//    //            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    //        }
//    //        else
//    //        {
//    //            if(SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured)
//    //            {
//    //                SysSetWorkStatus(emLocalPMId,WS_STOP);
//    //            }
//    //        }
//    //    }
//    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle5
*
* 功能描述: 非稳态处理函数5，处理状态为：S_W_O、S_W_W。
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
void SysStateHandle5(void)
{/*接收前一系数据,同步工程区数据前系统需暂停*/
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();      /* 同步工程队列         */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();      /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
    SysSrvInfo_t* pstUP1CtrlSrvInfo = NULL;                         /* 系统资源上服务信息         */
    SysSrvInfo_t* pstUP2CtrlSrvInfo = NULL;                         /* 系统资源上服务信息         */
    SyncTime64_t stSyncCurTime;                                     /* 时间结构体               */
    MSGHead_t stMSGHead;
    uint32_t uiBuffIndex = 0U;
    SysPMStateInfo_t *pstPMStateInfo = SysGetPMStateInfo(emPrePMId);  /* 本机工程信息数据指针     */
    SysPMStateInfo_t* pstLocalPMState = NULL;
    
    SysGetSyncTime(&stSyncCurTime);                                 /* 获取当前时间             */
    
    pstUP1CtrlSrvInfo = SysGetCtrlSrvInfo(TASK_UP1);
    pstUP2CtrlSrvInfo = SysGetCtrlSrvInfo(TASK_UP2);

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);
    
    if((pstLocalPrjInfo != NULL) && (pstPrePrjInfo != NULL) && (pstUP1CtrlSrvInfo != NULL) && \
       (pstUP2CtrlSrvInfo != NULL) && (pstPMStateInfo != NULL))
    {
        if(((pstPrePrjInfo->uiPrjID != pstLocalPrjInfo->uiPrjID) || (pstPrePrjInfo->uiMainVern != pstLocalPrjInfo->uiMainVern) || \
           (pstPrePrjInfo->uiMinorVern != pstLocalPrjInfo->uiMinorVern) || (pstPrePrjInfo->uiPrjCRC != pstLocalPrjInfo->uiPrjCRC)) ||
           (SYS_PRJ_CONFIGURED != pstLocalPrjInfo->ucIsConfigured))
         {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/

             if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
             {/* 若同步任务队列无同步工程文件任务 */
                 
                 SysCfgFileInit();
                 pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
                 
                 memset(&pstLocalPMState->stPrjInfo, 0x00, sizeof(PrjInfo_t));
                 memset(&pstLocalPMState->stDLPrjInfo, 0x00, sizeof(PrjInfo_t));
                 
                 SysSetIsConfigured(NOT_HAS_BE_CONFIGURED);

                 SysSetTaskStatus(emLocalPMId,TASK_UP1, TS_INIT);
                 SysSetTaskStatus(emLocalPMId,TASK_UP2, TS_INIT);
                 
                 SharedConfigAreaInit();
                 
                 SharedSetSyncDataFlag(TASK_UP1,DATA_NOT_SYNC);
                 SharedSetSyncDataFlag(TASK_UP2,DATA_NOT_SYNC);
                 
                 stSyncTask.emTaskId = SYS_RECV_FILE;
                 stSyncTask.emPMId = emPrePMId;
                 stSyncTask.emTaskWait = TASK_NOWAIT;
                 stSyncTask.emTaskStatus = TASK_FILE;
                 SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);

                 DEBUG_STATMNGT("RECV_FILE\n");
             }
         }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
            
            if((SYS_RECV_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                
                DEBUG_STATMNGT("RECV_FILE OK\n");
                
                SysDeSyncPrjQueue(pstSyncPrjQueue);
            }
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
            {
                if(SharedGetPauseFlag(TASK_UP1) != PF_PAUSE)
                {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
                    if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                       (SRV_NEED_PROCESS != pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                    {
                        stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                        stSyncTask.emPMId = INVALID_PM_ID;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_SEND;
                        stSyncTask.emUPTaskId = TASK_UP1;
                        
                        
                        uiBuffIndex = 0;
                        memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                        
                        pstUP1CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_PAUSE;
                        stMSGHead.ucType =0xAA;
                        stMSGHead.ucProtect = 0xBB;
                        stMSGHead.usActualType = SYS_UP_PAUSE;
                        stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                        
                        memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                sizeof(MSGHead_t));
                        uiBuffIndex += sizeof(MSGHead_t);
                        memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                sizeof(stSyncTask.emUPTaskId));
                        uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                        pstUP1CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;
                        
                        SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                          &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                        SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                          &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                        pstUP1CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                        
                        SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                        
                        pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                        
                        
                        DEBUG_STATMNGT("UP1_PAUSE\n");
                        
                    }
                }
                else
                {/* 同步工程数据区，从G区*/

                    if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
                    {
                        stSyncTask.emTaskId = SYS_RECV_REALDATA;
                        stSyncTask.emPMId = emPrePMId;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_G_DATA;
                        stSyncTask.emUPTaskId = TASK_UP1;
                        SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);
                        
                        DEBUG_STATMNGT("RECV_REALDATA UP1. Time %d\n",stSyncCurTime.stTimeStruct.uiLow32b);
                        
                    }
                }
            }
            else
            {/*队列中存在同步实时数据服务*/
                /* 获取当前任务队列中队首元素 */
                SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
                
                if((SYS_RECV_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                   (TASK_UP2 == stSyncTask.emUPTaskId))
                {/*同步到UP2的实时数据完成，将UP2恢复*/
                    if(PF_CONTINUE != SharedGetPauseFlag(TASK_UP2))
                    {/*若任务状态未恢复，添加同步任务恢复状态*/
                        if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                           (SRV_NEED_PROCESS != pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                        {
                            stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                            stSyncTask.emPMId = INVALID_PM_ID;
                            stSyncTask.emTaskWait = TASK_NOWAIT;
                            stSyncTask.emTaskStatus = TASK_SEND;
                            stSyncTask.emUPTaskId = TASK_UP2;
                            
                            uiBuffIndex = 0;
                            memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                            
                            pstUP2CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_CONTINUE;
                            stMSGHead.ucType =0xAA;
                            stMSGHead.ucProtect = 0xBB;
                            stMSGHead.usActualType = SYS_UP_CONTINUE;
                            stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                            
                            memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                    sizeof(MSGHead_t));
                            uiBuffIndex += sizeof(MSGHead_t);
                            memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                    sizeof(stSyncTask.emUPTaskId));
                            uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                            pstUP2CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;
                            
                            SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                            SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                            pstUP2CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                            
                            SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                            
                            SysSetTaskStatus(emLocalPMId, TASK_UP2,pstPMStateInfo->emTaskStatus[TASK_UP2]);
                            DEBUG_STATMNGT("Sync UP2 Stat\n");
                            
                            pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                            DEBUG_STATMNGT("UP2_CONTINUE\n");
                        }
                    }
                    else
                    {
                        SysDeSyncPrjQueue(pstSyncPrjQueue);
                        SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
                        DEBUG_STATMNGT("SYNC OK\n");
                    }
                }
                else
                {
                    if((SYS_RECV_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                       (TASK_UP1 == stSyncTask.emUPTaskId))
                    {/*同步传输UP1数据完成，将UP1恢复*/
                        if(PF_CONTINUE != SharedGetPauseFlag(TASK_UP1))
                        {/*若任务1状态未恢复，添加同步任务恢复状态*/
                            if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                               (SRV_NEED_PROCESS != pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                            {
                                stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                                stSyncTask.emPMId = INVALID_PM_ID;
                                stSyncTask.emTaskWait = TASK_NOWAIT;
                                stSyncTask.emTaskStatus = TASK_SEND;
                                stSyncTask.emUPTaskId = TASK_UP1;
                                
                                uiBuffIndex = 0;
                                memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                                
                                pstUP1CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_CONTINUE;
                                stMSGHead.ucType =0xAA;
                                stMSGHead.ucProtect = 0xBB;
                                stMSGHead.usActualType = SYS_UP_CONTINUE;
                                stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                                
                                memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                        sizeof(MSGHead_t));
                                uiBuffIndex += sizeof(MSGHead_t);
                                memcpy(&pstUP1CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                        sizeof(stSyncTask.emUPTaskId));
                                uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                                pstUP1CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;
                                
                                SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                                  &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                                SysCalRelativeTime(&pstUP1CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                                  &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                                pstUP1CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                                
                                SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                                
                                SysSetTaskStatus(emLocalPMId, TASK_UP1,pstPMStateInfo->emTaskStatus[TASK_UP1]);
                                DEBUG_STATMNGT("Sync UP1 Stat\n");
                                
                                pstUP1CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                
                                DEBUG_STATMNGT("UP1_CONTINUE\n");
                            }
                        }
                        else
                        {/*若任务1状态恢复，开始同步UP2实时数据*/
                            if(SharedGetPauseFlag(TASK_UP2) != PF_PAUSE)
                            {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
                                if((SYS_QUEUE_NOEXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLSRV)) && 
                                   (SRV_NEED_PROCESS != pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag))
                                {
                                    stSyncTask.emTaskId = SYS_BROADCAST_CTRLSRV;
                                    stSyncTask.emPMId = INVALID_PM_ID;
                                    stSyncTask.emTaskWait = TASK_NOWAIT;
                                    stSyncTask.emTaskStatus = TASK_SEND;
                                    stSyncTask.emUPTaskId = TASK_UP2;
                                    
                                    uiBuffIndex = 0;
                                    memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                                    
                                    pstUP2CtrlSrvInfo->stSysSrvData.usSrvNo = SYS_UP_PAUSE;
                                    stMSGHead.ucType =0xAA;
                                    stMSGHead.ucProtect = 0xBB;
                                    stMSGHead.usActualType = SYS_UP_PAUSE;
                                    stMSGHead.usLength = sizeof(MSGHead_t) + sizeof(stSyncTask.emUPTaskId);
                                    
                                    memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stMSGHead, \
                                            sizeof(MSGHead_t));
                                    uiBuffIndex += sizeof(MSGHead_t);
                                    memcpy(&pstUP2CtrlSrvInfo->stSysSrvData.ucSrvData[uiBuffIndex], &stSyncTask.emUPTaskId, \
                                            sizeof(stSyncTask.emUPTaskId));
                                    uiBuffIndex += sizeof(stSyncTask.emUPTaskId);
                                    pstUP2CtrlSrvInfo->stSysSrvData.uiSrvLen = uiBuffIndex;

                                    SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                                    SysCalRelativeTime(&pstUP2CtrlSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                                    pstUP2CtrlSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                                    
                                    pstUP2CtrlSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                    
                                    
                                    SysEnSyncSrvQueue(pstSyncSrvQueue, &stSyncTask);
                                    
                                    DEBUG_STATMNGT("UP2_PAUSE\n");
                                    
                                }
                            }
                            else
                            {/*当UP1实时数据同步完成并UP2暂停之后， 同步UP2的工程数据区，从G区*/

                                stSyncTask.emTaskId = SYS_RECV_REALDATA;
                                stSyncTask.emPMId = emPrePMId;
                                stSyncTask.emTaskWait = TASK_NOWAIT;
                                stSyncTask.emTaskStatus = TASK_G_DATA;
                                stSyncTask.emUPTaskId = TASK_UP2;
                                
                                SysDeSyncPrjQueue(pstSyncPrjQueue);
                                SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);
                                SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);
                                
                                DEBUG_STATMNGT("RECV_REALDATA UP2. Time %d\n",stSyncCurTime.stTimeStruct.uiLow32b);
                                

                            }
                        }
                    }
                }
            }
        }
    }
    
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle6
*
* 功能描述: 非稳态处理函数6，处理状态为：P_T_O、P_T_T。
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
void SysStateHandle6(void)
{/*接收前一系数据,同步工程区数据前系统不需暂停*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
//    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
//    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
//    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
//
//    /* 获取本机工程信息 */
//    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
//    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);
//
//    if((pstLocalPrjInfo != NULL)&&(pstPrePrjInfo != NULL))
//    {
//        if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
//           (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
//           (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
//           (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
//        {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
//            {/* 若同步任务队列无同步工程文件任务 */
//                stSyncTask.emTaskId = SYS_RECV_FILE;
//                stSyncTask.emPMId = emPrePMId;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_FILE;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//    //    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//    //    {/* 同步工程数据区，从G区*/
//    //            stSyncTask.emTaskId = SYS_RECV_REALDATA;
//    //            stSyncTask.emPMId = emPrePMId;
//    //            stSyncTask.emTaskWait = TASK_NOWAIT;
//    //            stSyncTask.emTaskStatus = TASK_G_DATA;
//    //            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    //    }
//    //    else
//    //    {
//    //        if(SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured)
//    //        {
//    //            SysSetWorkStatus(emLocalPMId,WS_STOP);
//    //        }
//    //    }
//    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle7
*
* 功能描述: 非稳态处理函数7，处理状态为：T_O_P、T_T_P。
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
void SysStateHandle7(void)
{/*向后一系发送数据,同步工程区数据前系统不需暂停*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
//    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
//    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
//    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
//
//    /* 获取本机工程信息 */
//    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
//    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);
//
//    
//    if((pstLocalPrjInfo != NULL)&&(pstNextPrjInfo != NULL))
//    {
//        if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
//           (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
//           (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
//           (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
//        {/*后一系工程文件无效，或者工程信息与本机有任何不一致，需要同步工程文件*/
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
//            {/* 若同步任务队列无同步工程文件任务 */
//                stSyncTask.emTaskId = SYS_SEND_FILE;
//                stSyncTask.emPMId = emNextPMId;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_FILE;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//    //    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    //    {/* 同步工程数据区，从G区*/
//    //        stSyncTask.emTaskId = SYS_SEND_REALDATA;
//    //        stSyncTask.emPMId = emNextPMId;
//    //        stSyncTask.emTaskWait = TASK_NOWAIT;
//    //        stSyncTask.emTaskStatus = TASK_G_DATA;
//    //        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    //    }
//    //    else
//    //    {
//    //
//    //    }
//    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle8
*
* 功能描述: 非稳态处理函数8，处理状态为：T_P_O。
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
void SysStateHandle8(void)
{/*向前一系发送数据,同步工程区数据前系统不需暂停*/
//    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
//    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
//    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
//    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
//    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
//    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
//    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
//
//    /* 获取本机工程信息 */
//    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
//    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);
//
//    if((pstLocalPrjInfo != NULL)&&(pstPrePrjInfo != NULL))
//    {
//        if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
//           (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
//           (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
//           (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
//        {/*前一系工程文件无效，或者其工程信息与本机有任何不一致，需要同步工程文件*/
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
//            {/* 若同步任务队列无同步工程文件任务 */
//                stSyncTask.emTaskId = SYS_SEND_FILE;
//                stSyncTask.emPMId = emPrePMId;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_FILE;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//    //    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    //    {/* 同步工程数据区，从G区*/
//    //        stSyncTask.emTaskId = SYS_SEND_REALDATA;
//    //        stSyncTask.emPMId = emPrePMId;
//    //        stSyncTask.emTaskWait = TASK_NOWAIT;
//    //        stSyncTask.emTaskStatus = TASK_G_DATA;
//    //        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    //    }
//    //    else
//    //    {
//    //
//    //    }
//    }
    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysStateHandle9
*
* 功能描述: 非稳态处理函数9，处理状态为：W_O_S、W_W_S。
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
void SysStateHandle9(void)
{/*向后一系发送，同步工程区数据前，需要暂停系统*/
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
    SyncTime64_t stSyncCurTime;                                     /* 时间结构体               */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    SysGetSyncTime(&stSyncCurTime);                                 /* 获取当前时间             */
    
    if((pstLocalPrjInfo != NULL) && (pstNextPrjInfo != NULL) && (pstSyncQueue != NULL))
    {
        if(((pstLocalPrjInfo->uiPrjID != 0) || (pstLocalPrjInfo->uiMainVern != 0) ||
           (pstLocalPrjInfo->uiMinorVern != 0) || (pstLocalPrjInfo->uiPrjCRC != 0))  && 
           
           ((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) || (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) || 
           (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) || (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC)))
        {/*后一系工程文件无效，或者工程信息与本机有任何不一致，需要同步工程文件*/
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                stSyncTask.emTaskId = SYS_SEND_FILE;
                stSyncTask.emPMId = emNextPMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
            
            if((SYS_SEND_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                
                DEBUG_STATMNGT("SEND_FILE OK\n");
                
                SysDeSyncPrjQueue(pstSyncQueue);
            }
            
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
            {
                if(PF_PAUSE == SharedGetPauseFlag(TASK_UP1))
                {/* 同步工程数据前，若系统处于暂停状态，则开始同步工程数据区，从G区开始 */
                    stSyncTask.emTaskId = SYS_SEND_REALDATA;
                    stSyncTask.emPMId = emNextPMId;
                    stSyncTask.emTaskWait = TASK_NOWAIT;
                    stSyncTask.emTaskStatus = TASK_G_DATA;
                    stSyncTask.emUPTaskId = TASK_UP1;
                    SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                    
                    DEBUG_STATMNGT("SEND_DATA UP1 Time %d\n",stSyncCurTime.stTimeStruct.uiLow32b);
                }
            }
            else
            {
                /* 获取当前任务队列中队首元素 */
                SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
                
                if((SYS_SEND_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                   (TASK_UP1 == stSyncTask.emUPTaskId))
                {/*同步到UP1的实时数据完成，开始同步UP2的数据*/
                    
                    if(PF_PAUSE == SharedGetPauseFlag(TASK_UP2))
                    {/* 同步工程数据前，若系统处于暂停状态，则开始同步工程数据区，从G区开始 */
                        stSyncTask.emTaskId = SYS_SEND_REALDATA;
                        stSyncTask.emPMId = emNextPMId;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_G_DATA;
                        stSyncTask.emUPTaskId = TASK_UP2;
                        
                        SysDeSyncPrjQueue(pstSyncQueue);
                        
                        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                        
                        DEBUG_STATMNGT("SEND_DATA UP2  Time %d\n",stSyncCurTime.stTimeStruct.uiLow32b);
                    }
                }
                else
                {
                    if((SYS_SEND_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                       (TASK_UP2 == stSyncTask.emUPTaskId))
                    {/*同步到UP2的实时数据完成，删除工程同步任务*/
                        SysDeSyncPrjQueue(pstSyncQueue);
                        
                        DEBUG_STATMNGT("SEND_DATA OK \n");
                    }
                }
            }
        }
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysStateHandle10
*
* 功能描述: 非稳态处理函数10，处理状态为：W_S_O。
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
void SysStateHandle10(void)
{/*向前一系发送，同步工程区数据前，需要暂停系统*/
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);

    if((pstLocalPrjInfo != NULL)&&(pstPrePrjInfo != NULL)&&(pstSyncQueue != NULL))
    {
        if(((pstLocalPrjInfo->uiPrjID != 0) || (pstLocalPrjInfo->uiMainVern != 0) ||
           (pstLocalPrjInfo->uiMinorVern != 0) || (pstLocalPrjInfo->uiPrjCRC != 0))  && 
           
           ((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) || (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) || 
           (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) || (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC)))
        {/*前一系工程文件无效，或者工程信息与本机有任何不一致，需要同步工程文件*/
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                stSyncTask.emTaskId = SYS_SEND_FILE;
                stSyncTask.emPMId = emPrePMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);

                DEBUG_STATMNGT("SEND_FILE !\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
            
            if((SYS_SEND_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                SysDeSyncPrjQueue(pstSyncQueue);
            }
            
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
            {
                if(PF_PAUSE == SharedGetPauseFlag(TASK_UP1))
                {/* 同步工程数据前，若系统处于暂停状态，则开始同步工程数据区，从G区开始 */
                    stSyncTask.emTaskId = SYS_SEND_REALDATA;
                    stSyncTask.emPMId = emPrePMId;
                    stSyncTask.emTaskWait = TASK_NOWAIT;
                    stSyncTask.emTaskStatus = TASK_G_DATA;
                    stSyncTask.emUPTaskId = TASK_UP1;
                    SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                    DEBUG_STATMNGT("SEND_DATA UP1\n");
                }
            }
            else
            {
                /* 获取当前任务队列中队首元素 */
                SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
                
                if((SYS_SEND_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                   (TASK_UP1 == stSyncTask.emUPTaskId))
                {/*同步到UP1的实时数据完成，开始同步UP2的数据*/
                    if(PF_PAUSE == SharedGetPauseFlag(TASK_UP2))
                    {/* 同步工程数据前，若系统处于暂停状态，则开始同步工程数据区，从G区开始 */
                        
                        
                        stSyncTask.emTaskId = SYS_SEND_REALDATA;
                        stSyncTask.emPMId = emPrePMId;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        stSyncTask.emTaskStatus = TASK_G_DATA;
                        stSyncTask.emUPTaskId = TASK_UP2;
                        SysDeSyncPrjQueue(pstSyncQueue);
                        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                        
                        DEBUG_STATMNGT("SEND_DATA UP2\n");
                    }
                }
                else
                {
                    if((SYS_SEND_REALDATA == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus) && \
                       (TASK_UP2 == stSyncTask.emUPTaskId))
                    {/*同步到UP2的实时数据完成，删除工程同步任务*/
                        SysDeSyncPrjQueue(pstSyncQueue);
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle12
*
* 功能描述: 稳态处理函数12
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
void SysStateHandle12(void)
{/*接收后一系工程*/
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PrjInfo_t *pstLocalDLPrjInfo = NULL;                                /* 本机下装工程信息数据指针     */
    PrjInfo_t *pstNextDLPrjInfo = NULL;                                 /* 后一系下装工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();          /* 同步工程队列         */
    SysSyncTask_t stSyncTask;                                           /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
    pstNextDLPrjInfo = SysGetDownLoadPrjInfo(emNextPMId);

    if((pstLocalDLPrjInfo != NULL) && (pstNextDLPrjInfo != NULL) && (pstSyncPrjQueue != NULL))
    {
        if((pstNextDLPrjInfo->uiPrjID != pstLocalDLPrjInfo->uiPrjID) || \
           (pstNextDLPrjInfo->uiMainVern != pstLocalDLPrjInfo->uiMainVern) || \
           (pstNextDLPrjInfo->uiMinorVern != pstLocalDLPrjInfo->uiMinorVern) || \
           (pstNextDLPrjInfo->uiPrjCRC != pstLocalDLPrjInfo->uiPrjCRC))
        {/*本机下装工程信息与后一系有任何不一致，需要接收同步工程文件*/

            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                
                stSyncTask.emTaskId = SYS_RECV_FILE;
                stSyncTask.emPMId = emNextPMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);

                DEBUG_STATMNGT("RECV FILE\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
            
            if((SYS_RECV_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                SysDeSyncPrjQueue(pstSyncPrjQueue);
            }
        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle14
*
* 功能描述: 稳态处理函数14
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
void SysStateHandle14(void)
{/*接收前一系工程*/
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PMID号     */
    PrjInfo_t *pstLocalDLPrjInfo = NULL;                                /* 本机下装工程信息数据指针     */
    PrjInfo_t *pstPreDLPrjInfo = NULL;                                  /* 前一系下装工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();          /* 同步工程队列         */
    SysSyncTask_t stSyncTask;                                           /* 同步任务             */

    

    /* 获取本机工程信息 */
    pstLocalDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
    pstPreDLPrjInfo = SysGetDownLoadPrjInfo(emPrePMId);

    if((pstLocalDLPrjInfo != NULL) && (pstPreDLPrjInfo != NULL) && (pstSyncPrjQueue != NULL))
    {
        if((pstPreDLPrjInfo->uiPrjID != pstLocalDLPrjInfo->uiPrjID) || \
           (pstPreDLPrjInfo->uiMainVern != pstLocalDLPrjInfo->uiMainVern) || \
           (pstPreDLPrjInfo->uiMinorVern != pstLocalDLPrjInfo->uiMinorVern) || \
           (pstPreDLPrjInfo->uiPrjCRC != pstLocalDLPrjInfo->uiPrjCRC))
        {/*本机下装工程信息与后一系有任何不一致，需要接收同步工程文件*/

            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                
                stSyncTask.emTaskId = SYS_RECV_FILE;
                stSyncTask.emPMId = emPrePMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncPrjQueue, &stSyncTask);

                DEBUG_STATMNGT("RECV FILE\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncPrjQueue, &stSyncTask);
            
            if((SYS_RECV_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                SysDeSyncPrjQueue(pstSyncPrjQueue);
            }
        }
    }
    
    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysStateHandle11
*
* 功能描述: 稳态处理函数11
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
void SysStateHandle11(void)
{/*向后一系发送工程文件*/
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PrjInfo_t *pstLocalDLPrjInfo = NULL;                            /* 本机下装工程信息数据指针     */
    PrjInfo_t *pstNextDLPrjInfo = NULL;                             /* 后一系下装工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
    SyncTime64_t stSyncCurTime;                                     /* 时间结构体               */

    /* 获取本机工程信息 */
    pstLocalDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
    pstNextDLPrjInfo = SysGetDownLoadPrjInfo(emNextPMId);

    SysGetSyncTime(&stSyncCurTime);                                 /* 获取当前时间             */
    
    if((pstLocalDLPrjInfo != NULL) && (pstNextDLPrjInfo != NULL) && (pstSyncQueue != NULL))
    {
        if(((pstLocalDLPrjInfo->uiPrjID != 0) || (pstLocalDLPrjInfo->uiMainVern != 0) || \
           (pstLocalDLPrjInfo->uiMinorVern != 0) || (pstLocalDLPrjInfo->uiPrjCRC != 0))  &&
           
           ((pstLocalDLPrjInfo->uiPrjID != pstNextDLPrjInfo->uiPrjID) || \
            (pstLocalDLPrjInfo->uiMainVern != pstNextDLPrjInfo->uiMainVern) || \
            (pstLocalDLPrjInfo->uiMinorVern != pstNextDLPrjInfo->uiMinorVern) || \
            (pstLocalDLPrjInfo->uiPrjCRC != pstNextDLPrjInfo->uiPrjCRC)))
        {/*后一系需要同步工程文件*/
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                stSyncTask.emTaskId = SYS_SEND_FILE;
                stSyncTask.emPMId = emNextPMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                
                DEBUG_STATMNGT("SEND FILE N\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
            
            if((SYS_SEND_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                
                DEBUG_STATMNGT("SEND_FILE OK\n");
                
                SysDeSyncPrjQueue(pstSyncQueue);
            }
        }
    }

    return;
}



/*
***********************************************************************************************************************
* 函数名称: SysStateHandle13
*
* 功能描述: 稳态处理函数13
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
void SysStateHandle13(void)
{/*向后一系发送工程文件*/
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalDLPrjInfo = NULL;                            /* 本机下装工程信息数据指针     */
    PrjInfo_t *pstPreDLPrjInfo = NULL;                              /* 前一系下装工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */
    SyncTime64_t stSyncCurTime;                                     /* 时间结构体               */

    /* 获取本机工程信息 */
    pstLocalDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
    pstPreDLPrjInfo = SysGetDownLoadPrjInfo(emPrePMId);

    SysGetSyncTime(&stSyncCurTime);                                 /* 获取当前时间             */
    
    if((pstLocalDLPrjInfo != NULL) && (pstPreDLPrjInfo != NULL) && (pstSyncQueue != NULL))
    {
        if(((pstLocalDLPrjInfo->uiPrjID != 0) || (pstLocalDLPrjInfo->uiMainVern != 0) || 
           (pstLocalDLPrjInfo->uiMinorVern != 0) || (pstLocalDLPrjInfo->uiPrjCRC != 0))  && 
           
           ((pstLocalDLPrjInfo->uiPrjID != pstPreDLPrjInfo->uiPrjID) || \
            (pstLocalDLPrjInfo->uiMainVern != pstPreDLPrjInfo->uiMainVern) || \
            (pstLocalDLPrjInfo->uiMinorVern != pstPreDLPrjInfo->uiMinorVern) || \
            (pstLocalDLPrjInfo->uiPrjCRC != pstPreDLPrjInfo->uiPrjCRC)))
        {/*前一系需要同步工程文件*/
            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
            {/* 若同步任务队列无同步工程文件任务 */
                stSyncTask.emTaskId = SYS_SEND_FILE;
                stSyncTask.emPMId = emPrePMId;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                stSyncTask.emTaskStatus = TASK_FILE;
                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
                
                DEBUG_STATMNGT("SEND FILE P\n");
            }
        }
        else 
        {
            /* 获取当前任务队列中队首元素 */
            SysSyncPrjQueueFront(pstSyncQueue, &stSyncTask);
            
            if((SYS_SEND_FILE == stSyncTask.emTaskId) && (TASK_COMPLETE == stSyncTask.emTaskStatus))
            {/*清除掉已经完成的接收工程文件服务*/
                
                DEBUG_STATMNGT("SEND_FILE OK\n");
                
                SysDeSyncPrjQueue(pstSyncQueue);
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
