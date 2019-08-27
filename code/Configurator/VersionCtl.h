#pragma once
#include "stdafx.h"
 
typedef std::set<int> intset;
class CVersion
{
public:
	explicit CVersion( float fVersion );
	CVersion( const CVersion &ori );
	~CVersion();

	CVersion& operator=( const CVersion &rhs );

private:
	float m_fXMLVer;      // XML Version
	intset *m_pFirmSet;   // Firmware serial number

public:
	float GetXMLVersion() const   { return m_fXMLVer; }
	void SetXMLVersion( float v ) { m_fXMLVer = v; }

	bool Add( const int code );
	int FindNewestCode();
	bool FindCode( const int code ) const;
};

typedef std::map< float, CVersion*> vermaps;

class CVersionCtl : public std::map< UINT, vermaps* >
{
public:
	CVersionCtl(void);
	~CVersionCtl(void);

public:
	bool Add( UINT type, vermaps *vers  );

	int   GetNewestCode( const UINT type );
	float GetXMLVersionFromCode( const UINT type, const int code );
	int   GetNewestCodeFromXML( const UINT type, const float fversion );
	bool  IsXMLMatchCode( const UINT type, const float fversion, const int code ) const;
	int   CanMatch( const UINT type, const float fversion ) const;
};

