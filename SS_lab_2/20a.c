/*
============================================================================
Name : 20a.c
Author : Prabal Singh
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 18th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buf[100];
    int fd=open("myfifo", O_RDONLY);
    read(fd, buf, sizeof(buf));
    printf("The msg in FIFO is= %s\n",buf);
    close(fd);
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 20a.c -o 20a
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./20a
The msg in FIFO is= Hello

*/