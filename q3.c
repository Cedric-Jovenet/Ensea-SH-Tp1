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


int main() {
    char command[1024];
    int status;

    // Display welcome message
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
    fflush(stdout);

    while (1) {
        // Display prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));



        // wait for user input 
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; 
        }	
        
         // remove '\n' (newline) from the command input
        command[strcspn(command, "\n")] = 0;	

        // if the command is fortune
        if (strcmp(command, "fortune") ==0 )  {
            write(STDOUT_FILENO, FORTUNE, strlen(FORTUNE));

        }
        
        // if the command is exit
        if (strcmp(command, "exit") ==0 )  {
            write(STDOUT_FILENO, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
			break;
        }
        
        
        else{ 
		execlp(command, command, NULL); //execute the command and write it
		}
		
    }

    return 0;
}
