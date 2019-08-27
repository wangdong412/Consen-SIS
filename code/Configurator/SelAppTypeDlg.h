#pragma once
#include "afxcmn.h"


// CSelAppTypeDlg 对话框

class CSelAppTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelAppTypeDlg)

public:
	CSelAppTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelAppTypeDlg();

// 对话框数据
	enum { IDD = IDD_SEL_APPTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_ListCtrl;
	CImageList *m_pImageList;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	int m_nIndex;
};
