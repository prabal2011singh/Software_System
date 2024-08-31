/*
============================================================================
Name : 6.c
Author : Prabal Singh
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 31th Aug, 2024.
============================================================================
*/

#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
    char buf[10];
    read(0, buf, 10);
    write(1, buf, 10);
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Hello 
Hello

*/