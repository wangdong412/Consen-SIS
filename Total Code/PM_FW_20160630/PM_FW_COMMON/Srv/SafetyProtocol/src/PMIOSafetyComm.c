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

#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"
#include "../inc/PMIOSafetyComm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static SysPMID_t s_emLocalPMID;                             /* 本PM模块的编号 */
static uint8_t s_ucTolCnt;                                  /* 用户配置的可容忍次数 */
static uint8_t s_ucTolThr;                                  /* 容忍门限值 */
static PMIOCommInfo_t s_stPMIOCommInfo[MAX_IO_MODULE_CNT];  /* PM维护的通信信息：每个I/O模块对应其中一个数组成员，
                                                                                                                                      索引号为I/O模块编号-4 */

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

    s_emLocalPMID = SysGetPMID();
    UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

    for( ucIndex = 0U; ucIndex < MAX_IO_MODULE_CNT; ucIndex++ )
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
* 输入参数: ucIOIndex：I/O模块的索引值
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleSendTO( uint8_t ucIOIndex )
{
    bool_t bResult = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bResult = true;

        /* Set Timeout Flag */
        SysSetPMIOSendTimeoutFlag( ucIOIndex, true );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMIOMakeReq
*
* 功能描述: 构造将发送给指定I/O模块的请求帧
*
* 输入参数: ucIOIndex：I/O模块的索引值；
*          pucData：输出模块数据首地址；
*          ucDataLen：输出模块数据长度；
*          bIParaEn：PM准备下发从站模块参数时，置true；
*          pucReq：存放请求帧的首地址；
*
* 输出参数: pusReqLen：用于返回请求帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOMakeReq( uint8_t ucIOIndex, uint8_t const pucData[], uint8_t ucDataLen, bool_t bIParaEn,
                    uint8_t pucReq[], uint16_t *pusReqLen )
{
    bool_t bResult = false;
    uint8_t ucIOID = 0U;
    uint16_t usIndex = 0U;
    uint16_t usReqBuffLen = 0U;
    uint16_t usReqLen = 0U;
    uint16_t usReqHeadLen = 0U;
    uint32_t uiCRC = 0U;
    PMIOReqHeadInfo_t stReqHead;
    PMIOCommInfo_t *pstPMIOInfo = NULL;

    if((ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT)) &&
       (pucData != NULL) &&
       (ucDataLen > 0U) &&
       (pucReq != NULL) &&
       (pusReqLen != NULL))
    {
        usReqBuffLen = *pusReqLen;  /* Store Length */
        *pusReqLen = 0U;            /* Reset Output Length */
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOIndex];

        /* Calculate Request Length */
        usReqHeadLen = (uint16_t)sizeof( PMIOReqHeadInfo_t );
        usReqLen = ( usReqHeadLen + (uint16_t)ucDataLen + (uint16_t)(PMIOSAFETY_CRC_LEN)) * 2;

        /* Check Buffer Length */
        if( usReqBuffLen >= usReqLen )
        {
            s_emLocalPMID = SysGetPMID();

            /* Make Response Head */
            ucIOID = ucIOIndex + (uint8_t)(MIN_IO_MODULE_ID);  /* IO Module ID */
            if( bIParaEn )
            {
                pstPMIOInfo->ucCtrlByte |= (uint8_t)(BIT_CTRL_IPARAEN);
            }
            else
            {
                pstPMIOInfo->ucCtrlByte &= ~(uint8_t)(BIT_CTRL_IPARAEN);
            }

            stReqHead.usPMID = (uint16_t)s_emLocalPMID;
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
* 输入参数: ucIOIndex：I/O模块的索引值
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleRecvTO( uint8_t ucIOIndex )
{
    bool_t bResult = false;
    PMIOCommInfo_t *pstPMIOInfo = NULL;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bResult = true;
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOIndex];
        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

        /* Handle Safety Communication Error */
        HandleSafetyCommError( &( pstPMIOInfo->stErrInfo ), s_ucTolThr );

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
* 输入参数: ucIOIndex：I/O模块的索引值；
*          bInputModule：输入模块标志位；
*          pucResp：应答帧首地址；
*          usRespLen：应答帧长度；
*          pucData：存放输入数据的首地址；
*
* 输出参数: pusDataLen：返回输入数据的长度
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIODecodeResp( uint8_t ucIOIndex, bool_t bInputModule, uint8_t pucResp[], uint16_t usRespLen,
                       uint8_t pucData[], uint8_t *pucDataLen )
{
    bool_t bResult = false;
    uint8_t ucIOID = 0U;          /* IO Module ID */
    uint8_t ucDataBuffLen = 0U;   /* Data Buffer Length */
    uint32_t uiSHeadLen = 0U;     /* Safety Head Length */
    uint32_t uiSFraLen = 0U;      /* Safety Frame Length */
    PMIOCommInfo_t *pstPMIOInfo = NULL;
    PMIORespHeadInfo_t stHead;

    if(( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT)) &&
       ( pucResp != NULL ) &&
       ( usRespLen > 0U ) &&
       ( pucData != NULL ) &&
       ( pucDataLen != NULL ))
    {
        ucDataBuffLen = *pucDataLen;  /* Store Data Buffer Length */
        *pucDataLen = 0U;             /* Reset */
        pstPMIOInfo = &s_stPMIOCommInfo[ucIOIndex];

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
                s_emLocalPMID = SysGetPMID();
                ucIOID = ucIOIndex + (uint8_t)(MIN_IO_MODULE_ID);  /* IO Module ID */

                /* Decode */
                if( DecodeIOResp((uint16_t)s_emLocalPMID, (uint16_t)ucIOID, pstPMIOInfo, pucResp, \
                                 uiSFraLen, s_ucTolThr ))
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
                }

                /* System Resource */
                SysSetPMIOTolFlag( ucIOIndex, pstPMIOInfo->stErrInfo.bTolFlag );
                SysSetPMIOErrFlag( ucIOIndex, pstPMIOInfo->stErrInfo.bErrFlag );
                SysSetPMIOFailureFlag( ucIOIndex, pstPMIOInfo->bIOModFailure );
                SysSetPMIOFVActivedFlag( ucIOIndex, pstPMIOInfo->bFVActived );
                SysSetPMIONewIParaFlag( ucIOIndex, pstPMIOInfo->bNewIPara );

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
*          usIOID: IO ID；
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

    if(( usPMID < (uint16_t)PM_NUM) &&
       ( usIOID <= (uint16_t)(MAX_IO_MODULE_ID)) &&
       ( pstPMIOInfo != NULL ) &&
       ( pucSResp != NULL ) &&
       ( uiSRespLen > 0U ))
    {
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
            }
        }

        if( bValid )
        {
            /* Calculate CRC & Check */
            uiCRC = SysCrc32Cal( 0U, &pucSResp[uiHeadIndex], uiCRCIndex );

            /* Compare */
            if( !CompareData( &pucSResp[uiCRCIndex], (uint8_t const*)(&uiCRC), (uint32_t)(PMIOSAFETY_CRC_LEN)))
            {
                bValid = false;
                ucCommStatus |= (uint8_t)(BIT_COMM_CRC);
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
            }
            else
            {
                /* Sequence */
                if( stHead.usSeq == (pstPMIOInfo->usSendSeqNum - 1U))
                {
                    bValid = true;
                    ucCommStatus = 0U;
                }
                else
                {
                    bValid = false;
                    ucCommStatus |= (uint8_t)(BIT_COMM_SEQ);  /* Sequence Error */
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

