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

-- Unit: strncpy

-- Subprogram: strncpy

-- Test Case: strncpy
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n != (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 4>>
TEST.VALUE:strncpy.strncpy.s1:"abc"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 5>>
TEST.VALUE:strncpy.strncpy.s2:"ABCD"
TEST.VALUE:strncpy.strncpy.n:0
TEST.EXPECTED:strncpy.strncpy.return:"abc"
TEST.END

-- Test Case: strncpy2
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> FALSE
      (4) while (--n > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 2>>
TEST.VALUE:strncpy.strncpy.s1:"a"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 2>>
TEST.VALUE:strncpy.strncpy.s2:"A"
TEST.VALUE:strncpy.strncpy.n:1
TEST.EXPECTED:strncpy.strncpy.return:"A"
TEST.END

-- Test Case: strncpy3
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy3
TEST.BASIS_PATH:5 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> TRUE
      (3) if (--n == (0)) ==> FALSE
      (4) while (--n > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 6>>
TEST.VALUE:strncpy.strncpy.s1:"abcde"
TEST.VALUE:strncpy.strncpy.s2:<<malloc 6>>
TEST.VALUE:strncpy.strncpy.s2:"ABCDE"
TEST.VALUE:strncpy.strncpy.n:3
TEST.EXPECTED:strncpy.strncpy.return:"ABCde"
TEST.END

-- Test Case: strncpy4
TEST.UNIT:strncpy
TEST.SUBPROGRAM:strncpy
TEST.NEW
TEST.NAME:strncpy4
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while ((*(d++) = *(s2++)) != 0) ==> FALSE
      (4) while (--n > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:strncpy.strncpy.s1:<<malloc 1>>
TEST.VALUE:strncpy.strncpy.s2:<<malloc 1>>
TEST.VALUE:strncpy.strncpy.s2[0]:0x0
TEST.VALUE:strncpy.strncpy.n:1
TEST.EXPECTED:strncpy.strncpy.return:"\0"
TEST.END
