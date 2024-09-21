/*
============================================================================
Name : 13a.c
Author : Prabal Singh
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    printf("SIGALRM is caught\n");
    _exit(0);
}

int main()
{
    pid_t pid = getpid();
    printf("Process ID: %d\n", pid);
    signal(SIGSTOP, (void *)handler);

    printf("Putting to sleep\n");
    sleep(10);
    
    
    return 0;  
}


/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./13a
Process ID: 27347
Putting to sleep

[1]+  Stopped                 ./13a


 */