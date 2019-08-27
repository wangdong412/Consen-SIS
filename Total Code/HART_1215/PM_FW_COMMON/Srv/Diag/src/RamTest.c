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
*  文件名称    : RamTest.c
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-11-18
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
#include "../inc/RamTest.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

/*RAM自检使用的测试背景数据，执行W0或者R0操作时对应的是本数据背景*/
static uint32_t uiRamTestDBsZero[DBS_LEN] = {
                    0x00000000U,     /* 00000000 00000000 00000000 00000000 */
                    0x0000FFFFU,     /* 00000000 00000000 11111111 11111111 */
                    0x00FF00FFU,     /* 00000000 11111111 00000000 11111111 */
                    0x0F0F0F0FU,     /* 00001111 00001111 00001111 00001111 */
                    0x33333333U,     /* 00110011 00110011 00110011 00110011 */
                    0x55555555U      /* 01010101 01010101 01010101 01010101 */          
};
/*RAM自检使用的测试背景数据，执行W1或者R1操作时对应的是本数据背景*/
static uint32_t uiRamTestDBsUno[DBS_LEN] = {
                    0xFFFFFFFFU,     /* 11111111 11111111 11111111 11111111 */
                    0xFFFF0000U,     /* 11111111 11111111 00000000 00000000 */
                    0xFF00FF00U,     /* 11111111 00000000 11111111 00000000 */
                    0xF0F0F0F0U,     /* 11110000 11110000 11110000 11110000 */
                    0xCCCCCCCCU,     /* 11001100 11001100 11001100 11001100 */
                    0xAAAAAAAAU      /* 10101010 10101010 10101010 10101010 */
};

static RamTestTable_t  s_stRamAddrTable;        /*RAM诊断地址表     */
static RamDiagInfo_t   s_stRamMessage;          /*RAM诊断控制信息   */

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
void RamTestStart(puint32_t puiVectorTest);

void RamTestMarchW0(puint32_t puiVectorTest,puint32_t puiStartAddress, uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum);

void RamTestMarchR0W1R1W0R0W1(puint32_t puiVectorTest,puint32_t puiStartAddress, uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum);

void RamTestMarchR1W0W1(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope, uint32_t uiDbs_SeqNum);

void RamTestMarchR1W0W1W0(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum);

void RamTestMarchR0W1W0(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum);

puint32_t RamTestGetCurDiagAddr(const RamDiagInfo_t * pstThis);

void RamTestUpdateCurState(RamDiagInfo_t * pstThis);

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: RamTestInit
*
* 功能描述: RAM自检初始化函数。
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
void RamTestInit(void)
{
    /* 计数值 */
    uint32_t uiCount = 0U;
    /*初始化RAM自检已完成内存片数*/
    s_stRamMessage.uiCurRamSn = 0U;
    /*初始化March算法步骤*/
    s_stRamMessage.emDiagSn = W0;
    /*初始化使用的数据背景*/
    s_stRamMessage.uiDBsSn = 0U;
    
    /*初始化当前状态为等待运行*/
    s_stRamMessage.emCurStatus = RAM_DIAG_WAITSTART;
    /*初始化当前使用内存区域，为了在上电后能执行RAM自检，*/
    /*跳过重定位检查，需特别设置当前使用*/
    s_stRamMessage.emCurUsedZone = (SysGetCurExeIndex() + EXECUTE_C)%RAM_ZONE_NUM;
    /*设置RAM自检步进距离*/
    s_stRamMessage.uiAddrStep = (RAM_TEST_MINLEN/sizeof(uint32_t))>0?\
            (RAM_TEST_MINLEN - (RAM_TEST_MINLEN%sizeof(uint32_t))):sizeof(uint32_t);
    if(CFG_DATA_SIZE < s_stRamMessage.uiAddrStep)
    {
        s_stRamMessage.uiAddrStep = (CFG_DATA_SIZE/sizeof(uint32_t));
    }
    
    /*初始化RAM表，设置各段起始地址和大小*/
    s_stRamAddrTable.stDiagAddrScope[uiCount].puiStartAddr = (puint32_t)CFG_FIR_DATABASE;
    s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope = CFG_DATA_SIZE;
    /*总自检RAM字节数更新*/
    s_stRamAddrTable.uiTotalSize += s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope;

    /* 填充下一组 */
    uiCount++;
    s_stRamAddrTable.stDiagAddrScope[uiCount].puiStartAddr = (puint32_t)CFG_SEC_DATABASE;
    s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope = CFG_DATA_SIZE;
    /*总自检RAM字节数更新*/
    s_stRamAddrTable.uiTotalSize += s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope;

    /* 填充下一组 */
    uiCount++;
    s_stRamAddrTable.stDiagAddrScope[uiCount].puiStartAddr = (puint32_t)CFG_THI_DATABASE;
    s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope = CFG_DATA_SIZE;
    /*总自检RAM字节数更新*/
    s_stRamAddrTable.uiTotalSize += s_stRamAddrTable.stDiagAddrScope[uiCount].uiAddrScope;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestCycle
*
* 功能描述: RAM自检调度函数，根据当前状态决定是否执行RAM自检算法或是更新状态数据
*
* 输入参数: 无
*
* 输出参数: puiVectorTest:   错误记录变量
*
* 返 回 值  :
*       uiResult:        RAM自检完成标志TEST_FINISHED_FALSE为RAM自检未完成，
*                                           TEST_FINISHED_TRUE为完成
*
* 注意事项: 无
***********************************************************************************************************************
*/
uint32_t RamTestCycle(puint32_t puiVectorTest)
{
    uint32_t uiResult = TEST_FINISHED_FALSE;     /*测试完成标志*/

    /*判断当前自检状态*/
    switch(s_stRamMessage.emCurStatus)
    {
        case RAM_DIAG_FINISHED:/*当前状态为RAM诊断完成*/
        {
            /*设置完成标志*/
            uiResult = TEST_FINISHED_TRUE;
            /*设置状态为等待RAM自检开始*/
            s_stRamMessage.emCurStatus = RAM_DIAG_WAITSTART;
            //BspLogSave(LOG_RAMTEST_FINISH);
            printf("RAM test finished.\n");
            break;
        }
        case RAM_DIAG_WAITSTART:/*当前状态为等待RAM自检状态*/
        {
            printf("RAM test start.\n");
            /*检查重定位是否完成*/
            if(SysGetCurExeIndex() ==((s_stRamMessage.emCurUsedZone + 1)%RAM_ZONE_NUM))
            {
                /*更新当前使用内存区域*/
                //s_stRamMessage.emCurUsedZone = SysGetCurExeIndex();//liqi removed temporary
                /*设置当前状态为诊断进行*/
                s_stRamMessage.emCurStatus = RAM_DIAG_UNDERWAY;
                //BspLogSave(LOG_RAMTEST_START);
            }
            else
            {
                //BspLogSave(LOG_RAMTEST_ABORT);
            }
            break;
        }
        case RAM_DIAG_UNDERWAY:/*当前状态为诊断进行*/
        {
            /*调用RAM自检执行函数*/
            RamTestStart(puiVectorTest);
            if(MASK_SETALL == *puiVectorTest)/*检查是否测试失败*/
            {
                /*更新控制信息*/
                RamTestUpdateCurState(&s_stRamMessage);
            }
            else
            {
                /* 内存自检失败，进入错误处理函数 */
                BoardDiagErrHandle(TEST_FAILED,
                    ERR_SYSRAM_A_TESTFAILED + SysGetCurExeIndex(),0,"RamTestMngt test failed ");
            }
            break;
        }
        default:
            break;
    }
    /* 返回 */
    return uiResult;
}
/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: RamTestStart
*
* 功能描述: RAM自检执行函数，实现分片RAM自检，在初始化中修改AddrStep 可调整每次自检RAM字节数。
*
* 输入参数: 无
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestStart(puint32_t puiVectorTest)
{
    uint32_t uiDCacheFlag = FALSE;                /*cache 属性改变标志*/
    puint32_t puiAddr = NULL;                     /*地址指针，指向当前执行自检的内存块首地址*/

#if 0//liqi
    /*判断目前Data Cache状态*/
    if(DCACHE_EN == BspStatusDcache())
    {
        /*如果当前Data Cache为使能状态，禁止Data Cache，否则*/
        /*该片测试RAM地址的操作均在cache中进行，实际没有测试到RAM*/
        BspDisableDcache();
        uiDCacheFlag = TRUE;
    }
#endif

    /*检查Data Cache禁止操作是否成功*/
    if(1)//(DCACHE_DIS == BspStatusDcache())//liqi
    {
        /*根据诊断序列执行相应自检March 算法步骤*/
        switch(s_stRamMessage.emDiagSn)
        {
            case W0:
            {
                /*获取当前RAM片自检起始地址*/
                puiAddr = RamTestGetCurDiagAddr(&s_stRamMessage);

                /*执行March算法*/
                if(NULL != puiAddr)
                {            
                    /* 获取起始地址成功 */
                    RamTestMarchW0(puiVectorTest,puiAddr,s_stRamMessage.uiAddrStep, s_stRamMessage.uiDBsSn);
                }
                else
                {
                    /*更新ram当前诊断地址，调用错误处理函数处理*/
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED 
                        + SysGetCurExeIndex(),0,"RamTestMngt W0 RamTestGetCurDiagAddr is null");
                }
                break;
            }
            case R0W1R1W0R0W1:
            {
                /*获取当前RAM片自检起始地址*/
                puiAddr = RamTestGetCurDiagAddr(&s_stRamMessage);

                /*执行March算法*/
                if(NULL != puiAddr)
                {            
                    /* 获取起始地址成功 */
                    RamTestMarchR0W1R1W0R0W1(puiVectorTest,puiAddr,
                            s_stRamMessage.uiAddrStep,s_stRamMessage.uiDBsSn);
                }
                else
                {
                    /*更新ram当前诊断地址，调用错误处理函数处理*/
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                        + SysGetCurExeIndex(),0,"RamTestMngt R0W1R1W0R0W1 RamTestGetCurDiagAddr is null");
                }
                break;
            }
            case R1W0W1:
            {
                /*获取当前RAM片自检起始地址*/
                puiAddr = RamTestGetCurDiagAddr(&s_stRamMessage);

                /*执行March算法*/
                if(NULL != puiAddr)
                {            
                    /* 获取起始地址成功 */
                    RamTestMarchR1W0W1(puiVectorTest,puiAddr,
                            s_stRamMessage.uiAddrStep,s_stRamMessage.uiDBsSn);
                }
                else
                {
                    /*更新ram当前诊断地址，调用错误处理函数处理*/
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                        + SysGetCurExeIndex(),0,"RamTestMngt R1W0W1 RamTestGetCurDiagAddr is null");
                }
                break;
            }

            case R1W0W1W0:
            {
                /*获取当前RAM片自检起始地址*/
                puiAddr = RamTestGetCurDiagAddr(&s_stRamMessage);

                /*执行March算法*/
                if(NULL != puiAddr)
                {
                    /* 获取起始地址成功 */
                    RamTestMarchR1W0W1W0(puiVectorTest,puiAddr,
                            s_stRamMessage.uiAddrStep,s_stRamMessage.uiDBsSn);
                }
                else
                {
                    /*更新ram当前诊断地址，调用错误处理函数处理*/
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                        + SysGetCurExeIndex(),0,"RamTestMngt R1W0W1W0 RamTestGetCurDiagAddr is null");
                }
                break;
            }
            case R0W1W0:
            {
                /*获取当前RAM片自检起始地址*/
                puiAddr = RamTestGetCurDiagAddr(&s_stRamMessage);

                /*执行March算法*/
                if(NULL != puiAddr)
                {            
                    /* 获取起始地址成功 */
                    RamTestMarchR0W1W0(puiVectorTest,puiAddr,
                            s_stRamMessage.uiAddrStep,s_stRamMessage.uiDBsSn);
                }
                else
                {
                    /*更新ram当前诊断地址，调用错误处理函数处理*/
                    BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                        + SysGetCurExeIndex(),0,"RamTestMngt R0W1W0 RamTestGetCurDiagAddr is null");
                }
                break;
            }
            default:
                break;
        }
        /* 检查是否关闭了cache */
        if(TRUE == uiDCacheFlag)
        {
#if 0 //liqi
            /* 使能cache */
            BspEnableDcache();
            /* 恢复cache 设置失败 */
            if(DCACHE_DIS == BspStatusDcache())
            {
                BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                    + SysGetCurExeIndex(),0,"RamTestMngt failed when recover instruction cache disable status");
            }
#endif
        }
    }
    else
    {
        /* 参数错误，进入错误处理函数 */
        BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED
                    + SysGetCurExeIndex(),0,"RamTestMngt failed when disable data cache");
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: RamTestMarchW0
*
* 功能描述: 自检内存片，执行W0操作
*
* 输入参数:
*      puiStartAddress:       自检内存块起始地址
*      uiTestAddrScope:       自检内存块大小
*      uiDbs_SeqNum:          自检采用的数据背景
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestMarchW0(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum)
{
    uint32_t uiCount = 0U;                          /*计数值*/
    uint32_t uiTempScope = 0U;                      /*记录测试区域大小*/
    puint32_t puiStartAddr = puiStartAddress;       /*指向起始地址*/
    /*计算步进字个数，输入应为4字节整数倍*/
    uiTempScope = uiTestAddrScope/sizeof(uint32_t);

    /*按照升序写入数据*/
    for(uiCount = 0U; uiCount < uiTempScope ; uiCount++)
    {
        /*执行 W0 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestMarchR0W1R1W0R0W1
*
* 功能描述: 自检内存片，执行R0W1R1W0R0W1操作
*
* 输入参数:
*      puiStartAddress:       自检内存块起始地址
*      uiTestAddrScope:       自检内存块大小
*      uiDbs_SeqNum:          自检采用的数据背景
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestMarchR0W1R1W0R0W1(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum)
{
    uint32_t uiCount = 0U;                          /*计数值*/
    uint32_t uiTempScope = 0U;                      /*记录测试区域大小*/
    uint32_t uiValueTemp = 0U;                      /*中间数据*/
    puint32_t puiStartAddr = puiStartAddress;       /*指向起始地址*/
    uiTempScope = uiTestAddrScope/sizeof(uint32_t);

    /*按照升序测试*/
    for(uiCount = 0U; uiCount < uiTempScope ; uiCount++)
    {
        /*执行 R0 操作*/
        uiValueTemp = *(puiStartAddr + uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsZero[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            break;
        }
        /*执行 W1 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsUno[uiDbs_SeqNum];
        /*执行 R1 操作*/
        uiValueTemp = *(puiStartAddr + uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsUno[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            break;
        }
        /*执行 W0 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
        /*执行 R0 操作*/
        uiValueTemp = *(puiStartAddr + uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsZero[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            break;
        }
        /*执行 W1 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsUno[uiDbs_SeqNum];
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestMarchR1W0W1
*
* 功能描述: 自检内存片，执行R1W0W1操作
*
* 输入参数:
*      puiStartAddress:       自检内存块起始地址
*      uiTestAddrScope:       自检内存块大小
*      uiDbs_SeqNum:          自检采用的数据背景
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestMarchR1W0W1(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope, uint32_t uiDbs_SeqNum)
{
    uint32_t uiCount = 0U;                      /*计数值*/
    uint32_t uiTempScope = 0U;                  /*记录测试区域大小*/
    uint32_t uiValueTemp = 0U;                  /*中间数据*/
    puint32_t puiStartAddr = puiStartAddress;   /*指向起始地址*/
    uiTempScope = uiTestAddrScope/sizeof(uint32_t);            /*计算自检片内存字大小*/

    /*按照升序测试*/
    for(uiCount = 0; uiCount < uiTempScope ; uiCount++)
    {
        /*执行 R1 操作*/
        uiValueTemp = *(puiStartAddr + uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsUno[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            break;
        }
        /*执行 W0 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
        /*执行 W1 操作*/
        *(puiStartAddr + uiCount) = uiRamTestDBsUno[uiDbs_SeqNum];
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestMarchR1W0W1W0
*
* 功能描述: 自检内存片，执行R1W0W1W0操作
*
* 输入参数:
*      puiStartAddress:       自检内存块起始地址
*      uiTestAddrScope:       自检内存块大小
*      uiDbs_SeqNum:          自检采用的数据背景
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestMarchR1W0W1W0(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum)
{
    uint32_t uiCount = 0U;                          /*计数值*/
    uint32_t uiTempScope = 0U;                      /*记录测试区域大小*/
    uint32_t uiValueTemp = 0U;                      /*中间数值*/
    puint32_t puiStartAddr = puiStartAddress;       /*指向起始地址*/
    /*计算自检片内存字大小*/
    uiTempScope = uiTestAddrScope/sizeof(uint32_t);

    /*按照降序测试*/
    for(uiCount = HARDCODE_ONE; uiCount <= uiTempScope ; uiCount++)
    {
        /*执行 R1 操作*/
        uiValueTemp = *(puiStartAddr - uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsUno[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            uiValueTemp = uiRamTestDBsUno[uiDbs_SeqNum];
            break;
        }
        /*执行 W0 操作*/
        *(puiStartAddr - uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
        /*执行 W1 操作*/
        *(puiStartAddr - uiCount) = uiRamTestDBsUno[uiDbs_SeqNum];
        /*执行 W0 操作*/
        *(puiStartAddr - uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestMarchR0W1W0
*
* 功能描述: 自检内存片，执行R0W1W0操作
*
* 输入参数:
*      puiStartAddress:       自检内存块起始地址
*      uiTestAddrScope:       自检内存块大小
*      uiDbs_SeqNum:          自检采用的数据背景
*
* 输出参数: puiVectorTest:       错误记录变量
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestMarchR0W1W0(puint32_t puiVectorTest, puint32_t puiStartAddress,uint32_t uiTestAddrScope,uint32_t uiDbs_SeqNum)
{
    uint32_t uiCount = 0U;                          /*计数值*/
    uint32_t uiTempScope = 0U;                      /*记录测试区域大小*/
    uint32_t uiValueTemp = 0U;                      /*中间数值*/
    puint32_t puiStartAddr = puiStartAddress;       /*指向起始地址*/
    /*计算自检片内存字大小*/
    uiTempScope = uiTestAddrScope/sizeof(uint32_t);

    /*按照降序测试*/
    for(uiCount = HARDCODE_ONE; uiCount <= uiTempScope ; uiCount++)
    {
        /*执行 R0 操作*/
        uiValueTemp = *(puiStartAddr - uiCount);
        /*读出来的数据与数据背景中数据比较*/
        if(uiRamTestDBsZero[uiDbs_SeqNum] != uiValueTemp)
        {
            /*自检失败，设置错误掩码*/
            *puiVectorTest &= ~(MASK_RAM);
            break;
        }
        /*执行 W1 操作*/
        *(puiStartAddr - uiCount) = uiRamTestDBsUno[uiDbs_SeqNum];
        /*执行 W0 操作*/
        *(puiStartAddr - uiCount) = uiRamTestDBsZero[uiDbs_SeqNum];
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestGetCurDiagAddr
*
* 功能描述: 获取当前诊断内存块首地址。
*
* 输入参数:
*      pstThis:     RAM 自检控制信息地址指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
puint32_t RamTestGetCurDiagAddr(const RamDiagInfo_t * pstThis)
{
    uint32_t  uiStartAddr = 0U;                 /*记录测试起始地址*/
    TestOrder  emOrder = TESTORDER_UP;          /*记录当前测试增长方向*/

    /*检查输入参数*/
    if(NULL != pstThis)
    {
        /*获取当前测试增长方向*/
        if(R1W0W1W0 > pstThis->emDiagSn)
        {
            /*当前为升序*/
            emOrder = TESTORDER_UP;
        }
        else
        {
            /*当前为降序*/
            emOrder = TESTORDER_DOWN;
        }

        /*根据诊断ID获取测试起始基地址*/
        switch(pstThis->emCurUsedZone)
        {
            case EXECUTE_A:
                /*当前执行空间为A，测试B和C，如果是升序则基地址为B片起始地址，否则为C片末地址*/
                uiStartAddr = (TESTORDER_UP == emOrder)?\
                        CFG_SEC_DATABASE:(CFG_THI_DATABASE + CFG_DATA_SIZE);
                break;
            case EXECUTE_B:
                /*当前执行空间为B，测试C和A，如果是升序则基地址为C片起始地址，否则为A片末地址*/
                uiStartAddr = (TESTORDER_UP == emOrder)?\
                        CFG_THI_DATABASE:(CFG_FIR_DATABASE + CFG_DATA_SIZE);
                break;
            case EXECUTE_C:
                /*当前执行空间为C，测试A和B，如果是升序则基地址为A片起始地址，否则为B片末地址*/
                uiStartAddr = (TESTORDER_UP == emOrder)?\
                        CFG_FIR_DATABASE:(CFG_SEC_DATABASE + CFG_DATA_SIZE);
                break;
            default:
                break;
        }

        /*数据环处理*/
        if(TESTORDER_UP == emOrder)
        {
            /*如果当前测试基地址为B片起始地址，需特殊处理以便环接数据段头和尾*/
            if(CFG_THI_DATABASE == uiStartAddr)
            {
                /*根据自检序号计算自检分片起始地址*/
                if(pstThis->uiCurRamSn * pstThis->uiAddrStep >= CFG_DATA_SIZE)
                {
                    uiStartAddr = CFG_FIR_DATABASE + pstThis->uiCurRamSn * pstThis->uiAddrStep - CFG_DATA_SIZE;
                }
                else
                {
                    uiStartAddr = uiStartAddr + pstThis->uiCurRamSn * pstThis->uiAddrStep;
                }
            }
            else
            {
                /*地址连续，根据自检序号计算自检分片起始地址*/
                uiStartAddr += pstThis->uiCurRamSn * pstThis->uiAddrStep;
            }
        }
        else
        {
            /*如果当前测试基地址为B片起始地址，需特殊处理以便环接数据段头和尾*/
            if((CFG_FIR_DATABASE + CFG_DATA_SIZE) == uiStartAddr)
            {
                /*根据自检序号计算自检分片起始地址*/
                if(pstThis->uiCurRamSn * pstThis->uiAddrStep >= CFG_DATA_SIZE)
                {
                    uiStartAddr = CFG_THI_DATABASE + CFG_DATA_SIZE - \
                        (pstThis->uiCurRamSn*pstThis->uiAddrStep - CFG_DATA_SIZE);
                }
                else
                {
                    uiStartAddr = uiStartAddr - pstThis->uiCurRamSn * pstThis->uiAddrStep;
                }
            }
            else
            {
                /*地址连续，根据自检序号计算自检分片起始地址*/
                uiStartAddr -= pstThis->uiCurRamSn * pstThis->uiAddrStep;
            }
        }
    }
    else
    {
        /*输入参数指针为空*/
        BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED + SysGetCurExeIndex(),0,\
                "RamTestGetCurDiagAddr input parameter is null");
    }

    /*返回首地址*/
    return (puint32_t)uiStartAddr;
}

/*
***********************************************************************************************************************
* 函数名称: RamTestUpdateCurState
*
* 功能描述: 更新RAM自检控制信息。
*
* 输入参数:
*      pstThis:     RAM 自检控制信息地址指针
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
void RamTestUpdateCurState(RamDiagInfo_t * pstThis)
{
    /*检查输入参数*/
    if(NULL != pstThis)
    {
        /*检查是否已完成整个数据区域单次自检*/
        if(RAM_TEST_TOTALSIZE <= (pstThis->uiCurRamSn + HARDCODE_ONE) * pstThis->uiAddrStep)
        {
            /*已完成整个数据区单次自检，计算是否需增长测试算法序列还是测试背景数据*/
            if(R0W1W0 <= pstThis->emDiagSn)
            {
                /*已完成March所有序列，返回起始序列*/
                pstThis->emDiagSn = W0;

                /*检查是否已轮测完所有背景数据*/
                if(DBS_LEN <= (pstThis->uiDBsSn + HARDCODE_ONE))
                {
                    /*已完成所有背景数据测试*/
                    /*设置完成标志*/
                    pstThis->emCurStatus = RAM_DIAG_FINISHED;
                    /*复位背景数据测试序号*/
                    pstThis->uiDBsSn = 0U;
                }
                else
                {
                    /*未完成所有背景数据测试，选取下一个背景数据*/
                    pstThis->uiDBsSn++;
                }
            }
            else
            {
                /*未完成March算法所有序列，增长序列号*/
                pstThis->emDiagSn = (RamDiagSn)(pstThis->emDiagSn + HARDCODE_ONE);
            }

            /*刷新步进距离*/
            //pstThis->uiAddrStep = pstThis->uiAddrStep;//liqi
            /*返回起始地址*/
            pstThis->uiCurRamSn = 0U;
        }
        else/*未到测试地址空间尾*/
        {
            /*更新地址序列*/
            pstThis->uiCurRamSn++;
        }

    }
    else
    {
        /*输入参数指针为空*/
        BoardDiagErrHandle(TEST_FAILED,ERR_SYSRAM_A_TESTFAILED + SysGetCurExeIndex(),0,
                    "RamTestUpdateCurState input parameter is null");
    }
    return;
}




