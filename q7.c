#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

#define PROMPT "enseash % "
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define EXIT_MESSAGE "Bye bye ...\n"
#define DIVISION_NS 1000000    // Division factor for execution time ns
#define DIVISION_S 1000        // Division factor for execution time s

int Command(char *command, int *status, long *exec_time){
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    char *input_file = NULL;
    char *output_file = NULL;
    char *args[100]; 
    int i = 0;
    char *token = strtok(command, " ");  // Tokenize the command input by spaces
    
    // Parse the command input and detect redirections
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");  // Get the next token (input file)
            input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");  // Get the next token (output file)
            output_file = token;
        } else {
            args[i++] = token;  // Store command/argument
        }
        token = strtok(NULL, " ");  // Move to the next token
    }
    args[i] = NULL;  // Null-terminate the argument list

    pid_t pid = fork();
    if (pid == -1) {  
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        // Handle input redirection if specified
        if (input_file) {
            int in_fd = open(input_file, O_RDONLY);
            if (in_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO);  // Redirect stdin to input file
            close(in_fd);  // Close the file descriptor
        }
        
		// Handle output redirection if specified
        if (output_file) {
            int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO);  // Redirect stdout to output file
            close(out_fd);  // Close the file descriptor
        }
        
        // Execute the command with arguments
        execvp(args[0], args);
        perror("execvp");  // Error if execvp fails
        exit(EXIT_FAILURE);
	} else {  // Parent process
        wait(status);  // Wait for child to finish 
        clock_gettime(CLOCK_MONOTONIC, &end);  // end timer
        *exec_time = (end.tv_sec - start.tv_sec) * DIVISION_S + (end.tv_nsec - start.tv_nsec) / DIVISION_NS;  // milliseconds
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
