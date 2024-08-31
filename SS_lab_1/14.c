/*
============================================================================
Name : 14.c
Author : Prabal Singh
Description : Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        perror("enter file name");
        return 1;
    }

    struct stat st;
    int rvstat=stat(argv[1], &st);
    if(rvstat==-1)
    {
        perror("stat");
        return 1;
    }

    if(S_ISREG(st.st_mode))
        printf("regular file");
    else if(S_ISDIR(st.st_mode))
        printf("directory");
    else if(S_ISCHR(st.st_mode))
        printf("Character device file");
    else if(S_ISBLK(st.st_mode))
        printf("Block device file");
    else if(S_ISFIFO(st.st_mode))
        printf("Fifo file");
    else
        printf("Special File");

    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out temp1.txt
regular file

*/