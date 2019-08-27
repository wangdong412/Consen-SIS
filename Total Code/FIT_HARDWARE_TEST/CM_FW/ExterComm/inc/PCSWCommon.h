/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : CM_FW外部通信模块、上位机应用模块
*
*  文件名称    : PCSWCommon.h
*
*  功能简介    : 上位机软件与外部通讯模块的公用部分
*
*  作者        : 王东
*
*  创建日期    : 2016-02-16
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef PCSWCOMMON_H_
#define PCSWCOMMON_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "Sys/inc/Sys.h"
#include "Sys/inc/DataType.h"
#include "ComCommon.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define DEBUG_PCSWCOMMON

#define MAX_CONN_PWD_LEN        (16)  /* 连接密码的最大长度 */

/******************** PC Software Message Begin ********************/
/* Length */
#define APP_MSG_HEADER_LEN      (16)  /* 上位机软件消息帧头的长度 */
#define APP_MSG_CRC_LEN         (4)   /* 上位机软件消息CRC域的长度 */
#define MAX_APP_MSG_DATA_LEN    ((MAX_MSG_LEN) - (APP_MSG_HEADER_LEN) - (APP_MSG_CRC_LEN))
                                      /* 上位机软件消息数据域的最大长度 */


/* Header Item Index: Request */
#define APP_REQ_TYPE_INDEX      (0)   /* 类型的索引值 */
#define APP_REQ_PROTECT_INDEX   (1)   /* 保护域的索引值 */
#define APP_REQ_ATYPE_HIINDEX   (2)   /* 实际类型的索引值-高字节 */
#define APP_REQ_ATYPE_LOWINDEX  (3)   /* 实际类型的索引值-低字节 */
#define APP_REQ_STAID_INDEX     (4)   /* 控制站ID的索引值 */
#define APP_REQ_PMID_INDEX      (5)   /* 指定的PMID的索引值 */
#define APP_REQ_SQ_HIINDEX      (6)   /* 请求序号的索引值-高字节 */
#define APP_REQ_SQ_LOWINDEX     (7)   /* 请求序号的索引值-低字节 */
#define APP_REQ_LAST_INDEX      (8)   /* 是否最后一包请求的索引值 */
#define APP_REQ_LEN_HIINDEX     (14)  /* 长度的索引值-高字节 */
#define APP_REQ_LEN_LOWINDEX    (15)  /* 长度的索引值-低字节 */

/* Header Item Index: Response */
#define APP_RESP_TYPE_INDEX     (0)   /* 类型的索引值 */
#define APP_RESP_PROTECT_INDEX  (1)   /* 保护域的索引值 */
#define APP_RESP_ATYPE_HIINDEX  (2)   /* 实际类型的索引值-高字节 */
#define APP_RESP_ATYPE_LOWINDEX (3)   /* 实际类型的索引值-低字节 */
#define APP_RESP_STAID_INDEX    (4)   /* 控制器站ID的索引值 */
#define APP_RESP_SQ_HIINDEX     (5)   /* 应答序号的索引值-高字节 */
#define APP_RESP_SQ_LOWINDEX    (6)   /* 应答序号的索引值-低字节 */
#define APP_RESP_FLAG_INDEX     (7)   /* 应答标志的索引值 */
#define APP_RESP_RESPCODE_INDEX (8)   /* 第一级响应码的索引值 */
#define APP_RESP_SUBREA_INDEX   (9)   /* 第二级响应码的索引值 */
#define APP_RESP_DATAFLAG_INDEX (10)  /* 是否存在数据标志的索引值 */
#define APP_RESP_LEN_HIINDEX    (14)  /* 长度的索引值-高字节 */
#define APP_RESP_LEN_LOWINDEX   (15)  /* 长度的索引值-低字节 */

/* Header Item: Request */
#define APP_REQ_TYPE            (0xAAU)  /* 类型: 0xAA */
#define APP_REQ_PROTECT         (0xBBU)  /* 保护域: 0xBB */
#define APP_REQ_PMID            (0)      /* 指定的PMID: 0-CM自行选取 */
#define APP_REQ_LAST_FLAG       (1)      /* 是否最后一包请求: 1-最后 */

/* Header Item: Response */
#define APP_RESP_TYPE           (0xBBU)  /* 类型 */
#define APP_RESP_PROTECT        (0xAAU)  /* 保护域 */
#define APP_RESP_FLAG           (3)      /* 应答标志: 3-单消息应答 */

/* Top level Response Code-RespCode：0，Successfully；255，Failed */
#define RESPCODE_SUCCESS        (0)      /* 请求成功 */
#define RESPCODE_FAILED         (255)    /* 请求失败 */

/* Second level Response Code-subReason：0，Successfully；*/
/* 1-50: Common reason；51-255：responding to specially service. */
#define ERR_SUCCESS             (0)   /* 请求成功 */
#define ERR_SRC_ADDR            (1)   /* 源地址无效 */
#define ERR_TYPE                (2)   /* 类型无效 */
#define ERR_PROTECT             (3)   /* 保护域无效 */
#define ERR_ACTUAL_TYPE         (4)   /* 实际消息类型无效 */
#define ERR_STAID               (5)   /* 控制站ID无效 */
#define ERR_PMID                (6)   /* PMID无效 */
#define ERR_LAST_FLAG           (7)   /* 是否最后一包标志无效 */
#define ERR_LOG                 (8)   /* 未登录 */
#define ERR_ACCESS              (9)   /* 无相应访问权限 */
#define ERR_CMBUS               (10)  /* CM BUS 故障 */
#define ERR_PMBUSY              (11)  /* PM Busy PM忙 */
#define ERR_CONFIG              (12)  /* 配置参数无效 */
#define ERR_CONN_PSD            (13)  /* 登录服务：连接密码错误 */
#define ERR_DOWNLOADING         (14)  /* 正在下装：对于其他CS1131软件的命令 ，将应答此错误码 */
#define ERR_CMNOCFG             (15)  /* CM 无配置 */
#define ERR_SYSWREQTOPM         (16)  /* 将请求写入系统资源失败 - 用于发往PM模块 */
#define ERR_PARA                (50)  /* 入参出错：函数调用出错，无需应答 */

/* subReason - CS1131 */
#define ERR_CM_SRCUP_START      (51)  /* CM源文件更新开始出错 */
#define ERR_CM_SRCUP_CONT       (52)  /* CM源文件更新继续出错 */
#define ERR_CM_SRCUP_FILE_LEN   (53)  /* CM源文件更新：文件数据长度错误 */
#define ERR_READ_FILE           (54)  /* 读文件操作失败 */
#define ERR_READ_STS_TYPE       (55)  /* 读取CM及PM状态服务：请求类型错误 */
#define ERR_TOTAL_CNT           (56)  /* 定义监视变量表服务： 变量个数超限 */
#define ERR_DEF_VARIABLE        (57)  /* 定义监视变量表服务：存在出错变量 */
#define ERR_ADD_PROJVER         (58)  /* 登录服务应答：添加工程版本信息出错 */
#define ERR_ADD_STATUS          (59)  /* 读取CM/PM状态服务：添加状态信息出错 */
#define ERR_ADD_ONLINE_DATA     (60)  /* 读变量在线值服务：添加变量在线值出错 */
#define ERR_SRCUP_INIT          (61)  /* 与源文件更新相关的资源初始化失败 */

/* subReason - OPC */
#define ERR_OPC_VER             (71)  /* 登录服务中OPC配置信息版本错误 */
#define ERR_ADD_OPCONLINE_DATA  (72)  /* 读变量服务：添加OPC变量在线值出错 */

/* subReason - Client */
#define ERR_CLI_PMID            (81)  /* PMID错误 */
#define ERR_ADD_DIAG            (82)  /* 添加诊断信息错误 */
#define ERR_ADD_VERN            (83)  /* 添加版本信息错误 */
#define ERR_EVENT_MID           (84)  /* 读(CM)事件服务：模块ID错误 */
#define ERR_EVENT_INDEX         (85)  /* 读(CM)事件服务：事件索引值错误 */
#define ERR_ADD_EVENT           (86)  /* 添加CM事件错误 */

#define ERR_FWUP_INIT           (87)  /* 与固件更新相关的资源初始化失败 */
#define ERR_FWPRG_TYPE          (88)  /* 固件更新：程序类型错误 */
#define ERR_FWUPCONT_PACKNUM    (89)  /* 固件更新继续服务：子包号出错 */
#define ERR_CM_RTSUP_START      (90)  /* CM RTS更新开始出错 */
#define ERR_CM_RTSUP_CONT       (91)  /* CM RTS更新继续出错 */
#define ERR_CM_RTSCRCUP_START   (92)  /* CM RTSCRC更新开始出错 */
#define ERR_CM_RTSCRCUP_CONT    (93)  /* CM RTSCRC更新继续出错 */
#define ERR_CM_RTSCRCUP_END     (94)  /* CM RTSCRC更新完成错误 */
#define ERR_CM_CHG_RTS_INDEX    (95)  /* 更新RTS索引值失败 */

#define ERR_CM_FPGAUP_START     (96)  /* CM FPGA更新开始出错 */
#define ERR_CM_FPGAUP_CONT      (97)  /* CM FPGA更新继续出错 */
#define ERR_CM_CHG_FPGA_INDEX   (98)  /* 更新FPGA索引值失败 */

#define ERR_CM_FWUP_OPR         (99)  /* CM固件更新：操作类型错误 */
#define ERR_CM_FWUP_FILE_LEN    (100) /* CM固件更新：文件数据长度错误 */

#define ERR_FWUP_BUSY           (101) /* CM固件更新：CM Busy，即升级包下发速度快，CM处理不过来 */
#define ERR_CM_FWUP_FILE_CRC    (102) /* CM固件更新：文件CRC错误 */
#define ERR_SEND_OPR_MSG        (103) /* CM固件更新：发送操作消息失败 */
#define ERR_SENDOPRMSG_NOT_FIN  (104) /* CM固件更新：操作消息未发送完成 */

/* Data Flag: If Data Area is Exist */
#define DATA_NOT_EXIST          (0)   /* 不存在 */
#define DATA_EXIST              (1)   /* 存在 */

/* Login Service */
#define PSD_LEN_INDEX           (20)  /* 密码长度索引值 */
#define PSD_INDEX               (24)  /* 密码首字节索引值 */

/******************** PC Software Message End ********************/

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* Response  应答模块类型枚举定义 */
typedef enum RespModuleTypeTag
{
    PM_RESP = 0,  /* PM Response  PM应答 */
    CM_RESP,      /* CM Response  CM应答 */
    NONE_RESP     /* None Response 无模块应答 */
}RespModuleType_t;

/* Log Status  登录状态枚举定义 */
typedef enum LogStatusTag
{
    LOGOUT = 0,   /* Logout  未登录 */
    LOGIN         /* Login   登录 */
}LogStatus_t;

/* Response Message 应答消息结构体定义 */
typedef struct RespMsgTag
{
    uint16_t usLen;                 /* Length 消息长度 */
    uint8_t ucBuff[MAX_MSG_LEN];    /* Buffer 缓冲区，用于存放消息内容 */
}RespMsg_t;

/* Access Control Info. 访问控制信息结构体定义 */
typedef struct AccCtrlInfoTag
{
    uint8_t ucIndex;       /* Index 当前使用的配置参数的索引值 */
    uint8_t ucReserve[3];  /* Reserve 预留 */
    uint32_t uiCMCfgCRC;   /* CM配置文件CRC */
    uint32_t uiCMCfgVer;   /* CM配置文件的版本 */
    AccessCtrlConfigInfo_t stAccCtrl[LOCAL_CFG_VER_CNT];  /* Access Control Configuration Info. 访问控制配置信息 */
}AccCtrlInfo_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GetReqActualType
*
* 功能描述: 获取请求帧实际类型
*
* 输入参数: pucReq：指向请求帧
*
* 输出参数: 无
*
* 返 回 值: 请求帧的实际类型
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetReqActualType( uint8_t const pucReq[]);

/*
***********************************************************************************************************************
* 函数名称: InitAccessCtrlInfo
*
* 功能描述: 初始化本地保存的访问控制信息
*
* 输入参数: pstAccCtrlInfo：指向本地保存的访问控制信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitAccessCtrlInfo( AccCtrlInfo_t *pstAccCtrlInfo );

/*
***********************************************************************************************************************
* 函数名称: HandleAccessCtrlInfo
*
* 功能描述: 访问控制信息版本变化时，拷贝到本地并切换
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 访问控制信息是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleAccessCtrlInfo( AccCtrlInfo_t *pstAccCtrlInfo );


#endif /*PCSWCOMMON_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


