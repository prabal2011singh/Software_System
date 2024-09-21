/*
============================================================================
Name : 14.c
Author : Prabal Singh
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.
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
    char buf2[10];
    pipe(fd);

    printf("Enter the msg= \n");
    scanf(" %[^\n]",buf1);
    int letters_written=write(fd[1], buf1, sizeof(buf1));

    read(fd[0],buf2,letters_written);
    printf("The msg is = \n");
    printf("%s\n",buf2);
    return 0;

}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 14.c
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./a.out
Enter the msg= 
Hello
The msg is = 
Hello

*/