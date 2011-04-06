.PHONY : test doc mem

make :
	-gcc -I/usr/include/netpbm -Wall -pedantic -std=c99 -lncurses -lpthread -lnetpbm -g *.c -o life

test :
	-gdb life

doc:
	-doxygen Doxyfile
	-cd doc/latex; make
	-cp doc/latex/refman.pdf Relatório.pdf

mem:
	-valgrind --leak-check=full ./life pulsar.pbm
	@echo ""
	@echo "Favor ignorar os leaks internos da ncurses"
