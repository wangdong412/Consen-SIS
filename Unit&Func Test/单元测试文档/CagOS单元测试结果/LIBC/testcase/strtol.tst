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

-- Unit: strtol

-- Subprogram: strtol

-- Test Case: strtol-0.002
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-0.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31da6c"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:3267180
TEST.END

-- Test Case: strtol-10
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10
TEST.BASIS_PATH:3 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 13>>
TEST.VALUE:strtol.strtol.nptr:"10379 cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:" cend$3"
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-10.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"10379e+3 cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:"e+3 cend$3"
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-10.003
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-10.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 12>>
TEST.VALUE:strtol.strtol.nptr:"10379cend$3"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.endptr[0]:"nd$3"
TEST.EXPECTED:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.return:17005006
TEST.END

-- Test Case: strtol-16
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 11>>
TEST.VALUE:strtol.strtol.nptr:"-1.24e+100"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.endptr[0]:".24e+100"
TEST.EXPECTED:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:-1
TEST.END

-- Test Case: strtol-16.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-16.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> TRUE
      (20) if any ==> TRUE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31DA6c"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.base:16
TEST.EXPECTED:strtol.strtol.return:3267180
TEST.END

-- Test Case: strtol-163
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-163
TEST.BASIS_PATH:4 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 9>>
TEST.VALUE:strtol.strtol.nptr:"0x31da6c"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:13
TEST.EXPECTED:strtol.strtol.endptr[0]:"x31da6c"
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol-2
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2
TEST.BASIS_PATH:1 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 6>>
TEST.VALUE:strtol.strtol.nptr:"10379"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:2
TEST.END

-- Test Case: strtol-2.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 10>>
TEST.VALUE:strtol.strtol.nptr:"+10379e\0"
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr[0]:"379e"
TEST.EXPECTED:strtol.strtol.return:2
TEST.END

-- Test Case: strtol-2.002
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-2.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 8>>
TEST.VALUE:strtol.strtol.nptr:"ee10379"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:2
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol-8.003
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-8.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 13>>
TEST.VALUE:strtol.strtol.nptr:"-10379cend$3"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:8
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:-543
TEST.END

-- Test Case: strtol-8.004
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-8.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set neg due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 5>>
TEST.VALUE:strtol.strtol.nptr:"0723"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.endptr:<<null>>
TEST.EXPECTED:strtol.strtol.return:467
TEST.END

-- Test Case: strtol-max
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-max
TEST.BASIS_PATH:7 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 15>>
TEST.VALUE:strtol.strtol.nptr:"21474836478888"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:2147483647
TEST.END

-- Test Case: strtol-space
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-space
TEST.BASIS_PATH:5 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> TRUE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 20>>
TEST.VALUE:strtol.strtol.nptr:"     10379c        "
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:10379
TEST.END

-- Test Case: strtol-space1
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol-space1
TEST.BASIS_PATH:6 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> TRUE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 26>>
TEST.VALUE:strtol.strtol.nptr:"     10      379c        "
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:0
TEST.EXPECTED:strtol.strtol.return:10
TEST.END

-- Test Case: strtol.001
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol.001
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"","#","$$^$#^","....."
TEST.VALUE:strtol.strtol.endptr:<<malloc 1>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:0
TEST.END

-- Test Case: strtol_36
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol_36
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 2>>
TEST.VALUE:strtol.strtol.nptr:"g"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:36
TEST.EXPECTED:strtol.strtol.return:16
TEST.END

-- Test Case: strtol_min
TEST.UNIT:strtol
TEST.SUBPROGRAM:strtol
TEST.NEW
TEST.NAME:strtol_min
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((c == 32 || c == 9) || c == 10) || c == 13) || c == 12) || c == 11) ==> FALSE
      (2) if (c == 45) ==> FALSE
      (3) if (c == 43) ==> FALSE
      (4) if (((base == 0 || base == 16) && c == 48) && (*s == 120 || *s == 88)) ==> FALSE
      (5) if (base == 0) ==> FALSE
      (7) if neg ==> FALSE
      (10) for  ==> TRUE
      (11) if (c >= 48 && c <= 57) ==> FALSE
      (12) if (c >= 97 && c <= 122 || c >= 65 && c <= 90) ==> TRUE
      (13) if (c >= 65 && c <= 90) ==> FALSE
      (14) if (c >= base) ==> FALSE
      (15) if (any < 0) ==> FALSE
      (18) if (acc > cutoff || acc == cutoff && c > cutlim) ==> FALSE
      (19) if (endptr != (0)) ==> FALSE
   Test Case Generation Notes:
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable s++ in branch 4 since it requires user code.
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set variable *s in branch 4 since it requires user code.
      Cannot set variable s in branch 4 since it requires user code.
      Cannot set neg due to assignment
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set neg due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 7/12
      Cannot set variable (unsigned char)(*(s++)) in branch 1 since it requires user code.
      Cannot set any due to assignment
      Cannot set neg due to assignment
      Cannot set acc due to assignment
      Cannot set cutoff due to assignment
      Cannot set c due to assignment
      Cannot set cutlim due to assignment
TEST.END_NOTES:
TEST.VALUE:strtol.strtol.nptr:<<malloc 16>>
TEST.VALUE:strtol.strtol.nptr:"-21474836478888"
TEST.VALUE:strtol.strtol.endptr:<<null>>
TEST.VALUE:strtol.strtol.base:10
TEST.EXPECTED:strtol.strtol.return:-2147483648
TEST.END
