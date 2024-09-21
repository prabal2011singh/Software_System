/*
============================================================================
Name : 8d.c
Author : Prabal Singh
Description : Write a separate program using signal system call to catch the following signals.
 d. SIGALRM (use alarm system call)
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("SIGALRM is caught\n");
    _exit(0);
}

int main()
{
    
    signal(SIGALRM, (void *)handler);
    printf("Alarm starting\n");
    alarm(1);
    printf("Alarm ending\n");
    pause();
    
    
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Alarm starting
Alarm ending
SIGALRM is caught

*/
