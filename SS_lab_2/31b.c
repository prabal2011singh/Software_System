/*
============================================================================
Name : 31b.c
Author : Prabal Singh
Description : Write a program to create a semaphore and initialize value to the semaphore.
 a. create a binary semaphore
 b. create a counting semaphore
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() 
{
    key_t key;
    int semid;

    union csem
    {
        int n;
    } csem1;

    key = ftok(".", 'A');
    semid = semget(key, 1, IPC_CREAT | 0777);

    csem1.n=9; //counting semaphore

    semctl(semid, 0, SETVAL, csem1);

    printf("Created counting semaphore with id= %d\n", semid);

    return 0;
}

/*

Output

rabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Created counting semaphore with id= 1
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x61083a83 0          prabal-sin 777        1         
0x41083a83 1          prabal-sin 777        1         

*/