/*
============================================================================
Name : 19.c
Author : Prabal Singh
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 31th Aug, 2024.
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
    printf("%d nano sec",nano_sec);
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
25153 nano sec

*/