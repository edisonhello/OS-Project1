#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "sudo this."
	exit 1
fi

in_folder="../testset"

dmesg -C

for i in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4;
do
	echo Demoing $i ...
	../scheduler < $in_folder/$i.txt
	dmesg -c | grep Project1
	echo ""
done
