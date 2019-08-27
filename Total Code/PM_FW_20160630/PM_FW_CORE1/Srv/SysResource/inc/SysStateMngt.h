/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : SysStateMngt.h
*
*  功能简介    :
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-04
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SYS_STATE_MNGT_H_
#define SYS_STATE_MNGT_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "SysCommon.h"
#include "SysSync.h"
#include "SysPMConfig.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/


/*工程文件存在*/
#define PRG_FILE_EXIST                      (0x00000000U)
/*工程文件不存在*/
#define PRG_FILE_NOT_EXIST                  (0x00000001U)
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/


typedef enum
{
    PF_CONTINUE = 0x00000000U,          /*系统暂停标志*/
    PF_PAUSE                            /*系统暂停恢复，继续运行标志*/
}Pause_Flag_t;

/* 系统状态 */
typedef enum
{
    SYS_STABLE_STATE = 0x00000000U,                        /* 系统状态:稳态 = 0 */
    SYS_UNSTABLE_STATE,                                    /* 系统状态:非稳态 = 1 */
    MAX_SYS_STATE
}sys_state_t;

/* PM 硬件诊断状态 */
typedef enum
{
    HARD_OFFLINE = 0x00000000U,                            /* 硬件状态: 离线 */
    HARD_ONLINE                                            /* 硬件状态: 在线 */
}hard_status_t;

#pragma pack (1)

typedef struct SysAjustSyncTimeTag
{
    /* 同步时钟主从标识 */
    uint32_t uiMaster;
    /* 校时使能标识 */
    ajust_status_t emAjustEn;
    /* 同步时钟校时时间 */
    SyncTime64_t unSyncAjustTime;
}SysAjustSyncTime_t; /* 12 bytes */


typedef struct PrjInfoTag
{
    uint8_t ucIsConfigured;                     /*工程是否解析完成*/
    uint8_t ucIsConfirmed;                      /*工程是否经用户确认*/
    uint8_t ucIsIOConfigured;                   /*IO配置是否完成*/
    uint8_t ucRtDataSync;                       /*实时数据是否同步完成*/
    uint32_t uiPrjID;
    uint32_t uiMainVern;
    uint32_t uiMinorVern;
    uint32_t uiPrjCRC;
}PrjInfo_t;


typedef struct SysPMStateInfoTag
{
    Key_Switch_Type_t emSwitchKeys;
    work_status_t emWorkStatus;
    task_status_t emTaskStatus[MAX_TASK_NUM];
    PrjInfo_t stPrjInfo;                        /*当前使用工程的工程信息*/
    PrjInfo_t stDLPrjInfo;                      /*下装工程（未生效），工程信息*/
    
}SysPMStateInfo_t;





#pragma pack ()
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SysSetAjustTimerEn
*
* 功能描述: 设置是否使能校时?。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetAjustTimerEn(PMController_t emPMId, ajust_status_t emAjustEn);

/*
***********************************************************************************************************************
* 函数名称: SysGetKeySwitchValue
*
* 功能描述: 获取钥匙开关的值。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
Key_Switch_Type_t SysGetKeySwitchValue(void);
/*
***********************************************************************************************************************
* 函数名称: SysSetKeySwitchValue
*
* 功能描述: 设置钥匙开关的值。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetKeySwitchValue(Key_Switch_Type_t emKeySwitchValue);

/*
***********************************************************************************************************************
* 函数名称: SysSetPauseFlag
*
* 功能描述: 设置系统的暂停标志。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPauseFlag(Pause_Flag_t emSysPauseFlag);
/*
***********************************************************************************************************************
* 函数名称: SysGetPauseFlag
*
* 功能描述: 获取系统的暂停标志。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
Pause_Flag_t SysGetPauseFlag(void);

/*
***********************************************************************************************************************
* 函数名称: SysGetWaitPMId
*
* 功能描述: 获取等待控制器ID。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
PMController_t SysGetWaitPMId(void);
/*
***********************************************************************************************************************
* 函数名称: SysSetWaitPMId
*
* 功能描述: 设置等待控制器ID。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetWaitPMId(PMController_t emPMId);

/*
***********************************************************************************************************************
* 函数名称: SysGetPMState
*
* 功能描述: 获取指定PM的状态信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
SysPMStateInfo_t* SysGetPMStateInfo(PMController_t emPMId);


/*
***********************************************************************************************************************
* 函数名称: SysGetPrjInfo
*
* 功能描述: 获取指定PM当前使用的工程信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
PrjInfo_t* SysGetPrjInfo(PMController_t emPMId);
/*
***********************************************************************************************************************
* 函数名称: SysGetDownLoadPrjInfo
*
* 功能描述: 获取指定PM当前下装的工程（未生效）信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
PrjInfo_t* SysGetDownLoadPrjInfo(PMController_t emPMId);

/*
***********************************************************************************************************************
* 函数名称: SysSetState
*
* 功能描述: 设置系统状态。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetState(sys_state_t emSysState);
/*
***********************************************************************************************************************
* 函数名称: SysGetState
*
* 功能描述: 获取系统状态。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
sys_state_t SysGetState(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetPrjFileExist
*
* 功能描述: 获取系统状态。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetPrjFileExist(void);
/*
***********************************************************************************************************************
* 函数名称: SysGetConfigStatus
*
* 功能描述: 获取工程是否进行过配置，是否存在配置信息状态。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SysGetConfigStatus(PMController_t emPMId);

/*
***********************************************************************************************************************
* 函数名称: SysSetConfigStatus
*
* 功能描述: 设置工程是否进行过配置，是否存在配置信息状态。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetConfigStatus(PMController_t emPMId, uint8_t ucConfigStatus);


#endif /* SYS_STATE_MNGT_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
