
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "afxext.h"

//Added by wangdong
#include "MySplitterWnd.h"    

class CCommSettings;
class CAliasMap;
class CHistoryServer;
class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//protected:  // control bar embedded members
public:
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	CMFCToolBar       m_viewToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

private:
	CMySplitterWnd m_vSplitter;   // one row and two columns
	DWORD m_dwRefreshTicks;       // Only used during monitoring
	UINT_PTR m_uThreadTimer;      // 0: ThreadTimer has been killed; Others not killed
	bool m_bAutoConnect;

private:
	bool AddView( int nRow, int nCol, CRuntimeClass* pViewClass, CCreateContext* pContext, int type, bool bFirst = false );
	void ShowClientView( CView* pView, int nColumn );
	inline void ShowView( int nRow, int nCol, CWnd* pWnd );
	inline void HideView( int nRow, int nCol );

public:
	enum TitleUpdateType { TITLE_NONE, TITLE_LOCAL, TITLE_LOCALCTRL };
	enum LeftView{  LSETTING_GRID, LTREND, LTUNING, LALIASES, LALARMS, LHISTORY };
	enum RightView{ RSETTING_GRID, RTREND, RALIASES, RHISTORY };

	bool IsRecording();
	void DoConnect( void );
    void DoDisconnect();
	bool DoCommSettingsDialog( void );
	void DoMonitor( bool bIsRecording = false );
	void DoStopMonitor( void );
	void CheckThreadDone( void );
	void ShowProgressDlg( void );
	void UpdateStatusBar( void );
	void UpdateTitle( UINT uType );
	bool IsConnected();
	HRESULT ReadAliasesFromDevice( CAliasMap* pAliases, bool bCalibrate = false );
	HRESULT WriteAliasesToDevice( CAliasMap* pAliases, bool bCalibrate = false, bool bTunning = false );
	void TestCommSettings( CCommSettings* pTestSettings );

	/// Calibrate
	void SendCalibrationFlags( UINT uProType, bool bSave );

	// DSC100 Calibrate
	bool IsOperationEnable( void );
	int GetPVAlgorithm( void );

	// TS350: Send controller settings
	HRESULT SendCTRLComSettings( bool bDateTime = false );

public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	/// Command
	// Device Menu
	afx_msg void OnDeviceEnterpassword();
	afx_msg void OnDeviceLogout();
	afx_msg void OnDeviceConnect();
	afx_msg void OnDeviceDisconnect();
	afx_msg void OnDeviceRetrieve();
	afx_msg void OnDeviceSend();
	afx_msg void OnDeviceCompare();
	afx_msg void OnDeviceCommsettings();
	afx_msg void OnControllerCommsettings();

	// Monitor Menu
	afx_msg void OnMonitorStart();
	afx_msg void OnMonitorStop();

	// View Menu
	afx_msg void OnViewConfig();
	afx_msg void OnViewTrend();
	afx_msg void OnViewTuning();
	afx_msg void OnViewAliases();
	afx_msg void OnViewAlarms();
	afx_msg void OnViewHistory();

	// Action
	afx_msg void OnActionCalibrate();
	afx_msg void OnActionRun();

	// Help
	afx_msg void OnHelpTopics();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	
	/// Update Command UI
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);

	afx_msg void OnUpdateDeviceLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceSelAppType(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceRetrieve(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceSend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceCommsettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControllerCommsettings(CCmdUI *pCmdUI);

	afx_msg void OnUpdateMonitorStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMonitorStop(CCmdUI *pCmdUI);

	afx_msg void OnUpdateRecordStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRecordStop(CCmdUI *pCmdUI);

	afx_msg void OnUpdateDeviceCompare(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewLanguageChs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewLanguageEng(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceDsc100(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceTs350(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeviceDsc110(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewConfig(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTrend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewTuning(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAliases(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAlarms(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewHistory(CCmdUI *pCmdUI);

	afx_msg void OnUpdateActionCalibrate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionRun(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHelpTopics(CCmdUI *pCmdUI);
};
