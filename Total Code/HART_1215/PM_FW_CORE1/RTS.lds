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
    CODE_DATA (rx)          : ORIGIN =  0x0000000001E00000, LENGTH = 28M
    USR_DATA_MEM(xrw)       : ORIGIN =  0x0000000003C00000, LENGTH = 2M
    SHARED_MEM(xrw)         : ORIGIN =  0x0000000003E00000, LENGTH = 6M
}

SECTIONS
{
  .text      :
  {
    cag_kernel_text_start = .; _cag_kernel_text_start = .;
    _CAG_START_TEXT = .;
    ../PM_FW_COMMON/Srv/Scheduler/lib/CORE1/cagOsCORE1.a(.text.entry) 
    *(.text)
    . = ALIGN(4096);
  }>CODE_DATA
  
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
  }>CODE_DATA
  
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
  }>CODE_DATA
  
  .sdata     :
  {
    _SDA_BASE_ = . + 0x7ff0 ;
    *(.sdata) *(.sdata.*)
    . = ALIGN(4096);
  }>CODE_DATA
  
  edata = .; _edata = .;
  cag_kernel_data_end = .; _cag_kernel_data_end = .;
  
  . = ALIGN(4096);
  .sbss      :
  {
    cag_kernel_bss_start = .; _cag_kernel_bss_start = .;
    *(.sbss) *(.sbss.*) *(.scommon) 
    . = ALIGN(4096);
  }>CODE_DATA

   .bss       :
  {
    *(.dynbss) *(.bss) *(.bss.*) *(COMMON) *(.gnu.linkonce.b*)
    . = ALIGN(4096);
  }>CODE_DATA

  end = .; _end = .;
  cag_kernel_bss_end = .; _cag_kernel_bss_end = .;

  .tdata       : 
  {
     *(.tdata .tdata.* .gnu.linkonce.td.*) 
  }>CODE_DATA
  
  .tbss        : 
  {
    *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) 
  }>CODE_DATA
}








