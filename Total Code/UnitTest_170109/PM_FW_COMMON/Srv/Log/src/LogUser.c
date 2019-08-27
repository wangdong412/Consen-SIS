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
*  文件名称    : Log.c
*
*  功能简介    : 
*
*  作者            : 李琦
*
*  创建日期    : 2015-12-31
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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/Log.h"
#include "Srv/Sys/inc/CommSys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static puint32_t s_puiLockFlag;
static LogQueue_t s_stQueueInfo;
static LogQueue_t *s_pstQueueInfo;
static LogInfo_t *s_pstBaseLogInfo;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void WriteQueueInfoToSRAM(void);
static void ReadQueueInfoFromSRAM(void);


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: LogInitUser
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
void LogInitUser(void)
{
    /* 0为core0,1为core1 */
    if(1==get_cpu_no())
    {
        memset((puint8_t)&s_stQueueInfo, 0, sizeof(LogQueue_t));

        s_pstQueueInfo = (LogQueue_t *)LOG_SRAM_QUEUE_INFO_SEC9_START_ADDR;

        /* test */
        //memcpy((puint8_t)s_pstQueueInfo, (puint8_t)&s_stQueueInfo, sizeof(LogQueue_t));

        if((s_pstQueueInfo->usMagic1 != LOG_MAGIC_1) && (s_pstQueueInfo->usMagic2 != LOG_MAGIC_2))
        {
            s_stQueueInfo.usMagic1 = LOG_MAGIC_1;
            s_stQueueInfo.usSize = 0;     /* 队列大小 */
            s_stQueueInfo.usHeader = 0;   /* 队头*/
            s_stQueueInfo.usRetainHeader = 0;   /* 掉电保持队头*/
            s_stQueueInfo.usTail = 0;     /* 队尾 */
            s_stQueueInfo.usQueueState = LOG_QUEUE_EMPTY;/* 队列空满状态标识 */
            s_stQueueInfo.usMagic2 = LOG_MAGIC_2;

            /* 第一次上电，初始化队列信息并存入SRAM */
            WriteQueueInfoToSRAM();
            //printf("magic=%x %x\n", s_pstQueueInfo->usMagic1, s_pstQueueInfo->usMagic2);
        }
        else
        {
            /* 获取队列信息 */
            memcpy((puint8_t)&s_stQueueInfo, (puint8_t)s_pstQueueInfo, sizeof(LogQueue_t));

            if(s_stQueueInfo.usSize >= MAX_LOG_NUM_RETAIN)
            {
                s_stQueueInfo.usSize = MAX_LOG_NUM_RETAIN;
                s_stQueueInfo.usHeader = s_stQueueInfo.usRetainHeader;
                s_stQueueInfo.usQueueState = LOG_QUEUE_NOT_EMPTY_NOT_FULL;
            }
            /* 更新队列信息 */
            WriteQueueInfoToSRAM();
        }

        s_pstBaseLogInfo = (LogInfo_t *)LOG_SRAM_LOG_INFO_SEC9_START_ADDR;
        s_puiLockFlag = (puint32_t)LOG_SRAM_LOCK_FLAG2_START_ADDR;
        *s_puiLockFlag = (uint32_t)LOG_QUEUE_UNLOCK;

        printf("%x %x %d %d %d %d\n",s_stQueueInfo.usMagic1, s_stQueueInfo.usMagic2, s_stQueueInfo.usSize,s_stQueueInfo.usHeader,s_stQueueInfo.usTail, s_stQueueInfo.usQueueState);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: LogWriteUser
*
* 功能描述: 将1条LOG记录写入队列
*
* 输入参数: uiLogID: 日志的id
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void LogWriteUser(uint32_t uiLogID)
{
    LogInfo_t *s_pstLogInfo = NULL;
    uint16_t usTail = 0;
    bool_t bAtomicFlg = false;
    SysTime_t stTime;

    /* 循环写入日志 */

    do
    {
        bAtomicFlg = atomic32Cas(s_puiLockFlag, LOG_QUEUE_UNLOCK, LOG_QUEUE_LOCK);

        //printf("w Flg=%d\n",bAtomicFlg);

        if(true == bAtomicFlg)
        {
            /* 从SRAM获取队列信息 */
            //memcpy((puint8_t)&s_stQueueInfo, (puint8_t)s_pstQueueInfo, sizeof(LogQueue_t));
            ReadQueueInfoFromSRAM();
            usTail = s_stQueueInfo.usTail;
            //printf("tail=%d\n", usTail);
            if(usTail <= LOG_INFO_SEC9_MAX_INDEX)
            {
                s_pstBaseLogInfo = (LogInfo_t *)LOG_SRAM_LOG_INFO_SEC9_START_ADDR;
                s_pstLogInfo = &s_pstBaseLogInfo[usTail];
            }
            else
            {
                s_pstBaseLogInfo = (LogInfo_t *)LOG_SRAM_LOG_INFO_SECA_START_ADDR;
                s_pstLogInfo = &s_pstBaseLogInfo[usTail-LOG_INFO_SECA_START_INDEX];
            }

            //printf("addr=%x %x %x\n",s_pstLogInfo, &s_pstLogInfo->uiSec, &s_pstLogInfo->uiMicroSec);
            /* 获取并记录系统时间 */
            SysGetSysTime(&stTime);
            s_pstLogInfo->uiSec = stTime.uiSec;
            s_pstLogInfo->uiMicroSec = stTime.uiMicSec;
            //printf("%x %x\n",s_pstLogInfo->uiSec, s_pstLogInfo->uiMicroSec);

            /* 记录id */
            s_pstLogInfo->uiLogID = uiLogID;
            //printf("logid=%d %d\n", usLogID, s_pstLogInfo->usLogID);

            /* 更新队尾 */
            s_stQueueInfo.usTail++;
            if(s_stQueueInfo.usTail >= MAX_LOG_NUM)
            {
                s_stQueueInfo.usTail = 0;
            }

            s_stQueueInfo.usSize++;

            /* 判断队列是否满 */
            if(s_stQueueInfo.usSize >= MAX_LOG_NUM)
            {/* 队列满 */
                s_stQueueInfo.usSize = MAX_LOG_NUM;
                s_stQueueInfo.usQueueState = LOG_QUEUE_FULL;

                /* 更新队头 */
                s_stQueueInfo.usHeader++;
                if(s_stQueueInfo.usHeader >= MAX_LOG_NUM)
                {
                    s_stQueueInfo.usHeader = 0;
                }

                s_stQueueInfo.usRetainHeader++;
                if(s_stQueueInfo.usRetainHeader >= MAX_LOG_NUM)
                {
                    s_stQueueInfo.usRetainHeader = 0;
                }
            }
            else
            {/* 队列未满 */
                s_stQueueInfo.usQueueState = LOG_QUEUE_NOT_EMPTY_NOT_FULL;
#if 0
                if(s_stQueueInfo.usSize >= MAX_LOG_NUM_RETAIN)
                {
                    s_stQueueInfo.usRetainHeader++;
                    if(s_stQueueInfo.usRetainHeader >= MAX_LOG_NUM)
                    {
                        s_stQueueInfo.usRetainHeader = 0;
                    }
                }
#endif
            }

            /* 将队列信息写回SRAM */
            //memcpy((puint8_t)s_pstQueueInfo, (puint8_t)&s_stQueueInfo, sizeof(LogQueue_t));
            WriteQueueInfoToSRAM();
            atomic32Set(s_puiLockFlag, LOG_QUEUE_UNLOCK);
        }
        else
        {
            udelay(LOG_DELAY_TIME_2US);
        }
    }while(true != bAtomicFlg);
}

/*
***********************************************************************************************************************
* 函数名称: LogReadUser
*
* 功能描述: 从队列读取一条日志
*
* 输入参数: pstLogInfo: 待存放日志的指针
*        usLogicPos: 队列中的位置(READ_LOG_FROM_BEGINNING 表示从开头读)
*
* 输出参数: puiNextIndex: 下一条记录的位置
*
* 返 回 值  : -1 队列已经为空
*        -2 最后一条LOG记录
*        -3 输入输出参数错误，读取失败
*        >=0 下一条LOG记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t LogReadUser(LogInfo_t *pstLogInfo, uint16_t usLogicPos, puint32_t puiNextIndex)
{
    int32_t iLogIndex = (int32_t)READ_LOG_QUEUE_FAILED;
    LogInfo_t *pstLogInfoTmp = NULL;
    uint16_t usLastLogIndex = 0;
    bool_t bAtomicFlg = false;

    if((pstLogInfo != NULL) && (puiNextIndex != NULL))
    {
        do
        {
            bAtomicFlg = atomic32Cas(s_puiLockFlag, LOG_QUEUE_UNLOCK, LOG_QUEUE_LOCK);
            //printf("r Flg=%d\n",bAtomicFlg);

            if(true == bAtomicFlg)
            {
                /* 获取队列信息 */
                //memcpy((puint8_t)&s_stQueueInfo, (puint8_t)s_pstQueueInfo, sizeof(LogQueue_t));
                ReadQueueInfoFromSRAM();
                if(LOG_QUEUE_EMPTY != s_stQueueInfo.usQueueState)
                {
                    if(READ_LOG_FROM_BEGINNING == usLogicPos)
                    {
                        usLogicPos = s_stQueueInfo.usHeader;
                    }

                    if((LOG_QUEUE_FULL == s_stQueueInfo.usQueueState) && (usLogicPos >= s_stQueueInfo.usSize))
                    {/* 队列满，逻辑位置不在队列内 */
                        iLogIndex = (int32_t)READ_LOG_QUEUE_FAILED;
                    }
                    else if((LOG_QUEUE_NOT_EMPTY_NOT_FULL == s_stQueueInfo.usQueueState) && (usLogicPos >= s_stQueueInfo.usTail))
                    {/* 队列未满，逻辑位置超出最后一个位置 */
                        iLogIndex = (int32_t)READ_LOG_QUEUE_FAILED;
                    }
                    else
                    {
                        /* 找到物理存储位置 */
                        iLogIndex = (int32_t)usLogicPos;
                        //printf("r index=%d\n",iLogIndex);
                        /* 找到最后一个位置 */
                        if(s_stQueueInfo.usTail > 0)
                        {
                            usLastLogIndex = s_stQueueInfo.usTail -1;
                        }
                        else
                        {
                            /* tail指向逻辑上的第一个元素 */
                            usLastLogIndex = s_stQueueInfo.usSize -1;
                        }

                        if(iLogIndex <= LOG_INFO_SEC9_MAX_INDEX)
                        {
                            s_pstBaseLogInfo = (LogInfo_t *)LOG_SRAM_LOG_INFO_SEC9_START_ADDR;
                            pstLogInfoTmp = &s_pstBaseLogInfo[iLogIndex];
                        }
                        else
                        {
                            s_pstBaseLogInfo = (LogInfo_t *)LOG_SRAM_LOG_INFO_SECA_START_ADDR;
                            pstLogInfoTmp = &s_pstBaseLogInfo[iLogIndex-LOG_INFO_SECA_START_INDEX];
                        }

                        /* 读取日志 */
                        memcpy((puint8_t)pstLogInfo, (puint8_t)pstLogInfoTmp, sizeof(LogInfo_t));

                        if(usLogicPos == usLastLogIndex)
                        {
                            iLogIndex = THE_LAST_LOG_IN_QUEUE;
                        }
                        else
                        {
                            iLogIndex = (usLogicPos + 1) % MAX_LOG_NUM;
                        }
                        *puiNextIndex = (usLogicPos + 1) % MAX_LOG_NUM;
                    }
                }
                else
                {
                    iLogIndex = LOG_QUEUE_IS_EMPTY_NOW;
                }
                atomic32Set(s_puiLockFlag, LOG_QUEUE_UNLOCK);
            }
            else
            {
                udelay(LOG_DELAY_TIME_2US);
            }
        }while(true != bAtomicFlg);
    }
    else
    {
        iLogIndex = READ_LOG_QUEUE_FAILED;
    }
    return iLogIndex;
}


/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: WriteQueueInfoToSRAM
*
* 功能描述: 记录队列信息
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
static void WriteQueueInfoToSRAM(void)
{
    s_pstQueueInfo->usMagic1 = s_stQueueInfo.usMagic1;
    s_pstQueueInfo->usSize = s_stQueueInfo.usSize;     /* 队列大小 */
    s_pstQueueInfo->usHeader = s_stQueueInfo.usHeader;   /* 队头*/
    s_pstQueueInfo->usRetainHeader = s_stQueueInfo.usRetainHeader;   /* 掉电保持队头*/
    s_pstQueueInfo->usTail = s_stQueueInfo.usTail;     /* 队尾 */
    s_pstQueueInfo->usQueueState = s_stQueueInfo.usQueueState;/* 队列空满状态标识 */
    s_pstQueueInfo->usMagic2 = s_stQueueInfo.usMagic2;
}

/*
***********************************************************************************************************************
* 函数名称: ReadQueueInfoFromSRAM
*
* 功能描述: 读取队列信息
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
static void ReadQueueInfoFromSRAM(void)
{
    s_stQueueInfo.usMagic1 = s_pstQueueInfo->usMagic1;
    s_stQueueInfo.usSize = s_pstQueueInfo->usSize;     /* 队列大小 */
    s_stQueueInfo.usHeader = s_pstQueueInfo->usHeader;   /* 队头*/
    s_stQueueInfo.usRetainHeader = s_pstQueueInfo->usRetainHeader;   /* 掉电保持队头*/
    s_stQueueInfo.usTail = s_pstQueueInfo->usTail;     /* 队尾 */
    s_stQueueInfo.usQueueState = s_pstQueueInfo->usQueueState;/* 队列空满状态标识 */
    s_stQueueInfo.usMagic2 = s_pstQueueInfo->usMagic2;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
