// -----------------------------------------------------------------------
// $Workfile: SettingItem.cpp $
// Author/Date  : WandDong 2010-12-17 15:30:00 (UTC-6:00)
// Description  : Option item, Option List and Option List map for the document
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2010, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Configurator/OptionItem.cpp  1     12/17/10 15:30a WangDong $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-12-17  WangDong          new cpp file
// -----------------------------------------------------------------------

#include "stdafx.h"
#include "Configurator.h"
#include "OptionItem.h"


// =======================================================================
// Function name : SetItemAppType
// Description   : 1 - Gen; 2 - Mech
// Return type   : 
// Parameters    : strValue
// Author        : WangDong 2011.10.26 11:40:00
// =======================================================================
void COptionItem::SetItemAppType( UINT value )
{
    if( value > 2 )
	{
		value = 0;
	}

	m_uItemAppType = value;
}

void COptionItem::SetItemAppType( CString strValue )
{
	if( strValue.CompareNoCase( L"Gen" ) == 0 )
	{
		m_uItemAppType = 1;
	}
	else if( strValue.CompareNoCase( L"Mech" ) == 0 )
	{
		m_uItemAppType = 2;
	}
	else
	{
		m_uItemAppType = 0;
	}
}

// =======================================================================
// Function name : find_num
// Description   : 
// Return type   : COptionItem*
// Parameters    : uValue
// Author        : WangDong 2010.12.17 16:30:00
// =======================================================================
COptionItem* COptionList::find_num( DWORD dwValue )
{
	iterator it = find( dwValue );
	if( it == end())
	{
		return NULL;
	}

	return it->second;
}

//-------------------------------------------------------------------------
const COptionItem* COptionList::find_num( DWORD dwValue )   const
{
    const_iterator it = find( dwValue );
	if( it == end())
	{
		return NULL;
	}

	return it->second;
}

// =======================================================================
// Function name : find_name
// Description   : 
// Return type   : COptionItem*
// Parameters    : strHelpName
// Author        : WangDong 2011.01.10 10:00:00
// =======================================================================
COptionItem* COptionList::find_name( CString strHelpName ) const
{
	COptionItem* pOptionItem = NULL;
    for( const_iterator it = begin(); it != end(); it++ )
	{
		strHelpName.MakeUpper();
		CString s = it->second->GetName();
		if( s.CompareNoCase( strHelpName ) == 0 )
		{
			pOptionItem = it->second;
			break;
		}
	}

	return pOptionItem;
}

// =======================================================================
/// COptionListMap Îö¹¹º¯Êý
COptionListMap::~COptionListMap()
{
    clear();
}

// =======================================================================
// Function name : find_name
// Description   : 
// Return type   : COptionList*
// Parameters    : strName
// Author        : WangDong 2010.12.17 16:40:00
// =======================================================================
COptionList* COptionListMap::find_name( CString strName )
{
    iterator it = find( strName );
	if( it == end())
	{
		return NULL;
	}

	return it->second;
}

//------------------------------------------------------------------------
const COptionList* COptionListMap::find_name( CString strName ) const
{
    const_iterator it = find( strName );
	if( it == end())
	{
		return NULL;
	}

	return it->second;
}

// =======================================================================
// Function name : clear
// Description   : 
// Return type   : 
// Parameters    : 
// Author        : WangDong 2010.12.17 16:45:00
// =======================================================================
void COptionListMap::clear()
{
    iterator it = begin();
	iterator eit = end();

	while( it != eit )
	{
		SAFE_DELETE( it->second );   // call ~COptionList()
		it++;
	}

	erase( begin(), end() );
}

//------------------------------------------------------------------------
