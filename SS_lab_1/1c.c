/*
============================================================================
Name : 1c.c
Author : Prabal Singh
Description : Create the following types of a files using (i) shell command (ii) system call
c. FIFO (mkfifo Library Function or mknod system call)
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main() {
	if(mkfifo("myfifo1c", 0666)==-1) {
		perror("mkfifo");
		return 1;
	}
	else
	printf("Fifo file created\n");
	return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Fifo file created

*/



