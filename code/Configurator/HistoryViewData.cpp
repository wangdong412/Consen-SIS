#include "stdafx.h"
#include "HistoryViewData.h"

#include "Configurator.h"

// =======================================================================
// Function name  : CHistoryViewDatas
// Description    : 
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.01 09:00:00
// =======================================================================
CHistoryViewDatas::CHistoryViewDatas()
{
	for( size_t i=0; i < 8; ++i )
	{
		m_vHisViewDatas.push_back( new CHistoryViewData( global::TraceColors[i]));
	}
}

// =======================================================================
// Function name  :~CHistoryViewDatas
// Description    : Release and Clear the vector
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.01 09:00:00
// =======================================================================
CHistoryViewDatas::~CHistoryViewDatas()
{
	Clear();
}

// =======================================================================
// Function name  : Release
// Description    : Release
// Return type    :
// Parameters     : 
// Author         : WangDong 2013.04.01 09:10:00
// =======================================================================
void CHistoryViewDatas::Clear()
{
	for( size_t i=0; i < m_vHisViewDatas.size(); i++ )
	{
		SAFE_DELETE( m_vHisViewDatas[i] );
	}

	m_vHisViewDatas.clear();
}

// =======================================================================
// Function name  : GetHisViewData
// Description    : Get the HisViewData according to the index
// Return type    : The HisViewData 
// Parameters     : 
// Author         : WangDong 2013.04.01 09:15:00
// =======================================================================
const CHistoryViewData* CHistoryViewDatas::GetHisViewData( const size_t index ) const
{
	ASSERT( index < m_vHisViewDatas.size());

	if( index < m_vHisViewDatas.size())
	{
		return m_vHisViewDatas[index];
	}
	else
	{
		return NULL;
	}
}

CHistoryViewData* CHistoryViewDatas::GetHisViewData( const size_t index )
{
	ASSERT( index < m_vHisViewDatas.size());

	if( index < m_vHisViewDatas.size())
	{
		return m_vHisViewDatas[index];
	}
	else
	{
		return NULL;
	}
}

// =======================================================================
// Function name  : SetHisViewData
// Description    : 
// Return type    : void 
// Parameters     : 
// Author         : WangDong 2013.04.01 14:00:00
// =======================================================================
void CHistoryViewDatas::SetHisViewData( const size_t index, CHistoryViewData* data )
{
	ASSERT( index < m_vHisViewDatas.size());

	if( index < m_vHisViewDatas.size())
	{
		m_vHisViewDatas[index] = data;
	}
}

// =======================================================================
// Function name  : Reset
// Description    : Switch Product or Apptype
// Return type    : void 
// Parameters     : 
// Author         : WangDong 2013.04.01 14:00:00
// =======================================================================
void CHistoryViewDatas::Reset()
{
	for( size_t i=0; i < m_vHisViewDatas.size(); i++ )
	{
		m_vHisViewDatas[i]->Reset( global::TraceColors[i] );
	}
}

