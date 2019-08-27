+// TuneView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "TuneFraView.h"

// Added by wangdong
#include "MainFrm.h"

// =============================================================
///Globals
#define ID_STATIC_STATUS      IDC_TUNEFRA_BASE
#define ID_STATIC_PB          IDC_TUNEFRA_BASE + 1
#define ID_STATIC_RESET       IDC_TUNEFRA_BASE + 2
#define ID_STATIC_RATE        IDC_TUNEFRA_BASE + 3
#define ID_EDIT_PB            IDC_TUNEFRA_BASE + 4
#define ID_EDIT_RESET         IDC_TUNEFRA_BASE + 5
#define ID_EDIT_RATE          IDC_TUNEFRA_BASE + 6
#define ID_BTN_PB_DNFAST      IDC_TUNEFRA_BASE + 7
#define ID_BTN_PB_DN          IDC_TUNEFRA_BASE + 8
#define ID_BTN_PB_UP          IDC_TUNEFRA_BASE + 9
#define ID_BTN_PB_UPFAST      IDC_TUNEFRA_BASE + 10
#define ID_BTN_RES_DNFAST     IDC_TUNEFRA_BASE + 11
#define ID_BTN_RES_DN         IDC_TUNEFRA_BASE + 12
#define ID_BTN_RES_UP         IDC_TUNEFRA_BASE + 13
#define ID_BTN_RES_UPFAST     IDC_TUNEFRA_BASE + 14
#define ID_BTN_RATE_DNFAST    IDC_TUNEFRA_BASE + 15
#define ID_BTN_RATE_DN        IDC_TUNEFRA_BASE + 16
#define ID_BTN_RATE_UP        IDC_TUNEFRA_BASE + 17
#define ID_BTN_RATE_UPFAST    IDC_TUNEFRA_BASE + 18
#define ID_STC_HIDE           IDC_TUNEFRA_BASE + 19

// =============================================================

// CTuneFraView

IMPLEMENT_DYNCREATE(CTuneFraView, CConfiguratorView)      // CView 

CTuneFraView::CTuneFraView()
             :m_pAliasPB( NULL )
			 ,m_pAliasReset( NULL )
			 ,m_pAliasRate( NULL )
			 ,m_pSettingPB( NULL )
			 ,m_pSettingReset( NULL )
			 ,m_pSettingRate( NULL )
			 ,m_pAliases( new CAliasMap())
{
}

CTuneFraView::~CTuneFraView()
{
	m_bmpDown.DeleteObject();
	m_bmpDownFast.DeleteObject();
	m_bmpUp.DeleteObject();
	m_bmpUpFast.DeleteObject();

	m_pAliases->clear();
	m_pAliases.reset();
}

BEGIN_MESSAGE_MAP(CTuneFraView, CConfiguratorView)       // CView       
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED( ID_BTN_PB_DNFAST, &OnPBDnFastClicked )
	ON_BN_CLICKED( ID_BTN_PB_DN, &OnPBDnClicked )
	ON_BN_CLICKED( ID_BTN_PB_UP, &OnPBUpClicked )
	ON_BN_CLICKED( ID_BTN_PB_UPFAST, &OnPBUpFastClicked )
	ON_BN_CLICKED( ID_BTN_RES_DNFAST, &OnResetDnFastClicked )
	ON_BN_CLICKED( ID_BTN_RES_DN, &OnResetDnClicked )
	ON_BN_CLICKED( ID_BTN_RES_UP, &OnResetUpClicked )
	ON_BN_CLICKED( ID_BTN_RES_UPFAST, &OnResetUpFastClicked )
	ON_BN_CLICKED( ID_BTN_RATE_DNFAST, &OnRateDnFastClicked )
	ON_BN_CLICKED( ID_BTN_RATE_DN, &OnRateDnClicked )
	ON_BN_CLICKED( ID_BTN_RATE_UP, &OnRateUpClicked )
	ON_BN_CLICKED( ID_BTN_RATE_UPFAST, &OnRateUpFastClicked )
END_MESSAGE_MAP()


// CTuneFraView 绘图

void CTuneFraView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}

// ========================================================
// Function name : OnEraseBkgnd
// Description   : Set Background Color
// Return type   : BOOL
// Parameters    : 
// Author        : WangDong 2011.02.17 13:00:00
// ========================================================
BOOL CTuneFraView::OnEraseBkgnd( CDC* pDC )
{
    CRect rect;
	GetClientRect( &rect );

	CBrush brush;
	if( !brush.CreateSolidBrush( GetSysColor( COLOR_BTNFACE)))
	{
		return FALSE;
	}

	CBrush* pOldBrush = pDC->SelectObject( &brush );
	rect.InflateRect( 1, 1, 1, 1 );
	pDC->Rectangle( &rect );
	pDC->SelectObject( pOldBrush );
	brush.DeleteObject();

	return TRUE;
}

// ========================================================
// Function name : OnSize
// Description   : Change width of the m_lblStatus
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.17 14:25:00
// ========================================================
void CTuneFraView::OnSize( UINT nType, int cx, int cy )
{
	if( m_lblStatus.GetSafeHwnd())
	{
		CRect rect;
		m_lblStatus.GetClientRect( &rect );
		m_lblStatus.SetWindowPos( &CWnd::wndBottom, 1, 0, cx-2, rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
	}

	if( m_lblHide.GetSafeHwnd())
	{
		CRect rect;
		m_lblHide.GetClientRect( &rect );
		m_lblHide.SetWindowPos( &CWnd::wndBottom, 1, cy-1, cx-2, rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
	}
}


// CTuneView 诊断

#ifdef _DEBUG
void CTuneFraView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTuneFraView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// =======================================================================
// Function name : OnCreate
// Description   : Create Frame Window 
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.16 11:00:00
// =======================================================================
int CTuneFraView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	CRect rect;
	GetClientRect( &rect );
	
	try
	{
		///1. Create
		// Labels
		if((!m_lblStatus.Create( L"",      WS_CHILD | WS_VISIBLE, CRect( 1, 0,  rect.Width()-2, 17), this, ID_STATIC_STATUS)) ||
		   (!m_lblPB.Create(     L"PB",    WS_CHILD | WS_VISIBLE, CRect( 5, 24, 5+50, 24+13), this, ID_STATIC_PB))            ||
		   (!m_lblReset.Create(  L"Reset", WS_CHILD | WS_VISIBLE, CRect( 5, 48, 5+50, 48+13), this, ID_STATIC_RESET))         ||
		   (!m_lblRate.Create(   L"Rate",  WS_CHILD | WS_VISIBLE, CRect( 5, 72, 5+50, 72+13), this, ID_STATIC_RATE))          ||
		   (!m_lblHide.Create(   L"",      WS_CHILD | WS_VISIBLE, CRect( 1, 89, 219, 90), this, ID_STC_HIDE)))
		{
			return -1;
		}

		// Edit
		if((!m_edtPB.CreateEx(    WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 60, 20, 60+92, 20+21 ), this, ID_EDIT_PB)) ||
		   (!m_edtReset.CreateEx( WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 60, 44, 60+92, 44+21 ), this, ID_EDIT_RESET)) ||
		   (!m_edtRate.CreateEx(  WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 60, 68, 60+92, 68+21 ), this, ID_EDIT_RATE)))
		{
			return -1;
		} 

		// Buttons
		// PB
		if((!m_btnPBDnFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 152, 20, 152+17, 20+21), this, ID_BTN_PB_DNFAST)) ||
		   (!m_btnPBDn.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 168, 20, 168+17, 20+21), this, ID_BTN_PB_DN))     ||
		   (!m_btnPBUp.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 186, 20, 186+17, 20+21), this, ID_BTN_PB_UP))     ||
		   (!m_btnPBUpFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 202, 20, 202+17, 20+21), this, ID_BTN_PB_UPFAST)))
		{
			return -1;
		}

		// Reset
		if((!m_btnResetDnFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 152, 44, 152+17, 44+21), this, ID_BTN_RES_DNFAST)) ||
		   (!m_btnResetDn.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 168, 44, 168+17, 44+21), this, ID_BTN_RES_DN))     ||
		   (!m_btnResetUp.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 186, 44, 186+17, 44+21), this, ID_BTN_RES_UP))     ||
		   (!m_btnResetUpFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 202, 44, 202+17, 44+21), this, ID_BTN_RES_UPFAST)))
		{
			return -1;
		}

		// Rate
		if((!m_btnRateDnFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 152, 68, 152+17, 68+21), this, ID_BTN_RATE_DNFAST)) ||
		   (!m_btnRateDn.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 168, 68, 168+17, 68+21), this, ID_BTN_RATE_DN))     ||
		   (!m_btnRateUp.Create(     NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 186, 68, 186+17, 68+21), this, ID_BTN_RATE_UP))     ||
		   (!m_btnRateUpFast.Create( NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, CRect( 202, 68, 202+17, 68+21), this, ID_BTN_RATE_UPFAST)))
		{
			return -1;
		}

		///2. Bitmaps
		if((!m_bmpDownFast.LoadBitmap( IDB_TUNE_DOWNFAST )) ||
		   (!m_bmpDown.LoadBitmap( IDB_TUNE_DOWN ))         ||
		   (!m_bmpUp.LoadBitmap( IDB_TUNE_UP ))             ||
		   (!m_bmpUpFast.LoadBitmap( IDB_TUNE_UPFAST )))
		{
            return -1;
		}

		// Set button bitmaps
		m_btnPBDnFast.SetBitmap((HBITMAP)m_bmpDownFast);
		m_btnPBDn.SetBitmap((HBITMAP)m_bmpDown);
		m_btnPBUp.SetBitmap((HBITMAP)m_bmpUp);
		m_btnPBUpFast.SetBitmap((HBITMAP)m_bmpUpFast);

		m_btnResetDnFast.SetBitmap((HBITMAP)m_bmpDownFast);
		m_btnResetDn.SetBitmap((HBITMAP)m_bmpDown);
		m_btnResetUp.SetBitmap((HBITMAP)m_bmpUp);
		m_btnResetUpFast.SetBitmap((HBITMAP)m_bmpUpFast);

		m_btnRateDnFast.SetBitmap((HBITMAP)m_bmpDownFast);
		m_btnRateDn.SetBitmap((HBITMAP)m_bmpDown);
		m_btnRateUp.SetBitmap((HBITMAP)m_bmpUp);
		m_btnRateUpFast.SetBitmap((HBITMAP)m_bmpUpFast);
	}
	catch( COleException& e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CTuneFraView::OnCreate!");
		return -1;
	}
	catch( ... )
	{
		theApp.LogInfo( L"Unknown exception occured in CTuneFraView::OnCreate!");

		return -1;
	}

	return 0;
}

// =======================================================================
// Function name : PreCreateWindow
// Description   : 
// Return type   : 
// Parameters    : 
// Author        : WangDong 2011.04.18 14:00:00
// =======================================================================
BOOL CTuneFraView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CConfiguratorView::PreCreateWindow(cs);
}

// =======================================================================
// Function name : BindSetting
// Description   : Initialize PB, Reset, Rate Alias 
// Return type   : void
// Parameters    : pSetting: Parent PID SetttingItem
// Author        : WangDong 2011.02.16 10:00:00
// =======================================================================
void CTuneFraView::BindSetting( CSettingItem* pSetting )
{
	ASSERT((pSetting != NULL) && pSetting->IsPIDSettingItem( pSetting->GetChildVector()));

	if( CheckReadPtr( pSetting, sizeof(CSettingItem)) &&
		pSetting->IsPIDSettingItem( pSetting->GetChildVector()))
	{
		m_lblStatus.SetWindowText( pSetting->GetName()); 

		CSettingVector* pChildVector = pSetting->GetChildVector();
		if( pChildVector != NULL )
		{
			CSettingVector::iterator it = pChildVector->begin();
			CSettingVector::iterator eit = pChildVector->end();

			while( it != eit )
			{
				CSettingItem* pPIDItem = *it;
				if( pPIDItem != NULL )
				{
					CString strVarName = pPIDItem->GetVariableName();
					strVarName.MakeUpper();

					if( strVarName.Find(L"PB") != -1 )
					{
						m_pAliasPB = pPIDItem->GetAlias();
						m_pSettingPB = pPIDItem;
					}
					else if( strVarName.Find(L"RES") != -1 )
					{
						m_pAliasReset = pPIDItem->GetAlias();
						m_pSettingReset = pPIDItem;
					}
					else if( strVarName.Find(L"RATE") != -1 )
					{
						m_pAliasRate = pPIDItem->GetAlias();
						m_pSettingRate = pPIDItem;
					}
				}

				it++;
			}//while
		}//if( pChildVector != NULL )
	}//if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
}

// =======================================================================
// Function name : RefreshTuning
// Description   : Refresh controls
//                 1. Initialize values
//                 2. Edits - ReadOnly( Not Enable )
//                 3. Enable Buttons
// Return type   : void
// Parameters    : bConnected
// Author        : WangDong 2011.02.17 09:30:00
// =======================================================================
void CTuneFraView::RefreshTuning( bool bConnected )
{
	bool bEnable = bConnected;
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		bEnable = bEnable && (!theApp.m_bAppMismatch);
	}

	/// Initialize values
	/// Edits - ReadOnly
	m_edtPB.SetReadOnly( !bEnable );
	m_edtReset.SetReadOnly( !bEnable );
	m_edtRate.SetReadOnly( !bEnable );

	/// Enable Buttons
	m_btnPBDnFast.EnableWindow( bEnable );
	m_btnPBDn.EnableWindow( bEnable );
	m_btnPBUp.EnableWindow( bEnable );
	m_btnPBUpFast.EnableWindow( bEnable );

	m_btnResetDnFast.EnableWindow( bEnable );
	m_btnResetDn.EnableWindow( bEnable );
	m_btnResetUp.EnableWindow( bEnable );
	m_btnResetUpFast.EnableWindow( bEnable );

	m_btnRateDnFast.EnableWindow( bEnable );
	m_btnRateDn.EnableWindow( bEnable );
	m_btnRateUp.EnableWindow( bEnable );
	m_btnRateUpFast.EnableWindow( bEnable );

	if((m_pAliasPB != NULL) && (m_pAliasReset != NULL) && (m_pAliasRate != NULL))
	{
		CString s;
		float fValue = m_pAliasPB->valR4();
		s.Format( L"%.2f", fValue );
		m_edtPB.SetWindowText( s );                     // PB

		fValue = m_pAliasReset->valR4();
		s.Format( L"%.2f", fValue );
		m_edtReset.SetWindowText( s );                  // Reset

		fValue = m_pAliasRate->valR4();
		s.Format( L"%.4f", fValue );
		m_edtRate.SetWindowText( s );                   // Rate
	}
}

// ==============================================================
// Function name : Click Button
// Description   : 
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.19 12:45:00
// ==============================================================
void CTuneFraView::OnPBDnFastClicked()
{
	TuningValue( m_pAliasPB, m_pSettingPB, &m_edtPB, -1.0f );
}

void CTuneFraView::OnPBDnClicked()
{
    TuningValue( m_pAliasPB, m_pSettingPB, &m_edtPB, -0.1f );
}

void CTuneFraView::OnPBUpClicked()
{
    TuningValue( m_pAliasPB, m_pSettingPB, &m_edtPB, 0.1f );
}

void CTuneFraView::OnPBUpFastClicked()
{
    TuningValue( m_pAliasPB, m_pSettingPB, &m_edtPB, 1.0f );
}

/************************** Reset ***************************/
void CTuneFraView::OnResetDnFastClicked()
{
    TuningValue( m_pAliasReset, m_pSettingReset, &m_edtReset, -1.0f );
}

void CTuneFraView::OnResetDnClicked()
{
    TuningValue( m_pAliasReset, m_pSettingReset, &m_edtReset, -0.1f );
}

void CTuneFraView::OnResetUpClicked()
{
    TuningValue( m_pAliasReset, m_pSettingReset, &m_edtReset, 0.1f );
}

void CTuneFraView::OnResetUpFastClicked()
{
    TuningValue( m_pAliasReset, m_pSettingReset, &m_edtReset, 1.0f );
}

/************************** Rate ***************************/
void CTuneFraView::OnRateDnFastClicked()
{
    TuningValue( m_pAliasRate, m_pSettingRate, &m_edtRate, -0.01f );
}

void CTuneFraView::OnRateDnClicked()
{
    TuningValue( m_pAliasRate, m_pSettingRate, &m_edtRate, -0.001f );
}

void CTuneFraView::OnRateUpClicked()
{
    TuningValue( m_pAliasRate, m_pSettingRate, &m_edtRate, 0.001f );
}

void CTuneFraView::OnRateUpFastClicked()
{
    TuningValue( m_pAliasRate, m_pSettingRate, &m_edtRate, 0.01f );
}

// ==============================================================
// Function name : TuningValue
// Description   : Tune the PID Value
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.19 13:30:00
// ==============================================================
void CTuneFraView::TuningValue( CAliasItem* pAlias, CSettingItem *pSetting, CEdit* pEdit, float fTuneUnit )
{
	if( pEdit->GetSafeHwnd() == NULL )
	{
		return;
	}

	if( CheckWritePtr( pAlias, sizeof(CAliasItem)) &&
		CheckWritePtr( pSetting, sizeof(CSettingItem)))
	{
		// Check Range
		float fValue = pAlias->valR4();
		fValue += fTuneUnit;
		CString strValue;
		strValue.Format( L"%f", fValue );
		if( pSetting->IsRangeValid( strValue ))
		{
			pAlias->valR4( fValue );
		}
		else
		{
			pSetting->ValidateSettingValue( strValue );
		}

		fValue = pAlias->valR4();

		// Edit Value
		CString strEditValue;
		pEdit->GetWindowTextW( strEditValue );
		float fEditValue = 0.0;
		::swscanf_s( strEditValue, L"%f", &fEditValue );

		if( fabs( fValue-fEditValue) > EPSILON_FLT )       // !=
		{
			// Final value
			if( pAlias == m_pAliasRate )
			{
				strValue.Format( L"%.4f", fValue );
			}
			else
			{
				strValue.Format( L"%.2f", fValue );
			}

			pEdit->SetWindowText( strValue );

			// Communication
			m_pAliases->clear();
			m_pAliases->Add( pAlias );
			((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( m_pAliases.get(), false, true );
		}
	}//if
}

// ==============================================================
// Function name : PreTranslateMessage
// Description   : Click Enter
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.19 13:30:00
// ==============================================================
BOOL CTuneFraView::PreTranslateMessage( MSG* pMsg )
{
	CWnd* pWnd = GetFocus();
	if( pWnd->GetSafeHwnd() == NULL )
	{
		return -1;
	}

	if((GetFocus()->GetDlgCtrlID() != ID_EDIT_PB)    &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_RESET) &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_RATE))
	{
	    return  CConfiguratorView::PreTranslateMessage( pMsg );	
	}

	CAliasItem* pAlias = NULL;
	CEdit* pEdit = NULL;
	CSettingItem *pSetting = NULL;
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))  // Enter Key was down
	{
        if( GetFocus()->GetDlgCtrlID() == ID_EDIT_PB )
		{
            pAlias = m_pAliasPB;
			pSetting = m_pSettingPB;
			pEdit = &m_edtPB;
		}
		else if( GetFocus()->GetDlgCtrlID() == ID_EDIT_RESET )
		{
            pAlias = m_pAliasReset;
			pSetting = m_pSettingReset;
			pEdit = &m_edtReset;
		}
		else //if( GetFocus()->GetDlgCtrlID() == ID_EDIT_RATE )
		{
            pAlias = m_pAliasRate;
			pSetting = m_pSettingRate;
			pEdit = &m_edtRate;
		}

		if((pAlias != NULL) && (pSetting != NULL) && (pEdit->GetSafeHwnd() != NULL))
		{
			// Edit Value
			CString strEditValue;
			pEdit->GetWindowTextW( strEditValue );
			float fEditValue = 0.0;
			::swscanf_s( strEditValue, L"%f", &fEditValue );

			// Check if valid
			if( !pSetting->IsRangeValid( strEditValue ))   
			{
				if( fEditValue > pAlias->valR4())
				{
					strEditValue = pSetting->GetRangeMax();
				}
				else if( fEditValue < pAlias->valR4())
				{
					strEditValue = pSetting->GetRangeMin();
				}

				::swscanf_s( strEditValue, L"%f", &fEditValue );
			}

			if( fabs(pAlias->valR4()-fEditValue) > EPSILON_FLT )  // !=
			{
				float fDiff = std::fabs(fEditValue - pAlias->valR4());
				float fPercent = 1.0f;                     // Default to 100%, to catch values of 0.0
				if( fabs(pAlias->valR4()) > EPSILON_FLT )  //if( pAlias->valR4() != 0.0f )
				{
					fPercent = fDiff / pAlias->valR4();
				}
				
				if( fPercent > 0.1f )
				{
					CString strWarning, strWrite, strCaption;
					strWarning.LoadString( IDS_TUNING_WARNING );
					strWrite.Format( strWarning, pAlias->valR4(), fEditValue, fPercent * 100.0f );
					strCaption.LoadString( IDS_WARNING );
					if( MessageBox( strWrite, strCaption, MB_ICONWARNING | MB_YESNO ) == IDNO )
					{
						CString strOriValue;
						if( pAlias == m_pAliasRate )
						{
							strOriValue.Format( L"%.4f", pAlias->valR4());
						}
						else
						{
							strOriValue.Format( L"%.2f", pAlias->valR4());
						}

						pEdit->SetWindowText( strOriValue );
						return TRUE;
					}
				}
				
				pAlias->valR4( fEditValue );  

				// Communication
				std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
				if( pAliases.get() == NULL )
				{
					return FALSE;
				}

				pAliases->Add( pAlias );
				((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.release(), false, true); 
			}//if !=
			
			if( pAlias == m_pAliasRate )
			{
				strEditValue.Format( L"%.4f", fEditValue );
			}
			else
			{
				strEditValue.Format( L"%.2f", fEditValue );
			}

			pEdit->SetWindowText( strEditValue );
		}//if != NULL
	}//if Enter Key was down

	return   CConfiguratorView::PreTranslateMessage( pMsg );	
}

void CTuneFraView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	return;
}
