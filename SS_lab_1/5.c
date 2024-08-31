/*
============================================================================
Name : 5.c
Author : Prabal Singh
Description : Write a program to create five new files with infinite loop. Execute the program in the background
and check the file descriptor table at /proc/pid/fd.
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char filename[7];
    for (;;) {
        for (int i = 0; i < 5; i++) {
            snprintf(filename, sizeof(filename), "file_%d", i);
            fd = open(filename, O_CREAT , 0644);
            close(fd);
        }
        sleep(1);
    }
    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out &
[1] 33310
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ls /proc/33310/fd
0  1  103  2  37  38  39  40
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ll /proc/33310/fd
total 0
dr-x------ 2 prabal-singh prabal-singh  8 Aug 31 18:46 ./
dr-xr-xr-x 9 prabal-singh prabal-singh  0 Aug 31 18:45 ../
lrwx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 0 -> /dev/pts/2
lrwx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 1 -> /dev/pts/2
lr-x------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 103 -> /usr/share/code/v8_context_snapshot.bin
lrwx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 2 -> /dev/pts/2
lr-x------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 37 -> /usr/share/code/resources/app/node_modules.asar
l-wx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 38 -> /home/prabal-singh/.config/Code/logs/20240831T093023/ptyhost.log
lrwx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 39 -> 'socket:[54876]'
lrwx------ 1 prabal-singh prabal-singh 64 Aug 31 18:46 40 -> /dev/ptmx
*/

