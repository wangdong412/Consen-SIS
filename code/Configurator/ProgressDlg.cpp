// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

// Added by wangdong
#include "ModbusThread.h"     // extern ModbusThread    

//=======================================================
/// Globals
extern std::auto_ptr<modbus::CModbusThread> ModbusThread;

//=======================================================
// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_MSG, m_ctlMessage);
	DDX_Control(pDX, IDC_PROGRESS_COMM, m_ctlProgress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序

// ================================================
// Function name: OnInitDialog
// Description  : Initialize dialog controls
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.09 13:00:00
// =================================================
BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctlMessage.SetWindowText( L"" );

	// Range: 0 - 100
	m_ctlProgress.SetRange( 0, 100 );
	m_ctlProgress.SetPos( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CProgressDlg::OnCancel()
{
	ModbusThread->Close();
	ModbusThread->ClearMessages();
	ModbusThread->ResetThreadFlags();

	DestroyWindow();  // Modaless
}
