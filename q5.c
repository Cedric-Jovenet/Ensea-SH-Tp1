#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define PROMPT "enseash % "
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define FORTUNE "Today is what happened to yesterday \n"
#define EXIT_MESSAGE "Bye bye ...\n"

void Exit(){
	ssize_t ByteWrite = write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE)); //Display of an exit message
	exit(EXIT_SUCCESS);
}

int Command(char *command, int *status, long *exec_time){
	struct timespec start, end;
	
	clock_gettime(CLOCK_MONOTONIC, &start);   //start timer

	pid_t pid = fork();  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){  //child process
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {  //parent process
		wait(status);  //wait for the child to finish
        clock_gettime(CLOCK_MONOTONIC, &end);  //end timer
        *exec_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;  // Milliseconds
    }
    return *status;  // Return the status of the command
}

void WelcomeMessage(){
    //Display of a welcome message
    ssize_t ByteWrite = write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

void Prompt(int status, long exec_time){
    char buf[50];
    
    if (WIFEXITED(status)) {
        snprintf(buf, sizeof (buf), "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exec_time);
    } else if (WIFSIGNALED(status)) {
        snprintf(buf, sizeof (buf), "enseash [exit:%d|%ldms] %% ", WTERMSIG(status), exec_time);
    }
    
    write(STDOUT_FILENO, buf, strlen(buf));
}

int main() {
    WelcomeMessage();
    int status = 0;
    long exec_time = 0;


    while(1){
        Prompt(status, exec_time);
		char userInput[1024];
		int byteread;

        //read user input
        if((byteread = read(STDIN_FILENO,userInput, sizeof(userInput))) == 0){
            //handle ctrl+d or end of file
            Exit();
        }
		
		// remove '\n' (newline) from the command input
        userInput[strcspn(userInput, "\n")] = 0;	

        // if the command is fortune
        if (strcmp(userInput, "fortune") == 0) {
            write(STDOUT_FILENO, FORTUNE, strlen(FORTUNE));
            continue;
        }
        
        // if the command is exit
        if (strcmp(userInput, "exit") == 0)  {
			Exit();
        }
        
        // Execute the entered command
        Command(userInput, &status, &exec_time);
	}
}
