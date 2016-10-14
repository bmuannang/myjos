#!/bin/bash

count=`ls -l $1  | awk '{print int($5)}'`
if [ $count -gt 510 ]
	then
		echo "boot block too large: $count bytes"
		exit 1
fi

let left=510-count
for i in `seq $left`
	do
		printf %b "\x0" >> $1
	done
printf %b "\x55" >> $1
printf %b "\xAA" >> $1