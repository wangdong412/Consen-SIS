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
*  文件名称    : SysTmp.c
*
*  功能简介    :
*
*  作者        : 李琦
*
*  创建日期    : 2016-7-7
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
#include "../inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysModbusRespMsg_t s_stModbusRespMsg[NUM_OF_NETWORK_PORT];

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
* 函数名称: SysGetLocalProjVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetLocalProjVersion( ProjVerInfo_t *pstProjVer )
{
    bool_t bRet = false;
    CtrlStationConfigInfo_t* pCfgInfo =NULL;

    if(pstProjVer != NULL)
    {
        pCfgInfo = CMCfgGetCtrlStationInfo();

        if((HAS_BE_CONFIGURED== pCfgInfo->ucIsConfigured) && (pCfgInfo != NULL))
        {
            pstProjVer->uiProjCRC = pCfgInfo->uiCRC;
            pstProjVer->uiProjID = pCfgInfo->stPrjConfigInfo.uiProjectID;
            pstProjVer->uiMainVer = pCfgInfo->stPrjConfigInfo.uiMajorVersion;
            pstProjVer->uiMinorVer = pCfgInfo->stPrjConfigInfo.uiMinorVersion;
            bRet = true;
        }
        else
        {
            pstProjVer->uiProjCRC = 0;
            pstProjVer->uiProjID = 0;
            pstProjVer->uiMainVer = 0;
            pstProjVer->uiMinorVer = 0;
            bRet = true; //false;
        }
    }
    return bRet;

}
/*
***********************************************************************************************************************
* 函数名称: SysGetPMProjVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetPMProjVersion( PMController_t emPMID, ProjVerInfo_t *pstProjVer )
{
    bool_t bRet = false;
    PrjInfo_t stPrjInfo;

    if((emPMID < NUM_OF_PM) && (pstProjVer != NULL))
    {
        SysGetPrjInfo(emPMID,&stPrjInfo);
        pstProjVer->uiProjCRC = stPrjInfo.uiPrjCRC;
        pstProjVer->uiProjID = stPrjInfo.uiPrjID;
        pstProjVer->uiMainVer = stPrjInfo.uiMainVern;
        pstProjVer->uiMinorVer = stPrjInfo.uiMinorVern;
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetDownloadProjVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetDownloadProjVersion( PMController_t emPMID, ProjVerInfo_t *pstProjVer )
{
    bool_t bRet = false;
    PrjInfo_t stPrjInfo;

    if((emPMID < NUM_OF_PM) && (pstProjVer != NULL))
    {
        SysGetDownloadPrjInfo(emPMID,&stPrjInfo);
        pstProjVer->uiProjCRC = stPrjInfo.uiPrjCRC;
        pstProjVer->uiProjID = stPrjInfo.uiPrjID;
        pstProjVer->uiMainVer = stPrjInfo.uiMainVern;
        pstProjVer->uiMinorVer = stPrjInfo.uiMinorVern;
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetPMDownloadStatus
*
* 功能描述: 获取PM系统状态中的下装状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 下装状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t SysGetPMDownloadStatus( void )
{
    uint16_t usDWStatus = 0U;
    uint16_t usPMState = 0U;
    PMController_t emPMID = PMA;

    if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
    {
        emPMID = PMA;
    }
    else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
    {
        emPMID = PMB;
    }
    else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
    {
        emPMID = PMC;
    }
    else
    {
        emPMID = INVALID_PM_ID;
    }

    if( SysGetPMState( emPMID, &usPMState ))
    {
        usDWStatus = usPMState & 0x0003;  /* Only Low 2 bits */
        printf( "DWStatus: PM%d, %d\n", emPMID, usDWStatus );
    }

    return usDWStatus;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMPrjVerInfo
*
* 功能描述: 获取PM系统状态中的工程版本
*
* 输入参数: 工程版本信息缓冲区
*
* 输出参数: 工程版本信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMPrjVerInfo( ProjVerInfo_t *pstPMPrjVerInfo )
{
    bool_t bRet = false;
    PMController_t emPMID = PMA;

    if( pstPMPrjVerInfo != NULL )
    {
        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
        {
            emPMID = PMA;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
        {
            emPMID = PMB;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
        {
            emPMID = PMC;
        }
        else
        {
            emPMID = INVALID_PM_ID;
        }

        if( SysGetPMProjVersion( emPMID, pstPMPrjVerInfo ))
        {
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusTCPConfigInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetModbusTCPConfigInfo( NetworkPortType_t emType, ModbusTCPConfigInfo_t *pstModbusTCPConfig )
{
    bool_t bRet = false;
    CMConfigInfo_t* pCMCfgInfo =NULL;
    CMController_t emCMID = INVALID_CM_ID;
    ModbusTCPConfigInfo_t *pCfg = NULL;

    pCMCfgInfo = CMCfgGetCMConfigInfo();
    emCMID = (CMController_t)CMCfgGetLocalCMID(); //SysGetLocalCMID();

    if((pCMCfgInfo != NULL) && (emType < NUM_OF_NETWORK_PORT) && (emCMID < NUM_OF_CM))
    {
        pCfg = &(pCMCfgInfo->stCMSingleInfo[emCMID].stModbusTCPConfigInfo[emType]);
        memcpy((puint8_t)pstModbusTCPConfig, (puint8_t)pCfg, sizeof(ModbusTCPConfigInfo_t));
        bRet = true;
    }
    else
    {
        bRet = false;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusMasterInfoVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetModbusMasterInfoVersion( puint32_t puiCRC, puint32_t puiSubVern )
{
    bool_t bRet = false;
    ModbusMasterInfo_t* pstCfg = NULL;

    pstCfg = CMCfgGetModbusMasterInfo();

    if((pstCfg != NULL) && (puiCRC != NULL) && (puiSubVern != NULL))
    {
        *puiCRC = pstCfg->uiCRC;
        *puiSubVern = pstCfg->uiFileVern;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusMasterInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetModbusMasterInfo( puint8_t pucBuff, uint32_t uiOffset, uint32_t uiLength )
{
    uint32_t uiLen = 0;
    ModbusMasterInfo_t* pstCfg = NULL;
    puint8_t pucCfg = NULL;

    pstCfg = CMCfgGetModbusMasterInfo();
    pucCfg = (puint8_t)pstCfg;

    if((pstCfg != NULL) && (pucBuff != NULL))
    {
        if((uiOffset + uiLength) < sizeof(ModbusMasterInfo_t))
        {
            uiLen = uiLength;
        }
        else
        {
            uiLen = sizeof(ModbusMasterInfo_t) - uiOffset;
        }
        memcpy(pucBuff, (puint8_t)(&pucCfg[uiOffset]), uiLen);
    }
    return uiLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusSlaveConfigVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetModbusSlaveConfigVersion( puint32_t puiCRC, puint32_t puiSubVern)
{
    bool_t bRet = false;
    ModbusSlaveInfo_t* pstCfg = NULL;

    pstCfg = CMCfgGetModbusSlaveInfo();

    if((pstCfg != NULL) && (puiCRC != NULL) && (puiSubVern != NULL))
    {
        *puiCRC = pstCfg->uiCRC;
        *puiSubVern = pstCfg->uiFileVern;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusSlaveConfigInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetModbusSlaveConfigInfo( puint8_t pucBuff, uint32_t uiOffset, uint32_t uiLength )
{
    uint32_t uiLen = 0;
    ModbusSlaveInfo_t* pstCfg = NULL;
    puint8_t pucCfg = NULL;

    pstCfg = CMCfgGetModbusSlaveInfo();
    pucCfg = (puint8_t)pstCfg;

    if((pstCfg != NULL) && (pucBuff != NULL))
    {
        if((uiOffset + uiLength) < sizeof(ModbusSlaveInfo_t))
        {
            uiLen = uiLength;
        }
        else
        {
            uiLen = sizeof(ModbusSlaveInfo_t) - uiOffset;
        }
        memcpy(pucBuff, (puint8_t)(&pucCfg[uiOffset]), uiLen);
    }
    return uiLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCtrlStationConfigVersion
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCtrlStationConfigVersion( puint32_t puiCRC, puint32_t puiSubVern )
{
    bool_t bRet = false;
    CtrlStationConfigInfo_t* pstCfg = NULL;

    pstCfg = CMCfgGetCtrlStationInfo();

    if((pstCfg != NULL) && (puiCRC != NULL) && (puiSubVern != NULL))
    {
        *puiCRC = pstCfg->uiCRC;
        *puiSubVern = pstCfg->uiFileVern;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetTaskConfigInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetTaskConfigInfo( Task_ID_t emTask, TaskConfigInfo_t *pstTaskCfgInfo )
{
    bool_t bRet = false;
    CtrlStationConfigInfo_t* pstCfg = NULL;
    TaskConfigInfo_t *pstTaskCfg = NULL;

    pstCfg = CMCfgGetCtrlStationInfo();

    if((pstCfg != NULL) && (emTask < MAX_TASK_NUM) && (pstTaskCfgInfo != NULL))
    {
        pstTaskCfg = &(pstCfg->stTaskConfigInfo[emTask]);
        memcpy((puint8_t)pstTaskCfgInfo, (puint8_t)pstTaskCfg, sizeof(TaskConfigInfo_t));
        bRet = true;
    }
    else
    {
        bRet = false;
    }
    return bRet;

}

/*
***********************************************************************************************************************
* 函数名称: SysGetCMConfigInfoActiveFlag
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
ActiveFlag_t SysGetCMConfigInfoActiveFlag(void)
{
    CMConfigInfo_t* pCMCfgInfo =NULL;
    CMController_t emCMID = INVALID_CM_ID;
    ActiveFlag_t emFlag = NOT_ACTIVE;

    pCMCfgInfo = CMCfgGetCMConfigInfo();
    emCMID = (CMController_t)CMCfgGetLocalCMID();  //SysGetLocalCMID();

    if((pCMCfgInfo != NULL) && (emCMID < NUM_OF_CM))
    {
        emFlag = pCMCfgInfo->stCMSingleInfo[emCMID].emActive;
    }
    return emFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusRespMsg
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: Get/Set Slave Response: Max 256 Bytes        Modbus TCP Master
***********************************************************************************************************************
*/
bool_t SysGetModbusRespMsg( NetworkPortType_t emType, puint8_t pucContent, puint16_t pusLen )
{
    bool_t bRet = false;
    uint16_t usLen = 0;

    if((emType < NUM_OF_NETWORK_PORT) && (pucContent != NULL) && (pusLen != NULL))
    {
        usLen = s_stModbusRespMsg[emType].usLen;

        if(usLen > 0)
        {
            memcpy(pucContent, (puint8_t)&(s_stModbusRespMsg[emType].ucContent[0]), usLen);
            *pusLen = usLen;
            s_stModbusRespMsg[emType].usLen = 0;
            bRet = true;
        }
        else
        {
            *pusLen = 0;
            bRet = true;
        }
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetModbusRespMsg
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetModbusRespMsg( NetworkPortType_t emType, puint8_t pucContent, uint16_t usLen )
{
    bool_t bRet = false;

    if((emType < NUM_OF_NETWORK_PORT) && (pucContent != NULL) && (usLen > 0))
    {
        memcpy((puint8_t)&(s_stModbusRespMsg[emType].ucContent[0]), pucContent, usLen);
        s_stModbusRespMsg[emType].usLen = usLen;
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysClearModbusRespMsg
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: Get/Set Slave Response: Max 256 Bytes        Modbus TCP Master
***********************************************************************************************************************
*/
bool_t SysClearModbusRespMsg( NetworkPortType_t emType )
{
    bool_t bRet = false;
    //uint16_t usLen = 0;

    if( emType < NUM_OF_NETWORK_PORT )
    {
        s_stModbusRespMsg[emType].usLen = 0;
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusMasterBlkStartFlag
*
* 功能描述: Modbus Master Block Start Flag
*
* 输入参数: uiBlockID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetModbusMasterBlkStartFlag( uint32_t uiBlockID )
{
    bool_t bRet = false;
    SharedAppend_t stAppend;
    PMController_t emPMID = INVALID_PM_ID;
    uint8_t ucVal = 0;
    uint8_t ucOffset = 0;

    if(uiBlockID < 64)
    {
        memset((puint8_t)&stAppend, 0 , sizeof(SharedAppend_t));

        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
        {
            emPMID = PMA;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
        {
            emPMID = PMB;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
        {
            emPMID = PMC;
        }
        else
        {
            emPMID = INVALID_PM_ID;
            bRet = false;
        }

        if((emPMID >= PMA) && (emPMID <= PMC))
        {
            SysGetAppendInfo(emPMID, &stAppend);

            ucOffset = uiBlockID % 8;
//            printf("Offset=%d\n", ucOffset);
//            ucVal = stAppend.ucModBusCtrl[uiBlockID/8];
//            printf("val=%d\n", ucVal);
//            ucVal = (ucVal >> ucOffset) & 0x01;

            ucVal = stAppend.ucModBusCtrl[uiBlockID];

            if(ucVal == MODBUS_STATE_START)
            {
                bRet = true;
            }
            else
            {
                bRet = false;
            }
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileType
*
* 功能描述: 根据文件处理序号获取文件的ID
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
SysCfgFileType_t SysGetCfgFileType(SysFileProcessSeq_t emFileProcessSeq)
{
    SysCfgFileType_t emCfgFileType = INVALID_CFG_FILE_TYPE;
    
    switch(emFileProcessSeq)
    {
        case PS_PRG:
        {
            emCfgFileType = CFG_PRG;
        }
        break;
        case PS_CM:
        {
            emCfgFileType = CFG_CM;
        }
        break;
        case PS_IO_MODULAR:
        {
            emCfgFileType = CFG_IO_MODULAR;
        }
        break;
        case PS_SOE:
        {
            emCfgFileType = CFG_SOE;
        }
        break;
        case PS_P2P:
        {
            emCfgFileType = CFG_P2P;
        }
        break;
        case PS_MODBUS_MASTER:
        {
            emCfgFileType = CFG_MODBUS_MASTER;
        }
        break;
        case PS_MODBUS_SLAVE:
        {
            emCfgFileType = CFG_MODBUS_SLAVE;
        }
        break;
        case PS_VOTE:
        {
            emCfgFileType = CFG_VOTE;
        }
        break;
        case PS_RETAIN:
        {
            emCfgFileType = CFG_RETAIN;
        }
        break;
        case PS_OPC:
        {
            emCfgFileType = CFG_OPC;
        }
        break;
        case PS_CTRL_STATION:
        {
            emCfgFileType = CFG_CTRL_STATION;
        }
        break;
        default:
        break;
    }
    
    return emCfgFileType;
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



