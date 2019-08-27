// -----------------------------------------------------------------------
// $Workfile: CfgSecurity.cpp $
// Author/Date  : rjvs 2002-12-11 11:16:42 -06:00
// Description  : Handles the application security levels.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2010, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/CfgSecurity.cpp 2     2/12/03 12:05p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-04-14  Wang Dong         Visual Studio 2010
// 2002-12-11  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------

#include "stdafx.h"
#include "CfgSecurity.h"

#include "AliasItem.h"

CCfgSecurity::CCfgSecurity(void)
	         :m_eAccessLevel( SECURITY_NONE )
{
}


CCfgSecurity::~CCfgSecurity(void)
{
}

/// <summary>
///   Returns the name of the current security level.</summary>
/// <return>
///   String containing the name of the current security level.</return>
/// <created>
///   rjvs 2002-12-11 10:55:03 -06:00</created>
CString CCfgSecurity::GetName() const
{
    CString Result;

    switch( m_eAccessLevel )
    {
        case SECURITY_TUNING:
            Result = L"Tune";
            break;

        case SECURITY_CONFIG:
            Result = L"Configure";     
            break;

        case SECURITY_ADMIN:
            Result = L"Manage";
            break;

        case SECURITY_READONLY: //@fallthrough@
        default:
            Result = L"Guest";
            break;
    }

    return Result;
}

/// <summary>
///   Attempts to validate a login and change the security level based on a supplied password.</summary>
/// <param name="Password">
///   The password that the user entered.</param>
/// <param name="Guest">
///   Indicates the user does not want to validate the password and login with
///   read-only access.</param>
/// <return>
///   True if the document allows the security level, otherwise false if the login is rejected.</return>
/// <created>
///   rjvs 2002-12-11 11:02:36 -06:00</created>
bool CCfgSecurity::Login( const CString& Password )
{
    bool Result = false;

	if( Password.CompareNoCase( L"TUNE") == 0 )
    {
        m_eAccessLevel = SECURITY_TUNING;
        Result = true;
    }
    else if( Password.CompareNoCase( L"CONFIGURE") == 0 )
    {
        m_eAccessLevel = SECURITY_CONFIG;
        Result = true;
    }
    else if( Password.CompareNoCase( L"MANAGE") == 0 )
    {
        m_eAccessLevel = SECURITY_ADMIN;
        Result = true;
    }
    else if((Password.CompareNoCase( L"GUEST") == 0) ||
            (Password.CompareNoCase( L"VIEW") == 0))
    {
        m_eAccessLevel = SECURITY_READONLY;
        Result = true;
    }
    else
    {
        Result = false;
    }

    return Result;
}

/// <summary>
///   Logs in with no password and read-only access.</summary>
/// <created>
///   rjvs 2002-12-11 11:02:36 -06:00</created>
void CCfgSecurity::LoginGuest(void)
{
    m_eAccessLevel = SECURITY_READONLY;
}

/// <summary>
///   Changes the current security level to read-only.</summary>
/// <created>
///   rjvs 2002-12-11 11:01:04 -06:00</created>
void CCfgSecurity::Logout(void)
{
    m_eAccessLevel = SECURITY_NONE;
}

/// <summary>
///   Checks the permissions for a particular alias to see if the user is
///   allowed to edit or download the alias.</summary>
/// <param name="pAlias">
///   The alias parameter to check for permissions.</param>
/// <return>
///   True if the user is allowed to edit the alias, otherwise false.</return>
/// <created>
///   rjvs 2002-12-12 08:32:46 -06:00</created>
bool CCfgSecurity::CanEditAlias( const CAliasItem* pAlias ) const
{
    if((m_eAccessLevel == SECURITY_NONE) || (m_eAccessLevel == SECURITY_READONLY))
	{
        return false;
	}

    if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
    {
        if( pAlias->IsReadOnly())
		{
            return false;
		}

		switch( pAlias->GetProtectType())
        {
            // Changed to only allow tuning to change aliases explicitly marked
            // as tuning.
            case CAliasItem::PROTECT_TUNE:          // Requires tune password
                return ( m_eAccessLevel >= SECURITY_TUNING );

            case CAliasItem::PROTECT_DEFAULT:       // No protection
            case CAliasItem::PROTECT_SETPOINT:      // Writeable setpoints
            case CAliasItem::PROTECT_CONFIGURE:     // Requires configuration password
            case CAliasItem::PROTECT_RUN:           // Run protected
                return ( m_eAccessLevel >= SECURITY_CONFIG );

            case CAliasItem::PROTECT_READONLY:      // Read only
            default:
                return false;
        }
    }

    return false;
}

