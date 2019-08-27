// -----------------------------------------------------------------------
// File         : AliasItem.h
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
// 12/06/10  WangDong          Changed from BCB5 to Visual Studio 2010
// 04/20/01  Ryan Van Slooten  Changed implementation to use STL elements
// 01/20/00  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#pragma once

class CAliasItem
{
public:
	CAliasItem();
	~CAliasItem();

    enum DataType
    {
        ALIAS_UNKNOWN,      // Unknown datatype
        ALIAS_BOOLEAN,      // Digital datatype
        ALIAS_INTEGER,      // 16-bit integer
        ALIAS_UINTEGER,     // 16-bit unsigned integer
        ALIAS_FLOAT,        // 32-bit IEEE floating point (LOWORD, HIWORD)
        ALIAS_FLOATSWAP,    // 32-bit IEEE floating point (HIWORD, LOWORD)
        ALIAS_LONG,         // 32-bit integer
        ALIAS_LONGSWAP,     // 32-bit integer
        ALIAS_STRING        // String datatype
    };

    enum ProtectType
    {
        PROTECT_DEFAULT,    // No protection
        PROTECT_READONLY,   // Read only
        PROTECT_SETPOINT,   // Writeable setpoints
        PROTECT_CONFIGURE,  // Requires configuration password
        PROTECT_TUNE,       // Requires tune password
        PROTECT_RUN,        // Run protected
		PROTECT_CALIBRATE   // Calibrate protected
    };

private:
	CString      m_strName;            // Name
	CString      m_strDesc;            // Desc
	CString      m_strAlias;           // Alias String
	int          m_nAlias;             // Alias
	UINT         m_uItemAppType;       // ItemAppType
    DataType     m_eDataType;          // DataType
	ProtectType  m_eProtectType;       // ProtectType
	BYTE         m_bySize;             // Alias Size(unsigned char)
	bool         m_bDirtyLocal;        // true: Local value was changed
	bool         m_bDirtyRemote;       // true: Remote value was changed

	VARIANT      m_avLocal;            // Local Value
	VARIANT      m_avRemote;           // Remote Value
	VARIANT      m_avCompareFile;      // Compare file

	bool         m_bRecord;            // History Record
	bool         m_bActive;            // true: In Monitor Range         

public:
	/// Set Properties
	// Name
	void SetName( CString strName )           { m_strName = strName; }
	void SetName( LPCTSTR pcStrName )         { m_strName = pcStrName; }  

	// Desc
	void SetDescription( CString strDesc )    { m_strDesc = strDesc; }
	void SetDescription( LPCTSTR pcStrDesc )  { m_strDesc = pcStrDesc; }

	// Alias String
	void SetAliasString( CString strAlias )   { m_strAlias = strAlias; }
	void SetAliasString( LPCTSTR pcStrAlias ) { m_strAlias = pcStrAlias; }

	// Alias
	void SetAlias( int nAlias )               { m_nAlias = nAlias; }

	// ItemAppType
	void SetItemAppType( UINT value )
	{
		if( value > 2 )
		{
			value = 0;
		}

		m_uItemAppType = value;
	}

	// DataType
	void SetDataType( DataType value );       // Set DataType and m_bySize(except String DataType)
	void SetStringSize( BYTE value );          // Length: Only for String( DateType )

	// Protect Type
	void SetProtectType( ProtectType value )  { m_eProtectType = value; }

	// m_avLocal value, and m_bDirtyLocal: include Default Value
	void SetValue( const CString& strValue, bool bCompareFile = false );   
	
	// Dirty Flag
	void SetDirtyLocal( bool bDirtyLocal )    { m_bDirtyLocal = bDirtyLocal; } 
	void SetDirtyRemote( bool bDirtyRemote )  { m_bDirtyRemote = bDirtyRemote; }

	// Record
	void SetRecord( bool bRecord )            { m_bRecord = bRecord; }

	// Active
	void SetActive( bool bActive )            { m_bActive = bActive; }


	/// Get Properties
	// Name
	LPCTSTR GetName()               const { return (LPCTSTR)m_strName; }

	// Desc
	LPCTSTR GetDescription()        const { return (LPCTSTR)m_strDesc; } 

	// Alias String
	CString GetAliasString()        const { return m_strAlias; }
	LPCTSTR GetAliasString_c()      const { return (LPCTSTR)m_strAlias; }

	// ItemAppType
	UINT GetItemAppType()           const { return m_uItemAppType; }

	// Alias: begin, end, size
	int GetAlias()                  const { return m_nAlias; }
	int GetAliasEnd()               const { return m_bySize ? m_nAlias+m_bySize-1 : m_nAlias; }
	BYTE GetAliasSize()             const { return m_bySize; }

	// DataType
	DataType GetDataType()          const { return m_eDataType; }

	// Protect Type
	ProtectType GetProtectType()    const { return m_eProtectType; }

	// Value
	void GetLocalValueString( CString &strValue ) const;

	// Record
	bool GetRecord()               const { return m_bRecord; }

	// Active
	bool GetActive()                const { return m_bActive; }

	// m_bDirtyLocal or m_bDirtyRemote
	bool IsDirtyLocal()             const { return m_bDirtyLocal; }
	bool IsDirtyRemote()            const { return m_bDirtyRemote; }

	// Get m_avLocal value
	const VARIANT& valVar()         const { return m_avLocal; }
	VARIANT_BOOL   valBool()        const { return V_BOOL(&m_avLocal); }
	int            valI2()          const { return V_I2( &m_avLocal ); }
	UINT           valUI2()         const { return V_UI2( &m_avLocal ); }
	long           valI4()          const { return V_I4( &m_avLocal ); }
	float          valR4()          const { return V_R4( &m_avLocal ); }
	const char*    valStr()         const { return V_I1REF(&m_avLocal); }

	// Set m_avLocal and m_bDirtyLocal flag except valString
	void           valBool( BOOL value );
	void           valI2( short value );
	void           valUI2( USHORT value );
	void           valI4( long value );
	void           valR4( float value );
	void           valString( const char* value );

	// const reference of m_avRemote
	const VARIANT& remVar()         const { return m_avRemote; }
	VARIANT_BOOL   remBool()        const { return (V_BOOL(&m_avRemote)); }
	int            remI2()          const { return V_I2( &m_avRemote ); }
	UINT           remUI2()         const { return V_UI2( &m_avRemote ); }
	long           remI4()          const { return V_I4( &m_avRemote ); }
	float          remR4()          const { return V_R4( &m_avRemote ); }
	ULONGLONG      remUI8()         const { return V_UI8( &m_avRemote );}

	// Set m_avRemote, and m_avRemote flag
	void           remBool( BOOL value );
	void           remI2( short value );
	void           remUI2( USHORT value );
	void           remI4( long value );
	void           remR4( float value );
	void           remString( const char* value );

	// const reference of m_avCompareFile
	const VARIANT& cmpFileVar()         const { return m_avCompareFile; }
	VARIANT_BOOL   cmpFileBool()        const { return (V_BOOL(&m_avCompareFile)); }
	int            cmpFileI2()          const { return V_I2( &m_avCompareFile ); }
	UINT           cmpFileUI2()         const { return V_UI2( &m_avCompareFile ); }
	long           cmpFileI4()          const { return V_I4( &m_avCompareFile ); }
	float          cmpFileR4()          const { return V_R4( &m_avCompareFile ); }

	HRESULT        SetDataDump( const BYTE* aDump );    

	// Copy, and Reset m_bDirtyRemote false
	void           CopyRemoteToLocal( void );
	void           CopyLocalToRemote( void );
	void           UpdateRemoteFlags( void );

	// Make databytes( in modbus message) when write
	HRESULT GetDataDump(BYTE* aDump) const;

    /// Others
	bool IsReadOnly()               const;
	bool IsWriteable()              const;
	bool IsTuning()                 const;
};

typedef std::set<CAliasItem::ProtectType> AliasProtectSet;
typedef std::map<int, CAliasItem*> CAliasMapInternal;
class CAliasMap : public CAliasMapInternal
{
	typedef CAliasMapInternal inherited;
	typedef std::map< CString, CAliasItem*> NameMap;

public:
    CAliasMap();
	virtual ~CAliasMap();        	

	NameMap m_AliasNameMap;
    CAliasItem*       find_name(LPCTSTR szName);
    const CAliasItem* find_name(LPCTSTR szName) const;   
    CAliasItem*       find_num(int nNumber);
    const CAliasItem* find_num(int nNumber) const;

    void              Add( CAliasItem* pAlias );
	void              clear(void);

	void SetDirtyLocal( bool bDirty );
	void SetDirtyRemote( bool bDirty );

	void SetActive( bool bActive );

	bool IsTuning();
	void UpdateRemoteFlags( int nBeginAlias, int nEndAlias );

	void RangeInclude( CAliasMap* pMap, int nBegin, int nEnd,
                       AliasProtectSet& P, bool bDirtyOnly );
    void RangeExclude( CAliasMap* pMap, int nBegin, int nEnd,
                       AliasProtectSet& P, bool bDirtyOnly );

public:
	CCriticalSection m_cirsec;
};

class COwnedAliasMap : public CAliasMap
{
public:
    // COwnedAliasMap destructor deletes all the contained aliases, whereas
    // CAliasMap does not delete the alias items.
    virtual ~COwnedAliasMap();

public:
	void clear(void);
};
