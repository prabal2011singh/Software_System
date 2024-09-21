/*
============================================================================
Name : 32c.c
Author : Prabal Singh
Description : Write a program to implement semaphore to protect any critical section.
 c. protect multiple pseudo resources ( may be two) using counting semaphore
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/ipc.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <stdio.h> 
#include <sys/sem.h> 
#include <sys/types.h>
#include <fcntl.h>  


int main()
{
    char *filename = "./testfiles/ticket-file.txt"; // File name of the sample file

    int fd;            
    ssize_t readBytes; 
    off_t lseekOffset;
    char data[1000];

    key_t semKey;     
    int semIdentifier; 
    int semctlStatus; 
    int semopStatus;  

    fd = open(filename, O_RDONLY );

    union semun
    {
        int val;             
    } semSet;

    semKey = ftok(".", 'a');

    semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
    if (semIdentifier == -1)
    {
        semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore 
        semSet.val = 2; // counting semaphore
        semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
    }

    struct sembuf semOp; // Defines the operation on the semaphore
    semOp.sem_num = 0;
    semOp.sem_flg = IPC_NOWAIT;

    printf("Press enter to obtain lock for the critical section!\n");
    getchar();

    semOp.sem_op = -1;
    semopStatus = semop(semIdentifier, &semOp, 1);

    printf("Obtained lock on critical section and now entering critical section!\n");

    readBytes = read(fd, &data, 1000);
    if (readBytes == -1)
    {
        perror("Error while reading from sample file!");
        _exit(0);
    }
    else if (readBytes == 0) // Sample file has no data
        printf("No data exists!\n");
    else
        printf("Sample data: \n %s", data);

    printf("\n\nPress enter key to exit critical section!\n");
    getchar();

    // Use semaphore to unlock the critical section
    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);

    close(fd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Press enter to obtain lock for the critical section!

Obtained lock on critical section and now entering critical section!
Sample data: 
 3

Press enter key to exit critical section!

*/