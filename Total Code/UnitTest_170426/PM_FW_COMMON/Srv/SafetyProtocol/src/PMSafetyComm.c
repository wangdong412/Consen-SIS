/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*
*  项目名称    : 安全控制系统
*
*  模块名称    : PM间安全通讯
*
*  文件名称    : PMSafetyComm.c
*
*  功能简介    : PM间安全通讯相关处理
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
#include "../inc/PMSafetyComm.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static uint8_t s_ucTolCnt;                      /* 用户配置的可容忍次数 */
static uint8_t s_ucTolThr;                      /* 容忍门限值 */
static PMRecvInfo_t s_stPMRecvInfo[NUM_OF_PM];  /* 本系PM作为接收方时，维护的接收信息：索引号为发送方PMID */
static PMSendInfo_t s_stPMSendInfo[NUM_OF_PM];  /* 本系PM作为发送方时，维护的发送信息：索引号为接收方PMID */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void ClearPMRecvInfo( PMRecvInfo_t *pstPMRecvInfo );
static void ClearPMSendInfo( PMSendInfo_t *pstPMSendInfo );
static bool_t DecodePMSafetyFra( uint8_t const pucSFra[], uint32_t uiSFraLen,
                                 PMController_t emSrcPMID, PMController_t emDstPMID,
                                 PMRecvInfo_t *pstRecvInfo, uint8_t ucTolThr );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: PMCommInit
*
* 功能描述: PM间安全通讯初始化函数
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

void PMCommInit( void )
{
    uint8_t ucIndex = 0U;

    UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

    for( ucIndex = 0U; ucIndex < NUM_OF_PM; ucIndex++ )
    {
        /* PM Receive Info. */
        ClearPMRecvInfo( &s_stPMRecvInfo[ucIndex]);

        /* PM Send Info. */
        ClearPMSendInfo( &s_stPMSendInfo[ucIndex]);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: PMHandleRecvTO
*
* 功能描述: 接收超时错误处理
*
* 输入参数: emSendPMID：发送方PMID
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMHandleRecvTO( PMController_t emSendPMID )
{
    bool_t bResult = false;
    PMRecvInfo_t *pstRecvInfo = NULL;

    if( emSendPMID < NUM_OF_PM )
    {
        bResult = true;
        pstRecvInfo = &s_stPMRecvInfo[emSendPMID];

        UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );
        HandleSafetyCommError( &( pstRecvInfo->stErrInfo ), s_ucTolThr );

        /* System Resource */
        SysSetPMRecvTolFlag( emSendPMID, pstRecvInfo->stErrInfo.bTolFlag );
        SysSetPMRecvErrFlag( emSendPMID, pstRecvInfo->stErrInfo.bErrFlag );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMDecodeFra
*
* 功能描述: 解析数据帧
*
* 输入参数: emSendPMID：发送方PMID；
*          pucFra：数据帧首地址；
*          usFraLen：数据帧长度；
*          pucData：存放数据的首地址；
*
* 输出参数: pusDataLen：用于返回数据长度。
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMDecodeFra( PMController_t emSendPMID, uint8_t const pucFra[], uint16_t usFraLen,
                    uint8_t pucData[], uint16_t *pusDataLen )
{
    bool_t bResult = false;
    uint16_t usDataBuffLen = 0U;
    uint32_t uiHeadLen = 0U;
    uint32_t uiSFraLen = 0U;  /* Safety Frame Length */
    PMRecvInfo_t *pstRecvInfo = NULL;
    PMSafetyHeadInfo_t stHead;
    PMController_t emLocalPMID = PMA;  /* 本PM模块的编号 */

    if(( emSendPMID < NUM_OF_PM ) &&
       ( pucFra != NULL ) &&
       ( pucData != NULL ) &&
       ( pusDataLen != NULL ) &&
       ( usFraLen > 0U ))
    {
        usDataBuffLen = *pusDataLen;  /* Store Data Buffer Length */
        *pusDataLen = 0U;             /* Reset */
        pstRecvInfo = &s_stPMRecvInfo[emSendPMID];

        /* Decode */
        uiHeadLen = sizeof( PMSafetyHeadInfo_t );
        if( usFraLen > (uint16_t)uiHeadLen )
        {
            emLocalPMID = SysGetLocalPMID();

            /* Head */
            memcpy( &stHead, pucFra, uiHeadLen );

            /* Safety Frame Length */
            uiSFraLen = uiHeadLen + stHead.uiSDataLen + (uint32_t)(PMSAFETY_CRC_LEN);

            /* Decode */
            if( usFraLen >= (uint16_t)uiSFraLen )
            {
                /* Update Tolerance Info. */
                UpdateTolInfo( &s_ucTolCnt, &s_ucTolThr );

                /* Decode Safety Frame */
                if( DecodePMSafetyFra( pucFra, uiSFraLen, emSendPMID, emLocalPMID, pstRecvInfo, s_ucTolThr ))
                {
                    /* Copy Data */
                    if( usDataBuffLen >= (uint16_t)stHead.uiSDataLen )
                    {
                        bResult = true;
                        memcpy( pucData, &pucFra[uiHeadLen], stHead.uiSDataLen );
                        *pusDataLen = (uint16_t)stHead.uiSDataLen;
                    }
                }

                /* System Resource */
                SysSetPMRecvTolFlag( emSendPMID, pstRecvInfo->stErrInfo.bTolFlag );
                SysSetPMRecvErrFlag( emSendPMID, pstRecvInfo->stErrInfo.bErrFlag );
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMHandleSendTO
*
* 功能描述: 发送超时错误处理
*
* 输入参数: emRecvPMID：接收方PMID；
*          bTimeout：发送超时标志。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMHandleSendTO( PMController_t emRecvPMID, bool_t bTimeout )
{
    bool_t bResult = false;

    if( emRecvPMID < NUM_OF_PM )
    {
        bResult = true;

        /* Set Timeout Flag */
        SysSetPMSendTimeoutFlag( emRecvPMID, bTimeout );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: PMMakeFra
*
* 功能描述: 构造将发送给指定接收方PM的数据帧
*
* 输入参数: emRecvPMID：接收方PMID；
*          pucData：数据首地址；
*          usDataLen：数据长度；
*          pucFra：存放数据帧的首地址；
*
* 输出参数: pusFraLen：用于返回数据帧的长度。
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMMakeFra( PMController_t emRecvPMID, uint8_t const pucData[], uint16_t usDataLen,
                  uint8_t pucFra[], uint16_t *pusFraLen )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usFraBuffLen = 0U; /* Frame Buffer Length */
    uint16_t usSFraLen = 0U;    /* Safety Frame Length */
    uint16_t usHeadLen = 0U;    /* Header Length */
    uint32_t uiCRC = 0U;
    PMSafetyHeadInfo_t stHead;
    PMSendInfo_t *pstPMSendInfo = NULL;
    PMController_t emLocalPMID = PMA;  /* 本PM模块的编号 */

    if(( emRecvPMID < NUM_OF_PM ) &&
       ( pucData != NULL ) &&
       ( pucFra != NULL ) &&
       ( pusFraLen != NULL ))
    {
        usFraBuffLen = *pusFraLen;  /* Store Length */
        *pusFraLen = 0U;            /* Reset Output Length */
        pstPMSendInfo = &s_stPMSendInfo[emRecvPMID];

        /* Calculate Safety Frame Length */
        usHeadLen = (uint16_t)sizeof( PMSafetyHeadInfo_t );
        usSFraLen = usHeadLen + usDataLen + (uint16_t)(PMSAFETY_CRC_LEN);

        /* Check Buffer Length */
        if( usFraBuffLen >= usSFraLen )
        {
            emLocalPMID = SysGetLocalPMID();

            /* Make Response Head */
            usIndex = 0U;
            stHead.ucSrcPMID = (uint8_t)emLocalPMID;
            stHead.ucDstPMID = (uint8_t)emRecvPMID;
            stHead.usSeq = pstPMSendInfo->usSendSeqNum;
            stHead.uiSDataLen = (uint32_t)usDataLen;
            memcpy( &pucFra[usIndex], &stHead, (uint32_t)usHeadLen );

            /* Safety Data */
            usIndex += usHeadLen;
            memcpy( &pucFra[usIndex], pucData, (uint32_t)usDataLen );

            /* CRC */
            usIndex += usDataLen;
            uiCRC = SysCrc32Cal( 0U, pucFra, (uint32_t)usIndex );
            memcpy( &pucFra[usIndex], &uiCRC, sizeof(uint32_t));

            /* Set Output Length */
            *pusFraLen = usSFraLen;
            bResult = true;

            /* Add Send Sequence */
            pstPMSendInfo->usSendSeqNum++;
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
* 函数名称: ClearPMRecvInfo
*
* 功能描述: 本PM作为接收方：清除本地保存的相应PM的信息
*
* 输入参数: pstPMRecvInfo：待清除的信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearPMRecvInfo( PMRecvInfo_t *pstPMRecvInfo )
{
    if( pstPMRecvInfo != NULL )
    {
        memset( pstPMRecvInfo, 0, sizeof(PMRecvInfo_t));
        pstPMRecvInfo->stErrInfo.bTolFlag = false;
        pstPMRecvInfo->stErrInfo.bErrFlag = false;
        pstPMRecvInfo->usRecvSeqNum = (uint16_t)(INIT_RECV_SEQ);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearPMSendInfo
*
* 功能描述: 本PM作为发送方：清除本地保存的相应PM的信息
*
* 输入参数: pstPMSendInfo：待清除的信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearPMSendInfo( PMSendInfo_t *pstPMSendInfo )
{
    if( pstPMSendInfo != NULL )
    {
        memset( pstPMSendInfo, 0, sizeof(PMSendInfo_t));
        pstPMSendInfo->usSendSeqNum = 0U;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodePMSafetyFra
*
* 功能描述: 解析PM发送的安全帧
*
* 输入参数: pucFra：PM发送的安全帧首地址；
*          uiSFraLen：安全帧的长度；
*          emSrcPMID：源PMID;
*          emDstPMID：目标PMID;
*          pstRecvInfo：接收信息；
*          ucTolThr：容忍门限值。
*
* 输出参数: 无
*
* 返 回 值: 安全帧是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t DecodePMSafetyFra( uint8_t const pucSFra[], uint32_t uiSFraLen,
                                 PMController_t emSrcPMID, PMController_t emDstPMID,
                                 PMRecvInfo_t *pstRecvInfo, uint8_t ucTolThr )
{
    bool_t bValid = false;
    PMSafetyHeadInfo_t stHead;
    uint32_t uiSFraHeadLen = 0U;
    uint32_t uiCRCIndex = 0U;     /* CRC Index */
    uint32_t uiCRC = 0U;          /* CRC Calculate Result */

    if(( pucSFra != NULL ) &&
       ( uiSFraLen > 0U ) &&
       ( emSrcPMID < NUM_OF_PM ) &&
       ( emDstPMID < NUM_OF_PM ) &&
       ( pstRecvInfo != NULL ))
    {
        bValid = true;

        /* Check Communication Error */
        /* Head */
        uiSFraHeadLen = sizeof( PMSafetyHeadInfo_t );
        memcpy( &stHead, pucSFra, uiSFraHeadLen );
        uiCRCIndex = uiSFraHeadLen + (uint32_t)stHead.uiSDataLen;

        /* Calculate CRC & Check */
        uiCRC = SysCrc32Cal( 0U, pucSFra, uiCRCIndex );
        if( !CompareData( &pucSFra[uiCRCIndex], (uint8_t const*)(&uiCRC), (uint32_t)(PMSAFETY_CRC_LEN)))
        {
           bValid = false;
        }

        if( bValid )
        {
            /* Check Head */
            /* Source/Destination Address */
            if(( stHead.ucSrcPMID != (uint8_t)emSrcPMID ) ||
               ( stHead.ucDstPMID != (uint8_t)emDstPMID ))
            {
                bValid = false;
            }
            else
            {
                /* Sequence */
                if( stHead.usSeq != (uint16_t)(pstRecvInfo->usRecvSeqNum + 1U))
                {
                    bValid = false;
                }
                else
                {
                    bValid = true;
                }

                pstRecvInfo->usRecvSeqNum = stHead.usSeq;
            }
        }/* end of if - Valid */

        if( bValid )
        {
            HandleSafetyCommRecover( &( pstRecvInfo->stErrInfo ));
        }
        else
        {
            HandleSafetyCommError( &( pstRecvInfo->stErrInfo ), ucTolThr );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

