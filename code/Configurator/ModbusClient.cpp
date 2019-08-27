// -----------------------------------------------------------------------
// File         : ModbusClient.cpp
// Author/Date  : rjvs 2002-01-21 20:51:57 (UTC-6:00)
// Description  : Modbus client builds, decodes, and validates incoming
//              : and outgoing messages.  It also updates the aliases
//              : with the received values from the message.
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Cfg320/ModbusClient.cpp 26    3/04/03 2:08p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2010-02-23  Wand Dong         BCB5 to Visual Studio 2010
// 2002-06-04  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#include "StdAfx.h"
#include "ModbusClient.h"

// Added by wangdong
#include "SerialDriver.h"
#include "SocketDriver.h"
#include "ModbusClient.h"
#include "ModbusMessage.h"
#include "Modbus.h"
#include "crc16.h"
#include "Configurator.h"

namespace modbus
{
const int RtuResponseDefault_ModbusSerial = 8;
const int RtuResponseDefault_ModbusTcp = 12;

const int FuncCodeIndex_ModbusSerial = 1;
const int FuncCodeIndex_ModbusTcp = 7;


// --------------------------------------------------------------------------
// { modbus::Client }

/// <summary>
///   </summary>
/// <created>
///   rjvs 2002-01-23 17:06:09 (UTC-6:00)</created>
CClient::CClient()
        :m_nUnitId( 1 ),             // Slave ID
		 m_nMessageBytes( 0 )        // Added by wangdong: Expected message size
{
}

/// <summary>
///   </summary>
/// <created>
///   rjvs 2002-01-23 17:06:22 (UTC-6:00)</created>
CClient::~CClient()
{
}

/// <summary>
/// </summary>
/// <return>
/// </return>
/// <created>
///   rjvs 2002-01-28 09:30:40 (UTC-6:00)</created>
HRESULT
CClient::ReadMessage( IComDriver* pDriver, CMessage *pMessage, bool bModbusTcp )
{
    if( CheckWritePtr( pMessage, sizeof(CMessage)))
    {
        int nReceive, nPreReceive, nBufferSize;
        BYTE* it = pMessage->InputBegin();
        nPreReceive = pMessage->InputCount();           // bytes received
        nBufferSize = pMessage->m_vInputBuffer.size();
        if( nPreReceive < nBufferSize )
        {
            if( nPreReceive > 0 )
            {
                it += nPreReceive;
            }

            int nBytes = 0;
            if( m_nMessageBytes > 0 )                   // expected message size
            {
                nBytes = m_nMessageBytes - nPreReceive;
            }

            if( nBytes <= 0 )
            {
                nBytes = pMessage->EstimateCount();
            }

            if( nBytes <= 0 )
            {
                if( !bModbusTcp )    // ModbusSerial
                {
                    nBytes = RtuResponseDefault_ModbusSerial;
                }
                else                //  ModbusTcp
                {
                    nBytes = RtuResponseDefault_ModbusTcp;
                }
            }

            if((nPreReceive + nBytes) < nBufferSize)
            {
                nReceive = pDriver->read( it, nBytes );
            }
            else
            {
                nReceive = pDriver->read( it, nBufferSize - nPreReceive );
            }

            pMessage->m_nInputCount += nReceive;
        }// if( nOldReceive < nBufferSize )
        else 
        {
            nReceive = 0;
        }

        // Reset the message tick timer if we received some bytes.
        if( nReceive > 0 )
		{
            pMessage->BeginTick( ::GetTickCount());
		}

        // Check if the communication object had an error.
        int err = pDriver->errnum();
        if( err != ERROR_SUCCESS )
		{
            return HRESULT_FROM_WIN32(err);
		}

        HRESULT hr = ValidateMessage( pMessage, bModbusTcp );
        if( hr == S_OK )
        {
            hr = DecodeMessage( pMessage, bModbusTcp );
            return hr;
        }

        return hr;
    }//if( CheckWritePtr(pMessage, sizeof(CMessage)))

    return E_POINTER;
}

/// <summary>
/// </summary>
/// <return>
/// </return>
/// <created>
///   rjvs 2002-01-28 09:30:56 (UTC-6:00)</created>
HRESULT
CClient::WriteMessage( IComDriver* pDriver, CMessage *pMessage )
{
    m_nMessageBytes = 0;
    if( CheckWritePtr( pMessage, sizeof(CMessage)))
    {
        // Clear the input buffer.  Limits the possibility of a client timeout
        // causing problems.
        pMessage->ClearInput();

        const BYTE* it = pMessage->OutputBegin();
        int nBytes = pMessage->m_nOutputCount;         // bytes need to sent
        int nRetry = 0;
        int nSucWritten = 0;                           // bytes success written
        int err = S_OK;

        while((nBytes > 0) && (nRetry < 3))
        {
			if((dynamic_cast<CSerialDriver*>(pDriver)) && 
               (CConfiguratorApp::TS350 == theApp.GetProductType()))
			{
			    Sleep( 30 );
			}

            nSucWritten = pDriver->write( it, nBytes );

            err = pDriver->errnum();
            if((err != 0) && (!pDriver->is_open()))
			{
                break;
			}

            if( nSucWritten > 0 )
            {
                it += nSucWritten;
                nBytes -= nSucWritten;
            }

            ++nRetry;
        } //while

        // Return Win32 API error that FAILs with HRESULT
        if( err != S_OK )
		{
            return HRESULT_FROM_WIN32( err );
		}

        // Incomplete write
        if( nBytes > 0 )
		{
            return E_CLIENT_WRITE;
		}

        return S_OK;
    }//if( CheckWritePtr( pMessage, sizeof(CMessage)))

    return E_POINTER;
}

/// <summary>
/// </summary>
/// <param name="TODO">
/// </param>
/// <return>
/// </return>
/// <created>
///   rjvs 2002-01-30 15:42:55 (UTC-6:00)</created>
HRESULT
CClient::ValidateMessage( CMessage* msg, bool bModbusTcp )
{
	HRESULT hr = E_POINTER;

    if( CheckReadPtr( msg, sizeof(CMessage)))
    {
        int nRtuResponseHeader = 0;
        if( !bModbusTcp )     // Modbus Serial
        {
            nRtuResponseHeader = 3;
        }
        else                  // Modbus Tcp
        {
            nRtuResponseHeader = 9;   // 3 + 6
        }
    
        if( msg->InputCount() >= nRtuResponseHeader )
        {
            // Read input buffer and estimate size of response
            BYTE FunctionCode = 0;   
            BYTE DataBytes = 0;

            if( !bModbusTcp )                     // Modbus Serial
            {
                FunctionCode = msg->m_vInputBuffer[1];
                DataBytes = msg->m_vInputBuffer[2];
            }
            else                                  // Modbus Tcp
            {
                FunctionCode = msg->m_vInputBuffer[7];
                DataBytes = msg->m_vInputBuffer[8];
            }
      
            m_nMessageBytes = RtuResponseLength( FunctionCode, DataBytes, bModbusTcp );
            msg->EstimateCount( m_nMessageBytes );  // m_nEstimateCount
		}
        else
        {
            m_nMessageBytes = 0;
        }

        // Validate CRC if enough bytes have been received
        if((m_nMessageBytes > 0) && (msg->InputCount() >= m_nMessageBytes))
        {
            /// Verification
            //  Modbus Serial: CRC Verification
            //  Modbus Tcp:    Transaction identifier, Protocol identifier, Length
            int nFunctionBufIndex = -1;
            if( !bModbusTcp )       // Modbus Serial
            {
                WORD wCrc = 0;
                CrcModbusBlock( msg->InputBegin(), m_nMessageBytes - 2, &wCrc );

                WORD wMsg = MAKEWORD(
                                      msg->m_vInputBuffer[m_nMessageBytes - 1],     // CRC Low 8 bit
                                      msg->m_vInputBuffer[m_nMessageBytes - 2]      // CRC Heigh 8 bit
                                     );

                if( wCrc != wMsg )      // CRC Error
                {
                    hr = E_CLIENT_CRC;
                    msg->m_hr = hr;
                    return hr;
                }
                else     
                { 
                    nFunctionBufIndex = 1;
                }
            } // if( !bModbusTcp )
            else   //  Modbus Tcp
            {
                /// Transaction identifier: 2 Bytes
                // Local
                WORD wLocalTransaction = MAKEWORD( msg->m_vOutputBuffer[1],
                                                   msg->m_vOutputBuffer[0] );

                // Remote                             
                WORD wRemoteTransaction = MAKEWORD( msg->m_vInputBuffer[1],
                                                    msg->m_vInputBuffer[0] );

                /// Protocal identifier: 2 Bytes
                // Local
                WORD wLocalProtocal = MAKEWORD( msg->m_vOutputBuffer[3],
                                                msg->m_vOutputBuffer[2]);

                // Remote
                WORD wRemoteProtocal = MAKEWORD( msg->m_vInputBuffer[3],
                                                 msg->m_vInputBuffer[2]);

                if( wLocalTransaction != wRemoteTransaction )
                {
                    hr = E_EMT_TRANSACTION_ERROR;
                    msg->m_hr = hr;
                    return hr;
                }
                else  if( wLocalProtocal != wRemoteProtocal )
                {
                    hr = E_EMT_PROTOCAL_ERROR;
                    msg->m_hr = hr;
                    return hr;
                }
                else        // normal
                {
                    nFunctionBufIndex = 7;
                }
            } // else Modbus Tcp

            /// ASSERT  nFunctionBufIndex
            if( !bModbusTcp )       // Modbus Serial
            {
                ASSERT( nFunctionBufIndex == 1 );
            }
            else
            {
                ASSERT( nFunctionBufIndex == 7 );
            }

            if(( msg->m_vInputBuffer[nFunctionBufIndex] & 0x80) != 0)           // Abnormal happened
            {
                // Check if message is a modbus exception
                switch (msg->m_vInputBuffer[nFunctionBufIndex+1])
                {
                    case mbexIllegalFunction:
                      hr = E_MODBUS_FUNCTION;
                      break;

                    case mbexIllegalDataAddress:
                      hr = E_MODBUS_DATAADDRESS;
                      break;

                    case mbexIllegalDataValue:
                      hr = E_MODBUS_DATAVALUE;
                      break;

                    case mbexDeviceFailure:
                      hr = E_MODBUS_FAILURE;
                      break;

                    case mbexAcknowledge:
                      hr = E_MODBUS_ACKNOWLEDGE;
                      break;

                    case mbexBusyRejected:
                      hr = E_MODBUS_BUSYREJECTED;
                      break;

                    case mbexNegativeAcknowledge:
                      hr = E_MODBUS_NEGACKNOWLEDGE;
                      break;

                    case mbexMemoryParityError:
                      hr = E_MODBUS_MEMORYPARITY;
                      break;

                    case mbexErrorCrc:
                      hr = E_MODBUS_CRC;
                      break;

                    case mbexErrorTimeout:
                      hr = E_MODBUS_TIMEOUT;
                      break;

                    default:
                      hr = E_MODBUS_UNK_EXCEPTION;  // Unknown exception
                      break;
                }//switch
            }//if(( msg->m_vInputBuffer[nFunctionBufIndex] & 0x80) != 0)
            else                                                                // Abnormal has not happened
            {
                hr = S_OK;
            }
        }//if ((m_nMessageBytes > 0) && (msg->InputCount() >= m_nMessageBytes))
        else
        {
            // Not enough bytes have been received.
            hr = S_FALSE;
        }

        // Save the message result
        msg->m_hr = hr;
    }// if (CheckReadPtr(msg, sizeof(Message)))

    // Not enough bytes have been received.
    return hr;
}

/// <summary>
/// </summary>
/// <param name="Buffer">
/// </param>
/// <param name="Count">
/// </param>
/// <return>  msg->m_hr
/// </return>
/// <created>
///   rjvs 2002-01-30 16:44:49 (UTC-6:00)</created>
HRESULT CClient::DecodeMessage(CMessage* msg, bool bModbusTcp)
{
    if( CheckWritePtr(msg, sizeof(CMessage)))
    {
        if( msg->IsWrite())
        {
            return msg->m_hr;
        }

        CString strValue;

        // Get modbus function code
        BYTE FuncCode = 0;
        unsigned int uFuncIndex = 0;
        unsigned int uDataBytesOffset = 0;
        if( !bModbusTcp )
        {
            FuncCode = msg->m_vInputBuffer[FuncCodeIndex_ModbusSerial];
            uFuncIndex = FuncCodeIndex_ModbusSerial;
            uDataBytesOffset = 2;
        }
        else
        {
            FuncCode = msg->m_vInputBuffer[FuncCodeIndex_ModbusTcp];
            uFuncIndex = FuncCodeIndex_ModbusTcp;
            uDataBytesOffset = 8;
        }

        // Check for exception code
        if( FuncCode & 0x80 )
        {
            BYTE ex = msg->m_vInputBuffer[uFuncIndex+1];
            msg->m_hr = E_MODBUS_BASE + ex;
            return msg->m_hr;
        }

        // If messages are mismatched...
        if( FuncCode != msg->m_vOutputBuffer[uFuncIndex] )
        {
            msg->m_hr = E_CLIENT_MESSAGE;
            return msg->m_hr;
        }

        int alias = msg->BeginAlias();                          
        const BYTE*  p = msg->InputBegin() + uDataBytesOffset;  // counts
        BYTE DataBytes = *p++;                                  // first data byte                               

        // Check if the databyte count is valid
        if( DataBytes > msg->InputCount())
        {
            msg->m_hr = E_CLIENT_MESSAGE;
            return msg->m_hr;
        }

        switch( FuncCode )
        {
            case mbfnReadCoilStatus:
            case mbfnReadInputStatus:
            {
                for( int i = 0; i < DataBytes; ++i )
                {
                    BYTE CurrentByte = *p;
                    for( int j = 0; j < 8; ++j )
                    {
                        bool value = ((CurrentByte & (1 << j)) != 0) ? true : false;
                        LPCTSTR name = L"<NULL>";

                        CAliasItem* pAlias = msg->FindAlias( alias );
                        if( CheckWritePtr(pAlias, sizeof(CAliasItem)))
                        {
                            name = pAlias->GetName();
                            pAlias->remBool( value );           // Set m_avRemote, and m_bDirtyRemote
                            if( pAlias->IsReadOnly() || msg->GetCopyValue())
							{
                                pAlias->CopyRemoteToLocal();    // Copy, and reset m_bDirtyRemote
							}
                        }

                        ++alias;
                    } //for (int j = 0; j < 8; ++j)

                    ++p;   
                } //for (int i = 0; i < DataBytes; ++i)

                msg->m_hr = S_OK;
                break;
            }

			case mbfnReadHoldingRegisters:
            case mbfnReadInputRegisters:
            {
                HRESULT hr = S_OK;
                for( int i = 0; i < DataBytes; )
                {
                    LPCTSTR name = L"<NULL>";
                    CAliasItem* pAlias = msg->FindAlias( alias );
                    if( CheckWritePtr( pAlias, sizeof(CAliasItem)))
                    {
                        name = pAlias->GetName();
                        HRESULT hr2 = pAlias->SetDataDump( p );    // Set m_avRemote, and m_bDirtyRemote
                        if( hr2 == S_OK )
                        {
                            if( pAlias->IsReadOnly() || msg->GetCopyValue())
							{
                                pAlias->CopyRemoteToLocal();       // Copy, and reset m_bDirtyRemote
							}
                        }
                        else  
                        {
                            CString value;
                            VariantToString( pAlias->remVar(), value );

							CString str;
							str.Format( L"SetDataDump (Error %08x) : %s (%d) = %s", 
								        static_cast<int>(hr2), name, alias, value );
							theApp.LogInfo( str );

                            // Only keep track of first error
                            if( hr == S_OK )
							{
								hr = hr2;
							}
                        } 

                       // Move to next alias index
                       i += (2 * pAlias->GetAliasSize());
                       p += (2 * pAlias->GetAliasSize());
                       alias += pAlias->GetAliasSize();

                       // VariantToString( pAlias->remVar(), strValue );  // Update the debugging value
                   } //if (CheckWritePtr(pAlias, sizeof(CAliasItem)))
                   else 
                   {
                       //int value = MAKEWORD( p[1], p[0] );
                       //strValue = Format("0x%X", ARRAYOFCONST((value)));

                       i += 2;
                       p += 2;
                       ++alias;
                   }

                   /* Log the modbus transaction
                   if( GLogCheck(TLogMessage::lmDebug5))
                   { 
                       GLogDebug5("", Format( "Read alias: %d (%s) = %s",
                                               ARRAYOFCONST((alias, name, strValue))));
                   }*/

                   pAlias = NULL;
              }// for( int i = 0; i < DataBytes; )

              msg->m_hr = hr;
              break;
          } // case

          default:
            // Modbus function code does not need to be decoded.
            msg->m_hr = S_OK;
            break;
        } // switch

        return msg->m_hr;
    }

    return E_POINTER;
}

// --------------------------------------------------------------------------

} // namespace modbus
