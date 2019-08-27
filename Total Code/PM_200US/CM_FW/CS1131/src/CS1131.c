/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CS1131软件应用模块
*
*  文件名称    : CS1131.c
*
*  功能简介    : 处理CS1131软件命令和相关配置
*
*  作者        : 王东
*
*  创建日期    : 2016-01-15
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../inc/CS1131.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                              /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                          /* 本控制站的ID */
static uint8_t s_ucCurSWIndex;                          /* 当前CS1131软件的索引号 */
static ModuleState_t s_emState;                         /* 状态 */
static uint32_t s_uiWaitPMCycleCnt;                     /* 已等待PM应答的拍数 */
static RespModuleType_t s_emResp;                       /* CS1131命令被哪个模块应答 */
static CS1131Info_t s_stCS1131Info[APP_SW_CS1131_NUM];  /* CS1131软件信息，数组成员与CS1131软件一一对应 */
static NetMsg_t s_stReqMsg;                             /* CS1131软件命令 */
static RespMsg_t s_stRespMsg;                           /* 应答消息 */
static PMController_t s_emLastPMID;                     /* 读变量在线值时，记录最近一次使用的PM的编号 */
static uint8_t s_ucPassword[(MAX_CONN_PWD_LEN)+1U];     /* 连接密码 */
static VarErrInfo_t s_stVarErrInfo;                     /* 变量错误统计信息 */
static PMTaskInfo_t s_stPMTaskInfo;                     /* PM任务信息 */

/* Configuration */
static AccCtrlInfo_t s_stAccCtrlInfo;                   /* 访问控制信息 */

/* Source File Update */
static WriteSrcFileInfo_t s_stWSrcInfo;                 /* 写源文件信息 */
static FUPOprInfo_t s_stFUPOprInfo;

/* Source File */
static int32_t s_iUpSrcFD;  /* File descriptor of updating source File */

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
static void InitSrcUPCtrlInfo( void );
static void InitLocalConfigInfo( void );
static void ClearCS1131LocalInfo( uint8_t ucSWIndex );
static void ClearCS1131SysInfo( uint8_t ucSWIndex );
static void ClearWriteSrcFileInfo( WriteSrcFileInfo_t *pstWSrcInfo );
static void HandleClrWriteSrcFileInfo( uint8_t ucSWIndex );

/* Configuration */
static void ClearLocalConfig( void );
static void UpdateAccessAuth( CS1131Info_t *pstSWInfo );
static bool_t CheckMonitorVar( VarInfo_t const *pstMonVar );

/* Handle Command */
static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, CS1131Info_t *pstSWInfo );
static bool_t CheckPMHandle( uint8_t const pucReq[]);
static void PMHandle( CS1131Info_t *pstSWInfo, uint8_t ucSWIndex, NetMsg_t *pstReqMsg, RespMsg_t *pstResp );
static SelPMID_t SelectPM( CS1131Info_t *pstSWInfo, uint8_t const pucReq[]);

/* Decode Command */
static void DecodeCmd( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeLogin( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeLogout( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeWriteFileStart( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeWriteFileCont( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeWriteFileEnd( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadSrcFileStart( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadSrcFileCont( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeClearSrcFile( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadStatus( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp );
static void DecodeDefMonVarTable( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadVarOnLine( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadStationID( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );
static void DecodeReadIOStatus( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg );

/* Read source file */
static int32_t ReadSrcFile( int32_t iFD, uint8_t pucBuff[], uint32_t uiCount, off_t iReadOffSet, bool_t *pbContinue );

/* Update source file */
static uint8_t SrcUpdateStart( void );
static uint8_t SrcUpdateCont( uint8_t const pucData[], uint16_t usDataLen );
static void SrcUpdateEnd( void );

/* PM Task Info. */
static bool_t ComparePMProjVer( ProjVerInfo_t const *pstPM1ProjVer, ProjVerInfo_t const *pstPM2ProjVer );
static bool_t IsPMProjValid( ProjVerInfo_t const *pstPMProjVer );
static void HandleTotalProjVer( TotalProjVerInfo_t *pstTotalProjVer );
static void HandlePMTaskInfo( PMTaskInfo_t *pstPMTaskInfo );

/* Response */
static bool_t MakeRespMsg( CS1131Info_t *pstSWInfo, uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp );
static uint16_t AddPrjVersion( uint8_t pucRespData[]);
static uint16_t AddFileData( CS1131Info_t *pstSWInfo, uint8_t pucRespData[]);
static uint16_t AddStatusData( StsReqType_t emType, uint8_t pucRespData[]);
static uint16_t AddCMStatusData( uint8_t pucRespData[]);
static uint16_t AddPMStatusData( uint8_t pucRespData[]);
static uint16_t AddVarOnlineData( MonVarTableInfo_t const *pstCurMonTblInfo, uint8_t pucRespData[], bool_t *pbCMBus );
static uint16_t AddIOStatusData( uint8_t pucRespData[]);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CS1131Init
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

void CS1131Init( void )
{
    DEBUG_CS1131( "CS1131Init\n" );

    uint8_t ucIndex = 0U;
    s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucCurSWIndex = 0U;
    s_emState = IDLE_STATE;
    s_uiWaitPMCycleCnt = 0U;
    s_emResp = NONE_RESP;
    s_emLastPMID = PMA;

    /* Local CS1131 Software Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_CS1131_NUM); ucIndex++ )
    {
        ClearCS1131LocalInfo( ucIndex );
    }

    /* CS1131 Software Command */
    memset( &s_stReqMsg, 0, sizeof(NetMsg_t));
    s_stReqMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Response Message Buffer */
    memset( &s_stRespMsg, 0, sizeof(RespMsg_t));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* Connect Password */
    memset( s_ucPassword, 0, sizeof(s_ucPassword));

    /* Variable Error Info. */
    memset( &s_stVarErrInfo, 0, sizeof(VarErrInfo_t));

    /* Task Info. */
    memset( &s_stPMTaskInfo, 0, sizeof(PMTaskInfo_t));
    s_stPMTaskInfo.bValid = false;
    s_stPMTaskInfo.bWait = false;

    /* Write Source File Info */
    ClearWriteSrcFileInfo( &s_stWSrcInfo );

    /* Write Source File Control Info. */
    InitSrcUPCtrlInfo();

    /* "SrcPrj" file: R/W, Create if not exist, flush after write */
    s_iUpSrcFD = open( "/home/root/SrcPrj", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iUpSrcFD )
    {
        DEBUG_CS1131( "Open source file failed, errno: %d!\n", errno );
        LogWrite( (uint32_t)(LOGID_CS1131_E_OPEN_FILE));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CS1131Cycle
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

void CS1131Cycle( void )
{
    static bool_t s_bClrDone = false;
    uint8_t ucSWIndex = 0U;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Update Local Station ID */
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();

    /* Handle Local Configuration */
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
    for( ucSWIndex = 0U; ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM); ucSWIndex++ )
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

    /* Send operation message if exists */
    SendOprMsg( &s_stFUPOprInfo.stOprMsgList, s_stFUPOprInfo.iMsgQID, &s_stFUPOprInfo.stSendEInfo );

    /* Receive operation result message if exists */
    if( RecvOprRetMsg( s_stFUPOprInfo.iMsgQID, MSGT_SRCUP_OPR_RET ))
    {
        LogWrite( (uint32_t)(LOGID_CS1131_E_UP_SRCFILE));

        /* Clear Write Source File Info */
        ClearWriteSrcFileInfo( &s_stWSrcInfo );
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
* 函数名称: ClearCS1131LocalInfo
*
* 功能描述: 清除指定CS1131软件的本地信息
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

static void ClearCS1131LocalInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))
    {
        memset( &s_stCS1131Info[ucSWIndex], 0, sizeof(CS1131Info_t));

        s_stCS1131Info[ucSWIndex].emNetwork = INVALID_NETWORK_PORT;
        s_stCS1131Info[ucSWIndex].ucAccAuth = (uint8_t)(AL_ALL);
        s_stCS1131Info[ucSWIndex].emLog = LOGOUT;
        s_stCS1131Info[ucSWIndex].emSelPMID = SELPM_NONE;
        s_stCS1131Info[ucSWIndex].stMonTblInfo.usMonVarCnt = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearCS1131SysInfo
*
* 功能描述: 清除指定CS1131软件的系统资源信息
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

static void ClearCS1131SysInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))
    {
        /* Reset Lock Flag */
        if( !SysResetAppSwLockedFlag( APP_CS1131, ucSWIndex ))
        {
            DEBUG_CS1131( "Reset CS1131-%d Locked Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_RST_LOCK));
        }

        /* Reset Connect Flag */
        if( !SysResetAppSwConnectFlag( APP_CS1131, ucSWIndex ))
        {
            DEBUG_CS1131( "Reset CS1131-%d Connect Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_RST_CONN));
        }

        /* Set Clear Flag */
        if( !SysSetAppSwClearFlag( APP_CS1131, ucSWIndex ))
        {
            DEBUG_CS1131( "Set CS1131-%d Clear Flag Failed\n", ucSWIndex );
            LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_SET_CLR));
        }

        /* Clear Request Msg */
        SysClearAppMsg( APP_CS1131, ucSWIndex, APP_MSG_REQ );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearWriteSrcFileInfo
*
* 功能描述: 清除写源文件信息
*
* 输入参数: pstWSrcInfo：写源文件信息；
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearWriteSrcFileInfo( WriteSrcFileInfo_t *pstWSrcInfo )
{
    if( pstWSrcInfo != NULL )
    {
        memset( pstWSrcInfo, 0, sizeof(WriteSrcFileInfo_t));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleClrWriteSrcFileInfo
*
* 功能描述: 处理清除写源文件信息
*
* 输入参数: ucSWIndex：软件索引值；
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleClrWriteSrcFileInfo( uint8_t ucSWIndex )
{
    if( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))
    {
        if((s_stWSrcInfo.bWrite) &&
           (ucSWIndex == s_stWSrcInfo.ucSWIndex))
        {
            ClearWriteSrcFileInfo( &s_stWSrcInfo );
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: InitSrcUPCtrlInfo
*
* 功能描述: 初始化固件更新控制信息
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

static void InitSrcUPCtrlInfo( void )
{
    FileUPTaskInfo_t stInfo;

    /* FUP Operation Message List */
    InitOprMsgList( &s_stFUPOprInfo.stOprMsgList );

    /* Send Error Info. */
    memset( &s_stFUPOprInfo.stSendEInfo, 0, sizeof(OprRetInfo_t));

    GetFileUpTaskInfo( &stInfo );
    s_stFUPOprInfo.bAllowFUP = stInfo.bFileUpTask;
    s_stFUPOprInfo.iMsgQID = stInfo.iMsgQID;
    s_stFUPOprInfo.uiShmOffset = 0U;

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
    if( HandleAccessCtrlInfo( &s_stAccCtrlInfo ))
    {
        DEBUG_CS1131( "Access Control Cfg Switch\n" );
        LogWrite( (uint32_t)(LOGID_CS1131_CFG_SWITCH_ACCESS_CTRL));
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
    InitLocalConfigInfo();

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
    if( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))
    {
        if( SysGetAppSwConnectFlag( APP_CS1131, ucSWIndex))  /* Connect */
        {
            s_stCS1131Info[ucSWIndex].uiWaitReqCycleCnt++;

            /* Conncet & Timeout: Clear */
            if( s_stCS1131Info[ucSWIndex].uiWaitReqCycleCnt >= (uint32_t)(CS1131_SW_TIMEOUT_CYCLE_CNT))
            {
                DEBUG_CS1131( "CS1131-%d Timeout\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_CS1131_E_SWTO));

                /* Clear Local Info */
                ClearCS1131LocalInfo( ucSWIndex );

                /* Clear System Info */
                ClearCS1131SysInfo( ucSWIndex );

                /* Check if this software writing source file: if yes, clear its info. */
                HandleClrWriteSrcFileInfo( ucSWIndex );
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
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleDisconnect( uint8_t ucSWIndex )
{
    bool_t bClear = false;

    if( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))
    {
        if( !SysGetAppSwConnectFlag( APP_CS1131, ucSWIndex ))  /* Disconnect */
        {
            if( s_stCS1131Info[ucSWIndex].uiIPAddr != 0U )
            {
                DEBUG_CS1131( "CS1131-%d Disconnected\n", ucSWIndex );

                /* Connect -> Disconnect & Non-Lock: Clear */
                if( ucSWIndex == s_ucCurSWIndex )
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
                    ClearCS1131LocalInfo( ucSWIndex );  /* Clear Local Info */

                    /* Check if this software writing source file: if yes, clear its info. */
                    HandleClrWriteSrcFileInfo( ucSWIndex );
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
    uint16_t usAType = 0U;
    CS1131Info_t *pstSWInfo = NULL;  /* Current CS1131 software local info. */

    if( s_stPMTaskInfo.bWait )
    {
        pstSWInfo = &( s_stCS1131Info[s_ucCurSWIndex]);
        HandlePMTaskInfo( &s_stPMTaskInfo );
        s_stPMTaskInfo.bWait = false;

        /* Make Response Message */
        if( !MakeRespMsg( pstSWInfo, (uint8_t)(ERR_SUCCESS), s_stReqMsg.ucBuff, &s_stRespMsg ))
        {
            DEBUG_CS1131( "MakeRespMsg Err\n" );
            LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
        }

        s_emState = BUSY_STATE;
    }
    else
    {
        s_stRespMsg.usLen = 0U;
        s_emResp = NONE_RESP;

        while( ucCheckCnt < (uint8_t)(APP_SW_CS1131_NUM))
        {
            /* Read Request Message */
            s_stReqMsg.stNetHead.usLen = 0U;
            usLen = (uint16_t)sizeof( NetMsg_t );
            if( !SysGetAppMsg( APP_CS1131, s_ucCurSWIndex, APP_MSG_REQ, (puint8_t)&s_stReqMsg, &usLen ))
            {
                usLen = 0U;
            }

            if((usLen > 0U) && (s_stReqMsg.stNetHead.usLen > 0U))
            {
                /* Has Message */
                pstSWInfo = &( s_stCS1131Info[s_ucCurSWIndex]);
                pstSWInfo->uiWaitReqCycleCnt = 0U;  /* Clear wait request count */
                ucSubReason = CheckCmd( &s_stReqMsg, pstSWInfo );
                if((uint8_t)(ERR_SUCCESS) == ucSubReason )
                {
                    /* Valid */
                    DEBUG_CS1131( "HandleCmd: Msg Valid\n" );
                }
                else
                {
                    /* Invalid */
                    DEBUG_CS1131( "HandleCmd: Msg Invalid\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CMD));
                }

                if((uint8_t)(ERR_SUCCESS) == ucSubReason )
                {
                    if( CheckPMHandle( s_stReqMsg.ucBuff ))
                    {
                        /* PM Handle */
                        DEBUG_CS1131( "HandleCmd: PM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));
                        PMHandle( pstSWInfo, s_ucCurSWIndex, &s_stReqMsg, &s_stRespMsg );
                    }
                    else
                    {
                        /* CM Handle */
                        DEBUG_CS1131( "HandleCmd: CM Handle, Actual Type: %d\n", GetReqActualType( s_stReqMsg.ucBuff ));

                        /* Decode Cmd */
                        DecodeCmd( pstSWInfo, s_stReqMsg.ucBuff, &s_stRespMsg, s_bCMHasCfg );
                        s_emResp = CM_RESP;
                    }
                }
                else
                {
                    /* Make Response Message */
                    if( !MakeRespMsg( pstSWInfo, ucSubReason, s_stReqMsg.ucBuff, &s_stRespMsg ))
                    {
                        DEBUG_CS1131( "MakeRespMsg Err\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
                    }

                    s_emResp = CM_RESP;
                }

                /* Lock & Busy */
                if( !SysSetAppSwLockedFlag( APP_CS1131, s_ucCurSWIndex ))
                {
                    DEBUG_CS1131( "HandleCmd: Set Locked Flag Failed\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_SET_LOCK));
                }

                if( !s_stPMTaskInfo.bWait )
                {
                    s_emState = BUSY_STATE;
                }

                break;
            }
            else
            {
                /* No Message */
                s_ucCurSWIndex++;
                s_ucCurSWIndex = s_ucCurSWIndex % (uint8_t)(APP_SW_CS1131_NUM);
                ucCheckCnt++;
            }
        }/*while*/
    }

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

static uint8_t CheckCmd( NetMsg_t const *pstReqMsg, CS1131Info_t *pstSWInfo )
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
                DEBUG_CS1131( "CheckCmd: SrcAddr Invalid\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_SW_SRCADDR));
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
                DEBUG_CS1131( "CheckCmd: Type-0x%X != 0x%X\n", pucReq[APP_REQ_TYPE_INDEX], (uint8_t)(APP_REQ_TYPE));
                ucSubReason = (uint8_t)(ERR_TYPE);  /* Type */
            }
            else if( pucReq[APP_REQ_PROTECT_INDEX] != (uint8_t)(APP_REQ_PROTECT))
            {
                DEBUG_CS1131( "CheckCmd: Protect-%d Invalid\n", pucReq[APP_REQ_PROTECT_INDEX]);
                ucSubReason = (uint8_t)(ERR_PROTECT);  /* Protect */
            }
            else
            {
                /* Actual Type */
                usAType = GetReqActualType( pucReq );
                if((usAType < (uint16_t)(CS1131_ATYPE_MIN)) ||
                   (usAType > (uint16_t)(CS1131_ATYPE_MAX)))
                {
                    DEBUG_CS1131( "CheckCmd: Actual Type-%d, HI-%d, LOW-%d Invalid\n", \
                                  usAType, pucReq[APP_REQ_ATYPE_HIINDEX], pucReq[APP_REQ_ATYPE_LOWINDEX]);
                    ucSubReason = (uint8_t)(ERR_ACTUAL_TYPE);
                }
                /*else if( pucReq[APP_RESP_STAID_INDEX] != s_ucLocalStaID )
                {
                    DEBUG_CS1131( "Check CS1131 Cmd: Station ID-%d Invalid.\n", pucReq[APP_RESP_STAID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_STAID);
                }*/
                else if( pucReq[APP_REQ_PMID_INDEX] > (uint8_t)(APP_MAX_REQ_PMID))
                {
                    DEBUG_CS1131( "CheckCmd: PM ID-%d Invalid\n", pucReq[APP_REQ_PMID_INDEX]);
                    ucSubReason = (uint8_t)(ERR_PMID);
                }
                else if( pucReq[APP_REQ_LAST_INDEX] != (uint8_t)(APP_REQ_LAST_FLAG))
                {
                    DEBUG_CS1131( "CheckCmd: Last Flag-%d Invalid\n", pucReq[APP_REQ_LAST_INDEX]);
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
        if(((uint16_t)(CS1131_LOGIN) == usAType) ||
           ((usAType >= (uint16_t)(CS1131_CLR_SRCFILE)) && (usAType <= (uint16_t)(CS1131_READ_VARONLINE))) ||
           ((uint16_t)(CS1131_READ_STAID) == usAType) ||
           ((uint16_t)(CS1131_READ_IO_STATUS) == usAType))
        {
            bPMHandle = false;
        }
        else if(((usAType >= (uint16_t)(CS1131_WRITEFILE_START)) && (usAType <= (uint16_t)(CS1131_WRITEFILE_END))) ||
                ((usAType >= (uint16_t)(CS1131_READFILE_START)) && (usAType <= (uint16_t)(CS1131_READFILE_CONT))))
        {
            /* Read/Write File: Check File Type */
            if((uint8_t)(FT_SRC) == pucReq[FILE_TYPE_INDEX])
            {
                bPMHandle = false;  /* Source File */
            }
            else
            {
                bPMHandle = true;   /* Binary or Symbol Table */
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

static bool_t MakeRespMsg( CS1131Info_t *pstSWInfo, uint8_t ucSubReason, uint8_t const pucReq[], RespMsg_t *pstResp )
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
        pstResp->usLen = 0U;            /* Reset Length */
        pucResp = &pstResp->ucBuff[0];  /* Buffer */

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

            /* Define Monitor Variable Table */
            if((uint16_t)(CS1131_DEF_VARTABLE) == usAType )
            {
                if( s_stVarErrInfo.usErrCnts > 0U )
                {
                    /* Has Data Area: Add Error Info. */
                    pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_EXIST);
                    usIndex = (uint16_t)(APP_MSG_HEADER_LEN);  /* Data Area Index */
                    usDataLen = sizeof(VarErrInfo_t);
                    memcpy( &pucResp[usIndex], &s_stVarErrInfo, (uint32_t)usDataLen );
                    usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                }
                else
                {
                    /* No Data Area */
                    pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
                    usLength = (uint16_t)(APP_MSG_HEADER_LEN);
                }
            }
            else
            {
                /* No Data Area */
                pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
                usLength = (uint16_t)(APP_MSG_HEADER_LEN);
            }
        }
        else
        {
            /* Valid Request Message */
            pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_SUCCESS);

            if(((uint16_t)(CS1131_LOGOUT) == usAType) ||
               ((uint16_t)(CS1131_CLR_SRCFILE) == usAType) ||
               ((uint16_t)(CS1131_WRITEFILE_START) == usAType) ||
               ((uint16_t)(CS1131_WRITEFILE_CONT) == usAType) ||
               ((uint16_t)(CS1131_WRITEFILE_END) == usAType) ||
               ((uint16_t)(CS1131_DEF_VARTABLE) == usAType))
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

                if((uint16_t)(CS1131_LOGIN) == usAType )
                {
                    usDataLen = AddPrjVersion( &pucResp[usIndex]);
                    if((usDataLen > 0U) && (usDataLen <= (uint16_t)(MAX_APP_MSG_DATA_LEN)))
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ADD_PROJVER);
                        DEBUG_CS1131( "MakeRespMsg: AddPrjVersion Failed\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ADD_PRJ_VER));
                    }
                }
                else if(((uint16_t)(CS1131_READFILE_START) == usAType) ||
                        ((uint16_t)(CS1131_READFILE_CONT) == usAType))
                {
                    usDataLen = AddFileData( pstSWInfo, &pucResp[usIndex]);
                    if((usDataLen > 0U) && (usDataLen <= (uint16_t)(MAX_APP_MSG_DATA_LEN)))
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_READ_FILE);  /* Read File Failed */
                        DEBUG_CS1131( "MakeRespMsg: AddFileData Failed\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ADD_FILE_DATA));
                    }
                }
                else if((uint16_t)(CS1131_READ_STATUS) == usAType )
                {
                    usDataLen = AddStatusData( pucReq[usIndex], &pucResp[usIndex]);
                    if((usDataLen > 0U) && (usDataLen <= (uint16_t)(MAX_APP_MSG_DATA_LEN)))
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ADD_STATUS);
                        DEBUG_CS1131( "MakeRespMsg: AddStatusData Failed\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ADD_STATUS_DATA));
                    }
                }
                else if((uint16_t)(CS1131_READ_VARONLINE) == usAType )
                {
                    usDataLen = AddVarOnlineData( &pstSWInfo->stMonTblInfo, &pucResp[usIndex], &bCMBus );
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
                        ucSubReason = (uint8_t)(ERR_ADD_ONLINE_DATA);
                        DEBUG_CS1131( "MakeRespMsg: AddVarOnlineData Failed\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ADD_ONLINE_DATA));
                    }
                }
                else if((uint16_t)(CS1131_READ_IO_STATUS) == usAType )
                {
                    usDataLen = AddIOStatusData( &pucResp[usIndex]);
                    if((usDataLen > 0U) && (usDataLen <= (uint16_t)(MAX_APP_MSG_DATA_LEN)))
                    {
                        usLength = (uint16_t)(APP_MSG_HEADER_LEN) + usDataLen;
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ADD_IO_STATUS);
                        DEBUG_CS1131( "MakeRespMsg: AddIOStatusData Failed\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ADD_IO_STATUS_DATA));
                    }
                }
                else
                {
                    /* Data Area: Local Station ID, 2 Bytes */
                    pucResp[usIndex] = (uint8_t)(pucResp[APP_RESP_STAID_INDEX]);
                    usIndex++;
                    pucResp[usIndex] = 0U;
                    usIndex++;
                    usLength = usIndex;
                }

                if( ucSubReason != (uint8_t)(ERR_SUCCESS))
                {
                    pucResp[APP_RESP_RESPCODE_INDEX] = (uint8_t)(RESPCODE_FAILED);
                    pucResp[APP_RESP_SUBREA_INDEX] = ucSubReason;
                    pucResp[APP_RESP_DATAFLAG_INDEX] = (uint8_t)(DATA_NOT_EXIST);
                    usLength = (uint16_t)(APP_MSG_HEADER_LEN);
                }
            }/* end of else - Has data area */
        }/*end of else - valid msg */

        /* Length: Header + Data Area */
        pucResp[APP_RESP_LEN_HIINDEX] = (uint8_t)((usLength >> 8) & 0xFF);
        pucResp[APP_RESP_LEN_LOWINDEX] = (uint8_t)(usLength & 0xFF);

        /* CRC */
        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucResp, (uint32_t)usLength );

        /* Write CRC Area: High First */
        usIndex = usLength;  /* CRC Index */
        memcpy( &pucResp[usIndex], &uiCalcCRC, sizeof(uint32_t));
        usIndex += sizeof(uint32_t);

        /* Update Response Message Length */
        pstResp->usLen = usIndex;
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: AddPrjVersion
*
* 功能描述: 添加工程版本信息
*
* 输入参数: pucRespData：应答帧数据域首字节地址
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddPrjVersion( uint8_t pucRespData[])
{
    uint16_t usDataLen = 0U;
    ProjVerInfo_t stProjVerInfo;

    if( pucRespData != NULL )
    {
        if( SysGetLocalProjVersion( &stProjVerInfo ))
        {
            memcpy( pucRespData, &stProjVerInfo, sizeof(ProjVerInfo_t));
            usDataLen = (uint16_t)sizeof(ProjVerInfo_t);
        }
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: AddFileData
*
* 功能描述: 添加文件数据
*
* 输入参数: pstSWInfo：当前软件信息；
*          pucRespData：应答帧数据域首字节地址: ContFlag(1B)+DataLen(2B)+Data(n)。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddFileData( CS1131Info_t *pstSWInfo, uint8_t pucRespData[])
{
    uint16_t usDataIndex = 0U;
    int32_t iDataLen = 0U;
    bool_t bContinue = 0U;
    uint16_t usDataLen = 0U;

    if((pstSWInfo != NULL) &&
       (pucRespData != NULL))
    {
        usDataIndex = 3U;  /* File Data Index: ContFlag(1B) + DataLen(2B) */
        iDataLen = ReadSrcFile( s_iUpSrcFD, &pucRespData[usDataIndex], (uint32_t)(MAX_MSG_FILEDATA_LEN), \
                                (off_t)pstSWInfo->uiSrcFileROffset, &bContinue );
        if( iDataLen != -1 )
        {
            usDataIndex = 0U;

            /* Continue Flag */
            if( bContinue )
            {
                pucRespData[usDataIndex] = 1U;  /* Continue */
                pstSWInfo->uiSrcFileROffset += (uint32_t)iDataLen;
            }
            else
            {
                pucRespData[usDataIndex] = 0U;  /* Done */
                pstSWInfo->uiSrcFileROffset = 0U;
            }

            usDataIndex++;

            /* File Data Length: High byte first */
            usDataLen = (uint16_t)iDataLen;
            memcpy( &pucRespData[usDataIndex], &usDataLen, sizeof(uint16_t));
            usDataIndex += sizeof(uint16_t);

            /* Update Length */
            usDataIndex += (uint16_t)iDataLen;
        }
        else
        {
            usDataIndex = 0U;
        }
    }

    return usDataIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddStatusData
*
* 功能描述: 根据请求类型，添加CM及PM状态信息
*
* 输入参数: emType：请求类型；
*          pucRespData：应答帧数据域首字节地址。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddStatusData( StsReqType_t emType, uint8_t pucRespData[])
{
    uint16_t usIndex = 0U;
    uint16_t usStatusLen = 0U;

    if( pucRespData != NULL )
    {
        if( SRT_ALL == emType )
        {
            /* CM Status */
            usStatusLen = AddCMStatusData( &pucRespData[usIndex]);
            if( usStatusLen > 0U )
            {
                usIndex += usStatusLen;

                /* PM Status */
                usStatusLen = AddPMStatusData( &pucRespData[usIndex]);
                if( usStatusLen > 0U )
                {
                    usIndex += usStatusLen;
                }
                else
                {
                    usIndex = 0U;
                }
            }
            else
            {
                usIndex = 0U;
            }
        }/* end of SRT_ALL */
        else if( SRT_CM == emType )
        {
            /* CM Status */
            usStatusLen = AddCMStatusData( &pucRespData[usIndex]);
            if( usStatusLen > 0U )
            {
                usIndex += usStatusLen;
            }
            else
            {
                usIndex = 0U;
            }
        }/* end of SRT_CM */
        else
        {
            /* PM Status */
            usStatusLen = AddPMStatusData( &pucRespData[usIndex]);
            if( usStatusLen > 0U )
            {
                usIndex += usStatusLen;
            }
            else
            {
                usIndex = 0U;
            }
        }
    }

    return usIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddCMStatusData
*
* 功能描述: 添加CM状态信息
*
* 输入参数: pucRespData：应答帧数据域地址
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddCMStatusData( uint8_t pucRespData[])
{
    CMController_t emCMID = CM1;
    PMController_t emPMID = PMA;
    uint16_t usIndex = 0U;
    uint16_t usStatus = 0U;

    /* Select PM */
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
        emPMID = PMA;
    }

    for( emCMID = CM1; emCMID < NUM_OF_CM; emCMID++ )
    {
        if( SysGetCMState(emCMID, &usStatus ))
        {
            /* Add Status */
            memcpy( &pucRespData[usIndex], &usStatus, sizeof(uint16_t));
            usIndex += sizeof(uint16_t);
        }
        else
        {
            usIndex = 0U;
            break;
        }
    }

    return usIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddPMStatusData
*
* 功能描述: 添加PM状态信息
*
* 输入参数: pucRespData：应答帧数据域地址
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddPMStatusData( uint8_t pucRespData[])
{
    uint16_t usIndex = 0U;
    uint16_t usStatus = 0U;
    uint8_t ucTaskIndex = 0U;
    PMController_t emPMID = PMA;

    if( pucRespData != NULL )
    {
        /* PM Status */
        for( emPMID = PMA; emPMID < NUM_OF_PM; emPMID++ )
        {
            if( SysGetPMState( emPMID, &usStatus ))
            {
                /* Add Status */
                memcpy( &pucRespData[usIndex], &usStatus, sizeof(uint16_t));
                usIndex += sizeof(uint16_t);
            }
            else
            {
                usIndex = 0U;
                break;
            }
        }

        /* Task Info. */
        if( usIndex > 0U )
        {
            pucRespData[usIndex] = (uint8_t)s_stPMTaskInfo.icPMTaskNum;
            usIndex++;
            if( s_stPMTaskInfo.icPMTaskNum > 0 )
            {
                /* Status */
                for( ucTaskIndex = 0U; ucTaskIndex < (uint8_t)s_stPMTaskInfo.icPMTaskNum; ucTaskIndex++ )
                {
                    pucRespData[usIndex] = s_stPMTaskInfo.stPMTaskInfo[ucTaskIndex].stVTaskInfo.ucTaskID;
                    usIndex++;
                    pucRespData[usIndex] = (uint8_t)(s_stPMTaskInfo.stPMTaskInfo[ucTaskIndex].stPMATaskInfo.icTaskStatus);
                    usIndex++;
                    pucRespData[usIndex] = (uint8_t)(s_stPMTaskInfo.stPMTaskInfo[ucTaskIndex].stPMBTaskInfo.icTaskStatus);
                    usIndex++;
                    pucRespData[usIndex] = (uint8_t)(s_stPMTaskInfo.stPMTaskInfo[ucTaskIndex].stPMCTaskInfo.icTaskStatus);
                    usIndex++;
                    pucRespData[usIndex] = (uint8_t)(s_stPMTaskInfo.stPMTaskInfo[ucTaskIndex].stVTaskInfo.icTaskStatus);
                    usIndex++;
                }
            }
        }

        /* Download Status */
        if( usIndex > 0U )
        {
            pucRespData[usIndex] = (uint8_t)SysGetPMDownloadStatus();
            usIndex++;
        }
    }

    return usIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddVarOnlineData
*
* 功能描述: 添加变量在线值
*
* 输入参数: pstCurMonTblInfo：当前软件的监视变量列表；
*          pucRespData：应答帧数据域首字节地址；
*
* 输出参数: pbCMBus：用于返回CM Bus是否故障
*
* 返 回 值: 添加的数据长度，即Data Area的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddVarOnlineData( MonVarTableInfo_t const *pstCurMonTblInfo, uint8_t pucRespData[], bool_t *pbCMBus )
{
    uint8_t ucValue = 0U;
    uint16_t usDataIndex = 0U;
    uint16_t usVarIndex = 0U;
    uint32_t uiOffset = 0U;
    SysSystemTime_t stTime;
    ReadOnlineDataInfo_t stRODInfo;
    VarInfo_t const *pstVar = NULL;
    uint8_t const *pucGAddr = NULL;
    uint8_t const *pucIAddr = NULL;
    uint8_t const *pucQAddr = NULL;
    uint8_t const *pucVarAddr = NULL;

    if((pstCurMonTblInfo != NULL) &&
       (pucRespData != NULL) &&
       (pbCMBus != NULL))
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
                /* Info.: Time & Count */
                stTime = SysGetSystemTime();
                stRODInfo.uiSecond = stTime.uiSecond;
                stRODInfo.usMiliSecond = stTime.usMiliSecond;
                stRODInfo.usVarCnts = pstCurMonTblInfo->usMonVarCnt;
                memcpy( &pucRespData[usDataIndex], &stRODInfo, sizeof(ReadOnlineDataInfo_t));
                usDataIndex += sizeof(ReadOnlineDataInfo_t);

                /* Loop Variable: Add its value */
                for( usVarIndex = 0U; usVarIndex < pstCurMonTblInfo->usMonVarCnt; usVarIndex++ )
                {
                    pstVar = &pstCurMonTblInfo->stMonVarTable[usVarIndex];
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

                    /* Set Value */
                    if( 0U == pstVar->usVarSize )
                    {
                        /* Offset: Bit */
                        ucValue = *pucVarAddr;
                        if((ucValue & (0x01U << (pstVar->uiOffset % 8U))) != 0U )
                        {
                            pucRespData[usDataIndex] = 1U;
                            usDataIndex++;
                        }
                        else
                        {
                            pucRespData[usDataIndex] = 0U;
                            usDataIndex++;
                        }
                    }
                    else if( pstVar->usVarSize <= (uint16_t)(MAX_VAR_SIZE))
                    {
                        memcpy( &pucRespData[usDataIndex], pucVarAddr, (uint32_t)pstVar->usVarSize );
                        usDataIndex += pstVar->usVarSize;
                    }
                    else
                    {
                        usDataIndex = 0U;
                        break;
                    }
                }/* end of for */
            }
            else
            {
                DEBUG_CS1131( "SysGetRTDataAddr Err: PM: %d\n", s_emLastPMID );
                LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_GET_RTDATA_ADDR));
            }
        }
    }

    return usDataIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddIOStatusData
*
* 功能描述: 添加IO状态信息: IO Count(2B) + PMA IO Status(240B)  + PMB IO Status(240B) + PMC IO Status(240B)
*
* 输入参数: pucRespData：应答帧数据域首字节地址
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，即Data Area的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddIOStatusData( uint8_t pucRespData[])
{
    uint16_t usDataIndex = 0U;
    uint16_t usIOIndex = 0U;
    uint16_t usIOCnt = 0U;
    SharedAppend_t const *pstAppend = NULL;
    PMController_t emPMID = PMA;

    if( pucRespData != NULL )
    {
        usDataIndex = sizeof(uint16_t);  /* IO Count(2B) */

        for( emPMID = PMA; emPMID < NUM_OF_PM; emPMID++ )
        {
            usIOCnt += (uint16_t)(MAX_SLAVE_NUM);

            pstAppend = (SharedAppend_t const *)SysGetAppendInfoAddr( emPMID );
            if((pstAppend != NULL) &&
               ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( emPMID )))
            {
                for( usIOIndex = 0U; usIOIndex < (uint16_t)(MAX_SLAVE_NUM); usIOIndex++ )
                {
                    pucRespData[usDataIndex] = pstAppend->stSlaveState[usIOIndex].stSlaveDiag.ucOnlineDiag;
                    usDataIndex++;
                }
            }
            else
            {
                /* all offline: 0 */
                memset( &pucRespData[usDataIndex], 0, sizeof(uint8_t)*(uint32_t)(MAX_SLAVE_NUM));
                usDataIndex += (uint16_t)(MAX_SLAVE_NUM);
            }
        }

        /* IO Count */
        memcpy( &pucRespData[0], &usIOCnt, sizeof(uint16_t));
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
*          ucSWIndex：1131软件索引值；
*          pstReqMsg：指向待检查的网络消息；
*          pstResp: 用于存放构造的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void PMHandle( CS1131Info_t *pstSWInfo, uint8_t ucSWIndex, NetMsg_t *pstReqMsg, RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    SelPMID_t emSelPMID = SELPM_NONE;
    uint16_t usAType = 0U;  /* Actual Request Message Type */
    DWStatus_t emDWStatus = DWS_NON;

    if(( pstSWInfo != NULL ) &&
       ( ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM)) &&
       ( pstReqMsg != NULL ) &&
       ( pstResp != NULL ))
    {
        if( pstSWInfo->emLog != LOGIN )
        {
            DEBUG_CS1131( "PMHandle: CS1131 should login first\n" );
            LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            ucSubReason = (uint8_t)(ERR_LOG);  /* Need To Login First */
        }
        else
        {
            if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
            {
                DEBUG_CS1131( "PMHandle: Access Authority - Deny\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS_DENY));
                ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
            }
            else if((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth )
            {
                usAType = GetReqActualType( pstReqMsg->ucBuff );
                if(((usAType >= (uint16_t)(CS1131_START_RUN)) && (usAType <= (uint16_t)(CS1131_SINGLE_STEP))) ||
                   ((uint16_t)(CS1131_DISABLE_VAR) == usAType) ||
                   ((usAType >= (uint16_t)(CS1131_WRITEFILE_START)) && (usAType <= (uint16_t)(CS1131_WRITEFILE_END))) ||
                   ((uint16_t)(CS1131_DOWNLOAD_END) == usAType ))
                {
                    DEBUG_CS1131( "PMHandle: Access Authority - Readonly\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS_RO));
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No Access Authority */
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_SUCCESS);
            }
        }

        if((uint8_t)(ERR_SUCCESS) == ucSubReason )
        {
            usAType = GetReqActualType( pstReqMsg->ucBuff );
            emDWStatus = (DWStatus_t)SysGetPMDownloadStatus();

            if( DWS_DWING == emDWStatus )
            {
                /* Downloading */
                if((uint16_t)(CS1131_DOWNLOAD_START) == usAType )
                {
                    DEBUG_CS1131( "Downloading, Response error code\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_DOWNING));
                    ucSubReason = (uint8_t)(ERR_DOWNLOADING);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
            }

            if((uint8_t)(ERR_SUCCESS) == ucSubReason )
            {
                /* Login & Has Access Authority: Select A PM Module */
                emSelPMID = SelectPM( pstSWInfo, &pstReqMsg->ucBuff[0]);
                if( SELPM_NONE == emSelPMID )
                {
                    DEBUG_CS1131( "PMHandle: CM BUS Invalid\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CMBUS));
                    ucSubReason = (uint8_t)(ERR_CMBUS);   /* CM BUS Error */
                }
                else
                {
                    /* Write Req To The Selected PM */
                    if( SysSetMsg( SwitchPMID( emSelPMID ), CS1131_REQ, pstReqMsg->ucBuff, pstReqMsg->stNetHead.usLen ))
                    {
                        /* PM Response */
#if 0
                        /* Print Request Message */
                        {
                            printf( "CS1131 Type %d, Len %d, Content(Top 50B): \n", usAType, pstReqMsg->stNetHead.usLen );
                            usPrintLen = pstReqMsg->stNetHead.usLen;
                            if( usPrintLen > 50U )
                            {
                                usPrintLen = 50U;
                            }

                            for( usIndex = 0U; usIndex < usPrintLen; usIndex++ )
                            {
                                printf( "%02X ", pstReqMsg->ucBuff[usIndex]);
                            }

                            printf( "\n" );
                        }
#endif
                        DEBUG_CS1131( "PMHandle: PM Resp\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        s_emResp = PM_RESP;
                        s_uiWaitPMCycleCnt = 0U;

                        /* Clear response message from the selected PM module */
                        /* Avoid old response messge make communication error */
                        if( !SysClearMsg( SwitchPMID(pstSWInfo->emSelPMID), CS1131_RESP ))
                        {
                            //DEBUG_CS1131( "PMHandle: SysClearMsg Failed, SelPMID-%d\n", emSelPMID );
                        }
                    }
                    else
                    {
                        DEBUG_CS1131( "PMHandle: SysSetMsg Failed, SelPMID-%d\n", emSelPMID );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_SET_MSG));
                        ucSubReason = (uint8_t)(ERR_SYSWREQTOPM);  /* 将请求写入系统资源失败 - 用于发往PM模块 */
                    }
                }
            }
        }

        if( ucSubReason != (uint8_t)(ERR_SUCCESS))
        {
            /* CM Response */
            DEBUG_CS1131( "PMHandle: CM Resp\n" );

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pstReqMsg->ucBuff, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
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
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          usAType：请求消息的实际类型。
*
* 输出参数: 无
*
* 返 回 值: 选取的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static SelPMID_t SelectPM( CS1131Info_t *pstSWInfo, uint8_t const pucReq[])
{
    SelPMID_t emSelPMID = SELPM_NONE;
    uint16_t usAType = 0U;
    uint8_t ucPMID = 0U;

    if((pstSWInfo != NULL) && (pucReq != NULL))
    {
        usAType = GetReqActualType( &pucReq[0]);
        ucPMID = pucReq[APP_REQ_PMID_INDEX];

        if(((usAType >= (uint16_t)(CS1131_WRITEFILE_START)) && (usAType <= (uint16_t)(CS1131_WRITEFILE_END))) ||
           ((usAType >= (uint16_t)(CS1131_READFILE_START))  && (usAType <= (uint16_t)(CS1131_READFILE_CONT))) ||
           ((uint16_t)(CS1131_DOWNLOAD_END) == usAType ))
        {
            /* Service included in download operation: use pre-selected pm */
            /* Do not update the pre-selected result */
            emSelPMID = pstSWInfo->emSelPMID;  /* Previous Selected Result */
            if( SysGetLocalInterCommState( SwitchPMID( emSelPMID )) != (uint16_t)(INTER_COMM_NORMAL))
            {
                emSelPMID = SELPM_NONE;
            }
        }
        else if((uint16_t)(CS1131_IO_RECONN) == usAType )
        {
            /* Specify PMID in request header */
            emSelPMID = ucPMID - 1;  /* 1-PMA; 2-PMB; 3-PMC */
            if( SysGetLocalInterCommState( emSelPMID ) != (uint16_t)(INTER_COMM_NORMAL))
            {
                emSelPMID = SELPM_NONE;
            }

            /* Record Select Result */
            pstSWInfo->emSelPMID = emSelPMID;
        }
        else
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
            
            DEBUG_CS1131( "SelPM: %d, usAType: %d\n", emSelPMID, usAType );
        }
    }

    return emSelPMID;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateAccessAuth
*
* 功能描述: 更新相应CS1131软件的访问权限
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

static void UpdateAccessAuth( CS1131Info_t *pstSWInfo )
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
                    DEBUG_CS1131( "AccCtrlNum: %d\n", ucItemNum );

                    /* Enable: Loop Access Control Table */
                    pstSWInfo->ucAccAuth = (uint8_t)(AL_DENY);
                    for( ucIndex = 0U; ucIndex < ucItemNum; ucIndex++ )
                    {
                        pstAccItem = &( pstCurAcc->stAccessCtrlConfig[ucIndex]);
                        DEBUG_CS1131( "AccCtrlItem Index %d: Net:%d, CS1131:%d, Client IP:0x%08X, Netmask:0x%08X\n", \
                                      ucIndex, pstAccItem->ucNetwork, pstAccItem->ucCS1131, pstAccItem->uiClientIPAddr, \
                                      pstAccItem->uiSubnetMask );

                        DEBUG_CS1131( "Mask IP:0x%08X, Access Mask IP:0x%08X\n", \
                                      pstSWInfo->uiIPAddr & pstAccItem->uiSubnetMask, \
                                      pstAccItem->uiClientIPAddr & pstAccItem->uiSubnetMask );

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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucCS1131;  /* CS1131 */
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
                                    pstSWInfo->ucAccAuth = pstAccItem->ucCS1131;  /* CS1131 */
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
* 函数名称: CheckMonVar
*
* 功能描述: 检查某一监视变量是否有效
*
* 输入参数: pstMonVar：指向待检查的监视变量
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckMonitorVar( VarInfo_t const *pstMonVar )
{
    bool_t bValid = false;
    uint32_t uiDataAreaLen = 0U;
    uint32_t uiOffset = 0U;

    if( pstMonVar != NULL )
    {
        if((pstMonVar->ucRefID < REF_NUM) &&
           (pstMonVar->usVarSize <= (uint16_t)(MAX_VAR_SIZE)))
        {
            if((0U == pstMonVar->usVarSize) ||
               (1U == pstMonVar->usVarSize) ||
               (2U == pstMonVar->usVarSize) ||
               (4U == pstMonVar->usVarSize) ||
               (8U == pstMonVar->usVarSize))
            {
                /* Section Size */
                uiDataAreaLen = GetDataAreaLen((SysRtDataAreaType_t)pstMonVar->ucRefID );
                if( uiDataAreaLen > 0U )
                {
                    /* Offset */
                    if( 0U == pstMonVar->usVarSize )
                    {
                        /* Offset: bit -> byte */
                        uiOffset = pstMonVar->uiOffset / 8;
                    }
                    else
                    {
                        /* Offset: byte */
                        uiOffset = pstMonVar->uiOffset;
                    }

                    if((uiOffset + (uint32_t)pstMonVar->usVarSize) <= uiDataAreaLen )
                    {
                        bValid = true;
                    }
                    else
                    {
                        DEBUG_CS1131( "BOff:%08X, DALen:%d, VOff:%08X, VSize:%d, RID:%d, TID:%d\n", \
                                      uiOffset, uiDataAreaLen, pstMonVar->uiOffset, pstMonVar->usVarSize, \
                                      pstMonVar->ucRefID, pstMonVar->ucTaskID );
                    }
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeCmd
*
* 功能描述: 解析CS1131软件命令
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

static void DecodeCmd( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );

        switch( usAType )
        {
            case (uint16_t)(CS1131_LOGIN):
            {
                DecodeLogin( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CS1131_LOGOUT):
            {
                DecodeLogout( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CS1131_WRITEFILE_START):
            {
                DecodeWriteFileStart( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_WRITEFILE_CONT):
            {
                DecodeWriteFileCont( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_WRITEFILE_END):
            {
                DecodeWriteFileEnd( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READFILE_START):
            {
                DecodeReadSrcFileStart( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READFILE_CONT):
            {
                DecodeReadSrcFileCont( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_CLR_SRCFILE):
            {
                DecodeClearSrcFile( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READ_STATUS):
            {
                DecodeReadStatus( pstSWInfo, pucReq, pstResp );
            }
            break;

            case (uint16_t)(CS1131_DEF_VARTABLE):
            {
                DecodeDefMonVarTable( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READ_VARONLINE):
            {
                DecodeReadVarOnLine( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READ_STAID):
            {
                DecodeReadStationID( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            case (uint16_t)(CS1131_READ_IO_STATUS):
            {
                DecodeReadIOStatus( pstSWInfo, pucReq, pstResp, bCMHasCfg );
            }
            break;

            default:
            {
                DEBUG_CS1131( "DecodeCmd: Actual Type-%d Invalid\n", usAType );
                LogWrite( (uint32_t)(LOGID_CS1131_E_CMD_ACTUAL_TYPE));
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
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeLogin( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
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
        if((uint16_t)(CS1131_LOGIN) == usAType )
        {
            UpdateAccessAuth( pstSWInfo );
            if((uint8_t)(AL_DENY) == pstSWInfo->ucAccAuth )
            {
                DEBUG_CS1131( "Login: Deny\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS_DENY));
                ucSubReason = (uint8_t)(ERR_ACCESS);
                pstSWInfo->emLog = LOGOUT;
            }
            else
            {
                /* Compare Connect Password */
                memset( &s_ucPassword[0], 0, sizeof(s_ucPassword));
                ucPSDLen = (uint8_t)sizeof( s_ucPassword );
                if( !SysGetPassword( &s_ucPassword[0], &ucPSDLen ))
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
                    /* Check Password Content */
                    bPSDValid = true;
                    for( ucIndex = 0U; ucIndex < ucPSDLen; ucIndex++ )
                    {
                        if( s_ucPassword[ucIndex] != pucReq[(uint8_t)(PSD_INDEX)+ucIndex])
                        {
                            bPSDValid = false;
                            DEBUG_CS1131( "PWD Err\n" );
                            LogWrite( (uint32_t)(LOGID_CS1131_E_CMD_PASSWORD_CONTENT));
                            break;
                        }
                    }
                }
                else
                {
                    bPSDValid = false;
                    DEBUG_CS1131( "PWDLen-%d != %d!\n", pucReq[PSD_LEN_INDEX], ucPSDLen );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CMD_PASSWORD_LEN));
                }

                if( bPSDValid )
                {
                    DEBUG_CS1131( "Login OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                    pstSWInfo->emLog = LOGIN;
                }
                else
                {
                    DEBUG_CS1131( "Login Failed: PWD Err\n" );
                    ucSubReason = (uint8_t)(ERR_CONN_PSD);
                    pstSWInfo->emLog = LOGOUT;
                }
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
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

static void DecodeLogout( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_LOGOUT) == usAType )
        {
            if( LOGIN == pstSWInfo->emLog )
            {
                DEBUG_CS1131( "Logout OK\n" );
                pstSWInfo->emLog = LOGOUT;
                ucSubReason = (uint8_t)(ERR_SUCCESS);
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeWriteFileStart
*
* 功能描述: 解析写文件开始命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址：Head(16) + Type(1) + ID(1) + DataLen(2) + Data(n)
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

static void DecodeWriteFileStart( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;        /* Actual Request Message Type */
    uint16_t usDataIndex = 0U;    /* Data Area Index */
    WriteFileInfo_t const *pstFileInfo = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usDataIndex = (uint16_t)(APP_MSG_HEADER_LEN);
        pstFileInfo = (WriteFileInfo_t const *)&pucReq[usDataIndex];
        usAType = GetReqActualType( &pucReq[0]);  /* Actual Type */

        /* Check Actual Type and File Type */
        if(((uint16_t)(CS1131_WRITEFILE_START) == usAType) &&
           ((uint8_t)(FT_SRC) == pstFileInfo->ucFileType))
        {
            /* LOGIN & AL_ALL */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth )
                    {
                        if( !s_stFUPOprInfo.bAllowFUP )
                        {
                            ucSubReason = (uint8_t)(ERR_SRCUP_INIT);
                            DEBUG_CS1131( "SrcUP resource initialized failed!\n" );
                        }
                        else
                        {
                            if( !IsOprMsgListEmpty( &s_stFUPOprInfo.stOprMsgList ))
                            {
                                ucSubReason = (uint8_t)(ERR_SENDOPRMSG_NOT_FIN);
                                DEBUG_CS1131( "SrcUP operation message not send finished!\n" );
                            }
                            else
                            {
                                /* Update to File Data Index */
                                usDataIndex += (uint16_t)sizeof(WriteFileInfo_t);

                                /* Clear Send error info. */
                                memset( &s_stFUPOprInfo.stSendEInfo, 0, sizeof(OprRetInfo_t));

                                /* Update Start */
                                ucSubReason = SrcUpdateStart();
                                if((uint8_t)(ERR_SUCCESS) == ucSubReason )
                                {
                                    ucSubReason = SrcUpdateCont( &pucReq[usDataIndex], pstFileInfo->usDataLen );
                                }
                            }
                        }
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeWriteFileCont
*
* 功能描述: 解析写文件继续命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址：Head(16) + Type(1) + ID(1) + DataLen(2) + Data(n)；
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

static void DecodeWriteFileCont( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;        /* Actual Request Message Type */
    uint16_t usDataIndex = 0U;    /* Data Area Index */
    WriteFileInfo_t const *pstFileInfo = NULL;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usDataIndex = (uint16_t)(APP_MSG_HEADER_LEN);
        pstFileInfo = (WriteFileInfo_t const *)&pucReq[usDataIndex];
        usAType = GetReqActualType( &pucReq[0]);  /* Actual Type */

        /* Check Actual Type and File Type */
        if(((uint16_t)(CS1131_WRITEFILE_CONT) == usAType) &&
           ((uint8_t)(FT_SRC) == pstFileInfo->ucFileType))
        {
            /* LOGIN & AL_ALL */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth )
                    {
                        /* Check Send error info. */
                        if( s_stFUPOprInfo.stSendEInfo.ucErrCode != (uint8_t)(ERR_SUCCESS))
                        {
                            ucSubReason = s_stFUPOprInfo.stSendEInfo.ucErrCode;

                            /* Clear Send Info. */
                            memset( &s_stFUPOprInfo.stSendEInfo, 0, sizeof(OprRetInfo_t));
                            InitOprMsgList( &s_stFUPOprInfo.stOprMsgList );
                        }
                        else
                        {
                            /* Update to File Data Index */
                            usDataIndex += (uint16_t)sizeof(WriteFileInfo_t);

                            /* Update Continue */
                            ucSubReason = SrcUpdateCont( &pucReq[usDataIndex], pstFileInfo->usDataLen );
                        }
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeWriteFileEnd
*
* 功能描述: 解析写文件完成命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址：Head(16) + Type(1) + ID(1)；
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

static void DecodeWriteFileEnd( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;        /* Actual Request Message Type */
    uint16_t usDataIndex = 0U;    /* Data Area Index */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usDataIndex = (uint16_t)(APP_MSG_HEADER_LEN);  /* File Type Index */
        usAType = GetReqActualType( &pucReq[0]);

        /* Check Actual Type and File Type */
        if(((uint16_t)(CS1131_WRITEFILE_END) == usAType) &&
           ((uint8_t)(FT_SRC) == pucReq[usDataIndex]))
        {
            /* LOGIN & AL_ALL */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth )
                    {
                        /* Check Send error info. */
                        if( s_stFUPOprInfo.stSendEInfo.ucErrCode != (uint8_t)(ERR_SUCCESS))
                        {
                            ucSubReason = s_stFUPOprInfo.stSendEInfo.ucErrCode;

                            /* Clear Send Info. */
                            memset( &s_stFUPOprInfo.stSendEInfo, 0, sizeof(OprRetInfo_t));
                            InitOprMsgList( &s_stFUPOprInfo.stOprMsgList );
                        }
                        else
                        {
                             SrcUpdateEnd();
                             ucSubReason = (uint8_t)(ERR_SUCCESS);
                             DEBUG_CS1131( "Write File End OK\n" );
                        }
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadSrcFileStart
*
* 功能描述: 解析读文件开始命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址：Head(16) + Type(1) + ID(1)；
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

static void DecodeReadSrcFileStart( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;      /* Actual Request Message Type */
    uint16_t usDataIndex = 0U;  /* Data Area Index */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usDataIndex = (uint16_t)(APP_MSG_HEADER_LEN);  /* File Type Index */
        usAType = GetReqActualType( &pucReq[0]);

        /* Check Actual Type and File Type */
        if(((uint16_t)(CS1131_READFILE_START) == usAType) &&
           ((uint8_t)(FT_SRC) == pucReq[usDataIndex]))
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                   ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                {
                    DEBUG_CS1131( "Read File Start OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                    pstSWInfo->uiSrcFileROffset = 0U;
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                    LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadSrcFileCont
*
* 功能描述: 解析读文件继续命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址：Head(16) + Type(1) + ID(1)；
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

static void DecodeReadSrcFileCont( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;      /* Actual Request Message Type */
    uint16_t usDataIndex = 0U;  /* Data Area Index */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usDataIndex = (uint16_t)(APP_MSG_HEADER_LEN);  /* File Type Index */
        usAType = GetReqActualType( &pucReq[0]);

        /* Check Actual Type and File Type */
        if(((uint16_t)(CS1131_READFILE_CONT) == usAType) &&
           ((uint8_t)(FT_SRC) == pucReq[usDataIndex]))
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                   ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                {
                    DEBUG_CS1131( "Read File Continue OK\n" );
                    ucSubReason = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                    LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeClearSrcFile
*
* 功能描述: 解析清除源文件命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：软件命令首地址；
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

static void DecodeClearSrcFile( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( &pucReq[0]);
        if((uint16_t)(CS1131_CLR_SRCFILE) == usAType )
        {
            /* LOGIN & AL_ALL */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth )
                    {
                        /* Clear Source File */
                        if( !s_stFUPOprInfo.bAllowFUP )
                        {
                            ucSubReason = (uint8_t)(ERR_SRCUP_INIT);
                            DEBUG_CS1131( "SrcUP resource initialized failed!\n" );
                        }
                        else
                        {
                            if( !IsOprMsgListEmpty( &s_stFUPOprInfo.stOprMsgList ))
                            {
                                ucSubReason = (uint8_t)(ERR_SENDOPRMSG_NOT_FIN);
                                DEBUG_CS1131( "SrcUP operation message not send finished!\n" );
                            }
                            else
                            {
                                /* Clear Send error info. */
                                memset( &s_stFUPOprInfo.stSendEInfo, 0, sizeof(OprRetInfo_t));

                                /* Update Start */
                                ucSubReason = SrcUpdateStart();
                                if((uint8_t)(ERR_SUCCESS) == ucSubReason )
                                {
                                    DEBUG_CS1131( "Clear Src File OK\n" );
                                }
                            }
                        }
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadStatus
*
* 功能描述: 解析读取CM及PM状态命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令；
*          pstResp：用于存放构造好的应答帧。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeReadStatus( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */
    uint16_t usTypeIndex = 0U;
    uint8_t ucType = 0U;

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_READ_STATUS) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                   ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                {
                    usTypeIndex = (uint16_t)(APP_MSG_HEADER_LEN);
                    ucType = pucReq[usTypeIndex];
                    if( ucType > NUM_OF_REQTYPE )
                    {
                        ucSubReason = (uint8_t)(ERR_READ_STS_TYPE);
                        LogWrite( (uint32_t)(LOGID_CS1131_E_READ_STS_TYPE));
                    }
                    else
                    {
                        DEBUG_CS1131( "Read PM/CM Status OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                        if((SRT_ALL == ucType) || (SRT_PM == ucType))
                        {
                            /* Clear Wait Flag */
                            s_stPMTaskInfo.bWait = false;

                            /* Handle Task Status Info.*/
                            HandlePMTaskInfo( &s_stPMTaskInfo );
                        }
                    }
                }
                else
                {
                    ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                    LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            if( !s_stPMTaskInfo.bWait )
            {
                /* Make Response Message */
                if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
                {
                    DEBUG_CS1131( "MakeRespMsg Err\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeDefMonVarTable
*
* 功能描述: 解析定义监视变量表命令
*
* 输入参数：pstSWInfo：指向相应软件的本地信息；
*          pucReq：指向软件命令首地址；
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

static void DecodeDefMonVarTable( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;         /* Actual Request Message Type */
    uint16_t usReqIndex = 0U;      /* Request Message Index */
    uint16_t usVarIndex = 0U;      /* Variable Index */
    uint16_t usTotalVarCnts = 0U;  /* Total Variable Counts In Request Message */
    uint16_t usErrCnts = 0U;       /* Error Variable Counts */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_DEF_VARTABLE) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                       ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                    {
                        usReqIndex = (uint16_t)(APP_MSG_HEADER_LEN);
                        usTotalVarCnts = (pucReq[usReqIndex] << 8U) | pucReq[usReqIndex + 1U];

                        if( usTotalVarCnts <= (uint16_t)(CS1131_MAX_MON_CNT))
                        {
                            usReqIndex += 4U;  /* Total Counts: 2 + Reserve: 2 */

                            /* Copy To Local Table */
                            pstSWInfo->stMonTblInfo.usMonVarCnt = 0U;
                            memcpy( &pstSWInfo->stMonTblInfo.stMonVarTable[0], &pucReq[usReqIndex], \
                                    sizeof(VarInfo_t)*usTotalVarCnts );

                            /* Reset Error Info. Before Check */
                            memset( &s_stVarErrInfo, 0, sizeof(VarErrInfo_t));

                            /* Loop Variable */
                            for( usVarIndex = 0U; usVarIndex < usTotalVarCnts; usVarIndex++ )
                            {
                                if( !CheckMonitorVar( &pstSWInfo->stMonTblInfo.stMonVarTable[usVarIndex]))
                                {
                                    usErrCnts++;

                                    if( 1U == usErrCnts )
                                    {
                                        s_stVarErrInfo.usFstErrIndex = usVarIndex;
                                        memcpy( &s_stVarErrInfo.stVarInfo, \
                                                &pstSWInfo->stMonTblInfo.stMonVarTable[usVarIndex], sizeof(VarInfo_t));
                                    }
                                }
                            }

                            /* Error Counts & Set SubReason */
                            s_stVarErrInfo.usErrCnts = usErrCnts;
                            if( 0U == s_stVarErrInfo.usErrCnts )
                            {
                                pstSWInfo->stMonTblInfo.usMonVarCnt = usTotalVarCnts;  /* Set Monitor Variable Count */
                                DEBUG_CS1131( "DefVarTable OK\n" );
                            }
                            else
                            {
                                ucSubReason = (uint8_t)(ERR_DEF_VARIABLE);
                                LogWrite( (uint32_t)(LOGID_CS1131_E_DEF_VARIABLE));
                            }
                        }
                        else
                        {
                            ucSubReason = (uint8_t)(ERR_TOTAL_CNT);  /* Total Variable Counts Invalid */
                            LogWrite( (uint32_t)(LOGID_CS1131_E_TOTAL_CNT));
                        }
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
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

static void DecodeReadVarOnLine( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_READ_VARONLINE) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                       ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                    {
                        DEBUG_CS1131( "ReadVarOnline OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        DEBUG_CS1131( "No Access Authority\n" );
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                }
            }
            else
            {
                DEBUG_CS1131( "Need to login first\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadStationID
*
* 功能描述: 解析读取站节点命令
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

static void DecodeReadStationID( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_READ_STAID) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                       ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                    {
                        DEBUG_CS1131( "Read StaID OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeReadIOStatus
*
* 功能描述: 解析读取所有IO模块状态服务
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

static void DecodeReadIOStatus( CS1131Info_t *pstSWInfo, uint8_t const pucReq[], RespMsg_t *pstResp, bool_t bCMHasCfg )
{
    uint8_t ucSubReason = (uint8_t)(ERR_SUCCESS);
    uint16_t usAType = 0U;  /* Actual Request Message Type */

    if((pstSWInfo != NULL) &&
       (pucReq != NULL) &&
       (pstResp != NULL))
    {
        usAType = GetReqActualType( pucReq );
        if((uint16_t)(CS1131_READ_IO_STATUS) == usAType )
        {
            /* LOGIN & Not AL_DENY */
            if( LOGIN == pstSWInfo->emLog )
            {
                if( bCMHasCfg )
                {
                    if(((uint8_t)(AL_READ_ONLY) == pstSWInfo->ucAccAuth) ||
                       ((uint8_t)(AL_ALL) == pstSWInfo->ucAccAuth))
                    {
                        DEBUG_CS1131( "Read IO Status OK\n" );
                        ucSubReason = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucSubReason = (uint8_t)(ERR_ACCESS);  /* No access authority */
                        LogWrite( (uint32_t)(LOGID_CS1131_E_ACCESS));
                    }
                }
                else
                {
                    DEBUG_CS1131( "CM No Cfg\n" );
                    ucSubReason = (uint8_t)(ERR_CMNOCFG);
                    LogWrite( (uint32_t)(LOGID_CS1131_E_CM_NOCFG));
                }
            }
            else
            {
                ucSubReason = (uint8_t)(ERR_LOG);  /* Need to login first */
                LogWrite( (uint32_t)(LOGID_CS1131_E_NOTLOGIN));
            }

            /* Make Response Message */
            if( !MakeRespMsg( pstSWInfo, ucSubReason, pucReq, pstResp ))
            {
                DEBUG_CS1131( "MakeRespMsg Err\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadSrcFile
*
* 功能描述: 读文件数据
*
* 输入参数: pucPathName：文件路径名；
*          pucBuff：用于存放读出的数据；
*          uiCount：目标读取字节数；
*          iReadOffSet：偏移值；
*          pbContinue：是否还需要继续读标志。
*
* 输出参数: 无
*
* 返 回 值: 读取的实际长度，-1代表读操作失败, 0代表文件已被读取完毕
*
* 注意事项: 无
***********************************************************************************************************************
*/

static int32_t ReadSrcFile( int32_t iFD, uint8_t pucBuff[], uint32_t uiCount, off_t iReadOffSet, bool_t *pbContinue )
{
    int32_t iReadCnt = -1;

#ifdef DEBUG_SWCS1131
    uint32_t uiTime = 0U;
    static uint32_t uiMaxTime = 0U;
    struct timeval stPreTime;
    struct timeval stNextTime;
#endif

    if((iFD != (int32_t)(INVALID_FD)) &&
       (pucBuff != NULL) &&
       (uiCount > 0U) &&
       (iReadOffSet >= 0) &&
       (pbContinue != NULL))
    {
#ifdef DEBUG_SWCS1131
        gettimeofday( &stPreTime, NULL );
#endif

        /* Read File */
        iReadCnt = ReadFile( iFD, iReadOffSet, &pucBuff[0], uiCount );
        if( iReadCnt != -1 )
        {
#ifdef DEBUG_SWCS1131
            gettimeofday( &stNextTime, NULL );

            if( stNextTime.tv_usec >= stPreTime.tv_usec )
            {
                uiTime = stNextTime.tv_usec - stPreTime.tv_usec;
            }
            else
            {
                uiTime = 1000000 + stNextTime.tv_usec - stPreTime.tv_usec;
            }

            if( uiTime > uiMaxTime )
            {
                uiMaxTime = uiTime;
            }

            DEBUG_CS1131( "ReadFile: Len %d; Time %d, MaxTime %d\n", iReadCnt, uiTime, uiMaxTime );
#endif

            if( 0 == iReadCnt )
            {
                *pbContinue = false;  /* read done */
            }
            else
            {
                *pbContinue = true;
            }
        }
        else
        {
            DEBUG_CS1131( "ReadSrcFile Failed %d\n", errno );
            LogWrite( (uint32_t)(LOGID_CS1131_E_READ_FILE));
        }
    }

    return iReadCnt;
}

/*
***********************************************************************************************************************
* 函数名称: ComparePMProjVer
*
* 功能描述: 比较两个PM的工程版本是否一致
*
* 输入参数: pstPM1ProjVer： PM1的工程版本；
*          pstPM2ProjVer： PM2的工程版本。
*
* 输出参数: 无
*
* 返 回 值: 是否一致
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ComparePMProjVer( ProjVerInfo_t const *pstPM1ProjVer, ProjVerInfo_t const *pstPM2ProjVer )
{
    bool_t bSame = false;

    if((pstPM1ProjVer != NULL) && (pstPM2ProjVer != NULL))
    {
        if((pstPM1ProjVer->uiMainVer == pstPM2ProjVer->uiMainVer) &&
           (pstPM1ProjVer->uiMinorVer == pstPM2ProjVer->uiMinorVer) &&
           (pstPM1ProjVer->uiProjCRC == pstPM2ProjVer->uiProjCRC) &&
           (pstPM1ProjVer->uiProjID == pstPM2ProjVer->uiProjID))
        {
            bSame = true;
        }
    }

    return bSame;
}

/*
***********************************************************************************************************************
* 函数名称: IsPMProjValid
*
* 功能描述: 判断PM工程版本是否有效：全零-无效
*
* 输入参数: pstPMProjVer： PM工程版本
*
* 输出参数: 无
*
* 返 回 值: 是否有效标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t IsPMProjValid( ProjVerInfo_t const *pstPMProjVer )
{
    bool_t bValid = false;

    if( pstPMProjVer != NULL )
    {
        if((pstPMProjVer->uiMainVer != 0U) ||
           (pstPMProjVer->uiMinorVer != 0U) ||
           (pstPMProjVer->uiProjCRC != 0U) ||
           (pstPMProjVer->uiProjID != 0U))
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: HandleTotalProjVer
*
* 功能描述: 处理总的工程版本信息：根据工程版本信息和内部通讯状态，选取PM
*
* 输入参数: pstPMTaskInfo：PM任务信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleTotalProjVer( TotalProjVerInfo_t *pstTotalProjVer )
{
    ProjVerInfo_t stPMAProjVer;  /* PMA Project Version  PMA工程版本  */
    ProjVerInfo_t stPMBProjVer;  /* PMB Project Version  PMB工程版本 */
    ProjVerInfo_t stPMCProjVer;  /* PMC Project Version  PMC工程版本 */

    if( pstTotalProjVer != NULL )
    {
        /* Project Version */
        SysGetPMProjVersion( PMA, &stPMAProjVer );
        SysGetPMProjVersion( PMB, &stPMBProjVer );
        SysGetPMProjVersion( PMC, &stPMCProjVer );

        if( IsPMProjValid( &stPMAProjVer ))
        {
            pstTotalProjVer->bPMAProjValid = true;
        }

        if( IsPMProjValid( &stPMBProjVer ))
        {
            pstTotalProjVer->bPMBProjValid = true;
        }

        if( IsPMProjValid( &stPMCProjVer ))
        {
            pstTotalProjVer->bPMCProjValid = true;
        }

        /* Internal Communication Status */
        if( pstTotalProjVer->bPMAProjValid )
        {
            if( SysGetLocalInterCommState( PMA ) != (uint16_t)(INTER_COMM_NORMAL))
            {
                pstTotalProjVer->bPMAProjValid = false;
            }
        }

        if( pstTotalProjVer->bPMBProjValid )
        {
            if( SysGetLocalInterCommState( PMB ) != (uint16_t)(INTER_COMM_NORMAL))
            {
                pstTotalProjVer->bPMBProjValid = false;
            }
        }

        if( pstTotalProjVer->bPMCProjValid )
        {
            if( SysGetLocalInterCommState( PMC ) != (uint16_t)(INTER_COMM_NORMAL))
            {
                pstTotalProjVer->bPMCProjValid = false;
            }
        }

        if( pstTotalProjVer->bPMAProjValid && pstTotalProjVer->bPMBProjValid && pstTotalProjVer->bPMCProjValid )
        {
            if( ComparePMProjVer( &stPMAProjVer, &stPMBProjVer ) &&
                ComparePMProjVer( &stPMAProjVer, &stPMCProjVer ))
            {
                DEBUG_CS1131( "All PrjVer Same, Select PMA\n" );
                pstTotalProjVer->emSelPMID = PMA;
            }
            else
            {
                DEBUG_CS1131( "PMA, B & C PrjVer Not Same, Select None\n" );
                pstTotalProjVer->emSelPMID = INVALID_PM_ID;
            }
        }
        else if( pstTotalProjVer->bPMAProjValid && pstTotalProjVer->bPMBProjValid )
        {
            if( ComparePMProjVer( &stPMAProjVer, &stPMBProjVer ))
            {
                DEBUG_CS1131( "PMA & B PrjVer Same, Select PMA\n" );
                pstTotalProjVer->emSelPMID = PMA;
            }
            else
            {
                DEBUG_CS1131( "PMA & B PrjVer Not Same, Select None\n" );
                pstTotalProjVer->emSelPMID = INVALID_PM_ID;
            }
        }
        else if( pstTotalProjVer->bPMAProjValid && pstTotalProjVer->bPMCProjValid )
        {
            if( ComparePMProjVer( &stPMAProjVer, &stPMCProjVer ))
            {
                DEBUG_CS1131( "PMA & C PrjVer Same, Select PMA\n" );
                pstTotalProjVer->emSelPMID = PMA;
            }
            else
            {
                DEBUG_CS1131( "PMA & C PrjVer Not Same, Select None\n" );
                pstTotalProjVer->emSelPMID = INVALID_PM_ID;
            }
        }
        else if( pstTotalProjVer->bPMBProjValid && pstTotalProjVer->bPMCProjValid )
        {
            if( ComparePMProjVer( &stPMBProjVer, &stPMCProjVer ))
            {
                DEBUG_CS1131( "PMB & C PrjVer Same, Select PMB\n" );
                pstTotalProjVer->emSelPMID = PMB;
            }
            else
            {
                DEBUG_CS1131( "PMB & C PrjVer Not Same, Select None\n" );
                pstTotalProjVer->emSelPMID = INVALID_PM_ID;
            }
        }
        else if( pstTotalProjVer->bPMAProjValid )
        {
            DEBUG_CS1131( "Only PMA PrjVer Valid, Select PMA\n" );
            pstTotalProjVer->emSelPMID = PMA;
        }
        else if( pstTotalProjVer->bPMBProjValid )
        {
            DEBUG_CS1131( "Only PMB PrjVer Valid, Select PMB\n" );
            pstTotalProjVer->emSelPMID = PMB;
        }
        else if( pstTotalProjVer->bPMCProjValid )
        {
            DEBUG_CS1131( "Only PMC PrjVer Valid, Select PMC\n" );
            pstTotalProjVer->emSelPMID = PMC;
        }
        else
        {
            DEBUG_CS1131( "No PM PrjVer Valid, Select None\n" );
            pstTotalProjVer->emSelPMID = INVALID_PM_ID;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandlePMTaskInfo
*
* 功能描述: 处理PM任务信息
*
* 输入参数: pstPMTaskInfo：PM任务信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandlePMTaskInfo( PMTaskInfo_t *pstPMTaskInfo )
{
    uint8_t ucTaskNum = 0U;
    uint8_t ucTaskIndex = 0U;
    TotalProjVerInfo_t stTotalProjVer;
    stTotalProjVer.bPMAProjValid = false;
    stTotalProjVer.bPMBProjValid = false;
    stTotalProjVer.bPMCProjValid = false;
    stTotalProjVer.emSelPMID = INVALID_PM_ID;

    if( pstPMTaskInfo != NULL )
    {
        /* Handle Project Version Info. */
        HandleTotalProjVer( &stTotalProjVer );
        if( stTotalProjVer.emSelPMID != INVALID_PM_ID )
        {
            pstPMTaskInfo->bWait = false;
            ucTaskNum = SysGetPMTaskNum( stTotalProjVer.emSelPMID );
            if( ucTaskNum <= (uint8_t)(MAX_TASK_NUM))
            {
                pstPMTaskInfo->icPMTaskNum = (int8_t)ucTaskNum;

                /* Task Status */
                for( ucTaskIndex = 0U; ucTaskIndex < ucTaskNum; ucTaskIndex++ )
                {
                    /* Voted */
                    SysGetPMTaskInfo( stTotalProjVer.emSelPMID, ucTaskIndex, \
                                      &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stVTaskInfo.ucTaskID,
                                      &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stVTaskInfo.icTaskStatus );

                    /* PMA */
                    if( stTotalProjVer.bPMAProjValid )
                    {
                        SysGetPMTaskInfo( PMA, ucTaskIndex, \
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMATaskInfo.ucTaskID,
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMATaskInfo.icTaskStatus );

                    }
                    else
                    {
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMATaskInfo.ucTaskID = (uint8_t)(INVALID_TASK_ID);
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMATaskInfo.icTaskStatus = (int8_t)(INVALID_TASK_STATUS);
                    }

                    /* PMB */
                    if( stTotalProjVer.bPMBProjValid )
                    {
                        SysGetPMTaskInfo( PMB, ucTaskIndex, \
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMBTaskInfo.ucTaskID,
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMBTaskInfo.icTaskStatus );

                    }
                    else
                    {
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMBTaskInfo.ucTaskID = (uint8_t)(INVALID_TASK_ID);
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMBTaskInfo.icTaskStatus = (int8_t)(INVALID_TASK_STATUS);
                    }

                    /* PMC */
                    if( stTotalProjVer.bPMCProjValid )
                    {
                        SysGetPMTaskInfo( PMC, ucTaskIndex, \
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMCTaskInfo.ucTaskID,
                                          &pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMCTaskInfo.icTaskStatus );

                    }
                    else
                    {
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMCTaskInfo.ucTaskID = (uint8_t)(INVALID_TASK_ID);
                        pstPMTaskInfo->stPMTaskInfo[ucTaskIndex].stPMCTaskInfo.icTaskStatus = (int8_t)(INVALID_TASK_STATUS);
                    }
                }
            }
            else
            {
                pstPMTaskInfo->icPMTaskNum = (int8_t)(INVALID_TASK_NUM);
            }
        }
        else
        {
            pstPMTaskInfo->bWait = true;
            pstPMTaskInfo->icPMTaskNum = (int8_t)(INVALID_TASK_NUM);
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
    CS1131Info_t *pstSWInfo = NULL;  /* Current Software Information */
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);

    if((s_emResp != NONE_RESP) &&
       (s_ucCurSWIndex < (uint8_t)(APP_SW_CS1131_NUM)))
    {
        pstSWInfo = &( s_stCS1131Info[s_ucCurSWIndex]);
        if( PM_RESP == s_emResp )
        {
            /* Read PM Response Message */
            s_stRespMsg.usLen = (uint16_t)sizeof( s_stRespMsg.ucBuff );
            if( !SysGetMsg( SwitchPMID( pstSWInfo->emSelPMID), CS1131_RESP, s_stRespMsg.ucBuff, &s_stRespMsg.usLen ))
            {
                s_stRespMsg.usLen = 0U;
            }

            if( 0U == s_stRespMsg.usLen )
            {
                /* No Response Message */
                if( s_uiWaitPMCycleCnt >= (uint32_t)(CS1131_PM_TIMEOUT_CYCLE_CNT))
                {
                    /* Record Error Info.: PM Timeout */
                    DEBUG_CS1131( "HandleResp: PMResp Timeout\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_RESP_PM_TIMEOUT));
                    s_uiWaitPMCycleCnt = 0U;

                    /* Make Response Message */
                    if( !MakeRespMsg( pstSWInfo, (uint8_t)(ERR_PMBUSY), s_stReqMsg.ucBuff, &s_stRespMsg ))
                    {
                        DEBUG_CS1131( "MakeRespMsg Err\n" );
                        LogWrite( (uint32_t)(LOGID_CS1131_E_MAKE_RESP));
                    }

                    s_emResp = CM_RESP;
                }
                else
                {
                    s_uiWaitPMCycleCnt++;
                    DEBUG_CS1131( "HandleResp: Wait PMResp-%d\n", s_uiWaitPMCycleCnt );
                }
            }
        }

        if( 0U == s_stRespMsg.usLen )
        {
            if( CM_RESP == s_emResp )  /* CM Response */
            {
                /* Record Error Info: s_stRespMsg.usLen should greater than zero */
                DEBUG_CS1131( "HandleResp: CM Resp - s_stRespMsg.usLen Invalid\n" );
                if( !SysResetAppSwLockedFlag( APP_CS1131, s_ucCurSWIndex ))
                {
                    DEBUG_CS1131( "HandleResp: Reset Locked Flag Failed\n" );
                    LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_RST_LOCK));
                }

                s_ucCurSWIndex++;
                s_ucCurSWIndex = s_ucCurSWIndex % (uint8_t)(APP_SW_CS1131_NUM);
                s_emState = IDLE_STATE;
            }
        }
        else
        {
            /* Send Response Message */
            ucResult = Send( s_stRespMsg.ucBuff, s_stRespMsg.usLen,
                             pstSWInfo->uiIPAddr, pstSWInfo->usPort, pstSWInfo->emNetwork,
                             TYPE_CS1131, s_ucCurSWIndex );

            /* Reset Lock Flag */
            if( !SysResetAppSwLockedFlag( APP_CS1131, s_ucCurSWIndex ))
            {
                DEBUG_CS1131( "HandleResp: Reset Locked Flag Failed\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_SYS_RST_LOCK));
            }

            if( ucResult != (uint8_t)(SEND_SUCCESS))
            {
                /* Send Failed: Record Error Info */
                DEBUG_CS1131( "HandleResp: ExtCommSend Failed\n" );
                LogWrite( (uint32_t)(LOGID_CS1131_E_RESP_EXCOMM_SEND));

                /* Clear Local Info */
                ClearCS1131LocalInfo( s_ucCurSWIndex );

                /* Clear System Info */
                ClearCS1131SysInfo( s_ucCurSWIndex );

                /* Check if this software writing source file: if yes, clear its info. */
                HandleClrWriteSrcFileInfo( s_ucCurSWIndex );
            }
            else
            {
                DEBUG_CS1131( "HandleResp: ExtCommSend OK\n" );
            }

            s_ucCurSWIndex++;
            s_ucCurSWIndex = s_ucCurSWIndex % (uint8_t)(APP_SW_CS1131_NUM);
            s_emState = IDLE_STATE;
        }
    }/* end of if */
    else
    {
        DEBUG_CS1131( "HandleResp: s_emResp-%d / s_ucCurSWIndex-%d Invalid\n", s_emResp, s_ucCurSWIndex );
        s_emState = IDLE_STATE;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SrcUpdateStart
*
* 功能描述: 源文件更新开始
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SrcUpdateStart( void )
{
    uint8_t ucErrCode = (uint8_t)(ERR_SUCCESS);
    FUPOprMsg_t stOprMsg;

    /* Make Operation Message */
    memset( &stOprMsg, 0, sizeof(FUPOprMsg_t));
    stOprMsg.iMsgType = MSGT_OPR;

    stOprMsg.stOprInfo.ucOprType = OPRT_UPSTART;
    stOprMsg.stOprInfo.ucFileType = FUT_SRC;

    /* Update Local Control Info. */
    s_stFUPOprInfo.uiShmOffset = (uint32_t)(BASE_ADDR_SRC);

    /* Write Operation Message */
    ucErrCode = WriteOprMsg( &s_stFUPOprInfo.stOprMsgList, &stOprMsg );
    if( ucErrCode != (uint8_t)(ERR_SUCCESS))
    {
        InitOprMsgList( &s_stFUPOprInfo.stOprMsgList );  /* Clear */
        DEBUG_CS1131( "SrcUpdateStart: WriteOprMsg failed: %d\n", ucErrCode );
    }
    else
    {
        /* Save write source file info. */
        s_stWSrcInfo.bWrite = true;
        s_stWSrcInfo.ucSWIndex = s_ucCurSWIndex;
    }

    return ucErrCode;
}

/*
***********************************************************************************************************************
* 函数名称: SrcUpdateCont
*
* 功能描述: CM Src更新继续
*
* 输入参数：pucData：数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SrcUpdateCont( uint8_t const pucData[], uint16_t usDataLen )
{
    uint8_t ucErrCode = (uint8_t)(ERR_PARA);
    FUPOprMsg_t stOprMsg;

    if( pucData != NULL )
    {
        /* offset: [BASE_ADDR_SRC, SHM_FUP_SIZE) */
        if(((s_stFUPOprInfo.uiShmOffset >= (uint32_t)(BASE_ADDR_SRC)) && (s_stFUPOprInfo.uiShmOffset < (uint32_t)(SHM_FUP_SIZE))) &&
           (s_stFUPOprInfo.uiShmOffset + (uint32_t)usDataLen <= (uint32_t)(SHM_FUP_SIZE)))
        {
            /* Copy to shared memory */
            CopyToShm( s_stFUPOprInfo.uiShmOffset, &pucData[0], (uint32_t)usDataLen );

            /* Make Operation Message */
            memset( &stOprMsg, 0, sizeof(FUPOprMsg_t));
            stOprMsg.iMsgType = MSGT_OPR;

            stOprMsg.stOprInfo.ucOprType = OPRT_UPCONT;
            stOprMsg.stOprInfo.ucFileType = FUT_SRC;
            stOprMsg.stOprInfo.usFDataLen = usDataLen;
            stOprMsg.stOprInfo.uiFDataOffset = s_stFUPOprInfo.uiShmOffset;

            /* Update Local Control Info. */
            s_stFUPOprInfo.uiShmOffset += (uint32_t)usDataLen;

            /* Write Operation Message */
            ucErrCode = WriteOprMsg( &s_stFUPOprInfo.stOprMsgList, &stOprMsg );
            if( ucErrCode != (uint8_t)(ERR_SUCCESS))
            {
                InitOprMsgList( &s_stFUPOprInfo.stOprMsgList );  /* Clear */
                DEBUG_CS1131( "SrcUpdateCont: WriteOprMsg failed: %d\n", ucErrCode );
            }
            else
            {
                DEBUG_CS1131( "SrcUpdateCont: Len %d\n", s_stFUPOprInfo.uiShmOffset );
            }
        }
        else
        {
            ucErrCode = (uint8_t)(ERR_CM_SRCUP_FILE_LEN);
            DEBUG_CS1131( "SrcUpdateCont-Len error: Offset %d, DataLen %d\n", s_stFUPOprInfo.uiShmOffset, usDataLen );
        }
    }

    return ucErrCode;
}

/*
***********************************************************************************************************************
* 函数名称: SrcUpdateEnd
*
* 功能描述: 源文件更新完成
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SrcUpdateEnd( void )
{
    s_stFUPOprInfo.uiShmOffset = 0U;

    /* Clear Write Source File Info */
    ClearWriteSrcFileInfo( &s_stWSrcInfo );

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

