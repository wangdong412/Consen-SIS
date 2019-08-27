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
*  文件名称    : SharedSOE.h
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-07-19
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef SHARED_SOE_H_
#define SHARED_SOE_H_

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

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
/* SRAM中存放SOE的最大条数 */
#define MAX_SOE_NUM_SRAM                          (10000U)
/* DDR中存放SOE的最大条数 */
#define MAX_SOE_NUM_DDR                           (100000U)
/* SOE队列最大个数 */
#define MAX_SOE_QUEUE_NUM                         (17U)
#define HARD_SOE_QUEUE_INDEX                      (16U)

/* 队列空或满 */
#define SOE_QUEUE_EMPTY                           (0)
#define SOE_QUEUE_NOT_EMPTY_NOT_FULL              (1)
#define SOE_QUEUE_FULL                            (2)

/* 队列类型 */
#define SOE_QUEUE_TYPE_HISTORY                    (2)
#define SOE_QUEUE_TYPE_CYCLE_FIFO                 (1)

/* 队列没有被配置或已被配置 */
#define SOE_QUEUE_NOT_CONFIGED                    (false)
#define SOE_QUEUE_CONFIGED                        (true)

/* 从队列头开始读取SOE */
#define READ_SOE_FROM_BEGINNING                   (0xFFFF)

/* 队列被读空 */
#define SOE_QUEUE_IS_EMPTY_NOW                    (-1)
/* 队列中最后一条SOE记录 */
#define THE_LAST_SOE_IN_QUEUE                     (-2)
/* 因传参错误导致读取SOE失败 */
#define READ_SOE_QUEUE_FAILED                     (-3)
/* 共享内存被锁定 */
#define READ_SOE_QUEUE_LOCKED                     (-4)
/* 已经被读空，没有新记录*/
#define READ_SOE_QUEUE_NO_NEW_RECORD              (-5)

/* SOE的存储位置 */
#define SOE_INVALID_STORAGE                       (0)
#define SOE_STORAGE_SRAM                          (1)
#define SOE_STORAGE_DDR                           (2)

/* 对SRAM的地址规划 */
#define SOE_SRAM_SEC0_START_ADDR                  (0xFF600000)
#define SOE_SRAM_SEC1_START_ADDR                  (0xFF610000)

#define SOE_SRAM_QUEUE_INFO_START_ADDR            (0xFF620000-512)

#define SOE_SRAM_SEC2_START_ADDR                  (0xFF620000)
#define SOE_SRAM_SEC3_START_ADDR                  (0xFF630000)

#define SOE_RECORD_SEC0_START_INDEX               (0U)
#define SOE_RECORD_SEC0_MAX_INDEX                 (6552U)
#define SOE_RECORD_SEC1_START_INDEX               (6553U)
#define SOE_RECORD_SEC1_MAX_INDEX                 (9999U)

#define SOE_RECORD_SEC2_START_INDEX               (0U)
#define SOE_RECORD_SEC2_MAX_INDEX                 (5461U)
#define SOE_RECORD_SEC3_START_INDEX               (5462U)
#define SOE_RECORD_SEC3_MAX_INDEX                 (9999U)

/* ______________________________________________________
* | S | Sector 0 start addr(0xFF600000)             |   |
* | O | SOE0 SOE1 SOE2 SOE3 SOE4 ...                |   |
* | F | ...                                         |   |
* | T | SOE6552                                     |   |
* |   |---------------------------------------------| S |
* | S | Sector 1 start addr(0xFF610000)             | R |
* | O | SOE6553 SOE6554 SOE6555 ...                 | A |
* | E | ...                                         | M |
* |   | SOE9999                                     |   |
* |___|_____________________________________________|   |
* | H | Sector 2 start addr(0xFF620000)             |   |
* | A | SOE0 SOE1 SOE2 SOE3 SOE4 ...                |   |
* | R | ...                                         |   |
* | D | SOE5461                                     |   |
* |   |---------------------------------------------|   |
* | S | Sector 3 start addr(0xFF630000)             |   |
* | O | SOE5462 SOE5463 SOE5464 ...                 |   |
* | E | ...                                         |   |
* |   | SOE9999                                     |   |
* |___|_____________________________________________|___|
*/

#define SOE_QUEUE_UNLOCK                         (0u)
#define SOE_QUEUE_LOCK                           (1u)

#define SOE_SRAM_QUEUE_INFO_VALID_FLAG                 (0x12345678)
#define SOE_SRAM_QUEUE_INFO_INVALID_FLAG               (0x00000000)

/* 用户所见SOE块号范围 */
#define SOE_MIN_BLOCK_ID                (1U)
#define SOE_MAX_BLOCK_ID                (16U)

#define SOE_STATE_NOCONFIG              (0U)
#define SOE_STATE_START                 (1U)
#define SOE_STATE_STOP                  (2U)
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
#pragma pack (1)

/*软SOE结构体定义*/
typedef struct SoftSOETag
{
    uint32_t uiSec;    /*时间戳 相对起点走过的秒 起点：1970.01.01 00:00:00 */
    uint32_t uiMicroSec; /* 时间戳 -微秒 */
    uint16_t usIndex;  /* 序号 */
    uint8_t ucBlockNo; /* 块号 */
    uint8_t ucValue;   /* 跳变信息 */
}SoftSOE_t;

/*硬SOE结构体定义*/
typedef struct HardSOETag
{
    uint32_t uiSec;     /*时间戳 相对起点走过的秒 起点：1970.01.01 00:00:00 */
    uint32_t uiMicroSec;  /* 时间戳 -微秒*/
    uint8_t ucModNo;    /* 模块号 */
    uint8_t ucChannelNo;/* 通道号*/
    uint8_t ucValue;    /* 跳变信息 */
    uint8_t ucReserved; /* 保留 */
}HardSOE_t;
#pragma pack ()

#pragma pack (4)
/*SOE队列结构体定义*/
typedef struct SOEQueueTag
{
    uint32_t uiLockFlg;  /*共享内存锁定标识*/
    uint16_t usType;     /*类型：1循环覆盖  2不覆盖 */
    uint32_t uiBase;     /* 队列基地址 */
    uint16_t usSize;     /* 队列大小 */
    uint16_t usHeader;   /* 队头*/
    uint16_t usTail;     /* 队尾 */
    uint8_t ucQueueState;/* 队列空满状态标识 */
    uint8_t ucStorage;   /* SRAM/DDR */
    bool_t bConfiged;    /* 队列是否被配置 */
}SOEQueue_t;

typedef struct SOESRAMQueueTag
{
    uint16_t usHeader;   /* 队头*/
    uint16_t usTail;     /* 队尾 */
    uint16_t usQueueState;/* 队列空满状态标识 */
    uint16_t usStorage;   /* SRAM/DDR */
}SOESRAMQueue_t;


typedef struct SOESRAMQueuesInfoTag
{
    uint32_t uiFlag;
    SOESRAMQueue_t stSOESRAMQueue[MAX_SOE_QUEUE_NUM];
}SOESRAMQueuesInfo_t;

#pragma pack ()


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SharedSOEInit
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
void SharedSOEInit(void);

/*
***********************************************************************************************************************
* 函数名称: SharedSOEKeyInit
*
* 功能描述: 钥匙开关在Init时的初始化
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
void SharedSOEKeyInit(void);

/*
***********************************************************************************************************************
* 函数名称: SharedSOEQueueCfg
*
* 功能描述: 根据配置信息对队列进行配置
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
void SharedSOEQueueCfg(void);

/*
***********************************************************************************************************************
* 函数名称: SharedSetSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: ucQueueIndex: 队列索引
*        stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedSetSOE(uint8_t ucQueueIndex, SoftSOE_t stSOE);

/*
***********************************************************************************************************************
* 函数名称: SharedGetSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数: ucQueueIndex: 队列索引
*        usPos: 队列中的偏移 （0表示从开头读）
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 队列已经为空
*        -2 最后一条SOE记录
*        -3 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t SharedGetSOE(uint8_t ucQueueIndex, SoftSOE_t* pstSOE, uint16_t usPos, puint32_t puiNextIndex);

/*
***********************************************************************************************************************
* 函数名称: SharedClearSOE
*
* 功能描述: 清除SOE
*
* 输入参数: ucQueueIndex: 队列索引号
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedClearSOE(uint8_t ucQueueIndex);

/*
***********************************************************************************************************************
* 函数名称: SharedSetHardSOE
*
* 功能描述: 将1条SOE记录写入队列
*
* 输入参数: stSOE: 1条待写入的SOE
*
* 输出参数: 无
*
* 返 回 值  : true-写入成功
*        false-写入失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
bool_t SharedSetHardSOE(HardSOE_t stSOE);

/*
***********************************************************************************************************************
* 函数名称: SharedGetHardSOE
*
* 功能描述: 从指定队列读取1条SOE记录
*
* 输入参数:
*        usLogicPos: 队列中的偏移  (READ_SOE_FROM_BEGINNING 表示从开头读)
*
* 输出参数: pstSOE: 存放一条SOE的指针
*
* 返 回 值  : -1 本次读取后队列已经为空
*        -2 输入输出参数错误，读取失败
*        >=0 当前被读取的SOE记录在队列中的偏移
*
* 注意事项: 无
***********************************************************************************************************************
*/
int32_t SharedGetHardSOE(HardSOE_t* pstSOE, uint16_t usLogicPos, puint32_t puiNextIndex);

#endif /* SHARED_SOE_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

