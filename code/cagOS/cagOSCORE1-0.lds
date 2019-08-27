/*
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

    CODE_SEG(rx)    :ORIGIN = 0x0000000001E00000 ,LENGTH = 68K 
    DATA_SEG(rwx)   :ORIGIN = 0x0000000001E11000 ,LENGTH = 2M
    SHARE(xrw)      :ORIGIN = 0x0000000003000000, LENGTH = 1M

}

SECTIONS
{
  .text      :
  {
    cag_kernel_text_start = .; _cag_kernel_text_start = .;
    _CAG_START_TEXT = .;
    *(.text.entry)
    *(.text)
    . = ALIGN(4096);
  }>CODE_SEG
  
     .shared_memory : 
  {
     . = ALIGN(16);
  }>SHARE
  
  .sdata2      : {
    _SDA2_BASE_ = . + 0x7ff0 ;
    *(.sdata2) *(.sbss2)                             /* for PPC */
  }>DATA_SEG
  
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
  }>DATA_SEG
  
  .sdata     :
  {
    _SDA_BASE_ = . + 0x7ff0 ;
    *(.sdata) *(.sdata.*)
    . = ALIGN(4096);
  }>DATA_SEG
  
  edata = .; _edata = .;
  cag_kernel_data_end = .; _cag_kernel_data_end = .;
  
  . = ALIGN(4096);
  .sbss      :
  {
    cag_kernel_bss_start = .; _cag_kernel_bss_start = .;
    *(.sbss) *(.sbss.*) *(.scommon) 
    . = ALIGN(4096);
  }>DATA_SEG

   .bss       :
  {
    *(.dynbss) *(.bss) *(.bss.*) *(COMMON) *(.gnu.linkonce.b*)
    . = ALIGN(4096);
  }>DATA_SEG

  end = .; _end = .;
  cag_kernel_bss_end = .; _cag_kernel_bss_end = .;

  .tdata       : 
  { *(.tdata .tdata.* .gnu.linkonce.td.*) }>DATA_SEG
  
  .tbss        : 
  { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }>DATA_SEG

}
