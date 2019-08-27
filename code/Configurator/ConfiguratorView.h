
// ConfiguratorView.h : interface of the CConfiguratorView class
//

#pragma once      // 防止某个头文件被多次include

class CConfiguratorDoc;
class CMygridn;
class CConfiguratorView : public CScrollView         //CView
{
protected: // create from serialization only
	CConfiguratorView();
	DECLARE_DYNCREATE(CConfiguratorView)

// Attributes
public:
	CConfiguratorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);	

// Implementation
public:
	virtual ~CConfiguratorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();	

private:
	LONG GetGridTotalVisibleRows( CMygridn &grid, std::vector<int> &vec, LONG lPageRows );
};

#ifndef _DEBUG  // debug version in ConfiguratorView.cpp
inline CConfiguratorDoc* CConfiguratorView::GetDocument() const
   { return reinterpret_cast<CConfiguratorDoc*>(m_pDocument); }
#endif

