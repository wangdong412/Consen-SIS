/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 通讯协议模块
*
*  文件名称    : CMBusProtocol.h
*
*  功能简介    : 负责PM与CM间的数据通讯
*
*  作者       : 李琦
*
*  创建日期    : 2016-02-24
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef CM_BUS_PROTOCOL_H_
#define CM_BUS_PROTOCOL_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"
#include "App/ProjectConfig/inc/Config.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
//#define DEBUG_OUT_CMBUSPROTOCOL
#ifdef DEBUG_OUT_CMBUSPROTOCOL
#define DEBUG_CMBUSPRO(format, args...)  printf ("CMBUS:") ; printf(format, ##args)
#else
#define DEBUG_CMBUSPRO(format, args...)
#endif

#define PM_SEND_BUFF_SIZE                   (1024*8)
#define PM_SEND_BUFF_BODY_SIZE              (1024*8 - sizeof(PMSendHeader_t))
#define PM_RECV_BUFF_SIZE                   (1024*8)
#define PM_RECV_BUFF_BODY_SIZE              (1024*8 - sizeof(PMRecvHeader_t))

#define CRC32_SIZE                          (4)

#define DETECT_MSG_FLAG                     (0x05U)      /* 探测消息 */
#define DETECT_MSG_ACK_FLAG                 (0x0AU)      /* 探测消息应答 */


#define DATA_BLOCK_INVALID_START_FLAG       (0x0000)
#define DATA_BLOCK_START_FLAG               (0x5555)

#define DATA_BLOCK_START_FLAG_OFFSET        (0)
#define DATA_BLOCK_START_FLAG_SIZE          (2)
#define DATA_BLOCK_MSG_LEN_OFFSET           (2)
#define DATA_BLOCK_MSG_LEN_SIZE             (2)
#define DATA_BLOCK_MSG_CONTENT_OFFSET       (4)
#define DATA_BLOCK_CRC32_SIZE               (4)
#define DATA_BLOCK_HEADER_TAIL_SIZE         (8)               /*START FLAG SIZE + MSG LEN SIZE + CRC32 SIZE*/


#define CONFIG_INFO_START                   (0xFFFFFFFF)      /* 传输配置文件开始命令 */
#define CONFIG_INFO_NO_REQ                  (0x00000000)      /* 无传输配置文件命令 */
#define CONFIG_INFO_CONTINUE                (0x00000001)      /* 传输配置文件继续命令 */
#define CONFIG_INFO_RETRANSFER              (0x00000002)      /* 重传传输配置文件命令 */
#define TRANSFER_CONFIG_INFO_FINISHED       (0x00000004)      /* 传输配置文件结束命令 */

#define MAX_CFGFILE_PACKAGE_SIZE            (2048)            /* 每次传输配置文件的最大长度 */

#define MAX_RTDATA_PACKAGE_SIZE             (4096)            /* 每次传输实时数据包的最大长度 */

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

#define FPGA_NAME  "/dev/fpga"

#define MAX_SEND_SN                         (10000U)

#define MAX_COMM_TIMEOUT                    (5)
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/*对缓冲区的动态操作过程*/
typedef struct PMCommHandlingTag
{
    uint16_t usUsed;                /* 已使用空间 */
    uint16_t usRest;                /* 剩余空间 */
    uint16_t usOffset;              /* 可以使用的偏移 */
} PMCommHandling_t;

/*PM各类收发信息在缓冲区中的偏移地址及长度*/
typedef struct PMInfoStructTag
{
    uint16_t usLen;                 /* 数据长度 */
    uint16_t usOffset;              /* 在缓冲区中的偏移地址 */
} PMInfoStruct_t;

/*PM发送数据格式*/
typedef struct PMSendDataHeaderTag
{
    uint16_t usCMAddr;              /* CM地址：1~4 */
    uint16_t usPackageSerialNum;    /* 数据包序列号  */
    uint16_t usDataLen;             /* 数据内容长度 */
    PMInfoStruct_t stSysStateBlock;   /* 系统状态信息（PM状态、CM状态及校时信息）*/
    PMInfoStruct_t stDetectMsgBlock;  /* PM发送的探测消息 */
    PMInfoStruct_t stSOEResp;         /* SOE应答 */
    PMInfoStruct_t stRealTimeData;    /* 实时数据 */
    PMInfoStruct_t stCS1131Resp;      /* 组态软件应答 */
    PMInfoStruct_t stP2PReq;          /* P2P请求 */
    PMInfoStruct_t stP2PResp;         /* P2P应答 */
    PMInfoStruct_t stOPCResp;         /* OPC应答 */
    PMInfoStruct_t stClientReq;       /* 客户端请求 */
    PMInfoStruct_t stClientResp;      /* 客户端应答 */
    PMInfoStruct_t stAMSResp;         /* AMS应答 */
    PMInfoStruct_t stCfgTableBlock;   /* 配置表 */
    PMInfoStruct_t stConfigRespBlock; /* 应答CM配置信息 */
    uint16_t usReserved[16];        /* 保留32B */
    uint32_t uiCRC32;               /* 32位CRC校验码 */
} PMSendHeader_t;

typedef struct PMSendDataBodyTag
{
    uint8_t ucDataBuffer[PM_SEND_BUFF_BODY_SIZE];
} PMSendBody_t;

typedef struct PMSendBufferTag
{
    PMSendHeader_t stSendHeader;      /* 发送的头部信息 */
    PMSendBody_t stSendBody;          /* 发送的实际信息 */
} PMSendBuffer_t;

/* PM接收数据格式 */
typedef struct PMRecvDataHeaderTag
{
    uint16_t usCMAddr;              /* CM地址：1~4 */
    uint16_t usPackageSerialNum;    /* 数据包序列号 */
    uint16_t usDataLen;             /* 数据内容长度 */
    PMInfoStruct_t stCMStateBlock;     /* 系统状态信息（CM状态及校时信息）*/
    PMInfoStruct_t stDetectMsgAckBlock;/* 对探测消息的应答 */
    PMInfoStruct_t stSOEReq;          /* SOE请求 */
    PMInfoStruct_t stRealTimeDataAck; /* 实时数据应答 */
    PMInfoStruct_t stCS1131Req;       /* 组态软件请求 */
    PMInfoStruct_t stP2PReq;          /* P2P请求 */
    PMInfoStruct_t stP2PResp;         /* P2P应答 */
    PMInfoStruct_t stModbus;          /* Modbus 数据 */
    PMInfoStruct_t stOPCReq;          /* OPC请求 */
    PMInfoStruct_t stClientReq;       /* 客户端请求 */
    PMInfoStruct_t stClientResp;      /* 客户端应答 */
    PMInfoStruct_t stAMSReq;          /* AMS请求 */
    PMInfoStruct_t stConfigReqBlock;  /* 请求CM配置信息 */
    PMInfoStruct_t stInterCmd;        /* 内部命令信息 */
    uint16_t usReserved[14];        /* 保留28B */
    uint32_t uiCRC32;               /* 32位CRC校验码 */
} PMRecvHeader_t;

typedef struct PMRecvDataBodyTag
{
    uint8_t ucDataBuffer[PM_RECV_BUFF_BODY_SIZE];
} PMRecvBody_t;

typedef struct PMRecvBufferTag
{
    PMRecvHeader_t stRecvHeader;      /* 接收的头部信息 */
    PMRecvBody_t stRecvBody;          /* 接收的实际内容信息 */
} PMRecvBuffer_t;

/* 发送到CM的探测消息 */
typedef struct PMDetectMsgTag
{
    uint32_t uiFlag;                /* 探测标识：  0x12345678-探测数据包 */
} PMDetectMsg_t;

/* 发送到CM的探测消息数据块 */
typedef struct PMDetectMsgBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(DetectMsg_t)*/
    uint16_t usLen;
    /* 系统状态 */
    PMDetectMsg_t stDetectMsg;
    /* 校验码 */
    uint32_t uiCrc32;
} PMDetectMsgBlock_t;

/* 接收的来自CM的应答消息 */
typedef struct PMDetectMsgAckTag
{
    uint32_t uiFlag;                /* 探测标识：  0x87654321-探测数据包应答 */
} PMDetectMsgAck_t;

/* 接收的来自CM的探测消息应答数据块 */
typedef struct PMDetectMsgAckBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(PMDetectMsgAck_t)*/
    uint16_t usLen;
    /* 应答 */
    PMDetectMsgAck_t stDetectMsgAck;
    /* 校验码 */
    uint32_t uiCrc32;
} PMDetectMsgAckBlock_t;

/* 来自CM的获取配置信息命令 */
typedef struct PMConfigReqTag
{
    uint32_t uiRequest;             /* 请求标识：  0x00000000-无请求  0xFFFFFFFF-请求所有配置信息 */
} PMConfigReq_t;

/* 来自CM的配置信息请求数据块 */
typedef struct PMConfigReqBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(PMConfigReq_t)*/
    uint16_t usLen;
    /* 请求命令 */
    PMConfigReq_t stConfigReq;
    /* 校验码 */
    uint32_t uiCrc32;
} PMConfigReqBlock_t;

/* 发送的CM的配置信息数据块 */
typedef struct PMConfigRespBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(CMConfigInfo_t)*/
    uint16_t usLen;
    /* 配置信息 */
    CMConfigInfo_t stCMConfigInfo;
    /* 校验码 */
    uint32_t uiCrc32;
} PMConfigRespBlock_t;

/* 发送的CM的配置表数据块 */
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
* 函数名称: CMBusProtocolInit
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
void CMBusProtocolInit (void);

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolCycle
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
void CMBusProtocolCycle (void);




#endif /* CM_BUS_PROTOCOL_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

