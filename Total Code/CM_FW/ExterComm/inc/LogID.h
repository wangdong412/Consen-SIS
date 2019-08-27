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

#ifndef EXCOMMLOGID_H_
#define EXCOMMLOGID_H_

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
/*#define LOG_ID_EXCOMM_LEVEL0                         (0x06000000U)*/
#define LOGID_EXCOMM_E_SEND_TYPE                       (0x06000000U)
#define LOGID_EXCOMM_E_DEL_MBTCP_SLAVE                 (0x06000001U)  /* Delete MBTCPSlave IP-(0x%08X) Not Exist */
#define LOGID_EXCOMM_E_ADD_MBTCP_SLAVE_EXIST           (0x06000002U)  /* MBTCPSlave: IP-(0x%08X) Aleady Exist */
#define LOGID_EXCOMM_E_ADD_MBTCP_SLAVE_FULL            (0x06000003U)  /* Add MBTCPSlave Failed: Net %d Slave Array Full */
//#define LOGID_EXCOMM_E_CLIENT_CONN                     (0x06000004U)  /* Connect to MBTCPSlave failed */
//#define LOGID_EXCOMM_E_CLIENT_CONN_TO                  (0x06000005U)  /* Connect to MBTCPSlave timeout */
//#define LOGID_EXCOMM_E_MBTCP_SLAVE_NOT_ALLOW_CONN      (0x06000006U)  /* MBTCP: Not Allow Connect */
#define LOGID_EXCOMM_E_ACCEPT_CONN_FULL                (0x06000007U)  /* Accept connect: local array full */
#define LOGID_EXCOMM_E_APP_MSG                         (0x06000008U)  /* APPTCPMsg Invalid */
#define LOGID_EXCOMM_E_APP_MSG_LEN                     (0x06000009U)  /* APPTCPMsg length error */
#define LOGID_EXCOMM_E_READ_APP_MSG_CONTENT            (0x0600000AU)  /* Read APPTCPMsg content failed */
#define LOGID_EXCOMM_E_P2P_MSG                         (0x0600000BU)  /* ReadP2PMsg: MSG Invalid */
#define LOGID_EXCOMM_E_MBTCPMSTR_MSG_LEN               (0x0600000CU)  /* ReadMBTCPMstrMsg: Len Invalid */
#define LOGID_EXCOMM_E_READ_MBTCPMSTR_MSG_CONTENT      (0x0600000DU)  /* MBTCPMstrMsg: Read non-head data failed */
#define LOGID_EXCOMM_E_MBTCPSLAVE_MSG_LEN              (0x0600000EU)
#define LOGID_EXCOMM_E_READ_MBTCPSLAVE_MSG_CONTENT     (0x0600000FU)

#define LOGID_EXCOMM_E_SEND_SOESW_DISCONN              (0x06000010U)
#define LOGID_EXCOMM_E_SEND_SOESW_SRC                  (0x06000011U)
#define LOGID_EXCOMM_E_SEND_AMSSW_DISCONN              (0x06000012U)
#define LOGID_EXCOMM_E_SEND_AMSSW_SRC                  (0x06000013U)
#define LOGID_EXCOMM_E_SEND_CLISW_DISCONN              (0x06000014U)
#define LOGID_EXCOMM_E_SEND_CLISW_SRC                  (0x06000015U)
#define LOGID_EXCOMM_E_SEND_OPCSW_DISCONN              (0x06000016U)
#define LOGID_EXCOMM_E_SEND_OPCSW_SRC                  (0x06000017U)
#define LOGID_EXCOMM_E_SEND_CS1131SW_DISCONN           (0x06000018U)
#define LOGID_EXCOMM_E_SEND_CS1131SW_SRC               (0x06000019U)
#define LOGID_EXCOMM_E_SEND_MBTCPMSTR_DISCONN          (0x0600001AU)
#define LOGID_EXCOMM_E_SEND_MBTCPMSTR_SRC              (0x0600001BU)
//#define LOGID_EXCOMM_E_SEND_MBTCPSLAVE_DISCONN         (0x0600001CU)
//#define LOGID_EXCOMM_E_SEND_MBTCPSLAVE_NOT_EXIST       (0x0600001DU)
#define LOGID_EXCOMM_E_SEND_P2P_SOCKET                 (0x0600001EU)

/* Linux OS */
#define LOGID_EXCOMM_E_SYSCALL                         (0x06000020U)  /* syscall os function */
#define LOGID_EXCOMM_E_SET_NET                         (0x06000021U)
#define LOGID_EXCOMM_E_SET_NET_IP                      (0x06000022U)
#define LOGID_EXCOMM_E_SET_NET_MASK                    (0x06000023U)
#define LOGID_EXCOMM_E_SET_GATEWAY                     (0x06000024U)
#define LOGID_EXCOMM_E_SET_IPROUTE                     (0x06000025U)
#define LOGID_EXCOMM_E_LINUX_EXECVE                    (0x06000026U)  /* LinuxSystem */
#define LOGID_EXCOMM_E_GETSOCKOPT                      (0x06000027U)
#define LOGID_EXCOMM_E_SELECT                          (0x06000028U)
#define LOGID_EXCOMM_E_SETNONBLKMODE                   (0x06000029U)
#define LOGID_EXCOMM_E_ACCEPT                          (0x0600002AU)
#define LOGID_EXCOMM_E_RECV_APPDATA                    (0x0600002BU)
#define LOGID_EXCOMM_E_RECV_APPDATA_DISCONN            (0x0600002CU)
#define LOGID_EXCOMM_E_RECV_MBTCPMSTRDATA              (0x0600002DU)
#define LOGID_EXCOMM_E_RECV_MBTCPMSTR_DISCONN          (0x0600002EU)
#define LOGID_EXCOMM_E_RECV_MBTCPSLAVEDATA             (0x0600002FU)

#define LOGID_EXCOMM_E_RECV_MBTCPSLAVE_DISCONN         (0x06000030U)
#define LOGID_EXCOMM_E_UDP_SENDTO_FAIL                 (0x06000031U)
#define LOGID_EXCOMM_E_UDP_SENDTO_NONFIN               (0x06000032U)
#define LOGID_EXCOMM_E_UDP_SENDTO_NOTRDY               (0x06000033U)
#define LOGID_EXCOMM_E_TCP_SEND_FAIL                   (0x06000034U)
#define LOGID_EXCOMM_E_TCP_SEND_NONFIN                 (0x06000035U)
#define LOGID_EXCOMM_E_TCP_SEND_NOTRDY                 (0x06000036U)

#define LOGID_EXCOMM_E_CFG_NET                         (0x06000040U)
#define LOGID_EXCOMM_E_CFG_MBTCP                       (0x06000041U)
#define LOGID_EXCOMM_E_CFG_ACCESS_CTRL                 (0x06000042U)
#define LOGID_EXCOMM_E_CFG_GET_MBMSTRINFO              (0x06000043U)
#define LOGID_EXCOMM_E_CFG_MBMSTR                      (0x06000044U)
#define LOGID_EXCOMM_E_CFG_TIMESYNC                    (0x06000045U)
#define LOGID_EXCOMM_E_CFG_MBMSTR_PORT                 (0x06000046U)

#define LOGID_EXCOMM_E_SYS_SET_APPMSG                  (0x06000060U)
#define LOGID_EXCOMM_E_SYS_RST_SWCONN                  (0x06000061U)
#define LOGID_EXCOMM_E_SYS_SET_SWCONN                  (0x06000062U)
#define LOGID_EXCOMM_E_SYS_RST_SWCLR                   (0x06000063U)
#define LOGID_EXCOMM_E_SYS_SET_SWCLR                   (0x06000064U)
#define LOGID_EXCOMM_E_SYS_SET_P2PMSG                  (0x06000065U)
#define LOGID_EXCOMM_E_SYS_SET_MBTCPMSTRMSG            (0x06000066U)
#define LOGID_EXCOMM_E_SYS_SET_MBTCPSLAVEMSG           (0x06000067U)
#define LOGID_EXCOMM_E_SYS_GET_RTDATA_ADDR             (0x06000068U)
#define LOGID_EXCOMM_E_GET_DATA_AREA_LEN               (0x06000080U)
#define LOGID_EXCOMM_E_CM_BUS                          (0x06000081U)

#define LOGID_EXCOMM_E_INIT_FUTASK                     (0x06000082U)  /* Init File Up Task Error */


/* ------------  L1: 严重  ----------------------------------------------------*/
/*#define LOG_ID_EXCOMM_LEVEL1                         (0x06400000U)*/



/* ------------  L2: 一般  ----------------------------------------------------*/
/*#define LOG_ID_EXCOMM_LEVEL2                          (0x06800000U)*/


/* ------------  L3: 提示  ----------------------------------------------------*/
/*#define LOG_ID_EXCOMM_LEVEL3                          (0x06C00000U)*/
#define LOGID_EXCOMM_CFG_SWITCH_NET                     (0x06C00000U)
#define LOGID_EXCOMM_CFG_SWITCH_MBTCP                   (0x06C00001U)
#define LOGID_EXCOMM_CFG_SWITCH_MBMSTR                  (0x06C00002U)

#define LOGID_EXCOMM_SELRTDATA_CHANGE_PM                (0x06C00010U)  /* Change RTData source PM module */

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

#endif /* EXCOMMLOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

