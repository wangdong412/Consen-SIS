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
*  文件名称    : InterComm.c
*
*  功能简介    : 负责PM与CM间的数据通讯
*
*  作者       : 李琦
*
*  创建日期    : 2016-01-13
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
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <error.h>
#include <dirent.h>
#include <string.h>
#include <byteswap.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <unistd.h>
#include <sys/resource.h>
#include "../inc/InterComm.h"
#include "../inc/InterCommRTData.h"
#include "Sys/inc/Sys.h"
#include "CommonLib/inc/Algrithm.h"
#include "Sys/inc/CMLocalbus.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
//static int32_t s_fdfpga;
static RecvBuffer_t s_stRecvBuffer1;                            /* 接收数据缓冲区 */
static RecvBuffer_t s_stRecvBuffer2;                            /* 接收数据缓冲区 */
static SendBuffer_t s_stSendBuffer;                             /* 发送数据缓冲区 */
static CommHandling_t s_stCommRecv;                             /* 记录从接收缓冲区解析数据时的动态操作 */
static CommHandling_t s_stCommSend;                             /* 记录向发送缓冲区写入打包数据时的动态操作 */
static uint16_t s_usSendPackageSerialNum[NUM_OF_PM];            /* 发送数据包序列号 */
static uint16_t s_usRecvPackageSerialNum[NUM_OF_PM];            /* 接收数据包序列号 */
static uint16_t s_usInterCommState[NUM_OF_PM][NUM_OF_SLOT];     /* 内部通讯状态 */
static uint32_t s_uiCfgFileReq[NUM_OF_PM];                      /* 发送给指定PM的跟配置文件相关的控制命令 */
static uint16_t s_usLastFileIndex[NUM_OF_CM];
static uint32_t s_uiInterCommNum[NUM_OF_PM];                    /* 记录未收到通讯数据的次数 */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 接收数据 */
static bool_t InterCommReceive(PMController_t emPMID);
/* 接收数据处理 */
static void InterCommReceiveHandling(RecvBuffer_t *pstRecvBuf, PMController_t emPMID);
/* 接收数据预处理 */
static bool_t InterCommReceivePreHandling(RecvBuffer_t *pstRecvBuf, PMController_t emPMID);
/* 解析系统状态信息 */
static void InterCommParseSysState(RecvBuffer_t *pstRecvBuf, PMController_t emPMID);
/* 解析探测数据 */
static bool_t InterCommParseDetectMsg(RecvBuffer_t *pstRecvBuf);
/* 解析序列号 */
//static bool_t InterCommParsePackageSerialNum(RecvBuffer_t *pstRecvBuf, uint16_t usSerialNum);
/* 解析PC端软件应答 */
static void InterCommParsePCsoftware(RecvBuffer_t *pstRecvBuf, PMController_t emPMID, SysMsgType_t emMsgType);
/* 解析P2P*/
static void InterCommParseP2P(RecvBuffer_t *pstRecvBuf, PMController_t emPMID, SysP2PMsgType_t emMsgType);
/* 解析CM配置信息 */
static void InterCommParseCfgFileInfo(RecvBuffer_t *pstRecvBuf, PMController_t emPMID);
/* 解析配置表 */
//static void InterCommParseCfgFileTable(RecvBuffer_t *pstRecvBuf);
/* 找到最后一个配置文件的索引号 */
//static uint16_t FindLastCfgFileIndex(CMController_t emCMID);
/* 解析实时数据 */
static void InterCommParseRTData(RecvBuffer_t *pstRecvBuf, PMController_t emPMID);

/* 发送数据 */
static void InterCommSend(PMController_t emPMID);
/* 发送数据处理 */
static void InterCommSendHandling(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包CM的状态信息 */
static void InterCommPackSysCMState(SendBuffer_t *pstSendBuf);
/* 打包对来自PM的探测消息的应答 */
static void InterCommPackDetectMsgAck(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包来自CS1131的请求 */
static void InterCommPackCS1131Req(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包SOE请求  */
static void InterCommPackSOEReq(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包OPC请求 */
static void InterCommPackOPCReq(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包客户端请求 */
static void InterCommPackClientReq(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包客户端应答 */
static void InterCommPackClientResp(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包AMS请求  */
static void InterCommPackAMSReq(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包PTP请求 */
static void InterCommPackPTP(SendBuffer_t *pstSendBuf, PMController_t emPMID, SysP2PMsgType_t emMsgType);
/* 打包Modbus请求 /应答*/
static void InterCommPackModbus(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 打包获取CM配置信息的请求命令 */
//static bool_t InterCommPackConfigReq(SendBuffer_t *pstSendBuf, PMController_t emPMID, uint32_t uiReqCmd);
/* 打包对实时数据的应答*/
static void InterCommPackRTDataResp(SendBuffer_t *pstSendBuf, PMController_t emPMID);
/* 设置内部通讯状态 */
static bool_t InterCommSetLocalState(PMController_t emPMID, uint16_t usInterState);
/* 获取内部通讯状态 */
static bool_t InterCommGetLocalState(PMController_t emPMID, puint16_t pusInterState);
/* 打包内部命令请求  */
static void InterCommPackInterCmd(SendBuffer_t *pstSendBuf, PMController_t emPMID);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: InternalCommInit
*
* 功能描述: 初始化函数
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
void InternalCommInit(void)
{
    /* 静态局部变量初始化 */
    memset((puint8_t)&s_stRecvBuffer1, 0, sizeof(RecvBuffer_t));
    memset((puint8_t)&s_stRecvBuffer2, 0, sizeof(RecvBuffer_t));
    memset((puint8_t)&s_stSendBuffer, 0, sizeof(SendBuffer_t));
    memset((puint8_t)&s_stCommRecv, 0, sizeof(CommHandling_t));
    memset((puint8_t)&s_stCommSend, 0, sizeof(CommHandling_t));
    memset((puint8_t)&s_usSendPackageSerialNum[0], 0, sizeof(uint16_t)*NUM_OF_PM);
    memset((puint8_t)&s_usRecvPackageSerialNum[0], 0, sizeof(uint16_t)*NUM_OF_PM);
    memset((puint16_t)&s_usInterCommState[0], 0, sizeof(uint16_t)*NUM_OF_PM*NUM_OF_SLOT);
    //memset((puint8_t)&s_uiCfgFileReq[0], CONFIG_INFO_NO_REQ, sizeof(uint32_t)*NUM_OF_PM);
    memset((puint16_t)&s_usLastFileIndex[0], 0, sizeof(uint16_t)*NUM_OF_CM);
    memset((puint8_t)&s_uiInterCommNum[0], 0, sizeof(uint32_t)*NUM_OF_PM);

    s_uiCfgFileReq[PMA] = CONFIG_INFO_ALL_CFG_START;
    s_uiCfgFileReq[PMB] = CONFIG_INFO_ALL_CFG_START;
    s_uiCfgFileReq[PMC] = CONFIG_INFO_ALL_CFG_START;
    CMLocalbusInit();

    InterCommRTDataInit();
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InternalCommCycle
*
* 功能描述: 周期运行函数
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
void InternalCommCycle(void)
{
    uint8_t i = INVALID_PM_ID;
    uint16_t usInterState = 0;
    bool_t bRet = false;

    //printf("-----------------------------------\n");

    /* 与三系PM分别进行数据通讯 */
    for(i = PMA; i < NUM_OF_PM; i++)
    {
        /* CM接收来自PM的数据 */
        InterCommReceive(i);

        bRet = InterCommGetLocalState(i, &usInterState);
        if((true == bRet) && (INTER_COMM_NORMAL == usInterState))
        {
            /* CM向PM发送数据 */
            InterCommSend(i);
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
* 函数名称: InterCommReceive
*
* 功能描述: 从FPGA相应的接收缓冲区读取数据
*
* 输入参数: emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommReceive(PMController_t emPMID)
{
    bool_t bRet = false;
    
    uint32_t uiState = 0;
    uint32_t uiTime1 = 0;
    uint32_t uiTime2 = 0;
    bool_t bRecvFlag1 = false;
    bool_t bRecvFlag2 = false;

    uiState = GetCMRecvState(emPMID);

    if((CMBUS_RECV_BUF1_HAS_DATA == (uiState & CMBUS_RECV_BUF1_HAS_DATA_MASK))
        && (CMBUS_RECV_BUF1_DATA_CORRECT == (uiState & CMBUS_RECV_BUF1_DATA_CORRECT_MASK)))
    {
        CMReceive((uint32_t)emPMID, (uint8_t)RECV_BUFF_NO1, (puint8_t)&s_stRecvBuffer1);
        uiTime1 = GetCMRecvTime(emPMID, RECV_BUFF_NO1);
        bRecvFlag1 = true;
    }
    else if((CMBUS_RECV_BUF1_HAS_DATA == (uiState & CMBUS_RECV_BUF1_HAS_DATA_MASK))
            && (CMBUS_RECV_BUF1_DATA_CORRECT != (uiState & CMBUS_RECV_BUF1_DATA_CORRECT_MASK)))
    {
        ClearCMRecvState(emPMID, 0x0003);
        printf("Incorrect data 1\n");
    }

    if((CMBUS_RECV_BUF2_HAS_DATA == (uiState & CMBUS_RECV_BUF2_HAS_DATA_MASK))
        && (CMBUS_RECV_BUF2_DATA_CORRECT == (uiState & CMBUS_RECV_BUF2_DATA_CORRECT_MASK)))
    {
        CMReceive((uint32_t)emPMID, (uint8_t)RECV_BUFF_NO2, (puint8_t)&s_stRecvBuffer2);
        uiTime2 = GetCMRecvTime(emPMID, RECV_BUFF_NO2);
        bRecvFlag2 = true;
    }
    else if((CMBUS_RECV_BUF2_HAS_DATA == (uiState & CMBUS_RECV_BUF2_HAS_DATA_MASK))
            && (CMBUS_RECV_BUF2_DATA_CORRECT != (uiState & CMBUS_RECV_BUF2_DATA_CORRECT_MASK)))
    {
        ClearCMRecvState(emPMID, 0x0300);
        printf("Incorrect data 2\n");
    }


    if((true == bRecvFlag1) && (false == bRecvFlag2))
    {
        ClearCMRecvState(emPMID, 0x0003);
        //printf("recv1:\n");
        /* CM接收来自PM的数据 */
        InterCommReceiveHandling (&s_stRecvBuffer1, emPMID);

        if(InterCommRTDataReceive(emPMID))
        {
            InterCommRTDataSend(emPMID);
        }
        bRet = true;
    }
    else if ((false == bRecvFlag1) && (true == bRecvFlag2))
    {
        ClearCMRecvState(emPMID, 0x0300);
        //printf("recv2:\n");
        /* CM接收来自PM的数据 */
        InterCommReceiveHandling (&s_stRecvBuffer2, emPMID);

        if(InterCommRTDataReceive(emPMID))
        {
            InterCommRTDataSend(emPMID);
        }
        bRet = true;
    }
    else if ((true == bRecvFlag1) && (true == bRecvFlag2))
    {
        ClearCMRecvState(emPMID, 0x0003);
        ClearCMRecvState(emPMID, 0x0300);

        if(uiTime1 > uiTime2)
        {
            //printf("recv12:\n");
            InterCommReceiveHandling (&s_stRecvBuffer1, emPMID);
            if(InterCommRTDataReceive(emPMID))
            {
                //InterCommRTDataSend(emPMID);
            }
            InterCommReceiveHandling (&s_stRecvBuffer2, emPMID);
            if(InterCommRTDataReceive(emPMID))
            {
                InterCommRTDataSend(emPMID);
            }
        }
        else
        {
            //printf("recv21:\n");
            InterCommReceiveHandling (&s_stRecvBuffer2, emPMID);
            if(InterCommRTDataReceive(emPMID))
            {
                //InterCommRTDataSend(emPMID);
            }
            InterCommReceiveHandling (&s_stRecvBuffer1, emPMID);
            if(InterCommRTDataReceive(emPMID))
            {
                InterCommRTDataSend(emPMID);
            }
        }

        bRet = true;
    }
    else
    {
        //no data received
        bRet = false;

        /* 记录未收到数据的次数 */
        s_uiInterCommNum[emPMID]++;

        if(s_uiInterCommNum[emPMID] >= MAX_NO_RECV_DATA_CYCLE_NUM)
        {
            InterCommSetLocalState(emPMID, INTER_COMM_ABNORMAL);
            s_uiInterCommNum[emPMID] = MAX_NO_RECV_DATA_CYCLE_NUM;
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommReceiveHandling
*
* 功能描述: 接收处理来自某系PM的数据
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数:
*
* 返 回 值  : 无
*
* 注意事项: 格式如下
* ___________________________________________________
*    | CM slot                                     |
*  H |---------------------------------------------|
*  E | Package serial num                          |
*  A |---------------------------------------------|
*  D | Data Len                                    |
*  E |---------------------------------------------|
*  R | Msg1 block length                           |
*    |---------------------------------------------|
*    | Msg1 block offset                           |
*    |---------------------------------------------|
*    | Msg2 block length                           |
*    |---------------------------------------------|
*    | Msg2 block offset                           |
*    |---------------------------------------------|
*    | Msg3 block length                           |
*    |---------------------------------------------|
*    | Msg3 block offset                           |
*    |---------------------------------------------|
*    | ...                                         |
*    |---------------------------------------------|
*    | CRC32                                       |
*____|_____________________________________________|
*    | usStartFlag | usMsgLen | Msg1...    | CRC32 |
*  B |---------------------------------------------|
*  O | usStartFlag | usMsgLen | Msg2...    | CRC32 |
*  D |---------------------------------------------|
*  Y | usStartFlag | usMsgLen | Msg3...    | CRC32 |
*    |---------------------------------------------|
*    |   ...                                       |
*____|_____________________________________________|
***********************************************************************************************************************
*/
static void InterCommReceiveHandling(RecvBuffer_t *pstRecvBuf, PMController_t emPMID)
{
    bool_t bResult = false;
//    int i = 0;//test
//    uint8_t *pBuf = NULL;
    
    /* 参数有效性检查 */
    if((pstRecvBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        bResult = InterCommReceivePreHandling(pstRecvBuf, emPMID);
    }

    /* 参数有效且数据预处理成功 */
    if(true == bResult)
    {
        /* 解析系统状态信息 */
        InterCommParseSysState(pstRecvBuf, emPMID);

        /* 解析探测消息 */
        bResult = InterCommParseDetectMsg(pstRecvBuf);

        /* 收到探测消息 */
        if(true == bResult)
        {
            /* 获取本CM的ID，并设置内部通讯状态 */
            InterCommSetLocalState(emPMID, INTER_COMM_NORMAL);  /* Changed by wangdong: INTER_COMM_DETECT_MSG */

            /* 解析SOE、CS1131、OPC、Client、AMS 数据*/
            InterCommParsePCsoftware(pstRecvBuf, emPMID, SOE_RESP);
            InterCommParsePCsoftware(pstRecvBuf, emPMID, CS1131_RESP);
            InterCommParsePCsoftware(pstRecvBuf, emPMID, OPC_RESP);
            //InterCommParsePCsoftware(pstRecvBuf, emPMID, CLIENT_REQ);
            InterCommParsePCsoftware(pstRecvBuf, emPMID, CLIENT_RESP);
            InterCommParsePCsoftware(pstRecvBuf, emPMID, AMS_RESP);

            /* 解析来自PM的P2P请求或应答数据 */
            InterCommParseP2P(pstRecvBuf, emPMID, P2P_PM_REQ);
            InterCommParseP2P(pstRecvBuf, emPMID, P2P_PM_RESP);

            /* 解析实时数据 */
            InterCommParseRTData(pstRecvBuf, emPMID);

            /* 解析配置信息 */
            InterCommParseCfgFileInfo(pstRecvBuf, emPMID);

            s_uiInterCommNum[emPMID] = 0;
        }
        else /* 未收到探测消息 */
        {
            s_uiInterCommNum[emPMID] ++;
            if(s_uiInterCommNum[emPMID] >= MAX_NO_RECV_DATA_CYCLE_NUM)
            {
                InterCommSetLocalState(emPMID, INTER_COMM_ABNORMAL);
                s_uiInterCommNum[emPMID] = MAX_NO_RECV_DATA_CYCLE_NUM;
            }
        }
    }
    else
    {
        /* 长时间未收到数据或者数据出错 */
        s_uiInterCommNum[emPMID] ++;
        if(s_uiInterCommNum[emPMID] >= MAX_NO_RECV_DATA_CYCLE_NUM)
        {
            InterCommSetLocalState(emPMID, INTER_COMM_ABNORMAL);
            s_uiInterCommNum[emPMID] = MAX_NO_RECV_DATA_CYCLE_NUM;
        }
    }
    
#if 0
    printf("Recv-----\n");
    pBuf = (puint8_t)pstRecvBuf;
    for(i=0;i<sizeof(RecvHeader_t);i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
    //printf("sizeof(RecvHeader_t) = %d\n", sizeof(RecvHeader_t));
    
    pBuf = (puint8_t)pstRecvBuf;
    for(i=sizeof(RecvHeader_t);i<sizeof(RecvHeader_t)+pstRecvBuf->stRecvHeader.usDataLen;i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
#endif
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommParseDetectMsg
*
* 功能描述: 解析来自PM的探测消息
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*
* 输出参数:
*
* 返 回 值  : true: 收到探测数据包
*        false: 未收到探测数据包
*
*              -------------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | Detect msg  ... | CRC32 |
*              -------------------------------------------------
***********************************************************************************************************************
*/
static bool_t InterCommParseDetectMsg(RecvBuffer_t *pstRecvBuf)
{
    DetectMsgBlock_t* pstDetectMsgBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bRet = false;

    /* 参数有效性判断*/
    if(pstRecvBuf != NULL)
    {
        /* 探测包的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stDetectMsgBlock.usLen;
        /* 探测包在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stDetectMsgBlock.usOffset;

        /* 取出探测包数据块的内容 */
        if((usBlockLen == sizeof(DetectMsgBlock_t)) && (usOffset +usBlockLen < CM_RECV_BUFF_BODY_SIZE))
        {
            pstDetectMsgBlock = (DetectMsgBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstDetectMsgBlock->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstDetectMsgBlock->uiCrc32;
                pucBuf = (puint8_t) &(pstDetectMsgBlock->stDetectMsg);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstDetectMsgBlock->usLen);
                if(uiCRC32 == uiCRC32calc)
                {
                    /* 判断探测数据内容 */
                    if((pstDetectMsgBlock->stDetectMsg.uiFlag & 0x0f) == DETECT_MSG_FLAG)
                    {
                        bRet = true;
                    }
                    else
                    {
                        bRet = false;
                    }
                }
            }
        }
    }

    return bRet;
}
#if 0
/*
***********************************************************************************************************************
* 函数名称: InterCommParsePackageSerialNum
*
* 功能描述: 解析数据包序列号
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        usSerialNum: 预期序列号
*
* 输出参数:
*
* 返 回 值  : true: 序列号正确
*        false: 序列号错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommParsePackageSerialNum(RecvBuffer_t *pstRecvBuf, uint16_t usSerialNum)
{
    bool_t bRet = false;

    /* 参数有效性判断*/
    if(pstRecvBuf != NULL)
    {
        /* 判断接收到的序列号是否与预期序列号一致 */
        if(usSerialNum == pstRecvBuf->stRecvHeader.usPackageSerialNum)
        {
            bRet = true;
        }
    }

    return bRet;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: InterCommParseSysState
*
* 功能描述: 解析系统状态
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ------------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | Sys state  ... | CRC32 |
*              ------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParseSysState(RecvBuffer_t *pstRecvBuf, PMController_t emPMID)
{
    SysStateBlock_t* pstSysStateBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;

    if( pstRecvBuf != NULL )
    {
        /* 系统状态信息的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stSysStateBlock.usLen;
        /* 系统状态信息在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stSysStateBlock.usOffset;

        /* 取出系统状态信息数据块 */
        if((usBlockLen == sizeof(SysStateBlock_t)) && ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
        {
            pstSysStateBlock = (SysStateBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstSysStateBlock->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstSysStateBlock->uiCrc32;
                pucBuf = (puint8_t) &(pstSysStateBlock->stSysState);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstSysStateBlock->usLen);
                if(uiCRC32 == uiCRC32calc)
                {
                    /* 将系统状态信息记录到系统资源管理模块中 */
                    SysSetSysState(&(pstSysStateBlock->stSysState), emPMID);
                    SysSetPrjInfo(emPMID,&(pstSysStateBlock->stSysState.stPrjInfo));
                    SysSetDownloadPrjInfo(emPMID,&(pstSysStateBlock->stSysState.stDownloadPrjInfo));
                    SysSetPMTaskNum(emPMID, pstSysStateBlock->stSysState.stTaskInfo.ucTaskNum);
                    SysSetPMTaskInfo(emPMID, pstSysStateBlock->stSysState.stTaskInfo.ucTask1State, \
                            pstSysStateBlock->stSysState.stTaskInfo.ucTask2State);
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommParsePCsoftware
*
* 功能描述: 解析来自PM对上位机软件或外部设备的请求/应答
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emPMID: PM的ID
*         emMsgType: 消息类型
*
* 输出参数:
*
* 返 回 值  : 无
*
*              --------------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | PC software  ... | CRC32 |
*              --------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParsePCsoftware(RecvBuffer_t *pstRecvBuf, PMController_t emPMID, SysMsgType_t emMsgType)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bMsgTypeOK = false;

    if(CS1131_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stCS1131Resp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stCS1131Resp.usOffset;
        bMsgTypeOK = true;
    }
    else if(SOE_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stSOEResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stSOEResp.usOffset;
        bMsgTypeOK = true;
    }
    else if(OPC_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stOPCResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stOPCResp.usOffset;
        bMsgTypeOK = true;
    }
    else if(CLIENT_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stClientResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stClientResp.usOffset;
        bMsgTypeOK = true;
    }
    else if(CLIENT_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stClientReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stClientReq.usOffset;
        bMsgTypeOK = true;
    }
    else if(AMS_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stAMSResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stAMSResp.usOffset;
        bMsgTypeOK = true;
        if(usBlockLen > 0)
        {
            printf("@@AMS %d %d\n",usBlockLen,usOffset);
        }
        
    }
    else
    {
        bMsgTypeOK = false;
    }

    if((true == bMsgTypeOK) && (usBlockLen > 0) && (pstRecvBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

        usStartFlag = *((puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET]);
        usMsgLen = *((puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET]);
        uiCRC32 = *((puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET+usMsgLen]);

        /* 取出应答的内容 */
        if((usBlockLen == (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
        {
            /* 判断数据块起始标识 */
            if (usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                pucBuf = (puint8_t) &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);
                if(uiCRC32 == uiCRC32calc)
                {
                    SysSetMsg (emPMID, emMsgType, pucBuf, usMsgLen);
                    printf("AMS SetMsg  %d %d %d\n",emPMID,emMsgType, usMsgLen);
                }
                else
                {
                    printf("AMS CRC %x %x\n",uiCRC32,uiCRC32calc);
                }
            }
        }
        else
        {
            printf("AMS %d %d %d\n",usBlockLen,usOffset,usMsgLen);
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommParseP2P
*
* 功能描述: 解析来自PM的P2P数据
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emPMID: PM的ID
*         emMsgType: 消息类型
*
* 输出参数:
*
* 返 回 值  : 无
*
*              --------------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | P2P Req/Resp ... | CRC32 |
*              --------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParseP2P(RecvBuffer_t *pstRecvBuf, PMController_t emPMID, SysP2PMsgType_t emMsgType)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bMsgTypeOK = false;

    if(P2P_PM_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stP2PReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stP2PReq.usOffset;
        bMsgTypeOK = true;
    }
    else if(P2P_PM_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stP2PResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stP2PResp.usOffset;
        bMsgTypeOK = true;
    }
    else
    {
        bMsgTypeOK = false;
    }

    if((true == bMsgTypeOK) && (usBlockLen > 0) && (pstRecvBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

        usStartFlag = *((puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET]);
        usMsgLen = *((puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET]);
        uiCRC32 = *((puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET+usMsgLen]);

        /* 取出应答的内容 */
        if((usBlockLen == (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
        {
            /* 判断数据块起始标识 */
            if (usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                pucBuf = (puint8_t) &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);
                if(uiCRC32 == uiCRC32calc)
                {
                    SysSetP2PMsg (emPMID, emMsgType, pucBuf, usMsgLen);
                }
            }
        }
    }
    return;
}
#if 0
/*
***********************************************************************************************************************
* 函数名称: FindLastCfgFileIndex
*
* 功能描述: 获取最后一个配置文件的索引号
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数:
*
* 返 回 值  : 无
*
***********************************************************************************************************************
*/
static uint16_t FindLastCfgFileIndex(CMController_t emCMID)
{
    uint16_t usLastFileIndex = 0;
    SysCfgFileTable_t stCfgFileTable;

    /* 获取配置表 */
    SysGetCfgFileTable(&stCfgFileTable);

    /* CM配置文件需要更新 */
    if(CFG_FILE_UPDATED == stCfgFileTable.bCfgFileCM[emCMID])
    {
        usLastFileIndex = (uint16_t)CFG_CM;
    }

    /* Modbus master 配置文件需要更新 */
    if(CFG_FILE_UPDATED == stCfgFileTable.bCfgFileModbusMaster[emCMID])
    {
        usLastFileIndex = (uint16_t)CFG_MODBUS_MASTER;
    }

    /* Modbus slave 配置文件需要更新 */
    if(CFG_FILE_UPDATED == stCfgFileTable.bCfgFileModbusSlave[emCMID])
    {
        usLastFileIndex = (uint16_t)CFG_MODBUS_SLAVE;
    }

    /* OPC配置文件需要更新 */
    if(CFG_FILE_UPDATED == stCfgFileTable.bCfgFileOPC[emCMID])
    {
        usLastFileIndex = (uint16_t)CFG_OPC;
    }

    /* 控制站配置文件需要更新 */
    if(CFG_FILE_UPDATED == stCfgFileTable.bCfgFileCtrlStation[emCMID])
    {
        usLastFileIndex = (uint16_t)CFG_CTRL_STATION;
    }
    /* 返回最后一个需要更新的配置文件的索引号 */
    return usLastFileIndex;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: InterCommParseCfgFileInfo
*
* 功能描述: 解析来自PM的配置文件
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数:
*
* 返 回 值  : 无
*
*                   -------------------------------------------------------------------------------
* 注意事项: 接收信息格式: |usStartFlag|usCfgFileType|uiOffset|usLen|usLastPackFlag|Content array...|CRC32|
*                  --------------------------------------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParseCfgFileInfo(RecvBuffer_t *pstRecvBuf, PMController_t emPMID)
{
    CfgFileRespHeader_t* pstCfgRespHeader = NULL;           /* 指向配置信息头部 */
    uint16_t usBlockLen = 0;                                /* 配置信息长度 */
    uint16_t usOffset = 0;                                  /* 配置信息所在的偏移地址 */
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;                                 /* 指向配置信息的开始 */
    uint16_t usCfgFileType = 0;                             /* 配置文件类型 */
    uint32_t uiOffsetAddr = 0;                              /* 数据内容偏移地址 */
    uint16_t usLen = 0;                                     /* 数据长度 */
    uint16_t usLastPackFlag = CFG_FILE_NOT_LAST_PACKAGE;    /* 最后一包标志 */
    uint16_t usWrLen = 0;                                   /* 实际写入内容长度 */
    SysCfgFileState_t stSysCfgFileState;                    /* 配置文件状态 */
    //CMController_t emCMID = INVALID_CM_ID;

    if((pstRecvBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stConfigRespBlock.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stConfigRespBlock.usOffset;

        /* 取出应答的内容，存入系统资源管理模块 */
        if((usBlockLen > sizeof(CfgFileRespHeader_t))&& ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
        {
            pstCfgRespHeader = (CfgFileRespHeader_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);
            pucBuf = (puint8_t)&(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstCfgRespHeader->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                usCfgFileType = pstCfgRespHeader->usCfgFileType;
                uiOffsetAddr = pstCfgRespHeader->uiOffset;
                usLen = pstCfgRespHeader->usLen;
                usLastPackFlag = pstCfgRespHeader->usLastPackFlag;

                uiCRC32 = *((puint32_t)&pucBuf[(uint16_t)CFG_FILE_CONTENT_OFFSET + usLen]);
                uiCRC32calc = SysCrc32Cal(0, &pucBuf[CFG_FILE_START_FLAG_OFFSET], usLen + (uint16_t)CFG_FILE_HEADER_LEN);

                /* 判断CRC32是否正确 */
                if((uiCRC32 == uiCRC32calc) && (usBlockLen == sizeof(CfgFileRespHeader_t) + usLen+ DATA_BLOCK_CRC32_SIZE))
                {
                    /* 获取当前待写入的配置文件的状态 */
                    SysGetCfgFileState ((SysCfgFileType_t) usCfgFileType, &stSysCfgFileState);

                    /* 解析消息后得到的偏移地址和文件的待写入偏移地址相等 */
                    if((stSysCfgFileState.uiWrOffset == uiOffsetAddr) && (CFG_FILE_WR_ENABLE == stSysCfgFileState.bWriteable))
                    {
                        /* 将接收的内容写入系统资源管理模块 */
                        usWrLen = SysSetCfgFileInfo((SysCfgFileType_t) usCfgFileType, &pucBuf[CFG_FILE_CONTENT_OFFSET], usLen);

                        if(usWrLen < usLen)
                        {
                            /* 发生错误，没有足够的存储空间，重传 */
                            s_uiCfgFileReq[emPMID] = CONFIG_INFO_RETRANSFER;

                            /* 删除该错误文件 */
                            SysDeleteCfgFile((SysCfgFileType_t) usCfgFileType);
                        }
                        else
                        {
                            /* 收到当前文件的最后一包数据 */
                            if(CFG_FILE_LAST_PACKAGE == usLastPackFlag)
                            {
                                /* 当前配置文件传输完毕 */
                                SysSetCfgFileFinished((SysCfgFileType_t) usCfgFileType);
                                //s_uiCfgFileReq[emPMID] = CONFIG_INFO_CONTINUE;
                                printf("file finished %d\n", usCfgFileType);
                            }
                            else if(CFG_FILE_TOTAL_FINISHED == usLastPackFlag)
                            {
                                SysSetCfgFileFinished((SysCfgFileType_t) usCfgFileType);
                                s_uiCfgFileReq[emPMID] = TRANSFER_CONFIG_INFO_FINISHED;
                                SysSetRecvConfigInfoFlg(SYS_RECV_CONFIGINFO_DONE);
                                printf("all finished %d\n", usCfgFileType);
                            }
                            else
                            {
                                //s_uiCfgFileReq[emPMID] = CONFIG_INFO_CONTINUE;
                                printf("continue\n");
                            }
                        }
                    }
                    else
                    {
                        /* 发生错误，重传 */
                        s_uiCfgFileReq[emPMID] = CONFIG_INFO_RETRANSFER;

                        /* 删除该错误文件 */
                        SysDeleteCfgFile((SysCfgFileType_t) usCfgFileType);
                    }
                }
                else
                {
                    /* 发生错误，重传 */
                    s_uiCfgFileReq[emPMID] = CONFIG_INFO_RETRANSFER;

                    /* 删除该错误文件 */
                    SysDeleteCfgFile((SysCfgFileType_t) usCfgFileType);
                }
            }
        }
    }
    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: InterCommParseCfgFileTable
*
* 功能描述: 解析来自PM的配置信息
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ----------------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | Cfg file table ... | CRC32 |
*              ----------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParseCfgFileTable(RecvBuffer_t *pstRecvBuf)
{
    CfgFileTableBlock_t* pstCfgTableBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    CMController_t emCMID = INVALID_CM_ID;

    if(pstRecvBuf != NULL)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stCfgTableBlock.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stCfgTableBlock.usOffset;

        /* 取出应答的内容 */
        if((usBlockLen == sizeof(CfgFileTableBlock_t)) && ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
        {
            pstCfgTableBlock = (CfgFileTableBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstCfgTableBlock->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstCfgTableBlock->uiCrc32;
                pucBuf = (uint8_t *) &(pstCfgTableBlock->stCfgTable);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstCfgTableBlock->usLen);
                if(uiCRC32 == uiCRC32calc)
                {
                    SysSetCfgFileTable(&(pstCfgTableBlock->stCfgTable));

                    emCMID = SysGetLocalCMID();
                    s_usLastFileIndex[emCMID] = FindLastCfgFileIndex(emCMID);
                }
            }
        }
    }
    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: InterCommParseRTData
*
* 功能描述: 解析来自PM的实时数据
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emPMID: PM的ID
*
* 输出参数:
*
* 返 回 值  : 无
*              --------------------------------------------------------------------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usServiceCode | uiSerialNum | uiOffset | usTransferFlag | usLen |Rt data type| RT data ... | CRC32 |
*              --------------------------------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommParseRTData(RecvBuffer_t *pstRecvBuf, PMController_t emPMID)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint8_t pucStartBuf = NULL;

    /* 应答的数据长度 */
    usBlockLen = pstRecvBuf->stRecvHeader.stRealTimeData.usLen;
    /* 应答在信息区的偏移地址 */
    usOffset = pstRecvBuf->stRecvHeader.stRealTimeData.usOffset;

    pucStartBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);
    pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

    usStartFlag = *((puint16_t)&pucBuf[RTDATA_START_FLAG_OFFSET]);
    usMsgLen = *((puint16_t)&pucBuf[RTDATA_LEN_OFFSET]);
    uiCRC32 = *((puint32_t)&pucBuf[RTDATA_CONTENT_OFFSET+usMsgLen]);

    /* 取出应答的内容 */
    if((usBlockLen == (usMsgLen + RTDATA_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < CM_RECV_BUFF_BODY_SIZE))
    {
        /* 判断数据块起始标识 */
        if (usStartFlag == DATA_BLOCK_START_FLAG)
        {
            /* 判断CRC32是否正确 */
            pucBuf = (puint8_t) &(pucBuf[RTDATA_CONTENT_OFFSET]);
            uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);
            if(uiCRC32 == uiCRC32calc)
            {
                SysSetRTDataMsg (emPMID, REAL_TIME_DATA, pucStartBuf, usBlockLen);
            }
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommReceivePreHandling
*
* 功能描述: 接收来自某系PM的数据，并对数据进行预处理
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : true: 收到数据且通过预处理成功
*        false: 未收到数据或预处理后认为其为无效数据
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommReceivePreHandling(RecvBuffer_t *pstRecvBuf, PMController_t emPMID)
{
    bool_t bRet = false;
    CMSlot_t emLocalCMSlot = INVALID_CM_SLOT;
    CMSlot_t emRecvCMSlot = INVALID_CM_SLOT;
    uint16_t usLen = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;

    if((pstRecvBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        /* 判断CRC是否正确 */
        pucBuf = (uint8_t *) &(pstRecvBuf->stRecvHeader);
        usLen = sizeof(RecvHeader_t) - CRC32_SIZE;
        uiCRC32calc = SysCrc32Cal(0, pucBuf, usLen);
        uiCRC32 = pstRecvBuf->stRecvHeader.uiCRC32;

        /* 解析并判断CM地址是否匹配 */
        emRecvCMSlot = pstRecvBuf->stRecvHeader.usCMAddr;
        emLocalCMSlot = GetCMSlotAddr(0x1000);

        if((emLocalCMSlot == emRecvCMSlot) && (emLocalCMSlot != INVALID_CM_SLOT) && (uiCRC32calc == uiCRC32))
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
        
        s_usRecvPackageSerialNum[emPMID] = pstRecvBuf->stRecvHeader.usPackageSerialNum;
        printf("recv sn =%d\n", pstRecvBuf->stRecvHeader.usPackageSerialNum);
    }
    
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommSend
*
* 功能描述: 把数据发送出去
*
* 输入参数: emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void InterCommSend(PMController_t emPMID)
{
    uint32_t uiState = 0;
    uiState = GetCMSendState(emPMID);
    //printf("send State reg=%d\n",uiState);
    
    if(uiState == 0)
    {
        InterCommSendHandling (&s_stSendBuffer, emPMID);
    }
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommSendHandling
*
* 功能描述: 数据发送处理函数
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数:
*
* 返 回 值  : 无
*
* 注意事项: 格式如下
* ___________________________________________________
*    | CM slot                                     |
*  H |---------------------------------------------|
*  E | Package serial num                          |
*  A |---------------------------------------------|
*  D | Data Len                                    |
*  E |---------------------------------------------|
*  R | Msg1 block length                           |
*    |---------------------------------------------|
*    | Msg1 block offset                           |
*    |---------------------------------------------|
*    | Msg2 block length                           |
*    |---------------------------------------------|
*    | Msg2 block offset                           |
*    |---------------------------------------------|
*    | Msg3 block length                           |
*    |---------------------------------------------|
*    | Msg3 block offset                           |
*    |---------------------------------------------|
*    | ...                                         |
*    |---------------------------------------------|
*    | CRC32                                       |
*____|_____________________________________________|
*    | usStartFlag | usMsgLen | Msg1...    | CRC32 |
*  B |---------------------------------------------|
*  O | usStartFlag | usMsgLen | Msg2...    | CRC32 |
*  D |---------------------------------------------|
*  Y | usStartFlag | usMsgLen | Msg3...    | CRC32 |
*    |---------------------------------------------|
*    |   ...                                       |
*____|_____________________________________________|
***********************************************************************************************************************
*/
static void InterCommSendHandling(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    CMSlot_t emLocalCMSlot = INVALID_CM_SLOT;     /* CM的槽号 */
    uint32_t uiCRC32 = 0;

    //int i = 0;//test
    //uint8_t *pBuf = NULL;
    
    /* 打包前对相关数据进行初始化  */
    s_stCommSend.usUsed = 0;
    s_stCommSend.usOffset = 0;
    memset((puint8_t)&(pstSendBuf->stSendHeader.usCMAddr), 0, sizeof(SendHeader_t));

    /* 打包CM的槽号地址  */
    emLocalCMSlot =  GetCMSlotAddr(0x1000);
    pstSendBuf->stSendHeader.usCMAddr = (uint16_t)emLocalCMSlot;

    /* 打包数据包序列号 */
    pstSendBuf->stSendHeader.usPackageSerialNum = s_usSendPackageSerialNum[emPMID];
    printf("send sn=%d\n", s_usSendPackageSerialNum[emPMID]);
    s_usSendPackageSerialNum[emPMID]++;
    
    /* 打包CM的状态信息  */
    InterCommPackSysCMState (pstSendBuf);

    /* 打包对探测包的应答  */
    InterCommPackDetectMsgAck(pstSendBuf, emPMID);

    /* 打包配置信息请求  */
    //InterCommPackConfigReqHandling(pstSendBuf, emPMID);

    /* 打包SOE请求  */
    InterCommPackSOEReq (pstSendBuf, emPMID);

    /* 打包组态软件请求  */
    InterCommPackCS1131Req (pstSendBuf, emPMID);

    /* 打包OPC请求 */
    InterCommPackOPCReq (pstSendBuf, emPMID);

    /* 打包客户端请求 */
    InterCommPackClientReq (pstSendBuf, emPMID);

    /* 打包客户端应答 */
//    InterCommPackClientResp (pstSendBuf, emPMID);

    /* 打包AMS请求  */
    InterCommPackAMSReq (pstSendBuf, emPMID);

    /* 打包PTP请求 */
    InterCommPackPTP (pstSendBuf, emPMID, P2P_EXT_REQ);
    /* 打包PTP应答*/
    InterCommPackPTP (pstSendBuf, emPMID, P2P_EXT_RESP);

    /* 打包Modbus serial请求/应答 */
    InterCommPackModbus(pstSendBuf, emPMID);

    /* 打包实时数据应答  */
    InterCommPackRTDataResp (pstSendBuf, emPMID);

    /* 打包内部命令  */
    InterCommPackInterCmd(pstSendBuf, emPMID);

    /* 设置数据内容长度  */
    pstSendBuf->stSendHeader.usDataLen = s_stCommSend.usUsed;

    /* 计算并填充头部CRC32 */
    uiCRC32 = SysCrc32Cal(0, (puint8_t)&(pstSendBuf->stSendHeader.usCMAddr),sizeof(SendHeader_t) - CRC32_SIZE);
    pstSendBuf->stSendHeader.uiCRC32 = uiCRC32;

    //通过localbus发送数据
    CMSend((uint32_t) emPMID, (puint8_t) &s_stSendBuffer, s_stCommSend.usUsed + sizeof(SendHeader_t), 0);

    
#if 0
    printf("send-----");
    pBuf = (puint8_t)pstSendBuf;
    for(i=0;i<sizeof(SendHeader_t);i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
    //printf("sizeof(SendHeader_t) = %d\n", sizeof(SendHeader_t));
    
    pBuf = (puint8_t)pstSendBuf;
    for(i=sizeof(SendHeader_t);i<sizeof(SendHeader_t)+pstSendBuf->stSendHeader.usDataLen;i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
    
#endif
    return;
}
#if 0
/*
***********************************************************************************************************************
* 函数名称: InterCommPackConfigReqHandling
*
* 功能描述: 打包获取CM配置信息的请求命令
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | stConfigReq | CRC32 |
*              ---------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackConfigReqHandling(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    bool_t bRet = false;
    uint16_t usMsgLen = 0;
    uint8_t ucArr[16];

    /* 获取消息长度 */
    SysGetMsgLen(emPMID, INTER_CMD_REQ, &usMsgLen);

    if((usMsgLen > 0) && (usMsgLen < 16))
    {
        /* 填消息内容 */
        SysGetMsg (emPMID, INTER_CMD_REQ, &(ucArr[0]), &usMsgLen);
    }

    if(CONFIG_INFO_ALL_CFG_START == s_uiCfgFileReq[emPMID])
    {
        /* 接下来发送开始命令 */
        bRet = InterCommPackConfigReq(pstSendBuf, emPMID, CONFIG_INFO_ALL_CFG_START);
        if(true == bRet)
        {
            s_uiCfgFileReq[emPMID] = CONFIG_INFO_NO_REQ;
        }
    }
    else if(CONFIG_INFO_INC_CFG_START == s_uiCfgFileReq[emPMID])
    {
        /* 接下来发送开始命令 */
        bRet = InterCommPackConfigReq(pstSendBuf, emPMID, CONFIG_INFO_INC_CFG_START);
        if(true == bRet)
        {
            s_uiCfgFileReq[emPMID] = CONFIG_INFO_NO_REQ;
        }
    }
    else if(CONFIG_INFO_CONTINUE == s_uiCfgFileReq[emPMID])
    {
        /* 接下来发送继续命令 */
        bRet = InterCommPackConfigReq(pstSendBuf, emPMID, CONFIG_INFO_CONTINUE);

        if(true == bRet)
        {
            s_uiCfgFileReq[emPMID] = CONFIG_INFO_NO_REQ;
        }
    }
    else if(CONFIG_INFO_RETRANSFER == s_uiCfgFileReq[emPMID])
    {
        /* 传输过程中出错，则发送重传命令 */
        bRet = InterCommPackConfigReq(pstSendBuf, emPMID, CONFIG_INFO_RETRANSFER);

        if(true == bRet)
        {
            s_uiCfgFileReq[emPMID] = CONFIG_INFO_NO_REQ;
        }
    }
    else if(TRANSFER_CONFIG_INFO_FINISHED == s_uiCfgFileReq[emPMID])
    {
        /* 传输完毕则发送完成命令 */
        bRet = InterCommPackConfigReq(pstSendBuf, emPMID, TRANSFER_CONFIG_INFO_FINISHED);

        if(true == bRet)
        {
            s_uiCfgFileReq[emPMID] = CONFIG_INFO_NO_REQ;
        }
        printf("finished\n");
    }
    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: InterCommPackSysCMState
*
* 功能描述: 打包CM的状态信息
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | CM state... | CRC32 |
*              ---------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackSysCMState(SendBuffer_t *pstSendBuf)
{
    SysCMStateBlock_t stSysCMstateBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;
    struct timeval stTmpTimeVal;

    if(pstSendBuf != NULL)
    {
        memset((puint8_t)&stSysCMstateBlock, 0, sizeof(SysCMStateBlock_t));
        /* CM状态信息数据块 */
        stSysCMstateBlock.usStartFlag = DATA_BLOCK_START_FLAG;
        stSysCMstateBlock.usLen = sizeof(SysCMState_t);
        SysGetSysCMState(&stSysCMstateBlock.stCMState);

        SysGetCM2PMInfo(&stSysCMstateBlock.stCM2PMInfo);
        //printf("ver=%x", stSysCMstateBlock.stCM2PMInfo.uiCMVer);

        memset(&stTmpTimeVal, 0, sizeof(struct timeval));
        gettimeofday(&stTmpTimeVal, NULL);

        stSysCMstateBlock.stCMState.stCMtime[CM1].uiSecond = stTmpTimeVal.tv_sec;
        stSysCMstateBlock.stCMState.stCMtime[CM1].usMiliSecond = stTmpTimeVal.tv_usec / 1000;
        stSysCMstateBlock.stCMState.stCMtime[CM2].uiSecond = stTmpTimeVal.tv_sec;
        stSysCMstateBlock.stCMState.stCMtime[CM2].usMiliSecond = stTmpTimeVal.tv_usec / 1000;
        stSysCMstateBlock.stCMState.stCMtime[CM3].uiSecond = stTmpTimeVal.tv_sec;
        stSysCMstateBlock.stCMState.stCMtime[CM3].usMiliSecond = stTmpTimeVal.tv_usec / 1000;
        stSysCMstateBlock.stCMState.stCMtime[CM4].uiSecond = stTmpTimeVal.tv_sec;
        stSysCMstateBlock.stCMState.stCMtime[CM4].usMiliSecond = stTmpTimeVal.tv_usec / 1000;

        SysGetLocalProjVersion(&stSysCMstateBlock.stProjVer);
        printf("ProjCrc=0x%x\n", stSysCMstateBlock.stProjVer.uiProjCRC);

        pucBuf = (uint8_t *) &(stSysCMstateBlock.stCMState);
        uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(SysCMState_t));
        stSysCMstateBlock.uiCrc32 = uiCRC32;

        /* 计算CM的状态信息长度及偏移地址 */
        usBlockLen = sizeof(SysCMStateBlock_t);
        usOffset = s_stCommSend.usOffset;

        /* 存储CM的状态信息 */
        if((usOffset + usBlockLen) < CM_SEND_BUFF_BODY_SIZE)
        {
            memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stSysCMstateBlock, usBlockLen);

            pstSendBuf->stSendHeader.stCMStateInfo.usLen = usBlockLen;
            pstSendBuf->stSendHeader.stCMStateInfo.usOffset = usOffset;
            s_stCommSend.usUsed += usBlockLen;
            s_stCommSend.usOffset += usBlockLen;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommPackDetectMsgAck
*
* 功能描述: 打包对来自PM的探测消息的应答
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Detect msg ACK... | CRC32 |
*              ------------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackDetectMsgAck(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usInterCommState = INTER_COMM_NORMAL;
    DetectMsgAckBlock_t stDetectMsgAckBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        /* 获取本CM与指定PM的通讯状态 */
        InterCommGetLocalState(emPMID, &usInterCommState);
        
//        //printf("usInterCommState=%d\n", usInterCommState);

//        if(INTER_COMM_DETECT_MSG == usInterCommState)
        {
            /* 生成应答数据块*/
            stDetectMsgAckBlock.usStartFlag = DATA_BLOCK_START_FLAG;
            stDetectMsgAckBlock.usLen = sizeof(DetectMsgAck_t);
            stDetectMsgAckBlock.stDetectMsgAck.uiFlag = DETECT_MSG_ACK_FLAG;

            pucBuf = (uint8_t *) &(stDetectMsgAckBlock.stDetectMsgAck);
            uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(DetectMsgAck_t));
            stDetectMsgAckBlock.uiCrc32 = uiCRC32;

            /* 计算应答信息长度及偏移地址 */
            usBlockLen = sizeof(stDetectMsgAckBlock);
            usOffset = s_stCommSend.usOffset;

            if((usOffset + usBlockLen) < CM_SEND_BUFF_BODY_SIZE)
            {
                memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stDetectMsgAckBlock, usBlockLen);
                pstSendBuf->stSendHeader.stDetectMsgAckBlock.usLen = usBlockLen;
                pstSendBuf->stSendHeader.stDetectMsgAckBlock.usOffset = usOffset;
                s_stCommSend.usUsed += usBlockLen;
                s_stCommSend.usOffset += usBlockLen;
            }
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommPackCS1131Req
*
* 功能描述: 打包来自CS1131的请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              --------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | CS1131 req... | CRC32 |
*              --------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackCS1131Req(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, CS1131_REQ, &usMsgLen);

        /* 判断是否有来自CS1131的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, CS1131_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stCS1131Req.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stCS1131Req.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stCS1131Req.usLen = 0;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommPackSOEReq
*
* 功能描述: 打包来自SOE的请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              -----------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | SOE req... | CRC32 |
*              -----------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackSOEReq(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 填消息长度 */
        SysGetMsgLen(emPMID, SOE_REQ, &usMsgLen);

        /* 判断是否有来自SOE的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, SOE_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stSOEReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stSOEReq.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stSOEReq.usLen = 0;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommPackOPCReq
*
* 功能描述: 打包来自OPC的请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              -----------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | OPC req... | CRC32 |
*              -----------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackOPCReq(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, OPC_REQ, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, OPC_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stOPCReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stOPCReq.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stOPCReq.usLen = 0;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommPackClientReq
*
* 功能描述: 打包来自CLIENT的请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              --------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Client req... | CRC32 |
*              --------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackClientReq(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, CLIENT_REQ, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, CLIENT_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stClientReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stClientReq.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stClientReq.usLen = 0;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommPackClientResp
*
* 功能描述: 打包来自CLIENT的应答
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Client resp... | CRC32 |
*              ---------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackClientResp(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, CLIENT_RESP, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, CLIENT_RESP, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stClientResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stClientResp.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stClientResp.usLen = 0;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommPackAMSReq
*
* 功能描述: 打包来自AMS的请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              -----------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | AMS req... | CRC32 |
*              -----------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackAMSReq(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, AMS_REQ, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, AMS_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stAMSReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stAMSReq.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stAMSReq.usLen = 0;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: InterCommPackPTP
*
* 功能描述: 打包来自PTP的请求/应答
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | P2P content... | CRC32 |
*              ---------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackPTP(SendBuffer_t *pstSendBuf, PMController_t emPMID, SysP2PMsgType_t emMsgType)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM) && (emMsgType < NUM_OF_P2PMSGTYPE))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetP2PMsgLen(emPMID, emMsgType, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetP2PMsg (emPMID, emMsgType, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            if(P2P_EXT_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stP2PReq.usOffset = usOffset;
            }
            else if(P2P_EXT_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stP2PResp.usOffset = usOffset;
            }
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            if(P2P_EXT_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PReq.usLen = 0;
            }
            else if(P2P_EXT_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PResp.usLen = 0;
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommPackModbus
*
* 功能描述: 打包来自Modbus的请求/应答
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*        emMsgType: Modbus 消息类型
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ----------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Modbus Msg content... | CRC32 |
*              ----------------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackModbus(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgNum = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL)&& (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息个数 */
        SysGetModbusMsgNum(emPMID, &usMsgNum);

        /* 判断是否有来自Modbus的数据，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgNum > 0) && ((usOffset + MAX_MODBUS_MSG_LEN + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息内容 */
            SysGetModbusMsgQueue (emPMID, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stModbus.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stModbus.usOffset = usOffset;

            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stModbus.usLen = 0;
        }
    }
    return;
}
#if 0
/*
***********************************************************************************************************************
* 函数名称: InterCommPackConfigReq
*
* 功能描述: 打包获取CM配置信息的请求命令
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              ---------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | stConfigReq | CRC32 |
*              ---------------------------------------------
***********************************************************************************************************************
*/
static bool_t InterCommPackConfigReq(SendBuffer_t *pstSendBuf, PMController_t emPMID, uint32_t uiReqCmd)
{
    ConfigReqBlock_t stConfigReqBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;
    bool_t bRet = false;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        /* 根据实际情况，构造请求命令 */
        stConfigReqBlock.usStartFlag = DATA_BLOCK_START_FLAG;
        stConfigReqBlock.usLen = sizeof(ConfigReq_t);
        stConfigReqBlock.stConfigReq.uiRequest = uiReqCmd;
        pucBuf = (uint8_t *) &(stConfigReqBlock.stConfigReq);
        uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(ConfigReq_t));
        stConfigReqBlock.uiCrc32 = uiCRC32;

        /* 总长度为请求内容长度加上CRC32的长度 */
        usBlockLen = sizeof(stConfigReqBlock);
        usOffset = s_stCommSend.usOffset;

        if((usOffset + usBlockLen) < CM_SEND_BUFF_BODY_SIZE)
        {
            /* 将请求命令放入待发送缓冲区，并且更新待发送缓冲区的使用情况 */
            memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stConfigReqBlock, usBlockLen);

            pstSendBuf->stSendHeader.stConfigReqBlock.usLen = usBlockLen;
            pstSendBuf->stSendHeader.stConfigReqBlock.usOffset = usOffset;
            s_stCommSend.usUsed += usBlockLen;
            s_stCommSend.usOffset += usBlockLen;

            bRet = true;
        }
    }

    return bRet;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: InterCommPackRTDataResp
*
* 功能描述: 打包对实时数据的应答
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              -----------------------------------------------------------------------------------------------------------
* 注意事项:  格式: |usStartFlag|usLen| usStartFlag | usServiceCode | uiSerialNum | usLen | usAck | usErrCode | CRC32 | CRC32 |
*              -----------------------------------------------------------------------------------------------------------
*                                |<---------------------RT DATA ACK MSG----------------------------------------->|
***********************************************************************************************************************
*/
static void InterCommPackRTDataResp(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL)&& (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetRTDataMsgLen (emPMID, REAL_TIME_DATA_ACK, &usMsgLen);

        /* 判断是否有应答，并且判断是否有剩余空间来存放应答内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetRTDataMsg (emPMID, REAL_TIME_DATA_ACK, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stRealTimeDataAck.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stRealTimeDataAck.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stRealTimeDataAck.usLen = 0;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommPackInterCmd
*
* 功能描述: 打包内部命令请求
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emPMID: PM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
*              -------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Inter cmd... | CRC32 |
*              -------------------------------------------------
***********************************************************************************************************************
*/
static void InterCommPackInterCmd(SendBuffer_t *pstSendBuf, PMController_t emPMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emPMID < NUM_OF_PM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emPMID, INTER_CMD_REQ, &usMsgLen);

        /* 判断是否有来自OPC的请求，并且判断是否有剩余空间来存放请求内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE) < CM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emPMID, INTER_CMD_REQ, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stInterCmd.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stInterCmd.usOffset = usOffset;
            s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
        }
        else
        {
            pstSendBuf->stSendHeader.stInterCmd.usLen = 0;
        }
    }
    return;
}


/*
***********************************************************************************************************************
* 函数名称: InterCommSetLocalState
*
* 功能描述: 设置本CM与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*        usInterState:内部通讯状态
*
* 输出参数: 无
*
* 返 回 值  : 设置成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommSetLocalState(PMController_t emPMID, uint16_t usInterState)
{
    bool_t bRet = false;
    CMSlot_t emLocalSlot = INVALID_CM_SLOT;

    emLocalSlot = GetCMSlotAddr(10);

    if((emPMID < NUM_OF_PM) && (emLocalSlot != INVALID_CM_SLOT))
    {
        s_usInterCommState[emPMID][emLocalSlot] = usInterState;
        SysSetLocalInterCommState( emPMID, usInterState );  /* Added by wangdong */
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: InterCommGetLocalState
*
* 功能描述: 本CM获取与指定PM的内部通讯状态
*
* 输入参数: emPMID:指定PM的ID
*
* 输出参数: pusInterState:指向内部通讯状态
*
* 返 回 值  : 获取成功或失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
static bool_t InterCommGetLocalState(PMController_t emPMID, puint16_t pusInterState)
{
    bool_t bRet = false;
    CMSlot_t emLocalSlot = INVALID_CM_SLOT;

    emLocalSlot = GetCMSlotAddr(10);

    if((emPMID < NUM_OF_PM) && (emLocalSlot != INVALID_CM_SLOT) && (pusInterState != NULL))
    {
        *pusInterState = s_usInterCommState[emPMID][emLocalSlot];
        bRet = true;
    }

    return bRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
