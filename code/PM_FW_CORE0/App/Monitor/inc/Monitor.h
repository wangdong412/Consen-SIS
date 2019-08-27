/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    :
*
*  文件名称    : Monitor.h
*
*  功能简介    :
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-28
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef MONITOR_H_
#define MONITOR_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Srv/SharedSys/inc/SysMonitor.h"
#include "APP/Main/inc/Task.h"
#include "Srv/Algrithm/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_OUT_MINITOR
#ifdef DEBUG_OUT_MINITOR
#define DEBUG_MONITOR(format, args...)  printf ("MONITOR:  ") ; printf(format, ##args)
#else
#define DEBUG_MONITOR(format, args...)
#endif


#define NEED_INIT                       (0x01)
#define NOT_NEED_INIT                   (0x02)

#ifdef CONTROL_CYCLE_20MS
#define CFG_MINIMUM_BEAT_TIME           (200U)
#endif

#ifdef CONTROL_CYCLE_5MS
#define CFG_MINIMUM_BEAT_TIME           (50)//liqi 5ms
#endif

#define CS1131_CHECK_EXTERLIB       (1001)      /* 检查外部库版本服务 */
#define CS1131_READ_PRJINFO         (1003)      /* 读取工程信息服务 */
#define CS1131_START_RUN            (1008)      /* 开始运行服务 */
#define CS1131_STOP_RUN             (1009)      /* 停止运行服务 */
#define CS1131_PAUSE_RUN            (1010)      /* 暂停运行服务 */
#define CS1131_WRITE_VAR            (1011)      /* 写变量服务 */
#define CS1131_DOWNLOAD_START       (1012)      /* 清空/增量下装服务 */
#define CS1131_SINGLE_STEP          (1013)      /* 单步运行服务 */
#define CS1131_FORCE_VAR            (1014)
#define CS1131_READ_FORCE           (1015)      /* 读强制列表服务 */
#define CS1131_RELEASE_ALL_FORCE    (1016)
#define CS1131_RELEASE_SOME_FORCE   (1017)
#define CS1131_DISABLE_VAR          (1018)
#define CS1131_START_OSC            (1033)      /* 启动示波器服务 */
#define CS1131_STOP_OSC             (1034)      /* 停止示波器服务 */
#define CS1131_NEED_OSC             (1035)      /* 示波器配置是否需要服务 */
#define CS1131_DOWNLOAD_COMFIR      (1039)      /* 清空/增量下装确认服务*/

/* Request Message Type：PM/CM Handle */
#define CS1131_WRITEFILE_START      (1021)      /* 写文件开始服务 */
#define CS1131_WRITEFILE_CONT       (1022)      /* 写文件继续服务 */
#define CS1131_WRITEFILE_END        (1023)      /* 写文件完成服务 */
#define CS1131_READFILE_START       (1025)      /* 读文件开始服务 */
#define CS1131_READFILE_CONT        (1026)      /* 读文件继续服务 */


/* OPC service ID */
/* Request Message Type：CM Handle */
#define OPC_LOGIN                   (2001)  /* 登录服务 */
#define OPC_LOGOUT                  (2002)  /* 退出登录服务 */
#define OPC_READ                    (2003)  /* 读变量服务 */
/* Request Message Type：PM Handle */
#define OPC_WRITE                   (2004)  /* 写变量服务*/

/* Second level Response Code-subReason; 0:Successfully*/
#define ERR_SUCCESS                 (0)
#define ERR_SRC_ADDR                (101)     /*SRC addr is error(NULL)*/
#define ERR_ACK_ADDR                (102)    /*ACK buffer addr is error*/
#define ERR_MSG_LEN                 (103)    /*MSG len is error*/
#define ERR_MSG_CRC                 (104)    /*MSG CRC is error*/
#define ERR_GET_PROINFO             (105)
#define ERR_GET_FORCELIST           (106)
#define ERR_WRITE_FILE_START        (107)
#define ERR_WRITE_FILE_CONTINUE     (108)
#define ERR_WRITE_FILE_END          (109)
#define ERR_READ_FILE_START         (110)
#define ERR_READ_FILE_CONTINUE      (111)
#define ERR_WRITE_VAR               (112)
#define ERR_FORCE_LIST              (113)
#define ERR_DISABLE_TABL_LEN        (114)
#define ERR_DISABLE_TABL_CRC        (115)
#define ERR_TASK_ID                 (116)
#define ERR_WRITE_VAR_ITERM         (117)
#define ERR_DISABLE_ITERM           (118)
#define ERR_WRTIE_VAR_TASK_STOP     (119)    /*写变量服务中有变量所属的任务处于STOP状态*/
#define ERR_OSCP_GET_VAR            (120)
#define ERR_OSCP_START              (121)
#define ERR_OSCP_DONE               (122)

#define ERR_OPC_WRITE_VAR           (126)

#define ERR_CTRL_PAUSE              (128)
#define ERR_CTRL_CONTINUE           (129)


/* Top level Response Code-ResCode 0 :Successfully; 255:Failed */
#define RESCODE_SUCCESS             (0U)
#define RESCODE_FAILED              (255U)


#define ACK_FRAME_MID               (0)
#define ACK_FRAME_FIRST             (1)
#define ACK_FRAME_LAST              (2)
#define ACK_FRAME_SINGLE            (3)


#define FORCE_TABLE                 (0x01)
#define WRITE_VAR                   (0x00)

#define HAS_FORCE_TABLE             (0x01)

#define MAX_WRITE_VAR_NUM           (64)
#define MAX_FORCE_ITEM_NUM          (64)

#define CHKSUM_CRC_SIZE             (sizeof(uint32_t))

/* Modbus */
#define MONITOR_MODBUS_MASTER       (0U)
#define MONITOR_MODBUS_SLAVE        (1U)

#define MONITOR_COIL_STATUS         (0U)
#define MONITOR_INPUT_STATUS        (1U)
#define MONITOR_INPUT_REGS          (2U)
#define MONITOR_HOLD_REGS           (3U)

#define MONITOR_WRITE_BUF_LEN       (2048U)


#define P2P_REQ_FRAME                (0)
#define P2P_RES_FRAME                (1)

#define ALLOW_IEC_RUN                (0U)
#define NOT_ALLOW_IEC_RUN            (1U)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

typedef void (*GlobaInit)(uint32_t);


#pragma pack (1)

typedef struct MSGHeadTag
{
    uint8_t ucType;                     /*0xAA*/
    uint8_t ucProtect;                  /*0xBB*/
    uint16_t usActualType;              /*Actually message type*/
    uint8_t ucNodeNum;                  /*control station node number*/
    uint8_t ucPMNum;
    uint16_t usSequenceNum;             /*request message sequence number*/
    uint8_t ucLastFlag;                 /*last package flag*/
    uint8_t ucReserved[5];
    uint16_t usLength;                  /*message data length (head + Data Area)*/
}MSGHead_t;

typedef struct ACKHeadTag
{
    uint8_t ucType;                     /*0xBB*/
    uint8_t ucProtect;                  /*0xAA*/
    uint16_t usActualType;              /*Actually message type*/
    uint8_t ucNodeNum;                  /*control station node number*/
    uint16_t usSequenceNum;             /*request message sequence number*/
    uint8_t ucFlag;                     /*用于指示应答的第一或最后一帧*/
                                        /*0：多消息应答的中间帧; 1：多消息应答的第一帧; 2：多消息应答的最后一帧; 3：单消息应答*/
    uint8_t ucResCode;                  /*响应码：0-请求已成功完成；255-请求失败，失败原因见subReason*/
    uint8_t ucSubReason;                /*请求失败的具体原因，详见本章响应码*/
    uint8_t ucDataFlg;                  /*是否存在应答数据：1-存在；0-不存在*/
    uint8_t ucReserved[3];
    uint16_t usLength;                  /*ACK data length (head + Data Area)*/
}ACKHead_t;

typedef struct WriteVarTag
{
    uint8_t ucTaskID;
    uint8_t ucRefID;
    uint16_t usSize;
    uint32_t uiOffSet;
}WriteVar_t;

typedef struct WriteVarArrayTag
{
    uint32_t uiReadIndex;
    uint32_t uiWriteIndex;
    uint8_t ucWriteVar[MONITOR_WRITE_BUF_LEN];
                               
}WriteVarArray_t;

typedef struct ForceItemTag
{
    uint8_t ucTaskID;
    uint8_t ucRefID;
    uint16_t usSize;
    uint32_t uiOffSet;
}ForceItem_t;

typedef struct ForceItemArrayTag
{
    uint32_t uiReadIndex;
    uint32_t uiWriteIndex;
    uint8_t ucForceItem[2*1024];
                               
}ForceItemArray_t;

typedef struct OPCWritVarHeadTag
{
    uint16_t usOPCWritVarNum;
    uint16_t usReserved;
}OPCWritVarHead_t;


/* Modbus主从通用头信息结构体定义 */
typedef struct CommModbusCommonHeadTag
{
    uint8_t ucPortType;           /* 端口类型 */
    uint8_t ucReserve;            /* 预留 */
    uint16_t usDataLen;           /* Modbus数据的长度（字节） */
}CommModbusCommonHead_t;

/* Modbus主站头信息结构体定义 */
typedef struct CommModbusMasterHeadTag
{
    CommModbusCommonHead_t stCommonHead;
    uint8_t ucCMSlotID;                      /* CM模块槽位号 */
    uint8_t ucPortID;                        /* 端口号 */
    uint8_t ucReserve1[2];                   /* 预留 */
    uint32_t uiBlockID;                      /* Modbus Master块号 */
    uint32_t uiProjID;                       /* 工程ID */
    uint32_t uiMainVer;                      /* 主版本 */
    uint32_t uiMinorVer;                     /* 次版本 */
    uint32_t uiProjCRC;                      /* 工程CRC */
}CommModbusMasterHead_t;

/* 端口Modbus从站头信息结构体定义 */
typedef struct CommModbusSlaveHeadTag
{
    CommModbusCommonHead_t stCommonHead;
    uint8_t ucReserve1[4];                   /* 预留 */
    uint32_t uiBlockID;                      /* Modbus Slave块号 */
    uint16_t usBeginAddr;                    /* 本数据帧中数据的起始地址 */
    uint16_t usElementNum;                   /* 线圈/寄存器数 */
    uint8_t ucReserve2[12];                  /* 预留 */
}CommModbusSlaveHead_t;

typedef struct P2PRecvReqHeadTag
{
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucLinkID;  /* Link ID  链路ID: 1-链路1；2-链路2。只对同CM双网口冗余有效 */
    uint16_t usTotalLen;
    uint8_t ucPMInfo;
    uint8_t ucReserve1;
}P2PRecvReqHead_t;

typedef struct P2PSendRspHeadTag
{
    uint8_t ucSrcCMID;
    uint8_t ucSrcNETID;
    uint8_t ucReserve1;
    uint8_t ucReserve2;
    uint32_t uiDstIP1Addr;
    uint32_t uiDstIP2Addr;
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve3;
    uint32_t uiTotalLen;
}P2PSendRspHead_t;

#pragma pack ()

typedef struct P2PRecvReqCtrlTag
{
    bool_t bHasRecv;        /* 已接收标志位 */
    uint8_t ucReserve[3];   /* 预留 */
    uint32_t uiIntrCycCnt;  /* 接收间隔周期计数 */
}P2PRecvReqCtrl_t;


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: MonitorInit
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
void MonitorInit(void);

/*
***********************************************************************************************************************
* 函数名称: MonitorCycle
*
* 功能描述: 周期运行函数
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
void MonitorCycle(void);

#endif /* MONITOR_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

