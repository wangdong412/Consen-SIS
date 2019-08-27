/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CM_FW通信公共部分
*
*  文件名称    : ComCommon.c
*
*  功能简介    : 通信公共部分
*
*  作者        : 王东
*
*  创建日期    : 2016-02-22
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
#include <string.h>
#include <stdio.h>
#include "../inc/LogID.h"
#include "Log/inc/log.h"
#include "../inc/ExterComm.h"
#include "../inc/ComCommon.h"

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

/* Modbus Master Info. */
static bool_t GetModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo, puint32_t puiOffset, bool_t *pbComplete );
static void SwitchModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo );
static void ClearCurModbusMstrCfg( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo );

/* Check Valid */
static bool_t CheckCurModbusMstrInfo( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo, uint8_t ucCMSlotID );
static bool_t CheckModbusMstrPortCfg( ModbusMasterPort_t const *pstPortCfg, CommPort_t emPort,
                                      ModbusMasterBlock_t const pstMBMstrBlk[]);
static bool_t CheckNetModbusSlaveChgInfo( ModbusSlaveChange_t const *pstChg );
static bool_t CheckModbusMstrBlockInfo( ModbusMasterBlock_t const *pstBlk, CommPort_t emPort );
static bool_t CheckModbusMstrBlkHead( ModbusMasterBlockHead_t const *pstBlkHead, CommPort_t emPort );

/* Task Configuration Info. */
static bool_t CheckTaskCfgInfo( TaskConfigInfo_t const *pstTaskCfgInfo );
static bool_t SwitchTaskCfgInfo( TaskInfo_t *pstTaskInfo, TaskConfigInfo_t const *pstTaskCfgInfo );
static bool_t CompareRTData( PMController_t emPM1, PMController_t emPM2, SysRtDataAreaType_t emDataArea );

/* Time Sync */
static bool_t CheckTimeSyncCfg( NTPTCPConfigInfo_t const *pstTSyncCfg );
static bool_t SwitchTimeSyncCfg( TimeSyncCfgInfo_t *pstTimeSyncInfo, NTPTCPConfigInfo_t const *pstTSyncCfg );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SwitchToSWType
*
* 功能描述: 将通讯类型 转换为 软件类型
*
* 输入参数: emCommType：通讯类型
*
* 输出参数: 无
*
* 返 回 值: 转换后的软件类型
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysSoftwareType_t SwitchToSWType( CommType_t emCommType )
{
    SysSoftwareType_t emSWType = -1;

    switch( emCommType )
    {
        case TYPE_SOE:
        {
            emSWType = APP_SOE;
        }
        break;

        case TYPE_AMS:
        {
            emSWType = APP_AMS;
        }
        break;

        case TYPE_CLIENT:
        {
            emSWType = APP_CLIENT;
        }
        break;

        case TYPE_OPC:
        {
            emSWType = APP_OPC;
        }
        break;

        case TYPE_CS1131:
        {
            emSWType = APP_CS1131;
        }
        break;

        default:
        {
            emSWType = -1;
        }
        break;
    }

    return emSWType;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchPMID
*
* 功能描述: 将选取结果转换为PM模块ID
*
* 输入参数: emSelPMID：选取的PMID
*
* 输出参数: 无
*
* 返 回 值: 转换后的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

PMController_t SwitchPMID( SelPMID_t emSelPMID )
{
    PMController_t emPMID = INVALID_PM_ID;

    if( SELPM_A == emSelPMID )
    {
        emPMID = PMA;
    }
    else if( SELPM_B == emSelPMID )
    {
        emPMID = PMB;
    }
    else if( SELPM_C == emSelPMID )
    {
        emPMID = PMC;
    }
    else
    {
        emPMID = INVALID_PM_ID;
    }

    return emPMID;
}

/*
***********************************************************************************************************************
* 函数名称: Send
*
* 功能描述: 发送数据
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucIndex：上位机软件/Modbus TCP主站的索引号，其他发送类型填零。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t Send( uint8_t const pucData[], uint16_t usDataLen,
              uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
              CommType_t emSendType, uint8_t ucIndex )

{
    uint8_t ucRetryCnts = 0U;
    uint8_t ucResult = (uint8_t)(EPARA);

    while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
    {
        ucResult = ExternalCommSend( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
        if( ucResult != (uint8_t)(ENOTREADY))
        {
            break;
        }

        DEBUG_RTS( "ExtCommSend Retry %d\n", ucRetryCnts );
        ucRetryCnts++;
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: HandleRTDataVer
*
* 功能描述: 检查实时数据的版本是否变化，若变化，则更新本地保存的实时数据版本
*
* 输入参数: pstLocalRTDataVer：指向本地保存的实时数据版本信息
*
* 输出参数: 无
*
* 返 回 值: 实时数据版本是否变化，true-变化；false-未变化
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleRTDataVer( RTDataVerInfo_t *pstLocalRTDataVer )
{
    bool_t bChange = false;
    RTDataVerInfo_t stRTDataVer;

    if( pstLocalRTDataVer != NULL )
    {
        if( SysGetRTDataVersion( &stRTDataVer ))
        {
            /*DEBUG_RTS( "RTDataVer: uiProjID: %d, uiMainVer: %d, uiMinorVer: %d, uiProjCRC: %d.\n", \
                        stRTDataVer.uiProjID, stRTDataVer.uiMainVer, stRTDataVer.uiMinorVer, stRTDataVer.uiProjCRC );*/

            if((stRTDataVer.uiProjID != pstLocalRTDataVer->uiProjID) ||
               (stRTDataVer.uiMainVer != pstLocalRTDataVer->uiMainVer) ||
               (stRTDataVer.uiMinorVer != pstLocalRTDataVer->uiMinorVer) ||
               (stRTDataVer.uiProjCRC != pstLocalRTDataVer->uiProjCRC))
            {
                bChange = true;
                memcpy( pstLocalRTDataVer, &stRTDataVer, sizeof(RTDataVerInfo_t));
            }
        }
    }

    return bChange;
}

/*
***********************************************************************************************************************
* 函数名称: InitModbusMstrInfo
*
* 功能描述: 初始化本地保存的 Modbus 主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向本地保存的Modbus 主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo )
{
    if( pstMBMstrCfgInfo != NULL )
    {
        memset( pstMBMstrCfgInfo, 0, sizeof(ModbusMstrCfgInfo_t));
        pstMBMstrCfgInfo->bCopying = false;
        pstMBMstrCfgInfo->bValid = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearModbusMstrInfo
*
* 功能描述: 清除本地保存的 Modbus 主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向本地保存的Modbus 主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void ClearModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo )
{
    uint32_t uiLen = 0U;
    uint8_t ucVerIndex = 0U;
    ModbusMasterInfo_t *pstMBMstrInfo = NULL;

    if( pstMBMstrCfgInfo != NULL )
    {
        uiLen = sizeof(ModbusMstrCfgInfo_t) - sizeof(ModbusMasterInfo_t)*(uint32_t)(LOCAL_CFG_VER_CNT);
        memset( pstMBMstrCfgInfo, 0, uiLen );

        /* Flag */
        pstMBMstrCfgInfo->bCopying = false;
        pstMBMstrCfgInfo->bValid = false;

        /* Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pstMBMstrInfo = &( pstMBMstrCfgInfo->stMasterCfg[ucVerIndex]);
            pstMBMstrInfo->uiCRC = 0U;
            pstMBMstrInfo->uiFileVern = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurModbusMstrInfo
*
* 功能描述: 获取当前使用的Modbus主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置信息
*
* 输出参数: 无
*
* 返 回 值: 当前使用的配置参数
*
* 注意事项: 无
***********************************************************************************************************************
*/

ModbusMasterInfo_t const* GetCurModbusMstrInfo( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo )
{
    ModbusMasterInfo_t const *pstMBMstrInfo = NULL;
    uint8_t ucCfgIndex = 0U;

    if( pstMBMstrCfgInfo != NULL )
    {
        if((!pstMBMstrCfgInfo->bCopying) &&
           (pstMBMstrCfgInfo->bValid))
        {
            ucCfgIndex = pstMBMstrCfgInfo->ucIndex;
            if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
            {
                pstMBMstrInfo = &( pstMBMstrCfgInfo->stMasterCfg[ucCfgIndex]);
            }
        }
    }

    return pstMBMstrInfo;
}

/*
***********************************************************************************************************************
* 函数名称: GetLocalModbusMstrCfg
*
* 功能描述: 获取本CM使用的Modbus主站配置
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置信息；
*          ucCMSlotID：本CM模块的槽号。
*
* 输出参数: 无
*
* 返 回 值: 本CM使用的Modbus主站配置
*
* 注意事项: 无
***********************************************************************************************************************
*/

ModbusMasterConfig_t const* GetLocalModbusMstrCfg( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo, uint8_t ucCMSlotID )
{
    /*DEBUG_RTS( "GetLocalModbusMstrCfg: ucCMSlotID: %d.\n", ucCMSlotID );*/
    ModbusMasterConfig_t const *pstMBMstrCfg = NULL;
    ModbusMasterInfo_t const *pstMBMstrInfo = NULL;
    uint8_t ucCMIndex = 0U;

    if( pstMBMstrCfgInfo != NULL )
    {
        pstMBMstrInfo = GetCurModbusMstrInfo( pstMBMstrCfgInfo );
        if( pstMBMstrInfo != NULL )
        {
            for( ucCMIndex = 0U; ucCMIndex < (uint8_t)(MAX_CM_NUM); ucCMIndex++ )
            {
                /*DEBUG_RTS( "GetLocalModbusMstrCfg: CMIndex: %d, Active: %d, CMSlotID: %d.\n", ucCMIndex, \
                           pstMBMstrInfo->stModbusMasterConfig[ucCMIndex].emActive,\
                           pstMBMstrInfo->stModbusMasterConfig[ucCMIndex].usCMSlotID );*/

                if( ACTIVE == pstMBMstrInfo->stModbusMasterConfig[ucCMIndex].emActive )
                {
                    if((uint16_t)ucCMSlotID == pstMBMstrInfo->stModbusMasterConfig[ucCMIndex].usCMSlotID )
                    {
                        pstMBMstrCfg = &( pstMBMstrInfo->stModbusMasterConfig[ucCMIndex]);
                        /*DEBUG_RTS( "GetLocalModbusMstrCfg OK: CMIndex: %d.\n", ucCMIndex );*/
                        break;
                    }
                }
            }
        }
    }

    return pstMBMstrCfg;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusMstrInfo
*
* 功能描述: 实时数据版本变化，且Modbus主站配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置参数；
*          puiOffset：指向拷贝偏移值；
*          ucCMSlotID：本CM模块的槽位号。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换 - 配置参数拷贝完成且有效时，返回true
*
* 注意事项:
* 1. 实时数据版本变化时，更新本地保存的实时数据版本；
* 2. 为了保证Modbus主站配置与实时数据的一致性：拷贝完成后，无论配置参数是否有效，均需进行切换操作。
***********************************************************************************************************************
*/

bool_t HandleModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo, puint32_t puiOffset, uint8_t ucCMSlotID )
{
    bool_t bSwitch = false;
    bool_t bComplete = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;

    if((pstMBMstrCfgInfo != NULL) &&
       (puiOffset != NULL))
    {
        if( *puiOffset > 0U )
        {
            /* Continue Copy */
            if( !GetModbusMstrInfo( pstMBMstrCfgInfo, puiOffset, &bComplete ))
            {
                DEBUG_RTS( "GetMBMstrInfo Failed\n" );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_GET_MBMSTRINFO));
                ClearCurModbusMstrCfg( pstMBMstrCfgInfo );
            }
            else
            {
                //DEBUG_RTS( "GetModbusMstrInfo Successfully!\n" );
            }
        }
        else
        {
            /* First Try To Copy */
            /* RTData Version */
            /*if( HandleRTDataVer( &( pstMBMstrCfgInfo->stRTDataVer)))*/
            {
                /* Get Configuration Version */
                if( SysGetModbusMasterInfoVersion( &uiCRC, &uiFileVer ))
                {
                    /* Compare Version */
                    if((uiCRC != pstMBMstrCfgInfo->uiCfgCRC) ||
                       (uiFileVer != pstMBMstrCfgInfo->uiCfgVer))
                    {
                        DEBUG_RTS( "Start Copy MBMstrInfo\n" );

                        /* Start Copy */
                        pstMBMstrCfgInfo->bCopying = true;

                        /* Switch */
                        SwitchModbusMstrInfo( pstMBMstrCfgInfo );

                        /* Clear Current Configuration */
                        ClearCurModbusMstrCfg( pstMBMstrCfgInfo );

                        /* Update Version Info. */
                        pstMBMstrCfgInfo->uiCfgCRC = uiCRC;
                        pstMBMstrCfgInfo->uiCfgVer = uiFileVer;

                        /* Copy Configuration Info. */
                        if( !GetModbusMstrInfo( pstMBMstrCfgInfo, puiOffset, &bComplete ))
                        {
                            DEBUG_RTS( "GetMBMstrInfo Failed\n" );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_GET_MBMSTRINFO));
                            ClearCurModbusMstrCfg( pstMBMstrCfgInfo );
                        }
                        else
                        {
                            //DEBUG_RTS( "GetModbusMstrInfo Successfully!\n" );
                        }
                    }
                }
            }
        }

        if( bComplete )
        {
            if( !CheckCurModbusMstrInfo( pstMBMstrCfgInfo, ucCMSlotID ))
            {
                /* Invalid */
                pstMBMstrCfgInfo->bValid = false;
                ClearCurModbusMstrCfg( pstMBMstrCfgInfo );
                DEBUG_RTS( "MBMstr Cfg Invalid\n" );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_MBMSTR));
            }
            else
            {
                pstMBMstrCfgInfo->bValid = true;
                bSwitch = true;
            }

            pstMBMstrCfgInfo->bCopying = false;
            *puiOffset = 0U;
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: GetTaskIndex
*
* 功能描述: 根据任务ID获取任务索引值
*
* 输入参数: ucTaskID：任务ID；
*          pstTaskInfo：指向任务配置信息数组。
*
* 输出参数: 无
*
* 返 回 值: 任务索引值, -1代表无相应的任务
*
* 注意事项: 无
***********************************************************************************************************************
*/

int8_t GetTaskIndex( uint8_t ucTaskID, TaskInfo_t const pstTaskInfo[])
{
    int8_t icTaskIndex = -1;
    uint8_t ucIndex = 0U;
    TaskConfigInfo_t const *pstTaskCfg = NULL;

    if( pstTaskInfo != NULL )
    {
        for( ucIndex = 0U; ucIndex < MAX_TASK_NUM; ucIndex++ )
        {
            pstTaskCfg = GetCurTaskCfg( &pstTaskInfo[ucIndex]);
            if( pstTaskCfg != NULL )
            {
                if( pstTaskCfg->uiTaskID == (uint32_t)ucTaskID )
                {
                    icTaskIndex = (int8_t)ucIndex;
                    break;
                }
            }
        }
    }

    return icTaskIndex;
}

/*
***********************************************************************************************************************
* 函数名称: GetDataAreaLen
*
* 功能描述: 获取某数据区的长度
*
* 输入参数: emDataArea：数据区ID
*
* 输出参数: 无
*
* 返 回 值: 数据区的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint32_t GetDataAreaLen( SysRtDataAreaType_t emDataArea )
{
    uint32_t uiSectionSize = 0U;
    if( emDataArea < MAX_RTDATA_AREA_NUM )
    {
        if( RTDATA_G_AREA == emDataArea )
        {
            uiSectionSize = (uint32_t)(MAX_RTDATA_G_AREA_LEN);
        }
        else if( RTDATA_I_AREA == emDataArea )
        {
            uiSectionSize = (uint32_t)(MAX_RTDATA_I_AREA_LEN);
        }
        else
        {
            uiSectionSize = (uint32_t)(MAX_RTDATA_Q_AREA_LEN);
        }
    }

    return uiSectionSize;
}

/*
***********************************************************************************************************************
* 函数名称: CheckMBSlaveVar
*
* 功能描述: 检查某一Modbus Slave块内的变量是否有效
*
* 输入参数: pstModbusVar：待检查的Modbus 变量；
*          ucModbusRgn：Modbus 变量所在的区。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t CheckMBSlaveVar( ModbusVar_t const *pstModbusVar, uint8_t ucModbusRgn )
{
    bool_t bValid = false;
    uint32_t uiMaxMBIndex = 0U;
    uint32_t uiSectionLen = 0U;
    uint32_t uiOffset = 0U;

    if((pstModbusVar != NULL) &&
       (ucModbusRgn <= (uint8_t)(MR_HOLD_REGS)))
    {
        if((pstModbusVar->ucRefID < REF_NUM) &&
           (pstModbusVar->usVarSize <= (uint16_t)(MAX_VAR_SIZE)))
        {
            /* Variable Size: According to its modbus area */
            if((MR_COIL == ucModbusRgn) || (MR_DISCRETE_INPUTS == ucModbusRgn))
            {
                if((0U == pstModbusVar->usVarSize) ||
                   (1U == pstModbusVar->usVarSize))
                {
                    bValid = true;
                    uiMaxMBIndex = (uint32_t)(MAX_MODBUS_RGN_VAR_NUM) - 1U;
                }
            }
            else
            {
                if((2U == pstModbusVar->usVarSize) ||
                   (4U == pstModbusVar->usVarSize) ||
                   (8U == pstModbusVar->usVarSize))
                {
                    bValid = true;
                    uiMaxMBIndex = (uint32_t)(MAX_MODBUS_RGN_VAR_NUM) - (uint32_t)(pstModbusVar->usVarSize / 2U);
                }
            }

            if( bValid )
            {
                bValid = false;

                /* Modbus Index */
                if( pstModbusVar->uiModbusIndex <= uiMaxMBIndex )
                {
                    bValid = true;
                }
            }

            if( bValid )
            {
                bValid = false;

                /* Section Size */
                uiSectionLen = GetDataAreaLen((SysRtDataAreaType_t)pstModbusVar->ucRefID );
                if( uiSectionLen > 0U )
                {
                    /* Offset */
                    if( 0U == pstModbusVar->usVarSize )
                    {
                        /* Offset: bit -> byte */
                        uiOffset = pstModbusVar->uiOffset / 8;
                    }
                    else
                    {
                        /* Offset: byte */
                        uiOffset = pstModbusVar->uiOffset;
                    }

                    if(( uiOffset + (uint32_t)pstModbusVar->usVarSize ) <= uiSectionLen )
                    {
                        bValid = true;
                    }
                }
                else
                {
                    DEBUG_RTS( "GetDataAreaLen Err: RefID %d\n", pstModbusVar->ucRefID );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_GET_DATA_AREA_LEN));
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckMBMstrVar
*
* 功能描述: 检查某一Modbus变量是否有效
*
* 输入参数: pstModbusVar：待检查的Modbus 变量；
*          ucModbusRgn：Modbus 变量所在的区；
*          uiStartAddr：Modbus主站块变量的起始地址。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t CheckMBMstrVar( ModbusVar_t const *pstModbusVar, uint8_t ucModbusRgn, uint32_t uiStartAddr )
{
    bool_t bValid = false;
    uint32_t uiMaxMBIndex = 0U;
    uint32_t uiSectionLen = 0U;
    uint32_t uiOffset = 0U;

    if((pstModbusVar != NULL) &&
       (ucModbusRgn <= (uint8_t)(MR_HOLD_REGS)) &&
       (uiStartAddr <= ((uint32_t)(MAX_MODBUS_RGN_VAR_NUM) - 1U)))
    {
        if((pstModbusVar->ucRefID < REF_NUM) &&
           (pstModbusVar->usVarSize <= (uint16_t)(MAX_VAR_SIZE)))
        {
            /* Variable Size: According to its modbus area */
            if((MR_COIL == ucModbusRgn) || (MR_DISCRETE_INPUTS == ucModbusRgn))
            {
                if((0U == pstModbusVar->usVarSize) ||
                   (1U == pstModbusVar->usVarSize))
                {
                    bValid = true;
                    if( uiStartAddr + (uint32_t)(MAX_MODBUS_VAR_NUM) <= (uint32_t)(MAX_MODBUS_RGN_VAR_NUM))
                    {
                        uiMaxMBIndex = uiStartAddr + (uint32_t)(MAX_MODBUS_VAR_NUM) - 1U;
                    }
                    else
                    {
                        uiMaxMBIndex = (uint32_t)(MAX_MODBUS_RGN_VAR_NUM) - 1U;
                    }
                }
            }
            else
            {
                if((2U == pstModbusVar->usVarSize) ||
                   (4U == pstModbusVar->usVarSize) ||
                   (8U == pstModbusVar->usVarSize))
                {
                    bValid = true;
                    if( uiStartAddr + (uint32_t)(MAX_MODBUS_VAR_NUM) <= (uint32_t)(MAX_MODBUS_RGN_VAR_NUM))
                    {
                        uiMaxMBIndex = uiStartAddr + (uint32_t)(MAX_MODBUS_VAR_NUM) - (uint32_t)(pstModbusVar->usVarSize/2U);
                    }
                    else
                    {
                        uiMaxMBIndex = (uint32_t)(MAX_MODBUS_RGN_VAR_NUM) - (uint32_t)(pstModbusVar->usVarSize/2U);
                    }
                }
            }

            if( bValid )
            {
                bValid = false;

                /* Modbus Index */
                if( pstModbusVar->uiModbusIndex <= uiMaxMBIndex )
                {
                    bValid = true;
                }
            }

            if( bValid )
            {
                bValid = false;

                /* Section Size */
                uiSectionLen = GetDataAreaLen((SysRtDataAreaType_t)pstModbusVar->ucRefID );
                if( uiSectionLen > 0U )
                {
                    /* Offset */
                    if( 0U == pstModbusVar->usVarSize )
                    {
                        /* Offset: bit -> byte */
                        uiOffset = pstModbusVar->uiOffset / 8;
                    }
                    else
                    {
                        /* Offset: byte */
                        uiOffset = pstModbusVar->uiOffset;
                    }

                    if((uiOffset + (uint32_t)pstModbusVar->usVarSize) <= uiSectionLen )
                    {
                        bValid = true;
                    }
                }
                else
                {
                    DEBUG_RTS( "GetDataAreaLen Err: RefID %d\n", pstModbusVar->ucRefID );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_GET_DATA_AREA_LEN));
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: InitTaskInfo
*
* 功能描述: 初始化本地保存的任务信息
*
* 输入参数: pstTaskInfo：指向本地保存的任务信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitTaskInfo( TaskInfo_t *pstTaskInfo )
{
    uint8_t ucVerIndex = 0U;
    TaskConfigInfo_t *pstTaskCfg = NULL;

    if( pstTaskInfo != NULL )
    {
        memset( pstTaskInfo, 0, sizeof(TaskInfo_t));

        /* Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pstTaskCfg = &( pstTaskInfo->stTaskCfgInfo[ucVerIndex]);
            pstTaskCfg->emActiveFlag = NOT_ACTIVE;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleTaskInfo
*
* 功能描述: 实时数据版本变化，且任务配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: pstTaskInfo：指向本地保存的任务信息
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleTaskInfo( Task_ID_t emTask, TaskInfo_t *pstTaskInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    TaskConfigInfo_t stTaskCfgInfo;

    if((emTask < MAX_TASK_NUM ) &&
       (pstTaskInfo != NULL))
    {
        /* RTData Version */
        if( HandleRTDataVer( &( pstTaskInfo->stRTDataVer )))
        {
            /* Get Control Station Configuration Version */
            if( SysGetCtrlStationConfigVersion( &uiCRC, &uiFileVer ))
            {
                /* Compare Version */
                if((uiCRC != pstTaskInfo->uiCfgCRC) ||
                   (uiFileVer != pstTaskInfo->uiCfgVer))
                {
                    /* Copy To Local */
                    if( SysGetTaskConfigInfo( emTask, &stTaskCfgInfo ))
                    {
                        /* Check Valid */
                        if( CheckTaskCfgInfo( &stTaskCfgInfo ))
                        {
                            /* Switch */
                            if( SwitchTaskCfgInfo( pstTaskInfo, &stTaskCfgInfo ))
                            {
                                bSwitch = true;
                                pstTaskInfo->uiCfgCRC = uiCRC;
                                pstTaskInfo->uiCfgVer = uiFileVer;
                            }
                        }
                        else
                        {
                            pstTaskInfo->uiCfgCRC = uiCRC;
                            pstTaskInfo->uiCfgVer = uiFileVer;
                            DEBUG_RTS( "Task %d Cfg Invalid\n", emTask );
                        }
                    }
                }
            }/* end of SysGetxxxVersion */
        }/* end of HandleRTDataVer */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: GetSectionOffset
*
* 功能描述: 获取区偏移地址
*
* 输入参数: pstTaskInfo：任务信息；
*          ucRefID：区ID；
*
* 输出参数: puiOffset：用于返回区偏移地址。
*
* 返 回 值: 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t GetSectionOffset( TaskInfo_t const *pstTaskInfo, uint8_t ucRefID, uint32_t *puiOffset )
{
    bool_t bResult = false;
    TaskConfigInfo_t const *pstTaskCfg = NULL;

    if((pstTaskInfo != NULL) &&
       (puiOffset != NULL) &&
       (ucRefID < REF_NUM))
    {
        pstTaskCfg = GetCurTaskCfg( pstTaskInfo );
        if( pstTaskCfg != NULL )
        {
            bResult = true;

            if( REF_G == ucRefID )
            {
                *puiOffset = pstTaskCfg->uiGOffset;
            }
            else if( REF_I == ucRefID )
            {
                *puiOffset = pstTaskCfg->uiIOffset;
            }
            else
            {
                *puiOffset = pstTaskCfg->uiOOffset;
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: GetSectionUsedSize
*
* 功能描述: 获取区的已使用大小
*
* 输入参数: pstTaskInfo：任务信息；
*          ucRefID：区ID；
*
* 输出参数: puiUsedSize：用于返回区的已使用大小。
*
* 返 回 值: 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t GetSectionUsedSize( TaskInfo_t const *pstTaskInfo, uint8_t ucRefID, uint32_t *puiUsedSize )
{
    bool_t bResult = false;
    TaskConfigInfo_t const *pstTaskCfg = NULL;

    if((pstTaskInfo != NULL) &&
       (puiUsedSize != NULL) &&
       (ucRefID < REF_NUM))
    {
        pstTaskCfg = GetCurTaskCfg( pstTaskInfo );
        if( pstTaskCfg != NULL )
        {
            bResult = true;

            if( REF_G == ucRefID )
            {
                *puiUsedSize = pstTaskCfg->uiGUsedSize;
            }
            else if( REF_I == ucRefID )
            {
                *puiUsedSize = pstTaskCfg->uiIUsedSize;
            }
            else
            {
                *puiUsedSize = pstTaskCfg->uiOUsedSize;
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SelectRTData
*
* 功能描述: 选取采用哪一系PM的实时数据
*
* 输入参数: pemLastPMID：输入上一次选取结果；
*
* 输出参数: pemLastPMID：输出更新后的结果；
*          pbCMBus：输出CM Bus是否正常。
*
* 返 回 值: 是否选取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SelectRTData( PMController_t *pemLastPMID, bool_t *pbCMBus )
{
    bool_t bResult = false;
    bool_t bPMA = false;       /* PMA内部通讯正常且有实时数据 */
    bool_t bPMB = false;       /* PMB内部通讯正常且有实时数据 */
    bool_t bPMC = false;       /* PMC内部通讯正常且有实时数据 */
    bool_t bPMABSame = false;  /* PMA 与 PMB Q区数据是否一致 */
    bool_t bPMACSame = false;  /* PMA 与 PMC Q区数据是否一致 */
    bool_t bPMBCSame = false;  /* PMB 与 PMC Q区数据是否一致 */
    PMController_t emOldSelPMID = INVALID_PM_ID;  /* 上一次选取结果 */

    /* Only For Print */
    PMController_t emPM1 = INVALID_PM_ID;
    PMController_t emPM2 = INVALID_PM_ID;

    if((pemLastPMID != NULL) &&
       (pbCMBus != NULL))
    {
        bResult = true;
        emOldSelPMID = *pemLastPMID;  /* Store last selected PMID */

        /* Internal Communication Status */
        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
        {
            if( SysGetRTDataAddr( PMA, RTDATA_Q_AREA ) != NULL )
            {
                bPMA = true;
            }
        }

        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
        {
            if( SysGetRTDataAddr( PMB, RTDATA_Q_AREA ) != NULL )
            {
                bPMB = true;
            }
        }

        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
        {
            if( SysGetRTDataAddr( PMC, RTDATA_Q_AREA ) != NULL )
            {
                bPMC = true;
            }
        }

        /* Select */
        if( bPMA || bPMB || bPMC )
        {
            /* CM Bus OK */
            *pbCMBus = true;

            /* Compare */
            if( bPMA && bPMB && bPMC )
            {
                /* All Internal Communication Normal: Compare And Select */
                /* Compare PMA & PMB */
                bPMABSame = CompareRTData( PMA, PMB, RTDATA_Q_AREA );

                /* Compare PMA & PMC */
                bPMACSame = CompareRTData( PMA, PMC, RTDATA_Q_AREA );

                if( bPMABSame && bPMACSame )
                {
                    DEBUG_RTS( "SelectRTData: All Same, Use PMA\n" );
                    *pemLastPMID = PMA;  /* All Same: Use PMA */
                }
                else
                {
                    /* Not All Same */
                    /* Compare PMB & PMC */
                    bPMBCSame = CompareRTData( PMB, PMC, RTDATA_Q_AREA );
                    if( bPMABSame || bPMACSame || bPMBCSame )
                    {
                        /* Select Smaller PM */
                        if( bPMBCSame )
                        {
                            DEBUG_RTS( "SelectRTData: PMB PMC Same, Use PMB\n" );
                            *pemLastPMID = PMB;
                        }
                        else
                        {
                            DEBUG_RTS( "SelectRTData: PMA PMB/PMC Same, Use PMA\n" );
                            *pemLastPMID = PMA;
                        }
                    }
                    else
                    {
                        /* All Different */
                        DEBUG_RTS( "SelectRTData: All Different, Use PMA\n" );
                        *pemLastPMID = PMA;
                    }
                }
            }/* end of if - All Normal */
            else if((bPMA && bPMB) ||
                    (bPMA && bPMC) ||
                    (bPMB && bPMC))
            {
                /* Two Internal Communication Normal: Select Smaller */
                if( bPMB && bPMC )
                {
                    *pemLastPMID = PMB;

                    /* Only For Print */
                    emPM1 = PMB;
                    emPM2 = PMC;
                }
                else
                {
                    *pemLastPMID = PMA;

                    /* Only For Print */
                    emPM1 = PMA;
                    if( bPMB )
                    {
                        emPM2 = PMB;
                    }
                    else
                    {
                        emPM1 = PMC;
                    }
                }

                DEBUG_RTS( "SelectRTData: Two Normal-PM%d, PM%d, Select Smaller-PM%d\n", \
                           emPM1, emPM2, *pemLastPMID );
            }
            else
            {
                /* Only One Normal: Use */
                if( bPMA )
                {
                    *pemLastPMID = PMA;
                }
                else if( bPMB )
                {
                    *pemLastPMID = PMB;
                }
                else
                {
                    *pemLastPMID = PMC;
                }

                DEBUG_RTS( "SelectRTData: Only One Normal: Use-PM%d\n", *pemLastPMID );
            }

            if( emOldSelPMID != *pemLastPMID )
            {
                DEBUG_RTS( "SelectRTData: Selected PM changed from %d to %d\n", emOldSelPMID, *pemLastPMID );
                LogWrite( (uint32_t)(LOGID_EXCOMM_SELRTDATA_CHANGE_PM));
            }
        }
        else
        {
            /* CM Bus Error: Use Last Selected PM's RTData */
            *pbCMBus = false;
            DEBUG_RTS( "SelectRTData: No Valid(CM_BUS/RTData), Use Last Selected PM: %d\n", *pemLastPMID );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_CM_BUS));
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurTaskCfg
*
* 功能描述: 获取当前使用的任务配置参数
*
* 输入参数: pstTaskInfo：指向任务信息
*
* 输出参数: 无
*
* 返 回 值: 指向当前使用的配置参数，NULL代表当前无有效配置参数
*
* 注意事项: 无
***********************************************************************************************************************
*/

TaskConfigInfo_t const * GetCurTaskCfg( TaskInfo_t const *pstTaskInfo )
{
    TaskConfigInfo_t const *pstTaskCfg = NULL;
    uint8_t ucCfgIndex = 0U;

    if( pstTaskInfo != NULL )
    {
        ucCfgIndex = pstTaskInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstTaskCfg = &( pstTaskInfo->stTaskCfgInfo[ucCfgIndex]);
            if( pstTaskCfg->emActiveFlag != ACTIVE )
            {
                pstTaskCfg = NULL;
            }
        }
    }

    return pstTaskCfg;
}

/*
***********************************************************************************************************************
* 函数名称: InitTimeSyncCfgInfo
*
* 功能描述: 初始化本地保存的时间同步配置信息
*
* 输入参数: pstTimeSyncInfo：指向本地保存的时间同步配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitTimeSyncCfgInfo( TimeSyncCfgInfo_t *pstTimeSyncInfo )
{
    uint8_t ucVerIndex = 0U;
    NTPTCPConfigInfo_t *pTimeCfg = NULL;

    if( pstTimeSyncInfo != NULL )
    {
        memset( pstTimeSyncInfo, 0, sizeof(TimeSyncCfgInfo_t));

        /* Network Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pTimeCfg = &( pstTimeSyncInfo->stTimeSyncConfig[ucVerIndex]);
            pTimeCfg->emActive = NOT_ACTIVE;
            pTimeCfg->bUpdate = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleTimeSyncConfig
*
* 功能描述: 检查时钟同步配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: pstTimeSyncInfo：指向时钟同步配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleTimeSyncConfig( TimeSyncCfgInfo_t *pstTimeSyncInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    NTPTCPConfigInfo_t stTSyncCfg;

    if( pstTimeSyncInfo != NULL )
    {
        /* Get CM Configuration Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstTimeSyncInfo->uiCMCfgCRC) ||
               (uiFileVer != pstTimeSyncInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysGetNTPTCPConfigInfo( &stTSyncCfg ))
                {
                    /* Check Active */
                    if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                       ( ACTIVE == stTSyncCfg.emActive ))
                    {
                        /* CM always exist: Switch when stTSyncCfg.bUpdate is true */
                        /* Online download and then change CM: stTSyncCfg.bUpdate true/false, Local ver is 0 */
                        if( stTSyncCfg.bUpdate ||
                            ((0U == pstTimeSyncInfo->uiCMCfgCRC ) && (0U == pstTimeSyncInfo->uiCMCfgVer)))
                        {
                            /* Check Valid */
                            if( CheckTimeSyncCfg( &stTSyncCfg ))
                            {
                                /* OPC does not have Update Flag: Switch directly */
                                if( SwitchTimeSyncCfg( pstTimeSyncInfo, &stTSyncCfg ))
                                {
                                    bSwitch = true;
                                    pstTimeSyncInfo->uiCMCfgCRC = uiCRC;
                                    pstTimeSyncInfo->uiCMCfgVer = uiFileVer;
                                }
                            }
                            else
                            {
                                pstTimeSyncInfo->uiCMCfgCRC = uiCRC;
                                pstTimeSyncInfo->uiCMCfgVer = uiFileVer;
                                DEBUG_RTS( "TimeSync Cfg Invalid\n" );
                                LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_TIMESYNC));
                            }
                        }
                    }
                }
            }
        }/* end of SysGetCMConfigVersion */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurTSyncCfg
*
* 功能描述: 获取当前时钟同步信息
*
* 输入参数: pstTimeSyncInfo：本地保存的时钟同步信息
*
* 输出参数: 无
*
* 返 回 值: 当前配置信息，NULL代表无有效配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

NTPTCPConfigInfo_t const* GetCurTSyncCfg( TimeSyncCfgInfo_t const *pstTimeSyncInfo )
{
    uint8_t ucCfgIndex = 0U;
    NTPTCPConfigInfo_t const *pstTSyncCfg = NULL;

    if( pstTimeSyncInfo != NULL )
    {
        ucCfgIndex = pstTimeSyncInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstTSyncCfg = &( pstTimeSyncInfo->stTimeSyncConfig[ucCfgIndex]);
        }
    }

    return pstTSyncCfg;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GetModbusMstrInfo
*
* 功能描述: 将Modbus主站配置信息拷贝到本地
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置参数；
*          puiOffset：指向拷贝偏移值。
*
* 输出参数: pbComplete：是否将所有有效Modbus主站配置拷贝到本地
*
* 返 回 值: 拷贝操作是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo, puint32_t puiOffset, bool_t *pbComplete )
{
    bool_t bResult = false;
    puint8_t pucBuff = NULL;  /* Buffer Pointer */
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    uint32_t uiDstLen = 0U;   /* Destination Copy Length */
    uint32_t uiCopyLen = 0U;  /* Real Copy Length */
    uint32_t uiContLen = 0U;  /* Content Length */
    ModbusMasterInfo_t *pstMBMstrInfo = NULL;

    if((pstMBMstrCfgInfo != NULL) &&
       (puiOffset != NULL) &&
       (pbComplete != NULL))
    {
        *pbComplete = false;

        /* Get First Address */
        ucCfgIndex = pstMBMstrCfgInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBMstrInfo = &( pstMBMstrCfgInfo->stMasterCfg[ucCfgIndex]);
            pucBuff = (puint8_t)pstMBMstrInfo;
            uiContLen = sizeof(ModbusMasterInfo_t) - *puiOffset;

            /* Calculate Cycle Copy Length */
            if( uiContLen > (uint32_t)(MAX_CYCLE_COPY_LEN))
            {
                uiDstLen = (uint32_t)(MAX_CYCLE_COPY_LEN);
            }
            else
            {
                uiDstLen = uiContLen;
            }

            /*DEBUG_RTS( "GetModbusMstrInfo: Total Size: %d, Offset: %d, Remain: %d, Dst Copy Len: %d.\n", \
                       sizeof(ModbusMasterInfo_t), *puiOffset, uiContLen, uiDstLen );*/

            /* Copy Content */
            uiCopyLen = SysGetModbusMasterInfo( &pucBuff[*puiOffset], *puiOffset, uiDstLen );
            if( uiCopyLen > 0U )
            {
                /* Copy Successfully */
                bResult = true;
                *puiOffset += uiCopyLen;
                if( uiContLen == uiCopyLen )
                {
                    *pbComplete = true;
                }
            }
            else
            {
                bResult = false;
                *puiOffset = 0U;
            }
        }
        else
        {
            bResult = false;
            *puiOffset = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchModbusMstrInfo
*
* 功能描述: 更新索引值
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SwitchModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo )
{
    uint8_t ucCfgIndex = 0U;
    if( pstMBMstrCfgInfo != NULL )
    {
        ucCfgIndex = pstMBMstrCfgInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = (ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);

            /* Update Index */
            pstMBMstrCfgInfo->ucIndex = ucCfgIndex;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearCurModbusMstrCfg
*
* 功能描述: 清除当前Modbus主站配置
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearCurModbusMstrCfg( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo )
{
    ModbusMasterInfo_t *pstMBMstrInfo = NULL;
    ModbusMasterConfig_t *pstMBMstrCfg = NULL;
    uint8_t ucCfgIndex = 0U;
    uint8_t ucIndex = 0U;
    uint8_t ucPortIndex = 0U;

    if( pstMBMstrCfgInfo != NULL )
    {
        ucCfgIndex = pstMBMstrCfgInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBMstrInfo = &( pstMBMstrCfgInfo->stMasterCfg[ucCfgIndex]);

            /* Modbus Master Config */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
            {
                pstMBMstrCfg = &( pstMBMstrInfo->stModbusMasterConfig[ucIndex]);
                pstMBMstrCfg->emActive = NOT_ACTIVE;

                /* Port */
                for( ucPortIndex = 0U; ucPortIndex < (uint8_t)(MAX_PORT_NUM); ucPortIndex++ )
                {
                    pstMBMstrCfg->stModbusMasterPort[ucPortIndex].emActive = NOT_ACTIVE;
                }
            }

            /* Modbus Master Block */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_BLOCK_NUM); ucIndex++ )
            {
                pstMBMstrInfo->stModbusMasterBlock[ucIndex].emActive = NOT_ACTIVE;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCurModbusMstrInfo
*
* 功能描述: 检查Modbus主站配置信息是否有效
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站信息；
*          ucCMSlotID：本CM模块的槽位号。
*
* 输出参数: 无
*
* 返 回 值: 配置信息是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckCurModbusMstrInfo( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo, uint8_t ucCMSlotID )
{
    bool_t bValid = false;
    uint8_t ucCfgIndex = 0U;
    uint8_t ucIndex = 0U;
    uint8_t ucPortIndex = 0U;
    ModbusMasterInfo_t const *pstMBMstrInfo = NULL;
    ModbusMasterConfig_t const *pstMBMstrCfg = NULL;
    ModbusMasterPort_t const *pstPortCfg = NULL;
    ModbusMasterBlock_t const *pstMBMstrBlk = NULL;

#ifdef DEBUG_EXTERCOMM
    /* Only for print */
    uint32_t uiIndex = 0U;
#endif

    if( pstMBMstrCfgInfo != NULL )
    {
        ucCfgIndex = pstMBMstrCfgInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            bValid = true;
            pstMBMstrInfo = &( pstMBMstrCfgInfo->stMasterCfg[ucCfgIndex]);

            /* Modbus Master Config: CM */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
            {
                pstMBMstrCfg = &( pstMBMstrInfo->stModbusMasterConfig[ucIndex]);
                if( ACTIVE == pstMBMstrCfg->emActive )
                {
                    if((uint16_t)ucCMSlotID == pstMBMstrCfg->usCMSlotID )
                    {
                        /* Port Configuration */
                        for( ucPortIndex = 0U; ucPortIndex < (uint8_t)(MAX_PORT_NUM); ucPortIndex++ )
                        {
                            pstPortCfg = &( pstMBMstrCfg->stModbusMasterPort[ucPortIndex]);
                            pstMBMstrBlk = &( pstMBMstrInfo->stModbusMasterBlock[0]);
                            if( !CheckModbusMstrPortCfg( pstPortCfg, (CommPort_t)ucPortIndex, pstMBMstrBlk ))
                            {
                                DEBUG_RTS( "MMMstr Port-%d Invalid!\n", ucPortIndex );
                                LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_MBMSTR_PORT));
                                bValid = false;
                                break;
                            }
#ifdef DEBUG_EXTERCOMM
                            else
                            {
                                /* Only for test */
                                if( 0U == ucPortIndex )
                                {
                                    if( ACTIVE == pstPortCfg->emActive )
                                    {
                                        DEBUG_RTS( "Port: %d, BlkCnts: %d.\n", ucPortIndex, pstPortCfg->uiBlockNum );

                                        for( uiIndex = 0U; uiIndex < pstPortCfg->uiBlockNum; uiIndex++ )
                                        {
                                            if( ACTIVE == pstMBMstrBlk[uiIndex].emActive )
                                            {
                                                DEBUG_RTS( "Block %d: SlaveID: %d, Rgn:%d, BeginAddr:%d, TargetSize: %d, VarNum: %d.\n", \
                                                           uiIndex, pstMBMstrBlk[uiIndex].stModbusMasterBlockHead.uiTargetStationID, pstMBMstrBlk[uiIndex].stModbusMasterBlockHead.ucModbusRgn, \
                                                           pstMBMstrBlk[uiIndex].stModbusMasterBlockHead.uiTargetBeginAddr, pstMBMstrBlk[uiIndex].stModbusMasterBlockHead.uiTargetSize, \
                                                           pstMBMstrBlk[uiIndex].stModbusMasterBlockHead.uiModbusVarNum );
                                            }
                                        }
                                    }
                                }
                            }
#endif
                        }

                        break;
                    }
                }
            }/* end of for: CM */
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusMstrPortCfg
*
* 功能描述: 检查某端口的Modbus主站配置信息是否有效
*
* 输入参数: pstPortCfg：指向某端口的Modbus主站配置信息；
*          emPort：端口号；
*          pstMBMstrBlk：Modbus主站块的首地址。
*
* 输出参数: 无
*
* 返 回 值: 配置信息是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusMstrPortCfg( ModbusMasterPort_t const *pstPortCfg, CommPort_t emPort,
                                      ModbusMasterBlock_t const pstMBMstrBlk[])
{
    bool_t bValid = false;
    uint8_t ucBlkID = 0;
    uint32_t uiIndex = 0U;

    if((pstPortCfg != NULL) &&
       ((emPort >= PORT_COM1) && (emPort < PORT_NUM)) &&
       (pstMBMstrBlk != NULL))
    {
        /* Active Flag */
        if((ACTIVE == pstPortCfg->emActive) ||
           (NOT_ACTIVE == pstPortCfg->emActive))
        {
            bValid = true;

            if( ACTIVE == pstPortCfg->emActive )
            {
                /* Net1, Net2: Slave Change */
                if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
                {
                    bValid = CheckNetModbusSlaveChgInfo( &( pstPortCfg->stModbusSlaveChange));
                    if( !bValid )
                    {
                        DEBUG_RTS( "MBMstrPort[%d]: MBSlaveChangeInfo Invalid\n", emPort );
                    }
                }

                /* Block */
                if( bValid )
                {
                    if( pstPortCfg->uiBlockNum <= (uint32_t)(MAX_MODBUS_BLOCK_NUM))
                    {
                        for( uiIndex = 0U; uiIndex < pstPortCfg->uiBlockNum; uiIndex++ )
                        {
                            ucBlkID = pstPortCfg->ucModbusMasterBlockID[uiIndex];
                            if( ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM))
                            {
                                if( !CheckModbusMstrBlockInfo( &pstMBMstrBlk[ucBlkID], emPort ))
                                {
                                    DEBUG_RTS( "MBMstrPort[%d]: Blk %d Invalid\n", emPort, ucBlkID );
                                    bValid = false;
                                    break;
                                }
                            }
                            else
                            {
                                DEBUG_RTS( "MBMstrPort[%d]: BlkID-%d >= %d\n", emPort, ucBlkID, MAX_MODBUS_BLOCK_NUM );
                                bValid = false;
                                break;
                            }
                        }/* end of for */
                    }
                    else
                    {
                        DEBUG_RTS( "MBMstrPortHead: BlkNum-%d Invalid", pstPortCfg->uiBlockNum );
                        bValid = false;
                    }
                }
            }
        }
        else
        {
            DEBUG_RTS( "MBMstrPort[%d]: Active Flag-0x%x Invalid\n", emPort, pstPortCfg->emActive );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckNetModbusSlaveChgInfo
*
* 功能描述: 检查某网口的Modbus从站变化配置信息是否有效
*
* 输入参数: pstChg：指向待检查的配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckNetModbusSlaveChgInfo( ModbusSlaveChange_t const *pstChg )
{
    bool_t bValid = false;
    uint32_t uiIndex = 0U;

    if( pstChg != NULL )
    {
        if((pstChg->uiAddTCPStationNum <= (uint32_t)(MAX_MODBUS_SLAVE_NUM)) &&
           (pstChg->uiDelTCPStationNum <= (uint32_t)(MAX_MODBUS_SLAVE_NUM)))
        {
            bValid = true;

            /* Added Slave */
            for( uiIndex = 0U; uiIndex < pstChg->uiAddTCPStationNum; uiIndex++ )
            {
                if( 0U == pstChg->uiAddStaionIP[uiIndex])
                {
                    DEBUG_RTS( "Added IP Addr Invalid: Index-%d, Number-%d\n", \
                               uiIndex, pstChg->uiAddTCPStationNum );
                    bValid = false;
                    break;
                }
            }

            /* Deleted Slave */
            if( bValid )
            {
                for( uiIndex = 0U; uiIndex < pstChg->uiDelTCPStationNum; uiIndex++ )
                {
                    if( 0U == pstChg->uiDelStaionIP[uiIndex] )
                    {
                        DEBUG_RTS( "Deleted IP Addr Invalid: Index-%d, Number-%d\n", \
                                   uiIndex, pstChg->uiDelTCPStationNum );
                        bValid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            DEBUG_RTS( "Add-%d / Delete-%d TCP Staiton Number Invalid\n", \
                       pstChg->uiAddTCPStationNum, pstChg->uiDelTCPStationNum );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusMstrBlockInfo
*
* 功能描述: 检查某一Modbus主站块的配置信息是否有效
*
* 输入参数: pstBlk：指向待检查的配置信息；
*          emPort：端口号。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusMstrBlockInfo( ModbusMasterBlock_t const *pstBlk, CommPort_t emPort )
{
    bool_t bValid = false;
    uint32_t uiIndex = 0U;
    uint32_t uiVarNum = 0U;
    ModbusVar_t const *pstModbusVar = NULL;
    ModbusMasterBlockHead_t const *pstMBMstrBlkHead = NULL;

    if((pstBlk != NULL) &&
       ((emPort >= PORT_COM1) && (emPort < PORT_NUM)))
    {
        /* Active Flag */
        if( ACTIVE == pstBlk->emActive )
        {
            bValid = true;

            /* Block Header */
            pstMBMstrBlkHead = &( pstBlk->stModbusMasterBlockHead );
            bValid = CheckModbusMstrBlkHead( pstMBMstrBlkHead, emPort );
            if( bValid )
            {
                /* Variable */
                uiVarNum = pstBlk->stModbusMasterBlockHead.uiModbusVarNum;
                for( uiIndex = 0U; uiIndex < uiVarNum; uiIndex++ )
                {
                    pstModbusVar = &( pstBlk->stModbusVar[uiIndex]);
                    if( !CheckMBMstrVar( pstModbusVar, pstMBMstrBlkHead->ucModbusRgn, \
                                         pstMBMstrBlkHead->uiTargetBeginAddr ))
                    {
                        DEBUG_RTS( "MBVar-Index:%d of Port-%d Invalid\n", uiIndex, emPort );
                        bValid = false;
                        break;
                    }
                }
            }
            else
            {
                DEBUG_RTS( "MBMstrBlkHead of Port-%d Invalid\n", emPort );
            }
        }
        else
        {
            DEBUG_RTS( "MBMstrBlk of Port-%d should be ACTIVE\n", emPort );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusMstrBlkHead
*
* 功能描述: 检查某一Modbus主站块的头信息是否有效
*
* 输入参数: pstBlkHead：指向待检查的配置信息；
*          emPort：通信端口。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusMstrBlkHead( ModbusMasterBlockHead_t const *pstBlkHead, CommPort_t emPort )
{
    bool_t bValid = false;

    if((pstBlkHead != NULL) &&
       ((emPort >= PORT_COM1) && (emPort < PORT_NUM)))
    {
        /* Visit Mode, Modbus Region */
        if((((uint8_t)(VM_READ) == pstBlkHead->ucVisitMode) || ((uint8_t)(VM_WRITE) == pstBlkHead->ucVisitMode )) &&
           ((pstBlkHead->ucModbusRgn >= (uint8_t)(MR_COIL)) && (pstBlkHead->ucModbusRgn <= (uint8_t)(MR_HOLD_REGS))))
        {
            bValid = true;
        }
        else
        {
            bValid = false;
            DEBUG_RTS( "MBMstrBlkHead: VisitMode-%d / MBRgn-%d Invalid\n", \
                       pstBlkHead->ucVisitMode, pstBlkHead->ucModbusRgn );
        }

        /* Write Coil / HoldRegs */
        if( bValid )
        {
            if((uint8_t)(VM_WRITE) == pstBlkHead->ucVisitMode )
            {
                if(((uint8_t)(MR_COIL) == pstBlkHead->ucModbusRgn) ||
                   ((uint8_t)(MR_HOLD_REGS) == pstBlkHead->ucModbusRgn))
                {
                    bValid = true;
                }
                else
                {
                    bValid = false;
                    DEBUG_RTS( "Write Invalid ModbusRgn-%d\n", pstBlkHead->ucModbusRgn );
                }
            }
        }

        /* Slave ID: 1-247 */
        if( bValid )
        {
            if((pstBlkHead->uiTargetStationID >= (uint32_t)(MIN_MODBUS_SLAVE_ID)) &&
               (pstBlkHead->uiTargetStationID <= (uint32_t)(MAX_MODBUS_SLAVE_ID)))
            {
                bValid = true;
            }
            else
            {
                bValid = false;
                DEBUG_RTS( "MBMstrBlkHead: SlaveID-%d Invalid\n", pstBlkHead->uiTargetStationID );
            }
        }

        /* Modbus TCP: IP Address */
        if( bValid )
        {
            if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
            {
                if( pstBlkHead->uiTargetIP != 0U )
                {
                    bValid = true;
                }
                else
                {
                    bValid = false;
                    DEBUG_RTS( "MBMstrBlkHead: Target IP Addr-%d Invalid\n", pstBlkHead->uiTargetIP );
                }
            }
        }

        if( bValid )
        {
            /* Begin Address: 0 - 65535 */
            if((pstBlkHead->uiTargetBeginAddr <= (uint32_t)(MAX_MBSLAVE_ALIAS)) &&
               (pstBlkHead->uiTargetSize <= (uint32_t)(MAX_MODBUS_VAR_NUM)) &&
               (pstBlkHead->uiModbusVarNum <= (uint32_t)(MAX_MODBUS_VAR_NUM)))
            {
                bValid = true;
            }
            else
            {
                bValid = false;
                DEBUG_RTS( "MBMstrBlkHead: TargetBeginAddr-%d / TargetSize-%d / MBVariableNum-%d Invalid\n", \
                           pstBlkHead->uiTargetBeginAddr, pstBlkHead->uiTargetSize, pstBlkHead->uiModbusVarNum );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckTaskCfgInfo
*
* 功能描述: 检查某一任务的配置信息是否有效
*
* 输入参数: pstTaskCfgInfo：指向待检查的配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckTaskCfgInfo( TaskConfigInfo_t const *pstTaskCfgInfo )
{
    bool_t bValid = false;

    if( pstTaskCfgInfo != NULL )
    {
        if((ACTIVE == pstTaskCfgInfo->emActiveFlag) ||
           (NOT_ACTIVE == pstTaskCfgInfo->emActiveFlag))
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchTaskCfgInfo
*
* 功能描述: 切换任务配置信息
*
* 输入参数: pstTaskInfo：指向本地保存的任务信息；
*          pstTaskCfgInfo：指向待切换的任务配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchTaskCfgInfo( TaskInfo_t *pstTaskInfo, TaskConfigInfo_t const *pstTaskCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    TaskConfigInfo_t *pstDstTaskCfgInfo = NULL;

    if(( pstTaskInfo != NULL ) &&
       ( pstTaskCfgInfo != NULL ))
    {
        ucCfgIndex = pstTaskInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstTaskCfgInfo = &( pstTaskInfo->stTaskCfgInfo[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstTaskCfgInfo, pstTaskCfgInfo, sizeof(TaskConfigInfo_t));

            /* Update Index */
            pstTaskInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CompareRTQData
*
* 功能描述: 比较两个PM的某个数据区中的数据是否一致
*
* 输入参数: emPM1：PM 1；
*          emPM2：PM 2；
*          emDataArea：数据区ID。
*
* 输出参数: 无
*
* 返 回 值: 是否一致
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CompareRTData( PMController_t emPM1, PMController_t emPM2, SysRtDataAreaType_t emDataArea )
{
    DEBUG_RTS( "CmpRTData: PM1-%d, PM2-%d, Data Area:%d\n", emPM1, emPM2, emDataArea );
    bool_t bSame = false;
    uint32_t uiIndex = 0U;
    uint32_t uiAreaLen = 0U;
    uint8_t const *pucPM1RTData = NULL;
    uint8_t const *pucPM2RTData = NULL;

    if((emPM1 < NUM_OF_PM) &&
       (emPM2 < NUM_OF_PM) &&
       (emDataArea < MAX_RTDATA_AREA_NUM))
    {
        /* PMA, PMB, PMC Real-Time Data Address：G/I/Q Area */
        pucPM1RTData = SysGetRTDataAddr( emPM1, emDataArea );
        pucPM2RTData = SysGetRTDataAddr( emPM2, emDataArea );
        if((pucPM1RTData != NULL) &&
           (pucPM2RTData != NULL))
        {
            uiAreaLen = GetDataAreaLen( emDataArea );
            DEBUG_RTS( "CmpRTData: DataAreaLen %d\n", uiAreaLen );

            /* Byte Compare */
            bSame = true;
            for( uiIndex = 0U; uiIndex < uiAreaLen; uiIndex++ )
            {
                if( pucPM1RTData[uiIndex] != pucPM2RTData[uiIndex])
                {
                    bSame = false;
                    break;
                }
            }
        }
        else
        {
            DEBUG_RTS( "CmpRTData - SysGetRTDataAddr Err\n" );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_GET_RTDATA_ADDR));
        }
    }

    return bSame;
}

/*
***********************************************************************************************************************
* 函数名称: IsCMHasCfg
*
* 功能描述: 检查CM是否有工程
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 是否有工程标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsCMHasCfg( void )
{
    bool_t bCMHasCfg = false;
    ProjVerInfo_t stProjVer;
    memset( &stProjVer, 0, sizeof(ProjVerInfo_t));

    if( SysGetLocalProjVersion( &stProjVer ))
    {
        if((stProjVer.uiMainVer != 0U) ||
           (stProjVer.uiMinorVer != 0U) ||
           (stProjVer.uiProjID != 0U) ||
           (stProjVer.uiProjCRC != 0U))
        {
            bCMHasCfg = true;
        }
    }

    return bCMHasCfg;
}

/*
***********************************************************************************************************************
* 函数名称: CheckTimeSyncCfg
*
* 功能描述: 检查时钟同步配置信息是否有效
*
* 输入参数: pstTSyncCfg：指向待切换的时钟同步配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckTimeSyncCfg( NTPTCPConfigInfo_t const *pstTSyncCfg )
{
    bool_t bValid = false;

    if( pstTSyncCfg != NULL )
    {
        /* Active Flag: ACTIVE/NOT_ACTIVE */
        if((ACTIVE == pstTSyncCfg->emActive) ||
           (NOT_ACTIVE == pstTSyncCfg->emActive))
        {
            bValid = true;
        }

        if( bValid && (ACTIVE == pstTSyncCfg->emActive))
        {
            /* Timing Source */
            if(((pstTSyncCfg->ucTimingSrc >= (uint8_t)(TSRC_NTP)) && (pstTSyncCfg->ucTimingSrc <= (uint8_t)(TSRC_TBLK))) &&
               (((uint8_t)(POLY_POS) == pstTSyncCfg->ucPulsePolarity) || ((uint8_t)(POLY_NEG) == pstTSyncCfg->ucTimingSrc)))
            {
                bValid = true;
            }
            else
            {
                bValid = false;
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchTimeSyncCfg
*
* 功能描述: 切换时钟同步配置信息
*
* 输入参数: pstTimeSyncInfo：指向本地保存的时钟同步信息；
*          pstTSyncCfg：指向待切换的时钟同步配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchTimeSyncCfg( TimeSyncCfgInfo_t *pstTimeSyncInfo, NTPTCPConfigInfo_t const *pstTSyncCfg )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    NTPTCPConfigInfo_t *pstDstTSyncCfg = NULL;

    if((pstTimeSyncInfo != NULL) &&
       (pstTSyncCfg != NULL))
    {
        ucCfgIndex = pstTimeSyncInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = (ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstTSyncCfg = &( pstTimeSyncInfo->stTimeSyncConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstTSyncCfg, pstTSyncCfg, sizeof(NTPTCPConfigInfo_t));

            /* Update Index */
            pstTimeSyncInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

