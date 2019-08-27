#pragma once


// CCalInfoDlg 对话框
class CDSC100Calibrate;
class CCalInfoDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CCalInfoDlg)

public:
	CCalInfoDlg();
	virtual ~CCalInfoDlg();

// 对话框数据
	enum { IDD = IDD_CAL_INTRO };

private:
	CFont m_boldFont;
	CFont m_norFont;
	CImage m_imgPassing;
	CImage m_imgUnPass;

	CDSC100Calibrate* m_pCalibrate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnPaint();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual void OnCancel();
};
