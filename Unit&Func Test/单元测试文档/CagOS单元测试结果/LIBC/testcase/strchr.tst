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

-- Unit: strchr

-- Subprogram: strchr

-- Test Case: strchr1
TEST.UNIT:strchr
TEST.SUBPROGRAM:strchr
TEST.NEW
TEST.NAME:strchr1
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
