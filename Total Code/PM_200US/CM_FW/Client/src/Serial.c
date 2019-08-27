/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : Client软件应用模块
*
*  文件名称    : Serial.c
*
*  功能简介    : 串口相关功能
*
*  作者        : 王东
*
*  创建日期    : 2017-04-01
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
#include <sys/types.h>
#include <error.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>    /* File control definitions */
#include <errno.h>
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/stat.h>
#include "../inc/Serial.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Serial Port */
static bool_t SetSerlNonBlock( int32_t iFD );
static bool_t SetBaudRate( struct termios *pstOptions, uint8_t ucBaudRate );
static bool_t SetDataBits( struct termios *pstOptions, uint8_t ucDataBits );
static bool_t SetStopBits( struct termios *pstOptions, uint8_t ucStopBits );
static bool_t SetParity( struct termios *pstOptions, uint8_t ucParity );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: OpenSerlPort
*
* 功能描述: 打开串口：未打开时，打开；已打开时，直接返回
*
* 输入参数: ucSerialIndex：串口索引值；
*          piSerlFDAray：串口文件描述符数组。
*
* 输出参数: 无
*
* 返 回 值: 是否打开成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t OpenSerlPort( uint8_t ucSerialIndex, int32_t piSerlFDAray[])
{
    bool_t bResult = false;
    int32_t iFD = (int32_t)(INVALID_FD);
    char_t cCOMName[100] = { 0 };

    if((ucSerialIndex < (uint8_t)(NUM_OF_CLI_SERIAL_PORT)) &&
       (piSerlFDAray != NULL))
    {
        if((int32_t)(INVALID_FD) == piSerlFDAray[ucSerialIndex])
        {
            /* Open */
            sprintf( cCOMName, "/dev/ttyS%d", ucSerialIndex );
            iFD = open( cCOMName, O_RDWR|O_NOCTTY|O_NDELAY );
            if( iFD != (int32_t)(INVALID_FD))
            {
                bResult = true;
                DEBUG_SERL( "Open serial port[%d] OK\n", ucSerialIndex );
            }
            else
            {
                DEBUG_SERL( "Open serial port[%d] failed, Errno: %d!\n", ucSerialIndex, GetErrorNo());
            }

            piSerlFDAray[ucSerialIndex] = iFD;
        }
        else
        {
            bResult = true;  /* Has been opened */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: IsSerlPortOpen
*
* 功能描述: 判断串口是否处于打开状态
*
* 输入参数: ucSerialIndex：串口索引值；
*          piSerlFDAray：串口文件描述符数组。
*
* 输出参数: 无
*
* 返 回 值: 是否处于打开状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsSerlPortOpen( uint8_t ucSerialIndex, int32_t piSerlFDAray[])
{
    bool_t bOpen = false;

    if((ucSerialIndex < (uint8_t)(NUM_OF_CLI_SERIAL_PORT)) &&
       (piSerlFDAray != NULL))
    {
        if( piSerlFDAray[ucSerialIndex] != (int32_t)(INVALID_FD))
        {
            bOpen = true;
        }
    }

    return bOpen;
}

/*
***********************************************************************************************************************
* 函数名称: CloseSerlPort
*
* 功能描述: 关闭串口：已打开时，关闭；未打开时，直接返回。
*
* 输入参数: ucSerialIndex：串口索引值；
*          piSerlFDAray：串口文件描述符数组。
*
* 输出参数: 无
*
* 返 回 值: 是否关闭成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t CloseSerlPort( uint8_t ucSerialIndex, int32_t piSerlFDAray[])
{
    bool_t bResult = false;

    if((ucSerialIndex < (uint8_t)(NUM_OF_CLI_SERIAL_PORT)) &&
       (piSerlFDAray != NULL))
    {
        if( piSerlFDAray[ucSerialIndex] != (int32_t)(INVALID_FD))
        {
            /* Flush: Clear Receive & Send Buffer */
            if( !ClearSerlPortBuff( piSerlFDAray[ucSerialIndex], BUFF_ALL ))
            {
                DEBUG_SERL( "Clear serial port[%d] buffer failed\n", ucSerialIndex );
            }

            /* Close */
            if( close( piSerlFDAray[ucSerialIndex]) != -1 )
            {
                bResult = true;
                piSerlFDAray[ucSerialIndex] = (int32_t)(INVALID_FD);
            }
            else
            {
                DEBUG_SERL( "Close serial port[%d] failed: errno %d\n", ucSerialIndex, GetErrorNo());
            }
        }
        else
        {
            bResult = true;  /* Has been closed */
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: ClearSerlPortBuff
*
* 功能描述: 清除串口发送/接收缓冲区
*
* 输入参数: iFD：串口文件描述符；
*          emBuffType：清除类型。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t ClearSerlPortBuff( int32_t iFD, BuffType_t emBuffType )
{
    bool_t bResult = false;
    int32_t iQueue = TCIOFLUSH;

    if((iFD != (int32_t)(INVALID_FD)) &&
       ((emBuffType >= BUFF_RECV) && (emBuffType <= BUFF_ALL)))
    {
        switch( emBuffType )
        {
            case BUFF_RECV:
            {
                iQueue = TCIFLUSH;
            }
            break;

            case BUFF_SEND:
            {
                iQueue = TCOFLUSH;
            }
            break;

            case BUFF_ALL:
            default:
            {
                iQueue = TCIOFLUSH;
            }
            break;
        }/* end of switch */

        if( tcflush( iFD, iQueue ) != -1 )
        {
            bResult = true;
        }
        else
        {
            DEBUG_SERL( "tcflush failed: Errno %d\n", GetErrorNo());
        }
    }

    return bResult;
}


/*
***********************************************************************************************************************
* 函数名称: ConfigSerlPort
*
* 功能描述: 配置串口
*
* 输入参数: ucSerialIndex：串口索引值；
*          piSerlFDAray：串口文件描述符数组；
*          pstSerlSets：串口设置参数。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t ConfigSerlPort( uint8_t ucSerialIndex, int32_t const piSerlFDAray[], SerlPortSets_t const *pstSerlSets )
{
    bool_t bResult = false;
    int32_t iFD = (int32_t)(INVALID_FD);
    struct termios stOptions;

    if((ucSerialIndex < (uint8_t)(NUM_OF_CLI_SERIAL_PORT)) &&
       (piSerlFDAray != NULL) &&
       (pstSerlSets != NULL))
    {
        iFD = piSerlFDAray[ucSerialIndex];

        /* Opened */
        if( iFD != (int32_t)(INVALID_FD))
        {
            /* Non-Block */
            if( SetSerlNonBlock( iFD ))
            {
                /* Get Serial Attributes */
                if( -1 == tcgetattr( iFD, &stOptions ))
                {
                    DEBUG_SERL( "tcgetattr failed: Serial Port[%d], Errno %d\n", ucSerialIndex, GetErrorNo());
                }
                else
                {
                    /* Set Serial Port */
                    /* CLOCAL: Set local mode */
                    /* CREAD: Enable the receiver */
                    stOptions.c_cflag |= (CLOCAL | CREAD);

                    /* Baud Rate */
                    if( SetBaudRate( &stOptions, pstSerlSets->ucBaudRate ))
                    {
                        /* Data Bits, Stop Bits  */
                        if( SetDataBits( &stOptions, pstSerlSets->ucDataBits ) &&
                            SetStopBits( &stOptions, pstSerlSets->ucStopBits ) &&
                            SetParity( &stOptions, pstSerlSets->ucParity ))
                        {
                            /* Raw input: Will not change input char */
                            stOptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

                            /* Raw output: Will not change output char */
                            stOptions.c_oflag &= ~OPOST;

                            /* Disable Software Flow Control */
                            stOptions.c_iflag &= ~(IXON | IXOFF | IXANY);

                            /* clear 0x0d and 0x0a map */
                            stOptions.c_oflag &= ~(ONLCR | OCRNL);
                            stOptions.c_iflag &= ~(INLCR | ICRNL);

                            /* If O_NDELAY: These settings will be ignored */
                            stOptions.c_cc[VTIME] = 0U;  /* Time to wait for data (tenths of seconds) */
                            stOptions.c_cc[VMIN] = 0U;  /* Minimum number of characters to read */

                            /* Flush: Clear Receive & Send Buffer */
                            if( ClearSerlPortBuff( iFD, BUFF_ALL ))
                            {
                                /* Set Serial Attributes */
                                /* TCSANOW: Make changes now without waiting for data to complete */
                                if( tcsetattr( iFD, TCSANOW, &stOptions ) != -1 )
                                {
                                    bResult = true;
                                }
                                else
                                {
                                    DEBUG_SERL( "tcsetattr failed: Serial Port[%d], Errno %d\n", \
                                               ucSerialIndex, GetErrorNo());
                                }
                            }
                            else
                            {
                                DEBUG_SERL( "ClearSerlPortBuff Failed: Serial Port[%d]\n", ucSerialIndex );
                            }
                        }
                    }
                    else
                    {
                        DEBUG_SERL( "SetBaudRate failed: Serial Port[%d], Errno %d\n", ucSerialIndex, GetErrorNo());
                    }
                }
            }/*end of if - SetSerlNonBlock */
            else
            {
                DEBUG_SERL( "SetSerlNonBlock Failed: Serial Port[%d]\n", ucSerialIndex );
            }
        }/* end of if-opened */
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetSerlPort
*
* 功能描述: 设置串口：已打开时，直接设置；未打开时，先打开在设置。
*
* 输入参数: ucSerialIndex：串口索引值；
*          piSerlFDAray：串口文件描述符数组；
*          pstSerlSets：串口配置参数。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SetSerlPort( uint8_t ucSerialIndex, int32_t piSerlFDAray[], SerlPortSets_t const *pstSerlSets )
{
    bool_t bResult = false;

    if((ucSerialIndex < (uint8_t)(NUM_OF_CLI_SERIAL_PORT)) &&
       (piSerlFDAray != NULL) &&
       (pstSerlSets != NULL))
    {
        bResult = true;

        /* Open */
        if( !IsSerlPortOpen( ucSerialIndex, piSerlFDAray ))
        {
            bResult = OpenSerlPort( ucSerialIndex, piSerlFDAray );
        }

        if( bResult )
        {
            /* Configure Serial Port */
            bResult = ConfigSerlPort( ucSerialIndex, piSerlFDAray, pstSerlSets );

            if( !bResult )
            {
                /* Close Serial Port */
                if( !CloseSerlPort( ucSerialIndex, piSerlFDAray ))
                {
                    DEBUG_SERL( "CloseSerlPort[%d] failed, Errno: %d\n", ucSerialIndex, GetErrorNo());
                }
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SetSerlNonBlock
*
* 功能描述: 设置非阻塞模式
*
* 输入参数: iFD：文件描述符
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetSerlNonBlock( int32_t iFD )
{
    bool_t bResult = false;
    int32_t iFlags = 0;

    if( iFD != (int32_t)(INVALID_FD))
    {
        /* Get Flags */
        iFlags = fcntl( iFD, F_GETFL, 0 );
        if( iFlags != -1 )
        {
            if( fcntl( iFD, F_SETFL, iFlags | O_NONBLOCK ) != -1 )  /* FNDELAY */
            {
                bResult = true;
            }
            else
            {
                DEBUG_SERL( "fcntl(set) failed-%d\n", GetErrorNo());
            }
        }
        else
        {
            DEBUG_SERL( "fcntl(get) failed-%d\n", GetErrorNo());
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetBaudRate
*
* 功能描述: 设置波特率
*
* 输入参数: pstOptions：指向串口选项结构体；
*          ucBaudRate：波特率。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetBaudRate( struct termios *pstOptions, uint8_t ucBaudRate )
{
    bool_t bResult = false;
    speed_t speed = B9600;

    if( pstOptions != NULL )
    {
        /* Speed */
        switch( ucBaudRate )
        {
            case (uint8_t)(BR1200):
            {
                speed = B1200;
            }
            break;

            case (uint8_t)(BR2400):
            {
                speed = B2400;
            }
            break;

            case (uint8_t)(BR4800):
            {
                speed = B4800;
            }
            break;

            case (uint8_t)(BR9600):
            {
                speed = B9600;
            }
            break;

            case (uint8_t)(BR19200):
            {
                speed = B19200;
            }
            break;

            case (uint8_t)(BR38400):
            {
                speed = B38400;
            }
            break;

            case (uint8_t)(BR57600):
            {
                speed = B57600;
            }
            break;

            case (uint8_t)(BR115200):
            {
                speed = B115200;
            }
            break;

            default:
            {
                speed = B9600;
            }
            break;
        }/* end of switch */

        /* Set */
        if(( -1 == cfsetispeed( pstOptions, speed )) ||
           ( -1 == cfsetospeed( pstOptions, speed )))
        {
            bResult = false;
        }
        else
        {
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetDataBits
*
* 功能描述: 设置数据位
*
* 输入参数: pstOptions：指向串口选项结构体；
*          ucDataBits：数据位。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetDataBits( struct termios *pstOptions, uint8_t ucDataBits )
{
    bool_t bResult = false;

    if( pstOptions != NULL )
    {
        bResult = true;
        pstOptions->c_cflag &= ~CSIZE; /* Mask the character size bits */

        switch( ucDataBits )
        {
            case (uint8_t)(DB7):
            {
                pstOptions->c_cflag |= CS7;  /* 7 Data Bits */
            }
            break;

            case (uint8_t)(DB8):
            default:
            {
                pstOptions->c_cflag |= CS8;  /* 8 Data Bits */
            }
            break;
        }/* end of switch */
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetStopBits
*
* 功能描述: 设置停止位
*
* 输入参数: pstOptions：指向串口选项结构体；
*          ucStopBits：停止位。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetStopBits( struct termios *pstOptions, uint8_t ucStopBits )
{
    bool_t bResult = false;

    if( pstOptions != NULL )
    {
        bResult = true;

        switch( ucStopBits )
        {
            case (uint8_t)(SB1):
            {
                pstOptions->c_cflag &= ~CSTOPB; /* 1 */
            }
            break;

            case (uint8_t)(SB2):
            default:
            {
                pstOptions->c_cflag |= CSTOPB;  /* 2 */
            }
            break;
        }/* end of switch */
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SetParity
*
* 功能描述: 设置奇偶校验位
*
* 输入参数: pstOptions：指向串口选项结构体；
*          ucParity：奇偶校验位。
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SetParity( struct termios *pstOptions, uint8_t ucParity )
{
    bool_t bResult = false;

    if( pstOptions != NULL )
    {
        bResult = true;

        /* PARENB: Enable parity bit */
        /* INPCK:  Enable parity check */
        /* PARODD: Use odd parity instead of even */
        switch( ucParity )
        {
            case (uint8_t)(PARITY_NONE):
            {
                pstOptions->c_cflag &= ~PARENB;
                pstOptions->c_iflag &= ~INPCK;
            }
            break;

            case (uint8_t)(PARITY_ODD):
            {
                pstOptions->c_cflag |= (PARODD | PARENB);
                pstOptions->c_iflag |= INPCK;
            }
            break;

            case (uint8_t)(PARITY_EVEN):
            {
                pstOptions->c_cflag |= PARENB;
                pstOptions->c_cflag &= ~PARODD;
                pstOptions->c_iflag |= INPCK;
            }
            break;

            case (uint8_t)(PARITY_MARK):
            {
                /* Mark parity: Parity Bit Value Always 1 */
                pstOptions->c_cflag |= PARENB | CS8 | CMSPAR |PARODD;
            }
            break;

            case (uint8_t)(PARITY_SPACE):
            default:
            {
                /* Space parity: Parity Bit Value Always 0 */
                pstOptions->c_cflag |= PARENB | CS8 | CMSPAR;
            }
            break;
        }/* end of switch */
    }

    return bResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

