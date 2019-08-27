// NonTuningFra.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "NonTuningFra.h"

// Added by wangdong
#include "ConfiguratorDoc.h"
#include "MainFrm.h"

// =============================================================
///Globals
#define ID_STATIC_STATUS      IDC_TUNEFRA_BASE + 90
#define ID_STATIC_HIDE        IDC_TUNEFRA_BASE + 91

#define ID_STATIC_CUR1        IDC_TUNEFRA_BASE + 101
#define ID_STATIC_CUR2        IDC_TUNEFRA_BASE + 102
#define ID_STATIC_CUR12       IDC_TUNEFRA_BASE + 103
#define ID_STATIC_AMP         IDC_TUNEFRA_BASE + 104
#define ID_STATIC_FRE         IDC_TUNEFRA_BASE + 105
#define ID_STATIC_SD          IDC_TUNEFRA_BASE + 106
#define ID_STATIC_CBS         IDC_TUNEFRA_BASE + 107

#define ID_EDIT_CUR1          IDC_TUNEFRA_BASE + 121
#define ID_EDIT_CUR2          IDC_TUNEFRA_BASE + 122
#define ID_EDIT_CUR12         IDC_TUNEFRA_BASE + 123
#define ID_EDIT_AMP           IDC_TUNEFRA_BASE + 124
#define ID_EDIT_FRE           IDC_TUNEFRA_BASE + 125
#define ID_EDIT_SD            IDC_TUNEFRA_BASE + 126
#define ID_EDIT_CBS           IDC_TUNEFRA_BASE + 127

#define ID_STATIC_CURUNIT1    IDC_TUNEFRA_BASE + 141
#define ID_STATIC_CURUNIT2    IDC_TUNEFRA_BASE + 142
#define ID_STATIC_CURUNIT12   IDC_TUNEFRA_BASE + 143
#define ID_STATIC_AMPUNIT     IDC_TUNEFRA_BASE + 144
#define ID_STATIC_FREUNIT     IDC_TUNEFRA_BASE + 145
#define ID_STATIC_SDUNIT      IDC_TUNEFRA_BASE + 146
#define ID_STATIC_CBSUNIT     IDC_TUNEFRA_BASE + 147


// =============================================================

// CNonTuningFra

IMPLEMENT_DYNCREATE(CNonTuningFra, CConfiguratorView)

CNonTuningFra::CNonTuningFra()
             : m_pAliasCurrent1( NULL )
			 , m_pAliasCurrent2( NULL )
			 , m_pAliasCurrent12( NULL )
			 , m_pAliasAmplitude( NULL )
			 , m_pAliasFrequency( NULL )
			 , m_pAliasSD( NULL )
			 , m_pAliasCBS( NULL )
{
}

CNonTuningFra::~CNonTuningFra()
{
}

// =======================================================================
// Function name : SetVisibleFromVersion
// Description   : v1.0 unvisible
//                 v1.1 visible; 
// Return type   : void
// Parameters    :
// Author        : WangDong 2012.08.21 11:00:00
// =======================================================================
void CNonTuningFra::SetVisibleFromVersion()
{
	UINT nVisible = (fabs(theApp.GetXMLVersion()-1.1) <= EPSILON_FLT) ? SW_SHOW : SW_HIDE;   // 1.1
	
	// label
	m_lblNullCurrent2.ShowWindow( nVisible );
	m_lblNullCurrent12.ShowWindow( nVisible );
	m_lblSD.ShowWindow( nVisible );
	m_lblCBS.ShowWindow( nVisible );

	// edit
	m_edtCurrent2.ShowWindow( nVisible );
	m_edtCurrent12.ShowWindow( nVisible );
	m_edtSD.ShowWindow( nVisible );
	m_edtCBS.ShowWindow( nVisible );

	// unit
	m_lblUnitCurrent2.ShowWindow( nVisible );
	m_lblUnitCurrent12.ShowWindow( nVisible );
	m_lblUnitSD.ShowWindow( nVisible );
	m_lblUnitCBS.ShowWindow( nVisible );
}

BEGIN_MESSAGE_MAP(CNonTuningFra, CConfiguratorView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNonTuningFra 绘图

void CNonTuningFra::OnDraw(CDC* pDC)
{
	CConfiguratorDoc* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CNonTuningFra 诊断

#ifdef _DEBUG
void CNonTuningFra::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CNonTuningFra::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNonTuningFra 消息处理程序


int CNonTuningFra::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
		std::vector<CString> LableVector( 8, L"" );
		LableVector[0].LoadStringW( IDS_NONTUNE_OTHER );
		LableVector[1].LoadStringW( IDS_NONTUNE_NULL1 );
		LableVector[2].LoadStringW( IDS_NONTUNE_NULL2 );
		LableVector[3].LoadStringW( IDS_NONTUNE_NULL12 );
		LableVector[4].LoadStringW( IDS_NONTUNE_AMP );
		LableVector[5].LoadStringW( IDS_NONTUNE_FRE );
		LableVector[6].LoadStringW( IDS_NONTUNE_SD );
		LableVector[7].LoadStringW( IDS_NONTUNE_CBS );


		///1. Create
		// Labels
		if((!m_lblStatus.Create(       LableVector[0], WS_CHILD | WS_VISIBLE, CRect( 1, 0,  rect.Width()-2, 17), this, ID_STATIC_STATUS)) ||
		   (!m_lblNullCurrent1.Create( LableVector[1], WS_CHILD | WS_VISIBLE, CRect( 5, 20, 5+225, 20+21),       this, ID_STATIC_CUR1))   ||
		   (!m_lblNullCurrent2.Create( LableVector[2], WS_CHILD | WS_VISIBLE, CRect( 5, 44, 5+225, 44+21),       this, ID_STATIC_CUR2))   ||
		   (!m_lblNullCurrent12.Create(LableVector[3], WS_CHILD | WS_VISIBLE, CRect( 5, 68, 5+225, 68+21),       this, ID_STATIC_CUR12))  ||
		   (!m_lblAmplitude.Create(    LableVector[4], WS_CHILD | WS_VISIBLE, CRect( 5, 92, 5+225, 92+21),       this, ID_STATIC_AMP))    ||
		   (!m_lblFrequency.Create(    LableVector[5], WS_CHILD | WS_VISIBLE, CRect( 5, 116, 5+225, 116+21),     this, ID_STATIC_FRE))    ||
		   (!m_lblSD.Create(           LableVector[6], WS_CHILD | WS_VISIBLE, CRect( 5, 140, 5+225, 140+21),     this, ID_STATIC_FRE))    ||
		   (!m_lblCBS.Create(          LableVector[7], WS_CHILD | WS_VISIBLE, CRect( 5, 164, 5+225, 164+21),      this, ID_STATIC_FRE))   ||
		   (!m_lblHide.Create(         L"",            WS_CHILD | WS_VISIBLE, CRect( 1, 185, rect.Width()-2, 185+1), this, ID_STATIC_HIDE)))
		{
			return -1;
		}

		// Edit
		if((!m_edtCurrent1.CreateEx(   WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 20, 230+92, 20+21 ), this, ID_EDIT_CUR1))  ||
		   (!m_edtCurrent2.CreateEx(   WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 44, 230+92, 44+21 ), this, ID_EDIT_CUR2))  ||
		   (!m_edtCurrent12.CreateEx(  WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 68, 230+92, 68+21 ), this, ID_EDIT_CUR12)) ||
		   (!m_edtAmplitude.CreateEx(  WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 92, 230+92, 92+21 ), this, ID_EDIT_AMP))  ||
		   (!m_edtFrequency.CreateEx(  WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 116, 230+92, 116+21 ), this, ID_EDIT_FRE))||
		   (!m_edtSD.CreateEx(         WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 140, 230+92, 140+21 ), this, ID_EDIT_SD))||
		   (!m_edtCBS.CreateEx(        WS_EX_STATICEDGE, _T("Edit"), _T("0.00"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 230, 164, 230+92, 164+21 ), this, ID_EDIT_CBS)))
		{
			return -1;
		} 

		// Unit
		if((!m_lblUnitCurrent1.Create(  L"mA", WS_CHILD | WS_VISIBLE, CRect( 325, 24, 325+54, 24+13), this, ID_STATIC_CURUNIT1)) ||
		   (!m_lblUnitCurrent2.Create(  L"mA", WS_CHILD | WS_VISIBLE, CRect( 325, 48, 325+54, 48+13), this, ID_STATIC_CURUNIT2)) ||
		   (!m_lblUnitCurrent12.Create( L"mA", WS_CHILD | WS_VISIBLE, CRect( 325, 72, 325+54, 72+13), this, ID_STATIC_CURUNIT12))||
		   (!m_lblUnitAmplitude.Create( L"%",  WS_CHILD | WS_VISIBLE, CRect( 325, 96, 325+54, 96+13), this, ID_STATIC_AMPUNIT))  ||
		   (!m_lblUnitFrequency.Create( L"Hz", WS_CHILD | WS_VISIBLE, CRect( 325, 120, 325+54, 120+13), this, ID_STATIC_FREUNIT))||
		   (!m_lblUnitSD.Create(        L"%",  WS_CHILD | WS_VISIBLE, CRect( 325, 144, 325+54, 144+13), this, ID_STATIC_SDUNIT)) ||
		   (!m_lblUnitCBS.Create(       L"%/Scan", WS_CHILD | WS_VISIBLE, CRect( 325, 168, 325+54, 168+13), this, ID_STATIC_CBSUNIT)))
		{
			return -1;
		}

		SetVisibleFromVersion();
	}
	catch( COleException& e )
	{
		e.ReportError();
		e.Delete();

		return -1;
	}
	catch( ... )
	{
		theApp.LogInfo( L"Unknown exception was throwed in CNonTuningFra::OnCreate!" );

		return -1;
	}

	return 0;
}


BOOL CNonTuningFra::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	return TRUE;
}


void CNonTuningFra::OnSize(UINT nType, int cx, int cy)
{
	CConfiguratorView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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

// =======================================================================
// Function name : BindSetting
// Description   : 
// Return type   : void
// Parameters    :
// Author        : WangDong 2011.04.24 14:00:00
// =======================================================================
void CNonTuningFra::Bind()
{
	if( GetDocument() && GetDocument()->m_aAliases )
	{
		m_pAliasCurrent1 = GetDocument()->m_aAliases->find_name( L"kAONULL1" );
		m_pAliasCurrent2 = GetDocument()->m_aAliases->find_name( L"kAONULL2" );
		m_pAliasCurrent12 = GetDocument()->m_aAliases->find_name( L"kAONULL12" );
		m_pAliasAmplitude = GetDocument()->m_aAliases->find_name( L"kPD" ); 
		m_pAliasFrequency = GetDocument()->m_aAliases->find_name( L"kPF" );
		m_pAliasSD = GetDocument()->m_aAliases->find_name( L"kStaDB" ); 
		m_pAliasCBS = GetDocument()->m_aAliases->find_name( L"kCBStep" );
	}
	else
	{
		m_pAliasCurrent1 = NULL;
		m_pAliasCurrent2 = NULL;
		m_pAliasCurrent12 = NULL;
		m_pAliasAmplitude = NULL;
		m_pAliasFrequency = NULL;
		m_pAliasSD = NULL;
		m_pAliasCBS = NULL;
	}
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
void CNonTuningFra::RefreshTuning( bool bConnected )
{
	try
	{
		/// Initialize values
		CString s;
		float fValue = 0.0F;

		if((m_pAliasCurrent1 != NULL) && (m_edtCurrent1.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasCurrent1->valR4();
			s.Format( L"%.2f", fValue );
			m_edtCurrent1.SetWindowText( s );        // Null Current 01
		}

		if((m_pAliasCurrent2 != NULL) && (m_edtCurrent2.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasCurrent2->valR4();
			s.Format( L"%.2f", fValue );
			m_edtCurrent2.SetWindowText( s );        // Null Current 02
		}

		if((m_pAliasCurrent12 != NULL) && (m_edtCurrent12.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasCurrent12->valR4();
			s.Format( L"%.2f", fValue );
			m_edtCurrent12.SetWindowText( s );       // Null Current 1/2
		}

		if((m_pAliasAmplitude != NULL) && (m_edtAmplitude.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasAmplitude->valR4();
			s.Format( L"%.2f", fValue );
			m_edtAmplitude.SetWindowText( s );       // Amplitude
		}

		if((m_pAliasFrequency != NULL) && (m_edtFrequency.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasFrequency->valR4();
			s.Format( L"%.2f", fValue );
			m_edtFrequency.SetWindowText( s );       // Frequency
		}

		if((m_pAliasSD != NULL) && (m_edtSD.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasSD->valR4();
			s.Format( L"%.2f", fValue );
			m_edtSD.SetWindowText( s );              // Stability DeadBand
		}

		if((m_pAliasCBS != NULL) && (m_edtCBS.GetSafeHwnd() != NULL))
		{
			fValue = m_pAliasCBS->valR4();
			s.Format( L"%.3f", fValue );
			m_edtCBS.SetWindowText( s );             // Current Balance Step
		}

		/// Edits - ReadOnly(Not Enable)
		m_edtCurrent1.SetReadOnly( !bConnected );
		m_edtCurrent2.SetReadOnly( !bConnected );
		m_edtCurrent12.SetReadOnly( !bConnected );
		m_edtAmplitude.SetReadOnly( !bConnected );
		m_edtFrequency.SetReadOnly( !bConnected );
		m_edtSD.SetReadOnly( !bConnected );
		m_edtCBS.SetReadOnly( !bConnected );

		SetVisibleFromVersion();
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CNonTuningFra::RefreshTuning!" );
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CNonTuningFra::RefreshTuning!" );
	}
}

// ==============================================================
// Function name : PreTranslateMessage
// Description   : Click Enter
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.19 13:30:00
// ==============================================================
BOOL CNonTuningFra::PreTranslateMessage( MSG* pMsg )
{
	CWnd* pWnd = GetFocus();
	if( pWnd == NULL )
	{
		return -1;
	}

	if((GetFocus()->GetDlgCtrlID() != ID_EDIT_CUR1)  &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_CUR2)  &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_CUR12) &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_AMP)   &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_FRE)   &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_SD)    &&
	   (GetFocus()->GetDlgCtrlID() != ID_EDIT_CBS))
	{
	    return  CConfiguratorView::PreTranslateMessage( pMsg );	
	}

	
	CAliasItem* pAlias = NULL;
	CEdit* pEdit = NULL;
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))  // Enter Key was down
	{
		bool bConnected = ((CMainFrame*)AfxGetMainWnd())->IsConnected();
		if( !bConnected )
		{
			return CConfiguratorView::PreTranslateMessage( pMsg );
		}

        if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_CUR1))
		{
            pAlias = m_pAliasCurrent1;
			pEdit = &m_edtCurrent1;
		}
		else if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_CUR2))
		{
            pAlias = m_pAliasCurrent2;
			pEdit = &m_edtCurrent2;
		}
		else if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_CUR12))
		{
            pAlias = m_pAliasCurrent12;
			pEdit = &m_edtCurrent12;
		}
		else if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_AMP))
		{
            pAlias = m_pAliasAmplitude;
			pEdit = &m_edtAmplitude;
		}
		else if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_FRE))
		{
            pAlias = m_pAliasFrequency;
			pEdit = &m_edtFrequency;
		}
		else if( GetFocus()->GetSafeHwnd() && (GetFocus()->GetDlgCtrlID() == ID_EDIT_SD))
		{
            pAlias = m_pAliasSD;
			pEdit = &m_edtSD;
		}
		else
		{
            pAlias = m_pAliasCBS;
			pEdit = &m_edtCBS;
		}

		try
		{
			if((pAlias != NULL) && pEdit->GetSafeHwnd())
			{
				CString strValue;
				float fValue = 0.0;
				pEdit->GetWindowText( strValue );
				::swscanf_s( strValue, L"%f", &fValue );

				if( fabs(pAlias->valR4()-fValue) > EPSILON_FLT )   //if( pAlias->valR4() != fValue )
				{
					if((pAlias == m_pAliasCurrent1) ||   // 42159, AO 01 NULL CURRENT
					   (pAlias == m_pAliasCurrent2) ||   // 42163, AO 02 NULL CURRENT
					   (pAlias == m_pAliasCurrent12))    // 42165, AO 03 NULL CURRENT
					{
						CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
						if( pFrame->GetSafeHwnd())
						{
							CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pFrame->GetActiveDocument();
							if( pDoc )
							{
								// NULL Current(42159): [Minimum Current(42147), Maximum Current(42149)]
								CAliasItem* pAO1MinAlias = pDoc->m_aAliases->find_name( L"kAO1MIN" );
								CAliasItem* pAO1MaxAlias = pDoc->m_aAliases->find_name( L"kAO1MAX" );
								ASSERT( pAO1MinAlias && pAO1MaxAlias );
								if( !(pAO1MinAlias && pAO1MaxAlias))
								{
									return TRUE;
								}

								float fMinCurrent  = pAO1MinAlias->valR4();
								float fMaxCurrent  = pAO1MaxAlias->valR4();
								if( fMinCurrent > fMaxCurrent )
								{
									float fCurrent = fMinCurrent;
									fMinCurrent = fMaxCurrent;
									fMaxCurrent = fCurrent;
								}

								if( fValue < fMinCurrent )   
								{
									fValue = fMinCurrent;							
								}
								else if( fValue > fMaxCurrent )
								{
									fValue = fMaxCurrent;          
								}
							}//if( pDoc )
						}//if( pFrame->GetSafeHwnd())
					}//if 42159
					else if( pAlias == m_pAliasAmplitude )   // 42031
					{
						if( fValue < 0.0F )
						{
							fValue = 0.0F;
						}
						else if( fValue > 10.0F )
						{
							fValue = 10.0F;
						}

					}//if 42031 Amplitude
					else if( pAlias == m_pAliasFrequency )   // 42047
					{
						if( fValue < 0.0F )
						{
							fValue = 0.0F;
						}
						else if( fValue > 100.0F )
						{
							fValue = 100.0F;
						}
					}
					else if( pAlias == m_pAliasSD )         // 42049, Stability DeadBand
					{
						if( fValue < 0.1F )
						{
							fValue = 0.1F;
						}
						else if( fValue > 2.0F )
						{
							fValue = 2.0F;
						}
					}
					else if( pAlias == m_pAliasCBS )         // 42051, Current Balance Step
					{
						if( fValue < 0.0F )
						{
							fValue = 0.0F;
						}
						else if( fValue > 0.5F )
						{
							fValue = 0.5F;
						}
					}

					pAlias->valR4( fValue ); 

					// Communication
					std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
					if( !pAliases.get())
					{
						return FALSE;
					}

					pAliases->Add( pAlias );
					((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get()); 

					pAliases->clear();
					pAliases.reset();
				}//if( pAlias->valR4() != static_cast<float>(fValue))
				
				RefreshTuning( bConnected );
			}//if( pAlias && pEdit )
		}//try
		catch( CException &e )
		{
			e.ReportError();
			e.Delete();

			theApp.LogInfo( L"Exception occured in CNonTuningFra::PreTranslateMessage!" );
		}
		catch(...)
		{
			theApp.LogInfo( L"Unknown exception occured in CNonTuningFra::PreTranslateMessage!" );
		}
	}//if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))

	return   CConfiguratorView::PreTranslateMessage( pMsg );	
}


void CNonTuningFra::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	return;
}
