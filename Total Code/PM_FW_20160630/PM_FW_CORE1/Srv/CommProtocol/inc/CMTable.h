﻿/*
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

#ifndef CMTABLE_H_
#define CMTABLE_H_

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
*                                数据结构定义
***********************************************************************************************************************
*/
/* CM配置表 */
typedef struct SysCMTableTag
{
    /* 是否插上了CM*/
    bool_t bValid[NUM_OF_SLOT];
    /* 逻辑号 */
    uint8_t ucLogicID[NUM_OF_SLOT];
} SysCMTable_t;


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: CMTableInit
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
void CMTableInit(void);

/*
***********************************************************************************************************************
* 函数名称: UpdateCMTable
*
* 功能描述:
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
void UpdateCMTable(void);

/*
***********************************************************************************************************************
* 函数名称: CMLogicIDToSLotID
*
* 功能描述: 逻辑号转槽号
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
CMSlot_t CMLogicIDToSLotID(CMController_t emCMID);

/*
***********************************************************************************************************************
* 函数名称: CMSLotIDToLogicID
*
* 功能描述: 槽号转逻辑号
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
CMController_t CMSLotIDToLogicID(CMSlot_t emCMSlot);

extern bool_t s_bCM_slot_table[NUM_OF_SLOT]; //true代表探测到槽上有CM

#endif /* CMTABLE_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
