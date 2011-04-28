#include "apue.h"
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<dirent.h>
#include<string.h>
#include<aio.h>

#define MAXBUFF 4096
#define OUTPUT 0700
#define TRUE 1

/**Função de cp mais básica, que apenas muda o diretório corrente,
cria um fd e copia o conteúdo para o mesmo*/
int mycp1( char **files);


/**Função de cp que usa a opção não bloqueante - O_NONBLOCK*/
int mycp2( char **files);


/**Função de cp que usa a função select() para monitorar os file descriptors*/
int mycp3(char **files);


/**Função que copia um arquivo utilizando as funções da aio.h*/
int mycp4(char **files);
