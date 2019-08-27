#pragma once
#include "afxext.h"


class CMySplitterWnd : public CSplitterWnd
{
public:
	CMySplitterWnd(void);
	~CMySplitterWnd(void);

private:
	int m_cx;
	int m_cy;
	int m_cxSplitterBorder;
	int m_cySplitterBorder;

public:
	void HideSplitter();
	void ShowSplitter();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

