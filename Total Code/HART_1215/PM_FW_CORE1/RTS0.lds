/*
 *------------------------------------------------------------
 * Project:cagOS
 * Goal: 
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains
 */

ENTRY(_start)

MEMORY
{
    RTS_ENTRY (rx)          : ORIGIN =  0x0000000001DFF010, LENGTH = 2032
    DIAG_ENTRY (rx)         : ORIGIN =  0x0000000001DFF800, LENGTH = 1024
    FILE_ENTRY (rx)         : ORIGIN =  0x0000000001DFFC00, LENGTH = 512
    COMM_ENTRY (rx)         : ORIGIN =  0x0000000001DFFE00, LENGTH = 512    
    
    RTS_CODE (rx)           : ORIGIN =  0x0000000001E00000, LENGTH = 512k
    RTS_DATA (rx)           : ORIGIN =  0x0000000001E80000, LENGTH = 26M
    USR_DATA_MEM(xrw)       : ORIGIN =  0x0000000003C00000, LENGTH = 2M
    SHARED_MEM(xrw)         : ORIGIN =  0x0000000003E00000, LENGTH = 6M
}

SECTIONS
{
  .rts      :
  {
    ./App/Main/src/RTSTaskExeEntryBin0.o(.text.TskRTSTaskExeEntry) 
    . = ALIGN(2048);
  }>RTS_ENTRY
  
  .diag      :
  {
    ./App/Main/src/DiagTaskExeEntryBin0.o(.text.TskDiagTaskExeEntry) 
    . = ALIGN(1024);
  }>DIAG_ENTRY
  
  .fm      :
  {
    ./App/Main/src/FileTaskExeEntryBin0.o(.text.TskFileTaskExeEntry) 
    . = ALIGN(512);
  }>FILE_ENTRY
  
  .comm      :
  {
    ./App/Main/src/CommTaskExeEntryBin0.o(.text.TskCommTaskExeEntry) 
    . = ALIGN(512);
  }>COMM_ENTRY
  
  .text      :
  {
    cag_kernel_text_start = .; _cag_kernel_text_start = .;
    _CAG_START_TEXT = .;
    ../PM_FW_COMMON/Srv/Scheduler/lib/CORE1/cagOsCORE1-0.a(.text.entry) 
    *(.text)
    . = ALIGN(4096);
  }>RTS_CODE
  
     .shared_memory : 
  {
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedRtData.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedRtData.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedRtData.o(COMMON)
        
        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedComm.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedComm.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedComm.o(COMMON)

        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedConfig.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedConfig.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedConfig.o(COMMON)
        
        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedMem.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedMem.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedMem.o(COMMON)
        
        . = ALIGN(8);        
        ../PM_FW_COMMON/Srv/SharedMem/src/LED.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/LED.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/LED.o(COMMON)
        
        . = ALIGN(8);        
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedWatchdog.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedWatchdog.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedWatchdog.o(COMMON)
        
        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSOE.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSOE.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSOE.o(COMMON)
        
        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedHandshake.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedHandshake.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedHandshake.o(COMMON)
        
        . = ALIGN(8);
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSafetyComm.o(.bss.*)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSafetyComm.o(.sbss)
        ../PM_FW_COMMON/Srv/SharedMem/src/SharedSafetyComm.o(COMMON)
        
        . = ALIGN(8);
  }>SHARED_MEM
  
  .sdata2      : {
    _SDA2_BASE_ = . + 0x7ff0 ;
    *(.sdata2) *(.sbss2)                             /* for PPC */
  }>RTS_DATA
  
  . = ALIGN(4096);
  cag_kernel_text_end = .; _cag_kernel_text_end = .;
  etext = .; _etext = .;
  . = ALIGN(4096);
  .data    :
  {
    cag_kernel_data_start = .; _cag_kernel_data_start = .;
    _CAG_START_DATA = .;
    *(.data)
    . = ALIGN(4096);
  }>RTS_DATA
  
  .sdata     :
  {
    _SDA_BASE_ = . + 0x7ff0 ;
    *(.sdata) *(.sdata.*)
    . = ALIGN(4096);
  }>RTS_DATA
  
  edata = .; _edata = .;
  cag_kernel_data_end = .; _cag_kernel_data_end = .;
  
  . = ALIGN(4096);
  .sbss      :
  {
    cag_kernel_bss_start = .; _cag_kernel_bss_start = .;
    *(.sbss) *(.sbss.*) *(.scommon) 
    . = ALIGN(4096);
  }>RTS_DATA

   .bss       :
  {
    *(.dynbss) *(.bss) *(.bss.*) *(COMMON) *(.gnu.linkonce.b*)
    . = ALIGN(4096);
  }>RTS_DATA

  end = .; _end = .;
  cag_kernel_bss_end = .; _cag_kernel_bss_end = .;

  .tdata       : 
  {
     *(.tdata .tdata.* .gnu.linkonce.td.*) 
  }>RTS_DATA
  
  .tbss        : 
  {
    *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) 
  }>RTS_DATA
}








