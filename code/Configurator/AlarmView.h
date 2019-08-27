#pragma once

// Added by wangdong
#include "mygridn.h"                  // For FlexGrid ActiveX control
#include "ConfiguratorView.h"        // Derived from CConfiguratorView               

// CAlarmView 视图

class CAlarmView : public CConfiguratorView          // CView -> CConfiguratorView
{
	DECLARE_DYNCREATE(CAlarmView)

protected:
	CAlarmView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAlarmView();

public:
//	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	enum Columns{ COL_NAME, COL_VALUE, COL_LAST = COL_VALUE };

	CMygridn m_sGrid;

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint );
	void UpdateRefresh( void );                          // 更新 Name 列，及一些判断
	void UpdateMonitor( bool bMonitor = false );         // 更新 Value 列
	void ApplyFont( void );
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnDraw(CDC* /*pDC*/);
};


