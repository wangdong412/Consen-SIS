#pragma once
#include "afxwin.h"


// CPasswordDlg 对话框

class CPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordDlg)

public:
	CPasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPasswordDlg();

// 对话框数据
	enum { IDD = IDD_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtCurrent;
	CEdit m_edtPassword;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
