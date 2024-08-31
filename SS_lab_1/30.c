/*
============================================================================
Name : 30.c
Author : Prabal Singh
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/stat.h>

void makeDaemon(time_t curr, time_t desired) {
        setsid();
        chdir("/");
        umask(0);
        printf("Childs pid is: %d and parents id is: %d\n", getpid(), getppid());
        printf("Deamon created and it will wait till the specific time\n");
        while (difftime(desired, curr) > 0)
        {
                printf("Deamon running...\n");
                sleep(10);
                time(&curr);
        } 
        printf("Script runned at specific time and deamon is exiting\n");
        exit(0);
}

int main() {
        time_t curr, desired;
        struct tm *deadline;
        time(&curr); //getting current time
        deadline = localtime(&curr); // converting to local time

        deadline->tm_hour = 23; //specifying the time
        deadline->tm_min = 58;
        deadline->tm_sec = 0;
        desired = mktime(deadline);

        int value_fork=fork();
        if(!value_fork) {
                makeDaemon(curr, desired);
        }
        else {
                printf("Parent %d is going to exit\n", getpid());
        }
        return 0; 
}

/*

Output


Terminal 1
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Parent 41175 is going to exit
Childs pid is: 41176 and parents id is: 41175
Deamon created and it will wait till the specific time
Deamon running...
prabal-singh@prabal-singh-Vostro-5415:~/temp$ Deamon running...
Deamon running...
Deamon running...
Deamon running...
Deamon running...
Script runned at specific time and deamon is exiting


Terminal 2
top - 23:57:22 up 11:30,  2 users,  load average: 0.55, 0.40, 0.41
Tasks:   1 total,   0 running,   1 sleeping,   0 stopped,   0 zombie
%Cpu(s):  4.1 us,  1.3 sy,  0.0 ni, 94.0 id,  0.0 wa,  0.0 hi,  0.6 si,  0.0 st 
MiB Mem :  15326.1 total,   7084.0 free,   5287.7 used,   3396.4 buff/cache     
MiB Swap:   4096.0 total,   4096.0 free,      0.0 used.  10038.3 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND                                                                                                             
  41176 prabal-+  20   0    2616    768    768 S   0.0   0.0   0:00.00 a.out 

*/