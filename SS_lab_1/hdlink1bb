/*
============================================================================
Name : 1b.c
Author : Prabal Singh
Description : Create the following types of a files using (i) shell command (ii) system call
b. hard link (link system call)
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>

int main() {
	if(link("1b.c","hdlink1bb") ==-1){
		perror("link");
		return 1;
	}
	else
	printf("Hard link created\n");
	return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out
Hard link created

*/
