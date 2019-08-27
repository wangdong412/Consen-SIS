#pragma once

// Added by wangdong  
#include "mytchart.h"
#include "ConfiguratorView.h"        // Derived from CConfiguratorView 

// CTrendView 视图

class CTrendView : public CConfiguratorView                // CView -> CConfiguratorView
{
	DECLARE_DYNCREATE(CTrendView)

protected:
	CTrendView();                       // 动态创建所使用的受保护的构造函数
	virtual ~CTrendView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	enum RadioButton{ RB01, RB02, RB03, RB04 };
	CCriticalSection m_cirsec;

	DWORD m_dwViewHelpID;
	DWORD m_dwTrendTick;
	COleDateTime m_dtLastTime;
	COleDateTime m_dtScrollTime;
	bool m_bAutoScroll;
	bool m_bUserScroll;
	int m_nRBSel;

	CMytchart m_tChart;

	CButton m_btRB01;
	CButton m_btRB02;
	CButton m_btRB03;
	CButton m_btRB04;

	void SetViewCtrl( int nRBSel = 0 );
	void OnUpdateRefresh( void );
	void OnUpdateMonitor( void );

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
    afx_msg void OnRadio3();
	afx_msg void OnRadio4();

public:
	DECLARE_EVENTSINK_MAP()
	void Mytchart_OnScroll();
};


