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
*  文件名称    : SysMonitor.h
*
*  功能简介    : 完成Core1内状态的监控，处理同步模块同步的部分任务（需要Core1处理的）
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-14
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_MONITOR_H_
#define SYS_MONITOR_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"
#include "SysCommon.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

//#define DEBUG_OUT_SYSMONITOR
#ifdef DEBUG_OUT_SYSMONITOR
#define DEBUG_SYSMONITOR(format, args...)  printf ("D_SYSMONI: ") ; printf(format, ##args)
#else
#define DEBUG_SYSMONITOR(format, args...)
#endif

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SysSwitchPosInit
*
* 功能描述: 初始化钥匙开关位置
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
void SysSwitchPosInit(void);
/*
***********************************************************************************************************************
* 函数名称: SysTemperatureInit
*
* 功能描述: 初始化模块温度
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
void SysTemperatureInit(void);
/*
***********************************************************************************************************************
* 函数名称: SysSupplyAlarmBitInit
*
* 功能描述: 初始化电源报警位
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
void SysSupplyAlarmBitInit(void);
/*
***********************************************************************************************************************
* 函数名称: SysPMHardStatusInit
*
* 功能描述: 初始化PM离在线状态
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
void SysPMHardStatusInit(PMController_t emPMId);
/*
***********************************************************************************************************************
* 函数名称: SysReadSwitchPos
*
* 功能描述: 从硬件寄存器获取钥匙开关位置，并写在系统资源
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
uint32_t SysReadSwitchPos(void);
/*
***********************************************************************************************************************
* 函数名称: SysReadTemperature
*
* 功能描述: 从硬件寄存器获取温度，并写在系统资源
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
void SysReadTemperature(void);

/*
***********************************************************************************************************************
* 函数名称: SysReadSupplyAlarmBit
*
* 功能描述: 从硬件寄存器获取电源电压及电池的报警位，并写在系统资源
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
void SysReadSupplyAlarmBit(void);
/*
***********************************************************************************************************************
* 函数名称: SysReadPMHardStatus
*
* 功能描述: 从硬件寄存器获取PM离在线状态，并写在系统资源
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
void SysReadPMHardStatus(PMController_t emPMId);
/*
***********************************************************************************************************************
* 函数名称: SysReadLocalPMID
*
* 功能描述: 从硬件寄存器获取本机PM id，并写在系统资源
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
void SysReadLocalPMID(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetTemperature
*
* 功能描述: 从系统资源获取温度
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
float_t SysGetTemperature(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetSupplyAlarmBit
*
* 功能描述: 从系统资源获取电源报警位
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
uint32_t SysGetSupplyAlarmBit(SupplyType_t emSupplyType);

/*
***********************************************************************************************************************
* 函数名称: SysReadMCUStat
*
* 功能描述: 从硬件寄存器获取MCU状态，并写在系统资源
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
void SysReadMCUStat(void);

/*
***********************************************************************************************************************
* 函数名称: SysReadPPMStat
*
* 功能描述: 从硬件寄存器获取PPM状态，并写在系统资源
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
void SysReadPPMStat(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMHardStatus
*
* 功能描述: 从系统资源获取PM离在线状态
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
uint32_t SysGetPMHardStatus(PMController_t emPMId);

#endif /* SYS_MONITOR_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
