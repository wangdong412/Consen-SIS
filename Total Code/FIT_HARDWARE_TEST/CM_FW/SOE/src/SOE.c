/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : SOE软件应用模块
*
*  文件名称    : SOE.c
*
*  功能简介    : 处理SOE软件命令和相关配置
*
*  作者        : 王东
*
*  创建日期    : 2016-02-04
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
#include "../inc/SOE.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                          /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                      /* 本控制站的ID */
static uint8_t s_ucCurIndex;                        /* 当前SOE软件的索引号 */
static ModuleState_t s_emState;                     /* 状态 */
static uint32_t s_uiWaitPMCycleCnt;                 /* 已等待PM应答的拍数 */
static RespModuleType_t s_emResp;                   /* SOE命令被哪个模块应答 */
static SOEInfo_t s_stSOEInfo[APP_SW_SOE_NUM];       /* SOE软件信息，数组成员与SOE软件一一对应 */
static NetMsg_t s_stReqMsg;                         /* SOE软件命令 */
static RespMsg_t s_stRespMsg;                       /* 应答消息 */
static uint8_t s_ucPassword[(MAX_CONN_PWD_LEN)+1U]; /* 连接密码 */

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
static void ClearSOELocalInfo( uint8_t ucSWIndex );
static void ClearSOESysInfo( uint8_t ucSWIndex );

/* Configuration */
static void ClearLocalConfig( void );
static void UpdateAccessAuth( SOEInfo_t *pstSWInfo );

/* Handle Command */
static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, SOEInfo_t *pstSWInfo );
static bool_t CheckPMHandle( uint8_t const pucReq[]);
static void PMHandle( SOEInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg );
static SelPMID_t SelectPM( SOEInfo_t *pstSWInfo, uint8_t const pucReq[]);

/* Decode Command */
static void DecodeCmd( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogin( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogout( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );

/* Response */
static void MakeRespMsg( uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SOEInit
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

void SOEInit( void )
{
    DEBUG_SOE( "SOEInit\n" );

    uint8_t ucSWIndex = 0U;
    s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucCurIndex = 0U;
    s_emState = IDLE_STATE;
    s_uiWaitPMCycleCnt = 0U;
    s_emResp = NONE_RESP;

    /* Local SOE Software Info. */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_SOE_NUM); ucSWIndex++ )
    {
        ClearSOELocalInfo( ucSWIndex );
    }

    /* SOE Software Command */
    memset( &s_stReqMsg, 0, sizeof(NetMsg_t));
    s_stReqMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Response Message Buffer */
    memset( &s_stRespMsg, 0, sizeof(RespMsg_t));

    /* Connect Password */
    memset( s_ucPassword, 0, sizeof(s_ucPassword));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SOECycle
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

void SOECycle( void )
{
    static bool_t s_bClrDone = false;
    uint8_t ucSWIndex = 0U;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Update Local Station ID */
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();

    /* Handle Configuration */
    if( s_bCMHasCfg )
    {
        s_bClrDone = false;
        HandleConfiguration();
    }
    else
    {
        if( !s_bClrDone )
        {
            ClearLocalConfig();
            s_bClrDone = true;
        }
    }

    /* Handle Software Timeout and Disconnect */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_SOE_NUM); ucSWIndex++ )
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
* 函数名称: ClearSOELocalInfo
*
* 功能描述: 清除指定SOE软件的本地信息
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

static void ClearSOELocalInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_SOE_NUM))
    {
        memset( &s_stSOEInfo[ucSWIndex], 0, sizeof(SOEInfo_t));

        s_stSOEInfo[ucSWIndex].uiIPAddr = 0U;
        s_stSOEInfo[ucSWIndex].usPort = 0U;
        s_stSOEInfo[ucSWIndex].emNetwork = INVALID_NETWORK_PORT;
        s_stSOEInfo[ucSWIndex].emLog = LOGOUT;
        s_stSOEInfo[ucSWIndex].ucAccAuth = (uint8_t)(AL_ALL);
        s_stSOEInfo[ucSWIndex].emSelPMID = SELPM_NONE;
        s_stSOEInfo[ucSWIndex].uiWaitReqCycleCnt = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearSOESysInfo
*
* 功能描述: 清除指定SOE软件的系统资源信息
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

static void ClearSOESysInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_SOE_NUM))
    {
        /* Reset Lock Flag */
        if( !SysResetAppSwLockedFlag( APP_SOE, ucSWIndex ))
        {
            DEBUG_SOE( "Reset SOE-%d Locked Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_SOE_E_SYS_RST_LOCK));
        }

        /* Reset Connect Flag */
        if( !SysResetAppSwConnectFlag( APP_SOE, ucSWIndex ))
        {
            DEBUG_SOE( "Reset SOE-%d Connect Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_SOE_E_SYS_RST_CONN));
        }

        /* Set Clear Flag */
        if( !SysSetAppSwClearFlag( APP_SOE, ucSWIndex ))
        {
            DEBUG_SOE( "Set SOE-%d Clear Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_SOE_E_SYS_SET_CLR));
        }

        /* Clear Request Msg */
        SysClearAppMsg( APP_SOE, ucSWIndex, APP_MSG_REQ );
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
        DEBUG_SOE( "Access Control Cfg Switch\n" );
        LogWrite( (uint32_t)(LOGID_SOE_CFG_SWITCH_ACCESS_CTRL));

        for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_SOE_NUM); ucSWIndex++ )
        {
            UpdateAccessAuth( &s_stSOEInfo[ucSWIndex]);
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
        UpdateAccessAuth( &s_stSOEInfo[ucSWIndex]);
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
    if( ucSWIndex < (uint8_t)(APP_SW_SOE_NUM))
    {
        if( SysGetAppSwConnectFlag( APP_SOE, ucSWIndex))  /* Connect */
        {
            s_stSOEInfo[ucSWIndex].uiWaitReqCycleCnt++;

            /* Connect & Timeout: Clear */
            if( s_stSOEInfo[ucSWIndex].uiWaitReqCycleCnt >= (uint32_t)(SOE_SW_TIMEOUT_CYCLE_CNT))
            {
                DEBUG_SOE( "SOE-%d Timeout\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_SOE_E_SWTO));

                /* Clear Local Info */
                ClearSOELocalInfo( ucSWIndex );

                /* Clear System Info */
                ClearSOESysInfo( ucSWIndex );
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

    if( ucSWIndex < (uint8_t)(APP_SW_SOE_NUM))
    {
        if( !SysGetAppSwConnectFlag( APP_SOE, ucSWIndex ))  /* Disconnect */
        {
            if( s_stSOEInfo[ucSWIndex].uiIPAddr != 0U )
            {
                DEBUG_SOE( "SOE-%d Disconnected\n", ucSWIndex );

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
                    ClearSOELocalInfo( ucSWIndex );  /* Clear Local Info */
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

    while( ucCheckCnt < (uint8_t)(APP_SW_SOE_NUM))
    {
        /* Read Request Message */
        s_stReqMsg.stNetHead.usLen = 0U;
        usLen = (uint16_t)sizeof( NetMsg_t );
        if( !SysGetAppMsg( APP_SOE, s_ucCurIndex, APP_MSG_REQ, (puint8_t)&s_stReqMsg, &usLen ))
        {
            usLen = 0U;
        }

        if((usLen > 0U) && (s_stReqMsg.stNetHead.usLen > 0U))
        {
            /* Has Message */
            s_stSOEInfo[s_ucCurIndex].uiWaitReqCycleCnt = 0U;  /* Clear wait request count */
            ucSubReason = CheckCmd( &s_stReqMsg, &s_stSOEInfo[s_ucCurIndex]);
            if((uint8_t)(ERR_SUCCESS) == ucSubReason )
            {
                /* Valid */
                DEBUG_SOE( "HandleCmd: Msg Valid\n" );
                if( CheckPMHandle( s_stReqMsg.ucBuff ))
                {
                    /* PM Handle */
                    DEBUG_SOE( "HandleCmd: PM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));
                    PMHandle( &s_stSOEInfo[s_ucCurIndex], &s_stReqMsg, &s_stRespMsg, s_bCMHasCfg );
                }
                else
                {
                    /* CM Handle */
                    DEBUG_SOE( "HandleCmd: CM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));

                    /* Decode Cmd */
                    DecodeCmd( &s_stSOEInfo[s_ucCurIndex], s_stReqMsg.ucBuff, &s_stRespMsg, s_bCMHasCfg );
                    s_emResp = CM_RESP;
                }
            }
            else
            {
                /* Invalid */
                DEBUG_SOE( "HandleCmd: Msg Invalid\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_CMD));
                MakeRespMsg( ucSubReason, s_stReqMsg.ucBuff, &s_stRespMsg );  /* Make Response Message */
                s_emResp = CM_RESP;
            }

            /* Lock & Busy */
            if( !SysSetAppSwLockedFlag( APP_SOE, s_ucCurIndex ))
            {
                DEBUG_SOE( "HandleCmd: Set Locked Flag Failed\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_SYS_SET_LOCK));
            }

            s_emState = BUSY_STATE;
            break;
        }
        else
        {
            /* No Message */
            s_ucCurIndex++;
            s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_SOE_NUM);
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

static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, SOEInfo_t *pstSWInfo )
{
    uint8_t ucSubReason = (uint8_t)(ERR_PARA);  /* Second Level Response Code */
    uint16_t usAType = 0U;                      /* Actual Command Type */
    uint8_t const *pucReq = NULL;

    if((pstReqMsg != NULL) &&
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
                DEBUG_SOE( "CheckCmd: SrcAddr Invalid\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_SW_SRCADDR));
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
                DEBUG_SOE( "CheckCmd: Type-0x%X != 0x%X\n", pucReq[APP_REQ_TYPE_INDEX], (uint8_t)(APP_REQ_TYPE));
                ucSubReason = (uint8_t)(ERR_TYPE);  /* Type */
            }
            else if( pucReq[APP_REQ_PROTECT_INDEX] != (uint8_t)(APP_REQ_PROTECT))
            {
                DEBUG_SOE( "CheckCmd: Protect-%d Invalid\n", pucReq[APP_REQ_PROTECT_INDEX]);
                ucSubReason = (uint8_t)(ERR_PROTECT);  /* Protect */
            }
            else
            {
                /* Actual Type */
                usAType = GetReqActualType( pucReq );
                if((usAType < (uint16_t)(SOE_ATYPE_MIN)) ||
                   (usAType > (uint16_t)(SOE_ATYPE_MAX)))
                {
                    DEBUG_SOE( "CheckCmd: Actual Type-%d, HI-%d, LOW-%d Invalid\n", \
                               usAType, pucReq[APP_REQ_ATYPE_HIINDEX], pucReq[APP_REQ_ATYPE_LOWINDEX]);
                    ucSubReason = (uint8_t)(ERR_ACTUAL_TYPE);
                }
                /*else if( pucReq[APP_RESP_STAID_INDEX] != s_ucLocalStaID )
                {
                    DEBUG_SOE( "CheckCmd: StaID-%d Invalid\n", pucReq[APP_RESP_STAID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_STAID);
                }*/
                else if( pucReq[APP_REQ_PMID_INDEX] != (uint8_t)(APP_REQ_PMID))
                {
                    DEBUG_SOE( "CheckCmd: PM ID-%d Invalid\n", pucReq[APP_REQ_PMID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_PMID);  /* PM ID */
                }
                else if( pucReq[APP_REQ_LAST_INDEX] != (uint8_t)(APP_REQ_LAST_FLAG))
                {
                    DEBUG_SOE( "CheckCmd: Last Flag-%d Invalid\n", pucReq[APP_REQ_LAST_INDEX]);
                    ucSubReason = (uint8_t)(ERR_LAST_FLAG);  /* Last Flag */
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

    if( pucReq != NULL )
    {
        usAType = GetReqActualType( pucReq );
        if(((uint16_t)(SOE_LOGIN) == usAType) ||
           ((uint16_t)(SOE_LOGOUT) == usAType))
        {
            bPMHandle = false;
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

static void MakeRespMsg( uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    puint8_t pucResp = NULL;
    uint16_t usLength = 0U;  /* Length Of Header + Data Area */
    uint16_t usIndex = 0U;
    uint32_t uiCalcCRC = 0U;

    if((pucReq != NULL) &&
       (pstResp != NULL))
    {
        pstResp->usLen = 0U;        /* Reset Length */
        pucResp = pstResp->ucBuff;  /* Buffer */

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

            /* Login/Logout: No Data Area */
            pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
            usLength = (uint16_t)(APP_MSG_HEADER_LEN);
        }

        /* Length: Header + Data Area */
        pucResp[APP_RESP_LEN_HIINDEX] = (uint8_t)((usLength >> 8) & 0xFF);
        pucResp[APP_RESP_LEN_LOWINDEX] = (uint8_t)(usLength & 0xFF);

        /* CRC */
        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucResp, (uint32_t)usLength );

        /* Write CRC Area: High First */
        usIndex = usLength;  /* CRC Index */
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 24) & 0xFF);  /* Highest Byte */
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 16) & 0xFF);
        pucResp[usIndex++] = (uint8_t)((uiCalcCRC >> 8)  & 0xFF);
        pucResp[usIndex++] = (uint8_t)( uiCalcCRC & 0xFF );

        /* Update Response Message Length */
        pstResp->usLen = usLength + (uint16_t)(APP_MSG_CRC_LEN);
    }

    return;
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

static void PMHandle( SOEInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    SelPMID_t emSelPMID = SELPM_NONE;
    uint16_t usAType = 0U;  /* Actual Request Message Type */
    uint16_t usIndex = 0U;

#ifdef DEBUG_SWSOE
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
            DEBUG_SOE( "PMHandle: SOE should login first\n" );
            LogWrite( (uint32_t)(LOGID_SOE_E_NOTLOGIN));
            ucSubReason = (uint8_t)(ERR_LOG);  /* Need To Login First */
        }
        else
        {
            if( bCMHasCfg )
            {
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_SOE( "PMHandle: Access Authority - Deny\n" );
                    LogWrite( (uint32_t)(LOGID_SOE_E_ACCESS_DENY));
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
                }
                else if((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth )
                {
                    usAType = GetReqActualType( pstReqMsg->ucBuff );
                    if((uint16_t)(SOE_CLEAR) == usAType )
                    {
                        DEBUG_SOE( "PMHandle: Access Authority - Readonly\n" );
                        LogWrite( (uint32_t)(LOGID_SOE_E_ACCESS_RO));
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
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
            /* Login & Has Access Authority: Send To PM Module Specified In Msg */
            usIndex = (uint16_t)(APP_MSG_HEADER_LEN);
            if((pstReqMsg->ucBuff[usIndex] < 0U) ||
               (pstReqMsg->ucBuff[usIndex] > 2U))
            {
                DEBUG_SOE( "PMHandle: PM ID-%d Invalid\n", pstReqMsg->ucBuff[usIndex]);
                LogWrite( (uint32_t)(LOGID_SOE_E_CMD_PMID));
                ucSubReason = (uint8_t)(ERR_PMID);
            }
            else
            {
                emSelPMID = SelectPM( pstSWInfo, pstReqMsg->ucBuff );
                if( SELPM_NONE == emSelPMID )
                {
                    DEBUG_SOE( "PMHandle: PMID-%d CM BUS Invalid\n", emSelPMID );
                    LogWrite( (uint32_t)(LOGID_SOE_E_CMBUS));
                    ucSubReason = (uint8_t)(ERR_CMBUS);  /* CM BUS Error */
                }
                else
                {
                    /* Write Req To The Selected PM */
                    if( SysSetMsg( SwitchPMID( emSelPMID ), SOE_REQ, pstReqMsg->ucBuff, pstReqMsg->stNetHead.usLen ))
                    {
                        /* PM Response */
#ifdef DEBUG_SWSOE
                        /* Print Request Message */
                        {
                            printf( "PMHandle: Message Length: %d, Content(Max Top 50B): \n", pstReqMsg->stNetHead.usLen );
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
                        DEBUG_SOE( "PMHandle: PM Response\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        s_emResp = PM_RESP;
                        s_uiWaitPMCycleCnt = 0U;

                        /* Clear response message from the selected PM module */
                        /* Avoid old response messge make communication error */
                        if( !SysClearMsg( SwitchPMID(pstSWInfo->emSelPMID), SOE_RESP ))
                        {
                            //DEBUG_SOE( "PMHandle: SysClearMsg Failed, SelPMID-%d\n", emSelPMID );
                        }
                    }
                    else
                    {
                        DEBUG_SOE( "PMHandle: SysSetMsg Failed, PMID-%d\n", emSelPMID );
                        LogWrite( (uint32_t)(LOGID_SOE_E_SYS_SET_MSG));
                        ucSubReason = (uint8_t)(ERR_SYSWREQTOPM);  /* 将请求写入系统资源失败 - 用于发往PM模块 */
                    }
                }
            }
        }

        if( ucSubReason != (uint8_t)(ERR_SUCCESS))
        {
            /* CM Response */
            DEBUG_SOE( "PMHandle: CM Response\n" );
            MakeRespMsg( ucSubReason, pstReqMsg->ucBuff, pstResp );
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
* 输入参数：pucReq：指向请求信息。
*
* 输出参数: 无
*
* 返 回 值: 选取的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static SelPMID_t SelectPM( SOEInfo_t *pstSWInfo, uint8_t const pucReq[])
{
    SelPMID_t emSelPMID = SELPM_NONE;
    uint16_t usIndex = (uint16_t)(APP_MSG_HEADER_LEN);

    if((pstSWInfo != NULL) &&
       (pucReq != NULL))
    {
        if( 0U == pucReq[usIndex])  /* PMA */
        {
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA ))
            {
                emSelPMID = SELPM_A;
            }
        }
        else if( 1U == pucReq[usIndex])  /* PMB */
        {
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB ))
            {
                emSelPMID = SELPM_B;
            }
        }
        else if( 2U == pucReq[usIndex])  /* PMC */
        {
            if((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC ))
            {
                emSelPMID = SELPM_C;
            }
        }
        else  /* Invalid */
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
* 功能描述: 更新相应SOE软件的访问权限
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

static void UpdateAccessAuth( SOEInfo_t *pstSWInfo )
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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucSOE;  /* SOE */
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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucSOE;  /* SOE */
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
* 功能描述: 解析SOE软件命令
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

static void DecodeCmd( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        switch( usAType )
        {
            case (uint16_t)(SOE_LOGIN):
            {
                DecodeLogin( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(SOE_LOGOUT):
            {
                DecodeLogout( pstSWInfo, pucReq, pstResp );
            }
            break;

            default:
            {
                DEBUG_SOE( "DecodeCmd: Actual Type-%d Invalid\n", usAType );
                LogWrite( (uint32_t)(LOGID_SOE_E_CMD_ACTUAL_TYPE));
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

static void DecodeLogin( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
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
        if((uint16_t)(SOE_LOGIN) == usAType )
        {
            if( bCMHasCfg )
            {
                UpdateAccessAuth( pstSWInfo );
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_SOE( "Login: Deny\n" );
                    LogWrite( (uint32_t)(LOGID_SOE_E_ACCESS_DENY));
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
                                DEBUG_SOE( "PWD Err\n" );
                                LogWrite( (uint32_t)(LOGID_SOE_E_CMD_PASSWORD_CONTENT));
                                break;
                            }
                        }
                    }
                    else
                    {
                        bPSDValid = false;
                        DEBUG_SOE( "PWDLen %d != %d\n", pucReq[PSD_LEN_INDEX], ucPSDLen );
                        LogWrite( (uint32_t)(LOGID_SOE_E_CMD_PASSWORD_LEN));
                    }

                    if( bPSDValid )
                    {
                        DEBUG_SOE( "Login OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        pstSWInfo->emLog = LOGIN;
                    }
                    else
                    {
                        DEBUG_SOE( "Login Failed: PWD Err\n" );
                        ucSubReason = (uint8_t)(ERR_CONN_PSD);
                        pstSWInfo->emLog = LOGOUT;
                    }
                }
            }
            else
            {
                DEBUG_SOE( "Login Failed: CM No Cfg\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_CM_NOCFG));
                ucSubReason = (uint8_t)(ERR_CMNOCFG);
                pstSWInfo->emLog = LOGOUT;
            }

            MakeRespMsg( ucSubReason, pucReq, pstResp );
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

static void DecodeLogout( SOEInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(SOE_LOGOUT) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                DEBUG_SOE( "Logout OK\n" );
                pstSWInfo->emLog = LOGOUT;
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_SOE_E_NOTLOGIN));
            }

            MakeRespMsg( ucSubReason, pucReq, pstResp );
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
    SOEInfo_t *pstSWInfo = NULL;  /* Current Software Information */
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);

    if((s_emResp != NONE_RESP) &&
       (s_ucCurIndex < (uint8_t)(APP_SW_SOE_NUM)))
    {
        pstSWInfo = &( s_stSOEInfo[s_ucCurIndex]);
        if( PM_RESP == s_emResp )
        {
            /* Read PM Response Message */
            s_stRespMsg.usLen = (uint16_t)sizeof( s_stRespMsg.ucBuff );
            if( !SysGetMsg( SwitchPMID( pstSWInfo->emSelPMID ), SOE_RESP, s_stRespMsg.ucBuff, &s_stRespMsg.usLen ))
            {
                s_stRespMsg.usLen = 0U;
            }

            if( 0U == s_stRespMsg.usLen )
            {
                /* No Response Message */
                if( s_uiWaitPMCycleCnt >= (uint32_t)(SOE_PM_TIMEOUT_CYCLE_CNT))
                {
                    /* Record Error Info.: PM Timeout */
                    DEBUG_SOE( "HandleResp: PMResp Timeout\n" );
                    LogWrite( (uint32_t)(LOGID_SOE_E_RESP_PM_TIMEOUT));
                    s_uiWaitPMCycleCnt = 0U;
                    MakeRespMsg((uint8_t)(ERR_PMBUSY), s_stReqMsg.ucBuff, &s_stRespMsg );
                    s_emResp = CM_RESP;
                }
                else
                {
                    s_uiWaitPMCycleCnt++;
                    DEBUG_SOE( "HandleResp: Wait PMResp-%d\n", s_uiWaitPMCycleCnt );
                }
            }
        }

        if( 0U == s_stRespMsg.usLen )
        {
            if( CM_RESP == s_emResp )  /* CM Response */
            {
                /* Record Error Info: s_stRespMsg.usLen should greater than zero */
                DEBUG_SOE( "HandleResp: CM Resp - s_stRespMsg.usLen Invalid\n" );
                if( !SysResetAppSwLockedFlag( APP_SOE, s_ucCurIndex ))
                {
                    DEBUG_SOE( "HandleResp: Reset Locked Flag Failed\n" );
                    LogWrite( (uint32_t)(LOGID_SOE_E_SYS_RST_LOCK));
                }

                s_ucCurIndex++;
                s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_SOE_NUM);
                s_emState = IDLE_STATE;
            }
        }
        else
        {
            /* Send Response Message */
            ucResult = Send( s_stRespMsg.ucBuff, s_stRespMsg.usLen,
                             pstSWInfo->uiIPAddr, pstSWInfo->usPort, pstSWInfo->emNetwork,
                             TYPE_SOE, s_ucCurIndex );

            /* Reset Lock Flag */
            if( !SysResetAppSwLockedFlag( APP_SOE, s_ucCurIndex ))
            {
                DEBUG_SOE( "HandleResp: Reset Locked Flag Failed\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_SYS_RST_LOCK));
            }

            if( ucResult != (uint8_t)(SEND_SUCCESS))
            {
                /* Send Failed: Record Error Info */
                DEBUG_SOE( "HandleResp: ExtCommSend Failed\n" );
                LogWrite( (uint32_t)(LOGID_SOE_E_RESP_EXCOMM_SEND));

                /* Clear Local Info */
                ClearSOELocalInfo( s_ucCurIndex );

                /* Clear System Info */
                ClearSOESysInfo( s_ucCurIndex );
            }
            else
            {
                DEBUG_SOE( "HandleResp: ExtCommSend OK\n" );
            }

            s_ucCurIndex++;
            s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_SOE_NUM);
            s_emState = IDLE_STATE;
        }
    }/* end of if( s_emResp != NONE_RESP ) */
    else
    {
        DEBUG_SOE( "HandleResp: s_emResp-%d / s_ucCurIndex-%d Invalid\n", s_emResp, s_ucCurIndex );
        s_emState = IDLE_STATE;
    }

    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

