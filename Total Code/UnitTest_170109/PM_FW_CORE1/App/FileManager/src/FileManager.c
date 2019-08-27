/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 文件管理模块
*
*  文件名称    : FileManager.h
*
*  功能简介    : 对文件进行读写管理
*
*  作者       : 李琦
*
*  创建日期    : 2016-04-06
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
#include "../inc/FileManager.h"
#include "Srv/SysResource/inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static int32_t s_iFd;               /* 文件描述符 */
static SysFMReq_t s_stSysFMReq;     /* 来自RTS task的请求消息 */
static SysFMAck_t s_stSysFMAck;     /* 发送到RTS task的应答消息 */

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
/* 打开文件处理函数（文件不存在时创建文件） */
static void FMOpenFileHandling(void);
/* 写文件处理函数 */
static void FMWriteFileHandling(void);
/* 关闭文件处理函数 */
static void FMCloseFileHandling(void);
/* 判断文件是否存在处理函数 */
static void FMIsFileExistHandling(void);
/* 删除文件处理函数 */
static void FMDeleteFileHandling(void);
/* 重命名文件 */
static void FMRenameFileHandling(void);
/* 无效控制命令处理函数 */
static void FMInvalidCTWHandling(void);
/* 读文件处理函数 */
static void FMReadFileHandling(void);

/*
***********************************************************************************************************************
* 函数名称: FileManagerInit
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
void FileManagerInit(void)
{
    /* 初始化 */
    s_iFd = FS_FAIL;
    memset((puint8_t)&s_stSysFMReq, 0 , sizeof(SysFMReq_t));
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FileManagerCycle
*
* 功能描述: 周期运行任务
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
void FileManagerCycle(void)
{
    uint32_t uiCTW = 0;                    /* 控制字 */
    bool_t bReadReq = false;               /* 读取请求是否成功 */

    /* 获取来自RTS任务的各种消息 */
    bReadReq = SysReadFMReqMsg(&s_stSysFMReq);

    /* 成功读取请求 */
    if(true == bReadReq)
    {
        /* 获取控制字内容 */
        uiCTW = s_stSysFMReq.uiCtrlWord;

        /* 根据控制字判断消息类别 */
        switch(uiCTW)
        {
            /* 打开文件 */
            case FM_OPEN_FILE:
            {
                FMOpenFileHandling();
            }
            break;

            /* 写文件文件 */
            case FM_WRITE_FILE:
            {
                FMWriteFileHandling();
            }
            break;

            /* 关闭文件 */
            case FM_CLOSE_FILE:
            {
                FMCloseFileHandling();
            }
            break;

            /* 判断文件是否存在 */
            case FM_IS_FILE_EXIST:
            {
                FMIsFileExistHandling();
            }
            break;

            /* 删除文件 */
            case FM_DELETE_FILE:
            {
                FMDeleteFileHandling();
            }
            break;

            /* 重命名文件 */
            case FM_RENAME_FILE:
            {
                FMRenameFileHandling();
            }
            break;

            /* 无效命令 */
            case FM_INVALID_CWT:
            {
                FMInvalidCTWHandling();
            }
            break;

            /* 读文件 */
            case FM_READ_FILE:
            {
                FMReadFileHandling();
            }
            break;

            /* 其它 */
            default:
            {

            }
            break;
        }
    }
    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: FMOpenFileHandling
*
* 功能描述: 打开文件处理函数（文件不存在时创建文件）
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
static void FMOpenFileHandling(void)
{
    puint8_t pucName = NULL;                    /* 指向文件名起始地址 */
    uint32_t uiErr = 0;                         /* 文件打开失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 指向文件名起始地址 */
    pucName = &s_stSysFMReq.ucFileName[0];
    /* 打开文件，若文件不存在，则创建文件 */
    s_iFd = fs_open((const int8_t *)pucName, FS_OPEN_CREATE, FS_WRONLY);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(s_iFd != FS_FAIL)
    {
        /* 文件打开成功 */
        s_stSysFMAck.uiStatusWord |= FM_OPEN_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 文件打开失败 */
        s_stSysFMAck.uiStatusWord &= FM_OPEN_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }
    /* 文件描述符 */
    s_stSysFMAck.iFd = s_iFd;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMWriteFileHandling
*
* 功能描述: 写文件处理函数
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
static void FMWriteFileHandling(void)
{
    puint8_t pucContent = NULL;                 /* 指向待写入内容起始位置 */
    int32_t iWrLen = 0;                         /* 实际写入内容长度 */
    uint32_t uiLen = 0;                         /* 期望写入内容长度 */
    uint32_t uiErr = 0;                         /* 写入失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 指向待写入内容的起始地址 */
    pucContent = &s_stSysFMReq.ucContent[0];
    uiLen = (uint32_t)s_stSysFMReq.usLen;

    /* 将数据写入指定的文件 */
    iWrLen = fs_write(s_iFd, pucContent, uiLen);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(iWrLen == uiLen)
    {
        /* 写文件成功 */
        s_stSysFMAck.uiStatusWord |= FM_WRITE_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 写文件失败 */
        s_stSysFMAck.uiStatusWord &= FM_WRITE_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }
    s_stSysFMAck.iFd = s_iFd;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMCloseFileHandling
*
* 功能描述: 关闭文件处理函数
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
static void FMCloseFileHandling(void)
{
    int32_t iRet = 0;                           /* 关闭文件，返回值 */
    uint32_t uiErr = 0;                         /* 关闭失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 关闭文件 */
    iRet = fs_close(s_iFd);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(FS_OK == iRet)
    {
        /* 关闭文件成功 */
        s_stSysFMAck.uiStatusWord |= FM_CLOSE_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 关闭文件失败 */
        s_stSysFMAck.uiStatusWord &= FM_CLOSE_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }
    s_stSysFMAck.iFd = s_iFd;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMIsFileExistHandling
*
* 功能描述: 判断文件是否存在处理函数
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
static void FMIsFileExistHandling(void)
{
    puint8_t pucName = NULL;                    /* 文件名 */
    uint32_t uiErr = 0;                         /* 文件打开失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */
    int32_t iRet = 0;                           /* 关闭文件fs_close()返回值 */

    /* 指向文件名起始地址 */
    pucName = &s_stSysFMReq.ucFileName[0];
    /* 打开文件，普通打开模式，如果文件不存在则返回失败*/
    s_iFd = fs_open((const int8_t *)pucName, FS_OPEN_NORMAL, FS_RDONLY);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(s_iFd != FS_FAIL)
    {
        /* 文件打开成功 */
        s_stSysFMAck.uiStatusWord |= FM_OPEN_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;

        /* 关闭文件 */
        iRet = fs_close(s_iFd);

        /* 生成应答 */
        if(FS_OK == iRet)
        {
            /* 关闭文件成功 */
            s_stSysFMAck.uiStatusWord |= FM_CLOSE_FILE_ACK;
            s_stSysFMAck.uiErrCode = FM_OK;
        }
        else
        {
            /* 关闭文件失败 */
            s_stSysFMAck.uiStatusWord &= FM_CLOSE_FILE_NACK;
            uiErr = fs_get_err();
            s_stSysFMAck.uiErrCode = uiErr;
        }
        s_stSysFMAck.iFd = s_iFd;
    }
    else
    {
        /* 文件打开失败 */
        s_stSysFMAck.uiStatusWord &= FM_OPEN_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }

    /* 打开与关闭文件均成功 */
    if((FM_OPEN_FILE_ACK|FM_CLOSE_FILE_ACK) == s_stSysFMAck.uiStatusWord)
    {
        s_stSysFMAck.uiStatusWord |= FM_IS_FILE_EXIST_ACK;
    }
    else
    {
        s_stSysFMAck.uiStatusWord &= FM_IS_FILE_EXIST_NACK;
    }

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMDeleteFileHandling
*
* 功能描述: 删除文件处理函数
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
static void FMDeleteFileHandling(void)
{
    puint8_t pucName = NULL;                    /* 文件名 */
    uint32_t uiErr = 0;                         /* 文件打开失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */
    int32_t iRet = FS_FAIL;                     /* fs_remove()返回值 */

    /* 指向文件名起始地址 */
    pucName = &s_stSysFMReq.ucFileName[0];

    /* 删除文件 */
    iRet = fs_remove((const int8_t *)pucName);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(FS_OK == iRet)
    {
        /* 文件删除成功 */
        s_stSysFMAck.uiStatusWord |= FM_DELETE_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 文件删除失败 */
        s_stSysFMAck.uiStatusWord &= FM_DELETE_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }
    s_stSysFMAck.iFd = s_iFd;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMRenameFileHandling
*
* 功能描述: 重命名文件
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
static void FMRenameFileHandling(void)
{
    puint8_t pucOldName = NULL;                 /* 旧文件名 */
    puint8_t pucNewName = NULL;                 /* 新文件名 */
    int32_t iRet = FS_FAIL;                     /* 文件重命名函数返回值 */
    uint32_t uiErr = 0;                         /* 文件打开失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 原文件名 */
    pucOldName = &s_stSysFMReq.ucFileName[0];
    /* 新文件名 */
    pucNewName = &s_stSysFMReq.ucContent[0];

    /* 文件重命名 */
    iRet = fs_rename((const int8_t *)pucOldName, (const int8_t *)pucNewName);

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    if(iRet != FS_FAIL)
    {
        /* 文件重命名成功 */
        s_stSysFMAck.uiStatusWord |= FM_RENAME_FILE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 文件重命名失败 */
        s_stSysFMAck.uiStatusWord &= FM_RENAME_FILE_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMInvalidCTWHandling
*
* 功能描述: 无效控制命令处理函数
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
static void FMInvalidCTWHandling(void)
{
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    s_stSysFMAck.uiStatusWord = FM_INVALIDE_CWT_ACK;
    s_stSysFMAck.uiErrCode = FM_CTW_ERR;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: FMReadFileHandling
*
* 功能描述: 读文件处理函数
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
static void FMReadFileHandling(void)
{
    puint8_t pucContent = NULL;                 /* 指向待读出内容起始位置 */
    int32_t iRdLen = 0;                         /* 实际读出内容长度 */
    uint32_t uiLen = 0;                         /* 期望读出内容长度 */
    uint32_t uiErr = 0;                         /* 读失败，错误码 */
    bool_t bSendAck = false;                    /* 发送应答是否成功 */

    /* 待读取的文件描述符及文件内容长度*/
    s_iFd = s_stSysFMReq.iFd;
    uiLen = (uint32_t)s_stSysFMReq.usLen;

    /* 生成应答 */
    memset((puint8_t)&s_stSysFMAck, 0 , sizeof(SysFMAck_t));

    /* 指向待存储文件内容的起始位置 */
    pucContent = &s_stSysFMAck.ucContent[0];
    /* 将数据从文件中读出 */
    iRdLen = fs_read(s_iFd, pucContent, uiLen);

    s_stSysFMAck.usLen = (uint16_t)iRdLen;

    if(iRdLen == uiLen)
    {
        /* 读文件成功 */
        s_stSysFMAck.uiStatusWord |= FM_READ_FILE_CONTINUE_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else if(iRdLen != FS_FAIL)
    {
        /* 读文件结束 */
        s_stSysFMAck.uiStatusWord |= FM_READ_FILE_END_ACK;
        s_stSysFMAck.uiErrCode = FM_OK;
    }
    else
    {
        /* 读文件失败 */
        s_stSysFMAck.uiStatusWord &= FM_READ_FILE_CONTINUE_NACK;
        s_stSysFMAck.uiStatusWord &= FM_READ_FILE_END_NACK;
        uiErr = fs_get_err();
        s_stSysFMAck.uiErrCode = uiErr;
    }
    s_stSysFMAck.iFd = s_iFd;

    /* 发送应答 */
    do
    {
        bSendAck = SysSendAck(&s_stSysFMAck);
    }while(false == bSendAck);

    return;
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
