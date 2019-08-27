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
*  功能简介    : 创建RTS周期任务、自检任务、通讯任务、文件管理任务
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
#define TASK_PRIO_INIT         (int32_t)(2u)    /* 可选的任务优先级为1~9 */
#define TASK_PRIO_RTS          (int32_t)(3u)
#define TASK_PRIO_CFG          (int32_t)(3u)
#define TASK_PRIO_DIAG         (int32_t)(3u)
#define TASK_PRIO_COMM         (int32_t)(3u)
#define TASK_PRIO_FILE         (int32_t)(3u)



#ifdef INT_500US
#define SLID_NUM               (TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_DIAG + TASK_TIME_SLIDE_FILE + TASK_TIME_SLIDE_COMM)
#define TASK_PERIOD            (5U * SLID_NUM)          /* 单位100us */
#define TIME_TOLERANCE         (6)                      /* 时间窗口为100us */
#define TASK_DELAY_TIME        (TIME_TOLERANCE*100)

#define RTS_TASK_START_TIME_OFFSET   (0)
#define DIAG_TASK_START_TIME_OFFSET  (TASK_TIME_SLIDE_RTS*5)
#define FILE_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_DIAG)*5)
#define COMM_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_DIAG + TASK_TIME_SLIDE_FILE)*5)
#endif

#ifdef INT_200US
#define SLID_NUM               (TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_CFG + TASK_TIME_SLIDE_DIAG + TASK_TIME_SLIDE_FILE + TASK_TIME_SLIDE_COMM)
#define TASK_PERIOD            (2U * SLID_NUM)          /* 单位100us */
#define TIME_TOLERANCE         (2)                      /* 时间窗口为100us */
#define TASK_DELAY_TIME        (TIME_TOLERANCE*100)

#define RTS_TASK_START_TIME_OFFSET   (0)
#define CFG_TASK_START_TIME_OFFSET  (TASK_TIME_SLIDE_RTS*2)
#define DIAG_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_CFG)*2)
#define FILE_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_CFG + TASK_TIME_SLIDE_DIAG)*2)
#define COMM_TASK_START_TIME_OFFSET  ((TASK_TIME_SLIDE_RTS + TASK_TIME_SLIDE_CFG + TASK_TIME_SLIDE_DIAG + TASK_TIME_SLIDE_FILE)*2)
#endif

/* 定义任务时间片 */
#if 0
#define TASK_TIME_SLIDE_INIT   (int32_t)(10)    /* 5ms */
#define TASK_TIME_SLIDE_RTS    (int32_t)(7u)    /* 3.5ms */
#define TASK_TIME_SLIDE_DIAG   (int32_t)(1u)    /* 0.5ms */
#define TASK_TIME_SLIDE_COMM   (int32_t)(1u)    /* 0.5ms */
#define TASK_TIME_SLIDE_FILE   (int32_t)(1u)    /* 0.5ms */
#endif
#if 0
#define TASK_TIME_SLIDE_INIT   (int32_t)(500u)    /* 500ms */
#define TASK_TIME_SLIDE_RTS    (int32_t)(350u)    /* 350ms */
#define TASK_TIME_SLIDE_DIAG   (int32_t)(50u)    /* 50ms */
#define TASK_TIME_SLIDE_COMM   (int32_t)(50u)    /* 50ms */
#define TASK_TIME_SLIDE_FILE   (int32_t)(50u)    /* 50ms */
#endif

#if 0 //50ms
#define TASK_TIME_SLIDE_INIT   (uint32_t)(5000*2u)    /* 5000ms */
#define TASK_TIME_SLIDE_RTS    (uint32_t)(20*2u)     /* 20ms */
#define TASK_TIME_SLIDE_DIAG   (uint32_t)(2*2u)      /* 2ms */
#define TASK_TIME_SLIDE_FILE   (uint32_t)(3*2u)      /* 3ms */
#define TASK_TIME_SLIDE_COMM   (uint32_t)(25*2u)     /* 25ms */
#endif

#ifdef CONTROL_CYCLE_20MS //20ms
#define TASK_TIME_SLIDE_RTS    (uint32_t)(12*2u)      /* 12ms */
#define TASK_TIME_SLIDE_DIAG   (uint32_t)(1*2u)       /* 1ms */
#define TASK_TIME_SLIDE_FILE   (uint32_t)(1*2u)       /* 1ms */
#define TASK_TIME_SLIDE_COMM   (uint32_t)(6*2u)       /* 6ms */
#endif

#ifdef CONTROL_CYCLE_5MS //liqi 5ms
#ifdef INT_500US
#define TASK_TIME_SLIDE_RTS    (uint32_t)(5u)     /* 2.5ms */
#define TASK_TIME_SLIDE_DIAG   (uint32_t)(1u)     /* 0.5ms */
#define TASK_TIME_SLIDE_FILE   (uint32_t)(1u)     /* 0.5ms */
#define TASK_TIME_SLIDE_COMM   (uint32_t)(3u)     /* 1.5ms */
#endif

#ifdef INT_200US
#define TASK_TIME_SLIDE_RTS    (uint32_t)(14u)    /* 2.8ms */
#define TASK_TIME_SLIDE_CFG    (uint32_t)(1u)     /* 0.2ms */
#define TASK_TIME_SLIDE_DIAG   (uint32_t)(1u)     /* 0.2ms */
#define TASK_TIME_SLIDE_FILE   (uint32_t)(1u)     /* 0.2ms */
#define TASK_TIME_SLIDE_COMM   (uint32_t)(8u)     /* 1.6ms */
#endif

#endif

#define TASK_SEQ_MAX                        (0x0000002CU)   /* 最大支持顺序自检函个数      44-1 */
#define TASK_SEQ_GRP_MAX                    (0x00000002U)   /* 最大支持并行顺序自检组数         */
/* 顺序启动正常 */
#define TASK_CHECK_SEQ_OK                                   (0x00000001U)
/* 顺序启动异常 */
#define TASK_CHECK_SEQ_ERR                                  (0x00000000U)

typedef enum
{
    RTS_TASK_SEQ = 0U,                      /*RTS自检                      */
    COMM_TASK_SEQ,                           /*COMM自检            */
    MAX_TASK_SEQ_NUM,                       /*自检序列号尾                      */
}TaskDiagSequence;

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
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
/*
***********************************************************************************************************************
* 函数名称: TskRTSTaskExe
*
* 功能描述: RTS周期任务执行函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
void TskRTSTaskExe(void);
/*
***********************************************************************************************************************
* 函数名称: TskRTSTaskExeEntry
*
* 功能描述: RTS任务入口函数
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
void TskRTSTaskExeEntry(void);
/*
***********************************************************************************************************************
* 函数名称: TskDiagTaskExe
*
* 功能描述: 诊断任务执行函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
void TskDiagTaskExe(void);
/*
***********************************************************************************************************************
* 函数名称: TskDiagTaskExeEntry
*
* 功能描述: 诊断任务入口函数
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
void TskDiagTaskExeEntry(void);
/*
***********************************************************************************************************************
* 函数名称: TskFileTaskExe
*
* 功能描述: 文件管理周期任务执行函数
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
void TskFileTaskExe(void);
/*
***********************************************************************************************************************
* 函数名称: TskFileTaskExeEntry
*
* 功能描述: 文件管理任务入口函数
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
void TskFileTaskExeEntry(void);
/*
***********************************************************************************************************************
* 函数名称: TskCommTaskExe
*
* 功能描述: 通讯任务执行函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 0
*
* 注意事项: 无
***********************************************************************************************************************
*/
void TskCommTaskExe(void);
/*
***********************************************************************************************************************
* 函数名称: TskCommTaskExeEntry
*
* 功能描述: 通讯任务入口函数
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
void TskCommTaskExeEntry(void);

#endif /* TASK_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

