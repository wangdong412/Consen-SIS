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

//static uint8_t ucTmp = 0;
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
        else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_B].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_B;
            s_stRTDataArea[emIndex].bLock = RTDATA_AREA_LOCKED;
            //printf("RTDATA_AREA_B\n");
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
                pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucContent[uiReadOff]);
            }
            else if(RTDATA_I_AREA == emDataArea)
            {
                pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucInputContent[uiReadOff]);
            }
            else if(RTDATA_Q_AREA == emDataArea) 
            {
                pucSrc = (puint8_t)(&s_stRTDataArea[emIndex].ucOutputContent[uiReadOff]);
            }
            else
            {
                /* 此情况不会发生 */
                pucSrc = NULL;
                s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;
            }

            if(pucSrc != NULL)
            {
                if((uiReadOff + usLen) < s_stRTDataArea[emIndex].uiLen[emDataArea])
                {
                    memcpy(pucContent, pucSrc, usLen);
                    s_stRTDataArea[emIndex].uiReadOffset[emDataArea] += usLen;
                    s_stRTDataArea[emIndex].bReadFlag[emDataArea] = RTDATA_AREA_READ_NOT_FINISHED;
                }
                else
                {
                    usLen = s_stRTDataArea[emIndex].uiLen[emDataArea] - uiReadOff;
                    memcpy(pucContent, pucSrc, usLen);
                    s_stRTDataArea[emIndex].bReadFlag[emDataArea] = RTDATA_AREA_READ_FINISHED;
                }
                *pusRdLen = usLen;

                s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;

                /* 返回读取成功 */
                bRet = true;
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
    //uint32_t uiOffset = 0;

    /* 未加锁且为只读区域 */
    if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_A].bLock) \
        &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus))
    {
        emIndex = RTDATA_AREA_A;
    }
    else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_B].bLock) \
        &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus))
    {
        emIndex = RTDATA_AREA_B;
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
        else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_B].bLock) \
                &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus))
        {
            /* 获取读偏移地址 */
            uiReadOff = s_stRTDataArea[RTDATA_AREA_B].uiReadOffset[emDataArea];
            *puiOffset = uiReadOff;
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

            if((RTDATA_AREA_READ_FINISHED == bFlag) && (emDataArea>=RTDATA_Q_AREA))
            {
                s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
                //printf("read A rt data area finished\n");
            }
            bRet = true;
        }
        else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_B].bLock) \
                &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus))
        {
            /* 获取读偏移地址 */
            bFlag = s_stRTDataArea[RTDATA_AREA_B].bReadFlag[emDataArea];
            *pbReadFlag = bFlag;

            if((RTDATA_AREA_READ_FINISHED == bFlag) && (emDataArea>=RTDATA_Q_AREA))
            {
                s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
                //printf("read B rt data area finished\n");
            }

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

    //printf("S lock=%d\n",g_stSharedRtData.bLock);
#if 0
    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_UNLOCKED, \
                                   (uint32_t)RTDATA_AREA_LOCKED);
#endif
    bResult = true;
    //printf("A lock=%d\n",s_stRTDataArea[RTDATA_AREA_A].bLock);
    //printf("A DataAreaStatus=%d\n",s_stRTDataArea[RTDATA_AREA_A].bDataAreaStatus);
    ////printf("bResult=%d\n",bResult);
    
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
    else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[RTDATA_AREA_B].bLock) \
        && (RTDATA_AREA_WRITE_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus) \
        && (true == bResult))
    {
        emIndex = RTDATA_AREA_B;
        s_stRTDataArea[emIndex].bLock = RTDATA_AREA_LOCKED;
        //printf("B write only\n");
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
                //test start
#if 0
                s_stRTDataArea[emIndex].ucContent[0] = 0+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[1] = 1+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[2] = 2+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[3] = 3+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[4] = 4+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[5] = 5+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[6] = 6+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[7] = 7+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[8] = 8+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[9] = 9+emIndex*128;
                
                s_stRTDataArea[emIndex].ucContent[10] = 10+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[11] = 11+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[12] = 12+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[13] = 13+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[14] = 14+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[15] = 15+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[16] = 16+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[17] = 17+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[18] = 18+emIndex*128;
                s_stRTDataArea[emIndex].ucContent[19] = 19+emIndex*128;
#endif
                //test end
                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_UP1_G_AREA] = uiOffset;
                s_stRTDataArea[emIndex].uiLen[RTDATA_UP1_G_AREA] = uiSize;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_UP1_G_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
                //printf("copy up1 G\n");
            }
            break;
        
            case RTDATA_UP2_G_AREA:
            {
                /* 拷贝UP2 G区数据 */
                s_uiG2Offset = 0;
                pucSrc = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucContent[uiOffset]);
                memcpy(pucDst, pucSrc, uiSize);
                
                //test start
#if 0
                s_stRTDataArea[emIndex].ucContent[0] = 0+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[1] = 1+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[2] = 2+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[3] = 3+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[4] = 4+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[5] = 5+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[6] = 6+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[7] = 7+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[8] = 8+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[9] = 9+emIndex*128 + ucTmp;
                
                s_stRTDataArea[emIndex].ucContent[10] = 10+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[11] = 11+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[12] = 12+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[13] = 13+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[14] = 14+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[15] = 15+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[16] = 16+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[17] = 17+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[18] = 18+emIndex*128 + ucTmp;
                s_stRTDataArea[emIndex].ucContent[19] = 19+emIndex*128 + ucTmp;
                ucTmp++;
#endif
                //test end
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
                s_stRTDataArea[emIndex].uiLen[RTDATA_I_AREA] = 512;//MAX_RTDATA_I_AREA_LEN;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_I_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;
            
            case RTDATA_Q_AREA:
            {
                /* 拷贝 Q区数据 */
                pucSrc = (puint8_t) &(g_stSharedRtData.ucQ[0]);
                pucDst = (puint8_t) &(s_stRTDataArea[emIndex].ucOutputContent[0]);
                memcpy(pucDst, pucSrc, uiSize);
                s_stRTDataArea[emIndex].uiReadOffset[RTDATA_Q_AREA] = 0;
                s_stRTDataArea[emIndex].uiLen[RTDATA_Q_AREA] = 512;//MAX_RTDATA_Q_AREA_LEN;
                s_stRTDataArea[emIndex].bReadFlag[RTDATA_Q_AREA] = RTDATA_AREA_READ_NOT_FINISHED;
            }
            break;
            
            default:
            break;
        }

        /* 原子操作：解除锁标志 */
        atomic32Set((puint32_t)&(g_stSharedRtData.bLock), (uint32_t)RTDATA_AREA_UNLOCKED);
        //s_stRTDataArea[emIndex].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
        s_stRTDataArea[emIndex].bLock = RTDATA_AREA_UNLOCKED;

        bRet = true;
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
    else if(RTDATA_AREA_WRITE_ONLY == s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus)
    {
        s_stRTDataArea[RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
        //printf("B readonly\n");
    }
    else
    {
        //printf("other\n");
    }
}



/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
