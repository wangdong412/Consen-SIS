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
