/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CM_FW外部通信模块
*
*  文件名称    : ExterComm.c
*
*  功能简介    : 收发网络数据
*
*  作者        : 王东
*
*  创建日期    : 2016-01-05
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
#include <stdio.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <error.h>
#include <dirent.h>
#include <string.h>
#include <byteswap.h>
#include <linux/fs.h>
#include "../inc/ExterComm.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                                        /* 本CM是否有配置 */
static uint8_t s_ucLocalStaID;                                    /* 本控制站的ID */
static uint8_t s_ucLocalCMSlotID;                                 /* 本CM模块的槽位号 */
static NetworkSockInfo_t s_stSockInfo[NUM_OF_NETWORK_PORT];       /* 网口套接字信息 */
static ConnSockInfo_t s_stSOEConnSock[APP_SW_SOE_NUM];            /* SOE接收信息，成员分别对应一个SOE软件 */
static ConnSockInfo_t s_stAMSConnSock[APP_SW_AMS_NUM];            /* AMS接收信息，成员分别对应一个AMS软件 */
static ConnSockInfo_t s_stCliConnSock[APP_SW_CLIENT_NUM];         /* 客户端软件接收信息，成员分别对应一个客户端软件 */
static ConnSockInfo_t s_stCS1131ConnSock[APP_SW_CS1131_NUM];      /* 与CS1131软件连接的套接字信息数组 */                                                        /* 成员分别对应一个CS1131软件 */
static ConnSockInfo_t s_stOPCConnSock[APP_SW_OPC_NUM];            /* OPC/HMI软件接收信息，成员分别对应一个OPC/HMI软件 */
static ConnSockInfo_t s_stMBTCPMstrConnSock[MAX_MBTCP_MSTR_CNT];  /* 与Modbus TCP Master连接的信息数组，*/
                                                                  /* 成员分别对应一个Modbus TCP主站 */
static uint8_t s_ucRecvBuff[MAX_MSG_LEN];                         /* 接收缓冲区，用于接收网络数据 */
static NetMsg_t s_stNetMsg;                                       /* 网络消息 */
static MBTCPMsg_t s_stMBTCPMsg;                                   /* Modbus TCP 消息 */
static CliConnSockInfo_t s_stMBTCPSlaveConnSock[NUM_OF_NETWORK_PORT][MAX_MODBUS_SLAVE_NUM];
                                                                  /* 与Modbus TCP Slave连接的套接字信息数组，*/
                                                                  /* 成员分别对应一个Modbus TCP从站 */

/* Configuration */
static ExtCommNetworkInfo_t s_stNetInfo[NUM_OF_NETWORK_PORT];     /* 网口配置参数信息  */
static ExtCommModbusTCPInfo_t s_stMBTCPInfo[NUM_OF_NETWORK_PORT]; /* Modbus TCP配置参数信息 */
static ModbusMstrCfgInfo_t s_stMBMstrCfgInfo;                     /* Modbus Master Info. Modbus主站信息 */

#if 0
static NTPInfo_t s_stNTPInfo;                                     /* 校时配置参数信息 */
#endif

/* Modbus Master Configuration Copy */
static uint32_t s_uiMBMstrCfgOffset;

/* Network Adapter Close Flag */
static bool_t s_bEth1Close;
static bool_t s_bEth2Close;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );  /* Configuration */
static void MaintainStatus( void );       /* Maintain Status */
static void AcceptNewConnect( void );     /* Accept New Connect */
static void ReadNetMsg( void );           /* Receive Network Message */

/* Network Adapter */
static bool_t LinuxSystem( char const *cShellCmd );
static bool_t SetLocalIPAddr( NetworkPortType_t emNet, uint32_t uiIPAddr );
static bool_t SetLocalNetMask( NetworkPortType_t emNet, uint32_t uiNetMask );
static bool_t SetLocalGateWay( NetworkPortType_t emNet, uint32_t uiNetMask );
static bool_t HandleIPRoute( NetworkPortType_t emNet, uint32_t uiIPAddr );

/* Initialize */
/* Initialize Configuration */
static void InitLocalConfigInfo( void );
static void InitNetInfo( ExtCommNetworkInfo_t *pstNetInfo, NetworkPortType_t emNet );
static void InitModbusTCPInfo( ExtCommModbusTCPInfo_t *pstMBTCPInfo );
/*static void InitNTPInfo( NTPInfo_t *pstNTPInfo );*/

/* Initialize Receive Info. */
#if 0
static void InitUDPRecvInfo( UDPRecvInfo_t *pstRecvInfo, uint8_t ucCount );
#endif

static void InitSocketInfo( void );
static void InitConnSockInfo( ConnSockInfo_t pstRecvInfo[], uint8_t ucCount );
static void InitCliConnSockInfo( CliConnSockInfo_t pstRecvInfo[], uint8_t ucCount );

/* Configuration */
static void ClearLocalInfo( void );
static void ClearLocalConfigInfo( void );

/* Network Configuration */
static bool_t HandleNetworkConfig( NetworkPortType_t emNet, ExtCommNetworkInfo_t *pstNetInfo );
static bool_t CheckNetCfgInfo( ExtCommNetConfigInfo_t const *pstNetCfgInfo );
static bool_t SwitchNetCfgInfo( ExtCommNetworkInfo_t *pstNetInfo, ExtCommNetConfigInfo_t const *pstNetCfgInfo );
static void HandleNetworkCfgChg( NetworkPortType_t emNet, ExtCommNetworkInfo_t *pstNetInfo );
static void HandleIPChange( NetworkPortType_t emNet, ExtCommNetworkInfo_t const *pstNetInfo );
static uint32_t GetIPAddr( NetworkPortType_t emNet );

/* Modbus TCP Configuration */
static bool_t HandleModbusTCPConfig( NetworkPortType_t emNet, ExtCommModbusTCPInfo_t *pstMBTCPInfo );
static bool_t CheckModbusTCPCfgInfo( ExtCommModbusTCPConfigInfo_t const *pstMBTCPCfgInfo );
static bool_t SwitchModbusTCPCfgInfo( ExtCommModbusTCPInfo_t *pstMBTCPInfo,
                                      ExtCommModbusTCPConfigInfo_t const *pstMBTCPCfgInfo );
static void HandleModbusTCPCfgChg( NetworkPortType_t emNet, ExtCommModbusTCPInfo_t const *pstMBTCPInfo );
static ExtCommModbusTCPConfigInfo_t* GetCurMBTCPCfg( ExtCommModbusTCPInfo_t *pstMBTCPInfo );
static uint32_t GetMBAllowMstrIPAddr( ExtCommModbusTCPInfo_t *pstMBTCPInfo );

/* Connect To Modbus TCP Slave*/
static void HandleConnToModbusTCPSlave( ModbusMasterConfig_t const *pstMstrCfg );
static CliConnSockInfo_t* FindModbusTCPSlaveConnInfo( uint32_t uiIPAddr, NetworkPortType_t emNet );
static CliConnSockInfo_t* FindEmptyModbusTCPSlave( NetworkPortType_t emNet );
static bool_t IsMBTCPSlaveExist( NetworkPortType_t emNet, uint32_t uiIPAddr );
static void ClearModbusTCPSlaveConnInfo( CliConnSockInfo_t *pstSlaveConnInfo );

#if 0
/* NTP Configuration */
static bool_t HandleNTPConfig( NTPInfo_t *pstNTPInfo );
static bool_t CheckNTPCfgInfo( NTPTCPConfigInfo_t const *pstNTPCfgInfo );
static bool_t SwitchNTPCfgInfo( NTPInfo_t *pstNTPInfo, NTPTCPConfigInfo_t const *pstNTPCfgInfo );
#endif

/* Create */
static void CreateNetworkSocket( NetworkPortType_t emNet, uint32_t uiIPAddr );
static void CreateModbusTCPListenSocket( NetworkPortType_t emNet, uint32_t uiIPAddr, uint16_t usPort,
                                         bool_t bNonBlock, SOCKET *phSocket );

/* Clear Receive Info. */
static void CloseNetworkSocket( NetworkPortType_t emNet );
#if 0
static void ClearUDPAppRecvInfo( NetworkPortType_t emNet, UDPRecvInfo_t *pstRecvInfo,
                                 uint8_t ucIndex, SysSoftwareType_t emSWType );
#endif
static void ClearTCPAppRecvInfo( NetworkPortType_t emNet, ConnSockInfo_t *pstRecvInfo,
                                 uint8_t ucIndex, SysSoftwareType_t emSWType );
static void ClearMBTCPSrvRecvInfo( NetworkPortType_t emNet, ConnSockInfo_t pstRecvInfo[], uint8_t ucIndex );
static void ClearMBTCPCliRecvInfo( NetworkPortType_t emNet, CliConnSockInfo_t pstRecvInfo[], uint8_t ucIndex );

/* Connect Status */
static void CheckCliConnStatus( CliConnSockInfo_t *pstCliInfo );
static void CliReConnect( CliConnSockInfo_t *pstCliInfo );

/* Accept Connect */
static void AcceptConnect( SOCKET hLisSock, NetworkPortType_t emNet, CommType_t emType,
                           ConnSockInfo_t *pstConnSock, uint8_t ucMaxConnCount );

/* Receive */
/* From CS1131 Software */
static void ReadCS1131Msg( void );

/* From OPC Software */
static void ReadOPCMsg( void );

/* From SOE Software */
static void ReadSOEMsg( void );

/* From AMS Software */
static void ReadAMSMsg( void );

/* From Client Software */
static void ReadClientMsg( void );

/* From Modbus TCP Master */
static void ReadModbusTCPMasterMsg( void );

/* From Modbus TCP Slave */
static void ReadModbusTCPSlaveMsg( void );

/* From Other Station */
static void ReadP2PMsg( void );

#if 0
/* From SNTP/NTP Master/Slave */
static void ReadNTPMsg( void );
#endif

/* Read Data - APP TCP */
static void ReadAPPTCPMsg( SysSoftwareType_t emSWType, uint8_t ucSWNum, ConnSockInfo_t pstConnArr[],
                           uint8_t pucRecvBuff[], NetMsg_t *pstNetMsg );
static bool_t ReadAPPTCPData( SysSoftwareType_t emSWType, ConnSockInfo_t pstConnArr[], uint8_t ucSWIndex,
                              uint8_t pucBuff[], uint32_t uiDataLen );

/* Read UDP Message */
static bool_t ReadSingleP2PMsg( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                SOCKADDR *pstSrcAddr );
#if 0
static bool_t ReadSingleNTPMsg( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                SOCKADDR *pstSrcAddr );
#endif

static bool_t ReadUDPHeaderAtLeast( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                    SOCKADDR *pstSrcAddr );
static bool_t ReadLastUDPData( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usLastLen, \
                               SOCKADDR *pstSrcAddr );

/* Read Data - Modbus */
static bool_t ReadModbusTCPMasterData( uint8_t ucIndex, uint8_t pucBuff[], uint32_t uiDataLen );
static bool_t ReadModbusTCPSlaveData( NetworkPortType_t emNet, uint8_t ucIndex, uint8_t pucBuff[], uint32_t uiDataLen );

/* Check Valid */
/* PC Software Message */
static bool_t CheckAppMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen );

/* P2P Message */
static bool_t CheckP2PMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen );

#if 0
/* NTP Message */
static bool_t CheckNTPMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen );
#endif


/* Send */
static uint8_t SendToSOE( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex );
static uint8_t SendToAMS( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex );
static uint8_t SendToClient( uint8_t const pucData[], uint16_t usDataLen,
                             uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                             CommType_t emSendType, uint8_t ucSWIndex );
static uint8_t SendToOPC( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex );
static uint8_t SendToCS1131( uint8_t const pucData[], uint16_t usDataLen,
                             uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                             CommType_t emSendType, uint8_t ucSWIndex );
static uint8_t SendToMBTCPMstr( uint8_t const pucData[], uint16_t usDataLen,
                                uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                CommType_t emSendType, uint8_t ucMasterIndex );
static uint8_t SendToMBTCPSlave( uint8_t const pucData[], uint16_t usDataLen,
                                 uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                 CommType_t emSendType );
static uint8_t SendToP2P( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType );
/*static uint8_t SendToNTPDevice( uint8_t const pucData[], uint16_t usDataLen,
                                uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                CommType_t emSendType );*/
static uint8_t SendUDPData( SOCKET hSock, uint32_t uiDstIPAddr, uint16_t usDstPort,
                            uint8_t const pucData[], uint32_t uiDataLen );
static uint8_t SendTCPData( SOCKET hSock, uint8_t const pucData[], uint32_t uiDataLen );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ExternalCommInit
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

void ExternalCommInit( void )
{
	DEBUG_RTS( "ExternalCommInit\n" );

	NetworkPortType_t emNet = NET1;
	uint32_t uiIPAddr = 0U;
	uint32_t uiFlag = 0U;

	s_bCMHasCfg = false;
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();

	/* Initialize Local Configuration Info. */
    InitLocalConfigInfo();

	/* Initialize Network Socket Info. */
	InitSocketInfo();

    /* Initialize Receive Info. */
	InitConnSockInfo( s_stSOEConnSock, (uint8_t)(APP_SW_SOE_NUM));           /* SOE */
	InitConnSockInfo( s_stAMSConnSock, (uint8_t)(APP_SW_AMS_NUM));           /* AMS */
	InitConnSockInfo( s_stCliConnSock, (uint8_t)(APP_SW_CLIENT_NUM));        /* Client */
    InitConnSockInfo( s_stCS1131ConnSock, (uint8_t)(APP_SW_CS1131_NUM));     /* CS1131 */
    InitConnSockInfo( s_stOPCConnSock, (uint8_t)(APP_SW_OPC_NUM));           /* OPC */
    InitConnSockInfo( s_stMBTCPMstrConnSock, (uint8_t)(MAX_MBTCP_MSTR_CNT)); /* Modbus TCP Master */

    /* Modbus TCP Slave */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        InitCliConnSockInfo( s_stMBTCPSlaveConnSock[emNet], (uint8_t)(MAX_MODBUS_SLAVE_NUM));
    }

    /* Receive Buffer */
    memset( s_ucRecvBuff, 0, sizeof(s_ucRecvBuff));

    /* Network Message */
    memset( &s_stNetMsg, 0, sizeof(NetMsg_t));
    s_stNetMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Modbus TCP Message */
    memset( &s_stMBTCPMsg, 0, sizeof(MBTCPMsg_t));
    s_stMBTCPMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Set Network */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        if( NET1 == emNet )
        {
            uiIPAddr = (uint32_t)(DEF_IPADDR_NET1);
        }
        else
        {
            uiIPAddr = (uint32_t)(DEF_IPADDR_NET2);
        }

        /* Set IP Address, Sub-NetMask */
        /* Note: GateWay-Set only when user defined! */
        /* Close IRQ */
        syscall( __NR_local_disable, uiFlag );

        if( SetLocalIPAddr( emNet, uiIPAddr ) &&
            SetLocalNetMask( emNet, (uint32_t)(DEF_SUBNET_MASK)))
        {
            usleep( 10000000 );  /* Wait network ready: 10s */
            DEBUG_RTS( "Set Net[%d] Success\n", emNet+1 );
            if( !HandleIPRoute( emNet, uiIPAddr ))
            {
                DEBUG_RTS( "Net[%d]: IPRoute Faild\n", emNet+1 );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_IPROUTE));
            }

            CreateNetworkSocket( emNet, uiIPAddr );  /* Create Network Socket */
        }
        else
        {
            DEBUG_RTS( "Set Net[%d] Failed\n", emNet+1 );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_NET));
        }

        /* Open IRQ */
        syscall(__NR_local_enable, uiFlag);
    }

    /* Network Adapter Close Flag */
    s_bEth1Close = false;
    s_bEth2Close = false;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExternalCommCycle
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

void ExternalCommCycle( void )
{
    static bool_t s_bClrDone = false;
    static uint32_t s_uiEth1CycCnt = 0U;  /* Eth1 Cycle Count */
    static uint32_t s_uiEth2CycCnt = 0U;  /* Eth2 Cycle Count */
    uint32_t uiEthStatus = 0;  /* Network Adapter Status: 0-Normal/ 1-Close */
    uint32_t uiFlag = 0U;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Update Local Station ID */
    s_ucLocalStaID = (uint8_t)SysGetLocalStationID();

    /* Update CM Slot ID */
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();

    /* Set Network Adapter Close Flag */
    /* 0: Normal; 1: Close */
    /* Eth1 */
    uiEthStatus = syscall( __NR_get_eth1 );
    if( 0 == uiEthStatus )
    {
        s_bEth1Close = false;
    }
    else if( 1 == uiEthStatus )
    {
        s_bEth1Close = true;

        /* Close IRQ */
        syscall( __NR_local_disable, uiFlag );

        /* Down */
        if( !LinuxSystem( "ifconfig eth1 down" ))
        {
            DEBUG_RTS( "Eth1 down Error!\n" );
        }

        /* Open IRQ */
        syscall(__NR_local_enable, uiFlag);

        DEBUG_RTS( "eth1 close\n" );
    }
    else
    {
        s_bEth1Close = false;
        DEBUG_RTS( "eth1 syscall error!\n" );
        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYSCALL));
    }

    /* Handle eth1 close */
    if( s_bEth1Close )
    {
        s_uiEth1CycCnt++;
        if( s_uiEth1CycCnt >= (uint32_t)(ETH_CHK_INTL_CYC_CNT))
        {
            s_uiEth1CycCnt = 0U;

            /* Close IRQ */
            syscall( __NR_local_disable, uiFlag );

            /* Down */
            if( !LinuxSystem( "ifconfig eth1 down" ))
            {
                DEBUG_RTS( "Eth1 down Error!\n" );
            }

            /* UP */
            if( !LinuxSystem( "ifconfig eth1 up" ))
            {
                DEBUG_RTS( "Eth1 Up Error!\n" );
            }

            /* IP Route */
            if( !HandleIPRoute( 0U, GetIPAddr(NET1)))
            {
                DEBUG_RTS( "Eth1 HandleIPRoute Error!\n" );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_IPROUTE));
            }

            /* Set */
            syscall( __NR_set_eth1 );

            /* Open IRQ */
            syscall(__NR_local_enable, uiFlag);
        }
    }
    else
    {
        s_uiEth1CycCnt = 0U;
    }

    /* Eth2 */
    uiEthStatus = syscall( __NR_get_eth2 );
    if( 0 == uiEthStatus )
    {
        s_bEth2Close = false;
    }
    else if( 1 == uiEthStatus )
    {
        s_bEth2Close = true;

        /* Close IRQ */
        syscall( __NR_local_disable, uiFlag );

        /* Down */
        if( !LinuxSystem( "ifconfig eth2 down" ))
        {
            DEBUG_RTS( "Eth2 down Error!\n" );
        }

        /* Open IRQ */
        syscall(__NR_local_enable, uiFlag);

        DEBUG_RTS( "eth2 close\n" );
    }
    else
    {
        s_bEth2Close = false;
        DEBUG_RTS( "eth2 syscall error!\n" );
        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYSCALL));
    }

    /* Handle eth2 close */
    if( s_bEth2Close )
    {
        s_uiEth2CycCnt++;
        if( s_uiEth2CycCnt >= (uint32_t)(ETH_CHK_INTL_CYC_CNT))
        {
            s_uiEth2CycCnt = 0U;

            /* Close IRQ */
            syscall( __NR_local_disable, uiFlag );

            /* Down */
            if( !LinuxSystem( "ifconfig eth2 down" ))
            {
                DEBUG_RTS( "Eth2 down Error!\n" );
            }

            /* UP */
            if( !LinuxSystem( "ifconfig eth2 up" ))
            {
                DEBUG_RTS( "Eth2 Up Error!\n" );
            }

            /* IP Route */
            if( !HandleIPRoute( 1U, GetIPAddr(NET2)))
            {
                DEBUG_RTS( "Eth2 HandleIPRoute Error!\n" );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_IPROUTE));
            }

            /* Set */
            syscall( __NR_set_eth2 );

            /* Open IRQ */
            syscall(__NR_local_enable, uiFlag);
        }
    }
    else
    {
        s_uiEth2CycCnt = 0U;
    }

    /* Handle Local Configuration */
    if( s_bCMHasCfg )
    {
        s_bClrDone = false;
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

    /* Maintain Status */
    MaintainStatus();

    /* Accept New Connect */
    AcceptNewConnect();

    /* Read and pre-process Network Message */
    ReadNetMsg();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExternalCommSend
*
* 功能描述: 模块发送函数
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucIndex：上位机软件/Modbus TCP主站的索引号，其他发送类型填零。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t ExternalCommSend( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    bool_t bEthClose = false;

    /* Eth Close */
    if( NET1 == emNetwork )
    {
        bEthClose = s_bEth1Close;
    }
    else
    {
        bEthClose = s_bEth2Close;
    }

    if( bEthClose )
    {
        ucResult = (uint8_t)(EETHCLOSE);
    }
    else
    {
        switch( emSendType )
        {
            case TYPE_SOE:
                ucResult = SendToSOE( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_AMS:
                ucResult = SendToAMS( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_CLIENT:
                ucResult = SendToClient( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_OPC:
                ucResult = SendToOPC( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_CS1131:
                ucResult = SendToCS1131( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_MBTCP_MSTR:
                ucResult = SendToMBTCPMstr( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType, ucIndex );
            break;
            case TYPE_MBTCP_SLAVE:
                ucResult = SendToMBTCPSlave( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType );
            break;
            case TYPE_P2P:
                ucResult = SendToP2P( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType );
            break;
    #if 0
            case TYPE_NTP:
                ucResult = SendToNTPDevice( pucData, usDataLen, uiIPAddr, usPort, emNetwork, emSendType );
            break;
    #endif
            default:
                ucResult = (uint8_t)(EPARA);
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_TYPE));
            break;
        }/* switch */
    }

    return ucResult;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: LinuxSystem
*
* 功能描述: 封装Linux system函数，健全对其返回值的处理
*
* 输入参数: cShellCmd：shell 命令
*
* 输出参数: 无
*
* 返 回 值: 是否执行成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t LinuxSystem( char const *cShellCmd )
{
    bool_t bResult = false;
    int32_t iStatus = 0;  /* exit status value */

    if( cShellCmd != NULL )
    {
        iStatus = system( cShellCmd );
        if( -1 == iStatus )
        {
            DEBUG_RTS( "system error: %d\n", GetErrorNo());
        }
        else
        {
            if( WIFEXITED(iStatus))
            {
                if( 0 == WEXITSTATUS(iStatus))
                {
                    /* run shell command/script successfully */
                    bResult = true;
                }
                else
                {
                    DEBUG_RTS( "run shell command/script fail, script exit code: %d\n", WEXITSTATUS(iStatus));
                }
            }
            else
            {
                DEBUG_RTS( "exit status = [%d]\n", WEXITSTATUS(iStatus));
            }
        }
    }

    if( !bResult )
    {
        LogWrite( (uint32_t)(LOGID_EXCOMM_E_LINUX_EXECVE));
    }

    return bResult;
}


/*
***********************************************************************************************************************
* 函数名称: SetLocalIPAddr
*
* 功能描述: 设置网口的IP地址，并立即生效
*
* 输入参数: emNet：网口索引值；
*          uiIPAddr：网络地址-主机字节序。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetLocalIPAddr( NetworkPortType_t emNet, uint32_t uiIPAddr )
{
    bool_t bResult = false;
    struct in_addr stAddr;          /* IP Address */
    char_t cNetName[10] = { 0 };    /* Network Name */
    char_t cShellCmd[100] = { 0 };  /* Shell Command */

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        memset( &stAddr, 0, sizeof(struct in_addr));
        memset( cNetName, 0, sizeof(cNetName));
        memset( cShellCmd, 0, sizeof(cShellCmd));
        stAddr.s_addr = htonl( uiIPAddr );

        sprintf( cNetName, "eth%d", emNet+1 );
        sprintf( cShellCmd, "ifconfig %s %s", cNetName, inet_ntoa( stAddr ));
        bResult = LinuxSystem( cShellCmd );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetLocalNetMask
*
* 功能描述: 设置网口的子网掩码
*
* 输入参数: emNet：网口索引值；
*          uiNetMask：子网掩码-主机字节序。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetLocalNetMask( NetworkPortType_t emNet, uint32_t uiNetMask )
{
    bool_t bResult = false;
    struct in_addr stAddr;          /* Netmask */
    char_t cNetName[10] = { 0 };    /* Network Name */
    char_t cShellCmd[100] = { 0 };  /* Shell Command */

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
       memset( &stAddr, 0, sizeof(struct in_addr));
       memset( cNetName, 0, sizeof(cNetName));
       memset( cShellCmd, 0, sizeof(cShellCmd));
       stAddr.s_addr = htonl( uiNetMask );

       sprintf( cNetName, "eth%d", emNet+1 );
       sprintf( cShellCmd, "ifconfig %s netmask %s", cNetName, inet_ntoa( stAddr ));
       bResult = LinuxSystem( cShellCmd );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetLocalGateWay
*
* 功能描述: 设置网口的默认网关
*
* 输入参数: emNet：网口索引值；
*          uiGateWay：默认网关-主机字节序。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetLocalGateWay( NetworkPortType_t emNet, uint32_t uiGateWay )
{
    bool_t bResult = false;
    int32_t iErr = -1;
    SOCKET hSock = (SOCKET)(INVALID_SOCKET);
    struct rtentry stRT;
    SOCKADDR_IN *pstSetAddr = NULL;

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        hSock = socket( AF_INET, SOCK_DGRAM, 0 );
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            /* Delete Existing Defalt Gateway */
            memset( &stRT, 0, sizeof(stRT));

            /* Destination */
            pstSetAddr = (SOCKADDR_IN*)&stRT.rt_dst;
            pstSetAddr->sin_family = AF_INET;
            pstSetAddr->sin_addr.s_addr = 0U;

            /* Mask */
            pstSetAddr = (SOCKADDR_IN*)&stRT.rt_genmask;
            pstSetAddr->sin_family = AF_INET;
            pstSetAddr->sin_addr.s_addr = 0U;

            stRT.rt_flags = RTF_UP;
            iErr = ioctl( hSock, SIOCDELRT, &stRT );
            if((0 == iErr) || (ESRCH == GetErrorNo()))
            {
                /* Set New Gateway */
                memset( &stRT, 0, sizeof(stRT));

                /* Destination */
                pstSetAddr = (SOCKADDR_IN*)&stRT.rt_dst;
                pstSetAddr->sin_family = AF_INET;
                pstSetAddr->sin_addr.s_addr = 0U;

                /* Gateway */
                pstSetAddr = (SOCKADDR_IN*)&stRT.rt_gateway;
                pstSetAddr->sin_family = AF_INET;
                pstSetAddr->sin_addr.s_addr = htonl( uiGateWay );

                /* Mask */
                pstSetAddr = (SOCKADDR_IN*)&stRT.rt_genmask;
                pstSetAddr->sin_family = AF_INET;
                pstSetAddr->sin_addr.s_addr = 0U;

                stRT.rt_flags = RTF_UP | RTF_GATEWAY;

                if( ioctl( hSock, SIOCADDRT, &stRT ) != (int32_t)(SOCKET_ERROR))
                {
                    bResult = true;
                }
                else
                {
                    DEBUG_RTS( "ioctl: Add Route Failed, Net %d, Errno %d\n", emNet+1, GetErrorNo());
                }
            }
            else
            {
                DEBUG_RTS( "ioctl: Delete Route Failed, Net %d, Errno %d\n", emNet+1, GetErrorNo());
            }

            CloseSocket( &hSock );
        }
        else
        {
            DEBUG_RTS( "Create Socket Failed, Net %d, Errno %d\n", emNet+1, GetErrorNo());
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: HandleIPRoute
*
* 功能描述: 设置网口的IP路由
*
* 输入参数: emNet：网口索引值；
*          uiIPAddr：网口IP地址-主机字节序。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleIPRoute( NetworkPortType_t emNet, uint32_t uiIPAddr )
{
    bool_t bResult = false;
    uint8_t ucTableNum = 0U;          /* IP Route Table Number */
    struct in_addr stAddr;            /* IP Address */
    char_t cNetName[10] = { 0 };      /* Network Name: eth0/1/2/... */
    char_t cShellCmd[500] = { 0 };    /* Shell Command */

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        memset( &stAddr, 0, sizeof(struct in_addr));
        memset( cNetName, 0, sizeof(cNetName));
        memset( cShellCmd, 0, sizeof(cShellCmd));

        sprintf( cNetName, "eth%d", emNet+1 );
        stAddr.s_addr = htonl(uiIPAddr);

        if( NET1 == emNet )
        {
            ucTableNum = 100U;
        }
        else
        {
            ucTableNum = 200U;
        }

        /* Shell Command Array */
        /* Clear ip route table */
        sprintf( cShellCmd, "ip route flush table %d", ucTableNum );
        bResult = LinuxSystem( cShellCmd );

        /* Add default route */
        if( bResult )
        {
            sprintf( cShellCmd, "ip route add default dev %s table %d", cNetName, ucTableNum );
            bResult = LinuxSystem( cShellCmd );
        }

        /* Add from route */
        if( bResult )
        {
            sprintf( cShellCmd, "ip rule add from %s table %d", inet_ntoa( stAddr ), ucTableNum );
            bResult = LinuxSystem( cShellCmd );
        }

        /* Clear ip route cache */
        if( bResult )
        {
            sprintf( cShellCmd, "ip route flush cache" );
            bResult = LinuxSystem( cShellCmd );
        }
    }

    return bResult;
}

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
    NetworkPortType_t emNet = NET1;

    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        /* Net Configuration Info. */
        InitNetInfo( &s_stNetInfo[emNet], emNet );

        /* Modbus TCP Configuration Info. */
        InitModbusTCPInfo( &s_stMBTCPInfo[emNet]);
    }

    /* Modbus Master Info. */
    InitModbusMstrInfo( &s_stMBMstrCfgInfo );
    s_uiMBMstrCfgOffset = 0U;

#if 0
    /* NTP Configuration */
    InitNTPInfo( &s_stNTPInfo );
#endif

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitNetInfo
*
* 功能描述: 初始化本地保存的某一网口的配置信息
*
* 输入参数: pstNetInfo：指向本地保存的访问控制信息；
*          emNet：网口号。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitNetInfo( ExtCommNetworkInfo_t *pstNetInfo, NetworkPortType_t emNet )
{
    uint8_t ucVerIndex = 0U;
    ExtCommNetConfigInfo_t *pNetCfg = NULL;

    if((pstNetInfo != NULL) &&
       ((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)))
    {
        memset( pstNetInfo, 0, sizeof(ExtCommNetworkInfo_t));

        /* IP Change Flag */
        pstNetInfo->bIPChange = false;

        /* Network Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pNetCfg = &( pstNetInfo->stNetConfig[ucVerIndex]);
            pNetCfg->emActive = NOT_ACTIVE;
            pNetCfg->bUpdate = false;
            pNetCfg->uiSubnetMask = (uint32_t)(DEF_SUBNET_MASK);
            pNetCfg->uiGateway = 0U;

            /* IP Address */
            if( NET1 == emNet )
            {
                pNetCfg->uiIPAddr = (uint32_t)(DEF_IPADDR_NET1);
            }
            else
            {
                pNetCfg->uiIPAddr = (uint32_t)(DEF_IPADDR_NET2);
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitModbusTCPInfo
*
* 功能描述: 初始化本地保存的某一网口的ModbusTCP信息
*
* 输入参数: pstMBTCPInfo：指向本地保存的ModbusTCP信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitModbusTCPInfo( ExtCommModbusTCPInfo_t *pstMBTCPInfo )
{
    uint8_t ucVerIndex = 0U;
    ExtCommModbusTCPConfigInfo_t *pMBTCPCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        memset( pstMBTCPInfo, 0, sizeof(ExtCommModbusTCPInfo_t));

        /* Modbus TCP Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pMBTCPCfg = &( pstMBTCPInfo->stMBTCPConfig[ucVerIndex]);
            pMBTCPCfg->emActive = NOT_ACTIVE;
            pMBTCPCfg->bUpdate = false;
            pMBTCPCfg->ucProtocol = (uint8_t)(MODBUS_TCP_NONE);
        }
    }

    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: InitNTPInfo
*
* 功能描述: 初始化本地保存的某一网口的配置信息
*
* 输入参数: pstNetInfo：指向本地保存的访问控制信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitNTPInfo( NTPInfo_t *pstNTPInfo )
{
    uint8_t ucVerIndex = 0U;
    NTPTCPConfigInfo_t *pNTPCfg = NULL;

    if( pstNTPInfo != NULL )
    {
        memset( pstNTPInfo, 0, sizeof(NTPInfo_t));

        /* Network Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pNTPCfg = &( pstNTPInfo->stNTPConfig[ucVerIndex]);
            pNTPCfg->emActive = NOT_ACTIVE;
            pNTPCfg->bUpdate = false;
        }
    }

    return;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: InitSocketInfo
*
* 功能描述: 初始化套接字信息
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

static void InitSocketInfo( void )
{
    NetworkPortType_t emNet = NET1;

    /* Initialize */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        /* CS1131 Listen Socket */
        s_stSockInfo[emNet].stLisCS1131Sock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisCS1131Sock.usPort = (uint16_t)(PORTNUM_CS1131);

        /* OPC Listen Socket */
        s_stSockInfo[emNet].stLisOPCSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisOPCSock.usPort = (uint16_t)(PORTNUM_OPC);

        /* SOE Server Socket */
        s_stSockInfo[emNet].stLisSOESock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisSOESock.usPort = (uint16_t)(PORTNUM_SOE);

        /* Client Server Socket */
        s_stSockInfo[emNet].stLisCliSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisCliSock.usPort = (uint16_t)(PORTNUM_CLIENT);

        /*AMS Server Socket */
        s_stSockInfo[emNet].stLisAMSSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisAMSSock.usPort = (uint16_t)(PORTNUM_AMS);

        /* Modbus TCP Listen Socket */
        s_stSockInfo[emNet].stLisMBSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stLisMBSock.usPort = (uint16_t)(PORTNUM_MODBUS);

        /* P2P Socket */
        s_stSockInfo[emNet].stP2PSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stP2PSock.usPort = (uint16_t)(PORTNUM_P2P);

#if 0
        /* NTP Socket */
        s_stSockInfo[emNet].stNTPSock.hSocket = (int32_t)(INVALID_SOCKET);
        s_stSockInfo[emNet].stNTPSock.usPort = (uint16_t)(PORTNUM_NTP);
#endif
    }

    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: InitUDPRecvInfo
*
* 功能描述: 初始化UDP接收信息
*
* 输入参数: pstRecvInfo：in，接收信息数组首地址；
*          uiCount: in，数组元素数目。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitUDPRecvInfo( UDPRecvInfo_t *pstRecvInfo, uint8_t ucCount )
{
	uint8_t ucIndex = 0U;

	/* Clear */
	if( pstRecvInfo != NULL )
	{
        for( ucIndex = 0U; ucIndex < ucCount; ucIndex++ )
        {
        	pstRecvInfo[ucIndex].uiIPAddr = 0U;
        	pstRecvInfo[ucIndex].usPort = 0U;
        	pstRecvInfo[ucIndex].emNetwork = INVALID_NETWORK_PORT;
        	pstRecvInfo[ucIndex].bConnFlag = false;
        }
	}

	return;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: InitConnSockInfo
*
* 功能描述: 初始化连接套接字接收信息
*
* 输入参数: pstRecvInfo：in，接收信息数组首地址；
*          ucCount: in，数组元素数目。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitConnSockInfo( ConnSockInfo_t pstRecvInfo[], uint8_t ucCount )
{
	uint8_t ucIndex = 0U;

	/* Clear */
	if( pstRecvInfo != NULL )
	{
        for( ucIndex = 0U; ucIndex < ucCount; ucIndex++ )
        {
        	pstRecvInfo[ucIndex].hSocket = (int32_t)(INVALID_SOCKET);
        	pstRecvInfo[ucIndex].uiIPAddr = 0U;
        	pstRecvInfo[ucIndex].usPort = 0U;
        	pstRecvInfo[ucIndex].emNetwork = INVALID_NETWORK_PORT;
        	pstRecvInfo[ucIndex].bConnFlag = false;
        }
	}

	return;
}

/*
***********************************************************************************************************************
* 函数名称: InitCliConnSockInfo
*
* 功能描述: 初始化客户端连接套接字接收信息
*
* 输入参数: pstRecvInfo：in，接收信息数组首地址；
*          ucCount: in，数组元素数目。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitCliConnSockInfo( CliConnSockInfo_t pstRecvInfo[], uint8_t ucCount )
{
    uint8_t ucIndex = 0U;

    /* Clear */
    if((pstRecvInfo != NULL) &&
       (ucCount <= (uint8_t)(MAX_MODBUS_SLAVE_NUM)))
    {
        for( ucIndex = 0U; ucIndex < ucCount; ucIndex++ )
        {
            memset( &pstRecvInfo[ucIndex], 0, sizeof(CliConnSockInfo_t));
            pstRecvInfo[ucIndex].bWaitConn = false;
            pstRecvInfo[ucIndex].stConnSock.hSocket = (int32_t)(INVALID_SOCKET);
            pstRecvInfo[ucIndex].stConnSock.uiIPAddr = 0U;
            pstRecvInfo[ucIndex].stConnSock.usPort = 0U;
            pstRecvInfo[ucIndex].stConnSock.emNetwork = INVALID_NETWORK_PORT;
            pstRecvInfo[ucIndex].stConnSock.bConnFlag = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 清除本地配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存；
*          清除系统资源中的Modbus相关缓冲区。
***********************************************************************************************************************
*/

static void ClearLocalInfo( void )
{
    uint8_t ucIndex = 0U;
    NetworkPortType_t emNet = NET1;

    /* Local Configuration Info. */
    ClearLocalConfigInfo();

    /* Modbus: Disconnect with MBTCP Slave */
    /*         Clear Modbus Msg In System Resource */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        /* Recv Info From Modbus TCP Slave */
        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            ClearMBTCPCliRecvInfo( emNet, s_stMBTCPSlaveConnSock[emNet], ucIndex );
        }

        SysClearModbusRespMsg( emNet );
        InitCliConnSockInfo( s_stMBTCPSlaveConnSock[emNet], (uint8_t)(MAX_MODBUS_SLAVE_NUM));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalConfigInfo
*
* 功能描述: 清除本地保存的配置信息：网口配置(保持连接+对比) 和 MBTCP配置(保持连接+对比) 除外
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存；
*          为保持已有连接 - 不包括与Modbus TCP从站的连接，网口配置和MBTCP配置保持不变。
***********************************************************************************************************************
*/

static void ClearLocalConfigInfo( void )
{
    /* Modbus Master Info. */
    ClearModbusMstrInfo( &s_stMBMstrCfgInfo );
    s_uiMBMstrCfgOffset = 0U;

#if 0
    /* NTP Configuration */
    InitNTPInfo( &s_stNTPInfo );
#endif

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleConfiguration
*
* 功能描述: 检查配置参数的版本是否变化，若变化，则拷贝到本地并切换，然后根据变化情况进行相应的处理
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
    ExtCommNetworkInfo_t *pstNetInfo = NULL;
    ExtCommModbusTCPInfo_t *pstMBTCPInfo = NULL;
    ModbusMasterConfig_t const *pstMBMstrCfg = NULL;
    NetworkPortType_t emNet = NET1;

    /* Loop Network */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        /* Network Port Configuration */
        pstNetInfo = &s_stNetInfo[emNet];
        if( HandleNetworkConfig( emNet, pstNetInfo ))
        {
            DEBUG_RTS( "Net[%d] Cfg Switch\n", emNet+1 );
            LogWrite( (uint8_t)(LOGID_EXCOMM_CFG_SWITCH_NET));

            /* Handle Configuration Change */
            HandleNetworkCfgChg( emNet, pstNetInfo );
        }

        /* Modbus TCP Configuration */
        pstMBTCPInfo = &s_stMBTCPInfo[emNet];
        if( HandleModbusTCPConfig( emNet, pstMBTCPInfo ))
        {
            DEBUG_RTS( "Net[%d] MBTCPCfg Switch\n", emNet+1 );
            LogWrite( (uint8_t)(LOGID_EXCOMM_CFG_SWITCH_MBTCP));

            /* Handle Configuration Change */
            if( !pstNetInfo->bIPChange )
            {
                HandleModbusTCPCfgChg( emNet, pstMBTCPInfo );
            }
        }

        /* Handle IP Change */
        if( pstNetInfo->bIPChange )
        {
            HandleIPChange( emNet, pstNetInfo );
            pstNetInfo->bIPChange = false;
        }
    }/* end of for */

    /* Modbus Master Info. */
    if( HandleModbusMstrInfo( &s_stMBMstrCfgInfo, &s_uiMBMstrCfgOffset, s_ucLocalCMSlotID ))
    {
        DEBUG_RTS( "ExtComm: MBMstr Cfg Switch\n" );
        LogWrite( (uint8_t)(LOGID_EXCOMM_CFG_SWITCH_MBMSTR));

        /* Handle Connect To Modbus Slave Station */
        pstMBMstrCfg = GetLocalModbusMstrCfg( &s_stMBMstrCfgInfo, s_ucLocalCMSlotID );
        if( pstMBMstrCfg != NULL )
        {
            if( ACTIVE == pstMBMstrCfg->emActive )
            {
                HandleConnToModbusTCPSlave( pstMBMstrCfg );
            }
        }
    }

#if 0
    /* NTP/SNTP Configuration */
    if( HandleNTPConfig( &s_stNTPInfo ))
    {
        DEBUG_RTS( "NTP Configuration Switch.\n" );
    }
#endif

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleNetworkConfig
*
* 功能描述: 配置参数版本变化时，拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: emNet：网口对应的端口号；
*          pstNetInfo：指向本地保存的网口信息。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleNetworkConfig( NetworkPortType_t emNet, ExtCommNetworkInfo_t *pstNetInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    ExtCommNetConfigInfo_t stNetCfgInfo;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstNetInfo != NULL))
    {
        /* Get CM Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstNetInfo->uiCMCfgCRC) ||
               (uiFileVer != pstNetInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysExterCommGetNetworkConfigInfo( emNet, &stNetCfgInfo ))
                {
                    /* Check Active */
                    if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                       ( ACTIVE == stNetCfgInfo.emActive ))
                    {
                        /* CM always exist: Switch when stNetCfgInfo.bUpdate is true */
                        /* Online download and then change CM: stNetCfgInfo.bUpdate true/false, Local ver is 0 */
                        if( stNetCfgInfo.bUpdate ||
                            ((0U == pstNetInfo->uiCMCfgCRC ) && (0U == pstNetInfo->uiCMCfgVer)))
                        {
                            /* Check Valid */
                            if( CheckNetCfgInfo( &stNetCfgInfo ))
                            {
                                /* Switch */
                                if( SwitchNetCfgInfo( pstNetInfo, &stNetCfgInfo ))
                                {
                                    bSwitch = true;
                                    pstNetInfo->uiCMCfgCRC = uiCRC;
                                    pstNetInfo->uiCMCfgVer = uiFileVer;
                                }
                            }
                            else
                            {
                                pstNetInfo->uiCMCfgCRC = uiCRC;
                                pstNetInfo->uiCMCfgVer = uiFileVer;
                                DEBUG_RTS( "ExtComm Net[%d] Cfg Invalid\n", emNet+1 );
                                LogWrite( (uint8_t)(LOGID_EXCOMM_E_CFG_NET));
                            }
                        }
                    }
                }
            }
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckNetCfgInfo
*
* 功能描述: 检查网口配置信息是否有效
*
* 输入参数: pstNetCfgInfo：指向待切换的网口配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckNetCfgInfo( ExtCommNetConfigInfo_t const *pstNetCfgInfo )
{
    bool_t bValid = false;

    if( pstNetCfgInfo != NULL )
    {
        /* Active Flag: ACTIVE/NOT_ACTIVE */
        if((ACTIVE == pstNetCfgInfo->emActive) ||
           (NOT_ACTIVE == pstNetCfgInfo->emActive))
        {
            bValid = true;

            /* Check If Active */
            if( ACTIVE == pstNetCfgInfo->emActive )
            {
                if((0U == pstNetCfgInfo->uiIPAddr) ||
                   (0U == pstNetCfgInfo->uiSubnetMask))
                {
                    bValid = false;
                }
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchNetCfgInfo
*
* 功能描述: 切换网口配置信息
*
* 输入参数: pstNetInfo：指向本地保存的网口信息；
*          pstNetCfgInfo：指向待切换的网口配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchNetCfgInfo( ExtCommNetworkInfo_t *pstNetInfo, ExtCommNetConfigInfo_t const *pstNetCfgInfo )
{
    bool_t bSwitch = false;
    uint8_t ucCfgIndex = 0U;
    ExtCommNetConfigInfo_t *pstDstNetCfgInfo = NULL;

    if((pstNetInfo != NULL) &&
       (pstNetCfgInfo != NULL))
    {
        ucCfgIndex = pstNetInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = (ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstNetCfgInfo = &( pstNetInfo->stNetConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstNetCfgInfo, pstNetCfgInfo, sizeof(ExtCommNetConfigInfo_t));

            /* Update Index */
            pstNetInfo->ucIndex = ucCfgIndex;
            bSwitch = true;
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: HandleNetworkCfgChg
*
* 功能描述: 网口配置参数变化时，根据网口配置参数变化情况进行相应的处理
*
* 输入参数: emNet：网口对应的端口号；
*          pstNetInfo：指向本地保存的网口信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleNetworkCfgChg( NetworkPortType_t emNet, ExtCommNetworkInfo_t *pstNetInfo )
{
    bool_t bSetIPRoute = false;  /* Set IP Route Flag */
    uint8_t ucCfgIndex = 0U;     /* Current Configuration Index */
    ExtCommNetConfigInfo_t const *pstNewCfg = NULL;
    ExtCommNetConfigInfo_t const *pstOldCfg = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstNetInfo != NULL))
    {
        ucCfgIndex = pstNetInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point to New Configuration */
            pstNewCfg = &( pstNetInfo->stNetConfig[ucCfgIndex]);

            /* Calculate Old Configuration Index */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);

            /* Point to Old Configuration */
            pstOldCfg = &( pstNetInfo->stNetConfig[ucCfgIndex]);

            /* Compare IP Address */
            if( pstNewCfg->uiIPAddr != pstOldCfg->uiIPAddr )
            {
                bSetIPRoute = true;
                pstNetInfo->bIPChange = true;

                /* IP Address */
                if( !SetLocalIPAddr( emNet, pstNewCfg->uiIPAddr ))
                {
                    DEBUG_RTS( "Net[%d]: SetLocalIPAddr Failed\n", emNet+1 );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_NET_IP));
                }
                else
                {
                    DEBUG_RTS( "Net[%d]: SetLocalIPAddr OK\n", emNet+1 );
                }
            }

            /* Compare SubNetMask */
            if( pstNewCfg->uiSubnetMask != pstOldCfg->uiSubnetMask )
            {
                bSetIPRoute = true;

                /* Sub-NetMask */
                if( !SetLocalNetMask( emNet, pstNewCfg->uiSubnetMask ))
                {
                    DEBUG_RTS( "Net[%d]: SetLocalNetMask Failed\n", emNet+1 );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_NET_MASK));
                }
                else
                {
                    DEBUG_RTS( "Net[%d]: SetLocalNetMask OK\n", emNet+1 );
                }
            }

            /* Compare Default Gateway */
            if( pstNewCfg->uiGateway != pstOldCfg->uiGateway )
            {
                bSetIPRoute = true;

                /* GateWay */
                if( !SetLocalGateWay( emNet, pstNewCfg->uiGateway ))
                {
                    DEBUG_RTS( "Net[%d]: SetLocalGateWay Failed\n", emNet+1 );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_GATEWAY));
                }
                else
                {
                    DEBUG_RTS( "Net[%d]: SetLocalGateWay OK\n", emNet+1 );
                }
            }

            if( bSetIPRoute )
            {
                if( !HandleIPRoute( emNet, pstNewCfg->uiIPAddr ))
                {
                    DEBUG_RTS( "Net[%d]: HandleIPRoute Faild\n", emNet+1 );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SET_IPROUTE));
                }
                else
                {
                    DEBUG_RTS( "Net[%d]: HandleIPRoute OK\n", emNet+1 );
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPConfig
*
* 功能描述: 检查配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: emNet：网口号；
*          pstMBTCPInfo：指向网口对应的Modbus TCP配置信息；
*          bIPChg：网口IP是否变化。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleModbusTCPConfig( NetworkPortType_t emNet, ExtCommModbusTCPInfo_t *pstMBTCPInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    ExtCommModbusTCPConfigInfo_t stMBTCPCfgInfo;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstMBTCPInfo != NULL))
    {
        /* RTData Version */
        /*if( HandleRTDataVer( &( pstMBTCPInfo->stRTDataVer )))*/
        {
            /* Get CM Configuration Version */
            if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
            {
                /* Compare Version */
                if((uiCRC != pstMBTCPInfo->uiCMCfgCRC) ||
                   (uiFileVer != pstMBTCPInfo->uiCMCfgVer))
                {
                    /* Copy To Local */
                    if( SysExterCommGetModbusTCPConfigInfo( emNet, &stMBTCPCfgInfo ))
                    {
#if 0
                        DEBUG_RTS( "ExterComm: Modbus TCP Configuration: CM Active: %d, TCP Active: %d, Update: %d.\n",
                                   SysGetCMConfigInfoActiveFlag(), stMBTCPCfgInfo.emActive, stMBTCPCfgInfo.bUpdate );
#endif

                        /* Check Active */
                        if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                           ( ACTIVE == stMBTCPCfgInfo.emActive ))
                        {
                            /* CM always exist: Switch when stMBTCPCfgInfo.bUpdate is true */
                            /* Online download and then change CM: stMBTCPCfgInfo.bUpdate true/false, Local ver is 0 */
                            if( stMBTCPCfgInfo.bUpdate ||
                                ((0U == pstMBTCPInfo->uiCMCfgCRC ) && (0U == pstMBTCPInfo->uiCMCfgVer)))
                            {
                                /* Check Valid */
                                if( CheckModbusTCPCfgInfo( &stMBTCPCfgInfo ))
                                {
                                    if( SwitchModbusTCPCfgInfo( pstMBTCPInfo, &stMBTCPCfgInfo ))
                                    {
                                        bSwitch = true;
                                        pstMBTCPInfo->uiCMCfgCRC = uiCRC;
                                        pstMBTCPInfo->uiCMCfgVer = uiFileVer;
                                    }
                                }
                                else
                                {
                                    pstMBTCPInfo->uiCMCfgCRC = uiCRC;
                                    pstMBTCPInfo->uiCMCfgVer = uiFileVer;
                                    DEBUG_RTS( "ExtComm: Net[%d] MBTCPCfg Invalid\n", emNet+1 );
                                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_CFG_MBTCP));
                                }
                            }
                        }
                    }
                }
            }/* end of SysGetCMConfigVersion */
        }/* end of HandleRTDataVer */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusTCPCfgInfo
*
* 功能描述: 检查网口ModbusTCP配置信息是否有效
*
* 输入参数: pstMBTCPCfgInfo：指向待切换的网口ModbusTCP配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusTCPCfgInfo( ExtCommModbusTCPConfigInfo_t const *pstMBTCPCfgInfo )
{
    bool_t bValid = false;

    if( pstMBTCPCfgInfo != NULL )
    {
        /* Active Flag: ACTIVE/NOT_ACTIVE */
        /* Protocol: None, Master, Slave, All */
        if(((ACTIVE == pstMBTCPCfgInfo->emActive) || (NOT_ACTIVE == pstMBTCPCfgInfo->emActive)) &&
           ((pstMBTCPCfgInfo->ucProtocol >= (uint8_t)(MODBUS_TCP_NONE)) && (pstMBTCPCfgInfo->ucProtocol <= (uint8_t)(MODBUS_TCP_ALL))))
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchModbusTCPCfgInfo
*
* 功能描述: 切换网口配置信息
*
* 输入参数: pstMBTCPInfo：指向本地保存的网口ModbusTCP信息；
*          pstMBTCPCfgInfo：指向待切换的网口ModbusTCP配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchModbusTCPCfgInfo( ExtCommModbusTCPInfo_t *pstMBTCPInfo,
                                      ExtCommModbusTCPConfigInfo_t const *pstMBTCPCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    ExtCommModbusTCPConfigInfo_t *pstDstMBTCPCfgInfo = NULL;

    if((pstMBTCPInfo != NULL) &&
       (pstMBTCPCfgInfo != NULL))
    {
        ucCfgIndex = pstMBTCPInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstMBTCPCfgInfo = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstMBTCPCfgInfo, pstMBTCPCfgInfo, sizeof(ExtCommModbusTCPConfigInfo_t));

            /* Update Index */
            pstMBTCPInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPCfgChg
*
* 功能描述: 网口IP未变化时，根据配置参数变化情况进行相应的处理
*
* 输入参数: emNet：网口号；
*          pstMBTCPInfo：指向网口ModbusTCP信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusTCPCfgChg( NetworkPortType_t emNet, ExtCommModbusTCPInfo_t const *pstMBTCPInfo )
{
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    uint8_t ucIndex = 0U;
    uint32_t uiIPAddr = 0U;
    uint32_t uiAMIPAddr = 0U;
    ExtCommModbusTCPConfigInfo_t const *pstNewCfg = NULL;
    ExtCommModbusTCPConfigInfo_t const *pstOldCfg = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstMBTCPInfo != NULL))
    {
        ucCfgIndex = pstMBTCPInfo->ucIndex;  /* Current Configuration Index */

        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point to New Configuration */
            pstNewCfg = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);

            /* Calculate Old Configuration Index */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);

            /* Point to Old Configuration */
            pstOldCfg = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);

            /* Compare Protocol */
#if 0
            DEBUG_RTS( "HandleModbusTCPCfgChg: Protocol-New %d, Old:%d.\n", pstNewCfg->ucProtocol, \
                       pstOldCfg->ucProtocol );
#endif
            if( pstNewCfg->ucProtocol != pstOldCfg->ucProtocol )
            {
                /* Handle Modbus TCP Protocol Change */
                if(((uint8_t)(MODBUS_TCP_NONE) == pstNewCfg->ucProtocol) ||
                   ((uint8_t)(MODBUS_TCP_MASTER) == pstNewCfg->ucProtocol))
                {
                    CloseListenSocket( &( s_stSockInfo[emNet].stLisMBSock.hSocket ));
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
                    {
                        ClearMBTCPSrvRecvInfo( emNet, s_stMBTCPMstrConnSock, ucIndex );
                    }
                }
                else /* SLAVE / MASTER_SLAVE */
                {
                    uiIPAddr = GetIPAddr( emNet );
                    CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisMBSock.usPort, true,
                                        &(s_stSockInfo[emNet].stLisMBSock.hSocket));
                }
            }

            /* MBTCPMstr Permission: Proto SLAVE / MASTER_SLAVE */
            if(((uint8_t)(MODBUS_TCP_SLAVE) == pstNewCfg->ucProtocol) ||
               ((uint8_t)(MODBUS_TCP_ALL) == pstNewCfg->ucProtocol))
            {
                /* Allowed IP Addr: 0-Allow all; Others-Only the specific ip addr */
                uiAMIPAddr = GetMBAllowMstrIPAddr( &s_stMBTCPInfo[emNet]);
                if( uiAMIPAddr != 0U )
                {
                    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
                    {
                        if( s_stMBTCPMstrConnSock[ucIndex].bConnFlag &&
                            (emNet == s_stMBTCPMstrConnSock[ucIndex].emNetwork) &&
                            (s_stMBTCPMstrConnSock[ucIndex].uiIPAddr != uiAMIPAddr))
                        {
                            /* Not Allow: Clear */
                            ClearMBTCPSrvRecvInfo( emNet, s_stMBTCPMstrConnSock, ucIndex );
                            DEBUG_RTS( "MBTCP Not Allow This Master Access: AllowIPAddr-0x%04X, MstrIPAddr-0x%04X\n", \
                                       uiAMIPAddr, s_stMBTCPMstrConnSock[ucIndex].uiIPAddr );
                        }
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurMBTCPCfg
*
* 功能描述: 获取当前使用的MBTCP配置
*
* 输入参数: pstMBTCPInfo：某网口ModbusTCP信息
*
* 输出参数: 无
*
* 返 回 值: 当前使用的MBTCP配置
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ExtCommModbusTCPConfigInfo_t* GetCurMBTCPCfg( ExtCommModbusTCPInfo_t *pstMBTCPInfo )
{
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    ExtCommModbusTCPConfigInfo_t* pstCurCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        ucCfgIndex = pstMBTCPInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point to New Configuration */
            pstCurCfg = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);
            if( NOT_ACTIVE == pstCurCfg->emActive )
            {
                pstCurCfg = NULL;
            }
        }
    }

    return pstCurCfg;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBTCPAllowMstrIP
*
* 功能描述: 网口IP未变化时，根据配置参数变化情况进行相应的处理
*
* 输入参数: emNet：网口号；
*          pstMBTCPInfo：指向网口ModbusTCP信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint32_t GetMBAllowMstrIPAddr( ExtCommModbusTCPInfo_t *pstMBTCPInfo )
{
    uint32_t uiAMIPAddr = 0U;  /* Allowed MBTCP Master IP Address */
    ExtCommModbusTCPConfigInfo_t const *pstCurCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        pstCurCfg = GetCurMBTCPCfg( pstMBTCPInfo );
        if( pstCurCfg != NULL )
        {
            uiAMIPAddr = pstCurCfg->uiIPAddr;
        }
    }

    return uiAMIPAddr;
}

/*
***********************************************************************************************************************
* 函数名称: HandleConnToModbusTCPSlave
*
* 功能描述: 与被移除从站断开连接，与新增从站建立连接
*
* 输入参数: pstMstrCfg：指向当前的Modbus主站配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleConnToModbusTCPSlave( ModbusMasterConfig_t const *pstMstrCfg )
{
    ModbusMasterPort_t const *pstMstrPort = NULL;
    ModbusSlaveChange_t const *pstSlaveChg = NULL;
    CliConnSockInfo_t *pstSlaveConnInfo = NULL;
    uint32_t uiIndex = 0U;
    uint32_t uiIPAddr = 0U;
    NetworkPortType_t emNet = NET1;
    CommPort_t emPort = PORT_NET1;
    SOCKET hSocket = (int32_t)(INVALID_SOCKET);

    if( pstMstrCfg != NULL )
    {
        for( emPort = PORT_NET1; emPort <= PORT_NET2; emPort++ )
        {
            pstMstrPort = &( pstMstrCfg->stModbusMasterPort[emPort]);
            emNet = (NetworkPortType_t)( emPort - PORT_NET1 ) + NET1;
            DEBUG_RTS( "Net[%d], Active Flag %d, Update Flag %d\n", emNet+1, pstMstrPort->emActive, \
                       pstMstrPort->bUpdate );

            if((ACTIVE == pstMstrPort->emActive) && (pstMstrPort->bUpdate))
            {
                /* Get Slave Change */
                pstSlaveChg = &( pstMstrPort->stModbusSlaveChange );

                /* Deleted Slave */
                DEBUG_RTS( "Net[%d]: Deleted Slave Num %d\n", emNet+1, pstSlaveChg->uiDelTCPStationNum );
                for( uiIndex = 0U; uiIndex < pstSlaveChg->uiDelTCPStationNum; uiIndex++ )
                {
                    uiIPAddr = pstSlaveChg->uiDelStaionIP[uiIndex];

                    /* Find Socket */
                    pstSlaveConnInfo = FindModbusTCPSlaveConnInfo( uiIPAddr, emNet );
                    if( pstSlaveConnInfo != NULL )
                    {
                        /* Clear Conn Info. */
                        ClearModbusTCPSlaveConnInfo( pstSlaveConnInfo );
                    }
                    else
                    {
                        DEBUG_RTS( "Net[%d]: Delete MBTCPSlave IP-(0x%08X) Not Exist\n", emNet+1, uiIPAddr );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_DEL_MBTCP_SLAVE));
                    }
                }

                /* Added Slave */
                DEBUG_RTS( "Net[%d]: Added Slave Num %d\n", emNet+1, pstSlaveChg->uiAddTCPStationNum );
                for( uiIndex = 0U; uiIndex < pstSlaveChg->uiAddTCPStationNum; uiIndex++ )
                {
                    uiIPAddr = pstSlaveChg->uiAddStaionIP[uiIndex];
                    DEBUG_RTS( "Net[%d]: Added Slave IP-0x%08X\n", emNet+1, uiIPAddr );
                    if( !IsMBTCPSlaveExist( emNet, uiIPAddr ))
                    {
                        pstSlaveConnInfo = FindEmptyModbusTCPSlave( emNet );
                        if( pstSlaveConnInfo != NULL )
                        {
                            /* Connect */
                            if( Connect( uiIPAddr, (uint16_t)(PORTNUM_MODBUS), true, &hSocket ))
                            {
                                /* Connect Successfully */
                                pstSlaveConnInfo->stConnSock.bConnFlag = true;
                                pstSlaveConnInfo->bWaitConn = false;
                                pstSlaveConnInfo->usWaitCnt = 0U;
                                DEBUG_RTS( "Net[%d] Connect To New MBTCPSlave: IP-(0x%08X) OK\n", emNet+1, uiIPAddr );
                            }
                            else
                            {
                                /* Connect Failed / Need Wait */
                                pstSlaveConnInfo->stConnSock.bConnFlag = false;
                                pstSlaveConnInfo->usWaitCnt = 0U;

                                if( hSocket != INVALID_SOCKET )
                                {
                                    /* Need wait connect finished */
                                    pstSlaveConnInfo->bWaitConn = true;
                                }
                                else
                                {
                                    /* Connect Failed: Need re-connected */
                                    pstSlaveConnInfo->bWaitConn = false;
                                }
                            }

                            /* Set Other Local Info. */
                            pstSlaveConnInfo->stConnSock.hSocket = hSocket;
                            pstSlaveConnInfo->stConnSock.uiIPAddr = uiIPAddr;
                            pstSlaveConnInfo->stConnSock.usPort = (uint16_t)(PORTNUM_MODBUS);
                            pstSlaveConnInfo->stConnSock.emNetwork = emNet;
                        }
                        else
                        {
                            DEBUG_RTS( "Add MBTCPSlave Failed: Net %d Slave Array Full\n", emNet+1 );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_ADD_MBTCP_SLAVE_FULL));
                        }
                    }
                    else
                    {
                        DEBUG_RTS( "Net[%d] MBTCPSlave: IP-(0x%08X) Aleady Exist\n", emNet+1, uiIPAddr );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_ADD_MBTCP_SLAVE_EXIST));
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FindModbusTCPSlaveConnInfo
*
* 功能描述: 根据从站IP地址和网口号，寻找相应从站的连接信息结构体
*
* 输入参数: uiIPAddr：从站IP地址；
*          emNet：从站所属的网口号。
*
* 输出参数: 无
*
* 返 回 值: 指向相应从站的连接信息结构体，未找到时，返回NULL
*
* 注意事项: 无
***********************************************************************************************************************
*/

static CliConnSockInfo_t* FindModbusTCPSlaveConnInfo( uint32_t uiIPAddr, NetworkPortType_t emNet )
{
    CliConnSockInfo_t *pstCliInfo = NULL;
    uint8_t ucIndex = 0U;

    if((uiIPAddr != 0) &&
       ((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)))
    {
        for( ucIndex = 0U;  ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pstCliInfo = &( s_stMBTCPSlaveConnSock[emNet][ucIndex]);
            if( uiIPAddr == pstCliInfo->stConnSock.uiIPAddr )
            {
                break;
            }
        }

        if( ucIndex >= (uint8_t)(MAX_MODBUS_SLAVE_NUM))
        {
            pstCliInfo = NULL;
        }
    }

    return pstCliInfo;
}

/*
***********************************************************************************************************************
* 函数名称: FindEmptyModbusTCPSlave
*
* 功能描述: 寻找空闲的从站连接信息结构体
*
* 输入参数: emNet：MBTCP从站所属的网口号
*
* 输出参数: 无
*
* 返 回 值: 指向相应从站的连接信息结构体，未找到时，返回NULL
*
* 注意事项: 无
***********************************************************************************************************************
*/

static CliConnSockInfo_t* FindEmptyModbusTCPSlave( NetworkPortType_t emNet )
{
    CliConnSockInfo_t *pstCliInfo = NULL;
    uint8_t ucIndex = 0U;

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        for( ucIndex = 0U;  ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pstCliInfo = &( s_stMBTCPSlaveConnSock[emNet][ucIndex]);
            if((pstCliInfo->stConnSock.uiIPAddr == 0U) &&
               (pstCliInfo->stConnSock.hSocket == (int32_t)(INVALID_SOCKET)))
            {
                break;
            }
        }

        if( ucIndex >= (uint8_t)(MAX_MODBUS_SLAVE_NUM))
        {
            pstCliInfo = NULL;
        }
    }

    return pstCliInfo;
}

/*
***********************************************************************************************************************
* 函数名称: IsMBTCPSlaveExist
*
* 功能描述: 判断新增MBTCP从站是否已经存在
*
* 输入参数: emNet：MBTCP从站所属的网口号；
*          uiIPAddr：MBTCP从站的IP地址。
*
* 输出参数: 无
*
* 返 回 值: 是否已经存在标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t IsMBTCPSlaveExist( NetworkPortType_t emNet, uint32_t uiIPAddr )
{
    bool_t bExist = false;
    uint8_t ucIndex = 0U;
    CliConnSockInfo_t *pstCliInfo = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       ( uiIPAddr != 0U ))
    {
        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pstCliInfo = &( s_stMBTCPSlaveConnSock[emNet][ucIndex]);
            if( pstCliInfo->stConnSock.uiIPAddr == uiIPAddr )
            {
                bExist = true;
                break;
            }
        }
    }

    return bExist;
}

/*
***********************************************************************************************************************
* 函数名称: ClearModbusTCPSlaveConnInfo
*
* 功能描述: 清除某一被移除从站的连接信息
*
* 输入参数: pstSlaveConnInfo：指向相应被移除从站的连接信息结构体
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearModbusTCPSlaveConnInfo( CliConnSockInfo_t *pstSlaveConnInfo )
{
    if( pstSlaveConnInfo != NULL )
    {
        /* ConnSock */
        /* Disconnect */
        CloseConnSocket( &( pstSlaveConnInfo->stConnSock.hSocket));

        /* Clear Local Info. */
        pstSlaveConnInfo->stConnSock.uiIPAddr = 0U;
        pstSlaveConnInfo->stConnSock.usPort = 0U;
        pstSlaveConnInfo->stConnSock.emNetwork = INVALID_NETWORK_PORT;
        pstSlaveConnInfo->stConnSock.bConnFlag = false;

        /* Wait Info. */
        pstSlaveConnInfo->bWaitConn = false;
        pstSlaveConnInfo->usWaitCnt = 0U;
    }

    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: HandleNTPConfig
*
* 功能描述: 配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: pstNTPInfo：指向本地保存的NTP/TCP配置参数
*
* 输出参数: 无
*
* 返 回 值: 是否切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleNTPConfig( NTPInfo_t *pstNTPInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    NTPTCPConfigInfo_t stNTPCfgInfo;

    if( pstNTPInfo != NULL )
    {
        /* Get CM Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstNTPInfo->uiCMCfgCRC) ||
               (uiFileVer != pstNTPInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysGetNTPTCPConfigInfo( &stNTPCfgInfo ))
                {
                    /* Check If Active & Update(if has flag) */
                    if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                       ( ACTIVE == stNTPCfgInfo.emActive ) &&
                       ( stNTPCfgInfo.bUpdate ))
                    {
                        /* Check Valid */
                        if( CheckNTPCfgInfo( &stNTPCfgInfo ))
                        {
                            /* Switch */
                            if( SwitchNTPCfgInfo( &s_stNTPInfo, &stNTPCfgInfo ))
                            {
                                bSwitch = true;
                                pstNTPInfo->uiCMCfgCRC = uiCRC;
                                pstNTPInfo->uiCMCfgVer = uiFileVer;
                            }
                        }
                        else
                        {
                            pstNTPInfo->uiCMCfgCRC = uiCRC;
                            pstNTPInfo->uiCMCfgVer = uiFileVer;
                            DEBUG_RTS( "NTP Configuration Invalid!\n" );
                        }
                    }
                }
            }
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckNTPCfgInfo
*
* 功能描述: 检查NTP配置信息是否有效
*
* 输入参数: pstNTPCfgInfo：指向待切换的NTP配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckNTPCfgInfo( NTPTCPConfigInfo_t const *pstNTPCfgInfo )
{
    bool_t bValid = false;

    if( pstNTPCfgInfo != NULL )
    {
        /* Active Flag: ACTIVE/NOT_ACTIVE */
        if((ACTIVE == pstNTPCfgInfo->emActive) ||
           (NOT_ACTIVE == pstNTPCfgInfo->emActive))
        {
            bValid = true;
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchNTPCfgInfo
*
* 功能描述: 切换NTP配置信息
*
* 输入参数: pstNTPInfo：指向本地保存的NTP/TCP信息；
*          pstNTPCfgInfo：指向待切换的NTP配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchNTPCfgInfo( NTPInfo_t *pstNTPInfo, NTPTCPConfigInfo_t const *pstNTPCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    NTPTCPConfigInfo_t *pstDstNTPCfgInfo = NULL;

    if((pstNTPInfo != NULL) &&
       (pstNTPCfgInfo != NULL))
    {
        ucCfgIndex = pstNTPInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstNTPCfgInfo = &( pstNTPInfo->stNTPConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstNTPCfgInfo, pstNTPCfgInfo, sizeof(NTPTCPConfigInfo_t));

            /* Update Index */
            pstNTPInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: HandleIPChange
*
* 功能描述: 端口IP地址变化时，进行相应的处理
*
* 输入参数: emNet：网口对应的端口号；
*          pstNetInfo：指向本地保存的网口信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleIPChange( NetworkPortType_t emNet, ExtCommNetworkInfo_t const *pstNetInfo )
{
    DEBUG_RTS( "HandleIPChange: Net %d.\n", emNet+1 );

    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    uint8_t ucIndex = 0U;
    ExtCommNetConfigInfo_t const *pstNetCfg = NULL;
    CliConnSockInfo_t *pstSlaveConnInfo = NULL;
    ConnSockInfo_t *pstConnSock = NULL;

    if((( emNet >= NET1 ) && ( emNet < NUM_OF_NETWORK_PORT )) &&
       ( pstNetInfo != NULL))
    {
        ucCfgIndex = pstNetInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point To Current Configuration */
            pstNetCfg = &( pstNetInfo->stNetConfig[ucCfgIndex]);

            /* Close Network Socket */
            CloseNetworkSocket((uint8_t)emNet );

            /* Clear Recv Info: SOE */
            for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_SOE_NUM); ucIndex++ )
            {
                ClearTCPAppRecvInfo( emNet, s_stSOEConnSock, ucIndex, APP_SOE );
            }

            /* Clear Recv Info: AMS */
            for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_AMS_NUM); ucIndex++ )
            {
                ClearTCPAppRecvInfo( emNet, s_stAMSConnSock, ucIndex, APP_AMS );
            }

            /* Clear Recv Info: Client */
            for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_CLIENT_NUM); ucIndex++ )
            {
                ClearTCPAppRecvInfo( emNet, s_stCliConnSock, ucIndex, APP_CLIENT );
            }

            /* Clear Recv Info: CS1131 */
            for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_CS1131_NUM); ucIndex++ )
            {
                ClearTCPAppRecvInfo( emNet, s_stCS1131ConnSock, ucIndex, APP_CS1131 );
            }

            /* Clear Recv Info: OPC */
            for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_OPC_NUM); ucIndex++ )
            {
                ClearTCPAppRecvInfo( emNet, s_stOPCConnSock, ucIndex, APP_OPC );
            }

            /* Recv Info From Modbus TCP Master */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
            {
                ClearMBTCPSrvRecvInfo( emNet, s_stMBTCPMstrConnSock, ucIndex );
            }

            /* Recv Info From Modbus TCP Slave */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
            {
                ClearMBTCPCliRecvInfo( emNet, s_stMBTCPSlaveConnSock[emNet], ucIndex );
            }

            if( pstNetCfg->uiIPAddr != 0U )
            {
                /* Create Network Socket */
                CreateNetworkSocket( emNet, pstNetCfg->uiIPAddr );

                /* Re-Connect With Pre-Connected Modbus TCP Slave Station */
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
                {
                    pstSlaveConnInfo = &s_stMBTCPSlaveConnSock[emNet][ucIndex];
                    pstConnSock = &( pstSlaveConnInfo->stConnSock );
                    if((pstConnSock->uiIPAddr != 0U) &&
                       (pstConnSock->usPort != 0U) &&
                       (!pstConnSock->bConnFlag))
                    {
                        CliReConnect( pstSlaveConnInfo );
                    }
                }
            }
        }/* end of if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetIPAddr
*
* 功能描述: 获取指定网口的IP地址
*
* 输入参数: emNet：网口对应的端口号；
*          pstNetInfo：指向本地保存的网口信息。
*
* 输出参数: 无
*
* 返 回 值: 网口的IP地址
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint32_t GetIPAddr( NetworkPortType_t emNet )
{
    uint32_t uiIPAddr = 0U;
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    ExtCommNetworkInfo_t const *pstNetInfo = NULL;

    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        pstNetInfo = &( s_stNetInfo[emNet]);
        ucCfgIndex = pstNetInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            uiIPAddr = pstNetInfo->stNetConfig[ucCfgIndex].uiIPAddr;
        }
    }

    return uiIPAddr;
}

/*
***********************************************************************************************************************
* 函数名称: CloseNetworkSocket
*
* 功能描述: 关闭指定网口的套接字
*
* 输入参数: emNet：网口索引值
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CloseNetworkSocket( NetworkPortType_t emNet )
{
    if((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT))
    {
        /* CS1131 Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisCS1131Sock.hSocket ));

        /* OPC Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisOPCSock.hSocket ));

        /* SOE Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisSOESock.hSocket ));

        /* Client Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisCliSock.hSocket ));

        /* AMS Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisAMSSock.hSocket ));

        /* Modbus Listen Socket */
        CloseListenSocket( &(s_stSockInfo[emNet].stLisMBSock.hSocket ));

        /* P2P Socket */
        CloseSocket( &(s_stSockInfo[emNet].stP2PSock.hSocket ));

#if 0
        /* NTP Socket */
        CloseSocket( &(s_stSockInfo[emNet].stNTPSock.hSocket ));
#endif
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CreateNetworkSocket
*
* 功能描述: 创建指定网口的套接字
*
* 输入参数: emNet：网口对应的端口号；
*          uiIPAddr: 网口IP地址。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CreateNetworkSocket( NetworkPortType_t emNet, uint32_t uiIPAddr )
{
    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&  /* Port: NET1-NET2 */
       (uiIPAddr != 0U))
    {
        DEBUG_RTS( "CreateNetworkSocket: Net %d, IP 0x%08X\n", emNet+1, uiIPAddr );

        /* CS1131 Listen Socket */
        CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisCS1131Sock.usPort, \
                            true, &(s_stSockInfo[emNet].stLisCS1131Sock.hSocket));

        /* OPC Listen Socket */
        CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisOPCSock.usPort, \
                            true, &(s_stSockInfo[emNet].stLisOPCSock.hSocket));

        /* SOE Listen Socket */
        CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisSOESock.usPort, \
                            true, &(s_stSockInfo[emNet].stLisSOESock.hSocket));

        /* Client Listen Socket */
        CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisCliSock.usPort, \
                            true, &(s_stSockInfo[emNet].stLisCliSock.hSocket));

        /* AMS Listen Socket */
        CreateListenSocket( uiIPAddr, s_stSockInfo[emNet].stLisAMSSock.usPort, \
                            true, &(s_stSockInfo[emNet].stLisAMSSock.hSocket));

        /* P2P Socket */
        CreateUDPServerSocket( uiIPAddr, s_stSockInfo[emNet].stP2PSock.usPort, \
                               true, &(s_stSockInfo[emNet].stP2PSock.hSocket));

#if 0
        /* NTP Socket: Only NET2 Support */
        if( NET2 == emNet )
        {
            CreateUDPServerSocket( uiIPAddr, s_stSockInfo[emNet].stNTPSock.usPort, \
                                   true, &(s_stSockInfo[emNet].stNTPSock.hSocket));
        }
#endif

        /* Modbus Listen Socket */
        CreateModbusTCPListenSocket( emNet, uiIPAddr, s_stSockInfo[emNet].stLisMBSock.usPort, \
                                     true, &(s_stSockInfo[emNet].stLisMBSock.hSocket));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CreateModbusTCPListenSocket
*
* 功能描述: 根据Modbus TCP配置参数，创建Modbus TCP监听套接字
*
* 输入参数: emNet：网口对应的端口号；
*          uiIPAddr: 网口IP地址；
*          usPort：端口号；
*          socket：监听套接字。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CreateModbusTCPListenSocket( NetworkPortType_t emNet, uint32_t uiIPAddr, uint16_t usPort,
                                         bool_t bNonBlock, SOCKET *phSocket )
{
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    ExtCommModbusTCPConfigInfo_t const *pstMBTCPCfg = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (uiIPAddr != 0U) &&
       (usPort != 0U) &&
       (phSocket != NULL))
    {
        ucCfgIndex = s_stMBTCPInfo[emNet].ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point to Current Configuration */
            pstMBTCPCfg = &( s_stMBTCPInfo[emNet].stMBTCPConfig[ucCfgIndex]);

            /* Handle Modbus TCP Protocol Change */
            if(((uint8_t)(MODBUS_TCP_SLAVE) == pstMBTCPCfg->ucProtocol) ||
               ((uint8_t)(MODBUS_TCP_ALL) == pstMBTCPCfg->ucProtocol))
            {
                CreateListenSocket( uiIPAddr, usPort, bNonBlock, phSocket );
            }
        }
    }

    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: ClearUDPAppRecvInfo
*
* 功能描述: 清除UDP上位机软件的接收信息
*
* 输入参数: emNet：          网口对应的端口号；
*          pstRecvInfo：接收信息数组首地址；
*          ucIndex:     接收信息元素的索引值；
*          emSWType：       软件类型，有效值：APP_SOE/AMS/CLIENT/OPC。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearUDPAppRecvInfo( NetworkPortType_t emNet, UDPRecvInfo_t *pstRecvInfo,
                                 uint8_t ucIndex, SysSoftwareType_t emSWType )
{
    bool_t bClear = false;  /* Clear Flag */

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&  /* Port: NET1-NET2 */
       ( pstRecvInfo != NULL ))
    {
        if(((APP_SOE == emSWType) && (ucIndex < (uint8_t)(APP_SW_SOE_NUM))) ||
           ((APP_AMS == emSWType) && (ucIndex < (uint8_t)(APP_SW_AMS_NUM))) ||
           ((APP_CLIENT == emSWType) && (ucIndex < (uint8_t)(APP_SW_CLIENT_NUM))) ||
           ((APP_OPC == emSWType) && (ucIndex < (uint8_t)(APP_SW_OPC_NUM))))
        {
            if( emNet == pstRecvInfo[ucIndex].emNetwork )  /* Same Port */
            {
                pstRecvInfo[ucIndex].bConnFlag = false;    /* Reset Local Connect Flag */

                /* Set Disconnect and Clear if not lock */
                if( SysGetAppSwLockedFlag( emSWType, ucIndex ))
                {
                    bClear = false;
                }
                else
                {
                    bClear = true;  /* Non-Lock: Clear */
                }

                if( bClear )
                {
                    /* Reset Connect Flag */
                    if( !SysResetAppSwConnectFlag( emSWType, ucIndex ))
                    {
                        DEBUG_RTS( "ClearUDPAppRecvInfo: Reset Connect Flag Failed!\n" );
                    }

                    SysClearAppMsg( emSWType, ucIndex, APP_MSG_REQ );  /* Clear Request Message */
                    pstRecvInfo[ucIndex].uiIPAddr = 0U;
                    pstRecvInfo[ucIndex].usPort = 0U;
                    pstRecvInfo[ucIndex].emNetwork = INVALID_NETWORK_PORT;
                }
            }
        }
    }

    return;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: ClearTCPAppRecvInfo
*
* 功能描述: 清除TCP上位机软件的接收信息
*
* 输入参数: emNet：          网口对应的端口号；
*          pstRecvInfo：接收信息数组首地址；
*          ucIndex:     接收信息元素的索引值；
*          emSWType：       软件类型，有效值：APP_CS1131/APP_OPC。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearTCPAppRecvInfo( NetworkPortType_t emNet, ConnSockInfo_t *pstRecvInfo,
                                 uint8_t ucIndex, SysSoftwareType_t emSWType )
{
    bool_t bClear = false;  /* Clear Flag */

    if(((emNet >= NET1) && (emNet < NET1+NUM_OF_NETWORK_PORT)) &&  /* Port: NET1-NET2 */
       ( pstRecvInfo != NULL ))
    {
        if(((APP_CS1131 == emSWType) && (ucIndex < (uint8_t)(APP_SW_CS1131_NUM))) ||
           ((APP_OPC == emSWType) && (ucIndex < (uint8_t)(APP_SW_OPC_NUM))) ||
           ((APP_SOE == emSWType) && (ucIndex < (uint8_t)(APP_SW_SOE_NUM))) ||
           ((APP_AMS == emSWType) && (ucIndex < (uint8_t)(APP_SW_AMS_NUM))) ||
           ((APP_CLIENT == emSWType) && (ucIndex < (uint8_t)(APP_SW_CLIENT_NUM))))
        {
            if( emNet == pstRecvInfo[ucIndex].emNetwork )  /* Same Port */
            {
                pstRecvInfo[ucIndex].bConnFlag = false;             /* Reset Local Connect Flag */
                CloseConnSocket( &(pstRecvInfo[ucIndex].hSocket));  /* Close Connect Socket */

                /* Set Disconnect and Clear if not lock */
                if( SysGetAppSwLockedFlag( emSWType, ucIndex ))
                {
                    bClear = false;
                }
                else
                {
                    bClear = true;  /* Non-Lock: Clear */
                }

                if( bClear )
                {
                    /* Reset Connect Flag */
                    if( !SysResetAppSwConnectFlag( emSWType, ucIndex ))
                    {
                        DEBUG_RTS( "ClearTCPAppRecvInfo: Reset Connect Flag Failed\n" );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCONN));
                    }

                    SysClearAppMsg( emSWType, ucIndex, APP_MSG_REQ );  /* Clear Request Message */
                    pstRecvInfo[ucIndex].uiIPAddr = 0U;
                    pstRecvInfo[ucIndex].usPort = 0U;
                    pstRecvInfo[ucIndex].emNetwork = INVALID_NETWORK_PORT;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearMBTCPSrvRecvInfo
*
* 功能描述: 清除来自Modbus TCP主站的接收信息
*
* 输入参数: emNet：          网口号；
*          pstRecvInfo：接收信息数组首地址；
*          ucIndex:     接收信息元素的索引值。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearMBTCPSrvRecvInfo( NetworkPortType_t emNet, ConnSockInfo_t pstRecvInfo[], uint8_t ucIndex )
{
    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&  /* Port: NET1-NET2 */
       ( pstRecvInfo != NULL ) &&
       ( ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT)))
    {
        if( emNet == pstRecvInfo[ucIndex].emNetwork )  /* Same Port */
        {
            /* Disconnect */
            CloseConnSocket( &( pstRecvInfo[ucIndex].hSocket));

            /* Clear Recv Info */
            pstRecvInfo[ucIndex].bConnFlag = false;
            pstRecvInfo[ucIndex].uiIPAddr = 0U;
            pstRecvInfo[ucIndex].usPort = 0U;
            pstRecvInfo[ucIndex].emNetwork = INVALID_NETWORK_PORT;
            SysClearModbusTCPMasterMsg( ucIndex );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearMBTCPCliRecvInfo
*
* 功能描述: 清除来自 Modbus TCP 从站的接收信息
*
* 输入参数: emNet：          网口对应的端口号；
*          pstRecvInfo：接收信息数组首地址；
*          ucIndex:     接收信息元素的索引值。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearMBTCPCliRecvInfo( NetworkPortType_t emNet, CliConnSockInfo_t pstRecvInfo[], uint8_t ucIndex )
{
    ConnSockInfo_t *pstConnSock = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&  /* Port: NET1-NET2 */
       (pstRecvInfo != NULL) &&
       (ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM)))
    {
        pstConnSock = &pstRecvInfo[ucIndex].stConnSock;
        if( emNet == pstConnSock->emNetwork )  /* Same Port */
        {
            /* Clear */
            pstConnSock->bConnFlag = false;
            CloseConnSocket( &( pstConnSock->hSocket));

            pstRecvInfo[ucIndex].bWaitConn = false;
            pstRecvInfo[ucIndex].usWaitCnt = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCliConnStatus
*
* 功能描述: 检查客户端与服务器端的连接是否建立成功
*
* 输入参数: pstCliInfo：客户端连接信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CheckCliConnStatus( CliConnSockInfo_t *pstCliInfo )
{
    fd_set  fsFd;         /* Socket Queue */
    FD_ZERO( &fsFd );
    struct timeval stTV;  /* Time out */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;    /* us */
    int32_t iValOpt = 0;
    uint32_t uiLength = 0U;
    SOCKET hConnSock = (SOCKET)( INVALID_SOCKET );
    ConnSockInfo_t *pstConnSock = NULL;
    bool_t bCheckTimeout = false;

    if( pstCliInfo != NULL )
    {
        pstConnSock = &( pstCliInfo->stConnSock );
        hConnSock = pstConnSock->hSocket;
        if( hConnSock != (SOCKET)( INVALID_SOCKET ))
        {
            /* Check Connect Status: Successfully, Failed or Timeout */
            FD_SET( hConnSock, &fsFd );
            if( select( hConnSock+1, NULL, &fsFd, NULL, &stTV ) > 0 )
            {
                uiLength = sizeof(int32_t);
                if( getsockopt( hConnSock, SOL_SOCKET, SO_ERROR, (void*)(&iValOpt), &uiLength ) != -1 )
                {
                    if( 0 == iValOpt )
                    {
                        /* Connect Successfully */
                        DEBUG_RTS( "Connect To IP(0x%08X) OK\n", pstConnSock->uiIPAddr );
                        pstConnSock->bConnFlag = true;
                        pstCliInfo->bWaitConn = false;
                        pstCliInfo->usWaitCnt = 0U;
                    }
                    else
                    {
                        /* Connect Failed */
                        DEBUG_RTS( "Connect To IP(0x%08X) Failed-%d\n", pstConnSock->uiIPAddr, iValOpt );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_CLIENT_CONN));
                        CloseSocket( &( pstConnSock->hSocket ));
                        pstCliInfo->bWaitConn = false;
                        pstCliInfo->usWaitCnt = 0U;
                    }
                }
                else
                {
                    /* getsockopt error */
                    bCheckTimeout = true;
                    DEBUG_RTS( "CheckCliConnStatus-IP(0x%08X): getsockopt Err-%d\n", \
                               pstConnSock->uiIPAddr, GetErrorNo());
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_GETSOCKOPT));
                }
            }
            else
            {
                /* select Timeout/Error */
                bCheckTimeout = true;
                DEBUG_RTS( "CheckCliConnStatus-IP(0x%08X): Select Timeout/Err, E-%d\n", \
                           pstConnSock->uiIPAddr, GetErrorNo());
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
            }

            if( bCheckTimeout )
            {
                pstCliInfo->usWaitCnt++;
                if( pstCliInfo->usWaitCnt >= (uint16_t)(CONN_TIMEOT_CYCLE_CNT))
                {
                    /* Connect Timeout */
                    DEBUG_RTS( "CheckCliConnStatus-IP(0x%08X): Connect Timeout\n", pstConnSock->uiIPAddr );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_CLIENT_CONN_TO));
                    CloseSocket( &( pstConnSock->hSocket ));
                    pstCliInfo->bWaitConn = false;
                    pstCliInfo->usWaitCnt = 0U;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CliReConnect
*
* 功能描述: 重新与服务器端建立连接
*
* 输入参数: pstCliInfo：客户端连接信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CliReConnect( CliConnSockInfo_t *pstCliInfo )
{
    SOCKET hSocket = (SOCKET)( INVALID_SOCKET );
    ConnSockInfo_t *pstConnSock = NULL;

    if( pstCliInfo != NULL )
    {
        pstConnSock = &( pstCliInfo->stConnSock );
        CloseConnSocket( &( pstConnSock->hSocket));

        /* Connect */
        if( Connect( pstConnSock->uiIPAddr, (uint16_t)(PORTNUM_MODBUS), true, &hSocket ))
        {
            /* Connect Successfully */
            DEBUG_RTS( "CliReConnect To IP-0x%08X OK\n", pstConnSock->uiIPAddr );
            pstConnSock->bConnFlag = true;
            pstCliInfo->bWaitConn = false;
            pstCliInfo->usWaitCnt = 0U;
        }
        else
        {
            /* Connect Failed / In Progress */
            pstConnSock->bConnFlag = false;
            pstCliInfo->usWaitCnt = 0U;

            if( hSocket != INVALID_SOCKET )
            {
                /* Connect In Progress: Wait Connect Finished */
                pstCliInfo->bWaitConn = true;
            }
            else
            {
                /* Connect Failed: Re-Connected */
                pstCliInfo->bWaitConn = false;
            }
        }

        /* Set Other Local Info. */
        pstConnSock->hSocket = hSocket;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MaintainStatus
*
* 功能描述: 维护与Modbus TCP从站的连接；根据上位机应用模块设置的标志，进行相应的操作
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

static void MaintainStatus( void )
{
    NetworkPortType_t emNet = NET1;
    uint8_t ucIndex = 0U;
    CliConnSockInfo_t *pstCliInfo = NULL;
    ConnSockInfo_t *pstConnSock = NULL;

    /* Maintain the connect status with Modbus TCP Slave: reconnect when disconnected. */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pstCliInfo = &( s_stMBTCPSlaveConnSock[emNet][ucIndex]);
            pstConnSock = &( pstCliInfo->stConnSock );
            if((pstConnSock->uiIPAddr != 0U) &&
               (pstConnSock->usPort != 0U) &&
               (!pstConnSock->bConnFlag))  /* Connect In Progress / Error */
            {
                if( pstCliInfo->bWaitConn )
                {
                    /* Connect In Progress: Check If Finished */
                    CheckCliConnStatus( pstCliInfo );
                }
                else
                {
                    /* Connect Error: Reconnect */
                    CliReConnect( pstCliInfo );
                }
            }
        }
    }

    /* PC Software: Clear local software receive info. */
    /* and reset connect flag in system when Clear flag was set. */
    /* SOE */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_SOE_NUM); ucIndex++ )
    {
        if( SysGetAppSwClearFlag( APP_SOE, ucIndex ))
        {
            DEBUG_RTS( "Clear And Disconnect With SOE-%d\n", ucIndex );

            if( !SysResetAppSwClearFlag( APP_SOE, ucIndex ))
            {
                DEBUG_RTS( "Reset SOE-%d Clear Flag Failed\n", ucIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCLR));
            }

            ClearTCPAppRecvInfo( s_stSOEConnSock[ucIndex].emNetwork, s_stSOEConnSock, ucIndex, APP_SOE );
        }
    }

    /* AMS */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_AMS_NUM); ucIndex++ )
    {
        if( SysGetAppSwClearFlag( APP_AMS, ucIndex ))
        {
            DEBUG_RTS( "Clear And Disconnect With AMS-%d\n", ucIndex );

            if( !SysResetAppSwClearFlag( APP_AMS, ucIndex ))
            {
                DEBUG_RTS( "Reset AMS-%d Clear Flag Failed\n", ucIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCLR));
            }

            ClearTCPAppRecvInfo( s_stAMSConnSock[ucIndex].emNetwork, s_stAMSConnSock, ucIndex, APP_AMS );
        }
    }

    /* Client */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_CLIENT_NUM); ucIndex++ )
    {
        if( SysGetAppSwClearFlag( APP_CLIENT, ucIndex ))
        {
            DEBUG_RTS( "Clear And Disconnect With Client-%d\n", ucIndex );

            if( !SysResetAppSwClearFlag( APP_CLIENT, ucIndex ))
            {
                DEBUG_RTS( "Reset Client-%d Clear Flag Failed\n", ucIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCLR));
            }

            ClearTCPAppRecvInfo( s_stCliConnSock[ucIndex].emNetwork, s_stCliConnSock, ucIndex, APP_CLIENT );
        }
    }

    /* CS1131 */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_CS1131_NUM); ucIndex++ )
    {
        if( SysGetAppSwClearFlag( APP_CS1131, ucIndex ))
        {
            DEBUG_RTS( "Clear And Disconnect With CS1131-%d\n", ucIndex );

            if( !SysResetAppSwClearFlag( APP_CS1131, ucIndex ))
            {
                DEBUG_RTS( "Reset CS1131-%d Clear Flag Failed\n", ucIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCLR));
            }

            ClearTCPAppRecvInfo( s_stCS1131ConnSock[ucIndex].emNetwork, s_stCS1131ConnSock, ucIndex, APP_CS1131 );
        }
    }

    /* OPC */
    for( ucIndex = 0U; ucIndex < (uint8_t)(APP_SW_OPC_NUM); ucIndex++ )
    {
        if( SysGetAppSwClearFlag( APP_OPC, ucIndex ))
        {
            DEBUG_RTS( "Clear And Disconnect With OPC-%d\n", ucIndex );

            if( !SysResetAppSwClearFlag( APP_OPC, ucIndex ))
            {
                DEBUG_RTS( "Reset OPC-%d Clear Flag Failed\n", ucIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_RST_SWCLR));
            }

            ClearTCPAppRecvInfo( s_stOPCConnSock[ucIndex].emNetwork, s_stOPCConnSock, ucIndex, APP_OPC );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: AcceptNewConnect
*
* 功能描述: 监听连接请求，接收新连接，将得到的套接字放入相应的数组中
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

static void AcceptNewConnect( void )
{
    struct timeval stTV;     /* Time out */
    fd_set  fsFd;            /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );
    bool_t bFDEmpty = true;  /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;     /* Max FDP of the socket queue */
    int32_t iResult = 0;
    SOCKET hLisSock = (int32_t)(INVALID_SOCKET);
    NetworkPortType_t emNet = NET1;

    /* Update Socket Queue */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        /* CS1131 Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisCS1131Sock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }

        /* OPC Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisOPCSock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }

        /* SOE Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisSOESock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }

        /* Client Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisCliSock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }

        /* AMS Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisAMSSock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }

        /* Modbus TCP Listen Socket */
        hLisSock = s_stSockInfo[emNet].stLisMBSock.hSocket;
        if( hLisSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hLisSock, &fsFd );  /* Add Valid Listen Socket */
            bFDEmpty = false;
            if( hLisSock > iMaxFDP )
            {
                iMaxFDP = hLisSock;
            }
        }
    }/* for */

    if( !bFDEmpty )
    {
        iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error: Record errno */
            DEBUG_RTS( "AcceptNewConnect: Select Error %d\n", GetErrorNo());
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
        }
        else if( 0 == iResult )
        {
            /* Select Timeout: donot need to do anything */
#if 0
            DEBUG_RTS( "AcceptNewConnect: Select Timeout - No Connect Request\n" );
#endif
        }
        else
        {
            /* Select Success */
            for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
            {
                /* CS1131 Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisCS1131Sock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    /* Has Client Connect */
                    AcceptConnect( hLisSock, emNet, TYPE_CS1131, s_stCS1131ConnSock, (uint8_t)(APP_SW_CS1131_NUM));
                }

                /* OPC Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisOPCSock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    /* Has Client Connect */
                    AcceptConnect( hLisSock, emNet, TYPE_OPC, s_stOPCConnSock, (uint8_t)(APP_SW_OPC_NUM));
                }

                /* SOE Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisSOESock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    /* Has Client Connect */
                    AcceptConnect( hLisSock, emNet, TYPE_SOE, s_stSOEConnSock, (uint8_t)(APP_SW_SOE_NUM));
                }

                /* Client Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisCliSock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    /* Has Client Connect */
                    AcceptConnect( hLisSock, emNet, TYPE_CLIENT, s_stCliConnSock, (uint8_t)(APP_SW_CLIENT_NUM));
                }

                /* AMS Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisAMSSock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    /* Has Client Connect */
                    AcceptConnect( hLisSock, emNet, TYPE_AMS, s_stAMSConnSock, (uint8_t)(APP_SW_AMS_NUM));
                }

                /* Modbus TCP Listen Socket */
                hLisSock = s_stSockInfo[emNet].stLisMBSock.hSocket;
                if( FD_ISSET( hLisSock, &fsFd ))
                {
                    AcceptConnect( hLisSock, emNet, TYPE_MBTCP_MSTR, s_stMBTCPMstrConnSock,
                                   (uint8_t)(MAX_MBTCP_MSTR_CNT));
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: AcceptConnect
*
* 功能描述: 监听连接请求，接收新连接，将得到的套接字放入相应的数组中
*
* 输入参数: hLisSock：监听套接字；
*          emNet：网口索引值；
*          emType：通讯类型-TYPE_CS1131/ OPC / AMS / Client/ SOE/ TYPE_MODBUS_TCP_MASTER；
*          pstConnSock：接收信息数组首地址；
*          ucMaxConnCount：接收信息数组元素的最大个数。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void AcceptConnect( SOCKET hLisSock, NetworkPortType_t emNet, CommType_t emType,
                           ConnSockInfo_t *pstConnSock, uint8_t ucMaxConnCount )
{
    SOCKADDR_IN stAddr;    /* Address */
    int32_t iAddrLen = 0;  /* Address Length */
    int32_t iErrno = 0;
    SOCKET hConnSock = (int32_t)(INVALID_SOCKET);
    uint8_t ucConnIndex = 0U;
    uint8_t ucRetryCnts = 0U;
    bool_t bValidConn = false;
    uint32_t uiAMIPAddr = 0U;  /* Allowed MBTCP Master IP Address */
    SysSoftwareType_t emSWType = -1;

    if((hLisSock != (int32_t)(INVALID_SOCKET)) &&
       ((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstConnSock != NULL))
    {
        if(((TYPE_CS1131 == emType) && (ucMaxConnCount <= (uint8_t)(APP_SW_CS1131_NUM))) ||
           ((TYPE_OPC == emType) && (ucMaxConnCount <= (uint8_t)(APP_SW_OPC_NUM))) ||
           ((TYPE_SOE == emType) && (ucMaxConnCount <= (uint8_t)(APP_SW_SOE_NUM))) ||
           ((TYPE_CLIENT == emType) && (ucMaxConnCount <= (uint8_t)(APP_SW_CLIENT_NUM))) ||
           ((TYPE_AMS == emType) && (ucMaxConnCount <= (uint8_t)(APP_SW_AMS_NUM))) ||
           ((TYPE_MBTCP_MSTR == emType) && (ucMaxConnCount <= (uint8_t)(MAX_MBTCP_MSTR_CNT))))
        {
            emSWType = SwitchToSWType( emType );

            while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
            {
                /* Initialize Address */
                iAddrLen = (int32_t)sizeof( SOCKADDR_IN );
                memset( &stAddr, 0, (uint32_t)iAddrLen );
                hConnSock = accept( hLisSock, (SOCKADDR*)&stAddr, (socklen_t*)&iAddrLen );
                if( hConnSock != (int32_t)(INVALID_SOCKET))
                {
                    /* accept successfully */
                    ucRetryCnts = 0U;  /* Clear retry counts */

                    /* Network-MBTCP Slave: Check if allow communication */
                    if( TYPE_MBTCP_MSTR == emType )
                    {
                        uiAMIPAddr = GetMBAllowMstrIPAddr( &s_stMBTCPInfo[emNet]);
                        if((0U == uiAMIPAddr) || (ntohl(stAddr.sin_addr.s_addr) == uiAMIPAddr))
                        {
                            /* Allow Connection */
                            bValidConn = true;
                        }
                        else
                        {
                            bValidConn = false;
                            CloseConnSocket( &hConnSock );  /* Dis-Conncet */
                            DEBUG_RTS( "MBTCP: Not Allow Connect, AllowIPAddr-0x%04X, MstrIPAddr-0x%04X\n", \
                                       uiAMIPAddr, ntohl(stAddr.sin_addr.s_addr));
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_MBTCP_SLAVE_NOT_ALLOW_CONN));
                        }
                    }
                    else
                    {
                        bValidConn = true;
                    }

                    if( bValidConn )
                    {
                        /* Add to corresponding array */
                        for( ucConnIndex = 0U; ucConnIndex < ucMaxConnCount; ucConnIndex++ )
                        {
                            if((0U == pstConnSock[ucConnIndex].uiIPAddr) &&
                               (0U == pstConnSock[ucConnIndex].usPort))
                            {
                                /* Available Item */
                                if( !SetNonBlockMode( hConnSock ))  /* Non-Block Mode */
                                {
                                    DEBUG_RTS( "AcceptConnect: Net[%d] SetNonBlockMode Failed\n", emNet+1 );
                                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SETNONBLKMODE));
                                }

                                pstConnSock[ucConnIndex].hSocket = hConnSock;
                                pstConnSock[ucConnIndex].uiIPAddr = ntohl(stAddr.sin_addr.s_addr);  /* Network To Host Order*/
                                pstConnSock[ucConnIndex].usPort = ntohs(stAddr.sin_port);           /* Network To Host Order*/
                                pstConnSock[ucConnIndex].emNetwork = emNet + NET1;
                                pstConnSock[ucConnIndex].bConnFlag = true;

                                /* Set App Connect Flag */
                                if( emSWType != -1 )
                                {
                                    if( SysSetAppSwConnectFlag( emSWType, ucConnIndex ))
                                    {
                                        DEBUG_RTS( "AcceptConnect: Net[%d] SW-%d Connect, Type %d, IP %X, Port %d\n", \
                                                   emNet+1, ucConnIndex, emSWType, \
                                                   pstConnSock[ucConnIndex].uiIPAddr, pstConnSock[ucConnIndex].usPort );
                                    }
                                    else
                                    {
                                        DEBUG_RTS( "AcceptConnect: Net[%d] SysSetAppSwConnectFlag Failed\n", \
                                                   emNet+1 );
                                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_SET_SWCONN));
                                    }
                                }
                                else
                                {
                                    /* From Modbus TCP Master */
                                    DEBUG_RTS( "AcceptConnect: Net[%d] MBTCPMstr-%d Connect, IP 0x%X, Port %d\n", \
                                               emNet+1, ucConnIndex, pstConnSock[ucConnIndex].uiIPAddr, \
                                               pstConnSock[ucConnIndex].usPort );
                                }

                                break;
                            }
                        }/* end of for */

                        /* Check if add successfully */
                        if( ucConnIndex == ucMaxConnCount )
                        {
                            /* Add Failed: Array is full. */
                            CloseConnSocket( &hConnSock );

                            /* Record Error Info. */
                            DEBUG_RTS( "Local Array For CommType-%d Full\n", emType );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_ACCEPT_CONN_FULL));

                            break;
                        }
                    }
                }/* end of if( hConnSock != (int32_t)(INVALID_SOCKET)) */
                else
                {
                    /* Error Handling */
                    iErrno = GetErrorNo();
                    if((EAGAIN == iErrno) ||
                       (EWOULDBLOCK == iErrno))
                    {
                        /* The socket is marked nonblocking and no connections are present to be accepted. */
                        DEBUG_RTS( "AcceptConnect: Net[%d] Accept done\n", emNet+1 );
                        break;
                    }
                    else
                    {
                        if((EINTR == iErrno) ||
                           (ENETDOWN == iErrno) ||
                           (EPROTO == iErrno) ||
                           (ENOPROTOOPT == iErrno) ||
                           (EHOSTDOWN == iErrno) ||
                           (ENONET == iErrno) ||
                           (EHOSTUNREACH == iErrno) ||
                           (EOPNOTSUPP == iErrno) ||
                           (ENETUNREACH == iErrno))
                        {
                            /* Retry: These errno except EINTR are already-pending network errors on the new socket, */
                            /* and only in linux can be passed by accept function. */
                            DEBUG_RTS( "AcceptConnect: Net[%d] Accept retry-%d\n", emNet+1, ucRetryCnts );
                            ucRetryCnts++;
                        }
                        else
                        {
                            DEBUG_RTS( "AcceptConnect: Net[%d] Accept Err-%d\n", emNet+1, iErrno );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_ACCEPT));
                            break;
                        }
                    }
                }/* end of Error Handling */
            }/* end of while */
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadNetMsg
*
* 功能描述: 接收并处理网络消息
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

static void ReadNetMsg( void )
{
    ReadCS1131Msg();           /* Data From CS1131 */
    ReadOPCMsg();              /* Data From OPC/HMI */
    ReadSOEMsg();              /* Data From SOE */
    ReadAMSMsg();              /* Data From AMS */
    ReadClientMsg();           /* Data From Client */
    ReadModbusTCPMasterMsg();  /* Data From Modbus TCP Master */
    ReadModbusTCPSlaveMsg();   /* Data From Modbus TCP Slave */
    ReadP2PMsg();              /* Data From Send/Recv Station */
    //ReadNTPMsg();            /* Data From SNTP/NTP Master/Slave */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadCS1131Msg
*
* 功能描述: 接收来自CS1131软件的请求消息，消息有效时，写入系统资源中
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

static void ReadCS1131Msg( void )
{
    ReadAPPTCPMsg( APP_CS1131, (uint8_t)(APP_SW_CS1131_NUM), s_stCS1131ConnSock, s_ucRecvBuff, &s_stNetMsg );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadOPCMsg
*
* 功能描述: 接收来自OPC软件的请求消息，消息有效时，写入系统资源中
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

static void ReadOPCMsg( void )
{
    ReadAPPTCPMsg( APP_OPC, (uint8_t)(APP_SW_OPC_NUM), s_stOPCConnSock, s_ucRecvBuff, &s_stNetMsg );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadSOEMsg
*
* 功能描述: 接收来自SOE软件的请求消息，消息有效时，写入系统资源中
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

static void ReadSOEMsg( void )
{
    ReadAPPTCPMsg( APP_SOE, (uint8_t)(APP_SW_SOE_NUM), s_stSOEConnSock, s_ucRecvBuff, &s_stNetMsg );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadAMSMsg
*
* 功能描述: 接收来自AMS软件的请求消息，消息有效时，写入系统资源中
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

static void ReadAMSMsg( void )
{
    ReadAPPTCPMsg( APP_AMS, (uint8_t)(APP_SW_AMS_NUM), s_stAMSConnSock, s_ucRecvBuff, &s_stNetMsg );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadClientMsg
*
* 功能描述: 接收来自Client软件的请求消息，消息有效时，写入系统资源中
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

static void ReadClientMsg( void )
{
    ReadAPPTCPMsg( APP_CLIENT, (uint8_t)(APP_SW_CLIENT_NUM), s_stCliConnSock, s_ucRecvBuff, &s_stNetMsg );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadAPPTCPMsg
*
* 功能描述: 接收TCP软件的请求消息，消息有效时，写入系统资源中
*
* 输入参数: emSWType：软件类型；
*          ucSWNum：软件数目；
*          pstConnArr：软件对应的连接套接字信息数组；
*          pucRecvBuff：接收缓冲区；
*          pstNetMsg：用于存放完整的网络消息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ReadAPPTCPMsg( SysSoftwareType_t emSWType, uint8_t ucSWNum, ConnSockInfo_t pstConnArr[],
                           uint8_t pucRecvBuff[], NetMsg_t *pstNetMsg )
{
    struct timeval stTV;     /* Time out */
    fd_set  fsFd;            /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );        /* Clear Socket Queue */
    bool_t bFDEmpty = true;  /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;     /* Max FDP of the socket queue */
    int32_t iResult = 0;     /* Select Result */
    uint8_t ucSWIndex = 0U;  /* Software Index */
    uint16_t usLen = 0U;     /* Length */
    uint16_t usMsgLen = 0U;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

#if 0
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
    uint16_t usPrintLen = 0U;
#endif

    if((pstConnArr != NULL) &&
       (pucRecvBuff != NULL) &&
       (pstNetMsg != NULL))
    {
        if(((APP_CS1131 == emSWType) && (ucSWNum <= (uint8_t)(APP_SW_CS1131_NUM))) ||
           ((APP_OPC == emSWType) && (ucSWNum <= (uint8_t)(APP_SW_OPC_NUM))) ||
           ((APP_SOE == emSWType) && (ucSWNum <= (uint8_t)(APP_SW_SOE_NUM))) ||
           ((APP_CLIENT == emSWType) && (ucSWNum <= (uint8_t)(APP_SW_CLIENT_NUM))) ||
           ((APP_AMS == emSWType) && (ucSWNum <= (uint8_t)(APP_SW_AMS_NUM))))
        {
            /* Update Socket Queue */
            for( ucSWIndex = 0U; ucSWIndex < ucSWNum; ucSWIndex++ )
            {
                hSock = pstConnArr[ucSWIndex].hSocket;
                if( hSock != (int32_t)(INVALID_SOCKET))
                {
                    FD_SET( hSock, &fsFd );  /* Add Valid Connect Socket */
                    bFDEmpty = false;
                    if( hSock > iMaxFDP )
                    {
                        iMaxFDP = hSock;
                    }
                }
            }

            if( !bFDEmpty )
            {
                iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
                if((int32_t)(SOCKET_ERROR) == iResult )
                {
                    /* Select Error: Only Record Errno */
                    DEBUG_RTS( "ReadAPPTCPMsg: Select Err %d\n", GetErrorNo());
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
                }
                else if( 0 == iResult )
                {
                    /* Select Timeout: No new data need to be received */
#if 0
                    DEBUG_RTS( "ReadAPPTCPMsg: No New Data\n" );
#endif
                }
                else
                {
                    /* Select Successfully */
                    for( ucSWIndex = 0U; ucSWIndex < ucSWNum; ucSWIndex++ )
                    {
                        hSock = pstConnArr[ucSWIndex].hSocket;
                        if( hSock != (int32_t)(INVALID_SOCKET))
                        {
                            if( FD_ISSET( hSock, &fsFd ))  /* Has Data */
                            {
                                while( 1 )
                                {
                                    /* Read Message Header */
                                    if( ReadAPPTCPData( emSWType, pstConnArr, ucSWIndex, \
                                                        pucRecvBuff, (uint32_t)(APP_MSG_HEADER_LEN)))
                                    {
                                        /* Read Message DataArea and CRC */
                                        /* Get Length: Header + DataArea */
                                        usLen = (pucRecvBuff[APP_REQ_LEN_HIINDEX] << 8) |
                                                 pucRecvBuff[APP_REQ_LEN_LOWINDEX];
                                        if((usLen >= (uint16_t)(APP_MSG_HEADER_LEN)) &&
                                           (usLen <= (uint16_t)(MAX_MSG_LEN) - (uint16_t)(APP_MSG_CRC_LEN)))
                                        {
                                            /* Calc Length: Data Area + CRC */
                                            usLen -= (uint16_t)(APP_MSG_HEADER_LEN);
                                            usLen += (uint16_t)(APP_MSG_CRC_LEN);
                                            if( ReadAPPTCPData( emSWType, pstConnArr, ucSWIndex, \
                                                                &pucRecvBuff[APP_MSG_HEADER_LEN], (uint32_t)usLen ))
                                            {
                                                /* Calc msg length */
                                                usMsgLen = (uint16_t)(APP_MSG_HEADER_LEN) + usLen;

                                                /* Check valid */
                                                if( CheckAppMsgValid( pucRecvBuff, usMsgLen ))
                                                {
                                                    DEBUG_RTS( "ReadAPPTCPMsg: Msg Valid, SW Type-%d\n", emSWType );

#if 0
                                                    /* print message */
                                                    if( APP_CS1131 == emSWType )
                                                    {
                                                        printf( "CS1131 Msg Len: %d, Data(Top 50B): \n", usMsgLen );
                                                        usPrintLen = usMsgLen;
                                                        if( usPrintLen > 50U )
                                                        {
                                                            usPrintLen = 50U;
                                                        }

                                                        for( usIndex = 0U; usIndex < usPrintLen; usIndex++ )
                                                        {
                                                            printf( "%02X ", pucRecvBuff[usIndex]);
                                                            usPerLen++;

                                                            if((30 == usPerLen) ||
                                                               ((usPrintLen-1) == usIndex))
                                                            {
                                                                printf( "\n" );
                                                                usPerLen = 0U;
                                                            }
                                                        }
                                                    }
#endif

                                                    /* Write to system resource */
                                                    pstNetMsg->stNetHead.uiIPAddr = pstConnArr[ucSWIndex].uiIPAddr;
                                                    pstNetMsg->stNetHead.usPort = pstConnArr[ucSWIndex].usPort;
                                                    pstNetMsg->stNetHead.emNetwork = pstConnArr[ucSWIndex].emNetwork;
                                                    pstNetMsg->stNetHead.usLen = usMsgLen;
                                                    memcpy( pstNetMsg->ucBuff, pucRecvBuff, (uint32_t)usMsgLen );

                                                    if( !SysSetAppMsg( emSWType, ucSWIndex, APP_MSG_REQ, \
                                                                       (puint8_t)pstNetMsg, \
                                                                       (uint16_t)sizeof(NetMsgHead_t) + usMsgLen))
                                                    {
                                                        DEBUG_RTS( "ReadAPPTCPMsg: SysSetAppMsg Failed\n" );
                                                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_SET_APPMSG));
                                                    }
                                                    else
                                                    {
                                                        DEBUG_RTS( "ReadAPPTCPMsg: SW %d SysSetAppMsg OK\n", emSWType );
                                                    }
                                                }
                                                else
                                                {
                                                    DEBUG_RTS( "ReadAPPTCPMsg: MSG Invalid\n" );
                                                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_APP_MSG));
                                                }
                                            }/* end of Read Non header */
                                            else
                                            {
                                                DEBUG_RTS( "ReadAPPTCPMsg: Read Data Area & CRC Failed\n" );
                                                LogWrite( (uint32_t)(LOGID_EXCOMM_E_READ_APP_MSG_CONTENT));
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            DEBUG_RTS( "ReadAPPTCPMsg: MSG Head Len %d Invalid\n", usLen );
                                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_APP_MSG_LEN));
                                        }
                                    }/* end of Read Header */
                                    else
                                    {
                                        break;  /* Read Done */
                                    }
                                }/* end of while */
                            }/* end of Has Data */
                        }/* end of  if( hSock != (int32_t)(INVALID_SOCKET))*/
                    }/* end of for */
                }/* end of select success */
            }/* end of if( !bFDEmpty ) */
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadTCPAPPData
*
* 功能描述: 读取来自应用软件的数据，根据读操作的结果进行必要的错误处理
*
* 输入参数: emSWType：软件类型：CS1131/OPC；
*          pstConnArr：连接套接字信息数组；
*          ucSWIndex：CS1131软件索引值；
*          pucBuff：接收缓冲区；
*          uiDataLen：目标读取字节数。
*
* 输出参数: 无
*
* 返 回 值: true：读取成功；false:读取失败/读空
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadAPPTCPData( SysSoftwareType_t emSWType, ConnSockInfo_t pstConnArr[], uint8_t ucSWIndex,
                              uint8_t pucBuff[], uint32_t uiDataLen )
{
    bool_t bResult = false;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    int32_t iReadLen = 0;
    int32_t iErrno = 0;
    uint8_t ucRetryCnts = 0U;
    uint32_t uiIndex = 0U;

    if((pstConnArr != NULL) &&
       (pucBuff != NULL) &&
       (uiDataLen > 0U ))
    {
        if(((APP_CS1131 == emSWType) && (ucSWIndex <= (uint8_t)(APP_SW_CS1131_NUM))) ||
           ((APP_OPC == emSWType) && (ucSWIndex <= (uint8_t)(APP_SW_OPC_NUM))) ||
           ((APP_SOE == emSWType) && (ucSWIndex <= (uint8_t)(APP_SW_SOE_NUM))) ||
           ((APP_CLIENT == emSWType) && (ucSWIndex <= (uint8_t)(APP_SW_CLIENT_NUM))) ||
           ((APP_AMS == emSWType) && (ucSWIndex <= (uint8_t)(APP_SW_AMS_NUM))))
        {
            hSock = pstConnArr[ucSWIndex].hSocket;
            if( hSock != (int32_t)(INVALID_SOCKET))
            {
                while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
                {
                    iReadLen = recv( hSock, &pucBuff[uiIndex], uiDataLen, 0 );
                    if((int32_t)(SOCKET_ERROR) == iReadLen )
                    {
                        /* Error Handling */
                        iErrno = GetErrorNo();  /* errno of recv function */
                        if((EAGAIN == iErrno) || (EWOULDBLOCK == iErrno))
                        {
                            /* Non Block Socket: EAGAIN/EWOULDBLOCK - Read done */
                            /* EAGAIN's value maybe same or not same as EWOULDBLOCK */
                            DEBUG_RTS( "ReadAPPTCPData: Recv Done\n" );
                            break;
                        }
                        else if( EINTR == iErrno )
                        {
                            /* EINTR: The receive was interrupted by delivery of a signal
                               before any data were available */
                            DEBUG_RTS( "ReadAPPTCPData: EINTR, Retry %d\n", ucRetryCnts );
                            ucRetryCnts++;
                        }
                        else
                        {
                            /* Recv Error */
                            DEBUG_RTS( "ReadAPPTCPData: Recv Failed %d\n", iErrno );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_APPDATA));
                            ClearTCPAppRecvInfo( pstConnArr[ucSWIndex].emNetwork, pstConnArr, ucSWIndex, emSWType );
                            break;
                        }
                    }/* end of Error Handling */
                    else if( 0 == iReadLen )
                    {
                        /* Has Disconnected */
                        DEBUG_RTS( "ReadAPPTCPData: Recv Failed - Disconnected\n" );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_APPDATA_DISCONN));
                        ClearTCPAppRecvInfo( pstConnArr[ucSWIndex].emNetwork, pstConnArr, ucSWIndex, emSWType );
                        break;
                    }
                    else
                    {
                        /* recv successfully */
                        uiDataLen -= (uint32_t)iReadLen;
                        if( 0U == uiDataLen )
                        {
                            bResult = true;  /* Read Done */
                            break;
                        }
                        else
                        {
                            DEBUG_RTS( "ReadAPPTCPData: Retry %d\n", ucRetryCnts );
                            ucRetryCnts++;
                            uiIndex += (uint32_t)iReadLen;
                        }
                    }
                }/* end of while */
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadP2PMsg
*
* 功能描述: 接收来自其他控制站的消息，消息有效时，写入系统资源中
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

static void ReadP2PMsg( void )
{
    struct timeval stTV;     /* Time out */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    fd_set  fsFd;            /* Socket Queue */
    FD_ZERO( &fsFd );
    bool_t bFDEmpty = true;   /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;      /* Max FDP of the socket queue */
    int32_t iResult = 0;      /* Select Result */
    NetworkPortType_t emNet = NET1;  /* Network Index */
    uint16_t usMsgLen = 0U;
    SOCKADDR_IN stSrcAddr;
    SysP2PMsgType_t emSysP2PMsg = P2P_EXT_REQ;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

    /* Update Socket Queue */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        hSock = s_stSockInfo[emNet].stP2PSock.hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hSock, &fsFd );  /* Add Valid Server Socket */
            bFDEmpty = false;
            if( hSock > iMaxFDP )
            {
                iMaxFDP = hSock;
            }
        }
    }

    if( !bFDEmpty )
    {
        iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error */
            DEBUG_RTS( "ReadP2PMsg: SelErr %d\n", GetErrorNo());
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
        }
        else if( 0 == iResult )
        {
            /* Select Timeout: No new data need to be received */
#if 0
            DEBUG_RTS( "ReadP2PMsg: No New Data\n" );
#endif
        }
        else
        {
            /* Select Successfully */
            for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
            {
                hSock = s_stSockInfo[emNet].stP2PSock.hSocket;
                if( hSock != (int32_t)(INVALID_SOCKET))
                {
                    if( FD_ISSET( hSock, &fsFd ))
                    {
                        /* Has Data */
                        while( 1 )
                        {
                            /* Read UDP Message */
                            memset( &stSrcAddr, 0, sizeof(SOCKADDR_IN));
                            usMsgLen = (uint16_t)sizeof( s_ucRecvBuff );
                            if( ReadSingleP2PMsg( hSock, s_ucRecvBuff, &usMsgLen, (uint16_t)(MAX_P2P_MSG_LEN), \
                                                  (SOCKADDR*)&stSrcAddr ))
                            {
                                DEBUG_RTS( "ReadP2PMsg: ReadSingleP2PMsg OK\n" );

                                /* Check Valid */
                                if( CheckP2PMsgValid( s_ucRecvBuff, usMsgLen ))
                                {
                                    /* Write to system resource: P2P_EXT_REQ / P2P_EXT_RESP */
                                    if( PMP2P_REQ == s_ucRecvBuff[P2P_TYPE_INDEX])
                                    {
                                        emSysP2PMsg = P2P_EXT_REQ;
                                    }
                                    else
                                    {
                                        emSysP2PMsg = P2P_EXT_RESP;
                                    }

                                    if( !SysSetStationMsg( emSysP2PMsg, s_ucRecvBuff, usMsgLen ))
                                    {
                                        DEBUG_RTS( "SysSetStationMsg Failed! MSG Type %d\n", emSysP2PMsg );
                                    }
                                    else
                                    {
                                        DEBUG_RTS( "MSG Type[%d] Valid. Write to system\n", emSysP2PMsg );
                                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_SET_P2PMSG));
                                    }
                                }
                                else
                                {
                                    /* Message Invalid */
                                    DEBUG_RTS( "ReadP2PMsg: MSG Invalid\n" );
                                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_P2P_MSG));
                                }
                            }
                            else
                            {
                                break;  /* Read Done */
                            }
                        }/* end of while */
                    }/* end of if( FD_ISSET( hSock, &fsFd )) */
                }/* end of if( hSock != (int32_t)(INVALID_SOCKET)) */
            }/* end of for */
        }/* end of select successfully */
    }/* end of if( !bFDEmpty ) */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadSingleP2PMsg
*
* 功能描述: 读取单一P2P-UDP消息
*
* 输入参数: hSock：UDP套接字；
*          pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度；
*          usMaxMsgLen：目标消息的最大字节数；
*          pstSrcAddr: 用于存放源地址。
*
* 输出参数: pusLen：返回UDP消息的实际长度；
*          pstSrcAddr：返回源地址。
*
* 返 回 值: true：读取成功；false：读取失败/读空
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadSingleP2PMsg( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                SOCKADDR *pstSrcAddr )
{
    bool_t bResult = false;
    uint16_t usIndex = 0U;
    uint16_t usBuffLen = 0U;
    uint16_t usMaxLen = 0U;
    uint16_t usTotalLen = 0U;

    if((hSock != (int32_t)(INVALID_SOCKET)) &&
       (pucBuff != NULL) &&
       (pusLen != NULL) &&
       (usMaxMsgLen > 0U) &&
       (pstSrcAddr != NULL))
    {
        usBuffLen = *pusLen;
        usMaxLen = usMaxMsgLen;

        if( ReadUDPHeaderAtLeast( hSock, pucBuff, pusLen, usMaxMsgLen, pstSrcAddr ))
        {
            /* Check if read done */
            usTotalLen = (pucBuff[P2P_LEN_HIINDEX] << 8) | pucBuff[P2P_LEN_LOWINDEX];
            if( *pusLen == usTotalLen )
            {
                /* Read Done */
                bResult = true;
            }
            else
            {
                /* Calculate Last Length */
                usIndex = *pusLen;  /* Readen Length */
                usBuffLen -= usIndex;
                usMaxLen = usTotalLen - usIndex;

                /* Read Last UDP Data */
                memset( pstSrcAddr, 0, sizeof(SOCKADDR_IN));
                if( ReadLastUDPData( hSock, &pucBuff[usIndex], &usBuffLen, usMaxLen, pstSrcAddr ))
                {
                    /* Read Done */
                    bResult = true;
                    *pusLen = usIndex + usBuffLen;
                }
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadUDPHeaderAtLeast
*
* 功能描述: 至少读出UDP消息头
*
* 输入参数: hSock：UDP套接字；
*          pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度；
*          usMaxMsgLen：目标消息的最大字节数；
*          pstSrcAddr: 用于存放源地址。
*
* 输出参数: pusLen：返回实际接收的长度；
*          pstSrcAddr：返回源地址。
*
* 返 回 值: true：读取成功；false：读取失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadUDPHeaderAtLeast( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                    SOCKADDR *pstSrcAddr )
{
    bool_t bResult = false;
    uint8_t ucRetryCnts = 0U;
    uint32_t uiSrcAddrLen = 0U;
    uint16_t usIndex = 0U;
    uint16_t usBuffLen = 0U;
    uint16_t usMaxLen = 0U;
    uint16_t usLen = 0U;
    int32_t iReadLen = 0;

    if((hSock != (int32_t)(INVALID_SOCKET)) &&
       (pucBuff != NULL) &&
       (pusLen != NULL) &&
       (usMaxMsgLen > 0U) &&
       (pstSrcAddr != NULL))
    {
        usBuffLen = *pusLen;
        usMaxLen = usMaxMsgLen;

        if( usBuffLen >= usMaxLen )
        {
            /* Read Additional Info. At Least */
            while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
            {
                usLen = usBuffLen;
                uiSrcAddrLen = sizeof( SOCKADDR_IN );
                iReadLen = recvfrom( hSock, &pucBuff[usIndex], (uint32_t)usMaxLen, 0, \
                                     pstSrcAddr, (socklen_t*)&uiSrcAddrLen );
                if((int32_t)(SOCKET_ERROR) != iReadLen )
                {
                    usLen = (uint16_t)iReadLen;
                    usIndex += usLen;
                    usMaxLen -= usLen;
                    usBuffLen -= usLen;

                    if( usIndex >= (uint32_t)(P2P_MSG_ADDL_LEN))
                    {
                        bResult = true;
                        *pusLen = usIndex;
                        break;
                    }
                }
                else
                {
                    ucRetryCnts++;
                }
            }/* end of while */
        }
        else
        {
            DEBUG_RTS( "ReadUDPHeaderAtLeast: RecvBuffLen[%d] should >= MaxMsgLen[%d]\n", usBuffLen, usMaxLen );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadLastUDPData
*
* 功能描述: 读取剩余的UDP数据
*
* 输入参数: hSock：UDP套接字；
*          pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度；
*          usLastLen：剩余字节数；
*          pstSrcAddr: 用于存放源地址。
*
* 输出参数: pusLen：返回接收到的实际长度；
*          pstSrcAddr：返回源地址。
*
* 返 回 值: true：读取成功；false:读取失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadLastUDPData( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usLastLen, \
                               SOCKADDR *pstSrcAddr )
{
    bool_t bResult = false;
    uint8_t ucRetryCnts = 0U;
    uint32_t uiSrcAddrLen = 0U;
    uint16_t usIndex = 0U;
    uint16_t usBuffLen = 0U;
    uint16_t usMaxLen = 0U;
    uint16_t usLen = 0U;
    int32_t iReadLen = 0;

    if((hSock != (int32_t)(INVALID_SOCKET)) &&
       (pucBuff != NULL) &&
       (pusLen != NULL) &&
       (usLastLen > 0U) &&
       (pstSrcAddr != NULL))
    {
        usBuffLen = *pusLen;
        usMaxLen = usLastLen;

        if( usBuffLen >= usMaxLen )
        {
            /* Read Additional Info. At Least */
            while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
            {
                usLen = usBuffLen;
                uiSrcAddrLen = sizeof( SOCKADDR_IN );
                iReadLen = recvfrom( hSock, &pucBuff[usIndex], (uint32_t)usMaxLen, 0, \
                                     pstSrcAddr, (socklen_t*)&uiSrcAddrLen );
                if((int32_t)(SOCKET_ERROR) != iReadLen )
                {
                    usLen = (uint16_t)iReadLen;
                    usIndex += usLen;
                    usMaxLen -= usLen;
                    usBuffLen -= usLen;

                    if( usIndex == usLastLen )
                    {
                        bResult = true;
                        *pusLen = usIndex;
                        break;
                    }
                }
                else
                {
                    ucRetryCnts++;
                }
            }/* end of while */
        }
        else
        {
            DEBUG_RTS( "ReadLastUDPData: RecvBuffLen[%d] should >= MaxMsgLen[%d]\n", usBuffLen, usMaxLen );
        }
    }

    return bResult;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: ReadNTPMsg
*
* 功能描述: 接收SNTP/NTP校时消息，消息有效时，写入系统资源中
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 只有NET2支持SNTP/NTP校时
***********************************************************************************************************************
*/

static void ReadNTPMsg( void )
{
    struct timeval stTV;     /* Time out */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    fd_set  fsFd;            /* Socket Queue */
    FD_ZERO( &fsFd );
    bool_t bFDEmpty = true;  /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;     /* Max FDP of the socket queue */
    int32_t iResult = 0;     /* Select Result */
    SOCKADDR_IN stSrcAddr;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    NetworkPortType_t emNet = NET2;

#if 0
    uint16_t usLen = 0U;
    uint16_t usMsgLen = 0U;
    SysNTPMsgType_t emSysNTPMsg = NTP_MSG_REQ;
#endif

    /* Update Socket Queue */
    hSock = s_stSockInfo[emNet].stNTPSock.hSocket;
    if( hSock != (int32_t)(INVALID_SOCKET))
    {
        FD_SET( hSock, &fsFd );  /* Add Valid Server Socket */
        bFDEmpty = false;
        if( hSock > iMaxFDP )
        {
            iMaxFDP = hSock;
        }
    }

    if( !bFDEmpty )
    {
        iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error */
            DEBUG_RTS( "ReadNTPMsg: Select Error - %d!\n", GetErrorNo());
        }
        else if( 0 == iResult )
        {
            /* Select Timeout: No new data need to be received */
#if 0
            DEBUG_RTS( "ReadNTPMsg: No New Data.\n" );
#endif
        }
        else
        {
            /* Select Successfully */
            while( 1 )
            {
                /* Read UDP Message */
                memset( &stSrcAddr, 0, sizeof(SOCKADDR_IN));
#if 0
                usMsgLen = (uint16_t)sizeof( s_ucRecvBuff );
                if( ReadSingleNTPMsg( hSock, s_ucRecvBuff, &usMsgLen, (uint32_t)(MAX_NTP_MSG_LEN), \
                                      (SOCKADDR*)&stSrcAddr ))
                {
                    DEBUG_RTS( "ReadNTPMsg: Read Successfully.\n" );

                    /* Check Valid */
                    if( CheckNTPMsgValid( s_ucRecvBuff, usMsgLen ))
                    {
                        /* Write to system resource: REQ / RESP */
                        if( XXX_REQ == s_ucRecvBuff[NTP_TYPE_INDEX])
                        {
                            emSysNTPMsg = NTP_MSG_REQ;
                        }
                        else
                        {
                            emSysNTPMsg = NTP_MSG_RESP;
                        }

                        if( !SysSetNTPMsg( emSysNTPMsg, s_ucRecvBuff, usMsgLen ))
                        {
                            DEBUG_RTS( "ReadNTPMsg: SysSetNTPMsg Failed! Message Type-%d.\n", \
                                       emSysNTPMsg );
                        }
                        else
                        {
                            DEBUG_RTS( "ReadNTPMsg: Message(Type[%d]) valid, and write to system.\n", \
                                       emSysNTPMsg );
                        }
                    }
                    else
                    {
                        /* Message Invalid */
                        DEBUG_RTS( "ReadNTPMsg: Message Invalid!\n" );
                    }
                }
                else
                {
                    break;  /* Read Done */
                }
#endif
            }/* end of while */
        }/* end of select successfully */
    }/* end of if( !bFDEmpty ) */

    return;
}
#endif

#if 0
/*
***********************************************************************************************************************
* 函数名称: ReadSingleNTPMsg
*
* 功能描述: 读取单一NTP-UDP消息
*
* 输入参数: hSock：UDP套接字；
*          pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度；
*          usMaxMsgLen：目标消息的最大字节数；
*          pstSrcAddr: 用于存放源地址。
*
* 输出参数: pusLen：返回UDP消息的实际长度；
*          pstSrcAddr：返回源地址。
*
* 返 回 值: true：读取成功；false：读取失败/读空
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t ReadSingleNTPMsg( SOCKET hSock, uint8_t pucBuff[], uint16_t *pusLen, uint16_t usMaxMsgLen, \
                                SOCKADDR *pstSrcAddr )
{
    return false;

#if 0
    /* Read UDP Message */
    memset( &stSrcAddr, 0, sizeof(SOCKADDR_IN));
    if( ReadSingleP2PMsg( hSock, s_ucRecvBuff, (uint32_t)(SNTP_MSG_HEADER_LEN), (SOCKADDR*)&stSrcAddr ))
    {
        DEBUG_RTS( "ReadNTPMsg: Read Header Successfully.\n" );

        /* Read Non-header data */
        /* Get Length:  */
        /*usLen = (s_ucRecvBuff[SNTP_LEN_HIINDEX] << 8) |
                   s_ucRecvBuff[SNTP_LEN_LOWINDEX];*/
        if( usLen >= (uint16_t)((SNTP_MSG_HEADER_LEN) + (SNTP_MSG_CRC_LEN)))
        {
            /* Calc Length: Non-header data */
            /*usLen = usLen - SNTP_MSG_HEADER_LEN;*/
            if( ReadSingleP2PMsg( hSock, &s_ucRecvBuff[SNTP_MSG_HEADER_LEN], (uint32_t)usLen, \
                             (SOCKADDR*)&stSrcAddr ))
            {
                DEBUG_RTS( "ReadNTPMsg: Read Non-header data Successfully.\n" );

                /* Calc msg length */
                /*usMsgLen = SNTP_MSG_HEADER_LEN + usLen;*/

                /* Check valid */
                if( CheckNTPMsgValid( s_ucRecvBuff, usMsgLen ))
                {
                    DEBUG_RTS( "ReadNTPMsg: Message valid, and write to system.\n" );


                }
                else
                {
                    /* Message Invalid */
                    DEBUG_RTS( "ReadNTPMsg: Message Invalid!\n" );
                }
            }/* end of Read Data Area & CRC */
            else
            {
                /* Read Error */
                DEBUG_RTS( "ReadNTPMsg: Read Non-header data Failed!\n" );
                break;
            }
        }
        else
        {
            /* Message Header Invalid */
            DEBUG_RTS( "ReadNTPMsg: Length-%d Invalid!\n", usLen );
        }
    }/* end of Read Header */
    else
    {
        break;  /* Read Done */
    }
#endif
}
#endif

/*
***********************************************************************************************************************
* 函数名称: ReadModbusTCPMasterMsg
*
* 功能描述: 接收来自Modbus TCP主站的请求消息，消息有效时，写入系统资源中
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

static void ReadModbusTCPMasterMsg( void )
{
    struct timeval stTV;     /* Time out */
    fd_set  fsFd;            /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );
    bool_t bFDEmpty = true;  /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;     /* Max FDP of the socket queue */
    int32_t iResult = 0;     /* Select Result */
    uint8_t ucIndex = 0U;
    uint16_t usLen = 0U;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

#if 0
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    /* Update Socket Queue */
    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
    {
        hSock = s_stMBTCPMstrConnSock[ucIndex].hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            FD_SET( hSock, &fsFd );  /* Add Valid Connect Socket */
            bFDEmpty = false;
            if( hSock > iMaxFDP )
            {
                iMaxFDP = hSock;
            }
        }
    }

    if( !bFDEmpty )
    {
        iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error: Only Record Errno */
            DEBUG_RTS( "ReadMBTCPMstrMsg: SelErr %d\n", GetErrorNo());
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
        }
        else if( 0 == iResult )
        {
#if 0
            /* Select Timeout: No new data need to be received */
            DEBUG_RTS( "ReadModbusTCPMasterMsg: No New Data.\n" );
#endif
        }
        else
        {
            /* Select Successfully */
            for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
            {
                hSock = s_stMBTCPMstrConnSock[ucIndex].hSocket;
                if( hSock != (int32_t)(INVALID_SOCKET))
                {
                    if( FD_ISSET( hSock, &fsFd ))  /* Has Data */
                    {
                        while( 1 )
                        {
                            /* Read Message Header */
                            if( ReadModbusTCPMasterData( ucIndex, s_ucRecvBuff, (uint32_t)(MBTCP_HEADER_LEN)))
                            {
                                /* Read Message Non-header data */
                                /* Get Length: following bytes - UID(1) + Funccode(1) + Data */
                                usLen = (s_ucRecvBuff[MBAP_LEN_H_INDEX] << 8) | s_ucRecvBuff[MBAP_LEN_L_INDEX];
                                if((usLen > 2U) && (usLen <= (uint16_t)(MAX_MBTCP_LEN_VALUE)))
                                {
                                    if( ReadModbusTCPMasterData( ucIndex, &s_ucRecvBuff[MBTCP_HEADER_LEN], \
                                                                 (uint32_t)(usLen-1U)))
                                    {
                                        /* Calc msg length */
                                        usLen = (uint16_t)(MBTCP_HEADER_LEN) + usLen - 1;

#if 0
                                        /* Print Request Message */
                                        {
                                            printf( "MBTCP Request Message Valid, Length: %d, Content: \n", usLen );

                                            for( usIndex = 0U; usIndex < usLen; usIndex++ )
                                            {
                                                printf( "%02X ", s_ucRecvBuff[usIndex]);
                                                usPerLen++;

                                                if((30 == usPerLen) ||
                                                   ((usLen-1) == usIndex))
                                                {
                                                    printf( "\n" );
                                                    usPerLen = 0U;
                                                }
                                            }
                                        }
#endif

                                        /* Write to system resource */
                                        s_stMBTCPMsg.stNetHead.uiIPAddr = s_stMBTCPMstrConnSock[ucIndex].uiIPAddr;
                                        s_stMBTCPMsg.stNetHead.usPort = s_stMBTCPMstrConnSock[ucIndex].usPort;
                                        s_stMBTCPMsg.stNetHead.emNetwork = s_stMBTCPMstrConnSock[ucIndex].emNetwork;
                                        s_stMBTCPMsg.stNetHead.usLen = usLen;
                                        memcpy( s_stMBTCPMsg.ucBuff, s_ucRecvBuff, (uint32_t)usLen );

                                        if( !SysSetModbusTCPMasterMsg( ucIndex, (puint8_t)&s_stMBTCPMsg, \
                                                                       (uint16_t)sizeof(NetMsgHead_t) + usLen ))
                                        {
                                            DEBUG_RTS( "SysSetMBTCPMstrMsg Failed, Mstr Index %d\n", ucIndex );
                                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_SET_MBTCPMSTRMSG));
                                        }
                                        else
                                        {
                                            DEBUG_RTS( "Write MBTCPReqMsg to system OK\n" );
                                        }
                                    }
                                    else
                                    {
                                        /* Read Error */
                                        DEBUG_RTS( "ReadMBTCPMstrMsg: Read Non-head data Failed\n" );
                                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_READ_MBTCPMSTR_MSG_CONTENT));
                                        break;
                                    }
                                }
                                else
                                {
                                    /* Message Header Invalid */
                                    DEBUG_RTS( "ReadMBTCPMstrMsg: Len %d Invalid\n", usLen );
                                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_MBTCPMSTR_MSG_LEN));
                                }
                            }/* end of Read Header */
                            else
                            {
                                break;  /* Read Done */
                            }
                        }/* end of while */
                    }/* end of Has Data */
                }/* end of  if( hSock != (int32_t)(INVALID_SOCKET))*/
            }/* end of for */
        }/* end of select success */
    }/* end of if( !bFDEmpty ) */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadModbusTCPSlaveMsg
*
* 功能描述: 接收来自Modbus TCP从站的应答消息，消息有效时，写入系统资源中
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

static void ReadModbusTCPSlaveMsg( void )
{
    struct timeval stTV;     /* Time out */
    fd_set  fsFd;            /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );
    bool_t bFDEmpty = true;  /* Socket Queue Empty Flag */
    int32_t iMaxFDP = 0;     /* Max FDP of the socket queue */
    int32_t iResult = 0;     /* Select Result */
    uint8_t ucIndex = 0U;
    NetworkPortType_t emNet = NET1;
    uint16_t usLen = 0U;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    ConnSockInfo_t *pstConnSock = NULL;

#if 0
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    /* Update Socket Queue */
    for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
    {
        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pstConnSock = &( s_stMBTCPSlaveConnSock[emNet][ucIndex].stConnSock );
            hSock = pstConnSock->hSocket;
            if(( hSock != (int32_t)(INVALID_SOCKET)) &&
               ( pstConnSock->bConnFlag ))
            {
                FD_SET( hSock, &fsFd );  /* Add Valid Connect Socket */
                bFDEmpty = false;
                if( hSock > iMaxFDP )
                {
                    iMaxFDP = hSock;
                }
            }
        }
    }

    if( !bFDEmpty )
    {
        iResult = select( iMaxFDP+1, &fsFd, NULL, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error: Only Record Errno */
            DEBUG_RTS( "ReadMBTCPSlaveMsg: SelErr %d\n", GetErrorNo());
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
        }
        else if( 0 == iResult )
        {
#if 0
            /* Select Timeout: No new data need to be received */
            DEBUG_RTS( "ReadModbusTCPSlaveMsg: No New Data.\n" );
#endif
        }
        else
        {
            /* Select Successfully */
            for( emNet = NET1; emNet < NUM_OF_NETWORK_PORT; emNet++ )
            {
                for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT); ucIndex++ )
                {
                    pstConnSock = &( s_stMBTCPSlaveConnSock[emNet][ucIndex].stConnSock );
                    hSock = pstConnSock->hSocket;
                    if(( hSock != (int32_t)(INVALID_SOCKET)) &&
                       ( pstConnSock->bConnFlag ))
                    {
                        if( FD_ISSET( hSock, &fsFd ))  /* Has Data */
                        {
                            while( 1 )
                            {
                                /* Read Message Header */
                                if( ReadModbusTCPSlaveData( emNet, ucIndex, s_ucRecvBuff, (uint32_t)(MBTCP_HEADER_LEN)))
                                {
                                    /* Read Message Non-header data */
                                    /* Get Length: following bytes - UID(1) + Funccode(1) + Data */
                                    usLen = (s_ucRecvBuff[MBAP_LEN_H_INDEX] << 8) | s_ucRecvBuff[MBAP_LEN_L_INDEX];
                                    if((usLen > 2U) && (usLen <= (uint16_t)(MAX_MBTCP_LEN_VALUE)))
                                    {
                                        if( ReadModbusTCPSlaveData( emNet, ucIndex, &s_ucRecvBuff[MBTCP_HEADER_LEN], \
                                                                    (uint32_t)(usLen-1U)))
                                        {
                                            /* Calc msg length */
                                            usLen = (uint16_t)(MBTCP_HEADER_LEN) + usLen - 1;

#if 0
                                            /* Print Response Message */
                                            {
                                                printf( "MBTCPRespMsg Valid, Len: %d, Data: \n", usLen );

                                                for( usIndex = 0U; usIndex < usLen; usIndex++ )
                                                {
                                                    printf( "%02X ", s_ucRecvBuff[usIndex]);
                                                    usPerLen++;

                                                    if((30 == usPerLen) ||
                                                       ((usLen-1) == usIndex))
                                                    {
                                                        printf( "\n" );
                                                        usPerLen = 0U;
                                                    }
                                                }
                                            }
#endif

                                            /* Write to system resource */
                                            if( !SysSetModbusRespMsg( emNet, s_ucRecvBuff, usLen ))
                                            {
                                                DEBUG_RTS( "SysSetMBRespMsg Failed, Net[%d]\n", emNet+1 );
                                                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SYS_SET_MBTCPSLAVEMSG));
                                            }
                                            else
                                            {
                                                DEBUG_RTS( "Write MBTCPRespMsg to system OK\n" );
                                            }
                                        }
                                        else
                                        {
                                            /* Read Error */
                                            DEBUG_RTS( "ReadMBTCPSlaveMsg: Read Non-head data Failed\n" );
                                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_READ_MBTCPSLAVE_MSG_CONTENT));
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        /* Message Header Invalid */
                                        DEBUG_RTS( "ReadMBTCPSlaveMsg: Len %d Invalid\n", usLen );
                                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_MBTCPSLAVE_MSG_LEN));
                                    }
                                }/* end of Read Header */
                                else
                                {
                                    break;  /* Read Done */
                                }
                            }/* end of while */
                        }/* end of Has Data */
                    }/* end of  if( hSock != (int32_t)(INVALID_SOCKET))*/
                }/* end of for */
            }/* end of for net */
        }/* end of select success */
    }/* end of if( !bFDEmpty ) */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ReadModbusTCPMasterData
*
* 功能描述: 读取来自 Modbus TCP主站的数据，根据读操作的结果进行必要的错误处理
*
* 输入参数: ucIndex： Modbus TCP主站的索引值；
*          pucBuff：接收缓冲区；
*          uiDataLen：目标读取字节数。
*
* 输出参数: 无
*
* 返 回 值: true：读取成功；false:读取失败/读空
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadModbusTCPMasterData( uint8_t ucIndex, uint8_t pucBuff[], uint32_t uiDataLen )
{
    bool_t bResult = false;
    int32_t iReadLen = 0;
    int32_t iErrno = 0;
    uint8_t ucRetryCnts = 0U;
    uint32_t uiIndex = 0U;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

    if(( ucIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT)) &&
       ( pucBuff != NULL) &&
       ( uiDataLen > 0U ))
    {
        hSock = s_stMBTCPMstrConnSock[ucIndex].hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
            {
                iReadLen = recv( hSock, &pucBuff[uiIndex], uiDataLen, 0 );
                if((int32_t)(SOCKET_ERROR) == iReadLen )
                {
                    /* Error Handling */
                    iErrno = GetErrorNo();  /* errno of recv function */
                    if((EAGAIN == iErrno) || (EWOULDBLOCK == iErrno))
                    {
                        /* Non Block Socket: EAGAIN/EWOULDBLOCK - Read done */
                        /* EAGAIN's value maybe same or not same as EWOULDBLOCK */
                        DEBUG_RTS( "ReadMBTCPMstrData: recv done\n" );
                        break;
                    }
                    else if( EINTR == iErrno )
                    {
                        /* EINTR: The receive was interrupted by delivery of a signal
                           before any data were available */
                        DEBUG_RTS( "ReadMBTCPMstrData: recv EINTR, retry %d\n", ucRetryCnts );
                        ucRetryCnts++;
                    }
                    else
                    {
                        /* Recv Error */
                        ClearMBTCPSrvRecvInfo( s_stMBTCPMstrConnSock[ucIndex].emNetwork, \
                                                s_stMBTCPMstrConnSock, ucIndex );
                        DEBUG_RTS( "ReadMBTCPMstrData: recv failed %d\n", iErrno );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_MBTCPMSTRDATA));
                        break;
                    }
                }/* end of Error Handling */
                else if( 0 == iReadLen )
                {
                    /* Has Disconnected */
                    DEBUG_RTS( "ReadMBTCPMstrData: recv failed - Disconnected\n" );
                    ClearMBTCPSrvRecvInfo( s_stMBTCPMstrConnSock[ucIndex].emNetwork, \
                                            s_stMBTCPMstrConnSock, ucIndex );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_MBTCPMSTR_DISCONN));
                    break;
                }
                else
                {
                    /* recv successfully */
                    uiDataLen -= (uint32_t)iReadLen;
                    if( 0U == uiDataLen )
                    {
                        bResult = true;  /* Successfully */
                        break;
                    }
                    else
                    {
                        DEBUG_RTS( "ReadMBTCPMstrData: Retry %d\n", ucRetryCnts );
                        ucRetryCnts++;
                        uiIndex += (uint32_t)iReadLen;
                    }
                }
            }/* end of while */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadModbusTCPSlaveData
*
* 功能描述: 读取来自 Modbus TCP从站的数据，根据读操作的结果进行必要的错误处理
*
* 输入参数: emNet：Modbus TCP从站所属的网口索引值；
*          ucIndex： Modbus TCP从站的索引值；
*          pucBuff：接收缓冲区；
*          uiDataLen：目标读取字节数。
*
* 输出参数: 无
*
* 返 回 值: true：读取成功；false:读取失败/读空
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadModbusTCPSlaveData( NetworkPortType_t emNet, uint8_t ucIndex, uint8_t pucBuff[], uint32_t uiDataLen )
{
    bool_t bResult = false;
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    int32_t iReadLen = 0;
    int32_t iErrno = 0;
    uint8_t ucRetryCnts = 0U;
    uint32_t uiIndex = 0U;
    ConnSockInfo_t *pstConnSock = NULL;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM)) &&
       (pucBuff != NULL) &&
       (uiDataLen > 0U))
    {
        pstConnSock = &( s_stMBTCPSlaveConnSock[emNet][ucIndex].stConnSock );
        hSock = pstConnSock->hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
            {
                iReadLen = recv( hSock, &pucBuff[uiIndex], uiDataLen, 0 );
                if((int32_t)(SOCKET_ERROR) == iReadLen )
                {
                    /* Error Handling */
                    iErrno = GetErrorNo();  /* errno of recv function */
                    if((EAGAIN == iErrno) || (EWOULDBLOCK == iErrno))
                    {
                        /* Non Block Socket: EAGAIN/EWOULDBLOCK - Read done */
                        /* EAGAIN's value maybe same or not same as EWOULDBLOCK */
                        DEBUG_RTS( "ReadMBTCPSlaveData: recv done\n" );
                        break;
                    }
                    else if( EINTR == iErrno )
                    {
                        /* EINTR: The receive was interrupted by delivery of a signal
                           before any data were available */
                        DEBUG_RTS( "ReadMBTCPSlaveData: recv EINTR, retry %d\n", ucRetryCnts );
                        ucRetryCnts++;
                    }
                    else
                    {
                        /* Recv Error */
                        ClearMBTCPCliRecvInfo( s_stMBTCPSlaveConnSock[emNet][ucIndex].stConnSock.emNetwork, \
                                               s_stMBTCPSlaveConnSock[emNet], ucIndex );
                        DEBUG_RTS( "ReadMBTCPSlaveData: recv failed %d\n", iErrno );
                        LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_MBTCPSLAVEDATA));
                        break;
                    }
                }/* end of Error Handling */
                else if( 0 == iReadLen )
                {
                    /* Has Disconnected */
                    DEBUG_RTS( "ReadMBTCPSlaveData: recv failed - Disconnected\n" );
                    ClearMBTCPCliRecvInfo( s_stMBTCPSlaveConnSock[emNet][ucIndex].stConnSock.emNetwork, \
                                           s_stMBTCPSlaveConnSock[emNet], ucIndex );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_RECV_MBTCPSLAVE_DISCONN));
                    break;
                }
                else
                {
                    /* recv successfully */
                    uiDataLen -= (uint32_t)iReadLen;
                    if( 0U == uiDataLen )
                    {
                        bResult = true;  /* Successfully */
                        break;
                    }
                    else
                    {
                        DEBUG_RTS( "ReadMBTCPSlaveData: Retry %d\n", ucRetryCnts );
                        ucRetryCnts++;
                        uiIndex += (uint32_t)iReadLen;
                    }
                }
            }/* end of while */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CheckAppMsgValid
*
* 功能描述: 检查来自上位机软件的消息是否有效
*
* 输入参数: pucMsg：消息缓冲区；
*          usMsgLen：消息长度。
*
* 输出参数: 无
*
* 返 回 值: true：消息有效；false：消息无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckAppMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen )
{
    bool_t bResult = false;
    uint32_t uiCalcCRC = 0U;
    uint32_t uiMsgCRC = 0U;
    uint16_t usCRCIndex = 0U;

    if((pucMsg != NULL) &&
       (usMsgLen >= ((uint16_t)(APP_MSG_HEADER_LEN) + APP_MSG_CRC_LEN)))
    {
        usCRCIndex = usMsgLen - (uint16_t)(APP_MSG_CRC_LEN);

        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucMsg, (uint32_t)usCRCIndex );

        /* Message CRC: High Byte First */
        uiMsgCRC = (pucMsg[usCRCIndex] << 24) |
                   (pucMsg[usCRCIndex+1U] << 16) |
                   (pucMsg[usCRCIndex+2U] << 8) |
                    pucMsg[usCRCIndex+3U];

        /* Compare CRC: If CRC Valid, true */
        if( uiCalcCRC == uiMsgCRC )
        {
            bResult = true; /* CRC Valid */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CheckP2PMsgValid
*
* 功能描述: 检查P2P消息是否有效
*
* 输入参数: pucMsg：消息缓冲区；
*          usMsgLen：消息长度。
*
* 输出参数: 无
*
* 返 回 值: true：消息有效；false：消息无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckP2PMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen )
{
    bool_t bValid = false;
    uint32_t uiCalcCRC = 0U;
    uint32_t uiMsgCRC = 0U;
    uint16_t usCRCIndex = 0U;

    if((pucMsg != NULL) &&
       (usMsgLen > ((uint16_t)(P2P_MSG_ADDL_LEN) + (P2P_MSG_FCS_LEN))))
    {
        usCRCIndex = usMsgLen - (uint16_t)(P2P_MSG_FCS_LEN);

        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucMsg, (uint32_t)usCRCIndex );

        /* Message CRC: High First */
        uiMsgCRC = (pucMsg[usCRCIndex] << 24) |
                   (pucMsg[usCRCIndex+1U] << 16) |
                   (pucMsg[usCRCIndex+2U] << 8) |
                    pucMsg[usCRCIndex+3U];

        DEBUG_RTS( "CheckP2PMsgValid: CalcCRC-0x%X; MsgCRC-0x%X\n", uiCalcCRC, uiMsgCRC );

        /* Compare CRC: If CRC Valid, true */
        if( uiCalcCRC == uiMsgCRC )
        {
            bValid = true; /* CRC Valid */
        }

        /* Frame Type */
        if( bValid )
        {
            if((PMP2P_REQ != pucMsg[P2P_TYPE_INDEX]) &&
               (PMP2P_RESP != pucMsg[P2P_TYPE_INDEX]))
            {
                bValid = false;
            }
        }
    }

    return bValid;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: CheckNTPMsgValid
*
* 功能描述: 检查校时消息是否有效
*
* 输入参数: pucMsg：消息缓冲区；
*          usMsgLen：消息长度。
*
* 输出参数: 无
*
* 返 回 值: true：消息有效；false：消息无效。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckNTPMsgValid( uint8_t const pucMsg[], uint16_t usMsgLen )
{
    bool_t bResult = false;
    uint32_t uiCalcCRC = 0U;
    uint32_t uiMsgCRC = 0U;
    uint16_t usCRCIndex = 0U;

    if(( pucMsg != NULL ) &&
       ( usMsgLen >= (uint16_t)((SNTP_MSG_HEADER_LEN) + (SNTP_MSG_CRC_LEN))))
    {
        usCRCIndex = usMsgLen - (uint16_t)(SNTP_MSG_CRC_LEN);

        /* Calc CRC */
        uiCalcCRC = SysCrc32Cal( 0U, pucMsg, (uint32_t)usCRCIndex );

        /* Message CRC: High First */
        uiMsgCRC = (pucMsg[usCRCIndex] << 24) |
                   (pucMsg[usCRCIndex+1U] << 16) |
                   (pucMsg[usCRCIndex+2U] << 8) |
                    pucMsg[usCRCIndex+3U];

        DEBUG_RTS( "CheckNTPMsgValid: CalcCRC-0x%X; MsgCRC-0x%X; Calc Length:%d.\n", \
                   uiCalcCRC, uiMsgCRC, usMsgLen - (uint16_t)(APP_MSG_CRC_LEN));

        /* Compare CRC: If CRC Valid, true */
        if( uiCalcCRC == uiMsgCRC )
        {
            bResult = true; /* CRC Valid */
        }
    }

    return bResult;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: SendToSOE
*
* 功能描述: 将数据发送到SOE软件
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucSWIndex：上位机软件的索引号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToSOE( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiSWIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_SOE == emSendType) && (ucSWIndex < (uint8_t)(APP_SW_SOE_NUM))))
    {
        hSock = s_stSOEConnSock[ucSWIndex].hSocket;

        /* Maybe need to adjust according to broadcast feature */
        if((s_stSOEConnSock[ucSWIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stSOEConnSock[ucSWIndex].uiIPAddr == uiIPAddr) &&
               (s_stSOEConnSock[ucSWIndex].usPort == usPort) &&
               (s_stSOEConnSock[ucSWIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToSOE: IP/Port/Net diff with SW-%d\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_SOESW_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiSWIPAddr = s_stSOEConnSock[ucSWIndex].uiIPAddr;
            DEBUG_RTS( "SendToSOE: IP %d.%d.%d.%d Disconnected\n", \
                       uiSWIPAddr>>24 & 0xFF, uiSWIPAddr>>16 & 0xFF, uiSWIPAddr>>8 & 0xFF, uiSWIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_SOESW_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToAMS
*
* 功能描述: 将数据发送到AMS软件
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucSWIndex：上位机软件的索引号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToAMS( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiSWIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_AMS == emSendType) && (ucSWIndex < (uint8_t)(APP_SW_AMS_NUM))))
    {
        hSock = s_stAMSConnSock[ucSWIndex].hSocket;

        /* Maybe need to adjust according to broadcast feature */
        if((s_stAMSConnSock[ucSWIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stAMSConnSock[ucSWIndex].uiIPAddr == uiIPAddr) &&
               (s_stAMSConnSock[ucSWIndex].usPort == usPort) &&
               (s_stAMSConnSock[ucSWIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToAMS: IP/Port/Net diff with SW-%d\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_AMSSW_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiSWIPAddr = s_stAMSConnSock[ucSWIndex].uiIPAddr;
            DEBUG_RTS( "SendToAMS: IP %d.%d.%d.%d Disconnected\n", \
                       uiSWIPAddr>>24 & 0xFF, uiSWIPAddr>>16 & 0xFF, uiSWIPAddr>>8 & 0xFF, uiSWIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_AMSSW_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToClient
*
* 功能描述: 将数据发送到客户端软件
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucSWIndex：上位机软件的索引号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToClient( uint8_t const pucData[], uint16_t usDataLen,
                             uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                             CommType_t emSendType, uint8_t ucSWIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiSWIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_CLIENT == emSendType) && (ucSWIndex < (uint8_t)(APP_SW_CLIENT_NUM))))
    {
        hSock = s_stCliConnSock[ucSWIndex].hSocket;

        /* Maybe need to adjust according to broadcast feature */
        if((s_stCliConnSock[ucSWIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stCliConnSock[ucSWIndex].uiIPAddr == uiIPAddr) &&
               (s_stCliConnSock[ucSWIndex].usPort == usPort) &&
               (s_stCliConnSock[ucSWIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToClient: IP/Port/Net diff with SW-%d\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_CLISW_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiSWIPAddr = s_stCliConnSock[ucSWIndex].uiIPAddr;
            DEBUG_RTS( "SendToClient: IP %d.%d.%d.%d Disconnected\n", \
                       uiSWIPAddr>>24 & 0xFF, uiSWIPAddr>>16 & 0xFF, uiSWIPAddr>>8 & 0xFF, uiSWIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_CLISW_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToOPC
*
* 功能描述: 将数据发送到OPC软件
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucSWIndex：上位机软件的索引号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToOPC( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType, uint8_t ucSWIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiSWIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_OPC == emSendType) && (ucSWIndex < (uint8_t)(APP_SW_OPC_NUM))))
    {
        hSock = s_stOPCConnSock[ucSWIndex].hSocket;

        /* Maybe need to adjust according to broadcast feature */
        if((s_stOPCConnSock[ucSWIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stOPCConnSock[ucSWIndex].uiIPAddr == uiIPAddr) &&
               (s_stOPCConnSock[ucSWIndex].usPort == usPort) &&
               (s_stOPCConnSock[ucSWIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToOPC: IP/Port/Net diff with SW-%d\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_OPCSW_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiSWIPAddr = s_stOPCConnSock[ucSWIndex].uiIPAddr;
            DEBUG_RTS( "SendToOPC: IP %d.%d.%d.%d Disconnected\n", \
                       uiSWIPAddr>>24 & 0xFF, uiSWIPAddr>>16 & 0xFF, uiSWIPAddr>>8 & 0xFF, uiSWIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_OPCSW_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToCS1131
*
* 功能描述: 将数据发送到CS1131软件
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucSWIndex：上位机软件的索引号，其他发送类型填零。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToCS1131( uint8_t const pucData[], uint16_t usDataLen,
                             uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                             CommType_t emSendType, uint8_t ucSWIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiSWIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_CS1131 == emSendType) && (ucSWIndex < (uint8_t)(APP_SW_CS1131_NUM))))
    {
        hSock = s_stCS1131ConnSock[ucSWIndex].hSocket;

        if((s_stCS1131ConnSock[ucSWIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stCS1131ConnSock[ucSWIndex].uiIPAddr == uiIPAddr) &&
               (s_stCS1131ConnSock[ucSWIndex].usPort == usPort) &&
               (s_stCS1131ConnSock[ucSWIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToCS1131: IP/Port/Net diff with SW-%d\n", ucSWIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_CS1131SW_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiSWIPAddr = s_stCS1131ConnSock[ucSWIndex].uiIPAddr;
            DEBUG_RTS( "SendToCS1131: IP %d.%d.%d.%d Disconnected\n", \
                       uiSWIPAddr>>24 & 0xFF, uiSWIPAddr>>16 & 0xFF, uiSWIPAddr>>8 & 0xFF, uiSWIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_CS1131SW_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToMBTCPMstr
*
* 功能描述: 将数据发送到 Modbus TCP主站
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型；
*          ucMasterIndex：Modbus TCP主站的索引号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToMBTCPMstr( uint8_t const pucData[], uint16_t usDataLen,
                                uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                CommType_t emSendType, uint8_t ucMasterIndex )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    uint32_t uiMasterIPAddr = 0U;  /* Only For Record Error Info. */

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ((TYPE_MBTCP_MSTR == emSendType) && (ucMasterIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT))))
    {
        hSock = s_stMBTCPMstrConnSock[ucMasterIndex].hSocket;
        if((s_stMBTCPMstrConnSock[ucMasterIndex].bConnFlag) &&
           (hSock != (int32_t)(INVALID_SOCKET)))
        {
            if((s_stMBTCPMstrConnSock[ucMasterIndex].uiIPAddr == uiIPAddr) &&
               (s_stMBTCPMstrConnSock[ucMasterIndex].usPort == usPort) &&
               (s_stMBTCPMstrConnSock[ucMasterIndex].emNetwork == emNetwork))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToMBTCPMstr: IP/Port/Net diff with Mstr-%d\n", ucMasterIndex );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_MBTCPMSTR_SRC));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            uiMasterIPAddr = s_stCS1131ConnSock[ucMasterIndex].uiIPAddr;
            DEBUG_RTS( "SendToMBTCPMstr: IP %d.%d.%d.%d Disconnected\n", \
                       uiMasterIPAddr>>24 & 0xFF, uiMasterIPAddr>>16 & 0xFF, \
                       uiMasterIPAddr>>8 & 0xFF, uiMasterIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_MBTCPMSTR_DISCONN));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToMBTCPSlave
*
* 功能描述: 将数据发送到Modbus TCP从站
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendToMBTCPSlave( uint8_t const pucData[], uint16_t usDataLen,
                                 uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                 CommType_t emSendType )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);
    ConnSockInfo_t *pConnSockInfo = NULL;
    uint8_t ucIndex = 0U;

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       (TYPE_MBTCP_SLAVE == emSendType ))
    {
        /* Find ConnSockInfo Item according to uiIPAddr, usPort, emNetwork */
        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM); ucIndex++ )
        {
            pConnSockInfo = &( s_stMBTCPSlaveConnSock[emNetwork][ucIndex].stConnSock );
            if((pConnSockInfo->uiIPAddr == uiIPAddr) &&
               (pConnSockInfo->usPort == usPort) &&
               (pConnSockInfo->emNetwork == emNetwork))
            {
                hSock = pConnSockInfo->hSocket;  /* Finded */
                break;
            }
        }

        if( ucIndex < (uint8_t)(MAX_MODBUS_SLAVE_NUM))
        {
            /* Has Finded */
            if((pConnSockInfo->bConnFlag) &&
               (hSock != (int32_t)(INVALID_SOCKET)))
            {
                ucResult = SendTCPData( hSock, pucData, (uint32_t)usDataLen );
            }
            else
            {
                DEBUG_RTS( "SendToMBTCPSlave: MBTCPSlave(IP-%d.%d.%d.%d) Not Connected\n", \
                            uiIPAddr>>24 & 0xFF, uiIPAddr>>16 & 0xFF, uiIPAddr>>8 & 0xFF, uiIPAddr & 0xFF );
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_MBTCPSLAVE_DISCONN));
                ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
            }
        }
        else
        {
            /* Not Find */
            DEBUG_RTS( "SendToMBTCPSlave: MBTCPSlave(IP-%d.%d.%d.%d) Not Exist\n", \
                       uiIPAddr>>24 & 0xFF, uiIPAddr>>16 & 0xFF, uiIPAddr>>8 & 0xFF, uiIPAddr & 0xFF );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_MBTCPSLAVE_NOT_EXIST));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendToP2P
*
* 功能描述: 将数据发送到其他控制站
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint8_t SendToP2P( uint8_t const pucData[], uint16_t usDataLen,
                          uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                          CommType_t emSendType )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ( TYPE_P2P == emSendType ))
    {
        hSock = s_stSockInfo[emNetwork].stP2PSock.hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            ucResult = SendUDPData( hSock, uiIPAddr, usPort, pucData, (uint32_t)usDataLen );
        }
        else
        {
            DEBUG_RTS( "P2P Socket Invalid\n" );
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SEND_P2P_SOCKET));
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: SendToNTPDevice
*
* 功能描述: 将数据发送到校时服务器 / 被校时从站
*
* 输入参数: pucData：待发送数据缓冲区；
*          usDataLen：待发送数据的字节数；
*          uiIPAddr：目的IP地址；
*          usPort：目的端口号；
*          emNetwork：本地网口号；
*          emSendType：发送类型。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint8_t SendToNTPDevice( uint8_t const pucData[], uint16_t usDataLen,
                                uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
                                CommType_t emSendType )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    SOCKET hSock = (int32_t)(INVALID_SOCKET);

    if(( pucData != NULL ) &&
       ( usDataLen > 0U ) &&
       ( uiIPAddr != 0U ) &&
       ( usPort != 0U ) &&
       ((emNetwork >= NET1) && (emNetwork < NUM_OF_NETWORK_PORT)) &&
       ( TYPE_NTP == emSendType ))
    {
        hSock = s_stSockInfo[emNetwork].stNTPSock.hSocket;
        if( hSock != (int32_t)(INVALID_SOCKET))
        {
            ucResult = SendUDPData( hSock, uiIPAddr, usPort, pucData, (uint32_t)usDataLen );
        }
        else
        {
            DEBUG_RTS( "NTP Socket Invalid\n" );
            ucResult = (uint8_t)(EDISCONNECT);  /* Disconnected */
        }
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: SendUDPData
*
* 功能描述: 发送通过UDP协议传输的数据
*
* 输入参数: hSock：本端连接套接字；
*          uiDstIPAddr：目的IP地址；
*          usDstPort：目的端口号；
*          pucData：待发送数据缓冲区；
*          uiDataLen：待发送数据的字节数。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint8_t SendUDPData( SOCKET hSock, uint32_t uiDstIPAddr, uint16_t usDstPort,
                            uint8_t const pucData[], uint32_t uiDataLen )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    int32_t iResult = 0;
    int32_t iSendLen = 0;       /* Sended Length */
    int32_t iTotalSendLen = 0;  /* Total Sended Length */
    int32_t iErrno = 0;         /* Errno of send operation */
    uint8_t ucRetryCnts = 0U;   /* Retry Counts */
    SOCKADDR_IN stDestAddr;     /* Destination Address */
    struct timeval stTV;        /* Time out */
    fd_set  fsFd;               /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );

    if(( hSock != (int32_t)(INVALID_SOCKET)) &&
       ( uiDstIPAddr != 0U ) &&
       ( usDstPort != 0U) &&
       ( pucData != NULL ) &&
       ( uiDataLen > 0U ))
    {
        FD_SET( hSock, &fsFd );  /* Add Valid Connect Socket */
        iResult = select( hSock+1, NULL, &fsFd, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error: Record errno */
            DEBUG_RTS( "SendUDPData: SelErr %d\n", GetErrorNo());
            LogWrite( (uint32_t)(LOGID_EXCOMM_E_SELECT));
            ucResult = ESELECT;
        }
        else if( 0 == iResult )
        {
            /* Select Timeout */
            DEBUG_RTS( "SendUDPData: select timeout-not ready\n" );
            ucResult = (uint8_t)(ENOTREADY);
        }
        else
        {
            /* Select Success */
            if( FD_ISSET( hSock, &fsFd ))  /* Writable */
            {
                /* Send */
                memset( &stDestAddr, 0, sizeof(SOCKADDR_IN));
                stDestAddr.sin_family = AF_INET;
                stDestAddr.sin_addr.s_addr = htonl( uiDstIPAddr );  /* Destination IP Address */
                stDestAddr.sin_port = htons( usDstPort );           /* Destination Port */
                while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
                {
                    iSendLen = sendto( hSock, &pucData[(uint32_t)iTotalSendLen], uiDataLen-(uint32_t)iTotalSendLen, \
                                       MSG_NOSIGNAL, (SOCKADDR*)&stDestAddr, sizeof(SOCKADDR_IN));
                    if((int32_t)(SOCKET_ERROR) == iSendLen )
                    {
                        /* Record errno */
                        iErrno = GetErrorNo();
                        if((EAGAIN == iErrno) || (EWOULDBLOCK == iErrno))
                        {
                            ucResult = (uint8_t)(ENOTREADY);  /* Not Ready */
                            DEBUG_RTS( "sendto: Not ready\n" );
                            break;
                        }
                        else if( EINTR == iErrno )
                        {
                            /* A signal occurred before any data was transmitted. */
                            DEBUG_RTS( "sendto: EINTR, retry-%d\n", ucRetryCnts );
                            ucRetryCnts++;
                        }
                        else
                        {
                            /* Send Error */
                            DEBUG_RTS( "sendto: Failed-%d\n", iErrno );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_UDP_SENDTO_FAIL));
                            ucResult = (uint8_t)(EDISCONNECT);
                            break;
                        }
                    }
                    else
                    {
                        iTotalSendLen += iSendLen;
                        DEBUG_RTS( "sendto: target %d; actual %d, total %d\n", uiDataLen, iSendLen, iTotalSendLen );
                        if((uint32_t)iTotalSendLen == uiDataLen )
                        {
                            DEBUG_RTS( "SendUDPData OK\n" );
                            ucResult = (uint8_t)(SEND_SUCCESS);  /* Send Successful */
                            break;
                        }
                        else
                        {
                            /* Record Error Info: Retry, not finish */
                            DEBUG_RTS( "sendto: Not Finish, Retry-%d\n", ucRetryCnts );
                            ucRetryCnts++;
                        }
                    }
                }/* while */

                if(((uint8_t)(SEND_SUCCESS) == ucResult) &&
                   ((uint32_t)iTotalSendLen != uiDataLen))
                {
                    /* Record Error Info */
                    DEBUG_RTS( "SendUDPData: Not finish-Actual %d < Target %d\n", iTotalSendLen, uiDataLen );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_UDP_SENDTO_NONFIN));
                    ucResult = (uint8_t)(ENOTFINISH);
                }
            }/* FD_ISSET */
            else
            {
                ucResult = (uint8_t)(ENOTREADY);
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_UDP_SENDTO_NOTRDY));
            }
        }/* Select Success */
    }
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
* 函数名称: SendTCPData
*
* 功能描述: 发送通过TCP协议传输的数据
*
* 输入参数: hSock：本端连接套接字；
*          pucData：待发送数据缓冲区；
*          uiDataLen：待发送数据的字节数。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t SendTCPData( SOCKET hSock, uint8_t const pucData[], uint32_t uiDataLen )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    int32_t iResult = 0;
    int32_t iSendLen = 0;       /* Sended Length */
    int32_t iTotalSendLen = 0;  /* Total Sended Length */
    int32_t iErrno = 0;         /* Errno of send operation */
    uint8_t ucRetryCnts = 0U;   /* Retry Counts */
    struct timeval stTV;        /* Time out */
    fd_set  fsFd;               /* Socket Queue */
    stTV.tv_sec = 0U;
    stTV.tv_usec = 0U;
    FD_ZERO( &fsFd );

    if(( hSock != (int32_t)(INVALID_SOCKET)) &&
       ( pucData != NULL ) &&
       ( uiDataLen > 0 ))
    {
        FD_SET( hSock, &fsFd );  /* Add Valid Connect Socket */
        iResult = select( hSock+1, NULL, &fsFd, NULL, &stTV );
        if((int32_t)(SOCKET_ERROR) == iResult )
        {
            /* Select Error: Record errno */
            DEBUG_RTS( "SendTCPData: select error-%d\n", GetErrorNo());
            ucResult = ESELECT;
        }
        else if( 0 == iResult )
        {
            /* Select Timeout */
            DEBUG_RTS( "SendTCPData: select timeout-not ready\n" );
            ucResult = (uint8_t)(ENOTREADY);
        }
        else
        {
            /* Select Success */
            if( FD_ISSET( hSock, &fsFd ))  /* Writable */
            {
                /* Send */
                while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
                {
                    iSendLen = send( hSock, &pucData[(uint32_t)iTotalSendLen], uiDataLen - (uint32_t)iTotalSendLen, \
                                     MSG_NOSIGNAL );
                    if((int32_t)(SOCKET_ERROR) == iSendLen )
                    {
                        /* Record errno */
                        iErrno = GetErrorNo();
                        if((EAGAIN == iErrno) || (EWOULDBLOCK == iErrno))
                        {
                            ucResult = (uint8_t)(ENOTREADY);  /* Not Ready */
                            DEBUG_RTS( "send: Not ready\n" );
                            break;
                        }
                        else if( EINTR == iErrno )
                        {
                            /* A signal occurred before any data was transmitted. */
                            DEBUG_RTS( "send: EINTR, retry-%d\n", ucRetryCnts );
                            ucRetryCnts++;
                        }
                        else
                        {
                            /* Send Error */
                            DEBUG_RTS( "send: Failed-%d\n", iErrno );
                            LogWrite( (uint32_t)(LOGID_EXCOMM_E_TCP_SEND_FAIL));
                            ucResult = (uint8_t)(EDISCONNECT);
                            break;
                        }
                    }
                    else
                    {
                        iTotalSendLen += iSendLen;
                        if((uint32_t)iTotalSendLen == uiDataLen )
                        {
                            ucResult = (uint8_t)(SEND_SUCCESS);  /* Send Successful */
                            break;
                        }
                        else
                        {
                            /* Record Error Info */
                            DEBUG_RTS( "send: Not Finish, Retry-%d\n", ucRetryCnts );
                            ucRetryCnts++;
                        }
                    }
                }/* while */

                if(((uint8_t)(SEND_SUCCESS) == ucResult) &&
                   ((uint32_t)iTotalSendLen != uiDataLen))
                {
                    /* Record Error Info */
                    DEBUG_RTS( "SendTCPData: Not finish, Actual %d < Target %d\n", iTotalSendLen, uiDataLen );
                    LogWrite( (uint32_t)(LOGID_EXCOMM_E_TCP_SEND_NONFIN));
                    ucResult = (uint8_t)(ENOTFINISH);
                }
            }/* FD_ISSET */
            else
            {
                ucResult = (uint8_t)(ENOTREADY);
                LogWrite( (uint32_t)(LOGID_EXCOMM_E_TCP_SEND_NOTRDY));
            }
        }/* Select Success */
    }/* Check Parameters */
    else
    {
        ucResult = (uint8_t)(EPARA);
    }

    return ucResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

