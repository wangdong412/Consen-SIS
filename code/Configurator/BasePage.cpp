#include "StdAfx.h"
#include "BasePage.h"


CBasePageCtl::CBasePageCtl(void)
{
}


CBasePageCtl::~CBasePageCtl(void)
{
	m_boldFont.DeleteObject();
    m_norFont.DeleteObject();
	m_imgPassing.ReleaseGDIPlus();
	m_imgPass.ReleaseGDIPlus();
	m_imgUnPass.ReleaseGDIPlus();
}

void CBasePageCtl::Init( CPropertyPage* pPage )
{
	/// Font
	// Create Font
	CClientDC dc( pPage );
	LOGFONT boldlf;
	memset( &boldlf, 0, sizeof(LOGFONT));
	boldlf.lfHeight = ::MulDiv( 11,-dc.GetDeviceCaps( LOGPIXELSY ), 72 );
	boldlf.lfWeight = 700;     // bold                
	::StrCpyNW( boldlf.lfFaceName, L"Times New Roman", 32 );
    m_boldFont.CreateFontIndirectW( &boldlf );

	LOGFONT norlf;
	memset( &norlf, 0, sizeof(LOGFONT));
	norlf.lfHeight = ::MulDiv( 11,-dc.GetDeviceCaps( LOGPIXELSY ), 72 );
	norlf.lfWeight = 400;     // normal                
	::StrCpyNW( norlf.lfFaceName, L"Times New Roman", 32 );
    m_norFont.CreateFontIndirectW( &norlf );

	/// Image
	if( S_OK == m_imgPassing.Load( L"res\\Calibrate\\Cal_Passing.bmp" ))
	{
	    m_imgPassing.SetTransparentColor( RGB(255,0,255));
	}

	if( S_OK == m_imgPass.Load( L"res\\Calibrate\\Cal_Pass.bmp" ))
	{
	    m_imgPass.SetTransparentColor( RGB(255,0,255));
	}

	if( S_OK == m_imgUnPass.Load( L"res\\Calibrate\\Cal_UnPass.bmp" ))
	{
	    m_imgUnPass.SetTransparentColor( RGB(255,0,255));
	}
}