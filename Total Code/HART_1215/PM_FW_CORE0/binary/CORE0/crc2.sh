#!/bin/bash

crc32 $1 > /tmp/temp

ls -l $1 | awk '{ print $5 >> "/tmp/temp"}' 

cat $1 >> /tmp/temp

#rm $1

mv /tmp/temp $1CRC2


