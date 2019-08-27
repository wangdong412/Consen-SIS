#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CSendDlg 对话框

class CMainFrame;
class CConfiguratorDoc;

class CSendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSendDlg(bool bRunning, CWnd* pParent = NULL);   
	virtual ~CSendDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkChanged;
	CStatic m_stcDef;
	CStatic m_stcDefChanged;
	CStatic m_stcDefSame;
	CStatic m_stcTuning;
	CStatic m_stcTuningChanged;
	CStatic m_stcTuningSame;
	CStatic m_stcCfg;
	CStatic m_stcCfgChanged;
	CStatic m_stcCfgSame;
	CButton m_rdoCfg;
	CStatic m_stcMode;
	CStatic m_stcSpeed;
	CProgressCtrl m_proPos;
	int m_nOptSel;
	bool m_bCTLRunning;

	virtual BOOL OnInitDialog();	
	virtual void OnOK();

	bool m_bTuningOnly;
    bool m_bDirtyOnly;

private:
	CMainFrame* m_pFrame;
	CConfiguratorDoc* m_pDoc;

};

