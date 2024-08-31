/*
============================================================================
Name : 1a.c
Author : Prabal Singh
Description : Create the following types of a files using (i) shell command (ii) system call
a. soft link (symlink system call)
Date: 31th Aug, 2024.
============================================================================
*/

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
/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
soft link created for 1a.c

*/
