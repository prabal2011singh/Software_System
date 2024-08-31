/*
============================================================================
Name : 23.c
Author : Prabal Singh
Description : Write a program to create a Zombie state of the running program.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    int child=fork();

    if(child!=0)
    {
        //only parent enters
        printf("Parent PID= %d\n",getpid());
        printf("Putting parent to sleep\n");
        sleep(30);
        printf("Sleep over\n");
    }
    else if(child==0)
    {
        //only child enters
        printf("Child PID=%d\n",getpid());
        printf("Exiting child forcefully while parent is sleeping\n");
        _exit(0);
    }
}


/*

=====================================================================================================
Terminal 1
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out &
[1] 14655
prabal-singh@prabal-singh-Vostro-5415:~/temp$ Parent PID= 14655
Putting parent to sleep
Child PID=14656
Exiting child forcefully while parent is sleeping
Sleep over


Terminal 2
prabal-singh@prabal-singh-Vostro-5415:~$ ps -ux | grep a.out
prabal-+   14655  0.0  0.0   2616  1280 pts/1    S    15:48   0:00 ./a.out
prabal-+   14656  0.0  0.0      0     0 pts/1    Z    15:48   0:00 [a.out] <defunct>
prabal-+   14665  0.0  0.0   9204  2304 pts/2    S+   15:48   0:00 grep --color=auto a.out
prabal-singh@prabal-singh-Vostro-5415:~$

======================================================================================================

*/