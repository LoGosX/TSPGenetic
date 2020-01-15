#!/bin/bash

instance="tsp500.txt"

for i in $(seq 1 5)
do
	for j in $(seq 1 5)
	do
		for k in $(seq 1 5)
		do
			for z in $(seq 1 6)
			do
				echo "i:$i/5 j:$j/5 k:$k/5 z:$z/6"
				m=`expr 100 \* $k`
				n=`expr 25 \* $z`
				./run.bash datasets/$instance 5000 $n 0.$i 0.$j 0.005 -$n 0 | tail -n 2 
			done
		done
	done
done
