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
