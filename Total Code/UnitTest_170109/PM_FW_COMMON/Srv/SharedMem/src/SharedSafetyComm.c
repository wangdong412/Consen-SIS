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
#include "Srv/IPBUS/inc/IPBUS.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/* P2P Safety Communication */
static SysP2PRecvReqCommInfo_t s_stSysP2PRecvInfo[MAX_STATION_CNT];
static SysP2PSendReqCommInfo_t s_stSysP2PSendInfo[MAX_STATION_CNT];

/* PM Safety Communication */
static SysPMRecvInfo_t s_stSysPMRecvInfo[NUM_OF_PM];  /* 接收信息：索引号为发送方PMID */
static SysPMSendInfo_t s_stSysPMSendInfo[NUM_OF_PM];  /* 发送信息：索引号为接收方PMID */

/**** PM IO Safety ****/
static bool_t s_bAutoResume;                                        /* 自动恢复标志位，false-需用户确认；true-自动恢复 */
static bool_t s_bUserConfirm;                                       /* 用户确认标志位，false-未确认；true-已确认 */
static SysPMIOCommInfo_t s_stSysPMIOCommInfo[IPBUS_MODULE_MAX_NUM]; /* 每个I/O模块对应其中一个数组成员  */

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
    for( ucIndex = 0U; ucIndex < NUM_OF_PM; ucIndex++ )
    {
        memset( &s_stSysPMRecvInfo[ucIndex], 0, sizeof(SysPMRecvInfo_t));
        s_stSysPMRecvInfo[ucIndex].bTolFlag = false;
        s_stSysPMRecvInfo[ucIndex].bErrFlag = false;

        memset( &s_stSysPMSendInfo[ucIndex], 0, sizeof(SysPMSendInfo_t));
        s_stSysPMSendInfo[ucIndex].bSendTimeout = false;
    }

    /* PM IO Safety */
    for( ucIndex = 0U; ucIndex < (uint8_t)(IPBUS_MODULE_MAX_NUM); ucIndex++ )
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

void SysSetPMRecvTolFlag( PMController_t emSrcPMID, bool_t bTolFlag )
{
    if( emSrcPMID < NUM_OF_PM )
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

bool_t SysGetPMRecvTolFlag( PMController_t emSrcPMID )
{
    bool_t bTolFlag = false;

    if( emSrcPMID < NUM_OF_PM )
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

void SysSetPMRecvErrFlag( PMController_t emSrcPMID, bool_t bErrFlag )
{
    if( emSrcPMID < NUM_OF_PM )
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

bool_t SysGetPMRecvErrFlag( PMController_t emSrcPMID )
{
    bool_t bErrFlag = false;

    if( emSrcPMID < NUM_OF_PM )
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

bool_t SysGetPMRecvCommInfo( PMController_t emSrcPMID, SysPMRecvInfo_t *pstRecvInfo )
{
    bool_t bResult = false;

    if((emSrcPMID < NUM_OF_PM) && (pstRecvInfo != NULL))
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

void SysSetPMSendTimeoutFlag( PMController_t emDstPMID, bool_t bSendTimeout )
{
    if( emDstPMID < NUM_OF_PM )
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

bool_t SysGetPMSendTimeoutFlag( PMController_t emDstPMID )
{
    bool_t bSendTimeout = false;

    if( emDstPMID < NUM_OF_PM )
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

bool_t SysGetPMSendCommInfo( PMController_t emDstPMID, SysPMSendInfo_t *pstSendInfo )
{
    bool_t bResult = false;

    if((emDstPMID < NUM_OF_PM) && (pstSendInfo != NULL))
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
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bTolFlag：容忍标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOTolFlag( uint8_t ucIOID, bool_t bTolFlag )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bTolFlag = bTolFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOTolFlag
*
* 功能描述: 获取PM IO 容忍标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*
* 输出参数: 无
*
* 返 回 值  : 容忍标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOTolFlag( uint8_t ucIOID )
{
    bool_t bTolFlag = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bTolFlag = s_stSysPMIOCommInfo[ucIOID].bTolFlag;
    }

    return bTolFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOErrFlag
*
* 功能描述: 设置PM IO 错误标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bTolFlag：错误标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOErrFlag( uint8_t ucIOID, bool_t bErrFlag )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bErrFlag = bErrFlag;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOErrFlag
*
* 功能描述: 获取PM IO 错误标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239
*
* 输出参数: 无
*
* 返 回 值  : 错误标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOErrFlag( uint8_t ucIOID )
{
    bool_t bErrFlag = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bErrFlag = s_stSysPMIOCommInfo[ucIOID].bErrFlag;
    }

    return bErrFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOSendTimeoutFlag
*
* 功能描述: 设置PM IO 发送超时标志位
*
* 输入参数: ucIOID：I/O模块ID - 4-239；
*          bTimeout：发送超时标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOSendTimeoutFlag( uint8_t ucIOID, bool_t bTimeout )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bSendTimeout = bTimeout;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOSendTimeoutFlag
*
* 功能描述: 获取PM IO 发送超时标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239
*
* 输出参数: 无
*
* 返 回 值  : 发送超时标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOSendTimeoutFlag( uint8_t ucIOID )
{
    bool_t bSendTimeout = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bSendTimeout = s_stSysPMIOCommInfo[ucIOID].bSendTimeout;
    }

    return bSendTimeout;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOFailureFlag
*
* 功能描述: 设置PM IO： IO模块存在故障标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bIOFailure：IO模块存在故障标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOFailureFlag( uint8_t ucIOID, bool_t bIOFailure )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bFailure = bIOFailure;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOFailureFlag
*
* 功能描述: 获取IO模块存在故障标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239
*
* 输出参数: 无
*
* 返 回 值  : IO模块存在故障标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOFailureFlag( uint8_t ucIOID )
{
    bool_t bFailure = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bFailure = s_stSysPMIOCommInfo[ucIOID].bFailure;
    }

    return bFailure;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIOFVActivedFlag
*
* 功能描述: 设置PM IO： IO模块使用故障安全值标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bFVActived：IO模块使用故障安全值标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIOFVActivedFlag( uint8_t ucIOID, bool_t bFVActived )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bFVActived = bFVActived;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOFVActivedFlag
*
* 功能描述: 获取IO模块使用故障安全值标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239
*
* 输出参数: 无
*
* 返 回 值  : IO模块使用故障安全值标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOFVActivedFlag( uint8_t ucIOID )
{
    bool_t bFVActived = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bFVActived = s_stSysPMIOCommInfo[ucIOID].bFVActived;
    }

    return bFVActived;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMIONewIParaFlag
*
* 功能描述: 设置PM IO： IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*          bNewIPara：模块采用主站下发的新参数标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SysSetPMIONewIParaFlag( uint8_t ucIOID, bool_t bNewIPara )
{
    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        s_stSysPMIOCommInfo[ucIOID].bNewIPara = bNewIPara;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIONewIParaFlag
*
* 功能描述: 获取IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*
* 输出参数: 无
*
* 返 回 值  : IO模块采用主站下发的新参数标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIONewIParaFlag( uint8_t ucIOID )
{
    bool_t bNewIPara = false;

    if((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) &&
       (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO)))
    {
        bNewIPara = s_stSysPMIOCommInfo[ucIOID].bNewIPara;
    }

    return bNewIPara;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIOCommInfo
*
* 功能描述: 获取IO模块采用主站下发的新参数标志位
*
* 输入参数: ucIOID：I/O模块ID, 4-239；
*
* 输出参数: pstCommInfo：用于返回IO模块对应的通信信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMIOCommInfo( uint8_t ucIOID, SysPMIOCommInfo_t *pstCommInfo )
{
    bool_t bResult = false;

    if(((ucIOID >= (uint8_t)(IPBUS_MIN_SLAVE_NO)) && (ucIOID <= (uint8_t)(IPBUS_MAX_SLAVE_NO))) &&
       (pstCommInfo != NULL))
    {
        bResult = true;
        memcpy( pstCommInfo, &s_stSysPMIOCommInfo[ucIOID], sizeof(SysPMIOCommInfo_t));
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
