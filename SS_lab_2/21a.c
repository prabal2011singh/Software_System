/*
============================================================================
Name : 21a.c
Author : Prabal Singh
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 19th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main()
{
    char buf[100];
    char buf2[100];

    printf("Enter the msg in FIFO 1 =\n");
    scanf(" %[^\n]" , buf);
    int fd=open("myfifo", O_WRONLY);  //blocking call that's why written below
    write(fd, buf, sizeof(buf));
    close(fd);

    int fd2=open("myfifo2", O_RDONLY);
    read(fd2, buf2, sizeof(buf2));
    printf("The msg in FIFO 2 is =%s\n", buf2);
    close(fd2);
    
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 21a.c -o 21a
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./21a
Enter the msg in FIFO 1 =
FIFO 1
The msg in FIFO 2 is =FIFO 2

*/