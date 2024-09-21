/*
============================================================================
Name : 32d.c
Author : Prabal Singh
Description : Write a program to implement semaphore to protect any critical section.
 d. remove the created semaphore
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <unistd.h>
#include <stdio.h>

int main()
{
    key_t semKey;
    int semIndentifier;
    int semctlStatus;

    semKey = ftok(".", 'A');
    semIndentifier = semget(semKey, 1, IPC_CREAT | 0777);
    printf("Deleting the semaphore with semid=%d\n",semIndentifier);
    printf("Press enter to delete the semaphore!\n");
    getchar();

    semctlStatus = semctl(semIndentifier, 0, IPC_RMID);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x61083a83 0          prabal-sin 700        1         
0x41083a83 4          prabal-sin 700        1         

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 32d.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Deleting the semaphore with semid=4
Press enter to delete the semaphore!

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x61083a83 0          prabal-sin 700        1         

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ 

*/