#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CCommSettings;
// CCommSettingsDlg 对话框

class CCommSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommSettingsDlg)

public:
	CCommSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommSettingsDlg();

// 对话框数据
	enum { IDD = IDD_COMM_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnBtnSerDef();
	afx_msg void OnBtnTCPDef();
	afx_msg void OnBtnTest();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer( UINT_PTR nIDEvent ); 

	CComboBox m_cboType;

	CStatic m_stcCommMsg;

	CComboBox m_cboSerPort;
	CComboBox m_cboBaud;
	CComboBox m_cboDatabits;
	CComboBox m_cboParity;
	CComboBox m_cboStop;
	CComboBox m_cboRTS;
	CButton m_chkHand;
	CComboBox m_cboSerSlaveID;
	CButton m_btnSerDef;

	CIPAddressCtrl m_ipAddress;
	CEdit m_edtTCPPort;
	CComboBox m_cboTCPSlaveID;
	CButton m_btnTCPDef;

	CButton m_btnTest;
	CButton m_btnOK;
	CButton m_btnCancel;

public:
	CCommSettings* m_pSettings;
	void SetSettings( CCommSettings* pSettings );
	void GetSettings( CCommSettings* pSettings );

private:
	DWORD m_dwViewHelpID;

	void SetCtlEnableStatus( void );
	UINT TestThreadFunc( LPVOID pParam );	
public:
	CButton m_btnProtocol;
	afx_msg void OnBnClickedPccommCancel();
};
