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
TEST.EXPECTED_USER_CODE:memchr.memchr.return
{{ <<memchr.memchr.return>> == ( NULL ) }}
TEST.END_EXPECTED_USER_CODE:
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
