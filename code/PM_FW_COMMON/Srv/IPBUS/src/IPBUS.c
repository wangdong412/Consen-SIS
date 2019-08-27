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
*  文件名称    : IPBUS.c
*
*  功能简介    : IPBUS协议相关处理
*
*  作者        : 朱耿华
*
*  创建日期    : 2016-06-10
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
#include <Srv/Scheduler/cagos.h>
#include "../inc/IPBUS.h"
//#include "Srv/SharedSys/inc/Sys.h"
#include "Srv/Sys/inc/CommSysConfig.h"
#include "Srv/Sys/inc/CommSysCommon.h"
#include "Srv/Algrithm/inc/Algrithm.h"
#include "Srv/Scheduler/include/string.h"
#include "Srv/SharedMem/inc/SharedConfig.h"
#include "inc/Hardware.h"

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
static void IPBUSSlaveInputAddrInit(void);

static void IPBUSSlaveInputLenInit(void);

static void IPBUSSlaveOutputAddrInit(void);

static void IPBUSSlaveOutputLenInit(void);

static void IPBUSSlavePrmAddrInit(uint8_t ucBufID);

static void IPBUSSlavePrmLenInit(uint8_t ucBufID);

static void IPBUSSetAPPMailRX(uint16_t usvalue);

static void IPBUSSetAPPMailTX(uint16_t usvalue);

static void IPBUSSetAPPMailNR(uint16_t usvalue);

static void IPBUSSetAPPMailSlaveAddr(uint16_t usvalue);

static void IPBUSSetAPPMailFunc(uint16_t usvalue);

static void IPBUSSetAPPMailLN(uint16_t usvalue);

static void IPBUSSetAPPMailACK(uint16_t usvalue);

static void IPBUSSetAPPMailFault(uint16_t usvalue);

static uint16_t IPBUSGetDeviceMailRX(void);

static uint16_t IPBUSGetDeviceMailTX(void);

static uint16_t IPBUSGetDeviceMailNR(void);

static uint16_t IPBUSGetDeviceMailSlaveAddr(void);

static uint16_t IPBUSGetDeviceMailFunc(void);

static uint16_t IPBUSGetDeviceMailLN(void);

static uint16_t IPBUSGetDeviceMailACK(void);

static uint16_t IPBUSGetDeviceMailFault(void);

static void IPBUSSetAPPWatchdog(uint16_t usAPPWatchdog);

static uint16_t IPBUSGetAPPWatchdog(void);

static uint16_t IPBUSGetDeviceWatchdog(void);

static void IPBUSMemCopyByLocalbus(uint32_t uiRegAddr, uint16_t usData[], uint16_t uiSize);

static void IPBUSSetAPPMark(uint16_t usMark);

static uint16_t IPBUSGetAPPMark(void);

static uint16_t IPBUSGetDeviceMark(void);




/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: IPBUSRamInit
*
* 功能描述: IPBUS初始化，初始化需要用户初始化的寄存器
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSDualRamInit(void)
{
    uint8_t ucInit256Buf[256] = {0};
    uint8_t ucInit32Buf[32] = {0};

    /* 清从站周期轮询列表 */
    IPBUSSetSlaveRollingList(&ucInit256Buf[0], IPBUS_SLAVE_MAX_NUM);

    /* 清从站使能标志（buffer1） */
    IPBUSSetSlaveEnableBuf(&ucInit32Buf[0], IPBUS_SLAVE_ENABLE_BUF0);
    /* 清从站使能标志（buffer2） */
    IPBUSSetSlaveEnableBuf(&ucInit32Buf[0], IPBUS_SLAVE_ENABLE_BUF1);

    /* 清在线下装从站参数变化标志 */
    IPBUSSetSlavePRMChange(&ucInit32Buf[0]);

    /* 清从站输入区基地址 */
    IPBUSSlaveInputAddrInit();

    /* 清从站输入区长度 */
    IPBUSSlaveInputLenInit();

    /* 清从站输出区基地址 */
    IPBUSSlaveOutputAddrInit();

    /* 清从站输出区长度 */
    IPBUSSlaveOutputLenInit();

    /* 清从站参数区基地址 */
    IPBUSSlavePrmAddrInit(IPBUS_SLAVE_ENABLE_BUF0);

    IPBUSSlavePrmAddrInit(IPBUS_SLAVE_ENABLE_BUF1);

    /* 清从站参数区长度 */
    IPBUSSlavePrmLenInit(IPBUS_SLAVE_ENABLE_BUF0);

    IPBUSSlavePrmLenInit(IPBUS_SLAVE_ENABLE_BUF1);


    return;
}



/*
***********************************************************************************************************************
* 函数名称: IPBUSMailSend
*
* 功能描述: 邮箱发送操作
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSMailSend(uint8_t ucMailSend[], IPBUSAPPMailHeader_t *pstMailHeader)
{
    uint8_t ucRet = IPBUS_RET_NOTOK;
    uint16_t ucDeviceState = 0U;
    uint8_t ucRight = IPBUS_RIGHTS_BUSY;

    if(pstMailHeader != NULL)
    {
        /* 读取设备层状态 */
        ucDeviceState = IPBUSGetDeviceState();
        if(ucDeviceState != DEVICE_STATE_RUN)
        {/* IPBUS主卡未处于运行状态 */
            ucRet = IPBUS_RET_NOTOK;
        }
        else
        {
            ucRight = IPBUSGetSendMailRight();
            if (ucRight == IPBUS_RIGHTS_FREE)
            {/*有发送邮箱权限*/
                /*填充发送邮箱*/
                ipbus_write(WRITE_SENDBOX, &ucMailSend[0], 0, pstMailHeader->usMailLN);

                /*填写应用层邮箱信息头*/
                IPBUSSetAPPMailRX(pstMailHeader->usMailRX);
                IPBUSSetAPPMailTX(pstMailHeader->usMailTX);
                IPBUSSetAPPMailNR(pstMailHeader->usMailNR);
                IPBUSSetAPPMailSlaveAddr(pstMailHeader->usMailSlaveAddr);
                IPBUSSetAPPMailFunc(pstMailHeader->usMailFunc);
                IPBUSSetAPPMailLN(pstMailHeader->usMailLN);
                IPBUSSetAPPMailACK(pstMailHeader->usMailACK);
                IPBUSSetAPPMailFault(pstMailHeader->usMailFault);

                /*释放发送邮箱权限*/
                IPBUSReleaseSendMailRight();

                ucRet = IPBUS_RET_OK;
            }
            else
            {
                ucRet = IPBUS_RET_NORIGHT;
            }
        }
    }
    
    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSMailRecv
*
* 功能描述: 邮箱接收操作
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSMailRecv(uint8_t ucMailRecv[], IPBUSAPPMailHeader_t *pstMailHeader)
{
    uint8_t ucRet = IPBUS_RET_NOTOK;
    uint16_t ucDeviceState = 0U;
    uint8_t ucRight = IPBUS_RIGHTS_BUSY;

    if(pstMailHeader != NULL)
    {
        ucDeviceState = IPBUSGetDeviceState(); /* 读取设备层状态 */

        if(ucDeviceState != DEVICE_STATE_RUN)
        {/* IPBUS主卡未处于运行状态 */
            ucRet = IPBUS_RET_NOTOK;
        }
        else
        {
            ucRight = IPBUSGetRecieveMailRight();
            if (ucRight == IPBUS_RIGHTS_FREE)
            {/*有接收邮箱权限*/
                /*读取设备层邮箱协议头*/
                pstMailHeader->usMailRX = IPBUSGetDeviceMailRX();
                pstMailHeader->usMailTX = IPBUSGetDeviceMailTX();
                pstMailHeader->usMailNR = IPBUSGetDeviceMailNR();
                pstMailHeader->usMailSlaveAddr = IPBUSGetDeviceMailSlaveAddr();
                pstMailHeader->usMailFunc = IPBUSGetDeviceMailFunc();
                pstMailHeader->usMailLN = IPBUSGetDeviceMailLN();
                pstMailHeader->usMailACK = IPBUSGetDeviceMailACK();
                pstMailHeader->usMailFault = IPBUSGetDeviceMailFault();

                /*根据消息长度，读取接收邮箱内容*/
                ipbus_read(READ_RECVBOX, &ucMailRecv[0], 0, pstMailHeader->usMailLN);

                /*释放接收邮箱权限*/
                IPBUSReleaseRecieveMailRight();

                ucRet = IPBUS_RET_OK;
            }
            else
            {
                ucRet = IPBUS_RET_NORIGHT;
            }
        }
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDataRAMRight
*
* 功能描述: 获取IPBUS主卡数据区权限状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetDataRAMRight(void)
{
    uint8_t ucRet = IPBUS_RIGHTS_BUSY;
    uint16_t ucDeviceMark = 0U;
    uint16_t usAPPMark = 0U;

    ucDeviceMark = fpga_read(REG_IPBUS_DEVICE_MARK); /* 读取设备层标志 */
    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */
    if (((ucDeviceMark ^ usAPPMark) & DATARAM_MASK) == DATARAM_MASK)
    {
        ucRet = IPBUS_RIGHTS_BUSY;
    }
    else
    {
        ucRet = IPBUS_RIGHTS_FREE;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSReleaseDataRAMRight
*
* 功能描述: 释放IPBUS主卡数据区RAM权限
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSReleaseDataRAMRight(void)
{
    uint16_t usAPPMark = 0U;

    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */
    usAPPMark = usAPPMark ^ DATARAM_MASK;
    fpga_write(REG_IPBUS_APP_MARK, usAPPMark);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetSendMailRight
*
* 功能描述: 获取IPBUS主卡发送邮箱区权限状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetSendMailRight(void)
{
    uint8_t ucRet = IPBUS_RIGHTS_BUSY;
    uint16_t ucDeviceMark = 0U;
    uint16_t usAPPMark = 0U;

    ucDeviceMark = fpga_read(REG_IPBUS_DEVICE_MARK); /* 读取设备层标志 */
    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */

    if(((ucDeviceMark ^ usAPPMark) & MAILSEND_MASK) == MAILSEND_MASK)
    {/* 设备层标志和应用层标志中的发送邮箱权限不同 */
        ucRet = IPBUS_RIGHTS_BUSY;
    }
    else
    {
        ucRet = IPBUS_RIGHTS_FREE;
    }


    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSReleaseSendMailRight
*
* 功能描述: 释放IPBUS主卡发送邮箱区权限
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSReleaseSendMailRight(void)
{
    uint16_t usAPPMark = 0U;

    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */
    usAPPMark = usAPPMark ^ MAILSEND_MASK;
    fpga_write(REG_IPBUS_APP_MARK, usAPPMark);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetRecieveMailRight
*
* 功能描述: 获取IPBUS主卡接收邮箱区权限状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetRecieveMailRight(void)
{
    uint8_t ucRet = IPBUS_RIGHTS_BUSY;
    uint16_t ucDeviceMark = 0U;
    uint16_t usAPPMark = 0U;

    ucDeviceMark = fpga_read(REG_IPBUS_DEVICE_MARK); /* 读取设备层标志 */
    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */

    if(((ucDeviceMark ^ usAPPMark) & MAILRECEIVE_MASK) == MAILRECEIVE_MASK)
    {/* 设备层标志和应用层标志中的发送邮箱权限不同 */
        ucRet = IPBUS_RIGHTS_FREE;
    }
    else
    {
        ucRet = IPBUS_RIGHTS_BUSY;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSReleaseRecieveMailRight
*
* 功能描述: 释放IPBUS主卡接收邮箱区权限
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSReleaseRecieveMailRight(void)
{
    uint16_t usAPPMark = 0U;

    usAPPMark = fpga_read(REG_IPBUS_APP_MARK); /* 读取应用层标志 */
    usAPPMark = usAPPMark ^ MAILRECEIVE_MASK;
    fpga_write(REG_IPBUS_APP_MARK, usAPPMark);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetMasterAddr
*
* 功能描述: 设置主站地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetMasterAddr(void)
{
    fpga_write(REG_IPBUS_MASTER_ADDR, IPBUS_MASTER_ADDR);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetRollingInterval
*
* 功能描述: 设置周期轮询时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetRollingInterval(void)
{
    fpga_write(REG_IPBUS_POLLING_INTERVAL, IPBUS_POLLING_INTERVAL);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveDelayMin
*
* 功能描述: 设置最小从站延迟时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveDelayMin(uint16_t usDelayTime)
{
    fpga_write(REG_IPBUS_SLAVE_DELAY_MIN, usDelayTime);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveDelayMax
*
* 功能描述: 设置最大从站延迟时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveDelayMax(uint16_t usDelayTime)
{
    fpga_write(REG_IPBUS_SLAVE_DELAY_MAX, usDelayTime);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveAckTimeout
*
* 功能描述: 设置从站应答超时时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveAckTimeout(uint16_t usTimeout)
{
    fpga_write(REG_IPBUS_SLAVE_ACK_TIMEOUT, usTimeout);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetMasterMailTimeout
*
* 功能描述: 设置非周期轮询应答超时时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetMasterMailTimeout(uint16_t usTimeout)
{
    fpga_write(REG_IPBUS_MASTER_MAIL_TIMEOUT, usTimeout);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetRetransmissionNumber
*
* 功能描述: 设置最大重传次数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetRetransmissionNumber(uint16_t usNumber)
{
    fpga_write(REG_IPBUS_RETRANSMISSION_NR, usNumber);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetFaultConter
*
* 功能描述: 读取总线故障计数器
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t IPBUSGetFaultConter(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_FAULT_COUNTER);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetWatchdogTimeout
*
* 功能描述: 设置看门狗超时时间
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetWatchdogTimeout(uint16_t usTimeout)
{
    fpga_write(REG_IPBUS_WATCHDOG_TIMEOUT, usTimeout);

    return;
}


/*
***********************************************************************************************************************
* 函数名称: IPBUSMonitorMasterWatchdog
*
* 功能描述: 根据看门狗监视主卡
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t IPBUSMonitorMasterWatchdog(void)
{
    uint8_t ucRet = 0U;
    uint16_t usAPPWatchdog = 0U;
    uint16_t usDeviceWatchdog = 0U;

    usAPPWatchdog = IPBUSGetAPPWatchdog();
    usDeviceWatchdog = IPBUSGetDeviceWatchdog();

    if(usAPPWatchdog == usDeviceWatchdog)
    {/*应用层看门狗和设备看门狗一致，表明主卡工作正常*/
        ucRet = IPBUS_RET_OK;
    }
    else
    {
        ucRet = IPBUS_RET_NOTOK;
    }
    /*无论设备看门狗是否正常，应用层看门狗都加1后再次写入*/
    usAPPWatchdog = usAPPWatchdog + 1;
    IPBUSSetAPPWatchdog(usAPPWatchdog);

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPState
*
* 功能描述: 设置应用层状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetAPPState(uint16_t usState)
{

    fpga_write(REG_IPBUS_APP_STATE, usState);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetAPPState
*
* 功能描述: 读取应用层状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t IPBUSGetAPPState(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_APP_STATE);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceState
*
* 功能描述: 读取设备层状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint16_t IPBUSGetDeviceState(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_STATE);

    return usRet;
}


/*
***********************************************************************************************************************
* 函数名称: IPBUSResetMaster
*
* 功能描述: 复位主卡
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSResetMaster(void)
{
    fpga_write(REG_IPBUS_RESET_MASTER, IPBUS_RESET_MASTER_VALUE);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetPrmBufID
*
* 功能描述: 设置应用层标志bit5，即使用的参数区ID号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetPrmBufID(uint8_t ucBufID)
{
    uint16_t usAPPMark = 0U;

    if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 使用参数区0 */
        usAPPMark = usAPPMark & (~APP_PRMRAM_MASK);
        IPBUSSetAPPMark(usAPPMark);
    }
    else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 使用参数区1 */
        usAPPMark = usAPPMark | APP_PRMRAM_MASK;
        IPBUSSetAPPMark(usAPPMark);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPConfigDeviceFlag
*
* 功能描述: 设置应用层标志bit1，即设置是否完成设备层配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetAPPConfigDeviceFlag(uint8_t ucConfigFlag)
{
    uint16_t usAPPMark = 0U;

    if (ucConfigFlag == IPBUS_APP_NOCONFIG_DEVICE)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 使用参数区0 */
        usAPPMark = usAPPMark & (~APP_CONFIG_MASK);
        IPBUSSetAPPMark(usAPPMark);
    }
    else if (ucConfigFlag == IPBUS_APP_CONFIG_DEVICE)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 使用参数区1 */
        usAPPMark = usAPPMark | APP_CONFIG_MASK;
        IPBUSSetAPPMark(usAPPMark);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPOnlineFlag
*
* 功能描述: 设置应用层标志bit0，即应用层离线在线标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetAPPOnlineFlag(uint8_t ucOnlineFlag)
{
    uint16_t usAPPMark = 0U;

    if (ucOnlineFlag == IPBUS_APP_OFFLINE)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 置离线标志 */
        usAPPMark = usAPPMark & (~APP_ACTIVE_MASK);
        IPBUSSetAPPMark(usAPPMark);
    }
    else if (ucOnlineFlag == IPBUS_APP_ONLINE)
    {
        /* 读取应用层标志 */
        usAPPMark = IPBUSGetAPPMark();
        /* 置在线标志 */
        usAPPMark = usAPPMark | APP_ACTIVE_MASK;
        IPBUSSetAPPMark(usAPPMark);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceConfigSlaveFlag
*
* 功能描述: 读取设备层标志bit1，即判断设备层是否完成从站配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetDeviceConfigSlaveFlag(void)
{
    uint8_t ucRet = IPBUS_DEVICE_NOCONFIG_SLAVE;
    uint16_t usDeviceMark = 0U;

    usDeviceMark = IPBUSGetDeviceMark();

    if((usDeviceMark & DEVICE_CONFIG_MASK) == DEVICE_CONFIG_MASK)
    {/* 该位为1，设备层完成从站配置 */
        ucRet = IPBUS_DEVICE_CONFIG_SLAVE;
    }
    else
    {/* 该位为0，设备层未完成从站配置 */
        ucRet = IPBUS_DEVICE_NOCONFIG_SLAVE;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceOnlineFlag
*
* 功能描述: 读取设备层标志bit0，即判断设备层离线在线状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetDeviceOnlineFlag(void)
{
    uint8_t ucRet = IPBUS_DEVICE_OFFLINE;
    uint16_t usDeviceMark = 0U;

    usDeviceMark = IPBUSGetDeviceMark();

    if((usDeviceMark & DEVICE_ACTIVE_MASK) == DEVICE_ACTIVE_MASK)
    {/* 该位为1，设备层在线 */
        ucRet = IPBUS_DEVICE_ONLINE;
    }
    else
    {/* 该位为0，设备层离线 */
        ucRet = IPBUS_DEVICE_OFFLINE;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSAcylicWrite
*
* 功能描述: IPBUS非周期写操作
*
* 输入参数: 无
*       ucSlaveAddr: 从站号
*       ucParDataAdd:参数数据地址
*       ucParDataLen:参数数据长度
*       ucParDataLen:参数数据长度
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
/*邮箱具体处理待定*/
uint8_t IPBUSAcylicWrite(void)
{
    return 0;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSAcylicRead
*
* 功能描述: IPBUS非周期读操作
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
/*邮箱具体处理待定*/
uint8_t IPBUSAcylicRead(void)
{
    return 0;
}






/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveRollingList
*
* 功能描述: 填写从站周期轮询列表
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveRollingList(uint8_t ucData[], uint32_t uiSize)
{
    uint16_t usDataTemp[128] = {0U};
    uint16_t usSizeTemp = 0U;
    uint32_t i = 0U;

    if(uiSize <= IPBUS_SLAVE_MAX_NUM)
    {
        /*Localbus按照2字节操作，如果实际长度不为2的倍数，需要补齐*/
        if(uiSize % 2 != 0U)
        {
            usSizeTemp = (uiSize + 1)/2;
        }
        else
        {
            usSizeTemp = uiSize/2;
        }

        for(i = 0; i< usSizeTemp; i++)
        {
            usDataTemp[i] = ((uint16_t)ucData[2*i+1]<<8) + (uint16_t)ucData[2*i];
        }

        IPBUSMemCopyByLocalbus(REG_IPBUS_SLAVE_ROLLINGLIST, &usDataTemp[0], usSizeTemp);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveEnableBuf
*
* 功能描述: 填写从站使能标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveEnableBuf(uint8_t ucData[], uint8_t ucBufID)
{
    uint16_t usDataTemp[16] = {0U};
    uint16_t usSizeTemp = 0U;
    uint8_t i = 0U;

    /*Localbus按照2字节操作*/
    usSizeTemp = IPBUS_SLAVE_ENABLE_BUFSIZE/2;

    for(i = 0; i< usSizeTemp; i++)
    {
        usDataTemp[i] = ((uint16_t)ucData[2*i+1]<<8) + (uint16_t)ucData[2*i];
    }
    if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
    {
        IPBUSMemCopyByLocalbus(REG_IPBUS_SLAVE_ENABLE_BUF0, &usDataTemp[0], usSizeTemp);
    }
    else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
    {
        IPBUSMemCopyByLocalbus(REG_IPBUS_SLAVE_ENABLE_BUF1, &usDataTemp[0], usSizeTemp);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetSlaveOnlineState
*
* 功能描述: 获取从站在线状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetSlaveOnlineState(uint8_t ucSlaveNo)
{
    uint16_t usOnlineStat = 0U;

    uint32_t uiQuotient = 0U;
    uint32_t uiResidue  = 0U;
    uint16_t usReg = 0U;
    uint8_t ucRet = 0U;

    uiQuotient = ucSlaveNo / UINT16_BITS; /* 商 */
    uiResidue =  ucSlaveNo % UINT16_BITS; /* 余数 */

    usReg = IPBUS_REG_SIZE * uiQuotient + REG_IPBUS_SLAVE_ACTIVE;
    usOnlineStat = fpga_read(usReg);

    /* 判断从站在线标识位 */
    if((usOnlineStat & ((uint16_t)1U<<uiResidue)) == ((uint16_t)1U<<uiResidue))
    {/* 该位为1，在线 */
        ucRet = IPBUS_SLAVE_ONLINE;
    }
    else
    {
        ucRet = IPBUS_SLAVE_OFFLINE;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetSlaveValidState
*
* 功能描述: 获取从站数据有效无效状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetSlaveValidState(uint8_t ucSlaveNo)
{
    uint16_t usOnlineStat = 0U;

    uint32_t uiQuotient = 0U;
    uint32_t uiResidue  = 0U;
    uint16_t usReg = 0U;
    uint8_t ucRet = 0U;

    uiQuotient = ucSlaveNo / UINT16_BITS; /* 商 */
    uiResidue =  ucSlaveNo % UINT16_BITS; /* 余数 */

    usReg = IPBUS_REG_SIZE * uiQuotient + REG_IPBUS_SLAVE_VALID;
    usOnlineStat = fpga_read(usReg);

    /* 判断从站数据有效标识位 */
    if((usOnlineStat & ((uint16_t)1U<<uiResidue)) == ((uint16_t)1U<<uiResidue))
    {/* 该位为1，数据有效 */
        ucRet = IPBUS_SLAVE_VALID;
    }
    else
    {
        ucRet = IPBUS_SLAVE_INVALID;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetSlaveConfigState
*
* 功能描述: 获取从站参数配置完成标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint8_t IPBUSGetSlaveConfigState(uint8_t ucSlaveNo)
{
    uint16_t usOnlineStat = 0U;

    uint32_t uiQuotient = 0U;
    uint32_t uiResidue  = 0U;
    uint16_t usReg = 0U;
    uint8_t ucRet = 0U;

    uiQuotient = ucSlaveNo / UINT16_BITS; /* 商 */
    uiResidue =  ucSlaveNo % UINT16_BITS; /* 余数 */

    usReg = IPBUS_REG_SIZE * uiQuotient + REG_IPBUS_SLAVE_CONFIG;
    usOnlineStat = fpga_read(usReg);

    /* 判断从站数据有效标识位 */
    if((usOnlineStat & ((uint16_t)1U<<uiResidue)) == ((uint16_t)1U<<uiResidue))
    {/* 该位为1，数据有效 */
        ucRet = IPBUS_SLAVE_CONFIG;
    }
    else
    {
        ucRet = IPBUS_SLAVE_NOCONFIG;
    }

    return ucRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlavePRMChange
*
* 功能描述: 填写在线下装从站参数变化标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlavePRMChange(uint8_t ucData[])
{
    uint16_t usDataTemp[16] = {0U};
    uint16_t uiSizeTemp = 0U;
    uint8_t i = 0U;

    /*Localbus按照2字节操作*/
    uiSizeTemp = IPBUS_SLAVE_ENABLE_BUFSIZE/2;

    for(i = 0; i< uiSizeTemp; i++)
    {
        usDataTemp[i] = ((uint16_t)ucData[2*i+1]<<8) + (uint16_t)ucData[2*i];
    }

    IPBUSMemCopyByLocalbus(REG_IPBUS_SLAVE_PRM_CHANGE, &usDataTemp[0], uiSizeTemp);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveInputAddr
*
* 功能描述: 设置单个从站输入区基地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveInputAddr(uint16_t usSlaveNo, uint16_t usInputAddr)
{
    uint16_t usReg = 0U;

    usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_INPUTADDR;
    fpga_write(usReg, usInputAddr);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveInputLen
*
* 功能描述: 设置单个从站输入区长度
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveInputLen(uint16_t usSlaveNo, uint16_t usInputLen)
{
    uint16_t usReg = 0U;

    usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_INPUTLEN;
    fpga_write(usReg, usInputLen);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveOutputAddr
*
* 功能描述: 设置单个从站输出区基地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveOutputAddr(uint16_t usSlaveNo, uint16_t usOutputAddr)
{
    uint16_t usReg = 0U;

    usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_OUTPUTADDR;
    fpga_write(usReg, usOutputAddr);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlaveOutputLen
*
* 功能描述: 设置单个从站输入区长度
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlaveOutputLen(uint16_t usSlaveNo, uint16_t usOutputLen)
{
    uint16_t usReg = 0U;

    usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_OUTPUTLEN;
    fpga_write(usReg, usOutputLen);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlavePrmAddr
*
* 功能描述: 设置单个从站参数区基地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlavePrmAddr(uint16_t usSlaveNo, uint16_t usPrmAddr, uint8_t ucBufID)
{
    uint16_t usReg = 0U;

    if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
    {
        usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_PRMADDR_BUF0;
        fpga_write(usReg, usPrmAddr);
    }
    else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
    {
        usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_PRMADDR_BUF1;
        fpga_write(usReg, usPrmAddr);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetSlavePrmLen
*
* 功能描述: 设置单个从站参数区基地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void IPBUSSetSlavePrmLen(uint16_t usSlaveNo, uint16_t usPrmLen, uint8_t ucBufID)
{
    uint16_t usReg = 0U;

    if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
    {
        usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_PRMLEN_BUF0;
        fpga_write(usReg, usPrmLen);
    }
    else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
    {
        usReg = IPBUS_REG_SIZE * usSlaveNo + REG_IPBUS_SLAVE_PRMLEN_BUF1;
        fpga_write(usReg, usPrmLen);
    }
    else
    {
        ;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlaveInputAddrInit
*
* 功能描述: IPBUS从站输入区基地址初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlaveInputAddrInit(void)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo <= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        IPBUSSetSlaveInputAddr(usSlaveNo, IPBUS_INIT_VALUE);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlaveInputLenInit
*
* 功能描述: IPBUS从站输入区长度初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlaveInputLenInit(void)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo <= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        IPBUSSetSlaveInputLen(usSlaveNo, IPBUS_INIT_VALUE);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlaveOutputAddrInit
*
* 功能描述: IPBUS从站输出区基地址初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlaveOutputAddrInit(void)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo <= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        IPBUSSetSlaveOutputAddr(usSlaveNo, IPBUS_INIT_VALUE);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlaveOutputLenInit
*
* 功能描述: IPBUS从站输出区长度初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlaveOutputLenInit(void)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo<= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        IPBUSSetSlaveOutputLen(usSlaveNo, IPBUS_INIT_VALUE);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlavePrmAddrInit
*
* 功能描述: IPBUS从站参数区基地址初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlavePrmAddrInit(uint8_t ucBufID)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo<= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
        {
            IPBUSSetSlavePrmAddr(usSlaveNo, IPBUS_INIT_VALUE, IPBUS_SLAVE_ENABLE_BUF0);
        }
        else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
        {
            IPBUSSetSlavePrmAddr(usSlaveNo, IPBUS_INIT_VALUE, IPBUS_SLAVE_ENABLE_BUF1);
        }
        else
        {
            ;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSlavePrmLenInit
*
* 功能描述: IPBUS从站参数区长度初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSlavePrmLenInit(uint8_t ucBufID)
{
    uint16_t usSlaveNo = 0U;

    for(usSlaveNo = 0; usSlaveNo<= IPBUS_MAX_SLAVE_NO; usSlaveNo++)
    {
        if (ucBufID == IPBUS_SLAVE_ENABLE_BUF0)
        {
            IPBUSSetSlavePrmLen(usSlaveNo, IPBUS_INIT_VALUE, IPBUS_SLAVE_ENABLE_BUF0);
        }
        else if (ucBufID == IPBUS_SLAVE_ENABLE_BUF1)
        {
            IPBUSSetSlavePrmLen(usSlaveNo, IPBUS_INIT_VALUE, IPBUS_SLAVE_ENABLE_BUF1);
        }
        else
        {
            ;
        }
    }

    return;
}


/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailRX
*
* 功能描述: 设置应用层邮箱收件人
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailRX(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_RX, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailTX
*
* 功能描述: 设置应用层邮箱发件人
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailTX(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_TX, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailNR
*
* 功能描述: 设置应用层邮箱消息号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailNR(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_NR, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailSlaveAddr
*
* 功能描述: 设置应用层邮箱消息地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailSlaveAddr(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_SLAVEADDR, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailFunc
*
* 功能描述: 设置应用层邮箱消息服务码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailFunc(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_FUNC, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailLN
*
* 功能描述: 设置应用层邮箱消息长度
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailLN(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_LN, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailACK
*
* 功能描述: 设置应用层邮箱消息应答请求
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailACK(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_ACK, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMailFault
*
* 功能描述: 设置应用层邮箱消息错误码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMailFault(uint16_t usvalue)
{
    fpga_write(REG_IPBUS_APP_MAIL_FAULT, usvalue);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailRX
*
* 功能描述: 获取设备层邮箱收件人
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailRX(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_RX);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailTX
*
* 功能描述: 获取设备层邮箱发件人
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailTX(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_TX);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailNR
*
* 功能描述: 获取设备层邮箱消息号
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailNR(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_NR);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailSlaveAddr
*
* 功能描述: 获取设备层邮箱消息地址
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailSlaveAddr(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_SLAVEADDR);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailFunc
*
* 功能描述: 获取设备层邮箱消息服务码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailFunc(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_FUNC);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailLN
*
* 功能描述: 获取设备层邮箱消息长度
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailLN(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_LN);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailACK
*
* 功能描述: 获取设备层邮箱消息应答请求
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailACK(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_ACK);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMailFault
*
* 功能描述: 获取设备层邮箱消息错误码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMailFault(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MAIL_FAULT);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPWatchdog
*
* 功能描述: 应用层看门狗自增
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPWatchdog(uint16_t usAPPWatchdog)
{
    fpga_write(REG_IPBUS_APP_WATCHDOG, usAPPWatchdog);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetAPPWatchdog
*
* 功能描述: 读取应用层看门狗计数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetAPPWatchdog(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_APP_WATCHDOG);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceWatchdog
*
* 功能描述: 读取设备层看门狗计数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceWatchdog(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_WATCHDOG);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSMemCopyByLocalbus
*
* 功能描述: 通过Localbus拷贝整片数据
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSMemCopyByLocalbus(uint32_t uiRegAddr, uint16_t usData[], uint16_t uiSize)
{
    uint8_t i = 0U;
    uint16_t usReg = 0U;

    for(i = 0; i< uiSize; i++)
    {
        usReg = IPBUS_REG_SIZE * i + uiRegAddr;
        fpga_write(usReg, usData[i]);
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSSetAPPMark
*
* 功能描述: 设置应用层标志
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void IPBUSSetAPPMark(uint16_t usMark)
{

    fpga_write(REG_IPBUS_APP_MARK, usMark);

    return;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetAPPMark
*
* 功能描述: 读取应用层状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetAPPMark(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_APP_MARK);

    return usRet;
}

/*
***********************************************************************************************************************
* 函数名称: IPBUSGetDeviceMark
*
* 功能描述: 读取设备层状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint16_t IPBUSGetDeviceMark(void)
{
    uint16_t usRet = 0U;

    usRet = fpga_read(REG_IPBUS_DEVICE_MARK);

    return usRet;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

















