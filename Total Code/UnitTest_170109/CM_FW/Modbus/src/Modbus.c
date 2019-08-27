/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : Modbus模块
*
*  文件名称    : Modbus.c
*
*  功能简介    : 收发Modbus数据
*
*  作者        : 王东
*
*  创建日期    : 2016-02-25
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>    /* File control definitions */
#include <errno.h>
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/types.h>
#include <sys/stat.h>
#include "../inc/Modbus.h"
#include "CommonLib/inc/Algrithm.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

static bool_t s_bCMHasCfg;                              /* 本CM是否有配置 */
static uint8_t s_ucLocalCMSlotID;                       /* 本CM模块的槽位号 */
static uint8_t s_ucMBTCPMstrIndex;                      /* 网口配置为Modbus从站时，目前查询哪个Modbus TCP主站的请求帧 */
static PortMstrInfo_t s_stPortMstrInfo[MAX_PORT_NUM];   /* 端口配置为Modbus主站时, 各个端口的相关信息 */
static uint8_t s_ucDataBuff[MAX_MBMSG_LEN];             /* 数据缓冲区 */
static uint8_t s_ucPMDataBuff[MAX_PM_MODBUS_DATA_LEN];  /* PM数据缓冲区 */
static uint8_t s_ucRespBuff[MAX_MBMSG_LEN];             /* 应答数据缓冲区 */
static MBTCPMsg_t s_stMBTCPMsg;                         /* Modbus TCP 消息 */
static ModbusMsg_t s_stMBSerlMsg;                       /* Modbus 串口消息 */
static PMController_t s_emLastPMID;                     /* 读 Modbus变量在线值时，记录最近一次使用的PM的编号 */

/* Configuration */
static SerlCfgInfo_t s_stSerlCfgInfo[NUM_OF_SERIAL_PORT];  /* Serial Configuration Info.        串口配置参数 */
static MBTCPCfgInfo_t s_stMBTCPInfo[NUM_OF_NETWORK_PORT];  /* Modbus TCP Info.                  Modbus TCP配置参数信息 */
static MBSlaveCfgInfo_t s_stMBSlaveCfgInfo;                /* Modbus Slave Configuration Info.  Modbus从站配置信息 */
static ModbusMstrCfgInfo_t s_stMBMstrCfgInfo;              /* Modbus Master Info.               Modbus主站信息 */

static uint8_t ucModbusOnlineCnt[PORT_NUM];                     /* modbus从站在线计数 */

/* Configuration Copy */
static uint32_t s_uiMBSlaveCfgOffset;  /* Modbus Slave Info. Offset */
static uint32_t s_uiMBMstrCfgOffset;   /* Modbus Master Info. Offset */

/* Default Serial Setting */
static uint8_t const s_ucDefaultTP = (uint8_t)(MB_TRANS_PROTO_RTU);
static uint8_t const s_ucDefaultBR = (uint8_t)(BR9600);
static uint8_t const s_ucDefaultTM = (uint8_t)(TM_RS485);
static uint8_t const s_ucDefaultParity = (uint8_t)(PARITY_ODD);
static bool_t const s_bDefaultEnable = false;

// CRC High Byte
const uint8_t s_ucAuchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

// CRC Low Byte
const uint8_t s_ucAuchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

/* Cycle */
static void HandleConfiguration( void );
static void HandleModbusTCP( void );
static void HandleModbusSerial( void );

/* Serial Port: Modbus */
static void ClearSerlPortRecvBuff( SerialPortType_t emSerlPortID );
static void ConfigSerlPort( SerialPortType_t emSerlPortID, SerlPortSets_t const *pstSerlSets );
static void SetSerlPort( SerialPortType_t emSerlPortID );
static bool_t SerialSend( SerialPortType_t emSerlPortID, uint8_t const pucData[], uint16_t usDataLen );
static bool_t SerialRecv( SerialPortType_t emSerlPortID, uint8_t pucBuff[], uint16_t *pusLen );

/* Initialize */
static void InitLocalConfigInfo( void );
static void InitSerlCfgInfo( SerlCfgInfo_t *pstSerlCfgInfo );
static void InitModbusTCPInfo( MBTCPCfgInfo_t *pstMBTCPInfo );
static void InitModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo );
static void InitPortMstrInfo( PortMstrInfo_t *pstPortMstrInfo );

/* Select PM Module */
static SelPMID_t SelectPM( void );

/******************** Configuration ********************/
static void ClearLocalInfo( void );
static void ClearLocalConfigInfo( void );
static void ClearMBSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo );


/**** Modbus Slave Configuration ****/
static bool_t HandleModbusSlaveCfgInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo, puint32_t puiOffset );
static bool_t GetModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo, puint32_t puiOffset, bool_t *pbComplete );
static void SwitchModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo );
static void ClearCurModbusSlaveCfg( MBSlaveCfgInfo_t *pstMBSlaveInfo );
static bool_t CheckCurModbusSlaveInfo( MBSlaveCfgInfo_t const *pstMBSlaveInfo );
static bool_t CheckModbusSlaveBlk( ModbusSlaveBlock_t const *pstBlk );
static bool_t CheckModbusSlaveBlkHead( ModbusSlaveBlockHead_t const *pstBlkHead );

/* Get... */
static ModbusSlaveInfo_t const* GetCurModbusSlaveCfg( MBSlaveCfgInfo_t const *pstMBSlaveInfo );
#if 0
static uint32_t GetModbusSlaveBlkNum( MBSlaveCfgInfo_t const *pstMBSlaveInfo );
#endif
static ModbusSlaveBlock_t const* GetModbusSlaveBlk( MBSlaveCfgInfo_t const *pstMBSlaveInfo, uint8_t ucModbusRgn );

/**** Modbus Master Configuration ****/
static void UpdatePortMstrInfo( PortMstrInfo_t *pstPortMstrInfo );

/* Get... */
static ModbusMasterPort_t const* GetPortMBMstrCfg( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex );
static bool_t GetPortMBMstrBlkID( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex,
                                  uint32_t uiBlkIndex, uint8_t *pucBlkID );
static bool_t GetPortMBMstrUpdate( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex );
static uint32_t GetPortMBMstrBlkCnt( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex );
#if 0
static ModbusMasterBlock_t const* GetPortMBMstrBlk( ModbusMstrCfgInfo_t const *pstModbusMstrInfo,
                                                    uint32_t uiPortIndex, uint32_t uiBlkIndex );
#endif
static ModbusMasterBlock_t const* GetPortMBMstrBlkInfo( ModbusMstrCfgInfo_t const *pstModbusMstrInfo,
                                                        uint32_t uiPortIndex, uint32_t uiBlkIndex, uint8_t *pucBlkID );

/**** Port Configuration：Serial/Network ****/
static void HandlePortConfig( CommPort_t emPort );
static uint8_t GetPortFPOrder( CommPort_t emPort );
static bool_t GetPortWriteEnable( CommPort_t emPort );
static uint8_t GetPortMBSlaveID( CommPort_t emPort );

/* Network: Modbus TCP */
static bool_t HandleModbusTCPConfig( NetworkPortType_t emNet, MBTCPCfgInfo_t *pstMBTCPInfo );
static bool_t CheckModbusTCPCfgInfo( ModbusTCPConfigInfo_t const *pstMBTCPCfgInfo );
static bool_t SwitchModbusTCPCfgInfo( MBTCPCfgInfo_t *pstMBTCPInfo, ModbusTCPConfigInfo_t const *pstMBTCPCfgInfo );
static ModbusTCPConfigInfo_t const* GetCurMBTCPCfg( MBTCPCfgInfo_t const *pstMBTCPInfo );
static uint8_t GetNetFPOrder( MBTCPCfgInfo_t const *pstMBTCPInfo );
static bool_t GetNetPortWriteEnable( MBTCPCfgInfo_t const *pstMBTCPInfo );

/* Serial: COM */
static bool_t HandleCOMConfig( SerialPortType_t emSerl, SerlCfgInfo_t *pstCOMInfo );
static bool_t CheckCOMCfgInfo( COMConfigInfo_t const *pstCOMCfgInfo, SerialPortType_t emSerl );
static bool_t SwitchCOMCfgInfo( SerlCfgInfo_t *pstCOMInfo, COMConfigInfo_t const *pstCOMCfgInfo );
static void HandleCOMCfgChg( SerialPortType_t emSerl, SerlCfgInfo_t *pstCOMInfo );
static COMConfigInfo_t const* GetCurCOMCfg( SerlCfgInfo_t const *pstCOMInfo );
static uint8_t GetCOMFPOrder( SerlCfgInfo_t const *pstCOMInfo );
static bool_t GetCOMPortWriteEnable( SerlCfgInfo_t const *pstCOMInfo );
static uint8_t GetCOMMBSlaveID( SerlCfgInfo_t const *pstCOMInfo );

/******************** Handle Modbus Master/Slave ********************/
static void SendMBDataToPM( uint8_t const pucPMData[], uint16_t usPMDataLen );

/**** Common - Master ****/
/* Data */
static uint8_t const* GetModbusVarAddr( PMController_t emSelPM, ModbusVar_t const *pstMBVar );
static bool_t GetMBBOOLValue( PMController_t emSelPM, ModbusVar_t const *pstMBVar, bool_t *pbValue );
static uint16_t AddCoilsReqData( PMController_t emSelPM, ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t pucReqData[]);
static uint16_t AddRegsReqData( PMController_t emSelPM, ModbusMasterBlock_t const *pstMBMstrBlk,
                                uint8_t pucReqData[], uint8_t ucFPOrder );

static uint8_t GetModbusFuncCode( ModbusMasterBlockHead_t const *pstBlkHead );
static uint16_t GetModbusReqDataLen( ModbusMasterBlockHead_t const *pstBlkHead, uint8_t ucFuncCode );
static uint16_t AddReadReqData( ModbusMasterBlockHead_t const *pstBlkHead, uint8_t pucReqData[]);
static uint16_t AddWriteReqData( ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucFuncCode,
                                 uint8_t pucReqData[], uint8_t ucFPOrder );

/* Ckeck Reponse Message */
static bool_t CkeckMBRespFuncCode( uint8_t const pucRespFunc[], uint8_t ucReqFuncCode );
static bool_t CheckMBRespData( uint8_t ucFuncCode, uint16_t usRespDataLen,
                               uint8_t const pucRespData[], uint8_t const pucReqData[]);

/* Make PM Frame */
static void MstrMakePMMBMsg( CommPort_t emPort, uint8_t ucFPOrder, RTDataVerInfo_t const *pstRTDataVer,
                             ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucBlkID,
                             uint8_t const pucMBData[], uint16_t usMBDataLen,
                             uint8_t pucPMMBMsg[], puint16_t pusPMMBMsgLen );

static void SlaveMakePMMBMsg( CommPort_t emPort, uint8_t ucFPOrder, uint8_t const pucReqFunc[], uint16_t usReqDataLen,
                              uint8_t pucPMMBMsg[], puint16_t pusPMMBMsgLen );

/**** Common - Slave ****/
static bool_t CheckReqFuncCode( uint8_t ucFuncCode );
static void AddBOOLRespData( ModbusSlaveBlock_t const *pstSlaveBlk, uint16_t usStartAddr,
                             uint16_t usNumber, uint8_t pucRespData[]);
static void AddRegsRespData( ModbusSlaveBlock_t const *pstSlaveBlk, uint8_t ucFPOrder,
                             uint16_t usStartAddr, uint16_t usNumber, uint8_t pucRespData[]);
static uint16_t AddMBRespData( bool_t bSerlPort, uint8_t ucFuncCode, uint8_t ucFPOrder,
                               uint8_t const pucReqData[], uint8_t pucRespData[]);

/* Network: Modbus Master */
static void HandleModbusTCPMaster( CommPort_t emPort );
static void HandleMBTCPMstrIdle( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo );
static void HandleMBTCPMstrBusy( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo );
static bool_t MakeModbusTCPReq( PortMstrInfo_t *pstPortMstrInfo, ModbusMasterBlock_t const *pstMBMstrBlk,
                                ModbusMsg_t *pstMBReqMsg, uint8_t ucFPOrder );
static bool_t SendModbusTCPReq( CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk,
                                ModbusMsg_t const *pstMBReqMsg );
static void HandleModbusTCPResp( CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucBlkID,
                                 PortMstrInfo_t *pstPortMstrInfo, uint8_t const pucResp[], uint16_t usRespLen );
static bool_t CheckMBTCPResp( uint8_t const pucReq[], uint8_t const pucResp[], uint16_t usRespLen );

/* Network: Modbus Slave */
static void HandleModbusTCPSlave( void );
static void DecodeModbusTCPReq( CommPort_t emPort, uint8_t const pucReq[], uint16_t usReqLen,
                                uint8_t pucResp[], puint16_t pusRespLen );
static uint16_t MakeMBTCPExRespMsg( uint8_t pucResp[], uint8_t ucReqFuncCode, ModbusException_t emExcCode );
static uint16_t MakeMBTCPRespMsg( uint8_t ucFPOrder, uint8_t const pucReq[], uint8_t pucResp[]);
static void SendModbusTCPResp( NetMsgHead_t const *pstHead, uint8_t ucMBTCPMstrIndex,
                               uint8_t const *pucResp, uint8_t usRespLen );

/* Serial Common */
static uint8_t CalcLRC( uint8_t const pucData[], uint16_t usDataLen );
static uint16_t CalcCRC( uint8_t const pucData[], uint16_t usDataLen );
static bool_t CheckModbusRTUMsg( uint8_t ucSlaveID, uint8_t const pucMsg[], uint16_t usMsgLen );
static bool_t CheckModbusASCIIMsg( uint8_t ucSlaveID, uint8_t const pucASCIIMsg[], uint16_t usASCIIMsgLen );
static bool_t RecvModbusSerlMsg( CommPort_t emPort, uint8_t pucBuff[], puint16_t pusLen );
static bool_t SendModbusSerlMsg( CommPort_t emPort, uint8_t const pucData[], uint16_t usDataLen );

/* Serial: Modbus Master */
static void HandleModbusSerlMaster( CommPort_t emPort, bool_t bRTUProt );
static void HandleMBSerlMstrIdle( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo, bool_t bRTUProt );
static void HandleMBSerlMstrBusy( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo, bool_t bRTUProt );
static bool_t MakeModbusSerlReq( bool_t bRTUProt, uint8_t ucFPOrder, PortMstrInfo_t *pstPortMstrInfo,
                                 ModbusMasterBlock_t const *pstMBMstrBlk, ModbusMsg_t *pstMBReqMsg );
static void HandleModbusSerlResp( bool_t bRTUProt, CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk,
                                  uint8_t ucBlkID, PortMstrInfo_t *pstPortMstrInfo,
                                  uint8_t const pucResp[], uint16_t usRespLen );
static bool_t CheckMBSerlResp( bool_t bRTUProt, uint8_t const pucReq[], uint8_t const pucResp[], uint16_t usRespLen );
static void HandleModbusSlaveTimeout( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo,
                                      ModbusMstrCfgInfo_t const *pstMBMstrInfo );

/* Serial: Modbus Slave */
static void HandleModbusSerlSlave( CommPort_t emPort, bool_t bRTUProt );
static void DecodeModbusSerlReq( uint8_t ucSlaveID, bool_t bRTUProt, CommPort_t emPort,
                                 uint8_t const pucReq[], uint16_t usReqLen,
                                 uint8_t pucResp[], puint16_t pusRespLen );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: ModbusInit
*
* 功能描述: 模块初始化函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void ModbusInit( void )
{
    DEBUG_MB( "ModbusInit\n" );

    uint8_t ucIndex = 0U;
    SerialPortType_t emSerlID = COM1;

    s_bCMHasCfg = false;
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();  /* CM Slot ID */
    s_ucMBTCPMstrIndex = 0U;
    s_emLastPMID = INVALID_PM_ID;

    /* Port */
    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_PORT_NUM); ucIndex++ )
    {
        InitPortMstrInfo( &s_stPortMstrInfo[ucIndex]);
    }

    /* Buffer */
    memset( s_ucDataBuff, 0, sizeof(s_ucDataBuff));
    memset( s_ucPMDataBuff, 0, sizeof(s_ucPMDataBuff));
    memset( s_ucRespBuff, 0, sizeof(s_ucRespBuff));

    /* Modbus TCP Message */
    memset( &s_stMBTCPMsg, 0, sizeof(MBTCPMsg_t));
    s_stMBTCPMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Modbus Serial Message */
    memset( &s_stMBSerlMsg, 0, sizeof(ModbusMsg_t));

    /* Local Configuration Info. */
    InitLocalConfigInfo();

    /* Set Serial Port */
    for( emSerlID = COM1; emSerlID < NUM_OF_SERIAL_PORT; emSerlID++ )
    {
        SetSerlPort( emSerlID );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ModbusCycle
*
* 功能描述: 模块周期运行函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void ModbusCycle( void )
{
    static bool_t s_bClrDone = false;

    /* Check If CM Has Configuration */
    s_bCMHasCfg = IsCMHasCfg();

    /* Update CM Slot ID */
    s_ucLocalCMSlotID = (uint8_t)SysGetLocalCMSlot();

    /* Handle Local Configuration */
    if( s_bCMHasCfg )
    {
        s_bClrDone = false;
        HandleConfiguration();

        /* Handle Modbus TCP */
        HandleModbusTCP();

        /* Handle Modbus Serial */
        HandleModbusSerial();
    }
    else
    {
        if( !s_bClrDone )
        {
            ClearLocalInfo();
            s_bClrDone = true;
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
* 函数名称: ClearSerlPortRecvBuff
*
* 功能描述: 清除串口发送/接收缓冲区
*
* 输入参数: emSerlPortID：串口ID
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearSerlPortRecvBuff( SerialPortType_t emSerlPortID )
{
    if( emSerlPortID < NUM_OF_SERIAL_PORT )
    {
        ResetMBPortRecvStatus( emSerlPortID );
    }
}


/*
***********************************************************************************************************************
* 函数名称: ConfigSerlPort
*
* 功能描述: 配置串口
*
* 输入参数: emSerlPortID：串口ID；
*          pstSerlSets：串口配置参数。
*
* 输出参数: 无
*
* 返 回 值: 是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ConfigSerlPort( SerialPortType_t emSerlPortID, SerlPortSets_t const *pstSerlSets )
{
    if((emSerlPortID < NUM_OF_SERIAL_PORT) &&
       (pstSerlSets != NULL))
    {
        DEBUG_MB( "COM%d: EN %d\n", emSerlPortID+1, pstSerlSets->bEnable );

        /* Port Disable */
        SetMBPortEnable( emSerlPortID, false );

        /* Config Port if enabled */
        if( pstSerlSets->bEnable )
        {
            DEBUG_MB( "COM%d: TransProt %d, BR %d, TransMode %d, Parity %d\n", \
                       emSerlPortID+1, pstSerlSets->ucTransProto, pstSerlSets->ucBaudRate,
                       pstSerlSets->ucTransMode, pstSerlSets->ucParity );

            SetMBTransProto( emSerlPortID, (uint16_t)pstSerlSets->ucTransProto );
            SetMBPortBaudRate( emSerlPortID, (uint16_t)pstSerlSets->ucBaudRate );
            SetMBPortTransMode( emSerlPortID, (uint16_t)pstSerlSets->ucTransMode );
            SetMBPortParity( emSerlPortID, (uint16_t)pstSerlSets->ucParity );

            /* Port Enable */
            SetMBPortEnable( emSerlPortID, pstSerlSets->bEnable );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SetSerlPort
*
* 功能描述: 设置串口
*
* 输入参数: emSerlPortID：串口ID
*
* 输出参数: 无
*
* 返 回 值: 是否设置成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SetSerlPort( SerialPortType_t emSerlPortID )
{
    COMConfigInfo_t const* pstCurCOMCfg = NULL;
    SerlPortSets_t stSerlSets;

    if( emSerlPortID < NUM_OF_SERIAL_PORT )
    {
        DEBUG_MB( "SetSerlPort %d\n", emSerlPortID+1 );

        /* Get Serial Port Configuration */
        pstCurCOMCfg = GetCurCOMCfg( &s_stSerlCfgInfo[emSerlPortID]);
        if( pstCurCOMCfg != NULL )
        {
            /* Configuration Settings */
            if(((uint8_t)(SPP_NONE) == pstCurCOMCfg->ucProtocol) ||
               ((uint8_t)(SPP_GPS) == pstCurCOMCfg->ucProtocol))
            {
                stSerlSets.bEnable = false;
            }
            else
            {
                stSerlSets.bEnable = true;
                if(((uint8_t)(MODBUS_MASTER_RTU) == pstCurCOMCfg->ucProtocol) ||
                   ((uint8_t)(MODBUS_SLAVE_RTU) == pstCurCOMCfg->ucProtocol))
                {
                    stSerlSets.ucTransProto = (uint8_t)(MB_TRANS_PROTO_RTU);
                }
                else
                {
                    stSerlSets.ucTransProto = (uint8_t)(MB_TRANS_PROTO_ASCII);
                }
            }

            stSerlSets.ucBaudRate = pstCurCOMCfg->ucBaudRate;
            stSerlSets.ucTransMode = pstCurCOMCfg->ucTransMode;
            stSerlSets.ucParity = pstCurCOMCfg->ucParity;
        }
        else
        {
            /* Default Settings */
            stSerlSets.ucTransProto = s_ucDefaultTP;  /* 0: RTU */
            stSerlSets.ucBaudRate = s_ucDefaultBR;    /* 3: 9600 */
            stSerlSets.ucTransMode = s_ucDefaultTM;   /* 0: RS485 */
            stSerlSets.ucParity = s_ucDefaultParity;  /* 1: Odd */
            stSerlSets.bEnable = s_bDefaultEnable;    /* false */
        }

        /* Configure Serial Port */
        ConfigSerlPort( emSerlPortID, &stSerlSets );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SerialSend
*
* 功能描述: 通过串口发送数据
*
* 输入参数: emSerlPortID：串口ID；
*          pucData：待发送的数据；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 是否发送成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SerialSend( SerialPortType_t emSerlPortID, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bResult = false;
    uint8_t ucRetryCnts = 0U;

    if((emSerlPortID < NUM_OF_SERIAL_PORT ) &&
       (pucData != NULL) &&
       (usDataLen > 0U))
    {
        /* Send */
        while( ucRetryCnts < (uint8_t)(MAX_RETRY_CNT))
        {
            if( MBPortSend( emSerlPortID, pucData, usDataLen ))
            {
                /* Send Successfully */
                bResult = true;
                break;
            }
            else
            {
                /* Sending pre-frame: wait and retry */
                ucRetryCnts++;
                DEBUG_MB( "COM%d SerlSend Retry %d\n", emSerlPortID+1, ucRetryCnts );
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SerialRecv
*
* 功能描述: 通过串口接收数据
*
* 输入参数: emSerlPortID：串口ID；
*          pucBuff：接收缓冲区；
*          pusLen：接收缓冲区长度。
*
* 输出参数: pusLen：数据的实际长度
*
* 返 回 值: 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SerialRecv( SerialPortType_t emSerlPortID, uint8_t pucBuff[], uint16_t *pusLen )
{
    bool_t bResult = false;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    if((emSerlPortID < NUM_OF_SERIAL_PORT) &&
       (pucBuff != NULL) &&
       (pusLen != NULL))
    {
        /* Receive */
        if( MBPortRecv( emSerlPortID, pucBuff, pusLen ))
        {
            bResult = true;

#ifdef DEBUG_MODBUS
            if( *pusLen > 0U )
            {
                printf( "COM[%d] MBSerl Message: Length: %d, Content: \n", emSerlPortID-COM1+1, *pusLen );

                for( usIndex = 0U; usIndex < *pusLen; usIndex++ )
                {
                    printf( "%02X ", pucBuff[usIndex]);
                    usPerLen++;

                    if(( 30U == usPerLen ) ||
                       ((*pusLen-1) == usIndex))
                    {
                        printf( "\n" );
                        usPerLen = 0U;
                    }
                }
            }
#endif
        }
        else
        {
            bResult = false;  /* Received a error frame */
            *pusLen = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: InitLocalConfigInfo
*
* 功能描述: 初始化本地保存的配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitLocalConfigInfo( void )
{
    uint8_t ucIndex = 0U;

    /* Serial Configuration Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_SERIAL_PORT); ucIndex++ )
    {
        InitSerlCfgInfo( &s_stSerlCfgInfo[ucIndex]);
    }

    /* Modbus TCP Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_NETWORK_PORT); ucIndex++ )
    {
        InitModbusTCPInfo( &s_stMBTCPInfo[ucIndex]);
    }

    /* Modbus Master Info. */
    InitModbusMstrInfo( &s_stMBMstrCfgInfo );
    s_uiMBMstrCfgOffset = 0U;

    /* Modbus Slave Configuration Info. */
    InitModbusSlaveInfo( &s_stMBSlaveCfgInfo );
    s_uiMBSlaveCfgOffset = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitSerlCfgInfo
*
* 功能描述: 初始化本地保存的某一串口的配置信息
*
* 输入参数: pstSerlCfgInfo：指向本地保存的串口配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitSerlCfgInfo( SerlCfgInfo_t *pstSerlCfgInfo )
{
    uint8_t ucVerIndex = 0U;
    COMConfigInfo_t *pCOMCfg = NULL;

    if( pstSerlCfgInfo != NULL )
    {
        memset( pstSerlCfgInfo, 0, sizeof(SerlCfgInfo_t));

        /* Modbus TCP Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pCOMCfg = &( pstSerlCfgInfo->stCOMConfig[ucVerIndex]);
            pCOMCfg->emActive = NOT_ACTIVE;
            pCOMCfg->bUpdate = false;
            pCOMCfg->ucProtocol = (uint8_t)(SPP_NONE);
            pCOMCfg->bPortWriteEnable = false;
            pCOMCfg->ucModbusSlaveAddr = 1U;
            pCOMCfg->ucBaudRate = (uint8_t)(BR9600);
            pCOMCfg->ucDataBits = (uint8_t)(DB8);
            pCOMCfg->ucStopBits = (uint8_t)(SB2);
            pCOMCfg->ucParity = (uint8_t)(PARITY_ODD);
            pCOMCfg->ucTransMode = (uint8_t)(TM_RS485);
            pCOMCfg->ucFPOrder = (uint8_t)(FP_ORDER_L);
            pCOMCfg->sModbusMin = (int16_t)(MODBUS_RNG_MIN);
            pCOMCfg->sModbusMax = (int16_t)(MODBUS_RNG_MAX);
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitModbusTCPInfo
*
* 功能描述: 初始化本地保存的某一网口的ModbusTCP信息
*
* 输入参数: pstMBTCPInfo：指向本地保存的ModbusTCP信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitModbusTCPInfo( MBTCPCfgInfo_t *pstMBTCPInfo )
{
    uint8_t ucVerIndex = 0U;
    ModbusTCPConfigInfo_t *pMBTCPCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        memset( pstMBTCPInfo, 0, sizeof(MBTCPCfgInfo_t));

        /* Modbus TCP Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pMBTCPCfg = &( pstMBTCPInfo->stMBTCPConfig[ucVerIndex]);
            pMBTCPCfg->emActive = NOT_ACTIVE;
            pMBTCPCfg->bUpdate = false;
            pMBTCPCfg->ucProtocol = (uint8_t)(MODBUS_TCP_NONE);
            pMBTCPCfg->bPortWriteEnable = false;
            pMBTCPCfg->ucMasterLogicPort = 0U;
            pMBTCPCfg->ucFPOrder = (uint8_t)(FP_ORDER_L);
            pMBTCPCfg->usTCPPortNum = (uint16_t)(PORTNUM_MODBUS);
            pMBTCPCfg->uiIPAddr = 0U;
            pMBTCPCfg->sModbusMin = (int16_t)(MODBUS_RNG_MIN);
            pMBTCPCfg->sModbusMax = (int16_t)(MODBUS_RNG_MAX);
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitModbusSlaveInfo
*
* 功能描述: 初始化本地保存的 Modbus 从站信息
*
* 输入参数: pstMBSlaveInfo：指向本地保存的Modbus 主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void InitModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo )
{
    uint8_t ucVerIndex = 0U;
    uint8_t ucBlkIndex = 0U;
    ModbusSlaveInfo_t *pstSlaveCfg = NULL;

    if( pstMBSlaveInfo != NULL )
    {
        memset( pstMBSlaveInfo, 0, sizeof(MBSlaveCfgInfo_t));

        /* Flag */
        pstMBSlaveInfo->bCopying = false;
        pstMBSlaveInfo->bValid = false;

        /* Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pstSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucVerIndex]);
            for( ucBlkIndex = 0U; ucBlkIndex < (uint8_t)(MAX_MODBUS_AREA_NUM); ucBlkIndex++ )
            {
                pstSlaveCfg->stModbusSlaveBlockInfo[ucBlkIndex].emActive = NOT_ACTIVE;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: InitPortMstrInfo
*
* 功能描述: 初始化某一端口的主站信息
*
* 输入参数: pstPortMstrInfo：指向本地保存的端口主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/
static void InitPortMstrInfo( PortMstrInfo_t *pstPortMstrInfo )
{
    if( pstPortMstrInfo != NULL )
    {
        memset( pstPortMstrInfo, 0, sizeof( PortMstrInfo_t ));

        pstPortMstrInfo->emState = IDLE_STATE;
        pstPortMstrInfo->bHandleSwitch = false;
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SelectPM
*
* 功能描述: 选取与CM模块内部通讯正常的，编号最小的PM模块
*
* 输入参数：无
*
* 输出参数: 无
*
* 返 回 值: 选取的PMID
*
* 注意事项: 无
***********************************************************************************************************************
*/

static SelPMID_t SelectPM( void )
{
    SelPMID_t emSelPMID = SELPM_NONE;

    /* Select: Normal, Minimum PMID */
    if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMA ))
    {
        emSelPMID = SELPM_A;
    }
    else if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMB ))
    {
        emSelPMID = SELPM_B;
    }
    else if( INTER_COMM_NORMAL == SysGetLocalInterCommState( PMC ))
    {
        emSelPMID = SELPM_C;
    }
    else
    {
        emSelPMID = SELPM_NONE;
    }

    return emSelPMID;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalInfo
*
* 功能描述: 清除本地配置信息，重新设置串口-禁止串口，清除串口缓冲区
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存；
*          网口缓冲区的清除由外部通讯模块完成。
***********************************************************************************************************************
*/

static void ClearLocalInfo( void )
{
    DEBUG_MB( "ClearLocalInfo\n" );

    uint8_t ucIndex = 0U;
    SerialPortType_t emSerlID = COM1;

    s_ucMBTCPMstrIndex = 0U;
    s_emLastPMID = INVALID_PM_ID;

    /* Port */
    for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_PORT_NUM); ucIndex++ )
    {
        InitPortMstrInfo( &s_stPortMstrInfo[ucIndex]);
    }

    /* Modbus TCP Message */
    memset( &s_stMBTCPMsg.stNetHead, 0, sizeof(s_stMBTCPMsg.stNetHead));
    s_stMBTCPMsg.stNetHead.emNetwork = INVALID_NETWORK_PORT;

    /* Modbus Serial Message */
    s_stMBSerlMsg.usLen = 0U;

    /* Local Configuration Info. */
    ClearLocalConfigInfo();

    /* Serial Port */
    for( emSerlID = COM1; emSerlID < NUM_OF_SERIAL_PORT; emSerlID++ )
    {
        /* Set Serial Port: Disable */
        SetSerlPort( emSerlID );

        /* Clear Serial Buffer */
        ClearSerlPortRecvBuff( emSerlID );
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearLocalConfigInfo
*
* 功能描述: 清除本地保存的配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/

static void ClearLocalConfigInfo( void )
{
    uint8_t ucIndex = 0U;

    /* Serial Configuration Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_SERIAL_PORT); ucIndex++ )
    {
        InitSerlCfgInfo( &s_stSerlCfgInfo[ucIndex]);
    }

    /* Modbus TCP Info. */
    for( ucIndex = 0U; ucIndex < (uint8_t)(NUM_OF_NETWORK_PORT); ucIndex++ )
    {
        InitModbusTCPInfo( &s_stMBTCPInfo[ucIndex]);
    }

    /* Modbus Master Info. */
    ClearModbusMstrInfo( &s_stMBMstrCfgInfo );
    s_uiMBMstrCfgOffset = 0U;

    /* Modbus Slave Configuration Info. */
    ClearMBSlaveInfo( &s_stMBSlaveCfgInfo );
    s_uiMBSlaveCfgOffset = 0U;

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearMBSlaveInfo
*
* 功能描述: 清除Modbus 从站配置信息
*
* 输入参数: pstMBSlaveInfo：Modbus 从站配置信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 为保证控制周期，只有在初始化时，可清大块内存。
***********************************************************************************************************************
*/

static void ClearMBSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo )
{
    uint8_t ucVerIndex = 0U;
    uint8_t ucBlkIndex = 0U;
    uint32_t uiLen = 0U;
    ModbusSlaveInfo_t *pstSlaveCfg = NULL;
    ModbusSlaveBlock_t *pstSlaveBlk = NULL;

    if( pstMBSlaveInfo != NULL )
    {
        uiLen = sizeof(MBSlaveCfgInfo_t) - sizeof(ModbusSlaveInfo_t)*(uint32_t)(LOCAL_CFG_VER_CNT);
        memset( pstMBSlaveInfo, 0, uiLen );

        /* Flag */
        pstMBSlaveInfo->bCopying = false;
        pstMBSlaveInfo->bValid = false;

        /* Local Configuration */
        for( ucVerIndex = 0U; ucVerIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucVerIndex++ )
        {
            pstSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucVerIndex]);
            uiLen = sizeof(ModbusSlaveInfo_t) - sizeof(ModbusSlaveBlock_t)*(uint32_t)(MAX_MODBUS_AREA_NUM);
            memset( pstSlaveCfg, 0, uiLen );

            for( ucBlkIndex = 0U; ucBlkIndex < (uint8_t)(MAX_MODBUS_AREA_NUM); ucBlkIndex++ )
            {
                pstSlaveBlk = &( pstSlaveCfg->stModbusSlaveBlockInfo[ucBlkIndex]);
                pstSlaveBlk->emActive = NOT_ACTIVE;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleConfiguration
*
* 功能描述: 处理配置信息
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleConfiguration( void )
{
    uint8_t ucIndex = 0U;
    PortMstrInfo_t *pstPortMstrInfo = NULL;

    /* Modbus Slave Configuration Info. */
    if( HandleModbusSlaveCfgInfo( &s_stMBSlaveCfgInfo, &s_uiMBSlaveCfgOffset ))
    {
        DEBUG_MB( "MBSlave Cfg Switch\n" );
    }

    /* Modbus Master Info. */
    if( HandleModbusMstrInfo( &s_stMBMstrCfgInfo, &s_uiMBMstrCfgOffset, s_ucLocalCMSlotID ))
    {
        DEBUG_MB( "MBMstr Cfg Switch\n" );

        for( ucIndex = 0U; ucIndex < (uint8_t)(MAX_PORT_NUM); ucIndex++ )
        {
            /* Port Update Flag */
            if( GetPortMBMstrUpdate( &s_stMBMstrCfgInfo, ucIndex ))
            {
                DEBUG_MB( "MBMstrPort: %d Cfg Update\n", ucIndex );

                pstPortMstrInfo = &( s_stPortMstrInfo[ucIndex]);
                UpdatePortMstrInfo( pstPortMstrInfo );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSlaveCfgInfo
*
* 功能描述: 处理Modbus从站配置信息
*
* 输入参数: pstMBSlaveInfo：指向本地保存的Modbus从站配置信息；
*          puiOffset：指向拷贝偏移值。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换 - 配置参数拷贝完成且有效时，返回true
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleModbusSlaveCfgInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo, puint32_t puiOffset )
{
    bool_t bSwitch = false;
    bool_t bComplete = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;

    if((pstMBSlaveInfo != NULL) &&
       (puiOffset != NULL))
    {
        if( *puiOffset > 0U )
        {
            /* Continue Copy */
            if( !GetModbusSlaveInfo( pstMBSlaveInfo, puiOffset, &bComplete ))
            {
                DEBUG_MB( "GetMBSlaveInfo Failed\n" );
                ClearCurModbusSlaveCfg( pstMBSlaveInfo );
            }
            else
            {
                //DEBUG_MB( "GetModbusSlaveInfo Successfully!\n" );
            }
        }
        else
        {
            /* First Try To Copy */
            /* RTData Version */
            /*if( HandleRTDataVer( &( pstMBSlaveInfo->stRTDataVer)))*/
            {
                /*DEBUG_MB( "HandleModbusSlaveCfgInfo: RTDataVer Changed, PrjCRC-0x%x.\n", \
                            pstMBSlaveInfo->stRTDataVer.uiProjCRC );*/

                /* Get Configuration Version */
                if( SysGetModbusSlaveConfigVersion( &uiCRC, &uiFileVer ))
                {
                    /*DEBUG_MB( "HandleModbusSlaveCfgInfo: MBSlave, CRC-0x%x, FileVer-%d.\n", \
                               uiCRC, uiFileVer );*/

                    /* Compare Version */
                    if((uiCRC != pstMBSlaveInfo->uiCfgCRC) ||
                       (uiFileVer != pstMBSlaveInfo->uiCfgVer))
                    {
                        DEBUG_MB( "Start Copy MBSlaveCfg\n" );

                        /* Start Copy */
                        pstMBSlaveInfo->bCopying = true;

                        /* Switch */
                        SwitchModbusSlaveInfo( pstMBSlaveInfo );

                        /* Clear Current Configuration */
                        ClearCurModbusSlaveCfg( pstMBSlaveInfo );

                        /* Update Version Info. */
                        pstMBSlaveInfo->uiCfgCRC = uiCRC;
                        pstMBSlaveInfo->uiCfgVer = uiFileVer;

                        /* Copy Configuration Info. */
                        if( !GetModbusSlaveInfo( pstMBSlaveInfo, puiOffset, &bComplete ))
                        {
                            DEBUG_MB( "GetMBSlaveInfo Failed\n" );
                            ClearCurModbusSlaveCfg( pstMBSlaveInfo );
                        }
                        else
                        {
                            //DEBUG_MB( "GetModbusSlaveInfo Successfully!\n" );
                        }
                    }
                }
            }
        }

        if( bComplete )
        {
            if( !CheckCurModbusSlaveInfo( pstMBSlaveInfo ))
            {
                /* Invalid */
                pstMBSlaveInfo->bValid = false;
                ClearCurModbusSlaveCfg( pstMBSlaveInfo );
                DEBUG_MB( "MBSlaveCfg Invalid\n" );
            }
            else
            {
                /* Valid */
                pstMBSlaveInfo->bValid = true;
                bSwitch = true;
            }

            pstMBSlaveInfo->bCopying = false;
            *puiOffset = 0U;
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: GetModbusSlaveInfo
*
* 功能描述: 将Modbus从站配置信息拷贝到本地
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站配置参数；
*          puiOffset：指向拷贝偏移值。
*
* 输出参数: pbComplete：是否将所有有效Modbus从站配置拷贝到本地
*
* 返 回 值: 拷贝操作是否成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo, puint32_t puiOffset, bool_t *pbComplete )
{
    bool_t bResult = false;
    puint8_t pucBuff = NULL;  /* Buffer Pointer */
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    uint32_t uiDstLen = 0U;   /* Destination Copy Length */
    uint32_t uiCopyLen = 0U;  /* Real Copy Length */
    uint32_t uiContLen = 0U;  /* Content Length */
    ModbusSlaveInfo_t *pstMBSlaveCfg = NULL;

    if((pstMBSlaveInfo != NULL) &&
       (puiOffset != NULL) &&
       (pbComplete != NULL))
    {
        *pbComplete = false;

        /* Get First Address */
        ucCfgIndex = pstMBSlaveInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucCfgIndex]);
            pucBuff = (puint8_t)pstMBSlaveCfg;
            uiContLen = sizeof(ModbusSlaveInfo_t) - *puiOffset;

            /* Calculate Cycle Copy Length */
            if( uiContLen > (uint32_t)(MAX_CYCLE_COPY_LEN))
            {
                uiDstLen = (uint32_t)(MAX_CYCLE_COPY_LEN);
            }
            else
            {
                uiDstLen = uiContLen;
            }

            /*DEBUG_MB( "GetModbusSlaveInfo: Total Size: %d, Offset: %d, Remain: %d, Dst Copy Len: %d.\n", \
                       sizeof(ModbusSlaveInfo_t), *puiOffset, uiContLen, uiDstLen );*/

            /* Copy Content */
            uiCopyLen = SysGetModbusSlaveConfigInfo( &pucBuff[*puiOffset], *puiOffset, uiDstLen );
            if( uiCopyLen > 0U )
            {
                /* Copy Successfully */
                bResult = true;
                *puiOffset += uiCopyLen;
                if( uiContLen == uiCopyLen )
                {
                    *pbComplete = true;
                }
            }
            else
            {
                bResult = false;
                *puiOffset = 0U;
            }
        }
        else
        {
            bResult = false;
            *puiOffset = 0U;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchModbusSlaveInfo
*
* 功能描述: 更新索引值
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SwitchModbusSlaveInfo( MBSlaveCfgInfo_t *pstMBSlaveInfo )
{
    uint8_t ucCfgIndex = 0U;

    if( pstMBSlaveInfo != NULL )
    {
        ucCfgIndex = pstMBSlaveInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = (ucCfgIndex + 1U) % (uint8_t)(LOCAL_CFG_VER_CNT);

            /* Update Index */
            pstMBSlaveInfo->ucIndex = ucCfgIndex;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: ClearCurModbusSlaveCfg
*
* 功能描述: 清除当前Modbus从站配置
*
* 输入参数: pstMBSlaveInfo：指向Modbus主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void ClearCurModbusSlaveCfg( MBSlaveCfgInfo_t *pstMBSlaveInfo )
{
    ModbusSlaveInfo_t *pstMBSlaveCfg = NULL;
    uint8_t ucCfgIndex = 0;

    if( pstMBSlaveInfo != NULL )
    {
        ucCfgIndex = pstMBSlaveInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucCfgIndex]);
            pstMBSlaveCfg->uiCRC = 0U;
            pstMBSlaveCfg->uiFileVern = 0U;
            pstMBSlaveCfg->uiBlockNum = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCurModbusSlaveInfo
*
* 功能描述: 检查Modbus从站配置信息是否有效
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站信息
*
* 输出参数: 无
*
* 返 回 值: 配置信息是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckCurModbusSlaveInfo( MBSlaveCfgInfo_t const *pstMBSlaveInfo )
{
    bool_t bValid = false;
    uint8_t ucCfgIndex = 0U;
    uint32_t uiIndex = 0U;
    uint32_t uiBlkNum = 0U;
    ModbusSlaveInfo_t const *pstMBSlaveCfg = NULL;

    if( pstMBSlaveInfo != NULL )
    {
        ucCfgIndex = pstMBSlaveInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucCfgIndex]);

            /* Block Number */
            uiBlkNum = pstMBSlaveCfg->uiBlockNum;

            /* Head */
            if( uiBlkNum <= (uint32_t)(MAX_MODBUS_AREA_NUM))
            {
                bValid = true;

                /* Block */
                for( uiIndex = 0U; uiIndex < (uint32_t)(MAX_MODBUS_AREA_NUM); uiIndex++ )
                {
                    if( !CheckModbusSlaveBlk( &( pstMBSlaveCfg->stModbusSlaveBlockInfo[uiIndex])))
                    {
                        DEBUG_MB( "MBSlaveBlk %d Invalid\n", uiIndex );
                        bValid = false;
                        break;
                    }
                }
            }
            else
            {
                DEBUG_MB( "MBSlaveCfg BlkNum-%d Invalid\n", uiBlkNum );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusSlaveBlk
*
* 功能描述: 检查某一Modbus从站块的配置信息是否有效
*
* 输入参数: pstBlk：指向待检查的配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusSlaveBlk( ModbusSlaveBlock_t const *pstBlk )
{
    bool_t bValid = false;
    uint32_t uiIndex = 0U;
    ModbusSlaveVar_t const *pstMBSlaveVar = NULL;

    if( pstBlk != NULL )
    {
        bValid = true;

        /* Active Flag */
        if( ACTIVE == pstBlk->emActive )
        {
            /* Block Header */
            bValid = CheckModbusSlaveBlkHead( &( pstBlk->stModbusSlaveBlockHead));
            if( bValid )
            {
                /* Variable: Check Active Variables */
                for( uiIndex = 0U; uiIndex < (uint32_t)(MAX_MODBUS_RGN_VAR_NUM); uiIndex++ )
                {
                    pstMBSlaveVar = &( pstBlk->stModbusSlaveVar[uiIndex]);
                    if((uint8_t)(MODBUS_VAR_ACTIVE) == pstMBSlaveVar->ucActive )
                    {
                        if( !CheckMBSlaveVar( &( pstMBSlaveVar->stModbusVar), \
                                              pstBlk->stModbusSlaveBlockHead.ucModbusRgn ))
                        {
                            DEBUG_MB( "MBVar-%d Invalid\n", uiIndex );
                            bValid = false;
                            break;
                        }
                    }
                }
            }
            else
            {
                DEBUG_MB( "MBSlaveBlk Head Invalid\n" );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusSlaveBlkHead
*
* 功能描述: 检查某一Modbus从站块的头信息是否有效
*
* 输入参数: pstBlkHead：指向待检查的配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusSlaveBlkHead( ModbusSlaveBlockHead_t const *pstBlkHead )
{
    bool_t bValid = false;

    if( pstBlkHead != NULL )
    {
        /* Modbus Region, Permission */
        if(((pstBlkHead->ucModbusRgn >= (uint8_t)(MR_COIL)) && (pstBlkHead->ucModbusRgn <= (uint8_t)(MR_HOLD_REGS))) &&
           (((uint8_t)(PMSN_READONLY) == pstBlkHead->ucPermission) || (PMSN_READWRITE == pstBlkHead->ucPermission)))
        {
            bValid = true;

            /* uiBeginAddr, uiEndAddr: 0 - 9999 */
            if((pstBlkHead->uiBeginAddr > pstBlkHead->uiEndAddr) ||
               (pstBlkHead->uiBeginAddr > MAX_MBRGN_INDEX) ||
               (pstBlkHead->uiEndAddr > MAX_MBRGN_INDEX))
            {
                bValid = false;
                DEBUG_MB( "MBSlaveBlkHead: BeginAddr-%d / EndAddr-%d Invalid\n", \
                           pstBlkHead->uiBeginAddr, pstBlkHead->uiEndAddr );
            }

            if( bValid )
            {
                /* Variable Number */
                if( pstBlkHead->uiModbusVarNum > (uint32_t)(MAX_MODBUS_RGN_VAR_NUM))
                {
                    bValid = false;
                    DEBUG_MB( "MBSlaveBlkHead: VarNum-%d Invalid\n", pstBlkHead->uiModbusVarNum );
                }
            }
        }
        else
        {
            DEBUG_MB( "MBSlaveBlkHead: MBRgn-%d / Permission-%d Invalid\n", \
                      pstBlkHead->ucModbusRgn, pstBlkHead->ucPermission );
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurModbusSlaveCfg
*
* 功能描述: 获取当前使用的Modbus从站配置参数
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站信息
*
* 输出参数: 无
*
* 返 回 值: 指向当前使用的配置参数：Modbus从站信息处于拷贝状态 或者 无效时，返回NULL。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusSlaveInfo_t const* GetCurModbusSlaveCfg( MBSlaveCfgInfo_t const *pstMBSlaveInfo )
{
    ModbusSlaveInfo_t const *pstSlaveCfg = NULL;
    uint8_t ucCfgIndex = 0U;

    if( pstMBSlaveInfo != NULL )
    {
        if((!pstMBSlaveInfo->bCopying) &&
           (pstMBSlaveInfo->bValid))
        {
            ucCfgIndex = pstMBSlaveInfo->ucIndex;
            if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
            {
                pstSlaveCfg = &( pstMBSlaveInfo->stSlaveCfg[ucCfgIndex]);
            }
        }
    }

    return pstSlaveCfg;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: GetModbusSlaveBlkNum
*
* 功能描述: 获取Modbus从站块的数目
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站信息
*
* 输出参数: 无
*
* 返 回 值: Modbus从站块的数目
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint32_t GetModbusSlaveBlkNum( MBSlaveCfgInfo_t const *pstMBSlaveInfo )
{
    uint32_t uiBlkNum = 0U;
    ModbusSlaveInfo_t const *pstSlaveCfg = NULL;

    if( pstMBSlaveInfo != NULL )
    {
        pstSlaveCfg = GetCurModbusSlaveCfg( pstMBSlaveInfo );
        if( pstSlaveCfg != NULL )
        {
            uiBlkNum = pstSlaveCfg->uiBlockNum;
        }
    }

    return uiBlkNum;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: GetModbusSlaveBlk
*
* 功能描述: 获取Modbus从站块配置参数
*
* 输入参数: pstMBSlaveInfo：指向Modbus从站信息；
*          ucModbusRgn：Modbus数据区。
*
* 输出参数: 无
*
* 返 回 值: 指向某一Modbus从站块配置参数
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusSlaveBlock_t const* GetModbusSlaveBlk( MBSlaveCfgInfo_t const *pstMBSlaveInfo, uint8_t ucModbusRgn )
{
    ModbusSlaveInfo_t const *pstSlaveCfg = NULL;
    ModbusSlaveBlock_t const *pstBlk = NULL;

    if((pstMBSlaveInfo != NULL) &&
       ((ucModbusRgn >= (uint8_t)(MR_COIL)) && (ucModbusRgn <= (uint8_t)(MR_HOLD_REGS))))
    {
        pstSlaveCfg = GetCurModbusSlaveCfg( pstMBSlaveInfo );
        if( pstSlaveCfg != NULL )
        {
            pstBlk = &( pstSlaveCfg->stModbusSlaveBlockInfo[ucModbusRgn]);
            if( ACTIVE == pstBlk->emActive )
            {
                if( ucModbusRgn != pstBlk->stModbusSlaveBlockHead.ucModbusRgn )
                {
                    DEBUG_MB( "MBSlaveBlk: Rgn-%d != %d\n", ucModbusRgn, pstBlk->stModbusSlaveBlockHead.ucModbusRgn );
                    pstBlk = NULL;
                }
            }
        }
    }

    return pstBlk;
}

/*
***********************************************************************************************************************
* 函数名称: UpdatePortMstrInfo
*
* 功能描述: 端口主站信息变化时，更新通信端口的Modbus主站信息
*
* 输入参数: pstPortMstrInfo：指向某一端口的Modbus主站信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void UpdatePortMstrInfo( PortMstrInfo_t *pstPortMstrInfo )
{
    if( pstPortMstrInfo != NULL )
    {
        if( IDLE_STATE == pstPortMstrInfo->emState )
        {
            pstPortMstrInfo->uiBlockIndex = 0U;
            pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
            pstPortMstrInfo->bHandleSwitch = false;
        }
        else
        {
            pstPortMstrInfo->bHandleSwitch = true;  /* Busy: Handle Switch Later */
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortMBMstrCfg
*
* 功能描述: 获取某一通信端口的Modbus主站信息
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息;
*          uiPortIndex：端口索引值。
*
* 输出参数: 无
*
* 返 回 值: 指向端口的Modbus主站信息：端口Modbus主站信息非Active时，返回NULL。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusMasterPort_t const* GetPortMBMstrCfg( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex )
{
    ModbusMasterConfig_t const *pstLocalMBMstrCfg = NULL;
    ModbusMasterPort_t const *pstPortCfg = NULL;       /* Port Configuration */

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)))
    {
        pstLocalMBMstrCfg = GetLocalModbusMstrCfg( pstModbusMstrInfo, s_ucLocalCMSlotID );
        if( pstLocalMBMstrCfg != NULL )
        {
            pstPortCfg = &( pstLocalMBMstrCfg->stModbusMasterPort[uiPortIndex]);
            if( pstPortCfg->emActive != ACTIVE )
            {
                pstPortCfg = NULL;
            }
        }
    }

    return pstPortCfg;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortMBMstrCfg
*
* 功能描述: 获取某一通信端口的Modbus主站信息
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息；
*          uiPortIndex：端口索引值；
*          uiBlkIndex：主站块索引值；
*
* 输出参数: pucBlkID：主站块的ID
*
* 返 回 值: 指向端口的Modbus主站信息：端口Modbus主站信息非Active时，返回NULL。
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetPortMBMstrBlkID( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex,
                                  uint32_t uiBlkIndex, uint8_t *pucBlkID )
{
    bool_t bResult = false;
    ModbusMasterPort_t const *pstMBMstrPort = NULL;

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)) &&
       (uiBlkIndex < (uint32_t)(MAX_MODBUS_BLOCK_NUM)) &&
       (pucBlkID != NULL))
    {
        pstMBMstrPort = GetPortMBMstrCfg( pstModbusMstrInfo, uiPortIndex );
        if( pstMBMstrPort != NULL )
        {
            if( uiBlkIndex < pstMBMstrPort->uiBlockNum )
            {
                bResult = true;
                *pucBlkID = pstMBMstrPort->ucModbusMasterBlockID[uiBlkIndex];
            }
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortMBMstrUpdate
*
* 功能描述: 获取某一通信端口Modbus主站信息的更新标志
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息；
*          uiPortIndex：端口索引值。
*
* 输出参数: 无
*
* 返 回 值: 更新标志
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetPortMBMstrUpdate( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex )
{
    bool_t bUpdate = false;
    ModbusMasterPort_t const *pstPortCfg = NULL;  /* Port Configuration */

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)))
    {
        pstPortCfg = GetPortMBMstrCfg( pstModbusMstrInfo, uiPortIndex );
        if( pstPortCfg != NULL )
        {
            bUpdate = pstPortCfg->bUpdate;
        }
    }

    return bUpdate;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBMstrBlkNum
*
* 功能描述: 获取某一通信端口Modbus主站块的数目
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息；
*          uiPortIndex：端口索引值。
*
* 输出参数: 无
*
* 返 回 值: 端口主站块的数目
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint32_t GetPortMBMstrBlkCnt( ModbusMstrCfgInfo_t const *pstModbusMstrInfo, uint32_t uiPortIndex )
{
    uint32_t uiBlkCnt = 0;
    ModbusMasterPort_t const *pstPortCfg = NULL;  /* Port Configuration */

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)))
    {
        pstPortCfg = GetPortMBMstrCfg( pstModbusMstrInfo, uiPortIndex );
        if( pstPortCfg != NULL )
        {
            uiBlkCnt = pstPortCfg->uiBlockNum;
        }
    }

    return uiBlkCnt;
}

#if 0
/*
***********************************************************************************************************************
* 函数名称: GetPortMBMstrBlk
*
* 功能描述: 获取某一通信端口的Modbus主站块配置
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息；
*          uiPortIndex：端口索引值；
*          uiBlkIndex：块索引值。
*
* 输出参数: 无
*
* 返 回 值: 指向端口某一主站块块配置
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusMasterBlock_t const* GetPortMBMstrBlk( ModbusMstrCfgInfo_t const *pstModbusMstrInfo,
                                                    uint32_t uiPortIndex, uint32_t uiBlkIndex )
{
    uint8_t ucBlkID = 0U;
    bool_t bResult = false;
    ModbusMasterInfo_t const *pstMBMstrInfo = NULL;
    ModbusMasterBlock_t const *pstBlk = NULL;     /* Block */

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)) &&
       (uiBlkIndex < (uint32_t)(MAX_MODBUS_BLOCK_NUM)))
    {
        pstMBMstrInfo = GetCurModbusMstrInfo( pstModbusMstrInfo );
        bResult = GetPortMBMstrBlkID( pstModbusMstrInfo, uiPortIndex, uiBlkIndex, &ucBlkID );
        if((pstMBMstrInfo != NULL) && bResult )
        {
            if( ucBlkID < (uint32_t)(MAX_MODBUS_BLOCK_NUM))
            {
                pstBlk = &( pstMBMstrInfo->stModbusMasterBlock[ucBlkID]);
                if( pstBlk->emActive != ACTIVE )
                {
                    pstBlk = NULL;
                }
            }
        }
    }

    return pstBlk;
}
#endif

/*
***********************************************************************************************************************
* 函数名称: GetPortMBMstrBlkInfo
*
* 功能描述: 获取某一Modbus主站块的信息：ID, Addr
*
* 输入参数: pstModbusMstrInfo：指向Modbus主站信息；
*          uiPortIndex：端口索引值；
*          uiBlkIndex：块索引值。
*
* 输出参数: pucBlkID：主站块ID
*
* 返 回 值: 主站块地址
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusMasterBlock_t const* GetPortMBMstrBlkInfo( ModbusMstrCfgInfo_t const *pstModbusMstrInfo,
                                                        uint32_t uiPortIndex, uint32_t uiBlkIndex, uint8_t *pucBlkID )
{
    uint8_t ucBlkID = 0U;
    bool_t bResult = false;
    ModbusMasterInfo_t const *pstMBMstrInfo = NULL;
    ModbusMasterBlock_t const *pstBlk = NULL;     /* Block */

    if((pstModbusMstrInfo != NULL) &&
       (uiPortIndex < (uint32_t)(MAX_PORT_NUM)) &&
       (uiBlkIndex < (uint32_t)(MAX_MODBUS_BLOCK_NUM)) &&
       (pucBlkID != NULL))
    {
        pstMBMstrInfo = GetCurModbusMstrInfo( pstModbusMstrInfo );
        bResult = GetPortMBMstrBlkID( pstModbusMstrInfo, uiPortIndex, uiBlkIndex, &ucBlkID );
        if((pstMBMstrInfo != NULL) && bResult )
        {
            if( ucBlkID < (uint32_t)(MAX_MODBUS_BLOCK_NUM))
            {
                pstBlk = &( pstMBMstrInfo->stModbusMasterBlock[ucBlkID]);
                if( pstBlk->emActive != ACTIVE )
                {
                    pstBlk = NULL;
                }
                else
                {
                    *pucBlkID = ucBlkID;
                }
            }
        }
    }

    return pstBlk;
}

/*
***********************************************************************************************************************
* 函数名称: HandlePortConfig
*
* 功能描述: 处理与通信端口相关的配置参数
*
* 输入参数: emPort：端口号
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandlePortConfig( CommPort_t emPort )
{
    bool_t bSwitch = false;
    uint8_t ucIndex = 0U;
    PortMstrInfo_t *pstPortMstrInfo = NULL;

    if((emPort >= PORT_COM1) && (emPort <= PORT_NET2))
    {
        /* Port Configuration */
        if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
        {
            /* Serial: COM */
            ucIndex = emPort - PORT_COM1;
            bSwitch = HandleCOMConfig( COM1 + ucIndex, &s_stSerlCfgInfo[ucIndex]);
            if( bSwitch )
            {
                DEBUG_MB( "Switch COM[%d] Cfg\n", ucIndex+1 );
                HandleCOMCfgChg( COM1 + ucIndex, &s_stSerlCfgInfo[ucIndex]);
            }
        }
        else
        {
            /* Network: Modbus TCP */
            ucIndex = emPort - PORT_NET1;
            bSwitch = HandleModbusTCPConfig( NET1 + ucIndex, &s_stMBTCPInfo[ucIndex]);
            if( bSwitch )
            {
                DEBUG_MB( "Switch Net[%d] MBTCP Cfg: \n", ucIndex+1 );

#ifdef DEBUG_MODBUS
                /* Debug */
                ModbusTCPConfigInfo_t *pMBTCPCfg = &( s_stMBTCPInfo[ucIndex].stMBTCPConfig[s_stMBTCPInfo[ucIndex].ucIndex]);
                DEBUG_MB( "Proto: %d, FPOrder: %d\n", pMBTCPCfg->ucProtocol, pMBTCPCfg->ucFPOrder );
#endif
            }
        }

        if( bSwitch )
        {
            /* Update Port Master Info */
            pstPortMstrInfo = &( s_stPortMstrInfo[emPort]);
            pstPortMstrInfo->uiBlockIndex = 0U;
            pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
            pstPortMstrInfo->emState = IDLE_STATE;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortFPOrder
*
* 功能描述: 获取端口的FP Order 属性
*
* 输入参数: emPort：端口号
*
* 输出参数: 无
*
* 返 回 值: 端口的FP Order 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetPortFPOrder( CommPort_t emPort )
{
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    uint8_t ucIndex = 0U;

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
    {
        ucIndex = emPort - PORT_COM1;
        ucFPOrder = GetCOMFPOrder( &( s_stSerlCfgInfo[ucIndex]));
    }
    else if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
    {
        ucIndex = emPort - PORT_NET1;
        ucFPOrder = GetNetFPOrder( &( s_stMBTCPInfo[ucIndex]));
    }
    else
    {
        ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    }

    return ucFPOrder;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortWriteEnable
*
* 功能描述: 获取端口的Port Write Enable 属性
*
* 输入参数: emPort：端口号
*
* 输出参数: 无
*
* 返 回 值: 端口的Port Write Enable 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetPortWriteEnable( CommPort_t emPort )
{
    bool_t bEnable = false;
    uint8_t ucIndex = 0U;

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
    {
        ucIndex = emPort - PORT_COM1;
        bEnable = GetCOMPortWriteEnable( &( s_stSerlCfgInfo[ucIndex]));
    }
    else if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
    {
        ucIndex = emPort - PORT_NET1;
        bEnable = GetNetPortWriteEnable( &( s_stMBTCPInfo[ucIndex]));
    }
    else
    {
        bEnable = false;
    }

    return bEnable;
}

/*
***********************************************************************************************************************
* 函数名称: GetPortMBSlaveID
*
* 功能描述: 获取端口的Slave ID 属性
*
* 输入参数: emPort：端口号
*
* 输出参数: 无
*
* 返 回 值: 端口的Slave ID 属性，0代表无效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetPortMBSlaveID( CommPort_t emPort )
{
    uint8_t ucSlaveID = 0U;
    uint8_t ucIndex = 0U;

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
    {
        ucIndex = emPort - PORT_COM1;
        ucSlaveID = GetCOMMBSlaveID( &( s_stSerlCfgInfo[ucIndex]));
    }
    else
    {
        ucSlaveID = 0U;
    }

    return ucSlaveID;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPConfig
*
* 功能描述: 检查配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: emNet：网口号；
*          pstMBTCPInfo：指向网口对应的Modbus TCP配置信息；
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleModbusTCPConfig( NetworkPortType_t emNet, MBTCPCfgInfo_t *pstMBTCPInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    ModbusTCPConfigInfo_t stMBTCPCfgInfo;

    if(((emNet >= NET1) && (emNet < NUM_OF_NETWORK_PORT)) &&
       (pstMBTCPInfo != NULL))
    {
        /* RTData Version */
        /*if( HandleRTDataVer( &( pstMBTCPInfo->stRTDataVer )))*/
        {
            /* Get CM Configuration Version */
            if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
            {
                /* Compare Version */
                if((uiCRC != pstMBTCPInfo->uiCMCfgCRC) ||
                   (uiFileVer != pstMBTCPInfo->uiCMCfgVer))
                {
                    /* Copy To Local */
                    if( SysGetModbusTCPConfigInfo( emNet, &stMBTCPCfgInfo ))
                    {
                        /* Check Active */
                        if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                           ( ACTIVE == stMBTCPCfgInfo.emActive ))
                        {
                            /* CM always exist: Switch when stMBTCPCfgInfo.bUpdate is true */
                            /* Online download and then change CM: stMBTCPCfgInfo.bUpdate true/false, Local ver is 0 */
                            if( stMBTCPCfgInfo.bUpdate ||
                                ((0U == pstMBTCPInfo->uiCMCfgCRC ) && (0U == pstMBTCPInfo->uiCMCfgVer)))
                            {
                                /* Check Valid */
                                if( CheckModbusTCPCfgInfo( &stMBTCPCfgInfo ))
                                {
                                    /* OPC does not have Update Flag: Switch directly */
                                    if( SwitchModbusTCPCfgInfo( pstMBTCPInfo, &stMBTCPCfgInfo ))
                                    {
                                        bSwitch = true;
                                        pstMBTCPInfo->uiCMCfgCRC = uiCRC;
                                        pstMBTCPInfo->uiCMCfgVer = uiFileVer;
                                    }
                                }
                                else
                                {
                                    pstMBTCPInfo->uiCMCfgCRC = uiCRC;
                                    pstMBTCPInfo->uiCMCfgVer = uiFileVer;
                                    DEBUG_MB( "Net[%d] MBTCPCfg Invalid\n", emNet+1 );
                                }
                            }
                        }
                    }
                }
            }/* end of SysGetCMConfigVersion */
        }/* end of HandleRTDataVer */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusTCPCfgInfo
*
* 功能描述: 检查网口ModbusTCP配置信息是否有效
*
* 输入参数: pstMBTCPCfgInfo：指向待切换的网口ModbusTCP配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusTCPCfgInfo( ModbusTCPConfigInfo_t const *pstMBTCPCfgInfo )
{
    bool_t bValid = false;

    if( pstMBTCPCfgInfo != NULL )
    {
        if( ACTIVE == pstMBTCPCfgInfo->emActive )
        {
            bValid = true;

            /* Protocol */
            if((pstMBTCPCfgInfo->ucProtocol < (uint8_t)(MODBUS_TCP_NONE)) ||
               (pstMBTCPCfgInfo->ucProtocol > (uint8_t)(MODBUS_TCP_ALL)))
            {
                bValid = false;
            }

            /* Master Logic Port */
            if( bValid )
            {
                if((pstMBTCPCfgInfo->ucMasterLogicPort < (uint8_t)(MODBUS_MLP_MIN)) ||
                   (pstMBTCPCfgInfo->ucMasterLogicPort > (uint8_t)(MODBUS_MLP_MAX)))
                {
                    bValid = false;
                }
            }

            /* FP Order */
            if( bValid )
            {
                if(( pstMBTCPCfgInfo->ucFPOrder != (uint8_t)(FP_ORDER_H)) &&
                   ( pstMBTCPCfgInfo->ucFPOrder != (uint8_t)(FP_ORDER_L)))
                {
                    bValid = false;
                }
            }

            /* Port Number */
            if( bValid )
            {
                if( pstMBTCPCfgInfo->usTCPPortNum != (uint16_t)(PORTNUM_MODBUS))
                {
                    bValid = false;
                }
            }
        }/* end of if - Active */
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchModbusTCPCfgInfo
*
* 功能描述: 切换网口配置信息
*
* 输入参数: pstMBTCPInfo：指向本地保存的网口ModbusTCP信息；
*          pstMBTCPCfgInfo：指向待切换的网口ModbusTCP配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchModbusTCPCfgInfo( MBTCPCfgInfo_t *pstMBTCPInfo, ModbusTCPConfigInfo_t const *pstMBTCPCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    ModbusTCPConfigInfo_t *pstDstMBTCPCfgInfo = NULL;

    if((pstMBTCPInfo != NULL) &&
       (pstMBTCPCfgInfo != NULL))
    {
        ucCfgIndex = pstMBTCPInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1 ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstMBTCPCfgInfo = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstMBTCPCfgInfo, pstMBTCPCfgInfo, sizeof(ModbusTCPConfigInfo_t));

            /* Update Index */
            pstMBTCPInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurMBTCPCfg
*
* 功能描述: 获取某一网口的Modbus TCP配置信息
*
* 输入参数: pstMBTCPInfo：指向Modbus TCP信息
*
* 输出参数: 无
*
* 返 回 值: 网口的Modbus TCP配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static ModbusTCPConfigInfo_t const* GetCurMBTCPCfg( MBTCPCfgInfo_t const *pstMBTCPInfo )
{
    uint8_t ucCfgIndex = 0U;
    ModbusTCPConfigInfo_t const *pstMBTCPCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        ucCfgIndex = pstMBTCPInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstMBTCPCfg = &( pstMBTCPInfo->stMBTCPConfig[ucCfgIndex]);
        }
    }

    return pstMBTCPCfg;
}

/*
***********************************************************************************************************************
* 函数名称: GetNetFPOrder
*
* 功能描述: 获取某一网口的FP Order 属性
*
* 输入参数: pstMBTCPInfo：指向Modbus TCP信息
*
* 输出参数: 无
*
* 返 回 值: 网口的FP Order 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetNetFPOrder( MBTCPCfgInfo_t const *pstMBTCPInfo )
{
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    ModbusTCPConfigInfo_t const *pstMBTCPCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        pstMBTCPCfg = GetCurMBTCPCfg( pstMBTCPInfo );

        if((pstMBTCPCfg != NULL) &&
           (ACTIVE == pstMBTCPCfg->emActive ))
        {
            if( pstMBTCPCfg->ucProtocol != (uint8_t)(MODBUS_TCP_NONE))
            {
                ucFPOrder = pstMBTCPCfg->ucFPOrder;
            }
        }
    }

    return ucFPOrder;
}

/*
***********************************************************************************************************************
* 函数名称: GetNetPortWriteEnable
*
* 功能描述: 获取某一网口的Port Write Enable 属性
*
* 输入参数: pstMBTCPInfo：指向Modbus TCP信息
*
* 输出参数: 无
*
* 返 回 值: 网口的Port Write Enable 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetNetPortWriteEnable( MBTCPCfgInfo_t const *pstMBTCPInfo )
{
    bool_t bEnable = false;
    ModbusTCPConfigInfo_t const *pstMBTCPCfg = NULL;

    if( pstMBTCPInfo != NULL )
    {
        pstMBTCPCfg = GetCurMBTCPCfg( pstMBTCPInfo );

        if((pstMBTCPCfg != NULL) &&
           (ACTIVE == pstMBTCPCfg->emActive ))
        {
            if( pstMBTCPCfg->ucProtocol != (uint8_t)(MODBUS_TCP_NONE))
            {
                bEnable = pstMBTCPCfg->bPortWriteEnable;
            }
        }
    }

    return bEnable;
}

/*
***********************************************************************************************************************
* 函数名称: HandleCOMConfig
*
* 功能描述: 检查配置参数版本是否变化，若变化，则拷贝并切换，然后根据变化情况进行相应的处理
*
* 输入参数: emSerl：串口号；
*          pstCOMInfo：指向串口对应的配置信息；
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t HandleCOMConfig( SerialPortType_t emSerl, SerlCfgInfo_t *pstCOMInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    COMConfigInfo_t stCOMCfgInfo;

    if(((emSerl >= COM1) && (emSerl < NUM_OF_SERIAL_PORT)) &&
       (pstCOMInfo != NULL))
    {
        /* RTData Version */
        /*if( HandleRTDataVer( &( pstCOMInfo->stRTDataVer )))*/
        {
            /* Get CM Configuration Version */
            if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
            {
                /* Compare Version */
                if((uiCRC != pstCOMInfo->uiCMCfgCRC) ||
                   (uiFileVer != pstCOMInfo->uiCMCfgVer))
                {
                    /* Copy To Local */
                    if( SysGetCOMConfigInfo( emSerl, &stCOMCfgInfo ))
                    {
                        /* Check Active */
                        if(( ACTIVE == SysGetCMConfigInfoActiveFlag()) &&
                           ( ACTIVE == stCOMCfgInfo.emActive ))
                        {
                            /* CM always exist: Switch when stCOMCfgInfo.bUpdate is true */
                            /* Online download and then change CM: stCOMCfgInfo.bUpdate true/false, Local ver is 0 */
                            if( stCOMCfgInfo.bUpdate ||
                                ((0U == pstCOMInfo->uiCMCfgCRC ) && (0U == pstCOMInfo->uiCMCfgVer)))
                            {
                                /* Check Valid */
                                if( CheckCOMCfgInfo( &stCOMCfgInfo, emSerl ))
                                {
                                    if( SwitchCOMCfgInfo( pstCOMInfo, &stCOMCfgInfo ))
                                    {
                                        bSwitch = true;
                                        pstCOMInfo->uiCMCfgCRC = uiCRC;
                                        pstCOMInfo->uiCMCfgVer = uiFileVer;
                                    }
                                }
                                else
                                {
                                    pstCOMInfo->uiCMCfgCRC = uiCRC;
                                    pstCOMInfo->uiCMCfgVer = uiFileVer;
                                    DEBUG_MB( "COM[%d] Cfg Invalid\n", emSerl+1 );
                                }
                            }
                        }
                    }
                }
            }/* end of SysGetCMConfigVersion */
        }/* end of HandleRTDataVer */
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
* 函数名称: CheckCOMCfgInfo
*
* 功能描述: 检查串口配置信息是否有效
*
* 输入参数: pstCOMCfgInfo：指向待切换的串口配置信息
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckCOMCfgInfo( COMConfigInfo_t const *pstCOMCfgInfo, SerialPortType_t emSerl )
{
    bool_t bValid = false;
    uint8_t ucProtoMin = (uint8_t)(SPP_NONE);
    uint8_t ucProtoMax = (uint8_t)(SPP_GPS);

    if((pstCOMCfgInfo != NULL) &&
       ((emSerl >= COM1) && (emSerl < NUM_OF_SERIAL_PORT)))
    {
        if( ACTIVE == pstCOMCfgInfo->emActive )
        {
            bValid = true;  /* COM Config Must Active */

            /* Protocol */
            if( COM4 == emSerl )
            {
                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) ||
                    (pstCOMCfgInfo->ucProtocol > ucProtoMax)))
                {
                    bValid = false;
                }
            }
            else if( COM3 == emSerl )
            {
                ucProtoMax = (uint8_t)(MODBUS_SLAVE_ASCII);

                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) ||
                    (pstCOMCfgInfo->ucProtocol > ucProtoMax)))
                {
                    bValid = false;
                }
            }
            else if( COM2 == emSerl )
            {
                ucProtoMax = (uint8_t)(SPP_PULSE_RPT);

                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) || (pstCOMCfgInfo->ucProtocol > ucProtoMax)) ||
                   ((uint8_t)(SPP_PULSE_SRC) == pstCOMCfgInfo->ucProtocol))
                {
                    bValid = false;
                }
            }
            else
            {
                ucProtoMax = (uint8_t)(SPP_PULSE_SRC);

                if(((pstCOMCfgInfo->ucProtocol < ucProtoMin) ||
                    (pstCOMCfgInfo->ucProtocol > ucProtoMax)))
                {
                    bValid = false;
                }
            }

            /* FP Order */
            if( bValid )
            {
                if(( pstCOMCfgInfo->ucFPOrder != (uint8_t)(FP_ORDER_H)) &&
                   ( pstCOMCfgInfo->ucFPOrder != (uint8_t)(FP_ORDER_L)))
                {
                    bValid = false;
                }
            }

            /* Baud Rate */
            if( bValid )
            {
                if(((pstCOMCfgInfo->ucBaudRate < (uint8_t)(BR1200)) ||
                    (pstCOMCfgInfo->ucBaudRate > (uint8_t)(BR115200))))
                {
                    bValid = false;
                }
            }

            /* Parity */
            if( bValid )
            {
                if(((pstCOMCfgInfo->ucParity < (uint8_t)(PARITY_NONE)) ||
                    (pstCOMCfgInfo->ucParity > (uint8_t)(PARITY_EVEN))))
                {
                    bValid = false;
                }
            }

            /* TransMode */
            if( bValid )
            {
                if((pstCOMCfgInfo->ucTransMode != TM_RS485) &&
                   (pstCOMCfgInfo->ucTransMode != TM_RS422))
                {
                    bValid = false;
                }
            }

            /* Stop Bits */
            if( bValid )
            {
                if(( pstCOMCfgInfo->ucStopBits != (uint8_t)(SB1)) &&
                   ( pstCOMCfgInfo->ucStopBits != (uint8_t)(SB2)))
                {
                    bValid = false;
                }
            }

            /* Logic */
            /* Slave ID, Logic Port */
            if( bValid )
            {
                if(((uint8_t)(MODBUS_SLAVE_RTU) == pstCOMCfgInfo->ucProtocol) ||
                   ((uint8_t)(MODBUS_SLAVE_ASCII) == pstCOMCfgInfo->ucProtocol))
                {
                    /* Slave ID */
                    if((pstCOMCfgInfo->ucModbusSlaveAddr < (uint8_t)(MIN_MODBUS_SLAVE_ID)) ||
                       (pstCOMCfgInfo->ucModbusSlaveAddr > (uint8_t)(MAX_MODBUS_SLAVE_ID)))
                    {
                        DEBUG_MB( "COM[%d]: SlaveID-%d Invalid!\n", emSerl+1, pstCOMCfgInfo->ucModbusSlaveAddr );
                        bValid = false;
                    }
                }
                else
                {
                    bValid = true;
                }
            }

            /* Data Bits */
            if( bValid )
            {
                if(((uint8_t)(MODBUS_MASTER_ASCII) == pstCOMCfgInfo->ucProtocol) ||
                   ((uint8_t)(MODBUS_SLAVE_ASCII) == pstCOMCfgInfo->ucProtocol))
                {
                    if( pstCOMCfgInfo->ucDataBits != (uint8_t)(DB7))
                    {
                        DEBUG_MB( "COM[%d]: ASCII DB != DB7\n", emSerl+1 );
                        bValid = false;
                    }
                }
                else if(((uint8_t)(MODBUS_MASTER_RTU) == pstCOMCfgInfo->ucProtocol) ||
                        ((uint8_t)(MODBUS_SLAVE_RTU) == pstCOMCfgInfo->ucProtocol))
                {
                    if( pstCOMCfgInfo->ucDataBits != (uint8_t)(DB8))
                    {
                        DEBUG_MB( "COM[%d]: RTU DB != DB8\n", emSerl+1 );
                        bValid = false;
                    }
                }
                else
                {
                    bValid = true;
                }
            }

            /* StopBits - Parity */
            if( bValid )
            {
                if((pstCOMCfgInfo->ucProtocol >= (uint8_t)(MODBUS_MASTER_RTU)) &&
                   (pstCOMCfgInfo->ucProtocol >= (uint8_t)(MODBUS_SLAVE_ASCII)))
                {
                    if((uint8_t)(PARITY_NONE) == pstCOMCfgInfo->ucParity )
                    {
                        if( pstCOMCfgInfo->ucStopBits != (uint8_t)(SB2))
                        {
                            DEBUG_MB( "COM[%d]: Parity None, SB != SB2\n", emSerl+1 );
                            bValid = false;
                        }
                    }
                    else
                    {
                        if( pstCOMCfgInfo->ucStopBits != (uint8_t)(SB1))
                        {
                            DEBUG_MB( "COM[%d]: Parity Odd/Even SB != SB1\n", emSerl+1 );
                            bValid = false;
                        }
                    }
                }
            }
        }/* end of if - Active */
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchCOMCfgInfo
*
* 功能描述: 切换串口配置信息
*
* 输入参数: pstCOMInfo：指向本地保存的串口信息；
*          pstCOMCfgInfo：指向待切换的串口配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchCOMCfgInfo( SerlCfgInfo_t *pstCOMInfo, COMConfigInfo_t const *pstCOMCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    COMConfigInfo_t *pstDstMBTCPCfgInfo = NULL;

    if((pstCOMInfo != NULL) &&
       (pstCOMCfgInfo != NULL))
    {
        ucCfgIndex = pstCOMInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstMBTCPCfgInfo = &( pstCOMInfo->stCOMConfig[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstMBTCPCfgInfo, pstCOMCfgInfo, sizeof(COMConfigInfo_t));

            /* Update Index */
            pstCOMInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: HandleCOMCfgChg
*
* 功能描述: 串口配置参数变化时，如果协议为Modbus，则重新配置串口
*
* 输入参数: emSerl：串口号；
*          pstCOMInfo：指向串口对应的配置信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleCOMCfgChg( SerialPortType_t emSerl, SerlCfgInfo_t *pstCOMInfo )
{
    uint8_t ucCfgIndex = 0U;  /* Current Configuration Index */
    COMConfigInfo_t const *pstNewCfg = NULL;

    if(((emSerl >= COM1) && (emSerl < NUM_OF_SERIAL_PORT)) &&
       (pstCOMInfo != NULL))
    {
        ucCfgIndex = pstCOMInfo->ucIndex;  /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Point to New Configuration */
            pstNewCfg = &( pstCOMInfo->stCOMConfig[ucCfgIndex]);

            if((pstNewCfg->ucProtocol >= (uint8_t)(MODBUS_MASTER_RTU)) &&
               (pstNewCfg->ucProtocol <= (uint8_t)(MODBUS_SLAVE_ASCII)))
            {
                /* Set Serial Port */
                SetSerlPort( emSerl );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetCurCOMCfg
*
* 功能描述: 获取某一串口的当前配置信息
*
* 输入参数: pstCOMInfo：指向串口信息
*
* 输出参数: 无
*
* 返 回 值: 串口的当前配置信息，NULL代表无有效配置信息
*
* 注意事项: 无
***********************************************************************************************************************
*/

static COMConfigInfo_t const* GetCurCOMCfg( SerlCfgInfo_t const *pstCOMInfo )
{
    uint8_t ucCfgIndex = 0U;
    COMConfigInfo_t const *pstCOMCfg = NULL;

    if( pstCOMInfo != NULL )
    {
        ucCfgIndex = pstCOMInfo->ucIndex;
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            pstCOMCfg = &( pstCOMInfo->stCOMConfig[ucCfgIndex]);
        }
    }

    return pstCOMCfg;
}

/*
***********************************************************************************************************************
* 函数名称: GetCOMFPOrder
*
* 功能描述: 获取某一串口的FP Order 属性
*
* 输入参数: pstCOMInfo：指向串口信息
*
* 输出参数: 无
*
* 返 回 值: 串口的FP Order 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetCOMFPOrder( SerlCfgInfo_t const *pstCOMInfo )
{
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    COMConfigInfo_t const *pstCOMCfg = NULL;

    if( pstCOMInfo != NULL )
    {
        pstCOMCfg = GetCurCOMCfg( pstCOMInfo );

        if((pstCOMCfg != NULL) &&
           (ACTIVE == pstCOMCfg->emActive ))
        {
            if((pstCOMCfg->ucProtocol >= (uint8_t)(MODBUS_MASTER_RTU)) &&
               (pstCOMCfg->ucProtocol <= (uint8_t)(MODBUS_SLAVE_ASCII)))
            {
                ucFPOrder = pstCOMCfg->ucFPOrder;
            }
        }
    }

    return ucFPOrder;
}

/*
***********************************************************************************************************************
* 函数名称: GetCOMPortWriteEnable
*
* 功能描述: 获取某一串口的Port Write Enable 属性
*
* 输入参数: pstCOMInfo：指向串口信息
*
* 输出参数: 无
*
* 返 回 值: 串口的Port Write Enable 属性
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetCOMPortWriteEnable( SerlCfgInfo_t const *pstCOMInfo )
{
    bool_t bEnable = false;
    COMConfigInfo_t const *pstCOMCfg = NULL;

    if( pstCOMInfo != NULL )
    {
        pstCOMCfg = GetCurCOMCfg( pstCOMInfo );

        if((pstCOMCfg != NULL) &&
           (ACTIVE == pstCOMCfg->emActive ))
        {
            if((pstCOMCfg->ucProtocol >= (uint8_t)(MODBUS_MASTER_RTU)) &&
               (pstCOMCfg->ucProtocol <= (uint8_t)(MODBUS_SLAVE_ASCII)))
            {
                bEnable = pstCOMCfg->bPortWriteEnable;
            }
        }
    }

    return bEnable;
}

/*
***********************************************************************************************************************
* 函数名称: GetCOMMBSlaveID
*
* 功能描述: 获取某一串口的Slave ID 属性
*
* 输入参数: pstCOMInfo：指向串口信息
*
* 输出参数: 无
*
* 返 回 值: 串口的Slave ID 属性，0代表无效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetCOMMBSlaveID( SerlCfgInfo_t const *pstCOMInfo )
{
    uint8_t ucSlaveID = 0U;
    COMConfigInfo_t const *pstCOMCfg = NULL;

    if( pstCOMInfo != NULL )
    {
        pstCOMCfg = GetCurCOMCfg( pstCOMInfo );

        if((pstCOMCfg != NULL) &&
           (ACTIVE == pstCOMCfg->emActive ))
        {
            if(((uint8_t)(MODBUS_SLAVE_RTU) == pstCOMCfg->ucProtocol) ||
               ((uint8_t)(MODBUS_SLAVE_ASCII) == pstCOMCfg->ucProtocol))
            {
                ucSlaveID = pstCOMCfg->ucModbusSlaveAddr;
            }
        }
    }

    return ucSlaveID;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCP
*
* 功能描述: 处理与网口相关的配置参数，收发Modbus TCP数据
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusTCP( void )
{
    uint8_t ucNetIndex = 0U;
    CommPort_t emPort = PORT_NET1;
    bool_t bHandleCfg = false;
    ModbusTCPConfigInfo_t const *pstMBTCPCfg = NULL;

    /* Loop Network Port */
    for( ucNetIndex = 0U; ucNetIndex < (uint8_t)(NUM_OF_NETWORK_PORT); ucNetIndex++ )
    {
        emPort = (CommPort_t)((uint8_t)PORT_NET1 + ucNetIndex);

        /* Get Modbus TCP Configuration */
        pstMBTCPCfg = GetCurMBTCPCfg( &( s_stMBTCPInfo[ucNetIndex]));
        if( pstMBTCPCfg != NULL )
        {
            /* Handle Configuration Flag */
            if(((uint8_t)(MODBUS_TCP_NONE) == pstMBTCPCfg->ucProtocol) ||
               ((uint8_t)(MODBUS_TCP_SLAVE) == pstMBTCPCfg->ucProtocol))
            {
                bHandleCfg = true;
            }
            else
            {
                /* MASTER / Master & Slave */
                if( IDLE_STATE == s_stPortMstrInfo[emPort].emState )
                {
                    bHandleCfg = true;
                }
            }

            /* Handle Port Configuration */
            if( bHandleCfg )
            {
                HandlePortConfig( emPort );

                /* Update pstMBTCPCfg */
                pstMBTCPCfg = GetCurMBTCPCfg( &( s_stMBTCPInfo[ucNetIndex]));
            }

            if((pstMBTCPCfg != NULL) &&
               (ACTIVE == pstMBTCPCfg->emActive))
            {
                if( pstMBTCPCfg->ucProtocol != (uint8_t)(MODBUS_TCP_NONE))
                {
                    if((uint8_t)(MODBUS_TCP_MASTER) == pstMBTCPCfg->ucProtocol )
                    {
                        HandleModbusTCPMaster( emPort );
                    }
                    else if((uint8_t)(MODBUS_TCP_SLAVE) == pstMBTCPCfg->ucProtocol )
                    {
                        HandleModbusTCPSlave();
                    }
                    else  /* Master & Slave */
                    {
                        HandleModbusTCPMaster( emPort );
                        HandleModbusTCPSlave();
                    }
                }
            }
            else
            {
                SysSetCM2PMModbusState(emPort, 0);
                ucModbusOnlineCnt[emPort] = 0;
            }
        }
        else
        {
            DEBUG_MB( "GetCurMBTCPCfg Failed\n" );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: SendMBDataToPM
*
* 功能描述: 将数据发往选取的PM模块
*
* 输入参数: pucPMData：待发送数据的首地址；
*          usPMDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SendMBDataToPM( uint8_t const pucPMData[], uint16_t usPMDataLen )
{
    SelPMID_t emSelPMID = SELPM_NONE;

    if((pucPMData != NULL) &&
       (usPMDataLen > 0))
    {
        /* Select PM */
        emSelPMID = SelectPM();
        if( emSelPMID != SELPM_NONE )
        {
            /* Write To PM */
            if( SysSetModbusMsgQueue( SwitchPMID(emSelPMID), pucPMData, usPMDataLen ))
            {
                DEBUG_MB( "SendMBDataToPM-%d OK\n", emSelPMID );
            }
            else
            {
                DEBUG_MB( "SendMBDataToPM-%d Failed\n", emSelPMID );
            }
        }
        else
        {
            DEBUG_MB( "SendMBDataToPM: CM_BUS Err\n" );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: GetModbusVarAddr
*
* 功能描述: 获取Modbus变量的首地址
*
* 输入参数: emSelPM：使用此系的实时数据；
*          pstMBVar：指向Modbus变量结构体。
*
* 输出参数: 无
*
* 返 回 值: Modbus变量的首地址，NULL表示存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t const* GetModbusVarAddr( PMController_t emSelPM, ModbusVar_t const *pstMBVar )
{
    uint32_t uiOffset = 0U;
    uint8_t const *pucAreaAddr = NULL;
    uint8_t const *pucVarAddr = NULL;

    if((emSelPM < NUM_OF_PM) &&
       (pstMBVar != NULL))
    {
        pucAreaAddr = SysGetRTDataAddr( emSelPM, (SysRtDataAreaType_t)pstMBVar->ucRefID );
        if( pucAreaAddr != NULL )
        {
            /* Offset */
            if( 0U == pstMBVar->usVarSize )
            {
                /* Offset: Bit */
                uiOffset = pstMBVar->uiOffset / 8U;
            }
            else
            {
                /* Offset: Byte */
                uiOffset = pstMBVar->uiOffset;
            }

            pucVarAddr = &pucAreaAddr[uiOffset];
        }
        else
        {
            DEBUG_MB( "SysGetRTDataAddr Err\n" );
        }
    }

    return pucVarAddr;
}

/*
***********************************************************************************************************************
* 函数名称: GetMBBOOLValue
*
* 功能描述: 获取Modbus BOOL变量的值
*
* 输入参数: emSelPM：使用此系PM的实时数据；
*          pstMBVar：指向Modbus变量结构体。
*
* 输出参数: pbValue：输出BOOL变量值
*
* 返 回 值: 是否获取成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t GetMBBOOLValue( PMController_t emSelPM, ModbusVar_t const *pstMBVar, bool_t *pbValue )
{
    bool_t bResult = false;
    uint8_t const *pucVarAddr = NULL;
    uint8_t ucValue = 0U;

    if((emSelPM < NUM_OF_PM) &&
       (pstMBVar != NULL) &&
       (pbValue != NULL))
    {
        pucVarAddr = GetModbusVarAddr( emSelPM, pstMBVar );
        if( pucVarAddr != NULL )
        {
            bResult = true;

            /* Set Value */
            if( 0U == pstMBVar->usVarSize )
            {
                /* Offset: Bit */
                ucValue = *pucVarAddr;
                if((ucValue & (0x01U << (pstMBVar->uiOffset % 8U))) != 0U )
                {
                    *pbValue = true;
                }
                else
                {
                    *pbValue = false;
                }
            }
            else if( 1U == pstMBVar->usVarSize )
            {
                *pbValue = *pucVarAddr;  /* Byte */
            }
            else
            {
                bResult = false;
                DEBUG_MB( "BOOLVarSize %d != 0/1.\n", pstMBVar->usVarSize );
            }
        }
        else
        {
            DEBUG_MB( "GetModbusVarAddr Failed\n" );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: AddCoilsReqData
*
* 功能描述: 添加请求线圈相关数据
*
* 输入参数: emSelPM：使用此系的实时数据；
*          pstMBMstrBlk：指向Modbus主站块；
*          pucReqData：指向目标填充区。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddCoilsReqData( PMController_t emSelPM, ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t pucReqData[])
{
    uint8_t ucByteCnts = 0U;
    uint16_t usDataLen = 0U;
    uint16_t usStartAddr = 0U;
    uint16_t usDataIndex = 0U;
    bool_t bCoilValue = false;  /* Coil Value */
    uint8_t ucData = 0U;        /* Data: 8 Coil Value */
    uint8_t ucBitIndex = 0U;    /* Bit Index: 0-7 */
    uint16_t usIndex = 0U;
    uint32_t uiVarIndex = 0U;   /* Variable Index */
    uint8_t const *pucVarAddr = NULL;    /* Variable Address */
    ModbusVar_t const *pstMBVar = NULL;  /* Variable */
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

#if 0
    uint8_t *pucAreaAddr = NULL;
    uint16_t usPIndex = 0U;
#endif

    if((emSelPM < NUM_OF_PM) &&
       (pstMBMstrBlk != NULL) &&
       (pucReqData != NULL))
    {
        pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
        usStartAddr = pstBlkHead->uiTargetBeginAddr;

        /* Number */
        pucReqData[usIndex++] = (uint8_t)((pstBlkHead->uiTargetSize >> 8) & 0xFF);
        pucReqData[usIndex++] = (uint8_t)(pstBlkHead->uiTargetSize & 0xFF);

        /* Byte Count */
        ucByteCnts = (uint8_t)((pstBlkHead->uiTargetSize + 7) / 8);
        pucReqData[usIndex++] = ucByteCnts;
        memset( &pucReqData[usIndex], 0, (size_t)ucByteCnts );

        /* Data Length */
        usDataLen = usIndex + (uint16_t)ucByteCnts;

#if 0
        printf( "G Data: " );
        pucAreaAddr = SysGetRTDataAddr( emSelPM, RTDATA_G_AREA );
        for( usPIndex = 0U; usPIndex < 20; usPIndex++ )
        {
            printf( "%d ", pucAreaAddr[usPIndex]);
        }
        printf( "\n" );
#endif

        /* Coils Value: Low Coil - First transfer, Low bit */
        for( uiVarIndex = 0U; uiVarIndex < pstBlkHead->uiModbusVarNum; uiVarIndex++ )
        {
            pstMBVar = &( pstMBMstrBlk->stModbusVar[uiVarIndex]);  /* Variable */
            pucVarAddr = GetModbusVarAddr( emSelPM, pstMBVar );    /* Variable Address */
            if( pucVarAddr != NULL )
            {
                if( pstMBVar->uiModbusIndex >= (uint32_t)usStartAddr )
                {
                    usDataIndex = usIndex + ((uint16_t)pstMBVar->uiModbusIndex - usStartAddr) / 8;
                    ucBitIndex = (uint8_t)(((uint16_t)pstMBVar->uiModbusIndex - usStartAddr) % 8);  /* Update Bit Index */
                    ucData = pucReqData[usDataIndex];

                    if((0U == pstMBVar->usVarSize) ||
                       (1U == pstMBVar->usVarSize))
                    {
                        /* Set ucData when Coil Value is true */
                        if( GetMBBOOLValue( emSelPM, pstMBVar, &bCoilValue ))
                        {
                            if( bCoilValue )
                            {
                                ucData |= (uint8_t)(0x01 << ucBitIndex);
                            }
                            else
                            {
                                ucData &= ~(uint8_t)(0x01 << ucBitIndex);
                            }

                            pucReqData[usDataIndex] = ucData;
#if 0
                            DEBUG_MB( "AddCoilsReqData: RefID-%d, TaskID-%d, VSize-%d, Offset-%d, MBIndex-%d.\n", \
                                       pstMBVar->ucRefID, pstMBVar->ucTaskID, pstMBVar->usVarSize, pstMBVar->uiOffset, \
                                       pstMBVar->uiModbusIndex );

                            DEBUG_MB( "AddCoilsReqData: SAddr-%d, DIndex-%d, BitIndex-%d, Value-%d!\n", \
                                      usStartAddr, usDataIndex, ucBitIndex, bCoilValue );

                            DEBUG_MB( "AddCoilsReqData: Offset[%d] = pucVarAddr[%d] - pucAreaAddr[%d].\n", \
                                      pucVarAddr-pucAreaAddr, pucVarAddr, pucAreaAddr );
#endif
                        }
                        else
                        {
                            DEBUG_MB( "GetMBBOOLValue Failed\n" );
                            usDataLen = 0U;
                            break;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "AddCoilsReqData-VarIndex[%d]: VarSize %d != 0/1\n", \
                                   uiVarIndex, pstMBVar->usVarSize );
                        usDataLen = 0U;
                        break;
                    }
                }
                else
                {
                    DEBUG_MB( "AddCoilsReqData-VarIndex[%d]: MBVarRgnIndex-%d should >= StartAddr-%d\n", \
                              uiVarIndex, pstMBVar->uiModbusIndex, usStartAddr );
                    usDataLen = 0U;
                    break;
                }
            }
            else
            {
                DEBUG_MB( "AddCoilsReqData-VarIndex[%d]: GetModbusVarAddr Err\n", uiVarIndex );
                usDataLen = 0U;
                break;
            }
        }/* end of for */
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: AddRegsReqData
*
* 功能描述: 添加请求寄存器相关数据
*
* 输入参数: emSelPM：使用此系的实时数据；
*          pstMBMstrBlk：指向Modbus主站块；
*          pucReqData：指向目标填充区。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddRegsReqData( PMController_t emSelPM, ModbusMasterBlock_t const *pstMBMstrBlk,
                                uint8_t pucReqData[], uint8_t ucFPOrder )
{
    uint8_t ucByteCnts = 0U;
    uint16_t usDataLen = 0U;
    uint16_t usStartAddr = 0U;
    uint16_t usDataIndex = 0U;
    uint16_t usIndex = 0U;
    uint32_t uiVarIndex = 0U;            /* Variable Index */
    uint8_t const *pucVarAddr = NULL;    /* Variable Address */
    ModbusVar_t const *pstMBVar = NULL;  /* Variable */
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

    if((emSelPM < NUM_OF_PM) &&
       (pstMBMstrBlk != NULL) &&
       (pucReqData != NULL) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
        usStartAddr = pstBlkHead->uiTargetBeginAddr;

        /* Number */
        pucReqData[usIndex++] = (uint8_t)((pstBlkHead->uiTargetSize >> 8) & 0xFF);
        pucReqData[usIndex++] = (uint8_t)(pstBlkHead->uiTargetSize & 0xFF);

        /* Byte Count */
        ucByteCnts = (uint8_t)(pstBlkHead->uiTargetSize * 2);
        pucReqData[usIndex++] = ucByteCnts;
        memset( &pucReqData[usIndex], 0, (size_t)ucByteCnts );

        /* Data Length */
        usDataLen = usIndex + (uint16_t)ucByteCnts;

        /* Registers Value: Low Alias Value - First transfer */
        for( uiVarIndex = 0U; uiVarIndex < pstBlkHead->uiModbusVarNum; uiVarIndex++ )
        {
            pstMBVar = &( pstMBMstrBlk->stModbusVar[uiVarIndex]);  /* Variable */
            pucVarAddr = GetModbusVarAddr( emSelPM, pstMBVar );    /* Variable Address */
            if( pucVarAddr != NULL )
            {
                if( pstMBVar->uiModbusIndex >= (uint32_t)usStartAddr )
                {
                    usDataIndex = usIndex + ((uint16_t)(pstMBVar->uiModbusIndex) - usStartAddr) * 2;

                    /* Value: 2/4/8 bytes */
                    /* CPU - Big Endian */
                    if( 2U == pstMBVar->usVarSize )
                    {
                        pucReqData[usDataIndex++] = pucVarAddr[0];
                        pucReqData[usDataIndex++] = pucVarAddr[1];
                    }
                    else if( 4U == pstMBVar->usVarSize )
                    {
                        /* H: 0,1,2,3 / L: 2,3,0,1 */
                        if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                        {
                            pucReqData[usDataIndex++] = pucVarAddr[0];
                            pucReqData[usDataIndex++] = pucVarAddr[1];
                            pucReqData[usDataIndex++] = pucVarAddr[2];
                            pucReqData[usDataIndex++] = pucVarAddr[3];
                        }
                        else
                        {
                            pucReqData[usDataIndex++] = pucVarAddr[2];
                            pucReqData[usDataIndex++] = pucVarAddr[3];
                            pucReqData[usDataIndex++] = pucVarAddr[0];
                            pucReqData[usDataIndex++] = pucVarAddr[1];
                        }
                    }
                    else if( 8U == pstMBVar->usVarSize )
                    {
                        /* 8: H: 0-7 / L: 2,3,0,1  6,7,4,5*/
                        if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                        {
                            pucReqData[usDataIndex++] = pucVarAddr[0];
                            pucReqData[usDataIndex++] = pucVarAddr[1];
                            pucReqData[usDataIndex++] = pucVarAddr[2];
                            pucReqData[usDataIndex++] = pucVarAddr[3];

                            pucReqData[usDataIndex++] = pucVarAddr[4];
                            pucReqData[usDataIndex++] = pucVarAddr[5];
                            pucReqData[usDataIndex++] = pucVarAddr[6];
                            pucReqData[usDataIndex++] = pucVarAddr[7];
                        }
                        else
                        {
                            pucReqData[usDataIndex++] = pucVarAddr[2];
                            pucReqData[usDataIndex++] = pucVarAddr[3];
                            pucReqData[usDataIndex++] = pucVarAddr[0];
                            pucReqData[usDataIndex++] = pucVarAddr[1];

                            pucReqData[usDataIndex++] = pucVarAddr[6];
                            pucReqData[usDataIndex++] = pucVarAddr[7];
                            pucReqData[usDataIndex++] = pucVarAddr[4];
                            pucReqData[usDataIndex++] = pucVarAddr[5];
                        }
                    }
                    else
                    {
                        DEBUG_MB( "AddRegsReqData-VarIndex[%d]: VarSize-%d != 2/4/8\n", \
                                   uiVarIndex, pstMBVar->usVarSize );
                        usDataLen = 0U;
                        break;
                    }
                }
                else
                {
                    DEBUG_MB( "AddRegsReqData-VarIndex[%d]: MBVarRgnIndex-%d should >= StartAddr-%d\n", \
                              uiVarIndex, pstMBVar->uiModbusIndex, usStartAddr );
                    usDataLen = 0U;
                    break;
                }
            }
            else
            {
                DEBUG_MB( "AddRegsReqData-VarIndex[%d]: GetModbusVarAddr Err\n", uiVarIndex );
                usDataLen = 0U;
                break;
            }
        }/* end of for */
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: GetModbusFuncCode
*
* 功能描述: 根据访问模式、Modbus数据区及访问大小，返回相应的功能码
*
* 输入参数: pstBlkHead：指向Modbus 主站块的头结构体；
*
* 输出参数: 无
*
* 返 回 值: Modbus功能码，INVALID_MB_FUNCCODE代表入参或配置参数存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint8_t GetModbusFuncCode( ModbusMasterBlockHead_t const *pstBlkHead )
{
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);  /* Modbus Function Code */

    if( pstBlkHead != NULL )
    {
        /*DEBUG_MB( "GetModbusFuncCode: VisitMode-%d, ModbusRgn-%d.\n", \
                   pstBlkHead->ucVisitMode, pstBlkHead->ucModbusRgn );*/

        if((uint8_t)(VM_READ) == pstBlkHead->ucVisitMode )
        {
            /* Read */
            switch( pstBlkHead->ucModbusRgn )
            {
                case (uint8_t)(MR_COIL):
                {
                    ucFuncCode = (uint8_t)(READ_COIL_STATUS);
                }
                break;

                case (uint8_t)(MR_DISCRETE_INPUTS):
                {
                    ucFuncCode = (uint8_t)(READ_INPUT_STATUS);
                }
                break;

                case (uint8_t)(MR_INPUT_REGS):
                {
                    ucFuncCode = (uint8_t)(READ_INPUT_REGS);
                }
                break;

                case (uint8_t)(MR_HOLD_REGS):
                {
                    ucFuncCode = (uint8_t)(READ_HOLD_REGS);
                }
                break;

                default:
                {
                    ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
                }
                break;
            }/* end of switch */
        }
        else
        {
            /* Write */
            switch( pstBlkHead->ucModbusRgn )
            {
                case (uint8_t)(MR_COIL):
                {
                    if( pstBlkHead->uiTargetSize > 1U )
                    {
                        ucFuncCode = (uint8_t)(FORCE_MULT_COILS);
                    }
                    else
                    {
                        ucFuncCode = (uint8_t)(FORCE_SINGLE_COIL);
                    }
                }
                break;

                case (uint8_t)(MR_HOLD_REGS):
                {
                    if( pstBlkHead->uiTargetSize > 1U )
                    {
                        ucFuncCode = (uint8_t)(PRESET_MULT_REGS);
                    }
                    else
                    {
                        ucFuncCode = (uint8_t)(PRESET_SINGLE_REG);
                    }
                }
                break;

                default:
                {
                    ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
                }
                break;
            }/* end of switch */
        }
    }

    return ucFuncCode;
}

/*
***********************************************************************************************************************
* 函数名称: GetModbusReqDataLen
*
* 功能描述: 计算Modbus 请求帧（Modbus TCP/Modbus RTU）数据域的长度：功能码之后的数据
*
* 输入参数: pstBlkHead：指向Modbus主站块头部；
*          ucFuncCode：功能码。
*
* 输出参数: 无
*
* 返 回 值: 数据域的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t GetModbusReqDataLen( ModbusMasterBlockHead_t const *pstBlkHead, uint8_t ucFuncCode )
{
    uint16_t usDataLen = 0U;

    if( pstBlkHead != NULL )
    {
        switch( ucFuncCode )
        {
            case (uint8_t)(READ_COIL_STATUS):
            case (uint8_t)(READ_INPUT_STATUS):
            case (uint8_t)(READ_HOLD_REGS):
            case (uint8_t)(READ_INPUT_REGS):
            {
                usDataLen = 4U;     /* Start Address: 2 bytes, Number: 2 bytes */
            }
            break;

            case (uint8_t)(FORCE_SINGLE_COIL):
            case (uint8_t)(PRESET_SINGLE_REG):
            {
                if( 1U == pstBlkHead->uiTargetSize )
                {
                    usDataLen = 4U;  /* Start Address: 2 bytes, Data: 2 bytes */
                }
                else
                {
                    DEBUG_MB( "FuncCode %d: TargetSize %d != 1\n", ucFuncCode, pstBlkHead->uiTargetSize );
                    usDataLen = 0U;
                }
            }
            break;

            case (uint8_t)(FORCE_MULT_COILS):
            {
                if( pstBlkHead->uiTargetSize > 0U )
                {
                    usDataLen = (uint16_t)(5 + (pstBlkHead->uiTargetSize + 7) / 8);
                }
                else
                {
                    DEBUG_MB( "FuncCode %d: TargetSize %d should > 0\n", ucFuncCode, pstBlkHead->uiTargetSize );
                    usDataLen = 0U;
                }
            }
            break;

            case (uint8_t)(PRESET_MULT_REGS):
            {
                if( pstBlkHead->uiTargetSize > 0U )
                {
                    usDataLen = (uint16_t)(5 + pstBlkHead->uiTargetSize * 2);
                }
                else
                {
                    DEBUG_MB( "FuncCode %d: TargetSize-%d should > 0\n", ucFuncCode, pstBlkHead->uiTargetSize );
                    usDataLen = 0U;
                }
            }
            break;

            default:
            {
                DEBUG_MB( "FuncCode %d should be 1/2/3/4/5/6/15/16.\n", ucFuncCode );
                usDataLen = 0U;
            }
            break;
        }
    }

    return usDataLen;
}

/*
***********************************************************************************************************************
* 函数名称: AddReadReqData
*
* 功能描述: 添加读请求数据
*
* 输入参数: pstBlkHead：指向Modbus主站块头部
*          ucFuncCode：功能码；
*          pucReqData：数据首地址 - 功能码之后的第一字节。
*
* 输出参数: 无
*
* 返 回 值: 添加的字节数，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddReadReqData( ModbusMasterBlockHead_t const *pstBlkHead, uint8_t pucReqData[])
{
    uint16_t usIndex = 0U;
    uint16_t usStartAddr = 0U;
    uint16_t usNumber = 0U;

    if((pstBlkHead != NULL) &&
       (pucReqData != NULL))
    {
        usStartAddr = (uint16_t)pstBlkHead->uiTargetBeginAddr;  /* 0 - 65535 */
        usNumber = (uint16_t)pstBlkHead->uiTargetSize;

        /* Starting Address */
        pucReqData[usIndex++] = (uint8_t)((usStartAddr >> 8) & 0xFF);
        pucReqData[usIndex++] = (uint8_t)(usStartAddr & 0xFF);

        /* Number */
        pucReqData[usIndex++] = (uint8_t)((usNumber >> 8) & 0xFF);
        pucReqData[usIndex++] = (uint8_t)(usNumber & 0xFF);
    }

    return usIndex;
}

/*
***********************************************************************************************************************
* 函数名称: AddWriteReqData
*
* 功能描述: 添加写请求数据
*
* 输入参数: pstMBMstrBlk：指向Modbus主站块；
*          ucFuncCode：功能码；
*          pucReqData：数据首地址；
*          ucFPOrder：端口FP Order属性。
*
* 输出参数: 无
*
* 返 回 值: 添加的字节数，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddWriteReqData( ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucFuncCode,
                                 uint8_t pucReqData[], uint8_t ucFPOrder )
{
    bool_t bCoilValue = false;
    bool_t bCMBus = false;
    uint16_t usIndex = 0U;
    uint16_t usStartAddr = 0U;
    uint16_t usValue = 0U;
    uint16_t usReqDataLen = 0U;
    uint8_t const *pucVarAddr = NULL;
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

    if((pstMBMstrBlk != NULL) &&
       (pucReqData != NULL) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        /* Select PM RT Data */
        if( SelectRTData( &s_emLastPMID, &bCMBus ))
        {
            pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
            usStartAddr = (uint16_t)pstBlkHead->uiTargetBeginAddr;

            /* Starting Address */
            pucReqData[usIndex++] = (uint8_t)((usStartAddr >> 8) & 0xFF);
            pucReqData[usIndex++] = (uint8_t)(usStartAddr & 0xFF);

            switch( ucFuncCode )
            {
                case (uint8_t)(FORCE_SINGLE_COIL):
                {
                    if( 1U == pstBlkHead->uiModbusVarNum )
                    {
                        if( GetMBBOOLValue( s_emLastPMID, &(pstMBMstrBlk->stModbusVar[0]), &bCoilValue ))
                        {
                            /* Value */
                            usValue = (uint16_t)(COIL_OFF);
                            if( bCoilValue )
                            {
                                usValue = (uint16_t)(COIL_ON);
                            }

                            /* Value */
                            pucReqData[usIndex++] = (uint8_t)((usValue >> 8) & 0xFF);
                            pucReqData[usIndex++] = (uint8_t)(usValue & 0xFF);
                        }
                        else
                        {
                            DEBUG_MB( "GetMBBOOLValue Failed\n" );
                            usIndex = 0U;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "MBVarNum %d != 1\n", pstBlkHead->uiModbusVarNum );
                        usIndex = 0U;
                    }
                }
                break;

                case (uint8_t)(PRESET_SINGLE_REG):
                {
                    if( 1U == pstBlkHead->uiModbusVarNum )
                    {
                        pucVarAddr = GetModbusVarAddr( s_emLastPMID, &( pstMBMstrBlk->stModbusVar[0]));
                        if( pucVarAddr != NULL )
                        {
                            /* Value: CPU - Big Endian */
                            pucReqData[usIndex++] = pucVarAddr[0];
                            pucReqData[usIndex++] = pucVarAddr[1];
                        }
                        else
                        {
                            DEBUG_MB( "GetModbusVarAddr Failed!\n" );
                            usIndex = 0U;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "MBVarNum %d != 1\n", pstBlkHead->uiModbusVarNum );
                        usIndex = 0U;
                    }
                }
                break;

                case (uint8_t)(FORCE_MULT_COILS):
                {
                    usReqDataLen = AddCoilsReqData( s_emLastPMID, pstMBMstrBlk, &pucReqData[usIndex] );
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;
                    }
                    else
                    {
                        DEBUG_MB( "AddCoilsReqData Failed\n" );
                        usIndex = 0U;
                    }
                }
                break;

                case (uint8_t)(PRESET_MULT_REGS):
                {
                    usReqDataLen = AddRegsReqData( s_emLastPMID, pstMBMstrBlk, &pucReqData[usIndex], ucFPOrder );
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;
                    }
                    else
                    {
                        DEBUG_MB( "AddRegsReqData Failed\n" );
                        usIndex = 0U;
                    }
                }
                break;

                default:
                {
                    DEBUG_MB( "FuncCode %d should be 5/6/15/16\n", ucFuncCode );
                    usIndex = 0U;
                }
                break;
            }
        }
    }

    return usIndex;
}



/*
***********************************************************************************************************************
* 函数名称: CkeckMBRespFuncCode
*
* 功能描述: 检查 Modbus 从站应答帧的功能码是否有效
*
* 输入参数: pucRespFunc：Modbus应答帧功能码地址；
*          ucReqFuncCode：Modbus 请求帧的功能码。
*
* 输出参数: 无
*
* 返 回 值: 功能码是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CkeckMBRespFuncCode( uint8_t const pucRespFunc[], uint8_t ucReqFuncCode )
{
    bool_t bValid = false;
    uint8_t ucRespFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);

    if( pucRespFunc != NULL )
    {
        bValid = true;
        ucRespFuncCode = pucRespFunc[0];

        if( ucRespFuncCode != ucReqFuncCode )
        {
            bValid = false;

            /* Check Exception Response */
            if((ucRespFuncCode & (uint8_t)(BIT_8)) != 0U )
            {
                DEBUG_MB( "Except Resp: FuncCode %d, Code %d\n", ucRespFuncCode, pucRespFunc[1]);
            }
            else
            {
                DEBUG_MB( "FuncCode %d != Req %d\n", ucRespFuncCode, ucReqFuncCode );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckMBRespData
*
* 功能描述: 检查Modbus 从站应答帧的数据是否有效
*
* 输入参数: ucFuncCode：功能码；
*          usRespDataLen：应答帧中的数据域长度；
*          pucRespData：Modbus 应答帧数据首地址；
*          pucReqData：Modbus 请求帧数据首地址。
*
* 输出参数: 无
*
* 返 回 值: 应答帧数据是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckMBRespData( uint8_t ucFuncCode, uint16_t usRespDataLen,
                               uint8_t const pucRespData[], uint8_t const pucReqData[])
{
    bool_t bValid = false;
    uint16_t usIndex = 0U;

    if((pucRespData != NULL) &&
       (pucReqData != NULL))
    {
        switch( ucFuncCode )  /* Function Code */
        {
            case (uint8_t)(READ_COIL_STATUS):
            case (uint8_t)(READ_INPUT_STATUS):
            case (uint8_t)(READ_HOLD_REGS):
            case (uint8_t)(READ_INPUT_REGS):
            {
                /* Response Data: Byte Counts(1 Byte) + Data */
                if( usRespDataLen > 1U )
                {
                    bValid = true;

                    /* Byte Counts */
                    if( pucRespData[usIndex] != (uint8_t)(usRespDataLen - 1U))
                    {
                        bValid = false;
                    }
                }
            }
            break;

            case (uint8_t)(FORCE_SINGLE_COIL):
            case (uint8_t)(PRESET_SINGLE_REG):
            case (uint8_t)(FORCE_MULT_COILS):
            case (uint8_t)(PRESET_MULT_REGS):
            {
                /* Start address and Number/Value: 4 Bytes, Same with Request Message */
                if( 4U == usRespDataLen )
                {
                    bValid = true;

                    for( usIndex = 0U; usIndex < 4U; usIndex++ )
                    {
                        if( pucRespData[usIndex] != pucReqData[usIndex] )
                        {
                            bValid = false;
                            break;
                        }
                    }
                }
            }
            break;

            default:
            {
                bValid = false;
            }
            break;
        }/* end of switch */
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: MstrMakePMDataMsg
*
* 功能描述: 端口配置为Modbus主站时，构造将发往PM模块的数据帧
*
* 输入参数: emPort：端口号；
*          ucFPOrder：端口的FP Order属性；
*          pstRTDataVer：实时数据版本信息；
*          pstMBMstrBlk：Modbus主站块配置信息；
*          pucMBData：Modbus数据首地址 - 字节计数；
*          usMBDataLen：应答帧中数据的长度 - ByteCnts(1B) + MBData(>0B)
*          MBTCP: MBAP Header(7) + Function Code(1) + Data
*          RTU: Slave ID(1) + Function Code(1) + Data + CRC(2)
*          pucPMMBMsg：用于存放将发往PM模块的消息；
*
* 输出参数: pusPMMBMsgLen：用于返回将发往PM模块的消息的长度。
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void MstrMakePMMBMsg( CommPort_t emPort, uint8_t ucFPOrder, RTDataVerInfo_t const *pstRTDataVer,
                             ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucBlkID,
                             uint8_t const pucMBData[], uint16_t usMBDataLen,
                             uint8_t pucPMMBMsg[], puint16_t pusPMMBMsgLen )
{
    bool_t bValid = false;
    uint16_t usMaxRespDataLen = 0U;
    uint8_t ucMBDataIndex = 0U;              /* Modbus Data Index */
    uint8_t ucByteCounts = 0U;               /* Byte Counts Area In Modbus Response Message */
    uint16_t usPMMsgIndex = 0U;              /* Index Of PM Modbus Message */
    uint16_t usDstBuffLen = 0U;              /* Destination Buffer Length: Used to store PM modbus message */
    uint16_t usDstMBDataLen = 0U;            /* Destination Modbus Data Length: Data in PM modbus message */
    uint16_t usEstimateLen = 0U;             /* Estimate PM Modbus Message Length */
    uint32_t uiMBVarIndex = 0U;              /* Modbus Variable Index */
    uint32_t uiMBSIndex = 0U;                /* Modbus Start Index */
    PortMBMstrHead_t *pstPortMBHead = NULL;  /* Port Modbus Master Header */
    ModbusMasterBlockHead_t const *pstHead = NULL;
    ModbusVar_t const *pstMBVar = NULL;

    if(((emPort >= PORT_COM1) && (emPort <= PORT_NET2)) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)) &&
       (pstRTDataVer != NULL) &&
       (pstMBMstrBlk != NULL) &&
       (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)) &&
       (pucMBData != NULL) &&
       (pucPMMBMsg != NULL) &&
       (pusPMMBMsgLen != NULL))
    {
        /* Maximum Data Length of the Modbus TCP/RTU Message */
        if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
        {
            usMaxRespDataLen = (uint16_t)(MAX_MBRTU_DATA_LEN);
        }
        else
        {
            usMaxRespDataLen = (uint16_t)(MAX_MBTCP_DATA_LEN);
        }

        /* Minimum Read Response Data Length: Byte Counts(1) + Data */
        if((usMBDataLen > 1U) && (usMBDataLen <= usMaxRespDataLen))
        {
            pstHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
            usDstBuffLen = *pusPMMBMsgLen;   /* Record Destination Buffer Length */
            *pusPMMBMsgLen = 0U;             /* Reset PM Data Length */

            /* Make PM Modbus Message */
            switch( pstHead->ucModbusRgn )
            {
                case (uint8_t)(MR_COIL):
                case (uint8_t)(MR_DISCRETE_INPUTS):
                {
                    ucByteCounts = (uint8_t)((pstHead->uiTargetSize + 7) / 8);
                    usDstMBDataLen = (uint16_t)(ucByteCounts);
                    usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;

                    /* Check if buffer is long enough */
                    if( usDstBuffLen >= usEstimateLen )
                    {
                        if( pucMBData[ucMBDataIndex++] == ucByteCounts )
                        {
                            bValid = true;
                            usPMMsgIndex = (uint16_t)sizeof(PortMBMstrHead_t);
                            memcpy( &pucPMMBMsg[usPMMsgIndex], &pucMBData[ucMBDataIndex], ucByteCounts );
                        }
                        else
                        {
                            bValid = false;
                            DEBUG_MB( "MstrMakePMMBMsg: ByteCnts-%d != %d. TarSize-%d\n", \
                                      pucMBData[ucMBDataIndex-1], ucByteCounts, pstHead->uiTargetSize );
                        }
                    }
                }
                break;

                case (uint8_t)(MR_INPUT_REGS):
                case (uint8_t)(MR_HOLD_REGS):
                {
                    ucByteCounts = (uint8_t)(pstHead->uiTargetSize * 2);
                    usDstMBDataLen = (uint16_t)(ucByteCounts);
                    usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;
                    uiMBSIndex = pstHead->uiTargetBeginAddr;

                    /* Check if buffer is long enough */
                    if( usDstBuffLen >= usEstimateLen )
                    {
                        if( pucMBData[ucMBDataIndex++] == ucByteCounts )
                        {
                            bValid = true;

                            /* Add Modbus Data: Big Endian */
                            usPMMsgIndex = (uint16_t)sizeof(PortMBMstrHead_t);
                            memcpy( &pucPMMBMsg[usPMMsgIndex], &pucMBData[ucMBDataIndex], ucByteCounts );

                            /* Replace With Real Order Value */
                            for( uiMBVarIndex = 0U; uiMBVarIndex < pstHead->uiModbusVarNum; uiMBVarIndex++ )
                            {
                                pstMBVar = &( pstMBMstrBlk->stModbusVar[uiMBVarIndex]);
                                ucMBDataIndex = 1U + (uint8_t)((pstMBVar->uiModbusIndex-uiMBSIndex) * 2U);
                                usPMMsgIndex = (uint16_t)sizeof(PortMBMstrHead_t) + (uint16_t)(ucMBDataIndex - 1U);
                                if( 2U == pstMBVar->usVarSize )
                                {
                                    memcpy( &pucPMMBMsg[usPMMsgIndex], &pucMBData[ucMBDataIndex], 2U );
                                }
                                else if( 4U == pstMBVar->usVarSize )
                                {
                                    /* H: 0-3 / L: 2,3,0,1 */
                                    if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                    {
                                        memcpy( &pucPMMBMsg[usPMMsgIndex], &pucMBData[ucMBDataIndex], 4U );
                                    }
                                    else
                                    {
                                        pucPMMBMsg[usPMMsgIndex] = pucMBData[ucMBDataIndex+2U];
                                        pucPMMBMsg[usPMMsgIndex+1U] = pucMBData[ucMBDataIndex+3U];
                                        pucPMMBMsg[usPMMsgIndex+2U] = pucMBData[ucMBDataIndex];
                                        pucPMMBMsg[usPMMsgIndex+3U] = pucMBData[ucMBDataIndex+1U];
                                    }
                                }
                                else if( 8U == pstMBVar->usVarSize )
                                {
                                    /* H: 0-7 / L: 2,3,0,1  6,7,4,5 */
                                    if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                    {
                                        memcpy( &pucPMMBMsg[usPMMsgIndex], &pucMBData[ucMBDataIndex], 8U );
                                    }
                                    else
                                    {
                                        pucPMMBMsg[usPMMsgIndex] = pucMBData[ucMBDataIndex+2U];
                                        pucPMMBMsg[usPMMsgIndex+1U] = pucMBData[ucMBDataIndex+3U];
                                        pucPMMBMsg[usPMMsgIndex+2U] = pucMBData[ucMBDataIndex];
                                        pucPMMBMsg[usPMMsgIndex+3U] = pucMBData[ucMBDataIndex+1U];

                                        pucPMMBMsg[usPMMsgIndex+4U] = pucMBData[ucMBDataIndex+6U];
                                        pucPMMBMsg[usPMMsgIndex+5U] = pucMBData[ucMBDataIndex+7U];
                                        pucPMMBMsg[usPMMsgIndex+6U] = pucMBData[ucMBDataIndex+4U];
                                        pucPMMBMsg[usPMMsgIndex+7U] = pucMBData[ucMBDataIndex+5U];
                                    }
                                }
                                else
                                {
                                    bValid = false;
                                    DEBUG_MB( "MstrMakePMMBMsg: Varsize %d != 2/4/8\n", pstMBVar->usVarSize );
                                    break;
                                }
                            }/* end of for */
                        }
                        else
                        {
                            bValid = false;
                            DEBUG_MB( "MstrMakePMMBMsg-Regs: ByteCnts-%d != %d. TarSize-%d\n", \
                                       pucMBData[ucMBDataIndex-1], ucByteCounts, pstHead->uiTargetSize );
                        }
                    }
                }
                break;

                default:
                {
                    bValid = false;
                    DEBUG_MB( "ModbusRgn %d != 0/1/2/3\n", pstHead->ucModbusRgn );
                }
                break;
            }/* end of Switch */

            if( bValid )
            {
                *pusPMMBMsgLen = usEstimateLen;

                /* Add Frame Header: Big Endian */
                pstPortMBHead = (PortMBMstrHead_t*)pucPMMBMsg;
                memset( pstPortMBHead, 0, sizeof(PortMBMstrHead_t));

                pstPortMBHead->ucPortType = (uint8_t)(PORT_MBT_MASTER);  /* Port Type */
                pstPortMBHead->usDataLen = usDstMBDataLen;               /* Data Length */
                pstPortMBHead->ucCMSlotID = s_ucLocalCMSlotID;           /* CM Slot ID */
                pstPortMBHead->ucPortID = (uint8_t)emPort;               /* Port ID */
                pstPortMBHead->uiBlockID = ucBlkID;                      /* Block ID */
                memcpy( &(pstPortMBHead->stRTDataVer), pstRTDataVer, sizeof(RTDataVerInfo_t));
            }
            else
            {
                *pusPMMBMsgLen = 0U;
            }
        }
        else
        {
            DEBUG_MB( "MstrMakePMMBMsg: RespMsgDataLen(ByteCnts(1B) + Data(nB)) - %d Invalid\n", usMBDataLen );
        }
    }
    else
    {
        /* Reset PM Modbus Message Length */
        if( pusPMMBMsgLen != NULL )
        {
            *pusPMMBMsgLen = 0U;
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: SlaveMakePMMBMsg
*
* 功能描述: 端口配置为Modbus从站时，构造将发往PM模块的数据帧
*
* 输入参数: emPort：端口号；
*          ucFPOrder：端口的FP Order属性；
*          pucReqFunc：请求帧功能码地址；
*          usReqDataLen：请求帧中数据的长度 - MBTCP: MBAP Header(7) + Function Code(1) + Data
*                                            RTU: Slave ID(1) + Function Code(1) + Data + CRC(2)
*          pucPMMBMsg：用于存放将发往PM模块的消息。
*
* 输出参数: pusPMMBMsgLen：用于返回消息长度
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SlaveMakePMMBMsg( CommPort_t emPort, uint8_t ucFPOrder, uint8_t const pucReqFunc[], uint16_t usReqDataLen,
                              uint8_t pucPMMBMsg[], puint16_t pusPMMBMsgLen )
{
    bool_t bValid = false;
    uint16_t usMaxReqDataLen = 0U;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
    uint16_t usStartAddr = 0U;
    uint16_t usIndex = 0U;
    uint16_t usNum = 0U;
    uint16_t usValue = 0U;
    uint8_t ucMBDataIndex = 0U;    /* Modbus Data Index */
    uint8_t ucByteCounts = 0U;     /* Byte Counts Area In Modbus Response Message */
    uint16_t usPMMsgIndex = 0U;    /* Index Of PM Modbus Message */
    uint16_t usDstBuffLen = 0U;    /* Destination Buffer Length: Used to store PM modbus message */
    uint16_t usDstMBDataLen = 0U;  /* Destination Modbus Data Length: Data in PM modbus message */
    uint16_t usEstimateLen = 0U;   /* Estimate PM Modbus Message Length */
    uint16_t usVarRgnIndex = 0U;   /* Modbus Variable Index In Region: Alias-1/10001/30001/40001 */
    uint32_t uiSlaveBlkID = 0U;
    uint16_t usAliasNum = 0U;      /* Coil/Reg Number */
    PortMBSlaveHead_t *pstPortMBHead = NULL;
    ModbusSlaveBlock_t const *pstSlaveBlk = NULL;
    ModbusSlaveVar_t const *pstMBSlaveVar = NULL;
    ModbusVar_t const *pstMBVar = NULL;

    if((pucReqFunc != NULL) &&
       (pucPMMBMsg != NULL) &&
       (pusPMMBMsgLen != NULL) &&
       ((emPort >= PORT_COM1) && (emPort <= PORT_NET2)) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        /* Maximum Data Length of the Modbus TCP/RTU Message */
        if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
        {
            usMaxReqDataLen = (uint16_t)(MAX_MBRTU_DATA_LEN);
        }
        else
        {
            usMaxReqDataLen = (uint16_t)(MAX_MBTCP_DATA_LEN);
        }

        /* Minimum Write Request Data Length: Start Address(2 bytes) */
        if((usReqDataLen > 2U) && (usReqDataLen <= usMaxReqDataLen))
        {
            usDstBuffLen = *pusPMMBMsgLen;   /* Record Destination Buffer Length */
            *pusPMMBMsgLen = 0U;             /* Reset PM Data Length */

            ucFuncCode = pucReqFunc[usIndex++];
            usStartAddr = (pucReqFunc[usIndex] << 8) | pucReqFunc[usIndex+1];
            usIndex += 2U;

            /* Make PM Modbus Message */
            switch( ucFuncCode )
            {
                case (uint8_t)(FORCE_SINGLE_COIL):
                {
                    /* 4 Bytes: Start Address(2 bytes) + Value(2 bytes) */
                    if( 4U == usReqDataLen )
                    {
                        usDstMBDataLen = 1U;  /* One BOOL Need One Byte */
                        usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;

                        /* Check if buffer is long enough */
                        if( usDstBuffLen >= usEstimateLen )
                        {
                            bValid = true;
                            usAliasNum = 1U;
                            uiSlaveBlkID = (uint32_t)(MR_COIL);

                            /* Add Coil Value */
                            usPMMsgIndex = (uint16_t)sizeof( PortMBSlaveHead_t );
                            usValue = (pucReqFunc[usIndex] << 8) | pucReqFunc[usIndex+1];
                            if((uint16_t)(COIL_ON) == usValue )
                            {
                                pucPMMBMsg[usPMMsgIndex++] = 0x01U;
                            }
                            else
                            {
                                pucPMMBMsg[usPMMsgIndex++] = 0x00U;
                            }
                        }
                        else
                        {
                            DEBUG_MB( "SlaveMakePMMBMsg-FORCE_SINGLE_COIL: DstBuffLen %d should >= %d\n", \
                                       usDstBuffLen, usEstimateLen );
                        }
                    }
                }
                break;

                case (uint8_t)(PRESET_SINGLE_REG):
                {
                    /* 4 Bytes: Start Address(2 bytes) + Value(2 bytes) */
                    if( 4U == usReqDataLen )
                    {
                        usNum = 1U;
                        usDstMBDataLen = usNum * 2;
                        usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;

                        /* Check if buffer is long enough */
                        if( usDstBuffLen >= usEstimateLen )
                        {
                            bValid = true;
                            usAliasNum = 1U;
                            uiSlaveBlkID = (uint32_t)(MR_HOLD_REGS);

                            /* Add Modbus Data: Big Endian */
                            usPMMsgIndex = (uint16_t)sizeof( PortMBSlaveHead_t );
                            pucPMMBMsg[usPMMsgIndex++] = pucReqFunc[usIndex++];
                            pucPMMBMsg[usPMMsgIndex++] = pucReqFunc[usIndex++];
                        }
                        else
                        {
                            DEBUG_MB( "SlaveMakePMMBMsg-PRESET_SINGLE_REG: DstBuffLen %d should >= %d\n", \
                                       usDstBuffLen, usEstimateLen );
                        }
                    }
                }
                break;

                case (uint8_t)(FORCE_MULT_COILS):
                {
                    /* 5 Bytes: Start Address(2) + Regs(2) + Byte Counts(1) + Value */
                    if( usReqDataLen > 5U )
                    {
                        usNum = (pucReqFunc[usIndex] << 8) | pucReqFunc[usIndex+1];
                        usIndex += 2U;
                        ucByteCounts = pucReqFunc[usIndex++];

                        if(((ucByteCounts + 5U) == usReqDataLen ) &&
                           ((usNum+7U)/8U == ucByteCounts ))
                        {
                            usDstMBDataLen = ucByteCounts;  /* 8 BOOL One Byte */
                            usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;

                            /* Check if buffer is long enough */
                            if( usDstBuffLen >= usEstimateLen )
                            {
                                bValid = true;
                                usAliasNum = usNum;
                                uiSlaveBlkID = (uint32_t)(MR_COIL);

                                /* Add Modbus Data: Big Endian */
                                usPMMsgIndex = (uint16_t)sizeof( PortMBSlaveHead_t );
                                memcpy( &pucPMMBMsg[usPMMsgIndex], &pucReqFunc[usIndex], (uint32_t)ucByteCounts );
                                usPMMsgIndex += ucByteCounts;
                            }
                            else
                            {
                                DEBUG_MB( "SlaveMakePMMBMsg-FORCE_MULT_COILS: DstBuffLen %d should >= %d\n", \
                                           usDstBuffLen, usEstimateLen );
                            }
                        }
                        else
                        {
                            DEBUG_MB( "SlaveMakePMMBMsg-FORCE_MULT_COILS: RegNums %d, ByteCnts %d != %d\n", \
                                       usNum, ucByteCounts, usReqDataLen-5U );
                        }
                    }
                }
                break;

                case (uint8_t)(PRESET_MULT_REGS):
                {
                    /* 5 Bytes: Start Address(2) + Regs(2) + Byte Counts(1) + Value */
                    if( usReqDataLen > 5U )
                    {
                        usNum = (pucReqFunc[usIndex] << 8) | pucReqFunc[usIndex+1];
                        usIndex += 2U;
                        ucByteCounts = pucReqFunc[usIndex++];

                        if(((ucByteCounts + 5U) == usReqDataLen ) &&
                           ( usNum == (ucByteCounts/2U)))
                        {
                            usDstMBDataLen = ucByteCounts;
                            usEstimateLen = (uint16_t)(PMMB_HEADER_LEN) + usDstMBDataLen;

                            /* Check if buffer is long enough */
                            if( usDstBuffLen >= usEstimateLen )
                            {
                                bValid = true;
                                usAliasNum = usNum;
                                uiSlaveBlkID = (uint32_t)(MR_HOLD_REGS);

                                /* Add Modbus Data: Big Endian */
                                usPMMsgIndex = (uint16_t)sizeof( PortMBSlaveHead_t );
                                memcpy( &pucPMMBMsg[usPMMsgIndex], &pucReqFunc[usIndex], (uint32_t)ucByteCounts );

                                /* Replace With Real Order Value */
                                pstSlaveBlk = GetModbusSlaveBlk( &s_stMBSlaveCfgInfo, (uint8_t)(MR_HOLD_REGS));
                                if((pstSlaveBlk != NULL) &&
                                   (ACTIVE == pstSlaveBlk->emActive))
                                {
                                    while( ucMBDataIndex < ucByteCounts )
                                    {
                                        usVarRgnIndex = usStartAddr + ucMBDataIndex / 2U;
                                        if( usVarRgnIndex < (uint16_t)(MAX_MODBUS_RGN_VAR_NUM))
                                        {
                                            pstMBSlaveVar = &( pstSlaveBlk->stModbusSlaveVar[usVarRgnIndex]);
                                            if((uint8_t)(MODBUS_VAR_ACTIVE) == pstMBSlaveVar->ucActive )
                                            {
                                                pstMBVar = &( pstMBSlaveVar->stModbusVar );
                                                if( 2U == pstMBVar->usVarSize )
                                                {
                                                    pucPMMBMsg[usPMMsgIndex++] = pucReqFunc[usIndex++];
                                                    pucPMMBMsg[usPMMsgIndex++] = pucReqFunc[usIndex++];
                                                    ucMBDataIndex += 2U;
                                                }
                                                else if( 4U == pstMBVar->usVarSize )
                                                {
                                                    /* H: 0-3 / L: 2,3,0,1 */
                                                    if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                                    {
                                                        memcpy( &pucPMMBMsg[usPMMsgIndex], &pucReqFunc[usIndex], 4U );
                                                    }
                                                    else
                                                    {
                                                        pucPMMBMsg[usPMMsgIndex] = pucReqFunc[usIndex+2U];
                                                        pucPMMBMsg[usPMMsgIndex+1U] = pucReqFunc[usIndex+3U];
                                                        pucPMMBMsg[usPMMsgIndex+2U] = pucReqFunc[usIndex];
                                                        pucPMMBMsg[usPMMsgIndex+3U] = pucReqFunc[usIndex+1U];
                                                    }

                                                    usIndex += 4U;
                                                    usPMMsgIndex += 4U;
                                                    ucMBDataIndex += 4U;
                                                }
                                                else if( 8U == pstMBVar->usVarSize )
                                                {
                                                    /* H: 0-7 / L: 2,3,0,1  6,7,4,5 */
                                                    if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                                    {
                                                        memcpy( &pucPMMBMsg[usPMMsgIndex], &pucReqFunc[usIndex], 8U );
                                                    }
                                                    else
                                                    {
                                                        pucPMMBMsg[usPMMsgIndex] = pucReqFunc[usIndex+2U];
                                                        pucPMMBMsg[usPMMsgIndex+1U] = pucReqFunc[usIndex+3U];
                                                        pucPMMBMsg[usPMMsgIndex+2U] = pucReqFunc[usIndex];
                                                        pucPMMBMsg[usPMMsgIndex+3U] = pucReqFunc[usIndex+1U];

                                                        pucPMMBMsg[usPMMsgIndex+4U] = pucReqFunc[usIndex+6U];
                                                        pucPMMBMsg[usPMMsgIndex+5U] = pucReqFunc[usIndex+7U];
                                                        pucPMMBMsg[usPMMsgIndex+6U] = pucReqFunc[usIndex+4U];
                                                        pucPMMBMsg[usPMMsgIndex+7U] = pucReqFunc[usIndex+5U];
                                                    }

                                                    usIndex += 8U;
                                                    usPMMsgIndex += 8U;
                                                    ucMBDataIndex += 8U;
                                                }
                                                else
                                                {
                                                    bValid = false;
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                /* Not Exist */
                                                usIndex += 2U;
                                                usPMMsgIndex += 2U;
                                                ucMBDataIndex += 2U;
                                            }
                                        }
                                        else
                                        {
                                            bValid = false;
                                            break;
                                        }
                                    }/* end of while */
                                }
                            }
                            else
                            {
                                DEBUG_MB( "SlaveMakePMMBMsg-PRESET_MULT_REGS: DstBuffLen %d should >= %d\n", \
                                           usDstBuffLen, usEstimateLen );
                            }
                        }
                        else
                        {
                            DEBUG_MB( "SlaveMakePMMBMsg-PRESET_MULT_REGS: RegNums %d, ByteCnts %d != %d\n", \
                                       usNum, ucByteCounts, usReqDataLen-5U );
                        }
                    }
                }
                break;

                default:
                {
                    bValid = false;
                }
                break;
            }/* end of Switch */

            if( bValid )
            {
                *pusPMMBMsgLen = usPMMsgIndex;

                /* Add Frame Header: Big Endian */
                pstPortMBHead = (PortMBSlaveHead_t*)pucPMMBMsg;
                memset( pstPortMBHead, 0, sizeof(PortMBSlaveHead_t));

                pstPortMBHead->ucPortType = (uint8_t)(PORT_MBT_SLAVE);  /* Port Type */
                pstPortMBHead->usDataLen = usDstMBDataLen;              /* Data Length */
                pstPortMBHead->uiBlockID = uiSlaveBlkID;                /* Slave Block ID */
                pstPortMBHead->usBeginAddr = usStartAddr;               /* 0-9999 */
                pstPortMBHead->usAliasNum = usAliasNum;                 /* Coil/Reg Number */
            }
            else
            {
                *pusPMMBMsgLen = 0U;
            }
        }
    }
    else
    {
        /* Reset PM Modbus Message Length */
        if( pusPMMBMsgLen != NULL )
        {
            *pusPMMBMsgLen = 0U;
        }
    }
}

/*
***********************************************************************************************************************
* 函数名称: CheckReqFuncCode
*
* 功能描述: 检查是否支持Modbus主站发送的请求帧中的功能码
*
* 输入参数: ucFuncCode：请求帧功能码
*
* 输出参数: 无
*
* 返 回 值: 是否支持此功能码
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckReqFuncCode( uint8_t ucFuncCode )
{
    bool_t bSupport = false;

    if(((ucFuncCode >= (uint8_t)(READ_COIL_STATUS)) && (ucFuncCode <= (uint8_t)(PRESET_SINGLE_REG))) ||
       ((uint8_t)(MODBUS_DIAG) == ucFuncCode ) ||
       ((uint8_t)(FORCE_MULT_COILS) == ucFuncCode) ||
       ((uint8_t)(PRESET_MULT_REGS) == ucFuncCode))
    {
        bSupport = true;
    }

    return bSupport;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPMaster
*
* 功能描述: 端口作为Modbus主站，根据配置信息轮询相应从站
*
* 输入参数: emPort：端口ID号
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusTCPMaster( CommPort_t emPort )
{
    PortMstrInfo_t *pstPortMstrInfo = NULL;  /* Port Master Info. */

    if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
    {
        pstPortMstrInfo = &( s_stPortMstrInfo[emPort]);

        /* Update Wait Cycle Count */
        if( pstPortMstrInfo->uiWaitLoopCycleCnt > 0U )
        {
            pstPortMstrInfo->uiWaitLoopCycleCnt--;
        }

        if( !s_stMBMstrCfgInfo.bCopying )
        {
            if( IDLE_STATE == pstPortMstrInfo->emState )
            {
                HandleMBTCPMstrIdle( emPort, pstPortMstrInfo );
            }
            else  /* Busy */
            {
                HandleMBTCPMstrBusy( emPort, pstPortMstrInfo );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleMBTCPMstrIdle
*
* 功能描述: 端口作为Modbus主站时的空闲状态处理
*
* 输入参数: emPort：端口ID号；
*          pstPortMstrInfo：本地保存的端口主站信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleMBTCPMstrIdle( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo )
{
    uint32_t uiBlkCnt = 0U;
    uint32_t uiCheckCnt = 0U;
    uint8_t ucBlkID = 0U;
    bool_t bBlkStart = false;
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    uint8_t ucNetIndex = 0U;
    ModbusMasterBlock_t const *pstMBMstrBlk = NULL;  /* Modbus Master Block */

    if((emPort >= PORT_NET1) && (emPort <= PORT_NET2))
    {
        ucNetIndex = (uint8_t)(emPort - PORT_NET1);

        if((s_stMBMstrCfgInfo.bValid) &&
           (0U == pstPortMstrInfo->uiWaitLoopCycleCnt))
        {
            uiBlkCnt = GetPortMBMstrBlkCnt( &s_stMBMstrCfgInfo, emPort );  /* Port Modbus Master Block Count */
            while( uiCheckCnt < uiBlkCnt )
            {
                pstMBMstrBlk = GetPortMBMstrBlkInfo( &s_stMBMstrCfgInfo, emPort, pstPortMstrInfo->uiBlockIndex, \
                                                     &ucBlkID );

                DEBUG_MB( "Net[%d]: BlkCnts %d, BlkIndex %d, BlkID %d\n", \
                           ucNetIndex+1, uiBlkCnt, pstPortMstrInfo->uiBlockIndex, ucBlkID );

                if((pstMBMstrBlk != NULL) &&
                   (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)))
                {
                    bBlkStart = SysGetModbusMasterBlkStartFlag((uint32_t)ucBlkID );
                    if( bBlkStart )
                    {
                        /* FP Order */
                        ucFPOrder = GetPortFPOrder( emPort );

                        /* Reset Length Info. */
                        pstPortMstrInfo->usEstimateRespLen = 0U;

                        /* Make */
                        pstPortMstrInfo->stMBReqMsg.usLen = (uint16_t)sizeof(pstPortMstrInfo->stMBReqMsg.ucBuff);
                        if( MakeModbusTCPReq( pstPortMstrInfo, pstMBMstrBlk, &( pstPortMstrInfo->stMBReqMsg), \
                                              ucFPOrder ))
                        {
                            if( pstPortMstrInfo->stMBReqMsg.usLen > 0U )
                            {
                                if( SendModbusTCPReq( emPort, pstMBMstrBlk, &( pstPortMstrInfo->stMBReqMsg)))
                                {
                                    pstPortMstrInfo->emState = BUSY_STATE;
                                    pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
                                }
                                else
                                {
                                    /* Send failed: Loop Next Block */
                                    DEBUG_MB( "SendMBTCPReq Failed: Net-%d, Blk-%d\n", \
                                              ucNetIndex+1, pstPortMstrInfo->uiBlockIndex );

                                    /* Update Block Index */
                                    pstPortMstrInfo->uiBlockIndex++;
                                    if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                                    {
                                        pstPortMstrInfo->uiBlockIndex = 0U;
                                    }
                                }
                            }
                        }
                        else
                        {
                            DEBUG_MB( "MakeMBTCPReq Failed: Net-%d, Block-%d\n", \
                                       ucNetIndex+1, pstPortMstrInfo->uiBlockIndex );
                            pstPortMstrInfo->uiBlockIndex = 0U;
                        }

                        break;
                    }/* end of if-start */
                    else
                    {
                        DEBUG_MB( "MBMstrBlkStartFlag: 0, Net-%d, Blk-%d\n", \
                                  ucNetIndex+1, pstPortMstrInfo->uiBlockIndex );

                        /* Update Block Index */
                        pstPortMstrInfo->uiBlockIndex++;
                        if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                        {
                            pstPortMstrInfo->uiBlockIndex = 0U;
                        }

                        uiCheckCnt++;  /* Check Count */
                    }
                }
                else
                {
                    pstPortMstrInfo->uiBlockIndex = 0U;
                    break;
                }
            }/* end of while */
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleMBTCPMstrBusy
*
* 功能描述: 端口作为Modbus主站时的忙状态处理
*
* 输入参数: emPort：端口ID号；
*          pstPortMstrInfo：本地保存的端口主站信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleMBTCPMstrBusy( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo )
{
    NetworkPortType_t emNet = NET1;
    uint8_t ucBlkID = 0U;
    uint16_t usLen = 0U;
    uint32_t uiBlkCnt = 0U;
    ModbusMasterBlock_t const *pstMBMstrBlk = NULL;

    if((emPort >= PORT_NET1) && (emPort <= PORT_NET2) &&
       (pstPortMstrInfo != NULL))
    {
        emNet = (NetworkPortType_t)(emPort - PORT_NET1);

        /* Wait / Handle Slave Response */
        pstPortMstrInfo->uiWaitLoopCycleCnt = 0U;
        usLen = (uint16_t)sizeof( s_ucRespBuff );
        if( SysGetModbusRespMsg( emNet, s_ucRespBuff, &usLen ))
        {
            if( usLen > 0U )
            {
                ucModbusOnlineCnt[emPort] = ucModbusOnlineCnt[emPort] + 1;

                /* Has Response */
                if( pstPortMstrInfo->bHandleSwitch )
                {
                    /* Port Modbus Master Info. Changed: Discard Data And Re-Loop */
                    DEBUG_MB( "Net[%d] HandleSwitch-Discard And Re-Loop\n", emPort+1 );
                    pstPortMstrInfo->uiBlockIndex = 0U;
                    pstPortMstrInfo->bHandleSwitch = false;
                }
                else
                {
                    if( s_stMBMstrCfgInfo.bValid )
                    {
                        pstMBMstrBlk = GetPortMBMstrBlkInfo( &s_stMBMstrCfgInfo, emPort, pstPortMstrInfo->uiBlockIndex,\
                                                             &ucBlkID );
                        if((pstMBMstrBlk != NULL) &&
                           (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)))
                        {
                            HandleModbusTCPResp( emPort, pstMBMstrBlk, ucBlkID, pstPortMstrInfo, s_ucRespBuff, usLen );

                            /* Update Block Index */
                            uiBlkCnt = GetPortMBMstrBlkCnt( &s_stMBMstrCfgInfo, emPort );
                            pstPortMstrInfo->uiBlockIndex++;
                            if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                            {
                                pstPortMstrInfo->uiBlockIndex = 0U;

                                SysSetCM2PMModbusState(emPort, 1);
                                ucModbusOnlineCnt[emPort] = 0;
                            }

                            /* Wait To Loop Next Block */
                            pstPortMstrInfo->uiWaitLoopCycleCnt = (uint32_t)(LOOPBLK_INVL_CYCLE_CNTS);
                        }
                        else
                        {
                            DEBUG_MB( "GetPortMBMstrBlkInfo Failed\n" );
                            pstPortMstrInfo->uiBlockIndex = 0U;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "s_stMBMstrCfgInfo Invalid\n" );
                        pstPortMstrInfo->uiBlockIndex = 0U;
                    }
                }

                pstPortMstrInfo->emState = IDLE_STATE;
                pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
            }
            else
            {
                /* No Response */
                pstPortMstrInfo->uiWaitRespCycleCnt++;
                if( pstPortMstrInfo->uiWaitRespCycleCnt > (uint32_t)(MBTCP_RESP_TO_CYCLE_CNT))
                {
                    /* Timeout */
                    DEBUG_MB( "MBTCPSlave Timeout: Net-%d, Blk-%d!\n", emPort+1, pstPortMstrInfo->uiBlockIndex );
                    HandleModbusSlaveTimeout( emPort, pstPortMstrInfo, &s_stMBMstrCfgInfo );
                }
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakeModbusTCPReq
*
* 功能描述: 构造Modbus TCP请求帧
*
* 输入参数: pstPortMstrInfo：端口主站信息；
*          pstMBMstrBlk：指向Modbus 主站块；
*          pstMBReqMsg：用于存放构造好的Modbus TCP请求帧；
*          ucFPOrder：网口 FP Order属性。
*
* 输出参数: 无
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t MakeModbusTCPReq( PortMstrInfo_t *pstPortMstrInfo, ModbusMasterBlock_t const *pstMBMstrBlk,
                                ModbusMsg_t *pstMBReqMsg, uint8_t ucFPOrder )
{
    bool_t bResult = false;    /* Make Successfully Flag */
    puint8_t pucReq = NULL;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
    uint16_t usByteCounts = 0U;
    uint16_t usDataLen = 0U;
    uint16_t usIndex = 0U;
    uint16_t usReqDataLen = 0U;
    uint16_t usLenArea = 0U;   /* Length Area: Unit ID + Funccode + Data */
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usPerLen = 0U;
#endif

    if((pstPortMstrInfo != NULL) &&
       (pstMBMstrBlk != NULL) &&
       (pstMBReqMsg != NULL) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
        ucFuncCode = GetModbusFuncCode( pstBlkHead );
        if( ucFuncCode != (uint8_t)(INVALID_MB_FUNCCODE))
        {
            usDataLen = GetModbusReqDataLen( pstBlkHead, ucFuncCode );
            if(( usDataLen > 0U ) &&
               ( usDataLen <= (uint16_t)(MAX_MBTCP_DATA_LEN)))
            {
                bResult = true;
                pstMBReqMsg->usLen = 0U;
                pucReq = pstMBReqMsg->ucBuff;

                /* Length Area: Unit ID(1B) + Function Code(1B) + Data */
                usLenArea = 2 + usDataLen;

                /* Modbus TCP Header: MBAP */
                pucReq[MBAP_TRANS_ID_H_INDEX] = ((MBAP_TRANS_ID_VALUE) >> 8) & 0xFF;
                pucReq[MBAP_TRANS_ID_L_INDEX] = (MBAP_TRANS_ID_VALUE) & 0xFF;
                pucReq[MBAP_PROT_ID_H_INDEX] = ((MBAP_PROT_ID_VALUE) >> 8) & 0xFF;
                pucReq[MBAP_PROT_ID_L_INDEX] = (MBAP_PROT_ID_VALUE) & 0xFF;
                pucReq[MBAP_LEN_H_INDEX] = (usLenArea >> 8) & 0xFF;
                pucReq[MBAP_LEN_L_INDEX] = usLenArea & 0xFF;
                pucReq[MBAP_UNIT_ID_INDEX] = (uint8_t)pstBlkHead->uiTargetStationID;  /* Slave ID */

                /* Content: Function Code + Data */
                usIndex = (uint16_t)(MBTCP_HEADER_LEN);

                /* Function Code */
                pucReq[usIndex++] = ucFuncCode;

                /* Data */
                if((uint8_t)(VM_READ) == pstBlkHead->ucVisitMode )
                {
                    usReqDataLen = AddReadReqData( pstBlkHead, &pucReq[usIndex]);
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;

                        /* Response Length: MBAP(7) + Function Code(1) + Byte Counts(1) + Data(n) */
                        if(((uint8_t)(MR_COIL) == pstBlkHead->ucModbusRgn) ||
                           ((uint8_t)(MR_DISCRETE_INPUTS) == pstBlkHead->ucModbusRgn))
                        {
                            usByteCounts = (uint16_t)((pstBlkHead->uiTargetSize + 7) / 8);
                        }
                        else
                        {
                            usByteCounts = (uint16_t)(pstBlkHead->uiTargetSize * 2);
                        }

                        /* Response Length: MBTCP Header(MBAP 7B) + FuncCode(1B) + ByteCnts(1B) + MBData(nB) */
                        pstPortMstrInfo->usEstimateRespLen = (uint16_t)(MBTCP_HEADER_LEN) + 2U + usByteCounts;
                    }
                    else
                    {
                        DEBUG_MB( "AddReadReqData Failed\n" );
                        bResult = false;
                    }
                }
                else
                {
                    usReqDataLen = AddWriteReqData( pstMBMstrBlk, ucFuncCode, &pucReq[usIndex], ucFPOrder );
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;

                        /* Response Length: MBTCP Header(MBAP 7B) + FuncCode(1B) + Start Address(2B) + Number(2B) */
                        pstPortMstrInfo->usEstimateRespLen = (uint16_t)(MBTCP_HEADER_LEN) + 5U;
                    }
                    else
                    {
                        DEBUG_MB( "AddWriteReqData Failed\n" );
                        bResult = false;
                    }
                }

                /* Record Request Length */
                if( bResult )
                {
                    pstMBReqMsg->usLen = usIndex;

#ifdef DEBUG_MODBUS
                    printf( "MBTCPReq Message: Len-%d, Content: \n", pstMBReqMsg->usLen );
                    for( usIndex = 0U; usIndex < pstMBReqMsg->usLen; usIndex++ )
                    {
                        printf( "%02X ", pucReq[usIndex]);
                        usPerLen++;

                        if(( 30 == usPerLen ) ||
                           ((pstMBReqMsg->usLen-1) == usIndex))
                        {
                            printf( "\n" );
                            usPerLen = 0U;
                        }
                    }
#endif
                }
            }
            else
            {
                DEBUG_MB( "MBTCPReq Data Len %d Invalid(Max: %d)\n", usDataLen, MAX_MBTCP_DATA_LEN );
            }
        }
        else
        {
            DEBUG_MB( "GetMBFuncCode Failed\n" );
        }
    }

    return bResult;
}



/*
***********************************************************************************************************************
* 函数名称: SendModbusTCPReq
*
* 功能描述: 向Modbus TCP从站发送请求帧
*
* 输入参数: emPort：端口号；
*          pstMBMstrBlk：指向Modbus 主站块；
*          pstMBReqMsg：用于存放待发送的Modbus TCP请求帧。
*
* 输出参数: 无
*
* 返 回 值: 是否发送成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SendModbusTCPReq( CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk,
                                ModbusMsg_t const *pstMBReqMsg )
{
    bool_t bSuccess = false;
    NetworkPortType_t emNet = NET1;
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

    if(((emPort >= PORT_NET1) && (emPort <= PORT_NET2)) &&
        (pstMBMstrBlk != NULL) &&
        (pstMBReqMsg != NULL))
    {
        emNet = (NetworkPortType_t)(emPort - PORT_NET1);
        pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );

        /* Send To Modbus TCP Slave Station */
        ucResult = Send( pstMBReqMsg->ucBuff, pstMBReqMsg->usLen,
                         pstBlkHead->uiTargetIP, (uint16_t)(PORTNUM_MODBUS), emNet,
                         TYPE_MBTCP_SLAVE, 0U );

        if( ucResult != (uint8_t)(SEND_SUCCESS))
        {
            bSuccess = false;
            DEBUG_MB( "Net[%d] SendMBTCPReq Failed: Reason %d\n", emNet+1, ucResult );
        }
        else
        {
            bSuccess = true;
            DEBUG_MB( "Net[%d] SendModbusTCPReq OK\n", emNet+1 );
        }

    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPResp
*
* 功能描述: 根据Modbus TCP请求信息，解析Modbus TCP应答帧
*
* 输入参数: emPort：端口号；
*          pstMBMstrBlk：Modbus 主站块地址；
*          ucBlkID：Modbus 主站块ID；
*          pstPortMstrInfo：端口主站信息；
*          pucResp：从站应答消息；
*          usRespLen：应答消息的长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusTCPResp( CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk, uint8_t ucBlkID,
                                 PortMstrInfo_t *pstPortMstrInfo, uint8_t const pucResp[], uint16_t usRespLen )
{
    bool_t bValid = false;
    uint16_t usNonDataLen = (uint16_t)(MBTCP_MSG_NONDATA_LEN);
    uint16_t usIndex = 0U;
    uint16_t usPMDataLen = 0U;
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    ModbusMasterBlockHead_t const *pstHead = NULL;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usPerLen = 0U;
#endif

    if(((emPort >= PORT_NET1) && (emPort <= PORT_NET2)) &&
       (pstMBMstrBlk != NULL) &&
       (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)) &&
       (pstPortMstrInfo != NULL) &&
       (pucResp != NULL) &&
       ((usRespLen > usNonDataLen) && (usRespLen <= (uint16_t)(MAX_MBTCP_MSG_LEN))))
    {
        /* Update Estimate Response Length */
        if((pucResp[MBTCP_FUNCCODE_INDEX] & (uint8_t)(BIT_8)) != 0U )
        {
            pstPortMstrInfo->usEstimateRespLen = (uint16_t)(MBTCP_EXMSG_LEN);  /* Exception Message */
        }

        /* Length */
        if( usRespLen == pstPortMstrInfo->usEstimateRespLen )
        {
            pstHead = &( pstMBMstrBlk->stModbusMasterBlockHead);

            /* Content of response message */
            bValid = CheckMBTCPResp( pstPortMstrInfo->stMBReqMsg.ucBuff, pucResp, usRespLen );
            if( bValid )
            {
                if((uint8_t)(VM_READ) == pstHead->ucVisitMode )
                {
                    ucFPOrder = GetPortFPOrder( emPort );             /* Port FP Order */
                    usIndex = (uint16_t)(MBTCP_FUNCCODE_INDEX) + 1U;  /* Byte Counts Index */

                    /* Make PM Data Frame */
                    usPMDataLen = (uint16_t)sizeof( s_ucPMDataBuff );
                    MstrMakePMMBMsg( emPort, ucFPOrder, &(s_stMBMstrCfgInfo.stRTDataVer), pstMBMstrBlk, ucBlkID,
                                     &pucResp[usIndex], usRespLen - usNonDataLen, s_ucPMDataBuff, &usPMDataLen );
                    if( usPMDataLen > 0U )
                    {
#ifdef DEBUG_MODBUS
                        printf( "Net[%d] MBTCPMaster Message To PM: Length: %d, Content: \n", \
                                emPort-PORT_NET1+1, usPMDataLen );

                        for( usIndex = 0U; usIndex < usPMDataLen; usIndex++ )
                        {
                            printf( "%02X ", s_ucPMDataBuff[usIndex]);
                            usPerLen++;

                            if(( 30 == usPerLen ) ||
                               ((usPMDataLen-1) == usIndex))
                            {
                                printf( "\n" );
                                usPerLen = 0U;
                            }
                        }
#endif

                        /* Send To PM */
                        SendMBDataToPM( s_ucPMDataBuff, usPMDataLen );
                    }
                }/* end of VM_READ */
            }/* end of Valid */
            else
            {
                DEBUG_MB( "Net[%d] Resp Data Invalid\n", emPort-PORT_NET1+1 );
            }
        }
        else
        {
            DEBUG_MB( "Net[%d] Len %d != %d\n", emPort-PORT_NET1+1, usRespLen, pstPortMstrInfo->usEstimateRespLen );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckMBTCPResp
*
* 功能描述: 检查Modbus TCP从站应答帧的内容是否有效
*
* 输入参数: pucReq：Modbus TCP主站的请求帧；
*          pucResp：Modbus TCP从站的应答帧；
*          usRespLen：Modbus TCP从站应答帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 应答帧是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckMBTCPResp( uint8_t const pucReq[], uint8_t const pucResp[], uint16_t usRespLen )
{
    bool_t bValid = false;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
    uint16_t usIndex = 0U;
    uint16_t usLen = 0U;       /* Length Value */
    uint16_t usValidLen = 0U;  /* Valid Length Value */
    uint16_t usDataLen = 0U;
    uint16_t usNonDataLen = (uint16_t)(MBTCP_MSG_NONDATA_LEN);

    if((pucResp != NULL) &&
       (pucReq != NULL) &&
       ((usRespLen > usNonDataLen) && (usRespLen <= (uint16_t)(MAX_MBTCP_MSG_LEN))))
    {
        bValid = true;

        /* MBAP */
        /* MBAP - Transaction Identifier */
        if((pucResp[MBAP_TRANS_ID_H_INDEX] != pucReq[MBAP_TRANS_ID_H_INDEX]) ||
           (pucResp[MBAP_TRANS_ID_L_INDEX] != pucReq[MBAP_TRANS_ID_L_INDEX]))
        {
            bValid = false;
            DEBUG_MB( "CheckMBTCPResp: Transaction Identifier Invalid\n" );
        }

        /* MBAP - Protocol Identifier */
        if( bValid )
        {
            if((pucResp[MBAP_PROT_ID_H_INDEX] != pucReq[MBAP_PROT_ID_H_INDEX]) ||
               (pucResp[MBAP_PROT_ID_L_INDEX] != pucReq[MBAP_PROT_ID_L_INDEX]))
            {
                bValid = false;
                DEBUG_MB( "CheckMBTCPResp: Protocol Identifier Invalid\n" );
            }
        }

        /* MBAP - Length */
        if( bValid )
        {
            usValidLen = usRespLen - 6U;  /* Unit ID(1B) + Function Code(1B) + Data */
            usLen = (pucResp[MBAP_LEN_H_INDEX] << 8U) | pucResp[MBAP_LEN_L_INDEX];
            if( usLen != usValidLen )
            {
                bValid = false;
                DEBUG_MB( "CheckMBTCPResp: Length-%d != %d\n", usLen, usValidLen );
            }
        }

        /* MBAP - Unit Identifier */
        if( bValid )
        {
            if( pucResp[MBAP_UNIT_ID_INDEX] != pucReq[MBAP_UNIT_ID_INDEX] )
            {
                bValid = false;
                DEBUG_MB( "CheckMBTCPResp: Unit Identifier Invalid\n" );
            }
        }

        /* Function Code And Data */
        if( bValid )
        {
            /* Function Code */
            usIndex = (uint16_t)(MBTCP_FUNCCODE_INDEX);
            bValid = CkeckMBRespFuncCode( &pucResp[usIndex], pucReq[usIndex] );
            if( bValid )
            {
                /* Data */
                usDataLen = usRespLen - usNonDataLen;
                ucFuncCode = pucReq[usIndex];
                usIndex++;
                bValid = CheckMBRespData( ucFuncCode, usDataLen, &pucResp[usIndex], &pucReq[usIndex] );
                if( !bValid )
                {
                    DEBUG_MB( "CheckMBTCPResp: RespData Invalid\n" );
                }
            }
            else
            {
                DEBUG_MB( "CheckMBTCPResp: FuncCode Invalid\n" );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusTCPSlave
*
* 功能描述: 端口作为Modbus TCP从站时，处理Modbus TCP 主站的请求并应答
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusTCPSlave( void )
{
    uint32_t uiHandleCnt = 0U;  /* Handle Count */
    uint32_t uiCheckCnt = 0U;   /* Check Count */
    uint16_t usLen = 0U;        /* Length */
    uint16_t usRespLen = 0U;    /* Length of response message */
    CommPort_t emPort = PORT_NET1;

#ifdef DEBUG_MODBUS
    /* only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    /* Handle when configuration copying finished and valid */
    if((!s_stMBSlaveCfgInfo.bCopying) &&
       (s_stMBSlaveCfgInfo.bValid))
    {
        while( uiCheckCnt < (uint32_t)(MAX_MBTCP_MSTR_CNT))
        {
            /* Read Master Message: Request */
            s_stMBTCPMsg.stNetHead.usLen = 0U;
            usLen = (uint16_t)sizeof( MBTCPMsg_t );
            if( !SysGetModbusTCPMasterMsg( s_ucMBTCPMstrIndex, (puint8_t)&s_stMBTCPMsg, &usLen ))
            {
                usLen = 0U;
                DEBUG_MB( "GetMBTCPMstrReqMsg Failed\n" );
            }

            if((usLen > 0U) && (s_stMBTCPMsg.stNetHead.usLen > 0U))
            {
                /* Has Request Message */
                emPort = PORT_NET1 + s_stMBTCPMsg.stNetHead.emNetwork;
                usRespLen = (uint16_t)sizeof( s_ucRespBuff );
                DecodeModbusTCPReq( emPort, s_stMBTCPMsg.ucBuff, s_stMBTCPMsg.stNetHead.usLen,
                                    s_ucRespBuff, &usRespLen );
                if( usRespLen > 0U )
                {
#ifdef DEBUG_MODBUS
                    printf( "Net[%d] MBSlave RespMsg: Len %d, Data: \n", emPort-PORT_NET1+1, usRespLen );

                    for( usIndex = 0U; usIndex < usRespLen; usIndex++ )
                    {
                        printf( "%02X ", s_ucRespBuff[usIndex]);
                        usPerLen++;

                        if(( 30U == usPerLen ) ||
                           ((usRespLen-1U) == usIndex))
                        {
                            printf( "\n" );
                            usPerLen = 0U;
                        }
                    }
#endif
                    SendModbusTCPResp( &( s_stMBTCPMsg.stNetHead ), s_ucMBTCPMstrIndex, s_ucRespBuff, usRespLen );
                }

                uiHandleCnt++;  /* Update Handle Count */
            }
            else
            {
                /* No Request Message */
                /* Update Master Index */
                s_ucMBTCPMstrIndex++;
                s_ucMBTCPMstrIndex = s_ucMBTCPMstrIndex % (uint8_t)(MAX_MBTCP_MSTR_CNT);
                uiCheckCnt++;
                if( uiHandleCnt > 0U )
                {
                    break;
                }
            }
        }/* end of while */
    }/* end of if-not copying */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeModbusTCPReq
*
* 功能描述: 解析请求帧，根据解析结果构造应答帧，帧有效且为写操作时，构造将发往PM的数据帧
*
* 输入参数: pucReq：请求帧；
*          usReqLen：请求帧长度；
*          pucResp：应答帧；
*
* 输出参数: pusRespLen：应答帧的实际长度。
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeModbusTCPReq( CommPort_t emPort, uint8_t const pucReq[], uint16_t usReqLen,
                                uint8_t pucResp[], puint16_t pusRespLen )
{
    uint8_t ucNetIndex = 0U;
    uint16_t usLenArea = 0U;                              /* Length Value In MBAP */
    uint16_t usPMDataLen = 0U;                            /* PM Data Buffer Length */
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);  /* Function Code */
    ModbusException_t emExcCode = MBEX_NONE;              /* Exception Code */
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    if(((emPort >= PORT_NET1) && (emPort <= PORT_NET2)) &&
       (pucReq != NULL) &&
       ((usReqLen > (uint16_t)(MBTCP_MSG_NONDATA_LEN)) && (usReqLen <= (uint16_t)(MAX_MBTCP_MSG_LEN))) &&
       (pucResp != NULL) &&
       (pusRespLen != NULL))
    {
        ucNetIndex = (uint8_t)(emPort - PORT_NET1);
        *pusRespLen = 0U;  /* Reset Response Length */

        /* MBAP: Except Length(2) */
        /* Transaction ID(2) + Protocol ID(2) */
        memcpy( pucResp, pucReq, 4U );

        /* Unit ID */
        pucResp[MBAP_UNIT_ID_INDEX] = pucReq[MBAP_UNIT_ID_INDEX];

        /* Check Request Function Code */
        ucFuncCode = pucReq[MBTCP_FUNCCODE_INDEX];
        if( !CheckReqFuncCode( ucFuncCode ))
        {
            DEBUG_MB( "Net[%d]: Not Support ReqFuncCode-%d\n", ucNetIndex+1, ucFuncCode );
            emExcCode = MBEX_ILLEGAL_FUNC;

            /* Make Exception Response Message */
            *pusRespLen = MakeMBTCPExRespMsg( pucResp, ucFuncCode, emExcCode );
            if( 0U == *pusRespLen )
            {
                DEBUG_MB( "Net[%d] MakeMBTCPExRespMsg Failed\n", ucNetIndex+1 );
            }
        }
        else
        {
            /* Check Length Area */
            usLenArea = ((pucReq[MBAP_LEN_H_INDEX] << 8U) | pucReq[MBAP_LEN_L_INDEX]);
            if( usLenArea == (usReqLen - 6U))
            {
                /* Request Valid */
                /* Make Normal Response Message */
                ucFPOrder = GetPortFPOrder( emPort );
#ifdef DEBUG_MODBUS
                DEBUG_MB( "Net[%d] FPOrder: %d\n",  emPort-PORT_NET1+1, ucFPOrder );
#endif
                *pusRespLen = MakeMBTCPRespMsg( ucFPOrder, pucReq, pucResp );
                if( *pusRespLen > 0U )
                {
                    if(((uint8_t)(FORCE_SINGLE_COIL) == ucFuncCode) ||
                       ((uint8_t)(PRESET_SINGLE_REG) == ucFuncCode) ||
                       ((uint8_t)(FORCE_MULT_COILS) == ucFuncCode)  ||
                       ((uint8_t)(PRESET_MULT_REGS) == ucFuncCode))
                    {
                        /* Port Write Enable */
                        if( GetPortWriteEnable( emPort ))
                        {
                            /* Make PM Data Frame */
                            usPMDataLen = sizeof( s_ucPMDataBuff );
                            SlaveMakePMMBMsg( emPort, ucFPOrder, &pucReq[MBTCP_FUNCCODE_INDEX], \
                                              usReqLen - (uint16_t)(MBTCP_MSG_NONDATA_LEN), \
                                              s_ucPMDataBuff, &usPMDataLen );

                            if( usPMDataLen > 0U )
                            {
                                /* Send To PM */
#ifdef DEBUG_MODBUS
                                printf( "Net[%d] MBTCPSlave Message: Length: %d, Content: \n", \
                                        ucNetIndex+1, usPMDataLen );

                                for( usIndex = 0U; usIndex < usPMDataLen; usIndex++ )
                                {
                                    printf( "%02X ", s_ucPMDataBuff[usIndex]);
                                    usPerLen++;

                                    if(( 30 == usPerLen ) ||
                                       ((usPMDataLen-1) == usIndex))
                                    {
                                        printf( "\n" );
                                        usPerLen = 0U;
                                    }
                                }
#endif

                                SendMBDataToPM( s_ucPMDataBuff, usPMDataLen );
                            }
                        }
                        else
                        {
                            DEBUG_MB( "Net[%d] PortWriteEnable: False\n", ucNetIndex+1 );
                        }
                    }
                }
                else
                {
                    DEBUG_MB( "Net[%d] MakeMBTCPRespMsg Failed\n", ucNetIndex+1 );
                }
            }
            else
            {
                DEBUG_MB( "Net[%d] MBTCPReq: LenArea-%d != %d\n", ucNetIndex+1, usLenArea, usReqLen - 6U );
            }
        }
    }
    else
    {
        if( pusRespLen != NULL )
        {
            *pusRespLen = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakeMBTCPExRespMsg
*
* 功能描述: 构造Modbus TCP 异常应答消息
*          MBAP-Length, Function Code + Exception Code
*
* 输入参数: pucResp：应答帧首地址；
*          ucReqFuncCode：请求帧功能吗；
*          emExcCode：异常码。
*
* 输出参数: 无
*
* 返 回 值: 异常应答帧的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t MakeMBTCPExRespMsg( uint8_t pucResp[], uint8_t ucReqFuncCode, ModbusException_t emExcCode )
{
    uint16_t usExRespLen = 0U;
    uint16_t usLenArea = 3U;  /* Unit ID(1) + Function Code(1) + Exception Code(1) */

    if((pucResp != NULL) &&
       (emExcCode != MBEX_NONE))
    {
        /* MBAP: Length(2) */
        pucResp[MBAP_LEN_H_INDEX] = (usLenArea >> 8U) & 0xFF;
        pucResp[MBAP_LEN_L_INDEX] = usLenArea & 0xFF;

        /* Function Code */
        pucResp[MBTCP_FUNCCODE_INDEX] = ucReqFuncCode | (uint8_t)(BIT_8);

        /* Exception Code */
        pucResp[MBTCP_EXCCODE_INDEX] = emExcCode;

        usExRespLen = (uint16_t)(MBTCP_EXMSG_LEN);
    }

    return usExRespLen;
}

/*
***********************************************************************************************************************
* 函数名称: MakeMBTCPRespMsg
*
* 功能描述: 构造Modbus TCP 应答消息
*          MBAP-Length, Function Code + Data：无效地址，数值填0
*
* 输入参数: ucFPOrder：端口的FP Order属性；
*          pucResp：应答帧首地址；
*          pucResp：存放应答帧的首地址。
*
* 输出参数: 无
*
* 返 回 值: 应答帧的长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t MakeMBTCPRespMsg( uint8_t ucFPOrder, uint8_t const pucReq[], uint8_t pucResp[])
{
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);  /* Function Code */
    uint16_t usIndex = 0U;
    uint16_t usRespDataLen = 0U;
    uint16_t usRespLen = 0U;
    uint16_t usLenArea = 0U;

    if((((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)) &&
       (pucReq != NULL) &&
       (pucResp != NULL))
    {
        /* Function Code */
        usIndex = (uint16_t)(MBTCP_FUNCCODE_INDEX);
        ucFuncCode = pucReq[usIndex];
        pucResp[usIndex++] = ucFuncCode;

        /* Data */
        usRespDataLen = AddMBRespData( false, ucFuncCode, ucFPOrder, &pucReq[usIndex], &pucResp[usIndex]);
        if((usRespDataLen > 0U) && (usRespDataLen <= (uint16_t)(MAX_MBTCP_DATA_LEN)))
        {
            usRespLen = usIndex + usRespDataLen;

            /* MBAP: Length(2) */
            usLenArea = usRespLen - 6U;  /* 6: MBAP(7) Except Unit ID(1) */
            pucResp[MBAP_LEN_H_INDEX] = (uint8_t)((usLenArea >> 8U) & 0xFF);
            pucResp[MBAP_LEN_L_INDEX] = (uint8_t)(usLenArea & 0xFF);
        }
        else
        {
            usRespLen = 0U;
        }
    }

    return usRespLen;
}

/*
***********************************************************************************************************************
* 函数名称: AddBOOLRespData
*
* 功能描述: 添加线圈/输入离散量 - BOOL 数据
*
* 输入参数: pstSlaveBlk：Modbus数据区块 - 线圈/输入离散量；
*          usStartAddr：请求帧中的起始地址 - Alias-1/10001/30001/40001；
*          usNumber：请求帧中的数目；
*          pucRespData：指向目标填充区 - Modbus数据。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度，0代表存在错误
*
* 注意事项: 不存在相应变量时，不应答错误，数值填0
***********************************************************************************************************************
*/

static void AddBOOLRespData( ModbusSlaveBlock_t const *pstSlaveBlk, uint16_t usStartAddr,
                             uint16_t usNumber, uint8_t pucRespData[])
{
    uint16_t usDataIndex = 0U;
    uint16_t usIndex = 0U;
    bool_t bCMBus = false;
    bool_t bValue = false;        /* BOOL Value */
    uint8_t ucData = 0U;          /* Data: 8 BOOL Value */
    uint8_t ucBitIndex = 0U;      /* Bit Index: 0-7 */
    uint16_t usVarRgnIndex = 0U;  /* Variable Region Index: 0-9999 */
    ModbusSlaveVar_t const *pstMBSlaveVar = NULL;

    if((pstSlaveBlk != NULL) &&
       (pucRespData != NULL) &&
       (usStartAddr <= (uint16_t)(MAX_MBRGN_INDEX)))
    {
        if( ACTIVE == pstSlaveBlk->emActive )
        {
            /* Real-Time Data Address */
            if( SelectRTData( &s_emLastPMID, &bCMBus))
            {
                /* BOOL Value: Low Index - First transfer, Low bit */
                for( usIndex = 0U; usIndex < usNumber; usIndex++ )
                {
                    ucBitIndex = usIndex % 8U;  /* Update Bit Index */
                    if( 0U == ucBitIndex )
                    {
                        ucData = 0U;  /* Fill with 0 */
                    }

                    /* Modbus Variable：If active, fill with its real value */
                    usVarRgnIndex = usStartAddr + usIndex;
                    if( usVarRgnIndex < (uint16_t)(MAX_MODBUS_RGN_VAR_NUM))
                    {
                        pstMBSlaveVar = &( pstSlaveBlk->stModbusSlaveVar[usVarRgnIndex]);
                        if((uint8_t)(MODBUS_VAR_ACTIVE) == pstMBSlaveVar->ucActive )
                        {
                            /* Active: Fill with its value */
                            if( GetMBBOOLValue( s_emLastPMID, &( pstMBSlaveVar->stModbusVar), &bValue ))
                            {
                                if( bValue )
                                {
                                    ucData |= 0x01 << ucBitIndex;
                                }
                            }
                        }
                    }

                    /* Set Request every 8 bits or Loop end */
                    if((7U == ucBitIndex) ||
                       (usIndex == usNumber - 1U))
                    {
                        pucRespData[usDataIndex++] = ucData;
                    }
                }/* end of for */
            }
            else
            {
                DEBUG_MB( "SelectRTData Failed\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: AddRegsRespData
*
* 功能描述: 添加保持寄存器/输入寄存器数据
*
* 输入参数: pstSlaveBlk：Modbus数据区块 - 保持/输入寄存器；
*          usStartAddr：请求帧中的起始地址 - Alias - 1/10001/30001/40001；
*          usNumber：请求帧中的数目；
*          pucRespData：指向目标填充区 - Modbus数据。
*
* 输出参数: 无
*
* 返 回 值: 添加的数据长度
*
* 注意事项: 不存在相应变量时，不应答错误，数值填0
***********************************************************************************************************************
*/

static void AddRegsRespData( ModbusSlaveBlock_t const *pstSlaveBlk, uint8_t ucFPOrder,
                             uint16_t usStartAddr, uint16_t usNumber, uint8_t pucRespData[])
{
    bool_t bCMBus = false;
    uint16_t usDataIndex = 0U;    /* Data Index */
    uint16_t usRegsIndex = 0U;    /* Register Index */
    uint16_t usVarRgnIndex = 0U;  /* Variable Region Index: 0-9999 */
    ModbusSlaveVar_t const *pstMBSlaveVar = NULL;
    ModbusVar_t const *pstMBVar = NULL;
    uint8_t const *pucGAddr = NULL;
    uint8_t const *pucIAddr = NULL;
    uint8_t const *pucQAddr = NULL;
    uint8_t const *pucVarAddr = NULL;

    if((pstSlaveBlk != NULL) &&
       (pucRespData != NULL) &&
       (usStartAddr <= (uint16_t)(MAX_MBRGN_INDEX)) &&
       (usStartAddr+usNumber <= (uint16_t)(MAX_MODBUS_RGN_VAR_NUM)) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        if( ACTIVE == pstSlaveBlk->emActive )
        {
            /* Select PM RT Data */
            if( SelectRTData( &s_emLastPMID, &bCMBus))
            {
                pucGAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_G_AREA );
                pucIAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_I_AREA );
                pucQAddr = SysGetRTDataAddr( s_emLastPMID, RTDATA_Q_AREA );
                if((pucGAddr != NULL) &&
                   (pucIAddr != NULL) &&
                   (pucQAddr != NULL))
                {
                    /* Registers Value: Low Alias Value - First transfer */
                    while( usRegsIndex < usNumber )
                    {
                        usVarRgnIndex = usStartAddr + usRegsIndex;
                        pstMBSlaveVar = &( pstSlaveBlk->stModbusSlaveVar[usVarRgnIndex]);

                        /* Modbus Variable：If not active, fill 0 */
                        if((uint8_t)(MODBUS_VAR_ACTIVE) == pstMBSlaveVar->ucActive )
                        {
                            pstMBVar = &( pstMBSlaveVar->stModbusVar );

                            if((uint8_t)RTDATA_G_AREA == pstMBVar->ucRefID )
                            {
                                pucVarAddr = &pucGAddr[pstMBVar->uiOffset];
                            }
                            else if((uint8_t)RTDATA_I_AREA == pstMBVar->ucRefID )
                            {
                                pucVarAddr = &pucIAddr[pstMBVar->uiOffset];
                            }
                            else
                            {
                                pucVarAddr = &pucQAddr[pstMBVar->uiOffset];
                            }

                            /* Register Index: 2 bytes add 1 */
                            usRegsIndex += pstMBVar->usVarSize / 2U;

                            /* Value: 2/4/8 bytes */
                            /* CPU - Big Endian */
                            if( 2U == pstMBVar->usVarSize )
                            {
                                pucRespData[usDataIndex++] = pucVarAddr[0];
                                pucRespData[usDataIndex++] = pucVarAddr[1];
                            }
                            else if( 4U == pstMBVar->usVarSize )
                            {
                                /* H: 0,1,2,3 / L: 2,3,0,1 */
                                if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                {
                                    pucRespData[usDataIndex++] = pucVarAddr[0];
                                    pucRespData[usDataIndex++] = pucVarAddr[1];
                                    pucRespData[usDataIndex++] = pucVarAddr[2];
                                    pucRespData[usDataIndex++] = pucVarAddr[3];
                                }
                                else
                                {
                                    pucRespData[usDataIndex++] = pucVarAddr[2];
                                    pucRespData[usDataIndex++] = pucVarAddr[3];
                                    pucRespData[usDataIndex++] = pucVarAddr[0];
                                    pucRespData[usDataIndex++] = pucVarAddr[1];
                                }
                            }
                            else if( 8U == pstMBVar->usVarSize )
                            {
                                /* 8: H: 0-7 / L: 2,3,0,1  6,7,4,5 */
                                if((uint8_t)(FP_ORDER_H) == ucFPOrder )
                                {
                                    pucRespData[usDataIndex++] = pucVarAddr[0];
                                    pucRespData[usDataIndex++] = pucVarAddr[1];
                                    pucRespData[usDataIndex++] = pucVarAddr[2];
                                    pucRespData[usDataIndex++] = pucVarAddr[3];

                                    pucRespData[usDataIndex++] = pucVarAddr[4];
                                    pucRespData[usDataIndex++] = pucVarAddr[5];
                                    pucRespData[usDataIndex++] = pucVarAddr[6];
                                    pucRespData[usDataIndex++] = pucVarAddr[7];
                                }
                                else
                                {
                                    pucRespData[usDataIndex++] = pucVarAddr[2];
                                    pucRespData[usDataIndex++] = pucVarAddr[3];
                                    pucRespData[usDataIndex++] = pucVarAddr[0];
                                    pucRespData[usDataIndex++] = pucVarAddr[1];

                                    pucRespData[usDataIndex++] = pucVarAddr[6];
                                    pucRespData[usDataIndex++] = pucVarAddr[7];
                                    pucRespData[usDataIndex++] = pucVarAddr[4];
                                    pucRespData[usDataIndex++] = pucVarAddr[5];
                                }
                            }
                            else
                            {
                                DEBUG_MB( "MBVarSize %d != 2/4/8\n", pstMBVar->usVarSize );
                                break;
                            }
                        }
                        else
                        {
                            /* Register Index: 2 bytes add 1 */
                            usRegsIndex += 1U;

                            /* Not Found: Fill with 0 */
                            pucRespData[usDataIndex++] = 0U;
                            pucRespData[usDataIndex++] = 0U;
                        }
                    }/* end of while */
                }
                else
                {
                    DEBUG_MB( "SysGetRTDataAddr Err\n" );
                }
            }
            else
            {
                DEBUG_MB( "SelectRTData Failed\n" );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: AddMBRespData
*
* 功能描述: 添加Modbus 应答数据
*
* 输入参数: bSerlPort：串口标志位；
*          ucFuncCode：功能码；
*          ucFPOrder：端口的FP Order属性；
*          pucReqData：请求帧数据首地址 - 功能码后的第一字节；
*          pucRespData：应答帧数据首地址 - 功能码后的第一字节。
*
* 输出参数: 无
*
* 返 回 值: 数据长度，0代表存在错误
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t AddMBRespData( bool_t bSerlPort, uint8_t ucFuncCode, uint8_t ucFPOrder,
                               uint8_t const pucReqData[], uint8_t pucRespData[])
{
    uint16_t usDataIndex = 0U;
    uint16_t usStartAddr = 0U;
    uint16_t usNumber = 0U;
    uint8_t ucMaxByteCnts = 0U;
    uint8_t ucByteCnts = 0U;
    ModbusSlaveBlock_t const *pstSlaveBlk = NULL;

    if((pucReqData != NULL) &&
       (pucRespData != NULL) &&
       (((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)))
    {
        /* Data */
        if(((uint8_t)(FORCE_SINGLE_COIL) == ucFuncCode) ||
           ((uint8_t)(PRESET_SINGLE_REG) == ucFuncCode) ||
           ((uint8_t)(MODBUS_DIAG) == ucFuncCode) ||
           ((uint8_t)(FORCE_MULT_COILS) == ucFuncCode) ||
           ((uint8_t)(PRESET_MULT_REGS) == ucFuncCode))
        {
            /* Write(5/6/15/16) / Diagnostic(8): Copy the following 4 bytes */
            memcpy( pucRespData, pucReqData, 4U );
            usDataIndex += 4U;
        }
        else
        {
            /* Read Response: Byte Counts(1) + MBData(n) */
            if( bSerlPort )
            {
                ucMaxByteCnts = (uint8_t)((MAX_MBRTU_DATA_LEN) - 1U);
            }
            else
            {
                ucMaxByteCnts = (uint8_t)((MAX_MBTCP_DATA_LEN) - 1U);
            }

            /* Read */
            usStartAddr = (pucReqData[usDataIndex] << 8U) | pucReqData[usDataIndex+1U];
            usNumber = (pucReqData[usDataIndex+2U] << 8U) | pucReqData[usDataIndex+3U];
            if((usStartAddr + usNumber) <= (uint16_t)(MAX_MODBUS_RGN_VAR_NUM))
            {
                switch( ucFuncCode )
                {
                    case (uint8_t)(READ_COIL_STATUS):
                    {
                        /* Byte Counts */
                        ucByteCnts = (uint8_t)((usNumber + 7U) / 8U);
                        if( ucByteCnts <= ucMaxByteCnts )
                        {
                            pucRespData[usDataIndex++] = ucByteCnts;

                            /* Add Response Data */
                            pstSlaveBlk = GetModbusSlaveBlk( &s_stMBSlaveCfgInfo, (uint8_t)(MR_COIL));
                            if((pstSlaveBlk != NULL) &&
                               (ACTIVE == pstSlaveBlk->emActive))
                            {
                                AddBOOLRespData( pstSlaveBlk, usStartAddr, usNumber, &pucRespData[usDataIndex]);
                            }
                            else
                            {
                                memset( &pucRespData[usDataIndex], 0, (size_t)(ucByteCnts));
                            }

                            usDataIndex += ucByteCnts;
                        }
                        else
                        {
                            DEBUG_MB( "MBReq-Read Coil: Num-%d; ByteCnts-%d should <= MaxByteCnts-%d\n", \
                                       usNumber, ucByteCnts, ucMaxByteCnts );
                        }
                    }
                    break;

                    case (uint8_t)(READ_INPUT_STATUS):
                    {
                        /* Byte Counts */
                        ucByteCnts = (uint8_t)((usNumber + 7U) / 8U);
                        if( ucByteCnts <= ucMaxByteCnts )
                        {
                            pucRespData[usDataIndex++] = ucByteCnts;

                            /* Add Response Data */
                            pstSlaveBlk = GetModbusSlaveBlk( &s_stMBSlaveCfgInfo, (uint8_t)(MR_DISCRETE_INPUTS));
                            if((pstSlaveBlk != NULL) &&
                               (ACTIVE == pstSlaveBlk->emActive))
                            {
                                AddBOOLRespData( pstSlaveBlk, usStartAddr, usNumber, &pucRespData[usDataIndex]);
                            }
                            else
                            {
                                memset( &pucRespData[usDataIndex], 0, (size_t)(ucByteCnts));
                            }

                            usDataIndex += ucByteCnts;
                        }
                        else
                        {
                            DEBUG_MB( "MBReq-Read Input Status: Num-%d; ByteCnts-%d should <= MaxByteCnts-%d\n", \
                                       usNumber, ucByteCnts, ucMaxByteCnts );
                        }
                    }
                    break;

                    case (uint8_t)(READ_HOLD_REGS):
                    {
                        /* Byte Counts */
                        if( usNumber <= (ucMaxByteCnts/2U))
                        {
                            ucByteCnts = usNumber * 2U;
                            pucRespData[usDataIndex++] = ucByteCnts;

                            /* Add Response Data */
                            pstSlaveBlk = GetModbusSlaveBlk( &s_stMBSlaveCfgInfo, (uint8_t)(MR_HOLD_REGS));
                            if((pstSlaveBlk != NULL) &&
                               (ACTIVE == pstSlaveBlk->emActive))
                            {
                                AddRegsRespData( pstSlaveBlk, ucFPOrder, usStartAddr, usNumber, \
                                                 &pucRespData[usDataIndex]);
                            }
                            else
                            {
                                memset( &pucRespData[usDataIndex], 0, (size_t)(ucByteCnts));
                            }

                            usDataIndex += ucByteCnts;
                        }
                        else
                        {
                            DEBUG_MB( "MBReq-Read Hold Regs: Num-%d should <= %d\n", \
                                       usNumber, ucMaxByteCnts/2 );
                        }
                    }
                    break;

                    case (uint8_t)(READ_INPUT_REGS):
                    {
                        /* Byte Counts */
                        if( usNumber <= (ucMaxByteCnts/2U))
                        {
                            ucByteCnts = usNumber * 2U;
                            pucRespData[usDataIndex++] = ucByteCnts;

                            /* Add Response Data */
                            pstSlaveBlk = GetModbusSlaveBlk( &s_stMBSlaveCfgInfo, (uint8_t)(MR_INPUT_REGS));
                            if((pstSlaveBlk != NULL) &&
                               (ACTIVE == pstSlaveBlk->emActive))
                            {
                                AddRegsRespData( pstSlaveBlk, ucFPOrder, usStartAddr, usNumber, \
                                                 &pucRespData[usDataIndex]);
                            }
                            else
                            {
                                memset( &pucRespData[usDataIndex], 0, (size_t)(ucByteCnts));
                            }

                            usDataIndex += ucByteCnts;
                        }
                        else
                        {
                            DEBUG_MB( "MBReq-Read Input Regs: Num-%d should <= %d\n", \
                                       usNumber, ucMaxByteCnts/2 );
                        }
                    }
                    break;

                    default:
                    {
                        DEBUG_MB( "MBReq FuncCode: %d != 1/2/3/4\n", ucFuncCode );
                    }
                    break;
                }/* end of switch */
            }/* end of if */
            else
            {
                DEBUG_MB( "MBReq: usStartAddr:%d + usNumber:%d should <= %d\n", \
                           usStartAddr, usNumber, (MAX_MODBUS_RGN_VAR_NUM));
            }
        }/* end of else - read */
    }

    return usDataIndex;
}

/*
***********************************************************************************************************************
* 函数名称: SendModbusTCPResp
*
* 功能描述: 发送应答帧
*
* 输入参数: pstHead：请求帧头信息；
*          pucResp：应答帧；
*          usRespLen：应答帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void SendModbusTCPResp( NetMsgHead_t const *pstHead, uint8_t ucMBTCPMstrIndex,
                               uint8_t const *pucResp, uint8_t usRespLen )
{
    uint8_t ucResult = (uint8_t)(SEND_SUCCESS);

    if((pstHead != NULL) &&
       (pucResp != NULL) &&
       (ucMBTCPMstrIndex < (uint8_t)(MAX_MBTCP_MSTR_CNT)))
    {
        /* Send To Modbus TCP Master Station */
        ucResult = Send( pucResp, usRespLen,
                         pstHead->uiIPAddr, pstHead->usPort, pstHead->emNetwork,
                         TYPE_MBTCP_MSTR, ucMBTCPMstrIndex );

        /* Check Result */
        if( ucResult != (uint8_t)(SEND_SUCCESS))
        {
            DEBUG_MB( "Net[%d] SendModbusTCPResp Failed: Reason %d\n", pstHead->emNetwork+1, ucResult );
        }
        else
        {
            DEBUG_MB( "Net[%d] SendModbusTCPResp OK\n", pstHead->emNetwork+1 );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSerial
*
* 功能描述: 处理与串口相关的配置参数，收发Modbus 串行数据
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusSerial( void )
{
    uint8_t ucCOMIndex = 0U;
    uint32_t uiPortIndex = 0U;
    bool_t bHandleCfg = false;
    bool_t bRTUProt = false;    /* RTU Protocol Flag */
    COMConfigInfo_t const *pstCOMCfg = NULL;

    /* Loop Serial Port */
    for( ucCOMIndex = 0U; ucCOMIndex < NUM_OF_SERIAL_PORT; ucCOMIndex++ )
    {
        uiPortIndex = PORT_COM1 + ucCOMIndex;

        /* Get Modbus Serial Configuration */
        pstCOMCfg = GetCurCOMCfg( &( s_stSerlCfgInfo[ucCOMIndex]));
        if( pstCOMCfg != NULL )
        {
            /* Handle Configuration Flag */
            if((pstCOMCfg->ucProtocol != (uint8_t)(MODBUS_MASTER_RTU)) &&
               (pstCOMCfg->ucProtocol != (uint8_t)(MODBUS_MASTER_ASCII)))
            {
                bHandleCfg = true;
            }
            else /* MASTER RTU/ASCII*/
            {
                if( IDLE_STATE == s_stPortMstrInfo[uiPortIndex].emState )
                {
                    bHandleCfg = true;
                }
            }

            /* Handle Port Configuration */
            if( bHandleCfg )
            {
                HandlePortConfig( uiPortIndex );

                /* Update pstCOMCfg */
                pstCOMCfg = GetCurCOMCfg( &( s_stSerlCfgInfo[ucCOMIndex]));
            }

            if((pstCOMCfg != NULL) &&
               (ACTIVE == pstCOMCfg->emActive))
            {
                if((pstCOMCfg->ucProtocol >= (uint8_t)(MODBUS_MASTER_RTU)) &&
                   (pstCOMCfg->ucProtocol <= (uint8_t)(MODBUS_SLAVE_ASCII)))
                {
                    if(((uint8_t)(MODBUS_MASTER_RTU) == pstCOMCfg->ucProtocol) ||
                       ((uint8_t)(MODBUS_MASTER_ASCII) == pstCOMCfg->ucProtocol))
                    {
                        if((uint8_t)(MODBUS_MASTER_RTU) == pstCOMCfg->ucProtocol )
                        {
                            bRTUProt = true;
                        }
                        else
                        {
                            bRTUProt = false;
                        }

                        HandleModbusSerlMaster( uiPortIndex, bRTUProt );
                    }
                    else /* Slave RTU/ASCII */
                    {
                        if((uint8_t)(MODBUS_SLAVE_RTU) == pstCOMCfg->ucProtocol )
                        {
                            bRTUProt = true;
                        }
                        else
                        {
                            bRTUProt = false;
                        }

                        HandleModbusSerlSlave( uiPortIndex, bRTUProt );
                    }
                }
            }
            else
            {
                SysSetCM2PMModbusState(uiPortIndex, 0);
                ucModbusOnlineCnt[uiPortIndex] = 0;
            }
        }
        else
        {
            DEBUG_MB( "GetCurCOMCfg Failed\n" );
        }
    }/* end of for */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CalcLRC
*
* 功能描述: 计算 LRC：RTU: Slave ID + Function Code + Data
*
* 输入参数: pucData：数据缓存区；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: LRC值
*
* 注意事项: 消息格式为RTU
***********************************************************************************************************************
*/

static uint8_t CalcLRC( uint8_t const pucData[], uint16_t usDataLen )
{
    uint8_t ucLRC = 0U;
    uint8_t ucSum = 0U;
    uint16_t usIndex = 0U;

    if( pucData != NULL )
    {
        /* Sum */
        for( usIndex = 0U; usIndex < usDataLen; usIndex++ )
        {
            ucSum += pucData[usIndex];
        }

        ucLRC = ~ucSum;
        ucLRC++;
    }

    return ucLRC;
}

/*
***********************************************************************************************************************
* 函数名称: CalcCRC
*
* 功能描述: 计算CRC：Slave ID + Function Code + Data
*
* 输入参数: pucData：数据缓存区；
*          usDataLen：数据长度。
*
* 输出参数: 无
*
* 返 回 值: CRC值
*
* 注意事项: 无
***********************************************************************************************************************
*/

static uint16_t CalcCRC( uint8_t const pucData[], uint16_t usDataLen )
{
    uint16_t usCRC = 0xFFFFU;   /* CRC Result */
    uint8_t ucCRCHi = 0xFFU;    /* High byte of CRC initialized */
    uint8_t ucCRCLo = 0xFFU;    /* Low byte of CRC initialized */
    uint32_t uiIndex = 0U;      /* Will index into CRC lookup table */
    uint16_t usDataIndex = 0U;  /* Data Index */

    if( pucData != NULL )
    {
        while( usDataLen-- )     /* Pass through message buffer */
        {
            uiIndex = ucCRCLo ^ pucData[usDataIndex];    /* Calculate the CRC */
            usDataIndex++;
            ucCRCLo = ucCRCHi ^ s_ucAuchCRCHi[uiIndex];
            ucCRCHi = s_ucAuchCRCLo[uiIndex];
        }

        usCRC = (ucCRCHi << 8U) | ucCRCLo;
    }

    return usCRC;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusRTUMsg
*
* 功能描述: 检查Modbus RTU消息是否有效: Slave ID, CRC
*
* 输入参数: ucSlaveID：端口从站ID；
*          pucMsg：消息首地址；
*          usMsgLen：消息长度。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusRTUMsg( uint8_t ucSlaveID, uint8_t const pucMsg[], uint16_t usMsgLen )
{
    bool_t bValid = false;
    uint16_t usCRC = 0U;

    if((pucMsg != NULL) &&
       ((usMsgLen > (uint16_t)(MBRTU_MSG_NONDATA_LEN)) && (usMsgLen <= (uint16_t)(MAX_MBRTU_MSG_LEN))))
    {
        bValid = true;

        /* Slave ID */
        if( pucMsg[MBRTU_SLAVEID_INDEX] != ucSlaveID )
        {
            bValid = false;  /* Discard */
        }

        if( bValid )
        {
            /* CRC: 2 Bytes */
            usCRC = CalcCRC( pucMsg, usMsgLen - 2U );
            if( usCRC != ((pucMsg[usMsgLen-1] << 8U) | pucMsg[usMsgLen-2U]))
            {
                bValid = false;
                DEBUG_MB( "MBRTUMsg CRC Err: Calc 0x%04X, Msg 0x%02X 0x%02X\n", \
                           usCRC, pucMsg[usMsgLen-1U], pucMsg[usMsgLen-2U]);
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: CheckModbusASCIIMsg
*
* 功能描述: 检查 Modbus ASCII消息(已被底层转换为RTU格式)是否有效: Slave ID, LRC
*
* 输入参数: ucSlaveID：端口从站ID；
*          pucASCIIMsg：消息首地址；
*          usASCIIMsgLen：消息长度。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckModbusASCIIMsg( uint8_t ucSlaveID, uint8_t const pucASCIIMsg[], uint16_t usASCIIMsgLen )
{
    bool_t bValid = false;
    uint8_t ucLRC = 0U;

    /* Calculate non-data and maximum message length of the transfered ASCII Message: */
    /* ASCII: LRC(1B); RTU: CRC(2B) */
    uint16_t usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN) - 1U;
    uint16_t usMaxMsgLen = (uint16_t)(MAX_MBRTU_MSG_LEN) - 1U;

    if((pucASCIIMsg != NULL) &&
       ((usASCIIMsgLen > usNonDataLen) && (usASCIIMsgLen <= usMaxMsgLen)))
    {
        bValid = true;

        /* Slave ID */
        if( pucASCIIMsg[MBRTU_SLAVEID_INDEX] != ucSlaveID )
        {
            bValid = false;  /* Discard */
        }

        if( bValid )
        {
            /* LRC: 1 Byte */
            ucLRC = CalcLRC( pucASCIIMsg, usASCIIMsgLen - 1U );
            if( ucLRC != ( pucASCIIMsg[usASCIIMsgLen - 1U]))
            {
                bValid = false;
                DEBUG_MB( "MBASCIIMsgLRC Err: Calc 0x%02X, Msg 0x%02X\n", ucLRC, pucASCIIMsg[usASCIIMsgLen-1U]);
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: RecvModbusSerlMsg
*
* 功能描述: 接收Modbus RTU/ASCII消息
*
* 输入参数: emPort：端口号；
*          pucBuff：接收缓冲区首地址；
*          pusLen：接收缓冲区长度；
*
* 输出参数: pusLen：数据的实际长度。
*
* 返 回 值: 是否接收成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t RecvModbusSerlMsg( CommPort_t emPort, uint8_t pucBuff[], puint16_t pusLen )
{
    bool_t bSuccess = false;  /* Receive Successfully Flag */

    if(((emPort >= PORT_COM1) && (emPort <= PORT_COM4)) &&
       (pucBuff != NULL) &&
       (pusLen != NULL))
    {
        /* Read Serial Message */
        if( SerialRecv((SerialPortType_t)emPort, pucBuff, pusLen ))
        {
            if( *pusLen > 0U )
            {
                bSuccess = true;
            }
        }
        else
        {
            *pusLen = 0U;
            ClearSerlPortRecvBuff((SerialPortType_t)emPort );
            DEBUG_MB( "COM[%d] SerialRecv Failed\n", emPort-PORT_COM1+1 );
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: SendModbusSerlMsg
*
* 功能描述: 发送Modbus RTU/ASCII消息 - 请求/应答
*
* 输入参数: emPort：端口号；
*          pucData：待发送数据；
*          usDataLen：待发送数据的长度。
*
* 输出参数: 无
*
* 返 回 值: 是否发送成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SendModbusSerlMsg( CommPort_t emPort, uint8_t const pucData[], uint16_t usDataLen )
{
    bool_t bSuccess = false;

    if(((emPort >= PORT_COM1) && (emPort <= PORT_COM4)) &&
        (pucData != NULL) &&
        (usDataLen > 0))
    {
        /* Send through serial port */
        bSuccess = SerialSend((SerialPortType_t)emPort, pucData, usDataLen );
        if( bSuccess )
        {
            DEBUG_MB( "COM[%d] MBSerialSend OK\n", emPort+1 );
        }
        else
        {
            DEBUG_MB( "COM[%d] MBSerialSend Failed\n", emPort+1 );
        }
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSerlMaster
*
* 功能描述: 端口作为Modbus主站，根据配置信息轮询相应从站
*
* 输入参数: emPort：端口号；
*          bRTUProt：RTU协议标志。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusSerlMaster( CommPort_t emPort, bool_t bRTUProt )
{
    PortMstrInfo_t *pstPortMstrInfo = NULL;          /* Port Master Info. */

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
    {
        pstPortMstrInfo = &( s_stPortMstrInfo[emPort]);

        /* Update Wait Cycle Count */
        if( pstPortMstrInfo->uiWaitLoopCycleCnt > 0U )
        {
            pstPortMstrInfo->uiWaitLoopCycleCnt--;
        }

        if( !s_stMBMstrCfgInfo.bCopying )
        {
            if( IDLE_STATE == pstPortMstrInfo->emState )
            {
                HandleMBSerlMstrIdle( emPort, pstPortMstrInfo, bRTUProt );
            }
            else
            {
                HandleMBSerlMstrBusy( emPort, pstPortMstrInfo, bRTUProt );
            }
        }/* end of if-not copying */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleMBSerlMstrIdle
*
* 功能描述: 端口作为Modbus主站时的空闲状态处理
*
* 输入参数: emPort：端口ID号；
*          pstPortMstrInfo：本地保存的端口主站信息；
*          bRTUProt：RTU协议标志。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleMBSerlMstrIdle( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo, bool_t bRTUProt )
{
    uint32_t uiBlkCnt = 0U;
    uint32_t uiCheckCnt = 0U;
    uint8_t ucBlkID = 0U;
    bool_t bBlkStart = false;
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    uint8_t ucCOMIndex = 0U;
    ModbusMasterBlock_t const *pstMBMstrBlk = NULL;  /* Master Block */

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4) &&
       (pstPortMstrInfo != NULL))
    {
        ucCOMIndex = emPort - PORT_COM1;

        if((s_stMBMstrCfgInfo.bValid) &&
           (0U == pstPortMstrInfo->uiWaitLoopCycleCnt))
        {
            uiBlkCnt = GetPortMBMstrBlkCnt( &s_stMBMstrCfgInfo, emPort );  /* Port Modbus Master Block Count */
            while( uiCheckCnt < uiBlkCnt )
            {
                pstMBMstrBlk = GetPortMBMstrBlkInfo( &s_stMBMstrCfgInfo, emPort, pstPortMstrInfo->uiBlockIndex, \
                                                     &ucBlkID );
                DEBUG_MB( "COM: %d, BlkCnts: %d, BlkIndex: %d, BlkID: %d\n", \
                           ucCOMIndex+1, uiBlkCnt, pstPortMstrInfo->uiBlockIndex, ucBlkID );

                if((pstMBMstrBlk != NULL) &&
                   (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)))
                {
                    bBlkStart = SysGetModbusMasterBlkStartFlag((uint32_t)ucBlkID );
                    if( bBlkStart )
                    {
                        /* FP Order */
                        ucFPOrder = GetPortFPOrder( emPort );

                        /* Reset Length Info. */
                        pstPortMstrInfo->usEstimateRespLen = 0U;
                        pstPortMstrInfo->stMBReqMsg.usLen = 0U;

                        /* Make */
                        s_stMBSerlMsg.usLen = (uint16_t)sizeof( s_stMBSerlMsg.ucBuff );
                        if( MakeModbusSerlReq( bRTUProt, ucFPOrder, pstPortMstrInfo,
                                               pstMBMstrBlk, &s_stMBSerlMsg ))
                        {
                            if( s_stMBSerlMsg.usLen > 0U )
                            {
                                if( SendModbusSerlMsg( emPort, s_stMBSerlMsg.ucBuff, s_stMBSerlMsg.usLen ))
                                {
                                    pstPortMstrInfo->emState = BUSY_STATE;
                                    pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
                                }
                                else
                                {
                                    /* Send failed: Loop Next Block */
                                    DEBUG_MB( "COM[%d] SendMBSerlReq Failed, Blk-%d\n", \
                                              ucCOMIndex+1, pstPortMstrInfo->uiBlockIndex );

                                    /* Update Block Index */
                                    pstPortMstrInfo->uiBlockIndex++;
                                    if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                                    {
                                        pstPortMstrInfo->uiBlockIndex = 0U;
                                    }
                                }
                            }
                        }
                        else
                        {
                            DEBUG_MB( "MakeMBSerlReq Failed: COM-%d, Blk-%d\n", \
                                       ucCOMIndex+1, pstPortMstrInfo->uiBlockIndex );
                            pstPortMstrInfo->uiBlockIndex = 0U;
                        }

                        break;
                    }/* end of if-start */
                    else
                    {
                        DEBUG_MB( "MBMstrBlkStartFlag 0: COM-%d, Blk-%d\n", \
                                   ucCOMIndex+1, pstPortMstrInfo->uiBlockIndex );

                        /* Update Block Index */
                        pstPortMstrInfo->uiBlockIndex++;
                        if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                        {
                            pstPortMstrInfo->uiBlockIndex = 0U;
                        }

                        uiCheckCnt++;  /* Check Count */
                    }
                }
                else
                {
                    pstPortMstrInfo->uiBlockIndex = 0U;
                    break;
                }
            }/* end of while */
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleMBSerlMstrBusy
*
* 功能描述: 端口作为Modbus主站时的忙状态处理
*
* 输入参数: emPort：端口ID号；
*          pstPortMstrInfo：本地保存的端口主站信息；
*          bRTUProt：RTU协议标志。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleMBSerlMstrBusy( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo, bool_t bRTUProt )
{
    uint8_t ucCOMIndex = 0U;
    uint8_t ucBlkID = 0U;
    uint16_t usLen = 0U;
    uint32_t uiBlkCnt = 0U;
    ModbusMasterBlock_t const *pstMBMstrBlk = NULL;

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4) &&
       (pstPortMstrInfo != NULL))
    {
        ucCOMIndex = emPort - PORT_COM1;

        /* Wait / Handle Slave Response */
        pstPortMstrInfo->uiWaitLoopCycleCnt = 0U;
        usLen = (uint16_t)sizeof( s_ucRespBuff );
        if( !RecvModbusSerlMsg( emPort, s_ucRespBuff, &usLen ))
        {
            usLen = 0U;
        }

        if( usLen > 0U )
        {
            ucModbusOnlineCnt[emPort] = ucModbusOnlineCnt[emPort] + 1;

            /* Has Response */
            if( pstPortMstrInfo->bHandleSwitch )
            {
                /* Port Modbus Master Info. Changed: Discard Data And Re-Loop */
                DEBUG_MB( "COM[%d] HandleSwitch-Discard And Re-Loop\n", ucCOMIndex+1 );
                pstPortMstrInfo->uiBlockIndex = 0U;
                pstPortMstrInfo->bHandleSwitch = false;
            }
            else
            {
                if( s_stMBMstrCfgInfo.bValid )
                {
                    pstMBMstrBlk = GetPortMBMstrBlkInfo( &s_stMBMstrCfgInfo, emPort, pstPortMstrInfo->uiBlockIndex, \
                                                         &ucBlkID );
                    if((pstMBMstrBlk != NULL) &&
                       (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)))
                    {
                        HandleModbusSerlResp( bRTUProt, emPort, pstMBMstrBlk, ucBlkID, pstPortMstrInfo, \
                                              s_ucRespBuff, usLen );

                        /* Update Block Index */
                        uiBlkCnt = GetPortMBMstrBlkCnt( &s_stMBMstrCfgInfo, emPort );
                        pstPortMstrInfo->uiBlockIndex++;
                        if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                        {
                            pstPortMstrInfo->uiBlockIndex = 0U;

                            SysSetCM2PMModbusState(emPort, 1);
                            ucModbusOnlineCnt[emPort] = 0;
                        }

                        /* Wait To Loop Next Block */
                        pstPortMstrInfo->uiWaitLoopCycleCnt = (uint32_t)(LOOPBLK_INVL_CYCLE_CNTS);
                    }
                    else
                    {
                        DEBUG_MB( "GetPortMBMstrBlkInfo Err\n" );
                        pstPortMstrInfo->uiBlockIndex = 0U;
                    }
                }
                else
                {
                    DEBUG_MB( "s_stMBMstrCfgInfo Invalid\n" );
                    pstPortMstrInfo->uiBlockIndex = 0U;
                }
            }

            pstPortMstrInfo->emState = IDLE_STATE;
            pstPortMstrInfo->uiWaitRespCycleCnt = 0U;
        }
        else
        {
            /* No Response */
            pstPortMstrInfo->uiWaitRespCycleCnt++;
            if( pstPortMstrInfo->uiWaitRespCycleCnt > (uint32_t)(MBSERL_RESP_TO_CYCLE_CNT))
            {
                /* Timeout */
                DEBUG_MB( "MBSerl Slave Timeout: COM-%d, Blk-%d\n", \
                           ucCOMIndex+1, pstPortMstrInfo->uiBlockIndex );
                HandleModbusSlaveTimeout( emPort, pstPortMstrInfo, &s_stMBMstrCfgInfo );
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: MakeModbusSerlReq
*
* 功能描述: 构造Modbus RTU/ASCII请求帧：ASCII转换由底层完成
*
* 输入参数: bRTUProt：RTU协议标志；
*          ucFPOrder：串口FP Order属性；
*          pstPortMstrInfo：端口主站信息；
*          pstMBMstrBlk：指向Modbus 主站块；
*          pstMBReqMsg：用于存放构造好的Modbus RTU/ASCII请求帧。
*
* 输出参数: 无
*
* 返 回 值: 是否构造成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t MakeModbusSerlReq( bool_t bRTUProt, uint8_t ucFPOrder, PortMstrInfo_t *pstPortMstrInfo,
                                 ModbusMasterBlock_t const *pstMBMstrBlk, ModbusMsg_t *pstMBReqMsg )
{
    bool_t bResult = false;    /* Make Successfully Flag */
    uint8_t ucLRC = 0U;        /* LRC Of ASCII Request Message */
    uint16_t usCRC = 0U;       /* CRC Of RTU Request Message */
    puint8_t pucReq = NULL;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
    uint16_t usByteCounts = 0U;
    uint16_t usDataLen = 0U;
    uint16_t usIndex = 0U;
    uint16_t usReqDataLen = 0U;
    ModbusMasterBlockHead_t const *pstBlkHead = NULL;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usPerLen = 0U;
#endif

    if((((uint8_t)(FP_ORDER_H) == ucFPOrder) || ((uint8_t)(FP_ORDER_L) == ucFPOrder)) &&
       (pstPortMstrInfo != NULL) &&
       (pstMBMstrBlk != NULL) &&
       (pstMBReqMsg != NULL))
    {
        pstBlkHead = &( pstMBMstrBlk->stModbusMasterBlockHead );
        ucFuncCode = GetModbusFuncCode( pstBlkHead );
        if( ucFuncCode != (uint8_t)(INVALID_MB_FUNCCODE))
        {
            /* Make Request Message：RTU With CRC(2B); */
            /*                       ASCII( Transfered by low layer ) With LRC(1B) */
            usDataLen = GetModbusReqDataLen( pstBlkHead, ucFuncCode );

            /* Check Data Length */
            if(( usDataLen > 0U ) &&
               ( usDataLen <= (uint16_t)(MAX_MBRTU_DATA_LEN)))
            {
                bResult = true;
                pstMBReqMsg->usLen = 0U;
                pucReq = pstMBReqMsg->ucBuff;

                /* Modbus RTU Message */
                pucReq[usIndex++] = (uint8_t)pstBlkHead->uiTargetStationID;  /* Slave ID */
                pucReq[usIndex++] = ucFuncCode;                              /* Function Code */

                /* Data */
                if((uint8_t)(VM_READ) == pstBlkHead->ucVisitMode )
                {
                    usReqDataLen = AddReadReqData( pstBlkHead, &pucReq[usIndex]);
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;

                        /* Response Msg: Slave ID(1) + Function Code(1) + Byte Counts(1) + Data(n) + CRC(2)/LRC(1) */
                        if(((uint8_t)(MR_COIL) == pstBlkHead->ucModbusRgn) ||
                           ((uint8_t)(MR_DISCRETE_INPUTS) == pstBlkHead->ucModbusRgn))
                        {
                            usByteCounts = (uint16_t)((pstBlkHead->uiTargetSize + 7U) / 8U);
                        }
                        else
                        {
                            usByteCounts = (uint16_t)(pstBlkHead->uiTargetSize * 2U);
                        }

                        if( bRTUProt )
                        {
                            /* SlaveID(1B) + FuncCode(1B) + ByteCnts(1B) + MBData + CRC(2B) */
                            pstPortMstrInfo->usEstimateRespLen = 5U + usByteCounts;
                        }
                        else
                        {
                            /* SlaveID(1B) + FuncCode(1B) + ByteCnts(1B) + MBData + LRC(1B) */
                            pstPortMstrInfo->usEstimateRespLen = 4U + usByteCounts;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "AddReadReqData Failed\n" );
                        bResult = false;
                    }
                }
                else
                {
                    usReqDataLen = AddWriteReqData( pstMBMstrBlk, ucFuncCode, &pucReq[usIndex], ucFPOrder );
                    if( usReqDataLen > 0U )
                    {
                        usIndex += usReqDataLen;

                        /* Response Msg: Slave ID(1) + Function Code(1) + Start Addr(2) + Number(2) + CRC(2)/LRC(1) */
                        if( bRTUProt )
                        {
                            pstPortMstrInfo->usEstimateRespLen = 8U;
                        }
                        else
                        {
                            pstPortMstrInfo->usEstimateRespLen = 7U;
                        }
                    }
                    else
                    {
                        DEBUG_MB( "AddWriteReqData Failed\n" );
                        bResult = false;
                    }
                }

                if( bResult )
                {
                    if( bRTUProt )
                    {
                        /* Calculate CRC */
                        usCRC = CalcCRC( pucReq, usIndex );

                        /* Set CRC */
                        pucReq[usIndex++] = (uint8_t)(usCRC & 0xFF);
                        pucReq[usIndex++] = (uint8_t)((usCRC >> 8U) & 0xFF);

                        /* Copy To Port Master Info. */
                        memcpy( pstPortMstrInfo->stMBReqMsg.ucBuff, pucReq, (uint32_t)usIndex );
                        pstPortMstrInfo->stMBReqMsg.usLen = usIndex;
                    }
                    else
                    {
                        /* Calculate LRC: Slave ID + Function Code + Data */
                        ucLRC = CalcLRC( pucReq, usIndex );

                        /* Set LRC */
                        pucReq[usIndex++] = ucLRC;

                        /* Copy To Port Master Info. */
                        memcpy( pstPortMstrInfo->stMBReqMsg.ucBuff, pucReq, usIndex );
                        pstPortMstrInfo->stMBReqMsg.usLen = usIndex;
                    }

                    /* Record Request Length */
                    pstMBReqMsg->usLen = usIndex;

#ifdef DEBUG_MODBUS
                    printf( "MBSerlReq Message: RTU Flag-%d, Len-%d, Content: \n", bRTUProt, pstMBReqMsg->usLen );
                    for( usIndex = 0U; usIndex < pstMBReqMsg->usLen; usIndex++ )
                    {
                        printf( "%02X ", pucReq[usIndex]);
                        usPerLen++;

                        if(( 30 == usPerLen ) ||
                           ((pstMBReqMsg->usLen-1) == usIndex))
                        {
                            printf( "\n" );
                            usPerLen = 0U;
                        }
                    }
#endif
                }/* end of if( bResult ) */
            }
            else
            {
                DEBUG_MB( "MBRTUReq Data Len %d Invalid(Max: %d)\n", usDataLen, (MAX_MBRTU_DATA_LEN));
            }
        }
        else
        {
            DEBUG_MB( "GetModbusFuncCode Failed\n" );
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSerlResp
*
* 功能描述: 处理Modbus Serial从站应答帧
*
* 输入参数: bRTUProt：RTU协议标志；
*          emPort：端口号；
*          pstMBMstrBlk：Modbus主站块；
*          ucBlkID：主站块 ID；
*          pstPortMstrInfo：端口主站信息；
*          pucResp：从站应答消息；
*          usRespLen：应答消息的长度。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusSerlResp( bool_t bRTUProt, CommPort_t emPort, ModbusMasterBlock_t const *pstMBMstrBlk,
                                  uint8_t ucBlkID, PortMstrInfo_t *pstPortMstrInfo,
                                  uint8_t const pucResp[], uint16_t usRespLen )
{
    bool_t bValid = false;  /* Valid Flag */
    uint8_t ucSlaveID = 0U;
    uint16_t usIndex = 0U;
    uint16_t usPMDataLen = 0U;
    uint16_t usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN);
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    ModbusMasterBlockHead_t const *pstHead = NULL;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usPerLen = 0U;
#endif

    if(((emPort >= PORT_COM1) && (emPort <= PORT_COM4)) &&
       (pstMBMstrBlk != NULL) &&
       (ucBlkID < (uint8_t)(MAX_MODBUS_BLOCK_NUM)) &&
       (pstPortMstrInfo != NULL) &&
       (pucResp != NULL))
    {
        /* Slave ID */
        ucSlaveID = pstPortMstrInfo->stMBReqMsg.ucBuff[MBRTU_SLAVEID_INDEX];

        /* Check RTU/ASCII Response Message */
        if( bRTUProt )
        {
            bValid = CheckModbusRTUMsg( ucSlaveID, pucResp, usRespLen );
            usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN);
        }
        else
        {
            bValid = CheckModbusASCIIMsg( ucSlaveID, pucResp, usRespLen );
            usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN) - 1U;  /* LRC: 1 Byte; CRC: 2 Bytes */
        }

        if( bValid )
        {
            /* Update estimate response length according to the response function code */
            if((pucResp[MBRTU_FUNCCODE_INDEX] & (uint8_t)(BIT_8)) != 0U )
            {
                /* Exception Message */
                if( bRTUProt )
                {
                    pstPortMstrInfo->usEstimateRespLen = (uint16_t)(MBRTU_EXMSG_LEN);       /* CRC: 2 Bytes */
                }
                else
                {
                    pstPortMstrInfo->usEstimateRespLen = (uint16_t)(MBRTU_EXMSG_LEN) - 1U;  /* LRC: 1 Byte */
                }
            }

            /* Length */
            if( usRespLen == pstPortMstrInfo->usEstimateRespLen )
            {
                pstHead = &( pstMBMstrBlk->stModbusMasterBlockHead );

                /* Check response message: Function Code, Data */
                bValid = CheckMBSerlResp( bRTUProt, pstPortMstrInfo->stMBReqMsg.ucBuff, pucResp, usRespLen );
                if( bValid )
                {
                    if((uint8_t)(VM_READ) == pstHead->ucVisitMode )
                    {
                        /* Port FP Order */
                        ucFPOrder = GetPortFPOrder( emPort );

                        /* Data Area First Index In Read Response Message: */
                        /* Slave ID(1B) + Func Code(1B) + Byte Counts(1B) + Data Area + CRC(2B)/LRC(1B) */
                        usIndex = (uint16_t)(MBRTU_FUNCCODE_INDEX) + 1U;  /* Byte Counts Index */

                        /* Make PM Data Frame */
                        usPMDataLen = (uint16_t)sizeof( s_ucPMDataBuff );
                        MstrMakePMMBMsg( emPort, ucFPOrder, &(s_stMBMstrCfgInfo.stRTDataVer), pstMBMstrBlk, ucBlkID,
                                         &pucResp[usIndex], usRespLen - usNonDataLen, s_ucPMDataBuff, &usPMDataLen );
                        if( usPMDataLen > 0U )
                        {
#ifdef DEBUG_MODBUS
                            printf( "COM[%d] MBMaster Message To PM: Length: %d, Content: \n", \
                                    emPort-PORT_COM1+1, usPMDataLen );

                            for( usIndex = 0U; usIndex < usPMDataLen; usIndex++ )
                            {
                                printf( "%02X ", s_ucPMDataBuff[usIndex]);
                                usPerLen++;

                                if(( 30 == usPerLen ) ||
                                   ((usPMDataLen-1) == usIndex))
                                {
                                    printf( "\n" );
                                    usPerLen = 0U;
                                }
                            }
#endif

                            /* Send To PM */
                            SendMBDataToPM( s_ucPMDataBuff, usPMDataLen );
                        }
                    }/* end of VM_READ */
                }/* end of Valid */
                else
                {
                    DEBUG_MB( "COM[%d] Resp Data Invalid\n", emPort-PORT_COM1+1 );
                }
            }
            else
            {
                DEBUG_MB( "COM[%d] RespLen-%d != %d\n", \
                           emPort-PORT_COM1+1, usRespLen, pstPortMstrInfo->usEstimateRespLen );
            }
        }/* end of if - Valid */
        else
        {
            DEBUG_MB( "COM[%d] MBSerlResp Msg Invalid\n", emPort-PORT_COM1+1 );
        }
    }/* end of if - Parameters Valid */

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CheckMBRTUResp
*
* 功能描述: 检查Modbus Serial从站应答帧的内容是否有效: Function Code, Data
*
* 输入参数: bRTUProt：RTU协议标志；
*          pucReq：Modbus Serial主站的请求帧首地址；
*          pucResp：Modbus Serial从站的应答帧首地址；
*          usRespLen：Modbus Serial从站应答帧的长度。
*
* 输出参数: 无
*
* 返 回 值: 应答帧是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckMBSerlResp( bool_t bRTUProt, uint8_t const pucReq[], uint8_t const pucResp[], uint16_t usRespLen )
{
    bool_t bValid = false;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);
    uint16_t usIndex = 0U;
    uint16_t usDataLen = 0U;
    uint16_t usNonDataLen = 0U;

    /* Non-Data Length */
    if( bRTUProt )
    {
        usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN);       /* CRC: 2 Bytes*/
    }
    else
    {
        usNonDataLen = (uint16_t)(MBRTU_MSG_NONDATA_LEN) - 1U;  /* LRC: 1 Byte */
    }

    if((pucReq != NULL) &&
       (pucResp != NULL) &&
       (usRespLen > usNonDataLen))
    {
        bValid = true;

        /* Function Code */
        usIndex = (uint16_t)(MBRTU_FUNCCODE_INDEX);
        bValid = CkeckMBRespFuncCode( &pucResp[usIndex], pucReq[usIndex]);
        if( bValid )
        {
            /* Data */
            usDataLen = usRespLen - usNonDataLen;
            ucFuncCode = pucReq[usIndex];
            usIndex++;
            bValid = CheckMBRespData( ucFuncCode, usDataLen, &pucResp[usIndex], &pucReq[usIndex]);
            if( !bValid )
            {
                DEBUG_MB( "CheckMBSerlResp: Data Invalid\n" );
            }
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSlaveTimeout
*
* 功能描述: Modbus 从站超时处理
*
* 输入参数: emPort：端口号；
*          pstPortMstrInfo：本地保存的端口主站信息。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusSlaveTimeout( CommPort_t emPort, PortMstrInfo_t *pstPortMstrInfo,
                                      ModbusMstrCfgInfo_t const *pstMBMstrInfo )
{
    uint32_t uiBlkCnt = 0U;

    if(((emPort >= PORT_COM1) && (emPort <= PORT_NET2)) &&
       (pstPortMstrInfo != NULL) &&
       (pstMBMstrInfo != NULL))
    {
        pstPortMstrInfo->emState = IDLE_STATE;
        pstPortMstrInfo->uiWaitRespCycleCnt = 0U;

        /* Block Index */
        if( pstPortMstrInfo->bHandleSwitch )
        {
            /* Port Modbus Master Info. Changed: Re-Loop */
            pstPortMstrInfo->uiBlockIndex = 0U;
            pstPortMstrInfo->bHandleSwitch = false;
        }
        else
        {
            if( pstMBMstrInfo->bValid )
            {
                /* Loop Next Block */
                uiBlkCnt = GetPortMBMstrBlkCnt( pstMBMstrInfo, emPort );
                pstPortMstrInfo->uiBlockIndex++;
                if( pstPortMstrInfo->uiBlockIndex >= uiBlkCnt )
                {
                    pstPortMstrInfo->uiBlockIndex = 0U;

                    if(ucModbusOnlineCnt[emPort] > 0)
                    {
                        SysSetCM2PMModbusState(emPort, 1);
                    }
                    else
                    {
                        SysSetCM2PMModbusState(emPort, 0);
                    }
                    ucModbusOnlineCnt[emPort] = 0;
                }
            }
            else  /* Invalid */
            {
                pstPortMstrInfo->uiBlockIndex = 0U;
                SysSetCM2PMModbusState(emPort, 0);
                ucModbusOnlineCnt[emPort] = 0;
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleModbusSerlSlave
*
* 功能描述: 端口作为Modbus从站，处理Modbus主站的请求并应答
*
* 输入参数: emPort：端口号；
*          bRTUProt：RTU协议标志。
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void HandleModbusSerlSlave( CommPort_t emPort, bool_t bRTUProt )
{
    uint8_t ucCOMIndex = 0U;  /* COM Port Index */
    uint8_t ucSlaveID = 0U;   /* Modbus Slave ID */
    uint16_t usReqLen = 0U;   /* Request Length */
    uint16_t usRespLen = 0U;  /* Response Message Length */

#ifdef DEBUG_MODBUS
    /* only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    if((emPort >= PORT_COM1) && (emPort <= PORT_COM4))
    {
        ucCOMIndex = (uint8_t)(emPort - PORT_COM1);
        ucSlaveID = GetPortMBSlaveID( emPort );

        /* Handle if slaveID valid */
        if((ucSlaveID >= (uint8_t)(MIN_MODBUS_SLAVE_ID)) &&
           (ucSlaveID <= (uint8_t)(MAX_MODBUS_SLAVE_ID)))
        {
            /* Handle when configuration copying finished and valid */
            if((!s_stMBSlaveCfgInfo.bCopying) &&
               (s_stMBSlaveCfgInfo.bValid))
            {
                /* Read Modbus Serial Request Message */
                usReqLen = (uint16_t)sizeof( s_ucDataBuff );
                if( !RecvModbusSerlMsg( emPort, s_ucDataBuff, &usReqLen ))
                {
                    usReqLen = 0U;
                }

                if( usReqLen > 0U )
                {
                    /* Has Request Message */
                    usRespLen = (uint16_t)sizeof( s_ucRespBuff );
                    DecodeModbusSerlReq( ucSlaveID, bRTUProt, emPort, s_ucDataBuff, usReqLen,
                                         s_ucRespBuff, &usRespLen );
                    if( usRespLen > 0U )
                    {
#ifdef DEBUG_MODBUS
                        printf( "COM[%d] MBSlave RespMsg: Length: %d, Content: \n", ucCOMIndex+1, usRespLen );

                        for( usIndex = 0U; usIndex < usRespLen; usIndex++ )
                        {
                            printf( "%02X ", s_ucRespBuff[usIndex]);
                            usPerLen++;

                            if(( 30U == usPerLen ) ||
                               ((usRespLen-1U) == usIndex))
                            {
                                printf( "\n" );
                                usPerLen = 0U;
                            }
                        }
#endif

                        /* Send Response Message */
                        if( !SendModbusSerlMsg( emPort, s_ucRespBuff, usRespLen ))
                        {
                            DEBUG_MB( "COM[%d] SendMBSerlRespMsg Failed\n", ucCOMIndex+1 );
                        }
                    }
                }/* end of if - Has Request */
            }/* end of if - Configuration */
        }/* end of if - Slave ID */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: DecodeModbusSerlReq
*
* 功能描述: 解析请求帧，根据解析结果构造应答帧，帧有效且为写操作时，构造将发往PM的数据帧
*          ASCII格式的请求帧已由底层转换为RTU形式，LRC(1B)
*
* 输入参数: ucSlaveID：Modbus从站ID;
*          bRTUProt：RTU协议标志；
*          emPort：端口号；
*          pucReq：请求帧首地址；
*          usReqLen：请求帧长度；
*          pucResp：应答帧首地址；
*
* 输出参数: pusRespLen：返回应答帧的实际长度。
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

static void DecodeModbusSerlReq( uint8_t ucSlaveID, bool_t bRTUProt, CommPort_t emPort,
                                 uint8_t const pucReq[], uint16_t usReqLen,
                                 uint8_t pucResp[], puint16_t pusRespLen )
{
    bool_t bValid = false;
    uint8_t ucCOMIndex = 0U;
    uint8_t ucFuncCode = (uint8_t)(INVALID_MB_FUNCCODE);  /* Function Code */
    ModbusException_t emExcCode = MBEX_NONE;              /* Exception Code */
    uint16_t usCRC = 0U;
    uint8_t ucLRC = 0U;
    uint16_t usPMDataLen = 0U;
    uint8_t ucFPOrder = (uint8_t)(FP_ORDER_INVALID);
    uint16_t usRespIndex = 0U;
    uint16_t usRespDataLen = 0U;
    uint16_t usReqDataLen = 0U;

#ifdef DEBUG_MODBUS
    /* Only for print message */
    uint16_t usIndex = 0U;
    uint16_t usPerLen = 0U;
#endif

    if(((ucSlaveID >= (uint8_t)(MIN_MODBUS_SLAVE_ID)) && (ucSlaveID <= (uint8_t)(MAX_MODBUS_SLAVE_ID))) &&
       ((emPort >= PORT_COM1) && (emPort <= PORT_COM4)) &&
       (pucReq != NULL) &&
       (pucResp != NULL) &&
       (pusRespLen != NULL))
    {
        ucCOMIndex = emPort - PORT_COM1;
        *pusRespLen = 0U;  /* Reset Response Length */

        /* Check RTU/ASCII Request Message: ASCII has been transfered by lower layer */
        if( bRTUProt )
        {
            bValid = CheckModbusRTUMsg( ucSlaveID, pucReq, usReqLen );
        }
        else
        {
            bValid = CheckModbusASCIIMsg( ucSlaveID, pucReq, usReqLen );
        }

        /* Make Response Message If Request Valid */
        if( bValid )
        {
            /* Slave ID */
            pucResp[usRespIndex++] = ucSlaveID;

            /* Check Request Function Code */
            ucFuncCode = pucReq[MBRTU_FUNCCODE_INDEX];
            if( !CheckReqFuncCode( ucFuncCode ))
            {
                DEBUG_MB( "COM[%d]: Not Support ReqFuncCode-%d\n", ucCOMIndex+1, ucFuncCode );
                emExcCode = MBEX_ILLEGAL_FUNC;

                /* Exception Response Message */
                pucResp[usRespIndex++] = ucFuncCode | (uint8_t)(BIT_8);
                pucResp[usRespIndex++] = emExcCode;
            }
            else
            {
                /* Make Normal Response Message */
                pucResp[usRespIndex++] = ucFuncCode;
                ucFPOrder = GetPortFPOrder( emPort );
                usRespDataLen = AddMBRespData( true, ucFuncCode, ucFPOrder, &pucReq[usRespIndex], \
                                               &pucResp[usRespIndex]);
                if((usRespDataLen > 0U) && (usRespDataLen <= (uint16_t)(MAX_MBRTU_DATA_LEN)))
                {
                    usRespIndex += usRespDataLen;

                    /* When Write Operation, Write to PM */
                    if(((uint8_t)(FORCE_SINGLE_COIL) == ucFuncCode) ||
                       ((uint8_t)(PRESET_SINGLE_REG) == ucFuncCode) ||
                       ((uint8_t)(FORCE_MULT_COILS) == ucFuncCode)  ||
                       ((uint8_t)(PRESET_MULT_REGS) == ucFuncCode))
                    {
                        /* Port Write Enable */
                        if( GetPortWriteEnable( emPort ))
                        {
                            if( bRTUProt )
                            {
                                /* RTU:          Slave ID(1) + Function Code(1) + Data + CRC(2) */
                                usReqDataLen = usReqLen - (uint16_t)(MBRTU_MSG_NONDATA_LEN);
                            }
                            else
                            {
                                /* ASCII(->RTU): Slave ID(1) + Function Code(1) + Data + LRC(1) */
                                usReqDataLen = usReqLen - ((uint16_t)(MBRTU_MSG_NONDATA_LEN) - 1);
                            }

                            /* Make PM Data Frame */
                            usPMDataLen = (uint16_t)sizeof( s_ucPMDataBuff );
                            SlaveMakePMMBMsg( emPort, ucFPOrder, &pucReq[MBRTU_FUNCCODE_INDEX], usReqDataLen, \
                                              s_ucPMDataBuff, &usPMDataLen );
                            if( usPMDataLen > 0U )
                            {
#ifdef DEBUG_MODBUS
                                printf( "COM[%d] MBSlave Message To PM: Length: %d, Content: \n", \
                                        ucCOMIndex+1, usPMDataLen );

                                for( usIndex = 0U; usIndex < usPMDataLen; usIndex++ )
                                {
                                    printf( "%02X ", s_ucPMDataBuff[usIndex]);
                                    usPerLen++;

                                    if(( 30U == usPerLen ) ||
                                       ((usPMDataLen-1U) == usIndex))
                                    {
                                        printf( "\n" );
                                        usPerLen = 0U;
                                    }
                                }
#endif

                                SendMBDataToPM( s_ucPMDataBuff, usPMDataLen );  /* Send To PM */
                            }
                        }
                        else
                        {
                            DEBUG_MB( "COM[%d] PortWriteEnable False\n", ucCOMIndex+1 );
                        }
                    }

                    if( bRTUProt )
                    {
                        /* Calculate CRC */
                        usCRC = CalcCRC( pucResp, usRespIndex );

                        /* Set CRC */
                        pucResp[usRespIndex++] = (uint8_t)( usCRC & 0xFF );
                        pucResp[usRespIndex++] = (uint8_t)((usCRC >> 8U) & 0xFF);

                        /* Update Response Length */
                        *pusRespLen = usRespIndex;
                    }
                    else
                    {
                        /* Calculate LRC: Slave ID + Function Code + Data */
                        ucLRC = CalcLRC( pucResp, usRespIndex );

                        /* Set LRC */
                        pucResp[usRespIndex++] = ucLRC;

                        /* Update Response Length */
                        *pusRespLen = usRespIndex;
                    }
                }/* end of if usRespDataLen > 0U */
                else
                {
                    *pusRespLen = 0U;
                }
            }/* end of else - Non-Exception */
        }/* end of if - Valid */
    }/* end of if - Parameters Valid */
    else
    {
        if( pusRespLen != NULL )
        {
            *pusRespLen = 0U;
        }
    }

    return;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

