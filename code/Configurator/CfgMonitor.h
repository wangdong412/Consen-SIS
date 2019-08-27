// -----------------------------------------------------------------------
// $Workfile: CfgMonitor.cpp $
// Author/Date  : wangdong 2010-11-24 10:30:00 (UTC-6:00)
// Description  : Monitor item and array for the document
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2010, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Configurator/CfgMonitor.cpp  1     11/24/10 11:00a WangDong $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-11-24    WangDong         Demo
// -----------------------------------------------------------------------
#pragma once         //Added by wangdong: 编译时，保证头文件只被编译一次
#include "stdafx.h"

class CAliasItem;

class CCfgMonitorValue
{
public:
	CCfgMonitorValue();

private:
	CString m_sVariable;
	CString m_sTitle;
	long    m_lFontColor;
	bool    m_bDirty;
	VARIANT m_avLastValue;
	CString m_strValue;

	CAliasItem* m_pAlias;
	UINT m_uItemAppType;

public:
	/// Set
	// Variable
	void SetVariable( CString sVariable )     { m_sVariable = sVariable; }
	void SetVariable( LPCTSTR pcStrVar )      { m_sVariable = pcStrVar; }

	// Title
	void SetTitle( CString sTitle )           { m_sTitle = sTitle; }
	void SetTitle( LPCTSTR pcStrTitle )       { m_sTitle = pcStrTitle; }

	// Font Color
	void SetFontColor( long lColor )          { m_lFontColor = lColor; }

	// Alias
	void SetAlias( CAliasItem* pAlias )       { m_pAlias = pAlias; }

	// ItemAppType
	void SetItemAppType( UINT value )
	{
		if( value > 2 )
		{
			value = 0;
		}

		m_uItemAppType = value;
	}

	/// Get
	CString GetVariable()                     const { return m_sVariable; }
	LPCTSTR GetVariable_C()                   const { return (LPCTSTR)m_sVariable; }

	CString GetTitle()                        const { return m_sTitle; }
	LPCTSTR GetTitle_C()                      const { return (LPCTSTR)m_sTitle; }

	long GetFontColor()                       const { return m_lFontColor; }

	CAliasItem* GetAlias()                    const { return m_pAlias; }

	UINT GetItemAppType()                     const { return m_uItemAppType; }

	bool Update( bool bRefresh = false );
	const CString& GetValue()                 const { return m_strValue; }
};

//---------------------------------------------------------------------------

struct CCfgMonitorValueFind
    :   public std::unary_function<CCfgMonitorValue, bool>
{
    LPCTSTR s_;
    CCfgMonitorValueFind( LPCTSTR s ) : s_( s )  {}

    bool operator ()( const CCfgMonitorValue& v ) const
    {
        return ::lstrcmpi( s_, v.GetVariable()) == 0;
    }
};

//---------------------------------------------------------------------------

typedef std::vector<CCfgMonitorValue> CCfgMonitorValues;