/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : PM与IO模块安全通讯
*
*  文件名称    : PMIOSafetyComm.h
*
*  功能简介    : PMIO安全通讯相关处理
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

#ifndef PMIO_SAFETY_H_
#define PMIO_SAFETY_H_

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

#define DEBUG_PMIOSAFETY

#define PMIOSAFETY_CRC_LEN  (4)        /* PM/IO安全通讯CRC域的长度 */
#define MIN_IO_MODULE_ID    (4)        /* IO模块ID的最小值 */
#define MAX_IO_MODULE_ID    (239)      /* IO模块ID的最大值 */

#define MASTER_MASK         (0xFF00U)  /* 主站地址-PM的掩码 */
#define SLAVE_MASK          (0x00FFU)  /* 从站地址-IOID的掩码 */

/**************************** PM I/O Message ****************************/
/**** Status Byte Bit ****/
#define BIT_STATUS_FAIL     (0x01U)   /* Bit0: I/O模块存在失效时，置1 */
#define BIT_STATUS_ADDR     (0x02U)   /* Bit1: 通信错误：Code Name错误时，置1 */
#define BIT_STATUS_CRCSEQ   (0x04U)   /* Bit2: 通信错误：CRC 或 序号错误时，置1 */
#define BIT_STATUS_WDTO     (0x08U)   /* Bit3: 通信错误：看门狗超时，置1 */

#define BIT_STATUS_FV       (0x10U)   /* Bit4: 从站使用故障安全值时，置1 */
#define BIT_STATUS_IPARAOK  (0x20U)   /* Bit5: 从站采用主站下发的新参数后，置1 */
#define BIT_STATUS_REDUN    (0x40U)   /* Bit6: 通信错误：冗余数据不一致，置1*/
#define BIT_STATUS_RESERVE  (0x80U)   /* Bit7: 预留 */

/**** Control Byte Bit ****/
#define BIT_CTRL_ACTIVEFV   (0x01U)   /* Bit0: Active_FV，使用故障安全值标志位 */
#define BIT_CTRL_IPARAEN    (0x02U)   /* Bit1: iPar_En：    使能标志位，主站准备下发从站模块参数时，置1 */

/**** Communication Status ****/
#define BIT_COMM_REDUN      (0x01U)   /* Bit0: 冗余数据不一致，置1 */
#define BIT_COMM_CRC        (0x02U)   /* Bit1: CRC错误时，置1 */
#define BIT_COMM_ADDR       (0x04U)   /* Bit2: Code Name错误时，置1 */
#define BIT_COMM_SEQ        (0x08U)   /* Bit3: 序号错误时，置1 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* PM I/O Communication Info  PM与I/O模块通信信息结构体定义 */
typedef struct PMIOCommInfoTag
{
    CommErrorInfo_t stErrInfo;  /* Error Info.                     错误信息 */
    uint16_t usSendSeqNum;      /* Send sequence number            发送序号 */
    uint8_t ucCtrlByte;         /* Control byte                    控制字节 */
    bool_t bIOModFailure;       /* IO Module Failure               IO模块失效标志位 */
    bool_t bFVActived;          /* Fail-safe Values (FV) Activated 从站使用故障安全值标志位 */
    bool_t bNewIPara;           /* New i-Parameter OK              从站采用主站下发的新参数标志位 */
}PMIOCommInfo_t;

/************************ Communication Data Structure ************************/
/* PM I/O Request Head Info.  PM I/O间请求帧头信息结构体定义 */
typedef struct PMIOReqHeadInfoTag
{
    uint16_t usPMID;      /* PM Module ID        源地址：PM模块ID号 */
    uint16_t usIOID;      /* IO Module ID        目的地址：IO模块ID号 */
    uint16_t usSeq;       /* Sequence            序号 */
    uint8_t ucCtrlByte;   /* Control Byte        控制字节 */
    uint8_t ucSDataLen;   /* Safety Data Length  安全数据的长度-字节 */
}PMIOReqHeadInfo_t;

/* PM I/O Response Head Info.  PM I/O间应答帧头信息结构体定义 */
typedef struct PMIORespHeadInfoTag
{
    uint16_t usIOID;      /* IO Module ID        源地址：IO模块ID号 */
    uint16_t usPMID;      /* PM Module ID        目的地址：PM模块ID号 */
    uint16_t usSeq;       /* Sequence            序号 */
    uint8_t ucStatusByte; /* Status Byte         状态字节 */
    uint8_t ucSDataLen;   /* Safety Data Length  安全数据的长度-字节 */
}PMIORespHeadInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: PMIOCommInit
*
* 功能描述: PM/IO间安全通讯初始化函数
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

void PMIOCommInit( void );

/*
***********************************************************************************************************************
* 函数名称: PMIOHandleSendTO
*
* 功能描述: 发送超时错误处理
*
* 输入参数: ucIOIndex：I/O模块的索引值
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleSendTO( uint8_t ucIOIndex );

/*
***********************************************************************************************************************
* 函数名称: PMIOMakReq
*
* 功能描述: 构造将发送给指定I/O模块的请求帧
*
* 输入参数: ucIOIndex：I/O模块的索引值；
*          pucData：输出模块数据首地址；
*          ucDataLen：输出模块数据长度；
*          bIParaEn：PM准备下发从站模块参数时，置true；
*          pucReq：存放请求帧的首地址；
*
* 输出参数: pusReqLen：用于返回请求帧的长度
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOMakeReq( uint8_t ucIOIndex, uint8_t const pucData[], uint8_t ucDataLen, bool_t bIParaEn,
                    uint8_t pucReq[], uint16_t *pusReqLen );

/*
***********************************************************************************************************************
* 函数名称: PMIOHandleRecvTO
*
* 功能描述: 接收超时错误处理
*
* 输入参数: ucIOIndex：I/O模块的索引值
*
* 输出参数: 无
*
* 返 回 值: 是否处理成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIOHandleRecvTO( uint8_t ucIOIndex );

/*
***********************************************************************************************************************
* 函数名称: PMIODecodeResp
*
* 功能描述: 解析来自I/O模块的应答帧
*
* 输入参数: ucIOIndex：I/O模块的索引值；
*          bInputModule：输入模块标志位；
*          pucResp：应答帧首地址；
*          usRespLen：应答帧长度；
*          pucData：存放输入数据的首地址；
*
* 输出参数: pucDataLen：返回输入数据的长度
*
* 返 回 值: 是否解析成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PMIODecodeResp( uint8_t ucIOIndex, bool_t bInputModule, uint8_t pucResp[], uint16_t usRespLen,
                       uint8_t pucData[], uint8_t *pucDataLen );

#endif /* PMIO_SAFETY_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


