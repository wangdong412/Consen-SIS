/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : 软件应用模块
*
*  文件名称    : FileUpTask.h
*
*  功能简介    : 处理CM固件更新 和 源文件更新
*
*  作者        : 王东
*
*  创建日期    : 2017-03-18
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef FILEUPTASK_H_
#define FILEUPTASK_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "../inc/PCSWCommon.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_FUP

#define DEBUG_OUT_FUP

#ifdef DEBUG_OUT_FUP
#define DEBUG_FU(format, args...)  printf ("DB_FU: ") ; printf(format, ##args)
#else
#define DEBUG_FU(format, args...)
#endif

#define MAX_SINGLE_FW_FRA_LEN        (8*1024)          /* 每包CM固件的最大长度 */
#define MAX_OPR_MSG_NUM              (10)              /* 操作消息的最大数目 */

/* FUP Shared Memory: 32MB = RTS 6MB + RTSCRC 1MB + FPGA 5MB + Source file 20MB */
#define SHM_FUP_SIZE                 (32*1024*1024)    /* 用于CM固件升级的共享内存的尺寸: 16MB=RTS10MB + FPGA6MB */
#define MAX_CMRTS_SIZE               (6*1024*1024)     /* CM RTS的最大尺寸： 6MB */
#define MAX_CMRTSCRC_SIZE            (1*1024*1024)     /* CM RTS的最大尺寸： 1MB */
#define MAX_CMFPGA_SIZE              (5*1024*1024)     /* CM FPGA的最大尺寸：5MB */
#define MAX_SRC_SIZE                 (20*1024*1024)    /* 源文件的最大尺寸：20MB */

/* Base address: Offset bytes */
#define BASE_ADDR_CM_RTS             (0)                                          /* CM RTS文件数据在共享内存中的基地址 */
#define BASE_ADDR_CM_RTSCRC          ((BASE_ADDR_CM_RTS) + (MAX_CMRTS_SIZE))      /* CM RTSCRC文件数据在共享内存中的基地址 */
#define BASE_ADDR_CM_FPGA            ((BASE_ADDR_CM_RTSCRC)+(MAX_CMRTSCRC_SIZE))  /* CM FPGA文件数据在共享内存中的基地址 */
#define BASE_ADDR_SRC                ((BASE_ADDR_CM_FPGA)+(MAX_CMFPGA_SIZE))      /* CM FPGA文件数据在共享内存中的基地址 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Operation Info. 操作信息 */
typedef struct OperationInfoTag
{
    uint8_t ucOprType;       /* Operation Type: clear, write, update index */
    uint8_t ucFileType;      /* File Type: RTS / RTSInfo */
    uint16_t usFDataLen;     /* File Data Length: max 8kB */
    uint32_t uiFDataOffset;  /* File Data Offset: base addr is shm 0 */
}OperationInfo_t;

/* FUP Operation Message 文件升级操作消息 */
typedef struct FUPOprMsgTag
{
    int32_t  iMsgType;          /* Message Type: 1-Operation */
    OperationInfo_t stOprInfo;  /* Operation Info. */
}FUPOprMsg_t;

/* Operation Result Info. 操作结果信息 */
typedef struct OprRetInfoTag
{
    uint8_t ucOprType;   /* Operation Type: clear, write, update index */
    uint8_t ucFileType;  /* File Type: RTS / RTSInfo */
    uint8_t ucErrCode;   /* Error Code: 0 - Success */
    uint8_t ucReserve;   /* Reserve */
}OprRetInfo_t;

/* FUP Operation Result Message 文件更新操作结果消息 */
typedef struct FUPOprRetMsgTag
{
    int32_t  iMsgType;          /* Message Type: 2-Operation Result */
    OprRetInfo_t stOprRetInfo;  /* Operation Result Info. */
}FUPOprRetMsg_t;

/* FUP Operation Message List 文件更新操作消息链表 */
typedef struct FUPOprMsgListTag
{
    uint8_t ucHead;       /* 写入索引值：以消息为单位 */
    uint8_t ucTail;       /* 读取索引值：以消息为单位，最老消息 */
    uint8_t ucCurNum;     /* 当前消息数目 */
    uint8_t ucTotalNum;   /* 容量：常数，10 */
    FUPOprMsg_t stOprMsg[MAX_OPR_MSG_NUM];
}FUPOprMsgList_t;

/* File Update Operation Info. 文件更新操作信息 */
typedef struct FUPOprInfoTag
{
    bool_t bAllowFUP;              /* Allow file update only when file update process start successfully */
    uint8_t ucReserve[3];          /* Reserve */
    uint32_t uiShmOffset;          /* File data offset: base addr is shm 0 */
    int32_t iMsgQID;               /* ID of IPC message queue */
    FUPOprMsgList_t stOprMsgList;  /* FUP Operation Message List */
    OprRetInfo_t stSendEInfo;      /* Send message error information */
}FUPOprInfo_t;

/* Operation Type 操作类型枚举定义 */
typedef enum OprTypeTag
{
    OPRT_UPSTART = 0,  /* Update Start     固件更新开始 */
    OPRT_UPCONT,       /* Update Continue  固件更新继续 */
    OPRT_UPINDEX       /* Update Index     更新索引值 */
}OprType_t;

/* Message Type 消息类型枚举定义 */
typedef enum MsgTypeTag
{
    MSGT_OPR = 1,       /* Operation                            操作消息 */
    MSGT_FWUP_OPR_RET,  /* FWUP operation result                固件升级操作结果消息 */
    MSGT_SRCUP_OPR_RET  /* Update source file operation result  更新源文件操作结果消息 */
}MsgType_t;

/* File Update Type 文件更新类型枚举定义 */
typedef enum FileUpTypeTag
{
    FUT_CMFPGA = 0,
    FUT_CMRTS,
    FUT_CMRTSCRC,
    FUT_SRC
}FileUpType_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: FWUPTask
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

void FWUPTask( int32_t iShmID, int32_t iMsgQID );

/*
***********************************************************************************************************************
* 函数名称: InitOprMsgList
*
* 功能描述: 初始化操作消息链表
*
* 输入参数: pstOprList: 操作消息链表
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitOprMsgList( FUPOprMsgList_t *pstOprList );

/*
***********************************************************************************************************************
* 函数名称: IsOprMsgListEmpty
*
* 功能描述: 操作消息链表是否为空
*
* 输入参数: pstOprList: 操作消息链表
*
* 输出参数: 无
*
* 返 回 值: 是否为空
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsOprMsgListEmpty( FUPOprMsgList_t const *pstOprList );

/*
***********************************************************************************************************************
* 函数名称: WriteOprMsg
*
* 功能描述: 向操作消息链表中写入消息
*
* 输入参数: pstOprList：操作消息链表；
*          pstOprMsg：操作消息。
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t WriteOprMsg( FUPOprMsgList_t *pstOprList, FUPOprMsg_t *pstOprMsg );

/*
***********************************************************************************************************************
* 函数名称: ReadOprMsg
*
* 功能描述: 从操作消息链表中读出消息
*
* 输入参数: pstOprList：操作消息链表；
*          pstOprMsg：操作消息。
*
* 输出参数: 无
*
* 返 回 值: 是否读取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t ReadOprMsg( FUPOprMsgList_t *pstOprList, FUPOprMsg_t *pstOprMsg );

/*
***********************************************************************************************************************
* 函数名称: SendOprMsg
*
* 功能描述: 发送操作消息链表中的消息：发空
*
* 输入参数: pstOprList：操作消息链表；
*          iMsgQID：消息队列ID；
*          pstSendEInfo：发送错误信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SendOprMsg( FUPOprMsgList_t *pstOprList, int32_t iMsgQID, OprRetInfo_t *pstSendEInfo );

/*
***********************************************************************************************************************
* 函数名称: RecvOprRetMsg
*
* 功能描述: 接收操作结果消息：一周期最多接收一个，失败时，只记录日志
*
* 输入参数: iMsgQID：消息队列ID；
*          emRetMsgType：结果类型。
*
* 输出参数: 无
*
* 返 回 值: 是否发生操作错误，若错误，则记录日志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t RecvOprRetMsg( int32_t iMsgQID, MsgType_t emRetMsgType );

/*
***********************************************************************************************************************
* 函数名称: ReadFile
*
* 功能描述: 读文件数据
*
* 输入参数: iFD：文件FD；
*          uiOffset：待读取数据的字节偏移，基地址为0；
*          pucData：数据缓冲区；
*          uiLen：目标读取长度。
*
* 输出参数: 无
*
* 返 回 值  : 实际读取的字节数
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t ReadFile( int32_t iFD, uint32_t uiOffset, uint8_t pucData[], uint32_t uiLen );

#endif /*FILEUPTASK_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


