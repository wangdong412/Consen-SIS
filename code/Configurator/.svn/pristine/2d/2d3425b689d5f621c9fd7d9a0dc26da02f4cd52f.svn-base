#pragma once
#include "ConfiguratorView.h"
#include "MyStatic.h"

// CTuneFraView 视图
#include "ConfiguratorDoc.h"

class CSettingItem;
class CAliasItem;
class CTuneFraView : public CConfiguratorView    
{
	DECLARE_DYNCREATE(CTuneFraView)

public:
	CTuneFraView();                              // 动态创建所使用的受保护的构造函数
	virtual ~CTuneFraView();

	void BindSetting( CSettingItem* pSetting );
	void RefreshTuning( bool bConnected );

	virtual BOOL PreTranslateMessage( MSG* pMsg );

private:
	/// Controls
	// Static
	CStatic m_lblStatus;
	CStatic m_lblPB;
	CStatic m_lblReset;
	CStatic m_lblRate;
	CStatic m_lblHide;

	// Edit
	CEdit m_edtPB;
	CEdit m_edtReset;
	CEdit m_edtRate;

	// Button
    CButton m_btnPBDnFast;
	CButton m_btnPBDn;
	CButton m_btnPBUp;
	CButton m_btnPBUpFast;

	CButton m_btnResetDnFast;
	CButton m_btnResetDn;
	CButton m_btnResetUp;
	CButton m_btnResetUpFast;

	CButton m_btnRateDnFast;
	CButton m_btnRateDn;
	CButton m_btnRateUp;
	CButton m_btnRateUpFast;

	/// Bitmaps
	CBitmap m_bmpDown;
	CBitmap m_bmpDownFast;
	CBitmap m_bmpUp;
	CBitmap m_bmpUpFast;

public:
	CAliasItem* m_pAliasPB;
	CAliasItem* m_pAliasReset;
	CAliasItem* m_pAliasRate;

	CSettingItem* m_pSettingPB;
	CSettingItem* m_pSettingReset;
	CSettingItem* m_pSettingRate;

	std::auto_ptr<CAliasMap> m_pAliases;

private:
	void TuningValue( CAliasItem *pAlias, CSettingItem *pSetting, CEdit *pEdit, float fTuneUnit );

public:
	virtual void OnDraw(CDC* pDC);               // 重写以绘制该视图
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
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	//afx_msg void OnPaint();
	afx_msg void OnPBDnFastClicked();
	afx_msg void OnPBDnClicked();
	afx_msg void OnPBUpClicked();
	afx_msg void OnPBUpFastClicked();
	afx_msg void OnResetDnFastClicked();
	afx_msg void OnResetDnClicked();
	afx_msg void OnResetUpClicked();
	afx_msg void OnResetUpFastClicked();
	afx_msg void OnRateDnFastClicked();
	afx_msg void OnRateDnClicked();
	afx_msg void OnRateUpClicked();
	afx_msg void OnRateUpFastClicked();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


