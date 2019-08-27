#pragma once

struct IComDriver;
class CSerialDriver;
class CSocketDriver;
class CCommSettings;
class CAliasMap;

namespace modbus
{
class CClient;
class CModbusMsgVector;
class CModbusStatus;
class CMessageRanges;

class CModbusThread
{
public:
	CModbusThread(void);
	~CModbusThread(void);

public:
    enum Action { mbsNONE, mbsCONNECT, mbsDOWNLOAD, mbsUPLOAD, mbsMONITOR, mbsTEST };
    enum State { NONE, REQUEST, RESPONSE, NEXT, WAITING, SUSPEND };

private:
	std::auto_ptr<CEvent> m_pEvent;
	std::auto_ptr<CSerialDriver>    m_pSerial;
	std::auto_ptr<CSocketDriver>    m_pSocket;
	std::auto_ptr<CClient>          m_pClient;
	std::auto_ptr<CModbusMsgVector> m_pMessages;
	std::auto_ptr<CModbusStatus>    m_pStatus;
	
	IComDriver* m_pDriver;

    DWORD  m_dwTimeOut;
    DWORD  m_dwTimeStamp;
    DWORD  m_dwRepeatStamp;
    DWORD  m_dwRepeatTime;
    DWORD  m_dwScanTime;
    DWORD  m_dwReadMessages;
    DWORD  m_dwWriteMessages;
    DWORD  m_dwErrorMessages;
    DWORD  m_dwRetryMessages;
    DWORD  m_dwTimeoutMessages;
	State  m_nState;                // Current thread state
    Action m_nAction;
    Action m_nLastAction;
	bool   m_bModbusTcp;       
    bool   m_bConnected;
	int    m_nRepeatCount;
    int    m_nIndex;                // Message index 
	bool   m_bTerminated;

	CCriticalSection m_cirsec;
    void SetState( State AState );

protected:
	HRESULT WriteMessage();
	HRESULT ReadMessage();
    HRESULT NextState();

public:
	void ThreadExcute( void );
	void SetCommSettings( const CCommSettings* pCommSettings );

	// Event
	CEvent* GetEvent()                       const  { return m_pEvent.get(); }
	bool HasEvent( DWORD dwMilliSecs = 0 )   const  { return (WaitForSingleObject( m_pEvent->m_hObject, dwMilliSecs ) == WAIT_OBJECT_0);}                        
	void ResetEvent()                        { m_pEvent->ResetEvent(); }

	// Modbus Type
	bool GetModbusType(void)                 const  { return m_bModbusTcp; }

	// Connect
	bool GetConnected()                      const  { return m_bConnected; }
	void SetConnected( bool bConnected )     { m_bConnected = bConnected; } 

	// State
	int GetState()                           const  { return m_nState; }

	// Action
	Action GetAction()                       const { return m_nAction; }
    Action GetLastAction()                   const { return m_nLastAction; }

	// Terminated
	void SetTerminated( bool bTerminated )   { m_bTerminated = bTerminated; } 
	bool GetTerminated()                     const { return m_bTerminated; }

	// IsDone
	bool IsDone()                           const
	{ 
		bool bResult = false;
		if((m_nState == NONE) || (m_nState == SUSPEND))
		{
			bResult = true;
		}

		return bResult;
	}

	// Status
	void ClearStatusError();
	HRESULT GetStatusError()                 const;
	CString GetStatusErrorMsg()              const; 
	LPCTSTR GetStatusMessage()               const;
	int     GetStatusProgress()              const;

	// Product information
	HRESULT CheckProductInfo( CString &s );

	// AliasMap
	void SetAliasMap( CAliasMap* pAliasMap );

	// Count
	DWORD GetReadMessageCount()              const { return m_dwReadMessages; }
	DWORD GetWriteMessageCount()             const { return m_dwWriteMessages; }
	DWORD GetErrorMessageCount()             const { return m_dwErrorMessages; }
	DWORD GetScanTime()                      const { return m_dwScanTime; }

	HRESULT Connect();
    void Close();                             // Close comm driver, reset connect flag 

	void ClearMessages(void);                 // Clear msg vector
	void ResetThreadFlags();                  // Reset thread flags and event

	// Build Modbus Messages
	HRESULT LoopbackMessage();
	HRESULT ConnectToDevice();
    HRESULT ReceiveFromDevice();
	HRESULT MonitorDevice( const CMessageRanges* pMonitorRanges = NULL, CAliasMap* pAliases = NULL );
    HRESULT SendToDevice( bool bDirtyOnly, bool bTuningOnly );
	HRESULT SendCtlCommSettings( bool bDateTime = false );
    
    HRESULT ReadAliasesFromDevice( CAliasMap* pAliases );
    HRESULT WriteAliasesToDevice( CAliasMap* pAliases, bool bTuning = false );

	// DSC100 Calibration
	HRESULT SendCalibrationFlags( UINT uProType, bool bSaveFlag );

private:
	void DSC100Calibrate( bool bSaveFlag );
	void DSC110Calibrate( bool bSaveFlag );
};

}

