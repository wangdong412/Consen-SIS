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
*  文件名称    : SharedComm.h
*
*  功能简介    : core0与core1间的消息通讯
*
*  作者            : 李琦
*
*  创建日期    : 2016-04-15
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
#include "../inc/SharedComm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static SMMsgBox_t  s_stSMMsgBox[NUM_OF_CM][NUM_OF_SM_MSGTYPE];

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
* 函数名称: SharedCommInit
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
void SharedCommInit(void)
{
    /* 初始化 */
    memset((puint8_t)&s_stSMMsgBox[0][0], 0, sizeof(SMMsgBox_t)*NUM_OF_CM*NUM_OF_SM_MSGTYPE);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetSMMsg
*
* 功能描述: 从共享内存中获取消息（请求/应答）
*
* 输入参数: emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待读出的消息内容
*         pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-收到消息
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetSMMsg (SMMsgType_t emMsgType,CMController_t emCMIndex, puint8_t pucContent, puint16_t pusLen)
{
    SMMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    /* 输入输出参数有效性检查 */
    if((emMsgType < NUM_OF_SM_MSGTYPE) &&(emCMIndex < NUM_OF_CM) && (pucContent != NULL) && (pusLen != NULL))
    {
        /* 找到待读出信息的信箱 */
        pstMsgBox = &(s_stSMMsgBox[emCMIndex][emMsgType]);

        /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
        bResult = (bool_t) atomic32Cas((puint32_t)&(pstMsgBox->uiLock), \
                                       (uint32_t)SM_MSGBOX_UNLOCKED, \
                                       (uint32_t)SM_MSGBOX_LOCKED);
        
//        printf("bResult: %d \n",bResult);

        if(true == bResult)
        {
            if(SM_MSGBOX_FULL == pstMsgBox->bMsgBoxStatus)
            {
                /* 获取消息内容 */
                memcpy(pucContent,(puint8_t)&(pstMsgBox->stMsg.ucContent[0]),pstMsgBox->stMsg.usLen);
                /* 获取消息长度 */
                *pusLen = pstMsgBox->stMsg.usLen;
                /* 清空消息内容 */
                pstMsgBox->bMsgBoxStatus = SM_MSGBOX_EMPTY;
                /* 置读取成功标识 */
                bRet = true;
            }

            /* 原子操作：解除锁标志 */
            atomic32Set((puint32_t)&(pstMsgBox->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
        }
        else
        {
            /* 不允许访问或无数据，置待读出消息内容长度为0 */
            *pusLen = 0;
            bRet = false;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetSMMsg
*
* 功能描述: 将请求或应答消息写入共享内存
*
* 输入参数: emMsgType: 各种消息类型
*        pucContent: 指向待写入的消息内容
*        pusLen: 指向待写入的消息内容长度
*
* 输出参数: 无
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetSMMsg (SMMsgType_t emMsgType ,CMController_t emCMIndex, puint8_t pucContent, uint16_t usLen)
{
    SMMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;
    bool_t bResult = false;

    /* 输入输出参数有效性检查 */
    if((emMsgType < NUM_OF_SM_MSGTYPE) && (emCMIndex < NUM_OF_CM) && (pucContent != NULL) && (usLen != 0))
    {
        /* 找到待写入信息的信箱 */
        pstMsgBox = &(s_stSMMsgBox[emCMIndex][emMsgType]);


        /* 原子操作：读锁标志，判断是否已加锁，若未加锁，则加锁，返回成功标志；若已加锁，则返回失败标志 */
        bResult = (bool_t) atomic32Cas((puint32_t)&(pstMsgBox->uiLock), \
                                       (uint32_t)SM_MSGBOX_UNLOCKED, \
                                       (uint32_t)SM_MSGBOX_LOCKED);
        
//        printf("before bResult: %d \n",bResult);

        if(true == bResult)
        {

            if(SM_MSGBOX_EMPTY == pstMsgBox->bMsgBoxStatus)
            {
                 /* 获取消息内容 */
                memcpy((puint8_t)&(pstMsgBox->stMsg.ucContent[0]), pucContent,usLen);
                /* 获取消息长度 */
                
                
                pstMsgBox->stMsg.usLen = usLen;
                
//                printf(" usLen : %d  ucContent : %x \n",pstMsgBox->stMsg.usLen,pstMsgBox->stMsg.ucContent[0]);
                /* 清空消息内容 */
                pstMsgBox->bMsgBoxStatus = SM_MSGBOX_FULL;
                /* 写入成功 */
                bRet = true;
            }
            else
            {
                printf("SM_MSGBOX_FULL == bMsgBoxStatus \n");
            }
          

            /* 原子操作：解除锁标志 */
            atomic32Set((puint32_t)&(pstMsgBox->uiLock), (uint32_t)SM_MSGBOX_UNLOCKED);
            
            
//            printf("after uiLock: %d \n",atomic32Get((puint32_t)&(pstMsgBox->uiLock)));
            
        }
        else
        {
            /* 不允许访问或信箱为满，则写入失败 */
            bRet = false;
            printf("false == bResult \n");
        }
    }
    else
    {
        printf("Test err \n");
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
