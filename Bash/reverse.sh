#!/bin/bash

function reverse {
	for (( i=${#array[@]}-1 ; i>=0 ; i-- )) ; do
		    echo "${array[i]}"
	    done
}

function main {
	cd $1
	array=( $(ls) )
	reverse
}

main $1
