/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : SysSync.c
*
*  功能简介    : 系统资源管理模块中的同步功能模块，供其他同步功能相关模块调用
*
*  作者            : 刘阳
*
*  创建日期    : 2016-03-23
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
#include <Srv/Scheduler/include/string.h>
#include "../inc/SysSync.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/* 服务同步任务队列 */
static SysSyncSrvQueue_t s_stSysSyncSrvQueue;
/* 工程同步队列*/
static SysSyncPrjQueue_t s_stSysSyncPrjQueue;
/* 同步数据初始化标志*/
static sync_data_init_t s_emSyncDataInit;
/* 同步服务数据存放结构*/
static SysSrvInfo_t s_stSysSrvInfo[NUM_OF_CM][NUM_OF_SRVTYPE];

SysJoinReqStatus_t  emSysActiveStatus;





/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysGetSyncSrvQueue
*
* 功能描述: 获取同步任务队列指针。
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

SysSyncSrvQueue_t* SysGetSyncSrvQueue(void)
{
    SysSyncSrvQueue_t* pstRetSyncSrvQueue = &s_stSysSyncSrvQueue;

    return pstRetSyncSrvQueue;
}
/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueExist
*
* 功能描述: 查找指定同步任务是否已在同步任务队列。。
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
uint32_t SysSyncSrvQueueExist(sync_task_id_t emTaskId)
{
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();
    uint32_t uiCount = 0U;
    uint32_t uiRet = SYS_QUEUE_NOEXIST;
    uint32_t uiIndex = 0U;

    for(uiCount = 0; uiCount < pstSyncSrvQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncSrvQueue->usHead + uiCount) % SYNC_SRV_TASKS_NUM;
        if(emTaskId == pstSyncSrvQueue->stSysSyncTask[uiIndex].emTaskId)
        {
            uiRet = SYS_QUEUE_EXIST;
            break;
        }
    }

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueEmpty
*
* 功能描述: 同步任务队列是否为空。
*
* 输入参数: pstSyncSrvQueue    任务队列指针
*
* 输出参数: uiRet           若队列为空 SYS_QUEUE_EMPTY 否则 SYS_QUEUE_NOEMPTY
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysSyncSrvQueueEmpty(const SysSyncSrvQueue_t* pstSyncSrvQueue)
{
    uint32_t uiRet = SYS_QUEUE_NOEMPTY;

    if(SYS_QUEUE_NULL == pstSyncSrvQueue->uiTaskNum)
    {
        uiRet = SYS_QUEUE_EMPTY;
    }

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueFront
*
* 功能描述: 获取同步任务队列队首的同步任务信息。
*
* 输入参数: pstSyncSrvQueue    任务队列指针
*
* 输出参数: pstSyncTask     同步任务信息
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncSrvQueueFront(SysSyncSrvQueue_t* pstSyncSrvQueue, SysSyncTask_t* pstSyncTask)
{
    memcpy((puint8_t)pstSyncTask, (puint8_t)&pstSyncSrvQueue->stSysSyncTask[pstSyncSrvQueue->usHead], \
                  sizeof(SysSyncTask_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueFind
*
* 功能描述: 查找指定同步任务是否已在同步任务队列，若存在将其填充到指定地址中。
*
* 输入参数: pstSyncTask     同步任务信息
*
* 输出参数:
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncSrvQueueFind(sync_task_id_t emTaskId, SysSyncTask_t* pstSyncTask)
{
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();
    uint32_t uiCount = 0U;
    uint32_t uiIndex = 0U;

    for(uiCount = 0; uiCount < pstSyncSrvQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncSrvQueue->usHead + uiCount) % SYNC_SRV_TASKS_NUM;
        if(emTaskId == pstSyncSrvQueue->stSysSyncTask[uiIndex].emTaskId)
        {
            memcpy((puint8_t)pstSyncTask, (puint8_t)&pstSyncSrvQueue->stSysSyncTask[uiIndex], sizeof(SysSyncTask_t));
            break;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysDeSyncSrvQueue
*
* 功能描述: 队首元素出队。
*
* 输入参数: pstSyncSrvQueue    任务队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysDeSyncSrvQueue(SysSyncSrvQueue_t* pstSyncSrvQueue)
{
    if(SYS_QUEUE_NOEMPTY == SysSyncSrvQueueEmpty(pstSyncSrvQueue))
    {
        pstSyncSrvQueue->uiTaskNum--;
        pstSyncSrvQueue->usHead = (pstSyncSrvQueue->usHead + HARDCODE_ONE) % SYNC_SRV_TASKS_NUM;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueFull
*
* 功能描述: 
*
* 输入参数: pstSyncSrvQueue    任务队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysSyncSrvQueueFull(const SysSyncSrvQueue_t* pstSyncQueue)
{
    uint32_t uiRet = SYS_QUEUE_NOFULL;
    
    if(SYNC_SRV_TASKS_NUM == pstSyncQueue->uiTaskNum)
    {
        uiRet = SYS_QUEUE_FULL;
    }
    
    return uiRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysEnSyncSrvQueue
*
* 功能描述: 将同步任务放入同步队列。
*
* 输入参数: pstSyncSrvQueue    任务队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysEnSyncSrvQueue(SysSyncSrvQueue_t* pstSyncSrvQueue, SysSyncTask_t* pstSyncTask)
{
    if(SYS_QUEUE_NOFULL == SysSyncSrvQueueFull(pstSyncSrvQueue))
    {
        pstSyncSrvQueue->uiTaskNum++;

        memcpy((puint8_t)&pstSyncSrvQueue->stSysSyncTask[pstSyncSrvQueue->usTail],(puint8_t)pstSyncTask, \
                    sizeof(SysSyncTask_t));
        pstSyncSrvQueue->usTail = (pstSyncSrvQueue->usTail + 1) % SYNC_SRV_TASKS_NUM;
    }
    else
    {
        /*log*/
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueUpdate
*
* 功能描述: 更新同步任务的任务状态。
*
* 输入参数: emTaskStatus             任务状态
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncSrvQueueUpdate(sync_task_stat_t emTaskStatus)
{
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();

    if(SYS_QUEUE_NULL != pstSyncSrvQueue->uiTaskNum)
    {
        pstSyncSrvQueue->stSysSyncTask[pstSyncSrvQueue->usHead].emTaskStatus = emTaskStatus;
    }
    else
    {
        /*log*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncSrvQueueWait
*
* 功能描述: 设置同步任务等待标识。
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
void SysSyncSrvQueueWait(sync_task_wait_t emTaskWait)
{
    SysSyncSrvQueue_t* pstSyncSrvQueue = SysGetSyncSrvQueue();

    if(pstSyncSrvQueue->uiTaskNum != SYS_QUEUE_NULL)
    {
        pstSyncSrvQueue->stSysSyncTask[pstSyncSrvQueue->usHead].emTaskWait = emTaskWait;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysClearSyncSrvQueue
*
* 功能描述: 清空任务同步队列中目的地址为某一PM的任务。
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
void SysClearSyncSrvQueue(SysSyncSrvQueue_t* pstSyncQueue, PMController_t emPMId)
{
    uint32_t uiCount = 0U;
    uint32_t uiIndex = 0U;
    uint32_t uiNewCount = 0U;
    uint32_t uiNewIndex = 0U;
    
    for(uiCount = 0; uiCount < pstSyncQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncQueue->usHead + uiCount) % SYNC_SRV_TASKS_NUM;
        if(emPMId != pstSyncQueue->stSysSyncTask[uiIndex].emPMId)
        {
            if(uiCount != uiNewCount)
            {
                uiNewIndex = (pstSyncQueue->usHead + uiNewCount) % SYNC_SRV_TASKS_NUM;
                memcpy((puint8_t)&pstSyncQueue->stSysSyncTask[uiNewIndex], 
                              (puint8_t)&pstSyncQueue->stSysSyncTask[uiIndex], 
                              sizeof(SysSyncTask_t));
            }
            uiNewCount++;
        }
    }
    pstSyncQueue->uiTaskNum = uiNewCount;
    pstSyncQueue->usTail = (pstSyncQueue->usHead + uiNewCount) % SYNC_SRV_TASKS_NUM;

    return;
}



/*
***********************************************************************************************************************
* 函数名称: SysGetSyncPrjQueue
*
* 功能描述: 获取同步任务队列指针。
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

SysSyncPrjQueue_t* SysGetSyncPrjQueue(void)
{
    SysSyncPrjQueue_t* pstRetSyncPrjQueue = &s_stSysSyncPrjQueue;

    return pstRetSyncPrjQueue;
}
/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueExist
*
* 功能描述: 查找指定同步任务是否已在同步任务队列。。
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
uint32_t SysSyncPrjQueueExist(sync_task_id_t emTaskId)
{
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();
    uint32_t uiCount = 0U;
    uint32_t uiRet = SYS_QUEUE_NOEXIST;
    uint32_t uiIndex = 0U;

    for(uiCount = 0; uiCount < pstSyncPrjQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncPrjQueue->usHead + uiCount) % SYNC_SRV_TASKS_NUM;
        if(emTaskId == pstSyncPrjQueue->stSysSyncTask[uiIndex].emTaskId)
        {
            uiRet = SYS_QUEUE_EXIST;
            break;
        }
    }

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueEmpty
*
* 功能描述: 同步任务队列是否为空。
*
* 输入参数: pstSyncPrjQueue    任务队列指针
*
* 输出参数: uiRet           若队列为空 SYS_QUEUE_EMPTY 否则 SYS_QUEUE_NOEMPTY
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysSyncPrjQueueEmpty(const SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    uint32_t uiRet = SYS_QUEUE_NOEMPTY;

    if(SYS_QUEUE_NULL == pstSyncPrjQueue->uiTaskNum)
    {
        uiRet = SYS_QUEUE_EMPTY;
    }

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueFront
*
* 功能描述: 获取同步任务队列队首的同步任务信息。
*
* 输入参数: pstSyncPrjQueue    任务队列指针
*
* 输出参数: pstSyncTask     同步任务信息
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncPrjQueueFront(SysSyncPrjQueue_t* pstSyncPrjQueue, SysSyncTask_t* pstSyncTask)
{
    memcpy((puint8_t)pstSyncTask, (puint8_t)&pstSyncPrjQueue->stSysSyncTask[pstSyncPrjQueue->usHead], \
                  sizeof(SysSyncTask_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueFind
*
* 功能描述: 查找指定同步任务是否已在同步任务队列，若存在将其填充到指定地址中。
*
* 输入参数: pstSyncTask     同步任务信息
*
* 输出参数:
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncPrjQueueFind(sync_task_id_t emTaskId, SysSyncTask_t* pstSyncTask)
{
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();
    uint32_t uiCount = 0U;
    uint32_t uiIndex = 0U;

    for(uiCount = 0; uiCount < pstSyncPrjQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncPrjQueue->usHead + uiCount) % SYNC_SRV_TASKS_NUM;
        if(emTaskId == pstSyncPrjQueue->stSysSyncTask[uiIndex].emTaskId)
        {
            memcpy((puint8_t)pstSyncTask, (puint8_t)&pstSyncPrjQueue->stSysSyncTask[uiIndex], sizeof(SysSyncTask_t));
            break;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysDeSyncPrjQueue
*
* 功能描述: 队首元素出队。
*
* 输入参数: pstSyncPrjQueue    任务队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysDeSyncPrjQueue(SysSyncPrjQueue_t* pstSyncPrjQueue)
{
    if(SYS_QUEUE_NOEMPTY == SysSyncPrjQueueEmpty(pstSyncPrjQueue))
    {
        pstSyncPrjQueue->uiTaskNum--;
        pstSyncPrjQueue->usHead = (pstSyncPrjQueue->usHead + HARDCODE_ONE) % SYNC_PRJ_TASKS_NUM;
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueFull
*
* 功能描述: 
*
* 输入参数: pstSyncPrjQueue    PRJ队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysSyncPrjQueueFull(const SysSyncPrjQueue_t* pstSyncQueue)
{
    uint32_t uiRet = SYS_QUEUE_NOFULL;
    
    if(SYNC_PRJ_TASKS_NUM == pstSyncQueue->uiTaskNum)
    {
        uiRet = SYS_QUEUE_FULL;
    }
    
    return uiRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysEnSyncPrjQueue
*
* 功能描述: 将同步任务放入同步队列。
*
* 输入参数: pstSyncPrjQueue    任务队列指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysEnSyncPrjQueue(SysSyncPrjQueue_t* pstSyncPrjQueue, SysSyncTask_t* pstSyncTask)
{
    if(SYS_QUEUE_NOFULL == SysSyncPrjQueueFull(pstSyncPrjQueue))
    {
        pstSyncPrjQueue->uiTaskNum++;

        memcpy((puint8_t)&pstSyncPrjQueue->stSysSyncTask[pstSyncPrjQueue->usTail],(puint8_t)pstSyncTask, \
                    sizeof(SysSyncTask_t));
        pstSyncPrjQueue->usTail = (pstSyncPrjQueue->usTail + 1) % SYNC_PRJ_TASKS_NUM;
    }
    else
    {
        /*log*/
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueUpdate
*
* 功能描述: 更新同步任务的任务状态。
*
* 输入参数: emTaskStatus             任务状态
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSyncPrjQueueUpdate(sync_task_stat_t emTaskStatus)
{
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();

    if(SYS_QUEUE_NULL != pstSyncPrjQueue->uiTaskNum)
    {
        pstSyncPrjQueue->stSysSyncTask[pstSyncPrjQueue->usHead].emTaskStatus = emTaskStatus;
    }
    else
    {
        /*log*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSyncPrjQueueWait
*
* 功能描述: 设置同步任务等待标识。
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
void SysSyncPrjQueueWait(sync_task_wait_t emTaskWait)
{
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();

    if(pstSyncPrjQueue->uiTaskNum != SYS_QUEUE_NULL)
    {
        pstSyncPrjQueue->stSysSyncTask[pstSyncPrjQueue->usHead].emTaskWait = emTaskWait;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysClearSyncPrjQueue
*
* 功能描述: 清空工程同步队列中目的地址为某一PM的任务。
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
void SysClearSyncPrjQueue(SysSyncPrjQueue_t* pstSyncQueue, PMController_t emPMId)
{
    uint32_t uiCount = 0U;
    uint32_t uiIndex = 0U;
    uint32_t uiNewCount = 0U;
    uint32_t uiNewIndex = 0U;
    
    for(uiCount = 0; uiCount < pstSyncQueue->uiTaskNum; uiCount++)
    {
        uiIndex = (pstSyncQueue->usHead + uiCount) % SYNC_PRJ_TASKS_NUM;
        if(emPMId != pstSyncQueue->stSysSyncTask[uiIndex].emPMId)
        {
            if(uiCount != uiNewCount)
            {
                uiNewIndex = (pstSyncQueue->usHead + uiNewCount) % SYNC_PRJ_TASKS_NUM;
                memcpy((puint8_t)&pstSyncQueue->stSysSyncTask[uiNewIndex], 
                              (puint8_t)&pstSyncQueue->stSysSyncTask[uiIndex], 
                              sizeof(SysSyncTask_t));
            }
            uiNewCount++;
        }
    }
    pstSyncQueue->uiTaskNum = uiNewCount;
    pstSyncQueue->usTail = (pstSyncQueue->usHead + uiNewCount) % SYNC_PRJ_TASKS_NUM;

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysDiffSyncTime
*
* 功能描述: 计算时间差。
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
uint32_t SysDiffSyncTime(const SyncTime64_t* punSyncOldTime, const SyncTime64_t* punSyncCurTime)
{
    uint32_t uiRet = 0;

    uiRet = punSyncCurTime->stTimeStruct.uiLow32b - punSyncOldTime->stTimeStruct.uiLow32b;

    return uiRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSyncDataInit
*
* 功能描述: 获取同步记录信息是否初始化标识。
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
sync_data_init_t SysGetSyncDataInit(void)
{
    sync_data_init_t emRet = SYNC_DATA_INIT;

    emRet = s_emSyncDataInit;

    return emRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetSyncDataInit
*
* 功能描述: 设置同步记录信息是否初始化标识。
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
void SysSetSyncDataInit(sync_data_init_t emDataInit)
{
    s_emSyncDataInit = emDataInit;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetTimerStatus
*
* 功能描述: 获取同步时钟主从状态。
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
timer_status_t SysGetTimerStatus(PMController_t emPMId)
{
    uint32_t uiTimerStatus = 0U;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地控制器ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);        /* 下一个控制器ID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);          /* 前一个控制器ID号     */

    timer_status_t emRetTimerStatus = TIMER_SLAVE;

//    uiTimerStatus = BspGetTimerStatus();

    if((emPMId == emLocalPMId) && (uiTimerStatus & BITMASK(0)))
    {
        emRetTimerStatus = TIMER_MASTER;
    }
    else if((emPMId == emNextPMId) && (uiTimerStatus & BITMASK(1)))
    {
        emRetTimerStatus = TIMER_MASTER;
    }
    else if((emPMId == emPrePMId) && (uiTimerStatus & BITMASK(2)))
    {
        emRetTimerStatus = TIMER_MASTER;
    }
    else
    {
        emRetTimerStatus = TIMER_SLAVE;
    }

    return emRetTimerStatus;
}


/*
***********************************************************************************************************************
* 函数名称: SysCalRelativeTime
*
* 功能描述: 计算绝对时间函数，根据当前时间及时间间隔计算出下一个绝对时间。
*
* 输入参数: punCalTime               计算结果
*        punCurTime               当前时间
*        uiIntervalTime           时间间隔
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysCalRelativeTime(SyncTime64_t* punCalTime, const SyncTime64_t* punCurTime, uint32_t uiIntervalTime)
{
    //uint32_t uiShedTime = SysGetPrjShedTime();
	uint32_t uiShedTime = 0U;
    ulong64_t ullTemp = 0U;

    punCalTime->ullTime = punCurTime->ullTime + uiIntervalTime;

    return;
}


/*
***********************************************************************************************************************
* 函数名称: SysGetSrvInfo
*
* 功能描述: 获取来自某CM指定类型服务相关数据指针。
*
* 输入参数:
*
* 输出参数: pstRetSysSrvInfo         服务相关数据指针
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
SysSrvInfo_t* SysGetSrvInfo(CMController_t emCMID, srv_srvdata_type_t emSrvType)
{
    SysSrvInfo_t* pstRetSysSrvInfo = &s_stSysSrvInfo[emCMID][emSrvType];

    return pstRetSysSrvInfo;
}

SysJoinReqStatus_t* SysGetActiveInfo(void)
{
    return (SysJoinReqStatus_t*)&emSysActiveStatus;
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
