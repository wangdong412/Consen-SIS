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
#include <Srv/Scheduler/include/string.h>
#include <Srv/Scheduler/include/stdio.h>
#include "../inc/DiagCommon.h"
#include "Srv/SharedMem/inc/LED.h"

//static uint32_t s_uiRecordErr = FALSE;           /* 错误记录变量       */
//static uint32_t s_uiSetValueMs = 0U;             /* 看门狗时间设置变量 */
static DiagCtrlInfo_t s_stDiagCtrlInfo;          /* 板级自检控制参数   */
static ExcptHandleInfo_t s_stExcptInfo;          /* 全局异常处理状态字 */

static uint32_t s_uiSeqNum[DIAG_SEQ_GRP_MAX] = {0U};       /* 顺序号            */
static const uint32_t s_uiCheckTab[DIAG_SEQ_MAX] =         /* 顺序和预设值      */
    {
           0U,    2U, 5U,    10U,
          17U,   28U, 41U,   58U,
          77U,  100U, 129U,  160U,
         197U,  238U, 281U,  328U,
         381U,  440U, 501U,  568U,
         639U,  712U, 791U,  874U,
         963U, 1060U, 1161U, 1264U,
        1371U, 1480U, 1593U, 1720U,
        1851U, 1988U, 2127U, 2276U,
        2427U, 2584U, 2747U, 2914U,
        3087U, 3266U, 3447U, 3638U
    };
static const uint32_t s_uiAddTab[DIAG_SEQ_MAX] =          /* 顺序累加值         */
    {
        2U, 3U, 5U, 7U,
        11U, 13U, 17U, 19U,
        23U, 29U, 31U, 37U,
        41U, 43U, 47U, 53U,
        59U, 61U, 67U, 71U,
        73U, 79U, 83U, 89U,
        97U, 101U, 103U, 107U,
        109U, 113U, 127U, 131U,
        137U, 139U, 149U, 151U,
        157U, 163U, 167U, 173U,
        179U, 181U, 191U, 193U
    };

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
void BoardDiagErrHandle(uint32_t uiErrType,uint32_t uiErrClass,uint32_t uiSubClass,char_t* pchErrMsg)
{
    /* 获取异常控制信息 */
    ExcptHandleInfo_t *pstMThis = DiagCommGetExcptCtrlInfo();
    /* 获取异常诊断信息 */
    ExcptRetInfo_t    *pSThis = (ExcptRetInfo_t *)&pstMThis->stExcptMsg;
    /* LED指示状态灯操作 */
    SetLEDState(LED_NAME_FAULT, LED_ON);

    /* 记录错误日志 */
    

    /* 根据错误类别分别处理 */
    switch(uiErrType)
    {/* 判断当前错误类型*/
        case IEC_EXCPT:         /* IEC执行异常 */
            //SysSetAbnormStat(SysGetLocalConId(),(abnorm_status )uiErrClass);
            //BspMessage("PC:",&pSThis->uiPC,HEX_FLAG);
            //BspMessage("LR:",&pSThis->uiLR,HEX_FLAG);
            break;
        case RTS_EXCPT:         /* 系统程序异常 */
            //BspMessage("RTS_EXCPT:",&uiErrClass,NULL);
            //BspMessage("PC:",&pSThis->uiPC,HEX_FLAG);
            //BspMessage("LR:",&pSThis->uiLR,HEX_FLAG);
            //BspLogSave(pSThis->uiPC);
            //BspLogSave(pSThis->uiLR);
            BoardDiagHandleErr_Fatal(pchErrMsg,uiSubClass);
            break;
        case TEST_FAILED:       /* 自检失败          */
            //BspMessage("TEST_FAILED:",&uiErrClass,NULL);
            BoardDiagHandleErr_Fatal(pchErrMsg,uiSubClass);
            break;
        case PARA_ERR:          /* 输入参数错误      */
            //BspMessage("PARA_ERR:",&uiErrClass,NULL);
            BoardDiagHandleErr_Fatal(pchErrMsg,uiSubClass);
            break;
        default:
            break;
    }
    
    return;
}
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
void BoardDiagHandleErr_Fatal(pchar_t pchErrMsg,uint32_t uiSubClass)
{
    do
    {
        /* 打印板级自检信息 */
        DiagCommPrintDiagVec(&s_stDiagCtrlInfo);
        /* 打印异常信息 */
        DiagCommPrintExcptInfo(&s_stExcptInfo);
        /*打印提示信息*/
        //BspMessage(pchErrMsg,&uiSubClass,HEX_FLAG);
    }
    while(1);/* 循环打印 */
    
    return;
}
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
DiagCtrlInfo_t* DiagCommGetDiagCtrlInfo(void)
{
    return &s_stDiagCtrlInfo;
}
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
ExcptHandleInfo_t* DiagCommGetExcptCtrlInfo(void)
{
    return &s_stExcptInfo;
}
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
abnorm_status DiagCommGetProgramErrType(const ExcptHandleInfo_t * pstHandleInfo)
{
    /* 返回值 */
    abnorm_status emErrType = ALLISOK;
    /* 获取异常诊断信息  */
    const ExcptRetInfo_t * pstRetInfo = &pstHandleInfo->stExcptMsg;
    /* 获取FPSCR寄存器值 */
    const FloatStoreRegs_t  * pstFloatMsg = (FloatStoreRegs_t *)pstHandleInfo->uiExtendMsg;
    
	/* 层次判断 */
    switch(pstRetInfo->uiSRR1 & PROGRAMMASK)
    {
        case FLOATEXCPT:/*浮点型异常*/
        {
            /*1.检查是否有inexact异常*/
            if(FPSCR_OX == ((pstFloatMsg->uiFpscr)&FPSCR_OX))
            {
                /* 浮点型指令结果上溢 */
                emErrType = ERR_IEC_OVERFLOW;
            }
            else if(FPSCR_UX == ((pstFloatMsg->uiFpscr)&FPSCR_UX))
            {
                /* 浮点型指令结果下溢 */
                emErrType = ERR_IEC_UNDERFLOW;
            }
            else if(FPSCR_ZX == ((pstFloatMsg->uiFpscr)&FPSCR_ZX))
            {
                /* 除零异常 */
                emErrType = ERR_IEC_DIVZERO;
            }
            else if(FPSCR_VXSNAN == ((pstFloatMsg->uiFpscr)&FPSCR_VXSNAN))
            {
                /* 浮点型指令不可用，操作数非数字 */
                emErrType = ERR_IEC_INVALID_SNAN;
            }
            else if(FPSCR_VXISI == ((pstFloatMsg->uiFpscr)&FPSCR_VXISI))
            {
                /* ∞- ∞*/
                emErrType = ERR_IEC_VXISI;
            }
            else if(FPSCR_VXIDI == ((pstFloatMsg->uiFpscr)&FPSCR_VXIDI))
            {
                /* ∞/ ∞*/
                emErrType = ERR_IEC_VXIDI;
            }
            else if(FPSCR_VXZDZ == ((pstFloatMsg->uiFpscr)&FPSCR_VXZDZ))
            {
                /* 0 / 0*/
                emErrType = ERR_IEC_VXZDZ;
            }
            else if(FPSCR_VXIMZ == ((pstFloatMsg->uiFpscr)&FPSCR_VXIMZ))
            {
                /* ∞* 0*/
                emErrType = ERR_IEC_VXIMZ;
            }
            else if(FPSCR_VXVC == ((pstFloatMsg->uiFpscr)&FPSCR_VXVC))
            {
                /* 浮点型比较指令错误 */
                emErrType = ERR_IEC_VXVC;
            }
            else if(FPSCR_VXCVI  == ((pstFloatMsg->uiFpscr)&FPSCR_VXCVI))
            {
                /* 浮点型转换指令错误 */
                emErrType = ERR_IEC_VXCVI;
            }
            else
            {
                /* 未知异常 */
                emErrType = ERR_IEC_UNKNOWN;
            }
            break;
        }
        case ILLINSEXCPT:
        {
            /*非法指令异常*/
            emErrType = ERR_IEC_ILLINST;
            break;
        }
        case PRIINSEXCPT:
        {
            /*越权访问*/
            emErrType = ERR_IEC_PRIINST;
            break;
        }
        case TRAPEXCPT:
        {
            /*无TRAP指令*/
            emErrType = ERR_IEC_UNKNOWN;
            break;
        }
        default:
            /*未知错误*/
            emErrType = ERR_IEC_UNKNOWN;
            break;
    }
    /* 返回异常类型 */
    return emErrType;
}

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
void DiagCommCheckExcptInfoMagic(void)
{
    /* 获取异常状态指针 */
    ExcptHandleInfo_t* pstThis = DiagCommGetExcptCtrlInfo();
    /* 检查幻数 */
    if((EXCPT_MAGIC1 != pstThis->uiTestMagic1) || (EXCPT_MAGIC2 != pstThis->uiTestMagic2))
    {
        /* 幻数错误 */
        //BoardDiagErrHandle(RTS_EXCPT,LOG_ERR_CHECK_EXCPTMAGIC_ERR,0,"check excptinfo magic fail");
    }
    
    return;
}

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
uint32_t DiagCommCheckRunSeq(uint32_t uiGrpSn, uint32_t uiCheckId, const uint32_t uiCheckMax)
{
    uint32_t uiRetCheck = SYS_CHECK_SEQ_OK;                    /* 顺序校验结果      */

    /* 检查输入参数 */
    if((DIAG_SEQ_GRP_MAX > uiGrpSn) && (DIAG_SEQ_MAX > uiCheckMax))
    {
        
        /* 检查顺序和 */
        if(s_uiCheckTab[uiCheckId] != s_uiSeqNum[uiGrpSn])
        {
            /* 顺序自检失败 */
            uiRetCheck = SYS_CHECK_SEQ_ERR;
            /* 进入自检错误处理函数 */
            //BoardDiagErrHandle(TEST_FAILED,LOG_ERR_SEQUENCE_DIAG,uiCheckId,"diag sequence check failed");
        }
        else
        {
            /* 顺序自检通过 */
            uiRetCheck = SYS_CHECK_SEQ_OK;
            /* 更新顺序和 */
            s_uiSeqNum[uiGrpSn] += s_uiAddTab[uiCheckId];
        }
        /* 判断本轮顺序自检是否结束 */
        if((SYS_CHECK_SEQ_OK == uiRetCheck) && (uiCheckMax == uiCheckId))
        {
            /* 一切源于初始 */
            s_uiSeqNum[uiGrpSn] = 0U;
        }

    }
    else
    {
        /* 参数输入超限 */
        //BoardDiagErrHandle(PARA_ERR,LOG_ERR_DIAG_SEQ_PER,0U,"diag sequence parameter overflow");
    }
    /* 从哪来回哪去 */
    return uiRetCheck;
}

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
uint32_t CalcStrLen(puint8_t puiStr)
{
    uint32_t uiStrLen = 0U;
    /* 计算字符个数 */
    while('\0' != *(puiStr + uiStrLen++)){;}
    
    /* 结束符未计算在内 */
    return (--uiStrLen);
}

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
void DiagCommPrintDiagVec(DiagCtrlInfo_t * pstDiagCtrlInfo)
{

    return;
}

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
void DiagCommPrintExcptInfo(ExcptHandleInfo_t * pstExcptHandleInfo)
{

    /* 返回 */
    return;
}

