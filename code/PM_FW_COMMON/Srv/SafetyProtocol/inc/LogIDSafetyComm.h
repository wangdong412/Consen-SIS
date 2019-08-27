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
*  文件名称    : LogIDSafetyComm.h
*
*  功能简介    :
*
*  作者            : 王东
*
*  创建日期    : 2017-04-10
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef LOGID_MANAGER_H_
#define LOGID_MANAGER_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

/* ------------  L0: 致命  ----------------------------------------------------*/
/*#define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL0                               (0x05000000U) */



/* ------------  L1: 严重  ----------------------------------------------------*/
/*#define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL1                               (0x05400000U) */



/* ------------  L2: 一般  ----------------------------------------------------*/
/*#define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL2                               (0x05800000U) */
/* P2P */
#define LOGID_SCOMM_P2P_BIT_REV            (0x05800000U)  /* P2P: Bit reverse error */
#define LOGID_SCOMM_P2P_CRC                (0x05800001U)  /* P2P: CRC error */
#define LOGID_SCOMM_P2P_ADDR               (0x05800002U)  /* P2P: Address error */
#define LOGID_SCOMM_P2P_SQ                 (0x05800003U)  /* P2P: SQ error */
#define LOGID_SCOMM_P2P_LEN                (0x05800004U)  /* P2P: Length error */
#define LOGID_SCOMM_P2P_RECV_REQ_TIMEOUT   (0x05800005U)  /* P2P: Receive request timeout */
#define LOGID_SCOMM_P2P_RECV_RESP_TIMEOUT  (0x05800006U)  /* P2P: Receive response timeout */
#define LOGID_SCOMM_P2P_SEND_REQ_TIMEOUT   (0x05800007U)  /* P2P: Send request timeout */

/* PM_BUS */
#define LOGID_SCOMM_PMS_CRC                (0x05800011U)  /* PMs safety communication: CRC error */
#define LOGID_SCOMM_PMS_ADDR               (0x05800012U)  /* PMs safety communication: Address error */
#define LOGID_SCOMM_PMS_SQ                 (0x05800013U)  /* PMs safety communication: SQ error */
#define LOGID_SCOMM_PMS_LEN                (0x05800014U)  /* PMs safety communication: Length error */
#define LOGID_SCOMM_PMS_RECV_TIMEOUT       (0x05800015U)  /* PMs safety communication: Receive timeout */
#define LOGID_SCOMM_PMS_SEND_TIMEOUT       (0x05800016U)  /* PMs safety communication: Send timeout */

/* IP_BUS */
#define LOGID_SCOMM_IP_BIT_REV             (0x05800020U)  /* IP safety communication: Bit reverse error */
#define LOGID_SCOMM_IP_CRC                 (0x05800021U)  /* IP safety communication: CRC error */
#define LOGID_SCOMM_IP_ADDR                (0x05800022U)  /* IP safety communication: Address error */
#define LOGID_SCOMM_IP_SQ                  (0x05800023U)  /* IP safety communication: SQ error */
#define LOGID_SCOMM_IP_LEN                 (0x05800024U)  /* IP safety communication: Length error */
#define LOGID_SCOMM_IP_RECV_RESP_TIMEOUT   (0x05800025U)  /* IP safety communication: Receive response timeout */
#define LOGID_SCOMM_IP_SEND_REQ_TIMEOUT    (0x05800026U)  /* IP safety communication: Send request timeout */


/* ------------  L3: 提示  ----------------------------------------------------*/
/*#define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL3                               (0x05C00000U) */



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


#endif /* LOGID_MANAGER_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

