#include "StdAfx.h"
#include "HistoryRecordDatas.h"

#include "AliasItem.h"
#include "HistoryRecordData.h"


CHistoryRecordDatas::CHistoryRecordDatas(void)
{
}


CHistoryRecordDatas::~CHistoryRecordDatas(void)
{
	Clear();
}


// =================================================================
// Function name  : LoadRecordData
// Description    : If Record was true, add it to the vector
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.03.29 09:30:00
// =================================================================
void CHistoryRecordDatas::Load( const COwnedAliasMap* aAliases )
{
	if( NULL == aAliases )
	{
		return;
	}

	Clear();

	CAliasItem *pAlias = NULL;
	COwnedAliasMap::const_iterator it = aAliases->begin();
	while( it != aAliases->end())
	{
		pAlias = it->second;
		if( pAlias != NULL )
		{
			if( pAlias->GetRecord())
			{
				m_vHistoryRecord.push_back( new CHistoryRecordData( pAlias ));
			}
		}

		++it;
	}
}

// =================================================================
// Function name  : Clear
// Description    : Delete the resource and clear the vector
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.03.29 09:35:00
// =================================================================
void CHistoryRecordDatas::Clear()
{
	for( size_t i=0; i < m_vHistoryRecord.size(); ++i )
	{
		SAFE_DELETE( m_vHistoryRecord[i] );
	}

	m_vHistoryRecord.clear();
}

// =================================================================
// Function name  : CacheRecordDatas
// Description    : Cache Local Datas 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.03.29 09:40:00
// =================================================================
void CHistoryRecordDatas::CacheRecordDatas()
{
	CString strTime;
	SYSTEMTIME tm;
	::GetLocalTime( &tm );
	COleDateTime time( tm );
	strTime.Format( L"%f", time );

	for( size_t i=0; i < m_vHistoryRecord.size(); ++i )
	{
		m_vHistoryRecord[i]->Add( strTime );
	}
}

// =================================================================
// Function name  : ClearDatas
// Description    : Clear all the "time-value" datas 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.03.29 09:40:00
// =================================================================
void CHistoryRecordDatas::ClearRecordDatas()
{
	for( size_t i=0; i < m_vHistoryRecord.size(); ++i )
	{
		m_vHistoryRecord[i]->Clear();
	}
}

// =================================================================
// Function name  : HasLocalDatas
// Description    : 
// Return type    : void
// Parameters     : 
// Author         : WangDong 2013.03.29 09:40:00
// =================================================================
bool CHistoryRecordDatas::HasLocalDatas() const
{
	bool bResult = false;
	for( size_t i = 0; i < m_vHistoryRecord.size(); i++ )
	{
		if( m_vHistoryRecord[i]->HasLocalDatas())
		{
			bResult = true;
			break;
		}
	}

	return bResult;
}