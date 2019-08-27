// -----------------------------------------------------------------------
// $Workfile: SocketDriver.hpp $
// Author       : Ryan Van Slooten
// Description  :
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2001, Triconex Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
//
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -------------------------------------------
// 2011-02-22  Wang Dong         BCB5 to Visual Studio 2010
// 2002-08-09  Ryan Van Slooten  Changed name to CSocketServer
// 2001-05-08  Ryan Van Slooten  Added Comment Block
// -------------------------------------------------------------------------

#pragma once

#include "comdriver.h"
#include <WinSock2.h>

class CSocketDriver : public IComDriver
{
public:
	CSocketDriver(void);
	~CSocketDriver(void);

private:
  bool     connected_;         // socket connected flag

  // Synchronization
  int               err_;                // Error code

  // Socket details
  SOCKET            socket_;            
  SOCKADDR_IN       sockaddr_;          // Socket address: port number, IP Address...
  unsigned short    port_;              // port number
  std::string       name_;              // IP Address

  // Blocking information
  timeval           tv_;                // time out
  long              tv_sec_;
  long              tv_usec_;
  fd_set            fds_;               // socket queue

  int               bytesRead_;         
  int               bytesWritten_;      

public:
  void              name(const std::string& strName);
  void              name(const char* szName);
  const char*       name() const;

  SOCKET            socket_handle()  { return socket_; }
  SOCKADDR_IN       socket_address() { return sockaddr_; }

  void              port(unsigned short n);
  unsigned short    port() const;

public:
  virtual int       open();
  virtual int       close();
  virtual bool      is_open() const                           { return connected_; }
  virtual int       read(BYTE *pBuffer, int nLength);
  virtual int       write(const BYTE *pBuffer, int nLength);

  virtual int       errnum(void) const                        { return err_; }
  virtual void*     handle(void)                              { return reinterpret_cast<void*>(socket_); }
  virtual int       purge()                                   { return ERROR_SUCCESS; }
 
  virtual bool      is_closed()  const;
};

