/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 
*
*  文件名称    : Monitor.c
*
*  功能简介    : 
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-28
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
#include "../inc/Monitor.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t s_ucDataAreaInitFlg[MAX_TASK_NUM];
static ulong64_t s_ullTaskNextShedTime[MAX_TASK_NUM];
static uint8_t s_ucSingleStepCount[MAX_TASK_NUM];





/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void MonitorIECCycleCtrl1(void);

static void MonitorIECCycleCtrl2(void);

static void MonitorSetTaskNextShedTime(Task_ID_t emTaskID, ulong64_t ullTime);

static ulong64_t MonitorGetTaskNextShedTime(Task_ID_t emTaskID);

static void MonitorSetTaskDataInitFlg(Task_ID_t emTaskID, uint8_t ucDataAreaInitFlg);

static uint8_t MonitorGetTaskDataInitFlg(Task_ID_t emTaskID);

static void MonitorServiceProcessCycle(void);

static void MonitorServicPreprocess(void);

static void MonitorModbusPreprocess(void);

static void MonitorCS1131Preprocess(void);




/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: MonitorInit
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
void MonitorInit(void)
{
    uint32_t q1err = 0U;
    int32_t qnum1 = 0;
    
    uint8_t ucTaskIndex= 0U;
    PMController_t emPMId;
    
    for(emPMId = PMA; emPMId < NUM_OF_PM; emPMId++)
    {
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            SysSetTaskStatus(emPMId, ucTaskIndex, TS_STOP);
        }
    }
    
    for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
    {
        SysSetIECShedCtrlFlg(ucTaskIndex, IEC_END);
        SysSetInputPollFlg(ucTaskIndex, INPUT_POLL_END);
        SysSetOutputPollFlg(ucTaskIndex, OUTPUT_POLL_END);
    }

}

/*
***********************************************************************************************************************
* 函数名称: MonitorCycle
*
* 功能描述: 周期运行函数
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
void MonitorCycle(void)
{
    MonitorIECCycleCtrl1();



    MonitorIECCycleCtrl2();

}


/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: MonitorIECCycleCtrl1
*
* 功能描述: IEC任务的周期控制函数1
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
static void MonitorIECCycleCtrl1(void)
{
    uint8_t ucTaskIndex= 0U;
    uint32_t uiShedTime = 0U;
    uint8_t ucIECShedCtrlFlg = IEC_END;
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    task_status_t emTaskStat;

    for(ucTaskIndex = TASK_UP1; ucTaskIndex < TASK_UP2; ucTaskIndex++)
    {
        emTaskStat = SysGetTaskStatus(emLocalPMId,ucTaskIndex);
        ucIECShedCtrlFlg = SysGetIECShedCtrlFlg(ucTaskIndex);

        if(ucIECShedCtrlFlg != IEC_END)
        {
            if((TS_RUN == emTaskStat) || (TS_SINGLE_STEP == emTaskStat))
            {
                SysSetIECShedCtrlFlg(ucTaskIndex, IEC_CONTINUE);
            }
            if((TS_STOP == emTaskStat) || (TS_PAUSE == emTaskStat))
            {
                SysSetIECShedCtrlFlg(ucTaskIndex, IEC_END);
            }
        }
        else
        {/*本周期IEC运算完成*/
            
            DEBUG_MONITOR("  IEC_END  \n");
            
            
            if(NEED_INIT == MonitorGetTaskDataInitFlg(ucTaskIndex))
            {/*若任务数据区需要初始化，进行数据初始化*/
//                SysInitTaskDataArea(ucTaskIndex);/*指定任务的数据区初始化函数，！！！*/
                /*初始化数据区后，更新数据初始化标志*/
                MonitorSetTaskDataInitFlg(ucTaskIndex, NOT_NEED_INIT);
            }

            if((TS_SINGLE_STEP == emTaskStat) && (s_ucSingleStepCount[ucTaskIndex] != 0U))
            {/*任务单步且执行次数不为0,更新任务状态为暂停*/
                SysSetTaskStatus(emLocalPMId, ucTaskIndex, TS_PAUSE);
            }
        }

    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: MonitorIECCycleCtrl2
*
* 功能描述: IEC任务的周期控制函数2
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
static void MonitorIECCycleCtrl2(void)
{
    uint8_t ucTaskIndex= 0U;
    uint32_t uiShedTime = 0U;
    uint32_t uiDiffTime = 0U;
    SyncTime64_t unCurTime;                                         /* 当前同步时间             */
    SyncTime64_t unNextTime;                                        /* 下个工程调度时间         */
    SyncTime64_t unTempTime;                                        /* 临时时间，用于计算       */
    PMController_t emLocalPMId = SysGetLocalPMID();                 /* 本地PM ID号           */
    task_status_t emTaskStat;
    static uint8_t ucFirstFlg = 0U;
    static Key_Switch_Type_t emOldSwitchKey = KS_INIT;
    Key_Switch_Type_t emSwitchKey = KS_INIT;
    GlobaInit GlobalInitPtr = NULL;  
    puint8_t pucDataAreaAddr = NULL;
    
    
/*test*/  
    emSwitchKey = SharedGetSwitchPos();
    
    if(emOldSwitchKey != emSwitchKey)
    {
        printf("......emSwitchKey  is : %d \n",emSwitchKey);
        if(KS_RUN == emSwitchKey)
        {
            SysSetTaskStatus(emLocalPMId, TASK_UP1, TS_RUN);
            SysSetTaskStatus(emLocalPMId, TASK_UP2, TS_RUN);
        }
        
        if((KS_STOP == emSwitchKey))
        {
            SysSetTaskStatus(emLocalPMId, TASK_UP1, TS_STOP);
            SysSetTaskStatus(emLocalPMId, TASK_UP2, TS_STOP);
        }
        
        emOldSwitchKey = emSwitchKey;
    }
    
/*test*/
    
    for(ucTaskIndex = TASK_UP1; ucTaskIndex < TASK_UP2; ucTaskIndex++)
    {
        SysSetTaskStatus(emLocalPMId,ucTaskIndex, TS_RUN);
        
        emTaskStat = SysGetTaskStatus(emLocalPMId,ucTaskIndex);
        
//        DEBUG_MONITOR("  emTaskStat is : %x \n",emTaskStat);
        
        if((TS_STOP == emTaskStat) || (TS_RUN == emTaskStat) ||
           (TS_PAUSE == emTaskStat) || (TS_SINGLE_STEP == emTaskStat))
        {

            /* 获取当前PM同步时间 */
            SysGetSyncTime((SyncTime64_t*)&unCurTime);
            
            /* 获取任务的工程调度周期*/
            uiShedTime = SysGetPrjShedTime(ucTaskIndex);
            uiShedTime = uiShedTime*10;/*转化为百微秒*/
            
            if((uiShedTime != 0U ) && (0U == (uiShedTime % CFG_MINIMUM_BEAT_TIME)) && (HAS_BE_CONFIGURED == SysGetIsConfigured()))
            {
                
                if(DOWN_LOAD_COMPLETE == SharedGetDownloadFlg())
                {
                    GlobalInitPtr = (GlobaInit)SharedGetTaskGlobalInitEntry(ucTaskIndex);
                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
                    if(pucDataAreaAddr != NULL)
                    {
                        GlobalInitPtr((uint32_t)pucDataAreaAddr);
                        
                        DEBUG_MONITOR("  Call GlobalInitPtr OK !  0x%x \n", GlobalInitPtr);
                        
                    }
                    
                    SharedSetDownloadFlg(NOT_DOWNLOAD);
                    
                }
                
                
                //printf("  uiShedTime is : %d \n",uiShedTime);
                //printf("  CodeEntry is %x    GolbalInitEntry is %x  \n",SharedGetTaskCodeEntry(ucTaskIndex),\
                //        SharedGetTaskGlobalInitEntry(ucTaskIndex));
                
                
                /* 计算下个工程调度周期起始时间为 */
                unTempTime.ullTime = unCurTime.ullTime;
                
            	if(0U == ucFirstFlg)
            	{
                    /*计算并设置下一个IEC周期开始时间*/
                    unNextTime.ullTime = unCurTime.ullTime + uiShedTime - (unTempTime.stTimeStruct.uiLow32b % uiShedTime);
                    
    //                DEBUG_MONITOR("  @@unNextTime uiHigh32b is: %x \n",unNextTime.stTimeStruct.uiHigh32b);
    //                DEBUG_MONITOR("  @@unNextTime uiLow32b is: %x \n",unNextTime.stTimeStruct.uiLow32b);
                    /*将下一个IEC周期开始时间，记录下来*/
                    
                    MonitorSetTaskNextShedTime(ucTaskIndex,unNextTime.ullTime);
                    
                    ucFirstFlg = 1U;
            	}
                
                unNextTime.ullTime = MonitorGetTaskNextShedTime(ucTaskIndex);

                
    //            DEBUG_MONITOR("  unCurTime uiHigh32b is : %x \n",unCurTime.stTimeStruct.uiHigh32b);
    //            DEBUG_MONITOR("  unCurTime uiLow32b is : %x \n",unCurTime.stTimeStruct.uiLow32b);
    //            DEBUG_MONITOR("  uiShedTime is : %x \n",uiShedTime);
    //            DEBUG_MONITOR("  unNextTime uiHigh32b is : %x \n",unNextTime.stTimeStruct.uiHigh32b);
    //            DEBUG_MONITOR("  unNextTime uiLow32b is : %x \n",unNextTime.stTimeStruct.uiLow32b);
                
                

                if(unCurTime.stTimeStruct.uiLow32b - unNextTime.stTimeStruct.uiLow32b <= CFG_MINIMUM_BEAT_TIME)
                {/*由操作系统调度保证5ms小周期，当当前时间大于上一次记录的调度开始时间且不大于5ms时，新的一个IEC周期开始*/
                    if(SysGetInputPollFlg(ucTaskIndex) != INPUT_POLL_END)
                    {
                        /*error log*/
                        
                        DEBUG_MONITOR("  SysGetInputPollFlg(ucTaskIndex) != INPUT_POLL_END \n");
                        
                    }
                    else
                    {/*输入轮询完成，若需要开始新一轮输入轮询*/
                        if(emTaskStat != TS_STOP)
                        {/*任务状态非Stop时，进行输入轮询*/
    //                        SysSetInputPollFlg(ucTaskIndex,INPUT_POLL_START);
                            
                            //DEBUG_MONITOR("  SysSetInputPollFlg(ucTaskIndex,INPUT_POLL_START) \n");
                            
                        }
                        if((TS_RUN == emTaskStat) || (TS_SINGLE_STEP == emTaskStat))
                        {/*任务状态为运行和单步时，进行IEC运算*/
                            SysSetIECShedCtrlFlg(ucTaskIndex, IEC_START);
                            
                            DEBUG_MONITOR("   IEC_START  \n");
                            SetLEDState(LED_NAME_ACTIVE, LED_ON);
                            
                        }
                        if(TS_SINGLE_STEP == emTaskStat)
                        {
                            s_ucSingleStepCount[ucTaskIndex]++;
                        }
                    }
                    /*计算并设置下一个IEC周期开始时间*/
                    unNextTime.ullTime = unCurTime.ullTime + uiShedTime - (unTempTime.stTimeStruct.uiLow32b % uiShedTime);
                    
                    DEBUG_MONITOR("  @@unNextTime uiHigh32b is: %x \n",unNextTime.stTimeStruct.uiHigh32b);
    //                DEBUG_MONITOR("  @@unNextTime uiLow32b is: %x \n",unNextTime.stTimeStruct.uiLow32b);
                    /*将下一个IEC周期开始时间，记录下来*/
                    
                    MonitorSetTaskNextShedTime(ucTaskIndex,unNextTime.ullTime);
                }
            }
            else
            {
                //printf("#### uiShedTime is : %d \n",uiShedTime);
            }
        }
    }
    return;
}


/*
***********************************************************************************************************************
* 函数名称: MonitorSetTaskNextShedTime
*
* 功能描述: IEC任务的周期控制函数2
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
static void MonitorSetTaskNextShedTime(Task_ID_t emTaskID, ulong64_t ullTime)
{
    s_ullTaskNextShedTime[emTaskID] = ullTime;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorGetTaskNextShedTime
*
* 功能描述: IEC任务的周期控制函数2
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
static ulong64_t MonitorGetTaskNextShedTime(Task_ID_t emTaskID)
{
    return s_ullTaskNextShedTime[emTaskID];
}

/*
***********************************************************************************************************************
* 函数名称: MonitorSetTaskDataInitFlg
*
* 功能描述: 设置指定任务的数据区初始化标志
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
static void MonitorSetTaskDataInitFlg(Task_ID_t emTaskID, uint8_t ucDataAreaInitFlg)
{
    s_ucDataAreaInitFlg[emTaskID] = ucDataAreaInitFlg;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MonitorGetTaskDataInitFlg
*
* 功能描述: 获取指定任务的数据区初始化标志
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
static uint8_t MonitorGetTaskDataInitFlg(Task_ID_t emTaskID)
{
    return s_ucDataAreaInitFlg[emTaskID];
}

/*
***********************************************************************************************************************
* 函数名称: MonitorServiceProcessCycle
*
* 功能描述: 完成对服务队列的读取，服务的预处理，服务的处理等功能的调度，以最终完成服务处理功能.
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
static void MonitorServiceProcessCycle(void)
{

}

/*
***********************************************************************************************************************
* 函数名称: MonitorServicPreprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorServicPreprocess(void)
{

}
/*
***********************************************************************************************************************
* 函数名称: MonitorModbusPreprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorModbusPreprocess(void)
{
    bool_t bRet = false;
    uint16_t usModbusDataLen = 0U;
    uint8_t ucModbusData[MAX_SM_MSG_LEN];
    CMController_t emCMID = CM1;


    /*从共享内存上获取Modbus服务数据*/
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        bRet = SysGetSMMsg(SM_MODBUS_REQ, emCMID, ucModbusData, &usModbusDataLen);
        if((true == bRet) || (usModbusDataLen != 0U))
        {/*函数返回值正确且Modbus数据长度大于0，预处理数据*/


            
            
            
        }
    }


    return;
}


/*
***********************************************************************************************************************
* 函数名称: MonitorCS1131Preprocess
*
* 功能描述: 完成对服务队列的读取，服务的分类和预处理，为服务的最终处理做准备.
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
static void MonitorCS1131Preprocess(void)
{
    bool_t bRet = false;
    uint16_t usCS1131DataLen = 0U;
    uint8_t ucCS1131Data[MAX_SM_MSG_LEN];
    MSGHead_t* pstMSGHead = NULL;
    MSGHead_t* pstACKHead = NULL;
    CMController_t emCMID = CM1;


    /*从共享内存上获取1131服务数据*/
    for(emCMID = CM1; emCMID < NUM_OF_CM; emCMID++)
    {
        bRet = SysGetSMMsg(SM_CS1131_REQ, emCMID, ucCS1131Data, &usCS1131DataLen);
        if((true == bRet) || (usCS1131DataLen != 0U))
        {/*函数返回值正确且CS1131数据长度大于0，预处理数据*/
            /*解析同步过来的CS1131请求数据，包括，同步服务头，1131消息头，消息内容等。*/
            /*暂只需要对写变量和强制服务进行预处理*/

            
            
            
        }
    }


    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
