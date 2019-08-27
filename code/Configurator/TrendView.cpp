// TrendView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "TrendView.h"

// Added by wangdong 
#include "MainFrm.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"

// Added by wangdong for TeeChart
#include "CAxes.h"              // 坐标轴
#include "CAxis.h"
#include "CPen0.h"              // 坐标轴Ticks，Grid操作
#include "CScroll.h"            // 移动（默认鼠标右键）

#include "CSeries.h"            // 序列
#include "CMarks.h"             // 序列Mark
#include "CValueList.h"         // X/YVaules
#include "CLegend.h"            // 图例
#include "CZoom.h"              // 缩放
#include "CPage.h"              // 页面
#include "CSeriesXMLSource.h"   // XML Source
#include "CAxisLabels.h"        // Label
#include "CSeriesList.h"        // Series list

#include "CAspect.h"            // 外观(3D)
#include "CTitles.h"            // 标题
#include "CTeeFont.h"           // 字体

#include "CCanvas.h"            // 画布
#include "CPanel.h"             // 设置背景色或者背景图片(BackImage, Backcolor)
#include "CGradient.h"          // 颜色渐变

//===========================================================
// Added by wangdong
#define TREND_RB_SELECTED   L"Trend_RB_Sel"
extern std::auto_ptr<modbus::CModbusThread> ModbusThread;
//===========================================================

// CTrendView

IMPLEMENT_DYNCREATE(CTrendView, CConfiguratorView)                // CView -> CConfiguratorView

CTrendView::CTrendView()
           :m_dwViewHelpID( HIDR_TREND )
		    ,m_dwTrendTick( INFINITE )
			,m_dtLastTime( 1899,12,30,0,0,0 )      // == 0.0
			,m_dtScrollTime( 1899,12,30,0,0,10 )   // 10s
			,m_bAutoScroll( false )
			,m_bUserScroll( false )
		    ,m_nRBSel( RB01 )
{
}

CTrendView::~CTrendView()
{
}

BEGIN_MESSAGE_MAP(CTrendView, CConfiguratorView)                // CView -> CConfiguratorView
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED( IDC_TREND_RB01, OnRadio1 )
	ON_BN_CLICKED( IDC_TREND_RB02, OnRadio2 )
	ON_BN_CLICKED( IDC_TREND_RB03, OnRadio3 )
	ON_BN_CLICKED( IDC_TREND_RB04, OnRadio4 )
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTrendView, CConfiguratorView)
	ON_EVENT(CTrendView, IDC_TREND_TEECHART, 24, CTrendView::Mytchart_OnScroll, VTS_NONE)
END_EVENTSINK_MAP()


// CTrendView 绘图

void CTrendView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
}


// CTrendView 诊断

#ifdef _DEBUG
void CTrendView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTrendView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// =======================================================================
// Function name  : SetViewCtrl
// Description    : Set View Control: 
//                                    RadioButton: Check
//                                    TeeChart: time range and time interal
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 17:10:00
// =======================================================================
void CTrendView::SetViewCtrl( int nRBSel )
{
	CSingleLock singleLock( &m_cirsec );
	singleLock.Lock();
	if( singleLock.IsLocked())
	{
		COleDateTime timeInterval, timeMax;

		switch( nRBSel )
		{
			case RB01:   // 1 minute
				m_btRB01.SetCheck( BST_CHECKED );
				timeInterval = COleDateTime( 1899,12,30,0,0,5 );     // 5s
				timeMax = COleDateTime( 1899,12,30,0,1,1 );          // 1 minute
				m_dtScrollTime.SetTime( 0, 0, 10 );                  // scroll time: 10s
			break;
			case RB02:   // 10 minute
				m_btRB02.SetCheck( BST_CHECKED );
				timeInterval = COleDateTime( 1899,12,30,0,0,30 );    // 30s
				timeMax = COleDateTime( 1899,12,30,0,10,1 );         // 10 minute
				m_dtScrollTime.SetTime( 0, 1, 0 );                   // scroll time: 1 minute
			break;
			case RB03:   // 30 minute
				m_btRB03.SetCheck( BST_CHECKED );   
				timeInterval = COleDateTime( 1899,12,30,0,1,0 );     // 1 minute
				timeMax = COleDateTime( 1899,12,30,0,30,1 );         // 30 minute
				m_dtScrollTime.SetTime( 0, 2, 0 );                   // scroll time: 2 minute
			break;
			case RB04:   // 1 hour
				m_btRB04.SetCheck( BST_CHECKED );
				timeInterval = COleDateTime( 1899,12,30,0,2,0 );     // 2 minute
				timeMax = COleDateTime( 1899,12,30,1,0,1 );          // 1 hour
				m_dtScrollTime.SetTime( 0, 4, 0 );                   // scroll time: 4 minute
			break;
			default:     // 1 minute
				m_btRB01.SetCheck( BST_CHECKED );
				timeInterval = COleDateTime( 1899,12,30,0,0,5 );     // 5s
				timeMax = COleDateTime( 1899,12,30,0,1,1 );          // 1 minute
				m_dtScrollTime.SetTime( 0, 0, 10 );                  // scroll time: 10s
		}

		CAxes axes( m_tChart.get_Axis());
		CAxis axis( axes.get_Bottom());
		axis.put_Increment( timeInterval );                           //  time interval
		axis.put_Maximum( axis.get_Minimum() + timeMax );             //  time range: 0.0 - timeMax  

		singleLock.Unlock();
	}
}

// =======================================================================
// Function name  : AddSeriesFixedValues
// Description    : 
// Return type    :
// Parameters     : 
// Author         : rjvs 2002-06-16 17:35:57 (UTC-6:00)
// =======================================================================
inline void AddSeriesFixedValues( CCfgTraceArray* pTraces, CMytchart* AChart, double dTime, double dValue )
{
    int index = 0;
    CCfgTraceArray::const_iterator  it = pTraces->begin();
    CCfgTraceArray::const_iterator eit = pTraces->end();
    while( it != eit )
    {
        CSeries aSeries( AChart->Series( index ));
        if (it->GetAlias() != NULL)
        {
            aSeries.AddNullXY( dTime, dValue, L"" );  // Adds a Null point at an X, Y location
        }

        ++index;
        ++it;
    }
}

// =======================================================================
// Function name  : AddSeriesValues
// Description    : 
// Return type    :
// Parameters     : 
// Author         : rjvs 2002-06-16 17:35:57 (UTC-6:00)
// =======================================================================
inline void AddSeriesValues( CCfgTraceArray* pTraces, CMytchart* AChart, double dTime )
{
    int index = 0;
    CCfgTraceArray::const_iterator  it = pTraces->begin();
    CCfgTraceArray::const_iterator eit = pTraces->end();
    while (it != eit)
    {
        CSeries aSeries( AChart->Series( index ));
        const CAliasItem* pAlias = it->GetAlias();
        if( pAlias != NULL )
        {
            double dValue = 0.0;

            if((pAlias->GetDataType() == CAliasItem::ALIAS_FLOAT) ||
               (pAlias->GetDataType() == CAliasItem::ALIAS_FLOATSWAP))
			{
                dValue = pAlias->remR4();
			}
            else
			{
                dValue = pAlias->remI4();
			}

            // Inserts a new point in the Series
            aSeries.AddXY( dTime, dValue, L"", aSeries.get_Color());
        }

        ++index;
        ++it;
    }
}

//===========================================================================
// CTrendView 消息处理程序

// =======================================================================
// Function name  : OnCreate
// Description    : 
// Return type    : 0 : Create successful
//                  -1: Create failed
// Parameters     : 
// Author         : WangDong 2011.01.17 09:00:00
// =======================================================================
int CTrendView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( m_dwViewHelpID );       // Set View HelpID

	try
	{
		///1. Create TeeChart
	    if( !m_tChart.Create( lpCreateStruct->lpszClass,
		             L"",
					 lpCreateStruct->dwExStyle,
					 CRect( 0, 0, lpCreateStruct->cx, (lpCreateStruct->cy-50)),
					 this,
					 IDC_TREND_TEECHART
					 ))
	    {
			theApp.LogInfo( L"m_tChart.Create in CTrendView::OnCreate is failed!" );
		    return -1;
	    }

		///2. Create 4 RadioButtons
		int nRBX = (lpCreateStruct->cx) / 6;
		int nRBY = lpCreateStruct->cy - 35;

		// Load string: Radio Button's Caption, and Get the string's size
		CClientDC dc( this );
		CString strTimeRange1, strTimeRange2, strTimeRange3, strTimeRange4;

		strTimeRange1.LoadStringW( IDS_TREND_TIMERANGE1 );    // Load string
	    CSize size1( dc.GetTextExtent( strTimeRange1 ));      // Get the string's size
                                
		strTimeRange2.LoadStringW( IDS_TREND_TIMERANGE2 );   
	    CSize size2( dc.GetTextExtent( strTimeRange2 ));      
                               
		strTimeRange3.LoadStringW( IDS_TREND_TIMERANGE3 );    
	    CSize size3( dc.GetTextExtent( strTimeRange3 ));      
                               
		strTimeRange4.LoadStringW( IDS_TREND_TIMERANGE4 );    
	    CSize size4( dc.GetTextExtent( strTimeRange4 ));     

		// sizen.cx: string; 16:radio button(经试验测出这个数值是合适的)
		CRect rect1( nRBX, nRBY, nRBX+size1.cx+16, nRBY+size1.cy );
		CRect rect2( 2*nRBX, nRBY, 2*nRBX+size2.cx+16, nRBY+size2.cy );
		CRect rect3( 3*nRBX, nRBY, 3*nRBX+size3.cx+16, nRBY+size3.cy );
		CRect rect4( 4*nRBX, nRBY, 4*nRBX+size4.cx+16, nRBY+size4.cy );

		if(!((m_btRB01.Create(strTimeRange1, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rect1, this, IDC_TREND_RB01 )) &&
		     (m_btRB02.Create(strTimeRange2, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rect2, this, IDC_TREND_RB02 )) &&
		     (m_btRB03.Create(strTimeRange3, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rect3, this, IDC_TREND_RB03 )) &&
		     (m_btRB04.Create(strTimeRange4, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rect4, this, IDC_TREND_RB04 ))))
		{
			return -1;
		}

		///3. Set View Ctrl 
		CString strSection = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
	    if( !strSection.IsEmpty())
	    {
			int nRBSel = theApp.GetProfileIntW( strSection, TREND_RB_SELECTED, RB01 );
            SetViewCtrl( nRBSel );
		}

		///4. Initialize m_tChart
	    // Cancel 3D View
	    CAspect aAspect( m_tChart.get_Aspect());
	    aAspect.put_View3D( false );

	    // Hide title header
	    CTitles aTitle( m_tChart.get_Header());
	    aTitle.put_Visible( false );                            // hide, default visible

	    // Set BKColor: White
	    CCanvas aCanvas( m_tChart.get_Canvas());
	    if( aCanvas.IsScreenHighColor())
	    {
		    CPanel aPanel( m_tChart.get_Panel());
			aPanel.put_Color( global::clWindow );               // set backcolors

		    /* 颜色渐变
		    CGradient aGradient( aPanel.get_Gradient());
		    aGradient.put_Visible( true );                      // 使渐变色可视
		    aGradient.put_Direction( CMytchart::gdTopBottom );
		    aGradient.put_EndColor(0xffff);                     // 黄色
		    */
	    }

	    // Hide Legend
	    CLegend legend( m_tChart.get_Legend());
	    legend.put_Visible( false );                            // hide, default visible

	    // Enable/Unable zoom function
	    CZoom zoom( m_tChart.get_Zoom());
	    zoom.put_Enable(true);                                  // default true: Enable zoom
	    if( !zoom.get_Animated() )
	    {
		    zoom.put_Animated( true );                          // default true
	    }
	    zoom.put_MouseButton( CMytchart::mbLeft );              // EMouseButton, default left mouse button
	    zoom.put_Direction( CMytchart::tzdBoth);                // ETeeZoomDirection:tzdHorizontal, tzdVertical, tzdBoth(default)

	    // Scroll
	    CScroll scroll( m_tChart.get_Scroll());
	    scroll.put_Enable( CMytchart::pmBoth );                 // EChartScroll：pmNone, pmHorizontal, pmVertical, pmBoth(default)
	    scroll.put_MouseButton( CMytchart::mbRight );           // EMouseButton, default right mouse button
	                                                   
	    // Axes
	    CAxes axes( m_tChart.get_Axis());

	    // Bottom Axis
	    CAxis axis( axes.get_Bottom());
	    axis.put_Automatic( false );
	    axis.put_ExactDateTime( true );                          // Controls if TChartAxis.Increment property calculates Axis Labels in exact DateTime steps.
	    CAxisLabels label( axis.get_Labels());
	    label.put_DateTimeFormat( L"n:ss" );                     // Set DateTime format

	    // Left Axis
	    axis = axes.get_Left();
	    axis.put_Automatic( false );
	    axis.put_AutomaticMaximum( false );
	    axis.put_AutomaticMinimum( false );
	    axis.SetMinMax( 0, 100.0 );                              // Range: 0 ~ 100
	    axis.put_StartPosition( 0.45 );                          // percent
	    axis.put_EndPosition( 99.65 );

	    // Right Axis
	    axis = axes.get_Right();
	    axis.put_Automatic( true );                              // true:The Maximum and Minimum values for the Axis will be calculated with all Axis dependent Series.
		axis.put_Minimum( 0.0 );
	    axis.put_StartPosition( 0.45 );
	    axis.put_EndPosition( 99.65 );

	    CPen0 aPen( axis.get_GridPen());       // 栅格线（即表格线）
		aPen.put_Color( global::clBlack );     // black           
	    aPen.put_Visible( false );             // hidden

	    aPen = (CPen0)axis.get_Ticks();        // 坐标轴刻度（即短横线）
	    aPen.put_Color( global::clBlack );     // black
	    aPen.put_Visible( false );             // hidden

	    axis.put_Visible( false );             // 隐藏右坐标轴

		// Add 8 series(Fastline) to chart if a index value greater than the
        // available count is chosen
	    for( size_t i = 0; i < 8; ++i )
	    {
		    m_tChart.AddSeries( scFastLine );                       // 增加序列
		
		    CSeries aSeries( m_tChart.Series( i ));
		    aSeries.put_Title( L"<none>");                          // The Title property is used in the Legend to draw the series descriptions.
		    aSeries.put_HorizontalAxis( CMytchart::aBottomAxis );   // 设置序列的水平坐标为底座标
		    aSeries.put_VerticalAxis( CMytchart::aLeft );           // 设置序列的垂直坐标为左坐标(默认)
		    aSeries.put_ShowInLegend( false );                      // Setting ShowInLegend enables/disables the display of the series in the Legend box.
			aSeries.put_Color( global::TraceColors[i]);             // 设置序列颜色

		    CMarks aMark( aSeries.get_Marks());
		    aMark.put_Visible( false );
		
			// Order: Automatically sorted or it they will remain always at their original positions.
		    CValueList aValuelist( aSeries.get_XValues());         // 水平坐标轴
		    aValuelist.put_DateTime( true );                       // 时间轴
		    aValuelist.put_Order( CMytchart::loAscending );        // 升序

		    aValuelist = (CValueList)aSeries.get_YValues();        // 垂直坐标轴
		    aValuelist.put_Order( CMytchart::loNone );             // 不排序
		    
			CPen0 aPen( aSeries.get_Pen());
		    aPen.put_Width( 2 );
		  
		    aSeries.put_Active( false );                            
        }

		m_tChart.put_Visible( true );
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CTrendView::OnCreate!" );

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CTrendView::OnCreate!" );

		return -1;
	}
	
	return 0;
}

// =======================================================================
// Function name  : OnSize
// Description    : Resize Tree Ctrl: TeeChart; Radio Buttons
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.17 10:00:00
// =======================================================================
void CTrendView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	///1. TeeChart
	if( m_tChart.GetSafeHwnd())
	{
	    m_tChart.put_Height( cy - 50 );
	    m_tChart.put_Width( cx );
	}

	///2. Radio Buttons
	if( m_btRB01.GetSafeHwnd() && 
		m_btRB02.GetSafeHwnd() && 
		m_btRB03.GetSafeHwnd() && 
		m_btRB04.GetSafeHwnd())
	{
		// Position
	    int nRBX = cx / 6;
		int nRBY = cy - 35;

		// Get ideal size
		CSize size1, size2, size3, size4;
		m_btRB01.GetIdealSize( &size1 );
		m_btRB02.GetIdealSize( &size2 );
		m_btRB03.GetIdealSize( &size3 );
		m_btRB04.GetIdealSize( &size4 );

		CRect rect1( nRBX,   nRBY, nRBX+size1.cx,   nRBY+size1.cy );
		CRect rect2( 2*nRBX, nRBY, 2*nRBX+size2.cx, nRBY+size2.cy );
		CRect rect3( 3*nRBX, nRBY, 3*nRBX+size3.cx, nRBY+size3.cy );
		CRect rect4( 4*nRBX, nRBY, 4*nRBX+size4.cx, nRBY+size4.cy ); 

		// Set Window Position
		// x, y: Client axis
		// SWP_NOZORDER: ignore the first parameter
		m_btRB01.SetWindowPos( &CWnd::wndBottom, rect1.left, rect1.top,
			                                     rect1.Width(), rect1.Height(), SWP_NOZORDER );

		m_btRB02.SetWindowPos( &CWnd::wndBottom, rect2.left, rect2.top,
			                                     rect2.Width(), rect2.Height(), SWP_NOZORDER);

		m_btRB03.SetWindowPos( &CWnd::wndBottom, rect3.left, rect3.top,
			                                     rect3.Width(), rect3.Height(), SWP_NOZORDER);

		m_btRB04.SetWindowPos( &CWnd::wndBottom, rect4.left, rect4.top,
			                                     rect4.Width(), rect4.Height(), SWP_NOZORDER);	
	}
}

// =======================================================================
// Function name  : OnRadio1 / OnRadio2 / OnRadio3 /OnRadio4
// Description    : 1. Set View Ctrl: Check radio button; Set time interval and maximum time range
//                  2. Write to registry
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 16:20:00
// =======================================================================
void CTrendView::OnRadio1()
{
	//1. Set View Ctrl: Check Radio button; Set time interval and maximum time range
	SetViewCtrl( RB01 );

	//2. Write to registry
	CString strPath = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
	if( !strPath.IsEmpty())
	{
        theApp.WriteProfileInt( strPath, L"Trend_RB_Sel", RB01 );
	}
}

void CTrendView::OnRadio2()
{
	//1. Set View Ctrl: Check Radio button; Set time interval and maximum time range
	SetViewCtrl( RB02 );

	//2. Write to registry
	CString strPath = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
	if( !strPath.IsEmpty())
	{
        theApp.WriteProfileInt( strPath, L"Trend_RB_Sel", RB02 );
	}
}

void CTrendView::OnRadio3()
{
	//1. Set View Ctrl: Check Radio button; Set time interval and maximum time range
	SetViewCtrl( RB03 );

	//2. Write to registry
	CString strPath = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
	if( !strPath.IsEmpty())
	{
        theApp.WriteProfileInt( strPath, L"Trend_RB_Sel", RB03 );
	}
}

void CTrendView::OnRadio4()
{
	//1. Set View Ctrl: Check Radio button; Set time interval and maximum time range
	SetViewCtrl( RB04 );

	//2. Write to registry
	CString strPath = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
	if( !strPath.IsEmpty())
	{
        theApp.WriteProfileInt( strPath, L"Trend_RB_Sel", RB04 );
	}
}

// =======================================================================
// Function name  : OnUpdate
// Description    : Update View Contents
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.17 13:40:00
// =======================================================================
void CTrendView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if( NULL == m_tChart.GetSafeHwnd())
	{
		return;
	}

	if((lHint != CConfiguratorDoc::HINT_REFRESH) &&
		(lHint != CConfiguratorDoc::HINT_TRACE)   &&
		(lHint != CConfiguratorDoc::HINT_MONITOR))
	{
		return;
	}

	if( GetDocument()->m_aMonitorValues->size() > 0 )
	{
		if((lHint == CConfiguratorDoc::HINT_REFRESH) ||
		   (lHint == CConfiguratorDoc::HINT_TRACE))
		{
			if( theApp.GetResetViewFlag())    // Application Type or Product Type was changed
			{
				CString strSection = theApp.GetPathAccordingToProduct( theApp.GetProductType()); 
				if( !strSection.IsEmpty())
				{
					int nRBSel = theApp.GetProfileIntW( strSection, TREND_RB_SELECTED, RB01 );
					SetViewCtrl( nRBSel );
				}

				CString strTimeRange;
				strTimeRange.LoadStringW( IDS_TREND_TIMERANGE1 );    // Load string
				m_btRB01.SetWindowTextW( strTimeRange );
                                
				strTimeRange.LoadStringW( IDS_TREND_TIMERANGE2 ); 
				m_btRB02.SetWindowTextW( strTimeRange );
	      
				strTimeRange.LoadStringW( IDS_TREND_TIMERANGE3 );    
	            m_btRB03.SetWindowTextW( strTimeRange );
                               
				strTimeRange.LoadStringW( IDS_TREND_TIMERANGE4 );    
	            m_btRB04.SetWindowTextW( strTimeRange );
			}

			OnUpdateRefresh();            // Set series
		}
		else
		{
			OnUpdateMonitor();
		}
	}
}

// =======================================================================
// Function name  : OnUpdateRefresh
// Description    : Set Series: Active;  Title;   Color
//                              Vertical axis;                                           
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.02.11 13:00:00
// =======================================================================
void CTrendView::OnUpdateRefresh( void )
{
	for( long i=0; i<m_tChart.get_SeriesCount(); ++i )
	{
        CSeries aSeries( m_tChart.Series( i ));
	    CString strName( GetDocument()->GetTraceName( i ));
		if( strName.IsEmpty())
		{
		    aSeries.put_Active( FALSE );
			aSeries.put_Title( L"<none>" );
			aSeries.put_Color( global::TraceColors[i]);
			aSeries.Clear();
		}
		else
		{
		    if((strName.CompareNoCase( aSeries.get_Title()) != 0) || theApp.GetResetViewFlag())
			{
			    // The trace name was different from the series title or 
				// the application / product type was changed,
                // so clear the current values, and reset series title 
                aSeries.Clear();
				aSeries.put_Title( strName );
			}

			// Set the correct vertical axis for the trace
			if( static_cast<size_t>(i) < GetDocument()->GetTraceCount())
			{
			    CCfgTrace &t = GetDocument()->m_aTraces->at(i);
				if( t.GetAxis() == 0 )
				{
                    aSeries.put_VerticalAxis( CMytchart::aLeft );             // Left Y axis
				}
				else
				{
				    aSeries.put_VerticalAxis( CMytchart::aRight );             // Right Y axis

					// Make Right Y axis visible
	                CAxes axes( m_tChart.get_Axis());
	                CAxis axis( axes.get_Right());
					axis.put_Visible( TRUE );
				}

				aSeries.put_Active( TRUE );  // Display the trace
			}
		}//else
	}//for  
}

// =======================================================================
// Function name  : OnUpdateMonitor
// Description    : Update View Contents
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.02.11 14:30:00
// =======================================================================
void CTrendView::OnUpdateMonitor( void )
{
	const static long MAXPOINTS = 60*60*3;                     // 一个小时（假设监视周期最小为350ms，1s最多记录3个点）
	const static long DEADPOINTS = MAXPOINTS-250;              // 保留的点数
	const static COleDateTime deadTime( EncodeTimeSec( 5.0 )); // 时间间隔大于此时间时，用0填充Series上（此段时间）的值

	// Delete extra points
	for( long i=0; i<m_tChart.get_SeriesCount(); ++i )
	{
		CSeries aSeries( m_tChart.Series( i ));
		long lCount = aSeries.get_Count();
		if( lCount > MAXPOINTS )
		{
			aSeries.BeginUpdate();

			lCount -= DEADPOINTS;
			while( lCount > 0 )
			{
				aSeries.Delete( 0L );
				lCount--;
			}

			aSeries.EndUpdate();
		}
	}//for

	if( m_dwTrendTick == INFINITE )
	{
        m_dwTrendTick = ::GetTickCount();   // When first start monitor: ms
	}

	// Get current time and trace array
	COleDateTime dtTime( EncodeTimeSec( (GetTickCount() - m_dwTrendTick) / 1000.0 ));
	CCfgTraceArray* pTraces = GetDocument()->m_aTraces;

    // Check the deadtime
    if((static_cast<double>(m_dtLastTime) > 0.0) &&   
		((dtTime - m_dtLastTime) > deadTime))               
	{
		// Add null values during the dead time
		CSingleLock sl( &pTraces->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

		AddSeriesFixedValues( pTraces, &m_tChart, m_dtLastTime, 0.0 );  // Adds a Null point at an X, Y location
        AddSeriesFixedValues( pTraces, &m_tChart, dtTime, 0.0 );

		sl.Unlock();
	}

	// Update the last time
    m_dtLastTime = dtTime;

	// Add all the variables in the array to the trend chart
    {
        CSingleLock sl( &pTraces->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

        AddSeriesValues(pTraces, &m_tChart, dtTime);

		sl.Unlock();
    }

	/* Manual Scroll axis (if necessary) -- Changed by wangdong for Bug #46 Begin */
	if((m_bUserScroll && m_bAutoScroll) || (!m_bUserScroll))
    {
		CSingleLock singleLock( &m_cirsec );
		singleLock.Lock();
		if( singleLock.IsLocked())
		{
			CAxes axes( m_tChart.get_Axis());
			CAxis axis( axes.get_Bottom());
			COleDateTime dtDiff = static_cast<double>(dtTime) - axis.get_Maximum();    // dtDiff: Current time - Maximum of Axis
			if( static_cast<double>(dtDiff) >= 0.0 )
			{
				if( dtDiff > m_dtScrollTime )   // scrollTime
				{
					axis.Scroll( dtDiff, TRUE );   
				}
				else
				{
					axis.Scroll( m_dtScrollTime, TRUE );
				}
			}

			singleLock.Unlock();
		}
    }//if
	/* Changed by wangdong for Bug #46 End */
}

// =======================================================================
// Function name  : Mytchart_OnScroll
// Description    : When Monitoring, Update Flags; Otherwise don't update Flags.
///                 只需要更新标志位，时间坐标轴会自动调整，具体如下：
///                 左移(时间坐标值变大) 或者 右移(时间坐标值变小) 时，
///                 如果到达了当前时间所在的时间段，则根据需要，手动调整时间坐标；
///                                                          否则，不手动调整。
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.02.11 17:22:00
// =======================================================================
void CTrendView::Mytchart_OnScroll()
{
    if((ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR) && (m_dwTrendTick != INFINITE))
    {
		CAxes axes( m_tChart.get_Axis());
		CAxis axis( axes.get_Bottom());
        m_bUserScroll = true;
        COleDateTime dtTime = EncodeTimeSec((GetTickCount() - m_dwTrendTick) / 1000.0);
        COleDateTime dtDiff = static_cast<double>(dtTime) - axis.get_Maximum();
        if( static_cast<double>(dtDiff) > 0.0 )
        {
            m_bAutoScroll = false;            // 未到达当前时间所在的监视界面
        }
        else
        {
            m_bAutoScroll = true;             // 到达当前时间所在的监视界面
        }
    } 
}
