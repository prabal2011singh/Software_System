/*
============================================================================
Name : 8c.c
Author : Prabal Singh
Description : Write a separate program using signal system call to catch the following signals.
 c. SIGFPE
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("SIGFPE is caught\n");
    _exit(0);
}

int main()
{

    signal(SIGFPE, (void *)handler);
    raise(SIGFPE); // SIGFPE is for floating point error
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
SIGFPE is caught

*/
