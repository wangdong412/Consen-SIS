﻿/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 共享内存
*
*  文件名称    : SharedRtData.h
*
*  功能简介    : core0与core1间的共享实时数据
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-15
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SHARED_RTDATA_H_
#define SHARED_RTDATA_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

/* 数据区长度 */
#define MAX_RTDATA_G_AREA_LEN           (300*1024u)   /* G区大小 */
#define MAX_RTDATA_I_AREA_LEN           (8*1024u)     /* I区大小 */
#define MAX_RTDATA_Q_AREA_LEN           (8*1024u)     /* Q区大小 */

#define RTDATA_AREA_LOCKED              (true)        /* 数据区被锁定，不允许访问*/
#define RTDATA_AREA_UNLOCKED            (false)       /* 数据区未锁定，允许访问 */

#define RTDATA_AREA_READ_ONLY           (true)        /* 实时数据区只读标识 */
#define RTDATA_AREA_WRITE_ONLY          (false)       /* 实时数据区只写标识 */

#define MAX_MODBUS_CTRL_INFO            (8U)
#define MAX_P2P_CTRL_INFO               (16U)
#define MAX_SOE_CTRL_INFO               (16U)
#define MAX_SLAVE_NUM                   (240)     /* 模块的最大个数 */
#define MAX_PM_NUM                      (3U)      /* PM模块的最大个数 */
#define MAX_CM_NUM                      (4U)      /* CM模块的最大个数 */
#define MAX_BI_NUM                      (15U)     /* CM模块的最大个数 */
#define MAX_UP_NUM                      (2U)      /* UP的最大个数 */
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* -------------------实时数据数据结构------------------------------------------------------------------------------------ */
/* 实时数据区 */
#pragma pack(1)

/* PM版本结构体 */
typedef struct SharedPMFirmwareVernTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucRTSVern[4]; /* RTS版本信息 */
    uint8_t ucOSVern[4]; /* OS版本信息 */
    uint16_t usFPGAModID; /* FPGA模块ID */
    uint16_t usFPGAHardwareVern; /* FPGA硬件版本 */
    uint16_t usFPGAChipID; /* FPGA芯片位号 */
    uint16_t usFPGAFWVern; /* FPGA固件版本 */
    uint16_t usMCUModID; /* MCU模块ID */
    uint16_t usMCUHardwareVern; /* MCU硬件版本 */
    uint16_t usMCUChipID; /* MCU芯片位号 */
    uint16_t usMCUFWVern; /* MCU固件版本 */
}SharedPMFirmwareVern_t;

typedef struct SharedPMVernTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedPMFirmwareVern_t stPMFirmwareVern;
}SharedPMVern_t;


/* CM版本结构体 */
typedef struct SharedCMFirmwareVernTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucRTSVern[4]; /* RTS版本信息 */
    uint8_t ucOSVern[4]; /* OS版本信息 */
    uint16_t usFPGAModID; /* FPGA模块ID */
    uint16_t usFPGAHardwareVern; /* FPGA硬件版本 */
    uint16_t usFPGAChipID; /* FPGA芯片位号 */
    uint16_t usFPGAFWVern; /* FPGA固件版本 */
    uint16_t usMCUModID; /* MCU模块ID */
    uint16_t usMCUHardwareVern; /* MCU硬件版本 */
    uint16_t usMCUChipID; /* MCU芯片位号 */
    uint16_t usMCUFWVern; /* MCU固件版本 */
}SharedCMFirmwareVern_t;

typedef struct SharedCMVernTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedCMFirmwareVern_t stCMFirmwareVern;
}SharedCMVern_t;


/* BI版本结构体 */
typedef struct SharedBIFirmwareVernTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucHardwareVern; /* 硬件版本信息 */
    uint8_t ucSoftwareVern; /* 软件版本信息 */
    uint16_t usChipID; /* 芯片位号 */
    uint8_t ucChipFWVern; /* 芯片固件版本 */
}SharedBIFirmwareVern_t;

typedef struct SharedBIVernTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedBIFirmwareVern_t stBIFirmwareVern;
}SharedBIVern_t;


/* IO从站版本结构体 */
typedef struct SharedSlaveFirmwareVernTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucHardwareVern; /* 硬件版本信息 */
    uint8_t ucSoftwareVern; /* 软件版本信息 */
    uint16_t usChip1ID; /* 芯片1位号 */
    uint8_t ucChip1FWVern; /* 芯片1固件版本 */
    uint16_t usChip2ID; /* 芯片2位号 */
    uint8_t ucChip2FWVern; /* 芯片2固件版本 */
    uint16_t usChip3ID; /* 芯片3位号 */
    uint8_t ucChip3FWVern; /* 芯片3固件版本 */
    uint16_t usChip4ID; /* 芯片4位号 */
    uint8_t ucChip4FWVern; /* 芯片4固件版本 */
}SharedSlaveFirmwareVern_t;

typedef struct SharedSlaveVernTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedSlaveFirmwareVern_t stSlaveFirmwareVern;
}SharedSlaveVern_t;


/* PM状态信息结构体 */
typedef struct SharedUPStateTag
{
    uint32_t uiUPPollingTime; /* 任务从站轮询时间 */
    uint8_t ucUPState; /* 任务状态 */
    uint32_t uiUPScanTime; /* 任务配置扫描时间，如实际扫描时间大于配置扫描时间，则显示实际扫描时间 */
    uint32_t uiUPScanSurplus; /* 任务扫描后剩余时间 */
    uint32_t uiUPIECTime; /* UP程序执行时间 */
}SharedUPState_t;

typedef struct SharedPMFirmwareStateTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint32_t uiProjID; /* Project ID 工程ID */
    uint32_t uiMainVer; /* Main Version 主版本 */
    uint32_t uiMinorVer; /* Minor Version 次版本 */
    uint32_t uiMaxDDR; /* 最大内存 */
    uint32_t uiFreeDDR; /* 空闲内存 */
    uint8_t ucKeyPosition; /* 钥匙开关位置 */
    SharedUPState_t stSharedUPState[MAX_UP_NUM];
    uint8_t ucIPBUSState; /* IPBUS状态 */
    uint8_t ucPrePMBUSState; /* 前系PMBUS状态 */
    uint8_t ucNextPMBUSState; /* 后系PMBUS状态 */
    uint8_t ucCMBUSState[4]; /* CMBUS状态 */
    uint16_t usStateLED; /* 状态灯状态 */
    uint16_t usKeyLED; /* 钥匙灯状态 */
    uint16_t usComLED; /* 通讯灯状态 */
}SharedPMFirmwareState_t;

typedef struct SharedPMStateTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedPMFirmwareState_t stPMFirmwareState;
}SharedPMState_t;


/* CM状态信息结构体 */
typedef struct SharedCMFirmwareStateTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint16_t usDDRUsage; /* 内存使用率 */
    uint16_t usCPUUsage; /* CPU使用率 */
    uint16_t usSystemNetUsage; /* 系统网负荷 */
    uint8_t ucCMBUSAState; /* A系CMBUS状态 */
    uint8_t ucCMBUSBState; /* B系CMBUS状态 */
    uint8_t ucCMBUSCState; /* C系CMBUS状态 */
    uint16_t ucStateLED; /* 状态灯状态 */
    uint16_t ucComLED; /* 通讯灯状态 */
}SharedCMFirmwareState_t;

typedef struct SharedCMStateTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedCMFirmwareState_t stCMFirmwareState;
}SharedCMState_t;


/* IO从站状态信息结构体 */
typedef struct SharedSlaveDiagTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucOnlineDiag; /* 离线在线诊断 */
    uint8_t ucStateDiag; /* 状态诊断 */
    uint8_t ucTempDiag; /* 温度诊断 */
    uint8_t DeviceDiag[3]; /* 设备诊断 */
    uint8_t ChannelDiag[32][3]; /* 通道诊断 */
}SharedSlaveDiag_t;

typedef struct SharedSlaveStateTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedSlaveDiag_t stSlaveDiag;
}SharedSlaveState_t;


typedef struct SharedAppendTag
{
    uint32_t uiProjID; /* Project ID 工程ID */
    uint32_t uiMainVer; /* Main Version 主版本 */
    uint32_t uiMinorVer; /* Minor Version 次版本 */
    uint32_t uiProjCRC; /* Project CRC 工程CRC */
    ulong64_t ulTime; /* 时间戳 */
    uint8_t ucModBusCtrl[MAX_MODBUS_CTRL_INFO]; /* Modbus启停控制 */
    uint8_t ucP2PCtrl[MAX_P2P_CTRL_INFO]; /* P2P启停控制 */
    uint8_t ucSOECtrl[MAX_SOE_CTRL_INFO]; /* SOE启停控制。0：未配置；1：开始；2：停止 */
    SharedPMState_t stPMState; /* PM状态 */
    SharedCMState_t stCMState[MAX_CM_NUM]; /* CM状态 */
//    SharedBIState_t stBIState[MAX_BI_NUM]; /* BI状态 */
    SharedSlaveState_t stSlaveState[MAX_SLAVE_NUM]; /* 通道状态 */
    SharedPMVern_t stPMVern; /* PM版本 */
    SharedCMVern_t stCMVern[MAX_CM_NUM]; /* CM版本 */
    SharedBIVern_t stBIVern[MAX_BI_NUM]; /* BI版本 */
    SharedSlaveVern_t stSlaveVern[MAX_SLAVE_NUM]; /* Slave版本，从站号4为数组元素[4] */
}SharedAppend_t;

typedef struct SharedRTDataAreaTag
{
    bool_t bLock;                                  /* 是否可以被访问:true-不允许访问 false-允许访问 */
    uint8_t ucReserved1[3];
    bool_t bStatus;                                /* 两种状态：只读或只写: true-只读 false-只写*/
    uint8_t ucReserved2[3];
    uint8_t ucG[MAX_RTDATA_G_AREA_LEN];            /* G区表决后数据内容 */
    uint8_t ucI[MAX_RTDATA_I_AREA_LEN];            /* I区表决后数据内容 */
    uint8_t ucQ[MAX_RTDATA_Q_AREA_LEN];            /* Q区表决后数据内容 */
    SharedAppend_t stAppend;                       /* 附加内容 */
}SharedRTDataArea_t;

#pragma pack()

/* 数据区类型，供内部通讯使用 */
typedef enum SysRtDataAreaTypeTag
{
    RTDATA_UP1_G_AREA = 0,                         /* UP1 G区 */
    RTDATA_UP2_G_AREA = 1,                         /* UP2 G区 */
    RTDATA_I_AREA = 2,                             /* I区 */
    RTDATA_Q_AREA = 3,                             /* Q区 */
    RTDATA_APPEND_AREA = 4,
    MAX_RTDATA_AREA_NUM = 5                        /* 数据区个数 */
} SysRtDataAreaType_t;

/* 实时数据区类型，供共享内存实时数据拷贝使用 */
typedef enum SysRtDataTypeTag
{
    RT_DATA_TYPE_G = 0x00,
    RT_DATA_TYPE_I,
    RT_DATA_TYPE_Q,
    NUM_OF_RT_DATA_TYPE,
    INVALID_RT_DATA_TYPE
} SysRtDataType_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedRtDataInit
*
* 功能描述: 初始化
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
void SharedRtDataInit(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetSharedMemRtDataStartAddr
*
* 功能描述: 获取共享内存的实时数据区首地址
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
puint8_t SysGetSharedMemRtDataStartAddr(SysRtDataType_t emDataType);

/*
***********************************************************************************************************************
* 函数名称: SysGetSharedMemAppendInfoStartAddr
*
* 功能描述: 获取共享内存的附加区首地址
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
puint8_t SysGetSharedMemAppendInfoStartAddr(void);

/*
***********************************************************************************************************************
*                                全局变量
***********************************************************************************************************************
*/
extern SharedRTDataArea_t g_stSharedRtData;


#endif /* SHARED_RTDATA_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
