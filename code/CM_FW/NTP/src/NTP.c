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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/syscall.h>
#include <fcntl.h>    /* File control definitions */
#include <unistd.h>
#include <errno.h>
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <byteswap.h>
#include <linux/fs.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../inc/NTP.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                      /* 本CM是否有配置 */
static TimeSyncCfgInfo_t s_stTimeSyncCfgInfo;   /* 时间同步配置参数信息 */

/* Command */
static int32_t s_iShmID;
static uint8_t *s_pucShmCmd;
SysCmdInfo_t s_stCmdInfo;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );

/* Initialize */
static void InitLocalConfigInfo( void );
static void InitCommand( void );

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

    /* Command Info. */
    InitCommand();

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

    /* System Command */
    if( s_stCmdInfo.ucCmdCnt > 0U )
    {
        if((int32_t)(ESC_FINISH_OK) == ExecSysCmd( &s_stCmdInfo, s_pucShmCmd, s_iShmID ))
        {
            DEBUG_NTP( "ExecSysCmd finished ok\n" );
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
* 函数名称: InitCommand
*
* 功能描述: 初始化命令
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

static void InitCommand( void )
{
    bool_t bInitRet = true;

    /* NetCmdInfo */
    memset( &s_stCmdInfo, 0, sizeof(SysCmdInfo_t));

    /* Shared memory */
    s_iShmID = -1;
    s_pucShmCmd = NULL;

    /* Shared Memory */
    /* Create if not exist; If exists, return fail; Not block; User/Group RW */
    s_iShmID = shmget( IPC_PRIVATE, (uint32_t)(SHM_CMD_SIZE), \
                       IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP );
    if( -1 == s_iShmID )
    {
        DEBUG_NTP( "InitCommand: shmget failed, errno %d\n", errno );
        bInitRet = false;
    }
    else
    {
        DEBUG_NTP( "InitCommand: ShmID %d\n", s_iShmID );
    }

    if( bInitRet )
    {
        /* Attach, Read/Write */
        s_pucShmCmd = (uint8_t *)shmat( s_iShmID, NULL, 0 );
        if((uint8_t *)-1 == s_pucShmCmd )
        {
            DEBUG_NTP( "InitCommand: shmat failed, errno %d\n", errno );
            s_pucShmCmd = NULL;
            bInitRet = false;
        }
    }

    if( bInitRet )
    {
        memset( s_pucShmCmd, 0, (uint32_t)(SHM_CMD_SIZE));
    }
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
        LogWrite( (uint32_t)(LOGID_NTP_CFG_SWITCH_TIME_SYNC));

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
            LogWrite( (uint32_t)(LOGID_NTP_E_WRITE_FILE));
        }

        /* Close */
        if( -1 == close( iFD ))
        {
            DEBUG_NTP( "close file: %s failed, err-%d\n", ucPathName, GetErrorNo());
            LogWrite( (uint32_t)(LOGID_NTP_E_CLOSE_FILE));
        }
    }
    else
    {
        DEBUG_NTP( "open file: %s failed, err-%d\n", ucPathName, errno );
        LogWrite( (uint32_t)(LOGID_NTP_E_OPEN_FILE));
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
    SysCmd_t stCmd;
    memset( &stCmd, 0, sizeof(SysCmd_t));

    /* Pathname */
    strcpy( &stCmd.cPrgPath[0], "/home/ntp/ntpd" );

    /* Argv: "ntpd -4d -P 110 -g -n -L -I eth2" */
    strcpy( &stCmd.cArgv[0][0], "ntpd" );
    strcpy( &stCmd.cArgv[1][0], "-4d" );
    strcpy( &stCmd.cArgv[2][0], "-P" );
    strcpy( &stCmd.cArgv[3][0], "110" );
    strcpy( &stCmd.cArgv[4][0], "-g" );
    strcpy( &stCmd.cArgv[5][0], "-n" );
    strcpy( &stCmd.cArgv[6][0], "-L" );
    strcpy( &stCmd.cArgv[7][0], "-I" );
    strcpy( &stCmd.cArgv[8][0], "eth2" );

    /* Add */
    if( AddSysCmd( &stCmd, &s_stCmdInfo ))
    {
        DEBUG_NTP( "Start ntpd ok\n" );
    }
    else
    {
        DEBUG_NTP( "Start ntpd failed\n" );
        LogWrite( (uint32_t)(LOGID_NTP_E_START_NTP_SRV));
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
    SysCmd_t stCmd;
    memset( &stCmd, 0, sizeof(SysCmd_t));

    /* Pathname */
    strcpy( &stCmd.cPrgPath[0], "/usr/bin/pkill" );

    /* Argv: "pkill ntpd" */
    strcpy( &stCmd.cArgv[0][0], "pkill" );
    strcpy( &stCmd.cArgv[1][0], "ntpd" );

    /* Stop local NTP timing server */
    if( AddSysCmd( &stCmd, &s_stCmdInfo ))
    {
        DEBUG_NTP( "Stop ntpd ok\n" );
    }
    else
    {
        DEBUG_NTP( "Stop ntpd failed\n" );
        LogWrite( (uint32_t)(LOGID_NTP_E_STOP_NTP_SRV));
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

