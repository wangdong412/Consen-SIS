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

-- Unit: strtoul

-- Subprogram: strtoul

-- Test Case: strtoul-0
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0
TEST.BASIS_PATH:16 of 16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 4>>
TEST.VALUE:strtoul.strtoul.nptr:"0xa"
TEST.VALUE:strtoul.strtoul.endptr:<<null>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.return:10
TEST.END

-- Test Case: strtoul-0.001
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 3>>
TEST.VALUE:strtoul.strtoul.nptr:"07"
TEST.VALUE:strtoul.strtoul.endptr:<<null>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.return:7
TEST.END

-- Test Case: strtoul-0.002
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-0.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 9>>
TEST.VALUE:strtoul.strtoul.nptr:"123eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:0
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.003
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 10>>
TEST.VALUE:strtoul.strtoul.nptr:"-123eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.004
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 14>>
TEST.VALUE:strtoul.strtoul.nptr:"+123    eeeee"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.endptr[0]:"    eeeee"
TEST.EXPECTED:strtoul.strtoul.return:123
TEST.END

-- Test Case: strtoul-10.005
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 14>>
TEST.VALUE:strtoul.strtoul.nptr:"2147483647999"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:4294967295
TEST.END

-- Test Case: strtoul-10.006
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.006
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 15>>
TEST.VALUE:strtoul.strtoul.nptr:"      21      "
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:21
TEST.END

-- Test Case: strtoul-10.007
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-10.007
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 16>>
TEST.VALUE:strtoul.strtoul.nptr:"","#","$$^$#^","....."
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:10
TEST.EXPECTED:strtoul.strtoul.return:0
TEST.END

-- Test Case: strtoul-16.005
TEST.UNIT:strtoul
TEST.SUBPROGRAM:strtoul
TEST.NEW
TEST.NAME:strtoul-16.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> TRUE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) for  ==> FALSE
      (14) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtoul.strtoul.nptr:<<malloc 4>>
TEST.VALUE:strtoul.strtoul.nptr:"0XA"
TEST.VALUE:strtoul.strtoul.endptr:<<malloc 1>>
TEST.VALUE:strtoul.strtoul.base:16
TEST.EXPECTED:strtoul.strtoul.return:10
TEST.END
