/*
============================================================================
Name : 33b.c
Author : Prabal Singh
Description : Write a program to communicate between two machines using socket.
Date: 21th Sept, 2024.
============================================================================
*/

#include <stdio.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 

//server

int  main()
{
    int sockfd, cntfd;  
    int binds;
    int lists;
    int clientsize;

    struct sockaddr_in add, client;

    ssize_t rbyte, wbyte;
    char buf[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server side socket successfully created!\n");

    add.sin_addr.s_addr = htonl(INADDR_ANY);
    add.sin_family = AF_INET;
    add.sin_port = htons(8080);

    binds = bind(sockfd, (struct sockaddr *)&add, sizeof(add));
    printf("Binding to socket was successful!\n");

    lists = listen(sockfd, 2);
    printf("Now listening for connections on a socket!\n");

    clientsize = (int)sizeof(client);
    cntfd = accept(sockfd, (struct sockaddr *)&client, &clientsize);

    wbyte = write(cntfd, "I'm the server!", 15);
    printf("Data sent to client!\n");

    rbyte = read(cntfd, buf, 100);
    printf("Data from client: %s\n", buf);

    close(cntfd);
    close(sockfd);
    return 0;
}

/*

Output

prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ cc 33b.c -o 33b
prabal-singh@prabal-singh-Vostro-5415:~/SS Exercises/SS_lab_2$ ./33b
Server side socket successfully created!
Binding to socket was successful!
Now listening for connections on a socket!
Data sent to client!
Data from client: msg from client!

*/