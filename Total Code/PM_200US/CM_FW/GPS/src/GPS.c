/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : GPS校时模块
*
*  文件名称    : GPS.h
*
*  功能简介    : GPS校时
*
*  作者        : 王东
*
*  创建日期    : 2016-11-23
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
#include <time.h>
#include "../inc/GPS.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                      /* 本CM是否有配置 */
static uint8_t s_ucOrigPktBuff[MAX_GPSTF_LEN];  /* 原始GPS校时包缓冲区 */
static GPSTimingMsg_t s_stGPSTMsg;              /* 处理后的GPS校时消息 */

/* Configuration */
static COMCfgInfo_t s_stCOMCfgInfo[NUM_OF_SERIAL_PORT];  /* 串口配置参数信息 */
static TimeSyncCfgInfo_t s_stTimeSyncCfgInfo;            /* 时间同步配置参数信息 */

/* Pre-Recording */
static GPSTimingSets_t s_stPreGPSSets;
static PPMSets_t s_stPrePPMSets;

/* Default Serial Setting */
static uint8_t const s_ucDeftBR = (uint8_t)(BR9600);
static uint8_t const s_ucDeftDataBits = (uint8_t)(DB8);
static uint8_t const s_ucDeftStop = (uint8_t)(SB1);
static uint8_t const s_ucDeftParity = (uint8_t)(PARITY_ODD);

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );
static void HandleCOMTimingRegs( void );
static void HandleGPSTiming( void );

/* Initialize */
static void InitLocalConfigInfo( void );
static void InitCOMTimingCfgInfo( COMCfgInfo_t *pstSerlCfgInfo );

/* Clear */
static void ClearLocalInfo( void );

/* Select PM Module */
static SelPMID_t SelectPM( void );

static void SetGPSTimingRegs( GPSTimingSets_t const *pstGPSSets );
static void SetPPMRegs( PPMSets_t const *pstPPMSets );

/******************** Configuration ********************/
/* COM Port */
static bool_t CheckCOMCfgInfo( COMConfigInfo_t const *pstCOMCfgInfo, SerialPortType_t emSerl );
static bool_t SwitchCOMCfgInfo( COMCfgInfo_t *pstCOMInfo, COMConfigInfo_t const *pstCOMCfgInfo );
static COMConfigInfo_t const* GetCurCOMCfg( COMCfgInfo_t const *pstCOMInfo );
static bool_t HandleCOMConfig( SerialPortType_t emSerl, COMCfgInfo_t *pstCOMInfo );

/* Serial Port: Timing */
static void ClearGPSRecvBuff( void );
static bool_t SerialRecv( uint8_t pucBuff[], uint16_t *pusLen );
static bool_t RecvGPSTimingPkt( uint8_t pucBuff[], puint16_t pusLen );
static bool_t HandleGPSTimingPkt( uint8_t const *pucOrigPkt, uint16_t usPktLen, GPSTimingMsg_t *pstGPSMsg );
static bool_t RemoveExtraDLE( uint8_t const *pucOrigData, uint16_t usDataLen, GPSTimingMsg_t *pstGPSMsg );
static bool_t CheckPrimUTCTimingData( PrimUTCTimeData_t const *pstPrimUTCData );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GPSInit
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

void GPSInit( void )
{
    DEBUG_GPS( "GPSInit\n" );

    s_bCMHasCfg = false;
    memset( s_ucOrigPktBuff, 0, sizeof(s_ucOrigPktBuff));
    memset( &s_stGPSTMsg, 0, sizeof(GPSTimingMsg_t));
    memset( &s_stPreGPSSets, 0, sizeof(GPSTimingSets_t));
    memset( &s_stPrePPMSets, 0, sizeof(PPMSets_t));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* Timing Regs */
    SetGPSTimingRegs( &s_stPreGPSSets );  /* GPS(Trimble) */
    SetPPMRegs( &s_stPrePPMSets );        /* PPM */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GPSCycle
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

void GPSCycle( void )
{
    static bool_t s_bClrDone = false;
    SysSystemTime_t stSysTime;
    struct timeval stTimeval;
    long64_t llTime = 0;
    PMController_t emPMID = PMA;
    bool_t bSetLinuxSysTime = false;
    NTPTCPConfigInfo_t const *pstTSyncCfg = NULL;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Handle Local Configuration */
    if( s_bCMHasCfg )
    {
        s_bClrDone = false;

        /* Configuration */
        HandleConfiguration();

        /* Handle Timing Registers */
        HandleCOMTimingRegs();

        /* Handle GPS Timing Report Packets */
        HandleGPSTiming();
    }
    else
    {
        if( !s_bClrDone )
        {
            ClearLocalInfo();
            s_bClrDone = true;
        }
    }

    /* Get Time From SysResource */
    emPMID = SwitchPMID( SelectPM());
    if( SysGetSysTimeFromPM( &stSysTime, emPMID ))
    {
        /* s，ms -> us */
        llTime = (long64_t)stSysTime.uiSecond * 1000000;
        llTime += (long64_t)stSysTime.usMiliSecond * 1000;
        DEBUG_GPS( "SysTimeFromPM: s %d, ms %d, 0x%llX\n", stSysTime.uiSecond, stSysTime.usMiliSecond, llTime );

        /* Set FPGA Regs */
        SetSysAdjTime( llTime );

        /* Set linux system time if funcblk/no timing */
        pstTSyncCfg = GetCurTSyncCfg( &s_stTimeSyncCfgInfo );
        if( pstTSyncCfg != NULL )
        {
            DEBUG_GPS( "TimeCfgInfo: %d %d\n", pstTSyncCfg->emActive, pstTSyncCfg->ucTimingSrc );

            if( NOT_ACTIVE == pstTSyncCfg->emActive )
            {
                /* No config */
                bSetLinuxSysTime = true;
            }
            else
            {
                if((TSRC_TBLK == pstTSyncCfg->ucTimingSrc) ||
                   (TSRC_NONE == pstTSyncCfg->ucTimingSrc))
                {
                    /* Funcblk Timing */
                    bSetLinuxSysTime = true;
                }
            }
        }
        else
        {
            DEBUG_GPS( "GetCurTSyncCfg failed\n" );
        }

        if( bSetLinuxSysTime )
        {
            stTimeval.tv_sec = (long)stSysTime.uiSecond;            /* s  -> s */
            stTimeval.tv_usec = (long)stSysTime.usMiliSecond*1000;  /* ms -> us */

            /* Set Linux System Time */
            if( -1 == settimeofday( &stTimeval, NULL ))
            {
                DEBUG_GPS( "settimeofday E-%d\n", errno );
                LogWrite( (uint32_t)(LOGID_GPS_E_SET_TIMEOFDAY));
            }
            else
            {
                DEBUG_GPS( "settimeofday: S-0x%08X, US-0x%08X\n", \
                           (uint32_t)(stTimeval.tv_sec), (uint32_t)(stTimeval.tv_usec));
            }
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
    SerialPortType_t emSerlPort = COM1;

    /* COM Timing Configuration Info. */
    for( emSerlPort = COM1; emSerlPort < NUM_OF_SERIAL_PORT; emSerlPort++ )
    {
        InitCOMTimingCfgInfo( &s_stCOMCfgInfo[emSerlPort]);
    }

    /* Time Sync */
    InitTimeSyncCfgInfo( &s_stTimeSyncCfgInfo );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitCOMTimingCfgInfo
*
* 功能描述: 初始化本地保存的某一串口的校时相关配置信息
*
* 输入参数: pstSerlCfgInfo：指向本地保存的串口配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitCOMTimingCfgInfo( COMCfgInfo_t *pstCOMCfgInfo )
{
    uint8_t ucVerIndex = 0U;
    COMConfigInfo_t *pCOMCfg = NULL;

    if( pstCOMCfgInfo != NULL )
    {
        memset( pstCOMCfgInfo, 0, sizeof(COMCfgInfo_t));

        /* Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pCOMCfg = &( pstCOMCfgInfo->stCOMConfig[ucVerIndex]);
            pCOMCfg->emActive = NOT_ACTIVE;
            pCOMCfg->bUpdate = false;
            pCOMCfg->ucProtocol = (uint8_t)(SPP_NONE);
            pCOMCfg->ucBaudRate = s_ucDeftBR;
            pCOMCfg->ucDataBits = s_ucDeftDataBits;
            pCOMCfg->ucStopBits = s_ucDeftStop;
            pCOMCfg->ucParity = s_ucDeftParity;
        }
    }

    return;
}



/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 清除本地配置信息，重新设置校时寄存器-GPS禁止，PPM
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
    DEBUG_GPS( "ClearLocalInfo\n" );

    s_stGPSTMsg.usLen = 0U;
    memset( &s_stPreGPSSets, 0, sizeof(GPSTimingSets_t));
    memset( &s_stPrePPMSets, 0, sizeof(PPMSets_t));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* Timing Regs */
    SetPPMRegs( &s_stPrePPMSets );        /* PPM */
    SetGPSTimingRegs( &s_stPreGPSSets );  /* GPS(Trimble) */

    /* Clear Timing Serial Buffer: Only COM4 */
    ClearGPSRecvBuff();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SelectPM
*
* 功能描述: 选取与CM模块内部通讯正常的，编号最小的PM模块
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

    /* Select: Normal, Minimum PMID */
    if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMA ))
    {
        emSelPMID = SELPM_A;
    }
    else if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMB ))
    {
        emSelPMID = SELPM_B;
    }
    else if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMC ))
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
* 函数名称: SetGPSTimingRegs
*
* 功能描述: 设置GPS校时(Trimble)寄存器
*
* 输入参数: pstGPSSets：GPS校时设置
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SetGPSTimingRegs( GPSTimingSets_t const *pstGPSSets )
{
    if( pstGPSSets != NULL )
    {
        if( pstGPSSets->bEnable )
        {
            SetTrimBaudRate( pstGPSSets->ucBaudRate );
            SetTrimDataBits( pstGPSSets->ucDataBits );
            SetTrimParity( pstGPSSets->ucParity );
            SetTrimStopBits( pstGPSSets->ucStopBits );
            DEBUG_GPS( "Trim: BR-%d, DB-%d, Parity-%d, SB-%d.\n", pstGPSSets->ucBaudRate, pstGPSSets->ucDataBits, \
                       pstGPSSets->ucParity, pstGPSSets->ucStopBits );
        }

        SetTrimEnable( pstGPSSets->bEnable );
        DEBUG_GPS( "Trim: EN-%d.\n", pstGPSSets->bEnable );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetPPMRegs
*
* 功能描述: 设置PPM寄存器
*
* 输入参数: pstPPMSets：PPM设置
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SetPPMRegs( PPMSets_t const *pstPPMSets )
{
    if( pstPPMSets != NULL )
    {
        SysSetPPMAdjPulseInput((uint32_t)pstPPMSets->ucPulseSrc );
        SysSetPPMAdjPulseTransmit((uint32_t)pstPPMSets->ucPulseRpt );
        SysSetPPMAdjPulsePolarity((uint32_t)pstPPMSets->ucPulsePoly );
        DEBUG_GPS( "PPM: Src%d, Rpt%d, Poly%d.\n", \
                   pstPPMSets->ucPulseSrc, pstPPMSets->ucPulseRpt, pstPPMSets->ucPulsePoly );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleCOMTimingRegs
*
* 功能描述: 设置串口校时相关的寄存器：PPM+GPS
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

static void HandleCOMTimingRegs( void )
{
    GPSTimingSets_t stGPSSets;
    PPMSets_t stPPMSets;
    COMConfigInfo_t const *pstCurCOMCfg = NULL;
    NTPTCPConfigInfo_t const *pstTSyncCfg = NULL;

    /* Init With Prevalue */
    memcpy( &stPPMSets, &s_stPrePPMSets, sizeof(PPMSets_t));
    memcpy( &stGPSSets, &s_stPreGPSSets, sizeof(GPSTimingSets_t));

    /* GPS Timing Sets: Only COM4 */
    pstCurCOMCfg = GetCurCOMCfg( &s_stCOMCfgInfo[COM4]);
    if( pstCurCOMCfg != NULL )
    {
        if((uint8_t)(SPP_GPS) == pstCurCOMCfg->ucProtocol )
        {
            stGPSSets.bEnable = true;
            stGPSSets.ucBaudRate = pstCurCOMCfg->ucBaudRate;
            stGPSSets.ucParity = pstCurCOMCfg->ucParity;

            /* Data Bits */
            if( DB7 == pstCurCOMCfg->ucDataBits )
            {
                stGPSSets.ucDataBits = 0U;
            }
            else
            {
                stGPSSets.ucDataBits = 1U;
            }

            /* Stop Bits */
            if( SB1 == pstCurCOMCfg->ucStopBits )
            {
                stGPSSets.ucStopBits = 0U;
            }
            else
            {
                stGPSSets.ucStopBits = 1U;
            }
        }
        else
        {
            stGPSSets.bEnable = false;
        }
    }
    else
    {
        stGPSSets.bEnable = false;
    }

    /* Pulse Source */
    if( stGPSSets.bEnable )
    {
        /* COM4 */
        stPPMSets.ucPulseSrc = (uint8_t)(PSRC_GPS);
    }
    else
    {
        /* COM1/None*/
        pstCurCOMCfg = GetCurCOMCfg( &s_stCOMCfgInfo[COM1]);
        if( pstCurCOMCfg != NULL )
        {
            if((uint8_t)(SPP_PULSE_SRC) == pstCurCOMCfg->ucProtocol )
            {
                stPPMSets.ucPulseSrc = (uint8_t)(PSRC_P2P);
            }
            else
            {
                stPPMSets.ucPulseSrc = (uint8_t)(PSRC_NONE);
            }
        }
        else
        {
            stPPMSets.ucPulseSrc = (uint8_t)(PSRC_NONE);
        }
    }

    /* Pulse Polarity */
    pstTSyncCfg = GetCurTSyncCfg( &s_stTimeSyncCfgInfo );
    if( pstTSyncCfg != NULL )
    {
        stPPMSets.ucPulsePoly = pstTSyncCfg->ucPulsePolarity;
    }
    else
    {
        stPPMSets.ucPulsePoly = (uint8_t)(POLY_POS);
    }

    /* Pulse Repeat: Only COM2 */
    pstCurCOMCfg = GetCurCOMCfg( &s_stCOMCfgInfo[COM2]);
    if( pstCurCOMCfg != NULL )
    {
        if((uint8_t)(SPP_PULSE_RPT) == pstCurCOMCfg->ucProtocol )
        {
            stPPMSets.ucPulseRpt = (uint8_t)(PRPT_SPT);
        }
        else
        {
            stPPMSets.ucPulseRpt = (uint8_t)(PRPT_NSPT);
        }
    }
    else
    {
        stPPMSets.ucPulseRpt = (uint8_t)(PRPT_NSPT);
    }

    /* Set PPM And GPS Regs */
    if((s_stPrePPMSets.ucPulseSrc != stPPMSets.ucPulseSrc) ||
       (s_stPrePPMSets.ucPulseRpt != stPPMSets.ucPulseRpt) ||
       (s_stPrePPMSets.ucPulsePoly != stPPMSets.ucPulsePoly))
    {
        SetPPMRegs( &stPPMSets );
    }

    if((s_stPreGPSSets.bEnable != stGPSSets.bEnable) ||
       (s_stPreGPSSets.ucBaudRate != stGPSSets.ucBaudRate) ||
       (s_stPreGPSSets.ucDataBits != stGPSSets.ucDataBits) ||
       (s_stPreGPSSets.ucStopBits != stGPSSets.ucStopBits) ||
       (s_stPreGPSSets.ucParity != stGPSSets.ucParity))
    {
        SetGPSTimingRegs( &stGPSSets );
    }

    /* Update */
    memcpy( &s_stPrePPMSets, &stPPMSets, sizeof(PPMSets_t));
    memcpy( &s_stPreGPSSets, &stGPSSets, sizeof(GPSTimingSets_t));

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
    SerialPortType_t emSerlPort = COM1;

    /* COM1-4 */
    for( emSerlPort = COM1; emSerlPort < NUM_OF_SERIAL_PORT; emSerlPort++ )
    {
        bSwitch = HandleCOMConfig( emSerlPort, &s_stCOMCfgInfo[emSerlPort]);
        if( bSwitch )
        {
            DEBUG_GPS( "Switch COM[%d] Cfg\n", emSerlPort+1 );
            LogWrite( (uint32_t)(LOGID_GPS_CFG_SWITCH_COM));
        }
    }

    /* Time Sync */
    bSwitch = HandleTimeSyncConfig( &s_stTimeSyncCfgInfo );
    if( bSwitch )
    {
        DEBUG_GPS( "Switch TimeSync Cfg\n" );
        LogWrite( (uint32_t)(LOGID_GPS_CFG_SWITCH_TIME_SYNC));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleCOMConfig
*
* 功能描述: 检查配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: emSerl：串口号；
*          pstCOMInfo：指向串口对应的配置信息；
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleCOMConfig( SerialPortType_t emSerl, COMCfgInfo_t *pstCOMInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    COMConfigInfo_t stCOMCfgInfo;

    if(((emSerl >= COM1) && (emSerl < NUM_OF_SERIAL_PORT)) &&
       (pstCOMInfo != NULL))
    {
        /* Get CM Configuration Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstCOMInfo->uiCMCfgCRC) ||
               (uiFileVer != pstCOMInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysGetCOMConfigInfo( emSerl, &stCOMCfgInfo ))
                {
                    /* Check Active */
                    if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                       ( ACTIVE == stCOMCfgInfo.emActive ))
                    {
                        /* CM always exist: Switch when stCOMCfgInfo.bUpdate is true */
                        /* Online download and then change CM: stCOMCfgInfo.bUpdate true/false, Local ver is 0 */
                        if( stCOMCfgInfo.bUpdate ||
                            ((0U == pstCOMInfo->uiCMCfgCRC ) && (0U == pstCOMInfo->uiCMCfgVer)))
                        {
                            /* Check Valid */
                            if( CheckCOMCfgInfo( &stCOMCfgInfo, emSerl ))
                            {
                                /* OPC does not have Update Flag: Switch directly */
                                if( SwitchCOMCfgInfo( pstCOMInfo, &stCOMCfgInfo ))
                                {
                                    bSwitch = true;
                                    pstCOMInfo->uiCMCfgCRC = uiCRC;
                                    pstCOMInfo->uiCMCfgVer = uiFileVer;
                                }
                            }
                            else
                            {
                                pstCOMInfo->uiCMCfgCRC = uiCRC;
                                pstCOMInfo->uiCMCfgVer = uiFileVer;
                                DEBUG_GPS( "COM%d Cfg Invalid\n", emSerl+1 );
                                LogWrite( (uint32_t)(LOGID_GPS_E_CFG_COM));
                            }
                        }
                    }
                }
            }
        }/* end of SysGetCMConfigVersion */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCOMCfgInfo
*
* 功能描述: 检查串口配置信息-校时相关是否有效
*
* 输入参数: pstCOMCfgInfo：指向待切换的串口配置信息；
*          emSerl：串口号。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckCOMCfgInfo( COMConfigInfo_t const *pstCOMCfgInfo, SerialPortType_t emSerl )
{
    bool_t bValid = false;
    uint8_t ucProtoMin = (uint8_t)(SPP_NONE);
    uint8_t ucProtoMax = (uint8_t)(SPP_GPS);

    if((pstCOMCfgInfo != NULL) &&
       ((emSerl >= COM1) && (emSerl < NUM_OF_SERIAL_PORT)))
    {
        if( ACTIVE == pstCOMCfgInfo->emActive )
        {
            bValid = true;  /* COM Config Must Active */

            if( COM4 == emSerl )
            {
                /* Protocol */
                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) ||
                    (pstCOMCfgInfo->ucProtocol > ucProtoMax)))
                {
                    bValid = false;
                }

                if( bValid )
                {
                    /* BaudRate: Fixed 9600 */
                    /* DataBits: Fixed 8 */
                    /* Parity: Fixed Odd */
                    /* StopBits: Fixed 1 */
                    if((pstCOMCfgInfo->ucBaudRate != (uint8_t)(BR9600)) ||
                       (pstCOMCfgInfo->ucDataBits != (uint8_t)(DB8)) ||
                       (pstCOMCfgInfo->ucParity != (uint8_t)(PARITY_ODD)) ||
                       (pstCOMCfgInfo->ucStopBits != (uint8_t)(SB1)))
                    {
                        bValid = false;
                    }
                }
            }
            else if( COM1 == emSerl )
            {
                ucProtoMax = (uint8_t)(SPP_PULSE_SRC);

                /* Protocol */
                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) ||
                    (pstCOMCfgInfo->ucProtocol > ucProtoMax)))
                {
                    bValid = false;
                }
            }
            else if( COM2 == emSerl )
            {
                ucProtoMax = (uint8_t)(SPP_PULSE_RPT);

                /* Protocol */
                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) || (pstCOMCfgInfo->ucProtocol > ucProtoMax)) ||
                   ((uint8_t)(SPP_PULSE_SRC) == pstCOMCfgInfo->ucProtocol))
                {
                    bValid = false;
                }
            }
            else
            {
                bValid = true;
            }
        }/* end of if - Active */
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchCOMCfgInfo
*
* 功能描述: 切换串口配置信息
*
* 输入参数: pstCOMInfo：指向本地保存的串口信息；
*          pstCOMCfgInfo：指向待切换的串口配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchCOMCfgInfo( COMCfgInfo_t *pstCOMInfo, COMConfigInfo_t const *pstCOMCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    COMConfigInfo_t *pstDstMBTCPCfgInfo = NULL;

    if((pstCOMInfo != NULL) &&
       (pstCOMCfgInfo != NULL))
    {
        ucCfgIndex = pstCOMInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstMBTCPCfgInfo = &( pstCOMInfo->stCOMConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstMBTCPCfgInfo, pstCOMCfgInfo, sizeof(COMConfigInfo_t));

            /* Update Index */
            pstCOMInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurCOMCfg
*
* 功能描述: 获取某一串口的当前配置信息
*
* 输入参数: pstCOMInfo：指向串口信息
*
* 输出参数: 无
*
* 返 回 值: 串口的当前配置信息，NULL代表无有效配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static COMConfigInfo_t const* GetCurCOMCfg( COMCfgInfo_t const *pstCOMInfo )
{
    uint8_t ucCfgIndex = 0U;
    COMConfigInfo_t const *pstCOMCfg = NULL;

    if( pstCOMInfo != NULL )
    {
        ucCfgIndex = pstCOMInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstCOMCfg = &( pstCOMInfo->stCOMConfig[ucCfgIndex]);
        }
    }

    return pstCOMCfg;
}

/*
***********************************************************************************************************************
* 函数名称: ClearGPSRecvBuff
*
* 功能描述: 清除串口校时接收缓冲区
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

static void ClearGPSRecvBuff( void )
{
    ResetGPSRecvStatus();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SerialRecv
*
* 功能描述: 通过串口接收数据
*
* 输入参数: pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度。
*
* 输出参数: pusLen：数据的实际长度
*
* 返 回 值: 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SerialRecv( uint8_t pucBuff[], uint16_t *pusLen )
{
    bool_t bResult = false;

#ifdef DEBUG_DBGPS
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    if((pucBuff != NULL) &&
       (pusLen != NULL))
    {
        /* Receive */
        if( GPSPortRecv( pucBuff, pusLen ))
        {
            bResult = true;

#ifdef DEBUG_DBGPS
            if( *pusLen > 0U )
            {
                printf( "GPS Timing OriPkt: L %d, Data: \n", *pusLen );

                for( usIndex = 0U; usIndex < *pusLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                    usPerLen++;

                    if(( 30U == usPerLen ) ||
                       ((*pusLen-1) == usIndex))
                    {
                        printf( "\n" );
                        usPerLen = 0U;
                    }
                }
            }
#endif
        }
        else
        {
            bResult = false;  /* Received a error frame */
            *pusLen = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RecvGPSTimingPkt
*
* 功能描述: 接收GPS校时包
*
* 输入参数: pucBuff：接收缓冲区首地址；
*          pusLen：接收缓冲区长度；
*
* 输出参数: pusLen：数据的实际长度。
*
* 返 回 值: 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RecvGPSTimingPkt( uint8_t pucBuff[], puint16_t pusLen )
{
    bool_t bSuccess = false;  /* Receive Successfully Flag */

    if((pucBuff != NULL) &&
       (pusLen != NULL))
    {
        /* Read Serial Message */
        if( SerialRecv( pucBuff, pusLen ))
        {
            if( *pusLen > 0U )
            {
                bSuccess = true;
            }
        }
        else
        {
            *pusLen = 0U;
            ClearGPSRecvBuff();
            DEBUG_GPS( "GPS SerialRecv Failed\n" );
            LogWrite( (uint32_t)(LOGID_GPS_E_SERIAL_RECV));
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: HandleGPSTiming
*
* 功能描述: 处理GPS校时：时钟更新源为GPS时，接收GPS校时包并处理
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

static void HandleGPSTiming( void )
{
    uint16_t usPktLen = 0U;       /* GPS Timing Packet Length */
    struct tm stTime;             /* C Library Time */
    struct timeval stTimeval;     /* Linux System Time */
    NTPTCPConfigInfo_t const *pstTSyncCfg = NULL;
    PrimUTCTimeData_t const *pstPrimUTCData = NULL;

    pstTSyncCfg = GetCurTSyncCfg( &s_stTimeSyncCfgInfo );
    if( pstTSyncCfg != NULL )
    {
        if((uint8_t)(TSRC_GPS) == pstTSyncCfg->ucTimingSrc )
        {
            /* Read Modbus Serial Request Message */
            usPktLen = (uint16_t)sizeof( s_ucOrigPktBuff );
            if( !RecvGPSTimingPkt( s_ucOrigPktBuff, &usPktLen ))
            {
                usPktLen = 0U;
            }

            if( usPktLen > 0U )
            {
                if( HandleGPSTimingPkt( s_ucOrigPktBuff, usPktLen, &s_stGPSTMsg ))
                {
                    pstPrimUTCData = (PrimUTCTimeData_t const *)(&s_stGPSTMsg.ucBuff[0]);

                    /* Check Valid */
                    if( CheckPrimUTCTimingData( pstPrimUTCData ))
                    {
                        DEBUG_GPS( "Tracking Status, UTC Flag: %d %d\n", \
                                   pstPrimUTCData->ucRecvStatus, pstPrimUTCData->ucUTCFlags );

                        /* && UTC Time*/
                        if(((uint8_t)(RECV_DOING_FIXES) == pstPrimUTCData->ucRecvStatus) ||
                           ((uint8_t)(RECV_GOOD_1SV) == pstPrimUTCData->ucRecvStatus) ||
                           ((uint8_t)(RECV_OVERDET_CLK) == pstPrimUTCData->ucRecvStatus))
                        {
                            /* PPS Second Regs */
                            SysSetPPSCounterInitVal( pstPrimUTCData->ucSecond );
                            SysSetPPSCounterInitValEnable( 1 );

                            /* Set Linux System Time */
                            /**** Time transfer ****/
                            memset( &stTime, 0, sizeof(struct tm));

                            /* long tm_year: the number of years since 1900 */
                            stTime.tm_year = (long)(pstPrimUTCData->usYear - 1900U);

                            /* int tm_mon: the number of months since January, in the range 0 to 11 */
                            /* pstPrimUTCData->ucMonth: 1-12 */
                            stTime.tm_mon = (int32_t)(pstPrimUTCData->ucMonth - 1U);

                            /* int tm_mday: the day of the month, in the range 1 to 31 */
                            /* pstPrimUTCData->ucDay: 1-31 */
                            stTime.tm_mday = (int32_t)pstPrimUTCData->ucDay;

                            /* int tm_hour: the number of hours past midnight, in the range 0 to 23 */
                            /* pstPrimUTCData->ucHour: 0-23 */
                            stTime.tm_hour = (int32_t)pstPrimUTCData->ucHour;

                            /* int tm_min: the number of minutes after the hour, in the range 0 to 59*/
                            /* pstPrimUTCData->ucMinute: 0-59 */
                            stTime.tm_min = (int32_t)pstPrimUTCData->ucMinute;

                            /* int tm_sec: the number of seconds after the minute, normally in the range
                             * 0 to 59, but can be up to 60 to allow for leap seconds */
                            /* pstPrimUTCData->ucSecond: 0-59, 60(leap seconds) */
                            stTime.tm_sec = (int32_t)pstPrimUTCData->ucSecond;

                            stTimeval.tv_sec = (long)mktime( &stTime );  /* s */
                            stTimeval.tv_usec = (long)(pstPrimUTCData->dFSecond*1000*1000);

                            DEBUG_GPS( "UTC: %d.%d.%d %d:%d:%d %f\n", pstPrimUTCData->usYear, pstPrimUTCData->ucMonth, \
                                       pstPrimUTCData->ucDay, pstPrimUTCData->ucHour, pstPrimUTCData->ucMinute, \
                                       pstPrimUTCData->ucSecond, pstPrimUTCData->dFSecond );

                            /* Set Linux System Time */
                            if( -1 == settimeofday( &stTimeval, NULL ))
                            {
                                DEBUG_GPS( "settimeofday E-%d\n", errno );
                                LogWrite( (uint32_t)(LOGID_GPS_E_SET_TIMEOFDAY));
                            }
                            else
                            {
                                DEBUG_GPS( "settimeofday: S-0x%08X, US-0x%08X\n", \
                                           (uint32_t)(stTimeval.tv_sec), (uint32_t)(stTimeval.tv_usec));
                            }
                        }
                    }
                    else
                    {
                        DEBUG_GPS( "PrimUTCTimingData Invalid\n" );
                        LogWrite( (uint32_t)(LOGID_GPS_E_UTC_TIMING_DATA));
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleGPSTimingPkt
*
* 功能描述: 处理GPS校时包：<DLE><id><data string bytes><DLE><ETX>
*
* 输入参数: pucOrigPkt：GPS原始校时包首地址；
*          usPktLen：GPS原始校时包的长度；
*
* 输出参数: pstGPSMsg：存放处理后的校时消息。
*
* 返 回 值: 无
*
* 注意事项: data string bytes: every <DLE> byte in the data string is preceded by an extra <DLE> byte ('stuffing').
***********************************************************************************************************************
*/

static bool_t HandleGPSTimingPkt( uint8_t const *pucOrigPkt, uint16_t usPktLen, GPSTimingMsg_t *pstGPSMsg )
{
    bool_t bResult = false;

    if((pucOrigPkt != NULL) &&
       (usPktLen > (uint16_t)(MIN_GPSTF_LEN)) &&
       (pstGPSMsg != NULL))
    {
        /* First: DLE; Last: DLE ETX */
        if(((uint8_t)(GPSTF_DLE) == pucOrigPkt[0]) &&
           ((uint8_t)(GPSTF_DLE) == pucOrigPkt[usPktLen-2]) &&
           ((uint8_t)(GPSTF_ETX) == pucOrigPkt[usPktLen-1]))
        {
            /* ID: 0x8F */
            if((uint8_t)(GPSTF_ID) == pucOrigPkt[1])
            {
                /* data string bytes */
                if( RemoveExtraDLE( &pucOrigPkt[2], usPktLen-(uint16_t)(MIN_GPSTF_LEN), pstGPSMsg ))
                {
                    /* Subcode: 0xAD - Primary UTC Time */
                    if((uint8_t)(GPSTF_SEL_SUBCODE) == pstGPSMsg->ucBuff[0])
                    {
                        if((uint16_t)(GPSTF_DATALEN_AD) == pstGPSMsg->usLen )
                        {
                            bResult = true;
                        }
                        else
                        {
                            DEBUG_GPS( "GPSTimingPkt(0x8F-AD) Data Len: %d != %d\n", \
                                       pstGPSMsg->usLen, GPSTF_DATALEN_AD );
                            LogWrite( (uint32_t)(LOGID_GPS_E_GPS_TIMING_PKT_LEN));
                        }
                    }
                    else
                    {
                        DEBUG_GPS( "GPSTimingPkt Subcode: 0x%02X != 0x%02X\n", pucOrigPkt[2], GPSTF_SEL_SUBCODE );
                        LogWrite( (uint32_t)(LOGID_GPS_E_GPS_TIMING_PKT_SUBCODE));
                    }
                }
                else
                {
                    DEBUG_GPS( "RemoveExtraDLE Error\n" );
                    LogWrite( (uint32_t)(LOGID_GPS_E_GPS_TIMING_PKT_REMOVE_DLE));
                }
            }
            else
            {
                DEBUG_GPS( "GPSTimingPkt ID: 0x%02X != 0x%02X\n", pucOrigPkt[1], GPSTF_ID );
                LogWrite( (uint32_t)(LOGID_GPS_E_GPS_TIMING_PKT_ID));
            }
        }
        else
        {
            DEBUG_GPS( "GPSTimingPkt E#1\n" );
            LogWrite( (uint32_t)(LOGID_GPS_E_GPS_TIMING_PKT_HT));
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RemoveExtraDLE
*
* 功能描述: 去除<data string bytes>中的额外<DLE>字节
*
* 输入参数: pucOrigData：GPS原始校时包中的数据；
*          usDataLen：GPS原始校时包中的数据长度；
*
* 输出参数: pstGPSMsg：存放处理后的校时消息。
*
* 返 回 值: 是否成功
*
* 注意事项: data string bytes: every <DLE> byte in the data string is preceded by an extra <DLE> byte ('stuffing').
***********************************************************************************************************************
*/

static bool_t RemoveExtraDLE( uint8_t const *pucOrigData, uint16_t usDataLen, GPSTimingMsg_t *pstGPSMsg )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usMsgIndex = 0U;

    if((pucOrigData != NULL) &&
       (usDataLen > 0U) &&
       (pstGPSMsg != NULL))
    {
        bResult = true;

        while( usIndex < usDataLen )
        {
            if( pucOrigData[usIndex] != (uint8_t)(GPSTF_DLE))
            {
                pstGPSMsg->ucBuff[usMsgIndex] = pucOrigData[usIndex];
                usMsgIndex++;
                usIndex++;
            }
            else
            {
                /* <DLE><DLE> */
                if( pucOrigData[usIndex] != pucOrigData[usIndex+1])
                {
                    /* Invalid */
                    bResult = false;
                    DEBUG_GPS( "<data string bytes> DLE Not Repeat\n" );
                    break;
                }
                else
                {
                    pstGPSMsg->ucBuff[usMsgIndex] = pucOrigData[usIndex];
                    usMsgIndex++;
                    usIndex += 2;
                }
            }
        }/* end of while */

        if( bResult )
        {
            pstGPSMsg->usLen = usMsgIndex;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CheckPrimUTCTimingData
*
* 功能描述: 检查校时数据是否有效
*
* 输入参数: pstPrimUTCData：校时数据
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckPrimUTCTimingData( PrimUTCTimeData_t const *pstPrimUTCData )
{
    bool_t bValid = false;

    if( pstPrimUTCData != NULL )
    {
        if((pstPrimUTCData->usYear >= 1970U) &&
           ((pstPrimUTCData->ucMonth >= 1U ) && (pstPrimUTCData->ucMonth <= 12U)) &&
           ((pstPrimUTCData->ucDay >= 1U ) && (pstPrimUTCData->ucDay <= 31U)) &&
           (pstPrimUTCData->ucHour <= 23U) &&
           (pstPrimUTCData->ucMinute <= 59U) &&
           (pstPrimUTCData->ucSecond <= 60U))  /* Leap second: 60 */
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

