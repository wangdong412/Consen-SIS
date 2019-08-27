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
#include "../inc/SysMonitor.h"


/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static float_t s_fTemperature;
static uint32_t s_uiSupplyVoltage;
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
* 函数名称: SysSupplyVoltageInit
*
* 功能描述: 初始化电源电压
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
void SysSupplyVoltageInit(void)
{
    s_uiSupplyVoltage = 0U;

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
void SysReadSwitchPos(void)
{
    uint32_t uiSwitchPos = 0;

    /*从硬件寄存器读取数值*/
    uiSwitchPos = GetSwitchKeyPos();
    
    //DEBUG_SYSMONITOR("&&&&&&&  usSwitchPos is : %x \n",usSwitchPos);
    
    SharedSetSwitchPos(uiSwitchPos);


    return;


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

    /*从硬件寄存器读取数值*/
    if(true == GetTemperature(&fTemperature, &bValid))
    {
        /* 数值有效 */
        if(true == bValid)
        {
            s_fTemperature = fTemperature;
        }
    }

    return;

}
/*
***********************************************************************************************************************
* 函数名称: SysReadSupplyVoltage
*
* 功能描述: 从硬件寄存器获取电源电压，并写在系统资源
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
void SysReadSupplyVoltage(void)
{
    uint32_t uiSupplyVoltage;

    /*从硬件寄存器读取数值*/
    /*从硬件寄存器读取数值*/
    s_uiSupplyVoltage = uiSupplyVoltage;

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
    uint32_t uiPMHardStatus;

    /*从硬件寄存器读取数值*/
    /*从硬件寄存器读取数值*/

    s_uiPMHardStatus[emPMId] = uiPMHardStatus;

    return;
}
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
void SysReadLocalPMID(void)
{
    PMController_t emLocalID = PMA;
    uint16_t usLocalPMAddr = 0U;

    /*从硬件寄存器读取数值*/
    usLocalPMAddr = GetLocalPMAddr();
    if(0x0000 == usLocalPMAddr)
    {
        emLocalID = PMA;
    }
    else if(0x0001 == usLocalPMAddr)
    {
        emLocalID = PMB;
    }
    else if(0x0002 == usLocalPMAddr)
    {
        emLocalID = PMC;
    }
    else
    {
        emLocalID = INVALID_PM_ID;
    }

    SharedSetLocalPMID(emLocalID);

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
* 函数名称: SysGetSupplyVoltage
*
* 功能描述: 从系统资源获取电源电压
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
uint32_t SysGetSupplyVoltage(void)
{
    return s_uiSupplyVoltage;

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