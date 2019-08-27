// -----------------------------------------------------------------------------
// File         : SerialDriver.h
// Description  :
// Author       : Ryan Van Slooten
// Purpose      :
// -----------------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------------
// $Header: /Software/Cfg310SV/SerialDriver.h 1     1/25/02 11:26a Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  ----------------------------------------------
// 2011-02-22  WangDong          BCB5 to Visual Studio 2010
// 2001-10-04  Ryan Van Slooten  Added overlapped i/o and Win32 API
//                               functions to class.
// 2001-06-11  Ryan Van Slooten  Moved two variables into class definition
//                               for multithreading purposes.
// 2001-05-08  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------------
#pragma once

#include "comdriver.h"


class CSerialDriver : public IComDriver
{
public:
	CSerialDriver(void);
	~CSerialDriver(void);

protected:
	HANDLE m_hPort;
	CString m_strPort;
	DWORD m_dwBaud;
	BYTE m_byDataBits;
	DWORD m_dwParity;
	BYTE m_byStopBits;
	BYTE m_byRTSControl;
	bool m_bHandshake;

	int m_nErrorCode;
	DWORD m_dwReadCount;
	DWORD m_dwWriteCount;

	DCB m_dcb;

public:
  virtual int         open(void);
  virtual int         close(void);
  virtual int         purge(void);
  virtual int         read(BYTE* pData, int byteCount);
  virtual int         write(const BYTE* pData, int byteCount);

  virtual bool        is_open(void)     const { return (m_hPort != INVALID_HANDLE_VALUE); }
  virtual int         errnum(void)      const { return m_nErrorCode; }
  virtual void*       handle(void)            { return m_hPort; }  

  int                 config( LPCTSTR strPort, DWORD dwBaud, BYTE byDataBits, DWORD dwParity, 
	                          BYTE byStopBits, BYTE byRTSControl, bool bHandshake );
private:
  int                 internal_config();
};

