#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<pthread.h>

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
	int** tabuleiro;	/** ponteiro para o tabuleiro */
	int** tabuleiro_pos;    /** ponteiro para a próxima fase do tabuleiro*/
	int linha_atual;	/** linha do pixel da thread */
	int coluna_atual;	/** coluna do pixel da thread */
	int tam_col;    	/** número de colunas do tabuleiro */
	int tam_lin;    	/** número de linhas */

} data;

int valida_celula(void*threadarg){
        //Indica a soma do numero de celulas vivas
        int num_viva;
       	data* dados = (data*)threadarg;

        if(&dados->tabuleiro !=NULL){
                num_viva = conta_celula(threadarg);
                /*Caso o numero de celulas vizinhas seja maior que tres ou menor
                que dois, a celula morre por superlotação/solidão. Senão, ela sobrevive
                para a proxima geração*/
                if(num_viva > SUPERLOT || num_viva < SOLIDAO){
                        return MORTA;
                }
                else{
                        return VIVA;
                }
		

        }
	else{
		printf("ERROR; the original file is absent or corrupted.");
   		exit(-1);

	}
       
}


/**
 * Função que conta as células adjacentes a uma célula de posição tabuleiro[linha][coluna].
 * A função também trata os casos especiais (se a célula estudada estiver numa borda, por ex.).
 */
int conta_celula(void *threadarg){

	data *dados = (data*)threadarg;
       
        int soma=0,i,j;
	int lin = dados->linha_atual;
	int col = dados->coluna_atual;
	int tamlin = dados->tam_lin;
	int tamcol=dados->tam_col;
       
        if(lin -1 < 0){
                lin=1;
        }

        if(col-1 < 0){
                col=1;
        }
       
        if(lin+1 >= tamlin){
                lin=tamlin-1;
        }
        if(col+1 >= tamcol){
                col=tamcol-1;        
        }

        for(i=lin-1;i<=lin+1;i++){
                for(j=col-1;j<=col+1;j++){
                        if(&dados->tabuleiro[i][j] > 0){
                                soma++;
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
 * o estado da célula (VIVA ou MORTA) e troca as posições dos tabuleiros:
 * o tabuleiro de próxima geração passa a ser o atual, e o atual de próxima;
 * 
 */
void *exec_thread(void *threadarg){

	data *dados = (data*)threadarg;



}

	/**
	 *  Imprime
	 * @param nlin número de linhas do tabuleiro
	 * @param ncol número de colunas do tabuleiro
	 * @param pmatriz ponteiro para o tabuleiro em memória
	 */
void imprime(int nlin, int ncol, int** pmatrix)	{
	int i,j;
	initscr();
	for(i=0; i<nlin; i++)
	{
		for(j=0; j<ncol; j++)
		{
			if(pmatrix[i][j])
				printw("# ");
			else
				printw("_ ");
		}
		printw("\n");
	}
	
	printw("fim\n");
	refresh();
	getch();
	endwin();
}

int main (int argc, char *argv[])
{
	int i,j,nlin,ncol,t;
	pthread_t** threads; /**ponteiro para uma matriz que guarda as threads (id)*/
	data** thread_data; /**ponteiro para os dados que vao ser passados - um para cada thread*/
	int ** pmatrix;     /**matriz auxiliar para criar um tabuleiro de próxima geração*/
	
	// Verifica se há parâmetros de linha de comando
	if(argc>1)
		pbm(argv[1], &pmatrix, &nlin, &ncol);
	else
		pbm("./Gospers_glider_gun.pbm", &pmatrix, &nlin, &ncol);

	
	/**
	 * Inicializaçao das estruturas, onde threads sao as threads disponiveis - uma para cada celula 
	 * do tabuleiro -, thread_data é a estrutura a ser passada como argumento para a função que as 
	 * threads executam, e pmatrix é um tabuleiro auxiliar, que registra o proximo estado do jogo da vida
	 */ 
	threads = (pthread_t **)malloc(nlin*sizeof(pthread_t*));
	thread_data = (data **)malloc(nlin*sizeof(data*));
	pmatrix = (int **)calloc(nlin,sizeof(int*));

	for(i=0;i<nlin;i++){
		/*Alocação dinâmica da matriz de threadsm dos dados e do tabuleiro*/
		threads[i]=(pthread_t*)malloc(ncol*sizeof(pthread_t));
		thread_data[i]=(data*)malloc(ncol*sizeof(data));
		pmatrix[i]=(int*)calloc(ncol,sizeof(int));

		for(j=0;j<ncol;j++){

			/*Inicialização dos dados a serem passados para cada thread*/
			thread_data[i][j].tabuleiro=pmatrix;
			thread_data[i][j].id[0] = i;
			thread_data[i][j].id[1] = j;
			thread_data[i][j].linha_atual = i;
			thread_data[i][j].coluna_atual = j;
			thread_data[i][j].tabuleiro_pos = pmatrix;
			/*Criação das threads propriamente ditas*/
			t=pthread_create(&threads[i][j],NULL,exec_thread,(void *)&thread_data[i][j]);

			if(t){
				printf("ERROR; return code from pthread_create() is %d\n", t);
   			        exit(-1);

			}
		}
	}
	
	
	imprime(nlin,ncol,pmatrix);
	
	return 0;
}


