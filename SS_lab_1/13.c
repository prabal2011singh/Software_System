/*
============================================================================
Name : 13.c
Author : Prabal Singh
Description : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/select.h>

int main()
{
    fd_set rdfds;
    struct timeval tv;
    int rv;

    FD_ZERO(&rdfds);
    FD_SET(0,&rdfds);

    tv.tv_sec=10;
    tv.tv_usec=0;

    rv=select(1,&rdfds, NULL, NULL, &tv);

    if(rv==-1)
    {
        perror("select");
        return 1;
    }
    else if(rv==0)
    {
        printf("No data available\n");
    }
    else
    {
        printf("Data available\n");
        char buf[1024];
        int rd=read(0,buf,1024);
        if(rd!=-1)
        printf("Data is %s\n", buf);
    }
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Hellooo
Data available
Data is Hellooo
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
No data available
*/