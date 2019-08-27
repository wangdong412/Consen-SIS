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
*  文件名称    : Log.c
*
*  功能简介    : 
*
*  作者            : 王东
*
*  创建日期    : 2017-02-13
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
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../inc/Log.h"
#include "../inc/LogID.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static int32_t s_iLogFD1;          /* FD of SRAM device 1: 0   -4999 */
static int32_t s_iLogFD2;          /* FD of SRAM device 2: 5000-9999 */
static LogCtrlInfo_t s_stLogCtrl;  /* Log control information */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static void InitLogCtrlInfo( int32_t iLogFD );
static bool_t WriteLogCtrlInfo( uint8_t const pucData[], uint32_t uiLen );

static bool_t WriteLogFile( uint8_t const pucData[], uint32_t uiLen );
static bool_t ReadLogFile( uint16_t usReadIndex, uint8_t pucData[], uint32_t uiLen );

static bool_t WriteLogFileData( int32_t iLogFD, uint8_t const pucBuff[], uint32_t uiLen, uint32_t uiOffset );
static bool_t ReadLogFileData( int32_t iLogFD, uint8_t pucBuff[], uint32_t uiLen, uint32_t uiOffset );

#ifdef DEBUG_LOGM
static void LogTest(void);
#endif

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: LogInit
*
* 功能描述: 初始化 - 打开SRAM设备文件
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

void LogInit( void )
{
    s_iLogFD1 = (int32_t)(INVALID_FD);
    s_iLogFD2 = (int32_t)(INVALID_FD);

    /* Open SRAM device file: R/W, flush after write,
     *                        non-block(no effect for block device and regular file) */
    s_iLogFD1 = open( "/dev/sram1", O_RDWR | O_SYNC | O_NDELAY );
    if((int32_t)(INVALID_FD) == s_iLogFD1 )
    {
        DEBUG_LOG( "Open SRAM device file 1 failed, errno: %d!\n", errno );
    }

    s_iLogFD2 = open( "/dev/sram2", O_RDWR | O_SYNC | O_NDELAY );
    if((int32_t)(INVALID_FD) == s_iLogFD2 )
    {
        DEBUG_LOG( "Open SRAM device file 2 failed, errno: %d!\n", errno );
    }

    /* Log control info.: read from sram1 */
    InitLogCtrlInfo( s_iLogFD1 );

#ifdef DEBUG_LOGM
    LogTest();
#endif

    return;
}

/*
***********************************************************************************************************************
* 函数名称: LogClear
*
* 功能描述: 清除日志
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

void LogClear( void )
{
    /* Clear Log control info. */
    memset( &s_stLogCtrl, 0, sizeof(LogCtrlInfo_t));
    s_stLogCtrl.usTotalNum = (uint16_t)(MAX_LOG_NUM);
}

/*
***********************************************************************************************************************
* 函数名称: LogGetCurNum
*
* 功能描述: 获取当前日志数目
*
* 输入参数: 无
*
* 输出参数: 当前日志数目
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t LogGetCurNum( void )
{
    return s_stLogCtrl.usCurNum;
}

/*
***********************************************************************************************************************
* 函数名称: LogWrite
*
* 功能描述: 记录1条日志
*
* 输入参数: uiLogID: 日志的id
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void LogWrite( uint32_t uiLogID )
{
    long64_t llTime = 0;
    long64_t llLowTime = 0;
    long64_t llRegTime = 0;
    LogInfo_t stLogInfo;
    memset( &stLogInfo, 0, sizeof(LogInfo_t));

    /* Make a log */
    /* Read system time: us */
    do
    {
        /* If lowest reg time will overflow: 35us */
        llLowTime = (long64_t)GetSysTimeReg1();
        llRegTime = (long64_t)GetSysTimeReg2();
        llTime = llLowTime | (llRegTime << 16);
        llRegTime = (long64_t)GetSysTimeReg3();
        llTime = llTime | (llRegTime << 32);
        llRegTime = (long64_t)GetSysTimeReg4();
        llTime = llTime | (llRegTime << 48);
    }while( llLowTime > 65500 );

    /* us -> s, us*/
    stLogInfo.uiSec = llTime / (1000*1000);
    stLogInfo.uiMicroSec = llTime % (1000*1000);

    /* LogID */
    stLogInfo.uiLogID = uiLogID;

    /* write file */
    if( WriteLogFile( &stLogInfo, sizeof(LogInfo_t)))
    {
        /* Update log control info. */
        /* Head */
        if( s_stLogCtrl.usHead < (s_stLogCtrl.usTotalNum - 1))
        {
            s_stLogCtrl.usHead++;
        }
        else
        {
            s_stLogCtrl.usHead = 0U;
        }

        if( s_stLogCtrl.bFull )
        {
            /* Tail */
            s_stLogCtrl.usTail = s_stLogCtrl.usHead;
        }
        else
        {
            /* Current Num */
            s_stLogCtrl.usCurNum++;

            /* Full flag */
            if( s_stLogCtrl.usTotalNum == s_stLogCtrl.usCurNum )
            {
                s_stLogCtrl.bFull = true;
            }
        }

        /* Record log control info. */
        if( !WriteLogCtrlInfo((uint8_t const *)&s_stLogCtrl, sizeof(s_stLogCtrl)))
        {
            DEBUG_LOG( "WriteLogCtrlInfo failed\n" );
        }
        else
        {
            DEBUG_LOG( "WriteLogCtrlInfo ok\n" );
        }
    }
    else
    {
        DEBUG_LOG( "WriteLogFile failed\n" );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: LogRead
*
* 功能描述: 从SRAM设备文件读取一条日志
*
* 输入参数: pstLogInfo: 待存放日志的指针；
*          usLogicPos: 队列中的位置(READ_LOG_FROM_BEGINNING 表示从开头读)；
*          uiSec：s；
*          uiMicroSec：us；
*          bFirRead：诊断软件每个请求的第一次读标志。
*
* 输出参数: pusNextIndex：下一可读索引值
*
* 返 回 值  : -1 日志队列为空，              下一可读索引值被设置为  0
*          -2 已读空，                          下一可读索引值被设置为 下一写入位置，usHead
*          -3 入参错误，                      下一可读索引值 未被设置，需调用者处理
*          -4 文件相关系统调用失败，下一可读索引值被设置为 usLogicPos非0xFFFF时：usLogicPos；否则当前最老的日志，uiTail
*          >=0 下一可读索引值，          下一可读索引值 与返回值相同
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t LogRead( LogInfo_t *pstLogInfo, uint16_t usLogicPos, puint16_t pusNextIndex,
                 uint32_t uiSec, uint32_t uiMicroSec )
{
    int32_t iLogIndex = (int32_t)(READ_LOG_EPARA);
    bool_t bRead = false;
    uint16_t usReadIndex = 0U;
    uint16_t usLastIndex = 0U;  /* Index of Last Log */

    if((pstLogInfo != NULL) &&
       (pusNextIndex != NULL) &&
       (((uint16_t)(READ_LOG_FROM_BEGINNING) == usLogicPos) || (usLogicPos < (uint16_t)(MAX_LOG_NUM))))
    {
        if( 0U == s_stLogCtrl.usCurNum )
        {
            iLogIndex = (int32_t)(LOG_QUEUE_EMPTY);  /* Empty */
            *pusNextIndex = 0U;
        }
        else
        {
            if((uint16_t)(READ_LOG_FROM_BEGINNING) == usLogicPos )
            {
                usReadIndex = s_stLogCtrl.usTail;
                bRead = true;
            }
            else
            {
                if( s_stLogCtrl.bFull )
                {
                    /* Get index of Last Log */
                    if( usLogicPos > 0U )
                    {
                        usLastIndex = usLogicPos - 1;
                    }
                    else
                    {
                        usLastIndex = (uint16_t)(MAX_LOG_NUM) - 1;
                    }

                    /* Read Last Log */
                    if( ReadLogFile( usLastIndex, (puint8_t)pstLogInfo, sizeof(LogInfo_t)))
                    {
                        /* Compare time stamp */
                        if((pstLogInfo->uiSec == uiSec) && (pstLogInfo->uiMicroSec == uiMicroSec ))
                        {
                            /* Not overlapped: Read if not repeat */
                            if( usLogicPos == s_stLogCtrl.usHead )
                            {
                                /* Repeat: Read done */
                                iLogIndex = (int32_t)(READ_LOG_DONE);  /* Has been read empty */
                                *pusNextIndex = s_stLogCtrl.usHead;
                            }
                            else
                            {
                                /* Not Repeat */
                                usReadIndex = usLogicPos;
                                bRead = true;
                            }
                        }
                        else
                        {
                            /* Overlapped: Read from the oldest log */
                            usReadIndex = s_stLogCtrl.usTail;
                            bRead = true;
                        }
                    }
                    else
                    {
                        /* Read Failed: Update iLogIndex and pusNextIndex */
                        iLogIndex = (int32_t)(READ_LOG_EFILE);
                        *pusNextIndex = usLogicPos;
                    }
                }
                else
                {
                    if( usLogicPos < s_stLogCtrl.usHead )
                    {
                        usReadIndex = usLogicPos;
                        bRead = true;
                    }
                    else
                    {
                        /* Read done */
                        iLogIndex = (int32_t)(READ_LOG_DONE);  /* Has been read empty */
                        *pusNextIndex = s_stLogCtrl.usHead;
                    }
                }
            }

            if( bRead )
            {
                /* Read Log file */
                if( ReadLogFile( usReadIndex, (puint8_t)pstLogInfo, sizeof(LogInfo_t)))
                {
                    /* Read Successfully: Update iLogIndex and pusNextIndex */
                    if( usReadIndex < (s_stLogCtrl.usTotalNum - 1))
                    {
                        usReadIndex++;
                    }
                    else
                    {
                        usReadIndex = 0U;
                    }

                    iLogIndex = (int32_t)usReadIndex;
                    *pusNextIndex = usReadIndex;
                }
                else
                {
                    /* Read Failed: Update iLogIndex and pusNextIndex */
                    iLogIndex = (int32_t)(READ_LOG_EFILE);
                    *pusNextIndex = usReadIndex;
                }
            }
        }/* end of else - has log */
    }

    return iLogIndex;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
************************************************************************************************************************
* 函数名称: LogTest
*
* 功能描述: 测试函数
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

#ifdef DEBUG_LOGM
static void LogTest(void)
{
    uint16_t usCurNum = 0U;
    LogInfo_t stLogInfo;
    uint16_t usLogicPos = 0U;
    uint16_t usNextIndex = 0U;
    int32_t iResult = 0;

    uint8_t i = 0U;
    uint8_t ucContent[100] = { 0 };
    uint8_t ucAdd = 2;

#if 0
    /* Test case 0: lseek, write, read */
    for( i = 0; i < sizeof(LogInfo_t); i++)
    {
       ucContent[i] = i;
    }

    /* Write */
    i = 0;
    while( i < sizeof(LogInfo_t))
    {
        lseek( s_iLogFD1, i, SEEK_SET );

        /* write file */
        write( s_iLogFD1, &ucContent[i], ucAdd );
        i += ucAdd;
    }

    /* Read */
    memset( &ucContent[0], 0, sizeof(LogInfo_t));
    i = 0;
    while( i < sizeof(LogInfo_t))
    {
        lseek( s_iLogFD1, i, SEEK_SET );

        /* write file */
        read( s_iLogFD1, &ucContent[i], ucAdd );
        i += ucAdd;
    }

    /* Print */
    for( i = 0; i < sizeof(LogInfo_t); i++ )
    {
        printf( "%d ", ucContent[i]);
    }
    printf( "\n" );
#endif

#if 0
    /* Test case 1: Empty; One log; Read done */
    LogClear();
    DEBUG_LOG( "LogNum: %d\n", LogGetCurNum());  /* 0 */

    usLogicPos = 0xFFFF;
    memset( &stLogInfo, 0, sizeof(LogInfo_t));
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* -1: Empty, 0, 0 */

    LogWrite( 0x12345678 );
    DEBUG_LOG( "LogNum: %d\n", LogGetCurNum());  /* 1 */
    usLogicPos = 0xFFFF;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 1:next index, 1, 0x12345678 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* -2: read done, 1, 0x12345678 */

    /* Test case 2: Overwrite - MAX_LOG_NUM: 3 */
    LogClear();
    DEBUG_LOG( "LogNum: %d\n", LogGetCurNum());  /* 0 */

    LogWrite( 0x00000001 );
    LogWrite( 0x00000002 );

    usLogicPos = 0xFFFF;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 1, 1, 0x00000001 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 2, 2, 0x00000002 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* -2, 2, 0x00000002 */

    LogWrite( 0x00000003 );  /* Full */
    usLogicPos = usNextIndex;  /* 2 */
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 0, 0, 0x00000003 */

    LogWrite( 0x00000004 );  /* Overwrite to index 0 */
    usLogicPos = usNextIndex;  /* 0 */
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 1, 1, 0x00000004 */

    LogWrite( 0x00000005 );  /* Overwrite to index 1 */
    usLogicPos = usNextIndex;  /* 1 */
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 2, 2, 0x00000005 */
#endif

    /* Test case 3: SRAM1, SRAM2, MAX_LOG_NUM 4, MAX_FILE_LOG_NUM 2 */
    LogClear();
    DEBUG_LOG( "LogNum: %d\n", LogGetCurNum());  /* 0 */

    /* SRAM 1*/
    LogWrite( 0x00000001 );
    LogWrite( 0x00000002 );

    /* SRAM2 */
    LogWrite( 0x00000011 );
    LogWrite( 0x00000022 );

    /* Read */
    usLogicPos = 0xFFFF;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 1, 1, 0x00000001 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 2, 2, 0x00000002 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 3, 3, 0x00000011 */

    usLogicPos = usNextIndex;
    iResult = LogRead( &stLogInfo, usLogicPos, &usNextIndex );
    DEBUG_LOG( "Result %d, NextIndex %d, ID %08X\n", iResult,  usNextIndex,  stLogInfo.uiLogID );  /* 0, 0, 0x00000022 */

    LogClear();
    DEBUG_LOG( "LogNum: %d\n", LogGetCurNum());  /* 0 */
}
#endif

/*
***********************************************************************************************************************
* 函数名称: InitLogCtrlInfo
*
* 功能描述: 初始化日志控制信息
*
* 输入参数: iLogFD：日志控制信息所在文件的文件描述符
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitLogCtrlInfo( int32_t iLogFD )
{
    bool_t bValid = false;
    uint32_t uiOffset = (uint32_t)(OFF_LOG_CTRL);

    if( ReadLogFileData( iLogFD, &s_stLogCtrl, sizeof(LogCtrlInfo_t), uiOffset ))
    {
        if((uint16_t)(MAX_LOG_NUM) == s_stLogCtrl.usTotalNum )
        {
            bValid = true;
            DEBUG_LOG( "History: %d, H %d, T %d\n", \
                       s_stLogCtrl.usCurNum, s_stLogCtrl.usHead, s_stLogCtrl.usTail );
        }
        else
        {
            DEBUG_LOG( "No History\n" );
        }
    }
    else
    {
        DEBUG_LOG( "InitLogCtrlInfo: ReadLogFileData fail\n" );
    }

    if( !bValid )
    {
        LogClear();
    }
}

/*
***********************************************************************************************************************
* 函数名称: WriteLogCtrlInfo
*
* 功能描述: 将日志控制信息写入日志设备文件
*
* 输入参数: pucData：待写入数据；
*          uiLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否写入成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t WriteLogCtrlInfo( uint8_t const pucData[], uint32_t uiLen )
{
    bool_t bResult = false;
    uint32_t uiOffset = 0U;
    int32_t iLogFD = (int32_t)(INVALID_FD);

    if( pucData != NULL )
    {
        iLogFD = s_iLogFD1;
        uiOffset = (uint32_t)(OFF_LOG_CTRL);

        /* Record Log ctrl info. */
        bResult = WriteLogFileData( iLogFD, pucData, uiLen, uiOffset );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: WriteLogFile
*
* 功能描述: 将1条日志写入日志设备文件
*
* 输入参数: pucData：待写入数据；
*          uiLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否写入成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t WriteLogFile( uint8_t const pucData[], uint32_t uiLen )
{
    bool_t bResult = false;
    uint32_t uiOffset = 0U;
    int32_t iLogFD = (int32_t)(INVALID_FD);

    if( pucData != NULL )
    {
        /* FD */
        if( s_stLogCtrl.usHead < (uint16_t)(MAX_LOG_NUM))
        {
            if( s_stLogCtrl.usHead < (uint16_t)(MAX_FILE_LOG_NUM))
            {
                iLogFD = s_iLogFD1;

                /* Calculate write position */
                uiOffset = (uint32_t)(OFF_LOG) + (s_stLogCtrl.usHead * sizeof(LogInfo_t));
            }
            else
            {
                iLogFD = s_iLogFD2;

                /* Calculate write position */
                uiOffset = (uint32_t)(OFF_LOG) + (s_stLogCtrl.usHead - (uint16_t)(MAX_FILE_LOG_NUM))*sizeof(LogInfo_t);
            }

            /* Record Log */
            bResult = WriteLogFileData( iLogFD, pucData, uiLen, uiOffset );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadLogFile
*
* 功能描述: 从日志设备文件中读出1条日志
*
* 输入参数: usReadIndex：日志索引值；
*          pucData：数据缓冲区；
*          uiLen：目标读取数据长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否读取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadLogFile( uint16_t usReadIndex, uint8_t pucData[], uint32_t uiLen )
{
    bool_t bResult = false;
    uint32_t uiOffset = 0U;
    int32_t iLogFD = (int32_t)(INVALID_FD);

    if((usReadIndex < (uint16_t)(MAX_LOG_NUM)) &&
       (pucData != NULL))
    {
        if( usReadIndex < (uint16_t)(MAX_FILE_LOG_NUM))
        {
            iLogFD = s_iLogFD1;

            /* Calculate read position */
            uiOffset = (uint32_t)(OFF_LOG) + (usReadIndex * sizeof(LogInfo_t));
        }
        else
        {
            iLogFD = s_iLogFD2;

            /* Calculate read position */
            uiOffset = (uint32_t)(OFF_LOG) + (usReadIndex - (uint16_t)(MAX_FILE_LOG_NUM)) * sizeof(LogInfo_t);
        }

        bResult = ReadLogFileData( iLogFD, pucData, uiLen, uiOffset );
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ReadLogFileData
*
* 功能描述: 读取日志文件数据
*
* 输入参数: iLogFD：文件描述符；
*          pucBuff：目标缓冲区；
*          uiLen：目标读取长度；
*          uiOffset：目标数据的偏移字节数。
*
* 输出参数: 无
*
* 返 回 值  : 是否读取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t ReadLogFileData( int32_t iLogFD, uint8_t pucBuff[], uint32_t uiLen, uint32_t uiOffset )
{
    bool_t bResult = false;
    int32_t iLen = 0;

    if((iLogFD != (int32_t)(INVALID_FD)) &&
       (pucBuff != NULL))
    {
        /* Update file pointer: Offset base address 0 */
        if( lseek( iLogFD, (off_t)uiOffset, SEEK_SET ) != -1 )
        {
            /* read file */
            iLen = read( iLogFD, pucBuff, uiLen );
            if( -1 == iLen )
            {
                DEBUG_LOG( "Read SRAM device file failed: %d\n", errno );
            }
            else
            {
                if((uint32_t)iLen != uiLen )
                {
                    DEBUG_LOG( "Read SRAM device file: not completed, %d != %d\n", iLen, uiLen );
                }
                else
                {
                    bResult = true;
                }
            }
        }
        else
        {
            DEBUG_LOG( "R-lseek SRAM device file failed: %d\n", errno );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: WriteLogFileData
*
* 功能描述: 写日志文件数据
*
* 输入参数: iLogFD：文件描述符；
*          pucBuff：数据缓冲区；
*          uiLen：数据长度；
*          uiOffset：偏移字节数。
*
* 输出参数: 无
*
* 返 回 值  : 是否写入成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t WriteLogFileData( int32_t iLogFD, uint8_t const pucBuff[], uint32_t uiLen, uint32_t uiOffset )
{
    bool_t bResult = false;
    int32_t iLen = 0;

    if((iLogFD != (int32_t)(INVALID_FD)) &&
       (pucBuff != NULL))
    {
        /* Update file pointer: Offset base address 0 */
        if( lseek( iLogFD, (off_t)uiOffset, SEEK_SET ) != -1 )
        {
            /* write file */
            iLen = write( iLogFD, pucBuff, uiLen );
            if( -1 == iLen )
            {
                DEBUG_LOG( "Write SRAM device file failed: %d\n", errno );
            }
            else
            {
                if((uint32_t)iLen != uiLen )
                {
                    DEBUG_LOG( "Write SRAM device file: not completed, %d != %d\n", iLen, uiLen );
                }
                else
                {
                    bResult = true;
                }
            }
        }
        else
        {
            DEBUG_LOG( "W-lseek SRAM device file failed: %d\n", errno );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
