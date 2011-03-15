#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<pthread.h>
#include<time.h>

#include"pbm.h"

#define MORTA 0 
#define VIVA 1
#define SUPERLOT 3
#define SOLIDAO 2
#define FPS 3.0
#define CHAR 10
#define MAXTHREADS 300

/**
 * Estrutura de argumentos de uma thread
 */
typedef struct thread_data_structure {
	int id[3];	    	/** id único */
	int linha_atual;	/** linha do pixel da thread */
	int coluna_atual;	/** coluna do pixel da thread */
} data;

/**Variaveis globais (compartilhadas pelas threads)*/
int **matriz;	    	/** matriz com o tabuleiro da posição/geração atual */
int **matriz_prox;  	/** matriz auxiliar para criar um tabuleiro de próxima geração */
int nlin,ncol;	    	/** número de linhas e colunas do tabuleiro */
int iter=0;         	/** número de iterações do jogo da vida */
float fps = FPS;	/** número de frames por segundo */

/**
 * Verifica se uma célula da matriz atual estará viva no próximo turno
 * 
 * @param lin Linha da célula
 * @param col Coluna da célula
 */
int valida_celula(int lin, int col);


/**
 * Função que conta as células adjacentes a uma célula de posição tabuleiro[linha][coluna].
 * A função também trata os casos especiais (se a célula estudada estiver numa borda, por ex.).
 * 
 * @param lin Linha da célula
 * @param col Coluna da célula
 */
int conta_celula(int lin, int col);


/**
 * Função que a thread vai executar ao ser criada:
 * A função verifica se a celula na qual a thread se encontra trabalhando
 * deve sobreviver à próxima geração (ser escrita no próximo tabuleiro);
 * Nesse caso, ela escreve na posição correspondente do pŕoximo tabuleiro
 * o estado da célula (VIVA ou MORTA)
 * 
 * @param threadarg Ponteiro void para a struct de parâmetros para a thread
 */
void *exec_thread(void *threadarg);

/**
 *  Imprime o tabuleiro atual
 */
void imprime();


/**
 *  Imprime as somas do tabuleiro atual (debug)
 */
void imprime_soma();



/**
 * Processa uma certa área da matriz.
 * Divide a imagem recursivamente em quatro partes
 *
 * @param lin_0 Linha inicial
 * @param lin_1 Linha final
 * @param col_0 Coluna inicial
 * @param col_1 Coluna final
 * @param threads Matriz de threads
 * @param thread_data Matriz de dados das threads
 */
void processa_area(int lin_0, int lin_1, int col_0, int col_1, pthread_t** threads, data** thread_data);


/**
 * Processa a matriz sem usar threads (debug)
 */
void processa_sem_threads();



