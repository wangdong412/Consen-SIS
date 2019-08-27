#pragma once

// Added by wangdong
#include "BaseGridView.h"                           // Derived from CBaseGridView
#include "mygridn.h"                                // class CMygridn


// CSettingGridView 视图

class CSettingGridView : public CBaseGridView
{
	DECLARE_DYNCREATE(CSettingGridView)

protected:
	CSettingGridView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSettingGridView();

private:
	enum Columns { COL_NAME,  COL_VARIABLE, COL_NUMBER,  COL_RANGE,
                   COL_UNITS, COL_VALUE,    COL_COMPARE, COL_LAST = COL_COMPARE }; 

	DWORD m_dwViewHelpID;

	virtual void AddSettings( CSettingVector* pSettings, long& lRow, long lDepth );

	// For DSC100
	void SetActuatorCurRange();
	void LimitActuatorCur( long lChangedRow );

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

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Popup menu
	afx_msg void OnCfgAlias();
	afx_msg void OnCfgCmp();
	afx_msg void OnCfgRead();
	afx_msg void OnCfgVariable();
	afx_msg void OnCfgWrite();

	// Grid Event
	DECLARE_EVENTSINK_MAP()
	void Mygridn_AfterUserResize(long Row, long Col);
	void Mygridn_AfterCollapse(long Row, short State);
	void Mygridn_BeforeEdit(long Row, long Col, BOOL* Cancel);
	void Mygridn_ValidateEdit(long Row, long Col, BOOL* Cancel);
	void Mygridn_RowColChange();
	void Mygridn_MouseDown(short Button, short Shift, float X, float Y);
	//void Mygridn_KeyDown(short* KeyCode, short Shift);

private:
	bool IsCheckOwnRange( CSettingItem* pSetting );
	bool DSC100IsCheckOwnRange( CSettingItem* pSetting );
	bool TS350IsCheckOwnRange( CSettingItem* pSetting );
	bool DSC110IsCheckOwnRange( CSettingItem* pSetting );

	void SpecialValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias );
	void DSC100ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias );
	void TS350ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias );
	void DSC110ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias );

public:
   virtual void RefreshColumnSizes();
	
   void ShowCompareColumn( bool bShow );
   bool IsCompareColVisible();
};


