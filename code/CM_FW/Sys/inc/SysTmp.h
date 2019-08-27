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
*  文件名称    : SysTmp.c
*
*  功能简介    :
*
*  作者       : 李琦
*
*  创建日期    : 2016-7-7
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
#ifndef SYS_TMP_H_
#define SYS_TMP_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "DataType.h"
#include "Hardware.h"
#include "CMconfig.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/
#define MAX_MODBUS_RESP_MSG_LEN    256

/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/
/* 消息*/
typedef struct SysModbusRespMsgTag
{
    uint16_t usLen;                             /* 消息内容长度 */
    uint8_t ucContent[MAX_MODBUS_RESP_MSG_LEN];      /* 存放消息内容 */
} SysModbusRespMsg_t;



/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/
bool_t SysGetLocalProjVersion( ProjVerInfo_t *pstProjVer );
 
bool_t SysGetPMProjVersion( PMController_t emPMID, ProjVerInfo_t *pstProjVer );
 
bool_t SysGetDownloadProjVersion( PMController_t emPMID, ProjVerInfo_t *pstProjVer );
 
/*
***********************************************************************************************************************
* 函数名称: SysGetPMDownloadStatus
*
* 功能描述: 获取PM系统状态中的下装状态
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值  : 下装状态
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t SysGetPMDownloadStatus( void );
 
/*
***********************************************************************************************************************
* 函数名称: SysGetPMPrjVerInfo
*
* 功能描述: 获取PM系统状态中的工程版本
*
* 输入参数: 工程版本信息缓冲区
*
* 输出参数: 工程版本信息
*
* 返 回 值  : 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SysGetPMPrjVerInfo( ProjVerInfo_t *pstPMPrjVerInfo );
 
 
 
/* Modbus */
/* Network��Modbus TCP */
bool_t SysGetModbusTCPConfigInfo( NetworkPortType_t emType, ModbusTCPConfigInfo_t *pstModbusTCPConfig );
 
 
/* Modbus Master Info. */
bool_t SysGetModbusMasterInfoVersion( puint32_t puiCRC, puint32_t puiSubVern );
uint32_t SysGetModbusMasterInfo( puint8_t pucBuff, uint32_t uiOffset, uint32_t uiLength );
 
 
 
/* Modbus Slave Info. */
bool_t SysGetModbusSlaveConfigVersion( puint32_t puiCRC, puint32_t puiSubVern);
uint32_t SysGetModbusSlaveConfigInfo( puint8_t pucBuff, uint32_t uiOffset, uint32_t uiLength );
 
 
/* Control Station */
bool_t SysGetCtrlStationConfigVersion( puint32_t puiCRC, puint32_t puiSubVern );
bool_t SysGetTaskConfigInfo( Task_ID_t emTask, TaskConfigInfo_t *pstTaskCfgInfo );
 
 
ActiveFlag_t SysGetCMConfigInfoActiveFlag(void);
 
 
/* Modbus TCP Master */
/* Get/Set Slave Response: Max 256 Bytes */
bool_t SysGetModbusRespMsg( NetworkPortType_t emType, puint8_t pucContent, puint16_t pusLen );
bool_t SysSetModbusRespMsg( NetworkPortType_t emType, puint8_t pucContent, uint16_t usLen );
bool_t SysClearModbusRespMsg( NetworkPortType_t emType );
 
 
/* Modbus Master Block Start Flag */
bool_t SysGetModbusMasterBlkStartFlag( uint32_t uiBlockID );
 
 
 
/*
***********************************************************************************************************************
* 函数名称: SysGetCfgFileType
*
* 功能描述: 根据文件处理序号获取文件的ID
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
SysCfgFileType_t SysGetCfgFileType(SysFileProcessSeq_t emFileProcessSeq);



#endif /* SYS_TMP_H_ */
/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


