/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 内部通讯模块
*
*  文件名称    : InterComm.h
*
*  功能简介    : 负责PM与CM间的数据通讯
*
*  作者       : 李琦
*
*  创建日期    : 2016-01-13
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef INTERCOMM_H_
#define INTERCOMM_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/DataType.h"
#include "Sys/inc/Hardware.h"
#include "Sys/inc/Sys.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define CM_SEND_BUFF_SIZE                   (1024*8)
#define CM_SEND_BUFF_BODY_SIZE              (1024*8 - sizeof(SendHeader_t))
#define CM_RECV_BUFF_SIZE                   (1024*8)
#define CM_RECV_BUFF_BODY_SIZE              (1024*8 - sizeof(RecvHeader_t))

#define CRC32_SIZE                          (4)

#define DETECT_MSG_FLAG                     (0x05U)      /* 探测消息 */
#define DETECT_MSG_ACK_FLAG                 (0x0AU)      /* 探测消息应答 */

#define DATA_BLOCK_INVALID_START_FLAG       (0x0000)          /* 非法开始标志 */
#define DATA_BLOCK_START_FLAG               (0x5555)          /* 正常开始标志 */

#define DATA_BLOCK_START_FLAG_OFFSET        (0)
#define DATA_BLOCK_START_FLAG_SIZE          (2)
#define DATA_BLOCK_MSG_LEN_OFFSET           (2)
#define DATA_BLOCK_MSG_LEN_SIZE             (2)
#define DATA_BLOCK_MSG_CONTENT_OFFSET       (4)
#define DATA_BLOCK_CRC32_SIZE               (4)
#define DATA_BLOCK_HEADER_TAIL_SIZE         (8)               /*START FLAG SIZE + MSG LEN SIZE + CRC32 SIZE*/


#define CONFIG_INFO_ALL_CFG_START           (0xFFFFFFFF)      /* 传输全部配置文件开始命令 */
#define CONFIG_INFO_INC_CFG_START           (0xFFFFFFFE)      /* 传输增量配置文件开始命令 */
#define CONFIG_INFO_NO_REQ                  (0x00000000)      /* 无传输配置文件命令 */
#define CONFIG_INFO_CONTINUE                (0x00000001)      /* 传输配置文件继续命令 */
#define CONFIG_INFO_RETRANSFER              (0x00000002)      /* 重传传输配置文件命令 */
#define TRANSFER_CONFIG_INFO_FINISHED       (0x00000004)      /* 传输配置文件结束命令 */

#define CFG_FILE_NOT_LAST_PACKAGE           (0x00)
#define CFG_FILE_LAST_PACKAGE               (0x01)
#define CFG_FILE_TOTAL_FINISHED             (0x80)


#define CFG_FILE_START_FLAG_OFFSET          (0)
#define CFG_FILE_TYPE_OFFSET                (2)
#define CFG_FILE_OFFSET_ADDR_OFFSET         (4)
#define CFG_FILE_CONTENT_LEN_OFFSET         (8)
#define CFG_FILE_LAST_PACKAGE_OFFSET        (10)
#define CFG_FILE_CONTENT_OFFSET             (12)

#define CFG_FILE_HEADER_LEN                 (12)
#define CFG_FILE_HEADER_TAIL_LEN            (16)

#define MAX_NO_RECV_DATA_CYCLE_NUM          (20)
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/*对缓冲区的动态操作过程*/
typedef struct CommHandlingTag
{
    uint16_t usUsed;                /* 已使用空间 */
    uint16_t usRest;                /* 剩余空间 */
    uint16_t usOffset;              /* 可以使用的偏移 */
} CommHandling_t;

/*CM各类收发信息在缓冲区中的偏移地址及长度*/
typedef struct InfoStructTag
{
    uint16_t usLen;                 /* 数据长度 */
    uint16_t usOffset;              /* 在缓冲区中的偏移地址 */
} InfoStruct_t;

/*CM接收数据格式*/
typedef struct ReceiveDataHeaderTag
{
    uint16_t usCMAddr;              /* CM地址：1~4 */
    uint16_t usPackageSerialNum;    /* 数据包序列号  */
    uint16_t usDataLen;             /* 数据内容长度 */
    InfoStruct_t stSysStateBlock;   /* 系统状态信息（PM状态、CM状态及校时信息）*/
    InfoStruct_t stDetectMsgBlock;  /* PM发送的探测消息 */
    InfoStruct_t stSOEResp;         /* SOE应答 */
    InfoStruct_t stRealTimeData;    /* 实时数据 */
    InfoStruct_t stCS1131Resp;      /* 组态软件应答 */
    InfoStruct_t stP2PReq;          /* P2P请求 */
    InfoStruct_t stP2PResp;         /* P2P应答 */
    InfoStruct_t stOPCResp;         /* OPC应答 */
    InfoStruct_t stClientReq;       /* 客户端请求 */
    InfoStruct_t stClientResp;      /* 客户端应答 */
    InfoStruct_t stAMSResp;         /* AMS应答 */
    InfoStruct_t stCfgTableBlock;   /* 配置表 */
    InfoStruct_t stConfigRespBlock; /* 应答CM配置信息 */
    uint16_t usReserved[16];        /* 保留32B */
    uint32_t uiCRC32;               /* 32位CRC校验码 */
} RecvHeader_t;

typedef struct ReceiveDataBodyTag
{
    uint8_t ucDataBuffer[CM_RECV_BUFF_BODY_SIZE];
} RecvBody_t;

typedef struct ReceiveBufferTag
{
    RecvHeader_t stRecvHeader;      /* 接收的头部信息 */
    RecvBody_t stRecvBody;          /* 接收的实际信息 */
} RecvBuffer_t;

/*对发送缓冲区的动态填充操作过程*/
typedef struct SendDataHeaderTag
{
    uint16_t usCMAddr;              /* CM地址：1~4 */
    uint16_t usPackageSerialNum;    /* 数据包序列号 */
    uint16_t usDataLen;             /* 数据内容长度 */
    InfoStruct_t stCMStateInfo;     /* 系统状态信息（CM状态及校时信息）*/
    InfoStruct_t stDetectMsgAckBlock;/* 对探测消息的应答 */
    InfoStruct_t stSOEReq;          /* SOE请求 */
    InfoStruct_t stRealTimeDataAck; /* 实时数据应答 */
    InfoStruct_t stCS1131Req;       /* 组态软件请求 */
    InfoStruct_t stP2PReq;          /* P2P请求 */
    InfoStruct_t stP2PResp;         /* P2P应答 */
    InfoStruct_t stModbus;          /* Modbus 数据 */
    InfoStruct_t stOPCReq;          /* OPC请求 */
    InfoStruct_t stClientReq;       /* 客户端请求 */
    InfoStruct_t stClientResp;      /* 客户端应答 */
    InfoStruct_t stAMSReq;          /* AMS请求 */
    InfoStruct_t stConfigReqBlock;  /* 请求CM配置信息 */
    InfoStruct_t stInterCmd;        /* 内部命令信息 */
    uint16_t usReserved[14];        /* 保留28B */
    uint32_t uiCRC32;               /* 32位CRC校验码 */
} SendHeader_t;

typedef struct SendDataBodyTag
{
    uint8_t ucDataBuffer[CM_SEND_BUFF_BODY_SIZE];
} SendBody_t;

typedef struct SendBufferTag
{
    SendHeader_t stSendHeader;      /* 发送的头部信息 */
    SendBody_t stSendBody;          /* 发送的实际内容信息 */
} SendBuffer_t;

typedef struct DetectMsgTag
{
    uint32_t uiFlag;                /* 探测标识：   0x12345678-探测数据包 */
} DetectMsg_t;

/* 探测消息数据块 */
typedef struct DetectMsgBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(DetectMsg_t)*/
    uint16_t usLen;
    /* 系统状态 */
    DetectMsg_t stDetectMsg;
    /* 校验码 */
    uint32_t uiCrc32;
} DetectMsgBlock_t;

typedef struct DetectMsgAckTag
{
    uint32_t uiFlag;                /* 探测标识：   0x87654321-探测数据包应答 */
} DetectMsgAck_t;

/* 探测消息应答数据块 */
typedef struct DetectMsgAckBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(DetectMsgAck_t)*/
    uint16_t usLen;
    /* 应答 */
    DetectMsgAck_t stDetectMsgAck;
    /* 校验码 */
    uint32_t uiCrc32;
} DetectMsgAckBlock_t;

/* 获取配置信息命令 */
typedef struct ConfigReqTag
{
    uint32_t uiRequest;             /* CONFIG_INFO_START
                                       CONFIG_INFO_CONTINUE
                                       CONFIG_INFO_RETRANSFER
                                       TRANSFER_CONFIG_INFO_FINISHED
                                   */
} ConfigReq_t;

/* 配置信息请求数据块 */
typedef struct ConfigReqBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(ConfigReq_t)*/
    uint16_t usLen;
    /* 请求命令 */
    ConfigReq_t stConfigReq;
    /* 校验码 */
    uint32_t uiCrc32;
} ConfigReqBlock_t;



/* 来自PM的配置信息头部 */
typedef struct CfgFileRespHeaderTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 配置文件类型 */
    uint16_t usCfgFileType;
    /* 数据内容偏移地址 */
    uint32_t uiOffset;
    /* 数据长度 */
    uint16_t usLen;
    /* 最后一包标志 */
    uint16_t usLastPackFlag;
} CfgFileRespHeader_t;

/* 配置表数据块 */
typedef struct CfgFileTableBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(SysCfgFileTable_t)*/
    uint16_t usLen;
    /* 配置表 */
    SysCfgFileTable_t stCfgTable;
    /* 校验码 */
    uint32_t uiCrc32;
} CfgFileTableBlock_t;

#pragma pack()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: InternalCommInit
*
* 功能描述: 初始化函数
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
void InternalCommInit (void);

/*
***********************************************************************************************************************
* 函数名称: InternalCommCycle
*
* 功能描述: 周期运行函数
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
void InternalCommCycle (void);




#endif /* INTERCOMM_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

