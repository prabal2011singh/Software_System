
/*
============================================================================
Name : 18.c
Author : Prabal Singh
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
Date: 18th Sept, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main()
{
    int fd[2];
    int fd2[2];

    pipe(fd);
    pipe(fd2);

    if(fork()) //parent process
    {
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ls", "ls", "-l", NULL);
    }
    else
    {
        if(fork())
        {
            close(fd[1]);
            dup2(fd[0],0);
            close(fd2[0]);
            dup2(fd2[1],1);
            execlp("grep", "grep", "^d", NULL);
        }
        else
        {
            close(fd2[1]);
            dup2(fd2[0],0);
            close(fd[0]);
            close(fd[1]);
            execlp("wc", "wc", NULL);
        }
        
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 18.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
      0       0       0
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ls -l | grep ^d | wc
      0       0       0
      
*/