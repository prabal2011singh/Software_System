/*
============================================================================
Name : 23.c
Author : Prabal Singh
Description : Write a program to print the maximum number of files can be opened within a process and
size of a pipe (circular buffer).
Date: 20th Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <limits.h>

int main()
{
    struct rlimit r1;

    getrlimit(RLIMIT_NOFILE, & r1);
    printf("Maximum no. of files that can be opened are= %ld\n", r1.rlim_max);

    long pb=pathconf(".", _PC_PIPE_BUF);
    printf("Maximum pipe buffer size = %ld\n", pb);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Maximum no. of files that can be opened are= 1048576
Maximum pipe buffer size = 4096

*/