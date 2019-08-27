/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 工程配置模块
*
*  文件名称    : Config.c
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

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include <Srv/Scheduler/include/string.h>
#include"../inc/Config.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SysCfgFileTable_t* s_pstSysCfgFileTable = NULL;
uint8_t ucTrace = 0U;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static int8_t ConfigPRGFileParse(uint8_t ucPRGfileAddr[]);

static int8_t ConfigSegmentUsedSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigProgramSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigExlibSectionParse(uint8_t ucSectionAddr[], uint8_t ucStringTableAddr[]);

static int8_t ConfigContrlStationFileParse(uint8_t ucCtrlStationfileAddr[]);

static int8_t ConfigSubFileCRCSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigProjectInfoSectionParse(uint8_t ucSectionAddr[],uint8_t ucStringTableAddr[],\
                                            CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigTaskSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigTaskDataExSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigStationInfoSectionParse(uint8_t ucSectionAddr[],CtrlStationConfigInfo_t* pstCtrlStationInfo);

static int8_t ConfigIOConfigFileParse(uint8_t ucIOConfigfileAddr[]);

static int8_t ConfigIOConfigSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigPMConfigSectionParse(uint8_t ucSectionAddr[]);

static uint32_t ConfigIOSlaveParse(uint8_t ucIOSlaveInfo[],uint8_t ucModType,SysIoModInfo_t* pstIoModInfo);

static int8_t ConfigSOEFileParse(uint8_t ucSOEfileAddr[]);

static int8_t ConfigSOEInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigP2PFileParse(uint8_t ucP2PfileAddr[]);

static int8_t ConfigP2PInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigModbusMasterFileParse(uint8_t ucModbusMasterfileAddr[]);

static int8_t ConfigModbusMasterSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigModbusSlaveFileParse(uint8_t ucModbusSlavefileAddr[]);

static int8_t ConfigModbusSlaveSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigVoteFileParse(uint8_t ucVotefileAddr[]);

static int8_t ConfigVoteInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

static int8_t ConfigRetainFileParse(uint8_t ucRetainfileAddr[]);

static int8_t ConfigRetainInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead);

//static int8_t ConfigOPCFileParse(uint8_t ucOPCfileAddr[]);

//static int8_t ConfigOPCInfoSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static int8_t ConfigCMConfigFileParse(uint8_t ucCMConfigfileAddr[]);

static int8_t ConfigCMConfigSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead);

static void ConfigInsertCMSlotID(uint32_t uiSlotID, CMSlotIdTabl_t* pstCMSlotIdTabl, CMSlotIdTabl_t* pstCMSlotIdTablTmp);

static void ConfigFillDualDataTable(void);

static void ConfigFillDualPrmTable(uint8_t ucBufID);

static void ConfigFillSlaveEnableTable(uint8_t ucBufID);

static void ConfigSetMasterPrm(void);

static void ConfigFillDualPrmArea(uint8_t ucBufID);

static void ConfigDownloadIOConfig(void);

static void ConfigOnlineDownloadIOConfig(void);

void ConfigFillSlaveChange();

static bool_t ConfigCheckFileCrc(CtrlStationConfigInfo_t* pstCtrlStationInfo);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
********************************函数描述*******************************************************************************
* 函数名称: ConfigInit
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
void ConfigInit(void)
{
#if 1
    SysCfgFileType_t emCfgFileType = CFG_CTRL_STATION;        /*配置文件类型*/
    int32_t iFd;
    FileInfo_t* pstFileInfo = NULL;
    uint8_t ucFileIntegrityCheck = 0U;
    SysCfgFileState_t stCfgFileState;                         /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;
    int32_t iFileDataLen = 0;
    uint8_t ucFileDataBuff[8*1024];
    uint32_t uiReadFileLen = 0U;
    int32_t iRet = FS_FAIL;
    bool_t bWriteFile = true;
    
    
    
    s_pstSysCfgFileTable = NULL;
    
    for(emCfgFileType = CFG_CTRL_STATION; emCfgFileType < NUM_OF_CFG_FILE_TYPE; emCfgFileType++)
    {
        /*根据文件类型，获取文件的控制结构*/
        pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
        if(pstFileInfo != NULL)
        {
            iFd = fs_open((const int8_t *)&pstFileInfo->ucFileName[0], FS_OPEN_NORMAL, FS_RDONLY);
            printf("file name %s\n", &pstFileInfo->ucFileName[0]);
            if(FS_FAIL == iFd)
            {/*打开文件失败*/
                printf("Open file fail  %d\n",emCfgFileType);
                break;
            }
            else
            {
                iRet = fs_close(iFd);
                if(FS_OK == iRet)
                {
                    if(CFG_CM == emCfgFileType)
                    {
                        printf("File Integrity Check OK \n");
                        ucFileIntegrityCheck = 1U;
                    }
                }
                else
                {/*关闭文件失败*/
                    printf("Close file fail : %d\n",emCfgFileType);
                    break;
                }
            }
        }
    }
    
    if(1U == ucFileIntegrityCheck)
    {
        for(emCfgFileType = CFG_CTRL_STATION; emCfgFileType < NUM_OF_CFG_FILE_TYPE; emCfgFileType++)
        {
            /*根据文件类型，获取文件的控制结构*/
            pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
            /*获取内存上文件信息*/
            SysGetCfgFileState(emCfgFileType, &stCfgFileState);

            if((NULL == pstFileInfo))
            {
                if(NULL == pstFileInfo)
                {
                    printf("Recv FileInfo fail : %d\n",emCfgFileType);
                }
            }
            else
            {
                
                /*将文件数据写入内存*/
                
                /*以读方式打开文件*/
                iFd = fs_open((const int8_t *)&pstFileInfo->ucFileName[0], FS_OPEN_NORMAL, FS_RDONLY);
                
                if(FS_FAIL == iFd)
                {/*打开文件失败*/
                    printf("Open file fail ! : %d\n",emCfgFileType);
                    SysCfgFileInit();
                    break;
                }
            
                iFileDataLen = fs_file_len((pint8_t)&pstFileInfo->ucFileName[0]);
                printf("File Len %d name %s\n",iFileDataLen, &pstFileInfo->ucFileName[0]);
                do
                {
                    uiReadFileLen = MIN(iFileDataLen,8*1024);
                    iRet = fs_read(iFd, &ucFileDataBuff[0], uiReadFileLen);
                    printf("fs_read iRet=%d\n", iRet);
                    if((iRet != -1) && (iRet <= uiReadFileLen))
                    {
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucFileDataBuff[0], uiReadFileLen);
                        
                        if(usWritDataLen != uiReadFileLen)
                        {/*写入内存失败*/
                            /*log*/
                            printf("WR fail : %d %d %d\n",emCfgFileType,usWritDataLen,uiReadFileLen);
                            SysCfgFileInit();
                            bWriteFile = false;
                            break;
                        }
                        else
                        {
                            iFileDataLen = iFileDataLen - uiReadFileLen;

                            /* debug: print file CRC */
                            if(iFileDataLen == 0)
                            {
                                printf("crc=0x%x\n", (uint32_t)(*(uint32_t*)&ucFileDataBuff[uiReadFileLen-4]));
                            }
                        }
                    }
                    else
                    {/*从flash读取文件内容失败*/
                        /*log*/
                        printf("RD fail : %d %d\n",emCfgFileType,iFd);
                        SysCfgFileInit();
                        bWriteFile = false;
                        break;
                    }

                    
                }while(iFileDataLen != 0);
                
                fs_close(iFd);
                if(false == bWriteFile)
                {
                    break;
                }
                else
                {
                    /*设置写文件结束*/
                    SysSetCfgFileFinished(emCfgFileType);
                    
                    if(CFG_CM == emCfgFileType)
                    {
                        printf("Read File OK \n");
                        SysSetAllCfgFileFinished();

                    }
                }
            }
        }
    }
    
    
    SysConfigModDownLoadVernInit();
#endif
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: ConfigCycle
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
void ConfigCycle(void)
{
    SysCfgFileType_t emCfgFileType = CFG_CM;        /*配置文件类型*/
    SysCfgFileState_t stCfgFileState;               /*配置文件在内存上存储情况信息*/
    puint8_t pucFileDataAddr = NULL;                /*存放文件内容的首地址*/
    CMController_t emCMID = INVALID_CM_ID;          /*CM的ID号*/
//    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;
    SysFileProcessSeq_t emFileProcessSeq = PS_PRG;
    
    
    
    if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
    {
        s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileIncreTable();
        
    }
    else
    {
        s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileAllTable();
    }

    for(emFileProcessSeq = PS_PRG; emFileProcessSeq < NUM_OF_PS_FILE; emFileProcessSeq++)
    {
        emCfgFileType= SysGetCfgFileType(emFileProcessSeq);
        SysGetCfgFileState(emCfgFileType, &stCfgFileState);

//        DEBUG_CONFIG("  emCfgFileType  is : %d \n",emCfgFileType);
        
        if((CFG_FILE_RD_ENABLE == stCfgFileState.bReadable) && (CFG_FILE_WR_FINISH == stCfgFileState.bFileStatus))
        {/*文件可读*/
            
            if(FILE_NOT_CONFIG == stCfgFileState.uiFileConfigFlg)
            {/*文件未被配置过*/
                /*获取文件内容首地址*/
                pucFileDataAddr = SysGetCfgFileContentStartAddr(emCfgFileType);
                
                if(NULL == pucFileDataAddr)
                {
//                    DEBUG_CONFIG("  NULL == pucFileDataAddr ! \n");
                }
                else
                {
//                    DEBUG_CONFIG("  pucFileDataAddr is : %x  \n",pucFileDataAddr);

                    switch(emCfgFileType)
                    {
                        case CFG_CM:
                        {
                            /*初始化上传CM文件表中CM配置文件相关信息*/
                            for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
                            {
                                s_pstSysCfgFileTable->bCfgFileCM[emCMID] = CFG_FILE_NOT_UPDATED;
                            }
    
                            ConfigCMConfigFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_MODBUS_MASTER:
                        {
                            /*初始化上传CM文件表中Modbus主相关信息*/
                            for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
                            {
                                s_pstSysCfgFileTable->bCfgFileModbusMaster[emCMID] = CFG_FILE_NOT_UPDATED;
                            }
    
                             ConfigModbusMasterFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_MODBUS_SLAVE:
                        {
                            /*初始化上传CM文件表中Modbus从相关信息，从站功能每个CM都具备，因此将其初始化为都需要上传*/
                            for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
                            {
                                s_pstSysCfgFileTable->bCfgFileModbusSlave[emCMID] = CFG_FILE_UPDATED;
                            }
    
                             ConfigModbusSlaveFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_OPC:
                        {
                            /*初始化上传CM文件表中OPC配置相关信息，OPC功能每个CM都具备，因此将其初始化为都需要上传*/
                            for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
                            {
                                s_pstSysCfgFileTable->bCfgFileOPC[emCMID] = CFG_FILE_UPDATED;
                            }
    
//                            ConfigOPCFileParse(pucFileDataAddr);
    
    
                        }
                        break;
    
                        case CFG_CTRL_STATION:
                        {
                            ConfigContrlStationFileParse(pucFileDataAddr);
                            
                            /*初始化上传CM文件表中控制站配置相关信息，控制站信息每个CM都需要，因此将其初始化为都需要上传*/
                            for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
                            {
                                s_pstSysCfgFileTable->bCfgFileCtrlStation[emCMID] = CFG_FILE_UPDATED;
                                s_pstSysCfgFileTable->bReady[emCMID] = true;
                            }
                            s_pstSysCfgFileTable->bFlag = true;

//                            pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                            
//                            printf("pstCMSlotIdTabl->uiCMNum is %d \n",pstCMSlotIdTabl->uiCMNum);
//                            printf("pstCMSlotIdTabl->uiCMSlotId[0] is %d \n",pstCMSlotIdTabl->uiCMSlotId[0]);
//                            printf("pstCMSlotIdTabl->uiCMSlotId[1] is %d \n",pstCMSlotIdTabl->uiCMSlotId[1]);
                            
//                            pstCMSlotIdTabl = SharedGetCMSlotIdTablWrit();
                            
                            //DEBUG_CONFIG("pstCMSlotIdTablWrit->uiCMNum is %d \n",pstCMSlotIdTabl->uiCMNum);
                            //DEBUG_CONFIG("pstCMSlotIdTablWrit->uiCMSlotId[0] is %d \n",pstCMSlotIdTabl->uiCMSlotId[0]);
                            
                            if(ONLINE_DOWNLOAD != SharedGetDownloadFlg())
                            {
                                /*全下装硬件配置*/
                                ConfigDownloadIOConfig();
                            }
                            else
                            {
                                ConfigOnlineDownloadIOConfig();
                            }

//                            SharedSetDownloadFlg(DOWN_LOAD_COMPLETE);
                        }
                        break;
    
                        case CFG_SOE:
                        {
                            ConfigSOEFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_P2P:
                        {
                            ConfigP2PFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_VOTE:
                        {
                            ConfigVoteFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_IO_MODULAR:
                        {
                            ConfigIOConfigFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_RETAIN:
                        {
                            ConfigRetainFileParse(pucFileDataAddr);
                        }
                        break;
    
                        case CFG_PRG:
                        {
                            ConfigPRGFileParse(pucFileDataAddr);
                        }
                        break;
    
                        default:
                        break;
                    }
                    
                    /*配置过后将配置表值置为已经配置*/
                    SysSetCfgFileConfigFlg(emCfgFileType, FILE_HAS_CONFIG);
                    
                }
            }
        }
    }

//    //DEBUG_CONFIG("Config Cycle End !!! \n");

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/



/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigPrjFileParse
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
static int8_t ConfigPRGFileParse(uint8_t ucPRGfileAddr[])
{
    int8_t cRet = RET_FAIL;
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    DEBUG_CONFIG("PRG\n");
    if(NULL == ucPRGfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucPRGfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucPRGfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                DEBUG_CONFIG("Calcrc %x CheckSum  %x\n",uiCheckSumCal, stFileHead.uiCheckSum);
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                /*按照段名解析文件内容*/
                for(uiCount = 0; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable,&ucPRGfileAddr[uiBuffIndex],sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    uiOffset = stSectionTable.uiOffset;
                    
    //                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
    //                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
    //                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
    //                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
    //                DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
                    
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_SEGMENT_USED:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigSegmentUsedSectionParse(&ucPRGfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    /*log*/
                                    
                                    DEBUG_CONFIG("SEGMENT_USED FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
                        case PRG_SECTION_ID_PROGRAM:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigProgramSectionParse(&ucPRGfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    /*log*/
                                    
                                    DEBUG_CONFIG("PROGRAM FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
                        
                        case PRG_SECTION_ID_VAR_LOAD_RELOC:
                            
                        break;
                        
                        case PRG_SECTION_ID_FUNC_LOAD_RELOC:
                            
                        break;
                        
                        case PRG_SECTION_ID_IMPORT_LOAD_RELOC:
                            
                        break;
                        
                        case PRG_SECTION_ID_IMP_FUNC_PTR_LOAD_RELOC:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigExlibSectionParse(&ucPRGfileAddr[uiOffset], \
                                        &ucPRGfileAddr[stFileHead.uiStringTableOffset]))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    /*log*/
                                    
                                    DEBUG_CONFIG("IMP_FUNC_PTR_LOAD_RELOC FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR  %d !\n",stSectionTable.uiSectionId);
                        }
                        break;
                    }
                }
                if(HANDLE_OK == cSecHandleResult)
                {
                    /*段内容解析全部无错，返回OK*/
                    
                    DEBUG_CONFIG("Parse OK\n");
                    
                    cRet = RET_OK;
                }
            }
        }
    }
    
    DEBUG_CONFIG("PRG End\n");
    
    return cRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigSegmentUsedSectionParse
*
* 功能描述: 完成对prg文件中SegmentUsed段的解析配置
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

static int8_t ConfigSegmentUsedSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{
    int8_t ucRet = RET_FAIL;
//    puint8_t pucSectionData = pSectionAddr;
//    uint32_t uiCount = 0;
//
//    SegmentUsedHead_t stSegmentUsedHead;
//    SegmentUsed_t    stSegmentUsed;
//
//
//    if(pucSectionData == NULL)
//    {
//        return ucRet;
//    }
//    memcpy((puint8_t)&stSegmentUsedHead,pucSectionData,sizeof(stSegmentUsedHead));
//    pucSectionData += sizeof(stSegmentUsedHead);
//    for(uiCount = 0;uiCount < stSegmentUsedHead.uiSegmentUsedEntryNum;uiCount++)
//    {
//        memcpy((puint8_t)&stSegmentUsed,pucSectionData,sizeof(stSegmentUsed));
//        pucSectionData += sizeof(stSegmentUsed);
//        pstLogicConfigInfo->uiDataAreaUsedSize[stSegmentUsed.uiSegmentId] = stSegmentUsed.uiSizeUsed;
//    }
    ucRet = RET_OK;
    return ucRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigProgramSectionParse
*
* 功能描述: 完成对prg文件中Program段的解析配置
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
static int8_t ConfigProgramSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    ProgramHead_t stProgramHead;
    LogicConfigInfo_t* pstLogicConfigInfo = NULL;               /*逻辑配置信息结构可写片段指针*/
    puint8_t pucCodeAreaAddr = NULL;                            /*Code区可写片区地址*/

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取逻辑配置信息结构可写片段指针*/
        pstLogicConfigInfo = SharedGetLogicInfoWrit();
        /*获取Code区可写片区地址*/
        pucCodeAreaAddr = SharedGetCodeAreaWrit();
        if((NULL == pstLogicConfigInfo) || (NULL == pucCodeAreaAddr))
        {
            /*log*/
        }
        else
        {
            memcpy(&stProgramHead,ucSectionAddr,sizeof(stProgramHead));
            uiBuffIndex += sizeof(stProgramHead);

            pstLogicConfigInfo->uiMachineCodeLen = stProgramHead.uiMachineCodeLen;
            pstLogicConfigInfo->uiMaxPous = stProgramHead.uiMaxPous;
            pstLogicConfigInfo->uiLoadAddr = stProgramHead.uiLoadAddr;
            
//            DEBUG_CONFIG("MachineCodeLen: %x\n",pstLogicConfigInfo->uiMachineCodeLen);
//            DEBUG_CONFIG("MaxPous: %x\n",pstLogicConfigInfo->uiMaxPous);
//            DEBUG_CONFIG("LoadAddr: %x\n",pstLogicConfigInfo->uiLoadAddr);
            
            /*加载用户逻辑Code*/
            if(pstLogicConfigInfo->uiMachineCodeLen > MAX_CODE_AREA_LEN)
            {
                
            }
            else
            {
                memcpy((puint8_t)pucCodeAreaAddr, (puint8_t)&ucSectionAddr[uiBuffIndex], \
                        pstLogicConfigInfo->uiMachineCodeLen);

                /*记录文件CRC及版本*/
                pstLogicConfigInfo->uiCRC = pstFileHead->uiCheckSum;
                pstLogicConfigInfo->uiFileVern = pstFileHead->uiVersion;

                cRet = RET_OK;
            }
        }
    }
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigExlibSectionParse
*
* 功能描述: 完成对prg文件中外部库重定位段的解析配置
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
static int8_t ConfigExlibSectionParse(uint8_t ucSectionAddr[], uint8_t ucStringTableAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    uint32_t uiFuncIndex = 0U;
    uint32_t uiFuncNameOffset = 0U;
    int8_t cRet = RET_FAIL;
    ExLibConfigInfo_t* pstExLibConfigInfo = NULL;               /*外部库配置信息结构可写片段指针*/

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取外部库配置信息结构可写片段指针*/
        pstExLibConfigInfo = SharedGetExLibInfoWrit();

        if((NULL == pstExLibConfigInfo))
        {
            /*log*/
        }
        else
        {
            memcpy(&pstExLibConfigInfo->uiExLibFuncNum,&ucSectionAddr[uiBuffIndex],sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
            
            if(pstExLibConfigInfo->uiExLibFuncNum <= MAX_EXLIB_FUNC_NUM)
            {
                for(uiFuncIndex = 0U; uiFuncIndex < pstExLibConfigInfo->uiExLibFuncNum; uiFuncIndex++)
                {
                    memcpy(&pstExLibConfigInfo->stExLibFuncInfo[uiFuncIndex].uiRelocOffset, \
                            &ucSectionAddr[uiBuffIndex],sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    
                    memcpy(&uiFuncNameOffset, &ucSectionAddr[uiBuffIndex],sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    
                    /*从字符串表中摘取工程名*/
                    strcpy((pchar_t)&pstExLibConfigInfo->stExLibFuncInfo[uiFuncIndex].cFuncName[0], \
                            (pchar_t)(&ucStringTableAddr[uiFuncNameOffset]));
                }
                
                cRet = RET_OK;
            }
        }
    }
    return cRet;
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
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PMID号       */
    PrjInfo_t *pstDLPrjInfo = NULL;                             /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrjInfo = NULL;                               /* 本机工程信息数据指针     */


    
    DEBUG_CONFIG("CS\n");
    
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
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
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
                pstCtrlStationInfo = SharedGetCtrlStationInfoWrit();
                if(NULL == pstCtrlStationInfo )
                {
                    /*log*/
                }
                else
                {
                    /*初始化控制站配置信息可写片段*/
                    memset((puint8_t)pstCtrlStationInfo, 0x00, sizeof(CtrlStationConfigInfo_t));
    
                    //DEBUG_CONFIG("stFileHead.uiSectionNum is : %d \n",stFileHead.uiSectionNum);
    
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
    //                    DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
                        
    
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
                                        
                                        DEBUG_CONFIG("SUBFILE_CRC FAIL !\n");
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
                                        
                                        DEBUG_CONFIG("PROJECT_INFO FAIL ! \n");
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
                                        
                                        DEBUG_CONFIG("TASK FAIL !\n");
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
                                        
                                        DEBUG_CONFIG("STATION_CONFIG FAIL ! \n");
                                    }
                                }
                            }
                            break;
    
                            case PRG_SECTION_ID_TASKDATAEXCHANGE:
                            {
                                if(HANDLE_OK == cSecHandleResult)
                                {
                                    if(RET_OK != ConfigTaskDataExSectionParse(&ucCtrlStationfileAddr[uiOffset], \
                                            pstCtrlStationInfo))
                                    {/*解析段内容错误，设置段处理失败*/
                                        cSecHandleResult = HANDLE_FAIL;
                                        
                                        DEBUG_CONFIG("TASKDATAEXCHANGE FAIL !\n");
                                    }
                                }
                            }
                            break;
    
                            default :
                            {
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
                            }
                            break;
                        }
                    }
                    if(HANDLE_OK == cSecHandleResult)
                    {/*每一段的解析都正确，文件解析正确*/
    
                        /*记录文件CRC及版本*/
                        pstCtrlStationInfo->uiCRC = stFileHead.uiCheckSum;
                        pstCtrlStationInfo->uiFileVern = stFileHead.uiVersion;
                        
                        if(true == ConfigCheckFileCrc(pstCtrlStationInfo))
                        {
                            pstCtrlStationInfo->uiIsConfigured = HAS_BE_CONFIGURED;
                        
                            pstDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
                            pstPrjInfo = SysGetPrjInfo(emLocalPMId);
                            
        //                    DEBUG_CONFIG("  emLocalPMId is %d \n",emLocalPMId);
        //                    DEBUG_CONFIG("  pstDLPrjInfo is %x \n",pstDLPrjInfo);
        //                    DEBUG_CONFIG("  pstPrjInfo is %x \n",pstPrjInfo);
                            
                            printf("Flg:%d\n",SharedGetDownloadFlg());
                            
                            
                            if((pstDLPrjInfo != NULL) && (pstPrjInfo != NULL))
                            {
                                if((INIT_DOWNLOAD == SharedGetDownloadFlg()) || \
                                   (ONLINE_DOWNLOAD == SharedGetDownloadFlg()))
                                {
                                    pstDLPrjInfo->uiPrjID = pstCtrlStationInfo->stPrjConfigInfo.uiProjectID;
                                    pstDLPrjInfo->uiMainVern = pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion;
                                    pstDLPrjInfo->uiMinorVern = pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion;
                                    pstDLPrjInfo->uiPrjCRC = stFileHead.uiCheckSum;
                                    pstDLPrjInfo->ucIsConfigured = HAS_BE_CONFIGURED;
                                }
                                else
                                {
                                    pstPrjInfo->uiPrjID = pstCtrlStationInfo->stPrjConfigInfo.uiProjectID;
                                    pstPrjInfo->uiMainVern = pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion;
                                    pstPrjInfo->uiMinorVern = pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion;
                                    pstPrjInfo->uiPrjCRC = stFileHead.uiCheckSum;
                                    pstPrjInfo->ucIsConfigured = HAS_BE_CONFIGURED;
                                    
                                    SharedSetPowerOnConfigSwitchFlg(NEED_POWERON_CONIFG_SWITCH);
                                }

        //                        printf(" uiPrjID is : %x \n",pstPrjInfo->uiPrjID);
        //                        printf(" uiMainVern is : %x \n",pstPrjInfo->uiMainVern);
        //                        printf(" uiMinorVern is : %x \n",pstPrjInfo->uiMinorVern);
//                                printf("PrjCRC: %x \n",pstDLPrjInfo->uiPrjCRC);
                                printf("IsConfigured %x %x\n",pstDLPrjInfo->ucIsConfigured,pstPrjInfo->ucIsConfigured);
                                
                                
                            }
                            
                            DEBUG_CONFIG("HANDLE_OK !\n");
                            cRet = RET_OK;
                        }
                        else
                        {
                            printf("Check Crc not ok\n");
                        }

                    }
                }
            }
        }
    }
    
//    DEBUG_CONFIG("  cRet is : %d \n",cRet);
    DEBUG_CONFIG("CS End\n");
    
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
//            DEBUG_CONFIG("  ContrlStationFile: uiFileNum != NUM_OF_CFG_FILE_TYPE : %d \n",uiFileNum);
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
//                    DEBUG_CONFIG("  ContrlStationFile: uiFileID > NUM_OF_CFG_FILE_TYPE : %d \n",uiFileID);
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
                    
//                    DEBUG_CONFIG("File %d  CRC: %x , SerialNum: %x \n",uiFileID, 
//                            pstCtrlStationInfo->stSubFileInfo.uiFileCRC[emFileType], 
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

    

    if((NULL == ucSectionAddr) && (NULL == ucStringTableAddr))
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
        
        
//        printf("PrjID: %x\n",pstCtrlStationInfo->stPrjConfigInfo.uiProjectID );
//        printf("MajorVern: %x\n",pstCtrlStationInfo->stPrjConfigInfo.uiMajorVersion );
//        printf("MinorVern: %x\n",pstCtrlStationInfo->stPrjConfigInfo.uiMinorVersion );
//        printf("LastDate: %x\n",pstCtrlStationInfo->stPrjConfigInfo.uiLastSaveDate );
        
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

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        uiBuffIndex += sizeof(uint32_t);/*跳过段名*/
        memcpy((puint8_t)&uiTaskNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
        
//        DEBUG_CONFIG("  ContrlStationFile:  uiTaskNum  is: %d \n",uiTaskNum );
        
        uiBuffIndex += sizeof(uint32_t);
        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
        uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
        
        if(uiTaskNum <= MAX_TASK_NUM)
        {
            pstCtrlStationInfo->ucTaskNum = (uint8_t)uiTaskNum;
            
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
        
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiTaskCodeEntry, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiGlobalInitEntry, \
                            &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
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
                    memcpy((puint8_t)&pstCtrlStationInfo->stTaskConfigInfo[emTaskID].uiChangeGlobalInitEntry, \
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

    if(NULL == ucSectionAddr)
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

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.uiStationID, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.ucObjPasswordLen, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint8_t));
        uiBuffIndex += sizeof(uint8_t);
        memcpy((pchar_t)&pstCtrlStationInfo->stStationConfigInfo.ucObjPassword[0], &ucSectionAddr[uiBuffIndex], \
                MAX_PASSWOED_NUM);
        uiBuffIndex += MAX_PASSWOED_NUM;
        memcpy((puint8_t)&pstCtrlStationInfo->stStationConfigInfo.uiTolerate, &ucSectionAddr[uiBuffIndex], \
                sizeof(uint32_t));
        uiBuffIndex += sizeof(uint32_t);
        
        
//        printf("StationID: %x\n",pstCtrlStationInfo->stStationConfigInfo.uiStationID);
//        printf("ObjPasswordLen: %x\n",pstCtrlStationInfo->stStationConfigInfo.ucObjPasswordLen);
//        printf("Tolerate: %x\n",pstCtrlStationInfo->stStationConfigInfo.uiTolerate);
//        DEBUG_CONFIG("  ContrlStationFile:  ucObjPassword is: \n");
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
* 函数名称: ConfigIOConfigFileParse
*
* 功能描述: 完成对IO Config文件的解析配置
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
static int8_t ConfigIOConfigFileParse(uint8_t ucIOConfigfileAddr[])
{
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;



    DEBUG_CONFIG("IO\n");
    
    if(NULL == ucIOConfigfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucIOConfigfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
        //DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
        //DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
        //DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
        //DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
        //DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
        //DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
        //DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
        //DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucIOConfigfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                
                //DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
                //DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
                //DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
                
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable, &ucIOConfigfileAddr[uiBuffIndex], sizeof(stSectionTable));
                    
                    //DEBUG_CONFIG("SectionId %x\n",stSectionTable.uiSectionId);
                    //DEBUG_CONFIG("Offset %x\n",stSectionTable.uiOffset);
                    //DEBUG_CONFIG("Size %x\n",stSectionTable.uiSize);
                    //DEBUG_CONFIG("Msg %x\n",stSectionTable.uiMsg);
                    //DEBUG_CONFIG("SameWithLastDownload %x\n",stSectionTable.uiSameWithLastDownload);
                    
                    
                    uiBuffIndex += sizeof(stSectionTable);
                    uiOffset = stSectionTable.uiOffset;
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_IO_CONFIG:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigIOConfigSectionParse(&ucIOConfigfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("IO_CONFIG FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
                        
                        case PRG_SECTION_ID_PMCONFIG:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigPMConfigSectionParse(&ucIOConfigfileAddr[uiOffset]))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("PM_CONFIG FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
                        }
                        break;
                    }
                }
                if(HANDLE_OK == cSecHandleResult)
                {/*每一段的解析都正确，文件解析正确*/
                    
                    DEBUG_CONFIG("HANDLE_OK\n");
                    
                    cRet = RET_OK;
                }
            }
        }
    }
    
    DEBUG_CONFIG("IO End !\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigIOConfigSectionParse
*
* 功能描述: 完成对IO Config段的解析配置
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
static int8_t ConfigIOConfigSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;
    uint32_t uiModNum = 0U;
    uint32_t uiCount = 0U;
    uint8_t ucModType = 0U;
    uint32_t uiParseDataSize = 0U;
    IOModuleConfigHead_t stIOModuleConfigHead;
    SysIoModInfo_t* pstIoModInfo = NULL;                     /*IO模块配置信息结构可写片区指针*/

    if(NULL == ucSectionAddr)
    {
        /*log*/
        DEBUG_CONFIG("NULL SectionAddr\n");
    }
    else
    {
        /*获取IO模块配置信息结构可写片区指针*/
        pstIoModInfo = SharedGetIoModInfoWrit();
        
        if(NULL == pstIoModInfo)
        {
            /*log*/
            
            DEBUG_CONFIG("NULL IoModInfo\n");
            
        }
        else
        {
            memset(pstIoModInfo, 0x00, sizeof(SysIoModInfo_t));
            
            /*跳过头部描述字符串*/
            uiBuffIndex += 8*sizeof(char_t);
            memcpy(&uiModNum,&ucSectionAddr[uiBuffIndex],sizeof(uint32_t));
            uiBuffIndex += 2*sizeof(uint32_t);
            
//            DEBUG_CONFIG("ModNum is : %d \n",uiModNum);
            SharedModConfigChangeInit();

            for(uiCount = 0U; uiCount < uiModNum; uiCount++)
            {
                memcpy(&stIOModuleConfigHead, &ucSectionAddr[uiBuffIndex], sizeof(stIOModuleConfigHead));
                
//                DEBUG_CONFIG("  uiCount is : %d \n",uiCount);
//                DEBUG_CONFIG("  stIOModuleConfigHead.usModuleTag is : %x \n",stIOModuleConfigHead.usModuleTag);
                
                switch(stIOModuleConfigHead.usModuleTag)
                {
                    case MODTAG_AI:
                    case MODTAG_DI:
                    case MODTAG_OSP:
                    {
                        ucModType = MODTYPE_INPUT;
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            uiParseDataSize = ConfigIOSlaveParse(&ucSectionAddr[uiBuffIndex],ucModType,pstIoModInfo);
                            
//                            DEBUG_CONFIG("  MODTYPE_INPUT: uiParseDataSize is : %d \n",uiParseDataSize);
                            
                            if(0U == uiParseDataSize)
                            {/*当解析长度为零时，表示该模块配置解析出错*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("INPUT FAIL\n");
                                
                            }
                            else
                            {
                                uiBuffIndex += uiParseDataSize;
                            }
                        }
                    }
                    break;

                    case MODTAG_AO:
                    case MODTAG_DO:
                    {
                        ucModType = MODTYPE_OUTPUT;
                        if(HANDLE_OK == cSecHandleResult)
                        {
                            uiParseDataSize = ConfigIOSlaveParse(&ucSectionAddr[uiBuffIndex],ucModType,pstIoModInfo);

//                            DEBUG_CONFIG("  MODTYPE_OUTPUT: uiParseDataSize is : %d \n",uiParseDataSize);
                            
                            if(0U == uiParseDataSize)
                            {
                                cSecHandleResult = HANDLE_FAIL;

                                DEBUG_CONFIG("OUTPUT FAIL\n");

                            }
                            else
                            {
                                uiBuffIndex += uiParseDataSize;
                            }
                        }
                    }
                    break;

                    default:
                    {
                        cSecHandleResult = HANDLE_FAIL;
                    }
                    break;
                }
            }
            if(HANDLE_OK == cSecHandleResult)
            {/*每一段的解析都正确，文件解析正确*/

                /*记录文件CRC及版本*/
                pstIoModInfo->uiCRC = pstFileHead->uiCheckSum;
                pstIoModInfo->uiFileVern = pstFileHead->uiVersion;

                cRet = RET_OK;
            }
        }

    }
    return cRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigPMConfigSectionParse
*
* 功能描述: 完成对PM Config段的解析配置
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
static int8_t ConfigPMConfigSectionParse(uint8_t ucSectionAddr[])
{
//    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    SysIoModInfo_t* pstIoModInfo = NULL;                        /*IO模块配置信息结构可写片区指针*/

    if(NULL == ucSectionAddr)
    {
        /*log*/
        DEBUG_CONFIG("NULL SectionAddr\n");
    }
    else
    {
        /*获取IO模块配置信息结构可写片区指针*/
        pstIoModInfo = SharedGetIoModInfoWrit();
        
        if(NULL == pstIoModInfo)
        {
            /*log*/
            
            DEBUG_CONFIG("NULL IoModInfo\n");
            
        }
        else
        {
            memset(&pstIoModInfo->stSysPMInfo, 0x00, sizeof(SysPMInfo_t));
            
            memcpy(&pstIoModInfo->stSysPMInfo, &ucSectionAddr[0], sizeof(SysPMInfo_t));

            cRet = RET_OK;
        }

    }
    return cRet;
}
/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigIOSlaveParse
*
* 功能描述: 完成对IO 从站配置信息的解析配置
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
static uint32_t ConfigIOSlaveParse(uint8_t ucIOSlaveInfo[],uint8_t ucModType,SysIoModInfo_t* pstIoModInfo)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    uint16_t usCount = 0U;
    uint16_t usParamSize = 0U;                                  /*参数长度*/
    //uint32_t uiDualRamOffset = 0U;                              /*双口RAM中的偏移*/
    uint8_t ucTaskID = 0U;
    uint16_t usDevNum = 0U;
    uint16_t usChannelNum = 0U;
    uint16_t usSubModNum = 0U;
    uint32_t uiRet = 0U;                                        /*模块解析数据的长度，当解析长度为零时，表示该模块配置信息有问题*/
    IOModuleConfigHead_t stIOModuleConfigHead;                  /*莫块配置头部信息*/
    IOModuleData_t stIOModuleData;                              /*RTS使用的配置信息结构*/
    uint8_t ucModNum = 0U;
    uint8_t ucModAddr = 0U;                                     /*模块ID*/
    uint8_t ucModDownLoadVern = 0U;

    if(NULL == ucIOSlaveInfo)
    {
        /*log*/
    }
    else if(NULL == pstIoModInfo)
    {
        /*log*/
    }
    else
    {
        /*获取从站信息头部*/
        memcpy(&stIOModuleConfigHead,ucIOSlaveInfo,sizeof(stIOModuleConfigHead));
        uiBuffIndex += sizeof(stIOModuleConfigHead);

        /*1、下发IO模块的参数配置解析*/
        /*获取IO参数长度*/
        memcpy((puint8_t)&usParamSize,&ucIOSlaveInfo[uiBuffIndex],sizeof(uint16_t));
        uiBuffIndex += sizeof(uint16_t);
        
        //DEBUG_CONFIG("  usParamSize is : %d \n",usParamSize);
        
        /*拷贝需要下发给IO的参数数据*/
        memcpy(&pstIoModInfo->stSysIoParamArea.ucIoParam[pstIoModInfo->stSysIoParamArea.usIoParamIndex], \
                &ucIOSlaveInfo[uiBuffIndex],usParamSize);

        ucModNum = pstIoModInfo->stSysIoParamArea.ucModNum;

//        DEBUG_CONFIG("  ucModNum is : %x \n",ucModNum);
        
        pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucModAddr = (uint8_t)(stIOModuleConfigHead.uiNodeId);
        pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucModType = ucModType;
        pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucChangeFlag = stIOModuleConfigHead.ucChangeFlag;
        pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usParamAreaDataLen = usParamSize;
        pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usParamAreaOffset = pstIoModInfo->stSysIoParamArea.usIoParamIndex;
        
        if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
        {
            ucModDownLoadVern = SysConfigGetModDownLoadVern(stIOModuleConfigHead.uiNodeId);
            
            if(2U == stIOModuleConfigHead.ucChangeFlag)
            {/*当模块是在线下装改变的模块时，将模块配置信息版本号加一*/

                if(0xff == ucModDownLoadVern)
                {
                    ucModDownLoadVern = 1;
                }
                else
                {
                    ucModDownLoadVern++;
                }
                SharedSetModConfigChangeFlg(stIOModuleConfigHead.uiNodeId, true);

                printf("%d FLG %x\n",ucModNum,ucModDownLoadVern);
            }
            
            if(0U == stIOModuleConfigHead.ucChangeFlag)
            {/*新增模块，版本从0开始计数*/
                ucModDownLoadVern = 0U;
            }
        }
        else
        {
            ucModDownLoadVern = 0U;
        }
        
        pstIoModInfo->stSysIoParamArea.ucIoParam[pstIoModInfo->stSysIoParamArea.usIoParamIndex] = ucModDownLoadVern;
        
        SysConfigSetModDownLoadVern(stIOModuleConfigHead.uiNodeId, ucModDownLoadVern);


        /*记录完该从站的参数起始位置后，更新参数区“写”的索引*/
        pstIoModInfo->stSysIoParamArea.usIoParamIndex += usParamSize;
        uiBuffIndex += usParamSize;
        
//        DEBUG_CONFIG("  ucModAddr is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucModAddr);
//        DEBUG_CONFIG("  ucModType is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucModType);
//        DEBUG_CONFIG("  ucChangeFlag is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].ucChangeFlag);
//        DEBUG_CONFIG("  usParamAreaDataLen is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usParamAreaDataLen);
//        DEBUG_CONFIG("  usParamAreaOffset is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usParamAreaOffset);
//        DEBUG_CONFIG("  usIoParamIndex is : %x \n",pstIoModInfo->stSysIoParamArea.usIoParamIndex);
        


        /*2、给RTS用的配置信息解析*/
        /*按照TaskID的不同和输入输出类型的不同，记录模块信息*/
        /*ModuleData 参数解析*/
        memcpy((puint8_t)&stIOModuleData, &ucIOSlaveInfo[uiBuffIndex], sizeof(IOModuleData_t));
        uiBuffIndex += sizeof(IOModuleData_t);
        
        ucTaskID = stIOModuleData.ucTaskID;
        
//        DEBUG_CONFIG("  ucTaskID is : %x \n",ucTaskID);
        
        

        if(MODTYPE_INPUT == ucModType)
        {
//            DEBUG_CONFIG("  MODTYPE_INPUT  !\n");
            
            /*配置IO参数中双口偏移及数据长度*/
            pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaOffset = stIOModuleData.uiDualInputOffset;
            pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaDataLen = (uint16_t)stIOModuleData.uiDualInputSize;
            
//            DEBUG_CONFIG("  usInputAreaOffset is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaOffset);
//            DEBUG_CONFIG("  usInputAreaDataLen is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaDataLen);
//            DEBUG_CONFIG("  usOutputAreaOffset is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaOffset);
//            DEBUG_CONFIG("  usOutputAreaDataLen is : %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaDataLen);
            

            /*配置给RTS用的模块参数*/
            usDevNum = pstIoModInfo->stSysIoSlaveInfo.usInDevNum[ucTaskID];
            
//            DEBUG_CONFIG("  usDevNum is : %x \n",usDevNum);
            
            pstIoModInfo->stSysIoSlaveInfo.ucInDevTabl[ucTaskID][usDevNum] = (uint8_t)stIOModuleConfigHead.uiNodeId;
            ucModAddr = (uint8_t)stIOModuleConfigHead.uiNodeId;
            
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag = stIOModuleConfigHead.usModuleTag;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiIecOffset = stIOModuleConfigHead.uiBitOffsetInput;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usIecSize = (uint16_t)stIOModuleConfigHead.uiBitSizeInput;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualInputOffset = stIOModuleData.uiDualInputOffset;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualInputSize = (uint16_t)stIOModuleData.uiDualInputSize;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualOutputOffset = stIOModuleData.uiDualOutputOffset;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualOutputSize = (uint16_t)stIOModuleData.uiDualOutputSize;
            
            
//            DEBUG_CONFIG("  ucModAddr is : %x \n",pstIoModInfo->stSysIoSlaveInfo.ucInDevTabl[ucTaskID][usDevNum]);
//            DEBUG_CONFIG("  usModTag is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag);
//            DEBUG_CONFIG("  uiIecOffset is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiIecOffset);
//            DEBUG_CONFIG("  usIecSize is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usIecSize);
//            printf("DInO %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualInputOffset);
//            printf("DInS %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualInputSize);
//            printf("DOutO %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualOutputOffset);
//            printf("DOutS %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualOutputSize);
            
            /*配置RTS用的模块内通道参数，此处是否还有用是否与表决文件重复，代码评审时讨论*/
            memcpy((puint8_t)&usChannelNum,&ucIOSlaveInfo[uiBuffIndex],sizeof(uint16_t));
            uiBuffIndex += sizeof(uint16_t);
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usChannelNum = usChannelNum;
            
//            DEBUG_CONFIG("  usChannelNum is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usChannelNum);
            
            for(usCount = 0; usCount < usChannelNum; usCount++)
            {
                memcpy(&pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].stSysIoChannelInfo[usCount], \
                        &ucIOSlaveInfo[uiBuffIndex], sizeof(SysIoChannelInfo_t));
                uiBuffIndex += sizeof(SysIoChannelInfo_t);
                
//                DEBUG_CONFIG("  usCount is : %x \n",usCount);
//                DEBUG_CONFIG("  sizeof(SysIoChannelInfo_t) is : %x \n",sizeof(SysIoChannelInfo_t));
                
            }
            /*配置RTS用的模块内子模块参数，预留，供以后其他有子模块的模块使用*/
            memcpy((puint8_t)&usSubModNum,&ucIOSlaveInfo[uiBuffIndex],sizeof(uint16_t));
            uiBuffIndex += sizeof(uint16_t);

            
//            for(usCount = 0; usCount < usSubModNum; usCount++)
//            {
//                memcpy(&pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].stSysIoSubModInfo[usCount],
//                        &ucIOSlaveInfo[uiBuffIndex],sizeof(SysIoSubModInfo_t));
//                uiBuffIndex += sizeof(SysIoSubModInfo_t);
//            }
            
            
            pstIoModInfo->stSysIoSlaveInfo.usInDevNum[ucTaskID]++;
            
            pstIoModInfo->stSysIoParamArea.ucModNum++;

        }
        else if(MODTYPE_OUTPUT == ucModType)
        {
            
//            DEBUG_CONFIG("  MODTYPE_OUTPUT  !\n");
            
            /*配置IO参数中双口偏移及数据长度*/
            pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaOffset = stIOModuleData.uiDualOutputOffset;
            pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaDataLen = (uint16_t)stIOModuleData.uiDualOutputSize;

//            printf("Inputoff %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaOffset);
//            printf("InputLen %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usInputAreaDataLen);
//            printf("OutputOff %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaOffset);
//            printf("OutputLen %x \n",pstIoModInfo->stSysIoParamArea.stIoParamIndex[ucModNum].usOutputAreaDataLen);
            
            /*配置给RTS用的模块参数*/
            usDevNum = pstIoModInfo->stSysIoSlaveInfo.usOutDevNum[ucTaskID];
            
//            DEBUG_CONFIG("DevNum %x\n",usDevNum);
            
            pstIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[ucTaskID][usDevNum] = (uint8_t)stIOModuleConfigHead.uiNodeId;
            ucModAddr = (uint8_t)stIOModuleConfigHead.uiNodeId;

            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag = stIOModuleConfigHead.usModuleTag;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiIecOffset = stIOModuleConfigHead.uiBitOffsetOutput;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usIecSize = (uint16_t)stIOModuleConfigHead.uiBitSizeOutput;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualInputOffset = stIOModuleData.uiDualInputOffset;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualInputSize = (uint16_t)stIOModuleData.uiDualInputSize;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualOutputOffset = stIOModuleData.uiDualOutputOffset;
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualOutputSize = (uint16_t)stIOModuleData.uiDualOutputSize;
            
            
//            DEBUG_CONFIG("  ucModAddr is : %x \n",pstIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[ucTaskID][usDevNum]);
//            DEBUG_CONFIG("  usModTag is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag);
//            DEBUG_CONFIG("  uiIecOffset is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiIecOffset);
//            DEBUG_CONFIG("  usIecSize is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usIecSize);
//            printf("DInO %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualInputOffset);
//            printf("DInS %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualInputSize);
//            printf("DOutO %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].uiDualOutputOffset);
//            printf("DOutS %x\n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usDualOutputSize);
            
            
            
            /*配置RTS用的模块内通道参数，此处是否还有用是否与表决文件重复，代码评审时讨论*/
            memcpy((puint8_t)&usChannelNum,&ucIOSlaveInfo[uiBuffIndex],sizeof(uint16_t));
            uiBuffIndex += sizeof(uint16_t);
            pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usChannelNum = usChannelNum;
            
//            DEBUG_CONFIG("  usChannelNum is : %x \n",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usChannelNum);
            
            for(usCount = 0U; usCount < usChannelNum; usCount++)
            {
                memcpy(&pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].stSysIoChannelInfo[usCount], \
                        &ucIOSlaveInfo[uiBuffIndex],sizeof(SysIoChannelInfo_t));
                uiBuffIndex += sizeof(SysIoChannelInfo_t);
                
//                DEBUG_CONFIG("  usCount is : %x \n",usCount);
//                DEBUG_CONFIG("  sizeof(SysIoChannelInfo_t) is : %x \n",sizeof(SysIoChannelInfo_t));
                
            }

            /*配置RTS用的模块内子模块参数，预留，供以后其他有子模块的模块使用*/
            memcpy((puint8_t)&usSubModNum,&ucIOSlaveInfo[uiBuffIndex],sizeof(uint16_t));
            uiBuffIndex += sizeof(uint16_t);

            
//            for(usCount = 0U; usCount < usSubModNum; usCount++)
//            {
//                memcpy(&pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].stSysIoSubModInfo[usCount],
//                        &ucIOSlaveInfo[uiBuffIndex],sizeof(SysIoSubModInfo_t));
//                uiBuffIndex += sizeof(SysIoSubModInfo_t);
//            }
            
            
            pstIoModInfo->stSysIoSlaveInfo.usOutDevNum[ucTaskID]++;
            
            pstIoModInfo->stSysIoParamArea.ucModNum++;
        }
        else
        {

            /*log*/
        }
    }

    uiRet = uiBuffIndex;

    return uiRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigSOEFileParse
*
* 功能描述: 完成对SOE文件的解析配置
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
static int8_t ConfigSOEFileParse(uint8_t ucSOEfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t ucRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;


    
    DEBUG_CONFIG("SOE\n");
    
    if(NULL == ucSOEfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucSOEfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
//        DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucSOEfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
    
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                
    //            DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
    //            DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
    //            DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
                
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable,ucSOEfileAddr + uiBuffIndex,sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    uiOffset = stSectionTable.uiOffset;
                    
    //                DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
    //                DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
    //                DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
    //                DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
    //                DEBUG_CONFIG("  stSectionTable.ucSameWithLastDownload  is : %x \n",stSectionTable.ucSameWithLastDownload);
                    
                    
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_SOE:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigSOEInfoSectionParse(ucSOEfileAddr + uiOffset, &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    /*log*/
                                    
                                    DEBUG_CONFIG("SOE FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
                        }
                        break;
                    }
                }
                if(HANDLE_OK == cSecHandleResult)
                {/*每一段的解析都正确，文件解析正确*/
                    ucRet = RET_OK;
                    
    //                DEBUG_CONFIG("cRet = RET_OK; ！\n");
                    
                }
            }
        }
    }
    DEBUG_CONFIG("SOE End !\n");
    
    return ucRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigSOESectionParse
*
* 功能描述: 完成对SOE文件中SOE信息段的解析配置
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
static int8_t ConfigSOEInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{

    uint32_t uiBuffIndex = 0U;                               /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiCount = 0U;
    uint32_t uiSOEVarCount = 0U;
    uint32_t uiSOEBlockNum = 0U;
    ActiveFlag_t emTaskActive[MAX_TASK_NUM];                /*用于统计软SOE块内变量分属哪些任务*/
    uint8_t ucBlockNum = 0U;                                /*用于软SOE拆分时，记录某一任务的软SOE块个数*/
    SOE_Block_Info_t stSOE_Block_Info;
    SoftSOEVar_t stSoftSOEVar;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;               /*SOE配置信息结构可写片区指针*/
    uint8_t ucBlockID = 0U;
    uint32_t uiSOEVarIndex = 0U;
    uint32_t uiSW_BaseAddr = 0U;

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取SOE配置信息结构可写片区指针*/
        pstSOEConfigInfo = SharedGetSOEConfigInfoWrit();
        if(NULL == pstSOEConfigInfo)
        {
            /*log*/
        }
        else
        {/*解析记录SOE配置信息*/

            /*每次进行配置,将信息结构初始化。只在全下装时进行SOE队列初始化*/
            memset((puint8_t)pstSOEConfigInfo, 0x00, sizeof(SOEConfigInfo_t));

            /*获取SOE块个数*/
            memcpy((puint8_t)&uiSOEBlockNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            
//            DEBUG_CONFIG("  uiSOEBlockNum is : %d \n",uiSOEBlockNum);
            
            if(uiSOEBlockNum > MAX_SOE_BLOCK_NUM)
            {
                DEBUG_CONFIG("SOENum > MAX_SOE_BLOCK_NUM\n");
            }
            else
            {
                for(uiCount = 0U; uiCount < uiSOEBlockNum; uiCount++)
                {/*逐个块解析文件*/
                    memcpy((puint8_t)&stSOE_Block_Info, &ucSectionAddr[uiBuffIndex], sizeof(SOE_Block_Info_t));
                    uiBuffIndex += sizeof(SOE_Block_Info_t);
                    
//                    DEBUG_CONFIG("  uiBlockID is : %d \n",stSOE_Block_Info.uiBlockID);
//                    DEBUG_CONFIG("  usBlockType is : %d \n",stSOE_Block_Info.usBlockType);
//                    DEBUG_CONFIG("  uiMaxRecordCount is : %d \n",stSOE_Block_Info.uiMaxRecordCount);
//                    DEBUG_CONFIG("  uiSOEVarNum is : %d \n",stSOE_Block_Info.uiSOEVarNum);
                    
                    ucBlockID = (uint8_t)stSOE_Block_Info.uiBlockID;
                    pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].usBlockType = stSOE_Block_Info.usBlockType;
                    pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiMaxRecordCount = stSOE_Block_Info.uiMaxRecordCount;
                    pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiSoftSOEVarNum = stSOE_Block_Info.uiSOEVarNum;
                    pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiSoftSOEVarIndex = uiSOEVarIndex;
                    pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiSW_BaseAddr = uiSW_BaseAddr;
                    
                    uiSW_BaseAddr += pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiMaxRecordCount;
                    if(uiSW_BaseAddr > MAX_SOE_RECORD_NUM)
                    {
                        /*log*/
                        
                        break;
                    }
                    
                    
//                    DEBUG_CONFIG("  uiSoftSOEVarIndex is : %d \n",pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].uiSoftSOEVarIndex);
    
                    
                    if((stSOE_Block_Info.uiSOEVarNum > MAX_SOE_VAR_SINGLE_BLOCK) ||
                       ((stSOE_Block_Info.uiSOEVarNum + uiSOEVarIndex) > MAX_SOE_VAR_NUM))
                    {
                        DEBUG_CONFIG("SOE VAR Num too many\n");
                        
                    }
                    else
                    {
                        /*软SOE块配置*/
                        memset(emTaskActive, 0x00,MAX_TASK_NUM*sizeof(ActiveFlag_t));
                        for(uiSOEVarCount = 0U; uiSOEVarCount < stSOE_Block_Info.uiSOEVarNum; uiSOEVarCount++)
                        {
                            memcpy((puint8_t)&stSoftSOEVar, &ucSectionAddr[uiBuffIndex], sizeof(SoftSOEVar_t));
                            uiBuffIndex += sizeof(SoftSOEVar_t);
                            emTaskActive[stSoftSOEVar.ucTaskID] = ACTIVE;
                            memcpy((puint8_t)&pstSOEConfigInfo->stSoftSOEVar[uiSOEVarIndex], \
                                    (puint8_t)&stSoftSOEVar, sizeof(stSoftSOEVar));/*根据任务ID将软SOE点配置信息记录下来*/

//                            DEBUG_CONFIG("  VAR %d : ucTaskID is : %d \n",uiSOEVarIndex,stSoftSOEVar.ucTaskID);
//                            DEBUG_CONFIG("  VAR %d : ucRefID is : %d \n",uiSOEVarIndex,stSoftSOEVar.ucRefID);
//                            DEBUG_CONFIG("  VAR %d : usVarSize is : %d \n",uiSOEVarIndex,stSoftSOEVar.usVarSize);
//                            DEBUG_CONFIG("  VAR %d : uiOffset is : %d \n",uiSOEVarIndex,stSoftSOEVar.uiOffset);
                            
                            uiSOEVarIndex++;
                            
                            
//                           DEBUG_CONFIG("  ucBlockNum is : %d \n",ucBlockNum);
                        }

                        if(ACTIVE == emTaskActive[TASK_UP1])/*若有变量属于任务1，填写其块的配置信息，并把任务1软SOE块数加一*/
                        {
                            ucBlockNum = pstSOEConfigInfo->ucSoftSOEBlockNum[TASK_UP1];
                            pstSOEConfigInfo->ucSoftSOEBlockTable[TASK_UP1][ucBlockNum] = ucBlockID;
                            pstSOEConfigInfo->ucSoftSOEBlockNum[TASK_UP1]++;

//                                DEBUG_CONFIG("  TASK_UP1: ucBlockNum is : %d \n",ucBlockNum);
//                                DEBUG_CONFIG("  TASK_UP1: uiBlockID is : %d \n",pstSOEConfigInfo->ucSoftSOEBlockTable[TASK_UP1][ucBlockNum]);

                        }
                        if(ACTIVE == emTaskActive[TASK_UP2])/*若有变量属于任务2，填写其块的配置信息，并把任务2软SOE块数加一*/
                        {
                            ucBlockNum = pstSOEConfigInfo->ucSoftSOEBlockNum[TASK_UP2];
                            pstSOEConfigInfo->ucSoftSOEBlockTable[TASK_UP2][ucBlockNum] = ucBlockID;
                            pstSOEConfigInfo->ucSoftSOEBlockNum[TASK_UP2]++;
                            
                            
//                                DEBUG_CONFIG("  TASK_UP2: ucBlockNum is : %d \n",ucBlockNum);
//                                DEBUG_CONFIG("  TASK_UP2: uiBlockID is : %d \n",pstSOEConfigInfo->ucSoftSOEBlockTable[TASK_UP2][ucBlockNum]);

                        }
                        
                        pstSOEConfigInfo->stSoftSOEBlock[ucBlockID].emActive = ACTIVE;
                    }
                }
            }

            /*记录文件CRC及版本*/
            pstSOEConfigInfo->uiCRC = pstFileHead->uiCheckSum;
            pstSOEConfigInfo->uiFileVern = pstFileHead->uiVersion;
            
            
            
//            DEBUG_CONFIG("  pstSOEConfigInfo->uiCRC is : %d \n",pstSOEConfigInfo->uiCRC);
//            DEBUG_CONFIG("  pstSOEConfigInfo->uiFileVern is : %d \n",pstSOEConfigInfo->uiFileVern);
            

            cRet = RET_OK;
        }

    }
    return cRet;
}


/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigP2PFileParse
*
* 功能描述: 完成对P2P文件的解析配置
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
static int8_t ConfigP2PFileParse(uint8_t ucP2PfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;
    
    DEBUG_CONFIG("P2P\n");


    if(NULL == ucP2PfileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucP2PfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
        //DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
        //DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
        //DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
        //DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
        //DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
        //DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
        //DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
        //DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00,(puint8_t)(&ucP2PfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                //DEBUG_CONFIG("  uiCheckSumCal != stFileHead.uiCheckSum \n");
                //DEBUG_CONFIG("  uiCheckSumCal is : %x \n",uiCheckSumCal);
                //DEBUG_CONFIG("  stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable,&ucP2PfileAddr[uiBuffIndex],sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    uiOffset = stSectionTable.uiOffset;
                    
                    //DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
                    //DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
                    //DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
                    //DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
                    //DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
                    
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_PEERTOPEER:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigP2PInfoSectionParse(&ucP2PfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("P2P FAIL ! \n");
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
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
    }
    
    //DEBUG_CONFIG("  cRet is : %d \n",cRet);
    DEBUG_CONFIG("P2P End\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigP2PInfoSectionParse
*
* 功能描述: 完成对P2P文件的解析配置
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
static int8_t ConfigP2PInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_OK;
    uint32_t uiCount = 0U;
    uint32_t uiP2PVarCount = 0U;
    uint32_t uiP2PBlockNum = 0U;
    uint32_t uiSendBlockNum = 0U;
    uint32_t uiRecvBlockNum = 0U;
    P2PBlockHead_t stP2PBlockHead;
    uint32_t uiStaID = 0U;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;                   /*P2P配置信息结构可写片区指针*/

    if(NULL == ucSectionAddr)
    {
        /*log*/
        cRet = RET_FAIL;
        DEBUG_CONFIG("1 NULL !\n");
    }
    else
    {
        /*获取P2P配置信息结构可写片区指针*/
        pstP2PConfigInfo = SharedGetP2PConfigInfoWrit();
        if(NULL == pstP2PConfigInfo)
        {
            /*log*/
            cRet = RET_FAIL;
            DEBUG_CONFIG("2 NULL !\n");
        }
        else
        {
            
            //DEBUG_CONFIG("    pstP2PConfigInfo is : %x \n",pstP2PConfigInfo);
            
            memcpy((puint8_t)&uiP2PBlockNum, ucSectionAddr, sizeof(uint32_t));/*获取P2P块总个数*/
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            
            //DEBUG_CONFIG("    uiP2PBlockNum is %d \n",uiP2PBlockNum);
            
            
            //DEBUG_CONFIG("    Step  1 \n");

            for(uiCount = 0U; uiCount < uiP2PBlockNum; uiCount++)
            {
                memcpy((puint8_t)&stP2PBlockHead, &ucSectionAddr[uiBuffIndex], sizeof(stP2PBlockHead));
                uiBuffIndex += sizeof(stP2PBlockHead);
                
                uiStaID = stP2PBlockHead.uiTargetStationID;
                
                //DEBUG_CONFIG("    Step  2 \n");
                
                if((stP2PBlockHead.usCommProperty == P2P_SEND_BLOCK) && (uiStaID <= MAX_P2P_BLOCK_NUM))
                {
                    
                    //DEBUG_CONFIG("    Step  3 \n");
                    
//                    uiSendBlockNum = pstP2PConfigInfo->stP2PConfigHead.uiSendBlockNum;
                    memcpy((puint8_t)&pstP2PConfigInfo->stP2PSendBlockInfo[uiSendBlockNum].stP2PBlockHead, \
                            (puint8_t)&stP2PBlockHead,sizeof(stP2PBlockHead));
                    
                    //DEBUG_CONFIG("    Step  4 \n");
                    
                    if(stP2PBlockHead.uiP2PVarNum > MAX_P2P_VAR_NUM)
                    {
                        DEBUG_CONFIG("S VarNum %d\n",stP2PBlockHead.uiP2PVarNum);
                        
                        cRet = RET_FAIL;
                        break;
                    }
                    
                    for(uiP2PVarCount = 0U; uiP2PVarCount < stP2PBlockHead.uiP2PVarNum; uiP2PVarCount++)
                    {
                        memcpy((puint8_t)&pstP2PConfigInfo->stP2PSendBlockInfo[uiSendBlockNum].stP2PVar[uiP2PVarCount], \
                                &ucSectionAddr[uiBuffIndex],sizeof(P2PVar_t));
                        uiBuffIndex += sizeof(P2PVar_t);
                    }
                    
                    //DEBUG_CONFIG("    Step  5 \n");
                    
                    pstP2PConfigInfo->stP2PSendBlockInfo[uiSendBlockNum].emActive = ACTIVE;
//                    pstP2PConfigInfo->stP2PConfigHead.uiSendBlockNum++;
                    uiSendBlockNum++;
                }
                else if((stP2PBlockHead.usCommProperty == P2P_RECV_BLOCK) && (uiStaID <= MAX_P2P_BLOCK_NUM))
                {
                    
                    //DEBUG_CONFIG("    Step  6 \n");
                    
//                    uiRecvBlockNum = pstP2PConfigInfo->stP2PConfigHead.uiRecvBlockNum;
                    memcpy((puint8_t)&pstP2PConfigInfo->stP2PRecvBlockInfo[uiStaID].stP2PBlockHead,\
                            (puint8_t)&stP2PBlockHead,sizeof(stP2PBlockHead));
                    
                    //DEBUG_CONFIG("    Step  7 \n");
                    
                    if(stP2PBlockHead.uiP2PVarNum > MAX_P2P_VAR_NUM)
                    {
                        DEBUG_CONFIG("R VarNum %d \n",stP2PBlockHead.uiP2PVarNum);
                        
                        cRet = RET_FAIL;
                        break;
                    }
                    
                    for(uiP2PVarCount = 0U; uiP2PVarCount < stP2PBlockHead.uiP2PVarNum; uiP2PVarCount++)
                    {
                        memcpy((puint8_t)&pstP2PConfigInfo->stP2PRecvBlockInfo[uiStaID].stP2PVar[uiP2PVarCount], \
                                &ucSectionAddr[uiBuffIndex],sizeof(P2PVar_t));
                        uiBuffIndex += sizeof(P2PVar_t);
                    }
                    
                    //DEBUG_CONFIG("    Step  8 \n");
                    
                    pstP2PConfigInfo->stP2PRecvBlockInfo[uiStaID].emActive = ACTIVE;
//                    pstP2PConfigInfo->stP2PConfigHead.uiRecvBlockNum++;
                    uiRecvBlockNum++;
                }
                else
                {
                    
                    DEBUG_CONFIG("CommProperty err %d\n",stP2PBlockHead.usCommProperty);
                    
                    cRet = RET_FAIL;
                    break;
                }
            }
            
            if(RET_OK == cRet)
            {
                pstP2PConfigInfo->stP2PConfigHead.uiRecvBlockNum = uiRecvBlockNum;
                pstP2PConfigInfo->stP2PConfigHead.uiSendBlockNum = uiSendBlockNum;
    
                /*记录文件CRC及版本*/
                pstP2PConfigInfo->uiCRC = pstFileHead->uiCheckSum;
                pstP2PConfigInfo->uiFileVern = pstFileHead->uiVersion;
            }
        }
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

    
    DEBUG_CONFIG("MMC\n");
    
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
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件数据CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucModbusMasterfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
//                
//                DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
//                DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
//                DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
                
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
    //                
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_MODBUSMASTER:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigModbusMasterSectionParse(&ucModbusMasterfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("MODBUSMASTER FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
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
    }
    
    DEBUG_CONFIG("MMC End !\n");
    
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

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取Modbus主配置信息可写片区结构指针*/
        pstModbusMasterInfo = SharedGetModbusMasterInfoWrit();
        /*获取CM槽位表可写片区的指针*/
        pstCMSlotIdTabl = SharedGetCMSlotIdTablWrit();
        if((NULL == pstModbusMasterInfo) || (NULL == pstCMSlotIdTabl))
        {
            /*log*/
            
            DEBUG_CONFIG("NULL ModbusMasterInfo  NULL CMSlotIdTabl \n");
            
        }
        else
        {/*解析数据*/
            /*初始化Modbus主配置信息结构*/
            memset((puint8_t)pstModbusMasterInfo, 0x00, sizeof(ModbusMasterInfo_t));
            /*获取配置了Modbus主站功能的CM个数*/
            memcpy((puint8_t)&uiCMNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            
//            DEBUG_CONFIG("CMNum is : %d\n",uiCMNum);
            
            if(uiCMNum > MAX_CM_NUM)
            {
                DEBUG_CONFIG("CMNum > MAX_CM_NUM\n");
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
                    
//                    DEBUG_CONFIG("  uiCMSlotID is : %d\n",uiCMSlotID);
                    
                    uiCMID = SysCheckCMSlotID(uiCMSlotID,pstCMSlotIdTabl);/*根据槽位号查出对应内部编号*/
                    if(INVALID_CM_SLOT_NUM == uiCMID)
                    {/*查找CM槽位号失败，终止解析*/
                        cSecHandleResult = HANDLE_FAIL;
                        
                        DEBUG_CONFIG("INVALID_CM_SLOT_NUM\n");
                        
                        break;
                        /*log*/
                    }
                    else
                    {
                        /*记录上传CM的文件信息，文件中存在那个CM的Modbus主配置信息，将记录置为true*/
                        s_pstSysCfgFileTable->bCfgFileModbusMaster[uiCMID] = CFG_FILE_UPDATED;
                        
//                        DEBUG_CONFIG("  CM%d: bCfgFileModbusMaster is : %d \n",uiCMID,s_pstSysCfgFileTable->bCfgFileModbusMaster[uiCMID]);
                    
                    }
                    
//                    DEBUG_CONFIG("  uiCMID is : %d\n",uiCMID);
                    
                    /*获取配置了Modbus主站功能的Port个数*/
                    memcpy((puint8_t)&uiPortNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
                    
//                    DEBUG_CONFIG("  uiPortNum is : %d\n",uiPortNum);
                    
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
                            
                            
//                            DEBUG_CONFIG("  usPortUpdate is : %d\n",usPortUpdate);
//                            DEBUG_CONFIG("  usPortID is : %d\n",usPortID);
//                            DEBUG_CONFIG("  uiMasterBlockNum is : %d\n",uiMasterBlockNum);
//                            DEBUG_CONFIG("  uiAddTCPStationNum is : %d\n",uiAddTCPStationNum);
//                            DEBUG_CONFIG("  uiDelTCPStationNum is : %d\n",uiDelTCPStationNum);
//                            
                            if(usPortID >= MAX_PORT_NUM)
                            {
                                cSecHandleResult = HANDLE_FAIL;
                                
                                break;
                            }
                            
                            if((uiAddTCPStationNum > MAX_MODBUS_SLAVE_NUM) || (uiDelTCPStationNum > MAX_MODBUS_SLAVE_NUM))
                            {/*变化从站个数超出性能允许范围*/
                                cSecHandleResult = HANDLE_FAIL;
                                
                                DEBUG_CONFIG("AddTCPNum || DelTCPNum > MAX_MODBUS_SLAVE_NUM\n");
                                
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
                                
//                                DEBUG_CONFIG("  AddTCPStation :%d IP is :%x \n",uiTCPChangeCount, 
//                                        pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiAddStaionIP[uiTCPChangeCount]);
                                
                            }
                            /*记录端口下对应减少从站IP地址*/
                            for(uiTCPChangeCount = 0U; uiTCPChangeCount < uiDelTCPStationNum; uiTCPChangeCount++)
                            {
                                memcpy((puint8_t)&pstModbusMasterInfo->stModbusMasterConfig[uiCMID].stModbusMasterPort[usPortID].stModbusSlaveChange.uiDelStaionIP[uiTCPChangeCount], \
                                        &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                                uiBuffIndex += sizeof(uint32_t);
                                
//                                DEBUG_CONFIG("  DelTCPStation :%d IP is :%x \n",uiTCPChangeCount, 
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
                                    
                                    DEBUG_CONFIG("BlockID > MAX_MODBUS_BLOCK_NUM %d\n",ucBlockID);
                                    
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
                                
//                                DEBUG_CONFIG("  uiModbusVarNum is : %d\n",uiModbusVarNum);
                                
                                
                                if(uiModbusVarNum > MAX_MODBUS_VAR_NUM)
                                {/*Modbus变量个数超限*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("ModbusVarNum > MAX_VAR_NUM %d\n",uiModbusVarNum);
                                    
                                    break;
                                    /*log*/
                                }
                                for(uiModbusVarCount = 0U; uiModbusVarCount < uiModbusVarNum; uiModbusVarCount++)
                                {
                                    memcpy((puint8_t)&pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount], \
                                            &ucSectionAddr[uiBuffIndex], sizeof(ModbusVar_t));
                                    uiBuffIndex += sizeof(ModbusVar_t);
                                    
//                                    DEBUG_CONFIG("  VAR %d : ucTaskID is : %d\n",uiModbusVarCount,
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].ucTaskID);
//                                    DEBUG_CONFIG("  VAR %d : ucRefID is : %d\n",uiModbusVarCount,
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].ucRefID);
//                                    DEBUG_CONFIG("  VAR %d : usVarSize is : %d\n",uiModbusVarCount,
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].usVarSize);
//                                    DEBUG_CONFIG("  VAR %d : uiOffset is : %d\n",uiModbusVarCount,
//                                            pstModbusMasterInfo->stModbusMasterBlock[ucBlockID].stModbusVar[uiModbusVarCount].uiOffset);
//                                    DEBUG_CONFIG("  VAR %d : uiModbusIndex is : %d\n",uiModbusVarCount,
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

    
    DEBUG_CONFIG("MSC\n");
    
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
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucModbusSlavefileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                
//                DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
//                DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
//                DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
                
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
                                    
                                    DEBUG_CONFIG("MODBUSSLAVE FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
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
    }
    
    DEBUG_CONFIG("MSC End\n");
    
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

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取Modbus从配置信息可写片区结构指针*/
        pstModbusSlaveInfo = SharedGetModbusSlaveInfoWrit();

        if((NULL == pstModbusSlaveInfo))
        {
            /*log*/
            
            DEBUG_CONFIG("NULL ModbusSlaveInfo\n");
            
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
            
//            DEBUG_CONFIG("  @@uiSlaveBlockNum is :%d \n",pstModbusSlaveInfo->uiBlockNum);
            
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
                    
                    memcpy((puint8_t)&pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead,
                            &ucSectionAddr[uiBuffIndex], sizeof(ModbusSlaveBlockHead_t));
                    uiBuffIndex += sizeof(ModbusSlaveBlockHead_t);
                    
//                    DEBUG_CONFIG("  BLOCK %d :uiBlockID is :%d \n",uiBlockID,
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBlockID);
//                    DEBUG_CONFIG("  BLOCK %d :ucModbusRgn is :%d \n",uiBlockID,
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.ucModbusRgn);
//                    DEBUG_CONFIG("  BLOCK %d :ucPermission is :%d \n",uiBlockID,
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.ucPermission);
//                    DEBUG_CONFIG("  BLOCK %d :uiBeginAddr is :%d \n",uiBlockID,
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBeginAddr);
//                    DEBUG_CONFIG("  BLOCK %d :uiEndAddr is :%d \n",uiBlockID,
//                            pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiEndAddr);
                    
//                    uiBlockID = pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiBlockID;

                    uiModbusVarNum = pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveBlockHead.uiModbusVarNum;
                    
//                    DEBUG_CONFIG("  BLOCK %d :uiModbusVarNum is :%d \n",uiBlockID, uiModbusVarNum);
                    if((uiBlockID > MAX_MODBUS_BLOCK_NUM) || (uiModbusVarNum > MAX_MODBUS_RGN_VAR_NUM))
                    {
                        cSecHandleResult = HANDLE_FAIL;
                        break;
                    }
                    else
                    {
                        for(uiModbusVarCount = 0U; uiModbusVarCount < uiModbusVarNum; uiModbusVarCount++)
                        {/*记录变量信息*/
                            memcpy((puint8_t)&stModbusVar,&ucSectionAddr[uiBuffIndex], sizeof(ModbusVar_t));
                            if(stModbusVar.uiModbusIndex > MAX_MODBUS_RGN_VAR_NUM)
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
                                
//                                DEBUG_CONFIG("  VAR %d :ucTaskID is :%d \n",stModbusVar.uiModbusIndex, 
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.ucTaskID);
//                                DEBUG_CONFIG("  VAR %d :ucRefID is :%d \n",stModbusVar.uiModbusIndex, 
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.ucRefID);
//                                DEBUG_CONFIG("  VAR %d :usVarSize is :%d \n",stModbusVar.uiModbusIndex, 
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.usVarSize);
//                                DEBUG_CONFIG("  VAR %d :uiOffset is :%d \n",stModbusVar.uiModbusIndex, 
//                                        pstModbusSlaveInfo->stModbusSlaveBlockInfo[uiBlockID].stModbusSlaveVar[stModbusVar.uiModbusIndex].stModbusVar.uiOffset);
//                                DEBUG_CONFIG("  VAR %d :uiModbusIndex is :%d \n",stModbusVar.uiModbusIndex, 
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
* 函数名称: ConfigVoteFileParse
*
* 功能描述: 完成对Vote文件的解析配置
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
static int8_t ConfigVoteFileParse(uint8_t ucVotefileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    
    DEBUG_CONFIG("VOT\n");
    
    if(NULL == ucVotefileAddr)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucVotefileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
        //DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
        //DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
        //DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
        //DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
        //DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
        //DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
        //DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
        //DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucVotefileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
                
                //DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
//                DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
//                DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
                
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable,ucVotefileAddr + uiBuffIndex,sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    
                    //DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
                    //DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
                    //DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
                    //DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
                    //DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
                    
                    uiOffset = stSectionTable.uiOffset;
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_VOTE:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigVoteInfoSectionParse(&ucVotefileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    
                                    DEBUG_CONFIG("VOT FAIL ！\n");
                                    
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d\n",stSectionTable.uiSectionId);
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
    }
    
    DEBUG_CONFIG("VOT End\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigVoteInfoSectionParse
*
* 功能描述: 完成对Vote文配置信息段的解析配置
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
static int8_t ConfigVoteInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{
    uint32_t ucBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiVoteModNum = 0U;
    uint32_t uiVoteModCount = 0U;
    uint16_t usChannelCount = 0U;
    uint16_t usChannelNum = 0U;
    uint8_t ucChannelID = 0U;
    VOTEConfigInfo_t* pstVOTEConfigInfo = NULL;                 /*表决配置信息结构可写片区指针*/
    VoteChannelInfo_t stVoteFileChannelInfo;
    uint32_t uiModAddr = 0U;

    if(NULL == ucSectionAddr)
    {
        /*log*/
        DEBUG_CONFIG("1 NULL\n");
    }
    else
    {
        /*获取表决配置信息结构可写片区指针*/
        pstVOTEConfigInfo = SharedGetVOTEConfigInfoWrit();
        if(NULL == pstVOTEConfigInfo)
        {
            /*log*/
            
            DEBUG_CONFIG("2 NULL\n");
            
        }
        else
        {
            /*初始化配置信息机构*/
            memset((puint8_t)pstVOTEConfigInfo, 0x00, sizeof(VOTEConfigInfo_t));

            memcpy((puint8_t)&uiVoteModNum, &ucSectionAddr[0], sizeof(uint32_t));/*获取配置的Modbus从站个数*/
            ucBuffIndex += sizeof(uint32_t);
            ucBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            pstVOTEConfigInfo->uiModNum = (uiVoteModNum / 2);

            DEBUG_CONFIG("ModNum %d \n", pstVOTEConfigInfo->uiModNum);


            for(uiVoteModCount = 0U; uiVoteModCount < pstVOTEConfigInfo->uiModNum; uiVoteModCount++)
            {
                memcpy(&uiModAddr,&ucSectionAddr[ucBuffIndex], sizeof(uint32_t));
                ucBuffIndex += sizeof(uint32_t);
                
                pstVOTEConfigInfo->ucModAddr[uiVoteModCount] = (uint8_t)uiModAddr;
                
//                DEBUG_CONFIG("ModAddr %d \n", pstVOTEConfigInfo->ucModAddr[uiVoteModCount]);
                
                memcpy((puint8_t)&pstVOTEConfigInfo->stVoteModInfo[uiModAddr].usChannelCount, \
                        &ucSectionAddr[ucBuffIndex], sizeof(uint16_t));
                ucBuffIndex += sizeof(uint16_t);
                
//                DEBUG_CONFIG("ChannelCount %d\n", pstVOTEConfigInfo->stVoteModInfo[uiModAddr].usChannelCount);

                usChannelNum = pstVOTEConfigInfo->stVoteModInfo[uiModAddr].usChannelCount;
                
                for(usChannelCount = 0U; usChannelCount < usChannelNum; usChannelCount++)
                {
                    memcpy((puint8_t)&ucChannelID, &ucSectionAddr[ucBuffIndex], sizeof(ucChannelID));
                    ucBuffIndex += sizeof(ucChannelID);
                    memcpy((puint8_t)&stVoteFileChannelInfo, &ucSectionAddr[ucBuffIndex], sizeof(VoteChannelInfo_t));
                    ucBuffIndex += sizeof(VoteChannelInfo_t);

                    pstVOTEConfigInfo->stVoteModInfo[uiModAddr].stVoteChannelInfo[ucChannelID].ucVoteType = stVoteFileChannelInfo.ucVoteType;
                    pstVOTEConfigInfo->stVoteModInfo[uiModAddr].stVoteChannelInfo[ucChannelID].uiSafetyValue = stVoteFileChannelInfo.uiSafetyValue;
//                    DEBUG_CONFIG("%d uiSafetyValue: %d \n", usChannelCount,
//                            pstVOTEConfigInfo->stVoteModInfo[uiModAddr].stVoteChannelInfo[ucChannelID].uiSafetyValue);
//                    DEBUG_CONFIG("%d ucVoteType: %d \n", usChannelCount,
//                            pstVOTEConfigInfo->stVoteModInfo[uiModAddr].stVoteChannelInfo[ucChannelID].ucVoteType);
                }
                
                pstVOTEConfigInfo->stVoteModInfo[uiModAddr].emActive = ACTIVE;
            }

            /*记录文件CRC及版本*/
            pstVOTEConfigInfo->uiCRC = pstFileHead->uiCheckSum;
            pstVOTEConfigInfo->uiFileVern = pstFileHead->uiVersion;

            cRet = RET_OK;
        }

    }
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigRetainFileParse
*
* 功能描述: 完成对Retain文件的解析配置
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
static int8_t ConfigRetainFileParse(uint8_t ucRetainfileAddr[])
{
    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    int8_t cSecHandleResult = HANDLE_OK;                        /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
    uint32_t uiCheckSumCal = 0U;
    uint32_t uiCount = 0U;
    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
    FileHead_t stFileHead;
    SectionTable_t stSectionTable;

    
    DEBUG_CONFIG("RTN\n");
    
    if(ucRetainfileAddr == NULL)
    {
        /*log*/
    }
    else
    {
        /*将文件头拷贝出来*/
        memcpy(&stFileHead,ucRetainfileAddr,sizeof(stFileHead));
        uiBuffIndex += sizeof(stFileHead);
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucRetainfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable, ucRetainfileAddr + uiBuffIndex,sizeof(stSectionTable));
                    uiBuffIndex += sizeof(stSectionTable);
                    uiOffset = stSectionTable.uiOffset;
    
                    switch(stSectionTable.uiSectionId)
                    {
                        case PRG_SECTION_ID_RETAIN:
                        {
                            if(HANDLE_OK == cSecHandleResult)
                            {
                                if(RET_OK != ConfigRetainInfoSectionParse(&ucRetainfileAddr[uiOffset], &stFileHead))
                                {/*解析段内容错误，设置段处理失败*/
                                    cSecHandleResult = HANDLE_FAIL;
                                    DEBUG_CONFIG("RETAIN FAIL\n");
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d\n",stSectionTable.uiSectionId);
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
    }
    
    DEBUG_CONFIG("RTN End\n");
    
    return cRet;
}

/*
*********************************函数描述******************************************************************************
* 函数名称: ConfigRetainInfoSectionParse
*
* 功能描述: 完成对Retain文配置信息段的解析配置
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
static int8_t ConfigRetainInfoSectionParse(uint8_t ucSectionAddr[], FileHead_t* pstFileHead)
{

    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
    int8_t cRet = RET_FAIL;
    uint32_t uiRetainVarNum = 0U;
    uint32_t uiRetainVarCount = 0U;
    uint32_t uiUP1RetainVarCount = 0U;
    uint32_t uiUP2RetainVarCount = 0U;
    RetainVarInfo_t* pstRetainVarInfo = NULL;                   /*Retain变量配置信息结构可写片区指针*/
    RetainVar_t* pstRetainVar = NULL;

    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        /*获取Retain变量配置信息结构可写片区指针*/
        pstRetainVarInfo = SharedGetRetainVarInfoWrit();
        if(NULL == pstRetainVarInfo)
        {
            /*log*/
        }
        else
        {
            /*初始化配置信息结构*/
            memset((puint8_t)pstRetainVarInfo, 0x00, sizeof(RetainVarInfo_t));

            memcpy((puint8_t)&uiRetainVarNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));/*获取配置的Retain变量个数*/
            uiBuffIndex += sizeof(uint32_t);
            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
            
            printf("RTNVarNum  %d\n",uiRetainVarNum);
            
//            pstRetainVarInfo->uiVarNum = uiRetainVarNum;
            if(uiRetainVarNum < MAX_RETAIN_NUM)
            {
                for(uiRetainVarCount = 0U; uiRetainVarCount < uiRetainVarNum; uiRetainVarCount++)
                {
                    pstRetainVar = (RetainVar_t*)&ucSectionAddr[uiBuffIndex];
                    
                    
//                    printf(" TaskID : %d",pstRetainVar->ucTaskID);
//                    printf(" RefID : %d",pstRetainVar->ucRefID);
//                    printf(" VarSize : %d",pstRetainVar->usVarSize);
//                    printf(" Offset : %d\n",pstRetainVar->uiOffset);
                    
                    if(TASK_UP1 == pstRetainVar->ucTaskID)
                    {
                    
                        memcpy((puint8_t)&pstRetainVarInfo->stRetainVar[TASK_UP1][uiUP1RetainVarCount], \
                                &ucSectionAddr[uiBuffIndex], sizeof(RetainVar_t));
                        uiBuffIndex += sizeof(RetainVar_t);
                        uiUP1RetainVarCount++;
                    }
                    if(TASK_UP2 == pstRetainVar->ucTaskID)
                    {
                        memcpy((puint8_t)&pstRetainVarInfo->stRetainVar[TASK_UP2][uiUP2RetainVarCount], \
                                &ucSectionAddr[uiBuffIndex], sizeof(RetainVar_t));
                        uiBuffIndex += sizeof(RetainVar_t);
                        uiUP2RetainVarCount++;
                    }
                }
                
//                printf("UP1RTNVar %d",uiUP1RetainVarCount);
                
//                printf("UP2RTNVar %d \n",uiUP2RetainVarCount);
                /*记录各自任务的retain变量个数*/
                pstRetainVarInfo->uiUPVarNum[TASK_UP1] = uiUP1RetainVarCount;
                
                pstRetainVarInfo->uiUPVarNum[TASK_UP2] = uiUP2RetainVarCount;
    
                /*记录文件CRC及版本*/
                pstRetainVarInfo->uiCRC = pstFileHead->uiCheckSum;
                pstRetainVarInfo->uiFileVern = pstFileHead->uiVersion;
    
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
//static int8_t ConfigOPCFileParse(uint8_t ucOPCfileAddr[])
//{
//    int8_t cRet = RET_FAIL;
//    uint32_t uiBuffIndex = 0U;                                   /*文件内容地址偏移*/
//    int8_t cSecHandleResult = HANDLE_OK;                         /*段内容处理结果，当包含多个段时，任何一段解析错误，整个文件解析失败*/
//    uint32_t uiCheckSumCal = 0U;
//    uint32_t uiCount = 0U;
//    uint32_t uiOffset = 0U;                                     /*用于记录段内容偏移地址*/
//    FileHead_t stFileHead;
//    SectionTable_t stSectionTable;
//
//    
//    //DEBUG_CONFIG("ConfigOPCFileParse begin !\n");
//    
//    if(NULL == ucOPCfileAddr)
//    {
//        /*log*/
//    }
//    else
//    {
//        /*将文件头拷贝出来*/
//        memcpy(&stFileHead, ucOPCfileAddr, sizeof(stFileHead));
//        uiBuffIndex += sizeof(stFileHead);
//        
//        //DEBUG_CONFIG("stFileHead.uiCheckSum  is : %x \n",stFileHead.uiCheckSum);
//        //DEBUG_CONFIG("stFileHead.uiSize  is : %x \n",stFileHead.uiSize);
//        //DEBUG_CONFIG("stFileHead.uiVersion  is : %x \n",stFileHead.uiVersion);
//        //DEBUG_CONFIG("stFileHead.uiFileID  is : %x \n",stFileHead.uiFileID);
//        //DEBUG_CONFIG("stFileHead.uiSectionNum  is : %x \n",stFileHead.uiSectionNum);
//        //DEBUG_CONFIG("stFileHead.uiTimeDateStamp  is : %x \n",stFileHead.uiTimeDateStamp);
//        //DEBUG_CONFIG("stFileHead.uiStringTableOffset  is : %x \n",stFileHead.uiStringTableOffset);
//        //DEBUG_CONFIG("stFileHead.uiFileSeriaNum  is : %x \n",stFileHead.uiFileSeriaNum);
//
//        /*计算文件总CRC*/
//        uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucOPCfileAddr[sizeof(stFileHead.uiCheckSum)]), 
//                stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
//        if(uiCheckSumCal != stFileHead.uiCheckSum)
//        {
//            /*log*/
//            
//            //DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
//            //DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
//            //DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
//        }
//        else
//        {/*计算整个文件的CRC正确，解析文件*/
//
//            for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
//            {/*逐个段解析*/
//                /*拷贝解析段表*/
//                memcpy(&stSectionTable, &ucOPCfileAddr[uiBuffIndex], sizeof(stSectionTable));
//                uiBuffIndex += sizeof(stSectionTable);
//                uiOffset = stSectionTable.uiOffset;
//                
//                //DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
//                //DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
//                //DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
//                //DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
//                //DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
//                
//                switch(stSectionTable.uiSectionId)
//                {
//                    case PRG_SECTION_ID_OPC:
//                    {
//                        if(HANDLE_OK == cSecHandleResult)
//                        {
//                            if(RET_OK != ConfigOPCInfoSectionParse(&ucOPCfileAddr[uiOffset], &stFileHead))
//                            {/*解析段内容错误，设置段处理失败*/
//                                cSecHandleResult = HANDLE_FAIL;
//                                
//                                //DEBUG_CONFIG("PRG_SECTION_ID_OPC  HANDLE_FAIL ！\n");
//                                
//                            }
//                        }
//                    }
//                    break;
//
//                    default :
//                    {
//                        cSecHandleResult = HANDLE_FAIL;
//                        
//                        //DEBUG_CONFIG("  stSectionTable.uiSectionId is ERROR : %d ! \n",stSectionTable.uiSectionId);
//                    }
//                    break;
//                }
//            }
//            if(HANDLE_OK == cSecHandleResult)
//            {/*每一段的解析都正确，文件解析正确*/
//                cRet = RET_OK;
//            }
//        }
//    }
//    
//    //DEBUG_CONFIG("ConfigOPCFileParse End !\n");
//    
//    return cRet;
//}

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
//static int8_t ConfigOPCInfoSectionParse(uint8_t ucSectionAddr[],FileHead_t* pstFileHead)
//{
//    int8_t ucRet = RET_FAIL;
//    uint32_t uiBuffIndex = 0U;                                  /*文件内容地址偏移*/
//    uint32_t uiOPCVarNum = 0U;
//    uint32_t uiOPCVarCount = 0U;
//    OPCVarInfo_t* pstOPCVarInfo = NULL;                         /*OPC配置信息可写片段指针*/
//
//    if(NULL == ucSectionAddr)
//    {
//        /*log*/
//    }
//    else
//    {
//        /*获取OPC配置信息可写片段指针*/
//        pstOPCVarInfo = SharedGetOPCVarInfoWrit();
//        if(NULL == pstOPCVarInfo)
//        {
//            /*log*/
//        }
//        else
//        {
//            /*初始化OPC配置信息可写部分*/
//            memset((puint8_t)pstOPCVarInfo, 0x00, sizeof(OPCVarInfo_t));
//            memcpy((puint8_t)&uiOPCVarNum, ucSectionAddr, sizeof(uint32_t));
//            uiBuffIndex += sizeof(uint32_t);
//            uiBuffIndex += sizeof(uint32_t);/*跳过预留字节*/
//            pstOPCVarInfo->uiVarNum = uiOPCVarNum;
//            
//            //DEBUG_CONFIG("    pstOPCVarInfo->uiVarNum : %d \n",pstOPCVarInfo->uiVarNum);
//            
//            if(pstOPCVarInfo->uiVarNum > MAX_OPC_NUM)
//            {
//                //DEBUG_CONFIG("    pstOPCVarInfo->uiVarNum > MAX_OPC_NUM  : %d \n",pstOPCVarInfo->uiVarNum);
//            }
//            else
//            {
//                for(uiOPCVarCount = 0U; uiOPCVarCount < uiOPCVarNum; uiOPCVarCount++)
//                {
//                    memcpy((puint8_t)&pstOPCVarInfo->stOPCVar[uiOPCVarCount], &ucSectionAddr[uiBuffIndex],
//                            sizeof(OPCVar_t));
//                    uiBuffIndex += sizeof(OPCVar_t);
//                    //DEBUG_CONFIG("    stOPCVar[%d].ucTaskID is : %d\n",uiOPCVarCount, 
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].ucTaskID);
//                    //DEBUG_CONFIG("    stOPCVar[%d].ucRefID is : %d\n",uiOPCVarCount, 
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].ucRefID);
//                    //DEBUG_CONFIG("    stOPCVar[%d].usVarSize is : %d\n",uiOPCVarCount, 
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].usVarSize);
//                    //DEBUG_CONFIG("    stOPCVar[%d].uiOffset is : %d\n",uiOPCVarCount, 
//                            pstOPCVarInfo->stOPCVar[uiOPCVarCount].uiOffset);
//                }
//                
//                pstOPCVarInfo->emActive = ACTIVE;
//
//                /*记录文件CRC及版本*/
//                pstOPCVarInfo->uiCRC = pstFileHead->uiCheckSum;
//                pstOPCVarInfo->uiFileVern = pstFileHead->uiVersion;
//
//                ucRet = RET_OK;
//            }
//        }
//    }
//    
//    return ucRet;
//}


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
    
    
    DEBUG_CONFIG("CM\n");

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
        
        if((stFileHead.uiSize >= sizeof(FileHead_t)) && (stFileHead.uiSize <= MAX_FILE_LEN))
        {
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucCMConfigfileAddr[sizeof(stFileHead.uiCheckSum)]), \
                    stFileHead.uiSize - sizeof(stFileHead.uiCheckSum));
            uiBuffIndex += sizeof(stFileHead);
            if(uiCheckSumCal != stFileHead.uiCheckSum)
            {
                /*log*/
    //            DEBUG_CONFIG("uiCheckSumCal != stFileHead.uiCheckSum  !\n");
    //            DEBUG_CONFIG("uiCheckSumCal is : %x \n",uiCheckSumCal);
    //            DEBUG_CONFIG("stFileHead.uiCheckSum is : %x \n",stFileHead.uiCheckSum);
            }
            else
            {/*计算整个文件的CRC正确，解析文件*/
    
                for(uiCount = 0U; uiCount < stFileHead.uiSectionNum;uiCount++)
                {/*逐个段解析*/
                    /*拷贝解析段表*/
                    memcpy(&stSectionTable, &ucCMConfigfileAddr[uiBuffIndex], sizeof(stSectionTable));
                    
    //                //DEBUG_CONFIG("  stSectionTable.uiSectionId  is : %x \n",stSectionTable.uiSectionId);
    //                //DEBUG_CONFIG("  stSectionTable.uiOffset  is : %x \n",stSectionTable.uiOffset);
    //                //DEBUG_CONFIG("  stSectionTable.uiSize  is : %x \n",stSectionTable.uiSize);
    //                //DEBUG_CONFIG("  stSectionTable.uiMsg  is : %x \n",stSectionTable.uiMsg);
    //                //DEBUG_CONFIG("  stSectionTable.uiSameWithLastDownload  is : %x \n",stSectionTable.uiSameWithLastDownload);
                    
                    
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
                                    DEBUG_CONFIG("CM_CONFIG FAIL ！\n");
                                }
                            }
                        }
                        break;
    
                        default :
                        {
                            cSecHandleResult = HANDLE_FAIL;
                            
                            DEBUG_CONFIG("SectionId ERR %d !\n",stSectionTable.uiSectionId);
                        }
                        break;
                    }
                }
                if(HANDLE_OK == cSecHandleResult)
                {/*每一段的解析都正确，文件解析正确*/
                    cRet = RET_OK;
//                    DEBUG_CONFIG("RET_OK ！\n");
                }
            }
        }
    }
    
    
    DEBUG_CONFIG("CM End\n");
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
    CMSlotIdTabl_t* pstCMSlotIdTablWrit = NULL;         /*CM槽位表可读片区的指针，供解析文件时进行比对*/
    CMConfigInfo_t* pstCMConfigInfo = NULL;             /*CM配置信息可写片区的指针，供解析文件将配置信息写入*/
//    SysCfgFileTable_t* pstSysCfgFileTable = NULL;       /*上传CM的文件统计表*/

//    DEBUG_CONFIG("CM Parse\n");
    
    if(NULL == ucSectionAddr)
    {
        /*log*/
    }
    else
    {
        pstCMSlotIdTablTmp = &stCMSlotIdTabl;
        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
        pstCMSlotIdTablWrit = SharedGetCMSlotIdTablWrit();
        pstCMConfigInfo = SharedGetCMConfigInfoWrit();
        if((NULL == pstCMSlotIdTablTmp) || (NULL == pstCMSlotIdTablWrit) || (NULL == pstCMConfigInfo))
        {
            /*log*/
            
            //DEBUG_CONFIG("  NULL == pstCMSlotIdTablTmp || NULL == pstCMSlotIdTablWrit || NULL == pstCMConfigInfo  \n");
        }
        else
        {
            
            uiBuffIndex += 8*sizeof(char_t);/*跳过字符串*/

            memset((puint8_t)pstCMSlotIdTablTmp,0x00,sizeof(CMSlotIdTabl_t));
            
            /*记录当前配置的CM的槽位号，并按照从小到大将槽位号对应到CM ID*/
            memcpy((puint8_t)&uiCMNum, &ucSectionAddr[uiBuffIndex], sizeof(uiCMNum));
            uiBuffIndex += sizeof(uiCMNum);
            pstCMSlotIdTablTmp->uiCMNum = uiCMNum;
            
//            DEBUG_CONFIG("CMNum: %d \n",pstCMSlotIdTablTmp->uiCMNum);

            if(ONLINE_DOWNLOAD == SharedGetDownloadFlg())
            {/*在线下装时，需要保留原有的CM的槽位号与逻辑ID号的对应关系，将去除的CM删除，将新加的插入其他可用逻辑位置*/
                s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileIncreTable();
                if(pstCMSlotIdTabl != NULL)
                {
                    for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
                    {
                        memcpy((puint8_t)&uiSlotID, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                        
                        uiBuffIndex += sizeof(uint32_t);
                        
                        ConfigInsertCMSlotID(uiSlotID, pstCMSlotIdTabl, pstCMSlotIdTablTmp);
                    }
                }
                else
                {
                    for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
                    {
                        memcpy((puint8_t)&pstCMSlotIdTablTmp->uiCMSlotId[uiCount], &ucSectionAddr[uiBuffIndex], \
                                sizeof(uint32_t));
                        pstCMSlotIdTablTmp->uiEffectiveFlg[uiCount] = 1U;
                        uiBuffIndex += sizeof(uint32_t);
                        
                    }
                }
            }
            else
            {/*非增量下装的情况，不需要保留原有的CM的槽位号与逻辑ID号的对应关系*/
                /*初始化CM槽位表*/

                for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
                {
                    memcpy((puint8_t)&pstCMSlotIdTablTmp->uiCMSlotId[uiCount], &ucSectionAddr[uiBuffIndex], \
                            sizeof(uint32_t));
                    pstCMSlotIdTablTmp->uiEffectiveFlg[uiCount] = 1U;
                    uiBuffIndex += sizeof(uint32_t);
                    
//                    DEBUG_CONFIG("  pstCMSlotIdTablTmp->uiCMSlotId[%d] is : %d \n",uiCount,pstCMSlotIdTablTmp->uiCMSlotId[uiCount]);
                }
            }
            /*将本地临时记录CM槽位号逻辑表，拷贝入系统资源*/
            memcpy(pstCMSlotIdTablWrit, pstCMSlotIdTablTmp, sizeof(CMSlotIdTabl_t));

            /*初始化配置信息结构*/
            memset((puint8_t)pstCMConfigInfo,0x00,sizeof(CMConfigInfo_t));

            /*按照CM为单位解析CM的配置信息*/
            for(uiCount = 0U; uiCount < uiCMNum; uiCount++)
            {
                memcpy((puint8_t)&uiSlotID, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                uiBuffIndex += sizeof(uint32_t);
                uiCMID = SysCheckCMSlotID(uiSlotID,pstCMSlotIdTablTmp);
                if(INVALID_CM_SLOT_NUM == uiCMID)
                {
                    break;
                }
                else
                {
                    /*记录上传CM的文件信息，文件中存在那个CM的配置信息，将记录置为true*/
                    s_pstSysCfgFileTable->bCfgFileCM[uiCMID] = CFG_FILE_UPDATED;
                    
//                    DEBUG_CONFIG("  CM%d: bCfgFileCM is : %d \n",uiCMID,s_pstSysCfgFileTable->bCfgFileCM[uiCMID]);
                    
                    
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].usSlotID = (uint16_t)uiSlotID;
                    
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/

                    /*记录串口配置信息*/
                    memcpy((puint8_t)&uiCOMNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
//                    printf("COMNum: %d\n",uiCOMNum);

                    for(uiIndex = 0U; uiIndex < uiCOMNum; uiIndex++)
                    {/*逐个串口，按照串口号来记录串口信息*/
                        uiBuffIndex += sizeof(uint8_t);/*取到端口号的位置*/
                        memcpy((puint8_t)&ucCOMID,&ucSectionAddr[uiBuffIndex],sizeof(uint8_t));
                        uiBuffIndex = uiBuffIndex - sizeof(uint8_t);/*重新取到串口配置信息的开始*/
                        pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].emActive = ACTIVE;
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stCOMConfigInfo[ucCOMID].bUpdate,
                                &ucSectionAddr[uiBuffIndex], (sizeof(COMConfigInfo_t) - sizeof(ActiveFlag_t)));
                        uiBuffIndex += (sizeof(COMConfigInfo_t) - sizeof(ActiveFlag_t));
                    }

                    /*记录网口配置信息*/
                    memcpy((puint8_t)&uiNetNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
//                    printf("NetNum: %d\n",uiNetNum);
                    

                    for(uiIndex = 0U; uiIndex < uiNetNum; uiIndex++)
                    {
                        uiBuffIndex += sizeof(uint8_t);/*取到网口号的位置*/
                        memcpy((puint8_t)&ucNetID, &ucSectionAddr[uiBuffIndex], sizeof(uint8_t));
                        uiBuffIndex = uiBuffIndex - sizeof(uint8_t);/*重新取到网口配置信息的开始*/
                        pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].emActive = ACTIVE;
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stNetConfigInfo[ucNetID].bUpdate,
                                &ucSectionAddr[uiBuffIndex], (sizeof(NetConfigInfo_t) - sizeof(ActiveFlag_t)));
                        uiBuffIndex += (sizeof(NetConfigInfo_t) - sizeof(ActiveFlag_t));
                    }

                    
                    
                    
                    /*记录 Modbus TCP 配置信息*/
                    memcpy((puint8_t)&uiModbusTCPNum, &ucSectionAddr[uiBuffIndex], sizeof(uint32_t));
                    uiBuffIndex += sizeof(uint32_t);
                    uiBuffIndex += sizeof(uint32_t);/*跳过预留*/
                    
//                    printf("ModbusTCPNum: %d\n",uiModbusTCPNum);
                    
                    for(uiIndex = 0U; uiIndex < uiModbusTCPNum; uiIndex++)
                    {
                        memcpy((puint8_t)&ucModbusTCPID,&ucSectionAddr[uiBuffIndex],sizeof(uint8_t));
                        uiBuffIndex += sizeof(uint8_t);
                        pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].emActive = ACTIVE;
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stModbusTCPConfigInfo[ucModbusTCPID].bUpdate,
                                &ucSectionAddr[uiBuffIndex], (sizeof(ModbusTCPConfigInfo_t) - sizeof(ActiveFlag_t)));
                        uiBuffIndex += (sizeof(ModbusTCPConfigInfo_t) - sizeof(ActiveFlag_t));
                    }

                    /*记录 SNTP/NTP校时服务器IP地址配置信息*/
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.emActive = ACTIVE;
                    memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stNTPTCPConfigInfo.bUpdate,
                            &ucSectionAddr[uiBuffIndex], (sizeof(NTPTCPConfigInfo_t) - sizeof(ActiveFlag_t)));
                    uiBuffIndex += (sizeof(NTPTCPConfigInfo_t) - sizeof(ActiveFlag_t));

                    /*记录访问控制配置信息*/
                    memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead, \
                            &ucSectionAddr[uiBuffIndex], sizeof(AccessCtrlHead_t));
                    uiBuffIndex += sizeof(AccessCtrlHead_t);
                    
//                    printf("AccessCtrlNum: %d\n",pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead.ucAccessCtrlNum);
                    
                    for(uiIndex = 0U; uiIndex < pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlHead.ucAccessCtrlNum; uiIndex++)
                    {
                        memcpy((puint8_t)&pstCMConfigInfo->stCMSingleInfo[uiCMID].stAccessCtrlConfigInfo.stAccessCtrlConfig[uiIndex], \
                                &ucSectionAddr[uiBuffIndex], sizeof(AccessCtrlConfig_t));
                        uiBuffIndex += sizeof(AccessCtrlConfig_t);
                    }/*按照结构逐一拷贝访问控制配置信息*/
                    
                    pstCMConfigInfo->stCMSingleInfo[uiCMID].emActive = ACTIVE;
                }
            }
            
            /*在配置信息中记录文件版本和CRC*/
            pstCMConfigInfo->uiCRC = pstFileHead->uiCheckSum;
            pstCMConfigInfo->uiFileVern = pstFileHead->uiVersion;


            cRet = RET_OK;
        }
    }
//    DEBUG_CONFIG("CM Parse end\n");
    
    return cRet;
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

    return;
}


/*
***********************************************************************************************************************
* 函数名称: ConfigFillDualDataTable
*
* 功能描述: 根据配置填充从站输入输出区基地址及长度数据表
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
static void ConfigFillDualDataTable(void)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint8_t ucSlaveNo = 0U;

    pstSysIoModInfo = SharedGetIoModInfoWrit();
    if (pstSysIoModInfo == NULL)
    {
        return;
    }

    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        /* 填充输入从站双口输入区基地址数据表、长度数据表；输入从站双口输出区基地址数据表、长度数据表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]; i++)
        {
            ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[j][i];
            IPBUSSetSlaveInputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset);
            IPBUSSetSlaveInputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
            IPBUSSetSlaveOutputAddr(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset);
            IPBUSSetSlaveOutputLen(ucSlaveNo, pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
        }

        /* 填充输出从站双口输入区基地址数据表、长度数据表；输出从站双口输出区基地址数据表、长度数据表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j]; i++)
        {
            ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[j][i];
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
* 函数名称: ConfigFillDualPrmTable
*
* 功能描述: 根据配置填充从站参数区基地址及长度数据表
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
static void ConfigFillDualPrmTable(uint8_t ucBufID)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint16_t usTotalSlaveNum = 0U;

    pstSysIoModInfo = SharedGetIoModInfoWrit();

    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        usTotalSlaveNum = usTotalSlaveNum + pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]
                          + pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j];
    }

    /* 填充从站参数区基地址数据表 */
    for(i = 0; i < usTotalSlaveNum; i++)
    {
        IPBUSSetSlavePrmAddr(pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].ucModAddr,
                pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].usParamAreaOffset, ucBufID);
    }
    /* 填充从站参数区长度数据表 */
    for(i = 0; i < usTotalSlaveNum; i++)
    {
        IPBUSSetSlavePrmLen(pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].ucModAddr,
                pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].usParamAreaDataLen, ucBufID);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ConfigFillSlaveEnableTable
*
* 功能描述: 根据配置填写从站使能标志数据表
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
static void ConfigFillSlaveEnableTable(uint8_t ucBufID)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint8_t usDataTemp[32] = {0U};
    uint32_t uiQuotient = 0U;
    uint32_t uiResidue  = 0U;

    pstSysIoModInfo = SharedGetIoModInfoWrit();

    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        /* 填充输入从站使能标志 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]; i++)
        {
            uiQuotient = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[j][i] / UINT8_BITS; /* 商 */
            uiResidue =  pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[j][i] % UINT8_BITS; /* 余数 */
            usDataTemp[uiQuotient] = usDataTemp[uiQuotient] | (uint16_t)BITMASK(uiResidue);
        }
        /* 填充输出从站使能标志 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j]; i++)
        {
            uiQuotient = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[j][i] / UINT8_BITS; /* 商 */
            uiResidue =  pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[j][i] % UINT8_BITS; /* 余数 */
            usDataTemp[uiQuotient] = usDataTemp[uiQuotient] | (uint16_t)BITMASK(uiResidue);
        }

        IPBUSSetSlaveEnableBuf(&usDataTemp[0], ucBufID);
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: ConfigSetMasterPrm
*
* 功能描述: 根据配置填写主站参数
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
static void ConfigSetMasterPrm()
{/*目前的IO配置文件里还没有主卡参数信息*/
    return;
}

/*
***********************************************************************************************************************
* 函数名称: ConfigFillDualPrmArea
*
* 功能描述: 根据配置填充参数区
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
static void ConfigFillDualPrmArea(uint8_t ucBufID)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    pstSysIoModInfo = SharedGetIoModInfoWrit();
    
//    uint8_t ucTest = 0U;
//    if(15 == pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[5].ucModAddr)
//    {
//        printf("PRA:");
//        for(ucTest = 20; ucTest < 30; ucTest++)
//        {
//            printf("%x",pstSysIoModInfo->stSysIoParamArea.ucIoParam[pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[5].usParamAreaOffset + ucTest]);
//        }
//        printf("\n");
//    }

    if(ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
    {
        ipbus_write( WRITE_IPBUS_ARG1, &pstSysIoModInfo->stSysIoParamArea.ucIoParam[0], 0,
                pstSysIoModInfo->stSysIoParamArea.usIoParamIndex);
//        printf("WRITE_IPBUS_ARG1\n");
    }
    if(ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
    {
        ipbus_write( WRITE_IPBUS_ARG2, &pstSysIoModInfo->stSysIoParamArea.ucIoParam[0], 0,
                pstSysIoModInfo->stSysIoParamArea.usIoParamIndex);
//        printf("WRITE_IPBUS_ARG2\n");
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: ConfigDownloadIOConfig
*
* 功能描述: 全下装硬件配置
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
static void ConfigDownloadIOConfig(void)
{
    uint8_t ucWRParaBufID = SharedGetWRParaBufID();
    
    if((IPBUS_SLAVE_ENABLE_BUF0 == ucWRParaBufID) || (IPBUS_SLAVE_ENABLE_BUF1 == ucWRParaBufID))
    {
        /*复位主卡*/
        IPBUSResetMaster();
        udelay(100);
        /*清双口相关数据区*/
        IPBUSDualRamInit();
    
        /*把应用层标志bit0设置未在线*/
        IPBUSSetAPPOnlineFlag(IPBUS_APP_ONLINE);
    
        /*把应用层状态设置为配置状态*/
        IPBUSSetAPPState(APP_STATE_CONFIG);
    
        /*填参数区*/
        ConfigFillDualPrmArea(ucWRParaBufID);
    
        /*填输入/输出区基地址及长度*/
        ConfigFillDualDataTable();
    
        /*填参数区基地址及长度*/
        ConfigFillDualPrmTable(ucWRParaBufID);
    
        /*填写从站使能标志（和使用的参数区一致）*/
        ConfigFillSlaveEnableTable(ucWRParaBufID);
    
        /*填主站参数*/
        ConfigSetMasterPrm();
    
        /*填写应用层标志bit5,注明使用的参数区*/
        IPBUSSetPrmBufID(ucWRParaBufID);
    
        /*填写bit1为应用层完成设备层配置*/
        IPBUSSetAPPConfigDeviceFlag(IPBUS_APP_CONFIG_DEVICE);
    }
    else
    {
        printf("WRParaBufID err%d\n",ucWRParaBufID);
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: ConfigOnlineDownloadIOConfig
*
* 功能描述: 在线下装硬件配置
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
static void ConfigOnlineDownloadIOConfig(void)
{
    uint8_t ucWRParaBufID = SharedGetWRParaBufID();
    
    if((IPBUS_SLAVE_ENABLE_BUF0 == ucWRParaBufID) || (IPBUS_SLAVE_ENABLE_BUF1 == ucWRParaBufID))
    {
        
        /*填参数区*/
        ConfigFillDualPrmArea(ucWRParaBufID);
    
        /*填充从站变化标志*/
        ConfigFillSlaveChange();
    
//        /*填输入/输出区基地址及长度*/
//        ConfigFillDualDataTable();
    
        /*填参数区基地址及长度*/
        ConfigFillDualPrmTable(ucWRParaBufID);
    
        /*填写从站使能标志（和使用的参数区一致）*/
        ConfigFillSlaveEnableTable(ucWRParaBufID);
    
//        /*填主站参数*/
//        ConfigSetMasterPrm();

        /*填写应用层标志bit5,注明使用的参数区*/
        IPBUSSetPrmBufID(ucWRParaBufID);
    
        /*填写bit1为应用层完成设备层配置*/
        IPBUSSetAPPConfigDeviceFlag(IPBUS_APP_CONFIG_DEVICE);
    }
    else
    {
        printf("WRParaBufID err %d\n",ucWRParaBufID);
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ConfigFillSlaveChange
*
* 功能描述: 在线下装时设置从站参数变化标志
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
void ConfigFillSlaveChange()
{
    uint8_t ucSlaveChangeFlg[32] = {0U};
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint16_t usTotalSlaveNum = 0U;
    uint8_t ucModAddr;
    uint8_t ucChangeFlag = false;
    uint8_t ucByteOffset = 0U;
    uint8_t ucBitOffset = 0U;

    pstSysIoModInfo = SharedGetIoModInfoWrit();

    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        usTotalSlaveNum = usTotalSlaveNum + pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]
                          + pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j];
    }

    /* 填充从站参数区基地址数据表 */
    for(i = 0; i < usTotalSlaveNum; i++)
    {
        ucModAddr = pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].ucModAddr;
        
        if((2U == pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].ucChangeFlag) || 
           (0U == pstSysIoModInfo->stSysIoParamArea.stIoParamIndex[i].ucChangeFlag))
        {
            ucChangeFlag = true;
        }
        else
        {
            ucChangeFlag = false;
        }
        
        if(ucModAddr <= MAX_MOD_NUM)
        {
            ucByteOffset = ucModAddr / UINT8_BITS;
            ucBitOffset = ucModAddr % UINT8_BITS;
            if(ucChangeFlag == 0)
            {
                ucChangeFlag = 0x01;
                ucChangeFlag = ucChangeFlag << ucBitOffset;
                ucSlaveChangeFlg[ucByteOffset] = (ucSlaveChangeFlg[ucByteOffset]) & (~ucChangeFlag);
            }
            else
            {
                ucChangeFlag = 0x01;
                ucChangeFlag = ucChangeFlag << ucBitOffset;
                ucSlaveChangeFlg[ucByteOffset] = (ucSlaveChangeFlg[ucByteOffset]) | ucChangeFlag;
            }
        }
    }
    
    IPBUSSetSlavePRMChange(&ucSlaveChangeFlg[0]);
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: ConfigCheckFileCrc
*
* 功能描述: 检查工程各文件CRC是否正确
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
static bool_t ConfigCheckFileCrc(CtrlStationConfigInfo_t* pstCtrlStationInfo)
{
    bool_t bRet = true;
    SysCfgFileType_t emCfgFileType = INVALID_CFG_FILE_TYPE;
    
    if(pstCtrlStationInfo != NULL)
    {
        for(emCfgFileType = CFG_PRG; emCfgFileType < NUM_OF_CFG_FILE_TYPE; emCfgFileType++)
        {
            if(emCfgFileType != CFG_OPC)
            {
                if(pstCtrlStationInfo->stSubFileInfo.uiFileCRC[emCfgFileType] != SharedGetConfigCrc(emCfgFileType))
                {
                    printf("FileCRC ERR %d %x %x\n",emCfgFileType, \
                            pstCtrlStationInfo->stSubFileInfo.uiFileCRC[emCfgFileType],SharedGetConfigCrc(emCfgFileType));
                    bRet = false;
//                    break;
                }
            }
        }
    }
    else
    {
        bRet = false;
    }
    
    return bRet;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
