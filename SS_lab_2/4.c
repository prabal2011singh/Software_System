/*
============================================================================
Name : 4.c
Author : Prabal Singh
Description : Write a program to measure how much time is taken to execute 100 getppid ( ) 
system call. Use time stamp counter. 
Date: 21th Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

unsigned long long rdtsc()
{
    unsigned long long dst;
    __asm__ __volatile__ ("rdtsc":"=A" (dst));
}

int main ()
{
    unsigned long long int start, end;
    start=rdtsc();
    for(int i=0;i<=100;i++)
    {
        getppid();
    }
    end=rdtsc();
    int nano_sec=(end-start)/4.09;
    printf("%d nano sec\n",nano_sec);
    return 0;
}


/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
25169 nano sec

*/