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


/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysState_t s_stSysState;                     /* 系统状态 */
static SysSystemTime_t s_stSystemTime;              /* 系统时间 */
static CMSlot_t s_emSlot[NUM_OF_CM];
static CMSlot_t s_emSlotNew[NUM_OF_CM];

static SysCfgFileTable_t s_stCfgFileAllTable;       /* 上传CM的完整配置文件表*/
static SysCfgFileTable_t s_stCfgFileIncreTable;     /* 上传CM的增量配置文件表*/


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 删除老的无效表项 */
static void DeleteOldSlotTable(void);
/* 添加新的增量表项 */
static void AddNewSlotTable(void);

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

    SysCfgFileInit();
    SysCMBusInit();
    SysFileManagerInit();
    SysModbusInit();
    SysP2PInit();
    SysRTDataInit();
    
    SharedMemInit();
    
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
    if(pstSysState != NULL)
    {
        memcpy((puint8_t)pstSysState, (puint8_t)&(s_stSysState),sizeof(SysState_t));
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

    emLocalID = SysGetLocalPMID();
    bRet = SysSetCMBusCommState(emCMID, emLocalID, usInterState);

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysCMConfigTableInit
*
* 功能描述: 上电初始化CM逻辑号与槽号的对应关系
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
void SysCMConfigTableInit(void)
{
    uint8_t i = 0;

    /* 初始化有效标识及槽号*/
    for(i = 0; i < NUM_OF_CM; i++)
    {
        s_stSysState.stCMConfigTable.bValid[i] = false;
        s_stSysState.stCMConfigTable.ucSlotID[i] = INVALID_CM_SLOT;
    }

    /* 初始化逻辑号 */
    s_stSysState.stCMConfigTable.ucLogicID[0] = CM1;
    s_stSysState.stCMConfigTable.ucLogicID[1] = CM2;
    s_stSysState.stCMConfigTable.ucLogicID[2] = CM3;
    s_stSysState.stCMConfigTable.ucLogicID[3] = CM4;

    /* 扫描3~15号槽，将最多4个CM注册到表中s_emSlot[] */
    s_emSlot[0] = SLOT3;//CM1对应的槽号 FOR TEST
    s_emSlot[1] = INVALID_CM_SLOT;//CM2对应的槽号FOR TEST
    s_emSlot[2] = INVALID_CM_SLOT;//CM3对应的槽号FOR TEST
    s_emSlot[3] = INVALID_CM_SLOT;//CM4对应的槽号FOR TEST

    /* 设置逻辑号及槽号对应表 */
    for(i = 0; i < NUM_OF_CM; i++)
    {
        if(s_emSlot[i] != INVALID_CM_SLOT)
        {
            s_stSysState.stCMConfigTable.bValid[i] = true;
            s_stSysState.stCMConfigTable.ucSlotID[i] = s_emSlot[i];
        }
        else
        {
            s_stSysState.stCMConfigTable.bValid[i] = false;
            s_stSysState.stCMConfigTable.ucSlotID[i] = INVALID_CM_SLOT;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysUpdateCMConfigTable
*
* 功能描述: 周期性更新CM逻辑号与槽号的对应关系
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
void SysUpdateCMConfigTable(void)
{
    //if(下装成功)
    /* 获取下装后新的CM的配置信息中的槽号，将各CM对应的槽号注册到表中s_emSlotNew[] */

    //else (没有工程)
    /* 扫描3~15号槽，将槽号注册到表中s_emSlotNew[] */
    s_emSlotNew[0] = SLOT3;//FOR TEST
    s_emSlotNew[1] = INVALID_CM_SLOT;//FOR TEST
    s_emSlotNew[2] = INVALID_CM_SLOT;//FOR TEST
    s_emSlotNew[3] = INVALID_CM_SLOT;//FOR TEST

    /* 删除拔掉的CM */
    DeleteOldSlotTable();

    /* 添加新安装的CM */
    AddNewSlotTable();
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysCMLogicIDToSLotID
*
* 功能描述: 逻辑号转槽号
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
CMSlot_t SysCMLogicIDToSLotID(CMController_t emCMID)
{
    CMSlot_t emCMSlot = INVALID_CM_SLOT;
    uint8_t i = 0;

    for(i = 0; i < NUM_OF_CM; i++)
    {
        /* 由逻辑号查找槽号 */
        if((emCMID == s_stSysState.stCMConfigTable.ucLogicID[i]) \
            && (true == s_stSysState.stCMConfigTable.bValid[i]))
        {
            emCMSlot = s_stSysState.stCMConfigTable.ucSlotID[i];
            break;
        }
    }
    return emCMSlot;
}
/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: DeleteOldSlotTable
*
* 功能描述: 删除老的无效表项
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
static void DeleteOldSlotTable(void)
{
    uint8_t i = 0;
    uint8_t j = 0;
    bool_t bRemove = true;

    for(i = 0; i < NUM_OF_CM; i++)
    {
        /* 判断是否有CM被移除 */
        for(j = 0; j < NUM_OF_CM; j++)
        {
            if(s_emSlotNew[j] == s_stSysState.stCMConfigTable.ucSlotID[i])
            {
                /* CM未被移除 */
                bRemove = false;
                break;
            }
            else
            {
                bRemove = true;
            }
        }

        /* CM被移除 */
        if(true == bRemove)
        {
            s_stSysState.stCMConfigTable.bValid[i] = false;
            s_stSysState.stCMConfigTable.ucSlotID[i] = INVALID_CM_SLOT;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: AddNewSlotTable
*
* 功能描述: 添加新的增量表项
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
static void AddNewSlotTable(void)
{
    uint8_t i = 0;
    uint8_t j = 0;
    bool_t bFind = false;

    for(i = 0; i < NUM_OF_CM; i++)
    {
        if(INVALID_CM_SLOT != s_emSlotNew[i])
        {
            /* 在原表中查找是否已经安装 */
            for(j = 0; j < NUM_OF_CM; j++)
            {
                if(s_stSysState.stCMConfigTable.ucSlotID[j] == s_emSlotNew[i])
                {
                    bFind = true;
                    break;
                }
                else
                {
                    bFind = false;
                }
            }

            /* 原来未安装 */
            if(false == bFind)
            {
                /* 找到空白项，添加新增的CM */
                for(j = 0; j < NUM_OF_CM; j++)
                {
                    /* 找到映射表的空白项 */
                    if(false == s_stSysState.stCMConfigTable.bValid[j])
                    {
                        /* 置该项已被使用标识 */
                        s_stSysState.stCMConfigTable.bValid[j] = true;
                        /* 将槽号写入映射表 */
                        s_stSysState.stCMConfigTable.ucSlotID[j] = s_emSlotNew[i];
                        break;
                    }
                }
            }
        }
    }
    return;
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
                                 文件结束
***********************************************************************************************************************
*/
