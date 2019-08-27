#include "StdAfx.h"
#include "SocketDriver.h"

#include "Configurator.h"

//================================================================
#define ZEROBYTE 0

// Static TCP Counter
static WSADATA              wsaData;            
static long                 lTcpCount = 0;

/// <summary>   
/// Load winsock ll, and conform its version
/// </summary>
/// <return>  
/// long err
/// </return>
/// <created>
///   wangdong 2011-02-23 18:10:00 (UTC-6:00)</created>
long TcpDriverInit( void )
{
    static int err = S_OK;

    if( ::InterlockedIncrement( &lTcpCount ) == 1 )
    {
         WORD wVersion = MAKEWORD(2,2);                 // Version: 2.2
         ::ZeroMemory( &wsaData, sizeof(WSADATA));

         err = ::WSAStartup( wVersion, &wsaData );      // Load WinSock DLL
         if( err == S_OK )
         {
			 // Conform Version: 2.2
			 if((LOBYTE(wsaData.wVersion) != 2) ||
				(HIBYTE(wsaData.wVersion) != 2))
			 {
				 err = -1;
				 ::WSACleanup();     // Realease dll resource
			 }
         }
         else
         {
             ::InterlockedDecrement( &lTcpCount );
			 CString str;
			 str.Format( L"WSAStartup failed. Error: %d", err );
			 theApp.LogInfo( str );
         }
    }//if( ::InterlockedIncrement( &lTcpCount ) == 1 )

	return err;
}

// ----------------------------------------------------------
/// <summary> 
///  Realease dll resource
/// </summary>
/// <return>  
/// </return>
/// <created>
///   wangdong 2011-02-23 18:30:00 (UTC-6:00)</created>
void TcpDriverUninit(void)
{
    if (::InterlockedDecrement(&lTcpCount) == 0)       
    {
        ::WSACleanup();                                   
        //GLogWarning("", "TcpDriverUninit:WSACleanup()");
    }
}
//===========================================================

CSocketDriver::CSocketDriver(void)
	: err_        ( S_OK ),
      socket_     ( INVALID_SOCKET ),          
      connected_  ( false )                    
{
  // Socket address: port number, IP Address...
  ::ZeroMemory( &sockaddr_, sizeof(sockaddr_));

  // Initialize blocking methods
  FD_ZERO( &fds_ );                  

  // Timeout£º1s
  tv_.tv_sec  = 1;                 // seconds
  tv_.tv_usec = 0;                 // microseconds

  // Initialize Windows TCP driver
  err_ = TcpDriverInit();
}


CSocketDriver::~CSocketDriver(void)
{
	close();
    TcpDriverUninit();  // release dll resource
}

// ----------------------------------------------------
/// Set IP address
void CSocketDriver::name(const std::string& strName)
{
    name_ = strName;
}

void CSocketDriver::name(const char* szName)
{
    name_ = szName;
}

/// return IP address
const char* CSocketDriver::name() const
{
  return name_.c_str();
}

// --------------------------------------------------------------------------
/// Set IP port
void CSocketDriver::port(unsigned short n)
{
  port_ = n;
}

/// return IP port
unsigned short CSocketDriver::port() const
{
  return port_;
}

// --------------------------------------------------------------------------
/// <summary>   
/// 1. Create socket£º 
/// 2. Set Device's IP Addr and Port no.
/// 3. Connect the device
/// </summary>
/// <return>  
/// </return>
/// <created>
/// wangdong 2009-12-07 11:00:00 (UTC-6:00)</created>
int CSocketDriver::open()
{
    err_ = S_OK;
    
    if( socket_ == INVALID_SOCKET )    
    {
        connected_ = false;
        
        // 1.Create socket£ºAddress Family 
        socket_ = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        if( socket_ == INVALID_SOCKET )      
        {
            err_ = ::WSAGetLastError();
     
			CString str;
			str.Format( L"socket create failed, Error:  %d( 0x%x).", err_,  err_);
			theApp.LogInfo( str );
			
			return err_;
        }

        // 2. Set Device's IP Addr and Port no.
        ::ZeroMemory( &sockaddr_, sizeof(sockaddr_));
        sockaddr_.sin_family = AF_INET;                            // Address Family
        sockaddr_.sin_port = ::htons( port_ );                     // port (Network byte order)
        sockaddr_.sin_addr.s_addr = ::inet_addr( name_.c_str());   // IP Address (const char* -> unsigned long(Network byte order))

        // 3. Connect the device
		if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
		{
			theApp.LogInfo( L"<CSocketDriver::open Status=\"Connecting\" />" );
		}

        err_ = ::connect( socket_, (sockaddr*)&sockaddr_, sizeof(sockaddr_));
        if( err_ == SOCKET_ERROR )     
        {
           err_ = ::WSAGetLastError();

		   CString str;
		   str.Format( L"CSocketDriver: connect() failed. Error: %d( 0X %x).", err_, err_ );
		   theApp.LogInfo( str );

           ::closesocket( socket_ );     // Close the created socket 
           socket_ = INVALID_SOCKET;
           return err_;
        }

		if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
		{
			theApp.LogInfo( L"<CSocketDriver::open Status=\"Connected\" />" );
		}

        connected_ = true;

        FD_ZERO( &fds_ );                // add the connected socket_ into the queue
        FD_SET( socket_, &fds_ );
    }  // if( socket_ == INVALID_SOCKET )

    return err_;
}

// --------------------------------------------------------------------------
/// <summary>   
/// 1. Shutdown the socket
/// 2. Close the socket
/// 3. Clear socket queue
/// </summary>
/// <return>  
/// </return>
/// <created>
/// wangdong 2009-12-07 16:00:00 (UTC-6:00)</created>
int CSocketDriver::close()
{
    if( connected_ )
    {
        if( socket_ != INVALID_SOCKET )
        { 
            // 1. Shutdown the socket
            err_ = ::shutdown( socket_, SD_SEND );    
            if( err_ == SOCKET_ERROR )
            { 
                err_ = ::WSAGetLastError();
            }

            // 2. Close the socket
            err_ = ::closesocket(socket_);          
            if( err_ == SOCKET_ERROR )
            {
                err_ = ::WSAGetLastError();
            } 

            socket_    = INVALID_SOCKET;
            connected_ = false;

			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
		    {
			    theApp.LogInfo( L"<CSocketDriver::close />" );
		    }
        }
    }// if (connected_)

	// 3. Clear socket queue
    FD_ZERO(&fds_);      

    return S_OK;
}

// --------------------------------------------------------------------------
/// <summary>  
/// 1. Check if cureent socket was connected
/// 2. check readable, if exists, read data
/// </summary>
/// <return>  
/// bytesReadTotal_: read successful bytes count
/// </return>
/// <created>
///   wangdong 2009-12-07 11:00:00 (UTC-6:00)</created>
int CSocketDriver::read(BYTE *pBuffer, int nLength)
{
	// 1. Check if cureent socket was connected
    if( !connected_ )     
    {
        return ERROR_INVALID_HANDLE;    
    }

    bytesRead_ = ZEROBYTE;

    // 2. Check readable
    int e = S_OK;
    unsigned int uCount = 3;
    int bytesReadTotal_ = 0;             

    while((bytesReadTotal_ < nLength) && ( uCount-- ))
    {
        err_ = ::select( 0, &fds_, NULL, NULL, &tv_ );    
        if( err_ == SOCKET_ERROR )  // error
        {
            e = ::WSAGetLastError();
            close();
            err_ = e;         

			CString str;
			str.Format( L"select in CSocketDriver::read() failed. Error: %d(0x%x).", err_, err_ );
			theApp.LogInfo( str );

            return ZEROBYTE;
        }
        else if( err_ == 0 )        // timeout
        {
			theApp.LogInfo( L"select timeout in CSocketDriver::read()" );
            continue;     
        }
        else //if( err_ > 0)        // exists readable socket
        {
            err_ = S_OK;    

            pBuffer +=  bytesReadTotal_;    

            bytesRead_ = ::recv( socket_, (char*)pBuffer, nLength - bytesReadTotal_, 0 );  // read
            if( bytesRead_ == SOCKET_ERROR )   
            {
                e = ::WSAGetLastError();
                close();
                err_ = e;                       

				CString str;
			    str.Format( L"recv in CSocketDriver::read() failed. Error: %d(0x%x).", err_, err_ );
			    theApp.LogInfo( str );

                return ZEROBYTE;
            }      
            else   
            {
                bytesReadTotal_ = bytesReadTotal_ + bytesRead_;

                if( bytesReadTotal_ < nLength )                // Check if read expected bytes count
                {
                    ::Sleep(0);
                }
            }
        } //else if( err_ > 0)
    }  //while()

    return bytesReadTotal_;
}

// --------------------------------------------------------------------------
/// <summary>   
/// 1. Check if cureent socket was connected
/// 2. check writeable, if exists, write data
/// </summary>
/// <return>  
/// lbytesWritten_:  send successful bytes count
/// </return>
/// <created>
///   wangdong 2009-12-07 11:00:00 (UTC-6:00)</created>
int CSocketDriver::write(const BYTE *pBuffer, int nLength)
{
	// 1. Check if cureent socket was connected
    if( !connected_ )    
    {
        return ERROR_INVALID_HANDLE;   
    }

    bytesWritten_ = ZEROBYTE;

    // 2. check writeable
    int e = S_OK;
    unsigned int uCount = 3;
    int bytesWriteTotal_ = 0;             

    while(( bytesWriteTotal_ < nLength ) && ( uCount-- ))
    {
        err_ = ::select( 0, NULL, &fds_, NULL, &tv_ );      
        if( err_ == SOCKET_ERROR )   // error
        {
            e = ::WSAGetLastError();
            close();
            err_ = e;                      

			CString str;
			str.Format( L"select in CSocketDriver::write() failed. Error: %d(0x%x).", err_, err_ );
			theApp.LogInfo( str );

            return ZEROBYTE;
        }
        else if( err_ == 0 )        // time out
        {
            theApp.LogInfo( L"select timeout in CSocketDriver::write()" );
            continue;
        }
        else //if( err_ > 0)        // exists writable socket
        {
            err_ = S_OK;    
            pBuffer +=  bytesWriteTotal_;     

			// Send Data
            bytesWritten_ = ::send( socket_, (char*)pBuffer, nLength-bytesWriteTotal_, 0 ); 
            if( bytesWritten_ == SOCKET_ERROR )  
            {
                 e = ::WSAGetLastError();
                 close();
                 err_ = e;                       

				 CString str;
			     str.Format( L"send in CSocketDriver::write() failed. Error: %d(0x%x).", err_, err_ );
			     theApp.LogInfo( str );

                 return ZEROBYTE;
            }
            else  
            {
                bytesWriteTotal_ += bytesWritten_;    
                if( bytesWriteTotal_ < nLength )      // Check if write expected bytes count 
                {
                    ::Sleep(0);
                }
            }
        }//else if( err_ > 0)
    }//while

    return bytesWriteTotal_;
}


/// <summary>   
/// </summary>
/// <return>
///   </return>
/// <created>
///   wangdong 2009-12-07 16:00:00 (UTC-6:00)</created>
bool CSocketDriver::is_closed() const
{
    return !connected_;
}
// --------------------------------------------------------------------------