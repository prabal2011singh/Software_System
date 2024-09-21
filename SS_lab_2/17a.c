/*
============================================================================
Name : 17a.c
Author : Prabal Singh
Description : Write a program to execute ls -l | wc.
a. use dup
Date: 17th Sept, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main()
{
    int fd[2];
    pipe(fd);

    if(fork()) //parent process
    {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        execlp("ls", "ls", "-l", NULL);
    }
    else
    {
        close(fd[1]);
        close(0);
        dup(fd[0]);
        execlp("wc", "wc", NULL);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 17a.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
      9      74     514

*/