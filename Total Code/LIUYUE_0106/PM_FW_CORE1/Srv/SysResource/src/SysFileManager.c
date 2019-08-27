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
*  文件名称    : SysCommon.c
*
*  功能简介    : 系统资源管理模块中的公共资源，其它模块均可调用
*
*  作者        : 李琦
*
*  创建日期    : 2016-02-24
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
#include "../inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
/* 存放请求消息 */
static SysFMReqMsgBox_t s_stFMReqMsgBox;
/* 存放应答消息 */
static SysFMAckMsgBox_t s_stFMAckMsgBox;

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
* 函数名称: SysFileManagerInit
*
* 功能描述: 文件管理初始化
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
void SysFileManagerInit(void)
{
    /* 初始化 */
    memset((puint8_t)&s_stFMReqMsgBox, 0, sizeof(SysFMReqMsgBox_t));
    memset((puint8_t)&s_stFMAckMsgBox, 0, sizeof(SysFMAckMsgBox_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysWriteFile
*
* 功能描述: RTS task写文件名称及内容到共享内存缓冲区
*
* 输入参数: ucName[]: 文件名
*        ucContent[]: 待写入的文件内容
*        usLen: 文件内容长度
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysWriteFile(int32_t iFd, uint8_t ucContent[], uint16_t usLen)
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;

    /* 输入参数合法性检查 */
    if((iFd >= 0) && (ucContent != NULL) && (usLen > 0) && (usLen < MAX_FILE_MSG_LEN))
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 文件描述符 */
            pstReqMsgBox->stReqMsg.iFd = iFd;
            /* 控制字 */
            pstReqMsgBox->stReqMsg.uiCtrlWord = FM_WRITE_FILE;
            /* 写文件内容 */
            memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucContent[0]), ucContent, usLen);
            /* 写文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = usLen;
            /* 消息内容置为满标识 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
        else
        {
            /* 不允许访问或信箱为满，则写入失败 */
            bRet = false;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysReadFile
*
* 功能描述: RTS task读文件内容到共享内存缓冲区
*
* 输入参数: iFd: 文件描述符
*        usLen: 文件内容长度
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysReadFile(int32_t iFd, uint16_t usLen)
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;

    /* 输入参数合法性检查 */
    if((iFd >= 0) && (usLen > 0) && (usLen < MAX_FILE_MSG_LEN))
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 文件描述符 */
            pstReqMsgBox->stReqMsg.iFd = iFd;
            /* 控制字 */
            pstReqMsgBox->stReqMsg.uiCtrlWord = FM_READ_FILE;
            /* 读文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = usLen;
            /* 消息内容置为满标识 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
        else
        {
            /* 不允许访问或信箱为满，则写入失败 */
            bRet = false;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysSendAck
*
* 功能描述: file manager task发送ack消息到共享内存缓冲区
*
* 输入参数: pstAck: 指向ack消息
*
* 输出参数: 无
*
* 返 回 值  : 发送成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSendAck(SysFMAck_t *pstAck)
{
    SysFMAckMsgBox_t *pstAckMsgBox = NULL;
    puint8_t pucAck = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pstAck != NULL)
    {
        /* 找到待读出信息的信箱 */
        pstAckMsgBox = &(s_stFMAckMsgBox);
        /* 允许访问且有数据 */
        if((FM_MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstAckMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstAckMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 获取消息内容 */
            pucAck = (puint8_t)&(pstAckMsgBox->stAckMsg);
            memcpy(pucAck, (puint8_t)pstAck, sizeof(SysFMAck_t));
            /* 清空消息内容 */
            pstAckMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 置读取成功标识 */
            bRet = true;
            /* 解除锁定 */
            pstAckMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysReadFMReqMsg
*
* 功能描述: file manager task从共享内存缓冲区中读文件名字及内容
*
* 输入参数: 无
*
* 输出参数:  ucName[]: 文件名
*         puiCTW: 控制字
*         ucContent[]: 待读出的文件内容
*         pusLen: 指向待读出的文件内容长度
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysReadFMReqMsg(SysFMReq_t *pstSysFMReq)
{
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;
    puint8_t pucReq = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pstSysFMReq != NULL)
    {
        /* 找到待读出信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);
        /* 允许访问且有数据 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_FULL == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 获取消息 */
            pucReq = (puint8_t)&(pstReqMsgBox->stReqMsg);
            memcpy((puint8_t)pstSysFMReq, pucReq, sizeof(SysFMReq_t));
            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_EMPTY;
            /* 置读取成功标识 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysRecvAck
*
* 功能描述: RTS task从共享内存缓冲区中接收来自file manager task的ack
*
* 输入参数: 无
*
* 输出参数:  pstAck: 指向ack消息
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysRecvAck(SysFMAck_t *pstAck)
{
    SysFMAckMsgBox_t *pstAckMsgBox = NULL;
    puint8_t pucAck = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pstAck != NULL)
    {
        /* 找到待读出信息的信箱 */
        pstAckMsgBox = &(s_stFMAckMsgBox);
        /* 允许访问且有数据 */
        if((FM_MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (FM_MSGBOX_FULL == pstAckMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstAckMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 获取消息内容 */
            pucAck = (puint8_t)&(pstAckMsgBox->stAckMsg);
            memcpy((puint8_t)pstAck, pucAck, sizeof(SysFMAck_t));
            /* 清空消息内容 */
            pstAckMsgBox->bStatus = FM_MSGBOX_EMPTY;
            /* 置读取成功标识 */
            bRet = true;
            /* 解除锁定 */
            pstAckMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysRecvAckHeader
*
* 功能描述: RTS task从共享内存缓冲区中接收来自file manager task的ack的头部
*
* 输入参数: 无
*
* 输出参数:  pstAckHeader: 指向ack消息头部
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysRecvAckHeader(SysFMAckHeader_t *pstAckHeader)
{
    SysFMAckMsgBox_t *pstAckMsgBox = NULL;
    puint8_t pucAck = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pstAckHeader != NULL)
    {
        /* 找到待读出信息的信箱 */
        pstAckMsgBox = &(s_stFMAckMsgBox);
        /* 允许访问且有数据 */
        if((FM_MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (FM_MSGBOX_FULL == pstAckMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstAckMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 获取消息内容 */
            pucAck = (puint8_t)&(pstAckMsgBox->stAckMsg);
            memcpy((puint8_t)pstAckHeader, pucAck, sizeof(SysFMAckHeader_t));
            /* 清空消息内容 */
            pstAckMsgBox->bStatus = FM_MSGBOX_EMPTY;
            /* 置读取成功标识 */
            bRet = true;
            /* 解除锁定 */
            pstAckMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
    }
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysOpenFile
*
* 功能描述: RTS task向共享内存缓冲区中写入需要打开的文件名称（若文件不存在则创建该文件）
*
* 输入参数: ucName[]: 文件名
*
* 输出参数: 无
*
* 返 回 值  : true-打开成功
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysOpenFile(uint8_t ucName[])
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;
    uint8_t ucNameLen = 0;

    /* 输入参数有效性检查 */
    if(ucName != NULL)
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 写文件名 */
            ucNameLen = (uint8_t)strlen((const pchar_t)&ucName[0]);
            if(ucNameLen < MAX_FILE_NAME_LEN)
            {
                memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucFileName[0]), ucName, ucNameLen+1);

                /* 控制字 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_OPEN_FILE;
            }
            else
            {
                /* 置控制命令无效 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_INVALID_CWT;
            }
            /* 当前无写入内容，写文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = 0;

            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
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
* 函数名称: SysCloseFile
*
* 功能描述: RTS task向共享内存缓冲区中写入需要关闭的文件名称
*
* 输入参数: iFd: 文件描述符
*
* 输出参数: 无
*
* 返 回 值  : true-关闭成功
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysCloseFile(int32_t iFd)
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;

    /* 输入参数有效性检查 */
    if(iFd >= 0)
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 写文件描述符 */
            pstReqMsgBox->stReqMsg.iFd = iFd;
            /* 控制字 */
            pstReqMsgBox->stReqMsg.uiCtrlWord = FM_CLOSE_FILE;
            /* 当前无写入内容，写文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = 0;
            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
        }
        else
        {
            /* 不允许访问或信箱为满，则写入失败 */
            bRet = false;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysDeleteFile
*
* 功能描述: RTS task向共享内存缓冲区中写入需要删除的文件名称
*
* 输入参数: ucName[]: 文件名
*
* 输出参数: 无
*
* 返 回 值  : true-删除成功
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysDeleteFile(uint8_t ucName[])
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;
    uint8_t ucNameLen = 0;

    /* 输入参数有效性检查 */
    if(ucName != NULL)
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 写文件名 */
            ucNameLen = (uint8_t)strlen((const pchar_t)&ucName[0]);
            if(ucNameLen < MAX_FILE_NAME_LEN)
            {
                memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucFileName[0]), ucName, ucNameLen+1);

                /* 控制字 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_DELETE_FILE;
            }
            else
            {
                /* 置控制命令无效 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_INVALID_CWT;
            }

            /* 当前无写入内容，写文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = 0;

            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
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
* 函数名称: SysSendIsFileExist
*
* 功能描述: RTS task向共享内存缓冲区中写入需要判断文件是否存在的文件名称
*
* 输入参数: ucName[]: 文件名
*
* 输出参数: 无
*
* 返 回 值  : true-输入参数正确 且允许访问
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSendIsFileExist(uint8_t ucName[])
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;
    uint8_t ucNameLen = 0;

    /* 输入参数有效性检查 */
    if(ucName != NULL)
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 写文件名 */
            ucNameLen = (uint8_t)strlen((const pchar_t)&ucName[0]);
            if(ucNameLen < MAX_FILE_NAME_LEN)
            {
                memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucFileName[0]), ucName, ucNameLen+1);

                /* 控制字 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_IS_FILE_EXIST;
            }
            else
            {
                /* 置控制命令无效 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_INVALID_CWT;
            }

            /* 当前无写入内容，写文件内容长度 */
            pstReqMsgBox->stReqMsg.usLen = 0;

            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
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
* 函数名称: SysRenameFile
*
* 功能描述: RTS task向共享内存缓冲区中写入新旧文件名
*
* 输入参数: ucOldName[]: 旧文件名
*        ucNewName[]: 新文件名
*
* 输出参数: 无
*
* 返 回 值  : true-输入参数正确 且允许访问
*        false- 1)输入参数错误  2)不允许访问
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysRenameFile(uint8_t ucOldName[], uint8_t ucNewName[])
{
    bool_t bRet = false;
    SysFMReqMsgBox_t *pstReqMsgBox = NULL;
    uint8_t ucOldNameLen = 0;
    uint8_t ucNewNameLen = 0;

    /* 输入参数有效性检查 */
    if((ucOldName != NULL) && (ucNewName != NULL))
    {
        /* 找到待写入信息的信箱 */
        pstReqMsgBox = &(s_stFMReqMsgBox);

        /* 允许访问且信箱为空 */
        if((FM_MSGBOX_UNLOCKED == pstReqMsgBox->bLock) && (FM_MSGBOX_EMPTY == pstReqMsgBox->bStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstReqMsgBox->bLock = FM_MSGBOX_LOCKED;
            /* 写文件名 */
            ucOldNameLen = (uint8_t)strlen((const pchar_t)&ucOldName[0]);
            ucNewNameLen = (uint8_t)strlen((const pchar_t)&ucNewName[0]);
            if((ucOldNameLen < MAX_FILE_NAME_LEN) && (ucNewNameLen < MAX_FILE_NAME_LEN))
            {
                memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucFileName[0]), ucOldName, ucOldNameLen+1);
                memcpy((puint8_t)&(pstReqMsgBox->stReqMsg.ucContent[0]), ucNewName, ucNewNameLen+1);
                pstReqMsgBox->stReqMsg.usLen = (uint16_t)ucNewNameLen;
                /* 控制字 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_RENAME_FILE;
            }
            else
            {
                /* 置控制命令无效 */
                pstReqMsgBox->stReqMsg.uiCtrlWord = FM_INVALID_CWT;
            }

            /* 清空消息内容 */
            pstReqMsgBox->bStatus = FM_MSGBOX_FULL;
            /* 写入成功 */
            bRet = true;
            /* 解除锁定 */
            pstReqMsgBox->bLock = FM_MSGBOX_UNLOCKED;
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
*                                局部函数实现
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
