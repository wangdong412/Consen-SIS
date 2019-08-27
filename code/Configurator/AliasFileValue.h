// -----------------------------------------------------------------------
// $Workfile: AliasValue.hpp $
// Author       : Ryan Van Slooten
// Description  : Declaration of temporary alias value class
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, La Marque, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/AliasValue.hpp 1     1/20/03 11:00a Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-03-28  Wangdong          BCB5 to VS2010: Add Record    
// 2003-01-13  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------
// Forward declarations

class CAliasItem;
    class CAliasScaling;

class CAliasMap;
    class COwnedAliasMap;

class CAliasValue;
    class CAliasValueMap;

// -----------------------------------------------------------------------
// { CAliasFileValue }
class CAliasFileValue
{
public:
	CAliasFileValue(void);
	CAliasFileValue( LPCTSTR lpszName, LPCTSTR lpszValue, bool bCapture );
    CAliasFileValue( const CString& strName, const CString& strValue, bool bRecord );
    CAliasFileValue( const CAliasItem* pAlias );
	~CAliasFileValue(void);

private:
    CString         m_strName;
    _variant_t      m_avLocal;
    bool            m_bDirty;
	bool            m_bRecord;

public:
	// Name
	const CString&     GetName()                               const { return m_strName; }
    LPCTSTR            GetName_C()                             const { return (LPCTSTR)m_strName; }
    
    void               SetName( LPCTSTR szValue )              { m_strName = szValue; }
    void               SetName( const CString& szValue )       { m_strName = szValue; }

	// VARIANT
    _variant_t&        GetValue()                              { return m_avLocal; }
    const _variant_t&  GetValue()                              const { return m_avLocal; }
    CString            GetValueStr()                           const { return m_avLocal; }
	void               SetValue( LPCTSTR strValue )            { m_avLocal = strValue; }

	// Dirty
    bool               GetDirty()                              const { return m_bDirty; }
    void               SetDirty( bool Value )                  { m_bDirty = Value; }

	// Record
	bool               GetRecord()                            const { return m_bRecord; }
    void               SetRecord( bool Value )                { m_bRecord = Value; }
};

// -----------------------------------------------------------------------

class CAliasFileValueMap : public std::map<CString, CAliasFileValue>
{
    typedef std::map<CString, CAliasFileValue> inherited;
    
public:
    CAliasFileValueMap();
    virtual ~CAliasFileValueMap();

    void Add( const CAliasItem* pAlias );
    void Add( const CAliasFileValue& pValue );
};

CAliasFileValueMap& operator >> ( CAliasFileValueMap& AliasFileValue, CAliasMap* pAliases );
CAliasFileValueMap& operator << ( CAliasFileValueMap& AliasFileValue, const CAliasMap* pAliases );

