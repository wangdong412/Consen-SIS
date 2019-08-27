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
/* 任务间采用相同优先级，任务间通过时间片轮转运行 */
/* 定义任务优先级 */
#define TASK_PRIO_INIT         (int32_t)(2u)    /* 可选的任务优先级为1~9 (数值越小优先级越高)*/
#define TASK_PRIO_RTS          (int32_t)(3u)
#define TASK_PRIO_UP1          (int32_t)(3u)
#define TASK_PRIO_UP2          (int32_t)(3u)

#define SLID_NUM               (TASK_TIME_SLIDE_RTS+TASK_TIME_SLIDE_UP1+TASK_TIME_SLIDE_UP2)
#define TASK_PERIOD            (5U * SLID_NUM)          /* 单位100us */
#define TIME_TOLERANCE         (6)                      /* 时间窗口为100us */

#define RTS_TASK_START_TIME_OFFSET   (0)
#define UP1_TASK_START_TIME_OFFSET  (TASK_TIME_SLIDE_RTS*5)
#define UP2_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_UP1)*5)

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

#if 1 //20ms
#define TASK_TIME_SLIDE_INIT    (uint32_t)(5000*2u)       /* 5000ms */
#define TASK_TIME_SLIDE_RTS    (uint32_t)(14*2u)         /* 14ms */
#define TASK_TIME_SLIDE_UP1    (uint32_t)(4*2u)          /* 4ms */
#define TASK_TIME_SLIDE_UP2    (uint32_t)(2*2u)          /* 2ms */
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

