.PHONY: histograma dados doc

make: histograma dados

histograma:
	@echo "Compilando o executável do histograma:"
	cd histograma/contagem && make
	@echo ""

dados:
	@echo "Compilando o executável que obtém os dados do sistema de arquivos:"
	cd dados_c && make
	@echo ""

doc:
	-doxygen Doxyfile
	-cd doc/latex; make
	-cp doc/latex/refman.pdf Relatório.pdf
