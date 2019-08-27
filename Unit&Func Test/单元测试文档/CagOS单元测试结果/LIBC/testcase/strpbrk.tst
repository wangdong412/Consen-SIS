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

-- Unit: strpbrk

-- Subprogram: strpbrk

-- Test Case: strpbrk1
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((c = *(s1++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 2>>
TEST.VALUE:strpbrk.strpbrk.s2:"f"
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 1>>
TEST.EXPECTED:strpbrk.strpbrk.return:<<null>>
TEST.END

-- Test Case: strpbrk2
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk2
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 2>>
TEST.VALUE:strpbrk.strpbrk.s2:"d"
TEST.EXPECTED:strpbrk.strpbrk.return:"de"
TEST.END

-- Test Case: strpbrk3
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk3
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 5>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcd"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 5>>
TEST.VALUE:strpbrk.strpbrk.s2:"abcd"
TEST.EXPECTED:strpbrk.strpbrk.return:"abcd"
TEST.END

-- Test Case: strpbrk4
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 7>>
TEST.VALUE:strpbrk.strpbrk.s1:"bbbbba"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 8>>
TEST.VALUE:strpbrk.strpbrk.s2:"efsdfds"
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 1>>
TEST.EXPECTED:strpbrk.strpbrk.return:<<null>>
TEST.END

-- Test Case: strpbrk5
TEST.UNIT:strpbrk
TEST.SUBPROGRAM:strpbrk
TEST.NEW
TEST.NAME:strpbrk5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while ((c = *(s1++)) != 0) ==> TRUE
      (2) for ((sc = *(scanp++)) != 0) ==> TRUE
      (3) if (sc == c) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable sc in branch 3
      Cannot set local variable c in branch 3
TEST.END_NOTES:
TEST.VALUE:strpbrk.strpbrk.s1:<<malloc 6>>
TEST.VALUE:strpbrk.strpbrk.s1:"abcde"
TEST.VALUE:strpbrk.strpbrk.s2:<<malloc 3>>
TEST.VALUE:strpbrk.strpbrk.s2:"fe"
TEST.EXPECTED:strpbrk.strpbrk.return:"e"
TEST.END
