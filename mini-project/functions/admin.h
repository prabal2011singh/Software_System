#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

#include "./common.h"

// Function Prototypes =================================

bool admin_operation_handler(int connFD);
bool add_employee_account(int connFD);
bool modify_employee_info(int connFD);
bool admin_modify_customer_info(int connFD);
bool change_employee_to_manager(int connFD);

// =====================================================

// Function Definition =================================

bool admin_operation_handler(int connFD)
{
    if(login_admin_customer_handler(4, connFD, NULL))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from/written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Welcome superman!\n");

        while (1)
        {
            strcat(writeBuffer, "1. Add New Bank Employee \n2. Modify Customer Details \n3. Modify Employee Details \n4. Manage User Roles \n5. Change Password \n6. Logout\n");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));

            bzero(writeBuffer, sizeof(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_employee_account(connFD);
                break;
            case 2:
                admin_modify_customer_info(connFD);
                break;
            case 3:
                modify_employee_info(connFD);
                break;
            case 4:
                change_employee_to_manager(connFD);
                break;
            case 5:
                //change_password(connFD);
                break;
            default:
                writeBytes = write(connFD, "Logging you out now superman! Goodbye!$", strlen("Logging you out now superman! Goodbye!$"));
                return false;
            }
        }
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    return true;
}

bool change_employee_to_manager(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee employee;

    int employeeID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, "Enter the ID of the employee who's information you want to edit", strlen("Enter the ID of the employee who's information you want to edit"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    employeeID = atoi(readBuffer);

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Employee File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No employee could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "No employee could be found for the given ID^");
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
    // Locking record
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    readBytes = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    // Unlocking record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);
    close(employeeFileDescriptor);


    writeBytes = write(connFD, "What would you like to make him? \nEnter 1. To make him Employee 2. To make him Manager", strlen("What would you like to make him? \nEnter 1. To make him Employee 2. To make him Manager"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int choice = atoi(readBuffer);

    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number!\nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        employee.isMag=0;
        break;
    case 2:
        employee.isMag=1;
        break;
    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    writeBytes = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    close(employeeFileDescriptor);


    writeBytes = write(connFD, "The required modification is done!\nYou'll now be redirected to the main menu!^", strlen("The required modification is done!\nYou'll now be redirected to the main menu!^"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

bool add_employee_account(int connFD)   //working correct
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee newEmployee, prevEmployee;

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1 && errno == ENOENT)
    {
        // Account file was never created
        newEmployee.id = 0;
    }
    else
    {
        int offset = lseek(employeeFileDescriptor, -sizeof(struct Employee), SEEK_END);
        //applying lock
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
        int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);

        readBytes = read(employeeFileDescriptor, &prevEmployee, sizeof(struct Employee));
        //unlocking
        lock.l_type = F_UNLCK;
        fcntl(employeeFileDescriptor, F_SETLK, &lock);
        close(employeeFileDescriptor);

        newEmployee.id = prevEmployee.id + 1;
    }

    newEmployee.isMag = 0;

    sprintf(writeBuffer, "%s%s", "Enter the details for the employee\n", "What is the employee's name?");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    strcpy(newEmployee.name, readBuffer);


    writeBytes = write(connFD, "What is the employee's gender? \nEnter M for male, F for female", strlen("What is the employee's gender? \nEnter M for male, F for female"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBuffer[0] == 'M' || readBuffer[0] == 'F')
        newEmployee.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD, "It seems you've enter a wrong gender choice! \nYou'll now be redirected to the main menu!^", strlen("It seems you've enter a wrong gender choice! \nYou'll now be redirected to the main menu!^"));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "What is the employee's age?");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    int employeeAge = atoi(readBuffer);
    if (employeeAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid age!\nNow, you'll be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    newEmployee.age = employeeAge;


    strcpy(newEmployee.login, newEmployee.name);
    strcat(newEmployee.login, "-");
    sprintf(writeBuffer, "%d", newEmployee.id);
    strcat(newEmployee.login, writeBuffer);

    memset(newEmployee.loanID, -1, MAX_LOANS * sizeof(int));

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt("1234", SALT_BAE));
    strcpy(newEmployee.password, hashedPassword);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", "The login ID for the employee is : ", newEmployee.name, newEmployee.id, "The autogenerated password for the employee is : ", "1234");


    employeeFileDescriptor = open(EMPLOYEE_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    writeBytes = write(employeeFileDescriptor, &newEmployee, sizeof(struct Employee));
    close(employeeFileDescriptor);

    strcat(writeBuffer, "\nRedirecting you to the main menu ...^");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(read)); // Dummy read
    return true;
}

bool modify_employee_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee employee;
    int employeeID;
    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, "Enter the ID of the employee who's information you want to edit", strlen("Enter the ID of the employee who's information you want to edit"));

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    employeeID = atoi(readBuffer);

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Employee File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        strcpy(writeBuffer, "No employee could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        strcpy(writeBuffer, "No employee could be found for the given ID^");
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
    // Locking record
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    readBytes = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    // Unlocking record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);
    close(employeeFileDescriptor);

    bzero(readBuffer, sizeof(readBuffer));
    writeBytes = write(connFD, "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel", strlen("Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int choice = atoi(readBuffer);

    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        strcpy(writeBuffer, "You have entered an invalid number!\nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, "What's the updated value for name?", strlen("What's the updated value for name?"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        strcpy(employee.name, readBuffer);
        break;

    case 2:
        writeBytes = write(connFD, "What's the updated value for age?", strlen("What's the updated value for age?"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        employee.age = updatedAge;
        break;

    case 3:
        writeBytes = write(connFD, "What's the updated value for gender?M for Male and F for Female", strlen("What's the updated value for gender?M for Male and F for Female"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        employee.gender = readBuffer[0];
        break;

    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    writeBytes = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    close(employeeFileDescriptor);


    writeBytes = write(connFD, "The required modification was successfully made!\nYou'll now be redirected to the main menu!^", strlen("The required modification was successfully made!\nYou'll now be redirected to the main menu!^"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

bool admin_modify_customer_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;
    int customerID;
    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, "Enter the ID of the customer who's information you want to edit", strlen("Enter the ID of the customer who's information you want to edit"));
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    customerID = atoi(readBuffer);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        strcpy(writeBuffer, "No customer could be found for the given ID^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
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


    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    // Locking record
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    // Unlocking record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    bzero(readBuffer, sizeof(readBuffer));
    writeBytes = write(connFD, "Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel", strlen("Which information would you like to modify?\n1. Name 2. Age 3. Gender \nPress any other key to cancel"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));

    int choice = atoi(readBuffer);

    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number!\nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
        writeBytes = write(connFD, "What's the updated value for name?", strlen("What's the updated value for name?"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        strcpy(customer.name, readBuffer);
        break;

    case 2:
        writeBytes = write(connFD, "What's the updated value for age?", strlen("What's the updated value for age?"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeBuffer, sizeof(writeBuffer));
            bzero(readBuffer, sizeof(readBuffer));
            strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        customer.age = updatedAge;
        break;

    case 3:
        writeBytes = write(connFD, "What's the updated value for gender?", strlen("What's the updated value for gender?"));
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        customer.gender = readBuffer[0];
        break;

    default:
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "You have entered an invalid number! \nYou'll now be redirected to the main menu!^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }


    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);
    close(customerFileDescriptor);


    writeBytes = write(connFD, "The required modification was successfully made!\nYou'll now be redirected to the main menu!^", strlen("The required modification was successfully made!\nYou'll now be redirected to the main menu!^"));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

#endif