/*
============================================================================
Name : 12.c
Author : Prabal Singh
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include <signal.h>
#include <stdlib.h>

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
        sleep(5);
        kill(getppid(), SIGKILL);
        printf("Exiting parent forcefully while child is alive\n");
        sleep(2);
        printf("child sleep over\n");
    }
    else if(child!=0)
    {
        //only parent enters
        printf("Parent PID= %d\n",getpid());
        while(1);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 12.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Parent PID= 26813
Child PID=26814
putting the child process to sleep
Exiting parent forcefully while child is alive
Killed
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ child sleep over

 */