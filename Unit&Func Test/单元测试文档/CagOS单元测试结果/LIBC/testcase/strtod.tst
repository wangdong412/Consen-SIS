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

-- Unit: strtod

-- Subprogram: is_real

-- Test Case: real1
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:<<MIN>>
TEST.EXPECTED:strtod.is_real.return:0
TEST.END

-- Test Case: real2
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real2
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:<<MAX>>
TEST.EXPECTED:strtod.is_real.return:0
TEST.END

-- Test Case: real3
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real3
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:0.0
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Test Case: real4
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real4
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:-112.345234
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Test Case: real5
TEST.UNIT:strtod
TEST.SUBPROGRAM:is_real
TEST.NEW
TEST.NAME:real5
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtod.is_real.x:112.345234
TEST.EXPECTED:strtod.is_real.return:1
TEST.END

-- Subprogram: strtod

-- Test Case: strtod
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod
TEST.BASIS_PATH:1 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod10
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod10
TEST.BASIS_PATH:10 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 10
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 9>>
TEST.VALUE:strtod.strtod.str:"-1234.56"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:-1234.56
TEST.END

-- Test Case: strtod11
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod11
TEST.BASIS_PATH:11 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 11
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (13) if negative ==> TRUE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 14>>
TEST.VALUE:strtod.strtod.str:"12345.67afdsf"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:12345.7
TEST.END

-- Test Case: strtod12
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod12
TEST.BASIS_PATH:12 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 12
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (11) case (*(++p)) ==> 43
      (12) while (*p >= 48 && *p <= 57) ==> TRUE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 11
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 8/12
      Cannot set local variable p in branch 13
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 13>>
TEST.VALUE:strtod.strtod.str:"-123.34abced"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.endptr[0]:"abced"
TEST.EXPECTED:strtod.strtod.return:-123.34
TEST.END

-- Test Case: strtod13
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod13
TEST.BASIS_PATH:13 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 13
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> TRUE
      (10) case (*(++p)) ==> 45
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set switch condition (*(++p)) in branch 10
      Cannot set p due to assignment
      Cannot set local variable p in branch 12
      Cannot set negative due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 11>>
TEST.VALUE:strtod.strtod.str:"-123.34e+4"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:-1233400.0
TEST.END

-- Test Case: strtod14
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod14
TEST.BASIS_PATH:14 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 14
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> TRUE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:0
TEST.VALUE:strtod.strtod.str:<<malloc 7>>
TEST.VALUE:strtod.strtod.str:"-10e-1"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:-1.0
TEST.END

-- Test Case: strtod15
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod15
TEST.BASIS_PATH:15 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 15
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> TRUE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set p due to assignment
      Cannot set local variable p in branch 6
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 8>>
TEST.VALUE:strtod.strtod.str:"1e+1025"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:1.7E308
TEST.END

-- Test Case: strtod16
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod16
TEST.BASIS_PATH:16 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 16
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> TRUE
      (6) while (*p >= 48 && *p <= 57) ==> TRUE
      (7) if (num_digits == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set local variable p in branch 5
      Cannot set local variable p in branch 6
      Cannot set p due to assignment
      Conflict: Cannot resolve multiple comparisons (  ) in branches 5/6
      Cannot set local variable p in branch 7
      Cannot set num_digits due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 10>>
TEST.VALUE:strtod.strtod.str:"1.5e-1022"
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:1.7E308
TEST.END

-- Test Case: strtod2
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod2
TEST.BASIS_PATH:2 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> TRUE
   Test Case Generation Notes:
      Cannot set p due to assignment
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod3
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod3
TEST.BASIS_PATH:3 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 2>>
TEST.VALUE:strtod.strtod.str:" "
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod4
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod4
TEST.BASIS_PATH:4 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> TRUE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<malloc 1>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod5
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod5
TEST.BASIS_PATH:5 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> TRUE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:0
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.END

-- Test Case: strtod6
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod6
TEST.BASIS_PATH:6 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod7
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod7
TEST.BASIS_PATH:7 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> TRUE
      (18) if (exponent < 0) ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod8
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod8
TEST.BASIS_PATH:8 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 8
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> FALSE
      (16) while n ==> TRUE
      (17) if (n & 1) ==> TRUE
      (18) if (exponent < 0) ==> TRUE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set exponent due to assignment
      Cannot set p10 due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 1>>
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:0.0
TEST.END

-- Test Case: strtod9
TEST.UNIT:strtod
TEST.SUBPROGRAM:strtod
TEST.NEW
TEST.NAME:strtod9
TEST.BASIS_PATH:9 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 9
      (1) while (((((*p == 32 || *p == 9) || *p == 10) || *p == 13) || *p == 12) || *p == 11) ==> FALSE
      (3) case (*p) ==> 43
      (4) while (*p >= 48 && *p <= 57) ==> FALSE
      (5) if (*p == 46) ==> FALSE
      (7) if (num_digits == 0) ==> FALSE
      (8) if negative ==> FALSE
      (9) if (*p == 101 || *p == 69) ==> FALSE
      (14) if (exponent < -1021 || exponent > 1024) ==> FALSE
      (15) if (n < 0) ==> TRUE
      (16) while n ==> FALSE
      (19) if (!is_real(number)) ==> FALSE
      (20) if endptr ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable p in branch 4
      Cannot set p due to assignment
      Cannot set local variable p in branch 5
      Cannot set num_digits due to assignment
      Cannot set negative due to assignment
      Cannot set p due to assignment
      Cannot set local variable p in branch 9
      Cannot set exponent due to assignment
      Cannot set n due to assignment
TEST.END_NOTES:
TEST.STUB:strtod.is_real
TEST.VALUE:strtod.is_real.return:<<MIN>>
TEST.VALUE:strtod.strtod.str:<<malloc 14>>
TEST.VALUE:strtod.strtod.str:"   12345.6789"
TEST.VALUE:strtod.strtod.endptr:<<null>>
TEST.EXPECTED:strtod.strtod.return:12345.7
TEST.END
