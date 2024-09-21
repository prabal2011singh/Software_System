/*
============================================================================
Name : 31a.c
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

    union bsem
    {
        int n;
    } bsem1;

    key = ftok(".", 'a');
    semid = semget(key, 1, IPC_CREAT | 0777);

    bsem1.n=1; //binary semaphore

    semctl(semid, 0, SETVAL, bsem1);

    printf("Created binary semaphore with id= %d\n", semid);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Created binary semaphore with id= 0
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x61083a83 0          prabal-sin 777        1    

*/