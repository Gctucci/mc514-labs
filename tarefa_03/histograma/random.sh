#!/bin/sh
if [ $# = 0 ]
then
	echo "Uso: ./random.sh [número de valores aleatórios gerados] [limite]"
else
	if [ $# = 2 ]
	then
		lim=$2
	else
		lim=270
	fi
	for i in `seq 1 $1`
	do
		echo $(( $RANDOM % $lim ))
	done
fi
