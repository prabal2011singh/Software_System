/*
============================================================================
Name : 18.c
Author : Prabal Singh
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
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
    } db;
    struct flock l;
    
    printf("Enter record number (1,2,3)\n");
    int n;
    scanf("%d", &n);
    getchar();
    int fd=open("record.txt",O_RDWR);

    l.l_type=F_WRLCK;
    l.l_whence=SEEK_SET;
    l.l_start=sizeof(db)*(n-1);
    l.l_len=sizeof(db);
    l.l_pid=getpid();

    lseek(fd,l.l_start, SEEK_SET);
    read(fd, &db, sizeof(db));

    printf("Locking record %d\n",n);
    fcntl(fd,F_SETLKW, &l);
    printf("Locked\n");

    printf("Current record count= %d\n",db.r_read_count);
    db.r_read_count++;

    lseek(fd, -1*sizeof(db), SEEK_CUR);
    write(fd, &db, sizeof(db));
    printf("Press enter to unlock record\n");
    getchar();
    
    l.l_type=F_UNLCK;
    fcntl(fd, F_SETLK, &l);
    printf("Record unlocked\n");

    return 0;

}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Enter record number (1,2,3)
1
Locking record 1
Locked
Current record count= 0
Press enter to unlock record

Record unlocked
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Enter record number (1,2,3)
1
Locking record 1
Locked
Current record count= 1
Press enter to unlock record

Record unlocked
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Enter record number (1,2,3)
3
Locking record 3
Locked
Current record count= 0
Press enter to unlock record

Record unlocked
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Enter record number (1,2,3)
3
Locking record 3
Locked
Current record count= 1
Press enter to unlock record

Record unlocked

*/