/*
============================================================================
Name : 3.c
Author : Prabal Singh
Description : .Write a program to set (any one) system resource limit. Use setrlimit system call. 
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>

int main()
{
    struct rlimit rl;

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("old soft limit of file descriptor =%ld\n",rl.rlim_cur);
    printf("old hard limit of file descriptor =%ld\n",rl.rlim_max);

    rl.rlim_cur=4;
    rl.rlim_max=1024;

    setrlimit(RLIMIT_NOFILE, &rl);

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("old soft limit of file descriptor =%ld\n",rl.rlim_cur);
    printf("old hard limit of file descriptor =%ld\n",rl.rlim_max);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
old soft limit of file descriptor =1048576
old hard limit of file descriptor =1048576
old soft limit of file descriptor =4
old hard limit of file descriptor =1024

*/