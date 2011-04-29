#include "mycp.h"
#include <aio.h>
#include <errno.h>

/**
 * Função que copia um arquivo utilizando as funções da aio.h
 * 
 * @param files Nomes dos arquivos de origem e destino
 */
int mycp5(char** files){
	int reader, writer, ret;
	char buffer[MAXBUFF+1];
	struct aiocb data_in;
	struct aiocb data_out;

	if(files == NULL){
		printf("Error while opening files: check if there any arguments were passed.\n");
		exit(-1);
	}

	// Struct de dados do arquivo de entrada
	memset(&data_in, 0, sizeof(struct aiocb));
	data_in.aio_nbytes = MAXBUFF;
	data_in.aio_offset = 0;
	data_in.aio_buf = buffer;
	// Abre o arquivo de entrada
	data_in.aio_fildes = open(files[1], O_RDONLY);
	if (data_in.aio_fildes < 0){ 
		printf("ERROR: File cannot be opened.\n");
		exit(-1);
	}
	
	// Struct de dados do arquivo de saida
	memset(&data_out, 0, sizeof(struct aiocb));
	data_out.aio_nbytes = MAXBUFF;
	data_out.aio_offset = 0;
	data_out.aio_buf = buffer;
	// Abre o arquivo de saída
	chdir(files[2]);
	data_out.aio_fildes = creat(files[1], OUTPUT);
	if (data_out.aio_fildes < 0){ 
		printf("ERROR: File cannot be created.\n");
		exit(-1);
	}
	
	// Loop que copia o arquivo
	while (TRUE) {
		// lê um bloco de dados
		reader = aio_read(&data_in);
		
		// Se ocorrer um erro durante a cópia, ou o arquivo acabar, o loop pára.
		if (reader < 0){
			printf("Error while reading file: File could not be read.\n");
			exit(-1);
		}
		
		// Espera até acabar de ler
		while(aio_error(&data_in) == EINPROGRESS);
		
		// Verifica a leitura
		ret = aio_return( &data_in );
		if (ret > 0) {
			// Avança o offset
			data_in.aio_offset += ret;
			// Define o quanto será escrito
			data_out.aio_nbytes = ret;
		} else if(ret < 0) {
			continue;
		} else {
			// Terminou de ler o arquivo todo
			break;
		}
		
		
		// escreve um bloco de dados
		writer = aio_write(&data_out);
		
		// Se ocorrer um erro durante a cópia, ou o arquivo acabar, o loop pára.
		if (writer < 0){
			printf("Error while writing file: File could not be written.\n");
			exit(-1);
		}
		
		// Espera até acabar de escrever
		while(aio_error(&data_out) == EINPROGRESS);
		
		// Verifica a escrita
		ret = aio_return( &data_out );
		if (ret > 0) {
			// Avança o offset
			data_out.aio_offset += ret;
		} else if(ret < 0) {
			//continue;
		} else {
			// Terminou de ler o arquivo todo
			break;
		}
	}
	/*Fechando os arquivos/diretórios*/
	close(data_in.aio_fildes); close(data_out.aio_fildes);
	
	return 0;
}

