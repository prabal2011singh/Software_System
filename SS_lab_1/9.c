/*
============================================================================
Name : 9.c
Author : Prabal Singh
Description : Write a program to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }
	
    int fd=open(argv[1],O_RDONLY);
    if(fd==-1)
    {
	    perror("open");
	    return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return 1;
    }

    printf("Inode: %ld\n", st.st_ino);
    printf("Number of hard links: %ld\n", st.st_nlink);
    printf("UID: %d\n", st.st_uid);
    printf("GID: %d\n", st.st_gid);
    printf("Size: %ld bytes\n", st.st_size);
    printf("Block size: %ld bytes\n", st.st_blksize);
    printf("Number of blocks: %ld\n", st.st_blocks);
    printf("Last access time: %s", ctime(&st.st_atime));
    printf("Last modification time: %s", ctime(&st.st_mtime));
    printf("Last change time: %s", ctime(&st.st_ctime));

    return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 1a.c
Inode: 3804760
Number of hard links: 1
UID: 1000
GID: 1000
Size: 623 bytes
Block size: 4096 bytes
Number of blocks: 8
Last access time: Sat Aug 31 17:30:48 2024
Last modification time: Sat Aug 31 17:30:48 2024
Last change time: Sat Aug 31 17:30:48 2024

*/

