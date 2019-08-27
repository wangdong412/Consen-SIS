// CalVerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CalVerDlg.h"
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


// CCalVerDlg 对话框

IMPLEMENT_DYNAMIC(CCalVerDlg, CPropertyPage)

CCalVerDlg::CCalVerDlg()
	: CPropertyPage(CCalVerDlg::IDD)
	, m_uUpdateCounts( 0 )
	, m_pAliasDemand( NULL )
	, m_pAliasAI01( NULL )
	, m_pAliasAI01V( NULL )
	, m_pAliasAI02( NULL )
	, m_pAliasAI02V( NULL )
	, m_pAliasPV( NULL )
	, m_pAliasActCur( NULL )
	, m_pAliasActNull( NULL )
	, m_pAliasActCur2( NULL )
	, m_pAliasACT02NULL( NULL )
	, m_pAliasACT12NULL( NULL )
	, m_pAliasActMin( NULL )
	, m_pAliasActMax( NULL )
    , m_pCalibrate( NULL )
{
	
}

CCalVerDlg::~CCalVerDlg()
{
	m_boldFont.DeleteObject();
	m_norFont.DeleteObject();
	m_imgPassing.ReleaseGDIPlus();
	m_imgPass.ReleaseGDIPlus();
	m_imgUnPass.ReleaseGDIPlus();
}

void CCalVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_DEMAND, m_edtDemand);       // Demand(%)

	DDX_Control(pDX, IDC_AI01P_NAME, m_stcAI01PName);    // AI01 Percent and Voltage
	DDX_Control(pDX, IDC_AI01P_VALUE, m_stcAI01P);
	DDX_Control(pDX, IDC_AI01P_UNIT, m_stcAI01PUnit);
	DDX_Control(pDX, IDC_AI01V_VALUE, m_stcAI01V);
	DDX_Control(pDX, IDC_AI01V_UNIT, m_stcAI01VUnit);

	DDX_Control(pDX, IDC_AI02P_NAME, m_stcAI02PName);    // AI02 Percent and Voltage
	DDX_Control(pDX, IDC_AI02P_VALUE, m_stcAI02P);
	DDX_Control(pDX, IDC_AI02P_UNIT, m_stcAI02PUnit);
	DDX_Control(pDX, IDC_AI02V_VALUE, m_stcAI02V);
	DDX_Control(pDX, IDC_AI02V_UNIT, m_stcAI02VUnit);

	DDX_Control(pDX, IDC_PV_PER, m_stcPV);               // PV Position

	// V1.0
	DDX_Control(pDX, IDC_ACTC_NAME, m_stcACTCName);       // Actuator Current
	DDX_Control(pDX, IDC_ACT_CUR, m_stcActCur);
	DDX_Control(pDX, IDC_ACTC_UNIT, m_stcACTCUnit);

	DDX_Control(pDX, IDC_ACTNULL_NAME, m_stcACTNULLName); // Actuator NULL Current
	DDX_Control(pDX, IDC_EDT_ACTUATOR, m_stcActNull);
	DDX_Control(pDX, IDC_ACTNULL_UNIT, m_stcACTNULLUnit);

	// V1.1
	DDX_Control(pDX, IDC_ACT01C_NAME, m_stcACT01CName);    // Actuator 01
	DDX_Control(pDX, IDC_ACT01C_VALUE, m_stcACT01CValue);
	DDX_Control(pDX, IDC_ACT01C_UNIT, m_stcACT01CUnit);

	DDX_Control(pDX, IDC_ACT02C_NAME, m_stcACT02CName);    // Actuator 02
	DDX_Control(pDX, IDC_ACT02C_VALUE, m_stcACT02CValue);
	DDX_Control(pDX, IDC_ACT02C_UNIT, m_stcACT02CUnit);

	DDX_Control(pDX, IDC_AI01NC_NAME, m_stcACTNULL01Name); // Actuator NULL 01 Current
	DDX_Control(pDX, IDE_AI01NC_VALUE, m_edtActNull01);
	DDX_Control(pDX, IDC_AI01NC_UNIT, m_stcACTNULL01Unit);

	DDX_Control(pDX, IDC_AI02NC_NAME, m_stcACTNULL02Name); // Actuator NULL 02 Current
	DDX_Control(pDX, IDE_AI02NC_VALUE, m_edtActNull02);
	DDX_Control(pDX, IDC_AI02NC_UNIT, m_stcACTNULL02Unit);

	DDX_Control(pDX, IDC_AI12NC_NAME, m_stcACTNULL12Name); // Actuator NULL 01/02 Current
	DDX_Control(pDX, IDE_AI12NC_VALUE, m_edtActNull12);
	DDX_Control(pDX, IDC_AI12NC_UNIT, m_stcACTNULL12Unit);
}


BEGIN_MESSAGE_MAP(CCalVerDlg, CPropertyPage)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// ==============================================================
// Function name: SetVisible
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-23 09:20:00
// ==============================================================
void CCalVerDlg::SetVisible()
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

		// Actuator 01 NULL Current
		m_stcACTNULL01Name.ShowWindow( SW_HIDE );
		m_edtActNull01.ShowWindow( SW_HIDE );
		m_stcACTNULL01Unit.ShowWindow( SW_HIDE );

		// Actuator 02 NULL Current
		m_stcACTNULL02Name.ShowWindow( SW_HIDE );
		m_edtActNull02.ShowWindow( SW_HIDE );
		m_stcACTNULL02Unit.ShowWindow( SW_HIDE );

		// Actuator 01/02 NULL Current
		m_stcACTNULL12Name.ShowWindow( SW_HIDE );
		m_edtActNull12.ShowWindow( SW_HIDE );
		m_stcACTNULL12Unit.ShowWindow( SW_HIDE );
	}
	else  // V1.1, Hide V1.0
	{
		// Actuator Current
		m_stcACTCName.ShowWindow( SW_HIDE );
	    m_stcActCur.ShowWindow( SW_HIDE );
	    m_stcACTCUnit.ShowWindow( SW_HIDE );

		// Actuator Null Current
		m_stcACTNULLName.ShowWindow( SW_HIDE );
		m_stcActNull.ShowWindow( SW_HIDE );
		m_stcACTNULLUnit.ShowWindow( SW_HIDE );
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
	}
	else if( CConfiguratorApp::PV_AI02 == nPVSel ) // Hide AI01
	{
		m_stcAI01PName.ShowWindow( SW_HIDE );
	    m_stcAI01P.ShowWindow( SW_HIDE );
	    m_stcAI01PUnit.ShowWindow( SW_HIDE );
	    m_stcAI01V.ShowWindow( SW_HIDE );
	    m_stcAI01VUnit.ShowWindow( SW_HIDE );
	}
}

// CCalVerDlg 消息处理程序


BOOL CCalVerDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/// String 
	// Use
	CString str;
	str.LoadString( IDS_VERFICATION );
	if( GetDlgItem( IDC_STC_USE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_STC_USE )->SetWindowText( str );
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
	if( GetDlgItem( IDC_CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_VER )->SetFont( &m_boldFont ); 
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
				m_pAliasDemand = pAliases->find_name( L"kDemand" );        // 42123  Demand
				
				m_pAliasAI01    = pAliases->find_name( L"iAPI1P" );        // 35001  AI01 Position
				m_pAliasAI01V   = pAliases->find_name( L"iAPI1V" );        // 35011  AI01 Value

				m_pAliasAI02    = pAliases->find_name( L"iAPI2P" );        // 35003  AI02 Position
				m_pAliasAI02V   = pAliases->find_name( L"iAPI2V" );        // 35013  AI02 Value

				m_pAliasPV      = pAliases->find_name( L"iPVP" );          // 35081  PV Position

				// V1.0
				m_pAliasActCur  = pAliases->find_name( L"iAO1V");          // 35039  Actuator Current
				m_pAliasActNull = pAliases->find_name( L"kAONULL1" );      // 42159  Actuator Null Current

				// V1.1
				m_pAliasActCur2 = pAliases->find_name( L"iAO2V");          // 35041  Actuator 02 Current
				m_pAliasACT02NULL = pAliases->find_name( L"kAONULL2");     // 42163  Actuator 02 NULL Current
				m_pAliasACT12NULL = pAliases->find_name( L"kAONULL12");    // 42165  Actuator 01/02 NULL Current

				// Others
				m_pAliasActMin = pAliases->find_name( L"kAO1MIN" );        // 42147
				m_pAliasActMax = pAliases->find_name( L"kAO1MAX" );        // 42149
			}
		}
	}

	SetVisible();

	CCalSheet* pCalSheet = (CCalSheet*)GetParent();
	if( pCalSheet->GetSafeHwnd())
	{
		m_pCalibrate = (CDSC100Calibrate*)pCalSheet->GetCalibrate();
	}

	SetTimer( IDC_CAL_VERTIMER, 100, NULL ); // Timer

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCalVerDlg::OnPaint()
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

	// Minimum: Passed
	if( GetDlgItem( IDC_CAL_MIN )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_MIN )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgPass.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Maximum: Passed
	if( GetDlgItem( IDC_CAL_MAX )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_MAX )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgPass.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Verification: Non Pass
	if( GetDlgItem( IDC_CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_VER )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgPassing.Draw( dc.GetSafeHdc(), 10, rect.top );

	// Save: Non Pass
	if( GetDlgItem( IDC_CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_CAL_SAVE )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_imgUnPass.Draw( dc.GetSafeHdc(), 10, rect.top );
}


BOOL CCalVerDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent();
	if( pParent->GetSafeHwnd())
	{
		pParent->SetWizardButtons( PSWIZB_NEXT );
	}

	return CPropertyPage::OnSetActive();
}


LRESULT CCalVerDlg::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer( IDC_CAL_VERTIMER );

	if( m_pCalibrate != NULL )
	{
		m_pCalibrate->SetAliasFlag( false, false, false, true, false,
			                        true, false, false, false );
		m_pCalibrate->SendCalibrationFlags( false );
	}

	return CPropertyPage::OnWizardNext();
}


void CCalVerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nIDEvent != IDC_CAL_VERTIMER )
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

			if( CheckReadPtr( m_pAliasDemand, sizeof( CAliasItem )))    //  42123   Demand
			{
				str.Format( L"%.2f", m_pAliasDemand->remR4());
				m_edtDemand.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasActNull, sizeof( CAliasItem )))   //  42159   Actuator NULL Current
			{
				str.Format( L"%.2f", m_pAliasActNull->remR4());
				m_stcActNull.SetWindowText( str );
				m_edtActNull01.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasACT02NULL, sizeof( CAliasItem ))) //  42163   Actuator 02 NULL Current
			{
				str.Format( L"%.2f", m_pAliasACT02NULL->remR4());
				m_edtActNull02.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasACT12NULL, sizeof( CAliasItem )))  //  42165  Actuator 01/02 NULL Current
			{
				str.Format( L"%.2f", m_pAliasACT12NULL->remR4());
				m_edtActNull12.SetWindowText( str );
			}
		}

		if( CheckReadPtr( m_pAliasAI01, sizeof( CAliasItem )))   //  35001    AI01 Position
        {
			str.Format( L"%.2f", m_pAliasAI01->remR4());
			m_stcAI01P.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasAI02, sizeof( CAliasItem )))   //  35003    AI02 Position
        {
			str.Format( L"%.2f", m_pAliasAI02->remR4());
			m_stcAI02P.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasAI01V, sizeof( CAliasItem )))   // 35011   AI01 Value
        {
			str.Format( L"%.2f", m_pAliasAI01V->remR4());
			m_stcAI01V.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasAI02V, sizeof( CAliasItem )))  // 35013   AI02 Value
        {
			str.Format( L"%.2f", m_pAliasAI02V->remR4());
			m_stcAI02V.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPV, sizeof( CAliasItem )))     //  35081   PV Position
        {
			str.Format( L"%.2f", m_pAliasPV->remR4());
			m_stcPV.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasActCur, sizeof( CAliasItem ))) // 35039    Actuator Current
        {
			str.Format( L"%.2f", m_pAliasActCur->remR4());
			m_stcActCur.SetWindowText( str );
			m_stcACT01CValue.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasActCur2, sizeof( CAliasItem ))) // 35041   Actuator 02 Current
        {
			str.Format( L"%.2f", m_pAliasActCur2->remR4());
			m_stcACT02CValue.SetWindowText( str );
		}
	}

	CPropertyPage::OnTimer(nIDEvent);
}

// ==============================================================
// Function name: SetDemand
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-23 10:00:00
// ==============================================================
bool CCalVerDlg::SetDemand()
{
	if((!CheckWritePtr( m_pAliasDemand, sizeof( CAliasItem ))) ||
		(NULL == m_edtDemand.GetSafeHwnd()))  
	{
		return false;
	}

	CString strValue;
	float fValue = 0.0;
	m_edtDemand.GetWindowText( strValue );
	::swscanf_s( strValue, L"%f", &fValue );

	// Demand Value: 0.0 - 100.0
	float fMinValid = 0.0;
    float fMaxValid = 100.0;
    if( fValue < fMinValid )
    {
        fValue = fMinValid;
    }
    else if( fValue > fMaxValid )
    {
        fValue = fMaxValid;
    }

	strValue.Format( L"%.2f", fValue );
	m_edtDemand.SetWindowTextW( strValue );

	m_pAliasDemand->valR4( fValue );

	// Send
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	if( NULL == pAliases.get())
	{
		return false;
	}

	pAliases->Add( m_pAliasDemand );

	((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true ); 
	pAliases->clear();
	pAliases.reset();

    return true;
}

// ==============================================================
// Function name: SetNullCurrent
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2012-08-22 10:00:00
// ==============================================================
bool CCalVerDlg::SetNullCurrent( CAliasItem *pAlias, CEdit &edtValue )
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

	// NULL Current Range: [Minimum Current(42147), Maximum Current(42149)]
	float fMinCurrent = 0.0;
	float fMaxCurrent = 0.0;
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

	if( fValue < fMinCurrent )   
	{
		fValue = fMinCurrent;       // Min
	}
	else if( fValue > fMaxCurrent )
	{
		fValue = fMaxCurrent;       // Max
	}

	strValue.Format( L"%.2f", fValue );
	edtValue.SetWindowTextW( strValue );

	pAlias->valR4( fValue );


	// Send
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	if( !pAliases.get())
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
// Author       : 
//                wangdong 2010-03-08 10:20:00
// ==============================================================
BOOL CCalVerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* pWnd = GetFocus();
	if( NULL == pWnd->GetSafeHwnd())
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pWnd->GetDlgCtrlID() != IDC_EDT_DEMAND)   &&   // Demand
	   (pWnd->GetDlgCtrlID() != IDC_EDT_ACTUATOR) &&   // Actuator NULL Current
	   (pWnd->GetDlgCtrlID() != IDE_AI01NC_VALUE) &&   // Actuator 01 NULL Current
	   (pWnd->GetDlgCtrlID() != IDE_AI02NC_VALUE) &&   // Actuator 02 NULL Current
	   (pWnd->GetDlgCtrlID() != IDE_AI12NC_VALUE))     // Actuator 1/2 NULL Current
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))      // Enter Key was down
	{
		if( IDC_EDT_DEMAND == pWnd->GetDlgCtrlID())           // Demand   
		{
			SetDemand();
		}
		else if( IDC_EDT_ACTUATOR == pWnd->GetDlgCtrlID())    // Actuator Null Current   
		{
			SetNullCurrent( m_pAliasActNull, m_stcActNull );
		}
		else if( IDE_AI01NC_VALUE == pWnd->GetDlgCtrlID())    // Actuator 01 Null Current   
		{
			SetNullCurrent( m_pAliasActNull, m_edtActNull01 );
		}
		else if( IDE_AI02NC_VALUE == pWnd->GetDlgCtrlID())    // Actuator 02 Null Current  
		{
			SetNullCurrent( m_pAliasACT02NULL, m_edtActNull02 );
		}
		else                                                  // Actuator 1/2 Null Current  
		{
			SetNullCurrent( m_pAliasACT12NULL, m_edtActNull12 );
		}

		return TRUE;
	}//if VK_RETURN

	return CPropertyPage::PreTranslateMessage(pMsg);
}
