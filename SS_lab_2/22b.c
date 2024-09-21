/*
============================================================================
Name : 22b.c
Author : Prabal Singh
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
Date: 19th Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
    char buf[100];
    fd_set rfds;
    struct timeval t;

    printf("Waiting for data on FIFO...\n");

    int fd=open("myfifo", O_RDONLY | O_NONBLOCK); //writing O_NONBLOCK is important!!
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    
    t.tv_sec=10;
    t.tv_usec=0;
    
    int temp=select(fd+1, &rfds, NULL, NULL, &t);
    if(!temp)
    {
        printf("No data is available in FIFO within 10 seconds\n");
    }
    else
    {
        printf("Data is available\n");
        read(fd, buf, sizeof(buf));
        printf("The msg in FIFO is =%s\n", buf);
    }  
    close(fd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 22b.c -o 22b
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./22b
Waiting for data on FIFO...
No data is available in FIFO within 10 seconds

*/