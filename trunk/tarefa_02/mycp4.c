#include "mycp.h"
#include <sys/mman.h>

/**
 * Função que copia um arquivo utilizando as funções da mman.h
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp4(char** files){
	int fdin, fdout;
	void *src, *dst;
	struct stat statbuf;

	if ((fdin = open(files[1], O_RDONLY)) < 0){
		printf("Can't open %s for reading", files[1]);
	}	
	chdir(files[2]);
	fdout = open(files[1], O_RDWR | O_CREAT | O_TRUNC,FILE_MODE);
	if (fstat(fdin, &statbuf) < 0){ /* need size of input file */
		printf("Fstat error.\n");
		exit(-1);
	}
	/* set size of output file */
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1){
		printf("Lseek error.\n");
		exit(-1);
	}
	if (write(fdout, "", 1) != 1){
		printf("Error while writing file. Please try again.\n");
		exit(-1);
	}
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED,
	fdin, 0)) == MAP_FAILED){
		printf("Mmap error for input.\n");
		exit(-1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE,
	MAP_SHARED, fdout, 0)) == MAP_FAILED){
		printf("Mmap error for output\n");
		exit(-1);
	}
	memcpy(dst, src, statbuf.st_size); /* does the file copy */
	

	/*Fechando os arquivos/diretórios*/
	close(fdin); 
	close(fdout);
	
	printf("File successfully copied!\n"); 

	return 0;
}

