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
*  文件名称    : SysCommon.c
*
*  功能简介    : 系统资源管理模块中的公共资源，其它模块均可调用
*
*  作者        : 李琦
*
*  创建日期    : 2016-02-24
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include <Srv/Scheduler/include/string.h>
#include "../inc/Sys.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Log/inc/Log.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysState_t s_stSysState;                     /* 系统状态 */
static SysCM2PM_t s_stCM2PMInfo[NUM_OF_CM];           /* CM到PM的信息 */

static SysCfgFileTable_t s_stCfgFileAllTable;       /* 上传CM的完整配置文件表*/
static SysCfgFileTable_t s_stCfgFileIncreTable;     /* 上传CM的增量配置文件表*/

static ProjVerInfo_t s_stProjVer[NUM_OF_CM];



SysDownLoadFile_t s_stSysDownLoadFile;
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysInit
*
* 功能描述: 系统资源管理模块初始化
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
void SysInit(void)
{

    memset((puint8_t)&s_stSysState, 0, sizeof(SysState_t));
    memset((puint8_t)&s_stSysDownLoadFile, 0x00, sizeof(SysDownLoadFile_t));
    memset((puint8_t)&s_stCM2PMInfo, 0, sizeof(SysCM2PM_t)*NUM_OF_CM);

    SysCfgFileInit();
    SysTransCfgFileFlagInit();
    SysCMBusInit();
    SysFileManagerInit();
    SysModbusInit();
    SysP2PInit();
    SysRTDataInit();
    
    SharedMemInit();
    LogInit();
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetPMState
*
* 功能描述: 获取指定PM的状态
*
* 输入参数: emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定PM的状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetPMState(PMController_t emPMID)
{
    uint16_t usState = 0;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        usState = s_stSysState.stPMState.usPMState[emPMID];
    }

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPMState
*
* 功能描述: 设置指定PM的状态
*
* 输入参数: emPMID:   指定PM的ID
*        usPMState:PM的状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetPMState(PMController_t emPMID, uint16_t usPMState)
{
    uint16_t usState = usPMState;
    bool_t bRet = false;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        if(s_stSysState.stPMState.usPMState[emPMID] != usState)
        {
            printf("PM %d State: %x\n",emPMID,usState);
        }
        
        
        s_stSysState.stPMState.usPMState[emPMID] = usState;
        bRet = true;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetSysCMState
*
* 功能描述: 获取所有CM的系统状态
*
* 输入参数: 无
*
* 输出参数: pstSysCMState: 指向所有CM的系统状态
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetSysCMState(SysCMState_t *pstSysCMState)
{
    if(pstSysCMState != NULL)
    {
        memcpy((puint8_t)pstSysCMState, (puint8_t)&(s_stSysState.stCMState),sizeof(SysCMState_t));
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSysState
*
* 功能描述: 获取系统状态
*
* 输入参数: 无
*
* 输出参数: pstSysState: 指向系统状态
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetSysState(SysState_t *pstSysState)
{
    PMController_t emPMID = PMA;
    PrjInfo_t* pstPrjInfo = NULL;
    uint16_t usPMState = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;                                 /*控制站配置信息可读片段指针*/

    if(pstSysState != NULL)
    {
        emPMID = SysGetLocalPMID();

        pstPrjInfo = SysGetPrjInfo(emPMID);
        pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);

        if(pstCtrlStationInfo != NULL)
        {
//            memcpy((puint8_t)&(s_stSysState.stPrjInfo), (puint8_t)pstPrjInfo,sizeof(PrjInfo_t));
            s_stSysState.stPrjInfo.uiPrjID = pstCtrlStationInfo->stPrjConfigInfo.uiProjectID;
            s_stSysState.stPrjInfo.uiMainVern = pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion;
            s_stSysState.stPrjInfo.uiMinorVern = pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion;
            s_stSysState.stPrjInfo.uiPrjCRC = pstCtrlStationInfo->uiCRC;
            s_stSysState.stPrjInfo.ucIsConfigured = pstCtrlStationInfo->uiIsConfigured;
            s_stSysState.stPrjInfo.ucIsIOConfigured = IPBUSGetDeviceConfigSlaveFlag();
        }
        else
        {
            s_stSysState.stPrjInfo.uiPrjID = 0;
            s_stSysState.stPrjInfo.uiMainVern = 0;
            s_stSysState.stPrjInfo.uiMinorVern = 0;
            s_stSysState.stPrjInfo.uiPrjCRC = 0;
            s_stSysState.stPrjInfo.ucIsConfigured = 0;
            s_stSysState.stPrjInfo.ucIsIOConfigured = 0;
        }

        pstPrjInfo = SysGetDownLoadPrjInfo(emPMID);

        if(pstPrjInfo != NULL)
        {
            memcpy((puint8_t)&(s_stSysState.stDownloadPrjInfo), (puint8_t)pstPrjInfo,sizeof(PrjInfo_t));
        }

        usPMState = SharedGetPMState();
        SysSetPMState(emPMID, usPMState);

        memcpy((puint8_t)pstSysState, (puint8_t)&s_stSysState,sizeof(SysState_t));

        /* get task info */
        pstSysState->stTaskInfo.ucTaskNum = SharedGetTaskNum();
        emPMID = SysGetLocalPMID();
        pstSysState->stTaskInfo.ucTask1State = (uint8_t) SysGetTaskStatus(emPMID, TASK_UP1);
        pstSysState->stTaskInfo.ucTask2State = (uint8_t) SysGetTaskStatus(emPMID, TASK_UP2);
        //printf("tn=%d ts1=%d ts2=%d\n", pstSysState->stTaskInfo.ucTaskNum, pstSysState->stTaskInfo.ucTask1State, pstSysState->stTaskInfo.ucTask2State);

        /* 向CM上报本PM的系统时间 目前版本为ms单位 */
        s_stSysState.stPMState.stPMtime[emPMID].uiSecond = (uint32_t) (SysGetSystemtime()/1000000);//以后改为us为单位
        s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond = (uint16_t)((SysGetSystemtime()/1000) % 1000);
        //s_stSysState.stPMState.stPMtime[1].uiSecond = (uint32_t) (SysGetSystemtime()/1000000);//以后改为us为单位
        //s_stSysState.stPMState.stPMtime[1].usMiliSecond = (uint16_t)((SysGetSystemtime()/1000) % 1000);
        //s_stSysState.stPMState.stPMtime[2].uiSecond = (uint32_t) (SysGetSystemtime()/1000000);//以后改为us为单位
        //s_stSysState.stPMState.stPMtime[2].usMiliSecond = (uint16_t)((SysGetSystemtime()/1000) % 1000);
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetCMLogicID
*
* 功能描述: 设置与当前通讯的CM的逻辑号
*
* 输入参数: emCMID逻辑号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCMLogicID(CMController_t emCMID)
{
    if((emCMID >= CM1) && (emCMID <= CM4))
    {
        s_stSysState.uiCMLogicID = (uint32_t)emCMID;
    }
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCMState
*
* 功能描述: 获取指定CM的状态
*
* 输入参数: emCMID:指定CM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定CM的状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetCMState(CMController_t emCMID)
{
    uint16_t usCMState = 0;

    if(emCMID < NUM_OF_CM)
    {
        usCMState = s_stSysState.stCMState.usCMState[emCMID];
    }

    return usCMState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCMState
*
* 功能描述: 设置指定CM的状态
*
* 输入参数: emCMID:   指定CM的ID
*        usCMState:CM的状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMState(CMController_t emCMID, uint16_t usCMState)
{
    uint16_t usState = usCMState;
    bool_t bRet = false;

    if(emCMID < NUM_OF_CM)
    {
        s_stSysState.stCMState.usCMState[emCMID] = usState;
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCMSysState
*
* 功能描述: 设置CM的状态
*
* 输入参数: emCMID:   指定CM的ID
*        usCMState:CM的状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMSysState(SysCMState_t *pstCMSysState)
{
    bool_t bRet = false;

    if(pstCMSysState != NULL)
    {
        memcpy((puint8_t)&(s_stSysState.stCMState), (puint8_t)pstCMSysState, sizeof(SysCMState_t));
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetSysState
*
* 功能描述: 设置系统的状态
*
* 输入参数: pstSysState: 系统状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSysState(SysState_t *pstSysState)
{
    bool_t bRet = false;

    if(pstSysState != NULL)
    {
        memcpy((puint8_t)&s_stSysState, (puint8_t)pstSysState, sizeof(SysState_t));
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCMBusCommState
*
* 功能描述: 获取指定CM与指定PM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 指定CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetCMBusCommState(CMController_t emCMID, PMController_t emPMID)
{
    uint16_t usState = CMBUS_COMM_INVALID;

    if((emCMID < NUM_OF_CM) && (emPMID < NUM_OF_PM))
    {
        usState = s_stSysState.stCMState.usInterCommState[emCMID][emPMID];
    }

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCMBusCommState
*
* 功能描述: 设置指定CM与指定PM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emPMID:指定PM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMBusCommState(CMController_t emCMID, PMController_t emPMID, uint16_t usInterState)
{
    uint16_t usState = usInterState;
    bool_t bRet = false;

    if((emCMID < NUM_OF_CM) && (emPMID < NUM_OF_PM))
    {
        s_stSysState.stCMState.usInterCommState[emCMID][emPMID] = usState;
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMSysTime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysSystemTime_t SysGetPMSysTime(void)
{
    SysSystemTime_t stSystime;
    PMController_t emPMID = SysGetLocalPMID();

    stSystime.uiSecond = 0;
    stSystime.usMiliSecond = 0;

    if((emPMID >= PMA) && (emPMID <= PMC))
    {
        stSystime.uiSecond = s_stSysState.stPMState.stPMtime[emPMID].uiSecond;
        stSystime.usMiliSecond = s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond;
    }

    return stSystime;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetSystemTime
*
* 功能描述: 设置系统时间
*
* 输入参数: stSysTime: 系统时间
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSystemTime(SysSystemTime_t *pstSysTime)
{
    bool_t bRet = false;
    PMController_t emPMID = SysGetLocalPMID();

    /* 输入参数合法性检查 */
    if((pstSysTime != NULL) && (emPMID >= PMA) && (emPMID <= PMC))
    {
        if(pstSysTime->usMiliSecond < 1000)
        {
            s_stSysState.stPMState.stPMtime[emPMID].uiSecond = pstSysTime->uiSecond;
            s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond = pstSysTime->usMiliSecond;
            bRet = true;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCMSystemTime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间
*
* 注意事项: 无
***********************************************************************************************************************
*/

SysSystemTime_t SysGetCMSystemTime(CMController_t emCMID)
{
    SysSystemTime_t stSystime;

    stSystime.uiSecond = 0;
    stSystime.usMiliSecond = 0;
    if((emCMID >= CM1) && (emCMID <= CM4))
    {
        stSystime.uiSecond = s_stSysState.stCMState.stCMtime[emCMID].uiSecond;
        stSystime.usMiliSecond = s_stSysState.stCMState.stCMtime[emCMID].usMiliSecond;
    }
    return stSystime;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCMSystemTime
*
* 功能描述: 设置系统时间
*
* 输入参数: stSysTime: 系统时间
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetCMSystemTime(CMController_t emCMID, SysSystemTime_t *pstSysTime)
{
    bool_t bRet = false;
    AdjSysTimeSignal_t stSysTime;
    CMConfigInfo_t* pstCMcfg = NULL;
    ulong64_t ulTime = 0;

    /* 输入参数合法性检查 */
    if((pstSysTime != NULL) && (emCMID >= CM1) && (emCMID <= CM4))
    {
        s_stSysState.stCMState.stCMtime[emCMID].uiSecond = pstSysTime->uiSecond;
        s_stSysState.stCMState.stCMtime[emCMID].usMiliSecond = pstSysTime->usMiliSecond;
        //printf("s=%d, ms=%d\n",pstSysTime->uiSecond,pstSysTime->usMiliSecond);

#if 0
        stSysTime.uiSignal = SIGNAL_CM_SYS_TIME;
        stSysTime.uiSec = pstSysTime->uiSecond;
        stSysTime.uiMilSec = pstSysTime->usMiliSecond;

        SysSetSysTimeSignal(CORE1_TO_CORE0, &stSysTime);
#endif

        pstCMcfg = SharedGetCMConfigInfo();

        if(pstCMcfg != NULL)
        {
            if((ACTIVE == pstCMcfg->stCMSingleInfo[emCMID].emActive) \
                && (ACTIVE == pstCMcfg->stCMSingleInfo[emCMID].stNTPTCPConfigInfo.emActive))
            {
                if((0 == pstCMcfg->stCMSingleInfo[emCMID].stNTPTCPConfigInfo.ucTimingSrc)\
                        || (1 == pstCMcfg->stCMSingleInfo[emCMID].stNTPTCPConfigInfo.ucTimingSrc))
                {
                    ulTime = (ulong64_t)pstSysTime->uiSecond * 1000; //ms
                    ulTime += (ulong64_t)(pstSysTime->usMiliSecond);
                    ulTime = ulTime * 1000; //us
                    SysAdjustSystemTime(ulTime);
                    SysAdjustSystemTimeEnable();
                }
            }
        }

    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetLocalPMID
*
* 功能描述: 获取本地PM的ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地PM的ID
*
* 注意事项: 无
***********************************************************************************************************************
*/
//PMController_t SysGetLocalPMID(void)
//{
//    PMController_t emLocalID = INVALID_PM_ID;
//
//    /* 读硬件寄存器，根据寄存器内的信息判断本PM的ID */
//
//    emLocalID = PMA;//for test
//
//
//    return emLocalID;
//}

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMBusCommState
*
* 功能描述: 获取本PM与指定CM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*
* 输出参数: 无
*
* 返 回 值  : 本CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalCMBusCommState(CMController_t emCMID)
{
    uint16_t usState = CMBUS_COMM_INVALID;
    PMController_t emLocalID = INVALID_PM_ID;

    emLocalID = SysGetLocalPMID();
    usState = SysGetCMBusCommState(emCMID, emLocalID);

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalCMBusCommState
*
* 功能描述: 设置本PM与指定CM的内部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalCMBusCommState(CMController_t emCMID, uint16_t usInterState)
{
    bool_t bRet = false;
    PMController_t emLocalID = INVALID_PM_ID;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    emLocalID = SysGetLocalPMID();
    bRet = SysSetCMBusCommState(emCMID, emLocalID, usInterState);


    switch(emCMID)
    {
        case CM1:
        {
            pstAppend->stPMState.stPMFirmwareState.ucCMBUSState[0] = (uint8_t)usInterState;
            pstAppend->stCMState[0].stCMFirmwareState.ucOnline = (uint8_t)usInterState;
        }
        break;

        case CM2:
        {
            pstAppend->stPMState.stPMFirmwareState.ucCMBUSState[1] = (uint8_t)usInterState;
            pstAppend->stCMState[1].stCMFirmwareState.ucOnline = (uint8_t)usInterState;
        }
        break;

        case CM3:
        {
            pstAppend->stPMState.stPMFirmwareState.ucCMBUSState[2] = (uint8_t)usInterState;
            pstAppend->stCMState[2].stCMFirmwareState.ucOnline = (uint8_t)usInterState;
        }
        break;

        case CM4:
        {
            pstAppend->stPMState.stPMFirmwareState.ucCMBUSState[3] = (uint8_t)usInterState;
            pstAppend->stCMState[3].stCMFirmwareState.ucOnline = (uint8_t)usInterState;
        }
        break;

        default:
        {
            ;
        }
        break;
    }/*switch*/

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileAllTable
*
* 功能描述: 获取上传CM的完整配置文件表。
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
SysCfgFileTable_t* SysGetCfgFileAllTable(void)
{
    SysCfgFileTable_t* pstCfgFileTable = &s_stCfgFileAllTable;

    return pstCfgFileTable;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileIncreTable
*
* 功能描述: 获取上传CM的增量配置文件表。
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
SysCfgFileTable_t* SysGetCfgFileIncreTable(void)
{
    SysCfgFileTable_t* pstCfgFileTable = &s_stCfgFileIncreTable;

    return pstCfgFileTable;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFileName
*
* 功能描述: 根据文件类型ID获取文件名。
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
void SysGetFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[])
{
    uint8_t ucNameLen = 0U;
    
    if((cFileName != NULL) && (emCfgFileType < NUM_OF_CFG_FILE_TYPE))
    {
        memset(cFileName, 0x00, MAX_FILE_NAME_LEN);
        switch(emCfgFileType)
        {
            case CFG_CTRL_STATION:
            {
                ucNameLen = strlen((pchar_t)RTS_SC_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SC_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_PRG:
            {
                ucNameLen = strlen((pchar_t)RTS_PRG_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_PRG_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_IO_MODULAR:
            {
                ucNameLen = strlen((pchar_t)RTS_IOC_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_IOC_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_SOE:
            {
                ucNameLen = strlen((pchar_t)RTS_SOE_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SOE_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_P2P:
            {
                ucNameLen = strlen((pchar_t)RTS_P2P_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_P2P_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_MASTER:
            {
                ucNameLen = strlen((pchar_t)RTS_MMC_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MMC_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_SLAVE:
            {
                ucNameLen = strlen((pchar_t)RTS_MSC_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MSC_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_VOTE:
            {
                ucNameLen = strlen((pchar_t)RTS_VOT_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_VOT_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_RETAIN:
            {
                ucNameLen = strlen((pchar_t)RTS_RTN_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_RTN_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_OPC:
            {
                ucNameLen = strlen((pchar_t)RTS_OPC_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_OPC_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_CM:
            {
                ucNameLen = strlen((pchar_t)RTS_CM_FILE);
                
                if(ucNameLen > FORMAL_FILE_NAME_LEN)
                {
                    ucNameLen = FORMAL_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_CM_FILE, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            default:
                
            break;
        }
    }
    
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetTmpFileName
*
* 功能描述: 根据文件类型ID获取临时文件名。
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
void SysGetTmpFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[])
{
    uint8_t ucNameLen = 0U;
    
    if((cFileName != NULL) && (emCfgFileType < NUM_OF_CFG_FILE_TYPE))
    {
        memset(cFileName, 0x00, MAX_FILE_NAME_LEN);
        switch(emCfgFileType)
        {
            case CFG_CTRL_STATION:
            {
                ucNameLen = strlen((pchar_t)RTS_SC_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SC_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_PRG:
            {
                ucNameLen = strlen((pchar_t)RTS_PRG_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_PRG_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_IO_MODULAR:
            {
                ucNameLen = strlen((pchar_t)RTS_IOC_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_IOC_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_SOE:
            {
                ucNameLen = strlen((pchar_t)RTS_SOE_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SOE_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_P2P:
            {
                ucNameLen = strlen((pchar_t)RTS_P2P_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_P2P_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_MASTER:
            {
                ucNameLen = strlen((pchar_t)RTS_MMC_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MMC_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_SLAVE:
            {
                ucNameLen = strlen((pchar_t)RTS_MSC_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MSC_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_VOTE:
            {
                ucNameLen = strlen((pchar_t)RTS_VOT_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_VOT_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_RETAIN:
            {
                ucNameLen = strlen((pchar_t)RTS_RTN_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_RTN_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_OPC:
            {
                ucNameLen = strlen((pchar_t)RTS_OPC_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_OPC_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_CM:
            {
                ucNameLen = strlen((pchar_t)RTS_CM_FILE_TMP);
                
                if(ucNameLen > TMP_FILE_NAME_LEN)
                {
                    ucNameLen = TMP_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_CM_FILE_TMP, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            default:
                
            break;
        }
    }

    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetOldFileName
*
* 功能描述: 根据文件类型ID获取老文件名。
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
void SysGetOldFileName(SysCfgFileType_t emCfgFileType, char_t cFileName[])
{
    uint8_t ucNameLen = 0U;
    
    if((cFileName != NULL) && (emCfgFileType < NUM_OF_CFG_FILE_TYPE))
    {
        memset(cFileName, 0x00, MAX_FILE_NAME_LEN);
        switch(emCfgFileType)
        {
            case CFG_CTRL_STATION:
            {
                ucNameLen = strlen((pchar_t)RTS_SC_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SC_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_PRG:
            {
                ucNameLen = strlen((pchar_t)RTS_PRG_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_PRG_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_IO_MODULAR:
            {
                ucNameLen = strlen((pchar_t)RTS_IOC_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_IOC_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_SOE:
            {
                ucNameLen = strlen((pchar_t)RTS_SOE_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_SOE_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_P2P:
            {
                ucNameLen = strlen((pchar_t)RTS_P2P_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_P2P_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_MASTER:
            {
                ucNameLen = strlen((pchar_t)RTS_MMC_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MMC_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_MODBUS_SLAVE:
            {
                ucNameLen = strlen((pchar_t)RTS_MSC_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_MSC_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_VOTE:
            {
                ucNameLen = strlen((pchar_t)RTS_VOT_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_VOT_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_RETAIN:
            {
                ucNameLen = strlen((pchar_t)RTS_RTN_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_RTN_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_OPC:
            {
                ucNameLen = strlen((pchar_t)RTS_OPC_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_OPC_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            case CFG_CM:
            {
                ucNameLen = strlen((pchar_t)RTS_CM_FILE_OLD);
                
                if(ucNameLen > OLD_FILE_NAME_LEN)
                {
                    ucNameLen = OLD_FILE_NAME_LEN;
                }
                memcpy(cFileName, (pchar_t)RTS_CM_FILE_OLD, ucNameLen);
                
                cFileName[ucNameLen] = BLANK_CHAR;
            }
            break;
            
            default:
                
            break;
        }
    }
    

    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetRemoteProjVersion
*
* 功能描述: 存储来自CM的工程版本信息
*
* 输入参数:  emCMID CM的ID号
*         pstProjVer 指向工程版本信息
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetRemoteProjVersion(CMController_t emCMID, ProjVerInfo_t *pstProjVer)
{
    bool_t bRet = false;

    if(pstProjVer != NULL)
    {
        s_stProjVer[emCMID].uiMainVer = pstProjVer->uiMainVer;
        s_stProjVer[emCMID].uiMinorVer = pstProjVer->uiMinorVer;
        s_stProjVer[emCMID].uiProjCRC = pstProjVer->uiProjCRC;
        s_stProjVer[emCMID].uiProjID = pstProjVer->uiProjID;
        //printf("ProjCrc=0x%x\n", s_stProjVer[emCMID].uiProjCRC);
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRemoteProjVersion
*
* 功能描述: 获取来自CM的工程版本信息
*
* 输入参数:  emCMID CM的ID号
*         pstProjVer 指向工程版本信息
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRemoteProjVersion(CMController_t emCMID, ProjVerInfo_t *pstProjVer)
{
    bool_t bRet = false;

    if(pstProjVer != NULL)
    {
        pstProjVer->uiMainVer = s_stProjVer[emCMID].uiMainVer;
        pstProjVer->uiMinorVer = s_stProjVer[emCMID].uiMinorVer;
        pstProjVer->uiProjCRC = s_stProjVer[emCMID].uiProjCRC;
        pstProjVer->uiProjID = s_stProjVer[emCMID].uiProjID;
        bRet = true;
    }

    return bRet;
}



/*
***********************************************************************************************************************
* 函数名称: SysSetFileDownloadFlg
*
* 功能描述: 设置文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  ucFileDownloadFlg：下装标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetFileDownloadFlg(SysCfgFileType_t emSysCfgFileType, uint8_t ucFileDownloadFlg)
{
    if((emSysCfgFileType < NUM_OF_CFG_FILE_TYPE) && \
       ((FILE_NEW_DOWNLOAD == ucFileDownloadFlg) || (FILE_NOT_NEW_DOWNLOAD == ucFileDownloadFlg)))
    {
        s_stSysDownLoadFile.ucFileDownloadFlg[emSysCfgFileType] = ucFileDownloadFlg;
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFileDownloadFlg
*
* 功能描述: 获取文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  
*
* 输出参数: 
*
* 返 回 值  : 下装标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetFileDownloadFlg(SysCfgFileType_t emSysCfgFileType )
{
    uint8_t ucFileDownloadFlg = FILE_NOT_NEW_DOWNLOAD;
    
    if(emSysCfgFileType < NUM_OF_CFG_FILE_TYPE)
    {
        ucFileDownloadFlg = s_stSysDownLoadFile.ucFileDownloadFlg[emSysCfgFileType];
    }
    return ucFileDownloadFlg;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetFileNewConfigFlg
*
* 功能描述: 设置文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  ucFileNewConfigFlg：新配置标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetFileNewConfigFlg(SysCfgFileType_t emSysCfgFileType, uint8_t ucFileNewConfigFlg)
{
    if((emSysCfgFileType < NUM_OF_CFG_FILE_TYPE) && \
       ((FILE_NEW_DOWNLOAD == ucFileNewConfigFlg) || (FILE_NOT_NEW_DOWNLOAD == ucFileNewConfigFlg)))
    {
        s_stSysDownLoadFile.ucFileNewConfigFlg[emSysCfgFileType] = ucFileNewConfigFlg;
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFileNewConfigFlg
*
* 功能描述: 获取文件的下装标志
*
* 输入参数:  emSysCfgFileType： 文件ID  
*
* 输出参数: 
*
* 返 回 值  : 新配置标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetFileNewConfigFlg(SysCfgFileType_t emSysCfgFileType )
{
    uint8_t ucFileNewConfigFlg = FILE_NOT_NEW_CONFIGURED;
    
    if(emSysCfgFileType < NUM_OF_CFG_FILE_TYPE)
    {
        ucFileNewConfigFlg = s_stSysDownLoadFile.ucFileNewConfigFlg[emSysCfgFileType];
    }
    return ucFileNewConfigFlg;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetCMVer
*
* 功能描述: 设置CM的版本号
*
* 输入参数: uiCMVer 版本号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
//void SysSetCMVer(CMController_t emCMID, uint32_t uiCMVer)
//{
//    if((emCMID >= CM1) && (emCMID <= CM4))
//    {
//        s_stCM2PMInfo[emCMID].uiCMVer = uiCMVer;
//    }
//    return;
//}
/*
***********************************************************************************************************************
* 函数名称: SysGetCMVer
*
* 功能描述: 获取CM的版本号
*
* 输入参数: puiCMVer 版本号
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMVer(CMController_t emCMID, puint32_t puiCMVer)
{
    bool_t bRet = false;

    if((emCMID >= CM1) && (emCMID <= CM4) && (puiCMVer != NULL))
    {
//        puiCMVer = (puint32_t)s_stCM2PMInfo[emCMID].uiCMVer;
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetCMLEDState
*
* 功能描述: 设置CM的状态灯状态
*
* 输入参数: uiCMLEDState
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
//void SysSetCMLEDState(CMController_t emCMID, uint32_t uiCMLEDState)
//{
//    if((emCMID >= CM1) && (emCMID <= CM4))
//    {
//        s_stCM2PMInfo[emCMID].uiLEDState = uiCMLEDState;
//    }
//    return;
//}

/*
***********************************************************************************************************************
* 函数名称: SysGetCMLEDState
*
* 功能描述: 获取CM的状态灯状态
*
* 输入参数: puiCMLEDState
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetCMLEDState(CMController_t emCMID, puint32_t puiCMLEDState)
{
    bool_t bRet = false;

    if((emCMID >= CM1) && (emCMID <= CM4) && (puiCMLEDState != NULL))
    {
//        puiCMLEDState = (puint32_t)s_stCM2PMInfo[emCMID].uiLEDState;
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/






/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
