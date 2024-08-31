/*
============================================================================
Name : 22.c
Author : Prabal Singh
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file.
Date: 31th Aug, 2024.
============================================================================
*/

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        perror("enter file name");
        return 1;
    }

    int fd=open(argv[1], O_RDWR );
    if(fd==-1)
    {
        perror("open");
        return 1;
    }

    fork();
    char buf[]="HelloWorld";
    write(fd,buf,10);


    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out temp1.txt
prabal-singh@prabal-singh-Vostro-5415:~/temp$ cat temp1.txt
HelloWorldHelloWorld

*/