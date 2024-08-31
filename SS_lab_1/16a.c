/*
============================================================================
Name : 16a.c
Author : Prabal Singh
Description : Write a program to perform mandatory locking.
a. Implement write lock
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include <unistd.h>


int main()
{
    struct flock l;
    l.l_type=F_WRLCK;
    l.l_pid=getpid();
    l.l_whence=SEEK_SET;
    l.l_start=0;
    l.l_len=0;

    int fd=open("temp1.txt", O_RDWR);
    if(fd==-1)
    {
        perror("error in open");
        return 1;
    }

    printf("Before locking\n");
    fcntl(fd,F_SETLKW, &l);
    printf("Write lock set\n");
    printf("Press enter to unlock.\n");
    getchar();
    l.l_type=F_UNLCK;
    fcntl(fd,F_SETLK, &l);
    printf("Write lock Unlocked\n");
    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Before locking
Write lock set
Press enter to unlock.

Write lock Unlocked

*/