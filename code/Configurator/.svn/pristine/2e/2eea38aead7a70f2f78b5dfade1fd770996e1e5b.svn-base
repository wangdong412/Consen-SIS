// CalMinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CalMinDlg.h"
#include "afxdialogex.h"

#include "CalSheet.h"
#include "MainFrm.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"
#include "CalibrateBase.h"

///======================================================
// global

extern std::auto_ptr<modbus::CModbusThread> ModbusThread;
extern std::auto_ptr<CEvent> g_pCalibrateEvent;

///======================================================

// CCalMinDlg 对话框

IMPLEMENT_DYNAMIC(CCalMinDlg, CPropertyPage)

CCalMinDlg::CCalMinDlg()
	: CPropertyPage(CCalMinDlg::IDD)
	, m_pAliasNullOff( NULL )
	, m_pAliasAI01( NULL )
	, m_pAliasAI01V( NULL )
	, m_pAliasAI02( NULL )
	, m_pAliasAI02V( NULL )
	, m_pAliasPV( NULL )
	, m_pAliasActCur( NULL )
	, m_pAliasActCur2( NULL )
	, m_pAliasAI01MinV( NULL )
	, m_pAliasAI02MinV( NULL)
	, m_pAliasNULLCur( NULL )
	, m_pAliasActMin( NULL )
	, m_pAliasActMax( NULL )
	, m_uUpdateCounts( 0 )
	, m_pCalibrate( NULL )
{
	
}

CCalMinDlg::~CCalMinDlg()
{
	m_boldFont.DeleteObject();
	m_norFont.DeleteObject();
	m_imgPassing.ReleaseGDIPlus();
	m_imgPass.ReleaseGDIPlus();
	m_imgUnPass.ReleaseGDIPlus();
}

void CCalMinDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_NULL, m_edtNULL);
	DDX_Control(pDX, IDC_PV_PER, m_stcPV);
	
	DDX_Control(pDX, IDC_AI01P_NAME, m_stcAI01PName);      // AI01 Percent and Voltage
	DDX_Control(pDX, IDC_AI01P_VALUE, m_stcAI01P);
	DDX_Control(pDX, IDC_AI01P_UNIT, m_stcAI01PUnit);
	DDX_Control(pDX, IDC_AI01V_VALUE, m_stcAI01V);
	DDX_Control(pDX, IDC_AI01V_UNIT, m_stcAI01VUnit);

	DDX_Control(pDX, IDC_AI02P_NAME, m_stcAI02PName);      // AI02 Percent and Voltage
	DDX_Control(pDX, IDC_AI02P_VALUE, m_stcAI02P);
	DDX_Control(pDX, IDC_AI02P_UNIT, m_stcAI02PUnit);
	DDX_Control(pDX, IDC_AI02V_VALUE, m_stcAI02V);
	DDX_Control(pDX, IDC_AI02V_UNIT, m_stcAI02VUnit);

	// V1.0
	DDX_Control(pDX, IDC_ACTC_NAME, m_stcACTCName);        // Actuator
	DDX_Control(pDX, IDC_ACT_CUR, m_stcActCur);
	DDX_Control(pDX, IDC_ACTC_UNIT, m_stcACTCUnit);

	// V1.1
	DDX_Control(pDX, IDC_ACT01C_NAME, m_stcACT01CName);    // Actuator 01
	DDX_Control(pDX, IDC_ACT01C_VALUE, m_stcACT01CValue);
	DDX_Control(pDX, IDC_ACT01C_UNIT, m_stcACT01CUnit);

	DDX_Control(pDX, IDC_ACT02C_NAME, m_stcACT02CName);    // Actuator 02
	DDX_Control(pDX, IDC_ACT02C_VALUE, m_stcACT02CValue);
	DDX_Control(pDX, IDC_ACT02C_UNIT, m_stcACT02CUnit);

	DDX_Control(pDX, IDC_AI01MIN_NAME, m_stcAI01MINName);  // AI01 Min
	DDX_Control(pDX, IDE_AI01MIN_VALUE, m_edtAI01MINValue);
	DDX_Control(pDX, IDC_AI01MIN_UNIT, m_stcAI01MINUnit);
	DDX_Control(pDX, IDB_AI01MIN, m_btnAI01MIN);

	DDX_Control(pDX, IDC_AI02MIN_NAME, m_stcAI02MINName);  // AI02 Min
	DDX_Control(pDX, IDE_AI02MIN_VALUE, m_edtAI02MINValue);
	DDX_Control(pDX, IDC_AI02MIN_UNIT, m_stcAI02MINUnit);
	DDX_Control(pDX, IDB_AI02MIN, m_btnAI02MIN);
}


BEGIN_MESSAGE_MAP(CCalMinDlg, CPropertyPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_AI01MIN, &CCalMinDlg::OnBnClickedAi01min)
	ON_BN_CLICKED(IDB_AI02MIN, &CCalMinDlg::OnBnClickedAi02min)
END_MESSAGE_MAP()


// ==============================================================
// Function name: SetVisible
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-21 16:20:00
// ==============================================================
void CCalMinDlg::SetVisible()
{
	/// Version
	if( fabs(theApp.GetXMLVersion()-1.0) <= EPSILON_FLT )  // V1.0, Hide V1.1
	{
		// Actuator 01 Current
		m_stcACT01CName.ShowWindow( SW_HIDE );
		m_stcACT01CValue.ShowWindow( SW_HIDE );
		m_stcACT01CUnit.ShowWindow( SW_HIDE );

		// Actuator 02 Current
		m_stcACT02CName.ShowWindow( SW_HIDE );
		m_stcACT02CValue.ShowWindow( SW_HIDE );
		m_stcACT02CUnit.ShowWindow( SW_HIDE );

		// AI01 Min
		m_stcAI01MINName.ShowWindow( SW_HIDE );
		m_edtAI01MINValue.ShowWindow( SW_HIDE );
		m_stcAI01MINUnit.ShowWindow( SW_HIDE );
		m_btnAI01MIN.ShowWindow( SW_HIDE );

		// AI02 Min
		m_stcAI02MINName.ShowWindow( SW_HIDE );
		m_edtAI02MINValue.ShowWindow( SW_HIDE );
		m_stcAI02MINUnit.ShowWindow( SW_HIDE );
		m_btnAI02MIN.ShowWindow( SW_HIDE );
	}
	else  // V1.1, Hide V1.0
	{
		m_stcACTCName.ShowWindow( SW_HIDE );
	    m_stcActCur.ShowWindow( SW_HIDE );
	    m_stcACTCUnit.ShowWindow( SW_HIDE );
	}


	/// PV Select Algorithm
	int nPVSel = -1;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pFrame->GetSafeHwnd())
	{
		nPVSel = pFrame->GetPVAlgorithm();
	}

	ASSERT( nPVSel != -1 );

	if( CConfiguratorApp::PV_AI01 == nPVSel )     // Hide AI02
	{
		m_stcAI02PName.ShowWindow( SW_HIDE );
	    m_stcAI02P.ShowWindow( SW_HIDE );
	    m_stcAI02PUnit.ShowWindow( SW_HIDE );
	    m_stcAI02V.ShowWindow( SW_HIDE );
	    m_stcAI02VUnit.ShowWindow( SW_HIDE );

		if( fabs(theApp.GetXMLVersion()-1.1) <= EPSILON_FLT ) // V1.1
		{
			m_stcAI02MINName.ShowWindow( SW_HIDE );
			m_edtAI02MINValue.ShowWindow( SW_HIDE );
			m_stcAI02MINUnit.ShowWindow( SW_HIDE );
			m_btnAI02MIN.ShowWindow( SW_HIDE );
		}
	}
	else if( CConfiguratorApp::PV_AI02 == nPVSel ) // Hide AI01
	{
		m_stcAI01PName.ShowWindow( SW_HIDE );
	    m_stcAI01P.ShowWindow( SW_HIDE );
	    m_stcAI01PUnit.ShowWindow( SW_HIDE );
	    m_stcAI01V.ShowWindow( SW_HIDE );
	    m_stcAI01VUnit.ShowWindow( SW_HIDE );

		if( fabs(theApp.GetXMLVersion()-1.1) <= EPSILON_FLT ) // V1.1
		{
			m_stcAI01MINName.ShowWindow( SW_HIDE );
			m_edtAI01MINValue.ShowWindow( SW_HIDE );
			m_stcAI01MINUnit.ShowWindow( SW_HIDE );
			m_btnAI01MIN.ShowWindow( SW_HIDE );
		}
	}
}


// CCalMinDlg 消息处理程序

BOOL CCalMinDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/// String 
	// Use
	CString str;
	str.Format( IDS_CAL_USE, 0 );
	if( GetDlgItem( IDC_STC_USE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_STC_USE )->SetWindowText( str );
	}

	// Once
	str.Format( IDS_CAL_ONCE, 0 );
	if( GetDlgItem( IDC_STC_ONCE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_STC_ONCE )->SetWindowText( str );
	}

	/// Font
	// Create Font
	CClientDC dc(this);
	LOGFONT boldlf;
	memset( &boldlf, 0, sizeof(LOGFONT));
	boldlf.lfHeight = ::MulDiv( 11,-dc.GetDeviceCaps( LOGPIXELSY ), 72 );
	boldlf.lfWeight = 700;     // bold                
	::StrCpyNW( boldlf.lfFaceName, L"Times New Roman", 32 );
    m_boldFont.CreateFontIndirectW( &boldlf );

	LOGFONT norlf;
	memset( &norlf, 0, sizeof(LOGFONT));
	norlf.lfHeight = ::MulDiv( 11,-dc.GetDeviceCaps( LOGPIXELSY ), 72 );
	norlf.lfWeight = 400;     // normal                
	::StrCpyNW( norlf.lfFaceName, L"Times New Roman", 32 );
    m_norFont.CreateFontIndirectW( &norlf );

	// Set Normal Font
	CWnd* pWnd = GetTopWindow();
	while( pWnd->GetSafeHwnd())
	{
		pWnd->SetFont( &m_norFont );
		pWnd = pWnd->GetNextWindow();
	}

	// Set Bold Font
	// Title
	if( GetDlgItem( IDC_CAL_TITLE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_TITLE )->SetFont( &m_boldFont ); 
	}

	// Introduction
	if( GetDlgItem( IDC_CAL_MIN )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_MIN )->SetFont( &m_boldFont ); 
	}

	/// Image
	if( S_OK == m_imgPassing.Load( L"res\\Calibrate\\Cal_Passing.bmp" ))
	{
	    m_imgPassing.SetTransparentColor( RGB(255,0,255));
	}

	if( S_OK == m_imgPass.Load( L"res\\Calibrate\\Cal_Pass.bmp" ))
	{
	    m_imgPass.SetTransparentColor( RGB(255,0,255));
	}

	if( S_OK == m_imgUnPass.Load( L"res\\Calibrate\\Cal_UnPass.bmp" ))
	{
	    m_imgUnPass.SetTransparentColor( RGB(255,0,255));
	}

	/// Alias
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if( pFrame->GetSafeHwnd())
	{
		CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pFrame->GetActiveDocument();
		if( pDoc )
		{
            CAliasMap* pAliases = pDoc->m_aAliases;
			ASSERT( CheckReadPtr(pAliases, sizeof(CAliasMap)));
			if( CheckReadPtr(pAliases, sizeof(CAliasMap)))
			{
				m_pAliasNullOff = pAliases->find_name( L"kNull1" );        // 42161  Null Current Offset
				m_pAliasAI01    = pAliases->find_name( L"iAPI1P" );        // 35001  AI01 Position
				m_pAliasAI01V   = pAliases->find_name( L"iAPI1V" );        // 35011  AI01 Voltage Value
				m_pAliasAI02    = pAliases->find_name( L"iAPI2P" );        // 35003  AI02 Position
				m_pAliasAI02V   = pAliases->find_name( L"iAPI2V" );        // 35013  AI02 Voltage Value
				m_pAliasPV      = pAliases->find_name( L"iPVP" );          // 35081  PV Position
				m_pAliasActCur  = pAliases->find_name( L"iAO1V");          // 35039  Actuator Current

				// V1.1
				m_pAliasActCur2 = pAliases->find_name( L"iAO2V");          // 35041  Actuator 02 Current

				m_pAliasAI01MinV = pAliases->find_name( L"kAI01Min");      // 42053  AI 01 Min Voltage
				m_pAliasAI02MinV = pAliases->find_name( L"kAI02Min");      // 42057  AI 02 Min Voltage

				// Others
				m_pAliasNULLCur = pAliases->find_name( L"iNULLCur");       // 35045  Actual NULL Current
	            m_pAliasActMin = pAliases->find_name( L"kAO1MIN");         // 42147  Actuator Min Current
	            m_pAliasActMax = pAliases->find_name( L"kAO1MAX");         // 42149  Actuator Max Current
			}
		}
	}

	SetVisible();                              // SetVisible according version and PV Select Algorithm

	CCalSheet* pCalSheet = (CCalSheet*)GetParent();
	if( pCalSheet->GetSafeHwnd())
	{
		m_pCalibrate = (CDSC100Calibrate*)pCalSheet->GetCalibrate();
	}

	SetTimer( IDC_CAL_MINTIMER, 100, NULL );   // Timer

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCalMinDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()

	// Check if bitmaps are currently loaded.
	if( m_imgPassing.IsNull() ||
		m_imgPassing.IsNull() || 
		m_imgUnPass.IsNull())
	{
		return;
	}

	// Introduction: Passed
	CRect rect;
	if( GetDlgItem( IDC_CAL_INTRO )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_INTRO )->GetWindowRect( rect );  // screen coordinates 
		ScreenToClient( &rect );
	}
	m_imgPass.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Minimum: Passing
	if( GetDlgItem( IDC_CAL_MIN )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_MIN )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgPassing.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Maximum: Non Pass
	if( GetDlgItem( IDC_CAL_MAX )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_MAX )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgUnPass.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Verification: Non Pass
	if( GetDlgItem( IDC_CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_VER )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgUnPass.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Save: Non Pass
	if( GetDlgItem( IDC_CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_SAVE )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgUnPass.Draw( dc.GetSafeHdc(), 10, rect.top );
}


BOOL CCalMinDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent();
	if( pParent->GetSafeHwnd())
	{
		pParent->SetWizardButtons( PSWIZB_NEXT );
	}

	return CPropertyPage::OnSetActive();
}


LRESULT CCalMinDlg::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer( IDC_CAL_MINTIMER );

	if( m_pCalibrate != NULL )
	{
		m_pCalibrate->SetAliasFlag( false, true, false, false, false,
			                        true, false, false, false );
		m_pCalibrate->SendCalibrationFlags( false );
	}

	return CPropertyPage::OnWizardNext();
}

void CCalMinDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nIDEvent != IDC_CAL_MINTIMER )
	{
        return;  
	}

	if( WaitForSingleObject( g_pCalibrateEvent->m_hObject, 100 ))
	{
		g_pCalibrateEvent->ResetEvent();

		CString str;

		if( m_uUpdateCounts == 0 )
		{
			m_uUpdateCounts++;

			if( CheckReadPtr( m_pAliasNullOff, sizeof( CAliasItem )))   // 42161, Null Current Offset    
			{
				str.Format( L"%.2f", m_pAliasNullOff->remR4());
				m_edtNULL.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasAI01MinV, sizeof( CAliasItem )))   // 42053, AI 01 Min    
			{
				str.Format( L"%.2f", m_pAliasAI01MinV->remR4());
				m_edtAI01MINValue.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasAI02MinV, sizeof( CAliasItem )))   // 42057, AI 02 Min    
			{
				str.Format( L"%.2f", m_pAliasAI02MinV->remR4());
				m_edtAI02MINValue.SetWindowText( str );
			}
		}

		// AI01 Percent and Voltage
		if( CheckReadPtr( m_pAliasAI01, sizeof( CAliasItem )))   //  35001, Percent
        {
			str.Format( L"%.2f", m_pAliasAI01->remR4());
			m_stcAI01P.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasAI01V, sizeof( CAliasItem )))   // 35011, Voltage
        {
			str.Format( L"%.2f", m_pAliasAI01V->remR4());
			m_stcAI01V.SetWindowText( str );
		}

		// AI02 Percent and Voltage
		if( CheckReadPtr( m_pAliasAI02, sizeof( CAliasItem )))   //  35003, Percent
        {
			str.Format( L"%.2f", m_pAliasAI02->remR4());
			m_stcAI02P.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasAI02V, sizeof( CAliasItem )))  // 35013, Voltage
        {
			str.Format( L"%.2f", m_pAliasAI02V->remR4());
			m_stcAI02V.SetWindowText( str );
		}

		// PV Position
		if( CheckReadPtr( m_pAliasPV, sizeof( CAliasItem )))     //  35081   
        {
			str.Format( L"%.2f", m_pAliasPV->remR4());
			m_stcPV.SetWindowText( str );
		}

		// Actuator Current/Actuator 01 Current
		if( CheckReadPtr( m_pAliasActCur, sizeof( CAliasItem ))) // 35039    
        {
			str.Format( L"%.2f", m_pAliasActCur->remR4());
			m_stcActCur.SetWindowText( str );
			m_stcACT01CValue.SetWindowText( str );
		}

		// Actuator 02 Current
		if( CheckReadPtr( m_pAliasActCur2, sizeof( CAliasItem ))) // 35041    
        {
			str.Format( L"%.2f", m_pAliasActCur2->remR4());
			m_stcACT02CValue.SetWindowText( str );
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}

// ==============================================================
// Function name: SetNULLCurOffset
// Description  : When click Enter, Send 42161, Null Current Offset
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 14:00:00
// ==============================================================
bool CCalMinDlg::SetNULLCurOffset()
{
	// 42161, Null Current Offset
	if( !CheckWritePtr( m_pAliasNullOff, sizeof( CAliasItem )))  
	{
		return false;
	}

	// Get Null Current, Minimum Current and Maximum Current
	float fNullCurrent = 0.0;
	float fMinCurrent = 0.0;
	float fMaxCurrent = 0.0;

	if( CheckReadPtr( m_pAliasNULLCur, sizeof( CAliasItem )))  // 35045 Actual Null Current
	{
		fNullCurrent = m_pAliasNULLCur->remR4();
	}

	if( CheckReadPtr( m_pAliasActMin, sizeof( CAliasItem )))   // 42147  Actuator Min Current
	{
		fMinCurrent = m_pAliasActMin->remR4();
	}

	if( CheckReadPtr( m_pAliasActMax, sizeof( CAliasItem )))   // 42149  Actuator Max Current
	{
		fMaxCurrent = m_pAliasActMax->remR4();
	}

	// If  fMinCurrent > fMaxCurrent, switch them
	if( fMinCurrent > fMaxCurrent )
	{
		float fCurrentValue = fMinCurrent;
		fMinCurrent = fMaxCurrent;
		fMaxCurrent =  fCurrentValue;
	}

	CString strValue;
	float fValue = 0.0;
	m_edtNULL.GetWindowText( strValue );
	::swscanf_s( strValue, L"%f", &fValue );

	if((fValue + fNullCurrent) < fMinCurrent )
	{
		fValue = fMinCurrent -  fNullCurrent;
	}
	else if((fValue + fNullCurrent) > fMaxCurrent )
	{
		fValue = fMaxCurrent -  fNullCurrent;
	}

	strValue.Format( L"%.2f", fValue );
	m_edtNULL.SetWindowTextW( strValue );


	m_pAliasNullOff->valR4( fValue );

    return true;
}

// ==============================================================
// Function name: SetAIVoltage
// Description  : When click Enter, Send 42053/42057,  AI 01/02 Min Voltage
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 14:00:00
// ==============================================================
bool CCalMinDlg::SetAIVoltage( CAliasItem *pAlias, CEdit &edtValue )
{
	if((!CheckWritePtr( pAlias, sizeof( CAliasItem ))) ||
		(NULL == edtValue.GetSafeHwnd()))  
	{
		return false;
	}

	CString strValue;
	float fValue = 0.0;
	edtValue.GetWindowText( strValue );
	::swscanf_s( strValue, L"%f", &fValue );

	float fMinValid = -10.0;
    float fMaxValid = 10.0;
    if( fValue < fMinValid )
    {
        fValue = fMinValid;
    }
    else if( fValue > fMaxValid )
    {
        fValue = fMaxValid;
    }

	strValue.Format( L"%.2f", fValue );
	edtValue.SetWindowTextW( strValue );

	pAlias->valR4( fValue );

    return true;
}

// ==============================================================
// Function name: SetPickup
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 17:00:00
// ==============================================================
bool CCalMinDlg::SetPickup( UINT uType )
{
	CAliasItem *pAlias = m_pAliasAI01MinV;
	CStatic *pSrc = &m_stcAI01V;
	CEdit *pDest = &m_edtAI01MINValue;
	
	if( PICKUP_AI02 == uType )
	{
		pAlias = m_pAliasAI02MinV;
		pSrc = &m_stcAI02V;
		pDest = &m_edtAI02MINValue;
	}

	// Check
	if((NULL == pAlias) || (NULL == pSrc->GetSafeHwnd()) || (NULL == pDest->GetSafeHwnd()))
	{
		return false;
	}

	CString s;
	pSrc->GetWindowTextW( s );
	pDest->SetWindowTextW( s );

	pAlias->SetValue( s );

	// Send
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	if( NULL == pAliases.get())
	{
		return false;
	}

	pAliases->Add( pAlias );

	((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true ); 
	pAliases->clear();
	pAliases.reset();

	return true;
}

// ==============================================================
// Function name: PreTranslateMessage
// Description  : When click Enter, Send 42161
// Return type  : 
// Parameters   : 
// Author       : wangdong 2010-03-08 10:20:00
// ==============================================================
BOOL CCalMinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* pWnd = GetFocus();
	if( NULL == pWnd->GetSafeHwnd())
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pWnd->GetDlgCtrlID() != IDC_EDT_NULL) &&
	   (pWnd->GetDlgCtrlID() != IDE_AI01MIN_VALUE) &&
	   (pWnd->GetDlgCtrlID() != IDE_AI02MIN_VALUE))
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))      // Enter Key was down
	{
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
		if( NULL == pAliases.get())
		{
			return CPropertyPage::PreTranslateMessage(pMsg);
		}

		if( IDC_EDT_NULL == pWnd->GetDlgCtrlID())            // 42161, Null Current Offset
		{
			if( SetNULLCurOffset())
			{
				pAliases->Add( m_pAliasNullOff );
			}
		}
		else if( IDE_AI01MIN_VALUE == pWnd->GetDlgCtrlID())  // 42053  AI 01 Min Voltage  
		{
			if( SetAIVoltage( m_pAliasAI01MinV, m_edtAI01MINValue ))
			{
				pAliases->Add( m_pAliasAI01MinV );
			}
		}
		else  // 42057  AI 02 Min Voltage 
		{
			if( SetAIVoltage( m_pAliasAI02MinV, m_edtAI02MINValue ))
			{
				pAliases->Add( m_pAliasAI02MinV );
			}
		}

		// Communication
		if( pAliases->size() > 0 )
		{
			((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true ); 

			pAliases->clear();
			pAliases.reset();
		}

		return TRUE;
	}//if VK_RETURN

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// ==============================================================
// Function name: OnBnClickedAi01min - Pickup AI01
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 17:00:00
// ==============================================================
void CCalMinDlg::OnBnClickedAi01min()
{
	SetPickup( PICKUP_AI01 );
}

// ==============================================================
// Function name: OnBnClickedAi02min - Pickup AI02
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 17:00:00
// ==============================================================
void CCalMinDlg::OnBnClickedAi02min()
{
	SetPickup( PICKUP_AI02 );
}
