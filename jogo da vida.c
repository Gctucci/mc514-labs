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

/*
 * Função que a thread vai executar ao ser criada
 */
void *exec_thread(void *threadarg){

	data *dados = (data*)threadarg;



}

int main (int argc, char *argv[])
{
	int i,j,nlin=2,ncol=2,t;
	pthread_t** threads;
	data** thread_data;
	int ** pmatrix;
	
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
		threads[i]=(pthread_t*)malloc(ncol*sizeof(pthread_t));
		thread_data[i]=(data*)malloc(ncol*sizeof(data));
		pmatrix[i]=(int*)calloc(ncol,sizeof(int));
		for(j=0;j<ncol;j++){

			
			thread_data[i][j].tabuleiro=pmatrix;
			thread_data[i][j].id[0] = i;
			thread_data[i][j].id[1] = j;
			thread_data[i][j].linha_atual = i;
			thread_data[i][j].coluna_atual = j;
		
			t=pthread_create(&threads[i][j],NULL,exec_thread,(void *)&thread_data[i][j]);

			if(t){
				printf("ERROR; return code from pthread_create() is %d\n", t);
   			        exit(-1);

			}
		}
	}
	
	
	/**
	 *  Imprime
	 */
	for(i=0; i<nlin; i++)
	{
		for(j=0; j<ncol; j++)
		{
			if(pmatrix[i][j])
				printf("# ");
			else
				printf("_ ");
		}
		printf("\n");
	}
	
	printf("fim\n");
	
	return 0;
}


