/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    : 系统资源管理模块
*
*  文件名称    : DataType.h
*
*  功能简介    : 数据类型定义
*
*  作者       : 李琦
*
*  创建日期    : 2015-12-30
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef DATATYPE_H_
#define DATATYPE_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#ifndef NULL
#define NULL                         (0x00000000U)          /* NULL = 0 */
#endif /* #ifndef NULL */

#ifndef true
#define true                         (1)
#endif /* #ifndef TRUE */

#ifndef false
#define false                        (0)
#endif /* #ifndef FALSE */

typedef char                         char_t;                /* define char_t     (0~255) */
typedef signed char                  int8_t;                /* define int8_t     (-128~127) */
typedef unsigned char                uint8_t;               /* define uint8_t    (0~255) */
typedef signed short                 int16_t;               /* define int16_t    (-32768~32767) */
typedef unsigned short               uint16_t;              /* define uint16_t   (0~65535) */
typedef signed int                   int32_t;               /* define int32_t    (-2147483648~2147483647) */
typedef unsigned int                 uint32_t;              /* define uint32_t   (0~4294967295) */
typedef float                        float_t;               /* define float_t */
typedef double                       double_t;              /* define double_t */
typedef unsigned char                bool_t;                /* define bool_t      (0,1) */
typedef long long                    ulong64_t;             /* define long64_t */
typedef long long                    long64_t;              /* define long64_t */

/* pointer types definition */
typedef char*                        pchar_t;               /* define pchar_t */
typedef signed char*                 pint8_t;               /* define pint8_t */
typedef unsigned char*               puint8_t;              /* define puint8_t */
typedef signed short*                pint16_t;              /* define pint16_t */
typedef unsigned short*              puint16_t;             /* define puint16_t */
typedef signed int*                  pint32_t;              /* define pint32_t */
typedef unsigned int*                puint32_t;             /* define puint32_t */
typedef float*                       pfloat_t;              /* define pfloat_t */
typedef double*                      pdouble_t;             /* define pdouble_t */
typedef void*                        pvoid_t;               /* define pvoid_t */

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/



/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/


#endif /* DATATYPE_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

