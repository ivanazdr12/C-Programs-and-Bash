#! /bin/bash 

cd /bin
l=ls /bin
count=0

for x in {a..z}; do
    for y in $x*; do
	if  [ "$y" != "$x*" ]; then
	let "count++"
	 fi
     done
    if  [ $count != 0 ]; then
	echo $x : $count
	let "count=0"
    fi
done
