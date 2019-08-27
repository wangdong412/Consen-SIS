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

-- Unit: strcpy

-- Subprogram: strcpy

-- Test Case: strcpy1
TEST.UNIT:strcpy
TEST.SUBPROGRAM:strcpy
TEST.NEW
TEST.NAME:strcpy1
TEST.VALUE:strcpy.strcpy.s1:<<malloc 2>>
TEST.VALUE:strcpy.strcpy.s1:"a"
TEST.VALUE:strcpy.strcpy.s2:<<malloc 7>>
TEST.VALUE:strcpy.strcpy.s2:"bbbbbb"
TEST.EXPECTED:strcpy.strcpy.return:"bbbbbb"
TEST.END

-- Test Case: strcpy2
TEST.UNIT:strcpy
TEST.SUBPROGRAM:strcpy
TEST.NEW
TEST.NAME:strcpy2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((*(s1++) = *(s2++)) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strcpy.strcpy.s1:<<malloc 4>>
TEST.VALUE:strcpy.strcpy.s1:"aaa"
TEST.VALUE:strcpy.strcpy.s2:<<malloc 4>>
TEST.VALUE:strcpy.strcpy.s2:"bbb"
TEST.EXPECTED:strcpy.strcpy.return:"bbb"
TEST.END
