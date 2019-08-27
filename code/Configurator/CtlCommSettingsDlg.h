#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"

class CMainFrame;
class CConfiguratorDoc;


// CCtlCommSettingsDlg 对话框

class CCtlCommSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCtlCommSettingsDlg)

public:
	CCtlCommSettingsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCtlCommSettingsDlg();

// 对话框数据
	enum { IDD = IDD_CTL_COMMSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
private:
	CMainFrame *m_pMainWnd;
	CConfiguratorDoc *m_pDoc;

	// Serial 1
	CButton m_chkSer1;
	CComboBox m_cboType1;        // COM1 Type
	CComboBox m_cboBaudRate1;    // Baud Rate
	CComboBox m_cboSB1;          // Stop Bits
	CComboBox m_cboDB1;          // Data Bits
	CComboBox m_cboPB1;          // Parity Bits
	CComboBox m_cboSer1ID;       // Slave ID
	CComboBox m_cboPT1;          // Protocol Type

	// Serial 2
	CButton m_chkSer2;
	CComboBox m_cboType2;
	CComboBox m_cboBaudRate2;
	CComboBox m_cboSB2;
	CComboBox m_cboDB2;
	CComboBox m_cboPB2;
	CComboBox m_cboSer2ID;
	CComboBox m_cboPT2;

	// Ethernet
	CIPAddressCtrl m_ctlIP;
	CIPAddressCtrl m_ctlMask;
	CIPAddressCtrl m_ctlGate;
	CComboBox m_cboTCPID;
	CEdit m_edtPN;

private:
	void EnableCtrls( BOOL bSerial1, BOOL bSerial2 );
public:
	afx_msg void OnBnClickedChkSer1();
	afx_msg void OnBnClickedChkSer2();
	afx_msg void OnBnClickedBtnDatetime();
};
