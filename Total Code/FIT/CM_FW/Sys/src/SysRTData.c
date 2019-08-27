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
*  功能简介    : 管理实时数据应用模块与内部通讯模块间的资源
*
*  作者        : 李琦
*
*  创建日期    : 2016-02-19
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
static SysRTDataMsgBox_t s_stSysRTDataMsgBox[NUM_OF_PM];
static SysRTDataAckMsgBox_t s_stSysRTDataAckMsgBox[NUM_OF_PM];

/* 实时数据区 */
static RTDataArea_t s_stRTDataArea[NUM_OF_PM][NUM_OF_RTDATA_AREA];

static SharedAppend_t s_stAppend[NUM_OF_PM];

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
    /* 信箱初始化 */
    memset((puint8_t)&s_stSysRTDataMsgBox[0], 0, sizeof(SysRTDataMsgBox_t)*NUM_OF_PM);
    memset((puint8_t)&s_stSysRTDataAckMsgBox[0], 0, sizeof(SysRTDataAckMsgBox_t)*NUM_OF_PM);

    /* 初始化：数据区未加锁，只写操作，写偏移地址为0 */
    memset((puint8_t)&s_stRTDataArea[0], 0, sizeof(RTDataArea_t)*NUM_OF_PM*NUM_OF_RTDATA_AREA);

    memset((puint8_t)&s_stAppend[0], 0, sizeof(SharedAppend_t)*NUM_OF_PM);

    /* 初始化为只写 */
    s_stRTDataArea[PMA][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMA][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMB][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMB][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMC][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMC][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;

    /* Only For Test */
	#if 0
    s_stRTDataArea[PMA][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
    s_stRTDataArea[PMB][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
    s_stRTDataArea[PMC][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_READ_ONLY;

    memset( s_stRTDataArea[PMA][RTDATA_AREA_A].ucContent, 1, 100 );
    memset( s_stRTDataArea[PMA][RTDATA_AREA_A].ucInputContent, 1, 100 );
    memset( s_stRTDataArea[PMA][RTDATA_AREA_A].ucOutputContent, 1, 100 );

    memset( s_stRTDataArea[PMB][RTDATA_AREA_A].ucContent, 1, 100 );
    memset( s_stRTDataArea[PMB][RTDATA_AREA_A].ucInputContent, 1, 100 );
    memset( s_stRTDataArea[PMB][RTDATA_AREA_A].ucOutputContent, 1, 100 );

    memset( s_stRTDataArea[PMC][RTDATA_AREA_A].ucContent, 1, 100 );
    memset( s_stRTDataArea[PMC][RTDATA_AREA_A].ucInputContent, 1, 100 );
    memset( s_stRTDataArea[PMC][RTDATA_AREA_A].ucOutputContent, 1, 100 );
  #endif
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysRTDataClear
*
* 功能描述: 清除实时数据区
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
void SysRTDataClear(void)
{
    /* 初始化为只写 */
    s_stRTDataArea[PMA][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMA][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMB][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMB][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMC][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
    s_stRTDataArea[PMC][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysRTDataClearRelatedPM
*
* 功能描述: 清除对应PM的实时数据区
*
* 输入参数: emPMID: PM的ID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/


void SysRTDataClearRelatedPM(PMController_t emPMID)
{
    if((emPMID < NUM_OF_PM) && (emPMID >= PMA))
    {
        /* 初始化为只写 */
        s_stRTDataArea[emPMID][RTDATA_AREA_A].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
        s_stRTDataArea[emPMID][RTDATA_AREA_B].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;

    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataMsg
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
bool_t SysGetRTDataMsg (PMController_t emPMID, SysRTDataMsgType_t emMsgType, puint8_t pucContent, puint16_t pusLen)
{
    SysRTDataMsgBox_t *pstMsgBox = NULL;
    SysRTDataAckMsgBox_t *pstAckMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if((pucContent != NULL) && (pusLen != NULL))
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_RTDATAMSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            if(REAL_TIME_DATA == emMsgType)
            {
                pstMsgBox = &(s_stSysRTDataMsgBox[emPMID]);

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
                pstAckMsgBox = &(s_stSysRTDataAckMsgBox[emPMID]);

                /* 允许访问且有数据 */
                if((MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (MSGBOX_FULL == pstAckMsgBox->bMsgBoxStatus))
                {
                    /* 锁定信箱，防止其它进程访问 */
                    pstAckMsgBox->bLock = MSGBOX_LOCKED;
                    /* 获取消息内容 */
                    memcpy(pucContent,(puint8_t)&(pstAckMsgBox->stMsg.ucContent[0]),pstAckMsgBox->stMsg.usLen);
                    /* 获取消息长度 */
                    *pusLen = pstAckMsgBox->stMsg.usLen;
                    /* 清空消息内容 */
                    pstAckMsgBox->bMsgBoxStatus = MSGBOX_EMPTY;
                    /* 置读取成功标识 */
                    bRet = true;
                    /* 解除锁定 */
                    pstAckMsgBox->bLock = MSGBOX_UNLOCKED;

                }
                else
                {
                    /* 不允许访问或无数据，置待读出消息内容长度为0 */
                    *pusLen = 0;
                }
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
* 函数名称: SysGetRTDataMsgLen
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
bool_t SysGetRTDataMsgLen (PMController_t emPMID, SysRTDataMsgType_t emMsgType, puint16_t pusLen)
{
    SysRTDataMsgBox_t *pstMsgBox = NULL;
    SysRTDataAckMsgBox_t *pstAckMsgBox = NULL;
    bool_t bRet = false;

    /* 输出参数有效性检查 */
    if(pusLen != NULL)
    {
        /* 输入参数有效性检查 */
        if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_RTDATAMSGTYPE))
        {
            /* 找到待读出信息的信箱 */
            if(REAL_TIME_DATA == emMsgType)
            {
                pstMsgBox = &(s_stSysRTDataMsgBox[emPMID]);

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
                pstAckMsgBox = &(s_stSysRTDataAckMsgBox[emPMID]);

                /* 允许访问且有数据 */
                if((MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (MSGBOX_FULL == pstAckMsgBox->bMsgBoxStatus))
                {
                    /* 锁定信箱，防止其它进程访问 */
                    pstAckMsgBox->bLock = MSGBOX_LOCKED;
                    /* 获取消息长度 */
                    *pusLen = pstAckMsgBox->stMsg.usLen;
                    /* 置读取成功标识 */
                    bRet = true;
                    /* 解除锁定 */
                    pstAckMsgBox->bLock = MSGBOX_UNLOCKED;
                }
                else
                {
                    /* 不允许访问或无数据，置待读出消息内容长度为0 */
                    *pusLen = 0;
                }
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
* 函数名称: SysSetRTDataMsg
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
bool_t SysSetRTDataMsg (PMController_t emPMID, SysRTDataMsgType_t emMsgType , puint8_t pucContent, uint16_t usLen)
{
    bool_t bRet = false;
    SysRTDataMsgBox_t *pstMsgBox = NULL;
    SysRTDataAckMsgBox_t *pstAckMsgBox = NULL;

    /* 输入参数有效性检查 */
    if((emPMID < NUM_OF_PM) && (emPMID >= PMA) &&(emMsgType < NUM_OF_RTDATAMSGTYPE))
    {
        /* 找到待读出信息的信箱 */
        if(REAL_TIME_DATA == emMsgType)
        {
            pstMsgBox = &(s_stSysRTDataMsgBox[emPMID]);

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
            pstAckMsgBox = &(s_stSysRTDataAckMsgBox[emPMID]);

            /* 允许访问且信箱为空 */
            if((MSGBOX_UNLOCKED == pstAckMsgBox->bLock) && (MSGBOX_EMPTY == pstAckMsgBox->bMsgBoxStatus))
            {
                /* 输出参数有效性检查 */
                if((pucContent != NULL) && (usLen != 0))
                {
                    /* 锁定信箱，防止其它进程访问 */
                    pstAckMsgBox->bLock = MSGBOX_LOCKED;
                     /* 获取消息内容 */
                    memcpy((puint8_t)&(pstAckMsgBox->stMsg.ucContent[0]), pucContent,usLen);
                    /* 获取消息长度 */
                    pstAckMsgBox->stMsg.usLen = usLen;
                    /* 清空消息内容 */
                    pstAckMsgBox->bMsgBoxStatus = MSGBOX_FULL;
                    /* 写入成功 */
                    bRet = true;
                    /* 解除锁定 */
                    pstAckMsgBox->bLock = MSGBOX_UNLOCKED;
                }
            }
            else
            {
                /* 不允许访问或信箱为满，则写入失败 */
                bRet = false;
            }
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
* 函数名称: SysGetRTDataAddr
*
* 功能描述: 获取实时数据区起始地址
*
* 输入参数: emPMID: PM索引号
*        emDataArea:数据区类型
*
* 输出参数: 无
*
* 返 回 值  : 实时数据区起始地址
*
* 注意事项: 两个实时数据区均不可读则返回NULL
***********************************************************************************************************************
*/

puint8_t SysGetRTDataAddr(PMController_t emPMID, SysRtDataAreaType_t emDataArea)
{
    puint8_t pucStartAddr = NULL;
    RTDataAreaIndex_t emIndex = INVALID_RTDATA_AREA_INDEX;

    if((emPMID < NUM_OF_PM) && (emDataArea < MAX_RTDATA_AREA_NUM))
    {
        /* 未加锁且为只读区域 */
        if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[emPMID][RTDATA_AREA_A].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[emPMID][RTDATA_AREA_A].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_A;
        }
        else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[emPMID][RTDATA_AREA_B].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[emPMID][RTDATA_AREA_B].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_B;
        }
        else
        {
            emIndex = INVALID_RTDATA_AREA_INDEX;
        }

        if(emIndex != INVALID_RTDATA_AREA_INDEX)
        {
            if(RTDATA_G_AREA == emDataArea)
            {
                /* 获取起始地址 */
                pucStartAddr = (puint8_t)(&s_stRTDataArea[emPMID][emIndex].ucContent[0]);

            }
            else if(RTDATA_I_AREA == emDataArea)
            {
                /* 获取起始地址 */
                pucStartAddr = (puint8_t)(&s_stRTDataArea[emPMID][emIndex].ucInputContent[0]);

            }
            else if(RTDATA_Q_AREA == emDataArea)
            {
                /* 获取起始地址 */
                pucStartAddr = (puint8_t)(&s_stRTDataArea[emPMID][emIndex].ucOutputContent[0]);

            }
            else
            {
                pucStartAddr = NULL;
            }
        }
    }

    return pucStartAddr;
}

#if 0
puint8_t SysGetRTDataAddr(PMController_t emPMID)
{
    puint8_t pucStartAddr = NULL;
    RTDataAreaIndex_t emIndex = INVALID_RTDATA_AREA_INDEX;

    if(emPMID < NUM_OF_PM)
    {
        /* 未加锁且为只读区域 */
        if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[emPMID][RTDATA_AREA_A].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[emPMID][RTDATA_AREA_A].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_A;
        }
        else if((RTDATA_AREA_UNLOCKED == s_stRTDataArea[emPMID][RTDATA_AREA_B].bLock) \
            &&(RTDATA_AREA_READ_ONLY == s_stRTDataArea[emPMID][RTDATA_AREA_B].bDataAreaStatus))
        {
            emIndex = RTDATA_AREA_B;
        }
        else
        {
            emIndex = INVALID_RTDATA_AREA_INDEX;
        }

        if(emIndex != INVALID_RTDATA_AREA_INDEX)
        {
            /* 获取起始地址 */
            pucStartAddr = (puint8_t)(&s_stRTDataArea[emPMID][emIndex].ucContent[0]);
        }
    }

    return pucStartAddr;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: SysGetRTDataArea
*
* 功能描述: 获取实时数据区结构体指针
*
* 输入参数: emPMID: 来自某个PM的实时数据
*
* 输出参数: 无
*
* 返 回 值  : 实时数据区结构体指针
*
* 注意事项: 无
***********************************************************************************************************************
*/
RTDataArea_t* SysGetRTDataArea(PMController_t emPMID)
{
    RTDataArea_t *pstRTDataArea = NULL;

    if(emPMID < NUM_OF_PM)
    {
        pstRTDataArea = &s_stRTDataArea[emPMID][RTDATA_AREA_A];
    }

    return pstRTDataArea;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetAppendInfo
*
* 功能描述: 获取附加区信息
*
* 输入参数: 无
*
* 输出参数: emPMID PM的ID号
*        pstAppend 指向附加区内容
*
* 返 回 值  : 读取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysGetAppendInfo(PMController_t emPMID, SharedAppend_t *pstAppend)
{
    bool_t bRet = false;

    if((pstAppend != NULL) && (emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        memcpy((puint8_t)pstAppend, (puint8_t)&s_stAppend[emPMID], sizeof(SharedAppend_t));
        bRet = true;
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetAppendInfoAddr
*
* 功能描述: 获取附加区信息的地址
*
* 输入参数: 无
*
* 输出参数: emPMID PM的ID号
*
* 返 回 值  : 成功：附加区起始地址
*        失败： NULL
*
* 注意事项: 无
***********************************************************************************************************************
*/
SharedAppend_t * SysGetAppendInfoAddr(PMController_t emPMID)
{
    SharedAppend_t * pstAppendAddr = NULL;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM))
    {
        pstAppendAddr = &s_stAppend[emPMID];
    }

    return pstAppendAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetAppendInfo
*
* 功能描述: 记录附加区内容
*
* 输入参数: emPMID PM的ID号
*        pucAppendInfo 待写入的内容
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SysSetAppendInfo(PMController_t emPMID, puint8_t pucAppendInfo)
{
    bool_t bRet = false;

    if((emPMID >= PMA) && (emPMID < NUM_OF_PM) && (pucAppendInfo!= NULL))
    {
        memcpy((puint8_t)&s_stAppend[emPMID], pucAppendInfo, sizeof(SharedAppend_t));
        //printf("set prjcrc=0x%x\n",*((puint32_t)&pucAppendInfo[12]));
        //printf("set timestamp=0x%x\n",*((puint32_t)&pucAppendInfo[20]));
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
