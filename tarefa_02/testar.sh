#!/bin/sh


if [ $# != 4 ]
then
	echo "uso: ./testar [arquivo a ser copiado] [destino] [número da implementação] [quantidade de execuções]"
else
	arq=$1
	dest=$2
	cp=$3
	max=$4
	for i in `seq 1 $max`
	do
		# Limpa o cache
		sudo ./drop-caches
		# Executa, salvando os tempos para um arquivo
		if [ $cp != 0 ]
		then
			(time ./mycp $arq $dest mycp$cp) 2> tmp
		else
			(time cp $arq $dest mycp$cp) 2> tmp
		fi
		
		# Pega as variáveis
		echo `cat tmp | grep -e "real" | cut -f 2 | cut -d "m" -f 1` `cat tmp | grep -e "real" | cut -f 2 | cut -d "m" -f 2 | cut -d "s" -f 1` >> real$cp
		echo `cat tmp | grep -e "user" | cut -f 2 | cut -d "m" -f 1` `cat tmp | grep -e "user" | cut -f 2 | cut -d "m" -f 2 | cut -d "s" -f 1` >> user$cp
		echo `cat tmp | grep -e "sys" | cut -f 2 | cut -d "m" -f 1` `cat tmp | grep -e "sys" | cut -f 2 | cut -d "m" -f 2 | cut -d "s" -f 1` >> sys$cp
	done
fi
