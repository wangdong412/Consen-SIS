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

-- Unit: strlen

-- Subprogram: strlen

-- Test Case: strlen1
TEST.UNIT:strlen
TEST.SUBPROGRAM:strlen
TEST.NEW
TEST.NAME:strlen1
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

-- Test Case: strlen2
TEST.UNIT:strlen
TEST.SUBPROGRAM:strlen
TEST.NEW
TEST.NAME:strlen2
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
