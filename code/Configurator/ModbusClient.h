// -----------------------------------------------------------------------
// File         : ModbusClient.hpp
// Author/Date  : rjvs 2002-01-21 20:51:57 (UTC-6:00)
// Description  : Modbus client builds, decodes, and validates incoming
//              : and outgoing messages.  It also updates the aliases
//              : with the received values from the message.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-02-23  Wang Dong         BCB5 to Visual Studio 2010   2010.02.23
// 2002-06-04  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#pragma once

// --------------------------------------------------------------------------
// Forward declare classes
struct IComDriver;

// --------------------------------------------------------------------------
// -- Modbus --

namespace modbus
{

// --------------------------------------------------------------------------
// Forward declare classes
class CMessage;

// --------------------------------------------------------------------------
// { modbus::Client }

class CClient
{
public:
    CClient();
    virtual ~CClient();

private:
    int m_nUnitId;                    // Modbus identifier
    int m_nMessageBytes;              // Expected message size

public:
    int GetUnitId() const { return m_nUnitId; }
    void SetUnitId(int Value)
    {
        if((Value > 0) && (Value < 255))
		{
            m_nUnitId = Value;
		}
    }

    HRESULT WriteMessage( IComDriver* pDriver, CMessage* pMsg );
    HRESULT ReadMessage( IComDriver* pDriver, CMessage* pMsg,  bool bModbusTcp );

protected:
    HRESULT ValidateMessage( CMessage* pMsg, bool bModbusTcp );
    HRESULT DecodeMessage( CMessage* pMsg, bool bModbusTcp );
};

// --------------------------------------------------------------------------

} // namespace modbus

// --------------------------------------------------------------------------

