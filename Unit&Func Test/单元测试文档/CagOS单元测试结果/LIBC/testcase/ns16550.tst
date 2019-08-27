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

-- Unit: ns16550

-- Subprogram: ns16550DevInit

-- Test Case: Devinit
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550DevInit
TEST.NEW
TEST.NAME:Devinit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:ns16550.<<GLOBAL>>.siodev:<<malloc 1>>
TEST.END

-- Subprogram: ns16550InputChar

-- Test Case: inputchar1
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550InputChar
TEST.NEW
TEST.NAME:inputchar1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((readb(&(siodev->lsr)) & 0x1) == 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.VALUE:uut_prototype_stubs.readb.return:1
TEST.VALUE:ns16550.<<GLOBAL>>.siodev:<<malloc 1>>
TEST.EXPECTED:ns16550.ns16550InputChar.return:\1
TEST.END

-- Test Case: inputchar2
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550InputChar
TEST.NEW
TEST.NAME:inputchar2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((readb(&(siodev->lsr)) & 0x1) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.VALUE:uut_prototype_stubs.readb.return:0
TEST.EXPECTED:ns16550.ns16550InputChar.return:\0
TEST.END

-- Subprogram: ns16550OutputChar

-- Test Case: outputchar1
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550OutputChar
TEST.NEW
TEST.NAME:outputchar1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) while ((readb(&(siodev->lsr)) & 0x20) == 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:uut_prototype_stubs.readb.return:32
TEST.STUB_EXP_USER_CODE:uut_prototype_stubs.writeb.val
{{ <<uut_prototype_stubs.writeb.val>> == ( 'A' ) }}
TEST.END_STUB_EXP_USER_CODE:
TEST.VALUE_USER_CODE:ns16550.ns16550OutputChar.c
<<ns16550.ns16550OutputChar.c>> = ( 'A' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: outputchar2
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550OutputChar
TEST.NEW
TEST.NAME:outputchar2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) while ((readb(&(siodev->lsr)) & 0x20) == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:uut_prototype_stubs.readb
TEST.STUB:uut_prototype_stubs.writeb
TEST.VALUE:uut_prototype_stubs.readb.return:0
TEST.STUB_EXP_USER_CODE:uut_prototype_stubs.writeb.val
{{ <<uut_prototype_stubs.writeb.val>> == ( 'B' ) }}
TEST.END_STUB_EXP_USER_CODE:
TEST.VALUE_USER_CODE:ns16550.ns16550OutputChar.c
<<ns16550.ns16550OutputChar.c>> = ( 'B' );
TEST.END_VALUE_USER_CODE:
TEST.END

-- Subprogram: ns16550SioInit

-- Test Case: IOinit
TEST.UNIT:ns16550
TEST.SUBPROGRAM:ns16550SioInit
TEST.NEW
TEST.NAME:IOinit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:ns16550.ns16550DevInit
TEST.END
