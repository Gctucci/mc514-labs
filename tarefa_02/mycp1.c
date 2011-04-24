#include <stdio.h>
#include <stdlib.h>



/**
 * Função main
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 * 
 * @returns 1 se houver algum erro, 0 caso contrário
 */
int main (int argc, char *argv[])
{
	/* Lê os parâmetros, e interrompe a execução se não houver nenhum */
	if(argc!=3)
	{
		printf("Modo de uso:\n\t./mycat1 [arquivo de origem] [arquivo de destino]\n");
		exit(0);
	}
	
	// LOL
	char comando[200];
	sprintf(comando, "cp %s %s", argv[1], argv[2]);
	system(comando);
	
		
	return 0;
}
