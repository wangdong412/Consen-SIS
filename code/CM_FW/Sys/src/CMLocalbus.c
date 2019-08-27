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
*  文件名称    : CMLocalbus.c
*
*  功能简介    :
*
*  作者        : 李琦
*
*  创建日期    : 2016-05-23
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
#include "../inc/Sys.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static int32_t s_ifdlocalbus;
static int32_t s_ifdfpga;

const float_t fTempFractionalValue[16]={ 0.0000, 0.0625, 0.1250, 0.1875,
                                         0.2500, 0.3125, 0.3750, 0.4375,
                                         0.5000, 0.5625, 0.6250, 0.6875,
                                         0.7500, 0.8125, 0.8750, 0.9375
                                       };

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static uint32_t ReadReg(uint32_t uiRegAddr);
static void WriteReg(uint32_t uiRegAddr, uint32_t uiVal);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: CMLocalbusInit
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
void CMLocalbusInit(void)
{
    uint32_t uiAddr = 0;
    
    s_ifdlocalbus = open("/dev/localbus_xfer", O_RDWR | O_SYNC);

    if(s_ifdlocalbus < 0)
    {
        printf("localbus_xfer open failed\n");
    }
    else
    {
        printf("s_ifdlocalbus=%d\n",s_ifdlocalbus);
    }

    s_ifdfpga = open("/dev/pcie_dma_xfer", O_RDWR | O_SYNC);

    if(s_ifdfpga < 0)
    {
     printf("pcie_dma_xfer open failed\n");
    }
    else
    {
        printf("s_ifdfpga=%d\n",s_ifdfpga);
    }
    
    uiAddr = GetCMSlotAddr(0x10000);

    SetCMSlotAddr(uiAddr);
    
}


/*
***********************************************************************************************************************
* 函数名称: GetCMSlotAddr
*
* 功能描述: 获取CM槽号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : CM的槽号
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t GetCMSlotAddr(int32_t iTimeout)
{
    uint32_t uiReg = 0;
    uint32_t uiAddr = 0xFF;             /* 无效值 */

    do
    {
        uiReg = ReadReg(REG_SLOT_ADDR);

        if((0x8000 == (uiReg & 0x8000)) && (0x0080 == (uiReg & 0x0080)))
        {
            if(((uiReg & 0x0F00)>>8) == (uiReg & 0x000F))
            {
                uiAddr = (uiReg & 0x000F);

                break;
            }
        }

        //add timeout solution
        iTimeout--;
    }while(iTimeout > 0);

    return uiAddr;
}

/*
***********************************************************************************************************************
* 函数名称: SetCMSlotAddr
*
* 功能描述: 设置CM槽号
*
* 输入参数: 槽号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SetCMSlotAddr(uint32_t uiAddr)
{
    WriteReg(REG_STATION_ADDR, (uint32_t)(uiAddr&0x000F));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetCMSendEnable
*
* 功能描述: 发送使能
*
* 输入参数: PM的地址
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SetCMSendEnable(uint32_t uiPMAddr)
{
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                WriteReg(REG_CMBUSA_SEND_ENABLE, CMBUS_SEND_ENABLE_FLAG);
            }
            break;

            case PMB:
            {
                WriteReg(REG_CMBUSB_SEND_ENABLE, CMBUS_SEND_ENABLE_FLAG);
            }
            break;

            case PMC:
            {
                WriteReg(REG_CMBUSC_SEND_ENABLE, CMBUS_SEND_ENABLE_FLAG);
            }
            break;

            default:
            break;
        }

    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetCMSendState
*
* 功能描述: 获取发送状态
*
* 输入参数: PM的地址
*
* 输出参数: uiAddr PM的地址
*
* 返 回 值  : 状态
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t GetCMSendState(uint32_t uiPMAddr)
{
    uint32_t uiState = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                uiState = ReadReg(REG_CMBUSA_SEND_STATE);
            }
            break;

            case PMB:
            {
                uiState = ReadReg(REG_CMBUSB_SEND_STATE);
            }
            break;

            case PMC:
            {
                uiState = ReadReg(REG_CMBUSC_SEND_STATE);
            }
            break;

            default:
            break;
        }
    }
    return uiState;
}

/*
***********************************************************************************************************************
* 函数名称: SetCMSendLen
*
* 功能描述: 设置发送数据长度
*
* 输入参数: PM的地址
*
* 输出参数: uiAddr PM的地址
*        uiLen  数据长度
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void SetCMSendLen(uint32_t uiPMAddr, uint32_t uiLen)
{
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                WriteReg(REG_CMBUSA_SEND_LEN, (uiLen & CMBUS_SNED_LEN_MASK));
            }
            break;

            case PMB:
            {
                WriteReg(REG_CMBUSB_SEND_LEN, (uiLen & CMBUS_SNED_LEN_MASK));
            }
            break;

            case PMC:
            {
                WriteReg(REG_CMBUSC_SEND_LEN, (uiLen & CMBUS_SNED_LEN_MASK));
            }
            break;

            default:
            break;
        }
    }

    return;

}

/*
***********************************************************************************************************************
* 函数名称: CMSend
*
* 功能描述: 发送数据
*
* 输入参数: PM的地址
*
* 输出参数: uiAddr PM的地址
*        uiLen  数据长度
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void CMSend(uint32_t uiPMAddr, puint8_t pSendBuf, uint32_t uiLen, int32_t iTimeout)
{
    uint32_t uiState = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if((uiPMAddr < NUM_OF_PM) && (pSendBuf != NULL) && (uiLen > 0))
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                ioctl(s_ifdfpga,CMBUSA_SEND,CMBUSA_RECV1);

                if (write(s_ifdfpga, pSendBuf, uiLen) < 0)
                {
                    printf("fdfpga fail to write\n");
                }
                else
                {
                    usleep(200);
                    SetCMSendLen(uiPMAddr, uiLen);

                    SetCMSendEnable(uiPMAddr);
                }
            }
            break;

            case PMB:
            {
                ioctl(s_ifdfpga,CMBUSB_SEND,CMBUSB_RECV1);

                if (write(s_ifdfpga, pSendBuf, uiLen) < 0)
                {
                    printf("fdfpga fail to write\n");
                }
                else
                {
                    usleep(200);
                    SetCMSendLen(uiPMAddr, uiLen);

                    SetCMSendEnable(uiPMAddr);
                }
            }
            break;

            case PMC:
            {
                ioctl(s_ifdfpga,CMBUSC_SEND,CMBUSC_RECV1);

                if (write(s_ifdfpga, pSendBuf, uiLen) < 0)
                {
                    printf("fdfpga fail to write\n");
                }
                else
                {
                    usleep(200);
                    SetCMSendLen(uiPMAddr, uiLen);

                    SetCMSendEnable(uiPMAddr);
                }
            }
            break;

            default:
            break;
        }


        
#if 0
        do
        {
           uiState = GetCMSendState(uiPMAddr);

           if(CMBUS_SNED_FINISHED == (uiState & CMBUS_SNED_STATE_MASK))
           {
               break;
           }

           iTimeout--;

        }while(iTimeout > 0);
        printf("state=%x iTimeout=%x\n", uiState, iTimeout);
#endif
        
    }

    return;

}

/*
***********************************************************************************************************************
* 函数名称: ClearCMRecvState
*
* 功能描述: 接收状态
*
* 输入参数: uiPMAddr PM的地址
*        usClr: 对状态位进行清除操作
*
* 输出参数:
*
* 返 回 值  : 时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
void ClearCMRecvState(uint32_t uiPMAddr, uint16_t usClr)
{
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                WriteReg(REG_CMBUSA_RECV_STATE,(uint32_t)usClr);
            }
            break;

            case PMB:
            {
                WriteReg(REG_CMBUSB_RECV_STATE,(uint32_t)usClr);
            }
            break;

            case PMC:
            {
                WriteReg(REG_CMBUSC_RECV_STATE,(uint32_t)usClr);
            }
            break;

            default:
            break;
        }
    }
    return ;
}

/*
***********************************************************************************************************************
* 函数名称: GetCMRecvState
*
* 功能描述: 接收状态
*
* 输入参数: uiPMAddr PM的地址
*
* 输出参数:
*
* 返 回 值  : 时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t GetCMRecvState(uint32_t uiPMAddr)
{
    uint32_t uiState = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                uiState = ReadReg(REG_CMBUSA_RECV_STATE);
            }
            break;

            case PMB:
            {
                uiState = ReadReg(REG_CMBUSB_RECV_STATE);
            }
            break;

            case PMC:
            {
                uiState = ReadReg(REG_CMBUSC_RECV_STATE);
            }
            break;

            default:
            break;
        }
    }
    return uiState;
}

/*
***********************************************************************************************************************
* 函数名称: GetCMRecvTime
*
* 功能描述: 接收时间
*
* 输入参数: uiPMAddr PM的地址
*        uiBufNo  接收缓冲区索引
*
* 输出参数:
*
* 返 回 值  : 时间
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t GetCMRecvTime(uint32_t uiPMAddr, uint8_t uiBufNo)
{
    uint32_t uiTime = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSA_RECV1_TIME);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSA_RECV2_TIME);
                }
            }
            break;

            case PMB:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSB_RECV1_TIME);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSB_RECV2_TIME);
                }
            }
            break;

            case PMC:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSC_RECV1_TIME);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiTime = ReadReg(REG_CMBUSC_RECV2_TIME);
                }
            }
            break;

            default:
            break;
        }
    }
    return uiTime;
}

/*
***********************************************************************************************************************
* 函数名称: GetCMRecvLen
*
* 功能描述: 接收数据长度
*
* 输入参数: uiPMAddr PM的地址
*        uiBufNo  接收缓冲区索引
*
* 输出参数:
*
* 返 回 值  : 数据长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t GetCMRecvLen(uint32_t uiPMAddr, uint8_t uiBufNo)
{
    uint32_t uiLen = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if(uiPMAddr < NUM_OF_PM)
    {
        emPMController = (PMController_t) uiPMAddr;

        switch(emPMController)
        {
            case PMA:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSA_RECV1_LEN);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSA_RECV2_LEN);
                }
            }
            break;

            case PMB:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSB_RECV1_LEN);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSB_RECV2_LEN);
                }
            }
            break;

            case PMC:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSC_RECV1_LEN);
                }
                if(RECV_BUFF_NO2 == uiBufNo)
                {
                    uiLen = ReadReg(REG_CMBUSC_RECV2_LEN);
                }
            }
            break;

            default:
            break;
        }
    }
    return uiLen;
}

/*
***********************************************************************************************************************
* 函数名称: CMRecvBuf
*
* 功能描述: 接收数据
*
* 输入参数:
*
* 输出参数:
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void CMReceive(uint32_t uiPMAddr, uint8_t uiBufNo, puint8_t pucBuf)
{
    uint32_t uiLen = 0;
    PMController_t emPMController = INVALID_PM_ID;

    if((uiPMAddr < NUM_OF_PM) && (pucBuf != NULL))
    {
        emPMController = (PMController_t) uiPMAddr;

        uiLen = GetCMRecvLen(uiPMAddr, uiBufNo);
        //printf("uiLen = %d\n", uiLen);

        switch(emPMController)
        {
            case PMA:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSA_SEND,CMBUSA_RECV1);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf1 failed\n");
                    }
                    else
                    {
                        printf("PMA recv1 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else if(RECV_BUFF_NO2 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSA_SEND,CMBUSA_RECV2);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf2 failed\n");
                    }
                    else
                    {
                        printf("PMA recv2 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else
                {
                }
            }
            break;

            case PMB:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSB_SEND,CMBUSB_RECV1);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf1 failed\n");
                    }
                    else
                    {
                        printf("PMB recv1 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else if(RECV_BUFF_NO2 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSB_SEND,CMBUSB_RECV2);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf2 failed\n");
                    }
                    else
                    {
                        printf("PMB recv2 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else
                {
                }
            }
            break;

            case PMC:
            {
                if(RECV_BUFF_NO1 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSC_SEND,CMBUSC_RECV1);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf1 failed\n");
                    }
                    else
                    {
                        printf("PMC recv1 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else if(RECV_BUFF_NO2 == uiBufNo)
                {
                    ioctl(s_ifdfpga,CMBUSC_SEND,CMBUSC_RECV2);

                    if (read(s_ifdfpga, pucBuf, uiLen) < 0)
                    {
                        printf("read buf2 failed\n");
                    }
                    else
                    {
                        printf("PMC recv2 ok sn=%d\n", (uint16_t)*((uint16_t*)(&pucBuf[2])));
                    }
                }
                else
                {
                }
            }
            break;

            default:
            break;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateStateLED
*
* 功能描述: 更新状态灯寄存器
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void UpdateStateLED(uint16_t usState)
{
    WriteReg(REG_CONTROL_STATE_LED, (uint32_t)usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: UpdateComLED
*
* 功能描述: 更新通讯灯寄存器
*
* 输入参数: usState:待更新内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void UpdateComLED(uint16_t usState)
{
    WriteReg(REG_CONTROL_COM_LED, (uint32_t)usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBTransProto
*
* 功能描述: 设置 Modbus 传输协议：ASCII / RTU
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          usTransProto：传输协议，0: Modbus RTU; 1: Modbus ASCII.
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBTransProto( SerialPortType_t emSerlPortID, uint16_t usTransProto )
{
    uint32_t uiRegAddr = 0U;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (((uint16_t)(MB_TRANS_PROTO_RTU) == usTransProto) || ((uint16_t)(MB_TRANS_PROTO_ASCII) == usTransProto)))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_TRANS_PROTO;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_TRANS_PROTO;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_TRANS_PROTO;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_TRANS_PROTO;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usTransProto );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortBaudRate
*
* 功能描述: 设置 Modbus 串口波特率
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          usBaudRate：波特率。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortBaudRate( SerialPortType_t emSerlPortID, uint16_t usBaudRate )
{
    uint32_t uiRegAddr = 0U;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (usBaudRate >= ((uint16_t)(BR1200)) && ( usBaudRate <= (uint16_t)(BR115200))))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_BAUD_RATE;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_BAUD_RATE;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_BAUD_RATE;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_BAUD_RATE;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usBaudRate );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortTransMode
*
* 功能描述: 设置 Modbus 串口收发(Transceiver)模式：RS485 / RS422
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          usTransMode：收发模式: 0-RS485; 1-RS422。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortTransMode( SerialPortType_t emSerlPortID, uint16_t usTransMode )
{
    uint32_t uiRegAddr = 0U;

    if(((emSerlPortID >= COM3) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (((uint16_t)(TM_RS485) == usTransMode) || ((uint16_t)(TM_RS422) == usTransMode)))
    {
        switch( emSerlPortID )
        {
            /*case COM1:
            {
                uiRegAddr = REG_SERL1_MB_TRANS_MODE;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_TRANS_MODE;
            }
            break;*/

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_TRANS_MODE;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_TRANS_MODE;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usTransMode );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortParity
*
* 功能描述: 设置 Modbus 串口奇偶校验
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          usParity：奇偶校验。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortParity( SerialPortType_t emSerlPortID, uint16_t usParity )
{
    uint32_t uiRegAddr = 0U;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (((uint16_t)(PARITY_NONE) == usParity) ||
        ((uint16_t)(PARITY_ODD) == usParity) ||
        ((uint16_t)(PARITY_EVEN) == usParity)))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_PARITY;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_PARITY;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_PARITY;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_PARITY;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usParity );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortEnable
*
* 功能描述: 使能/禁止 Modbus 串口
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          bEnable：使能标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortEnable( SerialPortType_t emSerlPortID, bool_t bEnable )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usEnable = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_ENABLE;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_ENABLE;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_ENABLE;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_ENABLE;
            }
            break;

            default:
                break;
        }

        if( bEnable )
        {
            usEnable |= (uint16_t)(MB_SERL_PORT_ENABLE_BIT);
        }
        else
        {
            usEnable &= ~(uint16_t)(MB_SERL_PORT_ENABLE_BIT);
        }

        WriteReg( uiRegAddr, (uint32_t)usEnable );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortSendEnable
*
* 功能描述: 设置 Modbus 串口发送使能标志
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          bSendEnable：发送使能标志位。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortSendEnable( SerialPortType_t emSerlPortID, bool_t bSendEnable )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usSendEnable = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_SEND_ENABLE;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_SEND_ENABLE;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_SEND_ENABLE;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_SEND_ENABLE;
            }
            break;

            default:
                break;
        }

        if( bSendEnable )
        {
            usSendEnable |= (uint16_t)(MB_SERL_PORT_SEND_ENABLE_BIT);
        }
        else
        {
            usSendEnable &= ~(uint16_t)(MB_SERL_PORT_SEND_ENABLE_BIT);
        }

        WriteReg( uiRegAddr, (uint32_t)usSendEnable );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetMBPortSendLen
*
* 功能描述: 设置 Modbus 串口发送长度
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          usSendLen：发送长度。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetMBPortSendLen( SerialPortType_t emSerlPortID, uint16_t usSendLen )
{
    uint32_t uiRegAddr = 0U;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (usSendLen <= (uint16_t)(MAX_MB_SERL_MSG_LEN)))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_SEND_LEN;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_SEND_LEN;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_SEND_LEN;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_SEND_LEN;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usSendLen );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBPortSendStatus
*
* 功能描述: 获取 Modbus 串口发送状态
*
* 输入参数: emSerlPortID：Modbus 串口ID
*
* 输出参数: 无
*
* 返 回 值  : Modbus 串口发送状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetMBPortSendStatus( SerialPortType_t emSerlPortID )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usSendStatus = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_SEND_STATUS;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_SEND_STATUS;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_SEND_STATUS;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_SEND_STATUS;
            }
            break;

            default:
                break;
        }

        usSendStatus = (uint16_t)ReadReg( uiRegAddr );
    }

    return usSendStatus;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBPortRecvLen
*
* 功能描述: 获取 Modbus 串口接收长度
*
* 输入参数: emSerlPortID：Modbus 串口ID
*
* 输出参数: 无
*
* 返 回 值  : Modbus 串口接收长度
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetMBPortRecvLen( SerialPortType_t emSerlPortID )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usRecvLen = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_RECV_LEN;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_RECV_LEN;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_RECV_LEN;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_RECV_LEN;
            }
            break;

            default:
                break;
        }

        usRecvLen = (uint16_t)ReadReg( uiRegAddr );
    }

    return usRecvLen;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBPortRecvStatus
*
* 功能描述: 获取 Modbus 串口接收状态
*
* 输入参数: emSerlPortID：Modbus 串口ID
*
* 输出参数: 无
*
* 返 回 值  : Modbus 串口接收状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetMBPortRecvStatus( SerialPortType_t emSerlPortID )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usRecvStatus = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_RECV_STATUS;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_RECV_STATUS;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_RECV_STATUS;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_RECV_STATUS;
            }
            break;

            default:
                break;
        }

        usRecvStatus = (uint16_t)ReadReg( uiRegAddr );
    }

    return usRecvStatus;
}

/*
***********************************************************************************************************************
* 函数名称: ResetMBPortRecvStatus
*
* 功能描述: 复位 Modbus 串口接收状态
*
* 输入参数: emSerlPortID：Modbus 串口ID
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 通过此方式清除接收缓冲区中的数据
***********************************************************************************************************************
*/

void ResetMBPortRecvStatus( SerialPortType_t emSerlPortID )
{
    uint32_t uiRegAddr = 0U;
    uint16_t usRecvStatus = 0x03U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                uiRegAddr = REG_SERL1_MB_RECV_STATUS;
            }
            break;

            case COM2:
            {
                uiRegAddr = REG_SERL2_MB_RECV_STATUS;
            }
            break;

            case COM3:
            {
                uiRegAddr = REG_SERL3_MB_RECV_STATUS;
            }
            break;

            case COM4:
            {
                uiRegAddr = REG_SERL4_MB_RECV_STATUS;
            }
            break;

            default:
                break;
        }

        WriteReg( uiRegAddr, (uint32_t)usRecvStatus );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IsMBPortSendCompleted
*
* 功能描述: 判断 Modbus 串口是否发送完成
*
* 输入参数: emSerlPortID：Modbus 串口ID
*
* 输出参数: 无
*
* 返 回 值  : 是否发送完成
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsMBPortSendCompleted( SerialPortType_t emSerlPortID )
{
    bool_t bSendCompleted = false;
    uint16_t usSendStatus = 0U;

    if((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT))
    {
        usSendStatus = GetMBPortSendStatus( emSerlPortID );
        if((uint16_t)(MB_SERL_PORT_SEND_CMPL_STATUS) == (usSendStatus & (uint16_t)(MB_SERL_PORT_SEND_CMP_BIT)))
        {
            bSendCompleted = true;
        }
    }

    return bSendCompleted;
}

/*
***********************************************************************************************************************
* 函数名称: IsMBPortRecvedFrame
*
* 功能描述: 判断 Modbus 串口是否接收到报文
*
* 输入参数: usRecvStatus：串口接收状态
*
* 输出参数: 无
*
* 返 回 值  : 是否接收到报文标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsMBPortRecvedFrame( uint16_t usRecvStatus )
{
    bool_t bHasFrame = false;

    if( usRecvStatus & (uint16_t)(MB_SERL_PORT_HAS_FRAME_BIT))
    {
        bHasFrame = true;
    }

    return bHasFrame;
}

/*
***********************************************************************************************************************
* 函数名称: IsMBPortRecvedError
*
* 功能描述: 判断 Modbus 串口是否发生接收错误
*
* 输入参数: usRecvStatus：串口接收状态
*
* 输出参数: 无
*
* 返 回 值  : 是否接收错误标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsMBPortRecvedError( uint16_t usRecvStatus )
{
    bool_t bRecvError = false;

    if( usRecvStatus & (uint16_t)(MB_SERL_PORT_RECV_ERROR_BIT))
    {
        bRecvError = true;
    }

    return bRecvError;
}

/*
***********************************************************************************************************************
* 函数名称: PCIEModbusSend
*
* 功能描述: 发送数据
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          pucBuff：待发送数据的首地址；
*          uiLen：待发送数据的长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否发送成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PCIEModbusSend( SerialPortType_t emSerlPortID, uint8_t const *pucBuff, uint32_t uiLen )
{
    bool_t bSuccess = false;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (pucBuff != NULL) &&
       ((uiLen > 0U) && (uiLen <= (uint16_t)(MAX_MB_SERL_MSG_LEN))))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSD_SEND, PCIE_MOBUSD_RECV ) != -1 )
                {
                    if( write( s_ifdfpga, pucBuff, uiLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            case COM2:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSE_SEND, PCIE_MOBUSE_RECV ) != -1 )
                {
                    if( write( s_ifdfpga, pucBuff, uiLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            case COM3:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSF_SEND, PCIE_MOBUSF_RECV ) != -1 )
                {
                    if( write( s_ifdfpga, pucBuff, uiLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            default:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSG_SEND, PCIE_MOBUSG_RECV ) != -1 )
                {
                    if( write( s_ifdfpga, pucBuff, uiLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: MBPortSend
*
* 功能描述: Modbus 串口发送报文
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          pucBuff：待发送数据的首地址；
*          usLen：待发送数据的长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否发送成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t MBPortSend( SerialPortType_t emSerlPortID, uint8_t const *pucBuff, uint16_t usLen )
{
    bool_t bSuccess = false;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (pucBuff != NULL) &&
       ((usLen > 0U) && (usLen <= (uint16_t)(MAX_MB_SERL_MSG_LEN))))
    {
        /* Send when pre-frame was send completed */
        if( IsMBPortSendCompleted( emSerlPortID ))
        {
            /* Write Frame To PCIE Send Buffer */
            if( PCIEModbusSend( emSerlPortID, pucBuff, (uint32_t)usLen ))
            {
                /* Set Send Length */
                SetMBPortSendLen( emSerlPortID, usLen );

                /* Set Send Enable */
                SetMBPortSendEnable( emSerlPortID, true );

                bSuccess = true;
            }
            else
            {
                printf( "COM[%d] PCIEModbusSend Failed!\n", emSerlPortID+1 );
            }
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: PCIEModbusReceive
*
* 功能描述: 接收数据
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          pucBuff：接收缓冲区首地址；
*          uiRecvLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值  : 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t PCIEModbusReceive( SerialPortType_t emSerlPortID, puint8_t pucBuff, uint32_t uiRecvLen )
{
    bool_t bSuccess = false;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (pucBuff != NULL) &&
       (uiRecvLen > 0U))
    {
        switch( emSerlPortID )
        {
            case COM1:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSD_SEND, PCIE_MOBUSD_RECV ) != -1 )
                {
                    if( read( s_ifdfpga, pucBuff, uiRecvLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            case COM2:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSE_SEND, PCIE_MOBUSE_RECV ) != -1 )
                {
                    if( read( s_ifdfpga, pucBuff, uiRecvLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            case COM3:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSF_SEND, PCIE_MOBUSF_RECV ) != -1 )
                {
                    if( read( s_ifdfpga, pucBuff, uiRecvLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;

            default:
            {
                if( ioctl( s_ifdfpga, PCIE_MOBUSG_SEND, PCIE_MOBUSG_RECV ) != -1 )
                {
                    if( read( s_ifdfpga, pucBuff, uiRecvLen ) > 0 )
                    {
                        bSuccess = true;
                    }
                }
            }
            break;
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: MBPortRecv
*
* 功能描述: Modbus 串口接收报文
*
* 输入参数: emSerlPortID：Modbus 串口ID；
*          pucBuff：接收缓冲区首地址；
*          pusLen：接收缓冲区的长度。
*
* 输出参数: pusLen：数据的实际长度
*
* 返 回 值  : 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t MBPortRecv( SerialPortType_t emSerlPortID, uint8_t *pucBuff, uint16_t *pusLen )
{
    bool_t bSuccess = false;
    uint16_t usRecvLen = 0U;
    uint16_t usRecvStatus = 0U;

    if(((emSerlPortID >= COM1) && (emSerlPortID < NUM_OF_SERIAL_PORT)) &&
       (pucBuff != NULL) &&
       (pusLen != NULL))
    {
        usRecvStatus = GetMBPortRecvStatus( emSerlPortID );

        /* Check if has received a new frame */
        if( IsMBPortRecvedFrame( usRecvStatus ))
        {
            if( !IsMBPortRecvedError( usRecvStatus ))
            {
                /* Receive Successfully */
                usRecvLen = GetMBPortRecvLen( emSerlPortID );
                if( *pusLen >= usRecvLen )
                {
                    /* Read Frame From PCIE Receive Buffer */
                    if( PCIEModbusReceive( emSerlPortID, pucBuff, usRecvLen ))
                    {
                        *pusLen = usRecvLen;  /* Update Frame Length */
                        bSuccess = true;
                    }
                    else
                    {
                        printf( "COM[%d] PCIEModbusReceive Failed.\n", emSerlPortID+1 );
                    }
                }
            }
            else
            {
                printf( "COM[%d] Received Error.\n", emSerlPortID+1 );
            }

            /* Reset Receive Status */
            ResetMBPortRecvStatus( emSerlPortID );
        }
        else
        {
            *pusLen = 0U;
            bSuccess = true;
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAModID
*
* 功能描述: 获取FPGA模块ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA模块ID
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAModID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_FPGA_MOD_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAHardwareVer
*
* 功能描述: 获取FPGA硬件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA硬件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAHardwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_FPGA_HW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAChipID
*
* 功能描述: 获取FPGA芯片位号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA芯片位号
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAChipID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_FPGA_CHIP_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFPGAFirmwareVer
*
* 功能描述: 获取FPGA固件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : FPGA固件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetFPGAFirmwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_FPGA_FW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUModID
*
* 功能描述: 获取MCU模块ID
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU模块ID
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUModID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MCU_MOD_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUHardwareVer
*
* 功能描述: 获取MCU硬件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU硬件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUHardwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MCU_HW_VER);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUChipID
*
* 功能描述: 获取MCU芯片位号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU芯片位号
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUChipID(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MCU_CHIP_ID);

    return usRegVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetMCUFirmwareVer
*
* 功能描述: 获取MCU固件版本
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU固件版本
*
* 注意事项:
***********************************************************************************************************************
*/
uint16_t SysGetMCUFirmwareVer(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MCU_FW_VER);

    return usRegVal;
}

uint16_t SysGetCMTemp(void)
{
    uint16_t usRegVal = 0;
    float_t fTemp = 0.0;
    uint8_t ucIndex = 0;
    uint16_t usTemp = 0;
    int16_t iTemp = 0;

    usRegVal = ReadReg(REG_MONITOR_TEMPERATURE);
    /* 判断温度数据是否有效 */
    if((usRegVal & 0x8000) != 0)
    {
        if((usRegVal & 0x4000) == 0)
        {/* 温度为正 */

            /* 计算温度*/
            iTemp = (usRegVal & 0x07F0)>>4;
            ucIndex = usRegVal & 0x000F;
            fTemp = fTempFractionalValue[ucIndex] + (float_t) iTemp;
        }
        else
        {/* 温度为负 */
            usTemp = (usRegVal & 0x07F0)>>4;
            iTemp = (int16_t)((uint16_t)0xFF80 | usTemp);
            ucIndex = usRegVal & 0x000F;
            fTemp = fTempFractionalValue[ucIndex] + (float_t) iTemp;
        }
    }

    SysSetCM2PMTemp(fTemp);

    return usRegVal;
}

uint16_t SysGetMCUState(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MCU_STATE);

    SysSetCM2PMMCUState(usRegVal);

    return usRegVal;
}

uint16_t SysGetPluseState(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_PLUSE_MONITOR);

    SysSetCM2PMPluseState(usRegVal);

    return usRegVal;
}

uint16_t SysGetPowerState(void)
{
    uint16_t usRegVal = 0;

    usRegVal = ReadReg(REG_MONITOR_STATE);

    SysSetCM2PMPowerState(usRegVal);

    return usRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogEnState
*
* 功能描述: 设置看门狗使能或禁止
*
* 输入参数: uiState：bit0:0：无效,1：使能看门狗
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogEnState(uint32_t uiState)
{
    WriteReg(REG_WATCHDOG_EN, (uiState & 0x0001));
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogPermission
*
* 功能描述: 设置看门狗权限
*
* 输入参数: uiState：bit0:0：禁止配置,1：允许配置
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogPermission(uint32_t uiState)
{
    WriteReg(REG_WATCHDOG_PERMISSION, (uiState & 0x0001));
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogLowLimit
*
* 功能描述: 设置看门狗下限
*
* 输入参数: uiTime：单位us
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogLowLimit(uint32_t uiTime)
{
    WriteReg(REG_WATCHDOG_LIMIT_LOW, uiTime);
}

/*
***********************************************************************************************************************
* 函数名称: SysSetWatchdogHighLimit
*
* 功能描述: 设置看门狗上
*
* 输入参数: uiTime：单位us
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetWatchdogHighLimit(uint32_t uiTime)
{
    WriteReg(REG_WATCHDOG_LIMIT_HIGH, uiTime);
}

/*
***********************************************************************************************************************
* 函数名称: SysGetFreeCounterValue
*
* 功能描述: 获取自由计数器的值
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 自由计数器的值
*
* 注意事项:
***********************************************************************************************************************
*/
long64_t SysGetFreeCounterValue(void)
{
    long64_t ulVal = 0;
    long64_t ulRegVal[4];

    ulRegVal[0] = ReadReg(REG_FREE_COUNTER_1);
    ulRegVal[1] = ReadReg(REG_FREE_COUNTER_2);
    ulRegVal[2] = ReadReg(REG_FREE_COUNTER_3);
    ulRegVal[3] = ReadReg(REG_FREE_COUNTER_4);

    ulVal = ulRegVal[0];
    ulVal |= (ulRegVal[1]<<16);
    ulVal |= (ulRegVal[2]<<32);
    ulVal |= (ulRegVal[3]<<48);

    return ulVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysGetFreeCounterValue32
*
* 功能描述: 获取自由计数器的低32位值
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 自由计数器的值
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t SysGetFreeCounterValue32(void)
{
    uint32_t uiVal = 0;
    uint32_t uiRegVal[2];
    uint32_t uiRegVal_1 = 0;


    uiRegVal[1] = ReadReg(REG_FREE_COUNTER_2);
    uiRegVal[0] = ReadReg(REG_FREE_COUNTER_1);


    uiRegVal_1 = ReadReg(REG_FREE_COUNTER_2);


    if(uiRegVal_1 != uiRegVal[1])
    {
        uiRegVal[1] = ReadReg(REG_FREE_COUNTER_2);
        uiRegVal[0] = ReadReg(REG_FREE_COUNTER_1);
    }


    uiVal |= uiRegVal[0];
    uiVal |= (uiRegVal[1]<<16);


    return uiVal;
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPPMAdjPulseInput
*
* 功能描述: 校时脉冲源选择
*
* 输入参数: uiPulseInput bit1-0:01：GPS校时源,10：站间校时源，其他：无校时源
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetPPMAdjPulseInput(uint32_t uiPulseInput)
{
    WriteReg(REG_PPM_ADJ_PULSE_INPUT, (uiPulseInput&0x0003));
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPPMAdjPulseTransmit
*
* 功能描述: 校时脉冲转发功能
*
* 输入参数: uiPulseTransmit bit0:0：不支持,1：支持
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetPPMAdjPulseTransmit(uint32_t uiPulseTransmit)
{
    WriteReg(REG_PPM_ADJ_PULSE_TRANSMIT, (uiPulseTransmit&0x0001));
}

/*
***********************************************************************************************************************
* 函数名称: SysSetPPMAdjPulsePolarity
*
* 功能描述: GPS校时脉冲极性选择
*
* 输入参数: uiPulsePolarity bit0:0：正脉冲,1：负脉冲
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetPPMAdjPulsePolarity(uint32_t uiPulsePolarity)
{
    WriteReg(REG_PPM_GPS_ADJ_PULSE_POLARITY, (uiPulsePolarity&0x0001));
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPPSCounterInitVal
*
* 功能描述: PPS计数初值
*
* 输入参数: uiInitVal bit5-0:当前分钟内秒值
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetPPSCounterInitVal(uint32_t uiInitVal)
{
    WriteReg(REG_PPM_PPS_COUNTER_INIT_VAL, (uiInitVal&0x003F));
    printf( "SysSetPPSCounterInitVal: %d\n", (uiInitVal & 0x003F));
}
/*
***********************************************************************************************************************
* 函数名称: SysSetPPSCounterInitValEnable
*
* 功能描述: PPS计数初值设定使能
*
* 输入参数: uiState bit0:0：无效,1：使能PPS计数初值设定
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SysSetPPSCounterInitValEnable(uint32_t uiState)
{
    WriteReg(REG_PPM_PPS_COUNTER_INIT_VAL_EN, (uiState&0x0001));
    printf( "SysSetPPSCounterInitValEnable: %d\n", (uiState & 0x0001));
}

/*
***********************************************************************************************************************
* 函数名称: SysGetPMOnlineFlag
*
* 功能描述: PM在线标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : PM在线标志
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t SysGetPMOnlineFlag(void)
{
    uint32_t uiRegVal = 0;

    uiRegVal = ReadReg(REG_PM_ONLINE_FLAG);

    return uiRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetMonitorState
*
* 功能描述: 读状态监控寄存器
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 状态监控寄存器的值
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t SysGetMonitorState(void)
{
    uint32_t uiRegVal = 0;

    uiRegVal = ReadReg(REG_MONITOR_STATE);

    return uiRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SysGetMonitorTemperature
*
* 功能描述: 读温度
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 温度值
*
* 注意事项:
***********************************************************************************************************************
*/
//uint32_t SysGetMonitorTemperature(void)
//{
//    uint32_t uiRegVal = 0;

//    uiRegVal = ReadReg(REG_MONITOR_TEMPERATURE);

//    return uiRegVal;
//}

/*
***********************************************************************************************************************
* 函数名称: SysGetMCUState
*
* 功能描述: 读MCU状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : MCU状态
*
* 注意事项:
***********************************************************************************************************************
*/
//uint32_t SysGetMCUState(void)
//{
//    uint32_t uiRegVal = 0;

//    uiRegVal = ReadReg(REG_MCU_STATE);

//    return uiRegVal;
//}

/*
***********************************************************************************************************************
* 函数名称: SysGetAlarm
*
* 功能描述: 读报警寄存器
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 报警寄存器值
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t SysGetAlarm(void)
{
    uint32_t uiRegVal = 0;

    uiRegVal = ReadReg(REG_ALARM);

    return uiRegVal;
}

/*
***********************************************************************************************************************
* 函数名称: SetTrimEnable
*
* 功能描述: 使能/禁止Trimble串口
*
* 输入参数: bEnable：使能标志位
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetTrimEnable( bool_t bEnable )
{
    uint32_t uiEnable = 0U;
    if( bEnable )
    {
        uiEnable = 1U;
    }

    WriteReg((uint32_t)(REG_TRIM_ENABLE), uiEnable );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetTrimBaudRate
*
* 功能描述: 设置Trimble串口波特率
*
* 输入参数: ucBaudRate：波特率, bit3-0: 0(1200) - 7(115200)
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetTrimBaudRate( uint8_t ucBaudRate )
{
    WriteReg((uint32_t)(REG_TRIM_BAUD_RATE), (uint32_t)(ucBaudRate & 0x07));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetTrimDataBits
*
* 功能描述: 设置Trimble串口数据位
*
* 输入参数: ucDataBits：数据位, bit0: 0(7bit)-1(8bit)
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetTrimDataBits( uint8_t ucDataBits )
{
    WriteReg((uint32_t)(REG_TRIM_DATA_BITS), (uint32_t)(ucDataBits & 0x01));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetTrimParity
*
* 功能描述: 设置Trimble奇偶校验
*
* 输入参数: ucParity：奇偶校验, 0: None, 1: Odd, 2: EVEN
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetTrimParity( uint8_t ucParity )
{
    if( ucParity <= 2 )
    {
        WriteReg((uint32_t)(REG_TRIM_PARITY), ucParity );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetTrimStopBits
*
* 功能描述: 设置Trimble停止位
*
* 输入参数: ucStopBits：停止位, bit0: 0(1 bit), 1(2 bit)
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetTrimStopBits( uint8_t ucStopBits )
{
    WriteReg((uint32_t)(REG_TRIM_STOP_BITS), (uint32_t)(ucStopBits & 0x01));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ResetGPSRecvStatus
*
* 功能描述: 清Trimble串口接收错误状态 - Bit 1：写1清
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

void ResetGPSRecvStatus( void )
{
    uint32_t uiStatus = 0x02;  /* Bit1 */
    WriteReg((uint32_t)(REG_TRIM_RECV_STS), uiStatus );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetGPSRecvStatus
*
* 功能描述: 获取 Trimble串口接收状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : Trimble串口接收状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetGPSRecvStatus( void )
{
    uint16_t usRecvStatus = 0U;

    usRecvStatus = (uint16_t)ReadReg((uint32_t)(REG_TRIM_RECV_STS));

    return usRecvStatus;
}

/*
***********************************************************************************************************************
* 函数名称: IsGPSPortRecvedFrame
*
* 功能描述: 判断 Trimble串口是否接收到报文
*
* 输入参数: usRecvStatus：接收状态
*
* 输出参数: 无
*
* 返 回 值  : 是否接收到报文标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsGPSPortRecvedFrame( uint16_t usRecvStatus )
{
    bool_t bHasFrame = false;

    if( usRecvStatus & (uint16_t)(GPS_PORT_HAS_FRAME_BIT))
    {
        bHasFrame = true;
    }

    return bHasFrame;
}

/*
***********************************************************************************************************************
* 函数名称: IsGPSPortRecvedError
*
* 功能描述: 判断 Trimble串口是否发生接收错误
*
* 输入参数: usRecvStatus：接收状态
*
* 输出参数: 无
*
* 返 回 值  : 是否接收错误标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsGPSPortRecvedError( uint16_t usRecvStatus )
{
    bool_t bRecvError = false;

    if( usRecvStatus & (uint16_t)(GPS_PORT_RECV_ERROR_BIT))
    {
        bRecvError = true;
    }

    return bRecvError;
}

/*
***********************************************************************************************************************
* 函数名称: IsGPSPortRecvFIFOEmpty
*
* 功能描述: 判断Trimble串口接收FIFO是否为空
*
* 输入参数: usRecvStatus：接收状态, bit2: 0-Empty; 1-Not Empty
*
* 输出参数: 无
*
* 返 回 值  : 空标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t IsGPSPortRecvFIFOEmpty( uint16_t usRecvStatus )
{
    bool_t bEmpty = true;

    if( usRecvStatus & (uint16_t)(GPS_PORT_RECV_FIFO_BIT))
    {
        bEmpty = false;
    }

    return bEmpty;
}

/*
***********************************************************************************************************************
* 函数名称: GetGPSPortRecvData
*
* 功能描述: 获取Trimble串口接收数据：逐字节接收
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 单字节数据
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint8_t GetGPSPortRecvData( void )
{
    uint8_t ucData = 0U;

    ucData = (uint8_t)ReadReg((uint32_t)(REG_TRIM_RECV_BUFF));

    return ucData;
}

/*
***********************************************************************************************************************
* 函数名称: GPSPortRecv
*
* 功能描述: Trimble串口接收校时包
*
* 输入参数: pucBuff：接收缓冲区首地址；
*          pusLen：接收缓冲区的长度。
*
* 输出参数: pusLen：返回数据长度
*
* 返 回 值  : 接收成功标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t GPSPortRecv( uint8_t pucBuff[], uint16_t *pusLen )
{
    bool_t bSuccess = false;
    uint8_t ucIndex = 0U;
    uint16_t usRecvStatus = 0U;

    if((pucBuff != NULL) &&
       (pusLen != NULL))
    {
        usRecvStatus = GetGPSRecvStatus();
        printf( "GPSRecvStatus 0x%04X\n", usRecvStatus );

        /* Check if has received a new frame */
        if( IsGPSPortRecvedFrame( usRecvStatus ))
        {
            if( !IsGPSPortRecvedError( usRecvStatus ))
            {
                /* Receive Successfully */
                while( !IsGPSPortRecvFIFOEmpty( usRecvStatus ))
                {
                    pucBuff[ucIndex] = GetGPSPortRecvData();
                    ucIndex++;

                    /* Update GPSRecvStatus */
                    usRecvStatus = GetGPSRecvStatus();
                    printf( "GPSRecvStatus 0x%04X\n", usRecvStatus );
                }

                *pusLen = ucIndex;
                bSuccess = true;
            }
            else
            {
                printf( "GPS Received Error.\n" );
            }

            /* Reset Receive Status */
            ResetGPSRecvStatus();
        }
        else
        {
            *pusLen = 0U;
            bSuccess = true;
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: GetSysTimeReg1
*
* 功能描述: 获取系统时间寄存器1的值, Bit 15-0
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 寄存器值
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetSysTimeReg1( void )
{
    uint16_t usTime = 0U;

    usTime = (uint16_t)ReadReg((uint32_t)(REG_SYSTIME_REG1));

    return usTime;
}

/*
***********************************************************************************************************************
* 函数名称: GetSysTimeReg2
*
* 功能描述: 获取系统时间寄存器2的值, Bit 31-16
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 寄存器值
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetSysTimeReg2( void )
{
    uint16_t usTime = 0U;

    usTime = (uint16_t)ReadReg((uint32_t)(REG_SYSTIME_REG2));

    return usTime;
}

/*
***********************************************************************************************************************
* 函数名称: GetSysTimeReg3
*
* 功能描述: 获取系统时间寄存器3的值, Bit 47-32
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 寄存器值
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetSysTimeReg3( void )
{
    uint16_t usTime = 0U;

    usTime = (uint16_t)ReadReg((uint32_t)(REG_SYSTIME_REG3));

    return usTime;
}

/*
***********************************************************************************************************************
* 函数名称: GetSysTimeReg4
*
* 功能描述: 获取系统时间寄存器4的值, Bit 63-48
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 寄存器值
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetSysTimeReg4( void )
{
    uint16_t usTime = 0U;

    usTime = (uint16_t)ReadReg((uint32_t)(REG_SYSTIME_REG4));

    return usTime;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysTimeAdjReg1
*
* 功能描述: 设置系统时间校准寄存器1, Bit 15-0
*
* 输入参数: usTime：时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysTimeAdjReg1( uint16_t usTime )
{
    WriteReg((uint32_t)(REG_SYSTIME_ADJ_REG1), (uint32_t)(usTime));
    printf( "SetSysTimeAdjReg1: 0x%04X\n", usTime );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysTimeAdjReg2
*
* 功能描述: 设置系统时间校准寄存器2, Bit 31-16
*
* 输入参数: usTime：时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysTimeAdjReg2( uint16_t usTime )
{
    WriteReg((uint32_t)(REG_SYSTIME_ADJ_REG2), (uint32_t)(usTime));
    printf( "SetSysTimeAdjReg2: 0x%04X\n", usTime );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysTimeAdjReg3
*
* 功能描述: 设置系统时间校准寄存器3, Bit 47-32
*
* 输入参数: usTime：时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysTimeAdjReg3( uint16_t usTime )
{
    WriteReg((uint32_t)(REG_SYSTIME_ADJ_REG3), (uint32_t)(usTime));
    printf( "SetSysTimeAdjReg3: 0x%04X\n", usTime );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysTimeAdjReg4
*
* 功能描述: 设置系统时间校准寄存器4, Bit 63-48
*
* 输入参数: usTime：时间
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysTimeAdjReg4( uint16_t usTime )
{
    WriteReg((uint32_t)(REG_SYSTIME_ADJ_REG4), (uint32_t)(usTime));
    printf( "SetSysTimeAdjReg4: 0x%04X\n", usTime );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysTimeAdjEnReg
*
* 功能描述: 设置系统时间校准使能寄存器
*
* 输入参数: usEn：使能标志位，bit 0: 0-Invalid; 1-Enable
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysTimeAdjEnReg( uint16_t usEn )
{
    WriteReg((uint32_t)(REG_SYSTIME_ADJ_EN), (uint32_t)(usEn & 0x01));

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSysAdjTime
*
* 功能描述: 设置系统校准时间
*
* 输入参数: llTime：时间，单位us
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void SetSysAdjTime( long64_t llTime )
{
    uint16_t usAdjEn = 1;

    SetSysTimeAdjReg1((uint16_t)(llTime & 0xFFFF));          /* Bit 15-0 */
    SetSysTimeAdjReg2((uint16_t)((llTime >> 16) & 0xFFFF));  /* Bit 31-16 */
    SetSysTimeAdjReg3((uint16_t)((llTime >> 32) & 0xFFFF));  /* Bit 47-32 */
    SetSysTimeAdjReg4((uint16_t)((llTime >> 48) & 0xFFFF));  /* Bit 63-48 */

    SetSysTimeAdjEnReg( usAdjEn );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: WatchdogEnable
*
* 功能描述: 看门狗使能
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
void WatchdogEnable(void)
{
    WriteReg(REG_WATCHDOG_EN, (uint32_t)WATCHDOG_ENABLE);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: WatchdogCfgPermission
*
* 功能描述: 看门狗配置使能
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
void WatchdogCfgPermission(void)
{
    WriteReg(REG_WATCHDOG_PERMISSION, (uint32_t)WATCHDOG_PERMISSION_ENABLE);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetWatchdogLimitLow
*
* 功能描述: 设置看门狗门限值
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
void SetWatchdogLimitLow(uint32_t uiLimitVal)
{
    WriteReg(REG_WATCHDOG_LIMIT_LOW, (uint32_t)uiLimitVal);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetWatchdogLimitHigh
*
* 功能描述: 设置看门狗门限值
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
void SetWatchdogLimitHigh(uint32_t uiLimitVal)
{
    WriteReg(REG_WATCHDOG_LIMIT_HIGH, (uint32_t)uiLimitVal);
    return;
}
/*
***********************************************************************************************************************
* 函数名称: GetSystemtime
*
* 功能描述: 获取系统时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 系统时间（单位：微秒）
*
* 注意事项:
*
***********************************************************************************************************************
*/
ulong64_t GetSystemtime(void)
{
    ulong64_t ulTime = 0;
    ulong64_t ulTime1 = 0;
    ulong64_t ulRegVal = 0;

    do
    {
        /* 读一次，若最低16位快要溢出时，则等待其溢出完成 */
        ulTime1 = ReadReg(REG_SYSTIME_REG1);
        ulRegVal = ReadReg(REG_SYSTIME_REG2);
        ulTime = ulTime1|(ulRegVal<<16);
        ulRegVal = ReadReg(REG_SYSTIME_REG3);
        ulTime = ulTime|(ulRegVal<<32);
        ulRegVal = ReadReg(REG_SYSTIME_REG4);
        ulTime = ulTime|(ulRegVal<<48);
    }while(ulTime1 > 65525);

    return ulTime;
}
/*
***********************************************************************************************************************
* 函数名称: SetTimingPulse
*
* 功能描述: 设置校时脉冲时间校准寄存器并使能(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
void SetTimingPulse(void)
{
    uint32_t uiTime = 0;
    ulong64_t ulTime = GetSystemtime();
    uint16_t usTime[2] = {0};

    uiTime = ulTime % (60*1000000);
    
    usTime[0] = uiTime & 0xffff;
    usTime[1] = (uiTime >> 16) & 0xffff;

    WriteReg(REG_ADJUST_TIME1, usTime[0]);
    WriteReg(REG_ADJUST_TIME2, usTime[1]);
    WriteReg(REG_ADJUST_TIME_ENABLE, 0x0001);
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetTimingPulse
*
* 功能描述: 获取校时脉冲时间校准寄存器的值(单位 微秒)
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项:
***********************************************************************************************************************
*/
uint32_t GetTimingPulse(void)
{
    uint32_t uiTime = 0;
    uint32_t uiRegVal = 0;

    uiTime = ReadReg(REG_ADJUST_TIME1);
    uiRegVal = ReadReg(REG_ADJUST_TIME2);
    uiTime = uiTime | (uiRegVal << 16);

    return uiTime;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: ReadReg
*
* 功能描述: 读寄存器
*
* 输入参数: 寄存器偏移地址
*
* 输出参数: 无
*
* 返 回 值  : 寄存器的值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ReadReg(uint32_t uiRegAddr)
{
    uint32_t uiContent = 0;
    uint32_t uiVal = 0;

    uiContent |= (uiRegAddr & 0xFFFF);

    if(s_ifdlocalbus > 0)
    {
        ioctl(s_ifdlocalbus, LOCALBUS_READ, &uiContent);
    }

    uiVal = (uiContent);

    return uiVal;
}

/*
***********************************************************************************************************************
* 函数名称: WriteReg
*
* 功能描述: 写寄存器
*
* 输入参数: uiRegAddr 寄存器偏移地址
*        uiVal 写入的内容
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void WriteReg(uint32_t uiRegAddr, uint32_t uiVal)
{
    uint32_t uiContent = 0;

    uiContent = (uiVal << 16) | (uiRegAddr & 0xFFFF);

    if(s_ifdlocalbus > 0)
    {
        ioctl(s_ifdlocalbus, LOCALBUS_WRITE, &uiContent);
    }

    return;
}


/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
