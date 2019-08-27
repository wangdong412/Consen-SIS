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
    /* offset 80MB */
    DIAG_SWITCH (rx)            : ORIGIN =  0x000000000A0FF010, LENGTH = 2032
    DIAG_SWITCHUP1 (rx)         : ORIGIN =  0x000000000A0FF800, LENGTH = 1024
    DIAG_SWITCHUP2 (rx)         : ORIGIN =  0x000000000A0FFC00, LENGTH = 1024
    
    RTS_CODE (rx)               : ORIGIN =  0x000000000A100000, LENGTH = 1M
    RTS_TCOMMON (rx)            : ORIGIN =  0x000000000A200000, LENGTH = 16K
    RTS_DATA (rx)               : ORIGIN =  0x000000000A204000, LENGTH = 25M
    USR_DATA_MEM(xrw)           : ORIGIN =  0x000000000DC00000, LENGTH = 2M
    SHARED_MEM(xrw)             : ORIGIN =  0x000000000DE00000, LENGTH = 6M

}


SECTIONS
{
      .diag      :
  {
    ./App/Main/src/RTSTaskExeEntryBin2.o(.text.TskRTSTaskExeEntry) 
    . = ALIGN(2048);
  }>DIAG_SWITCH
  
      .diagUP1      :
  {
    ./App/Main/src/UP1TaskExeEntryBin2.o(.text.TskUP1TaskExeEntry) 
    . = ALIGN(1024);
  }>DIAG_SWITCHUP1
  
      .diagUP2      :
  {
    ./App/Main/src/UP2TaskExeEntryBin2.o(.text.TskUP2TaskExeEntry) 
    . = ALIGN(1024);
  }>DIAG_SWITCHUP2
  .text      :
  {
    cag_kernel_text_start = .; _cag_kernel_text_start = .;
    _CAG_START_TEXT = .;
    ../PM_FW_COMMON/Srv/Scheduler/lib/CORE0/cagOsCORE0-2.a(.text.entry) 
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
  
       .usr_data_memory : 
  {
        . = ALIGN(8);
        Srv/SysResource/src/SysCore0RtData.o(.bss.*)
        Srv/SysResource/src/SysCore0RtData.o(.sbss)
        Srv/SysResource/src/SysCore0RtData.o(COMMON)
        
        . = ALIGN(8);
  }>USR_DATA_MEM
  
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
   }>RTS_TCOMMON
  
  .tbss        : 
  { 
    *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) 
  }>RTS_TCOMMON
}
