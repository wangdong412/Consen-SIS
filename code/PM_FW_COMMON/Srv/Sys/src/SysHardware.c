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
*  文件名称    : SysHardware.c
*
*  功能简介    :
*
*  作者        : 李琦
*
*  创建日期    : 2016-05-13
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
#include "../inc/SysHardware.h"
#include "Srv/sys/inc/CommSysCommon.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
const float_t fTempFractionalValue[16]={ 0.0000, 0.0625, 0.1250, 0.1875,
                                         0.2500, 0.3125, 0.3750, 0.4375,
                                         0.5000, 0.5625, 0.6250, 0.6875,
                                         0.7500, 0.8125, 0.8750, 0.9375
                                       };

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
* 函数名称: GetTemperature
*
* 功能描述: 获取温度
*
* 输入参数: 无
*
* 输出参数: pfTemp: 指向获取的温度值
*        bpValid: 温度值是否有效
*
* 返 回 值  : 读取温度是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t GetTemperature(float_t *pfTemp, bool_t *bpValid)
{
    bool_t bRet = false;
    bool_t bValid = false;
    uint16_t usRegVal = 0;

    float_t fTemp = 0.0;
    uint8_t ucIndex = 0;
    uint16_t usTemp = 0;
    int16_t iTemp = 0;
    
    if((pfTemp != NULL) && (bpValid != NULL))
    {
        usRegVal = fpga_read(REG_MONITOR_TEMPERATURE);

        /* 判断温度数据是否有效 */
        if((usRegVal & 0x8000) != 0)
        {
            /* 温度数据有效 */
            bValid = true;

            if((usRegVal & 0x4000) == 0)
            {/* 温度为正 */

                /* 计算温度*/
                iTemp = (usRegVal & 0x07F0)>>4;
                ucIndex = usRegVal & 0x000F;
                fTemp = fTempFractionalValue[ucIndex] + (float_t) iTemp;
            }
            else
            {/* 温度为负 */
                usTemp = (usRegVal & 0x07F0)>>4;
                iTemp = (int16_t)((uint16_t)0xFF80 | usTemp);
                ucIndex = usRegVal & 0x000F;
                fTemp = fTempFractionalValue[ucIndex] + (float_t) iTemp;
            }

            *pfTemp = fTemp;

        }
        else
        {
            bValid = false;
        }

        *bpValid = bValid;

        bRet = true;
    }

    return bRet;
}


/*
***********************************************************************************************************************
* 函数名称: GetSwitchKeyPos
*
* 功能描述: 获取钥匙开关状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 钥匙开关状态（run/stop/prog/init）
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t GetSwitchKeyPos(void)
{
    uint16_t usRegVal = 0;
    uint16_t usKeyPos = 0;

    usRegVal = fpga_read(REG_SWITCH_KEY);
    usKeyPos = (usRegVal & 0x07);

    return usKeyPos;
}

/*
***********************************************************************************************************************
* 函数名称: Get24Volt1AlarmBit
*
* 功能描述: 获取24v1报警位
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 故障/正常
*
* 注意事项: 无
***********************************************************************************************************************
*/
Alarm_t Get24Volt1AlarmBit(void)
{
    uint16_t usRegVal = 0;
    Alarm_t emAlarmBit = ALARM_BIT_OK;

    usRegVal = fpga_read(REG_MONITOR_STATE);
    emAlarmBit = (Alarm_t)((usRegVal & 0x02)>>1);

    return emAlarmBit;
}

/*
***********************************************************************************************************************
* 函数名称: Get24Volt2AlarmBit
*
* 功能描述: 获取24v2报警位
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 故障/正常
*
* 注意事项: 无
***********************************************************************************************************************
*/
Alarm_t Get24Volt2AlarmBit(void)
{
    uint16_t usRegVal = 0;
    Alarm_t emAlarmBit = ALARM_BIT_OK;

    usRegVal = fpga_read(REG_MONITOR_STATE);
    emAlarmBit = (Alarm_t)((usRegVal & 0x04)>>2);

    return emAlarmBit;
}

/*
***********************************************************************************************************************
* 函数名称: GetBatteryAlarmBit
*
* 功能描述: 获取电池报警位
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 故障/正常
*
* 注意事项: 无
***********************************************************************************************************************
*/
Alarm_t GetBatteryAlarmBit(void)
{
    uint16_t usRegVal = 0;
    Alarm_t emAlarmBit = ALARM_BIT_OK;

    usRegVal = fpga_read(REG_MONITOR_STATE);
    emAlarmBit = (Alarm_t)(usRegVal & 0x01);

    return emAlarmBit;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateStateLED
*
* 功能描述: 更新状态灯寄存器
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void UpdateStateLED(uint16_t usState)
{
    fpga_write(REG_CONTROL_STATE_LED, usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateKeyLED
*
* 功能描述: 更新钥匙灯寄存器
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void UpdateKeyLED(uint16_t usState)
{
    fpga_write(REG_CONTROL_KEY_LED, usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateComLED
*
* 功能描述: 更新通讯灯寄存器
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void UpdateComLED(uint16_t usState)
{
    fpga_write(REG_CONTROL_COM_LED, usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetLocalPMAddr
*
* 功能描述: 设置本机PM地址到系统资源
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SetLocalPMAddr(void)
{
    bool_t bRet = false;
    uint16_t usRegVal = 0;
    uint16_t usAddrA = 0;
    uint16_t usAddrB = 0;
    PMController_t emLocalID = INVALID_PM_ID;

    usRegVal = fpga_read(REG_SLOT_ADDR);
    usAddrA = (usRegVal&0x03);
    usAddrB = ((usRegVal>>8)&0x03);

    if(usAddrA == usAddrB)
    {
        fpga_write(REG_STATION_ADDR, usAddrA);

        if(0x0000U == usAddrA)
        {
            emLocalID = PMA;
        }
        else if(0x0001U == usAddrA)
        {
            emLocalID = PMB;
        }
        else if(0x0002U == usAddrA)
        {
            emLocalID = PMC;
        }
        else
        {
            emLocalID = INVALID_PM_ID;
        }
        SharedSetLocalPMID(emLocalID);

        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetMCUMonitorState
*
* 功能描述: 读取MCU状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : Bit0: MCU状态位。0：无故障，1：有故障
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetMCUMonitorState(void)
{
    uint16_t usRegVal = 0;
    uint16_t usState = 0;

    usRegVal = fpga_read(REG_MCU_MONITOR);
    usState = (usRegVal & 0x01);

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: GetLocalPMStabState
*
* 功能描述: 判断插稳状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : true：稳；false：不稳
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t GetLocalPMStabState(void)
{
    bool_t bRet = false;
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_SLOT_ADDR);
    if(((usRegVal&0x80) == 0x80) && ((usRegVal&0x8000) == 0x8000))
    {
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetAlarmLED
*
* 功能描述: 设置Alarm灯
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetAlarmLED(uint16_t usState)
{
    fpga_write(REG_ALARM, usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSyncTimeLow32
*
* 功能描述: 获取同步时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 低32位时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetSyncTimeLow32(void)
{
//    uint16_t usRegValLow1 = 0;       /*bit0~15*/
//    uint16_t usRegValLow2 = 0;       /*bit0~15*/
//    uint16_t usRegValHigh = 0;      /* bit16~31 */
//    uint32_t uiSyncTime1 = 0;
//    uint32_t uiSyncTime2 = 0;
//    uint32_t uiSyncTime = 0;

    /* 读取一次 */
//    usRegValLow1 = fpga_read(REG_SYNC_TIME1);
//    usRegValHigh = fpga_read(REG_SYNC_TIME2);

//    uiSyncTime1 = (uint32_t)usRegValHigh;
//    uiSyncTime1 = (uiSyncTime1<<16);
//    uiSyncTime1 += (uint32_t)usRegValLow1;

    /* 再读取一次 */
//    usRegValLow2 = fpga_read(REG_SYNC_TIME1);
//    usRegValHigh = fpga_read(REG_SYNC_TIME2);

//    uiSyncTime2 = (uint32_t)usRegValHigh;
//    uiSyncTime2 = (uiSyncTime2<<16);
//    uiSyncTime2 += (uint32_t)usRegValLow2;

    /* 若低16位溢出则用第二次读取的值，否则用第一次读取的值 */
//    if(usRegValLow1 > usRegValLow2)
//    {
//        uiSyncTime = uiSyncTime2;
//    }
//    else
//    {
//        uiSyncTime = uiSyncTime1;
//    }

//    return uiSyncTime;


    uint32_t uiVal = 0;
    uint32_t uiRegVal[2];
    uint32_t uiRegVal_1 = 0;


    uiRegVal[1] = fpga_read(REG_SYNC_TIME2);
    uiRegVal[0] = fpga_read(REG_SYNC_TIME1);


    uiRegVal_1 = fpga_read(REG_SYNC_TIME2);


    if(uiRegVal_1 != uiRegVal[1])
    {
        uiRegVal[1] = fpga_read(REG_SYNC_TIME2);
        uiRegVal[0] = fpga_read(REG_SYNC_TIME1);
    }


    uiVal |= uiRegVal[0];
    uiVal |= (uiRegVal[1]<<16);


    return uiVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSyncTimeHigh16
*
* 功能描述: 获取同步时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 高16位时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetSyncTimeHigh16(void)
{
    uint16_t usRegVal = 0;       /*bit32~47*/

    usRegVal = fpga_read(REG_SYNC_TIME3);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetSyncTimeState
*
* 功能描述: 获取同步时间状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 32位时间
*
* 注意事项: "Bit0：0：同步校准未完成，1：同步校准已完成"
***********************************************************************************************************************
*/
uint16_t SysGetSyncTimeState(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_SYNC_TIME_STATE);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetSyncAdjustTimeLow32
*
* 功能描述: 设置同步校时时间
*
* 输入参数: 低32位时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetSyncAdjustTimeLow32(uint32_t uiAdjustTime)
{
    uint16_t usRegVal = 0;

    usRegVal = (uint16_t)(uiAdjustTime);
    fpga_write(REG_SYNC_ADJUST_TIME1, usRegVal);

    usRegVal = (uint16_t)(uiAdjustTime >> 16);
    fpga_write(REG_SYNC_ADJUST_TIME2, usRegVal);

    fpga_write(REG_SYNC_ADJUST_TIME3, 0);

    return ;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetSyncAdjustTimeHigh16
*
* 功能描述: 设置同步校时时间
*
* 输入参数: 高16位时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetSyncAdjustTimeHigh16(uint16_t usAdjustTime)
{
    fpga_write(REG_SYNC_ADJUST_TIME3, usAdjustTime);

    return ;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetSyncAdjustTimeEnable
*
* 功能描述: 设置同步校时时间使能
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
void SysSetSyncAdjustTimeEnable(void)
{
    uint16_t usRegVal = 1; /* "Bit0:0：无效，1：使能同步校准" */

    fpga_write(REG_SYNC_ADJUST_TIME_ENABLE, usRegVal);

    return ;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModularOnLine
*
* 功能描述: 模块在线状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 在线状态
*
* 注意事项: "Bit1：对模块2主备状态。0：未在线，1：在线"
*         "Bit0：对模块1主备状态。0：未在线，1：在线"
***********************************************************************************************************************
*/
uint16_t SysGetModularOnLine(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MODULAR_ON_LINE);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetTimerMasterEnable
*
* 功能描述: 设置升主使能
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 在线状态
*
* 注意事项: 写入0xA5A5指示升主使能，写入0x5A5A指示降备使能，写入其他值无效。
* 
***********************************************************************************************************************
*/
void SysSetTimerMasterEnable(uint16_t usRegVal)
{

    fpga_write(REG_MAIN_ENABLE,usRegVal);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetSWRuninOK
*
* 功能描述: 设置软件运行OK
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 在线状态
*
* 注意事项: 写入0xA5A5指示软件运行OK，写入其他值指示软件NOK。
* 
***********************************************************************************************************************
*/
void SysSetSWRuninOK(uint16_t usRegVal)
{

    fpga_write(REG_SW_RUNING_OK,usRegVal);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetClkDiagVal
*
* 功能描述: 时钟诊断寄存器
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : Bit15-1：保留 Bit0：时钟诊断结果。0：故障，1：正常
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetClkDiagVal(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_CLK_DIAG_VAR);

    usRegVal = usRegVal & 0x01;

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetMainBackupState
*
* 功能描述: 获取同步时钟准备状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 同步时钟准备状态
*
* 注意事项: "Bit2：对模块2主备状态。0：备用，1：主用"
*        "Bit1：对模块1主备状态。0：备用，1：主用"
*        "Bit0：本模块主备状态。0：备用，1：主用"
* 
***********************************************************************************************************************
*/
uint16_t SysGetMainBackupState(void)
{
    
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MAIN_BACKUP_STATE);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSystemtime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间（单位：微秒）
*
* 注意事项:
*
***********************************************************************************************************************
*/
ulong64_t SysGetSystemtime(void)
{
    ulong64_t ulTime = 0;
    ulong64_t ulTime1 = 0;
    ulong64_t ulRegVal = 0;

    do
    {
        /* 读一次，若最低16位快要溢出时，则等待其溢出完成 */
        ulTime1 = fpga_read(REG_SOE_TIME1);
        ulRegVal = fpga_read(REG_SOE_TIME2);
        ulTime = ulTime1|(ulRegVal<<16);
        ulRegVal = fpga_read(REG_SOE_TIME3);
        ulTime = ulTime|(ulRegVal<<32);
        ulRegVal = fpga_read(REG_SOE_TIME4);
        ulTime = ulTime|(ulRegVal<<48);
    }while(ulTime1 > 65525);

    return ulTime;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSysTime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: pstSysTime 指向系统时间
*
* 返 回 值  : 无
*
* 注意事项:
*
***********************************************************************************************************************
*/
void SysGetSysTime(SysTime_t *pstSysTime)
{
    ulong64_t ulTime = 0;

    ulTime = SysGetSystemtime();

    if(pstSysTime != NULL)
    {
        if(ulTime/1000000 < 0xFFFFFFFF)
        {
            pstSysTime->uiSec = (uint32_t)(ulTime / 1000000);
            pstSysTime->uiMicSec = (uint32_t)(ulTime % 1000000);
        }
        else
        {
            pstSysTime->uiSec = 0xFFFFFFFF;
            pstSysTime->uiMicSec = 0;
        }
    }
}
/*
***********************************************************************************************************************
* 函数名称: SysAdjustSystemTime
*
* 功能描述: 校准系统时间
*
* 输入参数: ulAdjustTime 校准时间（单位：微秒）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysAdjustSystemTime(ulong64_t ulAdjustTime)
{
    uint16_t usAdjustTime = 0;

    usAdjustTime = (ulAdjustTime & 0xFFFF);
    fpga_write(REG_SOE_ADJUST_TIME1, usAdjustTime);

    usAdjustTime = ((ulAdjustTime>>16) & 0xFFFF);
    fpga_write(REG_SOE_ADJUST_TIME2, usAdjustTime);

    usAdjustTime = ((ulAdjustTime>>32) & 0xFFFF);
    fpga_write(REG_SOE_ADJUST_TIME3, usAdjustTime);

    usAdjustTime = ((ulAdjustTime>>48) & 0xFFFF);
    fpga_write(REG_SOE_ADJUST_TIME4, usAdjustTime);

    return ;
}

/*
***********************************************************************************************************************
* 函数名称: SysAdjustSystemTimeEnable
*
* 功能描述: 校准系统时间使能
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
void SysAdjustSystemTimeEnable(void)
{
    fpga_write(REG_ENABLE_SOE_ADJUST_TIME, ENABLE_SYSTEM_ADJUST_TIME);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSelectAdjustTimePulseSrc
*
* 功能描述: 校时脉冲源选择寄存器
*
* 输入参数: usPulseSrc: bit1-0:00：无外部校时源,01：主机架校时源，10：通讯模块校时源
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: SYSTEM_ADJUST_TIME_PULSE_SRC_MAIN/SYSTEM_ADJUST_TIME_PULSE_SRC_CM
***********************************************************************************************************************
*/
void SysSelectAdjustTimePulseSrc(uint16_t usPulseSrc)
{
    fpga_write(REG_ADJUST_TIME_PULSE_SRC, (usPulseSrc & 0x0003));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSelectAdjustTimePulsePolarity
*
* 功能描述: 校时脉冲源极性选择
*
* 输入参数: bit0:0：正脉冲,1：负脉冲
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSelectAdjustTimePulsePolarity(uint16_t usPulsePolarity)
{
    fpga_write(REG_ADJUST_TIME_PULSE_POLARITY, (usPulsePolarity & 0x0001));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysWatchdogEnable
*
* 功能描述: 使能看门狗
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
void SysWatchdogEnable(void)
{
    fpga_write(REG_WTD_ENABLE, ENABLE_WTD);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysWatchdogConfig
*
* 功能描述: 看门狗配置权限
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: WTD_CONFIG_ENABLE/WTD_CONFIG_DISABLE
***********************************************************************************************************************
*/
void SysWatchdogConfig(uint16_t usCfg)
{
    fpga_write(REG_WTD_CONFIG, (usCfg&0x0001));
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogLowLimit
*
* 功能描述: 设置看门狗下限值
*
* 输入参数: 下限（单位：100us）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogLowLimit(uint16_t usLowLimit)
{
    fpga_write(REG_WTD_LOW_LIMT, usLowLimit);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogHighLimit
*
* 功能描述: 设置看门狗上限值
*
* 输入参数: 上限（单位：100us）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogHighLimit(uint16_t usHighLimit)
{
    fpga_write(REG_WTD_HIGH_LIMT, usHighLimit);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetWatchdogTestResult
*
* 功能描述: 获取看门狗自检结果
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 自检结果 0正常  1异常
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetWatchdogTestResult(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_WTD_TEST_RESULT);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAModID
*
* 功能描述: 获取FPGA模块ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA模块ID
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAModID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_FPGA_MOD_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAHardwareVer
*
* 功能描述: 获取FPGA硬件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA硬件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAHardwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_FPGA_HW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAChipID
*
* 功能描述: 获取FPGA芯片位号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA芯片位号
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAChipID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_FPGA_CHIP_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAFirmwareVer
*
* 功能描述: 获取FPGA固件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA固件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAFirmwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_FPGA_FW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAFirmwareSubVer
*
* 功能描述: 获取FPGA固件子版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA固件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAFirmwareSubVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_FPGA_FW_SUB_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUModID
*
* 功能描述: 获取MCU模块ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU模块ID
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUModID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MCU_MOD_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUHardwareVer
*
* 功能描述: 获取MCU硬件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU硬件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUHardwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MCU_HW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUChipID
*
* 功能描述: 获取MCU芯片位号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU芯片位号
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUChipID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MCU_CHIP_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUFirmwareVer
*
* 功能描述: 获取MCU固件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU固件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUFirmwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = fpga_read(REG_MCU_FW_VER);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFreeCounterValue
*
* 功能描述: 获取自由计数器的值(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 自由计数器的值
*
* 注意事项:
***********************************************************************************************************************
*/
long64_t SysGetFreeCounterValue(void)
{
    long64_t ulVal = 0;
    long64_t ulRegVal[4];
    long64_t ulRegVal_1;

    ulRegVal[3] = fpga_read(REG_FREE_COUNTER_4);
    ulRegVal[2] = fpga_read(REG_FREE_COUNTER_3);
    ulRegVal[1] = fpga_read(REG_FREE_COUNTER_2);
    ulRegVal[0] = fpga_read(REG_FREE_COUNTER_1);

    ulRegVal_1 = fpga_read(REG_FREE_COUNTER_2);

    if(ulRegVal_1 != ulRegVal[1])
    {
        ulRegVal[3] = fpga_read(REG_FREE_COUNTER_4);
        ulRegVal[2] = fpga_read(REG_FREE_COUNTER_3);
        ulRegVal[1] = fpga_read(REG_FREE_COUNTER_2);
        ulRegVal[0] = fpga_read(REG_FREE_COUNTER_1);
    }

    ulVal = ulRegVal[0];
    ulVal |= (ulRegVal[1]<<16);
    ulVal |= (ulRegVal[2]<<32);
    ulVal |= (ulRegVal[3]<<48);

    return ulVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFreeCounterVal
*
* 功能描述: 获取自由计数器的值(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: pstSysFCVal
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysGetFreeCounterVal(SysFCVal_t *pstSysFCVal)
{
    ulong64_t ulTime = 0;

    ulTime = SysGetFreeCounterValue();

    if(pstSysFCVal != NULL)
    {
        if(ulTime/1000000 < 0xFFFFFFFF)
        {
            pstSysFCVal->uiSec = (uint32_t)(ulTime / 1000000);
            pstSysFCVal->uiMicSec = (uint32_t)(ulTime % 1000000);
        }
        else
        {
            pstSysFCVal->uiSec = 0xFFFFFFFF;
            pstSysFCVal->uiMicSec = 0;
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: SysSetTimingPulse
*
* 功能描述: 设置校时脉冲时间校准寄存器并使能(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetTimingPulse(void)
{
    uint32_t uiTime = 0;
    ulong64_t ulTime = SysGetSystemtime();
    uint16_t usTime[2] = {0};

    uiTime = ulTime % (60*1000000);
    
    usTime[0] = uiTime & 0xffff;
    usTime[1] = (uiTime >> 16) & 0xffff;

    fpga_write(REG_ADJUST_TIME1, usTime[0]);
    fpga_write(REG_ADJUST_TIME2, usTime[1]);
    fpga_write(REG_ADJUST_TIME_ENABLE, 0x0001);
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetTimingPulse
*
* 功能描述: 获取校时脉冲时间校准寄存器的值(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t SysGetTimingPulse(void)
{
    uint32_t uiTime = 0;
    uint32_t uiRegVal = 0;

    uiTime = fpga_read(REG_ADJUST_TIME1);
    uiRegVal = fpga_read(REG_ADJUST_TIME2);
    uiTime = uiTime | (uiRegVal << 16);

    return uiTime;
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
