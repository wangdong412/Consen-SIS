// ValuesView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "ValuesView.h"

// Added by wangdong
#include "MainFrm.h"
#include "ConfiguratorDoc.h"      // class CConfiguratorDoc    
#include <afxctl.h>               // declare CPictureHolder class

// =======================================================================
// Function name  : GridIsRowColSelected
// Description    : Check if cell(Row, Col) is selected
// Return type    :
// Parameters     : True: selected
// Author         : rjvs 2002-03-14 14:54:18 -06:00
// =======================================================================
inline bool GridIsRowColSelected( CMygridn& sGrid, long Row, long Col )
{
	long r1, r2, c1, c2;
	sGrid.GetSelection( &r1, &c1, &r2, &c2 );
    return ((Row >= r1) && (Row <= r2) && (Col >= c1) && (Col <= c2));
}
// =======================================================================

// CValuesView

IMPLEMENT_DYNCREATE(CValuesView, CConfiguratorView)      // CView -> CConfiguratorView

CValuesView::CValuesView()
{

}

CValuesView::~CValuesView()
{
}

BEGIN_MESSAGE_MAP(CValuesView, CConfiguratorView)      // CView -> CConfiguratorView
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TRACE_UNASSIGNED, &CValuesView::OnTraceUnassigned)
	ON_COMMAND(ID_TRACE_LEFTY, &CValuesView::OnTraceLefty)
	ON_COMMAND(ID_TRACE_RIGHTY, &CValuesView::OnTraceRighty)
	ON_COMMAND(ID_TRACE_TRACE1, &CValuesView::OnTraceTrace1)
	ON_COMMAND(ID_TRACE_TRACE2, &CValuesView::OnTraceTrace2)
	ON_COMMAND(ID_TRACE_TRACE3, &CValuesView::OnTraceTrace3)
	ON_COMMAND(ID_TRACE_TRACE4, &CValuesView::OnTraceTrace4)
	ON_COMMAND(ID_TRACE_TRACE5, &CValuesView::OnTraceTrace5)
	ON_COMMAND(ID_TRACE_TRACE6, &CValuesView::OnTraceTrace6)
	ON_COMMAND(ID_TRACE_TRACE7, &CValuesView::OnTraceTrace7)
	ON_COMMAND(ID_TRACE_TRACE8, &CValuesView::OnTraceTrace8)
END_MESSAGE_MAP()

// =======================================================================
/// VSFLEXGRID EVENT
BEGIN_EVENTSINK_MAP(CValuesView, CConfiguratorView)
	ON_EVENT(CValuesView, IDC_VALUESGRID, DISPID_MOUSEDOWN, CValuesView::Mygridn_MouseDown, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
END_EVENTSINK_MAP()
// =======================================================================


// CValuesView 绘图

void CValuesView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}

// CValuesView 诊断

#ifdef _DEBUG
void CValuesView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CValuesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CValuesView 消息处理程序


int CValuesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( HIDR_MONITOR );      // Help ID

	try
	{
	    if( !m_sGrid.Create( lpCreateStruct->lpszClass,
		                 _T(""), 
						 lpCreateStruct->dwExStyle,
						 CRect( lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx, lpCreateStruct->cy),
						 this, 
						 IDC_VALUESGRID
						 ))
	    {
			theApp.LogInfo( L"m_sGrid.Create in CValuesView is failed!" );
		    return -1;
	    }
	
	    // Set Font
	    ApplyFont();

	    // 允许用户修改列宽
	    m_sGrid.put_AllowUserResizing( CMygridn::flexResizeColumns );

	    // 设定表格包含的行，列数
	    m_sGrid.put_Rows( 1 );        // Rows
	    m_sGrid.put_Cols( 2 );        // Columns

	    // 固定的行/列数：固定1行，0列
	    m_sGrid.put_FixedRows( 1 );
	    m_sGrid.put_FixedCols( 0 );

	    // 初始化表格内容
		CString s;
		s.LoadStringW( IDS_NAME );
	    m_sGrid.put_TextMatrix( 0, COL_NAME, s );         // COL_NAME

		s.Empty();
		s.LoadStringW( IDS_VALUE );
	    m_sGrid.put_TextMatrix( 0, COL_VALUE, s );        // COL_VALUE
    
	    // BigSelection：true:点击固定区域时，所有的行列均被选择
	    // Selection: true: Can select ranges of cells with the mouse and keyboard.
	    m_sGrid.put_AllowBigSelection( false );
	    m_sGrid.put_AllowSelection( false );

		// Color
		m_sGrid.put_BackColorBkg( global::clWindow );      // 视图中，实际表格外的其他区域：white
		m_sGrid.put_SheetBorder( global::clWindow );       // 实际表格的边界色：white

	    // 以下属性暂时不理解 ??
	    m_sGrid.put_MergeCells( CMygridn::flexMergeSpill );   // 溢出 
	    m_sGrid.put_OutlineCol( 0 );
	    m_sGrid.put_OutlineBar( CMygridn::flexOutlineBarComplete );   // ？
	    m_sGrid.put_SelectionMode( CMygridn::flexSelectionListBox);   // ?

	    m_sGrid.put_Redraw( CMygridn::flexRDBuffered );  // 设置重画模式，避免闪烁

	    // 如果不可视，则设置为可视
	    if( !m_sGrid.IsWindowVisible() )
	    {
	        m_sGrid.ShowWindow( SW_SHOW );
	    }
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CValuesView::OnCreate!" );

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CValuesView::OnCreate!" );

		return -1;
	}

	return 0;
}


void CValuesView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_sGrid.GetSafeHwnd())
	{
		long ln = m_sGrid.get_Col();
	    m_sGrid.SetWindowPos( &CWnd::wndBottom, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );

		// Get Client Visible Width
		long cw = m_sGrid.get_ClientWidth();

		m_sGrid.put_ColWidth( 0, cw*2/3 );
		m_sGrid.put_ColWidth( 1, cw/3 );
	}
}

// =======================================================================
// Function name  : OnUpdate
// Description    : 
//                 1.判断更新操作：Refresh or Monitor，非法操作时，直接返回；
//                 2.判断文档是否存在：文档不存在时，设定视图行数为1行；
//                 3.文档存在时，根据刷新操作类型，更新视图（设定视图行数，显示xml文件中的内容）
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.07 09:30:00
// =======================================================================
void CValuesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint )
{
	// TODO: 在此添加专用代码和/或调用基类
	try
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

		if((lHint != CConfiguratorDoc::HINT_REFRESH) &&
		   (lHint != CConfiguratorDoc::HINT_MONITOR))
		{
			return;
		}

		if( theApp.GetResetViewFlag())
		{
			m_sGrid.put_Rows( 1L );

            // vol name
			CString s;
			s.LoadStringW( IDS_NAME );
			m_sGrid.put_TextMatrix( 0, COL_NAME, s );         // COL_NAME

			s.Empty();
			s.LoadStringW( IDS_VALUE );
			m_sGrid.put_TextMatrix( 0, COL_VALUE, s );        // COL_VALUE
		}

	    m_sGrid.put_Redraw( CMygridn::flexRDNone );

		size_t size = GetDocument()->m_aMonitorValues->size();
	    if( size > 0 )
	    {
			if( lHint == CConfiguratorDoc::HINT_REFRESH )
			{
			    UpdateRefresh();                // Name Column    
				UpdateMonitor( false );         // Value Column    
			}
			else
			{
				UpdateMonitor( true );          // Value Column
			}
	    }
		else
		{
			 m_sGrid.put_Rows( 1L );
		}	

		// Font
		 if( theApp.GetResetViewFlag())
		 {
			 ApplyFont();
		 }

         m_sGrid.put_Redraw( CMygridn::flexRDBuffered );
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		CString str;
		str.Format( L"Exception occured in CValuesView::OnUpdate - %d", lHint );
		theApp.LogInfo( str );

		return;
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CValuesView::OnUpdate - %d", lHint );
		theApp.LogInfo( str );

		return;
	}
}

// =======================================================================
/// Set Name Column
/// 1.判断文档是否存在，不存在时，设置视图行数为1行，然后返回
/// 2.文档存在时：设置 Name 列
/// 3.判断是否存在变量，如果不存在时：
///                                加粗该行；
///                                分类：SubTotal
///                                OutlineLevel
// =======================================================================
void CValuesView::UpdateRefresh()
{
	if( 0 == GetDocument()->m_aMonitorValues->size())
	{
        m_sGrid.put_Rows( 1L ); 
		return;
	}

	// Rows
	long lRows = GetDocument()->m_aMonitorValues->size() + 1;
	m_sGrid.put_Rows( lRows );

	// Picture Alignment
	m_sGrid.put_Cell( CMygridn::flexcpPictureAlignment, _variant_t( 0 ), _variant_t( 0 ),
		              _variant_t( lRows - 1 ), _variant_t( COL_LAST ), 
					  _variant_t( CMygridn::flexAlignRightTop ));

	// 遍历Vector容器，将xml文件内容显示到视图上
	long lRow = 1L;
	CCfgMonitorValues* pValues = GetDocument()->m_aMonitorValues;

	CCfgMonitorValues::const_iterator it = pValues->begin();
	CCfgMonitorValues::const_iterator eit = pValues->end();
    while( it!= eit )
	{
        const CCfgMonitorValue& p = *it;
		m_sGrid.put_TextMatrix( lRow, COL_NAME, p.GetTitle_C() );   // Set Name Column
		
		CString s = p.GetVariable();
		if( s.IsEmpty() )    // not exists
		{
			// Group aliases together
			m_sGrid.put_IsSubtotal( lRow, true );                                                    
			m_sGrid.put_RowOutlineLevel( lRow, 1 );  

			// Set Font Bold 
			m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t(lRow), _variant_t(0), 
				               _variant_t(lRow), _variant_t(COL_LAST), _variant_t(true));            
		}
		else if( p.GetAlias() == NULL )
		{
            // Monitor value has an alias name, but the document was unable to
            // link to the specified name.
			m_sGrid.put_Cell( CMygridn::flexcpFontItalic, _variant_t(lRow), _variant_t(0), 
				               _variant_t(lRow), _variant_t(COL_LAST), _variant_t(true)); 
		}
		else if( !p.GetAlias()->GetActive())
		{
			// Use a gray color for items that are not being monitored.
			m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t(lRow), _variant_t(0), 
				              _variant_t(lRow), _variant_t(COL_LAST), _variant_t( global::clGray )); 
		}
		else
		{
            m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t(lRow), _variant_t(0), 
				              _variant_t(lRow), _variant_t(COL_LAST), _variant_t( global::clWindowText ));  
		}

		lRow++;
		it++;
	}
        
    // Force a refresh of the trace bitmaps
    UpdateTraceBitmaps();
}

// =======================================================================
// Function name  : UpdateMonitor
// Description    : Set Value Column
// Return type    : void
// Parameters     : bMonitor: true,  Monitor Operation
//                            false, Refresh Operation
// Author         : WangDong 2011.01.07 10:55:00
// =======================================================================
void CValuesView::UpdateMonitor( bool bMonitor )
{
    if( 0 == GetDocument()->m_aMonitorValues->size())
	{
        m_sGrid.put_Rows( 1L ); 
		return;
	}

	CCfgMonitorValues* pValues = GetDocument()->m_aMonitorValues;

	long lRow = 1L;
	bool bChanged = false;

	CCfgMonitorValues::iterator it = pValues->begin();
	CCfgMonitorValues::iterator eit = pValues->end();
	while( it != eit )
	{
		CCfgMonitorValue& p = *it;
		const CAliasItem* pAlias = p.GetAlias();
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			// When Refresh or the remote value was changed: Display the value
			bChanged = p.Update( !bMonitor );  
			if( !bMonitor || bChanged )
			{
				if( pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN )
				{
					_variant_t var_t = (pAlias->remBool() ? CMygridn::flexChecked : CMygridn::flexUnchecked);
					m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t( COL_VALUE ),
						              _variant_t( lRow ), _variant_t( COL_VALUE ), var_t );

					// Font Color
					if( p.GetFontColor() != -1 )
					{
						// 1: set font color
						_variant_t varColor_t = pAlias->remBool() ? p.GetFontColor() : global::clWindowText; 
						if( CConfiguratorApp::DSC100 == theApp.GetProductType())
						{
							// DSC100: 0: set font color
							varColor_t = ( pAlias->remBool() ? global::clWindowText : p.GetFontColor()); 
						}

						m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t( COL_NAME ),
						              _variant_t( lRow ), _variant_t( COL_VALUE ), varColor_t );
					}
				}
				else
				{
					CString s;
					CString strName(pAlias->GetName());
					if( strName.CompareNoCase( L"iCPV" ) == 0)      // control period
					{
						if( CConfiguratorApp::DSC110 == theApp.GetProductType())
						{
							s.Format( L"%.3f", pAlias->remI2()/10.0 );
						}
						else
						{
							s.Format( L"%.2f", pAlias->remI2()/10.0 );
						}
					}
					else
					{
						VariantToString( pAlias->remVar(), s );
					}

					m_sGrid.put_TextMatrix( lRow, COL_VALUE, s );
				}
			}
		}

		lRow++;
		it++;
	}
}

// =======================================================================
// Function name  : ApplyFont
// Description    : Set Font
// Return type    : void
// Parameters     : 
// Author         : WangDong 
// =======================================================================
void CValuesView::ApplyFont( void )
{
	if( m_sGrid.GetSafeHwnd())
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
}

// =======================================================================
// Function name  : Mygridn_MouseDown
// Description    : popupmenu
// Return type    : void
// Parameters     : X,Y: Client, Twips which need to convert to pixels
// Author         : WangDong 
// =======================================================================
void CValuesView::Mygridn_MouseDown(short Button, short Shift, float X, float Y)
{
	if( Button == MK_RBUTTON )
	{
		long lRow = m_sGrid.get_MouseRow();
		long lCol = m_sGrid.get_MouseCol();
		if((lRow > 0) && (lCol >= 0))
		{
			// Check if the clicked cell is within the selected cells
			// If not select it
            if( !GridIsRowColSelected( m_sGrid, lRow, lCol ))
			{
				m_sGrid.Select( lRow, lCol, _variant_t(lRow), _variant_t(lCol));
			}

			CMenu menu;
		    BOOL bLoadMenu = menu.LoadMenuW( IDM_VALUES );
		    if( bLoadMenu != 0 )
		    {
			    CMenu *pSubMenu = NULL;
	            pSubMenu = menu.GetSubMenu( 0 );
			    if( pSubMenu->m_hMenu )
			    {
			        CPoint point(static_cast<int>(X), static_cast<int>(Y));

			        // Convert twips to pixels
			        PointTwipsToPixels( point.x, point.y, point );

					// Convert client to screen
			        ClientToScreen( &point );

					// Update menu items
					UpdateMenuItems( pSubMenu );

					// Show shortcut menu
			        pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_HORPOSANIMATION, point.x, point.y, this );
			    }
		    }
		}//if( lRow > 0 && (lCol >= 0))	
	}
}

// =======================================================================
// Function name  : UpdateMenuItems
// Description    : 1. Bitmap
//                  2. Caption
//                  3. Check Left/Right Y axis;
//                     Enable all menu items
// Return type    : void
// Parameters     : pMenu: 
// Author         : WangDong  2011.02.09 17:50
// =======================================================================
void CValuesView::UpdateMenuItems( CMenu* pMenu )
{
	if( !pMenu->m_hMenu )
	{
		return;
	}

	/// 1. Bitmap
	CBitmap* bitmaps[] = { &m_bitmap1, &m_bitmap2, &m_bitmap3, &m_bitmap4,
	                       &m_bitmap5, &m_bitmap6, &m_bitmap7, &m_bitmap8 };
	
	for( size_t i = 0; i < global::TRACE_COUNT; ++i )
	{
		if( !bitmaps[i]->m_hObject )
	    {
            bitmaps[i]->LoadBitmapW( IDB_TRACE1+i );
	    }

        pMenu->SetMenuItemBitmaps( ID_TRACE_TRACE1+i, MF_BYCOMMAND, bitmaps[i], bitmaps[i] );
	}
	
    /// 2. Caption
	bool bEnabled = false;
	size_t size = GetDocument()->m_aMonitorValues->size();
	if( size > 0 )
	{
		bEnabled = true;

		// Update each menu item's caption
		for( size_t i = 0; i < global::TRACE_COUNT; ++i )
		{
            CString strLable;
			strLable.Format( IDS_TRACE_LABLE, i+1 ); 

			if( i < GetDocument()->GetTraceCount())
			{
				CString strName = GetDocument()->GetTraceName( i );
				if( !strName.IsEmpty())
				{
					strLable.AppendFormat( L" - %s", strName );
				}
			}

			pMenu->ModifyMenuW( ID_TRACE_TRACE1+i, MF_BYCOMMAND | MF_STRING, ID_TRACE_TRACE1+i, strLable );
		}
	}

	/// 3. Check and Enable
	// Check if the value row is currently being traced
	long lRow = m_sGrid.get_Row();
	CCfgTrace* pTrace = GetRowTrace( lRow );
	bool bTrace = CheckReadPtr( pTrace, sizeof(CCfgTrace));
    bool bLeft = false;
    bool bRight = false;

    if( bTrace )
    {
        bLeft = (pTrace->GetAxis() == 0);
        bRight = (pTrace->GetAxis() == 1);
    }
	
	// Check Left/Right Y axis
	UINT uLeftCheck = bLeft ? MF_CHECKED : MF_UNCHECKED;
	UINT uRightCheck = bRight ? MF_CHECKED : MF_UNCHECKED;

	pMenu->CheckMenuItem( ID_TRACE_LEFTY, MF_BYCOMMAND | uLeftCheck );
	pMenu->CheckMenuItem( ID_TRACE_RIGHTY, MF_BYCOMMAND | uRightCheck );

	// Enable Unsigned, Left/Right Y axis
	UINT uEnable = bTrace ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	pMenu->EnableMenuItem( ID_TRACE_UNASSIGNED, MF_BYCOMMAND | uEnable );
	pMenu->EnableMenuItem( ID_TRACE_LEFTY, MF_BYCOMMAND | uEnable );
	pMenu->EnableMenuItem( ID_TRACE_RIGHTY, MF_BYCOMMAND | uEnable );
	
	// Enable/Disable Trace 1 - 8
	UINT uTraceEnable = bEnabled ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	for( size_t i = 0; i < global::TRACE_COUNT; ++i )
	{
        pMenu->EnableMenuItem( ID_TRACE_TRACE1+i, MF_BYCOMMAND | uTraceEnable );
	}
}

// -----------------------------------------------------------------------
// Function name  : GetRowTrace
// Description    : 1. Caption
//                  2. Enable
// Return type    : void
// Parameters     : pMenu
// Author         : WangDong  2011.02.09 15:00
// -----------------------------------------------------------------------
CCfgTrace* CValuesView::GetRowTrace( long lRow )
{
	CCfgTrace* Result = NULL;
    CAliasItem* pAlias = GetRowAlias( lRow );

    if( pAlias != NULL )
    {
        CCfgTraceArray* pTraces = GetDocument()->m_aTraces;
        if (CheckWritePtr(pTraces, sizeof(CCfgTraceArray)))
        {
            // Lock the trace array for reading, to search for the trace name.
            CSingleLock sl( &pTraces->m_cirsec );
		    sl.Lock();
		    ASSERT( sl.IsLocked());

            CCfgTraceArray::iterator it = std::find_if(
                pTraces->begin(),
                pTraces->end(),
                CCfgTraceFind( pAlias->GetName())
            );

            if (it != pTraces->end())
            {
                Result = &(*it);
            }

			sl.Unlock();
        }
    }

    return Result;  
}

/// <summary>Returns the value alias for a specified row in the grid.</summary>
/// <param name="Row">Row of the value grid.</param>
/// <return>The pointer to the alias item if it exists, otherwise NULL.</return>
/// <created>rjvs 2002-11-13 11:27:12 (UTC-6:00)</created>
CAliasItem* CValuesView::GetRowAlias( long lRow )
{
    CAliasItem* Result = NULL;
    CCfgMonitorValue* pValue = GetRowMonitorValue( lRow );

    if (pValue != NULL)
    {
        Result = pValue->GetAlias();
    }

    return Result;
}


/// <summary> Returns the monitor value pointer for a specified row in the grid. </summary>
/// <param name="lRow"> Row of the value grid. </param>
/// <return> The pointer to the monitor value, otherwise NULL. </return>
/// <created> rjvs 2002-11-13 11:33:57 (UTC-6:00) </created>
CCfgMonitorValue* CValuesView::GetRowMonitorValue(long lRow)
{
    CCfgMonitorValue* Result = NULL;

	size_t size = GetDocument()->m_aMonitorValues->size();
    if( size > 0 )
    {
        CCfgMonitorValues* pValues = GetDocument()->m_aMonitorValues;
        if((lRow > 0) && (pValues != NULL))
        {
            size_t index = lRow - 1;
            if( index < pValues->size())
            {
                Result = &pValues->at(index);
            }
        }
    }

    return Result;
}

// =======================================================================
// Function name  : OnTracexxx
// Description    : Click event of popup menu items                 
// Return type    : void
// Parameters     : none 
// Author         : WangDong  2011.02.10 11:40
// =======================================================================
void CValuesView::OnTraceUnassigned()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias( lRow );

    if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
	    GetDocument()->RemoveTraceName( pAlias->GetName());
        UpdateTraceBitmaps();	     // Update cell picture
	}
}

/// Assigns a selected trace to an axis
void CValuesView::OnTraceLefty()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
	CCfgTrace* pTrace = GetRowTrace( lRow );

    if( CheckWritePtr( pTrace, sizeof(CCfgTrace)))
    {
		int nAxis = 0;
		if( nAxis != pTrace->GetAxis())
		{
			GetDocument()->SetModifiedFlag();                   // Set document dirty
		    pTrace->SetAxis( nAxis );
		    GetDocument()->UpdateAllViews(NULL, CConfiguratorDoc::HINT_TRACE, NULL);
		}
	}
}


void CValuesView::OnTraceRighty()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
	CCfgTrace* pTrace = GetRowTrace( lRow );

    if( CheckWritePtr( pTrace, sizeof(CCfgTrace)))
    {
		int nAxis = 1;
		if( nAxis != pTrace->GetAxis())
		{
			GetDocument()->SetModifiedFlag();                   // Set document dirty
		    pTrace->SetAxis( nAxis );
		    GetDocument()->UpdateAllViews(NULL, CConfiguratorDoc::HINT_TRACE, NULL);
		}
	}
}


void CValuesView::OnTraceTrace1()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName( pAlias->GetName(), 0 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace2()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName( pAlias->GetName(), 1 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace3()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 2 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace4()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 3 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace5()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 4 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace6()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 5 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace7()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 6 );
        UpdateTraceBitmaps();        // Update cell picture
	}
}


void CValuesView::OnTraceTrace8()
{
	// TODO: 在此添加命令处理程序代码
	long lRow = m_sGrid.get_Row();
    CAliasItem* pAlias = GetRowAlias(lRow);

    if (CheckReadPtr(pAlias, sizeof(CAliasItem)))
    {
        GetDocument()->AddTraceName(pAlias->GetName(), 7 );
        UpdateTraceBitmaps();       // Update cell picture
	}
}

// =======================================================================
/************* Bitmaps: Changed by wangdong 2011.02.10 14:30 ************/
/// <summary>  Update cell picture: Clear cell picture when user click Unsigned
///                                 Trace bitmaps according to the Trace index user clicked
/// </summary>
/// <created>
///   rjvs 2002-04-05 14:29:09 (UTC-6:00)</created>
void CValuesView::UpdateTraceBitmaps( void )
{
	if( GetDocument()->m_aMonitorValues->size() == 0 )
	{
        return;
	}

	if( m_sGrid.get_Rows() <= 1 )
	{
        return;
	}
	/*
    if (MTrendPictures.size() < global::TRACE_COUNT)
        CreateTraceBitmaps();*/

    // Clear the existing pictures...
    {
		COleVariant vPic;
		V_VT(&vPic) = VT_DISPATCH;
		V_DISPATCH(&vPic) = NULL;
		m_sGrid.put_Cell( CMygridn::flexcpPicture, 
					      _variant_t(0),                    _variant_t(COL_NAME),
					      _variant_t(m_sGrid.get_Rows()-1), _variant_t(COL_NAME), vPic );
    }

    CCfgTraceArray* pTraces = GetDocument()->m_aTraces;
    CCfgMonitorValues* pValues = GetDocument()->m_aMonitorValues;

    // Match items in the trace vector with monitored item vector
    size_t index = 0;
    CCfgTraceArray::const_iterator  it = pTraces->begin();
    CCfgTraceArray::const_iterator eit = pTraces->end();

    while (it != eit)
    {
        CCfgMonitorValues::iterator f = std::find_if(
            pValues->begin(),
            pValues->end(),
            CCfgMonitorValueFind( it->GetName_C())
        );

        if((f != pValues->end()) && (f->GetAlias() != NULL))
        {
            long lRow = std::distance( pValues->begin(), f ) + 1;

			CPictureHolder pic;
			pic.CreateFromBitmap( IDB_TRACE1 + index );
		    COleVariant vPic;
		    V_VT(&vPic) = VT_DISPATCH;
			V_DISPATCH(&vPic) = pic.GetPictureDispatch();
		    m_sGrid.put_Cell( CMygridn::flexcpPicture, 
					          _variant_t(lRow), _variant_t(COL_NAME),
					          _variant_t(lRow), _variant_t(COL_NAME), vPic );
        }

        ++it;
        ++index;
    }
}

// =======================================================================
/* <summary>
///   Updates the grid row heights based on a new typeface.</summary>
/// <param name="pFont">
///   The new font object.</param>
/// <created>
///   rjvs 2002-10-15 10:32:37 (UTC-6:00)</created>
// =======================================================================
void TAlarmsForm::OnUpdateFont(TFont* pFont)
{
    inherited::OnUpdateFont(pFont);

    // Apply the new typeface to the grid
    MyGrid->Font->Name = pFont->Name;
    MyGrid->Font->Size = pFont->Size;
    MyGrid->Font->Style = pFont->Style;
}*/

//===========================================================================
/// 暂定删除此函数
/* <summary>
///   </summary>
/// <created>
///   rjvs 2002-04-05 13:59:05 (UTC-6:00)</created>
void CValuesView::CreateTraceBitmaps( void )
{
    TMenuItem *pMenuItems[] = { mnuPopupTrace1, mnuPopupTrace2,
                                mnuPopupTrace3, mnuPopupTrace4,
                                mnuPopupTrace5, mnuPopupTrace6,
                                mnuPopupTrace7, mnuPopupTrace8};

    if (MTrendPictures.size() == 0)
    {
        MTrendPictures.resize(global::TRACE_COUNT);
        for (size_t i = 0; i < global::TRACE_COUNT; ++i)
        {
            TColor nColor = global::TraceColors[i];
            if (GetDocument() != NULL)
            {
                if ( GetDocument()->IsTraceIndexAssigned(i) )
                {
                    // Use the specified trace color
                    TColor nTemp = GetDocument()->GetTraceColor(i);
                    if (nTemp != TCfgTrace::clTraceDefault)
                    {
                        nColor = nTemp;
                    }
                }
            }

            Graphics::TPicture* pPicture = new Graphics::TPicture();
            Graphics::TBitmap* pBitmap = pPicture->Bitmap;
            pBitmap->Height = 16;
            pBitmap->Width = 16;

            TCanvas* pCanvas = pBitmap->Canvas;
            pCanvas->Brush->Color = nColor;
            pCanvas->FillRect(TRect(0, 0, 16, 16));
            pCanvas->Font->Name = "Arial";
            pCanvas->Font->Size = 8; // TODO: Scale properly

            AnsiString s = IntToStr(i+1);
            pCanvas->TextOut(6, 1, s);

            // Add bitmap to imagelist
            MyImageList->Add(pBitmap, NULL);

            // Set popup menu item bitmaps
            pMenuItems[i]->ImageIndex = i;

            // TODO: Make IPictureDisp's out of each bitmap
            _di_IPictureDisp pVal;
            GetOlePicture(pPicture, pVal);
            MTrendPictures[i] = pVal; // push_back causes an exception
            SAFE_DELETE(pPicture);
        }
    }
}*/
//=========================================================================

