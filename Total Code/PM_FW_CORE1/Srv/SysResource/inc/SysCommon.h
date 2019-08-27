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
*  文件名称    : SysCommon.h
*
*  功能简介    :
*
*  作者       : 李琦
*
*  创建日期    : 2016-02-24
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
#include "inc/DataType.h"
#include "Srv/Sys/inc/CommSys.h"
#include "SysCfgFile.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define CMBUS_COMM_INVALID          0xFFFF      /* CMBUS通讯无效状态 */
#define CMBUS_COMM_NORMAL           0x0000      /* CMBUS通讯正常 */
#define CMBUS_COMM_DETECT_MSG       0x0001      /* CMBUS通讯异常，向CM发送探测消息 */

#define RET_OK                      0           /* 公共返回值 ---OK             */
#define RET_FAIL                    -1          /* 公共返回值 ---fail           */

//#define MAX_CM_NUM                  (4U)

//#define MAX_FILE_NUM                (10)

#define MAX_FILE_NAME_LEN           (17U)   /* 文件名称最大字节数     */

#define FORMAL_FILE_NAME_LEN        (11U)
#define OLD_FILE_NAME_LEN           (15U)
#define TMP_FILE_NAME_LEN           (15U)

#define HARDCODE_ONE                (1U)



#ifndef MIN                                     /* Define MIN */
#define MIN(m,n)                    (((m) < (n)) ? (m) : (n))
#endif/* #ifndef MIN */

#ifndef MAX                                    /* Define MAX */
#define MAX(m,n)                    (((m) > (n)) ? (m) : (n))
#endif/* #ifndef MAX */

/* 文件校验错误 */
#define SYS_FILE_CHECK_ERR          (0x00000000U)
/* 文件校验无误 */
#define SYS_FILE_CHECK_OK           (0x00000001U)

#define CMP_EQUAL                   (0x00000000U)   /* compare equal 0 */
#define CMP_GREAT                   (0x00000001U)   /* compare great 1 */
#define CMP_LESS                    (0xFFFFFFFFU)   /* compare less -1 */




#define SYS_SEND_STAT               (0x00000002U)



#define BLANK_CHAR                   '\0'           /* BLANK_CHAR = '\0' = 0 */


/**********************************IEC工程涉及的宏定义*********************************/
/* 工程有效 */
#define SYS_PRJ_VALID               (0x01U)
/* 工程无效 */
#define SYS_PRJ_INVALID             (0x00U)
/* 工程已确认 */
#define SYS_PRJ_CONFIRM             (0x01U)
/* 工程未确认 */
#define SYS_PRJ_NOCONFIRM           (0x00U)
/* 工程已配置 */
#define SYS_PRJ_CONFIGURED          (0x01U)
/* 工程未配置 */
#define SYS_PRJ_NOCONFIGURED        (0x00U)
/* 配置过程中初始化工程数据区 */
#define SYS_PRJ_INITDATA            (0x01U)
/* 配置过程中不初始化工程数据区 */
#define SYS_PRJ_NOINITDATA          (0x00U)
/* 工程数据区需要同步 */
#define SYS_PRJ_SYNCDATA            (0x01U)
/* 工程数据区不需同步 */
#define SYS_PRJ_NOSYNCDATA          (0x00U)

#define FILE_HAS_CONFIG             (0x00000000)                /*文件已经被解析配置过*/
#define FILE_NOT_CONFIG             (0x000000aa)                /*文件没有被解析配置过*/
#define FILE_CONFIG_FLG_OFF         (0x00000000)                /*文件是否被配置过的标志在文件存储区的偏移*/



#define RTS_SC_FILE                 "DEFAULT.SC"        /* SC 文件               */
#define RTS_SC_FILE_TMP             "DEFAULT.SC.TMP"    /* 临时 SC 文件          */
#define RTS_SC_FILE_OLD             "DEFAULT.SC.OLD"    /* 旧 SC 文件          */
#define RTS_PRG_FILE                "DEFAULT.PRG"       /* PRG 文件               */
#define RTS_PRG_FILE_TMP            "DEFAULT.PRG.TMP"   /* 临时 PRG 文件          */
#define RTS_PRG_FILE_OLD            "DEFAULT.PRG.OLD"   /* 旧 PRG 文件          */
#define RTS_IOC_FILE                "DEFAULT.IOC"       /* IOC 文件               */
#define RTS_IOC_FILE_TMP            "DEFAULT.IOC.TMP"   /* 临时 IOC 文件          */
#define RTS_IOC_FILE_OLD            "DEFAULT.IOC.OLD"   /* 旧 IOC 文件          */
#define RTS_SOE_FILE                "DEFAULT.SOE"       /* SOE 文件               */
#define RTS_SOE_FILE_TMP            "DEFAULT.SOE.TMP"   /* 临时 SOE 文件          */
#define RTS_SOE_FILE_OLD            "DEFAULT.SOE.OLD"   /* 旧 SOE 文件          */
#define RTS_P2P_FILE                "DEFAULT.P2P"       /* P2P 文件               */
#define RTS_P2P_FILE_TMP            "DEFAULT.P2P.TMP"   /* 临时 P2P 文件          */
#define RTS_P2P_FILE_OLD            "DEFAULT.P2P.OLD"   /* 旧 P2P 文件          */
#define RTS_MMC_FILE                "DEFAULT.MMC"       /* MMC 文件               */
#define RTS_MMC_FILE_TMP            "DEFAULT.MMC.TMP"   /* 临时 MMC 文件          */
#define RTS_MMC_FILE_OLD            "DEFAULT.MMC.OLD"   /* 旧 MMC 文件          */
#define RTS_MSC_FILE                "DEFAULT.MSC"       /* MSC 文件               */
#define RTS_MSC_FILE_TMP            "DEFAULT.MSC.TMP"   /* 临时 MSC 文件          */
#define RTS_MSC_FILE_OLD            "DEFAULT.MSC.OLD"   /* 旧 MSC 文件          */
#define RTS_VOT_FILE                "DEFAULT.VOT"       /* VOT 文件               */
#define RTS_VOT_FILE_TMP            "DEFAULT.VOT.TMP"   /* 临时 VOT 文件          */
#define RTS_VOT_FILE_OLD            "DEFAULT.VOT.OLD"   /* 旧 VOT 文件          */
#define RTS_RTN_FILE                "DEFAULT.RTN"       /* RTN 文件               */
#define RTS_RTN_FILE_TMP            "DEFAULT.RTN.TMP"   /* 临时 RTN 文件          */
#define RTS_RTN_FILE_OLD            "DEFAULT.RTN.OLD"   /* 旧 RTN 文件          */
#define RTS_OPC_FILE                "DEFAULT.OPC"       /* OPC 文件               */
#define RTS_OPC_FILE_TMP            "DEFAULT.OPC.TMP"   /* 临时 OPC 文件          */
#define RTS_OPC_FILE_OLD            "DEFAULT.OPC.OLD"   /* 旧 OPC 文件          */
#define RTS_CM_FILE                 "DEFAULT.CM"        /* CM 文件               */
#define RTS_CM_FILE_TMP             "DEFAULT.CM.TMP"    /* 临时 CM 文件          */
#define RTS_CM_FILE_OLD             "DEFAULT.CM.OLD"    /* 旧 CM 文件          */

#define FILE_NEW_DOWNLOAD           (0x01)
#define FILE_NOT_NEW_DOWNLOAD       (0x00)

#define FILE_NEW_CONFIGURED         (0x01)
#define FILE_NOT_NEW_CONFIGURED     (0x00)


/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)
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

/* 系统时间*/
typedef struct SysSystemTimeTag
{
    uint32_t uiSecond;      /*时间 s*/
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
    uint8_t ucModSN[14]; /* 模块SN号*/
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

typedef struct TaskInfoTag
{
    uint8_t ucTaskNum;
    uint8_t ucTask1State;
    uint8_t ucTask2State;
}TaskInfo_t;

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
    TaskInfo_t stTaskInfo;
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




typedef struct SysCtrlTag
{

}SysCtrl_t;

typedef struct SysDownLoadFileTag
{
    uint8_t ucFileDownloadFlg[NUM_OF_CFG_FILE_TYPE];
    uint8_t ucFileNewConfigFlg[NUM_OF_CFG_FILE_TYPE];
}SysDownLoadFile_t;


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
uint16_t SysGetPMState(PMController_t emPMID);

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
* 函数名称: SysGetSysState
*
* 功能描述: 获取系统状态
*
* 输入参数: 无
*
* 输出参数: pstSysState: 指向系统状态
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetSysState(SysState_t *pstSysState);

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
uint16_t SysGetCMState(CMController_t emCMID);

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
* 函数名称: SysSetCMSysState
*
* 功能描述: 设置CM的状态
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
bool_t SysSetCMSysState(SysCMState_t *pstCMSysState);

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
bool_t SysSetSysState(SysState_t *pstSysState);

/*
***********************************************************************************************************************
* 函数名称: SysGetCMBusCommState
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
uint16_t SysGetCMBusCommState(CMController_t emCMID, PMController_t emPMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetCMBusCommState
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
bool_t SysSetCMBusCommState(CMController_t emCMID, PMController_t emPMID, uint16_t usInterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalPMID
*
* 功能描述: 获取本地PM的ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地PM的ID
*
* 注意事项: 无
***********************************************************************************************************************
*/
//PMController_t SysGetLocalPMID(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMBusCommState
*
* 功能描述: 获取本PM与指定CM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*
* 输出参数: 无
*
* 返 回 值  : 本CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalCMBusCommState(CMController_t emCMID);

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalCMBusCommState
*
* 功能描述: 设置本PM与指定CM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalCMBusCommState(CMController_t emCMID, uint16_t usInterState);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMSysTime
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
SysSystemTime_t SysGetPMSysTime(void);

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
* 函数名称: SysSetCMLogicID
*
* 功能描述: 设置与当前通讯的CM的逻辑号
*
* 输入参数: emCMID逻辑号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCMLogicID(CMController_t emCMID);

/*
***********************************************************************************************************************
* 函数名称: SysCheckAllFile
*
* 功能描述: 校验所有文件的完整性。
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
uint32_t SysCheckAllFile(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetWorkStatus
*
* 功能描述: 获取PM工作状态（RUN、OFFLINE、STANDBY、STOP）。
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
//work_status_t SysGetWorkStatus(PMController_t emPMId );
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileAllTable
*
* 功能描述: 获取上传CM的完整配置文件表。
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
SysCfgFileTable_t* SysGetCfgFileAllTable(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileIncreTable
*
* 功能描述: 获取上传CM的增量配置文件表。
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
SysCfgFileTable_t* SysGetCfgFileIncreTable(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetFileName
*
* 功能描述: 根据文件类型ID获取文件名。
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
void SysGetFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[]);

/*
***********************************************************************************************************************
* 函数名称: SysGetTmpFileName
*
* 功能描述: 根据文件类型ID获取临时文件名。
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
void SysGetTmpFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[]);

/*
***********************************************************************************************************************
* 函数名称: SysGetOldFileName
*
* 功能描述: 根据文件类型ID获取老文件名。
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
void SysGetOldFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[]);

/*
***********************************************************************************************************************
* 函数名称: SysSetRemoteProjVersion
*
* 功能描述: 存储来自CM的工程版本信息
*
* 输入参数:  emCMID CM的ID号
*         pstProjVer 指向工程版本信息
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetRemoteProjVersion(CMController_t emCMID, ProjVerInfo_t *pstProjVer);


/*
***********************************************************************************************************************
* 函数名称: SysGetRemoteProjVersion
*
* 功能描述: 获取来自CM的工程版本信息
*
* 输入参数:  emCMID CM的ID号
*         pstProjVer 指向工程版本信息
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRemoteProjVersion(CMController_t emCMID, ProjVerInfo_t *pstProjVer);

/*
***********************************************************************************************************************
* 函数名称: SysSetFileDownloadFlg
*
* 功能描述: 设置文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  ucFileDownloadFlg：下装标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetFileDownloadFlg(SysCfgFileType_t emSysCfgFileType, uint8_t ucFileDownloadFlg);

/*
***********************************************************************************************************************
* 函数名称: SysGetFileDownloadFlg
*
* 功能描述: 获取文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  
*
* 输出参数: 
*
* 返 回 值  : 下装标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetFileDownloadFlg(SysCfgFileType_t emSysCfgFileType );

/*
***********************************************************************************************************************
* 函数名称: SysSetFileNewConfigFlg
*
* 功能描述: 设置文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  ucFileNewConfigFlg：新配置标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetFileNewConfigFlg(SysCfgFileType_t emSysCfgFileType, uint8_t ucFileNewConfigFlg);

/*
***********************************************************************************************************************
* 函数名称: SysGetFileNewConfigFlg
*
* 功能描述: 获取文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID
*
* 输出参数: 
*
* 返 回 值  : 新配置标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetFileNewConfigFlg(SysCfgFileType_t emSysCfgFileType );
/*
***********************************************************************************************************************
* 函数名称: SysSetCMVer
*
* 功能描述: 设置CM的版本号
*
* 输入参数: emCMID
*        uiCMVer 版本号
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
//void SysSetCMVer(CMController_t emCMID, uint32_t uiCMVer);

/*
***********************************************************************************************************************
* 函数名称: SysGetCMVer
*
* 功能描述: 获取CM的版本号
*
* 输入参数: puiCMVer 版本号
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMVer(CMController_t emCMID, puint32_t puiCMVer);

/*
***********************************************************************************************************************
* 函数名称: SysSetCMLEDState
*
* 功能描述: 设置CM的状态灯状态
*
* 输入参数: emCMID
*        uiCMLEDState
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
//void SysSetCMLEDState(CMController_t emCMID, uint32_t uiCMLEDState);

/*
***********************************************************************************************************************
* 函数名称: SysGetCMLEDState
*
* 功能描述: 获取CM的状态灯状态
*
* 输入参数: puiCMLEDState
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMLEDState(CMController_t emCMID, puint32_t puiCMLEDState);
#endif /* SYS_COMMON_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

