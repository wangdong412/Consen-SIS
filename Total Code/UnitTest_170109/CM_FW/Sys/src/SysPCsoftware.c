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
*  文件名称    : SysPCsoftware.c
*
*  功能简介    : 管理上位机应用模块与内部通讯模块及外部通讯模块间的资源
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
static SysMsgBox_t s_stSysMsgBox[NUM_OF_PM][NUM_OF_MSGTYPE];     /* 每个PM对应若干种信箱  (例如：SOE请求信箱，SOE应答信箱)*/

/* 用于外部通讯的信箱 */
static SysAppMsgBox_t s_stSysAppReqMsgBox[NUM_OF_SOFTWARETYPE][APP_SW_MAX_NUM];  /* 每种软件的一个连接对应一个请求信箱 */
static SysAppMsgBox_t s_stSysAppRespMsgBox[NUM_OF_SOFTWARETYPE][APP_SW_MAX_NUM]; /* 每种软件的一个连接对应一个应答信箱 */

/* 各种标志 */
static bool_t s_bAppSwConnectFlag[NUM_OF_SOFTWARETYPE][APP_SW_MAX_NUM];
static bool_t s_bAppSwLockedFlag[NUM_OF_SOFTWARETYPE][APP_SW_MAX_NUM];
static bool_t s_bAppSwClearFlag[NUM_OF_SOFTWARETYPE][APP_SW_MAX_NUM];


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* 对软件类型及软件索引号进行合法性检查 */
static bool_t CheckSwTypeAndIndex(SysSoftwareType_t emSwType, uint8_t ucSwIndex);


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysPCsoftwareInit
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
void SysPCsoftwareInit(void)
{
    /* 用于内部通讯的信箱初始化 */
    memset((puint8_t)&s_stSysMsgBox[0][0], 0, sizeof(SysMsgBox_t)*NUM_OF_PM*NUM_OF_MSGTYPE);
    /* 用于外部通讯的信箱初始化 */
    memset((puint8_t)&s_stSysAppReqMsgBox[0][0], 0, sizeof(SysAppMsgBox_t)*NUM_OF_SOFTWARETYPE*APP_SW_MAX_NUM);
    memset((puint8_t)&s_stSysAppRespMsgBox[0][0], 0, sizeof(SysAppMsgBox_t)*NUM_OF_SOFTWARETYPE*APP_SW_MAX_NUM);
    /* 各种标志初始化 */
    memset((puint8_t)&s_bAppSwConnectFlag[0][0], 0, sizeof(bool_t)*NUM_OF_SOFTWARETYPE*APP_SW_MAX_NUM);
    memset((puint8_t)&s_bAppSwLockedFlag[0][0], 0, sizeof(bool_t)*NUM_OF_SOFTWARETYPE*APP_SW_MAX_NUM);
    memset((puint8_t)&s_bAppSwClearFlag[0][0], 0, sizeof(bool_t)*NUM_OF_SOFTWARETYPE*APP_SW_MAX_NUM);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetMsg
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
bool_t SysGetMsg (PMController_t emPMID, SysMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen)
{
    SysMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if((pucContent != NULL) && (pusLen != NULL))
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_MSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysMsgBox[emPMID][emMsgType]);
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
* 函数名称: SysGetMsgLen
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
bool_t SysGetMsgLen (PMController_t emPMID, SysMsgType_t emMsgType, puint16_t pusLen)
{
    SysMsgBox_t *pstMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pusLen != NULL)
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_MSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysMsgBox[emPMID][emMsgType]);
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
* 函数名称: SysSetMsg
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
bool_t SysSetMsg (PMController_t emPMID, SysMsgType_t emMsgType , puint8_t pucContent, uint16_t usLen)
{
    bool_t bRet = false;
    SysMsgBox_t *pstMsgBox = NULL;

    /* 输入参数有效性检查 */
    if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_MSGTYPE))
    {
        /* 找到待写入信息的信箱 */
        pstMsgBox = &(s_stSysMsgBox[emPMID][emMsgType]);
        
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
* 函数名称: SysGetAppMsg
*
* 功能描述: 从系统资源模块获取来自上位机软件的消息（请求/应答）
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: pucContent: 指向待读出的消息内容
*         pusLen: 指向待读出的消息内容长度
*
* 返 回 值  : true-读取成功
*        false- 1)无数据  2)不允许访问  3)输入输出参数错误
*
* 注意事项: 输入参数错误或信箱不允许被访问或信箱为空时，输出参数pusLen值为0
***********************************************************************************************************************
*/
bool_t SysGetAppMsg(SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType,
                  puint8_t pucContent, puint16_t pusLen)
{
    SysAppMsgBox_t *pstMsgBox = NULL;
    bool_t bValide = false;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if((pucContent != NULL) && (pusLen != NULL))
    {
        /* 对软件类型和软件索引进行有效性检查 */
        bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

        if((true == bValide) &&(emMsgType < NUM_OF_APPMSGTYPE))
        {
            if(APP_MSG_REQ == emMsgType)
            {
                /* 找到待读出信息的信箱 */
                pstMsgBox = &(s_stSysAppReqMsgBox[emSwType][ucSwIndex]);
            }
            else
            {
                /* 找到待读出信息的信箱 */
                pstMsgBox = &(s_stSysAppRespMsgBox[emSwType][ucSwIndex]);
            }
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
* 函数名称: SysSetAppMsg
*
* 功能描述: 将上位机软件请求或应答消息写入系统资源模块
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: pucContent: 指向待写入的消息内容
*         pusLen: 指向待写入的消息内容长度
*
* 返 回 值  : 写入成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppMsg (SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType,
                  puint8_t pucContent, uint16_t usLen)
{
    bool_t bRet = false;
    SysAppMsgBox_t *pstMsgBox = NULL;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if((true == bValide) &&(emMsgType < NUM_OF_APPMSGTYPE))
    {
        if(APP_MSG_REQ == emMsgType)
        {
            /* 找到待写入信息的信箱 */
            pstMsgBox = &(s_stSysAppReqMsgBox[emSwType][ucSwIndex]);
        }
        else
        {
            /* 找到待写入信息的信箱 */
            pstMsgBox = &(s_stSysAppRespMsgBox[emSwType][ucSwIndex]);
        }
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
* 函数名称: SysClearAppMsg
*
* 功能描述: 清除系统资源模块获取来自上位机软件的消息（请求/应答）
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*        emMsgType: 消息类型（请求或应答）
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysClearAppMsg(SysSoftwareType_t emSwType, uint8_t ucSwIndex, SysAppMsgType_t emMsgType)
{
    SysAppMsgBox_t *pstMsgBox = NULL;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if((true == bValide) &&(emMsgType < NUM_OF_APPMSGTYPE))
    {
        if(APP_MSG_REQ == emMsgType)
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysAppReqMsgBox[emSwType][ucSwIndex]);
        }
        else
        {
            /* 找到待读出信息的信箱 */
            pstMsgBox = &(s_stSysAppRespMsgBox[emSwType][ucSwIndex]);
        }
        /* 允许访问且有数据 */
        if((MSGBOX_UNLOCKED == pstMsgBox->bLock) && (MSGBOX_FULL == pstMsgBox->bMsgBoxStatus))
        {
            /* 锁定信箱，防止其它进程访问 */
            pstMsgBox->bLock = MSGBOX_LOCKED;
            /* 设置消息长度为0 */
            pstMsgBox->stMsg.usLen = 0;
            /* 清空消息内容 */
            pstMsgBox->bMsgBoxStatus = MSGBOX_EMPTY;
            /* 解除锁定 */
            pstMsgBox->bLock = MSGBOX_UNLOCKED;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetAppSwConnectFlag
*
* 功能描述: 置位"连接"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回true
*        置位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwConnectFlag[emSwType][ucSwIndex] = true;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwConnectFlag
*
* 功能描述: 复位"连接"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回true
*        复位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwConnectFlag[emSwType][ucSwIndex] = false;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwConnectFlag
*
* 功能描述: 获取连接标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "连接"返回true
*        "断开"或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwConnectFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        if(true == s_bAppSwConnectFlag[emSwType][ucSwIndex])
        {
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
* 函数名称: SysSetAppSwLockedFlag
*
* 功能描述: 置位"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回true
*        置位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwLockedFlag[emSwType][ucSwIndex] = true;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwLockedFlag
*
* 功能描述: 复位"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回true
*        复位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwLockedFlag[emSwType][ucSwIndex] = false;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwLockedFlag
*
* 功能描述: 获取"锁定"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "锁定"返回true
*        "未锁定"或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwLockedFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        if(true == s_bAppSwLockedFlag[emSwType][ucSwIndex])
        {
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
* 函数名称: SysSetAppSwClearFlag
*
* 功能描述: 置位"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 置位成功返回true
*        置位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwClearFlag[emSwType][ucSwIndex] = true;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysResetAppSwClearFlag
*
* 功能描述: 复位"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 复位成功返回true
*        复位失败或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysResetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        s_bAppSwClearFlag[emSwType][ucSwIndex] = false;
        bRet = true;
    }
    else
    {
        bRet = false;
    }

    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetAppSwClearFlag
*
* 功能描述: 获取"清除"标志
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : "清除"标识置位，返回true
*        "清除"标识未置位或输入参数错误，返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppSwClearFlag (SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;
    bool_t bValide = false;

    /* 对软件类型和软件索引进行有效性检查 */
    bValide = CheckSwTypeAndIndex(emSwType, ucSwIndex);

    if(true == bValide)
    {
        if(true == s_bAppSwClearFlag[emSwType][ucSwIndex])
        {
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
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CheckSwTypeAndIndex
*
* 功能描述: 对软件类型及软件索引号进行合法性检查
*
* 输入参数: emSwType: 软件类型
*        emSwIndex: 连接的软件索引号
*
* 输出参数: 无
*
* 返 回 值  : 输入参数有效返回true
*        输入参数无效返回false
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t CheckSwTypeAndIndex(SysSoftwareType_t emSwType, uint8_t ucSwIndex)
{
    bool_t bRet = false;

    switch(emSwType)
    {
        case APP_SOE:
            if(ucSwIndex < APP_SW_SOE_NUM)
            {
                bRet = true;
            }
            break;

        case APP_CS1131:
            if(ucSwIndex < APP_SW_CS1131_NUM)
            {
                bRet = true;
            }
            break;

        case APP_OPC:
            if(ucSwIndex < APP_SW_OPC_NUM)
            {
                bRet = true;
            }
            break;

        case APP_CLIENT:
            if(ucSwIndex < APP_SW_CLIENT_NUM)
            {
                bRet = true;
            }
            break;

        case APP_AMS:
            if(ucSwIndex < APP_SW_AMS_NUM)
            {
                bRet = true;
            }
            break;

        default:
            bRet = false;
            break;
    }

    return bRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
