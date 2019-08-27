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

-- Unit: abs

-- Subprogram: abs

-- Test Case: abs1
TEST.UNIT:abs
TEST.SUBPROGRAM:abs
TEST.NEW
TEST.NAME:abs1
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
