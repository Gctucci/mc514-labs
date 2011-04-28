#include "mycp.h"


int mycp2(char** files){
	int     ntowrite, nwrite,indescr,outdescr;
    	char    *ptr;
	char buffer[MAXBUFF];


	if(files == NULL){
		printf("Error while opening files: check if there any arguments were passed.\n");
		exit(-1);
	}
	
	indescr = open(files[1], O_RDONLY);


	if (indescr < 0){ 
		printf("ERROR: File cannot be opened.\n");
		exit(-1);
	}

	chdir(files[2]);
	outdescr = creat(files[1], OUTPUT);
	
	if (outdescr < 0){ 
		printf("ERROR: File cannot be created.\n");
	}

	while(TRUE){
	    	ntowrite = read(indescr, buffer, MAXBUFF);
	    	if (ntowrite <= 0) break;

	    	set_fl(outdescr, O_NONBLOCK); /* set nonblocking */

	    	ptr = buffer;
	    	while (ntowrite > 0) {
	    	    
	    	    nwrite = write(outdescr, ptr, ntowrite);
		
	    	    if (nwrite > 0) {
	    	        ptr += nwrite;
	    	        ntowrite -= nwrite;
			
	    	    }
	    	}
	}
	    	clr_fl(outdescr, O_NONBLOCK); /* clear nonblocking */
		close(indescr); close(outdescr);
		
		if (ntowrite == 0){
			printf("File successfully copied!\n"); 
			exit(0);
		} 
		else{
			printf("Error while writing file. Please try again.\n");
			exit(-1);
		}
return 0;
}
