#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define PROMPT "enseash % "
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define EXIT_MESSAGE "Bye bye ...\n" 
#define DIVISION_NS 1000000    // Division factor for execution time ns
#define DIVISION_S 1000        // Division factor for execution time s

int Command(char *command, int *status, long *exec_time){
	struct timespec start, end;
	
	clock_gettime(CLOCK_MONOTONIC, &start);  //start timer
	
	pid_t pid = fork();  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){  // child process
        char *args[1024];
        char *token = strtok(command, " ");  // Tokenize the command input by spaces
        int i = 0;

        // Store tokens into the args array
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;  // Null-terminate the argument list
        
        // execvp will handle that correctly.
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {  //parent process
		wait(status);  //wait for the child to finish
        clock_gettime(CLOCK_MONOTONIC, &end);  //end timer
        *exec_time = (end.tv_sec - start.tv_sec) * DIVISION_S + (end.tv_nsec - start.tv_nsec) / DIVISION_NS;  // Milliseconds
    }
    return *status;  // Return the status of the command
}

void Exit(){
	ssize_t ByteWrite = write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE)); //Display of an exit message
	exit(EXIT_SUCCESS);
}

void WelcomeMessage(){
    //Display of a welcome message
    ssize_t ByteWrite = write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

void Prompt(int status, long exec_time){
    char prompt[100];
    
    if (WIFEXITED(status)) {
        snprintf(prompt, sizeof (prompt), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exec_time);
    } else if (WIFSIGNALED(status)) {
        snprintf(prompt, sizeof (prompt), "enseash [exit:%d|%ldms] %% ", WTERMSIG(status), exec_time);
    }
    
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main() {
    WelcomeMessage();
    int status = 0;
    long exec_time = 0;

    while(1){
        Prompt(status, exec_time);
		char userInput[1024];
		int byteread = read(STDIN_FILENO, userInput, sizeof(userInput));

        //read user input
        if(byteread == 0){
            //handle ctrl+d or end of file
            Exit();
        }
		
		// remove '\n' (newline) from the command input
        userInput[strcspn(userInput, "\n")] = 0;	
        
        // if the command is exit
        if (strcmp(userInput, "exit") == 0)  {
			Exit();
        }
        
        // Execute the entered command
        Command(userInput, &status, &exec_time);
	}
	return 0;
}
