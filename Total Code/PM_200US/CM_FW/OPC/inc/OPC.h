/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : OPC软件应用模块
*
*  文件名称    : OPC.h
*
*  功能简介    : 处理OPC软件命令和相关配置
*
*  作者        : 王东
*
*  创建日期    : 2016-02-16
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef OPC_H_
#define OPC_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "ExterComm/inc/ComCommon.h"
#include "ExterComm/inc/ExterComm.h"
#include "ExterComm/inc/PCSWCommon.h"
#include "Task/inc/MainTask.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_SWOPC

#define DEBUG_OUT_OPC

#ifdef DEBUG_OUT_OPC
#define DEBUG_OPC(format, args...)  printf ("DB_OPC: ") ; printf(format, ##args)
#else
#define DEBUG_OPC(format, args...)
#endif

#define OPC_PM_TIMEOUT_CYCLE_CNT  (5*1000*1000/(MAIN_TASK_PERIOD))   /* PM应答OPC命令的超时拍数：5s */
#define OPC_SW_TIMEOUT_CYCLE_CNT  (30*1000*1000/(MAIN_TASK_PERIOD))  /* OPC软件超时拍数: 30s */
#define OPC_MAX_CYCLE_COPY_LEN    (10*1024)           /* OPC每周期最大允许拷贝配置的字节数 */
#define OPC_MAX_READ_LEN          (8000U-16U-4U-20U)  /* OPC读变量时，每帧最多能存放的变量值长度-字节 */

/* Index */
#define OPC_READ_CNT_INDEX        (16U+6U)            /* OPC读变量应答帧：Count域索引值 */

/******************** 通信服务相关开始 ********************/
/* Request Message Type Range */
#define OPC_ATYPE_MIN       (2001)  /* Minimum */
#define OPC_ATYPE_MAX       (2500)  /* Maximum */

/* Request Message Type：CM Handle */
#define OPC_LOGIN           (2001)  /* 登录服务 */
#define OPC_LOGOUT          (2002)  /* 退出登录服务 */
#define OPC_READ            (2003)  /* 读变量服务 */

/* Request Message Type：PM Handle */
#define OPC_WRITE           (2004)  /* 写变量服务*/

/******************** 通信服务相关结束 ********************/

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* OPC Info.  OPC软件信息 */
typedef struct OPCInfoTag
{
    NetworkPortType_t emNetwork;  /* Network                   网口号 */
    uint32_t uiIPAddr;            /* IP Address                数据源的IP地址 */
    uint16_t usPort;              /* Port                      数据源的端口号 */
    uint8_t ucAccAuth;            /* Access Authority          访问权限 */
    uint8_t ucReserve1;           /* Reserve 1                 预留1 */
    LogStatus_t emLog;            /* Log Status                登录状态 */
    SelPMID_t emSelPMID;          /* Selected PM ID            已选取的PM模块的编号 */
    uint32_t uiWaitReqCycleCnt;   /* Wait Request Cycle Count  已等待软件请求的拍数 */
    uint16_t usVarIndex;          /* Read Variable Index       已读取OPC变量的索引值 */
    uint16_t usSequence;          /* Sequence                  序列号 */
    uint16_t usMsgIndex;          /* Message Index             本包序号 */
    uint8_t ucReserve2[2];        /* Reserve 2                 预留2 */
}OPCInfo_t;

/* Network Info. 网口信息结构体定义 */
typedef struct NetworkPortInfoTag
{
    uint8_t ucIndex;                                    /* Index 当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];                               /* Reserve 预留 */
    uint32_t uiCMCfgCRC;                                /* CM配置文件CRC */
    uint32_t uiCMCfgVer;                                /* CM配置文件的版本 */
    OPCNetConfigInfo_t stNetConfig[LOCAL_CFG_VER_CNT];  /* Network Configuration Info. 网口配置信息 */
}OPCNetworkInfo_t;

/* OPC Variable Table Info.  OPC变量表信息结构体定义 */
typedef struct OPCVarTblInfoTag
{
    uint8_t ucIndex;                               /* Index                     当前使用的配置参数的索引值 */
    bool_t bValid;                                 /* Valid Flag                当前配置是否有效：拷贝期间/参数超限：无效 */
    bool_t bCopying;                               /* Copying                   是否正在拷贝 */
    uint8_t ucReserve;                             /* Reserve                   预留 */
    uint32_t uiOPCCfgCRC;                          /* OPC Configuration CRC     OPC配置文件CRC */
    uint32_t uiOPCCfgVer;                          /* OPC Configuration Version OPC配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;                   /* Real-time Data Version    实时数据的版本 */
    OPCVarInfo_t stOPCVarInfo[LOCAL_CFG_VER_CNT];  /* OPC Variable Info.        OPC变量表信息 */
}OPCVarTblInfo_t;

/******************** Communication Data Structure ********************/
/* OPC Read Response Head  OPC读变量服务的应答头信息结构体定义 */
typedef struct OPCReadRespHeadTag
{
    uint32_t uiSec;       /* Second                时间-秒 */
    uint16_t usMilliSec;  /* Millisecond           时间-毫秒 */
    uint16_t usSeq;       /* Sequence              序列号 */
    uint16_t usTotalNum;  /* Total Number          总包数 */
    uint16_t usMsgIndex;  /* Message Index         本包的序号 */
    uint8_t ucReserve[4]; /* Reserve               预留 */
    uint16_t usVarCnts;   /* Variable Counts       本包中变量的个数 */
    uint16_t usFirIndex;  /* First Variable Index  本包中第一个变量的索引值 */
}OPCReadRespHead_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: OPCInit
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

void OPCInit( void );

/*
***********************************************************************************************************************
* 函数名称: OPCCycle
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

void OPCCycle( void );

#endif /*OPC_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


