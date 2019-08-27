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

-- Unit: strtok_r

-- Subprogram: strtok_r

-- Test Case: strtok_r.001
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r.001
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 6>>
TEST.VALUE_USER_CODE:strtok_r.strtok_r.string
<<strtok_r.strtok_r.string>> = ( "a,b,c" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok_r1
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 9>>
TEST.VALUE:strtok_r.strtok_r.string:<<null>>
TEST.VALUE:strtok_r.strtok_r.separators:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<null>>
TEST.EXPECTED:strtok_r.strtok_r.return:<<null>>
TEST.END

-- Test Case: strtok_r2
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> FALSE
      (2) if (*(string += strspn(string, separators)) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 7>>
TEST.VALUE:strtok_r.strtok_r.string:"a","b","\0"
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<malloc 1>>
TEST.EXPECTED:strtok_r.strtok_r.ppLast[0]:<<null>>
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: strtok_r3
TEST.UNIT:strtok_r
TEST.SUBPROGRAM:strtok_r
TEST.NEW
TEST.NAME:strtok_r3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (string == (void *)0 && (string = *ppLast) == (void *)0) ==> FALSE
      (2) if (*(string += strspn(string, separators)) == 0) ==> FALSE
      (3) if ((*ppLast = strpbrk(string, separators)) != (void *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:strpbrk.strpbrk
TEST.VALUE:strpbrk.strpbrk.return:<<malloc 9>>
TEST.VALUE:strpbrk.strpbrk.return:<<null>>
TEST.VALUE:strtok_r.strtok_r.string:<<malloc 6>>
TEST.VALUE:strtok_r.strtok_r.string:"a","b","c"
TEST.VALUE:strtok_r.strtok_r.separators:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast:<<malloc 1>>
TEST.VALUE:strtok_r.strtok_r.ppLast[0]:<<malloc 1>>
TEST.EXPECTED:strtok_r.strtok_r.return:"a","b","c"
TEST.VALUE_USER_CODE:strtok_r.strtok_r.separators
<<strtok_r.strtok_r.separators>> = ( "," );
TEST.END_VALUE_USER_CODE:
TEST.END
