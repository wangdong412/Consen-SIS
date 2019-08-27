/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 共享内存
*
*  文件名称    : SharedHandshake.c
*
*  功能简介    : core0与core1间握手信号，用于同步
*
*  作者            : 李琦
*
*  创建日期    : 2016-09-08
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
#include "../inc/SharedComm.h"
#include "../inc/SharedHandshake.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static HandshakeSignal_t  s_stC0toC1Signal;
static HandshakeSignal_t  s_stC1toC0Signal;

static AdjSysTimeSignal_t s_stC0toC1SysTime;
static AdjSysTimeSignal_t s_stC1toC0SysTime;

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
* 函数名称: SharedHandshakeInit
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
void SharedHandshakeInit(void)
{
    /* 初始化 */
    memset((puint8_t)&s_stC0toC1Signal, 0, sizeof(HandshakeSignal_t));
    memset((puint8_t)&s_stC1toC0Signal, 0, sizeof(HandshakeSignal_t));

    memset((puint8_t)&s_stC0toC1SysTime, 0, sizeof(AdjSysTimeSignal_t));
    memset((puint8_t)&s_stC1toC0SysTime, 0, sizeof(AdjSysTimeSignal_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetHandshakeSignal
*
* 功能描述: 从共享内存中获取握手信号
*
* 输入参数: uiSignalTrans: 信号传输方向
*        puiSignal: 存放信号
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetHandshakeSignal(uint32_t uiSignalTrans, puint32_t puiSignal)
{
    HandshakeSignal_t *pstSignal = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    /* 输入输出参数有效性检查 */
    if(puiSignal != NULL)
    {
        if(CORE0_TO_CORE1 == uiSignalTrans)
        {
            /* 找到待读出信息的信箱 */
            pstSignal = &s_stC0toC1Signal;
        }
        else
        {
            pstSignal = &s_stC1toC0Signal;
        }

        /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
        bResult = (bool_t) atomic32Cas((puint32_t)&(pstSignal->uiLock), \
                                       (uint32_t)SM_MSGBOX_UNLOCKED, \
                                       (uint32_t)SM_MSGBOX_LOCKED);

        if(true == bResult)
        {
            /* 获取内容 */
            *puiSignal = pstSignal->uiSignal;
            /* 读取成功 */
            bRet = true;
            /* 原子操作：解除锁标志 */
            atomic32Set((puint32_t)&(pstSignal->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
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
* 函数名称: SysSetHandshakeSignal
*
* 功能描述: 向共享内存中设置握手信号
*
* 输入参数: uiSignalTrans: 信号传输方向
*        uiSignal: 信号
*
* 输出参数: 无
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetHandshakeSignal(uint32_t uiSignalTrans, uint32_t uiSignal)
{
    HandshakeSignal_t *pstSignal = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    if(CORE0_TO_CORE1 == uiSignalTrans)
    {
        /* 找到待读出信息的信箱 */
        pstSignal = &s_stC0toC1Signal;
    }
    else
    {
        pstSignal = &s_stC1toC0Signal;
    }

    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(pstSignal->uiLock), \
                                   (uint32_t)SM_MSGBOX_UNLOCKED, \
                                   (uint32_t)SM_MSGBOX_LOCKED);
    if(true == bResult)
    {
         /* 写入内容 */
        pstSignal->uiSignal = uiSignal;
        /* 写入成功 */
        bRet = true;
        /* 原子操作：解除锁标志 */
        atomic32Set((puint32_t)&(pstSignal->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
    }
    else
    {
        /* 不允许访问或信箱为满，则写入失败 */
        bRet = false;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSysTimeSignal
*
* 功能描述: 从共享内存中获取系统时间信号
*
* 输入参数: uiSignalTrans: 信号传输方向
*        pstSysTime: 存放信号类型及时间
*
* 输出参数: 无
*
* 返 回 值  : 获取成功或失败
*
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetSysTimeSignal(uint32_t uiSignalTrans, AdjSysTimeSignal_t *pstSysTime)
{
    AdjSysTimeSignal_t *pstTime = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    /* 输入输出参数有效性检查 */
    if(pstSysTime != NULL)
    {
        if(CORE0_TO_CORE1 == uiSignalTrans)
        {
            /* 找到待读出信息的信箱 */
            pstTime = &s_stC0toC1SysTime;
        }
        else
        {
            pstTime = &s_stC1toC0SysTime;
        }

        /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
        bResult = (bool_t) atomic32Cas((puint32_t)&(pstTime->uiLock), \
                                       (uint32_t)SM_MSGBOX_UNLOCKED, \
                                       (uint32_t)SM_MSGBOX_LOCKED);

        if(true == bResult)
        {
            /* 获取内容 */
            pstSysTime->uiSignal = pstTime->uiSignal;
            pstSysTime->uiSec = pstTime->uiSec;
            pstSysTime->uiMilSec = pstTime->uiMilSec;

            /* 读取成功 */
            bRet = true;
            /* 原子操作：解除锁标志 */
            atomic32Set((puint32_t)&(pstTime->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
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
* 函数名称: SysSetSysTimeSignal
*
* 功能描述: 向共享内存中设置系统时间信号
*
* 输入参数: uiSignalTrans: 信号传输方向
*        pstSysTime: 系统时间信号
*
* 输出参数: 无
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSysTimeSignal(uint32_t uiSignalTrans, AdjSysTimeSignal_t *pstSysTime)
{
    AdjSysTimeSignal_t *pstTime = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    if(CORE0_TO_CORE1 == uiSignalTrans)
    {
        /* 找到待读出信息的信箱 */
        pstTime = &s_stC0toC1SysTime;
    }
    else
    {
        pstTime = &s_stC1toC0SysTime;
    }

    /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
    bResult = (bool_t) atomic32Cas((puint32_t)&(pstTime->uiLock), \
                                   (uint32_t)SM_MSGBOX_UNLOCKED, \
                                   (uint32_t)SM_MSGBOX_LOCKED);
    if(true == bResult)
    {
         /* 写入内容 */
        pstTime->uiSignal = pstSysTime->uiSignal;
        pstTime->uiSec = pstSysTime->uiSec;
        pstTime->uiMilSec = pstSysTime->uiMilSec;

        /* 写入成功 */
        bRet = true;
        /* 原子操作：解除锁标志 */
        atomic32Set((puint32_t)&(pstTime->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
    }
    else
    {
        /* 不允许访问或信箱为满，则写入失败 */
        bRet = false;
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
