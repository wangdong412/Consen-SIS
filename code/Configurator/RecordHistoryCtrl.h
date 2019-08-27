#pragma once

// 具体类，用于记录用户配置的历史记录功能参数。这些参数不能在运行时进行修改。
// 
// 用法:
// 
// 用途：
// 保存用户设定的记录功能参数；
// 供历史记录线程函数使用
// 
const UINT MAX_FILE_SIZE = 2*1024*1024;     // 2M
class CRecordHistoryCtrl
{
public:
    CRecordHistoryCtrl( int nSamplingRate=1, UINT nMaxsize=MAX_FILE_SIZE )
		: m_nSamplingRate( nSamplingRate )  // Default 1s
		, m_nInterval( 500 )                // Fixed 500s
		, m_nMaxSize( nMaxsize )            // Default 2M
		, m_strFilePathName( L"" )
		, m_bFileModified( false )
	{
	}

private:
	CRecordHistoryCtrl(const CRecordHistoryCtrl&);
	CRecordHistoryCtrl& operator=(const CRecordHistoryCtrl&);

private:
    UINT m_nSamplingRate;        // 数据采样速率（Data Sampling Rate）:小时/分/秒/毫秒；以s为单位；默认1s
    UINT m_nInterval;            // 写文件间隔（Write File Interval）：固定500s
    UINT m_nMaxSize;             // 数据文件的最大尺寸：固定500M; 单位字节
    CString m_strFilePathName;   // 历史数据文件路径+文件名
	bool m_bFileModified;        // new datas was added to file or create new record file after maximum size
	
	CCriticalSection m_cs;       // 历史记录线程写：文件到达最大尺寸时，改写此文件路径名；
	                             // 主线程读：查看当前数据时，读取此文件路径名；

public:
	UINT GetSamplingRate()     const  { return m_nSamplingRate; }   // 获取用户设定的数据采样速率
	UINT GetInterval()         const  { return m_nInterval; }       // 获取写文件间隔
	UINT GetMaxSize()          const  { return m_nMaxSize; }        // 获取最大数据文件尺寸
	bool IsFileModified()          const  { return m_bFileModified; }

	// 获取数据文件的存储位置和文件名
	// 查看当前数据时，读取此文件路径名
	CString GetFilePathName()  
	{ 
		CSingleLock sl( &m_cs );
		sl.Lock();
		
		return m_strFilePathName;
	}

	

    void SetSamplingRate( UINT nSamplingRate )   { m_nSamplingRate = nSamplingRate; }  //s
	void SetMaxSize( UINT value )                { m_nMaxSize = value; }
	void SetFileModified( bool bModified )       { m_bFileModified = bModified; }                      

	// 设置历史数据文件路径名：路径+文件名
	// 历史记录线程会改写此文件名：当前文件达到设定的最大尺寸后，改写
    void SetFilePathName( CString strFilePathName )  
	{
		CSingleLock sl( &m_cs );
		sl.Lock();
		if( sl.IsLocked())
		{
			m_strFilePathName = strFilePathName;
			sl.Unlock();
		}
	}

	void Reset()
	{
	    m_nSamplingRate = 1;          // Default 1s
		m_nInterval = 500;            // Fixed 500s
		m_nMaxSize = MAX_FILE_SIZE;   // Default 2M
		m_strFilePathName = L""; 
		m_bFileModified = false;
	}
};
