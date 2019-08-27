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
*  文件名称    : TaskExeEntryBin1.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-11-17
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
#include "Srv/Sys/inc/CommSys.h"
#include "../inc/Task.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static uint32_t uiStartTime = 0;
//static uint32_t uiEndTime = 0;
static uint32_t uiTimeWindow1 = 0;
static uint32_t uiTimeWindow2 = 0 + TIME_TOLERANCE;

extern void powerpcIntDisable(void);
extern void powerpcIntEnable(void);
extern void ImageSwitchRTS1ToRTS0(void);

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
* 函数名称: TskRTSTaskExeEntry
*
* 功能描述: RTS周期任务入口函数
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
void TskRTSTaskExeEntry(void)
{
    /* 开中断 */
    powerpcIntEnable();

    uiStartTime = SysGetSyncTimeLow32 ();

    if(((uiStartTime%TASK_PERIOD) >= uiTimeWindow1)&&((uiStartTime%TASK_PERIOD) <= uiTimeWindow2))
    {
        //printf("bin1\n");
        /* 执行RTS任务 */
        TskRTSTaskExe();

    }
    return;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: ImageSwitch
*
* 功能描述: RTS镜像切换
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
extern uint32_t * os_get_tcb(unsigned int pos);
extern uint32_t os_set_vec(void * TEXT_BASE);

extern int32_t s_iRTSTaskId;
extern int32_t s_iUP1TaskId;
extern int32_t s_iUP2TaskId;

static uint32_t s_uiPos = 0;
static uint32_t *s_puiTCBStartAddr = NULL;
static uint32_t s_uiSp = 0;
static uint32_t *s_puiPc = 0;

void ImageSwitch(void)
{
    /* 拷贝全局变量  */
    memcpy((puint8_t)RTS2_DATA_BASE, (puint8_t)RTS1_DATA_BASE, RTS_DATA_SIZE);

    /* 拷贝堆栈 */
    memcpy((puint8_t)RTS2_RTS_STACKF_BOTTOM, (puint8_t)RTS1_RTS_STACKF_BOTTOM, RTS_STACKF_SIZE);
    memcpy((puint8_t)RTS2_RTS_STACKI_BOTTOM, (puint8_t)RTS1_RTS_STACKI_BOTTOM, RTS_STACKI_SIZE);

    /* 关中断 */
    powerpcIntDisable();
#if 1
    /* tcb sp pc */
    //------------------------------------------------------------
    s_uiPos = (s_iRTSTaskId & 0xFFFF0000)>>16;
    printf("1rtsTaskid=%x %x\n", s_iRTSTaskId, &s_iRTSTaskId);
    //tcb起始地址，第一项为sp
    s_puiTCBStartAddr = os_get_tcb(s_uiPos);
    printf("1tcb addr=%x\n", s_puiTCBStartAddr);

    //当前任务的sp内容
    s_uiSp = *s_puiTCBStartAddr;
    printf("sp=%x\n", s_uiSp);

    //改pc
    s_puiPc = (uint32_t *)(s_uiSp + 12);//当前空间中存储pc的地址
    printf("pc=%x *pc=%x\n", s_puiPc , *s_puiPc);
    s_puiPc += 0x05000000;              //新空间中存储pc的地址
    printf("new pc=%x\n", s_puiPc);
    *s_puiPc = 0x0A0FF010;              //新空间中pc的内容
    printf("new *pc=%x\n", *s_puiPc);
    
    //改sp
    s_puiTCBStartAddr += 0x05000000;    //新空间tcb的地址
    *s_puiTCBStartAddr = s_uiSp + 0x05000000;//RTS1_RTS_STACKI_BOTTOM; //新空间sp的内容
    printf("new sp=%x\n",*s_puiTCBStartAddr);

    //------------------------------------------------------------
    s_uiPos = (s_iUP1TaskId & 0xFFFF0000)>>16;
    //tcb起始地址，第一项为sp
    s_puiTCBStartAddr = os_get_tcb(s_uiPos);

    //当前任务的sp内容
    s_uiSp = *s_puiTCBStartAddr;

    //改pc
    s_puiPc = (uint32_t *)(s_uiSp + 12);//当前空间中存储pc的地址
    s_puiPc += 0x05000000;              //新空间中存储pc的地址
    *s_puiPc = 0x0A0FF800;              //新空间中pc的内容

    //改sp
    s_puiTCBStartAddr += 0x05000000;    //新空间tcb的地址
    *s_puiTCBStartAddr = s_uiSp + 0x05000000;//RTS1_UP1_STACKI_BOTTOM; //新空间sp的内容

    //------------------------------------------------------------
    s_uiPos = (s_iUP2TaskId & 0xFFFF0000)>>16;
    //tcb起始地址，第一项为sp
    s_puiTCBStartAddr = os_get_tcb(s_uiPos);

    //当前任务的sp内容
    s_uiSp = *s_puiTCBStartAddr;

    //改pc
    s_puiPc = (uint32_t *)(s_uiSp + 12);//当前空间中存储pc的地址
    s_puiPc += 0x05000000;              //新空间中存储pc的地址
    *s_puiPc = 0x0A0FFC00;              //新空间中pc的内容

    //改sp
    s_puiTCBStartAddr += 0x05000000;    //新空间tcb的地址
    *s_puiTCBStartAddr = s_uiSp + 0x05000000;//RTS1_UP2_STACKI_BOTTOM; //新空间sp的内容


    //------------------------------------------------------------
    //os_set_vec(0x0A100000);             //新空间的中断向量

    //memset((puint8_t)RTS0_RTS_STACKF_BOTTOM, 0, RTS_STACKF_SIZE);
    //memset((puint8_t)RTS0_RTS_STACKI_BOTTOM, 0, RTS_STACKI_SIZE);
    //memset((puint8_t)RTS0_DATA_BASE, 0, RTS_DATA_SIZE);
#endif
    printf("R1-2\n");

    /* 改sp和lr */
    //ImageSwitchRTS1ToRTS2();            //任务到新空间运行

    return;
}
#endif

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


