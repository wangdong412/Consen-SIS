/*
***********************************************************************************************************************
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
* 
*  模块名称    : 算法库模块
*
*  文件名称    : ExtLib.c
*
*  功能简介    : 外部库（TON，TOF等）
*
*  作者            : 李琦 朱耿华 王东
*
*  创建日期    : 2016-05-03
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
#include <Srv/Scheduler/cagos.h>
#include "../inc/ExtLib.h"
#include "Srv/Sys/inc/CommSys.h"
#include "Srv/Sys/inc/CommSysConfig.h"
#include "Srv/SharedMem/inc/SharedMem.h"
#include "Srv/SharedMem/inc/SharedSOE.h"
#include "Srv/SharedMem/inc/SharedHandshake.h"
#include "Srv/SharedMem/inc/SharedSafetyComm.h"
#include "Srv/MathLib/inc/Math.h"
#include "Srv/SysResource/inc/Sys.h"
#include "Srv/IPBUS/inc/IPBUS.h"
#include "Srv/SysResource/inc/SysMonitor.h"

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/
static int32_t StringCompare(char_t *pStr1, char_t *pStr2);

static double_t ExtABS(double_t x);
static float_t ExtABS_REAL(float_t x);
static int32_t ExtABS_INT(int32_t x);

static int32_t MUX_INT(uint32_t uiNum, uint32_t uiIndex,\
        int32_t x0, int32_t x1, int32_t x2, int32_t x3,\
        int32_t x4, int32_t x5, int32_t x6, int32_t x7,\
        int32_t x8, int32_t x9, int32_t x10, int32_t x11,\
        int32_t x12, int32_t x13, int32_t x14, int32_t x15);
static uint32_t MUX_UINT(uint32_t uiNum, uint32_t uiIndex,\
        uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,\
        uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,\
        uint32_t x8, uint32_t x9, uint32_t x10, uint32_t x11,\
        uint32_t x12, uint32_t x13, uint32_t x14, uint32_t x15);
static float_t MUX_REAL(uint32_t uiNum, uint32_t uiIndex,\
        float_t x0, float_t x1, float_t x2, float_t x3,\
        float_t x4, float_t x5, float_t x6, float_t x7,\
        float_t x8, float_t x9, float_t x10, float_t x11,\
        float_t x12, float_t x13, float_t x14, float_t x15);
static double_t MUX_LREAL(uint32_t uiNum, uint32_t uiIndex,\
        double_t x0, double_t x1, double_t x2, double_t x3,\
        double_t x4, double_t x5, double_t x6, double_t x7,\
        double_t x8, double_t x9, double_t x10, double_t x11,\
        double_t x12, double_t x13, double_t x14, double_t x15);

static double_t ExtSQRT(double_t x);
static double_t ExtLN(double_t x);
static double_t ExtLOG(double_t x);
static double_t ExtEXP(double_t x);
static double_t ExtSIN(double_t x);
static double_t ExtCOS(double_t x);
static double_t ExtTAN(double_t x);
static double_t ExtCOT(double_t x);
static double_t ExtASIN(double_t x);
static double_t ExtACOS(double_t x);
static double_t ExtATAN(double_t x);
static double_t ExtACOT(double_t x);
static double_t ExtEXPT(double_t x, double_t y);
static double_t ExtLIMIT(Limittype_t *psLimitDat);
static double_t ExtLIMITMAX(LimitMaxtype_t *psLimitMaxDat);
static double_t ExtLIMITMIN(LimitMintype_t *psLimitMinDat);

static float_t ExtSQRT_REAL(float_t x);
static float_t ExtLN_REAL(float_t x);
static float_t ExtLOG_REAL(float_t x);
static float_t ExtEXP_REAL(float_t x);
static float_t ExtSIN_REAL(float_t x);
static float_t ExtCOS_REAL(float_t x);
static float_t ExtTAN_REAL(float_t x);
static float_t ExtCOT_REAL(float_t x);
static float_t ExtASIN_REAL(float_t x);
static float_t ExtACOS_REAL(float_t x);
static float_t ExtATAN_REAL(float_t x);
static float_t ExtACOT_REAL(float_t x);
static float_t ExtEXPT_REAL(float_t x, float_t y);

static void ExtSQRT_LR(MATH_LRtype_t *pstDat);
static void ExtLN_LR(MATH_LRtype_t *pstDat);
static void ExtLOG_LR(MATH_LRtype_t *pstDat);
static void ExtEXP_LR(MATH_LRtype_t *pstDat);
static void ExtASIN_LR(MATH_LRtype_t *pstDat);
static void ExtACOS_LR(MATH_LRtype_t *pstDat);
static void ExtEXPT_LR(EXPT_LRtype_t *pstDat);

static void ExtSQRT_R(MATH_Rtype_t *pstDat);
static void ExtLN_R(MATH_Rtype_t *pstDat);
static void ExtLOG_R(MATH_Rtype_t *pstDat);
static void ExtEXP_R(MATH_Rtype_t *pstDat);
static void ExtASIN_R(MATH_Rtype_t *pstDat);
static void ExtACOS_R(MATH_Rtype_t *pstDat);
static void ExtEXPT_R(EXPT_Rtype_t *pstDat);

static double_t ExtCEIL(double_t x);
static double_t ExtFLOOR(double_t x);
static int32_t ExtNUMBITS(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15,\
                          bool_t b16, bool_t b17, bool_t b18, bool_t b19, bool_t b20, bool_t b21, bool_t b22, bool_t b23, \
                          bool_t b24, bool_t b25, bool_t b26, bool_t b27, bool_t b28, bool_t b29, bool_t b30, bool_t b31, \
                          bool_t b32, bool_t b33, bool_t b34, bool_t b35, bool_t b36, bool_t b37, bool_t b38, bool_t b39, \
                          bool_t b40, bool_t b41, bool_t b42, bool_t b43, bool_t b44, bool_t b45, bool_t b46, bool_t b47, \
                          bool_t b48, bool_t b49, bool_t b50, bool_t b51, bool_t b52, bool_t b53, bool_t b54, bool_t b55, \
                          bool_t b56, bool_t b57, bool_t b58, bool_t b59, bool_t b60, bool_t b61, bool_t b62, bool_t b63);
static int32_t ExtNUMBITS_DWORD(uint32_t x);

static uint32_t ExtPACK16(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15);
static uint32_t ExtPACK32(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15,\
                          bool_t b16, bool_t b17, bool_t b18, bool_t b19, bool_t b20, bool_t b21, bool_t b22, bool_t b23, \
                          bool_t b24, bool_t b25, bool_t b26, bool_t b27, bool_t b28, bool_t b29, bool_t b30, bool_t b31);
static void ExtUNPACK16(UNPACK16type_t *pstDat);
static void ExtUNPACK32(UNPACK32type_t *pstDat);

static uint32_t ExtDT_TO_TOD(uint32_t uiDT);
static uint32_t ExtADD_TOD_TIME(uint32_t uiTOD, uint32_t uiTime);
static uint32_t ExtSUB_TOD_TIME(uint32_t uiTOD, uint32_t uiTime);
static uint32_t ExtADD_DT_TIME(uint32_t uiDTin, uint32_t uiTime);
static uint32_t ExtADD_DATE_TIME(uint32_t uiDATEin, uint32_t uiTime);
static uint32_t ExtSUB_DT_TIME(uint32_t uiDTin, uint32_t uiTime);
static uint32_t ExtSUB_DT_DT(uint32_t uiDTin1, uint32_t uiDTin2);
static uint32_t ExtSUB_DATE_TIME(uint32_t uiDTin, uint32_t uiTime);
static uint32_t ExtSUB_DATE_DATE(uint32_t uiDATEin1, uint32_t uiDATEin2);


static void ExlibRS(RStype_t *pstRSDat);
static void ExlibSR(SRtype_t *pstSRDat);
static void ExlibCTD(CTDtype_t *pstCTDDat);
static void ExlibCTU(CTUtype_t *pstCTUDat);
static void ExlibCTUD(CTUDtype_t *pstCTUDDat);
static void ExlibTimeSet(TimeSettype_t *pstTimeSetDat);
static void ExlibTimeAdj(TimeAdjtype_t *pstTimeAdjDat);
static void ExlibRTC(RTCtype_t *pstRTCDat);
static void ExlibTOF(TOFtype_t *pstDat);
static void ExlibTOFR(TOFRtype_t *pstDat);
static void ExlibTON(TONtype_t *pstDat);
static void ExlibTONR(TONRtype_t *pstDat);
static void ExlibTP(TPtype_t *pstDat);
static void ExlibTPR(TPRtype_t *pstDat);
static void ExlibFTRIG(TRIGtype_t *pstDat);
static void ExlibRTRIG(TRIGtype_t *pstDat);
static void ExlibBLINK(BLINKtype_t *pstDat);
static void ExlibBLINKR(BLINKRtype_t *pstDat);
static void ExlibTOGGLE(TOGGLEtype_t *pstDat);

static void ExlibMBMasterStart(MBMastertype_t *pstMBMasterDat);
static void ExlibMBMasterStop(MBMastertype_t *pstMBMasterDat);
static void ExlibMBState(MBMastertype_t *pstMBMasterDat);
static void ExlibSOEClear(SOEtype_t *pstSOEData);
static void ExlibSOEState(SOEtype_t *pstSOEData);
static void ExlibSOEStop(SOEtype_t *pstSOEData);
static void ExlibSOEStart(SOEtype_t *pstSOEData);
//static void ExlibSysTimeSet(Timetype_t *pstTimeData);

static void ExlibP2PStop(P2Ptype_t *pstP2PData);
static void ExlibP2PStart(P2Ptype_t *pstP2PData);
static void ExlibP2PState(P2Ptype_t *pstP2PData);
static void ExlibP2PSendStatus(P2PSendtype_t *pstP2PData);
static void ExlibP2PRecvStatus(P2PRecvtype_t *pstP2PData);

static double_t ExlibF32toF64(float_t x);
static float_t ExlibF64toF32(double_t x);
static void ExlibCheckErr(uint32_t uiErr1, uint32_t uiErr2);
static void ExlibChkErr(ChkErrtype_t *pstChkErr);
static void ExlibClrErr(ClrErrtype_t *pstClrErr);

static void DisableBit(puint8_t pucAddr, uint32_t uiBitPos, bool_t bValue);
static void DisableByte(puint8_t pucAddr, uint8_t ucValue);
static void DisableWord(puint16_t pusAddr, uint16_t usValue);
static void DisableDWord(puint32_t puiAddr, uint32_t uiValue);
static void DisableFloat(float_t* pfAddr, float_t fValue);
static void DisableDouble(double_t* pdAddr, double_t dValue);
static void SetBitValue(puint8_t pucAddr, uint32_t uiPos, bool_t bValue);

static uint32_t ExtCONCAT_DATE_TOD(uint32_t uiDate, uint32_t uiTod);

static void ExtDIV_DW(DIV_DWtype_t *pstData);
static void ExtDIV_D(DIV_Dtype_t *pstData);
static void ExtDIV_R(DIV_Rtype_t *pstData);
static void ExtDIV_LR(DIV_LRtype_t *pstData);

static void ExtTIMER(TIMERtype_t *pstData);
static void ExtTIMER_R(TIMER_Rtype_t *pstData);
static int32_t ExtGetDelta(int32_t uiTaskNo);
static float_t ExtGetDelta_R(int32_t uiTaskNo);
static void ExlibTRScanStatus(SCANSTATUStype_t *pstData);
static void ExlibTRCalendar(CALENDARtype_t *pstData);

static void ExlibOSPStatus(OSPStatustype_t *pstData);

static int32_t __isleap(int32_t year);
static TMtype_t *os_gmtime(const int32_t *timep, TMtype_t *r);
static int32_t os_mktime(TMtype_t * const t);

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/
static const ExtLibMathTable stMathTable[MAX_EXLIB_FUNC_NUM]={
                                            {"ABS_LREAL", (fp)ExtABS},
                                            {"ABS_REAL", (fp)ExtABS_REAL},
                                            {"ABS_INT", (fp)ExtABS_INT},

                                            {"MUX_INT",(fp)MUX_INT},
                                            {"MUX_UINT",(fp)MUX_UINT},
                                            {"MUX_REAL",(fp)MUX_REAL},
                                            {"MUX_LREAL",(fp)MUX_LREAL},

                                            {"SQRT_LREAL", ExtSQRT},
                                            {"LN_LREAL", ExtLN},
                                            {"LOG_LREAL", ExtLOG},
                                            {"EXP_LREAL", ExtEXP},
                                            {"SIN_LREAL", ExtSIN},
                                            {"COS_LREAL", ExtCOS},
                                            {"TAN_LREAL", ExtTAN},
                                            {"COT_LREAL", ExtCOT},
                                            {"ASIN_LREAL", ExtASIN},
                                            {"ACOS_LREAL", ExtACOS},
                                            {"ATAN_LREAL", ExtATAN},
                                            {"ACOT_LREAL", ExtACOT},
                                            {"EXPT_LREAL", (fp)ExtEXPT},

                                            {"SQRT_REAL", ExtSQRT_REAL},
                                            {"LN_REAL", ExtLN_REAL},
                                            {"LOG_REAL", ExtLOG_REAL},
                                            {"EXP_REAL", ExtEXP_REAL},
                                            {"SIN_REAL", ExtSIN_REAL},
                                            {"COS_REAL", ExtCOS_REAL},
                                            {"TAN_REAL", ExtTAN_REAL},
                                            {"COT_REAL", ExtCOT_REAL},
                                            {"ASIN_REAL", ExtASIN_REAL},
                                            {"ACOS_REAL", ExtACOS_REAL},
                                            {"ATAN_REAL", ExtATAN_REAL},
                                            {"ACOT_REAL", ExtACOT_REAL},
                                            {"EXPT_REAL", (fp)ExtEXPT_REAL},

                                            {"SQRT_LR", (fp)ExtSQRT_LR},
                                            {"LN_LR", (fp)ExtLN_LR},
                                            {"LOG_LR", (fp)ExtLOG_LR},
                                            {"EXP_LR", (fp)ExtEXP_LR},
                                            {"ASIN_LR", (fp)ExtASIN_LR},
                                            {"ACOS_LR", (fp)ExtACOS_LR},
                                            {"EXPT_LR", (fp)ExtEXPT_LR},

                                            {"SQRT_R", (fp)ExtSQRT_R},
                                            {"LN_R", (fp)ExtLN_R},
                                            {"LOG_R", (fp)ExtLOG_R},
                                            {"EXP_R", (fp)ExtEXP_R},
                                            {"ASIN_R", (fp)ExtASIN_R},
                                            {"ACOS_R", (fp)ExtACOS_R},
                                            {"EXPT_R", (fp)ExtEXPT_R},

                                            {"LIMIT", (fp)ExtLIMIT},
                                            {"LIMIT_MAX", (fp)ExtLIMITMAX},
                                            {"LIMIT_MIN", (fp)ExtLIMITMIN},

                                            {"CEIL",(fp)ExtCEIL},
                                            {"FLOOR",(fp)ExtFLOOR},
                                            {"NUMBITS",(fp)ExtNUMBITS},
                                            {"NUMBITS_DWORD",(fp)ExtNUMBITS_DWORD},

                                            {"DT_TO_TOD", (fp)ExtDT_TO_TOD},
                                            {"ADD_TOD_TIME", (fp)ExtADD_TOD_TIME},
                                            {"SUB_TOD_TIME", (fp)ExtSUB_TOD_TIME},

                                            {"ADD_DT_TIME", (fp)ExtADD_DT_TIME},
                                            {"ADD_DATE_TIME", (fp)ExtADD_DATE_TIME},
                                            {"SUB_DT_TIME", (fp)ExtSUB_DT_TIME},
                                            {"SUB_DT_DT", (fp)ExtSUB_DT_DT},
                                            {"SUB_DATE_TIME", (fp)ExtSUB_DATE_TIME},
                                            {"SUB_DATE_DATE", (fp)ExtSUB_DATE_DATE},

                                            {"CONCAT_DATE_TOD", (fp)ExtCONCAT_DATE_TOD},

                                            {"PACK16", (fp)ExtPACK16},
                                            {"PACK32", (fp)ExtPACK32},
                                            {"UNPACK16", (fp)ExtUNPACK16},
                                            {"UNPACK32", (fp)ExtUNPACK32},

                                            {"RS", (fp)ExlibRS},
                                            {"SR", (fp)ExlibSR},
                                            {"CTD", (fp)ExlibCTD},
                                            {"CTU", (fp)ExlibCTU},
                                            {"CTUD", (fp)ExlibCTUD},
                                            {"TIMESET", (fp)ExlibTimeSet},
                                            {"TIMEADJ", (fp)ExlibTimeAdj},
                                            {"RTC", (fp)ExlibRTC},
                                            {"TOF", (fp)ExlibTOF},
                                            {"TOF_I", (fp)ExlibTOF},
                                            {"TOF_R", (fp)ExlibTOFR},
                                            {"TON", (fp)ExlibTON},
                                            {"TON_I", (fp)ExlibTON},
                                            {"TON_R", (fp)ExlibTONR},
                                            {"TP", (fp)ExlibTP},
                                            {"TP_I", (fp)ExlibTP},
                                            {"TP_R", (fp)ExlibTPR},
                                            {"F_TRIG", (fp)ExlibFTRIG},
                                            {"R_TRIG", (fp)ExlibRTRIG},
                                            {"BLINK", (fp)ExlibBLINK},
                                            {"BLINK_I", (fp)ExlibBLINK},
                                            {"BLINK_R", (fp)ExlibBLINKR},
                                            {"TOGGLE",(fp)ExlibTOGGLE},

                                            {"MBSTRT", (fp)ExlibMBMasterStart},
                                            {"MBSTOP", (fp)ExlibMBMasterStop},
                                            {"MBSTAT", (fp)ExlibMBState},

                                            {"SOECLR", (fp)ExlibSOEClear},
                                            {"SOESTAT", (fp)ExlibSOEState},
                                            {"SOESTOP", (fp)ExlibSOEStop},
                                            {"SOESTRT", (fp)ExlibSOEStart},

                                            //{"SNTPTIMESET", (fp)ExlibSysTimeSet},

                                            {"PTPSTOP", (fp)ExlibP2PStop},
                                            {"PTPSTRT", (fp)ExlibP2PStart},
                                            {"PTPSTAT", (fp)ExlibP2PState},
                                            {"PTPSENDSTATUS", (fp)ExlibP2PSendStatus},
                                            {"PTPRECVSTATUS", (fp)ExlibP2PRecvStatus},


                                            {"DIV_DW",(fp)(ExtDIV_DW)},
                                            {"DIV_D",(fp)(ExtDIV_D)},
                                            {"DIV_R", (fp)(ExtDIV_R)},
                                            {"DIV_LR", (fp)(ExtDIV_LR)},

                                            {"TMR", (fp)(ExtTIMER)},
                                            {"TMR_I", (fp)(ExtTIMER)},
                                            {"TMR_R", (fp)(ExtTIMER_R)},

                                            {"REALTOLREAL",(fp)(ExlibF32toF64)},
                                            {"LREALTOREAL",(fp)(ExlibF64toF32)},
                                            {"CHECKERR", (fp)(ExlibCheckErr)},
                                            {"CHK_ERR", (fp)(ExlibChkErr)},
                                            {"CLR_ERR", (fp)(ExlibClrErr)},

                                            {"DisableBit1",(fp)DisableBit},
                                            {"DisableByte1",(fp)DisableByte},
                                            {"DisableByte2",(fp)DisableWord},
                                            {"DisableByte4",(fp)DisableDWord},
                                            {"DisableFloat32",(fp)DisableFloat},
                                            {"DisableFloat64",(fp)DisableDouble},

                                            {"GetDeltaT",(fp)ExtGetDelta},
                                            {"GetDelta_DINT",(fp)ExtGetDelta},
                                            {"GetDelta_REAL",(fp)ExtGetDelta_R},
                                            {"DisableFloat64",(fp)DisableDouble},
                                            {"DisableFloat64",(fp)DisableDouble},

                                            {"TR_SCAN_STATUS",(fp)ExlibTRScanStatus},
                                            {"TR_CALENDAR",(fp)ExlibTRCalendar},

                                            {"SYS_OSP_STATUS",(fp)ExlibOSPStatus},

                                            {"null", NULL}
                                        };

static uint32_t s_uiUP1CheckErr1 = 0;
static uint32_t s_uiUP1CheckErr2 = 0;
static uint32_t s_uiUP2CheckErr1 = 0;
static uint32_t s_uiUP2CheckErr2 = 0;

extern const uint8_t s_ucPMVersion1[];
extern const uint8_t s_ucOSVersion1[];

extern uint32_t uiUP1DeltaTime;
extern uint32_t uiUP2DeltaTime;

extern bool_t bUP1DeltaFlag;
extern bool_t bUP2DeltaFlag;

extern uint32_t uiUP1AverageScanSurplus;
extern uint32_t uiUP1AverageScanOverrun;
extern uint32_t uiUP2AverageScanSurplus;
extern uint32_t uiUP2AverageScanOverrun;

/* days per month -- nonleap! */
const short __spm[13] =
  { 0,
    (31),
    (31+28),
    (31+28+31),
    (31+28+31+30),
    (31+28+31+30+31),
    (31+28+31+30+31+30),
    (31+28+31+30+31+30+31),
    (31+28+31+30+31+30+31+31),
    (31+28+31+30+31+30+31+31+30),
    (31+28+31+30+31+30+31+31+30+31),
    (31+28+31+30+31+30+31+31+30+31+30),
    (31+28+31+30+31+30+31+31+30+31+30+31),
  };

static long int timezone;
static const char days[] = "Sun Mon Tue Wed Thu Fri Sat ";
static const char months[] = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: ExtLibInit
*
* 功能描述: 外部库列表初始化
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
void ExtLibInit(void)
{
    ExLibConfigInfo_t *pstExLib = NULL;
    uint32_t uiFuncNum = 0;
    uint32_t i = 0, j = 0;
    int32_t iRet = -1;
    puint8_t pucGDataAreaAddr = NULL;
    uint32_t uiExFuncAddr = 0;
    uint32_t uiOffset = 0;

    pstExLib = SharedGetExLibInfo();

    if(pstExLib != NULL)
    {
        uiFuncNum = pstExLib->uiExLibFuncNum;

        for(i =0; i < uiFuncNum; i++)
        {
            for(j = 0; j < MAX_EXLIB_FUNC_NUM; j++)
            {
                //iRet = strcmp(&pstExLib->stExLibFuncInfo[i].cFuncName[0], &stMathTable[j].cName[0]);
                iRet = StringCompare(&pstExLib->stExLibFuncInfo[i].cFuncName[0], &stMathTable[j].cName[0]);
                if(0 == iRet)
                {
                    //printf("Func Name=%s\n",(puint8_t)&pstExLib->stExLibFuncInfo[i].cFuncName[0]);
                    uiOffset = pstExLib->stExLibFuncInfo[i].uiRelocOffset;
                    //printf("offset=%x\n", uiOffset);
                    uiExFuncAddr = (uint32_t)stMathTable[j].pFunc;
                    //printf("exFunAddr=0x%x\n", uiExFuncAddr);
                    pucGDataAreaAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
                    //printf("pucGDataAreaBaseAddr=0x%x\n", pucGDataAreaAddr);
                    if(pucGDataAreaAddr != NULL)
                    {
                        pucGDataAreaAddr += uiOffset;
                        //printf("pucGDataAreaAddr = 0x%x\n", pucGDataAreaAddr);
                        *((puint32_t)pucGDataAreaAddr) = uiExFuncAddr;
                        //printf("*((puint32_t)pucGDataAreaAddr)=0x%x\n",*((puint32_t)pucGDataAreaAddr));
                    }
                    break;
                }
            }
        }
    }

    s_uiUP1CheckErr1 = 0;
    s_uiUP1CheckErr2 = 0;
    s_uiUP2CheckErr1 = 0;
    s_uiUP2CheckErr2 = 0;


    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExtAppendDataInit
*
* 功能描述: AppendData区初始化
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
void ExtAppendDataInit(void)
{
    uint32_t i = 0, j = 0;
    puint8_t pucSrc = NULL;
    SOEConfigInfo_t* pstSOEcfg = NULL;
    P2PConfigInfo_t* pstP2Pcfg = NULL;
    ModbusMasterInfo_t* pstModbusMastercfg = NULL;
    SysIoModInfo_t* pstSysIoModInfo = NULL;
    SharedAppend_t *pstAppend = NULL;
    uint8_t ucSlaveNo = 0U;
    uint16_t usModuleType = 0U;
    uint8_t ucBINo = 0U;
    PMController_t emLocalPMId; /* 本PMID号 */
    CMConfigInfo_t* pstCMConfigInfo = NULL;
    uint32_t uiSlaveNum = 0;
    uint32_t uiBlockID = 0;
    
    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    if(pstAppend != NULL)
    {
        emLocalPMId = SysGetLocalPMID();

        /* 清除附加区 */
        pucSrc = SysGetSharedMemAppendInfoStartAddr();
        if(pucSrc != NULL)
        {
            memset(pucSrc,0,sizeof(SharedAppend_t));
        }

        /* 重新置SOE块状态 */
        pstSOEcfg = SharedGetSOEConfigInfo(MAX_TASK_NUM);
        if(pstSOEcfg != NULL)
        {
            for(i = 0; i < MAX_SOE_BLOCK_NUM; i++)
            {
                if(ACTIVE == pstSOEcfg->stSoftSOEBlock[i].emActive)
                {
                    pstAppend->ucSOECtrl[i] = SOE_STATE_START;
                }
            }
        }

        /* 重新置PTP块状态 */
        pstP2Pcfg = SharedGetP2PConfigInfo();
        if(pstP2Pcfg != NULL)
        {
            for(i = 0; i < MAX_P2P_BLOCK_NUM + 1; i++)
            {
                if(ACTIVE == pstP2Pcfg->stP2PSendBlockInfo[i].emActive)
                {
                    uiBlockID = pstP2Pcfg->stP2PSendBlockInfo[i].stP2PBlockHead.uiBlockID;
                    pstAppend->ucP2PCtrl[uiBlockID] = P2P_STATE_START;
                }
            }
        }

        /* 重新置Modbus块状态 */
        pstModbusMastercfg = SharedGetModbusMasterInfo();
        if(pstModbusMastercfg != NULL)
        {
            for(i = 0; i < MAX_MODBUS_BLOCK_NUM; i++)
            {
                if(ACTIVE == pstModbusMastercfg->stModbusMasterBlock[i].emActive)
                {
                    pstAppend->ucModBusCtrl[i] = MODBUS_STATE_START;
                }
            }
        }

        /* 增加重新置ptp及modbu状态后，不需要此处置位 */
        /* 默认启动modbus和ptp */
//        memset((puint8_t)&pstAppend->ucModBusCtrl[0], 0x01, sizeof(uint8_t)* MAX_MODBUS_CTRL_INFO);
//        memset((puint8_t)&pstAppend->ucP2PCtrl[0], 0x01, sizeof(uint8_t)* MAX_P2P_CTRL_INFO);

        /* 重新置PM版本信息 */
        pstAppend->stPMVern.ucActive = 1;
        pstAppend->stPMVern.stPMFirmwareVern.ucSlotIndex = (uint8_t)emLocalPMId;
        pstAppend->stPMVern.stPMFirmwareVern.usModuleType = MODTAG_PM;
        /* PM版本 test */
        pstAppend->stPMVern.stPMFirmwareVern.ucRTSVern[0] = s_ucPMVersion1[0];
        pstAppend->stPMVern.stPMFirmwareVern.ucRTSVern[1] = s_ucPMVersion1[1];
        pstAppend->stPMVern.stPMFirmwareVern.ucRTSVern[2] = s_ucPMVersion1[2];
        pstAppend->stPMVern.stPMFirmwareVern.ucRTSVern[3] = s_ucPMVersion1[3];
        /* OS版本 test */
        pstAppend->stPMVern.stPMFirmwareVern.ucOSVern[0] = s_ucOSVersion1[0];
        pstAppend->stPMVern.stPMFirmwareVern.ucOSVern[1] = s_ucOSVersion1[1];
        pstAppend->stPMVern.stPMFirmwareVern.ucOSVern[2] = s_ucOSVersion1[2];
        pstAppend->stPMVern.stPMFirmwareVern.ucOSVern[3] = s_ucOSVersion1[3];
        /* FPGA版本 */
        pstAppend->stPMVern.stPMFirmwareVern.usFPGAModID = SysGetFPGAModID();
        pstAppend->stPMVern.stPMFirmwareVern.usFPGAHardwareVern = SysGetFPGAHardwareVer();
        pstAppend->stPMVern.stPMFirmwareVern.usFPGAChipID = SysGetFPGAChipID();
        pstAppend->stPMVern.stPMFirmwareVern.usFPGAFWVern = SysGetFPGAFirmwareVer();
        /* MCU版本 */
        pstAppend->stPMVern.stPMFirmwareVern.usMCUModID = SysGetMCUModID();
        pstAppend->stPMVern.stPMFirmwareVern.usMCUHardwareVern = SysGetMCUHardwareVer();
        pstAppend->stPMVern.stPMFirmwareVern.usMCUChipID = SysGetMCUChipID();
        pstAppend->stPMVern.stPMFirmwareVern.usMCUFWVern = SysGetMCUFirmwareVer();

        /* 重新置PM状态信息 */
        pstAppend->stPMState.ucActive = 1;
        pstAppend->stPMState.stPMFirmwareState.ucSlotIndex = (uint8_t)emLocalPMId;
        pstAppend->stPMState.stPMFirmwareState.usModuleType = MODTAG_PM;
        pstAppend->stPMState.stPMFirmwareState.uiMaxDDR = 512 * 1024 * 1024;
        pstAppend->stPMState.stPMFirmwareState.uiFreeDDR = 438 * 1024 * 1024;

        pstCMConfigInfo = SharedGetCMConfigInfo();
        if(pstCMConfigInfo != NULL)
        {
            /* 重新置CM是否配置状态 */
            for(i = 0; i < MAX_CM_NUM; i++)
            {
                if(pstCMConfigInfo->stCMSingleInfo[i].emActive == ACTIVE)
                {
                    pstAppend->stCMState[i].ucActive = 1;
                    pstAppend->stCMState[i].stCMFirmwareState.ucSlotIndex = (uint8_t)pstCMConfigInfo->stCMSingleInfo[i].usSlotID;
                    pstAppend->stCMState[i].stCMFirmwareState.usModuleType = MODTAG_CM;
                    pstAppend->stCMVern[i].ucActive = 1;
                    pstAppend->stCMVern[i].stCMFirmwareVern.ucSlotIndex = (uint8_t)pstCMConfigInfo->stCMSingleInfo[i].usSlotID;
                    pstAppend->stCMVern[i].stCMFirmwareVern.usModuleType = MODTAG_CM;
                }
                else
                {
                    pstAppend->stPMState.stPMFirmwareState.ucCMBUSState[i] = 2;
                }
            }
        }

        /* 重新置Slave是否配置状态,重新置BI是否配置状态 */
        pstSysIoModInfo = SharedGetIoModInfo(MAX_TASK_NUM); /* 获取对应IO表 */
        if(pstSysIoModInfo != NULL)
        {
            for(i = 0; i < MAX_TASK_NUM; i++)
            {
                for(j = 0; j < pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[i]; j++)
                {
                    ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucInDevTabl[i][j];
                    usModuleType = pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                    pstAppend->stSlaveState[ucSlaveNo].ucActive = 1;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucSlotIndex = ucSlaveNo;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.usModuleType = usModuleType;
                    pstAppend->stSlaveVern[ucSlaveNo].ucActive = 1;
                    pstAppend->stSlaveVern[ucSlaveNo].stSlaveFirmwareVern.ucSlotIndex = ucSlaveNo;
                    pstAppend->stSlaveVern[ucSlaveNo].stSlaveFirmwareVern.usModuleType = usModuleType;

                    ucBINo = ucSlaveNo/16;
                    pstAppend->stBIVern[ucBINo].ucActive = 1; /* 每个机笼16个模块，当这个机笼中有一个模块被配置，则BI被配置 */
                    pstAppend->stBIVern[ucBINo].stBIFirmwareVern.ucSlotIndex = ucBINo;
                    pstAppend->stBIVern[ucBINo].stBIFirmwareVern.usModuleType = MODTAG_BI;
                }
                for(j = 0; j < pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[i]; j++)
                {
                    ucSlaveNo = pstSysIoModInfo->stSysIoSlaveInfo.ucOutDevTabl[i][j];
                    usModuleType = pstSysIoModInfo->stSysIoSlaveInfo.stDevInfo[ucSlaveNo].usModTag;
                    pstAppend->stSlaveState[ucSlaveNo].ucActive = 1;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.ucSlotIndex = ucSlaveNo;
                    pstAppend->stSlaveState[ucSlaveNo].stSlaveDiag.usModuleType = usModuleType;
                    pstAppend->stSlaveVern[ucSlaveNo].ucActive = 1;
                    pstAppend->stSlaveVern[ucSlaveNo].stSlaveFirmwareVern.ucSlotIndex = ucSlaveNo;
                    pstAppend->stSlaveVern[ucSlaveNo].stSlaveFirmwareVern.usModuleType = usModuleType;

                    ucBINo = ucSlaveNo/16;
                    pstAppend->stBIVern[ucBINo].ucActive = 1; /* 每个机笼16个模块，当这个机笼中有一个模块被配置，则BI被配置 */
                    pstAppend->stBIVern[ucBINo].stBIFirmwareVern.ucSlotIndex = ucBINo;
                    pstAppend->stBIVern[ucBINo].stBIFirmwareVern.usModuleType = MODTAG_BI;
                }
                uiSlaveNum = pstSysIoModInfo->stSysIoSlaveInfo.usInDevNum[i] + pstSysIoModInfo->stSysIoSlaveInfo.usOutDevNum[i];
                if(i == 0)
                {
                    if((uiSlaveNum % 4) == 0)
                    {
                        pstAppend->stPMState.stPMFirmwareState.stSharedUPState[i].uiUPPollingTime = (uiSlaveNum / 4) * 20000;
                    }
                    else
                    {
                        pstAppend->stPMState.stPMFirmwareState.stSharedUPState[i].uiUPPollingTime = (uiSlaveNum / 4 + 1) * 20000;
                    }
                }
                else
                {
                    if((uiSlaveNum % 2) == 0)
                      {
                          pstAppend->stPMState.stPMFirmwareState.stSharedUPState[i].uiUPPollingTime = (uiSlaveNum / 2) * 20000;
                      }
                      else
                      {
                          pstAppend->stPMState.stPMFirmwareState.stSharedUPState[i].uiUPPollingTime = (uiSlaveNum / 2 + 1) * 20000;
                      }
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibCheckErr
*
* 功能描述: check error
*
* 输入参数: uiErr1, uiErr2
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibCheckErr(uint32_t uiErr1, uint32_t uiErr2)
{
    if(TASK_ID_UP1 == os_get_tno())
    {
        s_uiUP1CheckErr1 = uiErr1; // 0正常       1除零      2 越界
        s_uiUP1CheckErr2 = uiErr2;
    }
    else if(TASK_ID_UP2 == os_get_tno())
    {
        s_uiUP2CheckErr1 = uiErr1; // 0正常       1除零      2 越界
        s_uiUP2CheckErr2 = uiErr2;
    }
    else
    {

    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibChkErr
*
* 功能描述: check error
*
* 输入参数: uiErr1, uiErr2
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibChkErr(ChkErrtype_t *pstChkErrDat)
{
    if(pstChkErrDat != NULL)
    {/* 非空 */
        if(pstChkErrDat->CI)
        {
            if(TASK_ID_UP1 == os_get_tno())
            {
                if(s_uiUP1CheckErr1 != 0)
                {
                    pstChkErrDat->ERR = true;

                    if(1 == s_uiUP1CheckErr1)
                    {
                        pstChkErrDat->DIVBYZERO = true;
                    }
                    else if(2 == s_uiUP1CheckErr1)
                    {
                        pstChkErrDat->ARRAYERR = true;
                    }
                    else
                    {
                    }
                }
                else
                {
                    pstChkErrDat->ERR = false;
                    pstChkErrDat->DIVBYZERO = false;
                    pstChkErrDat->ARRAYERR = false;
                }
            }
            else if(TASK_ID_UP2 == os_get_tno())
            {
                if(s_uiUP2CheckErr1 != 0)
                {
                    pstChkErrDat->ERR = true;

                    if(1 == s_uiUP2CheckErr1)
                    {
                        pstChkErrDat->DIVBYZERO = true;
                    }
                    else if(2 == s_uiUP2CheckErr1)
                    {
                        pstChkErrDat->ARRAYERR = true;
                    }
                    else
                    {
                    }
                }
                else
                {
                    pstChkErrDat->ERR = false;
                    pstChkErrDat->DIVBYZERO = false;
                    pstChkErrDat->ARRAYERR = false;
                }
            }
            else
            {

            }

            pstChkErrDat->CO = true;
        }
        else
        {
            pstChkErrDat->CO = false;
            pstChkErrDat->ERR = false;
            pstChkErrDat->DIVBYZERO = false;
            pstChkErrDat->ARRAYERR = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibChkErr
*
* 功能描述: check error
*
* 输入参数: uiErr1, uiErr2
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibClrErr(ClrErrtype_t *pstClrErr)
{
    if(pstClrErr != NULL)
    {/* 非空 */
        if(pstClrErr->CI)
        {
            if(TASK_ID_UP1 == os_get_tno())
            {
                s_uiUP1CheckErr1 = 0;
                s_uiUP1CheckErr2 = 0;
            }
            else if(TASK_ID_UP2 == os_get_tno())
            {
                s_uiUP2CheckErr1 = 0;
                s_uiUP2CheckErr2 = 0;
            }
            else
            {
            }
            pstClrErr->CO = true;
        }
        else
        {
            pstClrErr->CO = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExtABS
*
* 功能描述: 绝对值运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : |x|
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtABS(double_t x)
{
    double_t dRet = (double_t)0.0;

    if(x >= 0)
    {
        dRet = x;
    }
    else
    {
        dRet = -x;
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtABS_REAL
*
* 功能描述: 绝对值运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : |x|
*
* 注意事项: 无
***********************************************************************************************************************
*/
static float_t ExtABS_REAL(float_t x)
{
    float_t dRet = (float_t)0.0;

    if(x >= 0)
    {
        dRet = x;
    }
    else
    {
        dRet = -x;
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtABS_INT
*
* 功能描述: 绝对值运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : |x|
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t ExtABS_INT(int32_t x)
{
    int32_t dRet = (int32_t)0;

    if(x >= 0)
    {
        dRet = x;
    }
    else
    {
        dRet = -x;
    }
    return dRet;
}
/*
***********************************************************************************************************************
* 函数名称: MUX_INT
*
* 功能描述: 多路选一
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 选取的某路值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t MUX_INT(uint32_t uiNum, uint32_t uiIndex,\
        int32_t x0, int32_t x1, int32_t x2, int32_t x3,\
        int32_t x4, int32_t x5, int32_t x6, int32_t x7,\
        int32_t x8, int32_t x9, int32_t x10, int32_t x11,\
        int32_t x12, int32_t x13, int32_t x14, int32_t x15)
{
    int32_t x = 0;
    uint32_t i = 0;

    if(uiNum <= 16)
    {
        if(uiIndex > (uiNum-1))
        {
            i = uiNum - 1;
        }
        else
        {
            i = uiIndex;
        }

        if(0 == i)
        {
            x = x0;
        }
        else if(1 == i)
        {
            x = x1;
        }
        else if(2 == i)
        {
            x = x2;
        }
        else if(3 == i)
        {
            x = x3;
        }
        else if(4 == i)
        {
            x = x4;
        }
        else if(5 == i)
        {
            x = x5;
        }
        else if(6 == i)
        {
            x = x6;
        }
        else if(7 == i)
        {
            x = x7;
        }
        else if(8 == i)
        {
            x = x8;
        }
        else if(9 == i)
        {
            x = x9;
        }
        else if(10 == i)
        {
            x = x10;
        }
        else if(11 == i)
        {
            x = x11;
        }
        else if(12 == i)
        {
            x = x12;
        }
        else if(13 == i)
        {
            x = x13;
        }
        else if(14 == i)
        {
            x = x14;
        }
        else
        {
            x = x15;
        }
    }

    return x;
}
/*
***********************************************************************************************************************
* 函数名称: MUX_UINT
*
* 功能描述: 多路选一
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 选取的某路值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t MUX_UINT(uint32_t uiNum, uint32_t uiIndex,\
        uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3,\
        uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7,\
        uint32_t x8, uint32_t x9, uint32_t x10, uint32_t x11,\
        uint32_t x12, uint32_t x13, uint32_t x14, uint32_t x15)
{
    uint32_t x = 0;
    uint32_t i = 0;

    if(uiNum <= 16)
    {
        if(uiIndex > (uiNum-1))
        {
            i = uiNum - 1;
        }
        else
        {
            i = uiIndex;
        }

        if(0 == i)
        {
            x = x0;
        }
        else if(1 == i)
        {
            x = x1;
        }
        else if(2 == i)
        {
            x = x2;
        }
        else if(3 == i)
        {
            x = x3;
        }
        else if(4 == i)
        {
            x = x4;
        }
        else if(5 == i)
        {
            x = x5;
        }
        else if(6 == i)
        {
            x = x6;
        }
        else if(7 == i)
        {
            x = x7;
        }
        else if(8 == i)
        {
            x = x8;
        }
        else if(9 == i)
        {
            x = x9;
        }
        else if(10 == i)
        {
            x = x10;
        }
        else if(11 == i)
        {
            x = x11;
        }
        else if(12 == i)
        {
            x = x12;
        }
        else if(13 == i)
        {
            x = x13;
        }
        else if(14 == i)
        {
            x = x14;
        }
        else
        {
            x = x15;
        }
    }
    return x;
}
/*
***********************************************************************************************************************
* 函数名称: MUX_REAL
*
* 功能描述: 多路选一
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 选取的某路值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static float_t MUX_REAL(uint32_t uiNum, uint32_t uiIndex,\
        float_t x0, float_t x1, float_t x2, float_t x3,\
        float_t x4, float_t x5, float_t x6, float_t x7,\
        float_t x8, float_t x9, float_t x10, float_t x11,\
        float_t x12, float_t x13, float_t x14, float_t x15)
{
    float_t x = 0;
    uint32_t i = 0;

    if(uiNum <= 16)
    {
        if(uiIndex > (uiNum-1))
        {
            i = uiNum - 1;
        }
        else
        {
            i = uiIndex;
        }

        if(0 == i)
        {
            x = x0;
        }
        else if(1 == i)
        {
            x = x1;
        }
        else if(2 == i)
        {
            x = x2;
        }
        else if(3 == i)
        {
            x = x3;
        }
        else if(4 == i)
        {
            x = x4;
        }
        else if(5 == i)
        {
            x = x5;
        }
        else if(6 == i)
        {
            x = x6;
        }
        else if(7 == i)
        {
            x = x7;
        }
        else if(8 == i)
        {
            x = x8;
        }
        else if(9 == i)
        {
            x = x9;
        }
        else if(10 == i)
        {
            x = x10;
        }
        else if(11 == i)
        {
            x = x11;
        }
        else if(12 == i)
        {
            x = x12;
        }
        else if(13 == i)
        {
            x = x13;
        }
        else if(14 == i)
        {
            x = x14;
        }
        else
        {
            x = x15;
        }
    }
    return x;
}
/*
***********************************************************************************************************************
* 函数名称: MUX_LREAL
*
* 功能描述: 多路选一
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 选取的某路值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t MUX_LREAL(uint32_t uiNum, uint32_t uiIndex,\
        double_t x0, double_t x1, double_t x2, double_t x3,\
        double_t x4, double_t x5, double_t x6, double_t x7,\
        double_t x8, double_t x9, double_t x10, double_t x11,\
        double_t x12, double_t x13, double_t x14, double_t x15)
{
    double_t x = 0;
    uint32_t i = 0;

    if(uiNum <= 16)
    {
        if(uiIndex > (uiNum-1))
        {
            i = uiNum - 1;
        }
        else
        {
            i = uiIndex;
        }

        if(0 == i)
        {
            x = x0;
        }
        else if(1 == i)
        {
            x = x1;
        }
        else if(2 == i)
        {
            x = x2;
        }
        else if(3 == i)
        {
            x = x3;
        }
        else if(4 == i)
        {
            x = x4;
        }
        else if(5 == i)
        {
            x = x5;
        }
        else if(6 == i)
        {
            x = x6;
        }
        else if(7 == i)
        {
            x = x7;
        }
        else if(8 == i)
        {
            x = x8;
        }
        else if(9 == i)
        {
            x = x9;
        }
        else if(10 == i)
        {
            x = x10;
        }
        else if(11 == i)
        {
            x = x11;
        }
        else if(12 == i)
        {
            x = x12;
        }
        else if(13 == i)
        {
            x = x13;
        }
        else if(14 == i)
        {
            x = x14;
        }
        else
        {
            x = x15;
        }
    }
    return x;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSQRT
*
* 功能描述: 开平方运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : 平方根
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtSQRT(double_t x)
{
    double_t dRet = (double_t)0.0;

    if(x >= 0)
    {
        #ifdef MATH_LIB
        dRet = sqrt(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtLN
*
* 功能描述: 常对数运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : 常对数
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtLN(double_t x)
{
    double_t dRet = (double_t)0.0;

    if(x > 0)
    {
        #ifdef MATH_LIB
        dRet = log(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtLOG
*
* 功能描述: 自然对数运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : 自然对数
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtLOG(double_t x)
{
    double_t dRet = (double_t)0.0;

    if(x > 0)
    {
        #ifdef MATH_LIB
        dRet = log10(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtEXP
*
* 功能描述: 指数运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : e^x
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtEXP(double_t x)
{
    double_t dRet = (double_t)0.0;

    #ifdef MATH_LIB
    dRet = exp(x);
    #endif
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSIN
*
* 功能描述: 正弦运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : sin(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtSIN(double_t x)
{
    double_t dRet = (double_t)0.0;
    
    dRet = sin(x);

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtCOS
*
* 功能描述: 余弦运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : cos(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtCOS(double_t x)
{
    double_t dRet = (double_t)0.0;

    dRet = cos(x);

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtTAN
*
* 功能描述: 正切运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : tan(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtTAN(double_t x)
{
    double_t dRet = (double_t)0.0;
    #ifdef MATH_LIB
    dRet = tan(x);
    #endif
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtCOT
*
* 功能描述: 余切运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : cot(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtCOT(double_t x)
{
    double_t dRet = (double_t)0.0;
    double_t dTmp = 0.0;
    
    dTmp = tan(x);
    if(dTmp != 0)
    {
        #ifdef MATH_LIB
        dRet = 1.0 / tan(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtASIN
*
* 功能描述: 反正弦运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : asin(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtASIN(double_t x)
{
    double_t dRet = (double_t)0.0;

    if((x >= -1 )&&(x <= 1))
    {
        #ifdef MATH_LIB
        dRet = asin(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtACOS
*
* 功能描述: 反余弦运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : acos(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtACOS(double_t x)
{
    double_t dRet = (double_t)0.0;

    if((x >= -1 )&&(x <= 1))
    {
        #ifdef MATH_LIB
        dRet = acos(x);
        #endif
    }
    else
    {
        dRet = 1.7e308;//无效值
    }
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtATAN
*
* 功能描述: 反正切运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : atan(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtATAN(double_t x)
{
    double_t dRet = (double_t)0.0;
    #ifdef MATH_LIB
    dRet = atan(x);
    #endif
    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtACOT
*
* 功能描述: 反余切运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : acot(x)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtACOT(double_t x)
{
    double_t dRet = (double_t)0.0;

    //dRet = acot(x);

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtEXPT
*
* 功能描述: 幂运算
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : expt(x,y)
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtEXPT(double_t x, double_t y)
{
    double_t dRet = (double_t)0.0;

    if((x < 0 ) && (((y > -1) && (y < 0)) || ((y > 0) && (y < 1))))
    {
        dRet = 1.7e308;//无效值
    }
    else if((x == 0) && (y <= 0))
    {
        dRet = 1.7e308;//无效值
    }
    else
    {
        #ifdef MATH_LIB
        dRet = pow(x, y);
        #endif
    }

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: SQRT etc.
*
* 功能描述: 除法运算等
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : 运算结果
*
* 注意事项: 无
***********************************************************************************************************************
*/
static float_t ExtSQRT_REAL(float_t x) { return (float_t)ExtSQRT(x); }
static float_t ExtLN_REAL(float_t x) { return (float_t)ExtLN(x); }
static float_t ExtLOG_REAL(float_t x) { return (float_t)ExtLOG(x); }
static float_t ExtEXP_REAL(float_t x) { return (float_t)ExtEXP(x); }
static float_t ExtSIN_REAL(float_t x) { return (float_t)ExtSIN(x); }
static float_t ExtCOS_REAL(float_t x) { return (float_t)ExtCOS(x); }
static float_t ExtTAN_REAL(float_t x) { return (float_t)ExtTAN(x); }
static float_t ExtCOT_REAL(float_t x) { return (float_t)ExtCOT(x); }
static float_t ExtASIN_REAL(float_t x) { return (float_t)ExtASIN(x); }
static float_t ExtACOS_REAL(float_t x) { return (float_t)ExtACOS(x); }
static float_t ExtATAN_REAL(float_t x) { return (float_t)ExtATAN(x); }
static float_t ExtACOT_REAL(float_t x) { return (float_t)ExtACOT(x); }
static float_t ExtEXPT_REAL(float_t x, float_t y) { return (float_t)ExtEXPT(x, y); }

/*
***********************************************************************************************************************
* 函数名称: ExtSQRT_LR
*
* 功能描述: 开平方运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtSQRT_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN >= 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = sqrt(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLN_LR
*
* 功能描述: 常对数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtLN_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN > 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = log(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLOG_LR
*
* 功能描述: 自然对数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtLOG_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN > 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = log10(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtEXP_LR
*
* 功能描述: 指数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtEXP_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN <= 709)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = exp(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtASIN_LR
*
* 功能描述: 反正弦运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtASIN_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->dIN >= -1) && (pstDat->dIN <= 1))
        {
            #ifdef MATH_LIB
            pstDat->dOUT = asin(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtACOS_LR
*
* 功能描述: 反余弦运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtACOS_LR(MATH_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->dIN >= -1) && (pstDat->dIN <= 1))
        {
            #ifdef MATH_LIB
            pstDat->dOUT = acos(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtEXPT_LR
*
* 功能描述: 幂运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtEXPT_LR(EXPT_LRtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->Y < 0.0001) && (pstDat->Y >= 0)) //Y趋近于0
        {
            if((pstDat->n < 0.0001) && (pstDat->n > 0))//n趋近于0
            {
                pstDat->dOUT = 1;
                pstDat->ucERR = 0;
            }
            else if(pstDat->n <= 0)
            {
                pstDat->ucERR = 2;
            }
            else
            {
                pstDat->dOUT = 0;
                pstDat->ucERR = 0;
            }
        }
        else if(pstDat->Y > 0)
        {
            if(pstDat->n == 0.0)
            {
                pstDat->dOUT = 1;
                pstDat->ucERR = 0;
            }
            else
            {
                #ifdef MATH_LIB
                if(ExtLN(ExtABS(ExtLN(pstDat->Y))) + ExtLN(ExtABS(pstDat->n)) <= 6.562)
                {
                    pstDat->dOUT = pow(pstDat->Y, pstDat->n);
                    pstDat->ucERR = 0;
                }
                else
                {
                    pstDat->ucERR = 3;
                }
                #endif
            }
        }
        else
        {
            pstDat->ucERR = 1;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtSQRT_R
*
* 功能描述: 开平方运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtSQRT_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN >= 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = sqrt(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLN_R
*
* 功能描述: 常对数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtLN_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN > 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = log(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLOG_R
*
* 功能描述: 自然对数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtLOG_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN > 0)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = log10(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtEXP_R
*
* 功能描述: 指数运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtEXP_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->dIN <= 88)
        {
            #ifdef MATH_LIB
            pstDat->dOUT = exp(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtASIN_R
*
* 功能描述: 反正弦运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtASIN_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->dIN >= -1) && (pstDat->dIN <= 1))
        {
            #ifdef MATH_LIB
            pstDat->dOUT = asin(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtACOS_R
*
* 功能描述: 反余弦运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtACOS_R(MATH_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->dIN >= -1) && (pstDat->dIN <= 1))
        {
            #ifdef MATH_LIB
            pstDat->dOUT = acos(pstDat->dIN);
            #endif

            pstDat->bERR = false;
        }
        else
        {
            pstDat->bERR = true;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtEXPT_R
*
* 功能描述: 幂运算
*
* 输入参数: pstDat
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtEXPT_R(EXPT_Rtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if((pstDat->Y < 0.0001) && (pstDat->Y >= 0)) //Y趋近于0
        {
            if((pstDat->n < 0.0001) && (pstDat->n > 0))//n趋近于0
            {
                pstDat->dOUT = 1;
                pstDat->ucERR = 0;
            }
            else if(pstDat->n <= 0)
            {
                pstDat->ucERR = 2;
            }
            else
            {
                pstDat->dOUT = 0;
                pstDat->ucERR = 0;
            }
        }
        else if(pstDat->Y > 0)
        {
            #ifdef MATH_LIB
            if(pstDat->n == 0.0)
            {
                pstDat->dOUT = 1;
                pstDat->ucERR = 0;
            }
            else
            {
                if(ExtLN(ExtABS(ExtLN((double_t)pstDat->Y))) + (ExtLN(ExtABS((double_t)pstDat->n))) <= 4.466)
                {
                    pstDat->dOUT = pow(pstDat->Y, pstDat->n);
                    pstDat->ucERR = 0;
                }
                else
                {
                    pstDat->ucERR = 3;
                }
            }
            #endif
        }
        else
        {
            pstDat->ucERR = 1;
        }
    }
    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibF32toF64
*
* 功能描述: real to long real
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : long real
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExlibF32toF64(float_t x)
{
    double_t dRet = (double_t)0.0;

    dRet = (double_t)x;

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibF64toF32
*
* 功能描述: long real to real
*
* 输入参数: x
*
* 输出参数: 无
*
* 返 回 值  : real
*
* 注意事项: 无
***********************************************************************************************************************
*/
static float_t ExlibF64toF32(double_t x)
{
    float_t dRet = (float_t)0.0;

    dRet = (float_t)x;

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtLIMIT
*
* 功能描述: 如果输入数值处于上限或下限之间，则返回当前的输入值。如果输入值小于等于下限值，则返回下限值。如果输入值大于等于上限值，则返回上限值。
*
* 输入参数: psLimitDat
*
* 输出参数: 无
*
* 返 回 值  : dOut
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtLIMIT(Limittype_t *psLimitDat)
{
    double dOut = 0.0;

    if(psLimitDat != NULL)
    {
        if(psLimitDat->dIn > psLimitDat->dMax)
        {
            psLimitDat->dOut = psLimitDat->dMax;
        }
        else if (psLimitDat->dIn < psLimitDat->dMin)
        {
            psLimitDat->dOut = psLimitDat->dMin;
        }
        else
        {
            psLimitDat->dOut = psLimitDat->dIn;
        }
        dOut = psLimitDat->dOut;
    }
    return dOut;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLIMITMAX
*
* 功能描述: 如果输入数值未超过上限，则返回当前的输入值。如果输入值大于等于上限值，则返回上限值。
*
* 输入参数: psLimitMaxDat
*
* 输出参数: 无
*
* 返 回 值  : dOut
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtLIMITMAX(LimitMaxtype_t *psLimitMaxDat)
{
    double dOut = 0.0;

    if(psLimitMaxDat != NULL)
    {
        if(psLimitMaxDat->dIn > psLimitMaxDat->dMax)
        {
            psLimitMaxDat->dOut = psLimitMaxDat->dMax;
        }
        else
        {
            psLimitMaxDat->dOut = psLimitMaxDat->dIn;
        }
        dOut = psLimitMaxDat->dOut;
    }
    return dOut;
}
/*
***********************************************************************************************************************
* 函数名称: ExtLIMITMIN
*
* 功能描述: 如果输入数值未超过下限，则返回当前的输入值。如果输入值小于等于下限值，则返回下限值。
*
* 输入参数: psLimitMinDat
*
* 输出参数: 无
*
* 返 回 值  : dOut
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtLIMITMIN(LimitMintype_t *psLimitMinDat)
{
    double dOut = 0.0;

    if(psLimitMinDat != NULL)
    {
        if (psLimitMinDat->dIn < psLimitMinDat->dMin)
        {
            psLimitMinDat->dOut = psLimitMinDat->dMin;
        }
        else
        {
            psLimitMinDat->dOut = psLimitMinDat->dIn;
        }
        dOut = psLimitMinDat->dOut;
    }
    return dOut;
}
/*
***********************************************************************************************************************
* 函数名称: ExtCEIL
*
* 功能描述: 向上取整算法。
*
* 输入参数: 双精度数x
*
* 输出参数: 无
*
* 返 回 值  : 向上取整后的值
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtCEIL(double_t x)
{
    double_t dRet = 0.0;

    if(x > 0)
    {
        dRet = (double_t)((int32_t)(x + 0.5));
    }
    else
    {
        dRet = (double_t)((int32_t)(x - 0.5));
    }

    return dRet;
}
/*
***********************************************************************************************************************
* 函数名称: ExtFLOOR
*
* 功能描述: 截取整数部分算法
*
* 输入参数: 双精度数x
*
* 输出参数: 无
*
* 返 回 值  : 整数部分
*
* 注意事项: 无
***********************************************************************************************************************
*/
static double_t ExtFLOOR(double_t x)
{
    double_t dRet = 0.0;

    dRet = (double_t)((int32_t)x);

    return dRet;
}

/*
***********************************************************************************************************************
* 函数名称: ExtNUMBITS
*
* 功能描述: 计算输入端为TRUE值的个数算法。
*
* 输入参数: b0~b63
*
* 输出参数: 无
*
* 返 回 值  : true的个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t ExtNUMBITS(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15,\
                          bool_t b16, bool_t b17, bool_t b18, bool_t b19, bool_t b20, bool_t b21, bool_t b22, bool_t b23, \
                          bool_t b24, bool_t b25, bool_t b26, bool_t b27, bool_t b28, bool_t b29, bool_t b30, bool_t b31, \
                          bool_t b32, bool_t b33, bool_t b34, bool_t b35, bool_t b36, bool_t b37, bool_t b38, bool_t b39, \
                          bool_t b40, bool_t b41, bool_t b42, bool_t b43, bool_t b44, bool_t b45, bool_t b46, bool_t b47, \
                          bool_t b48, bool_t b49, bool_t b50, bool_t b51, bool_t b52, bool_t b53, bool_t b54, bool_t b55, \
                          bool_t b56, bool_t b57, bool_t b58, bool_t b59, bool_t b60, bool_t b61, bool_t b62, bool_t b63)
{
    int32_t iNum = 0;

    if(true == b0)  { iNum ++; }
    if(true == b1)  { iNum ++; }
    if(true == b2)  { iNum ++; }
    if(true == b3)  { iNum ++; }
    if(true == b4)  { iNum ++; }
    if(true == b5)  { iNum ++; }
    if(true == b6)  { iNum ++; }
    if(true == b7)  { iNum ++; }
    if(true == b8)  { iNum ++; }
    if(true == b9)  { iNum ++; }
    if(true == b10) { iNum ++; }
    if(true == b11) { iNum ++; }
    if(true == b12) { iNum ++; }
    if(true == b13) { iNum ++; }
    if(true == b14) { iNum ++; }
    if(true == b15) { iNum ++; }
    if(true == b16) { iNum ++; }
    if(true == b17) { iNum ++; }
    if(true == b18) { iNum ++; }
    if(true == b19) { iNum ++; }
    if(true == b20) { iNum ++; }
    if(true == b21) { iNum ++; }
    if(true == b22) { iNum ++; }
    if(true == b23) { iNum ++; }
    if(true == b24) { iNum ++; }
    if(true == b25) { iNum ++; }
    if(true == b26) { iNum ++; }
    if(true == b27) { iNum ++; }
    if(true == b28) { iNum ++; }
    if(true == b29) { iNum ++; }
    if(true == b30) { iNum ++; }
    if(true == b31) { iNum ++; }
    if(true == b32) { iNum ++; }
    if(true == b33) { iNum ++; }
    if(true == b34) { iNum ++; }
    if(true == b35) { iNum ++; }
    if(true == b36) { iNum ++; }
    if(true == b37) { iNum ++; }
    if(true == b38) { iNum ++; }
    if(true == b39) { iNum ++; }
    if(true == b40) { iNum ++; }
    if(true == b41) { iNum ++; }
    if(true == b42) { iNum ++; }
    if(true == b43) { iNum ++; }
    if(true == b44) { iNum ++; }
    if(true == b45) { iNum ++; }
    if(true == b46) { iNum ++; }
    if(true == b47) { iNum ++; }
    if(true == b48) { iNum ++; }
    if(true == b49) { iNum ++; }
    if(true == b50) { iNum ++; }
    if(true == b51) { iNum ++; }
    if(true == b52) { iNum ++; }
    if(true == b53) { iNum ++; }
    if(true == b54) { iNum ++; }
    if(true == b55) { iNum ++; }
    if(true == b56) { iNum ++; }
    if(true == b57) { iNum ++; }
    if(true == b58) { iNum ++; }
    if(true == b59) { iNum ++; }
    if(true == b60) { iNum ++; }
    if(true == b61) { iNum ++; }
    if(true == b62) { iNum ++; }
    if(true == b63) { iNum ++; }
    return iNum;
}
/*
***********************************************************************************************************************
* 函数名称: ExtNUMBITS_DWORD
*
* 功能描述: 计算DWORD中为1的位的个数。
*
* 输入参数: x 32位无符号整数
*
* 输出参数: 无
*
* 返 回 值  : 为1的位的个数
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t ExtNUMBITS_DWORD(uint32_t x)
{
    int32_t i = 0;
    int32_t n = 0;
    uint32_t uiTmp = 0;

    uiTmp = x;

    for(i = 0; i < 31; i++)
    {
        if(1 == (uiTmp & 0x01U))
        {
            n++;
        }
        uiTmp = uiTmp >> 1;
    }

    return n;
}
/*
***********************************************************************************************************************
* 函数名称: ExtPACK16
*
* 功能描述: 将16个布尔型输入变量组合成一个DWORD型变量后输出。
*
* 输入参数: b0~b15
*
* 输出参数: 无
*
* 返 回 值  : 合成一个DWORD型变量后输出
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtPACK16(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15)
{
    uint32_t uiResult = 0;

    if(true == b0)  { uiResult |= ((uint32_t)b0); }
    if(true == b1)  { uiResult |= ((uint32_t)b1)<<1; }
    if(true == b2)  { uiResult |= ((uint32_t)b2)<<2; }
    if(true == b3)  { uiResult |= ((uint32_t)b3)<<3; }
    if(true == b4)  { uiResult |= ((uint32_t)b4)<<4; }
    if(true == b5)  { uiResult |= ((uint32_t)b5)<<5; }
    if(true == b6)  { uiResult |= ((uint32_t)b6)<<6; }
    if(true == b7)  { uiResult |= ((uint32_t)b7)<<7; }
    if(true == b8)  { uiResult |= ((uint32_t)b8)<<8; }
    if(true == b9)  { uiResult |= ((uint32_t)b9)<<9; }
    if(true == b10) { uiResult |= ((uint32_t)b10)<<10; }
    if(true == b11) { uiResult |= ((uint32_t)b11)<<11; }
    if(true == b12) { uiResult |= ((uint32_t)b12)<<12; }
    if(true == b13) { uiResult |= ((uint32_t)b13)<<13; }
    if(true == b14) { uiResult |= ((uint32_t)b14)<<14; }
    if(true == b15) { uiResult |= ((uint32_t)b15)<<15; }

    return uiResult;
}
/*
***********************************************************************************************************************
* 函数名称: ExtPACK32
*
* 功能描述: 将32个布尔型输入变量组合成一个DWORD型变量后输出。
*
* 输入参数: b0~b31
*
* 输出参数: 无
*
* 返 回 值  : 合成一个DWORD型变量后输出
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtPACK32(bool_t b0,  bool_t b1,  bool_t b2,  bool_t b3,  bool_t b4,  bool_t b5,  bool_t b6,  bool_t b7, \
                          bool_t b8,  bool_t b9,  bool_t b10, bool_t b11, bool_t b12, bool_t b13, bool_t b14, bool_t b15,\
                          bool_t b16, bool_t b17, bool_t b18, bool_t b19, bool_t b20, bool_t b21, bool_t b22, bool_t b23, \
                          bool_t b24, bool_t b25, bool_t b26, bool_t b27, bool_t b28, bool_t b29, bool_t b30, bool_t b31)
{
    uint32_t uiResult = 0;

    if(true == b0)  { uiResult |= ((uint32_t)b0); }
    if(true == b1)  { uiResult |= ((uint32_t)b1)<<1; }
    if(true == b2)  { uiResult |= ((uint32_t)b2)<<2; }
    if(true == b3)  { uiResult |= ((uint32_t)b3)<<3; }
    if(true == b4)  { uiResult |= ((uint32_t)b4)<<4; }
    if(true == b5)  { uiResult |= ((uint32_t)b5)<<5; }
    if(true == b6)  { uiResult |= ((uint32_t)b6)<<6; }
    if(true == b7)  { uiResult |= ((uint32_t)b7)<<7; }
    if(true == b8)  { uiResult |= ((uint32_t)b8)<<8; }
    if(true == b9)  { uiResult |= ((uint32_t)b9)<<9; }
    if(true == b10) { uiResult |= ((uint32_t)b10)<<10; }
    if(true == b11) { uiResult |= ((uint32_t)b11)<<11; }
    if(true == b12) { uiResult |= ((uint32_t)b12)<<12; }
    if(true == b13) { uiResult |= ((uint32_t)b13)<<13; }
    if(true == b14) { uiResult |= ((uint32_t)b14)<<14; }
    if(true == b15) { uiResult |= ((uint32_t)b15)<<15; }
    if(true == b16) { uiResult |= ((uint32_t)b16)<<16; }
    if(true == b17) { uiResult |= ((uint32_t)b17)<<17; }
    if(true == b18) { uiResult |= ((uint32_t)b18)<<18; }
    if(true == b19) { uiResult |= ((uint32_t)b19)<<19; }
    if(true == b20) { uiResult |= ((uint32_t)b20)<<20; }
    if(true == b21) { uiResult |= ((uint32_t)b21)<<21; }
    if(true == b22) { uiResult |= ((uint32_t)b22)<<22; }
    if(true == b23) { uiResult |= ((uint32_t)b23)<<23; }
    if(true == b24) { uiResult |= ((uint32_t)b24)<<24; }
    if(true == b25) { uiResult |= ((uint32_t)b25)<<25; }
    if(true == b26) { uiResult |= ((uint32_t)b26)<<26; }
    if(true == b27) { uiResult |= ((uint32_t)b27)<<27; }
    if(true == b28) { uiResult |= ((uint32_t)b28)<<28; }
    if(true == b29) { uiResult |= ((uint32_t)b29)<<29; }
    if(true == b30) { uiResult |= ((uint32_t)b30)<<30; }
    if(true == b31) { uiResult |= ((uint32_t)b31)<<31; }

    return uiResult;
}
/*
***********************************************************************************************************************
* 函数名称: ExtUNPACK16
*
* 功能描述: 将DWORD型变量的低16位分别输出到16个布尔型变量。
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtUNPACK16(UNPACK16type_t *pstDat)
{
    uint32_t uiTmp = 0;

    if(pstDat != NULL)
    {
        if(true == pstDat->bCI)
        {
            if(pstDat->uiInput != 0)
            {
                pstDat->bNZ = true;
            }
            else
            {
                pstDat->bNZ = false;
            }

            pstDat->bCO = true;

            uiTmp = pstDat->uiInput;
            pstDat->Q01 = (bool_t)(uiTmp & 0x01);
            pstDat->Q02 = (bool_t)((uiTmp>>1) & 0x01);
            pstDat->Q03 = (bool_t)((uiTmp>>2) & 0x01);
            pstDat->Q04 = (bool_t)((uiTmp>>3) & 0x01);
            pstDat->Q05 = (bool_t)((uiTmp>>4) & 0x01);
            pstDat->Q06 = (bool_t)((uiTmp>>5) & 0x01);
            pstDat->Q07 = (bool_t)((uiTmp>>6) & 0x01);
            pstDat->Q08 = (bool_t)((uiTmp>>7) & 0x01);
            pstDat->Q09 = (bool_t)((uiTmp>>8) & 0x01);
            pstDat->Q10 = (bool_t)((uiTmp>>9) & 0x01);
            pstDat->Q11 = (bool_t)((uiTmp>>10) & 0x01);
            pstDat->Q12 = (bool_t)((uiTmp>>11) & 0x01);
            pstDat->Q13 = (bool_t)((uiTmp>>12) & 0x01);
            pstDat->Q14 = (bool_t)((uiTmp>>13) & 0x01);
            pstDat->Q15 = (bool_t)((uiTmp>>14) & 0x01);
            pstDat->Q16 = (bool_t)((uiTmp>>15) & 0x01);
        }
        else
        {
            pstDat->bNZ = false;
            pstDat->bCO = false;
            pstDat->Q01 = false;
            pstDat->Q02 = false;
            pstDat->Q03 = false;
            pstDat->Q04 = false;
            pstDat->Q05 = false;
            pstDat->Q06 = false;
            pstDat->Q07 = false;
            pstDat->Q08 = false;
            pstDat->Q09 = false;
            pstDat->Q10 = false;
            pstDat->Q11 = false;
            pstDat->Q12 = false;
            pstDat->Q13 = false;
            pstDat->Q14 = false;
            pstDat->Q15 = false;
            pstDat->Q16 = false;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtUNPACK32
*
* 功能描述: 将DWORD型变量的32位分别输出到32个布尔型变量。
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtUNPACK32(UNPACK32type_t *pstDat)
{
    uint32_t uiTmp = 0;

    if(pstDat != NULL)
    {
        if(true == pstDat->bCI)
        {
            if(pstDat->uiInput != 0)
            {
                pstDat->bNZ = true;
            }
            else
            {
                pstDat->bNZ = false;
            }

            pstDat->bCO = true;

            uiTmp = pstDat->uiInput;
            pstDat->Q01 = (bool_t)(uiTmp & 0x01);
            pstDat->Q02 = (bool_t)((uiTmp>>1) & 0x01);
            pstDat->Q03 = (bool_t)((uiTmp>>2) & 0x01);
            pstDat->Q04 = (bool_t)((uiTmp>>3) & 0x01);
            pstDat->Q05 = (bool_t)((uiTmp>>4) & 0x01);
            pstDat->Q06 = (bool_t)((uiTmp>>5) & 0x01);
            pstDat->Q07 = (bool_t)((uiTmp>>6) & 0x01);
            pstDat->Q08 = (bool_t)((uiTmp>>7) & 0x01);
            pstDat->Q09 = (bool_t)((uiTmp>>8) & 0x01);
            pstDat->Q10 = (bool_t)((uiTmp>>9) & 0x01);
            pstDat->Q11 = (bool_t)((uiTmp>>10) & 0x01);
            pstDat->Q12 = (bool_t)((uiTmp>>11) & 0x01);
            pstDat->Q13 = (bool_t)((uiTmp>>12) & 0x01);
            pstDat->Q14 = (bool_t)((uiTmp>>13) & 0x01);
            pstDat->Q15 = (bool_t)((uiTmp>>14) & 0x01);
            pstDat->Q16 = (bool_t)((uiTmp>>15) & 0x01);
            pstDat->Q17 = (bool_t)((uiTmp>>16) & 0x01);
            pstDat->Q18 = (bool_t)((uiTmp>>17) & 0x01);
            pstDat->Q19 = (bool_t)((uiTmp>>18) & 0x01);
            pstDat->Q20 = (bool_t)((uiTmp>>19) & 0x01);
            pstDat->Q21 = (bool_t)((uiTmp>>20) & 0x01);
            pstDat->Q22 = (bool_t)((uiTmp>>21) & 0x01);
            pstDat->Q23 = (bool_t)((uiTmp>>22) & 0x01);
            pstDat->Q24 = (bool_t)((uiTmp>>23) & 0x01);
            pstDat->Q25 = (bool_t)((uiTmp>>24) & 0x01);
            pstDat->Q26 = (bool_t)((uiTmp>>25) & 0x01);
            pstDat->Q27 = (bool_t)((uiTmp>>26) & 0x01);
            pstDat->Q28 = (bool_t)((uiTmp>>27) & 0x01);
            pstDat->Q29 = (bool_t)((uiTmp>>28) & 0x01);
            pstDat->Q30 = (bool_t)((uiTmp>>29) & 0x01);
            pstDat->Q31 = (bool_t)((uiTmp>>30) & 0x01);
            pstDat->Q32 = (bool_t)((uiTmp>>31) & 0x01);
        }
        else
        {
            pstDat->bNZ = false;
            pstDat->bCO = false;
            pstDat->Q01 = false;
            pstDat->Q02 = false;
            pstDat->Q03 = false;
            pstDat->Q04 = false;
            pstDat->Q05 = false;
            pstDat->Q06 = false;
            pstDat->Q07 = false;
            pstDat->Q08 = false;
            pstDat->Q09 = false;
            pstDat->Q10 = false;
            pstDat->Q11 = false;
            pstDat->Q12 = false;
            pstDat->Q13 = false;
            pstDat->Q14 = false;
            pstDat->Q15 = false;
            pstDat->Q16 = false;
            pstDat->Q17 = false;
            pstDat->Q18 = false;
            pstDat->Q19 = false;
            pstDat->Q20 = false;
            pstDat->Q21 = false;
            pstDat->Q22 = false;
            pstDat->Q23 = false;
            pstDat->Q24 = false;
            pstDat->Q25 = false;
            pstDat->Q26 = false;
            pstDat->Q27 = false;
            pstDat->Q28 = false;
            pstDat->Q29 = false;
            pstDat->Q30 = false;
            pstDat->Q31 = false;
            pstDat->Q32 = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExtDT_TO_TOD
*
* 功能描述: DT to TOD
*
* 输入参数: uiDT
*
* 输出参数: 无
*
* 返 回 值  : TOD
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtDT_TO_TOD(uint32_t uiDT)
{
    uint32_t uiTOD = 0;

    uiTOD = (uiDT % DAY_TO_SECONDS)*1000;

    return uiTOD;
}

/*
***********************************************************************************************************************
* 函数名称: ExtADD_TOD_TIME
*
* 功能描述: Add tod time
*
* 输入参数: uiTOD uiTime
*
* 输出参数: 无
*
* 返 回 值  : tod+time
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtADD_TOD_TIME(uint32_t uiTOD, uint32_t uiTime)
{
    uint32_t uiResult = 0;

    uiResult = (uiTOD + uiTime) % DAY_TO_MILLISECONDS;

    return uiResult;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSUB_TOD_TIME
*
* 功能描述: Sub tod time
*
* 输入参数: uiTOD uiTime
*
* 输出参数: 无
*
* 返 回 值  : tod-time
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtSUB_TOD_TIME(uint32_t uiTOD, uint32_t uiTime)
{
    uint32_t uiResult = 0;

    if(uiTOD >= uiTime)
    {
        uiResult = uiTOD - uiTime;
    }
    else
    {
        uiResult = DAY_TO_MILLISECONDS - ((uiTime - uiTOD) % DAY_TO_MILLISECONDS);
    }

    return uiResult;
}

/*
***********************************************************************************************************************
* 函数名称: ExtADD_DT_TIME
*
* 功能描述: DT类型与TIME类型输入变量进行加法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtADD_DT_TIME(uint32_t uiDTin, uint32_t uiTime)
{
    uint32_t uiDTout = 0;

    uiDTout = uiDTin + (uiTime/1000);

    return uiDTout;
}

/*
***********************************************************************************************************************
* 函数名称: ExtADD_DATE_TIME
*
* 功能描述: DATE类型与TIME类型输入变量进行加法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtADD_DATE_TIME(uint32_t uiDATEin, uint32_t uiTime)
{
    uint32_t uiDATEout = 0;

    uiDATEout = uiDATEin + (uiTime/1000);

    return uiDATEout;
}
/*
***********************************************************************************************************************
* 函数名称: ExtSUB_DT_TIME
*
* 功能描述: DT类型与TIME类型输入变量进行减法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtSUB_DT_TIME(uint32_t uiDTin, uint32_t uiTime)
{
    uint32_t uiDTout = 0;

    uiDTout = uiDTin - (uiTime/1000);

    return uiDTout;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSUB_DT_DT
*
* 功能描述: DT类型与DT类型输入变量进行减法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtSUB_DT_DT(uint32_t uiDTin1, uint32_t uiDTin2)
{
    uint32_t uiTIMEout = 0;

    //second to millisecond
    uiTIMEout = (uiDTin1 - uiDTin2)*1000;

    return uiTIMEout;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSUB_DATE_TIME
*
* 功能描述: DATE类型与TIME类型输入变量进行减法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtSUB_DATE_TIME(uint32_t uiDTin, uint32_t uiTime)
{
    uint32_t uiDATEout = 0;

    uiDATEout = uiDTin - (uiTime/1000);

    return uiDATEout;
}

/*
***********************************************************************************************************************
* 函数名称: ExtSUB_DATE_DATE
*
* 功能描述: DATE类型与DATE类型输入变量进行减法运算
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtSUB_DATE_DATE(uint32_t uiDATEin1, uint32_t uiDATEin2)
{
    uint32_t uiTimeOut = 0;

    uiTimeOut = (uiDATEin1 - uiDATEin2)*1000;

    return uiTimeOut;
}

/*
***********************************************************************************************************************
* 函数名称: ExtCONCAT_DATE_TOD
*
* 功能描述: 将DATE与TOD合成DT
*
* 输入参数: uiDate: 日期  uiTod: time of day
*
* 输出参数: 无
*
* 返 回 值  : date and time
*
* 注意事项: 无
***********************************************************************************************************************
*/
static uint32_t ExtCONCAT_DATE_TOD(uint32_t uiDate, uint32_t uiTod)
{
    uint32_t uiDT = 0;

    if(uiTod < DAY_TO_SECONDS*1000)
    {
        if(uiDate < 0xFFFFFFFF - DAY_TO_SECONDS)
        {
            uiDT = uiDate + (uiTod/1000);
        }
    }
    return uiDT;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibRS
*
* 功能描述: 复位优先双稳态功能
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibRS(RStype_t *pstRSDat)
{
    if(pstRSDat != NULL)
    {
        pstRSDat->Q1 = (char_t)(!pstRSDat->RESET1 && (pstRSDat->SET || pstRSDat->Q1));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSR
*
* 功能描述: 置位优先双稳态功能
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibSR(SRtype_t *pstSRDat)
{
    if(pstSRDat != NULL)
    {
        pstSRDat->Q1 = (char_t) (pstSRDat->SET1 || (!pstSRDat->RESET && pstSRDat->Q1));
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibCTD
*
* 功能描述: 减计数器
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibCTD(CTDtype_t *pstCTDDat)
{
    if(pstCTDDat != NULL)
    {/* 地址非空 */
        if(pstCTDDat->LOAD)
        {/* load =1,set pv = cv */
            pstCTDDat->CV = pstCTDDat->PV;
        }
        else if((pstCTDDat->CD == true) && (pstCTDDat->CV > MIN_INT))
        {/* trigger here, cv -- */
            pstCTDDat->CV --;
        }
        else
        {
            ;
        }

        if(pstCTDDat->CV <= 0)
        {
            pstCTDDat->Q = true;
        }
        else
        {
            pstCTDDat->Q = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibCTU
*
* 功能描述: 增计数器
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibCTU(CTUtype_t *pstCTUDat)
{
    if(pstCTUDat != NULL)
    {/* 非空 */
        if(pstCTUDat->RESET)
        {/* reset ture, cv = 0 */
            pstCTUDat->CV = 0;
        }
        else if((pstCTUDat->CU == true) && (pstCTUDat->CU < MAX_INT))
        {/* trigger here, cv ++ */
            pstCTUDat->CV ++;
        }
        else
        {
            ;
        }

        if(pstCTUDat->CV >= pstCTUDat->PV)
        {
            pstCTUDat->Q = true;
        }
        else
        {
            pstCTUDat->Q = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibCTUD
*
* 功能描述: 增减计数器
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibCTUD(CTUDtype_t *pstCTUDDat)
{

    if(pstCTUDDat != NULL)
    {/* 非空 */
        if(pstCTUDDat->RESET)
        {/* reset=1,cv =0 */
            pstCTUDDat->CV = 0;
        }
        else if(pstCTUDDat->LOAD)
        {/* load=1,pv = cv*/
            pstCTUDDat->CV = pstCTUDDat->PV;
        }
        else if((pstCTUDDat->CU == true) && (pstCTUDDat->CD == false) && (pstCTUDDat->CV < MAX_INT))
        {/* count ++ */
            pstCTUDDat->CV ++;
        }
        else if((pstCTUDDat->CU == false) && (pstCTUDDat->CD == true) && (pstCTUDDat->CV > MIN_INT))
        {/* count --*/
            pstCTUDDat->CV --;
        }

        pstCTUDDat->QU = (char_t)(pstCTUDDat->CV >= pstCTUDDat->PV); /* ++ flag*/
        pstCTUDDat->QD = (char_t)(pstCTUDDat->CV <= 0); /* -- flag*/
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTimeSet
*
* 功能描述: 设定系统时间
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTimeSet(TimeSettype_t *pstTimeSetDat)
{
    uint64_t ulAdjustTime = 0;

    if(pstTimeSetDat != NULL)
    {/* 非空 */
        if(pstTimeSetDat->EN)
        {

            if((pstTimeSetDat->GMT >= 0) && (pstTimeSetDat->MS >= 0))
            {
                ulAdjustTime = pstTimeSetDat->GMT;
                ulAdjustTime = ulAdjustTime * 1000; //ms
                ulAdjustTime += pstTimeSetDat->MS;
                ulAdjustTime = ulAdjustTime * 1000; //us

                if(ulAdjustTime > (2147483647 * 1000000))
                {
                    ulAdjustTime = (2147483647 * 1000000);
                }

                SysAdjustSystemTime(ulAdjustTime);
                SysAdjustSystemTimeEnable();

                pstTimeSetDat->Q = TRUE;
            }
            else
            {
                pstTimeSetDat->Q = FALSE;
            }
        }
        else
        {
            pstTimeSetDat->Q = FALSE;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTimeAdj
*
* 功能描述: 调整系统时间
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTimeAdj(TimeAdjtype_t *pstTimeAdjDat)
{
    uint64_t ulAdjustTime = 0;
    uint64_t ulTime = 0;
    long64_t lAdjTime = 0;

    if(pstTimeAdjDat != NULL)
    {/* 非空 */
        if(pstTimeAdjDat->EN)
        {
            ulTime = SysGetSystemtime();
            lAdjTime = (long64_t)pstTimeAdjDat->MS;
            if(lAdjTime >= 0)
            {
                ulAdjustTime = ((uint64_t)lAdjTime) * 1000;//us
                ulTime += ulAdjustTime;
            }
            else
            {
                ulAdjustTime = (lAdjTime*(-1)) * 1000;//us
                if(ulTime > ulAdjustTime)
                {
                    ulTime -= ulAdjustTime;
                }
                else
                {
                    ulTime = 0;
                }
            }
            SysAdjustSystemTime(ulTime);
            SysAdjustSystemTimeEnable();
            pstTimeAdjDat->Q = TRUE;
        }
        else
        {
            pstTimeAdjDat->Q = FALSE;
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibRTC
*
* 功能描述: 实时时钟
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibRTC(RTCtype_t *pstRTCDat)
{
    ulong64_t ulTime = 0;

    if(pstRTCDat != NULL)
    {/* 非空 */
        if(pstRTCDat->IN)
        {
            pstRTCDat->Q = TRUE;
            ulTime = SysGetSystemtime();
            pstRTCDat->CurrentDateTime = ulTime / 1000000; //s
        }
        else
        {
            pstRTCDat->Q = FALSE;
            pstRTCDat->CurrentDateTime = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTOF
*
* 功能描述: TOF
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTOF(TOFtype_t *pstDat)
{
    ulong64_t ulTime = 0;
    uint32_t curTime = 0;

    if(pstDat != NULL)
    {
        if ((!pstDat->IN )&& (pstDat->M))
        {/* get start time */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = ulTime / 1000;
        }
        else if (!pstDat->IN)
        {
            if (pstDat->Q)
            {
                ulTime = SysGetFreeCounterValue();
                curTime = ulTime / 1000;
                //if(curTime >= pstDat->ST)
                {/*curTime > time */
                    pstDat->ET = curTime - pstDat->ST;
                }

                if (pstDat->ET >= pstDat->PT)
                {/* time done */
                    pstDat->Q = 0;
                    pstDat->ET = pstDat->PT;
                }
            }
        }
        else //if (pstDat->IN)
        {/* */
            pstDat->Q = 1;
            pstDat->ET = 0;
        }

        pstDat->M = pstDat->IN; /* 过程值 */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTOFR
*
* 功能描述: TOF
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTOFR(TOFRtype_t *pstDat)
{
    uint64_t ulTime = 0;
    uint64_t curTime = 0;

    if(pstDat != NULL)
    {
        if ((!pstDat->IN )&& (pstDat->M))
        {/* get start time */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = ulTime / 1000;
        }
        else if (!pstDat->IN)
        {
            if (pstDat->Q)
            {
                ulTime = SysGetFreeCounterValue();
                curTime = ulTime / 1000;
                //if(curTime >= pstDat->ST)
                {/*curTime > time */
                    pstDat->ET = ((float_t)(curTime - pstDat->ST))/1000.0;
                }

                if (pstDat->ET >= pstDat->PT)
                {/* time done */
                    pstDat->Q = 0;
                    pstDat->ET = pstDat->PT;
                }
            }
        }
        else //if (pstDat->IN)
        {/* */
            pstDat->Q = 1;
            pstDat->ET = 0;
        }

        pstDat->M = pstDat->IN; /* 过程值 */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTON
*
* 功能描述: TON
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTON(TONtype_t *pstDat)
{
    ulong64_t ulTime = 0;
    uint32_t uiCurTime = 0U;

    if(pstDat != NULL)
    {/* 非空 */
        if ((pstDat->IN) && (!pstDat->M))
        {/* Start Timer */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = ulTime/1000;//单位ms
        }
        else if (pstDat->IN)
        {/* Timer is running */
            if (!pstDat->Q)
            {/* 计时未完成 */
                ulTime = SysGetFreeCounterValue();
                uiCurTime = ulTime / 1000;//单位ms

                //if(uiCurTime >= pstDat->ST)
                {/* 计算计时 */
                    pstDat->ET = uiCurTime - pstDat->ST;
                }

                if (pstDat->ET >= pstDat->PT)
                {/* 计时完成 */
                    pstDat->Q = 1;
                    pstDat->ET = pstDat->PT;
                }
            }
        }
        else
        {/* Reset everything */
            pstDat->Q = 0;
            pstDat->ET = 0;
        }
        pstDat->M = pstDat->IN;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTONR
*
* 功能描述: TON
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTONR(TONRtype_t *pstDat)
{
    uint64_t ulTime = 0;
    uint64_t ulCurTime = 0U;

    if(pstDat != NULL)
    {/* 非空 */
        if ((pstDat->IN) && (!pstDat->M))
        {/* Start Timer */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = ulTime / 1000;//ms
        }
        else if (pstDat->IN)
        {/* Timer is running */
            if (!pstDat->Q)
            {/* 计时未完成 */
                ulTime = SysGetFreeCounterValue();
                ulCurTime = ulTime/1000;//ms

                {/* 计算计时 */
                    pstDat->ET = ((float_t)(ulCurTime - pstDat->ST))/1000.0; //s
                }

                if (pstDat->ET >= pstDat->PT)
                {/* 计时完成 */
                    pstDat->Q = 1;
                    pstDat->ET = pstDat->PT;
                }
            }
        }
        else
        {/* Reset everything */
            pstDat->Q = 0;
            pstDat->ET = 0.0;
        }
        pstDat->M = pstDat->IN;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTP
*
* 功能描述: TP
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTP(TPtype_t *pstDat)
{
    ulong64_t ulTime = 0;
    uint32_t curTime = 0U;

    if(pstDat != NULL)
    {/* 非空 */
        if (pstDat->Q)
        {
            ulTime = SysGetFreeCounterValue();
            curTime = (uint32_t)(ulTime/1000);

            if(curTime >= pstDat->ST)
            {
                pstDat->ET = curTime - pstDat->ST; /* compute time */
            }

            if (pstDat->ET >= pstDat->PT)
            {/* 计时完成 */
                pstDat->Q = 0;  /* set Q value */

                if (!pstDat->IN)
                {
                    pstDat->ET = 0;  /* set ET value */
                }
                else
                {
                    pstDat->ET = pstDat->PT; /* set ET value */
                }
            }
        }
        else if ((!pstDat->IN)&& (!pstDat->Q))
        {/* 计时 */
            pstDat->ET = 0U;
        }
        else if ((pstDat->IN) &&(!pstDat->Q) &&( pstDat->ET == 0))
        {/* 计时 */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = (uint32_t)(ulTime/1000);
            pstDat->Q = 1U;
            pstDat->ET = 0U;
        }
        else
        {
            ;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTPR
*
* 功能描述: TP_R
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTPR(TPRtype_t *pstDat)
{
    uint64_t ulTime = 0;
    uint64_t curTime = 0U;

    if(pstDat != NULL)
    {/* 非空 */
        if (pstDat->Q)
        {
            ulTime = SysGetFreeCounterValue();
            curTime = ulTime/1000;//ms

            //if(curTime >= pstDat->ST)
            {
                pstDat->ET = ((float_t)(curTime - pstDat->ST))/1000.0; //s
            }

            if (pstDat->ET >= pstDat->PT)
            {/* 计时完成 */
                pstDat->Q = 0;  /* set Q value */

                if (!pstDat->IN)
                {
                    pstDat->ET = 0;  /* set ET value */
                }
                else
                {
                    pstDat->ET = pstDat->PT; /* set ET value */
                }
            }
        }
        else if ((!pstDat->IN)&& (!pstDat->Q))
        {/* 计时 */
            pstDat->ET = 0U;
        }
        else if ((pstDat->IN) &&(!pstDat->Q) &&( pstDat->ET <= 0.000001))
        {/* 计时 */
            ulTime = SysGetFreeCounterValue();
            pstDat->ST = ulTime/1000;//ms
            pstDat->Q = 1U;
            pstDat->ET = 0;
        }
        else
        {
            ;
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibFTRIG
*
* 功能描述: F_TRIG
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibFTRIG(TRIGtype_t *pstDat)
{
    if(pstDat != NULL)
    {/* 非空 */
        pstDat->Q = ((!pstDat->CLK) && (!pstDat->M)); /* if a faling edge, q=1 else 0 */
        pstDat->M = (!pstDat->CLK);
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibRTRIG
*
* 功能描述: R_TRIG
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibRTRIG(TRIGtype_t *pstDat)
{
    if(pstDat != NULL)
    {/* 非空 */
        pstDat->Q = ((pstDat->CLK) && (!pstDat->M)); /* if a faling edge, q=1 else 0 */
        pstDat->M = (pstDat->CLK);
    }
}
/*
***********************************************************************************************************************
* 函数名称: ExlibBLINK
*
* 功能描述: BLINK
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibBLINK(BLINKtype_t *pstDat)
{
    uint64_t ulTime = 0;
    uint32_t uiTon = 0;

    if(pstDat != NULL)
    {/* 非空 */
        if(pstDat->RESET)
        {
            ulTime = SysGetFreeCounterValue();
            pstDat->ulST = ulTime/1000;
            pstDat->Q = false;
        }
        else if(pstDat->IN)
        {//IN 使能功能块
            ulTime = SysGetFreeCounterValue();
            uiTon = (uint32_t)(ulTime/1000 - pstDat->ulST);
            if(pstDat->Q == true)
            {
                if(uiTon >= pstDat->uiTon)
                {
                    pstDat->Q = false;
                    pstDat->ulST = ulTime/1000;
                }
            }
            else
            {
                if(uiTon >= pstDat->uiToff)
                {
                    pstDat->Q = true;
                    pstDat->ulST = ulTime/1000;
                }
            }
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibBLINKR
*
* 功能描述: BLINK_R
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibBLINKR(BLINKRtype_t *pstDat)
{
    ulong64_t ulTime = 0;
    float_t fTon = 0;

    if(pstDat != NULL)
    {/* 非空 */
        if(pstDat->RESET)
        {
            ulTime = SysGetFreeCounterValue();
            pstDat->ulST = ulTime/1000;
            pstDat->Q = false;
        }
        else if(pstDat->IN)
        {//IN 使能功能块
            ulTime = SysGetFreeCounterValue();
            fTon = ((float_t)(ulTime/1000 - pstDat->ulST))/1000.0;//s
            if(pstDat->Q == true)
            {
                if(fTon >= pstDat->fTon)
                {
                    pstDat->Q = false;
                    pstDat->ulST = ulTime/1000;
                }
            }
            else
            {
                if(fTon >= pstDat->fToff)
                {
                    pstDat->Q = true;
                    pstDat->ulST = ulTime/1000;
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTOGGLE
*
* 功能描述: TOGGLE
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTOGGLE(TOGGLEtype_t *pstDat)
{
    if(pstDat != NULL)
    {
        if(pstDat->RESET == true)
        {
            pstDat->Q = false;
        }
        else
        {
            if(pstDat->IN == true)
            {
                if(pstDat->Q == true)
                {
                    pstDat->Q = false;
                }
                else
                {
                    pstDat->Q = true;
                }
            }
        }

    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExlibMBMasterStart
*
* 功能描述: Modbus主站启动
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibMBMasterStart(MBMastertype_t *pstMBMasterDat)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstMBMasterDat != NULL)
    {/* 非空 */
        if((pstMBMasterDat->CI) && (pstMBMasterDat->uiBlockIndex >= MIN_MODBUS_BLOCK_ID)
                && (pstMBMasterDat->uiBlockIndex <= MAX_MODBUS_BLOCK_ID))
        {
            uiBlockIndex = pstMBMasterDat->uiBlockIndex - 1;
            pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
            if(pstAppend->ucModBusCtrl[uiBlockIndex] == MODBUS_STATE_NOCONFIG)
            {
                pstMBMasterDat->CO = FALSE;
                pstMBMasterDat->iStat = 0;
            }
            else
            {
                pstAppend->ucModBusCtrl[uiBlockIndex] = MODBUS_STATE_START;
                pstMBMasterDat->CO = TRUE;
                pstMBMasterDat->iStat = 1;
            }
        }
        else
        {
            pstMBMasterDat->CO = FALSE;
            pstMBMasterDat->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibMBMasterStop
*
* 功能描述: Modbus主站停止
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibMBMasterStop(MBMastertype_t *pstMBMasterDat)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstMBMasterDat != NULL)
    {/* 非空 */
        if((pstMBMasterDat->CI) && (pstMBMasterDat->uiBlockIndex >= MIN_MODBUS_BLOCK_ID)
                && (pstMBMasterDat->uiBlockIndex <= MAX_MODBUS_BLOCK_ID))
        {
            uiBlockIndex = pstMBMasterDat->uiBlockIndex - 1;
            pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
            if(pstAppend->ucModBusCtrl[uiBlockIndex] == MODBUS_STATE_NOCONFIG)
            {
                pstMBMasterDat->CO = FALSE;
                pstMBMasterDat->iStat = 0;
            }
            else
            {
                pstAppend->ucModBusCtrl[uiBlockIndex] = MODBUS_STATE_STOP;
                pstMBMasterDat->CO = TRUE;
                pstMBMasterDat->iStat = 2;
            }
        }
        else
        {
            pstMBMasterDat->CO = FALSE;
            pstMBMasterDat->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibMBState
*
* 功能描述: 读Modbus状态
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibMBState(MBMastertype_t *pstMBMasterDat)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstMBMasterDat != NULL)
    {/* 非空 */
        if(pstMBMasterDat->CI)
        {/* CI为true */
            if((pstMBMasterDat->uiBlockIndex < MIN_MODBUS_BLOCK_ID) || (pstMBMasterDat->uiBlockIndex > MAX_MODBUS_BLOCK_ID))
            {/* 块号不在范围内 */
                pstMBMasterDat->CO = false;
                pstMBMasterDat->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstMBMasterDat->uiBlockIndex - 1;
                pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                if(pstAppend->ucModBusCtrl[uiBlockIndex] == MODBUS_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstMBMasterDat->CO = false;
                    pstMBMasterDat->iStat = 0;
                }
                else if(pstAppend->ucModBusCtrl[uiBlockIndex] == MODBUS_STATE_START)
                {/* 块号未停止 */
                    pstMBMasterDat->CO = true;
                    pstMBMasterDat->iStat = 1;
                }
                else if(pstAppend->ucModBusCtrl[uiBlockIndex] == MODBUS_STATE_STOP)
                {
                    pstMBMasterDat->CO = true;
                    pstMBMasterDat->iStat = 2;
                }
                else
                {
                    ;
                }
            }
        }
        else
        {
            pstMBMasterDat->CO = false;
            pstMBMasterDat->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSOEClear
*
* 功能描述: 清SOE记录
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibSOEClear(SOEtype_t *pstSOEData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstSOEData != NULL)
    {
        if(pstSOEData->CI)
        {
            if((pstSOEData->uiBlockIndex < SOE_MIN_BLOCK_ID) || (pstSOEData->uiBlockIndex > SOE_MAX_BLOCK_ID))
            {/* 块号不在范围内 */
                pstSOEData->CO = false;
                pstSOEData->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstSOEData->uiBlockIndex - 1;
                pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstSOEData->CO = false;
                    pstSOEData->iStat = 0;
                }
                else if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_START)
                {/* 块号未停止 */
                    pstSOEData->CO = false;
                    pstSOEData->iStat = 1;
                }
                else
                {
                    SharedClearSOE(uiBlockIndex);
                    pstSOEData->CO = true;
                    pstSOEData->iStat = 2;
                }
            }
        }
        else
        {
            pstSOEData->CO = false;
            pstSOEData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSOEState
*
* 功能描述: 读SOE状态
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibSOEState(SOEtype_t *pstSOEData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstSOEData != NULL)
    {
        if(pstSOEData->CI)
        {
            if((pstSOEData->uiBlockIndex < SOE_MIN_BLOCK_ID) || (pstSOEData->uiBlockIndex > SOE_MAX_BLOCK_ID))
            {/* 块号不在范围内 */
                pstSOEData->CO = false;
                pstSOEData->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstSOEData->uiBlockIndex - 1;
                pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstSOEData->CO = false;
                    pstSOEData->iStat = 0;
                }
                else if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_START)
                {/* 块号未停止 */
                    pstSOEData->CO = true;
                    pstSOEData->iStat = 1;
                }
                else if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_STOP)
                {
                    pstSOEData->CO = true;
                    pstSOEData->iStat = 2;
                }
                else
                {
                    ;
                }
            }
        }
        else
        {
            pstSOEData->CO = false;
            pstSOEData->iStat = 0;
        }

    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSOEStop
*
* 功能描述: SOE停止
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibSOEStop(SOEtype_t *pstSOEData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstSOEData != NULL)
    {/* 非空且CI为true */
        if(pstSOEData->CI)
        {
            if((pstSOEData->uiBlockIndex < SOE_MIN_BLOCK_ID) || (pstSOEData->uiBlockIndex > SOE_MAX_BLOCK_ID))
            {/* 块号不在范围内 */
                pstSOEData->CO = false;
                pstSOEData->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstSOEData->uiBlockIndex - 1;
                pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstSOEData->CO = false;
                    pstSOEData->iStat = 0;
                }
                else
                {
                    pstAppend->ucSOECtrl[uiBlockIndex] = SOE_STATE_STOP;
                    pstSOEData->CO = true;
                    pstSOEData->iStat = 2;
                }
            }
        }
        else
        {
            pstSOEData->CO = false;
            pstSOEData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSOEStart
*
* 功能描述: SOE启动
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibSOEStart(SOEtype_t *pstSOEData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    if(pstSOEData != NULL)
    {
        if(pstSOEData->CI)
        {
            if((pstSOEData->uiBlockIndex < SOE_MIN_BLOCK_ID) || (pstSOEData->uiBlockIndex > SOE_MAX_BLOCK_ID))
            {/* 块号不在范围内 */
                pstSOEData->CO = false;
                pstSOEData->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstSOEData->uiBlockIndex - 1;
                pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();
                if(pstAppend->ucSOECtrl[uiBlockIndex] == SOE_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstSOEData->CO = false;
                    pstSOEData->iStat = 0;
                }
                else
                {
                    pstAppend->ucSOECtrl[uiBlockIndex] = SOE_STATE_START;
                    pstSOEData->CO = true;
                    pstSOEData->iStat = 1;
                }
            }
        }
        else
        {
            pstSOEData->CO = false;
            pstSOEData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibSysTimeSet
*
* 功能描述: CM对PM校时
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
#if 0
static void ExlibSysTimeSet(Timetype_t *pstTimeData)
{
    ulong64_t ulTime = 0;
    bool_t bRet = false;
    AdjSysTimeSignal_t stSysTime;

    if(pstTimeData != NULL)
    {/* 非空 */
        if(pstTimeData->EN)
        {
            //if(!pstTimeData->M)
            {
                bRet = SysGetSysTimeSignal(CORE1_TO_CORE0, &stSysTime);
                if((true == bRet) && (stSysTime.uiSignal == SIGNAL_CM_SYS_TIME))
                {
                    ulTime = (ulong64_t)stSysTime.uiSec * 1000; //ms
                    //printf("sec=%d\n",stSysTime.uiSec);
                    ulTime += (ulong64_t)(stSysTime.uiMilSec );

                    ulTime = ulTime * 1000; //us

                    SysAdjustSystemTime(ulTime);
                    SysAdjustSystemTimeEnable();
                    pstTimeData->M = TRUE;
                    pstTimeData->Q = TRUE;
                }
            }
        }
        else
        {
            pstTimeData->M = FALSE;
            pstTimeData->Q = FALSE;
        }
    }

    return;
}
#endif
/*
***********************************************************************************************************************
* 函数名称: ExlibP2PStop
*
* 功能描述: P2P停止
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ExlibP2PStop(P2Ptype_t *pstP2PData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    if(pstP2PData != NULL)
    {/* 非空 */
        if((pstP2PData->CI) && (pstP2PData->uiBlockIndex >= MIN_P2P_BLOCK_ID) \
                && (pstP2PData->uiBlockIndex <= MAX_P2P_BLOCK_ID) && (pstAppend != NULL))
        {
            uiBlockIndex = pstP2PData->uiBlockIndex - 1;
            if(pstAppend->ucP2PCtrl[uiBlockIndex] == P2P_STATE_NOCONFIG)
            {
                pstP2PData->CO = FALSE;
                pstP2PData->iStat = 0;
            }
            else
            {
                pstAppend->ucP2PCtrl[uiBlockIndex] = P2P_STATE_STOP;
                pstP2PData->CO = TRUE;
                pstP2PData->iStat = 2;
            }
        }
        else
        {
            pstP2PData->CO = FALSE;
            pstP2PData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibP2PStart
*
* 功能描述: P2P启动
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibP2PStart(P2Ptype_t *pstP2PData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    if(pstP2PData != NULL)
    {/* 非空 */
        if((pstP2PData->CI) && (pstP2PData->uiBlockIndex >= MIN_P2P_BLOCK_ID) \
                && (pstP2PData->uiBlockIndex <= MAX_P2P_BLOCK_ID) && (pstAppend != NULL))
        {
            uiBlockIndex = pstP2PData->uiBlockIndex - 1;
            if(pstAppend->ucP2PCtrl[uiBlockIndex] == P2P_STATE_NOCONFIG)
            {
                pstP2PData->CO = FALSE;
                pstP2PData->iStat = 0;
            }
            else
            {
                pstAppend->ucP2PCtrl[uiBlockIndex] = P2P_STATE_START;
                pstP2PData->CO = TRUE;
                pstP2PData->iStat = 1;
            }
        }
        else
        {
            pstP2PData->CO = FALSE;
            pstP2PData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibP2PState
*
* 功能描述: 读P2P状态
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibP2PState(P2Ptype_t *pstP2PData)
{
    uint32_t uiBlockIndex = 0;
    SharedAppend_t *pstAppend = NULL;

    pstAppend = (SharedAppend_t *) SysGetSharedMemAppendInfoStartAddr();

    if(pstP2PData != NULL)
    {
        if(pstP2PData->CI)
        {
            if((pstP2PData->uiBlockIndex < MIN_P2P_BLOCK_ID) || (pstP2PData->uiBlockIndex > MAX_P2P_BLOCK_ID) || (pstAppend == NULL))
            {/* 块号不在范围内或pstAppend为空 */
                pstP2PData->CO = false;
                pstP2PData->iStat = 0;
            }
            else
            {
                uiBlockIndex = pstP2PData->uiBlockIndex - 1;

                if(pstAppend->ucP2PCtrl[uiBlockIndex] == P2P_STATE_NOCONFIG)
                {/* 块号未配置 */
                    pstP2PData->CO = false;
                    pstP2PData->iStat = 0;
                }
                else if(pstAppend->ucP2PCtrl[uiBlockIndex] == P2P_STATE_START)
                {/* 块号未停止 */
                    pstP2PData->CO = true;
                    pstP2PData->iStat = 1;
                }
                else if(pstAppend->ucP2PCtrl[uiBlockIndex] == P2P_STATE_STOP)
                {
                    pstP2PData->CO = true;
                    pstP2PData->iStat = 2;
                }
                else
                {
                    ;
                }
            }
        }
        else
        {
            pstP2PData->CO = false;
            pstP2PData->iStat = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibP2PSendStatus
*
* 功能描述: P2P发送状态
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibP2PSendStatus(P2PSendtype_t *pstP2PData)
{
    uint32_t uiNode = 0;
    SysP2PSendReqCommInfo_t stSendReqInfo;

    if(pstP2PData != NULL)
    {/* 非空且CI为true */
        if(pstP2PData->En)
        {
            if((pstP2PData->Node< MIN_P2P_STATION_ID) || (pstP2PData->Node > MAX_P2P_STATION_ID))
            {/* 块号不在范围内 */
                pstP2PData->Q = false;
                pstP2PData->bErrFlag = 0;
                pstP2PData->bTolFlag = 0;
                pstP2PData->bSendTimeout = 0;
            }
            else
            {
                uiNode = pstP2PData->Node - 1;
                SysGetP2PSendReqCommInfo((uint8_t)uiNode, &stSendReqInfo );
                pstP2PData->Q = true;
                pstP2PData->bErrFlag = stSendReqInfo.bErrFlag;
                pstP2PData->bTolFlag = stSendReqInfo.bTolFlag;
                pstP2PData->bSendTimeout = stSendReqInfo.bSendTimeout;
            }
        }
        else
        {
            pstP2PData->Q = false;
            pstP2PData->bErrFlag = 0;
            pstP2PData->bTolFlag = 0;
            pstP2PData->bSendTimeout = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibP2PRecvStatus
*
* 功能描述: P2P接收状态
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibP2PRecvStatus(P2PRecvtype_t *pstP2PData)
{
    uint32_t uiNode = 0;
    SysP2PRecvReqCommInfo_t stRecvReqInfo;

    if(pstP2PData != NULL)
    {/* 非空且CI为true */
        if(pstP2PData->En)
        {
            if((pstP2PData->Node< MIN_P2P_STATION_ID) || (pstP2PData->Node > MAX_P2P_STATION_ID))
            {/* 块号不在范围内 */
                pstP2PData->Q = false;
                pstP2PData->bErrFlag = 0;
                pstP2PData->bTolFlag = 0;
            }
            else
            {
                uiNode = pstP2PData->Node - 1;
                SysGetP2PRecvReqCommInfo((uint8_t)uiNode, &stRecvReqInfo );
                pstP2PData->Q = true;
                pstP2PData->bErrFlag = stRecvReqInfo.bErrFlag;
                pstP2PData->bTolFlag = stRecvReqInfo.bTolFlag;
            }
        }
        else
        {
            pstP2PData->Q = false;
            pstP2PData->bErrFlag = 0;
            pstP2PData->bTolFlag = 0;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DisableBit
*
* 功能描述:
*
* 输入参数: pucAddr 地址
*        uiBitPos 字节内偏移
*        bValue 相应位的值  true/false
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void DisableBit(puint8_t pucAddr, uint32_t uiBitPos, bool_t bValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagAddr = NULL;
    uint32_t uiOffset = 0;
    uint8_t ucFlagContent = 0;

    if((pucAddr != NULL) && (uiBitPos <= 7))
    {
        if((pucAddr >= pucRtDataStartAddr) && (pucAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = pucAddr - pucRtDataStartAddr;
            pucFlagAddr = pucFlagStartAddr + uiOffset;
            ucFlagContent = *pucFlagAddr;
            if(UNDISABLED == ((ucFlagContent>>uiBitPos)&0x01))
            {
                SetBitValue(pucAddr, uiBitPos, bValue);
            }
        }
        else
        {
            SetBitValue(pucAddr, uiBitPos, bValue);
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DisableByte
*
* 功能描述:
*
* 输入参数: pucAddr 地址
*        ucValue 相应的值
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void DisableByte(puint8_t pucAddr, uint8_t ucValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagAddr = NULL;
    uint32_t uiOffset = 0;

    if(pucAddr != NULL)
    {
        if((pucAddr >= pucRtDataStartAddr) && (pucAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = pucAddr - pucRtDataStartAddr;
            pucFlagAddr = pucFlagStartAddr + uiOffset;

            if(UNDISABLED == *pucFlagAddr)
            {
                *pucAddr = ucValue;
            }
        }
        else
        {
            *pucAddr = ucValue;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DisableWord
*
* 功能描述:
*
* 输入参数: pusAddr 地址
*        usValue 相应的值
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void DisableWord(puint16_t pusAddr, uint16_t usValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    puint16_t pusFlagAddr = NULL;
    uint32_t uiOffset = 0;

    if(pusAddr != NULL)
    {
        if(((puint8_t)pusAddr >= pucRtDataStartAddr) && ((puint8_t)pusAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = (puint8_t)pusAddr - pucRtDataStartAddr;
            pusFlagAddr = (puint16_t)(pucFlagStartAddr + uiOffset);

            if(UNDISABLED == *pusFlagAddr)
            {
                *pusAddr = usValue;
            }
        }
        else
        {
            *pusAddr = usValue;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: DisableDWord
*
* 功能描述:
*
* 输入参数: puiAddr 地址
*        uiValue 相应的值
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void DisableDWord(puint32_t puiAddr, uint32_t uiValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    puint32_t puiFlagAddr = NULL;
    uint32_t uiOffset = 0;

    if(puiAddr != NULL)
    {
        if(((puint8_t)puiAddr >= pucRtDataStartAddr) && ((puint8_t)puiAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = (puint8_t)puiAddr - pucRtDataStartAddr;
            puiFlagAddr = (puint32_t)(pucFlagStartAddr + uiOffset);

            if(UNDISABLED == *puiFlagAddr)
            {
                *puiAddr = uiValue;
            }
        }
        else
        {
            *puiAddr = uiValue;
        }
    }
    return;
}
static void DisableFloat(float_t* pfAddr, float_t fValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    puint32_t puiFlagAddr = NULL;
    uint32_t uiOffset = 0;

    if(pfAddr != NULL)
    {
        if(((puint8_t)pfAddr >= pucRtDataStartAddr) && ((puint8_t)pfAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = (puint8_t)pfAddr - pucRtDataStartAddr;
            puiFlagAddr = (puint32_t)(pucFlagStartAddr + uiOffset);

            if(UNDISABLED == *puiFlagAddr)
            {
                *pfAddr = fValue;
            }
        }
        else
        {
            *pfAddr = fValue;
        }
    }
    return;
}
static void DisableDouble(double_t* pdAddr, double_t dValue)
{
    puint8_t pucRtDataStartAddr = SysGetPRGRtDataStartAddr(RT_DATA_TYPE_G);
    puint8_t pucFlagStartAddr = SysGetRtDataDisableFlagStartAddr(RT_DATA_TYPE_G);
    long64_t* plFlagAddr = NULL;
    uint32_t uiOffset = 0;

    if(pdAddr != NULL)
    {
        if(((puint8_t)pdAddr >= pucRtDataStartAddr) && ((puint8_t)pdAddr < pucRtDataStartAddr + RT_DATA_AREA_SIZE))
        {
            uiOffset = (puint8_t)pdAddr - pucRtDataStartAddr;
            plFlagAddr = (long64_t*)(pucFlagStartAddr + uiOffset);

            if(UNDISABLED == *plFlagAddr)
            {
                *pdAddr = dValue;
            }
        }
        else
        {
            *pdAddr = dValue;
        }
    }
    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtDIV_DW
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtDIV_DW(DIV_DWtype_t *pstData)
{
    if(pstData != NULL)
    {
        if(pstData->uiIN2 != 0)
        {
            pstData->uiOUT = pstData->uiIN1 / pstData->uiIN2;
            pstData->bERR = false;
        }
        else
        {
            pstData->bERR = true;
        }
    }
}
/*
***********************************************************************************************************************
* 函数名称: ExtDIV_D
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtDIV_D(DIV_Dtype_t *pstData)
{
    if(pstData != NULL)
    {
        if(pstData->iIN2 != 0)
        {
            pstData->iOUT = pstData->iIN1 / pstData->iIN2;
            pstData->bERR = false;
        }
        else
        {
            pstData->bERR = true;
        }
    }
}
/*
***********************************************************************************************************************
* 函数名称: ExtDIV_R
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtDIV_R(DIV_Rtype_t *pstData)
{
    if(pstData != NULL)
    {
        if((pstData->fIN2 < -0.000000001) || (pstData->fIN2 > 0.000000001))
        {
            pstData->fOUT = pstData->fIN1 / pstData->fIN2;
            pstData->bERR = false;
        }
        else
        {
            pstData->bERR = true;
        }
    }
}
/*
***********************************************************************************************************************
* 函数名称: ExtDIV_LR
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtDIV_LR(DIV_LRtype_t *pstData)
{
    if(pstData != NULL)
    {
        if((pstData->dIN2 < -0.000000001) || (pstData->dIN2 > 0.000000001))
        {
            pstData->dOUT = pstData->dIN1 / pstData->dIN2;
            pstData->bERR = false;
        }
        else
        {
            pstData->bERR = true;
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: ExtTIMER
*
* 功能描述: 定时器
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtTIMER(TIMERtype_t *pstData)
{
    uint64_t ulTime = 0;
    uint32_t uiDeltaT = 0;

    if(pstData != NULL)
    {
        if(true == pstData->bRESET)
        {
            pstData->bQ = false;
            pstData->uiET = pstData->uiIT;

            ulTime = SysGetFreeCounterValue();

            pstData->uiStart = ulTime/1000; //ms
        }

       if(true == pstData->bIN)
        {
            ulTime = SysGetFreeCounterValue();
            uiDeltaT = (uint32_t)((ulTime/1000) - pstData->uiStart);

            pstData->uiET = uiDeltaT + pstData->uiIT;
            if(pstData->uiET >= pstData->uiPT)
            {
                pstData->uiET = pstData->uiPT;
                pstData->bQ = true;
            }
        }
    }

    return;
}
/*
***********************************************************************************************************************
* 函数名称: ExtTIMER_R
*
* 功能描述: 定时器
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExtTIMER_R(TIMER_Rtype_t *pstData)
{
    uint64_t ulTime = 0;
    int32_t iDeltaT = 0;
    uint32_t uiET = 0;
    float_t fTmp = 0;


    if(pstData != NULL)
    {
        if(true == pstData->bRESET)
        {
            pstData->bQ = false;
            pstData->fET = pstData->fIT;

            ulTime = SysGetFreeCounterValue();

            pstData->uiStart = ulTime/1000; //ms
        }

       if(true == pstData->bIN)
        {
            ulTime = SysGetFreeCounterValue();
            iDeltaT = (int32_t)(ulTime/1000 - pstData->uiStart);//ms

            fTmp = (float_t)(iDeltaT); //ms
            pstData->fET = fTmp/((float_t)1000.0f) + pstData->fIT;//s
            if(pstData->fET >= pstData->fPT)
            {
                pstData->fET = pstData->fPT;
                pstData->bQ = true;
            }

        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExtGetDelta
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static int32_t ExtGetDelta(int32_t iTaskNo)
{
    int32_t DeltaTime = 0;

    if(iTaskNo == 1)
    {
        DeltaTime = (int32_t)uiUP1DeltaTime;
    }
    else if(iTaskNo == 2)
    {
        DeltaTime = (int32_t)uiUP2DeltaTime;
    }
    else
    {
        ;
    }

    return DeltaTime;
}


/*
***********************************************************************************************************************
* 函数名称: ExtGetDelta_R
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  :
*
* 注意事项: 无
***********************************************************************************************************************
*/
static float_t ExtGetDelta_R(int32_t uiTaskNo)
{
    int32_t DeltaTime = 0;
    float_t fTmp = 0;

    if(uiTaskNo == 1)
    {
        DeltaTime = (int32_t)(uiUP1DeltaTime);
        fTmp = ((float_t)(DeltaTime))/1000;
    }
    else if(uiTaskNo == 2)
    {
        DeltaTime = (int32_t)(uiUP2DeltaTime);
        fTmp = ((float_t)(DeltaTime))/1000;
    }
    else
    {
        ;
    }

    return fTmp;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTRScanStatus
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTRScanStatus(SCANSTATUStype_t *pstData)
{
    int32_t iSwitchPos;

    if(pstData != NULL)
    {/* 非空 */
        if (pstData->CI == true)
        {
            /* 钥匙开关位置显示调整，用户使用习惯 */
            switch(SharedGetSwitchPos())
            {
                case 0:
                {/* init */
                    iSwitchPos = 3;
                }
                break;

                case 2:
                {/* prog */
                    iSwitchPos = 2;
                }
                break;

                case 4:
                {/* run */
                    iSwitchPos = 1;
                }
                break;

                case 6:
                {/* stop */
                    iSwitchPos = 0;
                }
                break;

                default:
                {/* 故障 */
                    iSwitchPos = 4;
                }
                break;
            }

            if(pstData->iTaskNo == 1)
            {
                pstData->POWERUP = true;
                pstData->bFirstScan = bUP1DeltaFlag;
                pstData->iKeySwitch = iSwitchPos;
                pstData->iScanDetra = (int32_t)uiUP1DeltaTime;
                pstData->iScanOverrun = (int32_t)uiUP1AverageScanOverrun;
                pstData->iScanRequest = SysGetPrjShedTime(0);;
                pstData->iScanSurplus = (int32_t)uiUP1AverageScanSurplus;
                pstData->uiDeltaT = (int32_t)uiUP1DeltaTime;
                pstData->CO = true;
            }
            else if(pstData->iTaskNo == 2)
            {
                pstData->POWERUP = true;
                pstData->bFirstScan = bUP2DeltaFlag;
                pstData->iKeySwitch = iSwitchPos;
                pstData->iScanDetra = (int32_t)uiUP2DeltaTime;
                pstData->iScanOverrun = (int32_t)uiUP2AverageScanOverrun;
                pstData->iScanRequest = SysGetPrjShedTime(1);;
                pstData->iScanSurplus = (int32_t)uiUP2AverageScanSurplus;
                pstData->uiDeltaT = (int32_t)uiUP2DeltaTime;
                pstData->CO = true;
            }
            else
            {
                pstData->POWERUP = false;
                pstData->bFirstScan = 0;
                pstData->iKeySwitch = 0;
                pstData->iScanDetra = 0;
                pstData->iScanOverrun = 0;
                pstData->iScanRequest = 0;
                pstData->iScanSurplus = 0;
                pstData->uiDeltaT = 0;
                pstData->CO = false;
            }
        }
        else
        {
            pstData->POWERUP = false;
            pstData->bFirstScan = 0;
            pstData->iKeySwitch = 0;
            pstData->iScanDetra = 0;
            pstData->iScanOverrun = 0;
            pstData->iScanRequest = 0;
            pstData->iScanSurplus = 0;
            pstData->uiDeltaT = 0;
            pstData->CO = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibTRCalendar
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibTRCalendar(CALENDARtype_t *pstData)
{
    ulong64_t ulTime = 0;
    uint32_t uiSystemTime = 0;

    TMtype_t tm;

    if(pstData != NULL)
    {/* 非空 */
        if (pstData->CI == true)
        {
            ulTime = SysGetSystemtime() / 1000000;
            uiSystemTime = (uint32_t)ulTime;
            os_gmtime(&uiSystemTime,&tm); /* 秒转日期 */
            pstData->iYear = tm.tm_year + 1900;
            pstData->iMonth = tm.tm_mon + 1; /* 算法中月是从0-11 */
            pstData->iDay = tm.tm_mday;
            pstData->iHour = tm.tm_hour;
            pstData->iMinute = tm.tm_min;
            pstData->iSecond = tm.tm_sec;
            pstData->iMillisec = (SysGetSystemtime() % 1000000) / 1000;
            pstData->iWeekday = tm.tm_wday;
            pstData->iRelsec = (int32_t)ulTime;
            pstData->CO = true;
        }
        else
        {
            pstData->iYear = 0;
            pstData->iMonth = 0;
            pstData->iDay = 0;
            pstData->iHour = 0;
            pstData->iMinute = 0;
            pstData->iSecond = 0;
            pstData->iMillisec = 0;
            pstData->iWeekday = 0;
            pstData->iRelsec = 0;
            pstData->CO = false;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ExlibGetOSPStatus
*
* 功能描述:
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void ExlibOSPStatus(OSPStatustype_t *pstData)
{
    SysOSPMonitorData_t* pstSysOSPMonitorData = NULL;

    if(pstData != NULL)
    {/* 非空 */
        pstData->CO = false;

        if((pstData->ucSlaveNo <= IPBUS_MAX_SLAVE_NO) && (pstData->ucSlaveNo >= IPBUS_MIN_SLAVE_NO) && (pstData->CI == true))
        {
            pstSysOSPMonitorData = SysGetOSPMonitorData(pstData->ucSlaveNo);
            if(pstSysOSPMonitorData != NULL)
            {
                switch(pstData->ucPMID)
                {
                    case 0:
                    {
                        pstData->uiOSPFunction = pstSysOSPMonitorData->uiOSPFunction;
                        pstData->fPRPM = pstSysOSPMonitorData->fPRPMA;
                        pstData->fPAccel = pstSysOSPMonitorData->fPAccelA;
                        pstData->uiAlarmPack = pstSysOSPMonitorData->uiAlarmPackA;
                        pstData->ucDigPack = pstSysOSPMonitorData->ucDigPackA;

                        pstData->CO = true;
                    }
                    break;

                    case 1:
                    {
                        pstData->uiOSPFunction = pstSysOSPMonitorData->uiOSPFunction;
                        pstData->fPRPM = pstSysOSPMonitorData->fPRPMB;
                        pstData->fPAccel = pstSysOSPMonitorData->fPAccelB;
                        pstData->uiAlarmPack = pstSysOSPMonitorData->uiAlarmPackB;
                        pstData->ucDigPack = pstSysOSPMonitorData->ucDigPackB;

                        pstData->CO = true;
                    }
                    break;

                    case 2:
                    {
                        pstData->uiOSPFunction = pstSysOSPMonitorData->uiOSPFunction;
                        pstData->fPRPM = pstSysOSPMonitorData->fPRPMC;
                        pstData->fPAccel = pstSysOSPMonitorData->fPAccelC;
                        pstData->uiAlarmPack = pstSysOSPMonitorData->uiAlarmPackC;
                        pstData->ucDigPack = pstSysOSPMonitorData->ucDigPackC;

                        pstData->CO = true;
                    }
                    break;

                    default :
                    {
                        ;
                    }
                    break;
                }
            }
        }
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
* 函数名称: SetBitValue
*
* 功能描述: pucAddr 地址
*        uiPos 字节内偏移
*        bValue 相应位的值  true/false
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 无
*
* 注意事项: 无
***********************************************************************************************************************
*/
static void SetBitValue(puint8_t pucAddr, uint32_t uiPos, bool_t bValue)
{
    uint8_t uiTmp = 1;
    /* 输入参数进行有效性检查 */
    if((pucAddr != NULL) && (uiPos <= 7))
    {
        if(true == bValue)
        {
            *pucAddr |= (uiTmp << uiPos);
        }
        else
        {
            *pucAddr &= (~(uiTmp << uiPos));
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: StringCompare
*
* 功能描述: pStr1 指向字符串1
*        pStr2 指向字符串2
*
* 输入参数:
*
* 输出参数: 无
*
* 返 回 值  : 0 相等
*        -1 不相等或存在NULL
*
* 注意事项: 不区分大小写
***********************************************************************************************************************
*/
static int32_t StringCompare(char_t *pStr1, char_t *pStr2)
{
    int32_t iRet = -1;
    char_t cName1[MAX_FUNC_NAME_LENGTH];
    char_t cName2[MAX_FUNC_NAME_LENGTH];
    uint8_t i = 0;

    if((pStr1 != NULL) && (pStr2 != NULL))
    {
        //printf("%s %s\n",pStr1, pStr2);
        do
        {
            if((*pStr1 >= 'A') && (*pStr1 <= 'Z'))
            {
                cName1[i] = *pStr1 + 32;
            }
            else
            {
                cName1[i] = *pStr1;
            }
            pStr1++;
            i++;
            if(i >= MAX_FUNC_NAME_LENGTH)
            {
                break;
            }
        }while(1);

        i = 0;
        do
        {
            if((*pStr2 >= 'A') && (*pStr2 <= 'Z'))
            {
                cName2[i] = *pStr2 + 32;
            }
            else
            {
                cName2[i] = *pStr2;
            }
            pStr2++;
            i++;
            if(i >= MAX_FUNC_NAME_LENGTH)
            {
                break;
            }
        }while(1);

        //printf("%s %s\n",&cName1[0], &cName2[0]);
        iRet = strcmp(&cName1[0], &cName2[0]);
    }

    return iRet;
}

static int32_t __isleap(int32_t year)
{
    /* every fourth year is a leap year except for century years that are
     * not divisible by 400. */
    return (!(year % 4) && ((year % 100) || !(year % 400)));
}

static TMtype_t *os_gmtime(const int32_t *timep, TMtype_t *r)
{
    int32_t i;
    int32_t work = *timep % (SPD);
    r->tm_sec = work % 60;
    work /= 60;
    r->tm_min = work % 60;
    r->tm_hour = work / 60;
    work = *timep / (SPD);
    r->tm_wday = (4 + work) % 7;
    for (i = 1970;; ++i)
    {
        int32_t k = __isleap(i) ? 366 : 365;
        if (work >= k)
            work -= k;
        else
            break;
    }
    r->tm_year = i - 1900;
    r->tm_yday = work;

    r->tm_mday = 1;
    if (__isleap(i) && (work > 58))
    {
        if (work == 59)
            r->tm_mday = 2; /* 29.2. */
        work -= 1;
    }

    for (i = 11; i && (__spm[i] > work); --i)
        ;
    r->tm_mon = i;
    r->tm_mday += work - __spm[i];
    return r;
}

static int32_t os_mktime(TMtype_t * const t)
{
    int32_t day;
    int32_t i;
    int32_t years = t->tm_year - 70;

    if (t->tm_sec >= 60)
    {
        t->tm_min += t->tm_sec / 60;
        t->tm_sec %= 60;
    }
    if (t->tm_min >= 60)
    {
        t->tm_hour += t->tm_min / 60;
        t->tm_min %= 60;
    }
    if (t->tm_hour >= 24)
    {
        t->tm_mday += t->tm_hour / 24;
        t->tm_hour %= 24;
    }
    if (t->tm_mon >= 12)
    {
        t->tm_year += t->tm_mon / 12;
        t->tm_mon %= 12;
    }
    while (t->tm_mday > __spm[1 + t->tm_mon])
    {
        if (t->tm_mon == 1 && __isleap(t->tm_year + 1900))
        {
            --t->tm_mday;
        }
        t->tm_mday -= __spm[t->tm_mon];
        ++t->tm_mon;
        if (t->tm_mon > 11)
        {
            t->tm_mon = 0;
            ++t->tm_year;
        }
    }

    if (t->tm_year < 70)
        return (int32_t) -1;

    /* Days since 1970 is 365 * number of years + number of leap years since 1970 */
    day = years * 365 + (years + 1) / 4;

    /* After 2100 we have to substract 3 leap years for every 400 years
     This is not intuitive. Most mktime implementations do not support
     dates after 2059, anyway, so we might leave this out for it's bloat. */
    if (years >= 131)
    {
        years -= 131;
        years /= 100;
        day -= (years >> 2) * 3 + 1;
        if ((years &= 3) == 3)
            years--;
        day -= years;
    }

    day += t->tm_yday = __spm[t->tm_mon] + t->tm_mday - 1 +
            (__isleap(t->tm_year + 1900) & (t->tm_mon > 1));

    /* day is now the number of days since 'Jan 1 1970' */
    i = 7;
    t->tm_wday = (day + 4) % i; /* Sunday=0, Monday=1, ..., Saturday=6 */

    i = 24;
    day *= i;
    i = 60;

    return ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/
