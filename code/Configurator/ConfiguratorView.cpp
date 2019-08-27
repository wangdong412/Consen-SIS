
// ConfiguratorView.cpp : implementation of the CConfiguratorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Configurator.h"
#endif

#include "ConfiguratorDoc.h"
#include "ConfiguratorView.h"
#include "MainFrm.h"
#include "mygridn.h"
#include "SettingGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ==================================================================
///globals
extern bool g_bPreView;
// ==================================================================

// CConfiguratorView

IMPLEMENT_DYNCREATE(CConfiguratorView, CScrollView)             // CView

BEGIN_MESSAGE_MAP(CConfiguratorView, CScrollView)               // CView
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CConfiguratorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CConfiguratorView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CConfiguratorView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CConfiguratorView::OnFileSaveAs)
END_MESSAGE_MAP()

// CConfiguratorView construction/destruction

CConfiguratorView::CConfiguratorView()
{
	// TODO: add construction code here
}

CConfiguratorView::~CConfiguratorView()
{
}

BOOL CConfiguratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);    //CView
}

// CConfiguratorView drawing

void CConfiguratorView::OnDraw(CDC* pDC)
{
	CConfiguratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
	{
		return;
	}

	// TODO: add draw code for native data here
}

// =======================================================================
// Function name: GetGridTotalVisibleRows
// Description  : 
// Return type  : Total Visible Rows Count
// Parameters   : 
// Author       : WangDong 2011.12.14 16:00:00
// =======================================================================
LONG CConfiguratorView::GetGridTotalVisibleRows( CMygridn &grid, std::vector<int> &vec, LONG lPageRows )
{
	LONG lResult = 0;

    for( long lRow = 0; lRow < grid.get_Rows(); lRow++ )
	{
		if( !grid.get_RowHidden(lRow))  // Visible
		{
			lResult++;

			if( 1 == lResult )
			{
				vec.push_back( lRow );    // 第一页的起始行数
			}
			else if( 1 == ((lResult - lPageRows) % (lPageRows-grid.get_FixedRows())))
			{
				vec.push_back( lRow );    // 后续页的起始行数
			}
		}//if Visible
	}//for

	return lResult;
}


// CConfiguratorView printing

void CConfiguratorView::OnFilePrintPreview()
{
	g_bPreView = true;

#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CConfiguratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	/* User can set CPrintInfo here, such as page count
	pInfo->SetMaxPage( 2 );
	pInfo->m_nNumPreviewPages = 2; */
	
	// default preparation
	return DoPreparePrinting(pInfo);
}

// =======================================================================
// Function name: OnBeginPrinting
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.06 14:00:00
// =======================================================================
void CConfiguratorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
}

void CConfiguratorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	CView::OnPrepareDC(pDC, pInfo);
}

// =======================================================================
// Function name: OnPrint
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.06 09:00:00
// =======================================================================
void CConfiguratorView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	///1. Print source: Grid in the SettingGridView
	CSettingGridView* pSettingGridView = (CSettingGridView*)(theApp.RightViewVector[CMainFrame::RSETTING_GRID]);
	if( !pSettingGridView )
	{
		return;
	}

	CMygridn& sGrid = pSettingGridView->GetViewGrid();     // Source Grid

	///2. GDI: Font, Pen, and Brush
	//2.0 BackMode
	pDC->SetBkMode( TRANSPARENT );

	//2.1 Grid Font
	// Bold Grid Font
	CFont gridBoldFont;
	LOGFONT logfont;
	memset( &logfont, 0, sizeof(LOGFONT));
	::lstrcpynW( logfont.lfFaceName, (LPCTSTR)sGrid.get_FontName(), 32 );
	logfont.lfHeight = static_cast<LONG>(sGrid.get_FontSize()*10);
	logfont.lfWeight = FW_BOLD;
	VERIFY( gridBoldFont.CreatePointFontIndirect( &logfont, pDC ));

	// Normal Grid Font
	CFont gridFont;
	VERIFY( gridFont.CreatePointFont( static_cast<int>(sGrid.get_FontSize()*10), sGrid.get_FontName(), pDC ));
	CFont* OriFont = pDC->SelectObject( &gridFont );

	//2.2 Pen
	CPen pen;
	VERIFY( pen.CreatePen( PS_SOLID, 10, RGB( 0,0,0 )));
	CPen* OriPen = pDC->SelectObject( &pen );

	//2.3 Brush
	HBRUSH hltGrayBrush = (HBRUSH)::GetStockObject( LTGRAY_BRUSH );
	CBrush ltGrayBrush;
	VERIFY( ltGrayBrush.Attach( hltGrayBrush ));
	CBrush* OriBrush = NULL;

	///3. Multiple pages
	//3.1 Set Window Origin
	int nRectHeight = pInfo->m_rectDraw.Height();
	pDC->SetWindowOrg( pInfo->m_rectDraw.left, (pInfo->m_nCurPage-1) * nRectHeight );

	//3.2 Adjust rect draw
	pInfo->m_rectDraw.top = (pInfo->m_nCurPage-1) * nRectHeight;
	pInfo->m_rectDraw.bottom = pInfo->m_nCurPage * nRectHeight;

	///4. Set Margin Top, Left, Bottom, Right: 9% / 4.5% of the total size 
	pInfo->m_rectDraw.left = pInfo->m_rectDraw.left + static_cast<LONG>(pInfo->m_rectDraw.Width()*0.09);
	pInfo->m_rectDraw.top = pInfo->m_rectDraw.top + static_cast<LONG>(pInfo->m_rectDraw.Height()*0.045);
	pInfo->m_rectDraw.right = pInfo->m_rectDraw.right - static_cast<LONG>(pInfo->m_rectDraw.Width()*0.09);
	pInfo->m_rectDraw.bottom = pInfo->m_rectDraw.bottom - static_cast<LONG>(pInfo->m_rectDraw.Height()*0.045);

	///5. Set Header and Footer
	//5.1.1 Header - File Title
	CString strFront, strFooterTime, strFooterPage(L"Print Date: ");
	strFront.Format( L"File: %s", GetDocument()->GetTitle());
	VERIFY( pDC->TextOutW( pInfo->m_rectDraw.left, pInfo->m_rectDraw.top, strFront )); 

	/* Header - Tri-Sen Logo
	HENHMETAFILE hmf = ::GetEnhMetaFileW( L"res\\Logo\\Tri-Sen Logo.emf" );
	ASSERT( hmf );
	CRect logoRect( pInfo->m_rectDraw.right - 786, pInfo->m_rectDraw.top, pInfo->m_rectDraw.right, pInfo->m_rectDraw.top+142 );
	pDC->PlayMetaFile( hmf, &logoRect );*/

	//5.2.1 Footer - Time
	SYSTEMTIME SysTime;
	::GetLocalTime( &SysTime );
	CTime time( SysTime );
	strFooterTime += time.Format( L"Print Date: %Y-%m-%d %H:%M" );
	VERIFY( pDC->TextOut( pInfo->m_rectDraw.left, pInfo->m_rectDraw.bottom, strFooterTime ));   
	
	//5.2.2 Footer - Page
	strFooterPage.Format( L"Page %d", pInfo->m_nCurPage );
	CSize size;
	size = pDC->GetTextExtent( strFooterPage );
	VERIFY( pDC->TextOut( pInfo->m_rectDraw.right - size.cx, pInfo->m_rectDraw.bottom, strFooterPage ));

	//5.3 Adjust rect top and bottom
	TEXTMETRIC tm;
	VERIFY( pDC->GetTextMetrics( &tm ));
	pInfo->m_rectDraw.top += (tm.tmHeight)*15/10;
	pInfo->m_rectDraw.bottom -= tm.tmHeight;

	///6. Get row height, visible col width and convert 
	long lHeight = sGrid.get_RowHeight( 1 );                   // Row Height
	long lWidth = 0;
	for( long i = 0; i < sGrid.get_Cols(); i++ )
	{
		if( !sGrid.get_ColHidden( i ))
		{
			lWidth += sGrid.get_ColWidth( i );                 // Visible Column width
		}
	}
	
	//6.1 Width radio and Height
	ASSERT( lWidth );
	double dWidthRadio = 0.0;
	if( lWidth != 0 )
	{
	    dWidthRadio = pInfo->m_rectDraw.Width() / static_cast<double>(lWidth);
	}
	LONG lRowHeight = static_cast<LONG>(tm.tmHeight * 1.5);

	///7. Page rows: start - end
	LONG lPageRows = pInfo->m_rectDraw.Height() / lRowHeight;   // Page Rows
	LONG lUnfixedRows = lPageRows - sGrid.get_FixedRows();
	std::vector<int> StartRowsVector;
	LONG lGridTotalVisibleRows = GetGridTotalVisibleRows( sGrid, StartRowsVector, lPageRows );
	UINT uPageCounts = ((lGridTotalVisibleRows - sGrid.get_FixedRows()) + (lUnfixedRows -1)) / lUnfixedRows;
	pInfo->SetMaxPage( uPageCounts );

	long lPageStartRow = lPageRows*( pInfo->m_nCurPage - 1 );                                  // Start Row
	long lTotalRows = lGridTotalVisibleRows + (pInfo->GetMaxPage() -1)*sGrid.get_FixedRows();  // Total Rows: each page have fixed rows
	long lPageEndRow = -1;
	if( lPageStartRow + lPageRows < lTotalRows )                                               // End Row
	{
		lPageEndRow = lPageStartRow + lPageRows;                
	}
	else
	{
		lPageEndRow = lTotalRows;                       
	}

	///8. Draw 
	CPen MyPen( PS_SOLID, 2, RGB(0,0,0));
	CPen* pOriPen = (CPen*)pDC->SelectObject( &MyPen );

    // Top Row line
	pDC->MoveTo( pInfo->m_rectDraw.left, pInfo->m_rectDraw.top );
	pDC->LineTo( pInfo->m_rectDraw.right,pInfo->m_rectDraw.top ); 

	// Left Col Line
    pDC->MoveTo( pInfo->m_rectDraw.left, pInfo->m_rectDraw.top );
	pDC->LineTo( pInfo->m_rectDraw.left, pInfo->m_rectDraw.top + (lRowHeight * (lPageEndRow - lPageStartRow))); 

	// Text
	ASSERT( pInfo->m_nCurPage <= StartRowsVector.size());
	long lGridRow = StartRowsVector[ pInfo->m_nCurPage - 1 ];

	for( long lRow = lPageStartRow; lRow < lPageEndRow; lRow++ )
	{
		// Row line
		int nRowLineY = (lRow+1)%lPageRows*lRowHeight + pInfo->m_rectDraw.top;
		if( lRow + 1 == lPageEndRow )
		{
            nRowLineY = (lPageEndRow - lPageStartRow)*lRowHeight + pInfo->m_rectDraw.top;
		}

		pDC->MoveTo( pInfo->m_rectDraw.left,  nRowLineY );
		pDC->LineTo( pInfo->m_rectDraw.right, nRowLineY ); 

		// Check if fixed rows
		bool bFixedRows = false;
		if( lRow % lPageRows < sGrid.get_FixedRows())
		{
			bFixedRows = true;

			// Fill Rect
			CRect fillRect( pInfo->m_rectDraw.left
				          , nRowLineY - lRowHeight
						  , pInfo->m_rectDraw.right
						  , nRowLineY);

            // Brush
			OriBrush = pDC->SelectObject( &ltGrayBrush );
			pDC->FillRect( &fillRect, &ltGrayBrush );  
		}

		if( bFixedRows )
		{
			lGridRow = lRow % lPageRows;
			ASSERT( !sGrid.get_RowHidden(lGridRow));
		}
		else
		{
			if( sGrid.get_RowHidden( lGridRow )) 
			{
				++lGridRow;
				while( sGrid.get_RowHidden( lGridRow ))
				{
					++lGridRow;
				}
			}
		}//else

		lWidth = 0;
	    for( long lCol = 0; lCol < sGrid.get_Cols(); lCol++ )
	    {
		    if( !sGrid.get_ColHidden( lCol ))
		    {
				int nDepth = sGrid.get_RowOutlineLevel( lGridRow );
			
				// Rect
				CRect rect( pInfo->m_rectDraw.left + static_cast<LONG>(lWidth * dWidthRadio) 
					      , pInfo->m_rectDraw.top  + (lRow % lPageRows) * lRowHeight
                          , pInfo->m_rectDraw.left + static_cast<LONG>((lWidth + sGrid.get_ColWidth(lCol)) * dWidthRadio) 
						  , pInfo->m_rectDraw.top  + (lRow % lPageRows) * lRowHeight + lRowHeight);

				// Col line
                pDC->MoveTo( rect.right, rect.top );
		        pDC->LineTo( rect.right, rect.bottom ); 

				if( lCol == 0 )
				{
					rect.left += nDepth*tm.tmAveCharWidth;  
				}

				rect.top += lRowHeight*1/5;
				rect.left += tm.tmAveCharWidth; 

				// Font Bold
				_variant_t var_t = sGrid.get_Cell( CMygridn::flexcpFontBold, _variant_t(lGridRow), _variant_t(lCol),
					                               _variant_t(lGridRow), _variant_t(lCol));
				bool bBold = var_t ? true : false;
				if( bBold )
				{
					pDC->SelectObject( &gridBoldFont );
				}
				else
				{
					pDC->SelectObject( &gridFont );
				}

				// CheckBox
				var_t = sGrid.get_Cell( CMygridn::flexcpChecked, _variant_t(lGridRow), _variant_t(lCol),
					                    _variant_t(lGridRow), _variant_t(lCol));
				
				if( var_t.lVal != CMygridn::flexNoCheckbox )
				{
					pDC->SelectObject( &gridBoldFont );
					CString str( L" V " );
					CSize size = pDC->GetTextExtent( str );
					CRect rcCheckBox( rect.left, rect.top, rect.left + size.cx, rect.top + size.cy );
					pDC->Rectangle( &rcCheckBox );

					if( var_t.lVal == CMygridn::flexChecked )
					{
						pDC->DrawText( str, rect, DT_BOTTOM | DT_LEFT );    
					}
				}
				else
				{
				    pDC->DrawText( sGrid.get_TextMatrix( lGridRow, lCol), &rect, DT_BOTTOM | DT_LEFT );
				}

				lWidth += sGrid.get_ColWidth(lCol);
		    }//if( !sGrid.get_ColHidden( lCol ))
	    }//for cols

		// Restore Brush
		pDC->SelectObject( OriBrush );

		if( bFixedRows )
		{
			if( 1 == pInfo->m_nCurPage )
			{
				lGridRow++;
			}
			else
			{
				lGridRow = StartRowsVector[ pInfo->m_nCurPage - 1 ];
			}
		}
		else
		{
			lGridRow++;
		}
	}//for rows

	//Reset Pen
	pDC->SelectObject( pOriPen );

	//Reset font
	pDC->SelectObject( OriFont );
	pDC->SelectObject( OriPen );

	// Delete the created object
	VERIFY( gridFont.DeleteObject());
	VERIFY( gridBoldFont.DeleteObject());

	VERIFY( pen.DeleteObject());

	VERIFY( ltGrayBrush.DeleteObject());
	
	CView::OnPrint(pDC, pInfo);
}

void CConfiguratorView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	if( g_bPreView )
	{
	    g_bPreView = false;
	}
}

void CConfiguratorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CConfiguratorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CConfiguratorView diagnostics

#ifdef _DEBUG
void CConfiguratorView::AssertValid() const
{
	CView::AssertValid();
}

void CConfiguratorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CConfiguratorDoc* CConfiguratorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CConfiguratorDoc)));
	return (CConfiguratorDoc*)m_pDocument;
}
#endif //_DEBUG


// CConfiguratorView message handlers

// ====================================================================
// Function name: OnFileOpen
// Description  : 1. SaveModified() 
//                2. Open File 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 17:00:00
// =====================================================================
void CConfiguratorView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	if( !GetDocument()->IsStoppingHistoryRecord())
	{
		return;
	}

	if( !GetDocument()->SaveModified())
	{
		return;
	}

	// Create file document
	std::auto_ptr<CFileDialog> dlg( new CFileDialog( true ));

	// File filter
	switch( theApp.GetProductType())
	{
		case CConfiguratorApp::DSC100:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC100 Files (*.DSC100)\0*.DSC100\0\0";
	    break;
	    case CConfiguratorApp::TS350:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen TS350 Files (*.TS350)\0*.TS350\0\0";
	    break;
		case CConfiguratorApp::DSC110:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC110 Files (*.DSC110)\0*.DSC110\0\0";
	    break;
		default:
			dlg->m_ofn.lpstrFilter = L"Tri-Sen DSC100 Files (*.DSC100)\0*.DSC100\0\0";
	}

	// Def Ext
	dlg->m_ofn.lpstrDefExt = theApp.GetProductName();  

	if( dlg->DoModal() == IDOK )
	{
		if( AfxGetMainWnd()->GetSafeHwnd())
	    {
            ((CMainFrame*)AfxGetMainWnd())->DoDisconnect();
	    }

		GetDocument()->SetModifiedFlag( FALSE );    // Avoid ReSaveModified
		CString strPath = dlg->GetPathName();
		theApp.OpenDocumentFile( strPath );
	}
}

// ====================================================================
// Function name: OnFileSave
// Description  : 1. If has not been saved, do SaveAs...
//                2. If has been saved, 
//                   check modified flag, if true, save it
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 17:00:00
// =====================================================================
void CConfiguratorView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	if( GetDocument())
	{
	    GetDocument()->DoSaveDocument();
	}
}

// ===========================================================
// Function name: OnFileSaveAs
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 16:00:00
// ============================================================
void CConfiguratorView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if( GetDocument())
	{
	    GetDocument()->SaveAsDocument();
	}
}
