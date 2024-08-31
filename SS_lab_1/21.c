/*
============================================================================
Name : 21.c
Author : Prabal Singh
Description : Write a program, call fork and print the parent and child process id.
Date: 31th Aug, 2024.
============================================================================
*/

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    int par_id=getpid();
    printf("Parent process id= %d\n",par_id);
    int child1=fork();
    if(child1!=0)
    printf("Child process1 id = %d\n",child1);

    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Parent process id= 37084
Child process1 id = 37085

*/