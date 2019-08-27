#pragma once
#include "afxwin.h"


// CCalMinDlg 对话框

class CAliasItem;
class CDSC100Calibrate;
class CCalMinDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CCalMinDlg)

public:
	CCalMinDlg();
	virtual ~CCalMinDlg();

// 对话框数据
	enum { IDD = IDD_CAL_MIN };
	enum { PICKUP_AI01, PICKUP_AI02 };

private:
	UINT m_uUpdateCounts;

	CFont m_boldFont;
	CFont m_norFont;
	CImage m_imgPassing;
	CImage m_imgPass;
	CImage m_imgUnPass;

	// Null Current offset mA
	CAliasItem* m_pAliasNullOff;    // 42161  

	// AI01
	CAliasItem* m_pAliasAI01;       // 35001  %
	CAliasItem* m_pAliasAI01V;      // 35011  v

	// AI02
	CAliasItem* m_pAliasAI02;       // 35003  %
	CAliasItem* m_pAliasAI02V;      // 35013  v

	// PV
	CAliasItem* m_pAliasPV;         // 35081  %

	/// Actuator
	// V1.0
	CAliasItem* m_pAliasActCur;     // 35039 mA

	// V1.1
	CAliasItem* m_pAliasActCur2;    // 35041  Actuator Current 02

	CAliasItem* m_pAliasAI01MinV;   // 42053  AI 01 Min Voltage
	CAliasItem* m_pAliasAI02MinV;   // 42057  AI 02 Min Voltage


	// Others
	CAliasItem* m_pAliasNULLCur;    // 35045  Actual NULL Current
	CAliasItem* m_pAliasActMin;     // 42147  Actuator Min Current
	CAliasItem* m_pAliasActMax;     // 42149  Actuator Max Current

	CDSC100Calibrate* m_pCalibrate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedAi01min();
	afx_msg void OnBnClickedAi02min();

public:
	CEdit m_edtNULL;
	CStatic m_stcPV;   // PV value

	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	// AI01 Percent and Voltage
	CStatic m_stcAI01PName;
	CStatic m_stcAI01P;
	CStatic m_stcAI01PUnit;
	CStatic m_stcAI01V;
	CStatic m_stcAI01VUnit;

	// AI02 Percent and Voltage
	CStatic m_stcAI02PName;
	CStatic m_stcAI02P;
	CStatic m_stcAI02PUnit;
	CStatic m_stcAI02V;
	CStatic m_stcAI02VUnit;

	// Actuator - V1.0
	CStatic m_stcACTCName;
	CStatic m_stcActCur;
	CStatic m_stcACTCUnit;

	/// V1.1:
	// Actuator 01
	CStatic m_stcACT01CName;
	CStatic m_stcACT01CValue;
	CStatic m_stcACT01CUnit;

	// Actuator 02
	CStatic m_stcACT02CName;
	CStatic m_stcACT02CValue;
	CStatic m_stcACT02CUnit;

	// AI01 Min
	CStatic m_stcAI01MINName;
	CEdit m_edtAI01MINValue;
	CStatic m_stcAI01MINUnit;
	CButton m_btnAI01MIN;

	// AI02 Min
	CStatic m_stcAI02MINName;
	CEdit m_edtAI02MINValue;
	CStatic m_stcAI02MINUnit;
	CButton m_btnAI02MIN;

private:
	void SetVisible();
	bool SetNULLCurOffset();                                   // 42161, Null Current Offset 
	bool SetAIVoltage( CAliasItem *pAlias, CEdit &edtValue );  // 42053/42057,  AI 01/02 Min Voltage
	bool SetPickup( UINT uType );
};
