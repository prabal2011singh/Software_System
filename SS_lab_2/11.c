/*
============================================================================
Name : 11.c
Author : Prabal Singh
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    struct sigaction siga;

    siga.sa_handler = SIG_IGN;
    siga.sa_flags=0;

    sigaction(SIGINT, &siga, NULL);
    printf("SIGINT signal is ingnored for 3 seconds... Try to press Ctrl+C\n");
    sleep(3);

    printf("\n");

    siga.sa_handler = SIG_DFL;

    sigaction(SIGINT, &siga, NULL);
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
