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
		
        // Modifier le prompt pour afficher l'exit status précédent
        char prompt_with_status[1024];
        snprintf(prompt_with_status, sizeof(prompt_with_status), "enseash [exit: %d] %% ", status);

        // Afficher le prompt modifié
        write(STDOUT_FILENO, prompt_with_status, strlen(prompt_with_status));


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
        
        
        else{ // for general commands //should not exit the shell after this
		execlp(command, command, NULL); //execute the command and write it
		}
		
		// Si le processus s'est terminé normalement
       if (WIFEXITED(status)) {
            status = WEXITSTATUS(status); // Récupérer le exit status
       } else if (WIFSIGNALED(status)) {
             status = WTERMSIG(status); // Récupérer le signal de terminaison
       }
		
    }

    return 0;
}
