// SettingTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "Configurator.h"
#include "SettingTreeView.h"

// Added by wangdong
#include "ConfiguratorDoc.h"
#include "MainFrm.h"

#include "SettingGridView.h"
#include "TagsGridView.h"
 
// CSettingTreeView

IMPLEMENT_DYNCREATE(CSettingTreeView, CConfiguratorView)   

CSettingTreeView::CSettingTreeView()
                 : m_nViewType( SETTING_GRID ),
				   m_dwViewHelpID( HIDR_SETTINGTREE ),
				   m_TreeCtrl(),
				   m_pPreSelData( NULL )  // for TS350
{
}

CSettingTreeView::~CSettingTreeView()
{
	m_ctrlFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CSettingTreeView, CConfiguratorView)           
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREECTRLID, TreeCtrl_OnClick)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREECTRLID, TreeCtrl_OnSelchanged)
END_MESSAGE_MAP()


// CSettingTreeView 绘图

void CSettingTreeView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CSettingTreeView 诊断

#ifdef _DEBUG
void CSettingTreeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSettingTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSettingTreeView 消息处理程序

// =======================================================================
// Function name  : OnCreate
// Description    : 1. Set HelpID: HIDR_SETTINGTREE
//                  2. Create TreeCtrl;
//                  3. Apply Font
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.13 10:40:00
// =======================================================================
int CSettingTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CConfiguratorView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	SetWindowContextHelpId( m_dwViewHelpID );

	try
	{
	    if( !m_TreeCtrl.Create( WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NOTOOLTIPS, 
		                        CRect(0, 0, 100, 100), this, IDC_TREECTRLID ))
	    { 
			CString str;
		    str.Format( L"m_TreeCtrl.Create:%s is failed!", ((m_nViewType==SETTING_GRID) ? L"Configuration View" : L"Aliases View"));
			theApp.LogInfo( str );

		    return -1;
	    }

		// Set Font
		CClientDC dc(this);
		LOGFONT lf;
		memset( &lf, 0, sizeof(LOGFONT));
		lf.lfHeight = ::MulDiv( static_cast<int>(global::FontSize),-dc.GetDeviceCaps( LOGPIXELSY ), 72 ); //-13;
		lf.lfWeight = 400;
		::StrCpyNW( lf.lfFaceName, global::FontName, 32 );
        m_ctrlFont.CreateFontIndirectW( &lf );

		ApplyFont();
	}
	catch( COleException &e )
	{
		e.ReportError();
		e.Delete();

		CString str;
		str.Format( L"Exception occured in CSettingTreeView::OnCreate - %s", ((m_nViewType==SETTING_GRID) ? L"Configuration View" : L"Aliases View"));
		theApp.LogInfo( str );

		return -1;
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CSettingTreeView::OnCreate - %s", ((m_nViewType==SETTING_GRID) ? L"Configuration View" : L"Aliases View"));
		theApp.LogInfo( str );

        return -1;
	}

	return 0;
}

// =======================================================================
// Function name  : OnSize
// Description    : If m_TreeCtrl has been created successfully, resize it 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.13 10:50:00
// =======================================================================
void CSettingTreeView::OnSize(UINT nType, int cx, int cy)
{
	CConfiguratorView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_TreeCtrl.GetSafeHwnd())
	{
		// SWP_NOZORDER: ignore the first parameter
		// SWP_NOMOVE: ignore the x, y parameters
		m_TreeCtrl.SetWindowPos( &CWnd::wndBottom, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
	}
}

// =======================================================================
// Function name  : GetViewSettings
// Description    : Get View SettingVector according to the view type 
// Return type    : CSettingVector
// Parameters     : nViewType: View Type
// Author         : WangDong 2011.01.13 19:10:00
// =======================================================================
CSettingVector* CSettingTreeView::GetViewSettings( int nViewType ) const
{
	CSettingVector* pSettings = NULL;
	switch( nViewType )
	{
        case SETTING_GRID:
		    pSettings = GetDocument()->m_aSettings;
            break;
		case TAGS_GRID:
		    pSettings = GetDocument()->m_aSettingsAlias;
            break;
		default:
		    pSettings = NULL;
	}

    return pSettings;
}

// =======================================================================
// Function name  : OnUpdate
// Description    : 1. If TreeCtrl has items:
//                     1.1 Check if access level: Manage <-> others：
//                                 true: must clear and redraw
//                                 false：check if document has changed 
//                  2. If need redraw:
//                     2.1 Delete all Tree Items
//                     2.2 Get SettingVector according to the view type
//                     2.3 Add items included in the SettingVector pointer
//                     2.4 Select the first node
//                     2.5 Expand RootItems                                    
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.13 11:00:00
// =======================================================================
void CSettingTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
	try
	{
		if(( m_TreeCtrl.GetSafeHwnd() == NULL) || (AfxGetMainWnd()->GetSafeHwnd() == NULL))
		{
			return;
		}

		if( lHint == CConfiguratorDoc::HINT_FONT )
		{
			ApplyFont();
			return;
		}

		if( GetViewSettings( m_nViewType ) != NULL )
		{
			if( GetViewSettings( m_nViewType )->size() == 0 )
			{
				m_TreeCtrl.DeleteAllItems();
				m_pPreSelData = NULL;
				return;
			}
		}
		else
		{
			m_TreeCtrl.DeleteAllItems();
			m_pPreSelData = NULL;
			return;
		}

		/* Log info if not monitor
		if( !(((CMainFrame*)AfxGetMainWnd())->IsConnected())    &&
			 (lHint == CConfiguratorDoc::HINT_REFRESH)          && 
		     (theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL))
		{
			theApp.LogInfo( L"CSettingTreeView::OnUpdate - Refresh." );
		}*/

		bool bRedraw = false;

		// 1. If TreeCtrl has items 
		if( m_TreeCtrl.GetCount() > 0 )
		{
			// 1.1 Check if access level: Manage <-> others：
			//                            true: must clear and redraw
			//                            false：check if document has changed
			if( theApp.GetResetViewFlag() || theApp.IsAccessRedraw())           
			{
				ApplyFont();
				m_pPreSelData = NULL;
				bRedraw = true;
			}
			else  //check if document has changed 
			{
				if( lHint != CConfiguratorDoc::HINT_REFRESH )
				{
					return;
				}

				// Refresh: TS350, Configuration View
				if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
				   (SETTING_GRID == m_nViewType))
				{  
					if( g_bCfgRefresh )
					{
						CSettingGridView* pConfigurationView = (CSettingGridView*)theApp.RightViewVector[CMainFrame::RSETTING_GRID];
						if( pConfigurationView->GetSafeHwnd() &&
							pConfigurationView->m_sGrid.GetSafeHwnd())
						{
							VARIANT v = pConfigurationView->m_sGrid.get_RowData( pConfigurationView->m_sGrid.get_Row()); // Current Row Data
							if( V_VT(&v) != VT_EMPTY )
							{
								CSettingItem *pSettingItem = reinterpret_cast<CSettingItem*>( V_I4(&v));
								if( pSettingItem && (m_pPreSelData != pSettingItem->GetParent()))
								{
									m_pPreSelData = pSettingItem->GetParent();
								}
							}

							bRedraw = true;
						}
					}//CfgRefresh

					if((bRedraw == false) && g_bReadRefresh )
					{
						bRedraw = true;
					}
				}// TS350, Configuration View
				
				if( bRedraw == false )
				{
					CSettingVector* pSettings = GetViewSettings( m_nViewType );
					if( CheckReadPtr( pSettings, sizeof( CSettingVector )))
					{
						// Get first configuration item
						CSettingVector::iterator it = pSettings->begin();
						CSettingItem* pCfgSettingItem = *it;

						// Get first treeview item
						CSettingItem* pTreeSettingItem = NULL;
						HTREEITEM hTreeItem = m_TreeCtrl.GetFirstVisibleItem();
						if( hTreeItem )
						{
						    pTreeSettingItem = (CSettingItem*)m_TreeCtrl.GetItemData( hTreeItem );
						}

						// Check if they are same
						if( CheckReadPtr(pCfgSettingItem, sizeof(CSettingItem)) &&
							CheckReadPtr(pTreeSettingItem, sizeof(CSettingItem)))
						{
							bRedraw = ( pCfgSettingItem != pTreeSettingItem );
						}
					}//if pSettings
				}//if( bRedraw == false )
			}//else  //check if document has changed 
		}//if( m_TreeCtrl.GetCount() > 0 )
		else
		{
            CSettingVector* pSettings = GetViewSettings( m_nViewType );
			if( CheckReadPtr( pSettings, sizeof( CSettingVector )))
			{
				if( pSettings->size() != 0 )
				{
                    bRedraw = true;
				}
			}
		}

		// 2. If need clear
		if( bRedraw )
		{
			// 2.1 Delete all Tree Items
			m_TreeCtrl.SelectItem( NULL );
			m_TreeCtrl.DeleteAllItems();

			// 2.2 Get SettingVector according to the view type
			CSettingVector* pSettings = GetViewSettings( m_nViewType );
			if( CheckReadPtr( pSettings, sizeof( CSettingVector )))
			{
				AddSettings( pSettings, NULL );    // 2.3 Add items included in the SettingVector pointer
			}

			// 2.4 Select the first node
			if( m_pPreSelData == NULL )
			{
				HTREEITEM hTreeItem = m_TreeCtrl.GetFirstVisibleItem();
				if( hTreeItem )
				{
					m_TreeCtrl.SelectItem( hTreeItem );
				}
			}

			// 2.5 Expand RootItems
		    HTREEITEM hRootItem = m_TreeCtrl.GetFirstVisibleItem();
		    while( hRootItem != NULL )
		    {
				m_TreeCtrl.Expand( hRootItem, TVE_EXPAND );
			    hRootItem = m_TreeCtrl.GetNextSiblingItem( hRootItem );
		    }	
		}// if( bRedraw )
	}//try
	catch( CException &e )
	{
		CString str;
		str.Format( L"Exception occured in CSettingTreeView::OnUpdate - %s; Update Type - %d.", (SETTING_GRID == m_nViewType) ? L"Configuration View" : L"Aliases View", lHint );
		theApp.LogInfo( str );

		e.ReportError();
		e.Delete();
	}
	catch(...)
	{
		CString str;
		str.Format( L"Unknown exception occured in CSettingTreeView::OnUpdate - %s; Update Type - %d.", (SETTING_GRID == m_nViewType) ? L"Configuration View" : L"Aliases View", lHint );
		theApp.LogInfo( str );
	}
}

// =======================================================================
// Function name  : AddSettings
// Description    : If the SettingItem is visible, and has Child items: 
//                  1. Add it to the TreeCtrl;
//                  2. Set Item Data
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.13 11:10:00
// =======================================================================
void CSettingTreeView::AddSettings( CSettingVector* pSettings, HTREEITEM hParent )
{
	if( CheckReadPtr( pSettings, sizeof(CSettingVector)))
	{
        CSettingVector::iterator it = pSettings->begin();
		CSettingVector::iterator eit = pSettings->end();

		while( it != eit )
		{
		    CSettingItem* pSettingItem = *it;
			if( pSettingItem == NULL )
			{
				++it;
				continue;
			}

			if( pSettingItem->GetVisible())
			{
				if( pSettingItem->GetChildVector())
				{
					// 1. Add it to the TreeCtrl
					HTREEITEM hTreeItem = m_TreeCtrl.InsertItem( pSettingItem->GetName_C(), hParent, NULL );
					if( hTreeItem != NULL )   // insert successful
					{
						// 2. Set Item Data
						m_TreeCtrl.SetItemData( hTreeItem, (DWORD_PTR)pSettingItem );
						ASSERT( m_TreeCtrl.SetItemData( hTreeItem, (DWORD_PTR)pSettingItem ));

						if( pSettingItem->GetChildVector())
						{
							AddSettings( pSettingItem->GetChildVector(), hTreeItem ); 
						}

						// TS350, Configuration View
						if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
						   (SETTING_GRID == m_nViewType)                        &&
						   (m_pPreSelData != NULL))
						{
							if( pSettingItem == m_pPreSelData )
							{
							    m_TreeCtrl.SelectItem( hTreeItem );
							}
						}
					}
				}//ChildVector
			}//visible
		    else if((theApp.GetProductType() == CConfiguratorApp::TS350)   &&
				    (theApp.GetAppType() == CConfiguratorDoc::APPTYPE_GEN) &&
					(m_nViewType == SETTING_GRID))
			{
				CString strName( pSettingItem->GetName());
				if((strName.CompareNoCase( L"Inlet Pressure" ) == 0) &&
				   (pSettingItem->GetParent() != NULL)               &&
				   (pSettingItem->GetParent()->GetName().CompareNoCase(L"Turbine Control") == 0))
				{
					ASSERT((m_TreeCtrl.GetItemText( hParent )).CompareNoCase( L"Turbine Control" ) == 0 );
					HTREEITEM hTreeItem = m_TreeCtrl.GetNextItem( hParent, TVGN_CHILD );    // first child item
					while( hTreeItem && (m_TreeCtrl.GetItemText(hTreeItem).CompareNoCase( L"Cascaded Control" ) != 0))
					{
						hTreeItem = m_TreeCtrl.GetNextSiblingItem( hTreeItem ); 
					}

					if( pSettingItem->GetChildVector())
					{
						AddSettings( pSettingItem->GetChildVector(), hTreeItem ); 
					}  
				}
			}//else if

            it++; 
	    }
	}
}

// =======================================================================
// Function name  : ApplyFont
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.13 11:10:00
// =======================================================================
void CSettingTreeView::ApplyFont( void )
{
	if( m_TreeCtrl.GetSafeHwnd())
	{
		if( global::g_Font.m_hObject )
		{
			m_TreeCtrl.SetFont( &global::g_Font );
		}
		else
		{
 			ASSERT( m_ctrlFont.m_hObject );
			if( m_ctrlFont.m_hObject )
			{
			    m_TreeCtrl.SetFont( &m_ctrlFont );
			}
		}
	}
}

// TreeCtrl 消息处理程序
// =======================================================================
// Function name  : TreeCtrl_OnClick
// Description    : 
//                  2. Set HelpId with the helpid of clicked item's data
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.14 14:30:00
// =======================================================================
void CSettingTreeView::TreeCtrl_OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hTreeItem = m_TreeCtrl.GetSelectedItem();
	if( hTreeItem )
	{
		CSettingItem* pSetting = reinterpret_cast<CSettingItem*>(m_TreeCtrl.GetItemData( hTreeItem ));
		if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
		{
			// Get Right View pointer according to the View Type
			CSettingGridView* pCfgView = NULL;
			CTagsGridView* pAliasesView = NULL;
			switch( m_nViewType )
	        {
                case SETTING_GRID:
					pCfgView = (CSettingGridView*)(theApp.RightViewVector[CMainFrame::RSETTING_GRID]);
					if( pCfgView )
					{
					    pCfgView->GotoSetting( pSetting );
					}
                break;

		        case TAGS_GRID:
					pAliasesView = (CTagsGridView*)(theApp.RightViewVector[CMainFrame::RALIASES]);
					if( pAliasesView )
					{
					    pAliasesView->GotoSetting( pSetting );
					}
                break;

				default:
				break;
	        }//switch

			// 2. Set HelpId with the helpid of clicked item's data
            DWORD dwHelpId = GetDocument()->GetSettingContextId( pSetting );
            if( dwHelpId == 0 )   // not found
            {
                SetWindowContextHelpId( m_dwViewHelpID );         // view's HelpID
            }
		    else
		    {
                SetWindowContextHelpId( dwHelpId );               // SettingItem's HelpID
		    }

			// 350, Configuration View: Set m_pPreSel
			if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
			   (SETTING_GRID == m_nViewType))
			{
				m_pPreSelData = pSetting;
			}
		}//if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
	}//if( hTreeItem )
	else
	{
		SetWindowContextHelpId( m_dwViewHelpID );
	}
	
	*pResult = 0;
}

// =======================================================================
// Function name  : TreeCtrl_OnSelchanged
// Description    : 
//                  2. Set HelpId with the helpid of seleted item's data
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.14 15:00:00
// =======================================================================
void CSettingTreeView::TreeCtrl_OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// TODO: Add your control notification handler code here
	HTREEITEM hTreeItem = m_TreeCtrl.GetSelectedItem();
	if( hTreeItem )
	{
		CSettingItem* pSetting = reinterpret_cast<CSettingItem*>(m_TreeCtrl.GetItemData( hTreeItem ));
		if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
		{
			// Get Right View pointer according to the View Type
			CSettingGridView* pCfgView = NULL;
			CTagsGridView* pAliasesView = NULL;
			switch( m_nViewType )
	        {
                case SETTING_GRID:
					pCfgView = (CSettingGridView*)(theApp.RightViewVector[CMainFrame::RSETTING_GRID]);
					if( pCfgView )
					{
					    pCfgView->GotoSetting( pSetting );
					}
                break;

		        case TAGS_GRID:
					pAliasesView = (CTagsGridView*)(theApp.RightViewVector[CMainFrame::RALIASES]);
					if( pAliasesView )
					{
					    pAliasesView->GotoSetting( pSetting );
					}
                break;

				default:
				break;
	        }//switch

			// 2. Set HelpId with the helpid of clicked item's data
            DWORD dwHelpId = GetDocument()->GetSettingContextId( pSetting );
            if( dwHelpId == 0 )   // not found
            {
                SetWindowContextHelpId( m_dwViewHelpID );         // view's HelpID
            }
		    else
		    {
                SetWindowContextHelpId( dwHelpId );               // SettingItem's HelpID
		    }

			// 350, Configuration View: Set m_pPreSel
			if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
			   (SETTING_GRID == m_nViewType))
			{
				m_pPreSelData = pSetting;
			}
		}//if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
	}//if( hTreeItem )
	else
	{
		SetWindowContextHelpId( m_dwViewHelpID );
	}
	
	*pResult = 0;
}
