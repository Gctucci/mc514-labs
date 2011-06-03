.PHONY : test doc mem


doc:
	-doxygen Doxyfile
	-cd doc/latex; make
	-cp doc/latex/refman.pdf Relatório.pdf

mem:
	-valgrind --leak-check=full ./dados
