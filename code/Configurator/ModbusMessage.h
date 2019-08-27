// -----------------------------------------------------------------------
// File         : ModbusMessage.cpp
// Author/Date  : rjvs 2002-01-28 10:20:22 (UTC-6:00)
// Description  : Modbus message holds a buffer to the input and output
//              : data, and an array of the aliases referenced by the
//              : message.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/ModbusMessage.hpp 18    2/18/03 2:37p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-02-24  Wang Dong         BCB5 to Visual Studio 2010
// 2002-06-04  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include "AliasItem.h"

namespace modbus
{

// --------------------------------------------------------------------------
// { modbus::CMessage }
class CMessage
{
public:
  CMessage();
  virtual ~CMessage();

private:
  int m_nBegin;
  int m_nEnd;
  bool m_bWrite;
  bool m_bCopyValue;
  bool m_bModbusTcp;
  HRESULT m_hr;
  DWORD m_dwRefresh;    // Time (msec) before refresh - 0 ms means to refresh as fast as possible
  DWORD m_dwBegin;      // Time (msec) at which message began
  DWORD m_dwElapsed;    // Time (msec) for completed message
  int m_nInputCount;    // Bytes received
  int m_nOutputCount;   // Bytes sent
  int m_nEstimateCount; // Bytes expected
  int m_nRepeatCount;   // Number of times to repeat (-1 = Infinite)
  int m_nSuccessCount;
  int m_nFailureCount;
  ByteVector m_vInputBuffer;
  ByteVector m_vOutputBuffer;
  CAliasMap m_vAliases;

protected:
  void         Add( CAliasItem* pAlias );
  BYTE*        InputBegin();             // Beginning of input buffer
  BYTE*        OutputBegin();            // Beginning of output buffer
  const BYTE*  InputBegin() const;       // Beginning of input buffer
  const BYTE*  OutputBegin() const;      // Beginning of output buffer
  void         InputResize( int n );
  void         OutputResize( int n );

  void         BeginTick( DWORD Value )          { m_dwBegin = Value; }
  void         EndTick( DWORD Value )            { m_dwElapsed = Value - m_dwBegin; }

  void         ClearInput();
  size_t       UpdateActive( bool Value );

  bool         GetCopyValue() const               { return m_bCopyValue; }
  void         SetCopyValue( bool Value )         { m_bCopyValue = Value; }

  CAliasItem* Item(int Index);
  const CAliasItem* Item(int Index)             const;

  CAliasItem* FindAlias(int ANumber);
  const CAliasItem* FindAlias(int ANumber)      const;

  CAliasItem* FindName(LPCTSTR AName);
  const CAliasItem* FindName(LPCTSTR AName)     const;

public:
  bool      IsRead()                     const { return !m_bWrite; }
  bool      IsWrite()                    const { return m_bWrite; }

  int       AliasCount()                 const;
  int       ItemCount()                  const;
  int       InputSize()                  const;        // Size of input buffer
  int       OutputSize()                 const;        // Size of output buffer
  int       InputCount()                 const { return m_nInputCount; }
  int       OutputCount()                const { return m_nOutputCount; }
  int       EstimateCount()              const { return m_nEstimateCount; }
  void      EstimateCount(int Value)     { m_nEstimateCount = Value; }
  int       BeginAlias()                 const { return m_nBegin; }
  int       EndAlias()                   const { return m_nEnd; }

  int       RepeatCount()                const { return m_nRepeatCount; }
  void      RepeatCount(int Value)       { m_nRepeatCount = Value; }

  int       SuccessCount()               const { return m_nSuccessCount; }
  void      SuccessCount(int Value)      { m_nSuccessCount = Value; }

  int       FailureCount()               const { return m_nFailureCount; }
  void      FailureCount(int Value)      { m_nFailureCount = Value; }

  DWORD     BeginTick()                  const { return m_dwBegin; }
  DWORD     ElapsedTick()                const { return m_dwElapsed; }
  DWORD     EndTick()                    const { return m_dwBegin + m_dwElapsed; }

  int       DecrementRepeatCount()
  {
      if((m_nRepeatCount == 0) || (m_nRepeatCount == -1))
	  {
          return m_nRepeatCount;
	  }
	  else
	  {
          return --m_nRepeatCount;
	  }
  }

  friend class CClient;
  friend class CModbusThread;
  friend class CModbusMsgVector;
};

// --------------------------------------------------------------------------

} // namespace modbus

// --------------------------------------------------------------------------

