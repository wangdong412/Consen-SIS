/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : Modbus模块
*
*  文件名称    : Modbus.h
*
*  功能简介    : 收发Modbus数据
*
*  作者        : 王东
*
*  创建日期    : 2016-02-25
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef MODBUS_H_
#define MODBUS_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "ExterComm/inc/ExterComm.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_MODBUS

#define DEBUG_OUT_MB

#ifdef DEBUG_OUT_MB
#define DEBUG_MB(format, args...)  printf ("DB_MB: ") ; printf(format, ##args)
#else
#define DEBUG_MB(format, args...)
#endif

#define MAX_MBMSG_LEN             (256)       /* Modbus 数据帧的最大长度: */
                                              /*     RTU / Modbus TCP: 最大256字节；*/
                                              /*     ASCII：                        最大513字节，但已被底层转换为RTU格式，最大255字节；*/
#define MAX_PM_MODBUS_DATA_LEN    (1*1000)    /* PM Modbus数据帧的最大长度 */
#define MBTCP_RESP_TO_CYCLE_CNT   (1*1000/8)  /* Modbus TCP从站应答超时拍数，时间1S */
#define MBSERL_RESP_TO_CYCLE_CNT  (1*1000/8)  /* Modbus Serial从站应答超时拍数，时间1S */
#define INVALID_HEX_VALUE         (0xFFU)     /* 无效16进制值 */
#define LOOPBLK_INVL_CYCLE_CNTS   (40)        /* Modbus 主站端口轮询块的间隔拍数(Interval)，时间40*32=1280ms */

/* PM Modbus Frame */
#define PMMB_HEADER_LEN           (28)        /* PM Modbus Frame Header Length */

/* Function Code: 1- 255 */
#define INVALID_MB_FUNCCODE       (0x00U)     /* Invalid Function Code     无效功能码 */
#define READ_COIL_STATUS          (0x01U)     /* Read Coil Status          读线圈状态 */
#define READ_INPUT_STATUS         (0x02U)     /* Read Input Status         读输入离散量状态 */
#define READ_HOLD_REGS            (0x03U)     /* Read Holding Registers    读保持寄存器 */
#define READ_INPUT_REGS           (0x04U)     /* Read Input Registers      读输入寄存器 */
#define FORCE_SINGLE_COIL         (0x05U)     /* Force Single Coil         强制单线圈 */
#define PRESET_SINGLE_REG         (0x06U)     /* Preset Single Register    预置单寄存器 */
#define MODBUS_DIAG               (0x08U)     /* Diagnostics               诊断校验 */
#define FORCE_MULT_COILS          (0x0FU)     /* Force Multiple Coils      强制多线圈 */
#define PRESET_MULT_REGS          (0x10U)     /* Preset Multiple Registers 预置多寄存器 */

/*************************** Value ***************************/
/* Coil */
#define COIL_ON                   (0xFF00U)   /* ON  开 */
#define COIL_OFF                  (0x0000U)   /* OFF 关 */

/* ASCII */
#define ASCII_START_VALUE         (':')       /* ASCII Start Char Value       起始符 */
#define ASCII_END_VALUE_H         ('\r')      /* ASCII End Char Value - High  结束符高字节 */
#define ASCII_END_VALUE_L         ('\n')      /* ASCII End Char Value - Low   结束符低字节 */

/* MBAP Content */
#define MBAP_TRANS_ID_VALUE       (0x0000U)   /* Transaction Identifier */
#define MBAP_PROT_ID_VALUE        (0x0000U)   /* Protocol Identifier: 0 = MODBUS Protocol */
#define MBAP_UNIT_ID_VALUE        (0x01U)     /* Unit Identifier */

/*********************** Message Length ***********************/
/* Maximum */
#define MAX_MBRGN_INDEX           (9999)      /* 控制器作为Modbus从站时，每个区内的最大索引值 */

/* RTU: Slave ID(1) + Function Code(1) + Data + CRC(2) */
#define MAX_MBRTU_MSG_LEN         (256)       /* Modbus RTU消息的最大长度 */

/* ASCII: Start(1) + Slave ID(2) + Function Code(2) + Data + LRC(2) + End(2) */
#define MAX_MBASCII_MSG_LEN       (513)       /* Modbus ASCII消息的最大长度 */

/* Non-Data Length：Zero Data Bytes */
#define MBTCP_MSG_NONDATA_LEN     (8)         /* Modbus TCP消息的非数据域长度 */
#define MBRTU_MSG_NONDATA_LEN     (4)         /* Modbus RTU消息的非数据域长度 */
#define MBASCII_MSG_NONDATA_LEN   (9)         /* Modbus ASCII消息的非数据域长度 */

/* Data Length */
#define MAX_MBTCP_DATA_LEN   ((MAX_MBTCP_MSG_LEN) - (MBTCP_MSG_NONDATA_LEN))      /* Maximum Modbus TCP Data Length */
#define MAX_MBRTU_DATA_LEN   ((MAX_MBRTU_MSG_LEN) - (MBRTU_MSG_NONDATA_LEN))      /* Maximum Modbus RTU Data Length */
#define MAX_MBASCII_DATA_LEN ((MAX_MBASCII_MSG_LEN) - (MBASCII_MSG_NONDATA_LEN))  /* Maximum Modbus ASCII Data Length */

/* Exception Message */
#define MBTCP_EXMSG_LEN           (9)    /* Modbus TCP异常消息的长度 ：MBAP Header(7) + Function Code(1) + Data(1) */
#define MBRTU_EXMSG_LEN           (5)    /* Modbus RTU异常消息的长度 ：Slave ID(1) + Function Code(1) + Data(1) + CRC(2)*/
#define MBASCII_EXMSG_LEN         (11)   /* Modbus ASCII异常消息的长度 */

/* Modbus Serial: RTU/ASCII */
#define MBRTU_HEADER_LEN          (2)    /* Modbus RTU Header Length: Slave ID(1) + Function Code(1) */

/***************************** Index *****************************/
/******** Modbus TCP ********/
/* Function Code */
#define MBTCP_FUNCCODE_INDEX      (7)    /* Modbus TCP Function Code Index */
#define MBTCP_EXCCODE_INDEX       (8)    /* Modbus TCP Exception Code Index */

/******** Modbus RTU ********/
#define MBRTU_SLAVEID_INDEX       (0)    /* Modbus RTU Slave ID Index */
#define MBRTU_FUNCCODE_INDEX      (1)    /* Modbus RTU Function Code Index */
#define MBRTU_BYCNTS_INDEX        (2)    /* Modbus RTU Byte Counts Index */

/******* Modbus ASCII ******/
#define MBASCII_START_INDEX       (0)    /* Start 起始符索引值 */

/* Port Modbus Type  端口Modbus类型定义 */
#define PORT_MBT_MASTER           (0)    /* Port Modbus Type Master  端口为Modbus 主站 */
#define PORT_MBT_SLAVE            (1)    /* Port Modbus Type Slave   端口为Modbus 从站 */
#define PORT_MBT_NONE             (2)    /* Port Modbus Type None    端口与Modbus 无关 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

typedef enum ModbusExceptionTag
{
    MBEX_NONE = 0,
    MBEX_ILLEGAL_FUNC,       /* Server does not support this function code */
    MBEX_ILLEGAL_DATA_ADDR,  /* Server does not have the requested address space */
    MBEX_ILLEGAL_DATA_VALUE, /* The value is not in the range allowed for the address */
    MBEX_DEVICE_FAILURE,     /* Device failure */
    MBEX_ACK,                /* Acknowledgment */
    MBEX_BUSY_REJECTED,      /* Device Busy */
}ModbusException_t;

/* Buffer Type  缓冲区类型枚举定义 */
typedef enum BuffTypeTag
{
    BUFF_RECV = 0,  /* Receive Buffer         接收缓冲区 */
    BUFF_SEND,      /* Send Buffer            发送缓冲区 */
    BUFF_ALL        /* Receive & Send Buffer  接收和接收缓冲区 */
}BuffType_t;

/* Modbus Message  Modbus 消息结构体定义 */
typedef struct ModbusMsgTag
{
    uint16_t usLen;                 /* Length 消息长度 */
    uint8_t ucBuff[MAX_MBMSG_LEN];  /* Buffer 缓冲区，用于存放消息内容 */
}ModbusMsg_t;

/* Port Master Info. 端口主站信息结构体定义 */
typedef struct PortMstrInfoTag
{
    ModuleState_t emState;        /* Port State                       端口状态 */
    bool_t bHandleSwitch;         /* Handle Switch Flag               处理切换标志：端口Busy且端口主站信息变化时，置位 */
    uint8_t ucReserve;            /* Reserve                          预留 */
    uint16_t usEstimateRespLen;   /* Estimate Response Length         预估应答帧的长度 */
    uint32_t uiBlockIndex;        /* Modbus Master Block Index        端口目前轮询块的索引值 */
    uint32_t uiWaitRespCycleCnt;  /* Wait Slave Response Cycle Count  端口目前已等待应答帧的拍数 */
    uint32_t uiWaitLoopCycleCnt;  /* Wait Loop Next Block Cycle Count 轮询下一个块还需等待的拍数 */
    ModbusMsg_t stMBReqMsg;       /* Modbus Request Message           Modbus请求消息 */
}PortMstrInfo_t;

/* Port Modbus Master Header  端口Modbus主站头信息结构体定义 */
typedef struct PortMBMstrHeadTag
{
    uint8_t ucPortType;           /* Port Type              端口类型 */
    uint8_t ucReserve1;           /* Reserve 1              预留 1 */
    uint16_t usDataLen;           /* Data Length            Modbus 数据的长度（字节） */
    uint8_t ucCMSlotID;           /* CM Slot ID             CM模块槽位号 */
    uint8_t ucPortID;             /* Port ID                端口号 */
    uint8_t ucReserve2[2];        /* Reserve 2              预留 2 */
    uint32_t uiBlockID;           /* Block ID               Modbus Master块号 */
    RTDataVerInfo_t stRTDataVer;  /* Real-time Data Version 实时数据的版本 */
}PortMBMstrHead_t;

/* Port Modbus Slave Header   端口Modbus从站头信息结构体定义 */
typedef struct PortMBSlaveHeadTag
{
    uint8_t ucPortType;           /* Port Type       端口类型 */
    uint8_t ucReserve1;           /* Reserve 1       预留 1 */
    uint16_t usDataLen;           /* Data Length     Modbus 数据的长度（字节） */
    uint8_t ucReserve2[4];        /* Reserve 2       预留 2 */
    uint32_t uiBlockID;           /* Block ID        Modbus Slave块号 */
    uint16_t usBeginAddr;         /* Begin Address   本数据帧中数据的起始地址 */
    uint16_t usAliasNum;          /* Coil/Reg Number 线圈或寄存器的数目 */
    uint8_t ucReserve3[12];       /* Reserve 3       预留 3 */
}PortMBSlaveHead_t;

/* Serial Port Settings 串口设置参数结构体定义 */
typedef struct SerlPortSetsTag
{
    uint8_t ucTransProto;  /* 0-1: RTU / ASCII */
    uint8_t ucBaudRate;    /* 0-7: 1200 - 115200 */
    uint8_t ucTransMode;   /* 0-1: RS485 / RS422 */
    uint8_t ucParity;      /* 0-2: None, Odd, Even */
    bool_t  bEnable;       /* Port Enable Flag */
    uint8_t ucReserve[3];
}SerlPortSets_t;

/***************************** Configuration *****************************/

/* Serial Configure Info. 串口配置信息结构体定义 */
typedef struct SerlCfgInfoTag
{
    uint8_t ucIndex;              /* Index                     当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];         /* Reserve                   预留 */
    uint32_t uiCMCfgCRC;          /* CM Configuration CRC      CM配置文件CRC */
    uint32_t uiCMCfgVer;          /* CM Configuration Version  CM配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;  /* Real-time Data Version    实时数据的版本 */
    COMConfigInfo_t stCOMConfig[LOCAL_CFG_VER_CNT];  /* COM Configure 串口配置参数 */
}SerlCfgInfo_t;

/* Modbus TCP Configure Info.  Modbus TCP配置信息结构体定义 */
typedef struct MBTCPCfgInfoTag
{
    uint8_t ucIndex;              /* Index                     当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];         /* Reserve                   预留 */
    uint32_t uiCMCfgCRC;          /* CM Configuration CRC      CM配置文件CRC */
    uint32_t uiCMCfgVer;          /* CM Configuration Version  CM配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;  /* Real-time Data Version    实时数据的版本 */
    ModbusTCPConfigInfo_t stMBTCPConfig[LOCAL_CFG_VER_CNT];  /* Modbus TCP Configure Info.  Modbus TCP配置参数信息 */
}MBTCPCfgInfo_t;

/* Modbus Slave Configuration Info.  Modbus从站配置信息结构体定义 */
typedef struct MBSlaveCfgInfoTag
{
    uint8_t ucIndex;              /* Index                  当前使用的配置参数的索引值 */
    bool_t bCopying;              /* Copying                是否正在拷贝 */
    bool_t bValid;                /* Valid Flag             当前配置是否有效 */
    uint8_t ucReserve;            /* Reserve                预留 */
    uint32_t uiCfgCRC;            /* Configuration CRC      配置文件CRC */
    uint32_t uiCfgVer;            /* Configuration Version  配置文件的版本 */
    RTDataVerInfo_t stRTDataVer;  /* Real-time Data Version 实时数据的版本 */
    ModbusSlaveInfo_t stSlaveCfg[LOCAL_CFG_VER_CNT];  /* Modbus Slave Configuration Modbus从站配置 */
}MBSlaveCfgInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ModbusInit
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

void ModbusInit( void );

/*
***********************************************************************************************************************
* 函数名称: ModbusCycle
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

void ModbusCycle( void );

#endif /*MODBUS_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


