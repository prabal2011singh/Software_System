/*
============================================================================
Name : 25.c
Author : Prabal Singh
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
 a. access permission
 b. uid, gid
 c. time of last message sent and received 
 d. time of last change in the message queue
 d. size of the queue
 f. number of messages in the queue 
 g. maximum number of bytes allowed 
 h. pid of the msgsnd and msgrcv
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <time.h>

int main() 
{
    key_t key;
    int mid;
    struct msqid_ds mbuf;

    key = ftok(".", 'a');

    mid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

    msgctl(mid, IPC_STAT, &mbuf);
    printf("Access permissions: %o\n", mbuf.msg_perm.mode);
    printf("UID: %d\n", mbuf.msg_perm.uid);
    printf("GID: %d\n", mbuf.msg_perm.gid);
    printf("Time of last message sent: %ld\n", mbuf.msg_stime);
    printf("Time of last message received: %ld\n", mbuf.msg_rtime);
    printf("Time of last change in the message queue: %s", ctime(&mbuf.msg_ctime));
    printf("Size of the queue: %ld\n", mbuf.__msg_cbytes);
    printf("Number of messages in the queue: %ld\n", mbuf.msg_qnum);
    printf("Maximum number of bytes allowed: %ld\n", mbuf.msg_qbytes);
    printf("PID of msgsnd: %d\n", mbuf.msg_lspid);
    printf("PID of msgrcv: %d\n", mbuf.msg_lrpid);

    msgctl(mid, IPC_RMID, NULL);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Access permissions: 666
UID: 1000
GID: 1000
Time of last message sent: 0
Time of last message received: 0
Time of last change in the message queue: Fri Sep 20 19:45:59 2024
Size of the queue: 0
Number of messages in the queue: 0
Maximum number of bytes allowed: 16384
PID of msgsnd: 0
PID of msgrcv: 0

*/