# Ensea-SH-Tp1
Synthesis Practical Work

Cédric JOVENET, Marin KERBORIOU 2G1 TP3


 ## Objectives : Develop a tiny shell, that displays exit codes and execution times of launched programs

 We created this shell progresively by following the questions of the TP, therefore the last version of our shell corresponds to the source code file named with the most avanced question number (q7.c).

## Setting up the shell

### installing packages

- Install fortune package : 

 `sudo apt-get install fortune-mod fortunes`


### Compilation and lunch

 - To compile the shell, go to the project directory and use : 
 
 `gcc -o name_of_executable_file qX.c`
 
then, to lunch the shell : 

`./name_of_executable_fil`

## How to use this shell 

You can use any command, with parameters or not. In the next prompt, the shell will show the exit code or exit signal along with the time it took to execute the previous command.
The shell also shows custom welcome and exit messages.
```
Welcome to ENSEA Tiny Shell.
Type 'exit' to quit.
enseash [exit:0|0ms] % fortune
Perfection is reached, not when there is no longer anything to add, but
when there is no longer anything to take away.
-- Antoine de Saint-Exupery
enseash [exit:0|63ms] % exit
Bye bye ...`
```

#### List of other functionalities

- You can exit the shell with `crtl + d`
- `stdin` can be typed as `>`; `stdout` as `<`

```
Welcome to ENSEA Tiny Shell.
Type 'exit' to quit.
enseash [exit:0|0ms] % ls > file.txt             
enseash [exit:0|21ms] % wc -l < file.txt
13
enseash [exit:0|11ms] %
```

- 
