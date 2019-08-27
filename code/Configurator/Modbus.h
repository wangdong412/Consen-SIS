// -----------------------------------------------------------------------
// $Workfile: Modbus.hpp $
// Author/Date  : rjvs 2002-01-28 10:20:22 (UTC-6:00)
// Description  : Modbus general types
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2001, Triconex Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/Modbus.hpp 1     2/03/03 1:38p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2003-01-29  Ryan Van Slooten  Moved code to this file
// -----------------------------------------------------------------------

#if !defined(Modbus_H__INCLUDED)
#define      Modbus_H__INCLUDED

namespace modbus
{

// --------------------------------------------------------------------------
// Forward declare classes

//class CMessage;
    //class MessageRange;
    //class MessageVector;

//class CClient;
//class CModbusComm;

// --------------------------------------------------------------------------
// Enumerations

enum ModbusFunction
{
  mbfnUnknown                 = 0,
  mbfnReadCoilStatus          = 1,    // Digital -- Alias 0:00001
  mbfnReadInputStatus         = 2,    // Digital -- Alias 1:00001
  mbfnReadHoldingRegisters    = 3,    // Analog  -- Alias 4:00001
  mbfnReadInputRegisters      = 4,    // Analog  -- Alias 3:00001
  mbfnWriteCoil               = 5,    // Digital -- Alias 0:00001
  mbfnWriteRegister           = 6,    // Analog  -- Alias 4:00001
  mbfnExceptionStatus         = 7,    // Exception
  mbfnDiagnostic              = 8,    // Diagnostic
  mbfnProgram484              = 9,    // NOT SUPPORTED
  mbfnPoll484                 = 10,   // NOT SUPPORTED
  mbfnCommCounter             = 11,   // NOT SUPPORTED
  mbfnCommEventLog            = 12,   // NOT SUPPORTED
  mbfnWriteMultipleCoils      = 15,   // Digital -- Alias 0:00001
  mbfnWriteMultipleRegisters  = 16,   // Analog  -- Alias 4:40001
  mbfnReportServerUnitId      = 17,   // NOT SUPPORTED
  mbfnProgram884M84           = 18,   // NOT SUPPORTED
  mbfnResetCommLink           = 19,   // NOT SUPPORTED
  mbfnReadGeneralReference    = 20,   // NOT SUPPORTED
  mbfnWriteGeneralReference   = 21,   // NOT SUPPORTED
  mbfnMaskWrite4xRegister     = 22,   // NOT SUPPORTED
  mbfnReadWrite4xRegisters    = 23,   // NOT SUPPORTED
  mbfnReadFifoQueue           = 24    // NOT SUPPORTED
};

enum ModbusException
{
  mbexNone                    = 0,
  mbexIllegalFunction         = 1,    // Server does not supported function code
  mbexIllegalDataAddress      = 2,    // Server does not have the requested address space
  mbexIllegalDataValue        = 3,    // The value is not in the range allowed for the address
  mbexDeviceFailure           = 4,    // NOT SUPPORTED
  mbexAcknowledge             = 5,    // NOT SUPPORTED
  mbexBusyRejected            = 6,    // NOT SUPPORTED
  mbexNegativeAcknowledge     = 7,    // NOT SUPPORTED
  mbexMemoryParityError       = 8,    // NOT SUPPORTED
  mbexErrorDoNotRespond       = 10,   // Miscellaneous error:  Do not respond to client
  mbexErrorCrc                = 11,   // CRC error:            Do not respond to client
  mbexErrorTimeout            = 12    // Server timeout:       Do not respond to client
};

enum ModbusDiagnosticCode
{
  mbdiagEcho                  = 0,    // Echo exact message
  mbdiagRestartComm           = 1,    // Restart communications (overrides Listen Only)
  mbdiagDiagnosticRegister    = 2,    // Returns diagnostic register
  mbdiagAsciiDelimiter        = 3,    // NOT SUPPORTED: Changes ASCII delimiter
  mbdiagListenOnly            = 4,    // Cease active communications
  mbdiagClearDiagnostics      = 10,   // Clears all counters and diagnostic registers
  mbdiagTotalMessageCount     = 11,   // Quantity of messages detected
  mbdiagServerCommErrCount    = 12,   // Quantity of CRC errors
  mbdiagServerExceptCount     = 13,   // Quantity of Modbus exceptions
  mbdiagServerMessageCount    = 14,   // Quantity of messages addressed to Modbus server
  mbdiagServerNoResponse      = 15,   // Quantity of addressed messages not responded
  mbdiagServerNegAcknowledge  = 16,   // Quantity of addressed messages negative acknowledged
  mbdiagServerBusy            = 17,   // Quantity of addressed messages busy
  mbdiagServerCharOverrun     = 18,   // Quantity of Server character overrun errors
  mbdiagIOPOverrun            = 19,   // NOT SUPPORTED: 884 IOP Overrun count
  mbdiagClearOverrun          = 20,   // NOT SUPPORTED: Reset overrun count
  mbdiagModbusPlusStatistics  = 21,   // NOT SUPPORTED: Get/Clear Modbus Plus statistics
};

// --------------------------------------------------------------------------
// Modbus HRESULTs
// --------------------------------------------------------------------------

#define HR_MODBUS(code)    MAKE_HRESULT( SEVERITY_ERROR, FACILITY_MODBUS, code )
#define FACILITY_MODBUS    202
#define E_BASE             0xC700
enum COMM_ERROR_CODE  	// Modbus HRESULT Codes
{
	// Modbus Server Error Codes
	E_MODBUS_BASE          = HR_MODBUS(E_BASE),
	E_MODBUS_FUNCTION      = HR_MODBUS(E_BASE+1),
	E_MODBUS_DATAADDRESS   = HR_MODBUS(E_BASE+2),
	E_MODBUS_DATAVALUE     = HR_MODBUS(E_BASE+3),
	E_MODBUS_FAILURE       = HR_MODBUS(E_BASE+4),
	E_MODBUS_ACKNOWLEDGE   = HR_MODBUS(E_BASE+5),
	E_MODBUS_BUSYREJECTED  = HR_MODBUS(E_BASE+6),
	E_MODBUS_NEGACKNOWLEDGE= HR_MODBUS(E_BASE+7),
	E_MODBUS_MEMORYPARITY  = HR_MODBUS(E_BASE+8),
	E_MODBUS_CRC           = HR_MODBUS(E_BASE+9),
	E_MODBUS_TIMEOUT       = HR_MODBUS(E_BASE+10),
	E_MODBUS_UNK_EXCEPTION = HR_MODBUS(E_BASE+11),

	// Modbus Client Error Codes
	E_CLIENT_UNKNOWN       = HR_MODBUS(E_BASE + 21),
	E_CLIENT_MESSAGE       = HR_MODBUS(E_BASE + 22),
	E_CLIENT_CRC           = HR_MODBUS(E_BASE + 23),
	E_CLIENT_TIMEOUT       = HR_MODBUS(E_BASE + 24),
	E_CLIENT_DATAVALUE     = HR_MODBUS(E_BASE + 25),
	E_CLIENT_BADALIAS      = HR_MODBUS(E_BASE + 26),
	E_CLIENT_BUFFER        = HR_MODBUS(E_BASE + 27),
	E_CLIENT_WRITE         = HR_MODBUS(E_BASE + 28),

	// Modbus Device Error Codes
	E_DEVICE_ALIAS_MISSING    = HR_MODBUS(E_BASE + 30),
	E_DEVICE_PRODUCT_NUMBER   = HR_MODBUS(E_BASE + 31),
	E_DEVICE_PRODUCT_REVISION = HR_MODBUS(E_BASE + 32),
	E_DEVICE_PRODUCT_APPTYPE  = HR_MODBUS(E_BASE + 33),
	E_DEVICE_CFG_MISMATCH     = HR_MODBUS(E_BASE + 34),
	E_DEVICE_XML_MISMATCH     = HR_MODBUS(E_BASE + 35),

	// Modbus Tcp  Error Codes
	E_EMT_TRANSACTION_ERROR   = HR_MODBUS(E_BASE + 41),
	E_EMT_PROTOCAL_ERROR      = HR_MODBUS(E_BASE + 42),
	E_EMT_LENGTH_ERROR        = HR_MODBUS(E_BASE + 43)
};


// --------------------------------------------------------------------------
// Function prototypes

WORD AliasOffset( int AliasNum );
BYTE ReadFunctionCode( int AliasNum );
BYTE WriteFunctionCode( int AliasNum, bool bMultiple );
WORD RtuResponseLength( BYTE Code, BYTE DataBytes, bool bModbusTcp );

// --------------------------------------------------------------------------

} // namespace modbus


#endif // !defined(Modbus_H__INCLUDED)