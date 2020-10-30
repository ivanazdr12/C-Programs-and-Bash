#!/bin/bash

function average {
	local dir=$1
	local filesCount=0
	local totalSize=0
	
	#for every file, increment the file count by 1 and increase the total size by each size
	for item in $(ls $dir); do
		local size=$(du -b $dir/$item | awk '{print $1}')
		let "totalSize+=size"
		let "filesCount++"
	done;
	#get the average
	echo $(expr $totalSize / $filesCount)
}

function filter {
	local dir=$1
	local average=$2
	echo "Average: $average"
	for item in $(ls $dir); do
		local size=$(du -b $dir/$item | awk '{print $1}')
		
		#if size of file is greater than average, print them
		if [[ $size -gt $avg ]]; then
			echo "File: $item, Size: $size"
		fi
	done
}

function main {
	filter $1 $(average $1)
}


if [ -z $1 ]; then
	main .
else
	main $1
fi
