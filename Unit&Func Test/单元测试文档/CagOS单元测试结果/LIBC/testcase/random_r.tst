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

-- Unit: random_r

-- Subprogram: __random_r

-- Test Case: random_r1
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (buf == (0) || result == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<null>>
TEST.VALUE:random_r.__random_r.result:<<null>>
TEST.EXPECTED:random_r.__random_r.return:-1
TEST.END

-- Test Case: random_r2
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (buf == (0) || result == (0)) ==> FALSE
      (2) if (buf->rand_type == 0) ==> FALSE
      (3) if (fptr >= end_ptr) ==> FALSE
      (4) if (rptr >= end_ptr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].rptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state[0]:0
TEST.VALUE:random_r.__random_r.buf[0].rand_type:0
TEST.VALUE:random_r.__random_r.buf[0].end_ptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.buf[0].state[0]:12345
TEST.EXPECTED:random_r.__random_r.return:0
TEST.END

-- Test Case: random_r3
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (buf == (0) || result == (0)) ==> FALSE
      (2) if (buf->rand_type == 0) ==> FALSE
      (3) if (fptr >= end_ptr) ==> FALSE
      (4) if (rptr >= end_ptr) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:random_r.__random_r.buf:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].fptr[0]:13
TEST.VALUE:random_r.__random_r.buf[0].rptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].rptr[0]:13
TEST.VALUE:random_r.__random_r.buf[0].state:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].state[0]:0
TEST.VALUE:random_r.__random_r.buf[0].rand_type:1
TEST.VALUE:random_r.__random_r.buf[0].end_ptr:<<malloc 1>>
TEST.VALUE:random_r.__random_r.buf[0].end_ptr[0]:0
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.buf[0].state[0]:0
TEST.EXPECTED:random_r.__random_r.buf[0].rand_type:1
TEST.EXPECTED:random_r.__random_r.buf[0].end_ptr[0]:0
TEST.EXPECTED:random_r.__random_r.result[0]:0xD
TEST.EXPECTED:random_r.__random_r.return:0
TEST.END

-- Test Case: random_r4
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r4
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.return:0
TEST.VALUE_USER_CODE:random_r.__random_r.buf
int a[1000]; 

struct random_data *buf1,buf11;
buf1=&buf11;
buf1->fptr=&a[50];
buf1->end_ptr=&a[10];
buf1->rptr=&a[70];
buf1->rand_type=1;


<<random_r.__random_r.buf>> = ( buf1 );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: random_r5
TEST.UNIT:random_r
TEST.SUBPROGRAM:__random_r
TEST.NEW
TEST.NAME:random_r5
TEST.VALUE:random_r.__random_r.result:<<malloc 1>>
TEST.EXPECTED:random_r.__random_r.return:0
TEST.VALUE_USER_CODE:random_r.__random_r.buf
int a2[1000]; 

struct random_data *buf2,buf3;
buf2=&buf3;
buf2->fptr=&a2[10];
buf2->end_ptr=&a2[50];
buf2->rptr=&a2[100];
buf2->rand_type=1;


<<random_r.__random_r.buf>> = ( buf2 );
TEST.END_VALUE_USER_CODE:
TEST.END
