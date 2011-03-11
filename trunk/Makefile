.PHONY : test doc

make :
	-gcc -I/usr/include/netpbm -lpthread -lnetpbm -g *.c -o life

test :
	-gdb life

doc:
	-doxygen Doxyfile
