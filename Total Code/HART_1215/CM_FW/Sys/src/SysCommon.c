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
*  创建日期    : 2015-12-31
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
#include <string.h>
#include "../inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysState_t s_stSysState;           /* 系统状态 */
static SysSystemTime_t s_stSystemTime;    /* 系统时间 */
static PrjInfo_t s_stPrjInfo[NUM_OF_PM];
static PrjInfo_t s_stDownloadPrjInfo[NUM_OF_PM];
static TaskInformation_t s_stTaskInfo[NUM_OF_PM];
/* Added by wangdong begin */
static uint16_t s_usInterCommState[NUM_OF_PM][NUM_OF_SLOT];     /* 内部通讯状态 */

/* Added by wangdong end */
static SysCM2PM_t s_stCM2PMInfo;           /* CM到PM的信息 */

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
    memset((puint8_t)&s_stSystemTime, 0, sizeof(SysSystemTime_t));

    memset((puint8_t)&s_stPrjInfo[0], 0, sizeof(PrjInfo_t)*NUM_OF_PM);
    memset((puint8_t)&s_stDownloadPrjInfo[0], 0, sizeof(PrjInfo_t)*NUM_OF_PM);
    memset((puint8_t)&s_stTaskInfo[0], 0, sizeof(TaskInformation_t)*NUM_OF_PM);
    memset((puint8_t)&s_stCM2PMInfo, 0, sizeof(SysCM2PM_t));
    /* Added by wangdong begin */
    s_stSysState.stCMState.usInterCommState[0][0] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[0][1] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[0][2] = 0xFFFF;

    s_stSysState.stCMState.usInterCommState[1][0] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[1][1] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[1][2] = 0xFFFF;

    s_stSysState.stCMState.usInterCommState[2][0] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[2][1] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[2][2] = 0xFFFF;

    s_stSysState.stCMState.usInterCommState[3][0] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[3][1] = 0xFFFF;
    s_stSysState.stCMState.usInterCommState[3][2] = 0xFFFF;

    memset( s_usInterCommState, 0xFF, sizeof(uint16_t)*NUM_OF_PM*NUM_OF_SLOT );
    /* Added by wangdong end */

    SysRTDataInit();
    SysModbusInit();
    SysP2PInit();
    SysPCsoftwareInit();
    SysConfigInit();
    SysDatabseInit();
    CMCfgInit();
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
bool_t SysGetPMState(PMController_t emPMID, uint16_t *pusState)
{
    bool_t bRet = false;
    uint16_t usState = 0;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM) && (pusState != NULL))
    {
        usState = s_stSysState.stPMState.usPMState[emPMID];
        *pusState = usState;
        bRet = true;
    }

    return bRet;
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
bool_t SysGetCMState(PMController_t emPMID, CMController_t emCMID, uint16_t *pusState)
{
    bool_t bRet = false;
    uint16_t usCMState = 0;

    if((emCMID < NUM_OF_CM) && (pusState != NULL))
    {
        usCMState = s_stSysState.stCMState.usCMState[emCMID];
        *pusState = usCMState;
        bRet = true;
    }

    return bRet;
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
bool_t SysSetSysState(SysState_t *pstSysState, PMController_t emPMID)
{
    bool_t bRet = false;

    if(pstSysState != NULL)
    {
        //memcpy((puint8_t)&s_stSysState, (puint8_t)pstSysState, sizeof(SysState_t));
        //printf("s=%d,ms=%d\n",s_stSysState.stPMState.stPMtime[2].uiSecond,s_stSysState.stPMState.stPMtime[2].usMiliSecond);

        memcpy((puint8_t)&s_stSysState.stCMState, (puint8_t)&(pstSysState->stCMState), sizeof(SysCMState_t));
        memcpy((puint8_t)&s_stSysState.stDownloadPrjInfo, (puint8_t)&(pstSysState->stDownloadPrjInfo), sizeof(PrjInfo_t));
        memcpy((puint8_t)&s_stSysState.stPrjInfo, (puint8_t)&(pstSysState->stPrjInfo), sizeof(PrjInfo_t));
        memcpy((puint8_t)&s_stSysState.stTaskInfo, (puint8_t)&(pstSysState->stTaskInfo), sizeof(TaskInformation_t));
        memcpy((puint8_t)&s_stSysState.uiCMLogicID, (puint8_t)&(pstSysState->uiCMLogicID), sizeof(uint32_t));
        s_stSysState.stPMState.stPMtime[emPMID].uiSecond = pstSysState->stPMState.stPMtime[emPMID].uiSecond;
        s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond = pstSysState->stPMState.stPMtime[emPMID].usMiliSecond;
        s_stSysState.stPMState.usPMState[emPMID] = pstSysState->stPMState.usPMState[emPMID];
        printf("set s=%d,ms=%d\n",s_stSysState.stPMState.stPMtime[emPMID].uiSecond,s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond);
        bRet = true;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetSysTimeFromPM
*
* 功能描述: 获取来自PM的时间
*
* 输入参数: emPMID: PM的id
*
* 输出参数: pstSysTime 指向获取的时间
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetSysTimeFromPM(SysSystemTime_t *pstSysTime, PMController_t emPMID)
{
    bool_t bRet = false;

    if((pstSysTime != NULL) && (emPMID >= PMA) && (emPMID <= PMC))
    {
        pstSysTime->uiSecond = s_stSysState.stPMState.stPMtime[emPMID].uiSecond;
        pstSysTime->usMiliSecond = s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond;
        printf("get s=%d,ms=%d\n",s_stSysState.stPMState.stPMtime[emPMID].uiSecond,s_stSysState.stPMState.stPMtime[emPMID].usMiliSecond);
        bRet = true;
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetExterCommState
*
* 功能描述: 获取指定CM的指定通讯口的外部通讯状态
*
* 输入参数: emCMID:指定CM的ID
*        emType:指定的通讯口
*
* 输出参数: 无
*
* 返 回 值  : 指定CM的某个通讯口的外部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetExterCommState(CMController_t emCMID, NetworkPortType_t emType)
{
    uint16_t usExterState = EXTER_COMM_INVALID;

    if((emCMID < NUM_OF_CM) && (emType < NUM_OF_NETWORK_PORT))
    {
        usExterState = s_stSysState.stCMState.usExterCommState[emCMID][emType];
    }

    return usExterState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetExterCommState
*
* 功能描述: 设置指定CM的指定通讯口的外部通讯状态
*
* 输入参数: emCMID:   指定CM的ID
*        emType:指定的通讯口
*        usExterState:外部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetExterCommState(CMController_t emCMID, NetworkPortType_t emType, uint16_t usExterState)
{
    uint16_t usState = usExterState;
    bool_t bRet = false;

    if((emCMID < NUM_OF_CM) && (emType < NUM_OF_NETWORK_PORT))
    {
        s_stSysState.stCMState.usExterCommState[emCMID][emType] = usState;
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetInterCommState
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

uint16_t SysGetInterCommState(CMController_t emCMID, PMController_t emPMID)
{
    uint16_t usState = INTER_COMM_INVALID;

    if((emCMID < NUM_OF_CM) && (emPMID < NUM_OF_PM))
    {
        usState = s_stSysState.stCMState.usInterCommState[emCMID][emPMID];
    }

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetInterCommState
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

bool_t SysSetInterCommState(CMController_t emCMID, PMController_t emPMID, uint16_t usInterState)
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
* 函数名称: SysGetSystemTime
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

SysSystemTime_t SysGetSystemTime(void)
{
    return s_stSystemTime;
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

    /* 输入参数合法性检查 */
    if(pstSysTime != NULL)
    {
        if(pstSysTime->usMiliSecond < 1000)
        {
            s_stSystemTime.uiSecond = pstSysTime->uiSecond;
            s_stSystemTime.usMiliSecond = pstSysTime->usMiliSecond;
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

    /* 输入参数合法性检查 */
    if((pstSysTime != NULL) && (emCMID >= CM1) && (emCMID <= CM4))
    {
        if(pstSysTime->usMiliSecond < 1000)
        {
            s_stSysState.stCMState.stCMtime[emCMID].uiSecond = pstSysTime->uiSecond;
            s_stSysState.stCMState.stCMtime[emCMID].usMiliSecond = pstSysTime->usMiliSecond;
            printf("s=%d, ms=%d\n",pstSysTime->uiSecond,pstSysTime->usMiliSecond);
            bRet = true;
        }
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetLocalExterCommState
*
* 功能描述: 获取本CM的指定通讯口的外部通讯状态
*
* 输入参数: emType:指定的通讯口
*
* 输出参数: 无
*
* 返 回 值  : 本CM的某个通讯口的外部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalExterCommState(NetworkPortType_t emType)
{
    uint16_t usExterState = EXTER_COMM_INVALID;
    CMController_t emLocalID = INVALID_CM_ID;

    emLocalID = SysGetLocalCMID();
    usExterState = SysGetExterCommState(emLocalID, emType);

    return usExterState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalExterCommState
*
* 功能描述: 设置本CM的指定通讯口的外部通讯状态
*
* 输入参数: emType:指定的通讯口
*        usExterState:外部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalExterCommState(NetworkPortType_t emType, uint16_t usExterState)
{
    bool_t bRet = false;
    CMController_t emLocalID = INVALID_CM_ID;

    emLocalID = SysGetLocalCMID();
    bRet = SysSetExterCommState(emLocalID, emType, usExterState);

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalInterCommState
*
* 功能描述: 获取本CM与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 本CM与指定PM的内部通讯状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SysGetLocalInterCommState(PMController_t emPMID)
{
    uint16_t usState = INTER_COMM_INVALID;
    CMSlot_t emLocalSlot = INVALID_CM_SLOT;
    emLocalSlot = GetCMSlotAddr(10);

    if((emPMID < NUM_OF_PM) && (emLocalSlot != INVALID_CM_SLOT))
    {
        usState = s_usInterCommState[emPMID][emLocalSlot];
    }

    return usState;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetLocalInterCommState
*
* 功能描述: 设置本CM与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetLocalInterCommState(PMController_t emPMID, uint16_t usInterState)
{
    bool_t bRet = false;
    CMSlot_t emLocalSlot = INVALID_CM_SLOT;

    emLocalSlot = GetCMSlotAddr(10);

    if((emPMID < NUM_OF_PM) && (emLocalSlot != INVALID_CM_SLOT))
    {
        s_usInterCommState[emPMID][emLocalSlot] = usInterState;
        printf( "SysSetLocalInterCommState: PM:%d, CM:%d, State:%d.\r\n", emPMID, emLocalSlot, usInterState );
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMSlot
*
* 功能描述: 获取本地CM的槽号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地CM的槽号
*
* 注意事项: 无
***********************************************************************************************************************
*/
CMSlot_t SysGetLocalCMSlot(void)
{
    CMSlot_t emSlot = INVALID_CM_SLOT;

    emSlot = GetCMSlotAddr(GET_SLOT_TIMEOUT);

    return emSlot;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetLocalCMID
*
* 功能描述: 获取本地CM的ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 本地CM的ID
*
* 注意事项: 无
***********************************************************************************************************************
*/
CMController_t SysGetLocalCMID(void)
{
    CMController_t emLocalID = INVALID_CM_ID;

    emLocalID = s_stSysState.uiCMLogicID;

    return emLocalID;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMTaskNum
*
* 功能描述: 获取某一PM中的任务个数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 任务个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SysGetPMTaskNum(PMController_t emPMID)
{
    uint8_t ucTaskNum = 0;//test

    if(emPMID < NUM_OF_PM)
    {
        ucTaskNum = s_stTaskInfo[emPMID].ucTaskNum;
    }

    return ucTaskNum;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetPMTaskInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetPMTaskInfo( PMController_t emPMID, uint8_t ucTaskIndex, uint8_t *pucTaskID, int8_t *picTaskStatus )
{
    if((emPMID >= PMA) && (emPMID <= PMC))
    {
        if((pucTaskID!=NULL)&&(picTaskStatus!=NULL)&&(ucTaskIndex==0))
        {
            *pucTaskID = 0;
            *picTaskStatus = s_stTaskInfo[emPMID].ucTask1State;
        }
        if((pucTaskID!=NULL)&&(picTaskStatus!=NULL)&&(ucTaskIndex==1))
        {
            *pucTaskID = 1;
            *picTaskStatus = s_stTaskInfo[emPMID].ucTask2State;
        }
    }
    return ;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPMTaskNum
*
* 功能描述: 存储某一PM中的任务个数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 任务个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPMTaskNum(PMController_t emPMID, uint8_t ucTaskNum)
{
    if(emPMID < NUM_OF_PM)
    {
        s_stTaskInfo[emPMID].ucTaskNum = ucTaskNum;
        printf("task num=%d\n", ucTaskNum);
    }

    return ;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPMTaskInfo
*
* 功能描述:
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPMTaskInfo( PMController_t emPMID, uint8_t ucTask1Status, uint8_t ucTask2Status )
{
    if((emPMID >= PMA) && (emPMID <= PMC))
    {
        s_stTaskInfo[emPMID].ucTask1State = ucTask1Status;
        s_stTaskInfo[emPMID].ucTask2State = ucTask2Status;
        printf("PMid=%d ts1 =%d ts2=%d\n",emPMID, ucTask1Status, ucTask2Status);
    }
    return ;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPrjInfo
*
* 功能描述: 存储工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo)
{
    if((emPMID < NUM_OF_PM) && (pstPrjInfo != NULL))
    {
        memcpy((puint8_t)&s_stPrjInfo[emPMID], (puint8_t)pstPrjInfo, sizeof(PrjInfo_t));
    }
}
/*
***********************************************************************************************************************
* 函数名称: SysSetDownloadPrjInfo
*
* 功能描述: 存储工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetDownloadPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo)
{
    if((emPMID < NUM_OF_PM) && (pstPrjInfo != NULL))
    {
        memcpy((puint8_t)&s_stDownloadPrjInfo[emPMID], (puint8_t)pstPrjInfo, sizeof(PrjInfo_t));
    }
}
/*
***********************************************************************************************************************
* 函数名称: SysGetPrjInfo
*
* 功能描述: 获取工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo)
{
    if((emPMID < NUM_OF_PM) && (pstPrjInfo != NULL))
    {
        memcpy((puint8_t)pstPrjInfo, (puint8_t)&s_stPrjInfo[emPMID], sizeof(PrjInfo_t));
    }
}
/*
***********************************************************************************************************************
* 函数名称: SysGetDownloadPrjInfo
*
* 功能描述: 获取工程信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetDownloadPrjInfo(PMController_t emPMID,PrjInfo_t *pstPrjInfo)
{
    if((emPMID < NUM_OF_PM) && (pstPrjInfo != NULL))
    {
        memcpy((puint8_t)pstPrjInfo, (puint8_t)&s_stDownloadPrjInfo[emPMID], sizeof(PrjInfo_t));
    }
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
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCMVer(uint32_t uiCMVer)
{
    s_stCM2PMInfo.uiCMVer = uiCMVer;
    //printf("cm ver=0x%x\n", uiCMVer);
    return;
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
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCMLEDState(uint32_t uiCMLEDState)
{
    s_stCM2PMInfo.uiLEDState = uiCMLEDState;
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetCM2PMInfo
*
* 功能描述: 获取CM到PM的信息
*
* 输入参数: 无
*
* 输出参数: pstCM2PMInfo
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysGetCM2PMInfo(SysCM2PM_t *pstCM2PMInfo)
{
    if(pstCM2PMInfo != NULL)
    {
        memcpy((puint8_t)pstCM2PMInfo, (puint8_t)&(s_stCM2PMInfo),sizeof(SysCM2PM_t));
    }
    return;
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
