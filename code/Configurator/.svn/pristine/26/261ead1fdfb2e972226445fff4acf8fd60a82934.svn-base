#pragma once

class CDSC100Calibrate;

// CCalMaxDlg 对话框

class CCalMaxDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CCalMaxDlg)

public:
	CCalMaxDlg();
	virtual ~CCalMaxDlg();

// 对话框数据
	enum { IDD = IDD_CAL_MAX };
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
	CAliasItem* m_pAliasActCur2;     // 35041  Actuator Current 02

	CAliasItem* m_pAliasAI01MaxV;    // 42055  AI 01 Min Voltage
	CAliasItem* m_pAliasAI02MaxV;    // 42059  AI 02 Min Voltage

	// Others
	CAliasItem* m_pAliasNULLCur;    // 35045
	CAliasItem* m_pAliasActMin;     // 42147
	CAliasItem* m_pAliasActMax;     // 42149

	CDSC100Calibrate* m_pCalibrate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

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

	// AI01 Max
	CStatic m_stcAI01MAXName;
	CEdit m_edtAI01MAXValue;
	CStatic m_stcAI01MAXUnit;
	CButton m_btnAI01MAX;

	// AI02 Max
	CStatic m_stcAI02MAXName;
	CEdit m_edtAI02MAXValue;
	CStatic m_stcAI02MAXUnit;
	CButton m_btnAI02MAX;

private:
	void SetVisible();
	bool SetNULLCurOffset();                                   // 42161, Null Current Offset 
	bool SetAIVoltage( CAliasItem *pAlias, CEdit &edtValue );  // 42055/42059,  AI 01/02 Max Voltage  
	bool SetPickup( UINT uType );
public:
	afx_msg void OnBnClickedAi01max();
	afx_msg void OnBnClickedAi02max();
};
