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
#include "../inc/Comm.h"
#include "Srv/IPBUS/inc/IPBUS.h"
#include "Srv/Log/inc/Log.h"
#include "Srv/Log/inc/LogID.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t ucRTDataBackupCount = 0;

static uint8_t ucCS1131ACKTimeoutCount[NUM_OF_CM];
static File_Handle_Flag_t emFileHandleFlg[NUM_OF_CM];
static uint8_t ucOPCACKTimeoutCount[NUM_OF_CM];

static uint8_t ucP2PACKTimeoutCount[NUM_OF_CM];
static FileInfo_t stFileInfo[NUM_OF_CFG_FILE_TYPE];

static P2PSendCtrl_t s_stP2PSendCtrl[MAX_P2P_BLOCK_NUM+1];

static uint8_t s_ucModOnlineFlg[MAX_SLAVE_NUM];

static AperiodicHandleFlg_t emAperiodicHandleFlg;

static uint16_t s_usMsgSEQ = 0U;

static SlaveFWInfo_t s_stSlaveFWInfo;

static uint32_t s_uiFWUpdateFlg;


static uint8_t ss_ucTest = 0;

extern int nor_flash_errno;

static uint8_t s_ucFWUpdateRest = 0;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void CommCS1131Init(void);

//static void CommCS1131Cycle(void);

static uint16_t CommReadDisableTabl(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[], uint8_t ucACKData[]);

static void CommFileInfoInit(void);

static uint16_t CommWriteFileStart(uint8_t ucCS1131Data[], uint32_t uiCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileDone(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileStart(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static void CommBitInvert(uint8_t ucSrcData[], uint8_t ucDestData[], uint16_t usDataLen);

static uint16_t CommOSCGetVar(uint8_t ucCS1131Data[], uint8_t ucACKData[]);

static uint16_t CommSlaveConfirm(uint8_t ucCS1131Data[], uint8_t ucACKData[]);

static void CommOPCInit(void);

static void CommOPCCycle(void);

static void CommModbusInit(void);

static void CommModbusCycle(void);

static void CommP2PInit(void);

void CommP2PCycle(void);

static void CommP2PRecv( bool_t bHasCfg );

static void CommP2PRecvReq( CMController_t emCMIndex, bool_t bHasCfg);

static void CommP2PRecvResp( CMController_t emCMIndex, bool_t bHasCfg );

static bool_t GetP2PSendStartFlag( uint8_t ucBlkIndex );

static void CommP2PSend(void);

static uint16_t CommP2PPacking(uint8_t ucP2PSendIndex, ACKTimeoutFlag_t emACKTimeoutFlag);

static void CommSOEInit(void);

static void CommSOECycle(void);

static uint16_t CommSOERead(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);

static uint16_t CommSOEDelete(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);

static void CommAMSInit(void);

static void CommAMSCycle(void);

static void CommAperiodicInit(void);

static void CommAperiodicCycle(void);

static void CommSlaveFWUpdate(void);

static void CommGetSlaveVern(void);

static void CommSlaveVernHandle(uint8_t ucSlaveVernData[], uint8_t ucSlaveNum);

static void CommAlarmCycle(void);

static void CommBIPWInfoHandle(uint8_t ucBIPWInfoData[], uint8_t ucRackID);

static void CommDiagInit(void);

static void CommDiagCycle(void);

static uint16_t CommFWUpdateStart(uint8_t ucDiagData[],uint8_t ucACKData[]);

static uint16_t CommFWUpdateContinue(uint8_t ucDiagData[],uint8_t ucACKData[]);

static uint16_t CommFWUpdateDone(uint8_t ucDiagData[],uint8_t ucACKData[]);

static uint16_t CommLOGRead(uint8_t ucLOGData[],uint8_t ucACKData[]);

static uint16_t CommDiagInfoClear(uint8_t ucDiagData[],uint8_t ucACKData[]);

static void CommInterCMDInit(void);

static void CommInterCMDCycle(void);

static uint16_t CommDownloadStart (uint8_t ucCS1131Data[], uint8_t ucACKData[]);

static void CommCtrlSrvInit(void);


static void CommCtrlSrvCycle(void);

FileOptRest_t CommEraseFlash(NorFlashOptType_t emNorFlashOptType,uint32_t uiFWUsedArea);

static void CommPMUpdate(void);


//static void CommAMSTest(void);


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
    ucRTDataBackupCount = 1;
    
    CommCS1131Init();
    
    CommOPCInit();
    
    CommModbusInit();
    
    CommP2PInit();
    
    CommSOEInit();
    
    CommAMSInit();
    
    CommAperiodicInit();
    
    CommDiagInit();
    
    CommInterCMDInit();
    
    CommCtrlSrvInit();
    
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
    uint32_t uiCtrl = 0;
    uint32_t uiStartTime = SysGetSyncTimeLow32();
    


    /* 根据同步时钟时间决定执行哪个分支程序 */
    uiCtrl = uiStartTime / TASK_PERIOD;
    
    if((uiCtrl % 10) == 0)
    {
        /*1131通讯处理*/
        CommCS1131Cycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 1)
    {
        /*OPC通讯处理*/
        CommOPCCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 2)
    {
        /*Modbus通讯处理*/
        CommModbusCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 3)
    {
        /*SOE通讯处理*/
        CommSOECycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 4)
    {
        /*AMS通讯处理*/
        CommAMSCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 5)
    {
        /*诊断软件通讯处理*/
        CommDiagCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 6)
    {
        /*内部命令通讯处理*/
        CommInterCMDCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 7)
    {
        /*CTRL命令通讯处理*/
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else if((uiCtrl % 10) == 8)
    {
        /*部分非周期通讯处理*/
        CommAperiodicCycle();
        
        CommCtrlSrvCycle();
        
        CommPMUpdate();
        
        CommSlaveFWUpdate();

    }
    else
    {
        CommPMUpdate();
        
        CommCtrlSrvCycle();
        
        CommSlaveFWUpdate();

    }
    


    /*P2P通讯处理*/
    //CommP2PCycle();

    return;
}
/*
********************************函数描述*******************************************************************************
* 函数名称: CommRtDataTransferCycle
*
* 功能描述: 周期传输实时数据
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
void CommRtDataTransferCycle(void)
{
    bool_t bRet = false;
    uint32_t uiOffset = 0U;
    uint32_t uiUsedDataLen = 0U;
    uint32_t uiAreaUsedLen = 0U;
    Task_ID_t emTaskID = TASK_UP1;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;

    if((HAS_BE_CONFIGURED == SysGetIsConfigured()) &&
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP1)) &&
       (DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(TASK_UP2)))
    {
        for(emTaskID = TASK_UP1; emTaskID < MAX_TASK_NUM; emTaskID++)
        {
            pstTaskConfigInfo = SharedGetTaskConfigInfo(emTaskID);

            if((pstTaskConfigInfo != NULL) && (ACTIVE == pstTaskConfigInfo->emActiveFlag))
            {
//                    printf("Copy RT Data\n");

                uiOffset = pstTaskConfigInfo->uiGOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiGUsedSize;
                uiAreaUsedLen = uiUsedDataLen;

//                    printf("G: %d %d\n",uiOffset,uiUsedDataLen);

                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_G_AREA_LEN)
                {
                    if(TASK_UP1 == emTaskID)
                    {
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP1_G_AREA, uiOffset, uiUsedDataLen, uiAreaUsedLen);
                    }
                    if(TASK_UP2 == emTaskID)
                    {
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP2_G_AREA, uiOffset, uiUsedDataLen, uiAreaUsedLen);
                    }

                    if(false == bRet)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }

                uiOffset = pstTaskConfigInfo->uiIOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiIUsedSize;

                uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN;

//                    printf("I: %d %d\n",uiOffset,uiUsedDataLen);


                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_I_AREA_LEN)
                {
                    if(uiUsedDataLen > 0)
                    {
                        uiAreaUsedLen = uiOffset + uiUsedDataLen;
                        
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_I_AREA, uiOffset, uiUsedDataLen, uiAreaUsedLen);
    
                        if(false == bRet)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }

                uiOffset = pstTaskConfigInfo->uiOOffset;
                uiUsedDataLen = pstTaskConfigInfo->uiOUsedSize;

                uiOffset = uiOffset - MAX_RTDATA_G_AREA_LEN - MAX_RTDATA_I_AREA_LEN;

//                    printf("Q: %d %d\n",uiOffset,uiUsedDataLen);

                if((uiOffset + uiUsedDataLen) < MAX_RTDATA_Q_AREA_LEN)
                {
                    if(uiUsedDataLen > 0)
                    {
                        uiAreaUsedLen = uiOffset + uiUsedDataLen;
                        
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_Q_AREA, uiOffset, uiUsedDataLen, uiAreaUsedLen);
    
                        if(false == bRet)
                        {
                            break;
                        }
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
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_APPEND_AREA, 0, sizeof(SharedAppend_t),0);
            SysSetCopyRtDataFinished();

            //printf(" @@ SysSetCopyRtDataFinished @@ \n");

        }
    }
    else
    {

    }

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
        s_stP2PSendCtrl[uiP2PBlockNum].emWaitACKFlag = NOT_WAIT_ACK;
        s_stP2PSendCtrl[uiP2PBlockNum].usWaitACKCount = 0U;
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
void CommCS1131Cycle(void)
{
    CMController_t emCMIndex = CM1;                     /*CM 索引*/
    uint16_t usBuffIndex = 0U;                          /*数组内地址索引*/
    uint8_t ucACKData[1024*5];
    static MSGHead_t stMSGHead;                         /*CS1131消息头部*/
    uint16_t CS1131DataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* CS1131数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* CS1131数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上CS1131服务信息         */
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */
    SyncTime64_t stSyncCurTime;                         /* 时间结构体               */
    bool_t bRet = false;
    static uint8_t s_ucTimeout[NUM_OF_CM] = {0,0,0,0};
    ACKHead_t* pstACKHead = NULL;
    uint32_t uiCalcCRC = 0U;
    static uint8_t ucSendACKFlg[NUM_OF_CM] = {0,0,0,0};

    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */
    
    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间             */

    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
//        memset(ucACKData, 0x00,1024*5);

        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,CS1131_SRV);

        if(pstSysSrvInfo == NULL)
        {
            
        }
        else if(SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag)
        {/*当服务处理标志为SRV_NEED_PROCESS时*/
            
            switch(pstSysSrvInfo->stSysSrvData.usSrvNo)
            {/*根据服务类型，进入对应服务处理函数继续处理文件操作*/
               case CS1131_WRITEFILE_START:
               {
                   /*process 1131 write file start*/
                    usACKDataLen = CommWriteFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength, \
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;

               case CS1131_WRITEFILE_CONT:
               {
                   /*process 1131 write file continue*/

                    usACKDataLen = CommWriteFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength,\
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;

               case CS1131_WRITEFILE_END:
               {
                   /*process 1131 write file end*/
                    usACKDataLen = CommWriteFileDone(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength,\
                            ucACKData);
                    emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
               }
               break;
               case CS1131_DOWNLOAD_START:
               {
                   if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                           (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                   {
                       /*process 1131 CS1131_DOWNLOAD_START*/
                        
                       pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                        
                        usACKDataLen = CommDownloadStart (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                   }
               }

               break;
               
               case CS1131_START_RUN:
               case CS1131_STOP_RUN:
               case CS1131_PAUSE_RUN:
               case CS1131_WRITE_VAR:
               case CS1131_SINGLE_STEP:
               case CS1131_FORCE_VAR:
               case CS1131_DISABLE_VAR:
               case CS1131_OSC_START:
               case CS1131_OSC_DONE:
               {

                   if(NEED_ACK == pstSysSrvInfo->stSysSrvInfoHead.emAckFlag)
                   {
                       /*从共享内存上获取应答数据*/
                       SysGetSMMsg(SM_CS1131_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);
    
                       if(usACKDataLen > 0)
                       {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                           ucCS1131ACKTimeoutCount[emCMIndex] = 0;
                           
                           pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                       }
                       else
                       {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
                           ucCS1131ACKTimeoutCount[emCMIndex]++;
                           if(CS1131_ACK_TIMEOUT == ucCS1131ACKTimeoutCount[emCMIndex])
                           {
                               
                               ucCS1131ACKTimeoutCount[emCMIndex] = 0;
                               
                               pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                               
                               pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                               
                               printf("GetSMMsg TIMEOUT\n ");
                           }
                       }
                   }
                   else
                   {
                       if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                               (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                       {
                           s_ucTimeout[emCMIndex]++;

                           bRet = SysSetSMMsg(SM_CS1131_REQ, emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                   pstSysSrvInfo->stSysSrvData.uiSrvLen);
                           if(true == bRet)
                           {
                               pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;
                               
                               //printf("SetSMMsg OK\n ");
                               
                               s_ucTimeout[emCMIndex] = 0;
                           }
                           
                           if(s_ucTimeout[emCMIndex] == 10)
                           {
                               pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                               
                               pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                               
                               printf("SetSMMsg TIMEOUT \n ");
                           }
                       }
                   }
               }
               break;

               default:
               break;
            }
        }
        else
        {/*ProcessFlag is SRV_NOT_NEED_PROCESS*/
            /*从系统资源获取来自CM的服务数据*/

            memset((puint8_t)pstSysSrvInfo, 0x00, 100);

            SysGetMsg(emCMIndex,CS1131_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&CS1131DataLen);

            if(CS1131DataLen > 0)
            {/*数据长度大于0，表示收到数据*/
                 /*获取消息头部*/
                 memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                 memcpy(&stMSGHead, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], sizeof(MSGHead_t));
                 /*记录服务数据相关信息*/
                 pstSysSrvInfo->stSysSrvData.uiSrvLen = CS1131DataLen;
                 pstSysSrvInfo->stSysSrvData.usSrvNo = stMSGHead.usActualType;

                 uiCalCrc = SysCrc32Cal(0x00, (puint8_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[0], stMSGHead.usLength);
                 usBuffIndex = stMSGHead.usLength;
                 memcpy((puint8_t)&uiReadCrc,(puint8_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex],CHKSUM_CRC_SIZE);

                 if(uiCalCrc != uiReadCrc)
                 {/*CRC 不一致*/
                     /*log*/
                     DEBUG_COMM("CalCrc != ReadCrc\n");
                 }
                 else if(CS1131DataLen != stMSGHead.usLength + CHKSUM_CRC_SIZE)
                 {/*数据长度不一致*/
                     /*log*/
                     DEBUG_COMM("DataLen err\n");
                 }
                 else if((KS_PROGRAM != SharedGetSwitchPos()) && (pstSysSrvInfo->stSysSrvData.usSrvNo != CS1131_READ_DISABLE_TABL))
                 {
                     /*produce ACK data*/
                     pstACKHead = (ACKHead_t*)&ucACKData[0];
                     pstACKHead->ucType = stMSGHead.ucProtect;
                     pstACKHead->ucProtect = stMSGHead.ucType;
                     pstACKHead->usActualType = stMSGHead.usActualType;
                     pstACKHead->ucNodeNum = stMSGHead.ucNodeNum;
                     pstACKHead->usSequenceNum = stMSGHead.usSequenceNum;
                     pstACKHead->ucDataFlg = 0U;
                     pstACKHead->ucFlag = ACK_FRAME_SINGLE;

                     pstACKHead->ucResCode = RESCODE_FAILED;
                     
                     pstACKHead->usLength = (uint16_t)(sizeof(ACKHead_t));
                     usACKDataLen = pstACKHead->usLength;
                     pstACKHead->ucSubReason = ERR_NOT_PROGRAM;
                     /*计算并填充数据应答的CRC*/
                     uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                     memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                     usACKDataLen += sizeof(uiCalcCRC);
                     
                     ucSendACKFlg[emCMIndex] = NEED_SEND_ACK;
                 }
                 else
                 {
                     ucSendACKFlg[emCMIndex] = NEED_SEND_ACK;
                     DEBUG_COMM("SrvNo %d\n",pstSysSrvInfo->stSysSrvData.usSrvNo);
                     switch(pstSysSrvInfo->stSysSrvData.usSrvNo)
                     {/*根据服务号，处理1131服务*/
                         /*these service need to synchronize*/
                        case CS1131_START_RUN:
                        case CS1131_STOP_RUN:
                        case CS1131_PAUSE_RUN:
                        case CS1131_WRITE_VAR:
                        case CS1131_DOWNLOAD_START:
                        case CS1131_SINGLE_STEP:
                        case CS1131_FORCE_VAR:
                        case CS1131_DISABLE_VAR:
                        case CS1131_OSC_START:
                        case CS1131_OSC_DONE:
                        {/*将服务进行同步*/

                            if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_START_RUN)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_START_RUN);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_STOP_RUN)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_STOP_RUN);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_PAUSE_RUN)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_PAUSE_RUN);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_WRITE_VAR)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_WRITE_VAR);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_SINGLE_STEP)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_SINGLE_STEP);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_FORCE_VAR)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_FORCE_VAR);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_DISABLE_VAR)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_DISABLE_VAR);
                            }
                            else if(pstSysSrvInfo->stSysSrvData.usSrvNo == CS1131_DOWNLOAD_START)
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_DOWNLOAD_START);
                            }
                            else
                            {
                                
                            }
                            
                            if(CS1131_DOWNLOAD_START == pstSysSrvInfo->stSysSrvData.usSrvNo)
                            {
                                SharedSetDownloadPM(SysGetLocalPMID());

                                if(HAS_BE_CONFIGURED == SysGetIsConfigured())
                                {
                                    SysSetDownloadAllStartFlag();
                                    SharedBackupOldCMSlotIdTabl();
                                    //printf("start backup table\n");
                                }

                                SysSetRtDataAreaWriteOnly();
                                printf("write only\n");

                            }
                            
                            SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                            SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                              &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                            
                            stSyncTask.emTaskId = SYS_BROADCAST_CS1131SRV;
                            stSyncTask.emPMId = INVALID_PM_ID;
                            stSyncTask.emCMId = emCMIndex;
                            stSyncTask.emTaskStatus = TASK_SEND;
                            stSyncTask.emTaskWait = TASK_NOWAIT;
                            pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                            /*暂定为：非稳态时也可以处理服务。*/
                            SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                                                   
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                        }
                        break;

                        case CS1131_CHECK_EXTERLIB:
                        {
                            /*process 1131 check exlibvern*/
                            LogWrite(LOG1_ID_USER_L0_CS1131_CHECK_EXTERLIB);

                        }
                        break;

                        case CS1131_READ_PRJINFO:
                        {
                            /*process 1131 read ProjectInfo*/
                            LogWrite(LOG1_ID_USER_L0_CS1131_READ_PRJINFO);
                        }
                        break;

                        case CS1131_READ_FORCE:
                        {
                            /*process 1131 read Forcelist*/
                            
                            LogWrite(LOG1_ID_USER_L0_CS1131_READ_FORCE);
                        }
                        break;
                        
                        case CS1131_READ_DISABLE_TABL:
                        {
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommReadDisableTabl(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_DOWNLOAD_COMFIR:
                        {
                            /*process 1131 Download end service*/
                            
                            LogWrite(LOG1_ID_USER_L0_CS1131_DOWNLOAD_COMFIR);

                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                            usACKDataLen = CommDownloadComfirm(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                        }

                        break;

                        case CS1131_WRITEFILE_START:
                        {
                            /*process 1131 write file start*/
                            
                            //printf("RECV WRITEFILE_ST\n");
                            LogWrite(LOG1_ID_USER_L0_CS1131_WRITEFILE_START);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommWriteFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_CONT:
                        {
                            
                            //printf("RECV WRITEFILE_CONT\n");
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                            /*process 1131 write file continue*/
                             usACKDataLen = CommWriteFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_END:
                        {
                            
                            //printf("RECV WRITEFILE_END\n");
                            LogWrite(LOG1_ID_USER_L0_CS1131_WRITEFILE_END);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                            /*process 1131 write file end*/
                             usACKDataLen = CommWriteFileDone(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_READFILE_START:
                        {
                            /*process 1131 read file start*/
                            LogWrite(LOG1_ID_USER_L0_CS1131_READFILE_START);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommReadFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READFILE_CONT:
                        {
                            /*process 1131 read file continue*/
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommReadFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;
                        
                        case CS1131_OSC_GET_VAR:
                        {
                            /*process 1131 get SOC var*/
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommOSCGetVar(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                        }
                        break;
                        
                        case CS1131_SLAVE_CONFIRM:
                        {
                            /*process 1131 Slave Confirm*/
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommSlaveConfirm(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                        }
                        break;

                        default:
                            
                            ucSendACKFlg[emCMIndex] = NOT_NEED_SEND_ACK;
                            //printf("RECV ERR SRV: %d\n",pstSysSrvInfo->stSysSrvData.usSrvNo);
                            
                        break;
                     }
                 }
             }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            if(NEED_SEND_ACK == ucSendACKFlg[emCMIndex])
            {
                DEBUG_COMM("Has RESP Data %d %d\n",emCMIndex, usACKDataLen);
                SysSetMsg(emCMIndex,CS1131_RESP,ucACKData,usACKDataLen);
            }
            else
            {
                DEBUG_COMM("SEND_ACK_FLG_ERR %d %d\n",ucSendACKFlg[emCMIndex],emCMIndex);
            }
            if(emFileHandleFlg[emCMIndex] != FILE_HANDLE_FREE)
            {
                emFileHandleFlg[emCMIndex] = FILE_HANDLE_FREE;
            }
            
            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            
            usACKDataLen = 0U;
            
            ucSendACKFlg[emCMIndex] = NOT_NEED_SEND_ACK;
        }
    }
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommReadDisableTabl
*
* 功能描述: 获取Disable表函数
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
static uint16_t CommReadDisableTabl(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    DisableTablHead_t stDisableTablHead;
    uint32_t uiCheckSumCal = 0U;
    uint16_t usDisableTablLen = 0U;
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;

    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {/*获取Disable表*/

        memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
        
        SharedGetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
        
        usDisableTablLen = (uint16_t)stDisableTablHead.uiSize;
        
        printf("DisTab len %d\n",stDisableTablHead.uiSize);
        printf("DisTab ItemNum %d\n",stDisableTablHead.uiItemNum);
        
        if((usDisableTablLen >= sizeof(DisableTablHead_t)) && (usDisableTablLen <= RETAIN_SRAM_DISABLE_SIZE))
        {
            uiACKDataIndex = sizeof(ACKHead_t);
            
            memcpy(&ucACKData[uiACKDataIndex], &usDisableTablLen, sizeof(uint16_t));
            
            uiACKDataIndex += sizeof(uint16_t);
            
            if((usDisableTablLen % 2) != 0)
            {
                usDisableTablLen = usDisableTablLen + 1;
            }
            SharedGetDisableTabl(&ucACKData[uiACKDataIndex], usDisableTablLen);
            
            /*计算文件总CRC*/
            uiCheckSumCal = SysCrc32Cal(0x00, (puint8_t)(&ucACKData[uiACKDataIndex + sizeof(uint32_t)]), \
                    stDisableTablHead.uiSize - sizeof(uint32_t));
            if(uiCheckSumCal != stDisableTablHead.uiCheckSum)
            {
                /*log*/
                ucSubSeason = ERR_DISABLE_TABL_CRC;
            }
        }
        else
        {
//            ucSubSeason = ERR_DISABLE_TABL_LEN;
            usDisableTablLen = 0U;
            
            uiACKDataIndex = sizeof(ACKHead_t);
            memcpy(&ucACKData[uiACKDataIndex], &usDisableTablLen, sizeof(uint16_t));
            
            memset(&stDisableTablHead, 0x00, sizeof(DisableTablHead_t));
            SharedSetDisableTabl(&stDisableTablHead, sizeof(DisableTablHead_t));
        }
        
        /*produce ACK data*/
//        DEBUG_COMM(" produce ACK data \n");
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucCS1131Data;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        uiACKDataIndex = 0U;
        uiACKDataIndex += sizeof(ACKHead_t);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;

            uiACKDataIndex += sizeof(uint16_t);
            uiACKDataIndex += stDisableTablHead.uiSize;
            pstACKHead->ucDataFlg = 1U;
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
static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[],uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0;
    uint32_t uiCalcCRC = 0;
    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
    uint8_t ucUserComfirmFlg = 0U;
    PrjInfo_t *pstLocalDLPrjInfo = NULL;                        /* 本机下装工程信息数据指针     */
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本PMID号           */

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

        ucUserComfirmFlg = ucCS1131Data[16];
        if(ucUserComfirmFlg != 0U)
        {
            printf("User Comfirm err %d\n",ucUserComfirmFlg);
        }
        else
        {
            printf("User Comfirm ok\n",ucUserComfirmFlg);
            pstLocalDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
            if(pstLocalDLPrjInfo != NULL)
            {
                pstLocalDLPrjInfo->ucIsConfirmed = HAS_BE_CONFIRMED;
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
        uiACKDataIndex = 0;
        uiACKDataIndex += sizeof(ACKHead_t);
//        if(ucSubSeason != ERR_SUCCESS)
//        {/*服务处理失败*/
//            pstACKHead->ucResCode = RESCODE_FAILED;
//        }
//        else
//        {/*服务处理成功*/
//            pstACKHead->ucResCode = RESCODE_SUCCESS;
//        }
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        
        
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
FileOptRest_t CommDetermineFileExist(uint8_t  ucFileName[])
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
       
    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送判断文件是否存在命令*/
            uiTimeCount = 0;
            SysSendIsFileExist(ucFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答，若收到应答，解析应答结果，并返回；若超时，返回超时*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                //printf("StatusWord=0x%x\n",stAckHeader.uiStatusWord);
                //printf("ErrCode=0x%x\n",stAckHeader.uiErrCode);

                if(FM_IS_FILE_EXIST_ACK == (stAckHeader.uiStatusWord & FM_IS_FILE_EXIST_ACK))
                {/*操作成功，且文件存在；返回结果*/
                    emFileOptRest = FILE_EXIST;
                    emProcessStag = FOS_START;
                }
                else
                {/*文件不存在；返回结果*/
                    emFileOptRest = FILE_NOT_EXIST;
                    emProcessStag = FOS_START;
                }
            }
            else
            {
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
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
FileOptRest_t CommOpenFile(uint8_t ucFileName[],pint32_t piFd)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送打开文件命令*/
            uiTimeCount = 0;
            SysOpenFile(ucFileName);
            emProcessStag = FOS_WAIT_ACK;
            emFileOptRest = NOT_COMPLETE;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                //printf("stw=0x%x fd=%d\n", stAckHeader.uiStatusWord, stAckHeader.iFd);
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
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
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
FileOptRest_t CommWriteFile(int32_t iFd,uint8_t ucFileData[],uint16_t usFileDataLen)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
    bool_t bRet = false;

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送写文件命令*/
            uiTimeCount = 0;
            bRet = SysWriteFile(iFd, ucFileData, usFileDataLen);
            //printf("bRet=%d fd=%d len=%d\n",bRet,  iFd, usFileDataLen);
            if(true == bRet)
            {
                emProcessStag = FOS_WAIT_ACK;
                emFileOptRest = NOT_COMPLETE;
            }
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                //printf("wf uiStatusWord=0x%x",stAckHeader.uiStatusWord);
                if((FM_WRITE_FILE_ACK == (stAckHeader.uiStatusWord&FM_WRITE_FILE_ACK)))
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
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
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
FileOptRest_t CommCloseFile(int32_t iFd)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送关闭文件命令*/
            uiTimeCount = 0;
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
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
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
FileOptRest_t CommDeleteFile(uint8_t ucFileName[])
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送删除文件命令*/
            uiTimeCount = 0;
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
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
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
FileOptRest_t CommRenameFile(uint8_t ucOldFileName[], uint8_t ucNewFileName[])
{
    static FileOperateStag_t s_emProcessStag = FOS_START;
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
    bool_t bRet = false;

    switch(s_emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送重命名文件命令*/
            uiTimeCount = 0;
            bRet = SysRenameFile(ucOldFileName, ucNewFileName);
            //printf("rename ret=%d\n", bRet);
            emFileOptRest = NOT_COMPLETE;
            s_emProcessStag = FOS_WAIT_ACK;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                //printf("rename stw=%d\n", stAckHeader.uiStatusWord);
                if((FM_RENAME_FILE_ACK == stAckHeader.uiStatusWord))
                {/*重命名文件成功，返回操作成功*/
                    emFileOptRest = SUCCESS;
                    s_emProcessStag = FOS_START;
                }
                else
                {/*重命名文件失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    s_emProcessStag = FOS_START;
                }
            }
            else
            {
                uiTimeCount++;
                if(uiTimeCount >= FS_OPERATE_TIMEOUT )
                {/*等待超时，返回操作失败*/
                    emFileOptRest = FAIL;
                    s_emProcessStag = FOS_START;
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
* 函数名称: CommFileInfoInit
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
static void CommFileInfoInit(void)
{
    SysCfgFileType_t emCfgFileType = CFG_CTRL_STATION;

    for(emCfgFileType = CFG_CTRL_STATION; emCfgFileType < NUM_OF_CFG_FILE_TYPE; emCfgFileType++)
    {
        stFileInfo[emCfgFileType].emFileType = emCfgFileType;
        stFileInfo[emCfgFileType].iFid = -1;
        stFileInfo[emCfgFileType].usFileNameLen = 0U;
        stFileInfo[emCfgFileType].uiFileDataLen = 0U;
        stFileInfo[emCfgFileType].uiFileWriteIndex = 0U;
        stFileInfo[emCfgFileType].uiFileReadIndex = 0U;
        
        SysGetFileName(emCfgFileType, (pchar_t)&stFileInfo[emCfgFileType].ucFileName[0]);
        SysGetTmpFileName(emCfgFileType, (pchar_t)&stFileInfo[emCfgFileType].ucTmpFileName[0]);
        SysGetOldFileName(emCfgFileType, (pchar_t)&stFileInfo[emCfgFileType].ucOldFileName[0]);
    }

    return;
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
FileInfo_t* CommGetFileInfo(SysCfgFileType_t emCfgFileType)
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
    static FileWriteInfo_t stFileWriteInfo;                                 /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    static SysCfgFileType_t emCfgFileType = 0U;
    
    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_START;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_START;
    }
    else
    {
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
                
                DEBUG_COMM("FileID: %x\n",stFileWriteInfo.ucFileID);
                
                usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
                
                if((stFileWriteInfo.ucFileID == 0U) || (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
                {
                    ucSubSeason = ERR_WRITE_FILE_START;
                    emProcessStag = FWRITE_END;
                }
                else
                {
                    /*将1131下发的文件ID，转化为PM使用的文件类型*/
                    emCfgFileType = stFileWriteInfo.ucFileID - 1U;

                    memcpy(&stFileWriteInfo.usFileDataLen, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.usFileDataLen));
                    
    //                DEBUG_COMM("FileLen %x \n",stFileWriteInfo.usFileDataLen);
                    
                    usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);
                    
                    /*根据文件类型，获取文件的控制结构*/
                    pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                                    
    //                DEBUG_COMM("CfgFileType is : %x \n",emCfgFileType);

                    if((stFileWriteInfo.usFileDataLen > MAX_WRITE_FILE_LEN) && (NULL == pstFileInfo))
                    {
                        /*file name is error ,file info is not exist*/
                        ucSubSeason = ERR_WRITE_FILE_START;
                        emProcessStag = FWRITE_END;
                    }
                    else
                    {/*save the data in memory*/
                        
    //                    DEBUG_COMM("save the data in memory \n");
                        
                        /*获取内存上文件信息*/
                        SysGetCfgFileState(emCfgFileType, &stCfgFileState);
        
                        /*将文件数据写入内存*/
//                        printf("usFileDataLen is : %d \n",stFileWriteInfo.usFileDataLen);
                        /*写文件开始服务，写入DDR前清一遍内存写片区*/
                        SysDeleteCfgFile(emCfgFileType);
                                            
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                stFileWriteInfo.usFileDataLen);
                        if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                        {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                            ucSubSeason = ERR_WRITE_FILE_START;
                            emProcessStag = FWRITE_END;
                            
                            DEBUG_COMM("WritLen: %d FileLen: %d \n", \
                                    usWritDataLen,stFileWriteInfo.usFileDataLen);
                        }
                        else
                        {/*写入成功*/
                            emProcessStag = FWRITE_DETER_TMPFILE_EXIST;
                            DEBUG_COMM("save file ok\n");
                        }
                    }
                }
            }
            break;
    
            case FWRITE_DETER_TMPFILE_EXIST:
            {/*完成判断文件是否存在操作*/
                ucFileOptRst = CommDetermineFileExist(&pstFileInfo->ucTmpFileName[0]);
                //printf("1 file exist?=%d\n", ucFileOptRst);
                //printf("name =%s\n", &pstFileInfo->ucTmpFileName[0]);
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
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
                //printf("2 erase=%d\n", ucFileOptRst);
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
                //printf("3 FileDataLen=%d\n",stFileWriteInfo.usFileDataLen);
                ucFileOptRst = CommOpenFile(&pstFileInfo->ucTmpFileName[0], &pstFileInfo->iFid);
                //printf("3 open tmp file=%d\n", ucFileOptRst);
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/
                    emProcessStag = FWRITE_WRITE_TMPFILE;
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
                usBuffIndex = 0U;
                usBuffIndex += sizeof(MSGHead_t);
                usBuffIndex += sizeof(stFileWriteInfo.ucFileType);
                usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
                usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);

                //printf("4 FileDataLen=%d\n",stFileWriteInfo.usFileDataLen);
                ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucCS1131Data[usBuffIndex], stFileWriteInfo.usFileDataLen);
                //printf("fd=%d len=%d\n",pstFileInfo->iFid,stFileWriteInfo.usFileDataLen);
                //printf("4 write tmp file=%d\n", ucFileOptRst);
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
                //printf("5 close tmp file=%d\n", ucFileOptRst);
                if(FAIL == ucFileOptRst)
                {/*操作失败，删除文件*/
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                }
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                }
                
            }
            break;
    
            case FWRITE_DELETE_TMPFILE:
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
                if(FAIL == ucFileOptRst)
                {/*操作失败，应答错误信息*/
                    /*record error log*/
                    
                    ucSubSeason = ERR_WRITE_FILE_START;
                    emProcessStag = FWRITE_END;
                }
                if(SUCCESS == ucFileOptRst)
                {/*操作失败，应答错误信息*/
                    /*record error log*/
                    
                    ucSubSeason = ERR_WRITE_FILE_START;
                    emProcessStag = FWRITE_END;
                }
    
            }
            break;
    
            case FWRITE_END:
            {/*服务处理结束，产生应答*/
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
                
    //            DEBUG_COMM("ACK:  ucSubSeason = %d \n",ucSubSeason);
    //            DEBUG_COMM("ACK:  pstACKHead->ucResCode = %d \n",pstACKHead->ucResCode);
                
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
static uint16_t CommWriteFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    static FileWriteStag_t emProcessStag = FWRITE_START;                    /*写文件开始服务处理流程*/
    static FileWriteInfo_t stFileWriteInfo;                                 /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    static SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_START;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_START;
    }
    else
    {
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
                
                if((stFileWriteInfo.ucFileID == 0U) || (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
                {
                    ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                    emProcessStag = FWRITE_END;
                    
                    printf("FIleID err: %x\n",stFileWriteInfo.ucFileID);
                }
                else
                {
                    /*将1131下发的文件ID，转化为PM使用的文件类型*/
                    emCfgFileType = stFileWriteInfo.ucFileID - 1U;
                    
                    DEBUG_COMM("FileID: %x\n",stFileWriteInfo.ucFileID);
                    
                    
                    memcpy(&stFileWriteInfo.usFileDataLen, &ucCS1131Data[usBuffIndex], sizeof(stFileWriteInfo.usFileDataLen));
                    usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);
                    
    //                DEBUG_COMM("FileDataLen : %x \n",stFileWriteInfo.usFileDataLen);

                    /*根据文件名字，获取文件的控制结构*/
                    pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                    
    //                DEBUG_COMM("----CommWriteFileContinue: emCfgFileType is : %x \n",emCfgFileType);

                    if((stFileWriteInfo.usFileDataLen > MAX_WRITE_FILE_LEN) && (NULL == pstFileInfo))
                    {
                        /*file data len is error ,file info is not exist*/
                        ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                        emProcessStag = FWRITE_END;
                        
                        DEBUG_COMM("pstFileInfo NULL");
                        
                    }
                    else
                    {/*save the data in memory*/
                        /*获取内存上文件信息*/
                        SysGetCfgFileState(emCfgFileType, &stCfgFileState);
    
                        /*将文件数据写入内存*/
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                stFileWriteInfo.usFileDataLen);
                        if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                        {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                            ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                            emProcessStag = FWRITE_END;
                            
                            DEBUG_COMM("WritLen: %d FileLen: %d \n", \
                                    usWritDataLen,stFileWriteInfo.usFileDataLen);
                        }
                        else
                        {/*写入成功*/
                            emProcessStag = FWRITE_WRITE_TMPFILE;//FWRITE_END;
                            DEBUG_COMM("save file ok\n");
                        }
                    }
                }
            }
            break;
    
            case  FWRITE_WRITE_TMPFILE:
            {
                usBuffIndex = 0U;
                usBuffIndex += sizeof(MSGHead_t);
                usBuffIndex += sizeof(stFileWriteInfo.ucFileType);
                usBuffIndex += sizeof(stFileWriteInfo.ucFileID);
                usBuffIndex += sizeof(stFileWriteInfo.usFileDataLen);
                
                if(pstFileInfo->iFid != -1)
                {
                    ucFileOptRst = CommWriteFile(pstFileInfo->iFid, &ucCS1131Data[usBuffIndex], stFileWriteInfo.usFileDataLen);
                    if(SUCCESS == ucFileOptRst)
                    {/*文件操作成功，进行下一步*/
                        emProcessStag = FWRITE_END;
                        ucSubSeason = ERR_SUCCESS;
                    }
                    if(FAIL == ucFileOptRst)
                    {/*操作失败，关闭文件，等待删除，并应答错误信息*/
                        ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                        emProcessStag = FWRITE_CLOSE_TMPFILE;
                    }
                }
                else
                {
                    ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                    emProcessStag = FWRITE_END;
                }
            }
            break;
    
            case FWRITE_CLOSE_TMPFILE:
            {
                ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
                if(FAIL == ucFileOptRst)
                {/*操作失败，删除文件*/
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                }
                
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                }
                
            }
            break;
    
            case FWRITE_DELETE_TMPFILE:
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
                if(FAIL == ucFileOptRst)
                {/*操作失败，应答错误信息*/
                    /*record error log*/
                    
                    ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                    emProcessStag = FWRITE_END;
                }
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功，进行下一步*/
                    ucSubSeason = ERR_WRITE_FILE_CONTINUE;
                    emProcessStag = FWRITE_END;
                }
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
static uint16_t CommWriteFileDone(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    static FileWriteStag_t emProcessStag = FWRITE_START;                    /*写文件开始服务处理流程*/
    FileWriteInfo_t stFileWriteInfo;                                        /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    static FileInfo_t* pstFileInfo = NULL;
    FileOptRest_t ucFileOptRst = 0;                                         /*文件操作结果*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usWritDataLen = 0U;                                            /*内存上写入数据的长度*/
    static SysCfgFileType_t emCfgFileType = 0U;

    if(NULL == ucCS1131Data)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
        emProcessStag = FWRITE_START;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
        emProcessStag = FWRITE_START;
    }
    else
    {
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
                
                if((stFileWriteInfo.ucFileID == 0U) || (stFileWriteInfo.ucFileID > NUM_OF_CFG_FILE_TYPE))
                {
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                    
                    printf("FIleID err: %x \n",stFileWriteInfo.ucFileID);
    
                }
                else
                {
                    /*将1131下发的文件ID，转化为PM使用的文件类型*/
                    emCfgFileType = stFileWriteInfo.ucFileID - 1U;
                    
                    DEBUG_COMM("FileID is: %x\n",stFileWriteInfo.ucFileID);
                    
                    stFileWriteInfo.usFileDataLen = 0U;
    
                    /*根据文件名字，获取文件的控制结构*/
                    pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
                    
    //                DEBUG_COMM("----CommWriteFileDone: emCfgFileType is : %x \n",emCfgFileType);
                    
                    if(NULL == pstFileInfo)
                    {
                        /*file name is error ,file info is not exist*/
                        ucSubSeason = ERR_WRITE_FILE_END;
                        emProcessStag = FWRITE_END;
                        
                        DEBUG_COMM("pstFileInfo NULL\n");
                        
                    }
                    else
                    {/*save the data in memory*/
                        /*获取内存上文件信息*/
                        SysGetCfgFileState(emCfgFileType, &stCfgFileState);
    
                        /*将文件数据写入内存*/
                        usWritDataLen = SysSetCfgFileInfo(emCfgFileType, &ucCS1131Data[usBuffIndex], \
                                stFileWriteInfo.usFileDataLen);
                        if(usWritDataLen != stFileWriteInfo.usFileDataLen)
                        {/*写入内存数据长度与实际文件数据长度不一致，写错误，结束服务处理*/
                            ucSubSeason = ERR_WRITE_FILE_END;
                            
                            emProcessStag = FWRITE_END;
                            
                            DEBUG_COMM("WritLen: %d FileLen: %d\n", \
                                    usWritDataLen,stFileWriteInfo.usFileDataLen);
                            
                        }
                        else
                        {/*写入成功*/
                            /*设置写文件结束*/
                            SysSetCfgFileFinished(emCfgFileType);
                            
                            if(CFG_CTRL_STATION == emCfgFileType)
                            {
                                SysSetAllCfgFileFinished();
                            }
                            emProcessStag = FWRITE_WRITE_TMPFILEDONE;//FWRITE_END;
                            DEBUG_COMM("save ok\n");
                        }
                    }
                }
            }
            break;
    
            case  FWRITE_WRITE_TMPFILEDONE:
            {
                if(pstFileInfo->iFid != -1)
                {
                    ucFileOptRst = CommCloseFile(pstFileInfo->iFid);
                    //printf("a wr tmp file=%d fd=%d\n", ucFileOptRst, pstFileInfo->iFid);
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
            }
            break;
    
            case FWRITE_DETER_FORMFILE_EXIST:
            {
                ucFileOptRst = CommDetermineFileExist(&pstFileInfo->ucFileName[0]);
                //printf("b form file exist=%d\n", ucFileOptRst);
                //printf("form name=%s\n",&pstFileInfo->ucFileName[0]);
                if(FILE_EXIST == ucFileOptRst)
                {/*文件存在，重命名文件*/
                    emProcessStag = FWRITE_RENAME_FORMFILE;
                    //printf("exist\n");
                }
                if(FILE_NOT_EXIST == ucFileOptRst)
                {/*文件不存在，重命名临时文件*/
                    emProcessStag = FWRITE_RENAME_TMPFILE;
                    //printf("no exist\n");
                }
                if(FAIL == ucFileOptRst)
                {/*操作失败,应答错误信息*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                }
            }
            break;
    
            case FWRITE_RENAME_FORMFILE: //FWRITE_DELETE_FORMFILE
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucFileName[0]);
                //printf("c rename form file=%d\n", ucFileOptRst);
                //printf("c  %s %s\n", &pstFileInfo->ucFileName[0], &pstFileInfo->ucOldFileName[0]);
                if(FAIL == ucFileOptRst)
                {/*重命名正式文件为旧文件失败，删除文件*/
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_FORMFILE;
                }
                if(SUCCESS == ucFileOptRst)
                {/*重命名正式文件为旧文件成功，将临时文件重命名为正式文件*/
                    emProcessStag = FWRITE_RENAME_TMPFILE;
                    //printf("%s  %s\n",&pstFileInfo->ucFileName[0], &pstFileInfo->ucOldFileName[0]);
                }
            }
            break;
    
            case FWRITE_RENAME_TMPFILE:
            {
                ucFileOptRst = CommRenameFile(&pstFileInfo->ucTmpFileName[0], &pstFileInfo->ucFileName[0]);
                //printf("d rename tmp file=%d\n", ucFileOptRst);
    
                if(FAIL == ucFileOptRst)
                {
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_TMPFILE;
                }
                if(SUCCESS == ucFileOptRst)
                {
                    ucSubSeason = ERR_SUCCESS;
                    pstFileInfo->uiFileDataLen = pstFileInfo->uiFileWriteIndex;
                    //emProcessStag = FWRITE_RENAME_TMPFILE;
                    emProcessStag = FWRITE_END;
                    //printf("%s  %s\n",&pstFileInfo->ucTmpFileName[0], &pstFileInfo->ucFileName[0]);
                }
            }
            break;
    
            case FWRITE_DELETE_TMPFILE:
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
                //printf("e del tmp file=%d\n", ucFileOptRst);
                //printf("delete tmp file=%s\n", &pstFileInfo->ucTmpFileName[0]);
                if(FAIL == ucFileOptRst)
                {
                    /*record error log*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                }
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                }
    
            }
            break;
    
            case FWRITE_DELETE_FORMFILE:
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucFileName[0]);
                //printf("f delete form file=%d\n", ucFileOptRst);
                if(FAIL == ucFileOptRst)
                {
                    /*record error log*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                }
                if(SUCCESS == ucFileOptRst)
                {/*文件操作成功*/
                    ucSubSeason = ERR_WRITE_FILE_END;
                    emProcessStag = FWRITE_END;
                }
    
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
                
    //            DEBUG_COMM("ACK:  ucSubSeason = %d \n",ucSubSeason);
    //            DEBUG_COMM("ACK:  pstACKHead->ucResCode = %d \n",pstACKHead->ucResCode);
                
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
static uint16_t CommReadFileStart(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;                                         /*文件控制结构*/
    FileReadInfo_t stFileReadInfo;                                          /*文件操作相关信息记录结构*/
    uint16_t usReadDataLen = 0;                                             /*读文件数据长度*/
    uint8_t ucContFlag = READ_FILE_TRANS_DONE;                              /*读文件继续标志*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucFileData[MAX_READ_FILE_LEN];                                  /*读取文件数据缓存*/
    SysCfgFileType_t emCfgFileType = 0U;
    uint16_t usReadRet = 0U;

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
        
        printf("FileID: %d\n",stFileReadInfo.ucFileID);

        /*根据文件名获取文件控制结构*/
        pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
        if(NULL == pstFileInfo)
        {
            /*file name is error ,file info is not exist*/
            ucSubSeason = ERR_READ_FILE_START;
            printf("FileInfo NULL %d\n",emCfgFileType);
        }
        else
        {
            /*获取内存上文件信息*/
            SysGetCfgFileState(emCfgFileType, &stCfgFileState);

            if(stCfgFileState.bReadable != CFG_FILE_RD_ENABLE)
            {/*文件不可写*/
             /**/
                /*log*/
                ucSubSeason = ERR_READ_FILE_START;
                printf("RD_DISABLE\n");
            }
            else
            {/*从内存上读取文件*/
                pstFileInfo->uiFileReadIndex = 0;
                printf("FileLen: %d\n",stCfgFileState.uiLen);
                if((stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex) > MAX_READ_FILE_LEN)
                {
                    usReadDataLen = MAX_READ_FILE_LEN;
                    ucContFlag = READ_FILE_TRANS_CONT;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    ucContFlag = READ_FILE_TRANS_DONE;
                }
                /*初始化文件数据读取缓存*/
//                memset(ucFileData, 0x00, MAX_READ_FILE_LEN);
                
                printf("ReadDataLen: %d\n",usReadDataLen);
                
                usReadRet = SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        &ucFileData[0], usReadDataLen);
                if(usReadDataLen != usReadRet)
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_START;
                    printf("Read err %d %d\n",usReadRet,usReadDataLen);
                }
            }
            
            if((ucSubSeason != ERR_SUCCESS) || (READ_FILE_TRANS_DONE == ucContFlag))
            {/*记录文件读取的位置*/
                pstFileInfo->uiFileReadIndex = 0U;
            }
            else
            {
                pstFileInfo->uiFileReadIndex += usReadDataLen;
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
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&ucContFlag,sizeof(uint8_t));
            usBuffIndex += sizeof(uint8_t);
            /*填充文件数据长度*/
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&usReadDataLen,sizeof(uint16_t));
            usBuffIndex += sizeof(uint16_t);
            /*填充文件数据*/
            CommBitInvert((puint8_t)(&ucFileData[0]), &ucACKData[usBuffIndex], usReadDataLen);

            usBuffIndex += usReadDataLen;
            /*填写应答数据的长度，除去头部*/
            pstACKHead->ucDataFlg = 1U;
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);
    }

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
static uint16_t CommReadFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FileInfo_t* pstFileInfo = NULL;                                         /*文件控制结构*/
    FileReadInfo_t stFileReadInfo;                                          /*文件操作相关信息记录结构*/
    uint16_t usReadDataLen = 0;                                             /*读文件数据长度*/
    uint8_t ucContFlag = READ_FILE_TRANS_DONE;                              /*读文件继续标志*/
    SysCfgFileState_t stCfgFileState;                                       /*内存上存储文件的相关信息*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucFileData[MAX_READ_FILE_LEN];                                  /*读取文件数据缓存*/
    SysCfgFileType_t emCfgFileType = 0U;
    uint16_t usReadRet = 0U;

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
        usBuffIndex += sizeof(MSGHead_t);
        /*获取要读取的文件信息*/
        memcpy(&stFileReadInfo.ucFileType, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileType));
        usBuffIndex += sizeof(stFileReadInfo.ucFileType);
        memcpy(&stFileReadInfo.ucFileID, &ucCS1131Data[usBuffIndex], sizeof(stFileReadInfo.ucFileID));
        usBuffIndex += sizeof(stFileReadInfo.ucFileID);
        
        printf("FileID: %d\n",stFileReadInfo.ucFileID);
        
        /*将1131下发的文件ID，转化为PM使用的文件类型*/
        emCfgFileType = stFileReadInfo.ucFileID - 1U;
        
        /*根据文件名获取文件控制结构*/
        pstFileInfo = (FileInfo_t*)CommGetFileInfo(emCfgFileType);
        if(NULL == pstFileInfo)
        {
            /*file name is error ,file info is not exist*/
            ucSubSeason = ERR_READ_FILE_CONTINUE;
            printf("FileInfo NULL %d\n",emCfgFileType);
        }
        else
        {
            /*获取内存上文件信息*/
            SysGetCfgFileState(emCfgFileType, &stCfgFileState);

            if(stCfgFileState.bReadable != CFG_FILE_RD_ENABLE)
            {/*文件不可写*/
                /*log*/
                ucSubSeason = ERR_READ_FILE_CONTINUE;
                printf("RD_DISABLE\n");
            }
            else
            {/*从内存上读取文件*/
                if((stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex) > MAX_READ_FILE_LEN)
                {
                    usReadDataLen = MAX_READ_FILE_LEN;
                    ucContFlag = READ_FILE_TRANS_CONT;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    ucContFlag = READ_FILE_TRANS_DONE;
                    
                }
                /*初始化文件数据读取缓存*/
                
                printf("ReadLen: %d ReadIndex %d\n",usReadDataLen, pstFileInfo->uiFileReadIndex);

                usReadRet = SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        ucFileData, usReadDataLen);
                if(usReadDataLen != usReadRet)
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_CONTINUE;
                    printf("Read err %d %d\n",usReadRet,usReadDataLen);
                }
            }
            
            if((ucSubSeason != ERR_SUCCESS) || (READ_FILE_TRANS_DONE == ucContFlag))
            {/*记录文件读取的位置*/
                pstFileInfo->uiFileReadIndex = 0U;
            }
            else
            {
                pstFileInfo->uiFileReadIndex += usReadDataLen;
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
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&ucContFlag,sizeof(uint8_t));
            usBuffIndex += sizeof(uint8_t);
            /*填充文件数据长度*/
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&usReadDataLen,sizeof(uint16_t));
            usBuffIndex += sizeof(uint16_t);
            /*填充文件数据*/
            CommBitInvert((puint8_t)(&ucFileData[0]), &ucACKData[usBuffIndex], usReadDataLen);

            usBuffIndex += usReadDataLen;
            /*填写应答数据的长度，出去头部*/
            pstACKHead->ucDataFlg = 1U;

            /**/
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);
    }

    return usACKDataLen;
 }

/*
********************************函数描述*******************************************************************************
* 函数名称: CommBitInvert
*
* 功能描述: 将数组内元素按位取反
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
static void CommBitInvert(uint8_t ucSrcData[], uint8_t ucDestData[], uint16_t usDataLen)
{
    uint16_t usBuffIndex = 0U;
    
    for(usBuffIndex = 0U; usBuffIndex < usDataLen; usBuffIndex++)
    {
        ucDestData[usBuffIndex] = ~ucSrcData[usBuffIndex];
    }
    
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommOSCGetVar
*
* 功能描述: 读示波器变量数据
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
static uint16_t CommOSCGetVar(uint8_t ucCS1131Data[], uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    OSCPCtrlInfo_t* pstOSCPCtrl = NULL;                                     /*文件操作相关信息记录结构*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucTaskID = MAX_TASK_NUM;
    uint32_t uiStartPos = 0U;
    uint32_t uiDataLen = 0U;
    uint32_t uiReadLen = 0U;
    puint8_t pucOSCPVarAddr = NULL;

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
        usBuffIndex += sizeof(MSGHead_t);
        /*获取要读取的位置信息*/
        memcpy(&ucTaskID, &ucCS1131Data[usBuffIndex], sizeof(uint8_t));
        usBuffIndex += sizeof(uint8_t);
        memcpy(&uiStartPos, &ucCS1131Data[usBuffIndex], sizeof(uint32_t));
        usBuffIndex += sizeof(uint32_t);
        memcpy(&uiDataLen, &ucCS1131Data[usBuffIndex], sizeof(uint32_t));
        usBuffIndex += sizeof(uint32_t);
        
        
        /*根据文件名获取文件控制结构*/
        pstOSCPCtrl = (OSCPCtrlInfo_t*)SharedGetOSCPCtrl(ucTaskID);
        if(NULL == pstOSCPCtrl)
        {
            printf("OSCPCtrlAddr %d %d\n",pstOSCPCtrl,ucTaskID);
            ucSubSeason = ERR_OSCP_GET_VAR;
        }
        else
        {
            if((uiStartPos <= pstOSCPCtrl->uiWRIndex) && (uiStartPos < OSCP_BUFF_LEN))
            {
                pucOSCPVarAddr = SharedGetOSCPBuff(ucTaskID, uiStartPos);
                uiReadLen = MIN((pstOSCPCtrl->uiWRIndex - uiStartPos), uiDataLen);
                if(NULL == pucOSCPVarAddr)
                {
                    printf("OSCPVarAddr %d\n",pucOSCPVarAddr);
                    ucSubSeason = ERR_OSCP_GET_VAR;
                }
            }
            else
            {
                printf("StartPos err %d %d\n",uiStartPos,pstOSCPCtrl->uiWRIndex);
                ucSubSeason = ERR_OSCP_GET_VAR;
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
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&ucTaskID,sizeof(uint8_t));
            usBuffIndex += sizeof(uint8_t);
            /*填充文件数据长度*/
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&uiReadLen,sizeof(uint32_t));
            usBuffIndex += sizeof(uint32_t);
            /*填充数据*/

            memcpy(&ucACKData[usBuffIndex],pucOSCPVarAddr,uiReadLen);

            usBuffIndex += uiReadLen;
            /*填写应答数据的长度，出去头部*/
            pstACKHead->ucDataFlg = 1U;
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);
    }

    return usACKDataLen;
 }

/*
********************************函数描述*******************************************************************************
* 函数名称: CommSlaveConfirm
*
* 功能描述:
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
static uint16_t CommSlaveConfirm(uint8_t ucCS1131Data[], uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    PMController_t emLocalPMId = SysGetLocalPMID();                         /* 本地PM ID号           */
    uint16_t usSlaveID = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

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
        if(pstAppend != NULL)
        {
            pstMSGHead = (MSGHead_t*)ucCS1131Data;
            if((pstMSGHead->ucPMNum - 1) != emLocalPMId)
            {
                ucSubSeason = ERR_SLAVE_CONFIRM;
                printf("PMId Err %d\n",pstMSGHead->ucPMNum);
            }
            else
            {
                usBuffIndex += sizeof(MSGHead_t);
                /*获取要确认的从站号*/
                memcpy(&usSlaveID, &ucCS1131Data[usBuffIndex], sizeof(uint16_t));
                usBuffIndex += sizeof(uint16_t);
                if(usSlaveID < MAX_SLAVE_NUM)
                {
                    pstAppend->stSlaveState[usSlaveID].stSlaveDiag.ucOnlineDiag = 
                            pstAppend->stSlaveState[usSlaveID].stSlaveDiag.ucOnlineDiag | 0x02;
                    printf("OnlineDiag %x %x",usSlaveID,pstAppend->stSlaveState[usSlaveID].stSlaveDiag.ucOnlineDiag);
                }
                else
                {
                    ucSubSeason = ERR_SLAVE_CONFIRM;
                    printf("SlaveID Err %d\n",usSlaveID);
                }
            }
        }
        else
        {
            ucSubSeason = ERR_APPEND_NULL;
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
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex, (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);
    }

    return usACKDataLen;
 }

/*
********************************函数描述*******************************************************************************
* 函数名称: CommOPCInit
*
* 功能描述: OPC初始化函数
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
static void CommOPCInit(void)
{
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommOPCCycle
*
* 功能描述: OPC数据处理函数
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
    SyncTime64_t stSyncCurTime;                         /* 时间结构体               */
    bool_t bRet = false;
    static uint8_t s_ucTimeout[NUM_OF_CM] = {0,0,0,0};
    ACKHead_t* pstACKHead = NULL;
    uint32_t uiCalcCRC = 0U;
    static uint8_t ucSendACKFlg[NUM_OF_CM] = {0,0,0,0};
    
    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间             */

    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,OPC_SRV);

        if(pstSysSrvInfo == NULL)
        {
            
        }
        else if(SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag)
        {/*当服务处理标志为busy时，等待来自core0的应答数据*/

            if(NEED_ACK == pstSysSrvInfo->stSysSrvInfoHead.emAckFlag)
            {
                /*从共享内存上获取应答数据*/
                memset(&ucACKData[0], 0x00,1024);
                SysGetSMMsg(SM_OPC_RESP,emCMIndex,&ucACKData[0],&usACKDataLen);

                if(usACKDataLen > 0)
                {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                    ucOPCACKTimeoutCount[emCMIndex] = 0;
                    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                }
                else
                {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
                    ucOPCACKTimeoutCount[emCMIndex]++;
                    if(OPC_ACK_TIMEOUT == ucOPCACKTimeoutCount[emCMIndex])
                    {
                        ucOPCACKTimeoutCount[emCMIndex] = 0;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        
                        printf("OPC:GetSMMsg TIMEOUT\n ");
                    }
                }
            }
            else
            {
                if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                        (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                {
                    s_ucTimeout[emCMIndex]++;
                    bRet = SysSetSMMsg(SM_OPC_REQ, emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                            pstSysSrvInfo->stSysSrvData.uiSrvLen);
                    if(true == bRet)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;
                        s_ucTimeout[emCMIndex] = 0;
                    }
                    if(s_ucTimeout[emCMIndex] == 10)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        
                        printf("OPC SetSMMsg TIMEOUT\n ");
                    }
                }
            }
        }
        else
        {/*ProcessFlag is SRV_NOT_NEED_PROCESS*/
            /*从系统资源获取来自CM的服务数据*/
            memset((puint8_t)pstSysSrvInfo, 0x00, 100);
            SysGetMsg(emCMIndex,OPC_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usOPCDataLen);
            if(usOPCDataLen > 0)
             {/*数据长度大于0，表示收到数据*/
                 /*获取消息头部*/
                 memset(&stMSGHead, 0x00, sizeof(MSGHead_t));
                 memcpy(&stMSGHead, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], sizeof(MSGHead_t));
                 /*记录服务数据相关信息*/
                 pstSysSrvInfo->stSysSrvData.uiSrvLen = usOPCDataLen;
                 pstSysSrvInfo->stSysSrvData.usSrvNo = stMSGHead.usActualType;
                 /*计算数据CRC*/
                 uiCalCrc = SysCrc32Cal(0x00, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                         stMSGHead.usLength);
                 usBuffIndex = stMSGHead.usLength;
                 /*读取下发的CRC*/
                 memcpy((puint8_t)&uiReadCrc,(puint8_t)&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex], \
                         CHKSUM_CRC_SIZE);

                 if(uiCalCrc != uiReadCrc)
                 {/*CRC 不一致*/
                     /*log*/
                 }
                 else if(usOPCDataLen != stMSGHead.usLength + CHKSUM_CRC_SIZE)
                 {/*数据长度不一致*/
                     /*log*/
                 }
                 else if(HAS_BE_CONFIGURED != SysGetIsConfigured())
                 {
                     /*produce ACK data*/
                     pstACKHead = (ACKHead_t*)&ucACKData[0];
                     pstACKHead->ucType = stMSGHead.ucProtect;
                     pstACKHead->ucProtect = stMSGHead.ucType;
                     pstACKHead->usActualType = stMSGHead.usActualType;
                     pstACKHead->ucNodeNum = stMSGHead.ucNodeNum;
                     pstACKHead->usSequenceNum = stMSGHead.usSequenceNum;
                     pstACKHead->ucDataFlg = 0U;
                     pstACKHead->ucFlag = ACK_FRAME_SINGLE;

                     pstACKHead->ucResCode = RESCODE_FAILED;
                     
                     pstACKHead->usLength = (uint16_t)(sizeof(ACKHead_t));
                     usACKDataLen = pstACKHead->usLength;
                     pstACKHead->ucSubReason = ERR_NOT_PRJ;
                     /*计算并填充数据应答的CRC*/
                     uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                     memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                     usACKDataLen += sizeof(uiCalcCRC);
                 }
                 else if(KS_PROGRAM != SharedGetSwitchPos())
                 {
                     /*produce ACK data*/
                     pstACKHead = (ACKHead_t*)&ucACKData[0];
                     pstACKHead->ucType = stMSGHead.ucProtect;
                     pstACKHead->ucProtect = stMSGHead.ucType;
                     pstACKHead->usActualType = stMSGHead.usActualType;
                     pstACKHead->ucNodeNum = stMSGHead.ucNodeNum;
                     pstACKHead->usSequenceNum = stMSGHead.usSequenceNum;
                     pstACKHead->ucDataFlg = 0U;
                     pstACKHead->ucFlag = ACK_FRAME_SINGLE;

                     pstACKHead->ucResCode = RESCODE_FAILED;
                     
                     pstACKHead->usLength = (uint16_t)(sizeof(ACKHead_t));
                     usACKDataLen = pstACKHead->usLength;
                     pstACKHead->ucSubReason = ERR_NOT_PROGRAM;
                     /*计算并填充数据应答的CRC*/
                     uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                     memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                     usACKDataLen += sizeof(uiCalcCRC);
                 }
                 else
                 {
                     ucSendACKFlg[emCMIndex] = NEED_SEND_ACK;
                     switch(stMSGHead.usActualType)
                     {/*根据服务号，处理1131服务*/
                         /*these service need to synchronize*/
                         case OPC_WRITE:
                         {/*将服务进行同步*/
                             
                             SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                               &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                             SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                               &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                             
                             stSyncTask.emTaskId = SYS_BROADCAST_OPCSRV;
                             stSyncTask.emPMId = INVALID_PM_ID;
                             stSyncTask.emCMId = emCMIndex;
                             stSyncTask.emTaskStatus = TASK_SEND;
                             stSyncTask.emTaskWait = TASK_NOWAIT;
                             pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                             /*暂定为：非稳态时也可以处理服务。*/
                             SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                             
                             pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                         }

                         break;

                         default:
                         {
                             ucSendACKFlg[emCMIndex] = NOT_NEED_SEND_ACK;
                         }
                         break;
                     }
                 }
             }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            DEBUG_COMM("OPC HAS ACK\n");
            if(NEED_SEND_ACK == ucSendACKFlg[emCMIndex])
            {
                SysSetMsg(emCMIndex,OPC_RESP,ucACKData,usACKDataLen);
            }
            else
            {
                DEBUG_COMM("SEND_ACK_FLG_ERR %d %d\n",ucSendACKFlg[emCMIndex],emCMIndex);
            }
            
            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            
            usACKDataLen = 0U;
            
            ucSendACKFlg[emCMIndex] = NOT_NEED_SEND_ACK;
        }
    }
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommModbusInit
*
* 功能描述: Modbus初始化函数
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
static void CommModbusInit(void)
{
    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommModbusCycle
*
* 功能描述: Modbus输入数据处理函数
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
static void CommModbusCycle(void)
{
    CMController_t emCMIndex = INVALID_CM_ID;           /* CM索引 */
    CommModbusCommonHead_t stModbusHead;                /* Modbus消息头部 */
    uint16_t usModbusMsgNum = 0;
    uint16_t usModbusMsgLen = 0;
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体 */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上Modbus服务信息 */
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列 */
    SyncTime64_t stSyncCurTime;                         /* 时间结构体 */
    bool_t bRet = false;
    static uint8_t s_ucTimeout[NUM_OF_CM] = {0,0,0,0};

    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间 */

    for(emCMIndex = CM1; emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        /* 获取服务数据在系统资源的存储结构指针 */
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex, MODBUS_SRV);
        if(pstSysSrvInfo != NULL)
        {
            if(SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag)
            {/* 服务处理标志为busy，即有服务正在处理 */
//                printf("4:%d\n",stSyncCurTime.stTimeStruct.uiLow32b);
//                printf("4:%d\n",pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.stTimeStruct.uiLow32b);
//                printf("4:%d\n",pstSysSrvInfo->stSysSrvInfoHead.emSyncRet);
                if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                        (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
//                if(stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime)
                {/* 等待同步完成 */
                    bRet = SysSetSMMsg(SM_MODBUS_REQ, emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                            pstSysSrvInfo->stSysSrvData.uiSrvLen);
                    if(true == bRet)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        s_ucTimeout[emCMIndex] = 0;
                    }
                    else
                    {
                        s_ucTimeout[emCMIndex]++;
                        if(s_ucTimeout[emCMIndex] == 10)
                        {
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                            pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        }
                    }
                }
            }
            else
            {/* 服务处理标志为Free */
//                printf("2\n");
                /* 从系统资源获取来自Modbus消息个数 */
                bRet = SysGetModbusMsgNum (emCMIndex, &usModbusMsgNum);
                

                if((bRet == true) && (usModbusMsgNum > 0))
                {/* 存在Modbus消息 */
                    if(HAS_BE_CONFIGURED == SysGetIsConfigured())
                    {
                        /* 读取一条Modbus消息 */
                        memset((puint8_t)pstSysSrvInfo, 0x00, 100);
                        memset(&stModbusHead, 0x00, sizeof(CommModbusCommonHead_t));
                        bRet = SysGetModbusMsgQueue (emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usModbusMsgLen);
                        /*记录服务数据相关信息*/
                        pstSysSrvInfo->stSysSrvData.uiSrvLen = usModbusMsgLen;
    //                    pstSysSrvInfo->stSysSrvData.usSrvNo = stModbusHead.usActualType;
    
                        /* 填同步队列 */
                        pstSysSyncSrvQueue = SysGetSyncSrvQueue();
                        if(pstSysSyncSrvQueue != NULL)
                        {
    //                        printf("3:%d\n",pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.stTimeStruct.uiLow32b);
    
                            SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unActiveTime,
                                              &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                            SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                              &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                            stSyncTask.emTaskId = SYS_BROADCAST_MODBUSSRV;
                            stSyncTask.emPMId = INVALID_PM_ID;
                            stSyncTask.emCMId = emCMIndex;
                            stSyncTask.emTaskStatus = TASK_SEND;
                            stSyncTask.emTaskWait = TASK_NOWAIT;
                            pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;
                            SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                        }
                    }
                    else
                    {
                        SysModbusClear();
                    }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PInit
*
* 功能描述: P2P初始化函数
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

static void CommP2PInit( void )
{
    /* Initialize Local Info. */
    memset( &s_stP2PSendCtrl[0], 0, sizeof(P2PSendCtrl_t)*(uint32_t)(MAX_P2P_BLOCK_NUM + 1 ));
    memset( &ucP2PACKTimeoutCount[0], 0, sizeof(uint8_t)*(uint32_t)(NUM_OF_CM));

    /* Initialize P2PSafety Module */
    P2PCommInitSend();
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PCycle
*
* 功能描述: P2P周期运行函数
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

void CommP2PCycle( void )
{
    static bool_t s_bClrDone = false;  /* Clear done flag */
    bool_t bHasCfg = false;            /* PM has config flag */

    if( HAS_BE_CONFIGURED == SysGetIsConfigured())
    {
        s_bClrDone = false;
        bHasCfg = true;

        CommP2PSend();  /* Send PM P2P Req */
    }
    else
    {
        if( !s_bClrDone )
        {
            s_bClrDone = true;

            /* Clear local control info. */
            /* Clear safety module info. */
            CommP2PInit();

            /* Clear system resource of P2P */
            SharedP2PSafetyCommInit();
        }
    }

    /* Recv Ext Req, send PM Resp; */
    /* Recv Ext Resp, send none */
    CommP2PRecv( bHasCfg );
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PRecv
*
* 功能描述: P2P接收函数
*
* 输入参数: bHasCfg: PM是否有配置
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CommP2PRecv( bool_t bHasCfg )
{
#if 0
    CMController_t emCMIndex = CM1;  /* CM 索引 */
    srv_srvdata_type_t emSyncType = P2P_REQ_SRV;

    for( emCMIndex = CM1; emCMIndex < NUM_OF_CM; emCMIndex++ )
    {
        for( emSyncType = P2P_REQ_SRV; emSyncType <= P2P_RSP_SRV; emSyncType++ )
        {
            /* P2P Message Type */
            if( P2P_REQ_SRV == emSyncType )
            {
                /* Recv Ext Req, send PM Resp */
                CommP2PRecvReq( emCMIndex, bHasCfg );
            }
            else
            {
                /* Recv Ext Resp, send none */
                CommP2PRecvResp( emCMIndex, bHasCfg );
            }
        }
    }
#endif

    static CMController_t s_emCMIndex = CM1;  /* CM 索引 */
    srv_srvdata_type_t emSyncType = P2P_REQ_SRV;

    for( emSyncType = P2P_REQ_SRV; emSyncType <= P2P_RSP_SRV; emSyncType++ )
    {
        /* P2P Message Type */
        if( P2P_REQ_SRV == emSyncType )
        {
            /* Recv Ext Req, send PM Resp */
            CommP2PRecvReq( s_emCMIndex, bHasCfg );
        }
        else
        {
            /* Recv Ext Resp, send none */
            CommP2PRecvResp( s_emCMIndex, bHasCfg );
        }
    }

    /* Update CM */
    s_emCMIndex++;
    if( s_emCMIndex > CM4 )
    {
        s_emCMIndex = CM1;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PRecvReq
*
* 功能描述: P2P接收请求函数：接收外部请求，并应答
*
* 输入参数: emCMIndex：CM逻辑号；
*          bHasCfg: PM是否有配置。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CommP2PRecvReq( CMController_t emCMIndex, bool_t bHasCfg )
{
    static uint8_t s_ucTimeout[NUM_OF_CM] = { 0 };
    bool_t bRet = false;
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint32_t uiReadCrc = 0U;                            /* P2P数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* P2P数据计算的CRC值  */
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    uint16_t usP2PDataLen = 0;
    uint8_t ucACKData[1024];
    uint16_t usACKDataLen = 0;
    SyncTime64_t stSyncCurTime;                         /* 时间结构体 */
    P2PRecvReqHead_t* pstReqHead = NULL;
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列 */
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上P2P服务信息  */

    SysGetSyncTime( &stSyncCurTime );                   /* 获取当前时间 */
    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列 */

    if((emCMIndex >= CM1) && (emCMIndex < NUM_OF_CM))
    {
        /* 获取服务数据在系统资源的存储结构指针 */
        pstSysSrvInfo = SysGetSrvInfo( emCMIndex, P2P_REQ_SRV );

        if( SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag )
        {/* 当服务处理标志为busy时，等待来自core0的应答数据 */
            if( NEED_ACK == pstSysSrvInfo->stSysSrvInfoHead.emAckFlag )
            {
                if( P2P_REQ_FRAME == pstSysSrvInfo->stSysSrvData.usSrvNo )
                {
                    /* 从共享内存上获取应答数据 */
                    usACKDataLen = 0U;
                    SysGetSMMsg( SM_P2P_RESP, emCMIndex, &ucACKData[0], &usACKDataLen );

                    if( usACKDataLen > 0U )
                    {/* 获取到应答数据，将服务处理标志清除，超时计数清零 */
                        if( !bHasCfg )
                        {
                            /* Just discard when PM has no cfg */
                            usACKDataLen = 0U;
                            s_ucTimeout[emCMIndex] = 0U;
                        }

                        ucP2PACKTimeoutCount[emCMIndex] = 0U;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                    }
                    else
                    {/* 当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答 */
                        ucP2PACKTimeoutCount[emCMIndex]++;
                        if( P2P_ACK_TIMEOUT == ucP2PACKTimeoutCount[emCMIndex])
                        {
                            //printf( "PC0TO\n" );
                            ucP2PACKTimeoutCount[emCMIndex] = 0;
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                            pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        }
                    }
                }
            }
            else
            {
                if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                   (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet) && \
                   (P2P_REQ_FRAME == pstSysSrvInfo->stSysSrvData.usSrvNo))
                {
                    if( !bHasCfg )
                    {
                        /* Just discard when PM has no cfg */
                        s_ucTimeout[emCMIndex] = 0U;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                    }
                    else
                    {
                        //printf( "PEReq\n" );
#if 0
                        /* print message */
                        printf( "P2PEReq L%d\n", pstSysSrvInfo->stSysSrvData.uiSrvLen );
                        printf( "Data(0x): " );
                        for( i = 0U; i < pstSysSrvInfo->stSysSrvData.uiSrvLen; i++ )
                        {
                            printf( "%02X ",pstSysSrvInfo->stSysSrvData.ucSrvData[i]);
                        }
                        printf("\n");
#endif

                        bRet = SysSetSMMsg( SM_P2P_REQ, emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                            (uint16_t)pstSysSrvInfo->stSysSrvData.uiSrvLen );
                        if( bRet )
                        {
                            /*printf( "P2PEReq SysSet OK\n" );*/
                            s_ucTimeout[emCMIndex] = 0U;
                            pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;

                            /* Record Source Station ID */
                            pstReqHead = (P2PRecvReqHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
                        }
                        else
                        {
                            /*printf( "P2PEReq: ESysSet, CM%d, Rty%d.\n", emCMIndex, s_ucTimeout[emCMIndex]);*/
                            s_ucTimeout[emCMIndex]++;

                            if( 10U == s_ucTimeout[emCMIndex])
                            {
                                s_ucTimeout[emCMIndex] = 0U;
                                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                                pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                                //printf( "PSSF\n" );
                            }
                        }
                    }
                }
            }
        }
        else
        {/*ProcessFlag is SRV_NOT_NEED_PROCESS*/
            //memset( pstSysSrvInfo, 0, sizeof(SysSrvInfo_t));
            memset( pstSysSrvInfo, 0, 100);
            SysGetP2PMsg( emCMIndex, P2P_EXT_REQ, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usP2PDataLen );
            if( usP2PDataLen > 0U )
            {/*数据长度大于0，表示收到数据*/
                if( !bHasCfg )
                {
                    /* Just discard when PM has no cfg */
                    s_ucTimeout[emCMIndex] = 0U;
                }
                else
                {
                    /* 获取消息头部 */
                    pstReqHead = (P2PRecvReqHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];

                    /* 记录服务数据相关信息 */
                    pstSysSrvInfo->stSysSrvData.uiSrvLen = (uint32_t)usP2PDataLen;
                    pstSysSrvInfo->stSysSrvData.usSrvNo = (uint16_t)pstReqHead->ucFrameType;

                    /* 计算数据CRC */
                    usBuffIndex = pstReqHead->usTotalLen - (uint16_t)(CHKSUM_CRC_SIZE);
                    uiCalCrc = SysCrc32Cal( 0, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], (uint32_t)usBuffIndex );

                    /* 读取下发的CRC */
                    memcpy( &uiReadCrc, &pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex], (uint32_t)(CHKSUM_CRC_SIZE));

                    if( uiCalCrc != uiReadCrc )
                    {/*CRC 不一致*/
                        //printf( "PEReqCRC\n" );
                    }
                    else if( usP2PDataLen != pstReqHead->usTotalLen )
                    {/*数据长度不一致*/
                        //printf( "PEReqLen\n" );
                    }
                    else
                    {
                        SysCalRelativeTime( &pstSysSrvInfo->stSysSrvInfoHead.unActiveTime,
                                            &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME );
                        SysCalRelativeTime( &pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                            &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME );

                        stSyncTask.emTaskId = SYS_BROADCAST_P2PREQSRV;
                        stSyncTask.emPMId = INVALID_PM_ID;
                        stSyncTask.emCMId = emCMIndex;
                        stSyncTask.emTaskStatus = TASK_SEND;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                        //if(SYS_STABLE_STATE == SysGetState())
                        //{/* 非稳态时按错误处理，不添加同步任务 */
                        //    SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                        //}

                        /*暂定为：非稳态时也可以处理服务。*/
                        SysEnSyncSrvQueue( pstSysSyncSrvQueue, &stSyncTask );

                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                    }
                }
            }
        }/* end of else - SRV_NOT_NEED_PROCESS */

        if( usACKDataLen > 0U )
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
#if 0
            printf( "C0 P2PResp: L-%d\n", usACKDataLen );
            printf( "Data(0x): " );
            for( i = 0U; i < usACKDataLen; i++ )
            {
                printf( "%02X ", ucACKData[i]);
            }
            printf("\n");
#endif

            /* Send PMP2PResp Msg */
            if( !SysSetP2PMsg( emCMIndex, P2P_PM_RESP, ucACKData, usACKDataLen ))
            {
                //printf( "ESSPRsp\n" );
            }
            else
            {
                //printf( "C0PRsp\n" );
            }

            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            usACKDataLen = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PRecvResp
*
* 功能描述: P2P接收应答函数
*
* 输入参数: emCMIndex：CM逻辑号
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void CommP2PRecvResp( CMController_t emCMIndex, bool_t bHasCfg )
{
    RespDecodeResultInfo_t stResDecodeResultInfo;
    bool_t bRedundant = false;                          /* 发送端网络是否为冗余网络的标志 */
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引 */
    uint32_t uiReadCrc = 0U;                            /* P2P数据中的CRC值 */
    uint32_t uiCalCrc = 0U;                             /* P2P数据计算的CRC值 */
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体 */
    uint16_t usP2PDataLen = 0;
    uint8_t ucBlkIndex = 0U;
    uint8_t ucBlkID = 0;
    uint32_t uiP2PSendBlkNum = 0U;
    bool_t bDiscard = false;                            /* 丢弃应答帧标志：防止 老应答包等无效数据帧 造成应答错位 */
    SyncTime64_t stSyncCurTime;                         /* 时间结构体 */
    P2PRecvRespHead_t *pstRespHead = NULL;
    SysSyncSrvQueue_t *pstSysSyncSrvQueue = NULL;       /* 服务同步步列 */
    SysSrvInfo_t *pstSysSrvInfo = NULL;                 /* 系统资源上P2P服务信息   */
    P2PBlock_t *pstP2PBlock = NULL;
    uint16_t uiRecvTOCycleCnt = 0U;

    SysGetSyncTime( &stSyncCurTime );                   /* 获取当前时间 */
    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列 */

    if((emCMIndex >= CM1) && (emCMIndex < NUM_OF_CM))
    {
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo( emCMIndex, P2P_RSP_SRV );

        if( SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag )
        {/* 由Core1来处理 */
            if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
               (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet) && \
               (P2P_RES_FRAME == pstSysSrvInfo->stSysSrvData.usSrvNo))
            {
                //printf( "PERsp\n" );
#if 0
                /* print message */
                printf( "P2PExtResp SRV DATA: Len-%d\n", pstSysSrvInfo->stSysSrvData.uiSrvLen );
                printf( "Data(0x): " );
                for( i = 0U; i < pstSysSrvInfo->stSysSrvData.uiSrvLen; i++ )
                {
                    printf( "%02X ",pstSysSrvInfo->stSysSrvData.ucSrvData[i]);
                }
                printf( "\n" );
#endif

                if( !bHasCfg )
                {
                    /* Just discard when PM has no cfg */
                    pstP2PBlock = NULL;
                }
                else
                {
                    pstRespHead = (P2PRecvRespHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
                    uiP2PSendBlkNum = SharedGetP2PSendBlockNum();

                    /* Find P2P Send Block: Loop */
                    for( ucBlkIndex = 0U; ucBlkIndex < (uint8_t)uiP2PSendBlkNum; ucBlkIndex++ )
                    {
                        ucBlkID = SharedGetP2PBlockIDSend(ucBlkIndex);
                        pstP2PBlock = SharedGetP2PSendBlockInfo( ucBlkID );
                        if( pstP2PBlock != NULL )
                        {
                            if( pstRespHead->ucSrcStaID == (uint8_t)pstP2PBlock->stP2PBlockHead.uiTargetStationID )
                            {
                                /* Finded */
                                break;
                            }
                        }
                    }

                    if((uint8_t)uiP2PSendBlkNum == ucBlkIndex )
                    {
                        pstP2PBlock = NULL;
                        //printf( "PNFB\n" );
                    }
                }

                if( pstP2PBlock != NULL )
                {
                    /* Discard if resp was invalid */
                    if( NOT_WAIT_ACK == s_stP2PSendCtrl[ucBlkID].emWaitACKFlag )
                    {
                        bDiscard = true;
                    }

                    if( !bDiscard )
                    {
                        /* Check if redundant communication */
                        if((pstP2PBlock->stP2PBlockHead.uiTargetStationIP1 != 0U) && \
                           (pstP2PBlock->stP2PBlockHead.uiTargetStationIP2 != 0U))
                        {
                            bRedundant = true;
                        }

                        /* 解析应答帧 */
                        if( P2PDecodeResp( pstRespHead->ucSrcStaID, bRedundant, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                           pstRespHead->usTotalLen, &stResDecodeResultInfo))
                        {
                            if( stResDecodeResultInfo.bRepeat )
                            {
                                /* Repeat: Discard, do nothing */
                                //printf( "PERspRpt\n" );
                            }
                            else
                            {
                                if( pstP2PBlock->stP2PBlockHead.uiPeriod < (uint32_t)(P2P_TIMEOUT_TIME))
                                {
                                    uiRecvTOCycleCnt = (10*(uint32_t)(P2P_TIMEOUT_TIME))/(uint32_t)(TASK_PERIOD);
                                }
                                else
                                {
                                    uiRecvTOCycleCnt = 10*pstP2PBlock->stP2PBlockHead.uiPeriod/(uint32_t)(TASK_PERIOD);
                                }

                                if( s_stP2PSendCtrl[ucBlkID].usWaitACKCount > (uint16_t)uiRecvTOCycleCnt )
                                {/* 等待ACK超时 */
                                    //printf( "PERspTO\n" );

                                    /* 调用安全层接口，错误处理 */
                                    P2PHandleRespTO( s_stP2PSendCtrl[ucBlkID].ucDestStaID );

                                    /*清空相应标志，计数*/
                                    s_stP2PSendCtrl[ucBlkID].emACKTimeoutFlag = ACK_TIMEOUT;
                                }
                                else
                                {
                                    s_stP2PSendCtrl[ucBlkID].emACKTimeoutFlag = ACK_NOT_TIMEOUT;
                                }

                                s_stP2PSendCtrl[ucBlkID].usWaitACKCount = 0U;
                                s_stP2PSendCtrl[ucBlkID].emWaitACKFlag = NOT_WAIT_ACK;
                            }
                        }
                        else
                        {
                            /* Print */
                            //printf( "PDERsp\n" );
                        }
                    }
                }

                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
            }
        }
        else
        {/* ProcessFlag is SRV_NOT_NEED_PROCESS */
            //memset( pstSysSrvInfo, 0, sizeof(SysSrvInfo_t));
            memset( pstSysSrvInfo, 0, 100);
            SysGetP2PMsg( emCMIndex, P2P_EXT_RESP, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usP2PDataLen );
            if( usP2PDataLen > 0U )
            {/* 数据长度大于0，表示收到数据 */
                /* Just discard when PM has no cfg */
                if( bHasCfg )
                {
                    /* 获取消息头部 */
                    pstRespHead = (P2PRecvRespHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];

                    /* 记录服务数据相关信息 */
                    pstSysSrvInfo->stSysSrvData.uiSrvLen = usP2PDataLen;
                    pstSysSrvInfo->stSysSrvData.usSrvNo = pstRespHead->ucFrameType;

                    /* 计算数据CRC */
                    usBuffIndex = pstRespHead->usTotalLen - (uint16_t)(CHKSUM_CRC_SIZE);
                    uiCalCrc = SysCrc32Cal( 0, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], (uint32_t)usBuffIndex );

                    /* 读取下发的CRC */
                    memcpy( &uiReadCrc, &pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex], (uint32_t)(CHKSUM_CRC_SIZE));

                    if( uiCalCrc != uiReadCrc )
                    {/* CRC 不一致 */
                        //printf( "PERspCRC\n" );
                    }
                    else if( usP2PDataLen != pstRespHead->usTotalLen )
                    {/* 数据长度不一致 */
                        //printf( "PERspL\n" );
                    }
                    else
                    {
                        SysCalRelativeTime( &pstSysSrvInfo->stSysSrvInfoHead.unActiveTime,
                                            &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME );
                        SysCalRelativeTime( &pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime,
                                            &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME );

                        stSyncTask.emTaskId = SYS_BROADCAST_P2PRSPSRV;
                        stSyncTask.emPMId = INVALID_PM_ID;
                        stSyncTask.emCMId = emCMIndex;
                        stSyncTask.emTaskStatus = TASK_SEND;
                        stSyncTask.emTaskWait = TASK_NOWAIT;
                        pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                        //if(SYS_STABLE_STATE == SysGetState())
                        //{/* 非稳态时按错误处理，不添加同步任务 */
                        //    SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                        //}

                        /* 暂定为：非稳态时也可以处理服务 */
                        SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);

                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                    }
                }
            }
        }/* end of else - SRV_NOT_NEED_PROCESS */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetP2PSendStartFlag
*
* 功能描述: 获取P2P发送功能块启停标志
*
* 输入参数: ucBlkIndex：发送功能块索引值， 0-63
*
* 输出参数: 无
*
* 返 回 值: 发送启停标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetP2PSendStartFlag( uint8_t ucBlkIndex )
{
    SharedAppend_t *pstAppend = NULL;
    uint8_t ucBit = 0U;
    uint8_t ucValue = 0U;
    bool_t bStart = false;

    if( ucBlkIndex < (uint8_t)(MAX_P2P_BLOCK_NUM)*2 )
    {
        /* Get Append Info. */
        pstAppend = (SharedAppend_t *)SysGetSharedMemAppendInfoStartAddr();
        if( pstAppend != NULL )
        {
            /* Calc Byte & Bit Index */
//            ucValue = pstAppend->ucP2PCtrl[ucBlkIndex / 8];
//            ucBit = ucBlkIndex % 8;
//            if( ucValue & (0x01U << ucBit))
            ucValue = pstAppend->ucP2PCtrl[ucBlkIndex];
            if( ucValue == 0x01 )
            {
                bStart = true;
            }
            else
            {
                bStart = false;
            }
        }
    }

    return bStart;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PSend
*
* 功能描述: P2P发送函数
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

static void CommP2PSend( void )
{
    uint8_t ucBlockIndex = 0U;        /* P2P发送块的块号索引 */
    uint32_t uiBlockSendCycle = 0U;   /* P2P发送块配置的发送周期 */
    uint8_t ucSendIndex = 0U;         /* 当前要发送块的索引 */
    uint8_t ucBlkID = 0U;         
    uint16_t usP2PSendDataLen = 0U;   /* 发送数据长度 */
    uint32_t uiP2PSendBlockNum = 0U;
    P2PBlock_t* pstP2PBlock = NULL;   /* P2P配置块结构指针 */
    uint32_t uiSyncTime = 0U;
    uint32_t uiRecvTOCycleCnt = 0U;
    P2PConfigInfo_t* pstP2PConfigInfo = NULL;
    
    pstP2PConfigInfo = SharedGetP2PConfigInfo();
    uiP2PSendBlockNum = SharedGetP2PSendBlockNum();
    uiSyncTime = SysGetSyncTimeLow32();
    
    if(uiP2PSendBlockNum > MAX_P2P_BLOCK_NUM)
    {
        uiP2PSendBlockNum = 0;
    }

    /* Send control process */
    for( ucBlockIndex = 0U; ucBlockIndex < (uint8_t)uiP2PSendBlockNum; ucBlockIndex++ )
    {
        ucBlkID = SharedGetP2PBlockIDSend(ucBlockIndex);
        if( ACTIVE == SharedGetP2PBlockSendActive( ucBlkID ))
        {
            pstP2PBlock = SharedGetP2PSendBlockInfo( ucBlkID );

            /* P2P Start */
            if( GetP2PSendStartFlag( (uint8_t)(pstP2PBlock->stP2PBlockHead.uiBlockID)))
            {
                /* 更新发送间隔周期计数 */
                if( s_stP2PSendCtrl[ucBlkID].bHasSend )
                {
                    s_stP2PSendCtrl[ucBlkID].uiIntrCycCnt++;
                }

                /* 轮询所有模块，发送周期计数 */
                uiBlockSendCycle = SharedGetP2PBlockSendCycle( ucBlkID );
                if( uiBlockSendCycle > 0U )
                {
                    if((uiSyncTime % (10*uiBlockSendCycle)) <= (uint32_t)(TASK_PERIOD))
                    {
                        s_stP2PSendCtrl[ucBlkID].emWaitSendFlag = WAIT_SEND;
                        s_stP2PSendCtrl[ucBlkID].ucDestStaID = pstP2PBlock->stP2PBlockHead.uiTargetStationID;
                    }
                }

                if( WAIT_ACK == s_stP2PSendCtrl[ucBlkID].emWaitACKFlag )
                {/* 在等待来自对端控制站的ACK */
                    s_stP2PSendCtrl[ucBlkID].usWaitACKCount++;
                    uiRecvTOCycleCnt = (10 * pstP2PBlock->stP2PBlockHead.uiPeriod)/((uint32_t)(TASK_PERIOD)*4);

                    /*if( pstP2PBlock->stP2PBlockHead.uiPeriod < (uint32_t)(P2P_TIMEOUT_TIME))
                    {
                        uiRecvTOCycleCnt = (10*(uint32_t)(P2P_TIMEOUT_TIME))/((uint32_t)(TASK_PERIOD)*4);
                    }
                    else
                    {
                        uiRecvTOCycleCnt = (10*pstP2PBlock->stP2PBlockHead.uiPeriod)/((uint32_t)(TASK_PERIOD)*4);
                    }*/

                    if( s_stP2PSendCtrl[ucBlkID].usWaitACKCount >= (uint16_t)uiRecvTOCycleCnt )
                    {/* 等待ACK超时 */
                        s_stP2PSendCtrl[ucBlkID].usWaitACKCount = 0U;

                        /* 调用安全层接口，错误处理 */
                        P2PHandleRespTO( s_stP2PSendCtrl[ucBlkID].ucDestStaID );

                        /* 清空相应标志，计数 */
                        s_stP2PSendCtrl[ucBlkID].emWaitACKFlag = NOT_WAIT_ACK;
                        s_stP2PSendCtrl[ucBlkID].emACKTimeoutFlag = ACK_TIMEOUT;
                    }
                }
            }
            else
            {
                /* Clear Info. */
                s_stP2PSendCtrl[ucBlkID].bHasSend = false;
                s_stP2PSendCtrl[ucBlkID].uiIntrCycCnt = 0U;
                s_stP2PSendCtrl[ucBlkID].emWaitSendFlag = NOT_WAIT_SEND;
            }
        }
    }

    /* P2P block send process, each cycle process one block */
    if( uiP2PSendBlockNum > 0U )
    {
        ucSendIndex = (uiSyncTime / ((uint32_t)(TASK_PERIOD)*4)) % uiP2PSendBlockNum;
        ucBlkID = SharedGetP2PBlockIDSend(ucSendIndex);

        //if( WAIT_SEND == s_stP2PSendCtrl[ucBlkID].emWaitSendFlag )
        //{
            //printe( "T%d, A%d\n", uiSyncTime, s_stP2PSendCtrl[ucBlkID].emWaitACKFlag );
        //}

        if((WAIT_SEND == s_stP2PSendCtrl[ucBlkID].emWaitSendFlag) &&
           (NOT_WAIT_ACK == s_stP2PSendCtrl[ucBlkID].emWaitACKFlag))  /* Timeout / Received ACK */
        {
            /* packing P2P data */
            usP2PSendDataLen = CommP2PPacking( ucBlkID, s_stP2PSendCtrl[ucBlkID].emACKTimeoutFlag );
            if( 0U == usP2PSendDataLen )
            {
                //printe( "P2P E\n" );
            }
            else
            {
                //printe( "P2P S\n" );
                
                s_stP2PSendCtrl[ucBlkID].emWaitACKFlag = WAIT_ACK;
                s_stP2PSendCtrl[ucBlkID].usWaitACKCount = 0U;

                /* Handle SendP2PReq Timeout */
                if( s_stP2PSendCtrl[ucBlkID].bHasSend )
                {
                    /* Check if send timeout */
                    uiBlockSendCycle = SharedGetP2PBlockSendCycle( ucBlkID );
                    if( s_stP2PSendCtrl[ucBlkID].uiIntrCycCnt > ((10*uiBlockSendCycle)/((uint32_t)(TASK_PERIOD)*4)))
                    {
                        /* Send Timeout */
                        P2PHandleSendReqTO( s_stP2PSendCtrl[ucBlkID].ucDestStaID, true );
                    }
                    else
                    {
                        P2PHandleSendReqTO( s_stP2PSendCtrl[ucBlkID].ucDestStaID, false );
                    }
                }
                else
                {
                    s_stP2PSendCtrl[ucBlkID].bHasSend = true;
                }

                s_stP2PSendCtrl[ucBlkID].uiIntrCycCnt = 0U;
            }

            s_stP2PSendCtrl[ucBlkID].emACKTimeoutFlag = ACK_NOT_TIMEOUT;
            s_stP2PSendCtrl[ucBlkID].emWaitSendFlag = NOT_WAIT_SEND;
        }
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PPacking
*
* 功能描述: P2P打包请求帧并发送
*
* 输入参数: ucP2PSendIndex：P2P发送功能块索引值；
*          emACKTimeoutFlag：应答超时标志位。
*
* 输出参数: 无
*
* 返 回 值: 发送的字节数
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t CommP2PPacking(uint8_t ucP2PSendIndex, ACKTimeoutFlag_t emACKTimeoutFlag)
{   
    P2PBlock_t* pstP2PBlock = NULL;  /* P2P配置块结构指针 */
    P2PSendReqHead_t* pstP2PSendHead = NULL;
    uint32_t uiBuffIndex = 0U;
    uint32_t uiP2PVarCount = 0U;
    puint8_t pucDataAreaAddr = NULL;
    uint8_t ucTmpBool = 0U;
    uint8_t ucTmpBuff[P2P_SEND_BUFF_SIZE];
    uint16_t usTmpBuffIndex = 0U;
    uint16_t usMakeReqDataLen = 0U;
    bool_t bMakeReqRet = false;
    uint32_t uiCalcCRC = 0U;
    uint16_t usRet = 0U;
    uint8_t ucP2PSendbuff[P2P_SEND_BUFF_SIZE];  /* 发送数据缓存 */
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;     /* CM槽位表可读片区的指针，供解析文件时进行比对 */
    CMController_t emCMIndex = INVALID_CM_ID;
    
    /* 入参有效性判断 */
    if((ACTIVE == SharedGetP2PBlockSendActive(ucP2PSendIndex)) && (ucP2PSendbuff != NULL))
    {
        pstP2PBlock = SharedGetP2PSendBlockInfo(ucP2PSendIndex);
        if(pstP2PBlock != NULL)
        {
            pstP2PSendHead = (P2PSendReqHead_t*)&ucP2PSendbuff[0];
            uiBuffIndex = sizeof(P2PSendReqHead_t);
            usTmpBuffIndex = 0U;
            
            for( uiP2PVarCount = 0U; uiP2PVarCount < pstP2PBlock->stP2PBlockHead.uiP2PVarNum; uiP2PVarCount++ )
            {
                pucDataAreaAddr = SysGetSharedMemRtDataStartAddr( pstP2PBlock->stP2PVar[uiP2PVarCount].ucRefID );
                if((pucDataAreaAddr != NULL) && (pstP2PBlock->stP2PVar[uiP2PVarCount].usVarSize <= 8))
                {
                    if(pstP2PBlock->stP2PVar[uiP2PVarCount].usVarSize != 0U)
                    {
                        memcpy(&ucTmpBuff[usTmpBuffIndex], pucDataAreaAddr + pstP2PBlock->stP2PVar[uiP2PVarCount].uiOffset, \
                                pstP2PBlock->stP2PVar[uiP2PVarCount].usVarSize);
                        usTmpBuffIndex += pstP2PBlock->stP2PVar[uiP2PVarCount].usVarSize;
                    }
                    else
                    {
                        memcpy(&ucTmpBool, pucDataAreaAddr + (pstP2PBlock->stP2PVar[uiP2PVarCount].uiOffset / 8), \
                                sizeof(uint8_t));
                        
                        if((ucTmpBool & (0x01U << (pstP2PBlock->stP2PVar[uiP2PVarCount].uiOffset % 8U))) != 0U )
                        {
                            ucTmpBool = 1U;
                        }
                        else
                        {
                            ucTmpBool = 0U;
                        }

                        memcpy(&ucTmpBuff[usTmpBuffIndex], &ucTmpBool, sizeof(uint8_t));
                        usTmpBuffIndex += sizeof(uint8_t);
                    }
                }
            }
            
            /* 安全层打包 */
            /* Calculate maximum buffer length for safety request frame */
            /* Check if make successfully and print error info. if make failed */
            usMakeReqDataLen = sizeof(ucP2PSendbuff) - sizeof(P2PSendReqHead_t) - sizeof(uint32_t);
            bMakeReqRet = P2PMakeReq(pstP2PBlock->stP2PBlockHead.uiTargetStationID, &ucTmpBuff[0], usTmpBuffIndex,  \
                                     &ucP2PSendbuff[uiBuffIndex], &usMakeReqDataLen);
            if( !bMakeReqRet )
            {
                usRet = 0U;
                //printf( "PMRF!\n" );
            }
            else
            {
                pstP2PSendHead->ucTimeoutFlag = (uint8_t)emACKTimeoutFlag;
                pstP2PSendHead->ucSrcStaID = SharedGetStationID();
                pstP2PSendHead->ucDstStaID = pstP2PBlock->stP2PBlockHead.uiTargetStationID;
                pstP2PSendHead->ucFrameType = P2P_REQ_FRAME;
                pstP2PSendHead->ucL1NetID = 0U;
                pstP2PSendHead->ucLinkID = 0U;

                uiBuffIndex += usMakeReqDataLen;
                pstP2PSendHead->uiTotalLen = uiBuffIndex + sizeof(uiCalcCRC);

                if((true == bMakeReqRet) && (pstP2PSendHead->uiTotalLen <= P2P_SEND_BUFF_SIZE))
                {
                    /* 填充头部信息 */
                    if((pstP2PBlock->stP2PBlockHead.ucCMID1 != 0U) && (pstP2PBlock->stP2PBlockHead.ucCMID2 != 0U) && \
                       (pstP2PBlock->stP2PBlockHead.ucCMID2 != pstP2PBlock->stP2PBlockHead.ucCMID1))
                    {
                        /* Link 1 */
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID1;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID1 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP1;
                        pstP2PSendHead->uiDstIP2Addr = 0U;
                        pstP2PSendHead->ucLinkID = 1U;

                        uiCalcCRC = SysCrc32Cal( 0, &ucP2PSendbuff[0], uiBuffIndex);
                        memcpy(&ucP2PSendbuff[uiBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
                        usRet = uiBuffIndex + sizeof(uiCalcCRC);

                        /*将数据交内部通讯转发CM*/
                        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                        emCMIndex = SysCheckCMSlotID(pstP2PSendHead->ucSrcCMID, pstCMSlotIdTabl);
                        if(emCMIndex < NUM_OF_CM)
                        {
                            if( !SysSetP2PMsg(emCMIndex,P2P_PM_REQ,ucP2PSendbuff,usRet))
                            {
                                //printf( "SSPPRF\n" );
                            }
                            else
                            {
                                /*printf( "SysSetPMP2PReqMsg OK\n" );*/
                            }
                        }

                        /* Link 2 */
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID2;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID2 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP2;
                        pstP2PSendHead->uiDstIP2Addr = 0U;
                        pstP2PSendHead->ucLinkID = 2U;

                        uiCalcCRC = SysCrc32Cal( 0, &ucP2PSendbuff[0], uiBuffIndex);
                        memcpy(&ucP2PSendbuff[uiBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
                        usRet = uiBuffIndex + sizeof(uiCalcCRC);


                        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                        emCMIndex = SysCheckCMSlotID(pstP2PSendHead->ucSrcCMID, pstCMSlotIdTabl);
                        if(emCMIndex < NUM_OF_CM)
                        {
                            if( !SysSetP2PMsg(emCMIndex,P2P_PM_REQ,ucP2PSendbuff,usRet))
                            {
                                //printf( "SSPPRF\n" );
                            }
                            else
                            {
                                /*printf( "SysSetPMP2PReqMsg OK\n" );*/
                            }
                        }
                    }
                    else if((pstP2PBlock->stP2PBlockHead.ucCMID1 != 0U) && (pstP2PBlock->stP2PBlockHead.ucCMID2 != 0U) && \
                            (pstP2PBlock->stP2PBlockHead.ucCMID2 == pstP2PBlock->stP2PBlockHead.ucCMID1))
                    {
                        /* 发送给同一个CM */
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID1;
                        pstP2PSendHead->ucSrcNETID = 2;
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP1;
                        pstP2PSendHead->uiDstIP2Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP2;
                        pstP2PSendHead->ucL1NetID = pstP2PBlock->stP2PBlockHead.ucNetID1 - 1;  /* Cfg: 1-Net1, 2-Net2*/

                        uiCalcCRC = SysCrc32Cal( 0, &ucP2PSendbuff[0], uiBuffIndex);
                        memcpy(&ucP2PSendbuff[uiBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
                        uiBuffIndex += sizeof(uiCalcCRC);
                        usRet = (uint16_t)(uiBuffIndex);

                        
                        /*将数据交内部通讯转发CM*/
                        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                        emCMIndex = SysCheckCMSlotID(pstP2PSendHead->ucSrcCMID, pstCMSlotIdTabl);
                        if(emCMIndex < NUM_OF_CM)
                        {
                            if( !SysSetP2PMsg(emCMIndex,P2P_PM_REQ,ucP2PSendbuff,usRet))
                            {
                                //printf( "SSPPRF\n" );
                            }
                            else
                            {
                                /*printf( "SysSetPMP2PReqMsg OK\n" );*/
                            }
                        }
                    }
                    else if(pstP2PBlock->stP2PBlockHead.ucCMID1 != 0U)
                    {
                        /* 发送给指定的CM */
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID1;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID1 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP1;
                        pstP2PSendHead->uiDstIP2Addr = 0U;

                        uiCalcCRC = SysCrc32Cal( 0, &ucP2PSendbuff[0], uiBuffIndex);
                        memcpy(&ucP2PSendbuff[uiBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
                        uiBuffIndex += sizeof(uiCalcCRC);
                        usRet = (uint16_t)(uiBuffIndex);

                        /*将数据交内部通讯转发CM*/
                        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                        emCMIndex = SysCheckCMSlotID(pstP2PSendHead->ucSrcCMID, pstCMSlotIdTabl);
                        if(emCMIndex < NUM_OF_CM)
                        {
                            if( !SysSetP2PMsg(emCMIndex,P2P_PM_REQ,ucP2PSendbuff,usRet))
                            {
                                //printf( "SSPPRF\n" );
                            }
                            else
                            {
                                /*printf( "SysSetPMP2PReqMsg OK\n" );*/
                            }
                        }
                    }
                    else if(pstP2PBlock->stP2PBlockHead.ucCMID2 != 0U)
                    {
                        /* 发送给指定的CM */
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID2;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID2 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP2;
                        pstP2PSendHead->uiDstIP2Addr = 0U;

                        uiCalcCRC = SysCrc32Cal( 0, &ucP2PSendbuff[0], uiBuffIndex);
                        memcpy(&ucP2PSendbuff[uiBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
                        uiBuffIndex += sizeof(uiCalcCRC);
                        usRet = (uint16_t)(uiBuffIndex);

                        /*当据长度大于零时，将数据交内部通讯转发CM*/
                        pstCMSlotIdTabl = SharedGetCMSlotIdTabl();
                        emCMIndex = SysCheckCMSlotID(pstP2PSendHead->ucSrcCMID, pstCMSlotIdTabl);
                        if(emCMIndex < NUM_OF_CM)
                        {
                            if( !SysSetP2PMsg(emCMIndex,P2P_PM_REQ,ucP2PSendbuff,usRet))
                            {
                                //printf( "SSPPRF\n" );
                            }
                            else
                            {
                                /*printf( "SysSetPMP2PReqMsg OK\n" );*/
                            }
                        }
                    }
                    else
                    {
                        /* log，配置信息错误 */
                        usRet = 0U;
                        //printf( "PSBCE\n" );
                    }
                }
            }
        }
    }
    
    return usRet;
}

void CommP2PSendCtrlClear(uint8_t ucBlkID)
{
    if(ucBlkID <= MAX_P2P_BLOCK_NUM)
    {
        memset(&s_stP2PSendCtrl[ucBlkID], 0x00, sizeof(P2PSendCtrl_t));
    }
    return;
}

static void CommSOEInit(void)
{
    return;
}

static void CommSOECycle(void)
{
    CMController_t emCMIndex = CM1;                     /* CM 索引*/
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint8_t ucACKData[4*1024];
    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
    uint16_t usSOEDataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* SOE数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* SOE数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上SOE服务信息         */
    
    
    static uint8_t uctest = 0U;
    if(uctest == 10U)
    {
//        printf("CommSOECycle \n");
        uctest = 0U;
    }
    uctest++;
    

    for(emCMIndex = 0;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
//        memset(&ucACKData[0], 0x00,4*1024);
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,SOE_SRV);
        
        if(pstSysSrvInfo != NULL)
        {
            memset((puint8_t)pstSysSrvInfo, 0x00, 100);

            /*从系统资源上获取上位机请求数据*/
            SysGetMsg(emCMIndex,SOE_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usSOEDataLen);
    
            if(usSOEDataLen > 0)
            {
                
                printf("Get SOE Srv! %d\n",usSOEDataLen);
                
                pstMSGHead = (MSGHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
    
                /*计算数据包的CRC*/
                uiCalCrc = SysCrc32Cal(0x00, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], pstMSGHead->usLength);
                
                usBuffIndex = pstMSGHead->usLength;
                /*获取下发的数据CRC*/
                memcpy((puint8_t)&uiReadCrc, (puint8_t)(&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex]), \
                        CHKSUM_CRC_SIZE);
                      
                if(uiCalCrc != uiReadCrc)
                {/*CRC is not consistent*/
                    /*log*/
                    printf("SOE CRC ERR !\n");
                }
                else if(usSOEDataLen != (pstMSGHead->usLength + CHKSUM_CRC_SIZE))
                {/*data len is not consistent*/
                    /*log*/
                    printf("SOELen ERR !\n");
                }
                else
                {
                    switch(pstMSGHead->usActualType)
                    {
                        case SOE_READ_RECORD:
                        {
                            /*process read SOE record service*/
                            printf("SOE Read !\n");
                            usACKDataLen = CommSOERead(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                                        pstMSGHead->usLength, &ucACKData[0]);
                        }
                        break;
    
                        case SOE_DELETE_RECORD:
                        {
                            /*process delete SOE record service*/
                            printf("SOE Delete !\n");
                            usACKDataLen = CommSOEDelete(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                                          pstMSGHead->usLength, &ucACKData[0]);
                        }
                        break;
    
                        default:
                        {
                            printf("ActualType ERR %d\n",pstMSGHead->usActualType);
                        }
                        break;
                    }
                }
                if(usACKDataLen > 0)
                {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
                    printf("SOE HAS RSP ! %d\n",usACKDataLen);
                    
                    SysSetMsg(emCMIndex,SOE_RESP,ucACKData,usACKDataLen);
                }
            }
        }
        else
        {
            if(uctest == 5U)
            {
                printf("pSrvInfo NULL\n");
            }
        }
    }
    return; 
}

static uint16_t CommSOERead(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                  /*服务处理结果错误码*/
    uint16_t usACKDataLen = 0;
    PMController_t emLocalID = PMA;
    uint16_t usSOEEntryNum = 0;
    uint16_t usIndex = 0;
    uint32_t uiCalcCRC = 0;
    SOEType_t emSOEType = 0;
    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
    SOEReadReq_t* pstSOEReadReq;                        /* 读SOE请求结构*/
    SOEReadRsp_t* pstSOEReadRsp;                        /* 读SOE应答结构*/
    SoftSOE_t stSoftSoe;                                /* 软SOE记录结构*/
    HardSOE_t stHardSoe;                                /* 硬SOE记录结构*/
    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
    int32_t iReadRet = 0;
    uint16_t usReadIndex = 0;
    SysTime_t stSysTime;
    uint32_t uiNextIndex = 0U;

    if(NULL == ucSOEData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        usBuffIndex += sizeof(MSGHead_t);
        pstSOEReadReq = (SOEReadReq_t*)(&ucSOEData[usBuffIndex]);
        printf("SOE RD: %d %d %d\n",pstSOEReadReq->ucPMID,pstSOEReadReq->ucSOEBlockID,pstSOEReadReq->usGetIndex);
        emLocalID = SysGetLocalPMID();
        if(pstSOEReadReq->ucPMID != emLocalID)
        {
            ucSubSeason = ERR_SOE_READ;
            printf("PMID ERR\n");
            
        }
        else if(SharedGetSOEActive(pstSOEReadReq->ucSOEBlockID) != ACTIVE)
        {
            ucSubSeason = ERR_SOE_READ;
            printf("B %d NOT ACTIVE\n",pstSOEReadReq->ucSOEBlockID);
        }
        else
        {
            emSOEType = SharedGetSOEType(pstSOEReadReq->ucSOEBlockID);  /*获取SOE块的软硬属性*/
            
            usSOEEntryNum = MAX_SOE_READ_NUM;
            
            printf("SOEType %d\n",emSOEType);
            
            /*清空，供应答缓存使用*/
            usBuffIndex = 0U;
            usBuffIndex += sizeof(ACKHead_t);
            pstSOEReadRsp = (SOEReadRsp_t*)(&ucACKData[usBuffIndex]);
            usBuffIndex +=  sizeof(SOEReadRsp_t);
            
            usReadIndex = pstSOEReadReq->usGetIndex;
            stSysTime.uiSec = pstSOEReadReq->uiSec;
            stSysTime.uiMicSec = pstSOEReadReq->uiMicSec;
            for(usIndex = 0; usIndex < usSOEEntryNum; usIndex++)
            {
                if(SW_SOE == emSOEType)
                {
                    iReadRet = SharedGetSOE(pstSOEReadReq->ucSOEBlockID, &stSoftSoe, usReadIndex,&uiNextIndex,&stSysTime);
                    
                    if((iReadRet >= 0) || (THE_LAST_SOE_IN_QUEUE == iReadRet))
                    {
                        
                        memcpy((puint8_t)&ucACKData[usBuffIndex],&stSoftSoe,sizeof(SoftSOE_t));
                        usBuffIndex += sizeof(SoftSOE_t);
                        stSysTime.uiSec = stSoftSoe.uiSec;
                        stSysTime.uiMicSec = stSoftSoe.uiMicroSec;
                        
                        if(THE_LAST_SOE_IN_QUEUE == iReadRet)
                        {
                            usSOEEntryNum = usIndex + 1;
                            printf("LAST SOE %d %d\n",usSOEEntryNum,uiNextIndex);
                            break;
                        }
                        usReadIndex = (uint16_t)uiNextIndex;
                    }
                    else if(SOE_QUEUE_IS_EMPTY_NOW == iReadRet)
                    {
                        usSOEEntryNum = 0;
                        uiNextIndex = 0xffff;
                        stSysTime.uiSec = 0;
                        stSysTime.uiMicSec = 0;
                        usBuffIndex = sizeof(ACKHead_t) + sizeof(SOEReadRsp_t);
                        
                        printf("SOE_QUEUE_EMPTY\n");
                        break;
                    }
                    else if(READ_SOE_QUEUE_NO_NEW_RECORD == iReadRet)
                    {
                        usSOEEntryNum = usIndex;
                        uiNextIndex = usReadIndex;
                        usBuffIndex = sizeof(ACKHead_t) + sizeof(SOEReadRsp_t);
                        printf("SOE_NO_NEW_RECORD\n");
                        break;
                    }
                    else
                    {
                        ucSubSeason = ERR_SOE_READ;
                        
                        printf("ERR_SOE_READ %d\n",iReadRet);
                        
                        break;
                    }
                }
                if(HW_SOE == emSOEType)
                {
                    iReadRet = SharedGetHardSOE(&stHardSoe, usReadIndex, &uiNextIndex, &stSysTime);
                    
                    if((iReadRet >= 0) || (THE_LAST_SOE_IN_QUEUE == iReadRet))
                    {
                        
                        memcpy((puint8_t)&ucACKData[usBuffIndex],&stHardSoe,sizeof(HardSOE_t));
                        usBuffIndex += sizeof(HardSOE_t);
                        stSysTime.uiSec = stHardSoe.uiSec;
                        stSysTime.uiMicSec = stHardSoe.uiMicroSec;
                        
                        if(THE_LAST_SOE_IN_QUEUE == iReadRet)
                        {
                            usSOEEntryNum = usIndex + 1;
                            printf("LAST SOE %d %d\n",usSOEEntryNum,uiNextIndex);
                            break;
                        }
                        usReadIndex = (uint16_t)uiNextIndex;
                    }
                    else if(SOE_QUEUE_IS_EMPTY_NOW == iReadRet)
                    {
                        usSOEEntryNum = 0;
                        uiNextIndex = 0xffff;
                        stSysTime.uiSec = 0;
                        stSysTime.uiMicSec = 0;
                        usBuffIndex = sizeof(ACKHead_t) + sizeof(SOEReadRsp_t);
                        
                        printf("SOE_QUEUE_IS_EMPTY_NOW\n");
                        
                        break;
                    }
                    else if(READ_SOE_QUEUE_NO_NEW_RECORD == iReadRet)
                    {
                        usSOEEntryNum = usIndex;
                        uiNextIndex = usReadIndex;
                        usBuffIndex = sizeof(ACKHead_t) + sizeof(SOEReadRsp_t);
                        printf("SOE_NO_NEW_RECORD\n");
                        break;
                    }
                    else
                    {
                        ucSubSeason = ERR_SOE_READ;
                        
                        printf("ERR_SOE_READ %d\n",iReadRet);
                        break;
                    }
                }
            }
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucSOEData;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;

        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED; 
            /* 错误应答只应答16字节头*/
            usBuffIndex = sizeof(ACKHead_t);
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
            /*填充读SOE应答的结构*/
            pstSOEReadRsp->ucPMID = pstSOEReadReq->ucPMID;
            pstSOEReadRsp->ucSOEBlockID = pstSOEReadReq->ucSOEBlockID;
            pstSOEReadRsp->usEntryNum = usSOEEntryNum;
            pstSOEReadRsp->usNextIndex = (uint16_t)uiNextIndex;
            pstSOEReadRsp->uiSec = stSysTime.uiSec;
            pstSOEReadRsp->uiMicSec = stSysTime.uiMicSec;
            
            printf("NextIndex: %d\n",pstSOEReadRsp->usNextIndex);
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        /*计算并填充应答CRC*/
        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength);
        memcpy(&ucACKData[usBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
    }
    
    usACKDataLen = (uint16_t)(usBuffIndex);

    return usACKDataLen;
}



static uint16_t CommSOEDelete(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                  /* 服务处理结果错误码*/
    uint16_t usACKDataLen = 0;                          /* 用于返回应答数据长度*/
    PMController_t emLocalID = PMA;
    uint32_t uiCalcCRC = 0;
    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
    MSGHead_t* pstMSGHead;                              /* SOE消息头部*/
    SOEDeleteReq_t* pstSOEDeleteReq;                    /* 删除SOE请求结构*/
    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/

    
    if(NULL == ucSOEData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        usBuffIndex += sizeof(MSGHead_t);
        pstSOEDeleteReq = (SOEDeleteReq_t*)(&ucSOEData[usBuffIndex]);
        emLocalID = SysGetLocalPMID();
        if(pstSOEDeleteReq->ucPMID != emLocalID)
        {
            ucSubSeason = ERR_SOE_CLEAR;
        }
        else if(SharedGetSOEActive(pstSOEDeleteReq->ucSOEBlockID) != ACTIVE)
        {
            ucSubSeason = ERR_SOE_CLEAR;
        }
        else
        {
            SharedClearSOE(pstSOEDeleteReq->ucSOEBlockID);  /*此函数未实现，相关错误处理后续添加*/
        }
        
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucSOEData;
        pstACKHead->ucType = pstMSGHead->ucProtect;
        pstACKHead->ucProtect = pstMSGHead->ucType;
        pstACKHead->usActualType = pstMSGHead->usActualType;
        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
        pstACKHead->ucDataFlg = 0U;
        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
        usBuffIndex = 0U;
        usBuffIndex += sizeof(ACKHead_t);
        if(ucSubSeason != ERR_SUCCESS)
        {/*服务处理失败*/
            pstACKHead->ucResCode = RESCODE_FAILED;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }
        pstACKHead->ucSubReason = ucSubSeason;
        pstACKHead->usLength = (uint16_t)(usBuffIndex);

        uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength);
        memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
    }

    usACKDataLen = (uint16_t)(usBuffIndex);

    return usACKDataLen;
}


static void CommAMSInit(void)
{
    return;
}

static void CommAMSCycle(void)
{
    static CMController_t emCMIndex = CM1;              /* CM 索引*/
    static uint8_t ucSubSeason = ERR_SUCCESS;           /*服务处理结果错误码*/
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint8_t ucACKData[1024];
    MSGHead_t* pstMSGHead = NULL;                       /* AMS消息头部*/
    ACKHead_t* pstACKHead = NULL;                       /*应答头部*/
    uint16_t usAMSDataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* AMS数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* AMS数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    static uint8_t s_ucWaitACKTimeout = 0;
    static IPBUSAPPMailHeader_t s_stSendMailHeader;
    static IPBUSAPPMailHeader_t stRecvMailHeader;
    uint8_t ucModAddr = 0U;
    uint8_t ucModIndex = 0U;
    uint8_t ucChannelID = 0U;
    uint8_t ucMailRet = 0U;
    static uint8_t s_ucRetryTime = 0;
    static AMSMsgHandleStag_t s_emAMSMsgHandleStag = AMS_START;
    static IPBUSMailData_t s_stIPBUSMail;
    static IPBUSMailData_t stRecvIPBUSMail;
    static uint8_t ucAMSMsg[1024];
    uint32_t uiCalcCRC = 0;
    AMSModScan_t stAMSModScan;
    SysIoModInfo_t* pstIoModInfo = NULL;                     /*IO模块配置信息结构指针*/
    AMSModInfo_t* pAMSModInfo = NULL;
    puint16_t pusAMSModInfoNum = NULL;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    switch(s_emAMSMsgHandleStag)
    {
        case AMS_START:
        {
            if(AH_FREE != emAperiodicHandleFlg)
            {
                break;
            }
            /*从系统资源获取来自CM的服务数据*/
//            memset(&ucAMSMsg[0], 0x00, 1024);
            for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
            {
                SysGetMsg(emCMIndex,AMS_REQ,&ucAMSMsg[0],&usAMSDataLen);
                if(usAMSDataLen > 0)
                {
                    break;
                }
            }
            if(usAMSDataLen > 0)
            {/*数据长度大于0，表示收到数据*/
                 /*获取消息头部*/
                pstMSGHead = (MSGHead_t*)&ucAMSMsg[0];
                 /*计算数据CRC*/
                uiCalCrc = SysCrc32Cal(0x00, &ucAMSMsg[0], pstMSGHead->usLength);
                usBuffIndex = pstMSGHead->usLength;
                 /*读取下发的CRC*/
                memcpy((puint8_t)&uiReadCrc,(puint8_t)&ucAMSMsg[usBuffIndex], CHKSUM_CRC_SIZE);

                if(uiCalCrc != uiReadCrc)
                {/*CRC 不一致*/
                    /*log*/
                    ucSubSeason = ERR_MSG_CRC;
                    s_emAMSMsgHandleStag = AMS_END;
                }
                else if(usAMSDataLen != pstMSGHead->usLength + CHKSUM_CRC_SIZE)
                {/*数据长度不一致*/
                    /*log*/
                    ucSubSeason = ERR_MSG_LEN;
                    s_emAMSMsgHandleStag = AMS_END;
                }
                else
                {
                    switch(pstMSGHead->usActualType)
                    {/*根据服务号，处理AMS服务*/
                        case AMS_DWHARTCMD:
                        {
                            emAperiodicHandleFlg = AH_AMS;
                            
                            printf("AMS_DWHARTCMD\n");
                            usBuffIndex = sizeof(MSGHead_t);
                            /*获取从站地址*/
                            memcpy(&ucModAddr, (puint8_t)&ucAMSMsg[usBuffIndex], \
                            sizeof(uint8_t));
                            usBuffIndex += sizeof(uint8_t);
                            /*获取通道号*/
                            memcpy(&ucChannelID, (puint8_t)&ucAMSMsg[usBuffIndex], \
                            sizeof(uint8_t));
                            usBuffIndex += sizeof(uint8_t);
                            
                            if(ucModAddr > 240)
                            {
                                ucSubSeason = ERR_AMS_MODADDR_ERR;
                                s_emAMSMsgHandleStag = AMS_END;
                            }
                            else if(ucChannelID > 32)
                            {
                                ucSubSeason = ERR_AMS_CHANNEL_ERR;
                                s_emAMSMsgHandleStag = AMS_END;
                            }
                            else if(NULL == pstAppend)
                            {
                                ucSubSeason = ERR_AMS_APPEND_ERR;
                                s_emAMSMsgHandleStag = AMS_END;
                            }
                            else
                            {
                                memset(&s_stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                                memset(&s_stSendMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                                
                                s_stIPBUSMail.usDataLen = pstMSGHead->usLength - sizeof(MSGHead_t);
                                
                                /*判断主备情况，发送给主，若无主模块，报错*/
                                if(((pstAppend->stSlaveState[(ucModAddr/2)*2].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                   ((pstAppend->stSlaveState[(ucModAddr/2)*2].stSlaveDiag.ucStateDiag & 0x01) != 0))
                                {
                                    ucModAddr = (ucModAddr/2)*2;
                                }
                                else 
                                {
                                    if(((pstAppend->stSlaveState[(ucModAddr/2)*2 + 1].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                        ((pstAppend->stSlaveState[(ucModAddr/2)*2 + 1].stSlaveDiag.ucStateDiag & 0x01) != 0))
                                    {
                                        ucModAddr = (ucModAddr/2)*2 + 1;
                                    }
                                }
                                /*填入通道号*/
                                s_stIPBUSMail.ucData[0] = ucModAddr;
                                s_stIPBUSMail.ucData[1] = ucChannelID;
                                /*拷贝AMS命令*/
                                memcpy(&s_stIPBUSMail.ucData[2], (puint8_t)&ucAMSMsg[usBuffIndex], \
                                s_stIPBUSMail.usDataLen - 2*sizeof(uint8_t));
                                s_stIPBUSMail.ucIndex = 0x04;
                                
                                s_stSendMailHeader.usMailRX = 0x0001;
                                s_stSendMailHeader.usMailTX = 0x0002;
                                s_stSendMailHeader.usMailNR = s_usMsgSEQ;
                                s_usMsgSEQ++;
                                s_stSendMailHeader.usMailSlaveAddr = ucModAddr;
                                s_stSendMailHeader.usMailFunc = 0x0003;
                                s_stSendMailHeader.usMailLN = s_stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                                s_stSendMailHeader.usMailACK = 0x0001;
                                s_stSendMailHeader.usMailFault = 0x0000;
                                
                                s_ucRetryTime = 0U;
                                
                                ucMailRet = IPBUSMailSend((puint8_t)&s_stIPBUSMail, &s_stSendMailHeader);
                                
                                s_ucRetryTime++;
                                
                                if(IPBUS_RET_OK == ucMailRet)
                                {
                                    s_emAMSMsgHandleStag = AMS_RECV_WR_ACK;
                                }
                                else
                                {
                                    s_emAMSMsgHandleStag = AMS_WR;
                                }
                            }
                        }
                        
                        break;
                    
                        case AMS_SCANIO:
                        {
                            usBuffIndex = sizeof(MSGHead_t);
                            /*获取扫描类型*/
                            memcpy(&stAMSModScan,(puint8_t)&ucAMSMsg[usBuffIndex], sizeof(AMSModScan_t));
                            switch(stAMSModScan.ucScanType)
                            {
                                case AMS_SCAN_ALL:
                                {
                                    usBuffIndex = sizeof(ACKHead_t);
                                    pusAMSModInfoNum = (puint16_t)&ucACKData[usBuffIndex];
                                    usBuffIndex += sizeof(uint16_t);
                                    *pusAMSModInfoNum = 0U;
                                    pAMSModInfo = (AMSModInfo_t*)&ucACKData[usBuffIndex];
                                    
                                    pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM);
                                    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                                    if((pstIoModInfo != NULL) && (pstAppend != NULL))
                                    {
                                        for(ucModIndex = 0U; ucModIndex < 120; ucModIndex++)
                                        {
                                            if((((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2].usModTag) ||
                                                (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2].usModTag)) &&
                                               ((pstAppend->stSlaveState[ucModIndex*2].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                               ((pstAppend->stSlaveState[ucModIndex*2].stSlaveDiag.ucStateDiag & 0x01) != 0)) ||
                                               (((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2+1].usModTag) ||
                                                 (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2+1].usModTag)) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2+1].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2+1].stSlaveDiag.ucStateDiag & 0x01) != 0)))
                                            {
                                                ucModAddr = ucModIndex*2;
                                                pAMSModInfo->usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag;
                                                pAMSModInfo->ucModAddr = ucModAddr;
                                                pAMSModInfo++;
                                                (*pusAMSModInfoNum)++;
                                                usBuffIndex += sizeof(AMSModInfo_t);
                                            }
                                        }
                                    }
                                    
                                    /*服务处理结束，产生应答*/
                                    pstACKHead = (ACKHead_t*)ucACKData;
                                    /*获取消息头部*/
                                    pstMSGHead = (MSGHead_t*)&ucAMSMsg[0];
                                    ucSubSeason = ERR_SUCCESS;
                                    pstACKHead->ucType = pstMSGHead->ucProtect;
                                    pstACKHead->ucProtect = pstMSGHead->ucType;
                                    pstACKHead->usActualType = pstMSGHead->usActualType;
                                    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
                                    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
                                    pstACKHead->ucDataFlg = 0U;
                                    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
                                    pstACKHead->ucResCode = RESCODE_SUCCESS;
                                    pstACKHead->ucDataFlg = 1U;

                                    
                                    pstACKHead->usLength = (uint16_t)(usBuffIndex);
                                    pstACKHead->ucSubReason = ucSubSeason;
                                    /*计算并填充数据应答的CRC*/
                                    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                                    memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                                    usBuffIndex += sizeof(uiCalcCRC);
                                    usACKDataLen = (uint16_t)(usBuffIndex);

                                    SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
                                    s_emAMSMsgHandleStag = FWRITE_START;
                                }
                                break;
                                
                                case AMS_SCAN_SINGLE:
                                {
                                    usBuffIndex = sizeof(ACKHead_t);
                                    pusAMSModInfoNum = (puint16_t)&ucACKData[usBuffIndex];
                                    usBuffIndex += sizeof(uint16_t);
                                    *pusAMSModInfoNum = 0U;
                                    pAMSModInfo = (AMSModInfo_t*)&ucACKData[usBuffIndex];
                                    
                                    pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM);
                                    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                                    if((pstIoModInfo != NULL) && (pstAppend != NULL))
                                    {
                                        if(0 == (stAMSModScan.ucModAddr%2))
                                        {
                                            ucModIndex = stAMSModScan.ucModAddr/2;
                                            if((((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2].usModTag) ||
                                                 (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2].usModTag)) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2].stSlaveDiag.ucStateDiag & 0x01) != 0)) ||
                                               (((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2+1].usModTag) ||
                                                 (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModIndex*2+1].usModTag)) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2+1].stSlaveDiag.ucOnlineDiag & 0x01) != 0) &&
                                                ((pstAppend->stSlaveState[ucModIndex*2+1].stSlaveDiag.ucStateDiag & 0x01) != 0)))
                                            {
                                                ucModAddr = ucModIndex*2;
                                                pAMSModInfo->usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag;
                                                pAMSModInfo->ucModAddr = ucModAddr;
                                                pAMSModInfo++;
                                                (*pusAMSModInfoNum) = 1U;
                                                usBuffIndex += sizeof(AMSModInfo_t);
                                            }
                                        }
                                    }
                                    
                                    /*服务处理结束，产生应答*/
                                    pstACKHead = (ACKHead_t*)ucACKData;
                                    /*获取消息头部*/
                                    pstMSGHead = (MSGHead_t*)&ucAMSMsg[0];
                                    ucSubSeason = ERR_SUCCESS;
                                    pstACKHead->ucType = pstMSGHead->ucProtect;
                                    pstACKHead->ucProtect = pstMSGHead->ucType;
                                    pstACKHead->usActualType = pstMSGHead->usActualType;
                                    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
                                    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
                                    pstACKHead->ucDataFlg = 0U;
                                    pstACKHead->ucFlag = ACK_FRAME_SINGLE;
                                    pstACKHead->ucResCode = RESCODE_SUCCESS;
                                    pstACKHead->ucDataFlg = 1U;

                                    pstACKHead->usLength = (uint16_t)(usBuffIndex);
                                    pstACKHead->ucSubReason = ucSubSeason;
                                    /*计算并填充数据应答的CRC*/
                                    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                                    memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                                    usBuffIndex += sizeof(uiCalcCRC);
                                    usACKDataLen = (uint16_t)(usBuffIndex);

                                    SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
                                    s_emAMSMsgHandleStag = FWRITE_START;
                                }
                                break;
                                
                                default:
                                break;
                            }
                        }
                        
                        break;
                    
                        default:
                        break;
                    }
                }
            }
        }
        
        break;
        
        case AMS_WR:
        {
            if(s_ucRetryTime >= 2)
            {/*重发次数超过3次，不再处理*/
                ucSubSeason = ERR_AMS_TIMEOUT;
                s_emAMSMsgHandleStag = AMS_END;
            }
            else
            {
                ucMailRet = IPBUSMailSend((puint8_t)&s_stIPBUSMail, &s_stSendMailHeader);
                s_ucRetryTime++;
                
                if(IPBUS_RET_OK == ucMailRet)
                {
                    s_emAMSMsgHandleStag = AMS_RECV_WR_ACK;
                }
                else
                {
                    s_emAMSMsgHandleStag = AMS_WR;
                }
            }
        }
        
        break;
        
        case AMS_RECV_WR_ACK:
        {
            memset(&stRecvMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
            memset(&stRecvIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
            ucMailRet = IPBUSMailRecv((puint8_t)&stRecvIPBUSMail, &stRecvMailHeader);
            if(stRecvMailHeader.usMailLN > 0U)
            {
                if((stRecvMailHeader.usMailFunc & 0x0080) != 0x00)
                {
                    s_emAMSMsgHandleStag = AMS_WR;
                    printf("AMS_WR_ACK err %x\n",stRecvMailHeader.usMailFunc);
                }
                else
                {
                    printf("AMS_WR_ACK\n");
                    s_emAMSMsgHandleStag = AMS_RD;
                    /*组织读Hart命令*/
                    s_stIPBUSMail.ucIndex = 0x05;
                    s_stSendMailHeader.usMailFunc = 0x0002;
                    s_stSendMailHeader.usMailNR = s_usMsgSEQ;
                    s_usMsgSEQ++;
                    s_ucRetryTime = 0U;
                }
                s_ucWaitACKTimeout = 0U;
            }
            else
            {
                s_ucWaitACKTimeout++;
                if(s_ucWaitACKTimeout > 5)
                {
                    s_ucWaitACKTimeout = 0U;
                    s_emAMSMsgHandleStag = AMS_WR;
                }
            }
        }
        
        break;
        
        case AMS_RD:
        {
            if(s_ucRetryTime >= 10)
            {/*重发次数超过10次，不再处理*/
                ucSubSeason = ERR_AMS_TIMEOUT;
                s_emAMSMsgHandleStag = AMS_END;
            }
            else
            {
                ucMailRet = IPBUSMailSend((puint8_t)&s_stIPBUSMail, &s_stSendMailHeader);
                s_ucRetryTime++;
                
                if(IPBUS_RET_OK == ucMailRet)
                {
                    s_emAMSMsgHandleStag = AMS_RECV_RD_ACK;
                }
                else
                {
                    s_emAMSMsgHandleStag = AMS_RD;
                }
            }
        }
        
        break;
        
        case AMS_RECV_RD_ACK:
        {
            memset(&stRecvMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
            memset(&stRecvIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
            ucMailRet = IPBUSMailRecv((puint8_t)&stRecvIPBUSMail, &stRecvMailHeader);
            if(stRecvMailHeader.usMailLN > 0U)
            {
                if((stRecvMailHeader.usMailFunc & 0x0080) != 0x00)
                {
                    s_emAMSMsgHandleStag = AMS_RD;
                }
                else
                {
                    if(stRecvIPBUSMail.usDataLen > 0)
                    {
                        ucSubSeason = ERR_SUCCESS;
                        s_emAMSMsgHandleStag = AMS_END;
                        s_ucWaitACKTimeout = 0U;
                    }
                }
            }
            else
            {
                s_ucWaitACKTimeout++;
                if(s_ucWaitACKTimeout > 10)
                {
                    s_ucWaitACKTimeout = 0U;
                    s_emAMSMsgHandleStag = AMS_RD;
                }
            }
        }
        
        break;
        
        case AMS_END:
        {/*服务处理结束，产生应答*/
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            /*获取消息头部*/
            pstMSGHead = (MSGHead_t*)&ucAMSMsg[0];
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
                memcpy(&ucACKData[usBuffIndex], (puint8_t)&stRecvIPBUSMail.ucData[0], stRecvIPBUSMail.usDataLen);
                ucACKData[usBuffIndex] = (ucACKData[usBuffIndex] / 2) * 2;
                
                usBuffIndex += stRecvIPBUSMail.usDataLen;
            }
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            pstACKHead->ucSubReason = ucSubSeason;
            /*计算并填充数据应答的CRC*/
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(usBuffIndex);

            SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
            s_emAMSMsgHandleStag = AMS_START;
            
            ucSubSeason = ERR_SUCCESS;
            
            emAperiodicHandleFlg = AH_FREE;
        }
        
        break;
        
        default:
        break;
    }
    return;
}


static void CommAperiodicInit(void)
{
    memset(&s_ucModOnlineFlg[0], 0x00, MAX_SLAVE_NUM);
    emAperiodicHandleFlg = AH_FREE;
    return;
}

static void CommAperiodicCycle(void)
{
    
    CommGetSlaveVern();
    CommAlarmCycle();
    
    return;
}

static void CommSlaveFWUpdate(void)
{
    static UpdateIOStag_t s_emUpdateIOStag = UPIO_WR_START;
    static UpdateIOStag_t s_emLastUpdateIOStag = UPIO_WR_START;
    IPBUSAPPMailHeader_t stMailHeader;
    IPBUSMailData_t stIPBUSMail;
    uint8_t ucMailRet = IPBUS_RET_NOTOK;
    static uint16_t s_usSubPackNum = 0U;
    uint16_t usLen = 0U;
    uint16_t usTotalPackNum = 0U;
    uint32_t uiBuffIndex = 0U;
    uint16_t usCalcCRC = 0U;
    static uint16_t s_ucWaitACKTimeout = 0U;
    uint16_t usReadFWFileLen = 0U;
    static uint8_t s_ucLastPackFlg = 0U;
    static uint16_t s_usTest1 = 0U;
    uint8_t ucStatusFlg = 0U;
    static uint16_t s_usWaitCount = 0U;
    puint8_t pucAddr = NULL;
    SharedAppend_t *pstAppend = NULL;
    static SyncTime64_t unLastSendTime;                             /* 上次发送数据时间             */
    SyncTime64_t unCurTime;                                         /* 当前同步时间             */

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    
    if(((HAS_IO_FW_UPDATE == s_uiFWUpdateFlg) || (NEED_ERASE_IO_FW == s_uiFWUpdateFlg)) && 
       ((AH_FREE == emAperiodicHandleFlg) || (AH_IO_FW == emAperiodicHandleFlg)))
    {
        SysGetSyncTime(&unCurTime);
        
        switch(s_emUpdateIOStag)
        {
            case UPIO_WR_START:
            {
                if(NEED_ERASE_IO_FW == s_uiFWUpdateFlg)
                {
                    emAperiodicHandleFlg = AH_IO_FW;
                    
                    if((s_stSlaveFWInfo.ucSlotID < MAX_MOD_NUM) && (s_stSlaveFWInfo.uiFileNameLen < MAX_FW_FILE_NAME_LEN) && 
                       (s_stSlaveFWInfo.uiIOFWLen < MAX_FW_LEN))
                    {
                        memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                        memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
        
                        stIPBUSMail.ucIndex = 0x01;
                        
                        s_usSubPackNum = 0U;
                        memcpy(&stIPBUSMail.ucData[0], &s_usSubPackNum, sizeof(uint16_t));
                        uiBuffIndex = sizeof(uint16_t);
                        
                        usLen = sizeof(usTotalPackNum) + s_stSlaveFWInfo.uiFileNameLen;
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], &usLen, sizeof(uint16_t));
                        uiBuffIndex += sizeof(uint16_t);
                        
                        usTotalPackNum = s_stSlaveFWInfo.uiIOFWLen / MAX_FW_PACK_LEN;
                        if((s_stSlaveFWInfo.uiIOFWLen % MAX_FW_PACK_LEN) != 0U)
                        {
                            usTotalPackNum = usTotalPackNum + 1;
                        }
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], &usTotalPackNum, sizeof(uint16_t));
                        uiBuffIndex += sizeof(uint16_t);
                        
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], &s_stSlaveFWInfo.ucFileName[0], s_stSlaveFWInfo.uiFileNameLen);
                        uiBuffIndex += s_stSlaveFWInfo.uiFileNameLen;
                        
                        usCalcCRC = SysCrc16Cal(&stIPBUSMail.ucData[0], (uint16_t)uiBuffIndex);
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], &usCalcCRC, sizeof(uint16_t));
                        uiBuffIndex += sizeof(uint16_t);
                        stIPBUSMail.usDataLen = uiBuffIndex;

                        stMailHeader.usMailRX = 0x0001;
                        stMailHeader.usMailTX = 0x0002;
                        stMailHeader.usMailNR = s_usMsgSEQ;
                        s_usMsgSEQ++;

                        stMailHeader.usMailSlaveAddr = s_stSlaveFWInfo.ucSlotID;
                        stMailHeader.usMailFunc = 0x0003;
                        stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                        stMailHeader.usMailACK = 0x0001;
                        stMailHeader.usMailFault = 0x0000;
                        
                        ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                        
                        if(IPBUS_RET_OK == ucMailRet)
                        {
                            s_emLastUpdateIOStag = s_emUpdateIOStag;
                            s_emUpdateIOStag = UPIO_WR_START_ACK;
                        }
                        else
                        {
                            s_emUpdateIOStag = UPIO_END;
                            
                            s_ucFWUpdateRest = 2;
                            pstAppend->ucFWUpdateFlg = 2;
                        }
                        
                        s_stSlaveFWInfo.uiFileReadIndex = 0U;
                        
                        SysGetSyncTime(&unLastSendTime);
                    }
                    else
                    {
                        s_emUpdateIOStag = UPIO_END;
                    }
                }
                else
                {/*没有IO在线升级，更新发送时间*/
                    SysGetSyncTime(&unLastSendTime);
                }
            }
            break;
            
            case UPIO_WR_START_ACK:
            {
                memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
                if(stMailHeader.usMailLN > 0U)
                {
                    if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                    {
                        s_emUpdateIOStag = UPIO_END;
                        
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                    }
                    else
                    {
                        s_emUpdateIOStag = UPIO_RD_STATUS;
                    }
                    s_ucWaitACKTimeout = 0U;
                }
                else
                {
                    s_ucWaitACKTimeout++;
                    if(s_ucWaitACKTimeout > 2000)
                    {
                        s_ucWaitACKTimeout = 0U;
                        s_emUpdateIOStag = UPIO_END;
                        
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                    }
                }
            }
            break;
            
            case UPIO_WR_CONTINUE:
            {
                if(HAS_IO_FW_UPDATE == s_uiFWUpdateFlg)
                {
                    if((s_stSlaveFWInfo.uiIOFWLen < MAX_FW_LEN) && 
                       (s_stSlaveFWInfo.uiFileReadIndex <= s_stSlaveFWInfo.uiIOFWLen))
                    {
                        memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                        memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
        
                        stIPBUSMail.ucIndex = 0x01;
                        
                        s_usSubPackNum++;
                        memcpy(&stIPBUSMail.ucData[0], &s_usSubPackNum, sizeof(uint16_t));
                        uiBuffIndex = sizeof(uint16_t);
                        
                        usReadFWFileLen = MIN(MAX_FW_PACK_LEN, (s_stSlaveFWInfo.uiIOFWLen - s_stSlaveFWInfo.uiFileReadIndex));
                        pucAddr = SysUpdateGetBuff(s_stSlaveFWInfo.uiFileReadIndex);
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], pucAddr, usReadFWFileLen);
                        uiBuffIndex += usReadFWFileLen;
                        
                        usCalcCRC = SysCrc16Cal(&stIPBUSMail.ucData[0], (uint16_t)uiBuffIndex);
                        memcpy(&stIPBUSMail.ucData[uiBuffIndex], &usCalcCRC, sizeof(uint16_t));
                        uiBuffIndex += sizeof(uint16_t);
                        stIPBUSMail.usDataLen = uiBuffIndex;
                        
                        
                        stMailHeader.usMailRX = 0x0001;
                        stMailHeader.usMailTX = 0x0002;
                        stMailHeader.usMailNR = s_usMsgSEQ;
                        s_usMsgSEQ++;
                        stMailHeader.usMailSlaveAddr = s_stSlaveFWInfo.ucSlotID;
                        stMailHeader.usMailFunc = 0x0003;
                        stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                        stMailHeader.usMailACK = 0x0001;
                        stMailHeader.usMailFault = 0x0000;
                        
                        ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                        
                        if(IPBUS_RET_OK == ucMailRet)
                        {
                            s_emLastUpdateIOStag = s_emUpdateIOStag;
                            s_emUpdateIOStag = UPIO_WR_CONTINUE_ACK;
                        }
                        else
                        {
                            s_emUpdateIOStag = UPIO_END;
                            
                            s_ucFWUpdateRest = 2;
                            pstAppend->ucFWUpdateFlg = 2;
                        }
                        s_stSlaveFWInfo.uiFileReadIndex += usReadFWFileLen;
                        if(s_stSlaveFWInfo.uiFileReadIndex >= s_stSlaveFWInfo.uiIOFWLen)
                        {
                            s_ucLastPackFlg = LAST_PACK_FLG;
                        }
                        
                        SysGetSyncTime(&unLastSendTime);
                        
                    }
                }
                else
                {
                    s_emUpdateIOStag = UPIO_RD_STATUS;
                }
            }
            break;
            
            case UPIO_WR_CONTINUE_ACK:
            {
                memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
                if(stMailHeader.usMailLN > 0U)
                {
                    if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                    {
                        s_emUpdateIOStag = UPIO_END;
                        
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                        printf("WR_CONT_ACK err %x\n",stMailHeader.usMailFunc);
                    }
                    else
                    {
                        s_emUpdateIOStag = UPIO_RD_STATUS;
                    }
                    s_ucWaitACKTimeout = 0U;
                }
                else
                {
                    s_ucWaitACKTimeout++;
                    if(s_ucWaitACKTimeout > 2000)
                    {
                        s_ucWaitACKTimeout = 0U;
                        
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                        s_emUpdateIOStag = UPIO_END;
                    }
                }
            }
            break;
            
            case UPIO_RD_STATUS:
            {
                memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));

                stIPBUSMail.ucIndex = 0x06;
                
                memcpy(&stIPBUSMail.ucData[0], &s_usSubPackNum, sizeof(uint16_t));
                uiBuffIndex = sizeof(uint16_t);
                
                usCalcCRC = SysCrc16Cal(&stIPBUSMail.ucData[0], (uint16_t)uiBuffIndex);
                memcpy(&stIPBUSMail.ucData[uiBuffIndex], &usCalcCRC, sizeof(uint16_t));
                uiBuffIndex += sizeof(uint16_t);
                stIPBUSMail.usDataLen = uiBuffIndex;
                
                
                stMailHeader.usMailRX = 0x0001;
                stMailHeader.usMailTX = 0x0002;
                stMailHeader.usMailNR = s_usMsgSEQ;
                s_usMsgSEQ++;
                stMailHeader.usMailSlaveAddr = s_stSlaveFWInfo.ucSlotID;
                stMailHeader.usMailFunc = 0x0002;
                stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                stMailHeader.usMailACK = 0x0001;
                stMailHeader.usMailFault = 0x0000;
                
                ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                
                if(IPBUS_RET_OK == ucMailRet)
                {
                    s_emUpdateIOStag = UPIO_RD_STATUS_ACK;
                }
                else
                {
                    s_ucFWUpdateRest = 2;
                    pstAppend->ucFWUpdateFlg = 2;
                    s_emUpdateIOStag = UPIO_END;
                }
            }
            break;
            
            case UPIO_RD_STATUS_ACK:
            {
                memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
                if(stMailHeader.usMailLN > 0U)
                {
                    if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                    {
                        s_emUpdateIOStag = UPIO_END;
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                        printf("RD_STAT_ACK err %x\n",stMailHeader.usMailFunc);
                    }
                    else
                    {
                        ucStatusFlg = stIPBUSMail.ucData[0];
                        if(UPIO_WR_START == s_emLastUpdateIOStag)
                        {
                            if((ucStatusFlg & 0x01) != 0)
                            {
                                if((ucStatusFlg & 0x10) != 0)
                                {
                                    s_emUpdateIOStag = UPIO_RD_WAIT;
                                }
                                else
                                {
                                    s_emUpdateIOStag = UPIO_WR_CONTINUE;
                                }
                            }
                            else
                            {
                                s_emUpdateIOStag = UPIO_END;
                                s_ucFWUpdateRest = 2;
                                pstAppend->ucFWUpdateFlg = 2;
                                printf("IO Not Ready\n");
                            }
                        }
                        
                        if(UPIO_WR_CONTINUE == s_emLastUpdateIOStag)
                        {
                            if(LAST_PACK_FLG == s_ucLastPackFlg)
                            {
                                s_emUpdateIOStag = UPIO_END;
                                if((ucStatusFlg & 0x20) != 0)
                                {
                                    printf("IO Update OK\n");
                                    
                                    s_ucFWUpdateRest = 1;
                                    pstAppend->ucFWUpdateFlg = 1;
                                }
                                else if((ucStatusFlg & 0x40) != 0)
                                {
                                    printf("IO Update Fail\n");
                                    s_ucFWUpdateRest = 2;
                                    pstAppend->ucFWUpdateFlg = 2;
                                }
                                else if((ucStatusFlg & 0x10) != 0)
                                {
                                    s_emUpdateIOStag = UPIO_RD_STATUS;
                                }
                                else
                                {
                                    printf("Err StatusFlg %x\n",ucStatusFlg);
                                }
                            }
                            else
                            {
                                if((ucStatusFlg & 0x8E) != 0)
                                {
                                    printf("IO Update Err %x\n",ucStatusFlg);
                                    s_emUpdateIOStag = UPIO_END;
                                    s_ucFWUpdateRest = 2;
                                    pstAppend->ucFWUpdateFlg = 2;
                                }
                                else
                                {
                                    s_emUpdateIOStag = UPIO_WR_CONTINUE;
                                }
                            }
                        }
                    }
                    s_ucWaitACKTimeout = 0U;
                }
                else
                {
                    s_ucWaitACKTimeout++;
                    if(s_ucWaitACKTimeout > 2000)
                    {
                        s_ucWaitACKTimeout = 0U;
                        s_emUpdateIOStag = UPIO_END;
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                        printf("RD_STAT_ACK TO\n");
                    }
                }
            }
            break;
            
            case UPIO_RD_WAIT:
            {
                s_usWaitCount++;
                if(100 == s_usWaitCount)
                {
                    s_usWaitCount = 0;
                    s_emUpdateIOStag = UPIO_RD_STATUS;
                }
            }
            break;
            
            case UPIO_END:
            {
                s_emUpdateIOStag = UPIO_WR_START;
                s_stSlaveFWInfo.uiFileReadIndex = 0U;
                emAperiodicHandleFlg = AH_FREE;
                s_ucLastPackFlg = 0;
                s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;
                
                SysSetPMUpdateFlg(NOT_HAS_FW_UPDATE);
            }
            break;
            
            default:
                
            break;
        }
        
        if(SysDiffSyncTime(&unLastSendTime, &unCurTime) >= (3*60*1000*10))
        {/*当有IO在线升级时，比较时间，当最后一次发送与当前时间大于3min时，结束升级*/
            s_emUpdateIOStag = UPIO_END;
        }
    }
    return;
}


static void CommGetSlaveVern(void)
{
    uint8_t ucModIndex = 0;
    uint8_t ucModOnlineFlg = 0;
    SharedAppend_t *pstAppend = NULL;
    IPBUSAPPMailHeader_t stMailHeader;
    IPBUSMailData_t stIPBUSMail;
    uint8_t ucMailRet = 0U;
    static uint8_t s_ucPollModAddr = 0;
    static ReadSlaveVernStag_t emReadSlaveVernStag = RSV_RD;
    static uint8_t s_ucWaitACKTimeout = 0U;
    
    switch(emReadSlaveVernStag)
    {
        case RSV_RD:
        {
            pstAppend = (SharedAppend_t *)SysGetSharedMemAppendInfoStartAddr();
            
            if((pstAppend != NULL) && (AH_FREE == emAperiodicHandleFlg))
            {
                for(ucModIndex = s_ucPollModAddr; ucModIndex < MAX_SLAVE_NUM; ucModIndex++)
                {
                    ucModOnlineFlg = pstAppend->stSlaveState[ucModIndex].stSlaveDiag.ucOnlineDiag & SLAVE_ONLINE;
                    if(ucModOnlineFlg != s_ucModOnlineFlg[ucModIndex])
                    {
                        if((SLAVE_OFFLINE == s_ucModOnlineFlg[ucModIndex]) && (SLAVE_ONLINE == ucModOnlineFlg))
                        {
                            emAperiodicHandleFlg = AH_SLAVE_VERN;
                            s_ucPollModAddr = ucModIndex;
                            
                            printf("MOD %d ONLN\n",ucModIndex);
                        }

                        s_ucModOnlineFlg[ucModIndex] = ucModOnlineFlg;
                        
                        if((AH_SLAVE_VERN == emAperiodicHandleFlg) && (ucModIndex != 0U))
                        {
                            break;
                        }
                        
                    }
                }
                
                if(AH_SLAVE_VERN == emAperiodicHandleFlg)
                {/*进行读取离线再上线模块的版本*/
                    
                    memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                    memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t)); 

                    stIPBUSMail.ucIndex = 0x01;
                    
                    stMailHeader.usMailRX = 0x0001;
                    stMailHeader.usMailTX = 0x0002;
                    stMailHeader.usMailNR = s_usMsgSEQ;
                    s_usMsgSEQ++;
                    stMailHeader.usMailSlaveAddr = s_ucPollModAddr;
                    stMailHeader.usMailFunc = 0x0002;
                    stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                    stMailHeader.usMailACK = 0x0001;
                    stMailHeader.usMailFault = 0x0000;
                    
                    ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                    
                    if(IPBUS_RET_OK == ucMailRet)
                    {
                        emReadSlaveVernStag = RSV_RECV_RD_ACK;
                        printf("SEND RSV OK %d\n",s_ucPollModAddr);
                    }
                    else
                    {
                        emReadSlaveVernStag = RSV_END;
                        printf("IPBUS_RET_ERR\n");
                    }
                }
            }
        }
        break;
        
        case RSV_RECV_RD_ACK:
        {
            memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
            memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
            ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
            if(stMailHeader.usMailLN > 0U)
            {
                if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                {
                    emReadSlaveVernStag = RSV_END;
                    printf("RSV_RD_ACK err %x\n",stMailHeader.usMailFunc);
                }
                else
                {
                    printf("RSV_RD_ACK %d\n",stIPBUSMail.usDataLen);
                    emReadSlaveVernStag = RSV_END;
                    
                    CommSlaveVernHandle(&stIPBUSMail.ucData[0], s_ucPollModAddr);

                }
                s_ucWaitACKTimeout = 0U;
            }
            else
            {
                s_ucWaitACKTimeout++;
                if(s_ucWaitACKTimeout > 250)
                {
                    s_ucWaitACKTimeout = 0U;
                    emReadSlaveVernStag = RSV_END;
                    printf("RSV_RD_ACK TO\n");
                }
            }
        }
        break;
        
        case RSV_END:
        {
            s_ucPollModAddr = 0;
            emReadSlaveVernStag = RSV_RD;
            emAperiodicHandleFlg = AH_FREE;
            printf("RSV_END\n");
        }
        break;
        
        default:

        break;
    }

    return;
}

static void CommSlaveVernHandle(uint8_t ucSlaveVernData[], uint8_t ucSlaveNum)
{
    SharedAppend_t *pstAppend = NULL;
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    uint32_t uiBuffIndex = 0U;
    uint8_t ucChipNum = 0U;
    uint8_t ucChipIndex = 0U;
    puint8_t pucChipVern = NULL;
    
    if((pstAppend != NULL) && (ucSlaveVernData != NULL))
    {
        if(ucSlaveNum < MAX_SLAVE_NUM)
        {
            pstAppend->stSlaveVern[ucSlaveNum].ucActive = 1;
            pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.ucSlotIndex = ucSlaveNum;
            
            uiBuffIndex = sizeof(uint8_t);
            memcpy(&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.usModuleType, &ucSlaveVernData[uiBuffIndex],
                    sizeof(uint16_t));
            uiBuffIndex += sizeof(uint16_t);
            
            memcpy(&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.ucModVern, &ucSlaveVernData[uiBuffIndex],
                    sizeof(uint8_t));
            uiBuffIndex += sizeof(uint8_t);
            
            memcpy(&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.ucHardwareVern, &ucSlaveVernData[uiBuffIndex],
                    sizeof(uint8_t));
            uiBuffIndex += sizeof(uint8_t);
            
            memcpy(&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.ucSoftwareVern, &ucSlaveVernData[uiBuffIndex],
                    sizeof(uint8_t));
            uiBuffIndex += sizeof(uint8_t);
            
            memcpy(&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.ucModSN[0], &ucSlaveVernData[uiBuffIndex],
                    14*sizeof(uint8_t));
            uiBuffIndex += 14*sizeof(uint8_t);
            
            memcpy(&ucChipNum, &ucSlaveVernData[uiBuffIndex], sizeof(uint8_t));
            uiBuffIndex += sizeof(uint8_t);
            pucChipVern = (puint8_t)&pstAppend->stSlaveVern[ucSlaveNum].stSlaveFirmwareVern.usChip1ID;
            if((pucChipVern != NULL) && (ucChipNum < 10))
            {
                for(ucChipIndex = 0U; ucChipIndex < ucChipNum; ucChipIndex++)
                {
                    memcpy(pucChipVern, &ucSlaveVernData[uiBuffIndex], sizeof(uint16_t));
                    uiBuffIndex += sizeof(uint16_t);
                    pucChipVern += sizeof(uint16_t);
                    
                    memcpy(pucChipVern, &ucSlaveVernData[uiBuffIndex], sizeof(uint8_t));
                    uiBuffIndex += sizeof(uint8_t);
                    pucChipVern += sizeof(uint8_t);
                }
            }
        }
    }
    
    return;
}

static void CommAlarmCycle(void)
{
    uint8_t ucModIndex = 0;
    SharedAppend_t *pstAppend = NULL;
    IPBUSAPPMailHeader_t stMailHeader;
    IPBUSMailData_t stIPBUSMail;
    uint8_t ucMailRet = 0U;
    static uint8_t s_ucPollModAddr = 0;
    static ReadAlarmStag_t emReadAlarmStag = RA_RD;
    static uint8_t s_ucWaitACKTimeout = 0U;
    static uint16_t s_usCounter = 0U;
    static uint8_t s_ucRackID = 0U;
    uint8_t ucMinModAddr = 0U;
    uint8_t ucMaxModAddr = 0U;
    
    s_usCounter++;
    
    switch(emReadAlarmStag)
    {
        case RA_RD:
        {
            pstAppend = (SharedAppend_t *)SysGetSharedMemAppendInfoStartAddr();
            
            if((pstAppend != NULL) && (AH_FREE == emAperiodicHandleFlg) && (s_usCounter >= 100))
            {
                if(s_ucRackID >= MAX_RACK_NUM)
                {
                    s_ucRackID = 0U;
                }
                ucMinModAddr = 16*s_ucRackID;
                ucMaxModAddr = 16*(s_ucRackID + 1);
                if(0 == s_ucRackID)
                {
                    ucMinModAddr = 4;
                }
                
                for(ucModIndex = ucMinModAddr; ucModIndex < ucMaxModAddr; ucModIndex++)
                {
                    if(ucModIndex >= MAX_SLAVE_NUM)
                    {
                        break;
                    }
                    if((SLAVE_ONLINE == (pstAppend->stSlaveState[ucModIndex].stSlaveDiag.ucOnlineDiag & SLAVE_ONLINE)) &&
                       (0 != (pstAppend->stSlaveState[ucModIndex].stSlaveDiag.ucStateDiag & 0x01)))
                    {
                        emAperiodicHandleFlg = AH_SLAVE_ALARM;
                        s_ucPollModAddr = ucModIndex;
                        
//                        printf("Mod %d Onln %d\n",ucModIndex,s_ucRackID);
                        
                        break;
                    }
                }
                
                if((AH_SLAVE_ALARM == emAperiodicHandleFlg) && (s_ucPollModAddr != 0))
                {/*进行读取离线再上线模块的版本*/
                    
                    memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                    memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));

                    stIPBUSMail.ucIndex = 0x04;
                    
                    stMailHeader.usMailRX = 0x0001;
                    stMailHeader.usMailTX = 0x0002;
                    stMailHeader.usMailNR = s_usMsgSEQ;
                    s_usMsgSEQ++;
                    stMailHeader.usMailSlaveAddr = s_ucPollModAddr;
                    stMailHeader.usMailFunc = 0x0002;
                    stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                    stMailHeader.usMailACK = 0x0001;
                    stMailHeader.usMailFault = 0x0000;
                    
                    ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                    
                    if(IPBUS_RET_OK == ucMailRet)
                    {
                        emReadAlarmStag = RA_RECV_RD_ACK;
                        printf("SEND RA OK %d\n",s_ucPollModAddr);
                    }
                    else
                    {
                        emReadAlarmStag = RA_END;
                        printf("IPBUS_RET_ERR\n");
                    }
                    
                    s_ucPollModAddr = 0;
                    s_usCounter = 0;
                }
                else
                {
                    s_ucRackID++;
                    s_usCounter = 0;
                }
            }
        }
        break;
        
        case RA_RECV_RD_ACK:
        {
            memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
            memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
            ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
            if(stMailHeader.usMailLN > 0U)
            {
                if(0x04 == stIPBUSMail.ucIndex)
                {
                    if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                    {
                        emReadAlarmStag = RA_END;
                        printf("RA_RD_ACK err %x\n",stMailHeader.usMailFunc);
                    }
                    else
                    {
                        printf("RA_RD_ACK %d\n",stIPBUSMail.usDataLen);
                        emReadAlarmStag = RA_END;
                        if(stIPBUSMail.usDataLen > 0)
                        {
                            CommBIPWInfoHandle(&stIPBUSMail.ucData[0], s_ucRackID);
                        }
                        
                        s_ucRackID++;
                    }
                    s_ucWaitACKTimeout = 0U;
                }

                
            }
            else
            {
                s_ucWaitACKTimeout++;
                if(s_ucWaitACKTimeout > 50)
                {
                    s_ucWaitACKTimeout = 0U;
                    emReadAlarmStag = RA_END;
                    s_ucRackID++;
                    printf("RA_RD_ACK TO\n");
                }
            }
        }
        break;
        
        case RA_END:
        {
            s_ucPollModAddr = 0;
            emReadAlarmStag = RA_RD;
            emAperiodicHandleFlg = AH_FREE;
            printf("RA_END\n");
        }
        break;
        
        default:
            
        break;
    }
    
    return;
}




static void CommBIPWInfoHandle(uint8_t ucBIPWInfoData[], uint8_t ucRackID)
{
    SharedAppend_t *pstAppend = NULL;
    uint8_t ucBISlotID = 0U;
    uint8_t ucPWSlotID = 0U;
    uint8_t ucValidByteOff = 0U;
    uint8_t ucValidBitOff = 0U;
    uint32_t uiBuffIndex = 0U;
    int16_t sTemperature = 0;
    int8_t cPWTemprature = 0;
    uint8_t ucTempValue = 0U;
    
    pstAppend = (SharedAppend_t *)SysGetSharedMemAppendInfoStartAddr();
    if((pstAppend != NULL) && (ucBIPWInfoData != NULL))
    {
        if(ucRackID < MAX_RACK_NUM)
        {
            ucBISlotID = ucRackID;
            ucPWSlotID = ucRackID;
            pstAppend->stBIVern[ucBISlotID].ucActive = 1;
            pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.ucSlotIndex = ucBISlotID;

            uiBuffIndex = 1U;
            if((ucBIPWInfoData[(99 + (0/8))] & (0x01 << (0 % 8))) != 0)
            {
                memcpy(&pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.usModuleType, &ucBIPWInfoData[uiBuffIndex], sizeof(uint16_t));
            }
            uiBuffIndex += sizeof(uint16_t);
            
            if((ucBIPWInfoData[(99 + (1/8))] & (0x01 << (1 % 8))) != 0)
            {
                memcpy(&pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.ucHardwareVern, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
                memcpy(&pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.ucSoftwareVern, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (2/8))] & (0x01 << (2 % 8))) != 0)
            {
                
                memcpy(&pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.usChipID, &ucBIPWInfoData[uiBuffIndex], sizeof(uint16_t));
                
                memcpy(&pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.ucChipFWVern, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            uiBuffIndex += sizeof(uint16_t);
            uiBuffIndex += sizeof(uint8_t);
            
            pstAppend->stBIState[ucBISlotID].ucActive = 1;
            pstAppend->stBIState[ucBISlotID].stBIDiagInfo.ucSlotIndex = ucBISlotID;
            pstAppend->stBIState[ucBISlotID].stBIDiagInfo.usModuleType = pstAppend->stBIVern[ucBISlotID].stBIFirmwareVern.usModuleType;
            
            memset((puint8_t)&pstAppend->stBIState[ucBISlotID].stBIDiagInfo.sBITemperature, 0xff, 4*sizeof(uint16_t));
            if((ucBIPWInfoData[(99 + (3/8))] & (0x01 << (3 % 8))) != 0)
            {
                memcpy(&sTemperature, &ucBIPWInfoData[uiBuffIndex], sizeof(int16_t));
                pstAppend->stBIState[ucBISlotID].stBIDiagInfo.sBITemperature = sTemperature / 16;
            }
            uiBuffIndex += sizeof(int16_t);
            
            if((ucBIPWInfoData[(99 + (4/8))] & (0x01 << (4 % 8))) != 0)
            {
                memcpy(&sTemperature, &ucBIPWInfoData[uiBuffIndex], sizeof(int16_t));
                pstAppend->stBIState[ucBISlotID].stBIDiagInfo.sSEP1Temperature = sTemperature / 256;
            }
            uiBuffIndex += sizeof(int16_t);
            
            if((ucBIPWInfoData[(99 + (5/8))] & (0x01 << (5 % 8))) != 0)
            {
                memcpy(&sTemperature, &ucBIPWInfoData[uiBuffIndex], sizeof(int16_t));
                pstAppend->stBIState[ucBISlotID].stBIDiagInfo.sSEP2Temperature = sTemperature / 256;
            }
            uiBuffIndex += sizeof(int16_t);
            
            if((ucBIPWInfoData[(99 + (6/8))] & (0x01 << (6 % 8))) != 0)
            {
                memcpy(&sTemperature, &ucBIPWInfoData[uiBuffIndex], sizeof(int16_t));
                pstAppend->stBIState[ucBISlotID].stBIDiagInfo.sSEP3Temperature = sTemperature / 256;
            }
            uiBuffIndex += sizeof(int16_t);

            memset(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA01_DT1, 0xff, sizeof(SharedPWDiagInfo_t));
            if((ucBIPWInfoData[(99 + (16/8))] & (0x01 << (16 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA01_DT1, &ucBIPWInfoData[uiBuffIndex], 4*sizeof(uint8_t));
            }
            uiBuffIndex += 4*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (17/8))] & (0x01 << (17 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (18/8))] & (0x01 << (18 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA03_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (19/8))] & (0x01 << (19 % 8))) != 0)
            {
                memcpy(&ucTempValue, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
                cPWTemprature = (uint8_t)ucTempValue;
                cPWTemprature = cPWTemprature - 50;
                pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.cPW1_AB01_DT1 = cPWTemprature;
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (20/8))] & (0x01 << (20 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AC01_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (21/8))] & (0x01 << (21 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA06_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (22/8))] & (0x01 << (22 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA07_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (23/8))] & (0x01 << (23 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AA08_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (24/8))] & (0x01 << (24 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AD01_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (25/8))] & (0x01 << (25 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AD02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (26/8))] & (0x01 << (26 % 8))) != 0)
            {
                memcpy(&ucTempValue, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
                cPWTemprature = (uint8_t)ucTempValue;
                cPWTemprature = cPWTemprature - 50;
                pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.cPW1_AB02_DT1 = cPWTemprature;
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (27/8))] & (0x01 << (27 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AC02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (28/8))] & (0x01 << (28 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AE01_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (29/8))] & (0x01 << (29 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AE02_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (30/8))] & (0x01 << (30 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AF01_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (31/8))] & (0x01 << (31 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AF02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (32/8))] & (0x01 << (32 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AF03_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (33/8))] & (0x01 << (33 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW1_AF04_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);

            if((ucBIPWInfoData[(99 + (48/8))] & (0x01 << (48 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA01_DT1, &ucBIPWInfoData[uiBuffIndex], 4*sizeof(uint8_t));
            }
            uiBuffIndex += 4*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (49/8))] & (0x01 << (49 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (50/8))] & (0x01 << (50 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA03_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (51/8))] & (0x01 << (51 % 8))) != 0)
            {
                memcpy(&ucTempValue, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
                cPWTemprature = (uint8_t)ucTempValue;
                cPWTemprature = cPWTemprature - 50;
                pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.cPW2_AB01_DT1 = cPWTemprature;
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (52/8))] & (0x01 << (52 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AC01_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (53/8))] & (0x01 << (53 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA06_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (54/8))] & (0x01 << (54 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA07_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (55/8))] & (0x01 << (55 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AA08_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (56/8))] & (0x01 << (56 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AD01_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (57/8))] & (0x01 << (57 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AD02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (58/8))] & (0x01 << (58 % 8))) != 0)
            {
                memcpy(&ucTempValue, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
                cPWTemprature = (uint8_t)ucTempValue;
                cPWTemprature = cPWTemprature - 50;
                pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.cPW2_AB02_DT1 = cPWTemprature;
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (59/8))] & (0x01 << (59 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AC02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (60/8))] & (0x01 << (60 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AE01_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (61/8))] & (0x01 << (61 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AE02_DT1, &ucBIPWInfoData[uiBuffIndex], 2*sizeof(uint8_t));
            }
            uiBuffIndex += 2*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (62/8))] & (0x01 << (62 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AF01_DT1, &ucBIPWInfoData[uiBuffIndex], sizeof(uint8_t));
            }
            uiBuffIndex += sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (63/8))] & (0x01 << (63 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AF02_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);
            
            if((ucBIPWInfoData[(99 + (64/8))] & (0x01 << (64 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AF03_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);

            if((ucBIPWInfoData[(99 + (65/8))] & (0x01 << (65 % 8))) != 0)
            {
                memcpy(&pstAppend->stPWState[ucPWSlotID].stPWDiagInfo.ucPW2_AF04_DT1, &ucBIPWInfoData[uiBuffIndex], 3*sizeof(uint8_t));
            }
            uiBuffIndex += 3*sizeof(uint8_t);

            pstAppend->stPWState[ucPWSlotID].ucActive = 1;
        }
    }
    return;
}



static void CommDiagInit(void)
{
    memset(&s_stSlaveFWInfo, 0x00, sizeof(SlaveFWInfo_t));
    s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;

    return;
}

static void CommDiagCycle(void)
{
    CMController_t emCMIndex = CM1;                     /* CM 索引*/
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint8_t ucACKData[4*1024];
    MSGHead_t* pstMSGHead;                              /* DIAG消息头部*/
    uint16_t usDiagDataLen = 0;
    uint32_t uiReadCrc = 0U;                            /* DIAG数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* DIAG数据计算的CRC值  */
    uint16_t usACKDataLen = 0;
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上DIAG服务信息         */
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    SysSyncSrvQueue_t* pstSysSyncSrvQueue = NULL;       /* 服务同步步列                 */
    SyncTime64_t stSyncCurTime;                         /* 时间结构体               */
    ACKHead_t* pstACKHead = NULL;
    uint32_t uiCalcCRC = 0;
    
    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */
    
    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间             */

    for(emCMIndex = 0;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
//        memset(&ucACKData[0], 0x00,4*1024);
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,CLIENT_SRV);
        
        if(pstSysSrvInfo != NULL)
        {
            if(SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag)
            {
                switch(pstSysSrvInfo->stSysSrvData.usSrvNo)
                {/*根据服务类型，进入对应服务处理函数继续处理文件操作*/
                   case FW_UPDATE_START:
                   {
                        if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                                (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                        {
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                             
                            usACKDataLen = CommFWUpdateStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                        }
                   }
                   break;

                   case FW_UPDATE_CONTINUE:
                   {
                        if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                                (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                        {
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                             
                            usACKDataLen = CommFWUpdateContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0],ucACKData);
                        }
                   }
                   break;

                   case FW_UPDATE_DONE:
                   {
                        if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                                (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                        {
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                             
                            usACKDataLen = CommFWUpdateDone(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                        }
                   }
                   break;
                   
                   case DIAG_INFO_CLEAR:
                   {
                       if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                               (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                       {
                           pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                           usACKDataLen = CommDiagInfoClear(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
                       }
                   }
                   break;
                   
                   default:
                   {
                       pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                       
                       usACKDataLen = 0U;
                       
                       printf("ERR Srv %d\n",pstSysSrvInfo->stSysSrvData.usSrvNo);
                   }

                   break;
                }
            }
            else
            {
                memset((puint8_t)pstSysSrvInfo, 0x00, 100);

                /*从系统资源上获取上位机请求数据*/
                SysGetMsg(emCMIndex,CLIENT_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usDiagDataLen);
        
                if(usDiagDataLen > 0)
                {
//                    printf("Get Diag Srv! %d\n",usDiagDataLen);
                    
                    pstMSGHead = (MSGHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
                    pstSysSrvInfo->stSysSrvData.uiSrvLen = usDiagDataLen;
                    pstSysSrvInfo->stSysSrvData.usSrvNo = pstMSGHead->usActualType;
        
                    /*计算数据包的CRC*/
                    uiCalCrc = SysCrc32Cal(0x00, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], pstMSGHead->usLength);
                    
                    usBuffIndex = pstMSGHead->usLength;
                    /*获取下发的数据CRC*/
                    memcpy((puint8_t)&uiReadCrc, (puint8_t)(&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex]), \
                            CHKSUM_CRC_SIZE);
                          
                    if(uiCalCrc != uiReadCrc)
                    {/*CRC is not consistent*/
                        /*log*/
                        printf("DIAG CRC ERR !\n");
                    }
                    else if(usDiagDataLen != (pstMSGHead->usLength + CHKSUM_CRC_SIZE))
                    {/*data len is not consistent*/
                        /*log*/
                        printf("DiagLen ERR !\n");
                    }
                    else
                    {
                        switch(pstMSGHead->usActualType)
                        {
                            case LOG_READ_RECORD:
                            {
                                /*process read SOE record service*/
                                printf("Log Read !\n");
                                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                
                                usACKDataLen = CommLOGRead(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], &ucACKData[0]);
                            }
                            break;
                            
                            case READ_IOCFG_START:
                            {
                                /*process DIAG read file start*/
                                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                
                                 usACKDataLen = CommReadFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                         pstMSGHead->usLength,&ucACKData[0]);
                            }
                            break;

                            case READ_IOCFG_CONTINUE:
                            {
                                /*process DIAG read file continue*/
                                
                                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                
                                 usACKDataLen = CommReadFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                         pstMSGHead->usLength,&ucACKData[0]);
                            }
                            break;
                            
                            case DIAG_INFO_CLEAR:
                            case FW_UPDATE_START:
                            case FW_UPDATE_CONTINUE:
                            case FW_UPDATE_DONE:
                            {
                                if(KS_PROGRAM != SharedGetSwitchPos())
                                {
                                    /*produce ACK data*/
                                    pstACKHead = (ACKHead_t*)&ucACKData[0];
                                    pstACKHead->ucType = pstMSGHead->ucProtect;
                                    pstACKHead->ucProtect = pstMSGHead->ucType;
                                    pstACKHead->usActualType = pstMSGHead->usActualType;
                                    pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
                                    pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
                                    pstACKHead->ucDataFlg = 0U;
                                    pstACKHead->ucFlag = ACK_FRAME_SINGLE;

                                    pstACKHead->ucResCode = RESCODE_FAILED;
                                    
                                    pstACKHead->usLength = (uint16_t)(sizeof(ACKHead_t));
                                    usACKDataLen = pstACKHead->usLength;
                                    pstACKHead->ucSubReason = ERR_NOT_PROGRAM;
                                    /*计算并填充数据应答的CRC*/
                                    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                                    memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                                    usACKDataLen += sizeof(uiCalcCRC);
                                }
                                else
                                {
                                    SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unActiveTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_ACTIVE_TIME);
                                    SysCalRelativeTime(&pstSysSrvInfo->stSysSrvInfoHead.unTimeOutTime, 
                                                      &stSyncCurTime, SYNC_RELATIVE_TIMEOUT_TIME);
                                    
                                    stSyncTask.emTaskId = SYS_BROADCAST_DIAGSRV;
                                    stSyncTask.emPMId = INVALID_PM_ID;
                                    stSyncTask.emCMId = emCMIndex;
                                    stSyncTask.emTaskStatus = TASK_SEND;
                                    stSyncTask.emTaskWait = TASK_NOWAIT;
                                    pstSysSrvInfo->stSysSrvInfoHead.emSyncRet = SYNC_ERR;

                                    /*暂定为：非稳态时也可以处理服务。*/
                                    SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                                                           
                                    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                                }
                            }
                            break;
        
                            default:
                            {
                                printf("ActualType ERR %d\n",pstMSGHead->usActualType);
                            }
                            break;
                        }
                    }
                }
            }

            if(usACKDataLen > 0)
            {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
                printf("DIAG RSP\n");
                SysSetMsg(emCMIndex,CLIENT_RESP,ucACKData,usACKDataLen);
                
                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                
                usACKDataLen = 0U;
            }
        }
        else
        {
            printf("SysSrvInfo NULL %d\n",emCMIndex);
        }
    }
    return;
}


/*
********************************函数描述*******************************************************************************
* 函数名称: CommUpdateStart
*
* 功能描述: 固件升级开始服务处理函数
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
static uint16_t CommFWUpdateStart(uint8_t ucDiagData[],uint8_t ucACKData[])
{
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FWUpdateSrvInfo_t stFWUpdateSrvInfo;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    
    printf("FWUpdateStart\n");
    
    if(NULL == ucDiagData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        /*写文件开始服务处理开始阶段，将文件数据存入内存上*/
        /*初始化*/
        memset((puint8_t)&stFWUpdateSrvInfo,0x00,sizeof(FWUpdateSrvInfo_t));
        ucSubSeason = ERR_SUCCESS;
        
        /*获取消息头部信息*/
        pstMSGHead = (MSGHead_t*)ucDiagData;
        usBuffIndex += sizeof(MSGHead_t);
        /*获取文件信息*/
        memcpy(&stFWUpdateSrvInfo, &ucDiagData[usBuffIndex], sizeof(FWUpdateSrvInfo_t));
        
        
        usBuffIndex += sizeof(FWUpdateSrvInfo_t);
        
        if(stFWUpdateSrvInfo.ucModuleType > IO_FW)
        {
            printf("ModuleType err %x\n",stFWUpdateSrvInfo.ucModuleType);
            ucSubSeason = ERR_FW_WRITE_START;
        }
        else if(stFWUpdateSrvInfo.ucPrgType >= FW_INVALID_TYPE)
        {
            printf("PrgType err %x\n",stFWUpdateSrvInfo.ucPrgType);
            ucSubSeason = ERR_FW_WRITE_START;
        }
        else if(stFWUpdateSrvInfo.ucIOSlotID > MAX_MOD_NUM)
        {
            printf("IOSlotID err %x\n",stFWUpdateSrvInfo.ucIOSlotID);
            ucSubSeason = ERR_FW_WRITE_START;
        }
        else if(stFWUpdateSrvInfo.usPrgDataLen >= MAX_FW_FILE_NAME_LEN)
        {
            printf("PrgLen err %x\n",stFWUpdateSrvInfo.usPrgDataLen);
            ucSubSeason = ERR_FW_WRITE_START;
        }
        else if(stFWUpdateSrvInfo.usSubPackNum != 0x00)
        {
            printf("SubPackNum err %x\n",stFWUpdateSrvInfo.usSubPackNum);
            ucSubSeason = ERR_FW_WRITE_START;
        }
//        else if(NOT_HAS_FW_UPDATE != s_uiFWUpdateFlg)
//        {
//            printf("HAS_FW_UPDATE err\n");
//            ucSubSeason = ERR_FW_WRITE_START;
//        }
        else
        {
            s_ucFWUpdateRest = 0;
            pstAppend->ucFWUpdateFlg = 0;
            if(0x01 == stFWUpdateSrvInfo.ucModuleType)
            {/*PM相关固件*/
                
                s_stSlaveFWInfo.uiFileWriteIndex = 0;
                s_stSlaveFWInfo.uiFileReadIndex = 0;
                s_stSlaveFWInfo.ucSlotID = stFWUpdateSrvInfo.ucIOSlotID;
                s_stSlaveFWInfo.ucPrgType = stFWUpdateSrvInfo.ucPrgType;
                s_stSlaveFWInfo.usSubPackNum = stFWUpdateSrvInfo.usSubPackNum;
                
                if(FW_PMFPGA == stFWUpdateSrvInfo.ucPrgType)
                {/*FPGA*/
                    s_uiFWUpdateFlg = NEED_ERASE_FPGA;
                    
                    memcpy(&s_stSlaveFWInfo.uiFPGALen, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                    usBuffIndex += sizeof(uint32_t);
                    printf("FPGA Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                            s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiFPGALen);

                }
                else if(FW_PMCORE0RTS == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    s_uiFWUpdateFlg = NEED_ERASE_RTS;
                    
                    s_stSlaveFWInfo.uiFileWriteIndex = FW_FM_CORE0_START_OFF;
                    memcpy(&s_stSlaveFWInfo.uiRTSCore0Len, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                    usBuffIndex += sizeof(uint32_t);
                    printf("CORE0RTS Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                            s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiRTSCore0Len);
                }
                else if(FW_PMCORE1RTS == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/

                    s_stSlaveFWInfo.uiFileWriteIndex = FW_FM_CORE1_START_OFF;
                    memcpy(&s_stSlaveFWInfo.uiRTSCore1Len, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                    usBuffIndex += sizeof(uint32_t);
                    printf("CORE1RTS Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                            s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiRTSCore1Len);
                }
                else if(FW_PMCORE0RTSCRC == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    
                    s_stSlaveFWInfo.uiFileWriteIndex = FW_FM_CORE0_CRC_START_OFF;
                    memcpy(&s_stSlaveFWInfo.uiRTSCore0CRCLen, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                    usBuffIndex += sizeof(uint32_t);
                    printf("CORE0RTSCRC Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                            s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiRTSCore0CRCLen);
                }
                else if(FW_PMCORE1RTSCRC == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/

                    s_stSlaveFWInfo.uiFileWriteIndex = FW_FM_CORE1_CRC_START_OFF;
                    memcpy(&s_stSlaveFWInfo.uiRTSCore1CRCLen, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                    usBuffIndex += sizeof(uint32_t);
                    printf("CORE1RTSCRC Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                            s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiRTSCore1CRCLen);
                }
                else
                {
                    ucSubSeason = ERR_FW_WRITE_START;
                }
            }
            else if(0x02 == stFWUpdateSrvInfo.ucModuleType)
            {/*IO相关固件*/
                
                memset(&s_stSlaveFWInfo, 0x00, sizeof(SlaveFWInfo_t));

                memcpy(&s_stSlaveFWInfo.uiIOFWLen, &ucDiagData[usBuffIndex], sizeof(uint32_t));
                
                usBuffIndex += sizeof(uint32_t);
                
                memcpy(&s_stSlaveFWInfo.ucFileName[0], (pchar_t)&ucDiagData[usBuffIndex], stFWUpdateSrvInfo.usPrgDataLen);
                
                printf("Prg %d WR %d File %d\n",stFWUpdateSrvInfo.usPrgDataLen,
                        s_stSlaveFWInfo.uiFileWriteIndex, s_stSlaveFWInfo.uiIOFWLen);

                s_stSlaveFWInfo.usSubPackNum = stFWUpdateSrvInfo.usSubPackNum;
                s_stSlaveFWInfo.uiFileNameLen = stFWUpdateSrvInfo.usPrgDataLen;
                s_stSlaveFWInfo.ucSlotID = stFWUpdateSrvInfo.ucIOSlotID;
                s_stSlaveFWInfo.ucPrgType = stFWUpdateSrvInfo.ucPrgType;
                
                if(FW_IOAPP == stFWUpdateSrvInfo.ucPrgType)
                {/*APP*/
                    s_uiFWUpdateFlg = NEED_ERASE_IO_FW;
                }
                else if(FW_IOFPGA == stFWUpdateSrvInfo.ucPrgType)
                {/*FPGA*/
                    s_uiFWUpdateFlg = NEED_ERASE_IO_FW;
                }
                else if((FW_IOL2BOOT1 == stFWUpdateSrvInfo.ucPrgType) || (FW_IOL2BOOT2 == stFWUpdateSrvInfo.ucPrgType))
                {/*BOOT*/
                    s_uiFWUpdateFlg = NEED_ERASE_IO_FW;
                }
                else
                {
                    ucSubSeason = ERR_FW_WRITE_START;
                    printf("PrgType Err%x\n",stFWUpdateSrvInfo.ucPrgType);
                }
            }
            else
            {
                ucSubSeason = ERR_FW_WRITE_START;
            }
        }
        
        /*服务处理结束，产生应答*/
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucDiagData;
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
            
            s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }

        
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        pstACKHead->ucSubReason = ucSubSeason;
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);
    }
    
    return usACKDataLen;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommFWUpdateContinue
*
* 功能描述: 固件升级继续服务处理函数
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
static uint16_t CommFWUpdateContinue(uint8_t ucDiagData[],uint8_t ucACKData[])
{
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FWUpdateSrvInfo_t stFWUpdateSrvInfo;
    puint8_t pucAddr = NULL;
    
//    printf("FWUpdateContinue\n");
    
    if(NULL == ucDiagData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        /*写文件开始服务处理开始阶段，将文件数据存入内存上*/
        /*初始化*/
        memset((puint8_t)&stFWUpdateSrvInfo,0x00,sizeof(FWUpdateSrvInfo_t));
        ucSubSeason = ERR_SUCCESS;
        
        /*获取消息头部信息*/
        pstMSGHead = (MSGHead_t*)ucDiagData;
        usBuffIndex += sizeof(MSGHead_t);
        /*获取文件信息*/
        memcpy(&stFWUpdateSrvInfo, &ucDiagData[usBuffIndex], sizeof(FWUpdateSrvInfo_t));

        usBuffIndex += sizeof(FWUpdateSrvInfo_t);

        if(stFWUpdateSrvInfo.ucModuleType > IO_FW)
        {
            printf("ModuleType err %x\n",stFWUpdateSrvInfo.ucModuleType);
            ucSubSeason = ERR_FW_WRITE_CONTINUE;
        }
        else if(stFWUpdateSrvInfo.ucPrgType >= FW_INVALID_TYPE)
        {
            printf("PrgType err %x\n",stFWUpdateSrvInfo.ucPrgType);
            ucSubSeason = ERR_FW_WRITE_CONTINUE;
        }
        else if(stFWUpdateSrvInfo.ucIOSlotID != s_stSlaveFWInfo.ucSlotID)
        {
            printf("IOSlotID err %x\n",stFWUpdateSrvInfo.ucIOSlotID);
            ucSubSeason = ERR_FW_WRITE_CONTINUE;
        }
        else if((stFWUpdateSrvInfo.usPrgDataLen + s_stSlaveFWInfo.uiFileWriteIndex) >= MAX_FW_LEN)
        {
            printf("PrgLen err %x\n",stFWUpdateSrvInfo.usPrgDataLen);
            ucSubSeason = ERR_FW_WRITE_CONTINUE;
        }
        else if(stFWUpdateSrvInfo.usSubPackNum != (s_stSlaveFWInfo.usSubPackNum + 1))
        {
            printf("SubPackNum err %x %x\n",stFWUpdateSrvInfo.usSubPackNum,s_stSlaveFWInfo.usSubPackNum);

            ucSubSeason = ERR_FW_WRITE_CONTINUE;
        }
        else
        {
//            memset(&s_stSlaveFWInfo, 0x00, sizeof(FWUpdateSrvInfo_t));
            
            pucAddr = SysUpdateGetBuff(s_stSlaveFWInfo.uiFileWriteIndex);
            
            memcpy(pucAddr, &ucDiagData[usBuffIndex], stFWUpdateSrvInfo.usPrgDataLen);

            s_stSlaveFWInfo.uiFileWriteIndex += stFWUpdateSrvInfo.usPrgDataLen;
            s_stSlaveFWInfo.usSubPackNum = stFWUpdateSrvInfo.usSubPackNum;
            
            printf("Sub %d Index %d\n", s_stSlaveFWInfo.usSubPackNum, s_stSlaveFWInfo.uiFileWriteIndex);

        }
        
        /*服务处理结束，产生应答*/
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucDiagData;
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
            
            s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }

        
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        pstACKHead->ucSubReason = ucSubSeason;
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);

    }
    
    return usACKDataLen;
}
/*
********************************函数描述*******************************************************************************
* 函数名称: CommFWUpdateDone
*
* 功能描述: 固件升级完成服务处理函数
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
static uint16_t CommFWUpdateDone(uint8_t ucDiagData[],uint8_t ucACKData[])
{
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    uint8_t ucSubSeason = ERR_SUCCESS;                                      /*服务处理结果错误码*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    FWUpdateSrvInfo_t stFWUpdateSrvInfo;
    uint32_t uiFileDataLen = 0;

    
    printf("FWUpdateDone\n");
    
    if(NULL == ucDiagData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        /*写文件开始服务处理开始阶段，将文件数据存入内存上*/
        /*初始化*/
        memset((puint8_t)&stFWUpdateSrvInfo,0x00,sizeof(FWUpdateSrvInfo_t));
        ucSubSeason = ERR_SUCCESS;
        
        /*获取消息头部信息*/
        pstMSGHead = (MSGHead_t*)ucDiagData;
        usBuffIndex += sizeof(MSGHead_t);
        /*获取文件信息*/
        memcpy(&stFWUpdateSrvInfo, &ucDiagData[usBuffIndex], (sizeof(FWUpdateSrvInfo_t) - 2*sizeof(uint16_t)));
        
        
        usBuffIndex += sizeof(FWUpdateSrvInfo_t);
        
        if(stFWUpdateSrvInfo.ucModuleType > IO_FW)
        {
            printf("ModuleType err %x\n",stFWUpdateSrvInfo.ucModuleType);
            ucSubSeason = ERR_FW_WRITE_END;
        }
        else if(stFWUpdateSrvInfo.ucPrgType >= FW_INVALID_TYPE)
        {
            printf("PrgType err %x\n",stFWUpdateSrvInfo.ucPrgType);
            ucSubSeason = ERR_FW_WRITE_END;
        }
        else if(stFWUpdateSrvInfo.ucIOSlotID !=  s_stSlaveFWInfo.ucSlotID)
        {
            printf("IOSlotID err %x\n",stFWUpdateSrvInfo.ucIOSlotID);
            ucSubSeason = ERR_FW_WRITE_END;
        }
        else
        {
            
            if(0x01 == stFWUpdateSrvInfo.ucModuleType)
            {/*PM相关固件*/
                if(FW_PMFPGA == stFWUpdateSrvInfo.ucPrgType)
                {/*FPGA*/
                    s_uiFWUpdateFlg = HAS_FPGA_UPDATE;
                    SysSetPMUpdateFlg(HAS_FPGA_UPDATE);
                    uiFileDataLen = s_stSlaveFWInfo.uiFPGALen;
                    printf("HAS_FPGA_UPDATE\n");
                    
                    
                    ss_ucTest = 0xaa;
                    
                    
                }
                else if(FW_PMCORE1RTSCRC == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    s_uiFWUpdateFlg = HAS_RTS_UPDATE;
                    SysSetPMUpdateFlg(HAS_RTS_UPDATE);
                    uiFileDataLen = s_stSlaveFWInfo.uiRTSCore1CRCLen;
                    s_stSlaveFWInfo.uiFileWriteIndex = s_stSlaveFWInfo.uiFileWriteIndex - FW_FM_CORE1_CRC_START_OFF;
                    printf("HAS_RTS_UPDATE\n");
                }
                else if(FW_PMCORE0RTS == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    uiFileDataLen = s_stSlaveFWInfo.uiRTSCore0Len;
                    s_stSlaveFWInfo.uiFileWriteIndex = s_stSlaveFWInfo.uiFileWriteIndex - FW_FM_CORE0_START_OFF;
                }
                else if(FW_PMCORE0RTSCRC == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    uiFileDataLen = s_stSlaveFWInfo.uiRTSCore0CRCLen;
                    s_stSlaveFWInfo.uiFileWriteIndex = s_stSlaveFWInfo.uiFileWriteIndex - FW_FM_CORE0_CRC_START_OFF;
                }
                else if(FW_PMCORE1RTS == stFWUpdateSrvInfo.ucPrgType)
                {/*RTS*/
                    uiFileDataLen = s_stSlaveFWInfo.uiRTSCore1Len;
                    s_stSlaveFWInfo.uiFileWriteIndex = s_stSlaveFWInfo.uiFileWriteIndex - FW_FM_CORE1_START_OFF;
                }
                else
                {
                    printf("PrgType Err%x\n",stFWUpdateSrvInfo.ucPrgType);
                    ucSubSeason = ERR_FW_WRITE_END;
                }
            }
            else if(0x02 == stFWUpdateSrvInfo.ucModuleType)
            {/*IO相关固件*/
                
                uiFileDataLen = s_stSlaveFWInfo.uiIOFWLen;
                
                if(FW_IOAPP == stFWUpdateSrvInfo.ucPrgType)
                {/*APP*/
                    s_uiFWUpdateFlg = HAS_IO_FW_UPDATE;
                }
                else if(FW_IOFPGA == stFWUpdateSrvInfo.ucPrgType)
                {/*FPGA*/
                    s_uiFWUpdateFlg = HAS_IO_FW_UPDATE;
                }
                else if((FW_IOL2BOOT1 == stFWUpdateSrvInfo.ucPrgType) || (FW_IOL2BOOT2 == stFWUpdateSrvInfo.ucPrgType))
                {/*BOOT*/
                    s_uiFWUpdateFlg = HAS_IO_FW_UPDATE;
                }
                else
                {
                    ucSubSeason = ERR_FW_WRITE_END;
                    printf("PrgType Err%x\n",stFWUpdateSrvInfo.ucPrgType);
                }
            }
            else
            {
                ucSubSeason = ERR_FW_WRITE_END;
            }
            
            if(uiFileDataLen != s_stSlaveFWInfo.uiFileWriteIndex)
            {
                printf("FileDataLen err %d %d\n",uiFileDataLen, s_stSlaveFWInfo.uiFileWriteIndex);
                ucSubSeason = ERR_FW_WRITE_END;
            }
            else
            {
                printf("FileLen %d SlotID %d\n",uiFileDataLen,s_stSlaveFWInfo.ucSlotID);
            }
        }
        
        /*服务处理结束，产生应答*/
        /*produce ACK data*/
        pstACKHead = (ACKHead_t*)ucACKData;
        pstMSGHead = (MSGHead_t*)ucDiagData;
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
            
            s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;
            
            SysSetPMUpdateFlg(NOT_HAS_FW_UPDATE);
        }
        else
        {/*服务处理成功*/
            pstACKHead->ucResCode = RESCODE_SUCCESS;
        }

        
        pstACKHead->usLength = (uint16_t)(usBuffIndex);
        pstACKHead->ucSubReason = ucSubSeason;
        /*计算并填充数据应答的CRC*/
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        usBuffIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(usBuffIndex);

    }
    
    return usACKDataLen;
}

static uint16_t CommLOGRead(uint8_t ucLOGData[],uint8_t ucACKData[])
{
    uint8_t ucSubSeason = ERR_SUCCESS;                  /*服务处理结果错误码*/
    uint16_t usACKDataLen = 0;
    PMController_t emLocalID = PMA;
    uint16_t usLOGEntryNum = 0;
    uint16_t usIndex = 0;
    uint32_t uiCalcCRC = 0;
    ACKHead_t* pstACKHead;                              /* 应答消息头部*/
    MSGHead_t* pstMSGHead;                              /* LOG消息头部*/
    LOGReadReq_t* pstLOGReadReq;                        /* 读LOG请求结构*/
    LOGReadRsp_t* pstLOGReadRsp;                        /* 读LOG应答结构*/
    LogInfo_t stLogInfo;                                /* LOG记录结构*/
    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
    int32_t iReadRet = 0;
    uint16_t usReadIndex = 0;
    uint32_t uiNextIndex = 0U;
    SysTime_t stSysTime;

    if(NULL == ucLOGData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        usBuffIndex += sizeof(MSGHead_t);
        pstLOGReadReq = (LOGReadReq_t*)(&ucLOGData[usBuffIndex]);
        printf("LOG RD: %d %d %d\n",pstLOGReadReq->ucPMID,pstLOGReadReq->ucLOGBlockID,pstLOGReadReq->usGetIndex);
        emLocalID = SysGetLocalPMID();
        if(pstLOGReadReq->ucPMID != emLocalID)
        {
            ucSubSeason = ERR_LOG_READ;
            printf("PMID ERR %d\n",pstLOGReadReq->ucPMID);
            
        }
        else
        {
            usLOGEntryNum = MAX_LOG_READ_NUM;
            
            /*清空，供应答缓存使用*/
            usBuffIndex = 0U;
            usBuffIndex += sizeof(ACKHead_t);
            pstLOGReadRsp = (LOGReadRsp_t*)(&ucACKData[usBuffIndex]);
            usBuffIndex +=  sizeof(LOGReadRsp_t);
            
            usReadIndex = pstLOGReadReq->usGetIndex;
            stSysTime.uiSec = pstLOGReadReq->uiSec;
            stSysTime.uiMicSec = pstLOGReadReq->uiMicSec;
            
            printf("ReadIndex %x\n",pstLOGReadReq->usGetIndex);
            
            for(usIndex = 0; usIndex < usLOGEntryNum; usIndex++)
            {
                iReadRet = LogRead(&stLogInfo, usReadIndex, pstLOGReadReq->ucLOGBlockID, &uiNextIndex,&stSysTime);
                
                if((iReadRet >= 0) || (THE_LAST_LOG_IN_QUEUE == iReadRet))
                {
                    printf("ReadLOG:%d ",usReadIndex);
                    printf("%d ",stLogInfo.uiSec);
                    printf("%d ",stLogInfo.uiMicroSec);
                    printf("%x ",stLogInfo.uiLogID);
                    printf("\n");
                    
                    memcpy((puint8_t)&ucACKData[usBuffIndex],&stLogInfo,sizeof(LogInfo_t));
                    usBuffIndex += sizeof(LogInfo_t);
                    stSysTime.uiSec = stLogInfo.uiSec;
                    stSysTime.uiMicSec = stLogInfo.uiMicroSec;
                    
                    if(THE_LAST_LOG_IN_QUEUE == iReadRet)
                    {
                        usLOGEntryNum = usIndex + 1;
                        printf("LAST LOG %d %d\n",usLOGEntryNum,uiNextIndex);
                        break;
                    }
                    usReadIndex = iReadRet;
                }
                else if(LOG_QUEUE_IS_EMPTY_NOW == iReadRet)
                {
                    usLOGEntryNum = 0;
                    uiNextIndex = 0xffff;
                    stSysTime.uiSec = 0;
                    stSysTime.uiMicSec = 0;
                    usBuffIndex = sizeof(ACKHead_t) + sizeof(LOGReadRsp_t);
                    printf("LOG_QUEUE_EMPTY\n");
                    break;
                }
                else if(READ_LOG_QUEUE_NO_NEW_RECORD == iReadRet)
                {
                    usLOGEntryNum = usIndex;
                    uiNextIndex = usReadIndex;
                    usBuffIndex = sizeof(ACKHead_t) + sizeof(LOGReadRsp_t);
                    printf("LOG_NO_NEW_RECORD\n");
                    break;
                }
                else
                {
                    ucSubSeason = ERR_LOG_READ;
                    printf("ERR_LOG_READ %d\n",iReadRet);
                    break;
                }
            }
            
            /*produce ACK data*/
            pstACKHead = (ACKHead_t*)ucACKData;
            pstMSGHead = (MSGHead_t*)ucLOGData;
            pstACKHead->ucType = pstMSGHead->ucProtect;
            pstACKHead->ucProtect = pstMSGHead->ucType;
            pstACKHead->usActualType = pstMSGHead->usActualType;
            pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
            pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
            pstACKHead->ucDataFlg = 0U;
            pstACKHead->ucFlag = ACK_FRAME_SINGLE;

            if(ucSubSeason != ERR_SUCCESS)
            {/*服务处理失败*/
                pstACKHead->ucResCode = RESCODE_FAILED; 
                /* 错误应答只应答16字节头*/
                usBuffIndex = sizeof(ACKHead_t);
            }
            else
            {/*服务处理成功*/
                pstACKHead->ucResCode = RESCODE_SUCCESS;
                /*填充读LOG应答的结构*/
                pstLOGReadRsp->ucPMID = pstLOGReadReq->ucPMID;
                pstLOGReadRsp->ucLOGBlockID = pstLOGReadReq->ucLOGBlockID;
                pstLOGReadRsp->usEntryNum = usLOGEntryNum;
                pstLOGReadRsp->usNextIndex = (uint16_t)uiNextIndex;
                pstLOGReadRsp->uiSec = stSysTime.uiSec;
                pstLOGReadRsp->uiMicSec = stSysTime.uiMicSec;
                
                printf("NextIndex: %d\n",pstLOGReadRsp->usNextIndex);
            }
            pstACKHead->ucSubReason = ucSubSeason;
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            /*计算并填充应答CRC*/
            uiCalcCRC = SysCrc32Cal(0, ucACKData, pstACKHead->usLength);
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&uiCalcCRC, sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
        }
    }
    
    usACKDataLen = (uint16_t)(usBuffIndex);

    return usACKDataLen;
}


/*
********************************函数描述*******************************************************************************
* 函数名称: CommDiagInfoClear
*
* 功能描述: IO模块诊断信息清除处理函数
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
static uint16_t CommDiagInfoClear(uint8_t ucDiagData[],uint8_t ucACKData[])
{
    static ClearDiagStag_t s_emClearDiagStag = CD_WR;
    static uint8_t s_ucWaitACKTimeout = 0;
    static uint8_t ucSubSeason = ERR_SUCCESS;                               /*服务处理结果错误码*/
    uint16_t usBuffIndex = 0U;                                              /*数组地址累加索引*/
    ACKHead_t* pstACKHead;                                                  /*应答数据头部指针*/
    MSGHead_t* pstMSGHead;                                                  /*服务数据消息数据头部指针*/
    uint16_t usACKDataLen = 0;                                              /*应答数据长度*/
    uint32_t uiCalcCRC = 0;
    DiagInfoClearSrv_t stDiagInfoClearSrv;
    IPBUSAPPMailHeader_t stMailHeader;
    IPBUSMailData_t stIPBUSMail;
    uint8_t ucMailRet = 0U;

    printf("DiagInfoClear\n");
    
    if(NULL == ucDiagData)
    {/*服务数据指针错误*/
        ucSubSeason = ERR_SRC_ADDR;
    }
    else if(NULL == ucACKData)
    {/*应答数据指针错误*/
        ucSubSeason = ERR_ACK_ADDR;
    }
    else
    {
        switch(s_emClearDiagStag)
        {
            case CD_WR:
            {
                /*初始化*/
                memset((puint8_t)&stDiagInfoClearSrv,0x00,sizeof(DiagInfoClearSrv_t));
                
                /*获取消息头部信息*/
                pstMSGHead = (MSGHead_t*)ucDiagData;
                usBuffIndex += sizeof(MSGHead_t);
                /*获取文件信息*/
                memcpy(&stDiagInfoClearSrv, &ucDiagData[usBuffIndex], sizeof(stDiagInfoClearSrv));
                
                
                usBuffIndex += sizeof(stDiagInfoClearSrv);
                
                if(stDiagInfoClearSrv.usModID > MAX_MOD_NUM)
                {
                    printf("ModID err %x\n",stDiagInfoClearSrv.usModID);
                    ucSubSeason = ERR_DIAG_CLEAR_MODID;
                    s_emClearDiagStag = CD_END;
                }
                else if((stDiagInfoClearSrv.ucClearType != 0x03) && (stDiagInfoClearSrv.ucClearType != 0x07))
                {
                    ucSubSeason = ERR_DIAG_CLEAR;
                    s_emClearDiagStag = CD_END;
                }
                else
                {
                    memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                    memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t)); 

                    if(0x03 == stDiagInfoClearSrv.ucClearType)
                    {
                        stIPBUSMail.ucIndex = 0x02;
                    }
                    
                    if(0x07 == stDiagInfoClearSrv.ucClearType)
                    {
                        stIPBUSMail.ucIndex = 0x03;
                    }
                    
                    
                    stMailHeader.usMailRX = 0x0001;
                    stMailHeader.usMailTX = 0x0002;
                    stMailHeader.usMailNR = s_usMsgSEQ;
                    s_usMsgSEQ++;
                    stMailHeader.usMailSlaveAddr = stDiagInfoClearSrv.usModID;
                    stMailHeader.usMailFunc = 0x0003;
                    stMailHeader.usMailLN = stIPBUSMail.usDataLen + sizeof(uint16_t) + sizeof(uint8_t);
                    stMailHeader.usMailACK = 0x0001;
                    stMailHeader.usMailFault = 0x0000;
                    
                    ucMailRet = IPBUSMailSend((puint8_t)&stIPBUSMail, &stMailHeader);
                    
                    if(IPBUS_RET_OK == ucMailRet)
                    {
                        s_emClearDiagStag = CD_RECV_WR_ACK;
                        printf("CD_WR OK %d\n",stDiagInfoClearSrv.usModID);
                    }
                    else
                    {
                        s_emClearDiagStag = CD_END;
                        ucSubSeason = ERR_DIAG_CLEAR;
                        printf("CD_WR Err\n");
                    }
                }
            }
            break;
            
            case CD_RECV_WR_ACK:
            {
                memset(&stMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                memset(&stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                ucMailRet = IPBUSMailRecv((puint8_t)&stIPBUSMail, &stMailHeader);
                if((stMailHeader.usMailLN > 0U) && 
                   ((0x02 == stIPBUSMail.ucIndex) || (0x03 == stIPBUSMail.ucIndex)))
                {
                    if((stMailHeader.usMailFunc & 0x0080) != 0x00)
                    {
                        s_emClearDiagStag = CD_END;
                        ucSubSeason = ERR_DIAG_CLEAR;
                        printf("CD_WR_ACK err %x\n",stMailHeader.usMailFunc);
                    }
                    else
                    {
                        printf("CD_WR_ACK %d\n",stIPBUSMail.usDataLen);
                        s_emClearDiagStag = CD_END;
                        ucSubSeason = ERR_SUCCESS;
                    }
                    s_ucWaitACKTimeout = 0U;
                }
                else
                {
                    s_ucWaitACKTimeout++;
                    if(s_ucWaitACKTimeout > 200)
                    {
                        s_ucWaitACKTimeout = 0U;
                        s_emClearDiagStag = CD_END;
                        ucSubSeason = ERR_DIAG_CLEAR;
                        printf("CD_WR_ACK TO\n");
                    }
                }
            }
            break;
            
            case CD_END:
            {
                /*服务处理结束，产生应答*/
                /*produce ACK data*/
                pstACKHead = (ACKHead_t*)ucACKData;
                pstMSGHead = (MSGHead_t*)ucDiagData;
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
                }

                
                pstACKHead->usLength = (uint16_t)(usBuffIndex);
                pstACKHead->ucSubReason = ucSubSeason;
                /*计算并填充数据应答的CRC*/
                uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                memcpy(ucACKData + usBuffIndex,(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                usBuffIndex += sizeof(uiCalcCRC);
                usACKDataLen = (uint16_t)(usBuffIndex);
                
                ucSubSeason = ERR_SUCCESS;
                s_emClearDiagStag = RSV_RD;
            }
            break;
            default:
            break;
        }
    }
    
    return usACKDataLen;
}


static void CommInterCMDInit(void)
{
    return;
}

static void CommInterCMDCycle(void) /*待补充*/
{
    uint16_t usInterCMDDataLen = 0U;                    /* 接收到的内部命令数据长度*/
//    PMController_t emLocalID = PMA;
    CMController_t emCMIndex = CM1;                     /* CM 索引*/
    uint32_t uiReadCrc = 0U;                            /* InterCMD数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* InterCMD数据计算的CRC值  */
    InterCMDHead_t* pstInterCMDHead;
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上服务信息         */
    uint16_t usBuffIndex = 0U;                          /* 数组地址累加索引*/
    SysCfgFileTable_t* pstSysCfgFileTable = NULL;       /* 上传CM的配置文件表*/

    for(emCMIndex = CM1; emCMIndex < NUM_OF_CM; emCMIndex++)
    {

        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,INTER_CMD_SRV);
        usBuffIndex = 0U;
        /*从系统资源上获取上位机请求数据*/
        SysGetMsg(emCMIndex,INTER_CMD_REQ, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usInterCMDDataLen);

        if((usInterCMDDataLen > 0) && (pstSysSrvInfo != NULL))
        {
            
            printf("InterCMD ! len is: %d\n",usInterCMDDataLen);
            
            
            pstInterCMDHead = (InterCMDHead_t*)&(pstSysSrvInfo->stSysSrvData.ucSrvData[0]);

            uiCalCrc = SysCrc32Cal(0x00, &pstSysSrvInfo->stSysSrvData.ucSrvData[0],
                                        sizeof(InterCMDHead_t));
            usBuffIndex += sizeof(InterCMDHead_t);
            /*获取下发的数据CRC*/
            memcpy((puint8_t)&uiReadCrc,(puint8_t)(&pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex]),CHKSUM_CRC_SIZE);

            if(uiCalCrc != uiReadCrc)
            {/*CRC is not consistent*/
                /*log*/
//                printf("  uiCalCrc != uiReadCrc !\n");
                
            }
            else if(usInterCMDDataLen != (sizeof(InterCMDHead_t) + CHKSUM_CRC_SIZE))
            {
                
            }
            else
            {
                switch(pstInterCMDHead->emInterCMD)
                {
                    case CMD_REQ_ALL_CMCONFIG:
                    {
//                        printf("  CMD_REQ_ALL_CMCONFIG \n");
                        
                        /*获取各CM需要配置文件的总表*/
                        pstSysCfgFileTable = SysGetCfgFileAllTable();
                        if(NULL == pstSysCfgFileTable)
                        {
                            /*log*/
                            
                            printf("pCfgTabl NULL\n");
                            
                        }
                        else
                        {
//                            printf("  TRANS_ALL_CFG_FILE_START \n");
                            
                            /*将文件上传传表更新到系统资源*/
                            SysSetCfgFileTable(pstSysCfgFileTable);

                            printf("TRANS_ALL_FILE :%d\n",emCMIndex);
                            SysSetTransCfgFileFlag(TRANS_ALL_CFG_FILE_START, emCMIndex);
                            
                            printf("Flag:%d\n",pstSysCfgFileTable->bFlag);
                            printf("Ready:%d\n",pstSysCfgFileTable->bReady[emCMIndex]);
                            printf("CM:%d\n",pstSysCfgFileTable->bCfgFileCM[emCMIndex]);
                        }
                    }
                    break;

                    case CMD_REQ_INCRE_CMCONFIG:
                    {
                        
                        printf("CMD_REQ_INCRE_CMCONFIG \n");
                        
                        /*获取各CM需要配置文件的最近一次增量下装的增量表*/
                        pstSysCfgFileTable = SysGetCfgFileIncreTable();
                        if(NULL == pstSysCfgFileTable)
                        {
                            /*log*/
                            
                            printf("pCfgTabl NULL\n");
                            
                        }
                        else
                        {
                            printf("TRANS_INC_FILE :%d\n",emCMIndex);
                            
                            /*将文件上传传表更新到系统资源*/
                            SysSetCfgFileIncreasedTable(pstSysCfgFileTable);
                            
                            SysSetTransCfgFileFlag(TRANS_INC_CFG_FILE_START, emCMIndex);
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

static uint16_t CommDownloadStart (uint8_t ucCS1131Data[], uint8_t ucACKData[])
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
    
    PMController_t emLocalPMId = SysGetLocalPMID();             /* 本地PM ID号           */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);      /* 前一系PM ID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);    /* 后一系PM ID号           */
    PrjInfo_t *pstDLPrjInfo = NULL;                             /* 本机工程信息数据指针     */
    PrjInfo_t *pstPrjInfo = NULL;                               /* 本机工程信息数据指针     */
    SysSyncPrjQueue_t* pstSyncPrjQueue = SysGetSyncPrjQueue();  /* 同步任务队列指针     */
    puint8_t pucDataAreaAddr = NULL;                            /* 共享数据区地址*/
    
//    DEBUG_COMM("begin DownloadStart\n");
    
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
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emNextPMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emPrePMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstPrjInfo = SysGetPrjInfo(emLocalPMId);
            memset(pstPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstPrjInfo = SysGetPrjInfo(emPrePMId);
            memset(pstPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstPrjInfo = SysGetPrjInfo(emNextPMId);
            memset(pstPrjInfo, 0x00, sizeof(PrjInfo_t));
            
            SysSetIsConfigured(NOT_HAS_BE_CONFIGURED);
            
            SysSetWorkStatus(emLocalPMId,WS_STANDBY);
            if(SysGetWorkStatus(emPrePMId) != WS_OFFLINE)
            {
                SysSetWorkStatus(emPrePMId,WS_STANDBY);
            }
            
            if(SysGetWorkStatus(emNextPMId) != WS_OFFLINE)
            {
                SysSetWorkStatus(emNextPMId,WS_STANDBY);
            }
            
            /* 清除同步任务 */
            SysDeSyncPrjQueue(pstSyncPrjQueue);
            
            SetLEDState(LED_NAME_SYNC, LED_OFF);
            
            SysSetTaskStatus(emLocalPMId,TASK_UP1, TS_INIT);
            SysSetTaskStatus(emLocalPMId,TASK_UP2, TS_INIT);
            
            SharedConfigAreaInit();
            
            SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);
            SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
            
            SharedSetPauseFlag(PF_CONTINUE, TASK_UP1);
            SharedSetPauseFlag(PF_CONTINUE, TASK_UP2);
            
            SharedSetOnlineDownLoadSwitchFlg(TASK_UP1, OS_NOT_NEED_SWITCH);
            SharedSetOnlineDownLoadSwitchFlg(TASK_UP2, OS_NOT_NEED_SWITCH);
            
            SharedClearIECErrTime(TASK_UP1);
            SharedClearIECErrTime(TASK_UP2);
            
            SetLEDState(LED_NAME_ACTIVE, LED_OFF);
            /* 无工程时IPBUS灯灭 */
            SetLEDState(LED_NAME_IPBUS, LED_OFF);
           
            
            DEBUG_COMM("INIT_DOWNLOAD\n");
        }
        if(0x01 == ucDownloadFlag)
        {
            SharedSetDownloadFlg(ONLINE_DOWNLOAD);
            s_pstSysCfgFileTable = (SysCfgFileTable_t*)SysGetCfgFileIncreTable();
            memset(s_pstSysCfgFileTable, 0x00, sizeof(SysCfgFileTable_t));
            
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emLocalPMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emNextPMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            pstDLPrjInfo = SysGetDownLoadPrjInfo(emPrePMId);
            memset(pstDLPrjInfo, 0x00, sizeof(PrjInfo_t));
            
            SharedSetOnlineDownLoadSwitchFlg(TASK_UP1, OS_NOT_NEED_SWITCH);
            SharedSetOnlineDownLoadSwitchFlg(TASK_UP2, OS_NOT_NEED_SWITCH);
            
            DataProcessModEnableFlgInit();
            
            DEBUG_COMM("ONLINE_DOWNLOAD\n");
        }
        
        /*清除PM状态中，关于下装完成标志*/
        uint16_t usPMState = 0U;
        usPMState = SharedGetPMState();
        usPMState = usPMState & 0xfffc;
        usPMState = usPMState | 0x0003;
        
//        usPMState = 0xffff;
        
        SharedSetPMState(usPMState);
        
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
//        if(ucSubSeason != ERR_SUCCESS)
//        {/*服务处理失败*/
//            pstACKHead->ucResCode = RESCODE_FAILED;
//        }
//        else
//        {/*服务处理成功*/
//            pstACKHead->ucResCode = RESCODE_SUCCESS;
//        }
        pstACKHead->ucResCode = RESCODE_SUCCESS;
        
        pstACKHead->ucSubReason = ucSubSeason;

        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
        
        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
        uiACKDataIndex += sizeof(uiCalcCRC);
        usACKDataLen = (uint16_t)(uiACKDataIndex);
    }
    
//    DEBUG_COMM("usACKDataLen is: %d  \n",usACKDataLen);
    
    return usACKDataLen;
}


static void CommCtrlSrvInit(void)
{
    return;
}


static void CommCtrlSrvCycle(void)
{
    uint8_t ucACKData[1024];
    uint16_t usACKDataLen = 0;
    SysSrvInfo_t* pstSysSrvInfo = NULL;                 /* 系统资源上OPC服务信息         */
    SyncTime64_t stSyncCurTime;                         /* 时间结构体               */
    bool_t bRet = false;
    static uint8_t s_ucTimeout = 0;
    static uint8_t s_ucCrtlSrvACKTimeoutCount[MAX_TASK_NUM] = {0U};
    Task_ID_t emTaskIndex = TASK_UP1;
    
    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间             */
    
    
    for(emTaskIndex = TASK_UP1; emTaskIndex < MAX_TASK_NUM; emTaskIndex++)
    {
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetCtrlSrvInfo(emTaskIndex);
    
        if(SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag)
        {/*当服务处理标志为SRV_NEED_PROCESS时，处理服务数据*/
    
            if(NEED_ACK == pstSysSrvInfo->stSysSrvInfoHead.emAckFlag)
            {/*当服务应答标志为NEED_ACK时，等待来自Core0的应答数据*/
                
                /*从共享内存上获取应答数据*/
                memset(&ucACKData[0], 0x00,1024);

                SysGetSMMsg(SM_CTRLSRV_RESP, (CMController_t)emTaskIndex, &ucACKData[0], &usACKDataLen);
    
                if(usACKDataLen > 0)
                {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                    s_ucCrtlSrvACKTimeoutCount[emTaskIndex] = 0;
                    
                    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                }
                else
                {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
                    s_ucCrtlSrvACKTimeoutCount[emTaskIndex]++;
                    if(CTRL_ACK_TIMEOUT == s_ucCrtlSrvACKTimeoutCount[emTaskIndex])
                    {
                        s_ucCrtlSrvACKTimeoutCount[emTaskIndex] = 0;
                        
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        
                        DEBUG_COMM("CTRL:GetSMMsg TIMEOUT\n ");
                    }
                }
            }
            else
            {
                if((stSyncCurTime.ullTime >= pstSysSrvInfo->stSysSrvInfoHead.unActiveTime.ullTime) && \
                        (SYNC_OK == pstSysSrvInfo->stSysSrvInfoHead.emSyncRet))
                {
//                    
//                    s_ucTimeout++;

                    DEBUG_COMM("CTRL DATA\n");
                    bRet = SysSetSMMsg(SM_CTRLSRV_REQ, (CMController_t)emTaskIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                            pstSysSrvInfo->stSysSrvData.uiSrvLen);
                    s_ucTimeout++;
                    
                    if(true == bRet)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;

//                        DEBUG_COMM("CTRL SetSMMsg OK\n ");
                        
                        s_ucTimeout = 0;
                    }

                    if(s_ucTimeout == 10)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                        
                        s_ucTimeout = 0;
                        
                        DEBUG_COMM("CTRL SetSMMsg TIMEOUT\n ");
                    }
                }
            }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            DEBUG_COMM("CTRL HAS ACK\n");
            
            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            
            usACKDataLen = 0U;
            
        }
    }

    return;
}

/*
********************************函数描述*******************************************************************************
* 函数名称: CommEraseFlash
*
* 功能描述: 擦除NorFLASH指定区域操作
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

FileOptRest_t CommEraseFlash(NorFlashOptType_t emNorFlashOptType,uint32_t uiFWUsedArea)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint32_t uiTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
    static uint32_t uiCMD = 0;
    static uint32_t uiStatusWord = 0;
    

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送判断文件是否存在命令*/
            switch(emNorFlashOptType)
            {
                case ERASE_RTS:
                {
                    if(FW_AREA_0 == uiFWUsedArea)
                    {
                        uiCMD = FM_ERASE_RTS_AREA0;
                        uiStatusWord = FM_ERASE_RTS_AREA0_ACK;
                    }
                    else if(FW_AREA_1 == uiFWUsedArea)
                    {
                        uiCMD = FM_ERASE_RTS_AREA1;
                        uiStatusWord = FM_ERASE_RTS_AREA1_ACK;
                    }
                    else
                    {
                        uiCMD = 0;
                        uiStatusWord = 0;
                    }
                }
                break;
                
                case ERASE_RTS_FLG:
                {
                    uiCMD = FM_ERASE_RTS_FLG;
                    uiStatusWord = FM_ERASE_RTS_FLG_ACK;
                }
                break;
                
                case ERASE_FPGA:
                {
                    if(FW_AREA_0 == uiFWUsedArea)
                    {
                        uiCMD = FM_ERASE_FPGA_AREA0;
                        uiStatusWord = FM_ERASE_FPGA_AREA0_ACK;
                    }
                    else if(FW_AREA_1 == uiFWUsedArea)
                    {
                        uiCMD = FM_ERASE_FPGA_AREA1;
                        uiStatusWord = FM_ERASE_FPGA_AREA1_ACK;
                    }
                    else
                    {
                        uiCMD = 0;
                        uiStatusWord = 0;
                    }
                }
                break;
                
                case ERASE_FPGA_FLG:
                {
                    uiCMD = FM_ERASE_FPGA_FLG;
                    uiStatusWord = FM_ERASE_FPGA_FLG_ACK;
                }
                break;
                
                default:
                {
                    uiCMD = 0;
                    uiStatusWord = 0;
                }
                break;
            }
            
            if(uiCMD > 0)
            {
                uiTimeCount = 0;
                SysEraseFirmware(uiCMD);
                emProcessStag = FOS_WAIT_ACK;
                emFileOptRest = NOT_COMPLETE;
            }
            else
            {
                emFileOptRest = FAIL;
            }
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答，若收到应答，解析应答结果，并返回；若超时，返回超时*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                if(uiStatusWord == stAckHeader.uiStatusWord)
                {/*擦除成功，返回操作成功*/
                    emFileOptRest = SUCCESS;
                    emProcessStag = FOS_START;
                    
                    printf("Erase NorFlash Success %x\n",uiCMD);
                }
                else
                {/*失败，返回操作失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                    
                    printf("Erase NorFlash FAIL %x %x\n",stAckHeader.uiStatusWord,uiCMD);
                }
            }
            else
            {
                uiTimeCount++;
                if(uiTimeCount >= NORFLASH_OPT_TIMEOUT )
                {/*超时处理；返回失败*/
                    emFileOptRest = FAIL;
                    emProcessStag = FOS_START;
                    
                    printf("Erase NorFlash TIMEOUT %x\n",uiCMD);
                }
            }
        }
        break;

        default:
        break;
    }
    return emFileOptRest;
}

static void CommPMUpdate(void)
{
    static FMUpdateStag_t s_emFMUpdateStag = FM_UPDATE_START;
    static uint32_t s_uiFWUsedArea = 0xffffffff;
    static NorFlashOptType_t s_emNorFlashOptType = NOR_OPT_INVALID_TYPE;
    FileOptRest_t emOptRst = NOT_COMPLETE;
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    static FWType_t s_emFWType = FW_INVALID_TYPE;
    int32_t iReadFWFileLen = 0U;
    int32_t iWriteFWFileLen = 0U;
    puint8_t pucSrcAddr = NULL;
    puint8_t pucDestAddr = NULL;
    uint32_t uiFileLen = 0;
    uint32_t uiStartOff = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    switch(s_emFMUpdateStag)
    {
        case FM_UPDATE_START:
        {
            if(NEED_ERASE_RTS == s_uiFWUpdateFlg)
            {
                lld_read(FW_FM_RTS_FLG_START_ADDR, &s_uiFWUsedArea, sizeof(uint32_t));
                s_emFWType = FW_PMCORE0RTS;
                s_emNorFlashOptType = ERASE_RTS;
                
                if((FW_AREA_0 != s_uiFWUsedArea) && (FW_AREA_1 != s_uiFWUsedArea))
                {
                    s_uiFWUsedArea = FW_AREA_0;
                }
            }
            else if(NEED_ERASE_FPGA == s_uiFWUpdateFlg)
            {
                lld_read(FW_FM_FPGA_FLG_START_ADDR, &s_uiFWUsedArea, sizeof(uint32_t));
                s_emFWType = FW_PMFPGA;
                s_emNorFlashOptType = ERASE_FPGA;
                
                if((FW_AREA_0 != s_uiFWUsedArea) && (FW_AREA_1 != s_uiFWUsedArea))
                {
                    s_uiFWUsedArea = FW_AREA_0;
                }
            }
            else
            {
                s_uiFWUsedArea = 0xffffffff;
            }
            
            if((FW_AREA_0 == s_uiFWUsedArea) || (FW_AREA_1 == s_uiFWUsedArea))
            {
                s_emFMUpdateStag = FM_UPDATE_ERASE_FW;
                if(FW_AREA_0 == s_uiFWUsedArea)
                {
                    s_uiFWUsedArea = FW_AREA_1;
                }
                else
                {
                    s_uiFWUsedArea = FW_AREA_0;
                }
            }
            else
            {
            }
        }
        break;
        
        case FM_UPDATE_ERASE_FW:
        {
            emOptRst = CommEraseFlash(s_emNorFlashOptType,s_uiFWUsedArea);

            if(SUCCESS == emOptRst)
            {/*擦除操作成功，进行下一步*/
                s_emFMUpdateStag = FM_UPDATE_WRITE_FW;
                s_stSlaveFWInfo.uiFileReadIndex = 0;
            }
            if(FAIL == emOptRst)
            {/*操作失败，并应答错误信息*/
                s_emFMUpdateStag = FM_UPDATE_END;
                
                s_ucFWUpdateRest = 2;
                pstAppend->ucFWUpdateFlg = 2;
            }
        }
        break;
        
        case FM_UPDATE_WRITE_FW:
        {
            if((HAS_RTS_UPDATE == s_uiFWUpdateFlg) || (HAS_FPGA_UPDATE == s_uiFWUpdateFlg))
            {
                switch(s_emFWType)
                {
                    case FW_PMFPGA:
                    {
                        if(FW_AREA_0 == s_uiFWUsedArea)
                        {
                            pucDestAddr = FW_FM_FPGA_START_ADDR1 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        else
                        {
                            pucDestAddr = FW_FM_FPGA_START_ADDR2 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        uiFileLen = s_stSlaveFWInfo.uiFPGALen;
                        uiStartOff = 0;
                        
                        if(s_stSlaveFWInfo.uiFileReadIndex == 0)
                        {
                            lld_write(pucDestAddr, (puint8_t)&uiFileLen, sizeof(uint32_t));
                            
                            pucDestAddr = pucDestAddr + sizeof(uint32_t);
                        }
                    }
                    break;
                    
                    case FW_PMCORE0RTS:
                    {
                        if(FW_AREA_0 == s_uiFWUsedArea)
                        {
                            pucDestAddr = FW_FM_CORE0_START_ADDR1 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        else
                        {
                            pucDestAddr = FW_FM_CORE0_START_ADDR2 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        uiFileLen = s_stSlaveFWInfo.uiRTSCore0Len;
                        uiStartOff = FW_FM_CORE0_START_OFF;
                    }
                    break;
                    
                    case FW_PMCORE1RTS:
                    {
                        if(FW_AREA_0 == s_uiFWUsedArea)
                        {
                            pucDestAddr = FW_FM_CORE1_START_ADDR1 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        else
                        {
                            pucDestAddr = FW_FM_CORE1_START_ADDR2 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        uiFileLen = s_stSlaveFWInfo.uiRTSCore1Len;
                        uiStartOff = FW_FM_CORE1_START_OFF;
                    }
                    break;
                    
                    case FW_PMCORE0RTSCRC:
                    {
                        if(FW_AREA_0 == s_uiFWUsedArea)
                        {
                            pucDestAddr = FW_FM_CORE0_CRC_START_ADDR1 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        else
                        {
                            pucDestAddr = FW_FM_CORE0_CRC_START_ADDR2 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        uiFileLen = s_stSlaveFWInfo.uiRTSCore0CRCLen;
                        uiStartOff = FW_FM_CORE0_CRC_START_OFF;
                    }
                    break;
                    
                    case FW_PMCORE1RTSCRC:
                    {
                        if(FW_AREA_0 == s_uiFWUsedArea)
                        {
                            pucDestAddr = FW_FM_CORE1_CRC_START_ADDR1 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        else
                        {
                            pucDestAddr = FW_FM_CORE1_CRC_START_ADDR2 + s_stSlaveFWInfo.uiFileReadIndex;
                        }
                        uiFileLen = s_stSlaveFWInfo.uiRTSCore1CRCLen;
                        uiStartOff = FW_FM_CORE1_CRC_START_OFF;
                    }
                    break;
                    
                    default:
                    {
                        pucDestAddr = NULL;
                    }
                    break;
                }
    
                pucSrcAddr = SysUpdateGetBuff(uiStartOff + s_stSlaveFWInfo.uiFileReadIndex);
                
                if((pucSrcAddr != NULL) && (pucDestAddr != NULL))
                {
                    iReadFWFileLen = MIN(MAX_NORFLASH_PACK_LEN, (uiFileLen - s_stSlaveFWInfo.uiFileReadIndex));
                    iWriteFWFileLen = lld_write(pucDestAddr, pucSrcAddr, iReadFWFileLen);

                    if(iWriteFWFileLen == iReadFWFileLen)
                    {
                        s_stSlaveFWInfo.uiFileReadIndex += iWriteFWFileLen;
                        if(s_stSlaveFWInfo.uiFileReadIndex >= uiFileLen)
                        {
                            s_stSlaveFWInfo.uiFileReadIndex = 0;
                            switch(s_emFWType)
                            {
                                case FW_PMCORE0RTS:
                                {
                                    s_emFWType = FW_PMCORE1RTS;
                                }
                                break;
                                
                                case FW_PMCORE1RTS:
                                {
                                    s_emFWType = FW_PMCORE0RTSCRC;
                                }
                                break;
                                
                                case FW_PMCORE0RTSCRC:
                                {
                                    s_emFWType = FW_PMCORE1RTSCRC;
                                }
                                break;
                                
                                case FW_PMCORE1RTSCRC:
                                {
                                    s_emFMUpdateStag = FM_UPDATE_ERASE_FLG;
                                    s_emNorFlashOptType = ERASE_RTS_FLG;
                                }
                                break;
                                case FW_PMFPGA:
                                {
                                    s_emFMUpdateStag = FM_UPDATE_ERASE_FLG;
                                    s_emNorFlashOptType = ERASE_FPGA_FLG;
                                }
                                break;
                                
                                default:
                                {
                                    s_emFMUpdateStag = FM_UPDATE_END;
                                    
                                    s_ucFWUpdateRest = 2;
                                    pstAppend->ucFWUpdateFlg = 2;
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        s_emFMUpdateStag = FM_UPDATE_END;
                        
                        s_ucFWUpdateRest = 2;
                        pstAppend->ucFWUpdateFlg = 2;
                    }
                }
                else
                {
                    s_emFMUpdateStag = FM_UPDATE_END;
                    printf("Addr err %d %d\n",pucSrcAddr,pucDestAddr);
                    
                    s_ucFWUpdateRest = 2;
                    pstAppend->ucFWUpdateFlg = 2;
                }
            }
        }
        break;
        
        case FM_UPDATE_ERASE_FLG:
        {
            emOptRst = CommEraseFlash(s_emNorFlashOptType,s_uiFWUsedArea);

            if(SUCCESS == emOptRst)
            {/*擦除操作成功，进行下一步*/
                s_emFMUpdateStag = FM_UPDATE_WRITE_FLG;
            }
            if(FAIL == emOptRst)
            {/*操作失败，并应答错误信息*/
                s_emFMUpdateStag = FM_UPDATE_END;
                
                s_ucFWUpdateRest = 2;
                pstAppend->ucFWUpdateFlg = 2;
            }
        }
        break;
        
        case FM_UPDATE_WRITE_FLG:
        {
            if(HAS_RTS_UPDATE == s_uiFWUpdateFlg)
            {
                if((FW_AREA_0 == s_uiFWUsedArea) || (FW_AREA_1 == s_uiFWUsedArea))
                {
                    lld_write(FW_FM_RTS_FLG_START_ADDR, &s_uiFWUsedArea, sizeof(uint32_t));
                }
            }
            else if(HAS_FPGA_UPDATE == s_uiFWUpdateFlg)
            {
                if((FW_AREA_0 == s_uiFWUsedArea) || (FW_AREA_1 == s_uiFWUsedArea))
                {
                    lld_write(FW_FM_FPGA_FLG_START_ADDR, &s_uiFWUsedArea, sizeof(uint32_t));
                }
            }
            else
            {
                
            }
            
            s_ucFWUpdateRest = 1;
            pstAppend->ucFWUpdateFlg = 1;
            
            s_emFMUpdateStag = FM_UPDATE_END;
        }
        break;
        
        case FM_UPDATE_END:
        {
            s_emFMUpdateStag = FM_UPDATE_START;
            s_uiFWUsedArea = 0xffffffff;
            s_emNorFlashOptType = NOR_OPT_INVALID_TYPE;
            s_emFWType = FW_INVALID_TYPE;
            s_uiFWUpdateFlg = NOT_HAS_FW_UPDATE;
            
            SysSetPMUpdateFlg(NOT_HAS_FW_UPDATE);
        }
        break;
        
        default:
        break;
    }

    return;
}


/*test*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
