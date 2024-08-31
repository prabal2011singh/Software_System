/*
============================================================================
Name : 25.c
Author : Prabal Singh
Description : Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

int main ()
{
    int c1=fork();
    if(c1==0)
    {
        printf("First child PID= %d\n",c1);
        sleep(5);
        printf("Exiting First child\n");
        exit(0);
    }

    int c2=fork();
    if(c2==0)
    {
        printf("Second child PID= %d\n",c2);
        sleep(10);
        printf("Exiting Second child\n");
        exit(0);
    }

    int c3=fork();
    if(c3==0)
    {
        printf("Third child PID= %d\n",c3);
        sleep(10);
        printf("Exiting Third child\n");
        exit(0);
    }

    printf("parent waiting for first child to change change state\n");
    int status;
    if(c1==waitpid(c1,&status,0))
    {
        if(WIFEXITED(status))
        printf("Child 1 Exited");
        else
        printf("Error");
    }
    
    return 0;
}

/*
Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 
First child PID= 0
Second child PID= 0
parent waiting for first child to change change state
Third child PID= 0
Exiting First child
Child 1 Exited
prabal-singh@prabal-singh-Vostro-5415:~/temp$ Exiting Second child
Exiting Third child

*/