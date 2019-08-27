/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : 软件应用模块
*
*  文件名称    : FileUpTask.h
*
*  功能简介    : 处理CM固件更新 和 源文件更新
*
*  作者        : 王东
*
*  创建日期    : 2017-03-18
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../inc/FileUpTask.h"
#include "Log/inc/log.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/* File descriptors */
/* RTS */
static int32_t s_iRTSInfoFD;     /* File descriptor of "RTSInfo" file */
static int32_t s_iRTS1FD;        /* File descriptor of "RTS1" file */
static int32_t s_iRTS1CRCFD;     /* File descriptor of "RTS1CRC" file */
static int32_t s_iRTS2FD;        /* File descriptor of "RTS2" file */
static int32_t s_iRTS2CRCFD;     /* File descriptor of "RTS2CRC" file */

static int32_t s_iUPRTSFD;       /* File descriptor of updating RTS File */
static int32_t s_iUPRTSCRCFD;    /* File descriptor of updating FPGA file */

static uint32_t s_uiUPRTSIndex;  /* Updating RTS Index */

/* FPGA */
static int32_t s_iFPGAInfoFD;    /* File descriptor of "FPGAInfo" file */
static int32_t s_iFPGA1FD;       /* File descriptor of "FPGA1" file */
static int32_t s_iFPGA2FD;       /* File descriptor of "FPGA2" file */

static int32_t s_iUPFPGAFD;      /* File descriptor of updating RTS File */

static uint32_t s_uiUPFPGAIndex; /* Updating FPGA Index */

/* Source File */
static int32_t s_iUpSrcFD;       /* File descriptor of updating source File */

static uint8_t const *s_pucShmAddr;  /* Address of Shared memory region */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Initialize */
static void FWUPInit( int32_t iShmID );

/* Check Operation Info. */
static uint8_t CheckOprInfo( OperationInfo_t const *pstOprInfo );

/* File Operation */
static bool_t WriteFile( int32_t iFD, uint32_t uiOffset, uint8_t const pucData[], uint32_t uiLen );
static bool_t ClearFileData( int32_t iFD );

/* RTS */
static bool_t RTSUpdateStart( void );
static bool_t RTSUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen );

/* RTSCRC */
static bool_t RTSCRCUpdateStart( void );
static bool_t RTSCRCUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen );

/* Update RTS Index */
static bool_t UpdateRTSIndex( void );

/* FPGA */
static bool_t FPGAUpdateStart( void );
static bool_t FPGAUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen );
static bool_t UpdateFPGAIndex( void );

/* Source file */
static bool_t SrcUpdateStart( void );
static bool_t SrcUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: FWUPTask
*
* 功能描述: 固件更新进程
*
* 输入参数: iShmID：共享内存资源ID；
*          iMsgQID：消息队列资源ID.
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void FWUPTask( int32_t iShmID, int32_t iMsgQID )
{
    FUPOprMsg_t stOprMsg;     /* Operation Message */
    FUPOprRetMsg_t stRetMsg;  /* Operation Result Message */
    uint8_t ucDataBuf[MAX_SINGLE_FW_FRA_LEN] = { 0 };
    uint8_t ucErrCode = (uint8_t)(ERR_SUCCESS);

    if((iShmID != -1) && (iMsgQID != -1))
    {
        FWUPInit( iShmID );

        while( 1 )
        {
            /* Receive Message: block */
            if( -1 != msgrcv( iMsgQID, (void*)&stOprMsg, sizeof(stOprMsg.stOprInfo), (int32_t)(MSGT_OPR), MSG_NOERROR ))
            {
                DEBUG_FU( "msgrcv ok: OprType %d, FileType %d, FDataLen %d, FDataOffset %d\n", \
                          stOprMsg.stOprInfo.ucOprType, stOprMsg.stOprInfo.ucFileType, \
                          stOprMsg.stOprInfo.usFDataLen, stOprMsg.stOprInfo.uiFDataOffset );

                /* Operation Result Message */
                stRetMsg.stOprRetInfo.ucOprType = stOprMsg.stOprInfo.ucOprType;
                stRetMsg.stOprRetInfo.ucFileType = stOprMsg.stOprInfo.ucFileType;
                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_SUCCESS);

                ucErrCode = CheckOprInfo( &stOprMsg.stOprInfo );
                if((uint8_t)(ERR_SUCCESS) == ucErrCode )
                {
                    DEBUG_FU( "Operation Message Valid\n" );

                    /* Operation */
                    if((uint8_t)(OPRT_UPSTART) == stOprMsg.stOprInfo.ucOprType )
                    {
                        /* Update Start */
                        if((uint8_t)(FUT_CMRTS) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !RTSUpdateStart())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_RTSUP_START);
                                DEBUG_FU( "RTSUpdateStart Failed\n" );
                            }
                        }
                        else if((uint8_t)(FUT_CMRTSCRC) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !RTSCRCUpdateStart())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_RTSCRCUP_START);
                                DEBUG_FU( "RTSCRCUpdateStart Failed\n" );
                            }
                        }
                        else if((uint8_t)(FUT_CMFPGA) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !FPGAUpdateStart())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_FPGAUP_START);
                                DEBUG_FU( "FPGAUpdateStart Failed\n" );
                            }
                        }
                        else  /* FUT_SRC */
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_SRCUP_OPR_RET);
                            if( !SrcUpdateStart())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_SRCUP_START);
                                DEBUG_FU( "SrcUpdateStart Failed\n" );
                            }
                        }
                    }
                    else if((uint8_t)(OPRT_UPCONT) == stOprMsg.stOprInfo.ucOprType )
                    {
                        /* Update Continue */
                        /* Copy file data from shared memory */
                        memcpy( &ucDataBuf[0], &s_pucShmAddr[stOprMsg.stOprInfo.uiFDataOffset], \
                                (uint32_t)stOprMsg.stOprInfo.usFDataLen );

                        if((uint8_t)(FUT_CMRTS) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !RTSUpdateCont( stOprMsg.stOprInfo.uiFDataOffset, &ucDataBuf[0], \
                                                stOprMsg.stOprInfo.usFDataLen ))
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_RTSUP_CONT);
                                DEBUG_FU( "RTSUpdateCont Failed\n" );
                            }
                        }
                        else if((uint8_t)(FUT_CMRTSCRC) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !RTSCRCUpdateCont( stOprMsg.stOprInfo.uiFDataOffset - (uint32_t)(BASE_ADDR_CM_RTSCRC), \
                                                   &ucDataBuf[0], stOprMsg.stOprInfo.usFDataLen ))
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_RTSCRCUP_CONT);
                                DEBUG_FU( "RTSCRCUpdateCont Failed\n" );
                            }
                        }
                        else if((uint8_t)(FUT_CMFPGA) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !FPGAUpdateCont( stOprMsg.stOprInfo.uiFDataOffset - (uint32_t)(BASE_ADDR_CM_FPGA), \
                                                 &ucDataBuf[0], stOprMsg.stOprInfo.usFDataLen ))
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_FPGAUP_CONT);
                                DEBUG_FU( "FPGAUpdateCont Failed\n" );
                            }
                        }
                        else  /* FUT_SRC */
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_SRCUP_OPR_RET);
                            if( !SrcUpdateCont( stOprMsg.stOprInfo.uiFDataOffset - (uint32_t)(BASE_ADDR_SRC), \
                                                &ucDataBuf[0], stOprMsg.stOprInfo.usFDataLen ))
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_SRCUP_CONT);
                                DEBUG_FU( "SrcUpdateCont Failed\n" );
                            }
                        }
                    }
                    else /* OPRT_UPINDEX */
                    {
                        if(((uint8_t)(FUT_CMRTS) == stOprMsg.stOprInfo.ucFileType) ||
                           ((uint8_t)(FUT_CMRTSCRC) == stOprMsg.stOprInfo.ucFileType))
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !UpdateRTSIndex())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_CHG_RTS_INDEX);
                                DEBUG_FU( "UpdateRTSIndex Failed\n" );
                            }
                        }
                        else if((uint8_t)(FUT_CMFPGA) == stOprMsg.stOprInfo.ucFileType )
                        {
                            stRetMsg.iMsgType = (int32_t)(MSGT_FWUP_OPR_RET);
                            if( !UpdateFPGAIndex())
                            {
                                stRetMsg.stOprRetInfo.ucErrCode = (uint8_t)(ERR_CM_CHG_FPGA_INDEX);
                                DEBUG_FU( "UpdateFPGAIndex Failed\n" );
                            }
                        }
                        else  /* FUT_SRC */
                        {
                            DEBUG_FU( "UpdateIndex: File Type Invalid!\n", stOprMsg.stOprInfo.ucFileType );
                        }
                    }
                }
                else
                {
                    stRetMsg.stOprRetInfo.ucErrCode = ucErrCode;
                }

                /* Send Operation Result: block */
                if( -1 == msgsnd( iMsgQID, (void*)&stRetMsg, sizeof(stRetMsg.stOprRetInfo), 0 ))
                {
                    DEBUG_FU( "msgsnd failed: errno %d\n", errno );
                }
            }
            else
            {
                DEBUG_FU( "msgrcv failed: errno %d\n", errno );
            }
        }/* while */
    }
    else
    {
        DEBUG_FU( "iShmID %d or iMsgQID %d is invalid\n", iShmID, iMsgQID );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitOprMsgList
*
* 功能描述: 初始化操作消息链表
*
* 输入参数: pstOprList: 操作消息链表
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitOprMsgList( FUPOprMsgList_t *pstOprList )
{
    if( pstOprList != NULL )
    {
        memset( pstOprList, 0, sizeof(FUPOprMsgList_t));
        pstOprList->ucTotalNum = (uint8_t)(MAX_OPR_MSG_NUM);
    }
}

/*
***********************************************************************************************************************
* 函数名称: IsOprMsgListEmpty
*
* 功能描述: 操作消息链表是否为空
*
* 输入参数: pstOprList: 操作消息链表
*
* 输出参数: 无
*
* 返 回 值: 是否为空
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsOprMsgListEmpty( FUPOprMsgList_t const *pstOprList )
{
    bool_t bEmpty = false;

    if( pstOprList != NULL )
    {
        if( 0U == pstOprList->ucCurNum )
        {
            bEmpty = true;
        }
    }

    return bEmpty;
}

/*
***********************************************************************************************************************
* 函数名称: WriteOprMsg
*
* 功能描述: 向操作消息链表中写入消息
*
* 输入参数: pstOprList：操作消息链表；
*          pstOprMsg：操作消息。
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t WriteOprMsg( FUPOprMsgList_t *pstOprList, FUPOprMsg_t *pstOprMsg )
{
    uint8_t ucErrCode = (uint8_t)(ERR_PARA);

    if((pstOprList != NULL) && (pstOprMsg != NULL))
    {
        if((pstOprList->ucCurNum < pstOprList->ucTotalNum) &&
           (pstOprList->ucHead < pstOprList->ucTotalNum))
        {
            memcpy( &pstOprList->stOprMsg[pstOprList->ucHead], pstOprMsg, sizeof(FUPOprMsg_t));

            /* Update List Info. */
            pstOprList->ucCurNum++;
            pstOprList->ucHead++;
            if( pstOprList->ucHead == pstOprList->ucTotalNum )
            {
                pstOprList->ucHead = 0;
            }

            ucErrCode = (uint8_t)(ERR_SUCCESS);
        }
        else
        {
            ucErrCode = (uint8_t)(ERR_FWUP_BUSY);
        }
    }

    return ucErrCode;
}

/*
***********************************************************************************************************************
* 函数名称: SendOprMsg
*
* 功能描述: 发送操作消息链表中的消息：发空
*
* 输入参数: pstOprList：操作消息链表；
*          iMsgQID：消息队列ID；
*          pstSendEInfo：发送错误信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SendOprMsg( FUPOprMsgList_t *pstOprList, int32_t iMsgQID, OprRetInfo_t *pstSendEInfo )
{
    FUPOprMsg_t const *pstOprMsg = NULL;
    bool_t bFail = false;

#ifdef DEBUG_FUP
    uint32_t uiTime = 0U;
    static uint32_t uiMaxTime = 0U;
    struct timeval stPreTime;
    struct timeval stNextTime;
#endif

    if((pstOprList != NULL) && (iMsgQID != -1) && (pstSendEInfo != NULL))
    {
        if( pstOprList->ucTail < pstOprList->ucTotalNum )
        {
            while( pstOprList->ucCurNum > 0U )
            {
                pstOprMsg = &pstOprList->stOprMsg[pstOprList->ucTail];

#ifdef DEBUG_FUP
                gettimeofday( &stPreTime, NULL );
#endif

                /* Send Operation Message: nonblock */
                if( -1 == msgsnd( iMsgQID, (void*)pstOprMsg, sizeof(pstOprMsg->stOprInfo), IPC_NOWAIT ))
                {
                    DEBUG_FU( "msgsnd errno %d\n", errno );

                    if((EAGAIN == errno) || (EINTR == errno))
                    {
                        DEBUG_FU( "msgsnd: Full/Interrupt\n" );
                    }
                    else
                    {
                        DEBUG_FU( "msgsnd failed\n" );
                        bFail = true;
                    }

                    break;
                }
                else
                {
#ifdef DEBUG_FUP
                    gettimeofday( &stNextTime, NULL );

                    if( stNextTime.tv_usec >= stPreTime.tv_usec )
                    {
                        uiTime = stNextTime.tv_usec - stPreTime.tv_usec;
                    }
                    else
                    {
                        uiTime = 1000000 + stNextTime.tv_usec - stPreTime.tv_usec;
                    }

                    if( uiTime > uiMaxTime )
                    {
                        uiMaxTime = uiTime;
                    }

                    DEBUG_FU( "msgsnd: Len %d; Time %d, MaxTime %d\n", sizeof(pstOprMsg->stOprInfo), uiTime, uiMaxTime );
#endif

                    /* Update List Info. */
                    pstOprList->ucCurNum--;
                    pstOprList->ucTail++;
                    if( pstOprList->ucTail == pstOprList->ucTotalNum )
                    {
                        pstOprList->ucTail = 0;
                    }
                }
            }/* end of while */

            /* Set Error Info. */
            if( bFail )
            {
                pstSendEInfo->ucOprType = pstOprMsg->stOprInfo.ucOprType;
                pstSendEInfo->ucFileType = pstOprMsg->stOprInfo.ucFileType;
                pstSendEInfo->ucErrCode = (uint8_t)(ERR_SEND_OPR_MSG);
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RecvOprRetMsg
*
* 功能描述: 接收操作结果消息：一周期最多接收一个，失败时，只记录日志
*
* 输入参数: iMsgQID：消息队列ID；
*          emRetMsgType：结果类型。
*
* 输出参数: 无
*
* 返 回 值: 是否发生操作错误，若错误，则记录日志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t RecvOprRetMsg( int32_t iMsgQID, MsgType_t emRetMsgType )
{
    bool_t bOprError = false;
    FUPOprRetMsg_t stRetMsg;

    if((iMsgQID != -1) &&
       ((MSGT_FWUP_OPR_RET == emRetMsgType) || (MSGT_SRCUP_OPR_RET == emRetMsgType)))
    {
        /* Receive Message: non-block */
        if( -1 == msgrcv( iMsgQID, (void*)&stRetMsg, sizeof(stRetMsg.stOprRetInfo), (int32_t)(emRetMsgType), \
                          MSG_NOERROR | IPC_NOWAIT ))
        {
            if( errno != ENOMSG )
            {
                DEBUG_FU( "msgrcv failed: %d\n", errno );
            }
        }
        else
        {
            DEBUG_FU( "Operation Result: OprType %d, FileType %d, ErrCode %d\n", \
                       stRetMsg.stOprRetInfo.ucOprType, stRetMsg.stOprRetInfo.ucFileType, \
                       stRetMsg.stOprRetInfo.ucErrCode );

            if( stRetMsg.stOprRetInfo.ucErrCode != (uint8_t)(ERR_SUCCESS))
            {
                DEBUG_FU( "Operation error: %d\n", stRetMsg.stOprRetInfo.ucErrCode );
                bOprError = true;
            }
        }
    }

    return bOprError;
}

/*
***********************************************************************************************************************
* 函数名称: ReadFile
*
* 功能描述: 读文件数据
*
* 输入参数: iFD：文件FD；
*          uiOffset：待读取数据的字节偏移，基地址为0；
*          pucData：数据缓冲区；
*          uiLen：目标读取长度。
*
* 输出参数: 无
*
* 返 回 值  : 实际读取的字节数
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t ReadFile( int32_t iFD, uint32_t uiOffset, uint8_t pucData[], uint32_t uiLen )
{
    int32_t iLen = -1;

    if((iFD != (int32_t)(INVALID_FD)) &&
       (pucData != NULL))
    {
        /* Update file pointer: Offset base address 0 */
        if( lseek( iFD, (off_t)uiOffset, SEEK_SET ) != -1 )
        {
            /* read file */
            iLen = read( iFD, &pucData[0], uiLen );
            if( -1 == iLen )
            {
                DEBUG_FU( "Read file failed: %d!\n", errno );
            }
            else
            {
                DEBUG_FU( "Read file: Dstlen %d, Actual len %d.\n", uiLen, iLen );
            }
        }
        else
        {
            DEBUG_FU( "R-lseek file failed: %d!\n", errno );
        }
    }

    return iLen;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: FWUPInit
*
* 功能描述: 模块初始化函数
*
* 输入参数: iShmID: ID of shared memory region
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void FWUPInit( int32_t iShmID )
{
    /* RTS */
    s_uiUPRTSIndex = 0U;
    s_iUPRTSFD = (int32_t)(INVALID_FD);
    s_iUPRTSCRCFD = (int32_t)(INVALID_FD);

    /* FPGA */
    s_uiUPFPGAIndex = 0U;
    s_iUPFPGAFD = (int32_t)(INVALID_FD);

    /************ Open file ************/
    /* R/W, flush after write,
     * non-block(no effect for block device and regular file) */
    /**** RTS ****/
    /* "RTSInfo" file */
    s_iRTSInfoFD = open( "/home/root/RTSInfo", O_RDWR|O_SYNC );
    if((int32_t)(INVALID_FD) == s_iRTSInfoFD )
    {
        DEBUG_FU( "Open RTSInfo file failed, errno: %d!\n", errno );
    }

    /* "RTS1" file: R/W, Create if not exist, flush after write */
    s_iRTS1FD = open( "/home/root/RTS1", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iRTS1FD )
    {
        DEBUG_FU( "Open RTS1 file failed, errno: %d!\n", errno );
    }

    /* "RTS1CRC" file: R/W, Create if not exist, flush after write */
    s_iRTS1CRCFD = open( "/home/root/RTS1CRC", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iRTS1CRCFD )
    {
        DEBUG_FU( "Open RTS1CRC file failed, errno: %d!\n", errno );
    }

    /* "RTS2" file: R/W, Create if not exist, flush after write */
    s_iRTS2FD = open( "/home/root/RTS2", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iRTS2FD )
    {
        DEBUG_FU( "Open RTS2 file failed, errno: %d!\n", errno );
    }

    /* "RTS2CRC" file: R/W, Create if not exist, flush after write */
    s_iRTS2CRCFD = open( "/home/root/RTS2CRC", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iRTS2CRCFD )
    {
        DEBUG_FU( "Open RTS2CRC file failed, errno: %d!\n", errno );
    }

    /**** FPGA ****/
    /* "FPGAInfo" file: R/W, Create if not exist, flush after write */
    s_iFPGAInfoFD = open( "/home/root/FPGAInfo", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iFPGAInfoFD )
    {
        DEBUG_FU( "Open FPGAInfo file failed, errno: %d!\n", errno );
    }

    /* "FPGA1" file: R/W, Create if not exist, flush after write */
    s_iFPGA1FD = open( "/home/root/FPGA1", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iFPGA1FD )
    {
        DEBUG_FU( "Open FPGA1 file failed, errno: %d!\n", errno );
    }

    /* "FPGA2" file: R/W, Create if not exist, flush after write */
    s_iFPGA2FD = open( "/home/root/FPGA2", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iFPGA2FD )
    {
        DEBUG_FU( "Open FPGA2 file failed, errno: %d!\n", errno );
    }

    /**** Source File ****/
    /* "SrcPrj" file: R/W, Create if not exist, flush after write */
    s_iUpSrcFD = open( "/home/root/SrcPrj", O_RDWR|O_CREAT|O_SYNC, S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR );
    if((int32_t)(INVALID_FD) == s_iUpSrcFD )
    {
        DEBUG_FU( "Open source file failed, errno: %d!\n", errno );
    }

    /* Shared Memory: Attach, Readonly */
    /* Return: address(success) / -1(error) */
    s_pucShmAddr = (uint8_t const *)shmat( iShmID, NULL, SHM_RDONLY );
    if((uint8_t const *)-1 == s_pucShmAddr )
    {
        s_pucShmAddr = NULL;
        DEBUG_FU( "shmat failed, errno: %d!\n", errno );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckOprInfo
*
* 功能描述: 检查操作信息
*
* 输入参数: pstOprInfo: 操作信息结构体
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t CheckOprInfo( OperationInfo_t const *pstOprInfo )
{
    uint8_t ucErrCode = (uint8_t)(ERR_SUCCESS);
    uint32_t uiOffset = 0U;
    uint32_t uiLen = 0U;

    if( pstOprInfo != NULL )
    {
        if((pstOprInfo->ucOprType >= (uint8_t)(OPRT_UPSTART)) &&
           (pstOprInfo->ucOprType <= (uint8_t)(OPRT_UPINDEX)))
        {
            if((pstOprInfo->ucFileType >= (uint8_t)(FUT_CMFPGA)) &&
               (pstOprInfo->ucFileType <= (uint8_t)(FUT_SRC)))
            {
                if((pstOprInfo->usFDataLen <= (uint16_t)(MAX_SINGLE_FW_FRA_LEN)) &&
                   ((pstOprInfo->uiFDataOffset + (uint32_t)pstOprInfo->usFDataLen) <= (uint32_t)(SHM_FUP_SIZE)))
                {
                    ucErrCode = (uint8_t)(ERR_SUCCESS);
                }
                else
                {
                    ucErrCode = (uint8_t)(ERR_CM_FWUP_FILE_LEN);
                }
            }
            else
            {
                ucErrCode = (uint8_t)(ERR_FWPRG_TYPE);
            }
        }
        else
        {
            ucErrCode = (uint8_t)(ERR_CM_FWUP_OPR);
        }

        if((uint8_t)(ERR_SUCCESS) == ucErrCode )
        {
            if((uint8_t)(OPRT_UPCONT) == pstOprInfo->ucOprType )
            {
                uiOffset = pstOprInfo->uiFDataOffset;
                uiLen = (uint32_t)pstOprInfo->usFDataLen;

                /* Check Offset and length */
                if((uint8_t)(FUT_CMRTS) == pstOprInfo->ucFileType )
                {
                    /* [0, BASE_ADDR_CM_RTSCRC) */
                    if(((uiOffset >= (uint32_t)(BASE_ADDR_CM_RTS)) && (uiOffset < (uint32_t)(BASE_ADDR_CM_RTSCRC))) &&
                        (uiOffset+uiLen <= (uint32_t)(BASE_ADDR_CM_RTSCRC)))
                    {
                        ucErrCode = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucErrCode = (uint8_t)(ERR_CM_FWUP_FILE_LEN);
                    }
                }
                else if((uint8_t)(FUT_CMRTSCRC) == pstOprInfo->ucFileType )
                {
                    /* [BASE_ADDR_CM_RTSCRC, BASE_ADDR_CM_FPGA) */
                    if(((uiOffset >= (uint32_t)(BASE_ADDR_CM_RTSCRC)) && (uiOffset < (uint32_t)(BASE_ADDR_CM_FPGA))) &&
                        (uiOffset+uiLen <= (uint32_t)(BASE_ADDR_CM_FPGA)))
                    {
                        ucErrCode = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucErrCode = (uint8_t)(ERR_CM_FWUP_FILE_LEN);
                    }
                }
                else if((uint8_t)(FUT_CMFPGA) == pstOprInfo->ucFileType )
                {
                    /* [BASE_ADDR_CM_FPGA, BASE_ADDR_SRC) */
                    if(((uiOffset >= (uint32_t)(BASE_ADDR_CM_FPGA)) && (uiOffset < (uint32_t)(BASE_ADDR_SRC))) &&
                        (uiOffset+uiLen <= (uint32_t)(BASE_ADDR_SRC)))
                    {
                        ucErrCode = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucErrCode = (uint8_t)(ERR_CM_FWUP_FILE_LEN);
                    }
                }
                else
                {
                    /* [BASE_ADDR_SRC, BASE_ADDR_SRC) */
                    if(((uiOffset >= (uint32_t)(BASE_ADDR_SRC)) && (uiOffset < (uint32_t)(SHM_FUP_SIZE))) &&
                        (uiOffset+uiLen <= (uint32_t)(SHM_FUP_SIZE)))
                    {
                        ucErrCode = (uint8_t)(ERR_SUCCESS);
                    }
                    else
                    {
                        ucErrCode = (uint8_t)(ERR_CM_FWUP_FILE_LEN);
                    }
                }
            }
        }
    }
    else
    {
        ucErrCode = (uint8_t)(ERR_PARA);
    }

    return ucErrCode;
}

/*
***********************************************************************************************************************
* 函数名称: WriteFile
*
* 功能描述: 将数据写入文件
*
* 输入参数: iFD：文件FD；
*          uiOffset：待写入数据的字节偏移，基地址为0；
*          pucData：待写入数据；
*          uiLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否写入成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t WriteFile( int32_t iFD, uint32_t uiOffset, uint8_t const pucData[], uint32_t uiLen )
{
    bool_t bResult = false;
    int32_t iLen = 0;

    if((iFD != (int32_t)(INVALID_FD)) &&
       (pucData != NULL))
    {
        /* Update file pointer: Offset base address 0 */
        if( lseek( iFD, (off_t)uiOffset, SEEK_SET ) != -1 )
        {
            /* write file */
            iLen = write( iFD, pucData, uiLen );
            if( -1 == iLen )
            {
                DEBUG_FU( "Write file failed: %d!\n", errno );
            }
            else
            {
                if((uint32_t)iLen != uiLen )
                {
                    DEBUG_FU( "Write file: not completed, %d != %d.\n", iLen, uiLen );
                }
                else
                {
                    bResult = true;
                }
            }
        }
        else
        {
            DEBUG_FU( "W-lseek file failed: %d!\n", errno );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ClearFileData
*
* 功能描述: 清除文件数据
*
* 输入参数: iFD：文件FD
*
* 输出参数: 无
*
* 返 回 值  : 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ClearFileData( int32_t iFD )
{
    bool_t bResult = false;
    int32_t iLen = 0;

    /* Test */
    struct timeval stPreTime;
    struct timeval stNextTime;

    gettimeofday( &stPreTime, NULL );

    if( iFD != (int32_t)(INVALID_FD))
    {
        gettimeofday( &stPreTime, NULL );
        if( ftruncate( iFD, 0 ) != -1 )  /* 249.218ms */
        {
            gettimeofday( &stNextTime, NULL );
            bResult = true;
            DEBUG_FU( "ftruncate tiem: Pre %d %d, Next %d %d\n", stPreTime.tv_sec, stPreTime.tv_usec,\
                                                                  stNextTime.tv_sec, stNextTime.tv_usec );
        }
        else
        {
            DEBUG_FU( "ftruncate failed, errno %d\n", errno );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RTSUpdateStart
*
* 功能描述: CM RTS更新开始
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RTSUpdateStart( void )
{
    bool_t bResult = false;
    char_t cIndex[4] = { 0 };
    uint32_t uiRTSIndex = 0U;
    int32_t iLen = 0;

    /* Read RTS index saved in the file "RTSInfo" */
    iLen = ReadFile( s_iRTSInfoFD, 0, &cIndex[0], sizeof(cIndex));
    if( iLen >= 0 )
    {
        if( iLen > 0 )
        {
            /* String to int */
            uiRTSIndex = (uint32_t)strtoul( &cIndex[0], NULL, 0 );
            DEBUG_FU( "RTSUpdateStart: RTSIndex %s %d\n", cIndex, uiRTSIndex );
        }
        else
        {
            uiRTSIndex = 1U;
            DEBUG_FU( "RTSUpdateStart: RTSInfo file is empty\n" );
        }

        if( 2U == uiRTSIndex )
        {
            s_iUPRTSFD = s_iRTS1FD;
        }
        else
        {
            s_iUPRTSFD = s_iRTS2FD;
        }

        /* File name: do not need to write  */
        if( ClearFileData( s_iUPRTSFD ))
        {
            bResult = true;
        }
        else
        {
            DEBUG_FU( "RTSUpdateStart: Clear RTSFileData Failed\n" );
        }
    }
    else
    {
        DEBUG_FU( "RTSUpdateStart: Read RTSInfo File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RTSUpdateCont
*
* 功能描述: CM RTS更新继续
*
* 输入参数：pucData：数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RTSUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bResult = false;

#ifdef DEBUG_FUP
    uint32_t uiTime = 0U;
    static uint32_t uiMaxTime = 0U;
    struct timeval stPreTime;
    struct timeval stNextTime;

    gettimeofday( &stPreTime, NULL );
#endif

    /* Write CM RTS data into current RTS file */
    if( WriteFile( s_iUPRTSFD, uiFWOffset, &pucData[0], (uint32_t)usDataLen ))
    {
#ifdef DEBUG_FUP
        gettimeofday( &stNextTime, NULL );

        if( stNextTime.tv_usec >= stPreTime.tv_usec )
        {
            uiTime = stNextTime.tv_usec - stPreTime.tv_usec;
        }
        else
        {
            uiTime = 1000000 + stNextTime.tv_usec - stPreTime.tv_usec;
        }

        if( uiTime > uiMaxTime )
        {
            uiMaxTime = uiTime;
        }

        DEBUG_FU( "RTSUpdateCont-WriteFile: Len %d; Time %d, MaxTime %d\n", usDataLen, uiTime, uiMaxTime );
#endif

        bResult = true;
    }
    else
    {
        DEBUG_FU( "RTSUpdateCont: Write UPRTS File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RTSCRCUpdateStart
*
* 功能描述: CM RTS CRC文件更新开始
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RTSCRCUpdateStart( void )
{
    bool_t bResult = false;
    char_t cIndex[4] = { 0 };
    uint32_t uiRTSIndex = 0U;
    int32_t iLen = 0;

    /* Read RTS index saved in the file "RTSInfo" */
    iLen = ReadFile( s_iRTSInfoFD, 0, &cIndex[0], sizeof(cIndex));
    if( iLen >= 0 )
    {
        if( iLen > 0 )
        {
            /* String to int */
            uiRTSIndex = (uint32_t)strtoul( &cIndex[0], NULL, 0 );
            DEBUG_FU( "RTSCRCUpdateStart: RTSIndex %s %d\n", cIndex, uiRTSIndex );
        }
        else
        {
            uiRTSIndex = 1U;
            DEBUG_FU( "RTSCRCUpdateStart: RTSInfo file is empty\n" );
        }

        if( 2U == uiRTSIndex )
        {
            s_iUPRTSCRCFD = s_iRTS1CRCFD;
            s_uiUPRTSIndex = 1U;
        }
        else
        {
            s_iUPRTSCRCFD = s_iRTS2CRCFD;
            s_uiUPRTSIndex = 2U;
        }

        /* File name: do not need to write */
        if( ClearFileData( s_iUPRTSCRCFD ))
        {
            bResult = true;
        }
        else
        {
            DEBUG_FU( "RTSCRCUpdateStart: Clear RTSCRCFileData Failed\n" );
        }
    }
    else
    {
        DEBUG_FU( "RTSCRCUpdateStart: Read RTSInfo File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: RTSCRCUpdateCont
*
* 功能描述: CM RTS CRC文件更新继续
*
* 输入参数：uiFWOffset：偏移字节数；
*          pucData：数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RTSCRCUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bResult = false;

#ifdef DEBUG_FUP
    uint32_t uiTime = 0U;
    static uint32_t uiMaxTime = 0U;
    struct timeval stPreTime;
    struct timeval stNextTime;

    gettimeofday( &stPreTime, NULL );
#endif

    /* Write data into current RTS CRC file */
    if( WriteFile( s_iUPRTSCRCFD, uiFWOffset, &pucData[0], (uint32_t)usDataLen ))
    {
#ifdef DEBUG_FUP
        gettimeofday( &stNextTime, NULL );

        if( stNextTime.tv_usec >= stPreTime.tv_usec )
        {
            uiTime = stNextTime.tv_usec - stPreTime.tv_usec;
        }
        else
        {
            uiTime = 1000000 + stNextTime.tv_usec - stPreTime.tv_usec;
        }

        if( uiTime > uiMaxTime )
        {
            uiMaxTime = uiTime;
        }

        DEBUG_FU( "RTSCRCUpdateCont-WriteFile: Len %d; Time %d, MaxTime %d\n", usDataLen, uiTime, uiMaxTime );
#endif

        bResult = true;
    }
    else
    {
        DEBUG_FU( "RTSCRCUpdateCont: Write UPRTSCRC File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateRTSIndex
*
* 功能描述: 更新索引值
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t UpdateRTSIndex( void )
{
    bool_t bResult = false;
    char_t cIndex[4] = { 0 };

    /* Write index to its file */
    cIndex[0] = '0' + (char_t)s_uiUPRTSIndex;
    if( WriteFile( s_iRTSInfoFD, 0U, &cIndex[0], sizeof(cIndex)))
    {
        bResult = true;
        DEBUG_FU( "UpdateRTSIndex: New RTSIndex: %d\n", s_uiUPRTSIndex );
    }
    else
    {
        DEBUG_FU( "UpdateRTSIndex: Write RTSInfo File Failed\n" );
    }

    /* Clear local info. */
    s_uiUPRTSIndex = 0U;
    s_iUPRTSFD = (int32_t)(INVALID_FD);
    s_iUPRTSCRCFD = (int32_t)(INVALID_FD);

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: FPGAUpdateStart
*
* 功能描述: CM FPGA文件更新开始
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t FPGAUpdateStart( void )
{
    bool_t bResult = false;
    char_t cIndex[4] = { 0 };
    uint32_t uiFPGAIndex = 0U;
    int32_t iLen = 0;

    /* Read FPGA index saved in the file "FPGAInfo" */
    iLen = ReadFile( s_iFPGAInfoFD, 0U, &cIndex[0], sizeof(cIndex));
    if( iLen >= 0 )
    {
        if( iLen > 0 )
        {
            /* String to int */
            uiFPGAIndex = (uint32_t)strtoul( &cIndex[0], NULL, 0 );
            DEBUG_FU( "FPGAUpdateStart: FPGAIndex  %s  %d\n", cIndex, uiFPGAIndex );
        }
        else
        {
            uiFPGAIndex = 1U;
            DEBUG_FU( "FPGAUpdateStart: FPGAInfo file is empty\n" );
        }

        if( 2U == uiFPGAIndex )
        {
            s_iUPFPGAFD = s_iFPGA1FD;
            s_uiUPFPGAIndex = 1U;
        }
        else
        {
            s_iUPFPGAFD = s_iFPGA2FD;
            s_uiUPFPGAIndex = 2U;
        }

        /* File name: do not need to write */
        if( ClearFileData( s_iUPFPGAFD ))
        {
            bResult = true;
        }
        else
        {
            DEBUG_FU( "FPGAUpdateStart: Clear FPGAFileData Failed\n" );
        }
    }
    else
    {
        DEBUG_FU( "FPGAUpdateStart: Read FPGAInfo File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: FPGAUpdateCont
*
* 功能描述: CM FPGA文件更新继续
*
* 输入参数：uiFWOffset：偏移字节数；
*          pucData：数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t FPGAUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bResult = false;

    /* Write data into current FPGA file */
    if( WriteFile( s_iUPFPGAFD, uiFWOffset, &pucData[0], (uint32_t)usDataLen ))
    {
        bResult = true;
    }
    else
    {
        DEBUG_FU( "FPGAUpdateCont: Write UPFPGA File Failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateFPGAIndex
*
* 功能描述: 更新FPGA索引值
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t UpdateFPGAIndex( void )
{
    bool_t bResult = false;
    char_t cIndex[4] = { 0 };

    /* Write index to its file */
    cIndex[0] = '0' + (char_t)s_uiUPFPGAIndex;
    if( WriteFile( s_iFPGAInfoFD, 0U, &cIndex[0], sizeof(cIndex)))
    {
        bResult = true;
        DEBUG_FU( "UpdateFPGAIndex: New FPGAIndex %d\n", s_uiUPFPGAIndex );
    }
    else
    {
        DEBUG_FU( "UpdateFPGAIndex: Write FPGAInfo File Failed\n" );
    }

    /* Clear local info. */
    s_uiUPFPGAIndex = 0U;
    s_iUPFPGAFD = (int32_t)(INVALID_FD);

    return bResult;
}


/*
***********************************************************************************************************************
* 函数名称: SrcUpdateStart
*
* 功能描述: 源文件更新开始
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SrcUpdateStart( void )
{
    bool_t bResult = false;

    /* Clear file data */
    if( ClearFileData( s_iUpSrcFD ))
    {
        bResult = true;
    }
    else
    {
        DEBUG_FU( "SrcUpdateStart: Clear source file data failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SrcUpdateCont
*
* 功能描述: 源文件更新继续
*
* 输入参数：uiFWOffset：写偏移值，相对于此文件首地址；
*          pucData：数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SrcUpdateCont( uint32_t uiFWOffset, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bResult = false;

    /* Write CM RTS data into current RTS file */
    if( WriteFile( s_iUpSrcFD, uiFWOffset, &pucData[0], (uint32_t)usDataLen ))
    {
        bResult = true;
    }
    else
    {
        DEBUG_FU( "SrcUpdateCont: Write source file data failed\n" );
    }

    return bResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

