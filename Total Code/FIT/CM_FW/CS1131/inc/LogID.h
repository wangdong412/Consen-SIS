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

#ifndef CS1131LOGID_H_
#define CS1131LOGID_H_

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
/*#define LOG_ID_CS1131_LEVEL0                 (0x04000000U)*/
#define LOGID_CS1131_E_SWTO                    (0x04000000U)
#define LOGID_CS1131_E_MAKE_RESP               (0x04000001U)
#define LOGID_CS1131_E_ACCESS                  (0x04000002U)
#define LOGID_CS1131_E_ACCESS_DENY             (0x04000003U)
#define LOGID_CS1131_E_ACCESS_RO               (0x04000004U)
#define LOGID_CS1131_E_NOTLOGIN                (0x04000005U)
#define LOGID_CS1131_E_CMBUS                   (0x04000006U)
#define LOGID_CS1131_E_SW_SRCADDR              (0x04000007U)

#define LOGID_CS1131_E_CMD                     (0x04000020U)
#define LOGID_CS1131_E_CMD_ACTUAL_TYPE         (0x04000021U)
#define LOGID_CS1131_E_CMD_PASSWORD_LEN        (0x04000022U)
#define LOGID_CS1131_E_CMD_PASSWORD_CONTENT    (0x04000023U)

#define LOGID_CS1131_E_RESP_PM_TIMEOUT         (0x04000040U)
#define LOGID_CS1131_E_RESP_EXCOMM_SEND        (0x04000041U)

#define LOGID_CS1131_E_SYS_SET_MSG             (0x04000060U)
#define LOGID_CS1131_E_SYS_GET_RTDATA_ADDR     (0x04000061U)
#define LOGID_CS1131_E_SYS_RST_LOCK            (0x04000062U)
#define LOGID_CS1131_E_SYS_SET_LOCK            (0x04000063U)
#define LOGID_CS1131_E_SYS_RST_CONN            (0x04000064U)
#define LOGID_CS1131_E_SYS_SET_CONN            (0x04000065U)
#define LOGID_CS1131_E_SYS_SET_CLR             (0x04000066U)

#define LOGID_CS1131_E_ADD_PRJ_VER             (0x04000080U)
#define LOGID_CS1131_E_ADD_FILE_DATA           (0x04000081U)
#define LOGID_CS1131_E_ADD_STATUS_DATA         (0x04000082U)
#define LOGID_CS1131_E_ADD_ONLINE_DATA         (0x04000083U)
#define LOGID_CS1131_E_WRITE_SRC_FILE          (0x04000084U)
#define LOGID_CS1131_E_SRC_FILE                (0x04000085U)
#define LOGID_CS1131_E_READ_STS_TYPE           (0x04000086U)  /* Type of ReadStatus message invalid */
#define LOGID_CS1131_E_TOTAL_CNT               (0x04000087U)  /* TotalVarCnts of DefMonVarTable message overrange */
#define LOGID_CS1131_E_DEF_VARIABLE            (0x04000088U)
#define LOGID_CS1131_E_ADD_IO_STATUS_DATA      (0x04000089U)  /* AddIOStatusData Failed */
#define LOGID_CS1131_E_UP_SRCFILE              (0x0400008AU)  /* Update Src File Error */

#define LOGID_CS1131_E_OPEN_FILE               (0x04000090U)
#define LOGID_CS1131_E_WRITE_FILE              (0x04000091U)
#define LOGID_CS1131_E_READ_FILE               (0x04000092U)
#define LOGID_CS1131_E_CLOSE_FILE              (0x04000093U)
#define LOGID_CS1131_E_LSEEK_FILE              (0x04000094U)
#define LOGID_CS1131_E_UNLINK_FILE             (0x04000095U)

/* ------------  L1: 严重  ----------------------------------------------------*/
/*#define LOG_ID_CS1131_LEVEL1                 (0x04400000U)*/



/* ------------  L2: 一般  ----------------------------------------------------*/
/*#define LOG_ID_CS1131_LEVEL2                 (0x04800000U)*/
#define LOGID_CS1131_E_CM_NOCFG                (0x04800000U)
#define LOGID_CS1131_E_CM_DOWNING              (0x04800001U)



/* ------------  L3: 提示  ----------------------------------------------------*/
/*#define LOG_ID_CS1131_LEVEL3                 (0x04C00000U)*/
#define LOGID_CS1131_CFG_SWITCH_ACCESS_CTRL    (0x04C00000U)



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

#endif /* CS1131LOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

