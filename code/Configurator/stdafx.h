
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

/* Added by wangdong Begin */
#include <atlimage.h>                 // CImage
// --------------------------------------------------------
// STL headers
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <list>
#include <memory>                           // std::auto_ptr<T>
#include <map>
#include <set>
#include <vector>
#include <afxdlgs.h>

typedef std::vector< BYTE > ByteVector;
typedef std::vector< UINT > UINTVector;

#define EPSILON_FLT  1e-7

// -----------------------------------------------------------------------
// Memory checking macro(s)
#if defined(_DEBUG)
# define POINTER_CHECK
#endif

#if defined(POINTER_CHECK)
    inline bool CheckReadPtr( const void* lp, UINT ucb )
	{
		if( lp == NULL )
		{
			return false;
		}

		return !::IsBadReadPtr( lp, ucb );
	}

	inline bool CheckWritePtr( void* lp, UINT ucb )
	{
		if( lp == NULL )
		{
			return false;
		}

		return !::IsBadWritePtr( lp, ucb );
	}
#else
    inline bool CheckReadPtr( const void* lp, UINT ucb )
	{
	    return ( lp != NULL );
	}

	inline bool CheckWritePtr( void* lp, UINT ucb )
	{
		return ( lp != NULL );
	}
#endif

// ----------------------------------------------------------------------
// Utility
#define SAFE_DELETE(x)        \
  if (x) {                    \
    delete (x);               \
    (x) = NULL;               \
  }

#define SAFE_DELETE_ARRAY(x)  \
  if (x) {                    \
    delete[] (x);             \
    (x) = NULL;               \
  }

//------------------------------------------------------------------------
// Grid ID
#define IDC_BASEGRID         1001
#define IDC_VALUESGRID       IDC_BASEGRID + 1
#define IDC_ALARMSGRID       IDC_BASEGRID + 2

// MainFrame
#define IDC_VIEWTOOLBAR      IDC_BASEGRID + 100
#define IDC_THREADTIMER      IDC_BASEGRID + 101
#define IDC_PASSWORDTIMER    IDC_BASEGRID + 102

// Calibrate
#define IDC_CAL_MINTIMER     IDC_BASEGRID + 151
#define IDC_CAL_MAXTIMER     IDC_BASEGRID + 152
#define IDC_CAL_VERTIMER     IDC_BASEGRID + 153

// SettingTreeView: TreeCtrl ID
#define IDC_TREECTRLID       IDC_BASEGRID + 200

// TrendView: TeeChart ID
#define IDC_TREND_TEECHART   IDC_BASEGRID + 400
#define IDC_TREND_RB01       IDC_BASEGRID + 401
#define IDC_TREND_RB02       IDC_BASEGRID + 402
#define IDC_TREND_RB03       IDC_BASEGRID + 403
#define IDC_TREND_RB04       IDC_BASEGRID + 404

// TuneView
#define IDC_TUNE_BASE        IDC_BASEGRID + 600

// TuneFraView
#define IDC_TUNEFRA_BASE     IDC_BASEGRID + 800       


// -----------------------------------------------------------------------
/* Added by wangdong End */

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


