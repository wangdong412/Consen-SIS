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
*  文件名称    : SysCfgFile.h
*
*  功能简介    : 配置文件读写
*
*  作者       : 李琦
*
*  创建日期    : 2016-03-17
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_CFG_FILE_H_
#define SYS_CFG_FILE_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "Srv/Sys/inc/CommSys.h"


/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define MAX_CFG_CM_LEN               10000UL            /* CM模块配置文件缓冲区长度*/
#define MAX_CFG_MODBUS_SLAVE_LEN      1000UL            /* Modbus slave配置文件缓冲区长度*/
#define MAX_CFG_MODBUS_MASTER_LEN     1000UL            /* Modbus master配置文件缓冲区长度*/
#define MAX_CFG_OPC_LEN               1000UL            /* OPC配置文件缓冲区长度*/
#define MAX_CFG_CTRL_STAION_LEN      10000UL            /* 控制站配置文件缓冲区长度*/
#define MAX_CFG_SOE_LEN               1000UL            /* SOE配置文件缓冲区长度*/
#define MAX_CFG_P2P_LEN               1000UL            /* P2P配置文件缓冲区长度*/
#define MAX_CFG_VOTE_LEN              1000UL            /* 表决配置文件缓冲区长度*/
#define MAX_CFG_IO_MODULAR_LEN       10000UL            /* IO模块配置文件缓冲区长度*/
#define MAX_CFG_RETAIN_LEN            1000UL            /* 掉电保持配置文件缓冲区长度*/
#define MAX_CFG_PRG_LEN            1000000UL            /* PRG文件缓冲区长度*/


#define CFG_FILE_RD_ENABLE            true              /* 配置文件可读*/
#define CFG_FILE_RD_DISABLE           false             /* 配置文件不可读 */
#define CFG_FILE_WR_ENABLE            true              /* 配置文件可写*/
#define CFG_FILE_WR_DISABLE           false             /* 配置文件不可写 */
#define CFG_FILE_WR_FINISH            true              /* 配置文件写完成 */
#define CFG_FILE_WR_NOT_FINISH        false             /* 配置文件未写完*/

#define CFG_DB_FULL                   true              /* 数据库满*/
#define CFG_DB_EMPTY                  false             /* 数据库空*/
#define CFG_DB_RD_ENABLE              true              /* 数据库可读*/
#define CFG_DB_RD_DISABLE             false             /* 数据库不可读 */
#define CFG_DB_WR_ENABLE              true              /* 数据库可写*/
#define CFG_DB_WR_DISABLE             false             /* 数据库不可写 */


#define CFG_FILE_UPDATED              true              /* 配置文件更新 */
#define CFG_FILE_NOT_UPDATED          false             /* 配置文件未更新 */

#define CM_CFG_FILE_NUM               5                 /* CM最大配置文件个数 */
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/* 两个配置文件数据库交替使用 */
typedef enum CfgDBIndexTag
{
    CFG_DB_A = 0,                              /* 配置文件数据库A*/
    CFG_DB_B = 1,                              /* 配置文件数据库B */
    NUM_OF_CFG_DB,                             /* 配置文件数据库个数 */
    INVALID_CFG_DB_INDEX                       /* 配置文件数据库错误的索引号 */
} CfgDBIndex_t;

typedef struct SysCfgFileStateTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
} SysCfgFileState_t;

/* CM 配置文件*/
typedef struct SysCfgFileCMTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_CM_LEN];                  /* 配置文件内容 */
} SysCfgFileCM_t;

/* Modbus master 配置文件*/
typedef struct SysCfgFileModbusMasterTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_MODBUS_MASTER_LEN];       /* 配置文件内容 */
} SysCfgFileModbusMaster_t;

/* Modbus slave 配置文件*/
typedef struct SysCfgFileModbusSlaveTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_MODBUS_SLAVE_LEN];        /* 配置文件内容 */
} SysCfgFileModbusSlave_t;

/* OPC 配置文件*/
typedef struct SysCfgFileOPCTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_OPC_LEN];                 /* 配置文件内容 */
} SysCfgFileOPC_t;

/* 控制站配置文件*/
typedef struct SysCfgFileCtrlStationTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_CTRL_STAION_LEN];         /* 配置文件内容 */
} SysCfgFileCtrlStation_t;

/* SOE 配置文件*/
typedef struct SysCfgFileSOETag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_SOE_LEN];                 /* 配置文件内容 */
} SysCfgFileSOE_t;

/* P2P 配置文件*/
typedef struct SysCfgFileP2PTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_P2P_LEN];                 /* 配置文件内容 */
} SysCfgFileP2P_t;

/* VOTE 配置文件*/
typedef struct SysCfgFileVoteTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_VOTE_LEN];                /* 配置文件内容 */
} SysCfgFileVote_t;

/* IO模块 配置文件*/
typedef struct SysCfgFileIOModTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_IO_MODULAR_LEN];          /* 配置文件内容 */
} SysCfgFileIOMod_t;

/* 掉电保持配置文件*/
typedef struct SysCfgFileRetainTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_RETAIN_LEN];              /* 配置文件内容 */
} SysCfgFileRetain_t;

/* PRG文件*/
typedef struct SysCfgFilePRGTag
{
    bool_t bReadable;                                   /* 配置文件是否可读 */
    bool_t bWriteable;                                  /* 配置文件是否可写 */
    bool_t bFileStatus;                                 /* 配置文件的两种状态：写完或未写完*/
    uint32_t uiLen;                                     /* 配置文件长度 */
    uint32_t uiWrOffset;                                /* 写偏移地址 */
    uint8_t ucContent[MAX_CFG_PRG_LEN];                 /* 配置文件内容 */
} SysCfgFilePRG_t;

/* 配置文件数据库 */
typedef struct SysCfgDataBaseTag
{
    bool_t bReadable;                                   /* 数据库是否可读 */
    bool_t bWriteable;                                  /* 数据库是否可写 */
    bool_t bDBStatus;                                   /* 数据库状态： 空或满*/
    SysCfgFileCM_t stCfgCM;                             /* CM 配置文件 */
    SysCfgFileModbusMaster_t stCfgModbusMaster;         /* Modbus master 配置文件 */
    SysCfgFileModbusSlave_t stCfgModbusSlave;           /* Modbus slave 配置文件 */
    SysCfgFileOPC_t stCfgOPC;                           /* OPC 配置文件 */
    SysCfgFileCtrlStation_t stCfgCtrlStation;           /* 控制站配置文件 */
    SysCfgFileSOE_t stCfgSOE;                           /* SOE配置文件 */
    SysCfgFileP2P_t stCfgP2P;                           /* P2P配置文件 */
    SysCfgFileVote_t stCfgVote;                         /* Vote 配置文件 */
    SysCfgFileIOMod_t stCfgIOMod;                       /* IO模块配置文件 */
    SysCfgFileRetain_t stCfgRetain;                     /* Retain 配置文件 */
    SysCfgFilePRG_t stCfgPrg;                           /* PRG 配置文件 */
} SysCfgDB_t;



typedef struct SysCfgFileTableTag
{
    bool_t bFlag;                                       /* 整个配置文件列表是否可用  （true 可用   false 不可用）*/
    bool_t bReady[NUM_OF_CM];                           /* 配置文件是否就绪 */
    bool_t bCfgFileCM[NUM_OF_CM];                       /* CM配置文件是否可读*/
    bool_t bCfgFileModbusMaster[NUM_OF_CM];             /* Modbus master 配置文件是否可读*/
    bool_t bCfgFileModbusSlave[NUM_OF_CM];              /* Modbus slave 配置文件是否可读*/
    bool_t bCfgFileOPC[NUM_OF_CM];                      /* OPC 配置文件是否可读*/
    bool_t bCfgFileCtrlStation[NUM_OF_CM];              /* Control station 配置文件是否可读*/
} SysCfgFileTable_t;
#pragma pack()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysCfgFileInit
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
void SysCfgFileInit(void); 

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileInfo
*
* 功能描述: 从系统资源模块获取配置信息
*
* 输入参数: emCfgFileType: 各种配置文件类型
*        uiRdOffset: 读偏移地址
* *      sLen: 指向待读出的配置信息长度
*
* 输出参数: pucContent: 指向待读出的配置信息内容
*
* 返 回 值  : 实际读出的数据长度
*
* 注意事项: 读取后不清除数据库中相应配置文件中的内容
***********************************************************************************************************************
*/
uint16_t SysGetCfgFileInfo (SysCfgFileType_t emCfgFileType, uint32_t uiRdOffset, puint8_t pucContent, uint16_t usLen);

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileState
*
* 功能描述: 从系统资源模块获取配置文件的各种状态信息
*
* 输入参数: emCfgFileType: 各种配置文件类型
*
* 输出参数: pstSysCfgFileState: 指向相应的配置文件的各种状态信息
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetCfgFileState (SysCfgFileType_t emCfgFileType, SysCfgFileState_t *pstSysCfgFileState);

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileInfo
*
* 功能描述: 写入配置信息内容
*
* 输入参数:  emCfgFileType: 配置信息类型
*         pucContent: 指向待写入的配置信息内容
*         usLen: 指向待写入的配置信息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 实际写入的长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysSetCfgFileInfo(SysCfgFileType_t emCfgFileType, puint8_t pucContent, uint16_t usLen);

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileFinished
*
* 功能描述: 写入配置文件结束
*
* 输入参数:  emCfgFileType: 配置文件类型
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCfgFileFinished(SysCfgFileType_t emCfgFileType);

/*
***********************************************************************************************************************
* 函数名称: SysSetAllCfgFileFinished
*
* 功能描述: 写入所有配置文件结束
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
void SysSetAllCfgFileFinished(void);

/*
***********************************************************************************************************************
* 函数名称: SysDeleteCfgFile
*
* 功能描述: 删除配置文件
*
* 输入参数:  emCfgFileType: 配置文件类型
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysDeleteCfgFile(SysCfgFileType_t emCfgFileType);

/*
***********************************************************************************************************************
* 函数名称: SysCheckCfgFileTransFlag
*
* 功能描述: 是否需要传输相应的配置文件到相应的CM
*
* 输入参数: emCMID：CM的ID号
*        emFileIndex：配置文件索引号
*
* 输出参数: 无
*
* 返 回 值  : true-需要传输
*        false-不需要传输
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysCheckCfgFileTransFlag(CMController_t emCMID, SysCfgFileType_t emFileIndex);

/*
***********************************************************************************************************************
* 函数名称: SysSetCfgFileTable
*
* 功能描述: 将配置文件表存储到系统资源管理模块
*
* 输入参数: pstCfgFileTable: 指向配置文件表
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCfgFileTable(SysCfgFileTable_t *pstCfgFileTable);

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileTable
*
* 功能描述: 从系统资源管理模块读取配置文件表存
*
* 输入参数: 无
*
* 输出参数: pstCfgFileTable: 指向配置文件表
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCfgFileTable(SysCfgFileTable_t *pstCfgFileTable);

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileContentStartAddr
*
* 功能描述: 获取指定配置文件内容的起始地址
*
* 输入参数: emFileIndex：配置文件索引号
*
* 输出参数: 无
*
* 返 回 值  : 起始地址
*
* 注意事项: 文件无法访问，返回NULL
***********************************************************************************************************************
*/
puint8_t SysGetCfgFileContentStartAddr(SysCfgFileType_t emFileIndex);

#endif /* SYS_CFG_FILE_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

