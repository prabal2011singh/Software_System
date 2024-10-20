#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

#include <stdio.h>     // Import for `printf` & `perror`
#include <unistd.h>    // Import for `read`, `write & `lseek`
#include <string.h>    // Import for string functions
#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`

#include "../record-struct/account.h"
#include "../record-struct/customer.h"
#include "../record-struct/transaction.h"
#include "../record-struct/loan_struct.h"
#include "../record-struct/employee-struct.h"
#include "../record-struct/feedback.h"
#include "./admin-credentials.h"
#include "./server-constants.h"


// Function Prototypes =================================

bool login_admin_customer_handler(int role, int connFD, struct Customer *ptrToCustomer);
bool login_employee_manager_handler(int role, int connFD, struct Employee *ptrToEmployeeID);
bool get_account_details(int connFD, struct Account *customerAccount);
bool get_customer_details(int connFD, int customerID);
bool get_transaction_details(int connFD, int accountNumber);
bool get_employee_details(int connFD, struct Employee *employeeAccount);
// =====================================================

// Function Definition =================================

bool login_admin_customer_handler(int role, int connFD, struct Customer *ptrToCustomerID)
{
    ssize_t readBytes, writeBytes;            // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000]; // Buffer for reading from / writing to the client
    char tempBuffer[1000];
    struct Customer customer;

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Get login message for respective user type
    
    if(role==1)
        strcpy(writeBuffer, "Welcome dear customer! Enter your credentials of your account!\n");
    else if (role==4)
        strcpy(writeBuffer, "Welcome dear Admin! Enter your credentials!\n");

    // Append the request for LOGIN ID message
    strcat(writeBuffer, "Enter your login ID");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    bool userFound = false;

    if (role==4)
    {
        if (strcmp(readBuffer, ADMIN_LOGIN_ID) == 0)
            userFound = true;
    }
    else if(role==1)
    {
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
        strtok(tempBuffer, "-");
        ID = atoi(strtok(NULL, "-"));

        int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);

        off_t offset = lseek(customerFileFD, ID * sizeof(struct Customer), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Customer), sizeof(struct Customer), getpid()};
            int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
            readBytes = read(customerFileFD, &customer, sizeof(struct Customer));

            lock.l_type = F_UNLCK;
            fcntl(customerFileFD, F_SETLK, &lock);

            if (strcmp(customer.login, readBuffer) == 0)
                userFound = true;

            close(customerFileFD);
        }
        else
        {
            writeBytes = write(connFD, "No customer could be found for the given login ID$", strlen("No customer could be found for the given login ID$"));
        }
    }

    if(userFound)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "Enter your password \n# ", strlen("Enter your password \n# "));

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));

        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT_BAE));

        if (role==4)
        {
            if (strcmp(hashedPassword, ADMIN_PASSWORD) == 0)
                return true;
        }
        else if(role==1)
        {
            if (strcmp(hashedPassword, customer.password) == 0)
            {
                *ptrToCustomerID = customer;
                return true;
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "The password specified doesn't match!$", strlen("The password specified doesn't match!$"));
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "The login ID specified doesn't exist!$", strlen("The login ID specified doesn't exist!$"));
    }

    return false;
}

bool login_employee_manager_handler(int role, int connFD, struct Employee *ptrToEmployeeID)
{
    ssize_t readBytes, writeBytes;            // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000]; // Buffer for reading from / writing to the client
    char tempBuffer[1000];
    struct Employee employee;

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Get login message for respective user type
    
    if(role==2)
        strcpy(writeBuffer, "Welcome dear Employee! Enter your credentials of your account!\n");
    else if(role==3)
        strcpy(writeBuffer, "Welcome dear Manager! Enter your credentials!\n");

    // Append the request for LOGIN ID message
    strcat(writeBuffer, "Enter your login ID");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    bool userFound = false;

    if(role==2 || role==3)
    {
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
        strtok(tempBuffer, "-");
        ID = atoi(strtok(NULL, "-"));

        int employeeFileFD = open(EMPLOYEE_FILE, O_RDONLY);

        off_t offset = lseek(employeeFileFD, ID * sizeof(struct Employee), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};
            int lockingStatus = fcntl(employeeFileFD, F_SETLKW, &lock);
            readBytes = read(employeeFileFD, &employee, sizeof(struct Employee));
            lock.l_type = F_UNLCK;
            fcntl(employeeFileFD, F_SETLK, &lock);

            if(role==3 && employee.isMag==0)
            {
                writeBytes = write(connFD, "No manager could be found for the given login ID $", strlen("No manager could be found for the given login ID! $"));
                return false;
            }
            else if(role==2 && employee.isMag==1)
            {
                writeBytes = write(connFD, "You are manager. Please login in Manager portal $", strlen("You are manager. Please login in Manager portal! $"));
                return false;
            }
            if (strcmp(employee.login, readBuffer) == 0)
                userFound = true;

            close(employeeFileFD);
        }
        else
        {
            writeBytes = write(connFD, "No employee could be found for the given login ID$", strlen("No employee could be found for the given login ID$"));
        }
    }

    if(userFound)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "Enter your password \n# ", strlen("Enter your password \n# "));

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));

        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT_BAE));

        if (strcmp(hashedPassword, employee.password) == 0)
        {
            *ptrToEmployeeID = employee;
            return true;
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "The password specified doesn't match!$", strlen("The password specified doesn't match!$"));
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "The login ID specified doesn't exist!$", strlen("The login ID specified doesn't exist!$"));
    }

    return false;
}

bool get_account_details(int connFD, struct Account *customerAccount)
{
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[1000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    int accountNumber;
    struct Account account;
    int accountFileDescriptor;

    if (customerAccount == NULL)
    {
        writeBytes = write(connFD, "Enter the account number of the account you're searching for", strlen("Enter the account number of the account you're searching for"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        accountNumber = atoi(readBuffer);
    }
    else
        accountNumber = customerAccount->accountNumber;


    accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);

    if (accountFileDescriptor == -1)
    {
        // Account file doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No file for account records^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    int offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1 && errno == EINVAL)
    {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No account could be found for the given account number^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required account record!");
        return false;
    }


    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
    fcntl(accountFileDescriptor, F_SETLKW, &lock);
    readBytes = read(accountFileDescriptor, &account, sizeof(struct Account));
    lock.l_type = F_UNLCK;
    fcntl(accountFileDescriptor, F_SETLK, &lock);

    if (customerAccount != NULL)
    {
        *customerAccount = account;
        return true;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Account Details - \n\tAccount Number : %d\n\tAccount Type : %s\n\tAccount Status : %s", account.accountNumber, (account.isRegularAccount ? "Regular" : "Joint"), (account.active) ? "Active" : "Deactived");
    if (account.active)
    {
        sprintf(tempBuffer, "\n\tAccount Balance:₹ %ld", account.balance);
        strcat(writeBuffer, tempBuffer);
    }

    sprintf(tempBuffer, "\n\tOwner ID: %d", account.owner);
    strcat(writeBuffer, tempBuffer);
    strcat(writeBuffer, "\n^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}

bool get_customer_details(int connFD, int customerID)
{
    ssize_t readBytes, writeBytes;             // Number of bytes read from/written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from/writing to the socket
    char tempBuffer[1000];

    struct Customer customer;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct Account), getpid()};

    if (customerID == -1)
    {
        writeBytes = write(connFD, "Enter the customer ID of the customer you're searching for", strlen("Enter the customer ID of the customer you're searching for"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        customerID = atoi(readBuffer);
    }

    customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No customer could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    int offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No customer could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_start = offset;
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Customer Details - \n\tID : %d\n\tName : %s\n\tGender : %c\n\tAge: %d\n\tAccount Number : %d\n\tLoginID : %s", customer.id, customer.name, customer.gender, customer.age, customer.account, customer.login);
    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}

bool get_transaction_details(int connFD, int accountNumber)
{

    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[20000], tempBuffer[1000]; // A buffer for reading from / writing to the socket

    struct Account account;

    if (accountNumber == -1)
    {
        // Get the accountNumber
        writeBytes = write(connFD, "Enter the account number of the account you're searching for", strlen("Enter the account number of the account you're searching for"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        account.accountNumber = atoi(readBuffer);
    }
    else
        account.accountNumber = accountNumber;

    if (get_account_details(connFD, &account))
    {
        int iter;

        struct Transaction transaction;
        struct tm transactionTime;

        bzero(writeBuffer, sizeof(writeBuffer));

        int transactionFileDescriptor = open(TRANSACTION_FILE, O_RDONLY);
        if (transactionFileDescriptor == -1)
        {
            perror("Error while opening transaction file!");
            write(connFD, "No transactions were performed on this account!^", strlen("No transactions were performed on this account!^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }

        for (iter = 0; iter < MAX_TRANSACTIONS && account.transactions[iter] != -1; iter++)
        {
            int offset = lseek(transactionFileDescriptor, account.transactions[iter] * sizeof(struct Transaction), SEEK_SET);
            if (offset == -1)
            {
                perror("Error while seeking to required transaction record!");
                return false;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Transaction), getpid()};
            int lockingStatus = fcntl(transactionFileDescriptor, F_SETLKW, &lock);
            readBytes = read(transactionFileDescriptor, &transaction, sizeof(struct Transaction));
            lock.l_type = F_UNLCK;
            fcntl(transactionFileDescriptor, F_SETLK, &lock);

            transactionTime = *localtime(&(transaction.transactionTime));

            bzero(tempBuffer, sizeof(tempBuffer));
            sprintf(tempBuffer, "Details of transaction %d - \n\t Date : %d:%d %d/%d/%d \n\t Operation : %s \n\t Balance - \n\t\t Before : %ld \n\t\t After : %ld \n\t\t Difference : %ld\n", (iter + 1), transactionTime.tm_hour, transactionTime.tm_min, transactionTime.tm_mday, (transactionTime.tm_mon + 1), (transactionTime.tm_year + 1900), (transaction.operation ? "Deposit" : "Withdraw"), transaction.oldBalance, transaction.newBalance, (transaction.newBalance - transaction.oldBalance));

            if (strlen(writeBuffer) == 0)
                strcpy(writeBuffer, tempBuffer); //first transaction so copy it
            else
                strcat(writeBuffer, tempBuffer);  // adding more transactions in it
        }

        close(transactionFileDescriptor);

        if (strlen(writeBuffer) == 0)
        {
            write(connFD, "No transactions were performed on this account by the customer!^", strlen("No transactions were performed on this account by the customer!^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        else
        {
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
}

bool get_employee_details(int connFD, struct Employee *employeeAccount)
{
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[1000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    int employeeNumber;
    struct Employee employee;
    int employeeFileDescriptor;

    if (employeeAccount == NULL)
    {
        writeBytes = write(connFD, "Enter the employee number of the employee you're searching for", strlen("Enter the employee number of the employee you're searching for"));
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        employeeNumber = atoi(readBuffer);
    }
    else
        employeeNumber = employeeAccount->id;


    employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);

    if (employeeFileDescriptor == -1)
    {
        // Employee file doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No file for employee records^");
        perror("Error opening account file in get_employee_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    int offset = lseek(employeeFileDescriptor, employeeNumber * sizeof(struct Employee), SEEK_SET);
    if (offset == -1 && errno == EINVAL)
    {
        // Employee record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No employee could be found for the given employee number^");
        perror("Error seeking to employee record in get_employee_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }


    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    readBytes = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    if (employeeAccount != NULL)
    {
        *employeeAccount = employee;
        return true;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Employee Details - \n\tEmployee ID : %d\n\tName : %s\n\t Is Manager : %s", employee.id, employee.name , (employee.isMag) ? "True" : "False");
    strcat(writeBuffer, "\n^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}


// =====================================================

#endif