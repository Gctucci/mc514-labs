.PHONY : test doc mem

CFLAGS=-Wall -pedantic -std=c99 -lpthread -g


make:
	-gcc $(CFLAGS) *.c -o mycp1

test: drop_caches
	-gdb mycp1

doc:
	-doxygen Doxyfile
	-cd doc/latex; make
	-cp doc/latex/refman.pdf Relatório.pdf

mem:
	-valgrind --leak-check=full ./mycp1

drop_caches:
	-sudo echo 1 >/proc/sys/vm/drop_caches
