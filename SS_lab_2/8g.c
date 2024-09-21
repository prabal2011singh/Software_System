/*
============================================================================
Name : 8g.c
Author : Prabal Singh
Description : Write a separate program using signal system call to catch the following signals.
 g. SIGPROF (use setitimer system call
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void handler()
{
    printf("SIGPROF is caught\n");
    _exit(0);
}

int main()
{
    struct itimerval t;

    t.it_value.tv_sec = 1;
    t.it_value.tv_usec = 0;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &t, 0);

    signal(SIGPROF, (void *)handler);
    while(1);
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
SIGPROF is caught

*/
