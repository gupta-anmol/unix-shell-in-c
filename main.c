#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
#include<signal.h>
#include "ash_ls.c"
#include "ash_date.c"
#include "ash_mkdir.c"
#include "ash_rm.c"
#include "ash_cat.c"

int num_commands_in_history;
char *history_file;

int builtin_cd(int argc, char **args) {
	if(args[1] == NULL)
		chdir("/Users/anmolgupta");

	else {
		if(chdir(args[1]) == -1) {
			printf("No such file or directory\n");
		}
	}

	return 1;
}

int builtin_echo(int argc, char **args) {
	if(argc == 2) {
		printf("%s\n", args[1]);
	}

	if(argc > 2) {
		if(strcmp(args[1], "-n") == 0) {
			for(int i = 2; args[i] != NULL; i++) {
				printf("%s ", args[i]);
			}
		}

		else if(strcmp(args[1], "-E") == 0) {
			for(int i = 2; args[i] != NULL; i++) {
				printf("%s ", args[i]);
			}

			printf("\n");
		}

		else {
			for(int i = 1; args[i] != NULL; i++) {
				printf("%s ", args[i]);
			}

			printf("\n");
		}
	}

	return 1;
}

int builtin_pwd(int argc, char **args) {
	char pwd[256];
	getcwd(pwd, sizeof(pwd));
	printf("%s\n", pwd);
	return 1;
}

int builtin_history(int argc, char **args) {
	// printf("here1");
	FILE *filer;

	if(argc == 2) {
		if(strcmp(args[1], "-c") == 0) {
			filer = fopen(history_file, "w");
		}

		else {
			filer = fopen(history_file, "r");
			fseek(filer, 0, SEEK_SET);
			
			int num = atoi(args[1]), x, i;

			char *fline = malloc(sizeof(char) * 100);
			fgets(fline, 100, filer);
			i = 1;

			while(!feof(filer)) {
				i++;
				fgets(fline, 100, filer);
			}

			num_commands_in_history = i - 1;
			x = num_commands_in_history - num + 1;

			// printf("%d %d\n", num_commands_in_history, x);
			i = 1;

			fseek(filer, 0, SEEK_SET);
			fgets(fline, 100, filer);

			while(!feof(filer)) {
				if(i >= x)
					printf("%d %s", i++, fline);
				else i++;

				fgets(fline, 100, filer);
			}

			printf("\n");

		}

		// filer = freopen(history_file, "r", filer);
	}

	else {
		filer = fopen(history_file, "r");
		fseek(filer, 0, SEEK_SET);

		char *fline = malloc(sizeof(char) * 100);
		fgets(fline, 100, filer);
		int i = 1;

		while(!feof(filer)) {
			printf("%d %s", i++, fline);
			fgets(fline, 100, filer);
		}

		printf("\n");

	}


	fflush(filer);
	fclose(filer);
	return 1;
}

int builtin_exit(int argc, char **args) {
	// printf("here");
	// printf("%d", getpid());
	exit(0);
	return 0;
}

int is_builtin(int argc, char **args) {
	if(strcmp(args[0], "cd") == 0) {
		return builtin_cd(argc, args);
	}

	if(strcmp(args[0], "echo") == 0) {
		return builtin_echo(argc, args);
	}

	if(strcmp(args[0], "pwd") == 0) {
		return builtin_pwd(argc, args);
	}

	if(strcmp(args[0], "history") == 0) {
		return builtin_history(argc, args);
	}

	if(strcmp(args[0], "exit") == 0) {
		return builtin_exit(argc, args);
	}

	return 0;
}

char** ash_split_line(char line[]) {
	char **tokens = malloc(sizeof(10) * 7);
	char *token;
	const char delim[] = " ";
	token = strtok(line, delim);
	int position = 0;
	tokens[position++] = token;

	while(token != NULL) {
		token = strtok(NULL, delim);
		tokens[position++] = token;
	}

	tokens[position] = NULL;

	return tokens;
}

char* ash_read_line() {
	char *line = NULL;
	size_t size = 0; // What's the difference between size_t and ssize_t?
	getline(&line, &size, stdin);
	return line;
}

void ash_loop() {
	char *line, **args;
	int status = 1, val;	
	pid_t child_pid;
	pid_t wait_pid;

	FILE *filer;

	do {
		filer = fopen(history_file, "a+");

		printf("> ");
		line = ash_read_line();

		// printf("%s\n", line);

		if(line[0] == '\n')
			continue;
		
		// Check for echo

		// printf("Writing to file...\n");
		fputs(line, filer);
		fflush(filer);
		fclose(filer);
		
		if(line[0] == 'e' && line[1] == 'c' && line[2] == 'h' && line[3] == 'o') {
			int i, j = 0;
			char *newArg = malloc(sizeof(char) * 200);

			for(int i = 0; line[i] != '\0'; i++) {
				if(line[i] != '"') {
					newArg[j++] = line[i];
				}
			}

			newArg[j] = '\0';
			line = newArg;
		}


		// Remove the newline from the end of the line

		for(int i = 0; line[i] != '\0'; i++) {
			if(line[i] == '\n')
				line[i] = '\0';
		}
		
		args = ash_split_line(line);

		// printf("%s%s\n", args[0], args[1]);

		// Check if it's a builtin command, and if so, deal with it

		int argc;

		for(argc = 0; args[argc] != NULL; argc++) {
			// printf("%s\n", args[argc]);
		}

		if(is_builtin(argc, args)) {
			continue;
		}

		child_pid = fork();

		if(child_pid < 0) {
			perror("Fork failed");
		}

		if(child_pid == 0) {
			if(strcmp(args[0], "ls") == 0) {
				ash_ls(argc, args);
			}
			
			else if(strcmp(args[0], "date") == 0) {
				ash_date(argc, args);
			}
			
			else if(strcmp(args[0], "rm") == 0) {
				ash_rm(argc, args);
			}

			else if(strcmp(args[0], "cat") == 0) {
				ash_cat(argc, args);
			}

			else if(strcmp(args[0], "mkdir") == 0) {
				ash_mkdir(argc, args);
			}

			else {
				printf("command not found\n");
				// fclose(filer);
				exit(0);
			}
		}

		// if(child_pid == 0)
		// 	execvp(args[0], args);

		else {
			wait_pid = waitpid(child_pid, &val, WUNTRACED);
		}

	} while(status);
}

int main() {
	
	char pwd[100];
	getcwd(pwd, sizeof(pwd));
	strcat(pwd, "/history.txt");
	history_file = pwd;

	printf("\n===============================\nWelcome to anmol's shell - ASH!\n===============================\n\n");

	ash_loop();

	return 0;
}