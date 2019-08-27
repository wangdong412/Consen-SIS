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
*  文件名称    : SysStateMngt.c
*
*  功能简介    : 系统资源管理模块中的状态管理模块，供其他状态管理功能相关模块调用
*
*  作者            : 刘阳
*
*  创建日期    : 2016-04-04
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
#include "../../Scheduler/include/string.h"
#include "../inc/CommSysStateMngt.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static task_status_t s_emTaskStatus[NUM_OF_PM][MAX_TASK_NUM];

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: SysGetTaskStatus
*
* 功能描述: 获取指定PM内指定任务的任务状态
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
task_status_t SysGetTaskStatus(PMController_t emPMId, Task_ID_t emTaskID)
{
    return s_emTaskStatus[emPMId][emTaskID];
}
/*
***********************************************************************************************************************
* 函数名称: SysSetTaskStatus
*
* 功能描述: 设置指定PM内指定任务的任务状态
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
void SysSetTaskStatus(PMController_t emPMId, Task_ID_t emTaskID, task_status_t emTaskStatus)
{
    if((emPMId < NUM_OF_PM) && (emTaskID < MAX_TASK_NUM))
    {
        s_emTaskStatus[emPMId][emTaskID] = emTaskStatus;
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