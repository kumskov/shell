#!/bin/bash

for (( i=0; $i<$1; i=$i+1 ))
do
	echo -n "cat /etc/passwd | "
	for (( j=0 ; $j<$2 ; j=$j+1 ))
	do
		echo -n "cat | "
	done
	echo "wc &"
	echo "ps -a"
done


