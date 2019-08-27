#include "StdAfx.h"
#include "VersionCtl.h"

// ===============================================================
// Function name  : Construction
// Description    : 
// Return         : 
// Parameters     : 
// Author         : WangDong 2012.08.07 9:00:00
// ===============================================================
CVersion::CVersion( float fVersion )
	:m_fXMLVer( fVersion )
	,m_pFirmSet( new intset())
{
}

CVersion::CVersion( const CVersion &ori )
	:m_fXMLVer( ori.m_fXMLVer )
	,m_pFirmSet( new intset())
{
	m_pFirmSet->insert( ori.m_pFirmSet->begin(), ori.m_pFirmSet->end());
}

// ===============================================================
// Function name  : Destruction
// Description    : 
// Return         : 
// Parameters     : 
// Author         : WangDong 2012.08.07 9:20:00
// ===============================================================
CVersion::~CVersion()
{
	if( m_pFirmSet != NULL )
	{
		m_pFirmSet->clear();
		SAFE_DELETE( m_pFirmSet );
	}
}

// ===============================================================
// Function name  : operator=
// Description    : 
// Return         : reference of CVersion
// Parameters     : 
// Author         : WangDong 2012.08.07 9:30:00
// ===============================================================
CVersion& CVersion::operator=( const CVersion &rhs )
{
	// avoid assign itself
	if( this == &rhs )
	{
		return *this;
	}

	m_fXMLVer = rhs.m_fXMLVer;
	m_pFirmSet->clear();
	m_pFirmSet->insert( rhs.m_pFirmSet->begin(), rhs.m_pFirmSet->end());

	return *this;
}

// ===============================================================
// Function name  : Add
// Description    : Add code to set
// Return         : true:  add successful
//                  false: if set aleady has this set
// Parameters     : 
// Author         : WangDong 2012.08.07 10:00:00
// ===============================================================
bool CVersion::Add( const int code )
{
	if( m_pFirmSet == NULL )
	{
		return false;
	}

	std::pair< intset::iterator, bool > result;
	result = m_pFirmSet->insert( code );
	return result.second;
}

// ===============================================================
// Function name  : FindNewestCode
// Description    : 
// Return         : >0: The newest firmware code; others -1
// Parameters     : 
// Author         : WangDong 2012.08.07 11:00:00
// ===============================================================
int CVersion::FindNewestCode()
{
    if( NULL == m_pFirmSet )
	{
		return -1;
	}

	intset::reverse_iterator it = m_pFirmSet->rbegin();
	return *it;
}

// ===============================================================
// Function name  : FindCode
// Description    : 
// Return         : true, if the code is in the set
// Parameters     : 
// Author         : WangDong 2012.08.07 13:50:00
// ===============================================================
bool CVersion::FindCode( const int code ) const
{
	if( NULL == m_pFirmSet )
	{
		return false;
	}

	return (m_pFirmSet->count( code ) > 0);
}





// ===============================================================
// Function name  : Construction/Destruction
// Description    : 
// Return         : 
// Parameters     : 
// Author         : WangDong 2012.08.07 11:00:00
// ===============================================================
CVersionCtl::CVersionCtl(void)
{
}

// need to test and confirm
CVersionCtl::~CVersionCtl(void)
{
	iterator it = begin();
	iterator eit = end();
	while( it != eit )
	{
		for( vermaps::iterator ver_it = it->second->begin(); ver_it != it->second->end(); ver_it++ )
		{
			SAFE_DELETE( ver_it->second );
		}

		it->second->clear();
		SAFE_DELETE( it->second );

		it++;
	}

	clear();
}

// ===============================================================
// Function name  : Add
// Description    : 
// Return         : true:  add successful
//                  false: if aleady has this item
// Parameters     : 
// Author         : WangDong 2012.08.07 10:50:00
// ===============================================================
bool CVersionCtl::Add( UINT type, vermaps *vers  )
{
	if( NULL == vers )
	{
		return false;
	}

	std::pair< CVersionCtl::iterator, bool > result;
	result = insert( value_type( type, vers ));
	return result.second;
}

// ===============================================================
// Function name  : GetNewestCode
// Description    : 
// Return         : >0: The newest firmware code; others -1
// Parameters     : type: Product type, such as DSC100
// Author         : WangDong 2012.08.07 11:00:00
// ===============================================================
int CVersionCtl::GetNewestCode( const UINT type )
{
	// Check if configurator has this product type
	iterator it = find( type );
	if( end() == it )  // not found
	{
		return -1;
	}

	if((NULL == it->second) || (it->second->size() == 0))
	{
		return -1;
	}

	vermaps::reverse_iterator lastit = it->second->rbegin();
	if( NULL == lastit->second )
	{
		return -1;
	}
	
	return lastit->second->FindNewestCode();
}

// ===============================================================
// Function name  : GetXMLVersionFromCode
// Description    : 
// Return         : xml file version if success, others 0.0
// Parameters     : type: Product type, such as DSC100
//                  code: Firmware code
// Author         : WangDong 2012.08.07 13:00:00
// ===============================================================
float CVersionCtl::GetXMLVersionFromCode( const UINT type, const int code )
{
	float fversion = 0.0;

	// Product type
	iterator it = find( type );
	if((end() == it) || (NULL == it->second))
	{
		return fversion;
	}

	vermaps::iterator ver_it = it->second->begin();
	vermaps::iterator ver_eit = it->second->end();
	while( ver_it != ver_eit )
	{
		if((ver_it->second != NULL) && (ver_it->second->FindCode( code )))
		{
			fversion = ver_it->second->GetXMLVersion();
			break;
		}

		ver_it++;
	}

	return fversion;
}

// ===============================================================
// Function name  : GetNewestCodeFromXML
// Description    : 
// Return         : newest code of the xml file if success, others -1
// Parameters     : type: Product type, such as DSC100
//                  fversion: xml file version
// Author         : WangDong 2012.08.07 13:00:00
// ===============================================================
int CVersionCtl::GetNewestCodeFromXML( const UINT type, const float fversion )
{
	// Check if configurator has this product type
	iterator it = find( type );
	if( end() == it )  // not found
	{
		return -1;
	}

	if((NULL == it->second) || (it->second->size() == 0))
	{
		return -1;
	}

	vermaps::iterator xmlit = it->second->find( fversion );
	if( NULL == xmlit->second )
	{
		return -1;
	}

	return xmlit->second->FindNewestCode();
}

// ===============================================================
// Function name  : IsXMLMatchCode
// Description    : Check if the current xml file can match the code
// Return         : true, xml can match the code;
// Parameters     : type:    Product type, such as DSC100
//                  fversion:XML file version to be checked
//                  code:    Firmware code
// Author         : WangDong 2012.08.07 14:00:00
// ===============================================================
bool CVersionCtl::IsXMLMatchCode( const UINT type, const float fversion, const int code ) const
{
	const_iterator it = find( type );
	if((end() == it) || (NULL == it->second))
	{
		return false;
	}

	vermaps::const_iterator ver_it = it->second->find( fversion );
	if((it->second->end() == ver_it) || (NULL == ver_it->second))
	{
		return false;
	}

	return ver_it->second->FindCode( code );
}

// ====================================================================
// Function name: CanMatch
// Description  : Check if Configurator can match the product and xml file
// Return type  : 0: OK;
//                1: Product mismatch
//                2: xml mismatch
// Parameters     : type: Product type, such as DSC100
//                  fversion: xml file version
// Author         : WangDong 2012.08.08 10:00:00
// ===============================================================
int CVersionCtl::CanMatch( const UINT type, const float fversion ) const
{
	// Check if product type match
	const_iterator it = find( type );
	if( end() == it )  // not found
	{
		return 1;      // product mismatch
	}

	// Check if xml file match
	ASSERT( it->second != NULL );
	vermaps::const_iterator ver_it = it->second->find( fversion );
	if( it->second->end() == ver_it )
	{
		return 2;      // xml file mismatch
	}

	return 0;
}