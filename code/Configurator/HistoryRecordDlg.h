#pragma once
#include "afxwin.h"


// CHistoryRecordDlg 对话框
class CConfiguratorDoc;
class CRecordHistoryCtrl;
class CHistoryRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryRecordDlg)

public:
	CHistoryRecordDlg( CConfiguratorDoc* pDoc, CRecordHistoryCtrl* pRecordCtrl, CWnd* pParent = NULL );   // 标准构造函数
	virtual ~CHistoryRecordDlg();

// 对话框数据
	enum { IDD = IDD_HISTORY_RECORD };

private:
	CRecordHistoryCtrl *m_pRecordCtrl;

	CEdit m_edtRecordFile;

	CSpinButtonCtrl m_spnRateH;
	CSpinButtonCtrl m_spnRateM;
	CSpinButtonCtrl m_spnRateS;

	int m_nRateH;
	int m_nRateM;
	int m_nRateS;

	UINT m_nFileSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked_Browse();
	afx_msg void OnBnClicked_Reset();
	afx_msg void OnBnClickedOk();
};
