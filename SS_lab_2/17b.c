/*
============================================================================
Name : 17b.c
Author : Prabal Singh
Description : Write a program to execute ls -l | wc.
b. use dup2
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
        dup2(fd[1],1);
        execlp("ls", "ls", "-l", NULL);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0],0);
        execlp("wc", "wc", NULL);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 17b.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
      9      74     514

*/