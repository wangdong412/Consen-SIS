/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 共享安全通讯
*
*  文件名称    : SharedSafetyComm.c
*
*  功能简介    : core0与core1间的共享安全通讯状态
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-27
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

#include <Srv/Scheduler/include/string.h>
#include "../inc/SharedSafetyComm.h"


/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static SysPMID_t s_emLocalPMID;
static uint8_t s_ucStationID;
static uint8_t s_ucTolCnt;

/* P2P Safety Communication */
static SysP2PRecvReqCommInfo_t s_stSysP2PRecvInfo[MAX_STATION_CNT];
static SysP2PSendReqCommInfo_t s_stSysP2PSendInfo[MAX_STATION_CNT];

/* PM Safety Communication */
static SysPMRecvInfo_t s_stSysPMRecvInfo[PM_NUM];  /* 接收信息：索引号为发送方PMID */
static SysPMSendInfo_t s_stSysPMSendInfo[PM_NUM];  /* 发送信息：索引号为接收方PMID */

/**** PM IO Safety ****/
static bool_t s_bAutoResume;                                      /* 自动恢复标志位，false-需用户确认；true-自动恢复 */
static bool_t s_bUserConfirm;                                     /* 用户确认标志位，false-未确认；true-已确认 */
static SysPMIOCommInfo_t s_stSysPMIOCommInfo[MAX_IO_MODULE_CNT];  /* 每个I/O模块对应其中一个数组成员  */

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
* 函数名称: SharedSafetyCommInit
*
* 功能描述: 初始化
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

void SharedSafetyCommInit( void )
{
    uint8_t ucIndex = 0U;
    s_emLocalPMID = PM_INVALID;
    s_ucStationID = (uint8_t)(MIN_STA_ID);
    s_ucTolCnt = 0U;
    s_bAutoResume = false;
    s_bUserConfirm = false;

    /* P2P Safety */
    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_STATION_CNT); ucIndex++ )
    {
        memset( &s_stSysP2PRecvInfo[ucIndex], 0, sizeof(SysP2PRecvReqCommInfo_t));
        s_stSysP2PRecvInfo[ucIndex].bTolFlag = false;
        s_stSysP2PRecvInfo[ucIndex].bErrFlag = false;

        memset( &s_stSysP2PSendInfo[ucIndex], 0, sizeof(SysP2PSendReqCommInfo_t));
        s_stSysP2PSendInfo[ucIndex].bTolFlag = false;
        s_stSysP2PSendInfo[ucIndex].bErrFlag = false;
        s_stSysP2PSendInfo[ucIndex].bSendTimeout = false;
    }

    /* PM Safety */
    for( ucIndex = 0U; ucIndex < PM_NUM; ucIndex++ )
    {
        memset( &s_stSysPMRecvInfo[ucIndex], 0, sizeof(SysPMRecvInfo_t));
        s_stSysPMRecvInfo[ucIndex].bTolFlag = false;
        s_stSysPMRecvInfo[ucIndex].bErrFlag = false;

        memset( &s_stSysPMSendInfo[ucIndex], 0, sizeof(SysPMSendInfo_t));
        s_stSysPMSendInfo[ucIndex].bSendTimeout = false;
    }

    /* PM IO Safety */
    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_IO_MODULE_CNT); ucIndex++ )
    {
        memset( &s_stSysPMIOCommInfo[ucIndex], 0, sizeof(SysPMIOCommInfo_t));
        s_stSysPMIOCommInfo[ucIndex].bTolFlag = false;
        s_stSysPMIOCommInfo[ucIndex].bErrFlag = false;
        s_stSysPMIOCommInfo[ucIndex].bSendTimeout = false;
        s_stSysPMIOCommInfo[ucIndex].bFailure = false;
        s_stSysPMIOCommInfo[ucIndex].bFVActived = false;
        s_stSysPMIOCommInfo[ucIndex].bNewIPara = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMID
*
* 功能描述: 设置本系PMID
*
* 输入参数: emPMID：PM模块ID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMID( SysPMID_t emPMID )
{
    s_emLocalPMID = emPMID;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMID
*
* 功能描述: 获取本系PMID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本系PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysPMID_t SysGetPMID( void )
{
    return s_emLocalPMID;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetStationID
*
* 功能描述: 设置本控制站ID
*
* 输入参数: ucStationID：控制站ID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetStationID( uint8_t ucStationID )
{
    s_ucStationID = ucStationID;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetStationID
*
* 功能描述: 获取本控制站ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本控制站ID
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t SysGetStationID( void )
{
    return s_ucStationID;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetTolCnt
*
* 功能描述: 设置容忍次数
*
* 输入参数: ucTolCnt：容忍次数
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetTolCnt( uint8_t ucTolCnt )
{
    s_ucTolCnt = ucTolCnt;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetTolCnt
*
* 功能描述: 获取容忍次数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 容忍次数
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t SysGetTolCnt( void )
{
    return s_ucTolCnt;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PRecvTolFlag
*
* 功能描述: 设置P2P接收容忍标志位
*
* 输入参数: ucSrcStaIndex：源（数据发送方）控制站索引值；
*          bTolFlag：容忍标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetP2PRecvTolFlag( uint8_t ucSrcStaIndex, bool_t bTolFlag )
{
    if( ucSrcStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        s_stSysP2PRecvInfo[ucSrcStaIndex].bTolFlag = bTolFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PRecvTolFlag
*
* 功能描述: 获取P2P接收容忍标志位
*
* 输入参数: ucSrcStaIndex：源（数据发送方）控制站索引值
*
* 输出参数: 无
*
* 返 回 值  : 容忍标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PRecvTolFlag( uint8_t ucSrcStaIndex )
{
    bool_t bTolFlag = false;

    if( ucSrcStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        bTolFlag = s_stSysP2PRecvInfo[ucSrcStaIndex].bTolFlag;
    }

    return bTolFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PRecvErrFlag
*
* 功能描述: 设置P2P接收错误标志位
*
* 输入参数: ucSrcStaIndex：源（数据发送方）控制站索引值；
*          bErrFlag：错误标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetP2PRecvErrFlag( uint8_t ucSrcStaIndex, bool_t bErrFlag )
{
    if( ucSrcStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        s_stSysP2PRecvInfo[ucSrcStaIndex].bErrFlag = bErrFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PRecvErrFlag
*
* 功能描述: 获取P2P接收错误标志位
*
* 输入参数: ucSrcStaIndex：源（数据发送方）控制站索引值
*
* 输出参数: 无
*
* 返 回 值  : 错误标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PRecvErrFlag( uint8_t ucSrcStaIndex )
{
    bool_t bErrFlag = false;

    if( ucSrcStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        bErrFlag = s_stSysP2PRecvInfo[ucSrcStaIndex].bErrFlag;
    }

    return bErrFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PRecvReqCommInfo
*
* 功能描述: 获取源控制站对应的接收信息
*
* 输入参数: ucSrcStaIndex：源（数据发送方）控制站索引值
*
* 输出参数: pstRecvReqInfo：用于返回源控制站对应的接收信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PRecvReqCommInfo( uint8_t ucSrcStaIndex, SysP2PRecvReqCommInfo_t *pstRecvReqInfo )
{
    bool_t bResult = false;

    if((ucSrcStaIndex < (uint8_t)(MAX_STATION_CNT)) && (pstRecvReqInfo != NULL))
    {
        bResult = true;
        memcpy( pstRecvReqInfo, &s_stSysP2PRecvInfo[ucSrcStaIndex], sizeof(SysP2PRecvReqCommInfo_t));
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PSendTolFlag
*
* 功能描述: 设置P2P发送容忍标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值；
*          bTolFlag：容忍标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetP2PSendTolFlag( uint8_t ucDstStaIndex, bool_t bTolFlag )
{
    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        s_stSysP2PSendInfo[ucDstStaIndex].bTolFlag = bTolFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PSendTolFlag
*
* 功能描述: 获取P2P发送错误标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值
*
* 输出参数: 无
*
* 返 回 值  : 容忍标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PSendTolFlag( uint8_t ucDstStaIndex )
{
    bool_t bTolFlag = false;

    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        bTolFlag = s_stSysP2PSendInfo[ucDstStaIndex].bTolFlag;
    }

    return bTolFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PSendErrFlag
*
* 功能描述: 设置P2P发送错误标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值；
*          bErrFlag：错误标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetP2PSendErrFlag( uint8_t ucDstStaIndex, bool_t bErrFlag )
{
    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        s_stSysP2PSendInfo[ucDstStaIndex].bErrFlag = bErrFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PSendErrFlag
*
* 功能描述: 获取P2P发送错误标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值
*
* 输出参数: 无
*
* 返 回 值  : 错误标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PSendErrFlag( uint8_t ucDstStaIndex )
{
    bool_t bErrFlag = false;

    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        bErrFlag = s_stSysP2PSendInfo[ucDstStaIndex].bErrFlag;
    }

    return bErrFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PSendTimeoutFlag
*
* 功能描述: 设置P2P发送超时标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值；
*          bSendTimeout：超时标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetP2PSendTimeoutFlag( uint8_t ucDstStaIndex, bool_t bSendTimeout )
{
    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        s_stSysP2PSendInfo[ucDstStaIndex].bSendTimeout = bSendTimeout;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PSendTimeoutFlag
*
* 功能描述: 获取P2P发送超时标志位
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值
*
* 输出参数: 无
*
* 返 回 值  : 超时标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PSendTimeoutFlag( uint8_t ucDstStaIndex )
{
    bool_t bSendTimeout = false;

    if( ucDstStaIndex < (uint8_t)(MAX_STATION_CNT))
    {
        bSendTimeout = s_stSysP2PSendInfo[ucDstStaIndex].bSendTimeout;
    }

    return bSendTimeout;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PSendReqCommInfo
*
* 功能描述: 获取目的控制站对应的发送信息
*
* 输入参数: ucDstStaIndex：目的（数据接收方）控制站索引值
*
* 输出参数: pstSendReqInfo：用于返回目的控制站对应的发送信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetP2PSendReqCommInfo( uint8_t ucDstStaIndex, SysP2PSendReqCommInfo_t *pstSendReqInfo )
{
    bool_t bResult = false;

    if((ucDstStaIndex < (uint8_t)(MAX_STATION_CNT)) && (pstSendReqInfo != NULL))
    {
        bResult = true;
        memcpy( pstSendReqInfo, &s_stSysP2PSendInfo[ucDstStaIndex], sizeof(SysP2PSendReqCommInfo_t));
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMRecvTolFlag
*
* 功能描述: 设置PM接收容忍标志位
*
* 输入参数: emSrcPMID：源（数据发送方）PMID；
*          bTolFlag：容忍标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMRecvTolFlag( SysPMID_t emSrcPMID, bool_t bTolFlag )
{
    if( emSrcPMID < PM_NUM )
    {
        s_stSysPMRecvInfo[emSrcPMID].bTolFlag = bTolFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMRecvTolFlag
*
* 功能描述: 获取PM接收容忍标志位
*
* 输入参数: emSrcPMID：源（数据发送方）PMID；
*
* 输出参数: 无
*
* 返 回 值  : 容忍标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMRecvTolFlag( SysPMID_t emSrcPMID )
{
    bool_t bTolFlag = false;

    if( emSrcPMID < PM_NUM )
    {
        bTolFlag = s_stSysPMRecvInfo[emSrcPMID].bTolFlag;
    }

    return bTolFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMRecvErrFlag
*
* 功能描述: 设置PM接收错误标志位
*
* 输入参数: emSrcPMID：源（数据发送方）PMID；
*          bErrFlag：错误标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMRecvErrFlag( SysPMID_t emSrcPMID, bool_t bErrFlag )
{
    if( emSrcPMID < PM_NUM )
    {
        s_stSysPMRecvInfo[emSrcPMID].bErrFlag = bErrFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMRecvErrFlag
*
* 功能描述: 获取PM接收错误标志位
*
* 输入参数: emSrcPMID：源（数据发送方）PMID
*
* 输出参数: 无
*
* 返 回 值  : 错误标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMRecvErrFlag( SysPMID_t emSrcPMID )
{
    bool_t bErrFlag = false;

    if( emSrcPMID < PM_NUM )
    {
        bErrFlag = s_stSysPMRecvInfo[emSrcPMID].bErrFlag;
    }

    return bErrFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMRecvCommInfo
*
* 功能描述: 获取源PM模块对应的接收信息
*
* 输入参数: emSrcPMID：源（数据发送方）PMID
*
* 输出参数: pstRecvInfo：用于返回源PM对应的接收信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMRecvCommInfo( SysPMID_t emSrcPMID, SysPMRecvInfo_t *pstRecvInfo )
{
    bool_t bResult = false;

    if((emSrcPMID < PM_NUM) && (pstRecvInfo != NULL))
    {
        bResult = true;
        memcpy( pstRecvInfo, &s_stSysPMRecvInfo[emSrcPMID], sizeof(SysPMRecvInfo_t));
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMSendTimeoutFlag
*
* 功能描述: 设置PM发送超时标志位
*
* 输入参数: emDstPMID：目的（数据接收方）PMID；
*          bSendTimeout：超时标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMSendTimeoutFlag( SysPMID_t emDstPMID, bool_t bSendTimeout )
{
    if( emDstPMID < PM_NUM )
    {
        s_stSysPMSendInfo[emDstPMID].bSendTimeout = bSendTimeout;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMSendTimeoutFlag
*
* 功能描述: 获取PM发送超时标志位
*
* 输入参数: emDstPMID：目的（数据接收方）PMID
*
* 输出参数: 无
*
* 返 回 值  : 超时标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMSendTimeoutFlag( SysPMID_t emDstPMID )
{
    bool_t bSendTimeout = false;

    if( emDstPMID < PM_NUM )
    {
        bSendTimeout = s_stSysPMSendInfo[emDstPMID].bSendTimeout;
    }

    return bSendTimeout;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMSendCommInfo
*
* 功能描述: 获取目的PM模块对应的发送信息
*
* 输入参数: emDstPMID：目的（数据接收方）PMID
*
* 输出参数: pstSendInfo：用于返回目的PM对应的发送信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMSendCommInfo( SysPMID_t emDstPMID, SysPMSendInfo_t *pstSendInfo )
{
    bool_t bResult = false;

    if((emDstPMID < PM_NUM) && (pstSendInfo != NULL))
    {
        bResult = true;
        memcpy( pstSendInfo, &s_stSysPMSendInfo[emDstPMID], sizeof(SysPMSendInfo_t));
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetAutoResume
*
* 功能描述: 设置自动恢复标志位
*
* 输入参数: bAutoResume：自动恢复标志位；
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetAutoResume( bool_t bAutoResume )
{
    s_bAutoResume = bAutoResume;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetAutoResume
*
* 功能描述: 获取自动恢复标志位
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 自动恢复标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetAutoResume( void )
{
    return s_bAutoResume;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetUserConfirm
*
* 功能描述: 设置用户确认标志位
*
* 输入参数: bUserConfirm：用户确认标志位；
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetUserConfirm( bool_t bUserConfirm )
{
    s_bUserConfirm = bUserConfirm;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetUserConfirm
*
* 功能描述: 获取用户确认标志位
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 用户确认标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetUserConfirm( void )
{
    return s_bUserConfirm;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOTolFlag
*
* 功能描述: 设置PM IO 容忍标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bTolFlag：容忍标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOTolFlag( uint8_t ucIOIndex, bool_t bTolFlag )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bTolFlag = bTolFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOTolFlag
*
* 功能描述: 获取PM IO 容忍标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : 容忍标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOTolFlag( uint8_t ucIOIndex )
{
    bool_t bTolFlag = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bTolFlag = s_stSysPMIOCommInfo[ucIOIndex].bTolFlag;
    }

    return bTolFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOErrFlag
*
* 功能描述: 设置PM IO 错误标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bTolFlag：错误标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOErrFlag( uint8_t ucIOIndex, bool_t bErrFlag )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bErrFlag = bErrFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOErrFlag
*
* 功能描述: 获取PM IO 错误标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : 错误标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOErrFlag( uint8_t ucIOIndex )
{
    bool_t bErrFlag = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bErrFlag = s_stSysPMIOCommInfo[ucIOIndex].bErrFlag;
    }

    return bErrFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOSendTimeoutFlag
*
* 功能描述: 设置PM IO 发送超时标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bSendTimeout：发送超时标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOSendTimeoutFlag( uint8_t ucIOIndex, bool_t bSendTimeout )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bSendTimeout = bSendTimeout;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOSendTimeoutFlag
*
* 功能描述: 获取PM IO 发送超时标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : 发送超时标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOSendTimeoutFlag( uint8_t ucIOIndex )
{
    bool_t bSendTimeout = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bSendTimeout = s_stSysPMIOCommInfo[ucIOIndex].bSendTimeout;
    }

    return bSendTimeout;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOFailureFlag
*
* 功能描述: 设置PM IO： IO模块存在故障标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bIOFailure：IO模块存在故障标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOFailureFlag( uint8_t ucIOIndex, bool_t bIOFailure )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bFailure = bIOFailure;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOFailureFlag
*
* 功能描述: 获取IO模块存在故障标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : IO模块存在故障标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOFailureFlag( uint8_t ucIOIndex )
{
    bool_t bFailure = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bFailure = s_stSysPMIOCommInfo[ucIOIndex].bFailure;
    }

    return bFailure;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOFVActivedFlag
*
* 功能描述: 设置PM IO： IO模块使用故障安全值标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bFVActived：IO模块使用故障安全值标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOFVActivedFlag( uint8_t ucIOIndex, bool_t bFVActived )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bFVActived = bFVActived;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOFVActivedFlag
*
* 功能描述: 获取IO模块使用故障安全值标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : IO模块使用故障安全值标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOFVActivedFlag( uint8_t ucIOIndex )
{
    bool_t bFVActived = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bFVActived = s_stSysPMIOCommInfo[ucIOIndex].bFVActived;
    }

    return bFVActived;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIONewIParaFlag
*
* 功能描述: 设置PM IO： IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*          bNewIPara：模块采用主站下发的新参数标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIONewIParaFlag( uint8_t ucIOIndex, bool_t bNewIPara )
{
    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        s_stSysPMIOCommInfo[ucIOIndex].bNewIPara = bNewIPara;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIONewIParaFlag
*
* 功能描述: 获取IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: 无
*
* 返 回 值  : IO模块采用主站下发的新参数标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIONewIParaFlag( uint8_t ucIOIndex )
{
    bool_t bNewIPara = false;

    if( ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT))
    {
        bNewIPara = s_stSysPMIOCommInfo[ucIOIndex].bNewIPara;
    }

    return bNewIPara;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOCommInfo
*
* 功能描述: 获取IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOIndex：IO模块的索引号 - 0-235；
*
* 输出参数: pstCommInfo：用于返回IO模块对应的通信信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOCommInfo( uint8_t ucIOIndex, SysPMIOCommInfo_t *pstCommInfo )
{
    bool_t bResult = false;

    if((ucIOIndex < (uint8_t)(MAX_IO_MODULE_CNT)) && (pstCommInfo != NULL))
    {
        bResult = true;
        memcpy( pstCommInfo, &s_stSysPMIOCommInfo[ucIOIndex], sizeof(SysPMIOCommInfo_t));
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
                                 文件结束
***********************************************************************************************************************
*/
