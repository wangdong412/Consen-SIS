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
*  文件名称    : task.c
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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/cagos.h>
#include "../inc/Task.h"

/* 包含Manager和Monitor中的头文件 */
#include "App/Manager/inc/Manager.h"
#include "App/Monitor/inc/Monitor.h"
/* 包含Diag中的头文件 */
//#include "App/Diag/inc/Diag.h"
/* 包含Comm中的头文件 */
#include "App/Comm/inc/Comm.h"
/* 包含FileManager中的头文件 */
#include "App/FileManager/inc/FileManager.h"

#include "Srv/SysResource/inc/Sys.h"
#include "Srv/CommProtocol/inc/CMBusProtocol.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/SharedMem/inc/SharedMem.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static int32_t s_iInitTaskId = -1;

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
/* 初始任务 */
static void TskInitTask (void);
static void TskRTSTask (void);
static void TskDiagTask (void);
static void TskCommTask (void);
static void TskFileTask (void);
static void TimeAlignment(void);
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

    printf("\n   #------------Create initial task cagRTS CORE1------------------#\n");

    /* Create initial task */
    s_iInitTaskId = os_create_task("Init_task", TskInitTask, NULL, TASK_PRIO_INIT, TASK_TIME_SLIDE_INIT, &uiErr);
    
    if(s_iInitTaskId < 0)
    {
        printf("Create initial task failed %d\n", s_iInitTaskId);
    }

    if(uiErr != 0)
    {
        printf("Create initial task failed %d\n", uiErr);
    }
    printf("\n   #------------Create initial task cagRTS CORE1 end------------------#\n");
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
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void TskInitTask(void)
{
    int32_t iTaskId = -1;
    uint32_t uiErr = 0;

    printf("\n   #------------Initial task cagRTS CORE1------------------#\n");

    /* Power on diagnosis */

    /* System resource module initial */
    SysInit();

    /* Monitor module initial */
    MonitorInit();

    /* Manager module initial */
    ManagerInit();

    /* File manager module initial */
    //FileManagerInit();

    /* Communication module initial */
    CommInit();

    /* Communication Protocol module initial */
    CMBusProtocolInit();
    
    
    SharedConfigInit();

    /* Create RTS task */
    iTaskId = os_create_task("RTS_task", TskRTSTask, NULL, TASK_PRIO_RTS, TASK_TIME_SLIDE_RTS, &uiErr);
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create RTS task failed %d\n", uiErr);
    }

    /* Create diagnosis task */
    iTaskId = os_create_task("Diag_task", TskDiagTask, NULL, TASK_PRIO_DIAG, TASK_TIME_SLIDE_DIAG, &uiErr);
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create diagnosis task failed %d\n", uiErr);
    }

    /* Create file management task */
    iTaskId = os_create_task("File_task", TskFileTask, NULL, TASK_PRIO_FILE, TASK_TIME_SLIDE_FILE, &uiErr);
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create file task failed %d\n", uiErr);
    }

    /* Create communication task */
    iTaskId = os_create_task("Comm_task", TskCommTask, NULL, TASK_PRIO_COMM, TASK_TIME_SLIDE_COMM, &uiErr);
    if(iTaskId < 0)
    {
        printf("Create initial task failed %d\n", iTaskId);
    }
    if(uiErr != 0)
    {
        printf("Create communication task failed %d\n", uiErr);
    }
    
    TimeAlignment();
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TimeAlignment
*
* 功能描述: 初始化任务调用该函数，进入RTS任务前将运行时间进行初步对齐
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
static void TimeAlignment(void)
{
    uint32_t uiStartTime = 0;
    uint32_t uiDelayTime = 0;

    uiStartTime = SysGetSyncTimeLow32 ();
    
    if((uiStartTime%TASK_PERIOD) != 0)
    {
        uiDelayTime = TASK_PERIOD - (uiStartTime%TASK_PERIOD); 
        udelay(uiDelayTime);
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
static void TskRTSTask(void)
{
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;

    uiTimeWindow1 = RTS_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = RTS_TASK_START_TIME_OFFSET + TIME_TOLERANCE;
    
    while(1)
    {
        uiStartTime = SysGetSyncTimeLow32 ();
        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            /* Monitor module cycle function */
            MonitorCycle();
    
            /* Manager module cycle function */
            ManagerCycle();
            
            udelay(1000);//浪费一些时间确保同一周期内不会执行两次

            //uiEndTime = SysGetSyncTimeLow32 ();
            //printf("RTS task start=%d  end=%d   %d\n", uiStartTime/10, uiEndTime/10, (uiEndTime - uiStartTime)/10 );
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: TskDiagTask
*
* 功能描述: 自检周期任务入口函数
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
static void TskDiagTask(void)
{
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;
    
    
    
    uiTimeWindow1 = DIAG_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;
    while(1)
    {  
//        printf("  TskDiagTask  \n");
        
        
        uiStartTime = SysGetSyncTimeLow32 ();
        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            //DiagCycle();
            SetLEDState(LED_NAME_PASS, LED_ON); //diag pass
            udelay(1000);//浪费一些时间确保同一周期内不会执行两次
            //uiEndTime = SysGetSyncTimeLow32 ();
            //printf("Diag task start=%d  end=%d   %d\n", uiStartTime/10, uiEndTime/10, (uiEndTime - uiStartTime)/10 );
        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskFileTask
*
* 功能描述: 文件管理周期任务入口函数
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
static void TskFileTask(void)
{
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;
    
    uiTimeWindow1 = FILE_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;
    
    
    
    while(1)
    {
//        printf("  TskFileTask  \n");
        
        uiStartTime = SysGetSyncTimeLow32 ();
        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            //FileManagerCycle();
            udelay(1000);//浪费一些时间确保同一周期内不会执行两次
            //uiEndTime = SysGetSyncTimeLow32 ();
            //printf("File task start=%d  end=%d   %d\n", uiStartTime/10, uiEndTime/10, (uiEndTime - uiStartTime)/10 );
        }
        
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: TskCommTask
*
* 功能描述: 通讯周期任务入口函数
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
static void TskCommTask(void)
{
    uint32_t uiStartTime = 0;
    //uint32_t uiEndTime = 0;
    uint32_t uiTimeWindow1 = 0;
    uint32_t uiTimeWindow2 = 0;
    
    uiTimeWindow1 = COMM_TASK_START_TIME_OFFSET;
    uiTimeWindow2 = uiTimeWindow1 + TIME_TOLERANCE;
    
    while(1)
    {
        uiStartTime = SysGetSyncTimeLow32 ();
        if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
        {
            CommCycle();
            CMBusProtocolCycle();
            udelay(1000);//浪费一些时间确保同一周期内不会执行两次
            //uiEndTime = SysGetSyncTimeLow32 ();
            //printf("Comm task start=%d  end=%d   %d\n", uiStartTime/10, uiEndTime/10, (uiEndTime - uiStartTime)/10 );
        }
    }

    return;
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


