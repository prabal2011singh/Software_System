/*
============================================================================
Name : 16b.c
Author : Prabal Singh
Description : Write a program to perform mandatory locking.
b. Implement read lock
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
    l.l_type=F_RDLCK;
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
    printf("Read lock set\n");
    printf("Press enter to unlock.\n");
    getchar();
    l.l_type=F_UNLCK;
    fcntl(fd,F_SETLK, &l);
    printf("Read lock Unlocked\n");
    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Before locking
Read lock set
Press enter to unlock.

Read lock Unlocked

*/