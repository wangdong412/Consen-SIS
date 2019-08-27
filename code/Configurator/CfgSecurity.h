// -----------------------------------------------------------------------
// $Workfile: CfgSecurity.hpp $
// Author/Date  : rjvs 2002-12-11 11:16:42 -06:00
// Description  : Handles the application security levels.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2002, Triconex Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/CfgSecurity.hpp 1     1/20/03 11:00a Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-04-14  Wang Dong         Visual Studio 2010
// 2002-12-11  Ryan Van Slooten  Initial code
// -----------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------
// { CCfgSecurity }
class CAliasItem;

class CCfgSecurity
{
public:
    CCfgSecurity(void);
    ~CCfgSecurity(void);

public:
    enum Security_t
    {
        SECURITY_NONE     = 0,
        SECURITY_READONLY = 1,
        SECURITY_TUNING   = 2,
        SECURITY_CONFIG   = 3,
        SECURITY_ADMIN    = 4
    };

private:
    Security_t m_eAccessLevel;

public:
    bool IsValid(void) const
    {
        return (m_eAccessLevel != SECURITY_NONE);
    }

    Security_t GetLevel()       const { return m_eAccessLevel; }
    CString GetName()           const;

    bool Login( const CString& Password );
    void LoginGuest( void );
    void Logout( void );

    bool CanTune()       const { return m_eAccessLevel >= SECURITY_TUNING; }
    bool CanConfig()     const { return m_eAccessLevel >= SECURITY_CONFIG; }
    bool CanAdmin()      const { return m_eAccessLevel >= SECURITY_ADMIN; }
    bool CanEditAlias( const CAliasItem* pAlias ) const;
};


