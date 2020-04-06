#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "sudo this."
	exit 1
fi

in_folder="testset"
out_folder="output"

mkdir -p $out_folder

dmesg -C
for i in $in_folder/TI*; do
	echo "Processing $i..."
	fullname=$(basename $i)
	testname=$(cut -f 1 -d '.' <<< $fullname)

	input="${testname}.txt"
	output="${testname}_stdout.txt"
	kerput="${testname}_dmesg.txt"

	echo $input $output $kerput

	./scheduler < $in_folder/$input | tee $out_folder/$output
	dmesg -c | grep Project1 | tee $out_folder/$kerput
done

chown -R edison:edison $out_folder
