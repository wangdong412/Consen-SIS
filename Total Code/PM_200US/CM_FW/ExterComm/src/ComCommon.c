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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <error.h>
#include <dirent.h>
#include <byteswap.h>
#include <linux/fs.h>
#include <sys/time.h>
#include <sys/resource.h>
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

/* Time Sync */
static bool_t CheckTimeSyncCfg( NTPTCPConfigInfo_t const *pstTSyncCfg );
static bool_t SwitchTimeSyncCfg( TimeSyncCfgInfo_t *pstTimeSyncInfo, NTPTCPConfigInfo_t const *pstTSyncCfg );

/* System Command */
static int32_t WaitChildFinish( int32_t iChildPID );
static int32_t ExecSingleCmd( int32_t iShmID );
static void ChdExecCmd( int32_t iShmID );

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
    bool_t bPMA = false;  /* PMA内部通讯正常且有实时数据 */
    bool_t bPMB = false;  /* PMB内部通讯正常且有实时数据 */
    bool_t bPMC = false;  /* PMC内部通讯正常且有实时数据 */
    bool_t bReSel = false; /* Re-select flag */
    PMController_t emOldSelPMID = INVALID_PM_ID;  /* 上一次选取结果 */

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

            /* Check if need to re-select */
            if( emOldSelPMID < PMA || emOldSelPMID > PMC )
            {
                bReSel = true;
            }
            else
            {
                if(((PMA == emOldSelPMID) && (!bPMA)) ||
                   ((PMB == emOldSelPMID) && (!bPMB)) ||
                   ((PMC == emOldSelPMID) && (!bPMC)))
                {
                    bReSel = true;
                }
            }

            /* Re-select */
            if( bReSel )
            {
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
* 函数名称: AddSysCmd
*
* 功能描述: 添加命令
*
* 输入参数: pstCmd：命令 ；
*          pstCmdInfo：命令信息结构体。
*
* 输出参数: 无
*
* 返 回 值: 是否添加成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t AddSysCmd( SysCmd_t *pstCmd, SysCmdInfo_t *pstCmdInfo )
{
    bool_t bResult = false;
    uint8_t ucIndex = 0U;

    if((pstCmd != NULL) && (pstCmdInfo != NULL))
    {
        if( pstCmdInfo->ucCmdCnt < (uint8_t)(MAX_CMD_CNT))
        {
            ucIndex = pstCmdInfo->ucCmdCnt;
            memcpy( &pstCmdInfo->stCmd[ucIndex], pstCmd, sizeof(SysCmd_t));

            pstCmdInfo->ucCmdCnt++;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ExecSysCmd
*
* 功能描述: 执行系统命令
*
* 输入参数: pstCmdInfo：命令信息；
*          pucShmCmd：父进程共享内存首地址；
*          iShmID：共享内存ID。
*
* 输出参数: 无
*
* 返 回 值: 结束码
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t ExecSysCmd( SysCmdInfo_t *pstCmdInfo, uint8_t *pucShmCmd, int32_t iShmID )
{
    int32_t iFin = (int32_t)(ESC_FINISH_FAIL);
    uint8_t ucIndex = 0U;
    bool_t bClear = false;
    int32_t iRet = -1;
    int32_t iPID = 0;

    if((pstCmdInfo != NULL) && (pucShmCmd != NULL) && (iShmID != -1))
    {
        if( !pstCmdInfo->bWaitChild )
        {
            /* Check if has command to execute */
            ucIndex = pstCmdInfo->ucCurIndex;
            if((pstCmdInfo->ucCmdCnt > 0U) && (ucIndex < pstCmdInfo->ucCmdCnt))
            {
                /* Execute a single command */
                memcpy( pucShmCmd, (uint8_t*)&pstCmdInfo->stCmd[ucIndex], sizeof(SysCmd_t));
                iPID = ExecSingleCmd( iShmID );
                if( iPID > 0 )
                {
                    DEBUG_RTS( "ExecSysCmd: ExecSingleCmd OK, child PID %d\n", iPID );
                    iFin = (int32_t)(ESC_FINISH_NOT);
                    pstCmdInfo->iChildPID = iPID;
                    pstCmdInfo->uiWaitCycCnt = 0U;
                    pstCmdInfo->bWaitChild = true;
                }
                else
                {
                    DEBUG_RTS( "ExecSysCmd: ExecSingleCmd Error\n" );
                    iFin = (int32_t)(ESC_FINISH_FAIL);
                    bClear = true;
                }
            }
            else
            {
                iFin = (int32_t)(ESC_FINISH_OK);
                bClear = true;
                DEBUG_RTS( "ExecSysCmd Finish\n" );
            }
        }
        else
        {
            /* Wait child process finish */
            if( pstCmdInfo->uiWaitCycCnt < (uint32_t)(MAX_WAIT_CHD_CYC_CNT))
            {
                iRet = WaitChildFinish( pstCmdInfo->iChildPID );
                if((int32_t)(EWCF_FINISH) == iRet )
                {
                    DEBUG_RTS( "ExecSysCmd: Child process finish %d\n", pstCmdInfo->uiWaitCycCnt );
                    pstCmdInfo->ucCurIndex++;
                    pstCmdInfo->bWaitChild = false;
                    pstCmdInfo->uiWaitCycCnt = 0U;
                    pstCmdInfo->iChildPID = -1;
                    iFin = (int32_t)(ESC_FINISH_NOT);

                    /* Check if finished all command */
                    if( pstCmdInfo->ucCurIndex == pstCmdInfo->ucCmdCnt )
                    {
                        iFin = (int32_t)(ESC_FINISH_OK);
                        bClear = true;
                        DEBUG_RTS( "ExecSysCmd Finish\n" );
                    }
                }
                else if((int32_t)(EWCF_NOTFINISH) == iRet )
                {
                    iFin = (int32_t)(ESC_FINISH_NOT);
                    pstCmdInfo->uiWaitCycCnt++;
                }
                else
                {
                    /* Error */
                    iFin = (int32_t)(ESC_FINISH_FAIL);
                    bClear = true;
                    DEBUG_RTS( "ExecSysCmd: WaitChildFinish error %d\n", iRet );
                }
            }
            else
            {
                /* Timeout */
                DEBUG_RTS( "ExecSysCmd: Wait child process timeout\n" );
                iFin = (int32_t)(ESC_FINISH_FAIL);
                bClear = true;
            }
        }

        if( bClear )
        {
            memset( pstCmdInfo, 0, sizeof(SysCmdInfo_t));
            pstCmdInfo->iChildPID = -1;
        }
    }

    return iFin;
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
            if(((pstTSyncCfg->ucTimingSrc >= (uint8_t)(TSRC_NTP)) && (pstTSyncCfg->ucTimingSrc <= (uint8_t)(TSRC_NONE))) &&
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
* 函数名称: WaitChildFinish
*
* 功能描述: 等待子进程执行完成
*
* 输入参数: iChildPID：子进程PID
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static int32_t WaitChildFinish( int32_t iChildPID )
{
    int32_t iErrCode = (int32_t)(EWCF_PARA);
    int32_t iStatus = 0;
    int32_t iRet = -1;

    if( iChildPID > 0 )
    {
        /* Check if child process terminated */
        iRet = waitpid( iChildPID, &iStatus, WNOHANG );
        if( iRet < 0 )
        {
            /* Error */
            DEBUG_RTS( "WaitChildFinish: waitpid error %d\n", errno );
            iErrCode = (int32_t)(EWCF_WAIT_FUN);
        }
        else if( 0 == iRet )
        {
            /* Not finish */
            iErrCode = (int32_t)(EWCF_NOTFINISH);
        }
        else
        {
            /* Finished */
            iErrCode = (int32_t)(EWCF_FINISH);
#if 0
            /* Check status */
            /* WIFEXITED: normal exit - non-zero */
            /* WEXITSTATUS: if WIFEXITED is non-zero, it return exit code(0-success) */
            if(( 0 == WIFEXITED( iStatus )) ||
               ( 0 != WEXITSTATUS( iStatus )))
            {
                DEBUG_RTS( "WaitChildFinish: Unexpected child status %d\n", iStatus );
                iErrCode = EWCF_EXIT;
            }
#endif
        }
    }

    return iErrCode;
}

/*
***********************************************************************************************************************
* 函数名称: ExecSingleCmd
*
* 功能描述: 执行单条命令
*
* 输入参数: iShmID: 共享内存ID
*
* 输出参数: 无
*
* 返 回 值: 子进程PID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static int32_t ExecSingleCmd( int32_t iShmID )
{
    int32_t iPID = -1;  /* Process ID */

    if( iShmID != -1 )
    {
        iPID = fork();
        if( -1 == iPID )
        {
            DEBUG_RTS( "ExecSingleCmd: fork failed, errno %d\n", errno );
        }
        else if( 0 == iPID )
        {
            /* Execute Command */
            ChdExecCmd( iShmID );
        }
        else
        {
            /* Parent Process */
            //DEBUG_RTS( "ExecSingleCmd: Parent Process, child PID %d\n", iPID );
            usleep( 100 );  /* 100us */
        }
    }

    return iPID;
}

/*
***********************************************************************************************************************
* 函数名称: ChdExecCmd
*
* 功能描述: 子进程执行单条命令
*
* 输入参数: iShmID: 共享内存ID
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ChdExecCmd( int32_t iShmID )
{
    bool_t bInitRet = true;
    uint32_t uiIndex = 0U;
    uint8_t const *pucShmAddr = NULL;
    SysCmd_t stCmd;
    pchar_t pcArgs[MAX_ARGV_ITEM_CNT] = { NULL };  /* Point array */

    if( iShmID != -1 )
    {
        /* Child Process: Lower priority */
        if( -1 == setpriority( PRIO_PROCESS, 0, 5 ))
        {
            DEBUG_RTS( "ChdExecCmd: setpriority failed %d\n", errno );
            bInitRet = false;
        }

        if( bInitRet )
        {
            /* Attach shared memory: Read/Write */
            pucShmAddr = (uint8_t const *)shmat( iShmID, NULL, 0 );
            if((uint8_t const *)-1 == pucShmAddr )
            {
                DEBUG_RTS( "ChdExecCmd: shmat failed, errno %d\n", errno );
                pucShmAddr = NULL;
                bInitRet = false;
            }
        }

        if( bInitRet )
        {
            memcpy((uint8_t*)&stCmd, pucShmAddr, sizeof(SysCmd_t));

            printf( "ExePath: %s\n", &stCmd.cPrgPath[0]);
            printf( "ExeCmd:" );
            for( uiIndex = 0U; uiIndex < (uint32_t)(MAX_ARGV_ITEM_CNT); uiIndex++ )
            {
                if( strlen( &stCmd.cArgv[uiIndex][0]) > 0 )
                {
                    pcArgs[uiIndex] = &stCmd.cArgv[uiIndex][0];
                    printf( " %s", pcArgs[uiIndex]);
                }
                else
                {
                    break;
                }
            }
            printf( "\n" );

            if( -1 == execve( &stCmd.cPrgPath[0], pcArgs, NULL ))
            {
                DEBUG_RTS( "ChdExecCmd: execve failed, errno %d\n", errno );
                bInitRet = false;

                /* Detach */
                if( -1 == shmdt( pucShmAddr ))
                {
                    DEBUG_RTS( "ChdExecCmd: shmdt failed, errno %d\n", errno );
                }
            }
        }

        if( !bInitRet )
        {
            exit( 1 );
        }
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

