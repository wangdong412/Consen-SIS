// DSC110SaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "DSC110SaveDlg.h"
#include "afxdialogex.h"

#include "BasePage.h"
#include "CalibrateBase.h"
#include "CalSheet.h"
#include "MainFrm.h"
#include "ModbusThread.h"

///======================================================
// global

extern std::auto_ptr<modbus::CModbusThread> ModbusThread;
///======================================================

// CDSC110SaveDlg 对话框

IMPLEMENT_DYNAMIC(CDSC110SaveDlg, CPropertyPage)

CDSC110SaveDlg::CDSC110SaveDlg()
	: CPropertyPage(CDSC110SaveDlg::IDD)
	, m_pBaseCtl( new CBasePageCtl())
	, m_pCalibrate( NULL )
{

}

CDSC110SaveDlg::~CDSC110SaveDlg()
{
	delete m_pBaseCtl;
	m_pBaseCtl = NULL;
}

void CDSC110SaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDSC110SaveDlg, CPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDSC110SaveDlg 消息处理程序
BOOL CDSC110SaveDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pBaseCtl->Init( this );

	/// String 
	// Info
	CString str;
	str.LoadString( IDS_DSC110CAL_SAVEINFO );
	if( GetDlgItem( IDC_DSC110CAL_INFO )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_INFO )->SetWindowText( str );
	}

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

	// Save 
	if( GetDlgItem( IDC_DSC1110CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC1110CAL_SAVE )->SetFont( &m_pBaseCtl->GetBoldFont()); 
	}

	// Calibrate control
	CCalSheet* pCalSheet = (CCalSheet*)GetParent();
	if( pCalSheet->GetSafeHwnd())
	{
		m_pCalibrate = (CDSC110Calibrate*)pCalSheet->GetCalibrate();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDSC110SaveDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()

	// Check if bitmaps are currently loaded.
	if( m_pBaseCtl->GetPassingImage().IsNull() ||
		m_pBaseCtl->GetPassImage().IsNull())
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

	// Verfication: Passed
	if( GetDlgItem( IDC_DSC110CAL_VER )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC110CAL_VER )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetPassImage().Draw( dc.GetSafeHdc(), 10, rect.top );

	// Save and Run: Passing
	if( GetDlgItem( IDC_DSC1110CAL_SAVE )->GetSafeHwnd())
	{
		GetDlgItem( IDC_DSC1110CAL_SAVE )->GetWindowRect( &rect ); 
		ScreenToClient( &rect );
	}
	m_pBaseCtl->GetPassingImage().Draw( dc.GetSafeHdc(), 10, rect.top );
}


BOOL CDSC110SaveDlg::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* pParent = (CPropertySheet*)GetParent();
	if( pParent->GetSafeHwnd())
	{
		pParent->SetWizardButtons( PSWIZB_FINISH );

		// Save button
		CString str;
		str.LoadString( IDS_CALBTN_SAVERUN );
		pParent->SetFinishText( str );
	}

	return CPropertyPage::OnSetActive();
}

BOOL CDSC110SaveDlg::OnWizardFinish()
{
	if( m_pCalibrate != NULL )
	{
		m_pCalibrate->SetAliasFlag( false, false, true, false );

		// Stop Monitor if not monitor before calibrate
		if( !m_pCalibrate->IsOrigMonitoring())
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if( pFrame->GetSafeHwnd())
			{
				WaitForSingleObject( ModbusThread->GetEvent()->m_hObject, 500 );
				::Sleep( 500 );
				pFrame->OnMonitorStop();
			}
		}
	}

	return CPropertyPage::OnWizardFinish();
}


void CDSC110SaveDlg::OnCancel()
{
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
