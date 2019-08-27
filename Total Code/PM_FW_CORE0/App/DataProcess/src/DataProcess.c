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
*  文件名称    : DataProcess.c
*
*  功能简介    :
*
*  作者        : 朱耿华
*
*  创建日期    : 2016-06-10
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
#include <Srv/Scheduler/cagos.h>
#include <Srv/Scheduler/include/stdio.h>
#include "../inc/DataProcess.h"
#include "../inc/DataExChange.h"
#include "Srv/SharedSys/inc/Sys.h"
#include "Srv/SharedSys/inc/SysCore0RtData.h"
#include "Srv/Sys/inc/CommSysConfig.h"
#include "Srv/Sys/inc/SysHardware.h"
#include "Srv/SharedSys/inc/SysMonitor.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/SharedMem/inc/SharedConfig.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SharedMem/inc/SharedSOE.h"
#include "Srv/IPBUS/inc/IPBUS.h"
#include "Srv/SafetyProtocol/inc/PMIOSafetyComm.h"
#include "Srv/Log/inc/Log.h"
#include "Srv/Log/inc/LogID.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static SharedRTDataArea_t g_stBackupRtData;

static uint8_t ucUPRollingSlaveNo[MAX_TASK_NUM][DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2]; /* 记录每周期轮询的从站号 */

static uint32_t uiUPRollingSlaveNum[MAX_TASK_NUM] = {0};

static uint8_t ucSlaveQuality[IPBUS_MODULE_MAX_NUM / 2][4]; /* 临时用于存放IO质量*/

/* 标志位，用于输入拷贝只执行一次 */
static uint32_t uiInputTaskFlag[MAX_TASK_NUM] = {0};

static SysTime_t stSOESysTime;

static uint8_t ucInputRetainSlaveNo[MAX_TASK_NUM][DATAPROCESS_SLAVE_POLL_NUM_TOTAL];
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

//static void DataProcessFillSlaveRollingListTable(void);

static void DataProcessSlaveSendHandle(Task_ID_t emTaskID, uint8_t ucSendDest[], uint8_t ucSlaveNo, bool_t bActiveFV, 
                                       bool_t bEnableFlg, bool_t bDataValid, bool_t bSlaveConfirm);

static uint8_t DataProcessSlaveRecvHandle(Task_ID_t emTaskID, uint8_t ucRecvSrc[], uint8_t ucSlaveNo, uint16_t usModTag);

static uint32_t DataProcessCheckDownloadIOConfigFinish(void);

static uint8_t DataProcessInputDataCopy(uint32_t uiTaskID);

static uint8_t DataProcessRecvNonSafetyDataHandle(uint8_t ucRecvSrc[]);

static uint8_t DataProcessRecvSafetyDataHandle(uint8_t ucRecvSrc[], uint32_t uiIecOffset, uint8_t ucSlotLen, uint8_t ucSlaveNo, uint16_t usModTag);

static uint8_t DataProcessRecvDiagDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo);

static void DataProcessHardSOEHandle(uint8_t ucRecvSrc[], uint8_t ucSOENum, uint8_t ucSlaveNo,uint16_t usModTag);

static uint8_t DataProcessRecvOSPDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo);

static void DataProcessSetIPBUSLight(void);

static void DataprocessSetPollNum(uint8_t *pucUP1HandleNum, uint8_t *pucUP2HandleNum);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: DataProcessInit
*
* 功能描述: 数据处理模块初始化
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
void DataProcessInit(void)
{
    /* 数据交换表决初始化 */
    DataExChangeInit();
    /* IPBUS相关的FPGA RAM初始化 */
    IPBUSDualRamInit();
    /* 置应用层状态为停止状态 */
    IPBUSSetAPPState(APP_STATE_STOP);
    /* 置应用层标志在线/离线位为在线 */
    IPBUSSetAPPOnlineFlag(IPBUS_APP_ONLINE);
    
    DataProcessModEnableFlgInit();
    
    memset(&g_stBackupRtData, 0x00, sizeof(SharedRTDataArea_t));
    
    memset(&ucInputRetainSlaveNo[0][0], 0x00, sizeof(uint8_t)*MAX_TASK_NUM*DATAPROCESS_SLAVE_POLL_NUM_TOTAL);

    return;
}


/*
***********************************************************************************************************************
* 函数名称: DataProcessOutputHandle
*
* 功能描述: 输入数据处理
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
void DataProcessOutputHandle(void)
{
    uint32_t i = 0U;
    uint8_t ucUPHandleNum[MAX_TASK_NUM] = {0}; /* 任务每周期轮询IO个数 */
    uint32_t uiSlaveCount = 0U;
    uint8_t ucOutputBuf[DATAPROCESS_OUTPUT_MAX_LEN] = {0}; /* 数据区buf */
    bool_t bActiveFV = false;
    bool_t bEnableFlg = false;
    bool_t bDataValid = false;
    bool_t bSlaveConfirm = false;
    uint8_t ucDataExBuf[DATAPROCESS_AO_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucPreDataExBuf[DATAPROCESS_AO_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucNextDataExBuf[DATAPROCESS_AO_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint32_t uiDataExBufIndex = 0U;
    uint8_t ucPreDataFlg = 0U;
    uint8_t ucNextDataFlg = 0U;
    puint8_t pucPreDataVoteAddr = NULL; /* 数据表决地址 */
    puint8_t pucNextDataVoteAddr = NULL; /* 数据表决地址 */
    uint8_t ucOldDataVoteBuf[DATAPROCESS_AO_SAFETYDATA_LEN] = {0}; /* 数据表决旧数据buf */
    uint8_t ucDesDataVoteBuf[DATAPROCESS_AO_SAFETYDATA_LEN] = {0}; /* 数据表决表决后buf */
    uint8_t ucDiagBuff[32] = {0}; /* 数据表决诊断buf，最大32通道 */
    uint32_t uiDataVoteBufIndex = 0U;
    uint8_t ucRollingListBuf[256] = {0U}; /* 轮询列表buf */
    uint32_t uiRollingListBufIndex = 0U;
    uint32_t uiUPRollingListBufIndex[MAX_TASK_NUM] = {0U};
    puint8_t pucStartAddr = NULL;
    SysIoModInfo_t* pstUPIoModInfo[MAX_TASK_NUM] = {NULL};
    uint8_t ucSlaveNo = 0U;
    uint8_t ucTaskIndex= 0U;
    /* 标志位，用于控制IO轮询的计数 */
    static uint32_t uiTaskSlaveCountIndex[MAX_TASK_NUM] = {0U};
    SharedAppend_t *pstAppend = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;
    
    /* 获取控制站信息 */
    pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM);
    /* 获取Append数据区 */
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    /* 获取对应IO表 */
    pstUPIoModInfo[TASK_UP1] = SharedGetIoModInfo(TASK_UP1);
    pstUPIoModInfo[TASK_UP2] = SharedGetIoModInfo(TASK_UP2);

    if((DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_CONFIG_SLAVE) && (pstUPIoModInfo[TASK_UP1] != NULL) &&
       (pstUPIoModInfo[TASK_UP2] != NULL) && (pstAppend != NULL) && (pstCtrlStationInfo != NULL))
    {
        /* 点IPBUS灯，和轮询周期无关，每周期判断 */
        DataProcessSetIPBUSLight();

        /* 清除记录每周期轮询的从站号 */
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            memset(&ucUPRollingSlaveNo[ucTaskIndex][0], 0, DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2);
        }

        /* 设置每个任务每周期轮询的IO个数 */
        DataprocessSetPollNum(&ucUPHandleNum[TASK_UP1], &ucUPHandleNum[TASK_UP2]);

        /* 清输入拷贝标志 */
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
            {
                uiInputTaskFlag[ucTaskIndex] = 0;
            }
        }

        /* 需交换的输出数据打包 */
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
            {
                if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(ucTaskIndex))
                {/* 已经经过一次IEC运算，可以进行输出数据处理 */
                    /* Q区交换，每周期处理固定数量*/
                    for(i = 0; i < ucUPHandleNum[ucTaskIndex]; i++)
                    {
                        /* 处理输入模块的输出数据，每次处理一对模块 */
                        if((uiTaskSlaveCountIndex[ucTaskIndex] + 2 * i) < pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex])
                        {/* 输出模块 */
                            uiSlaveCount = uiTaskSlaveCountIndex[ucTaskIndex] + 2 * i;
                            ucSlaveNo = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.ucOutDevTabl[ucTaskIndex][uiSlaveCount];
                            if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                            {
                                pucStartAddr = &(g_stBackupRtData.ucQ[0]);
                                memcpy(&ucDataExBuf[uiDataExBufIndex],
                                        pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                        pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                                uiDataExBufIndex = uiDataExBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                            }
                        }
                        else
                        {/* 非输出模块 */
                            break;
                        }
                    }
                }
            }
        }

        /* 输出数据交换 */
        if(uiDataExBufIndex > 0)
        {/* 有Q区数据需要交换 */
            DataExChange(&ucDataExBuf[0], (uint16_t)uiDataExBufIndex, &ucPreDataExBuf[0], &ucNextDataExBuf[0], &ucPreDataFlg, &ucNextDataFlg, 0);
        }

        /* 输出数据表决 */
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
            {
                if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(ucTaskIndex))
                {
                    /* UP1按模块依次表决 */
                    for(i = 0; i < ucUPHandleNum[ucTaskIndex]; i++)
                    {
                        if((uiTaskSlaveCountIndex[ucTaskIndex] + 2 * i) < pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex])
                        {
                            memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AO_SAFETYDATA_LEN);
                            memset(&ucDiagBuff[0], 0x00, 32);
                            uiSlaveCount = uiTaskSlaveCountIndex[ucTaskIndex] + 2 * i;
                            ucSlaveNo = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.ucOutDevTabl[ucTaskIndex][uiSlaveCount];
                            if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                            {
                                /* 拷贝旧数据 */
                                pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);

                                memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                                if(ucPreDataFlg != 0x01)
                                {
                                    pucPreDataVoteAddr = NULL;
                                }
                                else
                                {
                                    pucPreDataVoteAddr = &ucPreDataExBuf[uiDataVoteBufIndex];
                                }
                                if(ucNextDataFlg != 0x01)
                                {
                                    pucNextDataVoteAddr = NULL;
                                }
                                else
                                {
                                    pucNextDataVoteAddr = &ucNextDataExBuf[uiDataVoteBufIndex];
                                }
                                /* 数据表决 */
                                DataExChangeVote(ucTaskIndex, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                        pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);
                                /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                                memcpy(pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                            }
                        }
                    }
                }
            }
        }

        /* 生成输出数据包，并填充主卡RAM */
        if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
        {/* 获取权限，向双口拷贝数据 */

            for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
            {
                if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
                {
                    for(i = 0; i < 2 * ucUPHandleNum[ucTaskIndex]; i++)
                    {
                        /* 每次处理单个模块 */
                        uiSlaveCount = uiTaskSlaveCountIndex[ucTaskIndex] + i;
                        if(uiSlaveCount < (uint32_t)pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex])
                        {
                            ucSlaveNo = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.ucOutDevTabl[ucTaskIndex][uiSlaveCount];
                            if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                            {
                                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                                {
                                    memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                    /* 输出数据生成 */
                                    if (OUTPUT_POLL_DISENABLE == SysGetOutputPollFlg(ucTaskIndex))
                                    {
                                        bActiveFV = true;
                                    }
                                    else
                                    {
                                        bActiveFV = false;
                                    }
                                    /*设置从站使能标志*/
                                    if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(ucTaskIndex))
                                    {
                                        if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                        {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                            DataProcessSetModEnableFlg(ucSlaveNo, true);
                                        }
                                    }
                                    bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);

                                    if(DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(ucTaskIndex))
                                    {
                                        bDataValid = true;
                                    }
                                    else
                                    {
                                        bDataValid = false;
                                    }
                                    
                                    if(((pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag & SLAVE_COMFIRM_MASK) == 0) &&
                                        (0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm))
                                    {
                                        bSlaveConfirm = false;
                                    }
                                    else
                                    {
                                        bSlaveConfirm = true;
                                    }
                                    

                                    DataProcessSlaveSendHandle(ucTaskIndex,&ucOutputBuf[0], ucSlaveNo, bActiveFV, 
                                                               bEnableFlg, bDataValid, bSlaveConfirm);

                                    /* 通过PCIE从双口RAM填写输出数据 */
                                    ipbus_write(WRITE_IPBUS_OUTPUT,
                                                &ucOutputBuf[0],
                                                pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                                pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                                }
                                else
                                {
                                    if(0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm)
                                    {
                                        pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag =
                                        pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag &(~SLAVE_COMFIRM_MASK);
                                    }
                                }
                            }
                            ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                            ucUPRollingSlaveNo[ucTaskIndex][uiUPRollingListBufIndex[ucTaskIndex]] = ucSlaveNo;
                            uiRollingListBufIndex = uiRollingListBufIndex + 1;
                            uiUPRollingListBufIndex[ucTaskIndex] = uiUPRollingListBufIndex[ucTaskIndex] + 1;
                            uiUPRollingSlaveNum[ucTaskIndex] = uiUPRollingListBufIndex[ucTaskIndex];
                        }
                        else if((uiSlaveCount - (uint32_t)pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex]) < pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usInDevNum[ucTaskIndex])
                        {
                            uiSlaveCount = uiSlaveCount - (uint32_t)pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex];
                            ucSlaveNo = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.ucInDevTabl[ucTaskIndex][uiSlaveCount];
                            if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                            {
                                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                                {
                                    memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                    /* 输出数据生成 */
                                    bActiveFV = false;
                                    /*设置从站使能标志*/
                                    if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(ucTaskIndex))
                                    {
                                        if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                        {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                            DataProcessSetModEnableFlg(ucSlaveNo, true);
                                        }
                                    }
                                    bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);

                                    if(DATA_SYNC_COMPLETE == SharedGetSyncDataFlag(ucTaskIndex))
                                    {
                                        bDataValid = true;
                                    }
                                    else
                                    {
                                        bDataValid = false;
                                    }
                                
                                    if(((pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag & SLAVE_COMFIRM_MASK) == 0) &&
                                    (0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm))
                                    {
                                        bSlaveConfirm = false;
                                    }
                                    else
                                    {
                                        bSlaveConfirm = true;
                                    }
                                
                                    DataProcessSlaveSendHandle(ucTaskIndex, &ucOutputBuf[0], ucSlaveNo, bActiveFV,
                                                           bEnableFlg, bDataValid, bSlaveConfirm);

                                    /* 通过PCIE从双口RAM填写输出数据 */
                                    ipbus_write(WRITE_IPBUS_OUTPUT,
                                            &ucOutputBuf[0],
                                            pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                            pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                                }
                            }

                            ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                            ucUPRollingSlaveNo[ucTaskIndex][uiUPRollingListBufIndex[ucTaskIndex]] = ucSlaveNo;
                            uiRollingListBufIndex = uiRollingListBufIndex + 1;
                            uiUPRollingListBufIndex[ucTaskIndex] = uiUPRollingListBufIndex[ucTaskIndex] + 1;
                            uiUPRollingSlaveNum[ucTaskIndex] = uiUPRollingListBufIndex[ucTaskIndex];
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            /* 设置应用层状态为运行 */
            IPBUSSetAPPState(APP_STATE_RUN);
            /*根据配置信息填充从站周期轮询列表*/
            IPBUSSetSlaveRollingList(&ucRollingListBuf[0], 2 * DATAPROCESS_SLAVE_POLL_NUM_TOTAL);
            /* 释放权限 */
            IPBUSReleaseDataRAMRight();
            
            if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
            {
                IPBUSReleaseDataRAMRight();
            }
        }

        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
            {
                uiTaskSlaveCountIndex[ucTaskIndex] = uiTaskSlaveCountIndex[ucTaskIndex] + 2 * ucUPHandleNum[ucTaskIndex]; /* 每次处理一对模块 */
                if(uiTaskSlaveCountIndex[ucTaskIndex] >= (pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usOutDevNum[ucTaskIndex] + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.usInDevNum[ucTaskIndex]))
                {
                    uiTaskSlaveCountIndex[ucTaskIndex] = 0;
                    SysSetIOPollFlg(ucTaskIndex, IO_POLL_END);
                }
            }
        }
    }
    else
    {
//        LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_NO_IOCONFIG);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessInputHandle
*
* 功能描述: 输入数据处理
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
void DataProcessInputHandle(void)
{
    uint32_t uiSlaveCount = 0U;
    uint8_t ucInputBuf[DATAPROCESS_INPUT_MAX_LEN] = {0}; /* 数据区buf */
    uint16_t usModTag = 0U;

    uint8_t ucDataExBuf[(DATAPROCESS_AI_QDATA_LEN +DATAPROCESS_AI_SAFETYDATA_LEN) * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucPreDataExBuf[(DATAPROCESS_AI_QDATA_LEN +DATAPROCESS_AI_SAFETYDATA_LEN) * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucNextDataExBuf[(DATAPROCESS_AI_QDATA_LEN +DATAPROCESS_AI_SAFETYDATA_LEN) * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint32_t uiDataExBufIndex = 0U;
    uint8_t ucPreDataFlg = 0U;
    uint8_t ucNextDataFlg = 0U;

    puint8_t pucPreDataVoteAddr = NULL; /* 数据表决地址 */
    puint8_t pucNextDataVoteAddr = NULL; /* 数据表决地址 */
    uint8_t ucOldDataVoteBuf[DATAPROCESS_AI_SAFETYDATA_LEN] = {0}; /* 数据表决旧数据buf */
    uint8_t ucDesDataVoteBuf[DATAPROCESS_AI_SAFETYDATA_LEN] = {0}; /* 数据表决表决后buf */
    uint8_t ucDiagBuff[32] = {0}; /* 数据表决诊断buf，最大32通道 */
    uint32_t uiDataVoteBufIndex = 0U;

    puint8_t pucStartAddr = NULL;

    SysIoModInfo_t* pstUPIoModInfo[MAX_TASK_NUM] = {NULL};
    uint8_t ucSlaveNo = 0U;
    uint8_t ucTaskIndex= 0U;
    SharedAppend_t *pstAppend = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;
    
    long64_t ulStartTime = 0;
    long64_t ulEndTime = 0;
    ulStartTime = SysGetFreeCounterValue();

    pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM); 
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    /* 取得系统时间，用于所有的硬SOE时间，减少读寄存器时间 */
    SysGetSysTime(&stSOESysTime);

    pstUPIoModInfo[TASK_UP1] = SharedGetIoModInfo(TASK_UP1);
    pstUPIoModInfo[TASK_UP2] = SharedGetIoModInfo(TASK_UP2);

    if((DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_CONFIG_SLAVE) && (pstUPIoModInfo[TASK_UP1] != NULL) &&
       (pstUPIoModInfo[TASK_UP1] != NULL) && (pstAppend != NULL) && (pstCtrlStationInfo != NULL))
    {
        if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
        {/* 获取权限，接收输入数据 */
            for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
            {
                /*进行UP1输入轮询*/
                for(uiSlaveCount = 0; uiSlaveCount < uiUPRollingSlaveNum[ucTaskIndex] / 2; uiSlaveCount++)
                {/* 按照输出处理时确定的模块进行轮询 */
                    ucSlaveNo = ucUPRollingSlaveNo[ucTaskIndex][2 * uiSlaveCount];
                    memset(&ucSlaveQuality[ucSlaveNo/2][0], 0, 4); /* 清临时诊断区 */
                    if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                    {
                        /* 清备用数据区，只清一次 */
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memset(pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                0x00, pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                        {
                            memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                            /* 通过PCIE从双口RAM获得输入数据 */
                            ipbus_read(READ_IPBUS_INPUT,
                                    &ucInputBuf[0],
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                            /* 输入数据解析，先拷贝到临时数据区 */
                            DataProcessSlaveRecvHandle(ucTaskIndex, &ucInputBuf[0], ucSlaveNo, pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                        }
                        else
                        {
                            if(0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm)
                            {
                                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag =
                                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag &(~SLAVE_COMFIRM_MASK);
                            }
                        }
                    }
                    ucSlaveNo = ucUPRollingSlaveNo[ucTaskIndex][2 * uiSlaveCount + 1];
                    if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                    {
                        if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                        {
                            memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                            /* 通过PCIE从双口RAM获得输入数据 */
                            ipbus_read(READ_IPBUS_INPUT,
                                    &ucInputBuf[0],
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);

                            /* 输入数据解析，先拷贝到临时数据区 */
                            DataProcessSlaveRecvHandle(ucTaskIndex,&ucInputBuf[0], ucSlaveNo, pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                        }
                        else
                        {
                            if(0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm)
                            {
                                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag =
                                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag &(~SLAVE_COMFIRM_MASK);
                            }
                        }
                    }
                    
                    /* 拷贝到数据交换区 */
                    usModTag = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                    switch(usModTag)
                    {
                        case MODTAG_AI:
                        {
                            /* 拷贝数据 */
                            pucStartAddr = &(g_stBackupRtData.ucI[0]);
                            memcpy(&ucDataExBuf[uiDataExBufIndex],
                                    pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                            uiDataExBufIndex = uiDataExBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                            /* 拷贝质量 */
                            memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_AI_QDATA_LEN);
                            uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_AI_QDATA_LEN;
                        }
                        break;

                        case MODTAG_DI:
                        {
                            pucStartAddr = &(g_stBackupRtData.ucI[0]);
                            memcpy(&ucDataExBuf[uiDataExBufIndex],
                                    pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                            uiDataExBufIndex = uiDataExBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                            memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_DI_QDATA_LEN);
                            uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_DI_QDATA_LEN;
                        }
                        break;

                        case MODTAG_OSP:
                        {
                            pucStartAddr = &(g_stBackupRtData.ucI[0]);
                            memcpy(&ucDataExBuf[uiDataExBufIndex],
                                    pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                            uiDataExBufIndex = uiDataExBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                            memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_OSP_QDATA_LEN);
                            uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_OSP_QDATA_LEN;
                        }
                        break;

                        default :
                        {
                            ;
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_NO_RIGHT);
        }

        /* 三系同步开始进行数据交换 */
        ulEndTime = SysGetFreeCounterValue();
        if(ulEndTime - ulStartTime < 500)
        {
            udelay(500-(ulEndTime - ulStartTime));
        }
        else
        {
//            printCfg(1);
//            printf("T1\n");
//            printCfg(0);
            LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_TO);
        }

        /* 数据进行交换及表决 */
        if(uiDataExBufIndex > 0)
        {/* 有I区数据需要交换 */
            /* 交换I区数据 */
            DataExChange(&ucDataExBuf[0], (uint16_t)uiDataExBufIndex, &ucPreDataExBuf[0], &ucNextDataExBuf[0], &ucPreDataFlg, &ucNextDataFlg, 1);

            for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
            {
                for(uiSlaveCount = 0; uiSlaveCount < uiUPRollingSlaveNum[ucTaskIndex] / 2; uiSlaveCount++)
                {/* 按照输出处理时确定的模块进行轮询 */
                    ucSlaveNo = ucUPRollingSlaveNo[ucTaskIndex][2 * uiSlaveCount];
                    if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                    {
                        usModTag = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                        if((usModTag == MODTAG_AI) || (usModTag == MODTAG_DI) || (usModTag == MODTAG_OSP))
                        {
                            ucInputRetainSlaveNo[ucTaskIndex][uiSlaveCount] = ucSlaveNo;
                            
                            memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AI_SAFETYDATA_LEN);
                            memset(&ucDiagBuff[0], 0x00, 32);
                            /* 拷贝旧数据 */
                            pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
                            memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                            if(ucPreDataFlg != 0x01)
                            {
                                pucPreDataVoteAddr = NULL;
                            }
                            else
                            {
                                pucPreDataVoteAddr = &ucPreDataExBuf[uiDataVoteBufIndex];
                            }
                            if(ucNextDataFlg != 0x01)
                            {
                                pucNextDataVoteAddr = NULL;
                            }
                            else
                            {
                                pucNextDataVoteAddr = &ucNextDataExBuf[uiDataVoteBufIndex];
                            }
                            /* 数据表决 */
                            DataExChangeVote(ucTaskIndex, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                        pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);

                            usModTag = pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;

                            /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                            memcpy(pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                    pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);


                            switch(usModTag)
                            {
                                case MODTAG_AI:
                                {
                                    uiDataVoteBufIndex = uiDataVoteBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_AI_QDATA_LEN;
                                }
                                break;

                                case MODTAG_DI:
                                {
                                    uiDataVoteBufIndex = uiDataVoteBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_DI_QDATA_LEN;
                                }
                                break;

                                case MODTAG_OSP:
                                {
                                    uiDataVoteBufIndex = uiDataVoteBufIndex + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_OSP_QDATA_LEN;
                                }
                                break;

                                default :
                                {
                                    ;
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        /*输入数据从共享数据区I区拷贝到运算数据区I区*/
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            if(IO_POLL_START == SysGetIOPollFlg(ucTaskIndex))
            {
                if(uiInputTaskFlag[ucTaskIndex] == 0)
                {
                    DataProcessInputDataCopy(ucTaskIndex);
                    uiInputTaskFlag[ucTaskIndex] = 1;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSlaveConfirmInit
*
* 功能描述: 根据配置，初始化从站重连确认状态
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
void DataProcessSlaveConfirmInit(void)
{
    uint32_t uiSlaveCount = 0U;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    if(pstAppend != NULL)
    {
        for(uiSlaveCount = 0; uiSlaveCount < MAX_SLAVE_NUM; uiSlaveCount++)
        {
            pstAppend->stSlaveState[uiSlaveCount].stSlaveDiag.ucOnlineDiag = 
                    pstAppend->stSlaveState[uiSlaveCount].stSlaveDiag.ucOnlineDiag | SLAVE_COMFIRM_MASK;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessInputRetainHandle
*
* 功能描述: 根据轮询列表，将输入数据存入掉电保护区
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
void DataProcessInputRetainHandle(void)
{
    uint32_t uiSlaveCount = 0U;
    uint8_t ucSlaveNo = 0U;
    uint8_t ucTaskIndex= 0U;
    puint8_t pucStartAddr = NULL;
    SysIoModInfo_t* pstUPIoModInfo[MAX_TASK_NUM] = {NULL};
    
    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
    pstUPIoModInfo[TASK_UP1] = SharedGetIoModInfo(TASK_UP1);
    pstUPIoModInfo[TASK_UP2] = SharedGetIoModInfo(TASK_UP2);
    
    if(pucStartAddr != NULL)
    {
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            for(uiSlaveCount = 0U; uiSlaveCount < DATAPROCESS_SLAVE_POLL_NUM_TOTAL; uiSlaveCount++)
            {
                ucSlaveNo = ucInputRetainSlaveNo[ucTaskIndex][uiSlaveCount];
                
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {/*有效从站号*/

                    if((PF_PAUSE != SharedGetPauseFlag(ucTaskIndex)) && (pstUPIoModInfo[ucTaskIndex] != NULL))
                    {
                        SharedSetInputModuleVar(pucStartAddr + pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                                pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                                pstUPIoModInfo[ucTaskIndex]->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                    }
                }
                
                ucInputRetainSlaveNo[ucTaskIndex][uiSlaveCount] = 0;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSlaveSendHandle
*
* 功能描述: 处理单个从站输出数据
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
static void DataProcessSlaveSendHandle(Task_ID_t emTaskID, uint8_t ucSendDest[], uint8_t ucSlaveNo, bool_t bActiveFV, 
                                       bool_t bEnableFlg ,bool_t bDataValid, bool_t bSlaveConfirm)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    puint8_t pucStartAddr = NULL;
    uint8_t ucRawDataBuf[256] = {0};  /* 原始数据缓存 */
    uint8_t ucRawDataDataLen = 0U;
    uint16_t usSafetyDataDataLen = 0U; /* 原始数据缓存 */
    DataProcessSlaveSlotHeader_t stSlotHead;

    /* 获得Q区原始数据 */
    pstSysIoModInfo = SharedGetIoModInfo(emTaskID); /* 获取对应IO表 */
    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q); /* 获取Q区首地址 */
    if((pstSysIoModInfo != NULL) && (pucStartAddr != NULL))
    {
        switch(pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag)
        {
            case MODTAG_AO:
            {/* AO模块数据处理 */
                memcpy(&ucRawDataBuf[0], pucStartAddr + pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, DATAPROCESS_AO_SAFETYDATA_LEN);
                ucRawDataDataLen = DATAPROCESS_AO_SAFETYDATA_LEN;
            }
            break;

            case MODTAG_DO:
            {/* DO模块数据处理 */
                memcpy(&ucRawDataBuf[0], pucStartAddr + pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, DATAPROCESS_DO_SAFETYDATA_LEN);
                ucRawDataDataLen = DATAPROCESS_DO_SAFETYDATA_LEN;
            }
            break;

            case MODTAG_AI:
            {/* AI模块数据处理 */
                ucRawDataDataLen = DATAPROCESS_NO_SAFETYDATA_LEN;
            }
            break;

            case MODTAG_DI:
            {/* DI模块数据处理 */
                ucRawDataDataLen = DATAPROCESS_NO_SAFETYDATA_LEN;
            }
            break;

            case MODTAG_OSP:
            {/* OSP模块数据处理 */
                ucRawDataDataLen = DATAPROCESS_NO_SAFETYDATA_LEN;
            }
            break;

            default:
            break;
        }
    }

    /* 将原始数据填充为安全实时数据 */
    usSafetyDataDataLen = 255U; /* 调用接口时需要输入安全实时数据缓冲区长度 */
    PMIOMakeReq(ucSlaveNo, &ucRawDataBuf[0], ucRawDataDataLen, bActiveFV, bEnableFlg, bDataValid, bSlaveConfirm,\
                &ucSendDest[sizeof(DataProcessSlaveSlotHeader_t)], &usSafetyDataDataLen);

    /* 填slot头 */
    stSlotHead.ucSlotType = DATAPROCESS_SLOT_TYPE_SAFETY;
    stSlotHead.ucSlotLen = (uint8_t)usSafetyDataDataLen;
    memcpy(&ucSendDest[0], &stSlotHead, sizeof(DataProcessSlaveSlotHeader_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSlaveRecvHandle
*
* 功能描述: 处理单个从站输入数据
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
static uint8_t DataProcessSlaveRecvHandle(Task_ID_t emTaskID, uint8_t ucRecvSrc[], uint8_t ucSlaveNo, uint16_t usModTag)
{
    uint8_t  ucRet = DATAPROCESS_RET_FAIL;
    uint8_t ucSlotCount = 0U;
    uint8_t ucActiveState = 0U;
    uint8_t ucSlotLen = 0U;
    uint16_t i = 0U;
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    DataProcessSlaveSlotHeader_t stSlotHead;
    uint16_t usBufIndex = 0U; /*数据地址偏移*/
    SharedAppend_t *pstAppend = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationInfo = NULL;
    
    pstCtrlStationInfo = SharedGetCtrlStationInfo(MAX_TASK_NUM); 
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    
    /* 得到Slot个数 */
    memcpy(&ucSlotCount, &ucRecvSrc[usBufIndex], sizeof(uint8_t));
    usBufIndex += sizeof(uint8_t);

    pstSysIoModInfo = SharedGetIoModInfo(emTaskID); /* 获取对应IO表 */

    if((pstSysIoModInfo != NULL) && (ucSlotCount <= DATAPROCESS_MAX_SLOT_NUM))
    {
        for(i = 0; i < ucSlotCount; i++)
        {
            /* 拷贝Slot头 */
            memcpy(&stSlotHead, &ucRecvSrc[usBufIndex], sizeof(DataProcessSlaveSlotHeader_t));
            usBufIndex += sizeof(DataProcessSlaveSlotHeader_t);

            if(usBufIndex + stSlotHead.ucSlotLen > pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize)
            {
                LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_ERR1);
                break;
            }

            switch(stSlotHead.ucSlotType)
            {
                case DATAPROCESS_SLOT_TYPE_NONSAFETY:
                {/* 非安全数据slot */
                    if(DATAPROCESS_RET_OK == DataProcessRecvNonSafetyDataHandle(&ucRecvSrc[usBufIndex]))
                    {
                        ucRet = DATAPROCESS_RET_OK;
                    }
                    else
                    {/*解析段内容错误，设置段处理失败*/
                        ucRet = DATAPROCESS_RET_FAIL;
                    }
                    usBufIndex += stSlotHead.ucSlotLen;
                }
                break;

                case DATAPROCESS_SLOT_TYPE_SAFETY:
                {/* 安全数据slot */
                    /* 读取主备状态 */
                    memcpy(&ucActiveState, &ucRecvSrc[usBufIndex], sizeof(uint8_t));
                    usBufIndex += sizeof(uint8_t);
                    ucSlotLen = stSlotHead.ucSlotLen - 1; /* 安全数据长度实际长度应减去1 */

                    if(ucActiveState == 85)
                    {
                        /* 处理安全数据 */
                        if(DATAPROCESS_RET_OK == DataProcessRecvSafetyDataHandle(&ucRecvSrc[usBufIndex],
                                pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                ucSlotLen, ucSlaveNo, usModTag))
                        {
                            ucRet = DATAPROCESS_RET_OK;
                        }
                        else
                        {/*解析段内容错误，设置段处理失败*/
                            ucRet = DATAPROCESS_RET_FAIL;
                        }
                        if((pstAppend != NULL) && (pstCtrlStationInfo != NULL))
                        {
                            if(0x01 == pstCtrlStationInfo->stStationConfigInfo.ucSlaveConfirm)
                            {/*属性为需要进行断线重连确认*/
                                if((pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag & SLAVE_COMFIRM_MASK) == 0)
                                {/*为主的从站未被确认*/
                                    memset(&ucSlaveQuality[ucSlaveNo/2][0], 0, 4); /* 清临时诊断区 */
                                }
                            }
                        }
                    }

                    usBufIndex += ucSlotLen;
                }
                break;

                case DATAPROCESS_SLOT_TYPE_DIAG:
                {/* 诊断数据slot */
                    if(DATAPROCESS_RET_OK == DataProcessRecvDiagDataHandle(&ucRecvSrc[usBufIndex], stSlotHead.ucSlotLen, ucSlaveNo))
                    {
                        ucRet = DATAPROCESS_RET_OK;
                    }
                    else
                    {/*解析段内容错误，设置段处理失败*/
                        ucRet = DATAPROCESS_RET_FAIL;
                    }
                    usBufIndex += stSlotHead.ucSlotLen;
                }
                break;

                case DATAPROCESS_SLOT_TYPE_SOE:
                {/* SOE数据slot */
                    if(stSlotHead.ucSlotLen % sizeof(DataProcessHardSOEData_t) == 0)
                    {
                        DataProcessHardSOEHandle(&ucRecvSrc[usBufIndex], stSlotHead.ucSlotLen / sizeof(DataProcessHardSOEData_t), ucSlaveNo,usModTag);
                        ucRet = DATAPROCESS_RET_OK;
                    }
                    usBufIndex += stSlotHead.ucSlotLen;
                }
                break;

                case DATAPROCESS_SLOT_TYPE_OSP:
                {/* OSP监控slot */
                    if(DATAPROCESS_RET_OK == DataProcessRecvOSPDataHandle(&ucRecvSrc[usBufIndex], stSlotHead.ucSlotLen, ucSlaveNo))
                    {
                        ucRet = DATAPROCESS_RET_OK;
                    }
                    else
                    {/*解析段内容错误，设置段处理失败*/
                        ucRet = DATAPROCESS_RET_FAIL;
                    }
                    usBufIndex += stSlotHead.ucSlotLen;
                }
                break;

                default :
                {
                    LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_ERR2);
                    ucRet = DATAPROCESS_RET_FAIL;
                }
                break;
            }
        }
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessInputDataCopy
*
* 功能描述: 输入数据从共享数据区I区拷贝到任务实时使用的数据区I区
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
static uint8_t DataProcessInputDataCopy(uint32_t uiTaskID)
{
    uint8_t  ucRet = DATAPROCESS_COPY_OK;
    puint8_t pucMemStartAddr = NULL;
    puint8_t pucPrgStartAddr = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    pucMemStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_G);
    pucPrgStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);

    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(uiTaskID);
    if (pstCtrlStationConfigInfo == NULL)
    {
        return DATAPROCESS_COPY_NOTOK;
    }

    memcpy(pucPrgStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiIOffset,
            pucMemStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiIOffset,
            pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiIUsedSize);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessOutputDataCopy
*
* 功能描述: 输出数据从运算数据区Q区拷贝到共享数据区Q区
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
uint8_t DataProcessOutputDataCopy(uint32_t uiTaskID)
{
    uint8_t  ucRet = DATAPROCESS_COPY_OK;
    puint8_t pStartAddr = NULL;
    puint8_t pucPrgStartAddr = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    pStartAddr = &(g_stBackupRtData.ucG[0]);
    pucPrgStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);

    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo(uiTaskID);
    if (pstCtrlStationConfigInfo == NULL)
    {
        ucRet = DATAPROCESS_COPY_NOTOK;
    }
    else
    {
        memcpy(pStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiOOffset,
                pucPrgStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiOOffset,
                pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiOUsedSize);
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessCheckDownloadIOConfigFinish
*
* 功能描述: 检查全下装硬件配置是否完成
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
static uint32_t DataProcessCheckDownloadIOConfigFinish(void)
{

    uint8_t ucRet = IPBUS_DEVICE_NOCONFIG_SLAVE;

    ucRet = IPBUSGetDeviceConfigSlaveFlag();

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessRecvNonSafetyDataHandle
*
* 功能描述: 处理非安全实时数据
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
static uint8_t DataProcessRecvNonSafetyDataHandle(uint8_t ucRecvSrc[])
{
    uint8_t ucRet = DATAPROCESS_RET_OK;

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessRecvSafetyDataHandle
*
* 功能描述: 处理安全实时数据
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
static uint8_t DataProcessRecvSafetyDataHandle(uint8_t ucRecvSrc[], uint32_t uiIecOffset, uint8_t ucSlotLen, uint8_t ucSlaveNo, uint16_t usModTag)
{
    uint8_t  ucRet = DATAPROCESS_RET_OK;
    puint8_t pucStartAddr = NULL;
    bool_t bResult = false;
    uint8_t ucSafetyDataBuf[256] = {0};  /* 安全数据缓存 */
    uint8_t ucSafetyDataLen = 255U;
    bool_t bParOK = false;

    /* 安全实时数据的安全层处理 */
    bResult = PMIODecodeResp(ucSlaveNo, &ucRecvSrc[0], ucSlotLen, &ucSafetyDataBuf[0], &ucSafetyDataLen, &bParOK);
    if(bResult == true)
    {
        if(true == bParOK)
        {/*当在线下装，接收到IO已经切换参数OK标志，清空本地记录使能标志*/
            DataProcessSetModEnableFlg((ucSlaveNo / 2) * 2, false);
            DataProcessSetModEnableFlg(((ucSlaveNo / 2) * 2) + 1, false);
        }

        /* 安全实时数据拷贝至 */
        pucStartAddr = &(g_stBackupRtData.ucI[0]);
        switch(usModTag)
        {
            case MODTAG_AI:
            {
                /* AI模块数据处理 */
                memcpy(pucStartAddr + uiIecOffset, &ucSafetyDataBuf[0], DATAPROCESS_AI_SAFETYDATA_LEN);
                memcpy(&ucSlaveQuality[ucSlaveNo / 2][0], &ucSafetyDataBuf[DATAPROCESS_AI_SAFETYDATA_LEN], DATAPROCESS_AI_QDATA_LEN);
            }
            break;

            case MODTAG_DI:
            {
                /* DI模块数据处理 */
                memcpy(pucStartAddr + uiIecOffset, &ucSafetyDataBuf[0], DATAPROCESS_DI_SAFETYDATA_LEN);
                memcpy(&ucSlaveQuality[ucSlaveNo / 2][0], &ucSafetyDataBuf[DATAPROCESS_DI_SAFETYDATA_LEN], DATAPROCESS_DI_QDATA_LEN);
            }
            break;

            case MODTAG_OSP:
            {
                /* OSP模块数据处理 */
                memcpy(pucStartAddr + uiIecOffset, &ucSafetyDataBuf[0], DATAPROCESS_OSP_SAFETYDATA_LEN);
                memcpy(&ucSlaveQuality[ucSlaveNo / 2][0], &ucSafetyDataBuf[DATAPROCESS_OSP_SAFETYDATA_LEN], DATAPROCESS_OSP_QDATA_LEN);
            }
            break;

            default:
            break;
        }
    }
    else
    {
        LogWrite(LOG0_ID_APP_DATA_PROCESS_LEVEL0_SAFETYDATA_ERR);
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessRecvOSPDataHandle
*
* 功能描述: 处理诊断数据
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
static uint8_t DataProcessRecvOSPDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo)
{
    uint8_t ucRet = DATAPROCESS_RET_FAIL;
    SysOSPMonitorData_t* pstSysOSPMonitorData = NULL;
    uint16_t usBufIndex = 0U; /*数据地址偏移*/
    uint8_t ucOSPFunctionType = 0;
    DataProcessOSPMonitorData_t stOSPMonitorData_t;

    pstSysOSPMonitorData = SysGetOSPMonitorData(ucSlaveNo);
    if(pstSysOSPMonitorData != NULL)
    {
        /* 得到OSP功能类型 */
        memcpy(&ucOSPFunctionType, &ucRecvSrc[usBufIndex], sizeof(uint8_t));
        usBufIndex += sizeof(uint8_t);

        if(ucOSPFunctionType == DATAPROCESS_OSP_OSPFUNCTION)
        {
            memcpy(&stOSPMonitorData_t, &ucRecvSrc[usBufIndex], sizeof(DataProcessOSPMonitorData_t));
            pstSysOSPMonitorData->uiOSPFunction = DATAPROCESS_OSP_OSPFUNCTION;
            pstSysOSPMonitorData->fPRPMA = stOSPMonitorData_t.fPRPMA;
            pstSysOSPMonitorData->fPRPMB = stOSPMonitorData_t.fPRPMB;
            pstSysOSPMonitorData->fPRPMC = stOSPMonitorData_t.fPRPMC;
            pstSysOSPMonitorData->fPAccelA = stOSPMonitorData_t.fPAccelA;
            pstSysOSPMonitorData->fPAccelB = stOSPMonitorData_t.fPAccelB;
            pstSysOSPMonitorData->fPAccelC = stOSPMonitorData_t.fPAccelC;
            pstSysOSPMonitorData->uiAlarmPackA = stOSPMonitorData_t.uiAlarmPackA;
            pstSysOSPMonitorData->uiAlarmPackB = stOSPMonitorData_t.uiAlarmPackB;
            pstSysOSPMonitorData->uiAlarmPackC = stOSPMonitorData_t.uiAlarmPackC;
            pstSysOSPMonitorData->ucDigPackA = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackA;
            pstSysOSPMonitorData->ucDigPackB = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackB;
            pstSysOSPMonitorData->ucDigPackC = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackC;

            ucRet = DATAPROCESS_RET_OK;
        }
        else if(ucOSPFunctionType == DATAPROCESS_OSP_DIRFUNCTION)
        {
            memcpy(&(stOSPMonitorData_t.OSPMonitorPackData), &ucRecvSrc[usBufIndex], sizeof(DataProcessOSPMonitorPackData_t));
            pstSysOSPMonitorData->uiOSPFunction = DATAPROCESS_OSP_DIRFUNCTION;
            pstSysOSPMonitorData->fPRPMA = 0;
            pstSysOSPMonitorData->fPRPMB = 0;
            pstSysOSPMonitorData->fPRPMC = 0;
            pstSysOSPMonitorData->fPAccelA = 0;
            pstSysOSPMonitorData->fPAccelB = 0;
            pstSysOSPMonitorData->fPAccelC = 0;
            pstSysOSPMonitorData->uiAlarmPackA = 0;
            pstSysOSPMonitorData->uiAlarmPackB = 0;
            pstSysOSPMonitorData->uiAlarmPackC = 0;
            pstSysOSPMonitorData->ucDigPackA = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackA;
            pstSysOSPMonitorData->ucDigPackB = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackB;
            pstSysOSPMonitorData->ucDigPackC = stOSPMonitorData_t.OSPMonitorPackData.ucDigPackC;

            ucRet = DATAPROCESS_RET_OK;
        }
        else if(ucOSPFunctionType == DATAPROCESS_OSP_NOFUNCTION)
        {
            pstSysOSPMonitorData->uiOSPFunction = DATAPROCESS_OSP_NOFUNCTION;
            pstSysOSPMonitorData->fPRPMA = 0;
            pstSysOSPMonitorData->fPRPMB = 0;
            pstSysOSPMonitorData->fPRPMC = 0;
            pstSysOSPMonitorData->fPAccelA = 0;
            pstSysOSPMonitorData->fPAccelB = 0;
            pstSysOSPMonitorData->fPAccelC = 0;
            pstSysOSPMonitorData->uiAlarmPackA = 0;
            pstSysOSPMonitorData->uiAlarmPackB = 0;
            pstSysOSPMonitorData->uiAlarmPackC = 0;
            pstSysOSPMonitorData->ucDigPackA = 0;
            pstSysOSPMonitorData->ucDigPackB = 0;
            pstSysOSPMonitorData->ucDigPackC = 0;
            ucRet = DATAPROCESS_RET_OK;
        }
        else
        {
            ucRet = DATAPROCESS_RET_FAIL;
        }
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSOEDataHandle
*
* 功能描述: 处理SOE数据
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
static void DataProcessHardSOEHandle(uint8_t ucRecvSrc[], uint8_t ucSOENum, uint8_t ucSlaveNo,uint16_t usModTag)
{
    uint8_t i = 0U;
    HardSOE_t stSOE;
    DataProcessHardSOEData_t stSlaveSOEData;
    uint16_t usBufIndex = 0U; /* 数据地址偏移 */
    uint8_t ucModType = 0U;

    if(MODTAG_AI == usModTag)
    {
        ucModType = 1;
    }
    if((MODTAG_DI == usModTag) || (MODTAG_OSP == usModTag))
    {
        ucModType = 0;
    }

    for(i = 0;i < ucSOENum; i++)
    {
        /* 拷贝一条SOE原始数据 */
        memcpy(&stSlaveSOEData, &ucRecvSrc[usBufIndex], sizeof(DataProcessHardSOEData_t));
        /* 填充一条SOE记录 */
        stSOE.ucModNo = ucSlaveNo;
        stSOE.ucChannelNo = stSlaveSOEData.ucChannelNo;
        stSOE.ucValue = stSlaveSOEData.ucEventState;
        stSOE.ucReserved = ucModType;

        /* 填写整分钟时的秒数 */
        stSOE.uiSec = stSOESysTime.uiSec - (stSOESysTime.uiSec % TIME_MIN_TO_SEC);
        /* 修正SOE时间 */
        if(stSlaveSOEData.uiSOETick > TIME_MIN_TO_MSEC +1)
        {
            stSlaveSOEData.uiSOETick -= TIME_MIN_TO_MSEC;
        }
        /* 填写综合后的秒数及微秒数 */
        stSOE.uiSec += stSlaveSOEData.uiSOETick / TIME_SEC_TO_MSEC;
        stSOE.uiMicroSec = stSlaveSOEData.uiSOETick % TIME_SEC_TO_MSEC;

        /* 修正分钟时间 */
        if(stSOE.uiSec >= stSOESysTime.uiSec)
        {/* 大于10s，回退1分钟 */
            if((stSOE.uiSec - stSOESysTime.uiSec) >= TIME_SEC_DIFF_PRECISON)
            {
                stSOE.uiSec = stSOE.uiSec - TIME_MIN_TO_SEC;
            }
        }
        else
        {/* 前溯1分钟 */
            if((stSOESysTime.uiSec - stSOE.uiSec) >= TIME_SEC_DIFF_PRECISON)
            {
                stSOE.uiSec = stSOE.uiSec + TIME_MIN_TO_SEC;
            }
        }

        /* 填SOE队列 */
        if(stSOE.uiSec < 0xF0000000)
        {
            SharedSetHardSOE(stSOE);
        }

        /* 移动数据地址偏移 */
        usBufIndex = usBufIndex + sizeof(DataProcessHardSOEData_t);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessRecvDiagDataHandle
*
* 功能描述: 处理OSP监控数据
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
static uint8_t DataProcessRecvDiagDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo)
{
    uint8_t ucRet = DATAPROCESS_RET_FAIL;
    uint8_t ucChannelDiagNum = 0;
    uint8_t ucChannelNo = 0;
    uint8_t i = 0;
    DataProcessStateDiagData_t stRecvDiagData;
    DataProcessChannelDiagData_t stChannelDiagData;
    uint16_t usBufIndex = 0U; /* 数据地址偏移 */
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    /* 获取状态、温度、设备诊断，并填写append区 */
    memcpy(&stRecvDiagData, &ucRecvSrc[usBufIndex], sizeof(DataProcessStateDiagData_t));
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucStateDiag = stRecvDiagData.ucStateDiag;
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.cTempDiag = stRecvDiagData.cTempDiag;
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.DeviceDiag[0] = stRecvDiagData.uiDeviceDiag[1];
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.DeviceDiag[1] = stRecvDiagData.uiDeviceDiag[2];
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.DeviceDiag[2] = stRecvDiagData.uiDeviceDiag[3];

    usBufIndex = usBufIndex + sizeof(DataProcessStateDiagData_t);

    if((ucSlotLen - sizeof(DataProcessStateDiagData_t)) % sizeof(DataProcessChannelDiagData_t) != 0)
    {
        ucRet = DATAPROCESS_RET_FAIL;
    }
    else
    {
        /* 拷贝一条通道诊断原始数据 */
        ucChannelDiagNum = (ucSlotLen - sizeof(DataProcessStateDiagData_t)) / sizeof(DataProcessChannelDiagData_t);

        for(i = 0;i < ucChannelDiagNum; i++)
        {
            /* 拷贝一条通道诊断原始数据 */
            memcpy(&stChannelDiagData, &ucRecvSrc[usBufIndex], sizeof(DataProcessChannelDiagData_t));
            ucChannelNo = stChannelDiagData.ucChannelNo;

            if((ucChannelNo <= 31) && (ucChannelNo >= 0))
            {
                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ChannelDiag[ucChannelNo][0] = stChannelDiagData.ucChannelDiag[0];
                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ChannelDiag[ucChannelNo][1] = stChannelDiagData.ucChannelDiag[1];
                pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ChannelDiag[ucChannelNo][2] = stChannelDiagData.ucChannelDiag[2];
            }

            /* 移动数据地址偏移 */
            usBufIndex = usBufIndex + sizeof(DataProcessChannelDiagData_t);
        }

        ucRet = DATAPROCESS_RET_OK;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSetIPBUSLight
*
* 功能描述: 根据从站在线状态设置IPBUS灯状态，当从站全不在线时，IPBUS灯状态为灭
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
static void DataProcessSetIPBUSLight(void)
{
    uint32_t uiSlaveCount = 0U;
    uint32_t uiTaskCount = 0U;
    uint8_t ucSlaveOnlineCount = 0U;
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint8_t ucSlaveNo = 0U;
    SharedAppend_t *pstAppend = NULL;

    pstSysIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    if (pstSysIoModInfo != NULL)
    {
        /* 轮询所有的从站，判断在线状态，只要有一个模块在线，IPBUS灯闪，反之则灭 */
        for(uiTaskCount = 0; uiTaskCount < MAX_TASK_NUM; uiTaskCount++)
        {
            for(uiSlaveCount = 0; uiSlaveCount < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[uiTaskCount]; uiSlaveCount++)
            {
                ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[uiTaskCount][uiSlaveCount];
                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                {
                    ucSlaveOnlineCount = ucSlaveOnlineCount + 1;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag = pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag | SLAVE_ONLINE_MASK;
                }
                else
                {
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag = pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag & (~SLAVE_ONLINE_MASK);
                }
            }
            for(uiSlaveCount = 0; uiSlaveCount < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[uiTaskCount]; uiSlaveCount++)
            {
                ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[uiTaskCount][uiSlaveCount];
                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                {
                    ucSlaveOnlineCount = ucSlaveOnlineCount + 1;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag = pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag | SLAVE_ONLINE_MASK;
                }
                else
                {
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag = pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucOnlineDiag & (~SLAVE_ONLINE_MASK);
                }
            }
        }
        if(ucSlaveOnlineCount > 0U)
        {
            SetLEDState(LED_NAME_IPBUS, LED_BLINK);
            pstAppend->stPMState.stPMFirmwareState.ucIPBUSState = 1;
        }
        else
        {
            SetLEDState(LED_NAME_IPBUS, LED_OFF);
            pstAppend->stPMState.stPMFirmwareState.ucIPBUSState = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSoftSOEHandle
*
* 功能描述: 处理软SOE数据
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
void DataProcessSoftSOEHandle(Task_ID_t uiTaskID)
{
    uint8_t i = 0U;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    uint8_t ucSOEBlockNo = 0U;
    SharedAppend_t *pstAppend = NULL;

    pstSOEConfigInfo = SharedGetSOEConfigInfo(uiTaskID); /* 获取SOE表 */
    if(pstSOEConfigInfo != NULL)
    {
        for(i = 0;i < pstSOEConfigInfo->ucSoftSOEBlockNum[(uint8_t)uiTaskID]; i++)
        {
            ucSOEBlockNo = pstSOEConfigInfo->ucSoftSOEBlockTable[(uint8_t)uiTaskID][i];
            pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
            if(pstAppend->ucSOECtrl[ucSOEBlockNo] == SOE_STATE_START)
            {/* 块状态为启动 */
                DataProcessGenerateSoftSOERecord(uiTaskID,ucSOEBlockNo);
            }
        }
    }

    return;
}



/*
***********************************************************************************************************************
* 函数名称: DataprocessSetPollNum
*
* 功能描述: 设置每周期IO轮询个数，填写append区pollingTime
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
static void DataprocessSetPollNum(uint8_t *pucUP1HandleNum, uint8_t *pucUP2HandleNum)
{
    SysIoModInfo_t* pstUPIoModInfo[MAX_TASK_NUM] = {NULL};
    uint32_t uiSlaveNum[MAX_TASK_NUM] = {0}; /* 每个任务所组IO模块的对数 */
    SharedAppend_t *pstAppend = NULL;


    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    pstUPIoModInfo[TASK_UP1] = SharedGetIoModInfo(TASK_UP1);
    pstUPIoModInfo[TASK_UP2] = SharedGetIoModInfo(TASK_UP2);
    uiSlaveNum[TASK_UP1] = (pstUPIoModInfo[TASK_UP1]->stSysIoSlaveInfo.usInDevNum[TASK_UP1] + pstUPIoModInfo[TASK_UP1]->stSysIoSlaveInfo.usOutDevNum[TASK_UP1]) / 2;
    uiSlaveNum[TASK_UP2] = (pstUPIoModInfo[TASK_UP2]->stSysIoSlaveInfo.usInDevNum[TASK_UP2] + pstUPIoModInfo[TASK_UP2]->stSysIoSlaveInfo.usOutDevNum[TASK_UP2]) / 2;

    /* 当没有任务2的IO时，所有的IO轮询给任务1 */
    if(NOT_ACTIVE == SharedGetTaskActive(TASK_UP2))
    {
        *pucUP1HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TOTAL;
        *pucUP2HandleNum = 0;

        if((uiSlaveNum[TASK_UP1] % (*pucUP1HandleNum)) == 0)
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP1].uiUPPollingTime = (uiSlaveNum[TASK_UP1] / (*pucUP1HandleNum)) * 5000;
        }
        else
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP1].uiUPPollingTime = (uiSlaveNum[TASK_UP1] / (*pucUP1HandleNum) + 1) * 5000;
        }

        /* 没有任务2时，PollingTime为0 */
        pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP2].uiUPPollingTime = 0;
    }
    else
    {
        *pucUP1HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TASK1;
        *pucUP2HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TASK2;

        if((uiSlaveNum[TASK_UP1] % (*pucUP1HandleNum)) == 0)
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP1].uiUPPollingTime = (uiSlaveNum[TASK_UP1] / (*pucUP1HandleNum)) * 5000;
        }
        else
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP1].uiUPPollingTime = (uiSlaveNum[TASK_UP1] / (*pucUP1HandleNum) + 1) * 5000;
        }

        if((uiSlaveNum[TASK_UP2] % (*pucUP2HandleNum)) == 0)
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP2].uiUPPollingTime = (uiSlaveNum[TASK_UP2] / (*pucUP2HandleNum)) * 5000;
        }
        else
        {
            pstAppend->stPMState.stPMFirmwareState.stSharedUPState[TASK_UP2].uiUPPollingTime = (uiSlaveNum[TASK_UP2] / (*pucUP2HandleNum) + 1) * 5000;
        }
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


