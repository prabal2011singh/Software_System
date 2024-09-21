/*
============================================================================
Name : 2.c
Author : Prabal Singh
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/time.h>  
#include <sys/resource.h> 
#include <stdio.h>      

int main()
{
    int status;                 
    struct rlimit rl;

    getrlimit(RLIMIT_CPU, &rl);
    printf("soft limit of CPU time =%ld\n",rl.rlim_cur);
    printf("hard limit of CPU time =%ld\n",rl.rlim_max);

    getrlimit(RLIMIT_AS, &rl);
    printf("soft limit of virtual process memory =%ld\n",rl.rlim_cur);
    printf("hard limit of virtual process memory =%ld\n",rl.rlim_max);

    getrlimit(RLIMIT_NOFILE, &rl);
    printf("soft limit of file descriptor =%ld\n",rl.rlim_cur);
    printf("hard limit of file descriptor =%ld\n",rl.rlim_max);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
soft limit of CPU time =-1
hard limit of CPU time =-1
soft limit of virtual process memory =-1
hard limit of virtual process memory =-1
soft limit of file descriptor =1048576
hard limit of file descriptor =1048576

*/