// ------------------------------------------------------------------------
// File         : SerialDriver.cpp
// Author       : Ryan Van Slooten
// Description  : Implementation of CSerialDriver class.
// ------------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// ------------------------------------------------------------------------
// $Header: /Cfg320/SerialDriver.cpp 11    4/16/03 3:15p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -------------------------------------------
// 2011-02-22  WangDong          BCB5 to Visual Studio 2010
// 2001-10-04  Ryan Van Slooten  Added overlapped i/o and Win32 API functions to class.
// 2001-06-11  Ryan Van Slooten  Moved two variables into class definition
//                               for multithreading purposes.
// 2001-05-08  Ryan Van Slooten  Added Comment Block
// -------------------------------------------------------------------------

#include "StdAfx.h"
#include "SerialDriver.h"

// Added by wangdong
#include "Configurator.h"

//======================================================================================
/// Globals
#if !defined(ERRORAPI)
# define ERRORAPI(e, x)  (e = ((BOOL)((x) != 0) ? (ERROR_SUCCESS) : (::GetLastError())))
#endif

#if !defined(SAFE_DELETE)
# define SAFE_DELETE(x)        { if (x) { delete   (x); (x) = NULL; } }
#endif

#if !defined(SAFE_DELETE_ARRAY)
# define SAFE_DELETE_ARRAY(x)  { if (x) { delete[] (x); (x) = NULL; } }
#endif

#if !defined(SAFE_CLOSE_HANDLE)
# define SAFE_CLOSE_HANDLE(p)                             \
  {                                                       \
    if ((p != NULL) && (p != INVALID_HANDLE_VALUE))       \
    {                                                     \
      CloseHandle(p);                                     \
      (p) = INVALID_HANDLE_VALUE;                         \
    }                                                     \
  }
#endif
//=======================================================================================

CSerialDriver::CSerialDriver(void)
	          :m_hPort( INVALID_HANDLE_VALUE ),
			   m_strPort( L"" ),
			   m_dwBaud( 0 ),
			   m_byDataBits( 0 ),
			   m_dwParity( 0 ),
			   m_byStopBits( 0 ),
			   m_byRTSControl( 0 ),
			   m_bHandshake( false ),
			   m_nErrorCode( ERROR_SUCCESS ),
			   m_dwReadCount( 0 ),
			   m_dwWriteCount( 0 )
{
	memset( &m_dcb, 0, sizeof(DCB) );
	m_dcb.DCBlength = sizeof(DCB);
}


CSerialDriver::~CSerialDriver(void)
{
}

// ===============================================================
// Function name  : open
// Description    : Open a Serial Port                  
// Return type    : error number
//                  ERROR_SUCCESS: open successful
//                  ::GetLastError(): open failed
// Parameters     : void
// Author         : WangDong 2011.02.22 14:20:00
// ===============================================================
int CSerialDriver::open( void )
{
	m_hPort = CreateFile( m_strPort,                             // Port Name 
		                  GENERIC_READ | GENERIC_WRITE,          // Read and Write
						  0,                                     // Share Mode: must 0 for Serial
						  NULL,                                  // Security: should NULL for Serial
						  OPEN_EXISTING,                         // Open an Existing Port
						  0,                                     // Synchronous
						  NULL );

	if( m_hPort != INVALID_HANDLE_VALUE )     // Open successful
	{  
		m_nErrorCode = internal_config();
	}
	else                                      // Open Failed
	{
		m_nErrorCode = ::GetLastError();
	}

	return m_nErrorCode;
}

// ===============================================================
// Function name  : close
// Description    : 
//                  1. Purge the opened serial port
//                  2. Close handle 
// Return         : ERROR_SUCCESS
// Parameters     : 
// Author         : WangDong 2011.02.22 14:40:00
// ===============================================================
int CSerialDriver::close(void)
{
   if( m_hPort != INVALID_HANDLE_VALUE )     // Opened
   {
	   ERRORAPI( m_nErrorCode, ::PurgeComm( m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ));
		                     
       SAFE_CLOSE_HANDLE( m_hPort );     
   } 

   return ERROR_SUCCESS;
}


// ===============================================================
// Function name  : purge
// Description    : Purge the opened serial port  
//                  Clear (any) existing comm error
// Return         : ERROR_SUCCESS
// Parameters     : void
// Author         : WangDong 2011.02.22 14:50:00
// ===============================================================
int CSerialDriver::purge(void)
{
	if( m_hPort != INVALID_HANDLE_VALUE )     // Opened
    {
		 // Purge the serial port
		ERRORAPI( m_nErrorCode, ::PurgeComm( m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ));

		// Clear (any) existing comm error
        ERRORAPI( m_nErrorCode, ::ClearCommError(m_hPort, NULL, NULL));
	}

	return ERROR_SUCCESS;
}

// ===============================================================
// Function name  : read
// Description    :   
// Return         : read byte count
// Parameters     : pData: receieve data buffer
//                  byteCount: bytes count to be readed
// Author         : WangDong 2011.02.22 15:00:00
// ===============================================================
int CSerialDriver::read( BYTE* pData, int byteCount )
{
    if( m_hPort != INVALID_HANDLE_VALUE )
	{
        ERRORAPI( m_nErrorCode, ::ReadFile( m_hPort, pData, byteCount, &m_dwReadCount, NULL )); 

		return m_dwReadCount;
	}

	return 0;
}

// ===============================================================
// Function name  : write
// Description    :   
// Return         : read byte count
// Parameters     : pData: write data buffer
//                  byteCount: bytes count to be writted
// Author         : WangDong 2011.02.22 15:10:00
// ===============================================================
int CSerialDriver::write( const BYTE* pData, int byteCount )
{
	if( m_hPort != INVALID_HANDLE_VALUE )
	{
        ERRORAPI( m_nErrorCode, ::WriteFile( m_hPort, pData, byteCount, &m_dwWriteCount, NULL )); 

		return m_dwWriteCount;
	}

	return 0;
}

// ===============================================================
// Function name  : config
// Description    :   
// Return         : 
// Parameters     : 
//                  
// Author         : WangDong 2011.03.04 10:30:00
// ===============================================================
int CSerialDriver::config( LPCTSTR strPort, DWORD dwBaud, BYTE byDataBits, DWORD dwParity, 
	                       BYTE byStopBits, BYTE byRTSControl, bool bHandshake )
{
	bool bOpen = is_open();

	// Compare the port name
	if( m_strPort.CompareNoCase( strPort ) != 0 )
	{
        // The port names are different, so close the current port
        close();
		m_strPort = strPort;
	}

	m_dwBaud = dwBaud;
	m_byDataBits = byDataBits;
	m_dwParity = dwParity;
	m_byStopBits = byStopBits;
	m_byRTSControl = byRTSControl;
	m_bHandshake = bHandshake;

	if( bOpen )
	{
		return internal_config();
	}
	else
	{
		return open();
	}
}

// ===============================================================
// Function name  : internal_config
// Description    : The serial port should be opened 
// Return         : 
// Parameters     : 
//                  
// Author         : WangDong 2011.03.04 11:50:00
// ===============================================================
int CSerialDriver::internal_config()
{
	//1. Check if the serial port has been opened
	if( !is_open())
	{
		m_nErrorCode = ERROR_INVALID_HANDLE;
        return m_nErrorCode;
	}

	//2. Get CommState
	if( !GetCommState( m_hPort, &m_dcb ))
	{
		m_nErrorCode = ::GetLastError();
		return m_nErrorCode;
	}

	//3. Set serial settings, and set CommState
	m_dcb.BaudRate = m_dwBaud;
	m_dcb.ByteSize = m_byDataBits;
	m_dcb.Parity = static_cast<BYTE>(m_dwParity);
	m_dcb.StopBits = m_byStopBits;

	if( theApp.GetProductType() == CConfiguratorApp::DSC100 )
	{
	    m_dcb.fRtsControl = static_cast<DWORD>(m_byRTSControl);
		if( m_dcb.fRtsControl == RTS_CONTROL_HANDSHAKE )
		{
            m_dcb.fOutxCtsFlow = 1;
            m_dcb.fOutxDsrFlow = 0;
	        m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
            m_dcb.fDsrSensitivity = FALSE;
		}
		else
		{
           m_dcb.fOutxCtsFlow = 0;
           m_dcb.fOutxDsrFlow = 0;
           m_dcb.fDsrSensitivity = FALSE;
           m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
		}
	}
	else
	{
	    if( m_bHandshake )
	    {
            m_dcb.fOutxCtsFlow = 1;
			m_dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

            m_dcb.fOutxDsrFlow = 0;
            m_dcb.fDsrSensitivity = FALSE;
			m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
	    }
	    else
	    {
		    m_dcb.fOutxCtsFlow = 0;
			m_dcb.fRtsControl = RTS_CONTROL_DISABLE;

            m_dcb.fOutxDsrFlow = 0;
            m_dcb.fDsrSensitivity = FALSE;
            m_dcb.fDtrControl = DTR_CONTROL_DISABLE;   
	    }
	}

	// Others
	m_dcb.fBinary  = 1;                            // Binary
	m_dcb.fParity = ( m_dcb.Parity != NOPARITY );  // Enable Parity: when set NOPARITY, disable it
	m_dcb.fOutX    = 0;                            // Disable output XON/XFF 
    m_dcb.fInX     = 0;                            // Disable input XON/XFF
	m_dcb.fNull    = 0;                            // Discard NULL Charactor
    m_dcb.fAbortOnError = 0;                       // Not teminate read/write operation when error occurs

	if( !SetCommState( m_hPort, &m_dcb ))
	{
		m_nErrorCode = ::GetLastError();
		CString str = HrGetMessage( m_nErrorCode );
		AfxMessageBox( str );
		return m_nErrorCode;
	}

	//4. Set handshake signal
	// Direct the port to perform extended functions SETDTR and SETRTS.
    // SETDTR: Sends the DTR (data-terminal-ready) signal.
    // SETRTS: Sends the RTS (request-to-send) signal.
	//if( m_bHandshake )
	if( m_dcb.fRtsControl == RTS_CONTROL_HANDSHAKE )
	{
        EscapeCommFunction( m_hPort, CLRDTR );   // clear DTR signal
		EscapeCommFunction( m_hPort, CLRRTS );   // clear RTS signal
	}
  
	//5. Get Comm Timeouts, and adjust and set it
	COMMTIMEOUTS timeouts;
	if( !GetCommTimeouts( m_hPort, &timeouts ))
	{
		m_nErrorCode = ::GetLastError();
		return m_nErrorCode;
	}

	// Adjust Comm Timeouts
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 250;
	timeouts.WriteTotalTimeoutMultiplier = 100;

	// Set Comm Timeouts
    if( !SetCommTimeouts( m_hPort, &timeouts ))
	{
		m_nErrorCode = ::GetLastError();
		return m_nErrorCode;
	}

	//6. Get CommProperties
	COMMPROP commprop;
	if( !GetCommProperties( m_hPort, &commprop ))
	{
		m_nErrorCode = ::GetLastError();
		return m_nErrorCode;
	}

	return m_nErrorCode;
}

// ===============================================================
