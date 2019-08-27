#pragma once



// CTuneFraForm 窗体视图

class CTuneFraForm : public CFormView
{
	DECLARE_DYNCREATE(CTuneFraForm)

//protected:
public:
	CTuneFraForm();           // 动态创建所使用的受保护的构造函数
	virtual ~CTuneFraForm();

public:
	enum { IDD = IDD_TUNE_FRA };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


