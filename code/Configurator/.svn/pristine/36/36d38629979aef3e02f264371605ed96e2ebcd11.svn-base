#pragma once


// CDSC110SaveDlg 对话框
class CBasePageCtl;
class CDSC110Calibrate;
class CDSC110SaveDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CDSC110SaveDlg)

public:
	CDSC110SaveDlg();
	virtual ~CDSC110SaveDlg();

// 对话框数据
	enum { IDD = IDD_DSC110CAL_SAVE };

private:
	CBasePageCtl* m_pBaseCtl;
	CDSC110Calibrate* m_pCalibrate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL OnWizardFinish();
	virtual BOOL OnSetActive();
	virtual void OnCancel();
};
