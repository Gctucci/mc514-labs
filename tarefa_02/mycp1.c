#include "mycp.h"

/**
 * Função de cp mais básica, que apenas muda o diretório corrente,
 * cria um fd e copia o conteúdo para o mesmo
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp1(char** files){

	int indescr, outdescr, reader, writer;
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
	outdescr = creat(files[1], OUTPUT);

	if (outdescr < 0){ 
		printf("ERROR: File cannot be created.\n");
		exit(-1);
	}

	/*Loop que copia o arquivo*/
	while (TRUE) {
		reader = read(indescr, buffer, MAXBUFF); /* le um bloco de dados */

		if (reader <= 0) break; /* Se ocorrer um erro durante a cópia, ou o arquivo acabar, o loop pára.*/

		writer = write(outdescr, buffer, reader); /*escreve um bloco de dados*/

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

