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
*  文件名称    : SysRTData.h
*
*  功能简介    :
*
*  作者       : 李琦
*
*  创建日期    : 2016-02-19
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_RTDATA_H_
#define SYS_RTDATA_H_

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
#define MAX_RTDATA_MSG_LEN              (4096u)        /*每包实时数据最大长度 */
#define MAX_RTDATA_ACK_MSG_LEN          (64u)          /*每包实时数据最大长度 */

#define MAX_RTDATA_AREA_LEN             (316*1024)    /* 实时数据区大小 */
#define MAX_RTDATA_G_AREA_LEN           (300*1024u)   /* G区大小 */
#define MAX_RTDATA_I_AREA_LEN           (8*1024u)     /* I区大小 */
#define MAX_RTDATA_Q_AREA_LEN           (8*1024u)     /* Q区大小 */
#define MAX_RTDATA_APPEND_INFO_SIZE     (1024*50U)    /* 预留50*1024字节存放附加信息等 */

#define RTDATA_AREA_LOCKED              (true)        /* 数据区被锁定，不允许访问*/
#define RTDATA_AREA_UNLOCKED            (false)       /* 数据区未锁定，允许访问 */

#define RTDATA_AREA_READ_ONLY           (true)        /* 实时数据区只读标识 */
#define RTDATA_AREA_WRITE_ONLY          (false)       /* 实时数据区只写标识 */

#define MAX_MODBUS_CTRL_INFO            (64U)
#define MAX_P2P_CTRL_INFO               (128U)
#define MAX_SOE_CTRL_INFO               (16U)
#define MAX_SLAVE_NUM                   (30)     /* 模块的最大个数 */
//#define MAX_SLAVE_NUM                   (240)     /* 模块的最大个数 */
#define MAX_PM_NUM                      (3U)      /* PM模块的最大个数 */
#define MAX_CM_NUM                      (4U)      /* CM模块的最大个数 */
#define MAX_BI_NUM                      (15U)     /* BI模块的最大个数 */
#define MAX_PW_NUM                      (15U)     /* PW模块的最大个数 */
#define MAX_UP_NUM                      (2U)      /* UP的最大个数 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/* 数据区类型 */
typedef enum SysRtDataAreaTypeTag
{
    RTDATA_G_AREA = 0,                             /* G区 */
    RTDATA_I_AREA = 1,                             /* I区 */
    RTDATA_Q_AREA = 2,                             /* Q区 */
    MAX_RTDATA_AREA_NUM = 3                        /* 数据区个数 */
} SysRtDataAreaType_t;

/* 消息*/
typedef struct SysRTDataMsgTag
{
    uint16_t usLen;                                 /* 消息内容长度 */
    uint8_t ucContent[MAX_RTDATA_MSG_LEN];          /* 存放消息内容 */
} SysRTDataMsg_t;

/* 信箱 */
typedef struct SysRTDataMsgBoxTag
{
    bool_t bLock;                                   /* 信箱是否可以被访问 */
    bool_t bMsgBoxStatus;                           /* 信箱的两种状态：空或满 */
    SysRTDataMsg_t stMsg;                           /* 消息 */
} SysRTDataMsgBox_t;

/* 消息*/
typedef struct SysRTDataAckMsgTag
{
    uint16_t usLen;                                 /* 消息内容长度 */
    uint8_t ucContent[MAX_RTDATA_ACK_MSG_LEN];      /* 存放消息内容 */
} SysRTDataAckMsg_t;

/* 信箱 */
typedef struct SysRTDataAckMsgBoxTag
{
    bool_t bLock;                                   /* 信箱是否可以被访问 */
    bool_t bMsgBoxStatus;                           /* 信箱的两种状态：空或满 */
    SysRTDataAckMsg_t stMsg;                        /* 消息 */
} SysRTDataAckMsgBox_t;

/* 消息类型（请求或应答）*/
typedef enum SysRTDataMsgTypeTag
{
    REAL_TIME_DATA = 0,                             /* 实时数据 */
    REAL_TIME_DATA_ACK = 1,                         /* 对实时数据的应答 */
    NUM_OF_RTDATAMSGTYPE
} SysRTDataMsgType_t;

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

/* BI诊断信息结构体 */
typedef struct SharedBIDiagInfoTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    int16_t sBITemperature; /* BI板温度 */
    int16_t sSEP1Temperature; /* SFP1模块的温度 */
    int16_t sSEP2Temperature; /* SFP2模块的温度 */
    int16_t sSEP3Temperature; /* SFP3模块的温度 */
}SharedBIDiagInfo_t;

typedef struct SharedBIStateTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedBIDiagInfo_t stBIDiagInfo;  /* BI诊断信息*/
}SharedBIState_t;

/* BI诊断信息结构体 */
typedef struct SharedPWDiagInfoTag
{
    /*"Type of unit, revision of firmware: 
     * revision of primary firmware:PW1_AA01_DT1.PW1_AA01_DT2
     * revision of secondary firmware:PW1_AA01_DT3.PW1_AA01_DT4*/
    uint8_t ucPW1_AA01_DT1; 
    uint8_t ucPW1_AA01_DT2; 
    uint8_t ucPW1_AA01_DT3; 
    uint8_t ucPW1_AA01_DT4; 
    /*Total Operational hours (big endian format)*/
    uint8_t ucPW1_AA02_DT1; 
    uint8_t ucPW1_AA02_DT2; 
    uint8_t ucPW1_AA02_DT3;
    /*Expired portion of lifetime(percentage format)"*/
    uint8_t ucPW1_AA03_DT1;
    /*maximum ambient temperature of current operating period 
     * (the value minused by 50 is the real celsius degree)*/
    int8_t cPW1_AB01_DT1;
    /*Current period Maximal input voltages (big endian format)*/
    uint8_t ucPW1_AC01_DT1; 
    uint8_t ucPW1_AC01_DT2; 
    /*Type of input, 1 : AC ; 0 : DC*/
    uint8_t ucPW1_AC01_DT3;
    /*Last failure report:bit0:OTP, bit1:OVP_input, bit2:UVP_input*/
    uint8_t ucPW1_AA06_DT1;
    /*Number of VDE 0160 transient in entire life cycle (big endian format)*/
    uint8_t ucPW1_AA07_DT1;
    uint8_t ucPW1_AA07_DT2;
    /*Cumulative running time(hour) of over-temperature (big endian format)*/
    uint8_t ucPW1_AA08_DT1;
    uint8_t ucPW1_AA08_DT2;
    /*Current period operational hours (big endian format) */
    uint8_t ucPW1_AD01_DT1;
    uint8_t ucPW1_AD01_DT2;
    uint8_t ucPW1_AD01_DT3;
    /*Last period operational hours (big endian format)*/
    uint8_t ucPW1_AD02_DT1;
    uint8_t ucPW1_AD02_DT2;
    uint8_t ucPW1_AD02_DT3;
    /*Last period maximum ambient temperatures
    (the value minused by 50 is the real celsius degree)*/
    uint8_t ucPW1_AB02_DT1;
    /*Last period maximal input voltages (big endian format)*/
    uint8_t ucPW1_AC02_DT1;
    uint8_t ucPW1_AC02_DT2;
    /*Type of input, 1 : AC ; 0 : DC*/
    uint8_t ucPW1_AC02_DT3;
    /*Number of VDE 0160 transient in current operating period (big endian format)"*/
    uint8_t ucPW1_AE01_DT1;
    uint8_t ucPW1_AE01_DT2;
    /*Number of VDE 0160 transient in last operating period (big endian format) */
    uint8_t ucPW1_AE02_DT1;
    uint8_t ucPW1_AE02_DT2;
    /*Current Failure report: bit0:OTP, bit1:OVP_input, bit2:UVP_input*/
    uint8_t ucPW1_AF01_DT1;
    /*Total Failure report of OTP in entire life cycle (big endian format)*/
    uint8_t ucPW1_AF02_DT1;
    uint8_t ucPW1_AF02_DT2;
    uint8_t ucPW1_AF02_DT3;
    /*Total Failure report of OVP INPUT in entire life cycle (big endian format)*/
    uint8_t ucPW1_AF03_DT1;
    uint8_t ucPW1_AF03_DT2;
    uint8_t ucPW1_AF03_DT3;
    /*Total Failure report of UVP INPUT in entire life cycle (big endian format)*/
    uint8_t ucPW1_AF04_DT1;
    uint8_t ucPW1_AF04_DT2;
    uint8_t ucPW1_AF04_DT3;
    
    /*"Type of unit, revision of firmware: 
     * revision of primary firmware:PW2_AA01_DT1.PW2_AA01_DT2
     * revision of secondary firmware:PW2_AA01_DT3.PW2_AA01_DT4*/
    uint8_t ucPW2_AA01_DT1; 
    uint8_t ucPW2_AA01_DT2; 
    uint8_t ucPW2_AA01_DT3; 
    uint8_t ucPW2_AA01_DT4; 
    /*Total Operational hours (big endian format)*/
    uint8_t ucPW2_AA02_DT1; 
    uint8_t ucPW2_AA02_DT2; 
    uint8_t ucPW2_AA02_DT3;
    /*Expired portion of lifetime(percentage format)"*/
    uint8_t ucPW2_AA03_DT1;
    /*maximum ambient temperature of current operating period 
     * (the value minused by 50 is the real celsius degree)*/
    int8_t cPW2_AB01_DT1;
    /*Current period Maximal input voltages (big endian format)*/
    uint8_t ucPW2_AC01_DT1; 
    uint8_t ucPW2_AC01_DT2; 
    /*Type of input, 1 : AC ; 0 : DC*/
    uint8_t ucPW2_AC01_DT3;
    /*Last failure report:bit0:OTP, bit1:OVP_input, bit2:UVP_input*/
    uint8_t ucPW2_AA06_DT1;
    /*Number of VDE 0160 transient in entire life cycle (big endian format)*/
    uint8_t ucPW2_AA07_DT1;
    uint8_t ucPW2_AA07_DT2;
    /*Cumulative running time(hour) of over-temperature (big endian format)*/
    uint8_t ucPW2_AA08_DT1;
    uint8_t ucPW2_AA08_DT2;
    /*Current period operational hours (big endian format) */
    uint8_t ucPW2_AD01_DT1;
    uint8_t ucPW2_AD01_DT2;
    uint8_t ucPW2_AD01_DT3;
    /*Last period operational hours (big endian format)*/
    uint8_t ucPW2_AD02_DT1;
    uint8_t ucPW2_AD02_DT2;
    uint8_t ucPW2_AD02_DT3;
    /*Last period maximum ambient temperatures
    (the value minused by 50 is the real celsius degree)*/
    uint8_t ucPW2_AB02_DT1;
    /*Last period maximal input voltages (big endian format)*/
    uint8_t ucPW2_AC02_DT1;
    uint8_t ucPW2_AC02_DT2;
    /*Type of input, 1 : AC ; 0 : DC*/
    uint8_t ucPW2_AC02_DT3;
    /*Number of VDE 0160 transient in current operating period (big endian format)"*/
    uint8_t ucPW2_AE01_DT1;
    uint8_t ucPW2_AE01_DT2;
    /*Number of VDE 0160 transient in last operating period (big endian format) */
    uint8_t ucPW2_AE02_DT1;
    uint8_t ucPW2_AE02_DT2;
    /*Current Failure report: bit0:OTP, bit1:OVP_input, bit2:UVP_input*/
    uint8_t ucPW2_AF01_DT1;
    /*Total Failure report of OTP in entire life cycle (big endian format)*/
    uint8_t ucPW2_AF02_DT1;
    uint8_t ucPW2_AF02_DT2;
    uint8_t ucPW2_AF02_DT3;
    /*Total Failure report of OVP INPUT in entire life cycle (big endian format)*/
    uint8_t ucPW2_AF03_DT1;
    uint8_t ucPW2_AF03_DT2;
    uint8_t ucPW2_AF03_DT3;
    /*Total Failure report of UVP INPUT in entire life cycle (big endian format)*/
    uint8_t ucPW2_AF04_DT1;
    uint8_t ucPW2_AF04_DT2;
    uint8_t ucPW2_AF04_DT3;

}SharedPWDiagInfo_t;

typedef struct SharedPWStateTag
{
    uint8_t ucActive; /* 该元素是否使用 */
    SharedPWDiagInfo_t stPWDiagInfo;  /* PW诊断信息*/
}SharedPWState_t;
/* IO从站版本结构体 */
typedef struct SharedSlaveFirmwareVernTag
{
    uint8_t ucSlotIndex; /* 模块所在的物理槽位序号 */
    uint16_t usModuleType; /* 模块类型 */
    uint8_t ucHardwareVern; /* 硬件版本信息 */
    uint8_t ucSoftwareVern; /* 软件版本信息 */
    uint16_t usChip1ID; /* 芯片1(A FPGA)位号 */
    uint8_t ucChip1FWVern; /* 芯片1固件版本 */
    uint16_t usChip2ID; /* 芯片2(A MCU)位号 */
    uint8_t ucChip2FWVern; /* 芯片2固件版本 */
    uint16_t usChip3ID; /* 芯片3(B FPGA)位号 */
    uint8_t ucChip3FWVern; /* 芯片3固件版本 */
    uint16_t usChip4ID; /* 芯片4(B MCU)位号 */
    uint8_t ucChip4FWVern; /* 芯片4固件版本 */
    uint16_t usChip5ID; /* 芯片5(C FPGA)位号 */
    uint8_t ucChip5FWVern; /* 芯片5固件版本 */
    uint16_t usChip6ID; /* 芯片6(C MCU)位号 */
    uint8_t ucChip6FWVern; /* 芯片6固件版本 */
    uint16_t usChip7ID; /* 芯片7(PCLD)位号 */
    uint8_t ucChip7FWVern; /* 芯片7固件版本 */
    uint16_t usChip8ID; /* 芯片8(PowerPC)位号 */
    uint8_t ucChip8FWVern; /* 芯片8固件版本 */
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
    uint8_t ucUPTimeout; /* 任务超时状态 */
    uint8_t ucUPTimeoutCnt; /* 任务超时计数  */
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
//    uint16_t usKeyPosition; /* 钥匙开关位置 */
    uint8_t ucKeyPosFlg;    /* 钥匙开关标志（当本系与表决结果一致时，为0，否则为1） */
    uint8_t ucKeyPosition;  /* 钥匙开关位置 */
    SharedUPState_t stSharedUPState[MAX_UP_NUM];
    uint8_t ucIPBUSState; /* IPBUS状态 */
    uint8_t ucPrePMBUSState; /* 前系PMBUS状态 */
    uint8_t ucNextPMBUSState; /* 后系PMBUS状态 */
    uint8_t ucCMBUSState[4]; /* CMBUS状态 */
    uint16_t usStateLED; /* 状态灯状态 */
    uint16_t usKeyLED; /* 钥匙灯状态 */
    uint16_t usComLED; /* 通讯灯状态 */
    uint16_t usPowerMonitor; /* 电源监测 */
    float_t fTemperatureMonitor; /* 温度监测 */
    uint16_t usMCUMonitor; /* MCU监测 */
    uint16_t usPPMMonitor; /* PPM监测 */
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
    uint16_t usSystemNet1Usage; /* 系统网1负荷 */
    uint16_t usSystemNet2Usage; /* 系统网2负荷 */
    uint8_t ucCMBUSAState; /* A系CMBUS状态 */
    uint8_t ucCMBUSBState; /* B系CMBUS状态 */
    uint8_t ucCMBUSCState; /* C系CMBUS状态 */
    uint16_t ucStateLED; /* 状态灯状态 */
    uint16_t ucComLED; /* 通讯灯状态 */
    uint16_t usPowerMonitor; /* 电源监测 */
    float_t fTemperatureMonitor; /* 温度监测 */
    uint16_t usMCUMonitor; /* MCU监测 */
    uint16_t usPluseMonitor; /* 脉冲监测 */
    uint8_t ucModbusState[6]; /* Modbus状态 */
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
    int8_t cTempDiag; /* 温度诊断 */
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
    SharedBIState_t stBIState[MAX_BI_NUM];      /* BI诊断信息*/
    SharedPWState_t stPWState[MAX_PW_NUM];      /* PW诊断信息*/
    uint8_t ucFWUpdateFlg;
}SharedAppend_t;

/* 实时数据区 */
typedef struct RTDataAreaTag
{
    bool_t bLock;                                   /* 是否可以被访问:true-不允许访问 false-允许访问 */
    bool_t bDataAreaStatus;                         /* 两种状态：只读或只写: true-只读 false-只写*/
    uint8_t ucContent[MAX_RTDATA_G_AREA_LEN];       /* G区数据内容 */
    uint8_t ucInputContent[MAX_RTDATA_I_AREA_LEN];  /* I区数据内容 */
    uint8_t ucOutputContent[MAX_RTDATA_Q_AREA_LEN]; /* Q区数据内容 */
    uint8_t ucAppendInfo[MAX_RTDATA_APPEND_INFO_SIZE];
} RTDataArea_t;

/* 两个实时数据区交替使用 */
typedef enum RTDataAreaIndexTag
{
    RTDATA_AREA_A = 0,                              /* 实时数据区A*/
    RTDATA_AREA_B = 1,                              /* 实时数据区B */
    NUM_OF_RTDATA_AREA,                             /* 实时数据区个数 */
    INVALID_RTDATA_AREA_INDEX                       /* 实时数据区错误的索引号 */
} RTDataAreaIndex_t;

#pragma pack()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysRTDataInit
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
void SysRTDataInit(void);

/*
***********************************************************************************************************************
* 函数名称: SysRTDataClear
*
* 功能描述: 清除实时数据区
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
void SysRTDataClear(void);
/*
***********************************************************************************************************************
* 函数名称: SysRTDataClearRelatedPM
*
* 功能描述: 清除对应PM的实时数据区
*
* 输入参数: emPMID: PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysRTDataClearRelatedPM(PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataMsg
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
*        false-未收到消息或者输入输出参数有误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataMsg (PMController_t emPMID, SysRTDataMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen);

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataMsgLen
*
* 功能描述: 从系统资源模块获取消息（请求/应答）的长度
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataMsgLen (PMController_t emPMID, SysRTDataMsgType_t emMsgType, puint16_t pusLen);

/*
***********************************************************************************************************************
* 函数名称: SysSetRTDataMsg
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
bool_t SysSetRTDataMsg (PMController_t emPMID, SysRTDataMsgType_t emMsgType , puint8_t pucContent, uint16_t usLen);

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataAddr
*
* 功能描述: 获取实时数据区起始地址
*
* 输入参数: emPMID: PM索引号
*        emDataArea:数据区类型
*
* 输出参数: 无
*
* 返 回 值  : 实时数据区起始地址
*
* 注意事项: 两个实时数据区均不可读则返回NULL
***********************************************************************************************************************
*/
puint8_t SysGetRTDataAddr(PMController_t emPMID, SysRtDataAreaType_t emDataArea);
//puint8_t SysGetRTDataAddr(PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataArea
*
* 功能描述: 获取实时数据区结构体指针
*
* 输入参数: emPMID: 来自某个PM的实时数据
*
* 输出参数: 无
*
* 返 回 值  : 实时数据区结构体指针
*
* 注意事项: 无
***********************************************************************************************************************
*/
RTDataArea_t* SysGetRTDataArea(PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppendInfo
*
* 功能描述: 获取附加区信息
*
* 输入参数: 无
*
* 输出参数: emPMID PM的ID号
*        pstAppend 指向附加区内容
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppendInfo(PMController_t emPMID, SharedAppend_t *pstAppend);

/*
***********************************************************************************************************************
* 函数名称: SysGetAppendInfoAddr
*
* 功能描述: 获取附加区信息的地址
*
* 输入参数: 无
*
* 输出参数: emPMID PM的ID号
*
* 返 回 值  : 成功：附加区起始地址
*        失败： NULL
*
* 注意事项: 无
***********************************************************************************************************************
*/
SharedAppend_t * SysGetAppendInfoAddr(PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetAppendInfo
*
* 功能描述: 记录附加区内容
*
* 输入参数: emPMID PM的ID号
*        pucAppendInfo 待写入的内容
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppendInfo(PMController_t emPMID, puint8_t pucAppendInfo);

#endif /* SYS_RTDATA_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

