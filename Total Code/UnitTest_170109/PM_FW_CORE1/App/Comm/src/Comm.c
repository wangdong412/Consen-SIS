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

static P2PSendCtrl_t s_stP2PSendCtrl[MAX_P2P_BLOCK_NUM];

static ProjectInfo_t stProjectInfo;
static ForceList_t stForceList;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void CommCS1131Init(void);

static void CommCS1131Cycle(void);

static uint16_t CommCheckExlibVern (uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommGetProjectInfo (uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadForceList(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadDisableTabl(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

//static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);



static void CommFileInfoInit(void);

static uint16_t CommWriteFileStart(uint8_t ucCS1131Data[], uint32_t uiCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommWriteFileDone(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileStart(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static uint16_t CommReadFileContinue(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[]);

static void CommOPCInit(void);

static void CommOPCCycle(void);

static void CommModbusInit(void);

static void CommModbusCycle(void);

static void CommP2PInit(void);

static void CommP2PCycle(void);

static void CommP2PRecv(void);

static void CommP2PRecvReq( CMController_t emCMIndex );

static void CommP2PRecvResp( CMController_t emCMIndex );

static bool_t GetP2PSendStartFlag( uint8_t ucBlkIndex );

static void CommP2PSend(void);

static uint16_t CommP2PPacking(uint8_t ucP2PSendIndex, ACKTimeoutFlag_t emACKTimeoutFlag);

static void CommSOEInit(void);

static void CommSOECycle(void);

static uint16_t CommSOERead(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);

static uint16_t CommSOEDelete(uint8_t ucSOEData[], uint32_t usSOEDataLen,uint8_t ucACKData[]);

static void CommAMSInit(void);

static void CommAMSCycle(void);

static void CommDiagInit(void);

static void CommDiagCycle(void);

static uint16_t CommLOGRead(uint8_t ucLOGData[],uint8_t ucACKData[]);

static void CommInterCMDInit(void);

static void CommInterCMDCycle(void);

static void CommTimingInit(void);

static void CommTimingCycle(void);

static void CommClientInit(void);

static void CommClientCycle(void);

static uint32_t SysGetExlibVern(void);

static ProjectInfo_t* SysGetProjectInfo(void);

static ForceList_t* SysGetForceList(void);



/*test*/

static uint16_t CommDownloadStart (uint8_t ucCS1131Data[], uint8_t ucACKData[]);



/*test*/

static void CommCtrlSrvInit(void);


static void CommCtrlSrvCycle(void);


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
//    uint8_t ucRet = 0;
    
    ucRTDataBackupCount = 0;
    
    CommCS1131Init();
    
    CommOPCInit();
    
    CommModbusInit();
    
    CommP2PInit();
    
    CommSOEInit();
    
    CommAMSInit();
    
    CommDiagInit();
    
    CommInterCMDInit();
    
    CommTimingInit();
    
    CommClientInit();
    
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
    bool_t bRet = false;
    uint32_t uiOffset = 0U;
    uint32_t uiUsedDataLen = 0U;
    Task_ID_t emTaskID = TASK_UP1;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;

    if((ucRTDataBackupCount % 25) == 0)
    {/*固定周期拷贝一次实时数据，供内部通讯上传CM*/
        
        if(HAS_BE_CONFIGURED == SysGetIsConfigured())
        {
            for(emTaskID = TASK_UP1; emTaskID < MAX_TASK_NUM; emTaskID++)
            {
                pstTaskConfigInfo = SharedGetTaskConfigInfo(emTaskID);
                
                if((pstTaskConfigInfo != NULL) && (ACTIVE == pstTaskConfigInfo->emActiveFlag))
                {
//                    printf("Copy RT Data\n");
                    
                    uiOffset = pstTaskConfigInfo->uiGOffset;
                    uiUsedDataLen = pstTaskConfigInfo->uiGUsedSize;
                    
//                    printf("G: %d %d\n",uiOffset,uiUsedDataLen);
                    
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
                            break;
                        }
                        else
                        {

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
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_I_AREA, uiOffset, uiUsedDataLen);
                        
                        if(false == bRet)
                        {

                            break;
                        }
                        else
                        {

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
                        bRet = SysCopySharedRtDataToCore1RtData(RTDATA_Q_AREA, uiOffset, uiUsedDataLen);
                        
                        if(false == bRet)
                        {

                            
                            break;
                        }
                        else
                        {
                            
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
                bRet = SysCopySharedRtDataToCore1RtData(RTDATA_APPEND_AREA, 0, sizeof(SharedAppend_t));
                SysSetCopyRtDataFinished();
                
                //printf(" @@ SysSetCopyRtDataFinished @@ \n");
                
            }
        }
        else
        {
    #if 0
    /*test*/
                
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP1_G_AREA, 0, 100);
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_UP2_G_AREA, 0, 100);
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_I_AREA, 0, 100);
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_Q_AREA, 0, 100);
            bRet = SysCopySharedRtDataToCore1RtData(RTDATA_APPEND_AREA, 0, 100);
            
            if(bRet)
            {
                SysSetCopyRtDataFinished();
                
                //printf(" ## SysSetCopyRtDataFinished ## \n");
                
            }
            
    /*test*/
    #endif
        }
        
        
        
        ucRTDataBackupCount = 1;
        
        /*P2P通讯处理*/
        CommP2PCycle();
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
        CommSOECycle();
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
        /*CTRL命令通讯处理*/
        CommCtrlSrvCycle();
        ucRTDataBackupCount++;

//        CommSOEReadTest();
    }

//    DEBUG_COMM("COMM Cycle End !!! \n");
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
    
    SyncTime64_t stSyncCurTime;                         /* 时间结构体               */
    bool_t bRet = false;
    static uint8_t s_ucTimeout[NUM_OF_CM] = {0,0,0,0};
    ACKHead_t* pstACKHead = NULL;
    uint32_t uiCalcCRC = 0U;


    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列             */
    
    SysGetSyncTime(&stSyncCurTime);                     /* 获取当前时间             */

    for(emCMIndex = CM1;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        memset(ucACKData, 0x00,1024);

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
                   LogWrite(LOG1_ID_USER_L0_CS1131_WRITEFILE_START);

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
                   LogWrite(LOG1_ID_USER_L0_CS1131_WRITEFILE_END);

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
                        
                       LogWrite(LOG1_ID_USER_L0_CS1131_DOWNLOAD_START);
                        
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
               {
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
                   else
                   {
                       LogWrite(LOG1_ID_USER_L0_CS1131_DISABLE_VAR);
                   }

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
                               
                               printf("SetSMMsg OK\n ");
                               
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

            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

            SysGetMsg(emCMIndex,CS1131_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&CS1131DataLen);
            
            
//            DEBUG_COMM("-----------CS1131DataLen  =  %d  \n",CS1131DataLen);
            if(CS1131DataLen > 0)
            {/*数据长度大于0，表示收到数据*/
//                DEBUG_COMM("SysGetMsg  CS1131DataLen: %d  \n",CS1131DataLen);
                 
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
                            else
                            {
                                LogWrite(LOG1_ID_USER_L0_CS1131_DOWNLOAD_START);
                            }
                            
                            if(CS1131_DOWNLOAD_START == pstSysSrvInfo->stSysSrvData.usSrvNo)
                            {
                                SharedSetDownloadPM(SysGetLocalPMID());
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
                        
/*test */
//                        case CS1131_DOWNLOAD_START:
//                        {
//                            /*process 1131 CS1131_DOWNLOAD_START*/
//                            
//                            printf("RECV CS1131_DOWNLOAD_START  \n");
//                            
//                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
//                            
//                             usACKDataLen = CommDownloadStart (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], ucACKData);
//                        }
//                            
//                        break;
/*test */

                        case CS1131_CHECK_EXTERLIB:
                        {
                            /*process 1131 check exlibvern*/
                            LogWrite(LOG1_ID_USER_L0_CS1131_CHECK_EXTERLIB);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommCheckExlibVern (&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READ_PRJINFO:
                        {
                            /*process 1131 read ProjectInfo*/
                            LogWrite(LOG1_ID_USER_L0_CS1131_READ_PRJINFO);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommGetProjectInfo(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
                        }
                        break;

                        case CS1131_READ_FORCE:
                        {
                            /*process 1131 read Forcelist*/
                            
                            LogWrite(LOG1_ID_USER_L0_CS1131_READ_FORCE);

                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommReadForceList(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);
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
                            
//                            usACKDataLen = CommDownloadComfirm(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
//                                    stMSGHead.usLength,ucACKData);
                        }

                        break;

                        case CS1131_WRITEFILE_START:
                        {
                            /*process 1131 write file start*/
                            
                            printf("RECV WRITEFILE_ST\n");
                            LogWrite(LOG1_ID_USER_L0_CS1131_WRITEFILE_START);
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                             usACKDataLen = CommWriteFileStart(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_CONT:
                        {
                            
                            printf("RECV WRITEFILE_CONT\n");
                            
                            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
                            
                            /*process 1131 write file continue*/
                             usACKDataLen = CommWriteFileContinue(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                                     stMSGHead.usLength,ucACKData);

                             emFileHandleFlg[emCMIndex] = FILE_HANDLE_BUSY;
                        }
                        break;

                        case CS1131_WRITEFILE_END:
                        {
                            
                            printf("RECV WRITEFILE_END\n");
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

                        default:
                            
                            printf("RECV ERR SRV: %d\n",pstSysSrvInfo->stSysSrvData.usSrvNo);
                            
                        break;
                     }
                 }
             }
        }
        if(usACKDataLen > 0)
        {/*当应答数据长度大于零时，将应答数据交内部通讯转发CM*/
            DEBUG_COMM("Has RESP Data %d\n",emCMIndex);
            SysSetMsg(emCMIndex,CS1131_RESP,ucACKData,usACKDataLen);
            if(emFileHandleFlg[emCMIndex] != FILE_HANDLE_FREE)
            {
                emFileHandleFlg[emCMIndex] = FILE_HANDLE_FREE;
            }
            
            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            
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
static uint16_t CommCheckExlibVern (uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
{
    uint32_t uiLocalExlibVern = SysGetExlibVern();              /*获取外部库版本，接口待定*/
    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
    uint32_t uiReadExlibVern = 0U;                               /*1131下发的外部库版本号*/
    ACKHead_t* pstACKHead;                                      /*应答头部*/
    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
    uint16_t usACKDataLen = 0U;
    uint32_t uiCalcCRC = 0U;
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
    {
        if(usCS1131DataLen != sizeof(uiLocalExlibVern))
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
    }
    
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
static uint16_t CommGetProjectInfo (uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
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
    }
    
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
static uint16_t CommReadForceList(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
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
    }
    
    return usACKDataLen;
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
//static uint16_t CommDownloadComfirm(uint8_t ucCS1131Data[], uint16_t usCS1131DataLen,uint8_t ucACKData[])
//{
//    uint8_t ucSubSeason = ERR_SUCCESS;                          /*服务处理过程错误码值*/
//    ACKHead_t* pstACKHead;                                      /*应答头部*/
//    MSGHead_t* pstMSGHead;                                      /*下发消息头部*/
//    uint16_t usACKDataLen = 0;
//    uint32_t uiCalcCRC = 0;
//    uint32_t uiACKDataIndex = 0U;                               /*应答数据数据地址索引*/
//
//    if(NULL == ucCS1131Data)
//    {/*服务数据指针错误*/
//        ucSubSeason = ERR_SRC_ADDR;
//    }
//    else if(NULL == ucACKData)
//    {/*应答数据指针错误*/
//        ucSubSeason = ERR_ACK_ADDR;
//    }
//    else
//    {/*设置本地下装文件被确认*/
//
//        //SysSetPrjComfirm();
//
//        /*produce ACK data*/
//        pstACKHead = (ACKHead_t*)ucACKData;
//        pstMSGHead = (MSGHead_t*)ucCS1131Data;
//        pstACKHead->ucType = pstMSGHead->ucProtect;
//        pstACKHead->ucProtect = pstMSGHead->ucType;
//        pstACKHead->usActualType = pstMSGHead->usActualType;
//        pstACKHead->ucNodeNum = pstMSGHead->ucNodeNum;
//        pstACKHead->usSequenceNum = pstMSGHead->usSequenceNum;
//        pstACKHead->ucDataFlg = 0U;
//        pstACKHead->ucFlag = ACK_FRAME_SINGLE;
//        uiACKDataIndex += sizeof(ACKHead_t);
//        if(ucSubSeason != ERR_SUCCESS)
//        {/*服务处理失败*/
//            pstACKHead->ucResCode = RESCODE_FAILED;
//        }
//        else
//        {/*服务处理成功*/
//            pstACKHead->ucResCode = RESCODE_SUCCESS;
//        }
//        pstACKHead->ucSubReason = ucSubSeason;
//
//        pstACKHead->usLength = (uint16_t)(uiACKDataIndex);
//
//        uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
//        memcpy(&ucACKData[uiACKDataIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
//        uiACKDataIndex += sizeof(uiCalcCRC);
//        usACKDataLen = (uint16_t)(uiACKDataIndex);
//    }
//
//    return usACKDataLen;
//}
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
                printf("StatusWord=0x%x\n",stAckHeader.uiStatusWord);
                printf("ErrCode=0x%x\n",stAckHeader.uiErrCode);

                if(FM_IS_FILE_EXIST_ACK == stAckHeader.uiStatusWord)
                {/*操作成功，且文件存在；返回结果*/
                    emFileOptRest = FILE_EXIST;
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
FileOptRest_t CommOpenFile(uint8_t ucFileName[],pint32_t piFd)
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
                printf("stw=0x%x fd=%d\n", stAckHeader.uiStatusWord, stAckHeader.iFd);
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
FileOptRest_t CommWriteFile(int32_t iFd,uint8_t ucFileData[],uint16_t usFileDataLen)
{
    static FileOperateStag_t emProcessStag = FOS_START;             /*记录文件操作当步骤*/
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
    bool_t bRet = false;

    switch(emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送写文件命令*/
            ucTimeCount = 0;
            bRet = SysWriteFile(iFd, ucFileData, usFileDataLen);
            printf("bRet=%d fd=%d len=%d\n",bRet,  iFd, usFileDataLen);
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
                printf("wf uiStatusWord=0x%x",stAckHeader.uiStatusWord);
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
FileOptRest_t CommCloseFile(int32_t iFd)
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
FileOptRest_t CommDeleteFile(uint8_t ucFileName[])
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
FileOptRest_t CommRenameFile(uint8_t ucOldFileName[], uint8_t ucNewFileName[])
{
    static FileOperateStag_t s_emProcessStag = FOS_START;
    FileOptRest_t emFileOptRest = NOT_COMPLETE;                     /*文件操作的返回结果*/
    static uint8_t ucTimeCount = 0;                                 /*获取应答超时计数*/
    SysFMAckHeader_t stAckHeader;                                   /*文件管理进程返回的应答头部*/
    bool_t bRet = false;

    switch(s_emProcessStag)
    {
        case FOS_START:
        {/*调用接口，发送重命名文件命令*/
            ucTimeCount = 0;
            bRet = SysRenameFile(ucOldFileName, ucNewFileName);
            printf("rename ret=%d\n", bRet);
            emFileOptRest = NOT_COMPLETE;
            s_emProcessStag = FOS_WAIT_ACK;
        }
        break;

        case FOS_WAIT_ACK:
        {/*等待应答*/
            if(true == SysRecvAckHeader(&stAckHeader))
            {/*获取操作应答*/
                printf("rename stw=%d\n", stAckHeader.uiStatusWord);
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
                ucTimeCount++;
                if(ucTimeCount >= FS_OPERATE_TIMEOUT )
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
                    
                    printf("FIleID err: %x\n",stFileWriteInfo.ucFileID);
    //                printf("CommWriteFileStart: ucFileType is : %x \n",stFileWriteInfo.ucFileType);
    //                
    //                printf("CommWriteFileStart: ucType is : %x \n",pstMSGHead->ucType);
    //                printf("CommWriteFileStart: ucProtect is : %x \n",pstMSGHead->ucProtect);
    //                printf("CommWriteFileStart: usActualType is : %x \n",pstMSGHead->usActualType);
    //                printf("CommWriteFileStart: ucNodeNum is : %x \n",pstMSGHead->ucNodeNum);
    //                printf("CommWriteFileStart: ucPMNum is : %x \n",pstMSGHead->ucPMNum);
    //                printf("CommWriteFileStart: usSequenceNum is : %x \n",pstMSGHead->usSequenceNum);
    //                printf("CommWriteFileStart: ucLastFlag is : %x \n",pstMSGHead->ucLastFlag);
    //                printf("CommWriteFileStart: usLength is : %x \n",pstMSGHead->usLength);
                    
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
                printf("1 file exist?=%d\n", ucFileOptRst);
                printf("name =%s\n", &pstFileInfo->ucTmpFileName[0]);
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
                printf("2 erase=%d\n", ucFileOptRst);
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
                printf("3 open tmp file=%d\n", ucFileOptRst);
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
                printf("4 write tmp file=%d\n", ucFileOptRst);
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
                printf("5 close tmp file=%d\n", ucFileOptRst);
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
    //                printf("CommWriteFileContinue: ucFileType is : %x \n",stFileWriteInfo.ucFileType);
    //                
    //                printf("CommWriteFileContinue: ucType is : %x \n",pstMSGHead->ucType);
    //                printf("CommWriteFileContinue: ucProtect is : %x \n",pstMSGHead->ucProtect);
    //                printf("CommWriteFileContinue: usActualType is : %x \n",pstMSGHead->usActualType);
    //                printf("CommWriteFileContinue: ucNodeNum is : %x \n",pstMSGHead->ucNodeNum);
    //                printf("CommWriteFileContinue: ucPMNum is : %x \n",pstMSGHead->ucPMNum);
    //                printf("CommWriteFileContinue: usSequenceNum is : %x \n",pstMSGHead->usSequenceNum);
    //                printf("CommWriteFileContinue: ucLastFlag is : %x \n",pstMSGHead->ucLastFlag);
    //                printf("CommWriteFileContinue: usLength is : %x \n",pstMSGHead->usLength);
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
    //SysCfgFileState_t *pstCfgFileState = NULL;                            /*内存上存储文件的相关信息*/
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
                                
//                                SharedSetSyncDataFlag(TASK_UP1,DATA_SYNC_COMPLETE);
//                                
//                                SharedSetSyncDataFlag(TASK_UP2,DATA_SYNC_COMPLETE);
                                
//                                SharedSetDownloadFlg(DOWN_LOAD_FILE_DONE);
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
                    printf("a wr tmp file=%d fd=%d\n", ucFileOptRst, pstFileInfo->iFid);
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
                printf("b form file exist=%d\n", ucFileOptRst);
                printf("form name=%s\n",&pstFileInfo->ucFileName[0]);
                if(FILE_EXIST == ucFileOptRst)
                {/*文件存在，重命名文件*/
                    emProcessStag = FWRITE_RENAME_FORMFILE;
                    printf("exist\n");
                }
                if(FILE_NOT_EXIST == ucFileOptRst)
                {/*文件不存在，重命名临时文件*/
                    emProcessStag = FWRITE_RENAME_TMPFILE;
                    printf("no exist\n");
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
                printf("c rename form file=%d\n", ucFileOptRst);
                //printf("c  %s %s\n", &pstFileInfo->ucFileName[0], &pstFileInfo->ucOldFileName[0]);
                if(FAIL == ucFileOptRst)
                {/*重命名正式文件为旧文件失败，删除文件*/
                    /*record error log*/
                    emProcessStag = FWRITE_DELETE_FORMFILE;
                }
                if(SUCCESS == ucFileOptRst)
                {/*重命名正式文件为旧文件成功，将临时文件重命名为正式文件*/
                    emProcessStag = FWRITE_RENAME_TMPFILE;
                    printf("%s  %s\n",&pstFileInfo->ucFileName[0], &pstFileInfo->ucOldFileName[0]);
                }
            }
            break;
    
            case FWRITE_RENAME_TMPFILE:
            {
                ucFileOptRst = CommRenameFile(&pstFileInfo->ucTmpFileName[0], &pstFileInfo->ucFileName[0]);
                printf("d rename tmp file=%d\n", ucFileOptRst);
    
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
                    printf("%s  %s\n",&pstFileInfo->ucTmpFileName[0], &pstFileInfo->ucFileName[0]);
                }
            }
            break;
    
            case FWRITE_DELETE_TMPFILE:
            {
                ucFileOptRst = CommDeleteFile(&pstFileInfo->ucTmpFileName[0]);
                printf("e del tmp file=%d\n", ucFileOptRst);
                printf("delete tmp file=%s\n", &pstFileInfo->ucTmpFileName[0]);
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
                printf("f delete form file=%d\n", ucFileOptRst);
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
    ReadFileContFlag_t emContFlag = CONT_TRANS;                             /*读文件继续标志*/
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
                    emContFlag = CONT_TRANS;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    emContFlag = TRANS_DONE;
                }
                /*初始化文件数据读取缓存*/
//                memset(ucFileData, 0x00, MAX_READ_FILE_LEN);
                
                usReadRet = SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        &ucFileData[0], usReadDataLen);
                if(usReadDataLen != usReadRet)
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_START;
                    printf("Read err %d %d\n",usReadRet,usReadDataLen);
                }
            }
            
            if((ucSubSeason != ERR_SUCCESS) || (TRANS_DONE == emContFlag))
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
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&emContFlag,sizeof(ReadFileContFlag_t));
            usBuffIndex += sizeof(ReadFileContFlag_t);
            /*填充文件数据长度*/
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&usReadDataLen,sizeof(uint16_t));
            usBuffIndex += sizeof(uint16_t);
            /*填充文件数据*/
            memcpy(&ucACKData[usBuffIndex],(puint8_t)(&ucFileData[0]),usReadDataLen);
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
    ReadFileContFlag_t emContFlag = CONT_TRANS;                             /*读文件继续标志*/
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
                    emContFlag = CONT_TRANS;
                }
                else
                {
                    usReadDataLen = (uint16_t)(stCfgFileState.uiLen - pstFileInfo->uiFileReadIndex);
                    emContFlag = TRANS_DONE;
                    
                }
                /*初始化文件数据读取缓存*/
//                memset(&ucFileData[0], 0x00, MAX_READ_FILE_LEN);
                
                usReadRet = SysGetCfgFileInfo(emCfgFileType, pstFileInfo->uiFileReadIndex, \
                        ucFileData, usReadDataLen);
                if(usReadDataLen != usReadRet)
                {
                    /*获取文件数据，长度错误*/
                    ucSubSeason = ERR_READ_FILE_CONTINUE;
                    printf("Read err %d %d\n",usReadRet,usReadDataLen);
                }
            }
            
            if((ucSubSeason != ERR_SUCCESS) || (TRANS_DONE == emContFlag))
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
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&emContFlag,sizeof(ReadFileContFlag_t));
            usBuffIndex += sizeof(ReadFileContFlag_t);
            /*填充文件数据长度*/
            memcpy(&ucACKData[usBuffIndex], (puint8_t)&usReadDataLen,sizeof(uint16_t));
            usBuffIndex += sizeof(uint16_t);
            /*填充文件数据*/
            memcpy(&ucACKData[usBuffIndex],(puint8_t)(&ucFileData[0]),usReadDataLen);
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
//                    uint8_t x = 0U;
                    printf("OPC SRV DATA %d ： ",pstSysSrvInfo->stSysSrvData.uiSrvLen);
//                    for(x = 0U; x < pstSysSrvInfo->stSysSrvData.uiSrvLen; x++)
//                    {
//                        printf(" %x ",pstSysSrvInfo->stSysSrvData.ucSrvData[x]);
//                    }
                    printf(" \n ");
                    
                    s_ucTimeout[emCMIndex]++;
                    bRet = SysSetSMMsg(SM_OPC_REQ, emCMIndex, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], \
                            pstSysSrvInfo->stSysSrvData.uiSrvLen);
                    if(true == bRet)
                    {
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NEED_ACK;
                        
                        printf("OPC SetSMMsg OK\n ");

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
            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

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

//                             if(SYS_STABLE_STATE == SysGetState())
//                             {/* 非稳态时按错误处理，不添加同步任务 */
//                                 SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
//                             }

                             /*暂定为：非稳态时也可以处理服务。*/
                             SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);
                             
                             pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
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
            DEBUG_COMM("OPC HAS ACK\n");
            
            SysSetMsg(emCMIndex,OPC_RESP,ucACKData,usACKDataLen);
            
            pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
            
            usACKDataLen = 0U;
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
                    /* 读取一条Modbus消息 */
                    memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));
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

static void CommP2PInit(void)
{
    memset(&s_stP2PSendCtrl[0], 0x00, sizeof(P2PSendCtrl_t)*MAX_P2P_BLOCK_NUM);
    
    /* P2PSafety Module Init */
    P2PCommInit();
    
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

static void CommP2PCycle(void)
{
    CommP2PRecv();
    CommP2PSend();

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PRecv
*
* 功能描述: P2P接收函数
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

static void CommP2PRecv(void)
{
    CMController_t emCMIndex = CM1;               /* CM 索引*/
    srv_srvdata_type_t emSyncType = P2P_REQ_SRV;

    for( emCMIndex = CM1; emCMIndex < NUM_OF_CM; emCMIndex++ )
    {
        for( emSyncType = P2P_REQ_SRV; emSyncType <= P2P_RSP_SRV; emSyncType++ )
        {
            /* P2P Message Type */
            if( P2P_REQ_SRV == emSyncType )
            {
                CommP2PRecvReq( emCMIndex );
            }
            else
            {
                CommP2PRecvResp( emCMIndex );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CommP2PRecvReq
*
* 功能描述: P2P接收请求函数
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

static void CommP2PRecvReq( CMController_t emCMIndex )
{
    static uint8_t s_ucTimeout[NUM_OF_CM] = {0,0,0,0};

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
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo( emCMIndex, P2P_REQ_SRV );

        if(pstSysSrvInfo == NULL)
        {
            
        }
        else if( SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag )
        {/*当服务处理标志为busy时，等待来自core0的应答数据*/
            if( NEED_ACK == pstSysSrvInfo->stSysSrvInfoHead.emAckFlag )
            {
                if( P2P_REQ_FRAME == pstSysSrvInfo->stSysSrvData.usSrvNo )
                {
                    /*从共享内存上获取应答数据*/
                    memset( &ucACKData[0], 0, sizeof(ucACKData));
                    usACKDataLen = 0U;
                    SysGetSMMsg( SM_P2P_RESP, emCMIndex, &ucACKData[0], &usACKDataLen );

                    if( usACKDataLen > 0U )
                    {/*获取到应答数据，将服务处理标志清除，超时计数清零*/
                        ucP2PACKTimeoutCount[emCMIndex] = 0U;
                        pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                        pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
                    }
                    else
                    {/*当没有获得来自core0的应答，进行超时计数，当等待应答超时，清除标志，不再等待应答*/
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
        else
        {/*ProcessFlag is SRV_NOT_NEED_PROCESS*/
            memset( pstSysSrvInfo, 0, sizeof(SysSrvInfo_t));
            SysGetP2PMsg( emCMIndex, P2P_EXT_REQ, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usP2PDataLen );
            if( usP2PDataLen > 0U )
            {/*数据长度大于0，表示收到数据*/
                /*获取消息头部*/
                pstReqHead = (P2PRecvReqHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];

                /*记录服务数据相关信息*/
                pstSysSrvInfo->stSysSrvData.uiSrvLen = (uint32_t)usP2PDataLen;
                pstSysSrvInfo->stSysSrvData.usSrvNo = (uint16_t)pstReqHead->ucFrameType;

                /*计算数据CRC*/
                usBuffIndex = pstReqHead->usTotalLen - (uint16_t)(CHKSUM_CRC_SIZE);
                uiCalCrc = SysCrc32Cal( 0, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], (uint32_t)usBuffIndex );

                /*读取下发的CRC*/
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

static void CommP2PRecvResp( CMController_t emCMIndex )
{
    RespDecodeResultInfo_t stResDecodeResultInfo;
    bool_t bRedundant = false;                          /*发送端网络是否为冗余网络的标志*/
    uint16_t usBuffIndex = 0U;                          /* 数组内地址索引*/
    uint32_t uiReadCrc = 0U;                            /* P2P数据中的CRC值    */
    uint32_t uiCalCrc = 0U;                             /* P2P数据计算的CRC值  */
    SysSyncTask_t stSyncTask;                           /* 同步任务结构体           */
    uint16_t usP2PDataLen = 0;
    uint8_t ucBlkIndex = 0U;
    uint32_t uiP2PSendBlkNum = 0U;
    SyncTime64_t stSyncCurTime;                         /* 时间结构体 */
    P2PRecvRespHead_t *pstRespHead = NULL;
    SysSyncSrvQueue_t *pstSysSyncSrvQueue = NULL;       /* 服务同步步列 */
    SysSrvInfo_t *pstSysSrvInfo = NULL;                 /* 系统资源上P2P服务信息   */
    P2PBlock_t *pstP2PBlock = NULL;

    SysGetSyncTime( &stSyncCurTime );                   /* 获取当前时间 */
    pstSysSyncSrvQueue = SysGetSyncSrvQueue();          /* 获取同步队列 */

    if((emCMIndex >= CM1) && (emCMIndex < NUM_OF_CM))
    {
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo( emCMIndex, P2P_RSP_SRV );

        if(pstSysSrvInfo == NULL)
        {
            
        }
        else if( SRV_NEED_PROCESS == pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag )
        {/*由Core1来处理*/
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

                pstRespHead = (P2PRecvRespHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];
                uiP2PSendBlkNum = SharedGetP2PSendBlockNum();

                /* Find P2P Send Block: Loop */
                for( ucBlkIndex = 0U; ucBlkIndex < (uint8_t)uiP2PSendBlkNum; ucBlkIndex++ )
                {
                    pstP2PBlock = SharedGetP2PSendBlockInfo( ucBlkIndex );
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

                if( pstP2PBlock != NULL )
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
                            if( s_stP2PSendCtrl[ucBlkIndex].usWaitACKCount > P2P_WAITACK_CYCLE )
                            {/* 等待ACK超时 */
                                //printf( "PERspTO\n" );

                                /* 调用安全层接口，错误处理 */
                                P2PHandleRespTO( s_stP2PSendCtrl[ucBlkIndex].ucDestStaID );

                                /*清空相应标志，计数*/
                                s_stP2PSendCtrl[ucBlkIndex].emACKTimeoutFlag = ACK_TIMEOUT;
                            }
                            else
                            {
                                s_stP2PSendCtrl[ucBlkIndex].emACKTimeoutFlag = ACK_NOT_TIMEOUT;
                            }

                            s_stP2PSendCtrl[ucBlkIndex].usWaitACKCount = 0U;
                            s_stP2PSendCtrl[ucBlkIndex].emWaitACKFlag = NOT_WAIT_ACK;
                        }
                    }
                    else
                    {
                        /* Print */
                        //printf( "PDERsp\n" );
                    }
                }

                pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NOT_NEED_PROCESS;
                pstSysSrvInfo->stSysSrvInfoHead.emAckFlag = NO_ACK;
            }
        }
        else
        {/*ProcessFlag is SRV_NOT_NEED_PROCESS*/
            memset( pstSysSrvInfo, 0, sizeof(SysSrvInfo_t));
            SysGetP2PMsg( emCMIndex, P2P_EXT_RESP, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], &usP2PDataLen );
            if( usP2PDataLen > 0U )
            {/*数据长度大于0，表示收到数据*/
                /*获取消息头部*/
                pstRespHead = (P2PRecvRespHead_t*)&pstSysSrvInfo->stSysSrvData.ucSrvData[0];

                /*记录服务数据相关信息*/
                pstSysSrvInfo->stSysSrvData.uiSrvLen = usP2PDataLen;
                pstSysSrvInfo->stSysSrvData.usSrvNo = pstRespHead->ucFrameType;

                /*计算数据CRC*/
                usBuffIndex = pstRespHead->usTotalLen - (uint16_t)(CHKSUM_CRC_SIZE);
                uiCalCrc = SysCrc32Cal( 0, &pstSysSrvInfo->stSysSrvData.ucSrvData[0], (uint32_t)usBuffIndex );

                /*读取下发的CRC*/
                memcpy( &uiReadCrc, &pstSysSrvInfo->stSysSrvData.ucSrvData[usBuffIndex], (uint32_t)(CHKSUM_CRC_SIZE));

                if( uiCalCrc != uiReadCrc )
                {/*CRC 不一致*/
                    //printf( "PERspCRC\n" );
                }
                else if( usP2PDataLen != pstRespHead->usTotalLen )
                {/*数据长度不一致*/
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

                    /*暂定为：非稳态时也可以处理服务。*/
                    SysEnSyncSrvQueue(pstSysSyncSrvQueue, &stSyncTask);

                    pstSysSrvInfo->stSysSrvInfoHead.ucProcessFlag = SRV_NEED_PROCESS;
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

    if( ucBlkIndex < (uint8_t)(MAX_P2P_BLOCK_NUM ))
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
            if( ucValue == 0x01)
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

static void CommP2PSend(void)
{
    uint8_t ucBlockIndex = 0U;        /*P2P发送块的块号索引*/
    uint32_t uiBlockSendCycle = 0U;   /*P2P发送块配置的发送周期*/
    uint8_t ucSendIndex = 0U;         /*当前要发送块的索引*/
    uint16_t usP2PSendDataLen = 0U;   /*发送数据长度*/
    uint32_t uiP2PSendBlockNum = 0U;
    P2PBlock_t* pstP2PBlock = NULL;   /*P2P配置块结构指针*/
    uint32_t uiSyncTime = 0U;

    uiP2PSendBlockNum = SharedGetP2PSendBlockNum();
    uiSyncTime = SysGetSyncTimeLow32();

    /*send control process*/
    for( ucBlockIndex = 0U; ucBlockIndex < (uint8_t)uiP2PSendBlockNum; ucBlockIndex++ )
    {
        if( ACTIVE == SharedGetP2PBlockSendActive( ucBlockIndex ))
        {
            pstP2PBlock = SharedGetP2PSendBlockInfo( ucBlockIndex );

            /* P2P Start */
            if( GetP2PSendStartFlag( (uint8_t)(pstP2PBlock->stP2PBlockHead.uiBlockID)))
            {
                /* 更新发送间隔周期计数 */
                if( s_stP2PSendCtrl[ucSendIndex].bHasSend )
                {
                    s_stP2PSendCtrl[ucSendIndex].uiIntrCycCnt++;
                }

                /*轮询所有模块，发送周期计数*/
                uiBlockSendCycle = SharedGetP2PBlockSendCycle( ucBlockIndex );
                //printf( "T: %d, %d, %d, %d\n", uiSyncTime, TASK_PERIOD, uiBlockSendCycle, uiSyncTime % (10*uiBlockSendCycle));

                if(uiBlockSendCycle > 0)
                {
                    if( uiSyncTime % (10*uiBlockSendCycle) <= (uint32_t)(TASK_PERIOD))
                    {
                        s_stP2PSendCtrl[ucBlockIndex].emWaitSendFlag = WAIT_SEND;
                        s_stP2PSendCtrl[ucBlockIndex].ucDestStaID = pstP2PBlock->stP2PBlockHead.uiTargetStationID;
                        /*printf( "SendBlk: %d Wait Send. DestStaID:%d.\n", ucBlockIndex, \
                                s_stP2PSendCtrl[ucBlockIndex].ucDestStaID );*/
                    }
                }

                if( WAIT_ACK == s_stP2PSendCtrl[ucBlockIndex].emWaitACKFlag )
                {/*在等待来自对端控制站的ACK*/
                    s_stP2PSendCtrl[ucBlockIndex].usWaitACKCount++;

                    if( s_stP2PSendCtrl[ucBlockIndex].usWaitACKCount > P2P_WAITACK_CYCLE )
                    {/*等待ACK超时*/
                        //printf( "PERspTO\n" );
                        s_stP2PSendCtrl[ucBlockIndex].usWaitACKCount = 0U;

                        /*调用安全层接口，错误处理*/
                        P2PHandleRespTO( s_stP2PSendCtrl[ucBlockIndex].ucDestStaID );

                        /*清空相应标志，计数*/
                        s_stP2PSendCtrl[ucBlockIndex].emWaitACKFlag = NOT_WAIT_ACK;
                        s_stP2PSendCtrl[ucBlockIndex].emACKTimeoutFlag = ACK_TIMEOUT;
                    }
                }
            }
            else
            {
                //printf( "P2PSBlk%d Stop\n", ucBlockIndex );

                /* Clear Info. */
                s_stP2PSendCtrl[ucSendIndex].bHasSend = false;
                s_stP2PSendCtrl[ucSendIndex].uiIntrCycCnt = 0U;
                s_stP2PSendCtrl[ucSendIndex].emWaitSendFlag = NOT_WAIT_SEND;
            }
        }
    }

    /* P2P block send process, each cycle process one block */
    /* Max Cycle Deviation: +- 5*100us */
    ucSendIndex = ((uiSyncTime+5) / (uint32_t)(TASK_PERIOD)) % uiP2PSendBlockNum;
    if( WAIT_SEND == s_stP2PSendCtrl[ucSendIndex].emWaitSendFlag )
    {
        /* packing P2P data */
        usP2PSendDataLen = CommP2PPacking( ucSendIndex, s_stP2PSendCtrl[ucSendIndex].emACKTimeoutFlag );
        if( 0U == usP2PSendDataLen )
        {
            //printf( "CPPF\n" );
        }
        else
        {
            s_stP2PSendCtrl[ucSendIndex].emWaitACKFlag = WAIT_ACK;

            /* Handle SendP2PReq Timeout */
            if( s_stP2PSendCtrl[ucSendIndex].bHasSend )
            {
                /* Check if send timeout */
                uiBlockSendCycle = SharedGetP2PBlockSendCycle( ucSendIndex );
                if( s_stP2PSendCtrl[ucSendIndex].uiIntrCycCnt > ((10*uiBlockSendCycle)/(uint32_t)(TASK_PERIOD)))
                {
                    /* Send Timeout */
                    //printf( "PSTO\n" );
                    P2PHandleSendReqTO( s_stP2PSendCtrl[ucBlockIndex].ucDestStaID, true );
                }
                else
                {
                    P2PHandleSendReqTO( s_stP2PSendCtrl[ucBlockIndex].ucDestStaID, false );
                }
            }
            else
            {
                s_stP2PSendCtrl[ucSendIndex].bHasSend = true;
            }

            s_stP2PSendCtrl[ucSendIndex].uiIntrCycCnt = 0U;
        }

        s_stP2PSendCtrl[ucSendIndex].emACKTimeoutFlag = ACK_NOT_TIMEOUT;
        s_stP2PSendCtrl[ucSendIndex].emWaitSendFlag = NOT_WAIT_SEND;
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
    P2PBlock_t* pstP2PBlock = NULL;                 /*P2P配置块结构指针*/
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
    uint8_t ucP2PSendbuff[P2P_SEND_BUFF_SIZE];          /*发送数据缓存*/
    CMSlotIdTabl_t* pstCMSlotIdTabl = NULL;             /*CM槽位表可读片区的指针，供解析文件时进行比对*/
    CMController_t emCMIndex = INVALID_CM_ID;
    
    /*入参有效性判断*/
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
            
            /*安全层打包*/
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

                uiBuffIndex += usMakeReqDataLen;

                pstP2PSendHead->uiTotalLen = uiBuffIndex + sizeof(uiCalcCRC);

                if((true == bMakeReqRet) && (pstP2PSendHead->uiTotalLen <= P2P_SEND_BUFF_SIZE))
                {
                    /*填充头部信息*/
                    if((pstP2PBlock->stP2PBlockHead.ucCMID1 != 0U) && (pstP2PBlock->stP2PBlockHead.ucCMID2 != 0U) && \
                       (pstP2PBlock->stP2PBlockHead.ucCMID2 != pstP2PBlock->stP2PBlockHead.ucCMID1))
                    {
                        /*发送给第一个CM*/
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID1;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID1 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP1;
                        pstP2PSendHead->uiDstIP2Addr = 0U;

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

                        /*发送给第二个CM*/
                        pstP2PSendHead->ucSrcCMID = pstP2PBlock->stP2PBlockHead.ucCMID2;        /* CM Slot ID */
                        pstP2PSendHead->ucSrcNETID = pstP2PBlock->stP2PBlockHead.ucNetID2 - 1;  /* Cfg: 1-Net1, 2-Net 2*/
                        pstP2PSendHead->uiDstIP1Addr = pstP2PBlock->stP2PBlockHead.uiTargetStationIP2;
                        pstP2PSendHead->uiDstIP2Addr = 0U;

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
                        /*发送给同一个CM*/
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
                        /*发送给指定的CM*/
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
                        /*发送给指定的CM*/
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
                        /*log，配置信息错误*/
                        usRet = 0U;
                        //printf( "PSBCE\n" );
                    }
                }
            }
        }
    }
    
    return usRet;
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
        memset(&ucACKData[0], 0x00,4*1024);
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,SOE_SRV);
        
        if(pstSysSrvInfo != NULL)
        {
            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

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
            for(usIndex = 0; usIndex < usSOEEntryNum; usIndex++)
            {
                if(SW_SOE == emSOEType)
                {
                    iReadRet = SharedGetSOE(pstSOEReadReq->ucSOEBlockID, &stSoftSoe, usReadIndex,&uiNextIndex);
                    
                    if((iReadRet >= 0) || (THE_LAST_SOE_IN_QUEUE == iReadRet))
                    {
                        printf("ReadSOE:%d ",usReadIndex);
//                        printf("SSOE:%d ",stSoftSoe.ucBlockNo);
//                        printf("%d ",stSoftSoe.usIndex);
//                        printf("V:%d ",stSoftSoe.ucValue);
//                        printf("T:%d ",stSoftSoe.uiSec);
//                        printf("%d",stSoftSoe.uiMicroSec);
                        printf("\n");
                        
                        memcpy((puint8_t)&ucACKData[usBuffIndex],&stSoftSoe,sizeof(SoftSOE_t));
                        usBuffIndex += sizeof(SoftSOE_t);
                        
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
                    iReadRet = SharedGetHardSOE(&stHardSoe, usReadIndex,&uiNextIndex);
                    
                    if((iReadRet >= 0) || (THE_LAST_SOE_IN_QUEUE == iReadRet))
                    {
                        printf("ReadSOE:%d ",usReadIndex);
//                        printf("SSOE:%d ",stHardSoe.ucModNo);
//                        printf("%d ",stHardSoe.ucChannelNo);
//                        printf("V:%d ",stHardSoe.ucValue);
//                        printf("T:%d ",stHardSoe.uiSec);
//                        printf("%d",stHardSoe.uiMicroSec);
                        printf("\n");
                        
                        memcpy((puint8_t)&ucACKData[usBuffIndex],&stHardSoe,sizeof(HardSOE_t));
                        usBuffIndex += sizeof(HardSOE_t);
                        
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

//static void CommAMSTest(void)
//{
//    SysIoModInfo_t* pstIoModInfo = NULL;                     /*IO模块配置信息结构指针*/
//    AMSModInfo_t* pAMSModInfo = NULL;
//    puint16_t pusAMSModInfoNum = NULL;
//    SharedAppend_t *pstAppend = NULL;
//    uint8_t ucModAddr = 0U;
//    
//    pstIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM);
//    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
//    if(pstIoModInfo != NULL)
//    {
////        printf("AMS:");
//        for(ucModAddr = 0U; ucModAddr < MAX_MOD_NUM; ucModAddr++)
//        {
//            if((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag) ||
//               (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag))
//            {
//                pAMSModInfo->usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag;
//                pAMSModInfo->ucModAddr = ucModAddr;
//
////                printf(" Mod %d Type %x",pAMSModInfo->ucModAddr,pAMSModInfo->usModType);
//                
//                pAMSModInfo++;
//                (*pusAMSModInfoNum)++;
//            }
//        }
//        
////        printf("\n");
//        printf("AMS: Mod10 ");
//        if((pstIoModInfo != NULL) && (pstAppend != NULL))
//        {
//            printf(" Type %x State %x",pstIoModInfo->stSysIoSlaveInfo.stDevInfo[10].usModTag,pstAppend->stSlaveState[10].stSlaveDiag.ucOnlineDiag);
//            
//            if(((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[10].usModTag) ||
//               (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[10].usModTag)) &&
//               ((pstAppend->stSlaveState[10].stSlaveDiag.ucOnlineDiag & 0x01) != 0))
//            {
//                pAMSModInfo->usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[10].usModTag;
//                pAMSModInfo->ucModAddr = 10;
//                (*pusAMSModInfoNum) = 1U;
//                
//                printf("\n");
//                printf("ONLINE",pAMSModInfo->usModType,pstAppend->stSlaveState[10].stSlaveDiag.ucOnlineDiag);
//            }
//        }
//        printf("\n");
//    }
//}

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
    uint8_t ucChannelID = 0U;
    uint8_t ucMailRet = 0U;
    static uint8_t s_ucRetryTime = 0;
    static AMSMsgHandleStag_t s_emAMSMsgHandleStag = AMS_START;
    static uint16_t s_usMsgSEQ = 0U;
    static IPBUSMailData_t s_stIPBUSMail;
    static IPBUSMailData_t stRecvIPBUSMail;
    static uint8_t ucAMSMsg[1024];
    uint32_t uiCalcCRC = 0;
    AMSModScan_t stAMSModScan;
    SysIoModInfo_t* pstIoModInfo = NULL;                     /*IO模块配置信息结构指针*/
    AMSModInfo_t* pAMSModInfo = NULL;
    puint16_t pusAMSModInfoNum = NULL;
    SharedAppend_t *pstAppend = NULL;
    
    
//    static uint8_t s_ucTest = 0U;
//    s_ucTest++;
//    if(s_ucTest == 255)
//    {
//        CommAMSTest();
//    }
    

    switch(s_emAMSMsgHandleStag)
    {
        case AMS_START:
        {
            /*从系统资源获取来自CM的服务数据*/
            memset(&ucAMSMsg[0], 0x00, 1024);
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
                
//                uint8_t ucTest = 0;
//                printf("%d AMS ",emCMIndex);
//                for(ucTest = 16; ucTest < usAMSDataLen; ucTest++)
//                {
//                    printf("%x ",ucAMSMsg[ucTest]);
//                }
//                printf("\n");
                
                
                 /*计算数据CRC*/
                uiCalCrc = SysCrc32Cal(0x00, &ucAMSMsg[0], \
                                        pstMSGHead->usLength);
                usBuffIndex = pstMSGHead->usLength;
                 /*读取下发的CRC*/
                memcpy((puint8_t)&uiReadCrc,(puint8_t)&ucAMSMsg[usBuffIndex], \
                         CHKSUM_CRC_SIZE);

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
                                printf("Mod err%d\n",ucModAddr);
                            }
                            else if(ucChannelID > 32)
                            {
                                ucSubSeason = ERR_AMS_CHANNEL_ERR;
                                s_emAMSMsgHandleStag = AMS_END;
                                printf("Cn err%d\n",ucChannelID);
                            }
                            else
                            {
                                memset(&s_stIPBUSMail, 0x00, sizeof(IPBUSMailData_t));
                                memset(&s_stSendMailHeader, 0x00, sizeof(IPBUSAPPMailHeader_t));
                                
                                s_stIPBUSMail.usDataLen = pstMSGHead->usLength - sizeof(MSGHead_t);
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
                                    printf("IPBUS_RET_ERR\n");
                                }
                            }
                        }
                        
                        break;
                    
                        case AMS_SCANIO:
                        {
                            printf("AMS_SCANIO\n");
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
                                    if(pstIoModInfo != NULL)
                                    {
                                        for(ucModAddr = 0U; ucModAddr < MAX_MOD_NUM; ucModAddr++)
                                        {
                                            if(((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag) ||
                                               (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModAddr].usModTag)) &&
                                               ((pstAppend->stSlaveState[ucModAddr].stSlaveDiag.ucOnlineDiag & 0x01) != 0))
                                            {
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
                                    }
                                    else
                                    {/*服务处理成功*/
                                        printf("AMS SUCCESS\n");
                                        pstACKHead->ucResCode = RESCODE_SUCCESS;
                                        pstACKHead->ucDataFlg = 1U;
                                    }
                                    
                        //            DEBUG_COMM("SubSeason %d\n",ucSubSeason);
                        //            DEBUG_COMM("ResCode %d\n",pstACKHead->ucResCode);
                                    
                                    pstACKHead->usLength = (uint16_t)(usBuffIndex);
                                    pstACKHead->ucSubReason = ucSubSeason;
                                    /*计算并填充数据应答的CRC*/
                                    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                                    memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                                    usBuffIndex += sizeof(uiCalcCRC);
                                    usACKDataLen = (uint16_t)(usBuffIndex);
                                    
                        //            DEBUG_COMM("AMS HAS ACK\n");
                                    SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
//                                    uint8_t ucTest1 = 0;
//                                    printf("%d AMS RESP ",emCMIndex);
//                                    for(ucTest1 = 16; ucTest1 < usACKDataLen; ucTest1++)
//                                    {
//                                        printf("%x ",ucACKData[ucTest1]);
//                                    }
//                                    printf("\n");
                                    
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
                                        if(((MODTAG_AI == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[stAMSModScan.ucModAddr].usModTag) ||
                                           (MODTAG_AO == pstIoModInfo->stSysIoSlaveInfo.stDevInfo[stAMSModScan.ucModAddr].usModTag)) &&
                                           ((pstAppend->stSlaveState[stAMSModScan.ucModAddr].stSlaveDiag.ucOnlineDiag & 0x01) != 0))
                                        {
                                            pAMSModInfo->usModType = pstIoModInfo->stSysIoSlaveInfo.stDevInfo[stAMSModScan.ucModAddr].usModTag;
                                            pAMSModInfo->ucModAddr = stAMSModScan.ucModAddr;
                                            (*pusAMSModInfoNum) = 1U;
                                            usBuffIndex += sizeof(AMSModInfo_t);
                                        }
                                    }
                                    
                                    /*服务处理结束，产生应答*/
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

                                    if(ucSubSeason != ERR_SUCCESS)
                                    {/*服务处理失败*/
                                        pstACKHead->ucResCode = RESCODE_FAILED;
                                    }
                                    else
                                    {/*服务处理成功*/
                                        printf("AMS SUCCESS\n");
                                        pstACKHead->ucResCode = RESCODE_SUCCESS;
                                        pstACKHead->ucDataFlg = 1U;
                                    }
                                    
                        //            DEBUG_COMM("SubSeason %d\n",ucSubSeason);
                        //            DEBUG_COMM("ResCode %d\n",pstACKHead->ucResCode);
                                    
                                    pstACKHead->usLength = (uint16_t)(usBuffIndex);
                                    pstACKHead->ucSubReason = ucSubSeason;
                                    /*计算并填充数据应答的CRC*/
                                    uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
                                    memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
                                    usBuffIndex += sizeof(uiCalcCRC);
                                    usACKDataLen = (uint16_t)(usBuffIndex);
                                    
                        //            DEBUG_COMM("AMS HAS ACK\n");
                                    SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
//                                    uint8_t ucTest1 = 0;
//                                    printf("%d AMS RESP ",emCMIndex);
//                                    for(ucTest1 = 16; ucTest1 < usACKDataLen; ucTest1++)
//                                    {
//                                        printf("%x ",ucACKData[ucTest1]);
//                                    }
//                                    printf("\n");
                                    
                                    s_emAMSMsgHandleStag = FWRITE_START;
                                }
                                break;
                                
                                default:
                                    
                                break;
                            }
                        }
                        
                        break;
                    
                        default:
                            printf("ActualType err%d\n",pstMSGHead->usActualType);
                        break;
                    }
                }
            }
        }
        
        break;
        
        case AMS_WR:
        {
            if(s_ucRetryTime >= 3)
            {/*重发次数超过3次，不再处理*/
                ucSubSeason = ERR_AMS_TIMEOUT;
                printf("WR TO %d\n",s_ucRetryTime);
                s_emAMSMsgHandleStag = AMS_END;
            }
            else
            {
                printf("AMS_WR%d\n",s_ucRetryTime);
                ucMailRet = IPBUSMailSend((puint8_t)&s_stIPBUSMail, &s_stSendMailHeader);
                s_ucRetryTime++;
                
                if(IPBUS_RET_OK == ucMailRet)
                {
                    s_emAMSMsgHandleStag = AMS_RECV_WR_ACK;
                }
                else
                {
                    s_emAMSMsgHandleStag = AMS_WR;
                    printf("IPBUS_RET_ERR\n");
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
                if(s_ucWaitACKTimeout > 50)
                {
                    s_ucWaitACKTimeout = 0U;
                    s_emAMSMsgHandleStag = AMS_WR;
                    printf("AMS_WR_ACK TO\n");
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
                printf("RD TO %d\n",s_ucRetryTime);
            }
            else
            {
                ucMailRet = IPBUSMailSend((puint8_t)&s_stIPBUSMail, &s_stSendMailHeader);
                s_ucRetryTime++;
                printf("AMS_RD %d\n",s_ucRetryTime);
                
                if(IPBUS_RET_OK == ucMailRet)
                {
                    s_emAMSMsgHandleStag = AMS_RECV_RD_ACK;
                }
                else
                {
                    s_emAMSMsgHandleStag = AMS_RD;
                    printf("IPBUS_RET_ERR\n");
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
                    printf("AMS_RD_ACK err %x\n",stRecvMailHeader.usMailFunc);
                }
                else
                {
                    printf("AMS_RD_ACK %d\n",stRecvIPBUSMail.usDataLen);
                    if(stRecvIPBUSMail.usDataLen > 0)
                    {
                        ucSubSeason = ERR_SUCCESS;
                        s_emAMSMsgHandleStag = AMS_END;
                        s_ucWaitACKTimeout = 0U;
                        
                        uint8_t ucTest2 = 0;
                        for(ucTest2 = 0; ucTest2 < stRecvIPBUSMail.usDataLen; ucTest2++)
                        {
                            printf("%x ",stRecvIPBUSMail.ucData[ucTest2]);
                        }
                        printf("\n");
                    }
                }
            }
            else
            {
                s_ucWaitACKTimeout++;
                if(s_ucWaitACKTimeout > 50)
                {
                    printf("AMS_RD_ACK TO\n");
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
                printf("AMS FAILED %d\n",ucSubSeason);
            }
            else
            {/*服务处理成功*/
                printf("AMS SUCCESS\n");
                
                pstACKHead->ucResCode = RESCODE_SUCCESS;
                memcpy(&ucACKData[usBuffIndex], (puint8_t)&stRecvIPBUSMail.ucData[0], \
                        stRecvIPBUSMail.usDataLen);
                
                usBuffIndex += stRecvIPBUSMail.usDataLen;
            }
            
//            DEBUG_COMM("SubSeason %d\n",ucSubSeason);
//            DEBUG_COMM("ResCode %d\n",pstACKHead->ucResCode);
            
            pstACKHead->usLength = (uint16_t)(usBuffIndex);
            pstACKHead->ucSubReason = ucSubSeason;
            /*计算并填充数据应答的CRC*/
            uiCalcCRC = SysCrc32Cal( 0, ucACKData, pstACKHead->usLength );
            memcpy(&ucACKData[usBuffIndex],(puint8_t)&uiCalcCRC,sizeof(uiCalcCRC));
            usBuffIndex += sizeof(uiCalcCRC);
            usACKDataLen = (uint16_t)(usBuffIndex);
            
//            DEBUG_COMM("AMS HAS ACK\n");
            SysSetMsg(emCMIndex,AMS_RESP,ucACKData,usACKDataLen);
//            uint8_t ucTest1 = 0;
//            printf("%d AMS RESP ",emCMIndex);
//            for(ucTest1 = 16; ucTest1 < usACKDataLen; ucTest1++)
//            {
//                printf("%x ",ucACKData[ucTest1]);
//            }
//            printf("\n");
            
            s_emAMSMsgHandleStag = AMS_START;
        }
        
        break;
        
        default:
        break;
    }

    return;
}


static void CommDiagInit(void)
{
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

    for(emCMIndex = 0;emCMIndex < NUM_OF_CM; emCMIndex++)
    {
        memset(&ucACKData[0], 0x00,4*1024);
        /*获取服务数据在系统资源的存储结构指针*/
        pstSysSrvInfo = SysGetSrvInfo(emCMIndex,CLIENT_SRV);
        
        if(pstSysSrvInfo != NULL)
        {
            memset((puint8_t)pstSysSrvInfo, 0x00, sizeof(SysSrvInfo_t));

            /*从系统资源上获取上位机请求数据*/
            SysGetMsg(emCMIndex,CLIENT_REQ,&pstSysSrvInfo->stSysSrvData.ucSrvData[0],&usDiagDataLen);
    
            if(usDiagDataLen > 0)
            {
                
                printf("Get Diag Srv! %d\n",usDiagDataLen);
                
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
                            usACKDataLen = CommLOGRead(&pstSysSrvInfo->stSysSrvData.ucSrvData[0], &ucACKData[0]);
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
                    printf("DIAG HAS RSP !\n");
                    SysSetMsg(emCMIndex,CLIENT_RESP,ucACKData,usACKDataLen);
                }
            }
        }
        else
        {

        }
    }
    return;
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
            
            printf("ReadIndex %x\n",pstLOGReadReq->usGetIndex);
            
            for(usIndex = 0; usIndex < usLOGEntryNum; usIndex++)
            {
                iReadRet = LogRead(&stLogInfo, usReadIndex, pstLOGReadReq->ucLOGBlockID, &uiNextIndex);
                
                if((iReadRet >= 0) || (THE_LAST_LOG_IN_QUEUE == iReadRet))
                {
                    printf("ReadLOG:%d ",usReadIndex);
                    printf("%d ",stLogInfo.uiSec);
                    printf("%d ",stLogInfo.uiMicroSec);
                    printf("%x ",stLogInfo.uiLogID);
                    printf("\n");
                    
                    memcpy((puint8_t)&ucACKData[usBuffIndex],&stLogInfo,sizeof(LogInfo_t));
                    usBuffIndex += sizeof(LogInfo_t);
                    
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
                    usBuffIndex = sizeof(ACKHead_t) + sizeof(LOGReadRsp_t);
                    printf("LOG_QUEUE_EMPTY\n");
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
                            
//                            for(uint8_t x = CM1; x < NUM_OF_CM; x++)
//                            {
//                                printf("  %d bCfgFileModbusMaster is :%d\n",x,pstSysCfgFileTable->bCfgFileModbusMaster[x]);
//                                printf("  %d bReady is :%d\n",x,pstSysCfgFileTable->bReady[x]);
//                                printf("  %d bCfgFileCtrlStation is :%d\n",x,pstSysCfgFileTable->bCfgFileCtrlStation[x]);
//                                printf("  %d bCfgFileModbusSlave is :%d\n",x,pstSysCfgFileTable->bCfgFileModbusSlave[x]);
//                                printf("  %d bCfgFileOPC is :%d\n",x,pstSysCfgFileTable->bCfgFileOPC[x]);
//                                printf("  %d bCfgFileCM is :%d\n",x,pstSysCfgFileTable->bCfgFileCM[x]);
//                            }
                            printf("TRANS_ALL_FILE :%d\n",emCMIndex);
                            SysSetTransCfgFileFlag(TRANS_ALL_CFG_FILE_START, emCMIndex);
                            
                            printf("Flag:%d\n",pstSysCfgFileTable->bFlag);
//                            printf("ModbusMaster is :%d\n",pstSysCfgFileTable->bCfgFileModbusMaster[emCMIndex]);
                            printf("Ready:%d\n",pstSysCfgFileTable->bReady[emCMIndex]);
//                            printf("CtrlStation is :%d\n",pstSysCfgFileTable->bCfgFileCtrlStation[emCMIndex]);
//                            printf("ModbusSlave is :%d\n",pstSysCfgFileTable->bCfgFileModbusSlave[emCMIndex]);
//                            printf("OPC is :%d\n",pstSysCfgFileTable->bCfgFileOPC[emCMIndex]);
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

/*test*/

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
            
            SharedClearIECErrTime(TASK_UP1);
            SharedClearIECErrTime(TASK_UP2);
            
            SetLEDState(LED_NAME_ACTIVE, LED_OFF);
           
            pucDataAreaAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);
            if(pucDataAreaAddr != NULL)
            {
                memset(pucDataAreaAddr,0,MAX_RTDATA_Q_AREA_LEN);
            }

            pucDataAreaAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
            if(pucDataAreaAddr != NULL)
            {
                memset(pucDataAreaAddr,0,MAX_RTDATA_I_AREA_LEN);
            }

            pucDataAreaAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_G);
            if(pucDataAreaAddr != NULL)
            {
                memset(pucDataAreaAddr,0,MAX_RTDATA_G_AREA_LEN);
            }
            
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
            
            DEBUG_COMM("ONLINE_DOWNLOAD\n");
        }
        
        /*清除PM状态中，关于下装完成标志*/
        uint16_t usPMState = 0U;
        usPMState = SharedGetPMState();
        usPMState = usPMState & 0xfc;
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
//                    uint8_t x = 0U;
//                    DEBUG_COMM("CTRL SRV DATA %d ： ",pstSysSrvInfo->stSysSrvData.uiSrvLen);
//                        for(x = 0U; x < pstSysSrvInfo->stSysSrvData.uiSrvLen; x++)
//                        {
//                            printf(" %x ",pstSysSrvInfo->stSysSrvData.ucSrvData[x]);
//                        }
//                        printf(" \n ");
//                        
//                    DEBUG_COMM("TaskIndex: %d \n",emTaskIndex);
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


/*test*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
