/*
============================================================================
Name : 10b.c
Author : Prabal Singh
Description : Write a separate program using sigaction system call to catch the following signals.
b. SIGINT
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("SIGINT is caught\n");
    _exit(0);
}

int main()
{
    struct sigaction siga;

    siga.sa_handler = handler;
    siga.sa_flags=0;

    sigaction(SIGINT, &siga, NULL);
    printf("Infinite loop has started... You can press Ctrl+C\n");
    while(1);
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Infinite loop has started... You can press Ctrl+C
^CSIGINT is caught

*/
