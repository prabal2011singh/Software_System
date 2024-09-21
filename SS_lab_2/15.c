/*
============================================================================
Name : 15.c
Author : Prabal Singh
Description : Write a simple program to send some data from parent to the child process.
Date: 17th Sept, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //for pipe()

int main()
{
    int fd[2];
    char buf1[10];
    pipe(fd);

    if(!fork())  //child process
    {
        close(fd[1]);
        read(fd[0],buf1, sizeof(buf1));
        printf("The msg is= %s\n",buf1);
        //close(fd[0]);
    }
    else  //parent process
    {
        close(fd[0]);
        printf("Enter the msg= \n");
        scanf(" %[^\n]",buf1);
        write(fd[1], buf1, sizeof(buf1));
        //close(fd[1]);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 15.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the msg= 
hi
The msg is= hi

*/