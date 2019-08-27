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
TEST.EXPECTED_USER_CODE:memcpy.memcpy.destination
{{ <<memcpy.memcpy.destination>> == ( "bed" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END
