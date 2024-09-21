/*
============================================================================
Name : 28.c
Author : Prabal Singh
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure).
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>   

int main()
{
    struct msqid_ds mbuf;
    key_t key;
    int mqid;

    key=ftok(".",'a');
    mqid=msgget(key, 0);

    msgctl(mqid, IPC_STAT, &mbuf);

    printf("The previous permission was=%o\n", mbuf.msg_perm.mode);

    mbuf.msg_perm.mode=0744;

    msgctl(mqid, IPC_SET, &mbuf);
    printf("The new permission was=%o\n", mbuf.msg_perm.mode);

    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
The previous permission was=777
The new permission was=744

*/