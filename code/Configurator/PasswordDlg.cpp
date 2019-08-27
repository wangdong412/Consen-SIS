// PasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "PasswordDlg.h"
#include "afxdialogex.h"


// CPasswordDlg 对话框

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialogEx)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordDlg::IDD, pParent)
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_CURRENT, m_edtCurrent);
	DDX_Control(pDX, IDC_EDT_PASSWORD, m_edtPassword);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialogEx)
END_MESSAGE_MAP()


// CPasswordDlg 消息处理程序


BOOL CPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowContextHelpId( HIDR_PASSWORD );    // Help ID

	m_edtCurrent.SetWindowText( global::Security.GetName());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPasswordDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strPassword;
	m_edtPassword.GetWindowText( strPassword );
	if( !global::Security.Login( strPassword ))
	{
        AfxMessageBox( IDS_INVALID_PASSWORD, MB_OK | MB_ICONERROR );
		return;
	}

	CDialogEx::OnOK();
}
