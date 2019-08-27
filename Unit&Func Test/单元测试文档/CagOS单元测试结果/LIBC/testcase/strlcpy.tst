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

-- Unit: strlcpy

-- Subprogram: strlcpy

-- Test Case: strlcpy2
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy2
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0) && --n != (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
      (5) if (siz != (0)) ==> FALSE
      (6) while (*(s++)) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable s++ in branch 6 since it requires user code.
      Cannot set variable *(s++) in branch 6 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 1>>
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 1>>
TEST.VALUE:strlcpy.strlcpy.siz:0
TEST.EXPECTED:strlcpy.strlcpy.return:0
TEST.END

-- Test Case: strlcpy3
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy3
TEST.BASIS_PATH:7 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (n != (0) && --n != (0)) ==> TRUE
      (2) if ((*(d++) = *(s++)) == 0) ==> FALSE
      (3) while (--n != (0)) ==> TRUE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 1
      Cannot set variable *(d++) = *(s++) in branch 2 since it requires user code.
      Cannot set variable s++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 10>>
TEST.VALUE:strlcpy.strlcpy.dst:"abcdefdsf"
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 3>>
TEST.VALUE:strlcpy.strlcpy.src:"w0"
TEST.VALUE:strlcpy.strlcpy.siz:7
TEST.EXPECTED:strlcpy.strlcpy.dst:"w0"
TEST.EXPECTED:strlcpy.strlcpy.return:2
TEST.END

-- Test Case: strlcpy4
TEST.UNIT:strlcpy
TEST.SUBPROGRAM:strlcpy
TEST.NEW
TEST.NAME:strlcpy4
TEST.BASIS_PATH:6 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (n != (0) && --n != (0)) ==> TRUE
      (2) if ((*(d++) = *(s++)) == 0) ==> FALSE
      (3) while (--n != (0)) ==> FALSE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 1
      Cannot set variable *(d++) = *(s++) in branch 2 since it requires user code.
      Cannot set variable s++ in branch 2 since it requires user code.
      Conflict: A condition in this branch uses an operand that cannot be set (--n) in branch 3
TEST.END_NOTES:
TEST.VALUE:strlcpy.strlcpy.dst:<<malloc 4>>
TEST.VALUE:strlcpy.strlcpy.dst:"abc"
TEST.VALUE:strlcpy.strlcpy.src:<<malloc 6>>
TEST.VALUE:strlcpy.strlcpy.src:"abcd5"
TEST.VALUE:strlcpy.strlcpy.siz:3
TEST.EXPECTED:strlcpy.strlcpy.dst:"ab"
TEST.EXPECTED:strlcpy.strlcpy.return:5
TEST.END
