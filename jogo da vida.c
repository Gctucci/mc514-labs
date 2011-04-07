#include "jogo da vida.h"


/**
 * Função que le os parametros iniciais e os atribui à variáveis
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 */
void config(int argc, char *argv[]){
	/* Verifica se há parâmetros de linha de comando e lê a figura inicial */
	if(argc>1)
		pbm(argv[1], &matriz, &nlin, &ncol);
	else
	{
		printf("Modo de uso:\n\t./life [arquivo de entrada] [frames por segundo(opcional)]\n");
		printf("Exemplo de execucao: ./life pulsar.pbm 5.0\n");
		exit(0);
	}
	
	/* Verifica se há um valor de FPS nos parâmetros */
	if(argc>2)
		sscanf(argv[2], "%f", &fps);

}

/**
 * Função que verifica se uma célula da matriz atual estará viva no próximo turno
 * 
 * @param lin Linha da célula
 * @param col Coluna da célula
 * 
 * @returns O estado da célula no próximo turno
 */
int valida_celula(int lin, int col){
	int num_viva; /** Indica a soma do numero de celulas vivas */

	num_viva = conta_celula(lin, col);
	if(matriz[lin][col] == VIVA)
	{
		/* Caso o numero de celulas vizinhas seja maior que tres ou menor
		 * que dois, a celula morre por superlotação/solidão. Senão, ela sobrevive
		 * para a proxima geração
		 */
		if(num_viva > SUPERLOT || num_viva < SOLIDAO)
			return MORTA;
		else
			return VIVA;
	}
	else
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
 * 
 * @returns Contágem de células vivas ao redor da posição indicada
 */
int conta_celula(int lin, int col){
	int soma=0,i,j;
	
	/* Limites da linha */
	for(i=lin-1; i<lin+2; i++){
		if( (i >= 0) && (i < nlin) ) {
		
			/* Limites da coluna */
			for(j=col-1;j<col+2;j++){
				if( (j >= 0) && (j < ncol) ){
				
					/* Ignorar a própria célula */
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
	int lin,col;
	Ponto *cel=NULL;
	while(!sair){
		// Acessa a lista de celulas
		pthread_mutex_lock(&mutex_lista);
		if(lista_cel!=NULL){
			// Pega a primeira, tira da lista e já desocupa o mutex
			cel = lista_cel;
			lista_cel = lista_cel->prox;
			pthread_mutex_unlock(&mutex_lista);
			
			// Trata a célula
			lin = cel->i;
			col = cel->j;
			matriz_prox[lin][col] = valida_celula(lin, col);
			
			// Incrementa o contador de células
			pthread_mutex_lock(&mutex_num);
			num_cel++;
			// Se já deu o limite, avisa pra main
			if(num_cel==nlin*ncol) pthread_cond_signal(&cond_num);
			pthread_mutex_unlock(&mutex_num);
		}
		else
			pthread_mutex_unlock(&mutex_lista);
	}
	return NULL;
}

/**
 * Função que imprime o tabuleiro atual.
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
	printw("num_cel: \t%d\n", num_cel);
	printw("Pressione qualquer tecla para sair.\n");
	refresh();
}

/**
 * Função que imprime as somas do tabuleiro atual. (debug)
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
 * Função que organiza a lista de celulas a serem tratadas
 */
void organiza_cel_livres(Ponto **matr_cel)
{
	int i, j;
	pthread_mutex_lock(&mutex_lista);
	lista_cel = &matr_cel[0][0];
	for(i=0;i<nlin;i++){
		for(j=0;j<ncol-1;j++)
		{
			matr_cel[i][j].prox = &matr_cel[i][j+1];
		}
		if(i<nlin-1)
			matr_cel[i][ncol-1].prox = &matr_cel[i+1][0];
	}
	pthread_mutex_unlock(&mutex_lista);
}

/**
 * Função main
 * @param argc numero de argumento passados na entrada
 * @param argv vetor com os valores dos argumentos passados
 * 
 * @returns 1 se houver algum erro, 0 caso contrário
 */
int main (int argc, char *argv[])
{
	int i,j,t;
	pthread_t* threads;     /* ponteiro para um vetor que guarda as threads (id)*/
	data* thread_data;	    /* ponteiro para os dados que vao ser passados - um para cada thread*/
	int **matriz_tmp;       /* ponteiro auxiliar para a troca entre as matrizes*/
	clock_t t0=0, t1=0;    	/* variáveis para controle do tempo*/
	char c;
	char tmp[100];
	pthread_attr_t attr;
	void *status;
	int rc;
	Ponto **matr_cel;       /* matriz de structs de pontos, a ser usado para a lista de celulas */
	pthread_mutex_init(&mutex_num, NULL);
	pthread_mutex_init(&mutex_lista, NULL);
	pthread_cond_init (&cond_num, NULL);
	
	/* atributos das threads */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	/* Lê os parâmetros, e interrompe a execução se não houver nenhum */
	config(argc, argv);
	
	/* Remove as imagens pbm da execução anterior */
	system("rm tmp/*.pbm");
	
	/* Aloca a matriz da próxima posição */
	matriz_prox = (int **)calloc(nlin,sizeof(int*));
	for(i=0;i<nlin;i++)
		matriz_prox[i]=(int*)calloc(ncol,sizeof(int));
	
	matr_cel = (Ponto **)calloc(nlin,sizeof(Ponto*));
	for(i=0;i<nlin;i++){
		matr_cel[i]=(Ponto*)calloc(ncol,sizeof(Ponto));
		for(j=0;j<ncol;j++)
		{
			matr_cel[i][j].i = i;
			matr_cel[i][j].j = j;
			matr_cel[i][j].prox = NULL;
		}
	}
	
	/* Organiza a lista de celulas a serem tratadas */
	organiza_cel_livres(matr_cel);
	
	/* Inicializaçao das estruturas, onde threads sao as threads do pool (MAXTHREADS)
	 * thread_data é a estrutura a ser passada como argumento para a função que as 
	 * threads executam
	 */ 
	threads = (pthread_t *)malloc(MAXTHREADS*sizeof(pthread_t));
	thread_data = (data *)malloc(MAXTHREADS*sizeof(data));
	
	/* Inicializa o pool de threads */
	for(i=0; i<MAXTHREADS; i++){
		t=pthread_create(&threads[i],&attr,exec_thread,(void *)&thread_data[i]);
		if(t){
			printf("ERRO; codigo de retorno de pthread_create() eh %d\n", t);
			exit(-1);
		}
	}
	
	/* Inicializa a ncurses */
	initscr(); cbreak(); noecho(); noraw(); timeout(0);
	
	t0 = clock(); // Inicializa o cronometro
	do
	{
		t1 = clock();
		/* Se já deu o tempo, roda mais uma iteração do jogo */
		if( (t1 - t0)/(double)CLOCKS_PER_SEC > 1.0/fps )
		{
			pthread_mutex_lock(&mutex_num);
			/* Organiza a lista de celulas a serem tratadas */
			organiza_cel_livres(matr_cel);
			
			/* Espera até as threads acabarem com as celulas da lista */
			while(num_cel<nlin*ncol)
			{
				pthread_cond_wait(&cond_num, &mutex_num);
			}
			
			/* Imprime */
			imprime();
			//imprime_soma(); // Descomentar para fins de depuração
			
			/* Gera o pbm do frame atual */
			sprintf(tmp, "tmp/pbm%04d.pbm", iter);
			pbm_write(tmp, matriz, nlin, ncol); 
			
			/* Realiza a troca entre as matrizes */
			matriz_tmp = matriz;
			matriz = matriz_prox;
			matriz_prox = matriz_tmp;
			
			/* Incrementa o contador de iterações */
			iter++;
			
			/* Passa para o proximo frame */
			t0 = t1;	
			num_cel=0;
			pthread_mutex_unlock(&mutex_num);
		}
		/* Verifica se é pra sair */
		c = getch();
		if(c!=CHAR) sair=1;
	} while(!sair);
	
	endwin();	/* Finaliza a ncurses */
	
	/* Gerando gif */
	sprintf(tmp, "convert -scale %dx%d -delay 20 -loop 0 tmp/pbm*.pbm out.gif", 4*nlin, 4*ncol);
	system(tmp);
	printf("Arquivo out.gif gerado.\n");

	/* Encerra as threads */
	for(i=0; i<MAXTHREADS; i++){
		rc = pthread_join(threads[i], &status);
		if (rc) {
			printf("ERRO; codigo de retorno de pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	
	/* Libera a memória utilizada
	 * obs: A biblioteca ncurses causa alguns leaks intencionais.
	 */
	for(i=0; i<nlin; i++)
	{
		free(matriz[i]);
		free(matriz_prox[i]);
	}
	free(threads);
	free(thread_data);
	free(matriz);
	free(matriz_prox);
	
	return 0;
}

