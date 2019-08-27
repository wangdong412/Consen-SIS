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

-- Unit: abort1

-- Subprogram: abort1

-- Test Case: abort1
TEST.UNIT:abort1
TEST.SUBPROGRAM:abort1
TEST.NEW
TEST.NAME:abort1
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while 1 ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 1
TEST.END_NOTES:
TEST.END

-- Unit: abs

-- Subprogram: abs

-- Test Case: abs.001
TEST.UNIT:abs
TEST.SUBPROGRAM:abs
TEST.NEW
TEST.NAME:abs.001
TEST.VALUE:abs.abs.j:0
TEST.EXPECTED:abs.abs.return:0
TEST.END

-- Test Case: abs2
TEST.UNIT:abs
TEST.SUBPROGRAM:abs
TEST.NEW
TEST.NAME:abs2
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (j < 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:abs.abs.j:<<MAX>>
TEST.EXPECTED:abs.abs.return:<<MAX>>
TEST.END

-- Test Case: abs3
TEST.UNIT:abs
TEST.SUBPROGRAM:abs
TEST.NEW
TEST.NAME:abs3
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (j < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:abs.abs.j:<<MIN>>
TEST.EXPECTED:abs.abs.return:<<MIN>>
TEST.END

-- Unit: atof

-- Subprogram: atof

-- Test Case: atof
TEST.UNIT:atof
TEST.SUBPROGRAM:atof
TEST.NEW
TEST.NAME:atof
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:atof.atof.ascii:<<malloc 1>>
TEST.EXPECTED:atof.atof.return:0.0
TEST.END

-- Unit: atoi

-- Subprogram: atoi

-- Test Case: atoi
TEST.UNIT:atoi
TEST.SUBPROGRAM:atoi
TEST.NEW
TEST.NAME:atoi
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:strtol.strtol
TEST.VALUE:atoi.atoi.str:<<malloc 1>>
TEST.VALUE:atoi.atoi.str[0]:0xB
TEST.VALUE:strtol.strtol.return:11
TEST.EXPECTED:atoi.atoi.return:11
TEST.END

-- Unit: atol

-- Subprogram: atol

-- Test Case: atol1
TEST.UNIT:atol
TEST.SUBPROGRAM:atol
TEST.NEW
TEST.NAME:atol1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:atol.atol.str:<<malloc 1>>
TEST.EXPECTED:atol.atol.return:0
TEST.END

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

-- Test Case: bcmp2
TEST.UNIT:bLib
TEST.SUBPROGRAM:bcmp
TEST.NEW
TEST.NAME:bcmp2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (nbytes == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:bLib.bcmp.buf1:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.buf2:VECTORCAST_INT1
TEST.VALUE:bLib.bcmp.nbytes:1
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
TEST.VALUE:bLib.bcopy.nbytes:3
TEST.VALUE_USER_CODE:bLib.bcopy.source
<<bLib.bcopy.source>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:bLib.bcopy.destination
<<bLib.bcopy.destination>> = ( "abc" );
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
TEST.VALUE:bLib.binvert.buf:VECTORCAST_INT1
TEST.VALUE:bLib.binvert.nbytes:1
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
TEST.VALUE:bLib.binvert.buf:VECTORCAST_INT1
TEST.VALUE:bLib.binvert.nbytes:5
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
TEST.VALUE:bLib.bzero.nbytes:<<MIN>>
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

-- Test Case: index3
TEST.UNIT:bLib
TEST.SUBPROGRAM:index
TEST.NEW
TEST.NAME:index3
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((ch = *(s++)) != c && ch != 0) ==> FALSE
      (2) if (ch == 0 && c != 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable ch in branch 1
      Cannot set local variable ch in branch 2
TEST.END_NOTES:
TEST.VALUE:bLib.index.s:<<malloc 1>>
TEST.VALUE:bLib.index.s[0]:0x0
TEST.VALUE:bLib.index.c:0
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
TEST.EXPECTED_USER_CODE:bLib.swab.source
{{ <<bLib.swab.source>> == ( "cd" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.EXPECTED_USER_CODE:bLib.swab.destination
{{ <<bLib.swab.destination>> == ( "ab" ) }}
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
TEST.END

-- Unit: memchr

-- Subprogram: memchr

-- Test Case: memchr.001
TEST.UNIT:memchr
TEST.SUBPROGRAM:memchr
TEST.NEW
TEST.NAME:memchr.001
TEST.VALUE:memchr.memchr.n:3
TEST.VALUE_USER_CODE:memchr.memchr.m
<<memchr.memchr.m>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memchr.memchr.c
<<memchr.memchr.c>> = ( 'a' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memchr.002
TEST.UNIT:memchr
TEST.SUBPROGRAM:memchr
TEST.NEW
TEST.NAME:memchr.002
TEST.VALUE:memchr.memchr.n:3
TEST.VALUE_USER_CODE:memchr.memchr.m
<<memchr.memchr.m>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memchr.memchr.c
<<memchr.memchr.c>> = ( 'e' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memchr1
TEST.UNIT:memchr
TEST.SUBPROGRAM:memchr
TEST.NEW
TEST.NAME:memchr1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n != (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:memchr.memchr.m:VECTORCAST_INT1
TEST.VALUE:memchr.memchr.c:<<MIN>>
TEST.VALUE:memchr.memchr.n:0
TEST.VALUE:memchr.memchr.return:VECTORCAST_INT1
TEST.EXPECTED_USER_CODE:memchr.memchr.return
{{ <<memchr.memchr.return>> == ( NULL ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Unit: memcmp

-- Subprogram: memcmp

-- Test Case: memcmp1
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:memcmp.memcmp.s1:VECTORCAST_INT1
TEST.VALUE:memcmp.memcmp.s2:VECTORCAST_INT1
TEST.VALUE:memcmp.memcmp.n:0
TEST.EXPECTED:memcmp.memcmp.return:0
TEST.END

-- Test Case: memcmp2
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:memcmp.memcmp.n:3
TEST.EXPECTED:memcmp.memcmp.return:-1
TEST.VALUE_USER_CODE:memcmp.memcmp.s1
<<memcmp.memcmp.s1>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memcmp.memcmp.s2
<<memcmp.memcmp.s2>> = ( "acd" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memcmp2.001
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:memcmp.memcmp.n:3
TEST.EXPECTED:memcmp.memcmp.return:1
TEST.VALUE_USER_CODE:memcmp.memcmp.s1
<<memcmp.memcmp.s1>> = ( "acc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memcmp.memcmp.s2
<<memcmp.memcmp.s2>> = ( "acb" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memcmp3
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> TRUE
      (3) if (--n == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:memcmp.memcmp.n:3
TEST.EXPECTED:memcmp.memcmp.return:0
TEST.VALUE_USER_CODE:memcmp.memcmp.s1
<<memcmp.memcmp.s1>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memcmp.memcmp.s2
<<memcmp.memcmp.s2>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memcmp4
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (n == (0)) ==> FALSE
      (2) while (*(p1++) == *(p2++)) ==> TRUE
      (3) if (--n == (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable *(p1++) in branch 2 since it requires user code.
      Cannot set const variable       Cannot set variable *(p2++) in branch 2 since it requires user code.
:
      Cannot set variable p2++ in branch 2 since it requires user code.
      Cannot set variable p1++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:memcmp.memcmp.n:3
TEST.EXPECTED:memcmp.memcmp.return:0
TEST.VALUE_USER_CODE:memcmp.memcmp.s1
<<memcmp.memcmp.s1>> = ( "abc1234" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memcmp.memcmp.s2
<<memcmp.memcmp.s2>> = ( "abc" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: memcpy

-- Subprogram: memcpy

-- Test Case: memcpy
TEST.UNIT:memcpy
TEST.SUBPROGRAM:memcpy
TEST.NEW
TEST.NAME:memcpy
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memcpy.memcpy.destination:VECTORCAST_BUFFER
TEST.VALUE:memcpy.memcpy.size:10
TEST.VALUE_USER_CODE:memcpy.memcpy.source
<<memcpy.memcpy.source>> = ( "bed" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: memmove

-- Subprogram: memmove

-- Test Case: memmove
TEST.UNIT:memmove
TEST.SUBPROGRAM:memmove
TEST.NEW
TEST.NAME:memmove
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memmove.memmove.destination:VECTORCAST_INT1
TEST.VALUE:memmove.memmove.source:VECTORCAST_INT1
TEST.VALUE:memmove.memmove.size:<<MIN>>
TEST.END

-- Unit: memset

-- Subprogram: memset

-- Test Case: BASIS-PATH-001
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:BASIS-PATH-001
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memset.memset.c:0
TEST.VALUE:memset.memset.size:3
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "aaa" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: BASIS-PATH-001.001
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:BASIS-PATH-001.001
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memset.memset.c:0
TEST.VALUE:memset.memset.size:3000
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memset.001
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:memset.001
TEST.VALUE:memset.memset.c:92
TEST.VALUE:memset.memset.size:3529
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "ad");
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: ns16550

-- Subprogram: ns16550DevInit

-- Test Case: Devinit
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550DevInit
TEST.NEW
TEST.NAME:Devinit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:ns16550.<<GLOBAL>>.siodev:<<malloc 1>>
TEST.END

-- Subprogram: ns16550InputChar

-- Test Case: inputchar1
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550InputChar
TEST.NEW
TEST.NAME:inputchar1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((readb(&(siodev->lsr)) & 0x1) == 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.VALUE:uut_prototype_stubs.readb.return:1
TEST.VALUE:ns16550.<<GLOBAL>>.siodev:<<malloc 1>>
TEST.EXPECTED:ns16550.ns16550InputChar.return:\1
TEST.END

-- Test Case: inputchar2
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550InputChar
TEST.NEW
TEST.NAME:inputchar2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((readb(&(siodev->lsr)) & 0x1) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.VALUE:uut_prototype_stubs.readb.return:0
TEST.EXPECTED:ns16550.ns16550InputChar.return:\0
TEST.END

-- Subprogram: ns16550OutputChar

-- Test Case: outputchar1
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550OutputChar
TEST.NEW
TEST.NAME:outputchar1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((readb(&(siodev->lsr)) & 0x20) == 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:uut_prototype_stubs.readb.return:32
TEST.STUB_EXP_USER_CODE:uut_prototype_stubs.writeb.val
{{ <<uut_prototype_stubs.writeb.val>> == ( 'A' ) }}
TEST.END_STUB_EXP_USER_CODE:
TEST.VALUE_USER_CODE:ns16550.ns16550OutputChar.c
<<ns16550.ns16550OutputChar.c>> = ( 'A' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: outputchar2
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550OutputChar
TEST.NEW
TEST.NAME:outputchar2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((readb(&(siodev->lsr)) & 0x20) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:uut_prototype_stubs.readb.return:0
TEST.STUB_EXP_USER_CODE:uut_prototype_stubs.writeb.val
{{ <<uut_prototype_stubs.writeb.val>> == ( 'B' ) }}
TEST.END_STUB_EXP_USER_CODE:
TEST.VALUE_USER_CODE:ns16550.ns16550OutputChar.c
<<ns16550.ns16550OutputChar.c>> = ( 'B' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: ns16550SioInit

-- Test Case: IOinit
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550SioInit
TEST.NEW
TEST.NAME:IOinit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:ns16550.ns16550DevInit
TEST.END

-- Unit: qsort

-- Subprogram: med3

-- Test Case: med31
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med31
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"a"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"b"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"c"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med32
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med32
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"b"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"a"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"c"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med33
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med33
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"a"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"c"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"b"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med34
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med34
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"c"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"a"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"b"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med35
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"c"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"b"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"a"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med36
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med36
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"b"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"c"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"a"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: qsort

-- Test Case: qsort1
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort1
TEST.BASIS_PATH:1 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:4
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "zzzzzxxxxxcccccaaaaasssssdddddeeeeeqqqqqwwwwwaaaaabbbbb" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort10
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:43
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:43
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort11.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort11.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:4
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort2
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort2
TEST.VALUE:qsort.qsort.n:6
TEST.VALUE:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "abced" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort3
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort3
TEST.BASIS_PATH:2 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> TRUE
      (5) for (pl > (char *)a && cmp(pl - es, pl) > 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:3
TEST.VALUE:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "aecbzyxvub" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort4
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort4
TEST.BASIS_PATH:4 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> TRUE
      (5) for (pl > (char *)a && cmp(pl - es, pl) > 0) ==> TRUE
      (6) if (swaptype == 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
      Cannot set swaptype due to assignment
      Cannot set local variable pl in branch 7
      Cannot set local variable pl in branch 7
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.a:<<null>>
TEST.VALUE:qsort.qsort.n:<<MIN>>
TEST.VALUE:qsort.qsort.es:<<MIN>>
TEST.END

-- Test Case: qsort5
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort5
TEST.BASIS_PATH:28 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 28
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> TRUE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.a:VECTORCAST_INT1
TEST.VALUE:qsort.qsort.n:<<MIN>>
TEST.VALUE:qsort.qsort.es:4
TEST.END

-- Test Case: qsort6
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort6
TEST.VALUE:qsort.qsort.n:3
TEST.VALUE:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[3]={"a","b","c"};
char *p=a+1;


<<qsort.qsort.a>> = ( p);
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort7
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:44
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:44
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort8
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort9
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:20
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:20
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "abcdefghijklmn" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: swapfunc

-- Test Case: swapfunc1
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:3
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"bbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaa"
TEST.END

-- Test Case: swapfunc1.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:0
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"bbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaa"
TEST.END

-- Test Case: swapfunc11
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:1
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"baa"
TEST.EXPECTED:qsort.swapfunc.b:"abb"
TEST.END

-- Test Case: swapfunc2
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc2
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (swaptype <= 1) ==> TRUE
      (2) while (--i > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.a:"aaaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.b:"bbbb"
TEST.VALUE:qsort.swapfunc.n:12
TEST.VALUE:qsort.swapfunc.swaptype:1
TEST.EXPECTED:qsort.swapfunc.a:"bbbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaaa"
TEST.END

-- Test Case: swapfunc2.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (swaptype <= 1) ==> TRUE
      (2) while (--i > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.a:"aaaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.b:"bbbb"
TEST.VALUE:qsort.swapfunc.n:1
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"baaa"
TEST.EXPECTED:qsort.swapfunc.b:"abbb"
TEST.END

-- Unit: rand

-- Subprogram: rand

-- Test Case: rand
TEST.UNIT:rand
TEST.SUBPROGRAM:rand
TEST.NEW
TEST.NAME:rand
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.END

-- Unit: random

-- Subprogram: __random

-- Test Case: random
TEST.UNIT:random
TEST.SUBPROGRAM:__random
TEST.NEW
TEST.NAME:random
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.END

-- Unit: random_r

-- Subprogram: __random_r

-- Test Case: __random_r.001
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:__random_r.001
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.return:0
TEST.VALUE_USER_CODE:random_r.__random_r.buf
int a[1000]; 

struct random_data *buf1,buf11;
buf1=&buf11;
buf1->fptr=&a[50];
buf1->end_ptr=&a[10];
buf1->rptr=&a[70];
buf1->rand_type=1;


<<random_r.__random_r.buf>> = ( buf1 );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: __random_r.002
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:__random_r.002
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.return:0
TEST.VALUE_USER_CODE:random_r.__random_r.buf
int a2[1000]; 

struct random_data *buf2,buf3;
buf2=&buf3;
buf2->fptr=&a2[10];
buf2->end_ptr=&a2[50];
buf2->rptr=&a2[100];
buf2->rand_type=1;


<<random_r.__random_r.buf>> = ( buf2 );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: random_r1
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (buf == (0) || result == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<null>>
TEST.VALUE:random_r.__random_r.result:<<null>>
TEST.EXPECTED:random_r.__random_r.return:-1
TEST.END

-- Test Case: random_r2
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (buf == (0) || result == (0)) ==> FALSE
      (2) if (buf->rand_type == 0) ==> FALSE
      (3) if (fptr >= end_ptr) ==> FALSE
      (4) if (rptr >= end_ptr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].rptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state[0]:0
TEST.VALUE:random_r.__random_r.buf[0].rand_type:0
TEST.VALUE:random_r.__random_r.buf[0].end_ptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.buf[0].state[0]:12345
TEST.EXPECTED:random_r.__random_r.return:0
TEST.END

-- Test Case: random_r2.001
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (buf == (0) || result == (0)) ==> FALSE
      (2) if (buf->rand_type == 0) ==> FALSE
      (3) if (fptr >= end_ptr) ==> FALSE
      (4) if (rptr >= end_ptr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr[0]:13
TEST.VALUE:random_r.__random_r.buf[0].rptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].rptr[0]:13
TEST.VALUE:random_r.__random_r.buf[0].state:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state[0]:0
TEST.VALUE:random_r.__random_r.buf[0].rand_type:1
TEST.VALUE:random_r.__random_r.buf[0].end_ptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].end_ptr[0]:0
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.buf[0].state[0]:0
TEST.EXPECTED:random_r.__random_r.buf[0].rand_type:1
TEST.EXPECTED:random_r.__random_r.buf[0].end_ptr[0]:0
TEST.EXPECTED:random_r.__random_r.result[0]:0xD
TEST.EXPECTED:random_r.__random_r.return:0
TEST.END

-- Unit: strcat

-- Subprogram: strcat

-- Test Case: strcat1
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*(destination++) != 0) ==> FALSE
      (2) while ((*(destination++) = *(append++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 4>>
TEST.VALUE:strcat.strcat.destination:"abc"
TEST.VALUE:strcat.strcat.append:<<malloc 1>>
TEST.VALUE:strcat.strcat.append[0]:0x0
TEST.EXPECTED:strcat.strcat.return:"abc"
TEST.ATTRIBUTES:strcat.strcat.destination[0]:INPUT_BASE=16
TEST.END

-- Test Case: strcat2
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*(destination++) != 0) ==> FALSE
      (2) while ((*(destination++) = *(append++)) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 1>>
TEST.VALUE:strcat.strcat.destination[0]:0x0
TEST.VALUE:strcat.strcat.append:<<malloc 4>>
TEST.VALUE:strcat.strcat.append:"abc"
TEST.EXPECTED:strcat.strcat.return:"abc"
TEST.END

-- Test Case: strcat3
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*(destination++) != 0) ==> TRUE
      (2) while ((*(destination++) = *(append++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 4>>
TEST.VALUE:strcat.strcat.destination:"abc"
TEST.VALUE:strcat.strcat.append:<<malloc 4>>
TEST.VALUE:strcat.strcat.append:"def"
TEST.EXPECTED:strcat.strcat.return:"abcdef"
TEST.END

-- Unit: strchr

-- Subprogram: strchr

-- Test Case: STRCHR1
TEST.UNIT:strchr
TEST.SUBPROGRAM:strchr
TEST.NEW
TEST.NAME:STRCHR1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*r != (char)c) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strchr.strchr.s:<<malloc 1>>
TEST.VALUE:strchr.strchr.s[0]:0x0
TEST.VALUE:strchr.strchr.c:0
TEST.EXPECTED:strchr.strchr.return:"\0"
TEST.END

-- Test Case: strchr2
TEST.UNIT:strchr
TEST.SUBPROGRAM:strchr
TEST.NEW
TEST.NAME:strchr2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*r != (char)c) ==> TRUE
      (2) if (*(r++) == 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: Cannot resolve multiple comparisons ( strchr.strchr.s[0] ) in branches 1/2
TEST.END_NOTES:
TEST.VALUE:strchr.strchr.s:<<malloc 1>>
TEST.VALUE:strchr.strchr.s[0]:<<MIN>>
TEST.VALUE:strchr.strchr.c:-127
TEST.EXPECTED:strchr.strchr.return:<<null>>
TEST.END

-- Test Case: strchr3
TEST.UNIT:strchr
TEST.SUBPROGRAM:strchr
TEST.NEW
TEST.NAME:strchr3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*r != (char)c) ==> TRUE
      (2) if (*(r++) == 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strchr.strchr.s:<<malloc 4>>
TEST.VALUE:strchr.strchr.s:"abc"
TEST.EXPECTED:strchr.strchr.return:<<null>>
TEST.VALUE_USER_CODE:strchr.strchr.c
<<strchr.strchr.c>> = ( "b" );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: strcmp

-- Subprogram: strcmp

-- Test Case: strcmp1
TEST.UNIT:strcmp
TEST.SUBPROGRAM:strcmp
TEST.NEW
TEST.NAME:strcmp1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*(s1++) == *(s2++)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcmp.strcmp.s1:<<malloc 2>>
TEST.VALUE:strcmp.strcmp.s1:"a"
TEST.VALUE:strcmp.strcmp.s2:<<malloc 2>>
TEST.VALUE:strcmp.strcmp.s2:"b"
TEST.EXPECTED:strcmp.strcmp.return:-1
TEST.END

-- Test Case: strcmp2
TEST.UNIT:strcmp
TEST.SUBPROGRAM:strcmp
TEST.NEW
TEST.NAME:strcmp2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*(s1++) == *(s2++)) ==> TRUE
      (2) if (s1[-1] == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:strcmp.strcmp.s1: in branch 2
TEST.END_NOTES:
TEST.VALUE:strcmp.strcmp.s1:<<malloc 3>>
TEST.VALUE:strcmp.strcmp.s1:"ab"
TEST.VALUE:strcmp.strcmp.s2:<<malloc 2>>
TEST.VALUE:strcmp.strcmp.s2:"a"
TEST.EXPECTED:strcmp.strcmp.return:98
TEST.ATTRIBUTES:strcmp.strcmp.s1[1]:INPUT_BASE=16
TEST.END

-- Test Case: strcmp3
TEST.UNIT:strcmp
TEST.SUBPROGRAM:strcmp
TEST.NEW
TEST.NAME:strcmp3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*(s1++) == *(s2++)) ==> TRUE
      (2) if (s1[-1] == 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:strcmp.strcmp.s1: in branch 2
TEST.END_NOTES:
TEST.VALUE:strcmp.strcmp.s1:<<malloc 2>>
TEST.VALUE:strcmp.strcmp.s1:"a"
TEST.VALUE:strcmp.strcmp.s2:<<malloc 4>>
TEST.VALUE:strcmp.strcmp.s2:"a\0"
TEST.EXPECTED:strcmp.strcmp.return:0
TEST.END

-- Unit: strcpy

-- Subprogram: strcpy

-- Test Case: strcpy.001
TEST.UNIT:strcpy
TEST.SUBPROGRAM:strcpy
TEST.NEW
TEST.NAME:strcpy.001
TEST.VALUE:strcpy.strcpy.s1:<<malloc 2>>
TEST.VALUE:strcpy.strcpy.s1:"a"
TEST.VALUE:strcpy.strcpy.s2:<<malloc 7>>
TEST.VALUE:strcpy.strcpy.s2:"bbbbbb"
TEST.EXPECTED:strcpy.strcpy.return:"bbbbbb"
TEST.END

-- Test Case: strcpy2
TEST.UNIT:strcpy
TEST.SUBPROGRAM:strcpy
TEST.NEW
TEST.NAME:strcpy2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((*(s1++) = *(s2++)) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcpy.strcpy.s1:<<malloc 4>>
TEST.VALUE:strcpy.strcpy.s1:"aaa"
TEST.VALUE:strcpy.strcpy.s2:<<malloc 4>>
TEST.VALUE:strcpy.strcpy.s2:"bbb"
TEST.EXPECTED:strcpy.strcpy.return:"bbb"
TEST.END

-- Unit: strlcat

-- Subprogram: strlcat

-- Test Case: strlcat1
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 1>>
TEST.VALUE:strlcat.strlcat.dst[0]:0x0
TEST.VALUE:strlcat.strlcat.src:<<malloc 1>>
TEST.VALUE:strlcat.strlcat.siz:0
TEST.EXPECTED:strlcat.strlcat.return:0
TEST.END

-- Test Case: strlcat2
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 6>>
TEST.VALUE:strlcat.strlcat.dst:"hello"
TEST.VALUE:strlcat.strlcat.src:<<malloc 6>>
TEST.VALUE:strlcat.strlcat.src:"world"
TEST.VALUE:strlcat.strlcat.siz:5
TEST.EXPECTED:strlcat.strlcat.return:10
TEST.END

-- Test Case: strlcat3
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> TRUE
      (4) if (n != (1)) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 3>>
TEST.VALUE:strlcat.strlcat.dst:"\0"
TEST.VALUE:strlcat.strlcat.src:<<malloc 4>>
TEST.VALUE:strlcat.strlcat.src:"aaa"
TEST.VALUE:strlcat.strlcat.siz:3
TEST.EXPECTED:strlcat.strlcat.return:3
TEST.END

-- Test Case: strlcat4
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> TRUE
      (4) if (n != (1)) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 5>>
TEST.VALUE:strlcat.strlcat.dst:"aaaa"
TEST.VALUE:strlcat.strlcat.src:<<malloc 3>>
TEST.VALUE:strlcat.strlcat.src:"\0"
TEST.VALUE:strlcat.strlcat.siz:3
TEST.EXPECTED:strlcat.strlcat.return:3
TEST.END

-- Unit: strlcpy

-- Subprogram: strlcpy

-- Test Case: strlcpy2
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy2
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0) && --n != (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
      (5) if (siz != (0)) ==> FALSE
      (6) while (*(s++)) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable s++ in branch 6 since it requires user code.
      Cannot set variable *(s++) in branch 6 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 1>>
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 1>>
TEST.VALUE:strlcpy.strlcpy.siz:0
TEST.EXPECTED:strlcpy.strlcpy.return:0
TEST.END

-- Test Case: strlcpy3
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy3
TEST.BASIS_PATH:7 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (n != (0) && --n != (0)) ==> TRUE
      (2) if ((*(d++) = *(s++)) == 0) ==> FALSE
      (3) while (--n != (0)) ==> TRUE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 1
      Cannot set variable *(d++) = *(s++) in branch 2 since it requires user code.
      Cannot set variable s++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 10>>
TEST.VALUE:strlcpy.strlcpy.dst:"abcdefdsf"
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 3>>
TEST.VALUE:strlcpy.strlcpy.src:"w0"
TEST.VALUE:strlcpy.strlcpy.siz:7
TEST.EXPECTED:strlcpy.strlcpy.dst:"w0"
TEST.EXPECTED:strlcpy.strlcpy.return:2
TEST.END

-- Test Case: strlcpy4
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy4
TEST.BASIS_PATH:6 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (n != (0) && --n != (0)) ==> TRUE
      (2) if ((*(d++) = *(s++)) == 0) ==> FALSE
      (3) while (--n != (0)) ==> FALSE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 1
      Cannot set variable *(d++) = *(s++) in branch 2 since it requires user code.
      Cannot set variable s++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 4>>
TEST.VALUE:strlcpy.strlcpy.dst:"abc"
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 6>>
TEST.VALUE:strlcpy.strlcpy.src:"abcd5"
TEST.VALUE:strlcpy.strlcpy.siz:3
TEST.EXPECTED:strlcpy.strlcpy.dst:"ab"
TEST.EXPECTED:strlcpy.strlcpy.return:5
TEST.END

-- Unit: strlen

-- Subprogram: strlen

-- Test Case: BASIS-PATH-001
TEST.UNIT:strlen
TEST.SUBPROGRAM:strlen
TEST.NEW
TEST.NAME:BASIS-PATH-001
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*(s++) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strlen.strlen.s:<<malloc 1>>
TEST.VALUE:strlen.strlen.s[0]:0x0
TEST.EXPECTED:strlen.strlen.return:0
TEST.END

-- Test Case: BASIS-PATH-002
TEST.UNIT:strlen
TEST.SUBPROGRAM:strlen
TEST.NEW
TEST.NAME:BASIS-PATH-002
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*(s++) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strlen.strlen.s:<<malloc 4>>
TEST.VALUE:strlen.strlen.s:"aaa"
TEST.EXPECTED:strlen.strlen.return:3
TEST.END

-- Unit: strncat

-- Subprogram: strncat

-- Test Case: strncat1
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n != (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 6>>
TEST.VALUE:strncat.strncat.dst:"hello"
TEST.VALUE:strncat.strncat.src:<<malloc 6>>
TEST.VALUE:strncat.strncat.src:"world"
TEST.VALUE:strncat.strncat.n:0
TEST.EXPECTED:strncat.strncat.return:"hello"
TEST.END

-- Test Case: strncat2
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 6>>
TEST.VALUE:strncat.strncat.dst:"hello"
TEST.VALUE:strncat.strncat.src:<<malloc 6>>
TEST.VALUE:strncat.strncat.src:"world"
TEST.VALUE:strncat.strncat.n:1
TEST.EXPECTED:strncat.strncat.return:"hellow"
TEST.END

-- Test Case: strncat3
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Trying to set variable strncat.strncat.n 'equal to' and 'not equal to' same value in branches 1/4
      Cannot set local variable d in branch 5
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 12>>
TEST.VALUE:strncat.strncat.dst:"hello\0eeee"
TEST.VALUE:strncat.strncat.src:<<malloc 8>>
TEST.VALUE:strncat.strncat.src:"world\0"
TEST.VALUE:strncat.strncat.n:5
TEST.EXPECTED:strncat.strncat.dst:"helloworld"
TEST.EXPECTED:strncat.strncat.return:"helloworld"
TEST.END

-- Test Case: strncat4
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Trying to set variable strncat.strncat.n 'equal to' and 'not equal to' same value in branches 1/4
      Cannot set local variable d in branch 5
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 12>>
TEST.VALUE:strncat.strncat.dst:"hello\0eeee"
TEST.VALUE:strncat.strncat.src:<<malloc 8>>
TEST.VALUE:strncat.strncat.src:"world\0"
TEST.VALUE:strncat.strncat.n:8
TEST.EXPECTED:strncat.strncat.dst:"helloworld"
TEST.EXPECTED:strncat.strncat.return:"helloworld"
TEST.END

-- Unit: strncmp

-- Subprogram: strncmp

-- Test Case: strncmp1
TEST.UNIT:strncmp
TEST.SUBPROGRAM:strncmp
TEST.NEW
TEST.NAME:strncmp1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncmp.strncmp.s1:<<malloc 1>>
TEST.VALUE:strncmp.strncmp.s2:<<malloc 1>>
TEST.VALUE:strncmp.strncmp.n:0
TEST.EXPECTED:strncmp.strncmp.return:0
TEST.END

-- Test Case: strncmp2
TEST.UNIT:strncmp
TEST.SUBPROGRAM:strncmp
TEST.NEW
TEST.NAME:strncmp2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n == (0)) ==> FALSE
      (2) while (*(s1++) == *(s2++)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncmp.strncmp.s1:<<malloc 2>>
TEST.VALUE:strncmp.strncmp.s1:"a"
TEST.VALUE:strncmp.strncmp.s2:<<malloc 2>>
TEST.VALUE:strncmp.strncmp.s2:"b"
TEST.VALUE:strncmp.strncmp.n:1
TEST.EXPECTED:strncmp.strncmp.return:-1
TEST.END

-- Test Case: strncmp3
TEST.UNIT:strncmp
TEST.SUBPROGRAM:strncmp
TEST.NEW
TEST.NAME:strncmp3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n == (0)) ==> FALSE
      (2) while (*(s1++) == *(s2++)) ==> TRUE
      (3) if (s1[-1] == 0 || --n == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:strncmp.strncmp.s1: in branch 3
TEST.END_NOTES:
TEST.VALUE:strncmp.strncmp.s1:<<malloc 2>>
TEST.VALUE:strncmp.strncmp.s1[0]:<<MIN>>
TEST.VALUE:strncmp.strncmp.s1[1]:0x0
TEST.VALUE:strncmp.strncmp.s2:<<malloc 1>>
TEST.VALUE:strncmp.strncmp.s2[0]:<<MIN>>
TEST.VALUE:strncmp.strncmp.n:1
TEST.EXPECTED:strncmp.strncmp.return:0
TEST.END

-- Test Case: strncmp4
TEST.UNIT:strncmp
TEST.SUBPROGRAM:strncmp
TEST.NEW
TEST.NAME:strncmp4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (n == (0)) ==> FALSE
      (2) while (*(s1++) == *(s2++)) ==> TRUE
      (3) if (s1[-1] == 0 || --n == (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set const variable TEST.VALUE:strncmp.strncmp.s1: in branch 3
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:strncmp.strncmp.s1:<<malloc 2>>
TEST.VALUE:strncmp.strncmp.s1[0]:<<MIN>>
TEST.VALUE:strncmp.strncmp.s1[1]:<<MIN>>
TEST.VALUE:strncmp.strncmp.s2:<<malloc 1>>
TEST.VALUE:strncmp.strncmp.s2[0]:<<MIN>>
TEST.VALUE:strncmp.strncmp.n:1
TEST.EXPECTED:strncmp.strncmp.return:0
TEST.END

-- Unit: strncpy

-- Subprogram: strncpy

-- Test Case: strncpy
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n != (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 4>>
TEST.VALUE:strncpy.strncpy.s1:"abc"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 5>>
TEST.VALUE:strncpy.strncpy.s2:"ABCD"
TEST.VALUE:strncpy.strncpy.n:0
TEST.EXPECTED:strncpy.strncpy.return:"abc"
TEST.END

-- Test Case: strncpy2
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> FALSE
      (4) while (--n > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 2>>
TEST.VALUE:strncpy.strncpy.s1:"a"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 2>>
TEST.VALUE:strncpy.strncpy.s2:"A"
TEST.VALUE:strncpy.strncpy.n:1
TEST.EXPECTED:strncpy.strncpy.return:"A"
TEST.END

-- Test Case: strncpy3
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy3
TEST.BASIS_PATH:5 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> TRUE
      (3) if (--n == (0)) ==> FALSE
      (4) while (--n > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 6>>
TEST.VALUE:strncpy.strncpy.s1:"abcde"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 6>>
TEST.VALUE:strncpy.strncpy.s2:"ABCDE"
TEST.VALUE:strncpy.strncpy.n:3
TEST.EXPECTED:strncpy.strncpy.return:"ABCde"
TEST.END

-- Test Case: strncpy4
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy4
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> FALSE
      (4) while (--n > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 1>>
TEST.VALUE:strncpy.strncpy.s2:<<malloc 1>>
TEST.VALUE:strncpy.strncpy.s2[0]:0x0
TEST.VALUE:strncpy.strncpy.n:1
TEST.EXPECTED:strncpy.strncpy.return:"\0"
TEST.END

-- Unit: strpbrk

-- Subprogram: strpbrk

-- Test Case: strpbrk1
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((c = *(s1++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 2>>
TEST.VALUE:strpbrk.strpbrk.s2:"f"
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 1>>
TEST.EXPECTED:strpbrk.strpbrk.return:<<null>>
TEST.END

-- Test Case: strpbrk2
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk2
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 2>>
TEST.VALUE:strpbrk.strpbrk.s2:"d"
TEST.EXPECTED:strpbrk.strpbrk.return:"de"
TEST.END

-- Test Case: strpbrk3
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk3
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 5>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcd"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 5>>
TEST.VALUE:strpbrk.strpbrk.s2:"abcd"
TEST.EXPECTED:strpbrk.strpbrk.return:"abcd"
TEST.END

-- Test Case: strpbrk4
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 7>>
TEST.VALUE:strpbrk.strpbrk.s1:"bbbbba"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 8>>
TEST.VALUE:strpbrk.strpbrk.s2:"efsdfds"
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 1>>
TEST.EXPECTED:strpbrk.strpbrk.return:<<null>>
TEST.END

-- Test Case: strpbrk5
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 3>>
TEST.VALUE:strpbrk.strpbrk.s2:"fe"
TEST.EXPECTED:strpbrk.strpbrk.return:"e"
TEST.END

-- Unit: strspn

-- Subprogram: strspn

-- Test Case: strspn1
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for ((c1 = *(s++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 1>>
TEST.VALUE:strspn.strspn.s[0]:0x0
TEST.VALUE:strspn.strspn.sep:<<malloc 1>>
TEST.EXPECTED:strspn.strspn.return:0
TEST.END

-- Test Case: strspn2
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for ((c1 = *(s++)) != 0) ==> TRUE
      (2) for ((c2 = *(p++)) != c1) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable c2 = *(p++) in branch 2 since it requires user code.
      Cannot set local variable c1 in branch 2
      Cannot set variable p++ in branch 2 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 8>>
TEST.VALUE:strspn.strspn.s:"abcdeds"
TEST.VALUE:strspn.strspn.sep:<<malloc 3>>
TEST.VALUE:strspn.strspn.sep:"ab"
TEST.EXPECTED:strspn.strspn.return:2
TEST.END

-- Test Case: strspn3
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for ((c1 = *(s++)) != 0) ==> TRUE
      (2) for ((c2 = *(p++)) != c1) ==> TRUE
      (3) if (c2 == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable c2 = *(p++) in branch 2 since it requires user code.
      Cannot set local variable c1 in branch 2
      Cannot set variable p++ in branch 2 since it requires user code.
      Cannot set local variable c2 in branch 3
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 9>>
TEST.VALUE:strspn.strspn.s:"abceddsf"
TEST.VALUE:strspn.strspn.sep:<<malloc 4>>
TEST.VALUE:strspn.strspn.sep:"ace"
TEST.EXPECTED:strspn.strspn.return:1
TEST.END

-- Unit: strtod

-- Subprogram: is_real

-- Test Case: real1
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:<<MIN>>
TEST.EXPECTED:strtod.is_real.return:0
TEST.END

-- Test Case: real2
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real2
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:<<MAX>>
TEST.EXPECTED:strtod.is_real.return:0
TEST.END

-- Test Case: real3
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real3
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:0.0
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Test Case: real4
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real4
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:-112.345234
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Test Case: real5
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real5
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:112.345234
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Subprogram: strtod

-- Test Case: strtod
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod
TEST.BASIS_PATH:1 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod10
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod10
TEST.BASIS_PATH:10 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 9>>
TEST.VALUE:strtod.strtod.str:"-1234.56"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:-1234.56
TEST.END

-- Test Case: strtod11
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod11
TEST.BASIS_PATH:11 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (13) if negative ==> TRUE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 14>>
TEST.VALUE:strtod.strtod.str:"12345.67afdsf"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:12345.7
TEST.END

-- Test Case: strtod12
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod12
TEST.BASIS_PATH:12 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 12
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (12) while (*p >= 48 && *p <= 57) ==> TRUE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 8/12
      Cannot set local variable p in branch 13
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 13>>
TEST.VALUE:strtod.strtod.str:"-123.34abced"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.endptr[0]:"abced"
TEST.EXPECTED:strtod.strtod.return:-123.34
TEST.END

-- Test Case: strtod13
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod13
TEST.BASIS_PATH:13 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (10) case (*(++p)) ==> 45
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 10
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 11>>
TEST.VALUE:strtod.strtod.str:"-123.34e+4"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:-1233400.0
TEST.END

-- Test Case: strtod14
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod14
TEST.BASIS_PATH:14 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> TRUE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:0
TEST.VALUE:strtod.strtod.str:<<malloc 7>>
TEST.VALUE:strtod.strtod.str:"-10e-1"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:-1.0
TEST.END

-- Test Case: strtod15
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod15
TEST.BASIS_PATH:15 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 15
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> TRUE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set p due to assignment
      Cannot set local variable p in branch 6
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 8>>
TEST.VALUE:strtod.strtod.str:"1e+1025"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:1.7E308
TEST.END

-- Test Case: strtod16
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod16
TEST.BASIS_PATH:16 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> TRUE
      (6) while (*p >= 48 && *p <= 57) ==> TRUE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set local variable p in branch 5
      Cannot set local variable p in branch 6
      Cannot set p due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 5/6
      Cannot set local variable p in branch 7
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 10>>
TEST.VALUE:strtod.strtod.str:"1.5e-1022"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:1.7E308
TEST.END

-- Test Case: strtod2
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod2
TEST.BASIS_PATH:2 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod3
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod3
TEST.BASIS_PATH:3 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 2>>
TEST.VALUE:strtod.strtod.str:" "
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod4
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod4
TEST.BASIS_PATH:4 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod5
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod5
TEST.BASIS_PATH:5 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> TRUE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:0
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.END

-- Test Case: strtod6
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod6
TEST.BASIS_PATH:6 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod7
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod7
TEST.BASIS_PATH:7 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> TRUE
      (18) if (exponent < 0) ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod8
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod8
TEST.BASIS_PATH:8 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> TRUE
      (18) if (exponent < 0) ==> TRUE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set p10 due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod9
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod9
TEST.BASIS_PATH:9 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> TRUE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 14>>
TEST.VALUE:strtod.strtod.str:"   12345.6789"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:12345.7
TEST.END

-- Unit: strtok

-- Subprogram: strtok

-- Test Case: strtok
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtok.strtok.separator:<<malloc 1>>
TEST.VALUE:strtok.strtok.return:<<malloc 3>>
TEST.VALUE_USER_CODE:strtok.strtok.string
<<strtok.strtok.string>> = ( "a,b,c" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:strtok.strtok.separator.separator.separator[0]
<<strtok.strtok.separator>>[0] = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok.001
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok.001
TEST.VALUE:strtok.strtok.string:<<malloc 9>>
TEST.VALUE:strtok.strtok.string:<<null>>
TEST.VALUE:strtok.strtok.separator:<<malloc 2>>
TEST.VALUE:strtok.strtok.separator:"1"
TEST.EXPECTED:strtok.strtok.return:<<null>>
TEST.END

-- Test Case: strtok1
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtok.strtok.string:<<malloc 6>>
TEST.VALUE:strtok.strtok.string:"a b c"
TEST.VALUE:strtok.strtok.separator:<<malloc 1>>
TEST.VALUE:strtok.strtok.return:<<malloc 3>>
TEST.VALUE_USER_CODE:strtok.strtok.separator.separator.separator[0]
<<strtok.strtok.separator>>[0] = ( " " );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: strtok_r

-- Subprogram: strtok_r

-- Test Case: strtok_r.001
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r.001
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 6>>
TEST.VALUE_USER_CODE:strtok_r.strtok_r.string
<<strtok_r.strtok_r.string>> = ( "a,b,c" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok_r1
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 9>>
TEST.VALUE:strtok_r.strtok_r.string:<<null>>
TEST.VALUE:strtok_r.strtok_r.separators:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<null>>
TEST.EXPECTED:strtok_r.strtok_r.return:<<null>>
TEST.END

-- Test Case: strtok_r2
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> FALSE
      (2) if (*(string += strspn(string, separators)) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 7>>
TEST.VALUE:strtok_r.strtok_r.string:"a","b","\0"
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<malloc 1>>
TEST.EXPECTED:strtok_r.strtok_r.ppLast[0]:<<null>>
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok_r3
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> FALSE
      (2) if (*(string += strspn(string, separators)) == 0) ==> FALSE
      (3) if ((*ppLast = strpbrk(string, separators)) != (void *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:strpbrk.strpbrk
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 9>>
TEST.VALUE:strpbrk.strpbrk.return:<<null>>
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 6>>
TEST.VALUE:strtok_r.strtok_r.string:"a","b","c"
TEST.VALUE:strtok_r.strtok_r.separators:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<malloc 1>>
TEST.EXPECTED:strtok_r.strtok_r.return:"a","b","c"
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Unit: strtol

-- Subprogram: strtol

-- Test Case: strtol-0.002
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-0.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31da6c"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:3267180
TEST.END

-- Test Case: strtol-10
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10
TEST.BASIS_PATH:3 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 13>>
TEST.VALUE:strtol.strtol.nptr:"10379 cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:" cend$3"
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-10.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"10379e+3 cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:"e+3 cend$3"
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-10.003
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 12>>
TEST.VALUE:strtol.strtol.nptr:"10379cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.endptr[0]:"nd$3"
TEST.EXPECTED:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.return:17005006
TEST.END

-- Test Case: strtol-16
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 11>>
TEST.VALUE:strtol.strtol.nptr:"-1.24e+100"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:".24e+100"
TEST.EXPECTED:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:-1
TEST.END

-- Test Case: strtol-16.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-16.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31DA6c"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.return:3267180
TEST.END

-- Test Case: strtol-163
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-163
TEST.BASIS_PATH:4 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31da6c"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:13
TEST.EXPECTED:strtol.strtol.endptr[0]:"x31da6c"
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol-2
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2
TEST.BASIS_PATH:1 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 6>>
TEST.VALUE:strtol.strtol.nptr:"10379"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:2
TEST.END

-- Test Case: strtol-2.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 10>>
TEST.VALUE:strtol.strtol.nptr:"+10379e\0"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr[0]:"379e"
TEST.EXPECTED:strtol.strtol.return:2
TEST.END

-- Test Case: strtol-2.002
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 8>>
TEST.VALUE:strtol.strtol.nptr:"ee10379"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol-8.003
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-8.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 13>>
TEST.VALUE:strtol.strtol.nptr:"-10379cend$3"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:8
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:-543
TEST.END

-- Test Case: strtol-8.004
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-8.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 5>>
TEST.VALUE:strtol.strtol.nptr:"0723"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:467
TEST.END

-- Test Case: strtol-max
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-max
TEST.BASIS_PATH:7 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 15>>
TEST.VALUE:strtol.strtol.nptr:"21474836478888"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:2147483647
TEST.END

-- Test Case: strtol-space
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-space
TEST.BASIS_PATH:5 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> TRUE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 20>>
TEST.VALUE:strtol.strtol.nptr:"     10379c        "
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-space1
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-space1
TEST.BASIS_PATH:6 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> TRUE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 26>>
TEST.VALUE:strtol.strtol.nptr:"     10      379c        "
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:10
TEST.END

-- Test Case: strtol.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol.001
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"","#","$$^$#^","....."
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol_36
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol_36
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 2>>
TEST.VALUE:strtol.strtol.nptr:"g"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:36
TEST.EXPECTED:strtol.strtol.return:16
TEST.END

-- Test Case: strtol_min
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol_min
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"-21474836478888"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:-2147483648
TEST.END

-- Unit: strtoul

-- Subprogram: strtoul

-- Test Case: strtoul-0
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0
TEST.BASIS_PATH:16 of 16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 4>>
TEST.VALUE:strtoul.strtoul.nptr:"0xa"
TEST.VALUE:strtoul.strtoul.endptr:<<null>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.return:10
TEST.END

-- Test Case: strtoul-0.001
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 3>>
TEST.VALUE:strtoul.strtoul.nptr:"07"
TEST.VALUE:strtoul.strtoul.endptr:<<null>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.return:7
TEST.END

-- Test Case: strtoul-0.002
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 9>>
TEST.VALUE:strtoul.strtoul.nptr:"123eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.003
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 10>>
TEST.VALUE:strtoul.strtoul.nptr:"-123eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.004
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 14>>
TEST.VALUE:strtoul.strtoul.nptr:"+123    eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"    eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.005
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 14>>
TEST.VALUE:strtoul.strtoul.nptr:"2147483647999"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:4294967295
TEST.END

-- Test Case: strtoul-10.006
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.006
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 15>>
TEST.VALUE:strtoul.strtoul.nptr:"      21      "
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:21
TEST.END

-- Test Case: strtoul-10.007
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.007
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 16>>
TEST.VALUE:strtoul.strtoul.nptr:"","#","$$^$#^","....."
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:0
TEST.END

-- Test Case: strtoul-16.005
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-16.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 4>>
TEST.VALUE:strtoul.strtoul.nptr:"0XA"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:16
TEST.EXPECTED:strtoul.strtoul.return:10
TEST.END
