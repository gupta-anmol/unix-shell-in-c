#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<string.h>
#include<sys/wait.h>


int ash_ls(int argc, char **args) {
	if(argc == 1)
		execl("/bin/ls", args[0], NULL);
	
	else if(argc == 2)
		execl("/bin/ls", args[0], args[1], NULL);

	else
		execl("/bin/ls", args[0], args[1], args[2], NULL);

	return 0;
}
