// -----------------------------------------------------------------------
// File         : ComDriver.h
// Description  : Interface for IComDriver, communications class.
// Author       : Ryan Van Slooten
// Purpose      : 
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /software/Modbus/ComDriver.h 6     8/24/01 9:51a Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  ------------------------------------------
// 2001-05-08  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------
#pragma once
#include "stdafx.h"

struct IComDriver
{
  virtual ~IComDriver() {}

  virtual int         open(void) = 0;
  virtual int         close(void) = 0;
  virtual bool        is_open(void) const = 0;
  virtual int         read(BYTE* pData, int byteCount) = 0;
  virtual int         write(const BYTE* pData, int byteCount) = 0;

  virtual int         errnum(void) const = 0;
  virtual void*       handle(void) = 0;
  virtual int         purge(void) = 0;
};

