// HistoryValueView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "HistoryValueView.h"

#include "ConfiguratorDoc.h"
#include "HistoryViewData.h"
#include <afxctl.h>               // declare CPictureHolder class

// CHistoryValueView

IMPLEMENT_DYNCREATE(CHistoryValueView, CConfiguratorView)

CHistoryValueView::CHistoryValueView()
{

}

CHistoryValueView::~CHistoryValueView()
{
}

BEGIN_MESSAGE_MAP(CHistoryValueView, CConfiguratorView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// =======================================================================
/// VSFLEXGRID EVENT
BEGIN_EVENTSINK_MAP(CHistoryValueView, CConfiguratorView)
	ON_EVENT(CHistoryValueView, IDC_BASEGRID, 18, CHistoryValueView::Mygridn_AfterUserResize, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


// CHistoryValueView 绘图

void CHistoryValueView::OnDraw(CDC* pDC)
{
	CConfiguratorDoc* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CHistoryValueView 诊断

#ifdef _DEBUG
void CHistoryValueView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHistoryValueView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHistoryValueView 消息处理程序
int CHistoryValueView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	try
	{
		if( !m_sGrid.Create( lpCreateStruct->lpszClass,
		                 _T(""), 
						 lpCreateStruct->dwExStyle,
						 CRect( lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx, lpCreateStruct->cy),
						 this, 
						 IDC_BASEGRID
						 ))
	    {
			theApp.LogInfo( L"m_sGrid.Create in CHistoryValueView is failed!" );
		}

		m_sGrid.put_Redraw( CMygridn::flexRDNone );

		// Set Font
	    ApplyFont();

	    // Allow Resize Column
	    m_sGrid.put_AllowUserResizing( CMygridn::flexResizeColumns );

	    // Set Rows and Columns
	    m_sGrid.put_Rows( 1 );        // Rows
	    m_sGrid.put_Cols( 4 );        // Columns

	    // Fixed one Row and zero column
	    m_sGrid.put_FixedRows( 1 );
	    m_sGrid.put_FixedCols( 0 );

	    // Initialize
		CString s;
		s.LoadStringW( IDS_DES );
	    m_sGrid.put_TextMatrix( 0, COL_DESC, s );        

		s.LoadStringW( IDS_VARIABLE );
	    m_sGrid.put_TextMatrix( 0, COL_TAGNAME, s );   

		s.LoadStringW( IDS_NUMBER );
	    m_sGrid.put_TextMatrix( 0, COL_ALIAS, s );

		s.LoadStringW( IDS_SERIES );
	    m_sGrid.put_TextMatrix( 0, COL_SERIES, s );
    
	    // BigSelection：true, 点击固定区域时，所有的行列均被选择
	    // Selection: true, Can select ranges of cells with the mouse and keyboard.
	    m_sGrid.put_AllowBigSelection( false );
	    m_sGrid.put_AllowSelection( false );

		// Color
		m_sGrid.put_BackColorBkg( global::clWindow );      
		m_sGrid.put_SheetBorder( global::clWindow );

	    m_sGrid.put_MergeCells( CMygridn::flexMergeSpill );
	    m_sGrid.put_OutlineCol( 0 );
	    m_sGrid.put_OutlineBar( CMygridn::flexOutlineBarComplete ); 
	    m_sGrid.put_SelectionMode( CMygridn::flexSelectionListBox);

		// Column width
		for( int i = 0; i < global::HISTORY_GRID_COLUMNS; ++i )
		{
			if( global::HisGridColWidths[i] > 0 )
			{
				m_sGrid.put_ColWidth( i, global::HisGridColWidths[i] );
			}
		}

		// Redraw Mode: Avoid Flicker 
	    m_sGrid.put_Redraw( CMygridn::flexRDBuffered ); 

		if( !m_sGrid.IsWindowVisible() )
	    {
	        m_sGrid.ShowWindow( SW_SHOW );
	    }
	}
	catch( COleException &e )
	{
		e.ReportError();
		e.Delete();

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception was throwed in CHistoryValueView::OnCreate!");

		return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : ApplyFont
// Description    : Set Font
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 15:20:00
// =======================================================================
void CHistoryValueView::ApplyFont( void )
{ 
	m_sGrid.put_FontName( (LPCTSTR)global::FontName );    
	m_sGrid.put_FontSize( global::FontSize ); 

	// Font Name
	m_sGrid.put_Cell( CMygridn::flexcpFontName, _variant_t( 0L ), _variant_t( 0L ), 
				      _variant_t( m_sGrid.get_Rows()-1 ), _variant_t( m_sGrid.get_Cols()-1 ), _variant_t(global::FontName));

	// Font Size
	m_sGrid.put_Cell( CMygridn::flexcpFontSize, _variant_t( 0L ), _variant_t( 0L ), 
				      _variant_t( m_sGrid.get_Rows()-1 ), _variant_t(m_sGrid.get_Cols()-1), _variant_t(global::FontSize));
}

// =======================================================================
// Function name  : OnSize
// Description    : Resize
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 15:30:00
// =======================================================================
void CHistoryValueView::OnSize(UINT nType, int cx, int cy)
{
	CConfiguratorView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码 
	if( m_sGrid.GetSafeHwnd())
	{
		m_sGrid.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
		RefreshColumnSizes();
	}
}

void CHistoryValueView::RefreshColumnSizes()
{
    if( m_sGrid.GetSafeHwnd())
	{	
		for( int i = 0; i < global::HISTORY_GRID_COLUMNS; ++i )
		{
			global::HisGridColWidths[i] = m_sGrid.get_ColWidth(i);
		}
	}
}

// =======================================================================
// Function name  : OnUpdate
// Description    : 1.Switch Product Type, Language Type or App Type;
//                  2.
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 15:40:00
// =======================================================================
void CHistoryValueView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if( !m_sGrid.GetSafeHwnd())
	{
		return;
	}

	if( lHint == CConfiguratorDoc::HINT_FONT )
	{
		ApplyFont();
		return;
	}

	/* Switch Product Type, Language Type or App Type
	if( theApp.GetResetViewFlag())
	{
		m_sGrid.put_Rows( 1L );

		// Vol name for switch language
		CString s;
		s.LoadStringW( IDS_DES );
		m_sGrid.put_TextMatrix( 0, COL_DESC, s );        

		s.LoadStringW( IDS_VARIABLE );
		m_sGrid.put_TextMatrix( 0, COL_TAGNAME, s );   

		s.LoadStringW( IDS_NUMBER );
		m_sGrid.put_TextMatrix( 0, COL_ALIAS, s );

		s.LoadStringW( IDS_SERIES );
		m_sGrid.put_TextMatrix( 0, COL_SERIES, s );

		// Column width for switch product
		for( int i = 0; i < global::HISTORY_GRID_COLUMNS; ++i )
		{
			if( global::HisGridColWidths[i] > 0 )
			{
				m_sGrid.put_ColWidth( i, global::HisGridColWidths[i] );
			}
		}
	}*/

	if((CConfiguratorDoc::HIS_CLEAR == lHint) ||
		(CConfiguratorDoc::HIS_INIT == lHint) ||
	   (CConfiguratorDoc::HIS_DYNUPDATE == lHint))
	{
		m_sGrid.put_Redraw( CMygridn::flexRDNone );
	
		UpdateHisViewDatas();
		ApplyFont();

		m_sGrid.put_Redraw( CMygridn::flexRDBuffered );
	}
}

// =======================================================================
// Function name  : UpdateHisViewDatas
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.02 10:00:00
// =======================================================================
void CHistoryValueView::UpdateHisViewDatas()
{
	CHistoryViewDatas *pHisViewDatas = GetDocument()->m_aHisViewDatas;
	ASSERT( pHisViewDatas != NULL );
	if( pHisViewDatas != NULL )
	{
		long Row = 1;
		m_sGrid.put_Rows( Row+pHisViewDatas->Size());

		for( size_t i=0; i < pHisViewDatas->Size(); i++ )
		{
			CHistoryViewData *data = pHisViewDatas->GetHisViewData(i);
			if((data != NULL) && ( data->GetFileData() != NULL))
			{
				m_sGrid.put_TextMatrix( Row, COL_DESC, data->GetDesc());       // Desc
				m_sGrid.put_TextMatrix( Row, COL_TAGNAME, data->GetTagName()); // TagName
				m_sGrid.put_TextMatrix( Row, COL_ALIAS, data->GetAlias_s());   // Alias
				SetColorPicture( Row, data->GetSeriesIndex());

				++Row;
			}
		}

		m_sGrid.put_Rows( Row );
	}
}

// =======================================================================
// Function name  : SetColorPicture
// Description    : Row
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.02 10:30:00
// =======================================================================
void CHistoryValueView::SetColorPicture( long Row, size_t index )
{
	ASSERT( index < 8 );
	if( index >= 8 )
	{
		return;
	}

	// Clear the existing pictures...
	COleVariant vPic;
	V_VT(&vPic) = VT_DISPATCH;
	V_DISPATCH(&vPic) = NULL;
	m_sGrid.put_Cell( CMygridn::flexcpPicture, 
					    _variant_t(Row), _variant_t(COL_SERIES),
					    _variant_t(Row), _variant_t(COL_SERIES), vPic );

	CPictureHolder pic;
	pic.CreateFromBitmap( IDB_TRACE1 + index );
	V_DISPATCH(&vPic) = pic.GetPictureDispatch();

	m_sGrid.put_Cell( CMygridn::flexcpPicture, 
					    _variant_t(Row), _variant_t(COL_SERIES),
					    _variant_t(Row), _variant_t(COL_SERIES), vPic );
}


// =======================================================================
/// VSFLEXGRID EVENT
// =======================================================================
// Function name  : Mygridn_AfterUserResize
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 15:50:00
// =======================================================================
void CHistoryValueView::Mygridn_AfterUserResize(long Row, long Col)
{
    RefreshColumnSizes();
}