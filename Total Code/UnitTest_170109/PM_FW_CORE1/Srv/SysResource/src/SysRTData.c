/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : SysRTData.c
*
*  功能简介    :
*
*  作者        : 李琦
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
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/Sys.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
/* 实时数据区 */
static RTDataArea_t s_stRTDataArea[NUM_OF_RTDATA_AREA];
static uint32_t s_uiG1Offset;
static uint32_t s_uiG2Offset;
static SharedAppend_t s_stAppendInfo;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysRTDataInit
*
* 功能描述: 静态变量初始化
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
void SysRTDataInit(void)
{
    /* 初始化：数据区未加锁，只写操作，写偏移地址为0 */
    memset((puint8_t)&s_stRTDataArea[0], 0, sizeof(RTDataArea_t)*NUM_OF_RTDATA_AREA);
    s_uiG1Offset = 0;
    s_uiG2Offset = 0;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysReadRTData
*
* 功能描述: 读取实时数据
*
* 输入参数: emDataArea:数据区类型
*        usLen: 预读取的实时数据长度
*
* 输出参数: pucContent: 指向实时数据内容
*
* 返 回 值  : true-读取成功
*        false-参数有误或者读取失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysReadRTData(SysRtDataAreaType_t emDataArea, puint8_t pucContent, uint16_t usLen, puint16_t pusRdLen)
{
    bool_t bRet = false;                        /* 返回值 */
    uint32_t uiReadOff = 0;                     /* 读偏移地址 */
    puint8_t pucSrc = NULL;                     /* 读取数据的源地址 */
    RTDataAreaIndex_t emIndex = INVALID_RTDATA_AREA_INDEX;
    uint32_t uiBaseOffset = 0;


    /* 参数合法性检查 */
    if((emDataArea < MAX_RTDATA_AREA_NUM) && (pucContent != NULL) && (usLen > 0) && (pusRdLen!=NULL))
    {
        /* 未加锁且为只读区域 */
        if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_A;
            s_stRTDataArea[emIndex].bLock = RTDATA_AREA_LOCKED;
            //printf("RTDATA_AREA_A\n");
        }
        else
        {
            emIndex = INVALID_RTDATA_AREA_INDEX;
            *pusRdLen = 0;
            //printf("INVALID_RTDATA_AREA_INDEX\n");
        }

        if(emIndex != INVALID_RTDATA_AREA_INDEX)
        {
            /* 获取读偏移地址 */
            uiReadOff = s_stRTDataArea[emIndex].uiReadOffset[emDataArea];

            if((RTDATA_UP1_G_AREA == emDataArea) || (RTDATA_UP2_G_AREA == emDataArea))
            {
                if(uiReadOff < MAX_RTDATA_G_AREA_LEN)
                {
                    pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucContent[uiReadOff]);
                }
                if(RTDATA_UP1_G_AREA == emDataArea)
                {
                    uiBaseOffset = s_uiG1Offset;
                }
                else
                {
                    uiBaseOffset = s_uiG2Offset;
                }
            }
            else if(RTDATA_I_AREA == emDataArea)
            {
                if(uiReadOff < MAX_RTDATA_I_AREA_LEN)
                {
                    pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucInputContent[uiReadOff]);
                }
                uiBaseOffset = 0;
            }
            else if(RTDATA_Q_AREA == emDataArea) 
            {
                if(uiReadOff < MAX_RTDATA_Q_AREA_LEN)
                {
                    pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucOutputContent[uiReadOff]);
                }
                uiBaseOffset = 0;
            }
            else if(RTDATA_APPEND_AREA == emDataArea)
            {
                if(uiReadOff < MAX_RTDATA_APPEND_INFO_SIZE)
                {
                    pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucAppendInfo[uiReadOff]);
                }
                uiBaseOffset = 0;
            }
            else
            {
                /* 此情况不会发生 */
                pucSrc = NULL;
                s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;
            }

            if(pucSrc != NULL)
            {
                //printf("rdoff=%d base=%d index=%d len=%d size=%d area=%d\n", uiReadOff, uiBaseOffset, emIndex, \
                //        usLen,s_stRTDataArea[emIndex].uiLen[emDataArea], emDataArea);
                if(uiReadOff + usLen < uiBaseOffset + s_stRTDataArea[emIndex].uiLen[emDataArea])
                {
                    memcpy(pucContent, pucSrc, usLen);
                    s_stRTDataArea[emIndex].uiReadOffset[emDataArea] += usLen;
                    s_stRTDataArea[emIndex].bReadFlag[emDataArea] = RTDATA_AREA_READ_NOT_FINISHED;
                }
                else
                {
                    usLen = uiBaseOffset + s_stRTDataArea[emIndex].uiLen[emDataArea] - uiReadOff;
                    memcpy(pucContent, pucSrc, usLen);
                    s_stRTDataArea[emIndex].bReadFlag[emDataArea] = RTDATA_AREA_READ_FINISHED;
                }
                *pusRdLen = usLen;

                s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;

                /* 返回读取成功 */
                bRet = true;
            }
            else
            {
                *pusRdLen = 0;
                bRet = false;
            }
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysReadRTDataReset
*
* 功能描述: 复位读操作
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : true-操作成功
*        false-操作失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysReadRTDataReset(void)
{
    bool_t bRet = false;
    RTDataAreaIndex_t emIndex = INVALID_RTDATA_AREA_INDEX;

    /* 未加锁且为只读区域 */
    if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
        &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus))
    {
        emIndex = RTDATA_AREA_A;
    }
    else
    {
        emIndex = INVALID_RTDATA_AREA_INDEX;
    }

    if(emIndex != INVALID_RTDATA_AREA_INDEX)
    {
        /* 复位读偏移地址 */
        s_stRTDataArea[emIndex].uiReadOffset[RTDATA_UP1_G_AREA] = s_uiG1Offset;
        s_stRTDataArea[emIndex].uiReadOffset[RTDATA_UP2_G_AREA] = s_uiG2Offset;
        s_stRTDataArea[emIndex].uiReadOffset[RTDATA_I_AREA] = 0;
        s_stRTDataArea[emIndex].uiReadOffset[RTDATA_Q_AREA] = 0;
        s_stRTDataArea[emIndex].uiReadOffset[RTDATA_APPEND_AREA] = 0;
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataReadOffset
*
* 功能描述: 获取实时数据区的读偏移地址
*
* 输入参数: emDataArea: 数据区类型
*
* 输出参数: puiOffset：指向读偏移地址
*
* 返 回 值  : true-读取成功
*        false- 失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataReadOffset(SysRtDataAreaType_t emDataArea, puint32_t puiOffset)
{
    bool_t bRet = false;
    uint32_t uiReadOff = 0;

    if((emDataArea < MAX_RTDATA_AREA_NUM) && (puiOffset != NULL))
    {
        /* 未加锁且为只读区域 */
        if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus))
        {
            /* 获取读偏移地址 */
            uiReadOff = s_stRTDataArea[RTDATA_AREA_A].uiReadOffset[emDataArea];
            *puiOffset = uiReadOff;
            /*读取成功*/
            bRet = true;
        }
        else
        {
            bRet = false;
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
* 函数名称: SysGetRTDataReadFlag
*
* 功能描述: 相应数据区是否读取完毕
*
* 输入参数: emDataArea: 数据区类型
*
* 输出参数: pbReadFlag：指向读取是否完毕标志
*
* 返 回 值  : true-读取成功
*        false- 失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetRTDataReadFlag(SysRtDataAreaType_t emDataArea, bool_t *pbReadFlag)
{
    bool_t bRet = false;
    uint32_t bFlag = 0;

    if((emDataArea < MAX_RTDATA_AREA_NUM) && (pbReadFlag != NULL))
    {
        /* 未加锁且为只读区域 */
        if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus))
        {
            /* 获取读偏移地址 */
            bFlag = s_stRTDataArea[RTDATA_AREA_A].bReadFlag[emDataArea];
            *pbReadFlag = bFlag;
#if 0
            if((RTDATA_AREA_READ_FINISHED == bFlag) && (emDataArea>=RTDATA_APPEND_AREA))
            {
                s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
                printf("read A rt data area finished\n");
            }
#endif
            bRet = true;
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
* 函数名称: SysCopySharedRtDataToCore1RtData
*
* 功能描述: 拷贝数据（SharedRtdata -> Core1RtData）
*
* 输入参数: emDataArea
*               uiOffset
*               uiSize
*
* 输出参数: 无
*
* 返 回 值  : true: 允许访问共享内存，拷贝成功
*        false: 不允许访问共享内存，拷贝失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysCopySharedRtDataToCore1RtData(SysRtDataAreaType_t emDataArea, uint32_t uiOffset, uint32_t uiSize)
{
    bool_t bRet = false;
    RTDataAreaIndex_t emIndex = INVALID_RTDATA_AREA_INDEX;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    bool_t bResult = false;
    SysState_t stSysState;
    //ulong64_t ulTime = 0;

    //printf("S lock=%d\n",g_stSharedRtData.bLock);

    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_UNLOCKED, \
                                   (uint32_t)RTDATA_AREA_LOCKED);

    //bResult = true;
    //printf("A lock=%d\n",s_stRTDataArea[RTDATA_AREA_A].bLock);
    //printf("A DataAreaStatus=%d\n",s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus);
    //printf("bResult=%d\n",bResult);
    
    //printf("B lock=%d\n",s_stRTDataArea[RTDATA_AREA_B].bLock);
    //printf("B DataAreaStatus=%d\n",s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus);
    
    /* 从core1的两个实时数据区中找到未加锁且为只写的区域 */
    if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
        && (RTDATA_AREA_WRITE_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus) \
        && (true == bResult))
    {
        emIndex = RTDATA_AREA_A;
        s_stRTDataArea[emIndex].bLock = RTDATA_AREA_LOCKED;
        //printf("A write only\n");
    }
    else
    {
        emIndex = INVALID_RTDATA_AREA_INDEX;
        //printf("invalid --\n");
    }

    if(emIndex != INVALID_RTDATA_AREA_INDEX)
    {
        switch(emDataArea)
        {
            case RTDATA_UP1_G_AREA:
            {
                /* 拷贝UP1 G区数据 */
                s_uiG1Offset = uiOffset;
                pucSrc = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucContent[uiOffset]);
                memcpy(pucDst, pucSrc, uiSize);

                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_UP1_G_AREA] = uiOffset;
                s_stRTDataArea[emIndex].uiLen[RTDATA_UP1_G_AREA] = uiSize;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_UP1_G_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
                //printf("up1 Gsize %d\n", uiSize);
            }
            break;
        
            case RTDATA_UP2_G_AREA:
            {
                /* 拷贝UP2 G区数据 */
                s_uiG2Offset = uiOffset;
                pucSrc = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucContent[uiOffset]);
                memcpy(pucDst, pucSrc, uiSize);

                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_UP2_G_AREA] = uiOffset;
                s_stRTDataArea[emIndex].uiLen[RTDATA_UP2_G_AREA] = uiSize;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_UP2_G_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;
            
            case RTDATA_I_AREA:
            {
                /* 拷贝 I区数据 */
                pucSrc = (puint8_t) &(g_stSharedRtData.ucI[0]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucInputContent[0]);
                memcpy(pucDst, pucSrc, MAX_RTDATA_I_AREA_LEN);
                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_I_AREA] = 0;
                s_stRTDataArea[emIndex].uiLen[RTDATA_I_AREA] = MAX_RTDATA_I_AREA_LEN;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_I_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;
            
            case RTDATA_Q_AREA:
            {
                /* 拷贝 Q区数据 */
                pucSrc = (puint8_t) &(g_stSharedRtData.ucQ[0]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucOutputContent[0]);
                memcpy(pucDst, pucSrc, MAX_RTDATA_Q_AREA_LEN);

                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_Q_AREA] = 0;
                s_stRTDataArea[emIndex].uiLen[RTDATA_Q_AREA] = MAX_RTDATA_Q_AREA_LEN;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_Q_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;

            case RTDATA_APPEND_AREA:
            {
                /* 临时在此拷贝PMstate中的工程版本 */
                SysGetSysState(&stSysState);
                g_stSharedRtData.stAppend.stPMState.stPMFirmwareState.uiProjID = stSysState.stPrjInfo.uiPrjID;
                g_stSharedRtData.stAppend.stPMState.stPMFirmwareState.uiMainVer = stSysState.stPrjInfo.uiMainVern;
                g_stSharedRtData.stAppend.stPMState.stPMFirmwareState.uiMinorVer = s_stAppendInfo.uiMinorVer = stSysState.stPrjInfo.uiMinorVern;

                memcpy((puint8_t)&(s_stAppendInfo), (puint8_t)&(g_stSharedRtData.stAppend), sizeof(SharedAppend_t));

                s_stAppendInfo.uiProjCRC = stSysState.stPrjInfo.uiPrjCRC;
                s_stAppendInfo.uiMainVer = stSysState.stPrjInfo.uiMainVern;
                s_stAppendInfo.uiMinorVer = stSysState.stPrjInfo.uiMinorVern;
                s_stAppendInfo.uiProjID = stSysState.stPrjInfo.uiPrjID;
                s_stAppendInfo.ulTime = SysGetSystemtime();

//                SysGetCMVer(CM1, (puint32_t)&s_stAppendInfo.stCMVern[CM1].stCMFirmwareVern.ucRTSVern);
//                SysGetCMVer(CM2, (puint32_t)&s_stAppendInfo.stCMVern[CM2].stCMFirmwareVern.ucRTSVern);
//                SysGetCMVer(CM3, (puint32_t)&s_stAppendInfo.stCMVern[CM3].stCMFirmwareVern.ucRTSVern);
//                SysGetCMVer(CM4, (puint32_t)&s_stAppendInfo.stCMVern[CM4].stCMFirmwareVern.ucRTSVern);

                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucAppendInfo[0]);
                memcpy(pucDst, (puint8_t)&(s_stAppendInfo), sizeof(SharedAppend_t));

#if 0
                /* 写入实时数据版本 */
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucAppendInfo[0]);
                SysGetSysState(&stSysState);
                memcpy(pucDst, (puint8_t)&(stSysState.stPrjInfo), sizeof(PrjInfo_t));
                //printf("prj crc=0x%x\n",stSysState.stPrjInfo.uiPrjCRC);

                /* 写入时间 */
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucAppendInfo[sizeof(PrjInfo_t)]);
                ulTime = SysGetSystemtime();
                memcpy(pucDst, (puint8_t)&(ulTime), sizeof(ulong64_t));
                //printf("set timestamp=0x%x %x %x %x %x %x %x %x\n",pucDst[0],pucDst[1],pucDst[2],pucDst[3],pucDst[4],pucDst[5],pucDst[6],pucDst[7]);
#endif
                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_APPEND_AREA] = 0;
                s_stRTDataArea[emIndex].uiLen[RTDATA_APPEND_AREA] = sizeof(SharedAppend_t);//MAX_RTDATA_APPEND_INFO_SIZE;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_APPEND_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;

            default:
            break;
        }

        /* 原子操作：解除锁标志 */
        //atomic32Set((puint32_t)&(g_stSharedRtData.bLock), (uint32_t)RTDATA_AREA_UNLOCKED);

        s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;

        bRet = true;
    }

    if(true == bResult)
    {
        atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_LOCKED, \
                                   (uint32_t)RTDATA_AREA_UNLOCKED);
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetCopyRtDataFinished
*
* 功能描述: SharedRtdata -> Core1RtData
*
* 输入参数: 
*
* 输出参数: 无
*
* 返 回 值  : 
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetCopyRtDataFinished(void)
{
    if(RTDATA_AREA_WRITE_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus)
    {
        s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
        //printf("A readonly\n");
    }
    else
    {
        //printf("other\n");
    }
}

/*
***********************************************************************************************************************
* 函数名称: SysSetRtDataAreaWriteOnly
*
* 功能描述: 将整个实时数据区置为只写标志
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SysSetRtDataAreaWriteOnly(void)
{
    s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
