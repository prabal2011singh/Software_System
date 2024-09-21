/*
============================================================================
Name : 7.c
Author : Prabal Singh
Description : Write a simple program to print the created thread ids.
Date: 17th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *argvp)
{
    printf("Running in thread with thread ID: %lu\n", pthread_self());
    return NULL;
}

int main()
{
    pthread_t th1, th2 ,th3;
    pthread_create(&th1, NULL, thread_func, NULL);
    pthread_create(&th2, NULL, thread_func, NULL);
    pthread_create(&th3, NULL, thread_func, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 7.c -lpthread
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Running in thread with thread ID: 139934969099968
Running in thread with thread ID: 139934952314560
Running in thread with thread ID: 139934960707264

*/

