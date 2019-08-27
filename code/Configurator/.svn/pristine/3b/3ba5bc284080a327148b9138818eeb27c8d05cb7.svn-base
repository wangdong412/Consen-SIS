#pragma once

#include <map>

// 历史文件数据类：根据目标历史数据文件加载而来
// 
// 用途：供用户选择已记录的标签名
 
class CHistoryViewData;
class CHistoryFileData
{
public:
    CHistoryFileData( CString strTagName, CString strDesc, int nAlias )
		: m_strTagName( strTagName )
		, m_strDesc( strDesc )
		, m_nAlias( nAlias )
	{
		m_strAlias.Format( L"%d", nAlias );
	}

private:
    CString m_strTagName;   // 标签名
    CString m_strDesc;      // 参数描述信息
    int m_nAlias;           // 参数别名（Modbus地址）
	CString m_strAlias;

public:
	CString GetTagname() const { return m_strTagName; }

friend class CHistoryViewData;
friend class CSelectTNDlg;
};




// 历史文件数据集合类
// 使用说明：
// 1.Doc中集合（map）初始化为空；
// 2.用户首次选择标签名时，根据查看类型，加载目标历史记录文件；
// 3.确定查看时，如果文件与之前不同，则清空集合；
// 4.切换应用程序类型、产品类型时：清空；
// 5.新建/打开文件时：不清空。
typedef std::map<int, CHistoryFileData*> HFileDatas;
class CHistoryFileDatas
{
public:
	CHistoryFileDatas()
		:m_vHFileDatas()
	{
	}

	~CHistoryFileDatas();

	CHistoryFileDatas( const CHistoryFileDatas& );
	CHistoryFileDatas& operator=( const CHistoryFileDatas& );
	
private:
	HFileDatas m_vHFileDatas;

public:
	const HFileDatas& GetHFileDatas()    const { return m_vHFileDatas; }
	
	void Clear();
	void Add( int nAlias, CHistoryFileData* data );
	const CHistoryFileData* find_num( int nAlias ) const;
	const CHistoryFileData* find_name( const CString strTagName ) const;
	CHistoryFileData* CHistoryFileDatas::find_name( const CString strTagName );
};
