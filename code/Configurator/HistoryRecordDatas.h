#pragma once

#include <vector>


class CHistoryRecordData;
class COwnedAliasMap;

typedef std::vector<CHistoryRecordData*> HisDatas;
class CHistoryRecordDatas
{
public:
	CHistoryRecordDatas(void);
	~CHistoryRecordDatas(void);

private:
	CHistoryRecordDatas(const CHistoryRecordDatas&);
	CHistoryRecordDatas& operator=(const CHistoryRecordDatas&);

private:
	HisDatas m_vHistoryRecord;

public:
    void Load( const COwnedAliasMap* aAliases );
	void Clear();
	void CacheRecordDatas();
	void ClearRecordDatas();
	bool IsEmpty()      const { return m_vHistoryRecord.empty(); }
	
	bool HasLocalDatas() const;
	HisDatas&       GetHisRecordDatas()   { return m_vHistoryRecord; }
	const HisDatas& GetHisRecordDatas()   const  { return m_vHistoryRecord; }
};

