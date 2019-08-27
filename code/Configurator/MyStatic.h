#pragma once


// CMyStatic

class CMyStatic : public CStatic
{
	DECLARE_DYNAMIC(CMyStatic)

public:
	CMyStatic();
	CMyStatic( COLORREF crTextColor, COLORREF crBKColor )
		:m_crTextColor( crTextColor )
		,m_crBKColor( crBKColor )
	{
	}
	
	virtual ~CMyStatic();

private:
	COLORREF m_crTextColor;     // Text
	COLORREF m_crBKColor;       // Text Background Color
	CBrush m_BKBrush;           // Control Background Brush 

public:
	void SetTextColor( COLORREF crTextColor )  { m_crTextColor = crTextColor; }
	void SetBKColor( COLORREF crBKColor )      { m_crBKColor = crBKColor; }

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};


