/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 站间安全通讯
*
*  文件名称    : P2PSafetyComm.h
*
*  功能简介    : 站间安全通讯相关处理
*
*  作者        : 王东
*
*  创建日期    : 2016-04-18
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
#include "../inc/P2PSafetyComm.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static uint8_t s_ucTolCnt;                                     /* 用户配置的可容忍次数 */
static uint8_t s_ucTolThr;                                     /* 容忍门限值 */
static P2PRecvReqCommInfo_t s_stP2PRecvInfo[MAX_STATION_CNT];  /* 本站作为接收站时，维护的通信信息结构体数组，
                                                                                                                                              每个发送站对应其中一个数组成员 */
static P2PSendReqCommInfo_t s_stP2PSendInfo[MAX_STATION_CNT];  /* 本站作为发送站时，维护的通信信息结构体数组，
                                                                                                                                              每个接收站对应其中一个数组成员 */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void ClearP2PRecvInfo( P2PRecvReqCommInfo_t *pstStaInfo );
static void ClearP2PSendInfo( P2PSendReqCommInfo_t *pstStaInfo );
static void ClearReqDecodeResult( ReqDecodeResultInfo_t *pstResult );
static void ClearRespDecodeResult( RespDecodeResultInfo_t *pstResult );
static void DecodePMResp( uint8_t ucRecvStaID, bool_t bRedundant, P2PSendReqCommInfo_t *pstStaInfo,
                          uint8_t pucSFra[], uint32_t uiSFraLen, RespDecodeResultInfo_t *pstResult );
static void HandleLocalSendSQ( StaRespHeadInfo_t const *pstHead, P2PSendReqCommInfo_t *pstStaInfo );
static void DecodePMReq( uint8_t ucSendStaID, uint16_t usRecvSeq, uint8_t pucSFra[], uint16_t usSFraLen,
                         StaSafetyDataInfo_t *pstPMDataInfo );
static uint16_t MakePMADataStatus( StaSafetyDataInfo_t const *pstPMDataInfo );
static uint16_t MakePMBDataStatus( StaSafetyDataInfo_t const *pstPMDataInfo );
static uint16_t MakePMCDataStatus( StaSafetyDataInfo_t const *pstPMDataInfo );
static void HandleReqValid( ReqDecodeInfo_t const *pstResult, P2PRecvReqCommInfo_t *pstStaInfo );
static void HandleReqFailure( ReqDecodeInfo_t const *pstResult, P2PRecvReqCommInfo_t *pstStaInfo );
static void HandleReqError( bool_t bRedundant, ReqDecodeInfo_t *pstResult,
                            P2PRecvReqCommInfo_t *pstStaInfo, uint8_t ucTolThr,
                            bool_t bPMAFraExist, bool_t bPMBFraExist, bool_t bPMCFraExist );
static void HandleReqErrorThree( bool_t bRedundant, ReqDecodeInfo_t *pstResult,
                                 P2PRecvReqCommInfo_t *pstStaInfo, uint8_t ucTolThr );
static void HandleReqErrorTwo( bool_t bRedundant, ReqDecodeInfo_t *pstResult, P2PRecvReqCommInfo_t *pstStaInfo,
                               uint8_t ucTolThr, StaSafetyDataInfo_t *pstPM1DataInfo, StaSafetyDataInfo_t *pstPM2DataInfo );
static void HandleReqErrorOne( bool_t bRedundant, ReqDecodeInfo_t *pstResult, P2PRecvReqCommInfo_t *pstStaInfo,
                               uint8_t ucTolThr, StaSafetyDataInfo_t *pstPMDataInfo );
static void MakeDecodeResult( ReqDecodeInfo_t const *pstDecodeInfo, ReqDecodeResultInfo_t *pstResult );



/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: P2PCommInit
*
* 功能描述: 站间安全通讯初始化函数
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

void P2PCommInit( void )
{
    uint8_t ucIndex = 0U;

    /* Update TolCnt & TolThr */
    UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_STATION_CNT); ucIndex++ )
    {
        /* P2P Receive Request Communication Info. */
        ClearP2PRecvInfo( &s_stP2PRecvInfo[ucIndex]);

        /* P2P Send Request Communication Info. */
        ClearP2PSendInfo( &s_stP2PSendInfo[ucIndex]);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: P2PHandleRecvReqTO
*
* 功能描述: 接收站：接收请求帧超时错误处理
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleRecvReqTO( uint8_t ucSendStaID )
{
    bool_t bResult = false;
    uint8_t ucSendStaIndex = 0U;
    P2PRecvReqCommInfo_t *pstInfo = NULL;

    if((ucSendStaID >= (uint8_t)(MIN_STA_ID)) &&
       (ucSendStaID <= (uint8_t)(MAX_STA_ID)))
    {
        bResult = true;
        ucSendStaIndex = ucSendStaID - 1U;
        pstInfo = &s_stP2PRecvInfo[ucSendStaIndex];
        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

        /* Update Status Info. */
        pstInfo->usStatusInfo = 0U;                     /* Clear */
        pstInfo->usStatusInfo |= (uint16_t)(BIT_WDTO);  /* Set Bit0: WatchDog timeout */

        /* Common Error Handle */
        HandleSafetyCommError( &( pstInfo->stErrInfo ), s_ucTolThr );

        /* System Resource */
        SysSetP2PRecvTolFlag( ucSendStaIndex, pstInfo->stErrInfo.bTolFlag );
        SysSetP2PRecvErrFlag( ucSendStaIndex, pstInfo->stErrInfo.bErrFlag );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PDecodeReq
*
* 功能描述: 解析来自指定发送控制站的请求帧：Additional Info. + Safety Frame + FCS
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64；
*          bRedundant：冗余链路标志位；
*          pucReq：请求帧首地址；
*          usReqLen：请求帧长度。
*
* 输出参数: pstResult：用于返回请求帧解析结果
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PDecodeReq( uint8_t ucSendStaID, bool_t bRedundant, uint8_t pucReq[], uint16_t usReqLen,
                     ReqDecodeResultInfo_t *pstResult )
{
    bool_t bResult = false;
    uint8_t ucSendStaIndex = 0U;
    uint32_t uiLen = 0U;           /* Length */
    uint32_t uiTotalSLen = 0U;     /* Total Safety Length */
    uint32_t uiSFraLen = 0U;       /* Safety Frame Length */
    uint32_t uiSFraHeadLen = 0U;   /* Safety Frame Head Length */
    uint16_t usStatus = 0U;        /* Status */
    bool_t bPMAExist = false;      /* PMA Safety Frame Exists Flag */
    uint16_t usPMASFraIndex = 0U;  /* PMA Safety Frame Index */
    uint16_t usPMASFraLen = 0U;    /* PMA Safety Frame Length */
    bool_t bPMBExist = false;      /* PMB Safety Frame Exists Flag */
    uint16_t usPMBSFraIndex = 0U;  /* PMB Safety Frame Index */
    uint16_t usPMBSFraLen = 0U;    /* PMB Safety Frame Length */
    bool_t bPMCExist = false;      /* PMC Safety Frame Exists Flag */
    uint16_t usPMCSFraIndex = 0U;  /* PMC Safety Frame Index */
    uint16_t usPMCSFraLen = 0U;    /* PMC Safety Frame Length */
    uint8_t ucPMReqCnt = 0U;       /* PMP2PReq message counts in the total request message */
    StaReqAddlInfo_t stAddlInfo;
    StaReqHeadInfo_t stHead;
    P2PRecvReqCommInfo_t *pstStaInfo = NULL;
    ReqDecodeInfo_t stDecodeInfo;

    if(((ucSendStaID >= (uint8_t)(MIN_STA_ID)) && (ucSendStaID <= (uint8_t)(MAX_STA_ID))) &&
       ( pucReq != NULL ) &&
       ( pstResult != NULL ) &&
       ( usReqLen > 0U ))
    {
        ucSendStaIndex = ucSendStaID - 1U;
        pstStaInfo = &s_stP2PRecvInfo[ucSendStaIndex];
        ClearReqDecodeResult( pstResult );  /* Reset Decode Result Info. */

        /* Clear Decode Info. */
        memset( &stDecodeInfo, 0, sizeof(ReqDecodeInfo_t));
        stDecodeInfo.bRepeat = false;
        stDecodeInfo.stPMADataInfo.bValid = false;
        stDecodeInfo.stPMBDataInfo.bValid = false;
        stDecodeInfo.stPMCDataInfo.bValid = false;

        /* Additional Info. */
        uiLen = sizeof( StaReqAddlInfo_t );
        if( usReqLen > (uint16_t)uiLen )
        {
            memcpy( &stAddlInfo, pucReq, uiLen );

            /* Length */
            if((usReqLen == stAddlInfo.usTotalLen) &&
               (usReqLen > (uint16_t)(uiLen + (uint32_t)(P2PSAFETY_CRC_LEN))))
            {
                bResult = true;
                UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

                /* Total Safety Length */
                uiTotalSLen = (uint32_t)usReqLen - uiLen - (uint32_t)(P2PSAFETY_CRC_LEN);

                /* Safety Frame */
                uiSFraHeadLen = sizeof( StaReqHeadInfo_t );
                while( uiTotalSLen > uiSFraHeadLen )
                {
                    /* Head */
                    memcpy( &stHead, &pucReq[uiLen], uiSFraHeadLen );

                    /* Safety Frame Length */
                    uiSFraLen = ( uiSFraHeadLen + (uint32_t)stHead.usSDataLen + (uint32_t)(P2PSAFETY_CRC_LEN)) * 2;
                    if( uiTotalSLen >= uiSFraLen )
                    {
                        uiTotalSLen -= uiSFraLen;
                        if((uint8_t)(PMID_A) == (stHead.ucSrcAddr & (uint8_t)(PMID_MASK)))
                        {
                            if( !bPMAExist )
                            {
                                bPMAExist = true;
                                usPMASFraIndex = (uint16_t)uiLen;
                                usPMASFraLen = (uint16_t)uiSFraLen;
                                DecodePMReq( ucSendStaID, pstStaInfo->usRecvSeqNum, &pucReq[usPMASFraIndex], \
                                             usPMASFraLen, &stDecodeInfo.stPMADataInfo );
#if 0
                                DEBUG_P2P( "PMAP2PReq: Valid-%d, RecvSq=%04X, SendSq-%04X, Sts-%02X\n", \
                                           stDecodeInfo.stPMADataInfo.bValid, \
                                           pstStaInfo->usRecvSeqNum, \
                                           stDecodeInfo.stPMADataInfo.usReqSendSeq, \
                                           stDecodeInfo.stPMADataInfo.ucReqStatus );
#endif
                            }
                        }
                        else if((uint8_t)(PMID_B) == (stHead.ucSrcAddr & (uint8_t)(PMID_MASK)))
                        {
                            if( !bPMBExist )
                            {
                                bPMBExist = true;
                                usPMBSFraIndex = (uint16_t)uiLen;
                                usPMBSFraLen = (uint16_t)uiSFraLen;
                                DecodePMReq( ucSendStaID, pstStaInfo->usRecvSeqNum, &pucReq[usPMBSFraIndex], \
                                             usPMBSFraLen, &stDecodeInfo.stPMBDataInfo );
#if 0
                                DEBUG_P2P( "PMBP2PReq: Valid-%d, RecvSq=%04X, SendSq-%04X, Sts-%02X\n", \
                                           stDecodeInfo.stPMBDataInfo.bValid, \
                                           pstStaInfo->usRecvSeqNum, \
                                           stDecodeInfo.stPMBDataInfo.usReqSendSeq, \
                                           stDecodeInfo.stPMBDataInfo.ucReqStatus );
#endif
                            }
                        }
                        else
                        {
                            if( !bPMCExist )
                            {
                                bPMCExist = true;
                                usPMCSFraIndex = (uint16_t)uiLen;
                                usPMCSFraLen = (uint16_t)uiSFraLen;
                                DecodePMReq( ucSendStaID, pstStaInfo->usRecvSeqNum, &pucReq[usPMCSFraIndex], \
                                             usPMCSFraLen, &stDecodeInfo.stPMCDataInfo );
#if 0
                                DEBUG_P2P( "PMCP2PReq: Valid-%d, RecvSq=%04X, SendSq-%04X, Sts-%02X\n", \
                                           stDecodeInfo.stPMCDataInfo.bValid, \
                                           pstStaInfo->usRecvSeqNum, \
                                           stDecodeInfo.stPMCDataInfo.usReqSendSeq, \
                                           stDecodeInfo.stPMCDataInfo.ucReqStatus );
#endif
                            }
                        }

                        uiLen += uiSFraLen;
                    }
                    else
                    {
                        break;  /* Request Not Enough: Finish */
                    }
                }/* end of while */

                /* Make Status Info. */
                usStatus = pstStaInfo->usStatusInfo;
                usStatus &= (uint16_t)(BIT_WDTO);  /* Reset Except WD-Timeout Flag */

                if( bPMAExist )
                {
                    /* Additional Info. - PM Info. */
                    if( 0U == (stAddlInfo.ucPMInfo & (uint8_t)(BIT_ADDL_PMA)))
                    {
                        usStatus |= (uint16_t)(BIT_ALARM);
                    }

                    usStatus |= MakePMADataStatus( &stDecodeInfo.stPMADataInfo );
                    ucPMReqCnt++;
                }

                if( bPMBExist )
                {
                    /* Additional Info. - PM Info. */
                    if( 0U == (stAddlInfo.ucPMInfo & (uint8_t)(BIT_ADDL_PMB)))
                    {
                        usStatus |= (uint16_t)(BIT_ALARM);
                    }

                    usStatus |= MakePMBDataStatus( &stDecodeInfo.stPMBDataInfo );
                    ucPMReqCnt++;
                }

                if( bPMCExist )
                {
                    /* Additional Info. - PM Info. */
                    if( 0U == (stAddlInfo.ucPMInfo & (uint8_t)(BIT_ADDL_PMC)))
                    {
                        usStatus |= (uint16_t)(BIT_ALARM);
                    }

                    usStatus |= MakePMCDataStatus( &stDecodeInfo.stPMCDataInfo );
                    ucPMReqCnt++;
                }

                if( ucPMReqCnt > 1U )
                {
                    usStatus |= (uint16_t)(BIT_MULIPMREQ);
                }

                pstStaInfo->usStatusInfo = usStatus;  /* Set Status Info. */

                /* Handle */
                if( stDecodeInfo.stPMADataInfo.bValid ||
                    stDecodeInfo.stPMBDataInfo.bValid ||
                    stDecodeInfo.stPMCDataInfo.bValid )
                {
                    HandleReqValid( &stDecodeInfo, pstStaInfo );
                    //printf( "VReq: SQ%04X Sts%04X\n", pstStaInfo->usRecvSeqNum, pstStaInfo->usStatusInfo );
                    printf( "PVR\n" );
                }
                else if(((uint8_t)(BIT_PM_FAILURE) == stDecodeInfo.stPMADataInfo.ucReqStatus) ||
                        ((uint8_t)(BIT_PM_FAILURE) == stDecodeInfo.stPMBDataInfo.ucReqStatus) ||
                        ((uint8_t)(BIT_PM_FAILURE) == stDecodeInfo.stPMCDataInfo.ucReqStatus))
                {
                    HandleReqFailure( &stDecodeInfo, pstStaInfo );
                    //printf( "FReq: SQ%04X Sts%04X\n", pstStaInfo->usRecvSeqNum, pstStaInfo->usStatusInfo );
                    printf( "PFR\n" );
                }
                else
                {
                    /* No Correct Safety Frame */
                    HandleReqError( bRedundant, &stDecodeInfo, pstStaInfo, s_ucTolThr, \
                                    bPMAExist, bPMBExist, bPMCExist );
                    //printf( "EReq: SQ%04X Sts%04X\n", pstStaInfo->usRecvSeqNum, pstStaInfo->usStatusInfo );
                    printf( "PER\n" );
                }

                /* System Resource */
                SysSetP2PRecvTolFlag( ucSendStaIndex, pstStaInfo->stErrInfo.bTolFlag );
                SysSetP2PRecvErrFlag( ucSendStaIndex, pstStaInfo->stErrInfo.bErrFlag );

                /* Make Decode Result */
                MakeDecodeResult( &stDecodeInfo, pstResult );
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PMakeResp
*
* 功能描述: 构造将发送给指定发送控制站的应答帧
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64；
*          pucResp：应答帧的首地址。
*
* 输出参数: pusRespLen：用于返回应答帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PMakeResp( uint8_t ucSendStaID, uint8_t pucResp[], uint16_t *pusRespLen )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usRespBuffLen = 0U;
    uint16_t usRespLen = 0U;
    uint16_t usRespHeadLen = 0U;
    uint32_t uiCRC = 0U;
    uint8_t ucLocalStaID = 0U;         /* 本控制站的ID */
    PMController_t emLocalPMID = PMA;  /* 本PM模块的编号 */
    StaRespHeadInfo_t stRespHead;
    P2PRecvReqCommInfo_t *pstStaInfo = NULL;

    if(((ucSendStaID >= (uint8_t)(MIN_STA_ID)) && (ucSendStaID <= (uint8_t)(MAX_STA_ID))) &&
       (pucResp != NULL) &&
       (pusRespLen != NULL))
    {
        usRespBuffLen = *pusRespLen;  /* Store Length */
        *pusRespLen = 0U;             /* Reset Output Length */
        pstStaInfo = &s_stP2PRecvInfo[ucSendStaID - 1U];

        /* Calculate Response Length */
        usRespHeadLen = (uint16_t)sizeof( stRespHead );
        usRespLen = ( usRespHeadLen + (uint16_t)(P2PSAFETY_CRC_LEN)) * 2;

        /* Check Buffer Length */
        if( usRespBuffLen >= usRespLen )
        {
            ucLocalStaID = SharedGetStationID();
            emLocalPMID = SysGetLocalPMID();

            /* Make Response Head */
            stRespHead.usSeq = pstStaInfo->usRecvSeqNum;
            stRespHead.ucSrcAddr = ((emLocalPMID << 6U) | ((ucLocalStaID-1U) & (uint8_t)(STAID_MASK)));
            stRespHead.ucDstAddr = ucSendStaID;
            stRespHead.usStatus = pstStaInfo->usStatusInfo;
            stRespHead.usSDataLen = 0U;

            /* Header */
            usIndex = 0U;
            memcpy( &pucResp[usIndex], &stRespHead, (uint32_t)usRespHeadLen );

            /* CRC */
            usIndex += usRespHeadLen;
            uiCRC = SysCrc32Cal( 0U, &pucResp[0], (uint32_t)usIndex );
            memcpy( &pucResp[usIndex], &uiCRC, sizeof(uint32_t));

            /* Reverse Message */
            usIndex += (uint16_t)sizeof(uint32_t);
            ReverseData( &pucResp[0], (uint32_t)usIndex, &pucResp[usIndex]);

            /* Set Output Length */
            *pusRespLen = usRespLen;
            bResult = true;

            /* Reset Status Info. */
            pstStaInfo->usStatusInfo = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PHandleSendReqTO
*
* 功能描述: 本站作为发送控制站：发送超时错误处理
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          bSendTimeout：发送超时标志位。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleSendReqTO( uint8_t ucRecvStaID, bool_t bTimeout )
{
    bool_t bResult = false;

    if((ucRecvStaID >= (uint8_t)(MIN_STA_ID)) && (ucRecvStaID <= (uint8_t)(MAX_STA_ID)))
    {
        bResult = true;

        /* Set Timeout Flag */
        SysSetP2PSendTimeoutFlag( ucRecvStaID - 1U, bTimeout );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PMakeReq
*
* 功能描述: 构造将发送给指定接收控制站的请求帧
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          pucData：数据首地址；
*          usDataLen：数据长度；
*          pucReq：存放请求帧的首地址；
*
* 输出参数: pusReqLen：用于返回请求帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PMakeReq( uint8_t ucRecvStaID, uint8_t const pucData[], uint16_t usDataLen,
                   uint8_t pucReq[], uint16_t *pusReqLen )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usReqBuffLen = 0U;
    uint16_t usReqLen = 0U;
    uint16_t usReqHeadLen = 0U;
    uint32_t uiCRC = 0U;
    uint8_t ucLocalStaID = 0U;         /* 本控制站的ID */
    PMController_t emLocalPMID = PMA;  /* 本PM模块的编号 */
    StaReqHeadInfo_t stReqHead;
    P2PSendReqCommInfo_t *pstStaInfo = NULL;

    if(((ucRecvStaID >= (uint8_t)(MIN_STA_ID)) && (ucRecvStaID <= (uint8_t)(MAX_STA_ID))) &&
       (pucData != NULL) &&
       (pucReq != NULL) &&
       (pusReqLen != NULL) &&
       (usDataLen > 0U))
    {
        usReqBuffLen = *pusReqLen;  /* Store Length */
        *pusReqLen = 0U;            /* Reset Output Length */
        pstStaInfo = &s_stP2PSendInfo[ucRecvStaID - 1U];

        /* Calculate Request Length */
        usReqHeadLen = (uint16_t)sizeof( stReqHead );
        usReqLen = ( usReqHeadLen + usDataLen + (uint16_t)(P2PSAFETY_CRC_LEN)) * 2;

        /* Check Buffer Length */
        if( usReqBuffLen >= usReqLen )
        {
            ucLocalStaID = SharedGetStationID();
            emLocalPMID = SysGetLocalPMID();

            /* Make Response Head */
            stReqHead.usSeq = pstStaInfo->usSendSeqNum;
            stReqHead.ucSrcAddr = ((emLocalPMID << 6U) | ((ucLocalStaID-1U) & (uint8_t)(STAID_MASK)));
            stReqHead.ucDstAddr = ucRecvStaID;
            stReqHead.ucReserve = 0U;
            stReqHead.ucCtrlByte = pstStaInfo->ucCtrlByte;
            stReqHead.usSDataLen = usDataLen;

            /* Head */
            usIndex = 0U;
            memcpy( &pucReq[usIndex], &stReqHead, (uint32_t)usReqHeadLen );

            /* Safety Data */
            usIndex += usReqHeadLen;
            memcpy( &pucReq[usIndex], pucData, (uint32_t)usDataLen );

            /* CRC */
            usIndex += usDataLen;
            uiCRC = SysCrc32Cal( 0U, &pucReq[0], (uint32_t)usIndex );
            memcpy( &pucReq[usIndex], &uiCRC, sizeof(uint32_t));

            /* Reverse Message */
            usIndex += (uint16_t)sizeof(uint32_t);
            ReverseData( &pucReq[0], (uint32_t)usIndex, &pucReq[usIndex]);

            /* Set Output Length */
            *pusReqLen = usReqLen;
            bResult = true;
            /*printf( "P2PReq SSQ 0x%04x\n", pstStaInfo->usSendSeqNum );*/

            /* Add Send Sequence */
            pstStaInfo->usSendSeqNum++;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PHandleRespTO
*
* 功能描述: 应答超时错误处理
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleRespTO( uint8_t ucRecvStaID )
{
    bool_t bResult = false;
    P2PSendReqCommInfo_t *pstStaInfo = NULL;

    if((ucRecvStaID >= (uint8_t)(MIN_STA_ID)) && (ucRecvStaID <= (uint8_t)(MAX_STA_ID)))
    {
        bResult = true;
        pstStaInfo = &s_stP2PSendInfo[ucRecvStaID - 1U];
        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

        /* Update Send Sequence */
        /*pstStaInfo->usSendSeqNum++;*/

        /* Handle Safety Communication Error */
        HandleSafetyCommError( &( pstStaInfo->stErrInfo ), s_ucTolThr );

        /* Update Control Byte */
        /*if( pstStaInfo->stErrInfo.bErrFlag )
        {
            pstStaInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_FAILURE);
        }*/
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: P2PDecodeResp
*
* 功能描述: 解析来自指定接收控制站的应答帧：Additional Info. + Safety Frame + FCS
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          bRedundant：冗余链路标志位；
*          pucResp：应答帧首地址；
*          usRespLen：应答帧长度；
*
* 输出参数: pstResult：存放应答帧解析结果。
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PDecodeResp( uint8_t ucRecvStaID, bool_t bRedundant, uint8_t pucResp[], uint16_t usRespLen,
                      RespDecodeResultInfo_t *pstResult )
{
    bool_t bResult = false;
    uint8_t ucRecvStaIndex = 0U;
    P2PSendReqCommInfo_t *pstStaInfo = NULL;
    StaRespAddlInfo_t stAddlInfo;
    uint32_t uiLen = 0U;          /* Length */
    uint32_t uiTotalSLen = 0U;    /* Total Safety Length */
    uint32_t uiSFraLen = 0U;      /* Safety Frame Length */
    uint32_t uiSFraHeadLen = 0U;  /* Safety Frame Head Length */

    if(((ucRecvStaID >= (uint8_t)(MIN_STA_ID)) && (ucRecvStaID <= (uint8_t)(MAX_STA_ID))) &&
      ( pucResp != NULL ) &&
      ( usRespLen > 0U ) &&
      ( pstResult != NULL ))
    {
        ucRecvStaIndex = ucRecvStaID - 1U;
        pstStaInfo = &s_stP2PSendInfo[ucRecvStaIndex];
        ClearRespDecodeResult( pstResult );  /* Reset Decode Result Info. */

        /* Additional Info. */
        uiLen = sizeof( StaRespAddlInfo_t );
        if( usRespLen > (uint16_t)uiLen )
        {
            memcpy( &stAddlInfo, pucResp, uiLen );

            /* Length */
            if((usRespLen == stAddlInfo.usTotalLen) &&
               (usRespLen > (uint16_t)(uiLen + (uint32_t)(P2P_FCS_LEN))))
            {
                /* Total Safety Length */
                uiTotalSLen = (uint32_t)usRespLen - uiLen - (uint32_t)(P2P_FCS_LEN);

                /* Safety Frame */
                uiSFraHeadLen = sizeof( StaRespHeadInfo_t );
                uiSFraLen = ( uiSFraHeadLen + (uint32_t)(P2PSAFETY_CRC_LEN)) * 2;
                if( uiTotalSLen == uiSFraLen )
                {
                    bResult = true;

                    /* Head */
                    DecodePMResp( ucRecvStaID, bRedundant, pstStaInfo, &pucResp[uiLen], uiSFraLen, pstResult );
                }

                /* System Resource */
                SysSetP2PSendTolFlag( ucRecvStaIndex, pstStaInfo->stErrInfo.bTolFlag );
                SysSetP2PSendErrFlag( ucRecvStaIndex, pstStaInfo->stErrInfo.bErrFlag );

                /* Update Control Byte */
                /*if( pstStaInfo->stErrInfo.bErrFlag )
                {
                    pstStaInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_FAILURE);
                }
                else
                {
                    pstStaInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_FAILURE);
                }*/
            }
            else
            {
                //printf( "P2PER: ELen, %d, %d\n", usRespLen, stAddlInfo.usTotalLen );
                printf( "PEL1\n" );
            }
        }
        else
        {
            //printf( "P2PER: ELen, %d <= %d\n", usRespLen, (uint16_t)uiLen );
            printf( "PEL2\n" );
        }
    }

    return bResult;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: ClearStationInfo
*
* 功能描述: 清除模块内保存的与相应块相关的信息
*
* 输入参数: ucStaID：控制站ID， 1-64；
*          bP2PRecv：P2P接收信息标志位，true时，ucStaID代表发送站ID；否则ucStaID代表接收站ID。
*
* 输出参数: 无
*
* 返 回 值: 是否清除成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t ClearStationInfo( uint8_t ucStaID, bool_t bP2PRecv )
{
    bool_t bResult = false;
    uint8_t ucStaIndex = 0U;

    if((ucStaID >= (uint8_t)(MIN_STA_ID)) && (ucStaID <= (uint8_t)(MAX_STA_ID)))
    {
        bResult = true;
        ucStaIndex = ucStaID - 1U;

        if( bP2PRecv )
        {
            ClearP2PRecvInfo( &s_stP2PRecvInfo[ucStaIndex]);
        }
        else
        {
            ClearP2PSendInfo( &s_stP2PSendInfo[ucStaIndex]);
        }
    }

    return bResult;
}
#endif

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ClearP2PRecvInfo
*
* 功能描述: 本站作为接收站：清除本地保存的相应发送站的信息
*
* 输入参数: pstStaInfo：待清除的信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearP2PRecvInfo( P2PRecvReqCommInfo_t *pstStaInfo )
{
    if( pstStaInfo != NULL )
    {
        memset( pstStaInfo, 0, sizeof(P2PRecvReqCommInfo_t));
        pstStaInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
        pstStaInfo->stErrInfo.bTolFlag = false;
        pstStaInfo->stErrInfo.bErrFlag = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearP2PSendInfo
*
* 功能描述: 本站作为发送站：清除本地保存的相应接收站的信息
*
* 输入参数: pstStaInfo：待清除的信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearP2PSendInfo( P2PSendReqCommInfo_t *pstStaInfo )
{
    if( pstStaInfo != NULL )
    {
        memset( pstStaInfo, 0, sizeof(P2PSendReqCommInfo_t));
        pstStaInfo->stErrInfo.bTolFlag = false;
        pstStaInfo->stErrInfo.bErrFlag = false;
        pstStaInfo->usLastResSeqNum = 0xFFFF;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearReqDecodeResult
*
* 功能描述: 清除请求帧解析结果
*
* 输入参数: pstResult：请求帧解析结果
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearReqDecodeResult( ReqDecodeResultInfo_t *pstResult )
{
    if( pstResult != NULL )
    {
        pstResult->bRepeat = false;
        pstResult->emSelPMID = INVALID_PM_ID;
        pstResult->usDataLen = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearRespDecodeResult
*
* 功能描述: 清除应答帧解析结果
*
* 输入参数: pstResult：应答帧解析结果
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearRespDecodeResult( RespDecodeResultInfo_t *pstResult )
{
    if( pstResult != NULL )
    {
        memset( pstResult, 0, sizeof(RespDecodeResultInfo_t));
        pstResult->bRepeat = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodePMResp
*
* 功能描述: 解析来自某一PM的安全应答帧
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          bRedundant：冗余链路标志位；
*          pstStaInfo：本地保存的接收站信息；
*          pucSFra：安全应答帧首地址；
*          uiSFraLen：安全应答帧长度：Normal + Reverse；
*
* 输出参数: pstResult：存放应答帧解析结果
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodePMResp( uint8_t ucRecvStaID, bool_t bRedundant, P2PSendReqCommInfo_t *pstStaInfo,
                          uint8_t pucSFra[], uint32_t uiSFraLen, RespDecodeResultInfo_t *pstResult )
{
    bool_t bValid = false;
    uint32_t uiSingleSFraLen = 0U;
    uint8_t ucStatus = 0U;
    uint8_t ucRecvStaIndex = 0U;
    uint32_t uiSFraHeadLen = 0U;
    uint32_t uiHeadIndex = 0U;    /* Head Index */
    uint32_t uiRHeadIndex = 0U;   /* Reverse Head Index */
    uint32_t uiCRCIndex = 0U;     /* CRC Index */
    uint32_t uiRCRCIndex = 0U;    /* Reverse CRC Index */
    uint32_t uiCRC = 0U;          /* CRC Calculate Result */
    uint8_t ucLocalStaID = 0U;    /* 本控制站的ID */
    StaRespHeadInfo_t stHead;

    if(((ucRecvStaID >= (uint8_t)(MIN_STA_ID)) && (ucRecvStaID <= (uint8_t)(MAX_STA_ID))) &&
       ( pstStaInfo != NULL ) &&
       ( pucSFra != NULL ) &&
       ( uiSFraLen > 0U ) &&
       ( pstResult != NULL ))
    {
        bValid = true;
        pstResult->bRepeat = false;
        uiSingleSFraLen = uiSFraLen / 2;
        ucRecvStaIndex = ucRecvStaID - 1U;
        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );
        ucLocalStaID = SharedGetStationID();

        /* Reverse Message */
        ReverseData( &pucSFra[uiSingleSFraLen], uiSingleSFraLen, &pucSFra[uiSingleSFraLen]);

        /* Check Communication Error */
        /* Head */
        uiSFraHeadLen = sizeof( StaRespHeadInfo_t );
        memcpy( &stHead, &pucSFra[0], uiSFraHeadLen );
        uiHeadIndex = 0U;
        uiRHeadIndex = uiHeadIndex + uiSingleSFraLen;

        /* Compare Head */
        if( !CompareData( &pucSFra[uiHeadIndex], &pucSFra[uiRHeadIndex], uiSFraHeadLen ))
        {
            bValid = false;
            ucStatus |= (uint8_t)(BIT_PM_REDUN);
            printf( "PRspEHC\n" );
        }

        if( bValid )
        {
            /* CRC */
            uiCRCIndex = uiHeadIndex + uiSFraHeadLen;
            uiRCRCIndex = uiCRCIndex + uiSingleSFraLen;

            /* Compare CRC */
            if( !CompareData( &pucSFra[uiCRCIndex], &pucSFra[uiRCRCIndex], (uint32_t)(P2PSAFETY_CRC_LEN)))
            {
                bValid = false;
                ucStatus |= (uint8_t)(BIT_PM_REDUN);
                printf( "PRspECC\n" );
            }
        }

        if( bValid )
        {
            /* Calculate CRC & Check */
            uiCRC = SysCrc32Cal( 0U, &pucSFra[uiHeadIndex], uiCRCIndex );

            /* Compare */
            if( !CompareData( &pucSFra[uiCRCIndex], (uint8_t const*)(&uiCRC), (uint32_t)(P2PSAFETY_CRC_LEN)))
            {
                bValid = false;
                ucStatus |= (uint8_t)(BIT_PM_CRC);
                printf( "PRspEC\n" );
            }
        }

        if( bValid )
        {
            /* Check Head */
            /* Source/Destination Address */
            if(((stHead.ucSrcAddr & (uint8_t)(STAID_MASK)) != ucRecvStaIndex ) ||
               ( stHead.ucDstAddr != ucLocalStaID ))
            {
                bValid = false;
                ucStatus |= (uint8_t)(BIT_PM_ADDR);
                /*printf( "P2PER: EAddr, %d, %d, %d, %d\n", stHead.ucSrcAddr, ucRecvStaIndex, \
                        stHead.ucDstAddr, ucLocalStaID );*/
                printf( "PRspEA\n" );
            }
            else
            {
                /* Sequence */
                if( stHead.usSeq == (uint16_t)(pstStaInfo->usSendSeqNum - 1U))
                {
                    if( bRedundant )
                    {
                        if( stHead.usSeq == pstStaInfo->usLastResSeqNum )
                        {
                            pstResult->bRepeat = true;
                            bValid = false;
                            ucStatus |= (uint8_t)(BIT_PM_SEQ);  /* Sequence Error */
                            /*printf( "P2PER: Repeat#1 SQ%04X LSQ%04X\n", stHead.usSeq, pstStaInfo->usLastResSeqNum );*/
                            printf( "PRspES\n" );
                        }
                        else
                        {
                            bValid = true;
                            ucStatus = 0U;
                            printf( "PRspV\n" );
                        }
                    }
                    else
                    {
                        bValid = true;
                        ucStatus = 0U;
                        printf( "PRspV\n" );
                    }
                }
                else
                {
                    bValid = false;
                    ucStatus |= (uint8_t)(BIT_PM_SEQ);
                    if( bRedundant )
                    {
                        if( stHead.usSeq == pstStaInfo->usLastResSeqNum )
                        {
                            pstResult->bRepeat = true;
                            /*printf( "P2PER: Repeat#2 SQ%04X LSQ%04X\n", stHead.usSeq, pstStaInfo->usLastResSeqNum );*/
                            printf( "PRsp#2\n" );
                        }
                    }

                    if( !pstResult->bRepeat )
                    {
                        /* Sequence Error */
                        /*printf( "P2PER: ESQ Rdn%d MSQ%04X SSQ%04X LSQ%04X\n", bRedundant, stHead.usSeq, \
                                pstStaInfo->usSendSeqNum, pstStaInfo->usLastResSeqNum );*/
                        printf( "PRsp#3\n" );

                        /* Update SendSQ If needed */
                        HandleLocalSendSQ( &stHead, pstStaInfo );
                    }
                }

                pstStaInfo->usLastResSeqNum = stHead.usSeq;
            }
        }/* end of if - Valid */

        if( bValid )
        {
            /* No Communication Error */
            HandleSafetyCommRecover( &( pstStaInfo->stErrInfo ));

#ifndef DEBUG_P2PSAFETY
            /* Handle Status: Record WD Timeout / Alarm */
#endif
        }
        else
        {
            if( ucStatus != (uint8_t)(BIT_PM_SEQ))
            {
                /* Not Sequence Error */
                printf( "PRspENSQ\n" );
                HandleSafetyCommError( &( pstStaInfo->stErrInfo ), s_ucTolThr );
            }
            else
            {
                if((!pstResult->bRepeat) || (!bRedundant))
                {
                    /* Sequence Error */
                    /*printf( "P2PER: ESQ F %d %d.\n", pstResult->bRepeat, bRedundant );*/
                    HandleSafetyCommError( &( pstStaInfo->stErrInfo ), s_ucTolThr );
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleLocalSendSQ
*
* 功能描述: 根据应答信息，处理本地发送序号:
*          根据安全协议决定是否更改本地发送序号，以使三系PM的发送序号保持一致
*
* 输入参数: pstHead：安全帧头信息；
*          pstStaInfo：相应站的发送控制信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleLocalSendSQ( StaRespHeadInfo_t const *pstHead, P2PSendReqCommInfo_t *pstStaInfo )
{
    uint16_t usStatus = 0U;
    bool_t bUpdate = false;
    PMController_t emLocalPM = INVALID_PM_ID;

    if((pstHead != NULL) && (pstStaInfo != NULL))
    {
        if((uint16_t)(INIT_RECV_SEQ) == pstHead->usSeq )
        {
            pstStaInfo->usSendSeqNum = 0U;
            printf( "SSQR0\n" );
        }
        else
        {
            emLocalPM = SysGetLocalPMID();
            usStatus = pstHead->usStatus;
            if( PMA == emLocalPM )
            {
                /* Check If Multiple PM Request & Local PM SQ Error */
                if((usStatus & BIT_MULIPMREQ) &&
                   (usStatus & BIT_PMASEQ))
                {
                    /* Check If Other PM Req SQ Correct */
                    if( !(usStatus & BIT_PMBCRC) &&
                        !(usStatus & BIT_PMBADDR) &&
                        !(usStatus & BIT_PMBREDUN) &&
                        !(usStatus & BIT_PMBSEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ B\n" );
                    }
                    else if( !(usStatus & BIT_PMCCRC) &&
                             !(usStatus & BIT_PMCADDR) &&
                             !(usStatus & BIT_PMCREDUN) &&
                             !(usStatus & BIT_PMCSEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ C\n" );
                    }
                    else
                    {
                        printf( "NUSSQE1\n" );
                    }
                }
                else
                {
                    printf( "NUSSQ#A\n" );
                }
            }
            else if( PMB == emLocalPM )
            {
                /* Check If Multiple PM Request & Local PM SQ Error */
                if((usStatus & BIT_MULIPMREQ) &&
                   (usStatus & BIT_PMBSEQ))
                {
                    /* Check If Other PM Req SQ Correct */
                    if( !(usStatus & BIT_PMACRC) &&
                        !(usStatus & BIT_PMAADDR) &&
                        !(usStatus & BIT_PMAREDUN) &&
                        !(usStatus & BIT_PMASEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ A\n" );
                    }
                    else if( !(usStatus & BIT_PMCCRC) &&
                             !(usStatus & BIT_PMCADDR) &&
                             !(usStatus & BIT_PMCREDUN) &&
                             !(usStatus & BIT_PMCSEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ C\n" );
                    }
                    else
                    {
                        printf( "NUSSQE2\n" );
                    }
                }
                else
                {
                    printf( "NUSSQ#B\n" );
                }
            }
            else if( PMC == emLocalPM )
            {
                /* Check If Multiple PM Request & Local PM SQ Error */
                if((usStatus & BIT_MULIPMREQ) &&
                   (usStatus & BIT_PMCSEQ))
                {
                    /* Check If Other PM Req SQ Correct */
                    if( !(usStatus & BIT_PMACRC) &&
                        !(usStatus & BIT_PMAADDR) &&
                        !(usStatus & BIT_PMAREDUN) &&
                        !(usStatus & BIT_PMASEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ A\n" );
                    }
                    else if( !(usStatus & BIT_PMBCRC) &&
                             !(usStatus & BIT_PMBADDR) &&
                             !(usStatus & BIT_PMBREDUN) &&
                             !(usStatus & BIT_PMBSEQ))
                    {
                        /* Update Local Send SQ */
                        bUpdate = true;
                        printf( "USSQ B\n" );
                    }
                    else
                    {
                        printf( "NUSSQE3\n" );
                    }
                }
                else
                {
                    printf( "NUSSQ#C\n" );
                }
            }
            else
            {
                printf( "LPMID E%d\n", emLocalPM );
            }

            if( bUpdate )
            {
                pstStaInfo->usSendSeqNum = pstHead->usSeq + 1U;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodePMReq
*
* 功能描述: 解析来自某一PM的安全请求帧
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64；
*          usRecvSeq：相应的接收序号；
*          pucSFra：安全请求帧首地址；
*          usSFraLen：安全帧长度: Normal + Reverse。
*
* 输出参数: pstPMDataInfo：用于返回解析结果
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodePMReq( uint8_t ucSendStaID, uint16_t usRecvSeq, uint8_t pucSFra[], uint16_t usSFraLen,
                         StaSafetyDataInfo_t *pstPMDataInfo )
{
    bool_t bValid = false;
    uint16_t usSingleSFraLen = 0U;
    uint8_t ucSendStaIndex = 0U;
    uint32_t uiSFraHeadLen = 0U;
    uint32_t uiHeadIndex = 0U;    /* Head Index */
    uint32_t uiRHeadIndex = 0U;   /* Reverse Head Index */
    uint32_t uiSDataIndex = 0U;   /* Data Index */
    uint32_t uiRSDataIndex = 0U;  /* Reverse Data Index */
    uint32_t uiCRCIndex = 0U;     /* CRC Index */
    uint32_t uiRCRCIndex = 0U;    /* Reverse CRC Index */
    uint32_t uiCRC = 0U;          /* CRC Calculate Result */
    uint8_t ucLocalStaID = 0U;    /* 本控制站的ID */
    StaReqHeadInfo_t stHead;

    if(((ucSendStaID >= (uint8_t)(MIN_STA_ID)) && (ucSendStaID <= (uint8_t)(MAX_STA_ID))) &&
       ( pucSFra != NULL ) &&
       ( usSFraLen > 0U ) &&
       ( pstPMDataInfo != NULL ))
    {
        bValid = true;
        ucSendStaIndex = ucSendStaID - 1U;
        usSingleSFraLen = usSFraLen / 2U;
        ucLocalStaID = SharedGetStationID();

        /* Reverse */
        ReverseData( &pucSFra[usSingleSFraLen], (uint32_t)usSingleSFraLen, &pucSFra[usSingleSFraLen]);

        /* Check Communication Error */
        /* Head */
        uiSFraHeadLen = sizeof( StaReqHeadInfo_t );
        memcpy( &stHead, &pucSFra[0], uiSFraHeadLen );
        uiHeadIndex = 0U;
        uiRHeadIndex = uiHeadIndex + (uint32_t)usSingleSFraLen;

        /* Compare Head */
        if( !CompareData( &pucSFra[uiHeadIndex], &pucSFra[uiRHeadIndex], uiSFraHeadLen ))
        {
            bValid = false;
            pstPMDataInfo->bValid = false;
            pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_REDUN);
            printf( "PReqEHC\n" );
        }

        if( bValid )
        {
            /* Safety Data */
            uiSDataIndex = uiHeadIndex + uiSFraHeadLen;
            uiRSDataIndex = uiSDataIndex + (uint32_t)usSingleSFraLen;

            /* Reverse & Compare Data */
            if( !CompareData( &pucSFra[uiSDataIndex], &pucSFra[uiRSDataIndex], (uint32_t)stHead.usSDataLen ))
            {
                bValid = false;
                pstPMDataInfo->bValid = false;
                pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_REDUN);
                printf( "PReqEDC\n" );
            }
        }

        if( bValid )
        {
            /* CRC */
            uiCRCIndex = uiSDataIndex + (uint32_t)stHead.usSDataLen;
            uiRCRCIndex = uiCRCIndex + (uint32_t)usSingleSFraLen;

            /* Reverse & Compare CRC */
            if( !CompareData( &pucSFra[uiCRCIndex], &pucSFra[uiRCRCIndex], (uint32_t)(P2PSAFETY_CRC_LEN)))
            {
                bValid = false;
                pstPMDataInfo->bValid = false;
                pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_REDUN);
                printf( "PReqECC\n" );
            }
        }

        if( bValid )
        {
            /* Calculate CRC & Check */
            uiCRC = SysCrc32Cal( 0U, &pucSFra[uiHeadIndex], (uint32_t)uiCRCIndex );

            /* Compare */
            if( !CompareData( &pucSFra[uiCRCIndex], (uint8_t const*)(&uiCRC), (uint32_t)(P2PSAFETY_CRC_LEN)))
            {
                bValid = false;
                pstPMDataInfo->bValid = false;
                pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_CRC);
                printf( "PReqEC\n" );
            }
        }

        if( bValid )
        {
            /* Check Head */
            /* Source/Destination Address */
            if(((stHead.ucSrcAddr & (uint8_t)(STAID_MASK)) != ucSendStaIndex ) ||
               ( stHead.ucDstAddr != ucLocalStaID ))
            {
                pstPMDataInfo->bValid = false;
                pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_ADDR);
                /*printf( "P2PEReq: EAddr, %d, %d, %d, %d\n", stHead.ucSrcAddr, ucSendStaIndex, \
                        stHead.ucDstAddr, ucLocalStaID );*/
                printf( "PReqEA\n" );
            }
            else
            {
                /* Sequence */
                if( stHead.usSeq != (uint16_t)(usRecvSeq + 1U))
                {
                    pstPMDataInfo->bValid = false;
                    pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_SEQ);
                    /*printf( "P2PEReq: ESQ %04X %04X\n", stHead.usSeq, usRecvSeq );*/
                    printf( "PReqES\n" );
                }
                else
                {
                    /* Control Byte */
                    if((stHead.ucCtrlByte & (uint8_t)(BIT_CTRL_FAILURE)) != 0U )
                    {
                        /* Failure Flag */
                        pstPMDataInfo->bValid = false;
                        pstPMDataInfo->ucReqStatus |= (uint8_t)(BIT_PM_FAILURE);
                        printf( "PReqECB\n" );
                    }
                    else
                    {
                        pstPMDataInfo->bValid = true;
                        pstPMDataInfo->ucReqStatus = 0U;
                        pstPMDataInfo->usDataLen = stHead.usSDataLen;
                        memcpy( pstPMDataInfo->ucBuff, &pucSFra[uiSDataIndex], (uint32_t)stHead.usSDataLen );
                        printf( "PReqV\n" );
                    }
                }

                /* Store Send Sequence In PM Request Message */
                pstPMDataInfo->usReqSendSeq = stHead.usSeq;
            }
        }/* end of if - Valid */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakePMADataStatus
*
* 功能描述: 设置PMA的状态
*
* 输入参数: pstPMDataInfo：PMA 数据状态信息。
*
* 输出参数: 无
*
* 返 回 值: 状态信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t MakePMADataStatus( StaSafetyDataInfo_t const *pstPMDataInfo )
{
    uint8_t ucPMStatus = 0U;
    uint16_t usStatus = 0U;

    if( pstPMDataInfo != NULL )
    {
        if( !pstPMDataInfo->bValid )
        {
            ucPMStatus = pstPMDataInfo->ucReqStatus;
            if( ucPMStatus & (uint8_t)(BIT_PM_CRC))
            {
                usStatus |= (uint16_t)(BIT_PMACRC);   /* CRC */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_ADDR))
            {
                usStatus |= (uint16_t)(BIT_PMAADDR);  /* Address */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_SEQ))
            {
                usStatus |= (uint16_t)(BIT_PMASEQ);   /* Sequence */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_REDUN))
            {
                usStatus |= (uint16_t)(BIT_PMAREDUN); /* Redundant */
            }
        }
    }

    return usStatus;
}

/*
***********************************************************************************************************************
* 函数名称: MakePMBDataStatus
*
* 功能描述: 设置PMB的状态
*
* 输入参数: pstPMDataInfo：PMB 数据状态信息
*
* 输出参数: 无
*
* 返 回 值: 状态信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t MakePMBDataStatus( StaSafetyDataInfo_t const *pstPMDataInfo )
{
    uint8_t ucPMStatus = 0U;
    uint16_t usStatus = 0U;

    if( pstPMDataInfo != NULL )
    {
        if( !pstPMDataInfo->bValid )
        {
            ucPMStatus = pstPMDataInfo->ucReqStatus;
            if( ucPMStatus & (uint8_t)(BIT_PM_CRC))
            {
                usStatus |= (uint16_t)(BIT_PMBCRC);   /* CRC */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_ADDR))
            {
                usStatus |= (uint16_t)(BIT_PMBADDR);  /* Address */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_SEQ))
            {
                usStatus |= (uint16_t)(BIT_PMBSEQ);   /* Sequence */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_REDUN))
            {
                usStatus |= (uint16_t)(BIT_PMBREDUN); /* Redundant */
            }
        }
    }

    return usStatus;
}

/*
***********************************************************************************************************************
* 函数名称: MakePMCDataStatus
*
* 功能描述: 设置PMC的状态
*
* 输入参数: pstPMDataInfo：PMC 数据状态信息
*
* 输出参数: 无
*
* 返 回 值: 状态信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t MakePMCDataStatus( StaSafetyDataInfo_t const *pstPMDataInfo )
{
    uint8_t ucPMStatus = 0U;
    uint16_t usStatus = 0U;

    if( pstPMDataInfo != NULL )
    {
        if( !pstPMDataInfo->bValid )
        {
            ucPMStatus = pstPMDataInfo->ucReqStatus;
            if( ucPMStatus & (uint8_t)(BIT_PM_CRC))
            {
                usStatus |= (uint16_t)(BIT_PMCCRC);   /* CRC */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_ADDR))
            {
                usStatus |= (uint16_t)(BIT_PMCADDR);  /* Address */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_SEQ))
            {
                usStatus |= (uint16_t)(BIT_PMCSEQ);   /* Sequence */
            }

            if( ucPMStatus & (uint8_t)(BIT_PM_REDUN))
            {
                usStatus |= (uint16_t)(BIT_PMCREDUN); /* Redundant */
            }
        }
    }

    return usStatus;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqValid
*
* 功能描述: 存在有效请求帧时的处理
*
* 输入参数: pstResult： 请求帧解析信息；
*          pstStaInfo：本地保存的相应发送站的信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqValid( ReqDecodeInfo_t const *pstResult, P2PRecvReqCommInfo_t *pstStaInfo )
{
    if((pstResult != NULL) &&
       (pstStaInfo != NULL))
    {
        /* Update Receive Sequence */
        if( pstResult->stPMADataInfo.bValid )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMADataInfo.usReqSendSeq;
        }
        else if( pstResult->stPMBDataInfo.bValid )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMBDataInfo.usReqSendSeq;
        }
        else if( pstResult->stPMCDataInfo.bValid )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMCDataInfo.usReqSendSeq;
        }
        else
        {
            printf( "NVPReq\n" );
        }

        HandleSafetyCommRecover( &( pstStaInfo->stErrInfo ));  /* Communication Recover Handle */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqFailure
*
* 功能描述: 存在发送站通知倒向安全时的处理
*
* 输入参数: pstResult： 请求帧解析结果；
*          pstStaInfo：本地保存的相应发送站的信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqFailure( ReqDecodeInfo_t const *pstResult, P2PRecvReqCommInfo_t *pstStaInfo )
{
    if((pstResult != NULL) &&
       (pstStaInfo != NULL))
    {
        if((uint8_t)(BIT_PM_FAILURE) == pstResult->stPMADataInfo.ucReqStatus )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMADataInfo.usReqSendSeq;
        }
        else if((uint8_t)(BIT_PM_FAILURE) == pstResult->stPMBDataInfo.ucReqStatus )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMBDataInfo.usReqSendSeq;
        }
        else if((uint8_t)(BIT_PM_FAILURE) == pstResult->stPMCDataInfo.ucReqStatus )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMCDataInfo.usReqSendSeq;
        }
        else
        {
            printf( "NFPReq\n" );
        }

        HandleSafetyCommRecover( &( pstStaInfo->stErrInfo ));  /* Communication Recover Handle */
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: HandleReqError
*
* 功能描述: 请求错误处理
*
* 输入参数: bRedundant：冗余链路标志位；
*          pstResult： 请求帧解析结果；
*          pstStaInfo：本地保存的相应发送站的信息；
*          ucTolThr：容忍门限值。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqError( bool_t bRedundant, ReqDecodeInfo_t *pstResult,
                            P2PRecvReqCommInfo_t *pstStaInfo, uint8_t ucTolThr,
                            bool_t bPMAFraExist, bool_t bPMBFraExist, bool_t bPMCFraExist )
{
    if((pstResult != NULL) &&
       (pstStaInfo != NULL))
    {
        if( bPMAFraExist && bPMBFraExist && bPMCFraExist )
        {
            HandleReqErrorThree( bRedundant, pstResult, pstStaInfo, ucTolThr );
        }
        else if( bPMAFraExist && bPMBFraExist )
        {
            HandleReqErrorTwo( bRedundant, pstResult, pstStaInfo, ucTolThr, \
                               &( pstResult->stPMADataInfo), &( pstResult->stPMBDataInfo));
        }
        else if( bPMAFraExist && bPMCFraExist )
        {
            HandleReqErrorTwo( bRedundant, pstResult, pstStaInfo, ucTolThr, \
                               &( pstResult->stPMADataInfo), &( pstResult->stPMCDataInfo));
        }
        else if( bPMBFraExist && bPMCFraExist )
        {
            HandleReqErrorTwo( bRedundant, pstResult, pstStaInfo, ucTolThr, \
                               &( pstResult->stPMBDataInfo), &( pstResult->stPMCDataInfo));
        }
        else if( bPMAFraExist )
        {
            HandleReqErrorOne( bRedundant, pstResult, pstStaInfo, ucTolThr, &( pstResult->stPMADataInfo));
        }
        else if( bPMBFraExist )
        {
            HandleReqErrorOne( bRedundant, pstResult, pstStaInfo, ucTolThr, &( pstResult->stPMBDataInfo));
        }
        else if( bPMCFraExist )
        {
            HandleReqErrorOne( bRedundant, pstResult, pstStaInfo, ucTolThr, &( pstResult->stPMCDataInfo));
        }
        else
        {
            pstStaInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqErrorThree
*
* 功能描述: 请求错误处理 - 两系
*
* 输入参数: bRedundant：冗余链路标志位；
*          pstResult： 请求帧解析结果；
*          pstStaInfo：本地保存的相应发送站的信息；
*          ucTolThr：容忍门限值。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqErrorThree( bool_t bRedundant, ReqDecodeInfo_t *pstResult,
                                 P2PRecvReqCommInfo_t *pstStaInfo, uint8_t ucTolThr )
{
    bool_t bReset = false;
    bool_t bUpdate = false;

    if((pstResult != NULL) &&
       (pstStaInfo != NULL))
    {
        if(((uint8_t)(BIT_PM_SEQ) == pstResult->stPMADataInfo.ucReqStatus) &&
           ((uint8_t)(BIT_PM_SEQ) == pstResult->stPMBDataInfo.ucReqStatus) &&
           ((uint8_t)(BIT_PM_SEQ) == pstResult->stPMCDataInfo.ucReqStatus))
        {
            if((pstResult->stPMADataInfo.usReqSendSeq == pstResult->stPMBDataInfo.usReqSendSeq) &&
               (pstResult->stPMBDataInfo.usReqSendSeq == pstResult->stPMCDataInfo.usReqSendSeq))
            {
                if( bRedundant )
                {
                    if( pstResult->stPMADataInfo.usReqSendSeq == pstStaInfo->usRecvSeqNum )
                    {
                        pstResult->bRepeat = true;      /* Repeat */
                        pstStaInfo->usStatusInfo = 0U;  /* Reset Status Info. */
                    }
                    else
                    {
                        bUpdate = true;
                    }
                }
                else
                {
                    bUpdate = true;
                }
            }
            else
            {
                bReset = true;
            }
        }
        else
        {
            bReset = true;
        }

        if( bUpdate )
        {
            pstStaInfo->usRecvSeqNum = pstResult->stPMADataInfo.usReqSendSeq;
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }

        if( bReset )
        {
            pstStaInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqErrorTwo
*
* 功能描述: 请求错误处理 - 两系
*
* 输入参数: bRedundant：冗余链路标志位；
*          pstResult： 请求帧解析结果；
*          pstStaInfo：本地保存的相应发送站的信息；
*          ucTolThr：容忍门限值；
*          pstPM1DataInfo：PM1解析结果；
*          pstPM2DataInfo：PM2解析结果。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqErrorTwo( bool_t bRedundant, ReqDecodeInfo_t *pstResult, P2PRecvReqCommInfo_t *pstStaInfo,
                               uint8_t ucTolThr, StaSafetyDataInfo_t *pstPM1DataInfo, StaSafetyDataInfo_t *pstPM2DataInfo )
{
    bool_t bReset = false;
    bool_t bUpdate = false;

    if((pstResult != NULL) &&
       (pstStaInfo != NULL) &&
       (pstPM1DataInfo != NULL) &&
       (pstPM2DataInfo != NULL))
    {
        if(((uint8_t)(BIT_PM_SEQ) == pstPM1DataInfo->ucReqStatus) &&
           ((uint8_t)(BIT_PM_SEQ) == pstPM2DataInfo->ucReqStatus))
        {
            if( pstPM1DataInfo->usReqSendSeq == pstPM2DataInfo->usReqSendSeq )
            {
                if( bRedundant )
                {
                    if( pstPM1DataInfo->usReqSendSeq == pstStaInfo->usRecvSeqNum )
                    {
                        pstResult->bRepeat = true;      /* Repeat */
                        pstStaInfo->usStatusInfo = 0U;  /* Reset Status Info. */
                    }
                    else
                    {
                        bUpdate = true;
                    }
                }
                else
                {
                    bUpdate = true;
                }
            }
            else
            {
                bReset = true;
            }
        }
        else
        {
            bReset = true;
        }

        if( bUpdate )
        {
            pstStaInfo->usRecvSeqNum = pstPM1DataInfo->usReqSendSeq;
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }

        if( bReset )
        {
            pstStaInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleReqErrorOne
*
* 功能描述: 请求错误处理 - 一系
*
* 输入参数: bRedundant：冗余链路标志位；
*          pstResult： 请求帧解析结果；
*          pstStaInfo：本地保存的相应发送站的信息；
*          ucTolThr：容忍门限值；
*          pstPMDataInfo：PM解析结果。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleReqErrorOne( bool_t bRedundant, ReqDecodeInfo_t *pstResult, P2PRecvReqCommInfo_t *pstStaInfo,
                               uint8_t ucTolThr, StaSafetyDataInfo_t *pstPMDataInfo )
{
    bool_t bReset = false;
    bool_t bUpdate = false;

    if((pstResult != NULL) &&
       (pstStaInfo != NULL) &&
       (pstPMDataInfo != NULL))
    {
        if((uint8_t)(BIT_PM_SEQ) == pstPMDataInfo->ucReqStatus )
        {
            if( bRedundant )
            {
                if( pstPMDataInfo->usReqSendSeq == pstStaInfo->usRecvSeqNum )
                {
                    pstResult->bRepeat = true;      /* Repeat */
                    pstStaInfo->usStatusInfo = 0U;  /* Reset Status Info. */
                }
                else
                {
                    bUpdate = true;
                }
            }
            else
            {
                bUpdate = true;
            }
        }
        else
        {
            bReset = true;
        }

        if( bUpdate )
        {
            pstStaInfo->usRecvSeqNum = pstPMDataInfo->usReqSendSeq;
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }

        if( bReset )
        {
            pstStaInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
            HandleSafetyCommError( &( pstStaInfo->stErrInfo ), ucTolThr );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakeDecodeResult
*
* 功能描述: 根据解析信息构造解析结果
*
* 输入参数: pstDecodeInfo：解析信息；
*          pstResult： 解析结果。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void MakeDecodeResult( ReqDecodeInfo_t const *pstDecodeInfo, ReqDecodeResultInfo_t *pstResult )
{
    bool_t bPMA = false;       /* PMA数据有效标志 */
    bool_t bPMB = false;       /* PMB数据有效标志 */
    bool_t bPMC = false;       /* PMC数据有效标志 */
    bool_t bPMABSame = false;  /* PMA 与 PMB 数据是否一致 */
    bool_t bPMACSame = false;  /* PMA 与 PMC 数据是否一致 */
    bool_t bPMBCSame = false;  /* PMB 与 PMC 数据是否一致 */

    if((pstDecodeInfo != NULL ) &&
       (pstResult != NULL))
    {
        if( pstDecodeInfo->bRepeat )
        {
            ClearReqDecodeResult( pstResult );
        }
        else
        {
            /* Select Data: Valid, Same, Smaller PM */
            bPMA = pstDecodeInfo->stPMADataInfo.bValid;
            bPMB = pstDecodeInfo->stPMBDataInfo.bValid;
            bPMC = pstDecodeInfo->stPMCDataInfo.bValid;

            /* Vote: Compare */
            if( bPMA && bPMB && bPMC )
            {
                if((pstDecodeInfo->stPMADataInfo.usDataLen == pstDecodeInfo->stPMBDataInfo.usDataLen) &&
                   (pstDecodeInfo->stPMADataInfo.usDataLen == pstDecodeInfo->stPMCDataInfo.usDataLen))
                {
                    /* All Normal: Compare And Select */
                    pstResult->usDataLen = pstDecodeInfo->stPMADataInfo.usDataLen;

                    /* Compare PMA & PMB */
                    bPMABSame = CompareData( pstDecodeInfo->stPMADataInfo.ucBuff, pstDecodeInfo->stPMBDataInfo.ucBuff, \
                                             (uint32_t)pstDecodeInfo->stPMADataInfo.usDataLen );

                    /* Compare PMA & PMC */
                    bPMACSame = CompareData( pstDecodeInfo->stPMADataInfo.ucBuff, pstDecodeInfo->stPMCDataInfo.ucBuff, \
                                             (uint32_t)pstDecodeInfo->stPMADataInfo.usDataLen );

                    if( bPMABSame && bPMACSame )
                    {
                        /* All Same: Use PMA */
                        printf( "MDRAA\n" );
                        pstResult->emSelPMID = PMA;
                        memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                                (uint32_t)pstResult->usDataLen );
                    }
                    else
                    {
                        /* Not All Same */
                        /* Compare PMB & PMC */
                        bPMBCSame = CompareData( pstDecodeInfo->stPMBDataInfo.ucBuff, \
                                                 pstDecodeInfo->stPMCDataInfo.ucBuff, \
                                                 (uint32_t)pstDecodeInfo->stPMBDataInfo.usDataLen );
                        if( bPMABSame || bPMACSame || bPMBCSame )
                        {
                            /* Select Smaller PM */
                            if( bPMBCSame )
                            {
                                printf( "MDRBCB\n" );
                                pstResult->emSelPMID = PMB;
                                memcpy( pstResult->ucBuff, pstDecodeInfo->stPMBDataInfo.ucBuff, \
                                        (uint32_t)pstResult->usDataLen );
                            }
                            else
                            {
                                printf( "MDR#A\n" );
                                pstResult->emSelPMID = PMA;
                                memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                                        (uint32_t)pstResult->usDataLen );
                            }
                        }
                        else
                        {
                            /* All Different, Use PMA */
                            printf( "MDRADA\n" );
                            pstResult->emSelPMID = PMA;
                            memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                                    (uint32_t)pstResult->usDataLen );
                        }
                    }
                }
                else
                {
                    /* Data length different: No valid PM data */
                    printf( "MDREL1\n" );
                    ClearReqDecodeResult( pstResult );
                }
            }
            else if( bPMA && bPMB )
            {
                if( pstDecodeInfo->stPMADataInfo.usDataLen == pstDecodeInfo->stPMBDataInfo.usDataLen )
                {
                    /* Two Normal: Select Smaller */
                    printf( "MDRABA\n" );
                    pstResult->emSelPMID = PMA;
                    pstResult->usDataLen = pstDecodeInfo->stPMADataInfo.usDataLen;
                    memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                            (uint32_t)pstResult->usDataLen );
                }
                else
                {
                    /* Data length different: No valid PM data */
                    printf( "MDREL2\n" );
                    ClearReqDecodeResult( pstResult );
                }
            }
            else if( bPMA && bPMC )
            {
                if( pstDecodeInfo->stPMADataInfo.usDataLen == pstDecodeInfo->stPMCDataInfo.usDataLen )
                {
                    /* Two Normal: Select Smaller */
                    printf( "MDRACA\n" );
                    pstResult->emSelPMID = PMA;
                    pstResult->usDataLen = pstDecodeInfo->stPMADataInfo.usDataLen;
                    memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                            (uint32_t)pstResult->usDataLen );
                }
                else
                {
                    /* Data length different: No valid PM data */
                    printf( "MDREL3\n" );
                    ClearReqDecodeResult( pstResult );
                }
            }
            else if( bPMB && bPMC )
            {
                if( pstDecodeInfo->stPMBDataInfo.usDataLen == pstDecodeInfo->stPMCDataInfo.usDataLen )
                {
                    /* Two Normal: Select Smaller */
                    printf( "MDRBCB\n" );
                    pstResult->emSelPMID = PMB;
                    pstResult->usDataLen = pstDecodeInfo->stPMBDataInfo.usDataLen;
                    memcpy( pstResult->ucBuff, pstDecodeInfo->stPMBDataInfo.ucBuff, \
                            (uint32_t)pstResult->usDataLen );
                }
                else
                {
                    /* Data length different: No valid PM data */
                    printf( "MDREL4\n" );
                    ClearReqDecodeResult( pstResult );
                }
            }
            else if( bPMA )
            {
                printf( "MDRA\n" );
                pstResult->emSelPMID = PMA;
                pstResult->usDataLen = pstDecodeInfo->stPMADataInfo.usDataLen;
                memcpy( pstResult->ucBuff, pstDecodeInfo->stPMADataInfo.ucBuff, \
                        (uint32_t)pstResult->usDataLen );
            }
            else if( bPMB )
            {
                printf( "MDRB\n" );
                pstResult->emSelPMID = PMB;
                pstResult->usDataLen = pstDecodeInfo->stPMBDataInfo.usDataLen;
                memcpy( pstResult->ucBuff, pstDecodeInfo->stPMBDataInfo.ucBuff, \
                        (uint32_t)pstResult->usDataLen );
            }
            else if( bPMC )
            {
                printf( "MDRC\n" );
                pstResult->emSelPMID = PMC;
                pstResult->usDataLen = pstDecodeInfo->stPMCDataInfo.usDataLen;
                memcpy( pstResult->ucBuff, pstDecodeInfo->stPMCDataInfo.ucBuff, \
                        (uint32_t)pstResult->usDataLen );
            }
            else
            {
                /* All Invalid */
                printf( "MDRNV\n" );
                ClearReqDecodeResult( pstResult );
            }
        }

        /* Update Repeat Flag */
        pstResult->bRepeat = pstDecodeInfo->bRepeat;
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

