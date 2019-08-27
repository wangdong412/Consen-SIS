/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : Client软件应用模块
*
*  文件名称    : Client.c
*
*  功能简介    : 处理Client软件命令和相关配置
*
*  作者        : 王东
*
*  创建日期    : 2016-02-16
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
#include "../inc/Client.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                              /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                          /* 本控制站的ID */
static uint8_t s_ucCurIndex;                            /* 当前Client软件的索引号 */
static ModuleState_t s_emState;                         /* 状态 */
static uint32_t s_uiWaitPMCycleCnt;                     /* 已等待PM应答的拍数 */
static RespModuleType_t s_emResp;                       /* Client命令被哪个模块应答 */
static ClientInfo_t s_stClientInfo[APP_SW_CLIENT_NUM];  /* Client软件信息，数组成员与Client软件一一对应 */
static NetMsg_t s_stReqMsg;                             /* Client软件命令 */
static RespMsg_t s_stRespMsg;                           /* 应答消息 */
static uint8_t s_ucPassword[(MAX_CONN_PWD_LEN)+1U];     /* 连接密码 */
static uint8_t s_ucFWBuffer[MAX_CMFW_LEN];              /* CM固件缓冲区 */
static uint32_t s_uiFWLen;                              /* CM固件的长度-字节数 */
static uint32_t s_uiFWOffsetAddr;                       /* CM固件写入偏移地址值-将被写入的第一个字节 */
static uint32_t s_uiFWFrameNum;                         /* CM固件升级包的包号 */

/* Configuration */
static AccCtrlInfo_t s_stAccCtrlInfo;  /* 访问控制信息 */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );
static void HandleReqTimeout( uint8_t ucSWIndex );
static void HandleDisconnect( uint8_t ucSWIndex );
static void HandleCmd( void );
static void HandleResponse( void );

/* Initialize */
static void InitLocalConfigInfo( void );
static void ClearClientLocalInfo( uint8_t ucSWIndex );
static void ClearClientSysInfo( uint8_t ucSWIndex );

/* Configuration */
static void ClearLocalConfig( void );
static void UpdateAccessAuth( ClientInfo_t *pstSWInfo );

/* Handle Command */
static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, ClientInfo_t *pstSWInfo );
static bool_t CheckPMHandle( uint8_t const pucReq[]);
static void PMHandle( ClientInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg );
static SelPMID_t SelectPM( ClientInfo_t *pstSWInfo, uint8_t const pucReq[]);

/* Decode Command */
static void DecodeCmd( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogin( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogout( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeReadDiag( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeReadVern( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeFWUPStart( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeFWUPCont( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeFWUPEnd( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );

/* Response */
static bool_t MakeRespMsg( uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp );
static uint16_t AddDiagData( uint8_t const pucReq[], uint8_t pucRespData[]);
static uint16_t AddVernData( uint8_t const pucReq[], uint8_t pucRespData[]);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ClientInit
*
* 功能描述: 模块初始化函数
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

void ClientInit( void )
{
    DEBUG_CLI( "ClientInit\n" );

    uint8_t ucSWIndex = 0U;
    s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucCurIndex = 0U;
    s_emState = IDLE_STATE;
    s_uiWaitPMCycleCnt = 0U;
    s_emResp = NONE_RESP;

    /* Local Client Software Info. */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM); ucSWIndex++ )
    {
        ClearClientLocalInfo( ucSWIndex );
    }

    /* Client Software Command */
    memset( &s_stReqMsg, 0, sizeof(NetMsg_t));
    s_stReqMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Response Message Buffer */
    memset( &s_stRespMsg, 0, sizeof(RespMsg_t));

    /* Connect Password */
    memset( s_ucPassword, 0, sizeof(s_ucPassword));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* CM Firmware */
    memset( s_ucFWBuffer, 0, sizeof(s_ucFWBuffer));
    s_uiFWLen = 0U;
    s_uiFWOffsetAddr = 0U;
    s_uiFWFrameNum = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClientCycle
*
* 功能描述: 模块周期运行函数
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

void ClientCycle( void )
{
    uint8_t ucSWIndex = 0U;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Update Local Station ID */
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();

    /* Handle Configuration */
    if( s_bCMHasCfg )
    {
        HandleConfiguration();
    }
    else
    {
        ClearLocalConfig();
    }

    /* Handle Software Timeout and Disconnect */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM); ucSWIndex++ )
    {
        HandleReqTimeout( ucSWIndex );  /* Handle Software Timeout */
        HandleDisconnect( ucSWIndex );  /* Handle Software Disconnect */
    }

    if( IDLE_STATE == s_emState )
    {
        HandleCmd();
    }
    else
    {
        HandleResponse();
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
* 函数名称: ClearClientLocalInfo
*
* 功能描述: 清除指定Client软件的本地信息
*
* 输入参数: ucSWIndex：软件索引值
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearClientLocalInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM))
    {
        memset( &s_stClientInfo[ucSWIndex], 0, sizeof(ClientInfo_t));

        s_stClientInfo[ucSWIndex].uiIPAddr = 0U;
        s_stClientInfo[ucSWIndex].usPort = 0U;
        s_stClientInfo[ucSWIndex].emNetwork = INVALID_NETWORK_PORT;
        s_stClientInfo[ucSWIndex].emLog = LOGOUT;
        s_stClientInfo[ucSWIndex].ucAccAuth = (uint8_t)(AL_ALL);
        s_stClientInfo[ucSWIndex].emSelPMID = SELPM_NONE;
        s_stClientInfo[ucSWIndex].uiWaitReqCycleCnt = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearClientSysInfo
*
* 功能描述: 清除指定Client软件的系统资源信息
*
* 输入参数: ucSWIndex：软件索引值
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearClientSysInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM))
    {
        /* Reset Lock Flag */
        if( !SysResetAppSwLockedFlag( APP_CLIENT, ucSWIndex ))
        {
            DEBUG_CLI( "Reset Client-%d Locked Flag Failed\n", ucSWIndex );
        }

        /* Reset Connect Flag */
        if( !SysResetAppSwConnectFlag( APP_CLIENT, ucSWIndex ))
        {
            DEBUG_CLI( "Reset Client-%d Connect Flag Failed\n", ucSWIndex );
        }

        /* Set Clear Flag */
        if( !SysSetAppSwClearFlag( APP_CLIENT, ucSWIndex ))
        {
            DEBUG_CLI( "Set Client-%d Clear Flag Failed\n", ucSWIndex );
        }

        /* Clear Request Msg */
        SysClearAppMsg( APP_CLIENT, ucSWIndex, APP_MSG_REQ );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitLocalConfigInfo
*
* 功能描述: 初始化本地保存的配置信息
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

static void InitLocalConfigInfo( void )
{
    /* Access Control Info. */
    InitAccessCtrlInfo( &s_stAccCtrlInfo );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleConfiguration
*
* 功能描述: 处理配置信息
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

static void HandleConfiguration( void )
{
    /* Access Control Info. */
    uint8_t ucSWIndex = 0U;

    if( HandleAccessCtrlInfo( &s_stAccCtrlInfo ))
    {
        DEBUG_CLI( "Access Control Cfg Switch\n" );
        for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM); ucSWIndex++ )
        {
            UpdateAccessAuth( &s_stClientInfo[ucSWIndex]);
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalConfig
*
* 功能描述: 清除本地配置信息
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

static void ClearLocalConfig( void )
{
    uint8_t ucSWIndex = 0U;

    InitLocalConfigInfo();

    /* Update Access Authority */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_AMS_NUM); ucSWIndex++ )
    {
        UpdateAccessAuth( &s_stClientInfo[ucSWIndex]);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqTimeout
*
* 功能描述: 当软件处于连接状态且超时时，清除软件相关信息
*
* 输入参数: ucSWIndex：软件索引值
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqTimeout( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM))
    {
        if( SysGetAppSwConnectFlag( APP_CLIENT, ucSWIndex))  /* Connect */
        {
            s_stClientInfo[ucSWIndex].uiWaitReqCycleCnt++;

            /* Connect & Timeout: Clear */
            if( s_stClientInfo[ucSWIndex].uiWaitReqCycleCnt >= (uint32_t)(CLIENT_SW_TIMEOUT_CYCLE_CNT))
            {
                DEBUG_CLI( "Client-%d Timeout\n", ucSWIndex );

                /* Clear Local Info */
                ClearClientLocalInfo( ucSWIndex );

                /* Clear System Info */
                ClearClientSysInfo( ucSWIndex );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleDisconnect
*
* 功能描述: 软件断开连接且未处于锁定状态时，清除本地保存的软件信息
*
* 输入参数: ucSWIndex：软件索引值
*
* 输出参数: 无
*
* 返 回 值: 处理是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleDisconnect( uint8_t ucSWIndex )
{
    bool_t bClear = false;

    if( ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM))
    {
        if( !SysGetAppSwConnectFlag( APP_CLIENT, ucSWIndex ))  /* Disconnect */
        {
            if( s_stClientInfo[ucSWIndex].uiIPAddr != 0U )
            {
                DEBUG_CLI( "Client-%d Disconnected\n", ucSWIndex );

                /* Connect -> Disconnect & Non-Lock: Clear */
                if( ucSWIndex == s_ucCurIndex )
                {
                    /* Current Software */
                    if( IDLE_STATE == s_emState )
                    {
                        bClear = true;
                    }
                    else
                    {
                        bClear = false;
                    }
                }
                else
                {
                    bClear = true;
                }

                if( bClear )
                {
                    ClearClientLocalInfo( ucSWIndex );  /* Clear Local Info */
                }
            }
        }/* end of if Disconnect */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleCmd
*
* 功能描述: 处理软件命令
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

static void HandleCmd( void )
{
    uint8_t ucCheckCnt = 0U;
    uint16_t usLen = 0U;
    uint8_t ucSubReason = 0U;
    s_stRespMsg.usLen = 0U;
    s_emResp = NONE_RESP;

    while( ucCheckCnt < (uint8_t)(APP_SW_CLIENT_NUM))
    {
        /* Read Request Message */
        s_stReqMsg.stNetHead.usLen = 0U;
        usLen = (uint16_t)sizeof( NetMsg_t );
        if( !SysGetAppMsg( APP_CLIENT, s_ucCurIndex, APP_MSG_REQ, (puint8_t)&s_stReqMsg, &usLen ))
        {
            usLen = 0U;
        }

        if((usLen > 0U) && (s_stReqMsg.stNetHead.usLen > 0U))
        {
            /* Has Message */
            s_stClientInfo[s_ucCurIndex].uiWaitReqCycleCnt = 0U;  /* Clear wait request count */
            ucSubReason = CheckCmd( &s_stReqMsg, &s_stClientInfo[s_ucCurIndex] );
            if((uint8_t)(ERR_SUCCESS) == ucSubReason )
            {
                /* Valid */
                DEBUG_CLI( "HandleCmd: Msg Valid\n" );
                if( CheckPMHandle( s_stReqMsg.ucBuff ))
                {
                    /* PM Handle */
                    DEBUG_CLI( "HandleCmd: PM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));
                    PMHandle( &s_stClientInfo[s_ucCurIndex], &s_stReqMsg, &s_stRespMsg, s_bCMHasCfg );
                }
                else
                {
                    /* CM Handle */
                    DEBUG_CLI( "HandleCmd: CM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));

                    /* Decode Cmd */
                    DecodeCmd( &s_stClientInfo[s_ucCurIndex], s_stReqMsg.ucBuff, &s_stRespMsg, s_bCMHasCfg );
                    s_emResp = CM_RESP;
                }
            }
            else
            {
                /* Invalid */
                DEBUG_CLI( "HandleCmd: Msg Invalid\n" );

                /* Make Response Message */
                if( !MakeRespMsg( ucSubReason, s_stReqMsg.ucBuff, &s_stRespMsg ))
                {
                    DEBUG_CLI( "MakeRespMsg Err\n" );
                }

                s_emResp = CM_RESP;
            }

            /* Lock & Busy */
            if( !SysSetAppSwLockedFlag( APP_CLIENT, s_ucCurIndex ))
            {
                DEBUG_CLI( "HandleCmd: Set Locked Flag Failed\n" );
            }

            s_emState = BUSY_STATE;
            break;
        }
        else
        {
            /* No Message */
            s_ucCurIndex++;
            s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_CLIENT_NUM);
            ucCheckCnt++;
        }
    }/*while*/

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCmd
*
* 功能描述: 检查软件命令是否有效
*
* 输入参数: pstReqMsg：指向待检查的网络消息；
*          pstSWInfo：指向相应软件的本地信息。
*
* 输出参数: 无
*
* 返 回 值: 第二级响应码，0表示有效；非零表示具体失败原因
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, ClientInfo_t *pstSWInfo )
{
    uint8_t ucSubReason = (uint8_t)(ERR_PARA);  /* Second Level Response Code */
    uint16_t usAType = 0U;                      /* Actual Command Type */
    uint8_t const *pucReq = NULL;

    if((pstReqMsg != NULL)  &&
       (pstSWInfo != NULL))
    {
        ucSubReason = (uint8_t)(ERR_SUCCESS);

        /* Check Source Address: IP, Port and Network */
        if( pstSWInfo->uiIPAddr != 0U )
        {
            if((pstSWInfo->uiIPAddr != pstReqMsg->stNetHead.uiIPAddr) ||
               (pstSWInfo->usPort != pstReqMsg->stNetHead.usPort) ||
               (pstSWInfo->emNetwork != pstReqMsg->stNetHead.emNetwork))
            {
                DEBUG_CLI( "CheckCmd: SrcAddr Invalid\n" );
                ucSubReason = (uint8_t)(ERR_SRC_ADDR);
            }
        }
        else
        {
            /* Update Local Software Information */
            pstSWInfo->uiIPAddr = pstReqMsg->stNetHead.uiIPAddr;
            pstSWInfo->usPort = pstReqMsg->stNetHead.usPort;
            pstSWInfo->emNetwork = pstReqMsg->stNetHead.emNetwork;
        }

        /* Check header if source address valid */
        if((uint8_t)(ERR_SUCCESS) == ucSubReason )
        {
            pucReq = pstReqMsg->ucBuff;
            if( pucReq[APP_REQ_TYPE_INDEX] != (uint8_t)(APP_REQ_TYPE))
            {
                DEBUG_CLI( "CheckCmd: Type-0x%X != 0x%X\n", pucReq[APP_REQ_TYPE_INDEX], (uint8_t)(APP_REQ_TYPE));
                ucSubReason = (uint8_t)(ERR_TYPE);  /* Type */
            }
            else if( pucReq[APP_REQ_PROTECT_INDEX] != (uint8_t)(APP_REQ_PROTECT))
            {
                DEBUG_CLI( "CheckCmd: Protect-%d Invalid\n", pucReq[APP_REQ_PROTECT_INDEX]);
                ucSubReason = (uint8_t)(ERR_PROTECT);  /* Protect */
            }
            else
            {
                /* Actual Type */
                usAType = GetReqActualType( pucReq );
                if((usAType < (uint16_t)(CLI_ATYPE_MIN)) ||
                   (usAType > (uint16_t)(CLI_ATYPE_MAX)))
                {
                    DEBUG_CLI( "CheckCmd: Actual Type-%d, HI-%d, LOW-%d Invalid\n", \
                               usAType, pucReq[APP_REQ_ATYPE_HIINDEX], pucReq[APP_REQ_ATYPE_LOWINDEX]);
                    ucSubReason = (uint8_t)(ERR_ACTUAL_TYPE);
                }
                /*else if( pucReq[APP_RESP_STAID_INDEX] != s_ucLocalStaID )
                {
                    DEBUG_CLI( "CheckCmd: StaID-%d Invalid\n", pucReq[APP_RESP_STAID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_STAID);
                }*/
                else if( pucReq[APP_REQ_PMID_INDEX] != (uint8_t)(APP_REQ_PMID))
                {
                    DEBUG_CLI( "CheckCmd: PMID-%d Invalid\n", pucReq[APP_REQ_PMID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_PMID);  /* PM ID */
                }
                else if( pucReq[APP_REQ_LAST_INDEX] != (uint8_t)(APP_REQ_LAST_FLAG))
                {
                    DEBUG_CLI( "CheckCmd: Last Flag-%d Invalid\n", pucReq[APP_REQ_LAST_INDEX]);
                    ucSubReason = (uint8_t)(ERR_LAST_FLAG);   /* Last Flag */
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
            }
        }
    }

    return ucSubReason;
}

/*
***********************************************************************************************************************
* 函数名称: CheckPMHandle
*
* 功能描述: 命令有效时，调用此函数 - 检查命令是否应由PM模块处理
*
* 输入参数: pucReq：指向请求消息
*
* 输出参数: 无
*
* 返 回 值: true：PM模块处理；false：CM模块处理
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckPMHandle( uint8_t const pucReq[])
{
    bool_t bPMHandle = false;  /* PM Handle Flag */
    uint16_t usAType = 0U;     /* Actual Type Of The Request Message */
    FWInfo_t const *pstFWInfo = NULL;

    if( pucReq != NULL )
    {
        usAType = GetReqActualType( pucReq );
        if(((uint16_t)(CLI_LOGIN) == usAType) ||
           ((uint16_t)(CLI_LOGOUT) == usAType) ||
           ((uint16_t)(CLI_READ_DIAG) == usAType) ||
           ((uint16_t)(CLI_READ_VERN) == usAType))
        {
            bPMHandle = false;
        }
        else if((usAType >= (uint16_t)(CLI_FWUP_START)) && (usAType <= (uint16_t)(CLI_FWUP_END)))
        {
            pstFWInfo = (FWInfo_t const *)&pucReq[APP_MSG_HEADER_LEN];
            if((uint8_t)(MT_CM) == pstFWInfo->ucModuleType )
            {
                bPMHandle = false;
            }
            else
            {
                bPMHandle = true;
            }
        }
        else
        {
            bPMHandle = true;
        }
    }

    return bPMHandle;
}

/*
***********************************************************************************************************************
* 函数名称: MakeRespMsg
*
* 功能描述: 构造应答帧
*
* 输入参数: ucSubReason：第二级响应码；
*          pucReq：请求帧；
*          pstResp：用于存放构造的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t MakeRespMsg( uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    bool_t bResult = false;
    uint16_t usDataLen = 0U;
    puint8_t pucResp = NULL;
    uint16_t usLength = 0U;   /* Length Of Header + Data Area */
    uint16_t usAType = 0U;    /* Actual Request Message Type */
    uint16_t usIndex = 0U;
    uint32_t uiCalcCRC = 0U;

    if((pucReq != NULL) &&
       (pstResp != NULL))
    {
        bResult = true;
        pstResp->usLen = 0U;        /* Reset Length */
        pucResp = pstResp->ucBuff;  /* Buffer */
        usAType = GetReqActualType( pucReq );

        /* Header */
        pucResp[APP_RESP_TYPE_INDEX] = (uint8_t)(APP_RESP_TYPE);
        pucResp[APP_RESP_PROTECT_INDEX] = (uint8_t)(APP_RESP_PROTECT);
        pucResp[APP_RESP_ATYPE_HIINDEX] = pucReq[APP_REQ_ATYPE_HIINDEX];
        pucResp[APP_RESP_ATYPE_LOWINDEX] = pucReq[APP_REQ_ATYPE_LOWINDEX];
        pucResp[APP_RESP_STAID_INDEX] = s_ucLocalStaID;
        pucResp[APP_RESP_SQ_HIINDEX] = pucReq[APP_REQ_SQ_HIINDEX];
        pucResp[APP_RESP_SQ_LOWINDEX] = pucReq[APP_REQ_SQ_LOWINDEX];
        pucResp[APP_RESP_FLAG_INDEX] = (uint8_t)(APP_RESP_FLAG);
        pucResp[APP_RESP_SUBREA_INDEX] = ucSubReason;

        /* Header & Data Area: usLength */
        if( ucSubReason != (uint8_t)(ERR_SUCCESS))
        {
            /* Invalid Request Message */
            pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_FAILED);
            pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
            usLength = (uint16_t)(APP_MSG_HEADER_LEN);
        }
        else
        {
            /* Valid Request Message */
            pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_SUCCESS);

            if(((uint16_t)(CLI_LOGIN) == usAType) ||
               ((uint16_t)(CLI_LOGOUT) == usAType) ||
               ((usAType >= (uint16_t)(CLI_FWUP_START)) && (usAType <= (uint16_t)(CLI_FWUP_END))))
            {
                /* No Data Area */
                pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
                usLength = (uint16_t)(APP_MSG_HEADER_LEN);
            }
            else
            {
                /* Has Data Area */
                pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_EXIST);
                usIndex = (uint16_t)(APP_MSG_HEADER_LEN);  /* Data Area Index */

                if((uint16_t)(CLI_READ_DIAG) == usAType )
                {
                    usDataLen = AddDiagData( &pucReq[0], &pucResp[usIndex]);
                    if( usDataLen > 0U )
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ADD_DIAG);
                        DEBUG_CLI( "AddDiagData Failed\n" );
                    }
                }
                else
                {
                    usDataLen = AddVernData( &pucReq[0], &pucResp[usIndex]);
                    if( usDataLen > 0U )
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ADD_VERN);
                        DEBUG_CLI( "AddVernData Failed\n" );
                    }
                }

                if( ucSubReason != (uint8_t)(ERR_SUCCESS))
                {
                    pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_FAILED);
                    pucResp[APP_RESP_SUBREA_INDEX] = ucSubReason;
                    pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
                    usLength = (uint16_t)(APP_MSG_HEADER_LEN);
                }
            }
        }

        /* Length: Header + Data Area */
        pucResp[APP_RESP_LEN_HIINDEX] = (uint8_t)((usLength >> 8) & 0xFF );
        pucResp[APP_RESP_LEN_LOWINDEX] = (uint8_t)( usLength & 0xFF );

        /* CRC */
        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucResp, (uint32_t)usLength );

        /* Write CRC Area: High First */
        usIndex = usLength;  /* CRC Index */
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 24) & 0xFF );  /* Highest Byte */
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 16) & 0xFF );
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 8)  & 0xFF );
        pucResp[usIndex++] = (uint8_t)( uiCalcCRC & 0xFF );

        /* Update Response Message Length */
        pstResp->usLen = usLength + (uint16_t)(APP_MSG_CRC_LEN);
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: AddDiagData
*
* 功能描述: 根据指定的PMID，添加诊断信息
*
* 输入参数: pucReq：请求帧首地址；
*          pucRespData：应答帧数据域首字节地址。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddDiagData( uint8_t const pucReq[], uint8_t pucRespData[])
{
    uint16_t usDataLen = 0U;
    uint8_t ucIndex = 0U;
    uint8_t ucSecCnt = 0U;  /* Section Counts */
    uint8_t ucBlkCnt = 0U;  /* Block Counts */
    uint32_t uiDataIndex = 0U;  /* Response Data Index */
    ReadReq_t const *pstRDReq = NULL;
    SharedAppend_t const *pstAppend = NULL;

    if((pucReq != NULL) && (pucRespData != NULL))
    {
        pstRDReq = (ReadReq_t const *)&pucReq[APP_MSG_HEADER_LEN];
        if( pstRDReq->ucPMID < (uint8_t)(NUM_OF_PM))
        {
            pstAppend = (SharedAppend_t const *)SysGetAppendInfoAddr((PMController_t)pstRDReq->ucPMID );
            if( pstAppend != NULL )
            {
                uiDataIndex = 1U;  /* First Section Data */

                /* PM */
                if( pstAppend->stPMState.ucActive )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_PM);  /* Section No. */
                    pucRespData[uiDataIndex++] = 1U;                  /* numberOfBlocks */

                    /* Block Data */
                    memcpy( &pucRespData[uiDataIndex], &pstAppend->stPMState.stPMFirmwareState, \
                            sizeof(SharedPMFirmwareState_t));
                    uiDataIndex += sizeof(SharedPMFirmwareState_t);
                }

                /* CM */
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
                {
                    if( pstAppend->stCMState[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_CM);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
                    {
                        if( pstAppend->stCMState[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stCMState[ucIndex].stCMFirmwareState, \
                                    sizeof(SharedCMFirmwareState_t));
                            uiDataIndex += sizeof(SharedCMFirmwareState_t);
                        }
                    }
                }

                /* BI */
#if 0
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_BI_NUM); ucIndex++ )
                {
                    if( pstAppend->stBIState[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_BI);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_BI_NUM); ucIndex++ )
                    {
                        if( pstAppend->stBIState[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stBIState[ucIndex].stBIDiag, \
                                    sizeof(SharedBIDiag_t));
                            uiDataIndex += sizeof(SharedBIDiag_t);
                        }
                    }
                }
#endif

                /* IO */
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_SLAVE_NUM); ucIndex++ )
                {
                    if( pstAppend->stSlaveState[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_IO);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_SLAVE_NUM); ucIndex++ )
                    {
                        if( pstAppend->stSlaveState[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stSlaveState[ucIndex].stSlaveDiag, \
                                    sizeof(SharedSlaveDiag_t));
                            uiDataIndex += sizeof(SharedSlaveDiag_t);
                        }
                    }
                }

                /* Number Of Sections */
                pucRespData[0] = ucSecCnt;

                /* Set Data Length */
                usDataLen = uiDataIndex;
            }
        }
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: AddVernData
*
* 功能描述: 根据指定的PMID，添加版本信息
*
* 输入参数: pucReq：请求帧首地址；
*          pucRespData：应答帧数据域首字节地址。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddVernData( uint8_t const pucReq[], uint8_t pucRespData[])
{
    uint16_t usDataLen = 0U;
    uint8_t ucIndex = 0U;
    uint8_t ucSecCnt = 0U;  /* Section Counts */
    uint8_t ucBlkCnt = 0U;  /* Block Counts */
    uint32_t uiDataIndex = 0U;  /* Response Data Index */
    ReadReq_t const *pstRDReq = NULL;
    SharedAppend_t const *pstAppend = NULL;

    if((pucReq != NULL) && (pucRespData != NULL))
    {
        pstRDReq = (ReadReq_t const *)&pucReq[APP_MSG_HEADER_LEN];
        if( pstRDReq->ucPMID < (uint8_t)(NUM_OF_PM))
        {
            pstAppend = (SharedAppend_t const *)SysGetAppendInfoAddr((PMController_t)pstRDReq->ucPMID );
            if( pstAppend != NULL )
            {
                uiDataIndex = 1U;  /* First Section Data */

                /* PM */
                if( pstAppend->stPMVern.ucActive )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_PM);  /* Section No. */
                    pucRespData[uiDataIndex++] = 1U;                  /* numberOfBlocks */

                    /* Block Data */
                    memcpy( &pucRespData[uiDataIndex], &pstAppend->stPMVern.stPMFirmwareVern, \
                            sizeof(SharedPMFirmwareVern_t));
                    uiDataIndex += sizeof(SharedPMFirmwareVern_t);
                }

                /* CM */
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
                {
                    if( pstAppend->stCMVern[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_CM);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_CM_NUM); ucIndex++ )
                    {
                        if( pstAppend->stCMVern[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stCMVern[ucIndex].stCMFirmwareVern, \
                                    sizeof(SharedCMFirmwareVern_t));
                            uiDataIndex += sizeof(SharedCMFirmwareVern_t);
                        }
                    }
                }

                /* BI */
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_BI_NUM); ucIndex++ )
                {
                    if( pstAppend->stBIVern[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_BI);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_BI_NUM); ucIndex++ )
                    {
                        if( pstAppend->stBIVern[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stBIVern[ucIndex].stBIFirmwareVern, \
                                    sizeof(SharedBIFirmwareVern_t));
                            uiDataIndex += sizeof(SharedBIFirmwareVern_t);
                        }
                    }
                }

                /* IO */
                ucBlkCnt = 0U;
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_SLAVE_NUM); ucIndex++ )
                {
                    if( pstAppend->stSlaveVern[ucIndex].ucActive )
                    {
                        ucBlkCnt++;
                    }
                }

                if( ucBlkCnt > 0U )
                {
                    ucSecCnt++;
                    pucRespData[uiDataIndex++] = (uint8_t)(ISID_IO);  /* Section No. */
                    pucRespData[uiDataIndex++] = ucBlkCnt;            /* numberOfBlocks */

                    /* Block Data */
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_SLAVE_NUM); ucIndex++ )
                    {
                        if( pstAppend->stSlaveVern[ucIndex].ucActive )
                        {
                            memcpy( &pucRespData[uiDataIndex], &pstAppend->stSlaveVern[ucIndex].stSlaveFirmwareVern, \
                                    sizeof(SharedSlaveFirmwareVern_t));
                            uiDataIndex += sizeof(SharedSlaveFirmwareVern_t);
                        }
                    }
                }

                /* Number Of Sections */
                pucRespData[0] = ucSecCnt;

                /* Set Data Length */
                usDataLen = uiDataIndex;
            }
        }
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: PMHandle
*
* 功能描述: 命令需由PM处理时，判断登录状态和访问权限，并根据判断结果进行相应操作。
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pstReqMsg：指向待检查的网络消息；
*          pstResp: 用于存放构造的应答帧；
*          bCMHasCfg：CM是否有配置标志位。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void PMHandle( ClientInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    SelPMID_t emSelPMID = SELPM_NONE;
    uint16_t usAType = 0U;  /* Actual Request Message Type */
    uint16_t usIndex = 0U;

#ifdef DEBUG_SWCLI
    /* Only for print message */
    uint16_t usPerLen = 0U;
    uint16_t usPrintLen = 0U;
#endif

    if(( pstSWInfo != NULL ) &&
       ( pstReqMsg != NULL ) &&
       ( pstResp != NULL ))
    {
        if( pstSWInfo->emLog != LOGIN )
        {
            DEBUG_CLI( "PMHandle: Client should login first\n" );
            ucSubReason = (uint8_t)(ERR_LOG);  /* Need To Login First */
        }
        else
        {
            if( bCMHasCfg )
            {
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_CLI( "PMHandle: Access - Deny\n" );
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
                }
                else if((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth )
                {
                    usAType = GetReqActualType( pstReqMsg->ucBuff );
                    if(((uint8_t)(CLI_CLR_DIAG) == usAType) ||
                       ((usAType >= (uint16_t)(CLI_FWUP_START)) && (usAType <= (uint16_t)(CLI_FWUP_END))))
                    {
                        DEBUG_CLI( "PMHandle: Access - Readonly\n" );
                        ucSubReason = (uint8_t)(ERR_ACCESS);
                    }
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_CMNOCFG);
            }
        }

        if((uint8_t)(ERR_SUCCESS) == ucSubReason )
        {
            /* Login & Has Access Authority: Select a PM Module */
            emSelPMID = SelectPM( pstSWInfo, pstReqMsg->ucBuff );
            if( SELPM_NONE == emSelPMID )
            {
                DEBUG_CLI( "PMHandle: CM BUS Invalid\n" );
                ucSubReason = (uint8_t)(ERR_CMBUS);  /* CM BUS Error */
            }
            else
            {
                /* Write Req To The Selected PM */
                if( SysSetMsg( SwitchPMID( emSelPMID ), CLIENT_REQ, pstReqMsg->ucBuff, pstReqMsg->stNetHead.usLen ))
                {
                    /* PM Response */
#ifdef DEBUG_SWCLI
                    /* Print Request Message */
                    {
                        printf( "PMHandle: MsgLen: %d, Data(Max Top 50B): \n", pstReqMsg->stNetHead.usLen );
                        usPrintLen = pstReqMsg->stNetHead.usLen;
                        if( usPrintLen > 50U )
                        {
                            usPrintLen = 50U;
                        }

                        for( usIndex = 0U; usIndex < usPrintLen; usIndex++ )
                        {
                            printf( "%02X ", pstReqMsg->ucBuff[usIndex]);
                            usPerLen++;

                            if(( 30 == usPerLen ) ||
                               ((usPrintLen-1) == usIndex))
                            {
                                printf( "\n" );
                                usPerLen = 0U;
                            }
                        }
                    }
#endif
                    DEBUG_CLI( "PMHandle: PM Resp\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                    s_emResp = PM_RESP;
                    s_uiWaitPMCycleCnt = 0U;
                }
                else
                {
                    DEBUG_CLI( "PMHandle: SysSetMsg Failed, SelPMID-%d\n", emSelPMID );
                    ucSubReason = (uint8_t)(ERR_SYSWREQTOPM);  /* 将请求写入系统资源失败 - 用于发往PM模块 */
                }
            }
        }

        if( ucSubReason != (uint8_t)(ERR_SUCCESS))
        {
            /* CM Response */
            DEBUG_CLI( "PMHandle: CM Response\n" );

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pstReqMsg->ucBuff, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Error\n" );
            }

            s_emResp = CM_RESP;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SelectPM
*
* 功能描述: 根据PM选取机制选取某一PM模块
*
* 输入参数：pstSWInfo：Client软件本地信息；
*          pucReq：请求帧首地址。
*
* 输出参数: 无
*
* 返 回 值: 选取的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static SelPMID_t SelectPM( ClientInfo_t *pstSWInfo, uint8_t const pucReq[])
{
    SelPMID_t emSelPMID = SELPM_NONE;
    SelPMMethod_t emSelMethod = SPM_INVALID;
    PMController_t emSpeyPMID = PMA;
    FWInfo_t const *pstFWInfo = NULL;
    uint16_t usIndex = (uint16_t)(APP_MSG_HEADER_LEN);
    uint16_t usAType = 0U;  /* Actual Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL))
    {
        /* 客户端软件明确指定PM编号时，选用软件指定的PM模块； */
        /* 由CM选取时，选用号小的PM模块 */
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_READFILE_START) == usAType )
        {
            emSelMethod = SPM_RESEL;
        }
        else if((uint16_t)(CLI_READFILE_CONT) == usAType )
        {
            emSelMethod = SPM_KEEP;
        }
        else if(((uint16_t)(CLI_READ_EVENT) == usAType) ||
                ((uint16_t)(CLI_CLR_DIAG) == usAType))
        {
            emSelMethod = SPM_SPEY;
            emSpeyPMID = (PMController_t)pucReq[usIndex];
        }
        else if((usAType >= (uint16_t)(CLI_FWUP_START)) && (usAType <= (uint16_t)(CLI_FWUP_END)))
        {
            pstFWInfo = (FWInfo_t const *)&pucReq[usIndex];
            if((uint8_t)MT_PM == pstFWInfo->ucModuleType )
            {
                emSelMethod = SPM_SPEY;
                emSpeyPMID = (PMController_t)pstFWInfo->ucPMID;
            }
            else if((uint8_t)MT_IO == pstFWInfo->ucModuleType )
            {
                if((uint16_t)(CLI_FWUP_START) == usAType )
                {
                    emSelMethod = SPM_RESEL;
                }
                else
                {
                    emSelMethod = SPM_KEEP;
                }
            }
            else
            {
                emSelMethod = SPM_INVALID;
            }
        }
        else
        {
            emSelMethod = SPM_INVALID;
        }

        if( SPM_RESEL == emSelMethod )
        {
            /* CM Select */
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
            {
                emSelPMID = SELPM_A;
            }
            else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
            {
                emSelPMID = SELPM_B;
            }
            else if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
            {
                emSelPMID = SELPM_C;
            }
            else
            {
                emSelPMID = SELPM_NONE;
            }
        }
        else if( SPM_KEEP == emSelMethod )
        {
            /* Keep Pre-Select Result */
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( pstSWInfo->emSelPMID ))
            {
                emSelPMID = pstSWInfo->emSelPMID;
            }
            else
            {
                emSelPMID = SELPM_NONE;
            }
        }
        else if( SPM_SPEY == emSelMethod )
        {
            /* Specify */
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( emSpeyPMID ))
            {
                emSelPMID = emSpeyPMID;
            }
            else
            {
                emSelPMID = SELPM_NONE;
            }
        }
        else
        {
            emSelPMID = SELPM_NONE;
        }

        pstSWInfo->emSelPMID = emSelPMID;
    }

    return emSelPMID;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateAccessAuth
*
* 功能描述: 更新相应Client软件的访问权限
*
* 输入参数：pstSWInfo：指向相应软件的本地信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void UpdateAccessAuth( ClientInfo_t *pstSWInfo )
{
    AccessCtrlConfigInfo_t *pstCurAcc = NULL;
    AccessCtrlConfig_t *pstAccItem = NULL;
    uint8_t ucItemNum = 0U;
    uint8_t ucIndex = 0U;

    if( pstSWInfo != NULL )
    {
        if((pstSWInfo->uiIPAddr != 0U) &&
           ((pstSWInfo->emNetwork >= NET1) && (pstSWInfo->emNetwork < NUM_OF_NETWORK_PORT)))
        {
            if( s_stAccCtrlInfo.ucIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
            {
                pstCurAcc = &( s_stAccCtrlInfo.stAccCtrl[s_stAccCtrlInfo.ucIndex]);
                ucItemNum = pstCurAcc->stAccessCtrlHead.ucAccessCtrlNum;
                if((ucItemNum > 0U) && (ucItemNum <= (uint8_t)(MAX_ACCESS_CTRL_NUM)))
                {
                    /* Enable: Loop Access Control Table */
                    pstSWInfo->ucAccAuth = (uint8_t)(AL_DENY);
                    for( ucIndex = 0U; ucIndex < ucItemNum; ucIndex++ )
                    {
                        pstAccItem = &( pstCurAcc->stAccessCtrlConfig[ucIndex]);

                        /* Check Network & IP */
                        /* Network */
                        if( NET1 == pstSWInfo->emNetwork )
                        {
                            if(((uint8_t)(NW_NET1) == pstAccItem->ucNetwork) ||
                               ((uint8_t)(NW_NET_ALL) == pstAccItem->ucNetwork))
                            {
                                /* IP */
                                if((pstSWInfo->uiIPAddr & pstAccItem->uiSubnetMask) == \
                                   (pstAccItem->uiClientIPAddr & pstAccItem->uiSubnetMask))
                                {
                                    pstSWInfo->ucAccAuth = pstAccItem->ucClient;  /* Client */
                                    break;
                                }
                            }
                        }
                        else  /* Network 2 */
                        {
                            if(((uint8_t)(NW_NET2) == pstAccItem->ucNetwork) ||
                               ((uint8_t)(NW_NET_ALL) == pstAccItem->ucNetwork))
                            {
                                /* IP */
                                if((pstSWInfo->uiIPAddr & pstAccItem->uiSubnetMask) == \
                                   (pstAccItem->uiClientIPAddr & pstAccItem->uiSubnetMask))
                                {
                                    pstSWInfo->ucAccAuth = pstAccItem->ucClient;  /* Client */
                                    break;
                                }
                            }
                        }
                    }/* end of for */
                }
                else
                {
                    /* Disabled */
                    pstSWInfo->ucAccAuth = (uint8_t)(AL_ALL);
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeCmd
*
* 功能描述: 解析Client软件命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧；
*          bCMHasCfg：CM是否有配置标志位。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeCmd( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        DEBUG_CLI( "DecodeCmd: Actual Type-%d\n", usAType );

        switch( usAType )
        {
            case (uint16_t)(CLI_LOGIN):
            {
                DecodeLogin( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CLI_LOGOUT):
            {
                DecodeLogout( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CLI_READ_DIAG):
            {
                DecodeReadDiag( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CLI_READ_VERN):
            {
                DecodeReadVern( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CLI_FWUP_START):
            {
                DecodeFWUPStart( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CLI_FWUP_CONT):
            {
                DecodeFWUPCont( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CLI_FWUP_END):
            {
                DecodeFWUPEnd( pstSWInfo, pucReq, pstResp );
            }
            break;

            default:
            {
                DEBUG_CLI( "DecodeCmd: Actual Type-%d Invalid\n", usAType );
            }
            break;
        }/*switch*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeLogin
*
* 功能描述: 解析登录命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧；
*          bCMHasCfg：CM是否有配置标志位。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeLogin( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;     /* Actual Request Message Type */
    uint8_t ucPSDLen = 0U;     /* Password Length */
    bool_t bPSDValid = false;  /* Password Valid Flag */
    uint8_t ucIndex = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_LOGIN) == usAType )
        {
            if( bCMHasCfg )
            {
                UpdateAccessAuth( pstSWInfo );
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_CLI( "Login: Deny\n" );
                    ucSubReason = (uint8_t)(ERR_ACCESS);
                    pstSWInfo->emLog = LOGOUT;
                }
                else
                {
                    /* Compare Connect Password */
                    memset( s_ucPassword, 0, sizeof(s_ucPassword));
                    ucPSDLen = (uint8_t)sizeof( s_ucPassword );
                    if( !SysGetPassword( s_ucPassword, &ucPSDLen ))
                    {
                        ucPSDLen = 0U;
                    }

                    /* Check Password: Length & Content */
                    if( 0U == ucPSDLen )
                    {
                        bPSDValid = true;  /* No Password */
                    }
                    else if( ucPSDLen == pucReq[PSD_LEN_INDEX])  /* Length */
                    {
                        /* Ckeck Password Content */
                        bPSDValid = true;
                        for( ucIndex = 0U; ucIndex < ucPSDLen; ucIndex++ )
                        {
                            if( s_ucPassword[ucIndex] != pucReq[(uint8_t)(PSD_INDEX)+ucIndex])
                            {
                                bPSDValid = false;
                                DEBUG_CLI( "PWD Err\n" );
                                break;
                            }
                        }
                    }
                    else
                    {
                        bPSDValid = false;
                        DEBUG_CLI( "PWDLen %d != %d\n", pucReq[PSD_LEN_INDEX], ucPSDLen );
                    }

                    if( bPSDValid )
                    {
                        DEBUG_CLI( "Login OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        pstSWInfo->emLog = LOGIN;
                    }
                    else
                    {
                        DEBUG_CLI( "Login Failed: PWD Err\n" );
                        ucSubReason = (uint8_t)(ERR_CONN_PSD);
                        pstSWInfo->emLog = LOGOUT;
                    }
                }
            }
            else
            {
                DEBUG_CLI( "Login Failed: CM No Cfg\n" );
                ucSubReason = (uint8_t)(ERR_CMNOCFG);
                pstSWInfo->emLog = LOGOUT;
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeLogout
*
* 功能描述: 解析退出登录命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeLogout( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_LOGOUT) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                DEBUG_CLI( "Logout OK\n" );
                pstSWInfo->emLog = LOGOUT;
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadDiag
*
* 功能描述: 解析读诊断信息命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeReadDiag( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;
    ReadReq_t const *pstRDReq = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_READ_DIAG) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                pstRDReq = (ReadReq_t const *)&pucReq[APP_MSG_HEADER_LEN];
                if( pstRDReq->ucPMID < (uint8_t)(NUM_OF_PM))
                {
                    DEBUG_CLI( "CliReadDiag OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_CLI_PMID);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadVern
*
* 功能描述: 解析读版本信息命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeReadVern( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;
    ReadReq_t const *pstRDReq = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_READ_VERN) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                pstRDReq = (ReadReq_t const *)&pucReq[APP_MSG_HEADER_LEN];
                if( pstRDReq->ucPMID < (uint8_t)(NUM_OF_PM))
                {
                    DEBUG_CLI( "CliReadVern OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_CLI_PMID);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeFWUPStart
*
* 功能描述: 解析固件版本更新开始命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeFWUPStart( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;
    uint32_t uiIndex = 0U;
    uint16_t usPrgLen = 0U;
    FWInfo_t const *pstFWInfo = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_FWUP_START) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                pstFWInfo = (FWInfo_t const *)&pucReq[APP_MSG_HEADER_LEN];
                if(((uint8_t)(MT_CM) == pstFWInfo->ucModuleType) &&
                   (((uint8_t)(FW_FPGA) == pstFWInfo->ucPrgType) || ((uint8_t)(FW_RTS) == pstFWInfo->ucPrgType)))
                {
                    uiIndex = (uint32_t)(APP_MSG_HEADER_LEN) + sizeof(FWInfo_t);
                    memcpy( &usPrgLen, &pucReq[uiIndex], sizeof(uint16_t));
                    uiIndex += sizeof(uint16_t);  /* FW Data Start Index */

#if 0
                    /* Write FW Data: Open & Write */
#endif

                    DEBUG_CLI( "CliFWUPStart OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_FWPRG_TYPE);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeFWUPCont
*
* 功能描述: 解析固件版本更新继续命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeFWUPCont( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;
    uint32_t uiIndex = 0U;
    uint16_t usPrgLen = 0U;
    FWInfo_t const *pstFWInfo = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_FWUP_CONT) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                pstFWInfo = (FWInfo_t const *)&pucReq[APP_MSG_HEADER_LEN];
                if(((uint8_t)(MT_CM) == pstFWInfo->ucModuleType) &&
                   (((uint8_t)(FW_FPGA) == pstFWInfo->ucPrgType) || ((uint8_t)(FW_RTS) == pstFWInfo->ucPrgType)))
                {
                    uiIndex = (uint32_t)(APP_MSG_HEADER_LEN) + sizeof(FWInfo_t);
                    memcpy( &usPrgLen, &pucReq[uiIndex], sizeof(uint16_t));
                    uiIndex += sizeof(uint16_t);  /* FW Data Start Index */

#if 0
                    /* Write FW Data: Write */
#endif

                    DEBUG_CLI( "CliFWUPCont OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_FWPRG_TYPE);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeFWUPEnd
*
* 功能描述: 解析固件版本更新完成命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令;
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeFWUPEnd( ClientInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;
    FWInfo_t const *pstFWInfo = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CLI_FWUP_END) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                pstFWInfo = (FWInfo_t const *)&pucReq[APP_MSG_HEADER_LEN];
                if(((uint8_t)(MT_CM) == pstFWInfo->ucModuleType) &&
                   (((uint8_t)(FW_FPGA) == pstFWInfo->ucPrgType) || ((uint8_t)(FW_RTS) == pstFWInfo->ucPrgType)))
                {
#if 0
                    /* Write FW Data: Close */
#endif

                    DEBUG_CLI( "CliFWUPEnd OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_FWPRG_TYPE);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CLI( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleResponse
*
* 功能描述: 处理应答
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

static void HandleResponse( void )
{
    ClientInfo_t *pstSWInfo = NULL;  /* Current Software Information */
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);

    if((s_emResp != NONE_RESP) &&
       (s_ucCurIndex < (uint8_t)(APP_SW_CLIENT_NUM)))
    {
        pstSWInfo = &( s_stClientInfo[s_ucCurIndex]);
        if( PM_RESP == s_emResp )
        {
            /* Read PM Response Message */
            s_stRespMsg.usLen = (uint16_t)sizeof( s_stRespMsg.ucBuff );
            if( !SysGetMsg( SwitchPMID( pstSWInfo->emSelPMID), CLIENT_RESP, s_stRespMsg.ucBuff, &s_stRespMsg.usLen ))
            {
                s_stRespMsg.usLen = 0U;
            }

            if( 0U == s_stRespMsg.usLen )
            {
                /* No Response Message */
                if( s_uiWaitPMCycleCnt >= (uint32_t)(CLIENT_PM_TIMEOUT_CYCLE_CNT))
                {
                    /* Record Error Info.: PM Timeout */
                    DEBUG_CLI( "HandleResp: PMResp Timeout\n" );
                    s_uiWaitPMCycleCnt = 0U;

                    /* Make Response Message */
                    if( !MakeRespMsg((uint8_t)(ERR_PMBUSY), s_stReqMsg.ucBuff, &s_stRespMsg ))
                    {
                        DEBUG_CLI( "MakeRespMsg Err\n" );
                    }

                    s_emResp = CM_RESP;
                }
                else
                {
                    s_uiWaitPMCycleCnt++;
                    DEBUG_CLI( "HandleResp: Wait PMResp-%d\n", s_uiWaitPMCycleCnt );
                }
            }
        }

        if( 0U == s_stRespMsg.usLen )
        {
            if( CM_RESP == s_emResp )  /* CM Response */
            {
                /* Record Error Info: s_stRespMsg.usLen should greater than zero */
                DEBUG_CLI( "HandleResp: CM Response - s_stRespMsg.usLen Invalid\n" );
                if( !SysResetAppSwLockedFlag( APP_CLIENT, s_ucCurIndex ))
                {
                    DEBUG_CLI( "HandleResp: Reset Locked Flag Failed\n" );
                }

                s_ucCurIndex++;
                s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_CLIENT_NUM);
                s_emState = IDLE_STATE;
            }
        }
        else
        {
            /* Send Response Message */
            ucResult = Send( s_stRespMsg.ucBuff, s_stRespMsg.usLen,
                             pstSWInfo->uiIPAddr, pstSWInfo->usPort, pstSWInfo->emNetwork,
                             TYPE_CLIENT, s_ucCurIndex );

            /* Reset Lock Flag */
            if( !SysResetAppSwLockedFlag( APP_CLIENT, s_ucCurIndex ))
            {
                DEBUG_CLI( "HandleResp: Reset Locked Flag Failed\n" );
            }

            if( ucResult != (uint8_t)(SEND_SUCCESS))
            {
                /* Send Failed: Record Error Info */
                DEBUG_CLI( "HandleResp: ExtCommSend Failed\n" );

                /* Clear Local Info */
                ClearClientLocalInfo( s_ucCurIndex );

                /* Clear System Info */
                ClearClientSysInfo( s_ucCurIndex );
            }
            else
            {
                DEBUG_CLI( "HandleResp: ExtCommSend OK\n" );
            }

            s_ucCurIndex++;
            s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_CLIENT_NUM);
            s_emState = IDLE_STATE;
        }
    }/* end of if( s_emResp != NONE_RESP ) */
    else
    {
        DEBUG_CLI( "HandleResp: s_emResp-%d / s_ucCurIndex-%d Invalid\n", s_emResp, s_ucCurIndex );
        s_emState = IDLE_STATE;
    }

    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

