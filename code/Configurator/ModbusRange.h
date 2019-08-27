// -----------------------------------------------------------------------
// File         : ModbusRange.h
// Description  : Modbus message range specifies the alias for different
//              : actions for devices.
// Author       : wangdong
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
// 2010-12-23  WangDong          Recode in Visual Studio 2010
// 2003-01-29  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#pragma once
#include "stdafx.h"


namespace modbus
{

// --------------------------------------------------------------------------
// { modbus::CMessageRange }
class CMessageRange
{
public:
    CMessageRange()
		:m_bWrite( false ), m_nBeginAlias( 0 ), m_nEndAlias( 0 )
	{
	}

	CMessageRange( int nBeginAlias, int nEndAlias )
		:m_bWrite( false ), m_nBeginAlias( nBeginAlias ), m_nEndAlias( nEndAlias )
	{
	}

private:
	bool m_bWrite;
	int m_nBeginAlias;
	int m_nEndAlias;

public:
	/// Set
	void SetWrite( bool bWrite )             { m_bWrite = bWrite; }
	void SetBeginAlias( int nAlias )         { m_nBeginAlias = nAlias; }
	void SetEndAlias( int nAlias )           { m_nEndAlias = nAlias; }

	/// Get
	bool IsWrite()                           const { return m_bWrite; }
	int GetBeginAlias()                      const { return m_nBeginAlias; }
	int GetEndAlias()                        const { return m_nEndAlias; }
};


// --------------------------------------------------------------------------
// { modbus::CMesageRanges }
class CMessageRanges : public std::vector<CMessageRange>
{
private:
	CString m_strActionName;

public:
	/// Set
	void SetActionName( CString strName )      { m_strActionName = strName; }
	void SetActionName( LPCTSTR pcStrName )    { m_strActionName = pcStrName; }   

	/// Get
	CString GetActionName()                    const { return m_strActionName; }
	LPCTSTR GetActionName_C()                  const { return (LPCTSTR)m_strActionName; }
};


// --------------------------------------------------------------------------
// { modbus::CMessageActions }
class CMessageActions : public std::map< CString, CMessageRanges >
{
public:
	CMessageRanges* find_name( CString strActionName )
	{
		iterator it = find( strActionName );
		if( it != end())
		{
			return &(it->second);
		}

		return NULL;
	}

	const CMessageRanges* find_name( CString strActionName ) const
	{
        const_iterator it = find( strActionName );
		if( it != end())
		{
			return &(it->second);
		}

		return NULL;
	}

	CMessageRanges* at( size_t index_ )
	{
		if( index_ < size())
		{
			iterator it = begin();
			std::advance( it, index_ );

			return &(it->second);
		}

		return NULL;
	}

	const CMessageRanges* at( size_t index_ ) const
	{
		if( index_ < size() )
		{
            const_iterator it = begin();
			std::advance( it, index_ );

			return &(it->second);
		}

		return NULL;
	}
};

} // namespace modbus