// SendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "SendDlg.h"
#include "afxdialogex.h"

#include "SendStatus.h"
#include "MainFrm.h"
#include "ConfiguratorDoc.h"

//=====================================================

// CSendDlg 对话框

IMPLEMENT_DYNAMIC(CSendDlg, CDialogEx)

CSendDlg::CSendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendDlg::IDD, pParent)
	, m_bCTLRunning( false )
	, m_nOptSel( 0 )
{
	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT( m_pFrame->GetSafeHwnd() );
	
	m_pDoc = (CConfiguratorDoc*)(m_pFrame->GetActiveDocument());
	ASSERT( m_pDoc );
}

CSendDlg::CSendDlg(bool bRunning, CWnd* pParent)
	: CDialogEx(CSendDlg::IDD, pParent)
	, m_bCTLRunning( bRunning )
	, m_nOptSel( 0 )
{
	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT( m_pFrame->GetSafeHwnd() );
	
	m_pDoc = (CConfiguratorDoc*)(m_pFrame->GetActiveDocument());
	ASSERT( m_pDoc );
}

CSendDlg::~CSendDlg()
{
}

void CSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CHANGED, m_chkChanged);
	DDX_Control(pDX, IDC_STC_DEF, m_stcDef);
	DDX_Control(pDX, IDC_STC_DEFCHANGED, m_stcDefChanged);
	DDX_Control(pDX, IDC_STC_DEFSAME, m_stcDefSame);
	DDX_Control(pDX, IDC_STC_TUNING, m_stcTuning);
	DDX_Control(pDX, IDC_STC_TUNINGCHANGED, m_stcTuningChanged);
	DDX_Control(pDX, IDC_STC_TUNINGSAME, m_stcTuningSame);
	DDX_Control(pDX, IDC_STC_CFG, m_stcCfg);
	DDX_Control(pDX, IDC_STC_CFGCHANGED, m_stcCfgChanged);
	DDX_Control(pDX, IDC_STC_CFGSAME, m_stcCfgSame);
	DDX_Control(pDX, IDC_RADIO_CFG, m_rdoCfg);
	DDX_Radio(pDX, IDC_RADIO_CFG, m_nOptSel);
	DDX_Control(pDX, IDC_STC_MODEVALUE, m_stcMode);
	DDX_Control(pDX, IDC_STC_SPDVALUE, m_stcSpeed);
	DDX_Control(pDX, IDC_PROGRESS_POS, m_proPos);
}


BEGIN_MESSAGE_MAP(CSendDlg, CDialogEx)
END_MESSAGE_MAP()


// CSendDlg 消息处理程序


BOOL CSendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT( m_pFrame->GetSafeHwnd() && m_pDoc );
	if( !(m_pFrame->GetSafeHwnd() && m_pDoc))
	{
		return FALSE;
	}

	///1. Option
	m_nOptSel = 0;
	m_chkChanged.SetCheck( BST_UNCHECKED );   // Uncheck changed option  

	///2. TS350: Enable and Disable
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		if( !m_bCTLRunning )
		{
			// Shutdown: if Application Type Mismatch: Disable Tuning and Changed Only Option
			BOOL bEnable = !theApp.m_bAppMismatch;

			// Tuning Only
			if( GetDlgItem( IDC_RADIO_TUNING )->GetSafeHwnd())
			{
				GetDlgItem( IDC_RADIO_TUNING )->EnableWindow( bEnable ); 
			}

			// Changed Only
			m_chkChanged.EnableWindow( bEnable );
		}
		else 
		{
			// Running: Check and Disable it
			m_chkChanged.SetCheck( BST_CHECKED );      // Check changed option 
			m_chkChanged.EnableWindow( FALSE );
		}
	}//TS350

	// Get Send Status to initialize Send Dialog
	const CSendStatus* pStatus = m_pDoc->GetSendDlgStatus();
	if( pStatus )
	{
		CString str;

		///3. Summary  
		// Def
		str.Format( L"%d", pStatus->GetDefChanged());
		m_stcDefChanged.SetWindowText( str );            // Def Changed

		str.Format( L"%d", pStatus->GetDefSame());       
		m_stcDefSame.SetWindowText( str );               // Def Same

		// Tuning
		str.Format( L"%d", pStatus->GetTuningChanged());
		m_stcTuningChanged.SetWindowText( str );         // Tuning Changed

		str.Format( L"%d", pStatus->GetTuningSame());       
		m_stcTuningSame.SetWindowText( str );            // Tuning Same

		// Configuration
		str.Format( L"%d", pStatus->GetCfgChanged());
		m_stcCfgChanged.SetWindowText( str );            // Configuration Changed

		str.Format( L"%d", pStatus->GetCfgSame());       
		m_stcCfgSame.SetWindowText( str );               // Configuration Same

        ///4. Turbine Status
		// Mode
		m_stcMode.SetWindowText( pStatus->GetMode()); 

		// Speed
		str.Format( L"%.2f", pStatus->GetSpeed());    
		m_stcSpeed.SetWindowText( str ); 

		// Speed   
		m_proPos.SetPos( static_cast<int>(pStatus->GetProgress()));
	}
	
	//5.Visible
	int nCmdShow = SW_HIDE;
	if( theApp.GetProductType() == CConfiguratorApp::TS350 )
	{
		nCmdShow = SW_SHOW;
	}// TS350

	// Group Box: Turbine Status
	if( GetDlgItem( IDC_STC_STATUS ))
	{
		GetDlgItem( IDC_STC_STATUS )->ShowWindow( nCmdShow );
	}

	// Static: Mode, Speed, Valve Position
	if( GetDlgItem( IDC_STC_MODE ))
	{
		GetDlgItem( IDC_STC_MODE )->ShowWindow( nCmdShow );
	}

	if( GetDlgItem( IDC_STC_SPD ))
	{
		GetDlgItem( IDC_STC_SPD )->ShowWindow( nCmdShow );
	}

	if( GetDlgItem( IDC_STC_POS ))
	{
		GetDlgItem( IDC_STC_POS )->ShowWindow( nCmdShow );
	}

	// Static: Mode, Speed Value, Progress
	if( GetDlgItem( IDC_STC_MODEVALUE ))
	{
		GetDlgItem( IDC_STC_MODEVALUE )->ShowWindow( nCmdShow );
	}

	if( GetDlgItem( IDC_STC_SPDVALUE ))
	{
		GetDlgItem( IDC_STC_SPDVALUE )->ShowWindow( nCmdShow );
	}

	if( GetDlgItem( IDC_PROGRESS_POS ))
	{
		GetDlgItem( IDC_PROGRESS_POS )->ShowWindow( nCmdShow );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSendDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	// Update global flags
	m_bTuningOnly = (m_nOptSel == 1);
	m_bDirtyOnly = (m_chkChanged.GetCheck() == BST_CHECKED );

	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		// TS350, 应用类型不一致时，控制器应处于停车状态。
		if( theApp.m_bAppMismatch )
		{
			ASSERT( !m_bCTLRunning );
			if( !m_bCTLRunning )
			{
				ASSERT( m_pFrame->GetSafeHwnd() && m_pDoc );
				if( !(m_pFrame->GetSafeHwnd() && m_pDoc))
				{
					return;
				}

				CAliasItem* pAlias = m_pDoc->m_aAliases->find_name( L"gAPPTYPE" );  // 2143 Application Type
				if( CheckWritePtr(pAlias, sizeof(pAlias)))
				{
					BOOL bAppType = (CConfiguratorDoc::APPTYPE_GEN == theApp.GetAppType());
					pAlias->valBool( bAppType );
				}
			}
			else // log info
			{
				theApp.LogInfo( L"CSendDlg::OnOK: TS350 is running, the local application type should be same with the controller." );
				return;
			}
		}//if( theApp.m_bAppMismatch )
		else if( m_bCTLRunning )  
		{
			if( !m_bTuningOnly ) // If not tunning, show warning
			{
				CString strWarning, strCaption;
				strWarning.LoadString( IDS_SENDCFG_WARNING );
				strCaption.LoadString( IDS_WARNING );
				if( MessageBox( strWarning, strCaption, MB_OKCANCEL | MB_ICONWARNING ) != IDOK )
				{
					return;
				}
			}//if( !m_bTuningOnly )
		}
	}//TS350

	CDialogEx::OnOK();
}
