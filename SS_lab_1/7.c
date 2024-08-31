/*
============================================================================
Name : 7.c
Author : Prabal Singh
Description : Write a program to copy file1 into file2 ($cp file1 file2).
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>

#define BUFFER_SIZE 4096

int main(int arg, char *argv[]) {
	if(arg!= 3) {
		fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
		return 1;
	}

	int s_fd = open(argv[1], O_RDONLY);
	    if (s_fd == -1) {
       		 perror("Error opening source file");
       		 return 1;
    	}

    	int d_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (d_fd == -1) {
        	perror("Error opening destination file");
        	close(s_fd);
        	return 1;
        }

	 char buffer[BUFFER_SIZE];
    	 ssize_t bytes_read, bytes_written;

   	 while ((bytes_read = read(s_fd, buffer, BUFFER_SIZE)) > 0) {
       		 bytes_written = write(d_fd, buffer, bytes_read);
       		 if (bytes_written != bytes_read) {
            		perror("write");  
			 close(s_fd);
           		 close(d_fd);
           		 return 1;
       		 }
   	 }

   	 if (bytes_read == -1) {
       		 perror("read");
       		 close(s_fd);
       		 close(d_fd);
       		 return 1;
   	 }

   	 close(s_fd);
   	 close(d_fd);

 	   return 0;
}

/*

Output
prabal-singh@prabal-singh-Vostro-5415:~/temp$ cc 7.c
prabal-singh@prabal-singh-Vostro-5415:~/temp$ ./a.out 7.c 7copy.c

*/