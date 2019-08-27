/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : GPS校时模块
*
*  文件名称    : GPS.h
*
*  功能简介    : GPS校时
*
*  作者        : 王东
*
*  创建日期    : 2016-11-23
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef GPS_H_
#define GPS_H_

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

#define DEBUG_DBGPS

#define DEBUG_OUT_GPS

#ifdef DEBUG_OUT_GPS
#define DEBUG_GPS(format, args...)  printf ("DB_GPS: ") ; printf(format, ##args)
#else
#define DEBUG_GPS(format, args...)
#endif

/***************************** GPS Timing Frame *****************************/
#define MAX_GPSTF_LEN        (256)    /* GPS校时包的最大长度：如不考虑"额外"字节，则最大长度为78B（0x8F-0B） */
#define MIN_GPSTF_LEN        (4)      /* GPS校时包的最小长度：<DLE><id><data string bytes><DLE><ETX> */

#define GPSTF_DLE            (0x10U)  /* DLE: 0x10 */
#define GPSTF_ETX            (0x03U)  /* ETX: 0x03 */

/* GPS Timing Frame: ID */
#define GPSTF_ID             (0x8FU)  /* GPS校时包的ID */

/* GPS Timing Frame: Selected Subcode */
#define GPSTF_SEL_SUBCODE    (0xADU)  /* 选用的GPS校时包的Subcode */

/* GPS Timing Frame: Data string bytes Length */
#define GPSTF_DATALEN_AD     (22U)    /* GPS校时包0x8F-AD */

/* UTC Leap Second Flag UTC闰秒标志 */
#define LEAP_UTC_FLAG      (0x01 << 0)  /* 1: UTC Time is available */
#define LEAP_SCHED         (0x01 << 4)  /* 1: GPS Almanac’s leap second date is not in the past */
#define LEAP_PENDING       (0x01 << 5)  /* 1: 24-hour warning. Cleared before leap second */
#define LEAP_WARNING       (0x01 << 6)  /* 1: Set ± 6 hours before/after leap event */
#define LEAP_IN_PROGRESS   (0x01 << 7)  /* 1: Leap second is now being inserted */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Receiver Tracking Status 接收器跟踪状态枚举定义 */
typedef enum RecvStatusTag
{
    RECV_DOING_FIXES = 0,        /* Receiver is navigating                  接收器正在导航 */
    RECV_GOOD_1SV = 1,           /* Receiver is timing using one satellite  接收器正在使用一个卫星进行校时 */
    RECV_APPX_1SV = 2,           /* Approximate time                        大致时间 */
    RECV_NEED_TIME = 3,          /* Start-up                                启动 */
    RECV_NEED_INITALIZATION = 4, /* Start-up                                启动(需初始化) */
    RECV_PDOP_HIGH = 5,          /* Dilution of Precision too High          精度稀释太高 */
    RECV_BAD_1SV = 6,            /* Satellite is unusable                   卫星不可用 */
    RECV_0SVS = 7,               /* No satellites usable                    无卫星可用 */
    RECV_1SV = 8,                /* Only 1 satellite usable                 只有1卫星可用 */
    RECV_2SVS = 9,               /* Only 2 satellites usable                只有2卫星可用 */
    RECV_3SVS = 10,              /* Only 3 satellites usable                只有3卫星可用 */
    RECV_NO_INTEGRITY = 11,      /* Invalid solution                        无效解决方案 */
    RECV_DCORR_GEN = 12,         /* Differential corrections                差分校正 */
    RECV_OVERDET_CLK = 13        /* Over-determined fixes                   超量修正 */
}RecvStatus_t;

#pragma pack(1)

/* Primary UTC Time Frame Data String Bytes(0x8F-AD)  GPS校时包0x8F-AD数据字节信息结构体定义 */
typedef struct PrimUTCTimeDataTag
{
    uint8_t ucSubcode;      /* Subcode                子码 */
    uint16_t usEventCnt;    /* Event Count            事件数目 */
    double_t dFSecond;      /* Fractional Second      分数秒 */
    uint8_t ucHour;         /* UTC Hour               UTC小时数 */
    uint8_t ucMinute;       /* UTC Minute             UTC分钟数 */
    uint8_t ucSecond;       /* UTC Second             UTC 秒数 */
    uint8_t ucDay;          /* Day Of Month           天 */
    uint8_t ucMonth;        /* Month                  月 */
    uint16_t usYear;        /* Year                   年 */
    uint8_t ucRecvStatus;   /* Receiver Status        接收器状态(决定时间是否有效) */
    uint8_t ucUTCFlags;     /* UTC Leap Second Flags  UTC闰秒标志 */
    uint8_t ucReserve1;     /* Reserve1               预留1 */
    uint8_t ucReserve2;     /* Reserve2               预留2 */
}PrimUTCTimeData_t;

#pragma pack()

/* GPS Timing Message  GPS校时消息结构体定义 */
typedef struct GPSTimingMsgTag
{
    uint16_t usLen;                 /* Length 消息长度 */
    uint8_t ucBuff[MAX_GPSTF_LEN];  /* Buffer 缓冲区，用于存放消息内容 */
}GPSTimingMsg_t;

/* GPS Timing Settings GPS校时设置参数结构体定义 */
typedef struct GPSTimingSetsTag
{
    bool_t  bEnable;       /* GPS Timing Enable Flag */
    uint8_t ucBaudRate;    /* 0-7: 1200 - 115200 */
    uint8_t ucDataBits;    /* 7/8 */
    uint8_t ucStopBits;    /* 1/2 */
    uint8_t ucParity;      /* 0-2: None, Odd, Even */
    uint8_t ucReserve[3];
}GPSTimingSets_t;

/* PPM Settings PPM设置参数结构体定义 */
typedef struct PPMSetsTag
{
    uint8_t ucPulseSrc;  /* Pulse Source: 0-None; 1-GPS(COM4); 2-P2P(COM1) */
    uint8_t ucPulseRpt;  /* Pulse Repeat: 0-Not Support; 1-Support */
    uint8_t ucPulsePoly; /* Pulse Polarity: 0-Positive; 1-Negative */
}PPMSets_t;

/***************************** Configuration *****************************/
/* Serial Configure Info. 串口配置信息结构体定义 */
typedef struct COMCfgInfoTag
{
    uint8_t ucIndex;                                 /* Index                     当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];                            /* Reserve                   预留 */
    uint32_t uiCMCfgCRC;                             /* CM Configuration CRC      CM配置文件CRC */
    uint32_t uiCMCfgVer;                             /* CM Configuration Version  CM配置文件的版本 */
    COMConfigInfo_t stCOMConfig[LOCAL_CFG_VER_CNT];  /* COM Configure             串口配置参数 */
}COMCfgInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GPSInit
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

void GPSInit( void );

/*
***********************************************************************************************************************
* 函数名称: GPSCycle
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

void GPSCycle( void );

#endif /*GPS_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


