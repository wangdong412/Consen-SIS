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

-- Unit: qsort

-- Subprogram: med3

-- Test Case: med31
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med31
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"a"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"b"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"c"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med32
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med32
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"b"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"a"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"c"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med33
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med33
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"a"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"c"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"b"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med34
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med34
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"c"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"a"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"b"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med35
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med35
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"c"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"b"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"a"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: med36
TEST.UNIT:qsort
TEST.SUBPROGRAM:med3
TEST.NEW
TEST.NAME:med36
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (cmp(a, b) < 0) ==> FALSE
      (4) if (cmp(b, c) > 0) ==> FALSE
      (5) if (cmp(a, c) < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
TEST.END_NOTES:
TEST.VALUE:qsort.med3.a:<<malloc 2>>
TEST.VALUE:qsort.med3.a:"b"
TEST.VALUE:qsort.med3.b:<<malloc 2>>
TEST.VALUE:qsort.med3.b:"c"
TEST.VALUE:qsort.med3.c:<<malloc 2>>
TEST.VALUE:qsort.med3.c:"a"
TEST.EXPECTED:qsort.med3.return:"b"
TEST.VALUE_USER_CODE:qsort.med3.cmp
<<qsort.med3.cmp>> = (strcmp);
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: qsort

-- Test Case: qsort1
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort1
TEST.BASIS_PATH:1 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:4
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "zzzzzxxxxxcccccaaaaasssssdddddeeeeeqqqqqwwwwwaaaaabbbbb" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort10
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:43
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:43
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort11.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort11.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:4
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort2
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort2
TEST.VALUE:qsort.qsort.n:6
TEST.VALUE:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "abced" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort3
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort3
TEST.BASIS_PATH:2 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> TRUE
      (5) for (pl > (char *)a && cmp(pl - es, pl) > 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:3
TEST.VALUE:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "aecbzyxvub" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort4
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort4
TEST.BASIS_PATH:4 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> TRUE
      (5) for (pl > (char *)a && cmp(pl - es, pl) > 0) ==> TRUE
      (6) if (swaptype == 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
      Conflict: Unable to validate expression-to-expression comparison in branch 5
      Cannot set swaptype due to assignment
      Cannot set local variable pl in branch 7
      Cannot set local variable pl in branch 7
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.a:<<null>>
TEST.VALUE:qsort.qsort.n:<<MIN>>
TEST.VALUE:qsort.qsort.es:<<MIN>>
TEST.END

-- Test Case: qsort5
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort5
TEST.BASIS_PATH:28 of 29
TEST.NOTES:
This is an automatically generated test case.
   Test Path 28
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> TRUE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.a:VECTORCAST_INT1
TEST.VALUE:qsort.qsort.n:<<MIN>>
TEST.VALUE:qsort.qsort.es:4
TEST.END

-- Test Case: qsort6
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort6
TEST.VALUE:qsort.qsort.n:3
TEST.VALUE:qsort.qsort.es:4
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[3]={"a","b","c"};
char *p=a+1;


<<qsort.qsort.a>> = ( p);
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort7
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:44
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:44
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
long a[100]={"z","s","z","s","z","s","a","a","a","f","f","f","a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s","z","s","z","s","z","s","a","a","a","f","f","f", "a","f","b","s","s","s","s","s"};
char *p=a;
<<qsort.qsort.a>> = ( p );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort8
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:41
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:41
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: qsort9
TEST.UNIT:qsort
TEST.SUBPROGRAM:qsort
TEST.NEW
TEST.NAME:qsort9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((char *)a - (char *)0) % sizeof(long) || es % sizeof(long)) ==> FALSE
      (2) if (es == sizeof(long)) ==> FALSE
      (3) if (n < (7)) ==> TRUE
      (4) for (pm < (char *)a + n * es) ==> FALSE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (((char *)a - (char *)0) % sizeof(long)) in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (es % sizeof(long)) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 4
TEST.END_NOTES:
TEST.VALUE:qsort.qsort.n:20
TEST.VALUE:qsort.qsort.es:1
TEST.EXPECTED:qsort.qsort.n:20
TEST.EXPECTED:qsort.qsort.es:1
TEST.VALUE_USER_CODE:qsort.qsort.a
<<qsort.qsort.a>> = ( "abcdefghijklmn" );
TEST.END_VALUE_USER_CODE:
TEST.VALUE_USER_CODE:qsort.qsort.cmp
<<qsort.qsort.cmp>> = ( strcmp );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: swapfunc

-- Test Case: swapfunc1
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:3
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"bbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaa"
TEST.END

-- Test Case: swapfunc1.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:0
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"bbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaa"
TEST.END

-- Test Case: swapfunc11
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (swaptype <= 1) ==> FALSE
      (3) while (--i > (0)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.a:"aaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 4>>
TEST.VALUE:qsort.swapfunc.b:"bbb"
TEST.VALUE:qsort.swapfunc.n:1
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"baa"
TEST.EXPECTED:qsort.swapfunc.b:"abb"
TEST.END

-- Test Case: swapfunc2
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc2
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (swaptype <= 1) ==> TRUE
      (2) while (--i > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.a:"aaaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.b:"bbbb"
TEST.VALUE:qsort.swapfunc.n:12
TEST.VALUE:qsort.swapfunc.swaptype:1
TEST.EXPECTED:qsort.swapfunc.a:"bbbb"
TEST.EXPECTED:qsort.swapfunc.b:"aaaa"
TEST.END

-- Test Case: swapfunc2.001
TEST.UNIT:qsort
TEST.SUBPROGRAM:swapfunc
TEST.NEW
TEST.NAME:swapfunc2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (swaptype <= 1) ==> TRUE
      (2) while (--i > (0)) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:qsort.swapfunc.a:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.a:"aaaa"
TEST.VALUE:qsort.swapfunc.b:<<malloc 5>>
TEST.VALUE:qsort.swapfunc.b:"bbbb"
TEST.VALUE:qsort.swapfunc.n:1
TEST.VALUE:qsort.swapfunc.swaptype:<<MAX>>
TEST.EXPECTED:qsort.swapfunc.a:"baaa"
TEST.EXPECTED:qsort.swapfunc.b:"abbb"
TEST.END
