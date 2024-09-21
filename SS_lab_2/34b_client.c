/*
============================================================================
Name : 34b_client.c
Author : Prabal Singh
Description :  Write a program to create a concurrent server.
 b. use pthread_create
Date: 21th Sept, 2024.
============================================================================
*/

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <stdio.h> 
#include <unistd.h> 

int main()
{
    int sockfd; 
    int connectStatus;  

    struct sockaddr_in address; 

    ssize_t readBytes, writeBytes; 
    char buf[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Client side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    connectStatus = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    printf("Client to serrver connection successfully established!\n");

    readBytes = read(sockfd, buf, 100);
    printf("Data from server: %s\n", buf);

    writeBytes = write(sockfd, "I'm the client!", 15);
    printf("Data sent to server!");

    getchar();
    
    close(sockfd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./34b_client 
Client side socket successfully created!
Client to serrver connection successfully established!
Data from server: I'm the server!
Data sent to server!
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./34b_client 
Client side socket successfully created!
Client to serrver connection successfully established!
Data from server: I'm the server!
Data sent to server!

*/