// -----------------------------------------------------------------------
// $WorkFile: $
// Author/Date  : rjvs 2002-08-09 16:30:39 (UTC-6:00)
// Description  : Modbus array that builds and hold the messages.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/ModbusVector.cpp 13    2/18/03 2:36p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-02-25  Wang Dong         BCB5 to Visual Studio 2010
// 2002-08-09  Ryan Van Slooten  Added Comment Block.  Moved code here.
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "ModbusMsgVector.h"

#include "AliasItem.h"
#include "ModbusMessage.h"
#include "Modbus.h"
#include "crc16.h"

#include "Configurator.h"  // res

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// --------------------------------------------------------------------------

namespace modbus
{

/// <summary>
///   Modbus message vector constructor.</summary>
/// <created>
///   rjvs 2002-08-09 16:53:34 (UTC-6:00)</created>
CModbusMsgVector::CModbusMsgVector(void)
	             :m_nUnitId( 1 ),
				  m_nMessageBytes( 0 ),
				  m_pAliasMap( NULL )
{
}

/// <summary>
///   Modbus message vector destructor.</summary>
/// <created>
///   rjvs 2002-08-09 16:54:02 (UTC-6:00)</created>
CModbusMsgVector::~CModbusMsgVector(void)
{
	clear();
}

/// <summary>
///   Clears the message vector, and 'Active' flag</summary>
/// <created>
///   rjvs 2002-08-09 17:51:38 (UTC-6:00)</created>
void CModbusMsgVector::clear()
{
	for( iterator it = begin(); it != end(); it++ )
	{
		SAFE_DELETE( *it );
	}

	erase( begin(), end());
	
	// Clear the 'Active' flag
	if( m_pAliasMap != NULL )
	{
		m_pAliasMap->SetActive( false );
	}
}

/// <summary>
///   If msg repeat count is 0, delete and erase the msg. </summary>
/// <created>
///   rjvs 2002-03-24 17:14:30 (UTC-6:00)</created>
void CModbusMsgVector::RemoveNonRepeatMessages()
{
	size_t nMsgIndex = 0;
	while( nMsgIndex < size())
	{
        CMessage* msg = at( nMsgIndex );
		if( CheckWritePtr( msg, sizeof(CMessage)))
		{
			if( msg->RepeatCount() == 0 )
			{
				SAFE_DELETE( msg );
				iterator it = begin() + nMsgIndex;
				ASSERT( it != end());
				erase( it );
			}
		}

		nMsgIndex++;
	}
}

/// <summary>
///   Sets the default alias map for the messages.</summary>
/// <param name="pAliasMap">
///   The default alias map for the messages.</param>
/// <created>
///   rjvs 2002-08-09 18:14:23 (UTC-6:00)</created>
void CModbusMsgVector::SetAliasMap( CAliasMap* pAliasMap )
{
	ASSERT(size() == 0);
    if( size() > 0 )
	{
        clear();
	}

    m_pAliasMap = pAliasMap;
}

/// <summary>
/// </summary>
/// <created>
///   rjvs 2002-01-25 16:00:13 (UTC-6:00)</created>
HRESULT CModbusMsgVector::BuildMessages( int BeginAlias, int EndAlias,
                                         int AliasesPerMessage, int AliasGapSize,
                                         bool bModbusTcp, bool bWrite, bool bCopyValue )
{
    return BuildMessages( m_pAliasMap, BeginAlias, EndAlias, AliasesPerMessage,
		                  AliasGapSize, bModbusTcp, bWrite, bCopyValue );
}


/// <summary>
///   Build Modbus Message </summary>
/// <param>
///   pAliasMap
///   BeginAlias:        first alias address
///   EndAlias:          end alias address
///   AliasesPerMessage: ?
///   AliasGapSize:      Zero-based number indicating the allowable size of gaps between
///                      aliases.  Zero means no gaps are allowed (all aliases must be
///                      consecutive or else another message is started).
///   bModbusTcp:        true, modbus tcp
///   bWrite:            true, write
///   bCopyValue:        true, read; false, readback
///  </param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-25 16:00:13 (UTC-6:00)
///   wangdong </created>
HRESULT CModbusMsgVector::BuildMessages( CAliasMap* pAliasMap, int BeginAlias, int EndAlias,
                                         int AliasesPerMessage, int AliasGapSize,
                                         bool bModbusTcp,
                                         bool bWrite, 
										 bool bCopyValue )
{
	try
	{
        ASSERT( pAliasMap );
	    if( !CheckReadPtr( pAliasMap, sizeof(CAliasMap)))
	    {
	        return S_FALSE;
	    }

		CSingleLock sl( &pAliasMap->m_cirsec );
		sl.Lock();
		ASSERT( sl.IsLocked());

        //GLogDebug("", Format("Build Messages %d-%d (Max=%d, Gap=%d, Write=%d, Copy=%d)",
                    //ARRAYOFCONST((BeginAlias, EndAlias, AliasesPerMessage, AliasGapSize,
                    //static_cast<int>(bWrite), static_cast<int>(bCopyValue))) ));

	    CAliasMap::iterator it = pAliasMap->lower_bound( BeginAlias );
	    if( it == pAliasMap->end())
	    {
            return E_INVALIDARG;
	    }

	    CAliasMap::iterator eit = pAliasMap->upper_bound( EndAlias );

	    // Adjust gap size (you will always have a gap of at least 1).
        if( AliasGapSize <= 0 )
	    {
            AliasGapSize = 1;
	    }
        else
	    {
            ++AliasGapSize;
	    }

	    // Start a new message
        CMessage* msg = new CMessage();
	    ASSERT( CheckWritePtr( msg, sizeof(CMessage)));
	    msg->m_bModbusTcp = bModbusTcp;
        msg->m_bWrite = bWrite;
        msg->m_bCopyValue = bCopyValue;

	    while( it != eit )
	    {
		    CAliasItem* pAlias = it->second;
		    if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		    {   
			    // Not include string alias
			    if( pAlias->GetDataType() != CAliasItem::ALIAS_STRING )
			    {
			        if( msg->ItemCount() != 0 )
			        {
				        bool bGapBreak = ((pAlias->GetAlias() - msg->m_nEnd) > AliasGapSize);
			            bool bSizeBreak = ((pAlias->GetAliasEnd() - msg->m_nBegin) >= AliasesPerMessage );

				        if( bGapBreak || bSizeBreak )
				        {
					        ASSERT((msg->m_nEnd - msg->m_nBegin) <= AliasesPerMessage );
					        HRESULT hr = CreateMessage( msg );
				            if( hr != S_OK )
					        {
						        return hr;
					        }

					        // Add msg to the vector
                            push_back( msg );

					        /*
					        GLogDebug("", Format("Created message %d-%d (Split: Gap=%d, Size=%d)",
                                      ARRAYOFCONST((msg->m_nBegin, msg->m_nEnd,
                                      static_cast<int>(bGapBreak),
                                      static_cast<int>(bSizeBreak))) ));*/

				            // Create a new Modbus Message
					        msg = new CMessage();
						    ASSERT( CheckWritePtr( msg, sizeof(CMessage)));
					        msg->m_bModbusTcp = bModbusTcp;
				            msg->m_bWrite = bWrite;
				            msg->m_bCopyValue = bCopyValue;
				        }//if( bGapBreak || bSizeBreak )
			        }//if( msg->ItemCount() != 0 )
			
		            msg->Add( pAlias );     // msg: m_vAliases, m_nBegin, m_nEnd
			    }//if( pAlias->GetDataType() != CAliasItem::ALIAS_STRING )
		     }//if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
		
		    it++;
	    }//while

	    if( msg->ItemCount() > 0 )
        {
            ASSERT((msg->m_nEnd - msg->m_nBegin) <= AliasesPerMessage);
            HRESULT hr = CreateMessage( msg );
            if( hr != S_OK )
		    {
                return hr;
		    }

            push_back(msg);
        }
        else
        {
            SAFE_DELETE( msg );
        }

	    sl.Unlock();

	    return S_OK;
    }//try
	catch(...)
	{
        AfxMessageBox( L"Exception occurs in CModbusMsgVector::BuildMessages!" );
		return S_OK;
	}
}

/// <summary>
///   </summary>
/// <param>
///   msg: Modbus message</param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-01-28 13:15:11 (UTC-6:00)</created>
HRESULT CModbusMsgVector::CreateMessage( CMessage* msg )
{
    if( !CheckWritePtr( msg, sizeof(CMessage)))
	{
		return E_POINTER;
	}

	// Offset: 1, 10001, 30001, 40001
	WORD wOffSet = AliasOffset( msg->m_nBegin );
	if( wOffSet == 0 )
	{
		return E_CLIENT_BADALIAS;
	}
	WORD wBeginAddress = static_cast<WORD>(msg->m_nBegin) - wOffSet;   // Begin Address

	// Alias Count
	int nAliasCount = msg->AliasCount();                     
    if( nAliasCount <= 0 )
	{
        return E_CLIENT_BADALIAS;                                      // "Modbus client bad alias."
	}

	// Initialize message counts
	msg->m_nInputCount = 0;
	msg->m_nOutputCount = 0;
	msg->m_nEstimateCount = 0;

	if( msg->m_bWrite )   
	{
		bool bMultiple = ((nAliasCount != 1) ? true : false);
		BYTE byWriteFuncCode = WriteFunctionCode( msg->m_nBegin, bMultiple );
		if( mbfnUnknown == byWriteFuncCode )
		{
		    return E_CLIENT_BADALIAS;
		}

		if((byWriteFuncCode == mbfnWriteCoil) || 
		   (byWriteFuncCode == mbfnWriteRegister))
		{
			CreateSingleWriteMessage( msg );
		}
		else if( byWriteFuncCode == mbfnWriteMultipleCoils )
		{
			return CreateWriteDigitalMessage( msg );
		}
		else
		{
			return CreateWriteAnalogMessage(msg );
		}
	}// if( msg->m_bWrite )  
	else
	{
		BYTE byReadFuncCode = ReadFunctionCode( msg->m_nBegin );    
		if( byReadFuncCode == mbfnUnknown )
		{
            return E_CLIENT_BADALIAS;                               
		}

		return CreateReadMessage( msg ); 
	}

	return S_OK;
}

/// <summary>
///   </summary>
/// <param>
///   msg: modbus message;
///   byFunction: single write function code
///   byBeginAddress: begin address
/// </param>
/// <return>
///   </return>
/// <created>
///   wangdong 2011-02-26 12:30:00 (UTC-6:00)</created>
HRESULT CModbusMsgVector::CreateSingleWriteMessage( CMessage* msg )
{
	if( !CheckWritePtr( msg, sizeof(CMessage)))
	{
		return E_POINTER;
	}

	CAliasItem* pAlias = msg->Item( 0 );
	if( !CheckReadPtr( pAlias, sizeof(CAliasItem)))
	{
		return E_CLIENT_BADALIAS;
	}

	BYTE byFunction = WriteFunctionCode( msg->BeginAlias(), false );
	if( byFunction == mbfnUnknown )
	{
		return E_CLIENT_BADALIAS; 
	}

    WORD wOffset = AliasOffset( msg->BeginAlias() );
	if( wOffset == 0 )
	{
		return E_CLIENT_BADALIAS;
	}
	WORD wBeginAddress = static_cast<WORD>(msg->BeginAlias()) - wOffset;

	/// 1. Outputcount
	int nOutputCount = 8;
    if( msg->m_bModbusTcp )
    {
        nOutputCount = 12;        // (8-2)+6
    }
    else
    {
        nOutputCount = 8; 
    }

    if( msg->OutputSize() < nOutputCount )
    {
	    msg->OutputResize( nOutputCount );
	}

	msg->m_nOutputCount = nOutputCount;

	/// 2. Outoffset
	int nOutOffset = 0;
	if( msg->m_bModbusTcp )
	{
        nOutOffset = 6;
	}

	/// 3. Make Output buffer
	ByteVector& v = msg->m_vOutputBuffer;
	v[0+nOutOffset] = static_cast<BYTE>(m_nUnitId);     // SlaveID
	v[1+nOutOffset] = byFunction;                       // Function Code  
	v[2+nOutOffset] = HIBYTE( wBeginAddress );          // Begin Address
	v[3+nOutOffset] = LOBYTE( wBeginAddress );

	if( byFunction == mbfnWriteCoil )
	{
		v[4+nOutOffset] = static_cast<BYTE>( pAlias->valBool() ? 0xff : 0x00 );
		v[5+nOutOffset] = 0;
	}
	else
	{
		v[4+nOutOffset] = HIBYTE( pAlias->valI2());
		v[5+nOutOffset] = LOBYTE( pAlias->valI2());
	}

	if( nOutOffset == 0 )  // Modbus Serial
	{
		WORD wCrc = 0;
		CrcModbusBlock( msg->OutputBegin(), msg->OutputCount()-2, &wCrc );

		v[6+nOutOffset] = HIBYTE( wCrc );
		v[7+nOutOffset] = LOBYTE( wCrc );
	}
	else                   // Modbus Tcp
	{
		ASSERT( nOutOffset == 6 );

		// Transaction identifier
		v[0] = 0;
		v[1] = 0;

		// Protocol identifier
		v[2] = 0;
		v[3] = 0;

		// Counts
		v[4] = HIBYTE( msg->m_nOutputCount - 6 );
		v[5] = LOBYTE( msg->m_nOutputCount - 6 );
	}

	/// 4. Set expected response size
	msg->m_nEstimateCount = nOutputCount;

	return S_OK;
}

/// <summary>
///   </summary>
/// <param>
///   msg: modbus message;
///   byBeginAddress: begin address
/// </param>
/// <return>
///   </return>
/// <created>
///   wangdong 2011-02-26 12:30:00 (UTC-6:00)</created>
HRESULT CModbusMsgVector::CreateReadMessage( CMessage* msg )
{
	if( !CheckWritePtr( msg, sizeof(CMessage)))
	{
		return E_POINTER;
	}

	int nAliasCount = msg->AliasCount();
	if( nAliasCount <= 0 )
	{
        return E_CLIENT_BADALIAS;                               
	}

	BYTE byFunction = ReadFunctionCode( msg->BeginAlias());
	if( byFunction == mbfnUnknown )
	{
		return E_CLIENT_BADALIAS; 
	}

    WORD wOffSet = AliasOffset( msg->BeginAlias() );
	if( wOffSet == 0 )
	{
		return E_CLIENT_BADALIAS;
	}
	WORD wBeginAddress = static_cast<WORD>(msg->BeginAlias()) - wOffSet;

	/// 1. Outputcount
    int nOutputCount = 8;

    if( msg->m_bModbusTcp )
	{
        nOutputCount = 12;        // (8-2)+6
	}
	else
	{
        nOutputCount = 8; 
	}

    if( msg->OutputSize() < nOutputCount )
	{
	    msg->OutputResize( nOutputCount );
	}

	msg->m_nOutputCount = nOutputCount;

	/// 2. OutOffset
	int nOutOffset = 0;
	if( msg->m_bModbusTcp )
	{
        nOutOffset = 6;
	}

	/// 3. Make Output buffer
	ByteVector& v = msg->m_vOutputBuffer;
	v[0+nOutOffset] = static_cast<BYTE>(m_nUnitId);     // SlaveID
	v[1+nOutOffset] = byFunction;                       // Function Code
	v[2+nOutOffset] = HIBYTE( wBeginAddress );          // Begin Address
	v[3+nOutOffset] = LOBYTE( wBeginAddress );         
	v[4+nOutOffset] = HIBYTE( nAliasCount );            // Count
	v[5+nOutOffset] = LOBYTE( nAliasCount );

	if( nOutOffset == 0 )
	{
		WORD wCrc = 0;
		CrcModbusBlock( msg->OutputBegin(), msg->OutputCount()-2, &wCrc );

		v[6+nOutOffset] = HIBYTE( wCrc );
		v[7+nOutOffset] = LOBYTE( wCrc );
	}
	else
	{
		ASSERT( nOutOffset == 6 );

		// Transaction identifier
		v[0] = 0;
		v[1] = 0;

		// Protocol identifier
		v[2] = 0;
		v[3] = 0;

		// Counts
		v[4] = HIBYTE( msg->m_nOutputCount - 6 );
		v[5] = LOBYTE( msg->m_nOutputCount - 6 );
	}

	/// 4. Set expected response size
	BYTE byDateBytes = 0;
	if((byFunction == mbfnReadCoilStatus) ||
	   (byFunction == mbfnReadInputStatus))
	{
        byDateBytes = (nAliasCount+7)/8;
	}
	else
	{
		byDateBytes = nAliasCount*2;
	}

	if( byDateBytes > (255-5))        // maximun: 250
	{
		return E_CLIENT_BADALIAS;
	}

	msg->m_nEstimateCount = RtuResponseLength( byFunction, byDateBytes, msg->m_bModbusTcp );

	return S_OK;
}

/// <summary>
///   </summary>
/// <param>
///   msg: modbus message 
/// </param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-02-20 15:57:24 (UTC-6:00)</created>
HRESULT CModbusMsgVector::CreateWriteDigitalMessage( CMessage* msg )
{
	if( !CheckReadPtr( msg, sizeof(CMessage)))
	{
		return E_POINTER;
	}

	BYTE byFunction = WriteFunctionCode( msg->BeginAlias(), true );
    if( byFunction == mbfnUnknown )
	{
       return E_CLIENT_BADALIAS;
	}

	WORD wOffset = AliasOffset(msg->BeginAlias());
    if( wOffset == 0 )
	{
        return E_CLIENT_BADALIAS;
	}
    WORD wBeginAddress = static_cast<WORD>(msg->BeginAlias()) - wOffset;

	// Alias Count
	int nAliasCount = msg->AliasCount();                     
    if( nAliasCount <= 0 )
	{
        return E_CLIENT_BADALIAS;                              
	}

	// Data counts
    int nDataBytes = ( nAliasCount + 7 ) / 8;
    if( nDataBytes > (255-9))         // >=250
	{
        return E_CLIENT_BADALIAS;
	}

    CAliasItem* pAlias = msg->Item(0);
    if( !CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
        return E_CLIENT_BADALIAS;
	}

	// OutOffset
	int nOutOffset = 0;
	if( msg->m_bModbusTcp )
	{
        nOutOffset = 6;

		if( msg->OutputSize() < (nDataBytes + 13))  // (9-2)+6
		{
            msg->OutputResize((nDataBytes + 13));
		}
		msg->m_nOutputCount = (nDataBytes + 13);
		msg->m_nEstimateCount = 12;
	}
	else
	{
		nOutOffset = 0;

		if( msg->OutputSize() < (nDataBytes + 9))
		{
            msg->OutputResize((nDataBytes + 9));
		}
		msg->m_nOutputCount = (nDataBytes + 9);
		msg->m_nEstimateCount = 8;
	}

	/// Make output buffer
	ByteVector& v = msg->m_vOutputBuffer;
	v[0+nOutOffset] = static_cast<BYTE>(m_nUnitId);
    v[1+nOutOffset] = byFunction;
    v[2+nOutOffset] = HIBYTE( wBeginAddress );
    v[3+nOutOffset] = LOBYTE( wBeginAddress );
    v[4+nOutOffset] = HIBYTE( nAliasCount );
    v[5+nOutOffset] = LOBYTE( nAliasCount );
    v[6+nOutOffset] = static_cast<BYTE>( nDataBytes );

	int nIndex = 7 + nOutOffset;

	// Data
	for( int nByteIndex=0; nByteIndex < nDataBytes; nByteIndex++ )
	{
		BYTE byData = 0;
		for( int nBitIndex = 0; nBitIndex < 8; nBitIndex++ )
		{
			if( nByteIndex*8 + nBitIndex + 1 > msg->ItemCount())
			{
				break;
			}

			pAlias = msg->Item( nByteIndex*8 + nBitIndex );
			ASSERT(CheckReadPtr( pAlias, sizeof(CAliasItem)));
            if( CheckReadPtr( pAlias, sizeof(CAliasItem)))
            {
			    if( pAlias->valBool())
				{
                    byData |= ( 1 << nBitIndex );
				}

				/*
				if (GLogCheck(TLogMessage::lmDebug3))
                {
                    AnsiString value;
                    pAlias->GetValue(value);

                    TVarRec v[] = { pAlias->GetAlias(), pAlias->GetName(), value };
                    AnsiString s = Format("Adding write alias: %d (%s) = %s", v, 2);

                    GLogDebug3("modbus::MessageVector::CreateWriteDigitalMessage", s);
                }*/
			}
		}//for

		v[nIndex] = byData;
		nIndex++;
	}//for

	/*
    int bit = 0;
    int value = 0;
    BYTE* p = v.begin() + index;
    for (int i = 0; i < msg->ItemCount(); ++i)
    {
      pAlias = msg->Item(i);
      ASSERT(CheckWritePtr(pAlias, sizeof(CAliasItem)));
      if (CheckWritePtr(pAlias, sizeof(CAliasItem)))
      {
        if (pAlias->valBool())
          value |= (1 << bit);
      } 

      ++bit;
      if ((bit >= 8) || ((i + 1) == msg->ItemCount()))
      {
        *p++ = static_cast<BYTE>(value);
        ++index;
        value = 0;
        bit = 0;
      }
    }*/

	if( nOutOffset == 0 )
	{
		WORD wCrc = 0;
		CrcModbusBlock( msg->OutputBegin(), msg->OutputCount()-2, &wCrc );

		v[msg->OutputCount()-2] = HIBYTE( wCrc );
		v[msg->OutputCount()-1] = LOBYTE( wCrc );
	}
	else
	{
		ASSERT( nOutOffset == 6 );

		// Transaction identifier
		v[0] = 0;
		v[1] = 0;

		// Protocol identifier
		v[2] = 0;
		v[3] = 0;

		// Counts
		v[4] = HIBYTE( msg->m_nOutputCount - 6 );
		v[5] = LOBYTE( msg->m_nOutputCount - 6 );
	}

	return S_OK;
}

/// <summary>
///   </summary>
/// <param>
///   msg: modbus message 
/// </param>
/// <return>
///   </return>
/// <created>
///   rjvs 2002-02-20 15:57:24 (UTC-6:00)</created>
HRESULT CModbusMsgVector::CreateWriteAnalogMessage( CMessage* msg )
{
    if( !CheckReadPtr( msg, sizeof(CMessage)))
	{
		return E_POINTER;
	}

	BYTE byFunction = WriteFunctionCode( msg->BeginAlias(), true );
    if( byFunction == mbfnUnknown )
	{
       return E_CLIENT_BADALIAS;
	}

	WORD wOffset = AliasOffset(msg->BeginAlias());
    if( wOffset == 0 )
	{
        return E_CLIENT_BADALIAS;
	}
    WORD wBeginAddress = static_cast<WORD>(msg->BeginAlias()) - wOffset;

	// Alias Count
	int nAliasCount = msg->AliasCount();                     
    if( nAliasCount <= 0 )
	{
        return E_CLIENT_BADALIAS;                              
	}

	// Data counts
    int nDataBytes = nAliasCount * 2;
    if( nDataBytes > (255-9))         // >=250
	{
        return E_CLIENT_BADALIAS;
	}

    CAliasItem* pAlias = msg->Item(0);
    if( !CheckReadPtr(pAlias, sizeof(CAliasItem)))
	{
        return E_CLIENT_BADALIAS;
	}

	// OutOffset
	int nOutOffset = 0;
	if( msg->m_bModbusTcp )
	{
        nOutOffset = 6;

		if( msg->OutputSize() < (nDataBytes + 13))  // (9-2)+6
		{
            msg->OutputResize((nDataBytes + 13));
		}
		msg->m_nOutputCount = (nDataBytes + 13);
		msg->m_nEstimateCount = 12;
	}
	else
	{
		nOutOffset = 0;

		if( msg->OutputSize() < (nDataBytes + 9))
		{
            msg->OutputResize((nDataBytes + 9));
		}
		msg->m_nOutputCount = (nDataBytes + 9);
		msg->m_nEstimateCount = 8;
	}

	/// Make output buffer
	ByteVector& v = msg->m_vOutputBuffer;
	v[0+nOutOffset] = static_cast<BYTE>(m_nUnitId);
    v[1+nOutOffset] = byFunction;
    v[2+nOutOffset] = HIBYTE( wBeginAddress );
    v[3+nOutOffset] = LOBYTE( wBeginAddress );
    v[4+nOutOffset] = HIBYTE( nAliasCount );
    v[5+nOutOffset] = LOBYTE( nAliasCount );
    v[6+nOutOffset] = static_cast<BYTE>( nDataBytes );

	int nIndex = 7 + nOutOffset;
    for( int i = 0; i < msg->ItemCount(); ++i ) 
    {
        pAlias = msg->Item(i);
        ASSERT(CheckReadPtr(pAlias, sizeof(CAliasItem)));
        if( CheckReadPtr(pAlias, sizeof(CAliasItem)))
        {
			/*
            if (GLogCheck(TLogMessage::lmDebug3))
            {
                AnsiString value;
                pAlias->GetValue(value);

                TVarRec v[] = { pAlias->GetAlias(), pAlias->GetName(), value };
                AnsiString s = Format("Adding write alias: %d (%s) = %s", v, 2);

                GLogDebug3("modbus::MessageVector::CreateWriteAnalogMessage", s);
            }*/

            pAlias->GetDataDump( &v[nIndex]);
            nIndex += ( 2 * pAlias->GetAliasSize());
        }
        else
        {
			v[nIndex] = 0;
			v[nIndex+1] = 0;
            nIndex += 2;
        }
    }

	if( nOutOffset == 0 )
	{
		WORD wCrc = 0;
		CrcModbusBlock( msg->OutputBegin(), msg->OutputCount()-2, &wCrc );

		v[msg->OutputCount()-2] = HIBYTE( wCrc );
		v[msg->OutputCount()-1] = LOBYTE( wCrc );	
	}
	else
	{
		ASSERT( nOutOffset == 6 );

		// Transaction identifier
		v[0] = 0;
		v[1] = 0;

		// Protocol identifier
		v[2] = 0;
		v[3] = 0;

		// Counts
		v[4] = HIBYTE( msg->m_nOutputCount - 6 );
		v[5] = LOBYTE( msg->m_nOutputCount - 6 );
	}

	return S_OK;
}

/// <summary>
///   Builds a loopback diagnostic message.</summary>
/// <return>
///   S_OK if successful.</return>
/// <created>
///   rjvs 2002-08-30 11:28:33 (UTC-6:00)</created>
HRESULT CModbusMsgVector::BuildLoopbackMessage( bool bModbusTcp )
{
	// Start a new message
	CMessage* msg = new CMessage();
	msg->m_bModbusTcp = bModbusTcp;
	msg->m_bWrite = false;
	msg->m_bCopyValue = false;

	// Initialize message counts
	int nOutOffset = 0;
	if( bModbusTcp )
	{
        msg->m_nOutputCount   = 12;
        msg->m_nEstimateCount = 12;
        msg->m_nInputCount    = 0;
		nOutOffset = 6;
	    
		if( msg->OutputSize() < 12 )
		{
            msg->OutputResize( 12 );
		}
	}
	else
	{
        msg->m_nOutputCount   = 8;
        msg->m_nEstimateCount = 8;
        msg->m_nInputCount    = 0;
		nOutOffset = 0;

		if( msg->OutputSize() < 8 )
		{
            msg->OutputResize( 8 );
		}
	}
	
	// Build the message
	ByteVector& v = msg->m_vOutputBuffer;
	v[0+nOutOffset] = static_cast<BYTE>( m_nUnitId );
	v[1+nOutOffset] = mbfnDiagnostic;                      // Diagnostic
	v[2+nOutOffset] = 0;
	v[3+nOutOffset] = 0;
	v[4+nOutOffset] = 0;
	v[5+nOutOffset] = 0;

	if((nOutOffset == 0) && (!bModbusTcp))
	{
        WORD wCrc = 0;
		CrcModbusBlock( msg->OutputBegin(), msg->OutputCount()-2, &wCrc );

		v[6+nOutOffset] = HIBYTE(wCrc);
	    v[7+nOutOffset] = LOBYTE(wCrc);
	}
	else
	{
        ASSERT( nOutOffset == 6 );

		// Transaction identifier
		v[0] = 0;
		v[1] = 0;

		// Protocol identifier
		v[2] = 0;
		v[3] = 0;

		// Counts
		v[4] = HIBYTE( msg->m_nOutputCount - 6 );
		v[5] = LOBYTE( msg->m_nOutputCount - 6 );
	}

	// Add the message to the vector, and return
	push_back( msg );

	return S_OK;
}

/// <summary>
///   Returns the total byte count of all the messages in the vector.</summary>
/// <return>
///   Integer</return>
/// <created>
///   2002-02-19 16:05:38 (UTC-6:00)</created>
int CModbusMsgVector::GetTotalByteCount() const
{
	int nCount = 0;

	const_iterator it = begin();
	const_iterator eit = end();

	while( it != eit )
	{
		const CMessage* msg = *it;
		nCount += msg->OutputCount();
		nCount += msg->EstimateCount();

		it++;
	}

	return nCount;
}

/// <summary>
///   Updates the 'Active' status of message variables.</summary>
/// <param name="Value">
///   Indicates if the aliases are to be marked as 'Active' or not.</param>
/// <return>
///   The number of items marked.</return>
/// <created>
///   rjvs 2003-02-18 09:06:37 -06:00</created>
size_t CModbusMsgVector::UpdateActive( bool Value )
{
    iterator it = begin();
	iterator eit = end();

	while( it != eit )
	{
		CMessage* msg = *it;
		if( msg != NULL )
		{
		    msg->UpdateActive( Value );
		}

		it++;
	}

	return size();
}

/// <summary>
///   Returns the message at a specific index.</summary>
/// <return>
///   The Modbus message.</return>
/// <created>
///   rjvs 2002-02-04 15:42:33 (UTC-6:00)</created>
CMessage* CModbusMsgVector::GetMessage( int index )
{ 
	return at( index );
}

const CMessage* CModbusMsgVector::GetMessage( int index ) const
{
	return at( index );
}

//-------------------------------------------------------------
}