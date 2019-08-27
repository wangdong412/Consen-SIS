
// Configurator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Configurator.h"
#include "MainFrm.h"

#include "ConfiguratorDoc.h"
#include "ConfiguratorView.h"

// Added by wangdong begin
#include "Splash.h"                   // Splash screen
#include "resource.h"                 // LoadString   
#include "afxwin.h"
#include "VersionInfo.h"              // Versions in About Dialog
#include "SelAppTypeDlg.h"            // Select Application Type
#include "CommSettings.h"             // Communication Settings
#include "CalSheet.h"
#include "PasswordDlg.h"

//#include "HistoryRecordData.h"
//#include "HistoryRecordDatas.h"
//#include "HistoryFileData.h"
// Added by wangdong end

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern std::auto_ptr<CCommSettings> CommSettings; 
extern std::auto_ptr<CCalSheet>     CalSheet;

// CConfiguratorApp

BEGIN_MESSAGE_MAP(CConfiguratorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CConfiguratorApp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW, &CConfiguratorApp::OnFileNew)
	ON_COMMAND_EX_RANGE( ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CConfiguratorApp::OnOpenRecentFile)
	ON_COMMAND(IDM_DEVICE_SELAPPTYPE, &CConfiguratorApp::OnDeviceSelapptype)
    ON_COMMAND(IDM_VIEW_LANGUAGE_CHS, &CConfiguratorApp::OnViewLanguageChs)
    ON_COMMAND(IDM_VIEW_LANGUAGE_ENG, &CConfiguratorApp::OnViewLanguageEng)
    ON_COMMAND(IDM_DEVICE_DSC100, &CConfiguratorApp::OnDeviceDsc100)
    ON_COMMAND(IDM_DEVICE_TS350, &CConfiguratorApp::OnDeviceTs350)
	ON_COMMAND(IDM_DEVICE_DSC110, &CConfiguratorApp::OnDeviceDsc110)
	ON_COMMAND(IDM_LOGLEVEL_ALL, &CConfiguratorApp::OnLoglevelAll)
	ON_COMMAND(IDM_LOGLEVEL_ERR, &CConfiguratorApp::OnLoglevelErr)
	ON_COMMAND(IDM_LOGLEVEL_OPR, &CConfiguratorApp::OnLoglevelOpr)
	ON_UPDATE_COMMAND_UI(IDM_LOGLEVEL_ALL, &CConfiguratorApp::OnUpdateLoglevelAll)
	ON_UPDATE_COMMAND_UI(IDM_LOGLEVEL_ERR, &CConfiguratorApp::OnUpdateLoglevelErr)
	ON_UPDATE_COMMAND_UI(IDM_LOGLEVEL_OPR, &CConfiguratorApp::OnUpdateLoglevelOpr)
END_MESSAGE_MAP()


// CConfiguratorApp construction

CConfiguratorApp::CConfiguratorApp()
	             :m_hLangDLL( NULL ),
				  m_hEngLangDLL( NULL ),
				  m_uLanguageType( CHS ),
				  m_uProductType( DSC100 ),
				  m_nProductNumber( 100 ),
				  m_nCurrentCode( 0 ),
				  m_fXMLVersion( 0.0 ),
				  m_nAppType( 1 ),
				  m_uLogLevel( LOG_OPR ),
				  m_bAccessRedraw( false ),
				  m_bResetViewFlag( false ),
				  m_bAppMismatch( false ),
				  m_pVersionCTL( new CVersionCtl())
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Configurator.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	LeftViewVector.clear();
	RightViewVector.clear();
}

// The one and only CConfiguratorApp object

CConfiguratorApp theApp;


// CConfiguratorApp initialization

BOOL CConfiguratorApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Tri-Sen"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Added by wangdong: 此项保持菜单栏的可视属性（0x0000 0001即可视），如果不删除，则切换语言时，菜单栏不能正常切换
	HKEY hAppKey = GetAppRegistryKey();  // HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName
	if( hAppKey != NULL )                //                              Tri-Sen     Configurator
	{
		long lResult = -1;
		HKEY key;

		// Menu
        lResult = RegOpenKey( hAppKey, _T("Workspace\\BasePane-593980"), &key);
		if( lResult == ERROR_SUCCESS )
		{
		    lResult = RegDeleteKey( hAppKey, _T("Workspace\\BasePane-593980"));
		    ASSERT( lResult == ERROR_SUCCESS );
		}

		// Standard ToolBar
		lResult = RegOpenKey( hAppKey, _T("Workspace\\MFCToolBar-59392"), &key);
		if( lResult == ERROR_SUCCESS )
		{
		    lResult = RegDeleteKey( hAppKey, _T("Workspace\\MFCToolBar-59392"));
		    ASSERT( lResult == ERROR_SUCCESS );
		}

		// View ToolBar
		lResult = RegOpenKey( hAppKey, _T("Workspace\\MFCToolBar-1101"), &key);
		if( lResult == ERROR_SUCCESS )
		{
		    lResult = RegDeleteKey( hAppKey, _T("Workspace\\MFCToolBar-1101"));
		    ASSERT( lResult == ERROR_SUCCESS );
		}

		RegCloseKey(hAppKey);
	}

	/// Version control
	/// DSC100: xml 1.0  code 980,981;   xml 1.1  code 982
	// xml 1.0: code, 980,981.982
	vermaps *pvers = new vermaps();
	float fver = 1.0f;
	CVersion *pver = new CVersion( fver );
	pver->Add( 980 );
	pver->Add( 981 );
	pver->Add( 982 );
	pvers->insert( make_pair( fver, pver ));

	// xml 1.1: code, 1000
	fver = 1.1f;
	pver = new CVersion( fver );
	pver->Add( 1000 );
	pvers->insert( make_pair( fver, pver ));

	m_pVersionCTL->Add( DSC100, pvers );

	// TS350: xml 1.0 code 1;
	pvers = new vermaps();
	fver = 1.0f;
	pver = new CVersion( fver );
	pver->Add( 10 );
	pvers->insert( make_pair( fver, pver ));

	m_pVersionCTL->Add( TS350, pvers );

	// DSC110: xml 1.0 code 1;
	pvers = new vermaps();
	fver = 1.0f;
	pver = new CVersion( fver );
	pver->Add( 10 );
	pvers->insert( make_pair( fver, pver ));

	m_pVersionCTL->Add( DSC110, pvers );
	
	// Get product type from the registry, default DSC100
	m_uProductType = GetProfileInt( L"SETTINGS", L"PRODUCT_TYPE", DSC100 );
	SetProductNumberByType();

	CString strProductRegPath( GetPathAccordingToProduct( m_uProductType ));

	// Update current code and xml version
	m_nCurrentCode = GetProfileInt( strProductRegPath, L"FIRM_CODE", 0 );
	if( 0 == m_nCurrentCode ) //not exist
	{
		m_nCurrentCode = m_pVersionCTL->GetNewestCode( m_uProductType );
		ASSERT( m_nCurrentCode != -1 );
	}

	m_fXMLVersion = m_pVersionCTL->GetXMLVersionFromCode( m_uProductType, m_nCurrentCode );
	ASSERT( fabs(m_fXMLVersion) > EPSILON_FLT );

	// Get Default Resource: English
	m_hEngLangDLL = AfxGetResourceHandle();

	/// Load language dll according to the product's language type
	// Get the product's language type by producttype 
	m_uLanguageType = GetLanguageByProduct( m_uProductType );
	if( ENG != m_uLanguageType )    // Default: English(not resource file)
	{
		switch( m_uLanguageType )
		{
			case CHS:        // Chinese
				m_hLangDLL = AfxLoadLibrary( _T("Configurator_Chs.dll"));
				ASSERT( m_hLangDLL );
				if( m_hLangDLL )
				{
					AfxSetResourceHandle( m_hLangDLL );
				}
				else
				{
					LogInfo( _T("Language_Chs.dll has not been found!"));
					return FALSE;
				}
			break;
			default:  // Default English
			break;
		}//switch
	}//if( ENG != m_uLanguageType )

    /// Grid Size
	// SettingGrid Size
	global::SettingGridColWidths[0] = 5000;
	global::SettingGridColWidths[1] = 1200;
	global::SettingGridColWidths[2] = 1200;
	global::SettingGridColWidths[3] = 1200;
	global::SettingGridColWidths[4] = 1200;
	global::SettingGridColWidths[5] = 2400;
	global::SettingGridColWidths[6] = 2400;

	CString strGrid = strProductRegPath + L"\\Grid Window";
	for( int i = 0; i < global::SETTING_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		global::SettingGridColWidths[i] = GetProfileInt( strGrid, strCol, global::SettingGridColWidths[i] );
	}

	// Alias Grid Size
	global::TagsGridColWidths[0] = 5000;
	global::TagsGridColWidths[1] = 1700;
	global::TagsGridColWidths[2] = 1200;
	global::TagsGridColWidths[3] = 1200;
	global::TagsGridColWidths[4] = 1400;
	global::TagsGridColWidths[4] = 1200;

	strGrid = strProductRegPath + L"\\Alias Window";
	for( int i = 0; i < global::TAGS_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		global::TagsGridColWidths[i] = GetProfileInt( strGrid, strCol, global::TagsGridColWidths[i] );
	}

	// History Grid Size
	global::HisGridColWidths[0] = 5000;
	global::HisGridColWidths[1] = 1700;
	global::HisGridColWidths[2] = 1200;
	global::HisGridColWidths[3] = 1200;

	strGrid = strProductRegPath + L"\\History Window";
	for( int i = 0; i < global::HISTORY_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		global::HisGridColWidths[i] = GetProfileInt( strGrid, strCol, global::HisGridColWidths[i] );
	}

	// Font
	CString strFont = strProductRegPath + L"\\Font";
	global::FontName = GetProfileStringW( strFont, L"Font Name", global::FontName );
	global::FontSize = (float)GetProfileIntW( strFont, L"Font Size", static_cast<int>(global::FontSize));

	/// Log file
	// Get log level stored in the registry
	m_uLogLevel = GetProfileInt( GetPathAccordingToProduct( m_uProductType ), L"LOG_LEVEL", LOG_OPR );

	// Rename existing log files
	global::RenameLogFiles();

	// Open new log file, and log User name and Computer name
	CString strLogFileName;
	strLogFileName.Format( L"%s\\%s%s", GetFilePath(), global::LOG_FILE_NAME, global::LOG_FILE_EXT ); 
	m_strLogFile = strLogFileName;
	m_osLogFile.open( m_strLogFile );
	if( m_osLogFile )
	{
	    LogInfo( L"Opening log file" );

		CString str;
		wchar_t w[MAX_PATH];
		memset( w, 0, sizeof(w));
		DWORD dwSize = MAX_PATH;

		// User Name
		if( ::GetUserNameW( w, &dwSize ))      //  in characters, include null
		{
		    str.Format( L"User: %s", w );
		    LogInfo( str );
		}

		// Computer Name
		str.Empty();
		memset( w, 0, sizeof(w));
		dwSize = MAX_PATH;
		if( ::GetComputerNameW( w, &dwSize ))  //  in characters, not include null
		{
		    str.Format( L"Machine: %s", w );
		    LogInfo( str );
		}
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CConfiguratorDoc),
		RUNTIME_CLASS(CMainFrame),           // main SDI frame window
		RUNTIME_CLASS(CConfiguratorView));
	if(!pDocTemplate)
	{
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);

	// Relate File to Configurator Program
	RegisterFileandProgram();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))   
	{
		return FALSE;
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CConfiguratorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	/// Write Registry
	CString strProductRegPath( GetPathAccordingToProduct( m_uProductType ));

	// Product Type
	WriteProfileInt( L"SETTINGS", L"PRODUCT_TYPE", m_uProductType );

	// Log Level
	WriteProfileInt( strProductRegPath, L"LOG_LEVEL", m_uLogLevel );

	// Firm code
	WriteProfileInt( strProductRegPath, L"FIRM_CODE", m_nCurrentCode );

	// SettingGrid Size
	CString strGrid = strProductRegPath + L"\\Grid Window";
	for( int i = 0; i < global::SETTING_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		WriteProfileInt( strGrid, strCol, global::SettingGridColWidths[i] );
	}

	// Alias Grid Size
	strGrid = strProductRegPath + L"\\Alias Window";
	for( int i = 0; i < global::TAGS_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		WriteProfileInt( strGrid, strCol, global::TagsGridColWidths[i] );
	}

	// History Grid Size
	strGrid = strProductRegPath + L"\\History Window";
	for( int i = 0; i < global::HISTORY_GRID_COLUMNS; i++ )
	{
		CString strCol;
		strCol.Format( L"ColWidth %d", i );
		WriteProfileInt( strGrid, strCol, global::HisGridColWidths[i] );
	}

	// Font
	CString strFont = strProductRegPath + L"\\Font";
	WriteProfileString( strFont, L"Font Name", global::FontName );
	WriteProfileInt( strFont, L"Font Size", static_cast<int>(global::FontSize));

	/// Log info and close log file
	if( m_osLogFile )
	{
	    LogInfo( L"Closing log file" );  
	}
	m_osLogFile.close();

	/// Free DLL library
	if( m_hLangDLL )
	{
	    AfxFreeLibrary( m_hLangDLL );
	}

	if( m_hEngLangDLL )
	{
		AfxFreeLibrary( m_hEngLangDLL );
	}

	// Clear View Vector
	LeftViewVector.clear();
	RightViewVector.clear();

	// Global font
	if( global::g_Font.m_hObject )
	{
		global::g_Font.DeleteObject();
	}

	SAFE_DELETE( m_pVersionCTL );

	return CWinAppEx::ExitInstance();
}

// =======================================================================
// Function name  : LogInfo
// Description    :                
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.11 13:00:00
// =======================================================================
void CConfiguratorApp::LogInfo( CString strInfo )
{
	if( !m_osLogFile )
	{
		return;		
	}

	// Get Current time: Year-Month-Day Hour:Minite:Second.milliseconds
	SYSTEMTIME tm;
	::GetLocalTime( &tm );
    COleDateTime time( tm );

	CString strLogTime = time.Format( L"%Y-%m-%d %H:%M:%S" );
	CString strMillSec;
	strMillSec.Format( L".%03d", tm.wMilliseconds );
	strLogTime += strMillSec;

	CString strLog( strLogTime + L" | " + strInfo + L"\r\n" );
	m_osLogFile.imbue( std::locale("chs"));
	m_osLogFile.write( strLog, strLog.GetLength());
	m_osLogFile.flush();
}

// =======================================================================
// Function name  : SetProductInfo
// Description    : Product Number and Type, Application Type
//                  No need to check, when double-click a file to start the app, 
//                  we need to update these two parameters with file's information                 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
void CConfiguratorApp::SetProductInfo( int nProductNumber, int nAppType )
{
	if((nProductNumber != m_nProductNumber) || (nAppType != m_nAppType))
	{
		m_bResetViewFlag = true;

		if( nAppType != m_nAppType )
		{
			SetAppType( nAppType );
		}

		if( nProductNumber != m_nProductNumber )
		{
		    m_nProductNumber = nProductNumber;   // Product ID
		    SetProductTypeByNumber();            // Set Product type according to Number
		}
	}
}

// ------------------------------------------------------------------------
// Function name  : SetProductNumberByType / SetProductTypeByNumber
// Description    : Make product number and type synchronous           
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// -------------------------------------------------------------------------
void CConfiguratorApp::SetProductNumberByType()
{
	// Set m_nProductNumber according to m_uProductType 
    switch( m_uProductType )
	{
		case DSC100:
		    m_nProductNumber = 100;
		break;

		case TS350:
		    m_nProductNumber = 350;
		break;

		case DSC110:
		    m_nProductNumber = 110;
		break;

		default:
			m_nProductNumber = 100;
	}
}

//--------------------------------------------------------------------------
// Set m_uProductType according to m_nProductNumber
void CConfiguratorApp::SetProductTypeByNumber()
{
	switch( m_nProductNumber )
	{
		case 100:
		    m_uProductType = DSC100;
		break;

		case 350:
			m_uProductType = TS350;
		break;

		case 110:
		    m_uProductType = DSC110;
		break;

		default:
			m_uProductType = DSC100;
	}
}

// =======================================================================
// Function name  : GetLanguageByProduct
// Description    : Get language type according to the current product type
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
CString CConfiguratorApp::GetPathAccordingToProduct( UINT uProductType )
{
    CString strPath( L"SETTINGS\\" );
	switch( uProductType )
	{
		case DSC100:
			strPath += L"DSC100";
	    break;
		case TS350:
			strPath += L"TS350";
	    break;
		case DSC110:
			strPath += L"DSC110";
	    break;
		default:
			strPath += L"DSC100";
	}

	return strPath;
}

// =======================================================================
// Function name  : GetLanguageByProduct
// Description    : Get language type according to the current product type
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
UINT CConfiguratorApp::GetLanguageByProduct( UINT uProductType )
{
	CString strPath( GetPathAccordingToProduct( uProductType ));

	return GetProfileInt( strPath, L"LANGUAGE_CFG", ENG );
}

// =======================================================================
// Function name  : GetProductName
// Description    : 
// Return type    : product name, such as "DSC100"
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
CString CConfiguratorApp::GetProductName()
{
	CString strProductName;
    switch( m_uProductType )
	{
		case DSC100:
			strProductName = L"DSC100";
	    break;
		case TS350:
			strProductName = L"TS350";
		break;
		case DSC110:
			strProductName = L"DSC110";
	    break;
		default:
			strProductName = L"DSC100";
	}

	return strProductName;
}

// =======================================================================
// Function name  : GetProductTypeByName
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
UINT CConfiguratorApp::GetProductTypeByName( CString strName )
{
	UINT uProductType = DSC100;

	if( strName.CompareNoCase( L"DSC100" ) == 0 )
	{
        uProductType = DSC100;
	}
	else if( strName.CompareNoCase( L"TS350" ) == 0 )
	{
        uProductType = TS350;
	}
	else if( strName.CompareNoCase( L"DSC110" ) == 0 )
	{
        uProductType = DSC110;
	}
	else
	{
		uProductType = DSC100;
	}

	return uProductType;
}

// =======================================================================
// Function name  : GetXMLFileName
// Description    : TS320: AppType 1: Gen320; 2: Mech320
//                  Others: such as DSC100, T121, TS312...
// Return type    : xml file name, such as "DSC100_Eng.xml", "DSC100_Chs.xml"
// Parameters     : 
// Author         : WangDong 2011.04.15 11:30:00
// =======================================================================
CString CConfiguratorApp::GetXMLFileName( bool bForSave )
{
	/// Exe file Path
	CString strPath( GetFilePath());                  

	/// XML File Name
	CString strProductName( GetProductName());          // such as "DSC100"
	CString strXMLFileName( strProductName );

	if( bForSave )   // not include language info
	{
        strXMLFileName +=  L".xml";
		return strXMLFileName;
	}
	else
	{
	    switch( m_uLanguageType )
	    {
	        case ENG:
	            strXMLFileName += L".xml";
		    break;

		    case CHS:
		        strXMLFileName += L"_Chs.xml";
		    break;
		
		    default:
		        strXMLFileName += L".xml";
	    }//switch
	}
	
	/// Full path xml file name
	CString strResult;
	strResult.Format( L"%s\\%s\\%.1f\\%s", strPath, strProductName, m_fXMLVersion, strXMLFileName );
	return strResult;
}

// =======================================================================
// Function name  : GetHelpFilePathName
// Description    : 
// Return type    : product name, such as "DSC100", "TS320"
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
CString CConfiguratorApp::GetHelpFilePathName()
{
	// Exe file Path
	CString strPath( GetFilePath());

	// Help Name: such as "DSC100.chm"
	CString strProductName( GetProductName()), strHelpName( strProductName );     // such as "DSC100"

	switch( m_uLanguageType )
	{
	    case ENG:
	        strHelpName += L".chm";
		break;

		case CHS:
		    strHelpName += L"_Chs.chm";
		break;
		
		default:
		    strHelpName += L".chm";
	}//switch

	// Help File Path
	CString strResult;
	strResult.Format( L"%s\\%s\\%.1f\\%s", strPath, strProductName, m_fXMLVersion, strHelpName );

	return strResult;
}

// =======================================================================
// Function name  : GetDefTraceName
// Description    : 
// Return type    : product name, such as "DSC100", "TS320"
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
void CConfiguratorApp::GetDefTraceName( CString& strTraceName1, CString& strTraceName2, CString& strTraceName3 )
{
    switch( m_uProductType )
	{
		case DSC100:
			strTraceName1 = L"iAPI1P";
			strTraceName2 = L"iSPP";
			strTraceName3 = L"iAO1P";
	    break;	
		case TS350:
			strTraceName1 = L"rVALVEV1";
			strTraceName2 = L"rVALVEV2";
			strTraceName3 = L"SP5_OUT";
	    break;	
	    case DSC110:
			strTraceName1 = L"iPV01AngP";
			strTraceName2 = L"iPV02AngP";
			strTraceName3 = L"iSelSP";
	    break;
		default:  // DSC100
			strTraceName1 = L"iAPI1P";
			strTraceName2 = L"iSPP";
			strTraceName3 = L"iAO1P";
	} 
}

// =======================================================================
// Function name  : GetProductName
// Description    : 
// Return type    : product name, such as "DSC100", "TS320"
// Parameters     : 
// Author         : WangDong 2011.01.19 10:30:00
// =======================================================================
CString CConfiguratorApp::GetCommMessage( int nModbusType )
{
	CString str;
	UINT uMsgID = 0;
	if( nModbusType != 0 )    // Modbus Tcp
	{
		uMsgID = IDS_TCP_COMMMSG;
	}
	else
	{
		switch( m_uProductType )
		{
			case DSC100:
				uMsgID = IDS_DSC100_SERMSG;
			break;	

			default:  // Serial communication message
				uMsgID = IDS_SERIAL_COMMMSG;
		} 
	}

	str.LoadStringW( uMsgID );
	return str;
}

// =======================================================================
// Function name  : SetAppType
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2011.12.13 15:00:00
// =======================================================================
void CConfiguratorApp::SetAppType( int nAppType )                 
{
	if( m_nAppType != nAppType )
	{
        /// Reset Compare Information
		//1 Default Compare Type: controller
		m_uCmpType = CMP_REM;

		//2 Reset compare file path name
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if( pMainFrame )
		{
			CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pMainFrame->GetActiveDocument();
			if( pDoc )
			{
				pDoc->ResetCmpFilePathName();

				// History
				pDoc->SetRecordModified( false );   // Reset RecordModified Flag
				pDoc->ResetHistory();               // Reset History Ctrls, Datas and Clear history views
			}
		}

		m_bResetViewFlag = true;
		m_nAppType = nAppType; 
	}
}

// CConfiguratorApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:;
	CStatic m_stcCfgName;
	CStatic m_stcWarning;
	CListBox m_listVersion;

	virtual BOOL OnInitDialog();

private:
    CFont m_font;

	CImage m_imgLogo;
	CImage m_imgImage;
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

CAboutDlg::~CAboutDlg()
{
	m_imgLogo.ReleaseGDIPlus();
	m_imgImage.ReleaseGDIPlus();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CRWARNING, m_stcWarning);
	DDX_Control(pDX, IDC_STC_CFGNAME, m_stcCfgName);
	DDX_Control(pDX, IDC_LIST_VERSION, m_listVersion);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/// Font of "Configurator"
	CClientDC dc(this);
	LOGFONT lf;
	memset( &lf, 0, sizeof(LOGFONT));
	lf.lfHeight = ::MulDiv( 18, -dc.GetDeviceCaps( LOGPIXELSY ), 72 ); 
	lf.lfWeight = 700;
	::StrCpyNW( lf.lfFaceName, global::FontName, 32 );
    if( m_font.CreateFontIndirectW( &lf ))
	{
	    m_stcCfgName.SetFont( &m_font );
	}

	/// Copy Right Warning
	CString strWarning;
	if( strWarning.LoadStringW( IDS_COPYRIGHT_WARNING ))
	{
	    m_stcWarning.SetWindowTextW( strWarning );
	}

	/// Version
	std::vector<LPCTSTR> files;
	files.push_back( L"Configurator.exe" );
	files.push_back( L"vsflex8n.ocx" );
	files.push_back( L"TeeChart8.ocx" );
	files.push_back( L"hhctrl.ocx" );
	files.push_back( L"advapi32.dll" );
	files.push_back( L"comctl32.dll" );
	files.push_back( L"comdlg32.dll" );
	files.push_back( L"gdi32.dll" );
	files.push_back( L"kernel32.dll" );
	files.push_back( L"mpr.dll" );
	files.push_back( L"ole32.dll" );
	files.push_back( L"oleaut32.dll" );
	files.push_back( L"shell32.dll" );
	files.push_back( L"user32.dll" );
	files.push_back( L"version.dll" );
	files.push_back( L"wsock32.dll" );

	CVersionInfo v;
	for( size_t i = 0; i < files.size(); i++ )
	{
		CString strFileName = files[i];
		CString strVersion( strFileName );

		if( v.GetVersionInfo( strFileName ) == S_OK )
		{
            strVersion += L" " + v.FileVersion();
		}
		else
		{
			strVersion += L" !Error!";
		}

		if( ::GetModuleHandleW( strFileName ) == NULL )
		{
			if( theApp.GetLogLevel() >= CConfiguratorApp::LOG_ALL )
			{
				CString str;
				str.Format( L"Dependency %s not loaded. (GetLastError()=%d)", strFileName, ::GetLastError());
				theApp.LogInfo( str );
			}
		}

		m_listVersion.AddString( strVersion );
	}//for

	/* Load Logo
	m_imgLogo.LoadFromResource( theApp.m_hInstance, IDB_LOGO );
	m_imgImage.LoadFromResource( theApp.m_hInstance, IDB_IMAGE );*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	// Check if bitmaps are currently loaded.
	if( m_imgLogo.IsNull() ||
		m_imgImage.IsNull())
	{
		return;
	}

	if( CImage::IsTransparencySupported())
	{
        m_imgLogo.SetTransparentColor( RGB(255,255,255));   // white
		m_imgImage.SetTransparentColor( RGB(255,255,255));  
	}

	// CImage::TransparentBlt: 4，8 bit source bitmaps with transparency 
	// CImage::AlphaBlend：    32 bits-per-pixel bitmaps with transparency
	CRect rect;
	GetClientRect( &rect );
	m_imgLogo.Draw( dc.GetSafeHdc(), 10, 10 );
	m_imgImage.Draw( dc.GetSafeHdc(), rect.right - m_imgImage.GetWidth() - 10, 10 );
}

// App command to run the dialog
void CConfiguratorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CConfiguratorApp customization load/save methods

void CConfiguratorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CConfiguratorApp::LoadCustomState()
{
}

void CConfiguratorApp::SaveCustomState()
{
}

// CConfiguratorApp message handlers

// Added by wangdong for Splash screen
BOOL CConfiguratorApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	// CG: The following lines were added by the Splash Screen component.	    
    if (CSplashWnd::PreTranslateAppMessage(pMsg))
	{
        return TRUE;
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

void CConfiguratorApp::OnUpdateLoglevelErr(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		if( m_uLogLevel == LOG_ERR )
		{
			pCmdUI->SetCheck();
		}
	}
}

void CConfiguratorApp::OnUpdateLoglevelOpr(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		if( m_uLogLevel == LOG_OPR )
		{
			pCmdUI->SetCheck();
		}
	}
}

void CConfiguratorApp::OnUpdateLoglevelAll(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if( pCmdUI )
	{
		if( m_uLogLevel == LOG_ALL )
		{
			pCmdUI->SetCheck();
		}
	}
}


void CConfiguratorApp::OnLoglevelErr()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	str.Format( L"Log level changed to: %s", L"Error" );
	LogInfo( str );

	m_uLogLevel = LOG_ERR;
}

void CConfiguratorApp::OnLoglevelOpr()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	str.Format( L"Log level changed to: %s", L"Error and Operation" );
	LogInfo( str );

	m_uLogLevel = LOG_OPR;
}

void CConfiguratorApp::OnLoglevelAll()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	str.Format( L"Log level changed to: %s", L"All" );
	LogInfo( str );

	m_uLogLevel = LOG_ALL;
}

// =======================================================================
// Function name  : SwitchLanguageType
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2012.01.04 10:00:00
// =======================================================================
void CConfiguratorApp::SwitchLanguageType( UINT uLanguageType, bool bFromOpenMenuCommand, bool bSwitchPd )
{
	switch( uLanguageType )
	{
	    case ENG:
			ASSERT( m_hEngLangDLL != NULL );
			AfxSetResourceHandle( m_hEngLangDLL );
		break;
		case CHS:
			if( m_hEngLangDLL == NULL )
			{
				m_hEngLangDLL = AfxGetResourceHandle();   // English Resource
			}

			m_hLangDLL = AfxLoadLibrary( _T("Configurator_Chs.dll"));
			ASSERT( m_hLangDLL != NULL );
			if( m_hLangDLL != NULL )
			{
				AfxSetResourceHandle( m_hLangDLL );
			}
		break;
		default:
			ASSERT( m_hEngLangDLL != NULL );
			AfxSetResourceHandle( m_hEngLangDLL );
		break;
	}// switch

	m_uLanguageType = uLanguageType;

	// Set the product's language type to registry
	CString strPath = GetPathAccordingToProduct( m_uProductType );
	WriteProfileInt( strPath, L"LANGUAGE_CFG", m_uLanguageType );

	// Reset Menu and ToolBar
	((CMainFrame*)m_pMainWnd)->m_wndMenuBar.ResetAll();
	((CMainFrame*)m_pMainWnd)->m_wndMenuBar.SetRecentlyUsedMenus( FALSE );

	// Set View ToolBar Text
	std::vector<CString> strVector;
	CString strToolBar;
	for( size_t i = IDS_TOOL_CFG; i <= IDS_TOOL_ALARM; i++ )
	{
		strToolBar.LoadStringW( i );
		strVector.push_back( strToolBar );
	}

	((CMainFrame*)m_pMainWnd)->m_viewToolBar.SetToolBarBtnText( 0, strVector.at( 0 ));   
	((CMainFrame*)m_pMainWnd)->m_viewToolBar.SetToolBarBtnText( 1, strVector.at( 1 ));
	((CMainFrame*)m_pMainWnd)->m_viewToolBar.SetToolBarBtnText( 2, strVector.at( 2 ));
	((CMainFrame*)m_pMainWnd)->m_viewToolBar.SetToolBarBtnText( 3, strVector.at( 3 ));
	((CMainFrame*)m_pMainWnd)->m_viewToolBar.SetToolBarBtnText( 4, strVector.at( 4 ));
	((CMainFrame*)m_pMainWnd)->m_viewToolBar.AdjustSizeImmediate();

	if( !bFromOpenMenuCommand )
	{
		// Set m_bResetViewFlag Flag
		if( !m_bResetViewFlag )
		{
		    m_bResetViewFlag = true;
		}

		// New or Open Document
		CString strTitle(((CMainFrame*)m_pMainWnd)->GetActiveDocument()->GetTitle());
		int index = strTitle.ReverseFind( L'.' );
		if( index == -1 )
		{
			if( TS350 == m_uProductType )
			{
				// When only switch laguage, New don't need to select application type
				// When switch product, need to select application type
				m_bMenuSelFlag = !bSwitchPd;
			}

			OnFileNew();   // New Document
			m_bMenuSelFlag = false;
		}
		else
		{
		    CString strName( strTitle.Right( strTitle.GetLength() - index - 1 ));
			if( strName.CompareNoCase( GetProductName()) == 0 )
			{
			    m_pDocManager->GetBestTemplate( strTitle )->OpenDocumentFile( strTitle, TRUE, TRUE );
			}
			else
			{
				if( TS350 == m_uProductType )
				{
				    m_bMenuSelFlag = true;    // When New, don't need to select application type  
				}

                OnFileNew();   // New Document
				m_bMenuSelFlag = false;
			}
		}
	}
}

// =======================================================================
// Function name  : OnViewLanguageChs
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2012.01.04 10:00:00
// =======================================================================
void CConfiguratorApp::OnViewLanguageChs()
{
    // TODO: 在此添加命令处理程序代码
	ASSERT( m_uLanguageType != CHS );
	SwitchLanguageType( CHS );
}

// =======================================================================
// Function name  : OnViewLanguageEng
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2012.01.04 10:15:00
// =======================================================================
void CConfiguratorApp::OnViewLanguageEng()
{
	// TODO: 在此添加命令处理程序代码
	ASSERT( m_uLanguageType != ENG );
	SwitchLanguageType( ENG );
}

// =======================================================================
// Function name  : SwitchProductType
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2011.04.14 15:00:00
// =======================================================================
void CConfiguratorApp::SwitchProductType( UINT uProductType, bool bFromOpenMenuCommand )
{
	// If not TS350, Update title, delete application type information
	if((uProductType != TS350) && AfxGetMainWnd()->GetSafeHwnd())
	{
		((CMainFrame*)AfxGetMainWnd())->UpdateTitle( CMainFrame::TITLE_NONE );
	}

	try
	{
		CString strPreProduct( GetProductName());      // Get product name before switch

		//1. Set m_bResetViewFlag
		m_bResetViewFlag = true;
	
		//2. Write registry in order to save product information
		CString strProductRegPath( GetPathAccordingToProduct( m_uProductType ));
	
		//2.1 Product Type
		WriteProfileInt( L"SETTINGS", L"PRODUCT_TYPE", m_uProductType );

		//2.2 Log Level
		WriteProfileInt( strProductRegPath, L"LOG_LEVEL", m_uLogLevel );

		//2.3 Language Type
		WriteProfileInt( strProductRegPath, L"LANGUAGE_CFG", m_uLanguageType );

		//2.5 SettingGrid Size
		CString strGrid = strProductRegPath + L"\\Grid Window";
		for( int i = 0; i < global::SETTING_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			WriteProfileInt( strGrid, strCol, global::SettingGridColWidths[i] );
		}

		//2.6 Alias Grid Size
		strGrid = strProductRegPath + L"\\Alias Window";
		for( int i = 0; i < global::TAGS_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			WriteProfileInt( strGrid, strCol, global::TagsGridColWidths[i] );
		}

		//2.7 History Grid Size
		strGrid = strProductRegPath + L"\\History Window";
		for( int i = 0; i < global::HISTORY_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			WriteProfileInt( strGrid, strCol, global::HisGridColWidths[i] );
		}

		//2.8 Font
		CString strFont = strProductRegPath + L"\\Font";
		WriteProfileString( strFont, L"Font Name", global::FontName );
		WriteProfileInt( strFont, L"Font Size", static_cast<int>(global::FontSize));

		//2.8 Calibrate sheet
		CalSheet.reset();

		//2.9 History
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		if( pMainFrame )
		{
			CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pMainFrame->GetActiveDocument();
			if( pDoc )
			{
				pDoc->SetRecordModified( false );   //2.9.1 Reset RecordModified Flag
				pDoc->ResetHistory();               //2.9.2 Reset History Ctrls, Datas and Clear history views
			}
		}

		//3. Switch Product Type
		m_uProductType = uProductType;
		SetProductNumberByType();
		CString strTargetProduct( GetProductName());   // Get Target Product Name

		//4. Application Type: If TS350，User will select it in CConfiguratorDoc::OnNewDocument;
		//                     else, Set default value 1.
		if((m_uProductType != TS350) && (m_nAppType != 1))
		{
			m_nAppType = 1;
		}

		//5. Read from registry to get target product informatio
		strProductRegPath = GetPathAccordingToProduct( m_uProductType );

		//5.1 SettingGrid Size
		strGrid = strProductRegPath + L"\\Grid Window";
		for( int i = 0; i < global::SETTING_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			global::SettingGridColWidths[i] = GetProfileInt( strGrid, strCol, 0 );
		}

		//5.2 Alias Grid Size
		strGrid = strProductRegPath + L"\\Alias Window";
		for( int i = 0; i < global::TAGS_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			global::TagsGridColWidths[i] = GetProfileInt( strGrid, strCol, 0 );
		}

		//5.3 History Grid Size
		strGrid = strProductRegPath + L"\\History Window";
		for( int i = 0; i < global::HISTORY_GRID_COLUMNS; i++ )
		{
			CString strCol;
			strCol.Format( L"ColWidth %d", i );
			global::HisGridColWidths[i] = GetProfileInt( strGrid, strCol, 0 );
		}

		//5.4 Font
		strFont = strProductRegPath + L"\\Font";
		global::FontName = GetProfileStringW( strFont, L"Font Name", global::FontName );
		global::FontSize = (float)GetProfileIntW( strFont, L"Font Size", static_cast<int>(global::FontSize));

		//5.5 Comm Settings
		if( CommSettings.get())
		{
			CommSettings->ReadRegistry();
		}

		//5.6 Log Level
		m_uLogLevel = GetProfileInt( GetPathAccordingToProduct( m_uProductType ), L"LOG_LEVEL", LOG_OPR );

		//6. Compare
		//6.1 Default Compare Type: controller
		m_uCmpType = CMP_REM;

		//6.2 Reset compare file path name
		if( pMainFrame )
		{
			CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pMainFrame->GetActiveDocument();
			if( pDoc )
			{
				pDoc->ResetCmpFilePathName();
			}
		}

		if( !bFromOpenMenuCommand )
		{
			//7 Language Type
			if( m_uLanguageType != GetProfileInt( GetPathAccordingToProduct( m_uProductType ), L"LANGUAGE_CFG", ENG ))
			{
				m_uLanguageType = GetProfileInt( GetPathAccordingToProduct( m_uProductType ), L"LANGUAGE_CFG", ENG ); 

				// Switch Language
				SwitchLanguageType( m_uLanguageType, bFromOpenMenuCommand, true );
			}
			else
			{
			    OnFileNew();   //8. New Document
			}
		}

		//9. Log
		CString str;
		str.Format( L"Converting product from %s to %s.", strPreProduct, strTargetProduct );
		LogInfo( str );
	}//try
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CConfiguratorApp::SwitchProductType!" );
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CConfiguratorApp::SwitchProductType!" );
	}
}

void CConfiguratorApp::OnDeviceDsc100()
{
	// TODO: 在此添加命令处理程序代码
	DeviceSwitch( DSC100 );
}


void CConfiguratorApp::OnDeviceTs350()
{
	// TODO: 在此添加命令处理程序代码
	DeviceSwitch( TS350 );
}


void CConfiguratorApp::OnDeviceDsc110()
{
	// TODO: 在此添加命令处理程序代码
	DeviceSwitch( DSC110 );
}

void CConfiguratorApp::DeviceSwitch( const UINT uNewProduct )
{
	if( m_uProductType != uNewProduct )
	{
		// Save code for the ori product
		CString strProductRegPath( GetPathAccordingToProduct( m_uProductType ));
		WriteProfileInt( strProductRegPath, L"FIRM_CODE", m_nCurrentCode );

		// New Product
		strProductRegPath = GetPathAccordingToProduct( uNewProduct );
		int code = GetProfileInt( strProductRegPath, L"FIRM_CODE", 0 );
		if( code != 0 )
		{
			m_nCurrentCode = code;
		}
		else
		{
			m_nCurrentCode = GetNewestCode( uNewProduct );
		}

		m_fXMLVersion = GetXMLVersionFromCode( uNewProduct, m_nCurrentCode );

		SwitchProductType( uNewProduct );
	}
}

// ====================================================================
// Function name: OnFileNew
// Description  : 1. DoDisconnected 
//                2. Call CWinApp::OnFileNew
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.30 10:30:00
// =====================================================================
void CConfiguratorApp::OnFileNew()
{
	try
	{
		if( m_pMainWnd->GetSafeHwnd())
		{
			CMainFrame* pFrame = (CMainFrame*)m_pMainWnd;
			CConfiguratorDoc* pDoc = (CConfiguratorDoc*)pFrame->GetActiveDocument();
			ASSERT( pDoc != NULL );
			if( !pDoc->IsStoppingHistoryRecord())
			{
				return;
			}

			pFrame->DoDisconnect();  // Do Disconnected
		}

		
		// Call CWinApp::OnFileNew
		CWinAppEx::OnFileNew();     // When m_pDocManager is not NULL, m_pDocManager->OnFileNew()

		// Access level: Enter password
		if( !global::Security.CanTune())  // None or ReadOnly
		{
			std::auto_ptr<CPasswordDlg> dlg( new CPasswordDlg());
			dlg->DoModal();

			// Refresh all views
			if(((CMainFrame*)AfxGetMainWnd() != NULL) && (((CMainFrame*)m_pMainWnd)->GetActiveDocument() != NULL))
			{
				((CMainFrame*)m_pMainWnd)->GetActiveDocument()->UpdateAllViews( NULL, CConfiguratorDoc::HINT_REFRESH );
			}
		}

		// Reset m_bResetViewFlag flag
		if( m_bResetViewFlag )
		{
			m_bResetViewFlag = false;
		}
	}
	catch( CException &e )
	{
		e.ReportError();
		e.Delete();

		theApp.LogInfo( L"Exception occured in CConfiguratorApp::OnFileNew!" );
	}
	catch(...)
	{
		theApp.LogInfo( L"Unknown exception occured in CConfiguratorApp::OnFileNew!" );
	}
}

// ====================================================================
// Function name: OnOpenRecentFile
// Description  : 1. DoDisconnected 
//                2. Call CWinApp::OnOpenRecentFile
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.03.30 14:40:00
// =====================================================================
BOOL CConfiguratorApp::OnOpenRecentFile( UINT nID )
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT( pFrame->GetSafeHwnd());
	CConfiguratorDoc* pDoc = (CConfiguratorDoc*)(pFrame->GetActiveDocument());
	ASSERT( pDoc != NULL );
	if( !pDoc->IsStoppingHistoryRecord())
	{
		return TRUE;
	}

    pFrame->DoDisconnect();

	return CWinAppEx::OnOpenRecentFile( nID );
}

// ====================================================================
// Function name: OnDeviceSelapptype
// Description  : IDOK: if not same, show warning, if choose yes, do new operation
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.11.04 11:30:00
// =====================================================================
void CConfiguratorApp::OnDeviceSelapptype()
{
	// TODO: 在此添加命令处理程序代码
	ASSERT( TS350 == m_uProductType );
	std::auto_ptr<CSelAppTypeDlg> dlg ( new CSelAppTypeDlg());
	if( dlg.get() && (dlg->DoModal() == IDOK))
	{
		int nAppType = -1;
		switch( dlg->m_nIndex )   
		{
			case 0:   // Gen
				nAppType = CConfiguratorDoc::APPTYPE_GEN;
			break;
			case 1:   // Mech
				nAppType = CConfiguratorDoc::APPTYPE_MECH;
			break;
			default:
				nAppType = CConfiguratorDoc::APPTYPE_GEN;
		}

		// Set Application Type
		if( nAppType != GetAppType())
		{
			// Show Warning
			if( AfxMessageBox( IDS_APPTYPE_CHANGE, MB_ICONEXCLAMATION | MB_YESNO) == IDYES )
			{
				m_bMenuSelFlag = true;
				SetAppType( nAppType );
				OnFileNew();
				m_bMenuSelFlag = false;
			}
		}
	}// IDOK
}

// ====================================================================
// Function name: CanMatch
// Description  : Check if Configurator can match the product and xml file
// Return type  : 0: OK;
//                1: Product mismatch
//                2: xml mismatch
// Parameters   : 
// Author       : WangDong 2011.11.04 11:30:00
// =====================================================================
int CConfiguratorApp::CanMatch( UINT uProductType, float fXMLVersion )
{
	ASSERT( m_pVersionCTL != NULL );
	if( NULL == m_pVersionCTL )
	{
		return PRODUCT_MISMATCH;
	}

	return m_pVersionCTL->CanMatch( uProductType, fXMLVersion );
}

// ===============================================================
// Function name  : IsXMLMatchCode
// Description    : Check if the current xml file can match the code
// Return         : true, xml can match the code;
// Parameters     : type:    Product type, such as DSC100
//                  fversion:XML file version to be checked
//                  code:    Firmware code
// Author         : WangDong 2012.08.07 14:00:00
// ===============================================================
bool CConfiguratorApp::IsXMLMatchCode( const UINT uProductType, const float fXMLVersion, const int nCode ) const
{
	ASSERT( m_pVersionCTL != NULL );
	if( NULL == m_pVersionCTL )
	{
		return false;
	}

	return m_pVersionCTL->IsXMLMatchCode( uProductType, fXMLVersion, nCode );
}

// ===============================================================
// Function name  : GetNewestCode
// Description    : 
// Return         : >0: The newest firmware code; others -1
// Parameters     : uProductType: Product type, such as DSC100
// Author         : WangDong 2012.08.08 13:30:00
// ===============================================================
int CConfiguratorApp::GetNewestCode( const UINT uProductType )
{
	ASSERT( m_pVersionCTL != NULL );
	if( NULL == m_pVersionCTL )
	{
		return -1;
	}

	return m_pVersionCTL->GetNewestCode( uProductType );
}

// ===============================================================
// Function name  : GetNewestCodeFromXML
// Description    : 
// Return         : newest code of the xml file if success, others -1
// Parameters     : type: Product type, such as DSC100
//                  fversion: xml file version
// Author         : WangDong 2012.08.07 13:00:00
// ===============================================================
int CConfiguratorApp::GetNewestCodeFromXML( const UINT type, const float fversion )
{
	ASSERT( m_pVersionCTL != NULL );
	if( NULL == m_pVersionCTL )
	{
		return -1;
	}

	return m_pVersionCTL->GetNewestCodeFromXML( type, fversion );
}

// ===============================================================
// Function name  : GetXMLVersionFromCode
// Description    : 
// Return         : xml file version if success, others 0.0
// Parameters     : type: Product type, such as DSC100
//                  code: Firmware code
// Author         : WangDong 2012.08.08 13:00:00
// ===============================================================
float CConfiguratorApp::GetXMLVersionFromCode( const UINT type, const int code )
{
	ASSERT( m_pVersionCTL != NULL );
	if( NULL == m_pVersionCTL )
	{
		return 0.0;
	}

	return m_pVersionCTL->GetXMLVersionFromCode( type, code );
}












//=====================================================================
/// Global Functions
// ====================================================================
// Function name: RegSetRelated
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.04.29 09:50:00
// =====================================================================
BOOL RegSetRelated( CString& strExt, CString& strCommand, CString& strDefaultIcon )
{
	if( strExt.IsEmpty() || strCommand.IsEmpty() || strDefaultIcon.IsEmpty())
	{
		return FALSE;
	}

	// Such as: .DSC100      Configurator.Document
	LPCTSTR lpData = L"Configurator.Document";
	RegSetValueW( HKEY_CLASSES_ROOT, strExt, REG_SZ, lpData, ::wcslen( lpData ));           // not include a null terminator

	CString strSubKey;
	strSubKey.Format( L"%s%s", strExt, L"\\shell\\open\\command" );
	RegSetValueW( HKEY_CLASSES_ROOT, strSubKey, REG_SZ, strCommand, strCommand.GetLength()); 

	/// Configurator.Document
	//  Default Icon
	strSubKey.Format( L"%s\\%s", lpData, L"DefaultIcon" );
	RegSetValueW( HKEY_CLASSES_ROOT, strSubKey, REG_SZ, strDefaultIcon, strDefaultIcon.GetLength());

	// shell
	strSubKey.Format( L"%s%s", lpData, L"\\shell\\open\\command" );
	RegSetValueW( HKEY_CLASSES_ROOT, strSubKey, REG_SZ, strCommand, strCommand.GetLength()); 

	return TRUE;
}

// ====================================================================
// Function name: RegisterFileandProgram
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2011.04.29 09:50:00
// =====================================================================
void RegisterFileandProgram()
{
	// File Extention
    std::vector<LPCTSTR> FileExtension;
	FileExtension.push_back( L".DSC100" );
	FileExtension.push_back( L".TS350" );
	FileExtension.push_back( L".DSC110" );
	
	// Get exe path
	wchar_t exepath[MAX_PATH];     
	memset( exepath, 0, sizeof(exepath));     
	GetModuleFileNameW( NULL, exepath, sizeof(exepath));   

	CString strCommand( exepath ); 
	strCommand += L" \"%1\"";       // C:\Program Files\Tri-Sen\Configurator\Configurator.exe "%1"

	CString strDefaultIcon( exepath );
	strDefaultIcon += L",1";         // C:\Program Files\Tri-Sen\Configurator\Configurator.exe,1

	// Set File Extention
	std::vector<LPCTSTR>::iterator it = FileExtension.begin();
	std::vector<LPCTSTR>::iterator eit = FileExtension.end();
	while( it != eit )
	{
		CString strExt( *it );
        RegSetRelated( strExt, strCommand, strDefaultIcon );

		it++;
	}

	FileExtension.clear();
}

// ====================================================================
// Function name: RegisterFileandProgram
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong  2011.04.11 15:30:00
// =====================================================================
CString GetFilePath()
{
	wchar_t exepath[MAX_PATH];    
	CString strdir, tmpdir;     
	memset( exepath, 0, sizeof(exepath));     
	GetModuleFileNameW( NULL, exepath, sizeof(exepath));   
    tmpdir = exepath;     
	strdir = tmpdir.Left( tmpdir.ReverseFind( L'\\' )); 
	return strdir; 
}

// =======================================================================
// Function name  : ValidateHisFile
// Description    : Check product type and if file exists
//                  Note: NULL is valid
// Return type    : bool
// Parameters     : 
// Author         : WangDong 2013.04.12 17:15:00
// =======================================================================
UINT ValidateHisFile( const CString strHisFileName )
{
	if( strHisFileName.IsEmpty())
	{
		return E_FILE_EMPTY;
	}

	// File Name: *-%s-*.xml
	// Ext: xml
	CString str( strHisFileName.Right(3));
	if( str.CompareNoCase( L"xml" ) != 0 )
	{
		return E_FILE_EXT;
	}

	// Product Name
	int last = strHisFileName.ReverseFind( '-' );
	if( -1 == last )
	{
		return E_FILE_FORMAT;
	}

	str = strHisFileName.Left( last );             // *-%s
	int index = str.ReverseFind( '-' );
	if( -1 == index )
	{
		return E_FILE_FORMAT;
	}

	str = str.Right( str.GetLength() - index -1 ); // %s 
	if( str.CompareNoCase( theApp.GetProductName()) != 0 )
	{
		return E_FILE_PRODUCT;
	}

	// File Exists
	if( !::PathFileExistsW( strHisFileName ))
	{
		return E_FILE_EXIST;
	}

	return E_NONE;
}

// ====================================================================
// Function name: GetEFileString
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : wangdong  2011.04.11 15:30:00
// =====================================================================
CString GetEHisFileString( UINT nType )
{
	CString strFileError;

	switch( nType )
	{
	    case E_NONE:
			strFileError = L"None";
		break;
		case E_FILE_EMPTY:
			strFileError = L"File name empty";
        break;
		case E_FILE_EXT:
			strFileError = L"File ext is not xml";
        break;
		case E_FILE_FORMAT:
			strFileError = L"Format error";
        break;
		case E_FILE_PRODUCT:
			strFileError = L"Product type error";
        break;
		case E_FILE_EXIST:
			strFileError = L"File not exists";
        break;
		default:
			strFileError = L"Others";
		break;
	}

	return strFileError;
}

// =======================================================================
// Function name  : IsHisFileSameArray
// Description    : 判断两历史文件是否属于同一序列：文件路径名中除序列号不同外，其他均相同;
//                  即启动记录的时间相同，只是写满后，增加了序列号
// Return type    : 
// Parameters     :
// Author         : WangDong 2013.04.17 16:00:00
// =======================================================================
bool IsHisFileSameArray(const CString strName1, const CString strName2)
{
	if((ValidateHisFile(strName1) != E_NONE) || (ValidateHisFile(strName2) != E_NONE))
	{
		return false;
	}

	// *-%s-*.xml
	int index = strName1.ReverseFind( '-' );
	CString str1( strName1.Left(index));

	index = strName2.ReverseFind( '-' );
	CString str2( strName2.Left(index));
	
	bool bResult = str1.CompareNoCase( str2 ) == 0;
	return bResult;
}

// =======================================================================
// Function name  : GetArrayCode
// Description    : 获取文件序列号
// Return type    : 文件无效时，返回-1；有效时，返回文件序列号
// Parameters     : 
// Author         : WangDong 2013.04.17 16:00:00
// =======================================================================
int GetHisFileArrayCode(const CString strHisFileName)
{
	if( ValidateHisFile(strHisFileName) != E_NONE )
	{
		return -1;
	}

	// *-%s-*.xml
	int index = strHisFileName.Find( '.' );
	CString str( strHisFileName.Left(index));

	// *-%s-* 
	index = str.ReverseFind( '-' );
	str = str.Right( str.GetLength() - index -1 );

	int code = -1;
	::swscanf_s( str, L"%d", &code );

	return code;
}

void VariantToRecordString( const VARIANT& v, CString& s )
{
    switch( V_VT( &v ))
	{
	    case VT_BOOL:
			s = ( V_BOOL(&v)? L"1" : L"0" );
        break;

		case VT_I2:      // short
			s.Format( L"%d", V_I2(&v));
		break;

		case VT_UI2:     // USHORT
			s.Format( L"%d", V_UI2(&v));
		break;
			
		case VT_R4:
			s.Format( L"%.2f", V_R4(&v));
		break;

		case VT_I4:      // long
		case VT_UI4:
			s.Format( L"%d", V_I4(&v));
		break;

        default:
			s = L"";
	}
}

void VariantToString(const VARIANT& v, CString& s, UINT nHexType)
{
    switch( V_VT( &v ))
	{
	    case VT_BOOL:
			s = ( V_BOOL( &v )? L"True" : L"False");
        break;

		case VT_I2:      // short
			if( TYPE_BIN == nHexType )
			{
			    s.Format( L"%d", V_I2(&v));
			}
			else
			{
				s.Format( L"0x%04X", V_I2(&v));
			}
		break;

		case VT_UI2:     // USHORT
			if( TYPE_BIN == nHexType )
			{
				s.Format( L"%d", V_UI2(&v));
			}
			else
			{
				s.Format( L"0x%04X", V_UI2(&v));
			}
		break;
			
		case VT_R4:
			if( CConfiguratorApp::DSC110 == theApp.GetProductType())
			{
				s.Format( L"%.3f", V_R4(&v));
			}
			else
			{
				s.Format( L"%.2f", V_R4(&v));
			}
		break;

		case VT_I4:      // long
		case VT_UI4:
			if( TYPE_BIN == nHexType )
			{
				s.Format( L"%d", V_I4(&v));
			}
			else
			{
				s.Format( L"0x%04X", V_I4(&v));
			}
		break;

		case (VT_I1 | VT_BYREF): 
        case (VT_UI1 | VT_BYREF):
          if (V_I1REF(&v) == NULL)
		  {
              s = L"";
		  }
          else
		  {
              s = V_I1REF(&v);
		  }
        break;

        default:
			ASSERT( false );
			s = L"";
	}
}

//========================================================================
// Function name  : PointTwipsToPixels
// Description    : Convert twips to pixels
// Return type    : void
// Parameters     : 
// Author         : rjvs 2002-03-14 16:43:03 (UTC-6:00)
// =======================================================================
void PointTwipsToPixels(int x, int y, CPoint& pt)
{
	HDC hdc = ::CreateIC( TEXT("DISPLAY"), NULL, NULL, NULL );
	int pix = ::GetDeviceCaps( hdc, LOGPIXELSX );
	pt.x = ::MulDiv( x, pix, 1440 );
	pix = ::GetDeviceCaps( hdc, LOGPIXELSY );
	pt.y = ::MulDiv( y, pix, 1440 );
	::DeleteDC( hdc );
}

//========================================================================
// Function name  : EncodeTimeSec
// Description    : Time/seconds function
// Return type    : COleDateTime
// Parameters     : ASeconds
// Author         : rjvs 
//                  wangdong 
// =======================================================================
COleDateTime EncodeTimeSec( double ASeconds )
{
	if( ASeconds < 0.0 )
	{
        ASeconds = 0.0;
	}

	// Days
	WORD d = static_cast<WORD>(ASeconds / 86400.0);

	// Hours
	WORD h = static_cast<WORD>(static_cast<int>(ASeconds / 3600.0) % 24);

	// Minutes
	WORD m = static_cast<WORD>(static_cast<int>(ASeconds / 60.0) % 60);

	// Seconds
	WORD s = static_cast<WORD>(static_cast<int>(ASeconds) % 60);

	// Milliseconds
	WORD z = static_cast<WORD>((ASeconds - static_cast<int>(ASeconds)) * 1000.0);

	SYSTEMTIME ts;
	ts.wYear = 1899;
	ts.wMonth = 12;
	ts.wDay = 30;
	ts.wDayOfWeek = 6;
	ts.wHour = h;
	ts.wMinute = m;
	ts.wSecond = s;
	ts.wMilliseconds = 0;
    COleDateTime Result( ts );

	double dt = 1.0/(24*3600*1000);        // the double value represent by 1ms 
	Result.m_dt += (d + z*dt);

	return Result;
}

/// <summary>
///   Returns an error message from Windows or from executable.</summary>
/// <created>
///   rjvs 2002-03-08 11:05:46 (UTC-6:00)
///   wangdong 2011-02-28 18:50:00
/// </created>
CString HrGetMessage( HRESULT hr )
{
	//  Check the facility
	CString strMessage;
	if( HRESULT_FACILITY(hr) == FACILITY_MODBUS )  // User defined mosbus code
	{
		strMessage.LoadStringW( HRESULT_CODE(hr)); 
	}
    else
    {
		LPVOID lpMsgBuf;
		
        DWORD c = ::FormatMessage(
                  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER ,
                  NULL,
                  hr,
                  NULL,
                  (LPWSTR)&lpMsgBuf,
                  0,
                  NULL );

		strMessage = (LPWSTR)lpMsgBuf;

		LocalFree(lpMsgBuf);
    }

	return strMessage;
}

//========================================================================
// Function name  : GetSubKeyString
// Description    : L"software\\tri-sen\\configurator\\settings" + L"\\product type"
//                  such as TS320, DSC100
// Return type    : void
// Parameters     : 
// Author         : Wangdong 2011-03-02 14:30:00 (UTC-6:00)
// =======================================================================
CString GetSubKeyString()
{
    CString strSubKey( L"software\\tri-sen\\configurator\\settings" );

	switch( theApp.GetProductType())
	{
		case CConfiguratorApp::DSC100:
            strSubKey += L"\\DSC100";
		break;
	    case CConfiguratorApp::TS350:
			strSubKey += L"\\TS350";
		break;
		case CConfiguratorApp::DSC110:
			strSubKey += L"\\DSC110";
		break;
		default:
			strSubKey += L"\\DSC100";
	}

	return strSubKey;
}

//========================================================================
// Function name  : DumpBytes
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : Wangdong 2011-03-08 14:10:00 (UTC-6:00)
// =======================================================================
CString DumpBytes( const BYTE* Buffer, int Count )
{
    CString strDataBytes;

    if( !CheckReadPtr( Buffer, Count*sizeof(BYTE)))
    {
        return strDataBytes;
    }

    for( int i = 0; i < Count; ++i )
    {
	    CString s;
	    if( i == 0 )
	    {
		    s.Format( L"%02X", *Buffer );
	    }
	    else
	    {
            s.Format( L" %02X", *Buffer );    
	    }
	  
	    strDataBytes += s;
        ++Buffer;
    }

    return strDataBytes;
}

//==========================================================================
/// global parameters
// Only for TS350：
bool g_bCfgRefresh = false;    // 改变配置项时，Configuration左视图节点，Tuning视图左视图的PID项的可视属性可能需要动态刷新
bool g_bReadRefresh = false;   // Retrieve操作或者Aliases视图中读操作引起的刷新

/// global namespace
namespace global
{
	CCfgSecurity Security;

	// font
	float FontSize = 10.0;
	CString FontName(L"Arial");
	CFont g_Font;

	// Column Counts
	std::vector<int> SettingGridColWidths( SETTING_GRID_COLUMNS, 0 );
	std::vector<int> TagsGridColWidths( TAGS_GRID_COLUMNS, 0 );
	std::vector<int> HisGridColWidths( HISTORY_GRID_COLUMNS, 0 );

	// Trace Colors
	const long TraceColors[8] = { clRed, clGreen, clBlue, clGray, clMaroon, clFuchsia, clTeal, clLBlue /*clYellow clHiPurple clNavy*/ };

	// Rename Existing log files
	void RenameLogFiles(void)
    {
        CString strExePath = GetFilePath();
		strExePath += L"\\" + LOG_FILE_NAME;              // ...Configurator

		for( int i = LOG_FILE_COUNT; i > 0; --i )
		{
			 // Rename existing file
			 CString strLogName;
			 if( i > 1 )
			 {
				 strLogName.Format( L"%s-%d%s", strExePath, i-1, LOG_FILE_EXT ); 
			 }
			 else
			 {
				 strLogName.Format( L"%s%s", strExePath, LOG_FILE_EXT ); 
			 }

			 if( ::PathFileExistsW( strLogName ))
			 {
			     if( i == LOG_FILE_COUNT )
			     {
					 ::DeleteFileW( strLogName );
			     }
				 else
				 {
					 CString strNewName;
					 strNewName.Format( L"%s-%d%s", strExePath, i, LOG_FILE_EXT );

					 const char* pOld = CW2A(strLogName);
					 const char* pNew = CW2A(strNewName);
					 ::rename( CW2A(strLogName), CW2A(strNewName));
				 }
			 }//if exists
		}//for
    }

}//namespace
//----------------------------------------------------------------
