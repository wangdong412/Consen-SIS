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
/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/


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
    /* 初始化设置重定位完成         */
    BoardDiagClrRelocateDone();
    /* 诊断使能                     */
    BoardDiagSetDiagEnable();
    /* 跳转地址初始化               */
    //pstThis->pstfFuncJump = (JumpTask)NULL;//liqi
    /* 初始化CPU 测试               */
    CPUTestInit();
    /* 初始化ram 测试               */
    RamTestInit();
    /* 异常自检初始化               */
    //ExcptTestInit();//liqi
    /* 代码测试初始化               */
    CodeTestInit();
    /* 初始化栈溢出检查环境         */
    CpuTestSetStack(SysGetCurExeIndex());
    /* 使能浮点型指令异常中断       */
    //EnableFloatException();//liqi
    
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
    /* 栈检查 */
    CpuTestCheckStack(SysGetCurExeIndex());
    
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
    uint32_t uiResult = FALSE;
    uint32_t uiVecFlag = FALSE;    
    DiagCtrlInfo_t  * volatile pstThis = NULL;

    //if(SYS_CHECK_SEQ_OK == SysCheckRunSeq(DIAG_CYCLE))
    {
        /* 获取全局诊断参数信息首地址 */
        pstThis = DiagCommGetDiagCtrlInfo();

        /* 检查诊断是否完成,如果诊断完成，进行重定位操作 */
        if(FLGA_DONE_TRUE == BoardDiagGetDiagDone())
        {
#if 1 //liqi
            /* 重定位前检查栈是否溢出 */
            CpuTestCheckStack(SysGetCurExeIndex());

            /* 清除异常状态字可用状态 */
            //ExcptTestSetCtrlInfoValid(FALSE);

            //if(DCACHE_DIS == BspStatusDcache())
            //{
            //    BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED + SysGetCurExeIndex()
            //        ,0,"data cache is disable before relocation");
            //}

            /* 重定位前刷新cache */
            //BspFlushDcache();
            /* Flash擦除操作挂起 */
            //FsEraseSuspend();
            /* 调用重定位函数    */
            //uiResult = BoardDiagJumpEntry(DiagCommGetDiagCtrlInfo());
            /* Flash擦除操作解挂 */
            //FsEraseResume();
            //BspFlushDcache();
#if 0
            if(0 != uiResult)
            {
                BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED + SysGetCurExeIndex()
                    ,uiResult,"stack check failed");
            }
#endif
            /* 检查重定位是否成功 */
            //if(FLGA_DONE_TRUE == BoardDiagGetRelocateDone())
            {
                /* 使能诊断 */
                BoardDiagCycleEnable(TRUE);
                /* 设置异常状态字可用状态 */
                //ExcptTestSetCtrlInfoValid(TRUE);
                /* 初始化栈溢出检查环境   */
                CpuTestSetStack(SysGetCurExeIndex());
                /* 清除重定位完成标志     */
                BoardDiagClrRelocateDone();
            }
            //else
            //{
            //    /* 重定位失败 */
            //    BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED + SysGetCurExeIndex()
            //                                ,0,"BoardDiagCycle relocation failed");
            //}
#endif
        }
        else
        {/*如果诊断尚未完成，继续执行未完成诊断*/
            /* 调用自检调度函数 */
            BoardDiagMngt();
            /* 栈空间检查 */
            CpuTestCheckStack(SysGetCurExeIndex());
        }

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
                 //uiResult = ExcptTestMngt(&pstThis->uiExceptionDiagVec);
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
        /*更新诊断索引ID    */
        pstThis->uiCurDiagZoneId = (SysGetCurExeIndex() + HARDCODE_ONE)%MAX_PROGRAMS;
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
    
    return     uiDoneFlag;
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

    return     uiEnFlag;
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
* 函数名称: BoardDiagGetRelocateDone
*
* 功能描述: 获取重定位完成标志。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 完成标志
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagGetRelocateDone(void)
{
    uint32_t uiReFlag = 0;
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    uiReFlag = pstThis->uiRelocateDone;                             /* 获取重定位完成标志   */

    return     uiReFlag;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagSetRelocateDone
*
* 功能描述: 设置重定位完成标志。
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
void BoardDiagSetRelocateDone(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    pstThis->uiRelocateDone = FLGA_DONE_TRUE;                       /* 设置重定位完成标志   */
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagClrRelocateDone
*
* 功能描述: 清除重定位完成标志。
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
void BoardDiagClrRelocateDone(void)
{
    DiagCtrlInfo_t * volatile pstThis = DiagCommGetDiagCtrlInfo();  /* 获取全局诊断控制信息 */
    pstThis->uiRelocateDone= FLGA_DONE_FALSE;                       /* 清除重定位完成标志   */
    
    return;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagRelocateStart
*
* 功能描述: 当自检完成时，执行重定位操作，转向诊断完成区域执行代码。
*
* 输入参数:
*       pstThis:     诊断控制信息
*      puiLrValue:  lr 寄存器值
*      uiStackPos:  当前堆栈指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void BoardDiagRelocateStart(DiagCtrlInfo_t* pstThis,uint32_t puiLrValue,uint32_t uiStackPos)
{
#if 0 //liqi
    uint32_t uiExecID = 0U;                     /* 记录当前执行代码地址空间索引 */
    uint32_t uiCurStackPos = 0U;                /* 记录栈顶位置                 */
    uint32_t uiStackSize = 0U;                  /* 记录栈大小                   */
    BinOffsetTbl_t * pstSrcTblOffset = NULL;    /* 指向源地址表                 */
    BinOffsetTbl_t * pstDstTblOffset = NULL;    /* 指向目的地址表               */

    /*判断自检是否完成*/
    if(FLGA_DONE_TRUE == BoardDiagGetDiagDone())
    {
        /*获取当前执行索引*/
        uiExecID = SysGetCurExeIndex();

        /*保护措施，当前自检空间起始索引应为执行空间索引的下一片，若不是，说明存在逻辑错误*/
        if(pstThis->uiCurDiagZoneId == ((uiExecID + CON_B)%MAX_PROGRAMS))
        {
            /*得到当前栈顶地址*/
            uiCurStackPos = uiStackPos;
            /*设置重定位完成标志*/
            BoardDiagSetRelocateDone();

            /*根据当前执行索引进入不同分支拷贝用户数据*/
            switch(uiExecID)
            {
                case EXECUTE_A:/*当前运行在地址空间A*/
                {
                    /*计算重定位地址    */
                    pstThis->pstfFuncJump = (JumpTask)(puiLrValue + CFG_DATA_SIZE);
                    /*获取源地址偏移表  */
                    pstSrcTblOffset = CodeTestGetTblOffset(EXECUTE_A);
                    /*获取目的地址偏移表*/
                    pstDstTblOffset = CodeTestGetTblOffset(EXECUTE_B);

                    /*检查返回值*/
                    if(NULL == pstSrcTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED,0U,"get src address failed");
                    }

                    /*检查返回值*/
                    if(NULL == pstDstTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED,0U,"get dst address failed");
                    }

                    /*复制DATA段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stDataSection.uiDstAddr,
                            (uint32_t)pstDstTblOffset->stDataSection.uiDataSize,
                                            (puint8_t)pstSrcTblOffset->stDataSection.uiDstAddr,
                                                (uint32_t)pstSrcTblOffset->stDataSection.uiDataSize);
                    /*复制BSS段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stBssSection.uiBssStartAddr,
                            (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize,
                                        (puint8_t)pstSrcTblOffset->stBssSection.uiBssStartAddr,
                                            (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize);
                    /*复制栈数据*/
                    uiStackSize = CFG_FIR_STACKBUTTOM - uiCurStackPos;

                    if((0U > uiStackSize) || (BRDDIAG_RELCTN_STACKSIZE < uiStackSize))
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED,0U,"stack size error");
                    }

                    BspMemcpy_ASM((puint8_t)(CFG_SEC_STACKBUTTOM - uiStackSize),uiStackSize,\
                                        (puint8_t)(CFG_FIR_STACKBUTTOM - uiStackSize),uiStackSize);

                    /*复制用户工程数据*/
                    BoardDiagCopyProgramData(CFG_DATA_SIZE);
                    /*更新堆栈指针*/
                    uiCurStackPos = uiCurStackPos + CFG_DATA_SIZE;

                    break;
                }
                case EXECUTE_B:/*当前运行在地址空间B*/
                {
                    /*计算重定位地址*/
                    pstThis->pstfFuncJump = (JumpTask)(puiLrValue + CFG_DATA_SIZE);
                    /*获取源地址偏移表*/
                    pstSrcTblOffset = CodeTestGetTblOffset(EXECUTE_B);
                    /*获取目的地址偏移表*/
                    pstDstTblOffset = CodeTestGetTblOffset(EXECUTE_C);

                    /*检查返回值*/
                    if(NULL == pstSrcTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_B_FAILED,0U,"get src address failed");
                    }

                    /*检查返回值*/
                    if(NULL == pstDstTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_B_FAILED,0U,"get dst address failed");
                    }
                    /*复制DATA段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stDataSection.uiDstAddr,
                                    (uint32_t)pstDstTblOffset->stDataSection.uiDataSize,
                                            (puint8_t)pstSrcTblOffset->stDataSection.uiDstAddr,
                                                (uint32_t)pstSrcTblOffset->stDataSection.uiDataSize);
                    /*复制BSS段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stBssSection.uiBssStartAddr,
                                (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize,
                                        (puint8_t)pstSrcTblOffset->stBssSection.uiBssStartAddr,
                                                (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize);
                    /*计算栈数据大小*/
                    uiStackSize = CFG_SEC_STACKBUTTOM - uiCurStackPos;

                    /*检查栈大小*/
                    if((0U > uiStackSize) || (BRDDIAG_RELCTN_STACKSIZE < uiStackSize))
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_B_FAILED,0U,"stack size error");
                    }
                    /*复制栈数据*/
                    BspMemcpy_ASM((puint8_t)(CFG_THI_STACKBUTTOM - uiStackSize),uiStackSize,\
                                        (puint8_t)(CFG_SEC_STACKBUTTOM - uiStackSize),uiStackSize);

                    /*复制用户工程数据*/
                    BoardDiagCopyProgramData(CFG_DATA_SIZE);
                    /*更新堆栈指针*/
                    uiCurStackPos = uiCurStackPos + CFG_DATA_SIZE;

                    break;
                }
                case EXECUTE_C:/*当前运行在地址空间C*/
                {
                    /*计算重定位地址*/
                    pstThis->pstfFuncJump = (JumpTask)(puiLrValue + CFG_DATA_SIZE - TOTAL_CFG_DATA_SIZE);
                    /*获取源地址偏移表*/
                    pstSrcTblOffset = CodeTestGetTblOffset(EXECUTE_C);
                    /*获取目的地址偏移表*/
                    pstDstTblOffset = CodeTestGetTblOffset(EXECUTE_A);

                    /*检查返回值*/
                    if(NULL == pstSrcTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_C_FAILED,0U,"get src address failed");
                    }

                    /*检查返回值*/
                    if(NULL == pstDstTblOffset)
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_C_FAILED,0U,"get dst address failed");
                    }

                    /*复制DATA段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stDataSection.uiDstAddr,
                        (uint32_t)pstDstTblOffset->stDataSection.uiDataSize,
                                            (puint8_t)pstSrcTblOffset->stDataSection.uiDstAddr,
                                                (uint32_t)pstSrcTblOffset->stDataSection.uiDataSize);
                    /*复制BSS段数据*/
                    BspMemcpy_ASM((puint8_t)pstDstTblOffset->stBssSection.uiBssStartAddr,
                        (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize,
                                        (puint8_t)pstSrcTblOffset->stBssSection.uiBssStartAddr,
                                            (uint32_t)pstSrcTblOffset->stBssSection.uiBssSize);
                    
                    /*计算栈数据大小*/
                    uiStackSize = CFG_THI_STACKBUTTOM - uiCurStackPos;
                    
                    if((0U > uiStackSize) || (BRDDIAG_RELCTN_STACKSIZE < uiStackSize))
                    {
                        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_C_FAILED,0U,"stack size error");
                    }
                    /*复制栈数据*/
                    BspMemcpy_ASM((puint8_t)(CFG_FIR_STACKBUTTOM - uiStackSize),uiStackSize,\
                                        (puint8_t)(CFG_THI_STACKBUTTOM - uiStackSize),uiStackSize);

                    /*复制用户工程数据*/
                    BoardDiagCopyProgramData(CFG_DATA_SIZE - TOTAL_CFG_DATA_SIZE);
                    /*更新堆栈指针*/
                    uiCurStackPos = uiCurStackPos + CFG_DATA_SIZE - TOTAL_CFG_DATA_SIZE;

                    break;
                }
                default:
                    break;
            }
            /*拷贝数据完成*/

            /*重定位栈指针*/
            RelocateStackPointer(uiCurStackPos);
            /*执行跳转*/
            pstThis->pstfFuncJump();
        }
        else
        {
            /*自检起始区域非当前执行区域下一片，存在逻辑错误*/
            BoardDiagClrRelocateDone();
            /*自检失败，调用错误处理函数*/
            BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED + uiExecID ,0U,
                                "BoardDiagRelocateStart test id is not the next of current exeute id");
        }
    }
    else
    {
        /*自检尚未结束即进入重定位函数，存在逻辑错误*/
        BoardDiagClrRelocateDone();
        /*自检失败，调用错误处理函数*/
        BoardDiagErrHandle(TEST_FAILED,ERR_RELOCATE_A_FAILED + uiExecID,0,
                            "BoardDiagRelocateStart-state-machine-error");
    }
#endif
    return ;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagCopyProgramData
*
* 功能描述: 复制用户工程数据至新执行区域。
*
* 输入参数: iOffset:     源地址至目的地址的偏移值
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void BoardDiagCopyProgramData(int32_t iOffset)
{
#if 0 //liqi
    uint32_t uiSrcAddress = 0U;                      /*拷贝数据源地址*/
    uint32_t uiSrcDataSize = 0U;                     /*拷贝数据源长度*/
    uint32_t uiDstAddress = 0U;                      /*拷贝数据目的地址*/
    uint32_t uiDstDataSize = 0U;                     /*拷贝数据目的长度*/
    uint8_t  uiCount = 0U;                           /*计数值*/

    if(0 != iOffset)
    {/*检查输入参数*/
        /*复制用户工程当前使用数据区数据*/
        for(uiCount = 0U ; uiCount < MAX_AREA_NUM; uiCount++)
        {
            /*获取源数据区起始地址*/
            uiSrcAddress = SysGetAreaBaseAddr(uiCount,REGION_USED);
            /*得到数据区大小*/
            uiSrcDataSize = SysGetAreaUsedLen(uiCount);
            /*计算目的地址*/
            uiDstAddress = (uint32_t)((int32_t)uiSrcAddress + iOffset);
            /*复制数据*/
            BspMemcpy_ASM((puint8_t)uiDstAddress,uiSrcDataSize,(puint8_t)uiSrcAddress,uiSrcDataSize);
        }
        /*复制用户工程通讯区数据*/
        for(uiCount = 0U ; uiCount < MAX_AREA_NUM; uiCount++)
        {
            /*得到源数据区起始地址*/
            uiSrcAddress = SysGetAreaBaseAddr(uiCount,REGION_COMM);
            /*获取数据区大小*/
            uiSrcDataSize = SysGetAreaUsedLen(uiCount);
            /*计算目的地址*/
            uiDstAddress = (uint32_t)((int32_t)uiSrcAddress + iOffset);
            /*复制数据*/
            BspMemcpy_ASM((puint8_t)uiDstAddress,uiSrcDataSize,(puint8_t)uiSrcAddress,uiSrcDataSize);
        }
    }
#endif
    return ;
}
/*
***********************************************************************************************************************
* 函数名称: BoardDiagCallIecProgram
*
* 功能描述: 调用IEC工程并保护其环境。
*
* 输入参数: puiFunc:     调用函数。
*
* 输出参数: 无
*
* 返 回 值  : 函数执行标志，TRUE为成功执行，FALSE为失败。
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t BoardDiagCallIecProgram(puint32_t puiFunc)
{
#if 0 //liqi
    uint32_t uiResult = FALSE;                                  /*函数执行标志                  */
    uint32_t uiIecStrtAddr = 0U;                                /*记录当前区域用户代码起始地址  */
    uint32_t uiCallFuncAddr = (uint32_t)puiFunc;                /*记录函数调用地址              */
    ExcptHandleInfo_t * pstExcptInfo = ExcptTestGetCtrlInfo();  /*获取异常自检全局控制字        */
    
    /* 计算测试使用地址 */
    switch(SysGetCurExeIndex())
    {
        case EXECUTE_A:
            {
                uiIecStrtAddr = CFG_FIR_USERCODEBASE;           /* A区用户代码区首地址 */
                break;
            }
        case EXECUTE_B:
            {
                uiIecStrtAddr = CFG_SEC_USERCODEBASE;           /* B区用户代码区首地址 */
                break;
            }
        case EXECUTE_C:
            {
                uiIecStrtAddr = CFG_THI_USERCODEBASE;           /* C区用户代码区首地址 */
                break;
            }
        default:
            {
                /* 使用区域A内测试地址 */
                BoardDiagErrHandle(PARA_ERR,ERR_IEC_UNKNOWN,0U,
                                "BoardDiagCallIecProgram SysGetCurExeIndex return err");
                break;
            }
    }
     /* 检查函数指针地址范围 */
    if((uiIecStrtAddr <= uiCallFuncAddr) && (uiCallFuncAddr <= (uiIecStrtAddr + CFG_USERCODE_SIZE)))
    {
        /* 进入IEC之前保护环境 */
        pstExcptInfo->uiCurCtrlStatus = FLAG_FALSE;
        pstExcptInfo->uiEnterIEC = FLAG_TRUE;
        pstExcptInfo->uiCurExcptMask = MASK_CLRALL;
        pstExcptInfo->uiCurExecId = SysGetCurExeIndex();
        
        /* 调用保护现场函数   */
        if(DCACHE_DIS != BspStatusDcache())
        {
            /* 刷新cache，异常发生时，mmu会被关闭，此时cache 不可用 */
            BspFlushDcache();
        }
        
        /* 调用执行环境保护函数 */
        ExcptTestPrtctEnv(puiFunc,NULL);
        
        /* 判断是否在执行过程中出现异常中断 */
        if(0U < pstExcptInfo->uiCurExcptMask)
        {
            /* 有异常产生，检查是何种异常 */
            switch(pstExcptInfo->uiCurExcptMask)
            {
                case MASK_ALIGN:    /* 对齐异常 */
                {
                    /*调用错误处理函数*/
                    BoardDiagErrHandle(IEC_EXCPT,ERR_IEC_ALIGN,0U,
                                "there exception happened when execute user program");
                    
                    break;
                }
                case MASK_DSI:      /* DSI异常 */
                {
                    /*调用错误处理函数*/
                    BoardDiagErrHandle(IEC_EXCPT,ERR_IEC_DSI,0U,
                                "there exception happened when execute user program");
                    
                    break;
                }
                case MASK_ISI:      /* ISI异常 */
                {
                    /*调用错误处理函数*/
                    BoardDiagErrHandle(IEC_EXCPT,ERR_IEC_ISI,0U,
                                "there exception happened when execute user program");
                    
                    break;
                }
                case MASK_PROGRAM:  /* Program异常 */
                {
                    /* 检查子错误类型 */
                    abnorm_status enErrType = DiagCommGetProgramErrType(pstExcptInfo);
                    /*调用错误处理函数*/
                    BoardDiagErrHandle(IEC_EXCPT,enErrType,0U,
                                "there exception happened when execute user program");
                    
                    break;
                }
                case MASK_ECC:      /* ECC错误 */
                {
                    /*调用错误处理函数*/
                    BoardDiagErrHandle(RTS_EXCPT,ERR_IEC_ECC,0U,
                                "there exception happened when execute user program");
                    
                    break;
                 }
                default:            /* 未知错误类型 */
                {
                    /* 调用错误处理函数 */
                    BoardDiagErrHandle(IEC_EXCPT,ERR_EXCPTTEST_UNKNOWN,0U,
                                "there exception happened when execute user program");
                    
                    break;
                }
            }      
            
        }
        /*设置执行成功标志*/
        uiResult = TRUE;
        /*复位异常状态信息*/
        ExcptTestResetCtrlInfo();
    }
    else
    {
        /*工程入口地址错误*/
        BoardDiagErrHandle(IEC_EXCPT,ERR_IEC_EPI,uiCallFuncAddr,
                                "BoardDiagCallIecProgram user program address err");
        /*执行失败        */
        uiResult = FALSE;
    }
    
    return uiResult;
#endif
    return 1;
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
/*
***********************************************************************************************************************
* 函数名称: PrintCompareDataArray
*
* 功能描述: 逐个比较两数据区数据
*
* 输入参数:
*      pucMsgStr:   文本头
*      puiSrcValue: 源数据地址指针
*      puiDstValue: 目的数据地址指针
*      uiCount:     比较数据字节数
*      ucFlag:      打印十进制或是十六进制
*
* 输出参数: 无
*
* 返 回 值  : 当前最大时间间隔
*
* 注意事项: 无
***********************************************************************************************************************
*/
void PrintCompareDataArray(puint8_t pucMsgStr, puint32_t puiSrcValue, 
                    puint32_t puiDstValue,uint32_t uiCount,uint8_t ucFlag)
{

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
