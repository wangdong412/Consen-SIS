// -----------------------------------------------------------------------
// File         : CfgProperty.h
// Description  : 
// Author       : wangdong
// Purpose      :
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2010, Tri-Sen Corporation, La Marque, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
//
//                         REVISION HISTORY
//                         ----------------
//
//   Date         Who          Change
// --------  ----------------  -------------------------------------------
// 2010-12-21  WangDong          Recode in Visual Studio 2010
// 2002-12-13  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#pragma once
#include "stdafx.h"

class CCfgProperty
{
public:
	CCfgProperty() :m_uValue(0) {}

	CCfgProperty( CString strName, UINT uValue )
		:m_strName( strName ),
		 m_uValue( uValue )
	{
	}

private:
	CString m_strName;
    UINT m_uValue;
   
public:
	///Set
	void SetName( CString strName )         { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )       { m_strName = pcStrName; }

	void SetValue( UINT uValue )            { m_uValue = uValue; }

	///Get
	CString GetName()                        const { return m_strName; }
	LPCTSTR GetName_C()                      const { return (LPCTSTR)m_strName; }

	UINT GetValue()                          const { return m_uValue; }
};



typedef std::map<CString, CCfgProperty*> CCfgPropertyMapInternal;
class CCfgProperties : public CCfgPropertyMapInternal
{
public:
	CCfgProperties()
	{
	}

	~CCfgProperties()
	{
		clear();
	}

public:

	void clear()
	{
		iterator it = begin();
		iterator eit = end();

		while( it != eit )
		{
			SAFE_DELETE( it->second );
			it++;
		}

		erase( begin(), end() );
	}

	// find_name
	CCfgProperty* find_name( CString strName )
	{
        iterator it = find( strName );
		if( it == end())
		{
			return NULL;
		}

		return it->second;
	}

    const CCfgProperty* find_name( CString strName ) const
	{
		const_iterator it = find( strName );
		if( it == end())
		{
			return NULL;
		}

		return it->second;
	}

	// Add
	void Add( LPCTSTR pcStrName, LPCTSTR pcStrValue )
	{
        if( find_name( pcStrName ) == NULL )  // not exist
		{
			UINT uValue = 0;
			::swscanf_s( pcStrValue, L"%d", &uValue );

			insert( value_type( pcStrName, new CCfgProperty( pcStrName, uValue )));
		}
	}

	void Add( LPCTSTR pcStrName, UINT uValue )
	{
		if( find_name( pcStrName ) == NULL )  // not exist
		{
            insert( value_type(pcStrName, new CCfgProperty( pcStrName, uValue )));
		}
	}
};