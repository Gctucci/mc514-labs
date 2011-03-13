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


/**
 * Verifica se uma célula da matriz atual estará viva no próximo turno
 * 
 * @param lin Linha da célula
 * @param col Coluna da célula
 */
int valida_celula(int lin, int col){
	int num_viva; /** Indica a soma do numero de celulas vivas */

	num_viva = conta_celula(lin, col);
	if(matriz[lin][col] == VIVA)
	{
		/**
		 * Caso o numero de celulas vizinhas seja maior que tres ou menor
		 * que dois, a celula morre por superlotação/solidão. Senão, ela sobrevive
		 * para a proxima geração
		 */
		if(num_viva > SUPERLOT || num_viva < SOLIDAO)
			return MORTA;
		else
			return VIVA;
	}
	if(matriz[lin][col] == MORTA)
	{
		if(num_viva == 3)
			return VIVA; // renasce
		else
			return MORTA;
	}
}

/**
 * Função que conta as células adjacentes a uma célula de posição tabuleiro[linha][coluna].
 * A função também trata os casos especiais (se a célula estudada estiver numa borda, por ex.).
 * 
 * @param lin Linha da célula
 * @param col Coluna da célula
 */
int conta_celula(int lin, int col){
	int soma=0,i,j;
	
	for(i=lin-1; i<lin+2; i++){
		if( (i >= 0) && (i < nlin) ) {
		
			for(j=col-1;j<col+2;j++){
				if( (j >= 0) && (j < nlin) ){
				
					if( !((i==lin) && (j==col)) ){
						if(matriz[i][j] == VIVA)
							soma++;
					}
				}
			}
		}
	}
	
	return soma;
}

/**
 * Função que a thread vai executar ao ser criada:
 * A função verifica se a celula na qual a thread se encontra trabalhando
 * deve sobreviver à próxima geração (ser escrita no próximo tabuleiro);
 * Nesse caso, ela escreve na posição correspondente do pŕoximo tabuleiro
 * o estado da célula (VIVA ou MORTA)
 * 
 * @param threadarg Ponteiro void para a struct de parâmetros para a thread
 */
void *exec_thread(void *threadarg){

	data *dados = (data*)threadarg;
	int ** auxm = NULL;
	int valida,lin,col;

	lin=dados->linha_atual;
	col=dados->coluna_atual;
	valida = valida_celula(lin, col);

	if(valida == VIVA){
		matriz_prox[lin][col] = VIVA;
	}
	else if(valida == MORTA){
		matriz_prox[lin][col] = MORTA;
	}
}

/**
 *  Imprime o tabuleiro atual
 */
void imprime()	{
	clear();
	int i,j;
	for(i=0; i<nlin; i++)
	{
		for(j=0; j<ncol; j++)
		{
			if(matriz[i][j])
				printw("# ");
			else
				printw("_ ");
		}
		printw("\n");
	}
	
	printw("iter: %d\n", iter);
	refresh();
}

/**
 *  Imprime as somas do tabuleiro atual (debug)
 */
void imprime_soma()	{
	printw("\n\n");
	int i,j;
	for(i=0; i<nlin; i++)
	{
		for(j=0; j<ncol; j++)
		{
			printw("%d ", conta_celula(i, j));
		}
		printw("\n");
	}	

	refresh();
}

/**
 * Função main
 */
int main (int argc, char *argv[])
{
	int i,j,t;
	pthread_t** threads;	/**ponteiro para uma matriz que guarda as threads (id)*/
	data** thread_data;		/**ponteiro para os dados que vao ser passados - um para cada thread*/
	int **matriz_tmp;		/**ponteiro auxiliar para a troca entre as matrizes*/
	clock_t t0=0, t1=0;    	/**variáveis para controle do tempo*/
	double fps=1.0;  		/**frames por segundo*/
	int sair=0;         	/**variável que verifica se é para sair ou não do programa*/
	char c;
	void *status;
	int rc;
	
	/**
	 * Verifica se há parâmetros de linha de comando e lê a figura inicial
	 */
	if(argc>1)
		pbm(argv[1], &matriz, &nlin, &ncol);
	else
		pbm("./Gospers_glider_gun.pbm", &matriz, &nlin, &ncol);
	
	/** Aloca a matriz da próxima posição */
	matriz_prox = (int **)calloc(nlin,sizeof(int*));
	for(i=0;i<nlin;i++)
		matriz_prox[i]=(int*)calloc(ncol,sizeof(int));

	
	/**
	 * Inicializaçao das estruturas, onde threads sao as threads disponiveis - uma para cada celula 
	 * do tabuleiro -, thread_data é a estrutura a ser passada como argumento para a função que as 
	 * threads executam
	 */ 
	threads = (pthread_t **)malloc(nlin*sizeof(pthread_t*));
	thread_data = (data **)malloc(nlin*sizeof(data*));

	for(i=0;i<nlin;i++){
		/**
		 * Alocação dinâmica da matriz de threads
		 */
		threads[i]=(pthread_t*)malloc(ncol*sizeof(pthread_t));
		thread_data[i]=(data*)malloc(ncol*sizeof(data));

		for(j=0;j<ncol;j++){
			/**
			 * Inicialização dos dados a serem passados para cada thread
			 */
			thread_data[i][j].id[0] = i;
			thread_data[i][j].id[1] = j;
			thread_data[i][j].linha_atual = i;
			thread_data[i][j].coluna_atual = j;
		}
	}
	
	/** Inicializa a ncurses */
	initscr(); cbreak(); noecho(); noraw(); timeout(0);
	
	t0 = clock(); // Inicializa o cronometro
	do
	{
		t1 = clock();
		/* Se já deu o tempo, roda mais uma iteração do jogo */
		if( (t1 - t0)/(double)CLOCKS_PER_SEC > 1.0/fps )
		{
			/**
			 *  CODIGO A CADA ITERACAO 
			 */
			/** Modifica o próximo tabuleiro */
			for(i=0;i<nlin;i++){
				for(j=0;j<ncol;j++){
					t=pthread_create(&threads[i][j],NULL,exec_thread,(void *)&thread_data[i][j]);

					if(t){
						printf("ERROR; return code from pthread_create() is %d\n", t);
						exit(-1);
					}
				}
			}
			
			/** Espera as threads morrerem */
			for(i=0;i<nlin;i++){
				for(j=0;j<ncol;j++){
					rc = pthread_join(threads[i][j], &status);
					if (rc) {
						printf("ERROR; return code from pthread_join() is %d\n", rc);
						exit(-1);
					}
					printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
				}
			}
			
			imprime();	/** Imprime */
			imprime_soma();
			iter++; 	/** Incrementa o contador de iterações */
			
			/** Realiza a troca entre as matrizes */
			matriz_tmp = matriz;
			matriz = matriz_prox;
			matriz_prox = matriz_tmp;
			
			/** Passa para o proximo frame */
			t0 = t1;	
		}
		/* Verifica se é pra sair */
		c = getch();
		if(c!=10) sair=1;
	} while(!sair);
	
	endwin();	/** Finaliza a ncurses */
	
	return 0;
}

