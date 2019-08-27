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
*  文件名称    : SysCfgFile.c
*
*  功能简介    :
*
*  作者        : 李琦
*
*  创建日期    : 2016-03-17
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
static SysCfgDB_t s_stSysCfgDB;
static SysCfgFileTable_t s_stCfgFileTable;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 读各配置文件 */
static uint16_t SysGetCfgFileCMInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileModbusMasterInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileModbusSlaveInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileOPCInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileCtrlStationInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileSOEInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileP2PInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileVoteInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileIOModInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFileRetainInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);
static uint16_t SysGetCfgFilePrgInfo(uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);

/* 写各配置文件 */
static uint16_t SysSetCfgFileCMInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileModbusMasterInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileModbusSlaveInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileOPCInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileCtrlStationInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileSOEInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileP2PInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileVoteInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileIOModInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFileRetainInfo(puint8_t pucContent, uint16_t usLen);
static uint16_t SysSetCfgFilePrgInfo(puint8_t pucContent, uint16_t usLen);
/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysDatabseInit
*
* 功能描述: 静态变量初始化
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
void SysDatabseInit(void)
{
    /* 初始化DB */
    s_stSysCfgDB.bDBStatus = CFG_DB_EMPTY;
    s_stSysCfgDB.bReadable = CFG_DB_RD_DISABLE;
    s_stSysCfgDB.bWriteable = CFG_DB_WR_ENABLE;

    /* 初始化CM配置文件缓冲区 */
    s_stSysCfgDB.stCfgCM.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgCM.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgCM.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgCM.uiLen = 0;
    s_stSysCfgDB.stCfgCM.uiWrOffset = 0;

    /* 初始化Modbus Master配置文件缓冲区 */
    s_stSysCfgDB.stCfgModbusMaster.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgModbusMaster.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgModbusMaster.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgModbusMaster.uiLen = 0;
    s_stSysCfgDB.stCfgModbusMaster.uiWrOffset = 0;

    /* 初始化Modbus Slave配置文件缓冲区 */
    s_stSysCfgDB.stCfgModbusSlave.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgModbusSlave.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgModbusSlave.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgModbusSlave.uiLen = 0;
    s_stSysCfgDB.stCfgModbusSlave.uiWrOffset = 0;

    /* 初始化OPC配置文件缓冲区 */
    s_stSysCfgDB.stCfgOPC.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgOPC.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgOPC.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgOPC.uiLen = 0;
    s_stSysCfgDB.stCfgOPC.uiWrOffset = 0;

    /* 初始化control station配置文件缓冲区*/
    s_stSysCfgDB.stCfgCtrlStation.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgCtrlStation.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgCtrlStation.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgCtrlStation.uiLen = 0;
    s_stSysCfgDB.stCfgCtrlStation.uiWrOffset = 0;

    /* 初始化SOE配置文件缓冲区 */
    s_stSysCfgDB.stCfgSOE.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgSOE.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgSOE.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgSOE.uiLen = 0;
    s_stSysCfgDB.stCfgSOE.uiWrOffset = 0;

    /* 初始化P2P配置文件缓冲区*/
    s_stSysCfgDB.stCfgP2P.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgP2P.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgP2P.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgP2P.uiLen = 0;
    s_stSysCfgDB.stCfgP2P.uiWrOffset = 0;

    /* 初始化IO模块配置文件缓冲区*/
    s_stSysCfgDB.stCfgIOMod.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgIOMod.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgIOMod.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgIOMod.uiLen = 0;
    s_stSysCfgDB.stCfgIOMod.uiWrOffset = 0;

    /* 初始化vote配置文件缓冲区*/
    s_stSysCfgDB.stCfgVote.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgVote.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgVote.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgVote.uiLen = 0;
    s_stSysCfgDB.stCfgVote.uiWrOffset = 0;

    /* 初始化Retain配置文件缓冲区*/
    s_stSysCfgDB.stCfgRetain.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgRetain.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgRetain.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgRetain.uiLen = 0;
    s_stSysCfgDB.stCfgRetain.uiWrOffset = 0;

    /* 初始化PRG配置文件缓冲区*/
    s_stSysCfgDB.stCfgPrg.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgPrg.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgPrg.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgPrg.uiLen = 0;
    s_stSysCfgDB.stCfgPrg.uiWrOffset = 0;

    memset((puint8_t)&s_stCfgFileTable, 0, sizeof(SysCfgFileTable_t));
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysClearDatabase
*
* 功能描述: 清空数据库
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
void SysClearDatabase(void)
{
    /* 初始化DB */
    s_stSysCfgDB.bDBStatus = CFG_DB_EMPTY;
    s_stSysCfgDB.bReadable = CFG_DB_RD_DISABLE;
    s_stSysCfgDB.bWriteable = CFG_DB_WR_ENABLE;

    /* 初始化CM配置文件缓冲区 */
    s_stSysCfgDB.stCfgCM.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgCM.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgCM.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgCM.uiLen = 0;
    s_stSysCfgDB.stCfgCM.uiWrOffset = 0;

    /* 初始化Modbus Master配置文件缓冲区 */
    s_stSysCfgDB.stCfgModbusMaster.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgModbusMaster.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgModbusMaster.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgModbusMaster.uiLen = 0;
    s_stSysCfgDB.stCfgModbusMaster.uiWrOffset = 0;

    /* 初始化Modbus Slave配置文件缓冲区 */
    s_stSysCfgDB.stCfgModbusSlave.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgModbusSlave.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgModbusSlave.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgModbusSlave.uiLen = 0;
    s_stSysCfgDB.stCfgModbusSlave.uiWrOffset = 0;

    /* 初始化OPC配置文件缓冲区 */
    s_stSysCfgDB.stCfgOPC.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgOPC.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgOPC.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgOPC.uiLen = 0;
    s_stSysCfgDB.stCfgOPC.uiWrOffset = 0;

    /* 初始化control station配置文件缓冲区*/
    s_stSysCfgDB.stCfgCtrlStation.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgCtrlStation.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgCtrlStation.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgCtrlStation.uiLen = 0;
    s_stSysCfgDB.stCfgCtrlStation.uiWrOffset = 0;

    /* 初始化SOE配置文件缓冲区 */
    s_stSysCfgDB.stCfgSOE.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgSOE.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgSOE.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgSOE.uiLen = 0;
    s_stSysCfgDB.stCfgSOE.uiWrOffset = 0;

    /* 初始化P2P配置文件缓冲区*/
    s_stSysCfgDB.stCfgP2P.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgP2P.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgP2P.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgP2P.uiLen = 0;
    s_stSysCfgDB.stCfgP2P.uiWrOffset = 0;

    /* 初始化IO模块配置文件缓冲区*/
    s_stSysCfgDB.stCfgIOMod.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgIOMod.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgIOMod.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgIOMod.uiLen = 0;
    s_stSysCfgDB.stCfgIOMod.uiWrOffset = 0;

    /* 初始化vote配置文件缓冲区*/
    s_stSysCfgDB.stCfgVote.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgVote.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgVote.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgVote.uiLen = 0;
    s_stSysCfgDB.stCfgVote.uiWrOffset = 0;

    /* 初始化Retain配置文件缓冲区*/
    s_stSysCfgDB.stCfgRetain.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgRetain.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgRetain.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgRetain.uiLen = 0;
    s_stSysCfgDB.stCfgRetain.uiWrOffset = 0;

    /* 初始化PRG配置文件缓冲区*/
    s_stSysCfgDB.stCfgPrg.bReadable = CFG_FILE_RD_DISABLE;
    s_stSysCfgDB.stCfgPrg.bWriteable = CFG_FILE_WR_ENABLE;
    s_stSysCfgDB.stCfgPrg.bFileStatus = CFG_FILE_WR_NOT_FINISH;
    s_stSysCfgDB.stCfgPrg.uiLen = 0;
    s_stSysCfgDB.stCfgPrg.uiWrOffset = 0;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileTable
*
* 功能描述: 将配置文件表存储到系统资源管理模块
*
* 输入参数: pstCfgFileTable: 指向配置文件表
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCfgFileTable(SysCfgFileTable_t *pstCfgFileTable)
{
    puint8_t pucBuf = NULL;

    if(pstCfgFileTable != NULL)
    {
        pucBuf = (puint8_t)pstCfgFileTable;
        memcpy((puint8_t)&s_stCfgFileTable, pucBuf, sizeof(SysCfgFileTable_t));
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileTable
*
* 功能描述: 从系统资源管理模块读取配置文件表存
*
* 输入参数: 无
*
* 输出参数: pstCfgFileTable: 指向配置文件表
*
* 返 回 值  : true 有就绪的配置文件列表
*        false 没有就绪的配置文件列表
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCfgFileTable(SysCfgFileTable_t *pstCfgFileTable)
{
    puint8_t pucBuf = NULL;
    bool_t bRet = false;

    if(pstCfgFileTable != NULL)
    {
        if(true == s_stCfgFileTable.bFlag)
        {
            pucBuf = (puint8_t)pstCfgFileTable;
            memcpy(pucBuf, (puint8_t)&s_stCfgFileTable, sizeof(SysCfgFileTable_t));
            s_stCfgFileTable.bFlag = false;
            bRet = true;
        }
        else
        {
            bRet = false;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileContentStartAddr
*
* 功能描述: 获取指定配置文件内容的起始地址
*
* 输入参数: emFileIndex：配置文件索引号
*
* 输出参数: 无
*
* 返 回 值  : 起始地址
*
* 注意事项: 文件无法访问，返回NULL
***********************************************************************************************************************
*/
puint8_t SysGetCfgFileContentStartAddr(SysCfgFileType_t emFileIndex)
{
    puint8_t pucStartAddr = NULL;
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */

    /* 找到待读出配置信息的数据库 */
    pstCfg = &s_stSysCfgDB;

    switch(emFileIndex)
    {
        case CFG_CM:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgCM.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgCM.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgCM.ucContent[0]);
                }
            }
        }
        break;

        case CFG_MODBUS_MASTER:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgModbusMaster.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgModbusMaster.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgModbusMaster.ucContent[0]);
                }
            }
        }
        break;

        case CFG_MODBUS_SLAVE:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgModbusSlave.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgModbusSlave.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgModbusSlave.ucContent[0]);
                }
            }
        }
        break;

        case CFG_OPC:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgOPC.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgOPC.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgOPC.ucContent[0]);
                }
            }
        }
        break;

        case CFG_CTRL_STATION:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgCtrlStation.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgCtrlStation.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgCtrlStation.ucContent[0]);
                }
            }
        }
        break;

        case CFG_SOE:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgSOE.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgSOE.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgSOE.ucContent[0]);
                }
            }
        }
        break;

        case CFG_P2P:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgP2P.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgP2P.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgP2P.ucContent[0]);
                }
            }
        }
        break;

        case CFG_VOTE:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgVote.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgVote.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgVote.ucContent[0]);
                }
            }
        }
        break;

        case CFG_IO_MODULAR:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgIOMod.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgIOMod.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgIOMod.ucContent[0]);
                }
            }
        }
        break;

        case CFG_RETAIN:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgRetain.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgRetain.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgRetain.ucContent[0]);
                }
            }
        }
        break;

        case CFG_PRG:
        {
            /* 文件可读 */
            if(CFG_FILE_RD_ENABLE == pstCfg->stCfgPrg.bReadable)
            {
                if(CFG_FILE_WR_FINISH == pstCfg->stCfgPrg.bFileStatus)
                {
                    pucStartAddr = &(pstCfg->stCfgPrg.ucContent[0]);
                }
            }
        }
        break;

        default:
        break;

    }

    return pucStartAddr;
}



/*
***********************************************************************************************************************
* 函数名称: SysCheckCfgFileTransFlag
*
* 功能描述: 是否需要传输相应的配置文件到相应的CM
*
* 输入参数: emCMID：CM的ID号
*        emFileIndex：配置文件索引号
*
* 输出参数: 无
*
* 返 回 值  : true-需要传输
*        false-不需要传输
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysCheckCfgFileTransFlag(CMController_t emCMID, SysCfgFileType_t emFileIndex)
{
    bool_t bRet = false;

    if(emCMID < NUM_OF_CM)
    {
        if(CFG_CM == emFileIndex)
        {
            if(true == s_stCfgFileTable.bReady[emCMID])
            {
                /* 是否向相应的CM传输CM配置文件 */
                bRet = s_stCfgFileTable.bCfgFileCM[emCMID];
            }
        }
        else if(CFG_MODBUS_MASTER == emFileIndex)
        {
            if(true == s_stCfgFileTable.bReady[emCMID])
            {
                /* 是否向相应的CM传输Modbus Master配置文件 */
                bRet = s_stCfgFileTable.bCfgFileModbusMaster[emCMID];
            }
        }
        else if(CFG_MODBUS_SLAVE == emFileIndex)
        {
            if(true == s_stCfgFileTable.bReady[emCMID])
            {
                /* 是否向相应的CM传输Modbus Slave配置文件 */
                bRet = s_stCfgFileTable.bCfgFileModbusSlave[emCMID];
            }
        }
        else if(CFG_OPC == emFileIndex)
        {
            if(true == s_stCfgFileTable.bReady[emCMID])
            {
                /* 是否向相应的CM传输OPC配置文件 */
                bRet = s_stCfgFileTable.bCfgFileOPC[emCMID];
            }
        }
        else if(CFG_CTRL_STATION == emFileIndex)
        {
            if(true == s_stCfgFileTable.bReady[emCMID])
            {
                /* 是否向相应的CM传输控制站配置文件 */
                bRet = s_stCfgFileTable.bCfgFileCtrlStation[emCMID];
            }
        }
        else
        {
            bRet = false;
        }
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: emCfgFileType: 各种配置文件类型
*        uiRdOffset: 读偏移地址
* *      usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 读取后不清除数据库中相应配置文件中的内容
***********************************************************************************************************************
*/
uint16_t SysGetCfgFileInfo (SysCfgFileType_t emCfgFileType, uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    uint16_t usRdLen = 0;

    /* 输出参数有效性检查 */
    if(pucContent != NULL)
    {
        /* 输入参数有效性检查 */
        if((emCfgFileType < NUM_OF_CFG_FILE_TYPE) && (usLen > 0))
        {
            /* CM配置文件*/
            if(CFG_CM == emCfgFileType)
            {
                usRdLen = SysGetCfgFileCMInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_MODBUS_MASTER == emCfgFileType)
            {
                usRdLen = SysGetCfgFileModbusMasterInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_MODBUS_SLAVE == emCfgFileType)
            {
                usRdLen = SysGetCfgFileModbusSlaveInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_OPC == emCfgFileType)
            {
                usRdLen = SysGetCfgFileOPCInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_CTRL_STATION == emCfgFileType)
            {
                usRdLen = SysGetCfgFileCtrlStationInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_SOE == emCfgFileType)
            {
                usRdLen = SysGetCfgFileSOEInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_P2P == emCfgFileType)
            {
                usRdLen = SysGetCfgFileP2PInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_VOTE == emCfgFileType)
            {
                usRdLen = SysGetCfgFileVoteInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_IO_MODULAR == emCfgFileType)
            {
                usRdLen = SysGetCfgFileIOModInfo(uiRdOffset, pucContent, usLen);
            }
            else if(CFG_RETAIN == emCfgFileType)
            {
                usRdLen = SysGetCfgFileRetainInfo(uiRdOffset, pucContent, usLen);
            }
            else /* CFG_PRG == emCfgFileType */
            {
                usRdLen = SysGetCfgFilePrgInfo(uiRdOffset, pucContent, usLen);
            }
        }
        else
        {
            /* 输入参数无效 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 输出参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileState
*
* 功能描述: 从系统资源模块获取配置文件的各种状态信息
*
* 输入参数: emCfgFileType: 各种配置文件类型
*
* 输出参数: pstSysCfgFileState: 指向相应的配置文件的各种状态信息
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetCfgFileState (SysCfgFileType_t emCfgFileType, SysCfgFileState_t *pstSysCfgFileState)
{
    SysCfgDB_t *pstCfg = NULL;

    /* 输出参数有效性检查 */
    if(pstSysCfgFileState != NULL)
    {
        /* 输入参数有效性检查 */
        if(emCfgFileType < NUM_OF_CFG_FILE_TYPE)
        {
            /* 找到待读出配置信息的数据库 */
            pstCfg = &s_stSysCfgDB;

            /* CM配置文件*/
            if(CFG_CM == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgCM.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgCM.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgCM.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgCM.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgCM.uiWrOffset;
            }
            else if(CFG_MODBUS_MASTER == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgModbusMaster.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgModbusMaster.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgModbusMaster.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgModbusMaster.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgModbusMaster.uiWrOffset;
            }
            else if(CFG_MODBUS_SLAVE == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgModbusSlave.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgModbusSlave.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgModbusSlave.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgModbusSlave.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgModbusSlave.uiWrOffset;
            }
            else if(CFG_OPC == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgOPC.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgOPC.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgOPC.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgOPC.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgOPC.uiWrOffset;
            }
            else if(CFG_CTRL_STATION == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgCtrlStation.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgCtrlStation.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgCtrlStation.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgCtrlStation.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgCtrlStation.uiWrOffset;
            }
            else if(CFG_SOE == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgSOE.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgSOE.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgSOE.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgSOE.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgSOE.uiWrOffset;
            }
            else if(CFG_P2P == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgP2P.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgP2P.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgP2P.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgP2P.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgP2P.uiWrOffset;
            }
            else if(CFG_VOTE == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgVote.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgVote.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgVote.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgVote.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgVote.uiWrOffset;
            }
            else if(CFG_IO_MODULAR == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgIOMod.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgIOMod.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgIOMod.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgIOMod.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgIOMod.uiWrOffset;
            }
            else if(CFG_RETAIN == emCfgFileType)
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgRetain.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgRetain.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgRetain.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgRetain.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgRetain.uiWrOffset;
            }
            else /* CFG_PRG == emCfgFileType */
            {
                pstSysCfgFileState->bReadable = pstCfg->stCfgPrg.bReadable;
                pstSysCfgFileState->bWriteable = pstCfg->stCfgPrg.bWriteable;
                pstSysCfgFileState->bFileStatus = pstCfg->stCfgPrg.bFileStatus;
                pstSysCfgFileState->uiLen = pstCfg->stCfgPrg.uiLen;
                pstSysCfgFileState->uiWrOffset = pstCfg->stCfgPrg.uiWrOffset;
            }
        }
        else
        {
            /* 配置文件类型错误 */

        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileInfo
*
* 功能描述: 写入配置信息内容
*
* 输入参数:  emCfgFileType: 配置信息类型
*         pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysSetCfgFileInfo(SysCfgFileType_t emCfgFileType, puint8_t pucContent, uint16_t usLen)
{
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */

    /* 输入输出参数有效性检查 */
    if((emCfgFileType < NUM_OF_CFG_FILE_TYPE) && (pucContent != NULL) && (usLen != 0))
    {
        /* CM配置文件*/
        if(CFG_CM == emCfgFileType)
        {
            usWrLen = SysSetCfgFileCMInfo(pucContent, usLen);
        }
        else if(CFG_MODBUS_MASTER == emCfgFileType)
        {
            usWrLen = SysSetCfgFileModbusMasterInfo(pucContent, usLen);
        }
        else if(CFG_MODBUS_SLAVE == emCfgFileType)
        {
            usWrLen = SysSetCfgFileModbusSlaveInfo(pucContent, usLen);
        }
        else if(CFG_OPC == emCfgFileType)
        {
            usWrLen = SysSetCfgFileOPCInfo(pucContent, usLen);
        }
        else if(CFG_CTRL_STATION == emCfgFileType)
        {
            usWrLen = SysSetCfgFileCtrlStationInfo(pucContent, usLen);
        }
        else if(CFG_SOE == emCfgFileType)
        {
            usWrLen = SysSetCfgFileSOEInfo(pucContent, usLen);
        }
        else if(CFG_P2P == emCfgFileType)
        {
            usWrLen = SysSetCfgFileP2PInfo(pucContent, usLen);
        }
        else if(CFG_VOTE == emCfgFileType)
        {
            usWrLen = SysSetCfgFileVoteInfo(pucContent, usLen);
        }
        else if(CFG_IO_MODULAR == emCfgFileType)
        {
            usWrLen = SysSetCfgFileIOModInfo(pucContent, usLen);
        }
        else if(CFG_RETAIN == emCfgFileType)
        {
            usWrLen = SysSetCfgFileRetainInfo(pucContent, usLen);
        }
        else /* CFG_PRG == emCfgFileType */
        {
            usWrLen = SysSetCfgFilePrgInfo(pucContent, usLen);
        }

    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileFinished
*
* 功能描述: 写入配置文件结束
*
* 输入参数:  emCfgFileType: 配置文件类型
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCfgFileFinished(SysCfgFileType_t emCfgFileType)
{
    SysCfgDB_t *pstCfg = NULL;

    if(emCfgFileType < NUM_OF_CFG_FILE_TYPE)
    {
        pstCfg = &s_stSysCfgDB;

        /* CM配置文件*/
        if(CFG_CM == emCfgFileType)
        {
            pstCfg->stCfgCM.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgCM.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgCM.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_MODBUS_MASTER == emCfgFileType)
        {
            pstCfg->stCfgModbusMaster.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgModbusMaster.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgModbusMaster.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_MODBUS_SLAVE == emCfgFileType)
        {
            pstCfg->stCfgModbusSlave.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgModbusSlave.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgModbusSlave.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_OPC == emCfgFileType)
        {
            pstCfg->stCfgOPC.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgOPC.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgOPC.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_CTRL_STATION == emCfgFileType)
        {
            pstCfg->stCfgCtrlStation.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgCtrlStation.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgCtrlStation.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_SOE == emCfgFileType)
        {
            pstCfg->stCfgSOE.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgSOE.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgSOE.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_P2P == emCfgFileType)
        {
            pstCfg->stCfgP2P.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgP2P.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgP2P.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_VOTE == emCfgFileType)
        {
            pstCfg->stCfgVote.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgVote.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgVote.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_IO_MODULAR == emCfgFileType)
        {
            pstCfg->stCfgIOMod.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgIOMod.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgIOMod.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else if(CFG_RETAIN == emCfgFileType)
        {
            pstCfg->stCfgRetain.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgRetain.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgRetain.bWriteable = CFG_FILE_WR_DISABLE;
        }
        else /* CFG_PRG == emCfgFileType */
        {
            pstCfg->stCfgPrg.bFileStatus = CFG_FILE_WR_FINISH;
            pstCfg->stCfgPrg.bReadable = CFG_FILE_RD_ENABLE;
            pstCfg->stCfgPrg.bWriteable = CFG_FILE_WR_DISABLE;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysDeleteCfgFile
*
* 功能描述: 删除配置文件
*
* 输入参数:  emCfgFileType: 配置文件类型
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysDeleteCfgFile(SysCfgFileType_t emCfgFileType)
{
    SysCfgDB_t *pstCfg = NULL;

    if(emCfgFileType < NUM_OF_CFG_FILE_TYPE)
    {
        pstCfg = &s_stSysCfgDB;

        /* CM配置文件*/
        if(CFG_CM == emCfgFileType)
        {
            pstCfg->stCfgCM.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgCM.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgCM.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgCM.uiWrOffset = 0;
            pstCfg->stCfgCM.uiLen = 0;
        }
        else if(CFG_MODBUS_MASTER == emCfgFileType)
        {
            pstCfg->stCfgModbusMaster.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgModbusMaster.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgModbusMaster.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgModbusMaster.uiWrOffset = 0;
            pstCfg->stCfgModbusMaster.uiLen = 0;
        }
        else if(CFG_MODBUS_SLAVE == emCfgFileType)
        {
            pstCfg->stCfgModbusSlave.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgModbusSlave.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgModbusSlave.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgModbusSlave.uiWrOffset = 0;
            pstCfg->stCfgModbusSlave.uiLen = 0;
        }
        else if(CFG_OPC == emCfgFileType)
        {
            pstCfg->stCfgOPC.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgOPC.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgOPC.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgOPC.uiWrOffset = 0;
            pstCfg->stCfgOPC.uiLen = 0;
        }
        else if(CFG_CTRL_STATION == emCfgFileType)
        {
            pstCfg->stCfgCtrlStation.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgCtrlStation.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgCtrlStation.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgCtrlStation.uiWrOffset = 0;
            pstCfg->stCfgCtrlStation.uiLen = 0;
        }
        else if(CFG_SOE == emCfgFileType)
        {
            pstCfg->stCfgSOE.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgSOE.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgSOE.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgSOE.uiWrOffset = 0;
            pstCfg->stCfgSOE.uiLen = 0;
        }
        else if(CFG_P2P == emCfgFileType)
        {
            pstCfg->stCfgP2P.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgP2P.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgP2P.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgP2P.uiWrOffset = 0;
            pstCfg->stCfgP2P.uiLen = 0;
        }
        else if(CFG_VOTE == emCfgFileType)
        {
            pstCfg->stCfgVote.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgVote.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgVote.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgVote.uiWrOffset = 0;
            pstCfg->stCfgVote.uiLen = 0;
        }
        else if(CFG_IO_MODULAR == emCfgFileType)
        {
            pstCfg->stCfgIOMod.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgIOMod.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgIOMod.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgIOMod.uiWrOffset = 0;
            pstCfg->stCfgIOMod.uiLen = 0;
        }
        else if(CFG_RETAIN == emCfgFileType)
        {
            pstCfg->stCfgRetain.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgRetain.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgRetain.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgRetain.uiWrOffset = 0;
            pstCfg->stCfgRetain.uiLen = 0;
        }
        else /* CFG_PRG == emCfgFileType */
        {
            pstCfg->stCfgPrg.bFileStatus = CFG_FILE_WR_NOT_FINISH;
            pstCfg->stCfgPrg.bReadable = CFG_FILE_RD_DISABLE;
            pstCfg->stCfgPrg.bWriteable = CFG_FILE_WR_ENABLE;
            pstCfg->stCfgPrg.uiWrOffset = 0;
            pstCfg->stCfgPrg.uiLen = 0;
        }
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
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetCfgFileCMInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgCM.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgCM.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgCM.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileModbusMasterInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgModbusMaster.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgModbusMaster.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgModbusMaster.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileModbusSlaveInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgModbusSlave.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgModbusSlave.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgModbusSlave.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileOPCInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgOPC.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgOPC.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgOPC.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileCtrlStationInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgCtrlStation.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgCtrlStation.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgCtrlStation.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileSOEInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgSOE.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgSOE.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgSOE.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileP2PInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgP2P.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgP2P.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgP2P.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileVoteInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgVote.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgVote.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgVote.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileIOModInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgIOMod.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgIOMod.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgIOMod.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFileRetainInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgRetain.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgRetain.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgRetain.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileCMInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: uiRdOffset: 读偏移地址
*        usLen: 希望读取的数据长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t SysGetCfgFilePrgInfo (uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 指向配置文件数据库 */
    uint16_t usRdLen = 0;                               /* 实际读取数据长度 */
    uint32_t uiLen = 0;                                 /* 存储配置文件长度 */

    if((pucContent != NULL) && (usLen > 0))
    {
        /* 找到待读出配置信息的数据库 */
        pstCfg = &s_stSysCfgDB;

        /* 文件可读 */
        if(CFG_FILE_RD_ENABLE == pstCfg->stCfgPrg.bReadable)
        {
            /* 获取配置文件长度 */
            uiLen = pstCfg->stCfgPrg.uiLen;

            /* 判断读取偏移是否超范围 */
            if(uiRdOffset < uiLen)
            {
                if((uiRdOffset + (uint32_t)usLen) < uiLen)
                {
                    /* 剩余内容充足 */
                    usRdLen = usLen;
                }
                else
                {
                    /* 剩余内容不足 */
                    usRdLen = (uint16_t)(uiLen - uiRdOffset);
                }
                /* 获取配置文件内容 */
                memcpy(pucContent,(puint8_t)&(pstCfg->stCfgPrg.ucContent[uiRdOffset]), usRdLen);
            }
            else
            {
                /* 超出文件范围 */
                usRdLen = 0;
            }
        }
        else
        {
            /* 文件不可读 */
            usRdLen = 0;
        }
    }
    else
    {
        /* 参数无效 */
        usRdLen = 0;
    }
    return usRdLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileCMInfo
*
* 功能描述: 写入CM配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileCMInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgCM.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgCM.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_CM_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_CM_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_CM_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgCM.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgCM.uiWrOffset += usWrLen;
            pstCfg->stCfgCM.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileModbusMasterInfo
*
* 功能描述: 写入Modbus master配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileModbusMasterInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgModbusMaster.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgModbusMaster.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_MODBUS_MASTER_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_MODBUS_MASTER_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_MODBUS_MASTER_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgModbusMaster.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgModbusMaster.uiWrOffset += usWrLen;
            pstCfg->stCfgModbusMaster.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileModbusSlaveInfo
*
* 功能描述: 写入Modbus slave配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileModbusSlaveInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgModbusSlave.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgModbusSlave.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_MODBUS_SLAVE_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_MODBUS_SLAVE_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_MODBUS_SLAVE_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgModbusSlave.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgModbusSlave.uiWrOffset += usWrLen;
            pstCfg->stCfgModbusSlave.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileOPCInfo
*
* 功能描述: 写入OPC配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileOPCInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgOPC.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgOPC.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_OPC_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_OPC_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_OPC_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgOPC.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgOPC.uiWrOffset += usWrLen;
            pstCfg->stCfgOPC.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileCtrlStationInfo
*
* 功能描述: 写入控制站配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileCtrlStationInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgCtrlStation.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgCtrlStation.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_CTRL_STAION_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_CTRL_STAION_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_CTRL_STAION_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgCtrlStation.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgCtrlStation.uiWrOffset += usWrLen;
            pstCfg->stCfgCtrlStation.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileSOEInfo
*
* 功能描述: 写入SOE配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileSOEInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgSOE.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgSOE.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_SOE_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_SOE_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_SOE_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgSOE.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgSOE.uiWrOffset += usWrLen;
            pstCfg->stCfgSOE.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileP2PInfo
*
* 功能描述: 写入P2P配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileP2PInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgP2P.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgP2P.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_P2P_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_P2P_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_P2P_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgP2P.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgP2P.uiWrOffset += usWrLen;
            pstCfg->stCfgP2P.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileVoteInfo
*
* 功能描述: 写入vote配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileVoteInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgVote.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgVote.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_VOTE_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_VOTE_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_VOTE_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgVote.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgVote.uiWrOffset += usWrLen;
            pstCfg->stCfgVote.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileIOModInfo
*
* 功能描述: 写入IO模块配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileIOModInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgIOMod.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgIOMod.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_IO_MODULAR_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_IO_MODULAR_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_IO_MODULAR_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgIOMod.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgIOMod.uiWrOffset += usWrLen;
            pstCfg->stCfgIOMod.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileRetainInfo
*
* 功能描述: 写入Retain配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFileRetainInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgRetain.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgRetain.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_RETAIN_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_RETAIN_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_RETAIN_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgRetain.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgRetain.uiWrOffset += usWrLen;
            pstCfg->stCfgRetain.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFilePrgInfo
*
* 功能描述: 写入PRG配置信息内容
*
* 输入参数: pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入数据长度
*
* 注意事项: 若配置文件缓冲区不可写或写入偏移超范围，则返回值为 0
***********************************************************************************************************************
*/
static uint16_t SysSetCfgFilePrgInfo(puint8_t pucContent, uint16_t usLen)
{
    SysCfgDB_t *pstCfg = NULL;                          /* 用于指向配置文件数据库 */
    uint16_t usWrLen = 0;                               /* 存储实际写入数据长度 */
    uint32_t uiWrOffset = 0;                            /* 写入数据偏移地址 */

    /* 指向配置文件数据库 */
    pstCfg = &s_stSysCfgDB;

    /* 文件可写 */
    if(CFG_FILE_WR_ENABLE == pstCfg->stCfgPrg.bWriteable)
    {
        /* 获取写入偏移 */
        uiWrOffset = pstCfg->stCfgPrg.uiWrOffset;

        /* 判断写入偏移是否超范围 */
        if(uiWrOffset < MAX_CFG_PRG_LEN)
        {
            if((uiWrOffset + (uint32_t)usLen) < MAX_CFG_PRG_LEN)
            {
                /* 剩余空间充足 */
                usWrLen = usLen;
            }
            else
            {
                /* 剩余空间不足 */
                usWrLen = (uint16_t)(MAX_CFG_PRG_LEN - uiWrOffset);
            }

            /* 写入配置文件内容 */
            memcpy((puint8_t)&(pstCfg->stCfgPrg.ucContent[uiWrOffset]), pucContent, usWrLen);

            /* 更新写偏移地址 */
            pstCfg->stCfgPrg.uiWrOffset += usWrLen;
            pstCfg->stCfgPrg.uiLen += usWrLen;
        }
        else
        {
            /* 超出范围 */
            usWrLen = 0;
        }
    }
    else
    {
        /* 不可写 */
        usWrLen = 0;
    }
    return usWrLen;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
