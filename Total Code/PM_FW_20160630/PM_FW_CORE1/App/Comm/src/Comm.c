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
*  文件名称    : Comm.c
*
*  功能简介    : 完成与CM模块的通讯和上位机数据的处理功能
*
*  作者        : 刘阳
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
#include"../inc/Comm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t ucRTDataBackupCount = 0;

static Service_Handle_Flag_t emCS1131BusyFlg[NUM_OF_CM];
static uint8_t ucCS1131ACKTimeoutCount[NUM_OF_CM];
static File_Handle_Flag_t emFileHandleFlg[NUM_OF_CM];
static Service_Handle_Flag_t emOPCBusyFlg[NUM_OF_CM];
static uint8_t ucOPCACKTimeoutCount[NUM_OF_CM];
static Service_Handle_Flag_t emModbusBusyFlg[NUM_OF_CM];
static uint8_t ucModbusACKTimeoutCount[NUM_OF_CM];
static Service_Handle_Flag_t emP2PBusyFlg[NUM_OF_CM];
static uint8_t ucP2PACKTimeoutCount[NUM_OF_CM];
static FileInfo_t stFileInfo[NUM_OF_CFG_FILE_TYPE];
static P2PSendCtrl_t stP2PSendCtrl[MAX_P2P_BLOCK_NUM];
static uint8_t ucP2PSendIndex = 0;

static uint8_t ucFileName[NUM_OF_CFG_FILE_TYPE][MAX_FILE_NAME_LEN] = {RTS_SC_FILE, RTS_PRG_FILE, RTS_IOC_FILE, \
        RTS_SOE_FILE, RTS_P2P_FILE, RTS_MMC_FILE, RTS_MSC_FILE, RTS_VOT_FILE, RTS_RTN_FILE, RTS_OPC_FILE, RTS_CM_FILE};

static uint8_t ucTmpFileName[NUM_OF_CFG_FILE_TYPE][MAX_FILE_NAME_LEN] = {RTS_SC_FILE_TMP, RTS_PRG_FILE_TMP, \
        RTS_IOC_FILE_TMP, RTS_SOE_FILE_TMP, RTS_P2P_FILE_TMP, RTS_MMC_FILE_TMP, RTS_MSC_FILE_TMP, RTS_VOT_FILE_TMP, \
        RTS_RTN_FILE_TMP, RTS_OPC_FILE_TMP, RTS_CM_FILE_TMP};

static uint8_t ucOldFileName[NUM_OF_CFG_FILE_TYPE][MAX_FILE_NAME_LEN] = {RTS_SC_FILE_OLD, RTS_PRG_FILE_OLD, \
        RTS_IOC_FILE_OLD,RTS_SOE_FILE_OLD, RTS_P2P_FILE_OLD, RTS_MMC_FILE_OLD, RTS_MSC_FILE_OLD, RTS_VOT_FILE_OLD, \
        RTS_RTN_FILE_OLD, RTS_OPC_FILE_OLD, RTS_CM_FILE_OLD};

static ProjectInfo_t stProjectInfo;
static ForceList_t stForceList;




/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void CommCS1131Init(void);

static void CommCS1131Cycle(void);

static uint16_t CommCheckExlibVern (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommGetProjectInfo (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadForceList(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

/*判断文件是否存在*/
static FileOptRest_t CommDetermineFileExist(uint8_t  ucFileName[]);
/*打开文件操作*/
static FileOptRest_t CommOpenFile(uint8_t  ucFileName[],pint32_t piFd);
/*写文件操作*/
static FileOptRest_t CommWriteFile(int32_t iFd,uint8_t  ucFileData[],uint16_t usFileDataLen);
/*关闭文件操作*/
static FileOptRest_t CommCloseFile(int32_t iFd);
/*删除文件操作*/
static FileOptRest_t CommDeleteFile(uint8_t  ucFileName[]);

/*重命名文件操作*/
static FileOptRest_t CommRenameFile(uint8_t  ucOldFileName[], uint8_t  ucNewFileName[]);

static FileInfo_t* CommGetFileInfo(SysCfgFileType_t emCfgFileType);

static uint16_t CommWriteFileStart(uint8_t ucCS1131Data[], uint32_t uiCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileContinue(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileDone(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileStart(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileContinue(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);

static void CommOPCInit(void);

static void CommOPCCycle(void);

static void CommModbusInit(void);

static void CommModbusCycle(void);

static void CommP2PInit(void);

static void CommP2PCycle(void);

static void CommP2PRecv(void);

static void CommP2PSend(void);

//static void CommSOEInit(void);
//
//static void CommSOECycle(void);
//
//static uint16_t CommSOERead(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);
//
//static uint16_t CommSOEDelete(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);

static void CommAMSInit(void);

static void CommAMSCycle(void);

static void CommDiagInit(void);

static void CommDiagCycle(void);

static void CommInterCMDInit(void);

static void CommInterCMDCycle(void);

static void CommTimingInit(void);

static void CommTimingCycle(void);

static void CommClientInit(void);

static void CommClientCycle(void);

static uint32_t SysGetExlibVern(void);

static ProjectInfo_t* SysGetProjectInfo(void);

static ForceList_t* SysGetForceList(void);

static void CommFileInfoInit(void);



/*test*/

static uint16_t CommDownloadStart (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[]);



/*test*/




/*
***********************************************************************************************************************
*                                全局函数实现
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
void CommInit(void)
{
//    uint8_t ucRet = 0;
    
    ucRTDataBackupCount = 0;
    
    CommCS1131Init();
    
    CommOPCInit();
    
    CommModbusInit();
    
    CommP2PInit();
    
//    CommSOEInit();
    
    CommAMSInit();
    
    CommDiagInit();
    
    CommInterCMDInit();
    
    CommInterCMDInit();
    
    CommTimingInit();
    
    CommClientInit();
    
    return;
}



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
void CommCycle(void)
{
    bool_t bRet = false;
    uint32_t uiOffset = 0U;
    uint32_t uiUsedDataLen = 0U;
    Task_ID_t emTaskID = TASK_UP1;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;
    DEBUG_COMM("COMM Cycle Begin !!! \n");
    
//    printf("  CommCycle  \n");
    
    if(HAS_BE_CONFIGURED == SysGetIsConfigured())
    {
        for(emTaskID = TASK_UP1; emTaskID < MAX_TASK_NUM; emTaskID++)
        {
            pstTaskConfigInfo = SharedGetTaskConfigInfo(emTaskID);
            
            if((pstTaskConfigInfo != NULL) && (ACTIVE == pstTaskConfigInfo->emActiveFlag))
            {
                //printf("  @@ Copy RT Data @@\n");
                
                uiOffset = pstTaskConfigInfo->uiGOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiGUsedSize;
                
                
                
                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_G_AREA_LEN)
                {
                    if(TASK_UP1 == emTaskID)
                    {
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP1_G_AREA, uiOffset, uiUsedDataLen);
                    }
                    if(TASK_UP2 == emTaskID)
                    {
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP2_G_AREA, uiOffset, uiUsedDataLen);
                    }
                    
                    if(false == bRet)
                    {
                        
                        //printf("111  false == bRet \n");
                        
                        break;
                    }
                    else
                    {
                        //printf("111  uiOffset is :%d   uiUsedDataLen is :%d \n",uiOffset,uiUsedDataLen);
                    }
                }
                else
                {
                    break;
                }
                
                uiOffset = pstTaskConfigInfo->uiIOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiIUsedSize;
                
                uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN;
                
                ////printf("222  uiOffset is :%d   uiUsedDataLen is :%d \n",uiOffset,uiUsedDataLen);
                
                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_I_AREA_LEN)
                {
                    bRet = SysCopySharedRtDataToCore1RtData(RTDATA_I_AREA, uiOffset, uiUsedDataLen);
                    
                    if(false == bRet)
                    {
                        
                        //printf("222  false == bRet \n");
                        
                        break;
                    }
                    else
                    {
                        //printf("222  uiOffset is :%d   uiUsedDataLen is :%d \n",uiOffset,uiUsedDataLen);
                    }
                }
                else
                {
                    break;
                }
                
                uiOffset = pstTaskConfigInfo->uiOOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiOUsedSize;
                
                uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN - 8*1024;
                
                ////printf("333  uiOffset is :%d   uiUsedDataLen is :%d \n",uiOffset,uiUsedDataLen);
                
                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_Q_AREA_LEN)
                {
                    bRet = SysCopySharedRtDataToCore1RtData(RTDATA_Q_AREA, uiOffset, uiUsedDataLen);
                    
                    if(false == bRet)
                    {
                        
                        //printf("333  false == bRet \n");
                        
                        break;
                    }
                    else
                    {
                        //printf("333  uiOffset is :%d   uiUsedDataLen is :%d \n",uiOffset,uiUsedDataLen);
                    }
                }
                else
                {
                    break;
                }
            }
        }
        if(bRet)
        {
            SysSetCopyRtDataFinished();
            
            //printf(" @@ SysSetCopyRtDataFinished @@ \n");
            
        }
    }
    else
    {
/*test*/
            
        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP1_G_AREA, 0, 100);
        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP2_G_AREA, 0, 100);
        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_I_AREA, 0, 100);
        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_Q_AREA, 0, 100);
        
        if(bRet)
        {
            SysSetCopyRtDataFinished();
            
            //printf(" ## SysSetCopyRtDataFinished ## \n");
            
        }
        
/*test*/
    }


    if((ucRTDataBackupCount % 10) == 0)
    {/*固定周期拷贝一次实时数据，供内部通讯上传CM*/

        
        ucRTDataBackupCount = 1;
    }
    else
    {
        /*1131通讯处理*/
        CommCS1131Cycle();
        /*OPC通讯处理*/
        CommOPCCycle();
        /*Modbus通讯处理*/
        CommModbusCycle();
        /*P2P通讯处理*/
        CommP2PCycle();
        /*SOE通讯处理*/
//      CommSOECycle();
        /*AMS通讯处理*/
        CommAMSCycle();
        /*诊断软件通讯处理*/
        CommDiagCycle();
        /*内部命令通讯处理*/
        CommInterCMDCycle();
        /*校时通讯处理*/
        CommTimingCycle();
        /*客户端软件通讯处理*/
        CommClientCycle();
        ucRTDataBackupCount++;
    }

    DEBUG_COMM("COMM Cycle End !!! \n");
    return;
}
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
void CommClearP2PWaitAckFlg(uint32_t uiP2PBlockNum)
{
    if((uiP2PBlockNum >= 0U) && (uiP2PBlockNum < MAX_P2P_BLOCK_NUM))
    {
        stP2PSendCtrl[uiP2PBlockNum].emWaitACKFlag = NOT_WAIT_ACK;
        stP2PSendCtrl[uiP2PBlockNum].usWaitACKCount = 0U;
    }
    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
********************************函数描述*******************************************************************************
* 函数名称: CommCS1131Init
*
* 功能描述: CS1131初始化函数
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
static void CommCS1131Init(void)
{
    CMController_t emCMID;

    CommFileInfoInit();
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        emCS1131BusyFlg[emCMID] = FREE;
        ucCS1131ACKTimeoutCount[emCMID] = 0U;
        emFileHandleFlg[emCMID] = FILE_HANDLE_FREE;
    }

    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommCS1131Cycle
*
* 功能描述: CS1131数据处理函数
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
static void CommCS1131Cycle(void)
{
    CMController_t emCMIndex = CM1;                     /*CM 索引*/
    uint16_t usBuffIndex = 0U;                          /*数组内地址索引*/
    uint8_t ucACKData[1024];
    static MSGHead_t stMSGHead;                         /*CS1131消息头部*/
    uint16_t CS1131DataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* CS1131数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* CS1131数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上CS1131服务信息         */
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */


    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */

    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        memset(ucACKData, 0x00,1024);

        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,CS1131_SRV);
//        DEBUG_COMM("------emCS1131BusyFlg %d is : %d .\n", emCMIndex,emCS1131BusyFlg[emCMIndex]);

        if((BUSY == emCS1131BusyFlg[emCMIndex] ) && (FILE_HANDLE_FREE == emFileHandleFlg[emCMIndex]))
        {/*当服务处理标志为busy时，且不是在处理文件操作时，等待来自core0的应答数据*/

            /*从共享内存上获取应答数据*/
            SysGetSMMsg(SM_CS1131_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);

            if(usACKDataLen > 0)
            {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                ucCS1131ACKTimeoutCount[emCMIndex] = 0;
                emCS1131BusyFlg[emCMIndex] = FREE;
            }
            else
            {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
                ucCS1131ACKTimeoutCount[emCMIndex]++;
                if(CS1131_ACK_TIMEOUT == ucCS1131ACKTimeoutCount[emCMIndex])
                {
                    
                    ucCS1131ACKTimeoutCount[emCMIndex] = 0;
                    emCS1131BusyFlg[emCMIndex] = FREE;
                }
            }
        }
        else if((BUSY == emCS1131BusyFlg[emCMIndex] ) && (FILE_HANDLE_BUSY == emFileHandleFlg[emCMIndex]))
        {/*当前处理文件操作*/

            switch(stMSGHead.usActualType)
            {/*根据服务类型，进入对应服务处理函数继续处理文件操作*/
               case CS1131_WRITEFILE_START:
               {
                   /*process 1131 write file start*/
                   DEBUG_COMM("  CommWriteFileStart  \n");
                    usACKDataLen = CommWriteFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength, \
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;

               case CS1131_WRITEFILE_CONT:
               {
                   /*process 1131 write file continue*/
                   DEBUG_COMM("  CommWriteFileContinue  \n");
                    usACKDataLen = CommWriteFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength,\
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;

               case CS1131_WRITEFILE_END:
               {
                   /*process 1131 write file end*/
                   DEBUG_COMM("  CommWriteFileDone  \n");
                    usACKDataLen = CommWriteFileDone(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength,\
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;

               default:
               break;

            }
        }
        else
        {/*BusyFlg is Free*/
            /*从系统资源获取来自CM的服务数据*/

            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

            SysGetMsg(emCMIndex,CS1131_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&CS1131DataLen);
            DEBUG_COMM("-----------CS1131DataLen  =  %d  \n",CS1131DataLen);
            if(CS1131DataLen > 0)
             {/*数据长度大于0，表示收到数据*/
            	DEBUG_COMM("---SysGetMsg  CS1131DataLen is:  %d  \n",CS1131DataLen);
                 

                 /*获取消息头部*/
                 memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                 memcpy(&stMSGHead, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], sizeof(MSGHead_t));
                 /*记录服务数据相关信息*/
                 pstSysSrvInfo->stSysSrvData.uiSrvLen = CS1131DataLen;
                 pstSysSrvInfo->stSysSrvData.ucSrvNo = stMSGHead.usActualType;

                 uiCalCrc = SysCrc32Cal(0x00, (pchar_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                         stMSGHead.usLength);
                 usBuffIndex = stMSGHead.usLength;
                 memcpy((puint8_t)&uiReadCrc,(puint8_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex],CHKSUM_CRC_SIZE);

                      
                 if(uiCalCrc != uiReadCrc)
                 {/*CRC 不一致*/
                     /*log*/
                     DEBUG_COMM("  uiCalCrc != uiReadCrc  \n");
                 }
                 else if(CS1131DataLen != stMSGHead.usLength + CHKSUM_CRC_SIZE)
                 {/*数据长度不一致*/
                     /*log*/
                     DEBUG_COMM("  DataLen is error.  \n");
                 }
                 else
                 {
                	 emCS1131BusyFlg[emCMIndex] = BUSY;
                     DEBUG_COMM("  stMSGHead.usActualType is : %d \n",stMSGHead.usActualType);
                     switch(stMSGHead.usActualType)
                     {/*根据服务号，处理1131服务*/
                         /*these service need to synchronize*/
                        case CS1131_START_RUN:
                        case CS1131_STOP_RUN:
                        case CS1131_PAUSE_RUN:
                        case CS1131_WRITE_VAR:
//                      case CS1131_DOWNLOAD_START:
                        case CS1131_SINGLE_STEP:
                        case CS1131_FORCE_VAR:
                        {/*将服务进行同步*/
                            stSyncTask.emTaskId = SYS_BROADCAST_CS1131SRV;
                            stSyncTask.emPMId = INVALID_CM_ID;
                            stSyncTask.emTaskStatus = TASK_SEND;
                            stSyncTask.emTaskWait = TASK_NOWAIT;
                            pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                            //if(SYS_STABLE_STATE == SysGetState())
                            //{/* 非稳态时按错误处理，不添加同步任务 */
                            //    SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                            //}

                            /*暂定为：非稳态时也可以处理服务。*/
                            SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                        }

                        break;
                        
/*test */
                        case CS1131_DOWNLOAD_START:
                        {
                            /*process 1131 CS1131_DOWNLOAD_START*/
                             usACKDataLen = CommDownloadStart (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                            
                        break;
/*test */

                        case CS1131_CHECK_EXTERLIB:
                        {
                            /*process 1131 check exlibvern*/
                             usACKDataLen = CommCheckExlibVern (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READ_PRJINFO:
                        {
                            /*process 1131 read ProjectInfo*/
                             usACKDataLen = CommGetProjectInfo(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READ_FORCE:
                        {
                            /*process 1131 read Forcelist*/
                             usACKDataLen = CommReadForceList(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_DOWNLOAD_COMFIR:
                        {
                            /*process 1131 Download end service*/
                            usACKDataLen = CommDownloadComfirm(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                    stMSGHead.usLength,ucACKData);
                        }

                        break;

                        case CS1131_WRITEFILE_START:
                        {
                            /*process 1131 write file start*/
                             usACKDataLen = CommWriteFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_CONT:
                        {
                            /*process 1131 write file continue*/
                             usACKDataLen = CommWriteFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_END:
                        {
                            /*process 1131 write file end*/
                             usACKDataLen = CommWriteFileDone(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_READFILE_START:
                        {
                            /*process 1131 read file start*/
                             usACKDataLen = CommReadFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READFILE_CONT:
                        {
                            /*process 1131 read file continue*/
                             usACKDataLen = CommReadFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        default:
                        break;
                     }
                 }
             }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            DEBUG_COMM("  usACKDataLen > 0 Has RESP Data \n");
//            for(uint8_t x = 0U; x < usACKDataLen; x++)
//            {
//                printf(" %x ",ucACKData[x]);
//                
//            }
//            printf("\n");
            SysSetMsg(emCMIndex,CS1131_RESP,ucACKData,usACKDataLen);
            if(emFileHandleFlg[emCMIndex] != FILE_HANDLE_FREE)
            {
                emFileHandleFlg[emCMIndex] = FILE_HANDLE_FREE;
            }
            
            emCS1131BusyFlg[emCMIndex] = FREE;
            usACKDataLen = 0U;
        }

    }
    return;
}


/*
********************************函数描述*******************************************************************************
* 函数名称: CommCheckExlibVern
*
* 功能描述: 获取外部库版本函数
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
static uint16_t CommCheckExlibVern (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint32_t uiLocalExlibVern = SysGetExlibVern();              /*获取外部库版本，接口待定*/
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    uint32_t uiReadExlibVern = 0;                               /*1131下发的外部库版本号*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else if(usCS1131DataLen != sizeof(uiLocalExlibVern))
    {/*服务数据长度错误*/
        ucSubSeason = ERR_MSG_LEN;
    }
    else
    {/*拷贝1131下发的外部库版本号，并与本地版本号比较*/
        memcpy((puint8_t)&uiReadExlibVern,ucCS1131Data,sizeof(uiReadExlibVern));
        if(uiReadExlibVern != uiLocalExlibVern)
        {
            ucSubSeason = ERR_MSG_CRC;
        }
    }

    /*produce ACK data*/
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    uiACKDataIndex += sizeof(ACKHead_t);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {/*服务处理成功*/
        pstACKHead->ucResCode = RESCODE_SUCCESS;
    }
    pstACKHead->ucSubReason = ucSubSeason;

    pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
    
    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
    memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
    uiACKDataIndex += sizeof(uiCalcCRC);
    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
    return usACKDataLen;
}


/*
********************************函数描述*******************************************************************************
* 函数名称: CommGetProjectInfo
*
* 功能描述: 获取工程版本函数
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
static uint16_t CommGetProjectInfo (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    ProjectInfo_t* pstProjectInfo;                              /*工程信息结构*/
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {/*获取工程信息*/
        pstProjectInfo = SysGetProjectInfo();/*this function is not defined*/
        if(pstProjectInfo == NULL)
        {
            ucSubSeason = ERR_GET_PROINFO;
        }
    }

    /*produce ACK data*/
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    uiACKDataIndex +=  sizeof(ACKHead_t);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {/*服务处理成功*/
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)pstProjectInfo, sizeof(ProjectInfo_t));
        uiACKDataIndex += sizeof(ProjectInfo_t);

        pstACKHead->ucDataFlg = 1U;
    }
    pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
    pstACKHead->ucSubReason = ucSubSeason;
    
    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
    memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
    uiACKDataIndex += sizeof(uiCalcCRC);
    
    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
    return usACKDataLen;

}
/*
********************************函数描述*******************************************************************************
* 函数名称: CommReadForceList
*
* 功能描述: 获取强制列表函数
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
static uint16_t CommReadForceList(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    ForceList_t* pstForceList;                                  /*强制列表结构指针*/
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;

    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {/*获取强制列表结构指针*/
        pstForceList = SysGetForceList();//this function is not defined
        if(pstForceList == NULL)
        {
            ucSubSeason = ERR_GET_FORCELIST;
            /**/
        }
    }

    /*produce ACK data*/
    DEBUG_COMM(" produce ACK data \n");
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    uiACKDataIndex += sizeof(ACKHead_t);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {/*服务处理成功*/
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)pstForceList, sizeof(ForceList_t));
        uiACKDataIndex += sizeof(ForceList_t);

        pstACKHead->ucDataFlg = 1U;
    }
    pstACKHead->ucSubReason = ucSubSeason;
    pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
    
    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
    memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
    uiACKDataIndex += sizeof(uiCalcCRC);
    
    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
    return usACKDataLen;
}
/*
********************************函数描述*******************************************************************************
* 函数名称: CommDownloadComfirm
*
* 功能描述: 下装确认处理函数
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
static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }

    else
    {/*设置本地下装文件被确认*/

        //SysSetPrjComfirm();

        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex += sizeof(ACKHead_t);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);

        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(uiACKDataIndex);
    }

    return usACKDataLen;
}
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
static FileOptRest_t CommDetermineFileExist(uint8_t  ucFileName[])
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
       
    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送判断文件是否存在命令*/
            ucTimeCount = 0;
            SysSendIsFileExist(ucFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答，若收到应答，解析应答结果，并返回；若超时，返回超时*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_IS_FILE_EXIST_ACK == stAckHeader.uiStatusWord) && (FM_FILE_EXIST_TRUE == stAckHeader.uiErrCode))
                {/*操作成功，且文件存在；返回结果*/
                    emFileOptRest = FILE_EXIST;
                    emProcessStag = FOS_START;
                }
                else if((FM_IS_FILE_EXIST_ACK == stAckHeader.uiStatusWord) && \
                        (FM_FILE_EXIST_FALSE == stAckHeader.uiErrCode))
                {/*操作成功，且文件不存在；返回结果*/
                    emFileOptRest = FILE_NOT_EXIST;
                    emProcessStag = FOS_START;
                }
                else
                {/*操作失败；返回失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*超时处理；返回失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}
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
static FileOptRest_t CommOpenFile(uint8_t ucFileName[],pint32_t piFd)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送打开文件命令*/
            ucTimeCount = 0;
            SysOpenFile(ucFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_OPEN_FILE_ACK == stAckHeader.uiStatusWord))
                {/*打开文件成功，返回操作成功，并返回文件描述符*/
                    emFileOptRest = SUCCESS;
                    *piFd = stAckHeader.iFd;
                    emProcessStag = FOS_START;
                }
                else
                {/*打开文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}
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
static FileOptRest_t CommWriteFile(int32_t iFd,uint8_t ucFileData[],uint16_t usFileDataLen)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送写文件命令*/
            ucTimeCount = 0;
            SysWriteFile(iFd, ucFileData, usFileDataLen);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_WRITE_FILE_ACK == stAckHeader.uiStatusWord))
                {/*写文件成功，返回操作成功，并返回文件描述符*/
                    emFileOptRest = SUCCESS;
                    emProcessStag = FOS_START;
                }
                else
                {/*写文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}

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
static FileOptRest_t CommCloseFile(int32_t iFd)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送关闭文件命令*/
            ucTimeCount = 0;
            SysCloseFile(iFd);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_CLOSE_FILE_ACK == stAckHeader.uiStatusWord))
                {/*关闭文件成功，返回操作成功，并返回文件描述符*/
                    emFileOptRest = SUCCESS;
                    emProcessStag = FOS_START;
                }
                else
                {/*关闭文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}


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
static FileOptRest_t CommDeleteFile(uint8_t ucFileName[])
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送删除文件命令*/
            ucTimeCount = 0;
            SysDeleteFile(ucFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_DELETE_FILE_ACK == stAckHeader.uiStatusWord))
                {/*删除文件成功，返回操作成功*/
                    emFileOptRest = SUCCESS;
                    emProcessStag = FOS_START;
                }
                else
                {/*删除文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}

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
static FileOptRest_t CommRenameFile(uint8_t ucOldFileName[], uint8_t ucNewFileName[])
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送重命名文件命令*/
            ucTimeCount = 0;
            SysRenameFile(ucOldFileName, ucNewFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if((FM_RENAME_FILE_ACK == stAckHeader.uiStatusWord))
                {/*重命名文件成功，返回操作成功*/
                    emFileOptRest = SUCCESS;
                    emProcessStag = FOS_START;
                }
                else
                {/*重命名文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}




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
static FileInfo_t* CommGetFileInfo(SysCfgFileType_t emCfgFileType)
{
    FileInfo_t* pstFileInfo = NULL;

    if( emCfgFileType < NUM_OF_CFG_FILE_TYPE )
    {
        pstFileInfo = (FileInfo_t*)&stFileInfo[emCfgFileType];
    }
    
    return pstFileInfo;
}




/*
********************************函数描述*******************************************************************************
* 函数名称: CommWriteFileStart
*
* 功能描述: 写文件开始服务处理函数
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
static uint16_t CommWriteFileStart(uint8_t ucCS1131Data[], uint32_t uiCS1131DataLen,uint8_t ucACKData[])
{
    static FileWriteStag_t emProcessStag = FWRITE_START;                    /*写文件开始服务处理流程*/
    FileWriteInfo_t stFileWriteInfo;                                        /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    uint32_t uiFileConfigFlg = 0U;
    static SysCfgFileType_t emCfgFileType = 0U;
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_END;
    }
    if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_END;
    }
    switch(emProcessStag)
    {
        case FWRITE_START:
        {/*写文件开始服务处理开始阶段，将文件数据存入内存上*/
            /*初始化*/
            memset((puint8_t)&stFileWriteInfo,0x00,sizeof(stFileWriteInfo));
            ucSubSeason = ERR_SUCCESS;
            
            /*获取消息头部信息*/
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            usBuffIndex += sizeof(MSGHead_t);
            /*获取文件信息*/
            memcpy(&stFileWriteInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileType));
            
 //           DEBUG_COMM("----CommWriteFileStart: stFileWriteInfo.ucFileType is : %x \n",stFileWriteInfo.ucFileType);
            
            usBuffIndex += sizeof(stFileWriteInfo.ucFileType);
            
            memcpy(&stFileWriteInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileID));
            
            DEBUG_COMM("----CommWriteFileStart: stFileWriteInfo.ucFileID is : %x \n",stFileWriteInfo.ucFileID);
            
            usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
            
            if((stFileWriteInfo.ucFileID == 0U) && (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_COMM("----CommWriteFileDone: FIleID is error : %d \n",stFileWriteInfo.ucFileID);
            }
            else
            {
                /*将1131下发的文件ID，转化为PM使用的文件类型*/
                emCfgFileType = stFileWriteInfo.ucFileID - 1U;
                
                
                memcpy(&stFileWriteInfo.usFileDataLen, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.usFileDataLen));
                
                DEBUG_COMM("----CommWriteFileStart: stFileWriteInfo.usFileDataLen is : %x \n",stFileWriteInfo.usFileDataLen);
                
                usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);
                
                /*根据文件名字，获取文件的控制结构*/
                pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                                
                DEBUG_COMM("----CommWriteFileContinue: emCfgFileType is : %x \n",emCfgFileType);
                
                
                if((stFileWriteInfo.usFileDataLen > MAX_WRITE_FILE_LEN) && (NULL == pstFileInfo))
                {
                    /*file name is error ,file info is not exist*/
                    ucSubSeason = ERR_WRITE_FILE_START;
                    emProcessStag = FWRITE_END;
                }
                else
                {/*save the data in memory*/
                    
                    DEBUG_COMM("save the data in memory \n");
                    
                    /*获取内存上文件信息*/
                    SysGetCfgFileState(emCfgFileType, &stCfgFileState);
    
                    if(stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE)
                    {/*文件不可写*/
                        /*log*/
//                        emProcessStag = FWRITE_START;
    
                        ucSubSeason = ERR_WRITE_FILE_START;
    
                        emProcessStag = FWRITE_END;
                        
                        DEBUG_COMM("pstCfgFileState->bWriteable != CFG_FILE_WR_ENABLE \n");
//                        DEBUG_COMM("pstCfgFileState->bWriteable = %d  \n",stCfgFileState.bWriteable);
//                        DEBUG_COMM("pstCfgFileState->bReadable = %d  \n",stCfgFileState.bReadable);
//                        
//                        DEBUG_COMM("pstCfgFileState->bFileStatus = %d  \n",stCfgFileState.bFileStatus);
//                        DEBUG_COMM("pstCfgFileState->uiLen = %d  \n",stCfgFileState.uiLen);
//                        DEBUG_COMM("pstCfgFileState->uiWrOffset = %d  \n",stCfgFileState.uiWrOffset);
//                        
//                        DEBUG_COMM("s_emWriteCfgFileType  is  : %d  \n",s_emWriteCfgFileType);
//                        DEBUG_COMM("NUM_OF_CFG_FILE_TYPE  is  :  %x  \n",NUM_OF_CFG_FILE_TYPE);
                        
                    }
                    else
                    {/*将文件数据写入内存*/
                        /*写入一字节文件数据未经解析配置的标志，供配置模块使用*/
                        uiFileConfigFlg = FILE_NOT_CONFIG;
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &uiFileConfigFlg, \
                                sizeof(uiFileConfigFlg));
                        if(usWritDataLen != sizeof(uiFileConfigFlg))
                        {
                            ucSubSeason = ERR_WRITE_FILE_START;
                            emProcessStag = FWRITE_END;
                        }
                        else
                        {
                            
//                            printf("usFileDataLen is : %d \n",stFileWriteInfo.usFileDataLen);
                            
                            usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                    stFileWriteInfo.usFileDataLen);
                            if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                            {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                                ucSubSeason = ERR_WRITE_FILE_START;
                                emProcessStag = FWRITE_END;
                                
                                DEBUG_COMM("usWritDataLen != stFileWriteInfo.usFileDataLen \n  usWritDataLen is : %d \n stFileWriteInfo.usFileDataLen is : %d \n", \
                                        usWritDataLen,stFileWriteInfo.usFileDataLen);
                                
                                
                            }
                            else
                            {/*写入成功*/
/*test*/
                                //emProcessStag = FWRITE_DETER_TMPFILE_EXIST;
                                /*调试，暂时不写入flash，写入内存后，直接应答*/
                                emProcessStag = FWRITE_END;
                                DEBUG_COMM("save the data in memory successful\n");
                                
/*test*/
                            }
                        }
    
                    }
                }
            }
        }
        break;

        case FWRITE_DETER_TMPFILE_EXIST:
        {/*完成判断文件是否存在操作*/
            ucFileOptRst = CommDetermineFileExist(pstFileInfo->ucTmpFileName);
            if(FILE_EXIST == ucFileOptRst)
            {/*临时文件存在，将其删除*/
                emProcessStag = FWRITE_ERASE_TMPFILE;
            }
            if(FILE_NOT_EXIST == ucFileOptRst)
            {/*临时文件不存在，将其以创建方式打开*/
                emProcessStag = FWRITE_OPEN_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败,应答错误信息*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_ERASE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(pstFileInfo->ucTmpFileName);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_OPEN_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，并应答错误信息*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_OPEN_TMPFILE:
        {
            ucFileOptRst = CommOpenFile(pstFileInfo->ucTmpFileName, &pstFileInfo->iFid);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_OPEN_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case  FWRITE_WRITE_TMPFILE:
        {
            ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucCS1131Data[usBuffIndex], stFileWriteInfo.usFileDataLen);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_END;
                ucSubSeason = ERR_SUCCESS;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，关闭文件，等待删除，并应答错误信息*/
                emProcessStag = FWRITE_CLOSE_TMPFILE;
            }
        }
        break;

        case FWRITE_CLOSE_TMPFILE:
        {
            ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
            if(FAIL == ucFileOptRst)
            {/*操作失败，删除文件*/
                /*record error log*/
            }
            emProcessStag = FWRITE_DELETE_TMPFILE;
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(pstFileInfo->ucTmpFileName);
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                /*record error log*/
            }
            ucSubSeason = ERR_WRITE_FILE_START;
            emProcessStag = FWRITE_END;
        }
        break;

        case FWRITE_END:
        {/*服务处理结束，长生应答*/
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            pstACKHead->ucType = pstMSGHead->ucProtect;
            pstACKHead->ucProtect = pstMSGHead->ucType;
            pstACKHead->usActualType = pstMSGHead->usActualType;
            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
            pstACKHead->ucDataFlg = 0U;
            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
            /*清除数组索引，供应答使用*/
            usBuffIndex = 0U;
            usBuffIndex += sizeof(ACKHead_t);
            if(ucSubSeason != ERR_SUCCESS)
            {/*服务处理失败*/
                pstACKHead->ucResCode = RESCODE_FAILED;
                
                /*下装文件失败，将正在写入内存的文件删除，将顺序记录文件ID的全局量初始化*/
                SysDeleteCfgFile(emCfgFileType);
            }
            else
            {/*服务处理成功*/
                pstACKHead->ucResCode = RESCODE_SUCCESS;
            }
            
            DEBUG_COMM("ACK:  ucSubSeason = %d \n",ucSubSeason);
            DEBUG_COMM("ACK:  pstACKHead->ucResCode = %d \n",pstACKHead->ucResCode);
            
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            pstACKHead->ucSubReason = ucSubSeason;
            /*计算并填充数据应答的CRC*/
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(usBuffIndex);
            emProcessStag = FWRITE_START;
        }
        break;

        default:
        {/*其他异常情况，错误应答*/
            /*record error log*/
            ucSubSeason = ERR_WRITE_FILE_START;
            emProcessStag = FWRITE_END;
        }
        break;
    }
    
    return usACKDataLen;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommWriteFileContinue
*
* 功能描述: 写文件继续服务处理函数
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
static uint16_t CommWriteFileContinue(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    static FileWriteStag_t emProcessStag = FWRITE_START;                    /*写文件开始服务处理流程*/
    FileWriteInfo_t stFileWriteInfo;                                        /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    static SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_END;
    }
    if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_END;

    }
    
    switch(emProcessStag)
    {/**/
        case FWRITE_START:
        {/*写文件开始服务处理开始阶段，将文件数据存入内存上*/
            /*初始化*/
            memset((puint8_t)&stFileWriteInfo,0x00,sizeof(stFileWriteInfo));
            ucSubSeason = ERR_SUCCESS;
            
            /*获取消息头部信息*/
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            usBuffIndex += sizeof(MSGHead_t);
            /*获取文件信息*/
            memcpy(&stFileWriteInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileType));
            usBuffIndex += sizeof(stFileWriteInfo.ucFileType);
            
//            DEBUG_COMM("----CommWriteFileContinue: stFileWriteInfo.ucFileType is : %x \n",stFileWriteInfo.ucFileType);
            
            memcpy(&stFileWriteInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileID));
            usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
            
            if((stFileWriteInfo.ucFileID == 0U) && (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_COMM("----CommWriteFileDone: FIleID is error : %d \n",stFileWriteInfo.ucFileID);
            }
            else
            {
                /*将1131下发的文件ID，转化为PM使用的文件类型*/
                emCfgFileType = stFileWriteInfo.ucFileID - 1U;
                
                DEBUG_COMM("----CommWriteFileContinue: stFileWriteInfo.ucFileID is : %x \n",stFileWriteInfo.ucFileID);
                
                
                memcpy(&stFileWriteInfo.usFileDataLen, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.usFileDataLen));
                usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);
                
                DEBUG_COMM("----CommWriteFileContinue: stFileWriteInfo.usFileDataLen is : %x \n",stFileWriteInfo.usFileDataLen);
                
    
                /*根据文件名字，获取文件的控制结构*/
                pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                
                DEBUG_COMM("----CommWriteFileContinue: emCfgFileType is : %x \n",emCfgFileType);
                
                
                if((stFileWriteInfo.usFileDataLen > MAX_WRITE_FILE_LEN) && (NULL == pstFileInfo))
                {
                    /*file data len is error ,file info is not exist*/
                    ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                    emProcessStag = FWRITE_END;
                    
                    DEBUG_COMM("NULL == pstFileInfo");
                    
                }
                else
                {/*save the data in memory*/
                    /*获取内存上文件信息*/
                    SysGetCfgFileState(emCfgFileType, &stCfgFileState);
                    
                    if(stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE)
                    {/*文件不可写*/
                        /*log*/
//                        emProcessStag = FWRITE_START;
                        
                        ucSubSeason = ERR_WRITE_FILE_START;
                        emProcessStag = FWRITE_END;
                        
                        DEBUG_COMM("stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE  \n");
    
                    }
                    else
                    {/*将文件数据写入内存*/
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                stFileWriteInfo.usFileDataLen);
                        if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                        {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                            emProcessStag = FWRITE_END;
                            
                            DEBUG_COMM("usWritDataLen != stFileWriteInfo.usFileDataLen \n  usWritDataLen is : %d \n stFileWriteInfo.usFileDataLen is : %d \n", \
                                    usWritDataLen,stFileWriteInfo.usFileDataLen);
                            
                        }
                        else
                        {/*写入成功*/
/*test*/
                            //emProcessStag = FWRITE_DETER_TMPFILE_EXIST;
                            /*调试，暂时不写入flash，写入内存后，直接应答*/
                            emProcessStag = FWRITE_END;
                            DEBUG_COMM("----CommWriteFileContinue: save the data in memory successful\n");
                            
/*test*/
                        }
                    }
    
                }
            }
        }
        break;

        case  FWRITE_WRITE_TMPFILE:
        {
            ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucCS1131Data[usBuffIndex], stFileWriteInfo.usFileDataLen);
            if(SUCCESS == ucFileOptRst)
            {/*文件操作成功，进行下一步*/
                emProcessStag = FWRITE_END;
                ucSubSeason = ERR_SUCCESS;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，关闭文件，等待删除，并应答错误信息*/
                emProcessStag = FWRITE_CLOSE_TMPFILE;
            }
        }
        break;

        case FWRITE_CLOSE_TMPFILE:
        {
            ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
            if(FAIL == ucFileOptRst)
            {/*操作失败，删除文件*/
                /*record error log*/
            }
            emProcessStag = FWRITE_DELETE_TMPFILE;
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(pstFileInfo->ucTmpFileName);
            if(FAIL == ucFileOptRst)
            {/*操作失败，应答错误信息*/
                /*record error log*/
            }
            ucSubSeason = ERR_WRITE_FILE_START;
            emProcessStag = FWRITE_END;
        }
        break;

        case FWRITE_END:
        {
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            pstACKHead->ucType = pstMSGHead->ucProtect;
            pstACKHead->ucProtect = pstMSGHead->ucType;
            pstACKHead->usActualType = pstMSGHead->usActualType;
            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
            pstACKHead->ucDataFlg = 0U;
            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
            /*清除数组索引，供应答使用*/
            usBuffIndex = 0U;
            usBuffIndex += sizeof(ACKHead_t);
            if(ucSubSeason != ERR_SUCCESS)
            {/*服务处理失败*/
                pstACKHead->ucResCode = RESCODE_FAILED;
                
                /*下装文件失败，将正在写入内存的文件删除，将顺序记录文件ID的全局量初始化*/
                SysDeleteCfgFile(emCfgFileType);
                
            }
            else
            {/*服务处理成功*/
                pstACKHead->ucResCode = RESCODE_SUCCESS;
            }
            
            DEBUG_COMM("ACK:  ucSubSeason = %d \n",ucSubSeason);
            DEBUG_COMM("ACK:  pstACKHead->ucResCode = %d \n",pstACKHead->ucResCode);
            
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            pstACKHead->ucSubReason = ucSubSeason;
            /*计算并填充数据应答的CRC*/
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(usBuffIndex);
            emProcessStag = FWRITE_START;
        }
        break;

        default:
        {/*其他异常情况，错误应答*/
            /*record error log*/
            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
            emProcessStag = FWRITE_END;
        }
        break;
    }
    
    return usACKDataLen;
}
/*
********************************函数描述*******************************************************************************
* 函数名称: CommWriteFileDone
*
* 功能描述: 写文件完成服务处理函数
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
static uint16_t CommWriteFileDone(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    static FileWriteStag_t emProcessStag = FWRITE_START;                    /*写文件开始服务处理流程*/
    FileWriteInfo_t stFileWriteInfo;                                        /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    //SysCfgFileState_t *pstCfgFileState = NULL;                            /*内存上存储文件的相关信息*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    static SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_END;
    }
    if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_END;
    }
    
    switch(emProcessStag)
    {
        case FWRITE_START:
        {/*写文件开始服务处理开始阶段，将文件数据存入内存上*/
            /*初始化*/
            memset((puint8_t)&stFileWriteInfo,0x00,sizeof(stFileWriteInfo));
            ucSubSeason = ERR_SUCCESS;
            
            /*获取消息头部信息*/
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            usBuffIndex += sizeof(MSGHead_t);
            /*获取文件信息*/
            memcpy(&stFileWriteInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileType));
            usBuffIndex += sizeof(stFileWriteInfo.ucFileType);
            
//            DEBUG_COMM("----CommWriteFileDone: stFileWriteInfo.ucFileType is : %x \n",stFileWriteInfo.ucFileType);
            
            memcpy(&stFileWriteInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.ucFileID));
            usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
            
            if((stFileWriteInfo.ucFileID == 0U) && (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
            {
                ucSubSeason = ERR_WRITE_FILE_END;
                emProcessStag = FWRITE_END;
                
                DEBUG_COMM("----CommWriteFileDone: FIleID is error : %d \n",stFileWriteInfo.ucFileID);
            }
            else
            {
                /*将1131下发的文件ID，转化为PM使用的文件类型*/
                emCfgFileType = stFileWriteInfo.ucFileID - 1U;
                
                DEBUG_COMM("----CommWriteFileDone: stFileWriteInfo.ucFileID is : %x \n",stFileWriteInfo.ucFileID);
                
                stFileWriteInfo.usFileDataLen = 0U;

                /*根据文件名字，获取文件的控制结构*/
                pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                
                DEBUG_COMM("----CommWriteFileDone: emCfgFileType is : %x \n",emCfgFileType);
                
                if(NULL == pstFileInfo)
                {
                    /*file name is error ,file info is not exist*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                    
                    DEBUG_COMM("NULL == pstFileInfo \n");
                    
                }
                else
                {/*save the data in memory*/
                    /*获取内存上文件信息*/
                    SysGetCfgFileState(emCfgFileType, &stCfgFileState);

                    if(stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE)
                    {/*文件不可写*/
                        /*log*/
    //                        emProcessStag = FWRITE_START;
                        
                        ucSubSeason = ERR_WRITE_FILE_START;
                        emProcessStag = FWRITE_END;
                        
                        DEBUG_COMM("stCfgFileState.bWriteable != CFG_FILE_WR_ENABLE \n");
                        
                    }
                    else
                    {/*将文件数据写入内存*/
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                stFileWriteInfo.usFileDataLen);
                        if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                        {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                            ucSubSeason = ERR_WRITE_FILE_END;
                            emProcessStag = FWRITE_END;
                            
                            DEBUG_COMM("usWritDataLen != stFileWriteInfo.usFileDataLen \n  usWritDataLen is : %d \n stFileWriteInfo.usFileDataLen is : %d \n", \
                                    usWritDataLen,stFileWriteInfo.usFileDataLen);
                            
                        }
                        else
                        {/*写入成功*/
                            /*设置写文件结束*/
                            SysSetCfgFileFinished(emCfgFileType);
                            /*写文件类型记录向后移一个,到达最后一个文件时，移回第一个文件*/

                            
                           
/*test*/
                            //emProcessStag = FWRITE_DETER_TMPFILE_EXIST;
                            /*调试，暂时不写入flash，写入内存后，直接应答*/
                            emProcessStag = FWRITE_END;
                            DEBUG_COMM("----CommWriteFileDone: save the data in memory successful\n");
                            
/*test*/
                        }
                    }
                }
            }

        }
        break;

        case  FWRITE_WRITE_TMPFILEDONE:
        {
            ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
            if(SUCCESS == ucFileOptRst)
            {/*操作成功，进行下一步操作*/
                emProcessStag = FWRITE_DETER_FORMFILE_EXIST;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败，删除文件*/
                /*record error log*/
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
        }
        break;

        case FWRITE_DETER_FORMFILE_EXIST:
        {
            ucFileOptRst = CommDetermineFileExist(pstFileInfo->ucFileName);
            if(FILE_EXIST == ucFileOptRst)
            {/*文件存在，重命名文件*/
                emProcessStag = FWRITE_RENAME_FORMFILE;
            }
            if(FILE_NOT_EXIST == ucFileOptRst)
            {/*文件不存在，重命名临时文件*/
                emProcessStag = FWRITE_RENAME_TMPFILE;
            }
            if(FAIL == ucFileOptRst)
            {/*操作失败,应答错误信息*/
                ucSubSeason = ERR_WRITE_FILE_START;
                emProcessStag = FWRITE_END;
            }
        }
        break;

        case FWRITE_RENAME_FORMFILE:
        {
            ucFileOptRst = CommRenameFile(pstFileInfo->ucFileName, pstFileInfo->ucOldFileName);
            if(FAIL == ucFileOptRst)
            {/*重命名正式文件为旧文件失败，删除文件*/
                /*record error log*/
                emProcessStag = FWRITE_DELETE_FORMFILE;
            }
            if(SUCCESS == ucFileOptRst)
            {/*重命名正式文件为旧文件成功，将临时文件重命名为正式文件*/
                emProcessStag = FWRITE_RENAME_TMPFILE;
            }
        }
        break;

        case FWRITE_RENAME_TMPFILE:
        {
            ucFileOptRst = CommRenameFile(pstFileInfo->ucTmpFileName, pstFileInfo->ucFileName);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
                emProcessStag = FWRITE_DELETE_TMPFILE;
            }
            if(SUCCESS == ucFileOptRst)
            {
                ucSubSeason = ERR_SUCCESS;
                pstFileInfo->uiFileDataLen = pstFileInfo->uiFileWriteIndex;
                emProcessStag = FWRITE_RENAME_TMPFILE;
            }
        }
        break;

        case FWRITE_DELETE_TMPFILE:
        {
            ucFileOptRst = CommDeleteFile(pstFileInfo->ucTmpFileName);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
            }
            ucSubSeason = ERR_WRITE_FILE_END;
            emProcessStag = FWRITE_END;
        }
        break;

        case FWRITE_DELETE_FORMFILE:
        {
            ucFileOptRst = CommDeleteFile(pstFileInfo->ucFileName);
            if(FAIL == ucFileOptRst)
            {
                /*record error log*/
            }
            ucSubSeason = ERR_WRITE_FILE_END;
            emProcessStag = FWRITE_END;
        }
        break;

        case FWRITE_END:
        {
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            pstACKHead->ucType = pstMSGHead->ucProtect;
            pstACKHead->ucProtect = pstMSGHead->ucType;
            pstACKHead->usActualType = pstMSGHead->usActualType;
            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
            pstACKHead->ucDataFlg = 0U;
            pstACKHead->ucFlag = ACK_FRAME_SINGLE;
            /*清除数组索引，供应答使用*/
            usBuffIndex = 0U;
            usBuffIndex += sizeof(ACKHead_t);
            
            if(ucSubSeason != ERR_SUCCESS)
            {/*服务处理失败*/
                pstACKHead->ucResCode = RESCODE_FAILED;
                
                /*下装文件失败，将正在写入内存的文件删除，将顺序记录文件ID的全局量初始化*/
                SysDeleteCfgFile(emCfgFileType);

            }
            else
            {/*服务处理成功*/
                pstACKHead->ucResCode = RESCODE_SUCCESS;
                
            }
            
            DEBUG_COMM("ACK:  ucSubSeason = %d \n",ucSubSeason);
            DEBUG_COMM("ACK:  pstACKHead->ucResCode = %d \n",pstACKHead->ucResCode);
            
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            pstACKHead->ucSubReason = ucSubSeason;
            /*计算并填充数据应答的CRC*/
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(usBuffIndex);
            emProcessStag = FWRITE_START;
        }
        break;

        default:
        {/*其他异常情况，错误应答*/
            /*record error log*/
            ucSubSeason = ERR_WRITE_FILE_END;
            emProcessStag = FWRITE_END;
        }
        break;
            
    }

    return usACKDataLen;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommReadFileStart
*
* 功能描述: 读文件开始服务处理函数
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
static uint16_t CommReadFileStart(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;                                         /*文件控制结构*/
    FileReadInfo_t stFileReadInfo;                                          /*文件操作相关信息记录结构*/
    uint16_t usReadDataLen = 0;                                             /*读文件数据长度*/
    ReadFileContFlag_t emContFlag = CONT_TRANS;                             /*读文件继续标志*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucFileData[MAX_READ_FILE_LEN];                                  /*读取文件数据缓存*/
    SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        /*初始化文件名*/
        usBuffIndex += sizeof(MSGHead_t);
        /*获取要读取的文件信息*/
        memcpy(&stFileReadInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileType));
        usBuffIndex += sizeof(stFileReadInfo.ucFileType);
        memcpy(&stFileReadInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileID));
        usBuffIndex += sizeof(stFileReadInfo.ucFileID);
        
        /*将1131下发的文件ID，转化为PM使用的文件类型*/
        emCfgFileType = stFileReadInfo.ucFileID - 1U;

        /*根据文件名获取文件控制结构*/
        pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
        if(NULL == pstFileInfo)
        {
            /*file name is error ,file info is not exist*/
            ucSubSeason = ERR_READ_FILE_START;
        }
        else
        {
            /*获取内存上文件信息*/
            SysGetCfgFileState(emCfgFileType, &stCfgFileState);

            if(stCfgFileState.bWriteable != CFG_FILE_RD_ENABLE)
            {/*文件不可写*/
                /*log*/
                ucSubSeason = ERR_READ_FILE_START;
            }
            else
            {/*从内存上读取文件*/
                pstFileInfo->uiFileReadIndex = 0;
                if((stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex) > MAX_READ_FILE_LEN)
                {
                    usReadDataLen = MAX_READ_FILE_LEN;
                    emContFlag = CONT_TRANS;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    emContFlag = TRANS_DONE;
                }
                /*初始化文件数据读取缓存*/
                memset(ucFileData, 0x00, MAX_READ_FILE_LEN);
                if(usReadDataLen != SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        ucFileData, usReadDataLen))
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_START;
                }
            }
        }
    }

    /*produce ACK data*/
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    /*清除数组索引，供应答使用*/
    usBuffIndex = 0U;
    usBuffIndex += sizeof(ACKHead_t);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        /*填充文件是否传输完整标志*/
        memcpy(ucACKData + usBuffIndex,(puint8_t)&emContFlag,sizeof(ReadFileContFlag_t));
        usBuffIndex += sizeof(ReadFileContFlag_t);
        /*填充文件数据长度*/
        memcpy(ucACKData + usBuffIndex, (puint8_t)&usReadDataLen,sizeof(uint16_t));
        usBuffIndex += sizeof(uint16_t);
        /*填充文件数据*/
        memcpy(ucACKData + usBuffIndex,(puint8_t)(ucFileData),usReadDataLen);
        usBuffIndex += usReadDataLen;
        /*填写应答数据的长度，出去头部*/
        pstACKHead->ucDataFlg = 1U;
        /*记录文件读取的位置*/
        pstFileInfo->uiFileReadIndex += usReadDataLen;
    }
    pstACKHead->ucSubReason = ucSubSeason;
    pstACKHead->usLength = (uint16_t)(usBuffIndex);
    /*计算并填充数据应答的CRC*/
    uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
    memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
    usBuffIndex += sizeof(uiCalcCRC);
    usACKDataLen = (uint16_t)(usBuffIndex);

    return usACKDataLen;
 }


/*
********************************函数描述*******************************************************************************
* 函数名称: CommReadFileContinue
*
* 功能描述: 读文件继续服务处理函数
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
static uint16_t CommReadFileContinue(uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;                                         /*文件控制结构*/
    FileReadInfo_t stFileReadInfo;                                          /*文件操作相关信息记录结构*/
    uint16_t usReadDataLen = 0;                                             /*读文件数据长度*/
    ReadFileContFlag_t emContFlag = CONT_TRANS;                             /*读文件继续标志*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucFileData[MAX_READ_FILE_LEN];                                  /*读取文件数据缓存*/
    SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {

        /*初始化文件名*/
        memset(&ucFileName[0], 0, MAX_FILE_NAME_LEN);
        usBuffIndex += sizeof(MSGHead_t);
        /*获取要读取的文件信息*/
        memcpy(&stFileReadInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileType));
        usBuffIndex += sizeof(stFileReadInfo.ucFileType);
        memcpy(&stFileReadInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileID));
        usBuffIndex += sizeof(stFileReadInfo.ucFileID);
        
        /*将1131下发的文件ID，转化为PM使用的文件类型*/
        emCfgFileType = stFileReadInfo.ucFileID - 1U;
        
        /*根据文件名获取文件控制结构*/
        pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
        if(NULL == pstFileInfo)
        {
            /*file name is error ,file info is not exist*/
            ucSubSeason = ERR_READ_FILE_CONTINUE;
        }
        else
        {
            /*获取内存上文件信息*/
            SysGetCfgFileState(emCfgFileType, &stCfgFileState);

            if(stCfgFileState.bWriteable != CFG_FILE_RD_ENABLE)
            {/*文件不可写*/
                /*log*/
                ucSubSeason = ERR_READ_FILE_CONTINUE;
            }
            else
            {/*从内存上读取文件*/
                if((stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex) > MAX_READ_FILE_LEN)
                {
                    usReadDataLen = MAX_READ_FILE_LEN;
                    emContFlag = CONT_TRANS;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    emContFlag = TRANS_DONE;
                }
                /*初始化文件数据读取缓存*/
                memset(ucFileData, 0x00, MAX_READ_FILE_LEN);
                if(usReadDataLen != SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        ucFileData, usReadDataLen))
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_START;
                }
            }
        }
    }

    /*produce ACK data*/
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    /*清除数组索引，供应答使用*/
    usBuffIndex = 0U;
    usBuffIndex += sizeof(ACKHead_t);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {/*服务处理成功*/
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        /*填充文件是否传输完整标志*/
        memcpy(ucACKData + usBuffIndex,(puint8_t)&emContFlag,sizeof(ReadFileContFlag_t));
        usBuffIndex += sizeof(ReadFileContFlag_t);
        /*填充文件数据长度*/
        memcpy(ucACKData + usBuffIndex, (puint8_t)&usReadDataLen,sizeof(uint16_t));
        usBuffIndex += sizeof(uint16_t);
        /*填充文件数据*/
        memcpy(ucACKData + usBuffIndex,(puint8_t)(ucFileData),usReadDataLen);
        usBuffIndex += usReadDataLen;
        /*填写应答数据的长度，出去头部*/
        pstACKHead->ucDataFlg = 1U;
        /*记录文件读取的位置*/
        pstFileInfo->uiFileReadIndex += usReadDataLen;
        /**/
    }
    pstACKHead->ucSubReason = ucSubSeason;
    pstACKHead->usLength = (uint16_t)(usBuffIndex);
    /*计算并填充数据应答的CRC*/
    uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
    memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
    usBuffIndex += sizeof(uiCalcCRC);
    usACKDataLen = (uint16_t)(usBuffIndex);
    
    return usACKDataLen;
 }


static void CommOPCInit(void)
{
    return;
}



static void CommOPCCycle(void)
{
    CMController_t emCMIndex = CM1;                     /* CM 索引*/
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint8_t ucACKData[1024];
    MSGHead_t stMSGHead;                                /* OPC消息头部*/
    uint16_t usOPCDataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* OPC数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* OPC数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上OPC服务信息         */
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */


    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */

    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        memset(ucACKData, 0x00,1024);

        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,OPC_SRV);

        if(BUSY == emOPCBusyFlg[emCMIndex])
        {/*当服务处理标志为busy时，等待来自core0的应答数据*/

            /*从共享内存上获取应答数据*/
            SysGetSMMsg(SM_OPC_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);

            if(usACKDataLen > 0)
            {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                ucOPCACKTimeoutCount[emCMIndex] = 0;
                emOPCBusyFlg[emCMIndex] = FREE;
            }
            else
            {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
                ucOPCACKTimeoutCount[emCMIndex]++;
                if(ucOPCACKTimeoutCount[emCMIndex] == OPC_ACK_TIMEOUT)
                {

                    ucOPCACKTimeoutCount[emCMIndex] = 0;
                    emOPCBusyFlg[emCMIndex] = FREE;
                }
            }
        }
        else
        {/*BusyFlg is Free*/
            /*从系统资源获取来自CM的服务数据*/

            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

            SysGetMsg(emCMIndex,OPC_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usOPCDataLen);
            if(usOPCDataLen > 0)
             {/*数据长度大于0，表示收到数据*/
                 emOPCBusyFlg[emCMIndex] = BUSY;

                 /*获取消息头部*/
                 memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                 memcpy(&stMSGHead, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], sizeof(MSGHead_t));
                 /*记录服务数据相关信息*/
                 pstSysSrvInfo->stSysSrvData.uiSrvLen = usOPCDataLen;
                 pstSysSrvInfo->stSysSrvData.ucSrvNo = stMSGHead.usActualType;
                 /*计算数据CRC*/
                 uiCalCrc = SysCrc32Cal(0x00, (pchar_t)pstSysSrvInfo->stSysSrvData.ucSrvData, \
                         sizeof(stMSGHead) + stMSGHead.usLength);
                 usBuffIndex = sizeof(stMSGHead) + stMSGHead.usLength;
                 /*读取下发的CRC*/
                 memcpy((puint8_t)&uiReadCrc,(puint8_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex], \
                         CHKSUM_CRC_SIZE);


                 if(uiCalCrc != uiReadCrc)
                 {/*CRC 不一致*/
                     /*log*/
                 }
                 else if(usOPCDataLen != sizeof(stMSGHead) + stMSGHead.usLength + CHKSUM_CRC_SIZE)
                 {/*数据长度不一致*/
                     /*log*/
                 }
                 else
                 {
                     switch(stMSGHead.usActualType)
                     {/*根据服务号，处理1131服务*/
                         /*these service need to synchronize*/
                         case OPC_WRITE_VAR:
                         {/*将服务进行同步*/
                             stSyncTask.emTaskId = SYS_BROADCAST_OPCSRV;
                             stSyncTask.emPMId = INVALID_CM_ID;
                             stSyncTask.emTaskStatus = TASK_SEND;
                             stSyncTask.emTaskWait = TASK_NOWAIT;
                             pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

//                             if(SYS_STABLE_STATE == SysGetState())
//                             {/* 非稳态时按错误处理，不添加同步任务 */
//                                 SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
//                             }

                             /*暂定为：非稳态时也可以处理服务。*/
                             SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                         }

                         break;

//                         case OPC_READ_CLOCK:
//                         {/*process 1131 check exlibvern*/
//                             usACKDataLen = CommOPCReadClock (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
//                                                             stMSGHead.usLength,&ucACKData[0]);
//                         }
//                         break;
//
//                         case OPC_READ_VERN:
//                         {/*process 1131 read ProjectInfo*/
//                             usACKDataLen = CommOPCReadCPStatus(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
//                                                             stMSGHead.usLength,&ucACKData[0]);
//                         }
//                         break;

                         default:
                         break;
                     }
                 }
             }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            SysSetMsg(emCMIndex,OPC_RESP,ucACKData,usACKDataLen);
        }
    }
    return;
}



//static uint16_t CommOPCReadClock(uint8_t ucOPCData[], uint32_t usOPCDataLen,uint8_t ucACKData[])
//{
//    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
//    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
//    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
//    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
//    uint32_t uiCalcCRC = 0;
//    uint16_t usReadDataLen = 0;                                             /*获取数据的长度*/
//    ContrlerClock_t stContrlerClock;
//    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/



//    if(NULL == ucOPCData)
//    {/*服务数据指针错误*/
//        ucSubSeason = ERR_SRC_ADDR;
//    }
//    else if(NULL == ucACKData)
//    {/*应答数据指针错误*/
//        ucSubSeason = ERR_ACK_ADDR;
//    }
//    else
//    {/*获取时钟*/
//        usReadDataLen = SysGetClock(&stContrlerClock);
//        if(usReadDataLen <= 0)
//        {/*read controller clock error*/
//            ucSubSeason = ERR_OPC_READ_CLOCK;
//        }
//    }

//    /*produce ACK data*/
//    pstACKHead = (ACKHead_t*)ucACKData;
//    pstMSGHead = (MSGHead_t*)ucOPCData;
//    pstACKHead->ucType = pstMSGHead->ucProtect;
//    pstACKHead->ucProtect = pstMSGHead->ucType;
//    pstACKHead->usActualType = pstMSGHead->usActualType;
//   pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//    pstACKHead->ucDataFlg = 0U;
//    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//    usBuffIndex +=  sizeof(ACKHead_t);
//    if(ucSubSeason != ERR_SUCCESS)
//    {/*服务处理失败*/
//        pstACKHead->ucResCode = RESCODE_FAILED;
//    }
//    else
//    {/*服务处理成功*/
//        pstACKHead->ucResCode = RESCODE_SUCCESS;
//        memcpy(ucACKData + usBuffIndex, (puint8_t)&stContrlerClock, sizeof(ContrlerClock_t));
//        /*设置包含应答数据*/
//        pstACKHead->ucDataFlg = 1U;
//        usBuffIndex += sizeof(ContrlerClock_t);
//
//    }
//    pstACKHead->usLength = (uint16_t)(usBuffIndex);
//    pstACKHead->ucSubReason = ucSubSeason;
//    /*计算并填充应答数据的CRC*/
//    uiCalcCRC = SysCrc32Cal( 0, ucACKData,  pstACKHead->usLength );
//    memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
//    usBuffIndex += sizeof(uiCalcCRC);
//
//    usACKDataLen = (uint16_t)(usBuffIndex);
//
//    return usACKDataLen;
//}


//static uint16_t CommOPCReadCPStatus(uint8_t ucOPCData[], uint32_t usOPCDataLen,uint8_t ucACKData[])
//{
//    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
//    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
//    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
//    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
//    uint32_t uiCalcCRC = 0;
//    uint16_t usReadDataLen = 0;                                             /*获取数据的长度*/
//    CPStatus_t stCPStatus;
//    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
//
//
//
//    if(NULL == ucOPCData)
//    {/*服务数据指针错误*/
//        ucSubSeason = ERR_SRC_ADDR;
//    }
//    else if(NULL == ucACKData)
//    {/*应答数据指针错误*/
//        ucSubSeason = ERR_ACK_ADDR;
//    }
//    else
//    {
//        usReadDataLen = SysGetCPStatus(&stCPStatus);
//        if(usReadDataLen <= 0)
//        {/*read controller clock error*/
//            ucSubSeason = ERR_OPC_READ_CLOCK;
//        }
//    }
//
//    /*produce ACK data*/
//    pstACKHead = (ACKHead_t*)ucACKData;
//    pstMSGHead = (MSGHead_t*)ucOPCData;
//    pstACKHead->ucType = pstMSGHead->ucProtect;
//    pstACKHead->ucProtect = pstMSGHead->ucType;
//    pstACKHead->usActualType = pstMSGHead->usActualType;
//    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//    pstACKHead->ucDataFlg = 0U;
//    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//    usBuffIndex +=  sizeof(ACKHead_t);
//    if(ucSubSeason != ERR_SUCCESS)
//    {/*服务处理失败*/
//        pstACKHead->ucResCode = RESCODE_FAILED;
//    }
//    else
//    {/*服务处理成功*/
//        pstACKHead->ucResCode = RESCODE_SUCCESS;
//        memcpy(ucACKData + usBuffIndex,(puint8_t)&stCPStatus, sizeof(CPStatus_t));
//        usBuffIndex += sizeof(CPStatus_t);
//        /*设置包含应答数据*/
//        pstACKHead->ucDataFlg = 1U;
//    }
//    pstACKHead->usLength = (uint16_t)(usBuffIndex);
//    pstACKHead->ucSubReason = ucSubSeason;
//
//    uiCalcCRC = SysCrc32Cal( 0, ucACKData,  pstACKHead->usLength );
//    memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//    usBuffIndex += sizeof(uiCalcCRC);
//
//    usACKDataLen = (uint16_t)(usBuffIndex);
//
//    return usACKDataLen;
//}



static void CommModbusInit(void)
{
    return;
}



static void CommModbusCycle(void)
{
//    CMController_t emCMIndex = CM1;                     /* CM 索引*/
//    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
//    uint8_t ucACKData[1024];
//    MSGHead_t stMSGHead;                                /* Modbus消息头部*/
//    uint16_t usModbusDataLen = 0;
//    uint32_t uiReadCrc = 0U;                            /* Modbus数据中的CRC值    */
//    uint32_t uiCalCrc = 0U;                             /* Modbus数据计算的CRC值  */
//    uint16_t usACKDataLen = 0;
//    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
//    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上Modbus服务信息         */
//    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */
//    ModbusHead_t* pstModbusHead;
//
//
//    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */
//
//    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
//    {
//        memset(ucACKData, 0x00,1024);
//
//        /*获取服务数据在系统资源的存储结构指针*/
//        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,OPC_SRV);
//
//        if(BUSY == emModbusBusyFlg[emCMIndex])
//        {/*当服务处理标志为busy时，等待来自core0的应答数据*/
//
//            /*从共享内存上获取应答数据*/
//            SysGetSMMsg(SM_MODBUS_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);
//
//            if(usACKDataLen > 0)
//            {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
//                ucModbusACKTimeoutCount[emCMIndex] = 0;
//                emModbusBusyFlg[emCMIndex] = FREE;
//            }
//            else
//            {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
//                ucModbusACKTimeoutCount[emCMIndex]++;
//                if(ucModbusACKTimeoutCount[emCMIndex] == MODBUS_ACK_TIMEOUT)
//                {
//
//                    ucModbusACKTimeoutCount[emCMIndex] = 0;
//                    emModbusBusyFlg[emCMIndex] = FREE;
//                }
//            }
//        }
//        else
//        {/*BusyFlg is Free*/
//            /*从系统资源获取来自CM的服务数据*/
//
//            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));
//
//            SysGetMsg(emCMIndex, MODBUS_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usModbusDataLen);
//            if(usModbusDataLen > 0)
//             {/*数据长度大于0，表示收到数据*/
//                 emModbusBusyFlg[emCMIndex] = BUSY;
//
//                 pstModbusHead = (ModbusHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
//
//                 if(usModbusDataLen != MODBUS_HEAD_LEN + pstModbusHead->usDataLen)//data len is not consistent
//                 {
//                     /*log*/
//                 }
//                 else
//                 {/*将服务进行同步*/
//                     stSyncTask.emTaskId = SYS_BROADCAST_MODBUSSRV;
//                     stSyncTask.emPMId = INVALID_CM_ID;
//                     stSyncTask.emTaskStatus = TASK_SEND;
//                     stSyncTask.emTaskWait = TASK_NOWAIT;
//                     pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//
////                             if(SYS_STABLE_STATE == SysGetState())
////                             {/* 非稳态时按错误处理，不添加同步任务 */
////                                 SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
////                             }
//
//                     /*暂定为：非稳态时也可以处理服务。*/
//                     SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
//                 }
//             }
//        }
//    }


    return;



}


static void CommP2PInit(void)
{
    return;
}



static void CommP2PCycle(void)
{

    CommP2PRecv();
    CommP2PSend();

    return;
    
}


static void CommP2PRecv(void)
{
//
//    CMController_t emCMIndex = CM1;                     /* CM 索引*/
//    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
//    uint8_t ucACKData[1024];
//    MSGHead_t stMSGHead;                                /* P2P消息头部*/
//    uint16_t usP2PDataLen = 0;
//    uint32_t uiReadCrc = 0U;                            /* P2P数据中的CRC值    */
//    uint32_t uiCalCrc = 0U;                             /* P2P数据计算的CRC值  */
//    uint16_t usACKDataLen = 0;
//    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
//    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上P2P服务信息         */
//    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */
//    P2PRecvHead_t* pstP2PRecvHead;
//
//    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */
//
//    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
//    {
//        memset(ucACKData, 0x00,1024);
//
//        /*获取服务数据在系统资源的存储结构指针*/
//        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,P2P_SRV);
//
//        if(BUSY == emP2PBusyFlg[emCMIndex])
//        {/*当服务处理标志为busy时，等待来自core0的应答数据*/
//
//            /*从共享内存上获取应答数据*/
//            SysGetSMMsg(SM_P2P_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);
//
//            if(usACKDataLen > 0)
//            {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
//                ucP2PACKTimeoutCount[emCMIndex] = 0;
//                emP2PBusyFlg[emCMIndex] = FREE;
//            }
//            else
//            {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
//                ucP2PACKTimeoutCount[emCMIndex]++;
//                if(ucP2PACKTimeoutCount[emCMIndex] == P2P_ACK_TIMEOUT)
//                {
//
//                    ucP2PACKTimeoutCount[emCMIndex] = 0;
//                    emP2PBusyFlg[emCMIndex] = FREE;
//                }
//            }
//        }
//        else
//        {/*BusyFlg is Free*/
//            /*从系统资源获取来自CM的服务数据*/
//
//            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));
//
//            SysGetMsg(emCMIndex,P2P_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usP2PDataLen);
//            if(usP2PDataLen > 0)
//             {/*数据长度大于0，表示收到数据*/
//                 emP2PBusyFlg[emCMIndex] = BUSY;
//
//                 /*获取消息头部*/
//                 pstP2PRecvHead = (P2PRecvHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
//
//                 if(usP2PDataLen != CHKSUM_CRC_SIZE + pstP2PRecvHead->usTotalLen)//data len is not consistent
//                 {
//                     /*log*/
//                 }
//                 else
//                 {
//                     switch(pstP2PRecvHead->ucFrameType)
//                     {/*将服务进行同步*/
//                         case P2P_REQ_FRAME:
//                             /*send to core0*/
//                         case P2P_RES_FRAME:
//                             /*send to core1*/
//                         {
//                             stSyncTask.emTaskId = SYS_BROADCAST_P2PSRV;
//                             stSyncTask.emPMId = INVALID_CM_ID;
//                             stSyncTask.emTaskStatus = TASK_SEND;
//                             stSyncTask.emTaskWait = TASK_NOWAIT;
//                             pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
//
//                             /*暂定为：非稳态时也可以处理服务。*/
//                             SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
//                         }
//                         break;
//
//                         default:
//                         break;
//                     }
//                 }
//             }
//        }
//
//        if(usACKDataLen > 0)
//        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
//            SysSetMsg(emCMIndex,P2P_RESP,ucACKData,usACKDataLen);
//        }
//    }


    return;
}


static void CommP2PSend(void)
{
//    uint8_t ucBlockIndex = 0U;                          /*P2P发送块的块号索引*/
//    uint16_t usBlockSendCycle = 0U;                     /*P2P发送块配置的发送周期*/
//    uint8_t ucSendIndex = 0U;                           /*当前要发送块的索引*/
//    uint8_t ucP2PSendbuff[P2P_SEND_BUFF_SIZE];          /*发送数据缓存*/
//    uint16_t usP2PSendDataLen = 0U;                     /*发送数据长度*/
//    CMController_t emCMIndex = CM1;
//
//    /*send control process*/
//    for(ucBlockIndex = 0; ucBlockIndex < MAX_P2P_BLOCK_NUM; ucBlockIndex++)
//    {
//        if(ACTIVE == stP2PSendCtrl[ucBlockIndex].emSendBlockActive)
//        {/*轮询所有模块，发送周期计数*/
//            stP2PSendCtrl[ucBlockIndex].usSendCount++;
//            usBlockSendCycle = SysGetP2PBlockSendCycle(ucBlockIndex);
//            if(stP2PSendCtrl[ucBlockIndex].usSendCount >= usBlockSendCycle)
//            {/*达到发送周期*/
//                stP2PSendCtrl[ucBlockIndex].usSendCount = 0;
//                if(WAIT_ACK == stP2PSendCtrl[ucBlockIndex].emWaitACKFlag)
//                {/*在等待来自对端控制站的ACK*/
//                    if(stP2PSendCtrl[ucBlockIndex].usWaitACKCount > P2P_WAITACK_CYCLE)
//                    {/*等待ACK超时*/
//                        /*调用安全层接口，错误处理*/
//                        StaHandleResTO(stP2PSendCtrl[ucBlockIndex].ucDestStaID);
//                        /*清空相应标志，计数*/
//                        stP2PSendCtrl[ucBlockIndex].emWaitACKFlag = NOT_WAIT_ACK;
//                        stP2PSendCtrl[ucBlockIndex].usWaitACKCount = 0;
//                        if(NEED_RETRY == stP2PSendCtrl[ucBlockIndex].emNeedRetryFlag)
//                        {/*如果需要重传，进行重传，将需要重传标志清除*/
//                            stP2PSendCtrl[ucBlockIndex].emNeedRetryFlag = NOT_NEED_RETRY;
//                            stP2PSendCtrl[ucBlockIndex].emWaitSendFlag = WAIT_SEND; 
//                        }
//                    }
//                }
//                else
//                {/*每次到达发送周期，设置发送标志，若应答不能及时到达，重传一次*/
//                    stP2PSendCtrl[ucBlockIndex].emNeedRetryFlag = NEED_RETRY;
//                    stP2PSendCtrl[ucBlockIndex].emWaitSendFlag = WAIT_SEND; 
//                }
//            }
//            if(WAIT_ACK == stP2PSendCtrl[ucBlockIndex].emWaitACKFlag)
//            {
//                stP2PSendCtrl[ucBlockIndex].usWaitACKCount++;
//            }
//       }
//    }
//
//    /*P2P block send process,each cycle process one block*/
//    ucSendIndex = ucP2PSendIndex;
//    ucSendIndex++;
//    if(WAIT_SEND == stP2PSendCtrl[ucSendIndex].emWaitSendFlag)
//    {
//        //packing P2P data 
//        usP2PSendDataLen = CommP2PPacking(ucSendIndex,(puint8_t) &ucP2PSendbuff[0]);//undefined
//        if(usP2PSendDataLen > 0)
//        {
//            SysSetMsg(emCMIndex,P2P_RESP,ucP2PSendbuff,usP2PSendDataLen);
//        }
//        else
//        {
//            /*log*/
//        }
//    }
//    ucP2PSendIndex = ucSendIndex;
    return;
}


//static void CommSOEInit(void)
//{
//    return;
//}
//
//static void CommSOECycle(void)
//{
//    CMController_t emCMIndex = CM1;                     /* CM 索引*/
//    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
//    uint8_t ucACKData[1024];
//    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
//    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
//    uint16_t usSOEDataLen = 0;
//    uint32_t uiReadCrc = 0U;                            /* SOE数据中的CRC值    */
//    uint32_t uiCalCrc = 0U;                             /* SOE数据计算的CRC值  */
//    uint16_t usACKDataLen = 0;
//    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上SOE服务信息         */
//
//    
//
//    for(emCMIndex = 0;emCMIndex < NUM_OF_CM; emCMIndex++)
//    {
//        memset(&ucACKData[0], 0x00,1024);
//        /*获取服务数据在系统资源的存储结构指针*/
//        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,SOE_SRV);
//        /*从系统资源上获取上位机请求数据*/
//        SysGetMsg(emCMIndex,SOE_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usSOEDataLen);
//
//        if(usSOEDataLen > 0)
//        {
//            pstMSGHead = (MSGHead_t*)pstSysSrvInfo->stSysSrvData.ucSrvData[0];
//
//            uiCalCrc = SysCrc32Cal(0x00, (pchar_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[0],
//                                        sizeof(MSGHead_t) + pstMSGHead->usLength);
//            usBuffIndex += sizeof(MSGHead_t) + pstMSGHead->usLength;
//            /*获取下发的数据CRC*/
//            memcpy((puint8_t)&uiReadCrc,(puint8_t)(&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex]),CHKSUM_CRC_SIZE);
//                  
//            if(uiCalCrc != uiReadCrc)/*CRC is not consistent*/
//            {
//                /*log*/
//            }
//            else if(usSOEDataLen != sizeof(MSGHead_t) + pstMSGHead->usLength+CHKSUM_CRC_SIZE)
//            {/*data len is not consistent*/
//                /*log*/
//            }
//            else
//            {
//                switch(pstMSGHead->usActualType)
//                {
//                    case SOE_READ_RECORD:
//                    {
//                        /*process read SOE record service*/
//                        usACKDataLen = CommSOERead(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], pstMSGHead->usLength, \
//                                &ucACKData[0]);
//                    }
//                    break;
//
//                    case SOE_DELETE_RECORD:
//                    {
//                        /*process delete SOE record service*/
//                        usACKDataLen = CommSOEDelete((pstSysSrvInfo->stSysSrvData.ucSrvData), pstMSGHead->usLength, \
//                                &ucACKData[0]);
//                    }
//                    break;
//
//                    default:
//                    break;
//                }
//            }
//            if(usACKDataLen > 0)
//            {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
//                SysSetMsg(emCMIndex,SOE_RESP,ucACKData,usACKDataLen);
//            }
//        }
//    }
//    return; 
//}
//
//static uint16_t CommSOERead(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[])
//{
//
//    uint8_t ucSubSeason = ERR_SUCCESS;                  /*服务处理结果错误码*/
//    uint16_t usACKDataLen = 0;
//    PMController_t emLocalID = PMA;
//    uint16_t usSOEEntryNum = 0;
//    uint16_t usIndex = 0;
//    uint32_t uiCalcCRC = 0;
//    SOEType_t emSOEType = 0;
//    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
//    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
//    SOEReadReq_t* pstSOEReadReq;                        /* 读SOE请求结构*/
//    SOEReadRsp_t* pstSOEReadRsp;                        /* 读SOE应答结构*/
//    SWSoeRecord_t stSWSoeRecord;                        /* 软SOE记录结构*/
//    HWSoeRecord_t stHWSoeRecord;                        /* 硬SOE记录结构*/
//    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
//
//    if(NULL == ucSOEData)
//    {/*服务数据指针错误*/
//        ucSubSeason = ERR_SRC_ADDR;
//    }
//    else if(NULL == ucACKData)
//    {/*应答数据指针错误*/
//        ucSubSeason = ERR_ACK_ADDR;
//    }
//    else
//    {
//        usBuffIndex += sizeof(MSGHead_t);
//        pstSOEReadReq = (SOEReadReq_t*)(&ucSOEData[usBuffIndex]);
//        emLocalID = SysGetLocalPMID();
//        if(pstSOEReadReq->ucPMID != emLocalID)
//        {
//            ucSubSeason = ERR_SOE_READ;
//        }
//        if(SysGetSOEActive(pstSOEReadReq->ucSOEBlockID) != ACTIVE)
//        {
//            ucSubSeason = ERR_SOE_READ;
//        }
//        else
//        {
//            usSOEEntryNum = SysGetSOEEntryNum(pstSOEReadReq->ucSOEBlockID,pstSOEReadReq->usGetIndex);
//            emSOEType = SysGetSOEType(pstSOEReadReq->ucSOEBlockID);  /*获取SOE块的软硬属性*/
//            if(usSOEEntryNum > MAX_SOE_READ_NUM)
//            {
//                usSOEEntryNum = MAX_SOE_READ_NUM;
//            }
//            /*清空，供应答缓存使用*/
//            usBuffIndex = 0U;
//            usBuffIndex += sizeof(ACKHead_t);
//            pstSOEReadRsp = (SOEReadRsp_t*)(ucACKData[usBuffIndex]);
//            usBuffIndex +=  sizeof(SOEReadRsp_t);
//            for(usIndex = 0; usIndex < usSOEEntryNum; usIndex++)
//            {
//                if(SW_SOE == emSOEType)
//                {
//                    SysGetSOEEntry(pstSOEReadReq->ucSOEBlockID,(pstSOEReadReq->usGetIndex + usIndex),&stSWSoeRecord);
//                    memcpy((puint8_t)&ucACKData[usBuffIndex],&stSWSoeRecord,sizeof(SWSoeRecord_t));
//                    usBuffIndex += sizeof(SWSoeRecord_t);
//                }
//                if(HW_SOE == emSOEType)
//                {
//                    SysGetSOEEntry(pstSOEReadReq->ucSOEBlockID,(pstSOEReadReq->usGetIndex + usIndex),&stHWSoeRecord);
//                    memcpy((puint8_t)&ucACKData[usBuffIndex],&stHWSoeRecord,sizeof(HWSoeRecord_t));
//                    usBuffIndex += sizeof(HWSoeRecord_t);
//
//                }
//            }
//        }
//
//        /*produce ACK data*/
//        pstACKHead = (ACKHead_t*)ucACKData;
//        pstMSGHead = (MSGHead_t*)ucSOEData;
//        pstACKHead->ucType = pstMSGHead->ucProtect;
//        pstACKHead->ucProtect = pstMSGHead->ucType;
//        pstACKHead->usActualType = pstMSGHead->usActualType;
//        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//        pstACKHead->ucDataFlg = 0U;
//        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//
//        if(ucSubSeason != ERR_SUCCESS)
//        {/*服务处理失败*/
//            pstACKHead->ucResCode = RESCODE_FAILED;
//        }
//        else
//        {/*服务处理成功*/
//            pstACKHead->ucResCode = RESCODE_SUCCESS;
//            /*填充读SOE应答的结构*/
//            pstSOEReadRsp->ucPMID = pstSOEReadReq->ucPMID;
//            pstSOEReadRsp->ucSOEBlockID = pstSOEReadReq->ucSOEBlockID;
//            pstSOEReadRsp->uiGeneration = pstSOEReadReq->uiGeneration;
//            pstSOEReadRsp->uiWrapCount = pstSOEReadReq->uiWrapCount;
//            pstSOEReadRsp->usEntriNum = usSOEEntryNum;
//            pstSOEReadRsp->usNextIndex = pstSOEReadReq->usGetIndex + usSOEEntryNum;
//        }
//        pstACKHead->ucSubReason = ucSubSeason;
//        pstACKHead->usLength = (uint16_t)(usBuffIndex);
//        /*计算并填充应答CRC*/
//        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength);
//        memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//        usBuffIndex += sizeof(uiCalcCRC);
//
//        usACKDataLen = (uint16_t)(usBuffIndex);
//    }
//
//    return usACKDataLen;
//}
//
//static uint16_t CommSOEDelete(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[])
//{
//    uint8_t ucSubSeason = ERR_SUCCESS;                  /* 服务处理结果错误码*/
//    uint16_t usACKDataLen = 0;                          /* 用于返回应答数据长度*/
//    PMController_t emLocalID = PMA;
//    uint16_t usIndex = 0;
//    uint32_t uiCalcCRC = 0;
//    SOEType_t emSOEType = 0;
//    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
//    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
//    SOEDeleteReq_t* pstSOEDeleteReq;                    /* 删除SOE请求结构*/
//    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
//
//    
//    if(NULL == ucSOEData)
//    {/*服务数据指针错误*/
//        ucSubSeason = ERR_SRC_ADDR;
//    }
//    else if(NULL == ucACKData)
//    {/*应答数据指针错误*/
//        ucSubSeason = ERR_ACK_ADDR;
//    }
//    else
//    {
//        usBuffIndex += sizeof(MSGHead_t);
//        pstSOEDeleteReq = (SOEDeleteReq_t*)(&ucSOEData[usBuffIndex]);
//        emLocalID = SysGetLocalPMID();
//        if(pstSOEDeleteReq->ucPMID != emLocalID)
//        {
//            ucSubSeason = ERR_SOE_READ;
//        }
//        if(SysGetSOEActive(pstSOEDeleteReq->ucSOEBlockID) != ACTIVE)
//        {
//            ucSubSeason = ERR_SOE_READ;
//        }
//        else
//        {
//            SysClearSOEEntry(pstSOEDeleteReq->ucSOEBlockID);  /*此函数未实现，相关错误处理后续添加*/
//        }
//
//        /*produce ACK data*/
//        pstACKHead = (ACKHead_t*)ucACKData;
//        pstMSGHead = (MSGHead_t*)ucSOEData;
//        pstACKHead->ucType = pstMSGHead->ucProtect;
//        pstACKHead->ucProtect = pstMSGHead->ucType;
//        pstACKHead->usActualType = pstMSGHead->usActualType;
//        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//        pstACKHead->ucDataFlg = 0U;
//        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//        usBuffIndex += sizeof(ACKHead_t);
//        if(ucSubSeason != ERR_SUCCESS)
//        {/*服务处理失败*/
//            pstACKHead->ucResCode = RESCODE_FAILED;
//        }
//        else
//        {/*服务处理成功*/
//            pstACKHead->ucResCode = RESCODE_SUCCESS;
//        }
//        pstACKHead->ucSubReason = ucSubSeason;
//        pstACKHead->usLength = (uint16_t)(usBuffIndex);
//
//        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength);
//        memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//        usBuffIndex += sizeof(uiCalcCRC);
//
//        usACKDataLen = (uint16_t)(usBuffIndex);
//    }
//
//    return usACKDataLen;
//}
//

static void CommAMSInit(void)
{
    return;
}

static void CommAMSCycle(void)
{
    return;
}


static void CommDiagInit(void)
{
    return;
}

static void CommDiagCycle(void)
{
    return;
}

static void CommInterCMDInit(void)
{
    return;
}

static void CommInterCMDCycle(void) /*待补充*/
{
    uint16_t usInterCMDDataLen = 0U;                    /* 接收到的内部命令数据长度*/
    PMController_t emLocalID = PMA;
    CMController_t emCMIndex = CM1;                     /* CM 索引*/
    uint16_t usIndex = 0U;
    uint32_t uiReadCrc = 0U;                            /* InterCMD数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* InterCMD数据计算的CRC值  */
    InterCMDHead_t* pstInterCMDHead;
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上SOE服务信息         */
    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
    SysCfgFileTable_t* pstSysCfgFileTable = NULL;       /* 上传CM的配置文件表*/

    for(emCMIndex = CM1; emCMIndex < NUM_OF_CM; emCMIndex++)
    {

        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,SOE_SRV);
        /*从系统资源上获取上位机请求数据*/
        SysGetMsg(emCMIndex,INTER_CMD_REQ, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usInterCMDDataLen);

        if(usInterCMDDataLen > 0)
        {
            pstInterCMDHead = (InterCMDHead_t*)pstSysSrvInfo->stSysSrvData.ucSrvData[0];

            uiCalCrc = SysCrc32Cal(0x00, (pchar_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[0],
                                        sizeof(InterCMDHead_t));
            usBuffIndex += sizeof(InterCMDHead_t);
            /*获取下发的数据CRC*/
            memcpy((puint8_t)&uiReadCrc,(puint8_t)(&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex]),CHKSUM_CRC_SIZE);

            if(uiCalCrc != uiReadCrc)
            {/*CRC is not consistent*/
                /*log*/
            }
            else
            {
                switch(pstInterCMDHead->emInterCMD)
                {
                    case CMD_REQ_ALL_CMCONFIG:
                    {
                        /*获取各CM需要配置文件的总表*/
                        pstSysCfgFileTable = SysGetCfgFileAllTable();
                        if(NULL == pstSysCfgFileTable)
                        {
                            /*log*/
                        }
                        else
                        {
                            /*将文件上传传表更新到系统资源*/
                            SysSetCfgFileTable(pstSysCfgFileTable);
                        }

                    }
                    break;

                    case CMD_REQ_INCRE_CMCONFIG:
                    {
                        /*获取各CM需要配置文件的最近一次增量下装的增量表*/
                        pstSysCfgFileTable = SysGetCfgFileIncreTable();
                        if(NULL == pstSysCfgFileTable)
                        {
                            /*log*/
                        }
                        else
                        {
                            /*将文件上传传表更新到系统资源*/
                            SysSetCfgFileTable(pstSysCfgFileTable);
                        }
                    }
                    break;

                    default:
                    break;
                }
            }
        }
    }
    return;
}


static void CommTimingInit(void)
{
    /*关于校时，内部通讯的时间消耗补偿需不需要，校时的精度怎么保证，时钟主从怎么设定。*/
    return;
}


static void CommTimingCycle(void)
{
    return;
}

static void CommClientInit(void)
{
    return;
}

static void CommClientCycle(void)
{
    return;
}

static uint32_t SysGetExlibVern(void)
{
    uint32_t uiExlibVern = 0U;

    return uiExlibVern;
}

static ProjectInfo_t* SysGetProjectInfo(void)
{
    return &stProjectInfo;
}

static ForceList_t* SysGetForceList(void)
{
    return &stForceList;
}

static void CommFileInfoInit(void)
{
    SysCfgFileType_t emCfgFileType = 0U;

    for(emCfgFileType = 0U; emCfgFileType < NUM_OF_CFG_FILE_TYPE; emCfgFileType++)
    {
        stFileInfo[emCfgFileType].emFileType = emCfgFileType;
        stFileInfo[emCfgFileType].iFid = -1;
        stFileInfo[emCfgFileType].uiFileDataLen = 0U;
        stFileInfo[emCfgFileType].uiFileReadIndex = 0U;
        stFileInfo[emCfgFileType].uiFileWriteIndex = 0U;
        stFileInfo[emCfgFileType].usFileNameLen = 0U;
//        printf("\n...-----begin  CommFileInfoInit   strcpy  !-----...\n");
        strcpy((pchar_t)&stFileInfo[emCfgFileType].ucFileName[0], (char_t)&ucFileName[emCfgFileType][0]);
        strcpy((pchar_t)&stFileInfo[emCfgFileType].ucTmpFileName[0], (char_t)&ucTmpFileName[emCfgFileType][0]);
        strcpy((pchar_t)&stFileInfo[emCfgFileType].ucOldFileName[0], (char_t)&ucOldFileName[emCfgFileType][0]);
//        
//        printf("\n...-----END  CommFileInfoInit   strcpy  !-----...\n");
        
        
    }

    return;
}

/*test*/

static uint16_t CommDownloadStart (uint8_t ucCS1131Data[], uint32_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucDownloadFlag = 0U;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint32_t uiBuffIndex = 0U;                                  /*数组下标索引*/
    SysCfgFileTable_t* s_pstSysCfgFileTable = NULL;             /*上传CM的文件表*/
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PMID号       */
    PrjInfo_t *pstDLPrjInfo = NULL;                             /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    
    DEBUG_COMM("begin CommDownloadStart  \n");
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
//        DEBUG_COMM("NULL == ucCS1131Data  \n");
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
//        DEBUG_COMM("NULL == ucACKData  \n");
    }
    else
    {/*拷贝1131下发的下装标志*/
        uiBuffIndex += sizeof(MSGHead_t);
        memcpy((puint8_t)&ucDownloadFlag,&ucCS1131Data[uiBuffIndex],sizeof(ucDownloadFlag));
//        DEBUG_COMM("ucDownloadFlag is: %x  \n",ucDownloadFlag);
        
        if(0x00 == ucDownloadFlag)
        {
            SharedSetDownloadFlg(INIT_DOWNLOAD);
            SysCfgFileInit();
            s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileAllTable();
            memset(s_pstSysCfgFileTable, 0x00, sizeof(SysCfgFileTable_t));
            
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstPrjInfo = SysGetPrjInfo(emLocalPMId);
            memset(pstPrjInfo, 0x00, sizeof(PrjInfo_t));
            
            DEBUG_COMM("CommDownloadStart : INIT_DOWNLOAD \n");
        }
        if(0x01 == ucDownloadFlag)
        {
            SharedSetDownloadFlg(ONLINE_DOWNLOAD);
            s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileIncreTable();
            memset(s_pstSysCfgFileTable, 0x00, sizeof(SysCfgFileTable_t));
            
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            
            DEBUG_COMM("CommDownloadStart : ONLINE_DOWNLOAD \n");
        }
    }

    /*produce ACK data*/
    pstACKHead = (ACKHead_t*)ucACKData;
    pstMSGHead = (MSGHead_t*)ucCS1131Data;
    pstACKHead->ucType = pstMSGHead->ucProtect;
    pstACKHead->ucProtect = pstMSGHead->ucType;
    pstACKHead->usActualType = pstMSGHead->usActualType;
    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
    pstACKHead->ucDataFlg = 0U;
    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
    uiACKDataIndex += sizeof(ACKHead_t);
//    DEBUG_COMM("sizeof(ACKHead_t) is : %d \n",sizeof(ACKHead_t));
//    DEBUG_COMM("ucSubSeason is: %d  \n",ucSubSeason);
    if(ucSubSeason != ERR_SUCCESS)
    {/*服务处理失败*/
        pstACKHead->ucResCode = RESCODE_FAILED;
    }
    else
    {/*服务处理成功*/
        pstACKHead->ucResCode = RESCODE_SUCCESS;
    }
    pstACKHead->ucSubReason = ucSubSeason;

    pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
    
    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
    memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
    uiACKDataIndex += sizeof(uiCalcCRC);
    usACKDataLen = (uint16_t)(uiACKDataIndex);
    
//    DEBUG_COMM("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}



/*test*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
