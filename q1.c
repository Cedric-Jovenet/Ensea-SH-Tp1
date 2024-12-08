#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROMPT "enseash % "
#define WELCOME_MESSAGE "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"

void WelcomeMessage(){
    //Display of a welcome message
    ssize_t ByteWrite = write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
}

void Prompt(){
    //Display of a simple prompt
    ssize_t ByteWrite = write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

int main(){
    WelcomeMessage();
    Prompt();
}
