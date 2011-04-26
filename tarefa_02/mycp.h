#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

#define MAXBUFF 4096
#define OUTPUT 0700
#define TRUE 1

/**Função de cp mais básica, que apenas muda o diretório corrente,
cria um fd e copia o conteúdo para o mesmo*/
int mycp1( char **files);
