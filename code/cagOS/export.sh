#!/bin/sh

#####unused file###



#ordered
fnames=(
"./include/libc/cagboot.h" 
"./include/ppc/sysAtomic.h" 
"./include/kernel/sysOs.h" 
"./include/kernel/sysApi.h" 
"./driver/device/dfs/dfs.h" 
"./driver/device/pcie/pmbus.h" 
"./driver/device/pcie/cmbus.h" 
"./driver/fs/fs_api.h")

export="cagos.h"

rm $export

echo "#ifndef __ExPORT_HEADER_H__ \n#define __ExPORT_HEADER_H__ \n" >> $export

echo "/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  OS configure code.
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
 * 01,Feb,2016,Ezio created
 *
 * DESCRIPTION
 * This file contains OS apis
 */\n" >> $export


echo "#define NULL 0 \n" >> $export
echo "#define OS_ERR_NONE 0 \n" >> $export


for fname in $fnames
do
    print $fname
    start_line=$(grep -n "to be exported" $fname | head -1 | cut  -d  ":"  -f  1)
    lines1=$(grep -n "to be exported" $fname | cut  -d  ":"  -f  1 |wc -l)
    end_line=$(grep -n "end exported" $fname | head -1 |cut  -d  ":"  -f  1)
    lines2=$(grep -n "end exported" $fname |cut  -d  ":"  -f  1|wc -l)
    print $lines1 $lines2
    
    cat  $fname | awk -v sl=$start_line -v el=$end_line 'NR==sl+1,NR==el-1 {print $0}' >> $export
    echo "\n" >> $export
done
echo "\n#endif\n" >> $export


sed -i "s/(OS_OPT)/(UINT16)/g" $export

