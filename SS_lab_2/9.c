/*
============================================================================
Name : 9.c
Author : Prabal Singh
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler()
{
    printf("SIGINT is caught!\n");
    _exit(0);
}

int main()
{
    signal(SIGINT, SIG_IGN);
    printf("SIGINT signal is ingnored for 3 seconds... Try to press Ctrl+C\n");
    sleep(3);

    printf("\n");
    signal(SIGINT, SIG_DFL);
    printf("SIGINT is activate now... You can press Ctrl+C\n");
    while(1);

    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
SIGINT signal is ingnored for 3 seconds... Try to press Ctrl+C
^C^C
SIGINT is activate now... You can press Ctrl+C
^C

*/
