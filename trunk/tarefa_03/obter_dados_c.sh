#!/bin/sh
if [ $# = 0 ]
then
	echo "Uso: ./obter_dados.sh [caminho absoluto para o arquivo/partição]"
else
	hist_1=fragmentação.html
	hist_1_titulo="Histograma de fragmentação de blocos alocados em no. de segmentos"
	hist_2=tamanhos.html
	hist_2_titulo="Histograma de tamanhos de arquivos em blocos de alocação"
	
	# Gera o primeiro histograma
	echo "Gerando o histograma de fragmentação de blocos alocados em no. de segmentos"
	(echo $hist_1_titulo && cd ./dados_c && ./dados 1 $1 ) | (cd ./histograma && ./histograma.sh) > $hist_1
	echo "Arquivo $hist_1 gerado"
	
	# Gera o segundo histograma
	echo "Gerando o histograma de tamanhos de arquivos em blocos de alocação"
	(echo $hist_2_titulo && cd ./dados_c && ./dados 2 $1 ) | (cd ./histograma && ./histograma.sh) > $hist_2
	echo "Arquivo $hist_2 gerado"
	echo
	
	# Obtém os dados pedidos
	echo "Obtendo os dados do sistema de arquivos:"
	cd ./dados_c && ./dados 3 $1
	echo
fi
