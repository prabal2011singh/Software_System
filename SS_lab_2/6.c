/*
============================================================================
Name : 6.c
Author : Prabal Singh
Description : Write a simple program to create three threads.
Date: 17th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *argvp)
{
    printf("Printing from Thread\n");
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
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 6.c -lpthread
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Printing from Thread
Printing from Thread
Printing from Thread

*/