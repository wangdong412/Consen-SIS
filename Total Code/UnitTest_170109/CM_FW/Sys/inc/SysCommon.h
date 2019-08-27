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
*  文件名称    : Sys.h
*
*  功能简介    : 外部通讯模块、上位机应用模块及内部通讯模块通过系统资源模块中收发缓冲区进行模块间的通讯，并且各模块可以将相关信息存储到系统资源模块中供其它模块查询
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-31
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_COMMON_H_
#define SYS_COMMON_H_

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
#define INTER_COMM_INVALID          (0xFFFF)      /* 内部通讯无效状态 */
#define INTER_COMM_NORMAL           (0x0000)      /* 内部通讯正常 */
#define INTER_COMM_DETECT_MSG       (0x0001)      /* 内部通讯异常，收到来自PM的探测消息 */
#define INTER_COMM_ABNORMAL         (0x0002)      /* 内部通讯正常 */

#define EXTER_COMM_INVALID          (0xFFFF)      /* 外部通讯无效状态 */
#define EXTER_COMM_NORMAL           (0x0000)      /* 外部通讯正常 */
#define EXTER_COMM_ABNORMAL         (0x0001)      /* 外部通讯异常 */

#define GET_SLOT_TIMEOUT            100
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
/* PM的ID */
typedef enum SysPMControllerTag
{
    PMA = 0x00,
    PMB,
    PMC,
    NUM_OF_PM,
    INVALID_PM_ID
} PMController_t;

/* CM的ID */
typedef enum SysCMControllerTag
{
    CM1 = 0x00,
    CM2,
    CM3,
    CM4,
    NUM_OF_CM,
    INVALID_CM_ID
} CMController_t;

/* CM的槽号 */
typedef enum SysCMSlotTag
{
    SLOT3 = 0x03,
    SLOT4,
    SLOT5,
    SLOT6,
    SLOT7,
    SLOT8,
    SLOT9,
    SLOT10,
    SLOT11,
    SLOT12,
    SLOT13,
    SLOT14,
    SLOT15,
    NUM_OF_SLOT,
    INVALID_CM_SLOT
} CMSlot_t;

/* 网络通讯端口类型 */
typedef enum emNetworkPortTypeTag
{
    NET1 = 0x00,
    NET2,
    NUM_OF_NETWORK_PORT,
    INVALID_NETWORK_PORT
} NetworkPortType_t;

/* 网络通讯端口类型 */
typedef enum emSerialPortTypeTag
{
    COM1 = 0x00,
    COM2,
    COM3,
    COM4,
    NUM_OF_SERIAL_PORT,
    INVALID_SERIAL_PORT
} SerialPortType_t;

/* Communication Port  通信端口枚举定义 */
typedef enum CommPortTag
{
    PORT_COM1 = 0,   /* COM 1  串口1 */
    PORT_COM2,       /* COM 2  串口2 */
    PORT_COM3,       /* COM 3  串口3 */
    PORT_COM4,       /* COM 4  串口4 */
    PORT_NET1,       /* Net 1  网口1 */
    PORT_NET2,       /* Net 2  网口2 */
    PORT_NUM
}CommPort_t;

/* 系统时间*/
typedef struct SysSystemTimeTag
{
    uint32_t uiSecond;     /*时间 s*/
    uint16_t usMiliSecond;  /*时间 ms*/
} SysSystemTime_t;

/* Project Version Info. 工程版本信息结构体定义 */
typedef struct ProjVerInfoTag
{
    uint32_t uiProjID;      /* Project ID      工程ID */
    uint32_t uiMainVer;     /* Main Version  主版本 */
    uint32_t uiMinorVer;    /* Minor Version 次版本 */
    uint32_t uiProjCRC;     /* Project CRC    工程CRC */
}ProjVerInfo_t;


/* PM状态 */
typedef struct SysPMStateTag
{
    /* PM状态 */
    uint16_t usPMState[NUM_OF_PM];
    /* PM设置的系统时间 */
    SysSystemTime_t stPMtime[NUM_OF_PM];
} SysPMState_t;

/* CM状态 */
typedef struct SysCMStateTag
{
    /* CM状态 */
    uint16_t usCMState[NUM_OF_CM];
    /* 内部通讯状态 */
    uint16_t usInterCommState[NUM_OF_CM][NUM_OF_PM];
    /* 各通讯口的外部通讯状态 */
    uint16_t usExterCommState[NUM_OF_CM][NUM_OF_NETWORK_PORT];
    /* CM设置的系统时间 */
    SysSystemTime_t stCMtime[NUM_OF_CM];
} SysCMState_t;

/* CM配置表 */
typedef struct SysCMConfigTableTag
{
    /* 是否配置了该CM*/
    bool_t bValid[NUM_OF_CM];
    /* 逻辑号 */
    uint8_t ucLogicID[NUM_OF_CM];
    /* 槽号 */
    uint8_t ucSlotID[NUM_OF_CM];
} SysCMConfigTable_t;

/* CM to PM info */
/* CM版本 */
typedef struct SysCMVernTag
{
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
}SysCMVern_t;

typedef struct SysCM2PMTag
{
    /* CM版本号 */
    SysCMVern_t stCMVern;
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
    float_t fTemperatureMonitor; /* 监测 */
    uint16_t usMCUMonitor; /* MCU监测 */
    uint16_t usPluseMonitor; /* 脉冲监测 */
    uint8_t ucModbusState[6]; /* Modbus状态 */
} SysCM2PM_t;

/* CM状态数据块 */
typedef struct SysCMStateBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(SysState_t)*/
    uint16_t usLen;
    /* 系统状态 */
    SysCMState_t stCMState;
    ProjVerInfo_t stProjVer;
    /* CM TO PM INFO */
    SysCM2PM_t stCM2PMInfo;
    /* 校验码 */
    uint32_t uiCrc32;
} SysCMStateBlock_t;

typedef struct PrjInfoTag
{
    uint8_t ucIsConfigured;                     /*工程是否解析完成*/
    uint8_t ucIsConfirmed;                      /*工程是否经用户确认*/
    uint8_t ucIsIOConfigured;                   /*IO配置是否完成*/
    uint8_t ucRtDataSync;                       /*实时数据是否同步完成*/
    uint32_t uiPrjID;
    uint32_t uiMainVern;
    uint32_t uiMinorVern;
    uint32_t uiPrjCRC;
}PrjInfo_t;

typedef struct TaskInformationTag
{
    uint8_t ucTaskNum;
    uint8_t ucTask1State;
    uint8_t ucTask2State;
}TaskInformation_t;

/* 系统状态 */
typedef struct SysStateTag
{
    /* PMA~C状态 */
    SysPMState_t stPMState;
    /* CM1~4状态 */
    SysCMState_t stCMState;
    /* CM logic ID */
    uint32_t uiCMLogicID;
    /* Project info */
    PrjInfo_t stPrjInfo;
    PrjInfo_t stDownloadPrjInfo;
    TaskInformation_t stTaskInfo;
} SysState_t;

/* 系统状态数据块 */
typedef struct SysStateBlockTag
{
    /* 起始标识 */
    uint16_t usStartFlag;
    /* 数据长度  sizeof(SysState_t)*/
    uint16_t usLen;
    /* 系统状态 */
    SysState_t stSysState;
    /* 校验码 */
    uint32_t uiCrc32;
} SysStateBlock_t;


/* RT Data Version Info. 实时数据版本信息结构体定义 */
typedef struct RTDataVerInfoTag
{
    uint32_t uiProjID;    /* Project ID 工程ID */
    uint32_t uiMainVer;   /* Main Version 主版本 */
    uint32_t uiMinorVer;  /* Minor Version 次版本 */
    uint32_t uiProjCRC;   /* Project CRC 工程CRC */
}RTDataVerInfo_t;


#pragma pack()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysInit
*
* 功能描述: 系统资源管理模块初始化
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
void SysInit(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMState
*
* 功能描述: 获取指定PM的状态
*
* 输入参数: emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定PM的状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetPMState(PMController_t emPMID, uint16_t *pusState);

/*
***********************************************************************************************************************
* 函数名称: SysSetPMState
*
* 功能描述: 设置指定PM的状态
*
* 输入参数: emPMID:   指定PM的ID
*        usPMState:PM的状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetPMState(PMController_t emPMID, uint16_t usPMState);

/*
***********************************************************************************************************************
* 函数名称: SysGetSysCMState
*
* 功能描述: 获取所有CM的系统状态
*
* 输入参数: 无
*
* 输出参数: pstSysCMState: 指向所有CM的系统状态
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetSysCMState(SysCMState_t *pstSysCMState);

/*
***********************************************************************************************************************
* 函数名称: SysGetCMState
*
* 功能描述: 获取指定CM的状态
*
* 输入参数: emCMID:指定CM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定CM的状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMState(CMController_t emCMID, uint16_t *pusState);

/*
***********************************************************************************************************************
* 函数名称: SysSetCMState
*
* 功能描述: 设置指定CM的状态
*
* 输入参数: emCMID:   指定CM的ID
*        usCMState:CM的状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMState(CMController_t emCMID, uint16_t usCMState);

/*
***********************************************************************************************************************
* 函数名称: SysSetSysState
*
* 功能描述: 设置系统的状态
*
* 输入参数: pstSysState: 系统状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSysState(SysState_t *pstSysState, PMController_t emPMID);
/*
***********************************************************************************************************************
* 函数名称: SysGetSysTimeFromPM
*
* 功能描述: 获取来自PM的时间
*
* 输入参数: emPMID: PM的id
*
* 输出参数: pstSysTime 指向获取的时间
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetSysTimeFromPM(SysSystemTime_t *pstSysTime, PMController_t emPMID);
/*
***********************************************************************************************************************
* 函数名称: SysGetExterCommState
*
* 功能描述: 获取指定CM的指定通讯口的外部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emType:指定的通讯口
*
* 输出参数: 无
*
* 返 回 值  : 指定CM的某个通讯口的外部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetExterCommState(CMController_t emCMID, NetworkPortType_t emType);

/*
***********************************************************************************************************************
* 函数名称: SysSetExterCommState
*
* 功能描述: 设置指定CM的指定通讯口的外部通讯状态
*
* 输入参数: emCMID:   指定CM的ID
*        emType:指定的通讯口
*        usExterState:外部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetExterCommState(CMController_t emCMID, NetworkPortType_t emType, uint16_t usExterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetInterCommState
*
* 功能描述: 获取指定CM与指定PM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetInterCommState(CMController_t emCMID, PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetInterCommState
*
* 功能描述: 设置指定CM与指定PM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emPMID:指定PM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetInterCommState(CMController_t emCMID, PMController_t emPMID, uint16_t usInterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetSystemTime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
SysSystemTime_t SysGetSystemTime(void);

/*
***********************************************************************************************************************
* 函数名称: SysSetSystemTime
*
* 功能描述: 设置系统时间
*
* 输入参数: stSysTime: 系统时间
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSystemTime(SysSystemTime_t *pstSysTime);

/*
***********************************************************************************************************************
* 函数名称: SysGetCMSystemTime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysSystemTime_t SysGetCMSystemTime(CMController_t emCMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetCMSystemTime
*
* 功能描述: 设置系统时间
*
* 输入参数: stSysTime: 系统时间
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMSystemTime(CMController_t emCMID, SysSystemTime_t *pstSysTime);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMID
*
* 功能描述: 获取本地CM的ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地CM的ID
*
* 注意事项: 无
***********************************************************************************************************************
*/
CMController_t SysGetLocalCMID(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalExterCommState
*
* 功能描述: 获取本CM的指定通讯口的外部通讯状态
*
* 输入参数: emType:指定的通讯口
*
* 输出参数: 无
*
* 返 回 值  : 本CM的某个通讯口的外部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalExterCommState(NetworkPortType_t emType);

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalExterCommState
*
* 功能描述: 设置本CM的指定通讯口的外部通讯状态
*
* 输入参数: emType:指定的通讯口
*        usExterState:外部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalExterCommState(NetworkPortType_t emType, uint16_t usExterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalInterCommState
*
* 功能描述: 获取本CM与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 本CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalInterCommState(PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalInterCommState
*
* 功能描述: 设置本CM与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalInterCommState(PMController_t emPMID, uint16_t usInterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMSlot
*
* 功能描述: 获取本地CM的槽号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地CM的槽号
*
* 注意事项: 无
***********************************************************************************************************************
*/
CMSlot_t SysGetLocalCMSlot(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMTaskNum
*
* 功能描述: 获取某一PM中的任务个数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 任务个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetPMTaskNum(PMController_t emPMID);
/*
***********************************************************************************************************************
* 函数名称: SysSetPrjInfo
*
* 功能描述: 存储工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo);
/*
***********************************************************************************************************************
* 函数名称: SysSetDownloadPrjInfo
*
* 功能描述: 存储工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetDownloadPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo);
/*
***********************************************************************************************************************
* 函数名称: SysGetPrjInfo
*
* 功能描述: 获取工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo);
/*
***********************************************************************************************************************
* 函数名称: SysGetDownloadPrjInfo
*
* 功能描述: 获取工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetDownloadPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMTaskInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetPMTaskInfo( PMController_t emPMID, uint8_t ucTaskIndex, uint8_t *pucTaskID, int8_t *picTaskStatus );

/*
***********************************************************************************************************************
* 函数名称: SysSetPMTaskNum
*
* 功能描述: 存储某一PM中的任务个数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 任务个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPMTaskNum(PMController_t emPMID, uint8_t ucTaskNum);

/*
***********************************************************************************************************************
* 函数名称: SysSetPMTaskInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPMTaskInfo( PMController_t emPMID, uint8_t ucTask1Status, uint8_t ucTask2Status );

/*
***********************************************************************************************************************
* 函数名称: SysSetCM2PMVer
*
* 功能描述: 设置CM的版本号
*
* 输入参数: uiCMVer 版本号
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCM2PMVer(uint8_t ucCMRTSVer[], uint8_t ucCMOSVer[]);

/*
***********************************************************************************************************************
* 函数名称: SysSetCM2PMState
*
* 功能描述: 设置CM的状态灯状态
*
* 输入参数: uiCMLEDState
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCM2PMState(void);

void SysSetCM2PMComLED(uint16_t usComLED);

void SysSetCM2PMStateLED(uint16_t usStateLED);

void SysSetCM2PMTemp(float_t fTemperature);

void SysSetCM2PMMCUState(uint16_t usMCUState);

void SysSetCM2PMPluseState(uint16_t usPluseState);

void SysSetCM2PMPowerState(uint16_t usPowerState);

void SysSetCM2PMCMBUSState(PMController_t emPMID, uint8_t ucCMBUSState);

void SysSetCM2PMModbusState(CommPort_t emPort, uint8_t ucModbusState);

#endif /* SYS_COMMON_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

