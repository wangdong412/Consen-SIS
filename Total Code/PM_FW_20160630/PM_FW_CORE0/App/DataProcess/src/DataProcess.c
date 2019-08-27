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
#include "App/DataProcess/inc/DataProcess.h"
#include "Srv/SharedMem/inc/SharedMem.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static uint8_t DataProcessSlaveRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint16_t usModTag,
                       uint32_t uiIecOffset);

static uint8_t DataProcessSlaveSendHandle(uint8_t ucSendDest[], uint8_t ucModAddr, uint16_t usModTag,
                       uint32_t uiIecOffset);

static uint8_t DataProcessAIRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset);

static uint8_t DataProcessDIRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset);

static uint8_t DataProcessOSPRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset);

static uint8_t DataProcessAOSendHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset);

static uint8_t DataProcessDOSendHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset);

static uint32_t DataProcessCheckDownloadIOConfigFinish(void);

static uint8_t DataProcessInputDataCopy(uint32_t uiTaskID);

static uint8_t DataProcessOutputDataCopy(uint32_t uiTaskID);

/*
static void Outputtest(void);
*/

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
    /* IPBUS相关的FPGA RAM初始化 */
    IPBUSDualRamInit();
    /* 置应用层状态为停止状态 */
    IPBUSSetAPPState(APP_STATE_STOP);
    /* 置应用层标志在线/离线位为在线 */
    IPBUSSetAPPOnlineFlag(IPBUS_APP_ONLINE);
    /* 模块使用的数据区及结构体初始化 */

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
void DataProcessFillSlaveRollingListTable(void)
{
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    uint16_t i = 0U;
    uint16_t j = 0U;
    uint8_t ucDataTemp[256] = {0U};
    uint32_t uiOffset = 0U;

    pstSysIoModInfo = SharedGetIoModInfo();
    if(pstSysIoModInfo == NULL)
    {
        return;
    }

    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        /* 依次将输出模块的从站号填入从站周期轮询列表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[j]; i++)
        {
            ucDataTemp[uiOffset]  = pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[j][i].ucModAddr;
            uiOffset = uiOffset + 1;
        }
    }
    for(j = 0;j < MAX_TASK_NUM; j++)
    {
        /* 依次将输入模块的从站号填入从站周期轮询列表 */
        for (i = 0; i < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[j]; i++)
        {
            ucDataTemp[uiOffset]  = pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[j][i].ucModAddr;
            uiOffset = uiOffset + 1;
        }
    }

    IPBUSSetSlaveRollingList(&ucDataTemp[0], uiOffset);

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
    uint32_t uiSlaveCount = 0U;
    uint32_t uiTaskCount = 0U;
    uint8_t ucOutputBuf[DATAPROCESS_INPUT_MAX_LEN] = {0}; /* 数据区缓存 */
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    /*输出数据从运算数据区Q区拷贝到共享数据区Q区*/
    DataProcessOutputDataCopy(TASK_UP1);
//    DataProcessOutputDataCopy(TASK_UP2);
    SetLEDState(LED_NAME_IPBUS, LED_OFF);

    if(DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_NOCONFIG_SLAVE)
    {/*检查主卡是否完成从站配置*/
//        printf("DownIOCon not OK\n");
        return;
    }
    pstSysIoModInfo = SharedGetIoModInfo(); /* 获取对应IO表 */
    if (pstSysIoModInfo == NULL)
    {
//        printf("IoInfo not OK\n");
        return;
    }

    if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
    {/* 获取权限，接收输入数据 */
        for(uiTaskCount = 0; uiTaskCount < MAX_TASK_NUM; uiTaskCount++)
        {
            for(uiSlaveCount = 0; uiSlaveCount < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[uiTaskCount]; uiSlaveCount++)
            {
                /* 此处应该分别读取偶数站和奇数站冗余槽位数据，根据模块主从信息及数据质量确定是否用该槽位数据 */
                /* 目前的数据结构无法支持 */
                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].ucModAddr))
                {
                    SetLEDState(LED_NAME_IPBUS, LED_BLINK);

                    memset(ucOutputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);

                    DataProcessSlaveSendHandle(&ucOutputBuf[0],
                            pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].ucModAddr,
                            pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].usModTag,
                            pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].uiIecOffset);
                    /* 通过PCIE从双口RAM填写输出数据 */
                    ipbus_write(WRITE_IPBUS_OUTPUT, &ucOutputBuf[0],
                            pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].uiDualOutputOffset,
                            pstSysIoModInfo->stSysIoSlaveInfo.stOutDevInfo[uiTaskCount][uiSlaveCount].usDualOutputSize);
                }
                else
                {
                    ;
                }
            }
        }

        /* 设置应用层状态为运行 */
        IPBUSSetAPPState(APP_STATE_RUN);
        /*根据配置信息填充从站周期轮询列表*/
        DataProcessFillSlaveRollingListTable();
        /* 释放权限 */
        IPBUSReleaseDataRAMRight();
    }
    else
    {
        ;
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
    uint32_t uiTaskCount = 0U;
    uint8_t ucInputBuf[DATAPROCESS_INPUT_MAX_LEN] = {0};  /* 数据区缓存 */
    SysIoModInfo_t* pstSysIoModInfo = NULL;

    if(DataProcessCheckDownloadIOConfigFinish() == IPBUS_DEVICE_NOCONFIG_SLAVE)
    {/*检查主卡是否完成从站配置*/
//        printf("DownIOCon not OK\n");
        return;
    }

    pstSysIoModInfo = SharedGetIoModInfo();;          /* 获取对应IO表            */
    if (pstSysIoModInfo == NULL)
    {
//        printf("IoInfo not OK\n");
        return;
    }

    if(IPBUS_RIGHTS_FREE == IPBUSGetDataRAMRight())
    {/* 获取权限，接收输入数据 */
        for(uiTaskCount = 0; uiTaskCount < MAX_TASK_NUM; uiTaskCount++)
        {
            for(uiSlaveCount = 0; uiSlaveCount < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[uiTaskCount]; uiSlaveCount++)
            {
                /* 此处应该分别读取偶数站和奇数站冗余槽位数据，根据模块主从信息及数据质量确定是否用该槽位数据 */
                /* 目前的数据结构无法支持 */
                if(IPBUS_SLAVE_ONLINE == IPBUSGetSlaveOnlineState(pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].ucModAddr))
                {
                    SetLEDState(LED_NAME_IPBUS, LED_BLINK);
                    memset(ucInputBuf, 0, DATAPROCESS_INPUT_MAX_LEN);

                    /* 通过PCIE从双口RAM获得输入数据 */
                    ipbus_read(READ_IPBUS_INPUT, &ucInputBuf[0],
                            pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].uiDualInputOffset,
                            pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].usDualInputSize);
                    DataProcessSlaveRecvHandle(&ucInputBuf[0],
                            pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].ucModAddr,
                            pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].usModTag,
                            pstSysIoModInfo->stSysIoSlaveInfo.stInDevInfo[uiTaskCount][uiSlaveCount].uiIecOffset);
                }
                else
                {
                    ;
                }
            }
        }
        /*输入数据从共享数据区I区拷贝到运算数据区I区*/
        DataProcessInputDataCopy(TASK_UP1);
//        DataProcessInputDataCopy(TASK_UP2);
    }

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
static uint8_t DataProcessSlaveRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint16_t usModTag,
                       uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;

    /* 判断源地址是否有效 */

    switch(usModTag)
    {
        case MODTAG_AI:
        {
            /* AI模块数据处理 */
            DataProcessAIRecvHandle(&ucRecvSrc[0], ucModAddr, uiIecOffset);
        }
        break;

        case MODTAG_DI:
        {
            /* DI模块数据处理 */
            DataProcessDIRecvHandle(&ucRecvSrc[0], ucModAddr, uiIecOffset);
        }
        break;

        case MODTAG_OSP:
        {
            /* OSP模块数据处理 */
            DataProcessOSPRecvHandle(&ucRecvSrc[0], ucModAddr, uiIecOffset);
        }
        break;

        default:
        break;
    }

    return ucRet;
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
static uint8_t DataProcessSlaveSendHandle(uint8_t ucSendDest[], uint8_t ucModAddr, uint16_t usModTag,
                       uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;

    /* 判断源地址是否有效 */

    switch(usModTag)
    {
        case MODTAG_AO:
        {
            /* AO模块数据处理 */
            DataProcessAOSendHandle(&ucSendDest[0], ucModAddr, uiIecOffset);
        }
        break;

        case MODTAG_DO:
        {
            /* DO模块数据处理 */
            DataProcessDOSendHandle(&ucSendDest[0], ucModAddr, uiIecOffset);
        }
        break;

        default:
        break;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessAIRecvHandle
*
* 功能描述: 处理AI输入数据
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
static uint8_t DataProcessAIRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;

    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
    memcpy(pucStartAddr + uiIecOffset, &ucRecvSrc[10], 64);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessDIRecvHandle
*
* 功能描述: 处理DI输入数据
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
static uint8_t DataProcessDIRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;

    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
    memcpy(pucStartAddr + uiIecOffset, &ucRecvSrc[10], 4);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessOSPRecvHandle
*
* 功能描述: 处理OSP输入数据
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
static uint8_t DataProcessOSPRecvHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;

    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_I);
    memcpy(pucStartAddr + uiIecOffset, &ucRecvSrc[10], 32);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessAOSendHandle
*
* 功能描述: 处理AO输入数据
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
static uint8_t DataProcessAOSendHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;

    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);
    memcpy(&ucRecvSrc[10], pucStartAddr + uiIecOffset, 32);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessDOSendHandle
*
* 功能描述: 处理DO输入数据
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
static uint8_t DataProcessDOSendHandle(uint8_t ucRecvSrc[], uint8_t ucModAddr, uint32_t uiIecOffset)
{
    uint8_t  ucRet = 0U;
    puint8_t pucStartAddr = NULL;

    pucStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_Q);
    memcpy(&ucRecvSrc[10], pucStartAddr + uiIecOffset, 4);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataProcessDOSendHandle
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

    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
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
* 函数名称: DataProcessDOSendHandle
*
* 功能描述: 输出数据从运算数据区O区拷贝到共享数据区O区
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
static uint8_t DataProcessOutputDataCopy(uint32_t uiTaskID)
{
    uint8_t  ucRet = DATAPROCESS_COPY_OK;
    puint8_t pucMemStartAddr = NULL;
    puint8_t pucPrgStartAddr = NULL;
    CtrlStationConfigInfo_t* pstCtrlStationConfigInfo = NULL;

    pucMemStartAddr = SysGetSharedMemRtDataStartAddr(RT_DATA_TYPE_G);
    pucPrgStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);

    pstCtrlStationConfigInfo = SharedGetCtrlStationInfo();
    if (pstCtrlStationConfigInfo == NULL)
    {
        ucRet = DATAPROCESS_COPY_NOTOK;
    }
    else
    {
        memcpy(pucMemStartAddr + pstCtrlStationConfigInfo->stTaskConfigInfo[uiTaskID].uiOOffset,
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
