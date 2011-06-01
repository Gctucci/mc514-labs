#include <stdio.h>

void coluna(int count, char *label)
{
	printf("\t\t\t\t<li>%d:%s</li>\n", count, label);
}

int main()
{
	char titulo[80]="";
	char tmp[10]="";
	int dado, contagem[15];
	int i=0;
	
	// Lê o título
	fgets(titulo, 80, stdin);
	
	for(i=0; i<15; i++)
		contagem[i] = 0;
	
	// Lê os dados, colocando-os em suas categorias
	while( scanf("%d\n", &dado) == 1)
	{
		if (dado <= 10){
			contagem[dado-1] += 1;
		}
		else if (dado <= 15){
			contagem[10] += 1;
		}
		else if (dado <= 25){
			contagem[11] += 1;
		}
		else if (dado <= 50){
			contagem[12] += 1;
		}
		else if (dado <= 250){
			contagem[13] += 1;
		}
		else{
			contagem[14] += 1;
		}
	}
	
	// Imprime os dados
	for(i=0; i<15; i++)
	{
		if(i<10){
			sprintf(tmp, "%d", i+1);
			coluna(contagem[i], tmp);
		}
		else if(i==10)
			coluna(contagem[i], "11-15");
		else if(i==11)
			coluna(contagem[i], "16-25");
		else if(i==12)
			coluna(contagem[i], "26-50");
		else if(i==13)
			coluna(contagem[i], "51-250");
		else
			coluna(contagem[i], "251-inf");
	}
}

