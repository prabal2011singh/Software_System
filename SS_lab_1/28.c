/*
============================================================================
Name : 28.c
Author : Prabal Singh
Description : Write a program to get maximum and minimum real time priority.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sched.h>

int main()
{
    printf("Max real time priority = %d\n",sched_get_priority_max(SCHED_FIFO));
    printf("Min real time priority = %d\n",sched_get_priority_min(SCHED_FIFO));
    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 
Max real time priority = 99
Min real time priority = 1

*/