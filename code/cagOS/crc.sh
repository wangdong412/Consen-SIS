#!/bin/bash

ls -l $1 | awk '{ print $5 > "/tmp/temp"}' 
crc32 $1 >> $1
cat /tmp/temp >> $1
rm /tmp/temp

