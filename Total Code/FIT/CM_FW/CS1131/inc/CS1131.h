/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : CS1131软件应用模块
*
*  文件名称    : CS1131.h
*
*  功能简介    : 处理CS1131软件命令和相关配置
*
*  作者        : 王东
*
*  创建日期    : 2016-01-15
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef CS1131_H_
#define CS1131_H_

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
#include "LED/inc/LED.h"
#include "ExterComm/inc/FileUpTask.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_SWCS1131

#define DEBUG_OUT_CS1131

#ifdef DEBUG_OUT_CS1131
#define DEBUG_CS1131(format, args...)  printf ("DB_1131: ") ; printf(format, ##args)
#else
#define DEBUG_CS1131(format, args...)
#endif

#define CS1131_PM_TIMEOUT_CYCLE_CNT  (5*1000/32)          /* PM应答CS1131命令的超时拍数：2000ms, Note: 2000/8 */
#define CS1131_SW_TIMEOUT_CYCLE_CNT  (30*1000/32)         /* CS1131软件超时拍数: 30s */
#define CS1131_MAX_MON_CNT           (1000)               /* 监视变量表中变量的最大个数：数值待定 */
#define MAX_FIME_NAME_LEN            (255)                /* 文件名的最大长度 */
#define MAX_MSG_FILEDATA_LEN         ((MAX_MSG_LEN)-23U)  /* 单个消息中文件数据的最大长度: 23:16,3,4 */

/******************** 通信服务相关开始 ********************/
/* Task Info. */
#define INVALID_TASK_NUM       (0xFF)  /* Invalid Task Number 无效任务数目 */
#define INVALID_TASK_ID        (0xFF)  /* Invalid Task ID     无效任务ID */
#define INVALID_TASK_STATUS    (0xFF)  /* Invalid Task Status 无效任务状态 */

/* Request Message Type Range */
#define CS1131_ATYPE_MIN       (1001)  /* Minimum */
#define CS1131_ATYPE_MAX       (1500)  /* Maximum */

/* Request Message Type：CM Handle */
#define CS1131_LOGIN           (1002)  /* 登录服务 */
#define CS1131_LOGOUT          (1028)  /* 退出登录服务 */
#define CS1131_CLR_SRCFILE     (1027)  /* 清除源文件服务 */
#define CS1131_READ_STATUS     (1029)  /* 读取CM及PM状态服务 */
#define CS1131_DEF_VARTABLE    (1030)  /* 定义监视变量表服务 */
#define CS1131_READ_VARONLINE  (1031)  /* 读变量在线值服务 */
#define CS1131_READ_STAID      (1036)  /* 读取站节点服务 */
#define CS1131_READ_IO_STATUS  (1040)  /* 读取所有IO模块状态服务 */

/* Request Message Type：PM Handle */
#define CS1131_DOWNLOAD_START  (1012)  /* 清空/增量下装服务 */
#define CS1131_DOWNLOAD_END    (1039)  /* 清空/增量下装确认服务*/
#define CS1131_CHECK_EXTERLIB  (1001)  /* 检查外部库版本服务 */
#define CS1131_START_RUN       (1008)  /* 开始运行服务 */
#define CS1131_STOP_RUN        (1009)  /* 停止运行服务 */
#define CS1131_PAUSE_RUN       (1010)  /* 暂停运行服务 */
#define CS1131_WRITE_VAR       (1011)  /* 写变量服务 */
#define CS1131_SINGLE_STEP     (1013)  /* 单步运行服务 */
#define CS1131_READ_FORCE      (1015)  /* 读强制列表服务 */
#define CS1131_DISABLE_VAR     (1018)  /* Disable变量服务 */
#define CS1131_START_OSC       (1033)  /* 启动示波器服务 */
#define CS1131_STOP_OSC        (1034)  /* 停止示波器服务 */
#define CS1131_NEED_OSC        (1035)  /* 示波器配置是否需要服务 */
#define CS1131_IO_RECONN       (1041)  /*  IO模块重连确认服务 */

/* Request Message Type：PM/CM Handle */
#define CS1131_WRITEFILE_START (1021)  /* 写文件开始服务 */
#define CS1131_WRITEFILE_CONT  (1022)  /* 写文件继续服务 */
#define CS1131_WRITEFILE_END   (1023)  /* 写文件完成服务 */
#define CS1131_READFILE_START  (1025)  /* 读文件开始服务 */
#define CS1131_READFILE_CONT   (1026)  /* 读文件继续服务 */

/* Data Area Index */
#define FILE_TYPE_INDEX        (16)    /* 文件类型索引值 */

/******************** 通信服务相关结束 ********************/

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Variable Information 变量信息结构体定义 */
typedef struct VarInfoTag
{
    uint8_t ucTaskID;    /* Task ID       第一个出错变量所属的任务ID */
    uint8_t ucRefID;     /* RefID         第一个出错变量所属的区域ID */
    uint16_t usVarSize;  /* Variable Size 变量大小，0代表Bit */
    uint32_t uiOffset;   /* Offset        第一个出错变量在相应区域中的偏移量 */
}VarInfo_t;

/* Monitor Variable Table Info.  监视变量表信息 */
typedef struct MonVarTableInfoTag
{
    uint16_t usMonVarCnt;  /* 监视变量的实际个数 */
    uint8_t ucReserve[2];
    VarInfo_t stMonVarTable[CS1131_MAX_MON_CNT];  /* 软件监视变量表 */
}MonVarTableInfo_t;

/* CS1131 Info.  CS1131软件信息 */
typedef struct CS1131InfoTag
{
    NetworkPortType_t emNetwork;    /* Network 网口号 */
    uint32_t uiIPAddr;              /* IP Address 数据源的IP地址 */
    uint16_t usPort;                /* Port 数据源的端口号 */
    uint8_t ucAccAuth;              /* Access Authority  访问权限 */
    uint8_t ucReserve;              /* Reserve 预留 */
    LogStatus_t emLog;              /* Log Status 登录状态 */
    SelPMID_t emSelPMID;            /* Selected PM ID  已选取的PM模块的编号 */
    uint32_t uiWaitReqCycleCnt;     /* 已等待软件请求的拍数 */
    MonVarTableInfo_t stMonTblInfo; /* 监视变量表信息 */
    uint32_t uiSrcFileROffset;      /* 读源文件的字节偏移 */
}CS1131Info_t;

/* File Type 文件类型枚举定义 */
typedef enum FileTypeTag
{
    FT_INVALID = 0,  /* Invalid  无效文件类型 */
    FT_BINARY,       /* Binary   二进制文件 */
    FT_SRC,          /* Source   源文件 */
    FT_SYMTBL        /* Symbol Table  符号表文件 */
}FileType_t;

/* Status Request Type 状态请求类型枚举定义 */
typedef enum StsReqTypeTag
{
    SRT_ALL = 0,     /* All 请求CM及PM状态 */
    SRT_CM,          /* CM  请求CM状态 */
    SRT_PM,          /* PM  请求PM状态 */
    NUM_OF_REQTYPE   /* Number Of Request Type 请求类型的最大有效值 */
}StsReqType_t;

/* Variable Error Info. 变量错误统计信息结构体定义 */
typedef struct VarErrInfoTag
{
    uint16_t usErrCnts;      /* Error Counts       出错变量的个数 */
    uint16_t usFstErrIndex;  /* First Error Index  第一个出错变量的索引值 */
    VarInfo_t stVarInfo;     /* Variable Info.     第一个出错变量 */
}VarErrInfo_t;

/* Total Project Version Info. 总工程版本信息结构体定义 */
typedef struct TotalProjVerInfoTag
{
    bool_t bPMAProjValid;        /* PMA Project Valid Flag  PMA工程有效标志位 */
    bool_t bPMBProjValid;        /* PMB Project Valid Flag  PMB工程有效标志位 */
    bool_t bPMCProjValid;        /* PMC Project Valid Flag  PMC工程有效标志位 */
    uint8_t ucReserve;           /* Reserve                 预留 */
    PMController_t emSelPMID;    /* Select PMID             选取的PMID */
}TotalProjVerInfo_t;

/* Task Info. 任务信息结构体定义 */
typedef struct TaskStatusInfoTag
{
    uint8_t ucTaskID;      /* Task ID      任务ID */
    int8_t icTaskStatus;   /* Task Status  任务状态， -1代表异常 */
    uint16_t usReserve;    /* Reserve      预留 */
}TaskStatusInfo_t;

/* PM Task Status Info. PM任务状态信息结构体定义 */
typedef struct PMTaskStatusInfoTag
{
    TaskStatusInfo_t stVTaskInfo;    /* Voted Task Info.  表决后的任务信息 */
    TaskStatusInfo_t stPMATaskInfo;  /* PMA Task Info.    PMA任务信息 */
    TaskStatusInfo_t stPMBTaskInfo;  /* PMB Task Info.    PMB任务信息 */
    TaskStatusInfo_t stPMCTaskInfo;  /* PMC Task Info.    PMC任务信息 */
}PMTaskStatusInfo_t;

/* Task Info. PM任务信息结构体定义 */
typedef struct PMTaskInfoTag
{
    bool_t bWait;                                   /* Wait Flag         等待标志位 */
    bool_t bValid;                                  /* Valid Flag        有效标志位 */
    int8_t icPMTaskNum;                             /* PM Voted Task Number CM表决后的任务数目：任务数目不一致时，设置为-1 */
    uint8_t ucReserve;                              /* Reserve           预留 */
    PMTaskStatusInfo_t stPMTaskInfo[MAX_TASK_NUM];  /* Voted Task Info.  表决后的任务信息 */
}PMTaskInfo_t;

/* Download Info. 下装信息结构体定义 */
typedef struct DownloadInfoTag
{
    uint8_t ucSWIndex;     /* Software Index   软件索引值 */
    bool_t bDownload;      /* Download Flag    下装标志位 */
    bool_t bConfirm;       /* Confirm Flag     下装确认标志位 */
    uint8_t ucDownStatus;  /* Download Status  预留 */
}DownloadInfo_t;

/* Write Source File Info. 写源文件信息结构体定义 */
typedef struct WriteSrcFileInfoTag
{
    uint8_t ucSWIndex;     /* Software Index   正在写源文件的软件索引值 */
    bool_t bWrite;         /* Write Flag       写标志位 */
    uint8_t ucReserve[2];  /* Download Status  预留 */
}WriteSrcFileInfo_t;

/* Read Online Data Info. 读变量在线值信息结构体定义 */
typedef struct ReadOnlineDataInfoTag
{
    uint32_t uiSecond;      /* Seconde          时间 s */
    uint16_t usMiliSecond;  /* MiliSecond       时间 ms */
    uint16_t usVarCnts;     /* Variable Counts  变量数目 */
}ReadOnlineDataInfo_t;

/* Write File Info. 写文件信息结构体定义 */
typedef struct WriteFileInfoTag
{
    uint8_t ucFileType;  /* File Type  文件类型1/2/3 */
    uint8_t ucFileID;    /* File ID    文件ID */
    uint16_t usDataLen;  /* DataLen    文件数据的长度 */
}WriteFileInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CS1131Init
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

void CS1131Init( void );

/*
***********************************************************************************************************************
* 函数名称: CS1131Cycle
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

void CS1131Cycle( void );

#endif /*CS1131_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


