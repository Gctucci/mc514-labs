#include "mycp.h"
#include <aio.h>

/**
 * Função que copia um arquivo utilizando as funções da aio.h
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp4(char** files){

	int indescr, outdescr, reader, writer;
	char buffer[MAXBUFF];
	aiocb data_in;
	aiocb data_out;

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
	outdescr = creat(files[1], OUTPUT);

	if (outdescr < 0){ 
		printf("ERROR: File cannot be created.\n");
		exit(-1);
	}

	memset(&data_in, 0, sizeof(aiocb));
	data_in.aio_nbytes = MAXBUFF;
	data_in.aio_fildes = indescr;
	data_in.aio_offset = 0;
	data_in.aio_buf = buffer;

	memset(&data_out, 0, sizeof(aiocb));
	data_out.aio_nbytes = MAXBUFF;
	data_out.aio_fildes = outdescr;
	data_out.aio_offset = 0;
	data_out.aio_buf = buffer;

	/*Loop que copia o arquivo*/
	while (TRUE) {
		reader = aio_read(&data_in); /* le um bloco de dados */

		if (reader <= 0) break; /* Se ocorrer um erro durante a cópia, ou o arquivo acabar, o loop pára.*/

		writer = aio_write(&data_out); /*escreve um bloco de dados*/

		if (writer <= 0){
			printf("Error while writing file: File could not be written.\n");
			exit(-1);
		}
	}
	/*Fechando os arquivos/diretórios*/
	close(indescr); close(outdescr);

	if (reader == 0){
		printf("File successfully copied!\n"); 
		exit(0);
	} 
	else{
		printf("Error while writing file. Please try again.\n");
		exit(-1);
	}

	return 0;
}

