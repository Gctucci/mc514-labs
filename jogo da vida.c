#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<pthread.h>
#define MORTA 0
#define VIVA 1
#define SUPERLOT 3
#define SOLIDAO 2

typedef struct thread_data_structure {

	int id[3];
	int** tabuleiro;
	int linha_atual;
	int coluna_atual;
	int tam_col;
	int tam_lin;

} data;

void* valida_celula(void *threadarg){
        //Indica a soma do numero de celulas vivas
        int num_viva;
       	data *dados = (data*)threadarg;

        if(dados->tabuleiro !=NULL){
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
       
}


/*Função que conta as células adjacentes a uma célula de posição tabuleiro[linha][coluna].
A função também trata os casos especiais (se a célula estudada estiver numa borda, por ex.).*/
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
                        if(dados->tabuleiro[i][j] >0){
                                soma++;
                        }      
                }
        }      

return soma;
}

int main (){

	int i,j,nlin,ncol,t;
	pthread_t** threads;
	data** thread_data;

	threads = (pthread_t **)malloc(nlin*sizeof(pthread_t*));
	thread_data = (data **)malloc(nlin*sizeof(data*));
	for(i=0;i<nlin;i++){
		for(j=0;j<ncol;j++){
			threads[i]=(pthread_t*)malloc(ncol*sizeof(pthread_t));
			thread_data[i]=(data*)malloc(ncol*sizeof(data));
			thread_data[i][j].id[0] = i;
			thread_data[i][j].id[1] = j;
			t=pthread_create(&threads[i][j],NULL,valida_celula,(void *)&thread_data);
			if(t){
				printf("ERROR; return code from pthread_create() is %d\n", t);
   			        exit(-1);

			}
		}
	}
	
return 0;
}


