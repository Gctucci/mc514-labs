.PHONY : test doc

make :
	-gcc -I/usr/include/netpbm  -lncurses -lpthread -lnetpbm -g *.c -o life

test :
	-gdb life

doc:
	-doxygen Doxyfile
