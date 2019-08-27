#pragma once
#include "ConfiguratorView.h"             // Added for Derived from CConfiguratorView

// CSettingTreeView 视图

class CSettingVector;
class CSettingItem;
class CSettingTreeView : public CConfiguratorView             // CView
{
	DECLARE_DYNCREATE(CSettingTreeView)

protected:
	CSettingTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSettingTreeView();

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

private:
	int m_nViewType;             // View Type
	DWORD m_dwViewHelpID;        // View HelpID
	CTreeCtrl m_TreeCtrl;        // TreeCtrl
	CFont m_ctrlFont;
	CSettingItem *m_pPreSelData; // Only for TS350

	inline CSettingVector* GetViewSettings( int nViewType ) const;

	void AddSettings( CSettingVector* pSettings, HTREEITEM hParent );
	void ApplyFont( void );

public:
	enum eViewType{ SETTING_GRID, TAGS_GRID };

	int GetViewType()                         const { return m_nViewType; }
	void SetViewType( int nViewType )         {  m_nViewType = nViewType; }

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void TreeCtrl_OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void TreeCtrl_OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);

	CSettingItem* GetPreSelItem()                  const { return m_pPreSelData; }
	void SetPreSelItem( CSettingItem* pSelItem )   { m_pPreSelData = pSelItem; }
};


