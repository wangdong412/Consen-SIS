#pragma once

// 待记录的Modbus参数类：用户启动历史记录时，动态生成并加入集合;
// 用途：记录一定数目的本地数据。
#include <map>
#include "AliasItem.h"

typedef std::map<CString,CString> StringMap;
class CHistoryRecordData
{
public:
    CHistoryRecordData(CAliasItem* pAlias )
		: m_pAliasItem( pAlias )
		, m_LocalDataMap()
	{
		ASSERT( m_pAliasItem );
	}

    // 拷贝构造函数 赋值运算符：按浅拷贝处理
    CHistoryRecordData(const CHistoryRecordData& orig);
	CHistoryRecordData& operator=(const CHistoryRecordData& rhs);

    // 清除缓存在本地的所有数据
    ~CHistoryRecordData()
	{
		Clear();
	}

private:
	CAliasItem* m_pAliasItem;
    StringMap m_LocalDataMap;    // 缓存记录本地数据：时间-数值对，类型均为CString

public:
    void Clear();                 // 清除缓存在本地的数据
    void Add( CString strTime );  // 将当前时间的数据添加到本地数据集合中
	bool HasLocalDatas() const
	{
		return (m_LocalDataMap.size() != 0);
	}

	int GetAlias() const
	{
		int nAlias = -1;
		if( m_pAliasItem != NULL )
		{
			nAlias = m_pAliasItem->GetAlias();
		}

		return nAlias;
	}
	
	const CAliasItem* GetAliasItem() const
	{
		return m_pAliasItem;
	}

	CAliasItem* GetAliasItem() 
	{
		return m_pAliasItem;
	}

	const StringMap& GetLocalDataMap() const
	{
		return m_LocalDataMap;
	}
};

