-- VectorCAST 6.4c (02/03/16)
-- Test Case Script
-- 
-- Environment    : CAGOS
-- Unit(s) Under Test: sffs
-- 
-- Script Features
TEST.SCRIPT_FEATURE:C_DIRECT_ARRAY_INDEXING
TEST.SCRIPT_FEATURE:CPP_CLASS_OBJECT_REVISION
TEST.SCRIPT_FEATURE:MULTIPLE_UUT_SUPPORT
TEST.SCRIPT_FEATURE:MIXED_CASE_NAMES
TEST.SCRIPT_FEATURE:STATIC_HEADER_FUNCS_IN_UUTS
--

-- Unit: sffs

-- Subprogram: NOLOG

-- Test Case: NOLOG.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:NOLOG
TEST.NEW
TEST.NAME:NOLOG.001
TEST.VALUE:sffs.NOLOG.c:<<malloc 5>>
TEST.VALUE:sffs.NOLOG.c:"aaaa"
TEST.END

-- Subprogram: dfs_erase

-- Test Case: erase1
TEST.UNIT:sffs
TEST.SUBPROGRAM:dfs_erase
TEST.NEW
TEST.NAME:erase1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.dfs_erase.dfd:<<MIN>>
TEST.VALUE:sffs.dfs_erase.no:<<MIN>>
TEST.VALUE:sffs.dfs_erase.num:<<MIN>>
TEST.EXPECTED:sffs.dfs_erase.return:0
TEST.END

-- Subprogram: dfs_read

-- Test Case: dfs_read.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:dfs_read
TEST.NEW
TEST.NAME:dfs_read.001
TEST.VALUE:sffs.dfs_read.buf:<<malloc 100>>
TEST.EXPECTED:sffs.dfs_read.return:80
TEST.END

-- Subprogram: dfs_write

-- Test Case: write1
TEST.UNIT:sffs
TEST.SUBPROGRAM:dfs_write
TEST.NEW
TEST.NAME:write1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.dfs_write.dfd:<<MIN>>
TEST.VALUE:sffs.dfs_write.dst:<<MIN>>
TEST.VALUE:sffs.dfs_write.src:<<malloc 1>>
TEST.VALUE:sffs.dfs_write.cnt:0
TEST.EXPECTED:sffs.dfs_write.return:0
TEST.END

-- Subprogram: fs_close

-- Test Case: close1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_close
TEST.NEW
TEST.NAME:close1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_close.fd:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_close.return:-1
TEST.END

-- Test Case: close2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_close
TEST.NEW
TEST.NAME:close2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_close.fd:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_close.return:-1
TEST.END

-- Test Case: close3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_close
TEST.NEW
TEST.NAME:close3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_close
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_close.return:-100
TEST.VALUE:sffs.fs_close.fd:<<MIN>>
TEST.EXPECTED:sffs.fs_close.return:-100
TEST.END

-- Subprogram: fs_file_len

-- Test Case: fsfilelen1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_file_len
TEST.NEW
TEST.NAME:fsfilelen1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_file_len.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_file_len.return:-1
TEST.END

-- Test Case: fsfilelen2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_file_len
TEST.NEW
TEST.NAME:fsfilelen2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MAX>>
TEST.VALUE:sffs.fs_file_len.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_file_len.return:-1
TEST.END

-- Test Case: fsfilelen3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_file_len
TEST.NEW
TEST.NAME:fsfilelen3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_file_len.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_file_len.return:-1
TEST.END

-- Test Case: fsfilelen4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_file_len
TEST.NEW
TEST.NAME:fsfilelen4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:-1
TEST.VALUE:sffs.fs_file_len.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_file_len.return:0
TEST.END

-- Test Case: fsfilelen5
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_file_len
TEST.NEW
TEST.NAME:fsfilelen5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_is_mounted.return:<<MAX>>
TEST.VALUE:sffs.fs_file_len.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_file_len.return:-1
TEST.END

-- Subprogram: fs_format

-- Test Case: format1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_format
TEST.NEW
TEST.NAME:format1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_format.type:<<MIN>>
TEST.END

-- Test Case: format2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_format
TEST.NEW
TEST.NAME:format2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_format.type:<<MIN>>
TEST.END

-- Test Case: format3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_format
TEST.NEW
TEST.NAME:format3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_format.type:<<MIN>>
TEST.END

-- Subprogram: fs_free

-- Test Case: fs_free1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_free
TEST.NEW
TEST.NAME:fs_free1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.EXPECTED:sffs.fs_free.return:-1
TEST.END

-- Test Case: fs_free2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_free
TEST.NEW
TEST.NAME:fs_free2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.EXPECTED:sffs.fs_free.return:-1
TEST.END

-- Test Case: fs_free3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_free
TEST.NEW
TEST.NAME:fs_free3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < sizeof (fs->free_se) / (4)) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Cannot set local variable fs in branch 4
      Cannot set local variable fs in branch 4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:uut_prototype_stubs.get_all_one.return:1
TEST.EXPECTED:sffs.fs_free.return:0
TEST.END

-- Subprogram: fs_get_err

-- Test Case: get_err1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_get_err
TEST.NEW
TEST.NAME:get_err1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.EXPECTED:sffs.fs_get_err.return:-1
TEST.END

-- Test Case: get_err2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_get_err
TEST.NEW
TEST.NAME:get_err2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.EXPECTED:sffs.fs_get_err.return:-1
TEST.END

-- Test Case: get_err3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_get_err
TEST.NEW
TEST.NAME:get_err3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_unlock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_unlock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:1
TEST.EXPECTED:sffs.fs_get_err.return:0
TEST.END

-- Test Case: get_err4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_get_err
TEST.NEW
TEST.NAME:get_err4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_unlock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_unlock.return:-1
TEST.VALUE:sffs.sffs_is_mounted.return:1
TEST.EXPECTED:sffs.fs_get_err.return:0
TEST.END

-- Subprogram: fs_ls

-- Test Case: fs_ls1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls1
TEST.BASIS_PATH:1 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Test Case: fs_ls2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls2
TEST.BASIS_PATH:2 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Test Case: fs_ls3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls3
TEST.BASIS_PATH:3 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < fs->se_size / fs->ss_size) ==> FALSE
      (8) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.ss_size:2
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.se_size:20
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Test Case: fs_ls4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < fs->se_size / fs->ss_size) ==> FALSE
      (8) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].stat:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.ss_size:2
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.se_size:20
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.VALUE_USER_CODE:sffs.<<GLOBAL>>.os_sffs.read
<<sffs.<<GLOBAL>>.os_sffs>>.read = dfs_read;
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: fs_ls5
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < fs->se_size / fs->ss_size) ==> FALSE
      (8) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].comt:0xC0
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].deled:0xFE
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].stat:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.ss_size:2
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.se_size:20
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Test Case: fs_ls6
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < fs->se_size / fs->ss_size) ==> FALSE
      (8) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].comt:0xC0
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].deled:0xFE
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].stat:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.ss_size:2
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.se_size:20
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.read:dfs_read
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Test Case: fs_ls7
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_ls
TEST.NEW
TEST.NAME:fs_ls7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < fs->se_size / fs->ss_size) ==> FALSE
      (8) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable fs in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Conflict: Unable to validate expression-to-expression comparison in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].comt:0xC0
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].deled:0xFE
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[8].stat:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].magic_num:0xCA905001
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].name_len:18
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[9].deled:0xFF
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.ss_size:2
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.se_size:20
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.read:dfs_read
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_ls.return:-1
TEST.END

-- Subprogram: fs_lseek

-- Test Case: fs_lseek1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:<<MIN>>
TEST.VALUE:sffs.fs_lseek.cfg:<<MIN>>
TEST.EXPECTED:sffs.fs_lseek.return:-1
TEST.END

-- Test Case: fs_lseek2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:<<MIN>>
TEST.VALUE:sffs.fs_lseek.cfg:<<MIN>>
TEST.EXPECTED:sffs.fs_lseek.return:-1
TEST.END

-- Test Case: fs_lseek3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek3
TEST.BASIS_PATH:3 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (sffs_ck_fd(fs, fd) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:<<MIN>>
TEST.VALUE:sffs.fs_lseek.cfg:<<MIN>>
TEST.EXPECTED:sffs.fs_lseek.return:-1
TEST.END

-- Test Case: fs_lseek4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek4
TEST.BASIS_PATH:4 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (6) case cfg ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:<<MIN>>
TEST.VALUE:sffs.fs_lseek.cfg:<<MIN>>
TEST.EXPECTED:sffs.fs_lseek.return:-1
TEST.END

-- Test Case: fs_lseek5
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek5
TEST.BASIS_PATH:5 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (4) case cfg ==> 1
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:<<MIN>>
TEST.VALUE:sffs.fs_lseek.cfg:1
TEST.EXPECTED:sffs.fs_lseek.return:0
TEST.END

-- Test Case: fs_lseek6
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_lseek
TEST.NEW
TEST.NAME:fs_lseek6
TEST.BASIS_PATH:6 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (5) case cfg ==> 0
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.fs_lseek.pos:10
TEST.VALUE:sffs.fs_lseek.cfg:0
TEST.EXPECTED:sffs.fs_lseek.return:-1
TEST.END

-- Subprogram: fs_mount

-- Test Case: fsmount1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_mount
TEST.NEW
TEST.NAME:fsmount1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.fs_mount.type:1
TEST.VALUE:sffs.fs_mount.return:0
TEST.END

-- Test Case: fsmount2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_mount
TEST.NEW
TEST.NAME:fsmount2
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_start.return:-2147483647
TEST.VALUE:sffs.fs_mount.type:2
TEST.VALUE:sffs.fs_mount.return:-1
TEST.END

-- Test Case: fsmount3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_mount
TEST.NEW
TEST.NAME:fsmount3
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.fs_mount.type:2
TEST.VALUE:sffs.fs_mount.return:-1
TEST.END

-- Subprogram: fs_open

-- Test Case: fsopen1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen1
TEST.BASIS_PATH:1 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:<<MIN>>
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_open.return:-1
TEST.END

-- Test Case: fsopen2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen2
TEST.BASIS_PATH:2 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:<<MIN>>
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_open.return:-1
TEST.END

-- Test Case: fsopen3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen3
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (7) case flags ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:<<MIN>>
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-1
TEST.EXPECTED:sffs.fs_open.return:-1
TEST.END

-- Test Case: fsopen4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen4
TEST.BASIS_PATH:4 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) case flags ==> 0
      (4) if (1 < 0) ==> TRUE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.lock:12
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:<<MIN>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:0
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.fs_open.return:-1
TEST.END

-- Test Case: fsopen5
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen5
TEST.BASIS_PATH:5 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) case flags ==> 0
      (4) if (1 < 0) ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_open
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:<<MAX>>
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_open.return:0
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:0
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.fs_open.return:0
TEST.END

-- Test Case: fsopen6
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen6
TEST.BASIS_PATH:6 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (5) case flags ==> 1
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_open
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_open.return:50
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:1
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.fs_open.return:50
TEST.END

-- Test Case: fsopen7
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_open
TEST.NEW
TEST.NAME:fsopen7
TEST.BASIS_PATH:7 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 7
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (6) case flags ==> 2
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_open
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_open.return:600
TEST.VALUE:sffs.fs_open.fname:<<malloc 1>>
TEST.VALUE:sffs.fs_open.flags:2
TEST.VALUE:sffs.fs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.fs_open.return:600
TEST.END

-- Subprogram: fs_read

-- Test Case: read1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_read
TEST.NEW
TEST.NAME:read1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_read.fd:<<MIN>>
TEST.VALUE:sffs.fs_read.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_read.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_read.return:-1
TEST.END

-- Test Case: read2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_read
TEST.NEW
TEST.NAME:read2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_read.fd:<<MIN>>
TEST.VALUE:sffs.fs_read.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_read.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_read.return:-1
TEST.END

-- Test Case: read3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_read
TEST.NEW
TEST.NAME:read3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (buf == (void *)0) ==> TRUE
      (4) while 1 ==> FALSE
   Test Case Generation Notes:
      Only one possible value for condition in branch 4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_read.fd:<<MIN>>
TEST.VALUE:sffs.fs_read.buf:<<null>>
TEST.VALUE:sffs.fs_read.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-25
TEST.EXPECTED:sffs.fs_read.return:-1
TEST.END

-- Test Case: read4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_read
TEST.NEW
TEST.NAME:read4
TEST.BASIS_PATH:5 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (buf == (void *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_read
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_read.return:-1
TEST.VALUE:sffs.fs_read.fd:<<MIN>>
TEST.VALUE:sffs.fs_read.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_read.count:<<MIN>>
TEST.EXPECTED:sffs.fs_read.return:-1
TEST.END

-- Subprogram: fs_remove

-- Test Case: remove1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_remove
TEST.NEW
TEST.NAME:remove1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_remove.fname:<<malloc 1>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_remove.return:-1
TEST.END

-- Test Case: remove2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_remove
TEST.NEW
TEST.NAME:remove2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_remove.fname:<<malloc 1>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_remove.return:-1
TEST.END

-- Test Case: remove3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_remove
TEST.NEW
TEST.NAME:remove3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_remove_byname
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byname.return:3
TEST.VALUE:sffs.fs_remove.fname:<<malloc 1>>
TEST.EXPECTED:sffs.fs_remove.return:3
TEST.END

-- Subprogram: fs_rename

-- Test Case: fs_rename1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_rename
TEST.NEW
TEST.NAME:fs_rename1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_rename.old_name:<<malloc 1>>
TEST.VALUE:sffs.fs_rename.new_name:<<malloc 1>>
TEST.EXPECTED:sffs.fs_rename.return:-1
TEST.END

-- Test Case: fs_rename2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_rename
TEST.NEW
TEST.NAME:fs_rename2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_rename.old_name:<<malloc 1>>
TEST.VALUE:sffs.fs_rename.new_name:<<malloc 1>>
TEST.EXPECTED:sffs.fs_rename.return:-1
TEST.END

-- Test Case: fs_rename3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_rename
TEST.NEW
TEST.NAME:fs_rename3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_rename
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_rename.return:0
TEST.VALUE:sffs.fs_rename.old_name:<<malloc 1>>
TEST.VALUE:sffs.fs_rename.new_name:<<malloc 1>>
TEST.EXPECTED:sffs.fs_rename.return:0
TEST.END

-- Subprogram: fs_umount

-- Test Case: fsumount1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_umount
TEST.NEW
TEST.NAME:fsumount1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:-2147483648
TEST.VALUE:sffs.fs_umount.fsid:<<MAX>>
TEST.VALUE:sffs.fs_umount.return:-1
TEST.END

-- Test Case: fsumount2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_umount
TEST.NEW
TEST.NAME:fsumount2
TEST.BASIS_PATH:2 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_umount.fsid:<<MIN>>
TEST.EXPECTED:sffs.fs_umount.return:-1
TEST.END

-- Test Case: fsumount3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_umount
TEST.NEW
TEST.NAME:fsumount3
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_umount
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:1
TEST.VALUE:sffs.sffs_umount.return:1
TEST.VALUE:sffs.fs_umount.fsid:0
TEST.EXPECTED:sffs.fs_umount.return:1
TEST.END

-- Subprogram: fs_used

-- Test Case: fs_used1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_used
TEST.NEW
TEST.NAME:fs_used1
TEST.BASIS_PATH:1 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.EXPECTED:sffs.fs_used.return:-1
TEST.END

-- Test Case: fs_used2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_used
TEST.NEW
TEST.NAME:fs_used2
TEST.BASIS_PATH:2 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.EXPECTED:sffs.fs_used.return:-1
TEST.END

-- Test Case: fs_used3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_used
TEST.NEW
TEST.NAME:fs_used3
TEST.BASIS_PATH:3 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_used.return:0
TEST.END

-- Test Case: fsused4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_used
TEST.NEW
TEST.NAME:fsused4
TEST.BASIS_PATH:4 of 5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) for (i < 20) ==> TRUE
      (4) if (((fs->file)[i]).comt == 0xc0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 3
      Cannot set local variable fs in branch 4
      Cannot set i due to assignment
      Cannot set local variable fs in branch 4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].size:100
TEST.VALUE:sffs.<<GLOBAL>>.os_sffs.file[0].comt:0xC0
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.EXPECTED:sffs.fs_used.return:100
TEST.END

-- Subprogram: fs_write

-- Test Case: write1
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_write
TEST.NEW
TEST.NAME:write1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_lock(fs) != 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.VALUE:sffs.sffs_lock.return:<<MIN>>
TEST.VALUE:sffs.fs_write.fd:<<MIN>>
TEST.VALUE:sffs.fs_write.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_write.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-2
TEST.EXPECTED:sffs.fs_write.return:-1
TEST.END

-- Test Case: write2
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_write
TEST.NEW
TEST.NAME:write2
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:0
TEST.VALUE:sffs.fs_write.fd:<<MIN>>
TEST.VALUE:sffs.fs_write.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_write.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-23
TEST.EXPECTED:sffs.fs_write.return:-1
TEST.END

-- Test Case: write3
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_write
TEST.NEW
TEST.NAME:write3
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (buf == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.fs_write.fd:<<MIN>>
TEST.VALUE:sffs.fs_write.buf:<<null>>
TEST.VALUE:sffs.fs_write.count:<<MIN>>
TEST.EXPECTED:sffs.<<GLOBAL>>.os_sffs.errno:-25
TEST.EXPECTED:sffs.fs_write.return:-1
TEST.END

-- Test Case: write4
TEST.UNIT:sffs
TEST.SUBPROGRAM:fs_write
TEST.NEW
TEST.NAME:write4
TEST.BASIS_PATH:4 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_lock(fs) != 0) ==> FALSE
      (2) if (!sffs_is_mounted(fs)) ==> FALSE
      (3) if (buf == (void *)0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_is_mounted
TEST.STUB:sffs.sffs_write
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_is_mounted.return:<<MIN>>
TEST.VALUE:sffs.sffs_write.return:70
TEST.VALUE:sffs.fs_write.fd:<<MIN>>
TEST.VALUE:sffs.fs_write.buf:VECTORCAST_INT1
TEST.VALUE:sffs.fs_write.count:<<MIN>>
TEST.EXPECTED:sffs.fs_write.return:70
TEST.END

-- Subprogram: intLock

-- Test Case: intLock.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:intLock
TEST.NEW
TEST.NAME:intLock.001
TEST.EXPECTED:sffs.intLock.return:0
TEST.END

-- Subprogram: intUnlock

-- Test Case: intUnlock.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:intUnlock
TEST.NEW
TEST.NAME:intUnlock.001
TEST.VALUE:sffs.intUnlock.a:-2147483648
TEST.END

-- Test Case: intUnlock.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:intUnlock
TEST.NEW
TEST.NAME:intUnlock.002
TEST.VALUE:sffs.intUnlock.a:0
TEST.END

-- Test Case: intUnlock.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:intUnlock
TEST.NEW
TEST.NAME:intUnlock.003
TEST.VALUE:sffs.intUnlock.a:2147483647
TEST.END

-- Subprogram: sffs_check_fid

-- Test Case: check_fid1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_check_fid
TEST.NEW
TEST.NAME:check_fid1
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < fs->fileNum) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_check_fid.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_check_fid.fs[0].file[0].fid:0
TEST.VALUE:sffs.sffs_check_fid.fs[0].file[1].fid:0
TEST.VALUE:sffs.sffs_check_fid.fs[0].file[19].fid:4294967295
TEST.VALUE:sffs.sffs_check_fid.fs[0].fileNum:19
TEST.VALUE:sffs.sffs_check_fid.fid:11
TEST.EXPECTED:sffs.sffs_check_fid.return:-1
TEST.END

-- Test Case: check_fid2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_check_fid
TEST.NEW
TEST.NAME:check_fid2
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < fs->fileNum) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_check_fid.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_check_fid.fs[0].file[0].fid:0
TEST.VALUE:sffs.sffs_check_fid.fs[0].file[1].fid:1
TEST.VALUE:sffs.sffs_check_fid.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_check_fid.fid:1
TEST.EXPECTED:sffs.sffs_check_fid.return:1
TEST.END

-- Subprogram: sffs_ck_fd

-- Test Case: ck_fd1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fs[0].file[0].size:0
TEST.VALUE:sffs.sffs_ck_fd.fd:0
TEST.EXPECTED:sffs.sffs_ck_fd.return:0
TEST.END

-- Test Case: ck_fd2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fd:-2147483648
TEST.EXPECTED:sffs.sffs_ck_fd.return:-1
TEST.END

-- Test Case: ck_fd3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fs[0].file[0].size:-2147483648
TEST.VALUE:sffs.sffs_ck_fd.fd:0
TEST.EXPECTED:sffs.sffs_ck_fd.return:-1
TEST.END

-- Test Case: ck_fd4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fs[0].file[0].size:1
TEST.VALUE:sffs.sffs_ck_fd.fd:19
TEST.EXPECTED:sffs.sffs_ck_fd.return:0
TEST.END

-- Test Case: ck_fd5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd5
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fd:20
TEST.EXPECTED:sffs.sffs_ck_fd.return:-1
TEST.END

-- Test Case: ck_fd6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ck_fd
TEST.NEW
TEST.NAME:ck_fd6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((0 <= fd && fd < 20) && ((fs->file)[fd]).size >= 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ck_fd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ck_fd.fd:2147483647
TEST.EXPECTED:sffs.sffs_ck_fd.return:-1
TEST.END

-- Subprogram: sffs_ckstat

-- Test Case: ckstat1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ckstat
TEST.NEW
TEST.NAME:ckstat1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ckstat.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ckstat.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_ckstat.fd:0
TEST.EXPECTED:sffs.sffs_ckstat.return:2
TEST.END

-- Test Case: ckstat2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_ckstat
TEST.NEW
TEST.NAME:ckstat2
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_ckstat.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_ckstat.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_ckstat.fs[0].file[19].stat:3
TEST.VALUE:sffs.sffs_ckstat.fd:19
TEST.EXPECTED:sffs.sffs_ckstat.return:3
TEST.END

-- Subprogram: sffs_clear_errno

-- Test Case: clear_errno
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_clear_errno
TEST.NEW
TEST.NAME:clear_errno
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_clear_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_clear_errno.fs[0].errno:0
TEST.END

-- Subprogram: sffs_clear_sebit

-- Test Case: clear_sebit
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_clear_sebit
TEST.NEW
TEST.NAME:clear_sebit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_clear_sebit.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_clear_sebit.fs[0].free_se[0]:2
TEST.VALUE:sffs.sffs_clear_sebit.se_no:2
TEST.END

-- Subprogram: sffs_close

-- Test Case: sffs_close1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close1
TEST.BASIS_PATH:1 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fd:<<MIN>>
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:-9
TEST.EXPECTED:sffs.sffs_close.return:-1
TEST.END

-- Test Case: sffs_close10
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:1
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fileNum:4
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:13
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.fsStat:2
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].comt:0xEE
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_close.fs[0].fileNum:3
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_close.return:-1
TEST.END

-- Test Case: sffs_close11
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close11
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:100
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fileNum:4
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:13
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].comt:0xEE
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_close.fs[0].fileNum:3
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_close.return:-1
TEST.END

-- Test Case: sffs_close2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close2
TEST.BASIS_PATH:2 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:192
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.END

-- Test Case: sffs_close3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close3
TEST.BASIS_PATH:3 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_read_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:<<MIN>>
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:1
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:1
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_close.fs[0].magic_num:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:5
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].magic_num:<<MAX>>
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].comt:0xC0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].fidcnt:2
TEST.EXPECTED:sffs.sffs_close.fs[0].seqcnt:2
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.END

-- Test Case: sffs_close3.4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close3.4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_ckstat.return:<<MIN>>
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:1
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:1
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_close.fs[0].magic_num:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:5
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[1]:1
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].magic_num:<<MAX>>
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].comt:0xC0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].free_ss_hdr[1]:1
TEST.EXPECTED:sffs.sffs_close.fs[0].fidcnt:2
TEST.EXPECTED:sffs.sffs_close.fs[0].seqcnt:2
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.END

-- Test Case: sffs_close5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close5
TEST.BASIS_PATH:4 of 14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:1
TEST.VALUE:sffs.sffs_erase_at.return:-1
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:<<MIN>>
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:-4
TEST.EXPECTED:sffs.sffs_close.return:-1
TEST.END

-- Test Case: sffs_close6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:1
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_write_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:<<MIN>>
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.fsStat:2
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_close.fs[0].fidcnt:256
TEST.EXPECTED:sffs.sffs_close.fs[0].seqcnt:256
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.END

-- Test Case: sffs_close7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:1
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fileNum:4
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:13
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.fsStat:1
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].comt:0xEE
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].fileNum:3
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_close.return:-1
TEST.END

-- Test Case: sffs_close8
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:1
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_write_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:<<MIN>>
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.fsStat:2
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_close.fs[0].fidcnt:256
TEST.EXPECTED:sffs.sffs_close.fs[0].seqcnt:256
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: sffs_close9
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_close
TEST.NEW
TEST.NAME:sffs_close9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if (sffs_ck_fd(fs, fd) < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (sffs_ckstat(fs, fd) == 2) ==> FALSE
      (11) if (sffs_read_at(fs, ofs, hdd, (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) < (UINT32)(&(((fs->file)[fd]).deled)) - (UINT32)(&((fs->file)[fd]))) ==> FALSE
      (12) for (i < ((((((((((((0 + 4) + 18) + 2) + 4) + 4) + 4) + 4) + 16 * 2) + 2) + 1) + 1) + 1) + 2) ==> FALSE
      (13) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 12
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.<<GLOBAL>>.fsStat:<<MAX>>
TEST.VALUE:sffs.sffs_read_at.return:0
TEST.VALUE:sffs.sffs_write_at.size:<<MAX>>
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_close.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_close.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_close.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_close.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_close.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_close.fs[0].se_size:5
TEST.VALUE:sffs.sffs_close.fd:0
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].fid:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].seq:255
TEST.EXPECTED:sffs.sffs_close.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_close.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_close.fs[0].fidcnt:256
TEST.EXPECTED:sffs.sffs_close.fs[0].seqcnt:256
TEST.EXPECTED:sffs.sffs_close.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_at.return
static int i=74;
i=i+1;
<<sffs.sffs_write_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Subprogram: sffs_erase_at

-- Test Case: erase1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_erase_at
TEST.NEW
TEST.NAME:erase1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < num) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_erase_at.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_erase_at.fs[0].erase:dfs_erase
TEST.VALUE:sffs.sffs_erase_at.no_from:0
TEST.VALUE:sffs.sffs_erase_at.num:240
TEST.EXPECTED:sffs.sffs_erase_at.return:0
TEST.END

-- Test Case: erase2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_erase_at
TEST.NEW
TEST.NAME:erase2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < num) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_erase_at.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_erase_at.fs[0].erase:dfs_erase
TEST.VALUE:sffs.sffs_erase_at.no_from:240
TEST.VALUE:sffs.sffs_erase_at.num:0
TEST.EXPECTED:sffs.sffs_erase_at.return:0
TEST.END

-- Subprogram: sffs_find_filehdr

-- Test Case: filehdr1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_find_filehdr
TEST.NEW
TEST.NAME:filehdr1
TEST.BASIS_PATH:1 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_find_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_find_filehdr.fs[0].file[0].deled:0xDE
TEST.VALUE:sffs.sffs_find_filehdr.fname:<<malloc 1>>
TEST.EXPECTED:sffs.sffs_find_filehdr.fs[0].errno:-8
TEST.EXPECTED:sffs.sffs_find_filehdr.return:-1
TEST.END

-- Test Case: filehdr2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_find_filehdr
TEST.NEW
TEST.NAME:filehdr2
TEST.BASIS_PATH:3 of 3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) for (i < 20) ==> TRUE
      (2) if (((((fs->file)[i]).deled != 0xde && ((fs->file)[i]).name_len > 0) && strlen((char *)fname) == ((fs->file)[i]).name_len) && strncmp((char *)fname, (char *)(((fs->file)[i]).name), ((fs->file)[i]).name_len) == 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Cannot set i due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_find_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_find_filehdr.fs[0].file[0].name:<<malloc 6>>
TEST.VALUE:sffs.sffs_find_filehdr.fs[0].file[0].name:"aaaaa"
TEST.VALUE:sffs.sffs_find_filehdr.fs[0].file[0].name_len:5
TEST.VALUE:sffs.sffs_find_filehdr.fs[0].file[0].deled:0xDD
TEST.VALUE:sffs.sffs_find_filehdr.fname:<<malloc 6>>
TEST.VALUE:sffs.sffs_find_filehdr.fname:"aaaaa"
TEST.EXPECTED:sffs.sffs_find_filehdr.return:0
TEST.END

-- Subprogram: sffs_format

-- Test Case: format1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format1
TEST.BASIS_PATH:1 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (9) case type ==> default
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.type:2
TEST.EXPECTED:sffs.sffs_format.return:0
TEST.END

-- Test Case: format2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format2
TEST.BASIS_PATH:2 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) case type ==> 2
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.type:<<MIN>>
TEST.EXPECTED:sffs.sffs_format.return:0
TEST.END

-- Test Case: format3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format3
TEST.BASIS_PATH:3 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (2) case type ==> 0
      (3) if (ret < 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_start.return:0
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.type:0
TEST.EXPECTED:sffs.sffs_format.return:0
TEST.END

-- Test Case: format4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format4
TEST.BASIS_PATH:4 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (2) case type ==> 0
      (3) if (ret < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_erase_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.type:0
TEST.EXPECTED:sffs.sffs_format.fs[0].errno:-4
TEST.EXPECTED:sffs.sffs_format.return:-1
TEST.END

-- Test Case: format5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format5
TEST.BASIS_PATH:5 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 5
      (2) case type ==> 0
      (3) if (ret < 0) ==> TRUE
      (4) if (ret < 0) ==> FALSE
   Test Case Generation Notes:
      Conflict: Trying to set variable sffs.sffs_erase_at.return 'less than' and 'greater than' same value in branches 3/4
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_start.return:0
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.type:0
TEST.EXPECTED:sffs.sffs_format.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;

i=i+1;
<<sffs.sffs_erase_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: format6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format6
TEST.BASIS_PATH:6 of 9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case type ==> 1
      (6) for (i < fs->device_size / fs->erase_unit) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 6
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_erase_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_start.return:0
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.fs[0].device_size:20
TEST.VALUE:sffs.sffs_format.fs[0].erase_unit:5
TEST.VALUE:sffs.sffs_format.type:1
TEST.EXPECTED:sffs.sffs_format.return:-1
TEST.END

-- Test Case: format7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_format
TEST.NEW
TEST.NAME:format7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 6
      (5) case type ==> 1
      (6) for (i < fs->device_size / fs->erase_unit) ==> FALSE
   Test Case Generation Notes:
      Cannot set local variable i in branch 6
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_start.return:0
TEST.VALUE:sffs.sffs_format.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_format.fs[0].device_size:20
TEST.VALUE:sffs.sffs_format.fs[0].erase_unit:5
TEST.VALUE:sffs.sffs_format.type:1
TEST.EXPECTED:sffs.sffs_format.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Subprogram: sffs_get_errno

-- Test Case: errno1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_errno
TEST.NEW
TEST.NAME:errno1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_errno.fs[0].errno:-2147483648
TEST.EXPECTED:sffs.sffs_get_errno.return:-2147483648
TEST.END

-- Test Case: errno2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_errno
TEST.NEW
TEST.NAME:errno2
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_errno.fs[0].errno:2147483647
TEST.EXPECTED:sffs.sffs_get_errno.return:2147483647
TEST.END

-- Test Case: errno3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_errno
TEST.NEW
TEST.NAME:errno3
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_errno.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_get_errno.return:0
TEST.END

-- Subprogram: sffs_get_filehdr

-- Test Case: filenum=20_fs_fail
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum=20_fs_fail
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->fileNum >= 20) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:20
TEST.EXPECTED:sffs.sffs_get_filehdr.return:-1
TEST.END

-- Test Case: filenum_fsfail
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum_fsfail
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[2].hdr_ss:4
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[10].hdr_ss:5
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[19].hdr_ss:6
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:19
TEST.EXPECTED:sffs.sffs_get_filehdr.return:-1
TEST.END

-- Test Case: filenum_i
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum_i
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[2].hdr_ss:4
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[10].hdr_ss:5
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[19].hdr_ss:-1
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:19
TEST.EXPECTED:sffs.sffs_get_filehdr.return:19
TEST.END

-- Test Case: filenum_max_fsfail
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum_max_fsfail
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->fileNum >= 20) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:2147483647
TEST.EXPECTED:sffs.sffs_get_filehdr.return:-1
TEST.END

-- Test Case: filenum_min_fsfail
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum_min_fsfail
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (i < 20) ==> TRUE
      (3) if (((fs->file)[i]).hdr_ss < 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
      Cannot set i due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[0].hdr_ss:32767
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:-2147483648
TEST.EXPECTED:sffs.sffs_get_filehdr.return:-1
TEST.END

-- Test Case: filenum_min_i
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_filehdr
TEST.NEW
TEST.NAME:filenum_min_i
TEST.BASIS_PATH:3 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (i < 20) ==> TRUE
      (3) if (((fs->file)[i]).hdr_ss < 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 2
      Cannot set i due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].file[0].hdr_ss:-32768
TEST.VALUE:sffs.sffs_get_filehdr.fs[0].fileNum:-2147483648
TEST.EXPECTED:sffs.sffs_get_filehdr.return:0
TEST.END

-- Subprogram: sffs_get_hdrss

-- Test Case: get_hdrss1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss1
TEST.BASIS_PATH:1 of 19
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->fileNum >= 20) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2147483647
TEST.VALUE:sffs.sffs_get_hdrss.fd:2147483647
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.END

-- Test Case: get_hdrss10
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss10
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.<<GLOBAL>>.hdrsStat[0]:1
TEST.VALUE:sffs.<<GLOBAL>>.hdrsStat[1]:11
TEST.VALUE:sffs.<<GLOBAL>>.hdrsStat[2]:2
TEST.VALUE:sffs.sffs_write_at.return:-200
TEST.VALUE:sffs.sffs_erase_at.return:5
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].hdr_ss:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].hdr_ss:-1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].comt:238
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].deled:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].stat:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:127
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[1]:127
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[2]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0..1]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xEE
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].stat:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].errno:-5
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.END

-- Test Case: get_hdrss11
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss11
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_erase_at.return:-1
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.END

-- Test Case: get_hdrss12
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss12
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:-200000
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:192
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:1
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = (i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: get_hdrss13
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss13
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:3
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xEE
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = (i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: get_hdrss14
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss14
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.<<GLOBAL>>.hdrsStat[0]:3
TEST.VALUE:sffs.<<GLOBAL>>.hdrsStat[1]:1
TEST.VALUE:sffs.sffs_erase_at.return:3
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[1].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[2].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[3].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[3].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[3].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[3].stat:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[4].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[4].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[4].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[4].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[5].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[5].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[5].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[5].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[6].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[6].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[6].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[6].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[7].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[7].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[7].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[7].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[8].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[8].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[8].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[8].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[9].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[9].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[9].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[9].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[10].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[10].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[10].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[10].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[11].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[11].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[11].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[11].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[12].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[12].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[12].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[12].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[13].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[13].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[13].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[13].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[14].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[14].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[14].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[14].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[15].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[15].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[15].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[15].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[16].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[16].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[16].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[16].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[17].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[17].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[17].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[17].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[18].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[18].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[18].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[18].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[19].hdr_ss:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[19].comt:0xEE
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[19].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[19].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[1]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:19
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[1]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:-1
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.END

-- Test Case: get_hdrss2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss2
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[5]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[6]:8
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:5
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[5]:3
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[6]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:6
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_get_hdrss.return:6
TEST.END

-- Test Case: get_hdrss3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss3
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr:<<malloc 2>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr:"2"
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:-1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_get_hdrss.return:0
TEST.END

-- Test Case: get_hdrss4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss4
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:5
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_get_hdrss.return:5
TEST.END

-- Test Case: get_hdrss5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss5
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[5]:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[6]:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[16]:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:5
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:0
TEST.END

-- Test Case: get_hdrss6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss6
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:-1
TEST.END

-- Test Case: get_hdrss7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss7
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:-200
TEST.VALUE:sffs.sffs_erase_at.return:5
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:192
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:1
TEST.END

-- Test Case: get_hdrss8
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss8
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:-200
TEST.VALUE:sffs.sffs_erase_at.return:5
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0x0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].stat:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:1
TEST.END

-- Test Case: get_hdrss9
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_hdrss
TEST.NEW
TEST.NAME:get_hdrss9
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:200
TEST.VALUE:sffs.sffs_erase_at.return:5
TEST.VALUE:sffs.sffs_get_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].comt:0x0
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].deled:0xFF
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:3
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].comt:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].file[0].stat:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[0]:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr[3]:0
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].free_ss_hdr_cur:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].ss_size:1
TEST.EXPECTED:sffs.sffs_get_hdrss.fs[0].se_size:2
TEST.EXPECTED:sffs.sffs_get_hdrss.return:1
TEST.END

-- Subprogram: sffs_get_se

-- Test Case: get_se_1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_se
TEST.NEW
TEST.NAME:get_se_1
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (j < fs->device_size / fs->se_size) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
TEST.END_NOTES:
TEST.STUB:sffs.dfs_write
TEST.VALUE:sffs.dfs_write.return:12
TEST.VALUE:sffs.sffs_get_se.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_se.fs[0].device_size:12
TEST.VALUE:sffs.sffs_get_se.fs[0].free_se_cur:10
TEST.VALUE:sffs.sffs_get_se.fs[0].se_size:3
TEST.VALUE:uut_prototype_stubs.get_bit_val.return:1
TEST.EXPECTED:sffs.sffs_get_se.fs[0].device_size:12
TEST.EXPECTED:sffs.sffs_get_se.fs[0].free_se_cur:10
TEST.EXPECTED:sffs.sffs_get_se.fs[0].se_size:3
TEST.EXPECTED:sffs.sffs_get_se.return:-1
TEST.END

-- Test Case: get_se_2i
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_get_se
TEST.NEW
TEST.NAME:get_se_2i
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) for (j < fs->device_size / fs->se_size) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_get_se.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_get_se.fs[0].device_size:12
TEST.VALUE:sffs.sffs_get_se.fs[0].free_se_cur:10
TEST.VALUE:sffs.sffs_get_se.fs[0].se_size:3
TEST.VALUE:uut_prototype_stubs.get_bit_val.return:0
TEST.EXPECTED:sffs.sffs_get_se.return:10
TEST.END

-- Subprogram: sffs_is_mounted

-- Test Case: sffs_is_mounted_MAX.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_is_mounted
TEST.NEW
TEST.NAME:sffs_is_mounted_MAX.001
TEST.VALUE:<<ALL_MAX>>
TEST.VALUE:sffs.sffs_is_mounted.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_is_mounted.fs[0].mounted:2147483647
TEST.EXPECTED:sffs.sffs_is_mounted.return:2147483647
TEST.END

-- Test Case: sffs_is_mounted_MID.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_is_mounted
TEST.NEW
TEST.NAME:sffs_is_mounted_MID.001
TEST.VALUE:<<ALL_MID>>
TEST.VALUE:sffs.sffs_is_mounted.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_is_mounted.fs[0].mounted:-2147483648
TEST.EXPECTED:sffs.sffs_is_mounted.return:-2147483648
TEST.END

-- Test Case: sffs_is_mounted_MIN.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_is_mounted
TEST.NEW
TEST.NAME:sffs_is_mounted_MIN.001
TEST.VALUE:<<ALL_MIN>>
TEST.VALUE:sffs.sffs_is_mounted.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_is_mounted.fs[0].mounted:0
TEST.EXPECTED:sffs.sffs_is_mounted.return:0
TEST.END

-- Subprogram: sffs_lock

-- Test Case: lock1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lock
TEST.NEW
TEST.NAME:lock1
TEST.VALUE:sffs.sffs_lock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lock.fs[0].lock:-128
TEST.EXPECTED:sffs.sffs_lock.return:0
TEST.END

-- Test Case: lock2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lock
TEST.NEW
TEST.NAME:lock2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->lock > 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_lock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lock.fs[0].lock:0
TEST.EXPECTED:sffs.sffs_lock.return:0
TEST.END

-- Test Case: lock3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lock
TEST.NEW
TEST.NAME:lock3
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->lock > 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_lock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lock.fs[0].lock:127
TEST.EXPECTED:sffs.sffs_lock.return:-1
TEST.END

-- Subprogram: sffs_lseek

-- Test Case: lseek1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.pos:<<MIN>>
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_lseek.return:-1
TEST.END

-- Test Case: lseek2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.pos:<<MIN>>
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_lseek.return:-1
TEST.END

-- Test Case: lseek3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.pos:<<MAX>>
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_lseek.return:-1
TEST.END

-- Test Case: lseek4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ckstat.return:2
TEST.VALUE:sffs.sffs_ck_fd.return:0
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fd:<<MIN>>
TEST.VALUE:sffs.sffs_lseek.pos:0
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:-19
TEST.EXPECTED:sffs.sffs_lseek.return:-1
TEST.END

-- Test Case: lseek53
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek53
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ckstat.return:255
TEST.VALUE:sffs.sffs_ck_fd.return:0
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fs[0].file[19].size:10
TEST.VALUE:sffs.sffs_lseek.fs[0].file[19].ofs:20
TEST.VALUE:sffs.sffs_lseek.fd:19
TEST.VALUE:sffs.sffs_lseek.pos:10
TEST.EXPECTED:sffs.sffs_lseek.fs[0].file[19].ofs:9
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_lseek.return:9
TEST.END

-- Test Case: lseek6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_lseek
TEST.NEW
TEST.NAME:lseek6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (sffs_ck_fd(fs, fd) < 0 || pos < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ckstat
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ckstat.return:255
TEST.VALUE:sffs.sffs_ck_fd.return:0
TEST.VALUE:sffs.sffs_lseek.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_lseek.fs[0].file[19].size:2147483645
TEST.VALUE:sffs.sffs_lseek.fs[0].file[19].ofs:20
TEST.VALUE:sffs.sffs_lseek.fd:19
TEST.VALUE:sffs.sffs_lseek.pos:10
TEST.EXPECTED:sffs.sffs_lseek.fs[0].file[19].ofs:30
TEST.EXPECTED:sffs.sffs_lseek.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_lseek.return:30
TEST.END

-- Subprogram: sffs_mount

-- Test Case: mount1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount1
TEST.BASIS_PATH:1 of 22
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->mounted == 1) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:1
TEST.VALUE:sffs.sffs_mount.type:2147483647
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount10
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:80
TEST.VALUE:sffs.sffs_check_fid.return:0
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:<<MIN>>
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0..4]:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount2
TEST.BASIS_PATH:2 of 22
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:-2147483648
TEST.VALUE:sffs.sffs_mount.type:1
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount3
TEST.BASIS_PATH:3 of 22
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_start.return:-2147483648
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:10000
TEST.VALUE:sffs.sffs_mount.type:-100
TEST.EXPECTED:sffs.sffs_mount.return:-1
TEST.END

-- Test Case: mount4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:-1000
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:-2147483648
TEST.VALUE:sffs.sffs_mount.type:-100
TEST.EXPECTED:sffs.sffs_mount.return:-1
TEST.END

-- Test Case: mount5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:10000
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:2147483647
TEST.VALUE:sffs.sffs_mount.type:-100
TEST.EXPECTED:sffs.sffs_mount.return:-1
TEST.END

-- Test Case: mount6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_check_fid.return:-1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:10000
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.type:-100
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-100
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:32
TEST.VALUE:sffs.sffs_check_fid.return:-1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:2147483647
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:10000
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.type:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:2147483647
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:2
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount8
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:32
TEST.VALUE:sffs.sffs_check_fid.return:-1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:-2147483648
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:10000
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount9
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_read_at.return:80
TEST.VALUE:sffs.sffs_check_fid.return:0
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount9.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_check_fid.return:-1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:3
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount9.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_write_at.return:1
TEST.VALUE:sffs.sffs_check_fid.return:1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[1].seq:3
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[1].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Test Case: mount9.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_write_at.return:-1
TEST.VALUE:sffs.sffs_check_fid.return:1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[1].seq:3
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[1].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:-1
TEST.END

-- Test Case: mount9.004
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_write_at.return:-1
TEST.VALUE:sffs.sffs_check_fid.return:1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[1].seq:3
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[1].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].errno:-10
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:-1
TEST.END

-- Test Case: mount9.005
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_check_fid.return:1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[1].seq:3
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:5
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[1].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_at.return
static int i=-1;
i=i+1;
<<sffs.sffs_write_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: mount9.006
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_mount
TEST.NEW
TEST.NAME:mount9.006
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (fs->mounted == 1) ==> FALSE
      (2) if (type == 1) ==> FALSE
      (3) if (sffs_start(fs) < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_lock
TEST.STUB:sffs.sffs_check_fid
TEST.STUB:sffs.sffs_start
TEST.VALUE:sffs.sffs_lock.return:0
TEST.VALUE:sffs.sffs_check_fid.return:-1
TEST.VALUE:sffs.sffs_start.return:2147483647
TEST.VALUE:sffs.sffs_mount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].se_no[0]:22
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.VALUE:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.VALUE:sffs.sffs_mount.fs[0].fileNum:0
TEST.VALUE:sffs.sffs_mount.fs[0].mounted:100
TEST.VALUE:sffs.sffs_mount.fs[0].fidcnt:255
TEST.VALUE:sffs.sffs_mount.fs[0].seqcnt:255
TEST.VALUE:sffs.sffs_mount.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_mount.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_mount.fs[0].se_size:5
TEST.VALUE:sffs.sffs_mount.fs[0].read:dfs_read
TEST.VALUE:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].fid:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].seq:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].se_no[0]:22
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].hdr_ss:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr[0]:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].free_ss_hdr_cur:2
TEST.EXPECTED:sffs.sffs_mount.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].mounted:1
TEST.EXPECTED:sffs.sffs_mount.fs[0].fidcnt:256
TEST.EXPECTED:sffs.sffs_mount.fs[0].seqcnt:255
TEST.EXPECTED:sffs.sffs_mount.fs[0].pageErase:255
TEST.EXPECTED:sffs.sffs_mount.fs[0].se_size:5
TEST.EXPECTED:sffs.sffs_mount.type:-2147483648
TEST.EXPECTED:sffs.sffs_mount.return:0
TEST.END

-- Subprogram: sffs_open

-- Test Case: open1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open1
TEST.BASIS_PATH:1 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fname == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fname:<<malloc 9>>
TEST.VALUE:sffs.sffs_open.fname:<<null>>
TEST.VALUE:sffs.sffs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.sffs_open.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_open.return:-1
TEST.END

-- Test Case: open2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open2
TEST.BASIS_PATH:2 of 8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:19
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:<<MIN>>
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].stat:1
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_open.return:19
TEST.END

-- Test Case: open3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:19
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:1
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].stat:1
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_open.return:19
TEST.END

-- Test Case: open4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:19
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].stat:2
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_open.return:19
TEST.END

-- Test Case: open5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_get_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_get_filehdr.return:-1
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_open.return:-1
TEST.END

-- Test Case: open6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_hdrss
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_get_filehdr
TEST.VALUE:sffs.sffs_get_hdrss.return:-1
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_get_filehdr.return:0
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:1
TEST.EXPECTED:sffs.sffs_open.return:-1
TEST.END

-- Test Case: open6.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open6.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_hdrss
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_get_filehdr
TEST.VALUE:sffs.sffs_get_hdrss.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_get_filehdr.return:0
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].name_len:6
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].hdr_ss:1
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_open.return:0
TEST.END

-- Test Case: open6.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open6.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_hdrss
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_get_filehdr
TEST.VALUE:sffs.sffs_get_hdrss.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_get_filehdr.return:0
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 21>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaabbbbbccccceeeee"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].name_len:18
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].hdr_ss:1
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_open.return:0
TEST.END

-- Test Case: open6.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_open
TEST.NEW
TEST.NAME:open6.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fname == (void *)0) ==> FALSE
      (2) if (pos < 0) ==> FALSE
      (6) if (mode == 1) ==> FALSE
      (7) if (mode == 2) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
      Cannot set local variable file in branch 7
      Cannot set local variable file in branch 8
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_hdrss
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_get_filehdr
TEST.VALUE:sffs.sffs_get_hdrss.return:0
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_get_filehdr.return:0
TEST.VALUE:sffs.sffs_open.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_open.fs[0].free_ss_hdr_cur:1
TEST.VALUE:sffs.sffs_open.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_open.fname:<<malloc 7>>
TEST.VALUE:sffs.sffs_open.fname:"aaaaaa"
TEST.VALUE:sffs.sffs_open.mode:2
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].name_len:6
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[0].hdr_ss:1
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].size:0
TEST.EXPECTED:sffs.sffs_open.fs[0].file[19].ofs:0
TEST.EXPECTED:sffs.sffs_open.fs[0].fileNum:2
TEST.EXPECTED:sffs.sffs_open.return:0
TEST.END

-- Subprogram: sffs_protect

-- Test Case: sffs_protect.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_protect
TEST.NEW
TEST.NAME:sffs_protect.001
TEST.END

-- Subprogram: sffs_read

-- Test Case: read1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read1
TEST.BASIS_PATH:1 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fd:<<MIN>>
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:<<MIN>>
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_read.return:-1
TEST.END

-- Test Case: read2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read2
TEST.BASIS_PATH:2 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fd:<<MIN>>
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:0
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Test Case: read3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read3
TEST.BASIS_PATH:3 of 13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:22
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:1
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-18
TEST.EXPECTED:sffs.sffs_read.return:-1
TEST.END

-- Test Case: read4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:-100
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:1
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:2
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:4
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:1
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_read.return:-100
TEST.END

-- Test Case: read4.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read4.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:-100
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:100
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:2
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:4
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:100
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_read.return:-100
TEST.END

-- Test Case: read5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:<<MAX>>
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:1
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:2
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:4
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:1
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Test Case: read5.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read5.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:0
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:20
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:1
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Test Case: read5.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read5.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:100
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:20
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:1
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Test Case: read5.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read5.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:100
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:20
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:2
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Test Case: read5.004
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read
TEST.NEW
TEST.NAME:read5.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (size == 0) ==> FALSE
      (3) if (file->stat != 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 3
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_read_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.return:-1
TEST.VALUE:sffs.sffs_read.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read.fs[0].file[0].size:20
TEST.VALUE:sffs.sffs_read.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_read.fs[0].file[0].stat:1
TEST.VALUE:sffs.sffs_read.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_read.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read.fd:0
TEST.VALUE:sffs.sffs_read.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read.size:2
TEST.EXPECTED:sffs.sffs_read.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_read.return:0
TEST.END

-- Subprogram: sffs_read_4k

-- Test Case: read_4k1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_4k
TEST.NEW
TEST.NAME:read_4k1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (size == (0)) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_read_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_4k.fd:<<MIN>>
TEST.VALUE:sffs.sffs_read_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read_4k.size:0
TEST.EXPECTED:sffs.sffs_read_4k.return:0
TEST.END

-- Test Case: read_4k2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_4k
TEST.NEW
TEST.NAME:read_4k2
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (size == (0)) ==> FALSE
      (2) if (file->size == 0 || file->ofs == file->size - 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_read_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].size:0
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].ofs:0
TEST.VALUE:sffs.sffs_read_4k.fd:19
TEST.VALUE:sffs.sffs_read_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read_4k.size:1
TEST.EXPECTED:sffs.sffs_read_4k.return:0
TEST.END

-- Test Case: read_4k2.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_4k
TEST.NEW
TEST.NAME:read_4k2.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (size == (0)) ==> FALSE
      (2) if (file->size == 0 || file->ofs == file->size - 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_read_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].size:1
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].ofs:0
TEST.VALUE:sffs.sffs_read_4k.fd:19
TEST.VALUE:sffs.sffs_read_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read_4k.size:1
TEST.EXPECTED:sffs.sffs_read_4k.return:0
TEST.END

-- Test Case: read_4k2.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_4k
TEST.NEW
TEST.NAME:read_4k2.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (size == (0)) ==> FALSE
      (2) if (file->size == 0 || file->ofs == file->size - 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.VALUE:sffs.sffs_read_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].size:<<MAX>>
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].ofs:7
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].se_no[1]:1
TEST.VALUE:sffs.sffs_read_4k.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_read_4k.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read_4k.fd:19
TEST.VALUE:sffs.sffs_read_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read_4k.size:1
TEST.EXPECTED:sffs.sffs_read_4k.fs[0].file[19].ofs:8
TEST.EXPECTED:sffs.sffs_read_4k.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_read_4k.return:<<MAX>>
TEST.END

-- Test Case: read_4k2.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_4k
TEST.NEW
TEST.NAME:read_4k2.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (size == (0)) ==> FALSE
      (2) if (file->size == 0 || file->ofs == file->size - 1) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.VALUE:sffs.sffs_read_at.return:-1
TEST.VALUE:sffs.sffs_read_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].size:10
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].ofs:7
TEST.VALUE:sffs.sffs_read_4k.fs[0].file[19].se_no[1]:1
TEST.VALUE:sffs.sffs_read_4k.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_read_4k.fs[0].se_size:5
TEST.VALUE:sffs.sffs_read_4k.fd:19
TEST.VALUE:sffs.sffs_read_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_read_4k.size:10
TEST.EXPECTED:sffs.sffs_read_4k.fs[0].file[19].ofs:7
TEST.EXPECTED:sffs.sffs_read_4k.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_read_4k.return:-1
TEST.END

-- Subprogram: sffs_read_at

-- Test Case: fs_read_notnull
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_at
TEST.NEW
TEST.NAME:fs_read_notnull
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->read != (void *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable fs->read in branch 1 since it has a type which requires user code.
TEST.END_NOTES:
TEST.STUB:sffs.dfs_read
TEST.VALUE:sffs.dfs_read.return:80
TEST.VALUE:sffs.sffs_read_at.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_at.offset:2
TEST.VALUE:sffs.sffs_read_at.data:VECTORCAST_BUFFER
TEST.VALUE:sffs.sffs_read_at.size:20
TEST.EXPECTED:sffs.sffs_read_at.return:80
TEST.VALUE_USER_CODE:sffs.sffs_read_at.fs.fs[0].read
<<sffs.sffs_read_at.fs>>[0].read = dfs_read;
TEST.END_VALUE_USER_CODE:
TEST.END

-- Test Case: fs_read_null
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_read_at
TEST.NEW
TEST.NAME:fs_read_null
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->read != (void *)0) ==> TRUE
   Test Case Generation Notes:
      Cannot set variable fs->read in branch 1 since it has a type which requires user code.
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_read_at.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_read_at.offset:2
TEST.VALUE:sffs.sffs_read_at.data:<<null>>
TEST.VALUE:sffs.sffs_read_at.size:20
TEST.EXPECTED:sffs.sffs_read_at.return:-1
TEST.END

-- Subprogram: sffs_remove_byfd

-- Test Case: remove_byfd1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd1
TEST.BASIS_PATH:1 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> TRUE
      (2) if (fs->pageErase != 1) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].hdr_ss:31
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].comt:0xBC
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_remove_byfd.fd:19
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].comt:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].unused:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[31]:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].errno:-20
TEST.EXPECTED:sffs.sffs_remove_byfd.return:0
TEST.END

-- Test Case: remove_byfd2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> TRUE
      (2) if (fs->pageErase != 1) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].hdr_ss:31
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[19].comt:0xBC
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_remove_byfd.fd:19
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].comt:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].unused:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[19].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[31]:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].errno:-20
TEST.EXPECTED:sffs.sffs_remove_byfd.return:0
TEST.END

-- Test Case: remove_byfd3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd3
TEST.BASIS_PATH:2 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> TRUE
      (2) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_write_at.return:1
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].hdr_ss:31
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].comt:0xC0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].se_size:5
TEST.VALUE:sffs.sffs_remove_byfd.fd:10
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[31]:2
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].comt:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].unused:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].deled:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[31]:2
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_remove_byfd.return:0
TEST.END

-- Test Case: remove_byfd4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> TRUE
      (2) if (fs->pageErase != 1) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_write_at.return:1
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].hdr_ss:31
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[10].comt:0xC0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].fileNum:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].se_size:5
TEST.VALUE:sffs.sffs_remove_byfd.fd:10
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].comt:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].unused:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].deled:0xFF
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[10].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[31]:2
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_remove_byfd.return:0
TEST.END

-- Test Case: remove_byfd5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd5
TEST.BASIS_PATH:3 of 7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> FALSE
      (3) if (ret != size) ==> TRUE
      (4) if (fs->pageErase == 1) ==> TRUE
      (5) if (sffs_erase_at(fs, ((fs->file)[fd]).hdr_ss, 1) < 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set size due to assignment
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_erase_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:192
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].fileNum:11
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].se_size:10
TEST.VALUE:sffs.sffs_remove_byfd.fd:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].comt:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].unused:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[0]:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].fileNum:10
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].errno:-21
TEST.EXPECTED:sffs.sffs_remove_byfd.return:-1
TEST.END

-- Test Case: remove_byfd6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> FALSE
      (3) if (ret != size) ==> TRUE
      (4) if (fs->pageErase == 1) ==> TRUE
      (5) if (sffs_erase_at(fs, ((fs->file)[fd]).hdr_ss, 1) < 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set size due to assignment
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_erase_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:192
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].fileNum:11
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].se_size:10
TEST.VALUE:sffs.sffs_remove_byfd.fd:0
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:2
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].comt:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].unused:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[0]:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].fileNum:10
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].errno:-21
TEST.EXPECTED:sffs.sffs_remove_byfd.return:-1
TEST.END

-- Test Case: remove_byfd7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byfd
TEST.NEW
TEST.NAME:remove_byfd7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if (((fs->file)[fd]).comt != 0xc0) ==> FALSE
      (3) if (ret != size) ==> TRUE
      (4) if (fs->pageErase == 1) ==> TRUE
      (5) if (sffs_erase_at(fs, ((fs->file)[fd]).hdr_ss, 1) < 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set size due to assignment
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.VALUE:sffs.sffs_write_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_erase_at.return:<<MAX>>
TEST.VALUE:sffs.sffs_remove_byfd.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].se_no[0]:-1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:0
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].file[0].comt:192
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].fileNum:11
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_remove_byfd.fs[0].se_size:10
TEST.VALUE:sffs.sffs_remove_byfd.fd:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].magic_num:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].name_len:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].size:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].ofs:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].comt:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].unused:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].deled:255
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].file[0].stat:0
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].free_ss_hdr[0]:-1
TEST.EXPECTED:sffs.sffs_remove_byfd.fs[0].fileNum:10
TEST.EXPECTED:sffs.sffs_remove_byfd.return:0
TEST.END

-- Subprogram: sffs_remove_byname

-- Test Case: remove_byname1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byname
TEST.NEW
TEST.NAME:remove_byname1
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (pos < 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set pos due to assignment
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:<<MIN>>
TEST.VALUE:sffs.sffs_remove_byname.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byname.fname:<<malloc 1>>
TEST.EXPECTED:sffs.sffs_remove_byname.fs[0].errno:-8
TEST.EXPECTED:sffs.sffs_remove_byname.return:-1
TEST.END

-- Test Case: remove_byname2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_remove_byname
TEST.NEW
TEST.NAME:remove_byname2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (pos < 0) ==> FALSE
   Test Case Generation Notes:
      Cannot set pos due to assignment
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.STUB:sffs.sffs_remove_byfd
TEST.VALUE:sffs.sffs_find_filehdr.return:<<MAX>>
TEST.VALUE:sffs.sffs_remove_byfd.return:-1
TEST.VALUE:sffs.sffs_remove_byname.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_remove_byname.fname:<<malloc 1>>
TEST.EXPECTED:sffs.sffs_remove_byname.return:-1
TEST.END

-- Subprogram: sffs_rename

-- Test Case: sffs_rename1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename1
TEST.BASIS_PATH:1 of 16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 9>>
TEST.VALUE:sffs.sffs_rename.old_name:<<null>>
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 9>>
TEST.VALUE:sffs.sffs_rename.new_name:<<null>>
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename10
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename10
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_hdrss
TEST.VALUE:sffs.sffs_get_hdrss.return:-1
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:187
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename12
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].free_ss_hdr[1]:3
TEST.VALUE:sffs.sffs_rename.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_rename.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename12.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename12.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_write_at.return:0
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].free_ss_hdr[1]:3
TEST.VALUE:sffs.sffs_rename.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_rename.fs[0].pageErase:5
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.<<GLOBAL>>.hdrsStat[0]:3
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename13
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename13
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_rename.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_at.return
static i=151;
i=i-75;
<<sffs.sffs_write_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: sffs_rename14
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename14
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].seqcnt:0
TEST.VALUE:sffs.sffs_rename.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.fs[0].free_ss_hdr[1]:2
TEST.EXPECTED:sffs.sffs_rename.fs[0].seqcnt:1
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_at.return
static i=151;
i=i-75;
<<sffs.sffs_write_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: sffs_rename15
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename15
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_write_at.return:76
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.fs[0].free_ss_hdr[1]:3
TEST.EXPECTED:sffs.sffs_rename.fs[0].errno:-28
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 9>>
TEST.VALUE:sffs.sffs_rename.old_name:<<null>>
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 9>>
TEST.VALUE:sffs.sffs_rename.new_name:<<null>>
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename4
TEST.BASIS_PATH:2 of 16
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.return:0
TEST.END

-- Test Case: sffs_rename5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:-1
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_find_filehdr
TEST.VALUE:sffs.sffs_find_filehdr.return:0
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Test Case: sffs_rename7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 21>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbbbbbaaaaaaaaaaaaa"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:187
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbbbbbaaaaaaaaaaaaa"
TEST.EXPECTED:sffs.sffs_rename.return:0
TEST.END

-- Test Case: sffs_rename8
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:187
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:0
TEST.END

-- Test Case: sffs_rename9
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_rename
TEST.NEW
TEST.NAME:sffs_rename9
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (old_name == (void *)0 || new_name == (void *)0) ==> FALSE
      (2) if (strlen((char *)new_name) == strlen((char *)old_name) && strncmp((char *)old_name, (char *)new_name, strlen((char *)old_name)) == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set return value of function strlen in branch 2
      Cannot set return value of function strncmp in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_write_at
TEST.VALUE:sffs.sffs_read_at.return:76
TEST.VALUE:sffs.sffs_write_at.return:76
TEST.VALUE:sffs.sffs_rename.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name:"aaaa"
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].hdr_ss:1
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.VALUE:sffs.sffs_rename.fs[0].free_ss_hdr[1]:1
TEST.VALUE:sffs.sffs_rename.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_rename.fs[0].se_size:5
TEST.VALUE:sffs.sffs_rename.old_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.old_name:"aaaa"
TEST.VALUE:sffs.sffs_rename.new_name:<<malloc 5>>
TEST.VALUE:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].name_len:4
TEST.EXPECTED:sffs.sffs_rename.fs[0].file[0].deled:0xBB
TEST.EXPECTED:sffs.sffs_rename.old_name:"aaaa"
TEST.EXPECTED:sffs.sffs_rename.new_name:"bbbb"
TEST.EXPECTED:sffs.sffs_rename.return:-1
TEST.END

-- Subprogram: sffs_reset_filehdr

-- Test Case: sffs_reset_filehdr_MAX.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_reset_filehdr
TEST.NEW
TEST.NAME:sffs_reset_filehdr_MAX.001
TEST.VALUE:<<ALL_MAX>>
TEST.VALUE:sffs.sffs_reset_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_reset_filehdr.fd:2147483625
TEST.END

-- Test Case: sffs_reset_filehdr_MID.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_reset_filehdr
TEST.NEW
TEST.NAME:sffs_reset_filehdr_MID.001
TEST.VALUE:<<ALL_MID>>
TEST.VALUE:sffs.sffs_reset_filehdr.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_reset_filehdr.fs[0].file[0].se_no[10]:-1
TEST.VALUE:sffs.sffs_reset_filehdr.fd:0
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].magic_num:0
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].name_len:0
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].size:-1
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].ofs:-1
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].fid:0
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].seq:0
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].se_no[9]:-1
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].comt:0xFF
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].unused:0xFF
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].deled:0xFF
TEST.EXPECTED:sffs.sffs_reset_filehdr.fs[0].file[0].stat:0
TEST.END

-- Test Case: sffs_reset_filehdr_MIN.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_reset_filehdr
TEST.NEW
TEST.NAME:sffs_reset_filehdr_MIN.001
TEST.VALUE:<<ALL_MIN>>
TEST.VALUE:sffs.sffs_reset_filehdr.fd:-2147483625
TEST.END

-- Subprogram: sffs_set_errno

-- Test Case: errno_max
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_errno
TEST.NEW
TEST.NAME:errno_max
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_errno.errno:2147483647
TEST.END

-- Test Case: errno_min
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_errno
TEST.NEW
TEST.NAME:errno_min
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_errno.errno:-2147483648
TEST.END

-- Test Case: errno_zero
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_errno
TEST.NEW
TEST.NAME:errno_zero
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_errno.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_errno.errno:0
TEST.END

-- Subprogram: sffs_set_free_hdrss

-- Test Case: set_free_hdrss1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_free_hdrss
TEST.NEW
TEST.NAME:set_free_hdrss1
TEST.BASIS_PATH:1 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->fileNum >= 20) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_free_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].fileNum:2147483647
TEST.EXPECTED:sffs.sffs_set_free_hdrss.return:-1
TEST.END

-- Test Case: set_free_hdrss2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_free_hdrss
TEST.NEW
TEST.NAME:set_free_hdrss2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (j < fs->se_size / fs->ss_size) ==> FALSE
   Test Case Generation Notes:
      Cannot set j due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_free_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr[0]:127
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].fileNum:-2147483648
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_set_free_hdrss.return:-1
TEST.END

-- Test Case: set_free_hdrss3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_free_hdrss
TEST.NEW
TEST.NAME:set_free_hdrss3
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (j < fs->se_size / fs->ss_size) ==> FALSE
   Test Case Generation Notes:
      Cannot set j due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_free_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr[0]:-128
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr[1]:-1
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].fileNum:-2147483648
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_set_free_hdrss.return:1
TEST.END

-- Test Case: set_free_hdrss4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_free_hdrss
TEST.NEW
TEST.NAME:set_free_hdrss4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (j < fs->se_size / fs->ss_size) ==> FALSE
   Test Case Generation Notes:
      Cannot set j due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_free_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr[0]:-1
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].fileNum:-2147483648
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_set_free_hdrss.return:0
TEST.END

-- Test Case: set_free_hdrss5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_free_hdrss
TEST.NEW
TEST.NAME:set_free_hdrss5
TEST.BASIS_PATH:2 of 6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->fileNum >= 20) ==> FALSE
      (2) for (j < fs->se_size / fs->ss_size) ==> FALSE
   Test Case Generation Notes:
      Cannot set j due to assignment
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_free_hdrss.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr[0]:-1
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].free_ss_hdr_cur:0
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].fileNum:-2147483648
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_set_free_hdrss.fs[0].se_size:12
TEST.EXPECTED:sffs.sffs_set_free_hdrss.return:0
TEST.END

-- Subprogram: sffs_set_sebit

-- Test Case: set_sebit
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_set_sebit
TEST.NEW
TEST.NAME:set_sebit
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_set_sebit.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_set_sebit.fs[0].free_se[0]:12
TEST.VALUE:sffs.sffs_set_sebit.se_no:2
TEST.END

-- Subprogram: sffs_start

-- Test Case: start1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_start
TEST.NEW
TEST.NAME:start1
TEST.BASIS_PATH:1 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((((fs->se_size / fs->ss_size != 32 || fs->ss_size != 4 * 1024) || fs->device_size / fs->se_size != 240) || 32 < 20) || 16 * fs->se_size < (2 * 1024) * 1024) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (32 < 20) in branch 1
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_start.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_start.fs[0].device_size:100000
TEST.VALUE:sffs.sffs_start.fs[0].ss_size:<<MIN>>
TEST.VALUE:sffs.sffs_start.fs[0].se_size:1000
TEST.EXPECTED:sffs.sffs_start.fs[0].errno:-22
TEST.EXPECTED:sffs.sffs_start.return:-1
TEST.END

-- Test Case: start2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_start
TEST.NEW
TEST.NAME:start2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((((fs->se_size / fs->ss_size != 32 || fs->ss_size != 4 * 1024) || fs->device_size / fs->se_size != 240) || 32 < 20) || 16 * fs->se_size < (2 * 1024) * 1024) ==> TRUE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (32 < 20) in branch 1
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_start.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_start.fs[0].device_size:100000
TEST.VALUE:sffs.sffs_start.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_start.fs[0].se_size:1000
TEST.EXPECTED:sffs.sffs_start.fs[0].errno:-22
TEST.EXPECTED:sffs.sffs_start.return:-1
TEST.END

-- Test Case: start3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_start
TEST.NEW
TEST.NAME:start3
TEST.BASIS_PATH:2 of 4
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((((fs->se_size / fs->ss_size != 32 || fs->ss_size != 4 * 1024) || fs->device_size / fs->se_size != 240) || 32 < 20) || 16 * fs->se_size < (2 * 1024) * 1024) ==> FALSE
      (2) for (i < 20) ==> FALSE
   Test Case Generation Notes:
      Conflict: Unable to validate expression-to-expression comparison in branch 1
      Conflict: A condition in this branch uses an operand that cannot be set (32 < 20) in branch 1
      Conflict: Unable to validate expression-to-expression comparison in branch 2
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_start.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_start.fs[0].device_size:31457280
TEST.VALUE:sffs.sffs_start.fs[0].ss_size:4096
TEST.VALUE:sffs.sffs_start.fs[0].se_size:131072
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].magic_num:0
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].name_len:0
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].size:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].ofs:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].se_no[0]:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].se_no[15]:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].hdr_ss:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].comt:0xFF
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].unused:0xFF
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].deled:0xFF
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].unused2[0]:0xFF
TEST.EXPECTED:sffs.sffs_start.fs[0].file[0].unused2[1]:0xFF
TEST.EXPECTED:sffs.sffs_start.fs[0].free_se_cur:1
TEST.EXPECTED:sffs.sffs_start.fs[0].free_ss_hdr_cur:-1
TEST.EXPECTED:sffs.sffs_start.fs[0].fileNum:0
TEST.EXPECTED:sffs.sffs_start.fs[0].fidcnt:0
TEST.EXPECTED:sffs.sffs_start.fs[0].seqcnt:0
TEST.EXPECTED:sffs.sffs_start.fs[0].lock:0
TEST.EXPECTED:sffs.sffs_start.fs[0].magic_num:0xCA905001
TEST.EXPECTED:sffs.sffs_start.return:0
TEST.END

-- Subprogram: sffs_umount

-- Test Case: umount1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_umount
TEST.NEW
TEST.NAME:umount1
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.STUB:sffs.sffs_start
TEST.STUB:sffs.sffs_mount
TEST.VALUE:sffs.sffs_start.return:-1
TEST.VALUE:sffs.sffs_umount.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_mount.return:0
TEST.EXPECTED:sffs.sffs_umount.fs[0].mounted:0
TEST.EXPECTED:sffs.sffs_umount.return:0
TEST.END

-- Subprogram: sffs_unlock

-- Test Case: unlock1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_unlock
TEST.NEW
TEST.NAME:unlock1
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->lock > 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_unlock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_unlock.fs[0].lock:-128
TEST.EXPECTED:sffs.sffs_unlock.return:-1
TEST.END

-- Test Case: unlock2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_unlock
TEST.NEW
TEST.NAME:unlock2
TEST.BASIS_PATH:2 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if (fs->lock > 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_unlock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_unlock.fs[0].lock:0
TEST.EXPECTED:sffs.sffs_unlock.return:-1
TEST.END

-- Test Case: unlock3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_unlock
TEST.NEW
TEST.NAME:unlock3
TEST.BASIS_PATH:1 of 2
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if (fs->lock > 0) ==> FALSE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_unlock.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_unlock.fs[0].lock:127
TEST.EXPECTED:sffs.sffs_unlock.return:0
TEST.END

-- Subprogram: sffs_unprotect

-- Test Case: sffs_unprotect.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_unprotect
TEST.NEW
TEST.NAME:sffs_unprotect.001
TEST.END

-- Subprogram: sffs_write

-- Test Case: write1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write1
TEST.BASIS_PATH:1 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fd:<<MIN>>
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:<<MIN>>
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-1
TEST.EXPECTED:sffs.sffs_write.return:-1
TEST.END

-- Test Case: write2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write2
TEST.BASIS_PATH:2 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fd:<<MIN>>
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2097152
TEST.END

-- Test Case: write3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3
TEST.BASIS_PATH:3 of 21
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:0
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:10
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-19
TEST.EXPECTED:sffs.sffs_write.return:-1
TEST.END

-- Test Case: write3.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:0
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xC0
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-12
TEST.EXPECTED:sffs.sffs_write.return:-1
TEST.END

-- Test Case: write3.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2097152
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-16
TEST.EXPECTED:sffs.sffs_write.return:-1
TEST.END

-- Test Case: write3.004
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.004
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:1
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:0
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.005
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.005
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:0
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write.fs[0].se_size:5
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-17
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.006
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.006
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write.fs[0].se_size:5
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=-1;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.007
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.007
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write.fs[0].se_size:5
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=-1;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.008
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.008
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:-1
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:7
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_write.return:-1
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=-1;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.009
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.009
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:0
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:7
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-17
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.010
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.010
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:12
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:7
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:2
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.011
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.011
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:-1
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:7
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:10
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-17
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Test Case: write3.012
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.012
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:7
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:10
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=3;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.013
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.013
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:4
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=2;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.014
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.014
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:255
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:4
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-17
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.STUB_VAL_USER_CODE:sffs.sffs_write_4k.return
static int i=2;
i=i+1;
<<sffs.sffs_write_4k.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Test Case: write3.015
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write
TEST.NEW
TEST.NAME:write3.015
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size < 0) || size > (2 * 1024) * 1024) ==> FALSE
      (2) if (file->stat != 2) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
      Cannot set file due to assignment
      Cannot set local variable file in branch 2
TEST.END_NOTES:
TEST.STUB:sffs.sffs_read_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_ck_fd
TEST.STUB:sffs.sffs_write_4k
TEST.VALUE:sffs.sffs_read_at.return:2
TEST.VALUE:sffs.sffs_erase_at.return:0
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.return:-1
TEST.VALUE:sffs.sffs_write.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write.fs[0].file[0].ofs:10
TEST.VALUE:sffs.sffs_write.fs[0].file[0].comt:0xEE
TEST.VALUE:sffs.sffs_write.fs[0].file[0].stat:2
TEST.VALUE:sffs.sffs_write.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write.fs[0].ss_size:5
TEST.VALUE:sffs.sffs_write.fs[0].se_size:15
TEST.VALUE:sffs.sffs_write.fd:0
TEST.VALUE:sffs.sffs_write.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write.size:4
TEST.EXPECTED:sffs.sffs_write.fs[0].errno:-17
TEST.EXPECTED:sffs.sffs_write.return:0
TEST.END

-- Subprogram: sffs_write_4k

-- Test Case: write_4k1
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k1
TEST.BASIS_PATH:1 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fd:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:<<MAX>>
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k1.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k1.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write_4k.fd:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:0
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k1.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k1.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:1
TEST.VALUE:sffs.sffs_write_4k.fd:5
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:3
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k1.003
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k1.003
TEST.NOTES:
This is an automatically generated test case.
   Test Path 1
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:-5
TEST.VALUE:sffs.sffs_write_4k.fd:5
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:-1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-11
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k2
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k2
TEST.BASIS_PATH:2 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 2
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:192
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:0
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-12
TEST.END

-- Test Case: write_4k3
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k3
TEST.BASIS_PATH:3 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 3
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> TRUE
   Test Case Generation Notes:
TEST.END_NOTES:
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:0
TEST.EXPECTED:sffs.sffs_write_4k.return:0
TEST.END

-- Test Case: write_4k4
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k4
TEST.BASIS_PATH:4 of 12
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_get_se.return:-1
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:12
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-3
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k4.001
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k4.001
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_get_se.return:-1
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:32
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:3
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:6
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-14
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k4.002
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k4.002
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_write_at.return:8
TEST.VALUE:sffs.sffs_get_se.return:-1
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:32
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].pageErase:2
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:2
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:6
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.return:8
TEST.END

-- Test Case: write_4k5
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k5
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_write_at.return:1
TEST.VALUE:sffs.sffs_erase_at.return:1
TEST.VALUE:sffs.sffs_get_se.return:3
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].pageErase:1
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:12
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].size:13
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].ofs:2
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].se_no[1]:3
TEST.EXPECTED:sffs.sffs_write_4k.return:1
TEST.END

-- Test Case: write_4k6
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k6
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_write_at.return:5
TEST.VALUE:sffs.sffs_erase_at.return:1
TEST.VALUE:sffs.sffs_get_se.return:3
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].pageErase:10
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:12
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].se_no[1]:3
TEST.EXPECTED:sffs.sffs_write_4k.return:5
TEST.END

-- Test Case: write_4k7
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k7
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_write_at.return:5
TEST.VALUE:sffs.sffs_erase_at.return:<<MIN>>
TEST.VALUE:sffs.sffs_get_se.return:3
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].pageErase:10
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:12
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].se_no[1]:3
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].errno:-13
TEST.EXPECTED:sffs.sffs_write_4k.return:-1
TEST.END

-- Test Case: write_4k8
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_4k
TEST.NEW
TEST.NAME:write_4k8
TEST.NOTES:
This is an automatically generated test case.
   Test Path 4
      (1) if ((sffs_ck_fd(fs, fd) < 0 || size > fs->ss_size) || size < 0) ==> FALSE
      (2) if (((fs->file)[fd]).comt == 0xc0) ==> FALSE
      (3) if (size == 0) ==> FALSE
      (4) if (file->size % fs->se_size == 0) ==> FALSE
      (9) if (file->size % fs->ss_size != 0) ==> TRUE
   Test Case Generation Notes:
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->se_size) in branch 4
      Conflict: A condition in this branch uses an operand that cannot be set (file->size % fs->ss_size) in branch 9
      Cannot set local variable file in branch 10
TEST.END_NOTES:
TEST.STUB:sffs.sffs_write_at
TEST.STUB:sffs.sffs_erase_at
TEST.STUB:sffs.sffs_get_se
TEST.STUB:sffs.sffs_ck_fd
TEST.VALUE:sffs.sffs_write_at.return:5
TEST.VALUE:sffs.sffs_get_se.return:3
TEST.VALUE:sffs.sffs_ck_fd.return:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.VALUE:sffs.sffs_write_4k.fs[0].file[0].comt:<<MIN>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].pageErase:10
TEST.VALUE:sffs.sffs_write_4k.fs[0].ss_size:<<MAX>>
TEST.VALUE:sffs.sffs_write_4k.fs[0].se_size:12
TEST.VALUE:sffs.sffs_write_4k.fd:0
TEST.VALUE:sffs.sffs_write_4k.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_4k.size:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].size:12
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].ofs:1
TEST.EXPECTED:sffs.sffs_write_4k.fs[0].file[0].se_no[1]:3
TEST.EXPECTED:sffs.sffs_write_4k.return:5
TEST.STUB_VAL_USER_CODE:sffs.sffs_erase_at.return
static int i=-2;
i=i+1;
<<sffs.sffs_erase_at.return>> = ( i );
TEST.END_STUB_VAL_USER_CODE:
TEST.END

-- Subprogram: sffs_write_at

-- Test Case: write_test
TEST.UNIT:sffs
TEST.SUBPROGRAM:sffs_write_at
TEST.NEW
TEST.NAME:write_test
TEST.BASIS_PATH:1 of 1
TEST.NOTES:
   No branches in subprogram
TEST.END_NOTES:
TEST.VALUE:sffs.sffs_write_at.fs:<<malloc 1>>
TEST.VALUE:sffs.sffs_write_at.fs[0].write:dfs_write
TEST.VALUE:sffs.sffs_write_at.offset:0
TEST.VALUE:sffs.sffs_write_at.data:VECTORCAST_INT1
TEST.VALUE:sffs.sffs_write_at.size:1
TEST.EXPECTED:sffs.sffs_write_at.return:1
TEST.END
