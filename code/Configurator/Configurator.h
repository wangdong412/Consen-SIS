
// Configurator.h : main header file for the Configurator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"           // main symbols

// Added by wangdong
#include "hlp\Device_Help.h"    // HelpID
#include <fstream>
#include "CfgSecurity.h"
#include "VersionCtl.h"

// Added by wangdong for view switch 
#ifndef VIEWVECTOR
#define VIEWVECTOR
using namespace std;
typedef vector<CView*> ViewVector;
#endif

// CConfiguratorApp:
// See Configurator.cpp for the implementation of this class
//

class CConfiguratorApp : public CWinAppEx
{
public:
	CConfiguratorApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnFileNew();
	afx_msg BOOL OnOpenRecentFile( UINT nID );
	afx_msg void OnDeviceSelapptype();
	afx_msg void OnDeviceDsc100();
	afx_msg void OnDeviceTs350();
	afx_msg void OnDeviceDsc110();
	afx_msg void OnLoglevelAll();
	afx_msg void OnLoglevelErr();
	afx_msg void OnLoglevelOpr();
	afx_msg void OnAppAbout();

	afx_msg void OnUpdateLoglevelAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLoglevelErr(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLoglevelOpr(CCmdUI *pCmdUI);

	afx_msg void OnViewLanguageChs();
	afx_msg void OnViewLanguageEng();

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	UINT m_uLanguageType;               // Language Type
	UINT m_uProductType;                // Product Type
	int m_nProductNumber;               // Product Number
	int m_nCurrentCode;                 // Current code
	float m_fXMLVersion;                // XML file version
	int m_nAppType;                     // Application Type
	bool m_bResetViewFlag;         // Product type, Application type, Language type or XML version
	                                    // Set true when change;
	                                    // Reset it when finish new operation
	bool m_bMenuSelFlag;                // When Select Application Type from menu, set it true,
	                                    // when finish,reset it 

	HINSTANCE m_hLangDLL;               // DLL
	HINSTANCE m_hEngLangDLL;            // English DLL

	wofstream m_osLogFile;
	UINT m_uLogLevel;
	CString m_strLogFile;

	UINT m_uCmpType;
	bool m_bAccessRedraw;               // SettingTreeView 
	CVersionCtl *m_pVersionCTL;

	void DeviceSwitch( const UINT uNewProduct );

public:
	enum eLanguageType    { ENG, CHS };                                // Language Type
	enum eProductType     { DSC100, TS350, DSC110 };                   // Product Type  
	enum eLogLevel        { LOG_ERR, LOG_OPR, LOG_ALL  };              // Log Level
	enum eCmpType         { CMP_REM, CMP_FILE };                       // Compare Type
	enum eMatch           { MATCH, PRODUCT_MISMATCH, XML_MISMATCH };
	enum ePVSel           { PV_HISEL, PV_AI01, PV_AI02, PV_AVERAGE };

	bool m_bAppMismatch;

	bool IsAccessRedraw()                          const { return m_bAccessRedraw; }
	void SetAccessRedraw( bool value )             { m_bAccessRedraw = value; }   

	void LogInfo( CString strInfo );
	UINT GetLogLevel()                              const { return m_uLogLevel; }

	UINT GetCmpType()                               const { return m_uCmpType; }
	void SetCmpType( UINT value )                   { m_uCmpType = value; }

	/// Set
	void SetLanguageType( UINT uLanguageType )      { m_uLanguageType = uLanguageType; }
	void SetProductType( UINT uProductType )        
	{ 
		m_uProductType = uProductType;
		SetProductNumberByType();
	}

	void SetAppType( int nAppType );

	void SetResetViewFlag( bool value )             { m_bResetViewFlag = value; }
	void SetMenuSelFlag( bool value )               { m_bMenuSelFlag = value; }

	/// Get
	UINT GetLanguageType()                          const { return m_uLanguageType; }
	UINT GetProductType()                           const { return m_uProductType; }
	int  GetProductNumber()                         const { return m_nProductNumber; }
	int  GetAppType()                               const { return m_nAppType; } 
	bool GetResetViewFlag()                         const { return m_bResetViewFlag; }
	bool GetMenuSelFlag()                           const { return m_bMenuSelFlag; }

	/// Set Product info
	void SetProductInfo( int nProductNumber, int nAppType );
	void GetProductInfo( int &nProductNumber, int &nAppType, float &fVersion ) const
	{ 
		nProductNumber = m_nProductNumber;
		nAppType = m_nAppType; 
		fVersion = m_fXMLVersion;
	}
	void SetProductNumberByType();
	void SetProductTypeByNumber();

	CString GetPathAccordingToProduct( UINT uProductType );
	UINT GetLanguageByProduct( UINT uProductType );
	CString GetProductName();
	UINT GetProductTypeByName( CString strName );
	CString GetXMLFileName( bool bForSave = false );
	CString GetHelpFilePathName();
	void GetDefTraceName( CString& strTraceName1, CString& strTraceName2, CString& strTraceName3 );
	CString GetCommMessage( int nModbusType = 0 );
	
	ViewVector LeftViewVector;
    ViewVector RightViewVector;	

	void SwitchProductType( UINT uProductType, bool bFromOpenMenuCommand = false ); 
	void SwitchLanguageType( UINT uLanguageType, bool bFromOpenMenuCommand = false, bool bSwitchPdt = false ); 

	// Current code
	void SetCurrentCode( int code )        { m_nCurrentCode = code; }
	int GetCurrentCode()                   const { return m_nCurrentCode; }

	// XML file version
	void SetXMLVersion( float fversion )   { m_fXMLVersion = fversion; }
	float GetXMLVersion()                   const { return m_fXMLVersion; }

	int CanMatch( UINT uProductType, float fXMLVersion );
	bool IsXMLMatchCode( const UINT uProductType, const float fXMLVersion, const int nCode ) const;
	int GetNewestCode( const UINT uProductType );
	int GetNewestCodeFromXML( const UINT type, const float fversion );
	float GetXMLVersionFromCode( const UINT type, const int code );
};

extern CConfiguratorApp theApp;


//==========================================================
/// Global members and functions
enum HexType{ TYPE_BIN, TYPE_HEX };
enum { E_NONE, E_FILE_EMPTY, E_FILE_EXT, E_FILE_FORMAT, E_FILE_PRODUCT, E_FILE_EXIST };
#define INFINITERETRY                0            // 0:正常流程； 1：消息序号不增加，重复次数无限制

// TS350
extern bool g_bCfgRefresh;  
extern bool g_bReadRefresh; 

BOOL RegSetRelated( CString& strExt, CString& strCommand, CString& strDefaultIcon );
void RegisterFileandProgram();

void VariantToString(const VARIANT& v, CString& s, UINT nHexType = TYPE_BIN);
void VariantToRecordString(const VARIANT& v, CString& s);
void PointTwipsToPixels(int x, int y, CPoint& pt);         // Convert twips to pixels
COleDateTime EncodeTimeSec( double ASeconds );
CString HrGetMessage( HRESULT hr );
CString GetSubKeyString();
CString DumpBytes( const BYTE* Buffer, int Count );
CString GetFilePath(void);

// History File
UINT ValidateHisFile( const CString strHisFileName );
CString GetEHisFileString( UINT nType );
bool IsHisFileSameArray(const CString strName1, const CString strName2);
int GetHisFileArrayCode(const CString strHisFileName);


// namespace global
namespace global
{
	void RenameLogFiles(void);

	extern CCfgSecurity Security;

	// font
	extern float FontSize;
	extern CString FontName;
	extern CFont g_Font;

	// Column Counts
	extern std::vector<int> SettingGridColWidths;
	extern std::vector<int> TagsGridColWidths;
	extern std::vector<int> HisGridColWidths;

	// Logging
    const int             LOG_FILE_COUNT = 7;
    const CString         LOG_FILE_NAME( "Configurator" );
    const CString         LOG_FILE_EXT( ".log" );

	extern const long TraceColors[8];

	// Column Counts
	const int SETTING_GRID_COLUMNS = 7;
	const int TAGS_GRID_COLUMNS = 6;
	const int HISTORY_GRID_COLUMNS = 4;

	// Trace Counts
	const int TRACE_COUNT = 8;

	// Gaps
	const int  AnalogReadGap   = 0;
    const int  AnalogReadMax   = 125;
    const int  AnalogWriteMax  = 2;
    const int  DigitalReadGap  = 0;
    const int  DigitalReadMax  = 2000;
    const int  DigitalWriteMax = 8;

	const UINT MIN_SECS = 60;
    const UINT HOUR_SECS = 60*MIN_SECS;
    const UINT DAY_SECS = 24*HOUR_SECS;

	//==============================================================
	// colors: from BCB VCL
	static const long clScrollBar = 0x80000000;
    static const long clBackground = 0x80000001;
    static const long clActiveCaption = 0x80000002;
    static const long clInactiveCaption = 0x80000003;
    static const long clMenu = 0x80000004;
    static const long clWindow = 0x80000005;
    static const long clWindowFrame = 0x80000006;
    static const long clMenuText = 0x80000007;
    static const long clWindowText = 0x80000008;             // Black
    static const long clCaptionText = 0x80000009;
    static const long clActiveBorder = 0x8000000a;
    static const long clInactiveBorder = 0x8000000b;
    static const long clAppWorkSpace = 0x8000000c;
    static const long clHighlight = 0x8000000d;
    static const long clHighlightText = 0x8000000e;
    static const long clBtnFace = 0x8000000f;
    static const long clBtnShadow = 0x80000010;
    static const long clGrayText = 0x80000011;
    static const long clBtnText = 0x80000012;
    static const long clInactiveCaptionText = 0x80000013;
    static const long clBtnHighlight = 0x80000014;
    static const long cl3DDkShadow = 0x80000015;
    static const long cl3DLight = 0x80000016;
    static const long clInfoText = 0x80000017;                // 黑色              
    static const long clInfoBk = 0x80000018;                  // 浅黄色
    static const long clBlack = 0x0;
    static const long clOlive = 0x8080;          
    static const long clPurple = 0x800080; 
    static const long clSilver = 0xf0f0f0;  //0xc0c0c0;
    static const long clLime = 0xff00;
    static const long clAqua = 0xffff00;
    static const long clLtGray = 0xc0c0c0;
    static const long clDkGray = 0x808080;
    static const long clWhite = 0xffffff;
    static const long clNone = 0x1fffffff;           // 黑色
    static const long clDefault = 0x20000000;        // 黑色


	static const long clRed = RGB(255,0,0);       //0xff;
	static const long clGreen = RGB(0, 255, 0);   //0x8000;
	static const long clBlue = RGB(0,0,255);      //0xff0000;
	static const long clGray = 0x808080;
	static const long clMaroon = 0x80;
	static const long clFuchsia = 0xff00ff;
	static const long clTeal = 0x808000;
	static const long clLBlue = RGB(0,255,255);
	
	static const long clYellow = RGB(255,255,0);
	static const long clHiPurple = 0x400080;
    //-----------------------------------------------

	const long ShadeColor = clSilver;                // 银白色
	const long DirtyLocalColor = clPurple;           // 紫色
	const long DirtyRemoteColor = clFuchsia;         // 紫红色
}// namespace global 
