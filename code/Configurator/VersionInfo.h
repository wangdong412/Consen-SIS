// -----------------------------------------------------------------------
// $Workfile: $
// Author/Date  : rjvs 2002-07-11 10:11:28 (UTC-6:00)
// Description  : Retrieves the version information from an exe or dll.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-04-19  Wang Dong         Build in VS2010
// 2002-07-11  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#pragma once

class CVersionInfo
{
public:
	CVersionInfo(void);
	~CVersionInfo(void);

private:
  CString FileName_;
  CString FullName_;
  bool Success_;
  CString FileDateTime_;
  COleDateTime FileDateTimeD_;
  CString CompanyName_;
  CString Description_;
  CString FileVersion_;
  CString InternalName_;
  CString LegalCopyright_;
  CString LegalTrademarks_;
  CString OriginalFilename_;
  CString ProductName_;
  CString ProductVersion_;
  __int64 FileSize_;
  DWORD Major_;
  DWORD Minor_;
  DWORD Revision_;
  DWORD Build_;
  VS_FIXEDFILEINFO Vffi_;

public:
  HRESULT GetVersionInfo(const CString& FileName);

  bool Success()                        const { return Success_; }
  const COleDateTime& FileDateTimeD()   const { return FileDateTimeD_; }
  const CString& FileDateTime()         const { return FileDateTime_; }
  const CString& CompanyName()          const { return CompanyName_; }
  const CString& Description()          const { return Description_; }
  const CString& FileVersion()          const { return FileVersion_; }
  const CString& InternalName()         const { return InternalName_; }
  const CString& LegalCopyright()       const { return LegalCopyright_; }
  const CString& LegalTrademarks()      const { return LegalTrademarks_; }
  const CString& OriginalFilename()     const { return OriginalFilename_; }
  const CString& ProductName()          const { return ProductName_; }
  const CString& ProductVersion()       const { return ProductVersion_; }
  const __int64& FileSize()             const { return FileSize_; }
  const DWORD& Major()                  const { return Major_; }
  const DWORD& Minor()                  const { return Minor_; }
  const DWORD& Revision()               const { return Revision_; }
  const DWORD& Build()                  const { return Build_; }
};

//----------------------------------------------------------------

