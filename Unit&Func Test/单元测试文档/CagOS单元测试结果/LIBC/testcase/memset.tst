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

-- Unit: memset

-- Subprogram: memset

-- Test Case: memset.001
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:memset.001
TEST.VALUE:memset.memset.c:97
TEST.VALUE:memset.memset.size:10000
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "ad");
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: memset1
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:memset1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memset.memset.c:0
TEST.VALUE:memset.memset.size:3
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "aaa" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:memset.memset.m
{{ <<memset.memset.m>> == ( "" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: memset1.001
TEST.UNIT:memset
TEST.SUBPROGRAM:memset
TEST.NEW
TEST.NAME:memset1.001
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memset.memset.c:98
TEST.VALUE:memset.memset.size:3
TEST.VALUE_USER_CODE:memset.memset.m
<<memset.memset.m>> = ( "aaa" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:memset.memset.m
{{ <<memset.memset.m>> == ( "bbb" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END
