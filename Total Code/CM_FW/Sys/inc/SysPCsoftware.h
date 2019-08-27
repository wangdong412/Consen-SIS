﻿/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : SysPCsoftware.h
*
*  功能简介    :
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-31
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_PCSOFTWARE_H_
#define SYS_PCSOFTWARE_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "DataType.h"
#include "Hardware.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define MAX_MSG_LEN         8000                /*最大请求或应答消息内容长度 (FPGA收发缓冲区为8192，预留192字节头部信息)*/

#define MSGBOX_LOCKED       true                /* 信箱被锁定*/
#define MSGBOX_UNLOCKED     false               /* 信箱未锁定 */

#define MSGBOX_FULL         true                /* 信箱满 */
#define MSGBOX_EMPTY        false               /* 信箱空 */

/* -------------------------------外部通讯相关------------------------------------- */
/* 每种上位机软件的最大连接个数*/
#define APP_SW_MAX_NUM        16                /* 软件的最大连接个数 */
#define APP_SW_SOE_NUM        4                 /* SOE软件的最大个数*/
#define APP_SW_AMS_NUM        2                 /* AMS软件的最大个数*/
#define APP_SW_CLIENT_NUM     4                 /* 客户端软件的最大个数*/
#define APP_SW_CS1131_NUM     4                 /* CS1131软件的最大个数*/
#define APP_SW_OPC_NUM        16                /* OPC软件的最大个数*/
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* -------------------------------内部通讯相关------------------------------------- */
/* 消息*/
typedef struct SysMsgTag
{
    uint16_t usLen;                             /* 消息内容长度 */
    uint8_t ucContent[MAX_MSG_LEN];             /* 存放消息内容 */
} SysMsg_t;

/* 信箱 */
typedef struct SysMsgBoxTag
{
    bool_t bLock;                               /* 信箱是否可以被访问 */
    bool_t bMsgBoxStatus;                       /* 信箱的两种状态：空或满 */
    SysMsg_t stMsg;                             /* 消息 */
} SysMsgBox_t;

/* 消息类型（请求或应答）*/
typedef enum SysMsgTypeTag
{
    SOE_REQ = 0,                                /* 请求 */
    SOE_RESP = 1,                               /* 应答 */
    CS1131_REQ = 2,                             /* 请求 */
    CS1131_RESP = 3,                            /* 应答 */
    OPC_REQ = 4,                                /* 请求 */
    OPC_RESP = 5,                               /* 应答 */
    CLIENT_REQ = 6,                             /* 请求 */
    CLIENT_RESP = 7,                            /* 应答 */
    AMS_REQ = 8,                                /* 请求 */
    AMS_RESP = 9,                               /* 应答 */
    INTER_CMD_REQ = 10,                         /* 请求 */
    INTER_CMD_RESP = 11,                        /* 应答 */
    NUM_OF_MSGTYPE
} SysMsgType_t;

/* -------------------------------外部通讯相关------------------------------------- */
/* 消息*/
typedef struct SysAppMsgTag
{
    uint16_t usLen;                             /* 消息内容长度 */
    uint8_t ucContent[MAX_MSG_LEN];             /* 存放消息内容 */
} SysAppMsg_t;

/* 信箱 */
typedef struct SysAppMsgBoxTag
{
    bool_t bLock;                               /* MsgBox是否可以被访问 */
    bool_t bMsgBoxStatus;                       /* MsgBox的两种状态：空或满 */
    SysMsg_t stMsg;                             /* 消息 */
} SysAppMsgBox_t;

/* 上位机软件类型*/
typedef enum SysSoftwareTypeTag
{
    APP_SOE = 0,                                /* SOE软件 */
    APP_CS1131 = 1,                             /* CS1131软件 */
    APP_OPC = 2,                                /* OPC软件 */
    APP_CLIENT = 3,                             /* 客户端软件 */
    APP_AMS = 4,                                /* AMS软件 */
    NUM_OF_SOFTWARETYPE
} SysSoftwareType_t;


/* 上位机软件的消息类型（请求或应答）*/
typedef enum SysAppMsgTypeTag
{
    APP_MSG_REQ = 0,                            /* 请求 */
    APP_MSG_RESP = 1,                           /* 应答 */
    NUM_OF_APPMSGTYPE
} SysAppMsgType_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysPCsoftwareInit
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
void SysPCsoftwareInit(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetMsg
*
* 功能描述: 从系统资源模块获取消息（请求/应答）
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待读出的消息内容
*         pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-收到消息
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetMsg (PMController_t emPMID, SysMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen);

/*
***********************************************************************************************************************
* 函数名称: SysClearMsg
*
* 功能描述: 清除请求/应答
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: 无
*
* 返 回 值  : true-无错误
*        false- 参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysClearMsg (PMController_t emPMID, SysMsgType_t emMsgType);

/*
***********************************************************************************************************************
* 函数名称: SysGetMsgLen
*
* 功能描述: 从系统资源模块获取消息（请求/应答）的长度
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-读取成功
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetMsgLen (PMController_t emPMID, SysMsgType_t emMsgType, puint16_t pusLen);

/*
***********************************************************************************************************************
* 函数名称: SysSetMsg
*
* 功能描述: 将请求或应答消息写入系统资源模块
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetMsg (PMController_t emPMID, SysMsgType_t emMsgType , puint8_t pucContent, uint16_t usLen);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppMsg
*
* 功能描述: 从系统资源模块获取来自上位机软件的消息（请求/应答）
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: pucContent: 指向待读出的消息内容
*         pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-读取成功
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 输入参数错误或信箱不允许被访问或信箱为空时，输出参数pusLen值为0
***********************************************************************************************************************
*/
bool_t SysGetAppMsg(SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType,
                  puint8_t pucContent, puint16_t pusLen);

/*
***********************************************************************************************************************
* 函数名称: SysSetAppMsg
*
* 功能描述: 将上位机软件请求或应答消息写入系统资源模块
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppMsg (SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType,
                  puint8_t pucContent, uint16_t usLen);

/*
***********************************************************************************************************************
* 函数名称: SysClearAppMsg
*
* 功能描述: 清除系统资源模块获取来自上位机软件的消息（请求/应答）
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysClearAppMsg(SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType);

/*
***********************************************************************************************************************
* 函数名称: SysSetAppSwConnectFlag
*
* 功能描述: 位置"连接"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回TRUE
*        置位失败或输入参数错误，返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwConnectFlag
*
* 功能描述: 复位"连接"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回TRUE
*        复位失败或输入参数错误，返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwConnectFlag
*
* 功能描述: 获取连接标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "连接"返回TRUE
*        "断开"或输入参数错误，返回FALSE
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysSetAppSwLockedFlag
*
* 功能描述: 置位"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回true
*        置位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwLockedFlag
*
* 功能描述: 复位"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回true
*        复位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwLockedFlag
*
* 功能描述: 获取"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "锁定"返回true
*        "未锁定"或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysSetAppSwClearFlag
*
* 功能描述: 置位"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回true
*        置位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwClearFlag
*
* 功能描述: 复位"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回true
*        复位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwClearFlag
*
* 功能描述: 获取"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "清除"标识置位，返回true
*        "清除"标识未置位或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex);


#endif /* SYS_PCSOFTWARE_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
