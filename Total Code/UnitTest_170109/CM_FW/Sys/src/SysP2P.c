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
*  文件名称    : SysP2P.c
*
*  功能简介    : 管理P2P模块与内部通讯模块间的资源
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-31
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
#include <string.h>
#include "../inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
/* 用于内部通讯的信箱 */
static SysP2PMsgBox_t s_stSysP2PMsgBox[NUM_OF_PM][NUM_OF_P2PMSGTYPE];
/* P2P消息队列 */
//static SysP2PMsgQueue_t s_stSysP2PMsgQueue;

static SysP2PMsgQueue_t s_stSysP2PExtReqMsgQueue;
static SysP2PMsgQueue_t s_stSysP2PExtRespMsgQueue;

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
* 函数名称: SysP2PInit
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
void SysP2PInit(void)
{
    /* 信箱初始化 */
    memset((puint8_t)&s_stSysP2PMsgBox[0][0], 0, sizeof(SysP2PMsgBox_t)*NUM_OF_PM*NUM_OF_P2PMSGTYPE);
    /* P2P消息队列初始化 */
    //memset((puint8_t)&s_stSysP2PMsgQueue, 0, sizeof(SysP2PMsgQueue_t));
    memset((puint8_t)&s_stSysP2PExtReqMsgQueue, 0, sizeof(SysP2PMsgQueue_t));
    memset((puint8_t)&s_stSysP2PExtRespMsgQueue, 0, sizeof(SysP2PMsgQueue_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PMsg
*
* 功能描述: 从系统资源模块获取消息（请求/应答）
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
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
bool_t SysGetP2PMsg (PMController_t emPMID, SysP2PMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen)
{
    SysP2PMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if((pucContent != NULL) && (pusLen != NULL))
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_P2PMSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysP2PMsgBox[emPMID][emMsgType]);
            /* 允许访问且有数据 */
            if((MSGBOX_UNLOCKED == pstMsgBox->bLock) && (MSGBOX_FULL == pstMsgBox->bMsgBoxStatus))
            {
                /* 锁定信箱，防止其它进程访问 */
                pstMsgBox->bLock = MSGBOX_LOCKED;
                /* 获取消息内容 */
                memcpy(pucContent,(puint8_t)&(pstMsgBox->stMsg.ucContent[0]),pstMsgBox->stMsg.usLen);
                /* 获取消息长度 */
                *pusLen = pstMsgBox->stMsg.usLen;
                /* 清空消息内容 */
                pstMsgBox->bMsgBoxStatus = MSGBOX_EMPTY;
                /* 置读取成功标识 */
                bRet = true;
                /* 解除锁定 */
                pstMsgBox->bLock = MSGBOX_UNLOCKED;
            }
            else
            {
                /* 不允许访问或无数据，置待读出消息内容长度为0 */
                *pusLen = 0;
            }
        }
        else
        {
            /* 输入参数无效，置待读出消息内容长度为0 */
            *pusLen = 0;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetP2PMsgLen
*
* 功能描述: 从系统资源模块获取消息（请求/应答）的长度
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-读取成功
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetP2PMsgLen (PMController_t emPMID, SysP2PMsgType_t emMsgType, puint16_t pusLen)
{
    SysP2PMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pusLen != NULL)
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_P2PMSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysP2PMsgBox[emPMID][emMsgType]);
            /* 允许访问且有数据 */
            if((MSGBOX_UNLOCKED == pstMsgBox->bLock) && (MSGBOX_FULL == pstMsgBox->bMsgBoxStatus))
            {
                /* 锁定信箱，防止其它进程访问 */
                pstMsgBox->bLock = MSGBOX_LOCKED;
                /* 获取消息长度 */
                *pusLen = pstMsgBox->stMsg.usLen;
                /* 置读取成功标识 */
                bRet = true;
                /* 解除锁定 */
                pstMsgBox->bLock = MSGBOX_UNLOCKED;
            }
            else
            {
                /* 不允许访问或无数据，置待读出消息内容长度为0 */
                *pusLen = 0;
            }
        }
        else
        {
            /* 输入参数无效，置待读出消息内容长度为0 */
            *pusLen = 0;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetP2PMsg
*
* 功能描述: 将请求或应答消息写入系统资源模块
*
* 输入参数: emPMID: PM的ID
*        emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetP2PMsg (PMController_t emPMID, SysP2PMsgType_t emMsgType , puint8_t pucContent, uint16_t usLen)
{
    bool_t bRet = false;
    SysP2PMsgBox_t *pstMsgBox = NULL;

    /* 输入参数有效性检查 */
    if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_P2PMSGTYPE))
    {
        /* 找到待写入信息的信箱 */
        pstMsgBox = &(s_stSysP2PMsgBox[emPMID][emMsgType]);

        /* 允许访问且信箱为空 */
        if((MSGBOX_UNLOCKED == pstMsgBox->bLock) && (MSGBOX_EMPTY == pstMsgBox->bMsgBoxStatus))
        {
            /* 输出参数有效性检查 */
            if((pucContent != NULL) && (usLen != 0))
            {
                /* 锁定信箱，防止其它进程访问 */
                pstMsgBox->bLock = MSGBOX_LOCKED;
                 /* 获取消息内容 */
                memcpy((puint8_t)&(pstMsgBox->stMsg.ucContent[0]), pucContent,usLen);
                /* 获取消息长度 */
                pstMsgBox->stMsg.usLen = usLen;
                /* 清空消息内容 */
                pstMsgBox->bMsgBoxStatus = MSGBOX_FULL;
                /* 写入成功 */
                bRet = true;
                /* 解除锁定 */
                pstMsgBox->bLock = MSGBOX_UNLOCKED;
            }
        }
        else
        {
            /* 不允许访问或信箱为满，则写入失败 */
            bRet = false;
        }
    }
    else
    {
        /* 输入参数无效，则写入失败 */
        bRet = false;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetStationMsg
*
* 功能描述: 将其它控制站的请求/应答消息写入队列
*
* 输入参数: emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetStationMsg(SysP2PMsgType_t emMsgType, puint8_t pucContent, uint16_t usLen)
{
    bool_t bRet = false;                    /* 函数返回值 */
    SysP2PMsg_t *pstP2PMsg = NULL;          /* 指向队列中待写入消息的位置 */
    uint16_t usNum = 0;                     /* 记录队列中的消息个数 */
    uint16_t usTail = 0;                    /* 记录队尾位置 */
    SysP2PMsgQueue_t *pstMsgQueue = NULL;

    /* 输入参数有效性检查 */
    if((emMsgType < NUM_OF_P2PMSGTYPE) && (emMsgType >= P2P_EXT_REQ))
    {
        /* 获取消息队列 */
        if( P2P_EXT_REQ == emMsgType )
        {
            pstMsgQueue = &s_stSysP2PExtReqMsgQueue;
        }
        else
        {
            pstMsgQueue = &s_stSysP2PExtRespMsgQueue;
        }

        /* 获取消息个数及队尾位置 */
        usNum = pstMsgQueue->usNum;
        usTail = pstMsgQueue->usTail;

        /* 判断队列是否已满 */
        if(usNum < MAX_P2P_MSG_NUM)
        {
            /* 找到待写入消息的位置*/
            pstP2PMsg = &(pstMsgQueue->stMsg[usTail]);

            /* 输出参数有效性检查 */
            if((pucContent != NULL) && (usLen != 0))
            {
                 /* 写入内容 */
                memcpy((puint8_t)&(pstP2PMsg->ucContent[0]), pucContent,usLen);
                /* 写入长度 */
                pstP2PMsg->usLen = usLen;
                /* 更新列尾及队列中的消息个数 */
                usTail = (usTail + 1) % MAX_P2P_MSG_NUM;
                pstMsgQueue->usTail = usTail;
                pstMsgQueue->usNum++;
                /* 写入成功 */
                bRet = true;
            }
        }

#if 0
        /* 获取消息个数及队尾位置 */
        usNum = s_stSysP2PMsgQueue.usNum;
        usTail = s_stSysP2PMsgQueue.usTail;

        /* 判断队列是否已满 */
        if(usNum < MAX_P2P_MSG_NUM)
        {
            /* 找到待写入消息的位置*/
            pstP2PMsg = &(s_stSysP2PMsgQueue.stMsg[usTail]);

            /* 输出参数有效性检查 */
            if((pucContent != NULL) && (usLen != 0))
            {
                 /* 写入内容 */
                memcpy((puint8_t)&(pstP2PMsg->ucContent[0]), pucContent,usLen);
                /* 写入长度 */
                pstP2PMsg->usLen = usLen;
                /* 更新列尾及队列中的消息个数 */
                usTail = (usTail + 1) % MAX_P2P_MSG_NUM;
                s_stSysP2PMsgQueue.usTail = usTail;
                s_stSysP2PMsgQueue.usNum++;
                /* 写入成功 */
                bRet = true;
            }
        }
#endif
    }
    else
    {
        /* 输入参数无效，则写入失败 */
        bRet = false;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetStationMsg
*
* 功能描述: 从队列中读出来自其它控制站的请求/应答消息
*
* 输入参数: emMsgType: 各种消息类型
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetStationMsg(SysP2PMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen)
{
    bool_t bRet = false;                    /* 函数返回值 */
    SysP2PMsg_t *pstP2PMsg = NULL;          /* 指向待读取的消息 */
    uint16_t usNum = 0;                     /* 记录队列中的消息个数 */
    uint16_t usHead = 0;                    /* 记录队头位置 */
    SysP2PMsgQueue_t *pstMsgQueue = NULL;

    /* 输出参数有效性检查 */
    if((pucContent != NULL) && (pusLen != NULL))
    {
        if((emMsgType < NUM_OF_P2PMSGTYPE) && (emMsgType >= P2P_EXT_REQ))
        {
            /* 获取消息队列 */
            if( P2P_EXT_REQ == emMsgType )
            {
                pstMsgQueue = &s_stSysP2PExtReqMsgQueue;
            }
            else
            {
                pstMsgQueue = &s_stSysP2PExtRespMsgQueue;
            }

            /* 获取队列中消息个数及队头位置 */
            usNum = pstMsgQueue->usNum;
            usHead = pstMsgQueue->usHead;

            /* 输入参数有效性检查 */
            if(usNum > 0)
            {
                /* 找到待读出信息的信箱 */
                pstP2PMsg = &(pstMsgQueue->stMsg[usHead]);
                /* 获取消息内容 */
                memcpy(pucContent,(puint8_t)&(pstP2PMsg->ucContent[0]),pstP2PMsg->usLen);
                /* 获取消息长度 */
                *pusLen = pstP2PMsg->usLen;
                /* 更新队列头位置及队列中的消息个数 */
                usHead = (usHead + 1) % MAX_P2P_MSG_NUM;
                pstMsgQueue->usHead = usHead;
                pstMsgQueue->usNum--;
                /* 置读取成功标识 */
                bRet = true;
            }
            else
            {
                /* 队列空，无消息 */
                *pusLen = 0;
                bRet = false;
            }

#if 0
            /* 获取队列中消息个数及队头位置 */
            usNum = s_stSysP2PMsgQueue.usNum;
            usHead = s_stSysP2PMsgQueue.usHead;

            /* 输入参数有效性检查 */
            if(usNum > 0)
            {
                /* 找到待读出信息的信箱 */
                pstP2PMsg = &(s_stSysP2PMsgQueue.stMsg[usHead]);
                /* 获取消息内容 */
                memcpy(pucContent,(puint8_t)&(pstP2PMsg->ucContent[0]),pstP2PMsg->usLen);
                /* 获取消息长度 */
                *pusLen = pstP2PMsg->usLen;
                /* 更新队列头位置及队列中的消息个数 */
                usHead = (usHead + 1) % MAX_P2P_MSG_NUM;
                s_stSysP2PMsgQueue.usHead = usHead;
                s_stSysP2PMsgQueue.usNum--;
                /* 置读取成功标识 */
                bRet = true;
            }
            else
            {
                /* 队列空，无消息 */
                *pusLen = 0;
                bRet = false;
            }
#endif
        }
        else
        {
            /* 输入参数无效 */
            *pusLen = 0;
            bRet = false;
        }
    }
    else
    {
        /* 输出参数无效 */
        bRet = false;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetStationMsgNum
*
* 功能描述: 从系统资源模块获取消息（请求/应答）的个数
*
* 输入参数: emMsgType: 各种消息类型
*
* 输出参数: pusNum: 指向待读出的消息个数
*
* 返 回 值  : true-读取成功
*        false- 1)无数据   2)输入输出参数错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetStationMsgNum(SysP2PMsgType_t emMsgType, puint16_t pusNum)
{
    bool_t bRet = false;                    /* 函数返回值 */
    SysP2PMsgQueue_t *pstMsgQueue = NULL;

    /* 输入输出参数有效性检查 */
    if((emMsgType < NUM_OF_P2PMSGTYPE) && (emMsgType >= P2P_EXT_REQ) && (pusNum != NULL))
    {
        /* 获取队列中的消息个数 */
        if( P2P_EXT_REQ == emMsgType )
        {
            pstMsgQueue = &s_stSysP2PExtReqMsgQueue;
        }
        else
        {
            pstMsgQueue = &s_stSysP2PExtRespMsgQueue;
        }

        *pusNum = pstMsgQueue->usNum;

#if 0
        *pusNum =  s_stSysP2PMsgQueue.usNum;
#endif
        bRet = true;
    }
    else
    {
        /* 输入或输出参数无效 */
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
