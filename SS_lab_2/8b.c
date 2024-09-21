/*
============================================================================
Name : 8b.c
Author : Prabal Singh
Description : Write a separate program using signal system call to catch the following signals.
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

    signal(SIGINT, (void *)handler);
    raise(SIGINT);
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
SIGINT is caught

*/
