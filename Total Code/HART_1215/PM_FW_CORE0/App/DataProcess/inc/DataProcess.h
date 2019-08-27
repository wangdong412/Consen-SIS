/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    :
*
*  模块名称    : 数据处理模块
*
*  文件名称    : DataProcess.h
*
*  功能简介    : 完成输入输出数据处理
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
#ifndef DATAPROCESS_H_
#define DATAPROCESS_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
/* 拷贝数据成功 */
#define DATAPROCESS_COPY_OK        (0U)
/* 拷贝数据失败 */
#define DATAPROCESS_COPY_NOTOK     (1U)

/* 单个从站数据长度 */
#define DATAPROCESS_INPUT_MAX_LEN  (512U)
#define DATAPROCESS_OUTPUT_MAX_LEN  (512U)

/* 每周期轮询IO个数 */
#define DATAPROCESS_SLAVE_POLL_NUM_TASK1  (2U)
#define DATAPROCESS_SLAVE_POLL_NUM_TASK2  (1U)

/* 各类型从站安全实时数据长度 */
#define DATAPROCESS_DI_SAFETYDATA_LEN    (4U)
#define DATAPROCESS_AI_SAFETYDATA_LEN    (64U)
#define DATAPROCESS_OSP_SAFETYDATA_LEN   (44U)
#define DATAPROCESS_DO_SAFETYDATA_LEN    (4U)
#define DATAPROCESS_AO_SAFETYDATA_LEN    (32U)
#define DATAPROCESS_NO_SAFETYDATA_LEN    (0U)

/* 各类型输入从站质量位数据长度 */
#define DATAPROCESS_DI_QDATA_LEN         (4U)
#define DATAPROCESS_AI_QDATA_LEN         (4U)
#define DATAPROCESS_OSP_QDATA_LEN        (2U)

/* 从站slot类型 */
#define DATAPROCESS_SLOT_TYPE_NONSAFETY  (0x05)
#define DATAPROCESS_SLOT_TYPE_SAFETY     (0x01)
#define DATAPROCESS_SLOT_TYPE_DIAG       (0x02)
#define DATAPROCESS_SLOT_TYPE_SOE        (0x03)
#define DATAPROCESS_SLOT_TYPE_OSP        (0x04)
/* 各类型从站slot个数 */
#define DATAPROCESS_DI_RECV_SLOT_NUM     (3U)
#define DATAPROCESS_DO_RECV_SLOT_NUM     (2U)
#define DATAPROCESS_AI_RECV_SLOT_NUM     (3U)
#define DATAPROCESS_AO_RECV_SLOT_NUM     (2U)
#define DATAPROCESS_OSP_RECV_SLOT_NUM    (4U)

#define DATAPROCESS_DI_SEND_SLOT_NUM     (1U)
#define DATAPROCESS_DO_SEND_SLOT_NUM     (1U)
#define DATAPROCESS_AI_SEND_SLOT_NUM     (1U)
#define DATAPROCESS_AO_SEND_SLOT_NUM     (1U)
#define DATAPROCESS_OSP_SEND_SLOT_NUM    (1U)

#define DATAPROCESS_MAX_SLOT_NUM         (4U)
/* OSP监控数据长度 */
#define DATAPROCESS_OSP_DIAG_ONE_LEN     (39U)
#define DATAPROCESS_OSP_DIAG_TWO_LEN     (3U)

/* 公共返回结果 */
#define DATAPROCESS_RET_OK                      (0U)
#define DATAPROCESS_RET_FAIL                    (1U)

/* 在线及主从置位标志 */
#define SLAVE_ONLINE_MASK           0x01
#define SLAVE_ACTIVE_MASK           0x10

/* 1min = 60s*/
#define TIME_MIN_TO_SEC             (60)
/* 1s=1000ms*/
#define TIME_SEC_TO_MSEC            (1000*1000)
/* 1min = 60000ms*/
#define TIME_MIN_TO_MSEC            (60*1000*1000)
/* SOE分钟校正精度 */
#define TIME_SEC_DIFF_PRECISON      (10)

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack(1)

/* 从站slot头结构 */
typedef struct DataProcessSlaveSlotHeaderTag
{
    uint8_t ucSlotType;            /* 从站slot类型 */
    uint8_t ucSlotLen;             /* 从站slot长度 */
}DataProcessSlaveSlotHeader_t;

/* 从站SOE数据结构 */
typedef struct DataProcessHardSOEDataTag
{
    uint32_t uiSOETick;                    /* SOE计数器，单位为微秒 */
    uint8_t ucChannelNo;                   /* 通道号 */
    uint8_t ucEventState;                  /* 事件前后状态 */
}DataProcessHardSOEData_t;

/* 从站状态、温度、设备诊断数据结构 */
typedef struct DataProcessStateDiagDataTag
{
    uint8_t ucStateDiag;                   /* 状态诊断区 */
    uint8_t ucTempDiag;                    /* 温度诊断区 */
    uint8_t uiDeviceDiag[4];                 /* 设备诊断区 */
}DataProcessStateDiagData_t;

/* 从站通道诊断数据结构 */
typedef struct DataProcessChannelDiagDataTag
{
    uint8_t ucChannelNo;                   /* 通道号 */
    uint8_t ucChannelDiag[3];              /* 通道诊断数据 */
}DataProcessChannelDiagData_t;

/* 从站通道OSP数据结构第一类 */
typedef struct DataProcessOSPDiagDataOneTag
{
    uint32_t uiPRPMA;
    uint32_t uiPRPMB;
    uint32_t uiPRPMC;
    uint32_t uiPAccelA;
    uint32_t uiPAccelB;
    uint32_t uiPAccelC;
    uint32_t uiAlarmPackA;
    uint32_t uiAlarmPackB;
    uint32_t uiAlarmPackC;
    uint8_t ucDigPackA;
    uint8_t ucDigPackB;
    uint8_t ucDigPackC;
}DataProcessOSPDiagDataOne_t;

/* 从站通道OSP数据结构第二类 */
typedef struct DataProcessOSPDiagDataTwoTag
{
    uint8_t ucDigPackA;
    uint8_t ucDigPackB;
    uint8_t ucDigPackC;
}DataProcessOSPDiagDataTwo_t;

#pragma pack ()


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: DataProcessInit
*
* 功能描述: 数据处理模块初始化
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
void DataProcessInit(void);

/*
***********************************************************************************************************************
* 函数名称: DataProcessOutputHandle
*
* 功能描述: 输出数据处理
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
void DataProcessOutputHandle(void);

/*
***********************************************************************************************************************
* 函数名称: DataProcessInputHandle
*
* 功能描述: 输入数据处理
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
void DataProcessInputHandle(void);
/*
***********************************************************************************************************************
* 函数名称: DataProcessModEnableFlgInit
*
* 功能描述: 在线下装模块使能新参数标志初始化
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
void DataProcessModEnableFlgInit(void);

#endif
