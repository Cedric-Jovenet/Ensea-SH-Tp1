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


void Command(char *command){
	pid_t pid = fork();  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){  //child process
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {  //parent process
        waitpid(pid, NULL, 0);  //wait for the child to finish
    }
}

void Exit(){
	ssize_t ByteWrite = write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE)); //Display of an exit message
	exit(EXIT_SUCCESS);
}

void WelcomeMessage(){
    //Display of a welcome message
    ssize_t ByteWrite = write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

void Prompt(){
    //Display of a simple prompt
    ssize_t ByteWrite = write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

int main() {
    WelcomeMessage();
    while(1){
		Prompt();
		char userInput[1024];
		int byteread = read(STDIN_FILENO,userInput, sizeof(userInput));

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
        
        //execute the entered command
        Command(userInput);
	}
}
