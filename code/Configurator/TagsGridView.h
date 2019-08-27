#pragma once

#include "BaseGridView.h"      // Added by wangdong 


// CTagsGridView 视图

class CTagsGridView : public CBaseGridView            // CView->CBaseGridView
{
	DECLARE_DYNCREATE(CTagsGridView)

protected:
	CTagsGridView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTagsGridView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	enum Columns { COL_DESC, COL_VARIABLE, COL_NUMBER, COL_REMOTE, COL_LOCAL,
                   COL_RECORD, COL_LAST = COL_RECORD };

	enum UpdateType { NONE, REFRESH, MONITOR } m_eUpdateType;

    virtual void AddSettings(CSettingVector* pSettings, long& lRow, long lDepth);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Popup menu
	afx_msg void OnAliasesRead();
	afx_msg void OnAliasesWrite();

	virtual void RefreshColumnSizes();

	DECLARE_EVENTSINK_MAP()
	void Mygridn_AfterUserResize(long Row, long Col);
	//void Mygridn_AfterCollapse(long Row, short State);     // 待定
	void Mygridn_BeforeEdit(long Row, long Col, BOOL* Cancel);
	void Mygridn_RowColChange();
	void Mygridn_ValidateEdit(long Row, long Col, BOOL* Cancel);
	void Mygridn_MouseDown(short Button, short Shift, float X, float Y);
	//void Mygridn_KeyDown(short* KeyCode, short Shift);     // 建议删除
};


