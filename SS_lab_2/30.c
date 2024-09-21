/*
============================================================================
Name : 30.c
Author : Prabal Singh
Description : Write a program to create a shared memory.
 a. write some data to the shared memory
 b. attach with O_RDONLY and check whether you are able to overwrite.
 c. detach the shared memory
 d. remove the shared memor
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    char *shmp;
    key_t key;
    int shmid;

    key=ftok(".",'a');

    shmid=shmget(key, 1024, IPC_CREAT |0777);
    shmp=shmat(shmid, NULL, 0);

    strcpy(shmp,"message from shm");
    
    char *data = shmat(shmid, 0, 0);
    printf("Text from shared memory: %s\n", data);
    shmp=shmat(shmid, NULL, O_RDONLY);

    if(strcpy(shmp, "this should fail\n")==NULL)
    {
        perror("strcpy in O_RDONLY shared memory\n");
    }

    shmdt(shmp);

    shmctl(shmid, IPC_RMID, NULL);

    printf("Shared memory operations completed\n");

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Text from shared memory: message from shm
Shared memory operations completed

*/