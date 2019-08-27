// SettingGridView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "SettingGridView.h"

#include "MainFrm.h"
#include "ConfiguratorDoc.h"    // for class CConfiguratorDoc
#include <afxctl.h>             // declare CPictureHolder class
#include "hlp\Device_Help.h"
#include "SettingTreeView.h"
#include "TuneView.h"

// =======================================================================
// Function name  : GridCheckedSetting
// Description    : grid check status
// Return type    :
// Parameters     : 
// Author         : rjvs 2002-12-05 10:28:09 -06:00
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


// CSettingGridView

IMPLEMENT_DYNCREATE(CSettingGridView, CBaseGridView)            // CView->CBaseGridView


CSettingGridView::CSettingGridView()
                 :m_dwViewHelpID( HIDR_SETTINGGRID )
{
}

CSettingGridView::~CSettingGridView()
{
}

BEGIN_MESSAGE_MAP(CSettingGridView, CBaseGridView)            // CView->CBaseGridView
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CFG_ALIAS, &CSettingGridView::OnCfgAlias)
	ON_COMMAND(ID_CFG_CMP, &CSettingGridView::OnCfgCmp)
	ON_COMMAND(ID_CFG_READ, &CSettingGridView::OnCfgRead)
	ON_COMMAND(ID_CFG_VARIABLE, &CSettingGridView::OnCfgVariable)
	ON_COMMAND(ID_CFG_WRITE, &CSettingGridView::OnCfgWrite)
END_MESSAGE_MAP()


// =======================================================================
/// VSFLEXGRID EVENT
BEGIN_EVENTSINK_MAP(CSettingGridView, CBaseGridView)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, 18,               CSettingGridView::Mygridn_AfterUserResize, VTS_I4 VTS_I4)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, 20,               CSettingGridView::Mygridn_AfterCollapse, VTS_I4 VTS_I2)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, 21,               CSettingGridView::Mygridn_BeforeEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, 23,               CSettingGridView::Mygridn_ValidateEdit, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, 2,                CSettingGridView::Mygridn_RowColChange, VTS_NONE)
	ON_EVENT(CSettingGridView, IDC_BASEGRID, DISPID_MOUSEDOWN, CSettingGridView::Mygridn_MouseDown, VTS_I2 VTS_I2 VTS_R4 VTS_R4)
	//ON_EVENT(CSettingGridView, IDC_BASEGRID, DISPID_KEYDOWN,   CSettingGridView::Mygridn_KeyDown, VTS_PI2 VTS_I2)   // 建议删除
END_EVENTSINK_MAP()

// CSettingGridView 绘图

void CSettingGridView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CSettingGridView 诊断

#ifdef _DEBUG
void CSettingGridView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettingGridView 消息处理程序

// =======================================================================
// Function name  : OnCreate
// Description    : Create View
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
int CSettingGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CBaseGridView::OnCreate(lpCreateStruct) == -1 )
	{
		theApp.LogInfo( L"CBaseGridView::OnCreate in CSettingGridView is failed!" );
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( m_dwViewHelpID );      // Help ID

	if( !m_sGrid.GetSafeHwnd())
	{
		theApp.LogInfo( L"m_sGrid in CSettingGridView is invalid!" );
        return -1;
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
		s.LoadStringW( IDS_NAME );
		m_sGrid.put_TextMatrix( 0, COL_NAME, s );      // Name
		
		s.LoadStringW( IDS_VARIABLE );
        m_sGrid.put_TextMatrix( 0, COL_VARIABLE, s );  // Variable

		s.LoadStringW( IDS_NUMBER );
        m_sGrid.put_TextMatrix( 0, COL_NUMBER, s );    // Number

		s.LoadStringW( IDS_RANGE );
        m_sGrid.put_TextMatrix( 0, COL_RANGE, s );     // Range

		s.LoadStringW( IDS_UNITS );
        m_sGrid.put_TextMatrix( 0, COL_UNITS, s );     // Units

		s.LoadStringW( IDS_VALUE );
        m_sGrid.put_TextMatrix( 0, COL_VALUE, s );     // Value

		s.LoadStringW( IDS_CMP );
        m_sGrid.put_TextMatrix( 0, COL_COMPARE, s );   // Compare

		// Hide cols: Variable, Number and Compare
		m_sGrid.put_ColHidden( COL_VARIABLE, true ); 
		m_sGrid.put_ColHidden( COL_NUMBER, true );
		m_sGrid.put_ColHidden( COL_COMPARE, true );

		// Alignment: flexAlignLeftTop
		m_sGrid.put_ColAlignment( COL_VALUE, CMygridn::flexAlignLeftTop );
		m_sGrid.put_ColAlignment( COL_COMPARE, CMygridn::flexAlignLeftTop );

		m_sGrid.put_Cell( CMygridn::flexcpAlignment, _variant_t(0), _variant_t(COL_NAME), 
			_variant_t(0), _variant_t(COL_NAME), _variant_t(CMygridn::flexAlignLeftTop));

		// 0 row: font bold
		m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t(0), _variant_t(0), 
			              _variant_t(0), _variant_t(COL_LAST), _variant_t(true));

		// Column width
		for( int i = 0; i < global::SETTING_GRID_COLUMNS; ++i )
		{
            if( global::SettingGridColWidths[i] > 0 )
			{
				m_sGrid.put_ColWidth( i, global::SettingGridColWidths[i] );
			}
		}

		// If not visible，set visible
	    if( !m_sGrid.IsWindowVisible() )
	    {
	        m_sGrid.ShowWindow( SW_SHOW );
	    }

		// 此语句是必须的，否则不会重画在屏幕上
		m_sGrid.put_Redraw( CMygridn::flexRDDirect );
	}
	catch( COleException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CSettingGridView::OnCreate!" );

		return -1;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CSettingGridView::OnCreate!" );

        return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : OnUpdate
// Description    : Update View Contents
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CSettingGridView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
    try
	{
		if( NULL == m_sGrid.GetSafeHwnd())
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

		bool bRefresh = false;

		long lItemCount = GetDocument()->m_aSettings->GetTotalCount();
		if( lItemCount > 0 )
		{
            if( lHint == CConfiguratorDoc::HINT_REFRESH )
			{
                bRefresh = true;
			}
			else if( lItemCount + 1 != m_sGrid.get_Rows())
			{
                bRefresh = true;
			}

			if( bRefresh )
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

					// Initialize Display Grid
					CString s;
					s.LoadStringW( IDS_NAME );
					m_sGrid.put_TextMatrix( 0, COL_NAME, s );      // Name
		
					s.LoadStringW( IDS_VARIABLE );
					m_sGrid.put_TextMatrix( 0, COL_VARIABLE, s );  // Variable

					s.LoadStringW( IDS_NUMBER );
					m_sGrid.put_TextMatrix( 0, COL_NUMBER, s );    // Number

					s.LoadStringW( IDS_RANGE );
					m_sGrid.put_TextMatrix( 0, COL_RANGE, s );     // Range

					s.LoadStringW( IDS_UNITS );
					m_sGrid.put_TextMatrix( 0, COL_UNITS, s );     // Units

					s.LoadStringW( IDS_VALUE );
					m_sGrid.put_TextMatrix( 0, COL_VALUE, s );     // Value

					s.LoadStringW( IDS_CMP );
					m_sGrid.put_TextMatrix( 0, COL_COMPARE, s );   // Compare

					long lCurRow = m_sGrid.get_Row();            // Current Row
					long lTopRow = m_sGrid.get_TopRow();
					m_sGrid.put_Rows( 1L );

					// Clear all cells if product or application type was changed 
					if( theApp.GetResetViewFlag() /*|| g_bReadRefresh*/ )
					{
						lCurRow = 1;
						/* m_sGrid.put_Rows( 1L );*/

						// Column width
						for( int i = 0; i < global::SETTING_GRID_COLUMNS; ++i )
						{
							if( global::SettingGridColWidths[i] > 0 )
							{
								m_sGrid.put_ColWidth( i, global::SettingGridColWidths[i] );
							}
						}
					}

					// Start adding all of the settings
                    long lRow = 1;
					m_sGrid.put_Redraw( CMygridn::flexRDNone );

					if((theApp.GetProductType() == CConfiguratorApp::DSC100) ||
					   (theApp.GetProductType() == CConfiguratorApp::DSC110))
					{
                        // Combine string settings and update m_bDirtyRemote/Local Flag
						SetStringSettings( 40132, 40137, 40201, true );    // Local first: m_avLocal, and m_bDirtyLocal
						SetStringSettings( 40132, 40137, 40201, false );   // Remote: m_avRemote, and m_bDirtyRemote
						
						// Split m_avLocal: string --> int
						//SplitStringSettings( 40132, 40137, 40201 );

					    AddSettings( GetDocument()->m_aSettings, lRow, 0 );

						SetActuatorCurRange();
					}
					else
					{
					    AddSettings( GetDocument()->m_aSettings, lRow, 0 );
					}

					// Go to prerow before refresh
					m_sGrid.put_Row( lCurRow );
					m_sGrid.ShowCell( lCurRow, 0 );
					m_sGrid.put_TopRow( lTopRow );

					// Autosize the columns if they have not been sized yet.
                    if( global::SettingGridColWidths[0] <= 0 )
                    { 
					    m_sGrid.put_AutoSizeMode( CMygridn::flexAutoSizeColWidth );
                    }

					// Font
					if( theApp.GetResetViewFlag())
					{
						ApplyFont();
					}
                      
				    sl.Unlock();
				}
			}//if( bRefresh )
		}//if( nItemCount > 0 )
		else
		{
			m_sGrid.put_Rows(1L);
		}

		if( m_sGrid.get_Redraw() != CMygridn::flexRDDirect )
		{
			m_sGrid.put_Redraw( CMygridn::flexRDDirect );
		}
	}//try
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		CString str;
		str.Format( L"Exception occured in CSettingGridView::OnUpdate - %d!", lHint );
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CSettingGridView::OnUpdate - %d!", lHint );
		theApp.LogInfo( str );
	}
}

// =======================================================================
// Function name  : AddSettings
// Description    : Add Setting Item
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.31 15:50:00
// =======================================================================
void CSettingGridView::AddSettings( CSettingVector* pSettings, long& lRow, long lDepth )
{
	if( lDepth == 0 )
	{
		long lRows = pSettings->GetTotalCount() + 1;
        m_sGrid.put_Rows( lRows );

		// Set BackColor and ForeColor
        if( lRows > 1)
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
		CSettingVector::iterator it = pSettings->begin();
		CSettingVector::iterator eit = pSettings->end();

		while( it != eit )
		{
			CSettingItem *pSetting = *it;
			if( pSetting == NULL )
			{
				++it;
				continue;
			}

			// Set Row Data
			m_sGrid.put_RowData( lRow, _variant_t((long) pSetting)); 

			// Name Column
			m_sGrid.put_TextMatrix( lRow, COL_NAME, pSetting->GetName_C());

			// Check if the current SettingItem has Variable
			bool bVariable = (pSetting->GetAlias() != NULL);
			if( bVariable )   
			{
				const CAliasItem* pAlias = pSetting->GetAlias();

				/// Font Color
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

				// Range Column
				m_sGrid.put_TextMatrix( lRow, COL_RANGE, pSetting->GetRange_C());

				// Units
				m_sGrid.put_TextMatrix( lRow, COL_UNITS, pSetting->GetUnits_C());

				// Value and Compare
				bool bCompareFile = (theApp.GetCmpType() == CConfiguratorApp::CMP_FILE) ? true : false;
				if( pSetting->IsBoolean())
				{
					// Value Column: m_avLocal
					_variant_t var_t = GridCheckedSetting( pSetting->IsChecked());
					m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t(COL_VALUE),
						              _variant_t( lRow ), _variant_t( COL_VALUE ), var_t); 

					// Compare Column: m_avRemote or m_avCompareFile
					var_t = GridCheckedSetting( pSetting->IsCheckedCmp( bCompareFile )); 
                    m_sGrid.put_Cell( CMygridn::flexcpChecked, _variant_t( lRow ), _variant_t(COL_COMPARE),
						              _variant_t( lRow ), _variant_t( COL_COMPARE ),
									  var_t);

					// If Compare column is visible, Set its fore color
					if( !m_sGrid.get_ColHidden( COL_COMPARE ))
					{
						 if( pSetting->IsChecked() != pSetting->IsCheckedCmp( bCompareFile ))
						 {
                             m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t(0),
						              _variant_t( lRow ), _variant_t( COL_LAST ),
									  _variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
						 }
					}
				}//if( pSetting->IsBoolean())
				else
				{
					CString strLocal, strCompare;

					// Value Column
					pSetting->GetValue( strLocal );
                    m_sGrid.put_TextMatrix( lRow, COL_VALUE, (LPCTSTR)strLocal );

					// Compare Column: m_avRemote or m_avCompareFile
					pSetting->GetValueCmp( strCompare, bCompareFile );
                    m_sGrid.put_TextMatrix( lRow, COL_COMPARE, (LPCTSTR)strCompare );

					// If Compare column is visible, Set its fore color
					if( !m_sGrid.get_ColHidden( COL_COMPARE ))
					{
						 if( strLocal != strCompare )
						 {
                             m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( lRow ), _variant_t(0),
						              _variant_t( lRow ), _variant_t( COL_LAST ),
									  _variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
						 }
					}
				}//else

				// Check if user can edit alias
                if( global::Security.CanEditAlias(pAlias))
                {
                    m_sGrid.put_Cell( CMygridn::flexcpBackColor, _variant_t( lRow ), _variant_t( COL_VALUE ),
						              _variant_t( lRow ), _variant_t( COL_VALUE ), _variant_t( global::clWindow));
                }

				// Set Cell Pictures
				CPictureHolder pic;
				if( pSetting->GetChildVector())
				{
                    pic.CreateFromBitmap( IDB_FOLDEROPEN );
				}
				else if( pAlias->IsReadOnly())
				{
					pic.CreateFromBitmap( IDB_READ_ONLY );
				}
				else if( pAlias->GetProtectType() == CAliasItem::PROTECT_TUNE )
				{
					if( pAlias->IsDirtyLocal() || pAlias->IsDirtyRemote())
					{
                        pic.CreateFromBitmap( IDB_TUNING_CHANGED );
					}
					else
					{
                        pic.CreateFromBitmap( IDB_TUNING );
					}
				}
				else if( pAlias->GetProtectType() == CAliasItem::PROTECT_CONFIGURE )
				{
					if( pAlias->IsDirtyLocal() || pAlias->IsDirtyRemote())
					{
                        pic.CreateFromBitmap( IDB_CFG_CHANGED );
					}
					else
					{
                        pic.CreateFromBitmap( IDB_CFG );
					}
				}
				
				COleVariant vPic;
				V_VT(&vPic) = VT_DISPATCH;
				V_DISPATCH(&vPic) = pic.GetPictureDispatch();
				m_sGrid.put_Cell( CMygridn::flexcpPicture, _variant_t(lRow), _variant_t(COL_NAME),
					              _variant_t(lRow), _variant_t(COL_NAME), vPic);
			}//if( bVariable )
			else
			{
				m_sGrid.put_TextMatrix( lRow, COL_VARIABLE, L"");  // Variable Column
				m_sGrid.put_TextMatrix( lRow, COL_NUMBER, L"");    // Number Column
				m_sGrid.put_TextMatrix( lRow, COL_RANGE, L"");     // Range Column
				m_sGrid.put_TextMatrix( lRow, COL_UNITS, L"");     // Units
				m_sGrid.put_TextMatrix( lRow, COL_VALUE, L"");     // Value
				m_sGrid.put_TextMatrix( lRow, COL_COMPARE, L"");   // Compare

				// Set Cell Pictures
				CPictureHolder pic;
				if( pSetting->GetChildVector())
				{
                    pic.CreateFromBitmap( IDB_FOLDEROPEN );
				}

				COleVariant vPic;
				V_VT(&vPic) = VT_DISPATCH;
				V_DISPATCH(&vPic) = pic.GetPictureDispatch();
				m_sGrid.put_Cell( CMygridn::flexcpPicture, _variant_t(lRow), _variant_t(COL_NAME),
					              _variant_t(lRow), _variant_t(COL_NAME), vPic);
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
			}
			else
			{
				m_sGrid.put_Cell( CMygridn::flexcpFontBold, _variant_t( lRow ), _variant_t(0),
						          _variant_t( lRow ), _variant_t( COL_LAST ),
								  _variant_t( false ));
                ++lRow;
			}
    
			++it;
		}//while
	}//if
}

// =======================================================================
// Function name  : SetActuatorCurRange / LimitActuatorCur
// Description    : For DSC100
//                  SetActuatorCurRange: Set 42147,42149,42159 and 42107 value range
//                  LimitActuatorCur:    When change 40003( Current Range(Jumper Select)), 
//                                       Validate 42147,42149,42159 and 42107 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.19 14:30:00
// =======================================================================
void CSettingGridView::SetActuatorCurRange()
{
    CSettingItem* pSetting = NULL;
    CAliasItem* pAlias = NULL;
    int nCurSel = -1;
    float fCurrent = 0.0;
    CString strVariable;

	for( long lRow = 1; lRow< m_sGrid.get_Rows(); lRow++ )
    {
        pSetting = GetRowSetting( lRow );
		if( pSetting != NULL )
		{
			pAlias = pSetting->GetAlias();
			if( pAlias != NULL )
			{
				strVariable = pAlias->GetName();
				if( strVariable.CompareNoCase( L"nActCurSel" ) == 0)          // 40003
				{
					nCurSel = pAlias->valI2();
					ASSERT( nCurSel != -1 );
				}

				if((strVariable.CompareNoCase( L"kAO1MIN") == 0 )   ||        // 42147
				   (strVariable.CompareNoCase( L"kAO1MAX") == 0 )   ||        // 42149
				   (strVariable.CompareNoCase( L"kAONULL1") == 0 )  ||        // 42159
				   (strVariable.CompareNoCase( L"kAONULL2") == 0 )  ||        // 42163   Only v1.1
				   (strVariable.CompareNoCase( L"kAONULL12") == 0 ) ||        // 42165   Only v1.1
				   (strVariable.CompareNoCase( L"kSDCurrent") == 0))          // 42107
				{
					switch( nCurSel )
					{
						case 0:
						{
						   pSetting->SetRange( L"-10.0", L"10.0" );
						}
						break;
						case 1:
						{
						   pSetting->SetRange( L"-25.0", L"25.0" );
						}
						break;
						case 2:
						{
						   pSetting->SetRange( L"-50.0", L"50.0" );
						}
						break;
						case 3:
						{
						   pSetting->SetRange( L"-100.0", L"100.0" );
						}
						break;
						case 4:
						 {
						   pSetting->SetRange( L"-250.0", L"250.0" );
						}
						break;
						default:
						{
						   pSetting->SetRange( L"-10.0", L"10.0" );
						}
					} // switch( nCurSel )

					m_sGrid.put_TextMatrix( lRow, COL_RANGE, pSetting->GetRange());
				}//if...
			}//if( pAlias )
	    }//if( pSetting != NULL )
    } //for
}

//---------------------------------------------------------------------------
// When change 40003( Current Range(Jumper Select)), Validate 42147,42149,42159,42163(Only v1.1),42165(Only v1.1) and 42107 
void CSettingGridView::LimitActuatorCur( long lChangedRow )
{
    CSettingItem* pSetting = GetRowSetting( lChangedRow );  // 40003: Current Range
	ASSERT( pSetting );
	if( !pSetting )
	{
		return;
	}

    CAliasItem* pAlias = pSetting->GetAlias();             
	ASSERT( pSetting );
	if( !pAlias )
	{
		return;
	}

    int nCurSel = pAlias->valI2();     // Current Range Index
	ASSERT( nCurSel != -1 );

    float fCurMin = -1.0;
    float fCurMax = -1.0;
    float fCurrent = 0.0;
    CString strVariable;
    switch( nCurSel )
    {
		case 0:
		{
			fCurMin = -10.0;
			fCurMax = 10.0;
		}
		break;
		case 1:
		{
			fCurMin = -25.0;
			fCurMax = 25.0;
		}
		break;
		case 2:
		{
			fCurMin = -50.0;
			fCurMax = 50.0;
		}
		break;
		case 3:
		{
			fCurMin = -100.0;
			fCurMax = 100.0;
		}
		break;
		case 4:
		{
			fCurMin = -250.0;
			fCurMax = 250.0;
		}
		break;
		default:
		{
			fCurMin = -10.0;
			fCurMax = 10.0;
		}
    } // switch( nCurSel )

    // Check four(v1.0)/six(v1.1) parameters: 
	//v1.0: 42147(kAO1MIN), 42149(kAO1MAX), 42159(kAONULL1) and 42107(kSDCurrent)
	//v1.1: 42147(kAO1MIN), 42149(kAO1MAX), 42159(kAONULL1), 42163(kAONULL2), 42165(kAONULL12) and 42107(kSDCurrent)                                        
	for( long lRow = lChangedRow+1; lRow <= lChangedRow+6; lRow++ )
	{
		pSetting = GetRowSetting( lRow );
		pAlias = pSetting->GetAlias();
		CString strVariable = pAlias->GetName();
		if((strVariable.CompareNoCase( L"kAO1MIN") == 0 )   ||        // 42147
		   (strVariable.CompareNoCase( L"kAO1MAX") == 0 )   ||        // 42149
		   (strVariable.CompareNoCase( L"kAONULL1") == 0 )  ||        // 42159
		   (strVariable.CompareNoCase( L"kAONULL2") == 0 )  ||        // 42163   Only v1.1
		   (strVariable.CompareNoCase( L"kAONULL12") == 0 ) ||        // 42165   Only v1.1
		   (strVariable.CompareNoCase( L"kSDCurrent") == 0))          // 42107
		{
			fCurrent = pAlias->valR4();
			if( fCurrent < fCurMin )
			{
				pAlias->valR4( fCurMin );
			}
			else  if( fCurrent > fCurMax )
			{
				pAlias->valR4( fCurMax );
			}

			CString s;
			pSetting->GetValue( s );    
			m_sGrid.put_TextMatrix( lRow, COL_VALUE, s );
		}
	}
}

// =======================================================================
// Function name  : OnSize
// Description    : Resize
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.30 13:10:00
// =======================================================================
void CSettingGridView::OnSize(UINT nType, int cx, int cy)
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
// Author         : WangDong 2011.04.13 10:05:00
// =======================================================================
void CSettingGridView::RefreshColumnSizes()
{
	if( m_sGrid.GetSafeHwnd())
	{	
		for( int i = 0; i < global::SETTING_GRID_COLUMNS; ++i )
		{
			global::SettingGridColWidths[i] = m_sGrid.get_ColWidth(i);
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
// Author         : WangDong 2011.04.13 10:15:00
// =======================================================================
void CSettingGridView::Mygridn_AfterUserResize(long Row, long Col)
{
    RefreshColumnSizes();
}

// =======================================================================
// Function name  : Mygridn_AfterCollapse
// Description    : Fired after the user expands or collapse one or more rows
//                  in an outline
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CSettingGridView::Mygridn_AfterCollapse(long Row, short State)
{
	// TODO: 在此处添加消息处理程序代码
	/*
	CSettingItem *pSetting = GetRowSetting(Row);
	if (CheckReadPtr(pSetting, sizeof(CSettingItem)))
	{
		if (pSetting->Children())
		{
			IPictureDispPtr pVal;

			// Determine if node is open or closed.
			if (State == flexOutlineExpanded)
				pVal = MPictures[GRIDICON_FOLDEROPEN];
			else if (State == flexOutlineCollapsed)
				pVal = MPictures[GRIDICON_FOLDERCLOSED];

			if (pVal)
			{
				TVariant v((IDispatch*)pVal);
				OLECHECK(m_spGrid->set_Cell(flexcpPicture, Row, COL_NAME, Row, COL_NAME, v));
			}
		}
	}*/
}

// =======================================================================
// Function name  : Mygridn_BeforeEdit
// Description    : Fired before the control enters cell edit mode
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CSettingGridView::Mygridn_BeforeEdit(long Row, long Col, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	if((Row == 0) || (Col != COL_VALUE))
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

	// If click Value column, Check if alias was writeable
	if( Col == COL_VALUE )
	{
		if( pAlias->IsReadOnly())   // ReadOnly
		{
            *Cancel = true;
		    return;
		}

		// Check permissions...
        if( !global::Security.CanEditAlias( pSetting->GetAlias()))
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
// Function name  : Mygridn_ValidateEdit
// Description    : Fired before the control exits cell mode
//                  1. Check the value user entered or selected, 
//                     if it invalid, set it with the Min/Max valid value;
//                  2. Color: Set the rows color according to the dirty flag 
//                  3. ICON: Update the icon
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.10 14:20:00
// =======================================================================
void CSettingGridView::Mygridn_ValidateEdit(long Row, long Col, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	CSettingItem *pSetting = GetRowSetting( Row );
	if( CheckWritePtr( pSetting, sizeof(CSettingItem)))
	{  
		/// 1.Get the value user entered or selected 
		bool bText = false;
		CString strValue;      // value user input
        if( pSetting->GetOptionList() != NULL )
		{
			long lIndex = m_sGrid.get_ComboIndex();
			strValue = m_sGrid.get_ComboData( _variant_t( lIndex ));   // Value, such as "0"	
		}
		else if( pSetting->IsBoolean())
		{
			strValue = m_sGrid.get_EditText();         // Checked: L"1"; Unchecked: L"2"  
			int nIsChecked = _wtoi( strValue );        // Return 0: if cannot be converted to a value 
			ASSERT( nIsChecked != 0 );                
			if( nIsChecked != CMygridn::flexChecked )
			{
                strValue = L"0";
			}
		}
		else
		{
			bText = true;
            strValue = m_sGrid.get_EditText();
			strValue = strValue.Trim();                      // Trims leading and trailing characters from the string

			// Check if the value is invalid, set it with its own Min/Max value
			if( IsCheckOwnRange( pSetting ) && !pSetting->IsRangeValid( strValue ))         // Invalid
			{
				int nORType = pSetting->ValidateSettingValue( strValue );  // Set m_avLocal and m_bDirtyLocal flag

				// Inform User the value was reset to its min/max valid value
			    CString strInfo;
			    if( nORType == CSettingItem::OR_MIN )
			    {
				    strInfo.Format( IDS_MINFORMAT, strValue, pSetting->GetRangeMin());
			    }
			    else 
			    {
			        strInfo.Format( IDS_MAXFORMAT, strValue, pSetting->GetRangeMax()); 
			    }

			    AfxMessageBox( strInfo );
			}
			else                                             // Valid
			{
				pSetting->SetValue( strValue );              // Set m_avLocal and m_bDirtyLocal flag
			}

			CString s;
			pSetting->GetValue( s );

			m_sGrid.put_EditText((LPCTSTR)s );
		}//else

		if( !bText )
		{
            pSetting->SetValue( strValue );                   // Set m_avLocal and m_bDirtyLocal flag
		}

		// Special Validate Edit
		CAliasItem* pAlias = pSetting->GetAlias();
		if( pAlias && pAlias->IsDirtyLocal())
		{
			SpecialValidateEdit( Row, pSetting, pAlias );
		}

		/// 2. Color: 
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
            if( pAlias->IsDirtyLocal())
			{
			    m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( Row ), _variant_t( 0 ),
						              _variant_t( Row ), _variant_t( COL_LAST ),
						              _variant_t( global::DirtyLocalColor ));           // DirtyLocal: 紫色

				GetDocument()->SetModifiedFlag( true );
            }
			else if( pAlias->IsDirtyRemote())
			{
				m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( Row ), _variant_t( 0 ),
						              _variant_t( Row ), _variant_t( COL_LAST ),
									  _variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
			}
			else
			{
                m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( Row ), _variant_t( 0 ),
						              _variant_t( Row ), _variant_t( COL_LAST ),
									  _variant_t( global::clWindowText ));              // 黑色
			}

			// 3. Update the displayed bitmap pictures
			CPictureHolder pic;
			if( pSetting->GetChildVector())
			{
                pic.CreateFromBitmap( IDB_FOLDEROPEN );
			}
			else if( pAlias->IsReadOnly())
			{
				pic.CreateFromBitmap( IDB_READ_ONLY );
			}
			else if( pAlias->GetProtectType() == CAliasItem::PROTECT_TUNE )
			{
				if( pAlias->IsDirtyLocal() || pAlias->IsDirtyRemote())
				{
                    pic.CreateFromBitmap( IDB_TUNING_CHANGED );
				}
				else
				{
                    pic.CreateFromBitmap( IDB_TUNING );
				}
			}
			else if( pAlias->GetProtectType() == CAliasItem::PROTECT_CONFIGURE )
			{
				if( pAlias->IsDirtyLocal() || pAlias->IsDirtyRemote())
				{
                    pic.CreateFromBitmap( IDB_CFG_CHANGED );
				}
				else
				{
                    pic.CreateFromBitmap( IDB_CFG );
				}
			}
				
			COleVariant vPic;
			V_VT(&vPic) = VT_DISPATCH;
			V_DISPATCH(&vPic) = pic.GetPictureDispatch();
			m_sGrid.put_Cell( CMygridn::flexcpPicture, _variant_t(Row), _variant_t(COL_NAME),
					           _variant_t(Row), _variant_t(COL_NAME), vPic);
		}//Color

		// Compare Color
		if( !m_sGrid.get_ColHidden( COL_COMPARE ))
		{
			bool bCompareFile = (theApp.GetCmpType() == CConfiguratorApp::CMP_FILE) ? true : false;
			if( pSetting->IsBoolean())
			{
				if( pSetting->IsChecked() != pSetting->IsCheckedCmp( bCompareFile ))
				{
                    m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( Row ), _variant_t(0),
						    _variant_t( Row ), _variant_t( COL_LAST ),
							_variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
				}
			}
			else
			{
				CString strLocal, strCompare;

				// Value Column
				pSetting->GetValue( strLocal );

				// Compare Column: m_avRemote or m_avCompareFile
			    pSetting->GetValueCmp( strCompare, bCompareFile );

				if( strLocal != strCompare )
				{
                    m_sGrid.put_Cell( CMygridn::flexcpForeColor, _variant_t( Row ), _variant_t(0),
						    _variant_t( Row ), _variant_t( COL_LAST ),
							_variant_t( global::DirtyRemoteColor ));          // DirtyRemote: 粉红色
				}
			}
		}//Compare color
	}//if( CheckWritePtr( pSetting, sizeof(CSettingItem)))
}

/********************************************************/
// Special ValidateEdit
/********************************************************/
bool CSettingGridView::IsCheckOwnRange( CSettingItem* pSetting )
{
	bool bResult = true;

	switch( theApp.GetProductType())
	{
		case CConfiguratorApp::DSC100:
			bResult = DSC100IsCheckOwnRange( pSetting );
		break;
		case CConfiguratorApp::DSC110:
			bResult = DSC110IsCheckOwnRange( pSetting );
		break;
		case CConfiguratorApp::TS350:
			bResult = TS350IsCheckOwnRange( pSetting );
        break;
		default:
		break;
	}

	return bResult;
}

// DSC100
bool CSettingGridView::DSC100IsCheckOwnRange( CSettingItem* pSetting )
{
	bool bResult = true;

	// Check with 42147(kAO1MIN) and 42149(kAO1MAX)
	if((pSetting->GetVariableName().CompareNoCase( L"kAONULL1" ) == 0) ||  // 42159
	   (pSetting->GetVariableName().CompareNoCase( L"kAONULL2" ) == 0) ||  // 42163
	   (pSetting->GetVariableName().CompareNoCase( L"kAONULL12" ) == 0)||  // 42165
	   (pSetting->GetVariableName().CompareNoCase( L"kSDCurrent" ) == 0))  // 42107
	{
		bResult = false;
	}

	return bResult;
}

// TS350
bool CSettingGridView::TS350IsCheckOwnRange( CSettingItem* pSetting )
{
	return true;
}

// DSC110
bool CSettingGridView::DSC110IsCheckOwnRange( CSettingItem* pSetting )
{
	bool bResult = true;

	if((pSetting->GetVariableName().CompareNoCase( L"kMinAngle" ) == 0) ||  // 42085
	   (pSetting->GetVariableName().CompareNoCase( L"kMaxAngle" ) == 0) ||  // 42087
	   (pSetting->GetVariableName().CompareNoCase( L"kPV01ZOF" ) == 0)  ||  // 42089
	   (pSetting->GetVariableName().CompareNoCase( L"kPV02ZOF" ) == 0)  ||  // 42091
	   (pSetting->GetVariableName().CompareNoCase( L"kSDDA" ) == 0))      
	{
		bResult = false;
	}

	return bResult; 
}



/********************************************************/
// Special ValidateEdit
/********************************************************/
void CSettingGridView::SpecialValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias )
{
	switch( theApp.GetProductType())
	{
		case CConfiguratorApp::DSC100:
			DSC100ValidateEdit( Row, pSetting, pAlias );
		break;
		case CConfiguratorApp::DSC110:
			DSC110ValidateEdit( Row, pSetting, pAlias );
		break;
		case CConfiguratorApp::TS350:
			TS350ValidateEdit( Row, pSetting, pAlias );
        break;
		default:
		break;
	}
}

/********************************************************/
// DSC110 ValidateEdit
/********************************************************/
void CSettingGridView::DSC110ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias )
{
	ASSERT((pSetting != NULL) && (pAlias != NULL) && pAlias->IsDirtyLocal());

	if( pSetting->GetVariableName().CompareNoCase( L"strMAC" ) == 0 )   // 40201: MAC, Split MAC Address
	{
		SplitStringSettings( 40132, 40137, 40201 );                     // string --> int
	}
	else if((pSetting->GetVariableName().CompareNoCase( L"kMinAngle" ) == 0) ||  // 最小控制工程角位置
		    (pSetting->GetVariableName().CompareNoCase( L"kMaxAngle" ) == 0) ||  // 最大控制工程角位置
			(pSetting->GetVariableName().CompareNoCase( L"kPV01ZOF" ) == 0)  ||  // PV01 0度工程角偏移
			(pSetting->GetVariableName().CompareNoCase( L"kPV02ZOF" ) == 0))     // PV02 0度工程角偏移
	{
		CAliasItem* pRatio = GetDocument()->m_aAliases->find_name( L"kAngRatio" );
		if( pRatio != NULL )
		{
			float fPos = pAlias->valR4();
			float fRatio =  pRatio->valR4();
			if( fabs(fRatio) > EPSILON_FLT )
			{
				float fMinValid = V_R4(&(pSetting->GetRangeMin_V())) / fRatio;
				float fMaxValid = V_R4(&(pSetting->GetRangeMax_V())) / fRatio;
				
				if( fPos < fMinValid )
				{
					fPos = fMinValid;
				}
				else if( fPos > fMaxValid )
				{
					fPos = fMaxValid;
				}

				pAlias->valR4( fPos );

				CString s;
				pSetting->GetValue( s );

				m_sGrid.put_EditText( s );
			}
		}//if( pRatio != NULL )
	}
	else if( pSetting->GetVariableName().CompareNoCase( L"kSDDA" ) == 0 )
	{
		CAliasItem *pMinAlias = GetDocument()->m_aAliases->find_name( L"kMinDA" );
		CAliasItem *pMaxAlias = GetDocument()->m_aAliases->find_name( L"kMaxDA" );
		if((pMinAlias!=NULL) && (pMaxAlias!=NULL))
		{
			float value = pAlias->valR4();
			float fMinValid = pMinAlias->valR4();
			float fMaxValid = pMaxAlias->valR4();
			if( value < fMinValid )
			{
				value = fMinValid;
			}
			else if( value > fMaxValid )
			{
				value = fMaxValid;
			}

			pAlias->valR4( value );

			CString s;
			pSetting->GetValue( s );

			m_sGrid.put_EditText( s );
		}
	}
}

/********************************************************/
// TS350 ValidateEdit
/********************************************************/
void CSettingGridView::TS350ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias )
{
	ASSERT((pSetting != NULL) && (pAlias != NULL) && pAlias->IsDirtyLocal());
	
	GetDocument()->SetVisible();

	// Force refresh
	CString str( pSetting->GetVariableName());
	if((str.CompareNoCase( L"kCTRLTYP")==0 ) ||
		(str.CompareNoCase( L"kCONTROL")==0 ) ||
		(str.CompareNoCase( L"kIPRLIM")==0 )  ||
		(str.CompareNoCase( L"kBPRLIM")==0 )  ||
		(str.CompareNoCase( L"kLIMTYPE")==0 ) ||
		(str.CompareNoCase( L"kLM1_HL")==0 )  ||
		(str.CompareNoCase( L"kDRTYPE")==0 ))
	{
		g_bCfgRefresh = true; //pAlias->IsDirtyLocal() ? true : false;
	}

	if( g_bCfgRefresh )
	{
		if( GetDocument())
		{
			GetDocument()->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH, NULL );
		}

		g_bCfgRefresh = false;
	}//if( g_bCfgRefresh )
	else
	{
		OnUpdate( NULL, CConfiguratorDoc::HINT_REFRESH, NULL );
	}
}


/********************************************************/
// DSC100 ValidateEdit
/********************************************************/
void CSettingGridView::DSC100ValidateEdit( long Row, CSettingItem* pSetting, CAliasItem* pAlias )
{
	ASSERT((pSetting != NULL) && (pAlias != NULL) && pAlias->IsDirtyLocal());

	if( pSetting->GetVariableName().CompareNoCase( L"nActCurSel" ) == 0 )    // 40003: Current Range(Jumper Select)
	{
		SetActuatorCurRange();
		LimitActuatorCur( Row );
	}
	else if( pSetting->GetVariableName().CompareNoCase( L"strMAC" ) == 0 )   // 40201: MAC, Split MAC Address
	{
		SplitStringSettings( 40132, 40137, 40201 );                          // string --> int
	}
	else if( pSetting->GetVariableName().CompareNoCase( L"nPVSel" ) == 0 )   // 40251: PV Select Algorithm
	{
		// Set 40360: PV Open/Over Range Shutdown Select
		CAliasItem* pFltAct = GetDocument()->m_aAliases->find_name( L"nPVORFault" );  
		if( CheckWritePtr(pFltAct, sizeof(CAliasItem)))
		{
			if( pAlias->valI2() == 1 )       // 40251: AIO1 Only
			{
				if( pFltAct->valI2() > 1 )
				{
					pFltAct->valI2( 0 );     // No Action
				}
			}
			else if( pAlias->valI2() == 2 )  // 40251: AIO2 Only
			{
				if( pFltAct->valI2() == 1 || pFltAct->valI2() == 3)
				{
					pFltAct->valI2(0);       // No Action
				}
			}

			CString s;
			GetRowSetting( Row+2 )->GetValue( s );
			m_sGrid.put_TextMatrix( Row + 2, COL_VALUE, s );
		}//if
	}//else if
	else
	{
		if((pSetting->GetVariableName().CompareNoCase( L"kAONULL1" ) == 0) ||  // 42159
			(pSetting->GetVariableName().CompareNoCase( L"kAONULL2" ) == 0) ||  // 42163
			(pSetting->GetVariableName().CompareNoCase( L"kAONULL12" ) == 0)||  // 42165
			(pSetting->GetVariableName().CompareNoCase( L"kSDCurrent" ) == 0))  // 42107
		{
			// NULL Current(42159): [Minimum Current(42147), Maximum Current(42149)]
			CAliasItem* pAO1MinAlias = GetDocument()->m_aAliases->find_name( L"kAO1MIN" );
			CAliasItem* pAO1MaxAlias = GetDocument()->m_aAliases->find_name( L"kAO1MAX" );
			ASSERT( pAO1MinAlias && pAO1MaxAlias );
			if( !(pAO1MinAlias && pAO1MaxAlias))
			{
				return;
			}

			double dNullCurrent = pAlias->valR4();
			double dMinCurrent  = pAO1MinAlias->valR4();
			double dMaxCurrent  = pAO1MaxAlias->valR4();

			if( dMinCurrent > dMaxCurrent )
			{
				double dCurrent = dMinCurrent;
				dMinCurrent = dMaxCurrent;
				dMaxCurrent = dCurrent;
			}

			if( dNullCurrent < dMinCurrent )   
			{
				dNullCurrent = dMinCurrent;            // dMinCurrent + 0.01;
				pAlias->valR4( static_cast<float>(dNullCurrent));
			}
			else if( dNullCurrent > dMaxCurrent )
			{
				dNullCurrent = dMaxCurrent;            // dMaxCurrent - 0.01;             
				pAlias->valR4( static_cast<float>(dNullCurrent));
			}

			CString s;
			pSetting->GetValue( s );

			m_sGrid.put_EditText( s );
		}
	}//else
}

// =======================================================================
// Function name  : Mygridn_RowColChange
// Description    : Fired when the current cell( Row, Col ) Changes to a different cell
//                  1. AutoSearch ??
//                  2. HelpID: View / RowSettingItem's HelpID
//                  3. OptionList: If the column user clicked is Value column
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CSettingGridView::Mygridn_RowColChange()
{
	// TODO: 在此处添加消息处理程序代码
    if( m_sGrid.get_Rows() == 1L )          // No SettingItem
	{
		return;
	}

	// Get current Row and Column
	long lRow = m_sGrid.get_Row();
	long lCol = m_sGrid.get_Col();

	// Set the autosearch if on a non-editable column
	m_sGrid.put_AutoSearch( (lCol == COL_VALUE) ? 
		                    CMygridn::flexSearchNone : CMygridn::flexSearchFromCursor);      // ??

	// Set HelpID and OptionList
    CSettingItem *pSetting = GetRowSetting( lRow );           // Get the row's CSettingItem
    if( CheckReadPtr(pSetting, sizeof(CSettingItem)))
    {
		// Set HelpId with the SettingItem in the current row
        DWORD dwHelpId = GetDocument()->GetSettingContextId( pSetting );
        if( dwHelpId == 0 )   // not found
        {
            SetWindowContextHelpId( m_dwViewHelpID );             // view's HelpID
        }
		else
		{
            SetWindowContextHelpId( dwHelpId );               // SettingItem's HelpID
		}

        // Set option list for the setting if the current column is Value column.
		COptionList* pOptList = NULL;
		bool bOptionList = false;
		if( lCol == COL_VALUE )
		{
			pOptList = pSetting->GetOptionList();
            bOptionList = (pOptList != NULL);
		}

		if( bOptionList )
		{
			CString strOption;
			bool bNormalLoop = true;

			COptionList::const_iterator it = pOptList->begin();  
			COptionList::const_iterator eit = pOptList->end();

			if( theApp.GetProductType() == CConfiguratorApp::DSC100 )
			{
			    if( pSetting->GetVariableName().CompareNoCase(L"nPVORFault") == 0 )         // 40360: PV Fault Action
				{
					CAliasItem* pAlias = GetDocument()->m_aAliases->find_name(L"nPVSel");   // 40251: PV Select Algorithm
					if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
					{
						ASSERT( CAliasItem::ALIAS_INTEGER == pAlias->GetDataType());
						int nPVSel = pAlias->valI2(); 
						if( nPVSel == 1 )                      // AI01 Only
						{
							eit = pOptList->upper_bound( 1 );
						}
						else if( nPVSel == 2 )                 // AI02 Only
						{
							bNormalLoop = false;
						}
					}
				}
			}//if DSC100

			while( it != eit )
			{
				COptionItem* pOptItem = it->second;
				LPCTSTR pcFormat = (it == pOptList->begin())? L"#%d;%s\t%d" : L"|#%d;%s\t%d";
				strOption.AppendFormat( pcFormat, pOptItem->GetValue(), pOptItem->GetName(), pOptItem->GetValue());

				if( bNormalLoop )
				{
					if((CConfiguratorApp::TS350 == theApp.GetProductType())    &&        // TS350 
					   (CConfiguratorDoc::APPTYPE_MECH == theApp.GetAppType()) &&        // Mech
					   ((pOptList->GetName().CompareNoCase(L"Ain Assignment") == 0) ||   // Ain Assignment
						(pOptList->GetName().CompareNoCase(L"Dout Assignment") == 0)))   // Dout Assignment
					{
						if( pOptList->GetName().CompareNoCase(L"Ain Assignment") == 0 )  // Ain Assignment
						{
							if( 1 == pOptItem->GetValue())
							{
								it = pOptList->find( 14 );
							}
							else if( 14 == pOptItem->GetValue())
							{
								it = pOptList->find( 3 );
							}
							else if( 5 == pOptItem->GetValue())
							{
								it = pOptList->find( 15 );
							}
							else if( 17 == pOptItem->GetValue())
							{
								it = pOptList->find( 9 );
							}
							else if( 9 == pOptItem->GetValue())
							{
								it = pOptList->find( 18 );
							}
							else if( 18 == pOptItem->GetValue())
							{
								it = pOptList->find( 11 );
							}
							else if( 13 == pOptItem->GetValue())
							{
								it = pOptList->find( 19 );
							}
							else
							{
								++it;
							}
						}// Ain
						else if( pOptList->GetName().CompareNoCase(L"Dout Assignment") == 0 )  // Dout Assignment
						{
							if( 17 == pOptItem->GetValue())
							{
								it = pOptList->find( 31 );
							}
							else if( 31 == pOptItem->GetValue())
							{
								it = pOptList->find( 26 );
							}
							else if( 30 == pOptItem->GetValue())
							{
								it = eit;
							}
							else
							{
								++it;
							}
						}// Dout
					}//TS350, Mech: Ain or Dout
					else
					{
					    ++it;
					}
				}// bNormalLoop
				else
				{
					++it;
					++it;
				}
			}// while

			m_sGrid.put_ComboList( strOption );
		}// if( bOptionList )
		else
		{
            m_sGrid.put_ComboList( L"" );
		}
	}//if( CheckReadPtr(pSetting, sizeof(CSettingItem)))
}

// =======================================================================
// Function name  : Mygridn_MouseDown
// Description    : Show Popupmenu
// Return type    : void
// Parameters     : X,Y: Client, Twips which need to convert to pixels
// Author         : WangDong 2011.01.22 13:00 
// =======================================================================
void CSettingGridView::Mygridn_MouseDown(short Button, short Shift, float X, float Y)
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
		    BOOL bLoadMenu = menu.LoadMenuW( IDM_CFG );
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
					    pSubMenu->EnableMenuItem( ID_CFG_READ, MF_BYCOMMAND | MF_ENABLED );
					    pSubMenu->EnableMenuItem( ID_CFG_WRITE, MF_BYCOMMAND | MF_ENABLED );
					}
					else
					{
                        pSubMenu->EnableMenuItem( ID_CFG_READ, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
					    pSubMenu->EnableMenuItem( ID_CFG_WRITE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
					}

					// 2.Visible
					BOOL bHidden = m_sGrid.get_ColHidden( COL_VARIABLE );
					if( bHidden )
					{
					    pSubMenu->CheckMenuItem( ID_CFG_VARIABLE, MF_BYCOMMAND | MF_UNCHECKED );
					}
					else
					{
						pSubMenu->CheckMenuItem( ID_CFG_VARIABLE, MF_BYCOMMAND | MF_CHECKED );
					}

					bHidden = m_sGrid.get_ColHidden( COL_NUMBER );
					if( bHidden )
					{
					    pSubMenu->CheckMenuItem( ID_CFG_ALIAS, MF_BYCOMMAND | MF_UNCHECKED );
					}
					else
					{
						pSubMenu->CheckMenuItem( ID_CFG_ALIAS, MF_BYCOMMAND | MF_CHECKED );
					}

					bHidden = m_sGrid.get_ColHidden( COL_COMPARE );
					if( bHidden )
					{
					    pSubMenu->CheckMenuItem( ID_CFG_CMP, MF_BYCOMMAND | MF_UNCHECKED );
					}
					else
					{
						pSubMenu->CheckMenuItem( ID_CFG_CMP, MF_BYCOMMAND | MF_CHECKED );
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
// =======================================================================
// Function name  : OnCfgRead / OnCfgWrite / OnCfgVariable / OnCfgAlias / OnCfgCmp
// Description    : Click Popup Menu
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.22 15:50 
// =======================================================================
void CSettingGridView::OnCfgRead()
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

void CSettingGridView::OnCfgWrite()
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

    // 延迟 500ms，在读取 CanRun Flag
    //::Sleep(500);
    //MainForm->ReadStatus();
}


void CSettingGridView::OnCfgVariable()
{
	// TODO: 在此添加命令处理程序代码
	bool bHidden = (m_sGrid.get_ColHidden( COL_VARIABLE ) != 0);
	m_sGrid.put_ColHidden( COL_VARIABLE, !bHidden ); 
		
}

void CSettingGridView::OnCfgAlias()
{
	// TODO: 在此添加命令处理程序代码
	bool bHidden = (m_sGrid.get_ColHidden( COL_NUMBER ) != 0);
	m_sGrid.put_ColHidden( COL_NUMBER, !bHidden );		
}


void CSettingGridView::OnCfgCmp()
{
	// TODO: 在此添加命令处理程序代码
	bool bHidden = (m_sGrid.get_ColHidden( COL_COMPARE ) != 0);
	m_sGrid.put_ColHidden( COL_COMPARE, !bHidden );
}

void CSettingGridView::ShowCompareColumn( bool bShow )
{
	if( m_sGrid.GetSafeHwnd())
	{
        m_sGrid.put_ColHidden( COL_COMPARE, !bShow );
	}
}

bool CSettingGridView::IsCompareColVisible()
{
	ASSERT( m_sGrid.GetSafeHwnd());
	return ( m_sGrid.get_ColHidden( COL_COMPARE ) == FALSE );
}

//------------------------------------------------------------------------
///Delete
/* =======================================================================
// Function name  : Mygridn_KeyDown
// Description    : Copy, Paste and show popupmenu, 建议删除，暂时保留此代码段
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.29 15:40:00
// =======================================================================
void CSettingGridView::Mygridn_KeyDown(short* KeyCode, short Shift)
{
	// TODO: 在此处添加消息处理程序代码
	bool bCopy = false,
         bPaste = false;

    if (Key == 'c')
        bCopy = Shift.Contains(ssCtrl);
    if (Key == 'v')
        bPaste = Shift.Contains(ssCtrl);

// Virtual key codes taken from <winuser.h>

    if (bCopy)
        mnuPopupCopy->Click();
    else if (bPaste)
        mnuPopupPaste->Click();

    // Show popup menu
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
// =======================================================================

