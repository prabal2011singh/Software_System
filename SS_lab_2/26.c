/*
============================================================================
Name : 26.c
Author : Prabal Singh
Description : Write a program to send messages to the message queue. Check $ipcs -q
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
    struct msgque
    {
        long int m_type;
        char msg[100];
    } msgq;

    key_t key;
    int mqid;

    key=ftok(".",'a');
    mqid=msgget(key, 0);

    printf("Enter the message type =");
    scanf("%ld", &msgq.m_type);
    printf("Enter the message\n");
    scanf(" %[^\n]", msgq.msg);

    int n = strlen(msgq.msg);
    msgsnd(mqid, &msgq, n + 1, 0);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the message type =2
Enter the message
Hello
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ipcs -q 

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x61083a83 0          prabal-sin 777        6            1           
          

*/