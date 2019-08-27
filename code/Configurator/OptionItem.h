// -----------------------------------------------------------------------
// File         : OptionItem.h
// Description  : Declaration of Configurator integral types
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
// 2010-12-17  WangDong          Changed from BCB5 to Visual Studio 2010
// -----------------------------------------------------------------------

#pragma once

class COptionItem
{
public:
    COptionItem()
		:m_dwValue( 0 ),
		 m_uItemAppType( 0 )
	{
	}

private:
	DWORD    m_dwValue;
	CString  m_strName;	
	UINT     m_uItemAppType;

public:
	/// Set
	void SetValue( DWORD dwValue )      { m_dwValue = dwValue; }

	void SetName( CString strName )     { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )   { m_strName = pcStrName; }

	void SetItemAppType( UINT value );   
	void SetItemAppType( CString strValue );

	/// Get
	DWORD GetValue()                     const { return m_dwValue; }

	CString GetName()                    const { return m_strName; }
	LPCTSTR GetName_C()                  const { return (LPCTSTR)m_strName; }

	UINT GetItemAppType()                 const { return m_uItemAppType; }
};

class COptionList : public std::map< DWORD, COptionItem*>
{
public:
	COptionList()
	{
	}

	~COptionList()
	{
		iterator it = begin();
		iterator eit = end();

		while( it != eit )
		{
			SAFE_DELETE( it->second );    // Free OptionItem
			it++;
		}

		erase( begin(), end() );
	}

private:
	CString m_strName;

public:
	/// Set
	void SetName( CString strName )       { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )     { m_strName = pcStrName; }

	/// Get
	CString GetName()                      const { return m_strName; }
	LPCTSTR GetName_C()                    const { return (LPCTSTR)m_strName; }

	/// Others
	COptionItem* find_num( DWORD dwValue );
	const COptionItem* find_num( DWORD dwValue )    const; 

	COptionItem* find_name( CString strHelpName )   const;

	CString FindNameByValue( DWORD dwValue ) const
	{
        const COptionItem* p = find_num( dwValue );
		if( CheckReadPtr( p, sizeof(COptionItem)))
		{
			return p->GetName();
		}
		else
		{
            return L"";
		}
	}
};


class COptionListMap : public std::map< CString, COptionList* >
{
public:
	~COptionListMap();

public:
	COptionList* find_name( CString strName );
	const COptionList* find_name( CString strName ) const;

	void clear( void );
};