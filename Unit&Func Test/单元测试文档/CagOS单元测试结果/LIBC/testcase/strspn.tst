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

-- Unit: strspn

-- Subprogram: strspn

-- Test Case: strspn1
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for ((c1 = *(s++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 1>>
TEST.VALUE:strspn.strspn.s[0]:0x0
TEST.VALUE:strspn.strspn.sep:<<malloc 1>>
TEST.EXPECTED:strspn.strspn.return:0
TEST.END

-- Test Case: strspn2
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) for ((c1 = *(s++)) != 0) ==> TRUE
      (2) for ((c2 = *(p++)) != c1) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable c2 = *(p++) in branch 2 since it requires user code.
      Cannot set local variable c1 in branch 2
      Cannot set variable p++ in branch 2 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 8>>
TEST.VALUE:strspn.strspn.s:"abcdeds"
TEST.VALUE:strspn.strspn.sep:<<malloc 3>>
TEST.VALUE:strspn.strspn.sep:"ab"
TEST.EXPECTED:strspn.strspn.return:2
TEST.END

-- Test Case: strspn3
TEST.UNIT:strspn
TEST.SUBPROGRAM:strspn
TEST.NEW
TEST.NAME:strspn3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for ((c1 = *(s++)) != 0) ==> TRUE
      (2) for ((c2 = *(p++)) != c1) ==> TRUE
      (3) if (c2 == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable c2 = *(p++) in branch 2 since it requires user code.
      Cannot set local variable c1 in branch 2
      Cannot set variable p++ in branch 2 since it requires user code.
      Cannot set local variable c2 in branch 3
TEST.END_NOTES:
TEST.VALUE:strspn.strspn.s:<<malloc 9>>
TEST.VALUE:strspn.strspn.s:"abceddsf"
TEST.VALUE:strspn.strspn.sep:<<malloc 4>>
TEST.VALUE:strspn.strspn.sep:"ace"
TEST.EXPECTED:strspn.strspn.return:1
TEST.END
