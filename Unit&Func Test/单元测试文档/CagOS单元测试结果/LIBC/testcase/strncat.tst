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

-- Unit: strncat

-- Subprogram: strncat

-- Test Case: strncat1
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (n != (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 6>>
TEST.VALUE:strncat.strncat.dst:"hello"
TEST.VALUE:strncat.strncat.src:<<malloc 6>>
TEST.VALUE:strncat.strncat.src:"world"
TEST.VALUE:strncat.strncat.n:0
TEST.EXPECTED:strncat.strncat.return:"hello"
TEST.END

-- Test Case: strncat2
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 6>>
TEST.VALUE:strncat.strncat.dst:"hello"
TEST.VALUE:strncat.strncat.src:<<malloc 6>>
TEST.VALUE:strncat.strncat.src:"world"
TEST.VALUE:strncat.strncat.n:1
TEST.EXPECTED:strncat.strncat.return:"hellow"
TEST.END

-- Test Case: strncat3
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Trying to set variable strncat.strncat.n 'equal to' and 'not equal to' same value in branches 1/4
      Cannot set local variable d in branch 5
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 12>>
TEST.VALUE:strncat.strncat.dst:"hello\0eeee"
TEST.VALUE:strncat.strncat.src:<<malloc 8>>
TEST.VALUE:strncat.strncat.src:"world\0"
TEST.VALUE:strncat.strncat.n:5
TEST.EXPECTED:strncat.strncat.dst:"helloworld"
TEST.EXPECTED:strncat.strncat.return:"helloworld"
TEST.END

-- Test Case: strncat4
TEST.UNIT:strncat
TEST.SUBPROGRAM:strncat
TEST.NEW
TEST.NAME:strncat4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (n != (0)) ==> TRUE
      (2) while (*(d++) != 0) ==> FALSE
      (3) while ((*(d++) = *(src++)) != 0 && --n > (0)) ==> FALSE
      (4) if (n == (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Trying to set variable strncat.strncat.n 'equal to' and 'not equal to' same value in branches 1/4
      Cannot set local variable d in branch 5
TEST.END_NOTES:
TEST.VALUE:strncat.strncat.dst:<<malloc 12>>
TEST.VALUE:strncat.strncat.dst:"hello\0eeee"
TEST.VALUE:strncat.strncat.src:<<malloc 8>>
TEST.VALUE:strncat.strncat.src:"world\0"
TEST.VALUE:strncat.strncat.n:8
TEST.EXPECTED:strncat.strncat.dst:"helloworld"
TEST.EXPECTED:strncat.strncat.return:"helloworld"
TEST.END
