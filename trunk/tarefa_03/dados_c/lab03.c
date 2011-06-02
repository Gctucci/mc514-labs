#include <ext2fs/ext2fs.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>




int main(int argc, char **argv){

	char * name;
	ext2_filsys  fs;
	errcode_t error;
	ext2_inode_scan *scan;

	if(argc < 2){
		printf("Numero de argumentos inválidos. Favor digitar o sistema de arquivos.\n");
	}
	else{
		strcpy(argv[1],name);
		error = ext2fs_open(name,0,0,0,unix_io,&fs);
		if(error){
			printf("Houve um problema ao abrir o filesystem.\n");
		}
		ext2fs_open_inode_scan (fs,0,scan); //Prepara a variavel scan para iterar sobre os inodes
		
		error = ext2fs_close (fs);
		if(error){
			printf("Houve um problema ao fechar o filesystem.\n");
		}
	}
	
return 0;
}
