// DSC110VerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "DSC110VerDlg.h"
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


// CDSC110VerDlg 对话框

IMPLEMENT_DYNAMIC(CDSC110VerDlg, CPropertyPage)

CDSC110VerDlg::CDSC110VerDlg()
	: CPropertyPage(CDSC110VerDlg::IDD)
	, m_pBaseCtl( new CBasePageCtl())
	, m_pCalibrate( NULL )
	, m_pAliasAPS( NULL )
	, m_pAliasPWMD( NULL )
	, m_pAliasPWMC( NULL )
	, m_pAliasPVS( NULL )
	, m_pAliasPV01( NULL )
	, m_pAliasPV02( NULL )
	, m_pVerThread( NULL )
	, m_pAliasPB1( NULL )
	, m_pAliasReset1( NULL )
	, m_pAliasRate1( NULL )
	, m_pAliasPB2( NULL )
	, m_pAliasReset2( NULL )
	, m_pAliasRate2( NULL )
	, m_bExitThread( false )
	, m_uUpdateCounts( 0 )
{

}

CDSC110VerDlg::~CDSC110VerDlg()
{
	delete m_pBaseCtl;
	m_pBaseCtl = NULL;
}

void CDSC110VerDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_AP, m_edtAPS);
	DDX_Control(pDX, IDC_PWMD_VALUE, m_stcPWMD);
	DDX_Control(pDX, IDC_PWMC_VALUE, m_stcPWMC);
	DDX_Control(pDX, IDC_PV_PER, m_stcPVS);
	DDX_Control(pDX, IDC_PV01_VALUE, m_stcPV01);
	DDX_Control(pDX, IDC_PV02_VALUE, m_stcPV02);
	DDX_Control(pDX, IDC_EDT_PB1, m_edtPB1);
	DDX_Control(pDX, IDC_EDT_RES1, m_edtRes1);
	DDX_Control(pDX, IDC_EDT_RATE1, m_edtRate1);
	DDX_Control(pDX, IDC_EDT_PB2, m_edtPB2);
	DDX_Control(pDX, IDC_EDT_RES2, m_edtRes2);
	DDX_Control(pDX, IDC_EDT_RATE2, m_edtRate2);
}


BEGIN_MESSAGE_MAP(CDSC110VerDlg, CPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDSC110VerDlg 消息处理程序

BOOL CDSC110VerDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pBaseCtl->Init( this );

	/// String 
	// Use
	CString str;
	str.LoadString( IDS_DSC110CAL_VERINFO );
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

	// Introduction
	if( GetDlgItem( IDC_DSC110CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_VER )->SetFont( &m_pBaseCtl->GetBoldFont()); 
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
				m_pAliasAPS  = pAliases->find_name( L"kAPS" );          // 42033  Angle Position Setting

				m_pAliasPWMD = pAliases->find_name( L"iPDMO" );         // 35023  PWM D
				m_pAliasPWMC = pAliases->find_name( L"iPWMCycle" );     // 35057  PWM Cycle
				m_pAliasPVS  = pAliases->find_name( L"iSelPV" );        // 35013  Select PV
				m_pAliasPV01 = pAliases->find_name( L"iPV01AngP" );     // 35001  PV01 Angle Position
				m_pAliasPV02 = pAliases->find_name( L"iPV02AngP" );     // 35007  PV02 Angle Position

				m_pAliasPB1 = pAliases->find_name( L"kPID1PB" ); 
				m_pAliasReset1 = pAliases->find_name( L"kPID1RES" );
			    m_pAliasRate1 = pAliases->find_name( L"kPID1RATE" );

				m_pAliasPB2 = pAliases->find_name( L"kPID2PB" ); 
				m_pAliasReset2 = pAliases->find_name( L"kPID2RES" );
			    m_pAliasRate2 = pAliases->find_name( L"kPID2RATE" );
			}
		}
	}

	// Calibrate control
	CCalSheet* pCalSheet = (CCalSheet*)GetParent();
	if( pCalSheet->GetSafeHwnd())
	{
		m_pCalibrate = (CDSC110Calibrate*)pCalSheet->GetCalibrate();
	}

	// 角位置给定: 0.0
	m_pAliasAPS->valR4( 0.0 );
	m_edtAPS.SetWindowTextW( L"0.000" );
	SetAnglePosSetting();

	// start verification thread
	m_pVerThread = AfxBeginThread( VerThread, this );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDSC110VerDlg::OnPaint()
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

	// Calibrate Angle: Passed
	CRect rect;
	if( GetDlgItem( IDC_DSC110CAL_CAL )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_CAL )->GetWindowRect( rect );  // screen coordinates 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetPassImage().Draw( dc.GetSafeHdc(), 10, rect.top );

	// Verfication: Passing
	if( GetDlgItem( IDC_DSC110CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_VER )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetPassingImage().Draw( dc.GetSafeHdc(), 10, rect.top );

	// Save and Run: UnPass
	if( GetDlgItem( IDC_DSC1110CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC1110CAL_SAVE )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetUnPassImage().Draw( dc.GetSafeHdc(), 10, rect.top );
}


BOOL CDSC110VerDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent();
	if( pParent->GetSafeHwnd())
	{
		pParent->SetWizardButtons( PSWIZB_NEXT );
	}

	return CPropertyPage::OnSetActive();
}


LRESULT CDSC110VerDlg::OnWizardNext()
{
	// If calibrate thread was active, exit it
	if((m_pVerThread != NULL) && !m_bExitThread )
	{
		m_bExitThread = true;

		if( WaitForSingleObject( m_pVerThread->m_hThread, 150 ) != WAIT_OBJECT_0 )
		{
			theApp.LogInfo( L"Terminate ver thread!" );
			::TerminateThread( m_pVerThread->m_hThread, 0 );
		}
	}

	return CPropertyPage::OnWizardNext();
}

void CDSC110VerDlg::OnCancel()
{
	// If calibrate thread was active, exit it
	if((m_pVerThread != NULL) && !m_bExitThread )
	{
		m_bExitThread = true;

		if( WaitForSingleObject( m_pVerThread->m_hThread, 150 ) != WAIT_OBJECT_0 )
		{
			theApp.LogInfo( L"Terminate ver thread!" );
			::TerminateThread( m_pVerThread->m_hThread, 0 );
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
BOOL CDSC110VerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* pWnd = GetFocus();
	if( NULL == pWnd->GetSafeHwnd())
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pWnd->GetDlgCtrlID() != IDC_EDT_AP)    &&   // Angle Position Settings
	   (pWnd->GetDlgCtrlID() != IDC_EDT_PB1)   &&
	   (pWnd->GetDlgCtrlID() != IDC_EDT_RES1)  &&
	   (pWnd->GetDlgCtrlID() != IDC_EDT_RATE1) &&
	   (pWnd->GetDlgCtrlID() != IDC_EDT_PB2)   &&
	   (pWnd->GetDlgCtrlID() != IDC_EDT_RES2)  &&
	   (pWnd->GetDlgCtrlID() != IDC_EDT_RATE2))
	{
		return CPropertyPage::PreTranslateMessage(pMsg);
	}

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))      // Enter Key was down
	{
		if( pWnd->GetDlgCtrlID() == IDC_EDT_AP )
		{
			SetAnglePosSetting();
		}
		else
		{
			TuningValue( pWnd->GetDlgCtrlID());
		}

		return TRUE;
	}//if VK_RETURN

	return CPropertyPage::PreTranslateMessage(pMsg);
}

// ==============================================================
// Function name: SetAnglePosSetting
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-20 17:00:00
// ==============================================================
bool CDSC110VerDlg::SetAnglePosSetting()
{
	if((!CheckWritePtr( m_pAliasAPS, sizeof( CAliasItem ))) ||
		(NULL == m_edtAPS.GetSafeHwnd()))  
	{
		return false;
	}

	CString strValue;
	float fValue = 0.0;
	m_edtAPS.GetWindowText( strValue );
	::swscanf_s( strValue, L"%f", &fValue );

	// Angle Position Setting: -360.0 - 360.0
	float fMinValid = -360.0;
    float fMaxValid = 360.0;
    if( fValue < fMinValid )
    {
        fValue = fMinValid;
    }
    else if( fValue > fMaxValid )
    {
        fValue = fMaxValid;
    }

	strValue.Format( L"%.3f", fValue );
	m_edtAPS.SetWindowTextW( strValue );

	m_pAliasAPS->valR4( fValue );

	// Send
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
	if( NULL == pAliases.get())
	{
		return false;
	}

	pAliases->Add( m_pAliasAPS );

	((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true ); 
	pAliases->clear();
	pAliases.reset();

    return true;
}


// ==============================================================
// Function name: TuningValue
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong 2013-02-20 17:00:00
// ==============================================================
void CDSC110VerDlg::TuningValue( UINT edtID )
{
	CAliasItem* pAlias = NULL;
	CEdit* pEdit = NULL;
	float fMinValid = 0.0;
    float fMaxValid = 0.0;
	switch( edtID )
	{
		case IDC_EDT_PB1:
			pAlias = m_pAliasPB1;
			pEdit = &m_edtPB1;
			fMaxValid = 1000.0;
		break;
		case IDC_EDT_RES1:
			pAlias = m_pAliasReset1;
			pEdit = &m_edtRes1;
			fMaxValid = 400.0;
		break;
		case IDC_EDT_RATE1:
			pAlias = m_pAliasRate1;
			pEdit = &m_edtRate1;
			fMaxValid = 10.0;
		break;
		
		case IDC_EDT_PB2:
			pAlias = m_pAliasPB2;
			pEdit = &m_edtPB2;
			fMaxValid = 1000.0;
		break;
		case IDC_EDT_RES2:
			pAlias = m_pAliasReset2;
			pEdit = &m_edtRes2;
			fMaxValid = 400.0;
		break;
		case IDC_EDT_RATE2:
			pAlias = m_pAliasRate2;
			pEdit = &m_edtRate2;
			fMaxValid = 10.0;
		break;
		default:
			return;
	}

	CString strValue;
	float fValue = 0.0;
	pEdit->GetWindowText( strValue );
	::swscanf_s( strValue, L"%f", &fValue );

	// Check if validate
	if( fValue < fMinValid )
    {
        fValue = fMinValid;
		strValue.Format( L"%.3f", fValue );
		pEdit->SetWindowTextW( strValue );
    }
    else if( fValue > fMaxValid )
    {
        fValue = fMaxValid;
		strValue.Format( L"%.3f", fValue );
		pEdit->SetWindowTextW( strValue );
    }

	if( fabs(pAlias->valR4()-fValue) > EPSILON_FLT )   //if( pAlias->valR4() != fValue )
	{
		/*float fDiff = std::fabs(fValue - pAlias->valR4());
		float fPercent = 1.0f;         // Default to 100%, to catch values of 0.0
		if( fabs(pAlias->valR4()) > EPSILON_FLT )  //if( pAlias->valR4() != 0.0f )
		{
			fPercent = fDiff / pAlias->valR4();
		}
				
		if( fPercent > 0.1f )
		{
			CString strWarning, strWrite, strCaption;
			strWarning.LoadString( IDS_TUNING_WARNING );
			strWrite.Format( strWarning, pAlias->valR4(), fValue, fPercent * 100.0f );
			strCaption.LoadString( IDS_WARNING );

			if( MessageBox( strWrite, strCaption, MB_ICONWARNING | MB_YESNO ) == IDNO )
			{
				CString strOriValue;
				strOriValue.Format( L"%.2f", pAlias->valR4());  
				pEdit->SetWindowText( strOriValue );
				return;
			}
		}*/
				
		pAlias->valR4( fValue );  

		// Communication
		std::auto_ptr<CAliasMap> pAliases( new CAliasMap());
		if( pAliases.get() == NULL )
		{
			return;
		}

		pAliases->Add( pAlias );
		((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get(), true, true); 

		pAliases->clear();
	    pAliases.reset();
	}
}


/***********************************************************/
// Communications
/***********************************************************/
UINT VerThread( LPVOID pParam )
{
	CDSC110VerDlg* pVerPage = (CDSC110VerDlg*)pParam;
	if( pVerPage->GetSafeHwnd())
	{
		while( !pVerPage->m_bExitThread )
		{
			pVerPage->Update();
			//::Sleep( 100 );     // Only for debug
		}
	}

	TRACE0( "VerThread Exit\n" );
	return 0;
}


void CDSC110VerDlg::Update()
{
	if((!m_bExitThread) && (WaitForSingleObject( g_pCalibrateEvent->m_hObject, 50 )==WAIT_OBJECT_0))
	{
		g_pCalibrateEvent->ResetEvent();

		CString str;
		if( m_uUpdateCounts == 0 )
		{
			m_uUpdateCounts++;

			if( CheckReadPtr( m_pAliasAPS, sizeof(CAliasItem))) // 42033 Angle Position Setting
			{
				str.Format( L"%.3f", m_pAliasAPS->remR4());
				m_edtAPS.SetWindowText( str );
			}

			// PID1
			if( CheckReadPtr( m_pAliasPB1, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasPB1->remR4());
				m_edtPB1.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasReset1, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasReset1->remR4());
				m_edtRes1.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasRate1, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasRate1->remR4());
				m_edtRate1.SetWindowText( str );
			}

			// PID2
			if( CheckReadPtr( m_pAliasPB2, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasPB2->remR4());
				m_edtPB2.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasReset2, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasReset2->remR4());
				m_edtRes2.SetWindowText( str );
			}

			if( CheckReadPtr( m_pAliasRate2, sizeof(CAliasItem)))
			{
				str.Format( L"%.3f", m_pAliasRate2->remR4());
				m_edtRate2.SetWindowText( str );
			}
		}

		if( CheckReadPtr( m_pAliasPWMD, sizeof( CAliasItem )))   // 35023 PWM D
		{
			str.Format( L"%.3f", m_pAliasPWMD->remR4());
			m_stcPWMD.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPWMC, sizeof( CAliasItem )))   // 35057 PWM Cycle
		{
			str.Format( L"%.3f", m_pAliasPWMC->remR4());
			m_stcPWMC.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPVS, sizeof( CAliasItem )))    // 35013 PV Select
		{
			str.Format( L"%.3f", m_pAliasPVS->remR4());
			m_stcPVS.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPV01, sizeof( CAliasItem )))   // 35001 PV01 Analog Position
		{
			str.Format( L"%.3f", m_pAliasPV01->remR4());
			m_stcPV01.SetWindowText( str );
		}

		if( CheckReadPtr( m_pAliasPV02, sizeof( CAliasItem )))   // 35007 PV02 Analog Position
		{
			str.Format( L"%.3f", m_pAliasPV02->remR4());
			m_stcPV02.SetWindowText( str );
		}
	}
}