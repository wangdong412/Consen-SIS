#pragma once


// CCTRLDateTimeDlg 对话框
class CMainFrame;
class CConfiguratorDoc;

class CCTRLDateTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCTRLDateTimeDlg)

public:
	CCTRLDateTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCTRLDateTimeDlg();

// 对话框数据
	enum { IDD = IDD_DLG_DATETIME };

private:
	CMainFrame *m_pMainWnd;
	CConfiguratorDoc *m_pDoc;
	CWinThread* m_pDateTimeThread;

	CDateTimeCtrl m_RealDateTime;
	CDateTimeCtrl m_DateTime;

public:
	CEvent *m_pEvent;
	bool m_bDateTimeTer;
	void SetEvent()    { m_pEvent->SetEvent(); }
	inline DWORD TerminateTimeThread( DWORD dwMiliSecs = INFINITE );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	void UpdateTime();
	virtual void OnCancel();
};
