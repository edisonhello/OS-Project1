#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "sudo this."
	exit 1
fi

rm -rf result
mkdir -p result

dmesg -C
for i in ../OS_PJ1_Test/*; do
	echo "Processing $i..."
	name=$(basename $i)
	./scheduler < $i | tee result/$name.out 
	dmesg -c | grep Project1 > result/$name.ker
done

chmod -R 777 result
