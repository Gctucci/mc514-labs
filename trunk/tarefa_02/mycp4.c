#include "mycp.h"
#include <sys/mman.h>

/**
 * Função que copia um arquivo utilizando as funções da mman.h
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp4(char** files){

	
	void *src, *dst;
	struct stat statbuf;
	int indescr, outdescr;
	char buffer[MAXBUFF];

	if(files == NULL){
		printf("Error while opening files: check if there any arguments were passed.\n");
		exit(-1);
	}

	indescr = open(files[1], O_RDONLY);


	if (indescr < 0){ 
		printf("ERROR: File cannot be opened.\n");
		exit(-1);
	}

	chdir(files[2]);
	outdescr = open(files[1], O_RDWR | O_CREAT | O_TRUNC,FILE_MODE);

	if (outdescr < 0){ 
		printf("ERROR: File cannot be created.\n");
		exit(-1);
	}
	if (fstat(indescr, &statbuf) < 0){ /* need size of input file */
		printf("ERROR: Could not determine the size of file.\n");
		exit(-1);
	}
	while(TRUE){
		/* set size of output file */
		if (lseek(outdescr, statbuf.st_size - 1, SEEK_SET) == -1)
			break;
		if (write(outdescr,buffer, 1) != 1){
			printf("Error while writing file: File could not be written.\n");
			exit(-1);
		}
		src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED,indescr, 0);
		if (src == MAP_FAILED){
			printf("ERROR: mmap failed for input.\n");	
			exit(-1);
		}
		dst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, outdescr, 0);
		if (dst == MAP_FAILED){
			printf("ERROR: mmap failed for output.\n");	
			exit(-1);
		}
		memcpy(dst, src, statbuf.st_size); /* does the file copy */
	}
	/*Fechando os arquivos/diretórios*/
	close(indescr); 
	close(outdescr);
	
	printf("File successfully copied!\n"); 

	return 0;
}

