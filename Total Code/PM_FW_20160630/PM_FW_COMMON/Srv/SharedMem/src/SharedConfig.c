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
#include "../inc/SharedConfig.h"


/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static LogicCfg_t s_stLogicCfg;
static SysIoModCfg_t s_stSysIoModCfg;
static CtrlStationCfg_t s_stCtrlStationCfg;
static SOECfg_t s_stSOECfg;
static P2PCfg_t s_stP2PCfg;
static ModbusMasterCfg_t s_stModbusMasterCfg;
static ModbusSlaveCfg_t s_stModbusSlaveCfg;
static VoteCfg_t s_stVoteCfg;
static OPCCfg_t s_stOPCCfg;
static RetainVarCfg_t s_stRetainVarCfg;
static CMCfg_t s_stCMCfg;
static CMSlotIdTablCfg_t s_stCMSlotIdTablCfg;

static download_flag_t s_emDownloadFlg;     /* 系统下装标志*/

static CodeArea_t s_stCodeArea;

static uint32_t s_uiSwitchPos;

static PMController_t s_emLocalID;

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
    /* 初始化 */
    memset(&s_stLogicCfg, 0x00, sizeof(LogicCfg_t));
    memset(&s_stSysIoModCfg, 0x00, sizeof(SysIoModCfg_t));
    memset(&s_stCtrlStationCfg, 0x00, sizeof(CtrlStationCfg_t));
    memset(&s_stSOECfg, 0x00, sizeof(SOECfg_t));
    memset(&s_stP2PCfg, 0x00, sizeof(P2PCfg_t));
    memset(&s_stModbusMasterCfg, 0x00, sizeof(ModbusMasterCfg_t));
    memset(&s_stModbusSlaveCfg, 0x00, sizeof(ModbusSlaveCfg_t));
    memset(&s_stVoteCfg, 0x00, sizeof(VoteCfg_t));
    memset(&s_stOPCCfg, 0x00, sizeof(OPCCfg_t));
    memset(&s_stRetainVarCfg, 0x00, sizeof(RetainVarCfg_t));
    memset(&s_stCMCfg, 0x00, sizeof(CMCfg_t));
    memset(&s_stCMSlotIdTablCfg, 0x00, sizeof(CMSlotIdTablCfg_t));
    memset(&s_stCodeArea, 0x00, sizeof(CodeArea_t));

    /*初始化各类配置信息的可读片区为无效片区*/
    s_stLogicCfg.uiAvailable = INVALID_BUFF_ID;
    s_stSysIoModCfg.uiAvailable = INVALID_BUFF_ID;
    s_stCtrlStationCfg.uiAvailable = INVALID_BUFF_ID;
    s_stSOECfg.uiAvailable = INVALID_BUFF_ID;
    s_stP2PCfg.uiAvailable = INVALID_BUFF_ID;
    s_stModbusMasterCfg.uiAvailable = INVALID_BUFF_ID;
    s_stModbusSlaveCfg.uiAvailable = INVALID_BUFF_ID;
    s_stVoteCfg.uiAvailable = INVALID_BUFF_ID;
    s_stOPCCfg.uiAvailable = INVALID_BUFF_ID;
    s_stRetainVarCfg.uiAvailable = INVALID_BUFF_ID;
    s_stCMCfg.uiAvailable = INVALID_BUFF_ID;
    s_stCMSlotIdTablCfg.uiAvailable = INVALID_BUFF_ID;
    s_stCodeArea.uiAvailable = INVALID_BUFF_ID;

    s_emDownloadFlg = NOT_DOWNLOAD;
    
    atomic32Set(&s_uiSwitchPos, 0U);
    
    atomic32Set(&s_emLocalID, PMA);


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
SysIoModInfo_t* SharedGetIoModInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    uiAvailable = atomic32Get(&s_stSysIoModCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstSysIoModInfo = &s_stSysIoModCfg.stSysIoModInfo[uiAvailable];
        //printf("GET IO: uiAvailable is %x\n",uiAvailable);
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
CtrlStationConfigInfo_t* SharedGetCtrlStationInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stCtrlStationCfg.uiAvailable);
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
SOEConfigInfo_t* SharedGetSOEConfigInfo (void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;

    uiAvailable = atomic32Get(&s_stSOECfg.uiAvailable);
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
OPCVarInfo_t* SharedGetOPCVarInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    OPCVarInfo_t* pstOPCVarInfo = NULL;

    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
    if(FIRST_PIECE == uiAvailable || SECOND_PIECE == uiAvailable)
    {
        pstOPCVarInfo = &s_stOPCCfg.stOPCVarInfo[uiAvailable];
    }
    else
    {
        /*log*/
    }

    return pstOPCVarInfo;
}
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
OPCVarInfo_t* SharedGetOPCVarInfoWrit(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    uint32_t uiWritAble = INVALID_BUFF_ID;         /*乒乓buffer中可写的配置信息片区ID*/
    OPCVarInfo_t* pstOPCVarInfo = NULL;

    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
    
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
    {
        pstOPCVarInfo = &s_stOPCCfg.stOPCVarInfo[uiWritAble];
    }
    else
    {
        /*log*/
    }

    return pstOPCVarInfo;
}
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    
    atomic32Set(&s_stLogicCfg.uiAvailable,uiAvailable);
    

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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
void SharedSwitchOPCVarInfo(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/

    uiAvailable = atomic32Get(&s_stOPCCfg.uiAvailable);
    
    if(INVALID_BUFF_ID == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
    }
    
    atomic32Set(&s_stOPCCfg.uiAvailable,uiAvailable);

    return;
}
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
     SharedSwitchLogicInfo();

     SharedSwitchIoModInfo();

     SharedSwitchCtrlStationInfo();

     SharedSwitchSOEConfigInfo ();

     SharedSwitchP2PConfigInfo();

     SharedSwitchModbusMasterInfo();

     SharedSwitchModbusSlaveInfo();

     SharedSwitchVOTEConfigInfo();

     SharedSwitchOPCVarInfo();

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
    if((DOWN_LOAD_COMPLETE == emDownloadFlg) || (NOT_DOWNLOAD == emDownloadFlg) ||\
            (INIT_DOWNLOAD == emDownloadFlg) || (ONLINE_DOWNLOAD == emDownloadFlg))
    {
        atomic32Set(&s_emDownloadFlg,emDownloadFlg);
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
    
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
    
    if((emTaskID < MAX_TASK_NUM) && (pstCtrlStationConfigInfo != NULL))
    {
        PrjShedTime = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiIECPeriod;
        
        if(0U == PrjShedTime)
        {
            //printf("  PrjShedTime is 0U \n");
            PrjShedTime = 2000U;
        }
    }
    else
    {
        //printf("  INIT PrjShedTime is 2000U \n");
        PrjShedTime = 2000U;
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
uint8_t SysGetIsConfigured(void)
{
    uint32_t ucIsConfigured = 0U;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    
    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
    
    if(pstCtrlStationConfigInfo != NULL)
    {
        ucIsConfigured = pstCtrlStationConfigInfo->ucIsConfigured;
    }
    
    return ucIsConfigured;
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
puint8_t SharedGetCodeArea(void)
{
    uint32_t uiAvailable = INVALID_BUFF_ID;        /*乒乓buffer中可读的配置信息片区ID*/
    puint8_t pucCodeArea = NULL;

    uiAvailable = atomic32Get(&s_stCodeArea.uiAvailable);
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
    
    if(FIRST_PIECE == uiWritAble || SECOND_PIECE == uiWritAble)
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
    
    if(FIRST_PIECE == uiAvailable)
    {
        uiAvailable =  SECOND_PIECE;
    }
    
    if(SECOND_PIECE == uiAvailable)
    {
        uiAvailable =  FIRST_PIECE;
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
        s_emLocalID = emLocalID;
    }
    else
    {
        s_emLocalID = INVALID_PM_ID;
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

    return s_emLocalID;
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
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
        
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
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
        
        if((pstCtrlStationConfigInfo != NULL) && (ACTIVE == pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag))
        {
            pucCodeAreaAddr = SharedGetCodeArea();
            if(pucCodeAreaAddr != NULL)
            {
                uiCodeEntry = pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].uiTaskCodeEntry;
                uiCodeEntry += (uint32_t)pucCodeAreaAddr;
            }
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
        pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
        
        if((pstCtrlStationConfigInfo != NULL) && (ACTIVE == pstCtrlStationConfigInfo->stTaskConfigInfo[emTaskID].emActiveFlag))
        {
            pucCodeAreaAddr = SharedGetCodeArea();
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
*                                局部函数实现
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
