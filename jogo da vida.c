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

	data *dados = (data*)threadarg;
	int ** auxm = NULL;
	int valida,lin,col;

	lin=dados->linha_atual;
	col=dados->coluna_atual;
	
	matriz_prox[lin][col] = valida_celula(lin, col);
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
 * Função que processa uma certa área da matriz.
 * Se a matriz for muito grande, ela é tratada em quatro partes, recursivamente.
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
	
	/* Verifica a área a ser tratada */
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
		/* Modifica o próximo tabuleiro */
		for(i=lin_0; i<lin_1; i++){
			for(j=col_0; j<col_1; j++){
				t=pthread_create(&threads[i][j],NULL,exec_thread,(void *)&thread_data[i][j]);

				if(t){
					printf("ERRO; codigo de retorno de pthread_create() is %d\n", t);
					exit(-1);
				}
			}
		}

		/* Espera as threads morrerem */
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
 * Função que processa a matriz sem usar threads. (debug)
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
 * Função que processa a matriz
 */
void processa_com_pool()
{
	
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
	int sair=0;             /* variável que verifica se é para sair ou não do programa*/
	char c;
	void *status;
	int rc;
	char tmp[100];
	pthread_attr_t attr;
	Vetor **cel_livres;
	
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
	
	cel_livres = (Vetor **)malloc(nlin, sizeof(Vetor*));
	for(i=0;i<nlin;i++){
		cel_livres[i]=(int*)calloc(ncol,sizeof(int));
	}
	

	
	/* Inicializaçao das estruturas, onde threads sao as threads do pool (MAXTHREADS)
	 * thread_data é a estrutura a ser passada como argumento para a função que as 
	 * threads executam
	 */ 
	threads = (pthread_t *)malloc(MAXTHREADS*sizeof(pthread_t));
	thread_data = (data *)malloc(MAXTHREADS*sizeof(data));
	
	/* Inicializa o pool de threads */
	for(i=0; i<MAXTHREADS; i++){
		t=pthread_create(&threads[i],attr,exec_thread,(void *)&thread_data[i]);
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
			/* Define o numero de celulas a serem tratadas */
			cel_livres = nlin * ncol;
			
			/* Modifica o próximo tabuleiro */
			//processa_sem_threads(); // Descomentar para fins de depuração
			processa_com_pool();
			
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

