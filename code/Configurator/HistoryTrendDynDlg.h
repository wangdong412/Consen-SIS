#pragma once


// CHistoryTrendDynDlg 对话框
class CViewHistoryCtrl;
class CConfiguratorDoc;
class CHistoryTrendDynDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryTrendDynDlg)

public:
	CHistoryTrendDynDlg( CConfiguratorDoc* pDoc, CViewHistoryCtrl *pViewCtrl, double dStartTime, CWnd* pParent = NULL );   // 标准构造函数
	virtual ~CHistoryTrendDynDlg();

// 对话框数据
	enum { IDD = IDD_HISTORY_DYN };

private:
	CConfiguratorDoc *m_pDoc;
	CViewHistoryCtrl *m_pViewCtrl;
	COleDateTime m_dtStart;

	CButton m_chkMM;
	CButton m_chkDD;
	CButton m_chkYY;
	CButton m_chkHH;
	CButton m_chkMIN;
	CButton m_chkSS;

	CSpinButtonCtrl m_spnRangD;
	CSpinButtonCtrl m_spnRangH;
	CSpinButtonCtrl m_spnRangM;
	CSpinButtonCtrl m_spnRangS;

	CSpinButtonCtrl m_spnRateH;
	CSpinButtonCtrl m_spnRateM;
	CSpinButtonCtrl m_spnRateS;

	int m_nStartM;
	int m_nStartD;
	int m_nStartY;
	int m_nStartH;
	int m_nStartMIN;
	int m_nStartS;

	int m_nRangD;
	int m_nRangH;
	int m_nRangM;
	int m_nRangS;

	int m_nRefH;
	int m_nRefM;
	int m_nRefS;

	CEdit m_edtTNV[8];
	CButton m_btnTNV[8];
	CMFCColorButton m_mcbV[8];
	CEdit m_edtWV[8];
	UINT m_nWidth[8];

public:
	double GetStartTime() const { return m_dtStart; }

private:
	void ResetPens();
	void DisablePens();
	void EnablePens( BOOL bEnable );
	void LoadFromViewCtrl();
	void LoadFromViewDatas();

	void SelectTagname( int nPenIndex );
	void SetViewHistoryCtrl();
	void SetHisViewDatas();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClicked_Reset();
	afx_msg void OnBnClickedBtnTn1();
	afx_msg void OnBnClickedBtnTn2();
	afx_msg void OnBnClickedBtnTn3();
	afx_msg void OnBnClickedBtnTn4();
	afx_msg void OnBnClickedBtnTn5();
	afx_msg void OnBnClickedBtnTn6();
	afx_msg void OnBnClickedBtnTn7();
	afx_msg void OnBnClickedBtnTn8();
};
