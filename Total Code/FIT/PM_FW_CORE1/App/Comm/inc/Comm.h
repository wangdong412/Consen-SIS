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
#include "inc/DataType.h"
#include "Srv/SharedMem/inc/SharedComm.h"
#include "Srv/SharedMem/inc/SharedConfig.h"
#include "Srv/SharedMem/inc/SharedSOE.h"
#include "Srv/SafetyProtocol/inc/P2PSafetyComm.h"
#include "Srv/Log/inc/Log.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"
#include "App/Main/inc/Task.h"
#include "Srv/SysResource/inc/SysUpdate.h"
/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define DEBUG_OUT_COMM
#ifdef DEBUG_OUT_COMM
#ifdef CONTROL_CYCLE_20MS
#define DEBUG_COMM(format, args...)  printf ("D_COMM: ") ; printf(format, ##args)
#endif

#ifdef CONTROL_CYCLE_5MS
#define DEBUG_COMM(format, args...)
#endif

#else
#define DEBUG_COMM(format, args...)
#endif

#define CS1131_ACK_TIMEOUT          (20U)       /*undetermined!!!*/
#define OPC_ACK_TIMEOUT             (20U)       /*undetermined!!!*/
#define MODBUS_ACK_TIMEOUT          (20U)       /*undetermined!!!*/
#define P2P_ACK_TIMEOUT             (20U)       /*undetermined!!!*/
#define CTRL_ACK_TIMEOUT            (100U)      /*undetermined!!!*/
#define AMS_ACK_TIMEOUT             (20U)       /*undetermined!!!*/

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
#define CS1131_DISABLE_VAR          (1018)
#define CS1131_READ_DISABLE_TABL    (1019)
#define CS1131_OSC_START            (1033)      /* 启动示波器服务 */
#define CS1131_OSC_DONE             (1034)      /* 示波器完成服务 */
#define CS1131_OSC_GET_VAR          (1035)      /* 获取示波器存储数据服务 */
#define CS1131_DOWNLOAD_COMFIR      (1039)      /* 清空/增量下装确认服务*/

/* Request Message Type：PM/CM Handle */
#define CS1131_WRITEFILE_START      (1021)      /* 写文件开始服务 */
#define CS1131_WRITEFILE_CONT       (1022)      /* 写文件继续服务 */
#define CS1131_WRITEFILE_END        (1023)      /* 写文件完成服务 */
#define CS1131_READFILE_START       (1025)      /* 读文件开始服务 */
#define CS1131_READFILE_CONT        (1026)      /* 读文件继续服务 */


/* OPC service ID */
/* Request Message Type：CM Handle */
#define OPC_LOGIN                   (2001)      /* 登录服务 */
#define OPC_LOGOUT                  (2002)      /* 退出登录服务 */
#define OPC_READ                    (2003)      /* 读变量服务 */
/* Request Message Type：PM Handle */
#define OPC_WRITE                   (2004)      /* 写变量服务*/

/*SOE service ID*/
#define SOE_READ_RECORD             (1503)
#define SOE_DELETE_RECORD           (1504)

/*DAIG service ID*/
#define LOG_READ_RECORD             (2505)

#define FW_UPDATE_START             (2510)
#define FW_UPDATE_CONTINUE          (2511)
#define FW_UPDATE_DONE              (2512)


/* Request Message Type：PM Handle */
#define AMS_SCANIO                  (4004)  /* 探测I/O模块 */
#define AMS_DWHARTCMD               (4005)  /* 下发Hart命令 */

#define AMS_SCAN_ALL                (0x00)
#define AMS_SCAN_SINGLE             (0x01)




#define CHKSUM_CRC_SIZE             (sizeof(uint32_t))

#define ACK_FRAME_MID               (0)
#define ACK_FRAME_FIRST             (1)
#define ACK_FRAME_LAST              (2)
#define ACK_FRAME_SINGLE            (3)

/* Top level Response Code-ResCode 0 :Successfully; 255:Failed */
#define RESCODE_SUCCESS             (0U)    /*请求成功*/
#define RESCODE_FAILED              (255U)  /*请求失败*/

/* Second level Response Code-subReason; 0:Successfully*/
#define ERR_SUCCESS                 (0)
#define ERR_SRC_ADDR                (1)     /*SRC addr is error(NULL)*/
#define ERR_ACK_ADDR                (15)    /*ACK buffer addr is error*/
#define ERR_MSG_LEN                 (16)    /*MSG len is error*/
#define ERR_MSG_CRC                 (17)    /*MSG CRC is error*/
#define ERR_GET_PROINFO             (51)
#define ERR_GET_FORCELIST           (52)
#define ERR_WRITE_FILE_START        (53)    /*写文件开始失败*/
#define ERR_WRITE_FILE_CONTINUE     (54)    /*写文件继续失败*/
#define ERR_WRITE_FILE_END          (55)    /*写文件完成失败*/
#define ERR_READ_FILE_START         (56)    /*读文件开始失败*/
#define ERR_READ_FILE_CONTINUE      (57)    /*读文件继续失败*/
#define ERR_WRITE_VAR               (58)    /*写变量错误（变量个数）*/
#define ERR_FORCE_LIST              (59)    /*强制列表错误（变量个数）*/
#define ERR_DISABLE_TABL_LEN        (60)    /*强制列表长度错误*/
#define ERR_DISABLE_TABL_CRC        (61)    /*强制列表CRC错误*/
#define ERR_TASK_ID                 (62)    /*调试命令下发任务ID错误*/
#define ERR_WRITE_VAR_ITERM         (63)    /*写变量错误（单个变量信息条目）*/
#define ERR_DISABLE_ITERM           (64)    /*强制列表错误（单个变量信息条目）*/
#define ERR_WRTIE_VAR_TASK_STOP     (65)    /*写变量服务中有变量所属的任务处于STOP状态*/
#define ERR_OSCP_GET_VAR            (66)
#define ERR_OSCP_START              (67)
#define ERR_OSCP_DONE               (68)
#define ERR_NOT_PROGRAM             (99)    /*钥匙开关不在PROG*/

#define ERR_OPC_WRITE_VAR           (100)   /*OPC写变量错误*/
#define ERR_OPC_READ_CLOCK          (101)

#define ERR_CTRL_PAUSE              (110)
#define ERR_CTRL_CONTINUE           (111)

#define ERR_SOE_READ                (120)   /*读SOE处理失败*/
#define ERR_SOE_CLEAR               (121)   /*清空SOE失败*/

#define ERR_LOG_READ                (130)   /*读日志处理失败*/

#define ERR_FW_WRITE_START          (141)
#define ERR_FW_WRITE_CONTINUE       (142)
#define ERR_FW_WRITE_END            (143)

#define ERR_AMS_TIMEOUT             (200)
#define ERR_AMS_MODADDR_ERR         (201)
#define ERR_AMS_CHANNEL_ERR         (202)
#define ERR_AMS_APPEND_ERR          (203)

#define ERR_HANDLE_NOT_DONE         (0xff)




//#define FS_OPERATE_TIMEOUT          (200)    /* file operate timeout */
#define FS_OPERATE_TIMEOUT           (3000)    /* file operate timeout */






#define MAX_READ_FILE_LEN           (4*1024)
#define MAX_WRITE_FILE_LEN          (4*1024)

#define MAX_CPNAME_LEN              (16)

#define MODBUS_HEAD_LEN             (28)

#define P2P_REQ_FRAME               (0)
#define P2P_RES_FRAME               (1)
#define P2P_WAITACK_CYCLE           (2000/5)      /*undetermined!!!*/
#define P2P_SEND_BUFF_SIZE          (8*1024)

#define MAX_SOE_READ_NUM            (50U)

#define MAX_LOG_READ_NUM            (100U)

#define SLAVE_OFFLINE               (0x00)
#define SLAVE_ONLINE                (0x01)

#define MAX_RACK_NUM                (15)

#define NEED_SEND_ACK               (1U)
#define NOT_NEED_SEND_ACK           (0U)

#define CM_FW                       (0x00)
#define PM_FW                       (0x01)
#define IO_FW                       (0x02)

#define FW_TYPE_FPGA                (0x00)
#define FW_TYPE_IO                  (0x01)
#define FW_TYPE_RTS                 (0x02)


//#define FW_IO_BASE_ADDR             (66*1024*1024)

#define NOT_HAS_FW_UPDATE           (0U)
#define HAS_IO_FW_UPDATE            (1U)
#define HAS_RTS_UPDATE              (2U)
#define HAS_FPGA_UPDATE             (3U)
#define NEED_ERASE_IO_FW            (4U)
#define NEED_ERASE_RTS              (5U)
#define NEED_ERASE_FPGA             (6U)

#define MAX_FW_FILE_NAME_LEN        (128)

#define MAX_FW_PACK_LEN             (480)

#define MAX_NORFLASH_PACK_LEN       (300)

#define LAST_PACK_FLG               (0xaa)

#define READ_FILE_TRANS_DONE        (0x00)
#define READ_FILE_TRANS_CONT        (0x01)



//#define FW_FM_RTS_FLG_BLK_NUM       (1U)
//#define FW_FM_FPGA_FLG_BLK_NUM      (1U)
//#define FW_FM_RTS_BLK_NUM           (40U)
//#define FW_FM_FPGA_BLK_NUM          (40U)

//#define FW_FM_RTS_FLG_START_BLK     (0U)
//#define FW_FM_FPGA_FLG_START_BLK    (FW_FM_RTS_FLG_START_BLK + FW_FM_RTS_FLG_BLK_NUM)
//#define FW_FM_RTS_AREA0_START_BLK   (FW_FM_FPGA_FLG_START_BLK + FW_FM_FPGA_FLG_BLK_NUM)
//#define FW_FM_RTS_AREA1_START_BLK   (FW_FM_RTS_AREA0_START_BLK + FW_FM_RTS_BLK_NUM)
//#define FW_FM_FPGA_AREA0_START_BLK  (FW_FM_RTS_AREA1_START_BLK + FW_FM_RTS_BLK_NUM)
//#define FW_FM_FPGA_AREA1_START_BLK  (FW_FM_FPGA_AREA0_START_BLK + FW_FM_FPGA_BLK_NUM)

#define NOR_FLASH_BASE_ADDR         (0xEC000000)
#define FW_FM_RTS_FLG_START_ADDR    (NOR_FLASH_BASE_ADDR)
#define FW_FM_FPGA_FLG_START_ADDR   (FW_FM_RTS_FLG_START_ADDR + (128*1024))
#define FW_FM_CORE0_CRC_START_ADDR1 (FW_FM_FPGA_FLG_START_ADDR + (128*1024))
#define FW_FM_CORE0_START_ADDR1     (FW_FM_CORE0_CRC_START_ADDR1 + (128*1024))
#define FW_FM_CORE1_CRC_START_ADDR1 (FW_FM_CORE0_START_ADDR1 + (15*128*1024))
#define FW_FM_CORE1_START_ADDR1     (FW_FM_CORE1_CRC_START_ADDR1 + (128*1024))
#define FW_FM_CORE0_CRC_START_ADDR2 (FW_FM_CORE1_START_ADDR1 + (23*128*1024))
#define FW_FM_CORE0_START_ADDR2     (FW_FM_CORE0_CRC_START_ADDR2 + (128*1024))
#define FW_FM_CORE1_CRC_START_ADDR2 (FW_FM_CORE0_START_ADDR2 + (15*128*1024))
#define FW_FM_CORE1_START_ADDR2     (FW_FM_CORE1_CRC_START_ADDR2 + (128*1024))
#define FW_FM_FPGA_START_ADDR1      (FW_FM_CORE1_START_ADDR2 + (23*128*1024))
#define FW_FM_FPGA_START_ADDR2      (FW_FM_FPGA_START_ADDR1 + (40*128*1024))


#define FW_FM_RTS_FLG_MAX_LEN       (128*1024)
#define FW_FM_CORE0_CRC_MAX_LEN     (128*1024)
#define FW_FM_CORE0_RTS_MAX_LEN     (15*128*1024)
#define FW_FM_CORE1_CRC_MAX_LEN     (128*1024)
#define FW_FM_CORE1_RTS_MAX_LEN     (23*128*1024)


/*用于内存中存储文件*/
#define FW_FM_CORE0_CRC_START_OFF   (0)
#define FW_FM_CORE0_START_OFF       (FW_FM_CORE0_CRC_START_OFF + (128*1024))
#define FW_FM_CORE1_CRC_START_OFF   (FW_FM_CORE0_START_OFF + (15*128*1024))
#define FW_FM_CORE1_START_OFF       (FW_FM_CORE1_CRC_START_OFF + (128*1024))
#define FW_FM_FPGA_START_OFF        (0)


#define FW_AREA_0                   (1U)
#define FW_AREA_1                   (2U)


#define NORFLASH_OPT_TIMEOUT        (10000)    /* Norflash operate timeout */




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
    FILE_WRITE_START,
    FILE_WRITE_CONTINUE,
    FILE_WRITE_DONE,
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

///*读文件继续标志*/
//typedef enum
//{
//    CONT_TRANS = 0x00,
//    TRANS_DONE
//
//}ReadFileContFlag_t;

typedef enum
{
    MODBUS_MASTER = 0x00,
    MODBUS_SLAVE
}ModbusType_t;

typedef enum
{
    NOT_WAIT_ACK = 0x00,
    WAIT_ACK

}WaitACKFlag_t;

typedef enum
{
    NOT_WAIT_SEND = 0x00,
    WAIT_SEND

}WaitSendFlag_t;

typedef enum
{
    FIRST_SEND = 0x00,
    RETRY_SEND

}RetryFlag_t;

typedef enum
{
    ACK_NOT_TIMEOUT = 0x00,
    ACK_TIMEOUT

}ACKTimeoutFlag_t;

typedef enum
{
    CMD_NONE = 0x00,
    CMD_REQ_ALL_CMCONFIG,
    CMD_REQ_INCRE_CMCONFIG
}InterCMD_t;

typedef enum 
{
    AMS_START = 0x00000000,
    AMS_WR,
    AMS_RECV_WR_ACK,
    AMS_RD,
    AMS_RECV_RD_ACK,
    AMS_END
    
}AMSMsgHandleStag_t;

typedef enum 
{
    RSV_RD = 0x00000000,
    RSV_RECV_RD_ACK,
    RSV_END
    
}ReadSlaveVernStag_t;

typedef enum 
{
    RA_RD = 0x00000000,
    RA_RECV_RD_ACK,
    RA_END
    
}ReadAlarmStag_t;


typedef enum 
{
    UPIO_WR_START = 0x00000000,
    UPIO_WR_START_ACK,
    UPIO_WR_CONTINUE,
    UPIO_WR_CONTINUE_ACK,
    UPIO_RD_STATUS,
    UPIO_RD_STATUS_ACK,
    UPIO_RD_WAIT,
    UPIO_END,
    
}UpdateIOStag_t;

typedef enum
{
    AH_FREE = 0x00000000,
    AH_AMS,
    AH_IO_FW,
    AH_SLAVE_VERN,
    AH_SLAVE_ALARM,
}AperiodicHandleFlg_t;

typedef enum
{
    FM_UPDATE_START = 0x00000000,
    FM_UPDATE_ERASE_FLG,
    FM_UPDATE_ERASE_FW,
    FM_UPDATE_WRITE_FW,
    FM_UPDATE_WRITE_FLG,
    FM_UPDATE_END,
}FMUpdateStag_t;

typedef enum
{
    FM_CMFPGA = 0x00000000,
    FM_CMRTS,
    FW_CMRTSCRC,
    FW_PMFPGA,
    FW_PMCORE0RTS,
    FW_PMCORE1RTS,
    FW_PMCORE0RTSCRC,
    FW_PMCORE1RTSCRC,
    FW_IOAPP,
    FW_IOFPGA,
    FW_IOL2BOOT1,
    FW_IOL2BOOT2,
    FW_INVALID_TYPE,

}FWType_t;

typedef enum
{
    ERASE_RTS = 0x00000000,
    ERASE_RTS_FLG,
    ERASE_FPGA,
    ERASE_FPGA_FLG,
    NOR_OPT_INVALID_TYPE,

}NorFlashOptType_t;



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


typedef struct SlaveFWInfoTag
{
    uint32_t uiRTSCore0Len;                             /*RTSCore0文件数据长度*/
    uint32_t uiRTSCore1Len;                             /*RTSCore1文件数据长度*/
    uint32_t uiRTSCore0CRCLen;                          /*RTSCore0CRC文件数据长度*/
    uint32_t uiRTSCore1CRCLen;                          /*RTSCore1CRC文件数据长度*/
    uint32_t uiFPGALen;                                 /*FPGA文件数据长度*/
    uint32_t uiIOFWLen;                                 /*IO固件文件数据长度*/
    uint32_t uiFileWriteIndex;                          /*文件写操作索引*/
    uint32_t uiFileReadIndex;                           /*文件读操作索引*/
    uint32_t uiFileNameLen;                             /*文件名长度*/
    uint8_t ucFileName[MAX_FW_FILE_NAME_LEN];           /*固件文件名*/
    uint16_t usSubPackNum;                              /*文件传输子包号*/
    uint8_t ucSlotID;
    uint8_t ucPrgType;

}SlaveFWInfo_t;


typedef struct FWUpdateSrvInfoTag
{
    uint8_t ucModuleType;                               /*模块类型：0-CM；1-PM；2-I/O*/
    uint8_t ucPrgType;                                  /*程序类型：0-FPGA(CM/PM)；1-I/O固件；2-RTS(CM/PM)*/
    uint8_t ucIOSlotID;                                 /*I/O模块的编号，其他模块时填0*/
    uint8_t ucPMID;                                     /*PMID*/
    uint16_t usPrgDataLen;                              /*文件长度*/
    uint16_t usSubPackNum;                              /*子包号*/
}FWUpdateSrvInfo_t;

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

typedef struct P2PSendRspHeadTag
{
    uint8_t ucSrcCMID;
    uint8_t ucSrcNETID;
    uint8_t ucReserve1;
    uint8_t ucReserve2;
    uint32_t uiDstIP1Addr;
    uint32_t uiDstIP2Addr;
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve3;
    uint32_t uiTotalLen;

}P2PSendRspHead_t;

typedef struct P2PSendReqHeadTag
{
    uint8_t ucSrcCMID;
    uint8_t ucSrcNETID;
    uint8_t ucTimeoutFlag;
    uint8_t ucL1NetID;  /* Link 1 NetID */
    uint32_t uiDstIP1Addr;
    uint32_t uiDstIP2Addr;
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve2;
    uint32_t uiTotalLen;
}P2PSendReqHead_t;

typedef struct P2PRecvReqHeadTag
{
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve1;
    uint16_t usTotalLen;
    uint8_t ucPMInfo;
    uint8_t ucReserve2;
}P2PRecvReqHead_t;

typedef struct P2PRecvRespHeadTag
{
    uint8_t ucSrcStaID;
    uint8_t ucDstStaID;
    uint8_t ucFrameType;
    uint8_t ucReserve1;
    uint16_t usTotalLen;
    uint8_t ucReserve2[2];
}P2PRecvRespHead_t;



typedef struct P2PSendCtrlTag
{
    uint16_t usWaitACKCount;
    uint8_t ucDestStaID;                /* 目标站号 */
    bool_t bHasSend;                    /* 已发送标志位 */
    uint32_t uiIntrCycCnt;              /* 发送间隔周期计数 */
    WaitSendFlag_t emWaitSendFlag;
    ACKTimeoutFlag_t emACKTimeoutFlag;
    WaitACKFlag_t emWaitACKFlag;
}P2PSendCtrl_t;

typedef struct SOEReadReqTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /* 1~16*/
    uint16_t usGetIndex;                    /* 本次获取Entry事件的起始索引*/
}SOEReadReq_t;

typedef struct SOEReadRspTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /* 1~16*/
    uint16_t usNextIndex;                   /* 可被请求的下一事件的索引*/
    uint16_t usEntryNum;                    /* 本包应答内Entry的数目*/
}SOEReadRsp_t;

typedef struct SOEDeleteReqTag
{
    uint8_t ucPMID;
    uint8_t ucSOEBlockID;                   /*1~16*/
}SOEDeleteReq_t;


typedef struct LOGReadReqTag
{
    uint8_t ucPMID;
    uint8_t ucLOGBlockID;                   /* 0~2(Core0,Core1,User) */
    uint16_t usGetIndex;                    /* 本次获取Entry事件的起始索引*/
}LOGReadReq_t;

typedef struct LOGReadRspTag
{
    uint8_t ucPMID;
    uint8_t ucLOGBlockID;                   /* 0~2(Core0,Core1,User) */
    uint16_t usNextIndex;                   /* 可被请求的下一事件的索引*/
    uint16_t usEntryNum;                    /* 本包应答内Entry的数目*/
}LOGReadRsp_t;

typedef struct InterCMDHeadTag
{
    PMController_t emPM_ID;                 /*目的PMid*/
    CMSlot_t emCMSlotID;                    /*来源CM的槽位号*/
    InterCMD_t emInterCMD;                  /*内部命令id*/
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

/* Modbus主从通用头信息结构体定义 */
typedef struct CommModbusCommonHeadTag
{
    uint8_t ucPortType;                     /* 端口类型 */
    uint8_t ucReserve;                      /* 预留 */
    uint16_t usDataLen;                     /* Modbus数据的长度（字节） */
}CommModbusCommonHead_t;

typedef struct AMSModInfoTag
{
    uint16_t usModType;
    uint8_t ucReserve;                      /* 预留 */
    uint8_t ucModAddr;
}AMSModInfo_t;

typedef struct AMSModScanTag
{
    uint8_t ucScanType;                     /*探测类型：0-All; 1-某一I/O模块*/
    uint8_t ucModAddr;                      /*I/O模块站号，仅当探测类型为1时，有意义*/
    uint16_t ucReserve;                     /* 预留 */
    
}AMSModScan_t;


typedef struct FWFMRTSInfoTag
{
    uint32_t uiCore0CRCStartAddr;
    uint32_t uiCore0CRCLen;
    uint32_t uiCore0StartAddr;
    uint32_t uiCore0Len;
    uint32_t uiCore1CRCStartAddr;
    uint32_t uiCore1CRCLen;
    uint32_t uiCore1StartAddr;
    uint32_t uiCore1Len;
}FWFMRTSInfo_t;

typedef struct FWFMFPGAInfoTag
{
    uint32_t uiFPGAStartAddr;
    uint32_t uiFPGALen;
    
}FWFMFPGAInfo_t;



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

/*
********************************函数描述*******************************************************************************
* 函数名称: CommGetFileInfo
*
* 功能描述: 获取文件信息
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
FileInfo_t* CommGetFileInfo(SysCfgFileType_t emCfgFileType);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommDetermineFileExist
*
* 功能描述: 判断文件是否存在
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
/*判断文件是否存在*/
FileOptRest_t CommDetermineFileExist(uint8_t  ucFileName[]);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommOpenFile
*
* 功能描述: 打开文件操作
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
/*打开文件操作*/
FileOptRest_t CommOpenFile(uint8_t  ucFileName[],pint32_t piFd);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommWriteFile
*
* 功能描述: 写文件操作
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
/*写文件操作*/
FileOptRest_t CommWriteFile(int32_t iFd,uint8_t  ucFileData[],uint16_t usFileDataLen);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommCloseFile
*
* 功能描述: 关闭文件操作
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
/*关闭文件操作*/
FileOptRest_t CommCloseFile(int32_t iFd);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommDeleteFile
*
* 功能描述: 删除文件操作
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
/*删除文件操作*/
FileOptRest_t CommDeleteFile(uint8_t  ucFileName[]);
/*
********************************函数描述*******************************************************************************
* 函数名称: CommRenameFile
*
* 功能描述: 重命名文件操作
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
/*重命名文件操作*/
FileOptRest_t CommRenameFile(uint8_t  ucOldFileName[], uint8_t  ucNewFileName[]);

#endif /*COMM_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
