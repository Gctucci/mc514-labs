#include <ext2fs/ext2fs.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>




int main(int argc, char **argv){

	const char * name;
	ext2_filsys  fs;
	errcode_t error;
	ext2_inode_scan scan;
	struct ext2_inode *inode;
	ext2_ino_t *inode_number;
	int i;
	

	if(argc < 2){
		printf("Numero de argumentos inválidos. Favor digitar o sistema de arquivos.\n");
	}
	else{
		strcpy(argv[1],name);
		error = ext2fs_open(name,0,0,0,unix_io_manager,&fs);
		if(error){
			printf("Houve um problema ao abrir o filesystem.\n");
		}
		ext2fs_open_inode_scan (fs,0,&scan); //Prepara a variavel scan para iterar sobre os inodes

		for(i=0;i<scan.s_inodes_count;i++){
			ext2fs_get_next_inode (scan,inode_number,inode);
			
		}
		ext2fs_close_inode_scan(scan);
		error = ext2fs_close (fs);
		if(error){
			printf("Houve um problema ao fechar o filesystem.\n");
		}
	}
	
return 0;
}
