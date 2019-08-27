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
*  文件名称    : LogID.h
*
*  功能简介    :
*
*  作者            : 王东
*
*  创建日期    : 2017-02-13
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef LOGID_H_
#define LOGID_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

/*
*  前10位基址在该文件定义，后22位在其它.h文件中定义 (注意： 按照图中的格式对后22位进行使用)
* ______________________________________________________________________________
* |      log ID 格式规划                                                                                                                          |
* |_____________________________________________________________________________|
* |  主模块号（8bit）                |错误等级 （2bit）|文件号（6bit） |错误号 （16bit）              |
* |-----------------------------------------------------------------------------|
* | AMS:      0x00         | 0-致命   1-严重    |             |                     |
* | Client:   0x01         | 2-一般   3-提示    |             |                     |
* | CommonLib:0x02         |               |             |                     |
* | Config:   0x03         |               |             |                     |
* | CS1131:   0x04         |               |             |                     |
* | Diag:     0x05         |               |             |                     |
* | ExterComm:0x06         |               |             |                     |
* | GPS:      0x07         |               |             |                     |
* | InterComm:0x08         |               |             |                     |
* | LED:      0x09         |               |             |                     |
* | Log:      0x0A         |               |             |                     |
* | Modbus:   0x0B         |               |             |                     |
* | Monitor:  0x0C         |               |             |                     |
* | NTP:      0x0D         |               |             |                     |
* | OPC:      0x0E         |               |             |                     |
* | P2P:      0x0F         |               |             |                     |
* | SOE:      0x10         |               |             |                     |
* | Sys:      0x11         |               |             |                     |
* | Task:     0x12         |               |             |                     |
* | Version:  0x13         |               |             |                     |
* |___________________|_____________|___________|_______________________________|
*/

/* ------------  AMS  ---------------------------------------------------------*/
#define LOG_ID_AMS_BASE                  (0x00000000U)
    #define LOG_ID_AMS_LEVEL0            (0x00000000U)
    #define LOG_ID_AMS_LEVEL1            (0x00400000U)
    #define LOG_ID_AMS_LEVEL2            (0x00800000U)
    #define LOG_ID_AMS_LEVEL3            (0x00C00000U)

/* ------------  Client  -----------------------------------------------------*/
#define LOG_ID_CLI_BASE                  (0x01000000U)
    #define LOG_ID_CLI_LEVEL0            (0x01000000U)
    #define LOG_ID_CLI_LEVEL1            (0x01400000U)
    #define LOG_ID_CLI_LEVEL2            (0x01800000U)
    #define LOG_ID_CLI_LEVEL3            (0x01C00000U)

/* ------------  CommonLib  --------------------------------------------------*/
#define LOG_ID_COMMLIB_BASE              (0x02000000U)
    #define LOG_ID_COMMLIB_LEVEL0        (0x02000000U)
    #define LOG_ID_COMMLIB_LEVEL1        (0x02400000U)
    #define LOG_ID_COMMLIB_LEVEL2        (0x02800000U)
    #define LOG_ID_COMMLIB_LEVEL3        (0x02C00000U)

/* ------------  Config  -----------------------------------------------------*/
#define LOG_ID_CFG_BASE                  (0x03000000U)
    #define LOG_ID_CFG_LEVEL0            (0x03000000U)
    #define LOG_ID_CFG_LEVEL1            (0x03400000U)
    #define LOG_ID_CFG_LEVEL2            (0x03800000U)
    #define LOG_ID_CFG_LEVEL3            (0x03C00000U)

/* ------------  CS1131  -----------------------------------------------------*/
#define LOG_ID_CS1131_BASE               (0x04000000U)
    #define LOG_ID_CS1131_LEVEL0         (0x04000000U)
    #define LOG_ID_CS1131_LEVEL1         (0x04400000U)
    #define LOG_ID_CS1131_LEVEL2         (0x04800000U)
    #define LOG_ID_CS1131_LEVEL3         (0x04C00000U)

/* ------------  Diag  -------------------------------------------------------*/
#define LOG_ID_DIAG_BASE                 (0x05000000U)
    #define LOG_ID_DIAG_LEVEL0           (0x05000000U)
    #define LOG_ID_DIAG_LEVEL1           (0x05400000U)
    #define LOG_ID_DIAG_LEVEL2           (0x05800000U)
    #define LOG_ID_DIAG_LEVEL3           (0x05C00000U)

/* ------------  ExterComm  --------------------------------------------------*/
#define LOG_ID_EXCOMM_BASE               (0x06000000U)
    #define LOG_ID_EXCOMM_LEVEL0         (0x06000000U)
    #define LOG_ID_EXCOMM_LEVEL1         (0x06400000U)
    #define LOG_ID_EXCOMM_LEVEL2         (0x06800000U)
    #define LOG_ID_EXCOMM_LEVEL3         (0x06C00000U)

/* ------------  GPS  --------------------------------------------------------*/
#define LOG_ID_GPS_BASE                  (0x07000000U)
    #define LOG_ID_GPS_LEVEL0            (0x07000000U)
    #define LOG_ID_GPS_LEVEL1            (0x07400000U)
    #define LOG_ID_GPS_LEVEL2            (0x07800000U)
    #define LOG_ID_GPS_LEVEL3            (0x07C00000U)

/* ------------  InterComm  --------------------------------------------------*/
#define LOG_ID_INTERCOMM_BASE            (0x08000000U)
    #define LOG_ID_INTERCOMM_LEVEL0      (0x08000000U)
    #define LOG_ID_INTERCOMM_LEVEL1      (0x08400000U)
    #define LOG_ID_INTERCOMM_LEVEL2      (0x08800000U)
    #define LOG_ID_INTERCOMM_LEVEL3      (0x08C00000U)

/* ------------  LED  --------------------------------------------------------*/
#define LOG_ID_LED_BASE                  (0x09000000U)
    #define LOG_ID_LED_LEVEL0            (0x09000000U)
    #define LOG_ID_LED_LEVEL1            (0x09400000U)
    #define LOG_ID_LED_LEVEL2            (0x09800000U)
    #define LOG_ID_LED_LEVEL3            (0x09C00000U)

/* ------------  Log  --------------------------------------------------------*/
#define LOG_ID_LOG_BASE                  (0x0A000000U)
    #define LOG_ID_LOG_LEVEL0            (0x0A000000U)
    #define LOG_ID_LOG_LEVEL1            (0x0A400000U)
    #define LOG_ID_LOG_LEVEL2            (0x0A800000U)
    #define LOG_ID_LOG_LEVEL3            (0x0AC00000U)

/* ------------  Modbus  -----------------------------------------------------*/
#define LOG_ID_MB_BASE                   (0x0B000000U)
    #define LOG_ID_MB_LEVEL0             (0x0B000000U)
    #define LOG_ID_MB_LEVEL1             (0x0B400000U)
    #define LOG_ID_MB_LEVEL2             (0x0B800000U)
    #define LOG_ID_MB_LEVEL3             (0x0BC00000U)

/* ------------  Monitor  ----------------------------------------------------*/
#define LOG_ID_MONITOR_BASE              (0x0C000000U)
    #define LOG_ID_MONITOR_LEVEL0        (0x0C000000U)
    #define LOG_ID_MONITOR_LEVEL1        (0x0C400000U)
    #define LOG_ID_MONITOR_LEVEL2        (0x0C800000U)
    #define LOG_ID_MONITOR_LEVEL3        (0x0CC00000U)

/* ------------  NTP  --------------------------------------------------------*/
#define LOG_ID_NTP_BASE                  (0x0D000000U)
    #define LOG_ID_NTP_LEVEL0            (0x0D000000U)
    #define LOG_ID_NTP_LEVEL1            (0x0D400000U)
    #define LOG_ID_NTP_LEVEL2            (0x0D800000U)
    #define LOG_ID_NTP_LEVEL3            (0x0DC00000U)

/* ------------  OPC  --------------------------------------------------------*/
#define LOG_ID_OPC_BASE                  (0x0E000000U)
    #define LOG_ID_OPC_LEVEL0            (0x0E000000U)
    #define LOG_ID_OPC_LEVEL1            (0x0E400000U)
    #define LOG_ID_OPC_LEVEL2            (0x0E800000U)
    #define LOG_ID_OPC_LEVEL3            (0x0EC00000U)

/* ------------  P2P  --------------------------------------------------------*/
#define LOG_ID_P2P_BASE                  (0x0F000000U)
    #define LOG_ID_P2P_LEVEL0            (0x0F000000U)
    #define LOG_ID_P2P_LEVEL1            (0x0F400000U)
    #define LOG_ID_P2P_LEVEL2            (0x0F800000U)
    #define LOG_ID_P2P_LEVEL3            (0x0FC00000U)

/* ------------  SOE  --------------------------------------------------------*/
#define LOG_ID_SOE_BASE                  (0x10000000U)
    #define LOG_ID_SOE_LEVEL0            (0x10000000U)
    #define LOG_ID_SOE_LEVEL1            (0x10400000U)
    #define LOG_ID_SOE_LEVEL2            (0x10800000U)
    #define LOG_ID_SOE_LEVEL3            (0x10C00000U)

/* ------------  Sys  --------------------------------------------------------*/
#define LOG_ID_SYS_BASE                  (0x11000000U)
    #define LOG_ID_SYS_LEVEL0            (0x11000000U)
    #define LOG_ID_SYS_LEVEL1            (0x11400000U)
    #define LOG_ID_SYS_LEVEL2            (0x11800000U)
    #define LOG_ID_SYS_LEVEL3            (0x11C00000U)

/* ------------  Task  -------------------------------------------------------*/
#define LOG_ID_TASK_BASE                 (0x12000000U)
    #define LOG_ID_TASK_LEVEL0           (0x12000000U)
    #define LOG_ID_TASK_LEVEL1           (0x12400000U)
    #define LOG_ID_TASK_LEVEL2           (0x12800000U)
    #define LOG_ID_TASK_LEVEL3           (0x12C00000U)

/* ------------  Version  -------------------------------------------------------*/
#define LOG_ID_VERN_BASE                 (0x13000000U)
    #define LOG_ID_VERN_LEVEL0           (0x13000000U)
    #define LOG_ID_VERN_LEVEL1           (0x13400000U)
    #define LOG_ID_VERN_LEVEL2           (0x13800000U)
    #define LOG_ID_VERN_LEVEL3           (0x13C00000U)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

#endif /* LOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

