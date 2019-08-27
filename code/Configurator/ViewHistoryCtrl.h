#pragma once

// 具体类，用于记录用户配置的查看功能参数。
// 
// 用法：全局；
// 
// 用途：
// 保存用户设定的查看功能参数；
// 供History左/右视图初始化及刷新显示用

#include "RecordHistoryCtrl.h"
class CViewHistoryCtrl
{
public:
	CViewHistoryCtrl( CRecordHistoryCtrl *record )
		: m_nViewType( VIEW_NONE )
		, m_nUpdateRate( 60 )
		, m_nTimeRange( 1800 )
		, m_strTimeFormat( L"HH:NN:SS" )
		, m_strFilePathName( L"" )
		, m_pRecordCtrl( record )
	{
		ASSERT( m_pRecordCtrl != NULL );
	}

private:
	CViewHistoryCtrl(const CViewHistoryCtrl&);
	CViewHistoryCtrl& operator=(const CViewHistoryCtrl&);

private:
    // 查看类型：
	// 0：未查看；
    // 1：查看正在被记录的数据，需定时刷新视图显示；
    // 2：查看指定数据文件中的数据；
    int m_nViewType;
    UINT m_nUpdateRate;       // 历史数据刷新速率：小时/分/秒，单位s，默认60秒
    UINT m_nTimeRange;        // 时间跨度：日/小时/分/秒，单位s，默认30分钟，即1800秒
    CString m_strTimeFormat;  // 时间刻度格式：MM:DD:YY HH:MM:SS 默认HH:MM:SS

	// 文件路径名：将被查看的历史数据文件路径+文件名。
	// 查看类型为0时, 空；
    // 查看类型为1时，文件是记录历史数据时指定的文件(在记满后，会被更新)；
    // 查看类型为2时，文件是用户指定查看的历史数据文件。
    CString m_strFilePathName;
	CRecordHistoryCtrl* m_pRecordCtrl;

public:
	enum { VIEW_NONE, VIEW_CURRENT, VIEW_FILE };

public:
	int  GetViewType()                 const { return m_nViewType; }           // 获取查看类型
	UINT GetUpdateRate()               const { return m_nUpdateRate; }         // 获取历史数据刷新速率
	UINT GetTimeRange()                const { return m_nTimeRange; }          // 获取时间跨度
	CString GetTimeFormat()            const { return m_strTimeFormat; }       // 获取时间刻度格式
	CString GetRecordingFilePathName() const { return m_pRecordCtrl->GetFilePathName(); }
	CString GetFilePathName() const 
	{ 
		if( VIEW_CURRENT == m_nViewType)
		{
			ASSERT( m_pRecordCtrl != NULL );
			return m_pRecordCtrl->GetFilePathName();
		}
		else
		{
			return m_strFilePathName;
		}
	}

	// 设置查看类型
    void SetViewType(int nViewType )
	{
		if( m_nViewType != nViewType ) 
		{
			m_nViewType = nViewType;
		}
	}

    // 设置历史数据刷新速率
    void SetUpdateRate(UINT nUpdateRate = 60)
	{
		if( m_nUpdateRate != nUpdateRate )
		{
			m_nUpdateRate = nUpdateRate;
		}
	}

    // 设置时间跨度
    void SetTimeRange(UINT nTimeRange = 30*60)
	{
		if( m_nTimeRange != nTimeRange )
		{
			m_nTimeRange = nTimeRange;
		}
	}

    // 设置时间刻度格式
    void SetTimeFormat(CString strFormat)
	{
		if( m_strTimeFormat != strFormat )
		{
			m_strTimeFormat = strFormat;
		}
	}

    // 设置数据文件路径名，并根据历史文件设置查看类型
    void SetFilePathName( CString strPathName )
	{
		if( m_nViewType != VIEW_CURRENT )
		{
			if( m_strFilePathName != strPathName )
			{
				m_strFilePathName = strPathName;
			}
		}
	}

	// Reset ctrl
	void Reset()
	{
		m_nViewType = VIEW_NONE;
		m_nUpdateRate = 60;
		m_nTimeRange = 1800;
		m_strTimeFormat = L"HH:NN:SS";
		m_strFilePathName = L"";
	}
};
