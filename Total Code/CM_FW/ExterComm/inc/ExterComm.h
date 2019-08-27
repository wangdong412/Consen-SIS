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
*  文件名称    : ExterComm.h
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

#ifndef EXTERCOMM_H_
#define EXTERCOMM_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "Socket.h"
#include "ComCommon.h"
#include "PCSWCommon.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_EXTERCOMM

#define CONN_TIMEOT_CYCLE_CNT    (80*1000/(MAIN_TASK_PERIOD))       /* TCP连接超时拍数：80ms */
#define ETH_CHK_INTL_CYC_CNT     (20*1000*1000/(MAIN_TASK_PERIOD))  /* 每隔20s检查网卡开关状态 */
#define ETH_CREATE_WAIT_CYC_CNT  (0*1000*1000/(MAIN_TASK_PERIOD))   /* 修改IP后，无需等待10s，即可创建相关套接字 */

/* Network Default Value */
#define DEF_IPADDR_NET1 (0xC0A80001U)  /* 网口1 IP地址默认值：192.168.0.1 */
#define DEF_IPADDR_NET2 (0xC0A80101U)  /* 网口2 IP地址默认值：192.168.1.1 */
#define DEF_SUBNET_MASK (0xFFFFFF00U)  /* 子网掩码默认值：255.255.255.0 */

/* Return Value Of ExternalCommSend */
#define SEND_SUCCESS    (0x00U)        /* 发送成功 */
#define EPARA           (0x01U)        /* 发送失败：入参无效 */
#define EDISCONNECT     (0x02U)        /* 发送失败：连接已断开 */
#define ESELECT         (0x03U)        /* 发送失败：Select 函数调用失败 */
#define ENOTREADY       (0x04U)        /* 发送失败：未准备好，应用程序应等待一段时间后，继续尝试发送 */
#define ENOTFINISH      (0x05U)        /* 发送失败：未发送完成 */
#define EETHCLOSE       (0x06U)        /* 发送失败：网卡关闭 */

/* Network Adapter */
#define __NR_storm_init     (355)
#define __NR_get_eth1       (356)
#define __NR_get_eth2       (357)
#define __NR_set_eth1       (358)
#define __NR_set_eth2       (359)
#define __NR_local_disable  (360)
#define __NR_local_enable   (361)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Socket Info. 与套接字相关的结构体定义 */
typedef struct SockInfoTag
{
    SOCKET hSocket;      /* Socket  套接字 */
    uint16_t usPort;     /* Port    端口号 */
    uint16_t usReserve;  /* Reserve 预留 */
}SockInfo_t;

/* Network Socket Info 网口套接字信息结构体定义 */
typedef struct NetworkSockInfoTag
{
    SockInfo_t stLisCS1131Sock; /* Listen CS1131 Socket  监听CS1131软件的连接请求 */
    SockInfo_t stLisOPCSock;    /* Listen OPC Socket     监听OPC软件的连接请求 */
    SockInfo_t stLisSOESock;    /* Listen SOE Socket     监听SOE软件的连接请求 */
    SockInfo_t stLisCliSock;    /* Listen Client Socket  监听客户端软件的连接请求 */
    SockInfo_t stLisAMSSock;    /* Listen AMS Socket     监听AMS软件的连接请求 */
    SockInfo_t stLisMBSock;     /* Listen Modbus Socket  监听Modbus TCP Master的连接请求 */
    SockInfo_t stP2PSock;       /* Peer-to-Peer Socket   站间通讯套接字 */
}NetworkSockInfo_t;

/* Connect Socket Info 已连接套接字信息结构体定义 */
typedef struct ConnSockInfoTag
{
    SOCKET hSocket;               /* Socket        套接字 */
    uint32_t uiIPAddr;            /* IP Address    对端的IP地址 */
    uint16_t usPort;              /* Port          对端的端口号 */
    bool_t bConnFlag;             /* Connect Flag  是否处于连接标志位 */
    uint8_t ucReserve;            /* Reserve       预留 */
    NetworkPortType_t emNetwork;  /* Network       网口号 */
}ConnSockInfo_t;

/* Client Connect Socket Info 客户端已连接套接字信息结构体定义 */
typedef struct CliConnSockInfoTag
{
    ConnSockInfo_t stConnSock;    /* ConnSockInfo        已连接套接字信息  */
    bool_t bWaitConn;             /* Wait Connect Finish 等待连接完成标志 */
    uint8_t ucReserve;            /* Reserve             预留 */
    uint16_t usWaitCnt;           /* Wait Counts         已等待拍数 */
}CliConnSockInfo_t;

/* UDP Receive Info  UDP接收信息结构体定义 */
typedef struct UDPRecvInfoTag
{
    uint32_t uiIPAddr;            /* IP Address    对端的IP地址 */
    uint16_t usPort;              /* Port          对端的端口号 */
    bool_t bConnFlag;             /* Connect Flag  是否处于连接标志位 */
    uint8_t ucReserve;            /* Reserve       预留 */
    NetworkPortType_t emNetwork;  /* Network       网口号 */
}UDPRecvInfo_t;

/* Message Head  消息头结构体定义 */
typedef struct NetMsgHeadTag
{
    uint32_t uiIPAddr;            /* IP Address  数据源的IP地址 */
    uint16_t usPort;              /* Port        数据源的端口号 */
    uint16_t usLen;               /* Length      消息长度 */
    NetworkPortType_t emNetwork;  /* Network     数据源的网口号 */
}NetMsgHead_t;

/* Network Message 网络消息结构体定义 */
typedef struct NetMsgTag
{
    NetMsgHead_t stNetHead;       /* Network Message Head  消息头信息 */
    uint8_t ucBuff[MAX_MSG_LEN];  /* Buffer                缓冲区，用于存放消息内容 */
}NetMsg_t;

/* Modbus TCP Message  Modbus TCP 消息结构体定义 */
typedef struct MBTCPMsgTag
{
    NetMsgHead_t stNetHead;       /* Network Message Head  消息头信息 */
    uint8_t ucBuff[MAX_MSG_LEN];  /* Buffer 缓冲区，用于存放消息内容 */
}MBTCPMsg_t;

/* File UP Task Info. 文件更新任务信息结构体定义 */
typedef struct FileUPTaskInfoTag
{
    bool_t bFileUpTask;    /* FileUpTask flag */
    uint8_t ucReserve[3];  /* Reserve */
    int32_t iMsgQID;       /* ID of IPC Message Queue */
    puint8_t pucShmAddr;   /* Start address of IPC shared memory */
}FileUPTaskInfo_t;

/***************************** Configuration *****************************/
/* External Communication Network Info. 外部通讯网口参数信息结构体定义 */
typedef struct ExtCommNetworkInfoTag
{
    uint8_t ucIndex;                                        /* Index 当前使用的配置参数的索引值 */
    bool_t bIPChange;                                       /* IP Change Flag  IP地址变化标志位 */
    bool_t bConnMBSlave;                                    /* Handle Connect with MBTCPSlave Flag */
    uint8_t ucReserve;                                      /* Reserve 预留 */
    uint32_t uiCMCfgCRC;                                    /* CM配置文件CRC */
    uint32_t uiCMCfgVer;                                    /* CM配置文件的版本 */
    ExtCommNetConfigInfo_t stNetConfig[LOCAL_CFG_VER_CNT];  /* Network Parameters 网口参数*/
}ExtCommNetworkInfo_t;

/* External Communication Modbus TCP Info.  外部通讯Modbus TCP信息结构体定义 */
typedef struct ExtCommModbusTCPInfoTag
{
    uint8_t ucIndex;                                                /* Index 当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];                                           /* Reserve 预留 */
    uint32_t uiCMCfgCRC;                                            /* CM配置文件CRC */
    uint32_t uiCMCfgVer;                                            /* CM配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;                                    /* Real-time Data Version 实时数据的版本 */
    ExtCommModbusTCPConfigInfo_t stMBTCPConfig[LOCAL_CFG_VER_CNT];  /* Modbus TCP Configure  Modbus TCP配置参数 */
}ExtCommModbusTCPInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
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

void ExternalCommInit( void );


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

void ExternalCommCycle( void );

/*
***********************************************************************************************************************
* 函数名称: ExternalCommSend
*
* 功能描述: 模块发送函数
*
* 输入参数: pucDataBuff：待发送数据缓冲区；
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
		                  CommType_t emSendType, uint8_t ucIndex );

/*
***********************************************************************************************************************
* 函数名称: GetFileUpTaskInfo
*
* 功能描述: 获取文件更新任务的信息
*
* 输入参数: pstInfo：用于返回文件更新任务信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void GetFileUpTaskInfo( FileUPTaskInfo_t *pstInfo );

/*
***********************************************************************************************************************
* 函数名称: CopyToShm
*
* 功能描述: 将数据拷贝进共享内存
*
* 输入参数: uiShmOffset：相对于共享内存首地址的偏移；
*          pucData：数据；
*          uiDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CopyToShm( uint32_t uiShmOffset, uint8_t const pucData[], uint32_t uiDataLen );

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

uint32_t GetIPAddr( NetworkPortType_t emNet );

#endif /*EXTERCOMM_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


