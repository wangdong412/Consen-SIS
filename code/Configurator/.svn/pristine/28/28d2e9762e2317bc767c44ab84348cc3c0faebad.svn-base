// -----------------------------------------------------------------------
// File         : SettingItem.h
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
// 2010-12-13  WangDong          Changed from BCB5 to Visual Studio 2010
// 2000-01-20  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#pragma once

class CSettingVector;
class CAliasItem;
class COptionList;
class CAliasMap;

class CSettingItem
{
public:
	CSettingItem();
	~CSettingItem();

private:
	CString m_strName;
	CString m_strVariable;
	CString m_strMin;
	CString m_strMax;
	CString m_strRange;
	CString m_strUnits;
	CString m_strOptionList;
	CString m_strHelpName;
	bool    m_bVisible;

	VARIANT m_avMin;
	VARIANT m_avMax;

	CSettingItem* m_pParent;
	CSettingVector* m_vChildVector;

	CAliasItem* m_pAlias;
	COptionList* m_pOptionList;

	UINT m_uDepth;
	UINT m_uItemAppType;

public:
	typedef enum{ VALID, OR_MIN, OR_MAX } OverRangeType;

public:
	/// Set
	// Name
	void SetName( CString strName )                      { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )                    { m_strName = pcStrName; }

	// Variable
	void SetVariableName( CString strVar )               { m_strVariable = strVar; }
	void SetVariableName( LPCTSTR pcStrVar )             { m_strVariable = pcStrVar; }

	// RangeMin - string
	void SetRangeMinString( CString strRangeMin )        { m_strMin = strRangeMin; }
	void SetRangeMinString( LPCTSTR pcStrRangMin )       { m_strMin = pcStrRangMin; }

	// RangeMax - string
	void SetRangeMaxString( CString strRangeMax )        { m_strMax = strRangeMax; }
	void SetRangeMaxString( LPCTSTR pcStrRangMax )       { m_strMax = pcStrRangMax; }

	// RangeMin and  RangeMax
	void SetRange( CString strMin, CString strMax ); 

	// Range: RangeMin - RangeMax
	void SetRange( CString strRange )                    { m_strRange = strRange; }
	void SetRange( LPCTSTR pcStrRange )                  { m_strRange = pcStrRange; }

	// Units
	void SetUnits( CString strUnits )                    { m_strUnits = strUnits; }
	void SetUnits( LPCTSTR pcStrUnits)                   { m_strUnits = pcStrUnits; }

	// OptionList Name
	void SetOptionListName( CString strOptionList )      { m_strOptionList = strOptionList; }
	void SetOptionListName( LPCTSTR pcStrOptionList )    { m_strOptionList = pcStrOptionList; }

	// HelpName
	void SetHelpName( CString strHelpName )              { m_strHelpName = strHelpName; }
	void SetHelpName( LPCTSTR pcStrHelpName )            { m_strHelpName = pcStrHelpName; }

	// Visible
	void SetVisible( bool bVisible )                     { m_bVisible = bVisible; }

	// Parent Setting Item
	void SetParent( CSettingItem* pParent )              { m_pParent = pParent; }

	// Alias: m_pAlias; Type of m_avMin and m_avMax
	void SetAlias( CAliasItem* pAlias );

	// Set RangeMin or RangeMax Value: string -> int/uint/long/float...
	void SetRangeValue( VARIANT& v, bool bIsRangeMin );

	// OptionList
	void SetOptionList( COptionList* pOptList )          { m_pOptionList = pOptList; } 

	// Depth
	void SetDepth( UINT uDepth )                         { m_uDepth = uDepth; }

	// AppType
	void SetItemAppType( UINT value )
	{
		if( value > 2 )
		{
			value = 0;
		}

		m_uItemAppType = value;
	}


	/// Get
	// Name
	CString GetName()                   const { return m_strName; }
	LPCTSTR GetName_C()                 const { return (LPCTSTR)m_strName; }

	// Variable
	CString GetVariableName()           const { return m_strVariable; }
	LPCTSTR GetVariableName_C()         const { return (LPCTSTR)m_strVariable; }

	// RangeMin
	CString GetRangeMin()               const { return m_strMin; }
	LPCTSTR GetRangeMin_C()             const { return (LPCTSTR)m_strMin; }
	VARIANT GetRangeMin_V()             const { return m_avMin; }

	// RangeMax
	CString GetRangeMax()               const { return m_strMax; }
	LPCTSTR GetRangeMax_C()             const { return (LPCTSTR)m_strMax; }
	VARIANT GetRangeMax_V()             const { return m_avMax; }

	// Range string
	CString GetRange()                  const { return m_strRange; }
	LPCTSTR GetRange_C()                const { return (LPCTSTR)m_strRange; }

	// Units
	CString GetUnits()                  const { return m_strUnits; }
	LPCTSTR GetUnits_C()                const { return (LPCTSTR)m_strUnits; }

	// OptionListName
	CString GetOptionListName()         const { return m_strOptionList; }
	LPCTSTR GetOptionListName_C()       const { return (LPCTSTR)m_strOptionList; }
	
	// HelpName
	CString GetHelpName()               const { return m_strHelpName; }
	LPCTSTR GetHelpName_C()             const { return (LPCTSTR)m_strHelpName; }

	// Visible
	bool GetVisible()                   const { return m_bVisible; }

	// Parent
	CSettingItem* GetParent()           const { return m_pParent; }

	// ChildVector
	CSettingVector* GetChildVector()    const { return m_vChildVector; }

	// CAliasItem
	CAliasItem* GetAlias()              const { return m_pAlias; }

	// COptionList
	COptionList* GetOptionList()        const { return m_pOptionList; }

	// Depth
	UINT    GetDepth()                  const { return m_uDepth; }

	// ItemAppType
	UINT GetItemAppType()               const { return m_uItemAppType; }

	// size of m_vChildVector
	UINT GetSubItemCount() const;

	/// Others
	void AddSubItem( CSettingItem* pSubSettingItem );    // Add Single SettingItem to Parent's m_vChildVector
	void AddAliases(CAliasMap* pAliasMap, int nBeginAlias, int nEndAlias); 

	/// Get Value
	// bool
	bool IsBoolean() const;
	bool IsChecked() const;
    bool IsCheckedCmp( bool bCompareFile = false ) const;

	// Others
	// Value
	void SetValue( CString strValue );

	// Validate Value
	bool IsRangeValid( CString strValue );
    int  ValidateSettingValue( CString strValue );    // Only called when the strValue is invalid

	HRESULT  GetValue(CString& strValue) const;
    HRESULT  GetValueCmp(CString& strValue, bool bCompareFile = false ) const;
	HRESULT  GetVariantString( CString& strValue, const VARIANT& v ) const;

	// Check if PID items
	bool IsPIDSettingItem( CSettingVector* pSettingVector );
};

class CSettingVector : public std::vector<CSettingItem*>
{
private:
	long m_lTotalCount;        // Total Setting Item Count in this vector

public:
	CSettingVector(): m_lTotalCount(0) {}
	virtual ~CSettingVector();
    
	void clear(void);

	// Total Count
	long GetTotalCount()      const { return m_lTotalCount; }  // return total Setting Item Count
	void UpdateTotalCount();

	// Find
	CSettingItem* FindHelpName( CString strHelpName );
	
};