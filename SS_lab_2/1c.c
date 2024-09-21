/*
============================================================================
Name : 1c.c
Author : Prabal Singh
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and  10micro second
 c. ITIMER_PROF
Date: 21th Sept, 2024.
============================================================================
*/


#include <sys/time.h>
#include <signal.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int ts;
    struct itimerval t1;

    if (argc != 2)
    {
        printf("Pass 1 for 10s timer or 2 for 10ms timer\n");
        _exit(0);
    }

    if((int)(*argv[1] - 48) ==1)
    {
        t1.it_interval.tv_sec=0;
        t1.it_interval.tv_usec=0;
        t1.it_value.tv_sec=10;
        t1.it_value.tv_usec=0;
    }
    else if ((int)(*argv[1] - 48) == 2)
    {

        t1.it_interval.tv_sec=0;
        t1.it_interval.tv_usec=0;
        t1.it_value.tv_sec=0;
        t1.it_value.tv_usec=10000;
    }
    

    ts=setitimer(ITIMER_PROF, &t1,0);
    
    if (ts == -1)
        perror("setitimer");

    while(1);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out 2
Profiling timer expired
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out 1
Profiling timer expired

*/
