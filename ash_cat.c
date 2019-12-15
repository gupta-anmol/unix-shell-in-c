#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<string.h>
#include<sys/wait.h>


int ash_cat(int argc, char **args) {
	
	if(argc == 1) {
		execl("/bin/cat", args[0], NULL);
	}
	
	else if(argc == 2) {
		execl("/bin/cat", args[0], args[1], NULL);
	}

	else if(argc == 3) {
		execl("/bin/cat", args[0], args[1], args[2], NULL);
	}

	else {
		execl("/bin/cat", args[0], args[1], args[2], args[3], NULL);
	}		
	return 0;
}
