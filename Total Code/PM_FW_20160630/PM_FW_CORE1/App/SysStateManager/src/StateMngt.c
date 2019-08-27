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
*  文件名称    : SysStateManager.c
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
const static uint32_t s_uiUnstableCode[MAX_UNSTABLE_STATE] = {  P_O_O, P_O_R, \
        P_O_T, P_O_S, P_R_O, P_R_R, P_T_O, P_T_T, P_S_O, P_S_S, T_O_P, T_P_O, \
        T_P_T, T_T_P, S_O_P, S_P_O, S_P_S, S_S_P, R_O_P, R_P_O, R_P_R, R_R_P, \
        D_O_O, D_D_O, D_O_D, D_D_D,R_P_P,P_R_P,P_P_R};



/* 稳态状态码表  */
/*
const static uint32_t s_uiStableCode[MAX_STABLE_STATE] = { T_O_O, T_O_T, T_T_O, \
        T_T_T, S_O_O, S_O_S, S_S_O , S_S_S, R_O_O, R_O_R, R_R_O, R_R_R};
*/


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

uint32_t SysInUnStableArray(uint32_t uiStateCode);

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

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
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
    static uint32_t s_uiFirstFlag = SYS_FIRST_IN;                                       /* 首次处理标识           */
    static uint32_t s_uiLocalState = WS_STANDBY;                                        /* 本PM上拍状态       */
    static timer_status_t s_emLocalTimerState = TIMER_SLAVE;                            /* 同步时钟上拍主从状态   */

    uint32_t uiState[NUM_OF_PM] = {WS_OFFLINE, WS_OFFLINE, WS_OFFLINE};                 /* 当前三冗余PM状态   */
    uint32_t uiSortState[NUM_OF_PM] = {WS_OFFLINE, WS_OFFLINE, WS_OFFLINE};             /* 排序后三冗余PM状态 */
    uint32_t uiStateCode = 0U;                                                          /* 状态码值               */
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    SysSyncSrvQueue_t* pstSyncQueue = SysGetSyncSrvQueue();                             /* 同步任务队列           */
    SysSyncTask_t stSyncTask;                                                           /* 同步任务               */
    timer_status_t emTimerStatus = SysGetTimerStatus(emLocalPMId);                      /* 同步时钟当前状态       */


//    printf("  SysStateMngt  \n");
    
    /* 硬件诊断更新三系PM状态，判断PM是否离线 */
    SysUpdateState();
    /* 获取三系PM当前工作状态 */
    uiState[PMA] = SysGetWorkStatus(PMA);
    uiState[PMB] = SysGetWorkStatus(PMB);
    uiState[PMC] = SysGetWorkStatus(PMC);

    /* 若第一次处理函数，初始化静态记录 */
    if(SYS_FIRST_IN == s_uiFirstFlag)
    {
        s_emLocalTimerState = emTimerStatus;
        s_uiLocalState = uiState[emLocalPMId];
        s_uiFirstFlag = SYS_NOFIRST_IN;
    }

    /* 判断本系PM状态是否改变，若改变，添加广播本PM状态任务到同步任务队列。
     * 或者本系同步时钟主从状态由从变为主，添加广播本PM状态任务到同步任务队列 */
//    if((s_uiLocalState != uiState[emLocalPMId]) ||
//       ((s_emLocalTimerState != emTimerStatus) && (TIMER_MASTER == emTimerStatus)))
//    {
//        if((s_emLocalTimerState != emTimerStatus) && (TIMER_MASTER == emTimerStatus))
//        {/* 判断是否需要校时 ,本机时钟主从变化，由从->主  */
//            SysSetAjustTimerEn(PMA, AJUST_EN);
//            SysSetAjustTimerEn(PMB, AJUST_EN);
//            SysSetAjustTimerEn(PMC, AJUST_EN);
//        }
//        /* 添加同步任务到同步任务队列 */
//        if(SYS_QUEUE_EXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLDATA))
//        {/* 同步任务队列已有广播PM信息任务 */
//            SysSyncSrvQueueFind(SYS_BROADCAST_CTRLDATA, &stSyncTask);
//            if((stSyncTask.emPMId != INVALID_PM_ID))
//            {/* 广播目的站号未包含本发送端,添加该广播到任务队列 */
//                stSyncTask.emPMId = INVALID_PM_ID;
//                stSyncTask.emTaskStatus = TASK_SEND;
//                stSyncTask.emTaskId = SYS_BROADCAST_CTRLDATA;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                SysEnSyncSrvQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//        else
//        {/* 同步任务队列中无广播PM信息任务,添加该广播到任务队列 */
//            stSyncTask.emPMId = INVALID_PM_ID;
//            stSyncTask.emTaskStatus = TASK_SEND;
//            stSyncTask.emTaskId = SYS_BROADCAST_CTRLDATA;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            SysEnSyncSrvQueue(pstSyncQueue, &stSyncTask);
//        }
//        /* 记录当前PM状态及同步时钟主从状态 */
//        s_uiLocalState = uiState[emLocalPMId];
//        s_emLocalTimerState = emTimerStatus;
//    }
    
/*test  周期性的发送PM状态同步任务*/
        /* 添加同步任务到同步任务队列 */
        if(SYS_QUEUE_EXIST == SysSyncSrvQueueExist(SYS_BROADCAST_CTRLDATA))
        {/* 同步任务队列已有广播PM信息任务 */
            SysSyncSrvQueueFind(SYS_BROADCAST_CTRLDATA, &stSyncTask);
            if((stSyncTask.emPMId != INVALID_PM_ID))
            {/* 广播目的站号未包含本发送端,添加该广播到任务队列 */
                stSyncTask.emPMId = INVALID_PM_ID;
                stSyncTask.emTaskStatus = TASK_SEND;
                stSyncTask.emTaskId = SYS_BROADCAST_CTRLDATA;
                stSyncTask.emTaskWait = TASK_NOWAIT;
                SysEnSyncSrvQueue(pstSyncQueue, &stSyncTask);
                
//                DEBUG_STATMNGT("  2 SysEnSyncSrvQueue : SYS_BROADCAST_CTRLDATA  \n");
                
            }
        }
        else
        {/* 同步任务队列中无广播PM信息任务,添加该广播到任务队列 */
            stSyncTask.emPMId = INVALID_PM_ID;
            stSyncTask.emTaskStatus = TASK_SEND;
            stSyncTask.emTaskId = SYS_BROADCAST_CTRLDATA;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            SysEnSyncSrvQueue(pstSyncQueue, &stSyncTask);
            
//            DEBUG_STATMNGT("  1 SysEnSyncSrvQueue : SYS_BROADCAST_CTRLDATA  \n");
            
        }
/*test  周期性的发送PM状态同步任务*/
    
    
    

    uiSortState[CMP_LOCAL] = SysGetWorkStatus(emLocalPMId);
    uiSortState[CMP_PRE] = SysGetWorkStatus(emPrePMId);
    uiSortState[CMP_NEXT] = SysGetWorkStatus(emNextPMId);
    
//    DEBUG_STATMNGT(" uiSortState[%d] is : %x  uiSortState[%d] is : %x  uiSortState[%d] is : %x \n",\
//            emLocalPMId,uiSortState[CMP_LOCAL],emPrePMId,uiSortState[CMP_PRE],emNextPMId,uiSortState[CMP_NEXT]);
    

    /* 根据排序后三冗余PM状态生成系统状态码值 */
    uiStateCode = SysGetStateCode(uiSortState[CMP_LOCAL], uiSortState[CMP_PRE], uiSortState[CMP_NEXT]);
    

    

    if(SYS_UNSTABLE_STATE == SysInUnStableArray(uiStateCode))
    {   /* 判断系统状态是否发生改变，即三个PM的状态是否发生改变 */
        
        
        
        if((SYS_STATE_CHANGED == SysStateChange(&s_uiOldState[PMA], &uiState[PMA]))
                && SYS_SYNC_STATE_READY == SysStateReady())
        {/* 若系统状态为非稳态 */

            /*log*/
            
            DEBUG_STATMNGT(" SYS_UNSTABLE_STATE \n");
            
            DEBUG_STATMNGT(" uiStateCode is : %x \n",uiStateCode);
            

            /* 调用非稳态处理函数 */
            SysUnstableStateHandle(uiStateCode);

            /* 设置当前系统处于非稳态 */
            SysSetState(SYS_UNSTABLE_STATE);
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
            DEBUG_STATMNGT(" SYS_STABLE_STATE \n");
            
            DEBUG_STATMNGT(" uiStateCode is : %x \n",uiStateCode);
        }
        
        
//        DEBUG_STATMNGT(" SYS_STABLE_STATE \n");
    }

    emlastPreWorkStat = SysGetWorkStatus(emPrePMId);
    emlastNextWorkStat = SysGetWorkStatus(emNextPMId);
    s_uiOldState[PMA] = uiState[PMA];
    s_uiOldState[PMB] = uiState[PMB];
    s_uiOldState[PMC] = uiState[PMC];

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
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
void SysUpdateState(void)
{
    uint16_t usState = 0U;                                                              /* 硬件状态值           */
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();                          /* 同步任务队列         */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();                          /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                                           /* 同步任务             */
    Key_Switch_Type_t emSwitchKey = KS_INVALID_TYPE;
    PrjInfo_t* pstLocalPrjInfo = NULL;                                                  /* 本机工程信息数据指针     */
    SysPMStateInfo_t* pstLocalPMState = NULL;
    SysPMStateInfo_t* pstNextPMState = NULL;
    SysPMStateInfo_t* pstPrePMState = NULL;

    /*PM升主使能*/
    SysSetTimerMasterEnable(MASTER_ENABLE_VAR);
    /*PM软件运行OK*/
    SysSetSWRuninOK(SW_RUNING_OK_VAR);
    
    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstLocalPMState = SysGetPMStateInfo(emLocalPMId);
    pstNextPMState = SysGetPMStateInfo(emNextPMId);
    pstPrePMState = SysGetPMStateInfo(emPrePMId);
    
//    DEBUG_STATMNGT("  emLocalPMId is : %x \n",emLocalPMId);
//    DEBUG_STATMNGT("  emNextPMId is : %x \n",emNextPMId);
//    DEBUG_STATMNGT("  emPrePMId is : %x \n",emPrePMId);
    

//    if(PMA == emLocalPMId )
//    {
//        pstLocalPrjInfo->uiPrjID = 0x01010101;
//        pstLocalPrjInfo->uiMainVern = 0x01010101;
//        pstLocalPrjInfo->uiMinorVern = 0x0a0a0a0a;
//        pstLocalPrjInfo->uiPrjCRC = 0x0a0a0a0a;
//        pstLocalPrjInfo->ucIsConfigured = HAS_BE_CONFIGURED;
//    }
    
    
//    DEBUG_STATMNGT("  Before:WorkStatus is : %x \n",SysGetWorkStatus(emLocalPMId));
    
    
    if((pstLocalPMState != NULL) && (pstLocalPrjInfo != NULL))
    {
        emSwitchKey = SharedGetSwitchPos();
        
        
        pstLocalPMState->emSwitchKeys = emSwitchKey;
        
//        DEBUG_STATMNGT("  emSwitchKey is : %x \n",pstLocalPMState->emSwitchKeys);
        
        switch(emSwitchKey)
        {
            case KS_INIT:
            {
                if((pstLocalPrjInfo->uiPrjID != 0U) || (pstLocalPrjInfo->uiMainVern != 0U) || 
                   (pstLocalPrjInfo->uiMinorVern != 0U) || (pstLocalPrjInfo->uiPrjCRC != 0U) || 
                   (HAS_BE_CONFIRMED == pstLocalPrjInfo->ucIsConfigured))
                {
                    memset(pstLocalPrjInfo, 0x00, sizeof(PrjInfo_t));
                    
                    SysCfgFileInit();
    
                }
                SysSetWorkStatus(emLocalPMId,WS_STANDBY);
            }
            break;
            
            case KS_PROGRAM:
            {
                if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
                {
                    SysSetWorkStatus(emLocalPMId,WS_ONLINE_DOWNLOAD);
                }
                else
                {
                    if((HAS_BE_CONFIRMED == pstLocalPrjInfo->ucIsConfigured))
                    {
                        SysSetWorkStatus(emLocalPMId,WS_RUN);
        
                    }
                    else
                    {
                        SysSetWorkStatus(emLocalPMId,WS_POWERON);
                    }
                }
            }
            break;
            
            case KS_RUN:
            {
                if((HAS_BE_CONFIRMED == pstLocalPrjInfo->ucIsConfigured))
                {
                    SysSetWorkStatus(emLocalPMId,WS_RUN);
    
                }
                else
                {
                    SysSetWorkStatus(emLocalPMId,WS_POWERON);
                }
            }
            break;
            
            case KS_STOP:
            {
                if((HAS_BE_CONFIRMED == pstLocalPrjInfo->ucIsConfigured))
                {
                    SysSetWorkStatus(emLocalPMId,WS_STOP);
    
                }
                else
                {
                    SysSetWorkStatus(emLocalPMId,WS_POWERON);
                }
            }
            break;
            
            default:
                
            break;
        }
        
        pstLocalPMState->emWorkStatus = SysGetWorkStatus(emLocalPMId);
        
//        DEBUG_STATMNGT("  After:WorkStatus is : %x \n",pstLocalPMState->emWorkStatus);
    }
    
    
//    DEBUG_STATMNGT("  NEXT and PRE usState is : %x \n",usState);
    
    
    /* 获取硬件提供状态值 */
    usState = SysGetModularOnLine();
    
    if(!(usState & BITMASK(0)))
    {
        
        if(SysGetWorkStatus(emNextPMId) != WS_OFFLINE)
        {
            /*log,PM离线*/
            
            DEBUG_STATMNGT("  NEXT OFFLINE \n");
            
        }
        /* 更新下一个PM状态 */
        SysSetWorkStatus(emNextPMId, WS_OFFLINE);
        
        pstNextPMState->emWorkStatus = SysGetWorkStatus(emNextPMId);
        
        if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
        {/*清空发送给已经离线的PM的同步任务*/
            SysSyncPrjQueueFront(pstSyncSrvQueue, &stSyncTask);
            
            if((stSyncTask.emPMId == emNextPMId) && ((SYS_SEND_FILE == stSyncTask.emTaskId) || (SYS_SEND_REALDATA == stSyncTask.emTaskId)))
            {
                /* 清空工程同步队列 */
                SysClearSyncPrjQueue(pstSyncPrjQueue, emNextPMId);
            }
        }
        /* 清空任务同步队列 */
        SysClearSyncSrvQueue(pstSyncSrvQueue, emNextPMId);
    }
    
    if(!(usState & BITMASK(1)))
    {
        
        if(SysGetWorkStatus(emPrePMId) != WS_OFFLINE)
        {
            /*log,前一系PM离线*/
            
            DEBUG_STATMNGT("  PRE OFFLINE \n");
            
        }
        /* 更新下一个PM状态 */
        SysSetWorkStatus(emPrePMId, WS_OFFLINE);
        pstPrePMState->emWorkStatus = SysGetWorkStatus(emPrePMId);
        
        if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
        {/*清空发送给已经离线的PM的同步任务*/
            SysSyncPrjQueueFront(pstSyncSrvQueue, &stSyncTask);
            
            if((stSyncTask.emPMId == emPrePMId) && ((SYS_SEND_FILE == stSyncTask.emTaskId) || (SYS_SEND_REALDATA == stSyncTask.emTaskId)))
            {
                /* 清空工程同步队列 */
                SysClearSyncPrjQueue(pstSyncPrjQueue, emPrePMId);
            }
        }
        /* 清空任务同步队列 */
        SysClearSyncSrvQueue(pstSyncSrvQueue, emPrePMId);
    }
    
    
    if((WS_OFFLINE == SysGetWorkStatus(emNextPMId)) && (WS_OFFLINE == SysGetWorkStatus(emPrePMId)))
    {
        SetLEDState(LED_NAME_PMBUS, LED_OFF);
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
uint32_t SysStateReady(void)
{
    uint16_t usState = 0U;                                          /* 硬件状态值           */
    uint32_t uiRetRecvOk = SYS_SYNC_STATE_READY;                    /* 返回值               */
    PMController_t emWaitPMId = SysGetWaitPMId();

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
uint32_t SysInUnStableArray(uint32_t uiStateCode)
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
    {/* 根据状态码值调用相应处理函数 */
        /*单独处理*/
        case P_O_O:
        {
            SysStateHandle1();
        }
        break;

        /*以前一系为准删除工程*/
        case P_S_O:
        case P_S_S:
        /*以后一系为准删除工程*/
        case P_O_S:
        {
            SysStateHandle2();
        }
        break;

        /*接收后一系数据，接收工程区数据前，需要暂停系统*/
        case P_O_R:
        {
            SysStateHandle3();
        }
        break;

        /*接收后一系数据，接收工程区数据前，不需要暂停系统*/
        case P_O_T:
        {
            SysStateHandle4();
        }
        break;

        /*接收前一系数据，接收工程区数据前，需要暂停系统*/
        case P_R_P:
        case P_R_O:
        case P_R_R:
        {
            SysStateHandle5();
        }
        break;
        
        case P_P_R:

        break;

        /*接收前一系数据，接收工程区数据前，不需要暂停系统*/
        case P_T_O:
        case P_T_T:
        {
            SysStateHandle6();
        }
        break;

        /*向后一系发送，同步工程区数据前，不需要暂停系统*/
        case T_O_P:
        case T_T_P:
        {
            SysStateHandle7();
        }
        break;

        /*向前一系发送，同步工程区数据前，不需要暂停系统*/
        case T_P_O:
        {
            SysStateHandle8();
        }
        break;

        /*不处理*/
        case T_P_T:
        break;

        case S_O_P:
        break;

        case S_P_O:
        break;

        case S_P_S:
        break;

        case S_S_P:
        break;

        /*向后一系发送，同步工程区数据前，需要暂停系统*/
        case R_P_P:
        case R_O_P:
        case R_R_P:
        {
            SysStateHandle9();
        }
        break;

        /*向前一系发送，同步工程区数据前，需要暂停系统*/
        case R_P_O:
        {
            SysStateHandle10();
        }
        break;

        /*不处理*/
        case R_P_R:
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
    switch(uiStateCode)
    {/* 根据状态码值调用相应处理函数 */
        case T_O_O:
        case T_O_T:
        case T_T_O:

        case T_T_T:

        case S_O_O:

        case S_O_S:
        case S_S_O:

        case S_S_S:
        case R_O_O:

        case R_O_R:
        case R_R_O:
        case R_R_R:



        default:
            /*记录日志*/
            break;
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地控制器ID号       */
    Key_Switch_Type_t emKeySwitchValue = KS_INVALID_TYPE;


    emKeySwitchValue = SysGetKeySwitchValue();
    switch(emKeySwitchValue)
    {
        case KS_INIT:
        {
            if((PRG_FILE_EXIST) == SysGetPrjFileExist() || (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
            {/*工程文件存在，或配置信息存在，说明有工程，清除工程*/
                /*清除所有工程信息，文件，配置等*/
//                SysClearProject();//待补充。。。。。。
            }
            SysSetWorkStatus(emLocalPMId,WS_STANDBY);
        }
        break;

        case KS_STOP:
        {
            if(((PRG_FILE_EXIST) == SysGetPrjFileExist()) && (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
            {/*工程文件存在,并且配置信息存在*/
                SysSetWorkStatus(emLocalPMId,WS_STOP);
            }
        }
        break;
        
        case KS_PROGRAM:
        case KS_RUN:
        {
            if(((PRG_FILE_EXIST) == SysGetPrjFileExist()) && (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
            {/*工程文件存在,并且配置信息存在*/
                SysSetWorkStatus(emLocalPMId,WS_RUN);
            }
        }
        break;

        default:
        break;
    }
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地控制器ID号       */


    if((PRG_FILE_EXIST) == SysGetPrjFileExist() || (HAS_BE_CONFIGURED == SysGetConfigStatus(emLocalPMId)))
    {/*工程文件存在，或配置信息存在，说明有工程，清除工程*/
        /*清除所有工程信息，文件，配置等*/
    	
//        SysClearProject();//待补充。。。。。。

    }
    SysSetWorkStatus(emLocalPMId,WS_STANDBY);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysStateHandle3
*
* 功能描述: 非稳态处理函数3，处理状态为：P_O_R。
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步工程队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
    {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_RECV_FILE;
            stSyncTask.emPMId = emNextPMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
            
            
            DEBUG_STATMNGT("  SYS_RECV_FILE !!!!!\n");
        }
    }
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//    {
//        if(SysGetPauseFlag() != PF_PAUSE)
//        {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_PAUSE))
//            {
//                stSyncTask.emTaskId = SYS_PAUSE;
//                stSyncTask.emPMId = INVALID_PM_ID;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_SEND;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//        else
//        {/* 同步工程数据区，从G区*/
//            stSyncTask.emTaskId = SYS_RECV_REALDATA;
//            stSyncTask.emPMId = emNextPMId;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            stSyncTask.emTaskStatus = TASK_G_DATA;
//            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//        }
//    }
    else
    {
//        if((PF_PAUSE == SysGetPauseFlag()) && (SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured))
//        {
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_CONTINUE))
//            {
//                stSyncTask.emTaskId = SYS_CONTINUE;
//                stSyncTask.emPMId = INVALID_PM_ID;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_SEND;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }

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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
    {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_RECV_FILE;
            stSyncTask.emPMId = emNextPMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
        }
    }
//    else
//    {/* 同步工程数据区，从G区*/
//        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//        {
//            stSyncTask.emTaskId = SYS_RECV_REALDATA;
//            stSyncTask.emPMId = emNextPMId;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            stSyncTask.emTaskStatus = TASK_G_DATA;
//            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//        }
//        else
//        {
//            if(SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured)
//            {
//                SysSetWorkStatus(emLocalPMId,WS_STOP);
//            }
//        }
//    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysStateHandle5
*
* 功能描述: 非稳态处理函数5，处理状态为：P_R_O、P_R_R。
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
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);

    if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
    {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_RECV_FILE;
            stSyncTask.emPMId = emPrePMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
        }
    }
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//    {
//        if(SysGetPauseFlag() != PF_PAUSE)
//        {/* 同步工程数据前，若系统未处于暂停状态，则添加系统暂停服务 */
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_PAUSE))
//            {
//                stSyncTask.emTaskId = SYS_PAUSE;
//                stSyncTask.emPMId = INVALID_PM_ID;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_SEND;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//        else
//        {/* 同步工程数据区，从G区*/
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//            {
//                stSyncTask.emTaskId = SYS_RECV_REALDATA;
//                stSyncTask.emPMId = emPrePMId;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_G_DATA;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//    }
//    else
//    {
//        if((PF_PAUSE == SysGetPauseFlag()) && (SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured))
//        {
//            if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_CONTINUE))
//            {
//                stSyncTask.emTaskId = SYS_CONTINUE;
//                stSyncTask.emPMId = INVALID_PM_ID;
//                stSyncTask.emTaskWait = TASK_NOWAIT;
//                stSyncTask.emTaskStatus = TASK_SEND;
//                SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//            }
//        }
//        if(PF_CONTINUE == SysGetPauseFlag())
//        {
//            SysSetWorkStatus(emLocalPMId,WS_RUN);
//        }
//
//    }
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);

    if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
    {/*本机工程文件无效，或者工程信息有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_RECV_FILE;
            stSyncTask.emPMId = emPrePMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
        }
    }
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_REALDATA))
//    {/* 同步工程数据区，从G区*/
//            stSyncTask.emTaskId = SYS_RECV_REALDATA;
//            stSyncTask.emPMId = emPrePMId;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            stSyncTask.emTaskStatus = TASK_G_DATA;
//            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    }
//    else
//    {
//        if(SYS_PRJ_CONFIGURED == pstLocalPrjInfo->ucIsConfigured)
//        {
//            SysSetWorkStatus(emLocalPMId,WS_STOP);
//        }
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
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
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    {/* 同步工程数据区，从G区*/
//        stSyncTask.emTaskId = SYS_SEND_REALDATA;
//        stSyncTask.emPMId = emNextPMId;
//        stSyncTask.emTaskWait = TASK_NOWAIT;
//        stSyncTask.emTaskStatus = TASK_G_DATA;
//        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    }
//    else
//    {
//
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
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);

    if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
    {/*前一系工程文件无效，或者其工程信息与本机有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_RECV_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_SEND_FILE;
            stSyncTask.emPMId = emPrePMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
        }
    }
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    {/* 同步工程数据区，从G区*/
//        stSyncTask.emTaskId = SYS_SEND_REALDATA;
//        stSyncTask.emPMId = emPrePMId;
//        stSyncTask.emTaskWait = TASK_NOWAIT;
//        stSyncTask.emTaskStatus = TASK_G_DATA;
//        SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//    }
//    else
//    {
//
//    }
    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysStateHandle9
*
* 功能描述: 非稳态处理函数9，处理状态为：R_O_P、R_R_P。
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
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstNextPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();         /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstNextPrjInfo = SysGetPrjInfo(emNextPMId);

    if((pstLocalPrjInfo->uiPrjID != pstNextPrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstNextPrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstNextPrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstNextPrjInfo->uiPrjCRC))
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
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    {
//        if(PF_PAUSE == SysGetPauseFlag())
//        {/* 同步工程数据前，若系统处于暂停状态，则同步工程数据区，从G区开始 */
//            stSyncTask.emTaskId = SYS_SEND_REALDATA;
//            stSyncTask.emPMId = emNextPMId;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            stSyncTask.emTaskStatus = TASK_G_DATA;
//            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//        }
//    }
//    else
//    {
//
//    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysStateHandle10
*
* 功能描述: 非稳态处理函数10，处理状态为：R_P_O。
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
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个PMID号     */
    PrjInfo_t *pstLocalPrjInfo = NULL;                              /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrePrjInfo = NULL;                                /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncQueue = SysGetSyncPrjQueue();               /* 同步任务队列         */
    SysSyncTask_t stSyncTask;                                       /* 同步任务             */

    /* 获取本机工程信息 */
    pstLocalPrjInfo = SysGetPrjInfo(emLocalPMId);
    pstPrePrjInfo = SysGetPrjInfo(emPrePMId);

    if((pstLocalPrjInfo->uiPrjID != pstPrePrjInfo->uiPrjID) ||
       (pstLocalPrjInfo->uiMainVern != pstPrePrjInfo->uiMainVern) ||
       (pstLocalPrjInfo->uiMinorVern != pstPrePrjInfo->uiMinorVern) ||
       (pstLocalPrjInfo->uiPrjCRC != pstPrePrjInfo->uiPrjCRC))
    {/*前一系工程文件无效，或者工程信息与本机有任何不一致，需要同步工程文件*/
        if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_FILE))
        {/* 若同步任务队列无同步工程文件任务 */
            stSyncTask.emTaskId = SYS_SEND_FILE;
            stSyncTask.emPMId = emPrePMId;
            stSyncTask.emTaskWait = TASK_NOWAIT;
            stSyncTask.emTaskStatus = TASK_FILE;
            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
            
            DEBUG_STATMNGT("  SYS_SEND_FILE !!!!!\n");
            
        }
    }
//    else if(SYS_QUEUE_NOEXIST == SysSyncPrjQueueExist(SYS_SEND_REALDATA))
//    {
//        if(PF_PAUSE == SysGetPauseFlag())
//        {/* 同步工程数据前，若系统处于暂停状态，则同步工程数据区，从G区开始 */
//            stSyncTask.emTaskId = SYS_SEND_REALDATA;
//            stSyncTask.emPMId = emPrePMId;
//            stSyncTask.emTaskWait = TASK_NOWAIT;
//            stSyncTask.emTaskStatus = TASK_G_DATA;
//            SysEnSyncPrjQueue(pstSyncQueue, &stSyncTask);
//        }
//    }
//    else
//    {
//
//    }

    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
