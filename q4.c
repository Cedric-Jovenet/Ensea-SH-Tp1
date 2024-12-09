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

int Command(char *command){
	pid_t pid = fork();  //fork a new process
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){  //child process
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {  //parent process
		int status;
        pid_t wpid = waitpid(pid, &status,0);
        if (wpid == -1) {
            perror("execlp");                            
            exit(EXIT_FAILURE);
        }
        return(status);  // return the status of the child process
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

void Prompt(int status){
    char prompt[100];

    if(WIFEXITED(status)){
        snprintf(prompt, sizeof(prompt), "enseash [exit:%d] %% ", WEXITSTATUS(status));
    } else if(WIFSIGNALED(status)){
        snprintf(prompt, sizeof(prompt), "enseash [sign:%d] %% ", WTERMSIG(status));
    } else {
        snprintf(prompt, sizeof(prompt), "enseash %% ");
    }

    write(STDOUT_FILENO, prompt, strlen(prompt));
}

int main() {
    WelcomeMessage();
    int status = 0;

    while(1){
		Prompt(status);
		char userInput[1024];
		int byteread;

        //read user input
        if((byteread = read(STDIN_FILENO, userInput, sizeof(userInput))) == 0){
            //handle ctrl+d or end of file
            Exit();
        }
		
		// remove '\n' (newline) from the command input
        userInput[strcspn(userInput, "\n")] = 0;	
        
        // if the command is exit
        if (strcmp(userInput, "exit") == 0)  {
			Exit();
        }
        
        // Execute the entered command and store its status
        status = Command(userInput);
	}
}
