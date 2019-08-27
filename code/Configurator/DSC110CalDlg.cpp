// DSC110CalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "DSC110CalDlg.h"
#include "afxdialogex.h"

#include "BasePage.h"
#include "CalibrateBase.h"
#include "CalSheet.h"
#include "MainFrm.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"

///======================================================
// global
extern std::auto_ptr<modbus::CModbusThread> ModbusThread;
extern std::auto_ptr<CEvent> g_pCalibrateEvent;
///======================================================

// CDSC110CalDlg 对话框

IMPLEMENT_DYNAMIC(CDSC110CalDlg, CPropertyPage)

CDSC110CalDlg::CDSC110CalDlg()
	: CPropertyPage(CDSC110CalDlg::IDD)
	, m_pBaseCtl( new CBasePageCtl())
	, m_pCalibrate( NULL )
	, m_pAliasPWMD( NULL )
	, m_pAliasPWMC( NULL )
	, m_pAliasPV01Off( NULL )
	, m_pAliasPV02Off( NULL )
	, m_pAliasPV01OffS( NULL )
	, m_pAliasPV02OffS( NULL )
	, m_pMinDA( NULL )
	, m_pMaxDA( NULL )
	, m_pCalThread( NULL )
	, m_bExitThread( false )
	, m_uUpdateCounts( 0 )
{

}

CDSC110CalDlg::~CDSC110CalDlg()
{
	delete m_pBaseCtl;
	m_pBaseCtl = NULL;
}

void CDSC110CalDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_PWMD, m_edtPWMD);
	DDX_Control(pDX, IDC_PWMC_VALUE, m_stcPWMC);
	DDX_Control(pDX, IDC_PV01OFF_VALUE, m_stcPV01);
	DDX_Control(pDX, IDC_PV02OFF_VALUE, m_stcPV02);
	DDX_Control(pDX, IDE_PV01FF_VALUE, m_edtPV01);
	DDX_Control(pDX, IDE_PV02FF_VALUE, m_edtPV02);
}


BEGIN_MESSAGE_MAP(CDSC110CalDlg, CPropertyPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDB_50P, &CDSC110CalDlg::OnBnClicked50P)
	ON_BN_CLICKED(IDB_PV01FF, &CDSC110CalDlg::OnBnClickedPv01ff)
	ON_BN_CLICKED(IDB_PV02FF, &CDSC110CalDlg::OnBnClickedPv02ff)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDSC110CalDlg 消息处理程序
BOOL CDSC110CalDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pBaseCtl->Init( this );

	/// String 
	// Use
	CString str;
	str.LoadString( IDS_DSC110CAL_CALINFO );
	if( GetDlgItem( IDC_DSC110CAL_INFO )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_INFO )->SetWindowText( str );
	}

	/// Font
	// Set Normal Font
	CWnd* pWnd = GetTopWindow();
	while( pWnd->GetSafeHwnd())
	{
		pWnd->SetFont( &m_pBaseCtl->GetNormalFont());
		pWnd = pWnd->GetNextWindow();
	}

	// Set Bold Font
	// Title
	if( GetDlgItem( IDC_DSC110CAL_TITLE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_TITLE )->SetFont( &m_pBaseCtl->GetBoldFont()); 
	}

	// Cal
	if( GetDlgItem( IDC_DSC110CAL_CAL )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_CAL )->SetFont( &m_pBaseCtl->GetBoldFont()); 
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
				m_pAliasPWMD     = pAliases->find_name( L"kPWMD" );     // 42031 PWM D

				m_pAliasPWMC     = pAliases->find_name( L"iPWMCycle" ); // 35057 PWM Cycle
				m_pAliasPV01Off  = pAliases->find_name( L"iPV01ZOF" );  // 35051 PV01 Zero-Offset Angle
				m_pAliasPV02Off  = pAliases->find_name( L"iPV02ZOF" );  // 35053 PV02 Zero-Offset Angle

				m_pAliasPV01OffS = pAliases->find_name( L"kPV01ZOF" );  // 42089 PV01 Zero-Offset Angle Position
				m_pAliasPV02OffS = pAliases->find_name( L"kPV02ZOF" );  // 42091 PV02 Zero-Offset Angle Position

				m_pMinDA = pAliases->find_name( L"kMinDA" );            // 42095: Min Duty Cycle
				m_pMaxDA = pAliases->find_name( L"kMaxDA" );            // 42097: Max Duty Cycle
			}
		}
	}

	// Calibrate control
	CCalSheet* pCalSheet = (CCalSheet*)GetParent();
	if( pCalSheet->GetSafeHwnd())
	{
		m_pCalibrate = (CDSC110Calibrate*)pCalSheet->GetCalibrate();
	}

	m_edtPV01.SetReadOnly();
	m_edtPV02.SetReadOnly();

	// PWM D: Set 50%
	m_pAliasPWMD->valR4( 50.0 );
	m_edtPWMD.SetWindowTextW( L"50.000" );
	SetPWMD( true );

	// start verification thread
	m_pCalThread = AfxBeginThread( CalThread, this );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDSC110CalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()

	// Check if bitmaps are currently loaded.
	if( m_pBaseCtl->GetPassImage().IsNull() ||
		m_pBaseCtl->GetPassingImage().IsNull() || 
		m_pBaseCtl->GetUnPassImage().IsNull())
	{
		return;
	}

	// Calibrate Angle: Passing
	CRect rect;
	if( GetDlgItem( IDC_DSC110CAL_CAL )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_CAL )->GetWindowRect( rect );  // screen coordinates 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetPassingImage().Draw( dc.GetSafeHdc(), 10, rect.top );

	// Verfication: UnPass
	if( GetDlgItem( IDC_DSC110CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_VER )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetUnPassImage().Draw( dc.GetSafeHdc(), 10, rect.top );

	// Save and Run: UnPass
	if( GetDlgItem( IDC_DSC1110CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC1110CAL_SAVE )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetUnPassImage().Draw( dc.GetSafeHdc(), 10, rect.top );
}


BOOL CDSC110CalDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent();
	if( pParent->GetSafeHwnd())
	{
		pParent->SetWizardButtons( PSWIZB_NEXT );
	}

	return CPropertyPage::OnSetActive();
}


LRESULT CDSC110CalDlg::OnWizardNext()
{
	// If calibrate thread was active, exit it
	if((m_pCalThread != NULL) && !m_bExitThread )
	{
		m_bExitThread = true;

		if( WaitForSingleObject( m_pCalThread->m_hThread, 150 ) != WAIT_OBJECT_0 )
		{
			theApp.LogInfo( L"Terminate cal thread!" );
			::TerminateThread( m_pCalThread->m_hThread, 0 );
		}
	}

	if( m_pCalibrate != NULL )
	{
		m_pCalibrate->SetAliasFlag( false, true, false, false );
	}

	return CPropertyPage::OnWizardNext();
}

void CDSC110CalDlg::OnCancel()
{
	// If calibrate thread was active, exit it
	if((m_pCalThread != NULL) && !m_bExitThread )
	{
		m_bExitThread = true;

		if( WaitForSingleObject( m_pCalThread->m_hThread, 150 ) != WAIT_OBJECT_0 )
		{
			theApp.LogInfo( L"Terminate cal thread!" );
			::TerminateThread( m_pCalThread->m_hThread, 0 );
		}
	}

	if(((CPropertySheet*)GetParent())->GetActivePage() == this )
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if( pFrame->GetSafeHwnd() && pFrame->IsConnected())
		{
			// Send Cancel Flag
			if( m_pCalibrate != NULL )
			{
				m_pCalibrate->SetAliasFlag( false, false, false, true );
			}

			// Stop Monitor if not monitor before calibrate
			if( !m_pCalibrate->IsOrigMonitoring())
			{
				WaitForSingleObject( ModbusThread->GetEvent()->m_hObject, 500 );
				::Sleep( 500 );
				pFrame->OnMonitorStop();
			}
		}
	}

	CPropertyPage::OnCancel();
}

// ==============================================================
// Function name: PreTranslateMessage
// Description  : When click Enter, Send 42033
// Return type  : 
// Parameters   : 
// Author       : 
//                wangdong 2012-02-20 16:20:00
// ==============================================================
BOOL CDSC110CalDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* pWnd = GetFocus();
	if( NULL == pWnd->GetSafeHwnd())
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if( pWnd->GetDlgCtrlID() != IDC_EDT_PWMD )           // Angle Position Settings
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))      // Enter Key was down
	{
		SetPWMD();

		return TRUE;
	}//if VK_RETURN

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// ==============================================================
// Function name: SetPWMD
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-20 17:00:00
// ==============================================================
bool CDSC110CalDlg::SetPWMD( bool bInit )
{
	if((!CheckWritePtr( m_pAliasPWMD, sizeof( CAliasItem ))) ||
		(NULL == m_edtPWMD.GetSafeHwnd()))  
	{
		return false;
	}

	if( !bInit )
	{
		CString strValue;
		float fValue = 0.0;
		m_edtPWMD.GetWindowText( strValue );
		::swscanf_s( strValue, L"%f", &fValue );

		// PWM D: Min(42095) - Max(42097)
		float fMinValid = m_pMinDA->remR4();
		float fMaxValid = m_pMaxDA->remR4();
		if( fValue < fMinValid )
		{
			fValue = fMinValid;
		}
		else if( fValue > fMaxValid )
		{
			fValue = fMaxValid;
		}

		strValue.Format( L"%.3f", fValue );
		m_edtPWMD.SetWindowTextW( strValue );

		m_pAliasPWMD->valR4( fValue );
	}

	// Send
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	if( NULL == pAliases.get())
	{
		return false;
	}

	pAliases->Add( m_pAliasPWMD );

	((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true ); 
	pAliases->clear();
	pAliases.reset();

    return true;
}


/***********************************************************/
// Communications
/***********************************************************/
UINT CalThread( LPVOID pParam )
{
	CDSC110CalDlg* pCalPage = (CDSC110CalDlg*)pParam;
	if( pCalPage->GetSafeHwnd())
	{
		while( !pCalPage->m_bExitThread )
		{
			pCalPage->Update();
			//::Sleep( 100 );     // Only for debug
		}
	}

	TRACE0( "CalThread Exit\n" );
	return 0;
}


// ==============================================================
// Function name: Update
// Description  : Update values
//                R/W only update once; 
//                Read only: update when monitor cycle finish
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-21 10:00:00
// ==============================================================
void CDSC110CalDlg::Update()
{
	if((!m_bExitThread) && (WaitForSingleObject( g_pCalibrateEvent->m_hObject, 50 )==WAIT_OBJECT_0))
	{
		g_pCalibrateEvent->ResetEvent();

		CString str;
		if( m_uUpdateCounts == 0 )
		{
			m_uUpdateCounts++;

			if( CheckReadPtr( m_pAliasPV01OffS, sizeof(CAliasItem))) // 42089 
			{
				str.Format( L"%.3f", m_pAliasPV01OffS->remR4());
				m_edtPV01.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasPV02OffS, sizeof(CAliasItem))) // 42091 
			{
				str.Format( L"%.3f", m_pAliasPV02OffS->remR4());
				m_edtPV02.SetWindowText( str );
			}
		}

		if( CheckReadPtr( m_pAliasPWMC, sizeof( CAliasItem )))     // 35057 PWM Cycle
		{
			str.Format( L"%.3f", m_pAliasPWMC->remR4());
			m_stcPWMC.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPV01Off, sizeof( CAliasItem )))  // 35051 PV01 Zero-Offset Angle
		{
			str.Format( L"%.3f", m_pAliasPV01Off->remR4());
			m_stcPV01.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPV02Off, sizeof( CAliasItem )))  // 35053 PV02 Zero-Offset Angle
		{
			str.Format( L"%.3f", m_pAliasPV02Off->remR4());
			m_stcPV02.SetWindowText( str );
		}
	}
}


// ==============================================================
// Function name: OnBnClicked50P
// Description  : PWM D: Set 50%
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-21 10:00:00
// ==============================================================
void CDSC110CalDlg::OnBnClicked50P()
{
	// PWM D: Set 50%
	m_edtPWMD.SetWindowTextW( L"50.0" );
	SetPWMD();
}

// ==============================================================
// Function name: OnBnClickedPv01/02ff
// Description  : Pickup 01/02
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-21 10:00:00
// ==============================================================
void CDSC110CalDlg::OnBnClickedPv01ff()
{
	SetPickup( PICKUP_PV01 );
}


void CDSC110CalDlg::OnBnClickedPv02ff()
{
	SetPickup( PICKUP_PV02 );
}

bool CDSC110CalDlg::SetPickup( UINT uType )
{
	CAliasItem *pAlias = m_pAliasPV01OffS;  // 42089
	CStatic *pSrc = &m_stcPV01;
	CEdit *pDest = &m_edtPV01;
	
	if( PICKUP_PV02 == uType )
	{
		pAlias = m_pAliasPV02OffS;  // 42091
		pSrc = &m_stcPV02;
		pDest = &m_edtPV02;
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


void CDSC110CalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CPropertyPage::OnTimer(nIDEvent);
}
