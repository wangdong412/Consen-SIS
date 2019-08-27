/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
*
*  模块名称    : 外部通讯软件应用模块
*
*  文件名称    : FPGALoad.c
*
*  功能简介    : 上电加载FPGA
*
*  作者        : 王东
*
*  创建日期    : 2017-03-31
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
#include <sys/stat.h>
#include "../inc/FPGALoad.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

typedef volatile unsigned char VCHAR;      /* shorthand for volatile UCHAR */
typedef volatile int VINT32;     /* volatile unsigned word */
typedef volatile short VINT16;     /* volatile unsigned halfword */
typedef volatile char VINT8;      /* volatile unsigned byte */
typedef volatile uint32_t VUINT32;    /* volatile unsigned word */
typedef volatile unsigned short VUINT16;    /* volatile unsigned halfword */
typedef volatile unsigned char VUINT8;     /* volatile unsigned byte */

#define GPIO_WRITE  (0)
#define GPIO_READ   (1)

#define OK     (0)
#define ERROR  (-1)

#define min(X, Y)                            \
        ({ typeof (X) __x = (X);             \
                 typeof (Y) __y = (Y);       \
                 (__x < __y) ? __x : __y; })

#define max(X, Y)                            \
        ({ typeof (X) __x = (X);             \
                 typeof (Y) __y = (Y);       \
                 (__x > __y) ? __x : __y; })

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static int32_t cycloneFpgaTx( int32_t iSPIFD, uint8_t const pucFPGAPrg[], uint32_t uiFPGAPrgLen );

/* GPIO */
static int32_t sysGpioLineConfig( int32_t iGPIOFD, uint8_t ucLineNo, uint32_t uiStyle );
static int32_t sysGpioLineGet( int32_t iGPIOFD, uint32_t *puiGpioData, uint8_t ucLineNo );
static int32_t sysGpioLineSet( int32_t iGPIOFD, uint8_t ucLineNo, uint8_t ucValue );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: sysCycloneFlush
*
* 功能描述: 上电加载FPGA
*
* 输入参数: iSPIFD：SPI设备FD；
*          pucFPGAPrg：FPGA程序数据；
*          uiFPGAPrgLen：FPGA程序长度。
*
* 输出参数: 无
*
* 返 回 值: 0: 成功； < 0 代表失败，具体如下：
*                  -1：Step 1 failed；
*                  -2：Step 2 failed；
*                  -3：Open GPIO device file failed；
*                  -4：Write to SPI device failed；
*                  -5: Error after write ok;
*                  -6：Input parameters invalid.
*
* 注意事项: 无
***********************************************************************************************************************
*/

int32_t sysCycloneFlush( int32_t iSPIFD, uint8_t const pucFPGAPrg[], uint32_t uiFPGAPrgLen )
{
    int32_t iRet = (int32_t)(EIPARA);
    int32_t iGPIOFD = -1;
    uint32_t ucS1RetryCnt = 0U;   /* Step 1 retry count */
    uint32_t ucS2RetryCnt = 0U;   /* Step 2 retry count */
    uint32_t uiGPIOStatus = 0U;   /* GPIO Status */
    uint32_t uiGPIOStatus1 = 0U;  /* GPIO Status 1 */

    if((iSPIFD != -1) && (pucFPGAPrg != NULL))
    {
        /* Open GPIO device file */
        iGPIOFD = open( "/dev/gpio_xfer", O_RDWR | O_SYNC );
        if( iGPIOFD < 0 )
        {
            iRet = (int32_t)(EOPEN);
            DEBUG_FPGA( "gpio_xfer open fail\n" );
        }
        else
        {
            iRet = (int32_t)(ESUCCESS);  /* no error */
            sysGpioLineConfig( iGPIOFD, (uint8_t)(GPIO_11), (uint32_t)(GPIO_IN));
            sysGpioLineConfig( iGPIOFD, (uint8_t)(GPIO_12), (uint32_t)(GPIO_IN));
            sysGpioLineConfig( iGPIOFD, (uint8_t)(GPIO_13), (uint32_t)(GPIO_OUT));
            sysGpioLineSet( iGPIOFD, (uint8_t)(GPIO_13), (uint8_t)(GPIO_HIGH));

            do
            {
                /* step 1 */
                do
                {
                    sysGpioLineGet( iGPIOFD, &uiGPIOStatus, (uint8_t)(GPIO_12));
                    DEBUG_FPGA( "uiGPIOStatus:%x, %d\n", uiGPIOStatus, __LINE__ );
                    sysGpioLineSet( iGPIOFD, (uint8_t)(GPIO_13), (uint8_t)(GPIO_LOW));
                    usleep(10000);
                    sysGpioLineGet( iGPIOFD, &uiGPIOStatus, (uint8_t)(GPIO_12));
                    DEBUG_FPGA( "uiGPIOStatus:%x, %d\n", uiGPIOStatus, __LINE__ );
                }while((0x1 == uiGPIOStatus) && (ucS1RetryCnt++ < 30));

                if( uiGPIOStatus != 0x1 )
                {
                    sysGpioLineSet( iGPIOFD, (uint8_t)(GPIO_13), (uint8_t)(GPIO_HIGH));
                    usleep( 10000 );
                    sysGpioLineGet( iGPIOFD, &uiGPIOStatus, (uint8_t)(GPIO_12));
                    DEBUG_FPGA( "uiGPIOStatus:%x, %d\n", uiGPIOStatus, __LINE__ );
                }
                else
                {
                    DEBUG_FPGA( "Step 1 failed\n" );
                    iRet = (int32_t)(ESTEP1);  /* step 1 failed */
                    break;
                }
            }while((0x0 == uiGPIOStatus) && (ucS2RetryCnt++ < 30));

            if((int32_t)(ESUCCESS) == iRet )
            {
                if( uiGPIOStatus != 0x0 )
                {
                    sysGpioLineGet( iGPIOFD, &uiGPIOStatus, (uint8_t)(GPIO_11));
                    DEBUG_FPGA( "gpioConfigDone: %x, %d\n", uiGPIOStatus, __LINE__ );

                    /* Program FPGA data to SPI flash */
                    if( cycloneFpgaTx( iSPIFD, &pucFPGAPrg[0], uiFPGAPrgLen ) != OK )
                    {
                        iRet = (int32_t)(EWRITE);  /* Write to SPI device failed */
                        DEBUG_FPGA( "ERROR: spiFlash program failure\n" );
                    }
                    else
                    {
                        usleep(2000000);
                        sysGpioLineGet( iGPIOFD, &uiGPIOStatus, (uint8_t)(GPIO_12));
                        usleep(2000000);
                        DEBUG_FPGA( "uiGPIOStatus: %x, %d\n", uiGPIOStatus, __LINE__ );
                        sysGpioLineGet( iGPIOFD, &uiGPIOStatus1, (uint8_t)(GPIO_11));
                        DEBUG_FPGA( "gpioConfigDone: %x, %d\n", uiGPIOStatus1, __LINE__ );

                        if((0x1 == uiGPIOStatus) && (0x1 == uiGPIOStatus1))
                        {
                            iRet = (int32_t)(ESUCCESS);
                        }
                        else
                        {
                            iRet = (int32_t)(EFIN);
                        }
                    }
                }
                else
                {
                    DEBUG_FPGA( "Step 2 failed\n" );
                    iRet = ESTEP2;  /* step 2 failed */
                }
            }
        }
    }

    return iRet;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: cycloneFpgaTx
*
* 功能描述: 将FPGA程序(.rbf file)加载到SPI设备中
*
* 输入参数: iSPIFD：SPI设备FD；
*          pucFPGAPrg：FPGA程序数据；
*          uiFPGAPrgLen：FPGA程序长度。
*
* 输出参数: 无
*
* 返 回 值: OK or ERROR if there is an error
*
* 注意事项: 无
***********************************************************************************************************************
*/

static int32_t cycloneFpgaTx( int32_t iSPIFD, uint8_t const pucFPGAPrg[], uint32_t uiFPGAPrgLen )
{
    int32_t iRet = (int32_t)(ERROR);
    uint32_t uiDataLen = 0U;
    uint32_t uiOffset = 0U;

    if((iSPIFD != -1) && (pucFPGAPrg != NULL))
    {
        /* Probe the Spansion device */
        iRet = (int32_t)(OK);
        while( uiOffset < uiFPGAPrgLen )
        {
            /* Compute the number of data ready to write for one write cycle */
            uiDataLen = min( uiFPGAPrgLen - uiOffset, 4 );
            if( write( iSPIFD, &pucFPGAPrg[uiOffset], uiDataLen ) < 0 )
            {
                iRet = (int32_t)(ERROR);
                break;
            }

            usleep(10);
            uiOffset += uiDataLen;
        }
    }

    return iRet;
}

/*******************************************************************************
*
* sysGpioLineConfig - configure a GPIO pin
*
* This routine configures a GPIO pin/line number specified in the <ucLineNo>
* parameter for use as either an input or output, as specified in the
* <uiStyle> parameter.
*
* Example:
*   1) Config GPIO_0 as an open drain output Pin, it should call
*
*      sysGpioLineConfig (GPIO_0, (GPIO_OUT | GIPO_OD));
*
*   2) Config GPIO_1 as an external interrupt input Pin, level change
*
*      sysGpioLineConfig (GPIO_1, (GPIO_IN | GPIO_INT | GPIO_INT_LEVEL_TRIG));
*
* RETURNS:
* 0 when line is successfully configured, else ERROR if the line number
* is out of range, or an unknown uiStyle is requested.
*
* ERRNO
*/

static int32_t sysGpioLineConfig( int32_t iGPIOFD, uint8_t ucLineNo, uint32_t uiStyle )
{
    VUINT32 uiGpioDirReg = 0U;
    VUINT32 uiMaskReg = 0U;
    VUINT32 uiCtrlReg = 0U;
    VUINT32 uiGpioOpenDrainReg = 0U;
    uint32_t uiCmd = 0U;
    int32_t iRet = -1;

    if( ucLineNo > (uint8_t)(GPIO_PIN_MAX))
    {
        iRet = -1;
    }
    else
    {
        iRet = 0;

        uiCmd = ((uint32_t)(GPDIR) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, &uiGpioDirReg );
        //GPIO_REG_READ(GPDIR, uiGpioDirReg);

        uiCmd = ((uint32_t)(GPODR) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, &uiGpioOpenDrainReg );
        //GPIO_REG_READ(GPODR, uiGpioOpenDrainReg);

        uiCmd = ((uint32_t)(GPICR) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, &uiCtrlReg );
        //GPIO_REG_READ(GPICR, uiCtrlReg);

        uiCmd = ((uint32_t)(GPIMR) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, &uiMaskReg );
        //GPIO_REG_READ(GPIMR, uiMaskReg);

        if( uiStyle & (uint32_t)(GPIO_OUT))
        {
            uiGpioDirReg |= (0x80000000 >> ucLineNo);

            if( uiStyle & (uint32_t)(GIPO_OD))
            {
                uiGpioOpenDrainReg |= (0x80000000 >> ucLineNo);
            }
            else
            {
                uiGpioOpenDrainReg &= ~(0x80000000 >> ucLineNo);
            }
        }
        else
        {
            uiGpioDirReg &= ~(0x80000000 >> ucLineNo);

            if( uiStyle & (uint32_t)(GPIO_INT))
            {
                if( uiStyle & (uint32_t)(GPIO_INT_FAIL_TRIG))
                {
                    uiCtrlReg |= (0x80000000 >> ucLineNo);
                }
                else if( uiStyle & (uint32_t)(GPIO_INT_LEVEL_TRIG))
                {
                    uiCtrlReg &= ~(0x80000000 >> ucLineNo);
                }
                else
                {
                    iRet = -1;
                }

                if( iRet != -1 )
                {
                    uiMaskReg &= ~(0x80000000 >> ucLineNo);
                }
            }
        }

        if( 0 == iRet )
        {
            /* Set the new uiStyle */
            uiCmd = ((uint32_t)(GPDIR) << 16) | (uint32_t)(GPIO_WRITE);
            ioctl( iGPIOFD, uiCmd, &uiGpioDirReg );
            //GPIO_REG_WRITE(GPDIR, uiGpioDirReg);

            uiCmd = ((uint32_t)(GPODR) << 16) | (uint32_t)(GPIO_WRITE);
            ioctl( iGPIOFD, uiCmd, &uiGpioOpenDrainReg );
            //GPIO_REG_WRITE(GPODR, uiGpioOpenDrainReg);

            uiCmd = ((uint32_t)(GPICR) << 16) | (uint32_t)(GPIO_WRITE);
            ioctl( iGPIOFD, uiCmd, &uiCtrlReg );
            //GPIO_REG_WRITE(GPICR, uiCtrlReg);

            uiCmd = ((uint32_t)(GPIMR) << 16) | (uint32_t)(GPIO_WRITE);
            ioctl( iGPIOFD, uiCmd, &uiMaskReg );
            //GPIO_REG_WRITE(GPIMR, uiMaskReg);
        }
    }

    return iRet;
}

/*******************************************************************************
*
* sysGpioLineGet - get the value of a GPIO pin
*
* This routine returns the value of a specified GPIO <ucLineNo> pin to the
* location specified in the <value> parameter.
*
* RETURNS:
* 0 when a pin value is successfully retrieved, else ERROR if the
* specified pin number is out of range.
*
* ERRNO
*/

static int32_t sysGpioLineGet( int32_t iGPIOFD, uint32_t *puiGpioData, uint8_t ucLineNo )
{
    uint32_t uiCmd = 0U;
    int32_t iRet = -1;

    if((ucLineNo > (uint8_t)(GPIO_PIN_MAX)) || (NULL == puiGpioData))
    {
        iRet = -1;
    }
    else
    {
        iRet = 0;

        uiCmd = ((uint32_t)(GPDAT) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, puiGpioData );
        //GPIO_REG_READ(GPDAT, *gpioData);

        *puiGpioData = (*puiGpioData >> (31 - ucLineNo)) & 0x01;
    }

    return iRet;
}

/*******************************************************************************
*
* sysGpioLineSet - set the value of a GPIO pin configured for output
*
* This routine sets the <value> of a specified GPIO <ucLineNo> pin number.
*
* RETURNS:
* 0 when pin is successfully set, else ERROR if the specified pin number
* is out of range.
*
* ERRNO
*/

static int32_t sysGpioLineSet( int32_t iGPIOFD, uint8_t ucLineNo, uint8_t ucValue )
{
    uint32_t uiGpioData = 0U;
    uint32_t uiCmd = 0U;
    int32_t iRet = -1;

    if( ucLineNo > (uint8_t)(GPIO_PIN_MAX))
    {
        iRet = -1;
    }
    else
    {
        iRet = 0;

        uiCmd = ((uint32_t)(GPDAT) << 16) | (uint32_t)(GPIO_READ);
        ioctl( iGPIOFD, uiCmd, &uiGpioData );
        //GPIO_REG_READ(GPDAT, uiGpioData);

        if((uint8_t)(GPIO_HIGH) == ucValue )
        {
            uiGpioData |= (0x80000000 >> ucLineNo);
        }
        else if((uint8_t)(GPIO_LOW) == ucValue )
        {
            uiGpioData &= ~(0x80000000 >> ucLineNo);
        }
        else
        {
            iRet = -1;
        }

        if( iRet != -1 )
        {
            uiCmd = ((uint32_t)(GPDAT) << 16) | (uint32_t)(GPIO_WRITE);
            ioctl( iGPIOFD, uiCmd, &uiGpioData );
            //GPIO_REG_WRITE(GPDAT, uiGpioData);
        }
    }

    return iRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

