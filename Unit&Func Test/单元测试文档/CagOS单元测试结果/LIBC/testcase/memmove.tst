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
TEST.VALUE:memmove.memmove.size:<<MIN>>
TEST.VALUE_USER_CODE:memmove.memmove.destination
<<memmove.memmove.destination>> = ( "123" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memmove.memmove.source
<<memmove.memmove.source>> = ( "456" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:memmove.memmove.destination
{{ <<memmove.memmove.destination>> == ( "123" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: memmove.001
TEST.UNIT:memmove
TEST.SUBPROGRAM:memmove
TEST.NEW
TEST.NAME:memmove.001
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memmove.memmove.size:3
TEST.VALUE_USER_CODE:memmove.memmove.destination
<<memmove.memmove.destination>> = ( "123" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memmove.memmove.source
<<memmove.memmove.source>> = ( "456" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:memmove.memmove.destination
{{ <<memmove.memmove.destination>> == ( "456" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END

-- Test Case: memmove.002
TEST.UNIT:memmove
TEST.SUBPROGRAM:memmove
TEST.NEW
TEST.NAME:memmove.002
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:memmove.memmove.size:1
TEST.VALUE_USER_CODE:memmove.memmove.destination
<<memmove.memmove.destination>> = ( "123" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:memmove.memmove.source
<<memmove.memmove.source>> = ( "456" );
TEST.END_VALUE_USER_CODE:
TEST.EXPECTED_USER_CODE:memmove.memmove.destination
{{ <<memmove.memmove.destination>> == ( "423" ) }}
TEST.END_EXPECTED_USER_CODE:
TEST.END
