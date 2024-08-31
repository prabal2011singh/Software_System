/*
============================================================================
Name : 8.c
Author : Prabal Singh
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read.
Close the file when end of file is reached.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char ch;
    int c;
    while ((c = read(fd, &ch, 1)) > 0) {
        write(1, &ch, 1);
        if (ch == '\n') {
            getchar(); // Wait for user to press enter
        }
    }

    if (c == -1) {
        perror("read");
    }

    close(fd);

    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 1a.c

#include<stdio.h>

#include<unistd.h>



int main() {

        if(symlink("1a.c","soft1aa")==-1) {

                perror("symlink");

                return 1;

        }

        else

        printf("soft link created for 1a.c");

        return 0;

}

*/