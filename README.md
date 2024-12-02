# Ensea-SH-Tp1
Synthesis Practical Work

## Question 1
- char command[1024];
  - Command buffer: A character array to store user input. It can hold up to 1023 characters plus a null terminator (\0).


write(STDOUT_FILENO, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE));
  - Display welcome message
  - write: Writes directly to the standard output (STDOUT_FILENO).
  - WELCOME_MESSAGE: The welcome text to be displayed.
  - strlen(WELCOME_MESSAGE): Calculates the length of the message to know how many bytes to write.


## Question 2
