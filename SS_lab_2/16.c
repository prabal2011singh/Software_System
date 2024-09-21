/*
============================================================================
Name : 16.c
Author : Prabal Singh
Description : Write a program to send and receive data from parent to child vice versa. Use two way
communication.
Date: 17th Sept, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //for pipe()

int main()
{
    int fd[2]; // for parent(write) to child(read) communication
    char buf1[20];
    pipe(fd);
    int fd2[2]; //for child(write) to parent(read) communication
    char buf2[20];
    pipe(fd2);

    if(!fork())  //child process
    {
        close(fd[1]);
        close(fd2[0]);
        read(fd[0],buf1, sizeof(buf1));
        printf("The msg from parent is= %s\n",buf1);
        printf("Enter the msg from child to parent= \n");
        scanf(" %[^\n]",buf2);
        write(fd2[1], buf2, sizeof(buf2));
        
    }
    else  //parent process
    {
        close(fd[0]);
        close(fd2[1]);
        printf("Enter the msg from parent to child= \n");
        scanf(" %[^\n]",buf1);
        write(fd[1], buf1, sizeof(buf1));
        read(fd2[0],buf2, sizeof(buf2));
        printf("The msg from parent is= %s\n",buf2);
        //close(fd[1]);
    }
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the msg from parent to child= 
hi from parent
The msg from parent is= hi from parent
Enter the msg from child to parent= 
hi from child
The msg from parent is= hi from child

*/