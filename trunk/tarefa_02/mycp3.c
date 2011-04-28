#include "mycp.h"
#define MAX(a,b) ((a < b) ?  (b) : (a))

/**
 * Função de cp que usa a função select() para monitorar os file descriptors
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp3(char** files){

	int indescr, outdescr, reader, writer,retvalue;
	fd_set fdwrite,fdread;
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

		/* Seta o descritor de arquivo de leitura a ser observado por select*/
		FD_ZERO(&fdread);
		FD_SET(indescr, &fdread);

		/* Seta o descritor de arquivo de leitura a ser observado por select*/
		FD_ZERO(&fdwrite);
		FD_SET(outdescr, &fdwrite);

		/*Chama select, verificando a disponibilidade dos descritores de arquivos para alguma operação de I/O */
		retvalue = select(MAX(indescr,outdescr)+1,&fdread,&fdwrite,NULL,NULL);

		if(retvalue == -1){
			printf("Error while executing select()");
			exit(-1);
		}

		reader = read(indescr, buffer, MAXBUFF); /* le um bloco de dados */

		if (reader <= 0) break; /* Se ocorrer um erro durante a cópia, ou o arquivo acabar, o loop pára.*/

		writer = write(outdescr, buffer, reader); /*escreve um bloco de dados*/

		/* Erro se um bloco de dados não for escrito*/
		if (writer <= 0){
			printf("Error while writing file: File could not be written.\n");
			exit(-1);
		}
	}
	/*Tira os descritores de arquivos do conjunto (set) de monitoramento*/
	FD_CLR(indescr, &fdread);
	FD_CLR(outdescr, &fdwrite);
	/*Fechando os arquivos/diretórios*/
	close(indescr); 
	close(outdescr);

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

