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

-- Test Case: memcmp5
TEST.UNIT:memcmp
TEST.SUBPROGRAM:memcmp
TEST.NEW
TEST.NAME:memcmp5
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
