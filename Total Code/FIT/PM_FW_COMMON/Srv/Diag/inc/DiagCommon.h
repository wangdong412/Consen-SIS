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
*  文件名称    : DiagCommon.h
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-11-24
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef _DIAGCOMM_H
#define _DIAGCOMM_H

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

#define TRUE (1)
#define FALSE (0)

#define HARDCODE_ONE    (1U)

/* 顺序启动正常 */
#define SYS_CHECK_SEQ_OK                                   (0x00000001U)
/* 顺序启动异常 */
#define SYS_CHECK_SEQ_ERR                                  (0x00000000U)

#define FLGA_DONE_TRUE                      (0x00000001U)   /* 完成标志                         */
#define FLGA_DONE_FALSE                     (0x00000000U)   /* 未完成标志                       */

#define FLGA_ENABLE                         (0x00000001U)   /* 使能标志                         */
#define FLGA_DISABLE                        (0x00000000U)   /* 禁止标志                         */

#define MASK_SETALL                         (0xFFFFFFFFU)   /* 全掩码                           */
#define MASK_CLRALL                         (0x00000000U)   /* 清除掩码                         */
#define MASK_LOWHALF                        (0x0000FFFFU)   /* 低半字掩码                       */
#define MASK_HIGHHALF                       (0xFFFF0000U)   /* 高半字掩码                       */

#define TEST_FINISHED_TRUE                  (0x00000000U)   /* 测试完成标志                     */
#define TEST_FINISHED_FALSE                 (0x00000002U)   /* 测试未完成标志                   */
#define ERR_MSG_LEN                         (0x00000064U)   /* 错误信息最大包含字符数           */

#define DIAG_SEQ_MAX                        (0x0000002CU)   /* 最大支持顺序自检函个数      44-1 */
#define DIAG_SEQ_GRP_MAX                    (0x00000002U)   /* 最大支持并行顺序自检组数         */
#define DIAG_SEQ_CHECK_END                  (DIAG_SEQ_MAX - 1U)/* 顺序自检完成标志              */

#define REGS_STORE_MAX                      (0x00000020U)   /* 保存寄存器最大个数               */
#define REGS_EXT_MAX                        (0x00000010U)   /* 异常扩展信息字个数               */

#define FLAG_TRUE                           (0x11555555U)   /* 异常自检模块使用的成功标志       */
#define FLAG_FALSE                          (0x00AAAAAAU)   /* 异常自检模块使用的失败标志       */

#define FLAG_TEST                           (0x11555555U)   /* 当前为自检状态                   */
#define FLAG_NORMAL                         (0x00AAAAAAU)   /* 当前为非自检状态                 */

#define EXCPTINFO_VALID                     (0x11FFCCCCU)   /* 状态信息有效                     */
#define EXCPTINFO_INVALID                   (0x000033CCU)   /* 状态信息无效                     */

#define BOARDDIAG_MAGIC1                    (0xDB011111U)   /* 板级自检控制状态字幻数           */

#define CPU_MAGIC1                          (0xDB111111U)   /* 代码自检控制状态字幻数           */
#define CPU_MAGIC2                          (0xDB122222U)   /* 代码自检控制状态字幻数           */

#define CODE_MAGIC1                         (0xDB211111U)   /* 代码自检控制状态字幻数           */
#define CODE_MAGIC2                         (0xDB222222U)   /* 代码自检控制状态字幻数           */

#define RAM_MAGIC1                          (0xDB311111U)   /* 代码自检控制状态字幻数           */
#define RAM_MAGIC2                          (0xDB322222U)   /* 代码自检控制状态字幻数           */

#define EXCPT_MAGIC1                        (0xDB411111U)   /* 异常控制状态字幻数               */
#define EXCPT_MAGIC2                        (0xDB422222U)   /* 异常控制状态字幻数               */

#define DIAGINFOFLASH_MAGIC                 (0xDB611111U)   /* 存FLASH信息幻数 */

/*****************************诊断地址分配************************************/
#define TEST_ADDRLINE_27_P                  (0x03FFFFF0U)   /* 地址线测试地址，第27根地址线取0  */
#define TEST_ADDRLINE_27_N                  (0x07FFFFF0U)   /* 地址线测试地址，第27根地址线取1  */

#define DIAG_ADDRLINE_ADDR                  (0x04000000U)   /* 地址线测试基地址                 */
#define DIAG_DATALINE_ADDR                  (0x02000000U)   /* 数据线测试基地址                 */
#define DIAG_ECC_ADDR                       (0x00ffc000U)   /* ECC 自检基地址                   */
#define DIAG_MULTECC_ADDR                   (0x00ffc300U)   /* 双位ECC自检基地址                */
#define DIAG_ECC_SIZE
#define DIAG_DSI_ADDR                       (0xFFFFc000U)   /* DSI异常自检测试地址              */
#define DIAG_ALIGN_BASEADDR                 (CFG_DATA_BASE + CFG_TESTDATA_OFFSET_A + 0x11)
#define DIAG_PROGRAM_BASEADDR               (CFG_DATA_BASE + CFG_TESTDATA_OFFSET_A)
#define DIAG_ISI_BASEADDR                   (0xec000010U)

#define DIAG_FIR_LSU_ADDR                   0x4000000//(0x00FF8000U)   /* 地址空间A片内LSU测试地址         */
#define DIAG_SEC_LSU_ADDR                   (0x013F8000U)   /* 地址空间B片内LSU测试地址         */
#define DIAG_THI_LSU_ADDR                   (0x017F8000U)   /* 地址空间C片内LSU测试地址         */
#define START_ADD_AREA_DATA_TEST_CPU_A_0    (0x4000000U)   /* CORE0 data cache测试地址  */
#define START_ADD_AREA_DATA_TEST_CPU_B_0    (0x4004000U)   /* CORE0 data cache测试地址  */

#define START_ADD_AREA_DATA_TEST_CPU_A_1    (0x0200000U)   /* CORE1 data cache测试地址  */
#define START_ADD_AREA_DATA_TEST_CPU_B_1    (0x0204000U)   /* CORE1 data cache测试地址  */



/**************************错误掩码定义*************************************/
#define MASK_ADDRESSLINE                    (0x10000000U)   /* 地址线测试失败掩码               */
#define MASK_DATALINE                       (0x20000000U)   /* 数据线测试失败掩码               */
#define MASK_CODE                           (0x08000000U)   /* 代码CRC校验失败掩码              */
#define MASK_RAM                            (0x00000001U)   /* 内存自检失败掩码                 */
#define MASK_CACHE                          (0x00000002U)   /* 内存自检失败掩码                 */

/*以下是异常自检掩码*/
#define MASK_ECC_INJECT                     (0x40000000U)   /* ECC 错误注入失败掩码             */
#define MASK_ECC_SINGLE                     (0x80000000U)   /* 单位ECC 错误掩码                 */
#define MASK_ECC_DOUBLE                     (0x01000000U)   /* 双位ECC 错误掩码                 */
#define MASK_ECC_ACE                        (0x04000000U)   /* 自动校准错误掩码                 */
#define MASK_ALIGN                          (0x00000001U)   /* 对齐异常测试失败掩码             */
#define MASK_DSI                            (0x00000010U)   /* DSI 异常测试失败掩码             */
#define MASK_ISI                            (0x00000100U)   /* ISI 异常测试失败掩码             */
#define MASK_PROGRAM                        (0x00001000U)   /* Program异常测试失败掩码          */
#define MASK_DIVZERO                        (0x00002000U)   /* 除零自检测试失败掩码             */
#define MASK_MACHINE                        (0x00010000U)   /* Machine 异常测试失败掩码         */
#define MASK_ECC                            (0x00100000U)   /* ECC 测试失败掩码                 */
/*Program异常类型掩码*/
#define PROGRAMMASK                         (0x001E0000U)   /* Program异常掩码                  */
#define FLOATEXCPT                          (0x00100000U)   /* 浮点型异常                       */
#define ILLINSEXCPT                         (0x00080000U)   /* 非法指令调用异常                 */
#define PRIINSEXCPT                         (0x00040000U)   /* 特权指令调用权限异常             */
#define TRAPEXCPT                           (0x00020000U)   /* 指令捕获                         */

#define FPSCR_MASK                          (0xBFF80000U)   /* 浮点型指令错误掩码               */
#define FPSCR_OX                            (0x90000000U)   /* 浮点数操作上溢状态位掩码         */
#define FPSCR_UX                            (0x88000000U)   /* 浮点数操作下溢状态位掩码         */
#define FPSCR_ZX                            (0x84000000U)   /* 浮点数操作除零状态位掩码         */
#define FPSCR_XX                            (0x82000000U)   /* 浮点数操作转换失真状态位掩码     */
#define FPSCR_VXSNAN                        (0xA1000000U)   /* 浮点数操作操作数非数字状态位掩码 */
#define FPSCR_VXISI                         (0xA0800000U)   /* ∞- ∞异常状态位掩码             */
#define FPSCR_VXIDI                         (0xA0400000U)   /* ∞/ ∞异常状态位掩码             */
#define FPSCR_VXZDZ                         (0xA0200000U)   /* 0 / 0 异常状态位掩码             */
#define FPSCR_VXIMZ                         (0xA0100000U)   /* ∞* 0*异常状态位掩码             */
#define FPSCR_VXVC                          (0xA0080000U)   /* 浮点型比较指令错误状态位掩码     */
#define FPSCR_VXCVI                         (0xA0000100U)   /* 转换指令错误状态位掩码           */

/*以下是CPU相关自检掩码*/
#define MASK_SPR_CRIT_INTERW                (0x10000000U)   /* Critical Spr字间自检错误掩码     */
#define MASK_SPR_CRIT_INTRAW                (0x20000000U)   /* Critical Spr字内自检错误掩码     */
#define MASK_IU1                            (0x00000001U)   /* 指令单元1自检错误掩码            */
#define MASK_IU2                            (0x00000002U)   /* 指令单元2自检错误掩码            */
#define MASK_GPR_INTERW                     (0x00000004U)   /* 通用寄存器字间错误自检错误掩码   */
#define MASK_GPR_INTRAW                     (0x00000008U)   /* 通用寄存器字内错误自检错误掩码   */
#define MASK_BPU_UNCOND                     (0x00000010U)   /* 无条件分支跳转指令自检错误掩码   */
#define MASK_FPR_INTERW                     (0x00000020U)   /* 浮点型寄存器字间错误自检错误掩码 */
#define MASK_FPR_INTRAW                     (0x00000040U)   /* 浮点型寄存器字内错误自检错误掩码 */
#define MASK_FPSCR                          (0x00000080U)   /* FPSCR寄存器错误自检错误掩码      */
#define MASK_PIPELINE                       (0x00000100U)   /* 流水线自检错误掩码               */
#define MASK_CR_TEST                        (0x00000200U)   /* CR寄存器自检错误掩码             */
#define MASK_LR_TEST                        (0x00000400U)   /* LR寄存器自检错误掩码             */
#define MASK_SPR_INTERW                     (0x00000800U)   /* SPR寄存器字间错误自检错误掩码    */
#define MASK_SPR_INTRAW                     (0x00001000U)   /* SPR寄存器字内错误自检错误掩码    */
#define MASK_SPRG                           (0x00002000U)   /* SPRG寄存器自检错误掩码           */
#define MASK_FPU1                           (0x00004000U)   /* 浮点型指令1自检错误掩码          */
#define MASK_FPU2                           (0x00008000U)   /* 浮点型指令2自检错误掩码          */

#define MASK_BPU_COND                       (0x80000000U)   /* 条件分支跳转指令自检错误掩码     */
#define MASK_BPU_COND_BCA                   (0x00800000U)   /* bca指令自检错误掩码              */
#define MASK_BPU_COND_BCLA                  (0x00400000U)   /* bcla指令自检错误掩码             */
#define MASK_BPU_UNCOND_BA_BLA              (0x00200000U)   /* 无条件跳转指令自检错误掩码       */

#define MASK_LSU_CONN_DATA                  (0x00000020U)   /* Load/Store单元数据自检错误掩码   */
#define MASK_LSU_EFF_ADDRESS                (0x00000040U)   /* Load/Store地址有效性自检错误掩码 */
#define MASK_LSU_INSTR_SET                  (0x00000080U)   /* Load/Store单元指令自检错误掩码   */

#define MASK_DATA_CACHE                     (0x02000000U)   /* Data Cache自检错误掩码           */
#define MASK_INSTR_CACHE                    (0x40000000U)   /* 指令Cache自检错误掩码            */
/**********************复位模式设置**********************/
#define MASK_RCWHR_EN                       (0x00800000U)   /* 看门狗使能(复位后立即开始计数)   */
#define MASH_WAT_EN                         (0x00000004U)   /* 看门狗定时器启动                 */
#define MASH_WAT_RESET_MODE                 (0x00000002U)   /* 1--复位; 0--Machine Check 异常   */
#define MASH_WAT_DIVIDE                     (0x00000001U)   /* 看门狗时钟分频                   */

#define DIAG_LOGID_HEADER                   (0x0000db00U)   /* 自检失败日志记录号前缀 */

#define DIAG_LOGINFO_BASESEQ                (0x00000004U)   /* 自检信息记录起始页面索引 */
/* 自检信息记录起始地址 */
#define DIAG_LOGINFO_BASEADDR               (BSP_NORFLASH_BASE + DIAG_LOGINFO_BASESEQ*BSP_NORFLASH_BLKSIZE)
#define DIAG_LOGINFO_MAXNUM                 (0x00000004U)   /* 自检信息最大记录页面数 */
#define DIAG_LOG_TIMEOUT                    (9000U)         /* 写Flash超时时间 */
#define BRDDIAG_RELCTN_STACKSIZE            (0x00001000U)   /* 重定位时栈合理大小     */
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
/*CPU类自检序列*/
typedef enum
{
    TESTLR = 0U,                        /*LR寄存器自检                      */
    TESTCR,                             /*CR寄存器自检                      */
    TESTGPR_INTERWORD,                  /*通用寄存器字间错误自检            */
    TESTGPR_INTRAWORD,                  /*通用寄存器字内错误自检            */
    TESTSPR_INTERWORD,                  /*特殊功能寄存器字间错误自检        */
    TESTSPR_INTRAWORD,                  /*特殊功能寄存器字内错误自检        */
    TESTSPRG,                           /*SPRG寄存器自检                    */
    TESTIU,                             /*指令单元自检                      */
    TESTCONBRANCHINST,                  /*条件分支跳转指令自检              */
    TESTUNCONBRANCHINST,                /*无条件分支跳转指令自检            */
    TESTPIPELINE,                       /*流水线自检                        */
    TESTLSU,                            /*Load/Store单元自检                */
    TESTCRISPR,                         /*Critical SPR寄存器自检            */
    TESTDATACACHE,                      /*Data Cache自检                    */
    MAX_CPU,                            /*自检序列号尾                      */
    TESTDATALINE,                       /*数据线自检                        */
    TESTADDRLINE,                       /*地址线自检                        */
}CPUDiagSequence;

typedef enum
{
    DIAG_CPU = 0U,                      /*CPU测试序列号                     */
    DIAG_CODE,                          /*代码测试序列号                    */
    DIAG_RAM,                           /*RAM测试序列号                     */
    DIAG_EXCPT,                         /*异常测试序列号                    */
    DIAG_CACHE,
    MAX_DIAG
}DiagSequence;

typedef enum
{
    NOERR = 0U,                         /* CPU测试序列号                   0*/
    CODE_INPUT_ERR,                     /* 输入参数地址为空                1*/
    CODE_MAGIC_ERR,                     /* 幻数错误                        2*/
    CODE_TABNUM_ERR,                    /* 地址表个数错误                  3*/
    CODE_TABID_ERR,                     /* 当前表ID错误                    4*/
    CODE_BLOCKNUM_ERR,                  /* 地址块个数错误                  5*/
    CODE_BLOCKID_ERR,                   /* 地址块ID错误                    6*/
    CODE_SECTNUM_ERR,                   /* 地址段个数错误                  7*/
    CODE_SECTID_ERR,                    /* 地址段ID错误                    8*/
    CODE_STEP_ERR,                      /* 步进距离设置错误                9*/
    CODE_CURADDR_ERR,                   /* 当前地址错误                     */
    MAX_ERR
}CodeCheckErrType;

/*错误类别*/
typedef enum
{
    PARA_ERR,                           /*输入参数错误                      */
    REGISTER_ERR,                       /*注册函数失败                      */
    TEST_FAILED,                        /*自检失败                          */
    IEC_EXCPT,                          /*iec 工程执行异常                  */
    RTS_EXCPT,                          /*程序异常                          */
    MAX_ERRType
}ErrorType;

/* 异常状态 */
typedef enum
{
    ALLISOK = 0x00000000U, 
    /* IEC工程错误 */
    ERR_IEC_DIVZERO,                    /*除零异常，                                       1*/
    ERR_IEC_OVERFLOW,                   /*浮点型指令结果上溢                               2*/
    ERR_IEC_UNDERFLOW,                  /*浮点型指令结果下溢                               3*/
    ERR_IEC_INEXACT,                    /*浮点型指令结果不正确                             4*/
    ERR_IEC_INVALID_SNAN,               /*浮点型指令不可用，操作数非数字                   5*/
    ERR_IEC_VXISI,                      /*∞- ∞                                           6*/
    ERR_IEC_VXIDI,                      /*∞/ ∞                                           7*/
    ERR_IEC_VXZDZ,                      /*0 / 0                                            8*/
    ERR_IEC_VXIMZ,                      /*∞* 0                                            9*/
    ERR_IEC_VXVC,                       /*浮点型比较指令错误                              10*/
    ERR_IEC_VXCVI,                      /*浮点型转换指令错误                              11*/
    ERR_IEC_EPI,                        /*IEC入口地址错误                                 12*/
    ERR_IEC_DSI,                        /*IEC工程出现DSI异常                              13*/
    ERR_IEC_ISI,                        /*IEC工程出现ISI异常                              14*/
    ERR_IEC_MACHINE,                    /*IEC工程出现MACHINE异常                          15*/
    ERR_IEC_ALIGN,                      /*IEC工程出现ALIGN异常                            16*/
    ERR_IEC_ECC,                        /*IEC工程执行过程中出现ECC中断                    17*/
    ERR_IEC_ILLINST,                    /*IEC工程执行了非法指令                           18*/
    ERR_IEC_PRIINST,                    /*IEC工程执行了特权指令                           19*/
    ERR_IEC_UNKNOWN,                    /*IEC工程未知错误                                 20*/
    /*CPU自检失败*/
    ERR_TEST_CPU_FAILED,                /*cpu类自检失败                                   21*/
    ERR_TESTLR_TESTFAILED,              /*TESTLR 自检失败                                 22*/
    ERR_TESTCR_TESTFAILED,              /*TESTCR 自检失败                                 23*/
    ERR_TESTGPR_INTERWORD_TESTFAILED,   /*TESTGPR_INTERWORD 自检失败                      24*/
    ERR_TESTGPR_INTRAWORD_TESTFAILED,   /*TESTGPR_INTRAWORD 自检失败                      25*/
    ERR_TESTSPR_INTERWORD_TESTFAILED,   /*TESTSPR_INTERWORD 自检失败                      26*/
    ERR_TESTSPR_INTRAWORD_TESTFAILED,   /*TESTSPR_INTRAWORD 自检失败                      27*/
    ERR_TESTSPRG_TESTFAILED,            /*TESTSPRG 自检失败                               28*/
    ERR_TESTFPSCR_TESTFAILED,           /*TESTFPSCR 自检失败                              29*/
    ERR_TESTIU_TESTFAILED,              /*TESTIU 自检失败                                 30*/
    ERR_TESTCONBRANCH_TESTFAILED,       /*测试条件相对地址跳转语句 自检失败               31*/
    ERR_TESTUNCONBRANCH_TESTFAILED,     /*测试无条件相对地址跳转语句 自检失败             32*/
    ERR_TESTPIPELINE_TESTFAILED,        /*流水线测试 自检失败                             33*/
    ERR_TESTLSU_FAILED,                 /*Load/Store单元 自检失败                         34*/
    ERR_TESTCRISPR_FAILED,              /*特殊功能寄存器自检(SPRG0-7之外SPRG) 未知错误    35*/
    ERR_TESTDATACACHE_TESTFAILED,       /*数据cache自检 自检失败                          36*/
    ERR_TESTINSTCACHE_TESTFAILED,       /*指令cache自检 自检失败                          37*/
    ERR_TESTSTACK_FAILED,               /*栈地址空间自检失败                              38*/
    ERR_TESTFPR_INTERWORD_TESTFAILED,   /*FPR字内自检 自检失败                            39*/
    ERR_TESTFPR_INTRAWORD_TESTFAILED,   /*FPR字间自检 自检失败                            40*/
    ERR_TESTFPU_TESTFAILED,             /*FPR指令自检 自检失败                            41*/
    ERR_TESTDATALINE_TESTFAILED,        /*数据线自检 自检失败                             42*/
    ERR_TESTADDRLINE_TESTFAILED,        /*地址线自检 自检失败                             43*/
    ERR_TEST_CPU_UNKNOWN,               /*CPU类自检失败                                   44*/
    /*CODE自检失败*/
    ERR_SYSCODE_A_TESTFAILED,           /*rts代码A片自检失败                              45*/
    ERR_SYSCODE_B_TESTFAILED,           /*rts代码B片自检失败                              46*/
    ERR_SYSCODE_C_TESTFAILED,           /*rts代码C片自检失败                              47*/
    ERR_PRJCODE_A_TESTFAILED,           /*IEC工程代码A片自检失败                          48*/
    ERR_PRJCODE_B_TESTFAILED,           /*IEC工程代码B片自检失败                          49*/
    ERR_PRJCODE_C_TESTFAILED,           /*IEC工程代码C片自检失败                          50*/
    ERR_SYSCODE_READ_FAILED,            /*读bin文件头错误                                 51*/
    /*RAM自检失败*/
    ERR_SYSRAM_A_TESTFAILED,            /*RAM A片自检失败                                 52*/
    ERR_SYSRAM_B_TESTFAILED,            /*RAM B片自检失败                                 53*/
    ERR_SYSRAM_C_TESTFAILED,            /*RAM C片自检失败                                 54*/
    /*异常自检失败*/
    ERR_EXCPTTEST_FAILED,               /*异常自检失败                                    55*/
    ERR_TESTDSI_TESTFAILED,             /*DSI异常测试自检失败                             56*/
    ERR_TESTALIGN_TESTFAILED,           /*ALIGN异常测试自检失败                           57*/
    ERR_TESTPROGRAM_TESTFAILED,         /*PROGRAM异常测试自检失败                         58*/
    ERR_TESTMACHINE_TESTFAILED,         /*MACHINE异常测试自检失败                         59*/
    ERR_TESTECC_TESTFAILED,             /*ECC测试自检失败                                 60*/
    ERR_TESTFLT_TESTFAILED,             /*浮点型异常测试自检失败                          61*/
    ERR_TESTISI_TESTFAILED,             /*ISI异常测试自检失败                             62*/
    ERR_TEST_GETINFO_FIALED,            /* 获取异常全局状态字失败                         63*/
    /*自检环境保护*/
    ERR_TESTPRCT_FAILED,                /*自检保护环境错误                                64*/
    ERR_EXCPTTEST_UNKNOWN,              /*异常自检 未知错误                               65*/
    /*重定位*/
    ERR_RELOCATE_A_FAILED,              /*重定位失败                                      66*/
    ERR_RELOCATE_B_FAILED,              /*重定位失败                                      67*/
    ERR_RELOCATE_C_FAILED,              /*重定位失败                                      68*/
    /*系统程序异常*/
    ERR_SYSEXCPT,                       /*RTS代码出现异常                                 69*/
    ERR_SYSEXCPT_ISI,                   /*指令异常                                        70*/
    ERR_SYSEXCPT_DSI,                   /*DSI异常                                         71*/
    ERR_SYSEXCPT_PROGRAM,               /*Program异常                                     72*/
    ERR_SYSEXCPT_DIVZERO,               /*除零异常，                                      73*/
    ERR_SYSEXCPT_OVERFLOW,              /*浮点型指令结果上溢                              74*/
    ERR_SYSEXCPT_UNDERFLOW,             /*浮点型指令结果下溢                              75*/
    ERR_SYSEXCPT_INEXACT,               /*浮点型指令结果不正确                            76*/
    ERR_SYSEXCPT_INVALID_SNAN,          /*浮点型指令不可用，操作数非数字                  77*/
    ERR_SYSEXCPT_VXISI,                 /*∞- ∞                                          78*/
    ERR_SYSEXCPT_VXIDI,                 /*∞/ ∞                                          79*/
    ERR_SYSEXCPT_VXZDZ,                 /*0 / 0                                           80*/
    ERR_SYSEXCPT_VXIMZ,                 /*∞* 0                                           81*/
    ERR_SYSEXCPT_VXVC,                  /*浮点型比较指令错误                              82*/
    ERR_SYSEXCPT_VXCVI,                 /*浮点型转换指令错误                              83*/
    ERR_SYSEXCPT_ILLINST,               /*IEC工程执行了非法指令                           84*/
    ERR_SYSEXCPT_PRIINST,               /*IEC工程执行了特权指令                           85*/
    ERR_SYSEXCPT_MACHINE,               /*Machine异常                                     86*/
    ERR_SYSEXCPT_ALIGN,                 /*IEC工程出现ALIGN异常                            87*/
    ERR_SYSEXCPT_ECC_D,                 /*双位ECC异常                                     88*/
    ERR_SYSEXCPT_ECC_S,                 /*单位ECC异常                                     89*/
    ERR_SYSEXCPT_UNKNOWN,               /*RTS未知错误                                     90*/
    ERR_SEQUENCE_EXEC,                  /*顺序执行异常自检                                91*/
    MAX_ABNORM_STATUS
}abnorm_status;

/*跳转函数类型*/
typedef void (*JumpTask)(void);


/*诊断状态字*/
typedef struct DiagCtrlInfoTag
{
    uint32_t uiCurDiagZoneId;           /* 当前自检区域     */
    uint32_t uiCurDiagSeqId;            /* 当前自检序列ID   */
    uint32_t uiDiagDone;                /* 自检完成标志     */
    //uint32_t uiRelocateDone;            /* 重定位完成标志   */
    uint32_t uiDiagEnable;              /* 自检使能标志     */
    uint32_t uiCPUDiagVec;              /* cpu自检错误记录  */
    uint32_t uiCodeDiagVec;             /* 代码自检错误记录 */
    uint32_t uiRamDiagVec;              /* 内存自检错误记录 */
    uint32_t uiExceptionDiagVec;        /* 异常自检错误记录 */
}DiagCtrlInfo_t;

/*保存寄存器*/
typedef struct ExcptStoreRegsTag
{
    uint32_t uiLR;                      /* LR寄存器值       */
    uint32_t uiXER;                     /* XER寄存器值      */
    uint32_t uiSTACKPOS;                /* 栈指针寄存器值   */
    uint32_t uiCR;                      /* CR寄存器值       */
    uint32_t uiCTR;                     /* CTR寄存器值      */
    uint32_t uiPC;                      /* PC指针寄存器值   */
    uint32_t uiMSR;                     /* MSR寄存器值      */
    uint32_t uiGPRS[REGS_STORE_MAX];    /* 通用寄存器值     */
}ExcptStoreRegs_t;

/*ECC中断诊断寄存器*/
typedef struct EccStoreRegsTag
{
    uint32_t uiSbe;                 /* ERR_SBE寄存器值      */
    uint32_t uiCptrAtri;            /* 捕获属性             */
    uint32_t uiCptrAddr;            /* 捕获地址             */
    uint32_t uiCptrEcc;             /* ECC校验码捕获值      */
    uint32_t uiCptrDataHi;          /* 捕获数据高字         */
    uint32_t uiCptrDataLo;          /* 捕获数据低字         */
    uint32_t uiErrInject;           /* ECC校验码注入寄存器  */
    uint32_t uiDataInjectHi;        /* ECC数据注入寄存器    */
    uint32_t uiDataInjectLo;        /* ECC数据注入寄存器    */
}EccStoreRegs_t;
/*浮点型异常诊断寄存器*/
typedef struct FloatStoreRegsTag
{
    uint32_t uiFpscr;               /* FPSCR寄存器值    */
}FloatStoreRegs_t;

/*异常返回寄存器*/
typedef struct ExcptRetInfoTag
{
    uint32_t       uiLR;            /* LR寄存器值       */
    uint32_t       uiMSR;           /* MSR寄存器值      */
    uint32_t       uiSRR1;          /* srr1寄存器值     */
    uint32_t       uiDSISR;         /* DSISR寄存器值    */
    uint32_t       uiDAR;           /* DAR寄存器值      */
    uint32_t       uiStackPos;      /* 栈顶指针         */
    uint32_t       uiPC;            /* PC指针           */
    uint32_t       uiVector;        /* 中断向量值       */
    uint32_t       uiIrqID;         /* 中断号           */
}ExcptRetInfo_t;

/*异常处理状态*/
typedef struct ExcptHandleInfoTag
{
    uint32_t         uiTestMagic1;  /* 测试幻数1        */
    ExcptStoreRegs_t stStoreRegs;   /* 保存寄存器       */
    uint32_t         uiTestMagic2;  /* 测试幻数2        */
    uint32_t         uiCurCtrlStatus;/*当前状态标志     */
    uint32_t         uiEnterIEC;    /* 当前执行代码标志 */
    uint32_t         uiCurExecId;   /* 当前执行位置索引 */
    uint32_t         uiExcptTestSn; /* 异常自检索引     */      
    uint32_t         uiCurExcptMask;/* 异常发生次数     */
    uint32_t         uiIsValid;     /* 状态信息是否有效 */
    ExcptRetInfo_t   stExcptMsg;    /* 异常诊断信息     */
    uint32_t         uiExtendMsg[REGS_EXT_MAX];/*扩展信息*/
}ExcptHandleInfo_t;

/*总线监控仲裁*/
typedef struct
{
    uint32_t uiAcr;
    uint32_t uiAtr;
    uint32_t uiAer;
    uint32_t uiAidr;
    uint32_t uiAmr;
    uint32_t uiAeatr;
    uint32_t uiAeadr;
    uint32_t uiAerr;
}B83xxArbBusReg_t;
/*复位配置*/
typedef struct
{
    uint32_t volatile uiRcwlr;
    uint32_t volatile uiRcwhr;
    uint32_t volatile uiRsrd[2U];
    uint32_t volatile uiRsr;
    uint32_t volatile uiRmr;
    uint32_t volatile uiRpr;
    uint32_t volatile uiRcr;
    uint32_t volatile uiRcer;
}B83xxRstCfgReg_t;

/*可编程中断控制器*/
typedef struct
{
    uint32_t volatile uiSicfr;              /* 全局中断配置寄存器*/
    uint32_t volatile uiSivcr;              /* 常规中断向量寄存器*/
    uint32_t volatile uiSipnr_h;            /* 高位中断未决寄存器*/
    uint32_t volatile uiSipnr_l;            /* 低位中断未决寄存器*/
    uint32_t volatile uiSiprr_a;            /* 内部中断A组优先级设置寄存器*/
    uint32_t volatile uiSiprr_b;            /* 内部中断B组优先级设置寄存器*/
    uint32_t volatile uiSiprr_c;            /* 内部中断C组优先级设置寄存器*/
    uint32_t volatile uiSiprr_d;            /* 内部中断D组优先级设置寄存器*/
    uint32_t volatile uiSimsr_h;            /* 高位内部中断掩码寄存器*/
    uint32_t volatile uiSimsr_l;            /* 低位内部中断掩码寄存器*/
    uint32_t volatile uiSicnr;              /* 内部中断控制寄存器*/
    uint32_t volatile uiSepnr;              /* 外部中断未决寄存器*/
    uint32_t volatile uiSmprr_a;            /* 混合中断A组优先级设置寄存器*/
    uint32_t volatile uiSmprr_b;            /* 混合中断B组优先级设置寄存器*/
    uint32_t volatile uiSemsr;              /* 外部中断掩码寄存器*/
    uint32_t volatile uiSecnr;              /* 外部中断控制寄存器*/
    uint32_t volatile uiSersr;              /* 错误状态寄存器*/
    uint32_t volatile uiSermr;              /* 错误掩码寄存器*/
    uint32_t volatile uiSercr;              /* 错误控制寄存器*/
    uint32_t volatile uiSepcr;              /* 外部中断输出极性配置寄存器*/
    uint32_t volatile uiSifcr_h;            /* 高位内部中断强制寄存器*/
    uint32_t volatile uiSifcr_l;            /* 低位内部中断强制寄存器*/
    uint32_t volatile uiSefcr;              /* 外部中断强制寄存器*/
    uint32_t volatile uiSerfr;              /* 错误强制寄存器*/
    uint32_t volatile uiScvcr;              /* 关键中断向量寄存器*/
    uint32_t volatile uiSmvcr;              /* 中断向量管理寄存器*/
}B83xxIpicReg_t;

typedef struct
{
    uint32_t volatile uiCs0Bnds;            /* Cs0 片选内存范围*/
    uint32_t volatile uiReserved8;          /* 保留*/
    uint32_t volatile uiCs1Bnds;            /* Cs1 片选内存范围*/
    uint32_t volatile uiReserved1[29U];     /* 保留*/
    uint32_t volatile uiCs0Config;          /* Cs0 片选配置*/
    uint32_t volatile uiCs1Config;          /* Cs1 片选配置*/
    uint32_t volatile uiReserved2[30U];     /* 保留*/
    uint32_t volatile uiTimingCfg3;         /* DDR SDRAM时序配置*/
    uint32_t volatile uiTimingCfg0;         /* DDR SDRAM时序配置*/
    uint32_t volatile uiTimingCfg1;         /* DDR SDRAM时序配置*/
    uint32_t volatile uiTimingCfg2;         /* DDR SDRAM时序配置*/
    uint32_t volatile uiSdramCfg;           /* DDR SSRAM控制寄存器*/
    uint32_t volatile uiSdramCfg2;          /* DDR SDRAM控制寄存器2*/
    uint32_t volatile uiSdramMode;          /* DDR SDRAM模式配置寄存器*/
    uint32_t volatile uiSdramMode2;         /* DDR SDRAM模式配置寄存器2*/
    uint32_t volatile uiSdramMdCntl;        /* DDR SDRAM模式控制寄存器*/
    uint32_t volatile uiSdramInterval;      /* DDR SDRAM间隔配置寄存器*/
    uint32_t volatile uiDataInit;           /* DDR SDRAM数据初始化寄存器*/
    uint32_t volatile uiReserved9;          /* 保留*/
    uint32_t volatile uiSdramClkCntl;       /* DDR SDRAM时钟控制寄存器*/
    uint32_t volatile uiReserved3[5U];      /* 保留*/
    uint32_t volatile uiInitData;           /* DDR 初始化地址*/
}B83xxDdrMemCtrl_1_Reg_t;

typedef struct
{
    uint32_t volatile uiDataErrInjectHi;    /* 高位数据路径错误注入掩码寄存器*/
    uint32_t volatile uiDataErrInjectLo;    /* 低位数据路径错误注入掩码寄存器*/
    uint32_t volatile uiErrInject;          /* ECC纠错码错误注入掩码寄存器*/
    uint32_t volatile uiReserved6[5U];      /* 保留*/
    uint32_t volatile uiCaptureDataHi;      /* 高位错误数据捕获寄存器*/
    uint32_t volatile uiCaptureDataLo;      /* 低位错误数据捕获寄存器*/
    uint32_t volatile uiCaptureEcc;         /* ECC纠错码捕获寄存器*/
    uint32_t volatile uiReserved7[5U];      /* 保留*/
    uint32_t volatile uiErrDetect;          /* 错误发现寄存器*/
    uint32_t volatile uiErrDisable;         /* 错误禁止寄存器*/
    uint32_t volatile uiErrIntEn;           /* 错误中断使能寄存器*/
    uint32_t volatile uiCaptureAttri;       /* 错误捕获配置寄存器*/
    uint32_t volatile uiCaptureAddrs;       /* 错误捕获地址寄存器*/
    uint32_t volatile uiReserved10;         /* 保留*/
    uint32_t volatile uiErrSbe;             /* 单位ECC错误管理寄存器*/
}B83xxDdrMemCtrlReg_t;

typedef struct
{
    uint32_t uiMagic;                       /* 幻数         */
    uint32_t uiLiveTime;                    /* 生存期       */
    uint32_t uiSelfPage;                    /* 自身页面号   */
    uint32_t uiNextPage;                    /* 下一条页面号 */
    uint32_t uiReserve[4U];                 /* 扩展保留     */
}DiagInfoHeader_t;
typedef struct
{
    DiagInfoHeader_t stDiagInfoHeader;      /* 信息头       */
    DiagCtrlInfo_t * pstDiagCtrlInfo;       /* 自检控制参数 */
    ExcptHandleInfo_t * pstExcptHandleInfo; /* 异常信息     */
    void * pstMessage;                      /* 提示信息     */
}DiagInfoinFlash_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: BoardDiagErrHandle
*
* 功能描述: 板级自检失败处理函数。
*
* 输入参数:
*       uiErrType    错误类别
*       uiErrClass   错误码
*       uiSubClass   错误子码
*       pErrMsg      错误提示信息
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void BoardDiagErrHandle(uint32_t uiErrType,uint32_t uiErrClass,uint32_t uiSubClass,char_t* pchErrMsg);
/*
***********************************************************************************************************************
* 函数名称: BoardDiagHandleErr_Fatal
*
* 功能描述: 致命错误处理函数
*
* 输入参数:
*        pchErrMsg:   异常信息
*        uiSubClass:  扩展信息
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void BoardDiagHandleErr_Fatal(pchar_t pchErrMsg,uint32_t uiSubClass);
/*
***********************************************************************************************************************
* 函数名称: DiagCommGetDiagCtrlInfo
*
* 功能描述: 获取全局诊断控制信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : &s_stDiagCtrlInfo:板级自检控制参数
*
* 注意事项: 无
***********************************************************************************************************************
*/
DiagCtrlInfo_t* DiagCommGetDiagCtrlInfo(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommGetExcptCtrlInfo
*
* 功能描述: 获取异常状态信息地址。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : &s_stExcptInfo:异常控制状态信息地址
*
* 注意事项: 无
***********************************************************************************************************************
*/
ExcptHandleInfo_t* DiagCommGetExcptCtrlInfo(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommGetProgramErrType
*
* 功能描述: 判断当前Program异常子类型
*
* 输入参数: pstHandleInfo:异常诊断信息
*
* 输出参数: 无
*
* 返 回 值  : 错误类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
abnorm_status DiagCommGetProgramErrType(const ExcptHandleInfo_t * pstHandleInfo);
/*
***********************************************************************************************************************
* 函数名称: DiagCommCheckExcptInfoMagic
*
* 功能描述: 检查异常自检状态字幻数。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 错误类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DiagCommCheckExcptInfoMagic(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommCheckRunSeq
*
* 功能描述: 校验程序运行顺序。
*
* 输入参数:
*      uiGrpSn              顺序自检组号
*      uiCheckId            校验ID号
*      uiCheckMax           本项顺序自检最大个数
* 输出参数: 无
*
* 返 回 值  :
*       uiRetCheck           若校验成功: SYS_CHECK_SEQ_OK
*                           否则: SYS_CHECK_SEQ_ERR
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t DiagCommCheckRunSeq(uint32_t uiGrpSn, uint32_t uiCheckId, const uint32_t uiCheckMax);
/*
***********************************************************************************************************************
* 函数名称: DiagCommWInfo2Flash
*
* 功能描述: 写异常诊断信息至Flash。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 错误类型
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DiagCommWInfo2Flash(void * pvMsg);
/*
***********************************************************************************************************************
* 函数名称: DiagCommRInfofFlash
*
* 功能描述: 从Flash读异常诊断信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 字符串长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DiagCommRInfofFlash(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommGetLatestInfo
*
* 功能描述: 获取最新诊断信息。
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 字符串长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
DiagInfoinFlash_t * DiagCommGetLatestInfo(void);
/*
***********************************************************************************************************************
* 函数名称: CalcStrLen
*
* 功能描述: 计算字符串长度
*
* 输入参数: puiStr       字符串指针
*
* 输出参数: 无
*
* 返 回 值  : 字符串长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t CalcStrLen(puint8_t puiStr);
/*
***********************************************************************************************************************
* 函数名称: DiagCommPrintDiagVec
*
* 功能描述: 打印板级自检信息。
*
* 输入参数: pstDiagCtrlInfo:控制信息指针
*
* 输出参数: 无
*
* 返 回 值  : 字符串长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DiagCommPrintDiagVec(DiagCtrlInfo_t * pstDiagCtrlInfo);
/*
***********************************************************************************************************************
* 函数名称: DiagCommPrintExcptInfo
*
* 功能描述: 打印异常状态信息。
*
* 输入参数: pstExcptHandleInfo:异常状态信息指针
*
* 输出参数: 无
*
* 返 回 值  : 字符串长度
*
* 注意事项: 无
***********************************************************************************************************************
*/
void DiagCommPrintExcptInfo(ExcptHandleInfo_t * pstExcptHandleInfo);
/*
***********************************************************************************************************************
* 函数名称: EnableFloatException
*
* 功能描述: 使能浮点型异常。
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
void EnableFloatException(void);
/*
***********************************************************************************************************************
* 函数名称: DisableFloatException
*
* 功能描述: 禁止浮点型异常。
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
void DisableFloatException(void);

/*
***********************************************************************************************************************
* 函数名称: DiagCommSetCore0FailFlag
*
* 功能描述: 置core0自检失败标志
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
void DiagCommSetCore0FailFlag(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommSetCore1FailFlag
*
* 功能描述: 置core1自检失败标志
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
void DiagCommSetCore1FailFlag(void);
/*
***********************************************************************************************************************
* 函数名称: DiagCommGetCore0FailFlag
*
* 功能描述: 读取core0自检失败标志
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
uint32_t DiagCommGetCore0FailFlag(void);

/*
***********************************************************************************************************************
* 函数名称: DiagCommGetCore1FailFlag
*
* 功能描述: 读取core1自检失败标志
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
uint32_t DiagCommGetCore1FailFlag(void);

#endif

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
