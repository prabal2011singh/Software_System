/*
============================================================================
Name : 11.c
Author : Prabal Singh
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
a. use dup
b. use dup2
c. use fcntl
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    int fd, nfd1, nfd2;
    char buf[]="abcdefgh";

    fd=open(argv[1], O_RDWR | O_CREAT, 0777);
    if(fd==-1)
    {
        perror("open");
        return 1;
    }

    nfd1=dup(fd);
    if(nfd1 == -1)
    {
        close(fd);
        perror("dup");
        return 1;
    }

    nfd2=dup2(fd,6);
    if(nfd2==-1)
    {
        close(fd);
        perror("dup2");
        return 1;
    }

    int nfd3=fcntl(fd,F_DUPFD, 5);


    write(fd,buf,sizeof(buf));
    write(nfd1,buf,sizeof(buf));
    write(nfd2,buf,sizeof(buf));
    write(nfd3,buf,sizeof(buf));
    close(fd);
    close(nfd1);
    close(nfd2);
    close(nfd3);
    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ cat temp1.txt
abcdefghabcdefghabcdefghabcdefgh

*/