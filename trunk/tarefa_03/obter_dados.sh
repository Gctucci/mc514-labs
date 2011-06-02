#!/bin/sh
if [ $# = 0 ]
then
	echo "Uso: ./obter_dados.sh [caminho absoluto para o arquivo/partição]"
else
	hist_1=fragmentação.html
	hist_2=tamanhos.html
	
	# Gera o primeiro histograma
	echo "Gerando o histograma de fragmentação de blocos alocados em no. de segmentos"
	(cd ./ruby_binding && ruby dados.rb 1 $1 ) | (cd ./histograma && ./histograma.sh) > $hist_1
	
	# Gera o segundo histograma
	echo "Gerando o histograma de tamanhos de arquivos em blocos de alocação"
	(cd ./ruby_binding && ruby dados.rb 2 $1 ) | (cd ./histograma && ./histograma.sh) > $hist_2
	echo
fi
