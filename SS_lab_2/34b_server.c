/*
============================================================================
Name : 34b_server.c
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
#include <pthread.h>


void communicate(int *fd)
{

    ssize_t readBytes, writeBytes;
    char buf[100];

    writeBytes = write(*fd, "I'm the server!", 15);
    printf("Data sent to client!\n");

    readBytes = read(*fd, buf, 100);
    printf("Data from client: %s\n", buf);

    close(*fd);
}

int main()
{
    int sockfd, cntfd;
    int bindStatus;
    int listenStatus;
    int clientSize;

    struct sockaddr_in address, client;

    pthread_t threadID;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    bindStatus = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    printf("Binding to socket was successful!\n");

    listenStatus = listen(sockfd, 2);
    printf("Now listening for connections on a socket!\n");

    while (1)
    {
        clientSize = (int)sizeof(client);
        cntfd = accept(sockfd, (struct sockaddr *)&client, &clientSize);

        if (pthread_create(&threadID, NULL, (void *)communicate, &cntfd))
        perror("Error while creating thread");
        
    }

    close(sockfd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./34b_server 
Server side socket successfully created!
Binding to socket was successful!
Now listening for connections on a socket!
Data sent to client!
Data from client: I'm the client!
Data sent to client!
Data from client: I'm the client!
^C

*/