/*
============================================================================
Name : 13b.c
Author : Prabal Singh
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    
    if (argc != 2)
    {
        printf("Check usage\n");
        _exit(0);
    }

    pid = atoi(argv[1]);

    kill(pid, SIGSTOP);
    printf("SIGSTOP sent to process id= (%d)\n", pid);
    
    return 0;  
}


/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./13b 27347
SIGSTOP sent to process id= (27347)


 */