/*
============================================================================
Name : 8a.c
Author : Prabal Singh
Description : Write a separate program using signal system call to catch the following signals.
 a. SIGSEGV
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("SIGSEGV is caught\n");
    _exit(0);
}

int main()
{

    signal(SIGSEGV, (void *)handler);
    raise(SIGSEGV); //SIGSEGV is for segmentation fault
    return 0;  
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
SIGSEGV is caught

*/
