#!/bin/bash

crc32 $1 > /tmp/temp

ls -l $1 | awk '{ print $5 >> "/tmp/temp"}' 

#cat $1 >> /tmp/temp

#rm $1

split -b 1511296 $1
crc32 xaa >/tmp/temp2 
ls -l xaa | awk '{ print $5 >> "/tmp/temp2"}' 


mv /tmp/temp $1CRCx
mv /tmp/temp2 $1CRC

cat $1CRCx >> $1CRC
rm $1CRCx

