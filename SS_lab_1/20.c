/*
============================================================================
Name : 20.c
Author : Prabal Singh
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include<sys/resource.h>

int main()
{
    int p=getpriority(PRIO_PROCESS,0); // 0 for current process else give process_id
    printf("Current Priority = %d\n",p);
    nice(10);
    int q=getpriority(PRIO_PROCESS,0);
    printf("New Priority = %d\n",q);
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Current Priority = 0
New Priority = 10

*/