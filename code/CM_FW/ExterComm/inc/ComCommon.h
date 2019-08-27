/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : CM_FW通信公共部分
*
*  文件名称    : ComCommon.h
*
*  功能简介    : 通信公共部分
*
*  作者        : 王东
*
*  创建日期    : 2016-02-22
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef COMCOMMON_H_
#define COMCOMMON_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_COMCOMMON

#define DEBUG_OUT_RTS

#ifdef DEBUG_OUT_RTS
#define DEBUG_RTS(format, args...)  printf ("DB_RTS: ") ; printf(format, ##args)
#else
#define DEBUG_RTS(format, args...)
#endif

#define MAX_RETRY_CNT        (3)        /* 最大可重试次数 */
#define LOCAL_CFG_VER_CNT    (2)        /* 本地配置参数的数目 */
#define MAX_CYCLE_COPY_LEN   (10*1024)  /* 每周期最大允许拷贝字节数 */

/* Variable */
#define MAX_VAR_SIZE         (8)        /* 数据最大长度值 */

/* Port Number */
#define PORTNUM_SOE          (1500)     /* SOE服务器端套接字端口号 */
#define PORTNUM_OPC          (1501)     /* OPC服务器端套接字端口号 */
#define PORTNUM_CS1131       (1502)     /* CS1131监听套接字端口号 */
#define PORTNUM_P2P          (1503)     /* 站间通讯套接字端口号 */
#define PORTNUM_CLIENT       (1504)     /* Client服务器端套接字端口号 */
#define PORTNUM_AMS          (1505)     /* AMS服务器端套接字端口号 */
#define PORTNUM_NTP          (123)      /* 校时套接字端口号 */
#define PORTNUM_MODBUS       (502)      /* Modbus监听套接字端口号 */

/* Bit */
#define BIT_1  (0x01U)  /* 第1位 */
#define BIT_2  (0x02U)  /* 第2位 */
#define BIT_3  (0x04U)  /* 第3位 */
#define BIT_4  (0x08U)  /* 第4位 */
#define BIT_5  (0x10U)  /* 第5位 */
#define BIT_6  (0x20U)  /* 第6位 */
#define BIT_7  (0x40U)  /* 第7位 */
#define BIT_8  (0x80U)  /* 第8位 */


/****************************** Modbus ******************************/
#define MAX_MBTCP_MSTR_CNT          (16)     /* Modbus TCP主站的最大个数 */
#define MIN_MODBUS_SLAVE_ID         (1)      /* Modbus 从站ID的最小有效值 */
#define MAX_MODBUS_SLAVE_ID         (247)    /* Modbus 从站ID的最大有效值 */
#define MAX_MBSLAVE_ALIAS           (65535)  /* Modbus 从站变量地址的最大有效值 */
#define MAX_MBMSTRBLK_DATA_OFFSET   (99)     /* Modbus 主站块中变量的最大偏移：BOOL-位偏移；INT-2字节的整数倍 */


/* Alias */
/* Protocol： Start Address = Alias-1/10001/30001/40001 */
#define MIN_MODBUS_ALIAS_COIL       (1)      /* Coil 线圈 */
#define MAX_MODBUS_ALIAS_COIL       (10000)

#define MIN_MODBUS_ALIAS_DIS_IN     (10001)  /* Discrete Inputs  离散量输入 */
#define MAX_MODBUS_ALIAS_DIS_IN     (20000)

#define MIN_MODBUS_ALIAS_IN_REGS    (30001)  /* Input Registers  输入寄存器 */
#define MAX_MODBUS_ALIAS_IN_REGS    (40000)

#define MIN_MODBUS_ALIAS_HOLD_REGS  (40001)  /* Holding Registers  保持寄存器 */
#define MAX_MODBUS_ALIAS_HOLD_REGS  (50000)

/* Modbus TCP */
/* MBTCP: MBAP Header(7) + Function Code(1) + Data */
#define MAX_MBTCP_MSG_LEN           (256)    /* Modbus TCP消息的最大长度 */
#define MBTCP_HEADER_LEN            (7)      /* Modbus TCP Header Length: MBAP */
#define MAX_MBTCP_LEN_VALUE         ((MAX_MBTCP_MSG_LEN) - (MBTCP_HEADER_LEN) + 1U)
                                             /* Modbus TCP消息中长度域的最大值：250 */

/* MBAP-7 Bytes */
#define MBAP_TRANS_ID_H_INDEX       (0)  /* Transaction Identifier - High Index */
#define MBAP_TRANS_ID_L_INDEX       (1)  /* Transaction Identifier - Low Index */
#define MBAP_PROT_ID_H_INDEX        (2)  /* Protocol Identifier - High Index */
#define MBAP_PROT_ID_L_INDEX        (3)  /* Protocol Identifier - Low Index */
#define MBAP_LEN_H_INDEX            (4)  /* Length: Length of following bytes - High Index */
#define MBAP_LEN_L_INDEX            (5)  /* Length - Low Index */
#define MBAP_UNIT_ID_INDEX          (6)  /* Unit Identifier Index */

/**************************** Peer-to-Peer ****************************/
/* Length */
#define P2P_MSG_ADDL_LEN       (8)  /* P2P Additional Info. Length */
#define P2P_MSG_FCS_LEN        (4)  /* P2P FCS Length */

/* Header Item Index */
#define P2P_SRC_STAID_INDEX    (0)  /* Source Station ID Index */
#define P2P_DST_STAID_INDEX    (1)  /* Destination Station Index */
#define P2P_TYPE_INDEX         (2)  /* Type Index */
#define P2P_LEN_HIINDEX        (4)  /* Length-High Byte Index */
#define P2P_LEN_LOWINDEX       (5)  /* Length-Low Byte Index */

/* Station Request */
#define P2P_REQ_PMINFO_INDEX   (6)  /* PM Info. Index */

/********************    Timing    ********************/
/* Length */
#define SNTP_MSG_HEADER_LEN    (8)  /* SNTP/NTP Message Header Length */
#define SNTP_MSG_CRC_LEN       (4)  /* SNTP/NTP Message CRC Length */

/********** System Command **********/
#define MAX_PATH_LEN          (256)  /* 路径名的最大长度-字节数，包含NULL字符 */
#define MAX_CMD_CNT           (50)   /* 命令的最大个数 */
#define MAX_ARGV_ITEM_CNT     (20)   /* 单条命令包含单词的最大个数 */
#define MAX_WORD_LEN          (20)   /* 单个单词的最大长度-字节数，包含NULL字符 */
#define MAX_WAIT_CHD_CYC_CNT  (100)  /* 等待子进程完成的最大拍数 */

/* WaitChildFinish Error Code */
#define EWCF_NOTFINISH  (0)
#define EWCF_FINISH     (1)
#define EWCF_PARA       (-1)
#define EWCF_WAIT_FUN   (-2)
#define EWCF_EXIT       (-3)

/* ExecSysCmd Finish Code */
#define ESC_FINISH_NOT  (0)
#define ESC_FINISH_OK   (1)
#define ESC_FINISH_FAIL (-1)
#define SHM_CMD_SIZE    (sizeof(SysCmd_t))

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Module State  模块状态枚举定义 */
typedef enum ModuleStateTag
{
    IDLE_STATE = 0,  /* Idle State 空闲状态 */
    BUSY_STATE       /* Busy State 忙状态 */
}ModuleState_t;

/* Selected PM ID  已选取的PM模块的编号 */
typedef enum SelPMIDTag
{
    SELPM_A = 0,     /* PMA  选取PMA模块 */
    SELPM_B,         /* PMB  选取PMB模块 */
    SELPM_C,         /* PMC  选取PMC模块 */
    SELPM_NONE       /* None 无有效PM模块 */
}SelPMID_t;

/* Comm Type 通讯类型枚举定义 */
typedef enum CommTypeTag
{
    TYPE_SOE = 0,      /* SOE Software           SOE软件 */
    TYPE_AMS,          /* AMS Software           AMS软件 */
    TYPE_CLIENT,       /* Client Software        客户端软件 */
    TYPE_OPC,          /* OPC/HMI Software       OPC/HMI软件 */
    TYPE_CS1131,       /* CS1131 Software        CS1131软件 */
    TYPE_MBTCP_MSTR,   /* Modbus TCP Master      发往Modbus TCP主站 */
    TYPE_MBTCP_SLAVE,  /* Modbus TCP Slave       发往Modbus TCP从站 */
    TYPE_P2P           /* Peer-to-Peer           站间通讯 */
}CommType_t;

/* PM P2P Frame Type  PM P2P帧类型 */
typedef enum PMP2PFraTypeTag
{
    PMP2P_REQ = 0,   /* Request  请求 */
    PMP2P_RESP,      /* Response 应答 */
    PMP2P_INVALID    /* Invalid  无效 */
}PMP2PFraType_t;

/* Ref. ID  数据区的编号 */
typedef enum RefIDTag
{
    REF_G = 0,       /* G      G区 */
    REF_I,           /* I      I区 */
    REF_O,           /* O      O区 */
    REF_NUM          /* Number 数据区的个数 */
}RefID_t;

/* Timing Source 时钟更新源枚举定义 */
typedef enum TimingSrcTag
{
    TSRC_NTP = 0,  /* SNTP/NTP Timing  SNTP/NTP校时 */
    TSRC_GPS,      /* GPS Timing       GPS校时 */
    TSRC_TBLK,     /* Timing Block     校时功能块 */
    TSRC_NONE      /* None             无 */
}TimingSrc_t;

/* Pulse Source 脉冲源枚举定义 */
typedef enum PulseSrcTag
{
    PSRC_NONE = 0,  /* None  无校时源 */
    PSRC_GPS,       /* GPS   GPS校时源 */
    PSRC_P2P,       /* P2P   站间校时源 */
    PSRC_LOCAL      /* Local 本地 */
}PulseSrc_t;

/* Pulse Repeat 脉冲重复支持枚举定义 */
typedef enum PulseRptTag
{
    PRPT_NSPT = 0,  /* Not Support  不支持 */
    PRPT_SPT = 1    /* Support      支持 */
}PulseRpt_t;

/* Pulse Polarity 脉冲极性枚举定义 */
typedef enum PulsePolyTag
{
    POLY_POS = 0,  /* Positive  正 */
    POLY_NEG = 1   /* Negative  负 */
}PulsePoly_t;

/* Modbus Master Configuration Info.  Modbus主站配置信息结构体定义 */
typedef struct ModbusMasterCfgInfoTag
{
    uint8_t ucIndex;                                    /* Index                        当前使用的配置参数的索引值 */
    bool_t bCopying;                                    /* Copying                      是否正在拷贝 */
    bool_t bValid;                                      /* Valid Flag                   当前配置是否有效 */
    uint8_t ucReserve;                                  /* Reserve                      预留 */
    uint32_t uiCfgCRC;                                  /* Configuration CRC            配置文件CRC */
    uint32_t uiCfgVer;                                  /* Configuration Version        配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;                        /* Real-time Data Version       实时数据的版本 */
    ModbusMasterInfo_t stMasterCfg[LOCAL_CFG_VER_CNT];  /* Modbus Master Configuration  Modbus主站配置 */
}ModbusMstrCfgInfo_t;

/* Task Section Info. 任务段信息结构体定义 */
typedef struct TaskInfoTag
{
    uint8_t ucIndex;                                    /* Index                  当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];                               /* Reserve                预留 */
    uint32_t uiCfgCRC;                                  /* Configuration CRC      配置文件CRC */
    uint32_t uiCfgVer;                                  /* Configuration Version  配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;                        /* Real-time Data Version 实时数据的版本 */
    TaskConfigInfo_t stTaskCfgInfo[LOCAL_CFG_VER_CNT];  /* Task Info.             任务信息 */
}TaskInfo_t;

/* Time Sync Configure Info.  校时配置信息结构体定义 */
typedef struct TimeSyncCfgInfoTag
{
    uint8_t ucIndex;                                         /* Index                     当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];                                    /* Reserve                   预留 */
    uint32_t uiCMCfgCRC;                                     /* CM Configuration CRC      CM配置文件CRC */
    uint32_t uiCMCfgVer;                                     /* CM Configuration Version  CM配置文件的版本 */
    NTPTCPConfigInfo_t stTimeSyncConfig[LOCAL_CFG_VER_CNT];  /* Timing Configure Info.    校时配置参数信息 */
}TimeSyncCfgInfo_t;

/***************************** system command *****************************/
/* System command  系统命令结构体定义 */
typedef struct SysCmdTag
{
    char_t cPrgPath[MAX_PATH_LEN];                  /* Program Pathname */
    char_t cArgv[MAX_ARGV_ITEM_CNT][MAX_WORD_LEN];  /* System command array */
}SysCmd_t;

/* System command Info.  系统命令信息结构体定义 */
typedef struct SysCmdInfoTag
{
    uint8_t ucCmdCnt;          /* Command count */
    uint8_t ucCurIndex;        /* Current command index */
    bool_t bWaitChild;         /* Wait child terminate */
    bool_t bIPChange;          /* IP change flag */
    uint32_t uiWaitNetCycCnt;  /* Wait network cycle count */
    int32_t iChildPID;         /* PID of child process */
    uint32_t uiWaitChdCycCnt;  /* Wait child process finish cycle count */
    SysCmd_t stCmd[MAX_CMD_CNT];
}SysCmdInfo_t;

/* Download Status 下装状态枚举定义 */
typedef enum DWStatusTag
{
    DWS_NON = 0,  /* 未下装 */
    DWS_SUCC,     /* 下装成功 */
    DWS_FAIL,     /* 下装失败 */
    DWS_DWING     /* 正在下装 */
}DWStatus_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SwitchToSWType
*
* 功能描述: 将通讯类型 转换为 软件类型
*
* 输入参数: emCommType：通讯类型
*
* 输出参数: 无
*
* 返 回 值: 转换后的软件类型
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysSoftwareType_t SwitchToSWType( CommType_t emCommType );

/*
***********************************************************************************************************************
* 函数名称: SwitchPMID
*
* 功能描述: 将选取结果转换为PM模块ID
*
* 输入参数: emSelPMID：选取的PMID
*
* 输出参数: 无
*
* 返 回 值: 转换后的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

PMController_t SwitchPMID( SelPMID_t emSelPMID );

/*
***********************************************************************************************************************
* 函数名称: Send
*
* 功能描述: 发送数据
*
* 输入参数: pucBuffer：待发送数据缓冲区；
*          usLen：待发送数据的字节数；
*          uiDstIPAddr：目的IP地址；
*          emNetwork：本地网口号。
*
* 输出参数: 无
*
* 返 回 值: 错误码，0-发送成功；非0值-发送失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t Send( uint8_t const pucData[], uint16_t usDataLen,
              uint32_t uiIPAddr, uint16_t usPort, NetworkPortType_t emNetwork,
              CommType_t emSendType, uint8_t ucIndex );

/*
***********************************************************************************************************************
* 函数名称: HandleRTDataVer
*
* 功能描述: 检查实时数据的版本是否变化，若变化，则更新本地保存的实时数据版本
*
* 输入参数: pstLocalRTDataVer：指向本地保存的实时数据版本信息
*
* 输出参数: 无
*
* 返 回 值: 实时数据版本是否变化，true-变化；false-未变化
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleRTDataVer( RTDataVerInfo_t *pstLocalRTDataVer );

/*
***********************************************************************************************************************
* 函数名称: InitModbusMstrInfo
*
* 功能描述: 初始化本地保存的 Modbus 主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向本地保存的Modbus 主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo );

/*
***********************************************************************************************************************
* 函数名称: ClearModbusMstrInfo
*
* 功能描述: 清除本地保存的 Modbus 主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向本地保存的Modbus 主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void ClearModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo );

/*
***********************************************************************************************************************
* 函数名称: GetCurModbusMstrInfo
*
* 功能描述: 获取当前使用的Modbus主站信息
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置信息
*
* 输出参数: 无
*
* 返 回 值: 当前使用的配置参数
*
* 注意事项: 无
***********************************************************************************************************************
*/

ModbusMasterInfo_t const* GetCurModbusMstrInfo( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo );

/*
***********************************************************************************************************************
* 函数名称: GetLocalModbusMstrCfg
*
* 功能描述: 获取本CM使用的Modbus主站配置
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置信息；
*          ucCMSlotID：本CM模块的槽号。
*
* 输出参数: 无
*
* 返 回 值: 本CM使用的Modbus主站配置
*
* 注意事项: 无
***********************************************************************************************************************
*/

ModbusMasterConfig_t const* GetLocalModbusMstrCfg( ModbusMstrCfgInfo_t const *pstMBMstrCfgInfo, uint8_t ucCMSlotID );

/*
***********************************************************************************************************************
* 函数名称: HandleModbusMstrInfo
*
* 功能描述: 实时数据版本变化，且Modbus主站配置参数版本变化时，拷贝到本地并切换
*
* 输入参数: pstMBMstrCfgInfo：指向Modbus主站配置参数；
*          puiOffset：指向拷贝偏移值；
*          ucCMSlotID：本CM模块的槽位号；
*          pstTaskInfo：指向任务配置信息数组。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换 - 配置参数拷贝完成且有效时，返回true
*
* 注意事项:
* 1. 实时数据版本变化时，更新本地保存的实时数据版本；
* 2. 为了保证Modbus主站配置与实时数据的一致性：拷贝完成后，无论配置参数是否有效，均需进行切换操作。
***********************************************************************************************************************
*/

bool_t HandleModbusMstrInfo( ModbusMstrCfgInfo_t *pstMBMstrCfgInfo, puint32_t puiOffset, uint8_t ucCMSlotID );

/*
***********************************************************************************************************************
* 函数名称: GetDataAreaLen
*
* 功能描述: 获取某数据区的长度
*
* 输入参数: emDataArea：数据区ID
*
* 输出参数: 无
*
* 返 回 值: 数据区的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint32_t GetDataAreaLen( SysRtDataAreaType_t emDataArea );

/*
***********************************************************************************************************************
* 函数名称: CheckMBSlaveVar
*
* 功能描述: 检查某一Modbus Slave块内的变量是否有效
*
* 输入参数: pstModbusVar：待检查的Modbus 变量；
*          ucModbusRgn：Modbus 变量所在的区。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t CheckMBSlaveVar( ModbusVar_t const *pstModbusVar, uint8_t ucModbusRgn );

/*
***********************************************************************************************************************
* 函数名称: CheckMBMstrVar
*
* 功能描述: 检查某一Modbus变量是否有效
*
* 输入参数: bMstrBlk：Master Block 标志；
*          pstModbusVar：待检查的Modbus 变量；
*          ucModbusRgn：Modbus 变量所在的区。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t CheckMBMstrVar( ModbusVar_t const *pstModbusVar, uint8_t ucModbusRgn, uint32_t uiStartAddr );

/*
***********************************************************************************************************************
* 函数名称: SelectRTData
*
* 功能描述: 获取实时数据的地址
*
* 输入参数: pemLastPMID：输入上一次选取结果；
*
* 输出参数: pemLastPMID：输出更新后的结果；
*          pbCMBus：输出CM Bus是否正常。
*
* 返 回 值: 实时数据的地址，NULL代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SelectRTData( PMController_t *pemLastPMID, bool_t *pbCMBus );

/*
***********************************************************************************************************************
* 函数名称: IsCMHasCfg
*
* 功能描述: 检查CM是否有工程
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 是否有工程标志位
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsCMHasCfg( void );

/*
***********************************************************************************************************************
* 函数名称: InitTimeSyncCfgInfo
*
* 功能描述: 初始化本地保存的时间同步配置信息
*
* 输入参数: pstTimeSyncInfo：指向本地保存的时间同步配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitTimeSyncCfgInfo( TimeSyncCfgInfo_t *pstTimeSyncInfo );

/*
***********************************************************************************************************************
* 函数名称: HandleTimeSyncConfig
*
* 功能描述: 检查时钟同步配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: pstTimeSyncInfo：指向时钟同步配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleTimeSyncConfig( TimeSyncCfgInfo_t *pstTimeSyncInfo );

/*
***********************************************************************************************************************
* 函数名称: GetCurTSyncCfg
*
* 功能描述: 获取当前时钟同步信息
*
* 输入参数: pstTimeSyncInfo：本地保存的时钟同步信息
*
* 输出参数: 无
*
* 返 回 值: 当前配置信息，NULL代表无有效配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

NTPTCPConfigInfo_t const* GetCurTSyncCfg( TimeSyncCfgInfo_t const *pstTimeSyncInfo );

/*
***********************************************************************************************************************
* 函数名称: AddSysCmd
*
* 功能描述: 添加命令
*
* 输入参数: pstCmd：命令 ；
*          pstCmdInfo：命令信息结构体。
*
* 输出参数: 无
*
* 返 回 值: 是否添加成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t AddSysCmd( SysCmd_t *pstCmd, SysCmdInfo_t *pstCmdInfo );

/*
***********************************************************************************************************************
* 函数名称: ExecSysCmd
*
* 功能描述: 执行系统命令
*
* 输入参数: pstCmdInfo：命令信息；
*          pucShmCmd：父进程共享内存首地址；
*          iShmID：共享内存ID。
*
* 输出参数: 无
*
* 返 回 值: 结束码
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t ExecSysCmd( SysCmdInfo_t *pstCmdInfo, uint8_t *pucShmCmd, int32_t iShmID );

/*
***********************************************************************************************************************
* 函数名称: SetNonBlockMode
*
* 功能描述: 设置非阻塞模式
*
* 输入参数: iFD：文件描述符
*
* 输出参数: 无
*
* 返 回 值: true：设置成功；false：设置失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SetNonBlockMode( int32_t iFD );

/*
***********************************************************************************************************************
* 函数名称: SetCloExec
*
* 功能描述: 设置close-on-exec：execve时，自动关闭从父进程继承过来的FD
*
* 输入参数: iFD：文件描述符
*
* 输出参数: 无
*
* 返 回 值: true：设置成功；false：设置失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SetCloExec( int32_t iFD );

#endif /*COMCOMMON_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


