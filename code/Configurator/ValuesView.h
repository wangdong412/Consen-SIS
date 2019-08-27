#pragma once

// Added by wangdong
#include "mygridn.h"                 // For FlexGrid ActiveX control
#include "ConfiguratorView.h"        // Derived from CConfiguratorView 

// CValuesView 视图
class CCfgTrace;
class CAliasItem;
class CCfgMonitorValue;
class CValuesView : public CConfiguratorView          // CView -> CConfiguratorView
{
	DECLARE_DYNCREATE(CValuesView)

protected:
	CValuesView();           // 动态创建所使用的受保护的构造函数
	virtual ~CValuesView();

public:
	// virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	enum Columns{ COL_NAME, COL_VALUE, COL_LAST = COL_VALUE };

	CMygridn m_sGrid;

	// Update View
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateRefresh( void );                        // Update Name Column, and Check all parameters
	void UpdateMonitor( bool bMonitor = false );       // Update Value Column

	// Update Menu Items
	void UpdateMenuItems( CMenu* pMenu );
	CCfgTrace* GetRowTrace( long lRow );
	CAliasItem* GetRowAlias( long lRow );
	CCfgMonitorValue* GetRowMonitorValue(long lRow);

	// Bitmaps
	CBitmap m_bitmap1;
	CBitmap m_bitmap2;
	CBitmap m_bitmap3;
	CBitmap m_bitmap4;
	CBitmap m_bitmap5;
	CBitmap m_bitmap6;
	CBitmap m_bitmap7;
	CBitmap m_bitmap8;
	void CreateTraceBitmaps( void );
	void UpdateTraceBitmaps( void );

	void ApplyFont( void );

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnDraw(CDC* /*pDC*/);

	DECLARE_EVENTSINK_MAP()
	void Mygridn_MouseDown(short Button, short Shift, float X, float Y);
	afx_msg void OnTraceUnassigned();
	afx_msg void OnTraceLefty();
	afx_msg void OnTraceRighty();
	afx_msg void OnTraceTrace1();
	afx_msg void OnTraceTrace2();
	afx_msg void OnTraceTrace3();
	afx_msg void OnTraceTrace4();
	afx_msg void OnTraceTrace5();
	afx_msg void OnTraceTrace6();
	afx_msg void OnTraceTrace7();
	afx_msg void OnTraceTrace8();
};


