/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 工程配置模块
*
*  文件名称    : CMconfig.h
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
#ifndef CMCONFIG_H_
#define CMCONFIG_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "DataType.h"
#include "SysCommon.h"
#include "SysCfgFile.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define MAX_CM_NUM                          (4U)//4
#define MAX_PORT_NUM                        (6U)//6
#define MAX_COMPORT_NUM                     (4U)//4
#define MAX_NETPORT_NUM                     (2U)//2
#define MAX_ACCESS_CTRL_NUM                 (16U)

#define MAX_MODBUS_VAR_NUM                  (100U)
#define MAX_MODBUS_RGN_VAR_NUM              (10000U)//10000          /* Added：每个Modbus数据区最多包含9999个变量 */
#define MAX_MODBUS_BLOCK_NUM                (64U)//64           
#define MAX_MODBUS_AREA_NUM                 (4U)//4         
#define MAX_MODBUS_SLAVE_NUM                (64U)            

#define MAX_OPC_NUM                         (10000U)//10000          
#define MAX_PASSWORD_LEN                    (16U)

#define NOT_UPDATE                          (bool_t)(0)       /*配置信息没有更新*/
#define UPDATE                              (bool_t)(1)       /*配置信息有更新*/


#define HANDLE_OK                           (int8_t)(1)
#define HANDLE_FAIL                         (int8_t)(-1)

#define MAX_SECTION_NAME_LEN                (8U)

#define PRG_SECTION_ID_SEGMENT_USED         (0x0001)          /*区已用大小段*/
#define PRG_SECTION_ID_PROGRAM              (0x0002)          /*程序代码段*/
#define PRG_SECTION_ID_TASK                 (0x0003)          /*任务段*/
#define PRG_SECTION_ID_IO_CONFIG            (0x0004)          /*硬件配置段*/
#define PRG_SECTION_ID_EXTFUN               (0x0005)          /*外部函数段*/
#define PRG_SECTION_ID_PROJECT_INFO         (0x0006)          /*工程信息段*/
#define PRG_SECTION_ID_PEERTOPEER           (0x0007)          /*PEER TO PEER段*/
#define PRG_SECTION_ID_SOE                  (0x0008)          /*SOE段*/
#define PRG_SECTION_ID_VOTE                 (0x0009)          /*表决表段*/
#define PRG_SECTION_ID_RETAIN               (0x000a)          /*掉电包含表段*/
#define PRG_SECTION_ID_OPC                  (0x000b)          /*OPC变量对照表段*/
#define PRG_SECTION_ID_MODBUSSLAVE          (0x000c)          /*MODBUS从段*/
#define PRG_SECTION_ID_MODBUSMASTER         (0x000d)          /*MODBUS主段*/
#define PRG_SECTION_ID_SUBFILE_CRC          (0x000e)          /*分文件CRC段*/
#define PRG_SECTION_ID_STATION_CONFIG       (0x000f)          /*控制站配置段*/
#define PRG_SECTION_NAME_TASKDATAEXCHANGE   (0x0010)          /*任务间数据交换段*/
#define PRG_SECTION_ID_CM_CONFIG            (0x0011)          /*CM模块配置段*/
#define PRG_SECTION_ID_VAR_LOAD_RELOC       (0x0012)          /*变量加载时重定位区*/
#define PRG_SECTION_ID_FUNC_LOAD_RELOC      (0x0013)          /*函数加载时重定位区*/
#define PRG_SECTION_ID_IMPORT_LOAD_RELOC    (0x0014)          /*导入函数加载时重定位区*/

#define SECTION_CONTENT_SAME                (0x01)
#define SECTION_CONTENT_NOTSAME             (0x00)


#define MAX_TASK_EXVAR_NUM                  (500U)            /*已确定*/

#define MAX_PASSWOED_NUM                    (16U)             /*待定*/

#define MAX_NAME_LEN                        (32U)
#define MAX_DESC_LEN                        (32U)

#define INVALID_CM_SLOT_NUM                 (0xffffffff)      /*无效的槽位号，在CM槽位表内未查到*/


/*工程没有被配置过，不存在配置信息*/
#define NOT_HAS_BE_CONFIGURED               (0x00U)
/*工程被配置过，存在配置信息*/
#define HAS_BE_CONFIGURED                   (0x01U)

/*工程没有被确认*/
#define NOT_HAS_BE_CONFIRMED                (0x00U)
/*工程被确认*/
#define HAS_BE_CONFIRMED                    (0x01U)

/*工程没有被配置过，不存在配置信息*/
#define IO_NOT_HAS_BE_CONFIGURED            (0x00U)
/*工程被配置过，存在配置信息*/
#define IO_HAS_BE_CONFIGURED                (0x01U)

#define FILE_HAS_CONFIG                     (0x00000000)                /*文件已经被解析配置过*/
#define FILE_NOT_CONFIG                     (0x000000aa)                /*文件没有被解析配置过*/
#define FILE_CONFIG_FLG_OFF                 (0x00000000)                /*文件是否被配置过的标志在文件存储区的偏移*/

#define MODBUS_VAR_ACTIVE                   (1U)
#define MODBUS_VAR_NOT_ACTIVE               (0U)

/* Serial Port Protocol */
#define SPP_NONE                            (0U)
#define MODBUS_MASTER_RTU                   (1U)
#define MODBUS_SLAVE_RTU                    (2U)
#define MODBUS_MASTER_ASCII                 (3U)
#define MODBUS_SLAVE_ASCII                  (4U)
#define SPP_GPS                             (5U)
#define SPP_PULSE_SRC                       (6U)
#define SPP_PULSE_RPT                       (7U)

/* Baud Rate */
#define BR1200                              (0U)
#define BR2400                              (1U)
#define BR4800                              (2U)
#define BR9600                              (3U)
#define BR19200                             (4U)
#define BR38400                             (5U)
#define BR57600                             (6U)
#define BR115200                            (7U)

/* Work Mode */
#define TM_RS485                            (0U)
#define TM_RS422                            (1U)

/* Data Bits */
#define DB7                                 (7U)
#define DB8                                 (8U)

/* Stop Bits */
#define SB1                                 (1U)
#define SB2                                 (2U)

/* Parity */
#define PARITY_NONE                         (0U)
#define PARITY_ODD                          (1U)
#define PARITY_EVEN                         (2U)
#define PARITY_MARK                         (3U)
#define PARITY_SPACE                        (4U)

/* Handshake */
#define HS_NONE                             (0U)
#define HS_HARDWARE                         (1U)

/* TermiOptions */
#define TO_NONE                             (0U)
#define TO_RESISTOR                         (1U)
#define TO_PULLUP                           (2U)
#define TO_DOWN                             (3U)
#define TO_ALL                              (4U)

/* FP Order */
#define FP_ORDER_H                          (0U)  /*0: High 16 Bits First*/
#define FP_ORDER_L                          (1U)  /*1: Low 16 Bits First*/
#define FP_ORDER_INVALID                    (2U)

/* NET ID */
#define NET_ID_1                            (1U)
#define NET_ID_2                            (2U)

/* Modbus TCP Protocol */
#define MODBUS_TCP_NONE                     (0U)
#define MODBUS_TCP_MASTER                   (1U)
#define MODBUS_TCP_SLAVE                    (2U)
#define MODBUS_TCP_ALL                      (3U)

/* Access Level */
#define AL_DENY                             (0U)
#define AL_READ_ONLY                        (1U)
#define AL_ALL                              (2U)

/* Network: 0-NET1 / 1-NET2 / 2- NET1 & NET2 */
#define NW_NONE                             (0U)
#define NW_NET1                             (1U)
#define NW_NET2                             (2U)
#define NW_NET_ALL                          (3U)

/* Visit Mode */
#define VM_READ                             (0U)  /* Read  读 */
#define VM_WRITE                            (1U)  /* Write  写 */

/* Modbus Region  Modbus数据区定义 */
#define MR_COIL                             (0U)  /* Coil  线圈 */
#define MR_DISCRETE_INPUTS                  (1U)  /* Discrete Inputs  离散量输入*/
#define MR_INPUT_REGS                       (2U)  /* Input Registers  输入寄存器 */
#define MR_HOLD_REGS                        (3U)  /* Holding Registers  保持寄存器 */

/* Permission  读写权限定义 */
#define PMSN_READONLY                       (0U)  /* Read only  只读 */
#define PMSN_READWRITE                      (1U)  /* Read/Write 可读写 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
typedef enum
{
    NOT_ACTIVE = 0x00000000,    /*配置块未使用*/
    ACTIVE = 0x00aa00bb         /*配置块被使用*/
}ActiveFlag_t;

typedef enum
{
    CS1131_DATA = 0x00,
    HMI_DATA,
    MODBUS_DATA,
    P2P_DATA,
    SOE_DATA,
    AMS_DATA,
    DIAG_DATA,
    INTER_CMD_DATA,
    TIMING_DATA,
    CLIENT_DATA
}Service_Type_t;

typedef enum
{
    TASK_UP1 = 0,                       /*UP1任务ID*/
    TASK_UP2,                             /*UP2任务ID*/
    MAX_TASK_NUM                         /*最大任务个数*/
}Task_ID_t;

typedef enum
{
    STATION_CONFIG = 0x00000001,        /*控制站配置文件*/
    PRG,                                /*PRG文件*/
    IO_CONFIG,                          /*IO配置文件(硬件配置文件)*/
    SOE_CONFIG,                         /*SOE配置文件*/
    PEERTOPEER_CONFIG,                  /*Peer To Peer 配置文件*/
    MODBUS_CONFIG,                      /*Modbus配置文件*/
    VOTE_CONFIG,                        /*VOTE 配置文件*/
    RETA_CONFIG,                        /*RETAIN 配置文件*/
    OPC_CONFIG,                         /*OPC 配置文件*/
    MAX_FILE_NUM                        /*最大文件个数*/
}File_ID_t;


#pragma pack (1)

/*OPC变量结构体定义*/
typedef struct OPCVarTag
{
    uint8_t ucTaskID;
    uint8_t ucRefID;
    uint16_t usVarSize;
    uint32_t uiOffset;
}OPCVar_t;/*8 byte*/

/*OPC信息结构体定义*/
typedef struct OPCVarInfoTag
{
    ActiveFlag_t emActive;
    uint32_t uiCRC;
    uint32_t uiFileVern;
    uint32_t uiVarNum;
    OPCVar_t stOPCVar[MAX_OPC_NUM];
}OPCVarInfo_t;/*80016 byte*/


/*Modbus Slave站块信息头结构体定义*/

typedef struct ModbusSlaveBlockHeadTag
{
    uint32_t uiBlockID;
    uint8_t ucModbusRgn;           /* Changed: 线圈、离散量输入、输入寄存器、保持寄存器 */
    uint8_t ucPermission;          /* Changed: 可读写 / 只读 */
    uint8_t ucReserve[2];
    uint32_t uiBeginAddr;
    uint32_t uiEndAddr;
    uint32_t uiModbusVarNum;
}ModbusSlaveBlockHead_t;/*20 byte*/



/*Modbus变量结构体定义*/
typedef struct ModbusVarTag
{
    uint8_t ucTaskID;
    uint8_t ucRefID;
    uint16_t usVarSize;
    uint32_t uiOffset;
    uint32_t uiModbusIndex;
}ModbusVar_t;/*12 byte*/

/*Modbus从变量结构体定义*/
typedef struct ModbusSlaveVarTag
{
    uint8_t ucActive;
    ModbusVar_t stModbusVar;
    
}ModbusSlaveVar_t;/*12 byte*/

/*Modbus Slave站块结构体定义*/
typedef struct ModbusSlaveBlockTag
{
    ActiveFlag_t emActive;              /*是否有有效配置信息*/
    ModbusSlaveBlockHead_t stModbusSlaveBlockHead;
    ModbusSlaveVar_t stModbusSlaveVar[MAX_MODBUS_RGN_VAR_NUM];
}ModbusSlaveBlock_t;/*24 byte + MAX_MODBUS_RGN_VAR_NUM * 12byte  */

/*Modbus Slave站配置信息结构体定义*/
typedef struct ModbusSlaveInfoTag
{
    uint32_t uiCRC;
    uint32_t uiFileVern;
    uint32_t uiBlockNum;
    ModbusSlaveBlock_t stModbusSlaveBlockInfo[MAX_MODBUS_AREA_NUM];
}ModbusSlaveInfo_t;/*12 byte + 120024 byte*4  */




/*CM上每个端口从站增减信息信息结构体定义*/
typedef struct ModbusSlaveChangeTag
{
    uint32_t uiAddTCPStationNum;                     /*与上次下装对比新增的TCP从站个数*/
    uint32_t uiDelTCPStationNum;                     /*与上次下装对比减少的TCP从站个数*/
    uint32_t uiAddStaionIP[MAX_MODBUS_SLAVE_NUM];   /*与上次下装对比，本次下装新增从站的IP地址*/
    uint32_t uiDelStaionIP[MAX_MODBUS_SLAVE_NUM];   /*与上次下装对比，本次下装减少的从站的IP地址*/

}ModbusSlaveChange_t;/*504 byte*/


/*Modbus master站块信息头结构体定义*/
typedef struct ModbusMasterBlockHeadTag
{
    uint8_t ucVisitMode;            /* Changed: 读 / 写 */
    uint8_t ucModbusRgn;            /* Changed: 线圈、离散量输入、输入寄存器、保持寄存器 */
    uint32_t uiTargetStationID;
    uint32_t uiTargetIP;
    uint32_t uiTargetBeginAddr;
    uint32_t uiTargetSize;
    uint32_t uiModbusVarNum;
}ModbusMasterBlockHead_t;/*28 byte*/

/*Modbus master配置文件块头部信息*/
typedef struct ModbusMasterFileBlockHeadTag
{
    uint32_t uiBlockID;
    uint8_t ucVisitMode;            /* Changed: 读 / 写 */
    uint8_t ucModbusRgn;            /* Changed: 线圈、离散量输入、输入寄存器、保持寄存器 */
    uint8_t ucReserve[2];
    uint32_t uiTargetStationID;
    uint32_t uiTargetIP;
    uint32_t uiTargetBeginAddr;
    uint32_t uiTargetSize;
    uint32_t uiModbusVarNum;
}ModbusMasterFileBlockHead_t;/*28 byte*/

/*Modbus Master站块结构体定义*/
typedef struct ModbusMasterBlockTag
{
    ActiveFlag_t emActive;              /*是否有有效配置信息*/
    ModbusMasterBlockHead_t stModbusMasterBlockHead;
    ModbusVar_t stModbusVar[MAX_MODBUS_VAR_NUM];
}ModbusMasterBlock_t;/*4+28+100*12 byte*/

/*CM上每个端口Modbus Master站配置信息结构体定义*/
typedef struct ModbusMasterPortTag
{
    ActiveFlag_t emActive;             /*是否有有效配置信息*/
    bool_t bUpdate;
    uint8_t ucReserved[3];
    ModbusSlaveChange_t stModbusSlaveChange;
    uint32_t uiBlockNum;
    uint8_t ucModbusMasterBlockID[MAX_MODBUS_BLOCK_NUM];
}ModbusMasterPort_t;

/*每个CM上Modbus Master站配置信息结构体定义*/
typedef struct ModbusMasterConfigTag
{
    ActiveFlag_t emActive;         /*是否有有效配置信息*/
    uint16_t usCMSlotID;
    ModbusMasterPort_t stModbusMasterPort[MAX_PORT_NUM];
}ModbusMasterConfig_t;

/*Modbus Master站配置信息结构体定义*/
typedef struct ModbusMasterInfoTag
{
    uint32_t uiCRC;
    uint32_t uiFileVern;
    ModbusMasterConfig_t stModbusMasterConfig[MAX_CM_NUM];
    ModbusMasterBlock_t stModbusMasterBlock[MAX_MODBUS_BLOCK_NUM];
}ModbusMasterInfo_t;





/*CM端口配置信息结构*/
typedef struct COMConfigInfoTag
{
    ActiveFlag_t emActive;
    bool_t bUpdate;
    uint8_t ucPortID;
    uint8_t ucProtocol;                 /*0-None；1-Modbus Master RTU；2-Modbus Slave RTU；3-Modbus Master ASCII；4-Modbus Slave ASCII；5-GPS-只COM4支持*/
    bool_t bPortWriteEnable;            /*协议为Modbus Slave RTU/ASCII时可配，默认值0(Disable)。*/
    uint8_t ucModbusSlaveAddr;          /*协议为Modbus Slave RTU/ASCII时可配，默认值1，范围1-247。*/
    uint8_t ucBaudRate;                 /*0:1200,1:2400,2:4800,3:9600,4:19200,5:38400,6:57600,7:115200，默认值为3 (9600)。*/
    uint8_t ucDataBits;                 /*Modbus Slave ASCII: 7/8，默认值8；其他：固定为8。*/
    uint8_t ucStopBits;                 /*可选值为1或者2，默认值1*/
    uint8_t ucParity;                   /*0:None,1:Odd,2:Even,3:Mark,4:Space，默认值为1 (Odd)。*/
    uint8_t ucTransMode;                /*只读，固定为RS-485，默认值0*/
    uint8_t ucFPOrder;                  /*0:High 16 Bits First，1:Low 16 Bits First,默认值1 (Low 16 Bits First)*/
    uint8_t ucMasterLogicalPort;        /*只有当协议为Modbus Master时，可配，默认值1，范围1-32*/
    int16_t sModbusMin;                 /*Min: 默认值0，范围-32768-32767*/
    int16_t sModbusMax;                 /*Max: 默认值32767，范围-32768-32767*/
    uint8_t ucReserve1[4];              /*保留*/

}COMConfigInfo_t;



/*CM网口配置信息结构*/
typedef struct NetConfigInfoTag
{
    ActiveFlag_t emActive;
    bool_t bUpdate;
    uint8_t ucNetID;                    /*1：使用NET1通讯；2：使用NET2通讯*/
    bool_t bPortWriteEnable;            /*涉及OPC和HMI软件，可配，默认不允许写(0)*/
    uint8_t ucReserved1;                /*保留*/
    uint32_t uiIPAddr;                  /*端口IP地址：NET 1默认值：192.168.1.0；NET 2默认值：192.168.1.1。*/
    uint32_t uiSubnetMask;              /*子网掩码，默认值：255.255.255.0*/
    uint32_t uiGateway;                 /*默认网关，默认值：0.0.0.0 典型地，此参数为网络路由器的IP地址*/
    uint8_t ucReserved2[4];             /*保留*/
}NetConfigInfo_t;



/*CM网口ModbusTCP配置信息结构*/
typedef struct ModbusTCPConfigInfoTag
{
    ActiveFlag_t emActive;
    bool_t bUpdate;
    uint8_t ucNetID;                    /*1：使用NET1通讯；2：使用NET2通讯*/
    uint8_t ucProtocol;                 /*1-Modbus TCP Master Net；2-Modbus TCP Slave Net；3- Modbus TCP Master & Slave Net (即同时支持两种协议)。*/
    bool_t bPortWriteEnable;            /*仅当协议为Modbus TCP Slave Net时可配，默认不勾选(0)，即不可写。*/
    uint8_t ucMasterLogicPort;          /*仅当协议为Modbus TCP Master Net时，可配，范围1-32，默认值为1。对于系统内安装的每个TCM，此参数必须是唯一的。*/
    uint8_t ucFPOrder;                  /*0：High 16 Bits First; 1：Low 16 Bits First*/
    uint16_t usTCPPortNum;              /*只读，固定为502*/
    uint32_t uiIPAddr;                  /*当协议为Modbus TCP Slave Net时，此参数意义如下：*/
                                        /*接受来自任何Modbus Master的通讯：0.0.0.0;接受某一指定Modbus Master的通讯：特定Master的IP地址。*/
    int16_t sModbusMin;                 /*仅当协议为Modbus TCP Slave Net时，可配，默认值：0范围：-32768-32767。*/
    int16_t sModbusMax;                 /*仅当协议为Modbus TCP Slave Net时，可配，默认值：32767范围：-32768-32767。*/
    uint8_t uiReserved[4];              /*保留*/
}ModbusTCPConfigInfo_t;

/*CM校时配置信息结构*/
typedef struct NTPTCPConfigInfoTag
{
    ActiveFlag_t emActive;
    bool_t bUpdate;
    uint8_t ucTimingSrc;                /*时钟更新源，0：SNTP/NTP，1：GPS；2：功能块*/
    uint8_t ucPulsePolarity;            /*脉冲极性，0: positive,1: negative*/
    uint8_t ucIsMaster;                 /*作SNTP/NTP主站,0:false,1:true*/
    uint32_t uiMasterIPAddr;            /*校时服务器IP地址*/
}NTPTCPConfigInfo_t;



/*CM访问控制配置信息结构*/
typedef struct AccessCtrlConfigTag
{
    bool_t bUpdate;
    uint8_t ucNetwork;                  /*网口号：0-NET1 / 1-NET2 / 2- NET1 & NET2*/
    uint8_t ucCS1131;                   /*CS1131访问级别：0-Deny / 1-Read Only / 2-All*/
    uint8_t ucOPC;                      /*OPC访问级别：0-Deny / 1-Read Only / 2-All*/
    uint8_t ucSOE;                      /*SOE访问级别：0-Deny / 1-Read Only / 2-All*/
    uint8_t ucAMS;                      /*AMS访问级别：0-Deny / 1-Read Only / 2-All*/
    uint8_t ucClient;                   /*Client访问级别：0-Deny / 1-Read Only / 2-All*/
    uint8_t ucReserved1;                /*保留*/
    uint32_t uiClientIPAddr;            /*IP地址*/
    uint32_t uiSubnetMask;              /*Mask子网掩码*/
    uint8_t ucReserved2[4];             /*保留*/

}AccessCtrlConfig_t;

/*CM访问控制配置信息head结构*/
typedef struct AccessCtrlHeadTag
{
    bool_t bUpdate;
    uint8_t ucAccessCtrlNum;
    uint8_t ucReserved[2];
}AccessCtrlHead_t;

/*CM访问控制配置信息结构*/
typedef struct AccessCtrlConfigInfoTag
{
    AccessCtrlHead_t stAccessCtrlHead;
    AccessCtrlConfig_t stAccessCtrlConfig[MAX_ACCESS_CTRL_NUM];
}AccessCtrlConfigInfo_t;

/*CM配置信息结构*/
typedef struct CMSingleInfoTag
{
    ActiveFlag_t emActive;
    uint32_t uiCRC;
    uint32_t uiFileVern;
    uint16_t usSlotID;
    uint32_t uiStationID;
    uint8_t ucPassWordLen;
    uint8_t ucPassWord[MAX_PASSWORD_LEN];
    uint8_t ucReserve;
    COMConfigInfo_t stCOMConfigInfo[MAX_COMPORT_NUM];
    NetConfigInfo_t stNetConfigInfo[MAX_NETPORT_NUM];
    ModbusTCPConfigInfo_t stModbusTCPConfigInfo[MAX_NETPORT_NUM];
    NTPTCPConfigInfo_t stNTPTCPConfigInfo;
    AccessCtrlConfigInfo_t stAccessCtrlConfigInfo;
}CMSingleInfo_t;

typedef struct CMConfigInfoTag
{
    CMSingleInfo_t stCMSingleInfo[NUM_OF_CM];
}CMConfigInfo_t;



typedef struct SubFileInfoTag
{
    uint32_t uiFileCRC[NUM_OF_CFG_FILE_TYPE];
    uint32_t uiFileSerialNum[NUM_OF_CFG_FILE_TYPE];
}SubFileInfo_t;
/*88B*/

/* ProjectConfigInfo struct  Project配置信息结构体定义 */
typedef struct PrjConfigInfoTag
{
    uint32_t uiProjectID;
    uint32_t uiMajorVersion;
    uint32_t uiMinorVersion;
    uint32_t uiLastSaveDate;
    char_t cProjectName[MAX_NAME_LEN];
    char_t cDesctipiton[MAX_DESC_LEN];
}PrjConfigInfo_t;
/*80B*/

/* TaskConfigInfo struct  UP_Task信息结构体定义 */
typedef struct TaskConfigInfoTag
{
    ActiveFlag_t emActiveFlag;
    uint32_t uiTaskID;
    uint32_t uiControlFlag;         /*bit0: Control/Safety;  bit1: 是否异常处理*/
    uint32_t uiPriority;
    uint32_t uiIECPeriod;
    uint32_t uiTolerate;            /*任务级容忍*/
    uint32_t uiTaskCodeEntry;
    uint32_t uiGlobalInitEntry;     /*任务的数据区初始化函数入口，相对于PRG文件的Program段的Machine Code的偏移。*/
    uint32_t uiGOffset;             /*G区的起始地址，相对于数据区首地址的偏移*/
    uint32_t uiGUsedSize;           /*G区的已使用大小*/
    uint32_t uiIOffset;             /*G区的起始地址，相对于数据区首地址的偏移*/
    uint32_t uiIUsedSize;           /*G区的已使用大小*/
    uint32_t uiOOffset;             /*G区的起始地址，相对于数据区首地址的偏移*/
    uint32_t uiOUsedSize;           /*G区的已使用大小*/
}TaskConfigInfo_t;
/*56B*/

/* Task Data Exchang Var struct  任务间数据交换变量信息结构体定义 */
typedef struct TaskDataExVarTag
{
    uint16_t usSize;                /*交换变量的大小，如果为0，表示BIT类型变量*/
    uint8_t ucSrcTaskID;            /*源变量所在任务ID*/
    uint8_t ucSrcRefID;             /*源变量的RefId*/
    uint32_t uiSrcOffset;           /*源变量的Offset*/
    uint8_t ucSrcBitPos;            /*源变量的Bit Position，非BIT类型变量置0*/
    uint8_t ucDestTaskID;           /*目标变量所在任务ID*/
    uint8_t ucDestRefId;            /*目标变量的RefId*/
    uint32_t uiDestOffset;          /*目标变量的Offset*/
    uint8_t ucDestBitPos;           /*目标变量的Bit Position，非BIT类型变量置0*/
}TaskDataExVar_t;
/*16B*/

/* Task Data Exchang Info struct  任务间数据交换信息结构体定义 */
typedef struct TaskDataExInfoTag
{
    uint32_t uiUP1ToUP2VarNum;
    uint32_t uiUP2ToUP1VarNum;
    TaskDataExVar_t stTaskDataExVar[MAX_TASK_EXVAR_NUM];
}TaskDataExInfo_t;

/* Station Config Info struct  站级配置信息结构体定义 */
typedef struct StationConfigInfoTag
{

    uint32_t uiStationID;                        /*站号*/
    uint8_t ucObjPasswordLen;                   /*工程密码长度*/
    char_t ucObjPassword[MAX_PASSWOED_NUM];     /*工程密码*/
    uint32_t uiTolerate;                        /*容忍位*/

}StationConfigInfo_t;

/*Control Station ConfigInfo 控制站配置信息*/
typedef struct CtrlStationConfigInfoTag
{
    uint32_t uiCRC;
    uint32_t uiFileVern;
    uint8_t ucIsConfigured;
    uint8_t ucIsComfirmed;
    uint8_t ucIsIOConfigured;
    uint8_t ucReserved;
    SubFileInfo_t stSubFileInfo;
    PrjConfigInfo_t stPrjConfigInfo;
    TaskConfigInfo_t stTaskConfigInfo[MAX_TASK_NUM];
    TaskDataExInfo_t stTaskDataExInfo;
    StationConfigInfo_t stStationConfigInfo;

}CtrlStationConfigInfo_t;



typedef struct FileHeadTag
{
    uint32_t uiCheckSum;
    uint32_t uiSize;
    uint32_t uiVersion;
    uint32_t uiFileID;
    uint32_t uiSectionNum;
    uint32_t uiTimeDateStamp;
    uint32_t uiStringTableOffset;           /*字符串表在文件中的偏移(相对于文件首地址) */
    uint32_t uiFileSeriaNum;                /*文件流水号，仅在每次文件内容变化后该值加1*/
    uint8_t uiReserved[16];
}FileHead_t;

typedef struct SectionTableTag
{
    uint8_t ucName[MAX_SECTION_NAME_LEN];
    uint32_t uiSectionId;
    uint32_t uiOffset;
    uint32_t uiSize;
    uint32_t uiMsg;
    uint8_t ucSameWithLastDownload;
    uint8_t ucReserved1[3];
    uint8_t ucReserved2[4];
}SectionTable_t;

typedef struct CMSlotIdTablTag
{
    uint32_t uiCMNum;
    uint32_t uiEffectiveFlg[MAX_CM_NUM];
    uint32_t uiCMSlotId[MAX_CM_NUM];

}CMSlotIdTabl_t;



typedef enum
{
    NOT_DOWNLOAD = 0x00000000,      /*当前无下装*/
    INIT_DOWNLOAD,                  /*当前正在进行清空下装*/
    ONLINE_DOWNLOAD,                /*当前正在进行在线下装*/
}download_flag_t;

typedef enum
{
    NOT_CONFIG = 0x00000000,        /*当前没有进行过配置*/
    HAS_CONFIG,                     /*当前有完整的配置信息*/
}config_flag_t;



#pragma pack ()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/


uint8_t SysGetRecvCFGFileFlg(void);    /*获取内部通讯接收CM配置信息是否完成标志*/







/*
***********************************************************************************************************************
* 函数名称: CMCfgInit
*
* 功能描述: 配置信息结构初始化
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
void CMCfgInit(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetCtrlStationInfo
*
* 功能描述: 获取控制站配置信息指针
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
CtrlStationConfigInfo_t* CMCfgGetCtrlStationInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetModbusMasterInfo
*
* 功能描述: 获取Modbus主配置信息指针
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
ModbusMasterInfo_t* CMCfgGetModbusMasterInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetModbusSlaveInfo
*
* 功能描述: 获取Modbus从配置信息指针
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
ModbusSlaveInfo_t* CMCfgGetModbusSlaveInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetOPCVarInfo
*
* 功能描述: 获取OPC配置信息指针
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
OPCVarInfo_t* CMCfgGetOPCVarInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetCMConfigInfo
*
* 功能描述: 获取CM配置信息指针
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
CMConfigInfo_t* CMCfgGetCMConfigInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CMCfgGetCMSlotIdTabl
*
* 功能描述: 获取CM槽位号与CM ID对照表信息指针
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
CMSlotIdTabl_t* CMCfgGetCMSlotIdTabl(void);

#endif /*CMCONFIG_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
