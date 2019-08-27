/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 内部通讯模块
*
*  文件名称    : InterCommRTData.c
*
*  功能简介    : 对实时数据区进行周期管理
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
#include <stdio.h>
#include <stdlib.h>
#include "../inc/InterCommRTData.h"
#include "Sys/inc/Sys.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static RTDataArea_t* s_pstRTDataArea;
/* 当前写操作的数据区 */
static uint8_t s_ucWrIndex[NUM_OF_PM];
/* 当前读操作的数据区 */
static uint8_t s_ucRdIndex[NUM_OF_PM];
/* 临时存放接收到的实时数据 */
static SysRTDataMsg_t s_stRTDATAMsg;
/* 序列号 */
static uint32_t s_uiSn[NUM_OF_PM];
static uint32_t s_uiSendSn[NUM_OF_PM];

/* 记录实时数据传输完成与否 */
static uint16_t s_usTransFlag[NUM_OF_PM];
/* 存放错误码 */
static uint16_t s_usErrorCode[NUM_OF_PM];

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* 解析实时数据消息的头部 */
static bool_t InterCommParseHeader(RTDataHeader_t *pRTDataHeader, PMController_t emPMID);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: InterCommRTDataInit
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
void InterCommRTDataInit(void)
{
    memset((puint8_t)&s_stRTDATAMsg, 0, sizeof(SysRTDataMsg_t));
    memset((puint8_t)&s_ucWrIndex[0], RTDATA_AREA_A, sizeof(uint8_t)*NUM_OF_PM);
    memset((puint8_t)&s_ucRdIndex[0], INVALID_RTDATA_AREA_INDEX, sizeof(uint8_t)*NUM_OF_PM);
    memset((puint8_t)&s_uiSn[0], 0, sizeof(uint32_t)*NUM_OF_PM);
    memset((puint8_t)&s_usErrorCode[0], RTDATA_MSG_OK, sizeof(uint16_t)*NUM_OF_PM);
    memset((puint8_t)&s_uiSendSn[0], 0, sizeof(uint32_t)*NUM_OF_PM);

    s_usTransFlag[PMA] = RTDATA_MSG_TRANS_CONTINUE_FLAG;
    s_usTransFlag[PMB] = RTDATA_MSG_TRANS_CONTINUE_FLAG;
    s_usTransFlag[PMC] = RTDATA_MSG_TRANS_CONTINUE_FLAG;

    s_pstRTDataArea = NULL;

    return;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: InterCommRTDataReceive
*
* 功能描述: 接收实时数据
*
* 输入参数: emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : true-收到数据
*        false-未收到数据
*
*              --------------------------------------------------------------------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usServiceCode | uiSerialNum | uiOffset | usTransferFlag | usLen |Rt data type| RT data ... | CRC32 |
*              --------------------------------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/
 bool_t InterCommRTDataReceive(PMController_t emPMID)
{
    RTDataHeader_t *pRTDataHeader = NULL;   /* 指向实时数据消息头部 */
    bool_t bCheckHeader = false;            /* 记录实时数据消息头部是否正确  false- 错误*/
    puint8_t pucDst = NULL;                 /* 指向存放接收到数据的目的地址 */
    puint8_t pucSrc = NULL;                 /* 指向接收到数据的起始地址 */
    uint32_t uiOffset = 0;                  /* 在实时数据区中的偏移 */
    uint16_t usMsgLen = 0;                  /* 实时数据消息长度 */
    uint32_t uiCRC32calc = 0;               /* 计算实时数据内容的CRC32 */
    uint32_t uiCRC32 = 0;                   /* 存放接收到的实时数据内容的CRC32 */
    bool_t bRcvFlag = false;                /* 记录是否收到了实时数据消息  false-未收到 */
    uint16_t usDataAreaType = 0;

    uint8_t ucWrIndex = 0;
    uint8_t ucRdIndex = 0;
    
    uint16_t usTransFlag = 0;
    uint16_t usErrorCode = 0;

    if(emPMID < NUM_OF_PM)
    {
        
        usErrorCode = s_usErrorCode[emPMID];
        ucRdIndex = s_ucRdIndex[emPMID];
        ucWrIndex = s_ucWrIndex[emPMID];
        usTransFlag = s_usTransFlag[emPMID];
        
        s_pstRTDataArea = SysGetRTDataArea(emPMID);

        /* 初始化 */
        memset((puint8_t)&s_stRTDATAMsg, 0, sizeof(SysRTDataMsg_t));

        /* 获取实时数据 */
        SysGetRTDataMsg (emPMID, REAL_TIME_DATA, (puint8_t) &(s_stRTDATAMsg.ucContent[0]), &(s_stRTDATAMsg.usLen));

        if(s_stRTDATAMsg.usLen > RTDATA_MSG_HEADER_LEN)
        {
            /* 解析实时数据消息头部 */
            pRTDataHeader = (RTDataHeader_t *)(&s_stRTDATAMsg.ucContent[0]);
            bCheckHeader = InterCommParseHeader(pRTDataHeader, emPMID);
            
            
            usErrorCode = s_usErrorCode[emPMID];
            ucRdIndex = s_ucRdIndex[emPMID];
            ucWrIndex = s_ucWrIndex[emPMID];
            usTransFlag = s_usTransFlag[emPMID];
            
            if(true == bCheckHeader)
            {
                usDataAreaType = (SysRtDataAreaType_t)pRTDataHeader->usAreaType;
                uiOffset = pRTDataHeader->uiOffset;

                if(((RTDATA_TYPE_UP1_G == usDataAreaType) || (RTDATA_TYPE_UP2_G == usDataAreaType)) \
                        && (uiOffset < MAX_RTDATA_G_AREA_LEN))
                {
                    pucDst = &(s_pstRTDataArea[ucWrIndex].ucContent[uiOffset]);
                    printf("#rtd %d\n", *((puint32_t)&s_stRTDATAMsg.ucContent[RTDATA_MSG_HEADER_LEN]));
                }
                else if((RTDATA_TYPE_I == usDataAreaType) && (uiOffset < MAX_RTDATA_I_AREA_LEN))
                {
                    pucDst = &(s_pstRTDataArea[ucWrIndex].ucInputContent[uiOffset]);
                }
                else if((RTDATA_TYPE_Q == usDataAreaType) && (uiOffset < MAX_RTDATA_Q_AREA_LEN))
                {
                    pucDst = &(s_pstRTDataArea[ucWrIndex].ucOutputContent[uiOffset]);
                }
                else if((RTDATA_TYPE_APPEND == usDataAreaType) && (uiOffset < MAX_RTDATA_APPEND_INFO_SIZE))
                {
                    pucDst = &(s_pstRTDataArea[ucWrIndex].ucAppendInfo[uiOffset]);
                }
                else
                {
                    pucDst = NULL;
                }

                pucSrc = &(s_stRTDATAMsg.ucContent[0]);

                /* 指向数据内容起始地址 */
                pucSrc += RTDATA_MSG_HEADER_LEN;
                /* 数据长度 */
                usMsgLen = pRTDataHeader->usLen;
                /* 计算CRC32 */
                uiCRC32calc = SysCrc32Cal(0, pucSrc, usMsgLen);
                /* 取出消息中的CRC32 */
                uiCRC32 = *((puint32_t)&pucSrc[usMsgLen]);
                if((uiCRC32 == uiCRC32calc) && (s_stRTDATAMsg.usLen == RTDATA_MSG_HEADER_LEN + usMsgLen + RTDATA_MSG_CRC32_SIZE))
                {
                    if(pucDst != NULL)
                    {
                        if((RTDATA_AREA_UNLOCKED == s_pstRTDataArea[ucWrIndex].bLock)
                         && (RTDATA_AREA_WRITE_ONLY == s_pstRTDataArea[ucWrIndex].bDataAreaStatus))
                        {
                            s_pstRTDataArea[ucWrIndex].bLock = RTDATA_AREA_LOCKED;

                            /* 将接收到的实时数据区写入到实时数据区 */
                            memcpy(pucDst, pucSrc, usMsgLen);

                            /* 若整个实时数据区传输完毕 */
                            if((RTDATA_MSG_TRANS_FINISH_FLAG == s_usTransFlag[emPMID])&&(RTDATA_TYPE_APPEND == usDataAreaType))
                            {
                                /* 将本区更新为只读区 */
                                ucRdIndex = ucWrIndex;
                                s_pstRTDataArea[ucRdIndex].bDataAreaStatus = RTDATA_AREA_READ_ONLY;
                                s_pstRTDataArea[ucRdIndex].bLock = RTDATA_AREA_UNLOCKED;

                                /* Q区 从8192以后记录实时数据版本号 */
                                SysSetRTDataVersion(emPMID, (RTDataVerInfo_t*) &(s_pstRTDataArea[ucWrIndex].ucAppendInfo[RTDATA_VER_INFO_OFFSET]));

                                SysSetRTDataTimestamp(emPMID, (puint8_t)&(s_pstRTDataArea[ucWrIndex].ucAppendInfo[RTDATA_SYS_TIME_OFFSET]));

                                SysSetAppendInfo(emPMID, (puint8_t)&(s_pstRTDataArea[ucWrIndex].ucAppendInfo[0]));
                                //test start
#if 0
                                {
                                    int i =0;
                                    printf("----rtd-s--------\n");
                                    for (i=0;i<16;i++)
                                    {
                                        printf("%d ",s_pstRTDataArea[ucRdIndex].ucContent[i]);
                                    }
                                    printf("\nucRdIndex=%d\n",ucRdIndex);
                                    printf("----rtd-e--------\n");
                                }
#endif
                                //test end
                                /* 将另一区更新为只写区 */
                                ucWrIndex++;
                                ucWrIndex = (ucWrIndex % NUM_OF_RTDATA_AREA);
                                s_pstRTDataArea[ucWrIndex].bDataAreaStatus = RTDATA_AREA_WRITE_ONLY;
                                
                                //printf("RTDATA_MSG_TRANS_FINISH_FLAG\n");
                                s_ucRdIndex[emPMID] = ucRdIndex;
                                s_ucWrIndex[emPMID] = ucWrIndex;
                                //s_usTransFlag[emPMID] = usTransFlag;
                            }
                            /* 成功接收到有效实时数据 */
                            bRcvFlag = true;
                            usErrorCode = RTDATA_MSG_OK;
                            s_usErrorCode[emPMID] = usErrorCode;
                            s_pstRTDataArea[ucWrIndex].bLock = RTDATA_AREA_UNLOCKED;
                        }
                        else
                        {
                            /* 成功接收到有效实时数据 */
                            bRcvFlag = true;
                            /* 实时数据区无法访问，重传 */
                            usErrorCode = RTDATA_MSG_RETRANS;
                            s_usErrorCode[emPMID] = usErrorCode;
                            /* 序列号复位为 0 */
                            
                            s_uiSn[emPMID] = 0;
                        }
                    }
                    else
                    {
                        /* 成功接收到有效实时数据 */
                        bRcvFlag = true;
                        /* 实时数据溢出，重传 */
                        usErrorCode = RTDATA_MSG_RETRANS;
                        s_usErrorCode[emPMID] = usErrorCode;
                        /* 序列号复位为 0 */
                        
                        s_uiSn[emPMID] = 0;
                    }
                }
                else
                {
                    /* 成功接收到无效实时数据 */
                    bRcvFlag = true;
                    /* CRC错误重传 */
                    usErrorCode = RTDATA_MSG_CRC_ERROR;
                    s_usErrorCode[emPMID] = usErrorCode;
                    /* 序列号复位为 0 */
                    
                    s_uiSn[emPMID] = 0;
                }
            }
            else
            {
                /* 成功接收到无效实时数据 */
                bRcvFlag = true;
                /* 重传 */
                usErrorCode = RTDATA_MSG_RETRANS;
                s_usErrorCode[emPMID] = usErrorCode;
                /* 序列号复位为 0 */
                
                s_uiSn[emPMID] = 0;
            }

            s_usErrorCode[emPMID] = usErrorCode;
            s_ucRdIndex[emPMID] = ucRdIndex;
            s_ucWrIndex[emPMID] = ucWrIndex;
            s_usTransFlag[emPMID] = usTransFlag;
        }
        else
        {
            /* 未收到实时数据 */
            bRcvFlag = false;
        }
    }

    return bRcvFlag;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommRTDataSend
*
* 功能描述: 对实时数据进行应答
*
* 输入参数: emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usServiceCode | uiSerialNum | usLen | usAck | usErrCode | CRC32 |
*              ---------------------------------------------------------------------------------
***********************************************************************************************************************
*/
 void InterCommRTDataSend(PMController_t emPMID)
{
    RTDataAck_t stRTDataAck;                /* 实时数据应答消息 */
    puint8_t pucContent = NULL;             /* 指向实时数据应答消息首地址 */
    uint16_t usLen = 0;                     /* 应答消息长度减去CRC32的长度 */
    uint32_t uiCRC32Calc = 0;               /* 计算应答消息的CRC32 */

    if(emPMID < NUM_OF_PM)
    {
        /* 初始化应答包 */
        memset((puint8_t)&stRTDataAck, 0, sizeof(RTDataAck_t));

        /* 开始标识 */
        stRTDataAck.usStartFlag = RTDATA_MSG_START_FLAG;
        /* 服务码 */
        stRTDataAck.usServiceCode = SRV_CODE_RTDATA;
        /* 序列号 */
        stRTDataAck.uiSerialNum = s_uiSendSn[emPMID];
        /* 应答包长度 */
        stRTDataAck.usLen = RTDATA_MSG_ACK_LEN;
        usLen = stRTDataAck.usLen - RTDATA_MSG_CRC32_SIZE;

        /* 生成ACK */
        //printf("error code =%d\n",s_usErrorCode[emPMID]);
        if(RTDATA_MSG_OK == s_usErrorCode[emPMID])
        {
            stRTDataAck.usAck = RTDATA_MSG_ACK;
        }
        else
        {
            stRTDataAck.usAck = RTDATA_MSG_NAK;
        }

        /* 错误码 */
        stRTDataAck.usErrCode = s_usErrorCode[emPMID];

        /* 指向实时数据应答消息首地址 */
        pucContent = (puint8_t)&stRTDataAck;

        /* 计算CRC32 */
        uiCRC32Calc = SysCrc32Cal(0, pucContent, usLen);
        stRTDataAck.uiCRC32 = uiCRC32Calc;

        /* 生成应答包 */
        SysSetRTDataMsg (emPMID, REAL_TIME_DATA_ACK, pucContent, stRTDataAck.usLen);

        /* 更新下一包预期序列号 */
        //s_uiSn[emPMID]++;
        //printf("s_uiSn=%d\n",s_uiSn[emPMID]);
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommParseHeader
*
* 功能描述: 解析收到的消息头部
*
* 输入参数: pRTDataHeader: 指向头部的指针
*
* 输出参数: 无
*
* 返 回 值  : true-正确，解析通过
*        false-错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommParseHeader(RTDataHeader_t *pRTDataHeader, PMController_t emPMID)
{
    bool_t bRet = false;

    if(pRTDataHeader != NULL)
    {
        /* 开始标识是否正确 */
        if(RTDATA_MSG_START_FLAG == pRTDataHeader->usStartFlag)
        {
            /* 服务码是否正确 */
            if(SRV_CODE_RTDATA == pRTDataHeader->usServiceCode)
            {
                /* 序列号是否正确 */
                printf("expect_sn =%d  recv_sn=%d type=%d\n", s_uiSn[emPMID], pRTDataHeader->uiSerialNum, pRTDataHeader->usAreaType);
                if( 0 == pRTDataHeader->uiSerialNum )
                {
                    s_uiSn[emPMID] = 1;
                    s_uiSendSn[emPMID] = pRTDataHeader->uiSerialNum;
                    s_usErrorCode[emPMID] = RTDATA_MSG_OK;
                    
                    /* 实时数据传输是否完成 */
                    if(RTDATA_MSG_TRANS_CONTINUE_FLAG == pRTDataHeader->usTransferFlag)
                    {
                        /* 传输未完成 */
                        s_usTransFlag[emPMID] = RTDATA_MSG_TRANS_CONTINUE_FLAG;
                        printf("re-continue rt data...\n");
                    }
                    else
                    {
                        /* 传输完成 */
                        s_usTransFlag[emPMID] = RTDATA_MSG_TRANS_FINISH_FLAG;
                        printf("re-finish rt data!\n");
                    }
                    bRet = true;
                }
                else if(s_uiSn[emPMID] == pRTDataHeader->uiSerialNum)
                {
                    /* 实时数据传输是否完成 */
                    if(RTDATA_MSG_TRANS_CONTINUE_FLAG == pRTDataHeader->usTransferFlag)
                    {
                        /* 传输未完成 */
                        s_usTransFlag[emPMID] = RTDATA_MSG_TRANS_CONTINUE_FLAG;
                        printf("continue rt data...\n");
                        s_uiSn[emPMID]++;
                    }
                    else
                    {
                        /* 传输完成 */
                        s_usTransFlag[emPMID] = RTDATA_MSG_TRANS_FINISH_FLAG;
                        printf("finish rt data!\n");

                        if(RTDATA_TYPE_APPEND == (SysRtDataAreaType_t)pRTDataHeader->usAreaType)
                        {
                            s_uiSn[emPMID]=0;
                        }
                        else
                        {
                            s_uiSn[emPMID]++;
                        }
                    }
                    s_usErrorCode[emPMID] = RTDATA_MSG_OK;
                    //s_uiSn[emPMID]++;
                    s_uiSendSn[emPMID] = pRTDataHeader->uiSerialNum;
                    bRet = true;
                }
                else
                {
                    s_uiSn[emPMID] = 0;
                    s_usErrorCode[emPMID] = RTDATA_MSG_SN_ERROR;
                    printf("RTDATA_MSG_SN_ERROR\n");
                }
            }
            else
            {
                s_uiSn[emPMID] = 0;
                s_usErrorCode[emPMID] = RTDATA_MSG_SRV_CODE_ERROR;
            }
        }
        else
        {
            s_uiSn[emPMID] = 0;
            s_usErrorCode[emPMID] = RTDATA_MSG_START_FLAG_ERROR;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
