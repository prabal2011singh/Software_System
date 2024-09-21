/*
============================================================================
Name : 20b.c
Author : Prabal Singh
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 18th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/stat.h>

int main()
{
    char buf[100];
    
    printf("Enter the msg in FIFO =\n");
    scanf(" %[^\n]" , buf);
    printf("The msg in FIFO is =%s\n", buf);
    int fd=open("myfifo", O_WRONLY);  //blocking call that's why written below
    write(fd, buf, sizeof(buf));
    close(fd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 20b.c -o 20b
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./20b
Enter the msg in FIFO =
Hello
The msg in FIFO is =Hello

*/