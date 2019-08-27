/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : OPC软件应用模块
*
*  文件名称    : OPC.c
*
*  功能简介    : 处理OPC软件命令和相关配置
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
#include "../inc/OPC.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                           /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                       /* 本控制站的ID */
static uint8_t s_ucCurIndex;                         /* 当前OPC软件的索引号 */
static ModuleState_t s_emState;                      /* 状态 */
static uint32_t s_uiWaitPMCycleCnt;                  /* 已等待PM应答的拍数 */
static RespModuleType_t s_emResp;                    /* OPC命令被哪个模块应答 */
static OPCInfo_t s_stOPCInfo[APP_SW_OPC_NUM];        /* OPC软件信息，数组成员与OPC软件一一对应 */
static NetMsg_t s_stReqMsg;                          /* OPC软件命令 */
static RespMsg_t s_stRespMsg;                        /* 应答消息 */
static uint8_t s_ucPassword[(MAX_CONN_PWD_LEN)+1U];  /* 连接密码 */
static uint16_t s_usReadIndex;                       /* 将被读取的OPC变量的索引值 */
static PMController_t s_emLastPMID;                  /* 读OPC变量在线值时，记录最近一次使用的PM的编号 */
static uint16_t s_usTotalMsgNum;                     /* OPC变量总包数 */

/* Configuration */
static AccCtrlInfo_t s_stAccCtrlInfo;                      /* 访问控制信息 */
static OPCVarTblInfo_t s_stOPCVarTblInfo;                  /* OPC变量表信息 */
static OPCNetworkInfo_t s_stNetInfo[NUM_OF_NETWORK_PORT];  /* 网口配置参数信息 */

/* OPC Configuration Copy */
static uint32_t s_uiOPCCfgOffset;

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
static void InitNetInfo( OPCNetworkInfo_t *pstNetInfo );
static void ClearOPCLocalInfo( uint8_t ucSWIndex );
static void ClearOPCSysInfo( uint8_t ucSWIndex );

/* Configuration */
static void ClearLocalInfo( void );
static void ClearLocalConfigInfo( void );

/* OPC Configuration */
static bool_t HandleOPCVarTblInfo( void );
static void SwitchOPCVarCfgInfo( void );
static void ClearCurOPCCfg( void );
static bool_t GetOPCConfigInfo( bool_t *pbComplete );
static bool_t CheckOPCVar( OPCVar_t const *pstOPCVar );
static bool_t CheckOPCVarCfgInfo( void );
static OPCVarInfo_t const* GetCurOPCVarInfo( OPCVarTblInfo_t const *pstOPCCfg );

/* Network Configuration */
static bool_t HandleNetworkInfo( NetworkPortType_t emNet, OPCNetworkInfo_t *pstNetInfo );
static bool_t CheckNetCfgInfo( OPCNetConfigInfo_t const *pstNetCfgInfo );
static bool_t SwitchNetCfgInfo( OPCNetworkInfo_t *pstNetInfo, OPCNetConfigInfo_t const *pstNetCfgInfo );
static bool_t CheckPortWriteEnable( NetworkPortType_t emNet, OPCNetworkInfo_t const *pstNetInfo );

/* Access Control Configuration */
static void UpdateAccessAuth( OPCInfo_t *pstSWInfo );

/* Handle Command */
static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, OPCInfo_t *pstSWInfo );
static bool_t CheckPMHandle( uint8_t const pucReq[]);
static void PMHandle( OPCInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg );
static SelPMID_t SelectPM( OPCInfo_t *pstSWInfo );

/* Decode Command */
static void DecodeCmd( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogin( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogout( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeReadOPCOnLine( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );

/* Response */
static bool_t MakeRespMsg( OPCInfo_t *pstSWInfo, uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp );
static uint16_t AddOPCOnlineData( OPCInfo_t *pstSWInfo, uint8_t pucRespData[], bool_t *pbCMBus );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: OPCInit
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

void OPCInit( void )
{
    DEBUG_OPC( "OPCInit\n" );

    uint8_t ucSWIndex = 0U;
    s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucCurIndex = 0U;
    s_emState = IDLE_STATE;
    s_uiWaitPMCycleCnt = 0U;
    s_emResp = NONE_RESP;
    s_usReadIndex = 0U;
    s_emLastPMID = INVALID_PM_ID;
    s_usTotalMsgNum = 0U;

    /* Local OPC Software Info. */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_OPC_NUM); ucSWIndex++ )
    {
        ClearOPCLocalInfo( ucSWIndex );
        s_stOPCInfo[ucSWIndex].usSequence = 0U;
    }

    /* OPC Software Command */
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
* 函数名称: OPCCycle
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

void OPCCycle( void )
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
        ClearLocalInfo();
    }

    /* Handle Software Timeout and Disconnect */
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_OPC_NUM); ucSWIndex++ )
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
* 函数名称: ClearOPCLocalInfo
*
* 功能描述: 清除指定OPC软件的本地信息
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

static void ClearOPCLocalInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_OPC_NUM))
    {
        s_stOPCInfo[ucSWIndex].uiIPAddr = 0U;
        s_stOPCInfo[ucSWIndex].usPort = 0U;
        s_stOPCInfo[ucSWIndex].emNetwork = INVALID_NETWORK_PORT;
        s_stOPCInfo[ucSWIndex].emLog = LOGOUT;
        s_stOPCInfo[ucSWIndex].ucAccAuth = (uint8_t)(AL_ALL);
        s_stOPCInfo[ucSWIndex].emSelPMID = SELPM_NONE;
        s_stOPCInfo[ucSWIndex].usVarIndex = 0U;
        s_stOPCInfo[ucSWIndex].uiWaitReqCycleCnt = 0U;
        s_stOPCInfo[ucSWIndex].usMsgIndex = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearOPCSysInfo
*
* 功能描述: 清除指定OPC软件的系统资源信息
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

static void ClearOPCSysInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_OPC_NUM))
    {
        /* Reset Lock Flag */
        if( !SysResetAppSwLockedFlag( APP_OPC, ucSWIndex ))
        {
            DEBUG_OPC( "Reset OPC-%d Locked Flag Failed\n", ucSWIndex );
        }

        /* Reset Connect Flag */
        if( !SysResetAppSwConnectFlag( APP_OPC, ucSWIndex ))
        {
            DEBUG_OPC( "Reset OPC-%d Connect Flag Failed\n", ucSWIndex );
        }

        /* Set Clear Flag */
        if( !SysSetAppSwClearFlag( APP_OPC, ucSWIndex ))
        {
            DEBUG_OPC( "Set OPC-%d Clear Flag Failed\n", ucSWIndex );
        }

        /* Clear Request Msg */
        SysClearAppMsg( APP_OPC, ucSWIndex, APP_MSG_REQ );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitNetInfo
*
* 功能描述: 初始化本地保存的某一网口的配置信息
*
* 输入参数: pstNetInfo：指向本地保存的访问控制信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitNetInfo( OPCNetworkInfo_t *pstNetInfo )
{
    uint8_t ucVerIndex = 0U;
    OPCNetConfigInfo_t *pNetCfg = NULL;

    if( pstNetInfo != NULL )
    {
        memset( pstNetInfo, 0, sizeof(OPCNetConfigInfo_t));

        /* Network Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pNetCfg = &( pstNetInfo->stNetConfig[ucVerIndex]);
            pNetCfg->emActive = NOT_ACTIVE;
            pNetCfg->bUpdate = false;
            pNetCfg->bPortWriteEnable = false;
        }
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
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/

static void InitLocalConfigInfo( void )
{
    uint8_t ucIndex = 0U;

    /* Access Control Info. */
    InitAccessCtrlInfo( &s_stAccCtrlInfo );

    /* Net Configuration Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_NETWORK_PORT); ucIndex++ )
    {
        InitNetInfo( &s_stNetInfo[ucIndex]);
    }

    /* OPC Variable Table */
    memset( &s_stOPCVarTblInfo, 0, sizeof(OPCVarTblInfo_t));

    /* Flag */
    s_stOPCVarTblInfo.bValid = false;
    s_stOPCVarTblInfo.bCopying = false;

    /* Local Configuration */
    for( ucIndex = 0U; ucIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucIndex++ )
    {
        s_stOPCVarTblInfo.stOPCVarInfo[ucIndex].emActive = NOT_ACTIVE;
    }

    /* OPC Configuration Copy */
    s_uiOPCCfgOffset = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CalcTotalMsgNum
*
* 功能描述: 计算总包数
*
* 输入参数: pstOPCVarTblInfo：OPC变量表信息
*
* 输出参数: 无
*
* 返 回 值: 总包数
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t CalcTotalMsgNum( OPCVarTblInfo_t const *pstOPCVarTblInfo )
{
    uint16_t usTotalMsgNum = 0U;
    uint32_t uiDataLen = 0U;
    uint32_t uiTotalDataLen = 0U;
    uint32_t uiIndex = 0U;
    OPCVarInfo_t const *pstOPCVarInfo = NULL;
    OPCVar_t const *pstVar = NULL;

    if( pstOPCVarTblInfo != NULL )
    {
        pstOPCVarInfo = GetCurOPCVarInfo( pstOPCVarTblInfo );
        if( pstOPCVarInfo != NULL )
        {
            for( uiIndex = 0U; uiIndex < pstOPCVarInfo->uiVarNum; uiIndex++ )
            {
                pstVar = &( pstOPCVarInfo->stOPCVar[uiIndex]);
                if( pstVar->usVarSize > 0U )
                {
                    uiDataLen = pstVar->usVarSize;
                }
                else
                {
                    uiDataLen = 1U;
                }

                if((uiTotalDataLen + uiDataLen) > (uint32_t)(OPC_MAX_READ_LEN))
                {
                    usTotalMsgNum++;
                    uiTotalDataLen = uiDataLen;
                }
                else
                {
                    uiTotalDataLen += uiDataLen;
                }
            }/* end of for */

            /* One message if not enough to OPC_MAX_READ_LEN */
            if( 0U == usTotalMsgNum )
            {
                usTotalMsgNum = 1U;
            }
        }
    }

    return usTotalMsgNum;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 清除本地配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/

static void ClearLocalInfo( void )
{
    uint8_t ucIndex = 0U;

    ClearLocalConfigInfo();

    /* Update Access Authority */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_AMS_NUM); ucIndex++ )
    {
        UpdateAccessAuth( &s_stOPCInfo[ucIndex]);
    }

    /* Reset */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_OPC_NUM); ucIndex++ )
    {
        s_stOPCInfo[ucIndex].usVarIndex = 0U;  /* Variable Index */
        s_stOPCInfo[ucIndex].usMsgIndex = 0U;  /* Message Index */
    }

    /* Reset Total Message Number */
    s_usTotalMsgNum = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalConfigInfo
*
* 功能描述: 清除本地保存的配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/

static void ClearLocalConfigInfo( void )
{
    uint8_t ucIndex = 0U;
    uint32_t uiLen = 0U;
    OPCVarInfo_t *pstOPCVarInfo = NULL;

    /* Access Control Info. */
    InitAccessCtrlInfo( &s_stAccCtrlInfo );

    /* Net Configuration Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_NETWORK_PORT); ucIndex++ )
    {
        InitNetInfo( &s_stNetInfo[ucIndex]);
    }

    /* OPC Variable Table */
    uiLen = sizeof(OPCVarTblInfo_t) - sizeof(OPCVarInfo_t)*(uint32_t)(LOCAL_CFG_VER_CNT);
    memset( &s_stOPCVarTblInfo, 0, uiLen );

    /* Flag */
    s_stOPCVarTblInfo.bValid = false;
    s_stOPCVarTblInfo.bCopying = false;

    /* Local Configuration */
    for( ucIndex = 0U; ucIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucIndex++ )
    {
        pstOPCVarInfo = &( s_stOPCVarTblInfo.stOPCVarInfo[ucIndex]);
        uiLen = sizeof(OPCVarInfo_t) - sizeof(OPCVar_t)*(uint32_t)(MAX_OPC_NUM);
        memset( pstOPCVarInfo, 0, uiLen );

        pstOPCVarInfo->emActive = NOT_ACTIVE;
    }

    /* OPC Configuration Copy */
    s_uiOPCCfgOffset = 0U;

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
    uint8_t ucIndex = 0U;
    NetworkPortType_t emNet = NET1;
    OPCNetworkInfo_t *pstNetInfo = NULL;

    /* Access Control */
    if( HandleAccessCtrlInfo( &s_stAccCtrlInfo ))
    {
        DEBUG_OPC( "Access Control Cfg Switch\n" );
        for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_OPC_NUM); ucIndex++ )
        {
            UpdateAccessAuth( &s_stOPCInfo[ucIndex]);
        }
    }

    /* Network Configuration */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        pstNetInfo = &s_stNetInfo[emNet];
        if( HandleNetworkInfo( emNet, pstNetInfo ))
        {
            DEBUG_OPC( "OPC Net[%d] Cfg Switch\n", emNet+1 );
        }
    }

    /* OPC Variable Table */
    if( HandleOPCVarTblInfo())
    {
        DEBUG_OPC( "OPCVarTable Cfg Switch\n" );

        /* Reset */
        for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_OPC_NUM); ucIndex++ )
        {
            s_stOPCInfo[ucIndex].usVarIndex = 0U;  /* Variable Index */
            s_stOPCInfo[ucIndex].usMsgIndex = 0U;  /* Message Index */
        }

        /* Calculate Total Message Number */
        s_usTotalMsgNum = CalcTotalMsgNum( &s_stOPCVarTblInfo );
        DEBUG_OPC( "OPC TotalMsgNum: %d\n", s_usTotalMsgNum );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleOPCVarTblInfo
*
* 功能描述: 网口配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项:
* 1. 实时数据版本变化时，更新本地保存的实时数据版本；
* 2. 为了保证OPC配置与实时数据的一致性：拷贝完成后，无论OPC配置参数是否有效，均需进行切换操作 -》当配置参数无效时，无法读取OPC变量。
***********************************************************************************************************************
*/

static bool_t HandleOPCVarTblInfo( void )
{
    bool_t bSwitch = false;
    bool_t bComplete = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;

    if( s_uiOPCCfgOffset > 0U )
    {
        /* Continue Copy */
        if( !GetOPCConfigInfo( &bComplete ))
        {
            DEBUG_OPC( "GetOPCConfigInfo Failed\n" );
            ClearCurOPCCfg();
        }
        else
        {
            //DEBUG_OPC( "GetOPCConfigInfo Successfully!\n" );
        }
    }
    else
    {
        /* First Try To Copy */
        /* RTData Version */
        /*if( HandleRTDataVer( &s_stOPCVarTblInfo.stRTDataVer ))*/
        {
            /* Get OPC Configuration Version */
            if( SysGetOPCConfigVersion( &uiCRC, &uiFileVer ))
            {
                /* Compare Version */
                if((uiCRC != s_stOPCVarTblInfo.uiOPCCfgCRC) ||
                   (uiFileVer != s_stOPCVarTblInfo.uiOPCCfgVer))
                {
                    DEBUG_OPC( "Start Copy OPCVarTblInfo\n" );

                    /* Start Copy */
                    s_stOPCVarTblInfo.bCopying = true;

                    /* Switch */
                    SwitchOPCVarCfgInfo();

                    /* Clear Current OPC Configuration */
                    ClearCurOPCCfg();

                    /* Update Version Info. */
                    s_stOPCVarTblInfo.uiOPCCfgCRC = uiCRC;
                    s_stOPCVarTblInfo.uiOPCCfgVer = uiFileVer;

                    /* Copy OPC Variable Configuration Info. */
                    if( !GetOPCConfigInfo( &bComplete ))
                    {
                        DEBUG_OPC( "GetOPCConfigInfo Failed\n" );
                        ClearCurOPCCfg();
                    }
                    else
                    {
                        //DEBUG_OPC( "GetOPCConfigInfo OK\n" );
                    }
                }
            }
        }
    }

    if( bComplete )
    {
        if( !CheckOPCVarCfgInfo())
        {
            /* Invalid */
            s_stOPCVarTblInfo.bValid = false;
            ClearCurOPCCfg();
            DEBUG_OPC( "OPCVarTable Cfg Invalid\n" );
        }
        else
        {
            /* Valid */
            s_stOPCVarTblInfo.bValid = true;
            bSwitch = true;
        }

        s_stOPCVarTblInfo.bCopying = false;
        s_uiOPCCfgOffset = 0U;
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchOPCVarCfgInfo
*
* 功能描述: 更新索引值
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

static void SwitchOPCVarCfgInfo( void )
{
    uint8_t ucCfgIndex = s_stOPCVarTblInfo.ucIndex;  /* Current Configuration Index */
    if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
    {
        /* Switch Version */
        ucCfgIndex = (ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);

        /* Update Index */
        s_stOPCVarTblInfo.ucIndex = ucCfgIndex;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearCurOPCCfg
*
* 功能描述: 清除当前OPC配置
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

static void ClearCurOPCCfg( void )
{
    OPCVarInfo_t *pstOPCVarInfo = NULL;
    uint8_t ucCfgIndex = s_stOPCVarTblInfo.ucIndex;  /* Current Configuration Index */

    if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
    {
        pstOPCVarInfo = &( s_stOPCVarTblInfo.stOPCVarInfo[ucCfgIndex]);
        pstOPCVarInfo->emActive = NOT_ACTIVE;
        pstOPCVarInfo->uiCRC = 0U;
        pstOPCVarInfo->uiFileVern = 0U;
        pstOPCVarInfo->uiVarNum = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetOPCConfigInfo
*
* 功能描述: 将OPC配置信息拷贝到本地
*
* 输入参数:
*
* 输出参数: pbComplete：是否将所有有效OPC配置拷贝到本地
*
* 返 回 值: 拷贝操作是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetOPCConfigInfo( bool_t *pbComplete )
{
    bool_t bResult = false;
    bool_t bCopyCont = false;   /* If Copy Content */
    puint8_t pucBuff = NULL;    /* Buffer Pointer */
    uint8_t ucCfgIndex = 0U;    /* Current Configuration Index */
    uint32_t uiDstLen = 0U;     /* Destination Copy Length */
    uint32_t uiCopyLen = 0U;    /* Real Copy Length */
    uint32_t uiHeaderLen = 0U;  /* Header Length */
    uint32_t uiContLen = 0U;    /* Content Length */
    OPCVarInfo_t *pstOPCVarInfo = NULL;

    if( pbComplete != NULL )
    {
        *pbComplete = false;

        /* Get First Address */
        ucCfgIndex = s_stOPCVarTblInfo.ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstOPCVarInfo = &( s_stOPCVarTblInfo.stOPCVarInfo[ucCfgIndex]);
            pucBuff = (puint8_t)pstOPCVarInfo;
            uiHeaderLen = sizeof(OPCVarInfo_t) - sizeof(OPCVar_t)*(uint32_t)(MAX_OPC_NUM);

            if( 0U == s_uiOPCCfgOffset )
            {
                /* Copy Header */
                uiCopyLen = SysGetOPCVarConfigInfo( pucBuff, s_uiOPCCfgOffset, uiHeaderLen );
                if((uiCopyLen > 0U) &&
                   (uiCopyLen == uiHeaderLen))
                {
                    /* Copy Successfully */
                    //DEBUG_OPC( "Copy OPCVarInfo Header Successfully, VarNum: %d.\n", pstOPCVarInfo->uiVarNum );
                    bResult = true;
                    s_uiOPCCfgOffset += uiCopyLen;
                    if( 0U == pstOPCVarInfo->uiVarNum )
                    {
                        *pbComplete = true;
                    }
                    else
                    {
                        /* Copy Content */
                        bCopyCont = true;
                    }
                }
                else
                {
                    bResult = false;
                    s_uiOPCCfgOffset = 0U;
                }
            }
            else
            {
                /* Copy Content */
                bCopyCont = true;
            }

            if( bCopyCont )
            {
                uiContLen = sizeof(OPCVar_t) * pstOPCVarInfo->uiVarNum;
                uiContLen = uiContLen - (s_uiOPCCfgOffset - uiHeaderLen);
                if( uiContLen > (uint32_t)(OPC_MAX_CYCLE_COPY_LEN))
                {
                    uiDstLen = (uint32_t)(OPC_MAX_CYCLE_COPY_LEN);
                }
                else
                {
                    uiDstLen = uiContLen;
                }

                /*DEBUG_OPC( "GetOPCConfigInfo: Offset: %d, Remain: %d, Dst Copy Len: %d.\n", \
                           s_uiOPCCfgOffset, uiContLen, uiDstLen );*/

                /* Copy Content */
                uiCopyLen = SysGetOPCVarConfigInfo( &pucBuff[s_uiOPCCfgOffset], s_uiOPCCfgOffset, uiDstLen );
                if( uiCopyLen > 0U )
                {
                    /* Copy Successfully */
                    bResult = true;
                    s_uiOPCCfgOffset += uiCopyLen;
                    if( uiContLen == uiCopyLen )
                    {
                        *pbComplete = true;
                    }
                }
                else
                {
                    bResult = false;
                    s_uiOPCCfgOffset = 0U;
                }
            }
        }
        else
        {
            bResult = false;
            s_uiOPCCfgOffset = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CheckOPCVar
*
* 功能描述: 检查某一OPC变量是否有效
*
* 输入参数: pstOPCVar：指向待检查的OPC变量
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckOPCVar( OPCVar_t const *pstOPCVar )
{
    bool_t bValid = false;
    uint32_t uiDataAreaLen = 0U;
    uint32_t uiOffset = 0U;

    if( pstOPCVar != NULL )
    {
        if((pstOPCVar->ucRefID < REF_NUM) &&
           (pstOPCVar->usVarSize <= (uint16_t)(MAX_VAR_SIZE)))
        {
            if((0U == pstOPCVar->usVarSize) ||
               (1U == pstOPCVar->usVarSize) ||
               (2U == pstOPCVar->usVarSize) ||
               (4U == pstOPCVar->usVarSize) ||
               (8U == pstOPCVar->usVarSize))
            {
                /* Section Size */
                uiDataAreaLen = GetDataAreaLen((SysRtDataAreaType_t)pstOPCVar->ucRefID );
                if( uiDataAreaLen > 0U )
                {
                    /* Offset */
                    if( 0U == pstOPCVar->usVarSize )
                    {
                        /* Offset: bit -> byte */
                        uiOffset = pstOPCVar->uiOffset / 8;
                    }
                    else
                    {
                        /* Offset: byte */
                        uiOffset = pstOPCVar->uiOffset;
                    }

                    if((uiOffset + (uint32_t)pstOPCVar->usVarSize) <= uiDataAreaLen )
                    {
                        bValid = true;
                    }
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckOPCVarCfgInfo
*
* 功能描述: 检查OPC配置信息是否有效
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: OPC配置信息是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckOPCVarCfgInfo( void )
{
    bool_t bValid = false;
    uint32_t uiIndex = 0U;
    uint32_t uiItemNum = 0U;
    uint8_t ucCfgIndex = s_stOPCVarTblInfo.ucIndex;
    OPCVarInfo_t const *pstOPCVarInfo = NULL;
    OPCVar_t const *pstOPCVar = NULL;

    if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
    {
        pstOPCVarInfo = &( s_stOPCVarTblInfo.stOPCVarInfo[ucCfgIndex]);

        if((ACTIVE == pstOPCVarInfo->emActive) ||
           (NOT_ACTIVE == pstOPCVarInfo->emActive))
        {
            if( ACTIVE == pstOPCVarInfo->emActive )
            {
                /* Header: Item Number */
                uiItemNum = pstOPCVarInfo->uiVarNum;
                if( uiItemNum <= (uint32_t)(MAX_OPC_NUM))
                {
                    bValid = true;

                    /* Check OPC Variable Info. */
                    for( uiIndex = 0U; uiIndex < uiItemNum; uiIndex++ )
                    {
                        pstOPCVar = &( pstOPCVarInfo->stOPCVar[uiIndex]);
                        if( !CheckOPCVar( pstOPCVar ))
                        {
                            bValid = false;
                            break;
                        }
                    }/* end of for */
                }
            }
            else
            {
                bValid = true;
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurOPCVarInfo
*
* 功能描述: 获取当前使用的 OPC变量表配置
*
* 输入参数: pstOPCCfg：指向OPC配置参数信息
*
* 输出参数: 无
*
* 返 回 值: 指向当前使用的配置参数，当前配置参数非Active时，返回NULL。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static OPCVarInfo_t const* GetCurOPCVarInfo( OPCVarTblInfo_t const *pstOPCCfg )
{
    OPCVarInfo_t const *pstOPCVarInfo = NULL;
    uint8_t ucCfgIndex = 0U;

    if( pstOPCCfg != NULL )
    {
        if((!pstOPCCfg->bCopying) &&
           (pstOPCCfg->bValid))
        {
            ucCfgIndex = pstOPCCfg->ucIndex;
            if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
            {
                pstOPCVarInfo = &( pstOPCCfg->stOPCVarInfo[ucCfgIndex]);
                if( pstOPCVarInfo->emActive != ACTIVE )
                {
                    pstOPCVarInfo = NULL;
                }
            }
        }
    }

    return pstOPCVarInfo;
}

/*
***********************************************************************************************************************
* 函数名称: HandleNetworkInfo
*
* 功能描述: 网口配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: emNet：网口号；
*          pstNetInfo：指向本地保存的网口配置信息；
*          puiCRC：指向本地保存的CRC信息；
*          puiFileVer：指向本地保存的文件版本信息。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleNetworkInfo( NetworkPortType_t emNet, OPCNetworkInfo_t *pstNetInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    OPCNetConfigInfo_t stNetCfgInfo;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstNetInfo != NULL))
    {
        /* Get CM Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstNetInfo->uiCMCfgCRC) ||
               (uiFileVer != pstNetInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysOPCGetNetworkConfigInfo( emNet, &stNetCfgInfo ))
                {
                    /* Check If Active & update(if has flag) */
                    if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                       ( ACTIVE == stNetCfgInfo.emActive ) &&
                       ( stNetCfgInfo.bUpdate ))
                    {
                        /* Check Valid */
                        if( CheckNetCfgInfo( &stNetCfgInfo ))
                        {
                            /* Switch */
                            if( SwitchNetCfgInfo( pstNetInfo, &stNetCfgInfo ))
                            {
                                bSwitch = true;
                                pstNetInfo->uiCMCfgCRC = uiCRC;
                                pstNetInfo->uiCMCfgVer = uiFileVer;
                            }
                        }
                        else
                        {
                            pstNetInfo->uiCMCfgCRC = uiCRC;
                            pstNetInfo->uiCMCfgVer = uiFileVer;
                            DEBUG_OPC( "OPC Net[%d] Cfg Invalid\n", emNet+1 );
                        }
                    }
                }
            }
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckNetCfgInfo
*
* 功能描述: 检查网口配置信息是否有效
*
* 输入参数: pstNetCfgInfo：指向待切换的网口配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckNetCfgInfo( OPCNetConfigInfo_t const *pstNetCfgInfo )
{
    bool_t bValid = false;

    if( pstNetCfgInfo != NULL )
    {
        /* Active Flag: ACTIVE/NOT_ACTIVE */
        if((ACTIVE == pstNetCfgInfo->emActive) ||
           (NOT_ACTIVE == pstNetCfgInfo->emActive))
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchNetCfgInfo
*
* 功能描述: 切换网口配置信息
*
* 输入参数: pstNetInfo：指向本地保存的网口信息；
*          pstNetCfgInfo：指向待切换的网口配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchNetCfgInfo( OPCNetworkInfo_t *pstNetInfo, OPCNetConfigInfo_t const *pstNetCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    OPCNetConfigInfo_t *pstDstNetCfgInfo = NULL;

    if((pstNetInfo != NULL) &&
       (pstNetCfgInfo != NULL))
    {
        ucCfgIndex = pstNetInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = (ucCfgIndex+1U) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstNetCfgInfo = &( pstNetInfo->stNetConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstNetCfgInfo, pstNetCfgInfo, sizeof(OPCNetConfigInfo_t));

            /* Update Index */
            pstNetInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
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
    if( ucSWIndex < (uint8_t)(APP_SW_OPC_NUM))
    {
        if( SysGetAppSwConnectFlag( APP_OPC, ucSWIndex))  /* Connect */
        {
            s_stOPCInfo[ucSWIndex].uiWaitReqCycleCnt++;

            /* Connect & Timeout: Clear */
            if( s_stOPCInfo[ucSWIndex].uiWaitReqCycleCnt >= (uint32_t)(OPC_SW_TIMEOUT_CYCLE_CNT))
            {
                DEBUG_OPC( "OPC-%d Timeout\n", ucSWIndex );

                /* Clear Local Info */
                ClearOPCLocalInfo( ucSWIndex );

                /* Clear System Info */
                ClearOPCSysInfo( ucSWIndex );
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

    if( ucSWIndex < (uint8_t)(APP_SW_OPC_NUM))
    {
        if( !SysGetAppSwConnectFlag( APP_OPC, ucSWIndex ))  /* Disconnect */
        {
            if( s_stOPCInfo[ucSWIndex].uiIPAddr != 0U )
            {
                DEBUG_OPC( "OPC-%d Disconnected\n", ucSWIndex );

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
                    ClearOPCLocalInfo( ucSWIndex );  /* Clear Local Info */
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

    while( ucCheckCnt < (uint8_t)(APP_SW_OPC_NUM))
    {
        /* Read Request Message */
        s_stReqMsg.stNetHead.usLen = 0U;
        usLen = (uint16_t)sizeof( NetMsg_t );
        if( !SysGetAppMsg( APP_OPC, s_ucCurIndex, APP_MSG_REQ, (puint8_t)&s_stReqMsg, &usLen ))
        {
            usLen = 0U;
        }

        if((usLen > 0U) && (s_stReqMsg.stNetHead.usLen > 0U))
        {
            /* Has Message */
            s_stOPCInfo[s_ucCurIndex].uiWaitReqCycleCnt = 0U;  /* Clear wait request count */
            ucSubReason = CheckCmd( &s_stReqMsg, &s_stOPCInfo[s_ucCurIndex]);
            if((uint8_t)(ERR_SUCCESS) == ucSubReason )
            {
                /* Valid */
                DEBUG_OPC( "HandleCmd: Msg Valid\n" );
                if( CheckPMHandle( s_stReqMsg.ucBuff ))
                {
                    /* PM Handle */
                    DEBUG_OPC( "HandleCmd: PM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));
                    PMHandle( &s_stOPCInfo[s_ucCurIndex], &s_stReqMsg, &s_stRespMsg, s_bCMHasCfg );
                }
                else
                {
                    /* CM Handle */
                    DEBUG_OPC( "HandleCmd: CM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));

                    /* Decode Cmd */
                    DecodeCmd( &s_stOPCInfo[s_ucCurIndex], s_stReqMsg.ucBuff, &s_stRespMsg, s_bCMHasCfg );
                    s_emResp = CM_RESP;
                }
            }
            else
            {
                /* Invalid */
                DEBUG_OPC( "HandleCmd: Msg Invalid\n" );

                /* Make Response Message */
                if( !MakeRespMsg( &s_stOPCInfo[s_ucCurIndex], ucSubReason, s_stReqMsg.ucBuff, &s_stRespMsg ))  /* Make Response Message */
                {
                    DEBUG_OPC( "MakeRespMsg Err\n" );
                }

                s_emResp = CM_RESP;
            }

            /* Lock & Busy */
            if( !SysSetAppSwLockedFlag( APP_OPC, s_ucCurIndex ))
            {
                DEBUG_OPC( "HandleCmd: Set Locked Flag Failed\n" );
            }

            s_emState = BUSY_STATE;
            break;
        }
        else
        {
            /* No Message */
            s_ucCurIndex++;
            s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_OPC_NUM);
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

static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, OPCInfo_t *pstSWInfo )
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
                DEBUG_OPC( "CheckCmd: SrcAddr Invalid\n" );
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
                DEBUG_OPC( "CheckCmd: Type-0x%X != 0x%X\n", \
                           pucReq[APP_REQ_TYPE_INDEX], (uint8_t)(APP_REQ_TYPE));
                ucSubReason = (uint8_t)(ERR_TYPE);  /* Type */
            }
            else if( pucReq[APP_REQ_PROTECT_INDEX] != (uint8_t)(APP_REQ_PROTECT))
            {
                DEBUG_OPC( "CheckCmd: Protect-%d Invalid\n", pucReq[APP_REQ_PROTECT_INDEX]);
                ucSubReason = (uint8_t)(ERR_PROTECT);  /* Protect */
            }
            else
            {
                /* Actual Type */
                usAType = GetReqActualType( pucReq );
                if((usAType < (uint16_t)(OPC_ATYPE_MIN)) ||
                   (usAType > (uint16_t)(OPC_ATYPE_MAX)))
                {
                    DEBUG_OPC( "CheckCmd: Actual Type-%d, HI-%d, LOW-%d Invalid\n", \
                               usAType, pucReq[APP_REQ_ATYPE_HIINDEX], pucReq[APP_REQ_ATYPE_LOWINDEX]);
                    ucSubReason = (uint8_t)(ERR_ACTUAL_TYPE);
                }
                /*else if( pucReq[APP_RESP_STAID_INDEX] != s_ucLocalStaID )
                {
                    DEBUG_OPC( "CheckCmd: StaID-%d Invalid\n", pucReq[APP_RESP_STAID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_STAID);
                }*/
                else if( pucReq[APP_REQ_PMID_INDEX] != (uint8_t)(APP_REQ_PMID))
                {
                    DEBUG_OPC( "CheckCmd: PMID-%d Invalid\n", pucReq[APP_REQ_PMID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_PMID);  /* PM ID */
                }
                else if( pucReq[APP_REQ_LAST_INDEX] != (uint8_t)(APP_REQ_LAST_FLAG))
                {
                    DEBUG_OPC( "CheckCmd: Last Flag-%d Invalid\n", pucReq[APP_REQ_LAST_INDEX]);
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

    if( pucReq != NULL )
    {
        usAType = GetReqActualType( pucReq );
        if(((uint16_t)(OPC_LOGIN) == usAType) ||
           ((uint16_t)(OPC_LOGOUT) == usAType) ||
           ((uint16_t)(OPC_READ) == usAType))
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
* 输入参数: pstSWInfo：OPC软件本地信息；
*          ucSubReason：第二级响应码；
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

static bool_t MakeRespMsg( OPCInfo_t *pstSWInfo, uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    bool_t bResult = false;
    bool_t bCMBus = false;
    uint16_t usDataLen = 0U;
    puint8_t pucResp = NULL;
    uint16_t usLength = 0U;   /* Length Of Header + Data Area */
    uint16_t usAType = 0U;    /* Actual Request Message Type */
    uint16_t usIndex = 0U;
    uint32_t uiCalcCRC = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        bResult = true;
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

            if(((uint16_t)(OPC_LOGIN) == usAType) ||
               ((uint16_t)(OPC_LOGOUT) == usAType))
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
                usDataLen = AddOPCOnlineData( pstSWInfo, &pucResp[usIndex], &bCMBus );
                if((usDataLen > 0U) && (usDataLen <= (uint16_t)(MAX_APP_MSG_DATA_LEN)))
                {
                    usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;

                    /* Set Error Info. */
                    if( !bCMBus )
                    {
                        /* CM Bus Error */
                        pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_FAILED);
                        pucResp[APP_RESP_SUBREA_INDEX] = (uint8_t)(ERR_CMBUS);
                    }
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_ADD_OPCONLINE_DATA);
                    DEBUG_OPC( "MakeRespMsg: AddOPCOnlineData Failed\n" );
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
        pucResp[usIndex++] = (uint8_t)( uiCalcCRC & 0xFF);

        /* Update Response Message Length */
        pstResp->usLen = usLength + (uint16_t)(APP_MSG_CRC_LEN);
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: AddOPCOnlineData
*
* 功能描述: 添加OPC变量在线值
*
* 输入参数: pstSWInfo：本地记录的OPC软件信息；
*          pucRespData：应答帧数据域首字节地址。
*
* 输出参数: pbCMBus：用于返回内部通讯的状态
*
* 返 回 值: 添加的数据长度，即Data Area的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddOPCOnlineData( OPCInfo_t *pstSWInfo, uint8_t pucRespData[], bool_t *pbCMBus )
{
    uint8_t ucValue = 0U;         /* Data Value */
    uint16_t usDataIndex = 0U;    /* Response Data Index */
    uint16_t usVarIndex = 0U;     /* Variable Index */
    uint16_t usAddVarCnt = 0U;    /* Added Variable Counts */
    uint16_t usValueLen = 0U;     /* Value Length */
    uint32_t uiOffset = 0U;       /* Byte Offset */
    SysSystemTime_t stTime;       /* Local System Time */
    OPCReadRespHead_t stRespHead; /* Response Head */
    OPCVarInfo_t const *pstOPCVarInfo = NULL;
    OPCVar_t const *pstVar = NULL;
    uint8_t const *pucGAddr = NULL;
    uint8_t const *pucIAddr = NULL;
    uint8_t const *pucQAddr = NULL;
    uint8_t const *pucVarAddr = NULL;

    if((pstSWInfo != NULL) &&
       (pucRespData != NULL) &&
       (pbCMBus != NULL))
    {
        /* Current OPC Variable Table */
        pstOPCVarInfo = GetCurOPCVarInfo( &s_stOPCVarTblInfo );
        if( pstOPCVarInfo != NULL )
        {
            /* Select PM RT Data */
            if( SelectRTData( &s_emLastPMID, pbCMBus ))
            {
                pucGAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_G_AREA );
                pucIAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_I_AREA );
                pucQAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_Q_AREA );
                if((pucGAddr != NULL) &&
                   (pucIAddr != NULL) &&
                   (pucQAddr != NULL))
                {
                    /* Loop Variable: Add its value */
                    usDataIndex = (uint16_t)sizeof( OPCReadRespHead_t );
                    for( usVarIndex = pstSWInfo->usVarIndex; usVarIndex < (uint16_t)pstOPCVarInfo->uiVarNum; usVarIndex++ )
                    {
                        pstVar = &( pstOPCVarInfo->stOPCVar[usVarIndex]);
                        if( 0U == pstVar->usVarSize )
                        {
                            uiOffset = pstVar->uiOffset / 8U;  /* Bit -> Byte */
                        }
                        else
                        {
                            uiOffset = pstVar->uiOffset;
                        }

                        if((uint8_t)RTDATA_G_AREA == pstVar->ucRefID )
                        {
                            pucVarAddr = &pucGAddr[uiOffset];
                        }
                        else if((uint8_t)RTDATA_I_AREA == pstVar->ucRefID )
                        {
                            pucVarAddr = &pucIAddr[uiOffset];
                        }
                        else
                        {
                            pucVarAddr = &pucQAddr[uiOffset];
                        }

                        /* Length */
                        if( 0U == pstVar->usVarSize )
                        {
                            usValueLen += 1U;
                        }
                        else if((1U == pstVar->usVarSize) ||
                                (2U == pstVar->usVarSize) ||
                                (4U == pstVar->usVarSize) ||
                                (8U == pstVar->usVarSize))
                        {
                            usValueLen += pstVar->usVarSize;
                        }
                        else
                        {
                            usDataIndex = 0U;
                            break;
                        }

                        if( usValueLen <= (uint16_t)(OPC_MAX_READ_LEN))
                        {
                            usAddVarCnt++;

                            /* Set Value */
                            if( 0U == pstVar->usVarSize )
                            {
                                /* Offset: Bit */
                                ucValue = *pucVarAddr;
                                if((ucValue & (0x01U << (pstVar->uiOffset % 8U))) != 0U )
                                {
                                    pucRespData[usDataIndex++] = 1U;
                                }
                                else
                                {
                                    pucRespData[usDataIndex++] = 0U;
                                }
                            }
                            else
                            {
                                memcpy( &pucRespData[usDataIndex], pucVarAddr, (uint32_t)pstVar->usVarSize );
                                usDataIndex += pstVar->usVarSize;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }/* end of for */

                    /* Set If No Error */
                    if( usDataIndex > 0U )
                    {
                        /* System Time */
                        stTime = SysGetSystemTime();

                        /* Response Message Head */
                        memset( &stRespHead, 0, sizeof(OPCReadRespHead_t));
                        stRespHead.uiSec = stTime.uiSecond;            /* Second */
                        stRespHead.usMilliSec = stTime.usMiliSecond;   /* Millisecond */
                        stRespHead.usSeq = pstSWInfo->usSequence++;    /* Sequence */
                        stRespHead.usTotalNum = s_usTotalMsgNum;       /* Total Message Number */
                        stRespHead.usMsgIndex = pstSWInfo->usMsgIndex; /* Message Index */
                        stRespHead.usVarCnts = usAddVarCnt;            /* Variable Count */
                        stRespHead.usFirIndex = pstSWInfo->usVarIndex; /* Variable Index */

                        /* Response Head */
                        memcpy( &pucRespData[0U], &stRespHead, sizeof(OPCReadRespHead_t));

                        DEBUG_OPC( "AddOnlineData: Seq: %d, TotalNum: %d, MsgIndex: %d, VarCnts: %d, FirIndex: %d\n", \
                                   stRespHead.usSeq, stRespHead.usTotalNum, stRespHead.usMsgIndex, \
                                   stRespHead.usVarCnts, stRespHead.usFirIndex );

                        /* Update Variable Index */
                        pstSWInfo->usVarIndex += usAddVarCnt;
                    }
                }
                else
                {
                    DEBUG_OPC( "SysGetRTDataAddr Err\n" );
                }
            }
        }
    }

    return usDataIndex;
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

static void PMHandle( OPCInfo_t *pstSWInfo, NetMsg_t *pstReqMsg, RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    SelPMID_t emSelPMID = SELPM_NONE;

#if 0
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
    uint16_t usPrintLen = 0U;
#endif

    if(( pstSWInfo != NULL ) &&
       ( pstReqMsg != NULL ) &&
       ( pstResp != NULL ))
    {
        if( pstSWInfo->emLog != LOGIN )
        {
            DEBUG_OPC( "PMHandle: OPC should login first\n" );
            ucSubReason = (uint8_t)(ERR_LOG);  /* Need To Login First */
        }
        else
        {
            if( bCMHasCfg )
            {
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_OPC( "PMHandle: Access Authority - Deny\n" );
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
                }
                else if((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_OPC( "PMHandle: Access Authority - Readonly\n" );
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
                }
                else
                {
                    /* Port Write Enable */
                    if( CheckPortWriteEnable( pstSWInfo->emNetwork, s_stNetInfo ))
                    {
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        DEBUG_OPC( "PMHandle: Net[%d] Not Writable\n", pstSWInfo->emNetwork+1 );
                        ucSubReason = (uint8_t)(ERR_ACCESS);
                    }
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
            emSelPMID = SelectPM( pstSWInfo );
            if( SELPM_NONE == emSelPMID )
            {
                DEBUG_OPC( "PMHandle: CM BUS Invalid\n" );
                ucSubReason = (uint8_t)(ERR_CMBUS);   /* CM BUS Error */
            }
            else
            {
                /* Write Req To The Selected PM */
                if( SysSetMsg( SwitchPMID( emSelPMID ), OPC_REQ, pstReqMsg->ucBuff, pstReqMsg->stNetHead.usLen ))
                {
                    /* PM Response */
#if 0
                    /* Print Request Message */
                    {
                        printf( "PMHandle: MSGLen %d, Data(Max Top 50B): \n", pstReqMsg->stNetHead.usLen );
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
                    DEBUG_OPC( "PMHandle: PM Resp\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                    s_emResp = PM_RESP;
                    s_uiWaitPMCycleCnt = 0U;
                }
                else
                {
                    DEBUG_OPC( "PMHandle: SysSetMsg Failed, SelPMID-%d\n", emSelPMID );
                    ucSubReason = (uint8_t)(ERR_SYSWREQTOPM);  /* 将请求写入系统资源失败 - 用于发往PM模块 */
                }
            }
        }

        if( ucSubReason != (uint8_t)(ERR_SUCCESS))
        {
            /* CM Response */
            DEBUG_OPC( "PMHandle: CM Resp\n" );

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pstReqMsg->ucBuff, pstResp ))
            {
                DEBUG_OPC( "MakeRespMsg Err\n" );
            }

            s_emResp = CM_RESP;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckPortWriteEnable
*
* 功能描述: 检查网口是否写使能
*
* 输入参数：emNet：网口索引号
*
* 输出参数: 无
*
* 返 回 值: 是否写使能：true，写使能
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckPortWriteEnable( NetworkPortType_t emNet, OPCNetworkInfo_t const *pstNetInfo )
{
    bool_t bEnable = false;
    uint8_t ucCfgIndex = 0U;                         /* Current Configuration Index */
    OPCNetConfigInfo_t const *pstNetCfgInfo = NULL;  /* Current Configuration */

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstNetInfo != NULL))
    {
        ucCfgIndex = pstNetInfo[emNet].ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstNetCfgInfo = &( pstNetInfo[emNet].stNetConfig[ucCfgIndex]);
            bEnable = pstNetCfgInfo->bPortWriteEnable;
        }
    }

    return bEnable;
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

static SelPMID_t SelectPM( OPCInfo_t *pstSWInfo )
{
    SelPMID_t emSelPMID = SELPM_NONE;

    if( pstSWInfo != NULL )
    {
        /* Select */
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

        /* Record Select Result */
        pstSWInfo->emSelPMID = emSelPMID;
    }

    return emSelPMID;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateAccessAuth
*
* 功能描述: 更新相应OPC软件的访问权限
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

static void UpdateAccessAuth( OPCInfo_t *pstSWInfo )
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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucOPC;  /* OPC */
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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucOPC;  /* OPC */
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
* 功能描述: 解析OPC软件命令
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

static void DecodeCmd( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );

        switch( usAType )
        {
            case (uint16_t)(OPC_LOGIN):
            {
                DecodeLogin( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(OPC_LOGOUT):
            {
                DecodeLogout( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(OPC_READ):
            {
                DecodeReadOPCOnLine( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            default:
            {
                DEBUG_OPC( "DecodeCmd: Actual Type-%d Invalid\n", usAType );
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

static void DecodeLogin( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;     /* Actual Request Message Type */
    uint8_t ucPSDLen = 0U;     /* Password Length */
    bool_t bPSDValid = false;  /* Password Valid Flag */
    uint8_t ucIndex = 0U;      /* Index */
    uint16_t usDataIndex = 0U; /* Request Data Index */

#ifndef DEBUG_SWOPC
    uint32_t uiOPCCfgCRC = 0U; /* OPC Variable Table Version - CRC */
    uint32_t uiOPCCfgVer = 0U; /* OPC Variable Table Version - Sub-Version */
#endif

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(OPC_LOGIN) == usAType )
        {
            if( bCMHasCfg )
            {
                UpdateAccessAuth( pstSWInfo );
                if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
                {
                    DEBUG_OPC( "IP-0x%08X Login: Deny\n", pstSWInfo->uiIPAddr );
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
                    usDataIndex = (uint16_t)(PSD_LEN_INDEX);
                    if( 0U == ucPSDLen )
                    {
                        bPSDValid = true;  /* No Password */
                    }
                    else if( ucPSDLen == pucReq[usDataIndex])  /* Length */
                    {
                        /* Check Password Content */
                        bPSDValid = true;
                        for( ucIndex = 0U; ucIndex < ucPSDLen; ucIndex++ )
                        {
                            if( s_ucPassword[ucIndex] != pucReq[(uint8_t)(PSD_INDEX)+ucIndex])
                            {
                                bPSDValid = false;
                                DEBUG_OPC( "PWD Err\n" );
                                break;
                            }
                        }
                    }
                    else
                    {
                        bPSDValid = false;
                        DEBUG_OPC( "PWDLen %d != %d\n", pucReq[usDataIndex], ucPSDLen );
                    }

                    if( bPSDValid )
                    {
#ifndef DEBUG_SWOPC
                        /* Compare Version */
                        /* CRC */
                        usDataIndex += (4U + 16U + 4U);  /* Password Length+RFU(1+3) + Password(16) + Station IP(4) */
                        memcpy( &uiOPCCfgCRC, &pucReq[usDataIndex], sizeof(uint32_t));

                        /* Sub-Version*/
                        usDataIndex += 4U;
                        memcpy( &uiOPCCfgVer, &pucReq[usDataIndex], sizeof(uint32_t));

                        if((s_stOPCVarTblInfo.uiOPCCfgCRC == uiOPCCfgCRC) &&
                           (s_stOPCVarTblInfo.uiOPCCfgVer == uiOPCCfgVer))
                        {
                            DEBUG_OPC( "Login Successfully!\n" );
                            ucSubReason = (uint8_t)(ERR_SUCCESS);
                            pstSWInfo->emLog = LOGIN;
                        }
                        else
                        {
                            DEBUG_OPC( "Login Failed: Version Error! CRC: %d, SubVer:%d Should be %d, %d.\n", \
                                       uiOPCCfgCRC, uiOPCCfgVer, s_stOPCVarTblInfo.uiOPCCfgCRC, \
                                       s_stOPCVarTblInfo.uiOPCCfgVer );
                            ucSubReason = (uint8_t)(ERR_OPC_VER);
                            pstSWInfo->emLog = LOGOUT;
                        }
#else
                        DEBUG_OPC( "IP-0x%08X Login OK\n", pstSWInfo->uiIPAddr );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        pstSWInfo->emLog = LOGIN;
#endif
                    }
                    else
                    {
                        DEBUG_OPC( "IP-0x%08X Login Failed: PWD Err\n", pstSWInfo->uiIPAddr );
                        ucSubReason = (uint8_t)(ERR_CONN_PSD);
                        pstSWInfo->emLog = LOGOUT;
                    }
                }
            }
            else
            {
                DEBUG_OPC( "IP-0x%08X Login Failed: CM No Cfg\n", pstSWInfo->uiIPAddr );
                ucSubReason = (uint8_t)(ERR_CMNOCFG);
                pstSWInfo->emLog = LOGOUT;
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_OPC( "MakeRespMsg Err\n" );
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

static void DecodeLogout( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(OPC_LOGOUT) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                DEBUG_OPC( "IP-0x%08X Logout OK\n", pstSWInfo->uiIPAddr );
                pstSWInfo->emLog = LOGOUT;
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_OPC( "MakeRespMsg Err\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadVarOnLine
*
* 功能描述: 解析读变量在限值命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令；
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

static void DecodeReadOPCOnLine( OPCInfo_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(OPC_READ) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                       ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                    {
                        /* OPC Configuration: Copy Finished & Valid */
                        if((!s_stOPCVarTblInfo.bCopying) &&
                           (s_stOPCVarTblInfo.bValid))
                        {
                            DEBUG_OPC( "IP-0x%08X: ReadVarOnlineValue OK\n", pstSWInfo->uiIPAddr );
                            ucSubReason = (uint8_t)(ERR_SUCCESS);
                        }
                        else
                        {
                            DEBUG_OPC( "OPCVarInfo Cfg Invalid\n" );
                            ucSubReason = (uint8_t)(ERR_CONFIG);  /* Configuration Invalid */
                        }
                    }
                    else
                    {
                        DEBUG_OPC( "IP-0x%08X: No Access Authority\n", pstSWInfo->uiIPAddr );
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                    }
                }
                else
                {
                    DEBUG_OPC( "IP-0x%08X: CM No Cfg\n", pstSWInfo->uiIPAddr );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                }
            }
            else
            {
                DEBUG_OPC( "IP-0x%08X Need to login first\n", pstSWInfo->uiIPAddr );
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_OPC( "MakeRespMsg Err\n" );
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
    bool_t bSwitch = false;       /* Switch State Flag */
    OPCInfo_t *pstSWInfo = NULL;  /* Current Software Information */
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    uint16_t usMsgType = 0U;

    if((s_emResp != NONE_RESP) &&
       (s_ucCurIndex < (uint8_t)(APP_SW_OPC_NUM)))
    {
        pstSWInfo = &( s_stOPCInfo[s_ucCurIndex]);
        if( PM_RESP == s_emResp )
        {
            /* Read PM Response Message */
            s_stRespMsg.usLen = (uint16_t)sizeof( s_stRespMsg.ucBuff );
            if( !SysGetMsg( SwitchPMID( pstSWInfo->emSelPMID), OPC_RESP, s_stRespMsg.ucBuff, &s_stRespMsg.usLen ))
            {
                s_stRespMsg.usLen = 0U;
            }

            if( 0U == s_stRespMsg.usLen )
            {
                /* No Response Message */
                if( s_uiWaitPMCycleCnt >= (uint32_t)(OPC_PM_TIMEOUT_CYCLE_CNT))
                {
                    /* Record Error Info.: PM Timeout */
                    DEBUG_OPC( "HandleResp: PMResp Timeout\n" );
                    s_uiWaitPMCycleCnt = 0U;

                    /* Make Response Message */
                    if( !MakeRespMsg( pstSWInfo, (uint8_t)(ERR_PMBUSY), s_stReqMsg.ucBuff, &s_stRespMsg ))
                    {
                        DEBUG_OPC( "MakeRespMsg Err\n" );
                    }

                    s_emResp = CM_RESP;
                }
                else
                {
                    s_uiWaitPMCycleCnt++;
                    DEBUG_OPC( "HandleResp: Wait PMResp-%d\n", s_uiWaitPMCycleCnt );
                }
            }
        }

        if( 0U == s_stRespMsg.usLen )
        {
            if( CM_RESP == s_emResp )  /* CM Response */
            {
                /* Record Error Info: s_stRespMsg.usLen should greater than zero */
                DEBUG_OPC( "HandleResp: CM Response - s_stRespMsg.usLen Invalid\n" );
                if( !SysResetAppSwLockedFlag( APP_OPC, s_ucCurIndex ))
                {
                    DEBUG_OPC( "HandleResp: Reset Locked Flag Failed\n" );
                }

                s_ucCurIndex++;
                s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_OPC_NUM);
                s_emState = IDLE_STATE;
            }
        }
        else
        {
            /* Send Response Message */
            ucResult = Send( s_stRespMsg.ucBuff, s_stRespMsg.usLen,
                             pstSWInfo->uiIPAddr, pstSWInfo->usPort, pstSWInfo->emNetwork,
                             TYPE_OPC, s_ucCurIndex );

            /* Check Send Result */
            if( ucResult != (uint8_t)(SEND_SUCCESS))
            {
                bSwitch = true;  /* Switch To Next Software */

                /* Clear Local Info */
                ClearOPCLocalInfo( s_ucCurIndex );

                /* Clear System Info */
                ClearOPCSysInfo( s_ucCurIndex );

                /* Send Failed: Record Error Info */
                DEBUG_OPC( "HandleResp: ExtCommSend Failed\n" );
            }

            /* 读变量：判断是否已经应答完所有的变量，若是，则下一个软件，IDLE；
             * 否则，构造应答帧，继续应答 */
            if( !bSwitch )
            {
                bSwitch = true;
                if( CM_RESP == s_emResp )
                {
                    usMsgType = ((s_stReqMsg.ucBuff[APP_REQ_ATYPE_HIINDEX] << 8) |
                                  s_stReqMsg.ucBuff[APP_REQ_ATYPE_LOWINDEX]);
                    if((uint32_t)(OPC_READ) == usMsgType )
                    {
                        pstSWInfo->usMsgIndex++;
                        if( pstSWInfo->usMsgIndex < s_usTotalMsgNum )
                        {
                            bSwitch = false;
                        }
                        else
                        {
                            /* Done */
                            pstSWInfo->usMsgIndex = 0U;
                            pstSWInfo->usVarIndex = 0U;
                        }
                    }
                }
            }

            if( bSwitch )
            {
                /* Reset Lock Flag */
                if( !SysResetAppSwLockedFlag( APP_OPC, s_ucCurIndex ))
                {
                    DEBUG_OPC( "HandleResp: Reset Locked Flag Failed\n" );
                }

                s_ucCurIndex++;
                s_ucCurIndex = s_ucCurIndex % (uint8_t)(APP_SW_OPC_NUM);
                s_emState = IDLE_STATE;
            }
            else
            {
                s_stRespMsg.usLen = 0U;
                if( !MakeRespMsg( pstSWInfo, ERR_SUCCESS, s_stReqMsg.ucBuff, &s_stRespMsg ))
                {
                    DEBUG_OPC( "MakeRespMsg Err\n" );
                }
            }
        }
    }/* end of if( s_emResp != NONE_RESP ) */
    else
    {
        DEBUG_OPC( "HandleResp: s_emResp-%d / s_ucCurIndex-%d Invalid\n", s_emResp, s_ucCurIndex );
        s_emState = IDLE_STATE;
    }

    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

