/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : P2P模块
*
*  文件名称    : P2P.c
*
*  功能简介    : 处理P2P请求和应答消息
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
#include "../inc/P2P.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                        /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                    /* 本控制站的ID */
static uint8_t s_ucLocalCMSlotID;                 /* 本CM模块的槽位号 */
static PMP2PReqItemInfo_t s_stPMP2PReqItem;       /* 用于处理本站PM发送的请求帧 */
static PMP2PRespMsgInfo_t s_stPMP2PRespInfo;      /* 用于接收本站PM模块发送的应答帧 */
static ModuleState_t s_emState;                   /* 状态 */
static SelPMID_t s_emSelPMID;                     /* 选取的PM模块的ID */
static uint32_t s_uiWaitPMCycleCnt;               /* 已等待PM应答的拍数 */
static uint8_t s_ucP2PDataBuff[MAX_P2P_MSG_LEN];  /* 站间数据缓冲区 */
static PMP2PRespItemInfo_t s_stRespInfo;          /* 用于处理本站PM发送的应答帧 */
static uint8_t s_ucExtReqStaID;                   /* 外部请求控制站的ID */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Initialize */
static void ClearPMP2PReqItemInfo( void );
static void ClearLocalInfo( void );

/* Cycle */
static void HandleLocalStaReq( void );
static void HandleOtherStaReq( void );
static void HandleLocalStaResp( void );
static void HandleOtherStaResp( void );

/* Check Function */
static bool_t CheckLocalPMP2PReq( PMP2PReqItemInfo_t *pstPMP2PReqItem, bool_t bComplete );
static bool_t CheckPMReqData( uint8_t const pucReq[], uint16_t usLen );
static bool_t CheckPMRespData( uint8_t const pucResp[], uint16_t usLen, uint8_t ucExtReqStaID );
static bool_t CheckStaReqData( uint8_t const pucReq[], uint16_t usLen );
static bool_t CheckStaRespData( uint8_t const pucResp[], uint16_t usLen );

/* Make Message */
static void MakeTotalReq( PMP2PReqItemInfo_t const *pstPMP2PReqItem, uint8_t pucBuff[], puint16_t pusLen );
static void MakeRespMsg( PMP2PRespMsgInfo_t const *pstPMP2PRespInfo, uint8_t pucBuff[], puint16_t pusLen );

/* Function */
static void HandleWait( void );
static void GetNetInfo( PMP2PReqItemInfo_t const *pstPMP2PReqItem, NetInfo_t *pstReqNetInfo );
static void P2PSendReq( NetInfo_t const *pstNetInfo, uint8_t pucBuff[], uint16_t usLen );
static void P2PSendResp( NetInfo_t const *pstNetInfo, uint8_t const pucBuff[], uint16_t usLen );
static SelPMID_t SelectPM( void );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: P2PInit
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

void P2PInit( void )
{
    DEBUG_P2P( "P2PInit\n" );

    s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();
    s_emState = IDLE_STATE;
    s_emSelPMID = SELPM_NONE;
    s_uiWaitPMCycleCnt = 0U;
    s_ucExtReqStaID = (uint8_t)(INVALID_STAID);

    /* Clear */
    memset( &s_stPMP2PReqItem, 0, sizeof(PMP2PReqItemInfo_t));
    memset( &s_stPMP2PRespInfo, 0, sizeof(PMP2PRespMsgInfo_t));
    memset( s_ucP2PDataBuff, 0, sizeof(s_ucP2PDataBuff));
    memset( &s_stRespInfo, 0, sizeof(PMP2PRespItemInfo_t));

    ClearPMP2PReqItemInfo();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: P2PCycle
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

void P2PCycle( void )
{
    static bool_t s_bClrDone = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();
    if( !s_bCMHasCfg )
    {
        if( !s_bClrDone )
        {
            ClearLocalInfo();
            s_bClrDone = true;
        }

        /* Clear request message from other stations */
        if( !SysClearStationMsg( P2P_EXT_REQ ))
        {
            /*DEBUG_P2P( "SysClearStationMsg: ExtReq failed\n" );*/
        }

        /* Clear response message from other stations */
        if( !SysClearStationMsg( P2P_EXT_RESP ))
        {
            /*DEBUG_P2P( "SysClearStationMsg: ExtResp failed\n" );*/
        }

        /* Clear request message from local PM */
        if( !SysClearP2PMsg( PMA, P2P_PM_REQ ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMA Req\n" );*/
        }

        if( !SysClearP2PMsg( PMB, P2P_PM_REQ ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMB Req\n" );*/
        }

        if( !SysClearP2PMsg( PMC, P2P_PM_REQ ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMC Req\n" );*/
        }

        /* Clear response message from local PM */
        if( !SysClearP2PMsg( PMA, P2P_PM_RESP ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMA Resp\n" );*/
        }

        if( !SysClearP2PMsg( PMB, P2P_PM_RESP ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMB Resp\n" );*/
        }

        if( !SysClearP2PMsg( PMC, P2P_PM_RESP ))
        {
            /*DEBUG_P2P( "SysClearP2PMsg Failed: PMC Resp\n" );*/
        }
    }
    else
    {
        s_bClrDone = false;

        /* Handle Local Station Request Message */
        HandleLocalStaReq();

        if( IDLE_STATE == s_emState )
        {
            HandleOtherStaReq();   /* Handle Other Station Request Message */
        }
        else
        {
            HandleLocalStaResp();  /* Handle Local Station Response Message */
        }

        /* Handle Other Station Response Message */
        HandleOtherStaResp();
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
* 函数名称: ClearPMP2PReqItemInfo
*
* 功能描述: 清除本站PM请求帧相关信息
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

static void ClearPMP2PReqItemInfo( void )
{
    s_stPMP2PReqItem.ucDstStaID = 0U;
    s_stPMP2PReqItem.bWaitFlag = false;
    s_stPMP2PReqItem.ucWaitCnt = 0U;
    s_stPMP2PReqItem.bPMAReqValid = false;
    s_stPMP2PReqItem.bPMBReqValid = false;
    s_stPMP2PReqItem.bPMCReqValid = false;
    s_stPMP2PReqItem.stPMAReq.usCount = 0U;
    s_stPMP2PReqItem.stPMBReq.usCount = 0U;
    s_stPMP2PReqItem.stPMCReq.usCount = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 无配置时，清除本地信息
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

static void ClearLocalInfo( void )
{
    s_emState = IDLE_STATE;
    s_emSelPMID = SELPM_NONE;
    s_uiWaitPMCycleCnt = 0U;
    s_ucExtReqStaID = (uint8_t)(INVALID_STAID);

    /* Clear */
    s_stPMP2PRespInfo.usCount = 0U;

    s_stRespInfo.bWaitFlag = false;
    s_stRespInfo.stPMAResp.usCount = 0U;
    s_stRespInfo.stPMBResp.usCount = 0U;
    s_stRespInfo.stPMCResp.usCount = 0U;

    ClearPMP2PReqItemInfo();
}

/*
***********************************************************************************************************************
* 函数名称: HandleLocalStaReq
*
* 功能描述: 处理本站请求帧
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

static void HandleLocalStaReq( void )
{
    uint16_t usDataLen = 0U;  /* Data Length */
    NetInfo_t stReqNetInfo;   /* Network info. of the request message */
    stReqNetInfo.uiDstIP1Addr = 0U;
    stReqNetInfo.uiDstIP2Addr = 0U;
    stReqNetInfo.emSrcNetwork = SRC_INVALID;

    if( s_stPMP2PReqItem.bWaitFlag )
    {
        HandleWait();
    }
    else
    {
        /* Clear Item Info */
        ClearPMP2PReqItemInfo();

        /* Read PMA, B And C Local Station Request Message */
        /* PMA */
        s_stPMP2PReqItem.stPMAReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMAReq.ucBuff );
        if( !SysGetP2PMsg( PMA, P2P_PM_REQ, s_stPMP2PReqItem.stPMAReq.ucBuff, &s_stPMP2PReqItem.stPMAReq.usCount ))
        {
            s_stPMP2PReqItem.stPMAReq.usCount = 0U;
        }

        /* PMB */
        s_stPMP2PReqItem.stPMBReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMBReq.ucBuff );
        if( !SysGetP2PMsg( PMB, P2P_PM_REQ, s_stPMP2PReqItem.stPMBReq.ucBuff, &s_stPMP2PReqItem.stPMBReq.usCount ))
        {
            s_stPMP2PReqItem.stPMBReq.usCount = 0U;
        }

        /* PMC */
        s_stPMP2PReqItem.stPMCReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMCReq.ucBuff );
        if( !SysGetP2PMsg( PMC, P2P_PM_REQ, s_stPMP2PReqItem.stPMCReq.ucBuff, &s_stPMP2PReqItem.stPMCReq.usCount ))
        {
            s_stPMP2PReqItem.stPMCReq.usCount = 0U;
        }

        if((s_stPMP2PReqItem.stPMAReq.usCount > 0U) ||
           (s_stPMP2PReqItem.stPMBReq.usCount > 0U) ||
           (s_stPMP2PReqItem.stPMCReq.usCount > 0U))
        {
            /* Has Request Message */
            DEBUG_P2P( "HandleLocalStaReq: Has Req Msg\n" );

            /* Set Wait Flag: No Request Message & Internal Communication Normal */
            if((0U == s_stPMP2PReqItem.stPMAReq.usCount) &&
               ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA )))
            {
                s_stPMP2PReqItem.bWaitFlag = true;
            }
            else
            {
                if((0U == s_stPMP2PReqItem.stPMBReq.usCount) &&
                   ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB )))
                {
                    s_stPMP2PReqItem.bWaitFlag = true;
                }
                else
                {
                    if((0U == s_stPMP2PReqItem.stPMCReq.usCount) &&
                       ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC )))
                    {
                        s_stPMP2PReqItem.bWaitFlag = true;
                    }
                }
            }

            if( !s_stPMP2PReqItem.bWaitFlag )
            {
                /* Check Request Message */
                if( CheckLocalPMP2PReq( &s_stPMP2PReqItem, true ))
                {
                    usDataLen = (uint16_t)sizeof( s_ucP2PDataBuff );
                    MakeTotalReq( &s_stPMP2PReqItem, s_ucP2PDataBuff, &usDataLen );
                    if( usDataLen > 0U )
                    {
                        /* Send To Dst Station */
                        GetNetInfo( &s_stPMP2PReqItem, &stReqNetInfo );
                        P2PSendReq( &stReqNetInfo, s_ucP2PDataBuff, usDataLen );
                    }
                }
                else
                {
                    DEBUG_P2P( "HandleLocalStaReq: No valid req msg\n" );
                    LogWrite( (uint32_t)(LOGID_P2P_E_MSG_PM_REQ));
                }

                /* Clear Item Info */
                ClearPMP2PReqItemInfo();
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleOtherStaReq
*
* 功能描述: 处理其他控制站请求帧
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

static void HandleOtherStaReq( void )
{
    /* Read Other Station Request Message */
    uint16_t usLen = (uint16_t)sizeof( s_ucP2PDataBuff );
    if( !SysGetStationMsg( P2P_EXT_REQ, s_ucP2PDataBuff, &usLen ))
    {
        usLen = 0U;
    }

    if( usLen > 0U )
    {
        /* Has Message */
        if( CheckStaReqData( s_ucP2PDataBuff, usLen ))
        {
            /* Select PM Module */
            s_emSelPMID = SelectPM();
            if( SELPM_NONE == s_emSelPMID )
            {
                /* Record Error Info.: No PM Valid */
                DEBUG_P2P( "HandleOtherStaReq: CM BUS Invalid\n" );
                LogWrite( (uint32_t)(LOGID_P2P_E_CMBUS));
            }
            else
            {
                /* Write Request Message To The Selected PM */
                if( SysSetP2PMsg( SwitchPMID( s_emSelPMID ), P2P_EXT_REQ, s_ucP2PDataBuff, usLen ))
                {
                    DEBUG_P2P( "HandleOtherStaReq: Send To PMID-%d\n", SwitchPMID( s_emSelPMID ));
                    s_uiWaitPMCycleCnt = 0U;
                    s_emState = BUSY_STATE;

                    /* Clear response message from the selected PM module */
                    /* Avoid old response messge make communication error */
                    if( !SysClearP2PMsg( SwitchPMID( s_emSelPMID ), P2P_PM_RESP ))
                    {
                        /*DEBUG_P2P( "PMHandle: SysClearMsg Failed, SelPMID-%d\n", emSelPMID );*/
                    }
                }
                else
                {
                    /* Record Error Info.: SysSetP2PMsg Failed */
                    DEBUG_P2P( "HandleOtherStaReq: SysSetP2PMsg Failed\n" );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SYS_SET_EXT_REQ));
                }
            }
        }
        else
        {
            /* Record Error Info.: Invalid */
            DEBUG_P2P( "HandleOtherStaReq: Other StaReqMsg Invalid\n" );
            LogWrite( (uint32_t)(LOGID_P2P_E_MSG_EXT_REQ));
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleLocalStaResp
*
* 功能描述: 处理本站应答帧
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

static void HandleLocalStaResp( void )
{
    uint16_t usLen = 0U;
    NetInfo_t stReqNetInfo;  /* Network info. of the request message */
    stReqNetInfo.uiDstIP1Addr = 0U;
    stReqNetInfo.uiDstIP2Addr = 0U;
    stReqNetInfo.emSrcNetwork = SRC_INVALID;
    PMP2PRespAddInfo_t const *pstPMResp = NULL;

    /* Read Local Station Response Message */
    s_stPMP2PRespInfo.usCount = (uint16_t)sizeof(s_stPMP2PRespInfo.ucBuff);
    if( !SysGetP2PMsg( SwitchPMID( s_emSelPMID ), P2P_PM_RESP, s_stPMP2PRespInfo.ucBuff, &s_stPMP2PRespInfo.usCount ))
    {
        s_stPMP2PRespInfo.usCount = 0U;
    }

    /* Check if valid response */
    if( s_stPMP2PRespInfo.usCount > 0U )
    {
        if( !CheckPMRespData( s_stPMP2PRespInfo.ucBuff, s_stPMP2PRespInfo.usCount, s_ucExtReqStaID ))
        {
            s_stPMP2PRespInfo.usCount = 0U;
            DEBUG_P2P( "HandleLocalStaResp: PMRespMsg Invalid\n" );
            LogWrite( (uint32_t)(LOGID_P2P_E_MSG_PM_RESP));
        }
    }

    if( s_stPMP2PRespInfo.usCount > 0U )
    {
        /* Has Valid Response Message */
        usLen = (uint16_t)sizeof( s_ucP2PDataBuff );
        MakeRespMsg( &s_stPMP2PRespInfo, s_ucP2PDataBuff, &usLen );
        if( usLen > 0U )
        {
            /* Send To Dst Station */
            pstPMResp = ( PMP2PRespAddInfo_t const *)&( s_stPMP2PRespInfo.ucBuff[0]);
            stReqNetInfo.emSrcNetwork = (SrcNet_t)pstPMResp->ucSrcNETID;
            stReqNetInfo.uiDstIP1Addr = pstPMResp->uiDstIP1;
            stReqNetInfo.uiDstIP2Addr = pstPMResp->uiDstIP2;

            P2PSendResp( &stReqNetInfo, s_ucP2PDataBuff, usLen );
        }

        s_ucExtReqStaID = (uint8_t)(INVALID_STAID);
        s_emState = IDLE_STATE;
    }
    else
    {
        /* No Response Message: Check if timeout */
        if( s_uiWaitPMCycleCnt >= (uint32_t)(P2P_PM_TIMEOUT_CYCLE_CNT))
        {
            /* PM Timeout: Record Error Info. */
            DEBUG_P2P( "HandleLocalStaResp: PM Timeout\n" );
            LogWrite( (uint32_t)(LOGID_P2P_E_PM_TIMEOUT));
            s_ucExtReqStaID = (uint8_t)(INVALID_STAID);
            s_emState = IDLE_STATE;
        }
        else
        {
            s_uiWaitPMCycleCnt++;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleOtherStaResp
*
* 功能描述: 处理其他站应答帧
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

static void HandleOtherStaResp( void )
{
    /* Read Other Station Response Message */
    uint16_t usLen = (uint16_t)sizeof( s_ucP2PDataBuff );
    if( !SysGetStationMsg( P2P_EXT_RESP, s_ucP2PDataBuff, &usLen ))
    {
        usLen = 0U;
    }

    if( usLen > 0U )
    {
       /* Has Message */
       if( CheckStaRespData( s_ucP2PDataBuff, usLen ))
       {
           /* Select PM Module */
           s_emSelPMID = SelectPM();
           if( SELPM_NONE == s_emSelPMID )
           {
               /* Record Error Info.: No PM Valid */
               DEBUG_P2P( "HandleOtherStaResp: CM BUS Invalid\n" );
               LogWrite( (uint32_t)(LOGID_P2P_E_CMBUS));
           }
           else
           {
               /* Write Request Message To The Selected PM */
               if( SysSetP2PMsg( SwitchPMID( s_emSelPMID ), P2P_EXT_RESP, s_ucP2PDataBuff, usLen ))
               {
                   DEBUG_P2P( "HandleOtherStaResp: Send To PMID: %d\n", SwitchPMID( s_emSelPMID ));
               }
               else
               {
                   /* Record Error Info. */
                   DEBUG_P2P( "HandleOtherStaResp: SysSetP2PMsg Failed\n" );
                   LogWrite( (uint32_t)(LOGID_P2P_E_SYS_SET_EXT_RESP));
               }
           }
       }
       else
       {
           /* Record Error Info.: Invalid */
           DEBUG_P2P( "HandleOtherStaResp: Other StaRespMsg Invalid\n" );
           LogWrite( (uint32_t)(LOGID_P2P_E_MSG_EXT_RESP));
       }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckLocalPMP2PReq
*
* 功能描述: 检查本站PM发送的请求帧
*
* 输入参数: pstPMP2PReqItem：指向保存本站所有PM发送的请求帧的结构体；
*          bComplete：是否已读取到所有在线PM的请求帧。
*
* 输出参数: 无
*
* 返 回 值: 是否存在有效的请求帧，true-存在；false-不存在
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckLocalPMP2PReq( PMP2PReqItemInfo_t *pstPMP2PReqItem, bool_t bComplete )
{
    bool_t bExist = false;
    bool_t bAllRespTimeout = false;  /* 均在应答超时情况下，发送下一请求帧 */

#ifdef DEBUG_P2PCOMM
    uint16_t usIndex = 0U;
#endif

    if( pstPMP2PReqItem != NULL )
    {
        /* Check Request Message Data */
        /* PMA */
        if( pstPMP2PReqItem->stPMAReq.usCount > 0U )
        {
            pstPMP2PReqItem->bPMAReqValid = CheckPMReqData( pstPMP2PReqItem->stPMAReq.ucBuff,
                                                            pstPMP2PReqItem->stPMAReq.usCount );
#ifdef DEBUG_P2PCOMM
            DEBUG_P2P( "PMA P2PReq: Len-%d, Valid-%d, Data(0x):\n", pstPMP2PReqItem->stPMAReq.usCount, \
                       pstPMP2PReqItem->bPMAReqValid );
            for( usIndex = 0U; usIndex < pstPMP2PReqItem->stPMAReq.usCount; usIndex++ )
            {
                printf( "%02X ", pstPMP2PReqItem->stPMAReq.ucBuff[usIndex]);
            }

            printf( "\n" );
#endif
        }
        else
        {
            pstPMP2PReqItem->bPMAReqValid = false;
        }

        /* PMB */
        if( pstPMP2PReqItem->stPMBReq.usCount > 0U )
        {
            pstPMP2PReqItem->bPMBReqValid = CheckPMReqData( pstPMP2PReqItem->stPMBReq.ucBuff,
                                                            pstPMP2PReqItem->stPMBReq.usCount );
#ifdef DEBUG_P2PCOMM
            DEBUG_P2P( "PMB P2PReq: Len-%d, Valid-%d, Data(0x):\n", pstPMP2PReqItem->stPMBReq.usCount, \
                       pstPMP2PReqItem->bPMBReqValid );
            for( usIndex = 0U; usIndex < pstPMP2PReqItem->stPMBReq.usCount; usIndex++ )
            {
                printf( "%02X ", pstPMP2PReqItem->stPMBReq.ucBuff[usIndex]);
            }

            printf( "\n" );
#endif
        }
        else
        {
            pstPMP2PReqItem->bPMBReqValid = false;
        }

        /* PMC */
        if( pstPMP2PReqItem->stPMCReq.usCount > 0U )
        {
            pstPMP2PReqItem->bPMCReqValid = CheckPMReqData( pstPMP2PReqItem->stPMCReq.ucBuff,
                                                            pstPMP2PReqItem->stPMCReq.usCount );
#ifdef DEBUG_P2PCOMM
            DEBUG_P2P( "PMC P2PReq: Len-%d, Valid-%d, Data(0x):\n", pstPMP2PReqItem->stPMCReq.usCount, \
                       pstPMP2PReqItem->bPMCReqValid );
            for( usIndex = 0U; usIndex < pstPMP2PReqItem->stPMCReq.usCount; usIndex++ )
            {
                printf( "%02X ", pstPMP2PReqItem->stPMCReq.ucBuff[usIndex]);
            }

            printf( "\n" );
#endif
        }
        else
        {
            pstPMP2PReqItem->bPMCReqValid = false;
        }

        /* Check Timeout Flag */
        if( bComplete )
        {
            /* Check if all valid msg timeout */
            bAllRespTimeout = true;
            if((pstPMP2PReqItem->bPMAReqValid) &&
               (pstPMP2PReqItem->stPMAReq.ucBuff[TIMEOUT_INDEX] != (uint8_t)(RESP_TIMEOUT)))
            {
                bAllRespTimeout = false;
            }
            else
            {
                if((pstPMP2PReqItem->bPMBReqValid) &&
                   (pstPMP2PReqItem->stPMBReq.ucBuff[TIMEOUT_INDEX] != (uint8_t)(RESP_TIMEOUT)))
                {
                    bAllRespTimeout = false;
                }
                else
                {
                    if((pstPMP2PReqItem->bPMCReqValid) &&
                       (pstPMP2PReqItem->stPMCReq.ucBuff[TIMEOUT_INDEX] != (uint8_t)(RESP_TIMEOUT)))
                    {
                        bAllRespTimeout = false;
                    }
                }
            }
        }

        if( !bAllRespTimeout )
        {
            /* PMA */
            if((pstPMP2PReqItem->bPMAReqValid) &&
               ((uint8_t)(RESP_TIMEOUT) == pstPMP2PReqItem->stPMAReq.ucBuff[TIMEOUT_INDEX]))  /* Timeout */
            {
                pstPMP2PReqItem->bPMAReqValid = false;
                DEBUG_P2P( "E-PMA TO\n" );
            }

            /* PMB */
            if((pstPMP2PReqItem->bPMBReqValid) &&
               ((uint8_t)(RESP_TIMEOUT) == pstPMP2PReqItem->stPMBReq.ucBuff[TIMEOUT_INDEX]))  /* Timeout */
            {
                pstPMP2PReqItem->bPMBReqValid = false;
                DEBUG_P2P( "E-PMB TO\n" );
            }

            /* PMC */
            if((pstPMP2PReqItem->bPMCReqValid) &&
               ((uint8_t)(RESP_TIMEOUT) == pstPMP2PReqItem->stPMCReq.ucBuff[TIMEOUT_INDEX]))  /* Timeout */
            {
                pstPMP2PReqItem->bPMCReqValid = false;
                DEBUG_P2P( "E-PMC TO\n" );
            }
        }

        /* Update Return Value */
        if(( pstPMP2PReqItem->bPMAReqValid ) ||
           ( pstPMP2PReqItem->bPMBReqValid ) ||
           ( pstPMP2PReqItem->bPMCReqValid ))
        {
            bExist = true;
        }
    }

    return bExist;
}

/*
***********************************************************************************************************************
* 函数名称: CheckPMReqData
*
* 功能描述: 检查本站PM发送的请求帧是否有效
*
* 输入参数: pucReq：指向PM发送的请求帧；
*          usLen：请求帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 请求帧是否有效：true，有效；false，无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckPMReqData( uint8_t const pucReq[], uint16_t usLen )
{
    bool_t bValid = false;
    uint8_t ucSrcNetID = 0U;  /* Source Network ID */
    uint8_t ucDstStaID = 0U;  /* Destination Station ID */
    uint32_t uiCRCIndex = 0U;
    uint32_t uiCRC = 0U;
    uint32_t uiCalcCRC = 0U;
    PMP2PReqAddInfo_t const *pstReqAddl = NULL;

    /* Check */
    if((pucReq != NULL) &&
       (usLen > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN)))
    {
        bValid = true;
        pstReqAddl = (PMP2PReqAddInfo_t const *)&pucReq[0];

        /* Length */
        if( pstReqAddl->uiTotalLen != (uint32_t)usLen )
        {
            bValid = false;
            DEBUG_P2P( "PMP2PReq Len:%d != %d\n", pstReqAddl->uiTotalLen, usLen );
        }

        if( bValid )
        {
            /* FCS */
            uiCRCIndex = (uint32_t)(usLen - (uint16_t)(PM_P2P_MSG_FCS_LEN));
            uiCalcCRC = SysCrc32Cal( 0U, pucReq, uiCRCIndex );
            memcpy( &uiCRC, &pucReq[uiCRCIndex], (uint32_t)(PM_P2P_MSG_FCS_LEN));
            if( uiCRC != uiCalcCRC )
            {
                bValid = false;
                DEBUG_P2P( "PMP2PReq CRC: 0x%08X != 0x%08X\n", uiCRC, uiCalcCRC );
            }
        }

        if( bValid )
        {
            ucSrcNetID = pstReqAddl->ucSrcNETID;
            ucDstStaID = pstReqAddl->ucDstStaID;

            if((pstReqAddl->ucSrcCMSlotID != s_ucLocalCMSlotID) ||  /* Source CMID */
               ((ucSrcNetID < (uint8_t)(SRC_NET1)) || (ucSrcNetID > (uint8_t)(SRC_NETALL))) ||  /* Source Network ID */
               (0U == pstReqAddl->uiDstIP1) ||                /* Destination IP Address */
               (pstReqAddl->ucSrcStaID != s_ucLocalStaID) ||  /* Source Station ID */
               ((ucDstStaID < (uint8_t)(MIN_STA_ID)) || (ucDstStaID > (uint8_t)(MAX_STA_ID))) || /* Destination Station ID */
               (pstReqAddl->ucFraType != PMP2P_REQ))  /* Frame Type */
            {
                bValid = false;
                DEBUG_P2P( "PMP2PReq Other Info Err: Local CMSlotID-%d, StaID-%d\n", \
                           s_ucLocalCMSlotID, s_ucLocalStaID );
            }
            else
            {
                if(((uint8_t)(SRC_NETALL) == ucSrcNetID) &&
                   (0U == pstReqAddl->uiDstIP2))
                {
                    bValid = false;
                    DEBUG_P2P( "PMP2PReq: Redundant, uiDstIP2 is 0\n" );
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckPMRespData
*
* 功能描述: 检查本站PM发送的应答帧是否有效
*
* 输入参数: pucResp：指向PM发送的应答帧；
*          usLen：应答帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 应答帧是否有效：true，有效；false，无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t CheckPMRespData( uint8_t const pucResp[], uint16_t usLen, uint8_t ucExtReqStaID )
{
    bool_t bValid = false;
    uint8_t ucSrcNetID = 0U;  /* Source Network ID */
    uint8_t ucDstStaID = 0U;  /* Destination Station ID */
    uint32_t uiCRCIndex = 0U;
    uint32_t uiCRC = 0U;
    uint32_t uiCalcCRC = 0U;
    PMP2PRespAddInfo_t const *pstRespAddl = NULL;

#ifdef DEBUG_P2PCOMM
    uint16_t usIndex = 0U;
#endif

    if((pucResp != NULL) &&
       (usLen > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN)) &&
       ((ucExtReqStaID >= (uint8_t)(MIN_STA_ID)) && (ucExtReqStaID <= (uint8_t)(MAX_STA_ID))))
    {
        bValid = true;
        pstRespAddl = (PMP2PRespAddInfo_t const *)&pucResp[0];

#ifdef DEBUG_P2PCOMM
        DEBUG_P2P( "PMP2PResp: Len-%d, Data(0x):\n", usLen );
        for( usIndex = 0U; usIndex < usLen; usIndex++ )
        {
            printf( "%02X ", pucResp[usIndex]);
        }

        printf( "\n" );
#endif

        /* Length */
        if( pstRespAddl->uiTotalLen != (uint32_t)usLen )
        {
            bValid = false;
            DEBUG_P2P( "PMP2PResp Len: %d != %d\n", pstRespAddl->uiTotalLen, usLen );
        }

        if( bValid )
        {
            /* FCS */
            uiCRCIndex = (uint32_t)(usLen - (uint16_t)(PM_P2P_MSG_FCS_LEN));
            uiCalcCRC = SysCrc32Cal( 0U, pucResp, uiCRCIndex );
            memcpy( &uiCRC, &pucResp[uiCRCIndex], (uint32_t)(PM_P2P_MSG_FCS_LEN));
            if( uiCRC != uiCalcCRC )
            {
                bValid = false;
                DEBUG_P2P( "PMP2PResp CRC: 0x%08X != 0x%08X\n", uiCRC, uiCalcCRC );
            }
        }

        if( bValid )
        {
            ucSrcNetID = pstRespAddl->ucSrcNETID;
            ucDstStaID = pstRespAddl->ucDstStaID;

            if((pstRespAddl->ucSrcCMSlotID != s_ucLocalCMSlotID) ||  /* Source CMID */
               ((ucSrcNetID < (uint8_t)(SRC_NET1)) || (ucSrcNetID >= (uint8_t)(SRC_NETALL))) ||  /* Source Network ID */
               (0U == pstRespAddl->uiDstIP1) ||                /* Destination IP Address */
               (pstRespAddl->ucSrcStaID != s_ucLocalStaID) ||  /* Source Station ID */
               ((ucDstStaID < (uint8_t)(MIN_STA_ID)) || (ucDstStaID > (uint8_t)(MAX_STA_ID))) || /* Destination Station ID */
               (pstRespAddl->ucFraType != PMP2P_RESP))  /* Frame Type */
            {
                bValid = false;
                DEBUG_P2P( "PMP2PResp Other Info Err: Local CMSlotID-%d, StaID-%d\n", \
                           s_ucLocalCMSlotID, s_ucLocalStaID );
            }
        }

        if( bValid )
        {
            if( ucExtReqStaID != ucDstStaID )
            {
                /* Invalid Response: Not response message of the request message */
                bValid = false;
                DEBUG_P2P( "Discard: PMResp DestStaID:%d != ExtReqStaID:%d\n", ucDstStaID, ucExtReqStaID );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckStaReqData
*
* 功能描述: 检查来自其他控制站的请求帧是否有效
*
* 输入参数：pucReq: 其他控制站发送的请求帧；
*          usLen：请求帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 是否有效，true-有效；false-无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckStaReqData( uint8_t const pucReq[], uint16_t usLen )
{
    bool_t bValid = false;
    uint8_t ucSrcStaID = 0U;  /* Source Station ID */
    CMP2PReqAddInfo_t const *pstCMP2PReq = NULL;

#ifdef DEBUG_P2PCOMM
    uint16_t usIndex = 0U;
#endif

    /* Check */
    if((pucReq != NULL) &&
       (usLen > (uint16_t)((P2P_MSG_ADDL_LEN) + (P2P_MSG_FCS_LEN))))
    {
        bValid = true;
        pstCMP2PReq = (CMP2PReqAddInfo_t const *)&pucReq[0];

        /* Source Station ID */
        ucSrcStaID = pstCMP2PReq->ucSrcStaID;

#ifdef DEBUG_P2PCOMM
        DEBUG_P2P( "ExtP2PReq: SrcStaID-%d, Len-%d, Data(0x):\n", ucSrcStaID, usLen );
        for( usIndex = 0U; usIndex < usLen; usIndex++ )
        {
            printf( "%02X ", pucReq[usIndex]);
        }

        printf( "\n" );
#endif

        /* Check */
        if(((ucSrcStaID < (uint8_t)(MIN_STA_ID)) || (ucSrcStaID > (uint8_t)(MAX_STA_ID))) ||
           (pstCMP2PReq->ucDstStaID != s_ucLocalStaID) ||  /* Destination Station ID */
           (pstCMP2PReq->ucFraType != PMP2P_REQ) ||        /* Frame Type */
           (pstCMP2PReq->usTotalLen != usLen))             /* Total Length */
        {
            bValid = false;
            DEBUG_P2P( "ExtP2PReq Error: SrcStaID-%d, DstStaID-%d, LocalStaID-%d\n", \
                       ucSrcStaID, pstCMP2PReq->ucDstStaID, s_ucLocalStaID );
        }

        if( bValid )
        {
            s_ucExtReqStaID = ucSrcStaID;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckStaRespData
*
* 功能描述: 检查来自其他控制站的应答帧是否有效
*
* 输入参数：pucResp: 其他控制站发送的应答帧；
*          usLen：应答帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 是否有效，true-有效；false-无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckStaRespData( uint8_t const pucResp[], uint16_t usLen )
{
    bool_t bValid = false;
    uint8_t ucSrcStaID = 0U;  /* Source Station ID */
    CMP2PRespAddInfo_t const *pstCMResp = NULL;

#ifdef DEBUG_P2PCOMM
    uint16_t usIndex = 0U;
#endif

    if((pucResp != NULL) &&
       (usLen > (uint16_t)((P2P_MSG_ADDL_LEN) + (P2P_MSG_FCS_LEN))))
    {
        bValid = true;
        pstCMResp = (CMP2PRespAddInfo_t const *)&pucResp[0];

        /* Source Station ID */
        ucSrcStaID = pstCMResp->ucSrcStaID;

#ifdef DEBUG_P2PCOMM
        DEBUG_P2P( "ExtP2PResp: SrcStaID-%d, Len-%d, Data(0x):\n", ucSrcStaID, usLen );
        for( usIndex = 0U; usIndex < usLen; usIndex++ )
        {
            printf( "%02X ", pucResp[usIndex]);
        }

        printf( "\n" );
#endif

        /* Check */
        if(((ucSrcStaID < (uint8_t)(MIN_STA_ID)) || (ucSrcStaID > (uint8_t)(MAX_STA_ID))) ||
           (pstCMResp->ucDstStaID != s_ucLocalStaID) ||  /* Destination Station ID */
           (pstCMResp->ucFraType != PMP2P_RESP) ||       /* Frame Type */
           (pstCMResp->usTotalLen != usLen))             /* Total Length */
        {
            bValid = false;
            DEBUG_P2P( "ExtP2PResp Error: SrcStaID-%d, DstStaID-%d, LocalStaID-%d\n", \
                       ucSrcStaID, pstCMResp->ucDstStaID, s_ucLocalStaID );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: MakeTotalReq
*
* 功能描述: 将本站PM发送的请求帧打包
*
* 输入参数: pstPMP2PReqItem：指向保存本站所有PM发送的请求帧的结构体；
*          pucBuff：用于保存打包后的请求帧；
*          pusLen：数据缓冲区长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void MakeTotalReq( PMP2PReqItemInfo_t const *pstPMP2PReqItem, uint8_t pucBuff[], puint16_t pusLen )
{
    uint16_t usTotalLen = 0U;      /* Total Request Message Length */
    uint16_t usIndex = 0U;
    uint8_t ucPMInfo = 0U;
    uint8_t const *pucReq = NULL;
    uint32_t uiCRC = 0U;
    uint16_t usSafetyMsgLen = 0U;  /* Safety Message Length */
    CMP2PReqAddInfo_t *pstCMP2PReq = NULL;

    if((pstPMP2PReqItem != NULL) &&
       (pucBuff != NULL) &&
       (pusLen != NULL))
    {
        /* Calculate Total Request Message Length */
        usTotalLen = (uint16_t)((P2P_MSG_ADDL_LEN) + (P2P_MSG_FCS_LEN));

        /* PMA Safety Frame */
        if( pstPMP2PReqItem->bPMAReqValid )
        {
            if( pstPMP2PReqItem->stPMAReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
            {
                usSafetyMsgLen = pstPMP2PReqItem->stPMAReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                usTotalLen += usSafetyMsgLen;
                ucPMInfo |= (uint8_t)(PMA_REQ_BIT);
                pucReq = pstPMP2PReqItem->stPMAReq.ucBuff;
            }
        }

        /* PMB Safety Frame */
        if( pstPMP2PReqItem->bPMBReqValid )
        {
            if( pstPMP2PReqItem->stPMBReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
            {
                usSafetyMsgLen = pstPMP2PReqItem->stPMBReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                usTotalLen += usSafetyMsgLen;
                ucPMInfo |= (uint8_t)(PMB_REQ_BIT);
                if( NULL == pucReq )
                {
                    pucReq = pstPMP2PReqItem->stPMBReq.ucBuff;
                }
            }
        }

        /* PMC Safety Frame */
        if( pstPMP2PReqItem->bPMCReqValid )
        {
            if( pstPMP2PReqItem->stPMCReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
            {
                usSafetyMsgLen = pstPMP2PReqItem->stPMCReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                usTotalLen += usSafetyMsgLen;
                ucPMInfo |= (uint8_t)(PMC_REQ_BIT);
                if( NULL == pucReq )
                {
                    pucReq = pstPMP2PReqItem->stPMCReq.ucBuff;
                }
            }
        }

        if((*pusLen >= usTotalLen) &&
           (pucReq != NULL))
        {
            /* Has Enough Space */
            /* Additional Info. */
            pstCMP2PReq = (CMP2PReqAddInfo_t*)&pucBuff[0];
            memcpy( pstCMP2PReq, &pucReq[12U], 4U );
            pstCMP2PReq->usTotalLen = usTotalLen;  /* Total Length */
            pstCMP2PReq->ucPMInfo = ucPMInfo;      /* PM Info. */
            usIndex = sizeof(CMP2PReqAddInfo_t);

            /* Safety Message */
            if( pstPMP2PReqItem->bPMAReqValid )
            {
                if( pstPMP2PReqItem->stPMAReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
                {
                    usSafetyMsgLen = pstPMP2PReqItem->stPMAReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                    memcpy( &pucBuff[usIndex], &( pstPMP2PReqItem->stPMAReq.ucBuff[PM_P2P_MSG_ADDL_LEN]), usSafetyMsgLen );
                    usIndex += usSafetyMsgLen;
                }
            }

            if( pstPMP2PReqItem->bPMBReqValid )
            {
                if( pstPMP2PReqItem->stPMBReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
                {
                    usSafetyMsgLen = pstPMP2PReqItem->stPMBReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                    memcpy( &pucBuff[usIndex], &( pstPMP2PReqItem->stPMBReq.ucBuff[PM_P2P_MSG_ADDL_LEN]), usSafetyMsgLen );
                    usIndex += usSafetyMsgLen;
                }
            }

            if( pstPMP2PReqItem->bPMCReqValid )
            {
                if( pstPMP2PReqItem->stPMCReq.usCount > (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN))
                {
                    usSafetyMsgLen = pstPMP2PReqItem->stPMCReq.usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);
                    memcpy( &pucBuff[usIndex], &( pstPMP2PReqItem->stPMCReq.ucBuff[PM_P2P_MSG_ADDL_LEN]), usSafetyMsgLen );
                    usIndex += usSafetyMsgLen;
                }
            }

            /* FCS */
            uiCRC = SysCrc32Cal( 0U, pucBuff, usIndex );
            memcpy( &pucBuff[usIndex], &uiCRC, sizeof(uint32_t));

            /* Update Length */
            *pusLen = usTotalLen;
        }
        else
        {
            *pusLen = 0U;
            DEBUG_P2P( "MakeTotalReq: BuffLen %d < MsgLen %d\n", *pusLen, usTotalLen );
        }
    }
    else
    {
        if( pusLen != NULL )
        {
            *pusLen = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakeRespMsg
*
* 功能描述: 将本站PM发送的应答帧打包
*
* 输入参数: pstPMP2PRespInfo：指向本站PM发送的应答帧结构体；
*          pucBuff：用于保存打包后的应答帧；
*          pusLen：数据缓冲区长度。
*
* 输出参数: pusLen：打包后的应答帧长度，0代表打包错误
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void MakeRespMsg( PMP2PRespMsgInfo_t const *pstPMP2PRespInfo, uint8_t pucBuff[], puint16_t pusLen )
{
    uint16_t usTotalLen = 0U;      /* Total Response Message Length */
    uint16_t usIndex = 0U;
    uint32_t uiCRC = 0U;
    uint16_t usSafetyMsgLen = 0U;  /* Safety Message Length */
    uint8_t const *pucPMResp = NULL;
    CMP2PRespAddInfo_t *pstCMResp = NULL;

    if((pstPMP2PRespInfo != NULL) &&
       (pucBuff != NULL) &&
       (pusLen != NULL))
    {
        usSafetyMsgLen = pstPMP2PRespInfo->usCount - (uint16_t)(PM_P2P_MSG_NONSAFETY_LEN);

        /* Calculate Total Request Message Length */
        usTotalLen = (uint16_t)(P2P_MSG_ADDL_LEN) + usSafetyMsgLen + (uint16_t)(P2P_MSG_FCS_LEN);
        if( *pusLen >= usTotalLen )
        {
            /* Has Enough Space */
            pstCMResp = (CMP2PRespAddInfo_t*)&pucBuff[0];
            pucPMResp = pstPMP2PRespInfo->ucBuff;

            /* Additional Info. */
            memset( pstCMResp, 0, sizeof(CMP2PRespAddInfo_t));
            memcpy( pstCMResp, &pucPMResp[12], 4U );
            pstCMResp->usTotalLen = usTotalLen;  /* Total Length */
            usIndex = (uint16_t)sizeof(CMP2PRespAddInfo_t);

            /* Safety Message */
            memcpy( &pucBuff[usIndex], &pucPMResp[PM_P2P_MSG_ADDL_LEN], usSafetyMsgLen );
            usIndex += usSafetyMsgLen;

            /* FCS */
            uiCRC = SysCrc32Cal( 0U, pucBuff, usIndex );
            memcpy( &pucBuff[usIndex], &uiCRC, sizeof(uint32_t));
            usIndex += sizeof(uint32_t);

            /* Update Length */
            *pusLen = usTotalLen;

#ifdef DEBUG_P2PCOMM
            DEBUG_P2P( "MakeRespMsg: Len-%d, Data(0x):\n", usTotalLen );
            for( usIndex = 0U; usIndex < usTotalLen; usIndex++ )
            {
                printf( "%02X ", pucBuff[usIndex]);
            }

            printf( "\n" );
#endif
        }
        else
        {
            *pusLen = 0U;
            DEBUG_P2P( "MakeRespMsg: BuffLen %d < MsgLen %d\n", *pusLen, usTotalLen );
        }
    }
    else
    {
        if( pusLen != NULL )
        {
            *pusLen = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleWait
*
* 功能描述: 处理等待-等待未获取的本站PM的请求帧
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

static void HandleWait( void )
{
    uint16_t usDataLen = 0U;  /* Data Length */
    NetInfo_t stReqNetInfo;   /* Network info. of the request message */
    stReqNetInfo.uiDstIP1Addr = 0U;
    stReqNetInfo.uiDstIP2Addr = 0U;
    stReqNetInfo.emSrcNetwork = SRC_INVALID;
    bool_t bComplete = false;

    if( s_stPMP2PReqItem.bWaitFlag )
    {
        /* Read if not exist */
        /* PMA */
        if( 0U == s_stPMP2PReqItem.stPMAReq.usCount )
        {
            s_stPMP2PReqItem.stPMAReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMAReq.ucBuff );
            if( !SysGetP2PMsg( PMA, P2P_PM_REQ, s_stPMP2PReqItem.stPMAReq.ucBuff, &s_stPMP2PReqItem.stPMAReq.usCount ))
            {
                s_stPMP2PReqItem.stPMAReq.usCount = 0U;
            }
        }

        /* PMB */
        if( 0U == s_stPMP2PReqItem.stPMBReq.usCount )
        {
            s_stPMP2PReqItem.stPMBReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMBReq.ucBuff );
            if( !SysGetP2PMsg( PMB, P2P_PM_REQ, s_stPMP2PReqItem.stPMBReq.ucBuff, &s_stPMP2PReqItem.stPMBReq.usCount ))
            {
                s_stPMP2PReqItem.stPMBReq.usCount = 0U;
            }
        }

        /* PMC */
        if( 0U == s_stPMP2PReqItem.stPMCReq.usCount )
        {
            s_stPMP2PReqItem.stPMCReq.usCount = (uint16_t)sizeof( s_stPMP2PReqItem.stPMCReq.ucBuff );
            if( !SysGetP2PMsg( PMC, P2P_PM_REQ, s_stPMP2PReqItem.stPMCReq.ucBuff, &s_stPMP2PReqItem.stPMCReq.usCount ))
            {
                s_stPMP2PReqItem.stPMCReq.usCount = 0U;
            }
        }

        /* Check if complete */
        bComplete = true;
        if((0U == s_stPMP2PReqItem.stPMAReq.usCount) &&
           ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMA )))
        {
            bComplete = false;
        }
        else
        {
            if((0U == s_stPMP2PReqItem.stPMBReq.usCount) &&
               ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMB )))
            {
                bComplete = false;
            }
            else
            {
                if((0U == s_stPMP2PReqItem.stPMCReq.usCount) &&
                   ((uint16_t)(INTER_COMM_NORMAL) == SysGetLocalInterCommState( PMC )))
                {
                    bComplete = false;
                }
            }
        }

        if( !bComplete )
        {
            /* Check if wait timeout */
            if( s_stPMP2PReqItem.ucWaitCnt > (uint8_t)(MAX_WAIT_PMREQ_CYCLE_CNT))
            {
                DEBUG_P2P( "HandleWait: Timeout\n" );
                s_stPMP2PReqItem.ucWaitCnt = 0U;
                s_stPMP2PReqItem.bWaitFlag = false;
            }
            else
            {
                s_stPMP2PReqItem.ucWaitCnt++;
            }
        }
        else
        {
            /* Complete: do not need to wait */
            DEBUG_P2P( "HandleWait: Complete, wait count %d\n", s_stPMP2PReqItem.ucWaitCnt );
            s_stPMP2PReqItem.ucWaitCnt = 0U;
            s_stPMP2PReqItem.bWaitFlag = false;
        }

        if( !s_stPMP2PReqItem.bWaitFlag )
        {
            if( CheckLocalPMP2PReq( &s_stPMP2PReqItem, bComplete ))
            {
                usDataLen = (uint16_t)sizeof( s_ucP2PDataBuff );
                MakeTotalReq( &s_stPMP2PReqItem, s_ucP2PDataBuff, &usDataLen );
                if( usDataLen > 0U )
                {
                    /* Send To Dst Station */
                    GetNetInfo( &s_stPMP2PReqItem, &stReqNetInfo );
                    P2PSendReq( &stReqNetInfo, s_ucP2PDataBuff, usDataLen );
                }
            }
            else
            {
                DEBUG_P2P( "HandleWait: No valid req msg\n" );
                LogWrite( (uint32_t)(LOGID_P2P_E_MSG_PM_REQ));
            }

            /* Clear Item Info */
            ClearPMP2PReqItemInfo();
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetNetInfo
*
* 功能描述: 获取请求帧中的网络信息 - 目的IP地址和源端口号
*
* 输入参数: pstPMP2PReqItem：指向保存本站所有PM发送的请求帧的结构体;
*          pstReqNetInfo：用于保存获取结果。
*
* 输出参数: pstReqNetInfo：用于保存获取结果
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void GetNetInfo( PMP2PReqItemInfo_t const *pstPMP2PReqItem, NetInfo_t *pstReqNetInfo )
{
    uint8_t const *pucReq = NULL;
    PMP2PReqAddInfo_t const *pstPMReq = NULL;

    if((pstPMP2PReqItem != NULL) &&
       (pstReqNetInfo != NULL ))
    {
        if( pstPMP2PReqItem->bPMAReqValid )
        {
            pucReq = pstPMP2PReqItem->stPMAReq.ucBuff;
        }
        else if( pstPMP2PReqItem->bPMBReqValid )
        {
            pucReq = pstPMP2PReqItem->stPMBReq.ucBuff;
        }
        else if( pstPMP2PReqItem->bPMCReqValid )
        {
            pucReq = pstPMP2PReqItem->stPMCReq.ucBuff;
        }
        else
        {
            pucReq = NULL;
        }

        if( pucReq != NULL )
        {
            pstPMReq = (PMP2PReqAddInfo_t const *)&pucReq[0];
            pstReqNetInfo->ucL1NetID = pstPMReq->ucL1NetID;
            if((pstPMReq->ucSrcNETID >= (uint8_t)SRC_NET1) && (pstPMReq->ucSrcNETID <= (uint8_t)SRC_NETALL))
            {
                pstReqNetInfo->emSrcNetwork = (SrcNet_t)pstPMReq->ucSrcNETID;
                pstReqNetInfo->uiDstIP1Addr = pstPMReq->uiDstIP1;
                pstReqNetInfo->uiDstIP2Addr = pstPMReq->uiDstIP2;
            }
            else
            {
                pstReqNetInfo->emSrcNetwork = SRC_INVALID;
                pstReqNetInfo->uiDstIP1Addr = 0U;
                pstReqNetInfo->uiDstIP2Addr = 0U;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: P2PSendReq
*
* 功能描述: 发送数据
*
* 输入参数: pstNetInfo：源网口和目的IP地址；
*          pucBuff：待发送数据缓冲区；
*          usLen：待发送数据的字节数。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void P2PSendReq( NetInfo_t const *pstNetInfo, uint8_t pucBuff[], uint16_t usLen )
{
    uint8_t ucResult = (uint8_t)(EPARA);
    uint32_t uiIndex = 0U;
    uint32_t uiCRC = 0U;
    CMP2PReqAddInfo_t *pstCMP2PReq = NULL;

#ifdef DEBUG_P2PCOMM
    uint16_t usIndex = 0U;
#endif

    if((pstNetInfo != NULL) &&
       (pucBuff != NULL) &&
       (usLen > 0U))
    {
        /* Get IP and Network */
        if((pstNetInfo->emSrcNetwork >= SRC_NET1) && (pstNetInfo->emSrcNetwork <= SRC_NETALL))
        {
            if( SRC_NET1 == pstNetInfo->emSrcNetwork )
            {
#ifdef DEBUG_P2PCOMM
                DEBUG_P2P( "Net1 P2PTotalReq: Len-%d, Data(0x):\n", usLen );
                for( usIndex = 0U; usIndex < usLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                }

                printf( "\n" );
#endif

                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP1Addr, \
                                 (uint16_t)(PORTNUM_P2P), NET1, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendReq Failed: Net 1, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_REQ));
                }
            }
            else if( SRC_NET2 == pstNetInfo->emSrcNetwork )
            {
#ifdef DEBUG_P2PCOMM
                DEBUG_P2P( "Net2 P2PTotalReq: Len-%d, Data(0x):\n", usLen );
                for( usIndex = 0U; usIndex < usLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                }

                printf( "\n" );
#endif

                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP1Addr, \
                                 (uint16_t)(PORTNUM_P2P), NET2, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendReq Failed: Net 2, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_REQ));
                }
            }
            else
            {
                /* Same CM Two Network Redundant */
                pstCMP2PReq = (CMP2PReqAddInfo_t*)&pucBuff[0];
                uiIndex = (uint32_t)pstCMP2PReq->usTotalLen - sizeof(uint32_t);

                /* Network 1 */
                /* Set LinkID */
                if( NET1 == pstNetInfo->ucL1NetID )
                {
                    pstCMP2PReq->ucLinkID = 1;
                }
                else
                {
                    pstCMP2PReq->ucLinkID = 2;
                }

                /* Re-Calc CRC */
                uiCRC = SysCrc32Cal( 0U, pucBuff, uiIndex );
                memcpy( &pucBuff[uiIndex], &uiCRC, sizeof(uint32_t));

#ifdef DEBUG_P2PCOMM
                DEBUG_P2P( "Net1 P2PTotalReq: Len-%d, Data(0x):\n", usLen );
                for( usIndex = 0U; usIndex < usLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                }

                printf( "\n" );
#endif

                /* Send */
                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP1Addr, \
                                 (uint16_t)(PORTNUM_P2P), NET1, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendReq Failed: Net 1, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_REQ));
                }

                /* Network 2 */
                /* Set LinkID */
                if( NET2 == pstNetInfo->ucL1NetID )
                {
                    pstCMP2PReq->ucLinkID = 1;
                }
                else
                {
                    pstCMP2PReq->ucLinkID = 2;
                }

                /* Re-Calc CRC */
                uiCRC = SysCrc32Cal( 0U, pucBuff, uiIndex );
                memcpy( &pucBuff[uiIndex], &uiCRC, sizeof(uint32_t));

#ifdef DEBUG_P2PCOMM
                DEBUG_P2P( "Net2 P2PTotalReq: Len-%d, Data(0x):\n", usLen );
                for( usIndex = 0U; usIndex < usLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                }

                printf( "\n" );
#endif

                /* Send */
                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP2Addr,
                                 (uint16_t)(PORTNUM_P2P), NET2, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendReq Failed: Net 2, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_REQ));
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: P2PSendResp
*
* 功能描述: 发送应答数据： 同CM双网口冗余时，不同时发送应答，从哪条链路来就从哪条链路回
*
* 输入参数: pstNetInfo：源网口和目的IP地址；
*          pucBuff：待发送数据缓冲区；
*          usLen：待发送数据的字节数。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 同CM双网口冗余时，不同时发送应答，从哪条链路来就从哪条链路回
***********************************************************************************************************************
*/

static void P2PSendResp( NetInfo_t const *pstNetInfo, uint8_t const pucBuff[], uint16_t usLen )
{
    uint8_t ucResult = (uint8_t)(EPARA);

    if((pstNetInfo != NULL) &&
       (pucBuff != NULL) &&
       (usLen > 0U))
    {
        /* Get IP and Network */
        if((pstNetInfo->emSrcNetwork >= SRC_NET1) && (pstNetInfo->emSrcNetwork < SRC_NETALL))
        {
            if( SRC_NET1 == pstNetInfo->emSrcNetwork )
            {
                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP1Addr, \
                                 (uint16_t)(PORTNUM_P2P), NET1, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendResp Failed: Net 1, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_RESP));
                }
            }
            else
            {
                ucResult = Send( pucBuff, usLen, pstNetInfo->uiDstIP1Addr, \
                                 (uint16_t)(PORTNUM_P2P), NET2, TYPE_P2P, 0U );
                if( ucResult != (uint8_t)(SEND_SUCCESS))
                {
                    DEBUG_P2P( "P2PSendResp Failed: Net 2, errno-%d\n", ucResult );
                    LogWrite( (uint32_t)(LOGID_P2P_E_SEND_PM_RESP));
                }
            }
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
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 选取的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static SelPMID_t SelectPM( void )
{
    SelPMID_t emSelPMID = SELPM_NONE;

    /* Select and update */
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

    return emSelPMID;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

