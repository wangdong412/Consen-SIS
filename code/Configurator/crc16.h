// -----------------------------------------------------------------------
// File         : crc16.h
// Description  : CRC-16 prototypes
// Author       : Ryan Van Slooten
// Purpose      :
// -----------------------------------------------------------------------
//
//                         COPYRIGHT NOTICE
//                         ----------------
// Copyright (C) 2011, Tri-Sen Corporation, Webster, Texas
// Unauthorized possession, sale, transmission or reproduction
// is prohibited by law.
// -----------------------------------------------------------------------
// $Header: /Software/Cfg310SV/crc16.h 1     2/15/02 5:14p Rvanslooten $
//                         REVISION HISTORY
//                         ----------------
//
//     Date         Who          Change
// ----------  ----------------  -----------------------------------------
// 2011-02-24  Wang Dong         BCB5 to Visual Studio 2010
// 2001-10-24  Ryan Van Slooten  Added single byte function
// 2001-05-08  Ryan Van Slooten  Added Comment Block
// -----------------------------------------------------------------------

#if !defined(CRC16_H__INCLUDED)
#define      CRC16_H__INCLUDED

// -----------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {
#endif

// -----------------------------------------------------------------------
// Function prototypes

HRESULT CrcModbusByte( BYTE byValue, WORD* pCrc16 );
HRESULT CrcModbusBlock( const BYTE* pbyBuffer, size_t nCount, WORD* pCrc16 );

// -----------------------------------------------------------------------

#if defined(__cplusplus)
} // extern "C"
#endif

// -----------------------------------------------------------------------


#endif // !defined(CRC16_H__INCLUDED)