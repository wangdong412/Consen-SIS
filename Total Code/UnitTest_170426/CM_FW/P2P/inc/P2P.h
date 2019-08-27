/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : P2P模块
*
*  文件名称    : P2P.h
*
*  功能简介    : 处理P2P请求和应答消息
*
*  作者        : 王东
*
*  创建日期    : 2016-02-17
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef P2P_H_
#define P2P_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "ExterComm/inc/ExterComm.h"
#include "Task/inc/MainTask.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_P2PCOMM

#define DEBUG_OUT_P2P

#ifdef DEBUG_OUT_P2P
#define DEBUG_P2P(format, args...)  printf ("DB_P2P: ") ; printf(format, ##args)
#else
#define DEBUG_P2P(format, args...)
#endif

#define P2P_PM_TIMEOUT_CYCLE_CNT (1600*1000/(MAIN_TASK_PERIOD))  /* PM应答P2P命令的超时拍数：1600ms */
#define MAX_PM_P2P_RESP_MSG_LEN  (100)      /* PM站间应答数据缓冲区的最大长度 */
#define MAX_PM_P2P_REQ_MSG_LEN   (1*1024)   /* PM构造的Peer-To-Peer请求帧的最大长度 */

/* PM P2P Message: Addtional Info.(20B) + Safety Message + FCS(4B) */
#define PM_P2P_MSG_ADDL_LEN      (sizeof(PMP2PReqAddInfo_t))  /* PM构造的请求帧/应答帧附加信息的长度 */
#define PM_P2P_MSG_FCS_LEN       (sizeof(uint32_t))           /* PM构造的请求帧/应答帧FCS域的长度 */
#define PM_P2P_MSG_NONSAFETY_LEN ((PM_P2P_MSG_ADDL_LEN) + (PM_P2P_MSG_FCS_LEN))

#define PMA_REQ_BIT              (0x01U)   /* PMA Request Bit */
#define PMB_REQ_BIT              (0x02U)   /* PMB Request Bit */
#define PMC_REQ_BIT              (0x04U)   /* PMC Request Bit */

/* Station ID */
#define MIN_STA_ID    (1)       /* 站ID的最小值 */
#define MAX_STA_ID    (64)      /* 站ID的最大值 */

/* Message Value */
#define RESP_TIMEOUT  (1)       /* Response Timeout */

/* Message Index */
#define TIMEOUT_INDEX (2)
#define INVALID_STAID (0xFF)

#define MAX_WAIT_PMREQ_CYCLE_CNT  (10)  /* 等待本站PM请求帧的最大拍数 */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Source Network  源端网口枚举定义 */
typedef enum SrcNetTag
{
    SRC_NET1 = 0,     /* Source Network 1   源网口1 */
    SRC_NET2,         /* Source Network 2   源网口2 */
    SRC_NETALL,       /* Source Network All 源网口1和2 */
    SRC_INVALID       /* Invalid Network    无效网口 */
}SrcNet_t;

/* PM P2P Request Message Info  PM站间请求消息信息结构体定义 */
typedef struct PMP2PReqMsgInfoTag
{
    uint16_t usCount;                         /* Count 实际字节数 */
    uint8_t ucBuff[MAX_PM_P2P_REQ_MSG_LEN];   /* Buffer 数据缓冲区 */
}PMP2PReqMsgInfo_t;

/* PM P2P Response Message Info  PM站间应答消息信息结构体定义 */
typedef struct PMP2PRespMsgInfoTag
{
    uint16_t usCount;                         /* Count 实际字节数 */
    uint8_t ucBuff[MAX_PM_P2P_RESP_MSG_LEN];  /* Buffer 数据缓冲区 */
}PMP2PRespMsgInfo_t;

/* Network Info  网络信息结构体定义 */
typedef struct NetInfoTag
{
    uint32_t uiDstIP1Addr;   /* Destination IP1 Address 对端的IP地址1 */
    uint32_t uiDstIP2Addr;   /* Destination IP2 Address 对端的IP地址2 */
    uint8_t ucL1NetID;       /* Link 1 NetID            链路1的网口ID: 0-Net1；1-Net2 */
    SrcNet_t emSrcNetwork;   /* Source Network          源端网口号 */
}NetInfo_t;

/* PM P2P Request Item Info  PM站间请求项信息结构体定义 */
typedef struct PMP2PReqItemInfoTag
{
    uint8_t ucDstStaID;          /* Dest Station ID 目标控制站ID */
    bool_t bWaitFlag;            /* Wait Flag 等待标志位 */
    uint8_t ucWaitCnt;           /* Wait Count    等待计数 */
    bool_t bPMAReqValid;         /* PMA Request Valid Flag  PMA请求帧有效标志位 */
    bool_t bPMBReqValid;         /* PMB Request Valid Flag  PMB请求帧有效标志位 */
    bool_t bPMCReqValid;         /* PMC Request Valid Flag  PMC请求帧有效标志位 */
    uint8_t ucReserve[2];        /* Reserve 预留 */
    PMP2PReqMsgInfo_t stPMAReq;  /* PMA Request 来自PMA的请求帧 */
    PMP2PReqMsgInfo_t stPMBReq;  /* PMB Request 来自PMB的请求帧 */
    PMP2PReqMsgInfo_t stPMCReq;  /* PMC Request 来自PMC的请求帧 */
}PMP2PReqItemInfo_t;

/* PM P2P Request Additional Info.  PM站间请求附加信息结构体定义 */
typedef struct PMP2PReqAddInfoTag
{
    uint8_t ucSrcCMSlotID;  /* Source CM Slot ID       源控制站CM模块的槽位号 */
    uint8_t ucSrcNETID;     /* Source Network ID       源控制站CM模块的网口编号 */
    bool_t bTimeout;        /* Timeout Flag            超时标志位：0-未超时；1-超时 */
    uint8_t ucL1NetID;      /* Link 1 NetID            链路1的网口ID: 0-Net1；1-Net2 */
    uint32_t uiDstIP1;      /* DstIP1 Address          目的控制站的IP地址1 */
    uint32_t uiDstIP2;      /* DstIP2 Address          目的控制站的IP地址2 */
    uint8_t ucSrcStaID;     /* Source Station ID       源控制站ID：1-64 */
    uint8_t ucDstStaID;     /* Destination Station ID  目的控制站ID：1-64 */
    uint8_t ucFraType;      /* Frame Type              帧类型：0-请求帧；1-应答帧 */
    uint8_t ucReserve2;     /* Reserve 2               预留 2*/
    uint32_t uiTotalLen;    /* Total Length            数据帧的总长度（字节），包括附加信息和FCS */
}PMP2PReqAddInfo_t;

/* PM P2P Response Additional Info.  PM站间应答附加信息结构体定义 */
typedef struct PMP2PRespAddInfoTag
{
    uint8_t ucSrcCMSlotID;  /* Source CM Slot ID       源控制站CM模块的槽位号 */
    uint8_t ucSrcNETID;     /* Source Network ID       源控制站CM模块的网口编号 */
    uint8_t ucReserve1[2];  /* Reserve 1               预留 1*/
    uint32_t uiDstIP1;      /* DstIP1 Address          目的控制站的IP地址1 */
    uint32_t uiDstIP2;      /* DstIP2 Address          目的控制站的IP地址2 */
    uint8_t ucSrcStaID;     /* Source Station ID       源控制站ID：1-64 */
    uint8_t ucDstStaID;     /* Destination Station ID  目的控制站ID：1-64 */
    uint8_t ucFraType;      /* Frame Type              帧类型：0-请求帧；1-应答帧 */
    uint8_t ucReserve2;     /* Reserve 2               预留 2*/
    uint32_t uiTotalLen;    /* Total Length            数据帧的总长度（字节），包括附加信息和FCS */
}PMP2PRespAddInfo_t;

/* CM P2P Request Additional Info.  PM站间请求附加信息结构体定义 */
typedef struct CMP2PReqAddInfoTag
{
    uint8_t ucSrcStaID;     /* Source Station ID       源控制站ID：1-64 */
    uint8_t ucDstStaID;     /* Destination Station ID  目的控制站ID：1-64 */
    uint8_t ucFraType;      /* Frame Type              帧类型：0-请求帧；1-应答帧 */
    uint8_t ucLinkID;       /* Link ID                 链路ID: 1-链路1；2-链路2。只对同CM双网口冗余有效 */
    uint16_t usTotalLen;    /* Total Length            数据帧的总长度（字节），包括附加信息和FCS */
    uint8_t ucPMInfo;       /* PM Info.                PM信息- Bit 0: PMA;  Bit 1: PMB;  Bit 2: PMC
                                                                                                                      当此数据帧包含相应PM的安全帧时，相应的位置1 */
    uint8_t ucReserve;      /* Reserve                 预留 */
}CMP2PReqAddInfo_t;

/* CM P2P Response Additional Info.  PM站间应答附加信息结构体定义 */
typedef struct CMP2PRespAddInfoTag
{
    uint8_t ucSrcStaID;     /* Source Station ID       源控制站ID：1-64 */
    uint8_t ucDstStaID;     /* Destination Station ID  目的控制站ID：1-64 */
    uint8_t ucFraType;      /* Frame Type              帧类型：0-请求帧；1-应答帧 */
    uint8_t ucReserve1;     /* Reserve 1               预留 1*/
    uint16_t usTotalLen;    /* Total Length            数据帧的总长度（字节），包括附加信息和FCS */
    uint8_t ucReserve2[2];  /* Reserve 2               预留 2 */
}CMP2PRespAddInfo_t;

/* PM P2P Response Item Info  PM站间应答项信息结构体定义 */
typedef struct PMP2PRespItemInfoTag
{
    bool_t bWaitFlag;              /* Wait Flag     等待标志位 */
    uint8_t ucReserve[3];          /* Reserve       预留 */
    PMP2PRespMsgInfo_t stPMAResp;  /* PMA Response  来自PMA的应答帧 */
    PMP2PRespMsgInfo_t stPMBResp;  /* PMB Response  来自PMB的应答帧 */
    PMP2PRespMsgInfo_t stPMCResp;  /* PMC Response  来自PMC的应答帧 */
}PMP2PRespItemInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: P2PInit
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

void P2PInit( void );

/*
***********************************************************************************************************************
* 函数名称: P2PCycle
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

void P2PCycle( void );

#endif /*P2P_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


