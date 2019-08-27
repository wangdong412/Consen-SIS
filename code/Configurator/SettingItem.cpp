// -----------------------------------------------------------------------
// $Workfile: SettingItem.cpp $
// Author/Date  : WandDong 2010-12-14 11:00:00 (UTC-6:00)
// Description  : Setting item and Vector for the document
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2010, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Configurator/SettingItem.cpp  1     12/14/10 11:00a WangDong $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-12-14  WangDong          BCB5 -> VS2010
// 2000-01-20  Ryan Van Slooten  Initial Creation
// -----------------------------------------------------------------------

#include "stdafx.h"
#include "Configurator.h"
#include "SettingItem.h"

#include "AliasItem.h"     // Added for sizeof(CAliasItem)  in CSettingItem::SetAlias
#include "OptionItem.h"    // Added for sizeof(COptionList) in CSettingItem::GetVariantString
#include "ConfiguratorDoc.h"

// =======================================================================
/// CSettingItem 构造函数与析构函数
CSettingItem::CSettingItem()
	         :m_bVisible( true ),
              m_pParent( NULL ),
			  m_vChildVector( NULL ),
			  m_pAlias( NULL ),
			  m_pOptionList( NULL ),
			  m_uDepth( 0 ),
			  m_uItemAppType( 0 )
{
	// vt: VT_EMPTY
	::VariantInit( &m_avMin );
	::VariantInit( &m_avMax );

	// Value: 0 (except "string")
	V_I8( &m_avMin ) = 0;
	V_I8( &m_avMax ) = 0;
}

CSettingItem::~CSettingItem()
{
	::VariantClear( &m_avMin );
	::VariantClear( &m_avMax );

	SAFE_DELETE( m_vChildVector );
}

// =======================================================================
// Function name : SetRange
// Description   : m_strMin, m_strMax, m_strRange
//                 m_avMin,  m_strMax
// Return type   : void
// Parameters    : 
// Author        : WangDong 2011.04.19 16:00:00
// =======================================================================
void CSettingItem::SetRange( CString strMin, CString strMax )
{
	m_strMin = strMin;
	m_strMax = strMax;
	m_strRange = m_strMin + L"-" + m_strMax;

	SetRangeValue( m_avMin, true );	
	SetRangeValue( m_avMax, false );
}

// =======================================================================
// Function name : SetAlias
// Description   : 1. Set m_pAlias;
//                 2. If RangeMin / RangeMax exists, 
//                    2.1 set m_avMin / m_avMax type with the pAlias's type
//                    2.2 m_strRange: pcStrRangMin - pcStrRangMax
// Return type   : void
// Parameters    : pAlias
// Author        : WangDong 2010.12.15 17:00:00
// =======================================================================
void CSettingItem::SetAlias( CAliasItem* pAlias )
{
	//1. Set m_pAlias
    m_pAlias = pAlias;

	//2.Set m_avMin and m_avMax type with the pAlias's type
	if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		const VARIANT& v = pAlias->valVar();

		if( m_strMin.CompareNoCase(L"") != 0 )
		{ 
			V_VT( &m_avMin)  = V_VT( &v );
			SetRangeValue( m_avMin, true );	
		}

		if( m_strMax.CompareNoCase(L"") != 0 )
		{
			V_VT( &m_avMax)  = V_VT( &v );
			SetRangeValue( m_avMax, false );

			m_strRange = m_strMin + L"-" + m_strMax;
		}
	}
}

//-----------------------------------------------------------------------
// Function name : SetRangeValue
// Description   : If m_strMin / m_strMax exists, 
//                 set m_avMin / m_avMax value
// Return type   : void
// Parameters    : v:           Reference of m_avMin or m_avMax
//                 bIsRangeMin: true: m_avMin; false, m_avMax 
// Author        : WangDong 2010.12.27 17:00:00
//------------------------------------------------------------------------
void CSettingItem::SetRangeValue( VARIANT& v, bool bIsRangeMin )
{
	LPCTSTR lpszValue = (LPCTSTR)m_strMin;

	if( !bIsRangeMin )
	{
		lpszValue = (LPCTSTR)m_strMax;
	}

	ASSERT( ::wcslen(lpszValue) > 0 );
	if( ::wcslen(lpszValue) == 0 )
	{
		return;
	}

	switch( V_VT( &v ) )
	{
	    case VT_I2:   // SHORT
			{
			    int nValue = 0;
                ::swscanf_s( lpszValue, L"%d", &nValue );  
			    V_I2( &v ) = static_cast<short>(nValue);
			}
		break;

		case VT_UI2:   // USHORT
			{
			    UINT uValue = 0;
			    ::swscanf_s( lpszValue, L"%u", &uValue );  
			    V_UI2( &v ) = static_cast<USHORT>(uValue);
			}
		break;

		case VT_I4:     // long
			{
			    long lValue = 0;
				::swscanf_s( lpszValue, L"%ld", &lValue );
				V_I4( &v ) = lValue;
			}
		break;

		case VT_R4:
			{
				float fValue = 0.0;
                ::swscanf_s( lpszValue, L"%f", &fValue );
				V_R4( &v ) = fValue;
			}
		break;

		default:
			V_UI8( &v ) = 0;
		break;
	}
}

// =======================================================================
// Function name : GetSubItemCount
// Description   : Get m_vChildVector's size
// Return type   : If m_vChildVector is NULL, return 0;
//                 else, return m_vChildVector's size.
// Parameters    : None
// Author        : WangDong 2010.12.14 15:00:00
// =======================================================================
UINT CSettingItem::GetSubItemCount() const
{
    if( m_vChildVector == NULL )
	{
		return 0;
	}

	return m_vChildVector->size();
}

// =======================================================================
// Function name : AddSubItem
// Description   : Set Parent and Depth of the Sub SettingItem
//                 and then add this sub SettingItem to its parent's m_vChildVector
// Return type   : void
// Parameters    : pSubSettingItem: Child SettingItem to be added
// Author        : WangDong 2010.12.14 15:00:00
// =======================================================================
void CSettingItem::AddSubItem( CSettingItem* pSubSettingItem )
{
	ASSERT( pSubSettingItem );
	if( !pSubSettingItem )
	{
		return;
	}

	if( m_vChildVector == NULL )
	{
	    m_vChildVector = new  CSettingVector();
	}

	// Set Parent
	pSubSettingItem->SetParent( this );

	// Set Depth
	pSubSettingItem->SetDepth( this->GetDepth() + 1 );

	m_vChildVector->push_back( pSubSettingItem );
}

// =======================================================================
// Function name : AddAliases
// Description   : pRangeSetting: Name, "Aliases xxxxx-xxxxx"
//                                Parent, this, 
//                                Depth， 1
//                                m_vChildVector <- pNewSetting
//                 pNewSetting: Set its Name, Parent(pRangeSetting), Depth(2)
//                                      m_pAlias, Variable
// Return type   : void
// Parameters    : pAliasMap:     
//                 nBeginAlias: Begin Alias 
//                 nEndAlias:   End Alias
// Application   : Only used to construct m_aSettingsAlias's element
// Author        : WangDong 2010.12.16 14:00:00
// =======================================================================
void CSettingItem::AddAliases(CAliasMap* pAliasMap, int nBeginAlias, int nEndAlias)
{
	if( CheckReadPtr( pAliasMap, sizeof(pAliasMap)))
	{
		CAliasMap::iterator it = pAliasMap->lower_bound( nBeginAlias );
		if( it == pAliasMap->end())
		{
			return;
		}

		CAliasMap::iterator eit = pAliasMap->upper_bound( nEndAlias );

		CSettingItem* pRangeSetting = NULL;
		CAliasItem* pAlias = NULL;
		int nStart = 0;
		int nEnd = 0;

		while( it != eit )
		{
			pAlias = it->second;
			if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
			{
				/* TS350: 如果应用类型不是COMM,且不匹配，则不加入
				if((theApp.GetProductType() == CConfiguratorApp::TS350)         &&
			       (pAlias->GetItemAppType() != CConfiguratorDoc::APPTYPE_COMM) &&     // 0: APPTYPE_COMM
				   (pAlias->GetItemAppType() != theApp.GetAppType()))
				{
                    it++;
					continue;
				}*/

				if( pRangeSetting == NULL )
				{
				    nStart = pAlias->GetAlias();
				    nEnd = pAlias->GetAliasEnd();

				    CAliasMap::iterator ait = it;
				    while( ait != eit)
				    {
					    CAliasItem* temp = ait->second;
					    if( CheckReadPtr( temp, sizeof(CAliasItem)))
					    {
					        if((temp->GetAlias() - nEnd) > 2 ) 
					        {
						        break;
					        }

					        nEnd = temp->GetAliasEnd();
					    }
					    else
					    {
						    break;
					    }

						ait++;
				    }

				    CString s;
				    s.Format( IDS_ALIASES_TREEITEM, nStart, nEnd ); 

				    pRangeSetting = new CSettingItem();
				    pRangeSetting->SetName( s );
					AddSubItem( pRangeSetting );
				}//if( pRangeSetting == NULL )

				CSettingItem* pNewSetting = new CSettingItem();
				pNewSetting->SetName( pAlias->GetDescription());
				pNewSetting->SetVariableName( pAlias->GetName());
				pNewSetting->SetAlias( pAlias );
				pRangeSetting->AddSubItem( pNewSetting );

				if( pAlias->GetAliasEnd() == nEnd )
				{
                    pRangeSetting = NULL;
				}
			}//if

			it++;
		}//while
	}
}

// =======================================================================
// Function name : IsBoolean
// Description   : 
// Return type   : void
// Parameters    : bool: true
// Author        : WangDong 2011.01.04 10:50:00
// =======================================================================
bool CSettingItem::IsBoolean() const
{
	bool bResult = false;

    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		if( m_pOptionList == NULL )
		{
		    if( m_pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN )
		    {
                bResult = true;
		    }
		}
	}

	return bResult;
}

//------------------------------------------------------------------------
// Function name : IsChecked / IsCheckedCmp
// Description   : 
// Return type   : void
// Parameters    : bool: true
// Author        : WangDong 2011.01.04 10:55:00
//------------------------------------------------------------------------
bool CSettingItem::IsChecked() const
{
	bool bResult = false;

    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		if( m_pOptionList == NULL )
		{
		    if( m_pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN )
		    {
			    if( m_pAlias->valBool())
			    {
                    bResult = true;
			    }
		    }
		}
	}

	return bResult;

}
//-------------------------------------------------------------------------

bool CSettingItem::IsCheckedCmp( bool bCompareFile ) const
{
    bool bResult = false;

    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		if( m_pOptionList == NULL )
		{
		    if( m_pAlias->GetDataType() == CAliasItem::ALIAS_BOOLEAN )
		    {
				VARIANT_BOOL bCheck = bCompareFile ? m_pAlias->cmpFileBool() : m_pAlias->remBool();
			    if( bCheck)
			    {
                    bResult = true;
			    }
			}
		}
	}

	return bResult;
}

// =======================================================================
// Function name : SetValue
// Description   : 
// Return type   : 
// Parameters    : 
// Author        : WangDong 2011.01.10 15:30:00
// =======================================================================
void CSettingItem::SetValue( CString strValue )
{
    if( CheckWritePtr( m_pAlias, sizeof(CAliasItem)))
	{
		m_pAlias->SetValue( strValue );
	}
}

// =======================================================================
// Function name : GetValue / GetValueCmp
// Description   : 
// Return type   : HRESULT
// Parameters    : 
// Author        : WangDong 2011.01.04 14:30:00
// =======================================================================
HRESULT CSettingItem::GetValue(CString& strValue) const
{
    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
	    return GetVariantString( strValue, m_pAlias->valVar());
	}
	else
	{
	    strValue = L"";
		return S_OK;
	}
}
//------------------------------------------------------------------------

HRESULT CSettingItem::GetValueCmp( CString& strValue, bool bCompareFile ) const
{
    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		if( bCompareFile )
		{
			return GetVariantString( strValue, m_pAlias->cmpFileVar());
		}
		else
		{
			return GetVariantString( strValue, m_pAlias->remVar());
		}
	}
	else
	{
		strValue = L"";
		return S_OK;
	}
}
//------------------------------------------------------------------------

HRESULT CSettingItem::GetVariantString( CString& strValue, const VARIANT& v ) const
{
	if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		int nValue = 0;

        if( m_pAlias->GetDataType() == CAliasItem::ALIAS_FLOAT )
		{
            nValue = static_cast<int>(V_R4(&v));
		}
        else
		{
            nValue = V_I4(&v);
		}

		// OptionList: OptionItem's Name attribute
        if( CheckReadPtr( m_pOptionList, sizeof(COptionList)))
		{
			strValue = m_pOptionList->FindNameByValue( static_cast<UINT>(nValue));
			if( strValue.IsEmpty())
			{
				strValue.Format( IDS_INVALID_OPTION, nValue ); 
			}
		}
		else // Others
		{
            VariantToString(v, strValue);
		}

		return S_OK;
	}

    return E_POINTER;
}

// =======================================================================
// Function name : IsRangeValid
// Description   : 
// Return type   : Valid: true; InValid: false
// Parameters    : strValue: Value to be checked
// Author        : WangDong 2011.01.10 18:30:00
// =======================================================================
bool CSettingItem::IsRangeValid( CString strValue )
{
	bool bValid = true;

    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		if( m_strRange.IsEmpty())
	    {
		    return bValid;
	    }

		int nSuccessFields = 0;
		switch( m_pAlias->GetDataType())
		{
		    case CAliasItem::ALIAS_INTEGER:       // short
			    {    
				    ASSERT((V_VT(&m_avMin) == VT_I2) && (V_VT(&m_avMax) == VT_I2));
				    int nValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%d", &nValue );
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
					    if((nValue < V_I2(&m_avMin)) || (nValue > V_I2(&m_avMax)))
					    {
                            bValid = false; 
					    }   
				    }
			    }
		    break;
		    case CAliasItem::ALIAS_UINTEGER:       // USHORT
			    {
				    ASSERT((V_VT(&m_avMin) == VT_UI2) && (V_VT(&m_avMax) == VT_UI2));
	                UINT uValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%u", &uValue );
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
					    if((uValue < V_UI2(&m_avMin)) || (uValue > V_UI2(&m_avMax)))
					    {
                            bValid = false; 
					    }   
				    }
			    }
		    break;
		    case  CAliasItem::ALIAS_FLOAT:
		    case  CAliasItem::ALIAS_FLOATSWAP:   // float
			    {
					ASSERT((V_VT(&m_avMin) == VT_R4) && (V_VT(&m_avMax) == VT_R4));
				    float fValue = 0.0;
				    nSuccessFields = ::swscanf_s( strValue, L"%f", &fValue );       
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
						if((fValue < V_R4(&m_avMin)) || (fValue > V_R4(&m_avMax)))
					    {
                            bValid = false; 
					    } 
				    }
			    }
		    break;
		    case CAliasItem::ALIAS_LONG:
		    case CAliasItem::ALIAS_LONGSWAP:     // long
			    {
				    ASSERT((V_VT(&m_avMin) == VT_I4) && (V_VT(&m_avMax) == VT_I4));
				    long lValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%ld", &lValue );        //long 为32位时，没有问题
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
					    if((lValue < V_I4(&m_avMin)) || (lValue > V_I4(&m_avMax)))
					    {
                            bValid = false; 
					    } 
				    }
			    }
		    break;
		    default:
			    break;
		}//switch
	}//if

	return bValid;
}

// =======================================================================
// Function name : ValidateSettingValue
// Description   : If value is invalid, Set m_avLocal and m_bDirtyLocal flag
// Return type   : true: less than RangeMin; false: grater than RangeMax
// Parameters    : strValue
// Author        : WangDong 2011.01.10 19:30:00
// =======================================================================
int CSettingItem::ValidateSettingValue( CString strValue )
{
	OverRangeType eORType = VALID;

	if( IsRangeValid(strValue))
	{
		return eORType;
	}

	if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
	{
		int nSuccessFields = 0;
		switch( m_pAlias->GetDataType())
		{
		    case CAliasItem::ALIAS_INTEGER:       // short
			    {    
				    ASSERT((V_VT(&m_avMin) == VT_I2) && (V_VT(&m_avMax) == VT_I2));
				    int nValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%d", &nValue );
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
					    if( nValue < V_I2(&m_avMin))
						{
							eORType = OR_MIN;
							m_pAlias->valI2( V_I2(&m_avMin));    // m_avLocal, m_bDirtyLocal
						}
						else if( nValue > V_I2(&m_avMax))
						{
							eORType = OR_MAX;
                            m_pAlias->valI2( V_I2(&m_avMax));  
						}
				    }
			    }
		    break;
		    case CAliasItem::ALIAS_UINTEGER:       // USHORT
			    {
				    ASSERT((V_VT(&m_avMin) == VT_UI2) && (V_VT(&m_avMax) == VT_UI2));
	                UINT uValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%u", &uValue );
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
						if( uValue < V_UI2(&m_avMin))
						{
							eORType = OR_MIN;
							m_pAlias->valUI2( V_UI2(&m_avMin));    // m_avLocal, m_bDirtyLocal
						}
						else if( uValue > V_UI2(&m_avMax))
						{
							eORType = OR_MAX;
                            m_pAlias->valUI2( V_UI2(&m_avMax));  
						}
				    }
			    }
		    break;
		    case  CAliasItem::ALIAS_FLOAT:
		    case  CAliasItem::ALIAS_FLOATSWAP:   // float
			    {
					ASSERT((V_VT(&m_avMin) == VT_R4) && (V_VT(&m_avMax) == VT_R4));
				    float fValue = 0.0;
				    nSuccessFields = ::swscanf_s( strValue, L"%f", &fValue );       
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
						if( fValue < V_R4(&m_avMin))
						{
							eORType = OR_MIN;
							m_pAlias->valR4( V_R4(&m_avMin));    // m_avLocal, m_bDirtyLocal
						}
						else if( fValue > V_R4(&m_avMax))
						{
							eORType = OR_MAX;
                            m_pAlias->valR4( V_R4(&m_avMax));  
						}
				    }
			    }
		    break;
		    case CAliasItem::ALIAS_LONG:
		    case CAliasItem::ALIAS_LONGSWAP:     // long
			    {
				    ASSERT((V_VT(&m_avMin) == VT_I4) && (V_VT(&m_avMax) == VT_I4));
				    long lValue = 0;
				    nSuccessFields = ::swscanf_s( strValue, L"%ld", &lValue );        //long 为32位时，没有问题
				    ASSERT( nSuccessFields );
				    if( nSuccessFields )
				    {
						if( lValue < V_I4(&m_avMin))
						{
							eORType = OR_MIN;
							m_pAlias->valI4( V_I4(&m_avMin));    // m_avLocal, m_bDirtyLocal
						}
						else if( lValue > V_I4(&m_avMax))
						{
							eORType = OR_MAX;
                            m_pAlias->valI4( V_I4(&m_avMax));  
						}
				    }
			    }
		    break;
		    default:
			    break;
		}//switch
	}//if 

	return eORType;
}

// =======================================================================
// Function name : IsPIDSettingItem
// Description   : 
// Return type   : True: if has three child settingitem: PB, RES, RATE
// Parameters    : pSettingVector
// Author        : WangDong 2011.02.15 14:35:00
// =======================================================================
bool CSettingItem::IsPIDSettingItem( CSettingVector* pSettingVector )
{
	bool bResult = false;
	int nItemCount = 0;

    if( CheckReadPtr( pSettingVector, sizeof(CSettingVector)))
	{
		// Check if has three child settingitems
        if( pSettingVector->size() == 3 )
		{
			CSettingVector::iterator it = pSettingVector->begin();
			CSettingVector::iterator eit = pSettingVector->end();
			while( it != eit )
			{
				CSettingItem* pSetting = *it;
				CString strVariable = pSetting->GetVariableName();
				strVariable.MakeUpper();                             
				if((strVariable.Find( L"PB") != -1)  ||
				   (strVariable.Find( L"RES") != -1) ||
				   (strVariable.Find( L"RATE") != -1))
				{
					nItemCount++;
					it++;
				}
				else
				{
					nItemCount = 0;
                    break;
				}
			}//while

			if( nItemCount == 3 )
			{
                bResult = true;
			}
		}//if( pSettingVector->size() == 3 )
	}//if

	return bResult;
}

// =======================================================================
///  CSettingVector 析构函数
CSettingVector::~ CSettingVector()
{
    clear();
}


void CSettingVector::clear()
{
    iterator it = begin();
    iterator eit = end();
	while( it != eit )
	{
		SAFE_DELETE( *it );
        ++it;
	}

	erase( begin(), end() );

	m_lTotalCount = 0;
}

// =======================================================================
// Function name : UpdateTotalCount
// Description   : Calculate Setting Item Count in this vector
// Return type   : void
// Parameters    : 
// Author        : WangDong 2010.12.15 15:30:00
// =======================================================================
void CSettingVector::UpdateTotalCount()
{
    m_lTotalCount = 0;

	iterator it = begin();
	iterator eit = end();

	while( it != eit )
	{
		CSettingItem* p = *it;
		if( p->GetChildVector())   // Have child nodes
		{
			p->GetChildVector()->UpdateTotalCount();
			m_lTotalCount += p->GetChildVector()->GetTotalCount();
		}

		m_lTotalCount++;
		it++;
	}
}

// =======================================================================
// Function name : FindHelpName
// Description   : Find a specific setting item according to its help name
// Return type   : NULL: if not found
//                 CSettingItem*: which help name is strHelpName
// Parameters    : strHelpName
// Author        : WangDong 2011.02.15 14:35:00
// =======================================================================
CSettingItem* CSettingVector::FindHelpName( CString strHelpName )
{
	if( strHelpName.IsEmpty())
	{
		return NULL;
	}

	CSettingItem* Result = NULL;

    iterator it = begin();
	iterator eit = end();
	while( it != eit )
	{
		CSettingItem* pSetting = *it;
		CString s = pSetting->GetHelpName();
		if( strHelpName.CompareNoCase(s) == 0 )
		{
			Result = pSetting;
			break;
		}
		else
		{
			if( pSetting->GetChildVector())
			{
				Result = pSetting->GetChildVector()->FindHelpName(strHelpName);
			}
		}

		it++;
	}// while

	return Result;
}

//--------------------------------------------------------------------------
