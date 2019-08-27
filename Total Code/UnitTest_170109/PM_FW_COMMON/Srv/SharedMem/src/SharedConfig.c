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
*  文件名称    : SharedConfig.h
*
*  功能简介    : core0与core1间的共享工程配置信息
*
*  作者            : 刘阳
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
#include "Srv/IPBUS/inc/IPBUS.h"
#include "../inc/SharedConfig.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
#pragma pack(4)
static CodeArea_t s_stCodeArea;
static task_status_t s_emTaskStatus[NUM_OF_PM][MAX_TASK_NUM];
static uint32_t s_uiSingleStepFlg[MAX_TASK_NUM];

static LogicCfg_t s_stLogicCfg;
static ExLibCfg_t s_stExLibCfg;
static SysIoModCfg_t s_stSysIoModCfg;
static CtrlStationCfg_t s_stCtrlStationCfg;
static SOECfg_t s_stSOECfg;
static P2PCfg_t s_stP2PCfg;
static ModbusMasterCfg_t s_stModbusMasterCfg;
static ModbusSlaveCfg_t s_stModbusSlaveCfg;
static VoteCfg_t s_stVoteCfg;
//static OPCCfg_t s_stOPCCfg;
static RetainVarCfg_t s_stRetainVarCfg;
static CMCfg_t s_stCMCfg;
static CMSlotIdTablCfg_t s_stCMSlotIdTablCfg;

static download_flag_t s_emDownloadFlg;     /* 系统下装标志*/



static uint32_t s_uiSwitchPos;

static PMController_t s_emLocalID;

static sync_data_flag_t s_emSyncDataFlag[MAX_TASK_NUM];

static PMController_t s_emDownloadPMID;            /*PM ID号（用于记录下装PM） */

static uint32_t s_uiPowerOnConfigSwitch;

static work_status_t s_emWorkStatus[NUM_OF_PM];

static uint32_t s_uiUsedParaBufID;

static OnlineDownLoad_Switch_Flg_t s_emOnlineDownLoadSwitchFlg[MAX_TASK_NUM];

static bool_t  s_bModConfigChangeFlg[MAX_MOD_NUM];

static uint32_t s_uiDisableRecover;


#pragma pack()
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
* 函数名称: SharedConfigInit
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
void SharedConfigInit(void)
{
    uint32_t uiLocalPMAddr = 0U;

    /* 初始化 */
    memset(&s_stLogicCfg, 0x00, sizeof(LogicCfg_t));
    memset(&s_stExLibCfg, 0x00, sizeof(ExLibCfg_t));
    memset(&s_stSysIoModCfg, 0x00, sizeof(SysIoModCfg_t));
    memset(&s_stCtrlStationCfg, 0x00, sizeof(CtrlStationCfg_t));
    memset(&s_stSOECfg, 0x00, sizeof(SOECfg_t));
    memset(&s_stP2PCfg, 0x00, sizeof(P2PCfg_t));
    memset(&s_stModbusMasterCfg, 0x00, sizeof(ModbusMasterCfg_t));
    memset(&s_stModbusSlaveCfg, 0x00, sizeof(ModbusSlaveCfg_t));
    memset(&s_stVoteCfg, 0x00, sizeof(VoteCfg_t));
//    memset(&s_stOPCCfg, 0x00, sizeof(OPCCfg_t));
    memset(&s_stRetainVarCfg, 0x00, sizeof(RetainVarCfg_t));
    memset(&s_stCMCfg, 0x00, sizeof(CMCfg_t));
    memset(&s_stCMSlotIdTablCfg, 0x00, sizeof(CMSlotIdTablCfg_t));
    memset(&s_stCodeArea, 0x00, sizeof(CodeArea_t));
    memset(&s_emTaskStatus[0][0], 0x00, sizeof(task_status_t)*NUM_OF_PM*MAX_TASK_NUM);
    
    SharedModConfigChangeInit();

    SharedConfigAreaInit();
    
    atomic32Set(&s_emDownloadFlg,NOT_DOWNLOAD);
    
    atomic32Set(&s_uiSwitchPos, 0U);
    
    /*从硬件寄存器读取数值*/
    uiLocalPMAddr = (uint32_t)GetLocalPMAddr();
    atomic32Set(&s_emLocalID, uiLocalPMAddr);
    
    atomic32Set(&s_emSyncDataFlag[TASK_UP1],DATA_NOT_SYNC);
    atomic32Set(&s_emSyncDataFlag[TASK_UP2],DATA_NOT_SYNC);
    
    
    atomic32Set(&s_uiSingleStepFlg[TASK_UP1],0);
    atomic32Set(&s_uiSingleStepFlg[TASK_UP2],0);
    
    atomic32Set(&s_emDownloadPMID,INVALID_PM_ID);
    
    atomic32Set(&s_uiPowerOnConfigSwitch,NOT_NEED_POWERON_CONIFG_SWITCH);
    
    atomic32Set(&s_emWorkStatus[PMA],WS_OFFLINE);
    atomic32Set(&s_emWorkStatus[PMB],WS_OFFLINE);
    atomic32Set(&s_emWorkStatus[PMC],WS_OFFLINE);
    
    atomic32Set(&s_uiUsedParaBufID, IPBUS_SLAVE_ENABLE_BUF0);
    
    atomic32Set(&s_emOnlineDownLoadSwitchFlg[TASK_UP1],OS_NOT_NEED_SWITCH);
    atomic32Set(&s_emOnlineDownLoadSwitchFlg[TASK_UP2],OS_NOT_NEED_SWITCH);
    
    atomic32Set(&s_uiDisableRecover,NEED_DISABLE_RECOVER);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedConfigAreaInit
*
* 功能描述: 配置信息结构片区初始化
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
void SharedConfigAreaInit(void)
{
    /*初始化各类配置信息的可读片区为无效片区*/
    atomic32Set(&s_stLogicCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stLogicCfg.stLogicConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stLogicCfg.stLogicConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stLogicCfg.stLogicConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stLogicCfg.stLogicConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stExLibCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stExLibCfg.stExLibConfigInfo[FIRST_PIECE].uiExLibFuncNum, 0);
    atomic32Set(&s_stExLibCfg.stExLibConfigInfo[SECOND_PIECE].uiExLibFuncNum, 0);
    
    atomic32Set(&s_stSysIoModCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stSysIoModCfg.stSysIoModInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stSysIoModCfg.stSysIoModInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stSysIoModCfg.stSysIoModInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stSysIoModCfg.stSysIoModInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stCtrlStationCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stCtrlStationCfg.stCtrlStationConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stCtrlStationCfg.stCtrlStationConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stCtrlStationCfg.stCtrlStationConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stCtrlStationCfg.stCtrlStationConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stSOECfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stSOECfg.stSOEConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stSOECfg.stSOEConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stSOECfg.stSOEConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stSOECfg.stSOEConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stP2PCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stP2PCfg.stP2PConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stP2PCfg.stP2PConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stP2PCfg.stP2PConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stP2PCfg.stP2PConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stModbusMasterCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stModbusMasterCfg.stModbusMasterInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stModbusMasterCfg.stModbusMasterInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stModbusMasterCfg.stModbusMasterInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stModbusMasterCfg.stModbusMasterInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stModbusSlaveCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stModbusSlaveCfg.stModbusSlaveInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stModbusSlaveCfg.stModbusSlaveInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stModbusSlaveCfg.stModbusSlaveInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stModbusSlaveCfg.stModbusSlaveInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stVoteCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stVoteCfg.stVOTEConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stVoteCfg.stVOTEConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stVoteCfg.stVOTEConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stVoteCfg.stVOTEConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stRetainVarCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stRetainVarCfg.stRetainVarInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stRetainVarCfg.stRetainVarInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stRetainVarCfg.stRetainVarInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stRetainVarCfg.stRetainVarInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stCMCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stCMCfg.stCMConfigInfo[FIRST_PIECE].uiCRC, 0);
    atomic32Set(&s_stCMCfg.stCMConfigInfo[FIRST_PIECE].uiFileVern, 0);
    atomic32Set(&s_stCMCfg.stCMConfigInfo[SECOND_PIECE].uiCRC, 0);
    atomic32Set(&s_stCMCfg.stCMConfigInfo[SECOND_PIECE].uiFileVern, 0);
    
    atomic32Set(&s_stCMSlotIdTablCfg.uiAvailable, INVALID_BUFF_ID);
    atomic32Set(&s_stCMSlotIdTablCfg.stCMSlotIdTabl[FIRST_PIECE].uiCMNum, 0);
    atomic32Set(&s_stCMSlotIdTablCfg.stCMSlotIdTabl[SECOND_PIECE].uiCMNum, 0);
    
    atomic32Set(&s_stCodeArea.uiAvailable, INVALID_BUFF_ID);
    
    
    atomic32Set(&s_stCodeArea.uiTmpAvailable[TASK_UP1], INVALID_BUFF_ID);
    atomic32Set(&s_stCodeArea.uiTmpAvailable[TASK_UP2], INVALID_BUFF_ID);
    atomic32Set(&s_stSysIoModCfg.uiTmpAvailable[TASK_UP1], INVALID_BUFF_ID);
    atomic32Set(&s_stSysIoModCfg.uiTmpAvailable[TASK_UP2], INVALID_BUFF_ID);
    atomic32Set(&s_stCtrlStationCfg.uiTmpAvailable[TASK_UP1], INVALID_BUFF_ID);
    atomic32Set(&s_stCtrlStationCfg.uiTmpAvailable[TASK_UP2], INVALID_BUFF_ID);
    atomic32Set(&s_stSOECfg.uiTmpAvailable[TASK_UP1], INVALID_BUFF_ID);
    atomic32Set(&s_stSOECfg.uiTmpAvailable[TASK_UP2], INVALID_BUFF_ID);

    
    return;
}


/****************************************各类配置信息可读片区对外接口***********************************************************/
/*
***********************************************************************************************************************
* 函数名称: SharedGetLogicInfo
*
* 功能描述: 获取逻辑配置信息指针
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
LogicConfigInfo_t* SharedGetLogicInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    LogicConfigInfo_t* pstLogicConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stLogicCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstLogicConfigInfo = &s_stLogicCfg.stLogicConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstLogicConfigInfo;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetExLibInfo
*
* 功能描述: 获取外部库配置信息指针
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
ExLibConfigInfo_t* SharedGetExLibInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    ExLibConfigInfo_t* pstExLibConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stExLibCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstExLibConfigInfo = &s_stExLibCfg.stExLibConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstExLibConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetIoModInfo
*
* 功能描述: 获取IO配置信息指针
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
SysIoModInfo_t* SharedGetIoModInfo(Task_ID_t emTaskID)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    if(emTaskID < MAX_TASK_NUM)
    {
        if((OS_SWITCH_START == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(emTaskID)))
        {
            uiAvailable = atomic32Get(&s_stSysIoModCfg.uiTmpAvailable[emTaskID]);
//            printf("IO: Available %x %x\n",uiAvailable,atomic32Get(&s_stSysIoModCfg.uiAvailable));
        }
        else
        {
            uiAvailable = atomic32Get(&s_stSysIoModCfg.uiAvailable);
        }
    }
    else
    {
        uiAvailable = atomic32Get(&s_stSysIoModCfg.uiAvailable);
    }
    
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstSysIoModInfo = &s_stSysIoModCfg.stSysIoModInfo[uiAvailable];
        //printf("GET IO: Available %x\n",uiAvailable);
    }
    else
    {
        /*log*/
    }

    return pstSysIoModInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCtrlStationInfo
*
* 功能描述: 获取控制站配置信息指针
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
CtrlStationConfigInfo_t* SharedGetCtrlStationInfo(Task_ID_t emTaskID)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    if(emTaskID < MAX_TASK_NUM)
    {
        if((OS_SWITCH_START == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_CONFIG_SWITCH == SharedGetOnlineDownLoadSwitchFlg(emTaskID)))
        {
            uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiTmpAvailable[emTaskID]);
//            printf("CS: Available %x %x\n",uiAvailable,atomic32Get(&s_stCtrlStationCfg.uiAvailable));
        }
        else
        {
            uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiAvailable);
        }
    }
    else
    {
        uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiAvailable);
    }
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstCtrlStationConfigInfo = &s_stCtrlStationCfg.stCtrlStationConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstCtrlStationConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetLogicInfo
*
* 功能描述: 获取SOE配置信息指针
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
SOEConfigInfo_t* SharedGetSOEConfigInfo (Task_ID_t emTaskID)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;

    if(emTaskID < MAX_TASK_NUM)
    {
        if((OS_SWITCH_START == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_CONFIG_SWITCH == SharedGetOnlineDownLoadSwitchFlg(emTaskID)))
        {
            uiAvailable = atomic32Get(&s_stSOECfg.uiTmpAvailable[emTaskID]);
//            printf("SOE: Available %x %x\n",uiAvailable,atomic32Get(&s_stSOECfg.uiAvailable));
        }
        else
        {
            uiAvailable = atomic32Get(&s_stSOECfg.uiAvailable);
        }
    }
    else
    {
        uiAvailable = atomic32Get(&s_stSOECfg.uiAvailable);
    }
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstSOEConfigInfo = &s_stSOECfg.stSOEConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstSOEConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PConfigInfo
*
* 功能描述: 获取P2P配置信息指针
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
P2PConfigInfo_t* SharedGetP2PConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stP2PCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstP2PConfigInfo = &s_stP2PCfg.stP2PConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstP2PConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModbusMasterInfo
*
* 功能描述: 获取Modbus主配置信息指针
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
ModbusMasterInfo_t* SharedGetModbusMasterInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;

    uiAvailable = atomic32Get(&s_stModbusMasterCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstModbusMasterInfo = &s_stModbusMasterCfg.stModbusMasterInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstModbusMasterInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModbusSlaveInfo
*
* 功能描述: 获取Modbus从配置信息指针
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
ModbusSlaveInfo_t* SharedGetModbusSlaveInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;

    uiAvailable = atomic32Get(&s_stModbusSlaveCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstModbusSlaveInfo = &s_stModbusSlaveCfg.stModbusSlaveInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstModbusSlaveInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetVOTEConfigInfo
*
* 功能描述: 获取通道表决配置信息指针
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
VOTEConfigInfo_t* SharedGetVOTEConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    VOTEConfigInfo_t* pstVOTEConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stVoteCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstVOTEConfigInfo = &s_stVoteCfg.stVOTEConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstVOTEConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetOPCVarInfo
*
* 功能描述: 获取OPC配置信息指针
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
//OPCVarInfo_t* SharedGetOPCVarInfo(void)
//{
//    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
//    OPCVarInfo_t* pstOPCVarInfo = NULL;
//
//    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
//    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
//    {
//        pstOPCVarInfo = &s_stOPCCfg.stOPCVarInfo[uiAvailable];
//    }
//    else
//    {
//        /*log*/
//    }
//
//    return pstOPCVarInfo;
//}
/*
***********************************************************************************************************************
* 函数名称: SharedGetRetainVarInfo
*
* 功能描述: 获取Retain变量配置信息指针
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
RetainVarInfo_t* SharedGetRetainVarInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    RetainVarInfo_t* pstRetainVarInfo = NULL;

    uiAvailable = atomic32Get(&s_stRetainVarCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstRetainVarInfo = &s_stRetainVarCfg.stRetainVarInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstRetainVarInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCMConfigInfo
*
* 功能描述: 获取CM配置信息指针
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
CMConfigInfo_t* SharedGetCMConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    CMConfigInfo_t* pstCMConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stCMCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstCMConfigInfo = &s_stCMCfg.stCMConfigInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstCMConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCMSlotIdTabl
*
* 功能描述: 获取CM槽位号与CM ID对照表信息指针
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
CMSlotIdTabl_t* SharedGetCMSlotIdTabl(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;

    uiAvailable = atomic32Get(&s_stCMSlotIdTablCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstCMSlotIdTabl = &s_stCMSlotIdTablCfg.stCMSlotIdTabl[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstCMSlotIdTabl;
}




/****************************************各类配置信息可写片区对外接口***********************************************************/


/*
***********************************************************************************************************************
* 函数名称: SharedGetLogicInfoWrit
*
* 功能描述: 获取逻辑配置信息可写片区指针
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
LogicConfigInfo_t* SharedGetLogicInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    LogicConfigInfo_t* pstLogicConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stLogicCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstLogicConfigInfo = &s_stLogicCfg.stLogicConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstLogicConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetExLibInfoWrit
*
* 功能描述: 获取外部库配置信息可写片区指针
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
ExLibConfigInfo_t* SharedGetExLibInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    ExLibConfigInfo_t* pstExlibConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stExLibCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstExlibConfigInfo = &s_stExLibCfg.stExLibConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstExlibConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetIoModInfoWrit
*
* 功能描述: 获取IO配置信息可写片区指针
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
SysIoModInfo_t* SharedGetIoModInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    uiAvailable = atomic32Get(&s_stSysIoModCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    printf("WritAble %d\n",uiWritAble);
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstSysIoModInfo = &s_stSysIoModCfg.stSysIoModInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstSysIoModInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCtrlStationInfoWrit
*
* 功能描述: 获取控制站配置信息可写片区指针
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
CtrlStationConfigInfo_t* SharedGetCtrlStationInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstCtrlStationConfigInfo = &s_stCtrlStationCfg.stCtrlStationConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstCtrlStationConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetLogicInfoWrit
*
* 功能描述: 获取SOE配置信息可写片区指针
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
SOEConfigInfo_t* SharedGetSOEConfigInfoWrit (void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stSOECfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstSOEConfigInfo = &s_stSOECfg.stSOEConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstSOEConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PConfigInfoWrit
*
* 功能描述: 获取P2P配置信息可写片区指针
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
P2PConfigInfo_t* SharedGetP2PConfigInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stP2PCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstP2PConfigInfo = &s_stP2PCfg.stP2PConfigInfo[uiWritAble];
        
    }
    else
    {
        /*log*/
    }

    return pstP2PConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModbusMasterInfoWrit
*
* 功能描述: 获取Modbus主配置信息可写片区指针
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
ModbusMasterInfo_t* SharedGetModbusMasterInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;

    uiAvailable = atomic32Get(&s_stModbusMasterCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstModbusMasterInfo = &s_stModbusMasterCfg.stModbusMasterInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstModbusMasterInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModbusSlaveInfoWrit
*
* 功能描述: 获取Modbus从配置信息可写片区指针
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
ModbusSlaveInfo_t* SharedGetModbusSlaveInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;

    uiAvailable = atomic32Get(&s_stModbusSlaveCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstModbusSlaveInfo = &s_stModbusSlaveCfg.stModbusSlaveInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstModbusSlaveInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetVOTEConfigInfoWrit
*
* 功能描述: 获取通道表决配置信息可写片区指针
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
VOTEConfigInfo_t* SharedGetVOTEConfigInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    VOTEConfigInfo_t* pstVOTEConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stVoteCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstVOTEConfigInfo = &s_stVoteCfg.stVOTEConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstVOTEConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetOPCVarInfoWrit
*
* 功能描述: 获取OPC配置信息可写片区指针
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
//OPCVarInfo_t* SharedGetOPCVarInfoWrit(void)
//{
//    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
//    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
//    OPCVarInfo_t* pstOPCVarInfo = NULL;
//
//    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
//    
//    if(INVALID_BUFF_ID == uiAvailable)
//    {
//        uiWritAble =  FIRST_PIECE;
//    }
//    
//    if(FIRST_PIECE == uiAvailable)
//    {
//        uiWritAble =  SECOND_PIECE;
//    }
//    
//    if(SECOND_PIECE == uiAvailable)
//    {
//        uiWritAble =  FIRST_PIECE;
//    }
//    
//    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
//    {
//        pstOPCVarInfo = &s_stOPCCfg.stOPCVarInfo[uiWritAble];
//    }
//    else
//    {
//        /*log*/
//    }
//
//    return pstOPCVarInfo;
//}
/*
***********************************************************************************************************************
* 函数名称: SharedGetRetainVarInfoWrit
*
* 功能描述: 获取Retain变量配置信息可写片区指针
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
RetainVarInfo_t* SharedGetRetainVarInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    RetainVarInfo_t* pstRetainVarInfo = NULL;

    uiAvailable = atomic32Get(&s_stRetainVarCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstRetainVarInfo = &s_stRetainVarCfg.stRetainVarInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstRetainVarInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCMConfigInfoWrit
*
* 功能描述: 获取CM配置信息可写片区指针
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
CMConfigInfo_t* SharedGetCMConfigInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    CMConfigInfo_t* pstCMConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stCMCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstCMConfigInfo = &s_stCMCfg.stCMConfigInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstCMConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCMSlotIdTablWrit
*
* 功能描述: 获取CM槽位号与CM ID对照表信息可写片区指针
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
CMSlotIdTabl_t* SharedGetCMSlotIdTablWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;

    uiAvailable = atomic32Get(&s_stCMSlotIdTablCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pstCMSlotIdTabl = &s_stCMSlotIdTablCfg.stCMSlotIdTabl[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstCMSlotIdTabl;
}


/*
***********************************************************************************************************************
* 函数名称: SharedSwitchLogicInfo
*
* 功能描述: 切换逻辑配置信息可读片区指针
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
void SharedSwitchLogicInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stLogicCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stLogicCfg.uiAvailable,uiAvailable);
    

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchExLibInfo
*
* 功能描述: 切换外部库配置信息可读片区指针
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
void SharedSwitchExLibInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stExLibCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stExLibCfg.uiAvailable,uiAvailable);
    

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchIoModInfo
*
* 功能描述: 切换IO配置信息可读片区指针
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
void SharedSwitchIoModInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stSysIoModCfg.uiAvailable);
    
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stSysIoModCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchCtrlStationInfo
*
* 功能描述: 切换控制站配置信息可读片区指针
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
void SharedSwitchCtrlStationInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stCtrlStationCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchLogicInfo
*
* 功能描述: 切换SOE配置信息可读片区指针
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
void SharedSwitchSOEConfigInfo (void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stSOECfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stSOECfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchP2PConfigInfo
*
* 功能描述: 切换P2P配置信息可读片区指针
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
void SharedSwitchP2PConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stP2PCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stP2PCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchModbusMasterInfo
*
* 功能描述: 切换Modbus主配置信息可读片区指针
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
void SharedSwitchModbusMasterInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stModbusMasterCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stModbusMasterCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchModbusSlaveInfo
*
* 功能描述: 切换Modbus从配置信息可读片区指针
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
void SharedSwitchModbusSlaveInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stModbusSlaveCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stModbusSlaveCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchVOTEConfigInfo
*
* 功能描述: 切换通道表决配置信息可读片区指针
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
void SharedSwitchVOTEConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stVoteCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stVoteCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchOPCVarInfo
*
* 功能描述: 切换OPC配置信息可读片区指针
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
//void SharedSwitchOPCVarInfo(void)
//{
//    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
//
//    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
//    
//    if(INVALID_BUFF_ID == uiAvailable)
//    {
//        uiAvailable =  FIRST_PIECE;
//    }
//    
//    if(FIRST_PIECE == uiAvailable)
//    {
//        uiAvailable =  SECOND_PIECE;
//    }
//    
//    if(SECOND_PIECE == uiAvailable)
//    {
//        uiAvailable =  FIRST_PIECE;
//    }
//    
//    atomic32Set(&s_stOPCCfg.uiAvailable,uiAvailable);
//
//    return;
//}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchRetainVarInfo
*
* 功能描述: 切换Retain变量配置信息可读片区指针
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
void SharedSwitchRetainVarInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/


    uiAvailable = atomic32Get(&s_stRetainVarCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stRetainVarCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchCMConfigInfo
*
* 功能描述: 切换CM配置信息可读片区指针
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
void SharedSwitchCMConfigInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stCMCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stCMCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchCMSlotIdTabl
*
* 功能描述: 切换CM槽位号与CM ID对照表信息可读片区指针
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
void SharedSwitchCMSlotIdTabl(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stCMSlotIdTablCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stCMSlotIdTablCfg.uiAvailable,uiAvailable);

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchAllConfigInfo
*
* 功能描述: 控制所有类型配置信息，进行统一的切换
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
void SharedSwitchAllConfigInfo(void)
{
    printf("SwitchConfigInfo\n");
    
     SharedSwitchLogicInfo();
     
     SharedSwitchExLibInfo();

     SharedSwitchIoModInfo();

     SharedSwitchCtrlStationInfo();

     SharedSwitchSOEConfigInfo ();

     SharedSwitchP2PConfigInfo();

     SharedSwitchModbusMasterInfo();

     SharedSwitchModbusSlaveInfo();

     SharedSwitchVOTEConfigInfo();

//     SharedSwitchOPCVarInfo();

     SharedSwitchRetainVarInfo();

     SharedSwitchCMConfigInfo();

     SharedSwitchCMSlotIdTabl();
     
     SharedSwitchCodeArea();
}


/*
***********************************************************************************************************************
* 函数名称: SharedGetDownloadFlg
*
* 功能描述: 获取系统下装标志。
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
download_flag_t SharedGetDownloadFlg(void)
{
    download_flag_t emDownloadFlg;
    
    emDownloadFlg = atomic32Get(&s_emDownloadFlg);
    return emDownloadFlg;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDownloadFlg
*
* 功能描述: 设置系统下装标志。
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
void SharedSetDownloadFlg(download_flag_t emDownloadFlg)
{
    if((DOWN_LOAD_FILE_DONE == emDownloadFlg) ||(DOWN_LOAD_COMPLETE == emDownloadFlg) || \
       (NOT_DOWNLOAD == emDownloadFlg) ||(INIT_DOWNLOAD == emDownloadFlg) || (ONLINE_DOWNLOAD == emDownloadFlg))
    {
        atomic32Set(&s_emDownloadFlg, emDownloadFlg);

    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPrjShedTime
*
* 功能描述: 获取指定任务ID的调度周期。
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
uint32_t SysGetPrjShedTime(Task_ID_t emTaskID)
{
    uint32_t PrjShedTime = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(emTaskID);
    
        if(pstCtrlStationConfigInfo != NULL)
        {
            PrjShedTime = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiIECPeriod;
            
        }
    }
    
    return PrjShedTime;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetIsConfigured
*
* 功能描述: 获取本机PM是否有可用的配置信息。
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
uint32_t SysGetIsConfigured(void)
{
    uint32_t uiIsConfigured = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    
    if(pstCtrlStationConfigInfo != NULL)
    {
        uiIsConfigured = pstCtrlStationConfigInfo->uiIsConfigured;
    }
    
    return uiIsConfigured;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetIsConfigured
*
* 功能描述: 设置本机PM是否有可用的配置信息。
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
void SysSetIsConfigured(uint32_t uiIsConfigured)
{
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    
    if(pstCtrlStationConfigInfo != NULL)
    {
        //printf("uiIsConfigured addr: 0x%x",&pstCtrlStationConfigInfo->uiIsConfigured);
        atomic32Set(&pstCtrlStationConfigInfo->uiIsConfigured, uiIsConfigured);
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetCodeArea
*
* 功能描述: 获取可执行Code区首地址
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
puint8_t SharedGetCodeArea(Task_ID_t emTaskID)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    puint8_t pucCodeArea = NULL;

    if(emTaskID < MAX_TASK_NUM)
    {
        if((OS_SWITCH_START == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(emTaskID)) || \
           (OS_CONFIG_SWITCH == SharedGetOnlineDownLoadSwitchFlg(emTaskID)))
        {
            uiAvailable = atomic32Get(&s_stCodeArea.uiTmpAvailable[emTaskID]);
//            printf("CD: Available %x %x\n",uiAvailable,atomic32Get(&s_stCodeArea.uiAvailable));
        }
        else
        {
            uiAvailable = atomic32Get(&s_stCodeArea.uiAvailable);
        }
    }
    else
    {
        uiAvailable = atomic32Get(&s_stCodeArea.uiAvailable);
    }
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pucCodeArea = (puint8_t)&s_stCodeArea.ucCodeArea[uiAvailable][0];
    }
    else
    {
        /*log*/
    }

    return pucCodeArea;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetCodeAreaWrit
*
* 功能描述: 获取可写Code区首地址
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
puint8_t SharedGetCodeAreaWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    puint8_t pucCodeArea = NULL;

    uiAvailable = atomic32Get(&s_stCodeArea.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiWritAble =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiWritAble =  FIRST_PIECE;
    }
    
    if((FIRST_PIECE == uiWritAble) || (SECOND_PIECE == uiWritAble))
    {
        pucCodeArea = (puint8_t)&s_stCodeArea.ucCodeArea[uiWritAble][0];
    }
    else
    {
        /*log*/
    }

    return pucCodeArea;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSwitchCodeArea
*
* 功能描述: 切换Code区可读片区指针
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
void SharedSwitchCodeArea(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stCodeArea.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    else if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    else
    {
        uiAvailable = INVALID_BUFF_ID;
    }
    
    atomic32Set(&s_stCodeArea.uiAvailable,uiAvailable);
    

    return;
}



/*
***********************************************************************************************************************
* 函数名称: SharedSwitchPosInit
*
* 功能描述: 初始化钥匙开关位置
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
void SharedSwitchPosInit(void)
{
    
    uint32_t uiSwitchPos;
    
    uiSwitchPos = 0U;
    
    atomic32Set(&s_uiSwitchPos, uiSwitchPos);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetSwitchPos
*
* 功能描述: 从系统资源获取钥匙开关位置
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
uint32_t SharedGetSwitchPos(void)
{
    uint32_t uiSwitchPos;
    
    uiSwitchPos = atomic32Get(&s_uiSwitchPos);
    
//    printf(" @@@@@  uiSwitchPos is : %x \n",uiSwitchPos);
    
    return uiSwitchPos;

}

/*
***********************************************************************************************************************
* 函数名称: SharedSetSwitchPos
*
* 功能描述: 设置要是开关
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
void SharedSetSwitchPos(uint32_t uiSwitchPos)
{
    atomic32Set(&s_uiSwitchPos, uiSwitchPos);
    
    //printf(" &&&&&&&  s_uiSwitchPos is : %x \n",s_uiSwitchPos);
    
    return;

}

/*
***********************************************************************************************************************
* 函数名称: SharedSetLocalPMID
*
* 功能描述: 设置本地PM的ID
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
void SharedSetLocalPMID(PMController_t emLocalID)
{

    if(emLocalID < NUM_OF_PM)
    {
        //s_emLocalID = emLocalID;
        atomic32Set(&s_emLocalID, (uint32_t)emLocalID);
    }
    else
    {
        //s_emLocalID = INVALID_PM_ID;
        atomic32Set(&s_emLocalID, (uint32_t)INVALID_PM_ID);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetLocalPMID
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
PMController_t SharedGetLocalPMID(void)
{
    //return s_emLocalID;
    uint32_t uiLocalIDTmp;
    PMController_t emLocalID = INVALID_PM_ID;
    
    uiLocalIDTmp = atomic32Get(&s_emLocalID);
    if(uiLocalIDTmp < NUM_OF_PM)
    {
        emLocalID = (PMController_t)uiLocalIDTmp;
    }

    return emLocalID;

}
/*
***********************************************************************************************************************
* 函数名称: SharedGetTaskConfigInfo
*
* 功能描述: 获取指定Task的配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 指定Task的配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/
TaskConfigInfo_t* SharedGetTaskConfigInfo(Task_ID_t emTaskID)
{
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(emTaskID);
        
        if(pstCtrlStationConfigInfo != NULL)
        {
            pstTaskConfigInfo = (TaskConfigInfo_t*)&pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID];
        }
    }
    
    return pstTaskConfigInfo;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetTaskCodeEntry
*
* 功能描述: 获取指定Task的执行入口
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 指定Task的配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetTaskCodeEntry(Task_ID_t emTaskID)
{
    uint32_t uiCodeEntry = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    puint8_t pucCodeAreaAddr = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(emTaskID);
        
        if((pstCtrlStationConfigInfo != NULL) && (ACTIVE == pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag))
        {
            pucCodeAreaAddr = SharedGetCodeArea(emTaskID);
            if(pucCodeAreaAddr != NULL)
            {
                uiCodeEntry = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiTaskCodeEntry;
                uiCodeEntry += (uint32_t)pucCodeAreaAddr;
                
//                if((uiCodeEntry < 0x10000000) || (uiCodeEntry > 0x20000000))
//                {
//                    printf("CodeEntry err:%d\n",uiCodeEntry);
//                }
            }
        }
        else
        {
            printf("CS Info%d %d %d\n",emTaskID,pstCtrlStationConfigInfo,pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag);
        }
    }
    return uiCodeEntry;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetTaskGlobalInitEntry
*
* 功能描述: 获取指定Task的GlobalInit的入口
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 指定Task的GlobalInit的入口
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetTaskGlobalInitEntry(Task_ID_t emTaskID)
{
    uint32_t uiGlobalInitEntry = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    puint8_t pucCodeAreaAddr = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(emTaskID);
        
        if((pstCtrlStationConfigInfo != NULL) && (ACTIVE == pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag))
        {
            pucCodeAreaAddr = SharedGetCodeArea(emTaskID);
            if(pucCodeAreaAddr != NULL)
            {
                uiGlobalInitEntry = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiGlobalInitEntry;
                uiGlobalInitEntry += (uint32_t)pucCodeAreaAddr;
            }
            
        }
    }
    
    return uiGlobalInitEntry;

}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTaskChangeGlobalInitEntry
*
* 功能描述: 获取指定Task的ChangeGlobalInit的入口
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 指定Task的ChangeGlobalInit的入口
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetTaskChangeGlobalInitEntry(Task_ID_t emTaskID)
{
    uint32_t uiChangeGlobalInitEntry = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    puint8_t pucCodeAreaAddr = NULL;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(emTaskID);
        
        if((pstCtrlStationConfigInfo != NULL) && (ACTIVE == pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag))
        {
            pucCodeAreaAddr = SharedGetCodeArea(emTaskID);
            if(pucCodeAreaAddr != NULL)
            {
                uiChangeGlobalInitEntry = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiChangeGlobalInitEntry;
                uiChangeGlobalInitEntry += (uint32_t)pucCodeAreaAddr;
            }
        }
    }
    
    return uiChangeGlobalInitEntry;

}
/*
***********************************************************************************************************************
* 函数名称: SysGetTaskStatus
*
* 功能描述: 获取指定PM内指定任务的任务状态
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
task_status_t SysGetTaskStatus(PMController_t emPMId, Task_ID_t emTaskID)
{
    //task_status_t ts= (task_status_t)atomic32Get(&s_emTaskStatus[emPMId][emTaskID]);
    //printf("ts=%d, pmid=%d, tid=%d\n", ts, emPMId, emTaskID);
    uint32_t uiTaskStaTmp = 0U;
    task_status_t emTaskSta = TS_INIT;
    if((emPMId < NUM_OF_PM) && (emTaskID < MAX_TASK_NUM))
    {
        uiTaskStaTmp = atomic32Get(&s_emTaskStatus[emPMId][emTaskID]);
        if(uiTaskStaTmp < MAX_TASK_STATUS)
        {
            emTaskSta = (task_status_t)uiTaskStaTmp;
        }
        else
        {
            emTaskSta = MAX_TASK_STATUS;
        }
    }
    return emTaskSta;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetTaskStatus
*
* 功能描述: 设置指定PM内指定任务的任务状态
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
void SysSetTaskStatus(PMController_t emPMId, Task_ID_t emTaskID, task_status_t emTaskStatus)
{
    if((emPMId >= PMA) && (emPMId < NUM_OF_PM) && (emTaskID >= TASK_UP1) && (emTaskID < MAX_TASK_NUM))
    {
        if(emTaskStatus != atomic32Get(&s_emTaskStatus[emPMId][emTaskID]))
        {
            printf("Task%d Stat %d\n",emTaskID,emTaskStatus);
            if(TS_STOP == emTaskStatus)
            {
                SysSetOutputPollFlg(emTaskID, OUTPUT_POLL_DISENABLE);
            }
        }
//        s_emTaskStatus[emPMId][emTaskID] = emTaskStatus;
        atomic32Set(&s_emTaskStatus[emPMId][emTaskID], emTaskStatus);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetTaskSingleStepFlg
*
* 功能描述: 获取指定任务的单步状态
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
uint32_t SysGetTaskSingleStepFlg(Task_ID_t emTaskID)
{
    uint32_t uiSingleStepFlg = 0;
    if(emTaskID < MAX_TASK_NUM)
    {
        uiSingleStepFlg = atomic32Get(&s_uiSingleStepFlg[emTaskID]);
    }
    return uiSingleStepFlg;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetTaskSingleStepFlg
*
* 功能描述: 设置指定任务的单步状态
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
void SysSetTaskSingleStepFlg(Task_ID_t emTaskID, uint32_t uiSingleStepFlg)
{
    if((emTaskID < MAX_TASK_NUM) && (uiSingleStepFlg <= 1))
    {
        atomic32Set(&s_uiSingleStepFlg[emTaskID], uiSingleStepFlg);
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTaskNum
*
* 功能描述: 获取任务个数
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
uint8_t SharedGetTaskNum(void)
{
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    PMController_t emPMID;
    uint8_t ucTaskNum = 0;

    emPMID = SharedGetLocalPMID();

    if((emPMID < NUM_OF_PM) && (emPMID >= PMA))
    {
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);

        if(pstCtrlStationConfigInfo != NULL)
        {
            ucTaskNum = pstCtrlStationConfigInfo->ucTaskNum;
        }
    }

    return ucTaskNum;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetSyncDataFlag
*
* 功能描述: 获取UP任务是否被同步过数据的标志
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
sync_data_flag_t SharedGetSyncDataFlag(Task_ID_t emTaskID)
{
    sync_data_flag_t emSyncDataFlag = DATA_NOT_SYNC;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        emSyncDataFlag = atomic32Get(&s_emSyncDataFlag[emTaskID]);
    }

    return emSyncDataFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetSyncDataFlag
*
* 功能描述: 设置UP任务是否被同步过数据的标志
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
void SharedSetSyncDataFlag(Task_ID_t emTaskID, sync_data_flag_t emSyncDataFlag)
{
    
    if((emTaskID < MAX_TASK_NUM) && (emSyncDataFlag <= DATA_SYNC_COMPLETE))
    {
        atomic32Set(&s_emSyncDataFlag[emTaskID],emSyncDataFlag);
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModType
*
* 功能描述: 获取指定地址的模块的类型
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t SharedGetModType(Task_ID_t emTaskID,uint8_t ucModID)
{
    uint16_t usModType = 0U;
    SysIoModInfo_t* pstIoModInfo = NULL;
    
    pstIoModInfo = SharedGetIoModInfo(emTaskID);
    
    if((pstIoModInfo != NULL) && (ucModID <= MAX_OUTDEV_NUM))
    {
        usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].usModTag;
    }
    
    return usModType;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetVoteInfo
*
* 功能描述: 获取指定地址的模块的表决信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
VoteModInfo_t* SharedGetVoteInfo(uint8_t ucModID)
{
    VOTEConfigInfo_t* pstVOTEInfo = NULL;
    VoteModInfo_t* pstVoteModInfo = NULL;
    
    pstVOTEInfo = SharedGetVOTEConfigInfo();
    
    if((pstVOTEInfo != NULL) && (ucModID < MAX_OUTDEV_NUM))
    {
        pstVoteModInfo = (VoteModInfo_t*)&pstVOTEInfo->stVoteModInfo[ucModID];
    }
    
    return pstVoteModInfo;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTolerate
*
* 功能描述: 获取容忍次数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t SharedGetTolerate(void)
{
    uint8_t ucTolerate = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;
    
    pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    
    if((pstCtrlStationInfo != NULL))
    {
        ucTolerate = (uint8_t)pstCtrlStationInfo->stStationConfigInfo.uiTolerate;
    }
    
    return ucTolerate;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetSOEType
*
* 功能描述: 获取获取SOE块的软硬属性
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
SOEType_t SharedGetSOEType(uint8_t ucSOEBlockID)
{
    SOEType_t emSOEType = INVALID_SOE_TYPE;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    
    pstSOEConfigInfo = SharedGetSOEConfigInfo(MAX_TASK_NUM);
    
    
    if(MAX_SOE_BLOCK_NUM == ucSOEBlockID)
    {
        emSOEType = HW_SOE;
    }
    else
    {
        if((pstSOEConfigInfo != NULL) && (ucSOEBlockID < MAX_SOE_BLOCK_NUM))
        {
            if(ACTIVE == pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockID].emActive)
            {
                emSOEType = SW_SOE;
            }
        }
    }
    
    return emSOEType;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetSOEActive
*
* 功能描述: 获取获取SOE块是否有配置信息的标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
ActiveFlag_t SharedGetSOEActive(uint8_t ucSOEBlockID)
{
    ActiveFlag_t emActive = NOT_ACTIVE;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    
    pstSOEConfigInfo = SharedGetSOEConfigInfo(MAX_TASK_NUM);
    
    if(MAX_SOE_BLOCK_NUM == ucSOEBlockID)
    {
        emActive = ACTIVE;
    }
    else
    {
        if((pstSOEConfigInfo != NULL) && (ucSOEBlockID < MAX_SOE_BLOCK_NUM))
        {
            emActive = (ActiveFlag_t)pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockID].emActive;
        }
    }
    
    return emActive;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PSendBlockNum
*
* 功能描述: 获取P2P发送块的个数
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
uint32_t SharedGetP2PSendBlockNum(void)
{
    uint32_t uiP2PSendBlockNum = 0U;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    

    if(pstP2PConfigInfo != NULL)
    {
        uiP2PSendBlockNum = pstP2PConfigInfo->stP2PConfigHead.uiSendBlockNum;
    }
    
    return uiP2PSendBlockNum;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PBlockSendCycle
*
* 功能描述: 获取P2P块当发送周期: ms
*
* 输入参数: ucP2PBlockIndex: 0-63
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetP2PBlockSendCycle(uint8_t ucP2PBlockIndex)
{
    uint32_t uiPeriod = 0U;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    

    if((pstP2PConfigInfo != NULL) && (ucP2PBlockIndex < MAX_P2P_BLOCK_NUM))
    {
        if(ACTIVE == pstP2PConfigInfo->stP2PSendBlockInfo[ucP2PBlockIndex].emActive)
        {
            uiPeriod = pstP2PConfigInfo->stP2PSendBlockInfo[ucP2PBlockIndex].stP2PBlockHead.uiPeriod;  /* ms */
        }
    }
    
    return uiPeriod;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PBlockSendActive
*
* 功能描述: 获取P2P发送块的是否Active
*
* 输入参数: ucP2PBlockIndex: 0-63
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
ActiveFlag_t SharedGetP2PBlockSendActive(uint8_t ucP2PBlockIndex)
{
    ActiveFlag_t emActiveFlag = NOT_ACTIVE;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    

    if((pstP2PConfigInfo != NULL) && (ucP2PBlockIndex < MAX_P2P_BLOCK_NUM))
    {
        emActiveFlag = pstP2PConfigInfo->stP2PSendBlockInfo[ucP2PBlockIndex].emActive;
    }
    
    return emActiveFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PBlockRecvActive
*
* 功能描述: 获取P2P接收块的是否Active
*
* 输入参数: ucP2PBlockIndex: Station ID, 1-64
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
ActiveFlag_t SharedGetP2PBlockRecvActive(uint8_t ucP2PBlockIndex)
{
    ActiveFlag_t emActiveFlag = NOT_ACTIVE;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    
    /* Changed by wangdong: <  ->  <= */
    if((pstP2PConfigInfo != NULL) && (ucP2PBlockIndex <= MAX_P2P_BLOCK_NUM))
    {
        emActiveFlag = pstP2PConfigInfo->stP2PRecvBlockInfo[ucP2PBlockIndex].emActive;
    }
    
    return emActiveFlag;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PSendBlockInfo
*
* 功能描述: 获取P2P发送块的配置信息
*
* 输入参数: ucP2PBlockIndex：目的控制站（接收此请求帧的控制站）的 索引值 0-63，非站ID
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
P2PBlock_t* SharedGetP2PSendBlockInfo(uint8_t ucP2PBlockIndex)
{
    P2PBlock_t* pstP2PBlock = NULL;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;

    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    

    if((pstP2PConfigInfo != NULL) && (ucP2PBlockIndex < MAX_P2P_BLOCK_NUM))
    {
        if(ACTIVE == pstP2PConfigInfo->stP2PSendBlockInfo[ucP2PBlockIndex].emActive)
        {
            pstP2PBlock = (P2PBlock_t*)&pstP2PConfigInfo->stP2PSendBlockInfo[ucP2PBlockIndex];
        }
    }
    
    return pstP2PBlock;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetP2PRecvBlockInfo
*
* 功能描述: 获取P2P接收块的配置信息
*
* 输入参数: ucP2PBlockIndex: Station ID, 1-64
*
* 输出参数: 无
*
* 返 回 值  : 模块的类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
P2PBlock_t* SharedGetP2PRecvBlockInfo(uint8_t ucP2PBlockIndex)
{
    P2PBlock_t* pstP2PBlock = NULL;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    
    /* Changed by wangdong: <  ->  <= */
    if((pstP2PConfigInfo != NULL) && (ucP2PBlockIndex <= MAX_P2P_BLOCK_NUM))
    {
        if(ACTIVE == pstP2PConfigInfo->stP2PRecvBlockInfo[ucP2PBlockIndex].emActive)
        {
            pstP2PBlock = (P2PBlock_t*)&pstP2PConfigInfo->stP2PRecvBlockInfo[ucP2PBlockIndex];
        }
    }
    
    return pstP2PBlock;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetStationID
*
* 功能描述: 获取控制站ID号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 控制站ID号
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetStationID(void)
{
    uint32_t uiStationID = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);

    if(pstCtrlStationConfigInfo != NULL)
    {
        uiStationID = pstCtrlStationConfigInfo->stStationConfigInfo.uiStationID;
    }
    
    return uiStationID;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetTolCnt
*
* 功能描述: 获取容忍次数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 容忍次数
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetTolCnt(void)
{
    uint32_t uiTolCnt = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);

    if(pstCtrlStationConfigInfo != NULL)
    {
        uiTolCnt = pstCtrlStationConfigInfo->stStationConfigInfo.uiTolerate;
    }

    return uiTolCnt;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetConfigCrc
*
* 功能描述: 获取指定文件配置信息结构内记录的文件CRC
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 文件CRC
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetConfigCrc(SysCfgFileType_t emCfgFileType)
{
    uint32_t uiFileCrc = 0U;
    LogicConfigInfo_t* pstLogicConfigInfo = NULL;
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;
    VOTEConfigInfo_t* pstVOTEConfigInfo = NULL;
    RetainVarInfo_t* pstRetainVarInfo = NULL;
    CMConfigInfo_t* pstCMConfigInfo = NULL;

    switch(emCfgFileType)
    {
        case CFG_PRG:
        {
            pstLogicConfigInfo = SharedGetLogicInfoWrit();
        
            if(pstLogicConfigInfo != NULL)
            {
                uiFileCrc = pstLogicConfigInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_IO_MODULAR:
        {
            pstSysIoModInfo = SharedGetIoModInfoWrit();
        
            if(pstSysIoModInfo != NULL)
            {
                uiFileCrc = pstSysIoModInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_SOE:
        {
            pstSOEConfigInfo = SharedGetSOEConfigInfoWrit();
        
            if(pstSOEConfigInfo != NULL)
            {
                uiFileCrc = pstSOEConfigInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_P2P:
        {
            pstP2PConfigInfo = SharedGetP2PConfigInfoWrit();
        
            if(pstP2PConfigInfo != NULL)
            {
                uiFileCrc = pstP2PConfigInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_MODBUS_MASTER:
        {
            pstModbusMasterInfo = SharedGetModbusMasterInfoWrit();
        
            if(pstModbusMasterInfo != NULL)
            {
                uiFileCrc = pstModbusMasterInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_MODBUS_SLAVE:
        {
            pstModbusSlaveInfo = SharedGetModbusSlaveInfoWrit();
        
            if(pstModbusSlaveInfo != NULL)
            {
                uiFileCrc = pstModbusSlaveInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_VOTE:
        {
            pstVOTEConfigInfo = SharedGetVOTEConfigInfoWrit();
        
            if(pstVOTEConfigInfo != NULL)
            {
                uiFileCrc = pstVOTEConfigInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_RETAIN:
        {
            pstRetainVarInfo = SharedGetRetainVarInfoWrit();
        
            if(pstRetainVarInfo != NULL)
            {
                uiFileCrc = pstRetainVarInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        case CFG_CM:
        {
            pstCMConfigInfo = SharedGetCMConfigInfoWrit();
        
            if(pstCMConfigInfo != NULL)
            {
                uiFileCrc = pstCMConfigInfo->uiCRC;
            }
            else
            {
                printf("ConfigInfo NULL:%d\n",emCfgFileType);
            }
        }
        break;
        default :
        {
            ;
        }
        break;
    }
    
    return uiFileCrc;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDownloadPM
*
* 功能描述: 获取接收1131下装的PM ID号
*
* 输入参数:  
*
* 输出参数: PM ID号
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
PMController_t SharedGetDownloadPM(void)
{
    return atomic32Get(&s_emDownloadPMID);
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDownloadPM
*
* 功能描述: 设置接收1131下装的PM ID号
*
* 输入参数:  PM ID号
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetDownloadPM(PMController_t emDownloadPMID)
{
    if(emDownloadPMID < NUM_OF_PM)
    {
        atomic32Set(&s_emDownloadPMID, emDownloadPMID);
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetPowerOnConfigSwitchFlg
*
* 功能描述: 获取上电配置切换标志
*
* 输入参数:  
*
* 输出参数: 上电配置切换标志
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetPowerOnConfigSwitchFlg(void)
{
    return atomic32Get(&s_uiPowerOnConfigSwitch);
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetPowerOnConfigSwitchFlg
*
* 功能描述: 设置上电配置切换标志
*
* 输入参数:  上电配置切换标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetPowerOnConfigSwitchFlg(uint32_t uiPowerOnConfigSwitch)
{
    if((NOT_NEED_POWERON_CONIFG_SWITCH == uiPowerOnConfigSwitch) || (NEED_POWERON_CONIFG_SWITCH == uiPowerOnConfigSwitch))
    {
        atomic32Set(&s_uiPowerOnConfigSwitch, uiPowerOnConfigSwitch);
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetWorkStatus
*
* 功能描述: 获取指定PM的工作状态
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
work_status_t SysGetWorkStatus(PMController_t emPMId)
{
    work_status_t emWork_Status = WS_OFFLINE ;
    
    if(emPMId < NUM_OF_PM)
    {
        emWork_Status = atomic32Get(&s_emWorkStatus[emPMId]);
    }
    
    return emWork_Status;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetWorkStatus
*
* 功能描述: 设置指定PM的工作状态
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
void SysSetWorkStatus(PMController_t emPMId, work_status_t emWork_Status)
{
    
    if((emPMId < NUM_OF_PM)&& (emWork_Status < MAX_WORK_STATUS))
    {
        atomic32Set(&s_emWorkStatus[emPMId],emWork_Status);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetWRParaBufID
*
* 功能描述: 获取双口上可写的参数片区
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
uint8_t SharedGetWRParaBufID()
{
    uint8_t ucWRParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
    
    if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
    {/*当进行在线下装时，参数可写片区进行切换*/
        if(IPBUS_SLAVE_ENABLE_BUF0 == s_uiUsedParaBufID)
        {
            ucWRParaBufID = IPBUS_SLAVE_ENABLE_BUF1;
        }
        else if(IPBUS_SLAVE_ENABLE_BUF1 == s_uiUsedParaBufID)
        {
            ucWRParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
        }
        else
        {
            ucWRParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
        }
    }
    else
    {
        ucWRParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
    }
    
    return ucWRParaBufID;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSwitchUsedParaBufID
*
* 功能描述: 对双口上使用的片区进行切换
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
void SharedSwitchUsedParaBufID()
{

    if(IPBUS_SLAVE_ENABLE_BUF0 == s_uiUsedParaBufID)
    {
        s_uiUsedParaBufID = IPBUS_SLAVE_ENABLE_BUF1;
    }
    else if(IPBUS_SLAVE_ENABLE_BUF1 == s_uiUsedParaBufID)
    {
        s_uiUsedParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
    }
    else
    {
        s_uiUsedParaBufID = IPBUS_SLAVE_ENABLE_BUF0;
    }
    
    printf("UsedParaBufID: %d\n",s_uiUsedParaBufID);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetOnlineDownLoadSwitchFlg
*
* 功能描述: 获取在线下装工程切换标志
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
OnlineDownLoad_Switch_Flg_t SharedGetOnlineDownLoadSwitchFlg(Task_ID_t emTaskID)
{
    OnlineDownLoad_Switch_Flg_t emOnlineDownLoadSwitchFlg = OS_NOT_NEED_SWITCH;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        emOnlineDownLoadSwitchFlg = atomic32Get(&s_emOnlineDownLoadSwitchFlg[emTaskID]);
    }
    
    return emOnlineDownLoadSwitchFlg;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSetOnlineDownLoadSwitchFlg
*
* 功能描述: 设置在线下装工程切换标志
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
void SharedSetOnlineDownLoadSwitchFlg(Task_ID_t emTaskID, OnlineDownLoad_Switch_Flg_t emOnlineDownLoadSwitchFlg)
{
    
    if((emTaskID < MAX_TASK_NUM) && 
       ((OS_NOT_NEED_SWITCH == emOnlineDownLoadSwitchFlg) || (OS_SWITCH_START == emOnlineDownLoadSwitchFlg) ||
        (OS_SLAVE_ENABLE == emOnlineDownLoadSwitchFlg) || (OS_CONFIG_SWITCH == emOnlineDownLoadSwitchFlg) || 
        (OS_CODE_SWITCH == emOnlineDownLoadSwitchFlg)))
    {
        atomic32Set(&s_emOnlineDownLoadSwitchFlg[emTaskID], emOnlineDownLoadSwitchFlg);
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedRecordTmpAvailable
*
* 功能描述: 用于在线下装时记录旧配置片区
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
void SharedRecordTmpAvailable(Task_ID_t emTaskID)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;
    
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_stSysIoModCfg.uiTmpAvailable[emTaskID], atomic32Get(&s_stSysIoModCfg.uiAvailable));
        atomic32Set(&s_stCtrlStationCfg.uiTmpAvailable[emTaskID], atomic32Get(&s_stCtrlStationCfg.uiAvailable));
        atomic32Set(&s_stCodeArea.uiTmpAvailable[emTaskID], atomic32Get(&s_stCodeArea.uiAvailable));
        atomic32Set(&s_stSOECfg.uiTmpAvailable[emTaskID], atomic32Get(&s_stSOECfg.uiAvailable));
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedClearIOTmpAvailable
*
* 功能描述: 清除IO配置的临时记录标志
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
void SharedClearIOTmpAvailable(Task_ID_t emTaskID)
{
    
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_stSysIoModCfg.uiTmpAvailable[emTaskID], INVALID_BUFF_ID);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedClearCSTmpAvailable
*
* 功能描述: 清除控制站配置的临时记录标志
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
void SharedClearCSTmpAvailable(Task_ID_t emTaskID)
{
    
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_stCtrlStationCfg.uiTmpAvailable[emTaskID], INVALID_BUFF_ID);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedClearCodeTmpAvailable
*
* 功能描述: 清除Code区的临时记录标志
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
void SharedClearCodeTmpAvailable(Task_ID_t emTaskID)
{
    
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_stCodeArea.uiTmpAvailable[emTaskID], INVALID_BUFF_ID);
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedClearSOETmpAvailable
*
* 功能描述: 清除SOE配置的临时记录标志
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
void SharedClearSOETmpAvailable(Task_ID_t emTaskID)
{
    
    if(emTaskID < MAX_TASK_NUM)
    {
        atomic32Set(&s_stSOECfg.uiTmpAvailable[emTaskID], INVALID_BUFF_ID);
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedOnlineDownLoadFillDualDataTable
*
* 功能描述: 在线下装时，根据配置填充从站输入输出区基地址及长度数据表
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
void SharedOnlineDownLoadFillDualDataTable(Task_ID_t emTaskID)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint8_t ucSlaveNo = 0U;

    pstSysIoModInfo = SharedGetIoModInfo(emTaskID);
    if (pstSysIoModInfo == NULL)
    {

    }
    else
    {
        /* 填充输入从站双口输入区基地址数据表、长度数据表；输入从站双口输出区基地址数据表、长度数据表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[emTaskID]; i++)
        {
            ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[emTaskID][i];
            IPBUSSetSlaveInputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset);
            IPBUSSetSlaveInputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
            IPBUSSetSlaveOutputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset);
            IPBUSSetSlaveOutputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
        }

        /* 填充输出从站双口输入区基地址数据表、长度数据表；输出从站双口输出区基地址数据表、长度数据表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[emTaskID]; i++)
        {
            ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[emTaskID][i];
            IPBUSSetSlaveInputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset);
            IPBUSSetSlaveInputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
            IPBUSSetSlaveOutputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset);
            IPBUSSetSlaveOutputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedModConfigChangeInit
*
* 功能描述: 模块的参数变化标志初始化
*
* 输入参数: 无
*
* 输出参数: bool_t
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedModConfigChangeInit(void)
{
    memset(&s_bModConfigChangeFlg[0], 0x00, sizeof(bool_t)*MAX_MOD_NUM);
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetModConfigChangeFlg
*
* 功能描述: 获取指定模块的参数变化标志
*
* 输入参数: 无
*
* 输出参数: bool_t
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedGetModConfigChangeFlg(uint8_t ucModAddr)
{
    bool_t bModConfigChangeFlg = false;
    
    if(ucModAddr < MAX_MOD_NUM)
    {
        bModConfigChangeFlg = s_bModConfigChangeFlg[ucModAddr];
    }
    
    return bModConfigChangeFlg;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSetModConfigChangeFlg
*
* 功能描述: 设置指定模块的参数变化标志
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
void SharedSetModConfigChangeFlg(uint8_t ucModAddr, bool_t bModConfigChangeFlg)
{
    
    if((ucModAddr < MAX_MOD_NUM) && 
       ((true == bModConfigChangeFlg) || (false == bModConfigChangeFlg)))
    {
        s_bModConfigChangeFlg[ucModAddr] = bModConfigChangeFlg;
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDisableRecoverFlg
*
* 功能描述: 获取Disable表恢复标志
*
* 输入参数:  
*
* 输出参数: Disable表恢复标志
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t SharedGetDisableRecoverFlg(void)
{
    return atomic32Get(&s_uiDisableRecover);
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetDisableRecoverFlg
*
* 功能描述: 设置Disable表恢复标志
*
* 输入参数:  Disable表恢复标志
*
* 输出参数: 
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SharedSetDisableRecoverFlg(uint32_t uiDisableRecover)
{
    if((NOT_NEED_DISABLE_RECOVER == uiDisableRecover) || (NEED_DISABLE_RECOVER == uiDisableRecover))
    {
        atomic32Set(&s_uiDisableRecover, uiDisableRecover);
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
