// -----------------------------------------------------------------------
// $Workfile: ModbusMessage.cpp $
// Author/Date  : rjvs 2002-01-28 10:20:22 (UTC-6:00)
// Description  : Modbus message holds a buffer to the input and output
//              : data, and an array of the aliases referenced by the
//              : message.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/ModbusMessage.cpp 12    2/18/03 2:37p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-02-24  Wang Dong         BCB5 to Visual Studio 2010
// 2002-06-04  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "ModbusMessage.h"

namespace modbus
{

const int DefaultMessageSize = 260;          // Modbus Serial:256; ModBus Tcp: 256+6-2=260

/// <summary>
///   </summary>
/// <created>
///   rjvs 2002-01-25 16:50:50 (UTC-6:00)</created>

CMessage::CMessage(void)
	: m_nBegin          (-1),
      m_nEnd            (-1),
      m_bWrite          (false),
      m_bCopyValue      (false),
	  m_bModbusTcp      (false),
	  m_hr              (S_OK),
	  m_dwRefresh       (0),
      m_dwBegin         (0),
      m_dwElapsed       (0),
      m_nInputCount     (0),
      m_nOutputCount    (0),
      m_nEstimateCount  (0),
      m_nRepeatCount    (1),
      m_nSuccessCount   (0),                   // Response successful msg count
      m_nFailureCount   (0),
      m_vInputBuffer    (DefaultMessageSize),
      m_vOutputBuffer   (DefaultMessageSize),
      m_vAliases        ()
{
}

/// <summary>
///   </summary>
/// <created>
///   rjvs 2002-01-25 16:50:39 (UTC-6:00)</created>
CMessage::~CMessage(void)
{
	m_vInputBuffer.clear();
	m_vOutputBuffer.clear();
	m_vAliases.clear();
}

/// <summary>
/// alias address count
/// </summary>
/// <return>
/// </return>
/// <created>
///   rjvs 2002-01-25 16:56:57 (UTC-6:00)</created>
int
CMessage::AliasCount() const
{
    if((m_nBegin == -1) && (m_nEnd == -1))
    {
        return 0;
    }

    return (m_nEnd - m_nBegin) + 1;
}

/// <summary>
/// CAliasItem count
/// </summary>
/// <return>
/// </return>
/// <created>
///   rjvs 2002-01-25 16:56:33 (UTC-6:00)</created>
int
CMessage::ItemCount() const
{
    return m_vAliases.size();
}

/// <summary>
///   Returns the alias in the array at the index specified.</summary>
/// <param name="Index">
///   The Index of array element desired.</param>
/// <return>
///   The alias item, however it preforms the default throw of vector if
///   index is out-of-bounds.</return>
/// <created>
///   rjvs 2002-08-26 14:46:22 (UTC-6:00)</created>
CAliasItem*
CMessage::Item( int Index )
{
    CAliasMap::iterator it = m_vAliases.begin();
    std::advance( it, Index );
    if( it != m_vAliases.end())
	{
        return it->second;
	}
    else
	{
        return NULL;
	}
}

/// <summary>
///   Returns the alias in the array at the index specified.</summary>
/// <param name="Index">
///   The Index of array element desired.</param>
/// <return>
///   The alias item, however it preforms the default throw of vector if
///   index is out-of-bounds.</return>
/// <created>
///   rjvs 2002-08-26 14:46:45 (UTC-6:00)</created>
const CAliasItem*
CMessage::Item(int Index) const
{
    CAliasMap::const_iterator it = m_vAliases.begin();
    std::advance( it, Index );
    if( it != m_vAliases.end())
	{
        return it->second;
	}
    else
	{
        return NULL;
	}
}

/// <summary>
///   Finds the alias in the array with the specified number.</summary>
/// <param name="Number">
///   The alias number desired.</param>
/// <return>
///   The alias with the desired number, or NULL if it is not in the array.</return>
/// <created>
///   rjvs 2002-08-26 14:53:18 (UTC-6:00)</created>
CAliasItem*
CMessage::FindAlias( int ANumber )
{
    return m_vAliases.find_num( ANumber );
}

const CAliasItem*
CMessage::FindAlias( int ANumber ) const
{
    return m_vAliases.find_num( ANumber) ;
}

/// <summary>
///   Finds the alias in the array with the specified name.</summary>
/// <param name="AName">
///   The alias number desired.</param>
/// <return>
///   The alias with the desired number, or NULL if it is not in the array.</return>
/// <created>
///   rjvs 2003-01-28 11:10:43 -06:00</created>
CAliasItem*
CMessage::FindName( LPCTSTR AName )
{
    return m_vAliases.find_name( AName );
}

const CAliasItem*
CMessage::FindName( LPCTSTR AName ) const
{
    return m_vAliases.find_name( AName );
}

/// <summary>
///   </summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 09:21:10 (UTC-6:00)</created>
int
CMessage::InputSize() const
{
    return m_vInputBuffer.size();
}

/// <summary>
///   </summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 09:21:31 (UTC-6:00)</created>
int
CMessage::OutputSize() const
{
    return m_vOutputBuffer.size();
}

/// <summary>
///   </summary>
/// <param name="n">
///   </param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 09:22:38 (UTC-6:00)</created>
/// <seealso cref="" />
void
CMessage::InputResize( int n )
{
    m_vInputBuffer.resize( n );
}

/// <summary>
///   </summary>
/// <param name="n">
///   </param>
/// <created>
///   rjvs 2002-01-28 09:23:14 (UTC-6:00)</created>
void
CMessage::OutputResize( int n )
{
    m_vOutputBuffer.resize( n );
}

/// <summary>
///   Beginning of input buffer</summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 10:27:07 (UTC-6:00)</created>
BYTE*
CMessage::InputBegin()
{
	ByteVector::iterator it = m_vInputBuffer.begin();

    return &(*it);
}

/// <summary>
///   Beginning of output buffer</summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 10:27:31 (UTC-6:00)</created>
BYTE*
CMessage::OutputBegin()
{
	ByteVector::iterator it = m_vOutputBuffer.begin();

    return &(*it);
}

/// <summary>
///   Beginning of input buffer</summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 10:28:09 (UTC-6:00)</created>
const BYTE*
CMessage::InputBegin() const
{
	ByteVector::const_iterator it = m_vInputBuffer.begin();

    return &(*it);
}

/// <summary>
///   Beginning of output buffer</summary>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 10:28:39 (UTC-6:00)</created>
const BYTE*
CMessage::OutputBegin() const
{
    ByteVector::const_iterator it = m_vOutputBuffer.begin();

    return &(*it);
}

/// <summary>
///   </summary>
/// <param name="TODO">
///   </param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-08-09 17:05:08 (UTC-6:00)</created>
void CMessage::ClearInput()
{
    m_nInputCount = 0;
    //std::fill_n(InputBegin(), 16, 0);
	std::fill( m_vInputBuffer.begin(), m_vInputBuffer.end(), 0 );
}

/// <summary>
/// </summary>
/// <param name="pAlias">
/// </param>
/// <created>
///   rjvs 2002-01-25 16:50:24 (UTC-6:00)</created>
void
CMessage::Add( CAliasItem* pAlias )
{
    ASSERT(CheckWritePtr(pAlias, sizeof(CAliasItem)));
    if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
    {
        if( m_vAliases.size() == 0 )
		{
            m_nBegin = pAlias->GetAlias();
		}

        m_vAliases.Add( pAlias );
        m_nEnd = pAlias->GetAliasEnd();
    }
}

/// <summary>
///   Updates the 'Active' status of message variables.</summary>
/// <param name="Value">
///   Indicates if the aliases are to be marked as 'Active' or not.</param>
/// <return>
///   The number of items marked.</return>
/// <created>
///   rjvs 2003-02-18 09:06:37 -06:00</created>
size_t
CMessage::UpdateActive( bool Value )
{
    CAliasMap::iterator  it = m_vAliases.begin();
    CAliasMap::iterator eit = m_vAliases.end();

    while( it != eit )
    {
        it->second->SetActive( Value );
        ++it;
    }

    return m_vAliases.size();
}

/*
/// <summary>
///   </summary>
/// <created>
///   rjvs 2002-08-26 14:51:57 (UTC-6:00)</created>
struct alias_array_find_num
  : public std::unary_function<CAliasArray::value_type, bool>
{
  int n_;
  alias_array_find_num(int n) : n_( n )  {}
  bool operator ()(const CAliasArray::value_type& v) const
    { return (n_ == v->GetAlias()); }
};*/

}// namespace modbus
