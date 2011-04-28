#include "mycp.h"

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
		printf("Right mode: ./mycp [archive] [destination] [type_of_copy -optional]\n");
		exit(-1);
	}
	else if(argc == 3){
		mycp1(argv);
	}
	else {
		if(!strcmp(argv[3],"mycp1")){
			mycp1(argv);
		}
		else if(!strcmp(argv[3],"mycp2")){
			mycp2(argv);
		}
		else if(!strcmp(argv[3],"mycp3")){
			mycp3(argv);
		}
		    else if(!strcmp(argv[3],"mycp4")){
		    mycp4(argv);
		    }
		else{
			printf("ERROR: parameter not recognized;\n");
			exit(-1);
		}
	}

	return 0;
}

