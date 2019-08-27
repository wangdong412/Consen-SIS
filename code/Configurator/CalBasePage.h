#pragma once


// CCalBasePage 对话框

class CCalBasePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCalBasePage)

public:
	CCalBasePage();
	virtual ~CCalBasePage();

// 对话框数据
	enum { IDD = IDD_CALBASEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
