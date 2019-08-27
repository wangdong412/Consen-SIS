#include "stdafx.h"
#include "HistoryFileData.h"


// =================================================================
// Function name  : CHistoryFileDatas
// Description    : Copy construct function
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.03.29 16:30:00
// =================================================================
CHistoryFileDatas::CHistoryFileDatas( const CHistoryFileDatas &orig )
{
	HFileDatas::const_iterator it = orig.m_vHFileDatas.begin();
	while( it != orig.m_vHFileDatas.end())
	{
		Add( it->first, new CHistoryFileData( *(it->second)));

		++it;
	}
}

// =================================================================
// Function name  : ~CHistoryFileDatas
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.03.29 16:30:00
// =================================================================
CHistoryFileDatas::~CHistoryFileDatas()
{
	Clear();
}

// =================================================================
// Function name  : operator=
// Description    : 
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.04.23 09:30:00
// =================================================================
CHistoryFileDatas& CHistoryFileDatas::operator=( const CHistoryFileDatas &rhs )
{
	// Avoid copy itself
	if( this != &rhs )
	{
		Clear();

		HFileDatas::const_iterator it = rhs.m_vHFileDatas.begin();
		while( it != rhs.m_vHFileDatas.end())
		{
			Add( it->first, new CHistoryFileData( *(it->second)));

			++it;
		}
	}

	return *this;
}

// =================================================================
// Function name  : Clear
// Description    : Release map resource
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.03.29 16:32:00
// =================================================================
void CHistoryFileDatas::Clear()
{
	HFileDatas::iterator it = m_vHFileDatas.begin();
	while( it != m_vHFileDatas.end())
	{
		SAFE_DELETE( it->second );
		++it;
	}

	m_vHFileDatas.clear();
}

// =================================================================
// Function name  : Add
// Description    : Release map resource
// Return type    : 
// Parameters     : 
// Author         : WangDong 2013.03.29 16:35:00
// =================================================================
void CHistoryFileDatas::Add( int nAlias, CHistoryFileData* data )
{
	ASSERT( data != NULL );
	if( data != NULL )
	{
		m_vHFileDatas.insert( std::make_pair<int,CHistoryFileData*>(nAlias,data));
	}
}

// =================================================================
// Function name  : find_num
// Description    : 
// Return type    : NULL if not found;
// Parameters     : 
// Author         : WangDong 2013.04.18 13:35:00
// =================================================================
const CHistoryFileData* CHistoryFileDatas::find_num( int nAlias ) const
{
	//return ->second;
	HFileDatas::const_iterator it = m_vHFileDatas.find( nAlias );
	if( it != m_vHFileDatas.end())
	{
		return it->second;
	}

	return NULL;
}

// =================================================================
// Function name  : find_name
// Description    : const
// Return type    : NULL if not found;
// Parameters     : 
// Author         : WangDong 2013.04.18 13:35:00
// =================================================================
const CHistoryFileData* CHistoryFileDatas::find_name( const CString strTagName ) const
{
	HFileDatas::const_iterator it = m_vHFileDatas.begin();
	while( it != m_vHFileDatas.end())
	{
		const CHistoryFileData* data = it->second;
		if( data != NULL )
		{
			if( strTagName.Compare( data->GetTagname()) == 0 )
			{
				return data;
			}
		}

		++it;
	}

	return NULL;
}

// =================================================================
// Function name  : find_name
// Description    : un const
// Return type    : NULL if not found;
// Parameters     : 
// Author         : WangDong 2013.04.18 13:40:00
// =================================================================
CHistoryFileData* CHistoryFileDatas::find_name( const CString strTagName )
{
	HFileDatas::iterator it = m_vHFileDatas.begin();
	while( it != m_vHFileDatas.end())
	{
		CHistoryFileData* data = it->second;
		if( data != NULL )
		{
			if( strTagName.Compare( data->GetTagname()) == 0 )
			{
				return data;
			}
		}

		++it;
	}

	return NULL;
}
