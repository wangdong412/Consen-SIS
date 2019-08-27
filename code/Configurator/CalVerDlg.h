#pragma once
#include "afxwin.h"


// CCalVerDlg 对话框
class CDSC100Calibrate;
class CCalVerDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CCalVerDlg)

public:
	CCalVerDlg();
	virtual ~CCalVerDlg();

// 对话框数据
	enum { IDD = IDD_CAL_VER };

private:
	CFont m_boldFont;
	CFont m_norFont;
	CImage m_imgPassing;
	CImage m_imgPass;
	CImage m_imgUnPass;

	UINT m_uUpdateCounts;

	// Demand(%)
	CAliasItem* m_pAliasDemand;     // 42123 

	// AI01 Position and Voltage Value
	CAliasItem* m_pAliasAI01;       // 35001
	CAliasItem* m_pAliasAI01V;      // 35011

	// AI02 Position and Voltage Value
	CAliasItem* m_pAliasAI02;       // 35003 
	CAliasItem* m_pAliasAI02V;      // 35013 

	// PV Position
	CAliasItem* m_pAliasPV;         // 35081 

	// V1.0
	CAliasItem* m_pAliasActCur;     // 35039 Actuator Current
	CAliasItem* m_pAliasActNull;    // 42159 Actuator Null Current

	// V1.1
	CAliasItem* m_pAliasActCur2;    // 35041 Actuator 02 Current 
	CAliasItem* m_pAliasACT02NULL;  // 42163
	CAliasItem* m_pAliasACT12NULL;  // 42165

	// Others
	CAliasItem* m_pAliasActMin;     // 42147  Actuator Min Current
	CAliasItem* m_pAliasActMax;     // 42149  Actuator Max Current

	CDSC100Calibrate* m_pCalibrate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CEdit m_edtDemand;
	CStatic m_stcPV;

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

	/// V1.0 
	// Actuator Current
	CStatic m_stcACTCName;
	CStatic m_stcActCur;
	CStatic m_stcACTCUnit;

	// Actuator Null Current
	CStatic m_stcACTNULLName;
	CEdit m_stcActNull;
	CStatic m_stcACTNULLUnit;

	/// V1.1:
	// Actuator 01 Current
	CStatic m_stcACT01CName;
	CStatic m_stcACT01CValue;
	CStatic m_stcACT01CUnit;

	// Actuator 02 Current
	CStatic m_stcACT02CName;
	CStatic m_stcACT02CValue;
	CStatic m_stcACT02CUnit;

	// Actuator 01 NULL Current
	CStatic m_stcACTNULL01Name;
	CEdit m_edtActNull01;
	CStatic m_stcACTNULL01Unit;

	// Actuator 02 NULL Current
	CStatic m_stcACTNULL02Name;
	CEdit m_edtActNull02;
	CStatic m_stcACTNULL02Unit;

	// Actuator 01/02 NULL Current
	CStatic m_stcACTNULL12Name;
	CEdit m_edtActNull12;
	CStatic m_stcACTNULL12Unit;

private:
	void SetVisible();
	bool SetDemand();
	bool SetNullCurrent( CAliasItem *pAlias, CEdit &edtValue );
};
