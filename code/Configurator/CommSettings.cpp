// -----------------------------------------------------------------------
// $Workfile: CommSettings.cpp $
// Description  : Implementation of Communication Settings Dialog
// Author       : Ryan Van Slooten
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, La Marque, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
//
//                         REVISION HISTORY
//                         ----------------
//
//    Date          Who          Change
// ----------  ----------------  -------------------------------------------
// 2011-03-02  Wang Dong         Add Modbus Tcp settings in Visual Studio 2010
// 2003-02-24  Ryan Van Slooten  Changed PARITY constants to NOPARITY, etc.
//                               PARITY_NONE, PARITY_EVEN, etc are invalid.
// 1999-12-06  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "CommSettings.h"

// Added by wangdong 
#include "Configurator.h"       // theApp     

//=============================================================
// Default settings...
const bool  c_bModbusTcp = false;

const DWORD c_dwBaud    = CBR_38400;
const BYTE  c_byDataBits = 8;
const DWORD c_dwParity  = NOPARITY;
const BYTE  c_byStopBits = ONESTOPBIT;         
const BYTE  c_byRtsCtl = RTS_CONTROL_DISABLE;
const bool  c_bHandshake = false;
const BYTE  c_bySerSlaveID  = 1;

const BYTE  c_byIPAddress01 = 192;
const BYTE  c_byIPAddress02 = 168;
const BYTE  c_byIPAddress03 = 1;
const BYTE  c_byIPAddress04 = 1;
const DWORD c_dwIPPort = 502;
const BYTE  c_byTcpSlaveID = 1;

//==============================================================

CCommSettings::CCommSettings(void)
	          :m_bModbusTcp( false ),
			   m_strPort(),
			   m_dwBaud( 0 ),
			   m_byDataBits( 0 ),
			   m_dwParity( 0 ),
			   m_byStopBits( 0 ),
			   m_byRtsCtl( 0 ),
			   m_bHandshake( false ),
			   m_bySerSlaveID( 0 ),
			   m_byIPAddress01( 0 ),
			   m_byIPAddress02( 0 ),
			   m_byIPAddress03( 0 ),
			   m_byIPAddress04( 0 ),
			   m_dwIPPort( 0 ),
			   m_byTcpSlaveID( 0 )
{
}


CCommSettings::~CCommSettings(void)
{
}

// =======================================================================
// Function name: =
// Description  : Copy data members
// Return type  : Reference CCommSettings
// Parameters   : 
// Author       : WangDong 2011.03.02 10:40:00
// =======================================================================
CCommSettings& CCommSettings::operator =( const CCommSettings &commSrc )
{
    // Check for self-assignment
    if( &commSrc != this )
    {
		// Modbus Type
		m_bModbusTcp  = commSrc.m_bModbusTcp;

		// Serial Settings
        m_strPort      = commSrc.m_strPort;
        m_dwBaud       = commSrc.m_dwBaud;
        m_byDataBits   = commSrc.m_byDataBits;
        m_dwParity     = commSrc.m_dwParity;
        m_byStopBits   = commSrc.m_byStopBits;
		m_byRtsCtl     = commSrc.m_byRtsCtl;       
        m_bHandshake   = commSrc.m_bHandshake;  
        m_bySerSlaveID = commSrc.m_bySerSlaveID;

		// TCP Settings
		m_byIPAddress01 = commSrc.m_byIPAddress01;
		m_byIPAddress02 = commSrc.m_byIPAddress02;
		m_byIPAddress03 = commSrc.m_byIPAddress03;
		m_byIPAddress04 = commSrc.m_byIPAddress04;
		m_dwIPPort      = commSrc.m_dwIPPort;
		m_byTcpSlaveID  = commSrc.m_byTcpSlaveID;
    }

    return *this;
}

// ===========================================================
// Function name: SetDefault
// Description  : Set Serial/TCP default settings
//                according to the bModbusTcp flag
// Return type  : Reference CCommSettings
// Parameters   : bModbusTcp
// Author       : WangDong 2011.03.02 11:00:00
// ===========================================================
CCommSettings& CCommSettings::SetDefault( bool bModbusTcp )
{
    // Modbus Type
	m_bModbusTcp = bModbusTcp;

	if( !bModbusTcp )  // Serial Settings
	{
        //m_strPort      = c_strPort;         // Serial Port has no default value
        m_dwBaud       = c_dwBaud;
        m_byDataBits   = c_byDataBits;
        m_dwParity     = c_dwParity;
        m_byStopBits   = c_byStopBits;
	    m_byRtsCtl     = c_byRtsCtl;       
        m_bHandshake   = c_bHandshake;  
        m_bySerSlaveID = c_bySerSlaveID;
	}
	else               // TCP Settings
	{
	    m_byIPAddress01 = c_byIPAddress01;
	    m_byIPAddress02 = c_byIPAddress02;
	    m_byIPAddress03 = c_byIPAddress03;
	    m_byIPAddress04 = c_byIPAddress04;
	    m_dwIPPort     = c_dwIPPort;
	    m_byTcpSlaveID = c_byTcpSlaveID;
	}

    return *this;
}

// ===========================================================
// Function name: IsValid
// Description  : Check if serial/tcp settings is valid
//                according to the bModbusTcp flag
// Return type  : true, valid
// Parameters   : bModbusTcp, true: check tcp settings
//                            false: check serial settings
// Author       : WangDong 2011.03.02 11:40:00
// ===========================================================
bool CCommSettings::IsValid( bool bModbusTcp ) const
{
    if( !bModbusTcp )
	{
		// Port: L"COM"
		CString strCom(L"COM");
		CString strTest = m_strPort.Left( 3 );
		if( strTest.CompareNoCase( strCom ) != 0 )
		{
            return false;
		}

		// Baud
		if((m_dwBaud != CBR_1200)
        && (m_dwBaud != CBR_2400)
        && (m_dwBaud != CBR_4800)
        && (m_dwBaud != CBR_9600)
        && (m_dwBaud != CBR_19200)
        && (m_dwBaud != CBR_38400)
        && (m_dwBaud != CBR_56000)
        && (m_dwBaud != CBR_57600)
        && (m_dwBaud != CBR_115200)
        && (m_dwBaud != CBR_128000)
        && (m_dwBaud != CBR_256000))
		{
            return false;
        }

		// Data bits: 7, 8
		if((m_byDataBits != 7) &&
		   (m_byDataBits != 8)) 
		{
            return false;
		}

		// Parity: none, even, odd
		if((m_dwParity != NOPARITY)   &&
		   (m_dwParity != EVENPARITY) &&
		   (m_dwParity != ODDPARITY))
		{
			return false;
		}

		// Stop Bits: 1, 2
		if((m_byStopBits != ONESTOPBIT) &&
		   (m_byStopBits != TWOSTOPBITS))
		{
			return false;
		}

		// RTS: disable, enable, handshake, toggle
		if((m_byRtsCtl != RTS_CONTROL_DISABLE)  &&
			(m_byRtsCtl != RTS_CONTROL_ENABLE) &&
			(m_byRtsCtl != RTS_CONTROL_HANDSHAKE) &&
			(m_byRtsCtl != RTS_CONTROL_TOGGLE))
		{
			return false; 
		}

		// Serial Slave ID: 1 - 247
		if((m_bySerSlaveID == 0) || (m_bySerSlaveID > 247))
		{
			return false;
		}
	}//if( !bModbusTcp )
	else
	{
		// TCP/IP Slave ID
		if((m_byTcpSlaveID == 0) || (m_byTcpSlaveID > 247))
		{
			return false;
		}
	}

	return true;
}

// ===========================================================
// Function name: WriteRegistry
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.02 14:00:00
// ===========================================================
void CCommSettings::WriteRegistry()
{
	CString strSubKey = GetSubKeyString();
	ASSERT( !strSubKey.IsEmpty());

	strSubKey += L"\\Comm Settings";

	HKEY hKey;

	// Create subkey
	LONG lResult = RegCreateKey( HKEY_CURRENT_USER, strSubKey, &hKey );
	if( lResult != ERROR_SUCCESS )
	{
		return;
	}

	// Write Registry
	DWORD dwValue = 0;
	dwValue = (m_bModbusTcp ? 1 : 0);
	RegSetValueExW( hKey, L"ModbusTcp",      0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // Modbus Type

	CString strPath;
	strPath.Format( L"Settings\\%s\\Comm Settings", theApp.GetProductName());
	theApp.WriteProfileString( strPath, L"Serial Port", m_strPort );

	RegSetValueExW( hKey, L"Baud",           0, REG_DWORD, (const BYTE*)&m_dwBaud, 4 );           // Baud

	dwValue = static_cast<DWORD>(m_byDataBits);
	RegSetValueExW( hKey, L"Data Bits",      0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // Data Bits
	RegSetValueExW( hKey, L"Parity",         0, REG_DWORD, (const BYTE*)&m_dwParity, 4 );         // Parity

	dwValue = static_cast<DWORD>(m_byStopBits);
	RegSetValueExW( hKey, L"Stop Bits",      0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // Stop Bits

	dwValue = static_cast<DWORD>(m_byRtsCtl);
	RegSetValueExW( hKey, L"RTS Control",    0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // RTS Control

	dwValue = (m_bHandshake ? 1 : 0);
	RegSetValueExW( hKey, L"Handshaking",    0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // Handshaking

	dwValue = static_cast<DWORD>(m_bySerSlaveID);
	RegSetValueExW( hKey, L"Serial SlaveID", 0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // Serial SlaveID

	dwValue = static_cast<DWORD>(m_byIPAddress01);
	RegSetValueExW( hKey, L"IP Address01",   0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // IP Address01

	dwValue = static_cast<DWORD>(m_byIPAddress02);
	RegSetValueExW( hKey, L"IP Address02",   0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // IP Address02

	dwValue = static_cast<DWORD>(m_byIPAddress03);
	RegSetValueExW( hKey, L"IP Address03",   0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // IP Address03

	dwValue = static_cast<DWORD>(m_byIPAddress04);
	RegSetValueExW( hKey, L"IP Address04",   0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // IP Address04

	RegSetValueExW( hKey, L"TCP Port",       0, REG_DWORD, (const BYTE*)&m_dwIPPort, 4 );         // TCP Port

    dwValue = static_cast<DWORD>(m_byTcpSlaveID);
	RegSetValueExW( hKey, L"TCP SlaveID",    0, REG_DWORD, (const BYTE*)&dwValue, 4 );            // TCP SlaveID

	// Close Registry
    RegCloseKey( hKey );
}

// ===============================================================
// Function name: ReadRegistry
// Description  : Open, Read the Registry, and set members
//                If member is invalid, set with its default value
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.02 15:50:00
// ===============================================================
void CCommSettings::ReadRegistry()
{
	CString strSubKey = GetSubKeyString();
	ASSERT( !strSubKey.IsEmpty());

	strSubKey += L"\\Comm Settings";

	LONG lResult = ERROR_SUCCESS;
	HKEY hKey;

	// Open Registry
	lResult = RegOpenKey( HKEY_CURRENT_USER, strSubKey, &hKey );   
	if( lResult != ERROR_SUCCESS )
	{
        return;
	}

	DWORD dwLength = sizeof(DWORD) / sizeof(BYTE);

	// Read Registry
	/// Modbus Type
	DWORD dwValue = 0;
	lResult = RegQueryValueExW( hKey, L"ModbusTcp", NULL, NULL, (LPBYTE)&dwValue, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
	    m_bModbusTcp = dwValue ? true : false;
	}

	///1. Serial Settings
	//1.1 Serial Port
	CString strPath;
	strPath.Format( L"Settings\\%s\\Comm Settings", theApp.GetProductName());
	m_strPort = theApp.GetProfileStringW( strPath, L"Serial Port", L"COM1" );

	//1.2 Baud
	DWORD dwBaud = 0;
	lResult = RegQueryValueEx( hKey, L"Baud", NULL, NULL, (LPBYTE)&dwBaud, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_dwBaud = dwBaud;
	}

	// If invalid, set default
	if((m_dwBaud != CBR_1200)   &&
       (m_dwBaud != CBR_2400)   &&
       (m_dwBaud != CBR_4800)   &&
       (m_dwBaud != CBR_9600)   &&
       (m_dwBaud != CBR_19200)  &&
       (m_dwBaud != CBR_38400)  &&
       (m_dwBaud != CBR_56000)  &&
       (m_dwBaud != CBR_57600)  &&
       (m_dwBaud != CBR_115200) &&
       (m_dwBaud != CBR_128000) &&
       (m_dwBaud != CBR_256000))
	{
        m_dwBaud = c_dwBaud;
	}

	//1.3 Data Bits
	DWORD dwDataBits = 0;
	lResult = RegQueryValueEx( hKey, L"Data Bits", NULL, NULL, (LPBYTE)&dwDataBits, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_byDataBits = static_cast<BYTE>(dwDataBits);
	}

	// If invalid, set default
	if((m_byDataBits != 7) &&
	   (m_byDataBits != 8)) 
	{
        m_byDataBits = c_byDataBits;    
	}

	//1.4 Parity
	DWORD dwParity = 0;
	lResult = RegQueryValueEx( hKey, L"Parity", NULL, NULL, (LPBYTE)&dwParity, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_dwParity = dwParity;
	}

	// If invalid, set default
	if((m_dwParity != NOPARITY)   &&
	   (m_dwParity != EVENPARITY) &&
	   (m_dwParity != ODDPARITY))
	{
        m_dwParity = c_dwParity;    
	}

	//1.5 Stop Bits
	DWORD dwStopBits = 0;
	lResult = RegQueryValueEx( hKey, L"Stop Bits", NULL, NULL, (LPBYTE)&dwStopBits, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_byStopBits = static_cast<BYTE>(dwStopBits);
	}

	// If invalid, set default
	if((m_byStopBits != ONESTOPBIT) &&
	   (m_byStopBits != TWOSTOPBITS))
	{
        m_byStopBits = c_byStopBits;    
	}

	//1.6 RTS Control
	DWORD dwRtsCtl = 0;
	lResult = RegQueryValueEx( hKey, L"RTS Control", NULL, NULL, (LPBYTE)&dwRtsCtl, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_byRtsCtl = static_cast<BYTE>(dwRtsCtl);
	}

	// If invalid, set default
	if((m_byRtsCtl != RTS_CONTROL_DISABLE)  &&
	   (m_byRtsCtl != RTS_CONTROL_ENABLE) &&
	   (m_byRtsCtl != RTS_CONTROL_HANDSHAKE) &&
	   (m_byRtsCtl != RTS_CONTROL_TOGGLE))
	{
        m_byRtsCtl = c_byRtsCtl;    
	}

	// 1.7 Handshaking
	DWORD dwHandshaking = 0;
	lResult = RegQueryValueEx( hKey, L"Handshaking", NULL, NULL, (LPBYTE)&dwHandshaking, &dwLength );
	if( lResult == ERROR_SUCCESS )
	{
		m_bHandshake = dwHandshaking ? true : false;
	}

	// 1.8 Serial SlaveID
	DWORD dwSerSlaveID = 0;
	lResult = RegQueryValueEx( hKey, L"Serial SlaveID", NULL, NULL, (LPBYTE)&dwSerSlaveID, &dwLength );     
	if( lResult == ERROR_SUCCESS )
	{
	    m_bySerSlaveID = static_cast<BYTE>(dwSerSlaveID);
	}

	// If invalid, set default
	if((m_bySerSlaveID == 0) || (m_bySerSlaveID > 247))
	{
	    m_bySerSlaveID = c_bySerSlaveID;
	}

	/// TCP Settings
	// IP Address 01 - 04
	DWORD dwIPAddress01 = 0;
	lResult = RegQueryValueEx( hKey, L"IP Address01", NULL, NULL, (LPBYTE)&dwIPAddress01, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_byIPAddress01 = static_cast<BYTE>(dwIPAddress01);
	}
	
	DWORD dwIPAddress02 = 0;
	lResult = RegQueryValueEx( hKey, L"IP Address02", NULL, NULL, (LPBYTE)&dwIPAddress02, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_byIPAddress02 = static_cast<BYTE>(dwIPAddress02);
	}

	DWORD dwIPAddress03 = 0;
	lResult = RegQueryValueEx( hKey, L"IP Address03", NULL, NULL, (LPBYTE)&dwIPAddress03, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_byIPAddress03 = static_cast<BYTE>(dwIPAddress03);
	}

	DWORD dwIPAddress04 = 0;
	lResult = RegQueryValueEx( hKey, L"IP Address04", NULL, NULL, (LPBYTE)&dwIPAddress04, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_byIPAddress04 = static_cast<BYTE>(dwIPAddress04);
	}
	
	// TCP/IP Port
	DWORD dwIPPort = 0;
	lResult = RegQueryValueEx( hKey, L"TCP Port", NULL, NULL, (LPBYTE)&dwIPPort, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_dwIPPort = dwIPPort;
	}
	
	// TCP SlaveID
	DWORD dwTcpSlaveID = 0;
	lResult = RegQueryValueEx( hKey, L"TCP SlaveID", NULL, NULL, (LPBYTE)&dwTcpSlaveID, &dwLength );   
	if( lResult == ERROR_SUCCESS )
	{
		m_byTcpSlaveID = static_cast<BYTE>(dwTcpSlaveID);
	}
	
	// If invalid, set default
	if((m_byTcpSlaveID == 0) || (m_byTcpSlaveID > 247))
	{
		m_byTcpSlaveID = c_byTcpSlaveID;
	}

	// Close Key
	RegCloseKey( hKey ); 
}

// ===============================================================
// Function name: Exists
// Description  : Check if the serial port was exist
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.02 15:50:00
// ===============================================================
bool CCommSettings::Exists( int nCommNum )
{
    bool bExists = false;
	CString strPort;
	strPort.Format( L"COM%d", nCommNum );
	
	HANDLE hFile = ::CreateFileW( strPort,                            // COMM Port 
		                         FILE_SHARE_READ | FILE_SHARE_WRITE,  // Desiared access: read and write
								 0,                                   // Share Mode: must 0 for Serial      
								 NULL,                                // Security: should NULL for Serial
								 OPEN_EXISTING,                       // Open an Existing Port
								 0,                                   // Synchronous
								 NULL );

	if( hFile != INVALID_HANDLE_VALUE )  // Open success
	{
        bExists = true;
	}
	else
	{
		// Check if another process has exclusive access (which is the only
        // kind granted to the com port) preventing us from opening the com.
		DWORD e = ::GetLastError();
		if( e == ERROR_ACCESS_DENIED )
		{
			bExists = true;
		}
	}

	::CloseHandle( hFile );
	return bExists;
}