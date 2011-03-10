#include <netpbm/pbm.h>

/*
 * Função que lê uma imagem pgm e retorna uma matriz de mapa
 */
void pbm(char *arq, bit **tabuleiro, int *tamlin, int *tamcol)
{
	FILE *img;
	
	img = fopen(arq, "r");
	if(img)
	{
		//void bit** pbm_readpbm(FILE * fp, int *colsP, int *rowsP);
		tabuleiro = pbm_readpbm(img, tamcol, tamlin);
		return;
	}
	else
	{
		// Erro na leitura
		printf("Arquivo não encontrado\n");
		exit(0);
	}
}
