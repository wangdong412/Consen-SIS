#pragma once
#include "afxwin.h"


// CDSC110CalDlg 对话框
class CBasePageCtl;
class CDSC110Calibrate;
class CAliasItem;
class CWinThread;
class CDSC110CalDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CDSC110CalDlg)

public:
	CDSC110CalDlg();
	virtual ~CDSC110CalDlg();

// 对话框数据
	enum { IDD = IDD_DSC110CAL_CAL };
	enum { Ctl_Open, Ctl_Stop, Ctl_Close };
	enum { PICKUP_PV01, PICKUP_PV02 };

private:
	CBasePageCtl* m_pBaseCtl;
	CDSC110Calibrate* m_pCalibrate;

	CAliasItem *m_pAliasPWMD;       // 42031

	CAliasItem *m_pAliasPWMC;       // 35057
	CAliasItem *m_pAliasPV01Off;    // 35051
	CAliasItem *m_pAliasPV02Off;    // 35053

	CAliasItem *m_pAliasPV01OffS;   // 42089
	CAliasItem *m_pAliasPV02OffS;   // 42091

	CAliasItem *m_pMinDA;           // 42095
	CAliasItem *m_pMaxDA;           // 42097

	CWinThread *m_pCalThread;
	bool m_bExitThread;
	UINT m_uUpdateCounts;

	

private:
	void Update();
	bool SetPWMD( bool bInit=false );
	void SendOpenCloseFlag( const UINT uType );
	bool SetPickup( UINT uType );

friend UINT CalThread( LPVOID pParam );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CEdit m_edtPWMD;
	CStatic m_stcPWMC;
	CStatic m_stcPV01;
	CStatic m_stcPV02;
	CEdit m_edtPV01;
	CEdit m_edtPV02;
public:
	afx_msg void OnBnClicked50P();      // 50%
	afx_msg void OnBnClickedPv01ff();   // Pickup 01
	afx_msg void OnBnClickedPv02ff();   // Pickup 02
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
