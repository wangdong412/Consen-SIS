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
// 2010-11-24  WangDong          Demo
// -----------------------------------------------------------------------

#include "stdafx.h"
#include "Configurator.h"
#include "CfgMonitor.h"

#include "AliasItem.h"


// ¹¹Ôìº¯Êý
CCfgMonitorValue::CCfgMonitorValue()
	             :m_lFontColor( -1 ),
                  m_pAlias( NULL ),
				  m_bDirty( false )
{
	::VariantInit( &m_avLastValue );    // vt: VT_EMPTY
	V_I8( &m_avLastValue ) = 0;
}

/// <summary>Updates the value of the monitored item.</summary>
/// <param name="bRefresh">Forces a refresh of the item.</param>
/// <return>True if the value is dirty or refreshed, false if it is the same.</return>
/// <created>rjvs 2002-11-11 11:26:18 (UTC-6:00)</created>
bool CCfgMonitorValue::Update( bool bRefresh ) // = false
{
    if( bRefresh )
	{
        m_bDirty = true;
	}

    if( CheckReadPtr( m_pAlias, sizeof(CAliasItem)))
    {
        switch( m_pAlias->GetDataType())
        {
            case CAliasItem::ALIAS_BOOLEAN:
                if( V_BOOL(&m_avLastValue) != m_pAlias->remBool())
                {
                    V_BOOL(&m_avLastValue) = m_pAlias->remBool();
                    m_bDirty = true;
                }
            break;

            case CAliasItem::ALIAS_INTEGER:
                if (V_I2(&m_avLastValue) != m_pAlias->remI2())
                {
                    V_I2(&m_avLastValue) = m_pAlias->remI2();
                    m_bDirty = true;
                }

                if( m_bDirty )
                {
                    m_strValue.Format( L"%d", m_pAlias->remI2());
                }
            break;

            case CAliasItem::ALIAS_UINTEGER:
                if (V_UI2(&m_avLastValue) != m_pAlias->remUI2())
                {
                    V_UI2(&m_avLastValue) = m_pAlias->remUI2();
                    m_bDirty = true;
                }

                if( m_bDirty )
                {
                    m_strValue.Format( L"%d", m_pAlias->remUI2());
                }
            break;

            case CAliasItem::ALIAS_LONG:      /*@fallthrough@*/
            case CAliasItem::ALIAS_LONGSWAP:
                if (V_I4(&m_avLastValue) != m_pAlias->remI4())
                {
                    V_I4(&m_avLastValue) = m_pAlias->remI4();
                    m_bDirty = true;
                }

                if( m_bDirty )
                {
                     m_strValue.Format( L"%d", m_pAlias->remI4());
                }
            break;

            case CAliasItem::ALIAS_FLOAT:     /*@fallthrough@*/
            case CAliasItem::ALIAS_FLOATSWAP:
                if (V_R4(&m_avLastValue) != m_pAlias->remR4())
                {
                    V_R4(&m_avLastValue) = m_pAlias->remR4();
                    m_bDirty = true;
                }

                if( m_bDirty )
                {
                    m_strValue.Format( L"%4.2f", m_pAlias->remR4());
                }
            break;

			default:
			break;
        }
    }

    return m_bDirty;
}



