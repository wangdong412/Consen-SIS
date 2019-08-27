/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 通讯模块
*
*  文件名称    : comm.h
*
*  功能简介    : 完成对工程文件的配置
*
*  作者           : 刘阳
*
*  创建日期    : 2015-12-30
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef COMM_H_
#define COMM_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include"../../../../PM_FW_COMMON/inc/DataType.h"
#include"../../../../PM_FW_COMMON/Srv/SharedMem/inc/SharedComm.h"
#include"../../../../PM_FW_COMMON/Srv/SharedMem/inc/SharedConfig.h"
//#include"../../../../PM_FW_COMMON/Srv/SafetyProtocol/inc/P2PSafetyComm.h"
#include"Srv/SysResource/inc/Sys.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"
/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
//#define DEBUG_OUT_COMM
#ifdef DEBUG_OUT_COMM
#define DEBUG_COMM(format, args...)  printf ("DEBUG_COMM:  ") ; printf(format, ##args)
#else
#define DEBUG_COMM(format, args...)
#endif




#define CS1131_ACK_TIMEOUT          (10U)       /*undetermined!!!*/
#define OPC_ACK_TIMEOUT             (10U)       /*undetermined!!!*/
#define MODBUS_ACK_TIMEOUT          (10U)       /*undetermined!!!*/
#define P2P_ACK_TIMEOUT             (10U)       /*undetermined!!!*/

/******************** 通信服务相关开始 ********************/
/* Request Message Type Range */
#define CS1131_ATYPE_MIN            (1001)      /* Minimum */
#define CS1131_ATYPE_MAX            (1500)      /* Maximum */

/* Request Message Type：CM Handle */
#define CS1131_LOGIN                (1002)      /* 登录服务 */
#define CS1131_SET_PASSWORD         (1004)      /*Set password */
#define CS1131_READ_PASSWORD        (1005)      /*Read password */
#define CS1131_CHECK_PASSWORD       (1006)      /*Check password */
#define CS1131_CLEAR_PASSWORD       (1007)      /*Clear password */
#define CS1131_CLR_SRCFILE          (1027)      /* 清除源文件服务 */
#define CS1131_LOGOUT               (1028)      /* 退出登录服务 */
#define CS1131_READ_STATUS          (1029)      /* 读取CM及PM状态服务 */
#define CS1131_DEF_VARTABLE         (1030)      /* 定义监视变量表服务 */
#define CS1131_READ_VARONLINE       (1031)      /* 读变量在线值服务 */
#define CS1131_READ_STAID           (1036)      /* 读取站节点服务 */



/* Request Message Type：PM Handle */
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
#define OPC_READ_VAR                (2001)
#define OPC_WRITE_VAR               (2002)
#define OPC_READ_CLOCK              (2003)
#define OPC_WRITE_CLOCK             (2004)
#define OPC_ADJUST_CLOCK            (2005)
#define OPC_READ_VERN               (2006)

/*SOE service ID*/
#define SOE_READ_RECORD             (1501)
#define SOE_DELETE_RECORD           (1502)





#define CHKSUM_CRC_SIZE             (sizeof(uint32_t))

#define ACK_FRAME_MID               (0)
#define ACK_FRAME_FIRST             (1)
#define ACK_FRAME_LAST              (2)
#define ACK_FRAME_SINGLE            (3)

/* Top level Response Code-ResCode 0 :Successfully; 255:Failed */
#define RESCODE_SUCCESS             (0U)
#define RESCODE_FAILED              (255U)

/* Second level Response Code-subReason; 0:Successfully*/
#define ERR_SUCCESS                 (0)
#define ERR_SRC_ADDR                (1)     /*SRC addr is error(NULL)*/
#define ERR_ACK_ADDR                (15)    /*ACK buffer addr is error*/
#define ERR_MSG_LEN                 (16)    /*MSG len is error*/
#define ERR_MSG_CRC                 (17)    /*MSG CRC is error*/
#define ERR_GET_PROINFO             (51)
#define ERR_GET_FORCELIST           (52)
#define ERR_WRITE_FILE_START        (53)
#define ERR_WRITE_FILE_CONTINUE     (54)
#define ERR_WRITE_FILE_END          (55)
#define ERR_READ_FILE_START         (56)
#define ERR_READ_FILE_CONTINUE      (57)

#define ERR_OPC_READ_CLOCK          (101)

#define ERR_SOE_READ                (111)
#define ERR_SOE_CLEAR               (112)





#define FS_OPERATE_TIMEOUT          (10)    /* file operate timeout */







#define MAX_READ_FILE_LEN           (4*1024)
#define MAX_WRITE_FILE_LEN          (4*1024)

#define MAX_CPNAME_LEN              (16)

#define MODBUS_HEAD_LEN             (28)

#define P2P_REQ_FRAME               (0)
#define P2P_RES_FRAME               (1))
#define P2P_WAITACK_CYCLE           (100)      /*undetermined!!!*/
#define P2P_SEND_BUFF_SIZE          (8*1024)

#define MAX_SOE_READ_NUM            (4000U)    /*undetermined!!!*/

#define RTS_SC_FILE                 "DEFAULT.SC"        /* SC 文件               */
#define RTS_SC_FILE_TMP             "DEFAULT.SC.TMP"    /* 临时 SC 文件          */
#define RTS_SC_FILE_OLD             "DEFAULT.SC.OLD"    /* 旧 SC 文件          */
#define RTS_PRG_FILE                "DEFAULT.PRG"       /* PRG 文件               */
#define RTS_PRG_FILE_TMP            "DEFAULT.PRG.TMP"   /* 临时 PRG 文件          */
#define RTS_PRG_FILE_OLD            "DEFAULT.PRG.OLD"   /* 旧 PRG 文件          */
#define RTS_IOC_FILE                "DEFAULT.IOC"       /* IOC 文件               */
#define RTS_IOC_FILE_TMP            "DEFAULT.IOC.TMP"   /* 临时 IOC 文件          */
#define RTS_IOC_FILE_OLD            "DEFAULT.IOC.OLD"   /* 旧 IOC 文件          */
#define RTS_SOE_FILE                "DEFAULT.SOE"       /* SOE 文件               */
#define RTS_SOE_FILE_TMP            "DEFAULT.SOE.TMP"   /* 临时 SOE 文件          */
#define RTS_SOE_FILE_OLD            "DEFAULT.SOE.OLD"   /* 旧 SOE 文件          */
#define RTS_P2P_FILE                "DEFAULT.P2P"       /* P2P 文件               */
#define RTS_P2P_FILE_TMP            "DEFAULT.P2P.TMP"   /* 临时 P2P 文件          */
#define RTS_P2P_FILE_OLD            "DEFAULT.P2P.OLD"   /* 旧 P2P 文件          */
#define RTS_MMC_FILE                "DEFAULT.MMC"       /* MMC 文件               */
#define RTS_MMC_FILE_TMP            "DEFAULT.MMC.TMP"   /* 临时 MMC 文件          */
#define RTS_MMC_FILE_OLD            "DEFAULT.MMC.OLD"   /* 旧 MMC 文件          */
#define RTS_MSC_FILE                "DEFAULT.MSC"       /* MSC 文件               */
#define RTS_MSC_FILE_TMP            "DEFAULT.MSC.TMP"   /* 临时 MSC 文件          */
#define RTS_MSC_FILE_OLD            "DEFAULT.MSC.OLD"   /* 旧 MSC 文件          */
#define RTS_VOT_FILE                "DEFAULT.VOT"       /* VOT 文件               */
#define RTS_VOT_FILE_TMP            "DEFAULT.VOT.TMP"   /* 临时 VOT 文件          */
#define RTS_VOT_FILE_OLD            "DEFAULT.VOT.OLD"   /* 旧 VOT 文件          */
#define RTS_RTN_FILE                "DEFAULT.RTN"       /* RTN 文件               */
#define RTS_RTN_FILE_TMP            "DEFAULT.RTN.TMP"   /* 临时 RTN 文件          */
#define RTS_RTN_FILE_OLD            "DEFAULT.RTN.OLD"   /* 旧 RTN 文件          */
#define RTS_OPC_FILE                "DEFAULT.OPC"       /* OPC 文件               */
#define RTS_OPC_FILE_TMP            "DEFAULT.OPC.TMP"   /* 临时 OPC 文件          */
#define RTS_OPC_FILE_OLD            "DEFAULT.OPC.OLD"   /* 旧 OPC 文件          */
#define RTS_CM_FILE                 "DEFAULT.CM"        /* CM 文件               */
#define RTS_CM_FILE_TMP             "DEFAULT.CM.TMP"    /* 临时 CM 文件          */
#define RTS_CM_FILE_OLD             "DEFAULT.CM.OLD"    /* 旧 CM 文件          */


#define RTS_BOOT_BIN                "SGM201_Boot_B.bin"   /* boot文件             */
#define RTS_UPDATE_BIN              "SGM201_Update_B.bin" /* 更新程序文件         */



/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

typedef enum
{
    FREE = 0x00,
    BUSY
}Service_Handle_Flag_t;

typedef enum
{
    FILE_HANDLE_FREE = 0x00,
    FILE_HANDLE_BUSY
}File_Handle_Flag_t;
/*服务数据类型*/
//typedef enum
//{
//    CS1131_DATA = 0x00,
//    OPC_DATA,
//    MODBUS_DATA,
//    P2P_DATA,
//    SOE_DATA,
//    AMS_DATA,
//    DIAG_DATA,
//    INTER_CMD_DATA,
//    TIMING_DATA,
//    CLIENT_DATA
//}Service_Type_t;

/*文件操作阶段*/
typedef enum
{
    FOS_START = 0x00,
    FOS_WAIT_ACK
}FileOperateStag_t;

/*文件操作结果*/
typedef enum
{
    NOT_COMPLETE = 0x00,
    SUCCESS,
    FILE_EXIST,
    FILE_NOT_EXIST,
    TOME_OUT,
    FAIL
}FileOptRest_t;

/*写文件服务处理阶段定义*/
typedef enum
{
    FWRITE_START = 0x00,                    /*写文件服务处理阶段：开始*/
    FWRITE_DETER_TMPFILE_EXIST,             /*写文件服务处理阶段：判断临时文件是否存在*/
    FWRITE_OPEN_TMPFILE,                    /*写文件服务处理阶段：打开临时文件*/
    FWRITE_ERASE_TMPFILE,                   /*写文件服务处理阶段：擦除临时文件*/
    FWRITE_WRITE_TMPFILE,                   /*写文件服务处理阶段：写临时文件*/
    FWRITE_WRITE_TMPFILEDONE,               /*写文件服务处理阶段：写临时文件完成*/
    FWRITE_RENAME_TMPFILE,                  /*写文件服务处理阶段：重命名临时文件*/
    FWRITE_CLOSE_TMPFILE,                   /*写文件服务处理阶段：关闭临时文件*/
    FWRITE_DELETE_TMPFILE,                  /*写文件服务处理阶段：删除临时文件*/
    FWRITE_DETER_FORMFILE_EXIST,            /*写文件服务处理阶段：判断正式文件是否存在*/
    FWRITE_RENAME_FORMFILE,                 /*写文件服务处理阶段：重命名正式文件*/
    FWRITE_CLOSE_FORMFILE,                  /*写文件服务处理阶段：关闭正式文件*/
    FWRITE_DELETE_FORMFILE,                 /*写文件服务处理阶段：关闭删除文件*/
    FWRITE_END                              /*写文件服务处理阶段：写文件结束*/
        
}FileWriteStag_t;
/*读文件继续标志*/
typedef enum
{
    CONT_TRANS = 0x00,
    TRANS_DONE

}ReadFileContFlag_t;

typedef enum
{
    MODBUS_MASTER = 0x00,
    MODBUS_SLAVE
}ModbusType_t;

typedef enum
{
    NOT_WAIT_ACK = 0x00,
    WAIT_ACK

}WaitACKFlag;

typedef enum
{
    NOT_WAIT_SEND = 0x00,
    WAIT_SEND

}WaitSendFlag;

typedef enum
{
    NOT_NEED_RETRY = 0x00,
    NEED_RETRY

}NeedRetryFlag;

typedef enum
{
    CMD_NONE = 0x00,
    CMD_REQ_ALL_CMCONFIG,
    CMD_REQ_INCRE_CMCONFIG
}InterCMD_t;

#pragma pack (1)

typedef struct MSGHeadTag
{
    uint8_t ucType;                      /*0xAA*/
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



typedef struct ModbusHeadTag
{
    ModbusType_t emType;
    uint8_t  ucReserve1;
    uint16_t usDataLen;
#if emType
    uint8_t ucCMSlotID;
    uint8_t ucPortID;
    uint8_t  ucReserve2;
    uint32_t uiBlockID;
    uint32_t uiProjectID;
    uint32_t uiMainVern;
    uint32_t uiMinorVern;
    uint32_t uiProjectCRC;
#else
    uint8_t ucReserved2[4];
    uint32_t uiBlockID;
    uint16_t usBeginAddr;
    uint8_t ucReserved3[14];
#endif
}ModbusHead_t;

typedef struct ProjectInfoTag
{
    
}ProjectInfo_t;

typedef struct ForceListTag
{
    
}ForceList_t;

typedef struct FileWriteInfoTag
{
    uint8_t ucFileType;
    uint8_t ucFileID;
    uint16_t usFileDataLen;
}FileWriteInfo_t;

typedef struct FileReadInfoTag
{
    uint8_t ucFileType;
    uint8_t ucFileID;
    uint16_t usFileDataLen;
}FileReadInfo_t;

typedef struct FileInfoTag
{
    SysCfgFileType_t emFileType;                        /*文件类型*/
    int32_t iFid;                                       /*文件描述符*/
    uint16_t usFileNameLen;                             /*文件名长度*/
    uint8_t ucReserved[2];
    uint8_t ucFileName[MAX_FILE_NAME_LEN];              /*正式文件名*/
    uint8_t ucTmpFileName[MAX_FILE_NAME_LEN];           /*临时文件名*/
    uint8_t ucOldFileName[MAX_FILE_NAME_LEN];           /*旧文件名*/
    uint32_t uiFileDataLen;                             /*文件数据长度*/
    uint32_t uiFileWriteIndex;                          /*文件写操作索引*/
    uint32_t uiFileReadIndex;                           /*文件读操作索引*/
}FileInfo_t;


typedef struct ContrlerClockTag
{
    uint32_t uiSec;
    uint16_t usMilliSec;
}ContrlerClock_t;


typedef struct CPStatusTag
{
    uint32_t uiVern;
    uint16_t usCPNameLen;
    uint8_t ucCPName[MAX_CPNAME_LEN];
}CPStatus_t;

typedef struct P2PRecvHeadTag
{
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve1;
    uint16_t usTotalLen;
    uint8_t ucPMInfo;
    uint8_t ucReserve2;

}P2PRecvHead_t;

typedef struct P2PSendHeadTag
{
    uint8_t ucSrcCMID;
    uint8_t ucSrcNETID;
    uint8_t ucReserve[2];
    uint32_t uiDstIPAddr;
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve2;
}P2PSendHead_t;



typedef struct P2PSendCtrlTag
{
    ActiveFlag_t emSendBlockActive;         /* 本发送块是否被配置使用*/
    uint32_t uiBlockID;                     /* 块号*/
    uint16_t usSendCount;
    uint16_t usWaitACKCount;
    uint8_t ucDestStaID;                    /*目标站号*/
    WaitSendFlag emWaitSendFlag;
    NeedRetryFlag emNeedRetryFlag;
    WaitACKFlag emWaitACKFlag;

}P2PSendCtrl_t;

typedef struct SOEReadReqTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /* 1~16*/
    uint16_t usGetIndex;
    uint8_t ucAckFlag;
    uint32_t uiWrapCount;
    uint32_t uiGeneration;
}SOEReadReq_t;

typedef struct SOEReadRspTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /* 1~16*/
    uint16_t usNextIndex;
    uint16_t usEntriNum;
    uint32_t uiWrapCount;
    uint32_t uiGeneration;
}SOEReadRsp_t;

typedef struct SOEDeleteReqTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /*1~16*/
}SOEDeleteReq_t;

typedef struct InterCMDHeadTag
{
    PMController_t emPM_ID;                 /*目的PMid*/
    CMSlot_t emCMSlotID;                    /*来源CM的槽位号*/
    InterCMD_t emInterCMD;                  /*内部命令id*/
    uint8_t ucReserved;                     /*预留*/
}InterCMDHead_t;


typedef struct ConfigUpdateFlagTag
{
    File_ID_t emFileID;
    bool_t bFileUpdateFlag;

}ConfigUpdateFlag_t;

typedef struct ConfigInfoTablTag
{
    ConfigUpdateFlag_t emConfigUpdateFlg[MAX_FILE_NUM];
}ConfigInfoTabl_t;


typedef struct TimeFlagTag
{
    uint32_t uiSec;                         /* the number of second */
    uint32_t uiMilliSec;                    /* the number of millisecond */
}TimeFlag_t;

/*软SOE记录结构*/
typedef struct SWSoeRecordTag
{
    uint8_t ucSOEBlodckID;                  /* SOE块号 */
    uint8_t ucIndex;                        /* SOE块内索引号 */
    uint8_t ucChFlag;                       /* 变位标识 */
    uint8_t ucReserve;                      /* 保留 */
    TimeFlag_t stTimeStamp;                 /* 时钟 */
}SWSoeRecord_t;

/*硬SOE记录结构*/
typedef struct HWSoeRecordTag
{
    uint8_t ucModAddr;                      /* 硬件模块站号 */
    uint8_t ucChNum;                        /* 通道号 */
    uint8_t ucChFlag;                       /* 通道变位标识 */
    uint8_t ucReserve;                      /* 保留 */
    TimeFlag_t stTimeStamp;                 /* 时钟 */
}HWSoeRecord_t;





#pragma pack ()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
********************************函数描述*******************************************************************************
* 函数名称: CommInit
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

void CommInit(void);

/*
********************************函数描述*******************************************************************************
* 函数名称: CommCycle
*
* 功能描述: 模块周期执行函数
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

void CommCycle(void);

/*
********************************函数描述*******************************************************************************
* 函数名称: CommClearP2PWaitAckFlg
*
* 功能描述: 清除P2P发送块的等待应答标志
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
void CommClearP2PWaitAckFlg(uint32_t uiP2PBlockNum);


#endif /*COMM_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
