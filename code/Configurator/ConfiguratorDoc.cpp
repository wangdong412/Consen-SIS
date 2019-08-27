
// ConfiguratorDoc.cpp : implementation of the CConfiguratorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Configurator.h"
#endif

#include "ConfiguratorDoc.h"

/* Added by wangdong begin */
#include <fstream>
#include "AliasFileValue.h"
#include "SelAppTypeDlg.h"
#include "PasswordDlg.h"

#include "hlp\Device_Help.h"
#include "MainFrm.h"
#include "SettingTreeView.h"
#include "TuneView.h"
#include "TuneFraView.h"
#include "NonTuningFra.h"

#include "HistoryValueView.h"
#include "HistoryTrendView.h"

#include "HistoryRecordDlg.h"
#include "HistoryTrendDlg.h"
#include "HistoryTrendDynDlg.h"

#include "HistoryServer.h"
/* Added by wangdong end */

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* Added by wangdong begin */
#ifndef DELETE_EXCEPTION
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
#endif
/* Added by wangdong end */


// CConfiguratorDoc

IMPLEMENT_DYNCREATE(CConfiguratorDoc, CDocument)

BEGIN_MESSAGE_MAP(CConfiguratorDoc, CDocument)
	ON_COMMAND(IDM_VIEW_REFRESH, &CConfiguratorDoc::OnViewRefresh)
	ON_COMMAND(IDM_VIEW_DISPLAYFONT, &CConfiguratorDoc::OnViewDisplayfont)
	ON_COMMAND(IDM_HISTORY_START, &CConfiguratorDoc::OnHistoryStart)
	ON_COMMAND(IDM_HISTORY_STOP, &CConfiguratorDoc::OnHistoryStop)
	ON_COMMAND(IDM_HISTORY_VIEW, &CConfiguratorDoc::OnHistoryView)
END_MESSAGE_MAP()


// CConfiguratorDoc construction/destruction

CConfiguratorDoc::CConfiguratorDoc()
	             : m_aProperties( new CCfgProperties())
                 , m_aOptions( new COptionListMap())              // OptionLists
                 , m_aAliases( new COwnedAliasMap())              // Variables
				 , m_aSettings( new CSettingVector())             // Settings
				 , m_aSettingsAlias( new CSettingVector())        // For Aliases view
                 , m_aMonitorValues( new CCfgMonitorValues())     // MonitorValues
				 , m_aAlarmValues( new CCfgMonitorValues())       // AlarmValues
				 , m_aActions( new modbus::CMessageActions())     // Communications
				 , m_aTraces( new CCfgTraceArray())               // Trace
				 , m_aAliasFileValue( new CAliasFileValueMap())   // File Alias Value
				 , m_aHisRecordDatas( new CHistoryRecordDatas())  // History Record
				 , m_aHisFileDatas( new CHistoryFileDatas())      // History File Datas
				 , m_aHisViewDatas( new CHistoryViewDatas())      // History View Datas
				 , m_SendDlgStatus()                              // Send Dialog Status
				 , m_bRecordModified(false)
				 , m_pHisEvent( new CEvent( TRUE, TRUE ))         // Has signal, manual reset
				 , m_pHisRecordSusEvent( new CEvent( TRUE,TRUE )) // Has signal, manual reset 
				 , m_pRecordCtrl( new CRecordHistoryCtrl())
				 , m_pViewCtrl( new CViewHistoryCtrl(m_pRecordCtrl))
				 , m_pHistoryServer( new CHistoryServer())
{
	// History Thread
	m_pHistoryThread.reset( AfxBeginThread( CHistoryServer::RecordThreadFunc, this, 0, 0, CREATE_SUSPENDED ));
}

CConfiguratorDoc::~CConfiguratorDoc()
{
	// Free Document Data Resource
	SAFE_DELETE( m_aProperties );
	SAFE_DELETE( m_aOptions );
	SAFE_DELETE( m_aAliases );
	SAFE_DELETE( m_aSettings );
	SAFE_DELETE( m_aSettingsAlias );
	SAFE_DELETE( m_aMonitorValues );
	SAFE_DELETE( m_aAlarmValues );
	SAFE_DELETE( m_aTraces );

	SAFE_DELETE( m_aHisRecordDatas );
	SAFE_DELETE( m_aHisFileDatas );
	SAFE_DELETE( m_aHisViewDatas );

	m_aActions.reset();                  
	m_aAliasFileValue.reset();

	SAFE_DELETE( m_pViewCtrl );
	SAFE_DELETE( m_pRecordCtrl );
}

bool CConfiguratorDoc::IsRecording()
{
	return (m_pHistoryServer.get() != NULL)? m_pHistoryServer->IsRecording() : false;
}

// =======================================================================
// Function name  : OnViewRefresh
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.14 19:35:00
// =======================================================================
void CConfiguratorDoc::OnViewRefresh()
{
	// TODO: 在此添加命令处理程序代码
	UpdateAllViews( NULL );
}

//-------------------------------------------------------------------------
void CConfiguratorDoc::OnViewDisplayfont()
{
	// TODO: 在此添加命令处理程序代码
	bool bDefault = true;
	LOGFONT lf;
	memset( &lf, 0, sizeof(LOGFONT));

	if( global::g_Font.GetSafeHandle())
	{
		bDefault = global::g_Font.GetLogFont( &lf ) ? false : true; 
	}
	
	if( bDefault )
	{
		CClientDC dc(AfxGetMainWnd());
		
		lf.lfHeight = ::MulDiv( static_cast<int>(global::FontSize), -dc.GetDeviceCaps(LOGPIXELSY), 72 ); 
		lf.lfWeight = 400;
		::StrCpyNW( lf.lfFaceName, global::FontName, 32 );
	}

	std::auto_ptr<CFontDialog> dlg ( new CFontDialog( &lf ));
	if( dlg->DoModal() == IDOK )
	{
		if( global::g_Font.GetSafeHandle())
		{
			global::g_Font.DeleteObject();
		}

		// Tree Control Font
		global::g_Font.CreateFontIndirectW( dlg->m_cf.lpLogFont );

		// Grid Font
		global::FontName = dlg->GetFaceName();
		global::FontSize = static_cast<float>(dlg->GetSize()/10);

		UpdateAllViews( NULL, HINT_FONT, NULL );
		UpdateAllViews( NULL );                       // Refresh
	}
}

// =======================================================================
// Function name  : IsInMonitorRange
// Description    : Check if nAlias is in monitor range
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.05.31 16:00:00
// =======================================================================
bool CConfiguratorDoc::IsInMonitorRange( int nAlias ) const
{
	ASSERT( nAlias != -1 );
	bool bIn = false;
	modbus::CMessageRanges *pMonitorRanges = m_aActions->find_name( L"Monitor" );
	if( pMonitorRanges != NULL )
	{
		int begin = 0, end = 0;
		modbus::CMessageRanges::const_iterator it = pMonitorRanges->begin();
		modbus::CMessageRanges::const_iterator eit = pMonitorRanges->end();
		while( it != eit )
		{
			modbus::CMessageRange msgRange = *it;
			begin = msgRange.GetBeginAlias();
			end = msgRange.GetEndAlias();
			if((nAlias>=begin) && (nAlias<=end))
			{
				bIn = true;
				break;
			}

			++it;
		}//while
	}

	return bIn;
}

// =======================================================================
// Function name  : GetHisRecordNonMonitorAliases
// Description    : Start Recording
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.05.31 16:00:00
// =======================================================================
CAliasMap* CConfiguratorDoc::GetHisRecordNonMonitorAliases()
{
	std::auto_ptr<CAliasMap> pAliases( new CAliasMap());

	HisDatas pHisDatas = m_aHisRecordDatas->GetHisRecordDatas();
	HisDatas::const_iterator it = pHisDatas.begin();
	CHistoryRecordData *pData = NULL;
	while( it != pHisDatas.end())
	{
		pData = *it;
		if( pData != NULL )
		{
			int nAlias = pData->GetAlias();
			if( nAlias != -1 )
			{
				if( !IsInMonitorRange( nAlias ))
				{
					pAliases->Add( pData->GetAliasItem());
				}
			}
		}

		++it;
	}//while


	if( 0 == pAliases->size())
	{
		pAliases.reset();
	}

	return pAliases.release();
}

// =======================================================================
// Function name  : OnHistoryStart
// Description    : Start Recording
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.10 15:00:00
// =======================================================================
void CConfiguratorDoc::OnHistoryStart()
{
	m_aHisRecordDatas->Load( m_aAliases );
	if( m_aHisRecordDatas->IsEmpty())
	{
		CString str;
		str.LoadStringW( IDS_HISREC_NOITEM );
		AfxMessageBox( str );
		return;
	}

	std::auto_ptr<CHistoryRecordDlg> dlg( new CHistoryRecordDlg(this, m_pRecordCtrl));
	if( IDOK == dlg->DoModal())
	{
		if( CreateHistoryFile( m_pRecordCtrl->GetFilePathName()))  // Create History Record File
		{
            ((CMainFrame*)theApp.m_pMainWnd)->DoMonitor( true );   // Start Monitoring
			m_pHistoryServer->m_dwStartTime = GetTickCount();  // ms
			m_pHistoryServer->m_bSuspend = false;
			m_pHisRecordSusEvent->ResetEvent();
			m_pHistoryServer->SetRecording( true );
			m_pHistoryThread->ResumeThread();                  // Resume History Record Thread
		}
		else
		{
			CString str;
			str.LoadStringW( IDS_HISFILE_CREFAIL );
			AfxMessageBox( str );
		}
	}
	else
	{
		m_aHisRecordDatas->Clear();
	}
}

// =======================================================================
// Function name  : OnHistoryStop
// Description    : Stop History Record Thread
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.10 15:00:00
// =======================================================================
void CConfiguratorDoc::OnHistoryStop()
{
	m_pHistoryServer->m_bSuspend = true;  // Suspend Recording Thread
	if( CViewHistoryCtrl::VIEW_CURRENT == m_pViewCtrl->GetViewType())
	{
		// Wait Record Thread Suspended
		DWORD dwRes = ::WaitForSingleObject( m_pHisRecordSusEvent->m_hObject, 10000 );
		if( WAIT_OBJECT_0 == dwRes )
		{
			theApp.LogInfo( L"OnHistoryStop(View Current): Record Thread Suspended!" );
		}

		// Stop Timer Update
		StopHisTrendViewTimerUpdate();    

		// Update Ctrls
		m_pViewCtrl->SetViewType( CViewHistoryCtrl::VIEW_FILE );
		m_pViewCtrl->SetFilePathName( m_pRecordCtrl->GetFilePathName());
		m_pRecordCtrl->SetFilePathName( L"" );
	}

	((CMainFrame*)theApp.m_pMainWnd)->DoStopMonitor();
}

// =======================================================================
// Function name  : OnHistoryView
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.10 15:00:00
// =======================================================================
void CConfiguratorDoc::OnHistoryView()
{
	std::auto_ptr<CHistoryTrendDlg> dlg( new CHistoryTrendDlg(this, m_pViewCtrl));
	if( IDOK == dlg->DoModal())
	{
		UpdateHistoryView( dlg->GetUpdateType());  // Update History View
	}
}

// =======================================================================
// Function name  : DynViewHistory
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.24 15:50:00
// =======================================================================
void CConfiguratorDoc::DynViewHistory( double dStartTime )
{
	std::auto_ptr<CHistoryTrendDynDlg> dlg( new CHistoryTrendDynDlg( this, m_pViewCtrl, dStartTime ));
	if( IDOK == dlg->DoModal())
	{
		UpdateHistoryView( HIS_DYNUPDATE, dlg->GetStartTime() );  // Update History View
	}
}

// =======================================================================
// Function name  : StopHisTrendViewTimerUpdate
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.05.27 15:00:00
// =======================================================================
void CConfiguratorDoc::StopHisTrendViewTimerUpdate()
{
	POSITION pos = GetFirstViewPosition();
	while( pos != NULL )
	{
		CView *pView = GetNextView( pos );
		if( dynamic_cast<CHistoryTrendView*>(pView))
		{
			CHistoryTrendView *pTView = dynamic_cast<CHistoryTrendView*>(pView);
			pTView->KillHisTimer();
			ASSERT( WAIT_OBJECT_0 == pTView->WaitTimerUpdateFinish());
			break;
		}
	}
}

// =======================================================================
// Function name  : UpdateHistoryView
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.04.10 15:00:00
// =======================================================================
void CConfiguratorDoc::UpdateHistoryView( LPARAM lHint, double dStartTime )
{
	bool bValue = false, bTrend = false;
	POSITION pos = GetFirstViewPosition();
	while( pos != NULL )
	{
		CView *pView = GetNextView( pos );

		if( dynamic_cast<CHistoryValueView*>(pView))
		{
			dynamic_cast<CHistoryValueView*>(pView)->OnUpdate( NULL, lHint, NULL );
			bValue = true;
		}
		else if( dynamic_cast<CHistoryTrendView*>(pView))
		{
			CHistoryTrendView *pTView = dynamic_cast<CHistoryTrendView*>(pView);
			if( fabs( dStartTime - (-1.0)) > FLT_EPSILON )
			{
				pTView->SetAxisTime( dStartTime );
			}

			pTView->OnUpdate( NULL, lHint, NULL );
			bTrend = true;
		}

		if( bValue && bTrend )
		{
			break;
		}
	}
}

/*************************************************************************************/
///<Summary>
/// 打开文档：
/// 1. 清空文档对象的所有容器成员
/// 2. 重新解析xml文件                      
/// 3. 刷新所有视图：经试验，应用程序启动 和 用户点击File New 菜单项时，MFC均会自动刷新当前 
///                 文档视图列表中的所有视图 
///<Parameters>
///<Created>  wangdong 2010.12.01
/*************************************************************************************/
BOOL CConfiguratorDoc::OnNewDocument()
{
	// TS350: Select Application Type
	if((CConfiguratorApp::TS350 == theApp.GetProductType()) &&
	   (!theApp.GetMenuSelFlag()))
	{
        std::auto_ptr<CSelAppTypeDlg> dlg ( new CSelAppTypeDlg());
		if( dlg->DoModal() == IDOK )
		{
			int nAppType = -1;
			switch( dlg->m_nIndex )   
			{
				case 0:   // Gen
					nAppType = APPTYPE_GEN;
				break;
			    case 1:   // Mech
					nAppType = APPTYPE_MECH;
				break;
				default:
					nAppType = APPTYPE_GEN;
			}

			// Set Application Type
            theApp.SetAppType( nAppType );
		}// IDOK
		else
		{
			theApp.SetAppType( APPTYPE_GEN );
		}
	}// TS350

	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	// Will call DeleteContents() to ensure that a document is empty before it is reused
	if( !DoFileNew())
	{
		DeleteContents();    // Added to delete document elements
		return FALSE;
	}

	// Set Visible
	SetVisible();

	// TS350: Set Application Type, 2143-gAPPTYPE 
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		// Set Modbus 2143: Application Type, 1 - Gen, 0 - Mech
		CAliasItem *pAlias = m_aAliases->find_name( L"gAPPTYPE" );
		ASSERT( pAlias );
		if( CheckWritePtr(pAlias, sizeof(CAliasItem)))
		{
			pAlias->valBool((theApp.GetAppType() == APPTYPE_GEN) ? TRUE : FALSE);
		}
		else
		{
			theApp.LogInfo( L"Can't find Modbus 2143: Application Type in CConfiguratorDoc::OnNewDocument!" );
			return FALSE;
		}
	}
	
	// Compare File
	if((!m_strCmpFilePathName.IsEmpty()) && (theApp.GetCmpType() == CConfiguratorApp::CMP_FILE))
	{
		if( ::PathFileExists( m_strCmpFilePathName )) 
		{
		    CompareFile( m_strCmpFilePathName );
		}
		else
		{
			CString strMsg;
			strMsg.Format( IDS_CMPFILE_ERROR, m_strCmpFilePathName );
			AfxMessageBox( strMsg );

			m_strCmpFilePathName.Empty();
		}
	}//if Compare File
	
	// TS350: Update Title of MainFrame, Add Local Application Type
	if((CConfiguratorApp::TS350 == theApp.GetProductType()) &&
	   (AfxGetMainWnd()->GetSafeHwnd()))
	{
		((CMainFrame*)AfxGetMainWnd())->UpdateTitle( CMainFrame::TITLE_LOCAL );
	}

	return TRUE;
}

// ----------------------------------------------------------------------
// Function name  : DoFileNew
// Description    : 1. Parse xml file
//                  2. Bind
//                  3. Reset all Dirty Flags  
//                  4. Set Product information: Product Number, Application Type, and check it
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.03.28 16:15:00
// ----------------------------------------------------------------------
BOOL CConfiguratorDoc::DoFileNew()
{
	try
	{
		// 1. Parse xml file
		if( !ParseXMLFile())
		{
			theApp.LogInfo( L"CConfiguratorDoc::ParseXMLFile failed!" );
			return FALSE;
		}

		// 2. Check Product Number
		if( !CheckProductNumber())
		{
			theApp.LogInfo( L"CConfiguratorDoc::CheckProductNumber failed!" );
			return FALSE;
		}

		// 3. Bind 
		BindDictionary();
		BindDocument();

		// 4. Reset all Dirty Flags
		m_aAliases->SetDirtyLocal( false );
		m_aAliases->SetDirtyRemote( false );
	}
	catch( CException &e )
	{
		e.ReportError();
		if( e.IsKindOf( RUNTIME_CLASS(CMemoryException)))
		{
			theApp.LogInfo( L"CConfiguratorDoc::DoFileNew: 'new' operation caused exception." );
		}

		e.Delete();
		return FALSE;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CConfiguratorDoc::DoFileNew." );

		return FALSE;
	}

	return TRUE;
}

// CConfiguratorDoc serialization

void CConfiguratorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// =======================================================================
// Function name  : CheckProductNumber
// Description    : When New a document, Only Check Product Number, 
//                  It should be equal to CxxxApp::m_nProductNumber
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.03.25 16:15:00
//                           2011.10.25: delete iApplicationType in the xml file
// =======================================================================
bool CConfiguratorDoc::CheckProductNumber()
{
	bool bResult = false;
	if( CheckReadPtr( m_aProperties, sizeof(CCfgProperties)))
	{
		if( m_aProperties->size() > 0 )
		{
			int nProductNumber = -1;

			// Get product number stored in the xml file
			CCfgProperty* pProperty = m_aProperties->find_name( L"iProductNumber");
			if( pProperty != NULL )
			{
				nProductNumber = pProperty->GetValue();
			}

			// Check product number
			if( nProductNumber == theApp.GetProductNumber())
			{
				bResult = true;
			}
			else
			{
				CString strXMLFileName( theApp.GetXMLFileName());
			    CString strError;
				strError.Format( IDS_PRODUCTNUM_ERROR, nProductNumber, strXMLFileName, theApp.GetProductNumber()); 
				AfxMessageBox( strError );
			}
		}//if
	}//if

	return bResult;
}

// =======================================================================
// Function name  : UpdateProductInfo
// Description    : When Open File: Update Product ID and AppType
// Return type    : bool
// Parameters     : 
// Author         : WangDong 2011.03.28 14:50:00
// =======================================================================
bool CConfiguratorDoc::UpdateProductInfo( LPCTSTR lpszPathName )
{
	int ProductNumber = -1, nAppType = -1;

    CCfgProperty* p = m_aProperties->find_name( L"Product.Name" );   // ProductNumber
	ASSERT( p );
	if( !p )
	{
		return false;
	}
	ProductNumber = static_cast<int>(p->GetValue());

	p = m_aProperties->find_name( L"Dictionary.AppType" );           // Application Type
	ASSERT( p );
	if( !p )
	{
		return false;
	}
	nAppType = static_cast<int>(p->GetValue());

	// TS350: 验证nAppType, 2143-gAPPTYPE 应该与 Dictionary.AppType 一致；否则,认为文档无效，返回FALSE。
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		CAliasFileValueMap::iterator it = m_aAliasFileValue->find( L"gAPPTYPE" );   // 2143 - gAPPTYPE
		if( it != m_aAliasFileValue->end())
		{
		    CAliasFileValue FileAlias = it->second;
		    
			// 0 - Mech; 1 - Gen
			bool bValue = false;
			CString strValue = FileAlias.GetValueStr();
			if( ::isdigit( strValue[0])) 
			{
				int nValue = 0;
			    ::swscanf_s( strValue, L"%d", &nValue );
				bValue = ( nValue != 0 );
			}
			else 
			{
				bValue = (strValue.CompareNoCase(L"True") == 0 );
			}

			if( nAppType != (bValue ? APPTYPE_GEN : APPTYPE_MECH))
			{
                AfxMessageBox( IDS_FILE_ARRTYPE_ERR );
				return false;
			}
		}
	}//TS350

	theApp.SetProductInfo( ProductNumber, nAppType );
	return true;
}

// =======================================================================
// Function name  :DeleteContents
// Description    :Override CDocument::DeleteContents 
//                 Make the document empty( Clear all containers )
// Apllication    :1. It will be called before the document is to be destroyed;
//                 2. It is also called before it is reused( creates or open another document);
// Return type    : void
// Parameters     : 
// Author         : WangDong 2010.12.14 10:00:00                                                                                                                                                                                                                                                                                                                                                                                                                          
// =======================================================================
void CConfiguratorDoc::DeleteContents()
{
	// Required and Dictionary attribute
	if( CheckWritePtr( m_aProperties, sizeof( CCfgProperties )))
	{
		m_aProperties->clear();
	}

	// OptionLists
	if( CheckWritePtr( m_aOptions, sizeof( COptionListMap )))
	{
		m_aOptions->clear();
	}

	// Variables
	if( CheckWritePtr( m_aAliases, sizeof( COwnedAliasMap )))
	{
        m_aAliases->clear();
	}

	// Settings
	if( CheckWritePtr( m_aSettings, sizeof( CSettingVector )))
	{
        m_aSettings->clear();
	}

	if( CheckWritePtr( m_aSettingsAlias, sizeof( CSettingVector )))
	{
		m_aSettingsAlias->clear();
	}

	// MonitorValues
	if( CheckWritePtr( m_aMonitorValues, sizeof( CCfgMonitorValues )))
	{
        m_aMonitorValues->clear();
	}

	// AlarmValues
	if( CheckWritePtr( m_aAlarmValues, sizeof( CCfgMonitorValues )))
	{
        m_aAlarmValues->clear();
	}

	// Communications
	if( CheckWritePtr( m_aActions.get(), sizeof( modbus::CMessageActions )))
	{
		m_aActions->clear();
	}

	// Trace
	if( CheckWritePtr( m_aTraces, sizeof(CCfgTraceArray)))
	{
		m_aTraces->clear();
	}

	// File Alias
	if( CheckWritePtr( m_aAliasFileValue.get(), sizeof(CAliasFileValueMap)))
	{
		m_aAliasFileValue->clear();
	}

	// Set View Parameters NULL
	POSITION pos = GetFirstViewPosition();
	while( pos != NULL )
	{
		CView *pView = GetNextView( pos );
		if( pView->GetSafeHwnd())
		{
		    if( pView->GetWindowContextHelpId() == HIDR_SETTINGTREE )
			{
				((CSettingTreeView*)pView)->SetPreSelItem( NULL );
			}
			else if( pView->GetWindowContextHelpId() == HIDX_TUNING )
			{
				((CTuneView*)pView)->m_pTuningBase = NULL;

				TuneFrameVector::iterator it = ((CTuneView*)pView)->m_FraVector.begin();
				TuneFrameVector::iterator eit = ((CTuneView*)pView)->m_FraVector.end();
				while( it != eit )
				{
					if( (*it)->GetSafeHwnd() != NULL )
					{
						(*it)->m_pAliasPB = NULL;
						(*it)->m_pAliasRate = NULL;
						(*it)->m_pAliasReset = NULL;

						(*it)->m_pSettingPB = NULL;
						(*it)->m_pSettingRate = NULL;
						(*it)->m_pSettingReset = NULL;

						(*it)->m_pAliases->clear();
					}

					it++;
				}//while

				if(((CTuneView*)pView)->m_pNonTuningFra->GetSafeHwnd())
				{
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasCurrent1 = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasCurrent2 = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasCurrent12 = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasAmplitude = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasFrequency = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasSD = NULL;
					(((CTuneView*)pView)->m_pNonTuningFra)->m_pAliasCBS = NULL;
				}
			}//Tune
		}//if( pView->GetSafeHwnd())
	}//while	
}

// =======================================================================
// Function name  :ParseXMLFile
// Description    :Parse a xml file according to the product and language type  
// Return type    :true: If success;
//                 false: Failed
// Parameters     : 
// Author         : WangDong 2010.12.15 18:30:00
// =======================================================================
bool CConfiguratorDoc::ParseXMLFile()
{
	bool bResult = true;
	CString strError;
	
	try
	{
		while( 1 )
		{
			HRESULT hr = CoInitialize( NULL );
			if( FAILED( hr ))
			{
				strError.Format( L"ParseXMLFile-CoInitialize failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			MSXML2::IXMLDOMDocumentPtr pXMLDoc;
			hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0");
			if( FAILED(hr))
			{
				strError.Format( L"ParseXMLFile-CreateInstance failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			hr = pXMLDoc->put_async( VARIANT_FALSE );  // 异步为假，保证XML解析器暂停执行，直到XML文件加载完成
			if( FAILED(hr))
			{
				strError.Format( L"ParseXMLFile-put_async failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			// Load xml file according to the Product and Language Type
			_variant_t _varXMLFile( theApp.GetXMLFileName());
			if( pXMLDoc->load( _varXMLFile ) != VARIANT_TRUE )
			{
				strError.Format( L"Unable to Load %s!", theApp.GetXMLFileName());
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
			{
				CString strLogXMLFile( L"Opening dictionary: " );
				strLogXMLFile += _varXMLFile;
				theApp.LogInfo( strLogXMLFile );
			}

			// Root Element: DataDictionary
			MSXML2::IXMLDOMElementPtr pXMLRootElement( pXMLDoc->documentElement);  
			if( !pXMLRootElement )
			{
				strError = L"DataDictionary Element is NULL!";
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			// Get DataDictionary's attribute             
			MSXML2::IXMLDOMNodePtr pAttributeNode;   // Attribute Node
			MSXML2::IXMLDOMNamedNodeMapPtr pAttributes( pXMLRootElement->Getattributes());
			if( pAttributes )
			{
				ASSERT( m_aProperties != NULL );
				if( m_aProperties == NULL )
				{
					theApp.LogInfo( L"ParseXMLFile: m_aProperties is NULL!" );
					bResult = false;
				    break;
				}

				// Name Attribute
				pAttributeNode = pAttributes->getNamedItem( "Name" );
				if( pAttributeNode )
				{
					m_aProperties->Add( L"Dictionary.Title", pAttributeNode->Gettext());
				}

				// Desc Attribute
				pAttributeNode = pAttributes->getNamedItem( "Desc" );
				if( pAttributeNode )
				{
					m_aProperties->Add( L"Dictionary.Description", pAttributeNode->Gettext());
				}

				// Version Attribute
				pAttributeNode = pAttributes->getNamedItem( "Version" );
				if( pAttributeNode )
				{
					m_aProperties->Add( L"Dictionary.Version", pAttributeNode->Gettext());
				}
			}

			// Get Child Nodes of DataDictionary
			MSXML2::IXMLDOMNodeListPtr pXMLChildNodes;
			hr = pXMLRootElement->get_childNodes( &pXMLChildNodes );
			if( FAILED(hr))
			{
				strError.Format( L"ParseXMLFile-get_childNodes failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			// Loop DataDictionary's Child Nodes
			MSXML2::IXMLDOMNodePtr pXMLCurrentNode;          // Current Node
			long lChildCounts = pXMLChildNodes->Getlength(); // Counts
			ASSERT( lChildCounts >= 1 );
			for( long i = 0; i<lChildCounts; i++ )
			{
				pXMLCurrentNode = pXMLChildNodes->Getitem( i ); 
				if( pXMLCurrentNode )
				{
					int nNodeType = pXMLCurrentNode->nodeType;
					if( MSXML2::NODE_ELEMENT == nNodeType )  // Node Type：Element
					{
						_bstr_t _bstrName = pXMLCurrentNode->GetnodeName();
						CString strNodeName( (LPCTSTR)_bstrName );
						if( strNodeName.Compare( L"Required") == 0 )
						{
							RequiredElement( pXMLCurrentNode );
						}
						else if( strNodeName.Compare( L"OptionLists") == 0 )
						{
							OptionListsElement( pXMLCurrentNode );
						}
						else if( strNodeName.Compare( L"Variables") == 0 )
						{
							AliasElement( pXMLCurrentNode );
						}
						else if( strNodeName.Compare( L"Settings") == 0 )
						{
							SettingElement( pXMLCurrentNode );
						}
						else if( strNodeName.Compare( L"MonitorValues") == 0 )
						{
							MonitorValueElement( pXMLCurrentNode, false );
						}
						else if( strNodeName.Compare( L"AlarmValues") == 0 )
						{
							MonitorValueElement( pXMLCurrentNode, true ); 
						}
						else if( strNodeName.Compare( L"Communications") == 0 )
						{
							CommunicationElement( pXMLCurrentNode ); 
						}
					}
					else // Other node, such as explanatory note
					{
						continue;
					}
				}//if pXMLCurrentNode
			}// for

			break;
		}//while

		if( !bResult )
		{
			MessageBox( theApp.m_pMainWnd->GetSafeHwnd(), strError, L"", MB_OK|MB_ICONERROR );
		}
	}
	catch( CException &e )
	{
		e.ReportError();
		if( e.IsKindOf(RUNTIME_CLASS(CMemoryException)))
		{
            theApp.LogInfo( L"ParseXMLFile: 'new' operation caused exception!" );
		}

		bResult = false;
	}
	catch(...)
	{
		strError = L"Unknown exception occured in ParseXMLFile!";
		theApp.LogInfo( strError );
		MessageBox( theApp.m_pMainWnd->GetSafeHwnd(), strError, L"", MB_OK|MB_ICONERROR );
		bResult = false;
	}

	CoUninitialize();
	return bResult;
}

// =======================================================================
// Function name  :xxxElement
// Description    :XML Parser
//                 RequiredElement:      Required
//                 OptionListElement:    OptionLists
//                 AliasElement:         Variables
//                 SettingElement:       Settings                
//                 MonitorValueElement:  Monitor/AlarmValues
//                 CommunicationElement: Communications 
// Return type    : void
// Parameters     : pNodePtr: Parent Node
// Author         : WangDong 2010.12.08 09:30:00
// =======================================================================
void CConfiguratorDoc::RequiredElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
    ASSERT( pParentNode && m_aProperties );
	if((!pParentNode) || (!m_aProperties))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList( pParentNode->GetchildNodes());  // Child nodes list
	if( pNodeList == NULL )
	{
		return;
	}

	// Parse all non-attribute child nodes of the parent node
	MSXML2::IXMLDOMNodePtr pChildNode;                             // Non-attribute child nodes 
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;                    // Attributes map
	MSXML2::IXMLDOMNodePtr pAttributeNode;                         // Attribute node
	for( long i=0; i< pNodeList->Getlength(); i++ )
	{
		pChildNode = pNodeList->Getitem( i );                      // Get non-attribute child node
		int nNodeType = pChildNode->nodeType;                      // Node type  
		if( pChildNode && (MSXML2::NODE_ELEMENT == nNodeType))   
		{
			pAttributes = pChildNode->Getattributes();            
			ASSERT( pAttributes );
			if( pAttributes )
			{
				LPCTSTR pcStrName = L"", pcStrValue = L"";

				// Name attribute
				pAttributeNode = pAttributes->getNamedItem( "Name" );
				if( pAttributeNode )
				{
					pcStrName = pAttributeNode->Gettext();
				}

				// Value attribute
				pAttributeNode = pAttributes->getNamedItem( "Value" );
				if( pAttributeNode )
				{
					pcStrValue = pAttributeNode->Gettext();
				}

				m_aProperties->Add( pcStrName, pcStrValue );
			}
		}
	}//for
}

//-----------------------------------------------------------------------
// Function name  : OptionListElement
// Description    : XML Parser:  OptionLists              
// Return type    : void
// Parameters     : pNodePtr: OptionLists Node
// Author         : WangDong 2010-12-21 09:20:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::OptionListsElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
	ASSERT( pParentNode && m_aOptions );
	if((!pParentNode) || (!m_aOptions))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList( pParentNode->GetchildNodes());  // Child nodes list
	if( pNodeList == NULL )
	{
        return;
	}

	// Parse all OptionList nodes of the OptionLists
	MSXML2::IXMLDOMNodePtr pCurrentNode;                           // Non-attribute child nodes
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;                    // Attributes map
	MSXML2::IXMLDOMNodePtr pAttributeNode;                         // Attribute node: Name of OptionList
	for( long i=0; i<pNodeList->Getlength(); i++ )
	{
		pCurrentNode = pNodeList->Getitem( i );                    // Get non-attribute node
		if( pCurrentNode != NULL )
		{
		    int nNodeType = pCurrentNode->nodeType;                // Node type 
		    if( MSXML2::NODE_ELEMENT == nNodeType )        
		    {
				COptionList* pOptList = new COptionList();
				ASSERT( pOptList != NULL );
				if( pOptList == NULL )
				{
					theApp.LogInfo( L"OptionListsElement: new COptionList failed!" );
					break;
				}

				pAttributes = pCurrentNode->Getattributes();
				ASSERT( pAttributes );                                         
				if( pAttributes != NULL )
				{
					// Name attribute
					pAttributeNode = pAttributes->getNamedItem("Name");           // Attribute Child Node
					ASSERT( pAttributeNode );                                     // OptionList node should have Name attribute
					if( pAttributeNode != NULL )
					{
						pOptList->SetName( pAttributeNode->Gettext());
				
						if( m_aOptions->find_name( pOptList->GetName()) == NULL  )
						{
							ParseOptionList( pCurrentNode, pOptList );
							m_aOptions->insert( COptionListMap::value_type( pOptList->GetName(), pOptList ));
						}
						else       // Already exists
						{
							// Log Error
							CString strErr;
							strErr.Format( L"Duplicate OptionList found: %s", pOptList->GetName() );
							theApp.LogInfo( strErr );

							SAFE_DELETE( pOptList ); 
						}
					}//if( pAttributeNode != NULL )
				}//if( pAttributes != NULL )
			}//if NODE_ELEMENT
		}//if( pCurrentNode != NULL )
	}//for
}

//-----------------------------------------------------------------------
// Function name  : ParseOptionList
// Description    : Parse OptionList Item                
// Return type    : void
// Parameters     : pOptListNode: OptionList Node
//                  pOptListMap:  OptionList Map
// Author         : WangDong 2010.12.21 11:15:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::ParseOptionList( MSXML2::IXMLDOMNodePtr pOptListNode, COptionList* pOptListMap )
{
    ASSERT( pOptListNode && pOptListMap );
	if((!pOptListNode) || (!pOptListMap))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pOptListNode->GetchildNodes();
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();
	if( lLength == 0 )                      // No OptionItem node
	{
		return;
	}

	MSXML2::IXMLDOMNodePtr pCurrentNode;                        // Non-attribute child nodes
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;                 // Attributes map
	MSXML2::IXMLDOMNodePtr pAttributeNode;                      // Attribute node
	for( long i=0; i<lLength; i++ )
	{
		pCurrentNode = pNodeList->Getitem( i );                 // Non-attribute child node
		if( pCurrentNode != NULL )
		{
			int nNodeType = pCurrentNode->nodeType;             // Node Type  
			if( MSXML2::NODE_ELEMENT == nNodeType )             // OptionItem Node
			{
				pAttributes = pCurrentNode->Getattributes();
				ASSERT( pAttributes );
				if( pAttributes != NULL )
				{
					if( theApp.GetProductType() == CConfiguratorApp::TS350 )       // TS350
					{
						// Check Application Type:ItemAppType, if not match, can't add it
						pAttributeNode = pAttributes->getNamedItem("ItemAppType");  // Attribute Child Node
						if( pAttributeNode )
						{
							CString str = pAttributeNode->Gettext();
							if( theApp.GetAppType() == APPTYPE_GEN )       // Gen
							{
								if( str.CompareNoCase( L"Mech" ) == 0 )
								{
									continue;
								}
							}
							else if( theApp.GetAppType() == APPTYPE_MECH )  // Mech
							{
								if( str.CompareNoCase( L"Gen" ) == 0 )
								{
									continue;
								}
							}
						}//if( pAttributeNode )
					}//if TS350

					COptionItem* pOptItem = new COptionItem();
					ASSERT( pOptItem != NULL );
					if( pOptItem == NULL )
					{
						theApp.LogInfo( L"ParseOptionList: new COptionItem failed!" );
						break;
					}

					// Value Property
					pAttributeNode = pAttributes->getNamedItem( "Value" );
					if( pAttributeNode != NULL )
					{
						LPCTSTR pcStrValue = pAttributeNode->Gettext();
						UINT uValue = 0;

						// Check Value attribute is decimal or hex
						CString s(pcStrValue);
						if( s.Find( L"0x" ) != -1 )   // found: Hex
						{
							::swscanf_s( pcStrValue, L"%x", &uValue );
						}
						else                          // not found：Decimal
						{
							::swscanf_s( pcStrValue, L"%d", &uValue );
						}

						pOptItem->SetValue( uValue );
					}

					// Name Property
					pAttributeNode = pAttributes->getNamedItem("Name");         // Attribute Child Node
					if( pAttributeNode != NULL )
					{
						CString strName = pAttributeNode->Gettext();
						if((CConfiguratorApp::TS350 == theApp.GetProductType())                                                &&  // TS350 
						   ((pOptListMap->GetName().CompareNoCase(L"Din Assignment") == 0)   && ( 32 == pOptItem->GetValue())) ||  // Din Assignment,  index 32
						   ((pOptListMap->GetName().CompareNoCase(L"Aout Assignment") == 0)  && ( 6 == pOptItem->GetValue()))  ||  // Aout Assignment, index 6
						   ((pOptListMap->GetName().CompareNoCase(L"Speed Droop Type") == 0) && ( 0  == pOptItem->GetValue())))    // Speed Droop Type, index 0
						{
							// TS350: Gen is differrent from Mech 
							int index = strName.Find( L'/' );
							ASSERT( index != -1 );
							if( theApp.GetAppType() == APPTYPE_GEN )
							{
								strName = strName.Left( index );
							}
							else
							{
								strName = strName.Right( strName.GetLength() - (index + 1));
							}
						}

						pOptItem->SetName( strName ); 
					}

					// ItemAppType
					pAttributeNode = pAttributes->getNamedItem("ItemAppType");  // Attribute Child Node
					if( pAttributeNode != NULL )
					{
						CString str = pAttributeNode->Gettext();
						if( str.CompareNoCase( L"Gen" ) == 0 )
						{
							pOptItem->SetItemAppType( APPTYPE_GEN );
						}
						else if( str.CompareNoCase( L"Mech" ) == 0 )
						{
							pOptItem->SetItemAppType( APPTYPE_MECH );
						}
						else
						{
							pOptItem->SetItemAppType( APPTYPE_COMM );
						}
					}

					//Check if current OptionItem exists
					if( pOptListMap->find_num( pOptItem->GetValue()) == NULL )
					{
						pOptListMap->insert( COptionList::value_type(pOptItem->GetValue(), pOptItem));
					}
					else   // Already Exists
					{
						 // Log Error
						 CString strErr;
						 strErr.Format( L"Duplicate option item found: %d - %s",  pOptItem->GetValue(),  pOptItem->GetName());
						 theApp.LogInfo( strErr );
				 
						 SAFE_DELETE( pOptItem );
					}
				}//if( pAttributes != NULL )
			}//if( MSXML2::NODE_ELEMENT == nNodeType )
		}//if( pCurrentNode != NULL ) 
	}//for
}

//-----------------------------------------------------------------------
// Function name  : AliasElement
// Description    : XML Parser:  Variables              
// Return type    : void
// Parameters     : pNodePtr: Variables Node
// Author         : WangDong 
//-----------------------------------------------------------------------
void CConfiguratorDoc::AliasElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
	ASSERT( pParentNode && m_aAliases );
	if((!pParentNode) || (!m_aAliases))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList( pParentNode->GetchildNodes());
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();                       // Get non-attribute child nodes number
	ASSERT( lLength >= 1 );                                      // Variables should have one non-attribute child node at least
	if( lLength < 1 )
	{
		theApp.LogInfo( L"AliasElement: Variables should have one non-attribute child node at least!" );
		return;
	}

	MSXML2::IXMLDOMNodePtr pChildNode;                  
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;          
	MSXML2::IXMLDOMNodePtr pAttributeNode;               

	// Parse all Variable node
	bool bNewCreatedAlias = false;
	for( long i=0; i< lLength; i++ )
	{
		pChildNode = pNodeList->Getitem( i );                      // Get non-attribute child node
		if( pChildNode == NULL )
		{
			continue;
		}

		int nNodeType = pChildNode->nodeType;                      // Node Type  
		if( MSXML2::NODE_ELEMENT == nNodeType )                    // Variable node
		{
			pAttributes = pChildNode->Getattributes();             // Attributes map
			ASSERT( pAttributes );
			if( pAttributes == NULL )
			{
				continue;
			}

			CAliasItem* pAlias = NULL;

			// 1. Name Attribute
			pAttributeNode = pAttributes->getNamedItem("Name");     // Attribute Child Node
			ASSERT( pAttributeNode );                               // Variable node should have Name attribute
			if( pAttributeNode )
			{
				LPCTSTR pcStrName = pAttributeNode->Gettext();
				pAlias = m_aAliases->find_name( pcStrName );
				if( pAlias != NULL )      // already exists
				{
					bNewCreatedAlias = false;
                    continue;
				}
				else              // not exists
				{
                     bNewCreatedAlias = true;
					 pAlias = new CAliasItem();
					 if( pAlias == NULL )
					 {
						 theApp.LogInfo( L"AliasElement: New CAliasItem Failed!" );
						 break;
					 }
				}//else

				pAlias->SetName( pcStrName );
			}

			// 2. Alias Attribute
			pAttributeNode = pAttributes->getNamedItem("Alias");
			ASSERT( pAttributeNode );                                // Variable node should have Alias attribute
			if( pAttributeNode )
			{
				int nAlias = 0;
				LPCTSTR pcStrAlias = pAttributeNode->Gettext();
				pAlias->SetAliasString( pcStrAlias );

				::swscanf_s( pcStrAlias, L"%d", &nAlias );
				pAlias->SetAlias( nAlias );
			    ASSERT( pAlias->GetAlias() >0 );                     // Must exist and > 0
			}
			
			// 3. Desc Property
			pAttributeNode = pAttributes->getNamedItem("Desc");
			if( pAttributeNode )
			{
				pAlias->SetDescription( pAttributeNode->Gettext());
			}

			// 4. DataType Property
			pAttributeNode = pAttributes->getNamedItem("DataType");
			if( pAttributeNode )
			{
				LPCTSTR pcStrType = pAttributeNode->Gettext();
				CString strDataType( pcStrType );
				if( strDataType.CompareNoCase( L"Bool" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_BOOLEAN );
				}
				else if( strDataType.CompareNoCase( L"Int" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_INTEGER );
				}
				else if( strDataType.CompareNoCase( L"UInt" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_UINTEGER );
				}
				else if( strDataType.CompareNoCase( L"Real" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_FLOAT );
				}
				else if( strDataType.CompareNoCase( L"RealSwap" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_FLOATSWAP );
				}
				else if( strDataType.CompareNoCase( L"Long" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_LONG );
				}
				else if( strDataType.CompareNoCase( L"LongSwap" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_LONGSWAP );
				}
				else if( strDataType.CompareNoCase( L"String" ) == 0 )
				{
					pAlias->SetDataType( CAliasItem::ALIAS_STRING );

					// 4.1 Length Property: Set byte count
					pAttributeNode = pAttributes->getNamedItem("Length");
					if( pAttributeNode )
					{
						int nValue = 0;
						LPCTSTR pcStr = pAttributeNode->Gettext();
						::swscanf_s( pcStr, L"%d", &nValue );
						pAlias->SetStringSize( nValue );
					}
				}
			}//if( pAttributeNode )

			// 5. Protect Property
			pAttributeNode = pAttributes->getNamedItem("Protect");
			if( pAttributeNode )
			{
				LPCTSTR pcStrProtect = pAttributeNode->Gettext();
				CString strProtect( pcStrProtect );
				if( strProtect.CompareNoCase( L"Tune" ) == 0 )
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_TUNE );
				}
				else if( strProtect.CompareNoCase( L"Configure" ) == 0 )
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_CONFIGURE );
				}
				else if( strProtect.CompareNoCase( L"Setpoint" ) == 0 )
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_SETPOINT );
				}
				else if( strProtect.CompareNoCase( L"Readonly" ) == 0 )
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_READONLY );
				}
				else if( strProtect.CompareNoCase( L"Calibrate" ) == 0 )
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_CALIBRATE );
				}
				else
				{
					pAlias->SetProtectType( CAliasItem::PROTECT_DEFAULT );
				}
			}

			// 6. Default Property
			pAttributeNode = pAttributes->getNamedItem("Default");
			if( pAttributeNode )
			{
				LPCTSTR pcStrAlias = pAttributeNode->Gettext();
				pAlias->SetValue( pcStrAlias );
			}

			// 7. ItemAppType
			pAttributeNode = pAttributes->getNamedItem("ItemAppType");  // Attribute Child Node
			if( pAttributeNode )
			{
				CString str = pAttributeNode->Gettext();
				if( str.CompareNoCase( L"Gen" ) == 0 )
				{
					pAlias->SetItemAppType( APPTYPE_GEN );
				}
				else if( str.CompareNoCase( L"Mech" ) == 0 )
				{
                    pAlias->SetItemAppType( APPTYPE_MECH );
				}
				else
				{
					pAlias->SetItemAppType( APPTYPE_COMM );
				}
			}

			if( bNewCreatedAlias )   // new operate
			{
			    m_aAliases->Add( pAlias );
			}
		}//if( pChildNode && (MSXML2::NODE_ELEMENT == nNodeType))
	}//for
}

//-----------------------------------------------------------------------
// Function name  : SettingElement
// Description    : XML Parser:  Settings              
// Return type    : void
// Parameters     : pNodePtr: Settings Node
// Author         : WangDong 
//-----------------------------------------------------------------------
void CConfiguratorDoc::SettingElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
    ASSERT( pParentNode && m_aSettings );
	if((!pParentNode) || (!m_aSettings))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList( pParentNode->GetchildNodes());
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength(); // Get non-attribute child nodes number
	ASSERT( lLength >= 1 );                // Settings should have one non-attribute child node at least
	if( lLength < 1 )
	{
		theApp.LogInfo( L"SettingElement: Settings should have one non-attribute child node at least!" );
		return;
	}

	// Parse all First Level non-attribute child nodes of Settings
	MSXML2::IXMLDOMNodePtr pCurrentNode;   // Non attribute child node
	for( long i=0; i<lLength; i++ )
	{
		pCurrentNode = pNodeList->Getitem( i ); 
		if( pCurrentNode != NULL )
		{
			int nNodeType = pCurrentNode->nodeType;                             
			if( MSXML2::NODE_ELEMENT == nNodeType )                                // First Level Setting child node
			{
				CSettingItem* pSetting = ParseSingleSettingItem( pCurrentNode );   // Parse Single Setting Node
				if( pSetting == NULL )    
				{
					continue;
				}

				// Check if First Level Setting node has child notes
				if( pCurrentNode->GetchildNodes() != NULL )
				{
					long lChildLength = pCurrentNode->GetchildNodes()->Getlength();
					if( lChildLength >= 1 )  
					{
						AddChildSettings( pCurrentNode, pSetting );  
					}
				}

				// Add all First Level non-attribute child nodes of Settings to m_aSettings
				m_aSettings->push_back( pSetting );
			}//if( MSXML2::NODE_ELEMENT == nNodeType ) 
		}//if( pCurrentNode != NULL )
	}// for
}

//-------------------------------------------------------------------------------
// Function name  :ParseSettingItem
// Description    :Parse Single Setting Item                
// Return type    :If success: CSettingItem*, Object of Class CSettingItem
//                 If pNode is NULL or not an element: NULL
// Parameters     :pNode: Element Node
// Author         :WangDong 2010.12.14 17:15:00
//-------------------------------------------------------------------------------
CSettingItem* CConfiguratorDoc::ParseSingleSettingItem( MSXML2::IXMLDOMNodePtr pNode )
{
	ASSERT( pNode );
	if( !pNode )
	{
		return NULL;
	}
	
	int nNodeType = pNode->nodeType;    
	ASSERT( nNodeType == MSXML2::NODE_ELEMENT );
	if( nNodeType != MSXML2::NODE_ELEMENT )
	{
		return NULL;
	}

	if( pNode && (MSXML2::NODE_ELEMENT == nNodeType))          // Setting Node
	{   
	    MSXML2::IXMLDOMNodePtr pAttributeNode;          // Attribute node
	    MSXML2::IXMLDOMNamedNodeMapPtr pAttributes( pNode->Getattributes());  // Attributes map                
		ASSERT( pAttributes );
		if( pAttributes == NULL )
		{
			return NULL;
		}

		// TS350
		if( CConfiguratorApp::TS350 == theApp.GetProductType() )
		{
			// Check Application Type:ItemAppType, if not match, can't add it
			pAttributeNode = pAttributes->getNamedItem("ItemAppType");  
			if( pAttributeNode )
			{
				CString str = pAttributeNode->Gettext();
				if( theApp.GetAppType() == APPTYPE_GEN )       // Gen
				{
					if( str.CompareNoCase( L"Mech" ) == 0 )
					{
						return NULL;
					}
				}
				else                                           // Mech
				{
					if( str.CompareNoCase( L"Gen" ) == 0 )
					{
						return NULL;
					}
				}
			}// if( pAttributeNode )

			// If has 'Variable' attribute, check its itemapptype
			pAttributeNode = pAttributes->getNamedItem("Variable");    
			if( pAttributeNode )
			{
				CString str = pAttributeNode->Gettext();
				if((!str.IsEmpty()) && (m_aAliases != NULL))
				{
					CAliasItem* pAlias = m_aAliases->find_name( str );
					if( !CheckReadPtr( pAlias, sizeof(CAliasItem)))  // not exist
					{
						return NULL;
					}
					else  // exist: if not common, and not match, return NULL
					{
						if((pAlias->GetItemAppType() != APPTYPE_COMM) && 
						   (pAlias->GetItemAppType() != theApp.GetAppType()))
						{
                            return NULL;  
						}
					}
				}
			}//if( pAttributeNode )
		}//if TS350

		CSettingItem* pSetting = new CSettingItem();
		if( pSetting == NULL )
		{
			theApp.LogInfo( L"ParseSingleSettingItem: New CSettingItem Failed!" );
			return NULL;
		}

		// Name Attribute
		pAttributeNode = pAttributes->getNamedItem("Name");
		ASSERT( pAttributeNode );                              // Setting Node should have Name attribute 
		if( pAttributeNode )
		{
			CString str = pAttributeNode->Gettext();
			if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
			   (str.CompareNoCase( L"Process PID/MW PID" ) == 0))
			{
                str = (theApp.GetAppType() == APPTYPE_GEN) ? L"MW PID" : L"Process PID";   
			}

			pSetting->SetName( str );
		}

		// HelpName Attribute
		pAttributeNode = pAttributes->getNamedItem("HelpName");   
		if( pAttributeNode )
		{
			pSetting->SetHelpName( pAttributeNode->Gettext());
		}
		
		// Variable Attribute
		pAttributeNode = pAttributes->getNamedItem("Variable");   
		if( pAttributeNode )
		{
			pSetting->SetVariableName( pAttributeNode->Gettext());
		}

		// RangMin Attribute
		pAttributeNode = pAttributes->getNamedItem("RangeMin");   
		if( pAttributeNode )
		{
			pSetting->SetRangeMinString( pAttributeNode->Gettext());
		}
		
		// RangMax Attribute
		pAttributeNode = pAttributes->getNamedItem("RangeMax");   
		if( pAttributeNode )
		{ 
			pSetting->SetRangeMaxString( pAttributeNode->Gettext());
		}

		// Units Attribute
		pAttributeNode = pAttributes->getNamedItem("Units");   
		if( pAttributeNode )
		{
			pSetting->SetUnits( pAttributeNode->Gettext());
		}

		// OptionList Attribute
		pAttributeNode = pAttributes->getNamedItem("OptionList");   
		if( pAttributeNode )
		{
			pSetting->SetOptionListName( pAttributeNode->Gettext());
		}

		// ItemAppType
		pAttributeNode = pAttributes->getNamedItem("ItemAppType");  
		if( pAttributeNode )
		{
			CString str = pAttributeNode->Gettext();
			if( str.CompareNoCase( L"Gen" ) == 0 )
			{
				pSetting->SetItemAppType( APPTYPE_GEN );
			}
			else if( str.CompareNoCase( L"Mech" ) == 0 )
			{
                pSetting->SetItemAppType( APPTYPE_MECH );
			}
			else
			{
				pSetting->SetItemAppType( APPTYPE_COMM );
			}
		}

		return pSetting;
	}//if( pNode && (MSXML2::NODE_ELEMENT == nNodeType))
	else
	{
		return NULL;
	}
}

//-------------------------------------------------------------------------------
// Function name  :AddChildSettings
// Description    :Parse Child SettingItem, and add it to Parent SettingItem              
// Return type    :void                
// Parameters     :pParentNode:    must have child nodes, otherwise return;
//                 pParentSetting: Parent SettingItem
// Author         :WangDong 2010.12.14 18:30:00
//-------------------------------------------------------------------------------
void CConfiguratorDoc::AddChildSettings( MSXML2::IXMLDOMNodePtr pParentNode, CSettingItem* pParentSetting )
{
    ASSERT( pParentNode && pParentSetting );
	if((!pParentNode) || (!pParentSetting))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pParentNode->GetchildNodes();
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();    // Get non-attribute child nodes number 
	ASSERT( lLength > 0);
	if( lLength == 0 )                        // No child nodes 
	{
	    return;
	}

    // Parse all non-attribute nodes
	CSettingItem* pSetting = NULL;
	MSXML2::IXMLDOMNodePtr pCurrentNode;      // non-attribute node
	for( long i=0; i< lLength; i++ )
	{
        pCurrentNode = pNodeList->Getitem( i );
		if( pCurrentNode != NULL )
		{
			int nNodeType = pCurrentNode->nodeType;                       
			if( MSXML2::NODE_ELEMENT == nNodeType )    // Setting node
			{
				pSetting = ParseSingleSettingItem( pCurrentNode );      // Parse Single Setting Node
				if( pSetting == NULL )
				{
					continue;
				}

				// If child setting helpname is empty, set it with its parent helpname
				CString strHelpName = pSetting->GetHelpName();
				if( strHelpName.IsEmpty() && (!pParentSetting->GetHelpName().IsEmpty()))
				{
					pSetting->SetHelpName( pParentSetting->GetHelpName()); 
				}
			
				pParentSetting->AddSubItem( pSetting );

				// Check if current node has child notes
				if( pCurrentNode->GetchildNodes() != NULL )
				{
					long lChildLength = pCurrentNode->GetchildNodes()->Getlength();
					if( lChildLength >= 1 )  
					{
						AddChildSettings( pCurrentNode, pSetting );  
					}
				}
			}//if( MSXML2::NODE_ELEMENT == nNodeType )
		}//if( pCurrentNode != NULL )
	}//for
}

//-----------------------------------------------------------------------
// Function name  : MonitorValueElement
// Description    : XML Parser
//                  Values   View    and    Alarms View
//                  "MonitorValues"        "AlarmValues"                 
// Return type    : void
// Parameters     : pNodePtr: MonitorValues / AlarmValues Node
//                  bIsAlarm: True,  Alarms View; 
//                            False, Values View
// Author         : WangDong 
//-----------------------------------------------------------------------
void CConfiguratorDoc::MonitorValueElement( MSXML2::IXMLDOMNodePtr pParentNode, bool bIsAlarm )
{
    ASSERT( pParentNode && ((!bIsAlarm && m_aMonitorValues) || (bIsAlarm && m_aAlarmValues)));
	if( !(pParentNode && (((!bIsAlarm) && m_aMonitorValues) || (bIsAlarm && m_aAlarmValues))))
	{
		CString str;
		str.Format( L"MonitorValueElement - %s: Entry parameters are error!", (bIsAlarm ? L"Alarm View" : L"Trend View" ));
		theApp.LogInfo( str );

		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList( pParentNode->GetchildNodes());
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();                          // Get non-attribute child nodes number 
	ASSERT( lLength >= 1 );                                         // xxxValues should have one xxxValue node at least
	if( lLength < 1 )
	{
		return;
	}

	// Parse all non-attribute child nodes
	MSXML2::IXMLDOMNodePtr pChildNode;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes; 
	MSXML2::IXMLDOMNodePtr pAttributeNode;
	for( long i = 0; i < lLength; i++ )
	{
		pChildNode = pNodeList->Getitem( i ); 
		if( pChildNode == NULL )
		{
			continue;
		}

		int nNodeType = pChildNode->nodeType;
		if( MSXML2::NODE_ELEMENT == nNodeType )                     // xxxValue node
		{    
			pAttributes = pChildNode->Getattributes();              // attributes map 
			ASSERT( pAttributes );
			if( pAttributes == NULL )
			{
				continue;
			}

			// TS350
			if( theApp.GetProductType() == CConfiguratorApp::TS350 )
			{
				// Check Application Type:ItemAppType, if not match, can't add it
				pAttributeNode = pAttributes->getNamedItem("ItemAppType");  
				if( pAttributeNode != NULL )
				{
					CString str = pAttributeNode->Gettext();
					if( theApp.GetAppType() == APPTYPE_GEN )       // Gen
					{
						if( str.CompareNoCase( L"Mech" ) == 0 )
						{
							continue;
						}
					}
					else                                           // Mech
					{
						if( str.CompareNoCase( L"Gen" ) == 0 )
						{
							continue;
						}
					}
				}//if( pAttributeNode != NULL )
			}//if TS350

			CCfgMonitorValue value;

			// 1. Title attribute
			pAttributeNode = pAttributes->getNamedItem( "Title" );
			ASSERT( pAttributeNode );                               // xxxValue node should have Title attribute
			if( pAttributeNode )                          
			{  
				value.SetTitle( pAttributeNode->Gettext());              
			}
		
			// 2. Variable attribute
			pAttributeNode = pAttributes->getNamedItem( "Variable" );  
			if( pAttributeNode )
			{                
				value.SetVariable( pAttributeNode->Gettext());
			}

			// 3. Color attribute
			pAttributeNode = pAttributes->getNamedItem( "FontColor" );  
			if( pAttributeNode )
			{                
				CString strColor( (LPCTSTR)pAttributeNode->Gettext());
				if( strColor.CompareNoCase( L"Red" ) == 0 )
				{
					value.SetFontColor( global::clRed );                     
				}
				else if( strColor.CompareNoCase( L"Fuchsia" ) == 0 )      
				{
					value.SetFontColor( global::clFuchsia );              
				}
				else if( strColor.CompareNoCase( L"Purple" ) == 0 )
				{
					value.SetFontColor( global::clPurple );
				}
				else
				{
					value.SetFontColor( global::clBlack );
				}
			}

			// 4. ItemAppType
			pAttributeNode = pAttributes->getNamedItem("ItemAppType");  
			if( pAttributeNode )
			{
				CString str = pAttributeNode->Gettext();
				if( str.CompareNoCase( L"Gen" ) == 0 )
				{
					value.SetItemAppType( APPTYPE_GEN );
				}
				else if( str.CompareNoCase( L"Mech" ) == 0 )
				{
					value.SetItemAppType( APPTYPE_MECH );
				}
				else
				{
					value.SetItemAppType( APPTYPE_COMM );
				}
			}
			
			if( !bIsAlarm )                         // Values View
			{
			    m_aMonitorValues->push_back( value );
			}
			else                                    // Alarm View
			{
				m_aAlarmValues->push_back( value );
			}
		}
	} //for
}

//-----------------------------------------------------------------------
// Function name  : CommunicationElement
// Description    : XML Parser:  Settings              
// Return type    : void
// Parameters     : pNodePtr: Settings Node
// Author         : WangDong 2010.12.23 13:30:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::CommunicationElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
	ASSERT( pParentNode && m_aActions.get());
	if((!pParentNode) || (!m_aActions.get()))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pParentNode->GetchildNodes();  // Child nodes list
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();            // Get non-attribute child nodes number
	ASSERT( lLength >= 1 );                           // Communications has one Action node at least
	if( lLength < 1 )
	{
		return;
	}

	// Parse all Action nodes of the Communications
	MSXML2::IXMLDOMNodePtr pCurrentNode;              // Non-attribute child nodes
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;       // Attributes map
	MSXML2::IXMLDOMNodePtr pAttributeNode;            // Attribute node: Name of Action
	for( long i=0; i< lLength; i++ )
	{
		pCurrentNode = pNodeList->Getitem( i );       // Get non-attribute node
		if( pCurrentNode == NULL )
		{
			continue;
		}

		int nNodeType = pCurrentNode->nodeType;       // Node type 
		if( MSXML2::NODE_ELEMENT == nNodeType )        
		{
			pAttributes = pCurrentNode->Getattributes();
			ASSERT( pAttributes );
			if( pAttributes == NULL )
			{
				continue;
			}

			modbus::CMessageRanges msgRanges;

			// Name attribute
			pAttributeNode = pAttributes->getNamedItem("Name");           // Attribute Child Node
			ASSERT( pAttributeNode );                                     // Action node should have Name attribute
			if( pAttributeNode )
			{
				LPCTSTR pcStrName = pAttributeNode->Gettext();            // Name: Monitor
				if( m_aActions->find_name( pcStrName ) == NULL )
				{
					msgRanges.SetActionName( pcStrName );
					ParseAction( pCurrentNode, &msgRanges );
					m_aActions->insert( modbus::CMessageActions::value_type( pcStrName, msgRanges ));
				}
				else  // Already exists
				{
					CString strErr;
		            strErr.Format( L"Duplicate Action found: %s", pcStrName );
		            theApp.LogInfo( strErr );
				}
			}
		}
	}// for
}

//-----------------------------------------------------------------------
// Function name  : ParseAction
// Description    : Parse Single Action Node           
// Return type    : void
// Parameters     : pActionNode: Action Node
//                  pMsgRanges:  CMessageRange Vector
// Author         : WangDong 2010.12.23 16:30:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::ParseAction( MSXML2::IXMLDOMNodePtr pActionNode, modbus::CMessageRanges* pMsgRanges )
{
    ASSERT( pActionNode && pMsgRanges );
	if((!pActionNode) || (!pMsgRanges))
	{
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pActionNode->GetchildNodes();
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();
	if( lLength == 0 )
	{
		return;
	}

	MSXML2::IXMLDOMNodePtr pCurrentNode;            // Non-attribute child nodes
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;     // Attributes map
	MSXML2::IXMLDOMNodePtr pAttributeNode;          // Attribute node
	for( long i=0; i<lLength; i++ )
	{
		pCurrentNode = pNodeList->Getitem( i );  
		if( pCurrentNode != NULL )
		{
			int nNodeType = pCurrentNode->nodeType;                           
			if( MSXML2::NODE_ELEMENT == nNodeType ) // Message Node
			{
				pAttributes = pCurrentNode->Getattributes();
				ASSERT( pAttributes );
				if( pAttributes == NULL )
				{
					continue;
				}

				int nBegin = 0, nEnd = 0;
				LPCTSTR pcStrValue = L"";

				// Begin Property
				pAttributeNode = pAttributes->getNamedItem( "Begin" );
				if( pAttributeNode )
				{
					pcStrValue = pAttributeNode->Gettext();

					// Check Value attribute is decimal or hex
					CString s(pcStrValue);
					if( s.Find( L"0x" ) != -1 )   // found: Hex
					{
						::swscanf_s( pcStrValue, L"%x", &nBegin );
					}
					else                          // not found：Decimal
					{
						::swscanf_s( pcStrValue, L"%d", &nBegin );
					}
				}

				// End Property
				pAttributeNode = pAttributes->getNamedItem( "End" );
				if( pAttributeNode )
				{
					pcStrValue = pAttributeNode->Gettext();

					// Check Value attribute is decimal or hex
					CString s(pcStrValue);
					if( s.Find( L"0x" ) != -1 )   // found: Hex
					{
						::swscanf_s( pcStrValue, L"%x", &nEnd );
					}
					else                          // not found：Decimal
					{
						::swscanf_s( pcStrValue, L"%d", &nEnd );
					}
				}

				pMsgRanges->push_back( modbus::CMessageRange( nBegin, nEnd ));
			}//if( MSXML2::NODE_ELEMENT == nNodeType )
		}//if( pCurrentNode != NULL )
	}//for
}

// =======================================================================
// Function name  : BindDocument
// Description    : Binds the monitored value aliases to names in the alias map
//                  1. m_aLogging
//                  2. m_aTraces
// Return type    : void
// Parameters     : void
// Author         : rjvs     2002.11.12 14:25:54 -06:00
//                  WangDong 2011.02.09 16:15:00
// =======================================================================
void CConfiguratorDoc::BindDocument( void )
{
    // Make sure all of the trace names are in the dictionary.
    if( CheckWritePtr( m_aTraces, sizeof(CCfgTraceArray)))
    {
        // Add default traces...
        if( m_aTraces->empty())
        {
			CString strTrace1, strTrace2, strTrace3;
			theApp.GetDefTraceName( strTrace1, strTrace2, strTrace3 );
			
            m_aTraces->push_back( CCfgTrace( strTrace1, 0));
            m_aTraces->push_back( CCfgTrace( strTrace2, 1));
            m_aTraces->push_back( CCfgTrace( strTrace3, 1));    
        }

        m_aTraces->bind( m_aAliases );
    }
}

// =======================================================================
// Function name  : BindDictionary
// Description    : Bind to CAliasItem 
//                  1.m_aSettingAliases
//                  2.m_aSettings
//                  3.m_aMonitorValues
//                  4.m_aAlarmValues
// Return type    : void
// Parameters     : void
// Author         : WangDong 2010.12.14 17:15:00
// =======================================================================
void CConfiguratorDoc::BindDictionary()
{
	// 1.m_aSettingAliases
	if( CheckWritePtr( m_aSettingsAlias, sizeof( CSettingVector )))
	{
		// Digital Read/Write
        CSettingItem* pSetting = new CSettingItem();
		if( pSetting == NULL )
		{
			theApp.LogInfo( L"CConfiguratorDoc::BindDictionary - Digital Read/Write: New CSettingItem Failed!" );
			return;
		}

		CString s;
		s.LoadString( IDS_ALIASES_TYPE1 );            
		pSetting->SetName( s );
		pSetting->AddAliases( m_aAliases, 1, 10000 );
		if( pSetting->GetSubItemCount() == 0 )
		{
			SAFE_DELETE( pSetting );
		}
		else
		{
            m_aSettingsAlias->push_back( pSetting );
		}

		// Digital Read Only
		pSetting = new CSettingItem();
		if( pSetting == NULL )
		{
			theApp.LogInfo( L"CConfiguratorDoc::BindDictionary - Digital Read Only: New CSettingItem Failed!" );
			return;
		}

		s.LoadString( IDS_ALIASES_TYPE2 );            
		pSetting->SetName( s );
		pSetting->AddAliases( m_aAliases, 10001, 20000 );
		if( pSetting->GetSubItemCount() == 0 )
		{
			SAFE_DELETE( pSetting );
		}
		else
		{
            m_aSettingsAlias->push_back( pSetting );
		}

		// Analog Read Only
		pSetting = new CSettingItem();
		if( pSetting == NULL )
		{
			theApp.LogInfo( L"CConfiguratorDoc::BindDictionary - Analog Read Only: New CSettingItem Failed!" );
			return;
		}

		s.LoadString( IDS_ALIASES_TYPE3 );            
		pSetting->SetName( s );
		pSetting->AddAliases( m_aAliases, 30001, 40000 );
		if( pSetting->GetSubItemCount() == 0 )
		{
			SAFE_DELETE( pSetting );
		}
		else
		{
            m_aSettingsAlias->push_back( pSetting );
		}

		// Analog Read/Write
		pSetting = new CSettingItem();
		if( pSetting == NULL )
		{
			theApp.LogInfo( L"CConfiguratorDoc::BindDictionary - Analog Read/Write: New CSettingItem Failed!" );
			return;
		}

		s.LoadString( IDS_ALIASES_TYPE4 );            
		pSetting->SetName( s );
		pSetting->AddAliases( m_aAliases, 40001, 50000 );
		if( pSetting->GetSubItemCount() == 0 )
		{
			SAFE_DELETE( pSetting );
		}
		else
		{
            m_aSettingsAlias->push_back( pSetting );
		}

		m_aSettingsAlias->UpdateTotalCount();
        ASSERT(static_cast<long>(m_aAliases->size()) <= m_aSettingsAlias->GetTotalCount());
	}//if

	// 2.m_aSettings
	if( CheckWritePtr( m_aSettings, sizeof( CSettingVector )))
	{
		m_aSettings->resize( m_aSettings->size());     // ？？?
		BindSettings( m_aSettings );
		m_aSettings->UpdateTotalCount();
	}

	// 3.m_aMonitorValues
	if( CheckWritePtr( m_aMonitorValues, sizeof( CCfgMonitorValues )))
	{
		CCfgMonitorValues::iterator it = m_aMonitorValues->begin();
		CCfgMonitorValues::iterator eit = m_aMonitorValues->end();

		while(it != eit )
		{
			it->SetAlias( m_aAliases->find_name( it->GetVariable_C()));
			it++;
		}
	}

	// 4.m_aAlarmValues
	if( CheckWritePtr( m_aAlarmValues, sizeof( CCfgMonitorValues )))
	{
		CCfgMonitorValues::iterator it = m_aAlarmValues->begin();
		CCfgMonitorValues::iterator eit = m_aAlarmValues->end();

		while(it != eit )
		{
			it->SetAlias( m_aAliases->find_name( it->GetVariable_C()));
			it++;
		}
	}
}

//-----------------------------------------------------------------------
// Function name  : BindSettings
// Description    : 1.Bind Variable: set alias
//                  2.Bind OptionList
// Return type    : void
// Parameters     : pSettings: Setting Vector
// Author         : WangDong 
//-----------------------------------------------------------------------
void CConfiguratorDoc::BindSettings( CSettingVector* pSettings )
{
	if( !CheckWritePtr( pSettings, sizeof( CSettingVector )))  
	{
		return;
	}
    
	if((theApp.GetProductType() == CConfiguratorApp::TS350) &&
	   (theApp.GetAppType() == APPTYPE_MECH))
	{
		CSettingVector::iterator it = pSettings->begin();
		CSettingVector::iterator eit = pSettings->end();
		while( it != eit )
		{
			CSettingItem* pSettingItem = *it;
			ASSERT( pSettingItem );
			if( pSettingItem == NULL )
			{
				theApp.LogInfo( L"CConfiguratorDoc::BindSettings - TS350,Mech: pSettingItem is NULL." );

				it++;
				continue;
			}

            CString strName( pSettingItem->GetName());
			if((strName.CompareNoCase( L"Inlet Pressure" ) == 0) &&
			   (pSettingItem->GetParent() != NULL)               &&
			   (pSettingItem->GetParent()->GetName().CompareNoCase(L"Inlet Pressure") == 0))
			{  
                CSettingItem *pParent = pSettingItem->GetParent();
				pParent->GetChildVector()->pop_back();
				for( size_t i = 0; i < pSettingItem->GetSubItemCount(); i++ )
				{
					pParent->AddSubItem( pSettingItem->GetChildVector()->at(i));
				}

				pSettingItem->GetChildVector()->assign( pSettingItem->GetSubItemCount(), NULL );
				SAFE_DELETE( pSettingItem );

				break;
			}

			it++;
		}
	}// if TS350 Mech

	if( CheckWritePtr( pSettings, sizeof( CSettingVector )))  
	{
		CSettingVector::iterator it = pSettings->begin();
		CSettingVector::iterator eit = pSettings->end();

		CSettingItem* pSettingItem = NULL;
		CAliasItem* pAlias = NULL;
		COptionList* pOptList = NULL;
		while( it != eit )
		{
            pSettingItem = *it;
			ASSERT( pSettingItem );
			if( pSettingItem == NULL )
			{
				theApp.LogInfo( L"CConfiguratorDoc::BindSettings: pSettingItem is NULL." );

				it++;
				continue;
			}

			// 1. Bind Variable: set alias
			LPCTSTR pcStrVariable = pSettingItem->GetVariableName_C();    // "": If no Variable attribute
			if( pcStrVariable && (::wcslen(pcStrVariable) > 0))
			{
				pAlias = m_aAliases->find_name( pcStrVariable );
				if( CheckReadPtr( pAlias, sizeof(CAliasItem))) 
				{
				    pSettingItem->SetAlias( pAlias );
				}
				else
                {
					CString strErr;
		            strErr.Format( L"CConfiguratorDoc::BindSettings: Variable '%s' not found in Alias Map.", pcStrVariable );
		            theApp.LogInfo( strErr );
                }
			}

			// 2. Bind OptionList: 
			LPCTSTR pcStrOptListName = pSettingItem->GetOptionListName_C();
			if( pcStrOptListName && ( ::wcslen( pcStrOptListName ) > 0 ))
			{
				pOptList = m_aOptions->find_name( pcStrOptListName );
				if( CheckWritePtr( pOptList, sizeof(COptionList)))
				{
					pSettingItem->SetOptionList( pOptList );
				}
				else
				{
					CString strErr;
		            strErr.Format( L"CConfiguratorDoc::BindSettings: OptionList '%s' not found in OptionList Map.", pcStrOptListName );
		            theApp.LogInfo( strErr );
				}
			}

			// Recurse
			if( pSettingItem->GetChildVector())
			{
                BindSettings( pSettingItem->GetChildVector() );
			}

			it++;   // Move to next iterator
		}//while
	}
}

// =======================================================================
// Function name  : GetSettingContextId
// Description    : Get HelpID of the SettingItem 
// Return type    : DWORD: HelpId
// Parameters     : pSetting: SettingItem
// Author         : WangDong 2010.12.14 17:15:00
// =======================================================================
DWORD CConfiguratorDoc::GetSettingContextId( CSettingItem* pSetting )
{
	DWORD dwHelpId = 0;
	if( CheckReadPtr( pSetting, sizeof(CSettingItem)))
	{
	    CString strHelpName = pSetting->GetHelpName();
		if( !strHelpName.IsEmpty())
		{
            COptionList* pOptionList = m_aOptions->find_name( L"HelpContext" );
			if( pOptionList != NULL )
			{
				COptionItem* pOptionItem = pOptionList->find_name( strHelpName );
				if( pOptionItem != NULL )
				{
					dwHelpId = pOptionItem->GetValue();
				}
			}
		}
	}

	return dwHelpId;
}

// =======================================================================
/************ Trace: Changed by wangdong 2011.02.10 13:00  **************/
/// <summary>Returns the size of the trace array.</summary>
/// <return>The count of array elements.</return>
/// <created>rjvs 2002-11-13 09:47:29 -06:00</created>
size_t CConfiguratorDoc::GetTraceCount() const
{
    if( CheckWritePtr( m_aTraces, sizeof(CCfgTraceArray)))
    {
        CSingleLock sl( &m_aTraces->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

        return m_aTraces->size();
    }

    return 0;
}

/// <summary>Returns the name of a specified trace index.</summary>
/// <param name="SeriesIndex">Index of the trace.</param>
/// <return>A string containing the name of the trace at the specified index.
///   </return>
/// <created>rjvs 2002-11-13 09:49:05 -06:00</created>
CString CConfiguratorDoc::GetTraceName( size_t SeriesIndex ) const
{
    if( CheckWritePtr(m_aTraces, sizeof(CCfgTraceArray)))
    {
        CSingleLock sl( &m_aTraces->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

        if( SeriesIndex < m_aTraces->size())
        {
            return m_aTraces->at( SeriesIndex ).GetName();
        }

		sl.Unlock();
	}

    return CString("");
}

/// <summary>
///   </summary>
/// <param name="TODO">
///   </param>
/// <return>
///   </return>
/// <created>rjvs 2002-11-13 09:31:52 -06:00</created>
HRESULT CConfiguratorDoc::AddTraceName( CString strName, size_t SeriesIndex )
{
    HRESULT hr = E_FAIL;

    if( CheckWritePtr( m_aAliases, sizeof(CAliasMap)) &&
        CheckWritePtr( m_aTraces, sizeof(CCfgTraceArray)))
    {
        // SeriesIndex must be less than the maximum trace count.
        if( SeriesIndex >= global::TRACE_COUNT )
		{
            return E_INVALIDARG;
		}

        // Make sure the trace array is large enough to hold the index.
        if( m_aTraces->size() < global::TRACE_COUNT )
        {
			CSingleLock sl( &m_aTraces->m_cirsec );
			sl.Lock();
			ASSERT( sl.IsLocked());
			
            m_aTraces->resize( global::TRACE_COUNT );
			
			sl.Unlock();
        }

		/* Added by wangdong Begin */
		// Check if pAlias has binded to another Trace
		CCfgTraceArray::iterator it = std::find_if(
                                      m_aTraces->begin(),
                                      m_aTraces->end(),
                                      CCfgTraceFind( strName ));

		// If has binded, remove the trace
	    if( it != m_aTraces->end())   
        {
			CSingleLock sl( &m_aTraces->m_cirsec );
			sl.Lock();
			ASSERT( sl.IsLocked());

            // Remove the trace index.
            it->SetName( L"" );
            it->SetAlias( NULL );
            SetModifiedFlag( TRUE );

			sl.Unlock();
		}
		/* Added by wangdong End */

        // Search for the alias by name
        CAliasItem* pAlias = m_aAliases->find_name( (LPCTSTR)strName );
        if( !CheckReadPtr( pAlias, sizeof(CAliasItem)))
        {
            // Did not find the alias, so clear the array index entry
			CSingleLock sl( &m_aTraces->m_cirsec );
			sl.Lock();
			ASSERT( sl.IsLocked());

            // Delete the name and alias values from the trace array
            CCfgTrace& t = m_aTraces->at( SeriesIndex );
            t.SetName( L"" );
            t.SetAlias( NULL );

			sl.Unlock();
        }
        else
        {
            // Replace array index with selected alias
			CSingleLock sl( &m_aTraces->m_cirsec );
			sl.Lock();
			ASSERT( sl.IsLocked());

			CCfgTrace& t = m_aTraces->at( SeriesIndex );
            t.SetName( strName );
            t.SetAlias( pAlias );

            // Determine if the alias belongs on the left or right axis
            bool bRightAxis = false;
            LPCTSTR aszRightAxis[] = { L"SP", L"Speed", L"Governor" };

            for( int i = 0; i < 3; ++i )
            {
                //if( strName.AnsiPos(aszRightAxis[i]) > 0)
				if( strName.Find( aszRightAxis[i]) != -1 ) 
                {
                    bRightAxis = true;
                    break;
                }
            }

            if( bRightAxis )
			{
                t.SetAxis(1);
			}
            else
			{
                t.SetAxis(0);
			}

			sl.Unlock();
        }

        // Update the current views
        UpdateAllViews( NULL, HINT_TRACE, NULL );
        SetModifiedFlag( TRUE );
        hr = S_OK;
    }

    return hr;
}

/// <summary> Removes an alias from the trace array specified by name.</summary>
/// <param name="AName"> Name of the trace to remove.</param>
/// <return> S_OK if the remove operation is successful.</return>
/// <created> rjvs 2002-11-13 10:00:33 -06:00</created>
HRESULT CConfiguratorDoc::RemoveTraceName( CString strName )
{
    HRESULT hr = E_FAIL;

    if( CheckWritePtr( m_aTraces, sizeof(CCfgTraceArray)))
    {
        CCfgTraceArray::iterator it;

        // Lock the trace array for reading, to search for the trace name.
		CSingleLock sl( &m_aTraces->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

        it = std::find_if(
                m_aTraces->begin(),
                m_aTraces->end(),
                CCfgTraceFind( strName ));

        if (it != m_aTraces->end())
        {
            // Remove the trace index.
            it->SetName( L"" );
            it->SetAlias( NULL );
            SetModifiedFlag( TRUE );
            hr = S_OK;
        }

		sl.Unlock();
    }

    if (hr == S_OK)
    {
        // Update the current views.
        UpdateAllViews(NULL, HINT_TRACE, NULL);
    }

    return hr;
}

// =======================================================================
// Function name  : UpdateRemoteFlags
// Description    : Update m_bDirtyRemote flags
// Return type    : 
// Parameters     : 
// Author         : WangDong  2011.03.17  16:10:00
// =======================================================================
void CConfiguratorDoc::UpdateRemoteFlags( int nBeginAlias, int nEndAlias )
{
	if( CheckWritePtr(m_aAliases, sizeof(COwnedAliasMap))) 
	{
	    m_aAliases->UpdateRemoteFlags( nBeginAlias, nEndAlias );
	}
}

// =======================================================================
// Function name  : GetSendDlgStatus
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong  2011.03.17  16:10:00
// =======================================================================
const CSendStatus* CConfiguratorDoc::GetSendDlgStatus()
{
	m_SendDlgStatus.UpdateSendDlgStatus( m_aSettings );

	if( CheckReadPtr( m_aAliases, sizeof(COwnedAliasMap)))
	{
		if( theApp.GetProductType() == CConfiguratorApp::TS350 )
		{
	        const CAliasItem* pAliasMode = m_aAliases->find_name( L"rMBMODE");     // 35129
			if( CheckReadPtr( pAliasMode, sizeof(CAliasItem)))
			{
				float fMode = pAliasMode->remR4();
				const COptionList* pModeList = m_aOptions->find_name( L"Turbine Modes" );
				if( pModeList != NULL )
				{
					CString strMode = pModeList->FindNameByValue( static_cast<DWORD>(fMode));
					m_SendDlgStatus.SetMode( strMode );
				}
			}

	        const CAliasItem* pAliasSpeed =  m_aAliases->find_name( L"rSPEED");    // 35131
			if( CheckReadPtr( pAliasSpeed, sizeof(CAliasItem)))
			{
				float fSpeed = pAliasSpeed->remR4();
				m_SendDlgStatus.SetSpeed( fSpeed );
			}

	        const CAliasItem* pAliasProgress = m_aAliases->find_name( L"rVALVEV1"); // 35065
			if( CheckReadPtr( pAliasProgress, sizeof(CAliasItem)))
			{
				float fProgress = pAliasProgress->remR4();
				m_SendDlgStatus.SetProgress( fProgress );
			}
		}//TS350
	}
	
    return &m_SendDlgStatus;
}

// =======================================================================
#ifdef SHARED_HANDLERS

// Support for thumbnails
void CConfiguratorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CConfiguratorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CConfiguratorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CConfiguratorDoc diagnostics

#ifdef _DEBUG
void CConfiguratorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConfiguratorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CConfiguratorDoc commands
// =======================================================================
// Function name  : SaveModified
// Description    : return TRUE if ok to continue
//      Called by the framework before a modified document is to be closed.
// Return type    : 
// Parameters     : 
// Author         : WangDong  2011.03.15  14:00:00
// =======================================================================
BOOL CConfiguratorDoc::SaveModified()
{
	if( !IsModified())
	{
        return TRUE;
	}

	CString strSave, strCaption, strSaveWarning;
	strSave.LoadString( IDS_SAVE_CHANGE );
	strCaption.LoadString( IDS_WARNING );
	strSaveWarning.Format( strSave, GetTitle());       // GetTitle():Current document title

	BOOL bResult = FALSE;
	int nResult = MessageBoxW( NULL, strSaveWarning, strCaption, MB_YESNOCANCEL | MB_ICONWARNING );
	
	switch( nResult )
	{
	    case IDYES:     // Save and return TRUE
			DoSaveDocument();
			bResult = TRUE;
		break;

		case IDNO:      // UnSave, return TRUE directly
			bResult = TRUE;
		break;

		case IDCANCEL:  // UnSave, return FALSE directly
			bResult = FALSE;
		break;

		default:
		    ASSERT(FALSE);
		    bResult = FALSE;
		break;
	}

	return bResult;
}

// =========================================================================
/// File Save, SaveAs
// =========================================================================
// Function name: Overwrite << operator
// Description  : 
// Return type  : reference of wofstream
// Parameters   : 
// Author       : WangDong 2011.03.23 11:00:00
// =========================================================================
wofstream& operator <<( wofstream& outfile, const CConfiguratorDoc& doc )
{
	// TS350: 验证CConfiguratorApp::m_nAppType 是否与2143-gAPPTYPE 
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		CAliasItem *pAlias = doc.m_aAliases->find_name( L"gAPPTYPE" );
		ASSERT( pAlias );
		if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
		{
			// 0 - Mech; 1 - Gen
			VARIANT_BOOL bAppType = pAlias->valBool();
			ASSERT( theApp.GetAppType() == (bAppType ? CConfiguratorDoc::APPTYPE_GEN : CConfiguratorDoc::APPTYPE_MECH));
			if( theApp.GetAppType() != (bAppType ? CConfiguratorDoc::APPTYPE_GEN : CConfiguratorDoc::APPTYPE_MECH))
			{
				// Log Error
				CString str;
				str.Format( L"2143-gAPPTYPE:%s is not same with CConfiguratorApp::m_nAppType:%s", 
					        bAppType ? L"Gen" : L"Mech",
							(theApp.GetAppType() == CConfiguratorDoc::APPTYPE_GEN) ? L"Gen" : L"Mech" );
				theApp.LogInfo( str );

				// Set 2143-gAPPTYPE 
				pAlias->valBool( !bAppType );
			}
		}
		else
		{
			theApp.LogInfo( L"TS350: Can't find 2143-gAPPTYPE when operator <<." );
		}
	}//TS350

	int nProductNumber = -1, nAppType = -1;
	float fVersion = 0.0;
	theApp.GetProductInfo( nProductNumber, nAppType, fVersion );

    outfile << "<?xml version= \"1.0\" ?>\n";     // Must be 1.0
	//outfile << "<Product Name=\"" << nProductNumber << "\" Version=\"1.0\">\n"; 
	outfile << "<Product Name=\"" << nProductNumber << "\" Version=\"" << fVersion << "\">\n";
	outfile << "  <Dictionary AppType=\"" << nAppType << "\" Name=\"" << (LPCTSTR)theApp.GetXMLFileName( true ) << "\" />\n";
	outfile << "  <FileVersion>" << fVersion << "</FileVersion>\n";
	outfile << "  <!--\n"
		    << "Warning:  Editing this file by any means, except through the\n"
			<< "PC Configurator will invalidate your configuration.\n"
	        << "Tri-Sen does not support manual editing of any configuration.\n"
			<< "  -->\n";

	// Trend values
	outfile << "  <Trend>\n";
	
	for( size_t i = 0; i < doc.m_aTraces->size(); i++ )
	{
		CCfgTrace Trace = doc.m_aTraces->at( i );
		const CAliasItem* pAlias = Trace.GetAlias();
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
		    outfile << "    <Trace Name=\"" << Trace.GetName_C()
		            << "\" Width=\"" << Trace.GetWidth() 
				    << std::hex
				    << "\" Color=\"0x" << Trace.GetColor()
				    << "\" Axis=\""  << Trace.GetAxis()
				    << "\" />\n";
		}
		else
		{
            outfile <<  "    <Trace Name=\"\" />\n";
		}
	}
	
	outfile << "  </Trend>\n";

	// Aliases
	outfile << "  <Variables>\n";

	COwnedAliasMap::const_iterator it = doc.m_aAliases->begin();
	COwnedAliasMap::const_iterator eit = doc.m_aAliases->end();
	while( it != eit )
	{
		const CAliasItem* pAlias = it ->second;
		ASSERT( CheckReadPtr(pAlias, sizeof(CAliasItem)));
		if( !CheckReadPtr(pAlias, sizeof(CAliasItem)))
		{
			it++;
			continue;
		}

		CString strValue;
		pAlias->GetLocalValueString( strValue );

		outfile << "    <Variable Name=\"";
		outfile << pAlias->GetName();

		if( pAlias->GetRecord())
		{
			outfile << "\" Record=\"True";
		}

		outfile << "\" Value=\"";
		outfile << (LPCTSTR)strValue << "\" />\n";

		it++;
	}

	outfile << "  </Variables>\n";

    outfile << "</Product>";

	return outfile;
}

// =========================================================================
// Function name: DoSaveDocument
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 16:20:00
// =========================================================================
void CConfiguratorDoc::DoSaveDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	const CString strPath = GetPathName();
	if( strPath.IsEmpty())  // has not been saved     
	{
		SaveAsDocument();
	}
	else  // has been saved
	{
        if( IsModified())             // modified
		{
            SaveDocument( strPath );  // save
		}
	}
}

// =========================================================================
// Function name: SaveAsDocument
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 17:10:00
// =========================================================================
BOOL CConfiguratorDoc::SaveAsDocument()
{
	std::auto_ptr<CFileDialog> dlg( new CFileDialog( false ));
	if( dlg.get() == NULL )
	{
		return FALSE;
	}

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
		CString strFilePath = dlg->GetPathName();
		return SaveDocument( strFilePath );
	}

	return true;
}

// =========================================================================
// Function name: SaveDocument
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 17:10:00
// =========================================================================
BOOL CConfiguratorDoc::SaveDocument( LPCTSTR lpszPathName )
{
	try
	{
		wofstream outfile( lpszPathName );   // == open the file
		if( !outfile )  // Open failed
		{
			return FALSE;
		}
	
		if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
		{
			CString str( "Writing document: ");
			str += lpszPathName;
			theApp.LogInfo( str );
		}

		outfile << *this;

		// Reset all dirty local flags and document modified flag
		m_aAliases->SetDirtyLocal( false );        
		SetModifiedFlag( false );

		// Set path name, and add the saved document to the MRU
		SetPathName( lpszPathName, TRUE );

		// Update all views
		UpdateAllViews( NULL, HINT_REFRESH, NULL );
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		return FALSE;
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CConfiguratorDoc::SaveDocument." );

		return FALSE;
	}
	
	return TRUE;
}

// =========================================================================
/// File Open
// =========================================================================
// Function name: OnOpenDocument
// Description  : Override CDocument::OnOpenDocument
//                1. Check if file exists: not exist, return False;
//                2. Clear old document contents before open a new file
//                3. Open the file
//                4. Update m_uProductType and m_nAppType with file information
//                5. Open the corresponding xml file
//                6. Open file >> document
//                7. Clear AliasFileValue
//                8. Reset flags
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.23 16:20:00
// =========================================================================
BOOL CConfiguratorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CWaitCursor wait;

	// 检查是否与当前产品类型一致，如果一致，继续正常打开文档；
	//                           否则，给出确认信息，由用户进行选择。
	CString strFileName( lpszPathName );
	int index = strFileName.ReverseFind( '.' );
	if( index != -1 )
	{
		CString strName( strFileName.Right( strFileName.GetLength() - index - 1 ));   // Product Name

		// Check product type and xml file version
		UINT uProductType = theApp.GetProductTypeByName( strName );
		float fversion = GetOpenFileVersion( lpszPathName );
		int result = theApp.CanMatch( uProductType, fversion );
		if( result != 0 )  // Configurator can't open this file
		{
			CString str;
			if( CConfiguratorApp::PRODUCT_MISMATCH == result )  // product type mismatch
			{
				str.Format( IDS_PRODUCT_MISMATCH, strName );
			}
			else               // xml file mismatch
			{
				str.Format( IDS_XMLFILE_MISMATCH, fversion );
			}

			AfxMessageBox( str, MB_OK | MB_ICONERROR );
			return FALSE;
		}
		else // Configurator can open this file
		{
			// Check if product type same
			if( strName.CompareNoCase( theApp.GetProductName()) != 0 )
			{
				// 提示用户产品类型不一致，由用户选择是否切换产品类型并打开此文档
				CString strConfirm;
				strConfirm.Format( IDS_PRODUCT_DIFFER, strFileName, theApp.GetProductName(), strName );
				if( AfxMessageBox( strConfirm, MB_YESNO | MB_ICONQUESTION ) != IDYES )
				{
					return FALSE;
				}

				// 设置切换产品类型标志位：CConfiguratorApp::m_bResetViewFlag
				theApp.SetXMLVersion( fversion );
				theApp.SetCurrentCode( theApp.GetNewestCodeFromXML( uProductType, fversion ));
				theApp.SwitchProductType( theApp.GetProductTypeByName( strName ), true );

				// 记录信息
				if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
				{
					CString str;
					str.Format( L"Switch product type to: %s when open the file: %s.", theApp.GetProductName(), lpszPathName );
					theApp.LogInfo( L"" );
				}
			}
			else // Check if xml file version same
			{
				if( fabs(fversion-theApp.GetXMLVersion()) > EPSILON_FLT )   //if( fversion != theApp.GetXMLVersion())
				{
					theApp.SetResetViewFlag( true );
					theApp.SetXMLVersion( fversion );
					theApp.SetCurrentCode( theApp.GetNewestCodeFromXML( uProductType, fversion ));
				}
			}
		}
	}// if( index != -1 )

	// Log Open Operation 
	if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
	{
	    CString strOpen;
		strOpen.Format( L"Opening document: %s", lpszPathName );
		theApp.LogInfo( strOpen );
	}

	// 0. When multiple product, check file ext, if not correct, tell user and return false
	// 1. Check if file exists
	CFileException* pfe = new CFileException;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, pfe);
	if (pFile == NULL)
	{
		TRY
		{
			ReportSaveLoadException(lpszPathName, pfe,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY

		DELETE_EXCEPTION(pfe);
		return FALSE;
	}

	ReleaseFile(pFile, FALSE);
	DELETE_EXCEPTION(pfe);

	// 2. Clear old document contents before open a new file
	DeleteContents();  
	SetModifiedFlag();  // dirty during de-serialize
	
	TRY
	{
		// 3. Open the file
	    if( !OpenDocFile( lpszPathName ))   
	    {
		    DeleteContents();                           
		    return FALSE;
	    }

	    // 4. Update m_uProductType and m_nAppType with file information
	    if( !UpdateProductInfo( lpszPathName ))
	    {
            DeleteContents();                           
		    return FALSE;
	    }

	    // 5. Open the corresponding xml file
	    if( !DoFileNew())
	    {
		    DeleteContents();                            
		    return FALSE;
	    }

	    // 6. Open file >> document
	    *(m_aAliasFileValue.get()) >> m_aAliases;

		// 7. Clear AliasFileValue
	    m_aAliasFileValue->clear();

		// 8. Set Visible
	    SetVisible();

		// 9. Compare File
	    if((!m_strCmpFilePathName.IsEmpty()) && (theApp.GetCmpType() == CConfiguratorApp::CMP_FILE))
	    {
			if( ::PathFileExists( m_strCmpFilePathName )) 
			{
		        CompareFile( m_strCmpFilePathName );
			}
			else
			{
				CString strMsg;
				strMsg.Format( IDS_CMPFILE_ERROR, m_strCmpFilePathName );
				AfxMessageBox( strMsg );

				m_strCmpFilePathName.Empty();
			}
		}// if

		// Access level: Enter password
		if( !global::Security.CanTune())  // None or ReadOnly
		{
			std::auto_ptr<CPasswordDlg> dlg( new CPasswordDlg());
			dlg->DoModal();
		}

		// Reset m_bResetViewFlag flag
		if( theApp.GetResetViewFlag())
		{
			UpdateAllViews( NULL, HINT_REFRESH, NULL );
			theApp.SetResetViewFlag( false );
		}

		// TS350: Update Title of MainFrame, Add Local Application Type
		if((CConfiguratorApp::TS350 == theApp.GetProductType()) &&
		   (AfxGetMainWnd()->GetSafeHwnd()))
		{
			((CMainFrame*)AfxGetMainWnd())->UpdateTitle( CMainFrame::TITLE_LOCAL );
		}
	}
	CATCH_ALL(e)
	{
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	// 8. Reset all dirty local/remote flags and document modified flag
	m_aAliases->SetDirtyLocal( false );  
	m_aAliases->SetDirtyRemote( false );
    SetModifiedFlag( FALSE );

	return TRUE;
}

// =======================================================================
// Function name  : CompareFile
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2011.04.13 18:30:00
// =======================================================================
BOOL CConfiguratorDoc::CompareFile( LPCTSTR lpszPathName, bool bDlg )
{
	if( !::PathFileExists( lpszPathName ))
	{
		return FALSE;
	}

	try
	{
		if( bDlg )
		{
			float fversion = GetOpenFileVersion( lpszPathName );
			ASSERT( fversion > 0.0 );
			if( fabs(fversion-theApp.GetXMLVersion()) > EPSILON_FLT )        //if( fversion != theApp.GetXMLVersion())
			{
				CString str;
				str.Format( IDS_XMLFILE_DIFFER, fversion, theApp.GetXMLVersion());
				AfxMessageBox( str, MB_OK | MB_ICONERROR );
				return FALSE;
			}
		}

		//1. Only Read Variables in the file
        if( !OpenDocFile( lpszPathName, true ))   
		{   
			m_strCmpFilePathName.Empty();
		    return FALSE;
	    }

		m_strCmpFilePathName = lpszPathName;

		//2. Initialize m_avCompareFile
	    if( CheckWritePtr( m_aAliases, sizeof(CAliasMap)))
		{
			CAliasFileValueMap::const_iterator it = m_aAliasFileValue->begin();
			CAliasFileValueMap::const_iterator eit = m_aAliasFileValue->end();

			while( it != eit )
			{
				CAliasItem* pAlias = m_aAliases->find_name( it->first );
				if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
				{
					const CAliasFileValue& value = it->second;
					pAlias->SetValue( value.GetValueStr(), true );     // Set CAliasItem::m_avCompareFile with file value
				}
				else
				{
					CString strErr;
					strErr.Format( L"CConfiguratorDoc::CompareFile: DataDictionary missing variable: %s", it->first );
					theApp.LogInfo( strErr );
				}

				it++;
			}//while
		}//if

		//3. Clear AliasFileValue
	    m_aAliasFileValue->clear();

		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
}

// =======================================================================
// Function name  : GetOpenFileVersion
// Description    : Get xml file version 
// Return type    : version of the to be opened file;
//                  0.0: failed
// Parameters     : 
// Author         : WangDong 2012.08.08 09:20:00
// =======================================================================
float CConfiguratorDoc::GetOpenFileVersion( LPCTSTR lpszPathName )
{
	float result = 0.0f;

	while( 1 )
	{
		HRESULT hr = CoInitialize( NULL );
		if( FAILED(hr))
		{
			break;
		}

		MSXML2::IXMLDOMDocumentPtr pXMLDoc;
		hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0");
		if( FAILED(hr))
		{
			break;
		}

		hr = pXMLDoc->put_async( VARIANT_FALSE ); // 异步为假，保证XML解析器暂停执行，直到XML文件加载完成
		if( FAILED(hr))
		{
			break;
		}

		// Load document xml file
		_variant_t _varXMLFile( lpszPathName );
		if( pXMLDoc->load( _varXMLFile ) != VARIANT_TRUE )
		{ 
			break;
		}
	
		// Get Root Element
		MSXML2::IXMLDOMElementPtr pXMLRootElement( pXMLDoc->documentElement);
		if( pXMLRootElement == NULL )
		{
			break;
		}

		// Get Root Element: Product Attributes
		MSXML2::IXMLDOMNamedNodeMapPtr pAttributes( pXMLRootElement->Getattributes());
		if( pAttributes )
		{
			// Version
			MSXML2::IXMLDOMNodePtr pAttributeNode( pAttributes->getNamedItem( "Version" ));
			if( pAttributeNode )
			{
				CString strVersion( (LPCTSTR)pAttributeNode->Gettext());
				::swscanf_s( strVersion, L"%f", &result );
			}
		}

		break;
	}//while

    CoUninitialize();
	return result;
}

// =======================================================================
// Function name  : OpenDocFile
// Description    : 
// Return type    : true: If success;
//                  false: Failed
// Parameters     : 
// Author         : WangDong 2011.03.24 11:20:00
// =======================================================================
bool CConfiguratorDoc::OpenDocFile( LPCTSTR lpszPathName, bool bCompareFile )
{
	bool bResult = true;
	CString strError;

	try
	{
		while( 1 )
		{
			HRESULT hr = CoInitialize( NULL );
			if( FAILED( hr ))
			{
				strError.Format( L"OpenDocFile-CoInitialize failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			MSXML2::IXMLDOMDocumentPtr pXMLDoc;
			hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0");
			if( FAILED(hr))
			{
				strError.Format( L"OpenDocFile-CreateInstance failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			hr = pXMLDoc->put_async( VARIANT_FALSE );  // 异步为假，保证XML解析器暂停执行，直到XML文件加载完成
			if( FAILED(hr))
			{
				strError.Format( L"OpenDocFile-put_async failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			// Load document xml file
			_variant_t _varXMLFile( lpszPathName );
			if( pXMLDoc->load( _varXMLFile ) != VARIANT_TRUE )
			{
				strError.Format( L"Unable to Load %s!", theApp.GetXMLFileName());
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			// Root Element: DataDictionary
			MSXML2::IXMLDOMElementPtr pXMLRootElement( pXMLDoc->documentElement);  
			if( !pXMLRootElement )
			{
				strError = L"DataDictionary Element is NULL!";
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			// Get Root Element: Product Attributes
			MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;    // Attribute child node map
			MSXML2::IXMLDOMNodePtr pAttributeNode;         // Attribute child node
			if( !bCompareFile )
			{
				pAttributes = pXMLRootElement->Getattributes();
				if( pAttributes )
				{
					// Name: Product Name
					pAttributeNode = pAttributes->getNamedItem( "Name" );
					if( pAttributeNode )
					{
						m_aProperties->Add( L"Product.Name", pAttributeNode->Gettext());
					}

					// Version: Product Version
					pAttributeNode = pAttributes->getNamedItem( "Version" );
					if( pAttributeNode )
					{
						m_aProperties->Add( L"Product.Version", pAttributeNode->Gettext());
					}
				}
			}//if( !bCompareFile )

			// Get first level child nodes 
			MSXML2::IXMLDOMNodeListPtr pXMLChildNodes;
			hr = pXMLRootElement->get_childNodes( &pXMLChildNodes );
			if( FAILED(hr))
			{
				strError.Format( L"OpenDocFile-get_childNodes failed:%d: %s!", hr, HrGetMessage(hr));
				theApp.LogInfo( strError );
				bResult = false;
				break;
			}

			// Loop the first level child node 
			MSXML2::IXMLDOMNodePtr pXMLCurrentNode;          // Current child node
			long lChildCounts = pXMLChildNodes->Getlength(); // Counts of child node
			ASSERT( lChildCounts >= 1 );
			for( long i = 0; i<lChildCounts; i++ )
			{
				pXMLCurrentNode = pXMLChildNodes->Getitem( i ); 
				if( pXMLCurrentNode )
				{
					int nNodeType = pXMLCurrentNode->nodeType;
					if( MSXML2::NODE_ELEMENT == nNodeType )    // node type：Element
					{
						_bstr_t _bstrName = pXMLCurrentNode->GetnodeName();
						CString strNodeName( (LPCTSTR)_bstrName );
						if( !bCompareFile )
						{
							// Dictionary: AppType, Name
							if( strNodeName.Compare( L"Dictionary") == 0 )
							{
								pAttributes = pXMLCurrentNode->Getattributes();
								if( pAttributes )
								{
									// AppType: Dictionary AppType
									pAttributeNode = pAttributes->getNamedItem( "AppType" );
									if( pAttributeNode )
									{
										m_aProperties->Add( L"Dictionary.AppType", pAttributeNode->Gettext());
									}

									// Name: Dictionary Name
									pAttributeNode = pAttributes->getNamedItem( "Name" );
									if( pAttributeNode )
									{
										m_aProperties->Add( L"Dictionary.Name", pAttributeNode->Gettext());
									}
								}
							}
							/*else if( strNodeName.Compare( L"FileVersion") == 0 )    //  Currently no using
							{
							}*/
							else if( strNodeName.Compare( L"Trend") == 0 )
							{
								ReadTraceElement( pXMLCurrentNode );
							}
							else if( strNodeName.Compare( L"Variables") == 0 )
							{
								ReadAliasElement( pXMLCurrentNode, bCompareFile );
							}
						}//if( !bCompareFile )
						else // CompareFile: 
						{
							// Check Application Type if TS350
							if( CConfiguratorApp::TS350 == theApp.GetProductType() )
							{
								if( strNodeName.Compare( L"Dictionary") == 0 )
								{
									pAttributes = pXMLCurrentNode->Getattributes();
									if( pAttributes )
									{
										// AppType: Dictionary AppType
										pAttributeNode = pAttributes->getNamedItem( "AppType" );
										if( pAttributeNode )
										{
											CString s = pAttributeNode->Gettext();
											int nAppType = -1;
											::swscanf_s( s, L"%d", &nAppType );
											if( nAppType != theApp.GetAppType())
											{
												strError.Format( IDS_FILECMP_ARRTYPE_ERR, lpszPathName );
												MessageBox( theApp.m_pMainWnd->GetSafeHwnd(), strError, L"", MB_OK|MB_ICONERROR );
												bResult = false;
												break;
											}
										}
									}
								}// Dictionary
							}// TS350

							// Read Variables
							if( strNodeName.Compare( L"Variables") == 0 )
							{
								ReadAliasElement( pXMLCurrentNode, bCompareFile );
							}
						}//else
					}
					else
					{
						continue;
					}
				}
			}// for

			break;
		}//while

		if( !bResult )
		{
			MessageBox( theApp.m_pMainWnd->GetSafeHwnd(), strError, L"", MB_OK|MB_ICONERROR );
		}
	}//try
	catch( CException &e )
	{
		e.ReportError();
		bResult = false;
	}
	catch(...)
	{
		strError = L"Unknown exception occured in CConfiguratorDoc::OpenDocFile!";
		theApp.LogInfo( strError );
		MessageBox( theApp.m_pMainWnd->GetSafeHwnd(), strError, L"", MB_OK|MB_ICONERROR );
		bResult = false;
	}

	CoUninitialize();
	return bResult;
}

//-----------------------------------------------------------------------
// Function name  : ReadTraceElement
// Description    : Read Trace elements' value             
// Return type    : void
// Parameters     : pNodePtr: Variables Node
// Author         : WangDong  2010.03.24  13:00:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::ReadTraceElement( MSXML2::IXMLDOMNodePtr pParentNode )
{
	ASSERT( pParentNode && m_aTraces );
	if((!pParentNode) || (!m_aTraces))
	{
		theApp.LogInfo( L"ReadTraceElement: Entry Parameters are invalid." );
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pParentNode->GetchildNodes();
	if( pNodeList == NULL )
	{
		return;
	}

	long lLength = pNodeList->Getlength();                       // Get non-attribute child nodes number
	ASSERT( lLength >= 1 );                                      // Variables should have one non-attribute child node at least
	if( lLength < 1 )
	{
		return;
	}

	// Parse all Variable node
	MSXML2::IXMLDOMNodePtr pChildNode;                  
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;          
	MSXML2::IXMLDOMNodePtr pAttributeNode;
	for( long i=0; i< lLength; i++ )
	{
		pChildNode = pNodeList->Getitem( i );                      // Get non-attribute child node
		if( pChildNode == NULL )
		{
			continue;
		}

		int nNodeType = pChildNode->nodeType;                      // Node Type  
		if( MSXML2::NODE_ELEMENT == nNodeType )                    // Variable node
		{
			pAttributes = pChildNode->Getattributes();             // Attributes map
			ASSERT( pAttributes );
			if( pAttributes == NULL )
			{
				continue;
			}
			
			CCfgTrace trace; 

			// 1. Name
			pAttributeNode = pAttributes->getNamedItem("Name");    
			ASSERT( pAttributeNode );                              
			if( pAttributeNode )
			{
				LPCTSTR lpszName = pAttributeNode->Gettext();
				trace.SetName( lpszName );
			}

			// 2.Width 
			pAttributeNode = pAttributes->getNamedItem( "Width" );
			if( pAttributeNode )
			{
				LPCTSTR lpszWidth = pAttributeNode->Gettext();
			    trace.SetWidth( _wtoi( lpszWidth ));
			}

			// 3. Color 
			pAttributeNode = pAttributes->getNamedItem( "Color" );
			if( pAttributeNode )
			{
				LPCTSTR lpszColor = pAttributeNode->Gettext();
				long value = 0;
				::swscanf_s( lpszColor, L"%x", &value );
			    trace.SetColor( value );
			}

			// 4. Axis 
			pAttributeNode = pAttributes->getNamedItem( "Axis" );
			if( pAttributeNode )
			{
				LPCTSTR lpszAxis = pAttributeNode->Gettext();
				trace.SetAxis( _wtoi( lpszAxis ));
			}

			m_aTraces->push_back( trace );
		}//if nNodeType
	}//for
}

//-----------------------------------------------------------------------
// Function name  : ReadAliasElement
// Description    : Read Alias elements' value             
// Return type    : void
// Parameters     : pNodePtr: Variables Node
// Author         : WangDong  2010.03.24  13:00:00
//-----------------------------------------------------------------------
void CConfiguratorDoc::ReadAliasElement( MSXML2::IXMLDOMNodePtr pParentNode, bool bCompareFile )
{
	ASSERT( pParentNode && m_aAliasFileValue.get());
	if((!pParentNode) || (!m_aAliasFileValue.get()))
	{
		theApp.LogInfo( L"ReadAliasElement: Entry parameters are invalid!" );
		return;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pParentNode->GetchildNodes();
	if( pNodeList == NULL )
	{
        return;
	}

	long lLength = pNodeList->Getlength();                       // Get non-attribute child nodes number
	ASSERT( lLength >= 1 );                                      // Variables should have one non-attribute child node at least
	if( lLength < 1 )
	{
		return;
	}

	// Parse all Variable node
	MSXML2::IXMLDOMNodePtr pChildNode;                  
	MSXML2::IXMLDOMNamedNodeMapPtr pAttributes;          
	MSXML2::IXMLDOMNodePtr pAttributeNode;
	for( long i=0; i< lLength; i++ )
	{
		pChildNode = pNodeList->Getitem( i );                      // Get non-attribute child node
		if( pChildNode == NULL )
		{
			continue;
		}

		int nNodeType = pChildNode->nodeType;                      // Node Type  
		if( MSXML2::NODE_ELEMENT == nNodeType )                    // Variable node
		{
			pAttributes = pChildNode->Getattributes();             // Attributes map
			ASSERT( pAttributes );
			if( pAttributes == NULL )
			{
				continue;
			}

			CAliasFileValue value;
			LPCTSTR lpszName = NULL, lpszValue = NULL;
			bool bRecord = false;

			// 1. Name attribute
			pAttributeNode = pAttributes->getNamedItem("Name");     // Attribute Child Node
			ASSERT( pAttributeNode );                               // Variable node should have Name attribute
			if( pAttributeNode )
			{
				value.SetName( pAttributeNode->Gettext());
			}

			// 2. Value attribute
			pAttributeNode = pAttributes->getNamedItem( "Value" );
			ASSERT( pAttributeNode );                               // Variable node should have Value attribute
			if( pAttributeNode )
			{
				value.SetValue( pAttributeNode->Gettext());
			}

			// 3. Record attribute
			pAttributeNode = pAttributes->getNamedItem( "Record" );
			if( pAttributeNode )
			{
				LPCTSTR lpszRecord = pAttributeNode->Gettext();
				bRecord = (::StrCmpW( lpszRecord, L"True") == 0);
				value.SetRecord( bRecord );
				if( bRecord && (!bCompareFile) && (!m_bRecordModified))
				{
					m_bRecordModified = true;
				}
			}

			m_aAliasFileValue->Add( value );
		}//if nNodeType
	}//for
}

// =======================================================================
// Function name  : SetVisible
// Description    : Set Visible
//                  DSC100: Settings( m_aSettings and m_aSettingsAlias )
//                  TS350:  Settings, Monitor, and Alarm 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2011.04.20 17:00:00
// =======================================================================
void CConfiguratorDoc::SetVisible()
{
	if( theApp.GetProductType() == CConfiguratorApp::TS350 )
	{
        SetSettingsVisible( m_aSettings );                  // m_aSettings 
	}
	else  // DSC100, DSC110
	{
		SetSettingsVisible( m_aSettings );                  // m_aSettings             
		SetSettingsVisible( m_aSettingsAlias, true );       // m_aSettingsAlias   
	}
}

// -----------------------------------------------------------------------
// Function name  : SetSettingsVisible
// Description    : Set Visible
// Return type    : 
// Parameters     : bSettingAlias: false, m_aSettings
//                                 true,  m_aSetting
// Author         : WangDong 2011.04.20 17:00:00
// ------------------------------------------------------------------------
void CConfiguratorDoc::SetSettingsVisible( CSettingVector* pSettings, bool bSettingAlias )
{
	ASSERT( pSettings != NULL );
	if( pSettings == NULL )
	{
		theApp.LogInfo( L"CConfiguratorDoc::SetSettingsVisible: Entry parameter - pSettings is NULL!" );
	}

	// TS350: Set visible according to the local conditions
	BOOL bHPLinear = FALSE, bLPLinear = FALSE;
	UINT uLimType = 0, uIPType = 0,  uBPType = 0, uEPType = 0, uCtrlType = 0; 
	UINT uDRType = 0,  uControl = 0, uIPRLim = 0, uBPRLim = 0, uLM1_HL = 0,   uPRType = 0;

	if((theApp.GetProductType() == CConfiguratorApp::TS350) && m_aAliases )
	{
		CAliasItem *pAlias = m_aAliases->find_name( L"gHPLINEAR" );   // 2141, Valve1 Linearization Enable
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			bHPLinear = pAlias->valBool();
		}

		pAlias = m_aAliases->find_name( L"gLPLINEAR" );               // 2142, Valve2 Linearization Enable
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			bLPLinear = pAlias->valBool();
		}

		pAlias = m_aAliases->find_name( L"kLIMTYPE" );                // 40252, Load Limit Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uLimType = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kIPTYPE" );                 // 40254, Inlet Pressure Droop Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uIPType = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kBPTYPE" );                 // 40255, Back Pressure Droop Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uBPType = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kEPTYPE" );                 // 40256, Extraction Pressure Droop Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uEPType = pAlias->valI2();
		}

	    pAlias = m_aAliases->find_name( L"kCTRLTYP" );                // 40354, Turbine Control Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uCtrlType = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kDRTYPE" );                 // 40355, Speed Droop Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uDRType = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kCONTROL" );                // 40356, Primary Control Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uControl = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kIPRLIM" );                 // 40478, Inlet Pressure Low Limiting
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uIPRLim = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kBPRLIM" );                 // 40479, Back Pressure Hi/Low Limiting
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uBPRLim = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kLM1_HL" );                 // 40513, Back Pressure Hi/Low Limiting
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uLM1_HL = pAlias->valI2();
		}

		pAlias = m_aAliases->find_name( L"kPRTYPE" );                 // 40514, Process Droop Type
		if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		{
			uPRType = pAlias->valI2();
		}
	}// if TS350

	if( CheckWritePtr( pSettings, sizeof( CSettingVector )))  
	{
		CSettingVector::iterator it = pSettings->begin();
		CSettingVector::iterator eit = pSettings->end();

		while( it != eit )
		{
            CSettingItem* pSettingItem = *it;
			ASSERT( pSettingItem );
			if( pSettingItem == NULL )
			{
				theApp.LogInfo( L"CConfiguratorDoc::SetSettingsVisible: pSettingItem is NULL!" );

				++it;
				continue;
			}

			if((theApp.GetProductType() == CConfiguratorApp::DSC100) ||
			   (theApp.GetProductType() == CConfiguratorApp::DSC110))
			{
				// MAC Address
				bool bVisible = false;
			    CString strVariable = pSettingItem->GetVariableName();
				if( strVariable.CompareNoCase( L"strMAC" ) == 0 )
				{
					bVisible = global::Security.CanAdmin();
					pSettingItem->SetVisible( bVisible );
				}

                if( bSettingAlias )
				{
					CString strName( pSettingItem->GetName()), strAliases;
					strAliases.LoadStringW( IDS_MAC_INT );

					// int Alias of MAC: Hide
					if((strVariable.CompareNoCase( L"uMAC01" ) == 0)          ||
					   (strVariable.CompareNoCase( L"uMAC02" ) == 0)          ||
					   (strVariable.CompareNoCase( L"uMAC03" ) == 0)          ||
					   (strVariable.CompareNoCase( L"uMAC04" ) == 0)          ||
					   (strVariable.CompareNoCase( L"uMAC05" ) == 0)          ||
					   (strVariable.CompareNoCase( L"uMAC06" ) == 0)          ||
					   (strName.CompareNoCase( strAliases ) == 0))
					{
						pSettingItem->SetVisible( false );
					}

					// String Alias of MAC: Check access level
					strAliases.LoadStringW( IDS_MAC_STRING );
					if( strName.CompareNoCase( strAliases ) == 0 )
					{
                        bVisible = global::Security.CanAdmin();
					    pSettingItem->SetVisible( bVisible );
					}
				}//if( bSettingAlias )
			}// if DSC100
			else if( theApp.GetProductType() == CConfiguratorApp::TS350 )
			{
				bool bVisible = true;
				CString strName( pSettingItem->GetName());
				CString strVariable( pSettingItem->GetVariableName());

				/// Set visible according to the local conditions
				if((strVariable.CompareNoCase(L"kEA") == 0)      ||    // Extraction Header Type, 
				   (strVariable.CompareNoCase(L"kHR") == 0)      ||    // Horse Power Ratio, V2/V1
				   (strVariable.CompareNoCase(L"kFR") == 0)      ||    // Flow Ratio, V2/V1
				   (strVariable.CompareNoCase(L"kTB") == 0)      ||    // Turbine Bias
				   (strVariable.CompareNoCase(L"kV2MIN") == 0)   ||    // Minimum V2    
				   (strVariable.CompareNoCase(L"kV2MINcl") == 0) ||    // Minimum V2 Clear Position
				   (strVariable.CompareNoCase(L"kV2MAXcl") == 0) ||    // Maximum V2 Clear Position
				   (strVariable.CompareNoCase(L"kV2MAX") == 0)   ||    // Maximum V2
				   (strVariable.CompareNoCase(L"kV2SPRR") == 0)  ||    // V2 Startup Ramp Rate
				   (strVariable.CompareNoCase(L"kV2SRR") == 0))        // V2 Ramp Rate    
				{
					//1.1 kCTRLTYP > 0 时显示
                    bVisible = (uCtrlType > 0);
				}
				else if((strVariable.CompareNoCase(L"kP1") == 0)                        ||   // Priority (Two Valve Interactive)
				        (strVariable.CompareNoCase(L"kP2") == 0)                        ||   // Priority (Two Valve Decouple)
				        (strName.CompareNoCase(L"Extraction/Induction Pressure") == 0 ) ||   // Extraction/Induction Pressure
					    (strVariable.CompareNoCase(L"kEXTENAB") == 0)                   ||   // Startup
						(strVariable.CompareNoCase(L"kEXPRRM") == 0)                    ||   // Ramp Rate 
				        (strVariable.CompareNoCase(L"kEXPRRL") == 0)                    ||   // Raise/Lower Ramp Rate
						(strVariable.CompareNoCase(L"kEXPLOCL") == 0)                   ||   // Minimum Setpoint
				        (strVariable.CompareNoCase(L"kEXPHICL") == 0)                   ||   // Maximum Setpoint
				        (strVariable.CompareNoCase(L"kMINEXT") == 0)                    ||   // Output low clamp 
						(strVariable.CompareNoCase(L"kMAXEXT") == 0)                    ||   // Output high clamp 
				        (strVariable.CompareNoCase(L"kEXSNAP") == 0)                    ||   // Snapback Delay
						(strVariable.CompareNoCase(L"EP2_Bit2") == 0)                   ||   // Snapback Delay
				        (strVariable.CompareNoCase(L"EP2_Bit3") == 0)                   ||   // "Raise" snaps from Minimum 
				        (strVariable.CompareNoCase(L"EP2_Bit4") == 0))                       // "Lower" snaps from Maximum
				{
					//1.2 kCTRLTYP == 1 时显示
					bVisible = (1== uCtrlType);
				}
				else if((strVariable.CompareNoCase(L"kP3") == 0))       // Priority (Two Valve Speed/Inlet
				{
					//1.3 kCTRLTYP == 2 时显示
					bVisible = (2 == uCtrlType);
				}
				else if((strName.CompareNoCase((theApp.GetAppType() == APPTYPE_GEN) ? L"MW PID" : L"Process PID" ) == 0) &&   
				        (pSettingItem->GetParent() != NULL)                                                              &&
				        (pSettingItem->GetParent()->GetName().CompareNoCase(L"Cascaded Control") == 0))
				{
					//2. kCONTROL: Gen, 0-3; Mech, 0, 4-6
					if( theApp.GetAppType() == APPTYPE_GEN )
					{
                        bVisible = (3 == uControl);
					}
					else
					{
						bVisible = (uControl > 3);
					}
				}
				else if((strVariable.CompareNoCase(L"kProcRmp") == 0)  ||  // Ramp Rate 
				        (strVariable.CompareNoCase(L"kProcRL") == 0)   ||  // Raise/Lower Ramp Rate
				        (strVariable.CompareNoCase(L"kProcLoCl") == 0) ||  // Minimum Setpoint
				        (strVariable.CompareNoCase(L"kProcHiCl") == 0) ||  // Maximum Setpoint
				        (strVariable.CompareNoCase(L"PR2_Bit2") == 0)  ||  // Enable Snapback    
				        (strVariable.CompareNoCase(L"kPrcSnap") == 0)  ||  // Snapback Delay
				        (strVariable.CompareNoCase(L"PR2_Bit3") == 0)  ||  // "Raise" snaps from Minimum 
				        (strVariable.CompareNoCase(L"PR2_Bit4") == 0))     // "Lower" snaps from Maximum
				              
				{
					//2.2 kCONTROL > 2 时显示
                    bVisible = (uControl > 2);
				}
				else if((strVariable.CompareNoCase(L"kMWOPN") == 0) &&
					    (theApp.GetAppType() == APPTYPE_GEN))
				{
					//2.3 Open Generator Breaker MW Setpoint: Gen; kCONTROL == 3 
					bVisible = (3 == uControl);
				}
				else if((strName.CompareNoCase( L"Inlet Pressure" ) == 0)                             &&
				        (pSettingItem->GetParent() != NULL)                                           &&
				        (pSettingItem->GetParent()->GetName().CompareNoCase(L"Turbine Control") == 0) &&
						(theApp.GetAppType() == APPTYPE_MECH))
				{
					// Inlet Presure: Mech; Parent: Turbine Control
					bVisible = (2 == uCtrlType);
				}
				else if(((strVariable.CompareNoCase( L"kMINIP" ) == 0) || (strVariable.CompareNoCase( L"kIPSP" ) == 0)) &&   // Assume Control Low Point or Control Setpoint
				        (theApp.GetAppType() == APPTYPE_GEN))  
				{
					// kIPRLIM == 1 时显示
					bVisible = (1 == uIPRLim);
				}
				else if((strVariable.CompareNoCase(L"kINPRRM") == 0)  ||   // Ramp Rate 
				        (strVariable.CompareNoCase(L"kINPRRL") == 0)  ||   // Raise/Lower Ramp Rate
				        (strVariable.CompareNoCase(L"kINPLOCL") == 0) ||   // Minimum Setpoint
				        (strVariable.CompareNoCase(L"kINPHICL") == 0) ||   // Maximum Setpoint
				        (strVariable.CompareNoCase(L"IP2_Bit2") == 0) ||   // Enable Snapback    
				        (strVariable.CompareNoCase(L"kIPSNAP") == 0)  ||   // Snapback Delay
				        (strVariable.CompareNoCase(L"IP2_Bit3") == 0) ||   // "Raise" snaps from Minimum 
				        (strVariable.CompareNoCase(L"IP2_Bit4") == 0))     // "Lower" snaps from Maximum
				              
				{
					// kCTRLTYP==2 or kCONTROL==1 or kIPRLIM==1时显示
					if((2 == uCtrlType) ||
					   (1 == uControl)  ||
				       (1 == uIPRLim))
					{
                        bVisible = true;
					}
					else
					{
						bVisible = false;
					}
				}
				else if( strVariable.CompareNoCase(L"kLIMBPL") == 0 )     // Assume Control Low Point
				{
					// kBPRLIM ==1 or 3时显示
					if((1 == uBPRLim) || (3 == uBPRLim))
					{
                        bVisible = true;
					}
					else
					{
						bVisible = false;
					}
				}
				else if( strVariable.CompareNoCase(L"kLIMBP") == 0 )      // Assume Control High Point
				{
					// kBPRLIM ==2 or 3时显示
					if((2 == uBPRLim) || (3 == uBPRLim))
					{
                        bVisible = true;
					}
					else
					{
						bVisible = false;
					}
				}
				else if( strVariable.CompareNoCase(L"kBPSP") == 0 )       // Control Setpoint
				{
					// kBPRLIM > 0时显示
					bVisible = (uBPRLim > 0);
				}
				else if((strVariable.CompareNoCase(L"kBKPRRM") == 0)  ||   // Ramp Rate 
				        (strVariable.CompareNoCase(L"kBKPRRL") == 0)  ||   // Raise/Lower Ramp Rate
				        (strVariable.CompareNoCase(L"kBKPLOCL") == 0) ||   // Minimum Setpoint
				        (strVariable.CompareNoCase(L"kBKPHICL") == 0) ||   // Maximum Setpoint
				        (strVariable.CompareNoCase(L"BP2_Bit2") == 0) ||   // Enable Snapback    
				        (strVariable.CompareNoCase(L"kBKSNAP") == 0)  ||   // Snapback Delay
				        (strVariable.CompareNoCase(L"BP2_Bit3") == 0) ||   // "Raise" snaps from Minimum 
				        (strVariable.CompareNoCase(L"BP2_Bit4") == 0))     // "Lower" snaps from Maximum
				              
				{
					// kCONTROL == 2  or  kBPRLIM > 0时显示
					if((2 == uControl) || (uBPRLim > 0))
					{
                        bVisible = true;
					}
					else
					{
						bVisible = false;
					}
				}
				else if( strVariable.CompareNoCase( L"kINTLOAD" ) ==0 )    // Internal Load Limit
				{
					// kLIMTYPE == 1 时显示
					bVisible = (1 == uLimType);   
				}
				else if((strVariable.CompareNoCase(L"kLOADLIM") == 0) ||   // Load Limit Setpoint 
				        (strVariable.CompareNoCase(L"kLDLOCL") == 0)  ||   // Low Setpoint Clamp
				        (strVariable.CompareNoCase(L"kLDHICL") == 0)  ||   // High Setpoint Clamp
				        (strVariable.CompareNoCase(L"kLDBIAS") == 0))      // Load Coupling Dead Band
				{
					// kLIMTYPE == 3 时显示
					bVisible = (3 == uLimType);
				}
				else if((strVariable.CompareNoCase(L"kSDROOP") == 0)  ||   // Droop 
				        (strVariable.CompareNoCase(L"kDRPRMP") == 0)  ||   // Droop Ramp Rate
				        (strVariable.CompareNoCase(L"kSDROOPDB") == 0))    // Droop Deadband
				{
					// kDRTYPE > 0时显示
					bVisible = (uDRType > 0);
				}
				else if(((strVariable.CompareNoCase(L"LM1_IN") == 0) || (strVariable.CompareNoCase(L"kLM1_Dir") == 0)) && // Load Limit Setpoint or Low Setpoint Clamp 
				        (theApp.GetAppType() == APPTYPE_MECH)) 
				{
					// Mech, kLM1_HL > 0 时显示
					bVisible = (uLM1_HL > 0);
				}
				else if(((strName.CompareNoCase(L"Process") == 0) &&
				         (pSettingItem->GetParent() != NULL)      &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Droop") == 0)) ||
					    (strVariable.CompareNoCase(L"kPRTYPE") == 0))
				{
					// Mech, 且kCONTROL > 3 时显示
					bVisible = ((theApp.GetAppType() == APPTYPE_MECH) && (uControl > 3));
				}
				else if((strVariable.CompareNoCase( L"kPRDROOP" ) == 0) && (theApp.GetAppType() == APPTYPE_MECH))
				{
					// Mech， kCONTROL > 3 且 kPRTYPE > 0时显示
					bVisible = (((theApp.GetAppType() == APPTYPE_MECH) && (uControl > 3)) && (uPRType > 0));
				}
				else if(((strName.CompareNoCase(L"Inlet Pressure") == 0) &&
				         (pSettingItem->GetParent() != NULL)             &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Droop") == 0)) ||
					    (strVariable.CompareNoCase(L"kIPTYPE") == 0))
				{
					// kCONTROL == 1 or kCTRLTYP == 2时显示
					bVisible = ((1==uControl) || (2==uCtrlType));
				}
				else if( strVariable.CompareNoCase( L"kIDROOP" ) == 0 )
				{
					//  kCONTROL == 1 or kCTRLTYP == 2， 且kIPTYPE > 0时显示
					bVisible = (((1==uControl) || (2==uCtrlType)) && (uIPType > 0));
				}
				else if(((strName.CompareNoCase(L"Back Pressure") == 0) &&
				         (pSettingItem->GetParent() != NULL)             &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Droop") == 0)) ||
					    (strVariable.CompareNoCase(L"kBPTYPE") == 0))
				{
					// Gen 且 kCONTROL == 2时显示
					bVisible = ((theApp.GetAppType() == APPTYPE_GEN) && (2 == uControl));
				}
				else if( strVariable.CompareNoCase( L"kBDROOP" ) == 0 )
				{
					//  Gen, kCONTROL == 2， 且kBPTYPE > 0时显示
					bVisible = (((theApp.GetAppType() == APPTYPE_GEN) && (2 == uControl)) && (uBPType > 0));
				} 
				else if(((strName.CompareNoCase(L"Extraction Pressure") == 0) &&
				         (pSettingItem->GetParent() != NULL)             &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Droop") == 0)) ||
					    (strVariable.CompareNoCase(L"kEPTYPE") == 0))
				{
					// kCTRLTYP == 1时显示
					bVisible = ( 1 == uCtrlType );
				}
				else if( strVariable.CompareNoCase( L"kEDROOP" ) == 0 )
				{
					//  kCTRLTYP == 1 且 kEPTYPE > 0时显示
					bVisible = ((1 == uCtrlType ) && (uEPType > 0));
				}
				else if(((strName.CompareNoCase(L"Speed PID - Droop") == 0) &&
				         (pSettingItem->GetParent() != NULL)                &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
					    (strVariable.CompareNoCase(L"kDRPPB") == 0)                            ||
						(strVariable.CompareNoCase(L"kDRPRES") == 0)                           ||
						(strVariable.CompareNoCase(L"kDRPRATE") == 0))
				{
					// kDRTYPE > 0时显示
					bVisible = ( uDRType > 0 );
				}
				else if(((strName.CompareNoCase((theApp.GetAppType() == APPTYPE_GEN) ? L"MW PID" : L"Process PID" ) == 0) &&   
				         (pSettingItem->GetParent() != NULL)                                                              &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
						(strVariable.CompareNoCase(L"kPrcPB") == 0)                            ||
						(strVariable.CompareNoCase(L"kPrcRes") == 0)                            ||
						(strVariable.CompareNoCase(L"kPrcRate") == 0))
				{
					if( theApp.GetAppType() == APPTYPE_GEN )           // Gen: kCONTROL == 3时显示
					{
                        bVisible = (3 == uControl);
					}
					else                                               // Mech: kCONTROL > 3时显示
					{
						bVisible = (uControl > 3);
					}
				}
				else if(((strName.CompareNoCase(L"Inlet Pressure PID") == 0) &&
				         (pSettingItem->GetParent() != NULL)                 &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
					    (strVariable.CompareNoCase(L"kINPPB") == 0)                            ||
						(strVariable.CompareNoCase(L"kINPRES") == 0)                           ||
						(strVariable.CompareNoCase(L"kINPRATE") == 0))
				{
					// kCONTROL == 1 or kCTRLTYP == 2 or kIPRLIM == 1时显示
					bVisible = ((1 == uControl) || (2==uCtrlType) || (1==uIPRLim));
				}
				else if(((strName.CompareNoCase( L"Back Pressure PID" ) == 0) &&   
				         (pSettingItem->GetParent() != NULL)                  &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
						(strVariable.CompareNoCase(L"kBKPPB") == 0)                            ||
						(strVariable.CompareNoCase(L"kBKPRES") == 0)                           ||
						(strVariable.CompareNoCase(L"kBKPRATE") == 0))
				{
					// Gen, 且kCONTROL == 2 or kBPRLIM > 0 时显示
                    bVisible = ((theApp.GetAppType() == APPTYPE_GEN) && ((2 == uControl) || (uBPRLim > 0)));
				}
				else if(((strName.CompareNoCase(L"Extraction/Induction Pressure PID") == 0) &&
				         (pSettingItem->GetParent() != NULL)                                &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
					    (strVariable.CompareNoCase(L"kEXPPB") == 0)                            ||
						(strVariable.CompareNoCase(L"kEXPRES") == 0)                           ||
						(strVariable.CompareNoCase(L"kEXPRATE") == 0))
				{
					// kCTRLTYP == 1时显示
					bVisible = ( 1 == uCtrlType );
				}
				else if(((strName.CompareNoCase( L"Load Limit PID" ) == 0) &&   
				         (pSettingItem->GetParent() != NULL)                  &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
						(strVariable.CompareNoCase(L"kLDPB") == 0)                             ||
						(strVariable.CompareNoCase(L"kLDRES") == 0)                            ||
						(strVariable.CompareNoCase(L"kLDRATE") == 0))
				{
					// Gen, 且kLIMTYPE == 3时显示
                    bVisible = ((theApp.GetAppType() == APPTYPE_GEN) && ( 3 == uLimType ));
				}
				else if(((strName.CompareNoCase( L"V1 Limit PID" ) == 0) &&   
				         (pSettingItem->GetParent() != NULL)                  &&
				         (pSettingItem->GetParent()->GetName().CompareNoCase(L"Tuning") == 0)) ||
						(strVariable.CompareNoCase(L"kLM1_PB") == 0)                             ||
						(strVariable.CompareNoCase(L"kLM1_Res") == 0)                            ||
						(strVariable.CompareNoCase(L"kLM1_Rate") == 0))
				{
					// Mech, 且kLM1_HL > 0时显示
                    bVisible = ((theApp.GetAppType() == APPTYPE_MECH) && ( uLM1_HL > 0 ));
				}
				else if((strVariable.CompareNoCase(L"kHP_X1") == 0)  ||   // X1 value 
				        (strVariable.CompareNoCase(L"kHP_X2") == 0)  ||   // X2 value
				        (strVariable.CompareNoCase(L"kHP_X3") == 0)  ||   // X3 value
				        (strVariable.CompareNoCase(L"kHP_X4") == 0)  ||   // X4 value
				        (strVariable.CompareNoCase(L"kHP_X5") == 0)  ||   // X5 value    
				        (strVariable.CompareNoCase(L"kHP_X6") == 0)  ||   // X6 value
				        (strVariable.CompareNoCase(L"kHP_X7") == 0)  ||   // X7 value
				        (strVariable.CompareNoCase(L"kHP_X8") == 0)  ||   // X8 value
						(strVariable.CompareNoCase(L"kHP_X9") == 0)  ||   // X9 value
						(strVariable.CompareNoCase(L"kHP_X10") == 0) ||   // X10 value
						(strVariable.CompareNoCase(L"kHP_X11") == 0) ||   // X11 value
						(strVariable.CompareNoCase(L"kHP_Y1") == 0)  ||   // Y1 value
						(strVariable.CompareNoCase(L"kHP_Y2") == 0)  ||   // Y2 value
						(strVariable.CompareNoCase(L"kHP_Y3") == 0)  ||   // Y3 value
						(strVariable.CompareNoCase(L"kHP_Y4") == 0)  ||   // Y4 value
						(strVariable.CompareNoCase(L"kHP_Y5") == 0)  ||   // Y5 value
						(strVariable.CompareNoCase(L"kHP_Y6") == 0)  ||   // Y6 value
						(strVariable.CompareNoCase(L"kHP_Y7") == 0)  ||   // Y7 value
						(strVariable.CompareNoCase(L"kHP_Y8") == 0)  ||   // Y8 value
						(strVariable.CompareNoCase(L"kHP_Y9") == 0)  ||   // Y9 value
						(strVariable.CompareNoCase(L"kHP_Y10") == 0) ||   // Y10 value
						(strVariable.CompareNoCase(L"kHP_Y11") == 0))     // Y11 value
				              
				{
					// gHPLINEAR == 1时显示
					bVisible = (bHPLinear == TRUE); 
				}
				else if((strVariable.CompareNoCase(L"kLP_X1") == 0)  ||   // X1 value 
				        (strVariable.CompareNoCase(L"kLP_X2") == 0)  ||   // X2 value
				        (strVariable.CompareNoCase(L"kLP_X3") == 0)  ||   // X3 value
				        (strVariable.CompareNoCase(L"kLP_X4") == 0)  ||   // X4 value
				        (strVariable.CompareNoCase(L"kLP_X5") == 0)  ||   // X5 value    
				        (strVariable.CompareNoCase(L"kLP_X6") == 0)  ||   // X6 value
				        (strVariable.CompareNoCase(L"kLP_X7") == 0)  ||   // X7 value
				        (strVariable.CompareNoCase(L"kLP_X8") == 0)  ||   // X8 value
						(strVariable.CompareNoCase(L"kLP_X9") == 0)  ||   // X9 value
						(strVariable.CompareNoCase(L"kLP_X10") == 0) ||   // X10 value
						(strVariable.CompareNoCase(L"kLP_X11") == 0) ||   // X99 value
						(strVariable.CompareNoCase(L"kLP_Y1") == 0)  ||   // Y1 value
						(strVariable.CompareNoCase(L"kLP_Y2") == 0)  ||   // Y2 value
						(strVariable.CompareNoCase(L"kLP_Y3") == 0)  ||   // Y3 value
						(strVariable.CompareNoCase(L"kLP_Y4") == 0)  ||   // Y4 value
						(strVariable.CompareNoCase(L"kLP_Y5") == 0)  ||   // Y5 value
						(strVariable.CompareNoCase(L"kLP_Y6") == 0)  ||   // Y6 value
						(strVariable.CompareNoCase(L"kLP_Y7") == 0)  ||   // Y7 value
						(strVariable.CompareNoCase(L"kLP_Y8") == 0)  ||   // Y8 value
						(strVariable.CompareNoCase(L"kLP_Y9") == 0)  ||   // Y9 value
						(strVariable.CompareNoCase(L"kLP_Y10") == 0) ||   // Y10 value
						(strVariable.CompareNoCase(L"kLP_Y11") == 0))     // Y11 value           
				{
					// gLPLINEAR == 1时显示
					bVisible = (bLPLinear == TRUE); 
				}
			
				pSettingItem->SetVisible( bVisible );

				/// Gen: Inlet Pressure
				if( theApp.GetAppType() == APPTYPE_GEN )
				{
					if((strName.CompareNoCase( L"Inlet Pressure" ) == 0) &&
					   (pSettingItem->GetParent() != NULL)               &&
					   (pSettingItem->GetParent()->GetName().CompareNoCase(L"Turbine Control") == 0))
					{
						pSettingItem->SetVisible( false );
					}
				}
			}// TS350

			// Recurse
			if( pSettingItem->GetChildVector())
			{
                SetSettingsVisible( pSettingItem->GetChildVector(), bSettingAlias );
			}

			it++;
		}//while
	}//if( CheckWritePtr( pSettings, sizeof( CSettingVector )))  
}

/************************************************************************/
// History Datas
/************************************************************************/
// =======================================================================
// Function name  : LoadHisFile
// Description    : Load history file user selected
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.18 10:00:00
// =======================================================================
bool CConfiguratorDoc::LoadHisFile( CString strHisFileName, CHistoryFileDatas *aHisFileDatas )
{
	ASSERT((E_NONE == ValidateHisFile( strHisFileName )) && (aHisFileDatas != NULL ));
	if((ValidateHisFile( strHisFileName ) != E_NONE) || (NULL == aHisFileDatas))
	{
		return false;
	}

	bool bResult = true;
	CString strError;
	while( 1 )
	{
		HRESULT hr = CoInitialize( NULL );
		if( FAILED( hr ))
		{
			strError.Format( L"LoadHisFile-CoInitialize failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		MSXML2::IXMLDOMDocumentPtr pXMLDoc;
		hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0");
		if( FAILED(hr))
		{
			strError.Format( L"LoadHisFile-CreateInstance failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		hr = pXMLDoc->put_async( VARIANT_FALSE );  // 异步为假，保证XML解析器暂停执行，直到XML文件加载完成
		if( FAILED(hr))
		{
			strError.Format( L"LoadHisFile-put_async failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		if( WAIT_OBJECT_0 == WaitHisFileFinish())
		{
			ResetHisFileFinishEvent();
			if( pXMLDoc->load( _variant_t(strHisFileName)) != VARIANT_TRUE )   // Load xml file
			{
				SetHisFileFinishEvent();

				strError.Format( L"LoadHisFile:Unable to Load %s", strHisFileName );
				theApp.LogInfo( strError  );
				bResult = false;
				break;
			}

			SetHisFileFinishEvent();
		}
		else
		{
			strError = L"LoadHisFile:WaitHisFileFinish Timeout!";
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}
	    
		// Root Element: "chart"
		MSXML2::IXMLDOMElementPtr pXMLRootElement( pXMLDoc->documentElement );
		if( !pXMLRootElement )
		{
			strError = L"LoadHisFile:Root Element - chart is NULL!";
			theApp.LogInfo( strError  );
			bResult = false;
			break;
		}

		MSXML2::IXMLDOMNodeListPtr pXMLChildNodes;
		hr = pXMLRootElement->get_childNodes( &pXMLChildNodes );  //The first level child nodes: "series"
		if( FAILED(hr))
		{
			strError.Format( L"LoadHisFile-get_childNodes failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		// Loop the first level child nodes
		MSXML2::IXMLDOMNodePtr pXMLCurrentNode;
		for( long i = 0; i< pXMLChildNodes->Getlength(); i++ )
		{
			pXMLCurrentNode = pXMLChildNodes->Getitem(i);
			if( pXMLCurrentNode && (MSXML2::NODE_ELEMENT == pXMLCurrentNode->nodeType))  // Node type：Element
			{             
				MSXML2::IXMLDOMNamedNodeMapPtr pAttributes( pXMLCurrentNode->Getattributes());

				CString strTagname, strDesc;
				int nAlias = 0;

				// 1. Alias Attribute
				MSXML2::IXMLDOMNodePtr pAttributeNode( pAttributes->getNamedItem("Alias"));
				ASSERT( pAttributeNode );
				if( pAttributeNode )
				{
					::swscanf_s( (LPCTSTR)pAttributeNode->Gettext(), L"%d", &nAlias );
					ASSERT( nAlias >0 );
					if( aHisFileDatas->find_num( nAlias ) != NULL )    // already exists
					{
						continue;
					} 
				}

				// 2. title Attribute: Tagname
				pAttributeNode = pAttributes->getNamedItem("title");
				ASSERT( pAttributeNode );
				if( pAttributeNode )
				{
					LPCTSTR pcStrName = pAttributeNode->Gettext();
					strTagname = pcStrName;
				}

				// 3. Desc Property
				pAttributeNode = pAttributes->getNamedItem("Desc");
				if( pAttributeNode )
				{
					strDesc = (LPCTSTR)pAttributeNode->Gettext();
				}

				aHisFileDatas->Add( nAlias, new CHistoryFileData( strTagname, strDesc, nAlias ));
			}
		}//for

		break;
	}//while

	CoUninitialize();
	return bResult;
}

// =======================================================================
// Function name  : ResetHistory
// Description    : switch product type or apptype
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.27 15:00:00
// =======================================================================
void CConfiguratorDoc::ResetHistory()
{
	//Reset HistoryCtrl
	m_pRecordCtrl->Reset();
	m_pViewCtrl->Reset();

	// ResetHistoryDatas
	m_aHisRecordDatas->Clear();
	m_aHisFileDatas->Clear();
	m_aHisViewDatas->Reset();

	// Clear view display
	UpdateHistoryView( CConfiguratorDoc::HIS_CLEAR );
}

// =======================================================================
// Function name  : CreateHistoryFile
// Description    : Create xml file and then add child items and properties
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.21 15:00:00
// =======================================================================
bool CConfiguratorDoc::CreateHistoryFile( CString strFilePathName )
{
	ASSERT( !strFilePathName.IsEmpty());

	bool bResult = true;
	CString strError;
	HRESULT hr = CoInitialize( NULL );
	if( FAILED( hr ))
	{
		strError.Format( L"CreateHistoryFile-CoInitialize failed:%d: %s!", hr, HrGetMessage(hr));
		theApp.LogInfo( strError );
		return false;
	}

	while( 1 )
	{
		// Create DOM Object
		MSXML2::IXMLDOMDocumentPtr pXMLDoc;
		hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0" );   
		if( FAILED(hr))
		{
			strError.Format( L"CreateHistoryFile-CreateInstance failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		// Chart which has attribute "SamplingRate" and "Interval"
		MSXML2::IXMLDOMElementPtr pRootElement( pXMLDoc->createElement( "chart" ));
		if( !pRootElement )
		{
			strError = L"CreateHistoryFile:createElement-chart failed!";
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		pRootElement->setAttribute( "SamplingRate", m_pRecordCtrl->GetSamplingRate());
		pRootElement->setAttribute( "Interval", m_pRecordCtrl->GetInterval());
		pXMLDoc->appendChild( pRootElement );

		// Series which has attribute "title", "Alias" and "Desc"
		MSXML2::IXMLDOMElementPtr pSeriesElement;
		MSXML2::IXMLDOMElementPtr pPointsElement;
		HisDatas& DatasToRecord = m_aHisRecordDatas->GetHisRecordDatas();
		for( size_t i=0; i < DatasToRecord.size(); ++i )
		{
			const CAliasItem* pAliasItem = DatasToRecord[i]->GetAliasItem();  
			ASSERT( pAliasItem != NULL );
			if( pAliasItem != NULL )
			{
				pSeriesElement = pXMLDoc->createElement( "series" );
				if( !pSeriesElement )
				{
					strError = L"CreateHistoryFile:createElement-series failed!";
					theApp.LogInfo( strError );
					bResult = false;
					break;
				}

				pSeriesElement->setAttribute( "title", pAliasItem->GetName());
				pSeriesElement->setAttribute( "Alias",pAliasItem->GetAlias());
				pSeriesElement->setAttribute( "Desc", pAliasItem->GetDescription());
				pPointsElement = pXMLDoc->createElement( "points" );
				if( !pPointsElement )
				{
					strError = L"CreateHistoryFile:createElement-points failed!";
					theApp.LogInfo( strError );
					bResult = false;
					break;
				}

				pSeriesElement->appendChild( pPointsElement );
				pRootElement->appendChild( pSeriesElement );
			}
		}//for

		hr = pXMLDoc->save( (LPCTSTR)strFilePathName);
		if( FAILED( hr ))
		{
			strError.Format( L"CreateHistoryFile-save failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
		}

		break;
	}//while

	CoUninitialize();
	return bResult;
}

// =======================================================================
// Function name  : RecordHistoryFile
// Description    : 查看当前文件时，可能产生读写冲突，所以需要互斥保护
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.21 15:00:00
// =======================================================================
bool CConfiguratorDoc::RecordHistoryFile()
{
	if( !m_aHisRecordDatas->HasLocalDatas())
	{
		return true;
	}

	bool bResult = true;
	CString strError;
	while( 1 )
	{
		HRESULT hr = CoInitialize( NULL );
		if( FAILED( hr ))
		{
			strError.Format( L"RecordHistoryFile-CoInitialize failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		MSXML2::IXMLDOMDocumentPtr pXMLDoc;
		hr = pXMLDoc.CreateInstance( L"MSXML2.DOMDocument.4.0");
		if( FAILED(hr))
		{
			strError.Format( L"RecordHistoryFile-CreateInstance failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		CString strFilePathName( m_pRecordCtrl->GetFilePathName());
		//DWORD dwTime = GetTickCount();
		if( pXMLDoc->load( _variant_t( strFilePathName)) != VARIANT_TRUE )
		{ 
			strError.Format( L"RecordHistoryFile:Unable to Load %s!", strFilePathName );
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		/*if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_OPR )
		{
			strError.Format( L"Load History Record File used %dms", GetTickCount()-dwTime );
			theApp.LogInfo( strError );
		}*/

		// Get Root Element
		MSXML2::IXMLDOMElementPtr pRootElement( pXMLDoc->documentElement);
		if( !pRootElement )
		{
			strError = L"RecordHistoryFile:Root Element is NULL!";
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}
		
		// Get Series List
		MSXML2::IXMLDOMNodeListPtr pXMLChildNodes;
		hr = pRootElement->get_childNodes( &pXMLChildNodes );  
		if( FAILED(hr))
		{
			strError.Format( L"RecordHistoryFile-get_childNodes failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		// Loop Series List
		long lChildCounts = pXMLChildNodes->Getlength();
		ASSERT( lChildCounts >= 1 );
		MSXML2::IXMLDOMNodePtr pXMLCurrentNode;     // Series
		MSXML2::IXMLDOMNodeListPtr pPointNodes;     // points
		MSXML2::IXMLDOMNodePtr pPoints;
		MSXML2::IXMLDOMElementPtr pPointElement;
		for( long i = 0; i < lChildCounts; i++ )
		{
			pXMLCurrentNode = pXMLChildNodes->Getitem( i );     // Series Element
			if( pXMLCurrentNode && (MSXML2::NODE_ELEMENT == pXMLCurrentNode->nodeType))
			{
				pPointNodes = pXMLCurrentNode->GetchildNodes();
				if( pPointNodes )
				{
					pPoints = pPointNodes->Getitem( 0 );  // Points
					if( pPoints )
					{
						HisDatas& DatasToRecord = m_aHisRecordDatas->GetHisRecordDatas();
						const StringMap& localDataMap = DatasToRecord[i]->GetLocalDataMap();
						StringMap::const_iterator it = localDataMap.begin();
						while( it != localDataMap.end())
						{
							pPointElement = pXMLDoc->createElement( "point" );
							if( pPointElement )
							{
								pPointElement->setAttribute( "X", (LPCTSTR)it->first );
								pPointElement->setAttribute( "Y", (LPCTSTR)it->second );
								pPoints->appendChild( pPointElement );
							}

							++it;
						}
					}
				}
			}
		}//for

		m_aHisRecordDatas->ClearRecordDatas();
		if( WAIT_OBJECT_0 != WaitHisFileFinish())
		{
			strError = L"RecordHistoryFile::WaitHisFileFinish timeout!";
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		ResetHisFileFinishEvent();
		hr = pXMLDoc->save( (LPCTSTR)m_pRecordCtrl->GetFilePathName());
		if( FAILED( hr ))
		{
			SetHisFileFinishEvent();
			strError.Format( L"RecordHistoryFile-save failed:%d: %s!", hr, HrGetMessage(hr));
			theApp.LogInfo( strError );
			bResult = false;
			break;
		}

		m_pRecordCtrl->SetFileModified( true );
		SetHisFileFinishEvent();

		break;
	}//while
	
	CoUninitialize();
	return bResult;
}

// =======================================================================
// Function name  : CheckCurRecordFile
// Description    : 是否达到设定的最大值，如果到达最大值，则创建新文件
//                  并更新记录控制类
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.05.21 15:00:00
// =======================================================================
void CConfiguratorDoc::CheckCurRecordFile()
{
	CString strRecFilePathName( m_pRecordCtrl->GetFilePathName());
	if( !strRecFilePathName.IsEmpty())
	{
		CFileStatus status;
		CFile::GetStatus( strRecFilePathName, status );
		if( status.m_size >= m_pRecordCtrl->GetMaxSize())
		{
			// Get new record file path name
			int sernum = GetHisFileArrayCode( strRecFilePathName );
			ASSERT( sernum != -1 );
			if( sernum != -1 )
			{
				int index = strRecFilePathName.ReverseFind( L'-' );
				strRecFilePathName = strRecFilePathName.Left( index+1 );

				++sernum;
				strRecFilePathName.AppendFormat( L"%d.xml", sernum );

				// Create new record file
				if( CreateHistoryFile( strRecFilePathName ))
				{
					m_pRecordCtrl->SetFilePathName( strRecFilePathName );
					m_pRecordCtrl->SetFileModified( true );
				}
			}
		}//if size
	}// if !IsEmpty
}

bool CConfiguratorDoc::IsStoppingHistoryRecord()
{
	bool bResult = true;
	if( IsRecording())
	{
		if( AfxMessageBox( IDS_HISREC_STOP, MB_ICONQUESTION|MB_YESNO ) == IDYES )
		{
			OnHistoryStop();  // Stop
		}
		else
		{
			bResult = false;
		}
	}

	return bResult;
}
