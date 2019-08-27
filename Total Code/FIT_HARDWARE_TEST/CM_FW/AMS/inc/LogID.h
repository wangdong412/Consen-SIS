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
*  创建日期    : 2017-02-14
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef AMSLOGID_H_
#define AMSLOGID_H_

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
* |___________________|_____________|___________|______________________________|
*/

/* ------------  L0: 致命  ----------------------------------------------------*/
/*#define LOG_ID_AMS_LEVEL0                    (0x00000000U)*/
#define LOGID_AMS_E_SWTO                       (0x00000000U)  /* AMS software timeout */
#define LOGID_AMS_E_MAKE_RESP                  (0x00000001U)  /* Make AMS response message error */
#define LOGID_AMS_E_ACCESS                     (0x00000002U)  /* AMS software has no suitable access right */
#define LOGID_AMS_E_ACCESS_DENY                (0x00000003U)  /* Access right of AMS software is deny */
#define LOGID_AMS_E_ACCESS_RO                  (0x00000004U)  /* Access right of AMS software is read-only */
#define LOGID_AMS_E_NOTLOGIN                   (0x00000005U)  /* AMS software is not logged in */
#define LOGID_AMS_E_CMBUS                      (0x00000006U)  /* AMS: CM BUS invalid */
#define LOGID_AMS_E_SW_SRCADDR                 (0x00000007U)  /* AMS source address invalid */

#define LOGID_AMS_E_CMD                        (0x00000020U)  /* AMS message invalid */
#define LOGID_AMS_E_CMD_ACTUAL_TYPE            (0x00000021U)  /* Actual type of AMS message invalid */
#define LOGID_AMS_E_CMD_PASSWORD_LEN           (0x00000022U)  /* AMS: password length error */
#define LOGID_AMS_E_CMD_PASSWORD_CONTENT       (0x00000023U)  /* AMS: password content error */

#define LOGID_AMS_E_RESP_PM_TIMEOUT            (0x00000040U)  /* AMS: PM timeout */
#define LOGID_AMS_E_RESP_EXCOMM_SEND           (0x00000041U)  /* AMS: send response message error */

#define LOGID_AMS_E_SYS_SET_MSG                (0x00000060U)  /* SysSetMsg-AMS_REQ error */
#define LOGID_AMS_E_SYS_GET_RTDATA_ADDR        (0x00000061U)  /* AMS: SysGetRTDataAddr error */
#define LOGID_AMS_E_SYS_RST_LOCK               (0x00000062U)  /* AMS: Reset locked flag failed */
#define LOGID_AMS_E_SYS_SET_LOCK               (0x00000063U)  /* AMS: Set locked flag failed */
#define LOGID_AMS_E_SYS_RST_CONN               (0x00000064U)  /* AMS: Reset connect flag failed */
#define LOGID_AMS_E_SYS_SET_CONN               (0x00000065U)  /* AMS: Set connect flag failed */
#define LOGID_AMS_E_SYS_SET_CLR                (0x00000066U)  /* AMS: Set clear flag failed */


/* ------------  L1: 严重  ----------------------------------------------------*/
/*#define LOG_ID_AMS_LEVEL1                    (0x00400000U)*/



/* ------------  L2: 一般  ----------------------------------------------------*/
/*#define LOG_ID_AMS_LEVEL2                    (0x00800000U)*/
#define LOGID_AMS_E_CM_NOCFG                   (0x00800000U)  /* AMS: CM has no configuration */


/* ------------  L3: 提示  ----------------------------------------------------*/
/*#define LOG_ID_AMS_LEVEL3                    (0x00C00000U)*/
#define LOGID_AMS_CFG_SWITCH_ACCESS_CTRL       (0x00C00000U)  /* AMS: access control configuration switch */

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

#endif /* AMSLOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

