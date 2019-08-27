// CompareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "CompareDlg.h"
#include "afxdialogex.h"

//Added by wangdong
#include "MainFrm.h"


// CCompareDlg 对话框

IMPLEMENT_DYNAMIC(CCompareDlg, CDialogEx)

CCompareDlg::CCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompareDlg::IDD, pParent)
	, m_nCompareType( 0 )
{

}

CCompareDlg::~CCompareDlg()
{
}

void CCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RDO_REM, m_nCompareType);
	DDX_Control(pDX, IDC_EDT_FILENAME, m_edtFileName);
	DDX_Control(pDX, IDC_BTN_FILE, m_btnBrouse);
	DDX_Control(pDX, IDC_RDO_REM, m_rdoCtrl);
}


BEGIN_MESSAGE_MAP(CCompareDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FILE, &CCompareDlg::OnClickedBtnFile)
	ON_BN_CLICKED(IDC_RDO_REM, &CCompareDlg::optCompareCtrl_OnClick)
	ON_BN_CLICKED(IDC_RDO_FILE, &CCompareDlg::optCompareFile_OnClick)
END_MESSAGE_MAP()


// CCompareDlg 消息处理程序
// =========================================================================
// Function name: OnInitDialog
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.04.14 09:00:00
// =========================================================================
BOOL CCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edtFileName.EnableWindow( FALSE );

	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		bool bConnected = false;           // Get Connect Flag
		if((CMainFrame*)AfxGetMainWnd())
		{
			bConnected = ((CMainFrame*)AfxGetMainWnd())->IsConnected();
		}

		if( bConnected )
		{
			if( !global::Security.IsValid())
			{
				m_rdoCtrl.EnableWindow( FALSE );
				m_nCompareType = 1;
			}
			else
			{
				m_rdoCtrl.EnableWindow( TRUE );
				m_nCompareType = 0;
			}
		}
		else
		{
			m_rdoCtrl.EnableWindow( FALSE );
			m_nCompareType = 1;
		}
	}// TS350
	else
	{
		if( !global::Security.IsValid())
		{
			m_rdoCtrl.EnableWindow( FALSE );
			m_nCompareType = 1;
		}
		else
		{
			m_rdoCtrl.EnableWindow( TRUE );
			m_nCompareType = 0;
		}
	}

	if( m_nCompareType == 0 )
	{
		m_btnBrouse.EnableWindow( FALSE );
	}
	else
	{
		m_btnBrouse.EnableWindow();
	}

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCompareDlg::optCompareCtrl_OnClick()
{
	// TODO: 在此添加控件通知处理程序代码	
	m_btnBrouse.EnableWindow( FALSE );
}

void CCompareDlg::optCompareFile_OnClick()
{
	m_btnBrouse.EnableWindow();
}

// =========================================================================
// Function name: OnClickedBtnFile
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.04.13 17:00:00
// =========================================================================
void CCompareDlg::OnClickedBtnFile()
{
	// TODO: 在此添加控件通知处理程序代码
	// Create FileDialog
	std::auto_ptr<CFileDialog> dlg( new CFileDialog( true ));

	// File filter
	switch( theApp.GetProductType())
	{ 
		case CConfiguratorApp::DSC100:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC100 Files (*.DSC100)\0*.DSC100\0\0";
	    break;
		case CConfiguratorApp::TS350:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen TS350 Files (*.TS350)\0*.TS350\0\0";
	    break;
		case CConfiguratorApp::DSC110:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC110 Files (*.DSC110)\0*.DSC110\0\0";
	    break;
		default:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC100 Files (*.DSC100)\0*.DSC100\0\0";
	}

	// Def Ext
	dlg->m_ofn.lpstrDefExt = theApp.GetProductName();  

	if( dlg->DoModal() == IDOK )
	{
		m_strFileName = dlg->GetPathName();
		m_edtFileName.SetWindowText( m_strFileName );
	}
}

void CCompareDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if( 1 == m_nCompareType )  // Compare File
	{
		if( m_strFileName.IsEmpty())
		{
			AfxMessageBox( IDS_FILECMP_EMPTY, MB_ICONERROR | MB_OK );
			return;
		}
	}

	CDialogEx::OnOK();
}
