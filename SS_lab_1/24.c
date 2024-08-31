/*
============================================================================
Name : 24.c
Author : Prabal Singh
Description : Write a program to create an orphan process.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int child=fork();
    if(child==-1)
    perror("fork");
    
    if(child==0)
    {
        //only child enters
        printf("Child PID=%d\n",getpid());
        printf("putting the child process to sleep\n");
        sleep(10);
        printf("sleep over");
    }
    else if(child!=0)
    {
        //only parent enters
        printf("Parent PID= %d\n",getpid());
        printf("Exiting parent forcefully while child is alive\n");
        _exit(0); 
    }
}

/*
Output
=================================================
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 
Parent PID= 18332
Exiting parent forcefully while child is alive
Child PID=18333
putting the child process to sleep
prabal-singh@prabal-singh-Vostro-5415:~/temp$ sleep over
=================================================
*/