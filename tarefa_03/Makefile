.PHONY : test doc mem

CFLAGS=-Wall -pedantic -std=c99 -lpthread -lrt -g


make:
	-gcc $(CFLAGS) *.c -o mycp

doc:
	-doxygen Doxyfile
	-cd doc/latex; make
	-cp doc/latex/refman.pdf Relatório.pdf

mem:
	-valgrind --leak-check=full ./mycp
