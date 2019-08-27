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

#ifndef MBLOGID_H_
#define MBLOGID_H_

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
/*#define LOG_ID_MB_LEVEL0                     (0x0B000000U)*/
#define LOGID_MB_E_CFG_GET_MBSLAVE_INFO        (0x0B000000U)
#define LOGID_MB_E_CFG_MBSLAVE                 (0x0B000001U)
#define LOGID_MB_E_CFG_MBTCP                   (0x0B000002U)
#define LOGID_MB_E_CFG_COM                     (0x0B000003U)

#define LOGID_MB_E_SYS_SEND_PM                 (0x0B000020U)  /* SysSetModbusMsgQueue */
#define LOGID_MB_E_SYS_GET_RTDATA_ADDR         (0x0B000021U)  /* SysGetRTDataAddr */
#define LOGID_MB_E_SYS_GETMBTCP_MSTR_MSG       (0x0B000022U)  /* SysGetModbusTCPMasterMsg */


#define LOGID_MB_E_CMBUS                       (0x0B000040U)
#define LOGID_MB_E_GET_MBVAR_ADDR              (0x0B000041U)  /* GetModbusVarAddr */
#define LOGID_MB_E_GET_MBBOOL_VALUE            (0x0B000042U)  /* GetMBBOOLValue */
#define LOGID_MB_E_ADD_COILS_REQDATA           (0x0B000043U)  /* AddCoilsReqData */
#define LOGID_MB_E_ADD_REGS_REQDATA            (0x0B000044U)  /* AddRegsReqData */
#define LOGID_MB_E_ADD_READREQDATA             (0x0B000045U)  /* AddReadReqData */
#define LOGID_MB_E_ADD_WRITEREQDATA            (0x0B000046U)  /* AddWriteReqData */
#define LOGID_MB_E_MAKE_EXCEPT_RESP            (0x0B000047U)  /* MakeMBTCPExRespMsg */
#define LOGID_MB_E_SEL_RTDATA                  (0x0B000048U)  /* SelectRTData */


#define LOGID_MB_E_SEND_MBTCP_MSG              (0x0B000050U)  /* SendModbusTCPReq/Resp */
#define LOGID_MB_E_MAKE_MBTCPREQ               (0x0B000051U)  /* MakeMBTCPReq */
#define LOGID_MB_E_MAKE_MBTCP_RESP             (0x0B000052U)  /* MakeMBTCPResp */
#define LOGID_MB_E_MBTCP_RESP                  (0x0B000053U)  /* CheckMBTCPResp */


#define LOGID_MB_E_SEND_MBSERL_MSG             (0x0B000060U)  /* SendMBSerlReq/Resp */
#define LOGID_MB_E_MAKE_MBSERL_REQ             (0x0B000061U)  /* MakeMBSerlReq */
#define LOGID_MB_E_MAKE_MBSERL_RESP            (0x0B000062U)  /* MakeMBSerlResp */
#define LOGID_MB_E_MBSERL_RESP                 (0x0B000063U)  /* CheckMBSerlResp */
#define LOGID_MB_E_MBSERL_SLAVE_TIMEOUT        (0x0B000064U)  /* Modbus serial slave timeout */


#define LOGID_MB_E_SERIAL_RECV                 (0x0B000070U)  /* SerialRecv */
#define LOGID_MB_E_SERIAL_SEND                 (0x0B000071U)  /* SerialSend */


#define LOGID_MB_E_FUNC_CODE                   (0x0B000080U)  /* Function code: Not support */
#define LOGID_MB_E_MBTCP_RESP_LEN              (0x0B000081U)  /* MBTCPResp length is invalid */
#define LOGID_MB_E_MBTCP_REQ_LEN               (0x0B000082U)  /* MBTCPReq length is invalid */
#define LOGID_MB_E_MBSERL_RESP_LEN             (0x0B000083U)  /* MBSerlResp length is invalid */
#define LOGID_MB_E_MBSERL_REQ_LEN              (0x0B000084U)  /* MBSerlReq length is invalid */
#define LOGID_MB_E_MBRTU_CRC                   (0x0B000085U)  /* MBRTU CRC is invalid */
#define LOGID_MB_E_MBASCII_LRC                 (0x0B000086U)  /* MBASCII LRC is invalid */



/* ------------  L1: 严重  ----------------------------------------------------*/
/*#define LOG_ID_MB_LEVEL1                     (0x0B400000U)*/



/* ------------  L2: 一般  ----------------------------------------------------*/
/*#define LOG_ID_MB_LEVEL2                     (0x0B800000U)*/


/* ------------  L3: 提示  ----------------------------------------------------*/
/*#define LOG_ID_MB_LEVEL3                     (0x0BC00000U)*/
#define LOGID_MB_CFG_SWITCH_MBSLAVE            (0x0BC00000U)
#define LOGID_MB_CFG_SWITCH_MBMSTR             (0x0BC00001U)
#define LOGID_MB_CFG_SWITCH_COM                (0x0BC00002U)
#define LOGID_MB_CFG_SWITCH_MBTCP              (0x0BC00003U)



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

#endif /* MBLOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

