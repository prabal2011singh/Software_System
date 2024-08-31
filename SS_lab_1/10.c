/*
============================================================================
Name : 10.c
Author : Prabal Singh
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[10]="abcdefghij";

	if(argc!=2)
	{
		perror("argc");
		return 1;
	}

	fd=open(argv[1], O_RDWR | O_CREAT, 0777);
	if(fd==-1)
	{
		perror("open");
		close(fd);
		return 1;
	}

	if(write(fd,buf,10) != 10)
	{
		perror("write1");
		close(fd);
		return 1;
	}

	int value_lseek=lseek(fd, 10, SEEK_CUR);
	if(value_lseek==-1)
	{
		perror("lseek");
		close(fd);
		return 1;
	}
	else
		printf("the off_set location of lseek is %d\n",value_lseek);

	if(write(fd, buf, 10)!=10)
	{
		perror("write2");
		close(fd);
		return 1;
	}

	close(fd);
	
	return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out new_file.txt
the off_set location of lseek is 20
prabal-singh@prabal-singh-Vostro-5415:~/temp$ od -c new_file.txt
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0   a   b   c   d   e   f   g   h   i   j
0000036

*/
