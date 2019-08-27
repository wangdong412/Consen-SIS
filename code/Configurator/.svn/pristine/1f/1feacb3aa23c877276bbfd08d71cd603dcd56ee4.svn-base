#pragma once


class CHistoryServer
{
public:
    CHistoryServer()
		: m_bIsRecording( false )
		, m_bExit( false )
		, m_bSuspend( false )
		, m_dwStartTime( 0 )
	{
	}

    ~CHistoryServer()
	{
	}

private:
    CHistoryServer(const CHistoryServer& orig);
    CHistoryServer& operator=(const CHistoryServer& rhs);

private:
    bool m_bIsRecording;  // 启动历史记录时，置true；停止历史记录时，置false；

public:
	bool m_bExit;
	bool m_bSuspend;

	DWORD m_dwStartTime;

public:
	bool IsRecording()                     const { return m_bIsRecording; }
	void SetRecording(bool bRecording)     {  m_bIsRecording = bRecording; }

    static UINT RecordThreadFunc( LPVOID pParam); // 历史记录线程函数：用于完成与历史记录相关的功能
};
