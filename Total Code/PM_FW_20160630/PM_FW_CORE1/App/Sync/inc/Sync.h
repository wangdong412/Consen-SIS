/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 同步模块
*
*  文件名称    : Sync.h
*
*  功能简介    : 完成对工程文件的配置
*
*  作者           : 刘阳
*
*  创建日期    : 2015-12-30
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYNC_H_
#define SYNC_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/

#include"Srv/SysResource/inc/SysCommon.h"
#include"Srv/SysResource/inc/SysFileManager.h"
#include"Srv/SysResource/inc/SysSync.h"
#include"Srv/SysResource/inc/SysStateMngt.h"
#include"Srv/CommProtocol/inc/PMBusProtocol.h"
#include"../../Comm/inc/Comm.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define DEBUG_OUT_SYNC
#ifdef DEBUG_OUT_SYNC
#define DEBUG_SYNC(format, args...)  printf ("DEBUG_SYNC:  ") ; printf(format, ##args)
#else
#define DEBUG_SYNC(format, args...)
#endif





/*此处是否安全协议层关心，具体小包大小，最大包数等*/

#define MAX_IO_PACK_SIZE                46                /* 每包用户IO数据大小           */
#define MAX_STAT_PACK_SIZE              46                /* 每包状态数据大小             */
#define MAX_IO_PACK_COUNT               38                /* 每包发送IO数据最大包数       */
#define MAX_STAT_PACK_COUNT             4                 /* 每包发送状态数据最大包数     */


#define MAX_IO_USER_DATA_SIZE           (MAX_IO_PACK_SIZE * MAX_IO_PACK_COUNT)
#define MAX_STAT_USER_DATA_SIZE         (MAX_STAT_PACK_SIZE * MAX_STAT_PACK_COUNT)
/* 最大IO数据交换长度 */
#define MAX_IO_SYNC_SIZE                (1024*2)          /* IO数据交换数据大小*/
/* 最大数据交换长度 */
#define MAX_SRV_SYNC_SIZE               (1024*4)          /* 服务数据同步数据大小*/
#define MAX_PRJ_SYNC_SIZE               (1024*2 - sizeof(uint32_t))          /* 工程数据同步数据大小*/

#define CFG_MAX_SYNC_SRV_SIZE           (MAX_SRV_SYNC_SIZE - sizeof(SysPMStateInfo_t) - 6*sizeof(uint32_t))

/* 最大同步数据长度 */
#define CFG_MAX_SYNCDATA_LEN            (1024*6U)
/* 同步文件头长度 */
#define SYNC_FILE_HEADSIZE              (sizeof(SyncFileHead_t) + sizeof(SyncHead_t) + sizeof(uint32_t))
/* 同步文件最大文件长度 */
#define CFG_MAX_SYNC_FILE_SIZE          (MAX_PRJ_SYNC_SIZE - SYNC_FILE_HEADSIZE)
/* 同步数据头长度 */
#define SYNC_DATA_HEADSIZE              (sizeof(SyncDataHead_t) + sizeof(SyncHead_t) + sizeof(uint32_t))
/* 同步数据最大数据长度 */
#define CFG_MAX_SYNC_DATA_SIZE          (MAX_PRJ_SYNC_SIZE - SYNC_DATA_HEADSIZE)
/* 最大静态数据应答长度 */
#define CFG_MAX_STATIC_ACK_SIZE         (MAX_PRJ_SYNC_SIZE - sizeof(SyncHead_t) - sizeof(uint32_t))
/* 链路A收到数据标识位 */
#define SYNC_SRVACK_HANDLE_BITA         (0x00000000U)
/* 链路B收到数据标识位 */
#define SYNC_SRVACK_HANDLE_BITB         (0x00000001U)
/* 未收到服务应答数据 */
#define SYNC_SRVACK_NOHANDLE            (0x00000000U)
/* 接收大小为0 */
#define SYNC_RECV_SIZE_0                ((int16_t)0)
/* 状态数据处理OK */
#define SYNC_HANDLE_STATDATA_OK         (0x00000000U)
/* 状态数据处理类型错误 */
#define SYNC_HANDLE_STATDATA_TYPE_ERR   (0x00000001U)
/* PM信息ID错误 */
#define SYNC_HANDLE_CTRLDATA_IDERR      (0x00000002U)
/* PM信息应答错误 */
#define SYNC_HANDLE_CTRLDATA_ACKERR     (0x00000003U)
/* 接收PM信息缓存为空指针 */
#define SYNC_HANDLE_CTRLDATA_BUFFERR    (0x00000004U)
/* 处理服务应答错误 */
#define SYNC_HANDLE_SRVDATA_ACKERR      (0x00000005U)
/* 找到下一个同步文件 */
#define HAS_NEXT_FILE                   (0x00000000U)
/* 没有文件需要同步 */
#define HAS_NONEXT_FILE                 (0x00000001U)
/* 找到下一个数据同步区域号 */
#define HAS_NEXT_DATA                   (0x00000000U)
/* 没有数据需要同步 */
#define HAS_NONEXT_DATA                 (0x00000001U)
/* 应答接收OK */
#define ACK_DATA_OK                     (0x00000000U)
/* 应答接收错误 */
#define ACK_DATA_ERR                    (0x00000001U)
/* 启动时等待时间 (2000ms) 单位1us */
#define SYNC_START_WAITTIME             (2000000U)
/* 启动时超时时间 (3000ms) 单位1us */
#define SYNC_START_TIMEOUT              (3000000U)
/* 同步定时器应答超时时间 (5000ms) 单位是100us */
#define SYNC_TIMER_TIMEOUT              (50000U)



/*******************全局的结构，需要移植到系统资源*******************/




/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/



/* 发送文件类型 */
typedef enum
{
    NEW_FILE = (0x00000000U),                           /* 新文件 */
    NOT_NEW_FILE                                        /* 文件发送继续 */
}file_send_type_t;

/* IEC工程数据区定义 */
typedef enum
{
    AREA_G_ID = 0x00000000U,                            /* 数据区:G区 id = 0 */
    AREA_Q_ID,                                          /* 数据区:输出区 id = 1 */
    AREA_I_ID,                                          /* 数据区:输入区 id = 2 */
    MAX_AREA_NUM                                        /* max area number = 11 */
}data_areaid_t;

/* 状态数据类型 */
typedef enum
{
    SRV_DATA = 0x00000000U,                             /* 服务数据类型:服务数据     id = 0 */
    SRVACK_DATA,                                        /* 服务数据类型:服服务应答数据 id = 1 */
    CTRL_DATA
}srv_data_type_t;





/* 同步服务类型 */
typedef enum
{
    SYNC_FILE = (0x00000000U),                           /* 同步静态文件 */
    SYNC_REAL_DATA,                                      /* 同步静态数据 */
    SYNC_COMPLETE                                        /* 同步静态数据完成 */
}sync_srv_type_t;

/* 数据包类型 */
typedef enum
{
    LAST_PACK = (0x00000000U),                          /* 最后一包数据 */
    NOT_LAST_PACK                                       /* 非最后一包数据 */
}pack_type_t;


/* 同步文件记录状态结构体定义 */
typedef struct SyncFileStatusTag
{
    SysCfgFileType_t emCfgFileType;                     /* 文件ID号            */
    uint32_t uiFileSize;                                /* 文件大小              */
    uint32_t uiSendSize;                                /* 已发送文件长度        */
    file_send_type_t emStartFlag;                       /* 发送新文件标识        */
    uint8_t ucFileName[MAX_FILE_NAME_LEN+1];            /* 文件名称              */
}SyncFileStatus_t;


/* 同步数据记录状态结构体定义 */
typedef struct SyncDataStatusTag
{
    Task_ID_t emTaskID;
    SysRtDataType_t emAreaId;                             /* 数据区域号            */
    uint32_t uiCurOffset;                               /* 当前同步数据区偏移    */
    uint32_t uiCurDataLen;
}SyncDataStatus_t;

/* 同步参数记录状态结构体定义 */
typedef struct SyncParamStatusTag
{
    uint32_t uiCurOffset;                               /* 当前同步参数区偏移    */
}SyncParamStatus_t;



/* 同步定时器结构体定义 */
typedef struct SyncTimerStatusTag
{
    SyncTime64_t unTaskStartTime;                       /* 同步任务服务启动时间  */
}SyncTimerStatus_t;



/* 同步数据结构 */
typedef struct SyncOneSyncDataTag
{
    srv_data_type_t emType;                            /* 状态数据类型          */
    srv_srvdata_type_t emSubType;                      /* 服务数据子类型        */
    ack_flag_t emAckFlag;                               /* 是否需应答标识        */
    uint8_t ucData[CFG_MAX_SYNCDATA_LEN];               /* 实际数据              */
}SyncOneSyncData_t;



/* PM服务结构 */
typedef struct SyncCtrlSrvTag
{
    uint32_t uiSrvId;                                   /* PM服务号               */
    SyncTime64_t unActiveTime;                          /* PM服务激活时间         */
    SyncTime64_t unTimeOutTime;                         /* PM服务等待应答超时时间 */
}SyncCtrlSrv_t;
/**/

/* 同步服务头结构 */
typedef struct SyncHeadTag
{
    sync_srv_type_t emSrvType;                          /* 服务类型              */
}SyncHead_t;

/* 同步文件头结构 */
typedef struct SyncFileHeadTag
{
    uint8_t ucFileName[MAX_FILE_NAME_LEN+1];            /* 文件名                */
    SysCfgFileType_t emCfgFileType;                     /* 文件ID    */
    pack_type_t emLastPackFlag;                         /* 最后一包标识          */
    uint32_t uiFileOffset;                              /* 文件偏移              */

}SyncFileHead_t;

/* 同步文件数据结构 */
typedef struct SyncFileTag
{
    SyncHead_t stSyncHead;                              /* 同步服务头            */
    SyncFileHead_t stFileHead;                          /* 同步文件头            */
    uint8_t ucData[CFG_MAX_SYNC_FILE_SIZE];             /* 文件数据              */
}SyncFile_t;

/* 同步工程数据头结构 */
typedef struct SyncDataHeadTag
{
    Task_ID_t emTaskID;
    SysRtDataType_t emAreaId;                             /* 数据区域号            */
    uint32_t uiOffset;                                  /* 数据区域偏移          */
    pack_type_t emLastPackFlag;                         /* 是否最后一包标识      */
    
}SyncDataHead_t;

/**/

/* 同步工程数据结构 */
typedef struct SyncDataTag
{
    SyncHead_t stSyncHead;                              /* 同步服务头            */
    SyncDataHead_t stDataHead;                          /* 同步工程数据头结构    */
    uint8_t ucData[CFG_MAX_SYNC_DATA_SIZE];             /* 工程数据内容          */
}SyncRealData_t;

/* 同步IO参数数据头结构 */
typedef struct SyncParamHeadTag
{
    uint32_t uiOffset;                                  /* 参数区域偏移          */
    pack_type_t emLastPackFlag;                         /* 是否最后一包标识      */
}SyncParamHead_t;

/* 同步IO参数数据结构 */
typedef struct SyncIoParamTag
{
    SyncHead_t stSyncHead;                              /* 同步服务头            */
    SyncParamHead_t stParamHead;                        /* 同步参数数据头结构    */
    uint8_t ucData[CFG_MAX_SYNC_DATA_SIZE];             /* 参数数据内容          */
}SyncIoParam_t;

/* 静态应答结构 */
typedef struct SyncStaticAckTag
{
    SyncHead_t stSyncHead;                              /* 同步服务头            */
    uint8_t ucData[CFG_MAX_STATIC_ACK_SIZE];            /* 应答数据              */
}SyncStaticAck_t;

/* 同步数据类型 */
typedef enum
{
    S_DATA_NULL = 0x00000000U,                            /* 同步数据类型:除状态数据外无其他数据 id = 0 */
    S_SRV_DATA,                                           /* 同步数据类型:服务数据     id = 1 */
    S_SRVACK_DATA,                                        /* 同步数据类型:服务应答数据 id = 2 */
    S_PRG_DATA,                                           /* 同步数据类型:工程数据 id = 3 */
}sync_data_type_t;



typedef struct SrvDataTag
{
    srv_data_type_t emType;                             /* 服务数据类型            */
    srv_srvdata_type_t emSubType;                       /* 服务数据子类型        */
    CMController_t emCMid;                              /* 服务来源的CM的id号*/
    ack_flag_t emAckFlag;                               /* 是否需应答标识        */
    uint32_t uiSrvDataLen;
    uint8_t ucData[CFG_MAX_SYNC_SRV_SIZE];              /* 具体数据        */
}SrvData_t;

typedef struct SyncSrvDataTag
{
    uint32_t uiSyncSrvDataLen;
    SysPMStateInfo_t stPMState;                          /* PM用于同步的信息，具体数据结构待定*/
    SrvData_t stSrvData;                                 /* 服务数据同步结构 */
}SyncSrvData_t;

typedef struct SyncPrjDataTag
{
    uint32_t uiSyncPrjDataLen;
    uint8_t ucPrjData[MAX_PRJ_SYNC_SIZE];

}SyncPrjData_t;

typedef struct SyncMSGTag
{/*内容分为：
        服务数据：包含数据长度，PM信息，服务数据。总长度：1024*8
        工程数据：包含数据长度，数据类型，工程数据（工程文件或实时数据）。总长度：1024*2    */
    SyncSrvData_t stSyncSrvData;

    SyncPrjData_t stSyncPrjData;

}SyncMSG_t;

/* 两条链路的状态数据结构 */
typedef struct SyncSyncDataTag
{
    int16_t sLinkASrvLen;                             /* 链路A接收服务数据长度 */
    int16_t sLinkBSrvLen;                             /* 链路A接收服务数据长度 */
    int16_t sLinkAPrjLen;                             /* 链路A接收工程数据长度 */
    int16_t sLinkBPrjLen;                             /* 链路A接收工程数据长度 */
    int16_t sLinkASrvErr;                             /* 链路A接收服务数据错误号       */
    int16_t sLinkBSrvErr;                             /* 链路B接收服务数据错误号       */
    int16_t sLinkAPrjErr;                             /* 链路A接收工程数据错误号       */
    int16_t sLinkBPrjErr;                             /* 链路B接收工程数据错误号       */
    SyncMSG_t stSyncMSGA;                               /* 链路A接收同步数据缓存 */
    SyncMSG_t stSyncMSGB;                               /* 链路B接收同步数据缓存 */
}SyncData_t;




/*******************全局的结构，需要移植到系统资源*******************/


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncInit
*
* 功能描述: 初始化同步记录信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SyncInit(void);
/*
*********************************函数描述******************************************************************************
* 函数名称: SyncMngt
*
* 功能描述: 对同步任务进行周期管理，完成三冗余功能
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SyncMngt(void);


#endif /*SYNC_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
