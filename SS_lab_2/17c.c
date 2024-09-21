/*
============================================================================
Name : 17c.c
Author : Prabal Singh
Description : Write a program to execute ls -l | wc.
c. use fcntl
Date: 17th Sept, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    int fd[2];
    pipe(fd);

    if(fork()) //parent process
    {
        close(fd[0]);
        close(1);
        fcntl(fd[1],F_DUPFD,1);
        execlp("ls", "ls", "-l", NULL);
    }
    else
    {
        close(fd[1]);
        close(0);
        fcntl(fd[0],F_DUPFD,0);
        execlp("wc", "wc", NULL);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 17c.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
     10      83     578

*/