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
#include "../inc/LogID.h"
#include "Srv/Sys/inc/CommSys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/


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
* 函数名称: LogInit
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
void LogInit(void)
{
    /* 0为core0,1为core1 */
    if(0==get_cpu_no())
    {
        LogInitCore0();
    }
    else
    {
        LogInitCore1();
        LogInitUser();
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: LogWrite
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
void LogWrite(uint32_t uiLogID)
{
    /* 0为core0,1为core1 */
    if(uiLogID < LOG0_ID_CORE0_BASE)
    {
        if(0==get_cpu_no())
        {
            LogWriteCore0(uiLogID);
        }
        else
        {
            LogWriteCore1(uiLogID);
        }
    }
    else if((uiLogID >= LOG0_ID_CORE0_BASE) && (uiLogID < LOG1_ID_CORE1_BASE))
    {
        LogWriteCore0(uiLogID);
    }
    else if((uiLogID >= LOG1_ID_CORE1_BASE) && (uiLogID < LOG1_ID_CORE1_USER_BASE))
    {
        LogWriteCore1(uiLogID);
    }
    else
    {
        LogWriteUser(uiLogID);
    }
}

/*
***********************************************************************************************************************
* 函数名称: LogRead
*
* 功能描述: 从队列读取一条日志
*
* 输入参数: pstLogInfo: 待存放日志的指针
*        usLogicPos: 队列中的位置(READ_LOG_FROM_BEGINNING 表示从开头读)
*        ucLogType: 日志类型
*
* 输出参数: 无
*
* 返 回 值  : -1 队列已经为空
*        -2 最后一条LOG记录
*        -3 输入输出参数错误，读取失败
*        >=0 下一条LOG记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t LogRead(LogInfo_t *pstLogInfo, uint16_t usLogicPos, uint32_t uiLogType, puint32_t puiNextIndex, \
        SysTime_t* pstTimestamp)
{
    int32_t iLogIndex = (int32_t)READ_LOG_QUEUE_FAILED;

    if(LOG_TYPE_CORE0 == uiLogType)
    {
        if((pstLogInfo != NULL) && (puiNextIndex != NULL))
        {
            iLogIndex = LogReadCore0(pstLogInfo, usLogicPos, puiNextIndex, pstTimestamp);
        }
    }
    else if(LOG_TYPE_CORE1 == uiLogType)
    {
        if((pstLogInfo != NULL) && (puiNextIndex != NULL))
        {
            iLogIndex = LogReadCore1(pstLogInfo, usLogicPos, puiNextIndex, pstTimestamp);
        }
    }
    else
    {
        if((pstLogInfo != NULL) && (puiNextIndex != NULL))
        {
            iLogIndex = LogReadUser(pstLogInfo, usLogicPos, puiNextIndex, pstTimestamp);
        }
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
* 函数名称: LogTest
*
* 功能描述: 测试函数
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
#if 0
void LogTest(void)
{
    LogInfo_t Log;
    int32_t index;
    uint32_t uiNextIndex;
    //puint16_t pusClear = NULL;
    //uint16_t i = 0;

    /* test */
    LogWrite(0x80001111U);

//    LogWrite(0xE0002222U);

    index = LogRead(&Log, 0xFFFF, LOG_TYPE_CORE1, &uiNextIndex);
    printf("0x%x 0x%x 0x%x\n",Log.uiLogID, Log.uiSec, Log.uiMicroSec);
    if(index >= 0)
    {
        do{
            index = LogRead(&Log, index, LOG_TYPE_CORE1, &uiNextIndex);
            if(index < 0)
            {
                break;
            }
            printf("0x%x 0x%x 0x%x\n",Log.uiLogID, Log.uiSec, Log.uiMicroSec);
        }while((index != THE_LAST_LOG_IN_QUEUE) && (index >= 0));
    }

#if 0
    index = LogRead(&Log, 0xFFFF, LOG_TYPE_USER, &uiNextIndex);
    printf("0x%x 0x%x 0x%x\n",Log.uiLogID, Log.uiSec, Log.uiMicroSec);
    if(index >= 0)
    {
        do{
            index = LogRead(&Log, index, LOG_TYPE_USER, &uiNextIndex);
            if(index < 0)
            {
                break;
            }
            printf("0x%x 0x%x 0x%x\n",Log.uiLogID, Log.uiSec, Log.uiMicroSec);
        }while((index != THE_LAST_LOG_IN_QUEUE) && (index >= 0));
    }
#endif

#if 0
    /* clear log Header */
    pusClear = (puint16_t)LOG_SRAM_QUEUE_INFO_SEC5_START_ADDR;
    for(i=0;i<10;i++)
    {
        *pusClear = 0;
        pusClear ++;
    }
    pusClear = (puint16_t)LOG_SRAM_QUEUE_INFO_SEC7_START_ADDR;
    for(i=0;i<10;i++)
    {
        *pusClear = 0;
        pusClear ++;
    }
    pusClear = (puint16_t)LOG_SRAM_QUEUE_INFO_SEC9_START_ADDR;
    for(i=0;i<10;i++)
    {
        *pusClear = 0;
        pusClear ++;
    }
#endif
}
#endif
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
