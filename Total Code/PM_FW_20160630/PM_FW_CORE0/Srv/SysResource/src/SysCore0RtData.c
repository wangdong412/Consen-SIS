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

static PRG_RTDataArea_t s_stPRGRtData;
static PMABC_IO_t s_stPMIOData;



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
               pStartAddr = &(s_stPMIOData.ucI[emPMID][RT_DATA_TYPE_I]);
            }
            break;

            case RT_DATA_TYPE_Q:
            {
               pStartAddr = &(s_stPMIOData.ucQ[emPMID][RT_DATA_TYPE_Q]);
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
* 函数名称: SysCopyCore0RtDataToSharedRtData
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
bool_t SysCopyCore0RtDataToSharedRtData(void)
{
    bool_t bRet = false;
    uint32_t uiOffset = 0;
    uint32_t uiSize = 0;
    puint8_t pucSrc = NULL;
    puint8_t pucDst = NULL;
    bool_t bResult = false;
#if 0
    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(g_stSharedRtData.bLock), \
                                   (uint32_t)RTDATA_AREA_UNLOCKED, \
                                   (uint32_t)RTDATA_AREA_LOCKED);
#endif
    //if((true == bResult) && (RTDATA_AREA_WRITE_ONLY == g_stSharedRtData.bStatus))
    {

        /* 拷贝UP1 G区数据 */
        uiOffset = 0 ; // use config info later
        uiSize = 512;  // use config info later
        pucDst = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucG[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);

#if 0
        /* 拷贝UP1 I区数据 */
        //uiOffset =    ;
        //uiSize =    ;
        pucDst = (puint8_t) &(g_stSharedRtData.ucI[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucI[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);

        /* 拷贝UP1 Q区数据 */
        //uiOffset =    ;
        //uiSize =    ;
        pucDst = (puint8_t) &(g_stSharedRtData.ucQ[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucQ[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);

        /* 拷贝UP2 G区数据 */
        //uiOffset =    ;
        //uiSize =    ;
        pucDst = (puint8_t) &(g_stSharedRtData.ucG[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucG[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);

        /* 拷贝UP2 I区数据 */
        //uiOffset =    ;
        //uiSize =    ;
        pucDst = (puint8_t) &(g_stSharedRtData.ucI[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucI[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);

        /* 拷贝UP2 Q区数据 */
        //uiOffset =    ;
        //uiSize =    ;
        pucDst = (puint8_t) &(g_stSharedRtData.ucQ[uiOffset]);
        pucSrc = (puint8_t) &(s_stPRGRtData.ucQ[uiOffset]);
        memcpy(pucDst, pucSrc, uiSize);
#endif
        g_stSharedRtData.bStatus = RTDATA_AREA_READ_ONLY;

        /* 原子操作：解除锁标志 */
        //atomic32Set((puint32_t)&(g_stSharedRtData.bLock), (uint32_t)RTDATA_AREA_UNLOCKED);
        bRet = true;
    }

    return bRet;
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
