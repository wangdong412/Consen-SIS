#pragma once
#include "afxwin.h"


// CCompareDlg 对话框

class CCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareDlg)

public:
	CCompareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompareDlg();

// 对话框数据
	enum { IDD = IDD_CMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCompareType;
	CButton m_rdoCtrl;
	CEdit m_edtFileName;
	CButton m_btnBrouse;

	afx_msg void OnClickedBtnFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	afx_msg void optCompareCtrl_OnClick();
	afx_msg void optCompareFile_OnClick();

	CString m_strFileName;
	CString GetFileName() const  { return m_strFileName; };
};
