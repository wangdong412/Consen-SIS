/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*
*  项目名称    : 安全控制系统
*
*  模块名称    : PM与IO模块安全通讯
*
*  文件名称    : PMIOSafetyComm.c
*
*  功能简介    : PMIO安全通讯相关处理
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
#include "../inc/PMIOSafetyComm.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static uint8_t s_ucTolCnt;                                     /* 用户配置的可容忍次数 */
static uint8_t s_ucTolThr;                                     /* 容忍门限值 */
static PMIOCommInfo_t s_stPMIOCommInfo[IPBUS_MODULE_MAX_NUM];  /* PM维护的通信信息，IO模块实际从4开始 */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void ClearPMIOCommInfo( PMIOCommInfo_t *pstPMIOInfo );
static bool_t DecodeIOResp( uint16_t usPMID, uint16_t usIOID, PMIOCommInfo_t *pstPMIOInfo, uint8_t pucSResp[],
                            uint32_t uiSRespLen, uint8_t ucTolThr );


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: PMIOCommInit
*
* 功能描述: PM/IO间安全通讯初始化函数
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

void PMIOCommInit( void )
{
    uint8_t ucIndex = 0U;

    UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

    for( ucIndex = 0U; ucIndex < (uint8_t)(IPBUS_MODULE_MAX_NUM); ucIndex++ )
    {
        /* PM I/O Communication Info. */
        ClearPMIOCommInfo( &s_stPMIOCommInfo[ucIndex]);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: PMIOHandleSendTO
*
* 功能描述: 发送超时错误处理
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bTimeout：发送超时标志。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleSendTO( uint8_t ucIOID, bool_t bTimeout )
{
    bool_t bResult = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bResult = true;

        /* Set Timeout Flag */
        SysSetPMIOSendTimeoutFlag( ucIOID, bTimeout );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMIOMakeReq
*
* 功能描述: 构造将发送给指定I/O模块的请求帧
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          pucData：输出模块数据首地址；
*          ucDataLen：输出模块数据长度；
*          bActiveFV：ActiveFV标志；
*          pucReq：存放请求帧的首地址；
*
* 输出参数: pusReqLen：用于返回请求帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOMakeReq( uint8_t ucIOID, uint8_t const pucData[], uint8_t ucDataLen, bool_t bActiveFV, bool_t bEnableFlg,
                    uint8_t pucReq[], uint16_t *pusReqLen )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usReqBuffLen = 0U;
    uint16_t usReqLen = 0U;
    uint16_t usReqHeadLen = 0U;
    uint32_t uiCRC = 0U;
    PMIOReqHeadInfo_t stReqHead;
    PMIOCommInfo_t *pstPMIOInfo = NULL;

    if(((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) && (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO))) &&
       (pucData != NULL) &&
       (pucReq != NULL) &&
       (pusReqLen != NULL))
    {
        usReqBuffLen = *pusReqLen;  /* Store Length */
        *pusReqLen = 0U;            /* Reset Output Length */
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOID];

        /* Calculate Request Length */
        usReqHeadLen = (uint16_t)sizeof( PMIOReqHeadInfo_t );
        usReqLen = ( usReqHeadLen + (uint16_t)ucDataLen + (uint16_t)(PMIOSAFETY_CRC_LEN)) * 2;

        /* Check Buffer Length */
        if( usReqBuffLen >= usReqLen )
        {
            /* Make Response Head */
            if( bActiveFV )
            {
                pstPMIOInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_ACTIVEFV);
            }
            else
            {
                pstPMIOInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_ACTIVEFV);
            }
            
            if(bEnableFlg)
            {
                pstPMIOInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_IPARAEN);
                printf("EN %d\n",ucIOID);
            }
            else
            {
                pstPMIOInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_IPARAEN);
            }

            stReqHead.usPMID = (uint16_t)(IPBUS_MASTER_ADDR) + (uint16_t)SysGetLocalPMID();
            stReqHead.usIOID = (uint16_t)ucIOID;
            stReqHead.usSeq = pstPMIOInfo->usSendSeqNum;
            stReqHead.ucCtrlByte = pstPMIOInfo->ucCtrlByte;
            stReqHead.ucSDataLen = ucDataLen;

            /* Head */
            usIndex = 0U;
            memcpy( &pucReq[usIndex], &stReqHead, (uint32_t)usReqHeadLen );

            /* Safety Data */
            usIndex += usReqHeadLen;
            memcpy( &pucReq[usIndex], pucData, (uint32_t)ucDataLen );

            /* CRC */
            usIndex += (uint16_t)ucDataLen;
            uiCRC = SysCrc32Cal( 0U, &pucReq[0], (uint32_t)usIndex );
            memcpy( &pucReq[usIndex], &uiCRC, sizeof(uint32_t));

            /* Reverse Message */
            usIndex += (uint16_t)sizeof(uint32_t);
            ReverseData( &pucReq[0], (uint32_t)usIndex, &pucReq[usIndex]);

            /* Set Output Length */
            *pusReqLen = usReqLen;
            bResult = true;

            /* Add Send Sequence */
            pstPMIOInfo->usSendSeqNum++;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMIOHandleRecvTO
*
* 功能描述: 接收超时错误处理
*
* 输入参数: ucIOID：I/O模块ID, 4-239
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleRecvTO( uint8_t ucIOID )
{
    bool_t bResult = false;
    PMIOCommInfo_t *pstPMIOInfo = NULL;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bResult = true;
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOID];
        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

        /* Handle Safety Communication Error */
        HandleSafetyCommError( &( pstPMIOInfo->stErrInfo ), s_ucTolThr );

        /* System Resource */
        SysSetPMIOTolFlag( ucIOID, pstPMIOInfo->stErrInfo.bTolFlag );
        SysSetPMIOErrFlag( ucIOID, pstPMIOInfo->stErrInfo.bErrFlag );

        /* Update Control Byte */
        if( pstPMIOInfo->stErrInfo.bErrFlag )
        {
            pstPMIOInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_ACTIVEFV);
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMIODecodeResp
*
* 功能描述: 解析来自I/O模块的应答帧
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          pucResp：应答帧首地址；
*          usRespLen：应答帧长度；
*          pucData：存放输入数据的首地址；
*
* 输出参数: pucDataLen：返回输入数据的长度；
*          pbParOK：true-从站已采用主站下发的新参数。
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIODecodeResp( uint8_t ucIOID, uint8_t pucResp[], uint16_t usRespLen,
                       uint8_t pucData[], uint8_t *pucDataLen, bool_t *pbParOK )
{
    bool_t bResult = false;
    uint8_t ucDataBuffLen = 0U;   /* Data Buffer Length */
    uint32_t uiSHeadLen = 0U;     /* Safety Head Length */
    uint32_t uiSFraLen = 0U;      /* Safety Frame Length */
    uint16_t usPMID = 0U;
    PMIOCommInfo_t *pstPMIOInfo = NULL;
    PMIORespHeadInfo_t stHead;

    if(((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) && (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO))) &&
       (pucResp != NULL) &&
       (usRespLen > 0U) &&
       (pucData != NULL) &&
       (pucDataLen != NULL) &&
       (pbParOK != NULL))
    {
        ucDataBuffLen = *pucDataLen;  /* Store Data Buffer Length */
        *pucDataLen = 0U;             /* Reset */
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOID];

        /* Safety Response Head */
        uiSHeadLen = sizeof( PMIORespHeadInfo_t );
        if( usRespLen > (uint16_t)uiSHeadLen )
        {
            memcpy( &stHead, pucResp, uiSHeadLen );
            uiSFraLen = (uiSHeadLen + (uint32_t)stHead.ucSDataLen + (uint32_t)(PMIOSAFETY_CRC_LEN)) * 2;

            /* Check Length */
            if( usRespLen >= uiSFraLen )
            {
                /* Update */
                UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

                /* Decode */
                usPMID = (uint16_t)(IPBUS_MASTER_ADDR) + (uint16_t)SysGetLocalPMID();
                if( DecodeIOResp(usPMID, (uint16_t)ucIOID, pstPMIOInfo, pucResp, uiSFraLen, s_ucTolThr ))
                {
                    if( ucDataBuffLen >= stHead.ucSDataLen )
                    {
                        bResult = true;
                        *pucDataLen = stHead.ucSDataLen;
                        if( stHead.ucSDataLen > 0U )
                        {
                            memcpy( pucData, &pucResp[uiSHeadLen], (uint32_t)stHead.ucSDataLen );
                        }
                    }

                    *pbParOK = pstPMIOInfo->bNewIPara;
                }

                /* System Resource */
                SysSetPMIOTolFlag( ucIOID, pstPMIOInfo->stErrInfo.bTolFlag );
                SysSetPMIOErrFlag( ucIOID, pstPMIOInfo->stErrInfo.bErrFlag );
                SysSetPMIOFailureFlag( ucIOID, pstPMIOInfo->bIOModFailure );
                SysSetPMIOFVActivedFlag( ucIOID, pstPMIOInfo->bFVActived );
                SysSetPMIONewIParaFlag( ucIOID, pstPMIOInfo->bNewIPara );

                /* Update Control Byte */
                if( pstPMIOInfo->stErrInfo.bErrFlag )
                {
                    pstPMIOInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_ACTIVEFV);
                }
                else
                {
                    if((pstPMIOInfo->ucCtrlByte & (uint8_t)(BIT_CTRL_ACTIVEFV)) != 0U )
                    {
                        /* Communication: Recover */
                        if( SysGetAutoResume())
                        {
                            pstPMIOInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_ACTIVEFV);
                        }
                        else
                        {
                            /* Need User Confirm: Check if user confirmed! */
                            if( SysGetUserConfirm())
                            {
                                pstPMIOInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_ACTIVEFV);
                            }
                        }
                    }
                }
            }/* end of if( usRespLen == uiSFraLen ) */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ClearPMIOCommInfo
*
* 功能描述: 清除某一IO模块对应的通信信息
*
* 输入参数: pstPMIOInfo：待清除的信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearPMIOCommInfo( PMIOCommInfo_t *pstPMIOInfo )
{
    if( pstPMIOInfo != NULL )
    {
        memset( pstPMIOInfo, 0, sizeof(PMIOCommInfo_t));
        pstPMIOInfo->stErrInfo.bTolFlag = false;
        pstPMIOInfo->stErrInfo.bErrFlag = false;
        pstPMIOInfo->bIOModFailure = false;
        pstPMIOInfo->bFVActived = false;
        pstPMIOInfo->bNewIPara = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeIOResp
*
* 功能描述: 解析PM发送的安全帧
*
* 输入参数: usPMID：PM ID；
*          usIOID: I/O模块ID, 4-239；
*          pstPMIOInfo：I/O模块对应的本地信息；
*          pucSResp：安全应答帧首地址；
*          uiSRespLen：安全应答帧的长度：Normal + Reverse；
*          ucTolThr：容忍门限值。
*
* 输出参数: 无
*
* 返 回 值: 安全帧是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t DecodeIOResp( uint16_t usPMID, uint16_t usIOID, PMIOCommInfo_t *pstPMIOInfo, uint8_t pucSResp[],
                            uint32_t uiSRespLen, uint8_t ucTolThr )
{
    bool_t bValid = false;
    uint32_t uiSingleSFraLen = 0U;
    uint8_t ucCommStatus = 0U;    /* Communication Status */
    uint32_t uiSHeadLen = 0U;     /* Safety Response Head Length */
    uint32_t uiHeadIndex = 0U;    /* Head Index */
    uint32_t uiRHeadIndex = 0U;   /* Reverse Head Index */
    uint32_t uiSDataIndex = 0U;   /* Data Index */
    uint32_t uiRSDataIndex = 0U;  /* Reverse Data Index */
    uint32_t uiCRCIndex = 0U;     /* CRC Index */
    uint32_t uiRCRCIndex = 0U;    /* Reverse CRC Index */
    uint32_t uiCRC = 0U;          /* CRC Calculate Result */
    PMIORespHeadInfo_t stHead;

    if(((usIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) && (usIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO))) &&
       (pstPMIOInfo != NULL) &&
       (pucSResp != NULL) &&
       (uiSRespLen > 0U))
    {
//        printf("E0\n");
        bValid = true;
        uiSingleSFraLen = uiSRespLen / 2;

        /* Reverse Message */
        ReverseData( &pucSResp[uiSingleSFraLen], uiSingleSFraLen, &pucSResp[uiSingleSFraLen]);

        /* Check Communication Error */
        /* Head */
        uiSHeadLen = sizeof( PMIORespHeadInfo_t );
        memcpy( &stHead, &pucSResp[0], uiSHeadLen );
        uiHeadIndex = 0U;
        uiRHeadIndex = uiHeadIndex + uiSingleSFraLen;

        /* Reverse & Compare Head */
        if( !CompareData( &pucSResp[uiHeadIndex], &pucSResp[uiRHeadIndex], uiSHeadLen ))
        {
            bValid = false;
            ucCommStatus |= (uint8_t)(BIT_COMM_REDUN);
//            printf("E1\n");
        }

        if( bValid )
        {
            /* Safety Data */
            uiSDataIndex = uiHeadIndex + uiSHeadLen;
            uiRSDataIndex = uiSDataIndex + uiSingleSFraLen;

            if( stHead.ucSDataLen > 0U )
            {
                /* Reverse & Compare Data */
                if( !CompareData( &pucSResp[uiSDataIndex], &pucSResp[uiRSDataIndex], (uint32_t)stHead.ucSDataLen ))
                {
                    bValid = false;
                    ucCommStatus |= (uint8_t)(BIT_COMM_REDUN);
//                    printf("E2\n");
                }
            }
        }

        if( bValid )
        {
            /* CRC */
            uiCRCIndex = uiSDataIndex + (uint32_t)stHead.ucSDataLen;
            uiRCRCIndex = uiCRCIndex + uiSingleSFraLen;

            /* Reverse & Compare CRC */
            if( !CompareData( &pucSResp[uiCRCIndex], &pucSResp[uiRCRCIndex], (uint32_t)(PMIOSAFETY_CRC_LEN)))
            {
                bValid = false;
                ucCommStatus |= (uint8_t)(BIT_COMM_REDUN);
//                printf("E3\n");
            }
        }

        if( bValid )
        {
            /* Calculate CRC & Check */
            uiCRC = SysCrc32Cal( 0U, &pucSResp[uiHeadIndex], uiCRCIndex );

//            printf("CRC=%x\n", uiCRC);

            /* Compare */
            if( !CompareData( &pucSResp[uiCRCIndex], (uint8_t const*)(&uiCRC), (uint32_t)(PMIOSAFETY_CRC_LEN)))
            {
                bValid = false;
                ucCommStatus |= (uint8_t)(BIT_COMM_CRC);
//                printf("E4\n");
            }
        }

        if( bValid )
        {
            /* Check Head */
            /* Source/Destination Address */
            if((stHead.usIOID != usIOID) ||
               (stHead.usPMID != usPMID))
            {
                bValid = false;
                ucCommStatus |= (uint8_t)(BIT_COMM_ADDR);
//                printf("E5\n");
            }
            else
            {
                /* Sequence */
                if( stHead.usSeq == (uint16_t)(pstPMIOInfo->usSendSeqNum - 1U))
                {
                    bValid = true;
                    ucCommStatus = 0U;
                }
                else
                {
                    bValid = false;
                    ucCommStatus |= (uint8_t)(BIT_COMM_SEQ);  /* Sequence Error */
//                    printf("E6\n");
                }
            }
        }/* end of if - Valid */

        if( bValid )
        {
            /* No Communication Error：Check I/O Module Status Byte */
            if(((stHead.ucStatusByte & BIT_STATUS_REDUN) != 0) ||
               ((stHead.ucStatusByte & BIT_STATUS_CRCSEQ) != 0) ||
               ((stHead.ucStatusByte & BIT_STATUS_ADDR) != 0) ||
               ((stHead.ucStatusByte & BIT_STATUS_WDTO) != 0))
            {
//                printf("E8\n");
                /* I/O Module Report Communication Error */
                HandleSafetyCommError( &( pstPMIOInfo->stErrInfo ), ucTolThr );
            }
            else
            {
                /* No Error */
                HandleSafetyCommRecover( &( pstPMIOInfo->stErrInfo ));
            }

            /* Update Status Byte */
            /* Failure */
            if((stHead.ucStatusByte & (uint8_t)(BIT_STATUS_FAIL)) != 0 )
            {
                pstPMIOInfo->bIOModFailure = true;
            }
            else
            {
                pstPMIOInfo->bIOModFailure = false;
            }

            /* FV Actived */
            if((stHead.ucStatusByte & (uint8_t)(BIT_STATUS_FV)) != 0 )
            {
                pstPMIOInfo->bFVActived = true;
            }
            else
            {
                pstPMIOInfo->bFVActived = false;
            }

            /* New i-Parameter */
            if((stHead.ucStatusByte & (uint8_t)(BIT_STATUS_IPARAOK)) != 0 )
            {
                pstPMIOInfo->bNewIPara = true;
            }
            else
            {
                pstPMIOInfo->bNewIPara = false;
            }
        }
        else
        {
            HandleSafetyCommError( &( pstPMIOInfo->stErrInfo ), ucTolThr );
        }
    }

    return bValid;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

