#include "jogo da vida.h"


/**
 * Função que le os parametros iniciais e os atribui à variáveis
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 */
void config(int argc, char *argv[]){
	/**
	 * Verifica se há parâmetros de linha de comando e lê a figura inicial
	 */
	if(argc>1)
		pbm(argv[1], &matriz, &nlin, &ncol);
	else
	{
		printf("Modo de uso:\n\t./life [arquivo de entrada] [frames por segundo(opcional)]\n");
		printf("Exemplo de execucao: ./life pulsar.pbm\n");
		exit(0);
	}
	
	if(argc>2)
		sscanf(argv[2], "%f", &fps);

}

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
				if( (j >= 0) && (j < ncol) ){
				
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
	
	matriz_prox[lin][col] = valida_celula(lin, col);
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
	
	printw("iter: \t%d\n", iter);
	printw("fps: \t%.1f\n", fps);
	printw("Pressione qualquer tecla para sair.\n");
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
void processa_area(int lin_0, int lin_1, int col_0, int col_1, pthread_t** threads, data** thread_data)
{
	int i, j, t;
	void *status;
	int rc;
	int lin_meio, col_meio;
	
	/** Verifica a área a ser tratada */
	if( (col_1-col_0)*(lin_1-lin_0) > MAXTHREADS)
	{
		lin_meio = (lin_1+lin_0)/2;
		col_meio = (col_1+col_0)/2;
		
		processa_area(lin_0, lin_meio, col_0, col_meio, threads, thread_data);
		processa_area(lin_meio, lin_1, col_0, col_meio, threads, thread_data);
		
		processa_area(lin_0, lin_meio, col_meio, col_1, threads, thread_data);
		processa_area(lin_meio, lin_1, col_meio, col_1, threads, thread_data);
	}
	else
	{
		/** Modifica o próximo tabuleiro */
		for(i=lin_0; i<lin_1; i++){
			for(j=col_0; j<col_1; j++){
				t=pthread_create(&threads[i][j],NULL,exec_thread,(void *)&thread_data[i][j]);

				if(t){
					printf("ERRO; codigo de retorno de pthread_create() is %d\n", t);
					exit(-1);
				}
			}
		}

		/** Espera as threads morrerem */
		for(i=lin_0; i<lin_1; i++){
			for(j=col_0; j<col_1; j++){
				rc = pthread_join(threads[i][j], &status);
				if (rc) {
					printf("ERRO; codigo de retorno de pthread_join() is %d\n", rc);
					exit(-1);
				}
			}
		}
	}
}

/**
 * Processa a matriz sem usar threads (debug)
 */
void processa_sem_threads()
{
	int valida, i, j;
	for(i=0;i<nlin;i++){
		for(j=0;j<ncol;j++){
			matriz_prox[i][j] = valida_celula(i, j);
		}
	}	
}

/**
 * Função main
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 */
int main (int argc, char *argv[])
{
	int i,j,t;
	pthread_t** threads;	/**ponteiro para uma matriz que guarda as threads (id)*/
	data** thread_data;		/**ponteiro para os dados que vao ser passados - um para cada thread*/
	int **matriz_tmp;		/**ponteiro auxiliar para a troca entre as matrizes*/
	clock_t t0=0, t1=0;    	/**variáveis para controle do tempo*/
	int sair=0;         	/**variável que verifica se é para sair ou não do programa*/
	char c;
	void *status;
	int rc;
	
	/**
	 * Lê os parâmetros, e interrompe a execução se não houver nenhum
	 */
	config(argc, argv);
	
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
			/** Modifica o próximo tabuleiro */
			processa_area(0, nlin, 0, ncol, threads, thread_data);
			//processa_sem_threads(); //Descomentar para fins de depuração 
			
			imprime();	/** Imprime */
			//imprime_soma();
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
		if(c!=CHAR) sair=1;
	} while(!sair);
	
	endwin();	/** Finaliza a ncurses */
	
	/**
	 * Libera a memória utilizada
	 * obs: A biblioteca ncurses causa alguns leaks intencionais.
	 */
	for(i=0; i<nlin; i++)
	{
		free(threads[i]);
		free(thread_data[i]);
		free(matriz[i]);
		free(matriz_prox[i]);
	}
	free(threads);
	free(thread_data);
	free(matriz);
	free(matriz_prox);
	
	return 0;
}
