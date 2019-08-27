// -----------------------------------------------------------------------
// $Workfile: AliasValue.cpp $
// Author       : Ryan Van Slooten
// Description  : Implementation of temporary alias value class
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2008, Tri-Sen  Corporation, La Marque, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/AliasValue.cpp 1     1/20/03 11:00a Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-03-28  Wangdong          BCB5 to VS2010: Add Record 
// 2003-01-13  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "AliasFileValue.h"

#include "AliasItem.h"
#include "Configurator.h"
#include "Configurator.h"


CAliasFileValue::CAliasFileValue(void)
	: m_bDirty( false )
	, m_bRecord( false )
{
}

CAliasFileValue::CAliasFileValue( LPCTSTR lpszName, LPCTSTR lpszValue, bool bRecord )
	: m_strName( lpszName )
	, m_avLocal( lpszValue )
	, m_bRecord( bRecord ) 
	, m_bDirty( false )
{
}

CAliasFileValue::CAliasFileValue( const CString& strName, const CString& strValue, bool bRecord )
	: m_strName( strName )
	, m_avLocal( strValue )
	, m_bRecord( bRecord )
	, m_bDirty( false )
{
}

CAliasFileValue::CAliasFileValue( const CAliasItem* pAlias )
	: m_bDirty( false )
{
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		m_strName = pAlias->GetName();
		m_avLocal = pAlias->valVar();
		m_bRecord = pAlias->GetRecord();
	}
}

CAliasFileValue::~CAliasFileValue(void)
{
}

// =========================================================================
// Function name: CAliasFileValueMap
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.28 13:11:00
// =========================================================================
CAliasFileValueMap::CAliasFileValueMap()
{
}
 
CAliasFileValueMap::~CAliasFileValueMap()
{
}

void CAliasFileValueMap::Add( const CAliasItem* pAlias )
{
	if( CheckReadPtr( pAlias, sizeof( CAliasItem)))
	{
        CAliasFileValue value( pAlias );
		insert( value_type( value.GetName(), value ));
	}
}

void CAliasFileValueMap::Add( const CAliasFileValue& pValue )
{
	insert( value_type( pValue.GetName(), pValue ));
}

// =========================================================================
// Function name: Override >> and <<
// Description  : 
// Return type  : 
// Parameters   : 
// Author       : WangDong 2010.03.28 13:20:00
// =========================================================================
CAliasFileValueMap& 
operator >> ( CAliasFileValueMap& AliasFileValue, CAliasMap* pAliases )
{
    if( CheckWritePtr( pAliases, sizeof(CAliasMap)))
	{
		CAliasFileValueMap::const_iterator it = AliasFileValue.begin();
		CAliasFileValueMap::const_iterator eit = AliasFileValue.end();

		while( it != eit )
		{
			CAliasItem* pAlias = pAliases->find_name( it->first );
			if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
			{
				const CAliasFileValue& value = it->second;
				pAlias->SetValue( value.GetValueStr());     // Set CAliasItem::m_avLocal with file value
				pAlias->SetDirtyLocal( value.GetDirty() );  // Dirty
				pAlias->SetRecord( value.GetRecord());      // Record
			}
			else
			{
				CString strErr;
		        strErr.Format( L"DataDictionary missing variable: %s", it->first );
		        theApp.LogInfo( strErr );
			}

			it++;
		}//while
	}//if

	return AliasFileValue;
}

//----------------------------------------------------------------------------
CAliasFileValueMap& 
operator << ( CAliasFileValueMap& AliasFileValue, const CAliasMap* pAliases )
{
	if( pAliases != NULL )
	{
		CAliasMap::const_iterator it = pAliases->begin();
		CAliasMap::const_iterator eit = pAliases->end();

		while( it != eit )
		{
			const CAliasItem* pAlias = it->second;
			AliasFileValue.Add( pAlias );

			it++;
		}//while
	}//if

	return AliasFileValue;
}
//----------------------------------------------------------------------------
