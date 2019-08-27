﻿/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 共享内存
*
*  文件名称    : SharedMem.h
*
*  功能简介    : core0与core1间的共享内存
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-15
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"
#include "Srv/Sys/inc/CommSysCommon.h"
#include "SharedComm.h"
#include "SharedConfig.h"
#include "SharedRtData.h"
#include "SharedSOE.h"
#include "LED.h"
#include "SharedWatchdog.h"
#include "SharedHandshake.h"
#include "SharedSafetyComm.h"
#include "SharedRetain.h"
#include "SharedDiagCode.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
//#define get_cpu_no() (*(volatile unsigned int *)(0xffe40090))

#define MAX_OSCP_VAR        (2U)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

typedef enum
{
    PF_CONTINUE = 0x00000000U,          /*系统暂停标志*/
    PF_PAUSE                            /*系统暂停恢复，继续运行标志*/
}Pause_Flag_t;


#pragma pack (1)
typedef struct OSCPCtrlInfoTag
{
    uint32_t uiWRIndex;
    uint32_t uiWRVarNum;
    uint32_t uiPeriodCount;
}OSCPCtrlInfo_t;


typedef struct OSCPVarInfoTag
{
    uint8_t ucTaskID;
    uint8_t ucRefID;
    uint16_t usSize;
    uint32_t uiOffset;
}OSCPVarInfo_t;

typedef struct OSCPCfgHeadTag
{
    uint8_t ucTaskID;
    uint8_t ucTriggerMode;
    uint32_t uiTotalPoints;
    uint16_t usTimeIntervals;
    uint8_t ucReserved[2];
    uint16_t usChannelNum;
}OSCPCfgHead_t;

typedef struct OSCPCfgInfoTag
{
    OSCPCfgHead_t stOSCPCfgHead;
    OSCPVarInfo_t stVar[MAX_OSCP_VAR];
}OSCPCfgInfo_t;

#pragma pack ()

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedMemInit
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
void SharedMemInit(void);
/*
***********************************************************************************************************************
* 函数名称: SharedFlagInit
*
* 功能描述: 共享内存上标志位的初始化。
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
void SharedFlagInit(void);
/*
***********************************************************************************************************************
* 函数名称: SysSetPauseFlag
*
* 功能描述: 设置系统的暂停标志。
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
void SharedSetPauseFlag(Pause_Flag_t emSysPauseFlag,Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SysGetPauseFlag
*
* 功能描述: 获取系统的暂停标志。
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
Pause_Flag_t SharedGetPauseFlag(Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SharedGetIECErrTime
*
* 功能描述: 获取指定任务的IEC运算错误次数。
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
uint8_t SharedGetIECErrTime(Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SharedAddIECErrTime
*
* 功能描述: 指定任务的IEC运算错误次数加1。
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
void SharedAddIECErrTime(Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SharedClearIECErrTime
*
* 功能描述: 清除指定任务的IEC运算错误次数。
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
void SharedClearIECErrTime(Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SharedSetPMState
*
* 功能描述: 设置PM状态。
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
void SharedSetPMState(uint16_t usPMState);
/*
***********************************************************************************************************************
* 函数名称: SharedGetPMState
*
* 功能描述: 获取PM状态。
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
uint16_t SharedGetPMState(void);
/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPCfg
*
* 功能描述: 获取变量示波器配置信息。
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
OSCPCfgInfo_t* SharedGetOSCPCfg(Task_ID_t emTaskID);
/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPCtrl
*
* 功能描述: 获取变量示波器控制信息。
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
OSCPCtrlInfo_t* SharedGetOSCPCtrl(Task_ID_t emTaskID);

/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPBuff
*
* 功能描述: 获取变量示波器存储地址。
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
puint8_t SharedGetOSCPBuff(Task_ID_t emTaskID, uint32_t uiOffset);

#endif /* SHARED_MEM_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
