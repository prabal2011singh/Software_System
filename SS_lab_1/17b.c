/*
============================================================================
Name : 17.c
Author : Prabal Singh
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include <unistd.h>

int main()
{
    struct
    {
        int tick_no;
    } db;

    struct flock l;
    int fd=open("db.txt",O_RDWR);

    printf("Entering critical section");

    fcntl(fd, F_SETLKW, &l);
    printf("Inside critical section");

    read(fd, &db, sizeof(db));
    printf("Curretn ticket number =%d\n",db.tick_no);

    printf("Incrementing Ticket number\n");
    db.tick_no++;

    lseek(fd,0,SEEK_SET);
    write(fd,&db , sizeof(db));
    printf("New ticket number =%d\n",db.tick_no);
    printf("Press enter to unlock\n");
    getchar();

    l.l_type=F_UNLCK;
    fcntl(fd, F_SETLK, &l);
    printf("Unlocked\n");
    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Entering critical sectionInside critical sectionCurretn ticket number =4
Incrementing Ticket number
New ticket number =5
Press enter to unlock

Unlocked
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Entering critical sectionInside critical sectionCurretn ticket number =5
Incrementing Ticket number
New ticket number =6
Press enter to unlock

Unlocked

*/