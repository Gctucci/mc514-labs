.PHONY : test doc mem

make :
	-gcc -I/usr/include/netpbm  -lncurses -lpthread -lnetpbm -g *.c -o life

test :
	-gdb life

doc:
	-doxygen Doxyfile

mem:
	-valgrind --leak-check=full ./life
	@echo ""
	@echo "Ignore os leaks da linha 278, são internos da ncurses"
