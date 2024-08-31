/*
============================================================================
Name : 4.c
Author : Prabal Singh
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>

int main() 
{
    int fd=open("new_file.txt", O_RDWR);
    if(fd==-1)
    {
        perror("error inopening file\n");
        return 1;
    }

    printf("fd = %d\n",fd);
    close(fd);

    fd=open("new_file.txt", O_CREAT | O_EXCL, 0777);
    if(fd==-1)
    {
        if(errno==EEXIST)
        printf("file exist check with O_EXCL flag\n");
        else
            perror("error in opeing the file");
    }
    else
    printf("File new_file.txt opened successfully\n");
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
fd = 3
file exist check with O_EXCL flag

*/
