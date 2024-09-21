/*
============================================================================
Name : 24.c
Author : Prabal Singh
Description : Write a program to create a message queue and print the key and message queue id.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main()
{
    key_t key;
    int msgid;

    key=ftok(".",'a');
    msgid=msgget(key, IPC_CREAT | IPC_EXCL | 0777);
    printf("The key of message queue is = %0x\n", key);
    printf("The message queue id is = %d\n", msgid);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 24.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
The key of message queue is = 61083a83
The message queue id is = 0
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x61083a83 0          prabal-sin 777        0            0           

*/