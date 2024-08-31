/*
============================================================================
Name : 26.c
Author : Prabal Singh
Description : Write a program to execute an executable program.
a. use some executable program
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *narg[] = { NULL, "hello", "world", NULL };
    char *nenviron[] = { NULL };
    narg[0] = argv[1];
    execve(argv[1], narg, nenviron);
    perror("execve");   
    return 0;
}


/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ cc 21.c -o 21
prabal-singh@prabal-singh-Vostro-5415:~/temp$ cc 26.c -o 26
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./26 ./21
Parent process id= 11359
Child process1 id = 11360

*/
