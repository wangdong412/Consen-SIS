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

-- Unit: strlcat

-- Subprogram: strlcat

-- Test Case: strlcat1
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 1>>
TEST.VALUE:strlcat.strlcat.dst[0]:0x0
TEST.VALUE:strlcat.strlcat.src:<<malloc 1>>
TEST.VALUE:strlcat.strlcat.siz:0
TEST.EXPECTED:strlcat.strlcat.return:0
TEST.END

-- Test Case: strlcat2
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 6>>
TEST.VALUE:strlcat.strlcat.dst:"hello"
TEST.VALUE:strlcat.strlcat.src:<<malloc 6>>
TEST.VALUE:strlcat.strlcat.src:"world"
TEST.VALUE:strlcat.strlcat.siz:5
TEST.EXPECTED:strlcat.strlcat.return:10
TEST.END

-- Test Case: strlcat3
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> TRUE
      (4) if (n != (1)) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 3>>
TEST.VALUE:strlcat.strlcat.dst:"\0"
TEST.VALUE:strlcat.strlcat.src:<<malloc 4>>
TEST.VALUE:strlcat.strlcat.src:"aaa"
TEST.VALUE:strlcat.strlcat.siz:3
TEST.EXPECTED:strlcat.strlcat.return:3
TEST.END

-- Test Case: strlcat4
TEST.UNIT:strlcat
TEST.SUBPROGRAM:strlcat
TEST.NEW
TEST.NAME:strlcat4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (n-- != (0) && *d != 0) ==> FALSE
      (2) if (n == (0)) ==> FALSE
      (3) while (*s != 0) ==> TRUE
      (4) if (n != (1)) ==> FALSE
   Test Case Generation Notes:
      Cannot set n due to assignment
      Cannot set variable *s in branch 3 since it requires user code.
      Cannot set variable s in branch 3 since it requires user code.
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.VALUE:strlcat.strlcat.dst:<<malloc 5>>
TEST.VALUE:strlcat.strlcat.dst:"aaaa"
TEST.VALUE:strlcat.strlcat.src:<<malloc 3>>
TEST.VALUE:strlcat.strlcat.src:"\0"
TEST.VALUE:strlcat.strlcat.siz:3
TEST.EXPECTED:strlcat.strlcat.return:3
TEST.END
