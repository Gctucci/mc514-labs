#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<pthread.h>
#define MORTA 0
#define VIVA 1

int valida_celula(int** tabuleiro, int linha, int coluna,int tamlin, int tamcol){
	//Indica a soma do numero de celulas vivas
	int num_viva;
	

	if(tabuleiro !=NULL){
		num_viva = conta_celula(tabuleiro,linha,coluna,tamlin,tamcol);
		/*Caso o numero de celulas vizinhas seja maior que tres ou menor 
		que dois, a celula morre por superlotação/solidão. Senão, ela sobrevive 
		para a proxima geração*/
		if(num_viva > 3 || num_viva < 2){
			return MORTA;
		}
		else{
			return VIVA;
		}
	}
	
}


/*Função que conta as células adjacentes a uma célula de posição tabuleiro[linha][coluna].
A função também trata os casos especiais (se a célula estudada estiver numa borda, por ex.).*/
int conta_celula(int ** tabuleiro, int linha, int coluna,int tamlin,int tamcol){
	
	int soma=0,i,j;
	
	if(linha -1 < 0){
		linha=1;
	}

	if(coluna-1 < 0){
		coluna=1;
	}
	
	if(linha+1 >= tamlin){
		linha=tamlin-1;
	}
	if(coluna+1 >= tamcol){
		coluna=tamcol-1;	
	}

	for(i=linha-1;i<=linha+1;i++){
		for(j=coluna-1;j<=coluna+1;j++){
			if(tabuleiro[i][j] >0){
				soma++;
			}	
		}
	}	

return soma;
}

int main (){


return 0;
}
