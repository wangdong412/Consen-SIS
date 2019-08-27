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
*  文件名称    : SysMonitor.c
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

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "App/Main/inc/Task.h"
#include <Srv/Scheduler/include/stdio.h>
#include "../inc/SysMonitor.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static float_t s_fTemperature;
static Alarm_t s_emSupplyAlarmBit[NUM_OF_SUPPLY_TYPE];
static uint32_t s_uiPMHardStatus[NUM_OF_PM];

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
void SysSwitchPosInit(void)
{
    SharedSwitchPosInit();

    return;
}
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
void SysTemperatureInit(void)
{
    s_fTemperature = 0;

    return;

}
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
void SysSupplyAlarmBitInit(void)
{
    s_emSupplyAlarmBit[ST_24V1] = 0U;
    s_emSupplyAlarmBit[ST_24V2] = 0U;
    s_emSupplyAlarmBit[ST_BTRY] = 0U;

    return;

}
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
void SysPMHardStatusInit(PMController_t emPMId)
{
    s_uiPMHardStatus[emPMId] = WS_OFFLINE;

    return;
}
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
uint32_t SysReadSwitchPos(void)
{
    uint32_t uiSwitchPos = 0;

    /*从硬件寄存器读取数值*/
    uiSwitchPos = GetSwitchKeyPos();

    return uiSwitchPos;
}
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
void SysReadTemperature(void)
{
    float_t fTemperature = 0;
    bool_t bValid = false;
    static uint32_t uiCount = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    
    uiCount++;
    /*从硬件寄存器读取数值*/
    if(true == GetTemperature(&fTemperature, &bValid))
    {
        /* 数值有效 */
        if(true == bValid)
        {
            s_fTemperature = fTemperature;
            pstAppend->stPMState.stPMFirmwareState.fTemperatureMonitor = fTemperature;

            if(uiCount%1200 == 0)//liqi 5ms
            {
                printf("t=%d\n",(int32_t)s_fTemperature);
            }
        }
    }

    return;

}
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
void SysReadSupplyAlarmBit(void)
{
    Alarm_t emSupplyAlarmBit = 0U;
    static uint32_t uiCount = 0;
    SharedAppend_t *pstAppend = NULL;
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MONITOR_STATE);
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    
    uiCount++;
    /*从硬件寄存器读取数值*/
    emSupplyAlarmBit = Get24Volt1AlarmBit();
    s_emSupplyAlarmBit[ST_24V1] = emSupplyAlarmBit;
    
    emSupplyAlarmBit = Get24Volt2AlarmBit();
    s_emSupplyAlarmBit[ST_24V2] = emSupplyAlarmBit;
    
    emSupplyAlarmBit = GetBatteryAlarmBit();
    s_emSupplyAlarmBit[ST_BTRY] = emSupplyAlarmBit;
    pstAppend->stPMState.stPMFirmwareState.usPowerMonitor = usRegVal;

    if(uiCount%1200 == 0)//liqi 5ms
    {
        #ifdef CONTROL_CYCLE_20MS
        //printf("V1Al=%d V2Al=%d BAl=%d\n",s_emSupplyAlarmBit[ST_24V1],s_emSupplyAlarmBit[ST_24V2],s_emSupplyAlarmBit[ST_BTRY]);//liqi 5ms
        #endif
    }
    return;

}

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
void SysReadMCUStat(void)
{
    SharedAppend_t *pstAppend = NULL;
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MCU_MONITOR);
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    pstAppend->stPMState.stPMFirmwareState.usMCUMonitor = usRegVal;

    return;
}

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
void SysReadPPMStat(void)
{
    SharedAppend_t *pstAppend = NULL;
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_PPM_MONITOR);
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    pstAppend->stPMState.stPMFirmwareState.usPPMMonitor = usRegVal;

    return;
}

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
void SysReadPMHardStatus(PMController_t emPMId)
{
    uint16_t usPMHardStatus;

    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    
    /*从硬件寄存器读取数值*/
    usPMHardStatus = SysGetModularOnLine();
    /*从硬件寄存器读取数值*/
    if(emPMId == emLocalPMId)
    {
        s_uiPMHardStatus[emPMId] = 0x0001;
    }
    if(emPMId == emNextPMId)
    {
        s_uiPMHardStatus[emPMId] = usPMHardStatus & BITMASK(0);
    }
    if(emPMId == emPrePMId)
    {
        s_uiPMHardStatus[emPMId] = usPMHardStatus & BITMASK(1);
    }

    return;
}


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
float_t SysGetTemperature(void)
{
    return s_fTemperature;

}
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
uint32_t SysGetSupplyAlarmBit(SupplyType_t emSupplyType)
{
    return s_emSupplyAlarmBit[emSupplyType];

}
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
uint32_t SysGetPMHardStatus(PMController_t emPMId)
{
    return s_uiPMHardStatus[emPMId];
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
