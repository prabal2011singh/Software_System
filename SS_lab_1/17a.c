/*
============================================================================
Name : 17.c
Author : Prabal Singh
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print
the new ticket number then close the file.
(DataBase)
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

    db.tick_no=4;
    int fd = open("db.txt", O_RDWR | O_CREAT, 0777);
    write(fd, &db, sizeof(db));
    close(fd);
    fd=open("db.txt", O_RDONLY);
    read (fd, &db, sizeof(db));
    printf("Ticket Number= %d\n", db.tick_no);
    close(fd);
    return 0;    
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Ticket Number= 4

*/