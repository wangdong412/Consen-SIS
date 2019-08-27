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
*  文件名称    : CodeTest.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-11-14
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

#include "../inc/BoardDiag.h"
#include "../inc/CPUTest.h"
#include "../inc/RamTest.h"
#include "../inc/ExceptionTest.h"
#include <Srv/Scheduler/cagos.h>
#include "Srv/SharedMem/inc/SharedMem.h"
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static char temp[140];

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static void os_switch(void);
static void CreateSwitchTask(void);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: Core1DiagInit
*
* 功能描述: 板级自检初始化
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
void Core1DiagInit(void)
{
    DiagCtrlInfo_t * pstThis = DiagCommGetDiagCtrlInfo();
    /* 初始化工程代码诊断记录变量   */
    pstThis->uiCodeDiagVec = MASK_SETALL;
    /* 初始化cpu诊断记录变量        */
    pstThis->uiCPUDiagVec = MASK_SETALL;
    /* 初始化ram诊断记录变量        */
    pstThis->uiRamDiagVec = MASK_SETALL;
    /* 初始化异常诊断记录变量       */
    pstThis->uiExceptionDiagVec = MASK_SETALL;
    /* 当前诊断序列为CPU 诊断       */
    pstThis->uiCurDiagSeqId = DIAG_CPU;
    /* 初始化诊断 区域为当前执行区域下一片 */
    pstThis->uiCurDiagZoneId = (SysGetCurExeIndex() + 1)%MAX_PROGRAMS;
    /* 清除诊断完成标志             */
    BoardDiagClrDiagDone();

    /* 诊断使能                     */
    BoardDiagSetDiagEnable();

    /* 初始化CPU 测试               */
    CPUTestInit();
    /* 初始化ram 测试               */
    RamTestInit();
    /* 异常自检初始化               */
    ExceptionTestInit();
    /* 代码测试初始化               */
    CodeTestInit();
    /* D cache test */
    DataCacheTestInit();
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: Core1DiagPowerOn
*
* 功能描述: 上电自检，只在初始化时调用一次。
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
void Core1DiagPowerOn(void)
{
    /* 获取诊断控制信息地址指针 */
    DiagCtrlInfo_t * pstThis = DiagCommGetDiagCtrlInfo();

    /* 调用上电自检 */
    CpuTestPowerOn(&pstThis->uiCPUDiagVec);
    
    return;
}

/*
***********************************************************************************************************************
* 函数名称: Core1DiagCycle
*
* 功能描述: 周期自检调度函数。
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
void Core1DiagCycle(void)
{
    bool_t bRet1 = false;
    bool_t bRet2 = false;
    uint32_t uiSignal = 0;

    /* 检查诊断是否完成,如果诊断完成，进行重定位操作 */
    if(FLGA_DONE_TRUE == BoardDiagGetDiagDone())
    {
        printf("Diag done!\n");

        do
        {
            bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
            udelay(5u);
            bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE1_DIAG_DONE0);
            udelay(5u);

        }while((SIGNAL_CORE0_DIAG_DONE0 != uiSignal) || (true != bRet1) || (true != bRet2));

        //printf("Get signal0\n");

        do
        {
            bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
            udelay(5u);
            bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE1_DIAG_DONE1);
            udelay(5u);

        }while((SIGNAL_CORE0_DIAG_DONE1 != uiSignal) || (true != bRet1) || (true != bRet2));

        //printf("Get signal1\n");

        do
        {
            bRet1 = SysGetHandshakeSignal(CORE0_TO_CORE1, &uiSignal);
            udelay(5u);
            bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE1_DIAG_DONE2);
            udelay(5u);

        }while((SIGNAL_CORE0_DIAG_DONE2 != uiSignal) || (true != bRet1) || (true != bRet2));

        //printf("Get signal2\n");

        udelay(50000);

        CreateSwitchTask();

        do
        {
            bRet2 = SysSetHandshakeSignal(CORE1_TO_CORE0, SIGNAL_CORE1_DIAG_NOT_DONE);
        }while(true != bRet2);

        /* 使能诊断 */
        BoardDiagCycleEnable(TRUE);
    }
    else
    {/*如果诊断尚未完成，继续执行未完成诊断*/
        /* 调用自检调度函数 */
        BoardDiagMngt();
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagMngt
*
* 功能描述: 依次调用各子自检函数
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
void BoardDiagMngt(void)
{
    uint32_t uiResult = FALSE;
    uint32_t uiVecFlag = FALSE;
    DiagCtrlInfo_t  * volatile pstThis = NULL;
    /*获取全局诊断参数信息首地址*/
    pstThis = DiagCommGetDiagCtrlInfo();
    
    /*检查诊断是否容许执行      */
    if(FLGA_ENABLE == BoardDiagGetDiagEnable())
    {
        /*根据诊断序列序号执行自检*/
        switch(pstThis->uiCurDiagSeqId)
        {
            case DIAG_CPU:  /*CPU相关自检   */
                 uiResult = CPUTestCycle(&pstThis->uiCPUDiagVec);
                 break;
            case DIAG_CODE: /*代码CRC自检   */
                 uiResult = CodeTestCycle(&pstThis->uiCodeDiagVec);
                 break;
            case DIAG_RAM:  /*RAM 自检      */
                 uiResult = RamTestCycle(&pstThis->uiRamDiagVec);
                 break;
            case DIAG_EXCPT:/*异常自检      */
                 uiResult = ExceptionTestCycle(&pstThis->uiExceptionDiagVec);
                 break;
            case DIAG_CACHE:/*异常自检      */
                 //uiResult = DataCacheTestCycle(&pstThis->uiRamDiagVec);
                 break;
            default:        /*参数错误      */
                 break;
        }

        /*单类诊断完成，继续下一类诊断*/
        if(TEST_FINISHED_TRUE == uiResult)
        {
            uiVecFlag = TRUE;
            /* 执行顺序自检 */
            DiagCommCheckRunSeq(1U,pstThis->uiCurDiagSeqId,MAX_DIAG); 
            /* 更新诊断序列      */
            pstThis->uiCurDiagSeqId = (pstThis->uiCurDiagSeqId + HARDCODE_ONE)%MAX_DIAG;
            
            /* 如果已经完成一轮自检，复查检查结果 */
            if(DIAG_CPU == pstThis->uiCurDiagSeqId)
            {
                if(MASK_SETALL != pstThis->uiCodeDiagVec)
                {/* 代码自检失败 */
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSCODE_A_TESTFAILED + \
                        pstThis->uiCurDiagZoneId,0,"BoardDiagCycle test failed");
                    uiVecFlag = FALSE;
                }

                if(MASK_SETALL != pstThis->uiCPUDiagVec)
                {/* CPU类自检失败 */
                    uiVecFlag = FALSE;
                    BoardDiagErrHandle(TEST_FAILED,ERR_TEST_CPU_UNKNOWN,0,"BoardDiagCycle test failed");
                }

                if(MASK_SETALL != pstThis->uiRamDiagVec)
                {/* RAM自检失败   */
                    uiVecFlag = FALSE;
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED +
                        pstThis->uiCurDiagZoneId, 0 ,"BoardDiagCycle test failed");
                }

                if(MASK_SETALL != pstThis->uiExceptionDiagVec)
                {/* 异常自检失败  */
                    uiVecFlag = FALSE;
                    BoardDiagErrHandle(TEST_FAILED,ERR_EXCPTTEST_FAILED,0,"BoardDiagCycle test failed");
                }

                /* 设置诊断完成各项标志  */
                if(TRUE == uiVecFlag)
                {
                    /* 设置诊断完成标志  */
                    BoardDiagSetDiagDone();
                    /* 清除诊断使能      */
                    BoardDiagClrDiagEnable();
                }
                else
                {
                    /* 如果诊断出现错误，不设置诊断完成标志，只清除诊断使能 */
                    BoardDiagClrDiagEnable();
                }
                /* 执行顺序自检结束 */
                DiagCommCheckRunSeq(1U,MAX_DIAG,MAX_DIAG); 
            }

        }
    }
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagCycleEnable
*
* 功能描述: 设置周期性诊断使能各标志位设置。
*
* 输入参数: uiflag:  为TRUE时使能周期诊断，否则不使能。
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagCycleEnable(uint32_t uiflag)
{ 
    uint32_t uiRet = TRUE;
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo(); /* 获取全局诊断控制信息 */

    if(TRUE ==uiflag)
    {
        /*设置诊断使能标志  */
        BoardDiagSetDiagEnable();
        /*清除诊断完成标志  */
        BoardDiagClrDiagDone();
    }
    else
    {
        /*清除诊断使能标志  */
        BoardDiagClrDiagEnable();
    }
    
    return (uiRet);
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagGetDiagDone
*
* 功能描述: 获取诊断完成标志。
*
* 输入参数: uiflag:  为TRUE时使能周期诊断，否则不使能。
*
* 输出参数: 无
*
* 返 回 值  : 诊断完成标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagGetDiagDone(void)
{
    uint32_t uiDoneFlag = 0U;                                       /* 诊断完成标记         */
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    uiDoneFlag = pstThis->uiDiagDone;
    
    return uiDoneFlag;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagSetDiagDone
*
* 功能描述: 设置诊断完成标志。
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
void BoardDiagSetDiagDone(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    pstThis->uiDiagDone = FLGA_DONE_TRUE;                           /* 写完成标记           */

    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagClrDiagDone
*
* 功能描述: 清除诊断完成标志。
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
void BoardDiagClrDiagDone(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo(); /* 获取全局诊断控制信息  */
    pstThis->uiDiagDone = FLGA_DONE_FALSE;                         /* 清除完成标记          */

    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagGetDiagEnable
*
* 功能描述: 获取诊断使能标志。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 诊断完成标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagGetDiagEnable(void)
{
    uint32_t uiEnFlag = 0U;
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo(); /* 获取全局诊断控制信息  */
    uiEnFlag = pstThis->uiDiagEnable;                              /* 获取完成标记          */

    return uiEnFlag;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagSetDiagEnable
*
* 功能描述: 设置诊断使能标志。
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
void BoardDiagSetDiagEnable(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    pstThis->uiDiagEnable = FLGA_ENABLE;                            /* 设置完成标记         */
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagClrDiagEnable
*
* 功能描述: 清除诊断使能标志。
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
void BoardDiagClrDiagEnable(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    pstThis->uiDiagEnable= FLGA_DISABLE;                            /* 清除完成标记         */
    
    return;
}


/*
***********************************************************************************************************************
* 函数名称: BoardDiagCopyProgramData
*
* 功能描述: 复制用户工程数据至新执行区域。
*
* 输入参数: uiSrcAddr:     源地址
*        iSize 拷贝数据长度
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void BoardDiagCopyProgramData(uint32_t uiSrcAddr, int32_t iSize)
{
    /*拷贝数据到新的地址映射*/
    os_copy_data(uiSrcAddr, iSize);
    /*拷贝stack到新的地址映射*/
    os_copy_stack();
    /*切换地址映射*/
    os_change_mmu();
    /*更新地址映射id（在执行了os_change_mmu之后执行）*/
    os_update_mno();

    return ;
}

/*
***********************************************************************************************************************
* 函数名称: BoardDiagCalcuMaxTime
*
* 功能描述: 统计目前输入时间中最大数值。
*
* 输入参数: uiCurTimeus:   输入用于与历史数据比较的时间间隔。
*
* 输出参数: 无
*
* 返 回 值  : 当前最大时间间隔
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagCalcuMaxTime(const uint32_t  uiCurTimeus)
{
    /*用于记录最大时间间隔          */
    static uint32_t s_uiMaxTimeus = 0U;
    /*当前时间间隔与历史最大间隔比较*/
    if(uiCurTimeus > s_uiMaxTimeus)
    {
        /*保存最大值*/
        s_uiMaxTimeus = uiCurTimeus;
    }

    return s_uiMaxTimeus;
}

static void os_switch(void)
{
    powerpcIntDisable();
    //printf("start1\n");
    os_copy_data(0x1000000,0x1000000);
    os_copy_stack();
    os_change_mmu();
    os_update_mno();
    //printf("end1\n");
    powerpcIntEnable();
}

static void CreateSwitchTask(void)
{
    int err = 0;
    /*需要在0-0x4000000区域内创建任务，来执行切换地址映射的代码*/
    //char *temp=0x110000;
    OSTaskCreate(temp,
            "switchos",
            os_switch,
            NULL,
            0,
            0x100000,
            0x100,
            0x200,
            0,
            0,
            0x100000+0x1000,
            1|2,
            &err);
}
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
