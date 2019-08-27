// -----------------------------------------------------------------------
// $Workfile: VersionInfo.cpp $
// Author/Date  : rjvs 2002-07-11 10:11:37 (UTC-6:00)
// Description  : Retrieves the version information from an exe or dll.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Software/Cfg310SV/VersionInfo.cpp 4     7/28/02 5:00p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-04-19  Wang Dong         Build in VS2010
// 2002-07-11  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "VersionInfo.h"

#pragma comment( lib, "version.lib")

#include <cstdio>


//---------------------------------------------------------------------------

struct LANGANDCODEPAGE
{
  WORD wLanguage;
  WORD wCodePage;
};

/* rjvs 2002-07-11 12:56:06 (UTC-6:00)
inline CString CopyString( LPCTSTR lpBuffer, UINT cbBytes )
{
	CString s;
	//s.SetLength( cbBytes );
	WCHAR* p = s.GetBuffer( s.GetLength() + 1);
	s.ReleaseBuffer();

	for (UINT i = 0; i < cbBytes; ++i)
	{
		*p = *(wchar_t *) lpBuffer;
		++p; ++lpBuffer;
	}

	return s;
}*/

bool GetToken( CString& Source, CString& Result, const CString& Delimiter )
{
	int i = Source.Find( Delimiter );                       
	if( i > 0 )
	{
		Result = Source.Left( i );                           
		Source = Source.Right( Source.GetLength() - (i+1));  
		return true;
	}

	Result = Source;
	return false;
}

// rjvs 2002-07-11 13:37:19 (UTC-6:00)
bool ParseVersionString( const CString& Version, DWORD& dwMajor,
                         DWORD& dwMinor, DWORD& dwRevision, DWORD& dwBuild)
{
	CString s, t, d;
	bool bToken = false;

	t = Version;
	d = L".";
	bToken = GetToken( t, s, d );
	if (! bToken)
	{
		d = L",";
		bToken = GetToken( t, s, d );
	}

	if (bToken)
	{
		if( 1 != ::swscanf_s( s, L"%lu", &dwMajor))
		{
			return false;
		}

		GetToken( t, s, d );
		if( 1 != ::swscanf_s( s, L"%lu", &dwMinor))
		{
			return false;
		}

		GetToken(t, s, d);
		if( 1 != ::swscanf_s( s, L"%lu",&dwRevision))
		{
			return false;
		}

		GetToken(t, s, d);
		if( 1 != ::swscanf_s( s, L"%lu",&dwBuild))
		{
			return false;
		}

		return true;
	}

	return false;
}

// rjvs 2002-07-11 13:05:57 (UTC-6:00)
inline HRESULT
VerQueryValueEntry( void * const pBlock,
                    const CString& LanguageAndCodePage,
                    const CString& InfoName,
                    CString& Result )
{
	CString SubBlock;
	LPVOID lpBuffer;
	UINT dwBytes;
	DWORD e;

	SubBlock = LanguageAndCodePage + InfoName;

	// Retrieve file description for language and code page "i".
	e = ::VerQueryValueW( pBlock,
				        SubBlock,
				        &lpBuffer,
				        &dwBytes );
	if( !e )
	{
		e = ::GetLastError();
		return HRESULT_FROM_WIN32(e);
	}

	// Parse the version block ...
	//Result = CopyString((LPCTSTR)lpBuffer, dwBytes );
	Result = (LPCTSTR)lpBuffer;
	
	return S_OK;
}

/// <summary>
///   Constructor</summary>
/// <created>
///   rjvs 2002-07-11 10:25:50 (UTC-6:00)</created>
CVersionInfo::CVersionInfo(void)
  : FileName_         (),
    FullName_         (),
    Success_          (false),
    FileDateTime_     (),
    FileDateTimeD_    (),
    CompanyName_      (),
    Description_      (),
    FileVersion_      (),
    InternalName_     (),
    LegalCopyright_   (),
    LegalTrademarks_  (),
    OriginalFilename_ (),
    ProductName_      (),
    ProductVersion_   (),
    FileSize_         (0),
    Major_            (0),
    Minor_            (0),
    Revision_         (0),
    Build_            (0)
{
    ZeroMemory(&Vffi_, sizeof(Vffi_));
}

/// <summary>
///   Destructor</summary>
/// <created>
///   rjvs 2002-07-11 10:26:12 (UTC-6:00)</created>
CVersionInfo::~CVersionInfo(void)
{
}

/// <summary>
///   </summary>
/// <param name="FileName">
///   Name of the file to check for version information.</param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-07-11 10:26:46 (UTC-6:00)</created>
HRESULT
CVersionInfo::GetVersionInfo( const CString& FileName )
{
	DWORD e = ERROR_FILE_NOT_FOUND;

	Success_ = false;
	FileName_ = FileName;
	if( !FileName.IsEmpty())
	{
		DWORD h = 0;
		DWORD dwSize = 0;

		// Locate the file
		LPWSTR pszName = NULL;
		WCHAR pFullName[_MAX_PATH] = { 0 };
		dwSize = ::SearchPathW( NULL, FileName, NULL, _MAX_PATH,
			                    pFullName, &pszName );
		FullName_ = pFullName;

		if( !FullName_.IsEmpty())
		{
			HANDLE hFile = ::CreateFileW( FullName_, GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL );
			if( hFile != INVALID_HANDLE_VALUE )
			{
				// Windows 2000 and greater only
				/* --
				LARGE_INTEGER li;
				e = GetFileSizeEx(hFile, &li);
				FileSize_ = li.QuadPart;
				-- */

				FileSize_ = ::GetFileSize( hFile, NULL );
				::CloseHandle(hFile);
				if (FileSize_ == -1)
				{
					e = ::GetLastError();
					return HRESULT_FROM_WIN32(e);
				}
			}
	    }

		dwSize = ::GetFileVersionInfoSize( FileName, &h );
		if( !dwSize )
		{
			e = ::GetLastError();
			return HRESULT_FROM_WIN32(e);
		}

		//Info.SetLength( dwSize );
		WCHAR* pInfo = new WCHAR[dwSize];
		memset( pInfo, 0 , sizeof(WCHAR)*dwSize );

		e = ::GetFileVersionInfoW( FileName, h, dwSize, pInfo );
		if( !e )
		{
			e = ::GetLastError();
			return HRESULT_FROM_WIN32(e);
		}

		Success_ = true;

		void * const pBlock = pInfo;
		UINT cbTranslate;

		// Get the fixed file information
		VS_FIXEDFILEINFO* lpVffi;
		e = VerQueryValue( pBlock,
						   TEXT("\\"),
						   (LPVOID*)&lpVffi,
						   &cbTranslate );

		if( e && lpVffi )
		{
			Vffi_ = *lpVffi;
		}

		// Read the list of languages and code pages.
		LANGANDCODEPAGE* lpTranslate;
		e = VerQueryValue( pBlock,
						   TEXT("\\VarFileInfo\\Translation"),
						   (LPVOID*)&lpTranslate,
						   &cbTranslate);
		if( !e )
		{
			e = ::GetLastError();
			return HRESULT_FROM_WIN32(e);
		}

		// Search for the default/user language page
		LANGANDCODEPAGE lcUser = lpTranslate[0];
		LANGID lidDefault = GetUserDefaultLangID();
		bool bLanguage = false;

		// Select the first entry, in case the user language is not found
		// Read the file description for each language and code page.
		for(UINT i = 0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); ++i)
		{
			if (lpTranslate[i].wLanguage == lidDefault)
			{
				bLanguage = true;
				lcUser = lpTranslate[i];
				break;
			}
		}

		if( !bLanguage )
		{
			// Search the file for the system language code page
			lidDefault = GetSystemDefaultLangID();
			for( UINT i = 0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); ++i)
			{
				if( lpTranslate[i].wLanguage == lidDefault )
				{
					bLanguage = true;
					lcUser = lpTranslate[i];
					break;
				}
			}
		}

		// Read the file description for each language and code page.
		CString LangBase;
		HRESULT hr;

		LangBase.Format( TEXT("\\StringFileInfo\\%04x%04x\\"),
					     lcUser.wLanguage,
					     lcUser.wCodePage );

		hr = VerQueryValueEntry( pBlock, LangBase, L"FileVersion", FileVersion_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"FileDescription", Description_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"InternalName", InternalName_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"LegalCopyright", LegalCopyright_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"LegalTrademarks", LegalTrademarks_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"CompanyName", CompanyName_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"OriginalFilename", OriginalFilename_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"ProductName", ProductName_ );
		hr = VerQueryValueEntry( pBlock, LangBase, L"ProductVersion", ProductVersion_ );

		if( !FileVersion_.IsEmpty())
		{
			if( ParseVersionString( FileVersion_, Major_, Minor_, Revision_, Build_))
			{
				// Reformat the version string
				FileVersion_.Format( L"%d.%d.%d.%d", Major_, Minor_, Revision_, Build_ );
			}
		}
		else
		{
			Major_    = HIWORD( Vffi_.dwFileVersionMS );
			Minor_    = LOWORD( Vffi_.dwFileVersionMS );
			Revision_ = HIWORD( Vffi_.dwFileVersionLS );
			Build_    = LOWORD( Vffi_.dwFileVersionLS );

			FileVersion_.Format( L"%d.%d.%d.%d", Major_, Minor_, Revision_, Build_ );
		}

		SAFE_DELETE_ARRAY( pInfo );
		hr = S_OK;
		return hr;
	}

	return S_OK;
}

