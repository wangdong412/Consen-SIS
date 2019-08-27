/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 任务层模块
*
*  文件名称    : Task.h
*
*  功能简介    : 创建RTS周期任务、up1任务、up2任务
*
*  作者       : 李琦
*
*  创建日期    : 2016-02-01
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef TASK_H_
#define TASK_H_

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
#define CONTROL_CYCLE_5MS
//#define INT_500US       /* 时间片为500us */
#define INT_200US         /* 时间片为200us */

//#define CONTROL_CYCLE_20MS


/* 任务间采用相同优先级，任务间通过时间片轮转运行 */
/* 定义任务优先级 */
#define TASK_PRIO_INIT         (int32_t)(2u)    /* 可选的任务优先级为1~9 (数值越小优先级越高)*/
#define TASK_PRIO_RTS          (int32_t)(3u)
#define TASK_PRIO_UP1          (int32_t)(3u)
#define TASK_PRIO_UP2          (int32_t)(3u)

#define SLID_NUM               (TASK_TIME_SLIDE_RTS+TASK_TIME_SLIDE_UP1+TASK_TIME_SLIDE_UP2)

#ifdef INT_500US
#define TASK_PERIOD            (5U * SLID_NUM)          /* 单位100us */
#define TIME_TOLERANCE         (6)                      /* 时间窗口为100us */

#define RTS_TASK_START_TIME_OFFSET   (0)
#define UP1_TASK_START_TIME_OFFSET  (TASK_TIME_SLIDE_RTS*5)
#define UP2_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_UP1)*5)
#endif

#ifdef INT_200US
#define TASK_PERIOD            (2U * SLID_NUM)          /* 单位100us */
#define TIME_TOLERANCE         (2)                      /* 时间窗口为100us */

#define RTS_TASK_START_TIME_OFFSET   (0)
#define UP1_TASK_START_TIME_OFFSET  (TASK_TIME_SLIDE_RTS*2)
#define UP2_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_UP1)*2)
#endif

/* 定义任务时间片 */
#if 0
#define TASK_TIME_SLIDE_INIT    (int32_t)(12000u)    /* 60s */
#define TASK_TIME_SLIDE_RTS    (int32_t)(7u)            /* 3.5ms */
#define TASK_TIME_SLIDE_UP1    (int32_t)(2u)            /* 1ms */
#define TASK_TIME_SLIDE_UP2    (int32_t)(1u)            /* 0.5ms */
//#else
#define TASK_TIME_SLIDE_INIT    (int32_t)(500u)
#define TASK_TIME_SLIDE_RTS    (int32_t)(350u)
#define TASK_TIME_SLIDE_UP1    (int32_t)(100u)
#define TASK_TIME_SLIDE_UP2    (int32_t)(50u)
#endif

#if 0 //50ms
#define TASK_TIME_SLIDE_INIT    (uint32_t)(5000*2u)       /* 5000ms */
#define TASK_TIME_SLIDE_RTS    (uint32_t)(35*2u)         /* 35ms */
#define TASK_TIME_SLIDE_UP1    (uint32_t)(10*2u)         /* 10ms */
#define TASK_TIME_SLIDE_UP2    (uint32_t)(5*2u)          /* 5ms */
#endif

#ifdef CONTROL_CYCLE_20MS //20ms
#define TASK_TIME_SLIDE_RTS    (uint32_t)(14*2u)         /* 14ms */
#define TASK_TIME_SLIDE_UP1    (uint32_t)(4*2u)          /* 4ms */
#define TASK_TIME_SLIDE_UP2    (uint32_t)(2*2u)          /* 2ms */
#endif

#ifdef CONTROL_CYCLE_5MS //5ms

#ifdef INT_500US
#define TASK_TIME_SLIDE_RTS    (uint32_t)(7u)         /* 3.5ms */
#define TASK_TIME_SLIDE_UP1    (uint32_t)(2u)         /* 1.0ms */
#define TASK_TIME_SLIDE_UP2    (uint32_t)(1u)         /* 0.5ms */
#endif

#ifdef INT_200US
#define TASK_TIME_SLIDE_RTS    (uint32_t)(17u)        /* 3.4ms */
#define TASK_TIME_SLIDE_UP1    (uint32_t)(5u)         /* 1.0ms */
#define TASK_TIME_SLIDE_UP2    (uint32_t)(3u)         /* 0.6ms */
#endif

#endif

/* 用户任务运行结束与否标志 */
#define UP_TASK_RUNNING_FINISHED       (true)
#define UP_TASK_RUNNING_NOT_FINISHED   (false)
/* 用户任务运行开始与否标志 */
#define UP_TASK_RUNNING_START          (true)
#define UP_TASK_RUNNING_NOT_START      (false)

#define RTS_DATA_SIZE                       (0x80000)
#define RTS0_DATA_BASE                      (0x00180000U)
#define RTS1_DATA_BASE                      (0x05180000U)
#define RTS2_DATA_BASE                      (0x0A180000U)

#define RTS_STACKF_SIZE                     (0x8000)
#define RTS_STACKI_SIZE                     (0x80000)

#define RTS0_RTS_STACKF_BOTTOM              (0x100000+0x001A0000-256*1024*5)
#define RTS0_RTS_STACKI_BOTTOM              (0x100000+0x00190000-256*1024*5)

#define RTS0_UP1_STACKF_BOTTOM              (0x100000+0x001A0000-256*1024*6)
#define RTS0_UP1_STACKI_BOTTOM              (0x100000+0x00190000-256*1024*6)
#define RTS0_UP2_STACKF_BOTTOM              (0x100000+0x001A0000-256*1024*7)
#define RTS0_UP2_STACKI_BOTTOM              (0x100000+0x00190000-256*1024*7)

#define RTS1_RTS_STACKF_BOTTOM              (0x5100000+0x001A0000-256*1024*5)
#define RTS1_RTS_STACKI_BOTTOM              (0x5100000+0x00190000-256*1024*5)

#define RTS1_UP1_STACKF_BOTTOM              (0x5100000+0x001A0000-256*1024*6)
#define RTS1_UP1_STACKI_BOTTOM              (0x5100000+0x00190000-256*1024*6)
#define RTS1_UP2_STACKF_BOTTOM              (0x5100000+0x001A0000-256*1024*7)
#define RTS1_UP2_STACKI_BOTTOM              (0x5100000+0x00190000-256*1024*7)

#define RTS2_RTS_STACKF_BOTTOM              (0xA100000+0x001A0000-256*1024*5)
#define RTS2_RTS_STACKI_BOTTOM              (0xA100000+0x00190000-256*1024*5)

#define TASK_SEQ_MAX                        (0x0000002CU)   /* 最大支持顺序自检函个数      44-1 */
#define TASK_SEQ_GRP_MAX                    (0x00000002U)   /* 最大支持并行顺序自检组数         */
/* 顺序启动正常 */
#define TASK_CHECK_SEQ_OK                                   (0x00000001U)
/* 顺序启动异常 */
#define TASK_CHECK_SEQ_ERR                                  (0x00000000U)

typedef enum
{
    RTS_TASK_SEQ = 0U,                      /*RTS自检                      */
    UP1_TASK_SEQ,                           /*UP1自检                      */
    UP2_TASK_SEQ,                           /*UP2自检            */
    MAX_TASK_SEQ_NUM,                       /*自检序列号尾                      */
}TaskDiagSequence;

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
typedef void (*UP1Entry)(uint32_t);
typedef void (*UP2Entry)(uint32_t);

typedef void (*fpRTSTaskEntry)(void);
/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: SysStart
*
* 功能描述: 系统启动函数
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
void SysStart(void);

void TskRTSTaskExe(void);

void TskRTSTaskExeEntry(void);

void TskUP1TaskExe(void);

void TskUP1TaskExeEntry(void);

void TskUP2TaskExe(void);

void TskUP2TaskExeEntry(void);

#endif /* TASK_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

