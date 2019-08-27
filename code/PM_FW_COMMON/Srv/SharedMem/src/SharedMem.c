/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 共享内存
*
*  文件名称    : SharedMem.c
*
*  功能简介    : core0与core1间的共享内存
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-15
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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static Pause_Flag_t s_emSysPauseFlag[MAX_TASK_NUM];
static uint32_t s_uiIECErrTime[MAX_TASK_NUM];
static uint16_t s_usPMState;
static OSCPCfgInfo_t stOSCPCfg[MAX_TASK_NUM];
static OSCPCtrlInfo_t stOSCPCtrl[MAX_TASK_NUM];
static uint8_t s_ucOSCPVar[MAX_TASK_NUM][OSCP_BUFF_LEN];

static uint32_t uiUP1DeltaTime;
static uint32_t uiUP2DeltaTime;

static bool_t bUP1DeltaFlag;
static bool_t bUP2DeltaFlag;
static bool_t bUP1FirstScanFlag;
static bool_t bUP2FirstScanFlag;

static uint32_t uiUP1AverageScanSurplus;
static uint32_t uiUP1AverageScanOverrun;
static uint32_t uiUP2AverageScanSurplus;
static uint32_t uiUP2AverageScanOverrun;

static uint32_t s_uiCheckErr;


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
* 函数名称: SharedMemInit
*
* 功能描述: 初始化
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
void SharedMemInit(void)
{
    /* 0为core0,1为core1 */
    if(0==get_cpu_no())
    {
        /* 初始化 */
        SharedCommInit();
        SharedConfigInit();
        SharedRtDataInit();
        SharedSOEInit();
        LEDInit();
        SharedWatchdogInit();
        SharedFlagInit();
        SharedHandshakeInit();
        SharedSafetyCommInit();
        SharedDiagCodeInit();

        uiUP1DeltaTime = 0;
        uiUP2DeltaTime = 0;

        bUP1DeltaFlag = false;
        bUP2DeltaFlag = false;
        bUP1FirstScanFlag = false;
        bUP2FirstScanFlag = false;

        uiUP1AverageScanSurplus = 0;
        uiUP1AverageScanOverrun = 0;
        uiUP2AverageScanSurplus = 0;
        uiUP2AverageScanOverrun = 0;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedFlagInit
*
* 功能描述: 共享内存上标志位的初始化。
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
void SharedFlagInit(void)
{
    s_emSysPauseFlag[TASK_UP1] = PF_CONTINUE;
    s_emSysPauseFlag[TASK_UP2] = PF_CONTINUE;
    s_uiIECErrTime[TASK_UP1] = 0U;
    s_uiIECErrTime[TASK_UP2] = 0U;
    s_usPMState = 0U;
    memset(&stOSCPCfg[TASK_UP1], 0x00, MAX_TASK_NUM * sizeof(OSCPCfgInfo_t));
    memset(&stOSCPCtrl[TASK_UP1], 0x00, MAX_TASK_NUM * sizeof(OSCPCtrlInfo_t));
    memset(&s_ucOSCPVar[0][0], 0x00, MAX_TASK_NUM*OSCP_BUFF_LEN);
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetPauseFlag
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
void SharedSetPauseFlag(Pause_Flag_t emSysPauseFlag,Task_ID_t emTaskID)
{
    if(((PF_CONTINUE == emSysPauseFlag ) || (PF_PAUSE == emSysPauseFlag )) && (emTaskID < MAX_TASK_NUM))
    {
        atomic32Set(&s_emSysPauseFlag[emTaskID], emSysPauseFlag);
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetPauseFlag
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
Pause_Flag_t SharedGetPauseFlag(Task_ID_t emTaskID)
{
    Pause_Flag_t emPauseFlag = PF_CONTINUE;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        emPauseFlag = atomic32Get(&s_emSysPauseFlag[emTaskID]);
    }
    return emPauseFlag;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetIECErrTime
*
* 功能描述: 获取指定任务的IEC运算错误次数。
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
uint8_t SharedGetIECErrTime(Task_ID_t emTaskID)
{
    uint8_t ucIECErrTime = 0U;
    if(emTaskID < MAX_TASK_NUM)
    {
        ucIECErrTime = (uint8_t)atomic32Get(&s_uiIECErrTime[emTaskID]);
    }
    
    return ucIECErrTime;
}
/*
***********************************************************************************************************************
* 函数名称: SharedAddIECErrTime
*
* 功能描述: 指定任务的IEC运算错误次数加1。
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
void SharedAddIECErrTime(Task_ID_t emTaskID)
{
    uint32_t uiIECErrTime = 0U;
    if(emTaskID < MAX_TASK_NUM)
    {
        uiIECErrTime = (uint8_t)atomic32Get(&s_uiIECErrTime[emTaskID]);
        if(uiIECErrTime < 255)
        {
            uiIECErrTime++;
            atomic32Set(&s_uiIECErrTime[emTaskID],uiIECErrTime);
        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedClearIECErrTime
*
* 功能描述: 清除指定任务的IEC运算错误次数。
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
void SharedClearIECErrTime(Task_ID_t emTaskID)
{
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_uiIECErrTime[emTaskID],0);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSetPMState
*
* 功能描述: 设置PM状态。
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
void SharedSetPMState(uint16_t usPMState)
{
    s_usPMState = usPMState;

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetPMState
*
* 功能描述: 获取PM状态。
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
uint16_t SharedGetPMState(void)
{

    return s_usPMState;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPCfg
*
* 功能描述: 获取变量示波器配置信息。
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
OSCPCfgInfo_t* SharedGetOSCPCfg(Task_ID_t emTaskID)
{
    OSCPCfgInfo_t* pstOSCPCfg = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstOSCPCfg = &stOSCPCfg[emTaskID];
    }
    return pstOSCPCfg;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPCtrl
*
* 功能描述: 获取变量示波器控制信息。
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
OSCPCtrlInfo_t* SharedGetOSCPCtrl(Task_ID_t emTaskID)
{
    OSCPCtrlInfo_t* pstOSCPCtrl = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstOSCPCtrl = &stOSCPCtrl[emTaskID];
    }
    return pstOSCPCtrl;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetOSCPBuff
*
* 功能描述: 获取变量示波器存储地址。
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
puint8_t SharedGetOSCPBuff(Task_ID_t emTaskID, uint32_t uiOffset)
{
    puint8_t pucOSCPVarAddr= NULL;
    
    if((emTaskID < MAX_TASK_NUM) && (uiOffset < OSCP_BUFF_LEN))
    {
        pucOSCPVarAddr = &s_ucOSCPVar[emTaskID][uiOffset];
    }
    return pucOSCPVarAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDelataTime
*
* 功能描述: 获取delata time
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : Delata time
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetDelataTime(Task_ID_t emTaskID)
{
    uint32_t uiDelataT= 0;

    if(emTaskID == TASK_UP1)
    {
        uiDelataT = uiUP1DeltaTime;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiDelataT = uiUP2DeltaTime;
    }
    else
    {
        uiDelataT = 0;
    }
    return uiDelataT;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDelataTime
*
* 功能描述: 设置 delata time
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetDelataTime(Task_ID_t emTaskID, uint32_t uiDelataTime)
{

    if(emTaskID == TASK_UP1)
    {
        uiUP1DeltaTime = uiDelataTime;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiUP2DeltaTime = uiDelataTime;
    }
    else
    {
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDelataFlag
*
* 功能描述: 获取delata Flag
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : Delata flag
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedGetDelataFlag(Task_ID_t emTaskID)
{
    bool_t bDelataFlag= false;

    if(emTaskID == TASK_UP1)
    {
        bDelataFlag = bUP1DeltaFlag;
    }
    else if(emTaskID == TASK_UP2)
    {
        bDelataFlag = bUP2DeltaFlag;
    }
    else
    {
        bDelataFlag = false;
    }
    return bDelataFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetFirstScanFlag
*
* 功能描述: 获取FirstScan Flag
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : FirstScan flag
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedGetFirstScanFlag(Task_ID_t emTaskID)
{
    bool_t bFirstScanFlag= false;

    if(emTaskID == TASK_UP1)
    {
        bFirstScanFlag = bUP1FirstScanFlag;
    }
    else if(emTaskID == TASK_UP2)
    {
        bFirstScanFlag = bUP2FirstScanFlag;
    }
    else
    {
        bFirstScanFlag = false;
    }
    return bFirstScanFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDelataFlag
*
* 功能描述: 设置 delata flag
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetDelataFlag(Task_ID_t emTaskID, bool_t bDelataFlag)
{
    if(emTaskID == TASK_UP1)
    {
        bUP1DeltaFlag = bDelataFlag;
    }
    else if(emTaskID == TASK_UP2)
    {
        bUP2DeltaFlag = bDelataFlag;
    }
    else
    {
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetFirstScanFlag
*
* 功能描述: 设置 FirstScan flag
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetFirstScanFlag(Task_ID_t emTaskID, bool_t bFirstScanFlag)
{
    if(emTaskID == TASK_UP1)
    {
        bUP1FirstScanFlag = bFirstScanFlag;
    }
    else if(emTaskID == TASK_UP2)
    {
        bUP2FirstScanFlag = bFirstScanFlag;
    }
    else
    {
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetAvgScanSurplus
*
* 功能描述: get average scan surplus
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : average scan surplus
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetAvgScanSurplus(Task_ID_t emTaskID)
{
    uint32_t uiVal= 0;

    if(emTaskID == TASK_UP1)
    {
        uiVal = uiUP1AverageScanSurplus;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiVal = uiUP2AverageScanSurplus;
    }
    else
    {
        uiVal = 0;
    }
    return uiVal;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetAvgScanSurplus
*
* 功能描述: Set average scan surplus
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetAvgScanSurplus(Task_ID_t emTaskID, uint32_t uiVal)
{
    if(emTaskID == TASK_UP1)
    {
        uiUP1AverageScanSurplus = uiVal;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiUP2AverageScanSurplus = uiVal;
    }
    else
    {
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetAvgScanOverrun
*
* 功能描述: get average scan overrun
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : average scan overrun
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetAvgScanOverrun(Task_ID_t emTaskID)
{
    uint32_t uiVal= 0;

    if(emTaskID == TASK_UP1)
    {
        uiVal = uiUP1AverageScanOverrun;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiVal = uiUP2AverageScanOverrun;
    }
    else
    {
        uiVal = 0;
    }
    return uiVal;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetAvgScanOverrun
*
* 功能描述: Set average scan overrun
*
* 输入参数: emTaskID 任务号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetAvgScanOverrun(Task_ID_t emTaskID, uint32_t uiVal)
{
    if(emTaskID == TASK_UP1)
    {
        uiUP1AverageScanOverrun = uiVal;
    }
    else if(emTaskID == TASK_UP2)
    {
        uiUP2AverageScanOverrun = uiVal;
    }
    else
    {
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetCheckErr
*
* 功能描述: Set error
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetCheckErr(uint32_t uiVal)
{
    s_uiCheckErr = uiVal;
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetCheckErr
*
* 功能描述: Get error
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetCheckErr(void)
{
    return s_uiCheckErr;
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
