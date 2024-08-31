/*
============================================================================
Name : 18.c
Author : Prabal Singh
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
(Database)
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
        int record_no;
        int r_read_count;
    } db[3];

    for(int i=0;i<3;i++)
    {
        db[i].record_no=i+1;
        db[i].r_read_count=0;
    }
    int fd = open("record.txt", O_RDWR | O_CREAT, 0777);
    write(fd, db, sizeof(db));
    close(fd);
    return 0;
}