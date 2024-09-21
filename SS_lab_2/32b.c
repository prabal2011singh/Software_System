/*
============================================================================
Name : 32b.c
Author : Prabal Singh
Description : Write a program to implement semaphore to protect any critical section.
 b. protect shared memory from concurrent write access
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/ipc.h>   
#include <sys/sem.h>  
#include <sys/shm.h>   
#include <unistd.h>  
#include <stdio.h>  
#include <sys/types.h> 


int main()
{
    key_t semKey;     
    int semIdentifier; 
    int semctlStatus;  
    int semopStatus;  

    key_t shmKey;       
    int shmIdentifier;   
    ssize_t shmSize = 20; 
    char *shmPointer;


    union semun
    {
        int val;               
        struct semid_ds *buf; 
        unsigned short *array; 
        struct seminfo *__buf; 
    } semSet;

    semKey = ftok(".", 'B');

    semIdentifier = semget(semKey, 1, 0);
    if (semIdentifier == -1)
    {
        semIdentifier = semget(semKey, 1, IPC_CREAT | 0700);
        semSet.val = 1; //binary semaphore
        semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
    }

    shmKey = ftok(".", 'C');

    shmIdentifier = shmget(shmKey, shmSize, IPC_CREAT | 0700);
    shmPointer = shmat(shmIdentifier, (void *)0, 0);

    struct sembuf semOp;
    semOp.sem_num = 0;
    semOp.sem_flg = 0;

    printf("Press enter to lock the critical section!\n");
    getchar();
    
    // Use semaphore to lock the critical section
    semOp.sem_op = -1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    
    printf("Critical Section is now locked!\n");

    printf("Writing to the shared memory!\n");
    sprintf(shmPointer, "Hello");

    printf("Press enter to read from the shared memory!\n");
    getchar();

    printf("%s\n", shmPointer);

    printf("Press enter to exit the critical section!\n");
    getchar();

    // Use semaphore to unlock the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);

    printf("Critical section is now unlocked!\n");
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./32b
Press enter to lock the critical section!

Critical Section is now locked!
Writing to the shared memory!
Press enter to read from the shared memory!

Hello
Press enter to exit the critical section!

Critical section is now unlocked!

*/