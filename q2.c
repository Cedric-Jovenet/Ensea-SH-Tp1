#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "enseash % "
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"

int main() {
    char command[1024];

    // Display welcome message
    write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));

    while (1) {
        // Display prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
        fflush(stdout);
       // Read user input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // Exit on Ctrl+D
        }

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            write(STDOUT_FILENO, "Bye bye...\n", 11);
            break; // Exit on "exit"
        }

        // Execute simple command (later updated for arguments)
        int ret = system(command);
        if (ret == -1) {
            perror("enseash");
        }
    }

    return 0;
}
