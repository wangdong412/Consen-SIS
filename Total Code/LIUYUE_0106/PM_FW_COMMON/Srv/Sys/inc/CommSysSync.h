/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : SysSync.h
*
*  功能简介    :
*
*  作者            : 刘阳
*
*  创建日期    : 2016-03-23
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef COMM_SYS_SYNC_H_
#define COMM_SYS_SYNC_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "CommSysCommon.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
/* sync (100us unit)*/
#define SYNC_SRV_DATA_LEN               (8000U)

/*Ctrl CMD ID*/
#define SYS_UP_PAUSE                    (3001)
#define SYS_UP_CONTINUE                 (3002)
#define SYS_PRJ_SWITCH                  (3003)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
/* 应答标志 */
typedef enum
{
    NO_ACK = 0x00000000U,                             /* 应答标志:需要应答 = 0 */
    NEED_ACK                                              /* 应答标志:无需应答 = 1 */
}ack_flag_t;

/* 同步结果 */
typedef enum
{
    SYNC_OK = 0x00000000U,                              /* 同步完成 */
    SYNC_ERR                                            /* 同步错误 */
}sync_ret_t;

typedef enum
{
    AJUST_NO = 0x00000000U,                                /* 系统校时:未校时 */
    AJUST_HAS                                              /* 系统校时:已校时 */
}sys_ajust_t;

#pragma pack (1)


typedef struct SyncTimeD32Tag
{
    uint32_t uiHigh32b;                                     /* the high 32 bits of sync (us) timer */
    uint32_t uiLow32b;                                      /* the low 32 bits of sync (us) timer */
}SyncTimeD32_t;

typedef union SyncTime64Tag
{
    SyncTimeD32_t stTimeStruct;                             /* double 32 bits struct */
    ulong64_t ullTime;                                      /* 64 bits value */
}SyncTime64_t;

typedef struct SysSrvDataTag
{
    /* 服务号 */
    uint16_t usSrvNo;
    /* 服务数据长度 */
    uint32_t uiSrvLen;
    /* 服务数据 */
    uint8_t ucSrvData[SYNC_SRV_DATA_LEN];
}SysSrvData_t;

typedef struct SysSrvInfoHeadTag
{
    /* 服务需处理标识 */
    uint8_t ucProcessFlag;
    /* 服务需应答标识 */
    ack_flag_t emAckFlag;
    /* 服务激活时间 */
    SyncTime64_t unActiveTime;
    /* 服务同步超时时间 */
    SyncTime64_t unTimeOutTime;
    /* 服务同步结果 */
    sync_ret_t emSyncRet;
}SysSrvInfoHead_t;

typedef struct SysSrvInfoTag
{
    /* 服务头 */
    SysSrvInfoHead_t stSysSrvInfoHead;
    /* 服务内容 */
    SysSrvData_t stSysSrvData;
}SysSrvInfo_t;
#pragma pack ()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: CommSysSyncInit
*
* 功能描述: 初始化。
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
void CommSysSyncInit(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetSyncTime
*
* 功能描述: 获取同步时钟值。
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
void SysGetSyncTime(SyncTime64_t* punSyncTime);
/*
***********************************************************************************************************************
* 函数名称: SysSetSyncTime
*
* 功能描述: 设置同步时钟值(百微秒)。
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
void SysSetSyncTime(SyncTime64_t* punSyncTime);
/*
***********************************************************************************************************************
* 函数名称: SysGetSyncAjust
*
* 功能描述: 获取同步时钟是否校时状态。
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
sys_ajust_t SysGetSyncAjust(void);
/*
***********************************************************************************************************************
* 函数名称: SysSetSyncAjust
*
* 功能描述: 设置同步时钟是否校时状态。
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
void SysSetSyncAjust(sys_ajust_t emRetAjustFlag);

#endif /* COMM_SYS_SYNC_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
