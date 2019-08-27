
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Configurator.h"

#include "MainFrm.h"

// Added by wangdong
#include "ConfiguratorDoc.h"       // Document
#include "Splash.h"                // Splash screen
#include "ConfiguratorView.h"      // Splitter Window

/// Views
// Left Views
#include "SettingTreeView.h"       // Configuration and Aliases
#include "ValuesView.h"            // Trend
#include "TuneView.h"              // Tune
#include "AlarmView.h"             // Alarm
#include "HistoryValueView.h"      // History

// Right Views 
#include "BaseGridView.h"
#include "SettingGridView.h"       // Configuration
#include "TagsGridView.h"          // Aliases
#include "TrendView.h"             // Trend, tune, Alarm
#include "HistoryTrendView.h"      // History

#include "CommSettings.h"

// Dialog
#include "PasswordDlg.h"           // Password Dialog
#include "CompareDlg.h"            // Compare Dialog
#include "CommSettingsDlg.h"       // Comm Settings Dialog
#include "CtlCommSettingsDlg.h"    // Controller Comm Settings Dialog
#include "ProgressDlg.h"           // Progress Dialog
#include "SendDlg.h"               // Send Dialog
#include "SelAppTypeDlg.h"         // Select Application Type

#include "CalSheet.h"              // Calibration   

#include "CalibrateBase.h"

// Modbus Communications
#include "ModbusThread.h"   
#include "ModbusRange.h"
#include "Modbus.h"

// History Server
#include "RecordHistoryCtrl.h"
#include "ViewHistoryCtrl.h"

// Help system
#include "HtmlHelp.h"
#pragma comment( lib, "htmlhelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)	

	ON_COMMAND(IDM_DEVICE_ENTERPASSWORD, &CMainFrame::OnDeviceEnterpassword)
	ON_COMMAND(IDM_DEVICE_LOGOUT, &CMainFrame::OnDeviceLogout)
	ON_COMMAND(IDM_DEVICE_CONNECT, &CMainFrame::OnDeviceConnect)
	ON_COMMAND(IDM_DEVICE_DISCONNECT, &CMainFrame::OnDeviceDisconnect)
	ON_COMMAND(IDM_DEVICE_RETRIEVE, &CMainFrame::OnDeviceRetrieve)
	ON_COMMAND(IDM_DEVICE_SEND, &CMainFrame::OnDeviceSend)
	ON_COMMAND(IDM_DEVICE_COMPARE, &CMainFrame::OnDeviceCompare)
	ON_COMMAND(IDM_DEVICE_COMMSETTINGS, &CMainFrame::OnDeviceCommsettings)
	ON_COMMAND(IDM_CONTROLLER_COMMSETTINGS, &CMainFrame::OnControllerCommsettings)

	ON_COMMAND(IDM_MONITOR_START, &CMainFrame::OnMonitorStart)
	ON_COMMAND(IDM_MONITOR_STOP, &CMainFrame::OnMonitorStop)

	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
    ON_COMMAND(IDM_VIEW_CONFIG, &CMainFrame::OnViewConfig)
    ON_COMMAND(IDM_VIEW_TREND, &CMainFrame::OnViewTrend)
    ON_COMMAND(IDM_VIEW_TUNING, &CMainFrame::OnViewTuning)
    ON_COMMAND(IDM_VIEW_ALIASES, &CMainFrame::OnViewAliases)
    ON_COMMAND(IDM_VIEW_ALARMS, &CMainFrame::OnViewAlarms)
	ON_COMMAND(IDM_VIEW_HISTORY, &CMainFrame::OnViewHistory)

	ON_COMMAND(IDM_ACTION_CALIBRATE, &CMainFrame::OnActionCalibrate)
	ON_COMMAND(IDM_ACTION_RUN, &CMainFrame::OnActionRun)

	ON_WM_HELPINFO()
	ON_COMMAND(IDM_HELP_TOPICS, &CMainFrame::OnHelpTopics)

	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)

	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CMainFrame::OnUpdateFileSaveAs)

	ON_UPDATE_COMMAND_UI(IDM_DEVICE_LOGOUT, &CMainFrame::OnUpdateDeviceLogout)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_SELAPPTYPE, &CMainFrame::OnUpdateDeviceSelAppType)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_CONNECT, &CMainFrame::OnUpdateDeviceConnect)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_DISCONNECT, &CMainFrame::OnUpdateDeviceDisconnect)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_RETRIEVE, &CMainFrame::OnUpdateDeviceRetrieve)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_SEND, &CMainFrame::OnUpdateDeviceSend)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_COMPARE, &CMainFrame::OnUpdateDeviceCompare)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_COMMSETTINGS, &CMainFrame::OnUpdateDeviceCommsettings)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_TS350, &CMainFrame::OnUpdateDeviceTs350)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_DSC100, &CMainFrame::OnUpdateDeviceDsc100)
	ON_UPDATE_COMMAND_UI(IDM_DEVICE_DSC110, &CMainFrame::OnUpdateDeviceDsc110)
	ON_UPDATE_COMMAND_UI(IDM_CONTROLLER_COMMSETTINGS, &CMainFrame::OnUpdateControllerCommsettings)

	ON_UPDATE_COMMAND_UI(IDM_MONITOR_START, &CMainFrame::OnUpdateMonitorStart)
	ON_UPDATE_COMMAND_UI(IDM_MONITOR_STOP, &CMainFrame::OnUpdateMonitorStop)

	ON_UPDATE_COMMAND_UI(IDM_HISTORY_START, &CMainFrame::OnUpdateRecordStart)
	ON_UPDATE_COMMAND_UI(IDM_HISTORY_STOP, &CMainFrame::OnUpdateRecordStop)

	ON_UPDATE_COMMAND_UI(IDM_VIEW_LANGUAGE_CHS, &CMainFrame::OnUpdateViewLanguageChs)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LANGUAGE_ENG, &CMainFrame::OnUpdateViewLanguageEng)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_CONFIG, &CMainFrame::OnUpdateViewConfig)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_TREND, &CMainFrame::OnUpdateViewTrend)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_TUNING, &CMainFrame::OnUpdateViewTuning)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_ALIASES, &CMainFrame::OnUpdateViewAliases)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_ALARMS, &CMainFrame::OnUpdateViewAlarms)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_HISTORY, &CMainFrame::OnUpdateViewHistory)

	ON_UPDATE_COMMAND_UI(IDM_ACTION_CALIBRATE, &CMainFrame::OnUpdateActionCalibrate)
	ON_UPDATE_COMMAND_UI(IDM_ACTION_RUN, &CMainFrame::OnUpdateActionRun)
	ON_UPDATE_COMMAND_UI(IDM_HELP_TOPICS, &CMainFrame::OnUpdateHelpTopics)
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

//============================================================
/// Globals
// Define const
#define LEFTPANE  0            // Left column
#define RIGHTPANE 1            // Right column

std::auto_ptr<CCommSettings>         CommSettings;     
std::auto_ptr<modbus::CModbusThread> ModbusThread;   // Modbus Communication
std::auto_ptr<CWinThread>            CommThread;
std::auto_ptr<CProgressDlg>          ProgressDlg;
std::auto_ptr<CCalSheet>             CalSheet;

std::auto_ptr<CEvent> g_pCalibrateEvent( new CEvent( FALSE, FALSE, NULL, NULL ));
std::auto_ptr<CEvent> g_pTestCommEvent( new CEvent( FALSE, FALSE, NULL, NULL ));
std::auto_ptr<CEvent> g_pThreadEvent( new CEvent( FALSE, FALSE, NULL, NULL ));     // When suspend successful, Set Event
bool g_bSuspend = true; 

bool g_bPreView = false;

// History 
std::auto_ptr<CEvent> g_pHisEvent( new CEvent( TRUE, FALSE, NULL, NULL ));

// Thread functions
UINT ThreadFunc( LPVOID pParam )
{
	ModbusThread->ThreadExcute();

	return 0;                       // Successful
}
//============================================================


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	       : m_vSplitter()
		   , m_dwRefreshTicks( INFINITE )
		   , m_uThreadTimer( 0 )
		   , m_bAutoConnect( false )
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	m_bAutoMenuEnable = false;    // MFC不自动设置菜单项的使能状态，需要用户手动控制
}

CMainFrame::~CMainFrame()
{
}

bool CMainFrame::IsRecording()
{
	CConfiguratorDoc* pDoc = dynamic_cast<CConfiguratorDoc*>( GetActiveDocument());
	ASSERT( pDoc != NULL );
	return pDoc->IsRecording();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
	{
		theApp.LogInfo( L"CFrameWndEx::OnCreate Failed." );
		return -1;
	}

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		theApp.LogInfo( L"Failed to create menubar." );
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if ((!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)) ||
		(!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME)))
	{
		TRACE0("Failed to create toolbar\n");
		theApp.LogInfo( L"Failed to create standard toolbar." );
		return -1;      // fail to create
	}
    

	// ToolBar Button Text Vector
	std::vector<CString> strVector;
	CString strToolBar;
	for( size_t i = IDS_TOOL_NEW; i <= IDS_TOOL_ALARM; i++ )
	{
        strToolBar.LoadStringW( i );
		strVector.push_back( strToolBar );
	}

	// Standard ToolBar Text
	m_wndToolBar.SetButtonText( 0, strVector.at( 0 ));
	m_wndToolBar.SetButtonText( 1, strVector.at( 1 ));
	m_wndToolBar.SetButtonText( 2, strVector.at( 2 ));

	m_wndToolBar.SetButtonText( 4, strVector.at( 3 ));
	m_wndToolBar.SetButtonText( 5, strVector.at( 4 ));

	m_wndToolBar.SetButtonText( 7, strVector.at( 5 ));
	m_wndToolBar.SetButtonText( 8, strVector.at( 6 ));
	m_wndToolBar.SetButtonText( 9, strVector.at( 7 ));
	m_wndToolBar.SetButtonText( 10, strVector.at( 8 ));

	m_wndToolBar.SetButtonText( 12, strVector.at( 9 ));
	m_wndToolBar.SetButtonText( 13, strVector.at( 10 ));

	m_wndToolBar.SetButtonText( 15, strVector.at( 11 ));

	m_wndToolBar.EnableTextLabels();

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	/// View Toolbar 
	if ((!m_viewToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDC_VIEWTOOLBAR)) ||
		(!m_viewToolBar.LoadToolBar( theApp.m_bHiColorIcons ? IDR_VIEW_256 : 0 )))
	{
		TRACE0("Failed to create view toolbar\n");
		theApp.LogInfo( L"Failed to create view toolbar." );
		return -1;      // fail to create
	}

	// View ToolBar Text
	m_viewToolBar.SetToolBarBtnText( 0, strVector.at( 12 ));              // SetButtonText
	m_viewToolBar.SetToolBarBtnText( 1, strVector.at( 13 ));
	m_viewToolBar.SetToolBarBtnText( 2, strVector.at( 14 ));
	m_viewToolBar.SetToolBarBtnText( 3, strVector.at( 15 ));
	m_viewToolBar.SetToolBarBtnText( 4, strVector.at( 16 ));

	// Set View Toolbar Name
	CString strViewToolBar;
	bNameValid = strViewToolBar.LoadString(IDS_TOOLBAR_VIEW);
	ASSERT(bNameValid);
	m_viewToolBar.SetWindowTextW( strViewToolBar );

	// Customize
	m_viewToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		theApp.LogInfo( L"Failed to create status bar." );
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_viewToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_viewToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	/* TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);*/

	// 取消菜单栏的伸缩功能
	//m_wndMenuBar.SetRecentlyUsedMenus( FALSE );

	// Status Bar
	m_wndStatusBar.SetPaneWidth( 1, 200 );
	m_wndStatusBar.SetPaneText( 1, L"" );

	CommThread.reset( AfxBeginThread( ThreadFunc, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL)); 

	// Create Communication: CommSettings, and ModbusThread
	CommSettings.reset( new CCommSettings());
	CommSettings->ReadRegistry();

	ModbusThread.reset( new modbus::CModbusThread());
	ASSERT( ModbusThread.get());
	ModbusThread->SetCommSettings( CommSettings.get());         // Set m_pSerial, m_pSocket

	// Progress Dialog
	ProgressDlg.reset( new CProgressDlg(this));
	ProgressDlg->Create( IDD_PROGRESS );
	if( ProgressDlg->GetSafeHwnd())
	{
	    ProgressDlg->ShowWindow( SW_HIDE );
	}

	
	CalSheet.reset();                      // DSC100: Calibration Dialog
	OnViewConfig();

	CSplashWnd::ShowSplashScreen(this);    // Show Splash Screen: 1s

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
	{
		return FALSE;
	}
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( g_bPreView )
	{
	    g_bPreView = false;
	}
	else
	{
		CConfiguratorDoc* pDoc = dynamic_cast<CConfiguratorDoc*>( GetActiveDocument());
	    ASSERT( pDoc != NULL );
		if( pDoc->IsRecording())
		{
			pDoc->OnHistoryStop();
			WaitForSingleObject( pDoc->GetHisRecordSusEvent()->m_hObject, 2000 );
		}

		CalSheet.reset();
		DoDisconnect();
		CommSettings.reset();
		ModbusThread.reset();
		CommThread.reset();

		if( ProgressDlg.get() && ProgressDlg->GetSafeHwnd())
		{
			ProgressDlg->DestroyWindow();
			ProgressDlg.reset();
		}
	}

	CFrameWndEx::OnClose();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
			case ID_VIEW_APPLOOK_OFF_2007_BLUE:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

			case ID_VIEW_APPLOOK_OFF_2007_BLACK:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

			case ID_VIEW_APPLOOK_OFF_2007_SILVER:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

			case ID_VIEW_APPLOOK_OFF_2007_AQUA:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;

			default:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

// =========================================================================
// Function name: OnCreateClient
// Description  : Static Splitter
// Return type  : BOOL
// Parameters   : 
// Author       : WangDong  2013.04.16 10:50:00
// =========================================================================
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// One Row, two columns
	if( !m_vSplitter.CreateStatic( this, 1, 2 ))
	{
		return FALSE;
	}

	// Create all views, and Add to Left/Right view vectors
	// Left View
	if( !AddView( 0, LEFTPANE, RUNTIME_CLASS(CSettingTreeView),  pContext, CSettingTreeView::SETTING_GRID, true) ||
		!AddView( 0, LEFTPANE, RUNTIME_CLASS(CValuesView),       pContext, -1)                                   ||
		!AddView( 0, LEFTPANE, RUNTIME_CLASS(CTuneView),         pContext, -1)                                   ||
		!AddView( 0, LEFTPANE, RUNTIME_CLASS(CSettingTreeView),  pContext, CSettingTreeView::TAGS_GRID)          ||
		!AddView( 0, LEFTPANE, RUNTIME_CLASS(CAlarmView),        pContext, -1)                                   ||
		!AddView( 0, LEFTPANE, RUNTIME_CLASS(CHistoryValueView), pContext, -1))
	{
		return FALSE;
	}
	
	// Right View
	if( !AddView( 0, RIGHTPANE, RUNTIME_CLASS(CSettingGridView),  pContext, -1, true) ||
		!AddView( 0, RIGHTPANE, RUNTIME_CLASS(CTrendView),        pContext, -1)       ||
		!AddView( 0, RIGHTPANE, RUNTIME_CLASS(CTagsGridView),     pContext, -1)       ||
		!AddView( 0, RIGHTPANE, RUNTIME_CLASS(CHistoryTrendView), pContext, -1))
	{
		return FALSE;
	}

	// Adjust Columns size
	CRect rect;
	GetClientRect( &rect );
	m_vSplitter.SetColumnInfo( 0, rect.Width()/4, 100 );
	
	RecalcLayout();

	return TRUE;  
}

// -------------------------------------------------------------------
// Function name: AddView
// Description  : Static Splitter
// Return type  : BOOL
// Parameters   : 
// Author       : WangDong  2013.04.16 10:50:00
// --------------------------------------------------------------------
bool CMainFrame::AddView( int nRow, int nCol, CRuntimeClass* pViewClass, CCreateContext* pContext
	                    , int type, bool bFirst )
{
	if((nCol != LEFTPANE) && (nCol != RIGHTPANE))
	{
		return false;
	}

	if( !bFirst )
	{
		HideView( nRow, nCol );  // Hide the current view of the pane
	}

	// Create the new view,if fail, set the previous view current
	if( !m_vSplitter.CreateView( nRow, nCol, pViewClass, CSize( 200, 200 ), pContext))
	{
		return false;
	}

	CView* pView = (CView*)m_vSplitter.GetPane( nRow, nCol );
	ASSERT( pView->GetSafeHwnd());

	const char* pClassName = pViewClass->m_lpszClassName;
	if( strcmp( pClassName, "CSettingTreeView") == 0 )
	{
	    CSettingTreeView* pTagsTreeView = (CSettingTreeView*)pView;
		pTagsTreeView->SetViewType( type );   // If SettingTreeView, set its view type
	}

	// Add to vector
	if( LEFTPANE == nCol )
	{
	    theApp.LeftViewVector.push_back( pView );
	}
	else
	{
        theApp.RightViewVector.push_back( pView );
	}

	return true;
}

// =========================================================================
// Function name: Show/Hide View
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// =========================================================================
void CMainFrame::ShowView( int nRow, int nCol, CWnd* pWnd )
{
	if( pWnd->GetSafeHwnd())
	{ 
		pWnd->SetDlgCtrlID( m_vSplitter.IdFromRowCol(nRow, nCol));
		pWnd->ShowWindow( SW_SHOW );
	}
}

void CMainFrame::HideView( int nRow, int nCol )
{
    CWnd* pWnd = m_vSplitter.GetPane( nRow, nCol );
	if( pWnd->GetSafeHwnd())
	{
		pWnd->SetDlgCtrlID( 0 );
		pWnd->ShowWindow( SW_HIDE );
	}
}

// =========================================================================
// Function name: ShowClientView
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// =========================================================================
void CMainFrame::ShowClientView( CView* pView, int nColumn )
{
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
		if( pView->IsWindowVisible())
		{
		    ASSERT( pView->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, nColumn)); 
			pView->OnInitialUpdate();
		}
		else
		{
			HideView( 0, nColumn );
			ShowView( 0, nColumn, pView );
			pView->OnInitialUpdate();
		}

        m_vSplitter.RecalcLayout();   // 重新布局（不可缺少，否则视图尺寸不一致时，不会自动调整）
	}
}

// =========================================================================
// Function name: OnViewConfig
// Description  : Config View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// =========================================================================
void CMainFrame::OnViewConfig()
{
	// Right View
	CView* pView = theApp.RightViewVector[RSETTING_GRID];
	ASSERT( pView->GetSafeHwnd() );
	if( pView->GetSafeHwnd())
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left View
	pView = theApp.LeftViewVector[LSETTING_GRID];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// -------------------------------------------------------------------------
// Function name: OnViewTrend
// Description  : Trend View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// -------------------------------------------------------------------------
void CMainFrame::OnViewTrend()
{
	// Right View
	CView* pView = theApp.RightViewVector[RTREND];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd() )
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left View
	pView = theApp.LeftViewVector[LTREND];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// -------------------------------------------------------------------------
// Function name: OnViewTuning
// Description  : Tuning View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// -------------------------------------------------------------------------
void CMainFrame::OnViewTuning()
{
	// Right View
	CView* pView = theApp.RightViewVector[RTREND];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left View
	pView = theApp.LeftViewVector[LTUNING];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// -------------------------------------------------------------------------
// Function name: OnViewAliases
// Description  : Aliases View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// -------------------------------------------------------------------------
void CMainFrame::OnViewAliases()
{
	// Right View
	CView* pView = theApp.RightViewVector[RALIASES];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left View
	pView = theApp.LeftViewVector[LALIASES];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// -------------------------------------------------------------------------
// Function name: OnViewAlarms
// Description  : Alarms View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// -------------------------------------------------------------------------
void CMainFrame::OnViewAlarms()
{
	// Right View
	CView* pView = theApp.RightViewVector[RTREND];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left pane
	pView = theApp.LeftViewVector[LALARMS];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// -------------------------------------------------------------------------
// Function name: OnViewHistory
// Description  : History Trend View
// Return type  : void
// Parameters   : 
// Author       : wangdong 2010.10.27 09:00:00
// -------------------------------------------------------------------------
void CMainFrame::OnViewHistory()
{
	// Right View
	CView* pView = theApp.RightViewVector[RHISTORY];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
		ShowClientView( pView, RIGHTPANE );
	}

	// Left pane
	pView = theApp.LeftViewVector[LHISTORY];
	ASSERT( pView->GetSafeHwnd());
	if( pView->GetSafeHwnd())
	{
	    ShowClientView( pView, LEFTPANE );
	}
}

// =========================================================================
// Function name: IsConnected
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong
// =========================================================================
bool CMainFrame::IsConnected()
{
	bool bConnected = false;

	if( CheckReadPtr( ModbusThread.get(), sizeof(modbus::CModbusThread)))
	{
	    bConnected = ModbusThread->GetConnected();
	}

	return bConnected;
}

// =========================================================================
// Function name: ShowProgressDlg
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.09 19:10:00
// =========================================================================
void CMainFrame::ShowProgressDlg( void )
{
	if( !ProgressDlg.get())
	{
		return;
	}

	if( !ProgressDlg->GetSafeHwnd())
	{
		ProgressDlg->Create( IDD_PROGRESS );
	}

	if( ProgressDlg->GetSafeHwnd() && ModbusThread.get())
	{
	    ProgressDlg->SetMessage( ModbusThread->GetStatusMessage());
		ProgressDlg->SetProgress( ModbusThread->GetStatusProgress());
		ProgressDlg->ShowWindow( SW_SHOW );
	}
}

// =========================================================================
// Function name: UpdateStatusBar
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.15 16:00:00
// =========================================================================
void CMainFrame::UpdateStatusBar( void )
{
	if( !IsConnected())
	{
        m_wndStatusBar.SetPaneText( 1, L"" );
		return;
	}

	ASSERT( ModbusThread.get());

	CString strCommStatus;
	if( ModbusThread->IsDone())
	{
		strCommStatus.LoadString( IDS_STATUS_IDLE );              //"Idle"
	}
	else
	{
        switch( ModbusThread->GetAction())
        {
            case modbus::CModbusThread::mbsCONNECT:
				strCommStatus.LoadString( IDS_STATUS_CONNECT );   //"Connecting";
            break;

            case modbus::CModbusThread::mbsDOWNLOAD:
				strCommStatus.LoadString( IDS_STATUS_DOWNLOAD );  //"Download";
            break;

            case modbus::CModbusThread::mbsUPLOAD:
                strCommStatus.LoadString( IDS_STATUS_UPLOAD );    //"Upload";
            break;

            case modbus::CModbusThread::mbsMONITOR:
                {
                    // TODO: Use scan time instead.
                    strCommStatus.Format( L"%d / %d / %d / %d",
                                          ModbusThread->GetReadMessageCount(),
                                          ModbusThread->GetWriteMessageCount(),
                                          ModbusThread->GetErrorMessageCount(),
                                          ModbusThread->GetScanTime());
                }
            break;

			default:
				strCommStatus.Empty();
			break;
        }//switch
	}

	m_wndStatusBar.SetPaneText( 1, strCommStatus ); 
	//m_status.SetWindowText( strCommStatus );
}

// =========================================================================
// Function name: OnTimer
// Description  : WM_TIMER
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.08 15:36:00
// =========================================================================
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// Thread Timer
	if( nIDEvent == IDC_THREADTIMER )
	{
		// CheckThread();
		ASSERT( ModbusThread.get());

		// 1. When communicating, update the progress dialog if visible
		if( ModbusThread->GetAction() != modbus::CModbusThread::mbsNONE )
		{
			if( ProgressDlg->GetSafeHwnd() && ProgressDlg->IsWindowVisible())
			{
				ProgressDlg->SetMessage( ModbusThread->GetStatusMessage());
				ProgressDlg->SetProgress( ModbusThread->GetStatusProgress());
			}
		}

		// Update Status Bar
		UpdateStatusBar();

		switch( ModbusThread->GetAction())
		{
			case modbus::CModbusThread::mbsCONNECT:  
			case modbus::CModbusThread::mbsUPLOAD:
            case modbus::CModbusThread::mbsDOWNLOAD:
			case modbus::CModbusThread::mbsTEST:
                 // Refresh ticks which are only used during monitoring
                 if (m_dwRefreshTicks != INFINITE)
				 {
                     m_dwRefreshTicks = INFINITE;
				 }

				 // Check if thread has done?
                 CheckThreadDone();
            break;

			case modbus::CModbusThread::mbsMONITOR:
				if( ModbusThread->HasEvent())       // When finish one period
				{
					ModbusThread->ResetEvent();     // Reset Event
					g_pCalibrateEvent->SetEvent();

					if( ModbusThread->GetStatusError() == S_OK )
					{
                        // Update all Views
					    ((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews( NULL, CConfiguratorDoc::HINT_MONITOR, NULL );

						// Force refresh when do read/write during monitoring
						if((m_dwRefreshTicks != INFINITE) &&
                           (::GetTickCount() > m_dwRefreshTicks))
                        {
							if((CConfiguratorApp::TS350 == theApp.GetProductType()) && 
							   ((CConfiguratorDoc*)GetActiveDocument()))
							{
								((CConfiguratorDoc*)GetActiveDocument())->SetVisible();
								g_bReadRefresh = true;
							}

                            ((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH, NULL);
                            m_dwRefreshTicks = INFINITE;

							if( g_bReadRefresh )
							{
							    g_bReadRefresh = false;
							}
                        }
					}// if S_OK
					else
					{
						bool bDoDisConnect = true;
						if( ModbusThread->GetStatusError() == modbus::E_CLIENT_TIMEOUT )
						{
                            bDoDisConnect = ModbusThread->IsDone();
						}

						if( bDoDisConnect )
						{
							// When monitoring, and has been set to autoconnect, try again
			                if( m_bAutoConnect )
							{
								// Clear m_pStatus error code and error message
								ModbusThread->ClearStatusError();
                            
								// When Modbus TCP, need to reconnect
								if( ModbusThread->GetModbusType())   // true: Modbus TCP
								{
									HRESULT hr = S_OK;
									do
									{
										::Sleep(0);
										theApp.LogInfo( L"Modbus TCP: Reconnect in CMainFrame::OnTimer" );
										hr = ModbusThread->Connect();  
									}while( FAILED(hr));

									ModbusThread->SetConnected( true );    //DoConnect();
								}

								theApp.LogInfo( L"Try Re-Monitor in CMainFrame::OnTimer" );

								// Monitor
								if( IsConnected())
								{
									DoMonitor();
								}
							}
							else
							{
								// Disconnect
								DoDisconnect();

								// When Calibrating, tell user the communication was failed, and then close the calibrate window
								// Otherwise, display the communication error.
								if( CalSheet.get() && CalSheet->GetSafeHwnd())
								{
                                    CString str;
									str.LoadString( IDS_CAL_CLOSE );
									::MessageBox( CalSheet->GetSafeHwnd(), str, L"", MB_OK|MB_ICONERROR );
									//MessageBoxW( str );
 
									CalSheet->PressButton( PSBTN_CANCEL );
									//CalSheet->EndDialog( PSWIZB_CANCEL );
								}
								else
								{
									// Show Error Message
									CString strCaption, strErrorMsg;
									strCaption.LoadString( IDS_ERROR );        // "Error" 
									strErrorMsg.Format( L"%s 0x%08X: %s", strCaption, ModbusThread->GetStatusError(), ModbusThread->GetStatusErrorMsg() );
									MessageBox( strErrorMsg, strCaption, MB_OK | MB_ICONERROR );
								}

								// Clear m_pStatus error code and error message
								ModbusThread->ClearStatusError();

								// Refresh all views
								((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH, NULL);
							}
						}// if( bDoDisConnect ): Thread done
						else
						{
							// Clear m_pStatus error code and error message
							ModbusThread->ClearStatusError();
						}
					}//else
				}//if( ModbusThread->HasEvent())

				// Check if thread has done?
                CheckThreadDone();
			break;

			default:
				theApp.LogInfo( L"Unknown connect state in OnTimer()." );
				ASSERT( ModbusThread->IsDone() );
				DoDisconnect();
		}//switch
	}

	CFrameWndEx::OnTimer(nIDEvent);
}

// =========================================================================
// Function name: CheckThreadDone
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.09 13:00:00
// =========================================================================
void CMainFrame::CheckThreadDone(void)
{
	if( CheckWritePtr( ModbusThread.get(), sizeof(modbus::CModbusThread)))
	{
		// If not done(or not suspended), return
		if( !ModbusThread->IsDone())
		{
			return;
		}
		
		// Destroy Thread Timer
		if( m_uThreadTimer != 0 )
		{
		    KillTimer( m_uThreadTimer );     
			m_uThreadTimer = 0;
		}

		// Suspend
		g_bSuspend = true;           
		WaitForSingleObject( g_pThreadEvent->m_hObject, 1000 );
		::Sleep( 50 );

		// bTestSettings: Test commsettings in the PC Comm Settings... dialog
		// bMonitor: Monitor operation
		bool bTestSettings = false, bMonitor = false;     
		if( ModbusThread->GetAction() == modbus::CModbusThread::mbsCONNECT )
		{
			ModbusThread->SetConnected( ModbusThread->GetStatusError() == S_OK );

			// TS350, Connected successful: Set Title, Local and Controller
			if(CConfiguratorApp::TS350 == theApp.GetProductType())
			{
				if( ModbusThread->GetStatusError() == S_OK )
				{
					UpdateTitle( TITLE_LOCALCTRL );  
				}
				else if( modbus::E_DEVICE_PRODUCT_APPTYPE == ModbusThread->GetStatusError())
				{
					ModbusThread->ClearStatusError();  // Application Type mismatch, user select No
				}
			}//TS350
		}
		else if( ModbusThread->GetAction() == modbus::CModbusThread::mbsTEST )
		{
            bTestSettings = true;
		}
		else if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
		{
			bMonitor = true;
		}
		else if( ModbusThread->GetAction() == modbus::CModbusThread::mbsUPLOAD )
		{
			if((CConfiguratorApp::TS350 == theApp.GetProductType()) && 
			   ((CConfiguratorDoc*)GetActiveDocument()))
			{
				((CConfiguratorDoc*)GetActiveDocument())->SetVisible();
				g_bReadRefresh = true;
			}
		}
		else if( ModbusThread->GetAction() == modbus::CModbusThread::mbsDOWNLOAD )
		{
			if((CConfiguratorApp::TS350 == theApp.GetProductType()) && 
			   (ModbusThread->GetStatusError() == S_OK )            &&
			   theApp.m_bAppMismatch )
			{
				CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
				if( pDoc )
				{
					CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"iAppType" );   // 31002, Application Type
					if( CheckReadPtr( pAlias, sizeof(CAliasItem)) &&
						(pAlias->remI2() == theApp.GetAppType()))
					{
					    theApp.m_bAppMismatch = false;          // Reset Application Mismatch Flag
						UpdateTitle( TITLE_LOCALCTRL ); 
					}
				}// pDoc                
			}// TS350, Write Successful and Application Type Mismatch
		}

		ModbusThread->ClearMessages();       // Clear modbus msgs
		ModbusThread->ResetThreadFlags();    // Reset thread flags and events

		if( ModbusThread->GetStatusError() != S_OK )
		{
			// When monitoring, and has been set to autoconnect, try again
			if( m_bAutoConnect && bMonitor )
			{
				// Clear m_pStatus error code and error message
				ModbusThread->ClearStatusError();
                            
				// When Modbus TCP, need to reconnect
				if( ModbusThread->GetModbusType())   // true: Modbus TCP
				{
					HRESULT hr = S_OK;
					do
					{
						::Sleep(0);
						theApp.LogInfo( L"Modbus TCP: Reconnect in CMainFrame::CheckThreadDone" );
						hr = ModbusThread->Connect();  
					}while( FAILED(hr));

					ModbusThread->SetConnected( true );   
				}

				theApp.LogInfo( L"Try Re-Monitor in CMainFrame::CheckThreadDone" );

				// Monitor
				if( IsConnected())
				{
					DoMonitor();
				}
			}
			else  // Normal operation
			{
				// Disconnect
				DoDisconnect();

				// When Calibrating, tell user the communication was failed, and then close the calibrate window
				// Otherwise, display the communication error.
				if( CalSheet.get() && CalSheet->GetSafeHwnd())
				{
                    CString str;
					str.LoadString( IDS_CAL_CLOSE );
					::MessageBox( CalSheet->GetSafeHwnd(), str, L"", MB_OK|MB_ICONERROR );
					//MessageBoxW( str );

					CalSheet->PressButton( PSBTN_CANCEL );   
					//CalSheet->EndDialog( PSWIZB_CANCEL );  // Modal
				}
				else
				{
					// Show Error Message
					if( !ModbusThread->GetStatusErrorMsg().IsEmpty())
					{
						CString strCaption, strErrorMsg;
						strCaption.LoadString( IDS_ERROR );        // "Error" 
						strErrorMsg.Format( L"%s 0x%08X: %s", strCaption, ModbusThread->GetStatusError(), ModbusThread->GetStatusErrorMsg());
						MessageBox( strErrorMsg, strCaption, MB_OK | MB_ICONERROR );
					}
					else
					{
						theApp.OnFileNew();
					}
				}
			}
		}//if( ModbusThread->GetStatusError() != S_OK )
		else if( bTestSettings )
		{
			CString strCaption, strSuccess;
			strCaption.LoadString( IDS_INFO );               // Information
			strSuccess.LoadString( IDS_TEST_SUCCESS );       // Success!
            MessageBox( strSuccess, strCaption, MB_OK | MB_ICONINFORMATION );

		    // Restore Previous CommSettings
			ModbusThread->SetCommSettings( CommSettings.get());

			// Close
			ModbusThread->Close();
		}

		if( bTestSettings )
		{
			g_pTestCommEvent->SetEvent();
		}

		// Refresh all views 
		((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH, NULL );
		if( g_bReadRefresh )
		{
			g_bReadRefresh = false;
		}
		m_dwRefreshTicks = INFINITE;

		// Hide ProgressDlg
		if( ProgressDlg.get() && ProgressDlg->GetSafeHwnd())
		{
		    ProgressDlg->ShowWindow( SW_HIDE );
		}
	}//if( CheckWritePtr( ModbusThread.get(), sizeof(modbus::CModbusThread)))
}

// =========================================================================
// Function name: UpdateTitle
// Description  : Only for TS350
// Return type  : 
// Parameters   : 
//                TITLE_NONE:        Delete application information;
//                TITLE_LOCAL:       Add Local Application Type;
//                TITLE_LOCALREMOTE: Add Local and Remote Application Type.
// Author       : WangDong 2010.03.21 11:00:00
// =========================================================================
void CMainFrame::UpdateTitle( UINT uType )
{
	m_strTitle = m_strTitle.Left( m_strTitle.Find( L"Configurator" ) + sizeof( L"Configurator" )/sizeof( wchar_t ) - 1 );

	CString strGen, strMech;
	strGen.LoadStringW( IDS_APPTYPE_GEN );
	strMech.LoadStringW( IDS_APPTYPE_MECH );

	CString strLocalType((1 == theApp.GetAppType()) ? strGen : strMech );
	CString strCtrlType( strLocalType );
	if( theApp.m_bAppMismatch )  
	{
        strCtrlType = (1 != theApp.GetAppType()) ? strGen : strMech;  // different from local
	}
	else
	{
		strCtrlType = (1 == theApp.GetAppType()) ? strGen : strMech;  // Same with local
	}

	switch( uType )
	{
		case TITLE_LOCAL:
		    m_strTitle.AppendFormat( IDS_APPTYPE_LOCAL, strLocalType );
        break;
		case TITLE_LOCALCTRL:
			m_strTitle.AppendFormat( IDS_APPTYPE_LOCAL_CTRL, strLocalType, strCtrlType );
		break;
		default:  // xxx Configurator
        break;
	}//switch

	OnUpdateFrameTitle( TRUE );
}

// =========================================================================
// Function name: TestCommSettings
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.21 11:00:00
// =========================================================================
void CMainFrame::TestCommSettings( CCommSettings* pTestSettings )
{
    if( !CheckReadPtr( pTestSettings, sizeof(CCommSettings)))
	{
		g_pTestCommEvent->SetEvent();
		return;
	}

	ASSERT( ModbusThread.get());

	// Set Test CommSettings
	ModbusThread->SetCommSettings( pTestSettings );

	// Open
	HRESULT hr = ModbusThread->Connect(); 
    if( FAILED(hr))
    {
        // TODO: Better error message
        CString strError, strMessage, strCaption;

        if( hr != S_OK )
		{
            strMessage = HrGetMessage(hr);
		}
        else
		{
			strMessage.LoadString( IDS_CONNECT_UNKNOWN );
		}

		if( ModbusThread->GetModbusType())
		{
            strError.Format( IDS_CONNECT_TCPERR, hr, strMessage );
		}
		else
		{
			strError.Format( IDS_CONNECT_SERERR, CommSettings->m_strPort, hr, strMessage );
		}

		strCaption.LoadString( IDS_ERROR );
		MessageBox( strError, strCaption, MB_OK | MB_ICONERROR ); 

        ModbusThread->Close();
		g_pTestCommEvent->SetEvent();
    }//if( FAILED(hr))
    else
	{ 
        if( !ModbusThread->GetModbusType())
        {
            ::Sleep(100);
        }
        else
        {
            ::Sleep(500);
        }

		// Build Modbus messages
        ModbusThread->LoopbackMessage();

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );
    }//else
}

//==========================================================================
/// Menu command
/// Device
// =========================================================================
// Function name: OnDeviceEnterpassword / OnDeviceLogout
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.04.14 16:11:00
// =========================================================================
void CMainFrame::OnDeviceEnterpassword()
{
	// TODO: 在此添加命令处理程序代码
	bool bAdminOri = global::Security.CanAdmin();

	std::auto_ptr<CPasswordDlg> dlg( new CPasswordDlg());
	if( dlg->DoModal() == IDOK )
	{
		bool bAdminCur = global::Security.CanAdmin();
		bool bRedraw = (bAdminOri != bAdminCur);
		theApp.SetAccessRedraw( bRedraw );

		CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
		if( pDoc )
		{
			pDoc->SetVisible();
			pDoc->UpdateAllViews( NULL );
		}

		theApp.SetAccessRedraw( false );
	}

	//PlayAppSound("Explorer", "Navigating", SND_FILENAME | SND_NODEFAULT | SND_NOSTOP | SND_ASYNC);
}

void CMainFrame::OnDeviceLogout()
{
	// TODO: 在此添加命令处理程序代码
	bool bAdminOri = global::Security.CanAdmin();
	global::Security.Logout();

	bool bAdminCur = global::Security.CanAdmin();
	bool bRedraw = (bAdminOri != false);
	theApp.SetAccessRedraw( bRedraw );
	
	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
	if( pDoc )
	{
		pDoc->SetVisible();
		pDoc->UpdateAllViews( NULL );
	}

	theApp.SetAccessRedraw( false );

	//PlayAppSound("Explorer", "Navigating", SND_FILENAME | SND_NODEFAULT | SND_NOSTOP | SND_ASYNC);
}

// =========================================================================
// Function name: OnDeviceConnect / OnDeviceDisconnect
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.08 15:36:00
// =========================================================================
void CMainFrame::OnDeviceConnect()
{
	// TODO: 在此添加命令处理程序代码
	if( IsConnected())
	{
		CConfiguratorDoc *pDoc = (CConfiguratorDoc*)GetActiveDocument();
		ASSERT( pDoc != NULL );
		if( !pDoc->IsStoppingHistoryRecord())
		{
			return;
		}
        DoDisconnect();
	}
    else
	{
        DoConnect();
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnDeviceDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	DoDisconnect();
}

//--------------------------------------------------------------------------
// Function name: DoConnect
// Description  : Attempts to connect to the device through the serial port
//                or through simulation (TCP/IP).
// Return type  : void
// Parameters   : 
// Author       : rjvs 2002-03-12 10:36:22 (UTC-6:00)
//                WangDong 2010.03.08 15:36:00
//--------------------------------------------------------------------------
void CMainFrame::DoConnect( void )
{
    if( !global::Security.IsValid())
    {
		CString str;
		str.LoadStringW( IDS_CONNECT_PERMISSION );

		AfxMessageBox( str, MB_OK | MB_ICONERROR );
        return;
    }

	ASSERT( ModbusThread.get());

    // Check the communications settings.
	bool bModbusTcp = ModbusThread->GetModbusType();
	if( !CommSettings->IsValid( bModbusTcp ))
	{
		// if user doesn't click OK, cancel connect operation
		if( !DoCommSettingsDialog())
		{
			return;                   
		}
	}

    // Set threading defaults
    ModbusThread->SetAliasMap(((CConfiguratorDoc*)GetActiveDocument())->m_aAliases );

    ((CConfiguratorDoc*)GetActiveDocument())->m_aAliases->SetDirtyRemote( false );     // Reset alias dirty remote flag
    HRESULT hr = ModbusThread->Connect();  // open
    if( FAILED(hr))
    {
        // TODO: Better error message
        CString strError, strMessage, strCaption;

        if( hr != S_OK )
		{
            strMessage = HrGetMessage(hr);
		}
        else
		{
			strMessage.LoadString( IDS_CONNECT_UNKNOWN );
		}

		if( ModbusThread->GetModbusType())
		{
            strError.Format( IDS_CONNECT_TCPERR, hr, strMessage );
		}
		else
		{
			strError.Format( IDS_CONNECT_SERERR, CommSettings->m_strPort, hr, strMessage );
		}

		strCaption.LoadString( IDS_ERROR );
		MessageBox( strError, strCaption, MB_OK | MB_ICONERROR ); 

        ModbusThread->Close();
    }//if( FAILED(hr))
    else
	{ 
        if( !ModbusThread->GetModbusType())
        {
            ::Sleep(100);
        }
        else
        {
            ::Sleep(500);
        }

		// Build Modbus messages
        ModbusThread->ConnectToDevice();

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}

		// Show progress dialog
		ShowProgressDlg();

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );
    }
}

//--------------------------------------------------------------------------
// Function name: DoDisconnect
// Description  : Disconnects from the device
//                and closes any open communications port.
// Return type  : void
// Parameters   : 
// Author       : rjvs 2002-03-12 10:36:22 (UTC-6:00)
//                WangDong 2010.03.08 15:36:00
//--------------------------------------------------------------------------
void CMainFrame::DoDisconnect()
{
    bool bConnected = IsConnected();

	// Destroy Thread Timer if it has not been killed
	if( m_uThreadTimer != 0 )
	{
        KillTimer( m_uThreadTimer );     
		m_uThreadTimer = 0;
	}

	// Update thread
    if( CheckWritePtr( ModbusThread.get(), sizeof(modbus::CModbusThread)))
    {
		// Suspend
		if( !g_bSuspend )
		{
		    g_bSuspend = true;            
		    WaitForSingleObject( g_pThreadEvent->m_hObject, 1000 );
		    ::Sleep( 50 );    
            ModbusThread->ClearMessages();
		    ModbusThread->ResetThreadFlags();
		}

        ModbusThread->Close();
    }

	// Hide progress dlg
	if( ProgressDlg.get() && ProgressDlg->GetSafeHwnd())
	{
        ProgressDlg->ShowWindow( SW_HIDE );
	}

	// UpdateStatusBar
	UpdateStatusBar();

    // Update the views if the document was connected at the beginning of this
    // function call. 
    if( bConnected )
    {
		if( CConfiguratorApp::TS350 == theApp.GetProductType())
		{
			UpdateTitle( CMainFrame::TITLE_LOCAL );    // Title

			if( theApp.m_bAppMismatch )                
			{
                theApp.m_bAppMismatch = false;          // Reset Application Mismatch Flag
			}
		}// TS350

        ((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews(NULL, CConfiguratorDoc::HINT_REFRESH, NULL);  
	}
}

// =========================================================================
// Function name: OnDeviceRetrieve
// Description  : 1. Check connected;
//                2. Confirm upload;
//                3. Build modbus retrieve msgs
//                4. Resume thread, set timer, show progress dlg
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.10 09:40:00
// =========================================================================
void CMainFrame::OnDeviceRetrieve()
{
	ASSERT( ModbusThread.get());
    ASSERT( IsConnected());
	if((!ModbusThread.get()) || (!IsConnected()))
	{
		return;
	}

	// Load Retrieve Waring message
	CString strProductName, strWaring, strCaption;
	strCaption.LoadString( IDS_CONFIRM );
	strProductName = theApp.GetProductName();
	strWaring.Format( IDS_RECEIVECFG_WARNING, strProductName, strProductName );

	// Confirm Upload
	if( MessageBox( strWaring, strCaption, MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		// Set Modified
		((CConfiguratorDoc*)GetActiveDocument())->SetModifiedFlag( TRUE );

        // Build Modbus messages
        ModbusThread->ReceiveFromDevice();

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}

		// Show progress dialog
		ShowProgressDlg();

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );
	}
}

// =========================================================================
// Function name: OnDeviceSend
// Description  : 1. Check connected;
//                2. Display Send Dialog;
//                3. Build modbus send msgs
//                4. Resume thread, set timer, show progress dlg
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.10 09:40:00
// =========================================================================
void CMainFrame::OnDeviceSend()
{
	//1. If unconnected, return
    ASSERT( ModbusThread.get() && IsConnected());
	if((!ModbusThread.get()) || (!IsConnected()))
	{
		return;
	}

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
	ASSERT( pDoc != NULL );
	if( pDoc == NULL )
	{
		return;
	}

	// YES: Save and send; NO: Unsave and send; CANCEL: return directly
	if( !pDoc->SaveModified())
	{
		return;
	}

	///2. Start monitoring the device if TS350
	if( theApp.GetProductType() == CConfiguratorApp::TS350 )
	{
        DoMonitor();
	}

	///3. Update m_bDirtyRemote flags: All Read/Write Alias(except string datatype)
    pDoc->UpdateRemoteFlags( 1, 10000 );
	pDoc->UpdateRemoteFlags( 40001, 50000 );

	bool bCTLRunning = false;
	if( CConfiguratorApp::TS350 == theApp.GetProductType())
	{
		if( ModbusThread->HasEvent( 2000 ))
		{
			bool bShutdownMode = false, bFTrip7 = false, bCTrip1 = false;
            CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"rMBMODE" );    // 35129, Mode
            if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
			{
                bShutdownMode = (fabs(pAlias->valR4() - 1.0) <= EPSILON_FLT);  //(pAlias->valR4() == 1.0);
			}

			if( bShutdownMode )
			{
				CAliasItem* pAlias = pDoc->m_aAliases->find_name( L"fTRIP7" );  // 12086
				if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
				{
					bFTrip7 = (pAlias->remBool() != VARIANT_FALSE);
				}

				pAlias = pDoc->m_aAliases->find_name( L"cTRIP1" );  // 12050
				if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
				{
					bCTrip1 = (pAlias->remBool() != VARIANT_FALSE);
				}
			}//if( bShutdownMode )
			
			// Running
			//bCTLRunning = !(bShutdownMode && (bFTrip7 || bCTrip1));
			bCTLRunning = !bShutdownMode;
			if( bCTLRunning )
			{
				// Application Type Mismatch: Send is not allowed.
				if( theApp.m_bAppMismatch )
				{
					AfxMessageBox( IDS_SEND_ERROR, MB_OK | MB_ICONERROR );
					DoStopMonitor();
					return;
				}
			}//if( !(bShutdownMode && (bFTrip7 || bCTrip1)))
		}//if( ModbusThread->HasEvent( 2000 ))
	}//TS350

	/*ASSERT( !(bCTLRunning && theApp.m_bAppMismatch));
	if( bCTLRunning && theApp.m_bAppMismatch )
	{
		return;
	}*/

	///4. Create the Send Dialog
	std::auto_ptr<CSendDlg> dlg( new CSendDlg( bCTLRunning ));   
	if( dlg.get() && (dlg->DoModal() == IDOK))
	{
		// Build Modbus messages
		ModbusThread->SendToDevice( dlg->m_bDirtyOnly, dlg->m_bTuningOnly );

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}

		// Show progress dialog
		ShowProgressDlg();

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );

		 // Check if thread has terminated (usually an exception)
        //CheckThread();

        //MyAnimation->Active = true;
	}// if( dlg->DoModal() == IDOK )  
	else if( theApp.GetProductType() == CConfiguratorApp::TS350 )
	{
		DoStopMonitor();
	}
}

// =========================================================================
// Function name: OnDeviceCompare
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.04.13 15:50:00
// =========================================================================
void CMainFrame::OnDeviceCompare()
{
	// Get SettingGridView
	CSettingGridView* pCfgView = (CSettingGridView*)theApp.RightViewVector[RSETTING_GRID];
	ASSERT( pCfgView->GetSafeHwnd());
	if( !pCfgView->GetSafeHwnd())
	{
		return;
	}

	if( pCfgView->IsCompareColVisible())
	{
        pCfgView->ShowCompareColumn( false );
	}
	else // Show Compare Dialog
	{
		CWaitCursor wait;
		std::auto_ptr<CCompareDlg> dlg( new CCompareDlg());
		if( dlg->DoModal() == IDOK )
		{
			if( dlg->m_nCompareType == CConfiguratorApp::CMP_REM )
			{
				theApp.SetCmpType( dlg->m_nCompareType );         // Set compare type

				CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
				if( pDoc )
				{
					pDoc->ResetCmpFilePathName();
				}

				if( IsConnected())
				{
					DoDisconnect();
				}
				DoConnect();

				// Show Configuration View and Compare column
				pCfgView->ShowCompareColumn( true );
				OnViewConfig();   
			}
			else // Compare file
			{
				CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
				if( pDoc )
				{
					if( pDoc->CompareFile( dlg->GetFileName(), true ))
					{
						theApp.SetCmpType( dlg->m_nCompareType );         // Set compare type
						pCfgView->ShowCompareColumn( true );
						OnViewConfig(); 
					}
				}//if( pDoc )
			}
		}//if( dlg->DoModal() == IDOK )
	}//else
}

// =========================================================================
// Function name: OnDeviceCommsettings
// Description  : 1.Create CommSettings Dialog;
//                2.Initialize it with CommSettings;
//                3.When User clicked ok button, 
//                  set CommSettings and write it to registry,
//                  and then set m_pSerial, and m_pSocket of the ModbusThread 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2010.03.03 10:00:00
// =========================================================================
void CMainFrame::OnDeviceCommsettings()
{
	// TODO: 在此添加命令处理程序代码
	DoCommSettingsDialog();
}

//--------------------------------------------------------------------------
// Function name: DoCommSettingsDialog
// Description  : 
// Return type  : True: User Click OK;
// Parameters   : 
// Author       : WangDong 2010.03.03 10:00:00
//--------------------------------------------------------------------------
bool CMainFrame::DoCommSettingsDialog( void )
{
	ASSERT( ModbusThread.get());

	CWaitCursor wait;
	bool bResult = false;
    std::auto_ptr<CCommSettingsDlg> dlg( new CCommSettingsDlg());
	dlg->m_pSettings = CommSettings.get();
	if( dlg->DoModal() == IDOK )
	{
		CommSettings->WriteRegistry();

		// Set m_bModbusTcp, m_pSerial, m_pSocket and m_pDriver
		ModbusThread->SetCommSettings( CommSettings.get());  

		bResult = true;
	}

	bResult = false;

	return bResult;
}

// =========================================================================
// Function name: OnControllerCommsettings
// Description  : TS350
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.11.17 15:30:00
// =========================================================================
void CMainFrame::OnControllerCommsettings()
{
	// TODO: 在此添加命令处理程序代码
	//1. If unconnected, return
	ASSERT((theApp.GetProductType() == CConfiguratorApp::TS350) && ModbusThread.get() && IsConnected());
	if((theApp.GetProductType() != CConfiguratorApp::TS350) || (!ModbusThread.get()) || (!IsConnected()))
	{
		return;
	}

	//2. Start monitoring to update corresponding parameters if TS350 and not monitoring
	bool bPreMonitor = false;
	if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
	{
        bPreMonitor = true;
	}
	else
	{
		DoMonitor();
	}

	if( !bPreMonitor )
	{
		if( ModbusThread->HasEvent(2000))
		{
			DoStopMonitor();
		}
	}

	///3. Create the Controller Comm Settings Dialog
	std::auto_ptr<CCtlCommSettingsDlg> dlg( new CCtlCommSettingsDlg());
	if( dlg.get() && (dlg->DoModal() == IDOK))
	{
		SendCTRLComSettings(); 
	}//if
}

//==========================================================================
/// Monitor
// =========================================================================
// Function name: OnMonitorStart
// Description  : 1. Check connected;
//                2. Build monitor modbus messages
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.10 14:00:00
// =========================================================================
void CMainFrame::OnMonitorStart()
{
	DoMonitor();
	((CConfiguratorDoc*)GetActiveDocument())->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH, NULL );  // Refresh all views
}

//--------------------------------------------------------------------------
// Function name: OnMonitorStop
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.10 13:50:00
//--------------------------------------------------------------------------
void CMainFrame::OnMonitorStop()
{
	CConfiguratorDoc *pDoc = (CConfiguratorDoc*)GetActiveDocument();
	if( pDoc != NULL )
	{
		if( pDoc->IsRecording())
		{
			if( AfxMessageBox( IDS_HISREC_STOP, MB_ICONQUESTION|MB_YESNO ) == IDYES )
			{
				pDoc->OnHistoryStop();
			}
		}
		else
		{
			DoStopMonitor();
			pDoc->UpdateAllViews(NULL, CConfiguratorDoc::HINT_REFRESH, NULL);    // Refresh all views
		}
	}
}

//-------------------------------------------------------------------------- 
void CMainFrame::DoMonitor( bool bIsRecording )
{
    //1. Check connected
	ASSERT( ModbusThread.get() && IsConnected());
	if((!ModbusThread.get()) || (!IsConnected()))
	{
		return;
	}

	//2. Build monitor modbus messages
	CConfiguratorDoc *pDoc = (CConfiguratorDoc*)GetActiveDocument();
	ASSERT( pDoc != NULL );
	modbus::CMessageRanges* pMonitorRanges = pDoc->m_aActions->find_name( L"Monitor" );
	std::auto_ptr<CAliasMap> pAliases;
	if( bIsRecording )
	{
		pAliases.reset( pDoc->GetHisRecordNonMonitorAliases());
	}

	ModbusThread->MonitorDevice( pMonitorRanges, pAliases.get());
	pAliases.reset();

	//3. Set Timer
	if( m_uThreadTimer == 0 )
	{
		 m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		 ASSERT( m_uThreadTimer != 0 ); 
	}

	//4. Resume Thread
	DWORD dwSuspendedCount = 0;
	do
	{
		dwSuspendedCount = CommThread->ResumeThread();
	    if( dwSuspendedCount < 0 )  // Error
		{
			break;
		}
		else if( dwSuspendedCount > 0 )
		{
            --dwSuspendedCount;
		}
	}while( dwSuspendedCount );
}

//-------------------------------------------------------------------------- 
void CMainFrame::DoStopMonitor( void )
{
    // Check connected
	ASSERT( ModbusThread.get() && IsConnected());
	if((!ModbusThread.get()) || (!IsConnected()))
	{
		return;
	}

	// Destroy Thread Timer if it has not been killed
	if( m_uThreadTimer != 0 )
	{
        KillTimer( m_uThreadTimer );     
		m_uThreadTimer = 0;
	}
    
	// Update thread
    if( CheckWritePtr( ModbusThread.get(), sizeof(modbus::CModbusThread)))
    {
		// Suspend
		g_bSuspend = true;        
		WaitForSingleObject( g_pThreadEvent->m_hObject, 1000 );
		::Sleep( 50 ); 

        ModbusThread->ClearMessages();
		ModbusThread->ResetThreadFlags();
    }

	// Update Status Bar
	UpdateStatusBar();
}

//==========================================================================
/// Action
// =========================================================================
// Function name: OnActionCalibrate
// Description  : DSC100
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.27 09:30:00
// =========================================================================
void CMainFrame::OnActionCalibrate()
{
	if((theApp.GetProductType() != CConfiguratorApp::DSC100) &&
	   (theApp.GetProductType() != CConfiguratorApp::DSC110))
	{
		return;
	}

	std::auto_ptr<CCalibrateBase> pCalibrate;
	switch( theApp.GetProductType())
	{
	    case CConfiguratorApp::DSC100:  // Modal
			pCalibrate.reset( new CDSC100Calibrate(this));
			CalSheet.reset( new CDSC100Sheet( IDS_CAL_CAPTION, pCalibrate.get()));
		break;

		case CConfiguratorApp::DSC110:  // Modal
			pCalibrate.reset( new CDSC110Calibrate());
			CalSheet.reset( new CDSC110Sheet( IDS_CAL_CAPTION, pCalibrate.get()));
		break;

		default:
			pCalibrate.reset( new CDSC100Calibrate(this));
			CalSheet.reset( new CDSC100Sheet( IDS_CAL_CAPTION, pCalibrate.get()));
	}

	if((NULL == pCalibrate.get()) ||
	   (NULL == CalSheet.get()))
	{
		return;
	}

	if( pCalibrate->CanCalibrate())
	{
		pCalibrate->Calibrate( CalSheet );
	}
}

// =========================================================================
// Function name: OnActionRun
// Description  : DSC100: Run
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.28 08:50:00
// =========================================================================
void CMainFrame::OnActionRun()
{
	if( theApp.GetProductType() != CConfiguratorApp::DSC100 )
	{
		return;
	}

	std::tr1::shared_ptr<CCalibrateBase> pCalibrate( new CDSC100Calibrate(this));
	if( pCalibrate.get())
	{
		pCalibrate->Run();
	}
}

// -------------------------------------------------------------------------
// Function name: SendCalibrationFlags
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.27 10:40:00
// -------------------------------------------------------------------------
void CMainFrame::SendCalibrationFlags( UINT uProType, bool bSave )
{
	if( ModbusThread.get() && IsConnected())
    {
		if( SUCCEEDED(ModbusThread->SendCalibrationFlags( uProType, bSave )))
		{
			// Mark the document as changed.
			((CConfiguratorDoc*)GetActiveDocument())->SetModifiedFlag(true);

			if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
			{ 
				// Force refresh in one second
				m_dwRefreshTicks = ::GetTickCount() + 1000;
			}

			// Set Timer
			if( m_uThreadTimer == 0 )
			{
				m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
				ASSERT( m_uThreadTimer != 0 ); 
			}
       
			// Resume Thread
			DWORD dwSuspendedCount = 0;
			do
			{
				dwSuspendedCount = CommThread->ResumeThread();
				if( dwSuspendedCount < 0 )  // Error
				{
					break;
				}
				else if( dwSuspendedCount > 0 )
				{
					--dwSuspendedCount;
				}
			}while( dwSuspendedCount );
		}//if
    }// if( IsConnected())
}

// =========================================================================
// Function name: IsOperationEnable
// Description  : 
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.28 10:00:00
// =========================================================================
bool CMainFrame::IsOperationEnable( void )
{
	if( !IsConnected())
    {
        return false;
    }

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
    if( !pDoc )
    {
		return false;
	}

    bool bOperationEnable = false;

    VARIANT_BOOL bRun = VARIANT_FALSE;
    VARIANT_BOOL bCal = VARIANT_FALSE;
    VARIANT_BOOL bCancel = VARIANT_FALSE;
    VARIANT_BOOL bCannotRun = VARIANT_FALSE;
    CAliasItem* pCalAlias = NULL;

    /// Get Calibration Flags: Run, IsCalibration, Cancel，CanRun
    // Run
    pCalAlias = pDoc->m_aAliases->find_name( L"kFlag_Run" );      // 2210
    if( CheckReadPtr(pCalAlias, sizeof(CAliasItem)))
    {
        bRun = pCalAlias->remBool();
    }

    // Calibration
    pCalAlias = pDoc->m_aAliases->find_name( L"kFlag_Cal" );      // 2207
    if( CheckReadPtr(pCalAlias, sizeof(CAliasItem)))
    {
        bCal = pCalAlias->remBool();
    }

    // Cancel
    pCalAlias = pDoc->m_aAliases->find_name( L"kFlag_Cancel" );    // 2208 
    if( CheckReadPtr(pCalAlias, sizeof(CAliasItem)))
    {
        bCancel = pCalAlias->remBool();
    }

    // CanRun
    pCalAlias = pDoc->m_aAliases->find_name( L"dCanRun" );         // 12065
    if (CheckWritePtr(pCalAlias, sizeof(CAliasItem)))
    {
        bCannotRun = pCalAlias->remBool();
    }

    // 上电连接后， m_bRun 0; m_bCal 0; m_bCancel 0. 判断 bCannotRun 标志位
    //                                                    true： Unable Retrieve, Send, Monitor, PopMenu Read/Write：
    //                                                    false: Enable Retrieve, Send, Monitor, PopMenu Read/Write：

    // 进行标定后，bCannotRun为false时，下面两种情况，未点击Run时： Retrieve, Send, Monitor，PopMenu Read/Write 命令无效
    //                                     正常标定完成：              m_bRun 0; m_bCal 1; m_bCancel 0
    //                                     标定时选择Cancel：          m_bRun 0; m_bCal 0; m_bCancel 1

    // 进行Send，PopMenu Write 后，要重新判断 bCannotRun标志位
    bOperationEnable = (!((bRun == 0) && (bCal || bCancel))) && (!bCannotRun);
    return bOperationEnable;
}

// =========================================================================
// Function name: GetPVAlgorithm
// Description  : 0, HiSelect
//                1, AI01 Only
//                2, AI02 Only
//                3, (AI01+AI02)/2
// Return type  : 
// Parameters   : 
// Author       : WangDong 2012.08.22 10:30:00
// =========================================================================
int CMainFrame::GetPVAlgorithm( void )
{
	int result = -1;

	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)GetActiveDocument();
	if( pDoc != NULL )
	{
		CAliasItem *pAlias = pDoc->m_aAliases->find_name( L"nPVSel" );
		if( pAlias != NULL )
		{
			result = pAlias->remI2();
		}
	}

	return result;
}

//==========================================================================
/// Help
// =========================================================================
// Function name: OnHelpTopics
// Description  : Just Run chm help file
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.14 19:50:00
// =========================================================================
void CMainFrame::OnHelpTopics()
{
	CString strHelpPathName( theApp.GetHelpFilePathName());
	//::ShellExecuteA( NULL, "open", CW2A((LPCTSTR)strHelpPathName ), NULL, NULL, SW_SHOW );
	
	// Display base topic
	::HtmlHelpW( ::GetDesktopWindow(),
			     strHelpPathName,
		  	     HH_DISPLAY_TOPIC,
				 NULL );
}

// -------------------------------------------------------------------------
// Function name: OnHelpInfo
// Description  : F1, Display specific help contents
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.20 10:17:00
// -------------------------------------------------------------------------
BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo)
{
	if( theApp.GetProductType() != CConfiguratorApp::DSC100 )
	{
		return CFrameWndEx::OnHelpInfo(pHelpInfo);
	}

	CString strHelpPathName( theApp.GetHelpFilePathName());
	
	HWND hwnd = NULL;
	DWORD dwHelpID = pHelpInfo->dwContextId;

	hwnd = ::HtmlHelpW( ::GetDesktopWindow(),
						strHelpPathName,
						HH_HELP_CONTEXT,  
						dwHelpID );

	if( hwnd == NULL )
	{
		// Log error info
		CString strInfo( L"CMainFrame::OnHelpInfo failed. " );
		strInfo.AppendFormat( L"HelpContextID not found: 0x%X", pHelpInfo->dwContextId );
		theApp.LogInfo( strInfo );

		// Display base topic
		hwnd = ::HtmlHelpW( ::GetDesktopWindow(),
						    strHelpPathName,
						    HH_DISPLAY_TOPIC,
						    NULL );
	}

	return CFrameWndEx::OnHelpInfo(pHelpInfo);
}

//==========================================================================
/// UPDATE_COMMAND_UI
// =========================================================================
// Function name: 
// Description  : File menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.28 12:00:00
// =========================================================================
void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = global::Security.IsValid();
		pCmdUI->Enable( bEnable );
	}
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = global::Security.IsValid();
		pCmdUI->Enable( bEnable );
	}
}

// =========================================================================
// Function name: 
// Description  : Device menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.10 11:35:00
// =========================================================================
void CMainFrame::OnUpdateDeviceLogout(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = global::Security.IsValid();
		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceSelAppType(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = (CConfiguratorApp::TS350 == theApp.GetProductType()) && 
			           global::Security.CanTune()                           && 
					   (!IsConnected());

		pCmdUI->Enable( bEnable );
	}
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceConnect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = global::Security.IsValid();
		pCmdUI->Enable( bEnable );

		pCmdUI->SetCheck( IsConnected());
	}
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceDisconnect(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		pCmdUI->Enable( IsConnected());
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceRetrieve(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = false;
		if( CheckReadPtr(ModbusThread.get(), sizeof(modbus::CModbusThread)))
		{
			bEnable = ( IsConnected() && 
				        global::Security.CanTune() &&
				        (ModbusThread->GetAction() != modbus::CModbusThread::mbsMONITOR));

			if( CConfiguratorApp::TS350 == theApp.GetProductType())
			{
				bEnable = bEnable && (!theApp.m_bAppMismatch);
			}
		}

		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceSend(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = false;
		if( CheckReadPtr(ModbusThread.get(), sizeof(modbus::CModbusThread)))
		{
		     bEnable = ( IsConnected() && 
				         global::Security.CanTune() &&
				         (ModbusThread->GetAction() != modbus::CModbusThread::mbsMONITOR));
		}

		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceCompare(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = true;
		if( CheckReadPtr(ModbusThread.get(), sizeof(modbus::CModbusThread)))
		{
		    bEnable = (ModbusThread->GetAction() == modbus::CModbusThread::mbsNONE);

			if( CConfiguratorApp::TS350 == theApp.GetProductType())
			{
				bEnable = bEnable && (!theApp.m_bAppMismatch);
			}
		}

		pCmdUI->Enable( bEnable );

		CSettingGridView* pView = (CSettingGridView*)theApp.RightViewVector[RSETTING_GRID];
		bool bCheck = pView->GetSafeHwnd() && 
			          pView->IsCompareColVisible();
		pCmdUI->SetCheck( bCheck );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateDeviceCommsettings(CCmdUI *pCmdUI)
{
    if( pCmdUI )
	{
		bool bEnable = (!IsConnected()) && (global::Security.IsValid());
		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateControllerCommsettings(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		BOOL bEnable = ((theApp.GetProductType() == CConfiguratorApp::TS350) &&
			            IsConnected()                                        && 
						global::Security.IsValid());

		if( CConfiguratorApp::TS350 == theApp.GetProductType())
		{
			bEnable = (bEnable && (!theApp.m_bAppMismatch));
		}

		pCmdUI->Enable( bEnable );
	}
}

// =========================================================================
// Function name: 
// Description  : Monitor menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.10 13:00:00
// =========================================================================
void CMainFrame::OnUpdateMonitorStart(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = false;
		if( CheckReadPtr(ModbusThread.get(), sizeof(modbus::CModbusThread)))
		{
			 bEnable = IsConnected() && 
				       (ModbusThread->GetAction() != modbus::CModbusThread::mbsMONITOR);

			 if( CConfiguratorApp::DSC100 == theApp.GetProductType())
			 {
				 bEnable = bEnable && IsOperationEnable();
			 }
			 else if( CConfiguratorApp::TS350 == theApp.GetProductType())
			 {
				 bEnable = bEnable && (!theApp.m_bAppMismatch);
			 }
		}

		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateMonitorStop(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = false;
		if( CheckReadPtr(ModbusThread.get(), sizeof(modbus::CModbusThread)))
		{
		     bEnable = (IsConnected() && (ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR));
		}

		pCmdUI->Enable( bEnable );
	}
}

// =========================================================================
// Function name: 
// Description  : History menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2013.04.10 11:10:00
// =========================================================================
void CMainFrame::OnUpdateRecordStart(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = (!IsRecording()) && IsConnected() && (ModbusThread->GetAction() != modbus::CModbusThread::mbsMONITOR);
		
		if( CConfiguratorApp::TS350 == theApp.GetProductType())
		{
			bEnable = bEnable && (!theApp.m_bAppMismatch);
		}

		pCmdUI->Enable( bEnable );
	}
}

//--------------------------------------------------------------------------
void CMainFrame::OnUpdateRecordStop(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = IsConnected() && IsRecording();
		pCmdUI->Enable( bEnable );
	}
}

// =========================================================================
// Function name: 
// Description  : View menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.03.10 11：35：00
// =========================================================================
void CMainFrame::OnUpdateDeviceTs350(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		if( CConfiguratorApp::TS350 == theApp.GetProductType() )
		{
            bCheck = true;
		}

		pCmdUI->SetCheck( bCheck );
		pCmdUI->Enable((!bCheck) && (!IsConnected()));   // If connected, can't change product type
	}
}

//--------------------------------------------------------------
void CMainFrame::OnUpdateDeviceDsc100(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		if( CConfiguratorApp::DSC100 == theApp.GetProductType() )
		{
            bCheck = true;
		}

		pCmdUI->SetCheck( bCheck );
		pCmdUI->Enable((!bCheck) && (!IsConnected()));  // If connected, can't change product type
	}
}

//--------------------------------------------------------------
void CMainFrame::OnUpdateDeviceDsc110(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		if( CConfiguratorApp::DSC110 == theApp.GetProductType() )
		{
            bCheck = true;
		}

		pCmdUI->SetCheck( bCheck );
		pCmdUI->Enable((!bCheck) && (!IsConnected()));  // If connected, can't change product type
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewLanguageChs(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		if( CConfiguratorApp::CHS == theApp.GetLanguageType())
		{
            bCheck = true;          
		}

		pCmdUI->SetCheck( bCheck );
		pCmdUI->Enable( !bCheck );

		if( IsConnected())
		{
            pCmdUI->Enable( false );     // If connected, can't change language type
		}
	}//if( pCmdUI )
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewLanguageEng(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		if( CConfiguratorApp::ENG == theApp.GetLanguageType())
		{
            bCheck = true;           
		}

		pCmdUI->SetCheck( bCheck );
		pCmdUI->Enable( !bCheck );

		if( IsConnected())
		{
            pCmdUI->Enable( false );     // If connected, can't change language type
		}
	}//if( pCmdUI )
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewConfig(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.RightViewVector[RSETTING_GRID];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 1));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewTrend(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.LeftViewVector[LTREND];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 0));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewTuning(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.LeftViewVector[LTUNING];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 0));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewAliases(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.RightViewVector[RALIASES];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 1));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewAlarms(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.LeftViewVector[LALARMS];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 0));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateViewHistory(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bCheck = false;
		CWnd* pWnd = theApp.LeftViewVector[LHISTORY];
		if( pWnd->GetSafeHwnd())
		{
			bCheck = (pWnd->GetDlgCtrlID() == m_vSplitter.IdFromRowCol(0, 0));
		}

		pCmdUI->SetCheck( bCheck );
	}
}

// =========================================================================
// Function name: OnUpdateActionCalibrate
// Description  : DSC100, DSC110, Action menu: Enable/Disable menu command
// Return type  : void
// Parameters   : 
// Author       : WangDong 2011.04.28 11：50：00
// =========================================================================
void CMainFrame::OnUpdateActionCalibrate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = (IsConnected() && 
			            ((CConfiguratorApp::DSC100 == theApp.GetProductType()) ||
					     (CConfiguratorApp::DSC110 == theApp.GetProductType())));
	    pCmdUI->Enable( bEnable );

		//pCmdUI->Enable( true );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateActionRun(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		bool bEnable = (IsConnected() && ( CConfiguratorApp::DSC100 == theApp.GetProductType()));
	    pCmdUI->Enable( bEnable );

		//pCmdUI->Enable( true );
	}
}

//-----------------------------------------------------------
void CMainFrame::OnUpdateHelpTopics(CCmdUI *pCmdUI)
{
	if( pCmdUI )
	{
		bool bEnable = ( CConfiguratorApp::DSC100 == theApp.GetProductType());
		pCmdUI->Enable( bEnable );
	}
}

// =========================================================================
// Function name: SendCTRLComSettings
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong  2012.01.13  10:00:00
// =========================================================================
HRESULT CMainFrame::SendCTRLComSettings( bool bDateTime )
{
    ASSERT( ModbusThread.get() && IsConnected());

	if((!ModbusThread.get()) || (!IsConnected()))
	{
		return S_OK;
	}

	// Update thread
	if( SUCCEEDED(ModbusThread->SendCtlCommSettings( bDateTime )))
    {
		// Mark the document as changed.
        ((CConfiguratorDoc*)GetActiveDocument())->SetModifiedFlag(true);

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}

		if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
	    { 
            m_dwRefreshTicks = ::GetTickCount() + 1000;  // Force refresh in one second
	    }
		else
		{
            ShowProgressDlg();                           // Display progress dialog if not monitoring
		}

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );
	}//if
	
	return S_OK;
}

// =========================================================================
// Function name: ReadAliasesFromDevice
// Description  : Addes the aliases in the map to the read action list 
//                in the modbus thread.
// Return type  : 
// Parameters   : The alias map containing the aliases to read.
// Author       : rjvs 2002-03-12 17:09:36 (UTC-6:00)
//                WangDong  2011.03.13  11:35:00
// =========================================================================
HRESULT CMainFrame::ReadAliasesFromDevice( CAliasMap* pAliases, bool bCalibrate )
{
	ASSERT( ModbusThread.get() && IsConnected());

	if((!ModbusThread.get()) ||
	   (!IsConnected())      || 
	   (!CheckReadPtr( pAliases, sizeof(CAliasMap))))
	{
		return S_OK;
	}

	if( !bCalibrate )
	{
		CString strRead, strCaption;
		strRead.LoadString( IDS_POPUPREAD_WARNING );
		strCaption.LoadString( IDS_WARNING );
		if( MessageBox( strRead, strCaption, MB_YESNO | MB_ICONWARNING ) != IDYES )
		{
			return S_FALSE;
		}
	}

	// Update thread
	if( SUCCEEDED( ModbusThread->ReadAliasesFromDevice(pAliases)))
	{
		// Mark the document as changed.
		((CConfiguratorDoc*)GetActiveDocument())->SetModifiedFlag(true);

		if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
		{
			m_dwRefreshTicks = ::GetTickCount() + 1000;   // Force refresh in one second
		}

		if( !bCalibrate && (ModbusThread->GetAction() != modbus::CModbusThread::mbsMONITOR))
		{
			ShowProgressDlg();  // Show progress dialog
		}

		// Set Timer
		if( m_uThreadTimer == 0 )
		{
			m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
			ASSERT( m_uThreadTimer != 0 ); 
		}

		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
			dwSuspendedCount = CommThread->ResumeThread();
			if( dwSuspendedCount < 0 )  // Error
			{
				break;
			}
			else if( dwSuspendedCount > 0 )
			{
				--dwSuspendedCount;
			}
		}while( dwSuspendedCount );
	}

    return S_OK;
}

// =========================================================================
// Function name: WriteAliasesToDevice
// Description  : Addes the aliases in the map to the write action list 
//                in the modbus thread.
// Return type  : 
// Parameters   : The alias map containing the aliases to write.
//                bCalibrate: true, Calibrating DSC100
//                bCalSave: true, Save Calibrate Flags
// Author       : rjvs 2002-03-12 17:09:36 (UTC-6:00)
//                WangDong  2011.03.13  13:30:00
// =========================================================================
HRESULT CMainFrame::WriteAliasesToDevice( CAliasMap* pAliases, bool bCalibrate, bool bTunning )
{
	ASSERT( ModbusThread.get() && IsConnected());

	if((!ModbusThread.get()) ||
	   (!IsConnected())      || 
	   (!CheckReadPtr( pAliases, sizeof(CAliasMap))))
	{
		return S_OK;
	}

	// Check if writing tuning aliases only
    bool bIsTuning = pAliases->IsTuning();

	bool bShowWriteWarning = true;
	if( bIsTuning )
	{
	    bShowWriteWarning = false;
	}
	else
	{
		// If TS350, Check if controller is running
		if( CConfiguratorApp::TS350 == theApp.GetProductType())
		{
			bool bIsMonitoring = (modbus::CModbusThread::mbsMONITOR == ModbusThread->GetAction());
            if( !bIsMonitoring )
            {
				DoMonitor(); 
            }

			// Wait for Modbus to finish one monitor scan cycle
			if( ModbusThread->HasEvent( 2000 ))
			{
                CAliasItem* pAlias = ((CConfiguratorDoc*)GetActiveDocument())->m_aAliases->find_name( L"rMBMODE" );  // 35129
                if( CheckWritePtr(pAlias, sizeof(CAliasItem)))
				{
                    bShowWriteWarning = (pAlias->valR4() > 1.0);
				}

				theApp.LogInfo( L"CMainFrame::WriteAliasesToDevice: Has Event." );

				// Stop monitoring if not previously monitoring
				if( !bIsMonitoring )
				{
					DoStopMonitor();
				}
			}	
		}// TS350
	}// else

	// Show warning (if necessary)
	if( !bCalibrate )
	{
		if( bShowWriteWarning )
		{
			CString strWarning, strCaption;
			strWarning.LoadString( IDS_SENDCFG_WARNING );
			strCaption.LoadString( IDS_WARNING );
			if( MessageBox( strWarning, strCaption, MB_OKCANCEL | MB_ICONWARNING ) != IDOK )
			{
				return S_FALSE;
			}
		}
		else if( !bIsTuning ) 
		{
			CString strWrite, strCaption;
			strWrite.LoadString( IDS_POPUPWRITE_WARNING );
			strCaption.LoadString( IDS_WARNING );
			if( MessageBox( strWrite, strCaption, MB_YESNO | MB_ICONWARNING ) != IDYES )
			{
				return S_FALSE;
			}
		}
	}

	// Update thread
	if( SUCCEEDED(ModbusThread->WriteAliasesToDevice( pAliases, bTunning )))
    {
		// Mark the document as changed.
        ((CConfiguratorDoc*)GetActiveDocument())->SetModifiedFlag(true);

		if( ModbusThread->GetAction() == modbus::CModbusThread::mbsMONITOR )
	    { 
            m_dwRefreshTicks = ::GetTickCount() + 1000;   // Force refresh in one second
	    }
		else // Display progress dialog if not monitoring
		{
			//if( !( bTunning && (CConfiguratorApp::TS350 == theApp.GetProductType())))
			if( !bTunning && !bCalibrate )
			{
				ShowProgressDlg();
			}
		}

        // Set Timer
		if( m_uThreadTimer == 0 )
		{
		    m_uThreadTimer = SetTimer( IDC_THREADTIMER, 100, NULL );
		    ASSERT( m_uThreadTimer != 0 ); 
		}
       
		// Resume Thread
		DWORD dwSuspendedCount = 0;
		do
		{
		    dwSuspendedCount = CommThread->ResumeThread();
		    if( dwSuspendedCount < 0 )  // Error
			{
			    break;
			}
		    else if( dwSuspendedCount > 0 )
		    {
                --dwSuspendedCount;
			}
		}while( dwSuspendedCount );
	}//if
	
	return S_OK;
}
