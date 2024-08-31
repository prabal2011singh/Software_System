/*
============================================================================
Name : 27c.c
Author : Prabal Singh
Description : Write a program to execute ls -Rl by the following system calls
c. execled
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("ls -Rl using execle comamnd\n");
    char *envp[]={NULL};
    execle("/bin/ls","ls","-Rl", NULL, envp);
    perror("execle");
    return 1;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
ls -Rl using execle comamnd
.:
total 200
-rwxrwxr-x 1 prabal-singh prabal-singh 16184 Aug 27 23:02 10
-rw-rw-r-- 1 prabal-singh prabal-singh  1001 Aug 30 20:20 10.c
-rw-rw-r-- 1 prabal-singh prabal-singh   662 Aug 28 18:09 11.c
-rw-rw-r-- 1 prabal-singh prabal-singh   717 Aug 28 18:46 12.c
-rw-rw-r-- 1 prabal-singh prabal-singh   615 Aug 28 22:01 13.c
-rw-rw-r-- 1 prabal-singh prabal-singh   737 Aug 28 22:32 14.c
-rw-rw-r-- 1 prabal-singh prabal-singh  4070 Aug 30 22:58 15.c
-rw-rw-r-- 1 prabal-singh prabal-singh   403 Aug 30 22:23 19.c
-rw-rw-r-- 1 prabal-singh prabal-singh   139 Aug  8 09:14 1a.c
-rw-rw-r-- 2 prabal-singh prabal-singh   138 Aug 30 18:12 1b.c
-rw-rw-r-- 1 prabal-singh prabal-singh   160 Aug  8 09:27 1c.c
-rw-rw-r-- 1 prabal-singh prabal-singh   309 Aug 30 11:03 20.c
-rwxrwxr-x 1 prabal-singh prabal-singh 16040 Aug 31 12:59 21
-rw-rw-r-- 1 prabal-singh prabal-singh   248 Aug 30 14:08 21.c
-rw-rw-r-- 1 prabal-singh prabal-singh   379 Aug 30 15:16 22.c
-rw-rw-r-- 1 prabal-singh prabal-singh  1315 Aug 30 17:48 23.c
-rw-rw-r-- 1 prabal-singh prabal-singh   882 Aug 30 18:00 24.c
-rw-rw-r-- 1 prabal-singh prabal-singh  1175 Aug 30 18:46 25.c
-rwxrwxr-x 1 prabal-singh prabal-singh 16056 Aug 31 12:16 26
-rw-rw-r-- 1 prabal-singh prabal-singh   526 Aug 31 12:17 26.c
-rw-rw-r-- 1 prabal-singh prabal-singh  2670 Aug 31 12:55 27a.c
-rw-rw-r-- 1 prabal-singh prabal-singh  2822 Aug 31 13:05 27b.c
-rw-rw-r-- 1 prabal-singh prabal-singh   295 Aug 31 13:10 27c.c
-rw-rw-r-- 1 prabal-singh prabal-singh   354 Aug 30 19:12 28.c
-rw-rw-r-- 1 prabal-singh prabal-singh  1344 Aug 30 21:24 29.c
-rw-rw-r-- 1 prabal-singh prabal-singh   186 Aug 30 11:23 3.c
-rw-rw-r-- 1 prabal-singh prabal-singh  2248 Aug 30 23:59 30.c
-rw-rw-r-- 1 prabal-singh prabal-singh   612 Aug 11 00:56 5.c
-rw-rw-r-- 1 prabal-singh prabal-singh  1161 Aug 11 01:59 7.c
-rw-rw-r-- 1 prabal-singh prabal-singh   653 Aug 11 00:33 7copy.c
-rw-rw-r-- 1 prabal-singh prabal-singh   596 Aug 27 19:21 8.c
-rw-rw-r-- 1 prabal-singh prabal-singh   954 Aug 11 01:35 9.c
-rwxrwxr-x 1 prabal-singh prabal-singh 16096 Aug 31 13:10 a.out
-rw-rw-r-- 2 prabal-singh prabal-singh   138 Aug 30 18:12 hdlink1bb
prw-rw-r-- 1 prabal-singh prabal-singh     0 Aug  8 08:47 myfifo
prw-rw-r-- 1 prabal-singh prabal-singh     0 Aug  8 09:29 myfifo1c
-rw-r--r-- 1 prabal-singh prabal-singh    30 Aug 30 20:20 new_file.txt
-rw-rw-r-- 1 prabal-singh prabal-singh    63 Aug 11 00:45 temp1.c
-rwxrwxr-x 1 prabal-singh prabal-singh    27 Aug 28 18:10 temp1.txt
-rw-rw-r-- 1 prabal-singh prabal-singh   383 Aug 30 23:15 test1.c

*/