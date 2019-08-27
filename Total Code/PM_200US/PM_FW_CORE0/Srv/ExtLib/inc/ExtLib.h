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
*  作者            : 李琦
*
*  创建日期    : 2016-05-03
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef EXT_LIB_H_
#define EXT_LIB_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "Srv/SysResource/inc/Sys.h"
/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define MATH_LIB

#define MAX_FUNC_NAME_LENGTH           (64U)

#define MAX_INT      (32767) /* max value of 16 bits for int */
#define MAX_LONG     (0XFFFFFFFFU) /* max value of 32 bits for long */
#define MIN_INT      (-32768)           /* min value of 16bits for int */

/* bytes dummy */
#define DUMMY1(n)   char_t cDummy1Byte_##n
#define DUMMY2(n)   char_t cDummy2Byte_##n[2]
#define DUMMY3(n)   char_t cDummy3Byte_##n[3]

#define MIN_MODBUS_BLOCK_ID                (1U)
#define MAX_MODBUS_BLOCK_ID                (64U)

#define MODBUS_STATE_NOCONFIG              (0U)
#define MODBUS_STATE_START                 (1U)
#define MODBUS_STATE_STOP                  (2U)

#define MIN_P2P_BLOCK_ID                (1U)
#define MAX_P2P_BLOCK_ID                (128U)

#define MIN_P2P_STATION_ID              (1U)
#define MAX_P2P_STATION_ID              (64U)

#define P2P_STATE_NOCONFIG              (0U)
#define P2P_STATE_START                 (1U)
#define P2P_STATE_STOP                  (2U)

#define RT_DATA_AREA_SIZE                   (sizeof(PRG_RTDataArea_t))
#define DISABLED                            (1)
#define UNDISABLED                            (0)

#define DAY_TO_SECONDS       (3600*24)
#define DAY_TO_MILLISECONDS       (3600*24*1000)

/* seconds per day */
#define SPD 24*60*60

#define TASK_ID_UP1 1
#define TASK_ID_UP2 2

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

typedef double_t (*fp)(double_t x);

typedef struct ExtLibMathTableTag
{
    char_t cName[MAX_FUNC_NAME_LENGTH];
    fp pFunc;
}ExtLibMathTable;



typedef struct LimittypeTag
{
    double_t dMin;
    double_t dIn;
    double_t dMax;
    double_t dOut;
}Limittype_t;

typedef struct LimitMaxtypeTag
{
    double_t dIn;
    double_t dMax;
    double_t dOut;
}LimitMaxtype_t;

typedef struct LimitMintypeTag
{
    double_t dIn;
    double_t dMin;
    double_t dOut;
}LimitMintype_t;

typedef struct RStypeTag
{
    char_t SET;
    char_t RESET1;
    char_t Q1;
    DUMMY1(0);
}RStype_t;

/* SR lib FUNCTION */
typedef struct SRtypeTag
{
    char_t SET1;
    char_t RESET;
    char_t Q1;
    DUMMY1(0);
}SRtype_t;

/* CTD LIB FUNCTION */
typedef struct CTDtypeTag
{
    char_t CD;
    char_t LOAD;
    int16_t PV;
    char_t Q;
    int16_t CV;
}CTDtype_t;

/* CTU LIB FUNCTION */
typedef struct CTUtypeTag
{
    char_t CU;
    char_t RESET;
    int16_t PV;
    char_t Q;
    int16_t CV;
}CTUtype_t;


/* CTUD LIB FUNCTION */
typedef struct CTUDtypeTag
{
    char_t CU;      /* falling edge detect */
    char_t CD;      /* rising edge detect */
    char_t RESET;   /* RESET =1, CV=0 */
    char_t LOAD;    /* LOAD =1, CV=PV */
    int16_t PV;    /* the set value */
    char_t QU;      /* output */
    char_t QD;      /* output */
    int16_t CV;    /* count output */
}CTUDtype_t;

/* RTC */
typedef struct RTCtypeTag
{
    bool_t IN;
    uint32_t PresetDateTime;
    bool_t Q;
    uint32_t CurrentDateTime;
}RTCtype_t;

/* TOF */
typedef struct TOFtypeTag
{
    bool_t IN;
    uint32_t PT;
    bool_t Q;
    uint32_t ET;
    uint32_t ST;
    bool_t M;
}TOFtype_t;
/* TOF_R */
typedef struct TOFRtypeTag
{
    bool_t IN;
    float_t PT;
    bool_t Q;
    float_t ET;
    uint64_t ST;
    bool_t M;
}TOFRtype_t;

/* TON */
typedef struct TONtypeTag
{
    bool_t IN;
    uint32_t PT;
    bool_t Q;
    uint32_t ET;
    uint32_t ST;
    bool_t M;
}TONtype_t;
/* TON_R */
typedef struct TONRtypeTag
{
    bool_t IN;
    float_t PT;
    bool_t Q;
    float_t ET;
    uint64_t ST;
    bool_t M;
}TONRtype_t;

/* TP */
typedef struct TPtypeTag
{
    bool_t IN;
    uint32_t PT;
    bool_t Q;
    uint32_t ET;
    uint32_t ST;
}TPtype_t;

/* TP_R */
typedef struct TPRtypeTag
{
    bool_t IN;
    float_t PT;
    bool_t Q;
    float_t ET;
    uint64_t ST;
}TPRtype_t;

/* TRIG */
typedef struct TRIGtypeTag
{
    bool_t CLK;
    bool_t Q;
    bool_t M;
}TRIGtype_t;

/* BLINK */
typedef struct BLINKtypeTag
{
    bool_t IN;
    bool_t RESET;
    uint32_t uiTon;
    uint32_t uiToff;
    bool_t Q;
    uint64_t ulST;
}BLINKtype_t;

typedef struct BLINKRtypeTag
{
    bool_t IN;
    bool_t RESET;
    float_t fTon;
    float_t fToff;
    bool_t Q;
    uint64_t ulST;
}BLINKRtype_t;

typedef struct TOGGLEtypeTag
{
    bool_t IN;
    bool_t RESET;
    bool_t Q;
}TOGGLEtype_t;

/* time set */
typedef struct TimeSettypeTag
{
    bool_t EN;
    int32_t GMT;
    int32_t MS;
    bool_t Q;
}TimeSettype_t;

/* time adjust */
typedef struct TimeAdjtypeTag
{
    bool_t EN;
    int32_t MS;
    bool_t Q;
}TimeAdjtype_t;

/* Modbus master start */
typedef struct MBMastertypeTag
{
    bool_t CI;
    int32_t uiBlockIndex;
    bool_t CO;
    int32_t iStat;
}MBMastertype_t;

/* SOE 相关结构体 */
typedef struct SOEtypeTag
{
    bool_t CI;
    int32_t uiBlockIndex;
    bool_t CO;
    int32_t iStat; /* 0:SOE 相关结构体 */
}SOEtype_t;

/* P2P 相关结构体 */
typedef struct P2PtypeTag
{
    bool_t CI;
    uint32_t uiBlockIndex;
    bool_t CO;
    int32_t iStat;
}P2Ptype_t;

/* P2P 发送状态相关结构体 */
typedef struct P2PSendtypeTag
{
    bool_t En;
    uint32_t Node;
    bool_t Q;
    bool_t bTolFlag;       /* Tolerance Flag     容忍标志位 */
    bool_t bErrFlag;       /* Error Flag         错误标志位 */
    bool_t bSendTimeout;   /* Send Timeout Flag  发送请求帧超时标志位 */
}P2PSendtype_t;

/* P2P 接收状态相关结构体 */
typedef struct P2PRecvtypeTag
{
    bool_t En;
    uint32_t Node;
    bool_t Q;
    bool_t bTolFlag;       /* Tolerance Flag  容忍标志位 */
    bool_t bErrFlag;       /* Error Flag      错误标志位 */
}P2PRecvtype_t;

/* 系统时间相关结构体 */
typedef struct TimetypeTag
{
    bool_t EN;
    bool_t Q;
    bool_t M;
}Timetype_t;

typedef struct ChkErrtypeTag
{
    bool_t CI;
    bool_t CO;
    bool_t ERR;
    bool_t DIVBYZERO;
    bool_t OVERFLOW;
    bool_t UNDERFLOW;
    bool_t BADPARAM;
    bool_t BADCONV;
    bool_t STRLEN;
    bool_t RANGE;
    bool_t ARRAYERR;
    bool_t INTERNALERR;
    bool_t OTHER;
}ChkErrtype_t;

typedef struct ClrErrtypeTag
{
    bool_t CI;
    bool_t CO;
}ClrErrtype_t;

#if 0
typedef struct CONCAT_DATE_TODtypeTag
{
    uint32_t uiDate; //date
    uint32_t uiTime; //time
    uint32_t uiDT; //date time
}CONCAT_DATE_TODtype_t;

typedef struct CONCAT_DATEtypeTag
{
    uint16_t usYear;
    uint16_t usMonth;
    uint16_t usDay;
    uint32_t uiDate;
}CONCAT_DATEtype_t;

typedef struct CONCAT_TODtypeTag
{
    uint16_t usHour;
    uint16_t usMinute;
    uint16_t usSecond;
    uint16_t usMillisecond;
    uint32_t uiTOD;
}CONCAT_TODtype_t;

typedef struct CONCAT_DTtypeTag
{
    uint16_t usYear; //1970+130年
    uint16_t usMonth;
    uint16_t usDay;
    uint16_t usHour;
    uint16_t usMinute;
    uint16_t usSecond;
    uint16_t usMillisecond;
    uint32_t uiDT;
}CONCAT_DTtype_t;

typedef struct SPLIT_DATEtypeTag
{
    uint32_t uiDate;
    uint16_t usYear;
    uint16_t usMonth;
    uint16_t usDay;
}SPLIT_DATEtype_t;

typedef struct SPLIT_TODtypeTag
{
    uint32_t uiTOD;
    uint16_t usHour;
    uint16_t usMinute;
    uint16_t usSecond;
    uint16_t usMillisecond;
}SPLIT_TODtype_t;

typedef struct SPLIT_DTtypeTag
{
    uint32_t uiDT;
    uint16_t usYear; //1970+130年
    uint16_t usMonth;
    uint16_t usDay;
    uint16_t usHour;
    uint16_t usMinute;
    uint16_t usSecond;
    uint16_t usMillisecond;
}SPLIT_DTtype_t;

typedef struct DAY_OF_WEEKtypeTag
{
    uint32_t uiDate;
    uint16_t usDayOfWeek;
}DAY_OF_WEEKtype_t;

typedef struct ADD_DATE_DAYtypeTag
{
    uint32_t uiDate;//内部表示为秒
    uint32_t uiDay;
    uint32_t uiNewDate;
}ADD_DATE_DAYtype_t;

typedef struct SUB_DATE_DAYtypeTag
{
    uint32_t uiDate;
    uint32_t uiDay;
    uint32_t uiNewDate;
}SUB_DATE_DAYtype_t;
#endif
typedef struct UNPACK16typeTag
{
    bool_t bCI;
    uint32_t uiInput;
    bool_t bCO;
    bool_t bNZ;
    bool_t Q01;
    bool_t Q02;
    bool_t Q03;
    bool_t Q04;
    bool_t Q05;
    bool_t Q06;
    bool_t Q07;
    bool_t Q08;
    bool_t Q09;
    bool_t Q10;
    bool_t Q11;
    bool_t Q12;
    bool_t Q13;
    bool_t Q14;
    bool_t Q15;
    bool_t Q16;
}UNPACK16type_t;

typedef struct UNPACK32typeTag
{
    bool_t bCI;
    uint32_t uiInput;
    bool_t bCO;
    bool_t bNZ;
    bool_t Q01;
    bool_t Q02;
    bool_t Q03;
    bool_t Q04;
    bool_t Q05;
    bool_t Q06;
    bool_t Q07;
    bool_t Q08;
    bool_t Q09;
    bool_t Q10;
    bool_t Q11;
    bool_t Q12;
    bool_t Q13;
    bool_t Q14;
    bool_t Q15;
    bool_t Q16;
    bool_t Q17;
    bool_t Q18;
    bool_t Q19;
    bool_t Q20;
    bool_t Q21;
    bool_t Q22;
    bool_t Q23;
    bool_t Q24;
    bool_t Q25;
    bool_t Q26;
    bool_t Q27;
    bool_t Q28;
    bool_t Q29;
    bool_t Q30;
    bool_t Q31;
    bool_t Q32;
}UNPACK32type_t;

typedef struct MATH_LRtypeTag
{
    double_t dIN;
    double_t dOUT;
    bool_t bERR;
}MATH_LRtype_t;

typedef struct EXPT_LRtypeTag
{
    double_t Y;
    double_t n;
    double_t dOUT;
    uint8_t ucERR;
}EXPT_LRtype_t;

typedef struct MATH_RtypeTag
{
    float_t dIN;
    float_t dOUT;
    bool_t bERR;
}MATH_Rtype_t;

typedef struct EXPT_RtypeTag
{
    float_t Y;
    float_t n;
    float_t dOUT;
    uint8_t ucERR;
}EXPT_Rtype_t;

//安全除法指令
typedef struct DIV_DWtypeTag
{
    uint32_t uiIN1;
    uint32_t uiIN2;
    uint32_t uiOUT;
    bool_t bERR;
}DIV_DWtype_t;

typedef struct DIV_DtypeTag
{
    int32_t iIN1;
    int32_t iIN2;
    int32_t iOUT;
    bool_t bERR;
}DIV_Dtype_t;

typedef struct DIV_RtypeTag
{
    float_t fIN1;
    float_t fIN2;
    float_t fOUT;
    bool_t bERR;
}DIV_Rtype_t;

typedef struct DIV_LRtypeTag
{
    double_t dIN1;
    double_t dIN2;
    double_t dOUT;
    bool_t bERR;
}DIV_LRtype_t;

typedef struct TIMERtypeTag
{
    bool_t bIN;
    bool_t bRESET;
    uint32_t uiPT;
    uint32_t uiIT;
    bool_t bQ;
    uint32_t uiET;
    uint64_t uiStart;
}TIMERtype_t;

typedef struct TIMER_RtypeTag
{
    bool_t bIN;
    bool_t bRESET;
    float_t fPT;
    float_t fIT;
    bool_t bQ;
    float_t fET;
    uint64_t uiStart;
}TIMER_Rtype_t;

typedef struct SCANSTATUStypeTag
{
    bool_t CI;
    int32_t iTaskNo;
    bool_t CO;
    bool_t POWERUP;
    bool_t bFirstScan;
    int32_t iScanRequest;
    int32_t iScanSurplus;
    int32_t iScanDetra;
    uint32_t uiDeltaT;
    int32_t iScanOverrun;
    int32_t iKeySwitch;
}SCANSTATUStype_t;

typedef struct CALENDARtypeTag
{
    bool_t CI;
    bool_t CO;
    int32_t iYear;
    int32_t iMonth;
    int32_t iDay;
    int32_t iHour;
    int32_t iMinute;
    int32_t iSecond;
    int32_t iMillisec;
    int32_t iWeekday;
    int32_t iRelsec;
}CALENDARtype_t;

typedef struct TMtypeTag
{
  int tm_sec;           /* Seconds. [0-60] (1 leap second) */
  int tm_min;           /* Minutes. [0-59] */
  int tm_hour;          /* Hours.   [0-23] */
  int tm_mday;          /* Day.     [1-31] */
  int tm_mon;           /* Month.   [0-11] */
  int tm_year;          /* Year - 1900. */
  int tm_wday;          /* Day of week. [0-6] */
  int tm_yday;          /* Days in year.[0-365] */
  int tm_isdst;         /* DST.     [-1/0/1]*/  /*可以忽略*/
}TMtype_t;

typedef struct OSPStatustypeTag
{
    bool_t CI;
    uint8_t ucSlaveNo;
    uint8_t ucPMID;
    bool_t CO;
    uint32_t uiOSPFunction;
    float_t fPRPM;
    float_t fPAccel;
    uint32_t uiAlarmPack;
    uint8_t ucDigPack;
}OSPStatustype_t;

/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
/*
***********************************************************************************************************************
* 函数名称: ExtLibInit
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
void ExtLibInit(void);

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
void ExtAppendDataInit(void);

#endif /* EXT_LIB_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

