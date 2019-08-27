/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : CM配置模块
*
*  文件名称    : Config.c
*
*  功能简介    : 完成CM相关配置信息的请求和解析
*
*  作者           : 刘阳
*
*  创建日期    : 2016-03-11
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"../inc/Config.h"
#include "Sys/inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t ucCMPrjInfoState;                        /*CM上配置信息完整性及版本检测的结果*/
static PMController_t emPMid;                           /*用于记录与CM本地版本不一致的PMid号，用于请求信息配置文件*/
static InterCMD_t emCMReqConfigFlag;                    /*CM请求配置信息的标志*/
static Service_Handle_Flag_t emCMConfigReqBusyFlg;
static uint8_t ucInterCMD[MAX_INTERCMD_DATA_LEN];
//static CMSlotIdTabl_t s_stCMSlotIdTablCfg;


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void CMConfigCheck (void);

static void CMConfigReqConfigInfo(void);

static int8_t ConfigContrlStationFileParse(uint8_t ucCtrlStationfileAddr[]);

static int8_t ConfigSubFileCRCSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigProjectInfoSectionParse(uint8_t ucSectionAddr[],uint8_t ucStringTableAddr[],\
                                            CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigTaskSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigTaskDataExSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigStationInfoSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigModbusMasterFileParse(uint8_t ucModbusMasterfileAddr[]);

static int8_t ConfigModbusMasterSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigModbusSlaveFileParse(uint8_t ucModbusSlavefileAddr[]);

static int8_t ConfigModbusSlaveSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigOPCFileParse(uint8_t ucOPCfileAddr[]);

static int8_t ConfigOPCInfoSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigCMConfigFileParse(uint8_t ucCMConfigfileAddr[]);

static int8_t ConfigCMConfigSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static uint32_t ConfigCheckCMSlotID(uint32_t uiCMSlotID,CMSlotIdTabl_t* pstCMSlotIdTabl);

static void ConfigInsertCMSlotID(uint32_t uiSlotID, CMSlotIdTabl_t* pstCMSlotIdTablOld, \
                                CMSlotIdTabl_t* pstCMSlotIdTablNew);

static void ConfigClearCfgInfo(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
********************************函数描述*******************************************************************************
* 函数名称: CMConfigInit
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
void CMConfigInit(void)
{
    emCMConfigReqBusyFlg = FREE;

    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CMConfigCycle
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
void CMConfigCycle(void)
{
    CMConfigCheck();
    CMConfigReqConfigInfo();

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/



/*
*********************************函数描述******************************************************************************
* 函数名称: CMConfigCheck
*
* 功能描述: 检测CM本地配置信息版本与PM是否一致
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static void CMConfigCheck (void)
{
    PMController_t emPMIndex = 0;
    ProjVerInfo_t stLocalPrjVerInfo;
    ProjVerInfo_t stPMPrjVerInfo;
    ProjVerInfo_t stDownLoadPrjVerInfo;
    
    ProjVerInfo_t stPMAPrjInfo;
    ProjVerInfo_t stPMBPrjInfo;
    ProjVerInfo_t stPMCPrjInfo;
    
    SysGetPMProjVersion(PMA,&stPMAPrjInfo);
    SysGetPMProjVersion(PMB,&stPMBPrjInfo);
    SysGetPMProjVersion(PMC,&stPMCPrjInfo);
    
    
    if((((0 == stPMAPrjInfo.uiProjID) && (0 == stPMAPrjInfo.uiMainVer) && (0 == stPMAPrjInfo.uiMinorVer) && (0 == stPMAPrjInfo.uiProjCRC)) || 
        (INTER_COMM_NORMAL != SysGetLocalInterCommState(PMA))) && 
        
       (((0 == stPMBPrjInfo.uiProjID) && (0 == stPMBPrjInfo.uiMainVer) && (0 == stPMBPrjInfo.uiMinorVer) && (0 == stPMBPrjInfo.uiProjCRC)) || 
       (INTER_COMM_NORMAL != SysGetLocalInterCommState(PMB))) && 
       
       (((0 == stPMCPrjInfo.uiProjID) && (0 == stPMCPrjInfo.uiMainVer) && (0 == stPMCPrjInfo.uiMinorVer) && (0 == stPMCPrjInfo.uiProjCRC)) || 
       (INTER_COMM_NORMAL != SysGetLocalInterCommState(PMC))))
    {/*三系PM均不在线或无工程，清除配置信息*/
        ConfigClearCfgInfo();
    }
    

    SysGetLocalProjVersion(&stLocalPrjVerInfo);
    ucCMPrjInfoState = 0;
    for(emPMIndex = PMA; emPMIndex < NUM_OF_PM; emPMIndex++)
    {
         SysGetPMProjVersion(emPMIndex,&stPMPrjVerInfo);

         SysGetDownloadProjVersion(emPMIndex,&stDownLoadPrjVerInfo);

         if(INTER_COMM_NORMAL == SysGetLocalInterCommState(emPMIndex))
         {
            if((stLocalPrjVerInfo.uiProjID == stPMPrjVerInfo.uiProjID)&&
                (stLocalPrjVerInfo.uiMainVer == stPMPrjVerInfo.uiMainVer)&&
                (stLocalPrjVerInfo.uiMinorVer == stPMPrjVerInfo.uiMinorVer)&&
                (stLocalPrjVerInfo.uiProjCRC == stPMPrjVerInfo.uiProjCRC))
            {/*本地版本与PM版本一致，表示CM与PM使用工程一致*/
                if((stLocalPrjVerInfo.uiProjID != stDownLoadPrjVerInfo.uiProjID)||
                    (stLocalPrjVerInfo.uiMainVer != stDownLoadPrjVerInfo.uiMainVer)||
                    (stLocalPrjVerInfo.uiMinorVer != stDownLoadPrjVerInfo.uiMinorVer)||
                    (stLocalPrjVerInfo.uiProjCRC != stDownLoadPrjVerInfo.uiProjCRC))
                {/*CM 与PM工程一致情况下，下装版本与CM本地版本不一致，说明有在线下装新工程存在*/
                    
                    if((0 == stDownLoadPrjVerInfo.uiProjID)&&
                        (0 == stDownLoadPrjVerInfo.uiMainVer)&&
                        (0 == stDownLoadPrjVerInfo.uiMinorVer)&&
                        (0 == stDownLoadPrjVerInfo.uiProjCRC))
                    {/*工程版本为0，无工程*/

                    }
                    else
                    {
                        ucCMPrjInfoState = CONFIG_DOWNLOAD_VERN_DIFFER;
                        emPMid = emPMIndex;
                        
                        DEBUG_CONFIG("CONFIG_DOWNLOAD_VERN_DIFFER  PM: %x \n",emPMIndex);
                        
                        break;
                    }
                }
                else
                {
//                    DEBUG_CONFIG("  stLocalPrjVerInfo.uiProjID is : %x \n",stLocalPrjVerInfo.uiProjID);
//                    DEBUG_CONFIG("  stLocalPrjVerInfo.uiProjCRC is : %x \n",stLocalPrjVerInfo.uiProjCRC);
//                    DEBUG_CONFIG("  stPMPrjVerInfo.uiProjID is : %x \n",stPMPrjVerInfo.uiProjID);
//                    DEBUG_CONFIG("  stPMPrjVerInfo.uiProjCRC is : %x \n",stPMPrjVerInfo.uiProjCRC);
//                    DEBUG_CONFIG("  stDownLoadPrjVerInfo.uiProjID is : %x \n",stDownLoadPrjVerInfo.uiProjID);
//                    DEBUG_CONFIG("  stDownLoadPrjVerInfo.uiProjCRC is : %x \n",stDownLoadPrjVerInfo.uiProjCRC);
                }
            }
            else
            {/*CM与PM使用工程版本不一致*/
                
                if((0 == stPMPrjVerInfo.uiProjID)&&
                    (0 == stPMPrjVerInfo.uiMainVer)&&
                    (0 == stPMPrjVerInfo.uiMinorVer)&&
                    (0 == stPMPrjVerInfo.uiProjCRC))
                {/*工程版本为0，无工程*/

                }
                else
                {
                    ucCMPrjInfoState = CONFIG_VERN_DIFFER;
                    emPMid = emPMIndex;
                    
                    DEBUG_CONFIG("CONFIG_VERN_DIFFER  PM : %x\n",emPMIndex);
                    DEBUG_CONFIG("%x %x\n",stLocalPrjVerInfo.uiProjID,stPMPrjVerInfo.uiProjID);
                    DEBUG_CONFIG("%x %x\n",stLocalPrjVerInfo.uiMainVer,stPMPrjVerInfo.uiMainVer);
                    DEBUG_CONFIG("%x %x\n",stLocalPrjVerInfo.uiMinorVer,stPMPrjVerInfo.uiMinorVer);
                    DEBUG_CONFIG("%x %x\n",stLocalPrjVerInfo.uiProjCRC,stPMPrjVerInfo.uiProjCRC);
                    
                    break;
                }
            }
         }
    }
    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: CMConfigReqConfigInfo
*
* 功能描述: 根据CM工程信息检测结果，（若不一致）向PM请求CM配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static void CMConfigReqConfigInfo(void)
{
    uint8_t ucRecvConfigInfoFlg = 0;
    puint8_t pucInterCMD = NULL;
    puint8_t pucFileAddr = NULL;
    int8_t cRet = 0;
    uint32_t uiCalcCRC = 0;
    uint16_t usInterCMDLen = 0;
    SysCfgFileType_t emCfgFileType = CFG_CM;        /*配置文件类型*/
    SysCfgFileState_t stCfgFileState ;              /*配置文件在内存上存储情况信息*/
    uint32_t uiFileConfigFlg = FILE_HAS_CONFIG;     /*文件是否被配置过的标志，放在文件开始地址*/
    puint8_t pucFileDataAddr = NULL;                /*存放文件内容的首地址*/
    uint16_t uiInterCommState = 0U;
    ProjVerInfo_t stLocalPrjVerInfo;
    InterCMDHead_t stInterCMDHead;
    SysFileProcessSeq_t emFileProcessSeq;
    static uint16_t s_usTimeout = 0U;


    SysGetLocalProjVersion(&stLocalPrjVerInfo);

    if(BUSY == emCMConfigReqBusyFlg)
    {
        /*检查PM上传配置信息是否完成*/
        ucRecvConfigInfoFlg = SysGetRecvConfigInfoFlg();
        if(RECV_CONFIGINFO_DONE == ucRecvConfigInfoFlg)
        {
            
            DEBUG_CONFIG("  RECV_CONFIGINFO_DONE !\n");
            
            for(emFileProcessSeq = PS_PRG; emFileProcessSeq < NUM_OF_PS_FILE; emFileProcessSeq++)
            {
                emCfgFileType = SysGetCfgFileType(emFileProcessSeq);
                
                SysGetCfgFileState(emCfgFileType, &stCfgFileState);
                if(CFG_FILE_RD_ENABLE == stCfgFileState.bReadable)
                {/*文件可读*/
                    /*获取文件内容首地址*/
                    pucFileDataAddr = SysGetCfgFileContentStartAddr(emCfgFileType);
                    switch(emCfgFileType)
                    {
                        case CFG_CM:
                        {
                            cRet = ConfigCMConfigFileParse(pucFileDataAddr);
                            if(RET_FAIL == cRet)
                            {
                                /*Record error log*/
                            }
                            
                        }
                        break;

                        case CFG_MODBUS_MASTER:
                        {
                            cRet = ConfigModbusMasterFileParse(pucFileDataAddr);
                            if(RET_FAIL == cRet)
                            {
                                /*Record error log*/
                            }
                        }
                        break;

                        case CFG_MODBUS_SLAVE:
                        {
                            cRet = ConfigModbusSlaveFileParse(pucFileDataAddr);
                            if(RET_FAIL == cRet)
                            {
                                /*Record error log*/
                            }
                        }
                        break;

                        case CFG_OPC:
                        {
                            cRet = ConfigOPCFileParse(pucFileDataAddr);
                            if(RET_FAIL == cRet)
                            {
                                /*Record error log*/
                            }
                        }
                        break;

                        case CFG_CTRL_STATION:
                        {
                            cRet = ConfigContrlStationFileParse(pucFileDataAddr);
                            if(RET_FAIL == cRet)
                            {
                                /*Record error log*/
                                
                            }
                        }
                        break;

                        default:
                        break;
                    }
                    SysDeleteCfgFile(emCfgFileType);
                }
                else
                {
                    DEBUG_CONFIG("CfgFileType : %d\n",emCfgFileType);
                    
                    SysDeleteCfgFile(emCfgFileType);
                }
            }
            
            emCMConfigReqBusyFlg = FREE;
            emCMReqConfigFlag = CMD_NONE;
            SysSetRecvConfigInfoFlg(SYS_CLR_RECV_CONFIGINFO_DONE);
        }
        
        s_usTimeout++;
        if( 750U == s_usTimeout)
        {
            emCMConfigReqBusyFlg = FREE;
            emCMReqConfigFlag = CMD_NONE;
            s_usTimeout = 0U;
        }
    }
    else
    {
        /*当配置模块空闲，根据配置信息版本检查结果，向PM请求配置信息*/
        if((CONFIG_VERN_DIFFER == ucCMPrjInfoState) || (CONFIG_DOWNLOAD_VERN_DIFFER == ucCMPrjInfoState))
        {
            stInterCMDHead.emCMSlotID = SysGetLocalCMSlot();
            /*在线下装时请求增量配置信息，其他情况请求全部配置信息*/
            if(CONFIG_DOWNLOAD_VERN_DIFFER == ucCMPrjInfoState)
            {
                stInterCMDHead.emInterCMD = CMD_REQ_INCRE_CMCONFIG;
                
                DEBUG_CONFIG(" @@@@ CMD_REQ_INCRE_CMCONFIG\n ");
                
            }
            else
            {
                stInterCMDHead.emInterCMD = CMD_REQ_ALL_CMCONFIG;
                
                DEBUG_CONFIG(" @@@@ CMD_REQ_ALL_CMCONFIG\n ");
                
            }
            emCMReqConfigFlag = stInterCMDHead.emInterCMD;
            /*判断与本地工程版本不一致的PM是否在线，若在线，向其发送请求配置文件的命令*/

            uiInterCommState = SysGetLocalInterCommState(emPMid);
            if(INTER_COMM_NORMAL == uiInterCommState)
            {
                stInterCMDHead.emPMID = emPMid;
                pucInterCMD = ucInterCMD;
                memcpy(pucInterCMD,&stInterCMDHead,sizeof(InterCMDHead_t));
                pucInterCMD += sizeof(InterCMDHead_t);

                uiCalcCRC = SysCrc32Cal( 0, ucInterCMD, sizeof(InterCMDHead_t));
                memcpy(pucInterCMD,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                pucInterCMD += sizeof(uiCalcCRC);

                usInterCMDLen = (uint16_t)(pucInterCMD - ucInterCMD);

                /*将内部命令数据交付内部通讯模块，发送给对应PM*/
                SysSetMsg(emPMid, INTER_CMD_REQ, ucInterCMD, usInterCMDLen);
                
                DEBUG_CONFIG(" @@@@ SEND INTER CMD  PMid: %x\n",emPMid);

                emCMConfigReqBusyFlg = BUSY;
            }
        }
    }
    return;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigContrlStationFileParse
*
* 功能描述: 完成对prg文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigContrlStationFileParse(uint8_t ucCtrlStationfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;         /*控制站配置信息可写片段指针*/
    
    
    DEBUG_CONFIG("ConfigCSFile begin !\n");
    
    if(NULL == ucCtrlStationfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,&ucCtrlStationfileAddr[uiBuffIndex],sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        /*计算文件总CRC*/
        uiCheckSumCal = SysCrc32Cal(0x00,(puint8_t)(&ucCtrlStationfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
        if(uiCheckSumCal != stFileHead.uiCheckSum)
        {
            /*log*/
//            DEBUG_CONFIG("  uiCheckSumCal != stFileHead.uiCheckSum \n");
//            DEBUG_CONFIG("  uiCheckSumCal is : %x \n",uiCheckSumCal);
//            DEBUG_CONFIG("  stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
        }
        else
        {/*计算整个文件的CRC正确，解析文件*/
            /*获取控制站配置信息可写片段指针*/
            pstCtrlStationInfo = CMCfgGetCtrlStationInfo();
            if(NULL == pstCtrlStationInfo )
            {
                /*log*/
            }
            else
            {
                /*初始化控制站配置信息可写片段*/
                memset((puint8_t)pstCtrlStationInfo, 0x00, sizeof(CtrlStationConfigInfo_t));
                
//                DEBUG_CONFIG("stFileHead.uiSectionNum is : %d \n",stFileHead.uiSectionNum);
                
                for(uiCount = 0; uiCount < stFileHead.uiSectionNum; uiCount++)
                {/*逐个段解析*/
                    
//                    DEBUG_CONFIG("SectionNum %d begin \n",uiCount);
//                    DEBUG_CONFIG("uiBuffIndex is : %d \n",uiBuffIndex);
                    
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable, &ucCtrlStationfileAddr[uiBuffIndex], sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    
//                    DEBUG_CONFIG("uiBuffIndex is : %d \n",uiBuffIndex);
                    
                    uiOffset = stSectionTable.uiOffset;
                    
//                    DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                    DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                    DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                    DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                    DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
//                    

                    /*按照段名解析文件内容*/
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_SUBFILE_CRC:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigSubFileCRCSectionParse(&ucCtrlStationfileAddr[uiOffset], \
                                        pstCtrlStationInfo))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ConfigSubFileCRC  HANDLE_FAIL !\n");
                                }
                            }
                        }
//                        DEBUG_CONFIG("PRG_SECTION_ID_SUBFILE_CRC is OVER \n");
                        break;

                        case PRG_SECTION_ID_PROJECT_INFO:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigProjectInfoSectionParse((&ucCtrlStationfileAddr[uiOffset]), \
                                        (&ucCtrlStationfileAddr[stFileHead.uiStringTableOffset]), pstCtrlStationInfo))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ConfigProjectInfo  HANDLE_FAIL !\n");
                                }
                            }
                        }
                        break;

                        case PRG_SECTION_ID_TASK:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigTaskSectionParse(&ucCtrlStationfileAddr[uiOffset], \
                                        pstCtrlStationInfo))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ConfigTask  HANDLE_FAIL !\n");
                                }
                            }
                        }
                        break;

                        case PRG_SECTION_ID_STATION_CONFIG:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigStationInfoSectionParse(&ucCtrlStationfileAddr[uiOffset], \
                                        pstCtrlStationInfo))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ConfigStationInfo  HANDLE_FAIL !\n");
                                }
                            }
                        }
                        break;

                        case PRG_SECTION_NAME_TASKDATAEXCHANGE:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigTaskDataExSectionParse(&ucCtrlStationfileAddr[uiOffset], \
                                        pstCtrlStationInfo))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ConfigTaskDataEx HANDLE_FAIL !\n");
                                }
                            }
                        }
                        break;

                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERROR %d !\n",stSectionTable.uiSectionId);
                        }
                        break;
                    }
                }
                if(HANDLE_OK == cSecHandleResult)
                {/*每一段的解析都正确，文件解析正确*/

                    /*记录文件CRC及版本*/
                    pstCtrlStationInfo->uiCRC = stFileHead.uiCheckSum;
                    pstCtrlStationInfo->uiFileVern = stFileHead.uiVersion;
                    pstCtrlStationInfo->ucIsConfigured = HAS_BE_CONFIGURED;

                        
                        DEBUG_CONFIG("PrjID %x\n",pstCtrlStationInfo->stPrjConfigInfo.uiProjectID);
//                        DEBUG_CONFIG(" MainVern is : %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion);
//                        DEBUG_CONFIG(" MinorVern is : %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion);
                        DEBUG_CONFIG("PrjCRC %x\n",pstCtrlStationInfo->uiCRC);
                        DEBUG_CONFIG("IsConfigured %x\n",pstCtrlStationInfo->ucIsConfigured);

                    
//                    DEBUG_CONFIG(" Parse HANDLE_OK !\n");
                    cRet = RET_OK;
                }
            }
        }
    }
    
//    DEBUG_CONFIG("  cRet is : %d \n",cRet);
//    DEBUG_CONFIG("ConfigContrlStationFileParse End .\n");
    
    return cRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigSubFileCRCSectionParse
*
* 功能描述: 完成对控制站配置文件SubFileCRC段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigSubFileCRCSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiFileID = 0U;
    SysCfgFileType_t emFileType = CFG_CM;
    uint32_t uiCount = 0U;
    uint32_t uiFileNum = 0U;

    if(NULL == ucSectionAddr)
    {
        /*log*/
        
        
        
    }
    else
    {
        memcpy(&uiFileNum, &ucSectionAddr[uiBuffIndex], sizeof(uiFileNum));
        uiBuffIndex += sizeof(uint32_t);
        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
        
        if(uiFileNum != NUM_OF_CFG_FILE_TYPE)
        {
            DEBUG_CONFIG("FileNum != NUM_OF_CFG_FILE_TYPE : %d \n",uiFileNum);
        }
        else
        {
            uiFileNum = uiFileNum -1;
            for(uiCount = 0; uiCount < uiFileNum; uiCount++)
            {/*逐个文件记录CRC*/
                memcpy((puint8_t)&uiFileID, &ucSectionAddr[uiBuffIndex], sizeof(uiFileID));
                uiBuffIndex += sizeof(uiFileID);
                if((0U == uiFileID) ||(uiFileID > NUM_OF_CFG_FILE_TYPE))
                {
                    DEBUG_CONFIG("FileID > NUM_OF_CFG_FILE_TYPE : %d \n",uiFileID);
                }
                else
                {
                    emFileType = uiFileID -1U;
                    memcpy((puint8_t)&pstCtrlStationInfo->stSubFileInfo.uiFileCRC[emFileType], &ucSectionAddr[uiBuffIndex],  \
                            sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stSubFileInfo.uiFileSerialNum[emFileType], &ucSectionAddr[uiBuffIndex],\
                            sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
//                    DEBUG_CONFIG("  ContrlStationFile: File %d  CRC is: %x ,  SerialNum is: %x \n",uiFileID, \
//                            pstCtrlStationInfo->stSubFileInfo.uiFileCRC[emFileType], \
//                            pstCtrlStationInfo->stSubFileInfo.uiFileSerialNum[emFileType]);
                }
            }
            cRet = RET_OK;
        }
    }
    
//    DEBUG_CONFIG("ConfigSubFileCRCSectionParse is end ;cRet is : %d \n",cRet);
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigProjectInfoSectionParse
*
* 功能描述: 完成对控制站配置文件ProjectInfo段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigProjectInfoSectionParse(uint8_t ucSectionAddr[],uint8_t ucStringTableAddr[],\
                                            CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    uint32_t uiBuffIndex = 0U;                              /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiPrjNameOffset = 0U;                          /*工程名存放偏移*/
    uint32_t uiDspOffset = 0U;                              /*工程描述存放偏移*/

    

    if((NULL == ucSectionAddr) || (NULL == ucStringTableAddr) || (NULL ==pstCtrlStationInfo))
    {
        /*log*/
    }
    else
    {
        /*记录工程的工程ID、主版本号、副版本号、最后保存时间*/
        memcpy((puint8_t)&pstCtrlStationInfo->stPrjConfigInfo.uiProjectID, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((puint8_t)&pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((puint8_t)&pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        uiBuffIndex += sizeof(uint32_t);/*跳过保留*/
        memcpy((puint8_t)&pstCtrlStationInfo->stPrjConfigInfo.uiLastSaveDate, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);

        /*记录工程名及描述*/
        memcpy((puint8_t)&uiPrjNameOffset, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((puint8_t)&uiDspOffset, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        /*从字符串表中摘取工程名*/
        strcpy((pchar_t)&pstCtrlStationInfo->stPrjConfigInfo.cProjectName[0], \
                (pchar_t)(&ucStringTableAddr[uiPrjNameOffset]));
        /*从字符串表中摘取工程描述*/
        strcpy((pchar_t)&pstCtrlStationInfo->stPrjConfigInfo.cDesctipiton[0], \
                (pchar_t)(&ucStringTableAddr[uiDspOffset]));
        
        
//        DEBUG_CONFIG("  ContrlStationFile:  uiProjectID is: %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiProjectID );
//        DEBUG_CONFIG("  ContrlStationFile:  uiMajorVersion is: %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion );
//        DEBUG_CONFIG("  ContrlStationFile:  uiMinorVersion is: %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion );
//        DEBUG_CONFIG("  ContrlStationFile:  uiLastSaveDate is: %x \n",pstCtrlStationInfo->stPrjConfigInfo.uiLastSaveDate );
        
//        DEBUG_CONFIG("  ContrlStationFile:  ProjectName is: ");
//        for(uint8_t x = 0U; x < MAX_NAME_LEN; x++)
//        {
//            printf("%s",pstCtrlStationInfo->stPrjConfigInfo.cProjectName[x]);
//        }
//        printf("\n");
//        
//        DEBUG_CONFIG("  ContrlStationFile:  Desctipiton is: ");
//        for(uint8_t y = 0U; y < MAX_NAME_LEN; y++)
//        {
//            printf("%s",pstCtrlStationInfo->stPrjConfigInfo.cDesctipiton[y]);
//        }
//        printf("\n");

        cRet = RET_OK;
    }
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigTaskSectionParse
*
* 功能描述: 完成对控制站配置文件TaskInfo段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigTaskSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiTaskNum = 0U;
    uint32_t uiTaskCount = 0U;
    Task_ID_t emTaskID = TASK_UP1;

    if((NULL == ucSectionAddr) || (NULL == pstCtrlStationInfo))
    {
        /*log*/
    }
    else
    {
        uiBuffIndex += sizeof(uint32_t);/*跳过段名*/
        memcpy((puint8_t)&uiTaskNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        
        DEBUG_CONFIG("TaskNum  is: %d \n",uiTaskNum );
        
        if(uiTaskNum > MAX_TASK_NUM)
        {
            
        }
        else
        {
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
            uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
            for(uiTaskCount = 0; uiTaskCount < uiTaskNum; uiTaskCount++)
            {
                memcpy((puint8_t)&emTaskID, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                
                if(emTaskID < MAX_TASK_NUM)
                {
                    pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTaskID = emTaskID;
                    
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiControlFlag, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiPriority, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIECPeriod, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
        /*去掉任务级的容忍*/
        /*
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTolerate, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        */
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
                    uiBuffIndex += sizeof(uint32_t);/*跳过函数入口个数*/
        
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTaskCodeEntry, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGlobalInitEntry, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiChangeGlobalInitEntry, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    
                    uiBuffIndex += sizeof(uint32_t);/*跳过数据区个数*/
                    
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGOffset, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGUsedSize, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIOffset, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIUsedSize, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiOOffset, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiOUsedSize, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);

                    
                    pstCtrlStationInfo->stTaskConfigInfo[emTaskID].emActiveFlag = ACTIVE;
                    
                    
//                    if((INIT_DOWNLOAD != SharedGetDownloadFlg()) && 
//                       (ONLINE_DOWNLOAD != SharedGetDownloadFlg()))
//                    {
//                        SysSetTaskStatus(emLocalPMId, emTaskID, TS_STOP);
//                    }
                    
                    

        
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
                    
//                    printf("TASK ID: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTaskID );
//                    printf("ControlFlag: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiControlFlag );
//                    printf("Priority: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiPriority );
//                    printf("IECPeriod: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIECPeriod );
//                    printf("TaskCodeEntry: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTaskCodeEntry );
//                    printf("GlobalInitEntry: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGlobalInitEntry );
//                    printf("GOffset: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGOffset );
//                    printf("GUsedSize: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGUsedSize );
//                    printf("IOffset: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIOffset );
//                    printf("IUsedSize: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiIUsedSize );
//                    printf("OOffset: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiOOffset );
//                    printf("OUsedSize: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiOUsedSize );
//                    printf("ChangeInitEntry: %x\n",pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiChangeGlobalInitEntry );
                }
            }
            cRet = RET_OK;
        }
    }
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigTaskDataExSectionParse
*
* 功能描述: 完成对控制站配置文件TaskDataEx段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigTaskDataExSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    uint32_t uiBuffIndex = 0U;
    int8_t cRet = RET_FAIL;
    uint32_t uiDataExVarNum = 0U;
    uint32_t uiUP1ToUP2VarNum = 0U;
    uint32_t uiUP2ToUP1VarNum = 0U;
    uint32_t uiVarCount = 0U;
    TaskDataExVar_t* pstTaskDataExVar = NULL;

    if((NULL == ucSectionAddr) || (NULL == pstCtrlStationInfo))
    {
        /*log*/
    }
    else
    {
        memcpy((puint8_t)&uiDataExVarNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        
        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
        
        //DEBUG_CONFIG("CS: uiDataExVarNum is: %x \n",uiDataExVarNum);
        if((uiDataExVarNum) > MAX_TASK_EXVAR_NUM)
        {
            
        }
        else
        {
            for(uiVarCount = 0; uiVarCount < uiDataExVarNum; uiVarCount++)
            {/*逐个记录任务间交换变量的信息*/
                
                pstTaskDataExVar = (TaskDataExVar_t*)&ucSectionAddr[uiBuffIndex];
                
                if((TASK_UP1 == pstTaskDataExVar->ucSrcTaskID) && (TASK_UP2 == pstTaskDataExVar->ucDestTaskID))
                {
                    uiUP1ToUP2VarNum++;
                }
                else if((TASK_UP2 == pstTaskDataExVar->ucSrcTaskID) && (TASK_UP1 == pstTaskDataExVar->ucDestTaskID))
                {
                    uiUP2ToUP1VarNum++;
                }
                else
                {
                    break;
                }
                
                memcpy((puint8_t)&pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount], \
                        &ucSectionAddr[uiBuffIndex], sizeof(TaskDataExVar_t));
                uiBuffIndex += sizeof(TaskDataExVar_t);

//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d usSize is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].usSize );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucSrcTaskID is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucSrcTaskID );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucSrcRefID is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucSrcRefID );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d uiSrcOffset is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].uiSrcOffset );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucSrcBitPos is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucSrcBitPos );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucDestTaskID is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucDestTaskID );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucDestRefId is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucDestRefId );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d uiDestOffset is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].uiDestOffset );
//            DEBUG_CONFIG("  ContrlStationFile:  VAR %d ucDestBitPos is: %x \n", uiVarCount,
//                    pstCtrlStationInfo->stTaskDataExInfo.stTaskDataExVar[uiVarCount].ucDestRefId );
            }
            
            pstCtrlStationInfo->stTaskDataExInfo.uiUP1ToUP2VarNum = uiUP1ToUP2VarNum;

            pstCtrlStationInfo->stTaskDataExInfo.uiUP2ToUP1VarNum = uiUP2ToUP1VarNum;
    
            cRet = RET_OK;
        }
    }
    return cRet;

}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigStationInfoSectionParse
*
* 功能描述: 完成对控制站配置文件StationConfig段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigStationInfoSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    uint32_t uiBuffIndex = 0U;
    int8_t cRet = RET_FAIL;

    if((NULL == ucSectionAddr) || (NULL == pstCtrlStationInfo))
    {
        /*log*/
    }
    else
    {
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.uiStationID, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.uiObjPasswordLen, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((pchar_t)&pstCtrlStationInfo->stStationConfigInfo.ucObjPassword[0], &ucSectionAddr[uiBuffIndex], \
                MAX_PASSWOED_NUM);
        uiBuffIndex += MAX_PASSWOED_NUM;
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.uiTolerate, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        
        
//        DEBUG_CONFIG("  ContrlStationFile:  uiStationID is: %x \n",pstCtrlStationInfo->stStationConfigInfo.uiStationID);
//        DEBUG_CONFIG("  ContrlStationFile:  ucObjPasswordLen is: %x \n",pstCtrlStationInfo->stStationConfigInfo.ucObjPasswordLen);
//        DEBUG_CONFIG("  ContrlStationFile:  uiTolerate is: %x \n",pstCtrlStationInfo->stStationConfigInfo.uiTolerate);
//        DEBUG_CONFIG("  ContrlStationFile:  ucObjPassword is: ");
//        for(uint8_t x = 0U; x < MAX_PASSWOED_NUM; x++)
//        {
//           printf("%s",pstCtrlStationInfo->stStationConfigInfo.ucObjPassword[x]);
//        }
//        printf("\n");
        

        cRet = RET_OK;
    }
    return cRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigModbusMasterFileParse
*
* 功能描述: 完成对Modbus主站文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigModbusMasterFileParse(uint8_t ucModbusMasterfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    
    DEBUG_CONFIG("ConfigModbusMasterFile begin !\n");
    
    if(NULL == ucModbusMasterfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucModbusMasterfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        

        /*计算文件数据CRC*/
        uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucModbusMasterfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
        if(uiCheckSumCal != stFileHead.uiCheckSum)
        {
            /*log*/
            
            DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
            DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
            DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
            
        }
        else
        {/*计算整个文件的CRC正确，解析文件*/

            for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
            {/*逐个段解析*/
                /*拷贝解析段表*/
                memcpy(&stSectionTable,ucModbusMasterfileAddr + uiBuffIndex,sizeof(stSectionTable));
                uiBuffIndex += sizeof(stSectionTable);
                uiOffset = stSectionTable.uiOffset;
                
//                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
                
                switch(stSectionTable.uiSectionId)
                {
                    case PRG_SECTION_ID_MODBUSMASTER:
                    {
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            if(RET_OK != ConfigModbusMasterSectionParse(&ucModbusMasterfileAddr[uiOffset], &stFileHead))
                            {/*解析段内容错误，设置段处理失败*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("PRG_SECTION_ID_MODBUSMASTER  HANDLE_FAIL ！\n");
                                
                            }
                        }
                    }
                    break;

                    default :
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("SectionId is ERROR : %d !\n",stSectionTable.uiSectionId);
                    }
                    break;
                }
            }
            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/
                cRet = RET_OK;
            }
        }
    }
    
//    DEBUG_CONFIG("ConfigModbusMasterFileParse End !\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigModbusMasterSectionParse
*
* 功能描述: 完成对Modbus主站文配置信息段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigModbusMasterSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead)
{

    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;
    uint32_t uiCount = 0U;
    uint32_t uiCMCount = 0U;
    uint32_t uiCMNum = 0U;
    uint32_t uiPortNum = 0U;                                    /*被配置了Modbus主站功能的Port个数*/
    uint32_t uiCMID = 0U;
    uint32_t uiCMSlotID = 0U;
    uint16_t usPortID = 0U;
    uint16_t usPortUpdate = 0U;
    uint32_t uiMasterBlockNum = 0U;
    uint32_t uiMasterBlockCount = 0U;
    uint32_t uiModbusVarNum = 0U;
    uint32_t uiModbusVarCount = 0U;
    uint32_t uiAddTCPStationNum = 0U;
    uint32_t uiDelTCPStationNum = 0U;
    uint32_t uiTCPChangeCount = 0U;
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;             /*Modbus主配置信息结构指针*/
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;                     /*CM槽位表可写片区的指针，供解析文件将配置信息写入*/
    ModbusMasterFileBlockHead_t stModbusMasterBlockHead;
    uint8_t ucBlockID = 0U;

    if((NULL == ucSectionAddr) || (NULL == pstFileHead))
    {
        /*log*/
    }
    else
    {
        /*获取Modbus主配置信息可写片区结构指针*/
        pstModbusMasterInfo = CMCfgGetModbusMasterInfo();
        /*获取CM槽位表可写片区的指针*/
        pstCMSlotIdTabl = CMCfgGetCMSlotIdTabl();
        if((NULL == pstModbusMasterInfo) || (NULL == pstCMSlotIdTabl))
        {
            /*log*/
            
            DEBUG_CONFIG("(NULL == ModbusMasterInfo) || (NULL == CMSlotIdTabl) \n");
            
        }
        else
        {/*解析数据*/
            /*初始化Modbus主配置信息结构*/
            memset((puint8_t)pstModbusMasterInfo, 0x00, sizeof(ModbusMasterInfo_t));
            /*获取配置了Modbus主站功能的CM个数*/
            memcpy((puint8_t)&uiCMNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            
            DEBUG_CONFIG("CMNum is : %d\n",uiCMNum);
            
            if(uiCMNum > MAX_CM_NUM)
            {
                DEBUG_CONFIG("CMNum > MAX_CM_NUM \n");
            }
            else
            {
                for(uiCMCount = 0U; uiCMCount < uiCMNum; uiCMCount++)
                {
                    
                    if(HANDLE_FAIL == cSecHandleResult)
                    {
                        break;
                    }
                    
                    memcpy((puint8_t)&uiCMSlotID, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    
                    DEBUG_CONFIG("  uiCMSlotID is : %d\n",uiCMSlotID);
                    
                    uiCMID = ConfigCheckCMSlotID(uiCMSlotID,pstCMSlotIdTabl);/*根据槽位号查出对应内部编号*/
                    if(INVALID_CM_SLOT_NUM == uiCMID)
                    {/*查找CM槽位号失败，终止解析*/
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("INVALID_CM_SLOT_NUM == uiCMID \n");
                        
                        break;
                        /*log*/
                    }
                    
//                    DEBUG_CONFIG("  uiCMID is : %d\n",uiCMID);
                    
                    /*获取配置了Modbus主站功能的Port个数*/
                    memcpy((puint8_t)&uiPortNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
                    
                    DEBUG_CONFIG("PortNum is : %d\n",uiPortNum);
                    
                    if(uiPortNum > MAX_PORT_NUM)
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        
                        break;
                    }
                    else
                    {
                        for(uiCount = 0U; uiCount < uiPortNum; uiCount++)
                        {
                            
                            if(HANDLE_FAIL == cSecHandleResult)
                            {
                                break;
                            }
                            
                            memcpy((puint8_t)&usPortUpdate, &ucSectionAddr[uiBuffIndex], sizeof(uint16_t));
                            uiBuffIndex += sizeof(uint16_t);
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].usCMSlotID = (uint16_t)uiCMSlotID;
                            memcpy((puint8_t)&usPortID, &ucSectionAddr[uiBuffIndex], sizeof(uint16_t));
                            uiBuffIndex += sizeof(uint16_t);
                            memcpy((puint8_t)&uiMasterBlockNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                            uiBuffIndex += sizeof(uint32_t);
                            memcpy((puint8_t)&uiAddTCPStationNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                            uiBuffIndex += sizeof(uint32_t);
                            memcpy((puint8_t)&uiDelTCPStationNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                            uiBuffIndex += sizeof(uint32_t);
                            
                            
                            if(usPortID >= MAX_PORT_NUM)
                            {
                                cSecHandleResult = HANDLE_FAIL;
                                
                                break;
                            }
//                            DEBUG_CONFIG("  usPortUpdate is : %d\n",usPortUpdate);
//                            DEBUG_CONFIG("  usPortID is : %d\n",usPortID);
//                            DEBUG_CONFIG("  uiMasterBlockNum is : %d\n",uiMasterBlockNum);
//                            DEBUG_CONFIG("  uiAddTCPStationNum is : %d\n",uiAddTCPStationNum);
//                            DEBUG_CONFIG("  uiDelTCPStationNum is : %d\n",uiDelTCPStationNum);
                            
                            
                            if((uiAddTCPStationNum > MAX_MODBUS_SLAVE_NUM) || (uiDelTCPStationNum > MAX_MODBUS_SLAVE_NUM))
                            {/*变化从站个数超出性能允许范围*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("AddTCPStationNum || DelTCPStationNum > MAX_MODBUS_SLAVE_NUM\n");
                                
                                break;
                                /*log*/
                            }
            
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiAddTCPStationNum = uiAddTCPStationNum;
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiDelTCPStationNum = uiDelTCPStationNum;
            
                            /*记录端口下对应增加从站IP地址*/
                            for(uiTCPChangeCount = 0U; uiTCPChangeCount < uiAddTCPStationNum; uiTCPChangeCount++)
                            {
                                memcpy((puint8_t)&pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiAddStaionIP[uiTCPChangeCount], \
                                        &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                                uiBuffIndex += sizeof(uint32_t);
                                
//                                DEBUG_CONFIG("  AddTCPStation :%d IP is :%x \n",uiTCPChangeCount, \
//                                        pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiAddStaionIP[uiTCPChangeCount]);
                                
                            }
                            /*记录端口下对应减少从站IP地址*/
                            for(uiTCPChangeCount = 0U; uiTCPChangeCount < uiDelTCPStationNum; uiTCPChangeCount++)
                            {
                                memcpy((puint8_t)&pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiDelStaionIP[uiTCPChangeCount], \
                                        &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                                uiBuffIndex += sizeof(uint32_t);
                                
//                                DEBUG_CONFIG("  DelTCPStation :%d IP is :%x \n",uiTCPChangeCount, \
//                                        pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiDelStaionIP[uiTCPChangeCount]);
                                
                            }
            
                            for(uiMasterBlockCount = 0U; uiMasterBlockCount < uiMasterBlockNum; uiMasterBlockCount++)
                            {
            
                                memcpy((puint8_t)&stModbusMasterBlockHead, \
                                        &ucSectionAddr[uiBuffIndex], sizeof(ModbusMasterFileBlockHead_t));
                                uiBuffIndex += sizeof(ModbusMasterFileBlockHead_t);
                                
                                ucBlockID = (uint8_t)stModbusMasterBlockHead.uiBlockID;
                                if(ucBlockID > MAX_MODBUS_BLOCK_NUM)
                                {
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("BlockID > MAX_MODBUS_BLOCK_NUM\n");
                                    
                                    break;
                                }
                                pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].ucModbusMasterBlockID[uiMasterBlockCount] = ucBlockID;
                                
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.ucVisitMode = stModbusMasterBlockHead.ucVisitMode;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.ucModbusRgn = stModbusMasterBlockHead.ucModbusRgn;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.uiTargetStationID = stModbusMasterBlockHead.uiTargetStationID;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.uiTargetIP = stModbusMasterBlockHead.uiTargetIP;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.uiTargetBeginAddr = stModbusMasterBlockHead.uiTargetBeginAddr;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.uiTargetSize = stModbusMasterBlockHead.uiTargetSize;
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusMasterBlockHead.uiModbusVarNum = stModbusMasterBlockHead.uiModbusVarNum;
                                
                                
//                                DEBUG_CONFIG("  ucBlockID is : %d\n",ucBlockID);
//                                DEBUG_CONFIG("  ucVisitMode is : %d\n",stModbusMasterBlockHead.ucVisitMode);
//                                DEBUG_CONFIG("  ucModbusRgn is : %d\n",stModbusMasterBlockHead.ucModbusRgn);
//                                DEBUG_CONFIG("  uiTargetStationID is : %d\n",stModbusMasterBlockHead.uiTargetStationID);
//                                DEBUG_CONFIG("  uiTargetIP is : %d\n",stModbusMasterBlockHead.uiTargetIP);
//                                DEBUG_CONFIG("  uiTargetBeginAddr is : %d\n",stModbusMasterBlockHead.uiTargetBeginAddr);
//                                DEBUG_CONFIG("  uiTargetSize is : %d\n",stModbusMasterBlockHead.uiTargetSize);
                                
                                
                                
                                uiModbusVarNum = stModbusMasterBlockHead.uiModbusVarNum;
                                
//                                DEBUG_CONFIG("ModbusVarNum is : %d\n",uiModbusVarNum);
                                
                                
                                if(uiModbusVarNum > MAX_MODBUS_VAR_NUM)
                                {/*Modbus变量个数超限*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ModbusVarNum > MAX_MODBUS_VAR_NUM \n");
                                    
                                    break;
                                    /*log*/
                                }
                                for(uiModbusVarCount = 0U; uiModbusVarCount < uiModbusVarNum; uiModbusVarCount++)
                                {
                                    memcpy((puint8_t)&pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount], \
                                            &ucSectionAddr[uiBuffIndex], sizeof(ModbusVar_t));
                                    uiBuffIndex += sizeof(ModbusVar_t);
                                    
//                                    DEBUG_CONFIG("  VAR %d : ucTaskID is : %d\n",uiModbusVarCount,\
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].ucTaskID);
//                                    DEBUG_CONFIG("  VAR %d : ucRefID is : %d\n",uiModbusVarCount,\
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].ucRefID);
//                                    DEBUG_CONFIG("  VAR %d : usVarSize is : %d\n",uiModbusVarCount,\
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].usVarSize);
//                                    DEBUG_CONFIG("  VAR %d : uiOffset is : %d\n",uiModbusVarCount,\
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].uiOffset);
//                                    DEBUG_CONFIG("  VAR %d : uiModbusIndex is : %d\n",uiModbusVarCount,\
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].uiModbusIndex);
                                    
                                }
                                pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].emActive = ACTIVE;
                            }
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].uiBlockNum = uiMasterBlockNum;
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].emActive = ACTIVE;
                            pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].bUpdate = (bool_t)usPortUpdate;
           
                        }
                    }
                    
                    pstModbusMasterInfo->stModbusMasterConfig[uiCMID].emActive = ACTIVE;
                    
                }
            }

            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/
                cRet = RET_OK;
            }
            /*记录文件CRC及版本*/
            pstModbusMasterInfo->uiCRC = pstFileHead->uiCheckSum;
            pstModbusMasterInfo->uiFileVern = pstFileHead->uiVersion;
            
            
//            DEBUG_CONFIG("  pstModbusMasterInfo->uiCRC is : %x \n",pstModbusMasterInfo->uiCRC);
//            DEBUG_CONFIG("  pstModbusMasterInfo->uiFileVern is : %x \n",pstModbusMasterInfo->uiFileVern);
            
        }
    }
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigModbusSlaveFileParse
*
* 功能描述: 完成对Modbus从站文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigModbusSlaveFileParse(uint8_t ucModbusSlavefileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                   /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                         /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    
    DEBUG_CONFIG("ConfigModbusSlave begin !\n");
    
    if(NULL == ucModbusSlavefileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucModbusSlavefileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        

        /*计算文件总CRC*/
        uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucModbusSlavefileAddr[sizeof(stFileHead.uiCheckSum)]), \
                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
        if(uiCheckSumCal != stFileHead.uiCheckSum)
        {
            /*log*/
            
            DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
            DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
            DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
            
        }
        else
        {/*计算整个文件的CRC正确，解析文件*/

            for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
            {/*逐个段解析*/
                /*拷贝解析段表*/
                memcpy(&stSectionTable,ucModbusSlavefileAddr + uiBuffIndex,sizeof(stSectionTable));
                uiBuffIndex += sizeof(stSectionTable);
                uiOffset = stSectionTable.uiOffset;
                
//                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
                
                
                switch(stSectionTable.uiSectionId)
                {
                    case PRG_SECTION_ID_MODBUSSLAVE:
                    {
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            if(RET_OK != ConfigModbusSlaveSectionParse(&ucModbusSlavefileAddr[uiOffset], &stFileHead))
                            {/*解析段内容错误，设置段处理失败*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("PRG_SECTION_ID_MODBUSSLAVE  HANDLE_FAIL ！\n");
                                
                            }
                        }
                    }
                    break;

                    default :
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("SectionId is ERROR : %d !\n",stSectionTable.uiSectionId);
                    }
                    break;
                }
            }
            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/
                cRet = RET_OK;
            }
        }
    }
    
//    DEBUG_CONFIG("ConfigModbusSlaveFileParse End !\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigModbusSlaveSectionParse
*
* 功能描述: 完成对Modbus从站文配置信息段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigModbusSlaveSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead)
{

    uint32_t uiBuffIndex = 0U;                                   /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiSlaveBlockNum = 0U;
    uint32_t uiSlaveBlockCount = 0U;
    uint32_t uiModbusVarCount = 0U;
    uint32_t uiModbusVarNum = 0U;                               /*每个块内变量的个数*/
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;               /*Modbus从配置信息结构指针*/
    ModbusVar_t stModbusVar;
    uint32_t uiBlockID = 0U;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/

    if((NULL == ucSectionAddr) || (NULL == pstFileHead))
    {
        /*log*/
    }
    else
    {
        /*获取Modbus从配置信息可写片区结构指针*/
        pstModbusSlaveInfo = CMCfgGetModbusSlaveInfo();

        if((NULL == pstModbusSlaveInfo))
        {
            /*log*/
            
            DEBUG_CONFIG("NULL == pstModbusSlaveInfo \n");
            
        }
        else
        {/*解析数据*/
            /*初始化配置结构*/
            memset((puint8_t)pstModbusSlaveInfo, 0x00, sizeof(ModbusSlaveInfo_t));
            /*获取配置的Modbus从站个数*/
            memcpy((puint8_t)&uiSlaveBlockNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            pstModbusSlaveInfo->uiBlockNum = uiSlaveBlockNum;
            
            DEBUG_CONFIG("SlaveBlockNum :%d \n",pstModbusSlaveInfo->uiBlockNum);
            
            if(uiSlaveBlockNum > MAX_MODBUS_BLOCK_NUM)
            {
                cSecHandleResult = HANDLE_FAIL;
            }
            else
            {
                for(uiSlaveBlockCount = 0U; uiSlaveBlockCount < uiSlaveBlockNum; uiSlaveBlockCount++)
                {/*逐个从站块记录配置信息*/
                    /*拷贝块头部信息*/
                    memcpy((puint8_t)&uiBlockID,&ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    
                    
//                    DEBUG_CONFIG("  BLOCK %d :uiBlockID is :%d \n",uiBlockID,\
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBlockID);
//                    DEBUG_CONFIG("  BLOCK %d :ucModbusRgn is :%d \n",uiBlockID,\
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.ucModbusRgn);
//                    DEBUG_CONFIG("  BLOCK %d :ucPermission is :%d \n",uiBlockID,\
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.ucPermission);
//                    DEBUG_CONFIG("  BLOCK %d :uiBeginAddr is :%d \n",uiBlockID,\
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBeginAddr);
//                    DEBUG_CONFIG("  BLOCK %d :uiEndAddr is :%d \n",uiBlockID,\
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiEndAddr);
                    
//                    uiBlockID = pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBlockID;

                    
                    
//                    DEBUG_CONFIG("  BLOCK %d :uiModbusVarNum is :%d \n",uiBlockID, uiModbusVarNum);
                    if(uiBlockID >= MAX_MODBUS_AREA_NUM) 
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        break;
                    }
                    else
                    {
                        memcpy((puint8_t)&pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead,
                                &ucSectionAddr[uiBuffIndex], sizeof(ModbusSlaveBlockHead_t));
                        uiBuffIndex += sizeof(ModbusSlaveBlockHead_t);
                        
                        uiModbusVarNum = pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiModbusVarNum;
                        
                        if(uiModbusVarNum >= MAX_MODBUS_RGN_VAR_NUM)
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            break;
                        }
                        
                        for(uiModbusVarCount = 0U; uiModbusVarCount < uiModbusVarNum; uiModbusVarCount++)
                        {/*记录变量信息*/
                            memcpy((puint8_t)&stModbusVar,&ucSectionAddr[uiBuffIndex], sizeof(ModbusVar_t));
                            if(stModbusVar.uiModbusIndex >= MAX_MODBUS_RGN_VAR_NUM)
                            {
                                cSecHandleResult = HANDLE_FAIL;
                                break;
                            }
                            else
                            {
                                memcpy((puint8_t)&pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar,
                                        &ucSectionAddr[uiBuffIndex], sizeof(ModbusVar_t));
                                uiBuffIndex += sizeof(ModbusVar_t);
                                
                                pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].ucActive = MODBUS_VAR_ACTIVE;
                                
//                                DEBUG_CONFIG("  VAR %d :ucTaskID is :%d \n",stModbusVar.uiModbusIndex, \
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.ucTaskID);
//                                DEBUG_CONFIG("  VAR %d :ucRefID is :%d \n",stModbusVar.uiModbusIndex, \
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.ucRefID);
//                                DEBUG_CONFIG("  VAR %d :usVarSize is :%d \n",stModbusVar.uiModbusIndex, \
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.usVarSize);
//                                DEBUG_CONFIG("  VAR %d :uiOffset is :%d \n",stModbusVar.uiModbusIndex, \
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.uiOffset);
//                                DEBUG_CONFIG("  VAR %d :uiModbusIndex is :%d \n",stModbusVar.uiModbusIndex, \
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.uiModbusIndex);
                            }
                        }
                    }
                    pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].emActive = ACTIVE;
                }
            }

            /*记录文件CRC及版本*/
            pstModbusSlaveInfo->uiCRC = pstFileHead->uiCheckSum;
            pstModbusSlaveInfo->uiFileVern = pstFileHead->uiVersion;
            
//            DEBUG_CONFIG("  pstModbusSlaveInfo->uiCRC is : %x \n",pstModbusSlaveInfo->uiCRC);
//            DEBUG_CONFIG("  pstModbusSlaveInfo->uiFileVern is : %x \n",pstModbusSlaveInfo->uiFileVern);
            

            if(HANDLE_OK == cSecHandleResult)
            {
                cRet = RET_OK;
            }
        }
    }
    return cRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigOPCFileParse
*
* 功能描述: 完成对OPC文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigOPCFileParse(uint8_t ucOPCfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                   /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                         /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    
    DEBUG_CONFIG("ConfigOPCbegin !\n");
    
    if(NULL == ucOPCfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead, ucOPCfileAddr, sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);

        /*计算文件总CRC*/
        uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucOPCfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
        if(uiCheckSumCal != stFileHead.uiCheckSum)
        {
            /*log*/
            
            DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
            DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
            DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
        }
        else
        {/*计算整个文件的CRC正确，解析文件*/

            for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
            {/*逐个段解析*/
                /*拷贝解析段表*/
                memcpy(&stSectionTable, &ucOPCfileAddr[uiBuffIndex], sizeof(stSectionTable));
                uiBuffIndex += sizeof(stSectionTable);
                uiOffset = stSectionTable.uiOffset;
                
//                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
                
                switch(stSectionTable.uiSectionId)
                {
                    case PRG_SECTION_ID_OPC:
                    {
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            if(RET_OK != ConfigOPCInfoSectionParse(&ucOPCfileAddr[uiOffset], &stFileHead))
                            {/*解析段内容错误，设置段处理失败*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("PRG_SECTION_ID_OPC  HANDLE_FAIL ！\n");
                                
                            }
                        }
                    }
                    break;

                    default :
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("SectionId is ERROR : %d !\n",stSectionTable.uiSectionId);
                    }
                    break;
                }
            }
            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/
                cRet = RET_OK;
            }
        }
    }
    
//    DEBUG_CONFIG("ConfigOPCFileParse End !\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigOPCInfoSectionParse
*
* 功能描述: 完成对OPC文配置信息段的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigOPCInfoSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead)
{

    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t ucRet = RET_FAIL;
    uint32_t uiOPCVarNum = 0U;
    uint32_t uiOPCVarCount = 0U;
    OPCVarInfo_t* pstOPCVarInfo = NULL;                         /*OPC配置信息可写片段指针*/

    if((NULL == ucSectionAddr) || (NULL == pstFileHead))
    {
        /*log*/
    }
    else
    {
        /*获取OPC配置信息可写片段指针*/
        pstOPCVarInfo = CMCfgGetOPCVarInfo();
        if(NULL == pstOPCVarInfo)
        {
            /*log*/
        }
        else
        {
            /*初始化OPC配置信息可写部分*/
            memset((puint8_t)pstOPCVarInfo, 0x00, sizeof(OPCVarInfo_t));
            memcpy((puint8_t)&uiOPCVarNum, ucSectionAddr, sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            pstOPCVarInfo->uiVarNum = uiOPCVarNum;
            
            DEBUG_CONFIG("VarNum : %d \n",pstOPCVarInfo->uiVarNum);
            
            if(pstOPCVarInfo->uiVarNum > MAX_OPC_NUM)
            {
//                DEBUG_CONFIG("    pstOPCVarInfo->uiVarNum > MAX_OPC_NUM  : %d \n",pstOPCVarInfo->uiVarNum);
            }
            else
            {
                for(uiOPCVarCount = 0U; uiOPCVarCount < uiOPCVarNum; uiOPCVarCount++)
                {
                    memcpy((puint8_t)&pstOPCVarInfo->stOPCVar[uiOPCVarCount], &ucSectionAddr[uiBuffIndex], \
                            sizeof(OPCVar_t));
                    uiBuffIndex += sizeof(OPCVar_t);
//                    DEBUG_CONFIG("    stOPCVar[%d].ucTaskID is : %d\n",uiOPCVarCount, \
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].ucTaskID);
//                    DEBUG_CONFIG("    stOPCVar[%d].ucRefID is : %d\n",uiOPCVarCount, \
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].ucRefID);
//                    DEBUG_CONFIG("    stOPCVar[%d].usVarSize is : %d\n",uiOPCVarCount, \
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].usVarSize);
//                    DEBUG_CONFIG("    stOPCVar[%d].uiOffset is : %d\n",uiOPCVarCount, \
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].uiOffset);
                }
                
                pstOPCVarInfo->emActive = ACTIVE;

                /*记录文件CRC及版本*/
                pstOPCVarInfo->uiCRC = pstFileHead->uiCheckSum;
                pstOPCVarInfo->uiFileVern = pstFileHead->uiVersion;
                
//                DEBUG_CONFIG("    pstOPCVarInfo->uiCRC is : %x\n",pstOPCVarInfo->uiCRC);
//                DEBUG_CONFIG("    pstOPCVarInfo->uiFileVern is : %x\n",pstOPCVarInfo->uiFileVern);

                ucRet = RET_OK;
            }
        }
    }
    
    return ucRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigCMConfigFileParse
*
* 功能描述: 完成对CM Config文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigCMConfigFileParse(uint8_t ucCMConfigfileAddr[])
{
    uint32_t uiBuffIndex = 0;                                   /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;
    
    
    DEBUG_CONFIG("ConfigCM begin !\n");

    if(NULL == ucCMConfigfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead, ucCMConfigfileAddr, sizeof(stFileHead));
        /*计算文件总CRC*/
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucCMConfigfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
        uiBuffIndex += sizeof(stFileHead);

        if(uiCheckSumCal != stFileHead.uiCheckSum)
        {
            /*log*/
            DEBUG_CONFIG("CheckSumCal != CheckSum\n");
            DEBUG_CONFIG("CheckSumCal : %x  ",uiCheckSumCal);
            DEBUG_CONFIG("CheckSum : %x\n",stFileHead.uiCheckSum);
        }
        else
        {/*计算整个文件的CRC正确，解析文件*/

            for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
            {/*逐个段解析*/
                /*拷贝解析段表*/
                memcpy(&stSectionTable, &ucCMConfigfileAddr[uiBuffIndex], sizeof(stSectionTable));
                
//                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
                
                
                uiBuffIndex += sizeof(stSectionTable);
                uiOffset = stSectionTable.uiOffset;
                switch(stSectionTable.uiSectionId)
                {/*解析段内具体内容*/
                    case PRG_SECTION_ID_CM_CONFIG:
                    {
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            if(RET_OK != ConfigCMConfigSectionParse(&ucCMConfigfileAddr[uiOffset], \
                                    &stFileHead))
                            {/*解析段内容错误，设置段处理失败*/
                                cSecHandleResult = HANDLE_FAIL;
                                DEBUG_CONFIG("PRG_SECTION_ID_CM_CONFIG  HANDLE_FAIL ！\n");
                            }
                        }
                    }
                    break;

                    default :
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("SectionId is ERROR : %d !\n",stSectionTable.uiSectionId);
                    }
                    break;
                }
            }
            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/
                cRet = RET_OK;
                DEBUG_CONFIG("RET_OK ！\n");
            }
        }
    }
    
    
//    DEBUG_CONFIG("ConfigCMConfigFileParse End !\n");
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigCMConfigSectionParse
*
* 功能描述: 完成对CM Config文件的解析配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static int8_t ConfigCMConfigSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead)
{
    uint32_t uiBuffIndex = 0;                           /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiCMNum = 0U;                              /*CM个数*/
    uint32_t uiCount = 0U;
    uint32_t uiSlotID = 0U;                             /*CM槽位号*/
    uint32_t uiCMID = 0U;                               /*CM的ID*/
    uint32_t uiIndex = 0U;
    uint32_t uiCOMNum = 0U;                             /*串口个数*/
    uint8_t ucCOMID = 0U;                               /*串口ID*/
    uint32_t uiNetNum = 0U;                             /*网口个数*/
    uint8_t ucNetID = 0U;                               /*网口ID*/
    uint32_t uiModbusTCPNum = 0U;
    uint8_t ucModbusTCPID = 0U;                         /*net Id*/
    CMSlotIdTabl_t stCMSlotIdTabl;
    CMSlotIdTabl_t* pstCMSlotIdTablTmp = NULL;          /*CM槽位表可写片区的指针，供解析文件将配置信息写入*/
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;             /*CM槽位表可读片区的指针，供解析文件时进行比对*/
    CMConfigInfo_t* pstCMConfigInfo = NULL;             /*CM配置信息可写片区的指针，供解析文件将配置信息写入*/
    SysCfgFileTable_t* pstSysCfgFileTable = NULL;       /*上传CM的文件统计表*/
    

    if((NULL == ucSectionAddr) || (NULL == pstFileHead))
    {
        /*log*/
    }
    else
    {
        pstCMSlotIdTablTmp = &stCMSlotIdTabl;
        pstCMSlotIdTabl = CMCfgGetCMSlotIdTabl();
        pstCMConfigInfo = CMCfgGetCMConfigInfo();
        if((NULL == pstCMSlotIdTablTmp) || (NULL == pstCMSlotIdTabl) || (NULL == pstCMConfigInfo))
        {
            /*log*/
            
            
            DEBUG_CONFIG("NULL == pstCMSlotIdTablTmp || NULL == pstCMSlotIdTabl || NULL == pstCMConfigInfo  \n");
        }
        else
        {
            uiBuffIndex += 8*sizeof(char_t);/*跳过字符串*/

            memset((puint8_t)pstCMSlotIdTablTmp,0x00,sizeof(CMSlotIdTabl_t));
            
            /*记录当前配置的CM的槽位号，并按照从小到大将槽位号对应到CM ID*/
            memcpy((puint8_t)&uiCMNum, &ucSectionAddr[uiBuffIndex], sizeof(uiCMNum));
            if(uiCMNum <= MAX_CM_NUM)
            {
                uiBuffIndex += sizeof(uiCMNum);
                pstCMSlotIdTablTmp->uiCMNum = uiCMNum;
                
                DEBUG_CONFIG("pstCMSlotIdTablTmp->uiCMNum %d \n",pstCMSlotIdTablTmp->uiCMNum);
                

                /*初始化CM槽位表*/
                for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
                {
                    memcpy((puint8_t)&pstCMSlotIdTablTmp->uiCMSlotId[uiCount], &ucSectionAddr[uiBuffIndex], \
                            sizeof(uint32_t));
                    pstCMSlotIdTablTmp->uiEffectiveFlg[uiCount] = 1U;
                    uiBuffIndex += sizeof(uint32_t);
                    
                    DEBUG_CONFIG("pstCMSlotIdTablTmp->uiCMSlotId[%d] : %d \n",uiCount,pstCMSlotIdTablTmp->uiCMSlotId[uiCount]);
                }

                /*将本地临时记录CM槽位号逻辑表，拷贝入系统资源*/
                memcpy(pstCMSlotIdTabl, pstCMSlotIdTablTmp, sizeof(CMSlotIdTabl_t));

                /*初始化配置信息结构*/
                memset((puint8_t)pstCMConfigInfo,0x00,sizeof(CMConfigInfo_t));

                /*按照CM为单位解析CM的配置信息*/
                for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
                {
                    memcpy((puint8_t)&uiSlotID, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiCMID = ConfigCheckCMSlotID(uiSlotID,pstCMSlotIdTablTmp);
                    if(INVALID_CM_SLOT_NUM == uiCMID)
                    {
                        
                        DEBUG_CONFIG("INVALID_CM_SLOT_NUM ==CMID \n");
                        
                        break;
                    }
                    else if(uiCMID >= MAX_CM_NUM)
                    {
                        
                        break;
                    }
                    else
                    {
                        pstCMConfigInfo->stCMSingleInfo[uiCMID].usSlotID = (uint16_t)uiSlotID;
                        
    //                    DEBUG_CONFIG("  uiCMID is : %d \n",uiCMID);
    //                    DEBUG_CONFIG("  uiSlotID is : %d \n",uiSlotID);
                        
                        
                        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/

                        /*记录串口配置信息*/
                        memcpy((puint8_t)&uiCOMNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                        uiBuffIndex += sizeof(uint32_t);
                        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                        
    //                    DEBUG_CONFIG("  uiCOMNum is : %d \n",uiCOMNum);
                        
                        for(uiIndex = 0U; uiIndex < uiCOMNum; uiIndex++)
                        {/*逐个串口，按照串口号来记录串口信息*/
                            uiBuffIndex += sizeof(uint8_t);/*取到端口号的位置*/
                            memcpy((puint8_t)&ucCOMID,&ucSectionAddr[uiBuffIndex],sizeof(uint8_t));
                            uiBuffIndex = uiBuffIndex - sizeof(uint8_t);/*重新取到串口配置信息的开始*/
/*test*/
                            if(ucCOMID > COM4)
                            {
                                break;
                            }
                            
/*test*/
                            DEBUG_CONFIG("  ucCOMID is : %d \n",ucCOMID);
                            
                            pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].emActive = ACTIVE;
                            memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].bUpdate,
                                    &ucSectionAddr[uiBuffIndex], (sizeof(COMConfigInfo_t) - sizeof(ActiveFlag_t)));
                            uiBuffIndex += (sizeof(COMConfigInfo_t) - sizeof(ActiveFlag_t));
                            
    //                        DEBUG_CONFIG("  bUpdate is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].bUpdate);
    //                        DEBUG_CONFIG("  ucParity is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].ucParity);
    //                        DEBUG_CONFIG("  sModbusMax is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].sModbusMax);

                        }

                        /*记录网口配置信息*/
                        memcpy((puint8_t)&uiNetNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                        uiBuffIndex += sizeof(uint32_t);
                        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                        
    //                    DEBUG_CONFIG("  uiNetNum is : %d \n",uiNetNum);
                        

                        for(uiIndex = 0U; uiIndex < uiNetNum; uiIndex++)
                        {
                            uiBuffIndex += sizeof(uint8_t);/*取到网口号的位置*/
                            memcpy((puint8_t)&ucNetID, &ucSectionAddr[uiBuffIndex], sizeof(uint8_t));
                            uiBuffIndex = uiBuffIndex - sizeof(uint8_t);/*重新取到网口配置信息的开始*/
                            
                            
    /*test*/

                            if(ucNetID > NET2)
                            {
                                break;
                            }
    /*test*/
    //                        DEBUG_CONFIG("  ucNetID is : %d \n",ucNetID);
                            
                            pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].emActive = ACTIVE;
                            memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].bUpdate,
                                    &ucSectionAddr[uiBuffIndex], (sizeof(NetConfigInfo_t) - sizeof(ActiveFlag_t)));
                            uiBuffIndex += (sizeof(NetConfigInfo_t) - sizeof(ActiveFlag_t));
                            
    //                        DEBUG_CONFIG("  bUpdate is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].bUpdate);
    //                        DEBUG_CONFIG("  uiIPAddr is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].uiIPAddr);
    //                        DEBUG_CONFIG("  uiGateway is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].uiGateway);
                            
                        }

                        /*记录 Modbus TCP 配置信息*/
                        memcpy((puint8_t)&uiModbusTCPNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                        uiBuffIndex += sizeof(uint32_t);
                        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                        
    //                    DEBUG_CONFIG("  uiModbusTCPNum is : %d \n",uiModbusTCPNum);
                        
                        for(uiIndex = 0U; uiIndex < uiModbusTCPNum; uiIndex++)
                        {
                            uiBuffIndex += sizeof(uint8_t);/*取到端口号的位置*/
                            memcpy((puint8_t)&ucModbusTCPID,&ucSectionAddr[uiBuffIndex],sizeof(uint8_t));
                            uiBuffIndex = uiBuffIndex - sizeof(uint8_t);/*重新取到网口配置信息的开始*/
                            
    /*test*/
                            if(ucModbusTCPID > NET2)
                            {
                                break;
                            }
    /*test*/

    //                        DEBUG_CONFIG("  ucModbusTCPID is : %d \n",ucModbusTCPID);
                            
                            pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].emActive = ACTIVE;
                            memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].bUpdate,
                                    &ucSectionAddr[uiBuffIndex], (sizeof(ModbusTCPConfigInfo_t) - sizeof(ActiveFlag_t)));
                            uiBuffIndex += (sizeof(ModbusTCPConfigInfo_t) - sizeof(ActiveFlag_t));
                            
    //                        DEBUG_CONFIG("  bUpdate is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].bUpdate);
    //                        DEBUG_CONFIG("  usTCPPortNum is : %d \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].usTCPPortNum);
    //                        DEBUG_CONFIG("  uiIPAddr is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].uiIPAddr);
    //                        DEBUG_CONFIG("  sModbusMax is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].sModbusMax);
                            
                        }

                        /*记录 SNTP/NTP校时服务器IP地址配置信息*/
                        pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.emActive = ACTIVE;
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.bUpdate,
                                &ucSectionAddr[uiBuffIndex], (sizeof(NTPTCPConfigInfo_t) - sizeof(ActiveFlag_t)));
                        uiBuffIndex += (sizeof(NTPTCPConfigInfo_t) - sizeof(ActiveFlag_t));
                        
    //                    DEBUG_CONFIG("  bUpdate is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.bUpdate);
    //                    DEBUG_CONFIG("  uiMasterIPAddr is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.uiMasterIPAddr);

                        /*记录访问控制配置信息*/
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead, \
                                &ucSectionAddr[uiBuffIndex], sizeof(AccessCtrlHead_t));
                        uiBuffIndex += sizeof(AccessCtrlHead_t);
                        
    //                    DEBUG_CONFIG("  ucAccessCtrlNum is : %x \n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead.ucAccessCtrlNum);
                        
                        for(uiIndex = 0U; uiIndex < pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead.ucAccessCtrlNum; uiIndex++)
                        {
                            memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex], \
                                    &ucSectionAddr[uiBuffIndex], sizeof(AccessCtrlConfig_t));
                            uiBuffIndex += sizeof(AccessCtrlConfig_t);
                            
    //                        DEBUG_CONFIG("  AccessCtrl %d:ucNetwork is : %x \n",uiIndex,pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex].ucNetwork);
    //                        DEBUG_CONFIG("  AccessCtrl %d:ucSOE is : %x \n",uiIndex,pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex].ucSOE);
    //                        DEBUG_CONFIG("  AccessCtrl %d:uiClientIPAddr is : %x \n",uiIndex,pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex].uiClientIPAddr);
    //                        DEBUG_CONFIG("  AccessCtrl %d:uiSubnetMask is : %x \n",uiIndex,pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex].uiSubnetMask);
                            
                        }/*按照结构逐一拷贝访问控制配置信息*/
                    }
                    
                    /*在配置信息中记录文件版本和CRC*/
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].uiCRC = pstFileHead->uiCheckSum;
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].uiFileVern = pstFileHead->uiVersion;
                    
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].emActive = ACTIVE;
                }
                
                cRet = RET_OK;
            }

        }
    }
    return cRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigCheckCMSlotID
*
* 功能描述: 根据槽位号查找对应内部ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static uint32_t ConfigCheckCMSlotID(uint32_t uiCMSlotID,CMSlotIdTabl_t* pstCMSlotIdTabl)
{
    uint32_t uiCount = 0U;
    uint32_t uiRet = INVALID_CM_SLOT_NUM;

    if((pstCMSlotIdTabl != NULL) && (pstCMSlotIdTabl->uiCMNum <= MAX_CM_NUM))
    {
        for(uiCount = 0U; uiCount < pstCMSlotIdTabl->uiCMNum; uiCount++)
        {
            if(uiCMSlotID == pstCMSlotIdTabl->uiCMSlotId[uiCount])
            {
                uiRet = uiCount;
                break;
            }
        }
    }

    return uiRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigInsertCMSlotID
*
* 功能描述: 根据槽位号及上个有效下装的工程配置，将槽位号插入新的对照表。要求：原有槽位号与逻辑ID的对应关系不变。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static void ConfigInsertCMSlotID(uint32_t uiSlotID, CMSlotIdTabl_t* pstCMSlotIdTabl, CMSlotIdTabl_t* pstCMSlotIdTablTmp)
{
    uint32_t uiIndex = 0U;
    uint32_t uiFlg = 0U;            /*根据旧有的配置信息查找CM槽位号的标志，若查找到，置1，否则置0*/

    if((pstCMSlotIdTabl != NULL) && (pstCMSlotIdTablTmp != NULL))
    {
        /*查找是否是上次工程已经存在的CM槽位号，若是，保留其逻辑ID，并置标识*/
        for(uiIndex = 0; uiIndex < NUM_OF_CM; uiIndex++)
        {
            if((uiSlotID == pstCMSlotIdTabl->uiCMSlotId[uiIndex]) && \
                    (1U == pstCMSlotIdTabl->uiEffectiveFlg[uiIndex]))
            {
                pstCMSlotIdTablTmp->uiCMSlotId[uiIndex] = uiSlotID;
                pstCMSlotIdTablTmp->uiEffectiveFlg[uiIndex] = 1U;
                uiFlg = 1U;
                break;
            }
        }
        if(uiFlg != 1U)
        {/*没有从旧有配置信息中查找到*/
            /*若没有找到与此槽位号匹配的原有配置，挑一个无效的逻辑ID位置，将新的槽位号插入*/
            for(uiIndex = 0; uiIndex < NUM_OF_CM; uiIndex++)
            {
                if(0U == pstCMSlotIdTabl->uiEffectiveFlg[uiIndex])
                {
                    pstCMSlotIdTablTmp->uiCMSlotId[uiIndex] = uiSlotID;
                    pstCMSlotIdTablTmp->uiEffectiveFlg[uiIndex] = 1U;
                    break;
                }
            }
        }
    }

    return;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigClearCfgInfo
*
* 功能描述: 清除所有配置信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项:
***********************************************************************************************************************
*/
static void ConfigClearCfgInfo(void)
{
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;
    ModbusMasterInfo_t* pstModbusMasterInfo = NULL;
    ModbusSlaveInfo_t* pstModbusSlaveInfo = NULL;
    OPCVarInfo_t* pstOPCVarInfo = NULL;
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;
    

    pstCtrlStationConfigInfo = CMCfgGetCtrlStationInfo();
    pstModbusMasterInfo = CMCfgGetModbusMasterInfo();
    pstModbusSlaveInfo = CMCfgGetModbusSlaveInfo();
    pstOPCVarInfo = CMCfgGetOPCVarInfo();
    pstCMConfigInfo = CMCfgGetCMConfigInfo();
    pstCMSlotIdTabl = CMCfgGetCMSlotIdTabl();
    
    if((pstCtrlStationConfigInfo != NULL) && (pstModbusMasterInfo != NULL) && (pstModbusSlaveInfo != NULL) && 
       (pstOPCVarInfo != NULL) && (pstCMConfigInfo != NULL) && (pstCMSlotIdTabl != NULL))
    {
        if((pstCtrlStationConfigInfo->uiCRC != 0U) || (pstCtrlStationConfigInfo->uiFileVern != 0U))
        {
            pstCtrlStationConfigInfo->uiCRC = 0U;
            pstCtrlStationConfigInfo->uiFileVern = 0U;
            pstCtrlStationConfigInfo->ucIsConfigured = NOT_HAS_BE_CONFIRMED;
            
            DEBUG_CONFIG("Clear CS Config\n");
        }
        
        if((pstCtrlStationConfigInfo->stPrjConfigInfo.uiProjectID != 0U) || 
           (pstCtrlStationConfigInfo->stPrjConfigInfo.uiMajorVersion != 0U) ||
           (pstCtrlStationConfigInfo->stPrjConfigInfo.uiMinorVersion != 0U) ||
           (pstCtrlStationConfigInfo->stPrjConfigInfo.uiLastSaveDate != 0U))
        {
            pstCtrlStationConfigInfo->stPrjConfigInfo.uiProjectID = 0U;
            pstCtrlStationConfigInfo->stPrjConfigInfo.uiMajorVersion = 0U;
            pstCtrlStationConfigInfo->stPrjConfigInfo.uiMinorVersion = 0U;
            pstCtrlStationConfigInfo->stPrjConfigInfo.uiLastSaveDate = 0U;
            
            DEBUG_CONFIG("Clear Config Vern\n");
        }
        
        if((pstModbusMasterInfo->uiCRC != 0U) || (pstModbusMasterInfo->uiFileVern != 0U))
        {
            pstModbusMasterInfo->uiCRC = 0U;
            pstModbusMasterInfo->uiFileVern = 0U;
            
            DEBUG_CONFIG("Clear ModbusMaster Config\n");
        }
        
        if((pstModbusSlaveInfo->uiCRC != 0U) || (pstModbusSlaveInfo->uiFileVern != 0U))
        {
            pstModbusSlaveInfo->uiCRC = 0U;
            pstModbusSlaveInfo->uiFileVern = 0U;
            
            DEBUG_CONFIG("Clear ModbusSlave Config \n");
        }
        
        if((pstOPCVarInfo->uiCRC != 0U) || (pstOPCVarInfo->uiFileVern != 0U))
        {
            pstOPCVarInfo->uiCRC = 0U;
            pstOPCVarInfo->uiFileVern = 0U;
            pstOPCVarInfo->emActive = NOT_ACTIVE;
            
            DEBUG_CONFIG("Clear OPC Config \n");
        }
        
        if((pstCMConfigInfo->stCMSingleInfo[CM1].uiCRC != 0U) || (pstCMConfigInfo->stCMSingleInfo[CM1].uiFileVern != 0U) ||
           (pstCMConfigInfo->stCMSingleInfo[CM2].uiCRC != 0U) || (pstCMConfigInfo->stCMSingleInfo[CM2].uiFileVern != 0U) ||
           (pstCMConfigInfo->stCMSingleInfo[CM3].uiCRC != 0U) || (pstCMConfigInfo->stCMSingleInfo[CM3].uiFileVern != 0U) ||
           (pstCMConfigInfo->stCMSingleInfo[CM4].uiCRC != 0U) || (pstCMConfigInfo->stCMSingleInfo[CM4].uiFileVern != 0U))
        {
            pstCMConfigInfo->stCMSingleInfo[CM1].uiCRC = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM1].uiFileVern = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM1].emActive = NOT_ACTIVE;
            pstCMConfigInfo->stCMSingleInfo[CM2].uiCRC = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM2].uiFileVern = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM2].emActive = NOT_ACTIVE;
            pstCMConfigInfo->stCMSingleInfo[CM3].uiCRC = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM3].uiFileVern = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM3].emActive = NOT_ACTIVE;
            pstCMConfigInfo->stCMSingleInfo[CM4].uiCRC = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM4].uiFileVern = 0U;
            pstCMConfigInfo->stCMSingleInfo[CM4].emActive = NOT_ACTIVE;
            
            DEBUG_CONFIG("Clear CM Config \n");
        }

        memset(pstCMSlotIdTabl, 0x00, sizeof(CMSlotIdTabl_t));
        
        DEBUG_CONFIG("Clear CMSlotIdTabl \n");
        
    }
    
    SysRTDataClear();

    return;
}



/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
