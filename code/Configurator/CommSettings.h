// -----------------------------------------------------------------------
// File         : CommSettings.h
// Description  : Declaration of Cfg3410 Communications Settings
// Author       : Ryan Van Slooten
// Purpose      :
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 1999, Triconex Corporation, La Marque, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
//
//                         REVISION HISTORY
//                         ----------------
//
//   Date           Who          Change
// ----------  ----------------  -----------------------------------------
// 2011/03/02  Wang Dong         Add modbus tcp settings in visual studio 2010
// 2001/03/23  Ryan Van Slooten  Converted to STL and Borland C++ Builder
// 1999/12/06  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#pragma once
class CCommSettings
{
public:
	CCommSettings(void);
	~CCommSettings(void);

public:
	bool m_bModbusTcp;

	CString m_strPort;
	DWORD m_dwBaud;
	BYTE  m_byDataBits;
	DWORD m_dwParity;
	BYTE  m_byStopBits;
	BYTE  m_byRtsCtl;
	bool  m_bHandshake;
	BYTE  m_bySerSlaveID;

	BYTE  m_byIPAddress01;
	BYTE  m_byIPAddress02;
	BYTE  m_byIPAddress03;
	BYTE  m_byIPAddress04;
	DWORD m_dwIPPort;
	BYTE  m_byTcpSlaveID;

public:
	CCommSettings& operator =(const CCommSettings &commSrc);
    CCommSettings& SetDefault( bool bModbusTcp );
	bool           IsValid( bool bModbusTcp ) const;
	bool           IsSerPortEmpty()           const { return m_strPort.IsEmpty(); }
	//bool           IsDefault() const;
    
	void           WriteRegistry();
	void           ReadRegistry();
    
    static bool    Exists( int nCommNum );
};

