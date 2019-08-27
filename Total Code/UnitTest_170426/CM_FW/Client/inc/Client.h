/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : Client软件应用模块
*
*  文件名称    : Client.h
*
*  功能简介    : 处理Client软件命令和相关配置
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

#ifndef CLIENT_H_
#define CLIENT_H_

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
#include "ExterComm/inc/FileUpTask.h"
#include "Task/inc/MainTask.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_SWCLI

#define DEBUG_OUT_CLIENT

#ifdef DEBUG_OUT_CLIENT
#define DEBUG_CLI(format, args...)  printf ("DB_CLI: ") ; printf(format, ##args)
#else
#define DEBUG_CLI(format, args...)
#endif

#define CLIENT_PM_TIMEOUT_CYCLE_CNT  (2*1000*1000/(MAIN_TASK_PERIOD))   /* PM应答Client命令的超时拍数：2s */
#define CLIENT_SW_TIMEOUT_CYCLE_CNT  (30*1000*1000/(MAIN_TASK_PERIOD))  /* Client软件超时拍数: 30s */
#define MAX_SRC_CNT                  (4)                    /* 命令来源的最大个数：4个客户端软件 */
#define MAX_PER_READ_EVENT_CNT       (100)                  /* 一次最多读100条事件 */
#define MAX_CRC_STR_LEN              (100)                  /* CRC字符串的最大长度，不包括终止符 */
#define MAX_CRC_STR_BUFF_LEN         ((MAX_CRC_STR_LEN)+1)  /* CRC字符串的最大长度，不包括终止符 */

/******************** 通信服务相关开始 ********************/
/* Request Message Type Range */
#define CLI_ATYPE_MIN       (2501)  /* Minimum */
#define CLI_ATYPE_MAX       (3500)  /* Maximum */

/* Request Message Type：CM Handle */
#define CLI_LOGIN           (2501)  /* 登录服务 */
#define CLI_LOGOUT          (2502)  /* 退出登录服务 */
#define CLI_READ_DIAG       (2506)  /* 读诊断信息服务 */
#define CLI_READ_VERN       (2508)  /* 读版本信息服务 */
#define CLI_READ_IPADDR     (2509)  /* 读IP地址服务：通过CM/PM上的调试口（串口）通讯，谁收到谁处理 */

/* Request Message Type：PM Handle */
#define CLI_READFILE_START  (2503)  /* 读硬件配置文件开始服务 */
#define CLI_READFILE_CONT   (2504)  /* 读硬件配置文件继续服务 */
#define CLI_CLR_DIAG        (2507)  /* 清诊断信息服务 */

/* Request Message Type：PM/CM Handle */
#define CLI_READ_EVENT      (2505)  /* 读事件服务 */
#define CLI_FWUP_START      (2510)  /* 固件版本更新开始服务 */
#define CLI_FWUP_CONT       (2511)  /* 固件版本更新继续服务 */
#define CLI_FWUP_END        (2512)  /* 固件版本更新完成服务 */

/******************** 通信服务相关结束 ********************/

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Client Info.  Client软件信息 */
typedef struct ClientInfoTag
{
    NetworkPortType_t emNetwork;   /* Network           网口号 */
    uint32_t uiIPAddr;             /* IP Address        数据源的IP地址 */
    uint16_t usPort;               /* Port              数据源的端口号 */
    uint8_t ucAccAuth;             /* Access Authority  访问权限 */
    uint8_t ucReserve;             /* Reserve           预留 */
    LogStatus_t emLog;             /* Log Status        登录状态 */
    SelPMID_t emSelPMID;           /* Selected PM ID    已选取的PM模块的编号 */
    uint32_t uiWaitReqCycleCnt;    /* 已等待软件请求的拍数 */
}ClientInfo_t;

/* Client Serial Info.  Client软件串口信息 */
typedef struct CliSerlInfoTag
{
    LogStatus_t emLog;             /* Log Status      登录状态 */
    SelPMID_t emSelPMID;           /* Selected PM ID  已选取的PM模块的编号 */
    uint32_t uiWaitReqCycleCnt;    /* 已等待软件请求的拍数 */
    uint32_t uiWaitPMCycleCnt;     /* 等待PM应答的超时拍数 */
}CliSerlInfo_t;

/* Module Type 模块类型枚举定义 */
typedef enum ModuleTypeTag
{
    MT_CM = 0,   /* CM  CM模块 */
    MT_PM,       /* PM  PM模块 */
    MT_IO        /* IO  I/O模块 */
}ModuleType_t;

/* FW Type 固件类型枚举定义 */
typedef enum FWTypeTag
{
    FW_CMFPGA = 0,
    FW_CMRTS,
    FW_CMRTSCRC,
    FW_PMFPGA,
    FW_PMCORE0RTS,
    FW_PMCORE1RTS,
    FW_PMCORE0RTSCRC,
    FW_PMCORE1RTSCRC,
    FW_IOAPP,
    FW_IOFPGA,
    FW_IOL2BOOT1,
    FW_IOL2BOOT2
}FWType_t;

/* Select PM Method Type  选取PM方法类型枚举定义 */
typedef enum SelPMMethodTag
{
    SPM_RESEL = 0, /* Re-Select  重新选择 */
    SPM_KEEP,      /* Keep       使用之前的选择结果 */
    SPM_SPEY,      /* Specify    使用指定的PM */
    SPM_INVALID    /* Invalid    无效选取方法 */
}SelPMMethod_t;

/* Section Info. ID  诊断/版本信息区枚举定义 */
typedef enum InfoSecIDTag
{
    ISID_INVALID = 0, /* Invalid  无效区 */
    ISID_PM,          /* PM       PM区 */
    ISID_CM,          /* CM       CM区 */
    ISID_BI,          /* BI       BI区 */
    ISID_IO           /* IO       IO区 */
}InfoSecID_t;

/* Module ID 模块ID枚举定义 */
typedef enum ModuleIDTag
{
    MID_PMA = 0,  /* PMA  PMA模块 */
    MID_PMB,      /* PMB  PMB模块 */
    MID_PMC,      /* PMC  PMC模块 */
    MID_CM,       /* CM   CM模块 */
}ModuleID_t;

#pragma pack(1)
/* Client Info.  Client软件信息 */
typedef struct FWInfoTag
{
    uint8_t ucModuleType;  /* Module Type  模块类型 */
    uint8_t ucPrgType;     /* FW Type      固件程序类型 */
    uint8_t ucIOSID;       /* I/O Slot ID  I/O模块的编号 */
    uint8_t ucPMID;        /* PM ID        PM模块的编号 */
}FWInfo_t;

/* Read Event Info. 读事件信息 */
typedef struct ReadEventInfoTag
{
    uint8_t ucModuleID;   /* Module ID  模块ID */
    uint8_t ucBlkID;      /* Block ID   块ID */
    uint16_t usGetIndex;  /* Get Index  事件索引值 */
}ReadEventInfo_t;

/* Read Request  读信息结构体定义 */
typedef struct ReadReqTag
{
    uint8_t ucPMID;        /* PM ID    PM模块的编号 */
    uint8_t ucReserve[3];  /* Reserve  预留 */
}ReadReq_t;

/* Read IP Address  读IP地址结构体定义 */
typedef struct ReadIPTag
{
    uint8_t ucCMSlotID;    /* CM Slot ID  CM槽号 */
    uint8_t ucReserve[3];  /* Reserve     预留 */
}ReadIP_t;

/* Read Event Response  读事件应答结构体定义 */
typedef struct ReadEventRespTag
{
    uint8_t ucModuleID;     /* Module ID  模块ID */
    uint8_t ucBlkID;        /* Block ID   块ID */
    uint16_t usNextIndex;   /* Next Index  可被请求的下一事件的索引值 */
    uint16_t usEventCount;  /* Event Count 事件的数目 */
}ReadEventResp_t;
#pragma pack()

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ClientInit
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

void ClientInit( void );

/*
***********************************************************************************************************************
* 函数名称: ClientCycle
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

void ClientCycle( void );

#endif /*CLIENT_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


