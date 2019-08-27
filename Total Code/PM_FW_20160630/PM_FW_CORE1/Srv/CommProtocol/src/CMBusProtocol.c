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
*  文件名称    : CMBusProtocol.c
*
*  功能简介    :
*
*  作者            : 李琦
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
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/Scheduler/cagos.h"
#include "../inc/CMBusProtocol.h"
#include "../inc/RealTimeData.h"
#include "../inc/CMTable.h"
#include "Srv/SharedMem/inc/SharedMem.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static PMRecvBuffer_t s_stRecvBuffer;                     /* 接收数据缓冲区 */
static PMSendBuffer_t s_stSendBuffer;                     /* 发送数据缓冲区 */
static PMCommHandling_t s_stCommRecv;                     /* 记录从接收缓冲区解析数据时的动态操作 */
static PMCommHandling_t s_stCommSend;                     /* 记录向发送缓冲区写入打包数据时的动态操作 */
//static uint16_t s_usSendPackageSerialNum[NUM_OF_CM];      /* 发送数据包序列号 */
//static uint16_t s_usRecvPackageSerialNum[NUM_OF_CM];      /* 接收数据包序列号 */
static bool_t s_bSendDetectMsgFlag[NUM_OF_CM];            /* 对相应CM的探测是否成功 true-成功  false-失败 */
static uint32_t s_uiCfgReqCmd[NUM_OF_CM];                 /* 来自CM的配置请求命令 */
static uint32_t s_uiRTDataSerialNum[NUM_OF_CM];           /* 发送实时数据包序列号 */

static uint16_t s_usCfgFileIndex[NUM_OF_CM];
static uint32_t s_uiRdOffset[NUM_OF_CM];
static uint16_t s_uiRdTotalNum[NUM_OF_CM];
static uint32_t s_uiCfgTransFlag[NUM_OF_CM];

static SysRtDataAreaType_t s_emDataArea[NUM_OF_CM];

bool_t s_bCM_slot_table[NUM_OF_SLOT]; //true代表探测到槽上有CM

static uint16_t s_usSdDetectSn[NUM_OF_SLOT];              /* 发送探测数据包序列号 */
static uint16_t s_usRvDetectSn[NUM_OF_SLOT];              /* 接收应答数据包序列号 */

static uint16_t s_usCount[NUM_OF_SLOT];
static bool_t s_bSendMsgNum[NUM_OF_SLOT];
static bool_t s_bRecvMsgAck[NUM_OF_SLOT];

static uint32_t s_uiCommCycle;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 接收数据处理: PM从指定的CM接收数据 */
//static void CMBusProtocolReceive (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);
static bool_t CMBusProtocolReceive (PMRecvBuffer_t *pstRecvBuf, CMSlot_t emCMSlot);
/* 接收数据预处理 */
//static bool_t CMBusProtocolReceivePreHandling (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);
static bool_t CMBusProtocolReceivePreHandling (PMRecvBuffer_t *pstRecvBuf, CMSlot_t emCMSlot);
/* 解析系统状态信息 */
static void CMBusProtocolParseSysCMState (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);
/* 解析探测数据 */
static bool_t CMBusProtocolParseDetectMsgAck (PMRecvBuffer_t *pstRecvBuf);
/* 解析序列号 */
static bool_t CMBusProtocolParsePackageSerialNum (PMRecvBuffer_t *pstRecvBuf, uint16_t usSerialNum);
/* 解析来自PC端软件的请求 */
static void CMBusProtocolParsePCsoftware (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID, SysMsgType_t emMsgType);
/* 解析来自CM模块的CM配置信息请求命令 */
static void CMBusProtocolParseConfigReq(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);
/* 解析实时数据应答 */
static void CMBusProtocolParseRTDataAck(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);
/* 解析P2P数据 */
static void CMBusProtocolParseP2P(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID, SysP2PMsgType_t emMsgType);
/* 解析Modbus数据 */
static void CMBusProtocolParseModbus(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID);

/* 发送数据处理: PM向指定的CM发送数据*/
static void CMBusProtocolSend(PMSendBuffer_t *pstSendBuf, CMSlot_t emCMSlot);
//static void CMBusProtocolSend (PMSendBuffer_t *pstSendBuf, CMController_t emCMID);
/* 打包系统状态信息 */
static void CMBusProtocolPackSysState (PMSendBuffer_t *pstSendBuf);
/* 打包探测消息，将其发送到指定的CM */
//static void CMBusProtocolPackDetectMsg (PMSendBuffer_t *pstSendBuf, CMController_t emCMID);
static void CMBusProtocolPackDetectMsg (PMSendBuffer_t *pstSendBuf, CMSlot_t emCMSlot, uint32_t uiMsg);
/* 打包对PC端软件的应答信息 */
static void CMBusProtocolPackPCsoftware (PMSendBuffer_t *pstSendBuf, CMController_t emCMID, SysMsgType_t emMsgType);
/* 打包配置文件 */
static void CMBusProtocolPackCfgFile (PMSendBuffer_t *pstSendBuf, CMController_t emCMID);
/* 打包实时数据 */
static void CMBusProtocolPackRTData (PMSendBuffer_t *pstSendBuf, CMController_t emCMID);
/* 打包P2P数据 */
static void CMBusProtocolPackP2P (PMSendBuffer_t *pstSendBuf, CMController_t emCMID, SysP2PMsgType_t emMsgType);
/* 打包配置表 */
static void CMBusProtocolPackCfgFileTable(PMSendBuffer_t *pstSendBuf, CMController_t emCMID);
/* 传输配置文件初始化 */
static void TransCfgFileInit(CMController_t emCMID);
/* 重传输配置文件 */
static void RetransCfgFile(CMController_t emCMID);

static int16_t cmbus_send(int32_t station, int8_t *buf, int32_t len,int32_t timeout);
static int16_t cmbus_recv(int8_t *buf, int32_t timeout);
static int16_t cmbus_send_state(int32_t timeout);
/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolInit
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
void CMBusProtocolInit(void)
{
    /* 对静态变量初始化 */
    memset((puint8_t)&s_stRecvBuffer, 0, sizeof(PMRecvBuffer_t));
    memset((puint8_t)&s_stSendBuffer, 0, sizeof(PMSendBuffer_t));
    memset((puint8_t)&s_stCommRecv, 0, sizeof(PMCommHandling_t));
    memset((puint8_t)&s_stCommSend, 0, sizeof(PMCommHandling_t));
//    memset((puint8_t)&s_usSendPackageSerialNum[0], 0, sizeof(uint16_t)*NUM_OF_CM);
//    memset((puint8_t)&s_usRecvPackageSerialNum[0], 0, sizeof(uint16_t)*NUM_OF_CM);
    memset((puint8_t)&s_bSendDetectMsgFlag[0], 1, sizeof(bool_t)*NUM_OF_CM);
    memset((puint32_t)&s_uiRTDataSerialNum[0], 0, sizeof(uint32_t)*NUM_OF_CM);
    memset((puint32_t)&s_uiCfgReqCmd[0], 0, sizeof(uint32_t)*NUM_OF_CM);

    memset((puint32_t)&s_emDataArea[0], 0, sizeof(SysRtDataAreaType_t)*NUM_OF_CM);

    memset((puint8_t)&s_bCM_slot_table[0], 0, sizeof(bool_t)*NUM_OF_SLOT);

    memset((puint8_t)&s_usSdDetectSn[0], 0, sizeof(uint16_t)*NUM_OF_SLOT);
    memset((puint8_t)&s_usRvDetectSn[0], 0, sizeof(uint16_t)*NUM_OF_SLOT);

    memset((puint8_t)&s_usCount[0], 0, sizeof(uint16_t)*NUM_OF_SLOT);

    memset((puint8_t)&s_bSendMsgNum[0], 0, sizeof(uint16_t)*NUM_OF_SLOT);
    memset((puint8_t)&s_bRecvMsgAck[0], 0, sizeof(uint16_t)*NUM_OF_SLOT);

    s_uiCommCycle = 0;

    /* 槽号与逻辑号映射表初始化 */
    SysCMConfigTableInit();

    /* 向CM1~CM4传输配置文件，初始化 */
    TransCfgFileInit(CM1);
    TransCfgFileInit(CM2);
    TransCfgFileInit(CM3);
    TransCfgFileInit(CM4);

    if(set_pm_slot()<0)
    {
        printf("set pm slot error\n");
    }
    
    CMTableInit();

    if((check_cm_recv() < 0) || (2 == check_cm_recv()))
    {
        /* PM向CM发送数据 */
        CMBusProtocolSend(&s_stSendBuffer, SLOT3);

        /* 发送包的序列号加一 */
        s_usSdDetectSn[SLOT3]++;
        udelay(800);
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolCycle
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
void CMBusProtocolCycle(void)
{
    static uint8_t j = SLOT3;
    bool_t bRecvFlag = false;

//    printf("------------------\n");
    
    //printf("  CMBusProtocolCycle  \n");
    
    s_uiCommCycle++;

    /* 从当前槽上收到数据 */
    if(1 == check_cm_recv())
    {
        /* PM接收来自CM的数据 */
        if( cmbus_recv(&s_stRecvBuffer, 0x100) < 0)
        {
            //printf("cmbus recv data fail\n");

            s_usCount[j]++;

            if(s_usCount[j] > 20)
            {
                s_bCM_slot_table[j] = false;
                s_usCount[j] = 0;
            }

            s_bRecvMsgAck[j] = false;
        }
        else
        {
            //printf("cmbuf recv data ok %x slot=%d\n", s_stRecvBuffer.stRecvHeader.usCMAddr, j);

            bRecvFlag = CMBusProtocolReceive (&s_stRecvBuffer, j);

            if(true == bRecvFlag)
            {
                s_bCM_slot_table[j] = true;
                s_usCount[j] = 0;

                s_bRecvMsgAck[j] = true;
                s_bSendMsgNum[j] = 0;
                
                SetLEDState(LED_NAME_CMBUS, LED_BLINK); 
            }
            else
            {
                s_usCount[j]++;

                if(s_usCount[j] > 20)
                {
                    s_bCM_slot_table[j] = false;
                    SetLEDState(LED_NAME_CMBUS, LED_OFF); 
                }
                s_bRecvMsgAck[j] = false;
            }
        }
    }

    /* 向下一个槽发送数据 */
    j++;
    if(j > SLOT15)
    {
        j = SLOT3;
    }

    /* PM向CM发送数据 */
    CMBusProtocolSend(&s_stSendBuffer, j);

    /* 发送包的序列号加一 */
    s_usSdDetectSn[j]++;

    UpdateCMTable();
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolReceive
*
* 功能描述: PM从指定CM接收数据
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstRecvBuf: 指向接收数据缓冲区
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
static bool_t CMBusProtocolReceive (PMRecvBuffer_t *pstRecvBuf, CMSlot_t emCMSlot)
{
    bool_t bRet = false;
    CMController_t emCMID = INVALID_CM_ID;
    int i = 0;//test
    uint8_t *pBuf = NULL;

    /* 参数有效性检查 */
    if((pstRecvBuf != NULL) && (emCMSlot < NUM_OF_SLOT))
    {
        /* 接收数据预处理 */
        if( CMBusProtocolReceivePreHandling(pstRecvBuf, emCMSlot))
        {
            if(CMBusProtocolParseDetectMsgAck(pstRecvBuf))
            {
                /* 解析CM状态信息 */
                emCMID = CMSLotIDToLogicID(emCMSlot);
                
                if(emCMID < NUM_OF_CM)
                {
                    CMBusProtocolParseSysCMState(pstRecvBuf, emCMID);
                    CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, CS1131_REQ);
                    
                    /* 解析实时数据应答 */
                    CMBusProtocolParseRTDataAck(pstRecvBuf, emCMID);
                }
#if 0
                printf("recv ack, cm id=%d\n",emCMID);

                pBuf = (puint8_t)pstRecvBuf;
                for(i=0;i<sizeof(PMRecvHeader_t);i++)
                {
                    printf("%2x  ", pBuf[i]);
                }
                printf("\n");
                printf("RecvHeaderLen = %d\n", sizeof(PMRecvHeader_t));

                pBuf = (puint8_t)pstRecvBuf;
                for(i=sizeof(PMRecvHeader_t);i<sizeof(PMRecvHeader_t)+pstRecvBuf->stRecvHeader.usDataLen;i++)
                {
                    printf("%2x  ", pBuf[i]);
                }
                printf("\n");
#endif
                bRet = true;
            }
        }
    }

    return bRet;
}
#if 0
static void CMBusProtocolReceive (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID)
{
    bool_t bResult = false;

    /* 参数有效性检查 */
    if((pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 接收数据预处理 */
        bResult = CMBusProtocolReceivePreHandling(pstRecvBuf, emCMID);
    }

    /* 收到数据且数据预处理成功 */
    if(true == bResult)
    {
        /* 解析CM状态信息 */
        CMBusProtocolParseSysCMState(pstRecvBuf, emCMID);

        /* 解析探测消息应答 */
        bResult = CMBusProtocolParseDetectMsgAck(pstRecvBuf);

        /* 需要接收探测消息应答并且未收到探测消息应答 */
        if((false == bResult) && (true == s_bSendDetectMsgFlag[emCMID]))
        {
            /* 收发序列号均清 0 */
            s_usSendPackageSerialNum[emCMID] = 0;
            s_usRecvPackageSerialNum[emCMID] = 0;

            /* 设置与对应的CM的通讯状态为：异常，发送探测消息 */
            SysSetLocalCMBusCommState(emCMID, CMBUS_COMM_DETECT_MSG);

            /* 需要发送探测消息 */
            s_bSendDetectMsgFlag[emCMID] = true;
        }
        else /* 收到探测消息应答或者不需要接收探测消息应答*/
        {
            /* 解析数据包序列号 */
            bResult = CMBusProtocolParsePackageSerialNum (pstRecvBuf, s_usRecvPackageSerialNum[emCMID]);

            /* 序列号正确 */
            if(true == bResult)
            {
                /* 更新下次接收包预期序列号 */
                s_usRecvPackageSerialNum[emCMID] = s_usSendPackageSerialNum[emCMID];

                /* 解析其它数据，如SOE、CS1131等 */
                CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, SOE_REQ);
                CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, CS1131_REQ);
                CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, OPC_REQ);
                CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, CLIENT_REQ);
                CMBusProtocolParsePCsoftware(pstRecvBuf, emCMID, INTER_CMD_REQ);

                CMBusProtocolParseP2P(pstRecvBuf, emCMID, P2P_EXT_REQ);

                CMBusProtocolParseP2P(pstRecvBuf, emCMID, P2P_EXT_RESP);

                CMBusProtocolParseModbus(pstRecvBuf, emCMID);

                CMBusProtocolParseConfigReq(pstRecvBuf, emCMID);

                /* 解析实时数据应答 */
                CMBusProtocolParseRTDataAck(pstRecvBuf, emCMID);

                /* 设置与对应的CM的通讯状态为：正常*/
                SysSetLocalCMBusCommState(emCMID, CMBUS_COMM_NORMAL);

                /* 无需再发送探测消息 */
                s_bSendDetectMsgFlag[emCMID] = false;
            }
            else /* 序列号错误 */
            {
                s_usSendPackageSerialNum[emCMID] = 0;
                s_usRecvPackageSerialNum[emCMID] = 0;

                /* 设置与对应的CM的通讯状态为：异常，发送探测消息 */
                SysSetLocalCMBusCommState(emCMID, CMBUS_COMM_DETECT_MSG);

                /* 需要发送探测消息 */
                s_bSendDetectMsgFlag[emCMID] = true;
            }
        }
    }
    else
    {
        /* 长时间未收到数据或者数据出错 */
        //若达到未接收到数据的最长时间则PM向相应的CM发送探测消息
        //s_bSendDetectMsgFlag[emCMID] = true;
    }
    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolReceivePreHandling
*
* 功能描述: 接收来自某系CM的数据，并对数据进行预处理
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstRecvBuf: 指向接收数据缓冲区
*
* 返 回 值  : true: 收到数据且通过预处理成功
*        false: 未收到数据或预处理后认为其为无效数据
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
***********************************************************************************************************************
*/
static bool_t CMBusProtocolReceivePreHandling (PMRecvBuffer_t *pstRecvBuf, CMSlot_t emCMSlot)
{
    bool_t bRet = false;
    uint16_t usLen = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    CMSlot_t emCMSlotRecv = INVALID_CM_SLOT;

    if((pstRecvBuf != NULL) && (emCMSlot < NUM_OF_SLOT))
    {
        /* 从FPGA相应的接收缓冲区读取数据 */

        /* 获取包内CRC，并重新计算CRC*/
        pucBuf = (uint8_t *) &(pstRecvBuf->stRecvHeader);
        usLen = sizeof(PMRecvHeader_t) - CRC32_SIZE;
        uiCRC32calc = SysCrc32Cal(0, pucBuf, usLen);
        uiCRC32 = pstRecvBuf->stRecvHeader.uiCRC32;

        /* 获取包内CM地址：槽号 */
        emCMSlotRecv = pstRecvBuf->stRecvHeader.usCMAddr;

        /* 判断CRC是否正确 ，并判断CM地址是否正确*/
        if((uiCRC32calc == uiCRC32) && (emCMSlot == emCMSlotRecv))
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }

//        printf("CMSlotRecv = %d  RecvSn=%d \n", emCMSlotRecv, pstRecvBuf->stRecvHeader.usPackageSerialNum);
        //printf("crc=%x    %x\n",uiCRC32calc,uiCRC32);
        //printf("emCMSlot=%d\n",emCMSlot);
    }


    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseSysCMState
*
* 功能描述: 解析CM状态
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emCMID: 指定CM的ID
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ---------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | CM state... | CRC32 |
*              ---------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParseSysCMState (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID)
{
    SysCMStateBlock_t* pstCMStateBlock = NULL;                  /* 指向CM状态信息数据块 */
    uint16_t usBlockLen = 0;                                    /* CM状态信息数据块长度 */
    uint16_t usOffset = 0;                                      /* 在Body中的偏移地址 */
    uint32_t uiCRC32 = 0;                                       /* 接收到的CRC32 */
    uint32_t uiCRC32calc = 0;                                   /* 根据接收内容计算出的CRC32 */
    puint8_t pucBuf = NULL;                                     /* 指向CM状态的起始地址 */
    uint16_t usCMState = 0;                                     /* 存放CM的状态 */

    /* 参数有效性检查 */
    if((pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 系统状态信息的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stCMStateBlock.usLen;
        /* 系统状态信息在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stCMStateBlock.usOffset;

        /* 取出系统状态信息数据块 */
        if((usBlockLen == sizeof(SysCMStateBlock_t)) && ((usOffset + usBlockLen) < PM_RECV_BUFF_BODY_SIZE))
        {
            pstCMStateBlock = (SysCMStateBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstCMStateBlock->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstCMStateBlock->uiCrc32;
                pucBuf = (puint8_t) &(pstCMStateBlock->stCMState);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstCMStateBlock->usLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* 将系统状态信息记录到系统资源管理模块中 */
                    usCMState = pstCMStateBlock->stCMState.usCMState[emCMID];
                    SysSetCMState(emCMID, usCMState);
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseDetectMsgAck
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
*              ------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Detect msg ACK... | CRC32 |
*              ------------------------------------------------------
***********************************************************************************************************************
*/
static bool_t CMBusProtocolParseDetectMsgAck (PMRecvBuffer_t *pstRecvBuf)
{
    PMDetectMsgAckBlock_t* pstDetectMsgAckBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bRet = false;

    if(pstRecvBuf != NULL)
    {
        /* 探测包的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stDetectMsgAckBlock.usLen;
        /* 探测包在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stDetectMsgAckBlock.usOffset;

        /* 取出探测包数据块的内容 */
        if((usBlockLen == sizeof(PMDetectMsgAckBlock_t)) && (usOffset +usBlockLen < PM_RECV_BUFF_BODY_SIZE))
        {
            pstDetectMsgAckBlock = (PMDetectMsgAckBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (pstDetectMsgAckBlock->usStartFlag == DATA_BLOCK_START_FLAG)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstDetectMsgAckBlock->uiCrc32;
                pucBuf = (puint8_t) &(pstDetectMsgAckBlock->stDetectMsgAck);

                /* pstDetectMsgAckBlock->usLen 即 sizeof(PMDetectMsgAck_t) */
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstDetectMsgAckBlock->usLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* 判断探测数据内容 */
                    if(pstDetectMsgAckBlock->stDetectMsgAck.uiFlag == DETECT_MSG_ACK_FLAG)
                    {
                        /* 收到预期的应答 */
                        bRet = true;
                    }
                    else
                    {
                        /* 未收到预期的应答 */
                        bRet = false;
                    }
                }
            }
        }
    }

    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParsePackageSerialNum
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
static bool_t CMBusProtocolParsePackageSerialNum (PMRecvBuffer_t *pstRecvBuf, uint16_t usSerialNum)
{
    bool_t bRet = false;

    if(pstRecvBuf != NULL)
    {
        /* 判断接收到的序列号是否与预期序列号一致 */
        if(usSerialNum == pstRecvBuf->stRecvHeader.usPackageSerialNum)
        {
            /* 一致，返回true */
            bRet = true;
        }
        else
        {
            /* 不一致，返回false */
            bRet = false;
        }
    }
    return bRet;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParsePCsoftware
*
* 功能描述: 解析来自PC端软件的请求
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emCMID: CM的ID
*         emMsgType: 消息类型
*
* 输出参数:
*
* 返 回 值  : 无
*
*              -----------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | PC software content... | CRC32 |
*              -----------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParsePCsoftware (PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID, SysMsgType_t emMsgType)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bMsgTypeOK = false;

    if(CS1131_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stCS1131Req.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stCS1131Req.usOffset;
        bMsgTypeOK = true;
    }
    else if(SOE_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stSOEReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stSOEReq.usOffset;
        bMsgTypeOK = true;
    }
    else if(OPC_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stOPCReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stOPCReq.usOffset;
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
    else if(AMS_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stAMSReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stAMSReq.usOffset;
        bMsgTypeOK = true;
    }
    else
    {
        /* 消息类型错误 */
        bMsgTypeOK = false;
    }

    /* 消息类型正确且参数合法*/
    if((true == bMsgTypeOK) && (pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 指向消息起始地址 */
        pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

        /* 取出开始标识 */
        usStartFlag = *((puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET]);
        /* 取出消息长度 */
        usMsgLen = *((puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET]);
        /* 取出消息的CRC32校验码 */
        uiCRC32 = *((puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET+usMsgLen]);

        /* 若消息合法 */
        if((usBlockLen == (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < PM_RECV_BUFF_BODY_SIZE))
        {
            /* 判断数据块起始标识 */
            if (DATA_BLOCK_START_FLAG == usStartFlag)
            {
                /* 判断CRC32是否正确 */
                pucBuf = (puint8_t) &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* 将内容写入系统资源管理模块 */
                    SysSetMsg (emCMID, emMsgType, pucBuf, usMsgLen);
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseP2P
*
* 功能描述: 解析来自外部的P2P的请求/应答
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emCMID: CM的ID
*         emMsgType: 消息类型
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ---------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | P2P content... | CRC32 |
*              ---------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParseP2P(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID, SysP2PMsgType_t emMsgType)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    bool_t bMsgTypeOK = false;

    if(P2P_EXT_REQ == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stP2PReq.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stP2PReq.usOffset;
        bMsgTypeOK = true;
    }
    else if(P2P_EXT_RESP == emMsgType)
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stP2PResp.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stP2PResp.usOffset;
        bMsgTypeOK = true;
    }
    else
    {
        /* 消息类型错误 */
        bMsgTypeOK = false;
    }

    /* 消息类型正确且参数合法*/
    if((true == bMsgTypeOK) && (pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 指向消息起始地址 */
        pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

        /* 取出开始标识 */
        usStartFlag = *((puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET]);
        /* 取出消息长度 */
        usMsgLen = *((puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET]);
        /* 取出消息的CRC32校验码 */
        uiCRC32 = *((puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET+usMsgLen]);

        /* 若消息合法 */
        if((usBlockLen == (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < PM_RECV_BUFF_BODY_SIZE))
        {
            /* 判断数据块起始标识 */
            if (DATA_BLOCK_START_FLAG == usStartFlag)
            {
                /* 判断CRC32是否正确 */
                pucBuf = (puint8_t) &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* 将内容写入系统资源管理模块 */
                    SysSetP2PMsg (emCMID, emMsgType, pucBuf, usMsgLen);
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseModbus
*
* 功能描述: 解析来自外部的 Modbus数据
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*         emCMID: CM的ID
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ----------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Modbus Msg content... | CRC32 |
*              ----------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParseModbus(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID)
{
    uint16_t usStartFlag = DATA_BLOCK_INVALID_START_FLAG;
    uint16_t usMsgLen = 0;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;

    if((pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stModbus.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stModbus.usOffset;

        /* 指向消息起始地址 */
        pucBuf = (puint8_t) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

        /* 取出开始标识 */
        usStartFlag = *((puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET]);
        /* 取出消息长度 */
        usMsgLen = *((puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET]);
        /* 取出消息的CRC32校验码 */
        uiCRC32 = *((puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET+usMsgLen]);

        /* 若消息合法 */
        if((usBlockLen == (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE)) && ((usOffset + usBlockLen) < PM_RECV_BUFF_BODY_SIZE))
        {
            /* 判断数据块起始标识 */
            if (DATA_BLOCK_START_FLAG == usStartFlag)
            {
                /* 判断CRC32是否正确 */
                pucBuf = (puint8_t) &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, usMsgLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* 将内容写入系统资源管理模块 */
                    SysSetModbusMsgQueue(emCMID, pucBuf, usMsgLen);
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseConfigReq
*
* 功能描述: 解析来自CM模块的CM配置信息请求命令
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emCMID: CM的ID
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ---------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | stConfigReq | CRC32 |
*              ---------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParseConfigReq(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID)
{
    PMConfigReqBlock_t* pstConfigReqBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;

    if((pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 应答的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stConfigReqBlock.usLen;
        /* 应答在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stConfigReqBlock.usOffset;

        /* 取出应答的内容 */
        if((usBlockLen == sizeof(PMConfigReqBlock_t)) && ((usOffset + usBlockLen) < PM_RECV_BUFF_BODY_SIZE))
        {
            pstConfigReqBlock = (PMConfigReqBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (DATA_BLOCK_START_FLAG == pstConfigReqBlock->usStartFlag)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstConfigReqBlock->uiCrc32;
                pucBuf = (uint8_t *) &(pstConfigReqBlock->stConfigReq);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstConfigReqBlock->usLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    /* CM的配置信息请求命令 */
                    s_uiCfgReqCmd[emCMID] = pstConfigReqBlock->stConfigReq.uiRequest;
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolParseRTDataAck
*
* 功能描述: 解析来自CM模块的对实时数据的应答
*
* 输入参数: pstRecvBuf: 指向接收数据缓冲区
*        emCMID: CM的ID
*
* 输出参数:
*
* 返 回 值  : 无
*
*              ----------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Real time data ACK... | CRC32 |
*              ----------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolParseRTDataAck(PMRecvBuffer_t *pstRecvBuf, CMController_t emCMID)
{
    RTDataAckBlock_t* pstRTDataAckBlock = NULL;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;

    if((pstRecvBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 探测包的数据长度 */
        usBlockLen = pstRecvBuf->stRecvHeader.stRealTimeDataAck.usLen;
        /* 探测包在信息区的偏移地址 */
        usOffset = pstRecvBuf->stRecvHeader.stRealTimeDataAck.usOffset;

        /* 取出探测包数据块的内容 */
        if((usBlockLen == sizeof(RTDataAckBlock_t)) && (usOffset +usBlockLen < PM_RECV_BUFF_BODY_SIZE))
        {
            pstRTDataAckBlock = (RTDataAckBlock_t*) &(pstRecvBuf->stRecvBody.ucDataBuffer[usOffset]);

            /* 判断数据块起始标识 */
            if (DATA_BLOCK_START_FLAG == pstRTDataAckBlock->usStartFlag)
            {
                /* 判断CRC32是否正确 */
                uiCRC32 = pstRTDataAckBlock->uiCRC32;
                pucBuf = (puint8_t) &(pstRTDataAckBlock->stRTDataAck);
                uiCRC32calc = SysCrc32Cal(0, pucBuf, pstRTDataAckBlock->usLen);

                if(uiCRC32 == uiCRC32calc)
                {
                    if(RTDATA_MSG_NAK == pstRTDataAckBlock->stRTDataAck.usAck)
                    {
                        /* CM接收实时数据失败 */
                        s_uiRTDataSerialNum[emCMID] = 0;

                        s_emDataArea[emCMID] = 0;

                        /* PM重新发送实时数据 */
                        SysReadRTDataReset();
                        printf("Rt data reset\n");
                    }
                }
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolSend
*
* 功能描述: 发送数据处理: PM向指定的CM发送数据
*
* 输入参数: pstSendBuf: 指向待发送数据缓冲区
*        emCMID: CM的ID号
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
static void CMBusProtocolSend(PMSendBuffer_t *pstSendBuf, CMSlot_t emCMSlot)
{
    uint32_t uiCRC32 = 0;
    int32_t iSendLen = 0;
    CMController_t emCMID;

//    int i = 0;//test
//    uint8_t *pBuf = NULL;
    /* 发送缓冲区已使用空间及可使用偏移地址均清零  */
    s_stCommSend.usUsed = 0;
    s_stCommSend.usOffset = 0;
    /* 发送缓冲区清零 */
    memset((puint8_t)&(pstSendBuf->stSendHeader.usCMAddr), 0, sizeof(PMSendHeader_t));

    /* 打包CM的地址  */
    pstSendBuf->stSendHeader.usCMAddr = (uint16_t) emCMSlot;

    /* 打包发送数据包序列号 */
    pstSendBuf->stSendHeader.usPackageSerialNum = s_usSdDetectSn[emCMSlot];

    /* 打包系统状态信息  */
    CMBusProtocolPackSysState(pstSendBuf);

    /* 发送探测消息 */
    CMBusProtocolPackDetectMsg(pstSendBuf, emCMSlot, DETECT_MSG_FLAG);

    /* 打包组态软件应答  */
    emCMID = CMSLotIDToLogicID(emCMSlot);
//    printf("CMID=%d\n",emCMID);

    if((emCMID >= CM1) && (emCMID <= CM4))
    {
        CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, CS1131_RESP);

        /* 下装成功后发送实时数据，若无工程则不发送实时数据 */
        CMBusProtocolPackRTData (pstSendBuf, emCMID);
    }
    
    /* 设置数据内容长度  */
    pstSendBuf->stSendHeader.usDataLen = s_stCommSend.usUsed;

    /* 计算并填充头部CRC32 */
    uiCRC32 = SysCrc32Cal(0, (puint8_t)&(pstSendBuf->stSendHeader.usCMAddr),sizeof(PMSendHeader_t) - CRC32_SIZE);
    pstSendBuf->stSendHeader.uiCRC32 = uiCRC32;

    /* 调用底层驱动将数据发送出去 */
    iSendLen = s_stCommSend.usUsed+ sizeof(PMSendHeader_t);

    if (cmbus_send(emCMSlot,pstSendBuf,iSendLen,0x1000)<0)
    {
        //printf("cmbus send fail\n");
    }
    else
    {
        //printf("cmbus send ok %x\n",pstSendBuf->stSendHeader.usCMAddr);
    }
    //printf("pm send sn=%d\n",pstSendBuf->stSendHeader.usPackageSerialNum);

#if 0
    printf("send-----");
    pBuf = (puint8_t)pstSendBuf;
    for(i=0;i<sizeof(PMSendHeader_t);i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
    
    
    pBuf = (puint8_t)pstSendBuf;
    for(i=sizeof(PMSendHeader_t);i<sizeof(PMSendHeader_t)+pstSendBuf->stSendHeader.usDataLen;i++)
    {
        printf("%2x  ", pBuf[i]);
    }
    printf("\n");
    
#endif
    return;
}
#if 0
static void CMBusProtocolSend(PMSendBuffer_t *pstSendBuf, CMController_t emCMID)
{
    uint32_t uiCRC32 = 0;
    CMSlot_t emCMSlot = INVALID_CM_SLOT;
    int32_t iSendLen = 0;
    int16_t sRetLen = 0;
    int16_t sRetEn = 0;
    int16_t sSendState = 0;

    /* 发送缓冲区已使用空间及可使用偏移地址均清零  */
    s_stCommSend.usUsed = 0;
    s_stCommSend.usOffset = 0;
    /* 发送缓冲区清零 */
    memset((puint8_t)&(pstSendBuf->stSendHeader.usCMAddr), 0, sizeof(PMSendHeader_t));

    /* 打包CM的地址  */
    emCMSlot = SysCMLogicIDToSLotID(emCMID);
    pstSendBuf->stSendHeader.usCMAddr = (uint16_t) emCMSlot;

    /* 打包发送数据包序列号 */
    pstSendBuf->stSendHeader.usPackageSerialNum = s_usSendPackageSerialNum[emCMID];

    /* 打包系统状态信息  */
    CMBusProtocolPackSysState(pstSendBuf);

    /* 根据当前情况决定是否发送探测消息  */
    if(true == s_bSendDetectMsgFlag[emCMID])
    {
        /* 发送探测消息 */
        CMBusProtocolPackDetectMsg(pstSendBuf, emCMID);
    }

    /* 打包SOE应答  */
    CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, SOE_RESP);

    /* 打包组态软件应答  */
    CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, CS1131_RESP);

    /* 打包客户端应答 */
    CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, CLIENT_RESP);

    /* 打包客户端请求 */
    CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, CLIENT_REQ);

    /* 打包AMS应答  */
    CMBusProtocolPackPCsoftware(pstSendBuf, emCMID, AMS_RESP);

    /* 打包配置信息  */
    /* 处理来自CM的命令 */
    if(CONFIG_INFO_START == s_uiCfgReqCmd[emCMID])
    {
        /* 收到开始命令，置传输开始标志 */
        s_uiCfgTransFlag[emCMID] = CONFIG_INFO_START;

        /* 打包配置表 */
        //CMBusProtocolPackCfgFileTable(pstSendBuf, emCMID);
    }
    else if(CONFIG_INFO_CONTINUE == s_uiCfgReqCmd[emCMID])
    {
        /* 收到继续命令，置传输继续标志 */
        s_uiCfgTransFlag[emCMID] = CONFIG_INFO_CONTINUE;
    }
    else if(CONFIG_INFO_RETRANSFER == s_uiCfgReqCmd[emCMID])
    {
        /* 收到重传命令，做重传配置文件的准备 */
        s_uiCfgTransFlag[emCMID] = CONFIG_INFO_RETRANSFER;
        RetransCfgFile(emCMID);
    }
    else if(TRANSFER_CONFIG_INFO_FINISHED == s_uiCfgReqCmd[emCMID])
    {
        /* 收到结束命令，置传输结束标志 */
        s_uiCfgTransFlag[emCMID] = TRANSFER_CONFIG_INFO_FINISHED;
        TransCfgFileInit(emCMID);
    }
    if((TRANSFER_CONFIG_INFO_FINISHED != s_uiCfgTransFlag[emCMID])
        && (CONFIG_INFO_NO_REQ != s_uiCfgTransFlag[emCMID]))
    {
        /* 对开始、继续、重传命令的响应 */
        CMBusProtocolPackCfgFile(pstSendBuf, emCMID);
    }

    /* 打包配置表 */
    CMBusProtocolPackCfgFileTable(pstSendBuf, emCMID);

    /* 打包P2P数据 */
    CMBusProtocolPackP2P (pstSendBuf, emCMID, P2P_PM_REQ);
    CMBusProtocolPackP2P (pstSendBuf, emCMID, P2P_PM_RESP);

    /* 打包实时数据  */
    CMBusProtocolPackRTData (pstSendBuf, emCMID);

    /* 设置数据内容长度  */
    pstSendBuf->stSendHeader.usDataLen = s_stCommSend.usUsed;

    /* 计算并填充头部CRC32 */
    uiCRC32 = SysCrc32Cal(0, (puint8_t)&(pstSendBuf->stSendHeader.usCMAddr),sizeof(PMSendHeader_t) - CRC32_SIZE);
    pstSendBuf->stSendHeader.uiCRC32 = uiCRC32;

    /* 更新发送包序列号 */
    s_usSendPackageSerialNum[emCMID] ++;

    /* 调用底层驱动将数据发送出去 */
    iSendLen = s_stCommSend.usUsed+ sizeof(PMSendHeader_t);

    if (cmbus_send(emCMSlot+3,pstSendBuf,iSendLen,0x20000)<0)
    {
        printf("cmbus send fail\n");
    }
    else
    {
        printf("cmbus send ok %x\n",pstSendBuf->stSendHeader.usCMAddr);
    }
    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackSysState
*
* 功能描述: 打包系统的状态信息
*
* 输入参数: 无
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*
*              ----------------------------------------------
* 注意事项:  格式: | usStartFlag | usLen | Sys state... | CRC32 |
*              ----------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackSysState (PMSendBuffer_t *pstSendBuf)
{
    SysStateBlock_t stSysStateBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;

    if(pstSendBuf != NULL)
    {
        /* 状态信息数据块 */
        stSysStateBlock.usStartFlag = DATA_BLOCK_START_FLAG;
        stSysStateBlock.usLen = sizeof(SysState_t);
        SysGetSysState(&stSysStateBlock.stSysState);

        pucBuf = (uint8_t *) &(stSysStateBlock.stSysState);
        uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(SysState_t));
        stSysStateBlock.uiCrc32 = uiCRC32;

        /* 计算CM的状态信息长度及偏移地址 */
        usBlockLen = sizeof(SysStateBlock_t);
        usOffset = s_stCommSend.usOffset;

        /* 若不超出发送缓冲区地址，则发送系统的状态信息 */
        if((usOffset + usBlockLen) < PM_SEND_BUFF_BODY_SIZE)
        {
            /* 将系统状态信息写入发送缓冲区 */
            memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stSysStateBlock, usBlockLen);
            /* 更新发送缓冲区头部 */
            pstSendBuf->stSendHeader.stSysStateBlock.usLen = usBlockLen;
            pstSendBuf->stSendHeader.stSysStateBlock.usOffset = usOffset;
            /* 更新已使用的空间，及可使用空间的偏移地址 */
            s_stCommSend.usUsed += usBlockLen;
            s_stCommSend.usOffset += usBlockLen;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackDetectMsg
*
* 功能描述: 打包探测消息，将其发送到指定的CM
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*
*              -----------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | content... | CRC32 |
*              -----------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackDetectMsg (PMSendBuffer_t *pstSendBuf, CMSlot_t emCMSlot, uint32_t uiMsg)
{
    PMDetectMsgBlock_t stDetectMsgBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;

    if((pstSendBuf != NULL) && (emCMSlot < NUM_OF_SLOT))
    {
        /* 生成探测消息数据块*/
        /* 开始标识 */
        stDetectMsgBlock.usStartFlag = DATA_BLOCK_START_FLAG;
        /* 探测消息内容长度 */
        stDetectMsgBlock.usLen = sizeof(PMDetectMsg_t);
        /* 探测消息内容 */
        stDetectMsgBlock.stDetectMsg.uiFlag = uiMsg;//DETECT_MSG_FLAG;

        /* 针对探测消息的内容，计算crc32 */
        pucBuf = (uint8_t *) &(stDetectMsgBlock.stDetectMsg);
        uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(PMDetectMsg_t));
        stDetectMsgBlock.uiCrc32 = uiCRC32;

        /* 计算探测信息长度及偏移地址 */
        usBlockLen = sizeof(stDetectMsgBlock);
        usOffset = s_stCommSend.usOffset;

        /* 判断是否超出发送缓冲区，若不超范围，则把探测消息发送出去 */
        if((usOffset + usBlockLen) < PM_SEND_BUFF_BODY_SIZE)
        {
            /* 将探测消息写入发送缓冲区 */
            memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stDetectMsgBlock, usBlockLen);
            /* 更新发送缓冲区头部 */
            pstSendBuf->stSendHeader.stDetectMsgBlock.usLen = usBlockLen;
            pstSendBuf->stSendHeader.stDetectMsgBlock.usOffset = usOffset;
            /* 更新当前已使用的空间，及当前可用空间的偏移地址 */
            s_stCommSend.usUsed += usBlockLen;
            s_stCommSend.usOffset += usBlockLen;
        }
    }
    return;
}
#if 0
static void CMBusProtocolPackDetectMsg (PMSendBuffer_t *pstSendBuf, CMController_t emCMID)
{
    uint16_t usInterCommState = CMBUS_COMM_NORMAL;
    PMDetectMsgBlock_t stDetectMsgBlock;
    uint16_t usBlockLen = 0;
    uint16_t usOffset = 0;
    uint32_t uiCRC32 = 0;
    puint8_t pucBuf = NULL;

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        /* 获取本PM与指定CM的内部通讯状态 */
        usInterCommState = SysGetLocalCMBusCommState(emCMID);

        /* 需要发送探测消息 */
        if(CMBUS_COMM_DETECT_MSG == usInterCommState)
        {
            /* 生成探测消息数据块*/
            /* 开始标识 */
            stDetectMsgBlock.usStartFlag = DATA_BLOCK_START_FLAG;
            /* 探测消息内容长度 */
            stDetectMsgBlock.usLen = sizeof(PMDetectMsg_t);
            /* 探测消息内容 */
            stDetectMsgBlock.stDetectMsg.uiFlag = DETECT_MSG_FLAG;

            /* 针对探测消息的内容，计算crc32 */
            pucBuf = (uint8_t *) &(stDetectMsgBlock.stDetectMsg);
            uiCRC32 = SysCrc32Cal(0, pucBuf, sizeof(PMDetectMsg_t));
            stDetectMsgBlock.uiCrc32 = uiCRC32;

            /* 计算探测信息长度及偏移地址 */
            usBlockLen = sizeof(stDetectMsgBlock);
            usOffset = s_stCommSend.usOffset;

            /* 判断是否超出发送缓冲区，若不超范围，则把探测消息发送出去 */
            if((usOffset + usBlockLen) < PM_SEND_BUFF_BODY_SIZE)
            {
                /* 将探测消息写入发送缓冲区 */
                memcpy((puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]), (puint8_t)&stDetectMsgBlock, usBlockLen);
                /* 更新发送缓冲区头部 */
                pstSendBuf->stSendHeader.stDetectMsgBlock.usLen = usBlockLen;
                pstSendBuf->stSendHeader.stDetectMsgBlock.usOffset = usOffset;
                /* 更新当前已使用的空间，及当前可用空间的偏移地址 */
                s_stCommSend.usUsed += usBlockLen;
                s_stCommSend.usOffset += usBlockLen;
            }
        }
    }
    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackPCsoftware
*
* 功能描述: 打包对PC端软件的应答信息
*
* 输入参数: emCMID: CM的ID号
*        emMsgType: 应答消息类型
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*              -----------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | content... | CRC32 |
*              -----------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackPCsoftware (PMSendBuffer_t *pstSendBuf, CMController_t emCMID, SysMsgType_t emMsgType)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM) && (emMsgType < NUM_OF_MSGTYPE))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetMsgLen(emCMID, emMsgType, &usMsgLen);

        /* 判断是否有来自PC端的应答，并且判断是否有剩余空间来存放应答内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen) < PM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetMsg (emCMID, emMsgType, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            if(CS1131_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stCS1131Resp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stCS1131Resp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(SOE_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stSOEResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stSOEResp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(OPC_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stOPCResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stOPCResp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(CLIENT_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stClientResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stClientResp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(CLIENT_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stClientReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stClientReq.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(AMS_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stAMSResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stAMSResp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }

        }
        else
        {
            /* 头部对应项填0表明不存在该项消息内容 */
            if(CS1131_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stCS1131Resp.usLen = 0;
            }
            else if(SOE_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stSOEResp.usLen = 0;
            }
            else if(OPC_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stOPCResp.usLen = 0;
            }
            else if(CLIENT_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stClientResp.usLen = 0;
            }
            else if(CLIENT_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stClientReq.usLen = 0;
            }
            else if(AMS_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stAMSResp.usLen = 0;
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackP2P
*
* 功能描述: 打包P2P数据发送到外部
*
* 输入参数: emCMID: CM的ID号
*        emMsgType: 消息类型
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*              ---------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | P2P content... | CRC32 |
*              ---------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackP2P (PMSendBuffer_t *pstSendBuf, CMController_t emCMID, SysP2PMsgType_t emMsgType)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM) && (emMsgType < NUM_OF_P2PMSGTYPE))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取消息长度 */
        SysGetP2PMsgLen (emCMID, emMsgType, &usMsgLen);

        /* 判断是否有来自PC端的应答，并且判断是否有剩余空间来存放应答内容 */
        if((usMsgLen > 0) && ((usOffset + usMsgLen) < PM_SEND_BUFF_BODY_SIZE))
        {
            /* 填开始标志 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
            *pusBuf = DATA_BLOCK_START_FLAG;

            /* 填消息长度 */
            pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
            *pusBuf = usMsgLen;

            /* 填消息内容 */
            SysGetP2PMsg (emCMID, emMsgType, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), &usMsgLen);

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
            puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            if(P2P_PM_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PReq.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stP2PReq.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else if(P2P_PM_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PResp.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stP2PResp.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }

        }
        else
        {
            /* 头部对应项填0表明不存在该项消息内容 */
            if(P2P_PM_REQ == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PReq.usLen = 0;
            }
            else if(P2P_PM_RESP == emMsgType)
            {
                pstSendBuf->stSendHeader.stP2PResp.usLen = 0;
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: TransCfgFileInit
*
* 功能描述: 传输配置文件初始化
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void TransCfgFileInit(CMController_t emCMID)
{
    if(emCMID < NUM_OF_CM)
    {
        s_usCfgFileIndex[emCMID] = 0;
        s_uiRdOffset[emCMID] = 0;
        s_uiRdTotalNum[emCMID] = 0;
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: RetransCfgFile
*
* 功能描述: 重传输配置文件
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void RetransCfgFile(CMController_t emCMID)
{
    if(emCMID < NUM_OF_CM)
    {
        /* 退回到上一个文件重新传输 */
        if(s_usCfgFileIndex[emCMID] > 0)
        {
            s_usCfgFileIndex[emCMID]--;
        }
        s_uiRdOffset[emCMID] = 0;
        s_uiRdTotalNum[emCMID] = 0;
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackCfgFileTable
*
* 功能描述: 打包传输需要更新的配置文件列表
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*              ---------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usMsgLen | Cfg table content... | CRC32 |
*              ---------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackCfgFileTable(PMSendBuffer_t *pstSendBuf, CMController_t emCMID)
{
    uint16_t usOffset = 0;
    uint16_t usMsgLen = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;
    SysCfgFileTable_t stCfgFileTable;
    bool_t bFileTableValid = false;

    /* 准备传输配置文件 */
    TransCfgFileInit(emCMID);

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 获取配置表 */
        bFileTableValid = SysGetCfgFileTable(&stCfgFileTable);

        /* 获取配置表成功 */
        if(true == bFileTableValid)
        {
            usMsgLen = sizeof(CfgFileTableBlock_t);

            /* 判断是否有来自PC端的应答，并且判断是否有剩余空间来存放应答内容 */
            if((usOffset + usMsgLen) < PM_SEND_BUFF_BODY_SIZE)
            {
                /* 填开始标志 */
                pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_START_FLAG_OFFSET];
                *pusBuf = DATA_BLOCK_START_FLAG;

                /* 填消息长度 */
                pusBuf = (puint16_t)&pucBuf[DATA_BLOCK_MSG_LEN_OFFSET];
                *pusBuf = usMsgLen;

                /* 填消息内容 */
                memcpy(&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET], (puint8_t)&stCfgFileTable, usMsgLen);

                /* 填CRC32 */
                uiCRC32calc = SysCrc32Cal(0, &(pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET]), usMsgLen);
                puiBuf = (puint32_t)&pucBuf[DATA_BLOCK_MSG_CONTENT_OFFSET + usMsgLen];
                *puiBuf = uiCRC32calc;

                /* 填发送包头部信息： 数据块长度及偏移 */
                pstSendBuf->stSendHeader.stCfgTableBlock.usLen = usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE;
                pstSendBuf->stSendHeader.stCfgTableBlock.usOffset = usOffset;
                s_stCommSend.usUsed += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
                s_stCommSend.usOffset += (usMsgLen + DATA_BLOCK_HEADER_TAIL_SIZE);
            }
            else
            {
                /* 头部对应项填0表明不存在该项消息内容 */
                pstSendBuf->stSendHeader.stCfgTableBlock.usLen = 0;
            }
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackCfgFile
*
* 功能描述: 打包CM的相关配置信息
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*
*              ---------------------------------------------------------------------------------------------
* 注意事项: 格式: | usStartFlag | usCfgFileType | uiOffset | usLen | usLastPackFlag | Content array... | CRC32 |
*             ----------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackCfgFile(PMSendBuffer_t *pstSendBuf, CMController_t emCMID)
{
    uint16_t usOffset = 0;
    uint32_t uiCRC32calc = 0;
    puint8_t pucBuf = NULL;
    puint16_t pusBuf = NULL;
    puint32_t puiBuf = NULL;
    uint16_t usRdMaxLen = 0;
    uint16_t usRdLen = 0;
    bool_t bTransFlag = false;
    SysCfgFileState_t stCfgFileState;

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM))
    {
        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        /* 检查待发送到CM的配置文件的索引号是否需要发送到相应的CM */
        bTransFlag = SysCheckCfgFileTransFlag(emCMID, (SysCfgFileType_t)s_usCfgFileIndex[emCMID]);

        if(true == bTransFlag)
        {
            SysGetCfgFileState ((SysCfgFileType_t) s_usCfgFileIndex[emCMID], &stCfgFileState);

            /* 用于存放配置文件内容的空间 */
            usRdMaxLen = (uint16_t)PM_SEND_BUFF_BODY_SIZE - usOffset - (uint16_t)CFG_FILE_CONTENT_OFFSET;

            /* 有部分剩余空间用于传输配置文件 */
            if((usRdMaxLen > CFG_FILE_HEADER_TAIL_LEN) && (CFG_FILE_RD_ENABLE == stCfgFileState.bReadable) \
                    && (stCfgFileState.uiLen > 0) && (CFG_FILE_WR_FINISH == stCfgFileState.bFileStatus))
            {
                /* 填开始标志 */
                pusBuf = (puint16_t)&pucBuf[CFG_FILE_START_FLAG_OFFSET];
                *pusBuf = DATA_BLOCK_START_FLAG;

                /* 配置文件索引号 */
                pusBuf = (puint16_t)&pucBuf[CFG_FILE_TYPE_OFFSET];
                *pusBuf = s_usCfgFileIndex[emCMID];

                /* 存放偏移地址 */
                puiBuf = (puint32_t)&pucBuf[CFG_FILE_OFFSET_ADDR_OFFSET];
                *puiBuf = s_uiRdOffset[emCMID];

                /* 读取消息内容 */
                usRdLen = SysGetCfgFileInfo ((SysCfgFileType_t) s_usCfgFileIndex[emCMID], s_uiRdOffset[emCMID], \
                                  &pucBuf[CFG_FILE_CONTENT_OFFSET], usRdMaxLen);
                s_uiRdOffset[emCMID] += (uint32_t)usRdLen;
                s_uiRdTotalNum[emCMID] += (uint32_t)usRdLen;

                /* 填消息长度 */
                pusBuf = (puint16_t)&pucBuf[CFG_FILE_CONTENT_LEN_OFFSET];
                *pusBuf = usRdLen;

                /* 传输是否为最后一包的标志 */
                pusBuf = (puint16_t)&pucBuf[CFG_FILE_LAST_PACKAGE_OFFSET];
                if(s_uiRdTotalNum[emCMID] >= stCfgFileState.uiLen )
                {
                    /* 该文件最后一包标志 */
                    *pusBuf = (uint16_t)CFG_FILE_LAST_PACKAGE;

                    /* 为传输下一个配置文件做准备 */
                    s_uiRdOffset[emCMID] = 0;
                    s_uiRdTotalNum[emCMID] = 0;
                    s_usCfgFileIndex[emCMID]++;
                }
                else
                {
                    /* 非最后一包标志 */
                    *pusBuf = (uint16_t)CFG_FILE_NOT_LAST_PACKAGE;
                }

                /* 填CRC32 */
                uiCRC32calc = SysCrc32Cal(0, &(pucBuf[CFG_FILE_START_FLAG_OFFSET]), usRdLen + (uint16_t)CFG_FILE_HEADER_LEN);
                puiBuf = (puint32_t)&pucBuf[(uint16_t)CFG_FILE_CONTENT_OFFSET + usRdLen];
                *puiBuf = uiCRC32calc;

                /* 填发送包头部信息： 数据块长度及偏移 */
                pstSendBuf->stSendHeader.stConfigRespBlock.usLen = usRdLen + (uint16_t)CFG_FILE_HEADER_TAIL_LEN;
                pstSendBuf->stSendHeader.stConfigRespBlock.usOffset = usOffset;
                s_stCommSend.usUsed += (usRdLen + (uint16_t)CFG_FILE_HEADER_TAIL_LEN);
                s_stCommSend.usOffset += (usRdLen + (uint16_t)CFG_FILE_HEADER_TAIL_LEN);
            }
            else
            {
                /* 头部对应项填0表明不存在该项消息内容 */
                pstSendBuf->stSendHeader.stConfigRespBlock.usLen = 0;
            }
        }
        else
        {
            if(s_usCfgFileIndex[emCMID] < (uint16_t)CM_CFG_FILE_NUM)
            {
                /* 为传输下一个配置文件做准备 */
                s_uiRdOffset[emCMID] = 0;
                s_uiRdTotalNum[emCMID] = 0;
                s_usCfgFileIndex[emCMID]++;
            }
            else
            {
                /* 所有配置文件传输结束 */
                s_uiCfgTransFlag[emCMID] = (uint32_t)TRANSFER_CONFIG_INFO_FINISHED;
            }

            pstSendBuf->stSendHeader.stConfigRespBlock.usLen = 0;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CMBusProtocolPackRTData
*
* 功能描述: 打包发送实时数据
*
* 输入参数: emCMID: CM的ID号
*
* 输出参数: pstSendBuf: 指向待发送数据缓冲区
*
* 返 回 值  : 无
*
*              --------------------------------------------------------------------------------------------------------------------
* 注意事项:  格式: | usStartFlag | usServiceCode | uiSerialNum | uiOffset | usTransferFlag | usLen |Rt data type| RT data ... | CRC32 |
*              --------------------------------------------------------------------------------------------------------------------
***********************************************************************************************************************
*/
static void CMBusProtocolPackRTData (PMSendBuffer_t *pstSendBuf, CMController_t emCMID)
{
    uint16_t usRest = 0;                        /* 发送缓冲区剩余空间 */
    uint16_t usOffset = 0;                      /* 发送缓冲区待填充的偏移地址 */
    uint16_t usMsgLen = 0;                      /* 实时数据长度 */
    uint16_t usRdMsgLen = 0;                    /* 实际读取的数据长度 */
    uint32_t uiCRC32calc = 0;                   /* 对实时数据的校验值 */
    puint8_t pucBuf = NULL;                     /* 指向发送缓冲区数据区的某个位置 */
    puint8_t pucBufContent = NULL;              /* 指向发送缓冲区数据区的某个位置 */
    puint16_t pusBuf = NULL;                    /* 指向发送缓冲区数据区的某个位置 */
    puint32_t puiBuf = NULL;                    /* 指向发送缓冲区数据区的某个位置 */
    uint32_t uiReadOffset = 0;                  /* 正在读取内容所在数据区的偏移地址 */
    bool_t bReadFlag;                           /* 相应数据区是否读取完毕 */
    SysRtDataAreaType_t emDataArea = RTDATA_UP1_G_AREA;

    if((pstSendBuf != NULL) && (emCMID < NUM_OF_CM))
    {

        /* 判断发送缓冲区剩余空间 */
        usRest = PM_SEND_BUFF_BODY_SIZE - s_stCommSend.usUsed;

        usOffset = s_stCommSend.usOffset;
        pucBuf = (puint8_t)&(pstSendBuf->stSendBody.ucDataBuffer[usOffset]);

        if(usRest > MAX_RTDATA_PACKAGE_SIZE)
        {
            usMsgLen = MAX_RTDATA_PACKAGE_SIZE - RTDATA_HEADER_TAIL_SIZE;
        }
        else
        {
            usMsgLen = usRest - RTDATA_HEADER_TAIL_SIZE;
        }

        /* 生成实时数据数据包，填开始标志 */
        pusBuf = (puint16_t)&pucBuf[RTDATA_START_FLAG_OFFSET];
        *pusBuf = DATA_BLOCK_START_FLAG;

        /* 服务码 */
        pusBuf = (puint16_t)&pucBuf[RTDATA_SRV_CODE_OFFSET];
        *pusBuf = SRV_CODE_RTDATA;

        /* 数据包序列号 */
        puiBuf = (puint32_t)&pucBuf[RTDATA_SERIAL_NUM_OFFSET];
        *puiBuf = s_uiRTDataSerialNum[emCMID];

        /* 实时数据偏移地址 */
        emDataArea = s_emDataArea[emCMID];
        SysGetRTDataReadOffset(emDataArea, &uiReadOffset);
        puiBuf = (puint32_t)&pucBuf[RTDATA_CONTENT_OFFSET_OFFSET];
        *puiBuf = uiReadOffset;

        /* 填消息长度 */
        pusBuf = (puint16_t)&pucBuf[RTDATA_LEN_OFFSET];
        *pusBuf = usMsgLen;

        /* 填实时数据类型G/I/Q */
        pusBuf = (puint16_t)&pucBuf[RTDATA_AREA_TYPE_OFFSET];
        *pusBuf = (uint16_t)emDataArea;

        /* 读取实时数据，并将其放入发送缓冲区 */
        pucBufContent = (puint8_t)&pucBuf[RTDATA_CONTENT_OFFSET];
        SysReadRTData(emDataArea, pucBufContent, usMsgLen, &usRdMsgLen);

        /* 获取到实时数据 */
        if(usRdMsgLen > 0)
        {
            /* 重新填写消息长度 */
            pusBuf = (puint16_t)&pucBuf[RTDATA_LEN_OFFSET];
            *pusBuf = usRdMsgLen;

            /* 判断传输完成与否 */
            SysGetRTDataReadFlag(emDataArea, &bReadFlag);
            if(RTDATA_AREA_READ_NOT_FINISHED == bReadFlag)
            {
                pusBuf = (puint16_t)&pucBuf[RTDATA_TRANSFER_FLAG_OFFSET];
                *pusBuf = RTDATA_MSG_TRANS_CONTINUE_FLAG;
            }
            else
            {
                pusBuf = (puint16_t)&pucBuf[RTDATA_TRANSFER_FLAG_OFFSET];
                *pusBuf = RTDATA_MSG_TRANS_FINISH_FLAG;
                s_emDataArea[emCMID]++;
                s_emDataArea[emCMID] = s_emDataArea[emCMID] % MAX_RTDATA_AREA_NUM;
            }

            /* 填CRC32 */
            uiCRC32calc = SysCrc32Cal(0, &(pucBuf[RTDATA_CONTENT_OFFSET]), usRdMsgLen);
            puiBuf = (puint32_t)&pucBuf[RTDATA_CONTENT_OFFSET + usRdMsgLen];
            *puiBuf = uiCRC32calc;

            /* 填发送包头部信息： 数据块长度及偏移 */
            pstSendBuf->stSendHeader.stRealTimeData.usLen = usRdMsgLen + RTDATA_HEADER_TAIL_SIZE;
            pstSendBuf->stSendHeader.stRealTimeData.usOffset = usOffset;
            s_stCommSend.usUsed += (usRdMsgLen + RTDATA_HEADER_TAIL_SIZE);
            s_stCommSend.usOffset += (usRdMsgLen + RTDATA_HEADER_TAIL_SIZE);

            /* 发送数据包序列号加一 */
            s_uiRTDataSerialNum[emCMID]++;
        }
        else
        {
            s_emDataArea[emCMID]++;
            s_emDataArea[emCMID] = s_emDataArea[emCMID] % MAX_RTDATA_AREA_NUM;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: cmbus_send
*
* 功能描述: 发送数据
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int16_t cmbus_send(int32_t station, int8_t *buf, int32_t len,int32_t timeout)
{
    int16_t res = -1;

    if(copy_to_cmbuf(buf,len) < 0)
    {
        return -1;
    }

    if ( enable_cm_send(station) < 0)
    {
        return -1;
    }

    while(1)
    {
        res = check_cm_send();
        if((res >= 0) || (timeout-- == 0))
        {
            break;
        }
    }

    //printf("send state=%d timeout=%x\n", res, timeout);

    return res;
}

/*
***********************************************************************************************************************
* 函数名称: cmbus_send_state
*
* 功能描述: 发送数据
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int16_t cmbus_send_state(int32_t timeout)
{
    int16_t res = -1;

    while(1)
    {
        res = check_cm_send();

        timeout--;

        if((res >= 0) || (0 == timeout))
        {
            break;
        }
    }

    return res;
}

/*
***********************************************************************************************************************
* 函数名称: cmbus_recv
*
* 功能描述: 接收数据
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int16_t cmbus_recv(int8_t *buf, int32_t timeout)
{
    int16_t res = -1;
    int16_t isLen = -1;

    while(1)
    {
        res = check_cm_recv();

        timeout--;

        if((1 == res) || (0 == timeout))
        {
            break;
        }
    }

    if (1 == res)
    {
        isLen = copy_from_cmbuf(buf);
    }
    else
    {
        isLen = -1;
    }
    return isLen;
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
