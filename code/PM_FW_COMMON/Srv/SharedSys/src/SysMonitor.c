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
*  功能简介    :
*
*  作者        : 刘阳
*
*  创建日期    : 2016-04-28
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
#include "Srv/IPBUS/inc/IPBUS.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysOSPMonitorData_t s_stOSPMonitorData[MAX_MOD_NUM];
static uint8_t ucDataProcessSOELastVar[MAX_SOE_VAR_NUM]; /* 软SOE上一拍值 */
static uint32_t s_uiPMUpdateFlg = 0;
static uint8_t s_ucIECShedCtrlFlg[MAX_TASK_NUM];
static bool_t s_bModEnableFlg[MAX_MOD_NUM];


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
* 函数名称: SysMonitorInit
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
void SysMonitorInit(void)
{
    SysOSPMonitorDataInit();
    SysSoftSOEInit();
    memset((puint8_t)(&s_ucIECShedCtrlFlg[0]),0 ,(sizeof(uint8_t)) * MAX_TASK_NUM);
    s_uiPMUpdateFlg = 0;

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysOSPMonitorDataInit
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
void SysOSPMonitorDataInit(void)
{
    /* 初始化core0的用于运算的实时数据区 */
    memset((puint8_t)(&s_stOSPMonitorData[0]),0 ,(sizeof(SysOSPMonitorData_t)) * MAX_MOD_NUM);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetOSPMonitorData
*
* 功能描述: 获取OSP的monitor数据首地址
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
SysOSPMonitorData_t* SysGetOSPMonitorData(uint8_t ucSlaveNo)
{
    SysOSPMonitorData_t* pStartAddr = NULL;

    if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
    {
        pStartAddr = &(s_stOSPMonitorData[ucSlaveNo]);
    }

    return pStartAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SysSoftSOEInit
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
void SysSoftSOEInit(void)
{

    memset((puint8_t)(&ucDataProcessSOELastVar[0]),0 ,(sizeof(uint8_t) * MAX_SOE_VAR_NUM));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSoftSOEInit
*
* 功能描述: 单块生成软SOE记录
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DataProcessSoftSOEInit(Task_ID_t emTaskID, bool_t bCfgFlg)
{
    uint16_t i = 0U;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    uint8_t ucCurrentVar = 0U;
    puint8_t pucVarAddr = NULL;
    uint32_t uiIndex = 0U;
    uint32_t uiOffset = 0U;
    uint8_t ucSOEBlockNo = 0U;
    uint8_t ucSOEBlockIndex = 0U;

    if(true == bCfgFlg)
    {
        pstSOEConfigInfo = SharedGetSOEConfigInfo(emTaskID); /* 获取SOE表 */
    }
    else
    {
        pstSOEConfigInfo = SharedGetSOEConfigInfoWrit(); /* 获取SOE表 */
    }
    
    if((pstSOEConfigInfo != NULL) && (emTaskID < MAX_TASK_NUM))
    {
        for(ucSOEBlockIndex = 0U; ucSOEBlockIndex < pstSOEConfigInfo->ucSoftSOEBlockNum[emTaskID]; ucSOEBlockIndex++)
        {
            ucSOEBlockNo = pstSOEConfigInfo->ucSoftSOEBlockTable[emTaskID][ucSOEBlockIndex];

            uiIndex = pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarIndex;

            for(i = 0; i < pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarNum; i++)
            {
                pucVarAddr = SysGetPRGRtDataStartAddr(pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].ucRefID);
                if(pucVarAddr != NULL)
                {
                    if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                    {
                        uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset / 8U;
                    }
                    else
                    {
                        uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset;
                    }
                    pucVarAddr = &pucVarAddr[uiOffset];
    
                    if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                    {
                        ucCurrentVar = *pucVarAddr;
                        if((ucCurrentVar & (0x01U << (pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset % 8U))) != 0U)
                        {
                            ucCurrentVar = 1U;
                        }
                        else
                        {
                            ucCurrentVar = 0U;
                        }
                    }
                    else
                    {
                        memcpy(&ucCurrentVar, pucVarAddr, (uint32_t)pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize);
                    }
                    
                    ucDataProcessSOELastVar[uiIndex+i] = ucCurrentVar;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessGenerateSoftSOERecord
*
* 功能描述: 单块生成软SOE记录
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DataProcessGenerateSoftSOERecord(Task_ID_t uiTaskID,uint8_t ucSOEBlockNo)
{
    uint16_t i = 0U;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    uint8_t ucCurrentVar = 0U;
    puint8_t pucVarAddr = NULL;
    uint32_t uiIndex = 0U;
    uint32_t uiOffset = 0U;
    SoftSOE_t stSOE;
    SysTime_t stSysTime;

    pstSOEConfigInfo = SharedGetSOEConfigInfo(uiTaskID); /* 获取SOE表 */
    if(pstSOEConfigInfo != NULL)
    {
        if(ucSOEBlockNo < MAX_SOE_BLOCK_NUM)
        {
            uiIndex = pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarIndex;

            for(i = 0; i < pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarNum; i++)
            {
                if((uiIndex+i) < MAX_SOE_VAR_NUM)
                {
                    pucVarAddr = SysGetPRGRtDataStartAddr(pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].ucRefID);
                    if(pucVarAddr != NULL)
                    {
                        if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                        {
                            uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset / 8U;
                        }
                        else
                        {
                            uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset;
                        }

                        pucVarAddr = &pucVarAddr[uiOffset];

                        if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                        {
                            ucCurrentVar = *pucVarAddr;
                            if((ucCurrentVar & (0x01U << (pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset % 8U))) != 0U)
                            {
                                ucCurrentVar = 1U;
                            }
                            else
                            {
                                ucCurrentVar = 0U;
                            }
                        }
                        else
                        {
                            memcpy(&ucCurrentVar, pucVarAddr, (uint32_t)pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize);
                        }

                        if(ucCurrentVar != ucDataProcessSOELastVar[uiIndex+i])
                        {/* 值变化，有软SOE产生 */
                            SysGetSysTime(&stSysTime);
                            stSOE.ucBlockNo = ucSOEBlockNo;
                            stSOE.ucValue = ucCurrentVar;
                            stSOE.usIndex = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usBlockIndex;
                            stSOE.uiSec = stSysTime.uiSec;
                            stSOE.uiMicroSec = stSysTime.uiMicSec;
                            if(stSOE.uiSec < 0xF0000000)
                            {
                                SharedSetSOE(ucSOEBlockNo, stSOE);
                            }
                            ucDataProcessSOELastVar[uiIndex+i] = ucCurrentVar;
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
* 函数名称: SysGetPMUpdateFlg
*
* 功能描述: 获取PM升级标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetPMUpdateFlg(void)
{
    return s_uiPMUpdateFlg;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPMUpdateFlg
*
* 功能描述: 设置PM升级标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPMUpdateFlg(uint32_t uiPMUpdateFlg)
{
     s_uiPMUpdateFlg = uiPMUpdateFlg;
     return;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetIECShedCtrlFlg
*
* 功能描述: 获取指定任务的任务调度控制标志
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
uint8_t SysGetIECShedCtrlFlg(Task_ID_t emTaskID)
{
    return s_ucIECShedCtrlFlg[emTaskID];
}
/*
***********************************************************************************************************************
* 函数名称: SysSetIECShedCtrlFlg
*
* 功能描述: 获取指定任务的任务调度控制标志
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
void SysSetIECShedCtrlFlg(Task_ID_t emTaskID, uint8_t ucIECShedCtrlFlg)
{
    if(emTaskID < MAX_TASK_NUM)
    {
        s_ucIECShedCtrlFlg[emTaskID] = ucIECShedCtrlFlg;
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DataProcessModEnableFlgInit(void)
{
    memset(&s_bModEnableFlg[0], 0x00, sizeof(bool_t)*MAX_MOD_NUM);
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t DataProcessGetModEnableFlg(uint8_t ucModAddr)
{
    bool_t bModEnableFlg = false;
    
    if(ucModAddr < MAX_MOD_NUM)
    {
        bModEnableFlg = s_bModEnableFlg[ucModAddr];
    }
    
    return bModEnableFlg;
}
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DataProcessSetModEnableFlg(uint8_t ucModAddr, bool_t bModEnableFlg)
{
    if((ucModAddr < MAX_MOD_NUM) && 
        ((false == bModEnableFlg) || (true == bModEnableFlg)))
    {
        s_bModEnableFlg[ucModAddr] = bModEnableFlg;
        
    }
    
    return;
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
