/*
============================================================================
Name : 3.c
Author : Prabal Singh
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include<unistd.h>

int main() {
    int fd = creat("new_file.txt", 0777);
    printf("File descriptor: %d\n", fd);
    close(fd);
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
File descriptor: 3

*/