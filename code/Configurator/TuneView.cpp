// TuneView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "TuneView.h"

// Added by wangdong
#include "MainFrm.h"
#include "ConfiguratorDoc.h"                  
#include "TuneFraView.h"  
#include "NonTuningFra.h"

// =======================================================================
///globals
#define TUNE_FRA_HEIGHT  90
#define TUNE_FRA_WIDTH   380   //220
// =======================================================================
// CTuneView

IMPLEMENT_DYNCREATE(CTuneView, CConfiguratorView)        // CView

CTuneView::CTuneView()
          :m_pTuningBase( NULL )
		  ,m_dwViewHelpID( HIDX_TUNING )
		  ,m_pNonTuningFra( NULL )
{
}

CTuneView::~CTuneView()
{
}

BEGIN_MESSAGE_MAP(CTuneView, CConfiguratorView)          // CView
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTuneView 绘图

void CTuneView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}

void CTuneView::OnInitialUpdate()
{
	CConfiguratorView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CSize size( 0, m_FraVector.size() * (TUNE_FRA_HEIGHT+10));
	SetScrollSizes( MM_TEXT, size );
}

// ======================================================
// Function name : OnEraseBkgnd
// Description   : Set Background Color
// Return type   : BOOL
// Parameters    : 
// Author        : WangDong 2011.02.17 13:00:00
// =======================================================
BOOL CTuneView::OnEraseBkgnd( CDC* pDC )
{
	CRect rect;
	GetClientRect( &rect );

	CBrush brush;
	if( !brush.CreateSolidBrush( GetSysColor( COLOR_BTNFACE )))        
	{
		return FALSE;
	}

	CBrush* pOldBrush = pDC->SelectObject( &brush );

	// Fill scroll and non-scroll section
	pDC->Rectangle( &rect );                  // Fill scroll section     - Logic Uints
	FillOutsideRect( pDC, &brush );           // Fill non-scroll section

	pDC->SelectObject( pOldBrush );

	return TRUE;
}

// ======================================================
// Function name : OnSize
// Description   : Change FraView's width only 
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.17 13:00:00
// =======================================================
void CTuneView::OnSize(UINT nType, int cx, int cy)
{
	// Scroll size
	CSize size( 0, m_FraVector.size() * (TUNE_FRA_HEIGHT+10));
	SetScrollSizes( MM_TEXT, size );

	TuneFrameVector::iterator it = m_FraVector.begin();
	TuneFrameVector::iterator eit = m_FraVector.end();

	while( it != eit )
	{
		CTuneFraView* pFraView = *it;
		if( pFraView->GetSafeHwnd())
		{
			CRect rect;
			pFraView->GetClientRect( &rect );

			// Change Width; SWP_NOMOVE: ignore x, y; SWP_NOZORDER: ignore the first parameter  
			pFraView->SetWindowPos( &CWnd::wndBottom, 0, 0, cx, rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
		}

		it++;
	}

	// DSC100
	if( theApp.GetProductType() == CConfiguratorApp::DSC100 )
	{
		if( m_pNonTuningFra->GetSafeHwnd())
		{
			CRect rect;
			m_pNonTuningFra->GetClientRect( &rect );

			// Change Width; SWP_NOMOVE: ignore x, y; SWP_NOZORDER: ignore the first parameter  
			m_pNonTuningFra->SetWindowPos( &CWnd::wndBottom, 0, 0, cx, rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
		}
	}
}


// CTuneView 诊断

#ifdef _DEBUG
void CTuneView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTuneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTuneView 消息处理程序
int CTuneView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( m_dwViewHelpID );       // Help ID
	
	return 0;
}

// =======================================================================
// Function name : OnUpdate
// Description   : 
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.02.15 14:35:00
// =======================================================================
void CTuneView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
	if((lHint != CConfiguratorDoc::HINT_REFRESH) || (AfxGetMainWnd() == NULL))
	{
		return;
	}

	try
	{
		size_t size = GetDocument()->m_aSettings->size();
		if( size > 0 )
		{
			bool bConnected = (((CMainFrame*)AfxGetMainWnd())->IsConnected()) &&
							  (global::Security.CanTune());

			if( bConnected && (CConfiguratorApp::DSC100 == theApp.GetProductType()))
            {
                CAliasItem *pAlias = GetDocument()->m_aAliases->find_name( L"bCTRLTYPE" );   // 2027
                if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
                {
                    bConnected = (pAlias->remBool() == VARIANT_FALSE);  // close loop
                }
            }
		
			CSettingItem* pSetting = GetDocument()->m_aSettings->FindHelpName( L"HIDX_TUNING" );
			if( pSetting == NULL )
			{
				m_pTuningBase = NULL;
				DestoryFrames();
				return;
			}

			if( theApp.GetResetViewFlag()   ||
				(m_pTuningBase != pSetting) || 
				((CConfiguratorApp::TS350 == theApp.GetProductType()) && (g_bCfgRefresh || g_bReadRefresh)))
			{
				m_pTuningBase = pSetting;
				DestoryFrames();
				CreateFrames();              // Create and Refresh TuneFrameViews
			}
			else  // Refresh TuneFrameViews
			{
				TuneFrameVector::iterator it = m_FraVector.begin();
				TuneFrameVector::iterator eit = m_FraVector.end();
				while( it != eit )
				{
					if( *it != NULL )
					{
					    (*it)->RefreshTuning( bConnected );
					}

					++it;
				}

				// DSC100
				if( theApp.GetProductType() == CConfiguratorApp::DSC100 )
				{
					if( m_pNonTuningFra->GetSafeHwnd())
					{
						m_pNonTuningFra->RefreshTuning( bConnected );
					}
				}
			}//else
		}//if( size > 0 )
	}
	catch( CException &e )
	{
		CString str;
		str.Format( L"Exception occured in CTuneView::OnUpdate - %d.", lHint );
		theApp.LogInfo( str );

		e.ReportError();
		e.Delete();
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CTuneView::OnUpdate - %d.", lHint );
		theApp.LogInfo( str );
	}
}

// =======================================================================
// Function name : CreateFrames
// Description   : Create and Refresh TuneFrameViews 
// Return type   : void
// Parameters    : void
// Author        : WangDong 2011.02.15 14:35:00
// =======================================================================
void CTuneView::CreateFrames( void )
{
	if( AfxGetMainWnd() == NULL )
	{
		return;
	}

	try
	{
		bool bConnected = ((CMainFrame*)AfxGetMainWnd())->IsConnected();
		CPoint point( GetScrollPosition());  //Scroll Position - Logic units

		if( CheckReadPtr( m_pTuningBase, sizeof(CSettingItem)))
		{
			int nIndex = 0;  // zero_based
	    
			CSettingVector* pSettingVector = m_pTuningBase->GetChildVector();
			if( pSettingVector != NULL )
			{ 
				CSettingVector::iterator it = pSettingVector->begin();
				CSettingVector::iterator eit = pSettingVector->end();
			
				while( it != eit )
				{
					CSettingItem* pSetting = *it;
					if( pSetting != NULL )
					{
						bool bIsPIDItem = pSetting->IsPIDSettingItem( pSetting->GetChildVector());
						if( bIsPIDItem )  // three settingitems: PB, RES, RATE
						{
							// TS350: 可视时，添加；否则，不添加。
							if( CConfiguratorApp::TS350 == theApp.GetProductType())
							{
								if( !pSetting->GetVisible())
								{
									it++;
									continue;
								}
							}// TS350

							CTuneFraView* pFraView = new CTuneFraView();
							if( pFraView != NULL )
							{
								// Device coordinate: need to be adjust by scroll size, it maybe less than 0.
								if( pFraView->Create( NULL, NULL, WS_CHILD | WS_VISIBLE & ~WS_BORDER,
									CRect(0 - point.x, (TUNE_FRA_HEIGHT+10)*nIndex -point.y, TUNE_FRA_WIDTH-point.x, (TUNE_FRA_HEIGHT+10)*nIndex+TUNE_FRA_HEIGHT-point.y ), 
													  this, IDC_TUNE_BASE+nIndex))
								{
									nIndex++;
									m_FraVector.push_back( pFraView );
									GetDocument()->AddView( pFraView );

									pFraView->BindSetting( pSetting );
									pFraView->RefreshTuning( bConnected );
								}
							}//if( pFraView != NULL )
						}//if( bIsPIDItem )
					}//if( pSetting != NULL )

					it++;
				}//while
			}//if( pSettingVector != NULL )
		}//if( CheckReadPtr( m_pTuningBase, sizeof(CSettingItem)))

		if( CConfiguratorApp::DSC100 == theApp.GetProductType())
		{
			m_pNonTuningFra = new CNonTuningFra();
			if( m_pNonTuningFra != NULL )
			{
				if( m_pNonTuningFra->Create( NULL, NULL, WS_CHILD | WS_VISIBLE & ~WS_BORDER,
					CRect(0 - point.x, (TUNE_FRA_HEIGHT+10)*m_FraVector.size() - point.y, TUNE_FRA_WIDTH - point.x, (TUNE_FRA_HEIGHT+10)*m_FraVector.size()+190 - point.y ), 
					this, IDC_TUNE_BASE+m_FraVector.size()))
				{
					GetDocument()->AddView( m_pNonTuningFra );

					m_pNonTuningFra->Bind();
					m_pNonTuningFra->RefreshTuning( bConnected );
				}
			}//if( m_pNonTuningFra != NULL )
		}//DSC100
	}//try
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CTuneView::CreateFrames!" );
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CTuneView::CreateFrames!" );
	}
}

// =======================================================================
// Function name : DestoryFrames
// Description   : Delete TuneFraViews 
// Return type   : void
// Parameters    : void
// Author        : WangDong 2011.02.15 17:00:00
// =======================================================================
void CTuneView::DestoryFrames( void )
{
	CMainFrame* pMainWnd = (CMainFrame*)theApp.m_pMainWnd;
	CView* pActiveView = pMainWnd->GetActiveView();

	TuneFrameVector::iterator it = m_FraVector.begin();
	TuneFrameVector::iterator eit = m_FraVector.end();

	while( it != eit )
	{
		if((*it)->GetSafeHwnd())
		{
			if( *it == pActiveView )
			{
				pMainWnd->SetActiveView( this );
			}

			GetDocument()->RemoveView( *it );
			SAFE_DELETE( *it );
		}

		++it;
	}

	m_FraVector.clear();
	m_FraVector.reserve(8);

	if( m_pNonTuningFra->GetSafeHwnd())
	{
		if( m_pNonTuningFra == pActiveView )
		{
			pMainWnd->SetActiveView( this );
		}

		GetDocument()->RemoveView( m_pNonTuningFra );
		SAFE_DELETE( m_pNonTuningFra );
	}
}
//-----------------------------------------------------------------------


