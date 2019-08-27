// HistoryTrendView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "HistoryTrendView.h"

// Added by wangdong 
#include "MainFrm.h"
#include "ConfiguratorDoc.h"
#include "ModbusThread.h"
#include "ViewHistoryCtrl.h"

// TeeChart
#include "CAxes.h"              // 坐标轴
#include "CAxis.h"
#include "CPen0.h"              // 坐标轴Ticks，Grid操作
#include "CScroll.h"            // 移动（默认鼠标右键）

#include "CSeries.h"            // 序列
#include "CSeriesSource.h"
#include "CSeriesXMLSource.h"
#include "CSeriesSourceFile.h"
#include "CMarks.h"             // 序列Mark
#include "CValueList.h"         // X/YVaules
#include "CLegend.h"            // 图例
#include "CZoom.h"              // 缩放
#include "CPage.h"              // 页面
#include "CAxisLabels.h"        // Label
#include "CSeriesList.h"        // Series list

#include "CAspect.h"            // 外观(3D)
#include "CTitles.h"            // 标题
#include "CTeeFont.h"           // 字体

#include "CCanvas.h"            // 画布
#include "CPanel.h"             // 设置背景色或者背景图片(BackImage, Backcolor)
#include "CGradient.h"          // 颜色渐变


// Timer Update Thread
UINT TimerUpdateThreadFunc( LPVOID lParam )
{
	CHistoryTrendView* pView = (CHistoryTrendView*)lParam;
	if( pView->GetSafeHwnd())
	{
		pView->TimerUpdate();
	}

	return 0;
}


// CHistoryTrendView

IMPLEMENT_DYNCREATE(CHistoryTrendView, CConfiguratorView)

CHistoryTrendView::CHistoryTrendView()
                  :m_pViewHisCtrl( NULL )
				  ,m_uHisTimer( 0 )
				  ,m_nLastRate( 0 )
				  ,m_pTimerUpdateEvent( new CEvent(TRUE,TRUE))  // has event, manual reset
{

}

CHistoryTrendView::~CHistoryTrendView()
{
}

BEGIN_MESSAGE_MAP(CHistoryTrendView, CConfiguratorView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CHistoryTrendView, CConfiguratorView)
	ON_EVENT(CHistoryTrendView, IDC_TREND_TEECHART, 24, CHistoryTrendView::Mytchart_OnScroll, VTS_NONE)
	ON_EVENT(CHistoryTrendView, IDC_TREND_TEECHART, 19, CHistoryTrendView::Mytchart_MouseDown, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


// CHistoryTrendView 绘图

void CHistoryTrendView::OnDraw(CDC* pDC)
{
	CConfiguratorDoc* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CHistoryTrendView 诊断

#ifdef _DEBUG
void CHistoryTrendView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHistoryTrendView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHistoryTrendView 消息处理程序
// =======================================================================
// Function name  : OnCreate
// Description    : 
// Return type    : 0 : Create successful
//                  -1: Create failed
// Parameters     : 
// Author         : WangDong 2013.04.01 17:00:00
// =======================================================================
int CHistoryTrendView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

    try
	{
		m_pViewHisCtrl = GetDocument()->GetViewHisCtrl();

		///1. Create TeeChart
	    if( !m_tChart.Create( lpCreateStruct->lpszClass,
							L"",
							lpCreateStruct->dwExStyle,
							CRect( 0, 0, lpCreateStruct->cx, lpCreateStruct->cy),
							this,
							IDC_TREND_TEECHART ))
	    {
			theApp.LogInfo( L"m_tChart.Create in CHistoryTrendView::OnCreate is failed!" );
		    return -1;
	    }

		///2. Initialize m_tChart
	    // Cancel 3D View
	    CAspect aAspect( m_tChart.get_Aspect());
	    aAspect.put_View3D( FALSE );

		// Set BKColor: White
	    CCanvas aCanvas( m_tChart.get_Canvas());
	    if( aCanvas.IsScreenHighColor() )
	    {
		    CPanel aPanel( m_tChart.get_Panel());
			aPanel.put_Color( RGB(255,255,0));     // set backcolors: global::clWindow
	    }

		// Axes
		CAxes axes( m_tChart.get_Axis());
	    CAxis axis( axes.get_Bottom());
		axis.put_AutomaticMinimum( TRUE );
		axis.put_AutomaticMaximum( TRUE );
		axis.put_ExactDateTime( true );       // Controls if TChartAxis.Increment property calculates Axis Labels in exact DateTime steps

		// Hide title header - Default "TeeChart"
	    CTitles aTitle( m_tChart.get_Header());
	    aTitle.put_Visible( false );

	    // Hide Legend
	    CLegend legend( m_tChart.get_Legend());
	    legend.put_Visible( false ); 

		// Series
		CSeries aSeries;
		CValueList aValuelist;
		VARIANT SourceSeries;
		for( size_t i=0; i < SERIES_COUNT; ++i )
		{
			m_tChart.AddSeries( scFastLine );               // Type: Fastline

			aSeries = (CSeries)m_tChart.Series( i );
			aSeries.put_Color( global::TraceColors[i]);     // Color
			aSeries.put_Active( FALSE );                    // Active

			CPen0 aPen( aSeries.get_Pen());
		    aPen.put_Width( 2 );                            // Pen Width

			// X axis
			aValuelist = (CValueList)aSeries.get_XValues(); 
			aValuelist.put_DateTime( true );                // DataTime
			aValuelist.put_Order( CMytchart::loAscending ); // Order
			aValuelist.put_ValueSource( L"X" );

			// XML Source 
			if( m_xmlSrcV[i].Create( L"", L"", 0, CRect( 0,0,1,1), this, i+1))
			{
				SourceSeries.vt = VT_DISPATCH;
				SourceSeries.pdispVal = m_tChart.Series( i );
				m_xmlSrcV[i].put_Series( SourceSeries );
				m_xmlSrcV[i].put_ValueSource( L"Y" );     // point type，default ValueSource is "Y"
				m_xmlSrcV[i].put_SeriesNode( L"" );
				m_xmlSrcV[i].Close();
			}
		}
                                
		m_tChart.put_Visible( true );
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CHistoryTrendView::OnCreate!" );

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CHistoryTrendView::OnCreate!" );

		return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : OnSize
// Description    : Resize Tree Ctrl: TeeChart; Radio Buttons
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 16:50:00
// =======================================================================
void CHistoryTrendView::OnSize(UINT nType, int cx, int cy)
{
	CConfiguratorView::OnSize(nType, cx, cy);

	// TeeChart
	if( m_tChart.GetSafeHwnd())
	{
	    m_tChart.put_Height( cy );
	    m_tChart.put_Width( cx );
	}
}

// ----------------------------------------------------------------------
// Function name  : SetAxisAutomatic
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.26 17:20:00
// ----------------------------------------------------------------------
void CHistoryTrendView::SetAxisAutomatic( bool bAutomatic )
{
	CAxes axes( m_tChart.get_Axis());
	CAxis axis( axes.get_Bottom());
	axis.put_AutomaticMinimum( bAutomatic );
	axis.put_AutomaticMaximum( bAutomatic );
}

// ----------------------------------------------------------------------
// Function name  : KillHisTimer
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.07 16:20:00
// ----------------------------------------------------------------------
void CHistoryTrendView::KillHisTimer()
{
	if( m_uHisTimer != 0 )
	{
		KillTimer( m_uHisTimer );     // Kill timer
		m_uHisTimer = 0;
		m_nLastRate = 0;
	}
}

void CHistoryTrendView::SetHisTimer( UINT nElapse )
{
	m_uHisTimer = SetTimer( IDC_HISTIMER, nElapse, NULL );
	ASSERT( m_uHisTimer != 0 );
	m_nLastRate = nElapse;
}

// =======================================================================
// Function name  : OnTimer
// Description    : Timer Update
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.09 14:00:00
// =======================================================================
void CHistoryTrendView::OnTimer(UINT_PTR nIDEvent)
{
	if( IDC_HISTIMER == nIDEvent )
	{
		if( GetDocument()->IsRecording())
		{
			//AfxBeginThread( TimerUpdateThreadFunc, this );
			TimerUpdate();
		}
		else
		{
			KillHisTimer();
		}
	}

	CConfiguratorView::OnTimer(nIDEvent);
}

// ----------------------------------------------------------------------
// Function name  : TimerUpdate
// Description    : Close and then open
//                  May be recording or the file was changed when record full
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.07 16:20:00
// ----------------------------------------------------------------------
void CHistoryTrendView::TimerUpdate()
{
	m_pTimerUpdateEvent->ResetEvent();

	// Wait Recording(write into the current history file) Finish
	if( WAIT_OBJECT_0 == GetDocument()->WaitHisFileFinish())
	{
		GetDocument()->ResetHisFileFinishEvent();
		if( GetDocument()->IsRecordFileModified())  // new datas was added to file or create new record file after maximum size
		{
			//DWORD dwTime = GetTickCount();
			CString strFileName( m_pViewHisCtrl->GetFilePathName());
			ASSERT( !strFileName.IsEmpty());

			// Lock Series
			CSingleLock sl( &m_cs );
			sl.Lock();
			if( sl.IsLocked())
			{
				for( size_t i=0; i < SERIES_COUNT; ++i )
				{
					CString strNode( m_xmlSrcV[i].get_SeriesNode());
					if( !strNode.IsEmpty())
					{
						// XML Source File
						if( strFileName.CompareNoCase( m_xmlSrcV[i].get_FileName()) != 0 )
						{
							m_xmlSrcV[i].put_FileName( strFileName );
						}

						m_xmlSrcV[i].Close();
						m_xmlSrcV[i].Open();
					}
				}//for
                
				sl.Unlock();
			}

			/*dwTime = GetTickCount() - dwTime;
			CString str;
			str.Format( L"TimerUpdate used %dms", dwTime );
			theApp.LogInfo( str );*/

			GetDocument()->SetRecordFileModified( false );
		}//modified

		GetDocument()->SetHisFileFinishEvent();
	}//if HisFileFinish

	m_pTimerUpdateEvent->SetEvent();
}

// =======================================================================
// Function name  : OnUpdate
// Description    : Update View Contents
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 17:00:00
// =======================================================================
void CHistoryTrendView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if( !m_tChart.GetSafeHwnd())
	{
		return;
	}

	switch( lHint )
	{
		case CConfiguratorDoc::HIS_CLEAR:
			ClearView();
		break;
		case CConfiguratorDoc::HIS_INIT:
			InitUpdate();
		break;
		case CConfiguratorDoc::HIS_DYNUPDATE:
			DynUpdate();
		break;
		default:
		break;
	}
}

// ----------------------------------------------------------------------
// Function name  : ClearView
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.09 13:00:00
// ----------------------------------------------------------------------
void CHistoryTrendView::ClearView()
{
	if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
	{
		theApp.LogInfo( L"CHistoryTrendView::ClearView" );
	}

	/// If timer updating: kill timer and then close old datas
	/// Else, close old datas directly
	if( m_uHisTimer != 0 )
	{
		KillHisTimer();
	}
	
	DWORD dwRes = ::WaitForSingleObject( m_pTimerUpdateEvent->m_hObject, 10000 );
	ASSERT( WAIT_OBJECT_0 == dwRes );

	// Directly Close Existing
	for( size_t i=0; i < SERIES_COUNT; ++i )
	{
		m_xmlSrcV[i].Close();
		m_xmlSrcV[i].put_SeriesNode( L"" );
		m_xmlSrcV[i].put_FileName( L"" );

		CSeries aSeries;
		aSeries = (CSeries)m_tChart.Series( i );
		aSeries.put_Active( FALSE );
	}
	
	SetAxisAutomatic( true );	
}

// ----------------------------------------------------------------------
// Function name  : InitUpdate
// Description    : Automatic and DynUpdate
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.26 17:20:00
// ----------------------------------------------------------------------
void CHistoryTrendView::InitUpdate()
{
	SetAxisAutomatic( true );
	DynUpdate();
}

// ----------------------------------------------------------------------
// Function name  : DynUpdate
// Description    :
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.08 14:00:00
// ----------------------------------------------------------------------
void CHistoryTrendView::DynUpdate()
{
	ASSERT( m_pViewHisCtrl != NULL );
	if( NULL == m_pViewHisCtrl )
	{
		return;
	}

	if( CViewHistoryCtrl::VIEW_CURRENT == m_pViewHisCtrl->GetViewType())
	{
		UpdateCurHisData();
	}
	else if( CViewHistoryCtrl::VIEW_FILE == m_pViewHisCtrl->GetViewType())
	{
		UpdateFileData();
	}
}

// ----------------------------------------------------------------------
// Function name  : UpdateCurHisData
// Description    : Current history file maybe changed if it was recorded full
//                  and maybe recording datas to the current history file,
//                  so need to consider the R/W conflict, 
//                  and need to update the file name if it was changed
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.08 16:10:00
// ----------------------------------------------------------------------
void CHistoryTrendView::UpdateCurHisData()
{
	UpdateAxisFormat();       // Update Axis
	
	// Wait Recording(write into the current history file) Finish
	if( WAIT_OBJECT_0 == GetDocument()->WaitHisFileFinish())
	{
		GetDocument()->ResetHisFileFinishEvent();
		UpdateSeries( true );     // Update Series
		GetDocument()->SetHisFileFinishEvent();
	}

	// Update Rate
	if( m_nLastRate != m_pViewHisCtrl->GetUpdateRate())
	{
		KillHisTimer();
		SetHisTimer( m_pViewHisCtrl->GetUpdateRate()*1000 );
	}
}

// ----------------------------------------------------------------------
// Function name  : UpdateFileData
// Description    : History File was fixed
//                  Don't need to consider r/w conflict
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.08 16:00:00
// ----------------------------------------------------------------------
void CHistoryTrendView::UpdateFileData()
{
	UpdateAxisFormat(); // Update Axis
	UpdateSeries();     // Update Series
	
	// Kill Timer if it works
	if( m_uHisTimer != 0 )
	{
		KillHisTimer();
	}
}

// ----------------------------------------------------------------------
// Function name  : UpdateAxisFormat
// Description    : View Type was fixed
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.08 14:00:00
// ----------------------------------------------------------------------
void CHistoryTrendView::UpdateAxisFormat()
{
	// DateTime Format
	CAxes axes( m_tChart.get_Axis());
	CAxis axis( axes.get_Bottom());
	CAxisLabels label( axis.get_Labels());
	CString str( label.get_DateTimeFormat());
	if( str.CompareNoCase(m_pViewHisCtrl->GetTimeFormatW()) != 0 )
	{
		label.put_DateTimeFormat( m_pViewHisCtrl->GetTimeFormatW());
	}
}

// ----------------------------------------------------------------------
// Function name  : UpdateSeries
// Description    : View Type was fixed
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.08 14:00:00
// ----------------------------------------------------------------------
void CHistoryTrendView::UpdateSeries( bool bViewCurrent )
{
	CString strFileName( m_pViewHisCtrl->GetFilePathName());
	CHistoryViewDatas *pViewDatas = GetDocument()->m_aHisViewDatas;
	ASSERT((!strFileName.IsEmpty()) && (pViewDatas != NULL));

	// Lock Series
	CSingleLock sl( &m_cs );
	sl.Lock();
	if( sl.IsLocked())
	{
		for( size_t i=0; i < SERIES_COUNT; ++i )
		{
			CSeries aSeries( m_tChart.Series( i ));
			CPen0 aPen( aSeries.get_Pen());

			const CHistoryViewData *data = pViewDatas->GetHisViewData( i );
			ASSERT( data != NULL );
			if( data != NULL )
			{
				aSeries.put_Color( data->GetColor());  // Color
				aPen.put_Width( data->GetWidth());     // Pen Width

				if( data->GetFileData() != NULL )
				{
					CString strTagName( data->GetTagName());
					ASSERT( !strTagName.IsEmpty());

					// XML Source File and Series Node
					if( strFileName.CompareNoCase( m_xmlSrcV[i].get_FileName()) != 0 )
					{
						m_xmlSrcV[i].put_FileName( strFileName );

						m_xmlSrcV[i].Close();
						m_xmlSrcV[i].put_SeriesNode( strTagName );
						m_xmlSrcV[i].Open();
					} 
					else
					{
						if( strTagName.CompareNoCase( m_xmlSrcV[i].get_SeriesNode()) != 0 )
						{
							m_xmlSrcV[i].Close();
							m_xmlSrcV[i].put_SeriesNode( strTagName );
							m_xmlSrcV[i].Open();
						}
						else if( bViewCurrent )
						{
							m_xmlSrcV[i].Close();
							m_xmlSrcV[i].Open();
						}
					}

					aSeries.put_Active( TRUE );
					continue;
				}
			}
		
			m_xmlSrcV[i].Close();
			m_xmlSrcV[i].put_SeriesNode( L"" );
			m_xmlSrcV[i].put_FileName( L"" );
			aSeries.put_Active( FALSE );
		}//for

		sl.Unlock();
	}//Lock
}

// =======================================================================
// Function name  : Mytchart_OnScroll
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 17:10:00
// =======================================================================
void CHistoryTrendView::Mytchart_OnScroll()
{
    /*if((ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR) && (m_dwTrendTick != INFINITE))
    {
		CAxes axes = (CAxes)m_tChart.get_Axis();
		CAxis axis = axes.get_Bottom();

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
    }*/
}

// =======================================================================
// Function name  : Mytchart_OnDblClick
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.01 17:10:00
// =======================================================================
void CHistoryTrendView::Mytchart_MouseDown(long Button, long Shift, long X, long Y)
{
	ASSERT( m_pViewHisCtrl != NULL );
	if((m_pViewHisCtrl != NULL) && (MK_RBUTTON==Button))
	{
		CCanvas aCanvas( m_tChart.get_Canvas());
	    if( aCanvas.IsScreenHighColor() )
	    {
		    CPanel aPanel( m_tChart.get_Panel());
			
			long cx = m_tChart.get_Width();
			long cy = m_tChart.get_Height();
			long lMLeft = aPanel.get_MarginLeft();
			long lMRight = aPanel.get_MarginRight();
			long lMTop = aPanel.get_MarginTop();
			long lMBottom = aPanel.get_MarginBottom();
			long lXMin=0, lXMax=0, lYmin=0, lYMax=0;
			
			if( muPercent == aPanel.get_MarginUnits())
			{
				lXMin = cx*lMLeft/100;
				lXMax = cx - cx*lMRight/100;
				lYmin = cy*lMTop/100;
				lYMax = cy - cy*lMBottom/100;
			}
			else //muPixels
			{
				lXMin = lMLeft;
				lXMax = cx - lMRight;

				lYmin = lMTop;
				lYMax = cy - lMBottom;
			}

			if(((X>=lXMin) && (X<=lXMax)) && ((Y>=lYmin) && (Y<=lYMax)))
			{
				return;
			}
	    }

		CString strFilePathName( m_pViewHisCtrl->GetFilePathName());
		if( E_NONE == ValidateHisFile(strFilePathName))
		{
			CAxes axes( m_tChart.get_Axis());
			CAxis axis( axes.get_Bottom());
			double min = axis.get_Minimum();
			double max = axis.get_Maximum();

			GetDocument()->DynViewHistory( min );
		}
	}
}

// =======================================================================
// Function name  : SetAxisTime
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.24 16:40:00
// =======================================================================
void CHistoryTrendView::SetAxisTime( const double dStartTime )
{
	if( m_tChart.GetSafeHwnd())
	{
		CAxes axes( m_tChart.get_Axis());
		CAxis axis( axes.get_Bottom());
		axis.put_AutomaticMinimum( FALSE );
		axis.put_AutomaticMaximum( FALSE );

		double range = static_cast<double>(m_pViewHisCtrl->GetTimeRange())/(24*60*60);
		axis.put_Maximum( dStartTime + range );
		axis.put_Minimum( dStartTime );
	}
}
