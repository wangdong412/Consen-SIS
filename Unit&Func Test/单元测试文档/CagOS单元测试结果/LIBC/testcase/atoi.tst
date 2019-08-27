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

-- Unit: atoi

-- Subprogram: atoi

-- Test Case: atoi
TEST.UNIT:atoi
TEST.SUBPROGRAM:atoi
TEST.NEW
TEST.NAME:atoi
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:strtol.strtol
TEST.VALUE:atoi.atoi.str:<<malloc 1>>
TEST.VALUE:atoi.atoi.str[0]:0xB
TEST.VALUE:strtol.strtol.return:11
TEST.EXPECTED:atoi.atoi.return:11
TEST.END
