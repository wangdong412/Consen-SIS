/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    :
*
*  文件名称    : DataExChange.c
*
*  功能简介    :
*
*  作者        :  刘阳
*
*  创建日期    : 2016-09-05
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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/DataExChange.h"
#include "Srv/PMBUS/inc/PMBUS.h"
#include "Srv/SharedMem/inc/SharedConfig.h"
#include "Srv/SharedMem/inc/SharedRtData.h"
#include "Srv/SafetyProtocol/inc/PMSafetyComm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint8_t s_ucChannelTolerate[MAX_MOD_NUM][MAX_CHANNEL_NUM];
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static bool_t DataExChangeTriVote(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucData1[], uint8_t ucData2[], uint8_t ucData3[], \
                                  uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[]);

static bool_t DataExChangeDoulVote(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucData1[], uint8_t ucData2[], \
                                  uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[]);

static bool_t DataExChangeSingleVote(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucData1[], \
                                     uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[]);

static bool_t DataExChangeTolerateVote(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[]);

static bool_t DataExChangeBoolTriVote(bool_t bValue1, bool_t bValue2, bool_t bValue3);

static bool_t DataExChangeBoolDoulVote(bool_t bValue1, bool_t bValue2, VoteType_t emVoteType);

static void DataExChangeAddTolerate(uint8_t ucModAddr, uint8_t ucChannelID);

static void DataExChangeClearTolerate(uint8_t ucModAddr, uint8_t ucChannelID);

static uint8_t DataExChangeCheckTolerate(uint8_t ucModAddr, uint8_t ucChannelID);

static void DataExChangeModForceVar(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucOldData[],uint8_t ucDestBuff[]);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: DataExChangeInit
*
* 功能描述: 数据交换模块初始化
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
void DataExChangeInit(void)
{
    memset(&s_ucChannelTolerate[0][0], 0x00, MAX_MOD_NUM*MAX_CHANNEL_NUM);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChange
*
* 功能描述: 数据交换模块
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
void DataExChange(uint8_t ucSrcData[], uint16_t usSrcDataLen, \
                  uint8_t ucPreData[], uint8_t ucNextData[], puint8_t pucPreDataFlg, puint8_t pucNextDataFlg)
{
    uint16_t usState = 0U; 
    PMController_t emLocalPMId = SysGetLocalPMID();                                     /* 本PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);                            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);                              /* 前一个PMID号     */
    uint32_t uiRecordCon = 0U;                                                          /* 记录等待应答标识         */
    bool_t bSendRet = false;
    bool_t bRecvRet = false;
    uint16_t usDataLen;
    SyncTime64_t stStartTime;                                                           /* 开始时间结构                */
    SyncTime64_t stCurrentTime;                                                         /* 当前时间结构                */
    
    *pucPreDataFlg = 0x00;
    *pucNextDataFlg = 0x00;
    
    clear_precv_status(1);
    clear_precv_status(2);

    udelay(500);

    SysGetSyncTime(&stStartTime);

    /* 获取硬件提供状态值 */
    usState = SysGetModularOnLine();
    if(!(usState & BITMASK(0)))
    {/*若后一系不在线*/
        uiRecordCon |= BITMASK(emNextPMId);
    }
    else
    {
        /* 置需接收后一个控制器应答标识 */
        uiRecordCon &= ~(BITMASK(emNextPMId));
        bSendRet = DataExChangeSend(emNextPMId, &ucSrcData[0], usSrcDataLen);
        if(bSendRet != true)
        {
            printf("Send N Err \n");
        }
    }
    
    if(!(usState & BITMASK(1)))
    {/*若前一系不在线*/

        uiRecordCon |= BITMASK(emPrePMId);
    }
    else
    {
        uiRecordCon &= ~(BITMASK(emPrePMId));
        bSendRet = DataExChangeSend(emPrePMId, &ucSrcData[0], usSrcDataLen);
        if(bSendRet != true)
        {
            printf("Send P Err \n");
        }
    }
    
    uiRecordCon |= BITMASK(emLocalPMId);
    
    do
    {
        bRecvRet = DataExChangeRecv(emPrePMId, &ucPreData[0],&usDataLen);
        if((true == bRecvRet) && (usDataLen > 0))
        {
            uiRecordCon |= BITMASK(emPrePMId);
            *pucPreDataFlg = 0x01;
        }
        
        bRecvRet = DataExChangeRecv(emNextPMId, &ucNextData[0],&usDataLen);
        if((true == bRecvRet) && (usDataLen > 0))
        {
            uiRecordCon |= BITMASK(emNextPMId);
            *pucNextDataFlg = 0x01;
        }

        SysGetSyncTime(&stCurrentTime); 
    }while(((stCurrentTime.stTimeStruct.uiLow32b - stStartTime.stTimeStruct.uiLow32b) < 10) && (SysGetBin1Num(uiRecordCon) < NUM_OF_PM));

    if((stCurrentTime.stTimeStruct.uiLow32b - stStartTime.stTimeStruct.uiLow32b) >= 10)
    {
        printf("TO\n");
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeSend
*
* 功能描述: IO数据发送
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
bool_t DataExChangeSend(PMController_t emPMID , uint8_t ucSendDest[], uint16_t usDataLen)
{
    bool_t bRet = false;
    bool_t bMakeFraRet = false;
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PMID号     */
    uint8_t ucSendBuff[MAX_IO_EXCHANGE_SIZE];
    uint16_t usSendLen = 0U;
    int16_t sRetSend = -1;
    
    if((usDataLen <= MAX_IO_EXCHANGE_SIZE) && (ucSendDest != NULL) && (emPMID < NUM_OF_PM))
    {
        /*安全层打包接口编译有问题，请王东解决*/
        /*将IO数据进行安全层打包*/
//        bMakeFraRet = PMMakeFra(emPMID, &ucSendDest[0], usDataLen, &ucSendBuff[0], &usSendLen);

/*test*/
        usSendLen = usDataLen;
        bMakeFraRet = true;
        memcpy((puint8_t)&ucSendBuff[0], (puint8_t)&ucSendDest[0], usDataLen);
        
/*test*/

        if(bMakeFraRet != true)
        {
            /*log*/
        }
        else
        {
            if(usSendLen > MAX_IO_EXCHANGE_SIZE)
            {
                /*log*/
            }
            else
            {
                if(emPMID == emNextPMId)
                {
                    sRetSend = PMBUS_Send(PM_BUS_A, PM_BUFF_EXDATA, (puint8_t)&ucSendBuff[0], \
                            (int32_t)usSendLen, PMBUS_SEND_TIMEOUT);
                }
                
                if(emPMID == emPrePMId)
                {
                    sRetSend = PMBUS_Send(PM_BUS_B, PM_BUFF_EXDATA, (puint8_t)&ucSendBuff[0], \
                            (int32_t)usSendLen, PMBUS_SEND_TIMEOUT);
                }
                
                if(0 == sRetSend)
                {
                    bRet = true;
                }
            }
        }
    }
    
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeRecv
*
* 功能描述: IO数据接收
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
bool_t DataExChangeRecv(PMController_t emPMID , uint8_t ucRecvData[], puint16_t pusDataLen)
{
    bool_t bRet = false;
    bool_t bDecodeFraRet = false;
    PMController_t emLocalPMId = SysGetLocalPMID();                     /* 本地PMID号           */
    PMController_t emNextPMId = SysGetNextPMID(emLocalPMId);            /* 下一个PMID号     */
    PMController_t emPrePMId = SysGetPrePMID(emLocalPMId);              /* 前一个PMID号     */
    uint8_t ucRecvBuff[MAX_IO_EXCHANGE_SIZE];
    uint16_t usRecvLen = 0U;
    int16_t sRetRecv= -1;

    *pusDataLen = 0U;
    if((ucRecvData != NULL) && (emPMID < NUM_OF_PM))
    {
        
        if(emPMID == emNextPMId)
        {
            /* 接收链路A即后一个 PM 发送的服务数据 */
            sRetRecv = PMBUS_Receive(PM_BUS_A, PM_BUFF_EXDATA, (puint8_t)&ucRecvBuff[0]);
        }
        
        if(emPMID == emPrePMId)
        {
            /* 接收链路B即前一个 PM 发送的服务数据 */
            sRetRecv = PMBUS_Receive(PM_BUS_B, PM_BUFF_EXDATA, (puint8_t)&ucRecvBuff[0]);
        }
        
        
        if((sRetRecv > 0) && (sRetRecv <= MAX_IO_EXCHANGE_SIZE))
        {
            /*安全层解包接口编译有问题，请王东解决*/
            /*将数据经安全层解包*/
//            bDecodeFraRet = PMDecodeFra(emPMID, &ucRecvBuff[0], (uint16_t)sRetRecv, &ucRecvData[0], &usRecvLen);
            
/*test*/
            bDecodeFraRet = true;
            usRecvLen = (uint16_t)sRetRecv;
            memcpy((puint8_t)&ucRecvData[0],(puint8_t)&ucRecvBuff[0],usRecvLen);
            
/*test*/
            
            
            if(bDecodeFraRet != true)
            {
                /*log*/
                printf("PMDecodeFra err\n");
            }
            else
            {
                if(usRecvLen < MAX_IO_EXCHANGE_SIZE)
                {
                    bRet = true;
                    *pusDataLen = usRecvLen;
                }
            }
        }
    }
    
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeVote
*
* 功能描述: IO数据表决
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
bool_t DataExChangeVote(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucData1[], uint8_t ucData2[], uint8_t ucData3[], \
                        uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[])
{
    bool_t bRet = false;
    
    if((ucOldData != NULL) && (ucDestBuff != NULL) && (ucDiagBuff != NULL))
    {
        if((ucData1 != NULL) && (ucData2 != NULL) && (ucData3 != NULL))
        {
            bRet = DataExChangeTriVote(emTaskID, ucModID , ucData1, ucData2, ucData3, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if((ucData1 != NULL) && (ucData2 != NULL))
        {
            bRet = DataExChangeDoulVote(emTaskID, ucModID ,ucData1, ucData2, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if((ucData1 != NULL) && (ucData3 != NULL))
        {
            bRet = DataExChangeDoulVote(emTaskID, ucModID ,ucData1, ucData3, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if((ucData2 != NULL) && (ucData3 != NULL))
        {
            bRet = DataExChangeDoulVote(emTaskID, ucModID ,ucData2, ucData3, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if(ucData1 != NULL)
        {
            bRet = DataExChangeSingleVote(emTaskID, ucModID, ucData1, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if(ucData2 != NULL)
        {
            bRet = DataExChangeSingleVote(emTaskID, ucModID, ucData2, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else if(ucData3 != NULL)
        {
            bRet = DataExChangeSingleVote(emTaskID, ucModID, ucData3, ucOldData, ucDestBuff, ucDiagBuff);
        }
        else
        {
            bRet = DataExChangeTolerateVote(emTaskID, ucModID, ucOldData, ucDestBuff, ucDiagBuff);
        }
        
        /*模块通道强制*/
        DataExChangeModForceVar(emTaskID, ucModID, ucOldData, ucDestBuff);
        
    }
    
   return bRet;
}




/*
***********************************************************************************************************************
* 函数名称: SysGetMedianValue
*
* 功能描述: 用于获取三个值之中的中值（uint16_t）
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
uint16_t SysGetMedianValue(uint16_t usValue1, uint16_t usValue2, uint16_t usValue3)
{
    uint16_t a = 0;
    uint16_t b = 0;
    uint16_t c = 0;
    uint16_t d = 0;
    
    a = usValue1;
    b = usValue2;
    c = usValue3;
    
    if(a < b)
    {
        d = a; a = b; b = d;
    }
    if(b < c)
    {
        d = b; b = c; c = d;
    }
    if(a < b)
    {
        d = a; a = b; b = d;
    }
    
    return b;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFloatMedianValue
*
* 功能描述: 用于获取三个浮点值之中的中值
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
float_t SysGetFloatMedianValue(float_t fValue1, float_t fValue2, float_t fValue3)
{
    float_t a = 0;
    float_t b = 0;
    float_t c = 0;
    float_t d = 0;
    
    a = fValue1;
    b = fValue2;
    c = fValue3;
    
    if(a < b)
    {
        d = a; a = b; b = d;
    }
    if(b < c)
    {
        d = b; b = c; c = d;
    }
    if(a < b)
    {
        d = a; a = b; b = d;
    }
    
    return b;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetDeviation
*
* 功能描述: 用于获取两个值的差值(uint16_t)
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
uint16_t SysGetDeviation(uint16_t usValue1, uint16_t usValue2)
{
    uint16_t usRet = 0;
    
    if(usValue1 < usValue2)
    {
        usRet = usValue2 - usValue1;
    }
    else
    {
        usRet = usValue1 - usValue2;
    }
    
    return usRet;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: DataExChangeTriVote
*
* 功能描述: 用于模块通道级三系的表决
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
static bool_t DataExChangeTriVote(Task_ID_t emTaskID,uint8_t ucModID , uint8_t ucData1[], uint8_t ucData2[], uint8_t ucData3[], \
                                  uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[])
{
    bool_t bRet = false;
    uint16_t usModType = 0U;
    VoteVarType_t emVoteVarType = VVT_INVALID_ID;
    VoteModInfo_t* pstVoteModInfo = NULL;
    bool_t bValue1 = false;
    bool_t bValue2 = false;
    bool_t bValue3 = false;
    bool_t bVoteValue = false;
    uint16_t usValue1 = 0;
    uint16_t usValue2 = 0;
    uint16_t usValue3 = 0;
    uint16_t usMedianValue = 0;
    uint16_t usVoteValue = 0;
    float_t fValue1 = 0.0;
    float_t fValue2 = 0.0;
    float_t fValue3 = 0.0;
    float_t fVoteValue = 0.0;
    uint16_t usChannelNum = 0U;
    uint16_t usCount = 0U;
    uint8_t ucQualityBit1 = true;
    uint8_t ucQualityBit2 = true;
    uint8_t ucQualityBit3 = true;
    uint8_t ucQualityBit = true;
    uint8_t ucChannelQuality[3][4]; /* 三系通道质量 */
    uint8_t ucDataLen = 0U;
    uint8_t ucChannelQualityLen = 0U;
    uint8_t ucBuffIndex = 0U; /* 用于从通道数据中取值 */
    uint8_t ucByteIndex = 0U; /* 用于从通道质量中取值 */
    uint8_t ucBitIndex = 0U; /* 用于从通道质量中取值，若为bool量，也用于从通道数据中取值 */
    
    usModType = SharedGetModType(emTaskID,ucModID);
    pstVoteModInfo = SharedGetVoteInfo(ucModID);
    
    switch(usModType)
    {
        case MODTAG_AI:
        {
            emVoteVarType = VVT_ANALOG_AI;
            ucDataLen = sizeof(uint16_t) * 32;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[2][0], &ucData3[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        case MODTAG_AO:
        {
            emVoteVarType = VVT_ANALOG_AO;
            ucDataLen = sizeof(uint16_t)*16;
            ucChannelQualityLen = 2;
            memset(&ucChannelQuality[0][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[1][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[2][0], 0xFF, ucChannelQualityLen);

        }
        break;
        
        case MODTAG_DI:
        {
            emVoteVarType = VVT_DIGITAL_DO;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[2][0], &ucData3[ucDataLen], ucChannelQualityLen);
        }
        break;

        case MODTAG_DO:
        {
            emVoteVarType = VVT_DIGITAL_DO;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memset(&ucChannelQuality[0][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[1][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[2][0], 0xFF, ucChannelQualityLen);
        }
        break;

        case MODTAG_OSP:
        {
            emVoteVarType = VVT_FLOAT;
            ucDataLen = sizeof(float_t)*11;
            ucChannelQualityLen =2;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[2][0], &ucData3[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        default:
        {
            emVoteVarType = VVT_INVALID_ID;
        }
        break;
    }
    if((pstVoteModInfo != NULL) && (emVoteVarType < VVT_INVALID_ID))
    {
        usChannelNum = pstVoteModInfo->usChannelCount;
        
        for(usCount = 0U;usCount < usChannelNum; usCount++)
        {
            /*获取通道质量位*/
            ucByteIndex = (usCount/8);
            ucBitIndex = (usCount%8);
            
            ucQualityBit1 = ucChannelQuality[0][ucByteIndex];
            ucQualityBit2 = ucChannelQuality[1][ucByteIndex];
            ucQualityBit3 = ucChannelQuality[2][ucByteIndex];
            
            ucQualityBit1 = (ucQualityBit1 >> ucBitIndex) & 0x01;
            ucQualityBit2 = (ucQualityBit2 >> ucBitIndex) & 0x01;
            ucQualityBit3 = (ucQualityBit3 >> ucBitIndex) & 0x01;
            
            if((VVT_ANALOG_AO == emVoteVarType) || (VVT_ANALOG_AI == emVoteVarType))
            {
                ucBuffIndex = sizeof(uint16_t)*usCount;
                
                memcpy(&usValue1, &ucData1[ucBuffIndex], sizeof(uint16_t));
                memcpy(&usValue2, &ucData2[ucBuffIndex], sizeof(uint16_t));
                memcpy(&usValue3, &ucData3[ucBuffIndex], sizeof(uint16_t));
                
                usMedianValue = SysGetMedianValue(usValue1, usValue2, usValue3);
                
                if((SysGetDeviation(usValue1, usValue2) > MAX_ANALOG_DEVIATION) && \
                   (SysGetDeviation(usValue1, usValue3) > MAX_ANALOG_DEVIATION))
                {
                    ucQualityBit1 = false;
                }
                if((SysGetDeviation(usValue2, usValue1) > MAX_ANALOG_DEVIATION) && \
                   (SysGetDeviation(usValue2, usValue3) > MAX_ANALOG_DEVIATION))
                {
                    ucQualityBit2 = false;
                }
                if((SysGetDeviation(usValue3, usValue1) > MAX_ANALOG_DEVIATION) && \
                   (SysGetDeviation(usValue3, usValue2) > MAX_ANALOG_DEVIATION))
                {
                    ucQualityBit3 = false;
                }
                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2) && (true == ucQualityBit3))
                {/*三系质量均为好，取中*/
                    usVoteValue = usMedianValue;
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    usVoteValue = (usValue1 + usValue2) / 2;
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit2) && (true == ucQualityBit3))
                {
                    usVoteValue = (usValue2 + usValue3) / 2;
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit3))
                {
                    usVoteValue = (usValue1 + usValue3) / 2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    usVoteValue = usValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    usVoteValue = usValue2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit3)
                {
                    usVoteValue = usValue3;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&usVoteValue, &ucOldData[ucBuffIndex], sizeof(uint16_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                    }
                    else
                    {
                        usVoteValue = (uint16_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                        DataExChangeClearTolerate(ucModID, usCount);
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &usVoteValue, sizeof(uint16_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if((VVT_DIGITAL_DO == emVoteVarType) || (VVT_DIGITAL_DI == emVoteVarType))
            {
                ucBuffIndex = (usCount/8);
                ucBitIndex = (usCount%8);

                memcpy(&bValue1, &ucData1[ucBuffIndex], sizeof(bool_t));
                memcpy(&bValue2, &ucData2[ucBuffIndex], sizeof(bool_t));
                memcpy(&bValue3, &ucData3[ucBuffIndex], sizeof(bool_t));

                bValue1 = (bValue1 >> ucBitIndex) & 0x01;
                bValue2 = (bValue2 >> ucBitIndex) & 0x01;
                bValue3 = (bValue3 >> ucBitIndex) & 0x01;
                
                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2) && (true == ucQualityBit3))
                {/*三系质量均为好，取中*/
                    bVoteValue = DataExChangeBoolTriVote(bValue1, bValue2, bValue3);
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    bVoteValue = DataExChangeBoolDoulVote(bValue1, bValue2, \
                                                          pstVoteModInfo->stVoteChannelInfo[usCount].ucVoteType);
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit2) && (true == ucQualityBit3))
                {
                    bVoteValue = DataExChangeBoolDoulVote(bValue2, bValue3, \
                                                          pstVoteModInfo->stVoteChannelInfo[usCount].ucVoteType);
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit3))
                {
                    bVoteValue = DataExChangeBoolDoulVote(bValue1, bValue3, \
                                                          pstVoteModInfo->stVoteChannelInfo[usCount].ucVoteType);
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    bVoteValue = bValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    bVoteValue = bValue2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit3)
                {
                    bVoteValue = bValue3;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&bVoteValue, &ucOldData[ucBuffIndex], sizeof(bool_t));
                        bVoteValue = (bVoteValue >> ucBitIndex) & 0x01;
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        bVoteValue = (bool_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                /*强表决值填入BUFF*/
                ucDestBuff[ucBuffIndex] = ucDestBuff[ucBuffIndex] | (bVoteValue << ucBitIndex);
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if(VVT_FLOAT == emVoteVarType)
            {
                ucBuffIndex = sizeof(float_t)*usCount;

                memcpy(&fValue1, &ucData1[ucBuffIndex], sizeof(float_t));
                memcpy(&fValue2, &ucData2[ucBuffIndex], sizeof(float_t));
                memcpy(&fValue3, &ucData3[ucBuffIndex], sizeof(float_t));
                
                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2) && (true == ucQualityBit3))
                {/*三系质量均为好，取中*/
                    fVoteValue = SysGetFloatMedianValue(fValue1, fValue2, fValue3);
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    fVoteValue = (fValue1 + fValue2) / 2;
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit2) && (true == ucQualityBit3))
                {
                    fVoteValue = (fValue2 + fValue3) / 2;
                    ucQualityBit = true;
                }
                else if((true == ucQualityBit1) && (true == ucQualityBit3))
                {
                    fVoteValue = (fValue1 + fValue3) / 2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    fVoteValue = fValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    fVoteValue = fValue2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit3)
                {
                    fVoteValue = fValue3;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&fVoteValue, &ucOldData[ucBuffIndex], sizeof(float_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        fVoteValue = (float_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &fVoteValue, sizeof(float_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
            }
        }
        bRet = true;
    }
    return bRet;
    
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeDoulVote
*
* 功能描述: 用于模块通道级两系的表决
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
static bool_t DataExChangeDoulVote(Task_ID_t emTaskID,uint8_t ucModID , uint8_t ucData1[], uint8_t ucData2[], \
                                  uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[])
{
    bool_t bRet = false;
    uint16_t usModType = 0U;
    VoteVarType_t emVoteVarType = VVT_INVALID_ID;
    VoteModInfo_t* pstVoteModInfo = NULL;
    bool_t bValue1 = false;
    bool_t bValue2 = false;
    bool_t bVoteValue = false;
    uint16_t usValue1 = 0;
    uint16_t usValue2 = 0;
    uint16_t usVoteValue = 0;
    float_t fValue1 = 0.0;
    float_t fValue2 = 0.0;
    float_t fVoteValue = 0.0;
    uint16_t usChannelNum = 0U;
    uint16_t usCount = 0U;
    uint8_t ucQualityBit1 = true;
    uint8_t ucQualityBit2 = true;
    uint8_t ucQualityBit = true;
    uint8_t ucChannelQuality[2][4];
    uint8_t ucDataLen = 0U;
    uint8_t ucChannelQualityLen = 0U;
    uint8_t ucBuffIndex = 0U;                               /*用于从通道数据中取值*/
    uint8_t ucByteIndex = 0U;                               /*用于从通道质量中取值*/
    uint8_t ucBitIndex = 0U;                                /*用于从通道质量中取值，若为bool量，也用于从通道数据中取值*/
    
    usModType = SharedGetModType(emTaskID,ucModID);
    pstVoteModInfo = SharedGetVoteInfo(ucModID);
    
    switch(usModType)
    {
        case MODTAG_AI:
        {
            emVoteVarType = VVT_ANALOG_AI;
            ucDataLen = sizeof(uint16_t)*32;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        case MODTAG_AO:
        {
            emVoteVarType = VVT_ANALOG_AO;
            ucDataLen = sizeof(uint16_t)*16;
            ucChannelQualityLen = 2;
            memset(&ucChannelQuality[0][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[1][0], 0xFF, ucChannelQualityLen);
        }
        break;
        
        case MODTAG_DI:
        {
            emVoteVarType = VVT_DIGITAL_DI;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
        }
        break;

        case MODTAG_DO:
        {
            emVoteVarType = VVT_DIGITAL_DO;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memset(&ucChannelQuality[0][0], 0xFF, ucChannelQualityLen);
            memset(&ucChannelQuality[1][0], 0xFF, ucChannelQualityLen);
        }
        break;

        case MODTAG_OSP:
        {
            emVoteVarType = VVT_FLOAT;
            ucDataLen = sizeof(float_t)*11;
            ucChannelQualityLen =2;
            memcpy(&ucChannelQuality[0][0], &ucData1[ucDataLen], ucChannelQualityLen);
            memcpy(&ucChannelQuality[1][0], &ucData2[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        default:
        {
            emVoteVarType = VVT_INVALID_ID;
        }
        break;
    }
    if((pstVoteModInfo != NULL) && (emVoteVarType < VVT_INVALID_ID))
    {

        usChannelNum = pstVoteModInfo->usChannelCount;
        
        for(usCount = 0U;usCount < usChannelNum; usCount++)
        {
            /*获取通道质量位*/
            ucByteIndex = (usCount/8);
            ucBitIndex = (usCount%8);
            
            ucQualityBit1 = ucChannelQuality[0][ucByteIndex];
            ucQualityBit2 = ucChannelQuality[1][ucByteIndex];
            
            ucQualityBit1 = (ucQualityBit1 >> ucBitIndex) & 0x01;
            ucQualityBit2 = (ucQualityBit2 >> ucBitIndex) & 0x01;
            
            if((VVT_ANALOG_AO == emVoteVarType) || (VVT_ANALOG_AI == emVoteVarType))
            {
                ucBuffIndex = sizeof(uint16_t)*usCount;
                
                memcpy(&usValue1, &ucData1[ucBuffIndex], sizeof(uint16_t));
                memcpy(&usValue2, &ucData2[ucBuffIndex], sizeof(uint16_t));
                
                
                if(SysGetDeviation(usValue1, usValue2) > MAX_ANALOG_DEVIATION)
                {
                    ucQualityBit1 = false;
                    ucQualityBit2 = false;
                }

                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    usVoteValue = (usValue1 + usValue2) / 2;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    usVoteValue = usValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    usVoteValue = usValue2;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&usVoteValue, &ucOldData[ucBuffIndex], sizeof(uint16_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        usVoteValue = (uint16_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &usVoteValue, sizeof(uint16_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if((VVT_DIGITAL_DO == emVoteVarType) || (VVT_DIGITAL_DI == emVoteVarType))
            {
                ucBuffIndex = (usCount/8);
                ucBitIndex = (usCount%8);

                memcpy(&bValue1, &ucData1[ucBuffIndex], sizeof(bool_t));
                memcpy(&bValue2, &ucData2[ucBuffIndex], sizeof(bool_t));

                bValue1 = (bValue1 >> ucBitIndex) & 0x01;
                bValue2 = (bValue2 >> ucBitIndex) & 0x01;
                
                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    bVoteValue = DataExChangeBoolDoulVote(bValue1, bValue2, \
                                                          pstVoteModInfo->stVoteChannelInfo[usCount].ucVoteType);
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    bVoteValue = bValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    bVoteValue = bValue2;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&bVoteValue, &ucOldData[ucBuffIndex], sizeof(bool_t));
                        bVoteValue = (bVoteValue >> ucBitIndex) & 0x01;
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        bVoteValue = (bool_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                /*强表决值填入BUFF*/
                ucDestBuff[ucBuffIndex] = ucDestBuff[ucBuffIndex] | (bVoteValue << ucBitIndex);
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if(VVT_FLOAT == emVoteVarType)
            {
                ucBuffIndex = sizeof(float_t)*usCount;

                memcpy(&fValue1, &ucData1[ucBuffIndex], sizeof(float_t));
                memcpy(&fValue2, &ucData2[ucBuffIndex], sizeof(float_t));
                
                /*获取最终表决值*/
                if((true == ucQualityBit1) && (true == ucQualityBit2))
                {
                    if(fValue1 >= fValue2)
                    {
                        fVoteValue = fValue1;
                    }
                    else
                    {
                        fVoteValue = fValue2;
                    }

                    ucQualityBit = true;
                }
                else if(true == ucQualityBit1)
                {
                    fVoteValue = fValue1;
                    ucQualityBit = true;
                }
                else if(true == ucQualityBit2)
                {
                    fVoteValue = fValue2;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&fVoteValue, &ucOldData[ucBuffIndex], sizeof(float_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        fVoteValue = (float_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &fVoteValue, sizeof(float_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
            }
        }
        bRet = true;
    }
    
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeSingleVote
*
* 功能描述: 用于模块通道级单系的表决
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
static bool_t DataExChangeSingleVote(Task_ID_t emTaskID,uint8_t ucModID , uint8_t ucData1[], \
                                     uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[])
{
    bool_t bRet = false;
    uint16_t usModType = 0U;
    VoteVarType_t emVoteVarType = VVT_INVALID_ID;
    VoteModInfo_t* pstVoteModInfo = NULL;
    bool_t bValue1 = false;
    bool_t bVoteValue = false;
    uint16_t usValue1 = 0;
    uint16_t usVoteValue = 0;
    float_t fValue1 = 0.0;
    float_t fVoteValue = 0.0;
    uint16_t usChannelNum = 0U;
    uint16_t usCount = 0U;
    uint8_t ucQualityBit1 = true;
    uint8_t ucQualityBit = true;
    uint8_t ucChannelQuality[4];
    uint8_t ucDataLen = 0U;
    uint8_t ucChannelQualityLen = 0U;
    uint8_t ucBuffIndex = 0U;                               /*用于从通道数据中取值*/
    uint8_t ucByteIndex = 0U;                               /*用于从通道质量中取值*/
    uint8_t ucBitIndex = 0U;                                /*用于从通道质量中取值，若为bool量，也用于从通道数据中取值*/
    
    usModType = SharedGetModType(emTaskID, ucModID);
    pstVoteModInfo = SharedGetVoteInfo(ucModID);
    
    switch(usModType)
    {
        case MODTAG_AI:
        {
            emVoteVarType = VVT_ANALOG_AI;
            ucDataLen = sizeof(uint16_t)*32;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0], &ucData1[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        case MODTAG_AO:
        {
            emVoteVarType = VVT_ANALOG_AO;
            ucDataLen = sizeof(uint16_t)*16;
            ucChannelQualityLen = 2;
            memset(&ucChannelQuality[0], 0xFF, ucChannelQualityLen);

        }
        break;
        
        case MODTAG_DI:
        {
            emVoteVarType = VVT_DIGITAL_DI;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memcpy(&ucChannelQuality[0], &ucData1[ucDataLen], ucChannelQualityLen);
        }
        break;

        case MODTAG_DO:
        {
            emVoteVarType = VVT_DIGITAL_DO;
            ucDataLen = 32/8;
            ucChannelQualityLen = 4;
            memset(&ucChannelQuality[0], 0xFF, ucChannelQualityLen);
        }
        break;
        case MODTAG_OSP:
        {
            emVoteVarType = VVT_FLOAT;
            ucDataLen = sizeof(float_t)*11;
            ucChannelQualityLen =2;
            memcpy(&ucChannelQuality[0], &ucData1[ucDataLen], ucChannelQualityLen);
        }
        break;
        
        default:
        {
            emVoteVarType = VVT_INVALID_ID;
        }
        break;
    }
    if((pstVoteModInfo != NULL) && (emVoteVarType < VVT_INVALID_ID))
    {
        usChannelNum = pstVoteModInfo->usChannelCount;

//        printf("CN=%d\n",usChannelNum);
        for(usCount = 0U;usCount < usChannelNum; usCount++)
        {
            /*获取通道质量位*/
            ucByteIndex = (usCount/8);
            ucBitIndex = (usCount%8);

            ucQualityBit1 = ucChannelQuality[ucByteIndex];

            ucQualityBit1 = (ucQualityBit1 >> ucBitIndex) & 0x01;

            if((VVT_ANALOG_AO == emVoteVarType) || (VVT_ANALOG_AI == emVoteVarType))
            {
                ucBuffIndex = sizeof(uint16_t)*usCount;
                
                memcpy(&usValue1, &ucData1[ucBuffIndex], sizeof(uint16_t));

                /*获取最终表决值*/
                if(true == ucQualityBit1)
                {
                    usVoteValue = usValue1;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&usVoteValue, &ucOldData[ucBuffIndex], sizeof(uint16_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        usVoteValue = (uint16_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &usVoteValue, sizeof(uint16_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if((VVT_DIGITAL_DI == emVoteVarType) || (VVT_DIGITAL_DO == emVoteVarType))
            {
                ucBuffIndex = (usCount/8);
                ucBitIndex = (usCount%8);

                memcpy(&bValue1, &ucData1[ucBuffIndex], sizeof(bool_t));
//                printf("%d;",bValue1);
                bValue1 = (bValue1 >> ucBitIndex) & 0x01;
//                printf("%d;",bValue1);
                
                /*获取最终表决值*/
                if(true == ucQualityBit1)
                {
                    bVoteValue = bValue1;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&bVoteValue, &ucOldData[ucBuffIndex], sizeof(bool_t));
                        bVoteValue = (bVoteValue >> ucBitIndex) & 0x01;
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        bVoteValue = (bool_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                /*强表决值填入BUFF*/
                ucDestBuff[ucBuffIndex] = ucDestBuff[ucBuffIndex] | (bVoteValue << ucBitIndex);
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if(VVT_FLOAT == emVoteVarType)
            {
                ucBuffIndex = sizeof(float_t)*usCount;

                memcpy(&fValue1, &ucData1[ucBuffIndex], sizeof(float_t));
                
                /*获取最终表决值*/
                if(true == ucQualityBit1)
                {
                    fVoteValue = fValue1;
                    ucQualityBit = true;
                }
                else
                {
                    ucQualityBit = false;
                    if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                    {
                        memcpy(&fVoteValue, &ucOldData[ucBuffIndex], sizeof(float_t));
                        ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                        DataExChangeAddTolerate(ucModID, usCount);
                    }
                    else
                    {
                        fVoteValue = (float_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                    }
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &fVoteValue, sizeof(float_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
            }
        }
        bRet = true;
    }
    
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeTolerateVote
*
* 功能描述: 用于模块通道级表决，当没有任何一系数据时的表决（决定数据保持还是采用安全值）
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
static bool_t DataExChangeTolerateVote(Task_ID_t emTaskID,uint8_t ucModID , uint8_t ucOldData[],uint8_t ucDestBuff[], uint8_t ucDiagBuff[])
{
    bool_t bRet = false;
    uint16_t usModType = 0U;
    VoteVarType_t emVoteVarType = VVT_INVALID_ID;
    VoteModInfo_t* pstVoteModInfo = NULL;
    bool_t bVoteValue = false;
    uint16_t usVoteValue = 0;
    float_t fVoteValue = 0.0;
    uint16_t usChannelNum = 0U;
    uint16_t usCount = 0U;
    uint8_t ucQualityBit = true;
    uint8_t ucBuffIndex = 0U;                               /*用于从通道数据中取值*/
    uint8_t ucBitIndex = 0U;                                /*用于从通道质量中取值，若为bool量，也用于从通道数据中取值*/
    
    usModType = SharedGetModType(emTaskID,ucModID);
    pstVoteModInfo = SharedGetVoteInfo(ucModID);
    
    switch(usModType)
    {
        case MODTAG_AI:
        {
            emVoteVarType = VVT_ANALOG_AI;
        }
        break;
        
        case MODTAG_AO:
        {
            emVoteVarType = VVT_ANALOG_AO;
        }
        break;
        
        case MODTAG_DI:
        {
            emVoteVarType = VVT_DIGITAL_DI;
        }
        break;

        case MODTAG_DO:
        {
            emVoteVarType = VVT_DIGITAL_DO;
        }
        break;

        case MODTAG_OSP:
        {
            emVoteVarType = VVT_FLOAT;
        }
        break;
        
        default:
        {
            emVoteVarType = VVT_INVALID_ID;
        }
        break;
    }
    if((pstVoteModInfo != NULL) && (emVoteVarType < VVT_INVALID_ID))
    {
        usChannelNum = pstVoteModInfo->usChannelCount;
        
        for(usCount = 0U;usCount < usChannelNum; usCount++)
        {
            ucBitIndex = (usCount%8);
            
            if((VVT_ANALOG_AO == emVoteVarType) || (VVT_ANALOG_AI == emVoteVarType))
            {
                ucBuffIndex = sizeof(uint16_t)*usCount;

                /*获取最终表决值*/
                ucQualityBit = false;
                if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                {
                    memcpy(&usVoteValue, &ucOldData[ucBuffIndex], sizeof(uint16_t));
                    ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                else
                {
                    usVoteValue = (uint16_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &usVoteValue, sizeof(uint16_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if((VVT_DIGITAL_DO == emVoteVarType) || (VVT_DIGITAL_DI == emVoteVarType))
            {
                ucBuffIndex = (usCount/8);
                
                /*获取最终表决值*/
                ucQualityBit = false;
                if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                {
                    memcpy(&bVoteValue, &ucOldData[ucBuffIndex], sizeof(bool_t));
                    bVoteValue = (bVoteValue >> ucBitIndex) & 0x01;
                    ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                else
                {
                    bVoteValue = (bool_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                /*强表决值填入BUFF*/
                ucDestBuff[ucBuffIndex] = ucDestBuff[ucBuffIndex] | (bVoteValue << ucBitIndex);
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
                
            }
            
            if(VVT_FLOAT == emVoteVarType)
            {
                ucBuffIndex = sizeof(float_t)*usCount;
                
                /*获取最终表决值*/
                ucQualityBit = false;
                if(TOLERANCE_ALLOWED == DataExChangeCheckTolerate(ucModID, usCount))
                {
                    memcpy(&fVoteValue, &ucOldData[ucBuffIndex], sizeof(float_t));
                    ucQualityBit  = ucQualityBit | TOLERANCE_ALLOWED;
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                else
                {
                    fVoteValue = (float_t)pstVoteModInfo->stVoteChannelInfo[usCount].uiSafetyValue;
                }
                
                if((ucQualityBit & TOLERANCE_ALLOWED) != TOLERANCE_ALLOWED)
                {
                    DataExChangeAddTolerate(ucModID, usCount);
                }
                
                memcpy(&ucDestBuff[ucBuffIndex], &fVoteValue, sizeof(float_t));
                memcpy(&ucDiagBuff[usCount], &ucQualityBit, sizeof(uint8_t));
            }
        }
        bRet = true;
    }
    
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeBoolTriVote
*
* 功能描述: 用于bool量三系表决（2OO3）
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
static bool_t DataExChangeBoolTriVote(bool_t bValue1, bool_t bValue2, bool_t bValue3)
{
    bool_t bRet = false;
    
    if(bValue1 == bValue2)
    {
        bRet = bValue1;
    }
    else if(bValue1 == bValue3)
    {
        bRet = bValue1;
    }
    else
    {
        bRet = bValue2;
    }
    
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeBoolDoulVote
*
* 功能描述: 用于bool量两系表决（1OO2，或2OO2）
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
static bool_t DataExChangeBoolDoulVote(bool_t bValue1, bool_t bValue2, VoteType_t emVoteType)
{
    bool_t bRet = false;
    
    switch(emVoteType)
    {
        case VT_1OO2:
        {
            if((true == bValue1) && (true == bValue2))
            {
                bRet = true;
            }
        }
        break;
        
        case VT_2OO2:
        {
            if((true == bValue1) || (true == bValue2))
            {
                bRet = true;
            }
        }
        break;
        
        default:
            
        break;
    }
    
    return bRet;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeAddTolerate
*
* 功能描述: 用于统计通道容忍次数
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
static void DataExChangeAddTolerate(uint8_t ucModAddr, uint8_t ucChannelID)
{
    if((ucModAddr < MAX_MOD_NUM) && (ucChannelID < MAX_CHANNEL_NUM))
    {
        s_ucChannelTolerate[ucModAddr][ucChannelID]++;
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeAddTolerate
*
* 功能描述: 用于清空通道容忍次数
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
static void DataExChangeClearTolerate(uint8_t ucModAddr, uint8_t ucChannelID)
{
    if((ucModAddr < MAX_MOD_NUM) && (ucChannelID < MAX_CHANNEL_NUM))
    {
        s_ucChannelTolerate[ucModAddr][ucChannelID] = 0U;
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DataExChangeCheckTolerate
*
* 功能描述: 用于判断通道是否容忍
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
static uint8_t DataExChangeCheckTolerate(uint8_t ucModAddr, uint8_t ucChannelID)
{
    uint8_t ucChannelTolerate = SharedGetTolerate();
    uint8_t ucRet = TOLERANCE_NOT_ALLOWED;
    if((ucModAddr < MAX_MOD_NUM) && (ucChannelID < MAX_CHANNEL_NUM))
    {
        if(s_ucChannelTolerate[ucModAddr][ucChannelID] >= ucChannelTolerate)
        {
            ucRet = TOLERANCE_NOT_ALLOWED;
        }
        else
        {
            ucRet = TOLERANCE_ALLOWED;
        }
    }
    
    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: DataExChangeModForceVar
*
* 功能描述: 用于完成模块的通道强制
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
static void DataExChangeModForceVar(Task_ID_t emTaskID, uint8_t ucModID , uint8_t ucOldData[],uint8_t ucDestBuff[])
{
    uint16_t usModType = 0U;
    SysIoModInfo_t* pstUPIoModInfo = NULL;
    puint8_t pucValue = NULL;
    puint8_t pucOldValue = NULL;
    puint8_t pucDisFlg = NULL;
    uint8_t ucBoolTmp = 0x01;
    puint16_t pusValue = NULL;
    puint16_t pusOldValue = NULL;
    puint16_t pusDisFlg = NULL;
    pfloat_t pfValue = NULL;
    pfloat_t pfOldValue = NULL;
    puint32_t puiDisFlg = NULL;
    uint16_t usChannelNum = 0U;
    uint16_t usCount = 0U;
    uint32_t uiOffset = 0U;         /* 通道变量相对G区偏移 */
    uint8_t ucByteIndex = 0U;       /* 用于从通道质量中取值 */
    uint8_t ucBitIndex = 0U;        /* 用于从通道质量中取值，若为bool量，也用于从通道数据中取值 */
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    
    usModType = SharedGetModType(emTaskID,ucModID);
    pstUPIoModInfo = SharedGetIoModInfo(emTaskID); /* 获取对应IO表 */
    
    if(pstUPIoModInfo != NULL)
    {
        usChannelNum = pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].usChannelNum;
        for(usCount = 0; usCount < usChannelNum; usCount++)
        {
            switch(usModType)
            {
                case MODTAG_AI:
                case MODTAG_AO:
                {
                    ucByteIndex = usCount * sizeof(uint16_t);
                    pusValue = (puint16_t)&ucDestBuff[ucByteIndex];
                    pusOldValue = (puint16_t)&ucOldData[ucByteIndex];
                    
                    uiOffset = 0;
                    if(0x01 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + \
                                pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset;
                    }
                    if(0x02 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + \
                                pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset;
                    }
                    if((uiOffset > 0) && (uiOffset < MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + MAX_RTDATA_Q_AREA_LEN))
                    {
                        pusDisFlg = (puint16_t)(pucFlagStartAddr + uiOffset);

                        if(*pusDisFlg != 0)
                        {
                            *pusValue = *pusOldValue;
                        }
                    }
                }
                break;
                
                case MODTAG_DI:
                case MODTAG_DO:
                {
                    ucByteIndex = (usCount/8);
                    ucBitIndex = (usCount%8);
                    pucValue = (puint8_t)&ucDestBuff[ucByteIndex];
                    pucOldValue = (puint8_t)&ucOldData[ucByteIndex];
                    
                    uiOffset = 0;
                    if(0x01 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + \
                                (pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset / 8);
                    }
                    if(0x02 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + \
                                (pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset / 8);
                    }
                    if((uiOffset > 0) && (uiOffset < MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + MAX_RTDATA_Q_AREA_LEN))
                    {
                        pucDisFlg = (puint8_t)(pucFlagStartAddr + uiOffset);

                        if((((*pucDisFlg)>>ucBitIndex)&0x01) != 0)
                        {
                            if(true == (((*pucOldValue)>>ucBitIndex)&0x01))
                            {
                                *pucValue |= (ucBoolTmp << ucBitIndex);
                            }
                            else
                            {
                                *pucValue &= (~(ucBoolTmp << ucBitIndex));
                            }
                        }
                    }
                }
                break;

                case MODTAG_OSP:
                {
                    ucByteIndex = usCount * sizeof(float_t);
                    pfValue = (pfloat_t)&ucDestBuff[ucByteIndex];
                    pfOldValue = (pfloat_t)&ucOldData[ucByteIndex];
                    
                    uiOffset = 0;
                    if(0x01 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + \
                                pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset;
                    }
                    if(0x02 == pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].ucRefId)
                    {
                        uiOffset = MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + \
                                pstUPIoModInfo->stSysIoSlaveInfo.stDevInfo[ucModID].stSysIoChannelInfo[usCount].uiBitOffset;
                    }
                    if((uiOffset > 0) && (uiOffset < MAX_RTDATA_G_AREA_LEN + MAX_RTDATA_I_AREA_LEN + MAX_RTDATA_Q_AREA_LEN))
                    {
                        puiDisFlg = (puint32_t)(pucFlagStartAddr + uiOffset);

                        if(*puiDisFlg != 0)
                        {
                            *pfValue = *pfOldValue;
                        }
                    }
                }
                break;
                
                default:
                {
                    printf("ModType err\n");
                }
                break;
            }
        }
    }
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

