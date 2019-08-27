-- VectorCAST 6.4c (02/03/16)
-- Test Case Script
-- 
-- Environment    : LIBC
-- Unit(s) Under Test: abort1 abs atof atoi atol bLib memchr memcmp memcpy memmove memset ns16550 qsort rand random random_r strcat strchr strcmp strcpy strlcat strlcpy strlen strncat strncmp strncpy strpbrk strspn strtod strtok strtok_r strtol strtoul
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Unit: bLib

-- Subprogram: bcmp

-- Test Case: bcmp1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcmp
TEST.NEW
TEST.NAME:bcmp1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (nbytes == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.bcmp.buf1:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.buf2:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.nbytes:0
TEST.EXPECTED:bLib.bcmp.return:0
TEST.END

-- Test Case: bcmp3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcmp
TEST.NEW
TEST.NAME:bcmp3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (nbytes == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> TRUE
      (3) if (--nbytes == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--nbytes) in branch 3
TEST.END_NOTES:
TEST.VALUE:bLib.bcmp.buf1:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.buf2:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.nbytes:5
TEST.EXPECTED:bLib.bcmp.return:0
TEST.END

-- Test Case: bcmp4
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcmp
TEST.NEW
TEST.NAME:bcmp4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (nbytes == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> TRUE
      (3) if (--nbytes == (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--nbytes) in branch 3
TEST.END_NOTES:
TEST.VALUE:bLib.bcmp.nbytes:3
TEST.EXPECTED:bLib.bcmp.return:-3
TEST.VALUE_USER_CODE:bLib.bcmp.buf1
<<bLib.bcmp.buf1>> = ( "123" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcmp.buf2
char str[3]={0xc};
char *p2=str;
<<bLib.bcmp.buf2>> = ( "456" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: bcopy

-- Test Case: bcopy.001
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopy
TEST.NEW
TEST.NAME:bcopy.001
TEST.VALUE:bLib.bcopy.destination:VECTORCAST_BUFFER
TEST.VALUE:bLib.bcopy.nbytes:3
TEST.VALUE_USER_CODE:bLib.bcopy.source
<<bLib.bcopy.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopy.destination
{{ <<bLib.bcopy.destination>> == ( "abc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopy2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopy
TEST.NEW
TEST.NAME:bcopy2
TEST.BASIS_PATH:2 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((size_t)(dst - src) >= nbytes) ==> FALSE
      (6) if (nbytes < (2) * sizeof(long) - (1) || ((long)dst ^ (long)src) & sizeof(long) - (1)) ==> TRUE
      (9) while (dst != dstend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable dst in branch 6
      Cannot set local variable src in branch 6
      Cannot set dst due to assignment
TEST.END_NOTES:
TEST.VALUE:bLib.bcopy.nbytes:0
TEST.VALUE_USER_CODE:bLib.bcopy.source
<<bLib.bcopy.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopy.destination
<<bLib.bcopy.destination>> = ( "cde" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopy.destination
{{ <<bLib.bcopy.destination>> == ( "cde" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopy3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopy
TEST.NEW
TEST.NAME:bcopy3
TEST.BASIS_PATH:3 of 10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((size_t)(dst - src) >= nbytes) ==> FALSE
      (6) if (nbytes < (2) * sizeof(long) - (1) || ((long)dst ^ (long)src) & sizeof(long) - (1)) ==> FALSE
      (7) while ((long)dst & sizeof(long) - (1)) ==> FALSE
      (8) while ((char *)dstl != dsttmp) ==> FALSE
      (9) while (dst != dstend) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable dst in branch 6
      Cannot set local variable src in branch 6
      Cannot set dst due to assignment
      Cannot set dst due to assignment
      Cannot set dsttmp due to assignment
      Cannot set dst due to assignment
TEST.END_NOTES:
TEST.VALUE:bLib.bcopy.nbytes:9
TEST.VALUE_USER_CODE:bLib.bcopy.source
char cc[]="abcd";

<<bLib.bcopy.source>> = ( cc );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopy.destination
long a[1024];
char * p=a;
p=p+5;
<<bLib.bcopy.destination>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopy.destination
{{ <<bLib.bcopy.destination>> == ( "abcd" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bcopyBytes

-- Test Case: bcopybyte3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyBytes
TEST.NEW
TEST.NAME:bcopybyte3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyBytes.source:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyBytes.destination:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyBytes.nbytes:<<MAX>>
TEST.END

-- Test Case: bcopybytes1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyBytes
TEST.NEW
TEST.NAME:bcopybytes1
TEST.BASIS_PATH:4 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> TRUE
      (3) while (dst != dstend) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set variable to unknown value in branch 3
      Cannot set dstend due to assignment
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyBytes.nbytes:<<MIN>>
TEST.EXPECTED:bLib.bcopyBytes.nbytes:0
TEST.VALUE_USER_CODE:bLib.bcopyBytes.source
<<bLib.bcopyBytes.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyBytes.destination
<<bLib.bcopyBytes.destination>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: bcopybytes2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyBytes
TEST.NEW
TEST.NAME:bcopybytes2
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (offset == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyBytes.nbytes:3
TEST.VALUE_USER_CODE:bLib.bcopyBytes.source
<<bLib.bcopyBytes.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyBytes.destination
<<bLib.bcopyBytes.destination>> = ( "qwe" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyBytes.destination
{{ <<bLib.bcopyBytes.destination>> == ( "abc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopybytes3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyBytes
TEST.NEW
TEST.NAME:bcopybytes3
TEST.VALUE:bLib.bcopyBytes.nbytes:10
TEST.VALUE_USER_CODE:bLib.bcopyBytes.source
<<bLib.bcopyBytes.source>> = ( "ABC" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyBytes.destination
<<bLib.bcopyBytes.destination>> = ( "EFD" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyBytes.destination
{{ <<bLib.bcopyBytes.destination>> == ( "ABC" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bcopyLongs

-- Test Case: bcopylongs1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyLongs
TEST.NEW
TEST.NAME:bcopylongs1
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable offset in branch 2
      Cannot set local variable nbytes in branch 2
      Cannot set dst due to assignment
      Cannot set local variable dst in branch 4
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyLongs.source:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyLongs.destination:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyLongs.nlongs:<<MIN>>
TEST.END

-- Test Case: bcopylongs2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyLongs
TEST.NEW
TEST.NAME:bcopylongs2
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (offset == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyLongs.nlongs:3
TEST.VALUE_USER_CODE:bLib.bcopyLongs.source
<<bLib.bcopyLongs.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyLongs.destination
<<bLib.bcopyLongs.destination>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyLongs.destination
{{ <<bLib.bcopyLongs.destination>> == ( "abc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopylongs2.001
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyLongs
TEST.NEW
TEST.NAME:bcopylongs2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (offset == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyLongs.nlongs:1
TEST.VALUE_USER_CODE:bLib.bcopyLongs.source
<<bLib.bcopyLongs.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyLongs.destination
<<bLib.bcopyLongs.destination>> = ( "aaaaaaaaaaaaaaaaaaaaaaa" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyLongs.destination
{{ <<bLib.bcopyLongs.destination>> == ( "a" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bcopyQuads

-- Test Case: bcopyquads1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyQuads
TEST.NEW
TEST.NAME:bcopyquads1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (offset == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyQuads.source:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyQuads.destination:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyQuads.nquads:<<MIN>>
TEST.END

-- Test Case: bcopyquads2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyQuads
TEST.NEW
TEST.NAME:bcopyquads2
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable offset in branch 2
      Cannot set local variable nbytes in branch 2
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyQuads.nquads:3
TEST.VALUE_USER_CODE:bLib.bcopyQuads.source
<<bLib.bcopyQuads.source>> = ( "abcdefgh" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyQuads.destination
<<bLib.bcopyQuads.destination>> = ( "higklmn" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyQuads.destination
{{ <<bLib.bcopyQuads.destination>> == ( "abc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopyquads3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyQuads
TEST.NEW
TEST.NAME:bcopyquads3
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable offset in branch 2
      Cannot set local variable nbytes in branch 2
      Cannot set dst due to assignment
      Cannot set local variable dst in branch 4
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyQuads.nquads:1
TEST.VALUE_USER_CODE:bLib.bcopyQuads.source
<<bLib.bcopyQuads.source>> = ( "dtdhgdhgdghdh56745342545455768gda" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyQuads.destination
<<bLib.bcopyQuads.destination>> = ( "1dfdfgdfgdfgdgdgfd2" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyQuads.destination
{{ <<bLib.bcopyQuads.destination>> == ( "d" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bcopyWords

-- Test Case: bcopywords1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyWords
TEST.NEW
TEST.NAME:bcopywords1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (offset == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyWords.source:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyWords.destination:VECTORCAST_INT1
TEST.VALUE:bLib.bcopyWords.nwords:<<MIN>>
TEST.END

-- Test Case: bcopywords2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyWords
TEST.NEW
TEST.NAME:bcopywords2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable offset in branch 2
      Cannot set local variable nbytes in branch 2
      Cannot set dst due to assignment
      Cannot set local variable dst in branch 4
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyWords.nwords:3
TEST.VALUE_USER_CODE:bLib.bcopyWords.source
<<bLib.bcopyWords.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyWords.destination
<<bLib.bcopyWords.destination>> = ( "efg" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyWords.destination
{{ <<bLib.bcopyWords.destination>> == ( "abc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bcopyworlds3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcopyWords
TEST.NEW
TEST.NAME:bcopyworlds3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (offset == (0)) ==> FALSE
      (2) if (offset >= nbytes) ==> FALSE
      (4) while (dst != dstend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable offset in branch 1
      Cannot set local variable offset in branch 2
      Cannot set local variable nbytes in branch 2
      Cannot set local variable dst in branch 4
      Cannot set local variable src in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bcopyWords.nwords:1
TEST.VALUE_USER_CODE:bLib.bcopyWords.source
<<bLib.bcopyWords.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopyWords.destination
<<bLib.bcopyWords.destination>> = ( "aaaaaaaaaaaaaaaaaaaa" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bcopyWords.destination
{{ <<bLib.bcopyWords.destination>> == ( "a" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bfill

-- Test Case: bfill.001
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfill
TEST.NEW
TEST.NAME:bfill.001
TEST.VALUE:bLib.bfill.nbytes:<<MAX>>
TEST.VALUE:bLib.bfill.ch:92
TEST.VALUE_USER_CODE:bLib.bfill.buf
<<bLib.bfill.buf>> = ( "aa" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: bfill1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfill
TEST.NEW
TEST.NAME:bfill1
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (nbytes < (2) * sizeof(long) - (1)) ==> TRUE
      (4) while (bufc != bufend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable bufend in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bfill.nbytes:10
TEST.VALUE_USER_CODE:bLib.bfill.buf
long a[1024];
char * p= a;
p++;
<<bLib.bfill.buf>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bfill.ch
<<bLib.bfill.ch>> = ( "c" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bfill.buf
{{ <<bLib.bfill.buf>> == ( "cccccccccc" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bfill2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfill
TEST.NEW
TEST.NAME:bfill2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (nbytes < (2) * sizeof(long) - (1)) ==> TRUE
      (4) while (bufc != bufend) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable bufend in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bfill.nbytes:0
TEST.VALUE_USER_CODE:bLib.bfill.buf
<<bLib.bfill.buf>> = ( "a\0" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bfill.ch
<<bLib.bfill.ch>> = ( "c" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bfill.buf
{{ <<bLib.bfill.buf>> == ( "a\0" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bfill3
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfill
TEST.NEW
TEST.NAME:bfill3
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (nbytes < (2) * sizeof(long) - (1)) ==> TRUE
      (4) while (bufc != bufend) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable to unknown value in branch 4
      Cannot set local variable bufend in branch 4
TEST.END_NOTES:
TEST.VALUE:bLib.bfill.nbytes:5
TEST.VALUE_USER_CODE:bLib.bfill.buf
<<bLib.bfill.buf>> = ( "b" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bfill.ch
<<bLib.bfill.ch>> = ( "a" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bfill.buf
{{ <<bLib.bfill.buf>> == ("aaaaa" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bfillBytes

-- Test Case: bfillbytes
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfillBytes
TEST.NEW
TEST.NAME:bfillbytes
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (bufc != bufend) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable to unknown value in branch 1
      Cannot set local variable bufend in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bfillBytes.nbytes:2
TEST.VALUE_USER_CODE:bLib.bfillBytes.buf
<<bLib.bfillBytes.buf>> = ( "ee" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bfillBytes.ch
<<bLib.bfillBytes.ch>> = ( "a" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bfillBytes.buf
{{ <<bLib.bfillBytes.buf>> == ( "aa" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bfillbytes.001
TEST.UNIT:bLib
TEST.SUBPROGRAM:bfillBytes
TEST.NEW
TEST.NAME:bfillbytes.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (bufc != bufend) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable to unknown value in branch 1
      Cannot set local variable bufend in branch 1
TEST.END_NOTES:
TEST.VALUE:bLib.bfillBytes.nbytes:<<MAX>>
TEST.VALUE_USER_CODE:bLib.bfillBytes.buf
<<bLib.bfillBytes.buf>> = ( "ee" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bfillBytes.ch
<<bLib.bfillBytes.ch>> = ( "a" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bfillBytes.buf
{{ <<bLib.bfillBytes.buf>> == ( "eeaa" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: binvert

-- Test Case: binvert1
TEST.UNIT:bLib
TEST.SUBPROGRAM:binvert
TEST.NEW
TEST.NAME:binvert1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (bufc < buf_end) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.binvert.nbytes:5
TEST.VALUE_USER_CODE:bLib.binvert.buf
<<bLib.binvert.buf>> = ( "12345" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.binvert.buf
{{ <<bLib.binvert.buf>> == ( "54321" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: binvert2
TEST.UNIT:bLib
TEST.SUBPROGRAM:binvert
TEST.NEW
TEST.NAME:binvert2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (bufc < buf_end) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable buf_end in branch 2
      Cannot set local variable buf_end in branch 2
TEST.END_NOTES:
TEST.VALUE_USER_CODE:bLib.binvert.buf
<<bLib.binvert.buf>> = ( "12345" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.binvert.buf
{{ <<bLib.binvert.buf>> == ( "12345" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bswap

-- Test Case: bswap1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bswap
TEST.NEW
TEST.NAME:bswap1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (nbytes >= (1)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.bswap.nbytes:2
TEST.VALUE_USER_CODE:bLib.bswap.buf1
<<bLib.bswap.buf1>> = ( "ab" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bswap.buf2
<<bLib.bswap.buf2>> = ( "cd" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bswap.buf1
{{ <<bLib.bswap.buf1>> == ( "cd" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bswap.buf2
{{ <<bLib.bswap.buf2>> == ( "ab" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: bswap2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bswap
TEST.NEW
TEST.NAME:bswap2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (nbytes >= (1)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.bswap.nbytes:0
TEST.EXPECTED:bLib.bswap.nbytes:0
TEST.VALUE_USER_CODE:bLib.bswap.buf1
<<bLib.bswap.buf1>> = ( "123" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bswap.buf2
<<bLib.bswap.buf2>> = ( "456" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bswap.buf1
{{ <<bLib.bswap.buf1>> == ( "123") }}
TEST.END_EXPECTED_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.bswap.buf2
{{ <<bLib.bswap.buf2>> == ( "456" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: bzero

-- Test Case: bzero1
TEST.UNIT:bLib
TEST.SUBPROGRAM:bzero
TEST.NEW
TEST.NAME:bzero1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:bLib.bzero.buffer:VECTORCAST_INT1
TEST.VALUE:bLib.bzero.nbytes:1
TEST.END

-- Subprogram: index

-- Test Case: index1
TEST.UNIT:bLib
TEST.SUBPROGRAM:index
TEST.NEW
TEST.NAME:index1
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((ch = *(s++)) != c && ch != 0) ==> FALSE
      (2) if (ch == 0 && c != 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable ch in branch 1
      Cannot set local variable ch in branch 2
TEST.END_NOTES:
TEST.VALUE:bLib.index.s:<<malloc 4>>
TEST.VALUE:bLib.index.s:"abc"
TEST.EXPECTED:bLib.index.return:"c"
TEST.VALUE_USER_CODE:bLib.index.c
<<bLib.index.c>> = ( 'c' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: index2
TEST.UNIT:bLib
TEST.SUBPROGRAM:index
TEST.NEW
TEST.NAME:index2
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while ((ch = *(s++)) != c && ch != 0) ==> TRUE
      (2) if (ch == 0 && c != 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable ch in branch 1
      Cannot set local variable ch in branch 2
TEST.END_NOTES:
TEST.VALUE:bLib.index.s:<<malloc 4>>
TEST.VALUE:bLib.index.s:"abc"
TEST.EXPECTED:bLib.index.return:<<null>>
TEST.VALUE_USER_CODE:bLib.index.c
<<bLib.index.c>> = ( 'd' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: rindex2

-- Test Case: rindex21
TEST.UNIT:bLib
TEST.SUBPROGRAM:rindex2
TEST.NEW
TEST.NAME:rindex21
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:bLib.rindex2.s:<<malloc 4>>
TEST.VALUE:bLib.rindex2.s:"abc"
TEST.EXPECTED:bLib.rindex2.return:"abc"
TEST.VALUE_USER_CODE:bLib.rindex2.c
<<bLib.rindex2.c>> = ( 'a' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: rindex22
TEST.UNIT:bLib
TEST.SUBPROGRAM:rindex2
TEST.NEW
TEST.NAME:rindex22
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:bLib.rindex2.s:<<malloc 5>>
TEST.VALUE:bLib.rindex2.s:"abcd"
TEST.EXPECTED:bLib.rindex2.return:<<null>>
TEST.VALUE_USER_CODE:bLib.rindex2.c
<<bLib.rindex2.c>> = ( 'f' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: swab

-- Test Case: swab1
TEST.UNIT:bLib
TEST.SUBPROGRAM:swab
TEST.NEW
TEST.NAME:swab1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (dst < dst_end) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.swab.nbytes:2
TEST.EXPECTED:bLib.swab.nbytes:2
TEST.VALUE_USER_CODE:bLib.swab.source
<<bLib.swab.source>> = ( "ab" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.swab.destination
<<bLib.swab.destination>> = ( "cd" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.swab.destination
{{ <<bLib.swab.destination>> == ( "ba" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: swab2
TEST.UNIT:bLib
TEST.SUBPROGRAM:swab
TEST.NEW
TEST.NAME:swab2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for (dst < dst_end) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.swab.nbytes:0
TEST.EXPECTED:bLib.swab.nbytes:0
TEST.VALUE_USER_CODE:bLib.swab.source
<<bLib.swab.source>> = ( "ab" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.swab.destination
<<bLib.swab.destination>> = ( "cd" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.swab.destination
{{ <<bLib.swab.destination>> == ("cd") }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Subprogram: uswab

-- Test Case: uswab2
TEST.UNIT:bLib
TEST.SUBPROGRAM:uswab
TEST.NEW
TEST.NAME:uswab2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (dst < dst_end) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.uswab.nbytes:3
TEST.VALUE_USER_CODE:bLib.uswab.source
<<bLib.uswab.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.uswab.destination
<<bLib.uswab.destination>> = ( "efd" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.uswab.destination
{{ <<bLib.uswab.destination>> == ( "bac" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: uswab2.001
TEST.UNIT:bLib
TEST.SUBPROGRAM:uswab
TEST.NEW
TEST.NAME:uswab2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (dst < dst_end) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:bLib.uswab.nbytes:2
TEST.VALUE_USER_CODE:bLib.uswab.source
<<bLib.uswab.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.uswab.destination
<<bLib.uswab.destination>> = ( "efd" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.uswab.destination
{{ <<bLib.uswab.destination>> == ( "bac" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END
