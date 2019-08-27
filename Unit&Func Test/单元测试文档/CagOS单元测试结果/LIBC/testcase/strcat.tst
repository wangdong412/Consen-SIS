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

-- Unit: strcat

-- Subprogram: strcat

-- Test Case: strcat1
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while (*(destination++) != 0) ==> FALSE
      (2) while ((*(destination++) = *(append++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 4>>
TEST.VALUE:strcat.strcat.destination:"abc"
TEST.VALUE:strcat.strcat.append:<<malloc 1>>
TEST.VALUE:strcat.strcat.append[0]:0x0
TEST.EXPECTED:strcat.strcat.return:"abc"
TEST.ATTRIBUTES:strcat.strcat.destination[0]:INPUT_BASE=16
TEST.END

-- Test Case: strcat2
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while (*(destination++) != 0) ==> FALSE
      (2) while ((*(destination++) = *(append++)) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 1>>
TEST.VALUE:strcat.strcat.destination[0]:0x0
TEST.VALUE:strcat.strcat.append:<<malloc 4>>
TEST.VALUE:strcat.strcat.append:"abc"
TEST.EXPECTED:strcat.strcat.return:"abc"
TEST.END

-- Test Case: strcat3
TEST.UNIT:strcat
TEST.SUBPROGRAM:strcat
TEST.NEW
TEST.NAME:strcat3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) while (*(destination++) != 0) ==> TRUE
      (2) while ((*(destination++) = *(append++)) != 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcat.strcat.destination:<<malloc 4>>
TEST.VALUE:strcat.strcat.destination:"abc"
TEST.VALUE:strcat.strcat.append:<<malloc 4>>
TEST.VALUE:strcat.strcat.append:"def"
TEST.EXPECTED:strcat.strcat.return:"abcdef"
TEST.END
