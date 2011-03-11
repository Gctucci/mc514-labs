make :
	-gcc -I/usr/include/netpbm -lpthread -lnetpbm -g *.c -o life
test :
	-gdb life
doc : Doxyfile $(wildcard $(SRC_DIR)/*.[ch])
	-doxygen Doxyfile
