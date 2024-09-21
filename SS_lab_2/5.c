/*
============================================================================
Name : 5.c
Author : Prabal Singh
Description : Write a program to print the system limitation of 
 a. maximum length of the arguments to the exec family of functions. 
 b. maximum number of simultaneous process per user id. 
 c. number of clock ticks (jiffy) per second. 
 d. maximum number of open files
 e. size of a page
 f. total number of pages in the physical memory
 g. number of currently available pages in the physical memory.
Date: 21th Sept, 2024.
============================================================================
*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<limits.h>

int main()
{
    
    long int l;

    l=sysconf(_SC_ARG_MAX);
    printf("maximum length of the arguments to the exec family of functions=%ld\n",l);

    l=sysconf(_SC_CHILD_MAX); 
    printf("maximum number of simultaneous process per user id=%ld\n",l);

    l=sysconf(_SC_CLK_TCK);
    printf("number of clock ticks (jiffy) per second=%ld\n",l);

    l=sysconf(_SC_OPEN_MAX); 
    printf("maximum number of open files=%ld\n",l);

    l=sysconf(_SC_PAGESIZE);
    printf("size of a page=%ld\n",l);

    l=sysconf(_SC_PHYS_PAGES); 
    printf("total number of pages in the physical memory=%ld\n",l);

    l=sysconf(_SC_AVPHYS_PAGES);
    printf("number of currently available pages in the physical memory=%ld\n",l);
    
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
maximum length of the arguments to the exec family of functions=2097152
maximum number of simultaneous process per user id=61011
number of clock ticks (jiffy) per second=100
maximum number of open files=1048576
size of a page=4096
total number of pages in the physical memory=3923467
number of currently available pages in the physical memory=1776410

*/