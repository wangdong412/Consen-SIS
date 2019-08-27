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
*  文件名称    : SharedSOE.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-07-19
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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/SharedSOE.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/Sys/inc/CommSys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SOEQueue_t stSOEQueue[MAX_SOE_QUEUE_NUM];
static SoftSOE_t stSoftSOEArray[MAX_SOE_NUM_DDR];


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static bool_t SharedSetSRAMSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE);
static bool_t SharedSetDDRSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE);
static int32_t SharedGetSRAMSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex);
static int32_t SharedGetDDRSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex);



/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedSOEInit
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
void SharedSOEInit(void)
{
    uint8_t ucBlockIndex = HARD_SOE_QUEUE_INDEX;
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    /* 初始化 */
    //memset(&stSoftSOEArray[0], 0x00, sizeof(SoftSOE_t)*MAX_SOE_NUM_DDR);
    memset(&stSOEQueue[0], 0x00, sizeof(SOEQueue_t)*MAX_SOE_QUEUE_NUM);

    if(pstSOESRAMQueuesInfoStartAddr->uiFlag != SOE_SRAM_QUEUE_INFO_VALID_FLAG)
    {
        pstSOESRAMQueuesInfoStartAddr->uiFlag = 0;
        //printf("flag=0x%x\n", pstSOESRAMQueuesInfoStartAddr->uiFlag);
        memset((puint8_t)pstSOESRAMQueuesInfoStartAddr, 0x00, sizeof(SOESRAMQueuesInfo_t));

        //Hard SOE config
        stSOEQueue[ucBlockIndex].bConfiged = SOE_QUEUE_CONFIGED;
        stSOEQueue[ucBlockIndex].ucQueueState = SOE_QUEUE_EMPTY;
        stSOEQueue[ucBlockIndex].ucStorage = SOE_STORAGE_SRAM;
        pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usStorage = (uint16_t)stSOEQueue[ucBlockIndex].ucStorage;
        //printf("Hard SOE storage=%d\n", pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usStorage);
        stSOEQueue[ucBlockIndex].uiBase = 0;
        //printf("Hard SOE base=%d index=%d\n", stSOEQueue[ucBlockIndex].uiBase, ucBlockIndex);
        stSOEQueue[ucBlockIndex].usSize = MAX_SOE_NUM_SRAM;
        //printf("Hard SOE queue size=%d\n", stSOEQueue[ucBlockIndex].usSize);
        stSOEQueue[ucBlockIndex].usHeader = 0;
        stSOEQueue[ucBlockIndex].usTail = 0;
        stSOEQueue[ucBlockIndex].usType = SOE_QUEUE_TYPE_CYCLE_FIFO;
        //printf("Hard SOE type=%d\n", stSOEQueue[ucBlockIndex].usType);
    }
    else
    {
        //Hard SOE config
        stSOEQueue[ucBlockIndex].bConfiged = SOE_QUEUE_CONFIGED;
        stSOEQueue[ucBlockIndex].ucQueueState = pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usQueueState;
        stSOEQueue[ucBlockIndex].ucStorage = SOE_STORAGE_SRAM;
        //printf("Hard SOE storage=%d\n", pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usStorage);
        stSOEQueue[ucBlockIndex].uiBase = 0;
        //printf("Hard SOE base=%d index=%d\n", stSOEQueue[ucBlockIndex].uiBase, ucBlockIndex);
        stSOEQueue[ucBlockIndex].usSize = MAX_SOE_NUM_SRAM;
        //printf("Hard SOE queue size=%d\n", stSOEQueue[ucBlockIndex].usSize);
        stSOEQueue[ucBlockIndex].usHeader = pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usHeader;
        stSOEQueue[ucBlockIndex].usTail = pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usTail;
        stSOEQueue[ucBlockIndex].usType = SOE_QUEUE_TYPE_CYCLE_FIFO;
        //printf("Hard SOE type=%d, Header=%d, Tail=%d\n", stSOEQueue[ucBlockIndex].usType,stSOEQueue[ucBlockIndex].usHeader, stSOEQueue[ucBlockIndex].usTail);
    }


}

/*
***********************************************************************************************************************
* 函数名称: SharedSOEKeyInit
*
* 功能描述: 钥匙开关在Init时的初始化
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
void SharedSOEKeyInit(void)
{
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    /* 清除硬SOE在SRAM中的记录标识 */
    pstSOESRAMQueuesInfoStartAddr->uiFlag = 0;
}
/*
***********************************************************************************************************************
* 函数名称: SharedSOEQueueCfg
*
* 功能描述: 根据配置信息对队列进行配置
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
void SharedSOEQueueCfg(void)
{
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    /* 根据SOE配置信息对队列进行初始化 */
    SOEConfigInfo_t* pstSOEcfg = NULL;
    uint8_t i = 0;
    uint8_t ucBlockIndex = 0;
    uint8_t ucBlockNum = 0;
    uint8_t ucTaskIndex = 0;

    pstSOEcfg = SharedGetSOEConfigInfo(MAX_TASK_NUM);

    if(pstSOEcfg != NULL)
    {
        for(ucTaskIndex = TASK_UP1; ucTaskIndex < MAX_TASK_NUM; ucTaskIndex++)
        {
            ucBlockNum = pstSOEcfg->ucSoftSOEBlockNum[ucTaskIndex];
            for(i = 0; i < ucBlockNum; i++)
            {
                ucBlockIndex = pstSOEcfg->ucSoftSOEBlockTable[ucTaskIndex][i];

                if(ucBlockIndex < MAX_SOE_QUEUE_NUM)
                {
                    if(ACTIVE == pstSOEcfg->stSoftSOEBlock[ucBlockIndex].emActive)
                    {
                        stSOEQueue[ucBlockIndex].bConfiged = SOE_QUEUE_CONFIGED;
                        stSOEQueue[ucBlockIndex].ucQueueState = SOE_QUEUE_EMPTY;
                        stSOEQueue[ucBlockIndex].ucStorage = pstSOEcfg->stSoftSOEBlock[ucBlockIndex].ucProperty;
                        pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usStorage = stSOEQueue[ucBlockIndex].ucStorage;
                        printf("storage=%d\n", pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucBlockIndex].usStorage);
                        stSOEQueue[ucBlockIndex].uiBase = pstSOEcfg->stSoftSOEBlock[ucBlockIndex].uiSW_BaseAddr;
                        printf("base=%d index=%d\n", stSOEQueue[ucBlockIndex].uiBase, ucBlockIndex);
                        stSOEQueue[ucBlockIndex].usSize = pstSOEcfg->stSoftSOEBlock[ucBlockIndex].uiMaxRecordCount;
                        printf("size=%d\n", stSOEQueue[ucBlockIndex].usSize);
                        stSOEQueue[ucBlockIndex].usHeader = 0;
                        stSOEQueue[ucBlockIndex].usTail = 0;
                        stSOEQueue[ucBlockIndex].usType = pstSOEcfg->stSoftSOEBlock[ucBlockIndex].usBlockType;
                        printf("type=%d\n", stSOEQueue[ucBlockIndex].usType);
                    }
                }
            }
        }

        /* 从SRAM中恢复SOE队列的相关掉电保持信息 */
        if(SOE_SRAM_QUEUE_INFO_VALID_FLAG == pstSOESRAMQueuesInfoStartAddr->uiFlag)
        {
            for(i = 0; i < MAX_SOE_QUEUE_NUM; i++)
            {
                if(SOE_STORAGE_SRAM == pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[i].usStorage)
                {
                    stSOEQueue[i].ucQueueState = (uint8_t)pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[i].usQueueState;
                    stSOEQueue[i].usHeader = pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[i].usHeader;
                    stSOEQueue[i].usTail = pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[i].usTail;
                    printf("State=%d, Header=%d, Tail=%d\n",stSOEQueue[i].ucQueueState, stSOEQueue[i].usHeader, stSOEQueue[i].usTail);
                }
            }
        }
    }
    else
    {
        printf("SOE CFG is NULL\n");
    }
#if 0
    /*TEST START*/
    stSOEQueue[0].bConfiged = SOE_QUEUE_CONFIGED;
    stSOEQueue[0].ucQueueState = SOE_QUEUE_EMPTY;
    stSOEQueue[0].ucStorage = SOE_STORAGE_SRAM;
    stSOEQueue[0].uiBase = 0;
    stSOEQueue[0].usSize = 100;
    stSOEQueue[0].usHeader = 0;
    stSOEQueue[0].usTail = 0;
    stSOEQueue[0].usType = SOE_QUEUE_TYPE_HISTORY;

    stSOEQueue[1].bConfiged = SOE_QUEUE_CONFIGED;
    stSOEQueue[1].ucQueueState = SOE_QUEUE_EMPTY;
    stSOEQueue[1].ucStorage = SOE_STORAGE_DDR;
    stSOEQueue[1].uiBase = 100;
    stSOEQueue[1].usSize = 4;
    stSOEQueue[1].usHeader = 0;
    stSOEQueue[1].usTail = 0;
    stSOEQueue[1].usType = SOE_QUEUE_TYPE_CYCLE_FIFO;
    /*TEST END*/
#endif

}

/*
***********************************************************************************************************************
* 函数名称: SharedSetSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: ucQueueIndex: 队列索引
*        stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedSetSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE)
{
    bool_t bRet = false;
    bool_t bAtomicFlg=false;
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    if(ucQueueIndex < MAX_SOE_QUEUE_NUM)
    {
        bAtomicFlg = atomic32Cas(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK, SOE_QUEUE_LOCK);
        //printf("set soe set flg=%d\n",stSOEQueue[ucQueueIndex].uiLockFlg);
        if(true == bAtomicFlg)
        {
            if(SOE_STORAGE_SRAM == stSOEQueue[ucQueueIndex].ucStorage)
            {
                bRet = SharedSetSRAMSOE(ucQueueIndex, stSOE);

                pstSOESRAMQueuesInfoStartAddr->uiFlag = SOE_SRAM_QUEUE_INFO_VALID_FLAG;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usQueueState = (uint16_t)stSOEQueue[ucQueueIndex].ucQueueState;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usHeader;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail;

            }
            else if(SOE_STORAGE_DDR == stSOEQueue[ucQueueIndex].ucStorage)
            {
                bRet = SharedSetDDRSOE(ucQueueIndex, stSOE);
            }
            else
            {
                bRet = false;
            }
            atomic32Set(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK);
            //printf("set soe reset flg=%d\n",stSOEQueue[ucQueueIndex].uiLockFlg);
        }
        else
        {
            bRet = false;
        }
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SharedGetSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数: ucQueueIndex: 队列索引
*        sPos: 队列中的偏移 （0表示从开头读）
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 本次读取后队列已经为空
*        -2 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t SharedGetSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex)
{
    uint32_t uiSoeIndex = READ_SOE_QUEUE_FAILED;
    bool_t bAtomicFlg = false;

    if((ucQueueIndex < MAX_SOE_QUEUE_NUM) && (pstSOE != NULL))
    {
        bAtomicFlg = atomic32Cas(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK, SOE_QUEUE_LOCK);
        //printf("get soe set flg=%d\n",stSOEQueue[ucQueueIndex].uiLockFlg);
        if(true == bAtomicFlg)
        {
            if(SOE_STORAGE_SRAM == stSOEQueue[ucQueueIndex].ucStorage)
            {
                uiSoeIndex = SharedGetSRAMSOE(ucQueueIndex, pstSOE, usLogicPos, puiNextIndex);
            }
            else if(SOE_STORAGE_DDR == stSOEQueue[ucQueueIndex].ucStorage)
            {
                uiSoeIndex = SharedGetDDRSOE(ucQueueIndex, pstSOE, usLogicPos, puiNextIndex);
            }
            else
            {
                uiSoeIndex = READ_SOE_QUEUE_FAILED;
            }
            atomic32Set(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK);
            //printf("get soe reset flg=%d\n",stSOEQueue[ucQueueIndex].uiLockFlg);
        }
        else
        {
            uiSoeIndex = READ_SOE_QUEUE_LOCKED;
        }
    }
    else
    {
        uiSoeIndex = READ_SOE_QUEUE_FAILED;
    }

    return uiSoeIndex;
}

/*
***********************************************************************************************************************
* 函数名称: SharedClearSOE
*
* 功能描述: 清除SOE
*
* 输入参数: ucQueueIndex: 队列索引号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedClearSOE(uint8_t ucQueueIndex)
{
    bool_t bRet = false;
    bool_t bAtomicFlg = false;
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    if(ucQueueIndex < MAX_SOE_QUEUE_NUM)
    {
        bAtomicFlg = atomic32Cas(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK, SOE_QUEUE_LOCK);

        if(true == bAtomicFlg)
        {
            stSOEQueue[ucQueueIndex].usHeader = 0;
            stSOEQueue[ucQueueIndex].usTail = 0;
            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_EMPTY;

            if(SOE_STORAGE_SRAM == stSOEQueue[ucQueueIndex].ucStorage)
            {
                pstSOESRAMQueuesInfoStartAddr->uiFlag = SOE_SRAM_QUEUE_INFO_VALID_FLAG;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usQueueState = (uint16_t)stSOEQueue[ucQueueIndex].ucQueueState;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usHeader;
                pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail;
            }
            bRet = true;
            atomic32Set(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK);
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetHardSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedSetHardSOE(HardSOE_t stSOE)
{
    bool_t bRet = false;
    puint8_t pucDst = NULL;
    uint32_t uiOffsetInSoeArr = 0;
    HardSOE_t* pstSec2StartAddr = (HardSOE_t*)SOE_SRAM_SEC2_START_ADDR;
    HardSOE_t* pstSec3StartAddr = (HardSOE_t*)SOE_SRAM_SEC3_START_ADDR;
    uint8_t ucQueueIndex = HARD_SOE_QUEUE_INDEX;
    bool_t bAtomicFlg=false;
    SOESRAMQueuesInfo_t* pstSOESRAMQueuesInfoStartAddr = (SOESRAMQueuesInfo_t*)SOE_SRAM_QUEUE_INFO_START_ADDR;

    //if(ucQueueIndex < MAX_SOE_QUEUE_NUM)
    {
        bAtomicFlg = atomic32Cas(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK, SOE_QUEUE_LOCK);

        if(true == bAtomicFlg)
        {
            /* 是否被配置 */
            if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
            {
                if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
                {
                    /* 写满后不循环覆盖 */
                    if(SOE_QUEUE_FULL != stSOEQueue[ucQueueIndex].ucQueueState)
                    {
                        /* 未写满 */
                        uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;

                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC3_MAX_INDEX)
                        {
                            if(uiOffsetInSoeArr <= SOE_RECORD_SEC2_MAX_INDEX)
                            {
                                pucDst = (puint8_t)&pstSec2StartAddr[uiOffsetInSoeArr];
                            }
                            else
                            {
                                uiOffsetInSoeArr -= SOE_RECORD_SEC3_START_INDEX;
                                pucDst = (puint8_t)&pstSec3StartAddr[uiOffsetInSoeArr];
                            }
                            memcpy(pucDst, (puint8_t)&stSOE, sizeof(HardSOE_t));
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                            stSOEQueue[ucQueueIndex].usTail++;
                            if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                            {
                                stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                            }
                            //if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                            //{
                            //    stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                            //}
                            /* 更新SRAM中记录的硬SOE信息 */
                            pstSOESRAMQueuesInfoStartAddr->uiFlag = SOE_SRAM_QUEUE_INFO_VALID_FLAG;
                            pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usQueueState = (uint16_t)stSOEQueue[ucQueueIndex].ucQueueState;
                            pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usHeader;
                            pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail;

                            bRet = true;
                        }
                        else
                        {
                            bRet = false;
                        }
                    }
                    else
                    {
                        /* 写满了 */
                        bRet = false;
                    }
                }
                else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
                {
                    uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;
                    if(uiOffsetInSoeArr <= SOE_RECORD_SEC3_MAX_INDEX)
                    {
                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC2_MAX_INDEX)
                        {
                            pucDst = (puint8_t)&pstSec2StartAddr[uiOffsetInSoeArr];
                        }
                        else
                        {
                            uiOffsetInSoeArr -= SOE_RECORD_SEC3_START_INDEX;
                            pucDst = (puint8_t)&pstSec3StartAddr[uiOffsetInSoeArr];
                        }
                        memcpy(pucDst, (puint8_t)&stSOE, sizeof(HardSOE_t));
                        stSOEQueue[ucQueueIndex].usTail++;
                        if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                        {
                            stSOEQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail % stSOEQueue[ucQueueIndex].usSize;
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                        }
                        if(SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState)
                        {
                            stSOEQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usTail;
                        }
                        if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                        {
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                        }

                        /* 更新SRAM中记录的硬SOE信息 */
                        pstSOESRAMQueuesInfoStartAddr->uiFlag = SOE_SRAM_QUEUE_INFO_VALID_FLAG;
                        pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usQueueState = (uint16_t)stSOEQueue[ucQueueIndex].ucQueueState;
                        pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usHeader;
                        pstSOESRAMQueuesInfoStartAddr->stSOESRAMQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail;

                        bRet = true;
                    }
                    else
                    {
                        bRet = false;
                    }
                }
            }
            atomic32Set(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK);
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetHardSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数:
*        usLogicPos: 队列中的偏移  (READ_SOE_FROM_BEGINNING 表示从开头读)
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 本次读取后队列已经为空
*        -2 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t SharedGetHardSOE(HardSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex)
{
    uint32_t uiSoeIndex = READ_SOE_QUEUE_FAILED;
    puint8_t pucSrc = NULL;
    uint32_t uiOffsetInSoeArr = 0;
    uint16_t usLastSoeIndex = 0;
    HardSOE_t* pstSec2StartAddr = (HardSOE_t*)SOE_SRAM_SEC2_START_ADDR;
    HardSOE_t* pstSec3StartAddr = (HardSOE_t*)SOE_SRAM_SEC3_START_ADDR;
    uint8_t ucQueueIndex = HARD_SOE_QUEUE_INDEX;
    bool_t bAtomicFlg=false;

    if((pstSOE != NULL) && (puiNextIndex != NULL))
    {
        bAtomicFlg = atomic32Cas(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK, SOE_QUEUE_LOCK);

        if(true == bAtomicFlg)
        {
            /* 是否被配置 */
            if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
            {
                if(SOE_QUEUE_EMPTY != stSOEQueue[ucQueueIndex].ucQueueState)
                {
                    if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
                    {
                        /* 非空*/
                        if(READ_SOE_FROM_BEGINNING == usLogicPos)
                        {
                            usLogicPos = 0;
                        }
                        uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;
                        usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC3_MAX_INDEX)
                        {
                            if(uiOffsetInSoeArr <= SOE_RECORD_SEC2_MAX_INDEX)
                            {
                                pucSrc = (puint8_t)&pstSec2StartAddr[uiOffsetInSoeArr];
                            }
                            else /*if(uiOffsetInSoeArr <= SOE_RECORD_SEC3_MAX_INDEX)*/
                            {
                                uiOffsetInSoeArr -= SOE_RECORD_SEC1_START_INDEX;
                                pucSrc = (puint8_t)&pstSec3StartAddr[uiOffsetInSoeArr];
                            }

                            if(usLogicPos <= usLastSoeIndex)
                            {
                                memcpy((puint8_t)pstSOE, pucSrc, sizeof(HardSOE_t));
                                if(usLogicPos == usLastSoeIndex)
                                {
                                    uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                                    *puiNextIndex = usLogicPos + 1;
                                }
                                else
                                {
                                    uiSoeIndex = usLogicPos + 1;
                                    *puiNextIndex = usLogicPos + 1;
                                }
                            }
                            else
                            {
                                uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                            }
                        }
                        else
                        {
                            uiSoeIndex = READ_SOE_QUEUE_FAILED;
                        }
                    }
                    else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
                    {
                        /* 从头读 */
                        if(usLogicPos == READ_SOE_FROM_BEGINNING)
                        {
                            usLogicPos = stSOEQueue[ucQueueIndex].usHeader;
                        }

                        if((SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                                && (usLogicPos >= stSOEQueue[ucQueueIndex].usSize))
                        {
                            uiSoeIndex = READ_SOE_QUEUE_FAILED;
                        }
                        else if((SOE_QUEUE_NOT_EMPTY_NOT_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                                &&(usLogicPos >= stSOEQueue[ucQueueIndex].usTail))
                        {
                            uiSoeIndex = READ_SOE_QUEUE_FAILED;
                        }
                        else
                        {
                            uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;

                            if(stSOEQueue[ucQueueIndex].usTail > 0)
                            {
                                usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                            }
                            else
                            {
                                /* tail指向逻辑上的第一个元素 */
                                usLastSoeIndex = stSOEQueue[ucQueueIndex].usSize -1;
                            }

                            if(uiOffsetInSoeArr <= SOE_RECORD_SEC3_MAX_INDEX)
                            {
                                if(uiOffsetInSoeArr <= SOE_RECORD_SEC2_MAX_INDEX)
                                {
                                    pucSrc = (puint8_t)&pstSec2StartAddr[uiOffsetInSoeArr];
                                }
                                else
                                {
                                    uiOffsetInSoeArr -= SOE_RECORD_SEC3_START_INDEX;
                                    pucSrc = (puint8_t)&pstSec3StartAddr[uiOffsetInSoeArr];
                                }

                                if(uiOffsetInSoeArr < (stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usSize))
                                {
                                    memcpy((puint8_t)pstSOE, pucSrc, sizeof(HardSOE_t));

                                    if(usLogicPos == usLastSoeIndex)
                                    {
                                        uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                                        *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                    }
                                    else
                                    {
                                        uiSoeIndex = usLogicPos + 1;
                                        *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                    }
                                }
                                else
                                {
                                    uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                                }
                            }
                            else
                            {
                                uiSoeIndex = READ_SOE_QUEUE_FAILED;
                            }
                        }
                     }
                }
                else
                {
                    /* 空 */
                    uiSoeIndex = SOE_QUEUE_IS_EMPTY_NOW;
                }
            }
            atomic32Set(&stSOEQueue[ucQueueIndex].uiLockFlg, SOE_QUEUE_UNLOCK);
        }
        else
        {
            uiSoeIndex = READ_SOE_QUEUE_LOCKED;
        }
    }
    else
    {
        uiSoeIndex = READ_SOE_QUEUE_FAILED;
    }

    return uiSoeIndex;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedSetDDRSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: ucQueueIndex: 队列索引
*        stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t SharedSetDDRSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE)
{
    bool_t bRet = false;
    puint8_t pucDst = NULL;
    uint32_t uiOffsetInSoeArr = 0;

    if(ucQueueIndex < MAX_SOE_QUEUE_NUM)
    {
        /* 是否被配置 */
        if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
        {
            if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
            {
                /* 写满后不循环覆盖 */
                if(SOE_QUEUE_FULL != stSOEQueue[ucQueueIndex].ucQueueState)
                {
                    /* 未写满 */
                    uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;
                    if(uiOffsetInSoeArr < MAX_SOE_NUM_DDR)
                    {
                        pucDst = (puint8_t)&stSoftSOEArray[uiOffsetInSoeArr];
                        memcpy(pucDst, (puint8_t)&stSOE, sizeof(SoftSOE_t));
                        stSOEQueue[ucQueueIndex].usTail++;
                        if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                        {
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                        }
                        if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                        {
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                        }
                        bRet = true;
                    }
                    else
                    {
                        bRet = false;
                    }
                }
                else
                {
                    /* 写满了 */
                    bRet = false;
                }
            }
            else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
            {
                uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;
                if(uiOffsetInSoeArr < MAX_SOE_NUM_DDR)
                {
                    pucDst = (puint8_t)&stSoftSOEArray[uiOffsetInSoeArr];
                    memcpy(pucDst, (puint8_t)&stSOE, sizeof(SoftSOE_t));
                    stSOEQueue[ucQueueIndex].usTail++;
                    if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                    {
                        stSOEQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail % stSOEQueue[ucQueueIndex].usSize;
                        stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                    }
                    if(SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState)
                    {
                        stSOEQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usTail;
                    }
                    if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                    {
                        stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                    }
                    bRet = true;
                }
                else
                {
                    bRet = false;
                }
            }
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedSetSRAMSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: ucQueueIndex: 队列索引
*        stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t SharedSetSRAMSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE)
{
    bool_t bRet = false;
    puint8_t pucDst = NULL;
    uint32_t uiOffsetInSoeArr = 0;
    SoftSOE_t* pstSec0StartAddr = (SoftSOE_t*)SOE_SRAM_SEC0_START_ADDR;
    SoftSOE_t* pstSec1StartAddr = (SoftSOE_t*)SOE_SRAM_SEC1_START_ADDR;

    if(ucQueueIndex < MAX_SOE_QUEUE_NUM)
    {
        /* 是否被配置 */
        if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
        {
            if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
            {
                /* 写满后不循环覆盖 */
                if(SOE_QUEUE_FULL != stSOEQueue[ucQueueIndex].ucQueueState)
                {
                    /* 未写满 */
                    uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;

                    if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)
                    {
                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC0_MAX_INDEX)
                        {
                            pucDst = (puint8_t)&pstSec0StartAddr[uiOffsetInSoeArr];
                        }
                        else /*if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)*/
                        {
                            uiOffsetInSoeArr -= SOE_RECORD_SEC1_START_INDEX;
                            pucDst = (puint8_t)&pstSec1StartAddr[uiOffsetInSoeArr];
                        }
                        memcpy(pucDst, (puint8_t)&stSOE, sizeof(SoftSOE_t));
                        stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                        stSOEQueue[ucQueueIndex].usTail++;
                        if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                        {
                            stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                        }
                        //if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                        //{
                        //    stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                        //}
                        bRet = true;
                    }
                    else
                    {
                        bRet = false;
                    }
                }
                else
                {
                    /* 写满了 */
                    bRet = false;
                }
            }
            else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
            {
                uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usTail;
                if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)
                {
                    if(uiOffsetInSoeArr <= SOE_RECORD_SEC0_MAX_INDEX)
                    {
                        pucDst = (puint8_t)&pstSec0StartAddr[uiOffsetInSoeArr];
                    }
                    else //if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)
                    {
                        uiOffsetInSoeArr -= SOE_RECORD_SEC1_START_INDEX;
                        pucDst = (puint8_t)&pstSec1StartAddr[uiOffsetInSoeArr];
                    }
                    memcpy(pucDst, (puint8_t)&stSOE, sizeof(SoftSOE_t));
                    stSOEQueue[ucQueueIndex].usTail++;
                    if(stSOEQueue[ucQueueIndex].usTail >= stSOEQueue[ucQueueIndex].usSize)
                    {
                        stSOEQueue[ucQueueIndex].usTail = stSOEQueue[ucQueueIndex].usTail % stSOEQueue[ucQueueIndex].usSize;
                        stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_FULL;
                    }
                    if(SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState)
                    {
                        stSOEQueue[ucQueueIndex].usHeader = stSOEQueue[ucQueueIndex].usTail;
                    }
                    if(SOE_QUEUE_EMPTY == stSOEQueue[ucQueueIndex].ucQueueState)
                    {
                        stSOEQueue[ucQueueIndex].ucQueueState = SOE_QUEUE_NOT_EMPTY_NOT_FULL;
                    }
                    bRet = true;
                }
                else
                {
                    bRet = false;
                }
            }
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetDDRSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数: ucQueueIndex: 队列索引
*        sPos: 队列中的偏移 （0表示从开头读）
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 本次读取后队列已经为空
*        -2 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t SharedGetDDRSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex)
{
    uint32_t uiSoeIndex = READ_SOE_QUEUE_FAILED;
    puint8_t pucSrc = NULL;
    uint32_t uiOffsetInSoeArr = 0;
    uint16_t usLastSoeIndex = 0;

    if((ucQueueIndex < MAX_SOE_QUEUE_NUM) && (pstSOE != NULL) && (puiNextIndex != NULL))
    {
        /* 是否被配置 */
        if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
        {
            if(SOE_QUEUE_EMPTY != stSOEQueue[ucQueueIndex].ucQueueState)
            {
                if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
                {
                    /* 非空*/
                    if(READ_SOE_FROM_BEGINNING == usLogicPos)
                    {
                        usLogicPos = 0;
                    }
                    uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;
                    usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                    if((usLogicPos <= usLastSoeIndex) && (uiOffsetInSoeArr < MAX_SOE_NUM_DDR))
                    {
                        pucSrc = (puint8_t)&stSoftSOEArray[uiOffsetInSoeArr];
                        memcpy((puint8_t)pstSOE, pucSrc, sizeof(SoftSOE_t));
                        if(usLogicPos == usLastSoeIndex)
                        {
                            uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                            *puiNextIndex = usLogicPos + 1;
                        }
                        else
                        {
                            uiSoeIndex = usLogicPos + 1;
                            *puiNextIndex = usLogicPos + 1;
                        }
                    }
                    else
                    {
                        uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                    }
                }
                else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
                {
                    /* 从头读 */
                    if(usLogicPos == READ_SOE_FROM_BEGINNING)
                    {
                        usLogicPos = stSOEQueue[ucQueueIndex].usHeader;
                    }

                    if((SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                            && (usLogicPos >= stSOEQueue[ucQueueIndex].usSize))
                    {
                        uiSoeIndex = READ_SOE_QUEUE_FAILED;
                    }
                    else if((SOE_QUEUE_NOT_EMPTY_NOT_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                            &&(usLogicPos >= stSOEQueue[ucQueueIndex].usTail))
                    {
                        uiSoeIndex = READ_SOE_QUEUE_FAILED;
                    }
                    else
                    {
                        uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;

                        if(stSOEQueue[ucQueueIndex].usTail > 0)
                        {
                            usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                        }
                        else
                        {
                            /* tail指向逻辑上的第一个元素 */
                            usLastSoeIndex = stSOEQueue[ucQueueIndex].usSize -1;
                        }

                        if(uiOffsetInSoeArr < (stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usSize))
                        {
                            pucSrc = (puint8_t)&stSoftSOEArray[uiOffsetInSoeArr];
                            memcpy((puint8_t)pstSOE, pucSrc, sizeof(SoftSOE_t));

                            if(usLogicPos == usLastSoeIndex)
                            {
                                uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                                *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                            }
                            else
                            {
                                uiSoeIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                            }
                        }
                        else
                        {
                            uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                        }
                    }
                 }
            }
            else
            {
                /* 空 */
                uiSoeIndex = SOE_QUEUE_IS_EMPTY_NOW;
            }

        }
    }
    else
    {
        uiSoeIndex = READ_SOE_QUEUE_FAILED;
    }

    return uiSoeIndex;
}

/*
***********************************************************************************************************************
* 函数名称: SharedGetSRAMSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数: ucQueueIndex: 队列索引
*        sPos: 队列中的偏移 （0表示从开头读）
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 本次读取后队列已经为空
*        -2 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t SharedGetSRAMSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex)
{
    uint32_t uiSoeIndex = READ_SOE_QUEUE_FAILED;
    puint8_t pucSrc = NULL;
    uint32_t uiOffsetInSoeArr = 0;
    uint16_t usLastSoeIndex = 0;
    SoftSOE_t* pstSec0StartAddr = (SoftSOE_t*)SOE_SRAM_SEC0_START_ADDR;
    SoftSOE_t* pstSec1StartAddr = (SoftSOE_t*)SOE_SRAM_SEC1_START_ADDR;

    if((ucQueueIndex < MAX_SOE_QUEUE_NUM) && (pstSOE != NULL) && (puiNextIndex != NULL))
    {
        /* 是否被配置 */
        if(SOE_QUEUE_CONFIGED == stSOEQueue[ucQueueIndex].bConfiged)
        {
            if(SOE_QUEUE_EMPTY != stSOEQueue[ucQueueIndex].ucQueueState)
            {
                if(SOE_QUEUE_TYPE_HISTORY == stSOEQueue[ucQueueIndex].usType)
                {
                    /* 非空*/
                    if(READ_SOE_FROM_BEGINNING == usLogicPos)
                    {
                        usLogicPos = 0;
                    }
                    uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;
                    usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                    if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)
                    {
                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC0_MAX_INDEX)
                        {
                            pucSrc = (puint8_t)&pstSec0StartAddr[uiOffsetInSoeArr];
                        }
                        else /*if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)*/
                        {
                            uiOffsetInSoeArr -= SOE_RECORD_SEC1_START_INDEX;
                            pucSrc = (puint8_t)&pstSec1StartAddr[uiOffsetInSoeArr];
                        }

                        if(usLogicPos <= usLastSoeIndex)
                        {
                            memcpy((puint8_t)pstSOE, pucSrc, sizeof(SoftSOE_t));
                            if(usLogicPos == usLastSoeIndex)
                            {
                                uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                                *puiNextIndex = usLogicPos + 1;
                            }
                            else
                            {
                                /* 返回下一条soe索引 */
                                uiSoeIndex = usLogicPos + 1;
                                *puiNextIndex = usLogicPos + 1;
                            }
                        }
                        else
                        {
                            uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                        }
                    }
                    else
                    {
                        uiSoeIndex = READ_SOE_QUEUE_FAILED;
                    }
                }
                else /* SOE_QUEUE_TYPE_CYCLE_FIFO */
                {
                    /* 从头读 */
                    if(usLogicPos == READ_SOE_FROM_BEGINNING)
                    {
                        usLogicPos = stSOEQueue[ucQueueIndex].usHeader;
                    }

                    if((SOE_QUEUE_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                            && (usLogicPos >= stSOEQueue[ucQueueIndex].usSize))
                    {
                        uiSoeIndex = READ_SOE_QUEUE_FAILED;
                    }
                    else if((SOE_QUEUE_NOT_EMPTY_NOT_FULL == stSOEQueue[ucQueueIndex].ucQueueState) \
                            &&(usLogicPos >= stSOEQueue[ucQueueIndex].usTail))
                    {
                        uiSoeIndex = READ_SOE_QUEUE_FAILED;
                    }
                    else
                    {
                        uiOffsetInSoeArr = stSOEQueue[ucQueueIndex].uiBase + usLogicPos;

                        if(stSOEQueue[ucQueueIndex].usTail > 0)
                        {
                            usLastSoeIndex = stSOEQueue[ucQueueIndex].usTail -1;
                        }
                        else
                        {
                            /* tail指向逻辑上的第一个元素 */
                            usLastSoeIndex = stSOEQueue[ucQueueIndex].usSize -1;
                        }

                        if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)
                        {
                            if(uiOffsetInSoeArr <= SOE_RECORD_SEC0_MAX_INDEX)
                            {
                                pucSrc = (puint8_t)&pstSec0StartAddr[uiOffsetInSoeArr];
                            }
                            else /*if(uiOffsetInSoeArr <= SOE_RECORD_SEC1_MAX_INDEX)*/
                            {
                                uiOffsetInSoeArr -= SOE_RECORD_SEC1_START_INDEX;
                                pucSrc = (puint8_t)&pstSec1StartAddr[uiOffsetInSoeArr];
                            }

                            if(uiOffsetInSoeArr < (stSOEQueue[ucQueueIndex].uiBase + stSOEQueue[ucQueueIndex].usSize))
                            {
                                memcpy((puint8_t)pstSOE, pucSrc, sizeof(SoftSOE_t));

                                if(usLogicPos == usLastSoeIndex)
                                {
                                    uiSoeIndex = THE_LAST_SOE_IN_QUEUE;
                                    *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                }
                                else
                                {
                                    uiSoeIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                    *puiNextIndex = (usLogicPos + 1) % stSOEQueue[ucQueueIndex].usSize;
                                }
                            }
                            else
                            {
                                uiSoeIndex = READ_SOE_QUEUE_NO_NEW_RECORD;
                            }
                        }
                        else
                        {
                            uiSoeIndex = READ_SOE_QUEUE_FAILED;
                        }
                    }
                 }
            }
            else
            {
                /* 空 */
                uiSoeIndex = SOE_QUEUE_IS_EMPTY_NOW;
            }

        }
    }
    else
    {
        uiSoeIndex = READ_SOE_QUEUE_FAILED;
    }

    return uiSoeIndex;
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
