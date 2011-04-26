#include "mycp.h"



int mycp1(char** files){
	
	int indescr, outdescr, reader, writer;
	DIR *dir;
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
	dir = opendir(files[2]);
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
	close(indescr); close(outdescr);closedir(dir);
		
	if (reader == 0){
		printf("File successfuly copied!\n"); 
		exit(0);
	} 
	else{
		printf("Error while writing file. Please try again.\n");
		exit(-1);
	}
	

}


/**
 * Função main
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 * 
 * @returns 1 se houver algum erro, 0 caso contrário
 */
int main(int argc, char** argv){

	

	if(argc < 3){
		printf("Error while reading the arguments. They may be invalid or inexistent.\n");
		exit(-1);
	}
	else if(argc == 3){
		mycp1(argv);
	}
	else {
		if(argv[3] == "mycp1"){
			mycp1(argv);
		}
		/*else if(argv[3] == "mycp2"){
			mycp2(argv);
		}
		else if(argv[3] == "mycp3"){
			mycp3(argv);
		}
		else if(argv[3] == "mycp4"){
			mycp4(argv);
		}*/
		else{
			printf("ERROR: parameter not recognized;\n");
			exit(-1);
		}
	}



return 0;
}

