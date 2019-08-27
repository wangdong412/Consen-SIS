#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CProgressDlg 对话框

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressDlg();

// 对话框数据
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_ctlMessage;
	CProgressCtrl m_ctlProgress;

public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	// Message
	void SetMessage( CString strMsg )      { m_ctlMessage.SetWindowText(strMsg); }
	CString GetMessage()                   const 
	{ 
		CString s;
		m_ctlMessage.GetWindowText( s );
		return s;
	}

	// Progress
	void SetProgress( int nProgress )      { m_ctlProgress.SetPos( nProgress ); }
	int GetProgress()                      const { return m_ctlProgress.GetPos(); }
};
