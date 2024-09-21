/*
============================================================================
Name : 29.c
Author : Prabal Singh
Description : Write a program to remove the message queue.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
  

int main()
{

    key_t key;
    int mqid;

    key=ftok(".",'a');
    mqid=msgget(key, 0);

    msgctl(mqid, IPC_RMID, NULL);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 29.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ 

*/