#include <stdio.h> // Import for `printf` & `perror` functions
#include <errno.h> // Import for `errno` variable

#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture

#include <string.h>  // Import for string functions
#include <stdbool.h> // Import for `bool` data type
#include <stdlib.h>  // Import for `atoi` function

#include "./functions/server-constants.h"
#include "./functions/admin.h"
#include "./functions/customer.h"
#include "./functions/manager.h"
#include "./functions/employee.h"

void connection_handler(int connFD); // Handles the communication with the client

void main()
{
    int socketFileDescriptor, socketBindStatus, socketListenStatus, connectionFileDescriptor;
    struct sockaddr_in serverAddress, clientAddress;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(8081);              // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    socketBindStatus = bind(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    socketListenStatus = listen(socketFileDescriptor, 10);

    int clientSize;
    while (1)
    {
        clientSize = (int)sizeof(clientAddress);
        connectionFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *)&clientAddress, &clientSize);
        if (!fork())
        {
            // Child will enter this branch
            connection_handler(connectionFileDescriptor);
            close(connectionFileDescriptor);
            _exit(0);
        }
    }
    close(socketFileDescriptor);
}

void connection_handler(int connectionFileDescriptor)
{
    printf("Client has connected to the server!\n");

    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    int userChoice;

    writeBytes = write(connectionFileDescriptor, "Enter 1 for Customer,\n 2 for Bank Employee,\n 3 for Manager,\n 4 for Admin,\n Enter any other number to exit!", strlen("Enter 1 for Customer,\n 2 for Bank Employee,\n 3 for Manager,\n 4 for Admin,\n Enter any other number to exit!"));
    if (writeBytes == -1)
        perror("Error while sending first prompt to the user!");
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFileDescriptor, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
            perror("Error while reading from client");
        else if (readBytes == 0)
            printf("No data was sent by the client");
        else
        {
            userChoice = atoi(readBuffer);
            switch (userChoice)
            {
            case 1:
                // Customer
                customer_operation_handler(connectionFileDescriptor);
                break;
            case 2:
                // Employee
                employee_operation_handler(connectionFileDescriptor);
                break;
            case 3:
                // Manager
                manager_operation_handler(connectionFileDescriptor);
                break;
            case 4:
                // Admin
                admin_operation_handler(connectionFileDescriptor);
                break;
            default:
                // Exit
                break;
            }
        }
    }
    printf("Terminating connection to client!\n");
}
