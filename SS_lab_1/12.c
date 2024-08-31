/*
============================================================================
Name : 12.c
Author : Prabal Singh
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main (int argc, char *argv[])
{
    if(argc!=2)
    {
        perror("enter file name");
        return 1;
    }

    int fd=open(argv[1], O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        return 1;
    }

    int f=fcntl(fd, F_GETFL);
    if(f==-1)
    {
        perror("fcntl");
        close(fd);
        return 1;
    }

    printf("Opening mode of file is %d\n",f);

    if(f==32768)
    printf("the file is opened in read only mode\n");

    if(f==32769)
    printf("the file is opened in write only mode\n");
    
    if(f==32770)
    printf("the file is opened in read write mode\n");

    close(fd);
    return 0;
}

/*
Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out temp1.txt
Opening mode of file is 32768
the file is opened in read only mode

 */