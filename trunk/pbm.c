#include <pbm.h>

/**
 * Função que transforma uma matriz de bits em ints (uso interno)
 * 
 * @param bit Matriz de bits
 * @param nlin Ponteiro para o número de linhas
 * @param ncol Ponteiro para o número de colunas
 * @param tab Ponteiro para a matriz de ints
 */
void bit2int(bit **bit, int *nlin, int *ncol, int ***tab)
{
	int i, j, **out;
	
	out = (int**) malloc(*nlin * sizeof(int*));
	for(i=0; i<*nlin; i++)
	{
		out[i] = (int*)malloc(*ncol * sizeof(int));
		for(j=0; j<*ncol; j++)
		{
			if(bit[i][j]==PBM_BLACK)
				out[i][j] = 1;
			else
				out[i][j] = 0;
		}
	}
	
	(*tab) = out;
}

/**
 * Função que lê uma imagem pgm e retorna uma matriz de ints
 * 
 * @param arq Caminho para o arquivo pbm
 * @param tabuleiro Ponteiro para a matriz de ints
 * @param nlin Ponteiro para o número de linhas
 * @param ncol Ponteiro para o número de colunas
 */
void pbm(char *arq, int ***tabuleiro, int *nlin, int *ncol)
{
	FILE *img;
	bit **tab;
	
	img = fopen(arq, "r");
	if(img)
	{
		//void bit** pbm_readpbm(FILE * fp, int *colsP, int *rowsP);
		tab = pbm_readpbm(img, ncol, nlin);
		bit2int(tab, nlin, ncol, tabuleiro);
		
		pbm_freearray(tab, *nlin);
		return;
	}
	else
	{
		// Erro na leitura
		printf("Arquivo não encontrado\n");
		exit(0);
	}
}

