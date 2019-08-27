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
*  文件名称    : CPUTest.c
*
*  功能简介    : 
*
*  作者            : 李琦
*
*  创建日期    : 2016-11-09
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
#include <Srv/Scheduler/cagos.h>
#include "../inc/CPUTest.h"
#include "../inc/SysTest.h"
#include "../inc/CPUTestAsm.h"

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
* 函数名称: CPUTestInit
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
void CPUTestInit(void)
{

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CPUTestCycle
*
* 功能描述: 自检
*
* 输入参数: 无
*
* 输出参数: puiVectorTest
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CPUTestCycle(puint32_t puiVectorTest)
{
    static uint32_t s_uiCTestSn = TESTLR;       /*记录测试序列序号*/
    uint32_t uiFinished = TEST_FINISHED_FALSE;
    uint32_t uiResult = FALSE;                  /*单项测试返回值*/

    /*检查输入参数*/
    if((NULL != puiVectorTest) &&(MASK_SETALL == *puiVectorTest))
    {
        powerpcIntDisable();
        /*根据测试序列序号跳转至相应测试函数*/
        switch(s_uiCTestSn)
        {
            case TESTLR:
                CpuTestLr(puiVectorTest);/* 调用LR寄存器测试函数 */
                //printf("cpu test lr\n");
                uiResult = TRUE;
                break;
            case TESTCR:
                CpuTestCr(puiVectorTest);/* 调用CR寄存器测试函数 */
                //printf("cpu test cr\n");
                uiResult = TRUE;
                break;
            case TESTGPR_INTERWORD:
                CpuTestGprInterWord(puiVectorTest);/* 调用GPR寄存器测试函数 */
                //printf("cpu test gpr\n");
                uiResult = TRUE;
                break;
            case TESTGPR_INTRAWORD:
                CpuTestGprIntraWord(puiVectorTest);/* 调用GPR寄存器测试函数 */
                //printf("cpu test gpr\n");
                uiResult = TRUE;
                break;
            case TESTSPR_INTERWORD:
                CpuTestSprInterWord(puiVectorTest);/* 调用SPR寄存器测试函数 */
                //printf("cpu test spr\n");
                uiResult = TRUE;
                break;
            case TESTSPR_INTRAWORD:
                CpuTestSprIntraWord(puiVectorTest);/* 调用SPR寄存器测试函数 */
                //printf("cpu test spr\n");
                uiResult = TRUE;
                break;
            case TESTSPRG:
                CpuTestSprg(puiVectorTest);        /* 调用SPRG寄存器测试函数 */
                //printf("cpu test sprg\n");
                uiResult = TRUE;
                break;
            case TESTIU:
                CpuTestIu(puiVectorTest);          /* 调用IU测试函数 */
                //printf("cpu test iu\n");
                uiResult = TRUE;
                break;
            case TESTCONBRANCHINST:
                CpuTestConBranchInst(puiVectorTest);/* 调用条件分支测试函数 */
                //printf("cpu test branch inst\n");
                uiResult = TRUE;
                break;
            case TESTUNCONBRANCHINST:
                CpuTestUnConBranchInst(puiVectorTest);/* 调用无条件分支测试函数 */
                //printf("cpu test branch inst\n");
                uiResult = TRUE;
                break;
            case TESTPIPELINE:
                CpuTestPipeLine(puiVectorTest);     /* 调用流水线测试函数 */
                //printf("cpu test pipe line\n");
                uiResult = TRUE;
                break;
            case TESTLSU:
                CpuTestLsu(puiVectorTest);          /* 调用LSU测试函数 */
                //printf("cpu test lsu\n");
                uiResult = TRUE;
                break;
            case TESTCRISPR:
                CpuTestCriSpr(puiVectorTest);/* 调用Critical SPR寄存器测试函数 */
                //printf("cpu test cri spr\n");
                uiResult = TRUE;
                break;
            case TESTDATACACHE:
                uiResult = CpuTestDataCache(puiVectorTest);/* 调用数据cache测试函数 */
                //printf("cpu test d-cache\n");
                break;
            default:
                break;
        }

        /*检查单项测试是否完成，若完成则更新测试序列序号*/
        if(TRUE == uiResult)
        {
            /*如果已经完成所有单项测试，置本类测试完成标志*/
            if(MAX_CPU <= ++s_uiCTestSn)
            {
                s_uiCTestSn = TESTLR;
                uiFinished = TEST_FINISHED_TRUE;
                //printf("cpu %d\n",*puiVectorTest);
                /* 执行顺序自检结束 */
                DiagCommCheckRunSeq(0U,MAX_CPU_SQ,MAX_CPU_SQ);
            }
        }
        powerpcIntEnable();
    }

    return uiFinished;
}

/*
***********************************************************************************************************************
* 函数名称: CpuTestPowerOn
*
* 功能描述: CPU自检上电调度函数。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest            错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void CpuTestPowerOn(puint32_t puiVectorTest)
{
    uint32_t uiRet = FALSE;
    /*测试 lr 寄存器*/
    uiRet = CpuTestLr(puiVectorTest);
    /*测试 cr 寄存器*/
    uiRet = CpuTestCr(puiVectorTest);
    /*测试通用寄存器，针对字间耦合错误类型*/
    uiRet = CpuTestGprInterWord(puiVectorTest);
    /*测试通用寄存器，针对字内耦合错误类型*/
    uiRet = CpuTestGprIntraWord(puiVectorTest);
    /*测试特殊功能寄存器，针对字间耦合错误类型*/
    uiRet = CpuTestSprInterWord(puiVectorTest);
    /*测试特殊功能寄存器，针对字内耦合错误类型*/
    uiRet = CpuTestSprIntraWord(puiVectorTest);
    /*测试 SPRG0-SPRG7*/
    uiRet = CpuTestSprg(puiVectorTest);
    /*测试整形指令单元*/
    uiRet = CpuTestIu(puiVectorTest);
    /*测试条件分支跳转指令*/
    uiRet = CpuTestConBranchInst(puiVectorTest);
    /*测试无条件分支跳转指令*/
    uiRet = CpuTestUnConBranchInst(puiVectorTest);
    /*测试流水线*/
    uiRet = CpuTestPipeLine(puiVectorTest);
    /*测试存储器操作指令*/
    uiRet = CpuTestLsu(puiVectorTest);
    /*测试临界SPR 测试*/
    uiRet = CpuTestCriSpr(puiVectorTest);

    /*数据cache测试*/
    do
    {
        uiRet = CpuTestDataCache(puiVectorTest);
    }while(TRUE != uiRet);
    /* 执行顺序自检结束 */
    DiagCommCheckRunSeq(0U,MAX_CPU_SQ,MAX_CPU_SQ);
    /*数据线测试*/
    CpuTestDDRDataLine(puiVectorTest);
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CpuTestLr
*
* 功能描述: 测试链接寄存器。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestLr(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                  /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        DiagCommCheckRunSeq(0U,TESTLR,MAX_CPU_SQ);  /* 执行顺序自检 */

        /* 调用测试函数 */
        TestLR(puiVectorTest, MASK_LR_TEST);

        if(((*puiVectorTest) & MASK_LR_TEST) != MASK_LR_TEST)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TESTLR_TESTFAILED, 0, "CpuTestLr test failed");
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TESTLR_TESTFAILED, 0,
                                "CpuTestLr input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestCr
*
* 功能描述: 测试条件寄存器。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestCr(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        DiagCommCheckRunSeq(0U,TESTCR,MAX_CPU_SQ);  /* 执行顺序自检 */

        /* 调用测试函数 */
        TestCR(puiVectorTest, MASK_CR_TEST);

        if(((*puiVectorTest) & MASK_CR_TEST) != MASK_CR_TEST)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TESTCR_TESTFAILED, 0, "CpuTestCr test failed");
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TESTCR_TESTFAILED, 0, "CpuTestCr input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestGprInterWord
*
* 功能描述: 测试通用寄存器，针对字间错误类型，采用March测试方法。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestGprInterWord(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        DiagCommCheckRunSeq(0U,TESTGPR_INTERWORD,MAX_CPU_SQ);  /* 执行顺序自检 */

        /*调用测试函数*/
        GPRInterWordTest(puiVectorTest, MASK_GPR_INTERW);

        if(((*puiVectorTest) & MASK_GPR_INTERW) != MASK_GPR_INTERW)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TESTGPR_INTERWORD_TESTFAILED, 0,
                                "CpuTestGprInterWord test failed");
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TESTGPR_INTERWORD_TESTFAILED, 0,
                                "CpuTestGprInterWord input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestGprIntraWord
*
* 功能描述: 测试通用寄存器，针对字内错误类型，采用March测试方法。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestGprIntraWord(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTGPR_INTRAWORD,MAX_CPU_SQ);
#if 1 // system hang  //该问题消失  2017.03.27
        /* 调用测试函数 */
        GPRIntraWordTest(puiVectorTest, MASK_GPR_INTRAW);

        if(((*puiVectorTest) & MASK_GPR_INTRAW) != MASK_GPR_INTRAW)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTGPR_INTRAWORD, 0,
                                        "CpuTestGprIntraWord test failed");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTGPR_INTRAWORD, 0,
                                "CpuTestGprIntraWord input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestSprInterWord
*
* 功能描述: 测试特殊功能寄存器，针对字间错误类型，采用March测试方法。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestSprInterWord(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTSPR_INTERWORD,MAX_CPU_SQ);
        /* 调用测试函数 */
        SPRInterwordTest(puiVectorTest, MASK_SPR_INTERW);

        if(((*puiVectorTest) & MASK_SPR_INTERW) != MASK_SPR_INTERW)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTSPR_INTERWORD, 0,
                                            "CpuTestSprInterWord test failed");
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTSPR_INTERWORD, 0,
                                        "CpuTestSprInterWord input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestSprIntraWord
*
* 功能描述: 测试特殊功能寄存器，针对字内错误类型，采用March测试方法。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestSprIntraWord(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTSPR_INTRAWORD,MAX_CPU_SQ);
        /* 调用测试函数 */
        SPRIntrawordTest(puiVectorTest, MASK_SPR_INTRAW);

        if(((*puiVectorTest) & MASK_SPR_INTRAW) != MASK_SPR_INTRAW)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTSPR_INTRAWORD, 0,
                                        "CpuTestSprIntraWord test failed");
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTSPR_INTRAWORD, 0,
                                  "CpuTestSprIntraWord input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestSprg
*
* 功能描述: 测试SPRG0-SPRG7。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestSprg(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTSPRG,MAX_CPU_SQ);
#if 1 //liqi system hang //core0 core1该问题消失  2017.03.27
        /* 调用测试函数 */
        SPRGTest(puiVectorTest, MASK_SPRG);

        if((*(puiVectorTest) & MASK_SPRG) != MASK_SPRG)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTSPRG,0,
                                        "CpuTestSprg test failed");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTSPRG,0,
                                    "CpuTestSprg input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestConBranchInst
*
* 功能描述: 测试条件分支跳转指令。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestConBranchInst(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTCONBRANCHINST,MAX_CPU_SQ);
#if 1 //not crush
        /* 调用测试函数 */
        BPUConBranchTest(puiVectorTest, MASK_BPU_COND);

        if((*(puiVectorTest) & MASK_BPU_COND) == MASK_BPU_COND)
        {/* 检查测试结果正确下一项测试 */
#if 1 //
            /* 调用测试函数 */
            BPUConBranchBcaTest(puiVectorTest, MASK_BPU_COND_BCA);

            if((*(puiVectorTest) & MASK_BPU_COND_BCA) == MASK_BPU_COND_BCA)
            {/* 检查测试结果正确下一项测试 */
                /* 调用测试函数 */
                BPUConBranchBclaTest(puiVectorTest, MASK_BPU_COND_BCLA);

                if((*(puiVectorTest) & MASK_BPU_COND_BCLA) != MASK_BPU_COND_BCLA)
                {/* 检查测试结果错误 */
                    uiTestCalled = FALSE;
                    /* 调用错误处理函数 */
                    BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTCONBRANCHINST,0,
                                "CpuTestConBranchInst test failed MASK_BPU_COND_BCLA");
                }
            }
            else
            {/* 检查测试结果错误 */
                uiTestCalled = FALSE;
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTCONBRANCHINST,0,
                                "CpuTestConBranchInst test failed MASK_BPU_COND_BCA");
            }
#endif
        }
        else
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTCONBRANCHINST, 0,
                                "CpuTestConBranchInst test failed MASK_BPU_COND");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTCONBRANCHINST,0,
                                "CpuTestConBranchInst input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestUnConBranchInst
*
* 功能描述: 测试无条件分支跳转指令。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestUnConBranchInst(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTUNCONBRANCHINST,MAX_CPU_SQ);
#if 1 //not crush
        /* 调用测试函数 */
        BPUBranchUnconTest(puiVectorTest, MASK_BPU_UNCOND);

        if(((*puiVectorTest) & MASK_BPU_UNCOND) == MASK_BPU_UNCOND)
        {/* 检查测试结果正确下一项测试 */
            /* 调用测试函数 */
#if 1 //

            BPUBranchUnconBaBlaTest(puiVectorTest, MASK_BPU_UNCOND_BA_BLA);

            if(((*puiVectorTest) & MASK_BPU_UNCOND_BA_BLA) != MASK_BPU_UNCOND_BA_BLA)
            {/* 检查测试结果错误 */
                uiTestCalled = FALSE;
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTUNCONBRANCHINST, 0,
                            "CpuTestUnConBranchInst test failed MASK_BPU_UNCOND_BA_BLA");
            }
#endif
        }
        else
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTUNCONBRANCHINST,0,
                            "CpuTestUnConBranchInst test failed MASK_BPU_UNCOND");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTUNCONBRANCHINST, 0,
                            "CpuTestUnConBranchInst input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestIu
*
* 功能描述: 测试指令单元。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestIu(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTIU,MAX_CPU_SQ);
        /* 调用测试函数 */
        IU1Test(puiVectorTest, MASK_IU1);

        if(((*puiVectorTest) & MASK_IU1) != MASK_IU1)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTIU,0, "CpuTestIu1 test failed");
        }
        else
        {
            /* 调用测试函数 */
            IU2Test(puiVectorTest, MASK_IU2);

            if((*(puiVectorTest) & MASK_IU2) != MASK_IU2)
            {/* 检查测试结果错误 */
                uiTestCalled = FALSE;
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTIU,0, "CpuTestIu2 test failed");
            }
        }
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTIU,0, "CpuTestIu input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestPipeLine
*
* 功能描述: 测试流水线。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestPipeLine(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTPIPELINE,MAX_CPU_SQ);
#if 1 //liqi
        /* 调用测试函数 */
        PipelineTest(puiVectorTest, MASK_PIPELINE);

        if(((*puiVectorTest) & MASK_PIPELINE) != MASK_PIPELINE)
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTPIPELINE,0,
                                        "CpuTestPipeLine test failed");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTPIPELINE,0,
                            "CpuTestPipeLine input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestLsu
*
* 功能描述: 测试Load、store单元。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestLsu(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    excute_id emCurExecId = SysGetCurExeIndex();            /* 当前运行索引 */
    puint32_t puiUseAddArea = NULL;                         /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTLSU,MAX_CPU_SQ);
#if 1 //liqi
        /* 根据当前运行区域ID计算测试地址 */

        if(0==get_cpu_no())
        {
            /* core0 测试地址 */
            puiUseAddArea = (puint32_t)START_ADD_AREA_DATA_TEST_CPU_A_0;
        }
        else
        {
            /* core1 测试地址 */
            puiUseAddArea = (puint32_t)START_ADD_AREA_DATA_TEST_CPU_A_1;
        }


        /* 调用测试函数 */
        LSUDataTest(puiVectorTest, MASK_LSU_CONN_DATA, puiUseAddArea);

        if((*(puiVectorTest) & MASK_LSU_CONN_DATA) == MASK_LSU_CONN_DATA)
        {/* 检查测试结果正确下一项测试 */
            /* 进行地址有效性测试 */
            LSUEffectiveAddressTest(puiVectorTest, MASK_LSU_EFF_ADDRESS, puiUseAddArea);

            if(((*puiVectorTest) & MASK_LSU_EFF_ADDRESS) == MASK_LSU_EFF_ADDRESS)
            {/* 检查测试结果正确下一项测试 */
                /* 进行指令测试 */
#if 0

                LSUInstructionTest(puiVectorTest, MASK_LSU_INSTR_SET, puiUseAddArea);

                if((*(puiVectorTest) & MASK_LSU_INSTR_SET) != MASK_LSU_INSTR_SET)
                {/* 检查测试结果错误 */
                    uiTestCalled = FALSE;
                    BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTLSU, 0,
                            "CpuTestLsu test failed MASK_LSU_INSTR_SET");
                }
#endif
            }
            else
            {/* 检查测试结果错误 */
                uiTestCalled = FALSE;
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTLSU,0,
                            "CpuTestLsu test failed MASK_LSU_EFF_ADDRESS");
            }
        }
        else
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTLSU, 0,
                        "CpuTestLsu test failed MASK_LSU_CONN_DATA");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTLSU, 0,
                        "CpuTestLsu input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestCriSpr
*
* 功能描述: 测试临界 Spr
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestCriSpr(puint32_t puiVectorTest)
{
    uint32_t uiTestCalled = TRUE;                           /* 测试结果返回值 */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTCRISPR,MAX_CPU_SQ);
#if 0 //crush
        /* 调用测试函数 */
        CriticalSprTestInter(puiVectorTest, MASK_SPR_CRIT_INTERW);

        if(((*puiVectorTest) & MASK_SPR_CRIT_INTERW) == MASK_SPR_CRIT_INTERW)
        {/* 检查测试结果正确下一项测试 */
            CriticalSprTestIntra(puiVectorTest, MASK_SPR_CRIT_INTRAW);

            if((*(puiVectorTest) & MASK_SPR_CRIT_INTRAW) != MASK_SPR_CRIT_INTRAW)
            {/* 检查测试结果错误 */
                uiTestCalled = FALSE;
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTCRISPR, 0,
                            "CpuTestCriSpr test failed MASK_SPR_CRIT_INTRAW");
            }
        }
        else
        {/* 检查测试结果错误 */
            uiTestCalled = FALSE;
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTCRISPR, 0,
                            "CpuTestCriSpr test failed MASK_SPR_CRIT_INTERW");
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        uiTestCalled = FALSE;
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTCRISPR, 0,
                            "CpuTestCriSpr input parameter is null");
    }

    return uiTestCalled;
}
/*
***********************************************************************************************************************
* 函数名称: CpuTestDataCache
*
* 功能描述: 测试数据cache。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : TRUE为测试成功，FALSE为测试失败
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CpuTestDataCache(puint32_t puiVectorTest)
{
    uint32_t uiStep = TEST_CACHE_SET_NUM;                       /* 单次测试set个数      */
    uint32_t uiFinished = FALSE;                                /* 测试完成标识         */
    uint32_t uiCacheEnFlag = FALSE;                             /* Cache使能标识        */
    uint32_t uiCurPos = 0U;                                     /* 当前检查SET号        */
    uint32_t uiTestCacheAddr1 = 0U;                             /* 测试数据地址1        */
    uint32_t uiTestCacheAddr2 = 0U;                             /* 测试数据地址2        */
    static uint32_t s_uiDataCacheRecord = 0U;                   /* 记录已测试set个数    */

    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        DiagCommCheckRunSeq(0U,TESTDATACACHE + s_uiDataCacheRecord/uiStep,MAX_CPU_SQ);
#if 1 //liqi

        if(0==get_cpu_no())
        {
            /* core0测试地址 */
            uiTestCacheAddr1 = START_ADD_AREA_DATA_TEST_CPU_A_0;
            uiTestCacheAddr2 = START_ADD_AREA_DATA_TEST_CPU_B_0;
        }
        else
        {
            uiTestCacheAddr1 = START_ADD_AREA_DATA_TEST_CPU_A_1;
            uiTestCacheAddr2 = START_ADD_AREA_DATA_TEST_CPU_B_1;
        }

        /* 先检查data cache 是否使能，如果尚未使能，先使能data cache */
        if(DCACHE_DIS == cacheIsOnD())
        {
            cacheE500EnableD();
            uiCacheEnFlag = TRUE;
        }

        /* 检查是否使能，如果没有，则之前使能失败 */
        if(DCACHE_EN == cacheIsOnD())
        {
            /* 测试前先刷新cache，避免影响系统数据 */
            //BspFlushDcache();

            /* 根据单步测试set个数，执行测试程序 */
            for(uiCurPos = s_uiDataCacheRecord; uiCurPos < (s_uiDataCacheRecord + uiStep); uiCurPos++)
            {
                /* 调用测试函数 */
                DataCacheTest(puiVectorTest, MASK_DATA_CACHE, (puint32_t)uiTestCacheAddr1, \
                                (puint32_t)uiTestCacheAddr2, uiCurPos);

                if((*(puiVectorTest) & MASK_DATA_CACHE) != MASK_DATA_CACHE)
                {/* 检查测试结果错误 */
                    /* 测试失败，调用错误处理函数处理 */
                    BoardDiagErrHandle(TEST_FAILED,ERR_TEST_CPU_FAILED + TESTDATACACHE,0,
                                                "CpuTestDataCache test failed");
                }
            }

            //BspFlushDcache();
        }
        else
        {
            /* 使能data cache失败，清除掩码，记录错误 */
            ResetTestBit(puiVectorTest, MASK_DATA_CACHE);
            BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTDATACACHE,0,
                                    "CpuTestDataCache enable data cache failed");
        }

        /* 测试结束，恢复设置 */
        if(TRUE == uiCacheEnFlag)
        {
            /* 检查恢复设置操作是否成功 */
            if(DCACHE_EN != cacheIsOnD())
            {
                /* 禁止data cache失败，清除掩码，记录错误 */
                ResetTestBit(puiVectorTest, MASK_DATA_CACHE);
                BoardDiagErrHandle(TEST_FAILED, ERR_TEST_CPU_FAILED + TESTDATACACHE, 0,
                            "CpuTestDataCache when recover data cache disable failed");
            }
        }

        /* 更新测试set 个数 */
        s_uiDataCacheRecord = uiCurPos;

        /* 判断data cache是否完成 */
        if(NUM_SET_DATA_CACHE <= s_uiDataCacheRecord)
        {
            /* 复位测试set个数 */
            s_uiDataCacheRecord = 0U;
            /* 设置完成标志 */
            uiFinished = TRUE;
        }
#endif
    }
    else
    {/* 检查输入参数错误 */
        BoardDiagErrHandle(PARA_ERR, ERR_TEST_CPU_FAILED + TESTDATACACHE, 0,
                        "CpuTestDataCache input parameter is null");
    }

    //return uiFinished;
    return TRUE;//liqi
}

/*
***********************************************************************************************************************
* 函数名称: CpuTestDDRDataLine
*
* 功能描述: 测试数据线是否正常，采用逐线检测法。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void CpuTestDDRDataLine(puint32_t puiVectorTest)
{
    uint32_t   uiCount = 0U;                                     /* 计数值                 */
    volatile uint32_t   uiTestValue = HARDCODE_ONE;              /* 存储写入数据           */
    volatile uint32_t   uiReadValue = 0U;                        /* 存储读取数据           */
    uint32_t uiRecordValue = HARDCODE_ONE;/* 当前测试的数据线对应的数据，除测试位外其他为0 */
    uint32_t uiDCacheFlag = FALSE;                               /* CACHE状态              */
    puint32_t puiAddress = (puint32_t)DATALINE_TEST_ADDR;        /* 设置数据线测试地址     */

    /*检查输入参数*/
    if((NULL != puiVectorTest) && (MASK_SETALL == *puiVectorTest))
    {/* 检查输入参数合法 */

        /* 执行顺序自检 */
        //DiagCommCheckRunSeq(0U,TESTDATALINE - TESTDATALINE,MAX_LINE_SQ);
        /* 如果此时data cache 使能，关闭，否则数据存在cache中，未通过地址线 */
        if(DCACHE_EN == cacheIsOnD())
        {
            /* 关闭data cache */
            cacheE500DisableD();
            uiDCacheFlag = TRUE;
        }

        if(DCACHE_DIS == cacheIsOnD())
        {
            /*分别测试每根数据线*/
            for(uiCount = 0U; uiCount < TEST_DATALINE_NUM ; ++uiCount)
            {
                /* 根据本次测试数据线产生数据，对测试数据线写1 */
                uiRecordValue = (1U<<uiCount);
                /*发送数据至数据线*/
                *puiAddress = uiRecordValue;

                if(uiRecordValue != *puiAddress)
                {
                    ResetTestBit(puiVectorTest, MASK_DATALINE);
                    BoardDiagErrHandle(TEST_FAILED, ERR_TESTDATALINE_TESTFAILED, 0,
                                    "CpuTestDDRDataLine  test failed");
                }
                /* 翻转测试数据线 */
                *puiAddress = 0U;

                if(*puiAddress != 0U)
                {/* 所读数据错误 */
                    ResetTestBit(puiVectorTest, MASK_DATALINE);
                    BoardDiagErrHandle(TEST_FAILED, ERR_TESTDATALINE_TESTFAILED, 0,
                                    "CpuTestDDRDataLine  test failed");
                }
            }

            if(TRUE == uiDCacheFlag)
            {
                /* 恢复data cache 设置 */
                cacheE500EnableD();

                /* 恢复cache 设置失败 */
                if(DCACHE_DIS == cacheIsOnD())
                {
                    ResetTestBit(puiVectorTest, MASK_DATALINE);
                    BoardDiagErrHandle(TEST_FAILED, ERR_TESTDATALINE_TESTFAILED, 0,
                        "CpuTestDDRDataLine failed when recover instruction cache disable status");
                }

            }
        }
        else
        {
            ResetTestBit(puiVectorTest, MASK_DATALINE);
            BoardDiagErrHandle(TEST_FAILED, ERR_TESTDATALINE_TESTFAILED, 0,
                        "CpuTestDDRDataLine failed when disable data cache");
        }
    }
    else
    {/* 检查输入参数错误 */
        ResetTestBit(puiVectorTest, MASK_DATALINE);
        BoardDiagErrHandle(PARA_ERR, ERR_TESTDATALINE_TESTFAILED, 0,
                        "CpuTestDDRDataLine input parameter is null");
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckRunEsitoTest
*
* 功能描述: 检查掩码位设置是否正确。
*
* 输入参数: uiMask           错误掩码
*
* 输出参数: puiVectorTest    错误记录变量
*
* 返 回 值  : 返回TRUE表示无错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CheckRunEsitoTest(puint32_t puiVectorTest, uint32_t uiMaskTest)
{
    uint32_t uiRetVal = FALSE;                          /* 返回值 */
    uint32_t uiValTest = *(puiVectorTest) & uiMaskTest; /* 设置掩码 */

    if((MASK_CLRALL != uiValTest) && (MASK_SETALL != uiValTest))
    {/* 检查掩码 */
        uiRetVal = TRUE;
    }

    return uiRetVal;
}
/*
***********************************************************************************************************************
* 函数名称: ResetTestBit
*
* 功能描述: 清除掩码。
*
* 输入参数: uiMaskTest               掩码
*
* 输出参数: puiVectorTest            错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void ResetTestBit(puint32_t puiVectorTest, uint32_t uiMaskTest)
{
    /* 检查输入参数 */
    if(NULL != puiVectorTest)
    {
        *(puiVectorTest) &= ~(uiMaskTest);/*清除掩码*/
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: CheckAddressDataCache
*
* 功能描述: 检查data cache 测试使用的地址是否正确。
*
* 输入参数: uiAddress   地址
*        uiFlag      地址类型标识。
*
* 输出参数:
*
* 返 回 值  : 返回1表示正确，返回0表示错误
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CheckAddressDataCache(uint32_t uiAddress, uint32_t uiFlag)
{
    uint32_t uiResult = TRUE;                       /* 测试结果返回值       */
    uint32_t uiTestCacheAddr1 = 0U;                 /* 测试数据地址1        */
    uint32_t uiTestCacheAddr2 = 0U;                 /* 测试数据地址2        */
#if 1
    /* 获取预设地址 */
    /* 使用区域测试地址 */
    if(0==get_cpu_no())
    {
        uiTestCacheAddr1 = START_ADD_AREA_DATA_TEST_CPU_A_0;
        uiTestCacheAddr2 = START_ADD_AREA_DATA_TEST_CPU_B_0;
    }
    else
    {
        uiTestCacheAddr1 = START_ADD_AREA_DATA_TEST_CPU_A_1;
        uiTestCacheAddr2 = START_ADD_AREA_DATA_TEST_CPU_B_1;
    }

    /* 根据输入标志判断当前是需要检查 */
    switch(uiFlag)
    {
        case DATACACHETEST_NORMAL:  /* 检查正序地址 */
        {
            if(!(uiTestCacheAddr1 <= uiAddress && (uiAddress < (uiTestCacheAddr1 +
                NUM_SET_DATA_CACHE * NUM_BYTE_BLOCK_DATA_CACHE))))
            {
                /* 测试地址失败，设置错误标志 */
                uiResult = FALSE;
            }

            break;
        }
        case DATACACHETEST_NEGATE:  /* 检查反序序地址 */
        {
            if(!(uiTestCacheAddr2 <= uiAddress && (uiAddress < (uiTestCacheAddr2 +
                NUM_SET_DATA_CACHE * NUM_BYTE_BLOCK_DATA_CACHE))))
            {
                /* 测试地址失败，设置错误标志 */
                uiResult = FALSE;
            }

            break;
        }
        default:
        {
            uiResult = FALSE;   /* 标志输入错误 */
            break;
        }
    }
#endif
    /* 返回 */
    return uiResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
