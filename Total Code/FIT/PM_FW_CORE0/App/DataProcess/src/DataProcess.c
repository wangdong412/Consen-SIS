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
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/SysResource/inc/SysCore0RtData.h"
#include "Srv/Sys/inc/CommSysConfig.h"
#include "Srv/Sys/inc/SysHardware.h"
#include "Srv/SysResource/inc/SysMonitor.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/SharedMem/inc/SharedConfig.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SharedMem/inc/SharedSOE.h"
#include "Srv/IPBUS/inc/IPBUS.h"
#include "Srv/SafetyProtocol/inc/PMIOSafetyComm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t ucDataProcessSOELastVar[MAX_SOE_VAR_NUM]; /* 软SOE上一拍值 */
static SharedRTDataArea_t g_stBackupRtData;

static uint8_t ucUP1RollingSlaveNo[DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2]; /* 记录每周期UP1轮询的从站号 */
static uint8_t ucUP2RollingSlaveNo[DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2]; /* 记录每周期UP2轮询的从站号 */

static uint32_t uiUP1RollingSlaveNum = 0U;
static uint32_t uiUP2RollingSlaveNum = 0U;
static uint8_t ucSlaveQuality[IPBUS_MODULE_MAX_NUM / 2][4]; /* 临时用于存放IO质量*/

/* 标志位，用于输入拷贝只执行一次 */
static uint32_t uiInputTask1Flag = 0U;
static uint32_t uiInputTask2Flag = 0U;

static bool_t s_bModEnableFlg[MAX_MOD_NUM];
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

//static void DataProcessFillSlaveRollingListTable(void);

static void DataProcessSlaveSendHandle(Task_ID_t emTaskID, uint8_t ucSendDest[], uint8_t ucSlaveNo, bool_t bActiveFV, bool_t bEnableFlg);

static uint8_t DataProcessSlaveRecvHandle(Task_ID_t emTaskID, uint8_t ucRecvSrc[], uint8_t ucSlaveNo, uint16_t usModTag);

static uint32_t DataProcessCheckDownloadIOConfigFinish(void);

static uint8_t DataProcessInputDataCopy(uint32_t uiTaskID);

static uint8_t DataProcessRecvNonSafetyDataHandle(uint8_t ucRecvSrc[]);

static uint8_t DataProcessRecvSafetyDataHandle(uint8_t ucRecvSrc[], uint32_t uiIecOffset, uint8_t ucSlotLen, uint8_t ucSlaveNo, uint16_t usModTag);

static uint8_t DataProcessRecvDiagDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo);

static void DataProcessHardSOEHandle(uint8_t ucRecvSrc[], uint8_t ucSOENum, uint8_t ucSlaveNo,uint16_t usModTag);

static uint8_t DataProcessRecvOSPDataHandle(uint8_t ucRecvSrc[], uint8_t ucSlotLen, uint8_t ucSlaveNo);

static void DataProcessSetIPBUSLight(void);

static void DataProcessGenerateSoftSOERecord(Task_ID_t uiTaskID,uint8_t ucSOEBlockNo);

static bool_t DataProcessGetModEnableFlg(uint8_t ucModAddr);

static void DataProcessSetModEnableFlg(uint8_t ucModAddr, bool_t bModEnableFlg);

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
    
    memset(&ucDataProcessSOELastVar[0], 0x00, sizeof(uint8_t)*MAX_SOE_VAR_NUM);

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
    uint8_t ucUP1HandleNum = 0U;
    uint8_t ucUP2HandleNum = 0U;
    uint32_t uiSlaveCount = 0U;
    uint8_t ucOutputBuf[DATAPROCESS_OUTPUT_MAX_LEN] = {0}; /* 数据区buf */
    bool_t bActiveFV = false;
    bool_t bEnableFlg = false;
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
    uint32_t uiUP1RollingListBufIndex = 0U;
    uint32_t uiUP2RollingListBufIndex = 0U;
    puint8_t pucStartAddr = NULL;
    SysIoModInfo_t* pstUP1IoModInfo = NULL;
    SysIoModInfo_t* pstUP2IoModInfo = NULL;
    uint8_t ucSlaveNo = 0U;

    /* 标志位，用于控制IO轮询的计数 */
    static uint32_t uiTask1SlaveCountIndex = 0U;
    static uint32_t uiTask2SlaveCountIndex = 0U;

    /* 点IPBUS灯，和轮询周期无关，每周期判断 */
    DataProcessSetIPBUSLight();

    /* 清除记录每周期轮询的从站号 */
    memset(&ucUP1RollingSlaveNo[0], 0, DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2);
    memset(&ucUP2RollingSlaveNo[0], 0, DATAPROCESS_SLAVE_POLL_NUM_TOTAL * 2);

    /* 获取对应IO表 */
    pstUP1IoModInfo = SharedGetIoModInfo(TASK_UP1);
    pstUP2IoModInfo = SharedGetIoModInfo(TASK_UP2);

    if((DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_CONFIG_SLAVE) && (pstUP1IoModInfo != NULL) && (pstUP2IoModInfo != NULL))
    {
        /* 当没有任务2的IO时，所有的IO轮询给任务1 */
        if(NOT_ACTIVE == SharedGetTaskActive(TASK_UP2))
        {
            ucUP1HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TOTAL;
            ucUP2HandleNum = 0;
        }
        else
        {
            ucUP1HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TASK1;
            ucUP2HandleNum = DATAPROCESS_SLAVE_POLL_NUM_TASK2;
        }
        /* 清输入拷贝标志 */
        /* UP1处理 */
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
        {
            uiInputTask1Flag = 0; /* 清输入拷贝标志 */
        }
        /* UP2处理 */
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
        {
            uiInputTask2Flag = 0; /* 清输入拷贝标志 */
        }

        /* 输出数据交换及表决 */
        /* UP1处理 */
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
        {
            if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(TASK_UP1))
            {/* 已经经过一次IEC运算，可以进行输出数据处理 */
                /* Q区交换，每周期处理固定数量*/
                for(i = 0; i < ucUP1HandleNum; i++)
                {
                    /* 处理输入模块的输出数据，每次处理一对模块 */
                    if((uiTask1SlaveCountIndex + 2 * i) < pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1])
                    {/* 输出模块 */
                        uiSlaveCount = uiTask1SlaveCountIndex + 2 * i;
                        ucSlaveNo = pstUP1IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP1][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
                            pucStartAddr = &(g_stBackupRtData.ucQ[0]);
                            memcpy(&ucDataExBuf[uiDataExBufIndex],
                                    pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                            uiDataExBufIndex = uiDataExBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        }
                    }
                    else
                    {/* 非输出模块 */
                        break;
                    }
                }
            }
        }
        /* UP2处理 */
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
        {
            if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(TASK_UP2))
            {/* 已经经过一次IEC运算，可以进行输出数据处理 */
                /* Q区交换，每周期处理固定数量*/
                for(i = 0; i < ucUP2HandleNum; i++)
                {
                    /* 处理输入模块的输出数据，每次处理一对模块 */
                    if((uiTask2SlaveCountIndex + 2 * i) < pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2])
                    {/* 输出模块 */
                        uiSlaveCount = uiTask2SlaveCountIndex + 2 * i;
                        ucSlaveNo = pstUP2IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP2][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
                            pucStartAddr = &(g_stBackupRtData.ucQ[0]);
                            memcpy(&ucDataExBuf[uiDataExBufIndex],
                                    pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                    pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                            uiDataExBufIndex = uiDataExBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        }
                    }
                    else
                    {/* 非输出模块 */
                        break;
                    }
                }
            }
        }
        /* UP1、UP2共同打包的数据进行交换 */
        if(uiDataExBufIndex > 0)
        {/* 有Q区数据需要交换 */
            /* 交换Q区数据 */
            DataExChange(&ucDataExBuf[0], (uint16_t)uiDataExBufIndex, &ucPreDataExBuf[0], &ucNextDataExBuf[0], &ucPreDataFlg, &ucNextDataFlg);
        }
        /* UP1、UP2共同打包的数据进行表决 */
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
        {
            if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(TASK_UP1))
            {
                /* UP1按模块依次表决 */
                for(i = 0; i < ucUP1HandleNum; i++)
                {
                    if((uiTask1SlaveCountIndex + 2 * i) < pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1])
                    {
                        memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AO_SAFETYDATA_LEN);
                        memset(&ucDiagBuff[0], 0x00, 32);
                        uiSlaveCount = uiTask1SlaveCountIndex + 2 * i;
                        ucSlaveNo = pstUP1IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP1][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
                            /* 拷贝旧数据 */
                            pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);

                            memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

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
                            DataExChangeVote(TASK_UP1, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                    pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);
                            /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                            memcpy(pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                            uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        }
                    }
                }
            }
        }
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
        {
            if (OUTPUT_POLL_ENABLE == SysGetOutputPollFlg(TASK_UP2))
            {
                /* UP2按模块依次表决 */
                for(i = 0; i < ucUP2HandleNum; i++)
                {
                    if((uiTask2SlaveCountIndex + 2 * i) < pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2])
                    {
                        memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AO_SAFETYDATA_LEN);
                        memset(&ucDiagBuff[0], 0x00, 32);
                        uiSlaveCount = uiTask2SlaveCountIndex + 2 * i;
                        ucSlaveNo = pstUP2IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP2][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
                            /* 拷贝旧数据 */
                            pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);

                            memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

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
                            DataExChangeVote(TASK_UP2, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                    pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);

                            /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                            memcpy(pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                            uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        }
                    }
                }
            }
        }

        /* 生成输出数据包，并填充主卡RAM */
        if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
        {/* 获取权限，向双口拷贝数据 */
//            printf("Q\n");
            /* UP1处理 */
            if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
            {
                for(i = 0; i < 2 * ucUP1HandleNum; i++)
                {
                    /* 每次处理单个模块 */
                    uiSlaveCount = uiTask1SlaveCountIndex + i;
                    if(uiSlaveCount < (uint32_t)pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1])
                    {
                        ucSlaveNo = pstUP1IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP1][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
//                            printf("%d\n", ucSlaveNo);
                            if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                            {
//                                printf("V\n");
                                memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                /* 输出数据生成 */
                                if (OUTPUT_POLL_DISENABLE == SysGetOutputPollFlg(TASK_UP1))
                                {
                                    bActiveFV = true;
                                }
                                else
                                {
                                    bActiveFV = false;
                                }
                                /*设置从站使能标志*/
                                if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(TASK_UP1))
                                {
                                    if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                    {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                        DataProcessSetModEnableFlg(ucSlaveNo, true);
                                    }
                                }
                                bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);
                                
                                DataProcessSlaveSendHandle(TASK_UP1,&ucOutputBuf[0], ucSlaveNo, bActiveFV, bEnableFlg);

                                /* 通过PCIE从双口RAM填写输出数据 */
                                ipbus_write(WRITE_IPBUS_OUTPUT,
                                            &ucOutputBuf[0],
                                            pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                            pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                            }
                            else
                            {
//                                printf("N\n");
                            }
                        }
                        ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                        ucUP1RollingSlaveNo[uiUP1RollingListBufIndex] = ucSlaveNo;
                        uiRollingListBufIndex = uiRollingListBufIndex + 1;
                        uiUP1RollingListBufIndex = uiUP1RollingListBufIndex + 1;
                        uiUP1RollingSlaveNum = uiUP1RollingListBufIndex;
                    }
                    else if((uiSlaveCount - (uint32_t)pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1]) < pstUP1IoModInfo->stSysIoSlaveInfo.usInDevNum[TASK_UP1])
                    {
                        uiSlaveCount = uiSlaveCount - (uint32_t)pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1];
                        ucSlaveNo = pstUP1IoModInfo->stSysIoSlaveInfo.ucInDevTabl[TASK_UP1][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
//                            printf("%d\n", ucSlaveNo);
                            if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                            {
//                                printf("V\n");
                                memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                /* 输出数据生成 */
                                bActiveFV = false;
                                /*设置从站使能标志*/
                                if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(TASK_UP1))
                                {
                                    if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                    {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                        DataProcessSetModEnableFlg(ucSlaveNo, true);
                                    }
                                }
                                bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);
                                
                                DataProcessSlaveSendHandle(TASK_UP1,&ucOutputBuf[0], ucSlaveNo, bActiveFV, bEnableFlg);

                                /* 通过PCIE从双口RAM填写输出数据 */
                                ipbus_write(WRITE_IPBUS_OUTPUT,
                                        &ucOutputBuf[0],
                                        pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                        pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                            }
                            else
                            {
//                                printf("N\n");
                            }
                        }

                        ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                        ucUP1RollingSlaveNo[uiUP1RollingListBufIndex] = ucSlaveNo;
                        uiRollingListBufIndex = uiRollingListBufIndex + 1;
                        uiUP1RollingListBufIndex = uiUP1RollingListBufIndex + 1;
                        uiUP1RollingSlaveNum = uiUP1RollingListBufIndex;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            /* UP2处理 */
            if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
            {
                for(i = 0; i < 2 * ucUP2HandleNum; i++)
                {
                    /* 每次处理单个模块 */
                    uiSlaveCount = uiTask2SlaveCountIndex + i;
                    if(uiSlaveCount < (uint32_t)pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2])
                    {
                        ucSlaveNo = pstUP2IoModInfo->stSysIoSlaveInfo.ucOutDevTabl[TASK_UP2][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
//                            printf("%d\n", ucSlaveNo);
                            if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                            {
//                                printf("V\n");
                                memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                /* 输出数据生成 */
                                if (OUTPUT_POLL_DISENABLE == SysGetOutputPollFlg(TASK_UP2))
                                {
                                    bActiveFV = true;
                                }
                                else
                                {
                                    bActiveFV = false;
                                }
                                /*设置从站使能标志*/
                                if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(TASK_UP2))
                                {
                                    if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                    {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                        DataProcessSetModEnableFlg(ucSlaveNo, true);
                                    }
                                }
                                bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);
                                
                                DataProcessSlaveSendHandle(TASK_UP2,&ucOutputBuf[0], ucSlaveNo, bActiveFV, bEnableFlg);

                                /* 通过PCIE从双口RAM填写输出数据 */
                                ipbus_write(WRITE_IPBUS_OUTPUT,
                                        &ucOutputBuf[0],
                                        pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                        pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                            }
                            else
                            {
//                                printf("N\n");
                            }
                        }

                        ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                        ucUP2RollingSlaveNo[uiUP2RollingListBufIndex] = ucSlaveNo;
                        uiRollingListBufIndex = uiRollingListBufIndex + 1;
                        uiUP2RollingListBufIndex = uiUP2RollingListBufIndex + 1;
                        uiUP2RollingSlaveNum = uiUP2RollingListBufIndex;
                    }
                    else if((uiSlaveCount - (uint32_t)pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2]) < pstUP2IoModInfo->stSysIoSlaveInfo.usInDevNum[TASK_UP2])
                    {
                        uiSlaveCount = uiSlaveCount - (uint32_t)pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2];
                        ucSlaveNo = pstUP2IoModInfo->stSysIoSlaveInfo.ucInDevTabl[TASK_UP2][uiSlaveCount];
                        if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                        {
//                            printf("%d\n", ucSlaveNo);
                            if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo))
                            {
//                                printf("V\n");
                                memset(ucOutputBuf, 0, DATAPROCESS_OUTPUT_MAX_LEN);
                                /* 输出数据生成 */
                                bActiveFV = false;
                                /*设置从站使能标志*/
                                if(OS_SLAVE_ENABLE == SharedGetOnlineDownLoadSwitchFlg(TASK_UP2))
                                {
                                    if(true == SharedGetModConfigChangeFlg(ucSlaveNo))
                                    {/*当在线下装，当需要使能变化的从站，置本地标志*/
                                        DataProcessSetModEnableFlg(ucSlaveNo, true);
                                    }
                                }
                                bEnableFlg = DataProcessGetModEnableFlg(ucSlaveNo);
                                
                                DataProcessSlaveSendHandle(TASK_UP2,&ucOutputBuf[0], ucSlaveNo, bActiveFV, bEnableFlg);

                                /* 通过PCIE从双口RAM填写输出数据 */
                                ipbus_write(WRITE_IPBUS_OUTPUT,
                                        &ucOutputBuf[0],
                                        pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualOutputOffset,
                                        pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualOutputSize);
                            }
                            else
                            {
//                                printf("N\n");
                            }
                        }

                        ucRollingListBuf[uiRollingListBufIndex] = ucSlaveNo;
                        ucUP2RollingSlaveNo[uiUP2RollingListBufIndex] = ucSlaveNo;
                        uiRollingListBufIndex = uiRollingListBufIndex + 1;
                        uiUP2RollingListBufIndex = uiUP2RollingListBufIndex + 1;
                        uiUP2RollingSlaveNum = uiUP2RollingListBufIndex;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            /* 设置应用层状态为运行 */
            IPBUSSetAPPState(APP_STATE_RUN);
            /*根据配置信息填充从站周期轮询列表*/
            IPBUSSetSlaveRollingList(&ucRollingListBuf[0], 2 * DATAPROCESS_SLAVE_POLL_NUM_TOTAL);

            /* 释放权限 */
            IPBUSReleaseDataRAMRight();
        }

        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
        {
            uiTask1SlaveCountIndex = uiTask1SlaveCountIndex + 2 * ucUP1HandleNum; /* 每次处理一对模块 */
            if(uiTask1SlaveCountIndex >= (pstUP1IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP1] + pstUP1IoModInfo->stSysIoSlaveInfo.usInDevNum[TASK_UP1]))
            {
                uiTask1SlaveCountIndex = 0;
                SysSetIOPollFlg(TASK_UP1, IO_POLL_END);
            }
        }
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
        {
            uiTask2SlaveCountIndex = uiTask2SlaveCountIndex + 2 * ucUP2HandleNum; /* 每次处理一对模块 */
            if(uiTask2SlaveCountIndex >= (pstUP2IoModInfo->stSysIoSlaveInfo.usOutDevNum[TASK_UP2] + pstUP2IoModInfo->stSysIoSlaveInfo.usInDevNum[TASK_UP2]))
            {
                uiTask2SlaveCountIndex = 0;
                SysSetIOPollFlg(TASK_UP2, IO_POLL_END);
            }
        }
    }
    else
    {
        printf("IO_N\n");
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

    uint8_t ucDataExBuf[DATAPROCESS_AI_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucPreDataExBuf[DATAPROCESS_AI_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
    uint8_t ucNextDataExBuf[DATAPROCESS_AI_SAFETYDATA_LEN * DATAPROCESS_SLAVE_POLL_NUM_TOTAL] = {0}; /* 数据交换区buf */
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
    SysIoModInfo_t* pstUP1IoModInfo = NULL;
    SysIoModInfo_t* pstUP2IoModInfo = NULL;
    uint8_t ucSlaveNo = 0U;

    pstUP1IoModInfo = SharedGetIoModInfo(TASK_UP1); /* 获取对应IO表 */
    pstUP2IoModInfo = SharedGetIoModInfo(TASK_UP2); /* 获取对应IO表 */
    if((DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_CONFIG_SLAVE) && (pstUP1IoModInfo != NULL) && (pstUP2IoModInfo != NULL))
    {
        if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
        {/* 获取权限，接收输入数据 */
//            printf("I\n");
            /*进行UP1输入轮询*/
            for(uiSlaveCount = 0; uiSlaveCount < uiUP1RollingSlaveNum / 2; uiSlaveCount++)
            {/* 按照输出处理时确定的模块进行轮询 */
                ucSlaveNo = ucUP1RollingSlaveNo[2 * uiSlaveCount];
                memset(&ucSlaveQuality[ucSlaveNo/2][0], 0, 4); /* 清临时诊断区 */
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
//                    printf("%d\n", ucSlaveNo);
                    /* 清备用数据区，只清一次 */
                    pucStartAddr = &(g_stBackupRtData.ucI[0]);
                    memset(pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                            0x00, pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                    if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                    {
//                        printf("V\n");
                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);

                        /* 通过PCIE从双口RAM获得输入数据 */
                        ipbus_read(READ_IPBUS_INPUT,
                                &ucInputBuf[0],
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                        /* 输入数据解析，先拷贝到临时数据区 */
                        DataProcessSlaveRecvHandle(TASK_UP1,&ucInputBuf[0], ucSlaveNo, pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                    }
                    else
                    {
//                        printf("N\n");
                        /* 保证无论模块是否离线，三系处理时间一致 */
//                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                        /* 通过PCIE从双口RAM获得输入数据 */
//                        ipbus_read(READ_IPBUS_INPUT, &ucInputBuf[0], pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset, pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                    }
                }
                ucSlaveNo = ucUP1RollingSlaveNo[2 * uiSlaveCount + 1];
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
//                    printf("%d\n", ucSlaveNo);
                    if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                    {
//                        printf("V\n");
                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                        /* 通过PCIE从双口RAM获得输入数据 */
                        ipbus_read(READ_IPBUS_INPUT,
                                &ucInputBuf[0],
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);

                        /* 输入数据解析，先拷贝到临时数据区 */
                        DataProcessSlaveRecvHandle(TASK_UP1,&ucInputBuf[0], ucSlaveNo, pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                    }
                    else
                    {
//                        printf("N\n");
                        /* 保证无论模块是否离线，三系处理时间一致 */
//                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
//                        ipbus_read(READ_IPBUS_INPUT, &ucInputBuf[0], pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset, pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                    }
                }

                /* 拷贝到数据交换区 */
                usModTag = pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                switch(usModTag)
                {
                    case MODTAG_AI:
                    {
                        /* 拷贝数据 */
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        /* 拷贝质量 */
                        memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_AI_QDATA_LEN);
                        uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_AI_QDATA_LEN;
                    }
                    break;

                    case MODTAG_DI:
                    {
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_DI_QDATA_LEN);
                        uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_DI_QDATA_LEN;
                    }
                    break;

                    case MODTAG_OSP:
                    {
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
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

            /*进行UP2输入轮询*/
            for(uiSlaveCount = 0; uiSlaveCount < uiUP2RollingSlaveNum / 2; uiSlaveCount++)
            {/* 按照输出处理时确定的模块进行轮询 */
                ucSlaveNo = ucUP2RollingSlaveNo[2 * uiSlaveCount];
                memset(&ucSlaveQuality[ucSlaveNo/2][0], 0, 4); /* 清临时诊断区 */
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
//                    printf("%d\n", ucSlaveNo);
                    /* 清备用数据区，只清一次 */
                    pucStartAddr = &(g_stBackupRtData.ucI[0]);
                    memset(pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                            0x00, pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                    if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                    {
//                        printf("V\n");
                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                        /* 通过PCIE从双口RAM获得输入数据 */
                        ipbus_read(READ_IPBUS_INPUT,
                                &ucInputBuf[0],
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);

                        /* 输入数据解析，先拷贝到临时数据区 */
                        DataProcessSlaveRecvHandle(TASK_UP2,&ucInputBuf[0], ucSlaveNo, pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                    }
                    else
                    {
//                        printf("N\n");
                        /* 保证无论模块是否离线，三系处理时间一致 */
//                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                        /* 通过PCIE从双口RAM获得输入数据 */
//                        ipbus_read(READ_IPBUS_INPUT, &ucInputBuf[0], pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset, pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                    }
                }
                ucSlaveNo = ucUP2RollingSlaveNo[2 * uiSlaveCount + 1];
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
//                    printf("%d\n", ucSlaveNo);
                    if((IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(ucSlaveNo)) && (IPBUS_SLAVE_VALID == IPBUSGetSlaveValidState(ucSlaveNo)))
                    {
//                        printf("V\n");
                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
                        /* 通过PCIE从双口RAM获得输入数据 */
                        ipbus_read(READ_IPBUS_INPUT,
                                &ucInputBuf[0],
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);

                        /* 输入数据解析，先拷贝到临时数据区 */
                        DataProcessSlaveRecvHandle(TASK_UP2,&ucInputBuf[0], ucSlaveNo, pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag);
                    }
                    else
                    {
//                        printf("N\n");
                        /* 保证无论模块是否离线，三系处理时间一致 */
//                        memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);
//                        ipbus_read(READ_IPBUS_INPUT, &ucInputBuf[0], pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiDualInputOffset, pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize);
                    }
                }

                /* 拷贝到数据交换区 */
                usModTag = pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                switch(usModTag)
                {
                    case MODTAG_AI:
                    {
                        /* 拷贝数据 */
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        /* 拷贝质量 */
                        memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_AI_QDATA_LEN);
                        uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_AI_QDATA_LEN;
                    }
                    break;

                    case MODTAG_DI:
                    {
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
                        memcpy(&ucDataExBuf[uiDataExBufIndex], ucSlaveQuality[ucSlaveNo/2], DATAPROCESS_DI_QDATA_LEN);
                        uiDataExBufIndex = uiDataExBufIndex + DATAPROCESS_DI_QDATA_LEN;
                    }
                    break;

                    case MODTAG_OSP:
                    {
                        pucStartAddr = &(g_stBackupRtData.ucI[0]);
                        memcpy(&ucDataExBuf[uiDataExBufIndex],
                                pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        uiDataExBufIndex = uiDataExBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize;
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
        else
        {
            printf("NoRight\n");
        }

        /* 数据进行交换及表决 */
        if(uiDataExBufIndex > 0)
        {/* 有I区数据需要交换 */
            /* 交换I区数据 */
            DataExChange(&ucDataExBuf[0], (uint16_t)uiDataExBufIndex, &ucPreDataExBuf[0], &ucNextDataExBuf[0], &ucPreDataFlg, &ucNextDataFlg);

            /*对UP1的模块进行表决*/
            for(uiSlaveCount = 0; uiSlaveCount < uiUP1RollingSlaveNum / 2; uiSlaveCount++)
            {/* 按照输出处理时确定的模块进行轮询 */
                memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AI_SAFETYDATA_LEN);
                memset(&ucDiagBuff[0], 0x00, 32);
                ucSlaveNo = ucUP1RollingSlaveNo[2 * uiSlaveCount];
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
                    usModTag = pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                    if((usModTag == MODTAG_AI) || (usModTag == MODTAG_DI) || (usModTag == MODTAG_OSP))
                    {
                        /* 拷贝旧数据 */
                        pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
                        memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

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
                        DataExChangeVote(TASK_UP1, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                    pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);

                        usModTag = pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;

                        /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                        memcpy(pucStartAddr + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

                        if(PF_PAUSE != SharedGetPauseFlag(TASK_UP1))
                        {
                            SharedSetInputModuleVar(&ucDesDataVoteBuf[0], 
                                                    pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                                    pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        }

                        switch(usModTag)
                        {
                            case MODTAG_AI:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_AI_QDATA_LEN;
                            }
                            break;

                            case MODTAG_DI:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_DI_QDATA_LEN;
                            }
                            break;

                            case MODTAG_OSP:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP1IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_OSP_QDATA_LEN;
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
            /*对UP2的模块进行表决*/
            for(uiSlaveCount = 0; uiSlaveCount < uiUP2RollingSlaveNum / 2; uiSlaveCount++)
            {/* 按照输出处理时确定的模块进行轮询 */
                memset(&ucDesDataVoteBuf[0], 0x00, DATAPROCESS_AI_SAFETYDATA_LEN);
                memset(&ucDiagBuff[0], 0x00, 32);
                ucSlaveNo = ucUP2RollingSlaveNo[2 * uiSlaveCount];
                if((ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (ucSlaveNo >= IPBUS_MIN_SLAVE_NO))
                {
                    usModTag = pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                    if((usModTag == MODTAG_AI) || (usModTag == MODTAG_DI) || (usModTag == MODTAG_OSP))
                    {
                        /* 拷贝旧数据 */
                        pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
                        memcpy(&ucOldDataVoteBuf[0], pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);

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
                        DataExChangeVote(TASK_UP2, ucSlaveNo, &ucDataExBuf[uiDataVoteBufIndex], pucPreDataVoteAddr,
                                    pucNextDataVoteAddr, &ucOldDataVoteBuf[0], &ucDesDataVoteBuf[0], &ucDiagBuff[0]);

                        usModTag = pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
//                        if(usModTag == MODTAG_AI)
//                        {
//                            printf("%d;%d\n",ucPreDataFlg,ucNextDataFlg);
//                            printf("IV%d;%d;%d;%d;%d\n", *((puint16_t)(&ucDataExBuf[uiDataVoteBufIndex])), *((puint16_t)(&ucDesDataVoteBuf[0])), ucDiagBuff[0],*((puint16_t)(&ucPreDataExBuf[uiDataVoteBufIndex])),*((puint16_t)(&ucNextDataExBuf[uiDataVoteBufIndex])));
//                        }

                        /* 将表决完的数据拷贝到共享数据区Q区的对应地址 */
                        memcpy(pucStartAddr + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset, &ucDesDataVoteBuf[0],
                                pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        
                        if(PF_PAUSE != SharedGetPauseFlag(TASK_UP2))
                        {
                            SharedSetInputModuleVar(&ucDesDataVoteBuf[0], 
                                                    pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].uiIecOffset,
                                                    pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize);
                        }

                        switch(usModTag)
                        {
                            case MODTAG_AI:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_AI_QDATA_LEN;
                            }
                            break;

                            case MODTAG_DI:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_DI_QDATA_LEN;
                            }
                            break;

                            case MODTAG_OSP:
                            {
                                uiDataVoteBufIndex = uiDataVoteBufIndex + pstUP2IoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usIecSize + DATAPROCESS_OSP_QDATA_LEN;
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

        /*输入数据从共享数据区I区拷贝到运算数据区I区*/
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP1))
        {
            if(uiInputTask1Flag == 0)
            {
                DataProcessInputDataCopy(TASK_UP1);
                uiInputTask1Flag = 1;
            }

        }
        if(IO_POLL_START == SysGetIOPollFlg(TASK_UP2))
        {
            if(uiInputTask2Flag == 0)
            {
                DataProcessInputDataCopy(TASK_UP2);
                uiInputTask2Flag = 1;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessSoftSOEInit
*
* 功能描述: 单块生成软SOE记录
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
void DataProcessSoftSOEInit(Task_ID_t emTaskID)
{
    uint16_t i = 0U;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    uint8_t ucCurrentVar = 0U;
    puint8_t pucVarAddr = NULL;
    uint32_t uiIndex = 0U;
    uint32_t uiOffset = 0U;
    SoftSOE_t stSOE;
    SysTime_t stSysTime;
    uint8_t ucSOEBlockNo = 0U;
    uint8_t ucSOEBlockIndex = 0U;

    pstSOEConfigInfo = SharedGetSOEConfigInfo(emTaskID); /* 获取SOE表 */
    if((pstSOEConfigInfo != NULL) && (emTaskID < MAX_TASK_NUM))
    {
        for(ucSOEBlockIndex = 0U; ucSOEBlockIndex < pstSOEConfigInfo->ucSoftSOEBlockNum[emTaskID]; ucSOEBlockIndex++)
        {
            ucSOEBlockNo = pstSOEConfigInfo->ucSoftSOEBlockTable[emTaskID][ucSOEBlockIndex];

            uiIndex = pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarIndex;

            for(i = 0; i < pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarNum; i++)
            {
                pucVarAddr = SysGetPRGRtDataStartAddr(pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].ucRefID);
                if(pucVarAddr != NULL)
                {
                    if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                    {
                        uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset / 8U;
                    }
                    else
                    {
                        uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset;
                    }
                    pucVarAddr = &pucVarAddr[uiOffset];
    //                printf("V1=%d\n",*pucVarAddr);
    
                    if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                    {
                        ucCurrentVar = *pucVarAddr;
                        if((ucCurrentVar & (0x01U << (pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset % 8U))) != 0U)
                        {
                            ucCurrentVar = 1U;
                        }
                        else
                        {
                            ucCurrentVar = 0U;
                        }
                    }
                    else
                    {
                        memcpy(&ucCurrentVar, pucVarAddr, (uint32_t)pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize);
                    }
                    
                    ucDataProcessSOELastVar[uiIndex+i] = ucCurrentVar;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSFillSlaveRollingListTable
*
* 功能描述: 根据配置信息填充从站周期轮询列表（先按照全部轮询处理）
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
//static void DataProcessFillSlaveRollingListTable(void)
//{
//    SysIoModInfo_t* pstSysIoModInfo = NULL;
//    uint16_t i = 0U;
//    uint16_t j = 0U;
//    uint8_t ucDataTemp[256] = {0U};
//    uint32_t uiOffset = 0U;
//
//    pstSysIoModInfo = SharedGetIoModInfo();
//    if(pstSysIoModInfo == NULL)
//    {
//        return;
//    }

//    for(j = 0;j < MAX_TASK_NUM; j++)
//    {
//        /* 依次将输出模块的从站号填入从站周期轮询列表 */
//        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j]; i++)
//        {
//            ucDataTemp[uiOffset]  = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[j][i];
//            uiOffset = uiOffset + 1;
//        }
//    }
//    for(j = 0;j < MAX_TASK_NUM; j++)
//    {
//        /* 依次将输入模块的从站号填入从站周期轮询列表 */
//        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]; i++)
//        {
//            ucDataTemp[uiOffset]  = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[j][i];
//            uiOffset = uiOffset + 1;
//        }
//    }

//    IPBUSSetSlaveRollingList(&ucDataTemp[0], uiOffset);

//    return;
//}

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
static void DataProcessSlaveSendHandle(Task_ID_t emTaskID, uint8_t ucSendDest[], uint8_t ucSlaveNo, bool_t bActiveFV, bool_t bEnableFlg)
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
    PMIOMakeReq(ucSlaveNo, &ucRawDataBuf[0], ucRawDataDataLen, bActiveFV, bEnableFlg, \
                &ucSendDest[sizeof(DataProcessSlaveSlotHeader_t)], &usSafetyDataDataLen);

    /* 填slot头 */
    stSlotHead.ucSlotType = DATAPROCESS_SLOT_TYPE_SAFETY;
    stSlotHead.ucSlotLen = (uint8_t)usSafetyDataDataLen;
//    printf("%d\n", usSafetyDataDataLen);
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

    /* 得到Slot个数 */
    memcpy(&ucSlotCount, &ucRecvSrc[usBufIndex], sizeof(uint8_t));
    usBufIndex += sizeof(uint8_t);

    pstSysIoModInfo = SharedGetIoModInfo(emTaskID); /* 获取对应IO表 */
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
    if((pstSysIoModInfo != NULL) && (ucSlotCount <= DATAPROCESS_MAX_SLOT_NUM))
    {
        for(i = 0; i < ucSlotCount; i++)
        {
            /* 拷贝Slot头 */
            memcpy(&stSlotHead, &ucRecvSrc[usBufIndex], sizeof(DataProcessSlaveSlotHeader_t));
            usBufIndex += sizeof(DataProcessSlaveSlotHeader_t);

            if(usBufIndex + stSlotHead.ucSlotLen > pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usDualInputSize)
            {
                printf("LEN ERROR!\n");
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
                    }

                    usBufIndex += ucSlotLen;
                }
                break;

                case DATAPROCESS_SLOT_TYPE_DIAG:
                {/* 诊断数据slot */
//                    if(ucSlaveNo == 4U)
//                    {
//                        printf("DIAG=%d\n", stSlotHead.ucSlotLen);
//                    }

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
//                    if(ucSlaveNo == 4U)
//                    {
//                        printf("SOE=%d\n", stSlotHead.ucSlotLen);
//                    }

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
//                    if(ucSlaveNo == 5U)
//                    {
//                        printf("OSP=%d:\n", stSlotHead.ucSlotLen);
//                    }

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
                    printf("Fail\n");
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
//        printf("E9\n");
        /* 数据交换 */
        
        if(true == bParOK)
        {/*当在线下装，接收到IO已经切换参数OK标志，清空本地记录使能标志*/
            DataProcessSetModEnableFlg(ucSlaveNo, false);
//            printf("M %d ParOKs\n");
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
    SysTime_t stSysTime;
    uint8_t ucModType = 0U;

    if(MODTAG_AI == usModTag)
    {
        ucModType = 1;
    }
    if((MODTAG_DI == usModTag) || (MODTAG_OSP == usModTag))
    {
        ucModType = 0;
    }

    /* 取得系统时间 */
    SysGetSysTime(&stSysTime);

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
        stSOE.uiSec = stSysTime.uiSec - (stSysTime.uiSec % TIME_MIN_TO_SEC);
        /* 修正SOE时间 */
        if(stSlaveSOEData.uiSOETick > TIME_MIN_TO_MSEC +1)
        {
            stSlaveSOEData.uiSOETick -= TIME_MIN_TO_MSEC;
        }
        /* 填写综合后的秒数及微秒数 */
        stSOE.uiSec += stSlaveSOEData.uiSOETick / TIME_SEC_TO_MSEC;
        stSOE.uiMicroSec = stSlaveSOEData.uiSOETick % TIME_SEC_TO_MSEC;

        /* 修正分钟时间 */
        if(stSOE.uiSec >= stSysTime.uiSec)
        {/* 大于10s，回退1分钟 */
            if((stSOE.uiSec - stSysTime.uiSec) >= TIME_SEC_DIFF_PRECISON)
            {
                stSOE.uiSec = stSOE.uiSec - TIME_MIN_TO_SEC;
            }
        }
        else
        {/* 前溯1分钟 */
            if((stSysTime.uiSec - stSOE.uiSec) >= TIME_SEC_DIFF_PRECISON)
            {
                stSOE.uiSec = stSOE.uiSec + TIME_MIN_TO_SEC;
            }
        }

        /* 填SOE队列 */
        SharedSetHardSOE(stSOE);

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
    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucTempDiag = stRecvDiagData.ucTempDiag;
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
* 函数名称: DataProcessGenerateSoftSOERecord
*
* 功能描述: 单块生成软SOE记录
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
static void DataProcessGenerateSoftSOERecord(Task_ID_t uiTaskID,uint8_t ucSOEBlockNo)
{
    uint16_t i = 0U;
    SOEConfigInfo_t* pstSOEConfigInfo = NULL;
    uint8_t ucCurrentVar = 0U;
    puint8_t pucVarAddr = NULL;
    uint32_t uiIndex = 0U;
    uint32_t uiOffset = 0U;
    SoftSOE_t stSOE;
    SysTime_t stSysTime;

    pstSOEConfigInfo = SharedGetSOEConfigInfo(uiTaskID); /* 获取SOE表 */
    if(pstSOEConfigInfo != NULL)
    {
        if(ucSOEBlockNo < MAX_SOE_BLOCK_NUM)
        {
            uiIndex = pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarIndex;

            for(i = 0; i < pstSOEConfigInfo->stSoftSOEBlock[ucSOEBlockNo].uiSoftSOEVarNum; i++)
            {
                if((uiIndex+i) < MAX_SOE_VAR_NUM)
                {
                    pucVarAddr = SysGetPRGRtDataStartAddr(pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].ucRefID);
                    if(pucVarAddr != NULL)
                    {
                        if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                        {
                            uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset / 8U;
                        }
                        else
                        {
                            uiOffset = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset;
                        }

                        pucVarAddr = &pucVarAddr[uiOffset];

                        if(0U == pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize)
                        {
                            ucCurrentVar = *pucVarAddr;
                            if((ucCurrentVar & (0x01U << (pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].uiOffset % 8U))) != 0U)
                            {
                                ucCurrentVar = 1U;
                            }
                            else
                            {
                                ucCurrentVar = 0U;
                            }
                        }
                        else
                        {
                            memcpy(&ucCurrentVar, pucVarAddr, (uint32_t)pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usVarSize);
                        }

                        if(ucCurrentVar != ucDataProcessSOELastVar[uiIndex+i])
                        {/* 值变化，有软SOE产生 */
                            SysGetSysTime(&stSysTime);
                            stSOE.ucBlockNo = ucSOEBlockNo;
                            stSOE.ucValue = ucCurrentVar;
                            stSOE.usIndex = pstSOEConfigInfo->stSoftSOEVar[uiIndex+i].usBlockIndex;
                            stSOE.uiSec = stSysTime.uiSec;
                            stSOE.uiMicroSec = stSysTime.uiMicSec;

                            SharedSetSOE(ucSOEBlockNo, stSOE);
                            ucDataProcessSOELastVar[uiIndex+i] = ucCurrentVar;
                        }
                    }
                }
            }
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
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
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
void DataProcessModEnableFlgInit(void)
{
    memset(&s_bModEnableFlg[0], 0x00, sizeof(bool_t)*MAX_MOD_NUM);
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
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
static bool_t DataProcessGetModEnableFlg(uint8_t ucModAddr)
{
    bool_t bModEnableFlg = false;
    
    if(ucModAddr < MAX_MOD_NUM)
    {
        bModEnableFlg = s_bModEnableFlg[ucModAddr];
    }
    
    return bModEnableFlg;
}
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
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
static void DataProcessSetModEnableFlg(uint8_t ucModAddr, bool_t bModEnableFlg)
{
    if((ucModAddr < MAX_MOD_NUM) && 
        ((false == bModEnableFlg) || (true == bModEnableFlg)))
    {
        s_bModEnableFlg[ucModAddr] = bModEnableFlg;
    }
    
    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: Outputtest
*
* 功能描述: 输出测试数据
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
/*
static void Outputtest(void)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;
    uint8_t uctestbuf[16] = {0x55,0xAA,0x55,0xAA,0};

    pucStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_Q);
    memcpy(pucStartAddr, &uctestbuf[0], 16);

    return;
}
*/
