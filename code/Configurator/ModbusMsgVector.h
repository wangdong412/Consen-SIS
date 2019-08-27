// -----------------------------------------------------------------------
// $WorkFile: $
// Author/Date  : rjvs 2002-08-09 16:30:39 (UTC-6:00)
// Description  : Modbus array that builds and hold the messages.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/ModbusVector.hpp 8     2/18/03 2:36p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-02-25  Wang Dong         CMessageVector -> CModbusMsgVector
// 2002-08-09  Ryan Van Slooten  Added Comment Block.  Moved code here.
// -----------------------------------------------------------------------
#pragma once

class CAliasMap;

//---------------------------------------------------------------------------

namespace modbus
{

class CMessage;

// --------------------------------------------------------------------------
// { CMessageVector }

typedef std::vector< CMessage* > __stlMessageVector;


class CModbusMsgVector : public __stlMessageVector
{
  typedef __stlMessageVector inherited;

private:
  int m_nUnitId;
  int m_nMessageBytes;              // expected size of message
  CAliasMap* m_pAliasMap;

public:
  CModbusMsgVector();
  virtual ~CModbusMsgVector();

  void clear();

  void RemoveNonRepeatMessages();
  void SetAliasMap( CAliasMap* pAliasMap );
  CAliasMap* GetAliasMap()   const { return m_pAliasMap; }

  HRESULT BuildMessages( int BeginAlias, int EndAlias,
                         int AliasesPerMessage, int AliasGapSize,
                         bool bModbusTcp = false,
                         bool bWrite = false, bool bCopyValue = false );

  HRESULT BuildMessages( CAliasMap* pAliasMap, int BeginAlias, int EndAlias,
                         int AliasesPerMessage, int AliasGapSize,
                         bool bModbusTcp = false,
                         bool bWrite = false, bool bCopyValue = false );

  HRESULT CreateMessage( CMessage* msg );
  HRESULT CreateSingleWriteMessage( CMessage* msg );
  HRESULT CreateReadMessage( CMessage* msg );
  HRESULT CreateWriteDigitalMessage( CMessage* msg );
  HRESULT CreateWriteAnalogMessage( CMessage* msg );

  HRESULT BuildLoopbackMessage( bool bModbusTcp = false );
  
  int GetMessageCount()                   const { return size(); }
  int GetTotalByteCount()                 const;
  size_t UpdateActive( bool Value );

  CMessage* GetMessage( int index );
  const CMessage* GetMessage( int index ) const;

  int GetUnitId() const { return m_nUnitId; }
  void SetUnitId(int Value)
  {
      if((Value > 0) && (Value < 255))
	  {
          m_nUnitId = Value;
	  }
  }
};


} // namespace modbus

//---------------------------------------------------------------------------
