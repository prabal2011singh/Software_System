/*
============================================================================
Name : 27b.c
Author : Prabal Singh
Description : Write a program to receive messages from the message queue.
 b. with IPC_NOWAIT as a flag
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
    int res=msgrcv(mqid, &msgq, sizeof(msgq.msg), msgq.m_type, IPC_NOWAIT);
    if(res==-1)
    perror("msgrcv");
    printf("The message type is= %ld\n", msgq.m_type);
    printf("The message is= %s\n", msgq.msg);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 27.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the message type =2
The message type is= 2
The message is= Hello
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the message type =3
The message type is= 3
The message is= hello3

 */