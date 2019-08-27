#pragma once

// DSC100
#include "CalInfoDlg.h"
#include "CalMinDlg.h"
#include "CalMaxDlg.h"
#include "CalVerDlg.h"
#include "CalSave.h"

// DSC110
#include "DSC110CalDlg.h"
#include "DSC110VerDlg.h"
#include "DSC110SaveDlg.h"

class CCalibrateBase;

// CCalSheet

class CCalSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCalSheet)

public:
	CCalSheet(UINT nIDCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCalSheet(LPCTSTR pszCaption, CCalibrateBase* pCalibrate, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CCalSheet();

private:
	CCalibrateBase* m_pCalibrate;

public:
	CCalibrateBase* GetCalibrate() const { return m_pCalibrate; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

//=============================================
// DSC100
class CDSC100Sheet : public CCalSheet
{
	DECLARE_DYNAMIC(CDSC100Sheet)

public:
	CDSC100Sheet(UINT nIDCaption, CCalibrateBase* pCalSheet, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDSC100Sheet(LPCTSTR pszCaption, CCalibrateBase* pCalSheet, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

private:
	CCalInfoDlg m_dlgIntro;
	CCalMinDlg  m_dlgMin;
	CCalMaxDlg  m_dlgMax;
	CCalVerDlg  m_dlgVer;
	CCalSave  m_dlgSave;

protected:
	DECLARE_MESSAGE_MAP()
};

//=============================================
// DSC110
class CDSC110Sheet : public CCalSheet
{
	DECLARE_DYNAMIC(CDSC110Sheet)

public:
	CDSC110Sheet(UINT nIDCaption, CCalibrateBase* pCalSheet, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDSC110Sheet(LPCTSTR pszCaption, CCalibrateBase* pCalSheet, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

private:
	CDSC110CalDlg m_dlgCal;
	CDSC110VerDlg  m_dlgVer;
	CDSC110SaveDlg  m_dlgSaveRun;

protected:
	DECLARE_MESSAGE_MAP()
};


