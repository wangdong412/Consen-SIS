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
#include <Srv/Scheduler/include/stdio.h>
#include <Srv/Scheduler/include/string.h>
#include "Srv/Diag/inc/CodeTest.h"
#include "Srv/Diag/inc/DiagCommon.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static RtsCodeHeader_t *s_pstRTSCodeHeader;
static RtsCode_t s_stRTSCode[NUM_OF_RTS_CODE];
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
* 函数名称: CodeTestInit
*
* 功能描述: 代码自检初始化
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
void CodeTestInit(void)
{
    return;
}

/*
***********************************************************************************************************************
* 函数名称: CodeTestCycle
*
* 功能描述: 代码自检管理函数，完成代码分片计算CRC的整个过程调度。
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
uint32_t CodeTestCycle(puint32_t puiVectorTest)
{
    uint32_t uiFinished = TEST_FINISHED_FALSE;
    uint32_t uiCodeCRC = 0;
    uint32_t uiCodeSize = 0;
    uint32_t uiTmp = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint32_t uiPower[7] = {1000000,100000,10000,1000,100,10,1};
    uint32_t uiCrc[NUM_OF_RTS_CODE];
    puint8_t pucBuf = NULL;
    uint32_t uiCount = 0;
    DiagCtrlInfo_t  * volatile pstThis = NULL;

    for(j = 0; j < NUM_OF_RTS_CODE; j++)
    {
        if(RTS_CODE_A == j)
        {
            s_stRTSCode[j].uiStartAddr = RTS_CODE_A_CONTENT_OFFSET_ADDR;
            s_pstRTSCodeHeader = (RtsCodeHeader_t*)RTS_CODE_A_OFFSET_ADDR;
        }
        else
        {
            s_stRTSCode[j].uiStartAddr = RTS_CODE_B_CONTENT_OFFSET_ADDR;
            s_pstRTSCodeHeader = (RtsCodeHeader_t*)RTS_CODE_B_OFFSET_ADDR;
        }

        /* 取出CRC */
        uiCodeCRC = 0;
        for(i=0;i<8;i++)
        {
            if((s_pstRTSCodeHeader->ucCRC[i] >= 0x30) && (s_pstRTSCodeHeader->ucCRC[i] <= 0x39) )
            {
                uiCodeCRC |= (s_pstRTSCodeHeader->ucCRC[i]-0x30);
            }
            else if((s_pstRTSCodeHeader->ucCRC[i] >= 0x61) && (s_pstRTSCodeHeader->ucCRC[i] <= 0x66))
            {
                uiCodeCRC |= (s_pstRTSCodeHeader->ucCRC[i]-0x61+0x0a);
            }
            else
            {
                printf("crc error\n");
            }
            if(i < 7)
            {
                uiCodeCRC = uiCodeCRC << 4;
            }
        }
        s_stRTSCode[j].uiCRC = uiCodeCRC;
        //printf("CRC%d =%x\n", j, uiCodeCRC);

        /* 取出code size */
        uiCodeSize = 0;
        for(i = 0; i < 7; i++)
        {
            if((s_pstRTSCodeHeader->ucSize[i] >= 0x30) && (s_pstRTSCodeHeader->ucSize[i] <= 0x39) )
            {
                uiTmp = (s_pstRTSCodeHeader->ucSize[i]-0x30);
                //printf("%d ", uiTmp);
            }
            else
            {
                printf("code size error\n");
            }

            uiCodeSize += (uiTmp * uiPower[i]);
        }
        s_stRTSCode[j].uiCodeSize = uiCodeSize;
        //printf("\nCode%d Size =%d\n",j, uiCodeSize);

        //printf("Code%d Start Address =%x\n",j, s_stRTSCode[j].uiStartAddr);
    }

    /* RTS BIN CRC校验 */
    for(j = 0; j < NUM_OF_RTS_CODE; j++)
    {
        pucBuf = (puint8_t)s_stRTSCode[j].uiStartAddr;
        uiCount = s_stRTSCode[j].uiCodeSize;
        uiCrc[j] = SysCrc32Cal(0, pucBuf, uiCount);
        //printf("crc=%x count=%d crc=%x\n", uiCrc[j], uiCount, s_stRTSCode[j].uiCRC);
    }

    if((uiCrc[RTS_CODE_A] == s_stRTSCode[RTS_CODE_A].uiCRC) && (uiCrc[RTS_CODE_B] == s_stRTSCode[RTS_CODE_B].uiCRC))
    {
        uiFinished = TEST_FINISHED_TRUE;
    }
    else
    {
        /* code自检失败 */
        /*获取全局诊断参数信息首地址*/
        pstThis = DiagCommGetDiagCtrlInfo();
        pstThis->uiCodeDiagVec = FLAG_FALSE;
        uiFinished = TEST_FINISHED_TRUE;
        printf("code test failed.\n");
    } 
    
    return uiFinished;
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

