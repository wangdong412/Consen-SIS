#pragma once
#include "ConfiguratorView.h"

// CNonTuningFra 视图
class CSettingItem;
class CAliasItem;

class CNonTuningFra : public CConfiguratorView
{
	DECLARE_DYNCREATE(CNonTuningFra)

public:
	CNonTuningFra();           // 动态创建所使用的受保护的构造函数
	virtual ~CNonTuningFra();

	void SetVisibleFromVersion();
	void Bind();
	void RefreshTuning( bool bConnected );

	virtual BOOL PreTranslateMessage( MSG* pMsg );

private:
	CStatic m_lblStatus;
	CStatic m_lblHide;

	// static label
	CStatic m_lblNullCurrent1;
	CStatic m_lblNullCurrent2;
	CStatic m_lblNullCurrent12;
	CStatic m_lblAmplitude;
	CStatic m_lblFrequency;
	CStatic m_lblSD;
	CStatic m_lblCBS;

	// Edit
	CEdit m_edtCurrent1;
	CEdit m_edtCurrent2;
	CEdit m_edtCurrent12;
	CEdit m_edtAmplitude;
	CEdit m_edtFrequency;
	CEdit m_edtSD;
	CEdit m_edtCBS;

	
	// Unit
	CStatic m_lblUnitCurrent1;
	CStatic m_lblUnitCurrent2;
	CStatic m_lblUnitCurrent12;
	CStatic m_lblUnitAmplitude;
	CStatic m_lblUnitFrequency;
	CStatic m_lblUnitSD;
	CStatic m_lblUnitCBS;

public:
	CAliasItem* m_pAliasCurrent1;
	CAliasItem* m_pAliasCurrent2;
	CAliasItem* m_pAliasCurrent12;
	CAliasItem* m_pAliasAmplitude;
	CAliasItem* m_pAliasFrequency;
	CAliasItem* m_pAliasSD;
	CAliasItem* m_pAliasCBS;

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
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


