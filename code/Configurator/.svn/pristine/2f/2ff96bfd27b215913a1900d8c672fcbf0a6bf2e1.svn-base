// CalSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CalSheet.h"


// CCalSheet

IMPLEMENT_DYNAMIC(CCalSheet, CPropertySheet)

CCalSheet::CCalSheet(UINT nIDCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	,m_pCalibrate( pCalibrate )
{
}

CCalSheet::CCalSheet(LPCTSTR pszCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
	,m_pCalibrate( pCalibrate )
{
}

CCalSheet::~CCalSheet()
{
}


BEGIN_MESSAGE_MAP(CCalSheet, CPropertySheet)
END_MESSAGE_MAP()


// CCalSheet 消息处理程序
// =========================================================================
// Function name: OnInitDialog
// Description  : Hide "上一步"
// Return type  : BOOL
// Parameters   : 
// Author       : WangDong 2012.08.23 14:50:00
// =========================================================================
BOOL CCalSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	/*CMenu *menu = GetSystemMenu( FALSE );
	if( menu != NULL )
	{
		menu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|MF_GRAYED );  // Disable
	}*/

	ModifyStyle( WS_SYSMENU,0 );    // delete system menu

	// Back
	if( GetDlgItem( ID_WIZBACK )->GetSafeHwnd() != NULL )
	{
		GetDlgItem( ID_WIZBACK )->ShowWindow( SW_HIDE );
	}

	// Next
	CString str;
	if( GetDlgItem( ID_WIZNEXT )->GetSafeHwnd() != NULL )
	{
		str.LoadStringW( IDS_CALBTN_NEXT );
		GetDlgItem( ID_WIZNEXT )->SetWindowTextW( str );
	}

	// Cancel
	if( GetDlgItem( IDCANCEL )->GetSafeHwnd() != NULL )
	{
		str.LoadStringW( IDS_CALBTN_CANCEL );
		GetDlgItem( IDCANCEL )->SetWindowTextW( str );
	}

	return bResult;
}

// =========================================================================
// Function name: OnCommand
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.04.28 09:30:00
// =========================================================================
BOOL CCalSheet::OnCommand( WPARAM wParam, LPARAM lParam )
{
	if( LOWORD(wParam)==IDCANCEL )
	{
		CString str;
		str.LoadString( IDS_CAL_CANCEL );
		if( AfxMessageBox( str, MB_YESNO | MB_ICONQUESTION ) != IDYES )
		{
			return TRUE;
		}
	}

	return CPropertySheet::OnCommand(wParam, lParam);
}





// CDSC100Sheet

IMPLEMENT_DYNAMIC(CDSC100Sheet, CCalSheet)

CDSC100Sheet::CDSC100Sheet(UINT nIDCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CCalSheet(nIDCaption, pCalibrate, pParentWnd, iSelectPage)
{
	AddPage( &m_dlgIntro );
	AddPage( &m_dlgMin );
	AddPage( &m_dlgMax );
	AddPage( &m_dlgVer );
	AddPage( &m_dlgSave );
}

CDSC100Sheet::CDSC100Sheet(LPCTSTR pszCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CCalSheet(pszCaption, pCalibrate, pParentWnd, iSelectPage)
{
    AddPage( &m_dlgIntro );
	AddPage( &m_dlgMin );
	AddPage( &m_dlgMax );
	AddPage( &m_dlgVer );
	AddPage( &m_dlgSave );
}

BEGIN_MESSAGE_MAP(CDSC100Sheet, CCalSheet)
END_MESSAGE_MAP()





// CDSC110Sheet

IMPLEMENT_DYNAMIC(CDSC110Sheet, CCalSheet)

CDSC110Sheet::CDSC110Sheet(UINT nIDCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CCalSheet(nIDCaption, pCalibrate, pParentWnd, iSelectPage)
{
	AddPage( &m_dlgCal );
	AddPage( &m_dlgVer );
	AddPage( &m_dlgSaveRun );
}

CDSC110Sheet::CDSC110Sheet(LPCTSTR pszCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd, UINT iSelectPage)
	:CCalSheet(pszCaption, pCalibrate, pParentWnd, iSelectPage)
{
    AddPage( &m_dlgCal );
	AddPage( &m_dlgVer );
	AddPage( &m_dlgSaveRun );
}

BEGIN_MESSAGE_MAP(CDSC110Sheet, CCalSheet)
END_MESSAGE_MAP()