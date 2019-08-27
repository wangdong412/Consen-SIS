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
*  文件名称    : SysCore0RtData.c
*
*  功能简介    : core0上的实时数据
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-20
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
#include "../inc/SysCore0RtData.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
/* 下面两个变量放在一起 */
static PRG_RTDataArea_t s_stPRGRtData;
static PRG_RTDataArea_t s_stRtDataDisableFlag;

static PMABC_IO_t s_stPMIOData;

static RTDataExchange_t s_stBackupUP1Var;
static RTDataExchange_t s_stBackupUP2Var;

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
* 函数名称: SysCore0RtDataInit
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
void SysCore0RtDataInit(void)
{
    /* 初始化core0的用于运算的实时数据区 */
    memset((puint8_t) &s_stPRGRtData,0 , sizeof(PRG_RTDataArea_t));
    memset((puint8_t) &s_stRtDataDisableFlag,0 , sizeof(PRG_RTDataArea_t));

    memset((puint8_t) &s_stPMIOData,0 , sizeof(PMABC_IO_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPRGRtDataStartAddr
*
* 功能描述: 获取运算的实时数据区首地址
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
puint8_t SysGetPRGRtDataStartAddr(SysRtDataType_t emDataType)
{
    puint8_t pStartAddr = NULL;

    switch(emDataType)
    {
        case RT_DATA_TYPE_G:
        {
           pStartAddr = &(s_stPRGRtData.ucG[0]);
        }
        break;

        case RT_DATA_TYPE_I:
        {
           pStartAddr = &(s_stPRGRtData.ucI[0]);
        }
        break;

        case RT_DATA_TYPE_Q:
        {
           pStartAddr = &(s_stPRGRtData.ucQ[0]);
        }
        break;

        default:
        break;
    }

    return pStartAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRtDataDisableFlagStartAddr
*
* 功能描述: 获取标志区首地址
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
puint8_t SysGetRtDataDisableFlagStartAddr(SysRtDataType_t emDataType)
{
    puint8_t pStartAddr = NULL;
    
    switch(emDataType)
    {
        case RT_DATA_TYPE_G:
        {
           pStartAddr = &(s_stRtDataDisableFlag.ucG[0]);
        }
        break;

        case RT_DATA_TYPE_I:
        {
           pStartAddr = &(s_stRtDataDisableFlag.ucI[0]);
        }
        break;

        case RT_DATA_TYPE_Q:
        {
           pStartAddr = &(s_stRtDataDisableFlag.ucQ[0]);
        }
        break;

        default:
        break;
    }

    return pStartAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMIODataStartAddr
*
* 功能描述: 获取相应的PM IO数据区首地址
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
puint8_t SysGetPMIODataStartAddr(PMController_t emPMID, SysRtDataType_t emDataType)
{
    puint8_t pStartAddr = NULL;

    if(emPMID < NUM_OF_PM)
    {
        switch(emDataType)
        {
            case RT_DATA_TYPE_I:
            {
               pStartAddr = &(s_stPMIOData.ucI[emPMID][0]);
            }
            break;

            case RT_DATA_TYPE_Q:
            {
               pStartAddr = &(s_stPMIOData.ucQ[emPMID][0]);
            }
            break;

            default:
            break;
        }
    }

    return pStartAddr;
}



/*
***********************************************************************************************************************
* 函数名称: SysCopyCore0RtDataToSharedRtDataUP1
*
* 功能描述: 拷贝数据（Core0RtData -> SharedRtData）
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : true: 允许访问共享内存，拷贝成功
*        false: 不允许访问共享内存，拷贝失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysCopyCore0RtDataToSharedRtDataUP1(void)
{
    bool_t bRet = false;
    uint32_t uiOffset = 0;
    uint32_t uiSize = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    bool_t bResult = false;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;

    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_UNLOCKED, \
                                   (uint32_t)RTDATA_AREA_LOCKED);
    //printf("UP1 RES=%d LOCK=%d\n",bResult, g_stSharedRtData.bLock);
    //if((true == bResult) && (RTDATA_AREA_WRITE_ONLY == g_stSharedRtData.bStatus))
    //printf("res=%d\n", bResult);
    if(true == bResult)
    {
        /* 拷贝UP1 G区数据 */
        pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP1);
        if(pstTaskConfigInfo!=NULL)
        {
            uiOffset = pstTaskConfigInfo->uiGOffset;
            uiSize = pstTaskConfigInfo->uiGUsedSize;
            //printf("up1 offset=%x size=%x\n", uiOffset, uiSize);
            pucDst = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
            pucSrc = (puint8_t) &(s_stPRGRtData.ucG[uiOffset]);
            memcpy(pucDst, pucSrc, uiSize);
        }

        g_stSharedRtData.bStatus = RTDATA_AREA_READ_ONLY;

        /* 原子操作：解除锁标志 */
        atomic32Set((puint32_t)&(g_stSharedRtData.bLock), (uint32_t)RTDATA_AREA_UNLOCKED);
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysCopyCore0RtDataToSharedRtDataUP2
*
* 功能描述: 拷贝数据（Core0RtData -> SharedRtData）
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : true: 允许访问共享内存，拷贝成功
*        false: 不允许访问共享内存，拷贝失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysCopyCore0RtDataToSharedRtDataUP2(void)
{
    bool_t bRet = false;
    uint32_t uiOffset = 0;
    uint32_t uiSize = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    bool_t bResult = false;
    TaskConfigInfo_t* pstTaskConfigInfo = NULL;

    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_UNLOCKED, \
                                   (uint32_t)RTDATA_AREA_LOCKED);
    //printf("UP2 RES=%d LOCK=%d\n",bResult, g_stSharedRtData.bLock);
    //if((true == bResult) && (RTDATA_AREA_WRITE_ONLY == g_stSharedRtData.bStatus))
    //printf("res=%d\n", bResult);
    if(true == bResult)
    {
        /* 拷贝UP2 G区数据 */
        pstTaskConfigInfo = SharedGetTaskConfigInfo(TASK_UP2);
        if(pstTaskConfigInfo!=NULL)
        {
            uiOffset = pstTaskConfigInfo->uiGOffset;
            uiSize = pstTaskConfigInfo->uiGUsedSize;
            //printf("up2 offset=%x size=%x\n", uiOffset, uiSize);
            pucDst = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
            pucSrc = (puint8_t) &(s_stPRGRtData.ucG[uiOffset]);
            memcpy(pucDst, pucSrc, uiSize);
        }

        g_stSharedRtData.bStatus = RTDATA_AREA_READ_ONLY;

        /* 原子操作：解除锁标志 */
        atomic32Set((puint32_t)&(g_stSharedRtData.bLock), (uint32_t)RTDATA_AREA_UNLOCKED);
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysBackupRtDataUP1
*
* 功能描述: up1运算完毕，备份其相应的用于传输到up2的数据
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
void SysBackupRtDataUP1(void)
{
    CtrlStationConfigInfo_t* pstCfg = NULL;
    uint32_t uiNum = 0;
    uint32_t i = 0;
    uint32_t uiSrcOff = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    uint16_t usVarSize = 0;

    pstCfg = SharedGetCtrlStationInfo(TASK_UP1);

    if(pstCfg != NULL)
    {
        uiNum = pstCfg->stTaskDataExInfo.uiUP1ToUP2VarNum;
        //printf("u12=%d\n",uiNum);

        if(uiNum < MAX_TASK_EXVAR_NUM)
        {
            for(i = 0; i < uiNum; i++)
            {
                if(0 != pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize)
                {
                    usVarSize = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize;
                    uiSrcOff = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].uiSrcOffset;
                    //printf("1 size=%d srcoff=%d\n",usVarSize, uiSrcOff );

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//G区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucG[uiSrcOff]);
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//I区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucI[uiSrcOff]);
                    }
                    else //Q区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucQ[uiSrcOff]);
                    }

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stBackupUP1Var.ucGVar[i].ucVar[0]);
                        s_stBackupUP1Var.ucGVar[i].ucVarSize = usVarSize;
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stBackupUP1Var.ucIVar[i].ucVar[0]);
                        s_stBackupUP1Var.ucIVar[i].ucVarSize = usVarSize;
                    }
                    else
                    {
                        pucDst = (puint8_t)&(s_stBackupUP1Var.ucQVar[i].ucVar[0]);
                        s_stBackupUP1Var.ucQVar[i].ucVarSize = usVarSize;
                    }
                    memcpy(pucDst, pucSrc, usVarSize);
                    //printf("1! %x %x %x %x\n",pucSrc[0],pucSrc[1],pucSrc[2],pucSrc[3] );
                }
            }
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: SysBackupRtDataUP2
*
* 功能描述: up2运算完毕，备份其相应的用于传输到up1的数据
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
void SysBackupRtDataUP2(void)
{
    CtrlStationConfigInfo_t* pstCfg = NULL;
    uint32_t uiNum = 0;
    uint32_t uiNum2 = 0;
    uint32_t i = 0;
    uint32_t uiSrcOff = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    uint16_t usVarSize = 0;

    pstCfg = SharedGetCtrlStationInfo(TASK_UP2);

    if(pstCfg != NULL)
    {
        uiNum = pstCfg->stTaskDataExInfo.uiUP2ToUP1VarNum;
        uiNum2 = pstCfg->stTaskDataExInfo.uiUP1ToUP2VarNum;
        //printf("u21=%d\n",uiNum);

        if(uiNum < MAX_TASK_EXVAR_NUM)
        {
            for(i = uiNum2 + 0; i < uiNum2 + uiNum; i++)
            {
                if(0 != pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize)
                {
                    usVarSize = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize;
                    uiSrcOff = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].uiSrcOffset;
                    //printf("2 size=%d srcoff=%d\n",usVarSize, uiSrcOff );

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//G区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucG[uiSrcOff]);
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//I区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucI[uiSrcOff]);
                    }
                    else //Q区
                    {
                        pucSrc = (puint8_t)&(s_stPRGRtData.ucQ[uiSrcOff]);
                    }

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stBackupUP2Var.ucGVar[i].ucVar[0]);
                        s_stBackupUP2Var.ucGVar[i].ucVarSize = usVarSize;
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stBackupUP2Var.ucIVar[i].ucVar[0]);
                        s_stBackupUP2Var.ucIVar[i].ucVarSize = usVarSize;
                    }
                    else
                    {
                        pucDst = (puint8_t)&(s_stBackupUP2Var.ucQVar[i].ucVar[0]);
                        s_stBackupUP2Var.ucQVar[i].ucVarSize = usVarSize;
                    }
                    memcpy(pucDst, pucSrc, usVarSize);
                    //printf("2! %x %x %x %x\n",pucSrc[0],pucSrc[1],pucSrc[2],pucSrc[3] );
                }
            }
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: SysExchangeUP1toUP2
*
* 功能描述: 将up1的数据传到up2
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
void SysExchangeUP1toUP2(void)
{
    CtrlStationConfigInfo_t* pstCfg = NULL;
    uint32_t uiNum = 0;
    uint32_t i = 0;
    uint32_t uiDstOff = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    uint16_t usVarSize = 0;

    pstCfg = SharedGetCtrlStationInfo(TASK_UP1);

    if(pstCfg != NULL)
    {
        uiNum = pstCfg->stTaskDataExInfo.uiUP1ToUP2VarNum;
        //printf("12$%d\n",uiNum);
        if(uiNum < MAX_TASK_EXVAR_NUM)
        {
            for(i = 0; i < uiNum; i++)
            {
                if(0 != pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize)
                {
                    usVarSize = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize;
                    uiDstOff = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].uiDestOffset;
                    //printf("12#%d %d\n",usVarSize,uiDstOff);
                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//G区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP1Var.ucGVar[i].ucVar[0]);
                   }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//I区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP1Var.ucIVar[i].ucVar[0]);
                    }
                    else //Q区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP1Var.ucQVar[i].ucVar[0]);
                    }

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucG[uiDstOff]);
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucI[uiDstOff]);
                    }
                    else
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucQ[uiDstOff]);
                    }
                    memcpy(pucDst, pucSrc, usVarSize);
                    //printf(">> %x %x %x %x\n",pucSrc[0],pucSrc[1],pucSrc[2],pucSrc[3] );
                }
            }
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: SysExchangeUP2toUP1
*
* 功能描述: 将up2的数据传到up1
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
void SysExchangeUP2toUP1(void)
{
    CtrlStationConfigInfo_t* pstCfg = NULL;
    uint32_t uiNum = 0;
    uint32_t uiNum2 = 0;
    uint32_t i = 0;
    uint32_t uiDstOff = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    uint16_t usVarSize = 0;

    pstCfg = SharedGetCtrlStationInfo(TASK_UP2);

    if(pstCfg != NULL)
    {
        uiNum = pstCfg->stTaskDataExInfo.uiUP2ToUP1VarNum;
        uiNum2 = pstCfg->stTaskDataExInfo.uiUP1ToUP2VarNum;

        if(uiNum < MAX_TASK_EXVAR_NUM)
        {
            for(i = 0 + uiNum2; i < uiNum2 + uiNum; i++)
            {
                if(0 != pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize)
                {
                    usVarSize = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].usSize;
                    uiDstOff = pstCfg->stTaskDataExInfo.stTaskDataExVar[i].uiDestOffset;
                    //printf("21#%d %d\n",usVarSize,uiDstOff);
                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//G区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP2Var.ucGVar[i].ucVar[0]);
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucSrcRefID)//I区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP2Var.ucIVar[i].ucVar[0]);
                    }
                    else //Q区
                    {
                        pucSrc = (puint8_t)&(s_stBackupUP2Var.ucQVar[i].ucVar[0]);
                    }

                    if(0 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucG[uiDstOff]);
                    }
                    else if(1 == pstCfg->stTaskDataExInfo.stTaskDataExVar[i].ucDestRefId)
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucI[uiDstOff]);
                    }
                    else
                    {
                        pucDst = (puint8_t)&(s_stPRGRtData.ucQ[uiDstOff]);
                    }
                    memcpy(pucDst, pucSrc, usVarSize);
                    //printf("<< %x %x %x %x\n",pucSrc[0],pucSrc[1],pucSrc[2],pucSrc[3] );
                }
            }
        }
    }
}


/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
