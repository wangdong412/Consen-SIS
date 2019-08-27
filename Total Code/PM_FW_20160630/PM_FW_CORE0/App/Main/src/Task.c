/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : Main模块
*
*  文件名称    : Task.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-02-01
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
#include "inc/DataType.h"
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/Task.h"


/* 包含Manager和Monitor中的头文件 */
#include "App/Manager/inc/Manager.h"
#include "App/Monitor/inc/Monitor.h"
#include "App/DataProcess/inc/DataProcess.h"

/* 包含DataProcess中的头文件 */
/* 包含Diag中的头文件 */
/* 包含RetainData中的头文件 */
/* 包含SoftSOE中的头文件 */
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SysResource/inc/Sys.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static int32_t s_iInitTaskId = -1;
static int32_t s_iRTSTaskId = -1;
static int32_t s_iUP1TaskId = -1;
static int32_t s_iUP2TaskId = -1;

static int32_t s_iRTStoUP1Sem;
static int32_t s_iRTStoUP2Sem;
static int32_t s_iUP1toRTSSem;
static int32_t s_iUP2toRTSSem;

static bool_t s_bUP1FinishFlag;
static bool_t s_bUP2FinishFlag;
static bool_t s_bUP1StartFlag;
static bool_t s_bUP2StartFlag;
extern UINT32 debug_tick;
/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 初始任务 */
static void TskInitTask (void);
/* RTS周期任务 */
static void TskRTSTask (void);
/* UP1周期任务 */
static void TskUP1Task (void);
/* UP2周期任务 */
static void TskUP2Task (void);
/*
***********************************************************************************************************************
*                                全局函数实现
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
void SysStart(void)
{
    uint32_t uiErr = 0;

    printf("\n   #------------Create initial task cagRTS CORE0------------------#\n");

    /* Create initial task */
    s_iInitTaskId = os_create_task("Init_task", TskInitTask, NULL, TASK_PRIO_INIT, TASK_TIME_SLIDE_INIT, &uiErr);

    if(uiErr != 0)
    {
        printf("Create initial task failed %d\n", uiErr);
    }
    printf("\n   #------------Create initial task cagRTS CORE0 end------------------#\n");
    printf("RTS$:\n");
}

/*
***********************************************************************************************************************
* 函数名称: TskInitTask
*
* 功能描述: 初始化任务入口函数
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
static void TskInitTask (void)
{
    uint32_t uiErr = 0;
    int32_t iRet = 0;
    
    printf("\n   #------------INIT TASK------------------#\n");

    s_iRTStoUP1Sem = os_sem_create("RTStoUP1", 1, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iRTStoUP1Sem create failed.\n");
    }

    s_iRTStoUP2Sem = os_sem_create("RTStoUP2", 1, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iRTStoUP2Sem create failed.\n");
    }

    s_iUP1toRTSSem = os_sem_create("UP1toRTS", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iUP1toRTSSem create failed.\n");
    }

    s_iUP2toRTSSem = os_sem_create("UP2toRTS", 0, &uiErr);
    if(uiErr != OS_ERR_NONE)
    {
       printf("s_iUP2toRTSSem create failed.\n");
    }

    s_bUP1FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
    s_bUP2FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;

    s_bUP1StartFlag = UP_TASK_RUNNING_START;
    s_bUP2StartFlag = UP_TASK_RUNNING_START;

    /* Power on diagnosis */

    /* System resource module initial */
    SysInit();

    /* Monitor module initial */
    MonitorInit();

    /* Manager module initial */
    ManagerInit();

    /* 初始化数据处理 */
    DataProcessInit();

    /* Load FPGA firmware */

    /* Create RTS task */
    s_iRTSTaskId = os_create_task("RTS_task", TskRTSTask, NULL, TASK_PRIO_RTS, TASK_TIME_SLIDE_RTS, &uiErr);
    printf("Create rts task.\n");
    if(uiErr != 0)
    {
        printf("Create RTS task failed %d\n", uiErr);
    }
    /* Create UP1 task */
    s_iUP1TaskId = os_create_task("UP1_task", TskUP1Task, NULL, TASK_PRIO_UP1, TASK_TIME_SLIDE_UP1, &uiErr);
    printf("Create up1 task.\n");
    if(uiErr != 0)
    {
        printf("Create UP1 task failed %d\n", uiErr);
    }
    /* Create UP2 task */
    s_iUP2TaskId = os_create_task("UP2_task", TskUP2Task, NULL, TASK_PRIO_UP2, TASK_TIME_SLIDE_UP2, &uiErr);
    //printf("Create up2 task.\n");
    if(uiErr != 0)
    {
        //printf("Create UP2 task failed %d\n", uiErr);
    }
    
    printf("Delete initial task\n");
    iRet = os_del_task(s_iInitTaskId, &uiErr);
    printf("Delete initial task ...\n");
    if(iRet != 0)
    {
        printf("Delete initial task failed!\n");
    }
}

/*
***********************************************************************************************************************
* 函数名称: TskRTSTask
*
* 功能描述: RTS周期任务入口函数
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
static void TskRTSTask (void)
{
    static uint32_t s_uiCycleCount = 0;
    uint32_t uiErr = 0;
    int32_t iPend = -1;
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;

    //printf("\n   #------------RTS TASK------------------#\n");

    uiTimeWindow1 = RTS_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;

    uiStartTime = SysGetSyncTimeLow32 ();
    printf("os%d rts%d\n",debug_tick, uiStartTime);

    while(1)
    {
        uiStartTime = SysGetSyncTimeLow32 ();

        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            printf("RS1=%d\n",uiStartTime);
            
            /* RTS task get semaphore from UP1 task*/
             iPend = os_sem_pend(s_iUP1toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
             if(uiErr != OS_ERR_NONE)
             {
                 //printf("RTS task get s_iUP1toRTSSem failed.\n");
             }
             else
             {
                 if(0 == iPend)
                 {
                     s_bUP1FinishFlag = UP_TASK_RUNNING_FINISHED;
                 }
             }
    
             /* Calculate UP1 running period */
             if(UP_TASK_RUNNING_FINISHED == s_bUP1FinishFlag) 
             {
                 s_bUP1FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
                 SysSetIECShedCtrlFlg(TASK_UP1, IEC_END);
             }
#if 0
    //         /* RTS task get semaphore from UP2 task*/
    //         iPend = os_sem_pend(s_iUP2toRTSSem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
    //         if(uiErr != OS_ERR_NONE)
    //         {
    //             //printf("RTS task get s_iUP2toRTSSem failed.\n");
    //         }
    //
    //         if(0 == iPend)
    //         {
    //             s_bUP2FinishFlag = UP_TASK_RUNNING_FINISHED;
    //
    //         }
    //         /* Calculate UP2 running period */
    //         if(UP_TASK_RUNNING_FINISHED == s_bUP2FinishFlag) //&& (s_uiCycleCount%(usr_period2/5) == 0) 用户配置的up2运行周期
    //         {
    //             s_bUP2FinishFlag = UP_TASK_RUNNING_NOT_FINISHED;
    //
    //             SysSetIECShedCtrlFlg(TASK_UP2, IEC_END);
    //             
    //         }
#endif
            
            /*输出*/
            DataProcessOutputHandle();

            /* Monitor module cycle function */
            MonitorCycle();
    
            /* Manager module cycle function */
            ManagerCycle();
            
            /* core0 --> shared memory */
            SysCopyCore0RtDataToSharedRtData();

            /*输入*/
            udelay(7000);
            DataProcessInputHandle();

            if(IEC_START == SysGetIECShedCtrlFlg(TASK_UP1))
            {
                /* RTS task send semaphore to UP1 task*/
                os_sem_post(s_iRTStoUP1Sem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);
                
                if(uiErr != OS_ERR_NONE)
                {
                    printf("RTS task post s_iRTStoUP1Sem failed.\n");
                }
            }
#if 0
    //        if(IEC_START == SysGetIECShedCtrlFlg(TASK_UP2))
    //        {
    //            /* RTS task send semaphore to UP1 task*/
    //            os_sem_post(s_iRTStoUP2Sem, OS_OPT_POST_1, &uiErr);
    //            if(uiErr != OS_ERR_NONE)
    //            {
    //                //printf("RTS task post s_iRTStoUP2Sem failed.\n");
    //            }
    //        }
#endif
            s_uiCycleCount++;
    
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: TskUP1Task
*
* 功能描述: UP1周期任务入口函数
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
static void TskUP1Task (void)
{
    uint32_t uiErr = 0;
    int32_t iPend = -1;
    uint32_t uiStartTime = 0;
    uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;
    UP1Entry pUP1Func = NULL;
    puint8_t pucDataAreaAddr = NULL;
    
    //printf("\n   #------------UP1 TASK------------------#\n");

    uiTimeWindow1 = UP1_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;
    
    while(1)
    {
        uiStartTime = SysGetSyncTimeLow32 ();

        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
//            printf("UP1 StartTime = %d ms\n",uiStartTime/10);
            iPend = os_sem_pend(s_iRTStoUP1Sem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
            if(uiErr != OS_ERR_NONE)
            {
                //printf("UP1 task get s_iRTStoUP1Sem failed.\n");
            }
            else
            {
                if(0 == iPend)
                {
//                    printf("S\n");
                    s_bUP1StartFlag = UP_TASK_RUNNING_START;
                }
            }
    
            if(UP_TASK_RUNNING_START == s_bUP1StartFlag)
            {
                /* Execute user program 1 */
                
                
                if(HAS_BE_CONFIGURED == SysGetIsConfigured())
                {
                    printf("U\n");
                    pUP1Func = (UP1Entry)SharedGetTaskCodeEntry(TASK_UP1);
                    pucDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
                    
                    if((pUP1Func != NULL) && (pucDataAreaAddr != NULL))
                    {
                        pUP1Func((uint32_t)pucDataAreaAddr);
                    }
                }
    
                os_sem_post(s_iUP1toRTSSem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);
    
                if(uiErr != OS_ERR_NONE)
                {
                    printf("UP1 task post s_uiUP1toRTSSem failed.\n");
                }
                s_bUP1StartFlag = UP_TASK_RUNNING_NOT_START;
            }

            uiEndTime = SysGetSyncTimeLow32 ();

            if(uiEndTime - uiStartTime < TIME_TOLERANCE)
            {
                udelay(2100);
            }
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: TskUP2Task
*
* 功能描述: UP2周期任务入口函数
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
static void TskUP2Task (void)
{
    uint32_t uiErr = 0;
    int32_t iPend = -1;
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;

    //printf("\n   #------------UP2 TASK------------------#\n");

    uiTimeWindow1 = UP2_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;

    while(1)
    {
        uiStartTime = SysGetSyncTimeLow32 ();

        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            //printf("UP2 StartTime = %d ms\n",uiStartTime/10);
            
            iPend = os_sem_pend(s_iRTStoUP2Sem, 0, OS_OPT_PEND_NON_BLOCKING, &uiErr);
    
            if(uiErr != OS_ERR_NONE)
            {
                //printf("UP2 task get s_iRTStoUP2Sem failed.\n");
            }
    
            if(0 == iPend)
            {
                s_bUP2StartFlag = UP_TASK_RUNNING_START;
            }
    
            if(UP_TASK_RUNNING_START == s_bUP2StartFlag)
            {
                /* Execute user program 2 */
    
                os_sem_post(s_iUP2toRTSSem, OS_OPT_POST_1|OS_OPT_POST_NO_SCHED, &uiErr);
    
                if(uiErr != OS_ERR_NONE)
                {
                    //printf("UP2 task post s_uiUP2toRTSSem failed.\n");
                }
                s_bUP2StartFlag = UP_TASK_RUNNING_NOT_START;
            }
        }
    }
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/




/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


