#! /bin/bash

nums=()

for arg in $@; do
	position=0
	for value in ${nums[@]}; do
		if [ $arg -lt $value ]; then
			temp=${nums[$position]}
			nums[$position]=$arg
			arg=$temp
		fi
		let position++
	done
	nums+=($arg)
done

echo ${nums[@]}
