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
*  文件名称    : InterCommRTData.h
*
*  功能简介    :
*
*  作者       : 李琦
*
*  创建日期    : 2016-02-19
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef INTERCOMM_RTDATA_H_
#define INTERCOMM_RTDATA_H_

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
#define RTDATA_MSG_START_FLAG           (0x5555)      /* 实时数据消息开始标识 */
#define RTDATA_MSG_TRANS_CONTINUE_FLAG  (0x1234)      /* 实时数据区传输未完成标识 */
#define RTDATA_MSG_TRANS_FINISH_FLAG    (0x5678)      /* 实时数据区传输完成标识 */
#define SRV_CODE_RTDATA                 (1)           /* 服务码 */

/* 应答 */
#define RTDATA_MSG_ACK                  (0)           /* 收到正确实时数据，肯定应答 */
#define RTDATA_MSG_NAK                  (1)           /* 收到错误实时数据，否定应答 */

/* 错误码 */
#define RTDATA_MSG_OK                   (0)           /* 收到正确实时数据，无错误 */
#define RTDATA_MSG_RETRANS              (1)           /* 重新传输 */
#define RTDATA_MSG_CRC_ERROR            (2)           /* CRC出错 */
#define RTDATA_MSG_DELAY                (4)           /* 延时传输 */

#define RTDATA_MSG_START_FLAG_ERROR     (0x10)        /* 开始标识错误 */
#define RTDATA_MSG_SRV_CODE_ERROR       (0x20)        /* 服务码错误 */
#define RTDATA_MSG_SN_ERROR             (0x40)        /* 序列号错误 */
#define RTDATA_MSG_OFFSET_ERROR         (0x80)        /* 偏移地址错误 */


/* 校验码占4字节*/
#define RTDATA_MSG_CRC32_SIZE           4

/* 实时数据消息头部长度 */
#define RTDATA_MSG_HEADER_LEN           (sizeof(RTDataHeader_t))
/* 实时数据应答包长度 */
#define RTDATA_MSG_ACK_LEN              (sizeof(RTDataAck_t))

/* 实时数据包的各项偏移 */
#define RTDATA_START_FLAG_OFFSET            0
#define RTDATA_SRV_CODE_OFFSET              2
#define RTDATA_SERIAL_NUM_OFFSET            4
#define RTDATA_CONTENT_OFFSET_OFFSET        8
#define RTDATA_TRANSFER_FLAG_OFFSET         12
#define RTDATA_LEN_OFFSET                   14
#define RTDATA_AREA_TYPE_OFFSET             16
#define RTDATA_CONTENT_OFFSET               18

#define RTDATA_HEADER_TAIL_SIZE             (RTDATA_MSG_HEADER_LEN + RTDATA_MSG_CRC32_SIZE)

/* 实时数据应答包的各项偏移 */
#define RTDATA_ACK_START_FLAG_OFFSET            0
#define RTDATA_ACK_SRV_CODE_OFFSET              2
#define RTDATA_ACK_SERIAL_NUM_OFFSET            4
#define RTDATA_ACK_LEN_OFFSET                   8
#define RTDATA_ACK_ACK_OFFSET                   10
#define RTDATA_ACK_ERR_CODE_OFFSET              12
#define RTDATA_ACK_CRC_OFFSET                   14

/* 实时数据类型 */
#define RTDATA_TYPE_UP1_G                       0
#define RTDATA_TYPE_UP2_G                       1
#define RTDATA_TYPE_I                           2
#define RTDATA_TYPE_Q                           3
#define RTDATA_TYPE_APPEND                      4


#define RTDATA_VER_INFO_OFFSET                  (0U)//4
#define RTDATA_SYS_TIME_OFFSET                  (16U)//20

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/* 每包实时数据头部信息 */
typedef struct RTDataHeaderTag
{
    uint16_t usStartFlag;                           /* 开始标识 */
    uint16_t usServiceCode;                         /* 服务码*/
    uint32_t uiSerialNum;                           /* 数据包序列号*/
    uint32_t uiOffset;                              /* 在数据区中的偏移地址 */
    uint16_t usTransferFlag;                        /* 传输标识 */
    uint16_t usLen;                                 /* 数据内容长度*/
    uint16_t usAreaType;                            /* 数据区类型 */
} RTDataHeader_t;

/* 每包实时数据应答信息 */
typedef struct RTDataAckTag
{
    uint16_t usStartFlag;                           /* 开始标识 */
    uint16_t usServiceCode;                         /* 服务码*/
    uint32_t uiSerialNum;                           /* 数据包序列号*/
    uint16_t usLen;                                 /* 应答包长度*/
    uint16_t usAck;                                 /* 应答*/
    uint16_t usErrCode;                             /* 错误码*/
    uint32_t uiCRC32;                               /* CRC32*/
} RTDataAck_t;

#pragma pack()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: InterCommRTDataInit
*
* 功能描述: 静态变量初始化
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
void InterCommRTDataInit(void);

/*
***********************************************************************************************************************
* 函数名称: InterCommRTDataReceive
*
* 功能描述: 接收实时数据
*
* 输入参数: emPMID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
 bool_t InterCommRTDataReceive(PMController_t emPMID);

 /*
 ***********************************************************************************************************************
 * 函数名称: InterCommRTDataSend
 *
 * 功能描述: 发送实时数据应答
 *
 * 输入参数: emPMID
 *
 * 输出参数: 无
 *
 * 返 回 值  : 无
 *
 * 注意事项: 无
 ***********************************************************************************************************************
 */
 void InterCommRTDataSend(PMController_t emPMID);


#endif /* INTERCOMM_RTDATA_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

