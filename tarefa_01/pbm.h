/**
 * Função que lê uma imagem pgm e retorna uma matriz de ints
 * 
 * @param arq Caminho para o arquivo pbm
 * @param tabuleiro Ponteiro para a matriz de ints
 * @param nlin Ponteiro para o número de linhas
 * @param ncol Ponteiro para o número de colunas
 */
void pbm(char *arq, int ***tabuleiro, int *nlin, int *ncol);


/**
 * Função que recebe uma matriz de ints e imprime em um pgm
 * 
 * @param arq Caminho para o arquivo pbm a ser escrito
 * @param tabuleiro Ponteiro para a matriz de ints
 * @param nlin Número de linhas
 * @param ncol Número de colunas
 */
void pbm_write(char *arq, int **tabuleiro, int nlin, int ncol);
