/*
============================================================================
Name : 21b.c
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

    int fd=open("myfifo", O_RDONLY);
    read(fd, buf, sizeof(buf));
    printf("The msg in FIFO 1 is =%s\n", buf);
    close(fd);

    printf("Enter the msg in FIFO 2 =\n");
    scanf(" %[^\n]" , buf2);
    int fd2=open("myfifo2", O_WRONLY);  //blocking call that's why written below
    write(fd2, buf2, sizeof(buf2));
    close(fd2);
    
    
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 21b.c -o 21b
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./21b
The msg in FIFO 1 is =FIFO 1
Enter the msg in FIFO 2 =
FIFO 2

*/