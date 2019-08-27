#pragma once
#include "afxwin.h"


// CDSC110VerDlg 对话框
class CBasePageCtl;
class CDSC110Calibrate;
class CAliasItem;
class CWinThread;
class CDSC110VerDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CDSC110VerDlg)

public:
	CDSC110VerDlg();
	virtual ~CDSC110VerDlg();

// 对话框数据
	enum { IDD = IDD_DSC110CAL_VER };

private:
	CBasePageCtl* m_pBaseCtl;
	CDSC110Calibrate* m_pCalibrate;

	CAliasItem *m_pAliasAPS;
	CAliasItem *m_pAliasPWMD;
	CAliasItem *m_pAliasPWMC;
	CAliasItem *m_pAliasPVS;
	CAliasItem *m_pAliasPV01;
	CAliasItem *m_pAliasPV02;

	CAliasItem* m_pAliasPB1;
	CAliasItem* m_pAliasReset1;
	CAliasItem* m_pAliasRate1;

	CAliasItem* m_pAliasPB2;
	CAliasItem* m_pAliasReset2;
	CAliasItem* m_pAliasRate2;

	CWinThread *m_pVerThread;
	bool m_bExitThread;
	UINT m_uUpdateCounts;

private:
	void Update();
	bool SetAnglePosSetting();
	void TuningValue( UINT edtID );

friend UINT VerThread( LPVOID pParam );

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
	CEdit m_edtAPS;
	CStatic m_stcPWMD;
	CStatic m_stcPWMC;
	CStatic m_stcPVS;
	CStatic m_stcPV01;
	CStatic m_stcPV02;
	CEdit m_edtPB1;
	CEdit m_edtRes1;
	CEdit m_edtRate1;
	CEdit m_edtPB2;
	CEdit m_edtRes2;
	CEdit m_edtRate2;
};
