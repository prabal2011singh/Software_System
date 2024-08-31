/*
============================================================================
Name : 29.c
Author : Prabal Singh
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<sched.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int curpol=sched_getscheduler(0);
    struct sched_param sp;

    sp.sched_priority=1;
    int newp;

    switch(curpol)
    {

        case SCHED_FIFO:
            printf("Current policy is FIFO\n");
            sched_setscheduler(0,SCHED_RR, &sp);
            newp=sched_getscheduler(0); 
            break;
        case SCHED_RR:
            printf("Current policy is RR\n");
            sched_setscheduler(0,SCHED_FIFO, &sp);
            newp=sched_getscheduler(0); 
            break;
        case SCHED_OTHER:
            printf("Current policy is Other\n");
            sched_setscheduler(0,SCHED_FIFO, &sp);
            newp=sched_getscheduler(0); 
            break;
        default:
        perror("Error in getting current process policy");
    }

    switch (newp)
    {
    case 0:
        printf("New policy is Other.\n");
        break;
    case 1:
        printf("New policy is FIFO.\n");
        break;
    case 2:
        printf("New policy is Round Robin.\n");
        break;
    default:
        perror("sched_getscheduler");
        break;
    }
    return 0;
}


/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ sudo ./a.out
Current policy is Other
New policy is FIFO.
prabal-singh@prabal-singh-Vostro-5415:~/temp$ 

*/