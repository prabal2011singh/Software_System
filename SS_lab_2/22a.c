/*
============================================================================
Name : 22a.c
Author : Prabal Singh
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
Date: 19th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buf[100];

    printf("Enter the msg in FIFO =\n");
    scanf(" %[^\n]" , buf);
    int fd=open("myfifo", O_WRONLY);  //blocking call that's why written below
    write(fd, buf, sizeof(buf));
    close(fd);
    return 0;
}

/*

Output

Terminal 1
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./22b
Waiting for data on FIFO...
Data is available
The msg in FIFO is =hello


Terminal 2
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 22a.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the msg in FIFO =
hello

*/