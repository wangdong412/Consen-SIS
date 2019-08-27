// TagsGridView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "TagsGridView.h"

// Added by wangdong
#include "ConfiguratorDoc.h"   
#include "MainFrm.h"
#include "HistoryServer.h"

// =======================================================================
inline _variant_t GridCheckedSetting(bool Value)
{
    if( Value )
	{
        return static_cast<long>(CMygridn::flexChecked);
	}
    else
	{
        return static_cast<long>(CMygridn::flexUnchecked);
	}
}

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

// CTagsGridView

IMPLEMENT_DYNCREATE(CTagsGridView, CBaseGridView)            // CView->CBaseGridView

CTagsGridView::CTagsGridView()
              :m_eUpdateType( NONE )
{

}

CTagsGridView::~CTagsGridView()
{
}

//============================================================================
BEGIN_MESSAGE_MAP(CTagsGridView, CBaseGridView)            // CView->CBaseGridView
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ALIASES_READ, &CTagsGridView::OnAliasesRead)
	ON_COMMAND(ID_ALIASES_WRITE, &CTagsGridView::OnAliasesWrite)
END_MESSAGE_MAP()
//============================================================================

//============================================================================
BEGIN_EVENTSINK_MAP(CTagsGridView, CBaseGridView)
	ON_EVENT(CTagsGridView, IDC_BASEGRID, 18,               CTagsGridView::Mygridn_AfterUserResize, VTS_I4 VTS_I4)
	//ON_EVENT(CTagsGridView, IDC_BASEGRID, 20,             CTagsGridView::Mygridn_AfterCollapse, VTS_I4 VTS_I2)  // 待定
	ON_EVENT(CTagsGridView, IDC_BASEGRID, 21,               CTagsGridView::Mygridn_BeforeEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CTagsGridView, IDC_BASEGRID, 2,                CTagsGridView::Mygridn_RowColChange, VTS_NONE)
	ON_EVENT(CTagsGridView, IDC_BASEGRID, 23,               CTagsGridView::Mygridn_ValidateEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CTagsGridView, IDC_BASEGRID, DISPID_MOUSEDOWN, CTagsGridView::Mygridn_MouseDown, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	//ON_EVENT(CTagsGridView, IDC_BASEGRID, DISPID_KEYDOWN,   CTagsGridView::Mygridn_KeyDown, VTS_PI2 VTS_I2)   //建议删除
END_EVENTSINK_MAP()
//============================================================================


// CTagsGridView 绘图

void CTagsGridView::OnDraw(CDC* pDC)
{
	// CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CTagsGridView 诊断

#ifdef _DEBUG
void CTagsGridView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTagsGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTagsGridView 消息处理程序

// =======================================================================
// Function name  : OnCreate
// Description    : Create View
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.05 15:40:00
// =======================================================================
int CTagsGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseGridView::OnCreate(lpCreateStruct) == -1)
	{
		theApp.LogInfo( L"CBaseGridView::OnCreate in CTagsGridView is failed!" );
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	// Help information
	SetWindowContextHelpId( HIDR_TAGSGRID );

	if( !m_sGrid.GetSafeHwnd())
	{
		theApp.LogInfo( L"m_sGrid in CTagsGridView is invalid!" );
        return 0;
	}

	try
	{
		m_sGrid.put_Redraw( CMygridn::flexRDNone );

		ApplyFont();  // Set Font

		/// Initial parameters
		// Rows and Cols
		m_sGrid.put_Rows( 1L );
		m_sGrid.put_Cols( COL_LAST + 1 );

		// Fixed Count of Rows and Cols 
		m_sGrid.put_FixedRows( 1L );
		m_sGrid.put_FixedCols( 0L );

		// Color
		//m_sGrid.put_BackColor( global::ShadeColor );     // 实际表格除固定行/列外的背景色：clSilver
		m_sGrid.put_BackColor( RGB(245,245,245));
		m_sGrid.put_BackColorBkg( global::clWindow );      // 视图中，实际表格外的其他区域：white
		m_sGrid.put_SheetBorder( global::clWindow );       // 实际表格的边界色：white
		if( global::ShadeColor == global::clSilver )
		{
			m_sGrid.put_GridColor( global::clBtnShadow );  // 实际表格线的颜色：灰色
		}

		// Selection
		// BigSelection：true: when clicking on the fixed area, 
		//                     entire columns and rows will be selected 
	    // Selection: true: Can select ranges of cells with the mouse and keyboard.
		m_sGrid.put_AllowBigSelection( false );
		m_sGrid.put_AllowSelection( true );

		// Resize
		m_sGrid.put_AllowUserResizing( CMygridn::flexResizeColumns );

		// Editable: bouth key board and mouse
		m_sGrid.put_Editable( CMygridn::flexEDKbdMouse );

		m_sGrid.put_FillStyle( CMygridn::flexFillSingle );                // Current cell
		m_sGrid.put_OutlineBar( CMygridn::flexOutlineBarCompleteLeaf );   // ？
		m_sGrid.put_ScrollTrack( true );                                  // Scroll will occur when user moves the scroll thumb

		// Initialize Display Grid
		CString s;
		s.LoadStringW( IDS_DES );
		m_sGrid.put_TextMatrix( 0, COL_DESC, s );      // Description

		s.LoadStringW( IDS_VARIABLE );
        m_sGrid.put_TextMatrix( 0, COL_VARIABLE, s );  // Variable

		s.LoadStringW( IDS_NUMBER );
        m_sGrid.put_TextMatrix( 0, COL_NUMBER, s );    // Number

		s.LoadStringW( IDS_REMOTE );
        m_sGrid.put_TextMatrix( 0, COL_REMOTE, s );    // Remote

		s.LoadStringW( IDS_LOCAL );
        m_sGrid.put_TextMatrix( 0, COL_LOCAL, s );     // Local

		s.LoadStringW( IDS_RECORD );
        m_sGrid.put_TextMatrix( 0, COL_RECORD, s );    // Record

		// Alignment: flexAlignLeftTop
		m_sGrid.put_ColAlignment( COL_REMOTE, CMygridn::flexAlignLeftTop );
		m_sGrid.put_ColAlignment( COL_LOCAL, CMygridn::flexAlignLeftTop );

		// 0 row: font bold
		m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t(0), _variant_t(0), 
			              _variant_t(0), _variant_t(COL_LAST), _variant_t(true));

		// Column width
		for( int i = 0; i < global::TAGS_GRID_COLUMNS; ++i )
		{
			if( global::TagsGridColWidths[i] > 0 )
			{
				m_sGrid.put_ColWidth( i, global::TagsGridColWidths[i] );
			}
		}

		// If not visible，set visible
	    if( !m_sGrid.IsWindowVisible() )
	    {
	        m_sGrid.ShowWindow( SW_SHOW );
	    }

		m_sGrid.put_Redraw( CMygridn::flexRDDirect );
	}
	catch( COleException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CTagsGridView::OnCreate!" );
		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CTagsGridView::OnCreate!");
        return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : OnUpdate
// Description    : Update View Contents
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.05 16:00:00
// =======================================================================
void CTagsGridView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

		long lItemCount = GetDocument()->m_aSettingsAlias->GetTotalCount();
		if( lItemCount > 0 )
		{
			if( lHint == CConfiguratorDoc::HINT_REFRESH )
			{
                m_eUpdateType = REFRESH;

				/*if( !(((CMainFrame*)AfxGetMainWnd())->IsConnected()) &&
					 (theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL ))
				{
					theApp.LogInfo( L"CTagsGridView::OnUpdate - Refresh." );
				}*/
			}
			else if( lHint == CConfiguratorDoc::HINT_MONITOR )
			{
                m_eUpdateType = MONITOR;
			}
			else if( lItemCount + 1 != m_sGrid.get_Rows())
			{
                m_eUpdateType = REFRESH;
			}
			else
			{
                m_eUpdateType = NONE;
			}
			
			if( m_eUpdateType != NONE )
			{
                CAliasMap* pAliases = GetDocument()->m_aAliases;
				if( CheckWritePtr( pAliases, sizeof(CAliasMap)))
				{
					// Lock the aliases for reading
                    CSingleLock sl( &pAliases->m_cirsec );
		            sl.Lock();
		            ASSERT( sl.IsLocked());

					// Mark all cells as normal
					m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t(0L), _variant_t(0L),
						               _variant_t(m_sGrid.get_Rows()-1L), _variant_t(m_sGrid.get_Cols()-1L), 
									   _variant_t(global::clWindowText));

					if( theApp.GetResetViewFlag())
					{
						// Initialize Display Grid
						CString s;
						s.LoadStringW( IDS_DES );
						m_sGrid.put_TextMatrix( 0, COL_DESC, s );      // Description

						s.LoadStringW( IDS_VARIABLE );
						m_sGrid.put_TextMatrix( 0, COL_VARIABLE, s );  // Variable

						s.LoadStringW( IDS_NUMBER );
						m_sGrid.put_TextMatrix( 0, COL_NUMBER, s );    // Number

						s.LoadStringW( IDS_REMOTE );
						m_sGrid.put_TextMatrix( 0, COL_REMOTE, s );    // Remote

						s.LoadStringW( IDS_LOCAL );
						m_sGrid.put_TextMatrix( 0, COL_LOCAL, s );     // Local

						s.LoadStringW( IDS_RECORD );
						m_sGrid.put_TextMatrix( 0, COL_RECORD, s );   // Record

						m_sGrid.put_Rows( 1L );

						// Column width
						for( int i = 0; i < global::TAGS_GRID_COLUMNS; ++i )
						{
							if( global::TagsGridColWidths[i] > 0 )
							{
								m_sGrid.put_ColWidth( i, global::TagsGridColWidths[i] );
							}
						}
					}

					// Start adding all of the settings
                    long lRow = 1;
					m_sGrid.put_Redraw( CMygridn::flexRDNone );

					AddSettings( GetDocument()->m_aSettingsAlias, lRow, 0 );

					long lRows = m_sGrid.get_Rows();
					if( lRows > 0 )
					{
						m_sGrid.put_Cell( CMygridn::flexcpPictureAlignment, _variant_t(0), _variant_t(0),
							              _variant_t(lRows-1), _variant_t(COL_LAST), 
										  _variant_t(CMygridn::flexAlignCenterTop));
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

					sl.Unlock();
				}
			}//if( m_eUpdateType != NONE )
		}//if( lItemCount > 0 )
		else   // lItemCount == 0
		{
			m_sGrid.put_Rows( 1L );
		}
	}
	catch( CException &e )
	{
        e.ReportError();
		e.Delete();

		CString str;
		str.Format( L"Exception occured in CTagsGridView::OnUpdate - %d!", lHint );
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CTagsGridView::OnUpdate - %d!", lHint );
		theApp.LogInfo( str );
	}
}

// =======================================================================
// Function name  : AddSettings
// Description    : Add Settings Alias Item
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.05 17:20:00
// =======================================================================
void CTagsGridView::AddSettings( CSettingVector* pSettings, long& lRow, long lDepth )
{
	if( lDepth == 0 )
	{
		/* When Refresh, clear all rows except the first row;
		// When Monitor, don't need to do this. 
		if( m_eUpdateType == REFRESH )
		{
			m_sGrid.put_Rows( 1L );  
		}*/

		long lRows = pSettings->GetTotalCount() + 1;
        m_sGrid.put_Rows( lRows );

		// When refresh, reset BackColor and ForeColor
        if( lRows > 1 && (m_eUpdateType == REFRESH))
        {
            // Reset all background colors
            m_sGrid.put_Cell( CMygridn::flexcpBackColor, _variant_t(1), _variant_t(0),
				              _variant_t(lRows-1), _variant_t(COL_LAST),
							  _variant_t(m_sGrid.get_BackColor()));         

            // Reset all font colors
			m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t(1), _variant_t(0),
				              _variant_t(lRows-1), _variant_t(COL_LAST),
							  _variant_t(global::clWindowText));             // Black
        }
	}//if( lDepth == 0 )

	if( CheckReadPtr( pSettings, sizeof(CSettingVector)))
	{
		for( CSettingVector::iterator it = pSettings->begin();
			 it != pSettings->end(); it++ )
		{
			CSettingItem *pSetting = *it;

			// Set Row Data
			m_sGrid.put_RowData( lRow, _variant_t((long)pSetting));

			// Desc Column
			m_sGrid.put_TextMatrix( lRow, COL_DESC, pSetting->GetName_C()); 

			bool bVariable = ( pSetting->GetAlias() != NULL );
			if( bVariable )
			{
				const CAliasItem* pAlias = pSetting->GetAlias();

				/// Color
				// BackColor of Local and Capture columns
				if( pAlias->IsWriteable() && global::Security.CanEditAlias(pAlias))
                {
					m_sGrid.put_Cell( CMygridn::flexcpBackColor, _variant_t( lRow ), _variant_t(COL_LOCAL),
						              _variant_t( lRow ), _variant_t( COL_RECORD ),
						              _variant_t( global::clWindow ));                         // white
                }
                else
                {
					m_sGrid.put_Cell( CMygridn::flexcpBackColor, _variant_t( lRow ), _variant_t(COL_RECORD),
						              _variant_t( lRow ), _variant_t( COL_RECORD ),
						              _variant_t( global::clWindow ));                         // white
                }

				// Font Color
				if( pAlias->IsDirtyLocal())
				{
					m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t(0),
						              _variant_t( lRow ), _variant_t( COL_LAST ),
						              _variant_t( global::DirtyLocalColor ));           // DirtyLocal: 紫色
				}
				else if( pAlias->IsDirtyRemote())
				{
					m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t(0),
						              _variant_t( lRow ), _variant_t( COL_LAST ),
									  _variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
				}
				else
				{
                    m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t(0),
						              _variant_t( lRow ), _variant_t( COL_LAST ),
									  _variant_t( global::clWindowText ));              // 黑色
				}

				// Variable Column
				m_sGrid.put_TextMatrix( lRow, COL_VARIABLE, pSetting->GetVariableName_C());

				// Number Column
				m_sGrid.put_TextMatrix( lRow, COL_NUMBER, pAlias->GetAliasString_c());

				// Remote and Local Column
				if( pSetting->IsBoolean())
				{
					// Remote Column
					_variant_t var_t = GridCheckedSetting( pSetting->IsCheckedCmp());
					m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t( COL_REMOTE ),
						              _variant_t( lRow ), _variant_t( COL_REMOTE ), var_t); 

					// Local Column
					var_t = GridCheckedSetting( pSetting->IsChecked());
					m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t( COL_LOCAL ),
						              _variant_t( lRow ), _variant_t( COL_LOCAL ), var_t);
				}
				else
				{
					CString s;

					// Remote Column
					pSetting->GetValueCmp( s );
					m_sGrid.put_TextMatrix( lRow, COL_REMOTE, s );

					// Local Column
					s.Empty();
					pSetting->GetValue( s );
					m_sGrid.put_TextMatrix( lRow, COL_LOCAL, s );
				}

				// Capture
				_variant_t varCapture_t = (pAlias->GetRecord() ? static_cast<long>(CMygridn::flexChecked) :
					                                             static_cast<long>(CMygridn::flexUnchecked));

				m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t( COL_RECORD ),
						              _variant_t( lRow ), _variant_t( COL_RECORD ), varCapture_t );
			}// if( bVariable )
			else
			{
				m_sGrid.put_TextMatrix( lRow, COL_VARIABLE, L"");  // Variable Column
				m_sGrid.put_TextMatrix( lRow, COL_NUMBER, L"");    // Number Column
				m_sGrid.put_TextMatrix( lRow, COL_LOCAL, L"");     // Local
				m_sGrid.put_TextMatrix( lRow, COL_REMOTE, L"");    // Remote
				m_sGrid.put_TextMatrix( lRow, COL_RECORD, L"");    // Capture
			}

			m_sGrid.put_IsSubtotal( lRow, true );
			m_sGrid.put_RowOutlineLevel( lRow, static_cast<short>(pSetting->GetDepth()));

			// Hidden
			bool bHidden = !pSetting->GetVisible();
			m_sGrid.put_RowHidden( lRow, bHidden );

			if( pSetting->GetChildVector())
			{
				m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t( lRow ), _variant_t(0),
						          _variant_t( lRow ), _variant_t( COL_LAST ),
								  _variant_t( true )); 
				++lRow;
				AddSettings( pSetting->GetChildVector(), lRow, pSetting->GetDepth() + 1 );

				//if (bHidden)
                //{
                    //OLECHECK(m_spGrid->set_IsCollapsed(nRowParent, flexOutlineCollapsed));
                //}
			}
			else
			{
				m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t( lRow ), _variant_t(0),
						          _variant_t( lRow ), _variant_t( COL_LAST ),
								  _variant_t( false )); 

                ++lRow;
			}
		}//for
	}//if( CheckReadPtr( pSettings, sizeof(CSettingVector)))
}

// =======================================================================
// Function name  : OnSize
// Description    : Resize
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.05 16:20:00
// =======================================================================
void CTagsGridView::OnSize(UINT nType, int cx, int cy)
{
	CBaseGridView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_sGrid.GetSafeHwnd())
	{
		m_sGrid.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
		RefreshColumnSizes();
	}
}

// =======================================================================
// Function name  : RefreshColumnSizes
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.13 10:00:00
// =======================================================================
void CTagsGridView::RefreshColumnSizes()
{
    if( m_sGrid.GetSafeHwnd())
	{	
		for( int i = 0; i < global::TAGS_GRID_COLUMNS; ++i )
		{
			global::TagsGridColWidths[i] = m_sGrid.get_ColWidth(i);
		}
	}
}

// =======================================================================
/// VSFLEXGRID EVENT
// =======================================================================
// Function name  : Mygridn_AfterUserResize
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.13 10:10:00
// =======================================================================
void CTagsGridView::Mygridn_AfterUserResize(long Row, long Col)
{
    RefreshColumnSizes();
}

/* =======================================================================
// Function name  : Mygridn_AfterCollapse
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CTagsGridView::Mygridn_AfterCollapse(long Row, short State)
{
	// TODO: 在此处添加消息处理程序代码
}*/

// =======================================================================
// Function name  : Mygridn_BeforeEdit
// Description    : 检查用户是否能够编辑单元格中的内容
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CTagsGridView::Mygridn_BeforeEdit(long Row, long Col, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	if((Row == 0) || ((Col != COL_LOCAL) && (Col != COL_RECORD)))
    {
        *Cancel = true;
		return;
    }

	// Check if user has a valid password
    if( !global::Security.IsValid())
    {
	    *Cancel = true;
		return;
    }

    // Check if row has valid SettingItem
    CSettingItem* pSetting = GetRowSetting( Row );
    if( !CheckReadPtr( pSetting, sizeof(CSettingItem)))
    {
        *Cancel = true;
		return;
    }

    // Check if setting has valid associated alias
	CAliasItem* pAlias = pSetting->GetAlias(); 
    if( !CheckReadPtr( pAlias, sizeof(CAliasItem)))
    {
        *Cancel = true;
		return;
    }

	// If click Local column, Check if alias was writeable
	if( Col == COL_LOCAL )
	{
		if( pAlias->IsReadOnly())   // ReadOnly
		{
            *Cancel = true;
		    return;
		}

		// Check permissions...
        if( !global::Security.CanEditAlias(pSetting->GetAlias()))
        {
            *Cancel = true;
		    return;
        }
	}
	else if( Col == COL_RECORD )
    {
		// If history recording, Unable to edit record column
		if( GetDocument()->IsRecording())   
		{
            *Cancel = true;
	        return;
		}
    }
	else
	{
        *Cancel = true;
		return;
	}
}

// =======================================================================
// Function name  : Mygridn_RowColChange
// Description    : Auto Search, ????
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CTagsGridView::Mygridn_RowColChange()
{
	// TODO: 在此处添加消息处理程序代码
	long Col = m_sGrid.get_Col();

    // Set the autosearch if on a non-editable column
    m_sGrid.put_AutoSearch(((Col == COL_LOCAL) || (Col == COL_RECORD)) ?
                            CMygridn::flexSearchNone : CMygridn::flexSearchFromCursor );
}

// =======================================================================
// Function name  : Mygridn_ValidateEdit
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CTagsGridView::Mygridn_ValidateEdit(long Row, long Col, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	do
    {
		if( Row <= 0 )
		{
		    break;
		}

		CSettingItem *pSetting = GetRowSetting(Row);
		if( CheckReadPtr(pSetting, sizeof(CSettingItem)) == FALSE )
		{
			break;
		}

		CAliasItem *pAlias = pSetting->GetAlias();
		if( CheckReadPtr(pAlias, sizeof(CAliasItem)) == FALSE )
		{
			break;
		}

		CString strValue = m_sGrid.get_EditText();  // Get the edited value

		// Check if the value is a checkbox
		_variant_t vChecked = m_sGrid.get_Cell( CMygridn::flexcpChecked,
			                                    _variant_t(Row), _variant_t(Col),
			                                    _variant_t(Row), _variant_t(Col));
		if( static_cast<short>(vChecked) != static_cast<short>(CMygridn::flexNoCheckbox))
		{
			// Convert text to "0" if not check
			try
			{
				int n = _wtoi( strValue );
				if( n != CMygridn::flexChecked ) 
				{
					strValue = "0";
				}
			}
			catch(...)
			{
			    strValue = "0";
			}
		}

		if (Col == COL_RECORD)
		{
			// If history recording, Unable to edit record column
			if(((CMainFrame*)AfxGetMainWnd())->IsRecording())
			{
			    break;
		    }

			bool bRecord = strValue.CompareNoCase( L"1" ) == 0;
			if( bRecord != pAlias->GetRecord())
			{
				pAlias->SetRecord( bRecord );
				GetDocument()->SetRecordModified( true );
			}
		}
		else if (Col == COL_LOCAL)
		{
			if( !pAlias->IsWriteable())
			{
			    break;
			}

			pSetting->SetValue( strValue );
			if( !pSetting->IsBoolean())
			{
			    CString s;
			    pSetting->GetValue( s );

				m_sGrid.put_EditText(s);
			}

			GetDocument()->SetModifiedFlag(true);
		}
		else
		{
			break;
		}

		*Cancel = FALSE;
		return;

	} while (0);

    *Cancel = TRUE;  // Default setting
}

// =======================================================================
// Function name  : Mygridn_MouseDown
// Description    : popupmenu
// Return type    : void
// Parameters     : X,Y: Client, Twips which need to convert to pixels
// Author         : WangDong 2011.01.22 13:20 
// =======================================================================
void CTagsGridView::Mygridn_MouseDown(short Button, short Shift, float X, float Y)
{
    if( Button == MK_RBUTTON )
	{
		long lRow = m_sGrid.get_MouseRow();
		long lCol = m_sGrid.get_MouseCol();
		if( lRow > 0 && (lCol >= 0))
		{
			// Check if the clicked cell is within the selected cells
			// If not select it
            if( !GridIsRowColSelected( m_sGrid, lRow, lCol ))
			{
				m_sGrid.Select( lRow, lCol, _variant_t(lRow), _variant_t(lCol));
			}

			CMenu menu;
		    BOOL bLoadMenu = menu.LoadMenuW( IDM_ALIASES );
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
					// 1.Enable
					bool bEnable = ((CMainFrame*)AfxGetMainWnd())->IsConnected();
					if( CConfiguratorApp::TS350 == theApp.GetProductType())
					{
						bEnable = bEnable && !theApp.m_bAppMismatch;
					}

					if( bEnable )
					{
					    pSubMenu->EnableMenuItem( ID_ALIASES_READ, MF_BYCOMMAND | MF_ENABLED );
					    pSubMenu->EnableMenuItem( ID_ALIASES_WRITE, MF_BYCOMMAND | MF_ENABLED );
					}
					else
					{
                        pSubMenu->EnableMenuItem( ID_ALIASES_READ, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
					    pSubMenu->EnableMenuItem( ID_ALIASES_WRITE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
					}

					// Show shortcut menu
			        pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_HORPOSANIMATION, point.x, point.y, this );
			    }
		    }
		}//if( lRow > 0 && (lCol >= 0))	
	}
}

//===============================================================================
/// Popup Menu

void CTagsGridView::OnAliasesRead()
{
	// TODO: 在此添加命令处理程序代码
	// Get setting items for current selection
    std::auto_ptr<CAliasMap> pAliases( GetSelectionSettings());
    if( pAliases->size() > 0)
    {
        // ReadAliasesFromDevice is an asynchronous function, so it
        // doesn't really return the actual result code.
        ((CMainFrame*)AfxGetMainWnd())->ReadAliasesFromDevice( pAliases.get());
    }
}


void CTagsGridView::OnAliasesWrite()
{
	// TODO: 在此添加命令处理程序代码
	// Get setting items for current selection
    std::auto_ptr<CAliasMap> pAliases( GetSelectionSettings());
    if( pAliases->size() > 0)
    {
        // WriteAliasesToDevice is an asynchronous function, so it
        // doesn't really return the actual result code.
         ((CMainFrame*)AfxGetMainWnd())->WriteAliasesToDevice( pAliases.get());
    }

    /* 延迟 500ms，在读取 CanRun Flag
    ::Sleep(500);
    MainForm->ReadStatus(); */
}

//------------------------------------------------------------------------
/// Delete
/* =======================================================================
// Function name  : Mygridn_KeyDown
// Description    : Show popup menu: 建议删除
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CTagsGridView::Mygridn_KeyDown(short* KeyCode, short Shift)
{
	// TODO: 在此处添加消息处理程序代码
	if (Key == VK_APPS)
    {
        int x, y;

        // Get cell position information
        x = m_spGrid->CellLeft + m_spGrid->CellWidth;
        y = m_spGrid->CellTop + m_spGrid->CellHeight;

        // Convert (x, y) from twips to pixels
        TPoint pt;
        PointTwipsToPixels(x, y, pt);
        pt = ClientToScreen(pt);

        // Display menu
        MyPopupMenu_Update(Sender);
        MyPopupMenu->Alignment = paRight;
        MyPopupMenu->Popup(pt.x, pt.y);
    } 
}*/
