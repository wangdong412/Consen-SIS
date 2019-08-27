#include "StdAfx.h"
#include "ModbusThread.h"

// Added by wangdong
#include "Configurator.h"
#include "SerialDriver.h"
#include "SocketDriver.h"
#include "ModbusClient.h"
#include "ModbusMessage.h"
#include "ModbusMsgVector.h"
#include "ModbusStatus.h"
#include "CommSettings.h"
#include "AliasItem.h"
#include "ModbusRange.h"
#include "Modbus.h"

extern std::auto_ptr<CWinThread> CommThread;
extern std::auto_ptr<CEvent> g_pThreadEvent;
extern bool g_bSuspend;

namespace modbus
{

const int           RETRY_COUNT         = 3;
const DWORD         TIMEOUT             = 750;
const int           TS310SV_REVISION    = 966;

const int           LOGGING_MAX_LINES   = 100;
const int           LOGGING_MAX_SIZE    = 0x100000; // Approx. 1 MB

const float         SCAN_FILTER         = 0.75;

//---------------------------------------------------------------------------
// { Thread }

/// <summary>
///   Constructor for Thread</summary>
/// <param name="CreateSuspended">
///   Specifies whether the thread is suspended on creation.</param>
/// <created>
///   rjvs 2002-01-22 15:16:16 (UTC-6:00)</created>
/// <seealso cref="" />
CModbusThread::CModbusThread(void)
	        :m_pEvent            ( new CEvent( FALSE, TRUE, NULL, NULL )),
			 m_pSerial           ( new CSerialDriver()),
			 m_pSocket           ( new CSocketDriver()),
			 m_pClient           ( new CClient()),
			 m_pMessages         ( new CModbusMsgVector()),
			 m_pStatus           ( new CModbusStatus()),
			 m_pDriver           ( NULL ),
			 m_dwTimeOut         ( TIMEOUT ),
             m_dwTimeStamp       ( 0 ),
             m_dwRepeatTime      ( 0 ),
             m_dwRepeatStamp     ( 0 ),
             m_dwScanTime        ( 0 ),
			 m_dwWriteMessages   ( 0 ),                 // Write(to client)  successful msg's count
             m_dwReadMessages    ( 0 ),                 // Read(from client) successful msg's count
             m_dwErrorMessages   ( 0 ),                 // Error msg count
             m_dwRetryMessages   ( 0 ),
             m_dwTimeoutMessages ( 0 ),
             m_nRepeatCount      ( 0 ),                 
             m_nIndex            ( 0 ),                 // msg index
             m_nState            ( NONE ),
             m_nAction           ( mbsNONE ),
             m_nLastAction       ( mbsNONE ),
			 m_bConnected        ( false ),
			 m_bModbusTcp        ( false ),
			 m_bTerminated       ( false )       
{
	// Check memory allocation
    ASSERT(CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)));
    ASSERT(CheckWritePtr( m_pStatus.get(), sizeof(modbus::CModbusStatus)));
    ASSERT(CheckWritePtr( m_pMessages.get(), sizeof(modbus::CModbusMsgVector)));
}


CModbusThread::~CModbusThread(void)
{
	m_pEvent.reset();
	m_pSerial.reset();
    m_pSocket.reset();
	m_pClient.reset();
	m_pMessages.reset();
	m_pStatus.reset();
}

// =============================================================
// Function name: SetCommSettings
// Description  : Set m_bModbusTcp, m_pSerial, m_pSocket, m_pDriver, and SlaveID
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.02 18:50:00
// =============================================================
void CModbusThread::SetCommSettings( const CCommSettings* pCommSettings )
{
	// Write clock
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	ASSERT( singleLock.IsLocked());

	ASSERT( CheckReadPtr(pCommSettings, sizeof(CCommSettings)));

	if( !CheckReadPtr(pCommSettings, sizeof(CCommSettings)))
	{
		return;
	}

	ASSERT( CheckWritePtr( m_pSerial.get(), sizeof(CSerialDriver)));
	ASSERT( CheckWritePtr( m_pSocket.get(), sizeof(CSocketDriver)));

	// Modbus Type
	m_bModbusTcp = pCommSettings->m_bModbusTcp;
	if( !pCommSettings->IsValid( m_bModbusTcp ))
	{
		return;
	}

	// m_pDriver
	if( m_bModbusTcp )
	{
		m_pDriver = m_pSocket.get();
	}
	else
	{
        m_pDriver = m_pSerial.get();
	}

	// Serial Settings
	if( !m_bModbusTcp && CheckWritePtr( m_pSerial.get(), sizeof(CSerialDriver)))
	{
		m_pSerial->config( pCommSettings->m_strPort,
			               pCommSettings->m_dwBaud, 
			               pCommSettings->m_byDataBits, 
						   pCommSettings->m_dwParity,
			               pCommSettings->m_byStopBits, 
						   pCommSettings->m_byRtsCtl, 
						   pCommSettings->m_bHandshake );
	}

	// TCP/IP Settings
	if( m_bModbusTcp && CheckWritePtr( m_pSocket.get(), sizeof(CSocketDriver)))
	{
		// IP Address ( const char*)
		CString strIPAddress;
		strIPAddress.Format( L"%d.%d.%d.%d", pCommSettings->m_byIPAddress01, pCommSettings->m_byIPAddress02,
			       pCommSettings->m_byIPAddress03, pCommSettings->m_byIPAddress04);

	    DWORD dwNum = WideCharToMultiByte( CP_OEMCP, NULL,(LPCWSTR)strIPAddress, -1, NULL, 0, NULL, FALSE );
		char *psIPAddress = new char[dwNum];
		WideCharToMultiByte( CP_OEMCP, NULL, (LPCWSTR)strIPAddress, -1, psIPAddress, dwNum, NULL, FALSE ); 
        const char* pIPAddress = psIPAddress;
		m_pSocket->name( pIPAddress );
		SAFE_DELETE_ARRAY( psIPAddress );

		// Port
	    m_pSocket->port( static_cast<USHORT>(pCommSettings->m_dwIPPort)); 
	}

	// Slave ID
	int nSlaveID = 0;
	if( m_bModbusTcp )
	{
		nSlaveID = pCommSettings->m_byTcpSlaveID;	  
	}
	else
	{
		nSlaveID = pCommSettings->m_bySerSlaveID;
	}

	m_pMessages->SetUnitId( nSlaveID );   // Build Message
	m_pClient->SetUnitId( nSlaveID );     // Read Message
	
	singleLock.Unlock();
}

// =============================================================
// Function name: ThreadExcute
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03
// =============================================================
void CModbusThread::ThreadExcute( void )
{
	HRESULT hr = S_OK;
	try
    {
        while( !m_bTerminated )
        {
            if( g_bSuspend )
            {
				g_pThreadEvent->SetEvent();
				m_nState = SUSPEND;
                m_dwRepeatTime = 0;
				CommThread->SuspendThread();				
            }
			if( m_bTerminated )
			{
				break;
			}

			// Write to Client: Request
            hr = WriteMessage();
            if( FAILED(hr))
			{
                if( hr != m_pStatus->GetError())
			    {
					m_pStatus->SetError( hr, HrGetMessage(hr));
			    }
			}
			if( m_bTerminated )
			{
				break;
			}

			// Read from Client: Response 
            hr = ReadMessage();
            if( FAILED(hr))
			{
                if (hr != m_pStatus->GetError())
				{
                    m_pStatus->SetError(hr, HrGetMessage(hr));
				}
			}
			if( m_bTerminated )
			{
				break;
			}

			// Update Flags
            hr = NextState();
            if( hr != S_OK )
			{
                if (hr != m_pStatus->GetError())
				{
                    m_pStatus->SetError(hr, HrGetMessage(hr));
				}
			}
			if( m_bTerminated )
			{
				break;
			}

            ::Sleep( 20 );
        } //while( !Terminated )
    } //try
    catch (...)
    {
	    theApp.LogInfo( L"CModbusThread::ThreadExcute: Unhandled thread exception!" );

		return;
    }
}

// rjvs 2002-08-26 14:33:16 (UTC-6:00)
template <typename var_t>
inline bool betweene(var_t Test, var_t MinTest, var_t MaxTest)
{
    return ((Test >= MinTest) && (Test <= MaxTest));
}

// =============================================================
// Function name: WriteMessage
// Description  : 1. Set m_pStatus if not Monitor;
//                2. Write the msg to the Modbus client.
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.07 18:24
// =============================================================
HRESULT CModbusThread::WriteMessage()
{
    HRESULT hr = S_OK;

    if( GetState() == REQUEST )
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		ASSERT( singleLock.IsLocked());

        if( GetState() != REQUEST )
		{
            return hr;
		}

        if( m_pMessages->GetMessageCount() <= m_nIndex )
        {
            hr = E_CLIENT_BUFFER;
            return hr;
        }

        CMessage* msg = m_pMessages->GetMessage( m_nIndex );
        if( msg == NULL )
        {
            hr = E_CLIENT_BUFFER;
            return hr;
        }

		// 1. Set m_pStatus message if not Monitor
        CString strStatus;
        if( msg->IsWrite())
        {
			strStatus.Format( IDS_WRITE_STATUS, 
				              m_nIndex + 1,                                   // Current msg index
							  m_pMessages->GetMessageCount(),                 // Total msgs count
							  msg->BeginAlias(),                              // Begin alias of the message
							  msg->EndAlias());                               // End alias of the message
        }
        else
        {
            strStatus.Format( IDS_READ_STATUS,         
                              m_nIndex + 1,
							  m_pMessages->GetMessageCount(),
							  msg->BeginAlias(),
							  msg->EndAlias());
        }

        if( m_dwRetryMessages > 0 )
        {
			strStatus.AppendFormat( IDS_RETRY_STATUS, m_dwRetryMessages );
        }

        if( m_nAction != mbsMONITOR )
		{
            m_pStatus->SetMessage(strStatus);
		}

        //2. Write the message to the Modbus client.
        hr = m_pClient->WriteMessage( m_pDriver, msg );
        if( hr == S_OK )  
        {
			// Log Message
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
			{
				CString s = CString( L"Master: ") + DumpBytes(msg->OutputBegin(), msg->OutputCount());
				theApp.LogInfo( s );
			}

			++m_dwWriteMessages;
            m_nState = RESPONSE;
			m_pStatus->IncrementCount( msg->OutputCount()); // Update status
        }
        else
        {
			CString s = CString("Master write Failed: ") + DumpBytes(msg->OutputBegin(), msg->OutputCount());
			theApp.LogInfo( s );
      
            ++m_dwErrorMessages;
            
            if((m_dwRepeatTime == 0) || (!m_pDriver->is_open()))
            {
                m_nState = NONE;
            }
            else   // Monitor
            {
                m_nState = NEXT;
            }
        }

        // Set the tick counter for the Modbus message
        msg->BeginTick( ::GetTickCount());

		singleLock.Unlock();
    }//if (GetState() == REQUEST)

    return hr;
}

// =============================================================
// Function name: ReadMessage
// Description  : 1. Read message from the Modbus client.
//                2. If m_nState is NONE, Update the m_pStatus
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.07 18:24
// =============================================================
HRESULT CModbusThread::ReadMessage()
{
    HRESULT hr = S_OK;

    if( GetState() == RESPONSE )
    {
        CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		ASSERT( singleLock.IsLocked());

        if( GetState() != RESPONSE )
	    {
            return hr;
	    }

		if( m_pMessages->GetMessageCount() <= m_nIndex )
        {
            hr = E_CLIENT_BUFFER;
            return hr;
        }

		// Read msg from the Modbus client
        CMessage* msg = m_pMessages->GetMessage( m_nIndex );
		ASSERT( CheckReadPtr( msg, sizeof(CMessage)));
        hr = m_pClient->ReadMessage( m_pDriver, msg, m_bModbusTcp );
        msg->EndTick( ::GetTickCount());
        if( hr == S_OK )     
        {
            // Record response data
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
			{
                CString s = CString("Slave: ") + DumpBytes( msg->InputBegin(), msg->InputCount());
				theApp.LogInfo( s );
			}

            // Check product version number
            if( mbsCONNECT == m_nAction )
            {
                if( betweene(msg->BeginAlias(), 31001, 31004) &&
                    betweene(msg->EndAlias(),   31001, 31004))
                {
					CString strAppError;
                    hr = CheckProductInfo( strAppError );
					if( hr != S_OK )
					{
						// TS350, Application Type Error
						if((CConfiguratorApp::TS350 == theApp.GetProductType()) && 
						   (E_DEVICE_PRODUCT_APPTYPE == hr ))  
						{
							//if( AfxMessageBox( strAppError, MB_YESNO | MB_ICONQUESTION ) == IDYES )                                               // fail
							//if( AfxMessageBox( strAppError, MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL ) == IDYES )                              // ok
							//if( ::MessageBoxW( NULL, strAppError, L"", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST ) == IDYES )                       // fail
							//if( ::MessageBoxW( theApp.m_pMainWnd->GetSafeHwnd(), strAppError, L"", MB_YESNO | MB_ICONQUESTION ) == IDYES )        // ok
							if( ::MessageBoxW( theApp.m_pMainWnd->GetSafeHwnd(), strAppError, NULL, MB_YESNO | MB_ICONQUESTION | MB_TOPMOST ) == IDYES ) // ok
							{
								hr = S_OK;
								theApp.m_bAppMismatch = true; 
							}
							else
							{
								theApp.m_bAppMismatch = false; 
							}

							m_nState = NONE;
							ClearStatusError();
						}
						else // ÆäËü´íÎó
						{
							m_bConnected = false;
							m_nState = NONE;
						}
					}//if( hr != S_OK )
                }
            }//if( m_nAction == mbsCONNECT ) 

            if((hr == S_OK) && (m_nState != NONE))      
            {
                ++m_dwReadMessages;
                msg->SuccessCount( msg->SuccessCount() + 1 );
                m_nState = NEXT;

                // Reset the error messages/retry counter if the message succeeded
                if( m_dwRetryMessages != 0 )
                {
                    m_dwRetryMessages = 0;
                    ClearStatusError();
                }
            } // if( hr == S_OK )
        }// if (hr == S_OK)
        else // Response failed
        {
            if( SUCCEEDED(hr) && (msg->ElapsedTick() >= m_dwTimeOut))   // Timeout: hr = S_FALSE
            {	
                hr = E_CLIENT_TIMEOUT;
                ++m_dwTimeoutMessages;
            }

            if( FAILED(hr))
            {
				// Log Error
				CString s = CString("Slave Failed: ") + DumpBytes(msg->InputBegin(), msg->InputCount());
				theApp.LogInfo( s );

                // Check communications error
                ASSERT( m_pDriver != NULL );
                if( m_pDriver->errnum() != S_OK )
                {
                    // TODO: Dump the serial/socket parameters
                    CString s;
                    HRESULT hr2 = HRESULT_FROM_WIN32( m_pDriver->errnum());
					s.Format( L"CModbusThread::ReadMessage: IComDriver error: 0x%08x - %d", hr);
                    s += HrGetMessage(hr2);
                    theApp.LogInfo( s );
                }
                m_pDriver->purge();

                // Update error count and set next state
                ++m_dwErrorMessages;
                ++m_dwRetryMessages;
                msg->FailureCount( msg->FailureCount() + 1 );

                // Stop communications if the user is not monitoring 
				// or if the communication port is closed.
                if( !m_pDriver->is_open())
				{
                    m_nState = NONE;
				}
                else
                {
#if INFINITERETRY == 1
                    if((m_dwRetryMessages >= RETRY_COUNT) && (m_nAction != mbsMONITOR))
#else
					if( m_dwRetryMessages >= RETRY_COUNT )
#endif
                    {
                        m_nState = NONE;
						theApp.LogInfo( L"Too many errors, no more retries." );
                    }
                    else
					{
						m_nState = NEXT;

                        CString strDebug;
						strDebug.Format( L"Message error - Attempting retry #%d.", m_dwRetryMessages);
                        theApp.LogInfo( strDebug );
                    }
                }

                // Set the error message
                if( m_nState == NONE )
                {
					CString s;
					if( msg->IsWrite())
					{
					    s.Format( IDS_WRITE_RESFAILED, 
						          HrGetMessage(hr), m_nIndex, msg->BeginAlias(), msg->EndAlias(),
								  DumpBytes(msg->OutputBegin(), msg->OutputCount()),
								  DumpBytes(msg->InputBegin(), msg->InputCount()),
								  msg->OutputCount(), msg->EstimateCount(), msg->InputCount());
					}
					else
					{
                        s.Format( IDS_READ_RESFAILED, 
						          HrGetMessage(hr), m_nIndex, msg->BeginAlias(), msg->EndAlias(),
								  DumpBytes(msg->OutputBegin(), msg->OutputCount()),
								  DumpBytes(msg->InputBegin(), msg->InputCount()),
								  msg->OutputCount(), msg->EstimateCount(), msg->InputCount());
					}

                    m_pStatus->SetError(hr, s);
			        //m_bTerminated = true;
                }
            }//if (FAILED(hr))
        }//else(Response Failed)

        if((m_nState == NEXT) || (m_nState == NONE))
        {
            // Update the repeat counter and status
            if( hr == S_OK )
            { 
                msg->DecrementRepeatCount();
                m_pStatus->IncrementCount( msg->InputCount());
            }
        } 

		singleLock.Unlock();
    } //if (GetState() == RESPONSE)

    return hr;
}

// =============================================================
// Function name: NextState
// Description  : 
//                
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.07 18:24
// =============================================================
HRESULT CModbusThread::NextState()
{
    HRESULT hr = S_OK;
    if( GetState() == NEXT )
    {
        CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		ASSERT( singleLock.IsLocked());

        if( GetState() != NEXT )
		{
            return hr;
		}

        // Use the current message result instead of the modbus status error,
        // if possible.
        HRESULT hr2 = GetStatusError();  
        if((m_nIndex > 0) && (m_nIndex < m_pMessages->GetMessageCount()))
        {
            CMessage* msg = m_pMessages->GetMessage( m_nIndex );
            if( msg != NULL )
            {
                hr2 = msg->m_hr;     //the current message result
            }
        }

        // Move to the next message if the message was successfully received, or if
        // the user is monitoring.
        if( SUCCEEDED(hr2))           // Response successful
        {
            ClearStatusError();
            ++m_nIndex;
        }
        else if( m_dwRepeatTime > 0 ) // Monitor
        {
            ++m_nIndex;
        }

        if( m_nIndex < m_pMessages->GetMessageCount())
        {
			m_nState = REQUEST;
        }
        else
        {
            //DoLogMessages();
            if( m_dwRepeatTime > 0 )  // Monitor
            {
                // Remove message which are not flagged as repeatable
                m_pMessages->RemoveNonRepeatMessages();

                // Roll-over the Modbus progress count
                m_pStatus->SetCount( 0 );
                m_pStatus->TotalCount( m_pMessages->GetTotalByteCount());

                // Check requested scan time
                DWORD dwTicks = ::GetTickCount();
                while((dwTicks + 11) < m_dwRepeatStamp )
                {
                    if( m_bTerminated || (GetState() != NEXT))
					{
                        return S_OK;
					}

                    ::Sleep(0);
                    dwTicks = ::GetTickCount();
                }

                if((dwTicks + 21) >= m_dwRepeatStamp )
                {
                    if( m_pMessages->GetMessageCount() > 0 )
                    {
                        m_nIndex = 0;              // Current msg index
                        m_pEvent->SetEvent();      // Set event
                        m_nState = REQUEST;          

                        m_dwRepeatStamp += m_dwRepeatTime;
                        if( dwTicks > m_dwRepeatStamp )
						{
                            m_dwRepeatStamp = dwTicks + m_dwRepeatTime;
						}

                        // Simple IIR filter to normalizing the scan time reporting
                        if( m_dwScanTime == 0 )
						{
                            m_dwScanTime = dwTicks - m_dwTimeStamp;
						}
                        else
						{
                             m_dwScanTime = static_cast<DWORD>((SCAN_FILTER * m_dwScanTime) +
                                            ((1.0f - SCAN_FILTER) * (dwTicks - m_dwTimeStamp)));
						}

                        m_dwTimeStamp = dwTicks;
                    }//if (m_pMessages->GetMessageCount() > 0)
                    else
                    {
                        m_nIndex = 0;
                        m_pEvent->SetEvent();
                        m_nState = NONE;
                    }
                }//if ((dwTicks + 21) >= m_dwRepeatStamp)
            }//if (m_dwRepeatTime > 0) ==> Monitor 
            else // Non Monitor
            {
                m_pEvent->SetEvent();
                m_nState = NONE;
				//m_bTerminated = true;
            }
        }//else ==> m_nIndex == m_pMessages->GetMessageCount()

		singleLock.Unlock();
    }//if (GetState() == NEXT)

    return S_OK;
}

// =============================================================
// Function name: ClearStatusError
// Description  : Clear m_pStatus error code and error message 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.07 19:24
// =============================================================
void CModbusThread::ClearStatusError()
{
	if( m_pStatus->GetError() != S_OK )
	{
		m_pStatus->SetError( S_OK, HrGetMessage(S_OK));
	}
}

//---------------------------------------------------------------
// Return status error code
HRESULT CModbusThread::GetStatusError() const
{
	return m_pStatus->GetError(); 
}

//---------------------------------------------------------------
// Function name: GetStatusErrorMsg
CString CModbusThread::GetStatusErrorMsg() const
{
	return m_pStatus->GetErrorMsg();
}

//---------------------------------------------------------------
LPCTSTR CModbusThread::GetStatusMessage() const
{
	return m_pStatus->GetMessage();
}

//---------------------------------------------------------------
int CModbusThread::GetStatusProgress() const
{
	return m_pStatus->GetProgress();
}

//--------------------------------------------------------------
void CModbusThread::SetState( State AState )
{
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		 m_nState = AState;
		 singleLock.Unlock();
	}
}

// =============================================================
// Function name: CheckProductInfo
// Description  : Only check ProductID to make sure 
//                we communicate with the appropriate product type
// Return type  : Returns S_OK if the device passes the ID check.
// Parameters   : 
// Author       : rjvs     2002-08-26 14:24:03 (UTC-6:00)
//                wangdong 2010-03-08 10:20:00
//                wangdong 2011-10-26 10:40:00
// =============================================================
HRESULT CModbusThread::CheckProductInfo( CString &s )
{
	CMessage* msg = m_pMessages->GetMessage( m_nIndex );
	if( !CheckReadPtr(msg, sizeof(CMessage)))
	{
        return E_POINTER;
	}

	HRESULT hr = S_OK;
	bool bDevice = false;
	int nProductNumber = theApp.GetProductNumber();

	// Product Number: 31001
	CAliasItem* pAlias = msg->FindAlias( 31001 );       // iProdNum
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
        bDevice = (nProductNumber == pAlias->remI2())? true : false;
		if( !bDevice )
		{
			hr = E_DEVICE_PRODUCT_NUMBER;
			CString strError;
			strError.Format( IDS_PROID_ERROR, 
				             pAlias->GetAlias(), pAlias->GetName(), nProductNumber, HrGetMessage(hr));
			m_pStatus->SetError( hr, strError );
			return hr;
		}
	}
	else
	{
		hr = E_DEVICE_ALIAS_MISSING;
        m_pStatus->SetError(hr, HrGetMessage(E_DEVICE_ALIAS_MISSING));
        return hr;
	}

	// Application Type: 31002, Only for TS350
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		CAliasItem* pAlias = msg->FindAlias( 31002 );   // iAppType 
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			bool bAppType = (theApp.GetAppType() == pAlias->remI2()) ? true : false;
			if( !bAppType )
			{
				hr = E_DEVICE_PRODUCT_APPTYPE;
				s.Format( IDS_APP_ERROR, 
							(1 == pAlias->remI2()) ? L"Generator" : L"Mechanical",
							(1 == theApp.GetAppType()) ? L"Generator" : L"Mechanical");
				return hr;
			}
		}
		else
		{
			hr = E_DEVICE_ALIAS_MISSING;
			m_pStatus->SetError(hr, HrGetMessage(E_DEVICE_ALIAS_MISSING));
			return hr;
		}
	}//TS350

	// Firm Code: 31003
	pAlias = msg->FindAlias( 31003 );       // iProjVer
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		int nCode = pAlias->remI2();
		if( nCode != theApp.GetCurrentCode())
		{
			// Check if current xml can match this code
			if( theApp.IsXMLMatchCode( theApp.GetProductType(), theApp.GetXMLVersion(), nCode ))
			{
				theApp.SetCurrentCode( nCode );
			}
			else // current xml mismatch
			{
				CString str;
				float fXMLVersion = theApp.GetXMLVersionFromCode( theApp.GetProductType(), nCode );
				if( fabs(fXMLVersion) > EPSILON_FLT )  //if( fXMLVersion != 0.0 )  // Configurator has xml file can match this code
				{
					str.Format( IDS_XMLFILE_SWITCH, theApp.GetXMLVersion(), fXMLVersion );
					if( ::MessageBoxW( theApp.m_pMainWnd->GetSafeHwnd(), str, NULL, MB_YESNO | MB_ICONQUESTION ) == IDYES )
					{
						theApp.SetResetViewFlag( true );
						theApp.SetXMLVersion( fXMLVersion );
						theApp.SetCurrentCode( nCode );
						str = L"";
					}
					else
					{
						str = ::HrGetMessage(E_DEVICE_XML_MISMATCH);
					}

					hr = E_DEVICE_XML_MISMATCH;
					m_pStatus->SetError( hr, str );
					return hr;
				}
				else  // Configurator can't match this code
				{
					hr = E_DEVICE_CFG_MISMATCH;
					str.Format( EMD_CFG_MISMATCH, nCode );
					m_pStatus->SetError( hr, str );
					return hr;
				}
			}
		}//if( nCode != theApp.GetCurrentCode())
	}//if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	else
	{
		hr = E_DEVICE_ALIAS_MISSING;
        m_pStatus->SetError(hr, HrGetMessage( E_DEVICE_ALIAS_MISSING ));
        return hr;
	}

	return hr;
}

// =============================================================
// Function name: SetAliasMap
// Description  : Set m_pAliasMap of m_pMessages(msgvector) 
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 16:22:00
// =============================================================
void CModbusThread::SetAliasMap( CAliasMap* pAliasMap )
{
    if( NULL == pAliasMap )
	{
		return;
	}

	if (CheckWritePtr( m_pClient.get(), sizeof(CClient)))
    {
        ASSERT( m_pMessages->GetMessageCount() == 0 );

        CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			m_pMessages->SetAliasMap( pAliasMap );

			singleLock.Unlock();
		}
    }
}

// =============================================================
// Function name: Connect
// Description  : Open
//                1. Check m_pDriver;
//                2. Check if has opened, true: return S_OK
//                                        false: open it
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 16:50:00
// =============================================================
HRESULT CModbusThread::Connect()
{
	//1. Check m_pDriver
    if( m_bModbusTcp )
    {
        ASSERT( m_pDriver == m_pSocket.get());
    }
    else
    {
        ASSERT( m_pDriver == m_pSerial.get());
    }

	//2. Open
    if( CheckWritePtr(m_pDriver, sizeof(IComDriver)))
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( m_pDriver->is_open())
			{
				return S_OK;
			}

			int err = m_pDriver->open();
			if( err != ERROR_SUCCESS )
			{
				m_pDriver->close();
				return HRESULT_FROM_WIN32(err);
			}

			singleLock.Unlock();
			return S_OK;
		}
    }

    return E_POINTER;
}

// =============================================================
// Function name: Close
// Description  : Close m_pSocket or m_pSerial
//                Reset m_bConnected flag                                       
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 16:50:00
// =============================================================
void CModbusThread::Close()
{
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		// Close
		if( m_bModbusTcp )
		{
			if( CheckWritePtr( m_pSocket.get(), sizeof(CSocketDriver)))
			{
				m_pSocket->close();
			}
		}
		else
		{
			if( CheckWritePtr( m_pSerial.get(), sizeof(CSerialDriver)))
			{
				m_pSerial->close();
			}
		}

		// Reset connected flag
		m_bConnected = false;

		singleLock.Unlock();
	}
}

// =============================================================
// Function name: ClearMessages
// Description  : Clears the array of messages.
//                                                       
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-09 09:15:00
// =============================================================
void CModbusThread::ClearMessages(void)
{
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		if( CheckWritePtr( m_pMessages.get(), sizeof(modbus::CModbusMsgVector)))
		{
			m_pMessages->clear();
		}

		singleLock.Unlock();
	}
}

// =============================================================
// Function name: ResetThreadFlags
// Description  : Reset flags and event                                                      
// Return type  : 
// Parameters   : 
// Author       : wangdong 2010-03-09 09:15:00
// =============================================================
void CModbusThread::ResetThreadFlags()
{
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		// Reset flags
		m_nIndex = 0;
		m_nState = NONE;
		m_nLastAction = m_nAction;
		m_nAction = mbsNONE;

		// Reset event
		if( CheckWritePtr(m_pEvent.get(), sizeof(CEvent)))
		{
			m_pEvent->ResetEvent();
		}

		singleLock.Unlock();
	}
}

//==============================================================
// Build Modbus Message
// =============================================================
// Function name: LoopbackMessage
// Description  : 1. Clear msg vector;
//                2. Build modbus loopback(dialog) msg;
//                3. Set flags
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 16:50:00
// =============================================================
HRESULT CModbusThread::LoopbackMessage()
{
    if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::LoopbackMessage" );
			}

			m_pMessages->clear();                               // clear modbus msg vector 
			m_pMessages->BuildLoopbackMessage( m_bModbusTcp );  // build loopback msg

			g_bSuspend = false;
			m_nIndex = 0;
			m_nState = REQUEST;
			m_nLastAction = m_nAction;
			m_nAction = mbsTEST;
			m_dwRepeatTime = 0;
			m_dwErrorMessages = 0;       
			m_dwRetryMessages = 0;
			m_dwTimeoutMessages = 0;     
			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_DIAG_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

// =============================================================
// Function name: ConnectToDevice
// Description  : 1. Clear msg vector;
//                2. Build modbus readback msg;
//                3. Set flags
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 16:50:00
// =============================================================
HRESULT CModbusThread::ConnectToDevice()
{
	if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::ConnectToDevice" );
			}

			m_pMessages->clear();   // clear modbus msg vector

			// Read the product info: number and apptype first
			m_pMessages->BuildMessages( 31001, 31004, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp );

			// Read the rest of the alias space
			m_pMessages->BuildMessages(     1, 10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp );
			m_pMessages->BuildMessages( 10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp );
			m_pMessages->BuildMessages( 30001, 40000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp );
			m_pMessages->BuildMessages( 40001, 50000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp );

			g_bSuspend = false;
			m_nIndex = 0;
			m_nState = REQUEST;
			m_nLastAction = m_nAction;
			m_nAction = mbsCONNECT;
			m_dwRepeatTime = 0;
			m_dwErrorMessages = 0;
			m_dwRetryMessages = 0;
			m_dwTimeoutMessages = 0;
			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_CONNECT_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

// =============================================================
// Function name: ReceiveFromDevice
// Description  : 1. Clear msg vector;
//                2. Build modbus read msg;
//                3. Set flags
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 19:40:00
// =============================================================
HRESULT CModbusThread::ReceiveFromDevice()
{
    if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::ReceiveFromDevice" );
			}

			m_pMessages->clear();   // clear modbus msg vector

			m_pMessages->BuildMessages(    1,  10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( 10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( 30001, 40000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( 40001, 50000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, true);

			g_bSuspend = false;
			m_nIndex = 0;
			m_nState = REQUEST;
			m_nLastAction = m_nAction;
			m_nAction = mbsUPLOAD;
			m_dwRepeatTime = 0;
			m_dwErrorMessages = 0;
			m_dwRetryMessages = 0;
			m_dwTimeoutMessages = 0;
			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_RETRIEVE_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

// =============================================================
// Function name: ReceiveFromDevice
// Description  : 1. Clear msg vector;
//                2. Build modbus read msg;
//                3. Set flags
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 19:40:00
// =============================================================
HRESULT CModbusThread::MonitorDevice( const CMessageRanges* pMonitorRanges, CAliasMap* pAliases )
{
	if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
   
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
		{
			theApp.LogInfo( L"CModbusThread::MonitorDevice" );
		}

		m_pMessages->clear();   // clear modbus msg vector
		if( CheckReadPtr( pMonitorRanges, sizeof(CMessageRanges)))
		{
			CMessageRanges::const_iterator it = pMonitorRanges->begin();
			CMessageRanges::const_iterator eit = pMonitorRanges->end();

			while( it != eit )
			{
				int nReadMax = 0, nReadGap = 0;
				CMessageRange msgRange = *it;
				
				if( msgRange.GetBeginAlias() > 30000 )
				{
					nReadMax = global::AnalogReadMax;
					nReadGap = global::AnalogReadGap;
				}
				else
				{
					nReadMax = global::DigitalReadMax;
					nReadGap = global::DigitalReadGap;
				}

				m_pMessages->BuildMessages( msgRange.GetBeginAlias(), msgRange.GetEndAlias(), nReadMax, nReadGap, m_bModbusTcp);

				it++;
			}
		}
		else
		{
			// Default monitored aliases if not specified in configuration
			m_pMessages->BuildMessages(    1,  2016, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp);
			m_pMessages->BuildMessages(10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp);
			m_pMessages->BuildMessages(30001, 30100, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp);
			m_pMessages->BuildMessages(35001, 40000, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp);
			m_pMessages->BuildMessages(40001, 40100, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp);
			m_pMessages->BuildMessages(42001, 42084, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp);
		}

		// History Recording
		if((pAliases != NULL) && (pAliases->size() != 0))
		{
			m_pMessages->BuildMessages( pAliases,     1, 10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp );
			m_pMessages->BuildMessages( pAliases, 10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp );
			m_pMessages->BuildMessages( pAliases, 30001, 40000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp );
			m_pMessages->BuildMessages( pAliases, 40001, 50000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp );
		}

		// Update the 'Active' flag
		m_pMessages->UpdateActive( true );

		for( int i = 0; i < m_pMessages->GetMessageCount(); ++i )
		{
			CMessage* msg = m_pMessages->GetMessage(i);
			msg->RepeatCount(-1);
		}

		g_bSuspend = false;
		m_nIndex = 0;
		m_nState = REQUEST;
		m_nLastAction = m_nAction;
		m_nAction = mbsMONITOR;
		m_dwRepeatTime = 200;
		m_dwScanTime = 0;
		m_dwTimeStamp = ::GetTickCount();
		m_dwRepeatStamp = m_dwTimeStamp + m_dwRepeatTime;
		m_dwErrorMessages = 0;
		m_dwRetryMessages = 0;
		m_dwTimeoutMessages = 0;
		m_pEvent->ResetEvent();
		m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
		m_pStatus->SetMessage( IDS_MONITOR_OPERATION );
		m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

		singleLock.Unlock();
	}

    return S_OK;
}

// =============================================================
// Function name: SendToDevice
// Description  : 1. Clear msg vector;
//                2. Build modbus Write msg;
//                3. Set flags
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2010-03-08 19:40:00
// =============================================================
HRESULT CModbusThread::SendToDevice( bool bDirtyOnly, bool bTuningOnly )
{
    if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::SendToDevice" );
			}

			m_pMessages->clear();                                // Clear modbus msg vector

			// Write the configuration data
			CAliasMap SendMap;
			CAliasMap* pAliasMap = m_pMessages->GetAliasMap();   // m_aAliases

			AliasProtectSet SendSet;
			if( bTuningOnly )   
			{
				SendSet.insert( CAliasItem::PROTECT_TUNE );
				pAliasMap->RangeInclude( &SendMap, 1, 50000, SendSet, bDirtyOnly );
			}
			else
			{
				// Exclude Setpoint and Calibrate
				SendSet.insert( CAliasItem::PROTECT_SETPOINT );
				SendSet.insert( CAliasItem::PROTECT_CALIBRATE );
				pAliasMap->RangeExclude( &SendMap, 1, 50000, SendSet, bDirtyOnly );
			}

			CAliasItem* pNewCfg = pAliasMap->find_name( L"fNewCfg" );
			if( pNewCfg != NULL )
			{
				pNewCfg->valBool(false);
			}

			m_pMessages->BuildMessages( &SendMap,     1, 10000, global::DigitalWriteMax, 0, m_bModbusTcp, true, false );
        
			if((theApp.GetProductType() == CConfiguratorApp::DSC100) ||
			   (theApp.GetProductType() == CConfiguratorApp::DSC110))
			{
				// DSC100, DSC110: Can't Send MAC Address:40132 - 40137 when user's password is not "manage"
				if( global::Security.CanAdmin())
				{
					m_pMessages->BuildMessages( &SendMap, 40001, 50000, global::AnalogWriteMax, 0,  m_bModbusTcp, true, false );
				}
				else     
				{
					m_pMessages->BuildMessages( &SendMap, 40001, 40131, global::AnalogWriteMax, 0,  m_bModbusTcp, true, false );
					m_pMessages->BuildMessages( &SendMap, 40138, 50000, global::AnalogWriteMax, 0,  m_bModbusTcp, true, false );
				}
			}
			else
			{
				m_pMessages->BuildMessages( &SendMap, 40001, 50000, global::AnalogWriteMax, 0,  m_bModbusTcp, true, false );
			}

			if( pNewCfg != NULL )
			{
				pNewCfg->valBool( true );
				m_pMessages->BuildMessages( pAliasMap, pNewCfg->GetAlias(), pNewCfg->GetAlias(),
											global::DigitalWriteMax, 0, m_bModbusTcp, true, false );
				pNewCfg->valBool( false );
			}

			// Read everything from device
			m_pMessages->BuildMessages( 1,     10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp,  false, false );
			m_pMessages->BuildMessages( 10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp,  false, false );
			m_pMessages->BuildMessages( 30001, 40000, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp,  false, false );
			m_pMessages->BuildMessages( 40001, 50000, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp,  false, false );

			g_bSuspend = false;
			m_nIndex = 0;
			m_nState = REQUEST;
			m_nLastAction = m_nAction;
			m_nAction = mbsDOWNLOAD;
			m_dwRepeatTime = 0;
			m_dwErrorMessages = 0;
			m_dwRetryMessages = 0;
			m_dwTimeoutMessages = 0;
			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_SEND_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

// =============================================================
// Function name: SendCtlCommSettings
// Description  : Controller Comm       and    Date-Time Parameters
//                2154,2155,40025-40052        40053 - 40058 
// Return type  : 
// Parameters   : bDateTime, true: 40053 - 40058
// Author       :  wangdong 2011-11-17 16:00:00
// =============================================================
HRESULT CModbusThread::SendCtlCommSettings( bool bDateTime )
{
    if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::SendCtlCommSettings" );
			}

			CAliasMap* pAliasMap = m_pMessages->GetAliasMap();
			CAliasItem* pSaveAlias = pAliasMap->find_name( L"eComSave" );  // 2193
			if( pSaveAlias != NULL)
			{
				pSaveAlias->valBool( FALSE );
			}

			if( !bDateTime )
			{
				// Write aliases: 2194-2195, 40025 - 40052
				m_pMessages->BuildMessages( 2194,  2195,  global::DigitalWriteMax, 0, m_bModbusTcp, true, false);
				m_pMessages->BuildMessages( 40025, 40052, global::AnalogWriteMax,  0, m_bModbusTcp, true, false);
			}
			else
			{
				// Write aliases: 40053 - 40058
				m_pMessages->BuildMessages( 40053, 40058, global::AnalogWriteMax,  0, m_bModbusTcp, true, false);
			}

			if( pSaveAlias != NULL )
			{
				pSaveAlias->valBool( TRUE );
				m_pMessages->BuildMessages( pSaveAlias->GetAlias(), pSaveAlias->GetAlias(), global::DigitalWriteMax, 0, m_bModbusTcp, true, false);
				pSaveAlias->valBool( FALSE );
			}

			// Now read them back
			if( !bDateTime )
			{
				m_pMessages->BuildMessages( 2194,  2195,  global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, false );
				m_pMessages->BuildMessages( 40025, 40052, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, false );
			}
			else
			{
				m_pMessages->BuildMessages( 40053, 40058, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, false );
			}


			if((m_nState == NONE) || (m_nState == SUSPEND))
			{
				m_nIndex = 0;
				m_nState = REQUEST;
			}

			m_nLastAction = m_nAction;
			if( m_nAction == mbsNONE )
			{
				g_bSuspend = false;
				m_nAction = mbsDOWNLOAD;
			}

			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_WRITE_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

// =============================================================
// Function name: ReadAliasesFromDevice / WriteAliasesToDevice
// Description  : 
// Return type  : 
// Parameters   : 
// Author       :  wangdong 2011-03-13 12:40:00
// =============================================================
HRESULT CModbusThread::ReadAliasesFromDevice( CAliasMap* pAliases )
{
	if((!CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient))) ||
	   (!CheckReadPtr( pAliases, sizeof(CAliasMap))))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::ReadAliasesFromDevice" );
			}

			m_pMessages->BuildMessages( pAliases,     1, 10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( pAliases, 10001, 20000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( pAliases, 30001, 40000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, true);
			m_pMessages->BuildMessages( pAliases, 40001, 50000, global::AnalogReadMax,  global::AnalogReadGap,  m_bModbusTcp, false, true);


			if((m_nState == NONE) || (m_nState == SUSPEND))
			{ 
				m_nIndex = 0;
				m_nState = REQUEST;
			}

			m_nLastAction = m_nAction;
			if( m_nAction == mbsNONE )
			{
				g_bSuspend = false;
				m_nAction = mbsUPLOAD;            
			}

			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_READ_OPERATION );
			m_pStatus->TotalCount( m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

//---------------------------------------------------------------
// Function name: WriteAliasesToDevice
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2010-03-13 12:40:00
//---------------------------------------------------------------
HRESULT CModbusThread::WriteAliasesToDevice( CAliasMap* pAliases, bool bTuning )
{
	if((!CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient))) ||
	   (!CheckReadPtr( pAliases, sizeof(CAliasMap))))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo( L"CModbusThread::WriteAliasesToDevice" );
			}

			// Write aliases 
			m_pMessages->BuildMessages( pAliases,     1, 10000, global::DigitalWriteMax, global::DigitalReadGap, m_bModbusTcp, true, false);
			m_pMessages->BuildMessages( pAliases, 40001, 50000, global::AnalogWriteMax, global::AnalogReadGap, m_bModbusTcp, true, false);
			
			
			// Now read them back
			m_pMessages->BuildMessages(pAliases,     1, 10000, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, false);
			m_pMessages->BuildMessages(pAliases, 40001, 50000, global::AnalogReadMax, global::AnalogReadGap, m_bModbusTcp, false, false);

			if((m_nState == NONE) || (m_nState == SUSPEND))
			{
				m_nIndex = 0;
				m_nState = REQUEST;
			}

			m_nLastAction = m_nAction;
			if( m_nAction == mbsNONE )
			{
				g_bSuspend = false;
				m_nAction = mbsDOWNLOAD;
			}

			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_WRITE_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }//else 

    return S_OK;
}

///=============================================================
// Calibrate DSC100
// =============================================================
// Function name: SendCalibrationFlags
// Description  : Send all calibrate flags( 2201 - 2210 )
// Return type  : 
// Parameters   : bSaveFlag: true, save parameters to eeprom
// Author       : wangdong 2010-04-27 11:30:00
// =============================================================
HRESULT CModbusThread::SendCalibrationFlags( UINT uProType, bool bSaveFlag )
{
    if( !CheckWritePtr( m_pClient.get(), sizeof(modbus::CClient)))
	{
        return E_POINTER;
	}
    else
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				theApp.LogInfo(  L"CModbusThread::SendCalibrationFlags"  );
			}

			if( CConfiguratorApp::DSC100 == uProType )
			{
				DSC100Calibrate( bSaveFlag );
			}
			else   // DSC110
			{
				DSC110Calibrate( bSaveFlag );
			}

			if((m_nState == NONE) || (m_nState == SUSPEND))
			{
				m_nIndex = 0;
				m_nState = REQUEST;
			}

			m_nLastAction = m_nAction;
			if( m_nAction == mbsNONE )
			{
				g_bSuspend = false;
				m_nAction = mbsDOWNLOAD;
			}

			m_pEvent->ResetEvent();
			m_pStatus->SetError( S_OK, HrGetMessage( S_OK ));
			m_pStatus->SetMessage( IDS_SENDCAL_OPERATION );
			m_pStatus->TotalCount(m_pMessages->GetTotalByteCount());

			singleLock.Unlock();
		}
    }

    return S_OK;
}

//-----------------------------------------------------------
// DSC100 Calibrate control
void CModbusThread::DSC100Calibrate( bool bSaveFlag )
{
	CAliasMap* pAliasMap = m_pMessages->GetAliasMap();
	CAliasItem* pNewCfg = pAliasMap->find_name( L"fNewCfg" );
	if (pNewCfg != NULL)
	{
		pNewCfg->valBool(false);
	}

	// Write aliases
	m_pMessages->BuildMessages( 2201, 2210, global::DigitalWriteMax, 0, m_bModbusTcp, true, false);

	// 2209: true: User choose no: Reset it
	CAliasItem* pChooseNoAlias = pAliasMap->find_name( L"kFlag_NotCal" );
	if( pChooseNoAlias != NULL )
	{
		if( pChooseNoAlias->valBool())
		{
			pChooseNoAlias->valBool( false );
		}
	}

	if( bSaveFlag )
	{
		if( pNewCfg != NULL )
		{
			pNewCfg->valBool(true);
			m_pMessages->BuildMessages( pAliasMap, pNewCfg->GetAlias(), pNewCfg->GetAlias(),
										global::DigitalWriteMax, 0, m_bModbusTcp, true, false );
			pNewCfg->valBool(false);
		}
	}

	// Now read them, and 12065( Can not Run Flag ) back
	m_pMessages->BuildMessages( 2201, 2210,  global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, false );
	m_pMessages->BuildMessages( 12065,12065, global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp,  false, true );   // Can Not Run Flag
}


//--------------------------------------------------
// DSC110 Calibrate Control
void CModbusThread::DSC110Calibrate( bool bSaveFlag )
{
	CAliasMap* pAliasMap = m_pMessages->GetAliasMap();
	CAliasItem* pNewCfg = pAliasMap->find_name( L"fNewCfg" );
	if (pNewCfg != NULL)
	{
		pNewCfg->valBool(false);
	}

	// Write aliases
	m_pMessages->BuildMessages( 2201, 2204, global::DigitalWriteMax, 0, m_bModbusTcp, true, false);

	if( bSaveFlag )
	{
		if( pNewCfg != NULL )
		{
			pNewCfg->valBool(true);
			m_pMessages->BuildMessages( pAliasMap, pNewCfg->GetAlias(), pNewCfg->GetAlias(),
										global::DigitalWriteMax, 0, m_bModbusTcp, true, false );
			pNewCfg->valBool(false);
		}
	}

	// Now read them back
	m_pMessages->BuildMessages( 2201, 2204,  global::DigitalReadMax, global::DigitalReadGap, m_bModbusTcp, false, false );
}

} // namespace modbus
