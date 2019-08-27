/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : 站间安全通讯
*
*  文件名称    : P2PSafetyComm.h
*
*  功能简介    : 站间安全通讯相关处理
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

#ifndef P2P_SAFETY_COMM_H_
#define P2P_SAFETY_COMM_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"
#include "SafetyCommon.h"
#include "Srv/SharedMem/inc/SharedSafetyComm.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_P2PSAFETY

#ifdef DEBUG_OUT_P2P
#define DEBUG_P2P(format, args...)  printf ("DEBUG_P2P: ") ; printf(format, ##args)
#else
#define DEBUG_P2P(format, args...)
#endif

#define P2PSAFETY_CRC_LEN       (sizeof(uint32_t))  /* P2P安全通讯CRC域的长度 */
#define P2P_FCS_LEN             (sizeof(uint32_t))  /* P2P安全通讯FCS域的长度 */
#define MAX_P2PSAFETY_DATA_LEN  (1024)              /* 站间通讯安全数据的最大长度 */
#define P2P_TIMEOUT_TIME        (2000)              /* 固定为2000ms */

#define PMID_MASK               (0xC0U)    /* PMID的掩码 */
#define STAID_MASK              (0x3FU)    /* 控制站ID的掩码 */

#define PMID_A                  (0x00)     /* PMID_A */
#define PMID_B                  (0x40)     /* PMID_B */
#define PMID_C                  (0x80)     /* PMID_C */

/**************************** PM P2P Message ****************************/
/**** Status Bit ****/
#define BIT_WDTO         (0x0001U)  /* Bit0: 通信错误-看门狗超时 */
#define BIT_PMACRC       (0x0002U)  /* Bit1: 通信错误-PMA CRC错误 */
#define BIT_PMBCRC       (0x0004U)  /* Bit2: 通信错误-PMB CRC错误 */
#define BIT_PMCCRC       (0x0008U)  /* Bit3: 通信错误-PMC CRC错误 */

#define BIT_PMAADDR      (0x0010U)  /* Bit4: 通信错误-PMA 源/目的地址错误 */
#define BIT_PMBADDR      (0x0020U)  /* Bit5: 通信错误-PMB 源/目的地址错误 */
#define BIT_PMCADDR      (0x0040U)  /* Bit6: 通信错误-PMC 源/目的地址错误 */
#define BIT_PMASEQ       (0x0080U)  /* Bit7: 通信错误-PMA 序号错误 */

#define BIT_PMBSEQ       (0x0100U)  /* Bit8: 通信错误-PMB 序号错误 */
#define BIT_PMCSEQ       (0x0200U)  /* Bit9: 通信错误-PMC 序号错误 */
#define BIT_PMAREDUN     (0x0400U)  /* Bit10: 通信错误-PMA冗余数据不一致 */
#define BIT_PMBREDUN     (0x0800U)  /* Bit11: 通信错误-PMB冗余数据不一致 */

#define BIT_PMCREDUN     (0x1000U)  /* Bit12: 通信错误-PMC冗余数据不一致 */
#define BIT_ALARM        (0x2000U)  /* Bit13: 通信错误-附加信息错误 */
#define BIT_MULIPMREQ    (0x4000U)  /* Bit14: 多PM请求帧标志位 */
#define BIT_STAREV       (0x8000U)  /* Bit15: 预留 */


/**** PM Status Bit ****/
#define BIT_PM_CRC       (0x01U)    /* Bit0: CRC错误 */
#define BIT_PM_ADDR      (0x02U)    /* Bit1: 源/目的地址错误 */
#define BIT_PM_SEQ       (0x04U)    /* Bit2: 序号错误 */
#define BIT_PM_REDUN     (0x08U)    /* Bit3: 冗余数据不一致 */

#define BIT_PM_FAILURE   (0x10U)    /* Bit4: 倒向安全 */

/**** Control Byte ****/
#define BIT_CTRL_FAILURE (0x01U)    /* Bit0: 倒向安全状态标志位 */

/**** Additional PM Info. ****/
#define BIT_ADDL_PMA     (0x01U)    /* Bit0: PMA 安全帧存在标志位 */
#define BIT_ADDL_PMB     (0x02U)    /* Bit1: PMB 安全帧存在标志位 */
#define BIT_ADDL_PMC     (0x04U)    /* Bit2: PMC 安全帧存在标志位 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* P2P Receive Request Communication Info. 接收站：接收请求帧通讯信息结构体定义 */
typedef struct P2PRecvReqCommInfoTag
{
    CommErrorInfo_t stErrInfo;  /* Error Info.             错误信息 */
    uint16_t usRecvSeqNum;      /* Receive Sequence Number 接收序号 */
    uint16_t usStatusInfo;      /* Status Information      状态信息 */
}P2PRecvReqCommInfo_t;

/* P2P Send Request Communication Info. 发送站：发送请求帧通讯信息结构体定义 */
typedef struct P2PSendReqCommInfoTag
{
    CommErrorInfo_t stErrInfo;  /* Error Info.                   错误信息 */
    uint16_t usSendSeqNum;      /* Send Sequence Number          发送序号 */
    uint16_t usLastResSeqNum;   /* Last Response Sequence Number 上一应答帧序号 */
    bool_t bCheckRpt;           /* Check repeat flag             检查重复标志 */
    uint8_t ucCtrlByte;         /* Control Byte                  控制字节 */
    uint8_t ucReserve[2];       /* Reserve                       预留 */
}P2PSendReqCommInfo_t;

/* Station Safety Data Info. 控制站安全数据信息结构体定义 */
typedef struct StaSafetyDataInfoTag
{
    bool_t bValid;                           /* Valid Flag             数据是否有效标志位 */
    uint8_t ucReqStatus;                     /* Status                 PM请求帧的状态 */
    uint16_t usReqSendSeq;                   /* Request Send Sequence  请求帧中的发送序号 */
    uint16_t usDataLen;                      /* Data Length            数据长度 */
    uint8_t ucBuff[MAX_P2PSAFETY_DATA_LEN];  /* Buffer                 数据缓冲区 */
}StaSafetyDataInfo_t;

/* Request Decode Info. 控制站请求帧解析信息结构体定义 */
typedef struct ReqDecodeInfoTag
{
    bool_t bRepeat;                     /* Repeat Flag  重复标志 */
    uint8_t ucReserve[3];               /* Reserve      预留 */
    StaSafetyDataInfo_t stPMADataInfo;  /* PMA Data     来自PMA的安全数据 */
    StaSafetyDataInfo_t stPMBDataInfo;  /* PMB Data     来自PMB的安全数据 */
    StaSafetyDataInfo_t stPMCDataInfo;  /* PMC Data     来自PMC的安全数据 */
}ReqDecodeInfo_t;

/* Request Decode Result Info. 控制站请求帧解析结果信息结构体定义 */
typedef struct ReqDecodeResultInfoTag
{
    bool_t bRepeat;                          /* Repeat Flag                    重复标志：重复时丢弃-不写变量，不应答 */
    uint8_t ucReserve[3];
    PMController_t emSelPMID;                /* Selected PM ID                 选取的安全数据来自发送站的哪系PM */
    uint16_t usDataLen;                      /* Data Length                    数据长度 */
    uint8_t ucBuff[MAX_P2PSAFETY_DATA_LEN];  /* Buffer                         数据缓冲区 */
}ReqDecodeResultInfo_t;

/* Response Decode Result Info. 控制站请求帧解析结果信息结构体定义*/
typedef struct RespDecodeResultInfoTag
{
    bool_t bRepeat;        /* Repeat Flag  重复标志 */
    uint8_t ucReserve[3];  /* Reserve      预留 */
}RespDecodeResultInfo_t;

/************************ Communication Data Structure ************************/

/* Station Request Additional Info.  控制站请求帧附加信息结构体定义 */
typedef struct StaReqAddlInfoTag
{
    uint8_t ucSrcStaID;          /* Source Station ID       源控制站ID */
    uint8_t ucDstStaID;          /* Destination Station ID  目的控制站ID */
    uint8_t ucFrameType;         /* Frame Type              帧类型 */
    uint8_t ucReserve1;          /* Reserve 1               预留1 */
    uint16_t usTotalLen;         /* Total Length            总长度，包含附加信息和FCS */
    uint8_t ucPMInfo;            /* PM Info.                PM信息 */
    uint8_t ucReserve2;          /* Reserve 2               预留2 */
}StaReqAddlInfo_t;

/* Station Request Head Info.  控制站请求帧头信息结构体定义 */
typedef struct StaReqHeadInfoTag
{
    uint16_t usSeq;       /* Sequence            序号 */
    uint8_t ucSrcAddr;    /* Source Address      源地址：PM编号+控制站节点号 */
    uint8_t ucDstAddr;    /* Destination Address 目的地址：控制站节点号 */
    uint8_t ucReserve;    /* Reserve             预留 */
    uint8_t ucCtrlByte;   /* Control Byte        控制字节 */
    uint16_t usSDataLen;  /* Safety Data Length  安全数据的长度-字节 */
}StaReqHeadInfo_t;

/* Station Response Additional Info.  控制站应答帧附加信息结构体定义 */
typedef struct StaRespAddlInfoTag
{
    uint8_t ucSrcStaID;          /* Source Station ID       源控制站ID */
    uint8_t ucDstStaID;          /* Destination Station ID  目的控制站ID */
    uint8_t ucFrameType;         /* Frame Type              帧类型 */
    uint8_t ucReserve1;          /* Reserve 1               预留1 */
    uint16_t usTotalLen;         /* Total Length            总长度，包含附加信息和FCS */
    uint8_t ucReserve2[2];       /* Reserve 2               预留2 */
}StaRespAddlInfo_t;

/* Station Response Head Info.  控制站应答帧头信息结构体定义 */
typedef struct StaRespHeadInfoTag
{
    uint16_t usSeq;       /* Sequence            序号 */
    uint8_t ucSrcAddr;    /* Source Address      源地址：PM编号+控制站节点号 */
    uint8_t ucDstAddr;    /* Destination Address 目的地址：控制站节点号 */
    uint16_t usStatus;    /* Status Info         状态信息 */
    uint16_t usSDataLen;  /* Safety Data Length  安全数据的长度-字节 */
}StaRespHeadInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: P2PCommInitSend
*
* 功能描述: 初始化发送状态信息：本站作为发送站，只有Core 1访问
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

void P2PCommInitSend( void );

/*
***********************************************************************************************************************
* 函数名称: P2PCommInitRecv
*
* 功能描述: 初始化接收状态信息：本站作为接收站，只有Core 0访问
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

void P2PCommInitRecv( void );

/*
***********************************************************************************************************************
* 函数名称: P2PHandleRecvReqTO
*
* 功能描述: 接收请求帧超时错误处理
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleRecvReqTO( uint8_t ucSendStaID );

/*
***********************************************************************************************************************
* 函数名称: P2PDecodeReq
*
* 功能描述: 解析来自指定发送控制站的请求帧
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64；
*          bRedundant：冗余链路标志位；
*          pucReq：请求帧首地址；
*          usReqLen：请求帧长度。
*
* 输出参数: pstResult：用于返回请求帧解析结果
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PDecodeReq( uint8_t ucSendStaID, bool_t bRedundant, uint8_t pucReq[], uint16_t usReqLen,
                     ReqDecodeResultInfo_t *pstResult );

/*
***********************************************************************************************************************
* 函数名称: P2PMakeResp
*
* 功能描述: 构造将发送给指定发送控制站的应答帧
*
* 输入参数: ucSendStaID：发送控制站ID， 1-64；
*          pucResp：应答帧的首地址。
*
* 输出参数: pusRespLen：用于返回应答帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PMakeResp( uint8_t ucSendStaID, uint8_t pucResp[], uint16_t *pusRespLen );

/*
***********************************************************************************************************************
* 函数名称: P2PHandleSendReqTO
*
* 功能描述: 本站作为发送控制站：发送超时错误处理
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          bSendTimeout：发送超时标志位。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleSendReqTO( uint8_t ucRecvStaID, bool_t bTimeout );

/*
***********************************************************************************************************************
* 函数名称: P2PMakeReq
*
* 功能描述: 构造将发送给指定接收控制站的请求帧
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          pucData：数据首地址；
*          usDataLen：数据长度；
*          pucReq：存放请求帧的首地址；
*
* 输出参数: pusReqLen：用于返回请求帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PMakeReq( uint8_t ucRecvStaID, uint8_t const pucData[], uint16_t usDataLen,
                   uint8_t pucReq[], uint16_t *pusReqLen );

/*
***********************************************************************************************************************
* 函数名称: P2PHandleRespTO
*
* 功能描述: 应答超时错误处理
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64。
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PHandleRespTO( uint8_t ucRecvStaID );

/*
***********************************************************************************************************************
* 函数名称: P2PDecodeResp
*
* 功能描述: 解析来自指定接收控制站的应答帧
*
* 输入参数: ucRecvStaID：接收控制站ID， 1-64；
*          bRedundant：冗余链路标志位；
*          pucResp：应答帧首地址；
*          usRespLen：应答帧长度；
*
* 输出参数: pstResult：存放应答帧解析结果
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t P2PDecodeResp( uint8_t ucRecvStaID, bool_t bRedundant, uint8_t pucResp[], uint16_t usRespLen,
                      RespDecodeResultInfo_t *pstResult );

#if 0
/*
***********************************************************************************************************************
* 函数名称: ClearStationInfo
*
* 功能描述: 清除模块内保存的与相应块相关的信息
*
* 输入参数: ucStaID：控制站ID， 1-64；
*          bP2PRecv：P2P接收信息标志位，true时，ucStaID代表发送站ID；否则ucStaID代表接收站ID。
*
* 输出参数: 无
*
* 返 回 值: 是否清除成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t ClearStationInfo( uint8_t ucStaID, bool_t bP2PRecv );
#endif

#endif /* P2P_SAFETY_COMM_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


