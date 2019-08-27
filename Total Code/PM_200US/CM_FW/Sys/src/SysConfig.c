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
*  文件名称    : SysConfig.c
*
*  功能简介    : 管理配置模块及内部通讯模块间的资源
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-31
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
#include "../inc/CMconfig.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t s_ucCfgFileFinishedFlag;
static RTDataVerInfo_t s_stRTDataVer[NUM_OF_PM];
static ulong64_t s_ulTimestamp[NUM_OF_PM];

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
* 函数名称: SysConfigInit
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
void SysConfigInit(void)
{
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        memset((puint8_t)pstCMConfigInfo, 0, sizeof(CMConfigInfo_t));

        /* added for login test */
        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[0].uiIPAddr = 0xC0A800FA;     /*192.168.0.250*/
        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[0].uiSubnetMask = 0xFFFFFF00; /* 255.255.255.0 */
        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[1].uiIPAddr = 0xC0A800FB;     /*192.168.0.251*/
        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[1].uiSubnetMask = 0xFFFFFF00; /* 255.255.255.0 */

        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[0].bUpdate = false;
        pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[1].bUpdate = false;
    }

    memset((puint8_t)&s_stRTDataVer[0], 0, sizeof(RTDataVerInfo_t)*NUM_OF_PM);
    s_ucCfgFileFinishedFlag = SYS_CLR_RECV_CONFIGINFO_DONE;
    memset(&s_ulTimestamp[0],0,sizeof(ulong64_t)*NUM_OF_PM);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetAccCtrlInfo
*
* 功能描述: 获取访问控制信息
*
* 输入参数: 无
*
* 输出参数: pstAccCtrlInfo: 指向访问控制信息结构体变量的指针
*
* 返 回 值  : 获取访问控制信息成功则返回TRUE
*        获取访问控制信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAccCtrlInfo(AccessCtrlConfigInfo_t *pstAccCtrlInfo)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if(pstAccCtrlInfo != NULL)
        {
            memcpy((puint8_t)pstAccCtrlInfo, \
                    (puint8_t)&(pstCMConfigInfo->stCMSingleInfo[emCMId].stAccessCtrlConfigInfo), \
                    sizeof(AccessCtrlConfigInfo_t));
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetNetworkConfigInfo
*
* 功能描述: 获取本CM的某网络通讯端口的配置信息
*
* 输入参数: emType:指定的网络通讯口
*
* 输出参数: pstNetworkParam: 指向配置信息的结构体变量
*
* 返 回 值  : 获取配置信息成功则返回TRUE
*        获取配置信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetNetworkConfigInfo(NetworkPortType_t emType, NetConfigInfo_t *pstNetworkParam)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((pstNetworkParam != NULL) && (emType < NUM_OF_NETWORK_PORT))
        {
            memcpy((puint8_t)pstNetworkParam, \
                    (puint8_t)&(pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType]), sizeof(NetConfigInfo_t));
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCOMConfigInfo
*
* 功能描述: 获取本CM的某串口的配置信息
*
* 输入参数: emType:指定的串口
*
* 输出参数: pstCOMConfigInfo: 指向配置信息的结构体变量
*
* 返 回 值  : 获取配置信息成功则返回TRUE
*        获取配置信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCOMConfigInfo(SerialPortType_t emType, COMConfigInfo_t *pstCOMConfigInfo)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((pstCOMConfigInfo != NULL) && (emType < NUM_OF_SERIAL_PORT))
        {
            memcpy((puint8_t)pstCOMConfigInfo, \
                    (puint8_t)&(pstCMConfigInfo->stCMSingleInfo[emCMId].stCOMConfigInfo[emType]),sizeof(COMConfigInfo_t));
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetModbusTcpConfigInfo
*
* 功能描述: 获取本CM的某网口的ModbusTCP配置信息
*
* 输入参数: emType:指定的网口
*
* 输出参数: pstModbusTcpConfigInfo: 指向配置信息的结构体变量
*
* 返 回 值  : 获取配置信息成功则返回TRUE
*        获取配置信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetModbusTcpConfigInfo(NetworkPortType_t emType, ModbusTCPConfigInfo_t *pstModbusTcpConfigInfo)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((pstModbusTcpConfigInfo != NULL) && (emType < NUM_OF_NETWORK_PORT))
        {
            memcpy((puint8_t)pstModbusTcpConfigInfo, \
                    (puint8_t)&(pstCMConfigInfo->stCMSingleInfo[emCMId].stModbusTCPConfigInfo[emType]), \
                    sizeof(ModbusTCPConfigInfo_t));
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetNTPTCPConfigInfo
*
* 功能描述: 获取本CM的NTP/TCP的配置信息
*
* 输入参数: emType:指定的串口
*
* 输出参数: pstNTPTCPConfigInfo: 指向配置信息的结构体变量
*
* 返 回 值  : 获取配置信息成功则返回TRUE
*        获取配置信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetNTPTCPConfigInfo(NTPTCPConfigInfo_t *pstNTPTCPConfigInfo)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if(pstNTPTCPConfigInfo != NULL)
        {
            memcpy((puint8_t)pstNTPTCPConfigInfo, \
                    (puint8_t)&(pstCMConfigInfo->stCMSingleInfo[emCMId].stNTPTCPConfigInfo), \
                    sizeof(NTPTCPConfigInfo_t));
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysExterCommGetNetworkConfigInfo
*
* 功能描述: 外部通讯模块获取本CM的某网络通讯端口的配置信息
*
* 输入参数: emType:指定的网络通讯口
*
* 输出参数: pstNetworkParam: 指向配置信息的结构体变量
*
* 返 回 值  : 获取配置信息成功则返回TRUE
*        获取配置信息失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysExterCommGetNetworkConfigInfo(NetworkPortType_t emType, ExtCommNetConfigInfo_t *pstNetworkParam)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((pstNetworkParam != NULL) && (emType < NUM_OF_NETWORK_PORT))
        {
            pstNetworkParam->emActive = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].emActive;
            pstNetworkParam->bUpdate = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].bUpdate;
            pstNetworkParam->uiIPAddr = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].uiIPAddr;
            pstNetworkParam->uiSubnetMask = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].uiSubnetMask;
            pstNetworkParam->uiGateway = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].uiGateway;
            bRet = true;
        }
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetLocalStationID
*
* 功能描述: 获取本控制站ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 版本号
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetLocalStationID(void)
{
    uint32_t uiStaID = 0U;
    CtrlStationConfigInfo_t *pstCtrlCfg = NULL;

    pstCtrlCfg = CMCfgGetCtrlStationInfo();
    if( pstCtrlCfg != NULL )
    {
        uiStaID = pstCtrlCfg->stStationConfigInfo.uiStationID;
    }

    return uiStaID;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataVersion
*
* 功能描述: 获取CRC及实时数据版本
*
* 输入参数: 无
*
* 输出参数: pstRTDataVer: 实时数据版本号
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataVersion(RTDataVerInfo_t *pstRTDataVer)
{
    bool_t bRet = false;

    if(pstRTDataVer != NULL)
    {
        if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMA))
        {
            pstRTDataVer->uiProjID = s_stRTDataVer[PMA].uiProjID;
            pstRTDataVer->uiMainVer = s_stRTDataVer[PMA].uiMainVer;
            pstRTDataVer->uiMinorVer = s_stRTDataVer[PMA].uiMinorVer;
            pstRTDataVer->uiProjCRC = s_stRTDataVer[PMA].uiProjCRC;
            bRet = true;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMB))
        {
            pstRTDataVer->uiProjID = s_stRTDataVer[PMB].uiProjID;
            pstRTDataVer->uiMainVer = s_stRTDataVer[PMB].uiMainVer;
            pstRTDataVer->uiMinorVer = s_stRTDataVer[PMB].uiMinorVer;
            pstRTDataVer->uiProjCRC = s_stRTDataVer[PMB].uiProjCRC;
            bRet = true;
        }
        else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState(PMC))
        {
            pstRTDataVer->uiProjID = s_stRTDataVer[PMC].uiProjID;
            pstRTDataVer->uiMainVer = s_stRTDataVer[PMC].uiMainVer;
            pstRTDataVer->uiMinorVer = s_stRTDataVer[PMC].uiMinorVer;
            pstRTDataVer->uiProjCRC = s_stRTDataVer[PMC].uiProjCRC;
            bRet = true;
        }
        else
        {
            pstRTDataVer->uiProjID = 0;
            pstRTDataVer->uiMainVer = 0;
            pstRTDataVer->uiMinorVer = 0;
            pstRTDataVer->uiProjCRC = 0;
            bRet = true;
        }
        //printf("prj crc=0x%x\n",pstRTDataVer->uiProjCRC);
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetRTDataVersion
*
* 功能描述: 设置实时数据版本
*
* 输入参数: pstRTDataVer 版本信息
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetRTDataVersion(PMController_t emPMID, RTDataVerInfo_t *pstRTDataVer)
{
    bool_t bRet = false;

    if((pstRTDataVer != NULL)&& (emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        s_stRTDataVer[emPMID].uiProjID = pstRTDataVer->uiProjID ;
        s_stRTDataVer[emPMID].uiMainVer = pstRTDataVer->uiMainVer;
        s_stRTDataVer[emPMID].uiMinorVer = pstRTDataVer->uiMinorVer;
        s_stRTDataVer[emPMID].uiProjCRC = pstRTDataVer->uiProjCRC;
        printf("prj crc=0x%x\n",s_stRTDataVer[emPMID].uiProjCRC);
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataTimestamp
*
* 功能描述: 实时数据时间戳
*
* 输入参数: 无
*
* 输出参数: pulTimestamp 时间
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataTimestamp(PMController_t emPMID, ulong64_t *pulTimestamp)
{
    bool_t bRet = false;

    if((pulTimestamp != NULL) && (emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        *pulTimestamp = s_ulTimestamp[emPMID];
        printf("get timestamp=0x%x\n",s_ulTimestamp[emPMID]);
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetRTDataTimestamp
*
* 功能描述: 记录时间戳
*
* 输入参数: ulTimestamp 时间戳
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetRTDataTimestamp(PMController_t emPMID, puint8_t pucTimestamp)
{
    bool_t bRet = false;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM) && (pucTimestamp!= NULL))
    {
        memcpy((puint8_t)&s_ulTimestamp[emPMID],pucTimestamp, sizeof(ulong64_t));
        printf("set timestamp=0x%x\n",*((puint32_t)&pucTimestamp[4]));
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCMConfigVersion
*
* 功能描述: 获取CM配置信息版本
*
* 输入参数: 无
*
* 输出参数: puiCRC: CRC32
*        puiFileVern: 版本号
*
* 返 回 值  : 版本号
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMConfigVersion(puint32_t puiCRC, puint32_t puiFileVern)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((puiCRC != NULL) && (puiFileVern != NULL))
        {
            *puiCRC = pstCMConfigInfo->stCMSingleInfo[emCMId].uiCRC;
            *puiFileVern = pstCMConfigInfo->stCMSingleInfo[emCMId].uiFileVern;
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPassword
*
* 功能描述: 获取连接密码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 版本号
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetPassword(puint8_t pucPwd, puint8_t pucLen)
{
    bool_t bRet = false;
    CtrlStationConfigInfo_t *pstCtrlStaCfgInfo = NULL;

    if((pucPwd != NULL) && (pucLen != NULL))
    {
        pstCtrlStaCfgInfo = CMCfgGetCtrlStationInfo();
        if((pstCtrlStaCfgInfo != NULL) &&
           ((uint8_t)(HAS_BE_CONFIRMED) == pstCtrlStaCfgInfo->ucIsConfigured))
        {
            *pucLen = (uint8_t)pstCtrlStaCfgInfo->stStationConfigInfo.uiObjPasswordLen;
            if((*pucLen >= 0U) && (*pucLen <= (uint8_t)(MAX_PASSWORD_LEN)))
            {
                memcpy( &pucPwd[0], &pstCtrlStaCfgInfo->stStationConfigInfo.ucObjPassword[0], *pucLen );
                bRet = true;
            }
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCMConfigInfoActiveFlag_t
*
* 功能描述: 获取CM配置信息是否更新标识
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 版本号
*
* 注意事项: 无
***********************************************************************************************************************
*/
ActiveFlag_t SysGetCMConfigInfoActiveFlag_t(void)
{
    ActiveFlag_t emActivFlag = NOT_ACTIVE;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        emActivFlag = pstCMConfigInfo->stCMSingleInfo[emCMId].emActive;
    }

    return emActivFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetOPCConfigVersion
*
* 功能描述: 获取OPC配置参数的版本
*
* 输入参数: 无
*
* 输出参数: puiCRC: 用CRC表示主版本
*        puiFileVern: 版本
*
* 返 回 值  : 获取成功则返回TRUE
*        获取失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetOPCConfigVersion(puint32_t puiCRC, puint32_t puiFileVern)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    OPCVarInfo_t* pstOPCVarInfo = NULL;
    pstOPCVarInfo = CMCfgGetOPCVarInfo();
    if(NULL == pstOPCVarInfo)
    {
        /*log*/
    }
    else
    {
        if((puiCRC != NULL) && (puiFileVern != NULL))
        {
            *puiCRC = pstOPCVarInfo->uiCRC;
            *puiFileVern = pstOPCVarInfo->uiFileVern;
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetOPCVarConfigInfo
*
* 功能描述: 获取OPC变量配置信息
*
* 输入参数: uiOffset: 欲读取的内容在整个配置信息中的偏移  uiLength:欲读取的配置信息长度
*
* 输出参数: pucBuff: 存放读取的内容
*
* 返 回 值  : 实际读取的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetOPCVarConfigInfo( puint8_t pucBuff, uint32_t uiOffset, uint32_t uiLength )
{
    uint32_t uiLen = 0;
    OPCVarInfo_t* pstCfg = NULL;
    puint8_t pucCfg = NULL;

    pstCfg = CMCfgGetOPCVarInfo();
    pucCfg = (puint8_t)pstCfg;

    if((pstCfg != NULL) && (pucBuff != NULL))
    {
        if((uiOffset + uiLength) < sizeof(OPCVarInfo_t))
        {
            uiLen = uiLength;
        }
        else
        {
            uiLen = sizeof(OPCVarInfo_t) - uiOffset;
        }
        memcpy(pucBuff, (puint8_t)(&pucCfg[uiOffset]), uiLen);
    }
    return uiLen;
}

/*
***********************************************************************************************************************
* 函数名称: SysOPCGetNetworkConfigInfo
*
* 功能描述: OPC模块获取本CM的指定网口的配置信息
*
* 输入参数: emType: 网口类型
*
* 输出参数: pstNetworkParam: 指向指定网口的配置信息
*
* 返 回 值  : 获取成功则返回TRUE
*        获取失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysOPCGetNetworkConfigInfo(NetworkPortType_t emType, OPCNetConfigInfo_t *pstNetworkParam)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();

    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((emType < NUM_OF_NETWORK_PORT) && (pstNetworkParam != NULL))
        {
            pstNetworkParam->emActive = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].emActive;
            pstNetworkParam->bUpdate = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].bUpdate;
            pstNetworkParam->bPortWriteEnable = pstCMConfigInfo->stCMSingleInfo[emCMId].stNetConfigInfo[emType].bPortWriteEnable;
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysExterCommGetModbusTCPConfigInfo
*
* 功能描述: 外部通讯模块获取本CM的某网口的Modbus TCP配置信息
*
* 输入参数: emType: 网口类型
*
* 输出参数: pstModbusTCPConfig: 指向指定网口的Modbus TCP配置信息
*
* 返 回 值  : 获取成功则返回TRUE
*        获取失败则返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysExterCommGetModbusTCPConfigInfo(NetworkPortType_t emType, ExtCommModbusTCPConfigInfo_t *pstModbusTCPConfig)
{
    bool_t bRet = false;
    CMController_t emCMId = SysGetLocalCMID();
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    pstCMConfigInfo = CMCfgGetCMConfigInfo();

    if(NULL == pstCMConfigInfo)
    {
        /*log*/
    }
    else
    {
        if((emType < NUM_OF_NETWORK_PORT) && (pstModbusTCPConfig != NULL))
        {
            pstModbusTCPConfig->emActive = pstCMConfigInfo->stCMSingleInfo[emCMId].stModbusTCPConfigInfo[emType].emActive;
            pstModbusTCPConfig->bUpdate = pstCMConfigInfo->stCMSingleInfo[emCMId].stModbusTCPConfigInfo[emType].bUpdate;
            pstModbusTCPConfig->ucProtocol = pstCMConfigInfo->stCMSingleInfo[emCMId].stModbusTCPConfigInfo[emType].ucProtocol;
            pstModbusTCPConfig->uiIPAddr = pstCMConfigInfo->stCMSingleInfo[emCMId].stModbusTCPConfigInfo[emType].uiIPAddr;
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRecvConfigInfoFlg
*
* 功能描述: 获取内部通讯接收CM配置信息是否完成标志
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
uint8_t SysGetRecvConfigInfoFlg(void)
{
    return s_ucCfgFileFinishedFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetRecvConfigInfoFlg
*
* 功能描述: 设置CM配置信息是否完成标志
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
void SysSetRecvConfigInfoFlg(uint8_t ucFlag)
{
    s_ucCfgFileFinishedFlag = ucFlag;
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
