/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : NTP校时模块
*
*  文件名称    : NTP.h
*
*  功能简介    : NTP校时
*
*  作者        : 王东
*
*  创建日期    : 2016-12-22
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>    /* File control definitions */
#include <errno.h>
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../inc/NTP.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                      /* 本CM是否有配置 */
static TimeSyncCfgInfo_t s_stTimeSyncCfgInfo;   /* 时间同步配置参数信息 */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );

/* Initialize */
static void InitLocalConfigInfo( void );

/* Clear */
static void ClearLocalInfo( void );

/* Configure NTP Timing */
static void ConfigNTPTiming( void );
static void SetRemoteNTPSrvIPAddr( uint32_t uiIPAddr );
static void StartLocalNTPServer( void );
static void StopLocalNTPServer( void );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: NTPInit
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

void NTPInit( void )
{
    DEBUG_NTP( "NTPInit\n" );

    s_bCMHasCfg = false;

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: NTPCycle
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

void NTPCycle( void )
{
    static bool_t s_bClrDone = false;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Handle Local Configuration */
    if( s_bCMHasCfg )
    {
        s_bClrDone = false;

        /* Configuration */
        HandleConfiguration();
    }
    else
    {
        if( !s_bClrDone )
        {
            ClearLocalInfo();
            s_bClrDone = true;
        }
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
    /* Time Sync */
    InitTimeSyncCfgInfo( &s_stTimeSyncCfgInfo );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 清除本地配置信息，重新设置NTP相关校时
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存
***********************************************************************************************************************
*/

static void ClearLocalInfo( void )
{
    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* Configure NTP Timing */
    ConfigNTPTiming();

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
    bool_t bSwitch = false;

    /* Time Sync */
    bSwitch = HandleTimeSyncConfig( &s_stTimeSyncCfgInfo );
    if( bSwitch )
    {
        DEBUG_NTP( "Switch TimeSync Cfg\n" );

        /* Configure NTP Timing */
        ConfigNTPTiming();
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ConfigNTPTiming
*
* 功能描述: 处理NTP校时：时钟更新源为NTP时，接收NTP校时包并处理
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

static void ConfigNTPTiming( void )
{
    NTPTCPConfigInfo_t const *pstTSyncCfg = NULL;

    pstTSyncCfg = GetCurTSyncCfg( &s_stTimeSyncCfgInfo );
    if( pstTSyncCfg != NULL )
    {
        if((uint8_t)(TSRC_NTP) == pstTSyncCfg->ucTimingSrc )
        {
            /* Set IP address of remote NTP server */
            SetRemoteNTPSrvIPAddr( pstTSyncCfg->uiMasterIPAddr );
        }
        else
        {
            /* Reset IP address of remote NTP server: 0.0.0.0 */
            SetRemoteNTPSrvIPAddr( 0U );
        }

        if( pstTSyncCfg->ucIsMaster )
        {
            /* Start Local NTP Server */
            StartLocalNTPServer();
        }
        else
        {
            /* Stop Local NTP Server */
            StopLocalNTPServer();
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetRemoteNTPSrvIPAddr
*
* 功能描述: 设置远程NTP校时服务器的IP地址
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

static void SetRemoteNTPSrvIPAddr( uint32_t uiIPAddr )
{
    int32_t iFD = -1;  /* File Descriptor */
    char_t ucPathName[256] = "/home/ntp/sntpc.conf";
    char_t *pcIPAddr = NULL;
    struct in_addr stIPAddr;

    /* Convert IP Address */
    stIPAddr.s_addr = uiIPAddr;
    pcIPAddr = inet_ntoa( stIPAddr );  /* Note: non-reentrant function */
    DEBUG_NTP( "IP: %s\n", pcIPAddr );

    /* Open cfg File: R/W; Flush after write; Size 0 */
    /* Default: file offset is set to the beginning */
    iFD = open((const pchar_t)ucPathName, O_RDWR|O_SYNC|O_TRUNC );
    if( iFD != -1 )
    {
        /* Write */
        if( -1 == write( iFD, pcIPAddr, strlen(pcIPAddr)+1 ))
        {
            DEBUG_NTP( "write file: %s failed, err-%d\n", ucPathName, GetErrorNo());
        }

        /* Close */
        if( -1 == close( iFD ))
        {
            DEBUG_NTP( "close file: %s failed, err-%d\n", ucPathName, GetErrorNo());
        }
    }
    else
    {
        DEBUG_NTP( "open file: %s failed, err-%d\n", ucPathName, errno );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: StartLocalNTPServer
*
* 功能描述: 启动本地NTP校时服务器
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

static void StartLocalNTPServer( void )
{
    char_t cCmdLine[100] = { 0 };
    char_t cPre[] = "/home/ntp/ntpd -4d -P 110 -g -n -L -I ";  /* Start command */
    char_t cNetDev[2][6] = { "eth1 &","eth2 &"};  /* Select listen network for the local timing server */
    int32_t iRet = 0;

    strncat( cCmdLine, cPre, strlen(cPre));
    strncat( cCmdLine, (char*)&cNetDev[1], 6 );
    DEBUG_NTP( "Cmdline: %s\n", cCmdLine );

    /* Start local NTP timing server */
    iRet = system( cCmdLine );
    if(iRet != 0)
    {
        DEBUG_NTP( "Start ntpd failed: %d\n", errno );
    }
    else
    {
        DEBUG_NTP( "Start ntpd ok\n" );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: StopLocalNTPServer
*
* 功能描述: 停止本地NTP校时服务器
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

static void StopLocalNTPServer( void )
{
    int32_t iRet = 0;

    /* Stop local NTP timing server */
    iRet = system( "pkill ntpd" );
    if( iRet != 0 )
    {
        DEBUG_NTP( "Stop ntpd failed: %d\n", errno );
    }
    else
    {
        DEBUG_NTP( "Stop ntpd ok\n" );
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

