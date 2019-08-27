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

-- Unit: strtok

-- Subprogram: strtok

-- Test Case: strtok
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtok.strtok.separator:<<malloc 1>>
TEST.VALUE:strtok.strtok.return:<<malloc 3>>
TEST.VALUE_USER_CODE:strtok.strtok.string
<<strtok.strtok.string>> = ( "a,b,c" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:strtok.strtok.separator.separator.separator[0]
<<strtok.strtok.separator>>[0] = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok.001
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok.001
TEST.VALUE:strtok.strtok.string:<<malloc 9>>
TEST.VALUE:strtok.strtok.string:<<null>>
TEST.VALUE:strtok.strtok.separator:<<malloc 2>>
TEST.VALUE:strtok.strtok.separator:"1"
TEST.EXPECTED:strtok.strtok.return:<<null>>
TEST.END

-- Test Case: strtok1
TEST.UNIT:strtok
TEST.SUBPROGRAM:strtok
TEST.NEW
TEST.NAME:strtok1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:strtok.strtok.string:<<malloc 6>>
TEST.VALUE:strtok.strtok.string:"a b c"
TEST.VALUE:strtok.strtok.separator:<<malloc 1>>
TEST.VALUE:strtok.strtok.return:<<malloc 3>>
TEST.VALUE_USER_CODE:strtok.strtok.separator.separator.separator[0]
<<strtok.strtok.separator>>[0] = ( " " );
TEST.END_VALUE_USER_CODE:
TEST.END
