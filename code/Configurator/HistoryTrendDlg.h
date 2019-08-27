#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"


// CHistoryTrendDlg 对话框
class CViewHistoryCtrl;
class CHistoryFileLoadCtrl;
class CConfiguratorDoc;
class CHistoryTrendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryTrendDlg)

public:
	CHistoryTrendDlg( CConfiguratorDoc* pDoc, CViewHistoryCtrl *pViewCtrl, CWnd* pParent = NULL );   
	virtual ~CHistoryTrendDlg();

// 对话框数据
	enum { IDD = IDD_HISTORY_TREND };

private:
	CConfiguratorDoc *m_pDoc;
	CViewHistoryCtrl *m_pViewCtrl;
	CHistoryFileLoadCtrl *m_pLoadCtrl;

	CMFCEditBrowseCtrl m_mebFile;

	CButton m_chkMM;
	CButton m_chkDD;
	CButton m_chkYY;
	CButton m_chkHH;
	CButton m_chkMIN;
	CButton m_chkSS;

	CSpinButtonCtrl m_spnRateH;
	CSpinButtonCtrl m_spnRateM;
	CSpinButtonCtrl m_spnRateS;

	int m_nRefH;
	int m_nRefM;
	int m_nRefS;

	CEdit m_edtTNV[8];
	CButton m_btnTNV[8];
	CMFCColorButton m_mcbV[8];
	CEdit m_edtWV[8];
	UINT m_nWidth[8];

	CButton m_btnLoad;

	bool m_bBrowseSetHisFile;
	CString m_strRecordingFileName;  // 对话框初始化时，记录当前正在被记录的历史文件名
	CString m_strPreHisFileName;     // 当用户选择查看的文件名无效时，用户恢复之前的有效的文件名
	LPARAM m_lHint;

public:
	LPARAM GetUpdateType() const { return m_lHint; }

private:
	void ResetPens();
	void DisablePens();
	void EnablePens( BOOL bEnable );
	void LoadFromViewCtrl();
	void LoadFromViewDatas();

	void SelectTagname( int nPenIndex );
	void SetViewHistoryCtrl( CString strHisFileName );
	void SetHisViewDatas();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeHisFile();
	afx_msg void OnBnClicked_LoadHisfile();
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
