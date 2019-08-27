#pragma once

// ConfiguratorDoc.h : interface of the CConfiguratorDoc class
//

// XML Data Class
#include "CfgProperty.h"                       // Required and Dictionary attribute
#include "OptionItem.h"                        // OptionLists
#include "AliasItem.h"                         // Variables
#include "SettingItem.h"                       // Settings
#include "CfgMonitor.h"                        // MonitorValues, AlarmValues
#include "ModbusRange.h"                       // Communications
#include "CfgTrace.h"                          // Trace information

// History
#include "HistoryRecordData.h"
#include "HistoryRecordDatas.h"
#include "HistoryFileData.h"
#include "HistoryViewData.h"

// History control
#include "RecordHistoryCtrl.h"
#include "ViewHistoryCtrl.h"

#include "SendStatus.h"
#include <memory>

// MSXML Parser
#import <msxml4.dll>              
using namespace MSXML2;


class CAliasFileValueMap;
class CHistoryServer;
class CConfiguratorDoc : public CDocument
{
protected: // create from serialization only
	CConfiguratorDoc();
	DECLARE_DYNCREATE(CConfiguratorDoc)

// Attributes
public:
	 CCfgProperties*      m_aProperties;
	 COptionListMap*      m_aOptions;
	 COwnedAliasMap*      m_aAliases;
	 CSettingVector*      m_aSettings;
	 CSettingVector*      m_aSettingsAlias;
	 CCfgMonitorValues*   m_aMonitorValues;
	 CCfgMonitorValues*   m_aAlarmValues;
	 CCfgTraceArray*      m_aTraces;
 
	 CHistoryRecordDatas* m_aHisRecordDatas;
	 CHistoryFileDatas*   m_aHisFileDatas;
	 CHistoryViewDatas*   m_aHisViewDatas;

	 std::auto_ptr<modbus::CMessageActions> m_aActions;
	 std::auto_ptr<CAliasFileValueMap> m_aAliasFileValue;

	 enum Hints
     {
         HINT_REFRESH,
         HINT_UPDATE,
         HINT_MONITOR,
         HINT_TRACE,
         HINT_RULE,
		 HINT_FONT,
		 
		 HIS_CLEAR = 11,
		 HIS_INIT,
		 HIS_DYNUPDATE
     };

	 enum ItemAppType{ APPTYPE_COMM, APPTYPE_GEN, APPTYPE_MECH };

// Operations
public:
	DWORD GetSettingContextId( CSettingItem* pSetting );       // Get HelpID of the SettingItem

	bool CheckProductNumber(); // Check Product Number
	bool UpdateProductInfo( LPCTSTR lpszPathName );  // Update Product Number and AppType

	/// Trace
	size_t GetTraceCount() const;
	CString GetTraceName( size_t SeriesIndex ) const;
	HRESULT AddTraceName( CString strName, size_t SeriesIndex );
	HRESULT RemoveTraceName( CString strName);

	void UpdateRemoteFlags( int nBeginAlias, int nEndAlias );
	const CSendStatus* GetSendDlgStatus();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	/// OverWrite: return TRUE if ok to continue
	virtual BOOL SaveModified(); 

// Implementation
public:
	virtual ~CConfiguratorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnViewRefresh();
	afx_msg void OnViewDisplayfont();
	afx_msg void OnHistoryView();
	
	afx_msg void OnHistoryStart();
	afx_msg void OnHistoryStop();

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	virtual void DeleteContents();   // Override 

	/// XML Parser
	bool ParseXMLFile();

	// Required
	void RequiredElement( MSXML2::IXMLDOMNodePtr pParentNode );

	//OptionLists
	void OptionListsElement( MSXML2::IXMLDOMNodePtr pParentNode ); 
	void ParseOptionList( MSXML2::IXMLDOMNodePtr pOptListNode, COptionList* pOptListMap ); 

	// Variables
	void AliasElement( MSXML2::IXMLDOMNodePtr pParentNode );          

	// Settings
	void SettingElement( MSXML2::IXMLDOMNodePtr pParentNode );
	CSettingItem* ParseSingleSettingItem( MSXML2::IXMLDOMNodePtr pNode );
	void AddChildSettings( MSXML2::IXMLDOMNodePtr pParentNode, CSettingItem* pParentSetting );

	// MonitorValues and AlarmValues
	void MonitorValueElement( MSXML2::IXMLDOMNodePtr pParentNode, bool bIsAlarm = false );

	// Communications
	void CommunicationElement( MSXML2::IXMLDOMNodePtr pParentNode );
	void ParseAction( MSXML2::IXMLDOMNodePtr pActionNode, modbus::CMessageRanges* pMsgRanges );

	// Bind
	void BindDictionary( void );
	void BindSettings( CSettingVector* pSettings );

	void BindDocument(void);

public:
	// New
	BOOL DoFileNew();

	// Save document
	void DoSaveDocument();
	BOOL SaveDocument( LPCTSTR lpszPathName );
	BOOL SaveAsDocument();

	// Open saved document file
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	bool OpenDocFile( LPCTSTR lpszPathName, bool bCompareFile = false );
	void ReadTraceElement( MSXML2::IXMLDOMNodePtr pParentNode );
	void ReadAliasElement( MSXML2::IXMLDOMNodePtr pParentNode, bool bCompareFile );

	float GetOpenFileVersion( LPCTSTR lpszPathName );

	// Compare file
	BOOL CompareFile( LPCTSTR lpszPathName, bool bDlg = false );

	// Compare File Path Name
	void ResetCmpFilePathName()        { m_strCmpFilePathName.Empty(); }

	// Set Settings Visible
	void SetVisible();
	void SetSettingsVisible( CSettingVector* pSettings, bool bSettingAlias = false );

// History

private:
	CSendStatus m_SendDlgStatus;
	bool m_bRecordModified;
	std::auto_ptr<CEvent> m_pHisEvent;  // R/W History File
	std::auto_ptr<CEvent> m_pHisRecordSusEvent;  // History Record Thread was suspended
	CRecordHistoryCtrl* m_pRecordCtrl;  // Record History Ctrl
	CViewHistoryCtrl* m_pViewCtrl;      // View History Ctrl 
	CString m_strCmpFilePathName;       // Compare File Path Name

	std::auto_ptr<CHistoryServer> m_pHistoryServer;
	std::auto_ptr<CWinThread> m_pHistoryThread;

    bool IsInMonitorRange( int nAlias ) const;

public:
	void SetRecordModified( bool flag )    { m_bRecordModified = flag; }

	CEvent* GetHisRecordSusEvent()         
	{ 
		ASSERT( m_pHisRecordSusEvent.get());
		return m_pHisRecordSusEvent.get(); 
	}

	bool IsRecording();
	CRecordHistoryCtrl* GetHisRecordCtrl() { return m_pRecordCtrl; }
	CHistoryServer* GetHistoryServer()     { return m_pHistoryServer.get(); }
	CWinThread* GetHistoryThread()         { return m_pHistoryThread.get(); }

	// Record History datas
	CAliasMap* GetHisRecordNonMonitorAliases();
	bool CreateHistoryFile( CString strFilePathName );
	bool RecordHistoryFile();
	void CheckCurRecordFile();
	bool IsRecordFileModified()             const { return m_pRecordCtrl->IsFileModified(); }
	void SetRecordFileModified( bool flag ) { m_pRecordCtrl->SetFileModified( flag ); }

	// View History datas
	DWORD WaitHisFileFinish( DWORD dwMS = 5000 )    { return WaitForSingleObject( m_pHisEvent->m_hObject, dwMS ); }
	BOOL SetHisFileFinishEvent()                    { return m_pHisEvent->SetEvent(); }
	BOOL ResetHisFileFinishEvent()                  { return m_pHisEvent->ResetEvent(); }
	const CViewHistoryCtrl*  GetViewHisCtrl() const { return m_pViewCtrl; }
	CViewHistoryCtrl*  GetViewHisCtrl()             { return m_pViewCtrl; }
	
	void UpdateHistoryView( LPARAM lHint, double dStartTime = -1 );
	void StopHisTrendViewTimerUpdate();

	bool LoadHisFile( CString strHisFileName, CHistoryFileDatas *aHisFileDatas );
	void DynViewHistory( double dStartTime );
	void ResetHistory();

	bool IsStoppingHistoryRecord();
};
