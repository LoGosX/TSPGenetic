#!/bin/bash

for instance in `ls datasets | grep .txt`
do
	echo "Processing $instance"
	for i in $(seq 1 9)
	do
		for j in $(seq 1 9)
		do
			for k in $(seq 1 10)
			do
				k=`expr 50 \* $k`
				./run.bash datasets/$instance 1500 $k 0.$i 0.$j 0.005 -50 0
				./run.bash datasets/$instance 1500 $k 0.$i 0.$j 0.005 -100 0
			done
		done
	done
done
