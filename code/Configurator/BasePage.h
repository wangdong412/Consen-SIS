#pragma once

class CPropertyPage;
class CBasePageCtl
{
public:
	CBasePageCtl(void);
	~CBasePageCtl(void);

private:
	CFont m_boldFont;
	CFont m_norFont;
	CImage m_imgPassing;
	CImage m_imgPass;
	CImage m_imgUnPass;

public:
	void Init( CPropertyPage* pPage );
	
	const CFont& GetBoldFont()      const { return m_boldFont; }
	const CFont& GetNormalFont()    const { return m_norFont; }
	const CImage& GetPassingImage() const { return m_imgPassing; }
	const CImage& GetPassImage()    const { return m_imgPass; }
	const CImage& GetUnPassImage()  const { return m_imgUnPass; }

	CFont& GetBoldFont()                  { return m_boldFont; }
	CFont& GetNormalFont()                { return m_norFont; }
	CImage& GetPassingImage()             { return m_imgPassing; }
	CImage& GetPassImage()                { return m_imgPass; }
	CImage& GetUnPassImage()              { return m_imgUnPass; }
};

