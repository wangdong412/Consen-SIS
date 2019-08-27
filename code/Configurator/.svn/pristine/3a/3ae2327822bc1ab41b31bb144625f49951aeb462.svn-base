// AlarmView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "AlarmView.h"

// Added by wangdong
#include "MainFrm.h"
#include "ConfiguratorDoc.h"        // For GetDocument() derived from CConfiguratorView 
#include "Configurator.h"

// 使用 _bstr_t： C++ 对BSTR 的封装
#include "comutil.h"
#pragma comment(lib, "comsupp.lib")

// CAlarmView

IMPLEMENT_DYNCREATE(CAlarmView, CConfiguratorView)      // CView -> CConfiguratorView

CAlarmView::CAlarmView()
{
}

CAlarmView::~CAlarmView()
{
}

BEGIN_MESSAGE_MAP(CAlarmView, CConfiguratorView)      // CView -> CConfiguratorView
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAlarmView 绘图

void CAlarmView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	//CConfiguratorDoc* pDoc = GetDocument();
}


// CAlarmView 诊断

#ifdef _DEBUG
void CAlarmView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAlarmView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAlarmView 消息处理程序


int CAlarmView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( HIDR_ALARM );      // Help ID

	try
	{
	    if( !m_sGrid.Create( lpCreateStruct->lpszClass,
		                 _T(""), 
						 lpCreateStruct->dwExStyle,
						 CRect( lpCreateStruct->x,lpCreateStruct->y,lpCreateStruct->cx, lpCreateStruct->cy),
						 this, 
						 IDC_ALARMSGRID
						 ))
	    {
		    theApp.LogInfo( L"m_sGrid.Create in CAlarmView is failed!" );
		    return -1;
	    }

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

		 // 设置字体
	    ApplyFont();
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CAlarmView::OnCreate!");

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CAlarmView::OnCreate!");

		return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : OnUpdate
// Description    : 
//                  1.判断更新操作：Refresh or Monitor，非法操作时，直接返回；
//                  2.判断文档是否存在：文档不存在时，设定视图行数为1行；
//                  3.文档存在时，根据刷新操作类型，更新视图（设定视图行数，显示xml文件中的内容）
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.07 10:45:00
// =======================================================================
void CAlarmView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint )
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

			// Vol name
			CString s;
			s.LoadStringW( IDS_NAME );
			m_sGrid.put_TextMatrix( 0, COL_NAME, s );         // COL_NAME

			s.Empty();
			s.LoadStringW( IDS_VALUE );
			m_sGrid.put_TextMatrix( 0, COL_VALUE, s );        // COL_VALUE
		}

	    m_sGrid.put_Redraw( CMygridn::flexRDNone );

		size_t size = GetDocument()->m_aAlarmValues->size();
	    if( size > 0 )
	    {
			if( lHint == CConfiguratorDoc::HINT_REFRESH )
			{
			    UpdateRefresh();       
				UpdateMonitor( false );    
			}
			else
			{
				UpdateMonitor( true );
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
	 
	    if( m_sGrid.get_Redraw() != CMygridn::flexRDBuffered )
		{
			m_sGrid.put_Redraw( CMygridn::flexRDBuffered );
		}
    }
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		CString str;
		str.Format( L"Exception occured in CAlarmView::OnUpdate - %d", lHint );
		theApp.LogInfo( str );

		return;
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CAlarmView::OnUpdate - %d", lHint );
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
void CAlarmView::UpdateRefresh( void )
{
	if( 0 == GetDocument()->m_aAlarmValues->size())
	{
        m_sGrid.put_Rows( 1L ); 
		return;
	}

	// Rows
	long lRows = GetDocument()->m_aAlarmValues->size() + 1;
	m_sGrid.put_Rows( lRows );

	// 遍历容器，将xml文件内容显示到视图上
	long lRow = 1L;
	CCfgMonitorValues* pValues = GetDocument()->m_aAlarmValues;

	CCfgMonitorValues::const_iterator it = pValues->begin();
	CCfgMonitorValues::const_iterator eit = pValues->end();
    while( it!= eit )
	{
        const CCfgMonitorValue& p = *it;
		m_sGrid.put_TextMatrix( lRow, COL_NAME, p.GetTitle_C() );   // Set Name Column
		
		CString s = p.GetVariable();
		if( s.IsEmpty())              // not exists
		{
			// Group aliases together
			m_sGrid.put_IsSubtotal( lRow, true );                                                     
			m_sGrid.put_RowOutlineLevel( lRow, 1 );     

			// Set Font Bold 
			m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t(lRow), _variant_t(COL_NAME), 
				               _variant_t(lRow), _variant_t(COL_VALUE), _variant_t(true));            // 加粗
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
}


// =======================================================================
// Function name  : UpdateMonitor
// Description    : Set Value Column
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.07 10:55:00
// =======================================================================
void CAlarmView::UpdateMonitor( bool bMonitor )
{
    if( 0 == GetDocument()->m_aMonitorValues->size())
	{
        m_sGrid.put_Rows( 1L ); 
		return;
	}

	CCfgMonitorValues* pValues = GetDocument()->m_aAlarmValues;

	long lRow = 1L;

	CCfgMonitorValues::iterator it = pValues->begin();
	CCfgMonitorValues::iterator eit = pValues->end();
	while( it != eit )
	{
		CCfgMonitorValue& p = *it;
		const CAliasItem* pAlias = p.GetAlias();
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			// When Refresh or the remote value was changed: Display the value
			bool bChanged = p.Update( !bMonitor );
			if( !bMonitor || bChanged )
			{
				_variant_t varColor_t = global::clWindowText;
				if( pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN )
				{
					_variant_t var_t = (pAlias->remBool() ? CMygridn::flexChecked : CMygridn::flexUnchecked);
					m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t( COL_VALUE ),
						              _variant_t( lRow ), _variant_t( COL_VALUE ), var_t );

					// Font Color
					if( p.GetFontColor() != -1 )
					{
						if(  CConfiguratorApp::DSC100 == theApp.GetProductType())
						{
							// DSC100: 0: set font color 
							varColor_t = ( pAlias->remBool() ? global::clWindowText : p.GetFontColor());
						}
						else 
						{
							varColor_t = ( pAlias->remBool() ? p.GetFontColor() : global::clWindowText );
						}
					}
				}
				else
				{
					CString s;
					VariantToString( pAlias->remVar(), s, TYPE_HEX ); // Hex
					/*if( theApp.GetProductType() != CConfiguratorApp::TS350 )
					{
						VariantToString( pAlias->remVar(), s );           // Binary
					}
					else
					{ 
						VariantToString( pAlias->remVar(), s, TYPE_HEX ); // Hex
					}*/
					m_sGrid.put_TextMatrix( lRow, COL_VALUE, s );


					// Font Color: non_zero, set font color
					if( p.GetFontColor() != -1 )
					{
					    varColor_t = ( pAlias->remUI8() ? p.GetFontColor() : global::clWindowText); 
					}
				}

				// Set Font Color
				m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t( COL_NAME ),
						            _variant_t( lRow ), _variant_t( COL_VALUE ), varColor_t );
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
void CAlarmView::ApplyFont( void )
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
// Function name  : OnSize
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong
// =======================================================================
void CAlarmView::OnSize(UINT nType, int cx, int cy)
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
//--------------------------------------------------------------------------


