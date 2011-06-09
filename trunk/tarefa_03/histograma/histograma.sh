#!/bin/sh

# Lê a primeira linha, contendo o título
read titulo

# Parte inicial do html, com o título substituído
if [ -e ./partes/top.html ]
then
	cat ./partes/top.html | sed "s/titulo/${titulo}/"
else
	cat ./histograma/partes/top.html | sed "s/titulo/${titulo}/"
fi

# Lê os dados e conta cada categoria
if [ -e ./contagem/contagem ]
then
	./contagem/contagem
else
	./histograma/contagem/contagem
fi

# Parte final do html
if [ -e ./partes/bottom.html ]
then
	cat ./partes/bottom.html
else
	cat ./histograma/partes/bottom.html
fi
