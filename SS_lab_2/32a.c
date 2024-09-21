/*
============================================================================
Name : 32a.c
Author : Prabal Singh
Description : Write a program to implement semaphore to protect any critical section.
 a. rewrite the ticket number creation program using semaphore
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/ipc.h>   
#include <sys/sem.h>  
#include <sys/types.h> 
#include <fcntl.h>   
#include <unistd.h>  
#include <stdio.h>   
#include <sys/stat.h>  


int main()
{
    char *ticketFile = "./testfiles/ticket-file.txt"; 

    int fd;   
    ssize_t readBytes, writeBytes;
    off_t lseekOffset;
    int data;

    key_t semKey;    
    int semctlStatus;  
    int semopStatus;  
    int semIdentifier; 
  
    fd = open(ticketFile, O_CREAT | O_RDWR, S_IRWXU);
    if (fd == -1)
    {
        perror("Error while creating / opening the ticket file!");
        _exit(0);
    }

    union semun
    {
        int val;               
        struct semid_ds *buf; 
        unsigned short *array; 
        struct seminfo *__buf;
    } semSet;

    semKey = ftok(".", 'A');

    semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
    if (semIdentifier == -1)
    {
        semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore 

        semSet.val = 1; //binary semaphore
        semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
    }

    struct sembuf semOp;
    semOp.sem_num = 0;
    semOp.sem_flg = 0;

    printf("Press enter to obtain lock on the critical section\n");
    getchar();

    semOp.sem_op = -1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    
    printf("Obtained lock on critical section and now entering critical section!\n");
   
    readBytes = read(fd, &data, sizeof(data));
    if (readBytes == -1)
    {
        perror("Error while reading from ticket file!");
        _exit(0);
    }
    else if (readBytes == 0) // Ticket file has no data
        data = 1;
    else
    {
        data += 1;
        lseekOffset = lseek(fd, 0, SEEK_SET);
    }

    writeBytes = write(fd, &data, sizeof(data));

    printf("Your ticker number is= %d\n", data);
    printf("Press enter to exit from critical section!\n");
    getchar();

    semOp.sem_op = 1;
    semopStatus = semop(semIdentifier, &semOp, 1);
    close(fd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Press enter to obtain lock on the critical section

Obtained lock on critical section and now entering critical section!
Your ticker number is= 1
Press enter to exit from critical section!

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Press enter to obtain lock on the critical section

Obtained lock on critical section and now entering critical section!
Your ticker number is= 2
Press enter to exit from critical section!

*/