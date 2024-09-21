/*
============================================================================
Name : 33a.c
Author : Prabal Singh
Description : Write a program to communicate between two machines using socket.
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <stdio.h> 
#include <unistd.h>  

//client

int main()
{
    int sockfd;
    int cnts; 

    struct sockaddr_in add;

    ssize_t rbyte, wbyte;
    char buf[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Client socket created!\n");

    add.sin_addr.s_addr = htonl(INADDR_ANY);
    add.sin_family = AF_INET;
    add.sin_port = htons(8080);

    cnts = connect(sockfd, (struct sockaddr *)&add, sizeof(add));

    printf("Client to server connection established!\n");

    rbyte = read(sockfd, buf, 100);
    printf("Data from server: %s\n", buf);

    wbyte = write(sockfd, "msg from client!", 16);
    printf("Data sent to server!");

    close(sockfd);
    return 0;

}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 33a.c -o 33a
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./33a
Client socket created!
Client to server connection established!
Data from server: I'm the server!
Data sent to server!

 */