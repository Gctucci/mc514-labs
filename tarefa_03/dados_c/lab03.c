#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2fs.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INO 50000

/**Estrutura que armazena o numero de blocos e fragmentos, bem como o bloco anterior a ele (para fins de encontrar segmentos contiguos)*/
typedef struct fsdados 
{
    long int datafrag, datablock;
    blk_t dataprev_block;
} Data;

/**Funcao que e passada como argumento para ext2fs_block_iterate2(), e que realiza o calculo das estatisticas propriamente ditas,
calculando a fragmentacao (e a clocagem) desejada e armezenando numa estrutura fsdados *arg*/
static int iterafunc(ext2_filsys fs,blk_t *bnumber, e2_blkcnt_t bcounter, blk_t ref_block EXT2FS_ATTR((unused)), int ref_offset EXT2FS_ATTR((unused)), void *priv_fsdados)
{
    struct fsdados *arg = (struct fsdados*) priv_fsdados;


    if (((int)bnumber[0]-arg->dataprev_block) > 1)
    {
        arg->datafrag++;
    }

    if ((int)bnumber[0] > 0)
    {
        arg->datablock++;
    }

    arg->dataprev_block = bnumber[1];
    return 0;
}





int main(int argc, char *argv[]){

	ext2_filsys  fs;
	errcode_t error;
	ext2_inode_scan scan;
	struct ext2_inode inode;
	ext2_ino_t inode_number;
	int i=0;
	Data fsdados;
	int tamfrag=0,taminode=0,frag_ocupados;

	/*Verifica se os parametros foram passados corretamente*/
	if(argc < 3){
		printf("Numero de argumentos inválidos. Programa deve ser executado da seguinte maneira:\n\
			--> make [opcao] [filesystem]\n\
			opcao 1 = histograma de fragmentação de blocos alocados em no. de segmentos\n\
			opcao 2 = histograma de tamanhos de arquivos em blocos de alocacao\n\
			opcao 3 = Retorna o numero de fragmentos ocupados(em KB/segmento)");
	}
	else{
		/*Abre o sistema de arquivos digitado (passado como argumento), usando as configurações padrão da função	
		*/
		error = ext2fs_open(argv[2],0,0,0,unix_io_manager,&fs);
		/*Verifica se o sistema de arquivos foi aberto corretamente*/
		if(error){
			printf("Houve um problema ao abrir o filesystem.\n");
			exit(-1);
		}

		/*Prepara a variavel scan para iterar sobre os inodes*/
		ext2fs_open_inode_scan (fs,0,&scan);
		/*Inicia a busca por inodes*/
		ext2fs_get_next_inode(scan,&inode_number,&inode);

		/*Mantém uma iteracao para buscar os inodes e calcular os fragmentos*/
		while(i<INO){
			i++;
			/*Busca o proximo inode e armazena seu nnumero em inode_number e o inode em inode*/
			ext2fs_get_next_inode(scan,&inode_number,&inode);

			/*Zera os valores da estrutura fsdados, para iniciar um novo calculo e evitar a classificacao
			incorreta dos segmentos na hora de plotar os dados num histograma*/ 
			fsdados.datafrag = 0;
        		fsdados.datablock = 0;
        		fsdados.dataprev_block = 0;

			/*Verifica se o inode encontrado nao e um diretorio*/
			if (ext2fs_check_directory(fs, inode_number) != 0)
        		{
            			/*Realiza a iteracao sobre todos os blocos do inode. 
				OBS: Foi usada a funcao iterate2 para evitar problemas com limites de sistemas de arquivos*/
            			ext2fs_block_iterate2(fs, inode_number, BLOCK_FLAG_DEPTH_TRAVERSE, 0, iterafunc, &fsdados);
 				
				/*Realiza o calculo de espaco ocupado calculando o tamanho de cada inode (inode.i_size), bem
				como o calculo dos segmentos (fsdados.datafrag)*/
				taminode +=inode.i_size;
				tamfrag += fsdados.datafrag;

				if(argv[1][0] == '1'){       
   					printf("%ld\n", fsdados.datafrag);
				}
				else if(argv[1][0] == '2'){       
   					printf("%ld\n", fsdados.datablock);
				}
				
        		}
			if(argv[1][0] == '3'){
				frag_ocupados = taminode/(tamfrag*1024);
				printf("%d\n",frag_ocupados);		
			}
			
		}
		/*Fecha(termina) a variavel scan*/
		ext2fs_close_inode_scan(scan);
		
		/*Fecha o sistema de arquivos apropriadamente*/
		error = ext2fs_close(fs);
		/*Imprime uma mensagem de erro caso o sistema de arquivos tenha sido fechado incorretamente*/
		if(error){
			printf("Houve um problema ao fechar o filesystem.\n");
		}
	}
	
return 0;
}
